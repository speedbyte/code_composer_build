/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_BAP_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_PHONE_BAP
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP 
*-----------------------------------------------------------------------------
* $Date: 2011-07-04 15:34:00 +0200 (Mo, 04 Jul 2011) $
* $Rev: 20591 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/src/APP_PHONE_BAP_C1.c $
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
#include "bap_defines.h"    // vw

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "BAP_Dispatcher.h"
#include "ERM.h"
#include "UART.h"
#include "APP_PHONE_AT_Request_Builder.h"
#include "APP_PHONE.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_CSHDL.h"
#include "ERM_Utils.h"
#include "ERM_DeviceHandler.h"
#include "APP_PHONE_BAP.h"
#include "APP_PHONE_BAP_CI.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONE_BAP_REQUEST.h"
#include "APP_PHONE_BAP_INDICATION.h"
#include "OSEK_AppTask_Alarm.h"
#include "APP_PHONE_BAP_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current call management function currently in progress
*/
AppPhoneBapContext gAppPhoneBapContext;
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

static void _APP_PHONE_BAP_vHandlePhoneStateChanged(void);
void _APP_PHONE_BAP_vDisconnectActiveUserOnCK5050ShutDown(void);

//callback functions
void _APP_PHONE_BAP_vDelayUserActivation_Cb(void);
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
/* Function    : APP_PHONE_sfInitApp                                         */
/**
    \brief      Callback function for initialization of AppTelephone

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONE_BAP_sfInitApp()
{
    (void)memset(&gAppPhoneBapContext, 0, sizeof(gAppPhoneBapContext));
    return SUCCESS;
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

void APP_PHONE_BAP_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************
    if( event & evAppTaskPhoneBapFctPending )
    {
        (void)ClearEvent( evAppTaskPhoneBapFctPending );
        if( gAppPhoneBapContext.common.fctId != PHONE_BAP_INVALID_FCT_ID )
        {
            _APP_PHONE_BAP_vHandleBAPIndication(&gAppPhoneBapContext.common.bapInd);
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Handle pend BAPMsg");
        }
        if( BT_TRUE == gAppPhoneBapContext.bUpdateActiveUserPending )
        {
            _APP_PHONE_BAP_vHandlePhoneStateChanged();
            gAppPhoneBapContext.bUpdateActiveUserPending = BT_FALSE;
        }
    }

    if( event & evAppTaskPhoneBapRequestPending )
    {
        (void)ClearEvent( evAppTaskPhoneBapRequestPending );

        if( gAppPhoneBapContext.notification == APP_PHONE_CALL_STATE_CHANGED )
        {
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Retry Phone notification after 10 ms.");
            APP_PHONE_BAP_vOnAppPhoneNotification(gAppPhoneBapContext.notification);
        }

    }

    // *****************************************************
    // look for events from BAPDispatcher and process them
    // check for new indication messages from BAP Dispatcher
    // *****************************************************
	if(event & evAppTaskPhoneBapInd )
	{
        (void)ClearEvent(evAppTaskPhoneBapInd);

        if( gAppPhoneBapContext.common.fctId == PHONE_BAP_INVALID_FCT_ID )
        {
            BapIndication bapInd;
            (void)ReceiveMessage( msgPhoneBapIndication, &bapInd);

            _APP_PHONE_BAP_vHandleBAPIndication(&bapInd);

            DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Recvd BAPMsg fctId %d",
                         bapInd.fct_id);
        }

        // check queue Status
        if( E_COM_NOMSG != GetMessageStatus(msgPhoneBapIndication))
        {
            // there are still some messages
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_BAP_IND,
                                               MSEC(10),
                                               0);
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_ERM_vHandleErmStateChange                      */
/**
    \brief      handle State change of ERM to Update e.g. FSG OperationState

    \param      newState
                new internal state of ERM module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_ERM_vHandleErmStateChange(ERMStateType newState)
{
    switch( newState )
    {
        case ERM_STATE_START_UP:
            (void)_APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(BAP_OPSTATE_MOBILE_NOT_ATTACHED);
            break;
        //case ERM_STATE_CK5050_ACTIVE:
        //{
        //    
        //
        //    _APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(BAP_OPSTATE_MOBILE_ON);
        //    break;
        //}
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_vOnErmNotification                             */
/**
    \brief      handle notifications from ERM

    \param      pNotification
                new ERM notification for the module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vOnErmNotification(const ERMNotificationType * pNotification)
{
    DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "OnErmNot: notId %d",pNotification->common.id);

    switch(pNotification->common.id)
    {
        case ERM_NOTIFY_ACTIVE_DEVICE_NAME_CHANGED:
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Active User Name Changed!");
            (void)_APP_PHONE_BAP_REQUEST_sfSendActiveUser();
            break;
        //case ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED:
        case ERM_NOTIFY_PBSN_PHONE:
            _APP_PHONE_BAP_vHandlePhoneStateChanged();
            break;
        case ERM_NOTIFY_STARTUP_DONE:
            /* send current user list status */
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: PhoneModOn");
            (void)_APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(BAP_OPSTATE_MOBILE_NOT_ATTACHED);
            // #4733 Send Changed Array instead of Status Array
            //(void)_APP_PHONE_BAP_REQUEST_sfSendUserListStatus(NULL);
            (void)_APP_PHONE_BAP_REQUEST_sfSendUserListChanged();
            break;

        case ERM_NOTIFY_WARN_DIAL_ERROR:
            if(gAppPhoneBapContext.bDialNumberPending == BT_TRUE)
            {
                //#3838 Send DialNumberError in case PWNG:11 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: warning dial error");
                APP_PHONE_BAP_vProcessDialNumber(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Dial number result already sent");
            }
            break;

        case ERM_NOTIFY_WARN_HANGUP_ERROR:
            if(gAppPhoneBapContext.bHangUpCallPending == BT_TRUE)
            {
                // Send HangUpError in case PWNG:12 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: warning hang up error");
                APP_PHONE_BAP_vProcessHangUpCall(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "HangUp call result already sent");
            }
            break;

        case ERM_NOTIFY_WARN_PICKUP_ERROR:
            if(gAppPhoneBapContext.bAcceptCallPending == BT_TRUE)
            {
                // Send PickUpError in case PWNG:13 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: warning accept call error");
                APP_PHONE_BAP_vProcessAcceptCall(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Accept call result already sent");
            }

            if(gAppPhoneBapContext.bResumeCallPending == BT_TRUE)
            {
                //Send PickUpError in case PWNG:13 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: resume call error");
                APP_PHONE_BAP_vProcessResumeCall(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Resume call result already sent");
            }
            break;

        case ERM_NOTIFY_WARN_2ND_CALL_ERROR:
            if(gAppPhoneBapContext.bMPSwapPending == BT_TRUE)
            {
                // Send second call error in case PWNG:14 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: swap calls error");
                APP_PHONE_BAP_vProcessMPSwap(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Swap calls result already sent");
            }

            if(gAppPhoneBapContext.bMPCallHoldAcceptWaitingCallPending == BT_TRUE)
            {
                // Send second call error in case PWNG:14 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: call hold accept waiting call error");
                APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "call hold accept waiting call result already sent");
            }

            if(gAppPhoneBapContext.bMPReleaseActiveCallAcceptWaitingCallPending == BT_TRUE)
            {
                // Send second call error in case PWNG:14 notification by CK5050
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP,"ErmNot: swap calls error");
                APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_RESULT_ERROR);
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Swap calls result already sent");
            }
            break;

        default:
            /* ignore notification */
            DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Ignoring ERMNot %d!", 
                               pNotification->common.id);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_vOnAppCshdlNotification                             */
