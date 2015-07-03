/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_BAP_REQUEST_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application for BAP
*-----------------------------------------------------------------------------
* Description:    Handles all call specific responses towards HU via BAP   
*-----------------------------------------------------------------------------
* $Date: 2011-07-20 15:26:10 +0200 (Mi, 20 Jul 2011) $
* $Rev: 20966 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/src/APP_PHONE_BAP_REQUEST_C1.c $
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
#include "APP_PHONE_BAP.h"
#include "APP_PHONE_BAP_CI.h"
#include "APP_PHONE_BAP_REQUEST.h"
#include "APP_PHONE_BAP_INDICATION.h"

#include "APP_PHONE_AT.h"
#include "APP_PHONE_AT_PNCC.h"

#include "ATParser.h"
#include "APP_COMMON_Utils.h"
#include "POOL.h"
#include "ERM.h"
#include "Bap_Dispatcher.h"
#include "CONTEXT_MAN.h"
#include "APP_PHONE_BAP_REQUEST_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

extern AppPhoneBapContext gAppPhoneBapContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/
const CallState AT2BapCallStateMatrix[] =
{
    BAP_ACTIVE,           /* AT_CS_ACTIVE_CALL      = 0 */ 
    BAP_ON_HOLD,          /* AT_CS_HELD_CALL        = 1 */
    BAP_DIALING,          /* AT_CS_OUTCALL_DIAL_IPR = 2 */
    BAP_DIALING,          /* AT_CS_OUTCALL_RING_IPR = 3 */
    BAP_RINGING_WAITING,  /* AT_CS_INCOMING         = 4 */
    BAP_RINGING_WAITING,  /* AT_CS_WAITING_CALL     = 5 */
    BAP_DISCONNECTING //BAP_IDLE              /* AT_CS_CALL_TERMINATED  = 6 */
}; 


/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/

static void _APP_PHONE_BAP_vUpdateNetworkProvider(void);
static void _APP_PHONE_BAP_vUpdateSignalQuality(void);
static void _APP_PHONE_BAP_vUpdateRoaming(void);
static void _APP_PHONE_BAP_vUpdateServiceAvailable(void);
static void _APP_PHONE_BAP_vUpdateMicMute(void);
static void _APP_PHONE_BAP_vUpdateHandsFreeMode(void);


static Uint8Type _APP_PHONE_BAP_u8GetSignalQualityMapping(  Uint8Type atSignalLevelValue);
static Uint8Type _APP_PHONE_BAP_u8GetRegisterStateMapping(  Uint8Type atServiceValue, 
                                                            Uint8Type atRoamingValue);

static SuccessFailType _APP_PHONE_BAP_sfHandleCallInfoChanged(void);
//static SuccessFailType _APP_PHONE_BAP_sfHandlePhoneStatus();
//static SuccessFailType _APP_PHONE_BAP_sfHandlePhoneStatusEvent();
//static SuccessFailType _APP_PHONE_BAP_sfHandleMicMuteMode(ATRspMicroMuteType* pmmu);
//static SuccessFailType _APP_PHONE_BAP_sfHandleHandsFreeMode(ATRspAudioTransferType* patr);
static SuccessFailType _APP_PHONE_BAP_sfSendHandsFreeUpdate(void);
//static SuccessFailType _APP_PHONE_BAP_sfSendMicroMuteUpdate();


static SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendCallInfoStatusRemove(
                            Uint16Type start,
                            Uint16Type elements);
static SuccessFailType _APP_PHONE_BAP_sfSendCallInfoChangedMessage(
                            Uint16Type start,
                            Uint16Type elements);
static SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendCallInfoChanged(void);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification)
{
    const ERMUserProfileType *pUser = ERM_pGetUserProfile_DevId(ERM_u8GetActivePhone());
    if( pUser == NULL)
    {   
        DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "OnAppPhoneNot: no user prof, ign %d",
                notification);
        return;
    }

    switch(notification)
    {
        case APP_PHONE_OPERATOR_NAME_CHANGED:
            /*send the info only if the user is not being activated*/
            if(APP_PHONE_BAP_bIsUserActivated() == BT_FALSE)
            {
                _APP_PHONE_BAP_vUpdateNetworkProvider();
            }
            break;
        case APP_PHONE_NW_SIGSTRENGHT_CHANGED:
            /*send the info only if the user is not being activated*/
            if(APP_PHONE_BAP_bIsUserActivated() == BT_FALSE)
            {
                _APP_PHONE_BAP_vUpdateSignalQuality();
            }
            break;
        case APP_PHONE_ROAMING_CHANGED:
            /*send the info only if the user is not being activated*/
            if(APP_PHONE_BAP_bIsUserActivated() == BT_FALSE)
            {
                _APP_PHONE_BAP_vUpdateRoaming();
            }
            break;
        case APP_PHONE_SERV_AVAIL_CHANGED:
            /*send the info only if the user is not being activated*/
            if(APP_PHONE_BAP_bIsUserActivated() == BT_FALSE)
            {
                _APP_PHONE_BAP_vUpdateServiceAvailable();
            }
            break;
        case APP_PHONE_CALL_STATE_CHANGED:
            /*send the info only if the user is not being activated*/
            if(APP_PHONE_BAP_bIsUserActivated() == BT_FALSE)
            {
               (void)_APP_PHONE_BAP_sfHandleCallInfoChanged();
            }
            break;
        case APP_PHONE_MIC_MUTE_CHANGED:
            _APP_PHONE_BAP_vUpdateMicMute();
            break;
        case APP_PHONE_HANDSFREE_MODE_CHANGED:
            /*send the info only if the user is not being activated*/
            if(APP_PHONE_BAP_bIsUserActivated() == BT_FALSE)
            {
                _APP_PHONE_BAP_vUpdateHandsFreeMode();
            }
            break;
        case APP_PHONE_HFP_VERSION_CHANGED:
            _APP_PHONE_BAP_vHandleHfpVersionChanged();
            break;
        case APP_PHONE_BATT_LEVEL_CHANGED:
        default:
            /* ignore notification */
            break;
    }
}

//RS: added an internal handler, to have a better control of who triggered the notification
void _APP_PHONE_BAP_vInternalNotification(APP_PHONE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_PHONE_OPERATOR_NAME_CHANGED:
            _APP_PHONE_BAP_vUpdateNetworkProvider();
            break;
        case APP_PHONE_SERV_AVAIL_CHANGED:
            _APP_PHONE_BAP_vUpdateServiceAvailable();
            break;
        default:
            DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "vInternalNot: unsup %d",
                                notification);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_vOnBAPDispatcherNotification                   */
/**
    \brief      handle BAP Dispatcher notifications

    \brief      Description:\n
                handle BAP Dispatcher notifications  \n

    \param      notification
                APP_BAPDispatcher_NotificationType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vOnBAPDispatcherNotification(APP_BAPDispatcher_NotificationType notification)
{
    switch(notification)
    {
        case APP_BAPDISPATCHER_NOTIFY_CALLINFO_SENT:
            if((gAppPhoneBapContext.CallInfoPending == BT_TRUE) && (gAppPhoneBapContext.CallStateChangedPending == BT_TRUE))
            {
                gAppPhoneBapContext.CallInfoPending = BT_FALSE;
                gAppPhoneBapContext.CallStateChangedPending = BT_FALSE;
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CallStateChanged event finished");
            }
            break;
        default:
            DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "vBAPDispatcherNot: unsup %d",
                                notification);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vHandlePhoneStatus                              */
