/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONEBOOK_AT_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Phonebook Application
*-----------------------------------------------------------------------------
* Description:    Handles all phonebook specific responses from
                  AT Cmd Parser
*-----------------------------------------------------------------------------
* $Date: 2011-08-30 16:53:44 +0200 (Di, 30 Aug 2011) $
* $Rev: 21443 $
* $Author: maennel $
* $URL:
* http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONEBOOK_AT_C1.c $
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
#include "APP_PHONEBOOK_BAP.h"
#include "APP_PHONEBOOK_BAP_CI.h"
#include "APP_PHONEBOOK_BAP_REQUEST.h"
#include "APP_PHONEBOOK_BAP_ERM.h"
#include "APP_SMS.h"
#include "APP_PHONEBOOK.h"
#include "ERM.h"
#include "APP_COMMON_Utils.h"
#include "POOL.h"
#include "APP_DIAG.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "APP_PHONEBOOK_BAP_AT_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

extern AppPhoneBookBapContext gAppPBBapContext;
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

static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleNumberOfUnreadSmsChanged(void);
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleSmsSupport(BooleanType bIsSmsSupported);
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleNewSms(void);
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleDeletedSms(void);
static SuccessFailType _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PHONEBOOK_BAP_enPbActType actType);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleUserPbChanged(void);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleUserPbNotChanged(void);
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleMissedCallListChanged(void);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleReceivedCallListChanged(void);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleDialedCallListChanged(void);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleMissedCallListNotChanged(void);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleReceivedCallListNotChanged(void);
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleDialedCallListNotChanged(void);

static void _APP_PHONEBOOK_BAP_vHandlePhoneStateChanged(void);

//APP_PHONEBOOK_enPbRefreshStateType _APP_PHONEBOOK_enSelectNextRefreshState();
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
void APP_PHONEBOOK_BAP_vOnAppSmsNotification(APP_SMS_enNotificationId notification)
{
    BooleanType bIsSmsAllowed = BT_FALSE;
    Uint8Type u8IsSmsAllowed;
    SuccessFailType result = SUCCESS;
    const ERMUserProfileType * userProfile = ERM_pGetActiveUserProfile();

    /* check if sms is deactivated via diagnosis, if so, do not handle any of
     * these events */
    (void)EEPROM_enGetSingleByteFromRamMirror(&u8IsSmsAllowed, EEPROM_LAYOUT_ENUM_SMSWriting);
	if ( u8IsSmsAllowed == 0x1 )
	{
		bIsSmsAllowed = BT_TRUE;
	}
	else if ( u8IsSmsAllowed == 0x0 )
	{
		bIsSmsAllowed = BT_FALSE;
	}
	else
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP,"APP_DIAG_GetSms_Writ failed");
        // make sure value
        bIsSmsAllowed = BT_FALSE;
    }
    if( BT_FALSE == bIsSmsAllowed )
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP,"OnAppSmsNotification: SmsNotAllowed, IgnorSmsNot.");
        return;
    }


    if( NULL != userProfile )
    {
        switch(notification)
        {
            case APP_SMS_SUPPORTED:
                result = _APP_PHONEBOOK_BAP_sfHandleSmsSupport(BT_TRUE);
                break;
            case APP_SMS_UNSUPPORTED:
                result = _APP_PHONEBOOK_BAP_sfHandleSmsSupport(BT_FALSE);
                break;
            case APP_SMS_DELETED_SMS:
                result = _APP_PHONEBOOK_BAP_sfHandleDeletedSms();
                break;
            case APP_SMS_NEW_SMS:
                result = _APP_PHONEBOOK_BAP_sfHandleNewSms();
                break;
            case APP_SMS_NUMBER_OF_NEW_SMS_CHANGED:
                result = _APP_PHONEBOOK_BAP_sfHandleNumberOfUnreadSmsChanged();
                break;
            default:
                DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP,"OnAppSmsNotification: ignore %d",
                           notification);
                break;
        }
    }
    (void)result;
}

