/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_PHONE
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP and
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2011-11-02 09:35:15 +0100 (Mi, 02 Nov 2011) $
* $Rev: 22941 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONE_C1.c $
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

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "APP_PHONEBOOK.h"
#include "ATCmdHandler.h"
#include "ERM.h"
#include "UART.h"
#include "APP_SD.h"
#ifdef UHV_ACTIVATE_FEATURE_LED 
#include "LED.h"
#endif
#include "APP_CSHDL.h"
#include "APP_DEVICE.h"
#include "APP_SMS.h"
#include "APP_DEVICE_AudioManager.h"
#include "APP_PHONE_BAP.h"
#include "APP_PHONE_AT_Request_Builder.h"

#include "APP_PHONE_C1.id"
#include "TRACE.h"

#include "APP_PHONE_AT.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_CI.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_PHONE_CAN_DATA.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current call management function currently in progress
*/
AppPhoneContext gAppPhoneContext;
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
/**
\brief	description of the method
*/
static SuccessFailType _APP_PHONE_sfHandlePendingFunction( const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingDial(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingDialIndex(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingRedial(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingDialNumberType(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingAcceptCall(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingHangUpCall(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingHangUpAllCalls(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingPhoneStatus(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingCallState(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingCallInfo(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingDtmf(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingPlpdRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingCallHold(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingCallReplace(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingResumeCall(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingHandsFreeOnOff(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingHeadsetOnOff(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingMicroMuteOnOff(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingSwitchCalls(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingJoinConference(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingSplitConference(const SCI_ST* puart);
//static SuccessFailType _APP_PHONE_sfSendPendingRelActAccWaitCall(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingPutCallsThrough(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingHfpVersionRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONE_sfSendPendingSilentRinging();


//static void _APP_PHONE_vPausePlayerBeforeDialAndSetPendingAlarm();
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

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_u16MissedCalls                         */
/**
    \brief      function to check if there are missed calls available, needed

    \brief      Description:\n

    \return     Uint16Type
                number of available missed calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_u16MissedCalls()
{
    //return 0;
    return gAppPhoneContext.u16MissedCalls;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_u16MissedNumbers                         */
/**
    \brief      function to check if there are missed numbers available, needed

    \brief      Description:\n

    \return     Uint16Type
                number of available missed numbers
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_u16MissedNumbers()
{
    //return 0;
    return gAppPhoneContext.u16MissedNumbers;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vResetMissedCalls                         */
/**
    \brief      function to reset missed calls
                for VCR

    \brief      Description:\n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vResetMissedCalls()
{
    gAppPhoneContext.u16MissedCalls     = 0U;
    gAppPhoneContext.u16MissedNumbers   = 0U;

    memset(&gAppPhoneContext.u32MissedCallerIdHashTable[0], 0x00, sizeof(gAppPhoneContext.u32MissedCallerIdHashTable));
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_sfInitApp                                         */
/**
    \brief      Callback function for initialization of AppTelephone

    \brief      Description:\n

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONE_sfInitApp(void)
{
    (void)memset(&gAppPhoneContext, 0, sizeof(gAppPhoneContext));

    // init pncc
    (void)_APP_PHONE_AT_PNCC_sfInit();
    //_APP_PHONE_AT_PLPD_sfInit();

    /* set piev values to invalid values */
    gAppPhoneContext.piev_signalStrength    = APP_PHONE_PIEV_INVALID_VALUE;
    gAppPhoneContext.piev_batteryLevel      = APP_PHONE_PIEV_INVALID_VALUE;

    gAppPhoneContext.isHandsFreeOff = BT_TRUE;


    _APP_PHONE_CAN_DATA_vSendPhoneOn(BT_FALSE);
    _APP_PHONE_CAN_DATA_vSendPhoneActiveCall(BT_FALSE);
    _APP_PHONE_CAN_DATA_vSendPhoneRing(BT_FALSE);
    _APP_PHONE_CAN_DATA_vSendProvider(NULL, 0);
    _APP_PHONE_CAN_DATA_vSendFieldStrength(APP_PHONE_PIEV_INVALID_VALUE);
    _APP_PHONE_CAN_DATA_vSendRegisterState(APP_PHONE_REG_STATE_NOT_AVAILABLE);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_vReInitApp                                    */
/**
    \brief      reinit the phone context

    \brief      Description:\n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_vReInitApp(void)
{

    /* RT: 2347, after phone disconnect InitApp is called and we need to check if
           a callback waits for result, otherwise it will wait forever */
    if(gAppPhoneContext.fct.common.callback)
    {
        DEBUG_VALUE1(  APP_PHONE_SWI_WNG, MOD_APP_PHONE, "vReInitApp: callback(ERR) for %d",
                             gAppPhoneContext.fct.common.fctId);

        gAppPhoneContext.fct.common.callback(APP_COMMON_RSP_ERROR);
    }

    (void)ERM_ReleaseUartForTel();

    APP_PHONE_sfInitApp();
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_PHONE_vReleaseFctContext                                 */
/**
  \brief        Reset the current function context

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _APP_PHONE_vReleaseFctContext()
{
    (void)memset(&gAppPhoneContext.fct, 0, sizeof(gAppPhoneContext.fct));
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_vSendResultReleaseContext                     */
/**
    \brief      notify caller of last function ( if available ) about result
                and release the function context

    \brief      Description:\n
                notify caller of last function ( if available ) about result\n
                and release the function context

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_vSendResultReleaseContext(APP_COMMON_enResponseCode result)
{
    DEBUG_VALUE2(  APP_PHONE_SWI_TRC, MOD_APP_PHONE, "RelCtx: fctId %d, res %d",
                    gAppPhoneContext.fct.common.fctId,
                    result );

    if(gAppPhoneContext.fct.common.callback)
    {
        gAppPhoneContext.fct.common.callback(result);
    }

    _APP_PHONE_vReleaseFctContext();

//    if( APP_COMMON_RSP_OK != result)
//    {   // e.g. when there is an error after dial
//        _APP_PHONE_vRestartPlayer();
//    }

    /* check for queued internal functions */
    if(gAppPhoneContext.pending.toggleMicMute)
    {
        gAppPhoneContext.pending.toggleMicMute = BT_FALSE;
        (void)APP_PHONE_rcToggleMicMute(NULL);
    }
    else if(gAppPhoneContext.pending.requestHfpVersion)
    {
        gAppPhoneContext.pending.requestHfpVersion = BT_FALSE;
        (void)_APP_PHONE_sfRequestHfpVersion();
    }
    else if( BT_TRUE == gAppPhoneContext.pending.resetHandsfreeToOn )
    {
        (void)APP_PHONE_rcHandsFreeOnOff(NULL, AT_AUDIO_HANDSFREE_MODE);
        gAppPhoneContext.pending.resetHandsfreeToOn = BT_FALSE;
    }
    else if( BT_TRUE == gAppPhoneContext.pending.resetHandsfreeToOff)
    {
        (void)APP_PHONE_rcHandsFreeOnOff(NULL, AT_AUDIO_DISCRETE_MODE);
        gAppPhoneContext.pending.resetHandsfreeToOff = BT_FALSE;
    }
    else if( BT_TRUE == gAppPhoneContext.pending.unmuteMicro )
    {
        (void)APP_PHONE_rcMicroMuteOnOff(NULL, AT_MIC_UNMUTE);
        gAppPhoneContext.pending.unmuteMicro = BT_FALSE;
    }
    else if( BT_TRUE == gAppPhoneContext.pending.resetHeadsetToOff )			// RT 2276
    {
        (void)APP_PHONE_rcHeadsetOnOff(NULL, AT_AUDIO_GW_MODE_OFF);
        gAppPhoneContext.pending.resetHeadsetToOff = BT_FALSE;
    }
	else
    {
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_HangUp_CallStatusChange                     */
/**
    \brief      handle Hang up all calls action in case of call status change reply from Parrot

    \brief      Description:\n
                handle Hang up all calls action in case of call status change reply from Parrot

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_HangUp_CallStatusChange(void)
{
	//check if we have a second command
	if (gAppPhoneContext.fct.hangUpCall.state == APP_PHONE_NEXT_COMMAND_NOT_SENT)
	{
		//check if alarm is running
		if( BT_TRUE == APP_COMMON_UTILS_bIsAlarmActive( CTA_APP_PHONE_FCT_PENDING) )
		{
			(void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING,
															MSEC(0), //execute now second AT+CHUP
															0);
		}
	}
}

void _APP_PHONE_vNotifyApp(APP_PHONE_enNotificationId notification)
{
    // notify all applications
    APP_PHONE_CAN_DATA_vOnAppPhoneNotification(notification);
    APP_PHONE_BAP_vOnAppPhoneNotification(notification);
    APP_SD_vOnAppPhoneNotification(notification);
#ifdef UHV_ACTIVATE_FEATURE_LED
    LED_vOnAppPhoneNotification(notification);
#endif
    APP_DEVICE_vOnAppPhoneNotification(notification);
    APP_SMS_vOnPhoneNotification(notification);
    ERM_vOnAppPhoneNotification(notification);
}

void APP_PHONE_vCbMicMute( void )
{
    /* workaround for APP3BM, it would be better if it accesses the ToggleMicMute
     * function directly */
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CbMicMute: toggle");

    // we need to mute / unmute micro
    response = APP_PHONE_rcToggleMicMute(NULL);
    switch( response )
    {
        case APP_COMMON_RSP_ERROR_BUSY:
            gAppPhoneContext.pending.toggleMicMute = BT_TRUE;

            break;
        case APP_COMMON_RSP_OK:
            /* the toggle will be done */
            break;
        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "CbMicMute: toggle failed (%d)",
                         response );
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vHandleEvents                                                 */
/**
    \brief      Task Function to handle events for Telephone Application Task

    \brief      Description:\n
                Waits for incoming events and if one arrives function \n
                reads Messages received From BAP-Dispatcher and \n
                AT_Cmd_Parser and handles them. \n

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/

void APP_PHONE_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************

    // remove event evAppTaskPhoneTResponse and message msgPhoneATResponse
    // from APP_PHONE_vHandleEvents to central AT-handling in
    // APP_COMMON_vHandleEvents

    if( event & evAppTaskPhoneFctPending )
    {
        const SCI_ST * puart    = NULL;
        (void)ClearEvent( evAppTaskPhoneFctPending );

        /* special handling for missing outgoing call indication by Ck5050
           we need to notify applications about this, no uart needed here ! */
        if(BT_TRUE == gAppPhoneContext.pending.indicateOutgCallError)
        {
            gAppPhoneContext.pending.indicateOutgCallError = BT_FALSE;

            // unmute only, if all calls are idle and no incoming nor hold calls are present
            if( BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
            {
                //(void)MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioUnmute, enMuteManagerTelephoneCall, __FILE__, __LINE__);
                APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);

                DEBUG_TEXT(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "HandleEvents: OutgCallIndTimeout!");
                _APP_PHONE_vNotifyApp(APP_PHONE_OUTG_CALL_INDICATION_TIMEOUT);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_SWI_WNG, MOD_APP_PHONE, "HdlEvents: OutgCallIndTimeout ign.");
            }
        }

        if(gAppPhoneContext.pending.patrTimeout == BT_TRUE)
        {
            gAppPhoneContext.pending.patrTimeout = BT_FALSE;
            DEBUG_TEXT( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "patrTimout -> privateCall" );

            gAppPhoneContext.isHandsFreeOff = BT_TRUE;
            _APP_PHONE_vNotifyApp(APP_PHONE_HANDSFREE_MODE_CHANGED);
        }

        //RS: prevent from handle only one event although more events were set
        if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT)
        {
            if(ERM_bIsStartupDone() != BT_TRUE)
            {
                /* we are not able to to request the UART in this state */
                DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "HandleEvents: CK5050_SHUTDOWN, drop pending, fct %d",
                              gAppPhoneContext.fct.common.fctId);

                _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            }
            else
            {
                // try to get ressource
                puart = ERM_GetUartForTel();
                if( !puart )
                {
                    // ressource is busy, set an alarm to try again in some cycles
                    // set event once again after 10 ms
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING,
                                                        MSEC(ERM_UART_BUSY_RETRY_TIME),
                                                        0);
                }
                else
                {
                    // call common function to handle next step
                    (void)_APP_PHONE_sfHandlePendingFunction(puart);
                }
            }
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_sfRequestHfpVersion()                            */
/**
    \brief      request hfp version of active user

    \brief      Description:\n
                request hfp version of active user  \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_sfRequestHfpVersion()
{

    if( gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT)
    {
        /* queue request */
        gAppPhoneContext.pending.requestHfpVersion = BT_TRUE;

        return FAIL;
    }

    gAppPhoneContext.fct.common.fctId          = APP_PHONE_REQUEST_HFP_VERSION;
    gAppPhoneContext.fct.common.callback       = NULL;

    gAppPhoneContext.fct.requestHfpVersion.state   = APP_PHONE_COMMAND_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING, MSEC(0),0);

    return SUCCESS;
}


static SuccessFailType _APP_PHONE_sfHandlePendingFunction( const SCI_ST* puart )
{
    SuccessFailType result = SUCCESS;

    switch(gAppPhoneContext.fct.common.fctId)
    {
        case APP_PHONE_DIAL_NUMBER:
            result = _APP_PHONE_sfSendPendingDial(puart);
            break;
        case APP_PHONE_DIAL_INDEX:
            result = _APP_PHONE_sfSendPendingDialIndex(puart);
            break;
        case APP_PHONE_REDIAL:
            result = _APP_PHONE_sfSendPendingRedial(puart);
            break;
        case APP_PHONE_DIAL_NUMBER_TYPE:
            result = _APP_PHONE_sfSendPendingDialNumberType(puart);
            break;
        case APP_PHONE_ACCEPT_CALL:
            result = _APP_PHONE_sfSendPendingAcceptCall(puart);
            break;
        case APP_PHONE_HANGUP_CALL:
            result = _APP_PHONE_sfSendPendingHangUpCall(puart);
            break;
        case APP_PHONE_HANGUP_ALL_CALL:
			result = _APP_PHONE_sfSendPendingHangUpAllCalls(puart);
			break;
        case APP_PHONE_PHONE_STATUS:
            result = _APP_PHONE_sfSendPendingPhoneStatus(puart);
            break;
        case APP_PHONE_CALL_STATE:
            result = _APP_PHONE_sfSendPendingCallState(puart);
            break;
        case APP_PHONE_CALL_INFO:
            result = _APP_PHONE_sfSendPendingCallInfo(puart);
            break;
        case APP_PHONE_SEND_DTMF:
            result = _APP_PHONE_sfSendPendingDtmf(puart);
            break;
        case APP_PHONE_PLPD_REQUEST:
            result = _APP_PHONE_sfSendPendingPlpdRequest(puart);
            break;
        case APP_PHONE_CALL_HOLD:
            result = _APP_PHONE_sfSendPendingCallHold(puart);
            break;
        case APP_PHONE_CALL_REPLACE:
            result = _APP_PHONE_sfSendPendingCallReplace(puart);
            break;
        case APP_PHONE_RESUME_CALL:
            result = _APP_PHONE_sfSendPendingResumeCall(puart);
            break;
        case APP_PHONE_HEADSET_ON_OFF:
            result = _APP_PHONE_sfSendPendingHeadsetOnOff(puart);
            break;
        case APP_PHONE_HANDS_FREE_ON_OFF:
            result = _APP_PHONE_sfSendPendingHandsFreeOnOff(puart);
            break;
        case APP_PHONE_MICRO_MUTE_ON_OFF:
            result = _APP_PHONE_sfSendPendingMicroMuteOnOff(puart);
            break;
        case APP_PHONE_SWITCH_CALLS:
            result = _APP_PHONE_sfSendPendingSwitchCalls(puart);
            break;
        case APP_PHONE_JOIN_CONFERENCE:
            result = _APP_PHONE_sfSendPendingJoinConference(puart);
            break;
        case APP_PHONE_SPLIT_CONFERENCE:
            result = _APP_PHONE_sfSendPendingSplitConference(puart);
            break;
//        case APP_PHONE_RELEASE_ACTIVE_ACCEPT_WAITING_CALL:
//            result = _APP_PHONE_sfSendPendingRelActAccWaitCall(puart);
//            break;
        case APP_PHONE_PUT_CALLS_THROUGH:
            result = _APP_PHONE_sfSendPendingPutCallsThrough(puart);
            break;
        case APP_PHONE_REQUEST_HFP_VERSION:
            result = _APP_PHONE_sfSendPendingHfpVersionRequest(puart);
            break;
        case APP_PHONE_SILENT_RINGING:
            (void)ERM_ReleaseUartForTel();
            result = _APP_PHONE_sfSendPendingSilentRinging();
            break;
        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "HandlePendFct: unsup fctId %d",
                           gAppPhoneContext.fct.common.fctId);
            result = FAIL;
            break;
    }

    if( SUCCESS != result )
    {
        _APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
        (void)ERM_ReleaseUartForTel();
    }

    return result;
}



static SuccessFailType _APP_PHONE_sfSendPendingDial(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendDial: state %d",
                   gAppPhoneContext.fct.dial.state);

    switch(gAppPhoneContext.fct.dial.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
            (void)ERM_ReleaseUartForTel();
            /* try to get mute. Note that we go on anyway, also if get mute failed */
            if(SUCCESS != APP_DEVICE_AM_sfGetMute(_APP_PHONE_MuteAck_Cb, APP_CAN_enTelephoneCall, __LINE__))
            {
                gAppPhoneContext.fct.dial.state      = APP_PHONE_DIAL_COMMAND_NOT_SENT;
                // set alarm to start immediately
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
            }
            // else do not set alarm, cause we are waiting for mute callback
            gAppPhoneContext.fct.dial.state      = APP_PHONE_GET_MUTE_SENT;
            break;
        case APP_PHONE_GET_MUTE_SENT:
            (void)ERM_ReleaseUartForTel();
            /* waiting for mute ack cb */
            break;
        case APP_PHONE_DIAL_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfDialNumber( puart,
                                                         gAppPhoneContext.fct.dial.number,
                                                         gAppPhoneContext.fct.dial.number_len);

            gAppPhoneContext.fct.dial.state = APP_PHONE_DIAL_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendDial: invalid state %d",
                            gAppPhoneContext.fct.dial.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingDialIndex(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendDialIndex: state %d",
                 gAppPhoneContext.fct.dialIndex.state);

    switch(gAppPhoneContext.fct.dialIndex.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
            (void)ERM_ReleaseUartForTel();
            /* try to get mute. Note that we go on anyway, also if get mute failed */
            if(SUCCESS != APP_DEVICE_AM_sfGetMute(_APP_PHONE_MuteAck_Cb, APP_CAN_enTelephoneCall, __LINE__))
            {
                gAppPhoneContext.fct.dialIndex.state      = APP_PHONE_DIAL_COMMAND_NOT_SENT;
                // set alarm to start immediately
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
            }
            // else do not set alarm, cause we are waiting for mute callback
            gAppPhoneContext.fct.dialIndex.state      = APP_PHONE_GET_MUTE_SENT;
            break;
        case APP_PHONE_GET_MUTE_SENT:
            (void)ERM_ReleaseUartForTel();
            /* waiting for mute ack cb */
            break;
        case APP_PHONE_DIAL_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfDialIndex( puart,
                                                        gAppPhoneContext.fct.dialIndex.pbIndex,
                                                        gAppPhoneContext.fct.dialIndex.pnIndex);
            gAppPhoneContext.fct.dialIndex.state = APP_PHONE_DIAL_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendDialIdx: invalid state %d",
                         gAppPhoneContext.fct.dialIndex.state);
            result = FAIL;
            break;
    }

    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingRedial(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC,MOD_APP_PHONE, "SendPendRedial: state %d",
                   gAppPhoneContext.fct.redial.state);

    switch(gAppPhoneContext.fct.redial.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
            (void)ERM_ReleaseUartForTel();
            /* try to get mute. Note that we go on anyway, also if get mute failed */
            if(SUCCESS != APP_DEVICE_AM_sfGetMute(_APP_PHONE_MuteAck_Cb, APP_CAN_enTelephoneCall, __LINE__))
            {
                gAppPhoneContext.fct.redial.state      = APP_PHONE_DIAL_COMMAND_NOT_SENT;
                // set alarm to start immediately
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
            }
            // else do not set alarm, cause we are waiting for mute callback
            gAppPhoneContext.fct.redial.state      = APP_PHONE_GET_MUTE_SENT;
            break;
        case APP_PHONE_GET_MUTE_SENT:
            (void)ERM_ReleaseUartForTel();
            /* waiting for mute ack cb */
            break;
        case APP_PHONE_DIAL_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfDialNumber( puart,
                                                         gAppPhoneContext.fct.redial.number,
                                                         gAppPhoneContext.fct.redial.number_len);

            gAppPhoneContext.fct.redial.state = APP_PHONE_DIAL_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR,MOD_APP_PHONE, "sfSendPendingRedial: invalid state %d",
                            gAppPhoneContext.fct.redial.state);
            result = FAIL;
            break;
    }

    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingDialNumberType(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendDialNumType: state %d",
                   gAppPhoneContext.fct.dialNumberType.state);

    switch(gAppPhoneContext.fct.dialNumberType.state)
    {
        case APP_PHONE_GET_MUTE_NOT_SENT:
            (void)ERM_ReleaseUartForTel();
            /* try to get mute. Note that we go on anyway, also if get mute failed */
            if(SUCCESS != APP_DEVICE_AM_sfGetMute(_APP_PHONE_MuteAck_Cb, APP_CAN_enTelephoneCall, __LINE__))
            {
                gAppPhoneContext.fct.dialNumberType.state      = APP_PHONE_DIAL_COMMAND_NOT_SENT;
                // set alarm to start immediately
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
            }
            // else do not set alarm, cause we are waiting for mute callback
            gAppPhoneContext.fct.dialNumberType.state      = APP_PHONE_GET_MUTE_SENT;
            break;

        case APP_PHONE_GET_MUTE_SENT:
            (void)ERM_ReleaseUartForTel();
            /* waiting for mute ack cb */
            break;
        case APP_PHONE_DIAL_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfDialNumberType( puart,
                                                            gAppPhoneContext.fct.dialNumberType.pbIndex,
                                                            gAppPhoneContext.fct.dialNumberType.type);

            gAppPhoneContext.fct.dialNumberType.state = APP_PHONE_DIAL_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR,MOD_APP_PHONE, "SendPendDialNumType: invalid state %d",
                           gAppPhoneContext.fct.dialNumberType.state);
            result = FAIL;
            break;
    }

    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingAcceptCall(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendAcceptCallType: state %d",
                   gAppPhoneContext.fct.acceptCall.state);

    switch(gAppPhoneContext.fct.acceptCall.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfAcceptCall(puart);

            gAppPhoneContext.fct.acceptCall.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendAcceptCallType: invalid state %d",
                           gAppPhoneContext.fct.acceptCall.state);
            result = FAIL;
            break;
    }

    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingHangUpCall(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendHangUp: state %d",
                 gAppPhoneContext.fct.hangUpCall.state);

    switch(gAppPhoneContext.fct.hangUpCall.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfHangUp( puart,
                                                    gAppPhoneContext.fct.hangUpCall.callIndex);

            gAppPhoneContext.fct.hangUpCall.state = APP_PHONE_COMMAND_SENT;


            if( (result == SUCCESS) &&
                (_APP_PHONE_AT_PNCC_u16GetValidPlccNumber() == 1) )
            {
                /* if there is only one call left to hangup set the patr prevent flag */
                DEBUG_VALUE1(APP_PHONE_SWI_TRC,MOD_APP_PHONE, "PendHangUp: last call, set patr flag",
                                    gAppPhoneContext.fct.hangUpCall.state);

                gAppPhoneContext.isHangUpSent = BT_TRUE;
            }
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendHangUp: invalid state %d",
                           gAppPhoneContext.fct.hangUpCall.state);
            result = FAIL;
            break;
    }

    return result;
}

SuccessFailType _APP_PHONE_Execute_HangUp(const SCI_ST* puart, ATCallHangupEnum callIndex, const ATRspCallStatValType *nextPlcc)
{
	SuccessFailType result = SUCCESS;
	//execute hang up
	result = _APP_PHONE_AT_REQUEST_sfHangUp( puart,
											callIndex);

	gAppPhoneContext.fct.hangUpCall.state = APP_PHONE_NEXT_COMMAND_NOT_SENT;
	(void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING,
											MSEC(APP_PHONE_TIMEOUT_HANGUP_CALL), //2000ms  needed before second AT+CHUP
											0);
	return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingHangUpAllCalls(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;
    const ATRspCallStatValType *plcc1 = APP_PHONE_AT_PNCC_pGetPlccContext(AT_CALL_INDEX_0);
    const ATRspCallStatValType *plcc2 = APP_PHONE_AT_PNCC_pGetPlccContext(AT_CALL_INDEX_1);

	switch(gAppPhoneContext.fct.hangUpCall.state)
	{
		case APP_PHONE_COMMAND_NOT_SENT:
			APP_PHONE_vResetHangUpAllCallsRetries();
		case APP_PHONE_NEXT_COMMAND_NOT_SENT:
			//first call on hold
			if( (plcc1 != NULL) && plcc1->state == AT_CS_HELD_CALL )
			{
				//first call on hold
				//execute hang up for first call
				result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_CALL_0, plcc2);
			}
			else if( (plcc2 != NULL) && plcc2->state == AT_CS_HELD_CALL )
			{
				//second call on hold
				//execute hang up for second call
				result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_CALL_1, plcc1);
			}
			else if( (plcc1 != NULL) && (plcc1->state > AT_CS_ACTIVE_CALL && plcc1->state <= AT_CS_WAITING_CALL) )
			{
				//first call exists and isn't active call
				//execute hang up for first call
				result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_CALL_0, plcc2);
			}
			else if( (plcc2 != NULL) && (plcc2->state > AT_CS_ACTIVE_CALL && plcc2->state <= AT_CS_WAITING_CALL) )
			{
				//second call exists and isn't active call
				//execute hang up for second call
				result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_CALL_1, plcc1);
			}
			else if( (plcc1 != NULL) && plcc1->state == AT_CS_ACTIVE_CALL )
			{
				//first call exists and isn't active call
				//check if conference is active
				if( (plcc2 != NULL) && plcc2->state == AT_CS_ACTIVE_CALL )
				{
					//execute hang up for conference call
					result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_NO_INDEX, NULL);
				}
				else
				{
					//execute hang up for first call
					result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_CALL_0, plcc2);
				}
			}
			else if( (plcc2 != NULL) && plcc2->state == AT_CS_ACTIVE_CALL )
			{
				//second call exists and isn't active call
				//check if conference is active
				if( (plcc1 != NULL) && plcc1->state == AT_CS_ACTIVE_CALL )
				{
					//execute hang up for conference call
					result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_NO_INDEX, NULL);
				}
				else
				{
					//execute hang up for second call
					result = _APP_PHONE_Execute_HangUp(puart, AT_HANGUP_CALL_1, plcc1);
				}
			}
			else
			{
				//nothing to hang up
                DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendHangUp: nothing to hang up");
				gAppPhoneContext.fct.hangUpCall.state = APP_PHONE_COMMAND_SENT;
				gAppPhoneContext.isHangUpSent = BT_FALSE;
				_APP_PHONE_vSendResultReleaseContext(APP_COMMON_RSP_OK);
				//release resource
				(void)ERM_ReleaseUartForTel();
			}
			break;

		default:
			DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendHangUpAll: invalid state %d",
						   gAppPhoneContext.fct.hangUpCall.state);
			result = FAIL;
			break;
	}

    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingPhoneStatus(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendPhoneStatus: state %d",
                   gAppPhoneContext.fct.phoneStatus.state);

    switch(gAppPhoneContext.fct.phoneStatus.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfPhoneStatus( puart );

            gAppPhoneContext.fct.phoneStatus.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendPhoneStatus: invalid state %d",
                           gAppPhoneContext.fct.phoneStatus.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingCallState(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendCallState: state %d",
                   gAppPhoneContext.fct.callState.state);

    switch(gAppPhoneContext.fct.callState.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfCallState( puart );

            gAppPhoneContext.fct.callState.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR,MOD_APP_PHONE, "SendPendCallState: invalid state %d",
                           gAppPhoneContext.fct.callState.state);
            result = FAIL;
            break;
    }
    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingCallInfo(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC,MOD_APP_PHONE, "SendPendCallInfo: state %d",
                   gAppPhoneContext.fct.callInfo.state);

    switch(gAppPhoneContext.fct.callInfo.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfHandleCallInfo( puart );

            gAppPhoneContext.fct.callInfo.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendCallInfo: invalid state %d",
                           gAppPhoneContext.fct.callInfo.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingDtmf(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE2(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendDtmf: state %d idx %d",
                 gAppPhoneContext.fct.dtmf.state,
                 gAppPhoneContext.fct.dtmf.sendIdx);

    switch(gAppPhoneContext.fct.dtmf.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
        {
            Uint8Type dtmfChar = gAppPhoneContext.fct.dtmf.value[gAppPhoneContext.fct.dtmf.sendIdx];
            gAppPhoneContext.fct.dtmf.sendIdx++;

            result = _APP_PHONE_AT_REQUEST_sfSendDTMF( puart,
                                                       dtmfChar );

            gAppPhoneContext.fct.dtmf.state = APP_PHONE_COMMAND_SENT;
            break;
        }
        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendDtmf: invalid state %d",
                           gAppPhoneContext.fct.dtmf.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingPlpdRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendPlpdReq: state %d",
                   gAppPhoneContext.fct.plpdReq.state);

    switch(gAppPhoneContext.fct.plpdReq.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfPlpdRequest(puart);

            gAppPhoneContext.fct.plpdReq.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendPlpdReq: invalid state %d",
                           gAppPhoneContext.fct.plpdReq.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingCallHold(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendCallHold: state %d",
                   gAppPhoneContext.fct.callHold.state);

    switch(gAppPhoneContext.fct.callHold.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfCallHold(puart);

            gAppPhoneContext.fct.callHold.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendCallHold: invalid state %d",
                           gAppPhoneContext.fct.callHold.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingCallReplace(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendCallReplace: state %d",
                   gAppPhoneContext.fct.callReplace.state);

    switch(gAppPhoneContext.fct.callReplace.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfCallReplace(puart);

            gAppPhoneContext.fct.callReplace.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendCallReplace: invalid state %d",
                           gAppPhoneContext.fct.callReplace.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingResumeCall(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendResumeCall: state %d",
                   gAppPhoneContext.fct.resumeCall.state);

    switch(gAppPhoneContext.fct.resumeCall.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfResumeCall(puart);

            gAppPhoneContext.fct.resumeCall.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendResumeCall: invalid state %d",
                           gAppPhoneContext.fct.resumeCall.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType  _APP_PHONE_sfSendPendingHeadsetOnOff(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendHsOnOff: state %d",
                   gAppPhoneContext.fct.headsetOnOff.state);

    switch(gAppPhoneContext.fct.headsetOnOff.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfHeadsetOnOff(puart,
                                                          gAppPhoneContext.fct.headsetOnOff.mode );

            gAppPhoneContext.fct.headsetOnOff.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendHsOnOff: invalid state %d",
                           gAppPhoneContext.fct.headsetOnOff.state);
            result = FAIL;
            break;
    }

    return result;
}




static SuccessFailType _APP_PHONE_sfSendPendingHandsFreeOnOff(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendHandsFreeOnOff: state %d",
                   gAppPhoneContext.fct.handsFreeOnOff.state);

    switch(gAppPhoneContext.fct.handsFreeOnOff.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfHandsFreeOnOff(puart,
                                                            gAppPhoneContext.fct.handsFreeOnOff.mode );


            gAppPhoneContext.fct.handsFreeOnOff.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendHandsFreeOnOff: invalid state %d",
                           gAppPhoneContext.fct.handsFreeOnOff.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingMicroMuteOnOff(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendMicroMuteOnOff: state %d",
                   gAppPhoneContext.fct.microMuteOnOff.state);

    switch(gAppPhoneContext.fct.microMuteOnOff.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfMicroMuteOnOff(puart,
                                                            gAppPhoneContext.fct.microMuteOnOff.mode);

            gAppPhoneContext.fct.microMuteOnOff.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendMicroMuteOnOff: invalid state %d",
                           gAppPhoneContext.fct.microMuteOnOff.state);
            result = FAIL;
            break;
    }

    return result;
}


static SuccessFailType _APP_PHONE_sfSendPendingSwitchCalls(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendSwitchCalls: state %d",
                   gAppPhoneContext.fct.switchCalls.state);

    switch(gAppPhoneContext.fct.switchCalls.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfSwitchCalls(puart);

            gAppPhoneContext.fct.switchCalls.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendSwitchCalls: invalid state %d",
                           gAppPhoneContext.fct.switchCalls.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingJoinConference(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendJoinConf: state %d",
                   gAppPhoneContext.fct.joinConference.state);

    switch(gAppPhoneContext.fct.joinConference.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfJoinToConference(puart);

            gAppPhoneContext.fct.joinConference.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendJoinConf: invalid state %d",
                           gAppPhoneContext.fct.joinConference.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingSplitConference(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendSplitConf: state %d",
                   gAppPhoneContext.fct.splitConference.state);

    switch(gAppPhoneContext.fct.splitConference.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfSplitCallFromConference(puart,
                                                       gAppPhoneContext.fct.splitConference.callIndex);

            gAppPhoneContext.fct.splitConference.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendSplitConf: invalid state %d",
                           gAppPhoneContext.fct.splitConference.state);
            result = FAIL;
            break;
    }

    return result;
}


//static SuccessFailType _APP_PHONE_sfSendPendingRelActAccWaitCall(const SCI_ST* puart)
//{
//    SuccessFailType result = SUCCESS;
//
//    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendRelActAccWaitCall: state %d",
//                   gAppPhoneContext.fct.relActAccWaitCall.state);
//
//    switch(gAppPhoneContext.fct.relActAccWaitCall.state)
//    {
//        case APP_PHONE_ACCEPT_NOT_SENT:
//            result = _APP_PHONE_AT_REQUEST_sfAcceptCall(puart);
//            gAppPhoneContext.fct.relActAccWaitCall.state = APP_PHONE_ACCEPT_SENT;
//            break;
//
//        case APP_PHONE_ACCEPT_SENT:
//            result = _APP_PHONE_AT_REQUEST_sfHangUp(puart,
//                                                    gAppPhoneContext.fct.relActAccWaitCall.call);
//            gAppPhoneContext.fct.relActAccWaitCall.state = APP_PHONE_RELEASE_SENT;
//            break;
//
//        default:
//            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendRelActAccWaitCall: invalid state %d",
//                           gAppPhoneContext.fct.relActAccWaitCall.state);
//            result = FAIL;
//            break;
//    }
//
//    return result;
//}


static SuccessFailType _APP_PHONE_sfSendPendingPutCallsThrough(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendPutCallsThrough: state %d",
                   gAppPhoneContext.fct.putCallsThrough.state);

    switch(gAppPhoneContext.fct.putCallsThrough.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfPutCallsThrough(puart);

            gAppPhoneContext.fct.putCallsThrough.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendPutCallsThrough: invalid state %d",
                           gAppPhoneContext.fct.putCallsThrough.state);
            result = FAIL;
            break;
    }

    return result;
}

static SuccessFailType _APP_PHONE_sfSendPendingHfpVersionRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendHfpVers: state %d",
                   gAppPhoneContext.fct.requestHfpVersion.state);

    switch(gAppPhoneContext.fct.requestHfpVersion.state)
    {
        case APP_PHONE_COMMAND_NOT_SENT:
            result = _APP_PHONE_AT_REQUEST_sfRequestHfpVersion(puart);

            gAppPhoneContext.fct.requestHfpVersion.state = APP_PHONE_COMMAND_SENT;
            break;

        default:
            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "SendPendHfpVersReq: invalid state %d",
                           gAppPhoneContext.fct.requestHfpVersion.state);
            result = FAIL;
            break;
    }

    return result;


}

static SuccessFailType _APP_PHONE_sfSendPendingSilentRinging(void)
{
    SuccessFailType result = SUCCESS;

    APP_COMMON_enResponseCode rspCode = ERM_ePlayRingtone( NULL, AT_PPRT_ACT_STOP, AT_RTMODE_DEACTIVATE);

    if( rspCode == APP_COMMON_RSP_ERROR_BUSY )
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SD_FCT_PENDING,
                                            MSEC(10),
                                            0);
        return SUCCESS;
    }

    // don´t release the telephone mute here, no mute granted for the telephone application.
    // mute reason is still CAN_Ring at this time.
    //(void)APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);

    DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "SendPendingSilentRinging finished");

    _APP_PHONE_vSendResultReleaseContext(rspCode);

    return result;
}


