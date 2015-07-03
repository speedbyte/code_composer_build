/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_CSHDL_C1.c
*-----------------------------------------------------------------------------
* Module Name:    AppTask part of CSHDL
*-----------------------------------------------------------------------------
* Description:    manage KL15 on / off and CK5050 shutdown preparation
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
#include "osek.h"
#include "CSHDL_c1.h"
#include "APP_SD.h"
#include "APP_PHONE_BAP.h"
#include "APP_PHONEBOOK_BAP.h"
#ifdef UHV_ACTIVATE_FEATURE_LED
#include "LED.h"
#endif
#include "APP_DEVICE.h"
#include "PHY_BUT_DRV.h"  // KL15 Notification to Baseplate
#include "APP_DEVICE_AudioManager.h"
#include "APP_MDI_CI.h"//RT14519

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_CSHDL.h"
#include "APP_CSHDL_CI.h"

#include "APP_CSHDL_C1.id"
#include "TRACE.h"


/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

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
static void _APP_CSHDL_vHandleCshdlRequest(CSHDL_enSystemStates state);
static void _APP_CSHDL_vNotifyApps(APP_CSHDL_NotificationType notification);

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
static AppCshdlContext gAppCshdlContext;
#if defined(CRADLE_RESTRICTION_ACTIVE)
static APP_CSHDL_enCradleRestriction_States _AppCshdlCradleRestrictionState;
#endif
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
/* Function    : APP_CSHDL_sfInitApp                                         */
/**
    \brief      Callback function for initialization of App Cshdl

    \brief      Description:

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_CSHDL_sfInitApp(void)
{
    (void)memset(&gAppCshdlContext, 0x00, sizeof(gAppCshdlContext));

    /* if we startup we have at least ACTIVE_KL_15_OFF state, if we are in KL15_ON
       state we will receive an update */
    gAppCshdlContext.cshdlState = APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF;

#if defined(CRADLE_RESTRICTION_ACTIVE)
    _AppCshdlCradleRestrictionState = APP_CSHDL_CRADLE_RESTRICTION_UNKNOWN;
#endif

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CSHDL_vHandleEvents                                      */
/**
    \brief      Task Function to handle events for Application part of CSHDL

    \brief      Description:
                Task Function to handle events for Application part of CSHDL

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/

void APP_CSHDL_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // **************************************
    // look for events from CSHDL in HostTask
    // **************************************
    if( event & evAppTaskSystemStates )
    {
	    // check for new response messages from AT_Cmd_Parser
        CSHDL_enSystemStates state;

        (void)ReceiveMessage(msgAppTaskSystemStates ,&state);
        _APP_CSHDL_vHandleCshdlRequest(state);

        if( E_COM_NOMSG == GetMessageStatus(msgAppTaskSystemStates))
        {
            // clear only if all msg received
            (void)ClearEvent(evAppTaskSystemStates);
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_CSHDL_vHandleCshdlRequest                                      */
/**
    \brief      handles requests from CSHDL in HostTask

    \brief      Description:
                handles requests from CSHDL in HostTask

    \return     void
*/
/*----------------------------------------------------------------------------*/

static void _APP_CSHDL_vHandleCshdlRequest(CSHDL_enSystemStates state)
{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(   UHV_TRACE,
                    UHV_APP_CSHDL,
                    "vHandleCshdlRequest: recvd sys_state 0x%02x",
                    state );
#endif

    DEBUG_VALUE1(CSHDL_SWI_TRC, MOD_CSHDL, "_APP_CSHDL_vHandleCshdlRequest Get SystemState 0x%02x", state);

    switch(state)
    {
        case KL15_OFF:
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_KL_15_OFF);
            break;
        case UHV_ACTIVE_KL15_ON:
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON);
            break;
        case UHV_ACTIVE_KL15_OFF:
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF);
            break;
        case UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF);
            break;
        case PREPARE_CK5050_OFF:
            gAppCshdlContext.bPrepareCk5050Shutdown = BT_TRUE;
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN);
            break;
        case WAIT_FOR_SHUTDOWN:
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_WAIT_FOR_SHUTDOWN);
            break;
#if defined(CRADLE_RESTRICTION_ACTIVE)
        case CRADLE_RESTRICTION_ON:
            if (APP_CSHDL_CRADLE_RESTRICTION_ON != _AppCshdlCradleRestrictionState)
            {
                // Send only on change to the APP'S
                _AppCshdlCradleRestrictionState = APP_CSHDL_CRADLE_RESTRICTION_ON;
                _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON);
            }
            break;
        case CRADLE_RESTRICTION_OFF:
            if (APP_CSHDL_CRADLE_RESTRICTION_OFF != _AppCshdlCradleRestrictionState)
            {
                // Send only on change to the APP'S
                _AppCshdlCradleRestrictionState = APP_CSHDL_CRADLE_RESTRICTION_OFF;
                _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_OFF);
            }
            break;

        case CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES:
            _APP_CSHDL_vNotifyApps(APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES);
            break;
#endif

        default:
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(   UHV_ERROR,
                            UHV_APP_CSHDL,
                            "vHandleCshdlRequest: unknown sys_state 0x%02x",
                            state );