/** 
    \brief      handle phone status response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles phone status response msgs from AT Cmd Parser \n

    \param      atRsp 
                ATRspMessageType
    
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
#if 0
SuccessFailType _APP_PHONE_BAP_sfHandlePhoneStatus()
{
    _APP_PHONE_BAP_vUpdateNetworkProvider();
    _APP_PHONE_BAP_vUpdateSignalQuality();
    _APP_PHONE_BAP_vUpdateServiceAvailable();

    return SUCCESS;

}/* END OF _APP_PHONE_BAP_vHandlePhoneStatus() */
#endif

static void _APP_PHONE_BAP_vUpdateNetworkProvider()
{
    const AT_NetworkProvider *nwProv = APP_PHONE_GetNetworkProvider();

    (void)_APP_PHONE_BAP_REQUEST_sfSendNetworkProvider(nwProv->name, 
                                                 nwProv->name_len);
}

static void _APP_PHONE_BAP_vUpdateSignalQuality()
{
    if((gAppPhoneBapContext.bSignalQualityAvailableAfterConnect == BT_FALSE)&&(APP_PHONE_PIEV_INVALID_VALUE == APP_PHONE_GetSignalStrength()))
    {
        (void)_APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();

        gAppPhoneBapContext.bSignalQualityAvailableAfterConnect = BT_TRUE;
    }
    (void)_APP_PHONE_BAP_REQUEST_sfSendSignalQuality(APP_PHONE_GetSignalStrength());    
}

static void _APP_PHONE_BAP_vUpdateServiceAvailable()
{
    (void)_APP_PHONE_BAP_REQUEST_sfSendRegisterState(APP_PHONE_GetNetworkAvailable(),
                                               APP_PHONE_GetRoaming());
}

static void _APP_PHONE_BAP_vUpdateRoaming()
{
    (void)_APP_PHONE_BAP_REQUEST_sfSendRegisterState(APP_PHONE_GetNetworkAvailable(),
                                               APP_PHONE_GetRoaming());
}
static void _APP_PHONE_BAP_vUpdateMicMute()
{
    // get new mode from APP_PHONE
    if(APP_PHONE_bIsMicMuted() == BT_FALSE)
    {
        (void)_APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(BAP_MICRO_UNMUTE);
    }
    else
    {
        (void)_APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(BAP_MICRO_MUTE);
    }
}

static void _APP_PHONE_BAP_vUpdateHandsFreeMode()
{
    // get new mode from APP_PHONE
    if(( APP_PHONE_bIsHandsFreeOff() == BT_FALSE) && ((gAppPhoneBapContext.HandsFreeModeAlreadySent == AT_AUDIO_DISCRETE_MODE)))
    {
        (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_ON);
        gAppPhoneBapContext.HandsFreeModeAlreadySent = AT_AUDIO_HANDSFREE_MODE;
    }
    else if(( APP_PHONE_bIsHandsFreeOff() == BT_TRUE) && ((gAppPhoneBapContext.HandsFreeModeAlreadySent == AT_AUDIO_HANDSFREE_MODE)))
    {
        if(!APP_PHONE_AT_PNCC_bAllCallStatesIdle())
            (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_OFF);
        gAppPhoneBapContext.HandsFreeModeAlreadySent = AT_AUDIO_DISCRETE_MODE;
    }
}


void _APP_PHONE_BAP_vHandleHfpVersionChanged()
{
    (void)_APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();
}

