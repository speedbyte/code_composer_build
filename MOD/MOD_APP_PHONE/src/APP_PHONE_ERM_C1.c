/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_ERM_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application
*-----------------------------------------------------------------------------
* Description:    Handles events received from ERM  
*-----------------------------------------------------------------------------
* $Date$
* $Rev$
* $Author$
* $URL$
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Pool.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "bap_types.h"
#include "ATCmdHandler.h"
#include "APP_COMMON_Utils.h"
#include "BAP_DISPATCHER_CE.h"
#include "ERM.h"
#include "APP_DEVICE_AudioManager.h"

#include "APP_PHONE_ERM_C1.id"
#include "TRACE.h"

#include "APP_PHONE.h"
#include "APP_PHONE_AT_Request_Builder.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_PHONE_CAN_DATA.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_CI.h"
#include "APP_PHONE_CI.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable to save the current call management function currently in progress
*/
extern AppPhoneContext gAppPhoneContext;

//used for Hang up all calls handling
static Uint8Type _APP_PHONE_u8RetryNb = 0;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _APP_PHONE_vHandleErmATResult(ATResponseEnum rsp);
static void _APP_PHONE_vOnDialResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnDialIndexResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnRedialResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnDialNumberTypeResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnAcceptCallResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnHangUpCallResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnHangUpAllCallResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnPhoneStatusResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnCallStateResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnCallInfoResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnSendDtmfResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnPlpdRequestResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnCallHoldResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnCallReplaceResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnResumeCallResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnHandsFreeOnOffResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnHeadsetOnOffResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnMicroMuteOnOffResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnSwitchCallsResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnJoinConferenceResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnSplitConferenceResult(APP_COMMON_enResponseCode rsp);
//static void _APP_PHONE_vOnRelActAccWaitCall(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnPutCallsThrough(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vOnHfpVersionResult(APP_COMMON_enResponseCode rsp);
static void _APP_PHONE_vSetOugoingCallDetectionTimer();
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

