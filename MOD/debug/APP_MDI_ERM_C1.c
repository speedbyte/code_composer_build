/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_MDI_ERM_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MID Application
*-----------------------------------------------------------------------------
* Description:    Handles events received from ERM
*-----------------------------------------------------------------------------
* $Date: 2011-11-09 16:14:34 +0100 (Mi, 09 Nov 2011) $
* $Rev: 23192 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/src/APP_MDI_ERM_C1.c $
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
#include "ERM.h"
#include "bap_types.h"
#include "ATCmdHandler.h"
#include "APP_COMMON_Utils.h"
#include "APP_MDI_BAP.h"
#include "OSEK_AppTask_Alarm.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "APP_MDI_ERM_C1.id"
#include "APP_DEVICE_AudioManager.h"
#include "APP_DEVICE.h"

#include "APP_MDI.h"
#include "APP_MDI_CI.h"
#include "APP_MDI_AT.h"
#include "APP_MDI_ERM.h"
#include "KPB.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to hold module global data
*/
extern AppMdiContext gAppMdiContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _APP_MDI_vOnServiceVisibilityResult(APP_MDI_enResult result);
static void _APP_MDI_vOnDiscoResult(APP_MDI_enResult result);
static void _APP_MDI_vOnSkipResult(APP_MDI_enResult result);
static void _APP_MDI_vOnPGMTResult(APP_MDI_enResult result);

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
/* Function :   APP_MDI_vHandleErmATResultEvent                               */
/**
    \brief      Handle result events from ERM (OK/ERROR)

    \param      result
                APP_MDI_enResult

    \return     void

*/
/*----------------------------------------------------------------------------*/
void APP_MDI_vHandleErmATResultEvent(APP_MDI_enResult result)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_ERROR;


    DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleErmATRes: %d, Fct %d", 
                  result, 
                  gAppMdiContext.fct.common.id);

    switch( gAppMdiContext.fct.common.id )
    {
        case APP_MDI_JOB_PBSV:
            _APP_MDI_vOnServiceVisibilityResult(result);
            /* do not release the context here */ 
            return;
        case APP_MDI_JOB_DPLY:
            _APP_MDI_vOnDiscoResult(result);
            /* do not release the context here */ 
            return;
        case APP_MDI_JOB_SKIP:
            _APP_MDI_vOnSkipResult(result);
            /* do not release the context here */ 
            return;
        case APP_MDI_JOB_PGMT:
            _APP_MDI_vOnPGMTResult(result);
            /* do not release the context here */
            return;
        case APP_MDI_JOB_DGED:
        case APP_MDI_JOB_DGPS:
        case APP_MDI_JOB_DSRA:
        case APP_MDI_JOB_DSRE:
        case APP_MDI_JOB_DCPN:
        case APP_MDI_JOB_ALCK:
            if( result == APP_MDI_enResultOk )
            {
                response = APP_COMMON_RSP_OK;
            }
            break;
        default:
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "HandleErmATRes: Unknown Fct %d", 
                          gAppMdiContext.fct.common.id);
            break;
    }

    _APP_MDI_vSendResultReleaseContext(response, __LINE__);

}/* END OF APP_MDI_vHandleErmATResultEvent() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_ERM_vHandleErmStateChange                           */
/**
    \brief      Handle state change of ERM

    \param      newState
                ERMStateType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_ERM_vHandleErmStateChange(ERMStateType newState)
{
    /* dummy */
}/* END OF APP_MDI_vHandleErmStateChange() */