SuccessFailType _APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport()
{
    SuccessFailType result = FAIL;

    if(AT_INVALID_DEVICE_ID != ERM_u8GetActivePhone())
    {

        if((APP_PHONE_enGetHfpVersion()== AT_CODPROF_HANDSFREE_1_50) && (BT_FALSE == ERM_bIsCDMAPhone()))
        {
            /* call hold and conference are supported */
            PhoneMobileServiceSupport phoneMss;
            _APP_PHONE_BAP_vSetBasicMobileServiceSupport(&phoneMss);
            phoneMss.CallHold         = 1U;
            phoneMss.ResumeCall       = 1U;
            phoneMss.CCJoin     = 1U;
            //phoneMss.MPReleaseActiveCallAcceptWaitingCall = 1U;
            phoneMss.MPSwap   = 1U;
            phoneMss.MPCallHoldAcceptWaitingCall = 1U;
#ifdef UHV_ACTIVATE_FEATURE_CCSPLIT                
            phoneMss.CCSplit    = 1U;
#endif

            result = _APP_PHONE_BAP_REQUEST_sfSendMobServSupp(&phoneMss);

            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HanHfpVersChg: CallHold/Conf. supp");
        }
        else
        {
            /* call hold ( CallHold ) and conference ( CCJoin , CCSplit ) are not supported */
            PhoneMobileServiceSupport phoneMss;
            _APP_PHONE_BAP_vSetBasicMobileServiceSupport(&phoneMss);

            result = _APP_PHONE_BAP_REQUEST_sfSendMobServSupp(&phoneMss);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HanHfpVersChg: CallHold/Conf. not supp");
        } 
    }
    else
    {
        PhoneMobileServiceSupport phoneMss;
        (void)memset(&phoneMss, 0, sizeof(phoneMss));
        result = _APP_PHONE_BAP_REQUEST_sfSendMobServSupp(&phoneMss);
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_u8GetSignalQualityMapping                      */
/** 
    \brief      map AT signal_level_value to BAP ServiceQuality 
  
    \brief      Description:\n
                map AT signal_level_value to BAP SignalQuality\n

    \param      atSignalLevelValue 
                Uint8Type
                holds signal level value ( from AT parser ) 
                
    \return     Uint8Type 
                SignalQuality in BAP valid range
*/
/*----------------------------------------------------------------------------*/
static Uint8Type _APP_PHONE_BAP_u8GetSignalQualityMapping( Uint8Type atSignalLevelValue)
{
    Uint8Type signalQuality = (Uint8Type)BAP_NO_RECEPTION;
    
    switch( atSignalLevelValue)
    {
        case 0:
            signalQuality = (Uint8Type)BAP_NO_RECEPTION;
            break;
        case 1:
            signalQuality = (Uint8Type)BAP_VERY_LOW_RECEPTION;
            break;
        case 2: 
            signalQuality = (Uint8Type)BAP_LOW_RECEPTION;
            break;
        case 3:
            signalQuality = (Uint8Type)BAP_MID_RECEPTION;
            break;
        case 4:
            signalQuality = (Uint8Type)BAP_GOOD_RECEPTION;
            break;
        case 5:
            signalQuality = (Uint8Type)BAP_BEST_RECEPTION;
            break;
        default:
            signalQuality = (Uint8Type)BAP_RECEPTION_NOT_SUPPORTED;
            break;
    }

    return signalQuality; 
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_u8GetRegisterStateMapping                      */
/** 
    \brief      map AT service_value and roaming_value to BAP RegisterState 
  
    \brief      Description:\n
                map AT service_value and roaming_value to BAP RegisterState\n

    \param      atServiceValue 
                Uint8Type
                holds service value ( from AT parser ) 
                
    \param      atRoamingValue
                Uint8Type
                holds roaming value ( from AT Parser )   

    \return     Uint8Type 
                RegisterState in BAP valid range
*/
/*----------------------------------------------------------------------------*/
static Uint8Type _APP_PHONE_BAP_u8GetRegisterStateMapping( Uint8Type atServiceValue, Uint8Type atRoamingValue)
{
    Uint8Type registerState = (Uint8Type)BAP_FUNCTION_NOT_SUPPORTED_BY_ME;

    if( !atServiceValue && !atRoamingValue )
    {
        // not registered and not searching
        registerState = (Uint8Type)BAP_NOT_REGISTERED_AND_NOT_SEARCHING;
    }   
    else if( !atServiceValue && atRoamingValue)
    {
        // not registered and searching
        registerState = (Uint8Type)BAP_NOT_REGISTERED_AND_SEARCHING;
    }
    else if( atServiceValue && !atRoamingValue)
    {
        // registered
        registerState = (Uint8Type)BAP_REGISTERED;
    }
    else if( atServiceValue && atRoamingValue)
    {   
        // registered and roaming
        registerState = (Uint8Type)BAP_REGISTERED_AND_ROAMING;
    }
    return registerState; 
}

#if 0
SuccessFailType _APP_PHONE_BAP_sfHandleHandsFreeMode(ATRspAudioTransferType* patr)
{
    // do not send HandsFreeMode from Phone to BAP 
    /*
    switch( patr->mode )
    {
        case AT_AUDIO_DISCRETE_MODE:
            return _APP_PHONE_AT_sfSendHandsFreeOnOffBapRequest(BAP_HANDSFREE_OFF);
        case AT_AUDIO_HANDSFREE_MODE:
            return _APP_PHONE_AT_sfSendHandsFreeOnOffBapRequest(BAP_HANDSFREE_ON);
    }
    */
    return SUCCESS;      
}
#endif

SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(HandsFreeMode mode)
{ 
    BapRequest request;

    // update property    
    request.lsg_id      = BapLsg_Telephone; 
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_INT8;
    request.fct_id      = BapFct_Telephone_HandsFreeOnOff; 

    switch( mode )
    {
        case BAP_HANDSFREE_OFF:
        case BAP_HANDSFREE_ON:
            break;
        default:
            return FAIL;
    }      
    request.payload.u8_value = (Uint8Type)mode;
    
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest(msgPhoneBapRequest, &request))
    {
        return FAIL;
    }
    return SUCCESS;
}

#if 0
SuccessFailType _APP_PHONE_BAP_sfHandleMicMuteMode(ATRspMicroMuteType* pmmu)
{
    // do not send MicMute events from Phone to BAP!
    /*
    switch( pmmu->mode )
    {
        case AT_MIC_MUTE:
            return _APP_PHONE_AT_sfSendMicroMuteOnOffBapRequest(BAP_MICRO_MUTE);
        case AT_AUDIO_HANDSFREE_MODE:
            return _APP_PHONE_AT_sfSendMicroMuteOnOffBapRequest(BAP_MICRO_UNMUTE);
    }
    */      
    return SUCCESS;
}
#endif

SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(MicroMuteMode mode)
{
    BapRequest request;

    // update property    
    request.lsg_id      = BapLsg_Telephone; 
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_INT8;
    request.fct_id      = BapFct_Telephone_MicroMuteOnOff; 

    switch( mode )
    {
        case BAP_MICRO_MUTE:
        case BAP_MICRO_UNMUTE:
            break;
        default:    
            return FAIL; 
     }      
 
    request.payload.u8_value = (Uint8Type)mode;

    if( E_OK != APP_COMMON_UTILS_stSendBapRequest(msgPhoneBapRequest, &request))
    {
        return FAIL;
    }
    return SUCCESS;

}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_REQUEST_sfSendMobServSupport                   */
/** 
    \brief      sends Update of mobile service support to BAP
  
    \brief      Description:\n
                sends Update of mobile service support to BAP \n

    \return     SuccessFailType
                FAIL or SUCCESS 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendMobServSupp(const PhoneMobileServiceSupport *pPhoneMss)
{
    BapRequest bapReq;
    SuccessFailType result = SUCCESS;
   
    /* build the request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_MobileServiceSupport; 
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;
   
    // allocate uffer for Mobile service support 
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
 
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "MobServSupport: POOL_sfAllocate( ) failed");
        return FAIL;
    }
   
    bapReq.payload.byte_seq.length = sizeof(*pPhoneMss);

    (void)memcpy( bapReq.payload.byte_seq.data, 
            pPhoneMss, 
            bapReq.payload.byte_seq.length);

 
    /* send bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    return SUCCESS;
}

 
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_REQUEST_sfSendFsgOperationStateRequest                     */
/** 
    \brief      sends Update for FSG OperationState to BAP
  
    \brief      Description:\n
                sends Update for FSG OperationState to BAP \n

    \return     SuccessFailType
                FAIL or SUCCESS 
*/
/*----------------------------------------------------------------------------*/

SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(FsgOperationState newState)
{
    BapRequest bapReq;
   
    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_FSG_OperationState; 
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_INT8;
   
    bapReq.payload.u8_value = (Uint8Type)newState;
 
    /* send FsgOperationState bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    return SUCCESS;
}/* END OF _APP_PHONE_sfSendFsgOperationStateRequest() */


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_REQUEST_sfSendFsgSetup                         */
/** 
    \brief      sends Update for FSG setup request to BAP
  
    \brief      Description:\n
                sends Update for FSG setup request to BAP \n

    \return     SuccessFailType
                FAIL or SUCCESS 
*/
/*----------------------------------------------------------------------------*/

SuccessFailType
_APP_PHONE_BAP_REQUEST_sfSendFsgSetup(BapFsgSetupMobileConnectionType connectionType)
{
    /* these values do not change dynamically */
    Uint8Type phoneCharacteristics = TELEPHONE_DEFAULT_FSG_SETUP__PHONE_CHARACTERISTICS;  /* HFP connection possible */   
    Uint8Type numberOfUserProfiles = TELEPHONE_DEFAULT_FSG_SETUP__SUPP_NUM_USER_PROFILES; /* number of supported profiles */
    Uint8Type phoneType            = TELEPHONE_DEFAULT_FSG_SETUP__PHONE_TYPE;             /* UHV High */

    SuccessFailType result = SUCCESS;
    BapRequest bapReq;
   
    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_FSG_Setup; 
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;
  
    result    = POOL_sfAllocate(POOL_enBapReqDataType,
                                (void**)&bapReq.payload.byte_seq.data);
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }

    bapReq.payload.byte_seq.length  = (Uint16Type)BAP_MAX_BYTE_SEQ_enAppTelFsgSetup;  

    bapReq.payload.byte_seq.data[0] = phoneCharacteristics;
    bapReq.payload.byte_seq.data[1] = (Uint8Type)connectionType;
    bapReq.payload.byte_seq.data[2] = numberOfUserProfiles;
    bapReq.payload.byte_seq.data[3] = phoneType;
    
 
    /* send FsgOperationState bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    return SUCCESS;
}/* END OF _APP_PHONE_sfSendFsgOperationStateRequest() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_sfHandleCallInfoChanged                       */
/** 
    \brief      handles notification about changed call states 
  
    \brief      Description:\n
                handles notification about changed call states \n 
    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_BAP_sfHandleCallInfoChanged()
{
    SuccessFailType result = SUCCESS;
    static Uint8Type MaxRetryCallInfoChangedCounter = 0;

    if(((gAppPhoneBapContext.CallInfoPending == BT_FALSE) && (gAppPhoneBapContext.CallStateChangedPending == BT_FALSE)) ||
        (MaxRetryCallInfoChangedCounter > 50))
    {
        gAppPhoneBapContext.notification = APP_PHONE_INVALID;
        MaxRetryCallInfoChangedCounter=0;

        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HandleCallInfoChanged" );

        /* Check first if CallInfo has changed */
        if( APP_PHONE_AT_PNCC_bCallInfoHasChanged() )
        {
            /* send an array changed message for call info */
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP,"PNCC array changed, send CallInfo changed");
            gAppPhoneBapContext.CallStateChangedPending = BT_TRUE;
            result = _APP_PHONE_BAP_REQUEST_sfSendCallInfoChanged();
        }

        /* Properties */
        if(APP_PHONE_AT_PNCC_bArrayHasChanged())
        {
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HandleCallArrayChanged" );

            /* send the line state */
            result = _APP_PHONE_BAP_REQUEST_sfSendCallState();
            /* send the call states */
            result = _APP_PHONE_BAP_REQUEST_sfSendLineState();
            /* send the hands free update */
            result = _APP_PHONE_BAP_sfSendHandsFreeUpdate();
            /* send the mobile service support when call array has changed*/
            _APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();
        }
    }
    else
    {
        gAppPhoneBapContext.notification = APP_PHONE_CALL_STATE_CHANGED;
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_BAP_REQUEST_PENDING, MSEC(10), 0);
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HandleCallInfoChanged in processing, retry after 10 ms" );
        MaxRetryCallInfoChangedCounter++;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendCallInfoChanged                 */
/** 
    \brief      checks what has changed regariding current call state and 
                and builds up the correct ChangedArray message  
  
    \brief      Description:\n
                checks what has changed regariding current call state and 
                and builds up the correct ChangedArray message \n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendCallInfoChanged()
{
    SuccessFailType result  = SUCCESS;
    Uint16Type i            = 0U;
    Uint16Type start        = 0U;    
    Uint16Type elements     = 0U;
    ElementChangedState prevElement     = ARRAY_ELEM_UNKNOWN;    
    // get pncc context from APP_PHONE
    const AppPhoneAtPnccContext * pPncc = APP_PHONE_AT_PNCC_pGetPnccContext();
    if( NULL == pPncc )
    {
        return FAIL;
    }

    for(i=0U; i<MAX_PLCC_ELEMENTS; i++)
    {
       switch(pPncc->CallInfoHasChanged[i])
       {
            case ARRAY_ELEM_ADDED:
            case ARRAY_ELEM_REMOVED:
            {
                if((prevElement == ARRAY_ELEM_ADDED)||(prevElement == ARRAY_ELEM_REMOVED))
                {
                     elements++;
                }
                else
                {
                    start = i;
                    elements++;
                }
                prevElement = ARRAY_ELEM_ADDED;
                break;
            }
    
            default:
            {
                prevElement = ARRAY_ELEM_NOT_CHANGED;
                break;
            }
        }
    }

   if(elements > 0)
   {
       result = _APP_PHONE_BAP_sfSendCallInfoChangedMessage(
                           start,
                           elements);
   }

   return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendCallInfoStatusRemoved           */
/** 
    \brief      checks what has changed regariding current call state and 
                and builds up the correct ChangedArray message  
  
    \brief      Description:\n
                checks what has changed regariding current call state and 
                and builds up the correct ChangedArray message \n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendCallInfoStatusRemove(
                            Uint16Type start,
                            Uint16Type elements)
{
    SuccessFailType result  = SUCCESS;
    ArrayHeaderMode statusMode;
    Uint16Type i = 0U;

    Uint8Type *pHeaderWriteBuf;
    Uint8Type *pDataWriteBuf;

    BapRequest bapReq;
    
    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_CallInfo;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted 
    bapReq.data_type    = BAP_DT_BYTE;

    
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "SendCallInfoStatusRemove: POOL_sfAllocate() failed");
        return FAIL;
    }
    
    pHeaderWriteBuf = bapReq.payload.byte_seq.data;
    pDataWriteBuf = bapReq.payload.byte_seq.data + 3U;

    /* build up the array content */     
    for(i=start; i<(start+elements); i++)
    { 
        Uint16Type tmpLen = 0U;
        
        /* write an empty string for the content of PbName, to indicate a
         * removed item */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf, (Uint8Type*)"", 0);
        pDataWriteBuf += tmpLen; 
    }
    
    /* build up the header */ 
    statusMode.data = 0U;
    /* indicate that we set the value for PbName */
    statusMode.status.recordAddress = 1U;
     
    /* set the header mode byte */    
    *pHeaderWriteBuf = statusMode.data;
    pHeaderWriteBuf++;
    
    /* set the header start byte */    
    *pHeaderWriteBuf = start;
    pHeaderWriteBuf++;
    
    /* set the header elements byte */    
    *pHeaderWriteBuf = elements;

    /* set the size of byte sequence */
    bapReq.payload.byte_seq.length = 
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);


    /* send callInfo bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    
    return SUCCESS;
}


static SuccessFailType _APP_PHONE_BAP_sfSendCallInfoChangedMessage(
                            Uint16Type start,
                            Uint16Type elements)
{
    SuccessFailType result  = SUCCESS;
    ArrayHeaderMode changedMode;
    Uint8Type headerSize = 0U;
    
    Uint8Type *pHeaderWriteBuf;
    BapRequest bapReq;

    //ASSERT( gPnccContext.pnccState == PNCC_VALID );
    
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_CallInfo;
    bapReq.req_type     = BapReq_Changed;  /* ChangedArray optype wanted */
    bapReq.data_type    = BAP_DT_BYTE;

    result = POOL_sfAllocate( POOL_enBapArrayHeader,
                              (void**)&bapReq.payload.byte_seq.data);
    
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "SendCallInfoChangedMessage: POOL_sfAllocate() failed");
        return FAIL;
    }

    if( (start>255) || (elements>255) )
    {
        headerSize = 5U;
    }
    else
    {
        headerSize = 3U;
    }

    /* set the size of byte sequence */
    bapReq.payload.byte_seq.length = headerSize;
   
    /* set the pointer write pointer */ 
    pHeaderWriteBuf = bapReq.payload.byte_seq.data;

    /* init mode byte */    
    changedMode.data = 0U;
    changedMode.changed.indexSize = (headerSize == 3) ? 0 : 1;

    *pHeaderWriteBuf = changedMode.data;
    pHeaderWriteBuf++;

    if(headerSize == 3U)
    {
        *pHeaderWriteBuf = (Uint8Type)start;
        pHeaderWriteBuf++;
        
        *pHeaderWriteBuf = (Uint8Type)elements;
    }
    else
    {
        *pHeaderWriteBuf = (start>>8) & 0xFF;
        pHeaderWriteBuf++;

        *pHeaderWriteBuf = (start & 0xFF);
        pHeaderWriteBuf++;

        
        *pHeaderWriteBuf = (elements>>8) & 0xFF;
        pHeaderWriteBuf++;

        *pHeaderWriteBuf = (elements & 0xFF);
    }

    /* send callInfo bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }

    return SUCCESS;
}    


SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendCallInfoStatus(const BapArrayHeaderType * pRecvHeader)
{
    BapRequest bapReq;
    SuccessFailType result  = SUCCESS;

    Uint16Type i = 0U;
    Uint8Type startTemp = 0U;
    Uint8Type headerLength = 0U;

    BapArrayHeaderType sendHeader;
    Uint8Type *pDataWriteBuf;
    
    BooleanType bIsMailboxCall = BT_FALSE;
    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();

    // get pncc context from APP_PHONE
    const AppPhoneAtPnccContext * pPncc = APP_PHONE_AT_PNCC_pGetPnccContext();

    if(gAppPhoneBapContext.CallStateChangedPending == BT_TRUE)
    {
        gAppPhoneBapContext.CallInfoPending = BT_TRUE;
    }

    if( NULL == pPncc )
    {

        return FAIL;
    }

    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_CallInfo;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted 
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the mode byte of the header */ 
    sendHeader.mode.data    = pRecvHeader->mode.data;
    /* get the start value of the header */ 
    sendHeader.start        = pRecvHeader->start;
    /* set the elements value to 0 */ 
    sendHeader.elements     = 0U;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }
    
    /* preset the header to get the length (3 or 5) */    
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    if((sendHeader.mode.status.shift == 1) && (sendHeader.mode.status.direction == 0))
    {
        startTemp = sendHeader.start + 1;
    }
    else
    {
        startTemp = sendHeader.start;
    }

    /* build up the array content */     
    for(i=startTemp; i<MAX_PLCC_ELEMENTS; i++)
    { 
        Uint16Type tmpLen = 0U;

        if(pPncc->plcc[i] == NULL)
        {
            continue;
        }

        if(sendHeader.elements >= pRecvHeader->elements)
        {
            break;
        }
       
        /* increase the number of records by one */ 
        sendHeader.elements++;

        /* write the positon of the record */
        *pDataWriteBuf = (Uint8Type)i;
        pDataWriteBuf++;


        /*-----------------------------*/
        // check if callerId is number saved for mailbox for active user, if so, show
        // 'Mailbox' instead of number 
        if( NULL != profile )
        {
            // first check if pbName equals the mailbox number ( this might be
            // if mailbox is not saved in phone, just in ck5050, then the name
            // of the called / calling party equals the number 
            if(pPncc->plcc[i]->id_avail && 
               pPncc->plcc[i]->caller_id_len == profile->voicemailbox_len && 
               0 == memcmp( profile->voicemailbox, pPncc->plcc[i]->caller_id, pPncc->plcc[i]->caller_id_len))
            {   // transmitted called / calling party name equals mailbox number
                bIsMailboxCall = BT_TRUE;
            }
            // else : although the transmitted name does not equals the mailbox
            // number saved in eeprom, this might be ca call to the mailbox.
            // this happens, when mailbox number is saved in phone with a name.
        
        }

        /* write the content of PbName */
        if( BT_TRUE == bIsMailboxCall )
        {   // show 'Mailbox' as name
            tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf, 
                                (Uint8Type*)"Mailbox",
                                strlen("Mailbox"));
            pDataWriteBuf += tmpLen; 
        }
        else if(pPncc->plcc[i]->id_avail)
        {   // copy transmitted name of called / calling party
            tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf, 
                                pPncc->plcc[i]->caller_id,
                                pPncc->plcc[i]->caller_id_len);
            pDataWriteBuf += tmpLen; 
        }
        else
        {
            tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf, 
                                                         (Uint8Type*)"\0", 
                                                         1);
            pDataWriteBuf += tmpLen; 
        }


        /*--------------------------------*/
        /* write the content of TelNumber */
        if(pPncc->plcc[i]->ext_avail)
        {
            tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf, 
                                pPncc->plcc[i]->call_id_ext,
                                pPncc->plcc[i]->caller_id_ext_len);
            pDataWriteBuf += tmpLen; 
        }
        else
        {
            /* send empty bap string */
            tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf,
                                                         (Uint8Type*)"", 
                                                         0);
            pDataWriteBuf += tmpLen; 
        }    

    }
    
    /* activate the position bit if we have elements to send*/
    sendHeader.mode.status.transmitPos = (sendHeader.elements > 0) ? 1 : 0;
     
    /* write the actual header to the start of the buffer */      
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );
    

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length = 
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send callInfo bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    
    if(gAppPhoneBapContext.bDialNumberPending == BT_TRUE) //RT:3838
    {
        APP_PHONE_BAP_vProcessDialNumber(BAP_CALLINFO_SENT);
    }

    if(gAppPhoneBapContext.bHangUpCallPending == BT_TRUE) //RT:3838
    {
        APP_PHONE_BAP_vProcessHangUpCall(BAP_CALLINFO_SENT);
    }

    DEBUG_TEXT(APP_PHONE_BAP_SWI_DBG, MOD_APP_PHONE_BAP, "CallInfo: Sent StatusArray Response to BAP");

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendLineState                       */
/** 
    \brief      update bap's line state 
  
    \brief      Description:\n
                update bap's line state \n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendLineState()
{
    Sint16Type i = 0;
    BapRequest bapReq;
    
    // get pncc context from APP_PHONE
    const AppPhoneAtPnccContext *pPncc = APP_PHONE_AT_PNCC_pGetPnccContext();
    if( NULL == pPncc )
    {
        return FAIL;
    }
   
    //ASSERT( gPnccContext.pnccState == PNCC_VALID );
    
    /* build the LineState request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_LineState; 
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_INT8;
   
    bapReq.payload.u8_value = (Uint8Type)BAP_LINE_STATE_NO_LINE;

    for( i=0; i<MAX_PLCC_ELEMENTS; i++ )
    {
        /* check if there is one call => Connected Line */
        if( pPncc->plcc[i] )
        {
            bapReq.payload.u8_value = (Uint8Type)BAP_LINE_STATE_CONNECTED_LINE;
        }
    } 
 
    /* send lineState bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    if(gAppPhoneBapContext.bDialNumberPending == BT_TRUE) //RT:3838 inform the application
    {
        APP_PHONE_BAP_vProcessDialNumber(BAP_LINESTATE_SENT);
    }

    if(gAppPhoneBapContext.bHangUpCallPending == BT_TRUE) //RT:3838 inform the application
    {
        APP_PHONE_BAP_vProcessHangUpCall(BAP_LINESTATE_SENT);
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendAllCallStatesIdle               */
/** 
    \brief      update bap's call state to all idle. this is done after
                disconnect of active user
  
    \brief      Description:\n
                update bap's call state to all idle. this is done after
                disconnect of active user\n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendAllCallStatesIdle()
{
    BapRequest bapReq;
    SuccessFailType result      = SUCCESS;
    
    /* build the callState request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_CallState; 
    bapReq.req_type     = BapReq_Data;
    bapReq.data_type    = BAP_DT_BYTE;
  
    result = POOL_sfAllocate(   POOL_enBapReqDataType, 
                                (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }

    bapReq.payload.byte_seq.length = (Uint16Type)BAP_MAX_BYTE_SEQ_enAppTelCallState;

    /* reset all to 0x00, al callstates are idle, type unknown call and no * divert */
    (void)memset(bapReq.payload.byte_seq.data, 0, BAP_MAX_BYTE_SEQ_enAppTelCallState);

    /* send callState bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    return SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendCallState                       */
