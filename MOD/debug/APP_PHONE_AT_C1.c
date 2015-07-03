/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_AT_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application
*-----------------------------------------------------------------------------
* Description:    Handles all call specific responses from   
                  AT Cmd Parser 
*-----------------------------------------------------------------------------
* $Date: 2010-11-30 08:15:36 +0100 (Di, 30 Nov 2010) $
* $Rev: 16582 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONE_AT_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_types.h"



/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONE.h"
#include "POOL.h"

//#include "ATCmdHandler.h"
#include "ATParser.h"
#include "APP_PHONE_CI.h"
#include "APP_PHONE_AT.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_COMMON_Utils.h"

#include "APP_PHONE_AT_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

extern AppPhoneContext gAppPhoneContext;

// SUSANNE: 05.10.2009: This was added by Thorsten. It is initialized in ATCmdHandler_Phone.c
// and shall avoid the UHV to evaluate the PATR:0 event sent by Ck5050 after call has been hung up.
// This does not fix the problem at all, cause call could also be terminated by phone or distant phone
// TODO: Parrot must handle order of PLCC with call state 6 and PATR:0 event .....  
//extern BooleanType ignoreLastAudioTransferMode;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
//static SuccessFailType 
//_APP_PHONE_AT_sfHandleMicMuteMode(ATRspMicroMuteType* pmmu);

//static SuccessFailType 
//_APP_PHONE_AT_sfHandleHandsFreeMode(ATRspAudioTransferType* patr);