void APP_PHONEBOOK_BAP_vOnAppPBNotification(APP_PB_enNotificationId new_notification)
{
    SuccessFailType result = SUCCESS;
    const ERMUserProfileType * userProfile = ERM_pGetActiveUserProfile();

    if( NULL == userProfile )
    {
        /* check PbState again when user profile has been added */
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP,"vAppPbNot: ign due to no prof.");
        return;
    }

    switch(new_notification)
    {
        // user pb
        case APP_PB_USERS_PB_CHANGED_INCOMPLETE:
            _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__CHANGED_INCOMPLETE);
            break;
        case APP_PB_USERS_PB_NOT_CHANGED_INCOMPLETE:
            _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__NOT_CHANGED_INCOMPLETE);
            break;
        case APP_PB_USERS_PB_NOT_AVAILABLE:
            _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__NOT_AVAILABLE);
            break;
        case APP_PB_USERS_PB_CHANGED:
            _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__CHANGED);
            break;
        case APP_PB_USERS_PB_NOT_CHANGED:
            _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__NOT_CHANGED);
            break;

        case APP_PB_SORTING_STARTED:
            // HU shall show pb loading until sorting has been finished
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOADING, 0xFFFF);
            break;
        // when sorting has been started but times out, we should be already in loading state
        case APP_PB_SORTING_TIMED_OUT:
            // as sorting was not really successful, we dont want to trigger a "new pb" update
            // so we will first sent PbState 0x00 for not available and than activate the pb
            _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__OLD_AFTER_SORTING_TIMOUT);
            break;


        /* missed pb */
        case APP_PB_MISSED_PB_CHANGED:
            // sends MissedCallIndication and ChangedArray
            result = _APP_PHONEBOOK_BAP_sfHandleMissedCallListChanged();
            break;
        case APP_PB_MISSED_PB_NOT_CHANGED:
            // always sent Full-Range ChangedArray
            // send this to be sure HU requests call stacks. This should not be
            // necessary, but due to sync errors occurred often in the past with CK5050
            // we still send this again here
            result = _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallListChangedArray();
            break;
        case APP_PB_MISSED_CALLS_RESET_BY_HMI:
            _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(0,0);
            break;

        /* received pb */
        case APP_PB_RECEIVED_PB_CHANGED:
            // always sent Full-Range ChangedArray
            result = _APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray();
            break;
        case APP_PB_RECEIVED_PB_NOT_CHANGED:
            // always sent Full-Range ChangedArray
            // send this to be sure HU requests call stacks. This should not be
            // necessary, but due to sync errors occurred often in the past with CK5050
            // we still send this again here
            result = _APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray();
            break;

        /* dialled pb */
        case APP_PB_DIALED_PB_CHANGED:
            result = _APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray();
            break;
        case APP_PB_DIALED_PB_NOT_CHANGED:
            // always sent Full-Range ChangedArray
            // send this to be sure HU requests call stacks. This should not be
            // necessary, but due to sync errors occurred often in the past with CK5050
            // we still send this again here
            result = _APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray();
            break;
    }
    (void)result;
}


void APP_PHONEBOOK_BAP_vOnErmNotification(const ERMNotificationType * pNotification)
{
    SuccessFailType result = SUCCESS;
    switch(pNotification->common.id)
    {
        //case ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED:
        case ERM_NOTIFY_PBSN_PHONE:
            _APP_PHONEBOOK_BAP_vHandlePhoneStateChanged();
            break;
    }
    (void)result;
}

void APP_PHONEBOOK_BAP_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    switch(notification)
    {
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:
        {
            PbMobileServiceSupport pbMss;
            /* reset mob Service support */
            (void)memset(&pbMss, 0x00, sizeof(pbMss));
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&pbMss);
            break;
        }
        case APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN:
            APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_APP_PHONEBOOK_BAP);
            break;
        case APP_CSHDL_NOTIFY_KL_15_OFF:
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON:
        default:
            /* ignore */
            break;
    }
}