void _APP_PHONE_vHandleErmATResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    if(rsp == AT_RSP_OK)
    {
        response = APP_COMMON_RSP_OK;
    }
    else
    {
        response = APP_COMMON_RSP_ERROR;
    }

    switch( gAppPhoneContext.fct.common.fctId )
    {
        case APP_PHONE_DIAL_NUMBER:
            _APP_PHONE_vOnDialResult(response);
            break;
        case APP_PHONE_DIAL_INDEX:
            _APP_PHONE_vOnDialIndexResult(response);
            break;
        case APP_PHONE_REDIAL:
            _APP_PHONE_vOnRedialResult(response);
            break;
        case APP_PHONE_DIAL_NUMBER_TYPE:
            _APP_PHONE_vOnDialNumberTypeResult(response);
            break;
        case APP_PHONE_ACCEPT_CALL:
            _APP_PHONE_vOnAcceptCallResult(response);
            break;
        case APP_PHONE_HANGUP_CALL:
            _APP_PHONE_vOnHangUpCallResult(response);
            break;
        case APP_PHONE_HANGUP_ALL_CALL:
			_APP_PHONE_vOnHangUpAllCallResult(response);
			break;
        case APP_PHONE_PHONE_STATUS:
            _APP_PHONE_vOnPhoneStatusResult(response);
            break;
        case APP_PHONE_CALL_STATE:
            _APP_PHONE_vOnCallStateResult(response);
            break;
        case APP_PHONE_CALL_INFO:
            _APP_PHONE_vOnCallInfoResult(response);
            break;
        case APP_PHONE_SEND_DTMF:
            _APP_PHONE_vOnSendDtmfResult(response);
            break;
        case APP_PHONE_PLPD_REQUEST:
            _APP_PHONE_vOnPlpdRequestResult(response);
            break;
        case APP_PHONE_CALL_HOLD:
            _APP_PHONE_vOnCallHoldResult(response);
            break;
        case APP_PHONE_CALL_REPLACE:
            _APP_PHONE_vOnCallReplaceResult(response);
            break;
        case APP_PHONE_RESUME_CALL:
            _APP_PHONE_vOnResumeCallResult(response);
            break;
        case APP_PHONE_HEADSET_ON_OFF:
            _APP_PHONE_vOnHeadsetOnOffResult(response);
            break;
        case APP_PHONE_HANDS_FREE_ON_OFF:
            _APP_PHONE_vOnHandsFreeOnOffResult(response);
            break;
        case APP_PHONE_MICRO_MUTE_ON_OFF:
            _APP_PHONE_vOnMicroMuteOnOffResult(response);
            break;
        case APP_PHONE_SWITCH_CALLS:
            _APP_PHONE_vOnSwitchCallsResult(response);
            break;
        case APP_PHONE_JOIN_CONFERENCE:
            _APP_PHONE_vOnJoinConferenceResult(response);
            break;
        case APP_PHONE_SPLIT_CONFERENCE:
            _APP_PHONE_vOnSplitConferenceResult(response);
            break;
        case APP_PHONE_PUT_CALLS_THROUGH:
            _APP_PHONE_vOnPutCallsThrough(response);
            break;
//        case APP_PHONE_RELEASE_ACTIVE_ACCEPT_WAITING_CALL:
//            _APP_PHONE_vOnRelActAccWaitCall(response);
//            break;
        case APP_PHONE_REQUEST_HFP_VERSION:
            _APP_PHONE_vOnHfpVersionResult(response);
            break;
        default:
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vHandleErmATOk: invalid fctId %d (rsp %d)", 
                           gAppPhoneContext.fct.common.fctId, rsp);

            _APP_PHONE_vReleaseFctContext();
            break;
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vHandleErmATOkEvent                                   */
/** 
    \brief      handle event from ERM when AT Parser sent OK 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vHandleErmATOkEvent(void)
{
    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaErmATOk: fctId %d", 
                   gAppPhoneContext.fct.common.fctId);
    
    _APP_PHONE_vHandleErmATResult(AT_RSP_OK);

}/* END OF APP_PHONE_vHandleErmATOkEvent() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vHandleErmATErrorEvent                             */
/** 
    \brief      handle event from ERM when AT Parser sent ERROR 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT ERROR \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vHandleErmATErrorEvent(void)
{
    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaErmATError: fctId %d", 
                   gAppPhoneContext.fct.common.fctId);
    
    _APP_PHONE_vHandleErmATResult(AT_RSP_ERROR);
}/* END OF APP_PHONE_vHandleErmATErrorEvent() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM to Update e.g. FSG OperationState
  
    \brief      Description:\n
                handle State change of ERM to Update e.g. FSG OperationState \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vHandleErmStateChange(ERMStateType newState)
{
    switch( newState )
    {
        //case ERM_STATE_CK5050_BT_ACTIVE:
        //    _APP_PHONE_sfRequestHfpVersion();
        //    break;
        case ERM_STATE_CK5050_ACTIVE:
            (void)_APP_PHONE_vReInitApp();
            break;
        default:
            break;
    }
}/* END OF APP_PHONE_vHandleErmStateChange() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vOnErmNotification                                 */
/** 
    \brief      handle Notifications of ERM
  
    \brief      Description:\n
                handle Notifications of ERM \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vOnErmNotification(const ERMNotificationType * pNotification)
{
    switch(pNotification->common.id)
    {
        //case ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED:
        case ERM_NOTIFY_PBSN_PHONE:
            if( ERM_u8GetActivePhone() == AT_INVALID_DEVICE_ID )
            {
                /* reinit after phone disconnect */
                (void)_APP_PHONE_vReInitApp();
            }
            else
            {
                (void)_APP_PHONE_sfRequestHfpVersion();
                _APP_PHONE_CAN_DATA_vSendPhoneOn(BT_TRUE);
            }
            break;
        default:
            /* ignore notification */
            break;
    }
}

