/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_ERM_C1.c
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
* http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_DEVICE_Erm_C1.c $
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
#include "OSEK_AppTask_Alarm.h"
#include "TRACE.h"
#include "APP_DEVICE_Erm_C1.id"
#include "ERM.h"

#include "APP_DEVICE.h"
#include "APP_DEVICE_CI.h"
#include "APP_DEVICE_Connection.h"
#include "APP_DEVICE_Profiles.h"
#include "APP_DEVICE_E2P_Backup_R2FS.h"

#include "APP_DEVICE_ERM_C1.id"
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
/* Function :   APP_DEVICE_vHandleErmATResultEvent                            */
/**
    \brief      Handle result events from ERM (OK/ERROR)

    \param      result
                APP_DEVICE_enResult

    \return     void

*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vHandleErmATResultEvent(APP_COMMON_enResponseCode response, ERMExtendedErrorCode errorcode)
{
    DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HandleErmATRes: %d, Fct %d",
                  response,
                  gAppDeviceContext.fct.common.fctType);

    /* add all fctIds here, which wait for a result from Ck */
    switch( gAppDeviceContext.fct.common.fctType )
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
            _APP_DEVICE_vOnServiceActivationResult(response, errorcode);
            /* do not release the context here */
            break;
        case APP_DEVICE_DEACTIVATE_PLAYER:
            _APP_DEVICE_vOnServiceDeactivationResult(response);
            /* do not release the context here */
            break;
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            _APP_DEVICE_vOnPhoneServiceActivationResult(response);
            /* do not release the context here */
            break;
        case APP_DEVICE_PHONE_SERVICE_DEACTIVATION:
            _APP_DEVICE_vOnPhoneServiceDeactivationResult(response);
            break;
        default:

            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "HandleErmATRes: Unexpected Fct %d",
                          gAppDeviceContext.fct.common.fctType);
            _APP_DEVICE_vSendResultReleaseContext(response, __LINE__);

            break;
    }

}/* END OF APP_DEVICE_vHandleErmATResultEvent() */

/*---------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vOnServiceActivationResult                         */
/**
    \brief      Is invoked after a result of service activation request is received

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnServiceActivationResult(APP_COMMON_enResponseCode response, ERMExtendedErrorCode errorcode)
{
    if(( APP_COMMON_RSP_OK == response )\
            /* error code 200 and 206 will show an already started serve avrcp/a2dp */
            ||((APP_COMMON_RSP_ERROR == response)\
                    && ((ERM_EXT_ERR_SERVICE_ALREADY_STARTED == errorcode)\
                            || (ERM_EXT_ERR_A2DP_ALREADY_STARTED == errorcode)) ))
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnSvcMdActResult: recvd AT_OK state: 0x%02x",
                      gAppDeviceContext.fct.actPlayer.state);

        switch(gAppDeviceContext.fct.actPlayer.state)
        {
            case APP_DEVICE_MDI_CONNECT_SENT:
            case APP_DEVICE_MDI_SERVICE_AVRCP_STOP_SENT:
            case APP_DEVICE_MDI_SERVICE_A2DP_STOP_SENT:
                /* do nothing here but wait for bt connection / services stopped */
                break;
//            case APP_DEVICE_MDI_SERVICE_NOT_SENT:
//                /* do nothing here but wait for
//                 * the ERM_NOTIFY_MDI_SERVICE_CHANGED notifcation */
//                break;
            case APP_DEVICE_MDI_SERVICE_AVRCP_START_SENT:
            {
                /* check if we are already finished, else wait for PBSN */
                if(BT_TRUE  == _APP_DEVICE_Connection_bMdActivateFinished(gAppDeviceContext.fct.actPlayer.deviceId))
                {   /* release */
                    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
                }
                else
                {
                    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "vOnSvcMdActRes: wait for avrcp PBSN",
                                  gAppDeviceContext.fct.actPlayer.deviceId);
                }
                break;
            }
            case APP_DEVICE_MDI_SERVICE_A2DP_START_SENT:
            {
                /* check if we are already finished, else wait for PBSN */
                if(BT_TRUE  == _APP_DEVICE_Connection_bMdActivateFinished(gAppDeviceContext.fct.actPlayer.deviceId))
                {   /* release */
                    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
                }
                else
                {
                    if (APP_COMMON_RSP_ERROR == response)
                    {
                        /* RT-14037: if we reach here because of error, the response code was 200 or 206.
                         * if in this case the mediaplayer activation is not finished we set the flag
                         * for avrcp activation, like in handling for a2dp start succeeded.
                         */
                        /* retry to start AVRCP */
                        gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_SERVICE_NOT_SENT;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(1000), 0);
                    }
                    else
                    {
                        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "vOnSvcMdActRes: wait for a2dp PBSN",
                                gAppDeviceContext.fct.actPlayer.deviceId);
                    }
                }
                break;
            }
            default:    /* let HandlePendingMdActivate decide what to do */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(1000), 0);
                break;
        }
    }
    else    /* received an AT ERROR */
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnSvcMdActResult: recvd AT_ERROR state: 0x%02x",
                          gAppDeviceContext.fct.actPlayer.state);

        /* check if we are already finsihed and got an already started error, else retry */
        if(BT_TRUE  == _APP_DEVICE_Connection_bMdActivateFinished(gAppDeviceContext.fct.actPlayer.deviceId))
        {   /* TODO: release */
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
        else
        {
            /* retry to start AVRCP */
            gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_SERVICE_NOT_SENT;

            /* give some more time cause some phones disconnect their services if we are too fast */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(1500), 0);
        }
    }
}