static void _APP_PHONEBOOK_BAP_vHandlePhoneStateChanged()
{
    // no matter if a phone got disconnected or connected, reset variables for
    gAppPBBapContext.bUsersPbAlreadySyncedAfterConnect      = BT_FALSE;

    if( AT_INVALID_DEVICE_ID != ERM_u8GetActivePhone())
    {   /* active phone, ceck for existing user profile */
        PbMobileServiceSupport pbMss;

        if( NULL == ERM_pGetActiveUserProfile())
        {
            // do nothing here, wait for profile to be created
            return;
        }

        /* active phone with existing user profile */
        _APP_PHONEBOOK_BAP_vSetBasicPbMobileServiceSupport(&pbMss);
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&pbMss);

        /* send PbState that pb is not yet available in ck5050, HU shall show persistently stored PB, if any  */
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_NOT_AVAILABLE, 0);

        /* phone connected, send mailbox number to hu */
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox();
    }
    else
    {
        PbMobileServiceSupport pbMss;
        (void)memset(&pbMss, 0, sizeof(pbMss));
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&pbMss);

        // send pbstate property with not_available and 0 entries
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState( BAP_PB_NOT_AVAILABLE, 0);
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(0,0);

        //SUSANNE: 21.10.2009 As discussed with Heiko Diekelmann we need to delete the stacks
        //         through sending Full-range ChangedArray for each stack
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallListChangedArray();
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray();
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray();
    }
}


void APP_PHONEBOOK_BAP_vOnAppDeviceNotification(APP_DEVICE_enNotificationId new_notification)
{
    SuccessFailType result = SUCCESS;
    switch(new_notification)
    {
        case APP_DEVICE_NOTIFY_MAILBOX_NUMBER_CHANGED:
            result = _APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox();
            break;
        case APP_DEVICE_NOTIFY_USER_PROFILE_ADDED:
        {
            ATPhoneBookDwnlState dwldState = APP_PHONEBOOK_enGetPBDwldState();

            _APP_PHONEBOOK_BAP_vHandlePhoneStateChanged();

            /* check whether pb has already been downloaded */
            if((AT_PB_LOAD_FINISHED     == dwldState ||
                AT_PB_LOAD_FINISHED_BUT == dwldState))
            {   /* pb sync has already been finished, so update pb's via bap */
                /* set pbState to loading first */
                Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
                DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "NewUserAndPbAlrSynced");

                if(AT_PB_LOAD_FINISHED_BUT == dwldState && 0 < pbSize)
                {   // we received PPBU:2 but have a pbSize greater zero so we assume its incomplete
                    _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__CHANGED_INCOMPLETE);
                }
                else
                {   // in all other cases we indicate a normal change
                    _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PB_ACT_TYPE__CHANGED);
                }
                // sends MissedCallIndication and ChangedArray
                (void)_APP_PHONEBOOK_BAP_sfHandleMissedCallListChanged();
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray();
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray();
            }
            /* check if sms intitialization has already been finished */
            if( APP_SMS_INITIALIZED_AND_SUPPORTED == APP_SMS_enGetSmsSupportState())
            {
                (void)_APP_PHONEBOOK_BAP_sfHandleSmsSupport(BT_TRUE);
            }
            /* else sms is not supported ( as already default setting */

            break;
        }
    }
    (void)result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfHandleDeletedSms                        */
/**
    \brief      handles deleted sms notification from APP_SMS

    \brief      Description:\n
                handles deleted sms notification from APP_SMS \n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleDeletedSms(void)
{
    // send Changed Array
    _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsDeletedChangedArray();
    return SUCCESS;

}/* END OF _APP_PHONEBOOK_BAP_sfHandleDeletedSms() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfHandleNewSms                           */
/**
    \brief      handles new sms notification from APP_SMS

    \brief      Description:\n
                handles new sms notification from APP_SMS \n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleNewSms()
{
    // update number of unread sms
    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState( 0x01, 0x00,
                      APP_SMS_u16GetNumUnreadSms() );

    // send Changed Array
    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendNewSmsChangedArray();

    return SUCCESS;

}/* END OF _APP_PHONEBOOK_BAP_vHandleSmsMsg() */

