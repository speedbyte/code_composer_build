/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_AT_C1.c
*-----------------------------------------------------------------------------
* Module Name:    DEVICE Application
*-----------------------------------------------------------------------------
* Description:    Handles all mdi specific requests/responses to/from
                  AT Cmd Parser
*-----------------------------------------------------------------------------
* $Date: 2010-03-09 14:29:55 +0100 (Di, 09 Mrz 2010) $
* $Rev: 11461 $
* $Author: weber $
* $URL:
* http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_DEVICE_AT_C1.c $
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
#include "POOL.h"
#include "ATCmdHandler.h"
#include "ATParser.h"
#include "APP_COMMON_Utils.h"
//#include "MUTE_MANAGER.h"
#include "OSEK_AppTask_Alarm.h"
#include "TRACE.h"
#include "APP_DEVICE_AT_C1.id"

#include "APP_DEVICE.h"
#include "APP_DEVICE_AT.h"
#include "APP_DEVICE_CI.h"
#include "APP_DEVICE_AudioManager.h"
#include "APP_DEVICE_AT_C1.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/**
\brief	variable to save the current device function in progress
*/
extern AppDeviceContext gAppDeviceContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

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
/* Function    : APP_DEVICE_vHandleATResponse                                   */
/**
    \brief      Handles response messages from AtCmdParser

    \param      atRsp
                ATRspAllType *

    \return     void

*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vHandleATResponse(const ATRspMessageType *atRsp)
{
    // check the message
    if ( (AT_RSP_WITH_DATA == atRsp->dataInd) && (NULL == atRsp->data) )
    {
        // seems to be something wrong with the message, ignore it
        return;
    }
    else if (   (AT_RSP_WITHOUT_DATA == atRsp->dataInd) &&
                (NULL != atRsp->data) )
    {
        return;
    }

    switch( atRsp->common.rsp_type )
    {
        case AT_RSP_AMGMT_SRD_ACTIVE_IND:       /* PASA */
            gAppDeviceContext.AudioSource = atRsp->data->pasa.state;
            APP_DEVICE_AM_vHandlePASA(atRsp->data->pasa.state);
            break;
        default:            
            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "_APP_DEVICE_vHandleATResponse no valid response type: %d", 
                          atRsp->common.rsp_type);
            break;
    }
}/* END OF APP_DEVICE_vHandleATResponse() */


/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_AT_sfHandlePASA                                      */
/**
  \brief        Parse and process incoming PASA messages

  \param        pPasa
                ATResAudioSrcActType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
//static SuccessFailType _APP_DEVICE_AT_sfHandlePASA( const ATResAudioSrcActType *pPasa )
//{
//    static MUTE_MANAGER_EN_MUTESOURCES lastMute = enMuteManagerInvalidSource;
//
//    gAppDeviceContext.AudioSource = pPasa->state;
//
//    switch(pPasa->state)
//    {
//        case AT_AUDIO_STATE_NO_ACTIVE:
////            if(lastMute != enMuteManagerInvalidSource)
////            {
////                (void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioUnmute, lastMute, __FILE__, __LINE__);
////                lastMute = enMuteManagerInvalidSource;
////            }
//            APP_DEVICE_AM_vPasaReceived(AT_AUDIO_STATE_NO_ACTIVE);
//            break;
//        case AT_AUDIO_STATE_CK5050:
//            lastMute = enMuteManagerRing;
//            (void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioMute, lastMute, __FILE__, __LINE__);
//            break;
////        case AT_AUDIO_STATE_COMMUNICATION:
////            lastMute = enMuteManagerTelephoneCall;
////            (void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioMute, lastMute);
////            break;
//        case AT_AUDIO_STATE_STREAMING_A2DP:
////#if (1 == WORKAROUND_RT_2667_IGNORE_FIRST_PASA3_EVENT)
////            /*
////               Parrot sends a wrong PASA3 Event after PlayerConnect.
////               this leads to wrong setting of the Mute Bit
////               this workaround ignores for a specific time this wrong event
////               As soon Parrot provides a solution this workaround could be removed
////            */
////            if ((osGetSystemCounter() - gAppMdiContext.PlayerConnectTime) < 7000)
////            {
//////                DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "PL-Connect->ignore PASA-Event");
////            }
////            else
////#endif
////            {
////               _APP_DEVICE_vNotifyApps(APP_DEVICE_A2DP_STREAMING_ACTIVE);
////               lastMute = enMuteManagerAudioStream;
////               (void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioMute, lastMute, __FILE__, __LINE__);
////            }
//            break;
////        case AT_AUDIO_STATE_BT_AUDIO:
////               lastMute = enMuteManagerAudioStream;
////            (void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioMute, lastMute);
////            break;
//        case AT_AUDIO_STATE_SIVR_PROMPTS:
//            break;
//        case AT_AUDIO_STATE_I2S:
//        case AT_AUDIO_STATE_STREAMING_UART:
//        case AT_AUDIO_STATE_STREAMING_USB:
//        case AT_AUDIO_STATE_BT_AUDIO:
//        default:
//            (void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioUnmute, enMuteManagerInvalidSource, __FILE__, __LINE__);
//            /* ignore other key events */
////            DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_DEVICE, "sfHandlePASA: invalid state %d", pPasa->state);
//            break;
//    }
//    return SUCCESS;
//}