/**
    \brief      handle notifications from central state handler(CSHDL)

    \param      notification
                new CSHDL notification for the module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "OnAppCshdlNot: notId %d",notification);

    switch(notification)
    {
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:
        {
            PhoneMobileServiceSupport phoneMss;

            /* set FSG Operation state to mobile off */
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "OnAppCshdl: CK5050 Off -> MMS upd");
            /* reset mob Service support */
            (void)memset(&phoneMss, 0x00, sizeof(phoneMss));
            (void)_APP_PHONE_BAP_REQUEST_sfSendMobServSupp(&phoneMss);
            break;
        }
        case APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN:

            /* first deactivate the user if still active */
            if( AT_INVALID_DEVICE_ID != ERM_u8GetActivePhone())
            {
                _APP_PHONE_BAP_vDisconnectActiveUserOnCK5050ShutDown();
            }

            APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_APP_PHONE_BAP);
            break;
//        case APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON:
//            _APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();
//            break;
//        case APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_OFF:
//            _APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();
//            break;
        case APP_CSHDL_NOTIFY_KL_15_OFF:
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON:
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF:
        default:
            /* ignore */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_vOnAppDeviceNotification                       */
/**
    \brief      handle notifications from APP_Device

    \param      pNotification
                new APP_Device notification for the module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_DEVICE_NOTIFY_USER_PROFILE_NAME_SAVED:
        {
            // TODO:
            // SUSANNE: 20.11.2009 #3888 
            // We need to send User list first!!! 
            // similar to APP_DEVICE_NOTIFY_USER_PROFILE_ADDED, but
            // only update ActiveUser in HandleEvents, and do not call HandlePhoneStateChanged()
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Active User Profile Name Changed!");
            (void)_APP_PHONE_BAP_REQUEST_sfSendActiveUser();
            break;
        }
        case APP_DEVICE_NOTIFY_USER_PROFILE_DELETED:
        {
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "BAP: User profile deleted!");
            (void)_APP_PHONE_BAP_REQUEST_sfSendUserListChanged();
            break;
        }
        case APP_DEVICE_NOTIFY_USER_PROFILE_ADDED:
        {
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "BAP: User profile added!");
            (void)_APP_PHONE_BAP_REQUEST_sfSendUserListChanged();
            /* check if we have an active user, if so set an alarm to send the
             * new active user not directly with new UserList */
            if( NULL != ERM_pGetActiveUserProfile())
            {
                DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "BAP: Set Alarm to upd. ActiveUser!");
                gAppPhoneBapContext.bUpdateActiveUserPending = BT_TRUE;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONE_BAP_FCT_PENDING, MSEC(500), MSEC(0));
            }
            break;
        }
        default:
            /* ignore notification */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vHandlePhoneStateChanged                      */