///*----------------------------------------------------------------------------*/
///* Function    : _APP_PHONEBOOK_BAP_sfHandleUserPbChanged                     */
///**
//    \brief      handles notification that users pb changed
//
//    \brief      Description:\n
//                handles notification that users pb changed\n
//
//    \return     SuccessFailType
//*/
///*----------------------------------------------------------------------------*/
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleUserPbChanged()
//{
//    Uint16Type newPbIndex = 0U;
//#if 0
//    ATPhoneBookDwnlState state = APP_PHONEBOOK_enGetPBDwldState();
//    Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
//#endif
//    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
//
//
//    if( NULL == profile )
//    {
//        /* this might be a bug. Do not communicate the change to BAP */
//        DEBUG_Printf(UHV_ERROR, UHV_APP_PHONEBOOK_BAP, "HdlUsrPbChanged: Usr prof. NULL");
//        return SUCCESS;
//    }
//
//    // do not trigger hu when there is not yet a phonebook
//    if(state == AT_PB_LOADING && 0 == pbSize)
//    {   /* do nothing */
//        return SUCCESS;
//    }
//
//    // check how last syncro was finished
//    newPbIndex = APP_PHONEBOOK_u16GetAddedPbIndex();
//    if( 0xFFFF != newPbIndex )
//    {   /* phonebook changed within FSG */
//        /* if new Pb index is known */
//        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendNewPbEntryChangedArray(newPbIndex);
//        /* if new pb index is not known */
//        //_APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray(0, 0xFFFF);
//    }
//    else
//    {
//        // user pb has been changed and hu shall download it again
//        // set start state
//        gAppPBBapContext.pbActState      = APP_PB_BAP_ACT_PB__PB_STATE_LOADING;
//        if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
//        {
//            // set pending variable, cause there is a fct already in progress
//            gAppPBBapContext.bActPbPending   = BT_TRUE;
//        }
//        else
//        {
//            gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_PB;
//            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
//        }
//    }
//
//    return SUCCESS;
//
//}/* END OF */