/*---------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vOnPhoneServiceActivationResult                         */
/**
    \brief      Is invoked after a result of service activation request is received

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnPhoneServiceActivationResult(APP_COMMON_enResponseCode response)
{
    if( APP_COMMON_RSP_OK == response )
    {

        DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnSvcPhoneActRes: recvd AT_OK state: 0x%02x",
                      gAppDeviceContext.fct.serviceAct.state);

        switch(gAppDeviceContext.fct.serviceAct.state)
        {
            case APP_DEVICE_PHONE_CONNECT_SENT:
                /* do nothing here but wait for bt connection */
                break;
            case APP_DEVICE_PHONE_WAIT_FOR_SERVICE_STOPPED:
                /* do nothing here but wait for phone service being stopped */
                break;
            case APP_DEVICE_PHONE_WAIT_FOR_MP_DEVICE_DISCONNECTED:
                /* do nothing here but wait for phone service being stopped */
                break;
            case APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED:
                /* do nothing here but wait for phone service being stopped */
                break;
            case APP_DEVICE_PHONE_START_SERVICE_SENT:
            {
                /* check if we are already finished, else wait for PBSN */
                if(BT_TRUE  == _APP_DEVICE_Connection_bPhoneActivateFinished(gAppDeviceContext.fct.serviceAct.deviceId))
                {   /* release */
                    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
                }
                else
                {
                    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "vOnSvcPhoneActRes: wait for phone PBSN",
                                  gAppDeviceContext.fct.serviceAct.deviceId);
                }
                break;
            }
            default:
                /* not waiting for any special event, try to gon on */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                break;
        }
    }
    else    /* received an AT ERROR */
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnSvcPhoneActRes: recvd AT_ERROR state: 0x%02x",
                          gAppDeviceContext.fct.serviceAct.state);

        /* check if we are already finsihed and got an already started error, else retry */
        if(BT_TRUE  == _APP_DEVICE_Connection_bPhoneActivateFinished(gAppDeviceContext.fct.serviceAct.deviceId))
        {   /* release */
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
        else
        {   /* reset the state */
            DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "error to AT*psbd received reset gAppDeviceContext.fct.serviceAct.state to APP_DEVICE_PHONE_STOP_SERVICE_NOT_SENT");
            /* start always from the beginning, the handlependingfunction will decide about the next action to be executed */
            gAppDeviceContext.fct.serviceAct.state = APP_DEVICE_PHONE_STOP_SERVICE_NOT_SENT;

            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(1500), 0);
        }
    }
}