static SuccessFailType _APP_PHONE_AT_sfHandlePhoneStatusEvent(const ATRspPhoneStatusIndEventType *pPiev);
static SuccessFailType _APP_PHONE_AT_sfHandlePhoneStatus(const ATRspPhoneStatusIndValueType *pindVal);
static SuccessFailType _APP_PHONE_AT_sfHandleMicMute(const ATRspMicroMuteType *pPmmu);
static SuccessFailType _APP_PHONE_AT_sfHandleAudioTransDisc(const ATRspAudioTransferType *pPatr);
static SuccessFailType _APP_PHONE_AT_sfHandleClassOfDevice(const ATRspClassOfDeviceRequestType *pPbcd);
//static void _APP_PHONE_AT_vUpdateValues(ATRspAllType* rsp);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vHandleATResponse                                */
/** 
    \brief      handle response msgs from AT Cmd Parser 
  
    \brief      Description:\n
                handles responses from AT Cmd Parser \n

    \param      atRsp 
                ATRspMessageType *
                pointer to AT response
    
    \return     void 
   
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vHandleATResponse(const ATRspMessageType *atRsp)
{

    // check the message 
    if (AT_RSP_WITH_DATA == atRsp->dataInd && NULL == atRsp->data )
    {
        // seems to be something wrong with the message, ignore it
        return;
    }
    else if (   AT_RSP_WITHOUT_DATA == atRsp->dataInd &&
                NULL != atRsp->data )
    {
        return;
    }

    switch( atRsp->common.rsp_type )
    {
        /* PNCC and PLCC are needed to decide if call management functions are
        * finished */
        case AT_RSP_CALL_STATUS_IND_COUNT:          /* PNCC */
            (void)_APP_PHONE_AT_PNCC_sfHandleCallStatusCount(&atRsp->data->pncc);
            break;
        case AT_RSP_CALL_STATUS_IND_INDEX:          /* PLCC */
            (void)_APP_PHONE_AT_PNCC_sfHandleCallStatusIndex(&atRsp->data->plcc); 
            break;
        case AT_RSP_PHONE_STATUS_IND_VAL:           /* PIND */
            (void)_APP_PHONE_AT_sfHandlePhoneStatus(&atRsp->data->pind_val);
            break;
        case AT_RSP_PHONE_STATUS_IND_EVT:           /* PIEV */
            (void)_APP_PHONE_AT_sfHandlePhoneStatusEvent(&atRsp->data->piev);
            break;
        case AT_RSP_AMGMT_MIC_MUTE:
            (void)_APP_PHONE_AT_sfHandleMicMute(&atRsp->data->pmmu);
            break;
        case AT_RSP_AUDIO_TRANS_DISCRETE:
            (void)_APP_PHONE_AT_sfHandleAudioTransDisc(&atRsp->data->patr);
            break;
        case AT_RSP_CLASS_DEVICE_REQUEST:
            (void)_APP_PHONE_AT_sfHandleClassOfDevice(&atRsp->data->pbcd);
            break;
        case AT_RSP_PHONE_STATUS_IND:
        default:
            /* not supported */
            break;
    } 

}/* END OF APP_PHONE_vHandleATResponse() */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_sfHandlePhoneStatus                              */
/** 
    \brief      handle phone status response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles phone status response msgs from AT Cmd Parser \n

    \param      pindVal 
                ATRspPhoneStatusIndValueType *
                pointer to AT response
    
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_sfHandlePhoneStatus(const ATRspPhoneStatusIndValueType *pindVal)
{
    Uint16Type opNameLen = 0U;

    // save signalStrength and serviceSupport
    gAppPhoneContext.piev_signalStrength = pindVal->signal_level;
    gAppPhoneContext.piev_serviceSupport = pindVal->service;
        
    _APP_PHONE_vNotifyApp(APP_PHONE_NW_SIGSTRENGHT_CHANGED);
    
    // reset operator name
    (void)memset( &gAppPhoneContext.piev_networkProvider, 0, 
            sizeof( gAppPhoneContext.piev_networkProvider));
 
    // new size of opName
    opNameLen = pindVal->operator_name_len;
    opNameLen = ( opNameLen > sizeof(gAppPhoneContext.piev_networkProvider.name) - 1) ?
                  sizeof(gAppPhoneContext.piev_networkProvider.name)-1 : opNameLen;   

    // save new operator name
    (void)memcpy( gAppPhoneContext.piev_networkProvider.name, 
            pindVal->operator_name, opNameLen); 
    // save the length of the operator name
    gAppPhoneContext.piev_networkProvider.name_len = opNameLen;
    
    _APP_PHONE_vNotifyApp(APP_PHONE_OPERATOR_NAME_CHANGED);

    // save battery level
    gAppPhoneContext.piev_batteryLevel = pindVal->battery_level;
    _APP_PHONE_vNotifyApp(APP_PHONE_BATT_LEVEL_CHANGED);

    // command is not an unsolicited event. Requesting app will be notified via
    // its given callback when AT OK / ERROR arrives

    return SUCCESS;

}/* END OF _APP_PHONE_vHandlePhoneStatus() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_sfHandlePhoneStatusEvent                       */
/** 
    \brief      handle phone status event response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles phone status event response msgs from AT Cmd Parser \n

    \param      pPiev 
                ATRspPhoneStatusIndEventType *
                pointer to AT response
    
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_sfHandlePhoneStatusEvent(const ATRspPhoneStatusIndEventType *pPiev)
{
    // check for PIEV number and value 
    switch( pPiev->index )
    {
        case AT_INDI_OPERATOR_NAME:
        {
            Uint16Type opNameLen = 0;

            // network provider
            if((gAppPhoneContext.piev_networkProvider.name_len == pPiev->len) && 
               (0 == memcmp(pPiev->value_str, gAppPhoneContext.piev_networkProvider.name, opNameLen)))
            {
                DEBUG_STRING(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "Received alr. known Provider: %s", 
                             (const char *)gAppPhoneContext.piev_networkProvider.name);
                return SUCCESS;
            }

            /* else copy new provider and notify it towards applications */
            (void)memset( &gAppPhoneContext.piev_networkProvider, 0, 
                    sizeof( gAppPhoneContext.piev_networkProvider)); 

            opNameLen = pPiev->len;
            opNameLen = ( opNameLen > sizeof(gAppPhoneContext.piev_networkProvider.name)- 1) ?
                        sizeof(gAppPhoneContext.piev_networkProvider.name)-1 : opNameLen;   
 
            (void)memcpy( gAppPhoneContext.piev_networkProvider.name,  
                    pPiev->value_str, opNameLen); 

            gAppPhoneContext.piev_networkProvider.name_len = opNameLen;

            _APP_PHONE_vNotifyApp(APP_PHONE_OPERATOR_NAME_CHANGED);
            break;
        }
        case AT_INDI_NW_SIGSTRENGHT:
            // signal quality 
            gAppPhoneContext.piev_signalStrength = pPiev->value;
            _APP_PHONE_vNotifyApp(APP_PHONE_NW_SIGSTRENGHT_CHANGED);
            break;

        case AT_INDI_ROAMING:
            gAppPhoneContext.piev_Roaming = pPiev->value;     
            _APP_PHONE_vNotifyApp(APP_PHONE_ROAMING_CHANGED);
            break;

        case AT_INDI_NW_SERV_AVAIL:
            gAppPhoneContext.piev_serviceSupport = pPiev->value;     
            _APP_PHONE_vNotifyApp(APP_PHONE_SERV_AVAIL_CHANGED);
            break;
    
        case AT_INDI_BATT_LEVEL:
            // save battery level
            gAppPhoneContext.piev_batteryLevel = pPiev->value;
            _APP_PHONE_vNotifyApp(APP_PHONE_BATT_LEVEL_CHANGED);
            break;

        default:
            break;
    }

    return SUCCESS;

}/* END OF _APP_PHONE_vHandlePhoneStatus() */