void _APP_PHONE_vOnDialResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.dial.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
        case APP_PHONE_GET_MUTE_SENT:
            /* this should not happen, but to be sure, do not release context */
            break;
        case APP_PHONE_DIAL_COMMAND_SENT:
            if( APP_COMMON_RSP_OK == rsp )
            {
                // TODO: set alarm to be able to detect missing indication for outgoing call
                _APP_PHONE_vSetOugoingCallDetectionTimer();
            }
            else
            {
                /* release the mute only if all call states are idle */
                if(BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
                {
                    APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);
                }
                _APP_PHONE_vNotifyApp(APP_PHONE_OUTG_CALL_ERROR);
            }
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "OnDialResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.dial.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnDialIndexResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.dialIndex.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
        case APP_PHONE_GET_MUTE_SENT:
            /* this should not happen, but to be sure, do not release context */
            break;
        case APP_PHONE_DIAL_COMMAND_SENT:
            if( APP_COMMON_RSP_OK == rsp )
            {
                // TODO: set alarm to be able to detect missing indication for outgoing call
                _APP_PHONE_vSetOugoingCallDetectionTimer();
            }
            else
            {
                /* release the mute only if all call states are idle */
                if(BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
                {
                    APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);
                }
                _APP_PHONE_vNotifyApp(APP_PHONE_OUTG_CALL_ERROR);
            }
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnDialIndexResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.dialIndex.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnRedialResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.redial.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
        case APP_PHONE_GET_MUTE_SENT:
            /* this should not happen, but to be sure, do not release context */
            break;
        case APP_PHONE_DIAL_COMMAND_SENT:
            if( APP_COMMON_RSP_OK == rsp )
            {
                // TODO: set alarm to be able to detect missing indication for outgoing call
                _APP_PHONE_vSetOugoingCallDetectionTimer();
            }
            else
            {
                /* release the mute only if all call states are idle */
                if(BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
                {
                    APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);
                }
                _APP_PHONE_vNotifyApp(APP_PHONE_OUTG_CALL_ERROR);
            }
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnRedialResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.redial.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnDialNumberTypeResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.dialNumberType.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
        case APP_PHONE_GET_MUTE_SENT:
            /* this should not happen, but to be sure, do not release context */
            break;
        case APP_PHONE_DIAL_COMMAND_SENT:
            if( APP_COMMON_RSP_OK == rsp )
            {
                // TODO: set alarm to be able to detect missing indication for outgoing call
                _APP_PHONE_vSetOugoingCallDetectionTimer();
            }
            else
            {
                /* release the mute only if all call states are idle */
                if(BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
                {
                    APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);
                }
                _APP_PHONE_vNotifyApp(APP_PHONE_OUTG_CALL_ERROR);
            }
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnDialNumberTypeResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.dialNumberType.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnAcceptCallResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.acceptCall.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;
    
        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnAcceptCallResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.acceptCall.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnHangUpCallResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.hangUpCall.state)
    {
        case APP_PHONE_COMMAND_SENT:
            if(rsp != APP_COMMON_RSP_OK)
            {   // received error from ck5050, so reset flag cause hangup wasn't successful
                gAppPhoneContext.isHangUpSent = BT_FALSE;
            }
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnHangUpCallResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.hangUpCall.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

//additional function for hang up all calls handling
void APP_PHONE_vResetHangUpAllCallsRetries()
{
	_APP_PHONE_u8RetryNb = 0;
}

void _APP_PHONE_vOnHangUpAllCallResult(APP_COMMON_enResponseCode rsp)
{
	switch(gAppPhoneContext.fct.hangUpCall.state)
    {
		case APP_PHONE_NEXT_COMMAND_NOT_SENT:
			_APP_PHONE_u8RetryNb++;
			if(rsp != APP_COMMON_RSP_OK)
			{   // received error from ck5050, so reset flag cause single hang up wasn't successful
				gAppPhoneContext.isHangUpSent = BT_FALSE;
				_APP_PHONE_u8RetryNb = 0;
				_APP_PHONE_vSendResultReleaseContext(rsp);
			}
			else
			{
				//we have to hang up second call
				if(APP_PHONE_MAX_NB_RETRIES_HANGUP_CALL < _APP_PHONE_u8RetryNb)
				{
					//retry NB. is out
					gAppPhoneContext.isHangUpSent = BT_FALSE;
					_APP_PHONE_u8RetryNb = 0;
					_APP_PHONE_vSendResultReleaseContext(rsp);
					//release resource
		            (void)ERM_ReleaseUartForTel();
				}
				else
				{
					//retries are not out of range
					//try to hang up one call, so do nothing here
				}
			}

			break;

		case APP_PHONE_COMMAND_SENT:
            if(rsp != APP_COMMON_RSP_OK)
            {   // received error from ck5050, so reset flag cause hang up wasn't successful
                gAppPhoneContext.isHangUpSent = BT_FALSE;
            }
            _APP_PHONE_u8RetryNb = 0;
            _APP_PHONE_vSendResultReleaseContext(rsp);
			//release resource
            (void)ERM_ReleaseUartForTel();
            break;

        default:
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnHangUpAllCallResult: invalid state %d (fctId %d)",
                           gAppPhoneContext.fct.hangUpCall.state,
                           gAppPhoneContext.fct.common.fctId );
            _APP_PHONE_u8RetryNb = 0;
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            //release resource
            (void)ERM_ReleaseUartForTel();
            break;
    }
}