/**
    \brief      handle the changes of the phone state(e.g. connecting or disconnecting)
                and updates the info over BAP to the HU.

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_PHONE_BAP_vHandlePhoneStateChanged(void)
{

    (void)_APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();

    if(( AT_INVALID_DEVICE_ID != ERM_u8GetActivePhone()) && (BT_FALSE == ERM_bGetDisconnectWillFollow()))
    {   /* phone service connected */

        // new Active User, send new UserList first
        //_APP_PHONE_BAP_REQUEST_sfSendUserListChanged();
        const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
        if( NULL == profile )
        {   /* there is no profile for the active phone, so ignore this event
               and wait until user profile has been created */
            DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "New Active User but no existing Profile!");
            return;
        }

        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "New Active User with existing Profile!");

        gAppPhoneBapContext.bUserIsBeingActivated = BT_TRUE;

        /* there is a profile so send new Active User to HU */
        (void)_APP_PHONE_BAP_REQUEST_sfSendActiveUser();

        /* start the timer for the processing*/
        AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_DELAY_USER_ACTIVATION,
                          MSEC(200),
                          _APP_PHONE_BAP_vDelayUserActivation_Cb);
    }
    else
    {   /* Disconnected */
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Active User disconnected!");

        // only set MobileConnectionType as all other values are static for UHV High
        (void)_APP_PHONE_BAP_REQUEST_sfSendFsgSetup(BAP_FSGSTP_MCT_NO_CONNECTION);

        // set FSG OperationState Mobile Not Attached
        (void)_APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(BAP_OPSTATE_MOBILE_NOT_ATTACHED);

        // all CallStates to Idle
        (void)_APP_PHONE_BAP_REQUEST_sfSendAllCallStatesIdle();

        // reset handsfree mode and micro mute mode to default
        (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_ON);
        (void)_APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(BAP_MICRO_UNMUTE);

        // send empty string for active user
        (void)_APP_PHONE_BAP_REQUEST_sfSendActiveUser();

        // send register state, not register and not searching
        (void)_APP_PHONE_BAP_REQUEST_sfSendRegisterState(0,0);

        // change heartBeats
        (void)_APP_PHONE_BAP_REQUEST_sfSendNetworkProvider((const Uint8Type *)"", 0 );
        (void)_APP_PHONE_BAP_REQUEST_sfSendSignalQuality(APP_PHONE_PIEV_INVALID_VALUE);

        gAppPhoneBapContext.bSignalQualityAvailableAfterConnect = BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vSetBasicMobileServiceSupport                 */
/**
    \brief      builds the mobile service support message depending on the
                HFP version.

    \param      pPhoneMss
                structure containing all off the service support fields

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vSetBasicMobileServiceSupport(PhoneMobileServiceSupport *pPhoneMss)
{
    (void)memset(pPhoneMss, 0, sizeof(*pPhoneMss));

    // Call Hold and Conference are enabled depending on the HFP version of the connected phone!!!

    //pPhoneMss->GetAll           = 1U; // RT 10469
    pPhoneMss->BapConfig        = 1U;
    pPhoneMss->FunctionList     = 1U;
    pPhoneMss->Heartbeat        = 1U;
    pPhoneMss->FsgSetup         = 1U;
    pPhoneMss->FsgOpState       = 1U;
    pPhoneMss->MobileServiceSupport = 1U;
    pPhoneMss->UserList         = 1U;
    pPhoneMss->ActiveUser       = 1U;
    pPhoneMss->RegisterState    = 1U;
    pPhoneMss->LockState        = 1U;
    pPhoneMss->NetworkProvider  = 1U;
    if(APP_PHONE_PIEV_INVALID_VALUE == APP_PHONE_GetSignalStrength())
    {
        pPhoneMss->SignalQuality    = 0U;
    }
    else
    {
        pPhoneMss->SignalQuality    = 1U;
    }
    pPhoneMss->LineState        = 1U;
    pPhoneMss->CallState        = 1U;
    pPhoneMss->CallInfo         = 1U;
    pPhoneMss->DialNumber       = 1U;
    pPhoneMss->HangupCall       = 1U;
    pPhoneMss->AcceptCall       = 1U;
    //pPhoneMss->CallHold         = 1U; // only for HFP1.5
    //pPhoneMss->ResumeCall       = 1U; // only for HFP1.5
    //RS: Enable handsfree in the case of active call available
    if(APP_PHONE_AT_PNCC_u16GetNumberOfActiveCalls() > 0)
    {
        pPhoneMss->HandsFreeOnOff   = 1U;
    }
    pPhoneMss->MicroMuteOnOff   = 1U;
    pPhoneMss->SendDTMF         = 1U;
    //pPhoneMss->MPReleaseActiveCallAcceptWaitingCall = 1U; // only for HFP1.5
    //pPhoneMss->MPSwap           = 1U;                     // only for HFP1.5
    pPhoneMss->MPCallHoldAcceptWaitingCall = 1U;          // only for HFP1.5


    if(APP_CSHDL_eGetCradleRestrictionState() == APP_CSHDL_CRADLE_RESTRICTION_ON)
    {
        pPhoneMss->RestoreFactorySettings = 0U;
    }
    else
    {
    pPhoneMss->RestoreFactorySettings = 1U;
    }
    //pPhoneMss->CCJoin           = 1; // only for HFP1.5
    //pPhoneMss->CCSplit          = 1; // only for HFP1.5
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vDisconnectActiveUserOnCK5050ShutDown         */
/**
    \brief      force the user disconnecting when CK module is shut down and no
                HFP disconnecting event is received. In this case the user deactivation
                has to be triggered manually

    \param      pNotification
                new ERM notification for the module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vDisconnectActiveUserOnCK5050ShutDown(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "Active User disconnected!");

    // only set MobileConnectionType as all other values are static for UHV High
    (void)_APP_PHONE_BAP_REQUEST_sfSendFsgSetup(BAP_FSGSTP_MCT_NO_CONNECTION);

    // set FSG OperationState Mobile Not Attached
    (void)_APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(BAP_OPSTATE_MOBILE_NOT_ATTACHED);

    // all CallStates to Idle
    (void)_APP_PHONE_BAP_REQUEST_sfSendAllCallStatesIdle();

    // reset handsfree mode and micro mute mode to default
    (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_ON);
    (void)_APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(BAP_MICRO_UNMUTE);

    // send empty string for active user
    (void)_APP_PHONE_BAP_REQUEST_sfDeactiveUser();

    // send register state, not register and not searching
    (void)_APP_PHONE_BAP_REQUEST_sfSendRegisterState(0,0);

    // change heartBeats
    (void)_APP_PHONE_BAP_REQUEST_sfSendNetworkProvider((const Uint8Type *)"", 0 );
    (void)_APP_PHONE_BAP_REQUEST_sfSendSignalQuality(APP_PHONE_PIEV_INVALID_VALUE);

    gAppPhoneBapContext.bSignalQualityAvailableAfterConnect = BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbDelayUserActivation                         */
/**
    \brief      this callback function is used to delay the BAP requests needed
                for the user activation in case to fulfill the specification(BAP_TEL_MSC)

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vDelayUserActivation_Cb(void)
{
    if(APP_PHONE_BAP_bIsUserActivated() == BT_TRUE)
    {
        // set FSG Operation State Mobile ON
        (void)_APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(BAP_OPSTATE_MOBILE_ON);

        // only set MobileConnectionType as all other values are static for UHV High
        (void)_APP_PHONE_BAP_REQUEST_sfSendFsgSetup(BAP_FSGSTP_MCT_HANDSFREE_PROFILE);

        gAppPhoneBapContext.bUserIsBeingActivated = BT_FALSE;

        /* start the timer for the processing */
        AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_DELAY_USER_ACTIVATION,
                          MSEC(200),
                          _APP_PHONE_BAP_vDelayUserActivation_Cb);
    }
    else
    {
        _APP_PHONE_BAP_vInternalNotification(APP_PHONE_SERV_AVAIL_CHANGED);

        /* update the signal quality */
        APP_PHONE_BAP_vOnAppPhoneNotification(APP_PHONE_OPERATOR_NAME_CHANGED);

        /* update the network provider */
        APP_PHONE_BAP_vOnAppPhoneNotification(APP_PHONE_NW_SIGSTRENGHT_CHANGED);

        /* send current call State if not idle */
        if( BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
        {
            (void)APP_PHONE_BAP_vOnAppPhoneNotification(APP_PHONE_CALL_STATE_CHANGED);
            (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_ON);
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_BAP_bIsUserActivated                               */
/**
    \brief      this function checks if the user is being activated on BAP_TEL
                (e.g. if UserActive, FSG-Setup and FSG-OperationState are being sending)

    \param      void

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_BAP_bIsUserActivated(void)
{
    return gAppPhoneBapContext.bUserIsBeingActivated;
}
/* End Of File APP_PHONE_C1.c */