/*---------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vOnServiceDeactivationResult                       */
/**
    \brief      Is invoked after a result of service deactivation request is received

    \param      response
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnServiceDeactivationResult(APP_COMMON_enResponseCode response)
{
    if( APP_COMMON_RSP_OK == response )
    {
        switch(gAppDeviceContext.fct.deactPlayer.state)
        {
            case APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED:
            case APP_DEVICE_MDI_SERVICE_A2DP_STOP_SENT:
            case APP_DEVICE_MDI_SERVICE_AVRCP_STOP_SENT:
                /* do nothing here but wait for
                 * the ERM_NOTIFY_MDI_SERVICE_CHANGED notifcation */
                break;
            default:    /* let HandlePendingMdDeactivate decide what to do */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                break;
        }
    }
    else
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnServiceMdDeacRes: rcvd AT_ERROR state:%02x",
                      gAppDeviceContext.fct.deactPlayer.state);

        /* check if we are already finsihed and got an not started / not connected error, else retry */
        if(BT_TRUE  == _APP_DEVICE_Connection_bMdDeactivateFinished(gAppDeviceContext.fct.actPlayer.deviceId))
        {   /* release */
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
        else
        {
            /* reset the state */
            DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "error to AT*psbd received reset gAppDeviceContext.fct.deactPlayer.state to APP_DEVICE_MDI_SERVICE_NOT_SENT");
            /* start always from the beginning, the handlependingfunction will decide about the next action to be executed */
            gAppDeviceContext.fct.deactPlayer.state = APP_DEVICE_MDI_SERVICE_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
        }
    }
}

/*---------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vOnPhoneServiceDeactivationResult                       */
/**
    \brief      Is invoked after a result of service deactivation request is received.

                if request was sent successful, we have to wait for the final event.
                If request was not sent, we need to retry.

    \param      response
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnPhoneServiceDeactivationResult(APP_COMMON_enResponseCode response)
{
    if( APP_COMMON_RSP_OK == response )
    {
        switch(gAppDeviceContext.fct.disconnectDevice.state)
        {
            case APP_DEVICE_DISCONNECT_DEVICE_NOT_SENT:
                /* disconnect is sent, now wait for final disconnect notification
                 * PSBD.
                 */
                DEBUG_TEXT(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE,"OnPhoneServiceDeactivationResult: Successful sent.")
                gAppDeviceContext.fct.disconnectDevice.state = APP_DEVICE_DISCONNECT_DEVICE_SENT;
                break;
            default:    /* let Pending function decide what to do */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                break;
        }
    }
    else
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnPhoneServiceDeactivationResult: rcvd AT_ERROR state:%02x",
                gAppDeviceContext.fct.disconnectDevice.state);

        /* check if we are already finsihed and got an not started / not connected error, else retry */
        if(  AT_INVALID_DEVICE_ID == APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE))
        {   /* release */
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
        else
        {
            /* reset the state */
            DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "error to AT*psbd received reset gAppDeviceContext.fct.deactPlayer.state to APP_DEVICE_MDI_SERVICE_NOT_SENT");
            /* start always from the beginning, the handlependingfunction will decide about the next action to be executed */
            gAppDeviceContext.fct.disconnectDevice.state = APP_DEVICE_DISCONNECT_DEVICE_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
        }
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vOnErmNotification                             */
/**
    \brief      handles notifications from ERM module

    \param      notification
                ERMNotificationType
                notification received from ERM module
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vOnErmNotification(const ERMNotificationType * pNotification)
{

    /************************************************************/
    /* first handle notification for connection specific stuff */
    /************************************************************/
    _APP_DEVICE_Connection_vOnErmNotification(pNotification);

    /************************************************************/
    /* first handle notification for profile specific stuff */
    /************************************************************/
    _APP_DEVICE_Profiles_vOnErmNotification(pNotification);

    /********************************************************************/
    /* first handle notification for E2P / Backup / R2FS specific stuff */
    /********************************************************************/
    _APP_DEVICE_E2P_Backup_R2FS_vOnErmNotification(pNotification);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_Connection_vOnErmNotification                     */