/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_ERM_vHandleErmNotification                           */
/**
    \brief      Handle notifications from ERM

    \param      new_notification
                ERMNotificationType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_ERM_vHandleErmNotification(const ERMNotificationType * pNotification)
{
    switch(pNotification->common.id)
    {
        case ERM_NOTIFY_STARTUP_DONE:
        {
            BooleanType playerEnabled = BT_FALSE;
            EEPROM_LAYOUT_vGetPlayerEnabled(&playerEnabled );
            APP_MDI_sfSetPlayerVisible( playerEnabled );
            break;
        }
        case ERM_NOTIFY_PBSN_A2DP:
			
            switch(pNotification->pbsn_a2dp.result)
            {
                case AT_SERVRES_START_SUCCEED:
                    if(KPB_bIsAsgAvailable() == BT_FALSE)
                    {
                        ERM_set_AutoConnectionMode(BT_FALSE);
                    }
					 ERM_vsetActiveCallFlagForSettingWithoutCI(BT_FALSE);
					 
                    if(gAppMdiContext.dev_meta_type == APP_MDI_INIT)
                    {
                        gAppMdiContext.dev_meta_type = APP_MDI_ONLY_A2DP;
                        _APP_MDI_vNotifyApps(APP_MDI_METADATATYPE_CHANGED);
                    }
                    AppTask_vSetAlarm(APP_TASK_ALARM_MDI_UNLOCK_AUDIO,  MSEC(APP_MDI_ALCK_STARTUP_DELAY), APP_MDI_cbStartupUnlockAudio);
                    break;
                case AT_SERVRES_STOP_SUCCEED:
                case AT_SERVRES_START_FAILED:
                    if(KPB_bIsAsgAvailable() == BT_FALSE  && ERM_bgetActiveCallFlagForSettingWithoutCI() == BT_FALSE)
                    {
                        ERM_set_AutoConnectionMode(BT_TRUE);
                    }
                    if( APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_AVRCP) == AT_INVALID_DEVICE_ID)
                    {
                        /*  release mute always when a2dp is disconnected*/
                        APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enAudioStream, __LINE__);

                        /*re-init the application MDI*/
                        _APP_MDI_vReInitApp();
                    }

                    if(APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_START) != SUCCESS)
                    {
 	                    
                        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "HandleErmNot: SetALCKMode failed");
                    }
                    break;
                default:
                    /* do nothing */
                    break;
            }
            break;
        case ERM_NOTIFY_PBSN_AVRCP:
            switch(pNotification->pbsn_avrcp.result)
            {
                case AT_SERVRES_START_SUCCEED:

                    if(KPB_bIsAsgAvailable() == BT_FALSE)
                    {
                        ERM_set_AutoConnectionMode(BT_FALSE);
                    }

                    gAppMdiContext.avrcp_device_id = pNotification->pbsn_avrcp.device_id;

                    if((gAppMdiContext.dev_meta_type == APP_MDI_INIT)||(gAppMdiContext.dev_meta_type == APP_MDI_ONLY_A2DP))
                    {
                        gAppMdiContext.dev_meta_type = APP_MDI_AVRCP_BASE;

                        if(APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK) != AT_INVALID_DEVICE_ID)
                        {
                            if(APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK) == pNotification->pbsn_avrcp.device_id)
                            {
                                _APP_MDI_vNotifyApps(APP_MDI_METADATATYPE_CHANGED);
                                AppTask_vSetAlarm(APP_TASK_ALARM_MDI_PGMT_REQUEST,  MSEC(APP_MDI_PGMT_REQUEST_DELAY), APP_MDI_cbGetPgmtDelayed);
                            }
                            else
                            {
                                DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "APP_MDI_ERM_vHandleErmNotification: a2dp not equal avrcp device!!");
                            }
                        }
                        else
                        {
                            DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "APP_MDI_ERM_vHandleErmNotification: invalid a2dp device");
                        }
                    }

                    break;
                case AT_SERVRES_STOP_SUCCEED:

                    /* change the MD service states if AVRCP disconnected by the phone */
                    gAppMdiContext.dev_meta_type = APP_MDI_ONLY_A2DP;

                    gAppMdiContext.avrcp_device_id = AT_INVALID_DEVICE_ID;  
                    if( APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK) == AT_INVALID_DEVICE_ID)
                    {
                        _APP_MDI_vReInitApp();
                    }
                    break;
                default:
                    /* do nothing */
                    break;
            }
            break;
        default:
            /* no handling */
            break;
    }
}