///*----------------------------------------------------------------------------*/
///* Function    : _APP_PHONEBOOK_BAP_sfHandleUserPbNotChanged                     */
///**
//    \brief      handles notification that users pb not changed
//
//    \brief      Description:\n
//                handles notification that users pb not changed\n
//
//    \return     SuccessFailType
//*/
///*----------------------------------------------------------------------------*/
//static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleUserPbNotChanged()
//{
//    //// just tell hu that phonebook is now available but has not been changed
//#if 0
//    Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
//    ATPhoneBookDwnlState state = APP_PHONEBOOK_enGetPBDwldState();
//#endif
//    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
//
//
//    if( NULL == profile )
//    {
//        /* this might be a bug. Do not communicate the change to BAP */
//        DEBUG_Printf(UHV_ERROR, UHV_APP_PHONEBOOK_BAP, "HdlUsrPbNotChang: Usr prof. NULL");
//        return SUCCESS;
//    }
//
//#if 0
//    if( BT_FALSE == profile->bPbLoadedCompletly ||
//        BT_TRUE  == APP_PHONEBOOK_bWasManualSync() )
//    {
//        /* pb has never been completely loaded or was manually synced, so hu
//         * shall download the complete pb (again) */
//        DEBUG_Printf(UHV_TRACE, UHV_APP_PHONEBOOK_BAP, "UsrPbNotChanged: Load:%d,ManSync:%d",
//                     profile->bPbLoadedCompletly, APP_PHONEBOOK_bWasManualSync());
//        gAppPBBapContext.pbActState      = APP_PB_BAP_ACT_PB__PB_STATE_LOADING;
//    }
//    else
//#endif
//
//
//    if(BAP_PB_LOAD_COMPLETE == gAppPBBapContext.curPbState)
//    {
//        // user phonebook sync has been finished but pb hasn't changed and is already activated
//        DEBUG_Printf(UHV_TRACE, UHV_APP_PHONEBOOK_BAP, "HdlUsrPbNotChang: alrd. activ, skip act.");
//        return SUCCESS;
//    }
//    else
//    {
//        /* pb has not been changed */
//        // SUSANNE: Reactivating this case, cause we have to activate the phonebook, else RCDs will never show a pb without
//        // updating the pb via SD!!!! There is a different BAP message sequence in activating an old PB and activating a new, changed pb !!!
//        gAppPBBapContext.pbActState    = APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE;
//        DEBUG_Printf(UHV_TRACE, UHV_APP_PHONEBOOK_BAP, "HdlUsrPbNotChang: act. old Pb");
//    }
//
//    if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
//    {
//        // set pending variable
//        gAppPBBapContext.bActPbPending   = BT_TRUE;
//    }
//    else
//    {
//        gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_PB;
//        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
//    }
//    return SUCCESS;
//}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfPreparePbActivation                     */
/**
    \brief      prepares to activate user phonebook depending on notification
                received by APP_PHONEBOOK

    \brief      Description:\n
                prepares to activate user phonebook depending on notification
                received by APP_PHONEBOOK\n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_BAP_sfPreparePbActivation(APP_PHONEBOOK_BAP_enPbActType actType)
{
    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
    if( NULL == profile )
    {
        /* this might be a bug. Do not communicate the change to BAP */
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "PrepPbAct: Usr prof. NULL");
        return SUCCESS;
    }

    switch(actType)
    {
        case APP_PB_ACT_TYPE__CHANGED:
            gAppPBBapContext.pbActState      = APP_PB_BAP_ACT_PB__PB_STATE_LOADING;
            if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
            {
                // set pending variable, cause there is a fct already in progress
                gAppPBBapContext.bActPbPending   = BT_TRUE;
            }
            else
            {
                gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_PB;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
            }
            break;

        case APP_PB_ACT_TYPE__CHANGED_INCOMPLETE:
            // when not changed and pbState already 0x02 -> do not activate again
            gAppPBBapContext.pbActState      = APP_PB_BAP_ACT_PB__PB_STATE_LOADING;
            if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
            {
                // set pending variable, cause there is a fct already in progress
                gAppPBBapContext.bActPbIncompletePending   = BT_TRUE;
            }
            else
            {
                gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_INC_PB;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
            }
            break;

        case APP_PB_ACT_TYPE__NOT_CHANGED:
            // when not changed and pbState already 0x02 -> do not activate again
            if(BAP_PB_LOAD_COMPLETE != gAppPBBapContext.curPbState)
            {
                gAppPBBapContext.pbActState      = APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE;
                if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
                {
                    // set pending variable, cause there is a fct already in progress
                    gAppPBBapContext.bActPbPending   = BT_TRUE;
                }
                else
                {
                    gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_PB;
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
                }
            }
            else
            {
                DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP,"PrepPbAct: ign. not chgd");
            }
            break;

        case APP_PB_ACT_TYPE__NOT_CHANGED_INCOMPLETE:
            // when not changed and pbState already 0x02 -> do not activate again
            if(BAP_PB_LOAD_INCOMPLETE_AVAILABLE != gAppPBBapContext.curPbState)
            {
                gAppPBBapContext.pbActState      = APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE;
                if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
                {
                    // set pending variable, cause there is a fct already in progress
                    gAppPBBapContext.bActPbIncompletePending   = BT_TRUE;
                }
                else
                {
                    gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_INC_PB;
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
                }
            }
            else
            {
                DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP,"PrepPbAct: ign. inc. not chgd");
            }
            break;

        case APP_PB_ACT_TYPE__NOT_AVAILABLE:
            // send pbstate not available -> 0x00
            if(BAP_PB_NOT_AVAILABLE != gAppPBBapContext.curPbState)
            {
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_NOT_AVAILABLE, 0);
            }
            break;

        case APP_PB_ACT_TYPE__OLD_AFTER_SORTING_TIMOUT:
            // when sorting was started, we sent PbState Loading as discussed with VW. When sorting was
            // successful, a new pb is activated. But here in this case, sorting wasn't successful so
            // we don't want to notify the user about a new pb, just activate the old one.
            // needed steps are: Reset PbState Loading via PbState NotAvailable, than normale Activation
            // with PbState Complete / Incomplete and Changed Array
            gAppPBBapContext.pbActOldPbState      = APP_PB_BAP_ACT_OLD_PB__PB_STATE_NOT_AVAILABLE;
            if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
            {
                // set pending variable, cause there is a fct already in progress
                gAppPBBapContext.bActOldPbPending   = BT_TRUE;
            }
            else
            {
                gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_OLD_PB_AFTER_SORT_TIMEOUT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(600), MSEC(0));
            }
            break;
    }

    return SUCCESS;

}/* END OF */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfUpdatePBDwldState                       */
/**
    \brief      update bap pb download state

    \brief      Description:\n
                update bap pb download state\n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_sfUpdatePbDwldState()
{
    SuccessFailType result = SUCCESS;
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfHandleMissedCallListChanged                     */
/**
    \brief      handles notification that missed call list changed

    \brief      Description:\n
                handles notification that missed call list changed\n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleMissedCallListChanged()
{
    Uint16Type missedCalls      = 0U;
    Uint16Type missedNumbers    = 0U;
    Uint16Type stackSize        = 0U;

    // current missed calls indication is sent in any case
    // *** BEGIN: Susanne 07.08.09: Use only missedCalls detected by TMS
    missedCalls     = APP_PHONE_u16MissedCalls();
    missedNumbers   = APP_PHONE_u16MissedNumbers();
    stackSize       = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enMissedCalls);

    // if we detected missed call but missed call stack size is zero, then do not
    // send the indication, cause callstack seems to be not supported and user will wonder about an
    // empty list although there was a missed call indication
    if( 0 < missedCalls &&
        0 < stackSize)
    {
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(missedCalls, missedNumbers);
    }
    else
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Skip MissCallInd. calls: %d numbers: %d)",
                     missedCalls, missedNumbers);
        DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Skip MissCallInd. stack size: %d",
                     stackSize);
    }
    // *** END: Susanne 07.08.09: Use only missedCalls detected by TMS

    // always sent ChangedArray
    _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallListChangedArray();

    return SUCCESS;

}/* END OF */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfHandleNumberOfUnreadSmsChanged                     */
/**
    \brief      handles notification that number of unread sms has been changed
                e.g. sms was read out by vcr or sd

    \brief      Description:\n
                handles notification that number of unread sms has been changed
                e.g. sms was read out by vcr or sd \n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandleNumberOfUnreadSmsChanged()
{
    //Uint16Type numUnread = APP_SMS_u16GetNumUnreadSms();

    // set sms state is not requested in MSC
    //_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState(0x01, 0x00, numUnread);
    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray();

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_SMS_sfHandleSmsSupport                         */
/**
    \brief      updates sms support\n

    \brief      Description:\n
                updates sms support\n

    \param      bIsSmsSupported
                BooleanType
                BT_TRUE or BT_FALSE

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType
_APP_PHONEBOOK_BAP_sfHandleSmsSupport(BooleanType bIsSmsSupported)
{
    BooleanType bIsSmsAllowed = BT_FALSE;
    Uint8Type u8IsSmsAllowed;

    // update mobile service support due to indication if sms supported or not

    // tell HU that we support sms, but sim data is not yet available
    /* bits are twisted ( bit order in each byte is changed ) */
    /* sms bits are now set cause PMST=1 was received */
    //const Uint8Type mobileServiceSupportBuf[] = { 0x38,0x03,0xfb,0xD4,0x80,0x0,0x0,0x0 };
    //const Uint8Type mobileServiceSupportBuf[] = { 0x1C,0xC0,0xDF,0x2B,0x01,0x0,0x0,0x0 };

    // first check if sms is allowed for head unit
    (void)EEPROM_enGetSingleByteFromRamMirror(&u8IsSmsAllowed, EEPROM_LAYOUT_ENUM_SMSWriting);
	if ( u8IsSmsAllowed == 0x1 )
	{
		bIsSmsAllowed = BT_TRUE;
	}
	else if ( u8IsSmsAllowed == 0x0 )
	{
		bIsSmsAllowed = BT_FALSE;
	}
	else
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP,"APP_DIAG_GetSms_Writ failed");
        // make sure value
        bIsSmsAllowed = BT_FALSE;
    }


    // check if sms is supported and allowed
    if(BT_TRUE == bIsSmsSupported && BT_TRUE == bIsSmsAllowed)
    {
        Uint16Type numUnread = APP_SMS_u16GetNumUnreadSms();

        PbMobileServiceSupport pbMss;
        _APP_PHONEBOOK_BAP_vSetBasicPbMobileServiceSupport(&pbMss);
        pbMss.SMSState      = 1;
        pbMss.SMSReceived   = 1;
        pbMss.SMSSend       = 1;
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&pbMss);

        DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlSmsSupp: sms supp. due to supp: %d,allow: %d",
                     bIsSmsSupported, bIsSmsAllowed);

        // set sms state
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState(0x01, 0x00, numUnread);
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray();
    }
    else
    {
        PbMobileServiceSupport pbMss;

        DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlSmsSupp: no sms due to supp: %d,allow: %d",
                     bIsSmsSupported, bIsSmsAllowed);

        _APP_PHONEBOOK_BAP_vSetBasicPbMobileServiceSupport(&pbMss);
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&pbMss);
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState(0x00, 0x00, 0x0000);
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnSmsSendCb                              */
/**
    \brief      handle sms send result\n

    \brief      Description:\n
                handle sms send result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnSendSmsCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_SMSSend;
    if( APP_COMMON_RSP_OK == result )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Phonebook, fctId);
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    }
    _APP_PHONEBOOK_BAP_vReleaseContext(&gAppPBBapContext.common.bapInd);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadSmsCb                              */