/** 
    \brief      update bap's call state 
  
    \brief      Description:\n
                update bap's call state \n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendCallState()
{
    BapRequest bapReq;
    SuccessFailType result      = SUCCESS;
    CallState callState         = BAP_IDLE;
    CallState callStateIdx[]   = {BAP_IDLE, BAP_IDLE};
    CallType callType           = BAP_UNKNOWN_CALL_TYPE;
    CallType callTypeIdx[]      = {BAP_UNKNOWN_CALL_TYPE, BAP_UNKNOWN_CALL_TYPE};
    Uint16Type numActiveCalls   = 0U;
    Uint16Type numHeldCalls     = 0U;
    Sint16Type i                = 0;

    // get pncc context from APP_PHONE
    const AppPhoneAtPnccContext * pPncc = APP_PHONE_AT_PNCC_pGetPnccContext();
    if( NULL == pPncc )
    {
        return FAIL;
    }
    
    /* build the callState request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_CallState; 
    bapReq.req_type     = BapReq_Data;
    bapReq.data_type    = BAP_DT_BYTE;
  
    result = POOL_sfAllocate(   POOL_enBapReqDataType, 
                                (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }

    bapReq.payload.byte_seq.length = (Uint16Type)BAP_MAX_BYTE_SEQ_enAppTelCallState;

    (void)memset(bapReq.payload.byte_seq.data, 0, BAP_MAX_BYTE_SEQ_enAppTelCallState);

    // *************************************************************************
    // first check if there is more than one call active which means we are in a
    // conference ( communicated from Parrot this way !!!! )
    // *************************************************************************    
    numActiveCalls  = APP_PHONE_AT_PNCC_u16GetNumberOfActiveCalls();
    numHeldCalls    = APP_PHONE_AT_PNCC_u16GetNumberOfHeldCalls();

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(pPncc->plcc[i])
        {
            /* first merge the AT to BAP call states */
            callState   = AT2BapCallStateMatrix[pPncc->plcc[i]->state];

            /* save the current call state for the respective caller ID */
            callStateIdx[i] = callState;

            /* first check if this is a SOS call */

            // TODO: Get the correct call type
            if( numActiveCalls > 1 && BAP_ACTIVE == callState)
            {
                callType = BAP_CONFERENCE;
            }
            else if( numHeldCalls > 1 && BAP_ON_HOLD == callState)
            {
                callType = BAP_CONFERENCE;
            }
            else
            {
                callType = BAP_REGULAR_VOICE_CALL;
            }

            /* save the current call type for the respective caller ID */
            callTypeIdx[i] = callType;

            /* Set CallState i */
            APP_COMMON_UTILS_vSetUpperNibble(bapReq.payload.byte_seq.data + i, (Uint8Type)callState);
            /* Set CallType i */
            APP_COMMON_UTILS_vSetLowerNibble(bapReq.payload.byte_seq.data + i, (Uint8Type)callType);
        }
    }

    DEBUG_VALUE2(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Current call states(CallID0: %d, CallID1: %d)", callStateIdx[0], callStateIdx[1]);

    DEBUG_VALUE2(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Current call types(CallID0: %d, CallID1: %d)", callTypeIdx[0], callTypeIdx[1]);

    // TODO: Handle CallDivert Byte 8 ?! 
 
    /* send callState bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }

    /* confirm the dial processing procedure if pending */
    if(BT_TRUE == gAppPhoneBapContext.bDialNumberPending)
    {
        if((callStateIdx[0] == BAP_DIALING) ||(callStateIdx[1] == BAP_DIALING))
        {
            APP_PHONE_BAP_vProcessDialNumber(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessDialNumber(BAP_RESULT_ERROR);
            DEBUG_VALUE2(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while DialNumber processing(%d, %d)", callStateIdx[0], callStateIdx[1] );
        }
    }

    /* confirm the hang-up call processing procedure if pending */
    if(BT_TRUE == gAppPhoneBapContext.bHangUpCallPending)
    {      // if the first call is terminated
        if(((gAppPhoneBapContext.bHangUpCallIndex == AT_HANGUP_CALL_0)&&((callStateIdx[0] == BAP_IDLE)||(callStateIdx[0] == BAP_DISCONNECTING)))||
           // if the second call is terminated
           ((gAppPhoneBapContext.bHangUpCallIndex == AT_HANGUP_CALL_1)&&((callStateIdx[1] == BAP_IDLE)||(callStateIdx[1] == BAP_DISCONNECTING)))||
           // if both calls are terminated
           ((gAppPhoneBapContext.bHangUpCallIndex == AT_HANGUP_NO_INDEX)&&((callStateIdx[0] == BAP_IDLE)||(callStateIdx[0] == BAP_DISCONNECTING)||
                                                                          (callStateIdx[1] == BAP_DISCONNECTING)||(callStateIdx[1] == BAP_DISCONNECTING))))
        {
            APP_PHONE_BAP_vProcessHangUpCall(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessHangUpCall(BAP_RESULT_ERROR);
            DEBUG_VALUE2(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while HangUp processing(%d, %d)", callStateIdx[0], callStateIdx[1] );
        }
    }

    //RT:4062 inform the application while resuming
    if(BT_TRUE == gAppPhoneBapContext.bResumeCallPending)
    {
        if(((callStateIdx[0] == BAP_ACTIVE) && (callType == BAP_REGULAR_VOICE_CALL))||  // when regular call, only first call active
        ((callStateIdx[0] == BAP_ACTIVE) && (callStateIdx[1] == BAP_ACTIVE) && (callType == BAP_CONFERENCE)))  // else when conf. both calls active
        {
            APP_PHONE_BAP_vProcessResumeCall(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessResumeCall(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while resume call in processing");
        }
    }

    if(BT_TRUE == gAppPhoneBapContext.bAcceptCallPending)
    {
        if((callStateIdx[0] == BAP_ACTIVE) ||(callStateIdx[1] == BAP_ACTIVE))
        {
            APP_PHONE_BAP_vProcessAcceptCall(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessAcceptCall(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while accept call in processing");
        }
    }

    if(BT_TRUE == gAppPhoneBapContext.bMPSwapPending)
    {
        if(((callStateIdx[0] == BAP_ACTIVE) &&(callStateIdx[1] == BAP_ON_HOLD)) ||
           ((callStateIdx[1] == BAP_ACTIVE) &&(callStateIdx[0] == BAP_ON_HOLD)))
        {
            APP_PHONE_BAP_vProcessMPSwap(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessMPSwap(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while swapping calls in processing");
        }
    }

    if(BT_TRUE == gAppPhoneBapContext.bMPCallHoldAcceptWaitingCallPending)
    {
        if((callStateIdx[1] == BAP_ACTIVE) &&(callStateIdx[0] == BAP_ON_HOLD))
        {
            APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while hold call and accept waiting call in processing");
        }
    }

    if(BT_TRUE == gAppPhoneBapContext.bCallHoldPending)
    {
        if(callStateIdx[0] == BAP_ON_HOLD)
        {
            APP_PHONE_BAP_vProcessCallHold(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessCallHold(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while hold call in processing");
        }
    }

    if(BT_TRUE == gAppPhoneBapContext.bCCJoinPending)
    {
        if((callStateIdx[0] == BAP_ACTIVE)&&(callStateIdx[1] == BAP_ACTIVE))
        {
            APP_PHONE_BAP_vProcessCCJoin(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessCCJoin(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while joining calls to conference in processing");
        }
    }

    if(BT_TRUE == gAppPhoneBapContext.bMPReleaseActiveCallAcceptWaitingCallPending)
    {
        if(((callStateIdx[0] == BAP_IDLE) || (callStateIdx[0] == BAP_DISCONNECTING)) &&(callStateIdx[1] == BAP_ACTIVE))
        {
            APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_CALLSTATE_SENT);
        }
        else
        {
            APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_RESULT_ERROR);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "Unexpected call state while release active call accept waiting call in processing");
        }
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendHandsFreeUpdate                 */
/** 
    \brief      update bap's handsfree on / off mode 
  
    \brief      Description:\n
                update bap's handsfree on / off mode \n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_BAP_sfSendHandsFreeUpdate()
{
    if( APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
    {
        // all call states are idle
        // unmute AudioSources
        return _APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_ON);
    }
    return SUCCESS;
}


#if 0
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendMicroMuteUpdate                 */
/** 
    \brief      update bap's micro mute on / off mode 
  
    \brief      Description:\n
                update bap's micro mute on / off mode \n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_sfSendMicroMuteUpdate()
{
    if( APP_PHONE_AT_PNCC_btAllCallStatesIdle() )
    {
        return _APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(BAP_MICRO_UNMUTE);
    }
    return FAIL;
}
#endif

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendActiveUser                      */
/** 
    \brief      Description:\n
                send Active user BAP request

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendActiveUser()
{
    SuccessFailType result  = SUCCESS;
    Uint16Type tmpLen = 0U;
    Uint8Type *pDataWriteBuf;
    BapRequest bapReq;
    const ERMUserProfileType * user         = NULL;
    const ERMPhysicalBTDeviceType *phy      = NULL;
    BooleanType bActiveUserProfileFound     = BT_FALSE;

    /* get active user profile */
    user = ERM_pGetActiveUserProfile();
    if( NULL != user )
    {
        /* try to get device / profile name */
        phy = ERM_pGetPhyDevice(user->deviceIndex);
        if( NULL != phy )
        {
            /* use name of device as active user */
            bActiveUserProfileFound = BT_TRUE;
        }
    }

    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_ActiveUser; 
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "SendActiveUserRequest: POOL_sfAllocate() failed");
        return FAIL;
    }
    
    pDataWriteBuf = bapReq.payload.byte_seq.data;

    if( BT_FALSE == bActiveUserProfileFound )
    {
        /* no active user available */
        /*--------------------------------*/
        /* write the content of SIMSerial */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf, (Uint8Type*)"", 0);
        pDataWriteBuf += tmpLen; 

        /*-----------------------------------*/
        /* write the content of SerialNumber */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf,
                                (const Uint8Type *)"", 
                                0);
        pDataWriteBuf += tmpLen; 

        /*-------------------------------*/
        /* write the content of UserName */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf,
                                (const Uint8Type *)"", 
                                0);
        pDataWriteBuf += tmpLen; 

        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Send empty string for Active User!");
    }
    else
    {   /* active user profile found */
        /*--------------------------------*/
        /* write the content of SIMSerial */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf, (Uint8Type*)"na", 2);
        pDataWriteBuf += tmpLen; 

        /*-----------------------------------*/
        /* write the content of SerialNumber */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf,
                                phy->bt_addr, 
                                sizeof(phy->bt_addr));
        pDataWriteBuf += tmpLen; 

        /*-------------------------------*/
        /* write the content of UserName */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf,
                                phy->name, 
                                phy->name_len);
        pDataWriteBuf += tmpLen; 

        DEBUG_STRING(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "New Active User: %s !", 
                     (const char*) phy->name );
    }

    /* don't forget to set the size of byte sequence */
    bapReq.payload.byte_seq.length = 
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);
     
    /* send callInfo bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfDeactiveUser                      */
/**
    \brief      Description:\n
                send Active user BAP request

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfDeactiveUser()
{
    SuccessFailType result  = SUCCESS;
    Uint16Type tmpLen = 0U;
    Uint8Type *pDataWriteBuf;
    BapRequest bapReq;

    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_ActiveUser;
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "SendActiveUserRequest: POOL_sfAllocate() failed");
        return FAIL;
    }

    pDataWriteBuf = bapReq.payload.byte_seq.data;

    /* no active user available */
    /*--------------------------------*/
    /* write the content of SIMSerial */
    tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf, (Uint8Type*)"", 0);
    pDataWriteBuf += tmpLen;

    /*-----------------------------------*/
    /* write the content of SerialNumber */
    tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                            pDataWriteBuf,
                            (const Uint8Type *)"",
                            0);
    pDataWriteBuf += tmpLen;

    /*-------------------------------*/
    /* write the content of UserName */
    tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                            pDataWriteBuf,
                            (const Uint8Type *)"",
                            0);
    pDataWriteBuf += tmpLen;

    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Send empty string for Active User!");

    /* don't forget to set the size of byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send callInfo bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendUserListStatus                      */
/** 
    \brief      Description:\n
                send userList status array BAP request

    \param      pRecvHeader
                const BapArrayHeaderType *
                if pointer is zero, sending the status array was initiated by
                uhv, if pointer is not null, this shall be a response to the 
                array get request saved in pRecvHeader

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendUserListStatus(const BapArrayHeaderType * pRecvHeader)
{
    SuccessFailType result  = SUCCESS;

    Uint16Type i = 0U;
    Uint8Type headerLength = 0U;

    BapArrayHeaderType sendHeader;
    Uint8Type *pDataWriteBuf;

    BapRequest bapReq;
    
    //ASSERT( gPlpdContext.plpdState == PLPD_VALID );
    
    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_UserList;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted 
    bapReq.data_type    = BAP_DT_BYTE;

    if( pRecvHeader != NULL )
    {
        /* get the mode byte of the header */ 
        sendHeader.mode.data    = pRecvHeader->mode.data;
        /* get the start value of the header */ 
        sendHeader.start        = pRecvHeader->start;
    
        DEBUG_VALUE2(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "UserList was req via BAP: start: %d, elems: %d!",
                     pRecvHeader->start, pRecvHeader->elements);
    }
    else
    {
        /* get the mode byte of the header */ 
        sendHeader.mode.data    = 0U;
        /* get the start value of the header */ 
        sendHeader.start        = 0U;
        
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Internal UserList Update was triggered!");
    }
    /* set the elements value of the header */ 
    sendHeader.elements     = 0U;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }
    
    /* preset the header to get the length (3 or 5) */    
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf       = bapReq.payload.byte_seq.data + headerLength;
    
    /* build up the array content */     
    //for(i=sendHeader.start; i<MAX_PLPD_ELEMENTS; i++)
    for( i= sendHeader.start; i<ERM_MAX_NUM_USER_PROFILES; i++)
    { 
        Uint16Type tmpLen = 0U;
        const ERMUserProfileType * user         = NULL;
        const ERMPhysicalBTDeviceType *phy      = NULL;
    
        /* check if we have an existing profile for i */
        user = ERM_pGetUserProfile((Uint8Type)i);
        if( NULL == user )
        {   /* no user for this index */
            continue;
        }

        /* if we have an active User, get device info like name */
        // get device information for active device id from ERM
        phy = ERM_pGetPhyDevice(user->deviceIndex);
        if(phy == NULL)
        {
            /* device not available */
            continue;
        }

        /* skip writing positon of the record cause pos is not required */
        

        /*--------------------------------*/
        /* write the content of SIMSerial */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf, (Uint8Type*)"na", 2);
        pDataWriteBuf += tmpLen; 

        /*-----------------------------------*/
        /* write the content of SerialNumber */
        /* bt address from btdevice table */ 
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf,
                                phy->bt_addr, 
                                sizeof(phy->bt_addr));
        pDataWriteBuf += tmpLen; 

        /*-------------------------------*/
        /* write the content of UserName */
        tmpLen = APP_COMMON_UTILS_u16WriteBapString( 
                                pDataWriteBuf,
                                phy->name, 
                                phy->name_len);
        pDataWriteBuf += tmpLen; 

        /* increase the number of records by one */ 
        sendHeader.elements++;
        /* check if we have already the number of requested elements */
        if(NULL != pRecvHeader && sendHeader.elements >= pRecvHeader->elements)
        {
            break;
        }
    }
    
    /* write the actual header to the start of the buffer */      
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length = 
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);
    
    /* send UserList bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    DEBUG_VALUE1(APP_PHONE_BAP_SWI_DBG, MOD_APP_PHONE_BAP, "Send New UserList with %d profiles!",
                 sendHeader.elements);
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendUserListChanged                     */
/** 
    \brief      Description:\n
                send userList changed array BAP request

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendUserListChanged()
{
    SuccessFailType     result          = SUCCESS;
    Uint8Type           headerLength    = 0U;
    Uint8Type           *pDataWriteBuf  = NULL;
    BapArrayHeaderType  sendHeader;

    BapRequest bapReq;
    
    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Telephone;
    bapReq.fct_id       = BapFct_Telephone_UserList;
    bapReq.req_type     = BapReq_Changed;  // ChangedArray optype wanted 
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the mode byte of the header */ 
    sendHeader.mode.data    = 0U;
    /* get the start value of the header */ 
    sendHeader.start        = 0U;
    /* set the elements value of the header */ 
    //sendHeader.elements     = ERM_u16GetDeviceCount();    
    sendHeader.elements     = ERM_u16GetNumberOfUserProfiles();    

    if( sendHeader.elements > 0xFF )
    {    
        sendHeader.mode.common.indexSize = 1U;        
    }

    // allocate memory
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "SendUserListChg: POOL_sfAllocate() failed");
        return FAIL;
    }
    
    /* preset the header to get the length (3 or 5) */    
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf       = bapReq.payload.byte_seq.data + headerLength;
    
    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length = 
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);
    
    /* send UserList bapRequest */ 
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest , &bapReq ))
    {
        return FAIL;
    }
    
    DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "SendUserListChg elems: %d",
                       sendHeader.elements);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendSignalQuality                        */