/*---------------------------------------------------------------------------*/
/* Function    : _APP_MDI_vOnServiceVisibilityResult                         */
/**
    \brief      Is invoked after a result of PBSV request is received

    \param      result
                APP_MDI_enResult

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_vOnServiceVisibilityResult(APP_MDI_enResult result)
{
    BooleanType playerEnabled = BT_FALSE;
    EEPROM_LAYOUT_vGetPlayerEnabled(&playerEnabled );

    if(result == APP_MDI_enResultOk)
    {
        if(gAppMdiContext.fct.pbsv.service == AT_SERVICE_A2DP_SINK)
        {
            gAppMdiContext.fct.pbsv.service = AT_SERVICE_A2DP_SOURCE;

            gAppMdiContext.fct.pbsv.action = AT_PBSV_ACT_DISABLE_VISIBILTY;

            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(0), 0);
        }
        else if((gAppMdiContext.fct.pbsv.service == AT_SERVICE_A2DP_SOURCE)&&(BT_FALSE == playerEnabled))
        {
            gAppMdiContext.fct.pbsv.service = AT_SERVICE_AVRCP;

            gAppMdiContext.fct.pbsv.action = AT_PBSV_ACT_DISABLE_VISIBILTY;

            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(0), 0);
        }
        else
        {
            _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
    }
    else
    {
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "OnServVisRes: failed %d", result);

        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
    }
}



/*---------------------------------------------------------------------------*/
/* Function    : _APP_MDI_vOnDiscoResult                                     */
/**
    \brief      Is invoked after a result of PBSV request is received

    \param      result
                APP_MDI_enResult

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_vOnDiscoResult(APP_MDI_enResult result)
{
    if(result == APP_MDI_enResultOk)
    {
        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
    }
    else
    {
        AppTask_vSetAlarm(APP_TASK_ALARM_MDI_PLAYSTATE,  MSEC(APP_MDI_FCT_DISCO_RETRY_TIME), APP_MDI_cbPlayStateTimeout);
        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
    }
}


/*---------------------------------------------------------------------------*/
/* Function    : _APP_MDI_vOnSkipResult                                     */
/**
    \brief      

    \param      result
                APP_MDI_enResult

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_vOnSkipResult(APP_MDI_enResult result)
{
    if(result == APP_MDI_enResultOk)
    {
        if(gAppMdiContext.fct.skip.counter > 1)
        {
            gAppMdiContext.fct.skip.counter--;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(200), 0);
        }
        else
        {
            _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
    }
    else
    {
        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
    }
}

/*---------------------------------------------------------------------------*/
/* Function    : _APP_MDI_vOnSkipResult                                     */
/**
    \brief

    \param      result
                APP_MDI_enResult

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_vOnPGMTResult(APP_MDI_enResult result)
{
    if(result == APP_MDI_enResultOk)
    {
        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        gAppMdiContext.PGMTErrorCounter = 0;
    }
    else
    {
        /* increase always the counter and retry after 5 sec. again*/
        gAppMdiContext.PGMTErrorCounter++;


        if(gAppMdiContext.PGMTErrorCounter < 5)
        {
            DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "vOnPGMTResult: PGMT failed, retry after 5 sec.");
            AppTask_vSetAlarm(APP_TASK_ALARM_MDI_PGMT_REQUEST,  MSEC(5000), APP_MDI_cbGetPgmtDelayed);

        }
        else
        {
            DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "vOnPGMTResult: PGMT failed, max retry reached, giving up!.");
            gAppMdiContext.PGMTErrorCounter = 0;
        }
        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
    }
}

/* End Of File APP_MDI_ERM_C1.c */