#endif
            DEBUG_VALUE1(CSHDL_SWI_ERR, MOD_CSHDL, "_APP_CSHDL_vHandleCshdlRequest: unknown SystemState 0x%02x", state);
            break;
    }
}

void APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_NotifiedApp app)
{
    if( BT_FALSE == gAppCshdlContext.bPrepareCk5050Shutdown )
    {   /* ignore */
        return;
    }


    switch(app)
    {
        //case APP_CSHDL_APP_DEVICE:
        //    gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_DEVICE;
        //    break;
        case APP_CSHDL_APP_SD:
            gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_SD;
            break;
        case APP_CSHDL_KPB:
            gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__KPB;
            break;
        case APP_CSHDL_ERM:
            gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__ERM;
            break;
        case APP_CSHDL_APP_PHONEBOOK_BAP:
            gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONEBOOK_BAP;
            break;
        case APP_CSHDL_APP_PHONE_BAP:
            gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONE_BAP;
            break;
        case APP_CSHDL_APP_PHONE:
            gAppCshdlContext.unhandledShutdown_AppBitmask &= ~APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONE;
            break;

        default:
            /* ignore */
            break;
    }


#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf( UHV_TRACE, UHV_APP_CSHDL, "app 0x%02x ready, 0x%02x",
                        app,
                        gAppCshdlContext.unhandledShutdown_AppBitmask );
#endif

    DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "APP_CSHDL_vCk5050ShutDownHandled Appl 0x%02x Ready 0x%02x ", app, gAppCshdlContext.unhandledShutdown_AppBitmask);

    /* check if all apps have handled ck5050 shutdown request */
    if( APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__NONE == gAppCshdlContext.unhandledShutdown_AppBitmask)
    {
        /* set event to Host task that we are finished */
        (void)SetEvent(OsSystemTask_event,evSystemTaskCallbackShutdown);
        gAppCshdlContext.bPrepareCk5050Shutdown = BT_FALSE;
    }
}

static void _APP_CSHDL_vNotifyApps(APP_CSHDL_NotificationType notification)
{
    //if(gAppCshdlContext.cshdlState == notification)
    //{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    //   (void)DEBUG_Printf(  UHV_WARNING,
    //                        UHV_APP_CSHDL,
    //                        "vNotifyApps: drop same state %d",
    //                        notification );
#endif
    //    return;
    //}


#if defined(CRADLE_RESTRICTION_ACTIVE)
    if ( (notification != APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON) &&
         (notification != APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_OFF) &&
         (notification != APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES)
       )
    {
        // 25.01.10 Absprache mit Roman:
        // store only notifications of System States not the Cradle Restriction information
        gAppCshdlContext.cshdlState = notification;
    }
#else
    gAppCshdlContext.cshdlState = notification;
#endif

    // TODO: If needed add new Application which needs to be notified about KL15
    // ON / OFF and CK5050 SHUTDown. In case of SHUTDOWN do not forget to call APP_CSHDL_vCk5050ShutDownHandled
    // when notified app has handled Notification !!!!!!!!!!!
    // Otherwise the evSystemTaskCallbackShutdown Event will not be sent back to
    // CSHDL in SystemTask!!!!!!
    MDI_vOnAppCshdlNotification(notification); //RT14519
    gAppCshdlContext.unhandledShutdown_AppBitmask |= APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__KPB;
    KPB_vOnAppCshdlNotification(notification);

    gAppCshdlContext.unhandledShutdown_AppBitmask |= APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_SD;
    APP_SD_vOnAppCshdlNotification(notification);

    gAppCshdlContext.unhandledShutdown_AppBitmask |= APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__ERM;
    ERM_vOnAppCshdlNotification(notification);

    gAppCshdlContext.unhandledShutdown_AppBitmask |= APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONEBOOK_BAP;
    APP_PHONEBOOK_BAP_vOnAppCshdlNotification(notification);

    gAppCshdlContext.unhandledShutdown_AppBitmask |= APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONE_BAP;
    APP_PHONE_BAP_vOnAppCshdlNotification(notification);

    gAppCshdlContext.unhandledShutdown_AppBitmask |= APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONE;

    APP_PHONE_vOnAppCshdlNotification(notification);
#ifdef UHV_ACTIVATE_FEATURE_LED
    LED_vOnAppCshdlNotification(notification);
#endif
    VCR_vOnAppCshdlNotification(notification);
//    MUTE_MANAGER_vOnAppCshdlNotification(notification);
    APP_DEVICE_vOnAppCshdlNotification(notification);
    PHY_BUT_DRV_vOnAppCshdlNotification(notification);
    APP_DEVICE_AM_vOnAppCshdlNotification(notification);
}


APP_CSHDL_NotificationType APP_CSHDL_eGetCshdlState()
{
    return gAppCshdlContext.cshdlState;
}

#if defined(CRADLE_RESTRICTION_ACTIVE)
APP_CSHDL_enCradleRestriction_States APP_CSHDL_eGetCradleRestrictionState(void)
{
    return(_AppCshdlCradleRestrictionState);
}
#endif
/* End Of File APP_CSHDL_C1.c */