// ***************  INTERNAL INTERFACES ************************************* //

// *************** EXTERNAL INTERFACES FOR APPS ***************************** //

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcDialNumber                                */
/**
    \brief      handles dial and redial number

    \brief      Description:\n
                handles dial and redial number \n

    \param      cb
                callback which is used to notify calling application that
                dialing the number has been successfully finished or that an
                error occurred

    \param      numberString
                const Uint8Type
                contains the number which shall be dialed

    \param      numberLen
                Uint16Type
                number of bytes valid in numberString

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...

*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcDialNumber(  void (*cb)(APP_COMMON_enResponseCode),
                                                   const Uint8Type * numberString,
                                                   Uint16Type numberLen )
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    if((numberLen > AT_MAX_PHONE_NUM_LEN) || (numberLen == 0))
    {
        DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "DialNumber: wrong length = %d", 
                     numberLen);
        return APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId    = APP_PHONE_DIAL_NUMBER;
    gAppPhoneContext.fct.common.callback = cb;
    gAppPhoneContext.fct.dial.state      = APP_PHONE_GET_MUTE_NOT_SENT;
    gAppPhoneContext.fct.dial.number_len = numberLen;

    (void)memcpy(gAppPhoneContext.fct.dial.number, numberString, numberLen);

    // set alarm to start immediately
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;

}   /* END OF _APP_PHONE_AT_REQUEST_sfDialNumber()                            */


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcDialPbIndex                                      */
/**
    \brief      builds AT request to dial a pn index

    \brief      Description:\n
                builds AT request to dial a pn index \n

    \param      cb
                callback which is used to notify calling application that
                dialing the number has been successfully finished or that an
                error occurred

    \param      pbIndex
                Uint16Type
                pb index to be dialed

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcDialPbIndex( void (*cb)(APP_COMMON_enResponseCode),
                                                    Uint16Type pbIndex)
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_DIAL_INDEX;
    gAppPhoneContext.fct.common.callback    = cb;
    gAppPhoneContext.fct.dialIndex.state    = APP_PHONE_GET_MUTE_NOT_SENT;
    gAppPhoneContext.fct.dialIndex.pbIndex  = pbIndex;
    gAppPhoneContext.fct.dialIndex.pnIndex  = 0U;

    // set alarm to start immediately
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcSilentRinging                                    */
/**
    \brief      build AT request and mute to set call in silent

    \brief      Description:\n
                build AT request and mute to set call in silent \n

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcSilentRinging(void)
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_SILENT_RINGING;
    // set alarm to start immediately
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_sfDialPnIndex                                      */
/**
    \brief      builds AT request to dial a pn index

    \brief      Description:\n
                builds AT request to dial a pn index \n

    \param      cb
                callback which is used to notify calling application that
                dialing the number has been successfully finished or that an
                error occurred

    \param      pbIndex
                Uint16Type

    \param      pnIndex
                Uint16Type

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcDialPnIndex( void (*cb)(APP_COMMON_enResponseCode),
                                                    Uint16Type pbIndex,
                                                    Uint16Type pnIndex )
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_DIAL_INDEX;
    gAppPhoneContext.fct.common.callback    = cb;
    gAppPhoneContext.fct.dialIndex.state    = APP_PHONE_GET_MUTE_NOT_SENT;
    gAppPhoneContext.fct.dialIndex.pbIndex  = pbIndex;
    gAppPhoneContext.fct.dialIndex.pnIndex  = pnIndex;

    // set alarm to start immediately
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcRedial                                           */
/**
    \brief      builds AT request to redial last dialled number

    \brief      Description:\n
                builds AT request to redial last dialled number \n

    \param      cb
                callback function

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcRedial( void (*cb)(APP_COMMON_enResponseCode) )
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // check if we have a dialled call stack. If not give ERROR back!!
    if(0        == APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enLastDialed) /* ||
       BT_FALSE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enLastDialed)*/ )
    {
        // SUSANNE: 14.11.2009
        // I tested this , but Lars Krämer mentioned that we don't need to show a popup
        // in APP_SD so no trigger needed here.
        // We can add the Notificication line again if VW decides different sometimes
        // generate a notification to applications especially APP_SD to be able to show
        // a notification
        // _APP_PHONE_vNotifyApp(APP_PHONE_REDIAL_DENIED_DUE_TO_EMPTY_STACK);
        return APP_COMMON_RSP_ERROR;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_REDIAL;
    gAppPhoneContext.fct.common.callback    = cb;
    gAppPhoneContext.fct.redial.number_len  = 0U;
    gAppPhoneContext.fct.redial.state       = APP_PHONE_GET_MUTE_NOT_SENT;

    // set alarm to start immediately
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

// SUSANNE: 26.03.2010: not used anymore
//void _APP_PHONE_vPausePlayerBeforeDialAndSetPendingAlarm()
//{
//    // stop player if running
//    if(BT_TRUE == APP_MDI_bPlayerActive())
//    {
//        if(SUCCESS != APP_MDI_sfSetPlayMode(APP_MDI_PAUSE))
//        {
//
//            DEBUG_VALUE1( APP_PHONE_SWI_ERR, MOD_APP_PHONE, "Cannot paus. act. player, fct %d",
//                          gAppPhoneContext.fct.common.fctId);
//        }
//        // save that we have to restart the palyer when all calls are idle again :)
//        APP_PHONE_vSetRestartPlayerOnCallIdleState();
//        // set alarm, make alarm take more time to exceed than we would use if player not started
//        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(1000),0);
//    }
//    else
//    {
//        // set alarm to start immediately
//        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
//    }
//}

// SUSANNE: 26.03.2010: seems to be not used
///*----------------------------------------------------------------------------*/
///* Function    : APP_PHONE_rcDialNumberType                                   */
///**
//    \brief      builds AT request to dial a given numberType of pbIndex
//
//    \brief      Description:\n
//                builds AT request to dial a given numberType of pbIndex \n
//
//    \param      cb
//                callback which is used to notify calling application that
//                dialing the number has been successfully finished or that an
//                error occurred
//
//    \param      pbIndex
//                Uint16Type
//                index within the phonebook of the currently connected phone
//                which shall be dialed
//
//    \param      numberType
//                ATPhoneNumberTypeEnum
//                category like home, office, mobile etc. of the pb index which
//                shall be dialed
//
//    \return     APP_COMMON_enResponseCode
//                APP_COMMON_RSP_OK or ...
//*/
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode APP_PHONE_rcDialNumberType( void (*cb)(APP_COMMON_enResponseCode),
//                                                      Uint16Type pbIndex,
//                                                      ATPhoneNumberTypeEnum numberType )
//{
//    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
//    {
//        return APP_COMMON_RSP_ERROR_BUSY;
//    }
//
//
//    // set callBack for OK / ERROR response
//    gAppPhoneContext.fct.common.fctId    = APP_PHONE_DIAL_NUMBER_TYPE;
//    gAppPhoneContext.fct.common.callback = cb;
//
//    gAppPhoneContext.fct.dialNumberType.state   = APP_PHONE_COMMAND_NOT_SENT;
//    gAppPhoneContext.fct.dialNumberType.pbIndex = pbIndex;
//    gAppPhoneContext.fct.dialNumberType.type    = numberType;
//
//    // set alarm
//    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
//
//    return APP_COMMON_RSP_OK;
//}



/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcAcceptCall                                       */
/**
    \brief      build accept call AT request

    \brief      Description:\n
                build accept call AT request \n

    \param      cb
                callback which is used to notify calling application that
                accepting the call has been successfully finished or that an
                error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcAcceptCall(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_ACCEPT_CALL;
    gAppPhoneContext.fct.common.callback    = cb;
    gAppPhoneContext.fct.acceptCall.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}   /* END OF _APP_PHONE_rcAcceptCall() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcHangUp                                           */
/**
    \brief      send hangup AT request

    \brief      Description:\n
                send hangup AT request \n

    \param      cb
                callback which is used to notify calling application that
                hangup the call has been successfully finished or that an
                error occurred

    \param      callIndex
                ATCallHangupEnum
                index of the call which shall be hung up

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...

*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcHangUp( void (*cb)(APP_COMMON_enResponseCode),
                                              ATCallHangupEnum callIndex)
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_HANGUP_CALL;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.hangUpCall.state     = APP_PHONE_COMMAND_NOT_SENT;
    gAppPhoneContext.fct.hangUpCall.callIndex = callIndex;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;

}   /* END OF _APP_PHONE_AT_REQUEST_sfHangUp() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcHangUpAll                                           */
/**
    \brief      send hangup AT request for all calls

    \brief      Description:\n
                send hangup AT request for all calls \n

    \param      cb
                callback which is used to notify calling application that
                hangup the call has been successfully finished or that an
                error occurred and start hangup for second call

    \param

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...

*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcHangUpAll( void (*cb)(APP_COMMON_enResponseCode) )
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
	gAppPhoneContext.fct.common.fctId       = APP_PHONE_HANGUP_ALL_CALL;
	gAppPhoneContext.fct.common.callback    = cb;
	gAppPhoneContext.fct.hangUpCall.state     = APP_PHONE_COMMAND_NOT_SENT;
	gAppPhoneContext.fct.hangUpCall.callIndex = AT_HANGUP_NO_INDEX;

	// set alarm
	(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;

}   /* END OF _APP_PHONE_AT_REQUEST_sfHangUpAll() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcPhoneStatus                                      */
/**
    \brief      build phone status AT request

    \brief      Description:\n
                build phone status AT request \n

    \param      cb
                callback which is used to notify calling application that
                requesting the current phone status has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcPhoneStatus(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_PHONE_STATUS;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.phoneStatus.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}   /* END OF APP_PHONE_rcPhoneStatus() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcCallState                                 */
/**
    \brief      build phone call state AT request

    \brief      Description:\n
                build phone call state AT request \n

    \param      cb
                callback which is used to notify calling application that
                requesting the current call status has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcCallState(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_CALL_STATE;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.callState.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}   /* END OF APP_PHONE_rcCallState() */


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcCallInfo                                  */
/**
    \brief      build phone call info AT request

    \brief      Description:\n
                build phone call info AT request \n

    \param      cb
                callback which is used to notify calling application that
                requesting the current call info has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcCallInfo(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_CALL_INFO;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.callInfo.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcSendDTMF                                         */
/**
    \brief      build sendDTMF AT request

    \brief      Description:\n
                build sendDTMF AT request \n

    \param      cb
                callback which is used to notify calling application that
                sending dtmf characters has been successfully
                finished or that an error occurred

    \param      dtmfChars
                const Uint8Type *
                characters which shall be sent as dtmf tones

    \param      length
                Uint16Type
                number of characters to be sent

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcSendDTMF(void (*cb)(APP_COMMON_enResponseCode),
                                               const Uint8Type *dtmfChars,
                                               Uint16Type   length )
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if(0U == APP_PHONE_AT_PNCC_u16GetNumberOfActiveCalls())
    {
        return APP_COMMON_RSP_ERROR_NO_ACTIVE_CALL;
    }

    if((length == 0U) || (dtmfChars == NULL))
    {
        return APP_COMMON_RSP_ERROR;
    }

    if((Uint32Type)length > (Uint32Type)sizeof(DtmfCharsType))
    {
        return APP_COMMON_RSP_ERROR;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_SEND_DTMF;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.dtmf.state   = APP_PHONE_COMMAND_NOT_SENT;
    gAppPhoneContext.fct.dtmf.sendIdx = 0U;
    gAppPhoneContext.fct.dtmf.length  = length;

    (void)memcpy(gAppPhoneContext.fct.dtmf.value, dtmfChars, length);

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;

}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcPlpdRequest       */
/**
    \brief      send PLPD Request to Parrot

    \brief      Description:\n
                send PLPD Request to Parrot

    \param      cb
                callback which is used to notify calling application that
                requesting a list of paired devices has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcPlpdRequest(void (*cb)(APP_COMMON_enResponseCode))
{

    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_PLPD_REQUEST;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.plpdReq.state   = APP_PHONE_COMMAND_NOT_SENT;


    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcCallHold                                         */
/**
    \brief      build call hold AT request

    \brief      Description:\n
                build call hold AT request \n

    \param      cb
                callback which is used to notify calling application that
                request to hold a call has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcCallHold(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_CALL_HOLD;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.callHold.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcCallReplace                                         */
/**
    \brief      build call replace AT request

    \brief      Description:\n
                build call replace AT request \n

    \param      cb
                callback which is used to notify calling application that
                request to hold a call has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcCallReplace(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_CALL_REPLACE;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.callReplace.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcResumeCall                                 */
/**
    \brief      build resume call AT request

    \brief      Description:\n
                build resume call AT request \n

    \param      cb
                callback which is used to notify calling application that
                request to resume a call has been successfully
                finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcResumeCall(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_RESUME_CALL;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.resumeCall.state   = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcHeadsetOnOff                              */
/**
    \brief      set current audio gateway mode

    \brief      Description:\n
                set current audio gateway mode ( tone on headset or speaker ) \n

    \param      cb
                callback which is used to notify calling application that
                request to transfer audio from or to a headset has been
                successfully finished or that an error occurred

    \param      mode
                ATAudioGatewayTransferModeEnum
                contains if audio shall be transfered to the headset or back to
                the phone / car kit

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcHeadsetOnOff(
                    void (*cb)(APP_COMMON_enResponseCode),
                    ATAudioGatewayModeEnum mode)
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_HEADSET_ON_OFF;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.headsetOnOff.mode  = mode;
    gAppPhoneContext.fct.headsetOnOff.state = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcHandsFreeOnOff                                   */
/**
    \brief      builds Hands Free On / Off AT request

    \brief      Description:\n
                builds Hands Free On / Off AT request \n

    \param      cb
                callback which is used to notify calling application that
                request to activate handsfree or private mode has been
                successfully finished or that an error occurred

    \param      mode
                ATAudioTransferModeEnum
                contains whether handsfree mode or private mode shall be activated

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcHandsFreeOnOff(void (*cb)(APP_COMMON_enResponseCode),
                                                     ATAudioTransferModeEnum mode)
{
    /* chaned on 08.04.2011, RT:10983 */
	ERM_vsetActiveCallFlagForSettingWithoutCI(BT_TRUE);

    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        if(mode == AT_AUDIO_DISCRETE_MODE)
            gAppPhoneContext.pending.resetHandsfreeToOn = BT_TRUE;
        else
            gAppPhoneContext.pending.resetHandsfreeToOff = BT_TRUE;
        return APP_COMMON_RSP_OK;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_HANDS_FREE_ON_OFF;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.handsFreeOnOff.mode  = mode;
    gAppPhoneContext.fct.handsFreeOnOff.state = APP_PHONE_COMMAND_NOT_SENT;

	
	// set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcMicroMuteOnOff                        */
/**
    \brief      build micro mute on/off AT request

    \brief      Description:\n
                build micro mute on/off AT request \n

    \param      cb
                callback which is used to notify calling application that
                request to activate the microphone or to mute it has been
                successfully finished or that an error occurred

    \param      mode
                ATMicModeEnum
                contains whether micro shall be muted or unmuted

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcMicroMuteOnOff(void (*cb)(APP_COMMON_enResponseCode),
                                                     ATMicModeEnum mode )
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_MICRO_MUTE_ON_OFF;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.microMuteOnOff.mode = mode;
    gAppPhoneContext.fct.microMuteOnOff.state = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcSwitchCalls                           */
/**
    \brief      build switch calls AT request

    \brief      Description:\n
                build switch calls AT request \n

    \param      cb
                callback which is used to notify calling application that
                request to switch an active and hold call has been
                successfully finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcSwitchCalls(void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_SWITCH_CALLS;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.switchCalls.state = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcJoinToConference                                 */
/**
    \brief      build AT request to join a held call to conference

    \brief      Description:\n
                build AT request to join a held call to conference \n

    \param      cb
                callback which is used to notify calling application that
                request to join a call to a conference has been
                successfully finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcJoinToConference( void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_JOIN_CONFERENCE;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.joinConference.state = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcPutCallsThrough                                  */
/**
    \brief      build AT request to join a held call to conference

    \brief      Description:\n
                build AT request to join a held call to conference \n

    \param      cb
                callback which is used to notify calling application that
                request to put through two calls has been
                successfully finished or that an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcPutCallsThrough( void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_PUT_CALLS_THROUGH;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.putCallsThrough.state = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcSplitCallFromConference                          */
/**
    \brief      builds AT request to split a given call Index from a conference

    \brief      Description:\n
                builds AT request to split a given call Index from a conference \n

    \param      cb
                callback which is used to notify calling application that
                request to split a call from an existing conference has been
                successfully finished or that an error occurred

    \param      callIndex
                ATCallIndexEnum
                contains the index of the call which shall be splitted from
                the conference

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcSplitCallFromConference( void (*cb)(APP_COMMON_enResponseCode),
                                                               ATCallIndexEnum callIndex)
{
    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPhoneContext.fct.common.fctId       = APP_PHONE_SPLIT_CONFERENCE;
    gAppPhoneContext.fct.common.callback    = cb;

    gAppPhoneContext.fct.splitConference.callIndex  = callIndex;
    gAppPhoneContext.fct.splitConference.state      = APP_PHONE_COMMAND_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_rcReleaseActiveCallAcceptWaitingCall               */
/**
    \brief      builds AT request to release the active call and accept the
                waiting call instead

    \brief      Description:\n
                builds AT request to release the active call and accept the
                waiting call instead \n

    \param      cb
                callback which is used to notify calling application that
                request to release the active call and accept the
                waiting call insteadhas been successfully finished or that
                an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode APP_PHONE_rcReleaseActiveCallAcceptWaitingCall(void (*cb)(APP_COMMON_enResponseCode))
//{
//    ATCallHangupEnum activeCall = AT_HANGUP_NO_INDEX;
//
//    if(gAppPhoneContext.fct.common.fctId != APP_PHONE_INVALID_FCT )
//    {
//        return APP_COMMON_RSP_ERROR_BUSY;
//    }
//
//    switch(_APP_PHONE_AT_PNCC_GetCurrentActiveCallIndex())
//    {
//        case APP_PHONE_CALL_INDEX_0:
//            activeCall = AT_HANGUP_CALL_0;
//            break;
//        case APP_PHONE_CALL_INDEX_1:
//            activeCall = AT_HANGUP_CALL_1;
//            break;
//        case APP_PHONE_CALL_INDEX_2:
//            activeCall = AT_HANGUP_CALL_2;
//            break;
//        default:
//            /* not ok */
//            return APP_COMMON_RSP_ERROR;
//    }
//
//    // set callBack for OK / ERROR response
//    gAppPhoneContext.fct.common.fctId       = APP_PHONE_RELEASE_ACTIVE_ACCEPT_WAITING_CALL;
//    gAppPhoneContext.fct.common.callback    = cb;
//
//    gAppPhoneContext.fct.relActAccWaitCall.state = APP_PHONE_ACCEPT_NOT_SENT;
//    gAppPhoneContext.fct.relActAccWaitCall.call  = activeCall;
//
//    // set alarm
//    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_FCT_PENDING,MSEC(0),0);
//
//    return APP_COMMON_RSP_OK;
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcToggleMicMute                                  */
/**
    \brief      checks the current micro setting and tries to toggle it

    \brief      checks the current micro setting and tries to toggle it

    \param      cb
                callback which is used to notify calling application that
                request to release the active call and accept the
                waiting call insteadhas been successfully finished or that
                an error occurred

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONE_rcToggleMicMute(void (*cb)(APP_COMMON_enResponseCode))
{
    ATMicModeEnum mode =  gAppPhoneContext.isMicMuted ? AT_MIC_UNMUTE : AT_MIC_MUTE;

    return APP_PHONE_rcMicroMuteOnOff(cb, mode);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bIsValidPhoneNumber                                  */
/**
    \brief      checks if the given number is valid in length and contained
                characters

    \brief      checks if the given number is valid in length and contained
                characters

    \param      pNumber
                const Uint8Type *
                contains the number to be checked

    \param      numberLen
                Uint16Type
                number of bytes valid in pNumber

    \return     BooleanType
                BT_TRUE means number is valid, else BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bIsValidPhoneNumber( const Uint8Type *pNumber, Uint16Type numberLen)
{
    /* check len */
    Uint16Type i            = 0U;
    //const Uint8Type * pTmp  = pNumber;

    if( numberLen == 0U || numberLen == 1U || numberLen > AT_MAX_PHONE_NUM_LEN )
    {
        return BT_FALSE;
    }
    for( i=0U; i<numberLen; i++)
    {
        switch(pNumber[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '(':
            case ')':
            case '*':
            case '+':
            case '#':
            case ' ':       //RT #3544
            case '-':       //RT #3544, separator used in USA for GSTN (Global Switched Telephone Network) address
            case '/':       //RT #3544
            case ',':       //RT #3544
            case 'p':       //RT #4630 see RFC3601 for pause (used e.g. with calling cards)
            case 'w':       //RT #4630 see RFC3601 for tonewait (wait for dial tone)
            case '.':       //RT #4630 see RFC3601, used as separator in e.g. Germany for GSTN address
                break;
            default:
                DEBUG_VALUE1(APP_PHONE_SWI_ERR, MOD_APP_PHONE, "ValidPhoneNumber: invalid char %c",
                            pNumber[i]);
                return BT_FALSE;
        }
    }

    return BT_TRUE;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_GetNetworkProvider                                  */
/**
    \brief      returns current Network provider, if any

    \brief      returns current Network provider, if any

    \return     const AT_NetworkProvider *
                pointer to network provider string
*/
/*----------------------------------------------------------------------------*/
const AT_NetworkProvider *APP_PHONE_GetNetworkProvider(void)
{
    return &gAppPhoneContext.piev_networkProvider;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_GetNetworkProvider                                  */
/**
    \brief      returns current signal strength

    \brief      returns current signal strength

    \return     AT_IndicatorValue
                any value, 0xFF is invalid
*/
/*----------------------------------------------------------------------------*/
AT_IndicatorValue APP_PHONE_GetSignalStrength()
{
    return gAppPhoneContext.piev_signalStrength;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_GetBatteryLevel                                  */
/**
    \brief      returns current battery level

    \brief      returns current battery level

    \return     AT_IndicatorValue
                0 to 5 are valid
*/
/*----------------------------------------------------------------------------*/
AT_IndicatorValue APP_PHONE_GetBatteryLevel()
{
    return gAppPhoneContext.piev_batteryLevel;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_GetRoaming                                  */
/**
    \brief      returns whether phone is currently roaming or not

    \brief      returns whether phone is currently roaming or not

    \return     AT_IndicatorValue
                0 = not roaming, 1 = roaming, others invalid
*/
/*----------------------------------------------------------------------------*/
AT_IndicatorValue APP_PHONE_GetRoaming()
{
    return gAppPhoneContext.piev_Roaming;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_GetNetworkAvailable                                  */
/**
    \brief      returns whether phone is currently registered or not

    \brief      returns whether phone is currently registered or not

    \return     AT_IndicatorValue
                0 = not registered, 1 = registered, others invalid
*/
/*----------------------------------------------------------------------------*/
AT_IndicatorValue APP_PHONE_GetNetworkAvailable()
{
    return gAppPhoneContext.piev_serviceSupport;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bNetworkAvailable                                  */
/**
    \brief      returns whether phone is currently registered or not

    \brief      returns whether phone is currently registered or not

    \return     BooleanType
                BT_FALSE = not registered, BT_TRUE = registered
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bNetworkAvailable()
{
    if( (Uint8Type)AT_FALSE == gAppPhoneContext.piev_serviceSupport )
    {
        return BT_FALSE;
    }
    return BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bRedialNumberAvailable                                  */
/**
    \brief      returns whether redial number is available or not

    \brief      returns whether redial number is available or not

    \return     BooleanType
                BT_FALSE = not available, BT_TRUE = available
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bRedialNumberAvailable()
{
    return BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bIsMicMuted                                  */
/**
    \brief      returns whether micro is muted or not

    \brief      returns whether micro is muted or not

    \return     BooleanType
                BT_FALSE = unmuted, BT_TRUE = muted
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bIsMicMuted()
{
    return gAppPhoneContext.isMicMuted;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bIsHeadsetOn                                  */
/**
    \brief      returns whether audio is on headset or not

    \brief      returns whether audio is on headset or not

    \return     BooleanType
                BT_FALSE = not on headset, BT_TRUE = audio on headset
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bIsHeadsetOn()
{
    return gAppPhoneContext.isHeadsetOn;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bIsHandsFreeOff                                  */
/**
    \brief      returns whether handsfree or private mode is active

    \brief      returns whether handsfree or private mode is active

    \return     BooleanType
                BT_FALSE = handsfree mode, BT_TRUE = private mode
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bIsHandsFreeOff()
{
    return gAppPhoneContext.isHandsFreeOff;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_enGetHfpVersion                                  */
/**
    \brief      returns supported hfp version

    \brief      returns supported hfp version of currently connected phone

    \return     ATCoDProfileEnum
                handsfree protocol version supported by connected phone
*/
/*----------------------------------------------------------------------------*/
ATCoDProfileEnum  APP_PHONE_enGetHfpVersion(void)
{
    return gAppPhoneContext.hfpVersion;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_bIsCDMAPhone                                  */
/**
    \brief      returns true if connected phone is a CDMA phone

    \brief      returns true if connected phone is a CDMA phone

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_bIsCDMAPhone(void)
{
    return gAppPhoneContext.isCDMAPhone;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vOnAppCshdlNotification                                  */
/**
    \brief      handles notifications from APP_CSHDL

    \brief      handles notifications from APP_CSHDL

    \param      notification
                APP_CSHDL_NotificationType
                event to be handled
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    switch(notification)
    {
        case APP_CSHDL_NOTIFY_KL_15_OFF:
            /* reset missed calls during KL15 off */
            // BEGIN Susanne: 06.08.2009: fixed this, now counting all during uhv active
            //gAppPhoneContext.u16MissedCallsDuringKl15Off = 0U;
            // END Susanne: 06.08.2009: fixed this, now counting all during uhv active
            break;
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:
            gAppPhoneContext.u16MissedCalls = 0U;
            gAppPhoneContext.u16MissedNumbers = 0U;
            break;
        case APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN:
            /* nothing else special to be done here, just say we've handled shutdown */
            APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_APP_PHONE);
            break;
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON:
            // BEGIN Susanne: 06.08.2009: fixed this, now counting all during uhv active
            //if( 0U < APP_PHONE_u16NewMissedCallsAvailable() &&
            //    /* we need this second check to avoid showing S904 although
            //     * missed pb size is zero. This sometimes happens for some phones if CK5050 does
            //     * not synchronized call stacks correctly. */
            //    0U < APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enMissedCalls))
            //{
            //    _APP_PHONE_vNotifyApp(APP_PHONE_KL15OFF_MISSED_CALLS);
            //}
            // END Susanne: 06.08.2009: fixed this, now counting all during uhv active
            break;
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF:
             /* Instrument Cluster is OFF */
        default:
            /* ignore */
            DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE,"Ignoring Cshdl NotId %d", notification);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vOnAppDeviceNotification                                  */
/**
    \brief      handles notifications from APP_DEVICE

    \brief      handles notifications from APP_DEVICE

    \param      new_notification
                APP_DEVICE_enNotificationId
                event to be handled
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_vOnAppDeviceNotification(APP_DEVICE_enNotificationId new_notification)
{
    switch(new_notification)
    {
        case APP_DEVICE_NOTIFY_USER_PROFILE_ADDED:
            /* need to update HFP version */
            (void)_APP_PHONE_sfRequestHfpVersion();
            break;
    }
}

BooleanType APP_PHONE_bIsUnknownCallerId(const Uint8Type * pCallerId, Uint16Type callerIdLen)
{
    static const Uint8Type UnknownCallerIDPattern1[5]={'D',',',' ','C',','};
    static const Uint8Type UnknownCallerIDPattern2[2]={'-','1'};
    static const Uint8Type UnknownCallerIDPattern3[2]={'-','2'};		//RT #14985

    if((  NULL  == pCallerId )                              ||
       (  0     == callerIdLen ) /* empty */                ||     /* empty */
       ( (1     == callerIdLen) && ('+' == pCallerId[0]) )  ||
       ( (1     == callerIdLen) && (';' == pCallerId[0]) )  ||
       ( (5     == callerIdLen) && (0 == memcmp(pCallerId, UnknownCallerIDPattern1,5)))||
       ( (2     == callerIdLen) && (0 == memcmp(pCallerId, UnknownCallerIDPattern2,2)))||
       ( (2     == callerIdLen) && (0 == memcmp(pCallerId, UnknownCallerIDPattern3,2))))
    {
        /* we have an unknown or restricted number */
        return BT_TRUE;
    }
    return BT_FALSE;
}


ALARMCALLBACK(APP_PHONE_cbOutgoingCallIndicationTimeout)
{
    gAppPhoneContext.pending.indicateOutgCallError  = BT_TRUE;
    gAppPhoneContext.bWaitForOutgCallIndication     = BT_FALSE;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING, MSEC(1), 0);
    //SetEvent(evAppTaskPhoneFctPending);
}

ALARMCALLBACK(APP_PHONE_cbOnPatrTimeout)
{
    gAppPhoneContext.pending.patrTimeout = BT_TRUE;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING, MSEC(0), 0);
}


void _APP_PHONE_MuteAck_Cb(APP_COMMON_enResponseCode result)
{
    DEBUG_VALUE2(APP_PHONE_SWI_TRC,MOD_APP_PHONE,"_APP_PHONE_MuteAck_Cb: recvd Mute result: %d, fctId: %d",
                 result,gAppPhoneContext.fct.common.fctId);

    /* depending on active fctType, set next state and go on */
    switch(gAppPhoneContext.fct.common.fctId)
    {
        case APP_PHONE_DIAL_NUMBER:
            gAppPhoneContext.fct.dial.state = APP_PHONE_DIAL_COMMAND_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING, MSEC(0), 0);
            break;
        case APP_PHONE_DIAL_INDEX:
            gAppPhoneContext.fct.dialIndex.state = APP_PHONE_DIAL_COMMAND_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING, MSEC(0), 0);
            break;
        case APP_PHONE_REDIAL:
            gAppPhoneContext.fct.redial.state = APP_PHONE_DIAL_COMMAND_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_FCT_PENDING, MSEC(0), 0);
            break;
        default:
            DEBUG_TEXT(APP_PHONE_SWI_TRC,MOD_APP_PHONE,"_APP_PHONE_MuteAck_Cb: ignored");
            break;
    }
}

/* End Of File APP_PHONE_C1.c */