/**
    \brief      handles the ERM notifications predestinated for the connection app

    \param      pNotification
                ERMNotificationType

    \return     void

*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vOnErmNotification(const ERMNotificationType * pNotification)
{

    DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE,"OnErmNot: notId %d", pNotification->common.id);

    switch(pNotification->common.id)
    {
        case ERM_NOTIFY_STARTUP_DONE:
            break;
        case ERM_NOTIFY_PBSN_PHONE:
            _APP_DEVICE_Connection_vHandlePhoneNotification(&pNotification->pbsn_phone);
            break;
        case ERM_NOTIFY_PBSN_A2DP:
            _APP_DEVICE_Connection_vHandleA2dpNotification(&pNotification->pbsn_a2dp);
            break;
        case ERM_NOTIFY_PBSN_AVRCP:
            _APP_DEVICE_Connection_vHandleAvrcpNotification(&pNotification->pbsn_avrcp);
            break;
        case ERM_NOTIFY_PPRS:
            _APP_DEVICE_Connection_vHandlePairingNotification(&pNotification->pprs);
            break;
        case ERM_NOTIFY_PSBD:
            _APP_DEVICE_Connection_vHandleBtConnectionNotification(&pNotification->psbd);
            break;
        case ERM_NOTIFY_SERVICE_SUPPORT_CHANGED:
            _APP_DEVICE_Connection_vHandleServiceSupportNotification(&pNotification->pbsc);
            break;
        default:
            /* ignore notification */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_Profiles_vOnErmNotification                */
/**
    \brief      handles the ERM notifications predestinated for the profiles app

    \param      pNotification
                ERMNotificationType

    \return     void

*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Profiles_vOnErmNotification(const ERMNotificationType * pNotification)
{
    DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE,"OnErmNot Profiles: notId %d", pNotification->common.id);

    switch(pNotification->common.id)
    {
        /**********************************************************/
        /* put all profile associated stuff here */
        /**********************************************************/
        case ERM_NOTIFY_STARTUP_DONE:
            break;
        case ERM_NOTIFY_PBSN_PHONE:
            _APP_DEVICE_Profiles_vHandlePhoneStateChanged(pNotification);
            break;
        case ERM_NOTIFY_DELETED_DEVICE:
            if (APP_DEVICE_ERM_DELETE_ALL_PROFILES == gAppDeviceContext.fct.common.fctType)
            {
                /* function type is "delete all profiles" and set at the end of *PPNE:<id> the next state */
                gAppDeviceContext.fct.delAllProfiles.state = APP_DEVICE_DELETE_ALL_PROFILES_IN_EEPROM_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            break;
        case ERM_NOTIFY_PBSN_A2DP:
             /* handle profile specific stuff when player connects / disconnects */
            _APP_DEVICE_Profiles_vHandlePlayerStateChanged(pNotification);
            break;
        default:
            /* ignore notification */

            DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE,"OnErmNot Profiles: notId %d ignored", pNotification->common.id);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_DEVICE_E2P_Backup_R2FS_vOnErmNotification                            */
/**
    \brief      handles the flash erase notification from ERM

    \param      pNotification
                ERMNotificationType

    \return     void

*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_E2P_Backup_R2FS_vOnErmNotification(const ERMNotificationType * pNotification)
{
    DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE,"E2P_Backup_R2FS_vOnErmNotification: notId %d", 
                 pNotification->common.id);

    switch(pNotification->common.id)
    {
        /**********************************************************/
        /* put all R2FS, E2P and Backup specific handlings here   */
        /**********************************************************/
        case ERM_NOTIFY_STARTUP_DONE:
            break;
        case ERM_NOTIFY_FLASHERASE_END:
            if (APP_DEVICE_ERM_RESET_TO_FACTORY_SETTING == gAppDeviceContext.fct.common.fctType)
            {
                gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_WRITE_BACK_BT_PIN_NAME;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            break;
        default:
            /* ignore notification */
            DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE,"E2P_Backup_R2FS_vOnErmNotification: notId %d ignored", pNotification->common.id);
            break;
    }
}

/* End Of File APP_DEVICE_Erm_C1.c */