void _APP_PHONE_vOnPhoneStatusResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.phoneStatus.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnPhoneStatusResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.phoneStatus.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnCallStateResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.callState.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnCallStateResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.callState.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnCallInfoResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.callInfo.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnCallInfoResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.callInfo.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnSendDtmfResult(APP_COMMON_enResponseCode rsp)
{
    if(rsp != APP_COMMON_RSP_OK)
    {
        _APP_PHONE_vSendResultReleaseContext(rsp);
    }
    else
    {
        switch(gAppPhoneContext.fct.dtmf.state)
        {
            case APP_PHONE_COMMAND_SENT:
                if(gAppPhoneContext.fct.dtmf.sendIdx < gAppPhoneContext.fct.dtmf.length)
                {
                    /* send the next dtmf char */
                    gAppPhoneContext.fct.dtmf.state = APP_PHONE_COMMAND_NOT_SENT;
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(500),0);
                }
                else
                {
                    /* we are finished */
                    _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_OK);
                }
                break;

            default:
                _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
                DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnSendDtmfResult: invalid state %d (fctId %d)", 
                               gAppPhoneContext.fct.dtmf.state,
                               gAppPhoneContext.fct.common.fctId );
                break;
        }
    }
}


void _APP_PHONE_vOnPlpdRequestResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.plpdReq.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnPlpdRequestResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.plpdReq.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnCallHoldResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.callHold.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnCallHoldResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.callHold.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnCallReplaceResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.callReplace.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnCallReplaceResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.callReplace.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnResumeCallResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.resumeCall.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnResumeCallResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.resumeCall.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

void _APP_PHONE_vOnHeadsetOnOffResult(APP_COMMON_enResponseCode rsp)
{
    if(rsp != APP_COMMON_RSP_OK)
    {
        _APP_PHONE_vSendResultReleaseContext(rsp);
    }
    else
    {
        switch(gAppPhoneContext.fct.headsetOnOff.state)
        {
            case APP_PHONE_COMMAND_SENT:
                if(gAppPhoneContext.fct.headsetOnOff.mode == AT_AUDIO_GW_MODE_ON)
                {
                    gAppPhoneContext.isHeadsetOn = BT_TRUE;			// RT 2276
                }
                else
                {
                    gAppPhoneContext.isHeadsetOn = BT_FALSE;		// RT 2276
                }
                _APP_PHONE_vNotifyApp(APP_PHONE_HEADSET_MODE_CHANGED);
                _APP_PHONE_vSendResultReleaseContext(rsp);
                break;

            default:
                _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
                DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnHeadsetOnOffResult: invalid state %d (fctId %d)", 
                               gAppPhoneContext.fct.headsetOnOff.state,
                               gAppPhoneContext.fct.common.fctId );
                break;
        }
    }
}


void _APP_PHONE_vOnHandsFreeOnOffResult(APP_COMMON_enResponseCode rsp)
{
    if(rsp != APP_COMMON_RSP_OK)
    {
        _APP_PHONE_vSendResultReleaseContext(rsp);
    }
    else
    {
        switch(gAppPhoneContext.fct.handsFreeOnOff.state)
        {
            case APP_PHONE_COMMAND_SENT:
                // SUSANNE: 05.10.2009, #2967 HFP Mode change should be indicated only through receiving the
                // associated PATR event, not already when we receive an AT_OK for AT*PATR= command
                //if(gAppPhoneContext.fct.handsFreeOnOff.mode == AT_AUDIO_DISCRETE_MODE)
                //{
                //    gAppPhoneContext.isHandsFreeOff = BT_TRUE;
                //}
                //else
                //{
                //    gAppPhoneContext.isHandsFreeOff = BT_FALSE;
                //}
                //_APP_PHONE_vNotifyApp(APP_PHONE_HANDSFREE_MODE_CHANGED);
                _APP_PHONE_vSendResultReleaseContext(rsp);
                break;

            default:
                _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
                DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnHandsFreeOnOffResult: invalid state %d (fctId %d)", 
                               gAppPhoneContext.fct.handsFreeOnOff.state,
                               gAppPhoneContext.fct.common.fctId );
                break;
        }
    }
}