/**
    \brief      handle sms read result\n

    \brief      Description:\n
                handle sms read result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadSmsCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_SMSReceived;
    if( APP_COMMON_RSP_OK == result )
    {
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsRequestFinished();
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnMarkSmsAsReadCb                        */
/**
    \brief      handle mark sms as read result\n

    \brief      Description:\n
                handle mark sms as read result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnMarkSmsAsReadCb(APP_COMMON_enResponseCode result)
{
    Uint16Type pos = gAppPBBapContext.appSms.convHeader.start;
    if( APP_COMMON_RSP_OK == result )
    {
        // get pos from ConvHeader ( start pos )
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsMarkedAsRead(pos, BAP_SMS_TYPE_READ);
        // sms was put from unread to read folder and therefor changed its
        // position. For now, we have no indicator about the new position, so we
        // need to trigger the HeadUnit to make a full range update

        /* already sent via Notification handling */
        //_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray();
    }
    else
    {
        // sms couldn't be marked as read
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsMarkedAsRead(pos, BAP_SMS_TYPE_UNREAD);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnDeleteSmsCb                        */
/**
    \brief      handle delete sms result\n

    \brief      Description:\n
                handle delete sms result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnHandleDeleteSmsCb(APP_COMMON_enResponseCode result)
{
    //TODO: answer for complete range ( start -> elements )
    fctId_t fctId = BapFct_Phonebook_SMSReceived;
    if( APP_COMMON_RSP_OK == result )
    {

        // the delete process makes all sms with a pos higher than the deleted
        // one moving to the next smaller position
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsDeletedChangedArray();
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnSearchPbCb                      */
/**
    \brief      handle search pb result\n

    \brief      Description:\n
                handle search pb result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnSearchPbCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_PbSpeller;
    if( APP_COMMON_RSP_OK == result )
    {
        Uint16Type index    = 0;
        Uint16Type number   = 0;
        SuccessFailType result2 = APP_PHONEBOOK_pGetPbSearchResult(&index,
                                                                  &number);
        if( SUCCESS != result2 )
        {
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbSpellerResult(0,0);
        }
        else
        {
            // SUSANNE: #3410 When HU makes a PB Request we always add 1 to the
            // indizes, cause HU starts at zero, and we start at 1 in Ck5050
            // So when we get the result here, we need to decrease start index
            // with 1, so HU will make the correct request
            index = ( 0 < index ) ? (index - 1) : index;
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbSpellerResult(index, number);
        }
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnPbStartDownloadCb                      */
/**
    \brief      handle pb start download result\n

    \brief      Description:\n
                handle pb start download result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnPbStartDownloadCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_PbStartDownload;
    if( APP_COMMON_RSP_OK == result )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Phonebook,
                                                            fctId);
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadMissedCallsCb                      */
/**
    \brief      handle read missed calls result\n

    \brief      Description:\n
                handle read missed calls result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadMissedCallsCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_MissedCalls;
    //if( APP_COMMON_RSP_OK == result )
    //{
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(fctId);
    //}
    //else
    //{
    //    APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    //}
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadDialedListCb                      */
/**
    \brief      handle read dialed list result\n

    \brief      Description:\n
                handle read dialed list result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadDialedListCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_DialedNumbers;
    //if( APP_COMMON_RSP_OK == result )
    //{
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(fctId);
    //}
    //else
    //{
    //    APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    //}
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadRecvCallsListCb                      */
/**
    \brief      handle read received calls list result\n

    \brief      Description:\n
                handle read received calls list result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadRecvCallsListCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Phonebook_ReceivedCalls;
    //if( APP_COMMON_RSP_OK == result )
    //{
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(fctId);
    //}
    //else
    //{
    //    APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    //}
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadUsersPbCb                      */
/**
    \brief      handle read users pb result\n

    \brief      Description:\n
                handle read users pb result \n

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadUsersPbCb(APP_COMMON_enResponseCode result)
{
    //fctId_t fctId = BapFct_Phonebook_Phonebook;
    //if( APP_COMMON_RSP_OK == result )
    //{
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookStatusArray();
    //}
    //else
    //{
    //    APP_COMMON_UTILS_vSendBapError(BapLsg_Phonebook, fctId, AppErr_Unknown);
    //}
}

/* End Of File APP_PHONEBOOK_AT_C1.c */