/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_sfHandleMicMute                                */
/** 
    \brief      handle phone status event response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles phone status event response msgs from AT Cmd Parser \n

    \param      pPmmu 
                ATRspMicroMuteType *
                pointer to AT response
    
    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_sfHandleMicMute(const ATRspMicroMuteType *pPmmu)
{
    switch(pPmmu->mode) 
    {
        case AT_MIC_MUTE:
            gAppPhoneContext.isMicMuted = BT_TRUE;
            break;
        case AT_MIC_UNMUTE:
            gAppPhoneContext.isMicMuted = BT_FALSE;
            break;
        default:
        
            return FAIL;
    }

    _APP_PHONE_vNotifyApp(APP_PHONE_MIC_MUTE_CHANGED);

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_sfHandleAudioTransDisc                         */
/** 
    \brief      handle phone status event response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles phone status event response msgs from AT Cmd Parser \n

    \param      pPatr 
                ATRspAudioTransferType *
                pointer to AT response
    
    \return     SuccessFailType 
                SUCCESS or FAIL
   
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_sfHandleAudioTransDisc(const ATRspAudioTransferType *pPatr)
{
    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPatr: PATR:%d", pPatr->mode ); 

    switch(pPatr->mode) 
    {
        case AT_AUDIO_DISCRETE_MODE:
            // it's not sure that PATR:0 indicates HandsFreeOff,
            // so we have to wait for about 1 sec, to decide if
            // private call is set or not
            // Possible workarounds (PARROT):
            // Show the popup 1 sec after PATR:0, only if:
            // -	No PLCC:6 is received (call terminated).
            // -	No PLCC:1 is received (call held).
            // -	PATR:1 (PATR:0 received before the call).

            if(BT_TRUE == gAppPhoneContext.isHangUpSent)
            {
                DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPatr: PATR:0 ign, hangup" );
            }
            else
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_PATR_WORKAROUND_TIMER, 
                                                          MSEC(APP_PHONE_PATR_TIMEOUT), 0);
            }
            
            break;
        case AT_AUDIO_HANDSFREE_MODE:
            (void)APP_COMMON_UTILS_stCancelOsekRelAlarm( CTA_APP_PHONE_PATR_WORKAROUND_TIMER );

            if(gAppPhoneContext.isHandsFreeOff == BT_TRUE)
            {
                gAppPhoneContext.isHandsFreeOff = BT_FALSE;
                _APP_PHONE_vNotifyApp(APP_PHONE_HANDSFREE_MODE_CHANGED);
            }
            break;
        default:
            return FAIL;
    }

//    if(ignoreLastAudioTransferMode == BT_FALSE)
//    {
//        _APP_PHONE_vNotifyApp(APP_PHONE_HANDSFREE_MODE_CHANGED);
//    }
//    else
//    {
//        ignoreLastAudioTransferMode = BT_FALSE;
//    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_sfHandleClassOfDevice                         */
/** 
    \brief      handle class of device response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles class of device response msg from AT Cmd Parser \n

    \param      pPbcd 
                ATRspClassOfDeviceRequestType *
                pointer to AT response
    
    \return     SuccessFailType 
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType
_APP_PHONE_AT_sfHandleClassOfDevice(const ATRspClassOfDeviceRequestType *pPbcd)
{

    ATCoDProfileEnum oldVersion = gAppPhoneContext.hfpVersion;

    if( AT_TRUE == pPbcd->profile_avail )
    {
        switch(pPbcd->profile) 
        {
            case AT_CODPROF_HANDSFREE_0_96:
            case AT_CODPROF_HANDSFREE_1_00:
            case AT_CODPROF_HANDSFREE_1_50:
                gAppPhoneContext.hfpVersion = pPbcd->profile;
                break;
            default:
                gAppPhoneContext.hfpVersion = AT_CODPROF_NONE;
                break;
        }
    }
    else
    {
        gAppPhoneContext.hfpVersion = AT_CODPROF_NONE;
    }
    
    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaClassOfDev: hfp vers %d",
                   gAppPhoneContext.hfpVersion );

    if(oldVersion != gAppPhoneContext.hfpVersion )
    {
        _APP_PHONE_vNotifyApp(APP_PHONE_HFP_VERSION_CHANGED);
    }

    return SUCCESS;
}

//void _APP_PHONE_AT_vUpdateValues(ATRspAllType* rsp)
//{
//    switch( rsp->common.rsp_type )
//    {
//        case AT_RSP_PHONE_STATUS_IND_VAL:       /* PIND */
//            _APP_PHONE_AT_sfHandlePhoneStatus(&rsp->pind_val);
//            break;
//        case AT_RSP_PHONE_STATUS_IND_EVT:       /* PIEV */
//            _APP_PHONE_AT_sfHandlePhoneStatusEvent(&rsp->piev);
//            break;
//    }
//}


//const ATRspPhoneStatusIndValueType * APP_PHONE_AT_pGetPindResponse()
//{
//    if( NULL != gAppPhoneContext.lastATRsp  && 
//        AT_RSP_PHONE_STATUS_IND_VAL == gAppPhoneContext.lastATRsp->common.rsp_type)
//    {
//        return &gAppPhoneContext.lastATRsp->data->pind_val;
//    }
//    return NULL;
//}
//const ATRspPhoneStatusIndEventType * APP_PHONE_AT_pGetPievEvent()
//{
//    if( NULL != gAppPhoneContext.lastATRsp  && 
//        AT_RSP_PHONE_STATUS_IND_EVT == gAppPhoneContext.lastATRsp->common.rsp_type)
//    {
//        return (const ATRspPhoneStatusIndEventType*)&gAppPhoneContext.lastATRsp->data->piev;
//    }
//    return NULL;
//}

/* End Of File APP_PHONE_AR_C1.c */