/*----------------------------------------------------------------------------*/
/* Function :   APP_DEVICE_GetPASAEvent                                      */
/**
  \brief        Get actiev Audio Source of CK5050+

  \param        none


  \return       last received PASA Event of CK5050+

 */
/*----------------------------------------------------------------------------*/
ATAudioSourceActStateEnum APP_DEVICE_GetPASAEvent(void)
{
    return (gAppDeviceContext.AudioSource);
}


void APP_DEVICE_InitPASAEvent(void)
{
    /* Parrot doesn't send a initial PASA Event, this function should be called if Parrot does a restart */
    gAppDeviceContext.AudioSource = AT_AUDIO_STATE_NO_ACTIVE;
}




/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfStartServiceATRequest                              */
/**
  \brief        Builds and sends a service start AT request

  \param        deviceId
                AT_DeviceId

  \param        service
                ATBTServiceEnum

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfStartServiceATRequest(   const SCI_ST    *pUart,
                                                    AT_DeviceId     deviceId,
                                                    ATBTServiceEnum service)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_BT_SERVICE_CONN;
    request.pbsc.device_id      = deviceId;
    request.pbsc.action         = AT_SERVICE_ACT_START;
    request.pbsc.service_avail  = AT_TRUE;
    request.pbsc.service        = service;

    return AT_Cmd_Handler_Device(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfStopServiceATRequest                               */
/**
  \brief        Builds and sends a service stop AT request

  \param        deviceId
                AT_DeviceId

  \param        service
                ATBTServiceEnum

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfStopServiceATRequest( const SCI_ST    *pUart,  
                                                 AT_DeviceId     deviceId,
                                                 ATBTServiceEnum service)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_BT_SERVICE_CONN;
    request.pbsc.device_id      = deviceId;
    request.pbsc.action         = AT_SERVICE_ACT_STOP;
    request.pbsc.service_avail  = AT_TRUE;
    request.pbsc.service        = service;

    return AT_Cmd_Handler_Device(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfConnectDeviceATRequest                             */
/**
  \brief        Builds and sends a connect device AT request

  \param        deviceId
                AT_DeviceId

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfConnectDeviceATRequest( const SCI_ST    *pUart,
                                                   AT_DeviceId     deviceId)
{
    ATReqAllType request;

    request.common.req_type         = AT_REQ_BT_SELECT_DEV;
    request.psbd.device_id          = deviceId;
    request.psbd.value              = AT_DEVSEL_CONNECT;

    return AT_Cmd_Handler_Device(&request, pUart);
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfConnectDeviceATRequest                             */
/**
  \brief        Builds and sends a connect device AT request

  \param        deviceId
                AT_DeviceId

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfDisconnectDeviceATRequest( const SCI_ST    *pUart,
                                                   AT_DeviceId     deviceId)
{
    ATReqAllType request;

    request.common.req_type         = AT_REQ_BT_SELECT_DEV;
    request.psbd.device_id          = deviceId;
    request.psbd.value              = AT_DEVSEL_DISCONNECT;

    return AT_Cmd_Handler_Device(&request, pUart);
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfSendALCKATRequest                                  */
/**
  \brief        Builds and sends a ALCK AT request

  \param        ATAlckLockType
                AT_ALCK_LOCK_START or AT_ALCK_LOCK_STOP

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfSendALCKATRequest(   const SCI_ST    *pUart,
                                                ATAlckLockType  lockState)
{
    ATReqAllType request;

    request.common.req_type         = AT_REQ_AMGMT_AUDIO_LOCK;
    request.alck.locked             = lockState;

    return AT_Cmd_Handler_Device(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfSendGetImsiRequest                                  */
/**
  \brief        Builds and sends a ALCK AT request

  \param        ATAlckLockType
                AT_ALCK_LOCK_START or AT_ALCK_LOCK_STOP

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfSendGetImsiRequest(   const SCI_ST    *pUart)
{
    ATReqAllType request;

    request.common.req_type         = AT_REQ_IMSI;    

    return AT_Cmd_Handler_Device(&request, pUart);
}
    
/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_sfSendDisableMdiATRequest                            */
/**
  \brief        Builds and sends a PBSV AT request

  \param        service
                ATBTServiceEnum

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfSendPbsvATRequest( const SCI_ST *pUart,
                                              ATBTServiceEnum service,
                                              ATPbsvActionType action)
{
    ATReqAllType request;

    request.common.req_type  = AT_REQ_BT_SERVICE_VISIBILITY;
    request.pbsv.service     = service;
    request.pbsv.action      = action;

    return AT_Cmd_Handler_Device(&request, pUart);
}

/* End Of File APP_DEVICE_AT_C1.c */