void _APP_PHONE_vOnMicroMuteOnOffResult(APP_COMMON_enResponseCode rsp)
{
    if(rsp != APP_COMMON_RSP_OK)
    {
        _APP_PHONE_vSendResultReleaseContext(rsp);
    }
    else
    {
        switch(gAppPhoneContext.fct.microMuteOnOff.state)
        {
            case APP_PHONE_COMMAND_SENT:
                if(gAppPhoneContext.fct.microMuteOnOff.mode == AT_MIC_MUTE)
                {
                    gAppPhoneContext.isMicMuted = BT_TRUE;
                }
                else
                {
                    gAppPhoneContext.isMicMuted = BT_FALSE;
                }
                
                _APP_PHONE_vNotifyApp(APP_PHONE_MIC_MUTE_CHANGED);
                _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_OK);
                break;
    
            default:
                _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
                DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnMicroMuteOnOffResult: invalid state %d (fctId %d)", 
                               gAppPhoneContext.fct.microMuteOnOff.state,
                               gAppPhoneContext.fct.common.fctId );
                break;
        }
    }
}


void _APP_PHONE_vOnSwitchCallsResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.switchCalls.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnSwitchCallsResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.switchCalls.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnJoinConferenceResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.joinConference.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnJoinConferenceResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.joinConference.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnSplitConferenceResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.splitConference.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnSplitConferenceResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.splitConference.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}

//void _APP_PHONE_vOnRelActAccWaitCall(APP_COMMON_enResponseCode rsp)
//{
//    switch(gAppPhoneContext.fct.relActAccWaitCall.state)
//    {
//        case APP_PHONE_ACCEPT_SENT:
//            if(rsp == APP_COMMON_RSP_OK)
//            {
//                /* do step 2: release the active call */
//                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
//            }
//            else
//            {
//                _APP_PHONE_vSendResultReleaseContext(rsp);
//            }
//            break;
//        case APP_PHONE_RELEASE_SENT:
//            _APP_PHONE_vSendResultReleaseContext(rsp);
//            break;
//        default:
//            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
//            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnRelActAccWaitCall: invalid state %d (fctId %d)", 
//                           gAppPhoneContext.fct.relActAccWaitCall.state,
//                           gAppPhoneContext.fct.common.fctId );
//            break;
//    
//    }
//}

void _APP_PHONE_vOnPutCallsThrough(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.putCallsThrough.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnPutCallsThrough: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.putCallsThrough.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vOnHfpVersionResult(APP_COMMON_enResponseCode rsp)
{
    switch(gAppPhoneContext.fct.requestHfpVersion.state)
    {
        case APP_PHONE_COMMAND_SENT:
            _APP_PHONE_vSendResultReleaseContext(rsp);
            break;

        default:
            _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            DEBUG_VALUE2(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "vOnHfpVersionResult: invalid state %d (fctId %d)", 
                           gAppPhoneContext.fct.requestHfpVersion.state,
                           gAppPhoneContext.fct.common.fctId );
            break;
    }
}


void _APP_PHONE_vSetOugoingCallDetectionTimer()
{
    
    //(void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioMute, enMuteManagerTelephoneCall, __FILE__, __LINE__);
    //APP_DEVICE_AM_vReleaseMute(NULL, APP_DEVICE_AM_enTelephoneCall, __LINE__);
    DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SetOuCallDet: called" );

    // when call state change has been detected, and there is a new outging call, this value shall be reset to BT_FALSE and timer shall be canceled    
    gAppPhoneContext.bWaitForOutgCallIndication = BT_TRUE;
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_OUTGOING_CALL_INDICATION_TIMER,MSEC(APP_PHONE_OUTGOING_CALL_INDICATION_TIMEOUT),0);
}

/* End Of File APP_PHONE_ERM_C1.c */