/** 
    \brief      send signal quality property status update
  
    \param      quality
                Uint8Type 
                new value for signal strength. This value is the one received
                from CK5050, so we need to map it to bap range

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendSignalQuality( Uint8Type quality )
{
    BapRequest request;
    /* set all common values */
    request.lsg_id      = BapLsg_Telephone; 
    request.req_type    = BapReq_Data; /* Status msg for Porperty */
    request.fct_id      = BapFct_Telephone_SignalQuality; 

    request.data_type           = BAP_DT_INT8;
    request.payload.u8_value    = _APP_PHONE_BAP_u8GetSignalQualityMapping(
                                  quality);  
    
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest, &request))
    {
        return FAIL;
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendRegisterState                        */
/** 
    \brief      update bap's register state via status message

    \param      atServiceValue
                service availability value received from CK5050, needs to be
                mapped to bap range
                
    \param      atRoamingValue
                indicator if phone is searching for a network or not
  
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendRegisterState( Uint8Type atServiceValue, 
                                                       Uint8Type atRoamingValue )
{
    BapRequest request;
    /* set all common values */
    request.lsg_id      = BapLsg_Telephone; 
    request.req_type    = BapReq_Data; /* Status msg for Porperty */
    request.fct_id      = BapFct_Telephone_RegisterState; 

    request.data_type           = BAP_DT_INT8;
    request.payload.u8_value    = _APP_PHONE_BAP_u8GetRegisterStateMapping( 
                                  atServiceValue,
                                  atRoamingValue);
   
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest, &request))
    {
        return FAIL;
    }
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendNetworkProvider                 */
/** 
    \brief      Update network provider shown in Headunit
  
    \param      opName
                name of network provider

    \param      len
                length of network provider name    

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendNetworkProvider( const Uint8Type * opName, Uint16Type len )
{
    SuccessFailType result = SUCCESS;
    BapRequest request;
    /* set all common values */
    request.lsg_id      = BapLsg_Telephone; 
    request.req_type    = BapReq_Data; /* Status msg for Property */
    request.fct_id      = BapFct_Telephone_NetworkProvider ; 
            
    request.data_type   = BAP_DT_BYTE;

    result = POOL_sfAllocate(POOL_enBapReqDataType,
                             (void**)&request.payload.byte_seq.data);
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }
    
    len = len > BAP_SIZE_NETWORK_PROVIDER ? BAP_SIZE_NETWORK_PROVIDER : len;


    request.payload.byte_seq.length = APP_COMMON_UTILS_u16WriteBapString(   
                                        request.payload.byte_seq.data,
                                        opName, 
                                        len);

    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBapRequest, &request))
    {
        return FAIL;
    }
    return SUCCESS;
}





/* End Of File APP_PHONE_AR_C1.c */


