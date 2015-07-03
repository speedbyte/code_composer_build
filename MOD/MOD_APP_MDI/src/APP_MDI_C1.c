/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_MDI_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_MDI
*-----------------------------------------------------------------------------
* Description:    Handles the control of the player specific communication to
                  the CK5050
*-----------------------------------------------------------------------------
* $Date: 2011-10-31 14:30:21 +0100 (Mo, 31 Okt 2011) $
* $Rev: 22910 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/src/APP_MDI_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_defines.h"    // vw
#include "BAP_Dispatcher.h"
#include "ATCmdHandler.h"
#include "ERM.h"
#include "UART.h"
#include "APP_COMMON_Utils.h"
#include "APP_CSHDL.h"
#include "APP_MDI_BAP.h"
#include "OSEK_AppTask_Alarm.h"
#include "APP_DEVICE.h"
#include "APP_DEVICE_AudioManager.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "APP_MDI_C1.id"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_MDI.h"
#include "APP_MDI_AT.h"
#include "APP_MDI_CI.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/**
\brief	variable to hold module global data
*/
AppMdiContext gAppMdiContext;
AppMdiJobType gAppMdiJobTable[APP_MDI_JOB_MAX];
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
/* Function    : APP_MDI_sfInitApp                                         */
/**
    \brief      Callback function for initialization of App MDI

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfInitApp(void)
{
    (void)memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
    (void)memset(&gAppMdiJobTable, 0, sizeof(gAppMdiJobTable));

    gAppMdiContext.dev_meta_type        = APP_MDI_INIT;
    gAppMdiContext.play_status          = APP_MDI_ACT_STATE_INIT;
    gAppMdiContext.play_status_at_sent  = AT_MDB_NONE;
    gAppMdiContext.random               = MDI_RANDOM_DISABLE;
    gAppMdiContext.repeat               = MDI_REPEAT_DISABLE;
    gAppMdiContext.enOldRequest         = APP_MDI_DEFER_INIT;
    gAppMdiContext.trackTime            = APP_MDI_TOTAL_TRACK_TIME_INVALID;
    gAppMdiContext.timePosition         = APP_MDI_TOTAL_TRACK_TIME_INVALID;
    gAppMdiContext.avrcp_device_id      = AT_INVALID_DEVICE_ID;
    gAppMdiContext.audioSourceLockLocal = 0xFF; // UHV starts with ALCK=1
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_sfInitApp                                         */
/**
    \brief      Callback function for initialization of App MDI

    \return     SuccessFailType
                Status of function run
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfReInitApp(void)
{
    (void)memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
    (void)memset(&gAppMdiJobTable, 0, sizeof(gAppMdiJobTable));

    gAppMdiContext.dev_meta_type        = APP_MDI_INIT;
    gAppMdiContext.random               = MDI_RANDOM_DISABLE;
    gAppMdiContext.repeat               = MDI_REPEAT_DISABLE;
    gAppMdiContext.play_status          = APP_MDI_ACT_STATE_INIT;
    gAppMdiContext.play_status_at_sent  = AT_MDB_NONE;
    gAppMdiContext.enOldRequest         = APP_MDI_DEFER_INIT;
    gAppMdiContext.trackTime            = APP_MDI_TOTAL_TRACK_TIME_INVALID;
    gAppMdiContext.timePosition         = APP_MDI_TOTAL_TRACK_TIME_INVALID;
    gAppMdiContext.avrcp_device_id      = AT_INVALID_DEVICE_ID;
    gAppMdiContext.audioSourceLockLocal = 0xFF;
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_vHandleEvents                                        */
/**
    \brief      Task Function to handle events for MDI Application

    \brief      Description:\n
                Waits for incoming events and calls the corresponding handler \n
                after a known event arrived. \n

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_vHandleEvents(EventMaskType event)
{
    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************

    // remove event evAppTaskMdiATResponse and message msgMdiATResponse
    // from APP_PHONE_vHandleEvents to central AT-handling in
    // APP_COMMON_vHandleEvents

    if( event & evAppTaskMdiFctPending )
    {
        (void)ClearEvent( evAppTaskMdiFctPending );


        if(gAppMdiContext.fct.common.id == APP_MDI_JOB_INVALID)
        {
            if(_APP_MDI_sfGetNextJob(&gAppMdiContext.fct) == SUCCESS)
            {
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleEvents: next job %d",
                              gAppMdiContext.fct.common.id);


                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(0), 0);
            }
            else
            {
                DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleEvents: no more jobs avail");
            }
        }
        else
        {
            if(ERM_bIsStartupDone() == BT_TRUE ||
                    gAppMdiContext.bPauseMdiPoweroffCK5050 == BT_TRUE)//RT14519
            {
                // try to get ressource
                const SCI_ST * puart = ERM_GetUartForMDI();

                if( !puart )
                {
                    // ressource is busy, set an alarm to try again in some cycles
                    // set event once again after 10 ms
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING,
                                                        MSEC(ERM_UART_BUSY_RETRY_TIME),
                                                        0);
                }
                else
                {
                    // call common function to handle next step
                    DEBUG_VALUE1( APP_MDI_SWI_DBG, MOD_APP_MDI, "HandleEvents: id %d pending",
                                  gAppMdiContext.fct.common.id);

                    (void)_APP_MDI_sfHandlePendingFunction(puart);
                }
            }
            else
            {
                /* we are not able to to request the UART in this state */
                DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "HandleEvents: CK5050_SHUTDOWN, drop pending, fct %d",
                        gAppMdiContext.fct.common.id);
                _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_bIsInitialized                                        */
/**
  \brief        Indicates whether the mdi module is available
  \brief        Description:\n
                The value is true, if the ERM has signalized that the CK5050 is \n
                ready to operate.

  \return       BooleanType
                BT_TRUE, BT_FALSE

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bIsInitialized(void)
{
    return ERM_bIsStartupDone();
}

/*----------------------------------------------------------------------------*/
/* Function :                                      */
/**
  \brief        Indicates whether the player is in state play

  \return       BooleanType
                BT_TRUE if the player is in state play
                BT_FALSE if the player is not in state play

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bPlayerActive(void)
{
    BooleanType result = BT_TRUE;

    switch(gAppMdiContext.play_status)
    {
        case APP_MDI_ACT_STATE_PAUSE:
        case APP_MDI_ACT_STATE_STOP:
        case APP_MDI_ACT_STATE_INIT:
            result = BT_FALSE;
            break;
        default:
            result = BT_TRUE;
            break;
    }

    DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "bPlayerAct: %d, %d",
                  result, gAppMdiContext.play_status);

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_bPlayerAvailable                                      */
/**
  \brief        Indicates whether a player is currently connected or not

  \return       BooleanType
                BT_TRUE if a player is connected
                BT_FALSE otherwise

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bPlayerAvailable(void)
{
    if(ERM_u8GetActivePlayer() == AT_INVALID_DEVICE_ID)
    {
        return BT_FALSE;
    }
    else
    {
        return BT_TRUE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :                                      */
/**
  \brief        Indicates whether the connected player supports AVRCP or not

  \return       BooleanType
                BT_TRUE if the connected player supports AVRCP
                BT_FALSE if the connected player doesn't support AVRCP or no\n
                         player is connected
 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bRemoteControlAvailable(void)
{
    AT_DeviceId deviceId = ERM_u8GetActivePlayer();
    const ERMBTDeviceTableType *pDevice = ERM_GetBTDevice(deviceId);

    if( (pDevice != NULL) &&
        (pDevice->avrcp_active == AT_TRUE) )
    {
        return BT_TRUE;
    }
    else
    {
        return BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetTrackTime                                          */
/**
  \brief        Function to retrieve the track time

  \return       Uint32Type
                Track time in ms

 */
/*----------------------------------------------------------------------------*/
Uint32Type APP_MDI_GetTrackTime(void)
{
    return gAppMdiContext.trackTime;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetTrackTimePosition                                  */
/**
  \brief        Function to retrieve the track time position

  \return       Uint32Type
                Track time position in ms

 */
/*----------------------------------------------------------------------------*/
Uint32Type APP_MDI_GetTrackTimePosition(void)
{
    return gAppMdiContext.timePosition;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetPlayStatus                                         */
/**
    \brief      Returns the current PlayStatus

    \return     APP_MDI_enActiveStatusValue

*/
/*----------------------------------------------------------------------------*/
APP_MDI_enActiveStatusValue APP_MDI_GetPlayStatus( void )
{
    return gAppMdiContext.play_status;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetSavePauseState                                     */
/**
    \brief      Returns the save Pause State

    \return     APP_MDI_enActiveStatusValue

*/
/*----------------------------------------------------------------------------*/
APP_MDI_enActiveStatusValue APP_MDI_GetSavePauseState( void )
{
    return gAppMdiContext.play_status;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetRandomMode                                         */
/**
    \brief      Returns the current RandomMode

    \return     APP_MDI_enRandomMode

*/
/*----------------------------------------------------------------------------*/
APP_MDI_enRandomMode APP_MDI_GetRandomMode(void)
{
    return gAppMdiContext.random;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetMetadataType                                         */
/**
    \brief      Returns the used metadata type of current player

    \return     APP_MDI_enMetaDataType

*/
/*----------------------------------------------------------------------------*/
APP_MDI_enMetaDataType APP_MDI_GetMetadataType(void)
{
    return gAppMdiContext.dev_meta_type;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_enRepeatMode                                         */
/**
    \brief      Returns the current RepeatMode

    \return     APP_MDI_enRepeatMode

*/
/*----------------------------------------------------------------------------*/
APP_MDI_enRepeatMode APP_MDI_GetRepeatMode(void)
{
    return gAppMdiContext.repeat;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetFileName                                             */
/**
    \brief      Returns the current File name

    \return     const AppMdiFileNameDataType*

*/
/*----------------------------------------------------------------------------*/
const AppMdiFileNameDataType* APP_MDI_GetFileName( void )
{
    return &(gAppMdiContext.fileName);
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetArtist                                             */
/**
    \brief      Returns the current Artist name

    \return     const AppMdiFileInfoDataType*

*/
/*----------------------------------------------------------------------------*/
const AppMdiFileInfoDataType* APP_MDI_GetArtist( void )
{
    return &(gAppMdiContext.artist);
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetTitle                                             */
/**
    \brief      Returns the current Title name

    \return     const AppMdiFileInfoDataType*

*/
/*----------------------------------------------------------------------------*/
const AppMdiFileInfoDataType* APP_MDI_GetTitle( void )
{
    return &(gAppMdiContext.title);
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_GetAlbum                                             */
/**
    \brief      Returns the current Album name

    \return     const AppMdiFileInfoDataType*

*/
/*----------------------------------------------------------------------------*/
const AppMdiFileInfoDataType* APP_MDI_GetAlbum( void )
{
    return &(gAppMdiContext.album);
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_GetActiveServiceDeviceId                             */
/**
    \brief      Function to retrieve the device where the specified service is active

    \param      service
                Id of the service

    \return     AT_DeviceId
                Device id if a device has the service connected

*/
/*----------------------------------------------------------------------------*/
AT_DeviceId APP_MDI_GetActiveServiceDeviceId( ATBTServiceEnum service)
{
    const ERMBTDeviceTableType *pBtDevice = NULL;
    AT_DeviceId                 deviceId  = AT_INVALID_DEVICE_ID;
    Sint32Type                  i         = 0;

    for(i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        pBtDevice = ERM_GetBTDevice(i);

        if(pBtDevice == NULL)
        {
            continue;
        }

        if(service == AT_SERVICE_AVRCP)
        {
            if( pBtDevice->avrcp_active == AT_TRUE )
            {
                deviceId =(Uint8Type) i;
            }
        }
        else if( service == AT_SERVICE_A2DP_SINK )
        {
            if( pBtDevice->a2dp_sink_active == AT_TRUE )
            {
                deviceId =(Uint8Type) i;
            }
        }
        else
        {
            /* other service id's will be ignored */
        }
    }

    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_vNotifyApps                                          */
/**
  \brief        Notifies other application

  \param        event
                APP_MDI_enNotificationId

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _APP_MDI_vNotifyApps(APP_MDI_enNotificationId event)
{
    if(event == APP_MDI_PLAY_STATUS_CHANGED)
    {
        switch( gAppMdiContext.play_status)
        {
            case APP_MDI_ACT_STATE_PLAY:
                if(APP_DEVICE_AM_sfGetMute(NULL, APP_CAN_enAudioStream, __LINE__ ) != SUCCESS)
                {
                    DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED): unmuted while VCR active, wait for mute from HU");
                    APP_MDI_sfSetPlayMode( APP_MDI_PAUSE );
                }
                break;
            case APP_MDI_ACT_STATE_PAUSE:
            case APP_MDI_ACT_STATE_STOP:
            case APP_MDI_ACT_STATE_INIT:
                APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enAudioStream, __LINE__);
                break;
            default:
                /* otherwise ignore the message */
                break;
        }
    }

    APP_MDI_BAP_vOnAppMdiNotification(event);
    ERM_vOnAppMdiNotification(event);
}




/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_vOnAppDeviceNotification                              */
/**
  \brief        This function handles notifications coming from AppDevice

  \param        notification
                APP_DEVICE_enNotificationId

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_DEVICE_NOTIFY_PLAYER_PROFILE_ADDED:
        {
            /* code moved to APP_DEVICE_vHandlePlayerProfileAdded */

            break;
        }
    }

}


/* ******************************************************************************** */
/* NEW JOB HANDLING BELOW                                                           */




/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfSetALCKMode                                       */
/**
    \brief      set the audio source for media player locked

    \param      lockState
                MDI_AUDIO_LOCK_STOP
                MDI_AUDIO_LOCK_START

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetALCKMode(APP_MDI_enAudioLock lockState)
{
    BooleanType bPlayerCoding = BT_TRUE;     /* player coding by diagnosis */
    AppMdiJobType newJob;

    newJob.common.id        = APP_MDI_JOB_ALCK;
    newJob.alck.lockType    = (lockState == MDI_AUDIO_LOCK_START) ?
                                    AT_ALCK_LOCK_START : AT_ALCK_LOCK_STOP;


    /* coding must be checked that the player can't play, if not coded, eg. over Handsfree */

    /* if the function fails, Multimedia is coded as default */
    EEPROM_LAYOUT_vGetPlayerEnabled(&bPlayerCoding);

    if(bPlayerCoding != BT_TRUE)
    {
        /* Multimedia is NOT coded, nothing to do */
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetALCKMode: ignore, no MP coded" );
        return SUCCESS;
    }

    AppTask_vClearAlarm(APP_TASK_ALARM_MDI_UNLOCK_AUDIO);

    if(gAppMdiContext.audioSourceLockLocal != lockState)
    {
        _APP_MDI_vJobManager( &newJob );
        gAppMdiContext.audioSourceLockLocal = lockState;
    }
    else
    {
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "SetALCKMode: ALCK mode not changed, actual ALCK is %d",
                    gAppMdiContext.audioSourceLockLocal);
    }
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfVcrSetPlayMode                                      */
/**
    \brief      set the disco player status for media player in the CK5050

    \param      cb
                Callback to notify the caller after finishing

    \param      discoMode
                MDI_RANDOM_DISABLE
                MDI_RANDOM_ENABLE

    \return     SUCCESS
                FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfVcrSetPlayMode( APP_MDI_enDiscoStatusValue discoMode )
{
    SuccessFailType result = SUCCESS;

    switch(discoMode)
    {
        case APP_MDI_STOP:
        case APP_MDI_PLAY:
        case APP_MDI_PAUSE:
        case APP_MDI_RESUME:
            DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "VcrSetPlayMode: set unmute_request %d -> %d",
                          gAppMdiContext.unmute_request,
                          discoMode );
            gAppMdiContext.unmute_request = discoMode;
            break;
        case APP_MDI_NEXT_SONG:
        case APP_MDI_PREVIOUS_SONG:
            /* send this play modes directly */
            result = APP_MDI_sfSetPlayMode( discoMode );
            break;
        default:
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "VcrSetPlayMode: unexpected discoMode %d",
                          discoMode );
            result = FAIL;
            break;
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfSetPlayMode                                       */
/**
    \brief      set the disco player status for media player in the CK5050

    \param      cb
                Callback to notify the caller after finishing

    \param      discoMode
                MDI_RANDOM_DISABLE
                MDI_RANDOM_ENABLE

    \return     SUCCESS
                FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetPlayMode( APP_MDI_enDiscoStatusValue discoMode )
{
    AppMdiJobType newJob;
    BooleanType SendDPLY = BT_FALSE;

    newJob.common.id        = APP_MDI_JOB_DPLY;

    if(gAppMdiContext.avrcp_device_id == AT_INVALID_DEVICE_ID)
    {
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetPlayMode: no avrcp" );

        return FAIL;
    }

    switch(discoMode)
    {
        case APP_MDI_PLAY:
            if(gAppMdiContext.play_status != APP_MDI_ACT_STATE_PLAY)
            {
                gAppMdiContext.play_status_at_sent = AT_MDB_START_PLAYING;
                newJob.dply.playModeAt = AT_MDB_START_PLAYING;
                newJob.dply.mute = APP_MDI_MUTE_GET;
                SendDPLY = BT_TRUE;
                gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(5000), APP_MDI_cbDPLYResponseTimeOut);
            }
            break;
        case APP_MDI_RESUME:
            if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_PLAY)||
               (gAppMdiContext.bSendRelesaButtonAfterSeek == BT_TRUE))
            {
                gAppMdiContext.play_status_at_sent = AT_MDB_RESUME_PLAYING;
                newJob.dply.playModeAt = AT_MDB_RESUME_PLAYING;
                newJob.dply.mute = APP_MDI_MUTE_GET;
                SendDPLY = BT_TRUE;
                if(gAppMdiContext.bSendRelesaButtonAfterSeek == BT_TRUE)
                {
                    gAppMdiContext.bSendRelesaButtonAfterSeek = BT_FALSE;

                    gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                    AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(500), APP_MDI_cbDPLYResponseTimeOut);
                }
                else
                {
                    gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                    AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(5000), APP_MDI_cbDPLYResponseTimeOut);
                }
            }
            break;
        case APP_MDI_STOP:
            if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_STOP)
              &&(gAppMdiContext.play_status != APP_MDI_ACT_STATE_PAUSE))
            {
                if( gAppMdiContext.play_status == APP_MDI_ACT_STATE_PAUSE)
                {
                    // After changing from PAUSE to STOP or vis versa no PKEV and
                    // no DGPS will be happen, thus no play status will be updated.
                    // There for the play status must be changed here correctly
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_STOP;
                    APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
                }
                else
                {
                    gAppMdiContext.play_status_at_sent = AT_MDB_STOP_PLAYING;
                    newJob.dply.playModeAt = AT_MDB_PAUSE_PLAYING;
                    newJob.dply.mute = APP_MDI_MUTE_RELEASE;
                    SendDPLY = BT_TRUE;
                    gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                    AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(5000), APP_MDI_cbDPLYResponseTimeOut);
                }
            }
            break;
        case APP_MDI_PAUSE:
            if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_STOP)
              &&(gAppMdiContext.play_status != APP_MDI_ACT_STATE_PAUSE))
            {
                if( gAppMdiContext.play_status == APP_MDI_ACT_STATE_STOP)
                {
                    // After changing from PAUSE to STOP or vis versa no PKEV and
                    // no DGPS will be happen, thus no play status will be updated.
                    // There for the play status must be changed here correctly
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_PAUSE;
                    APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
                }
                else
                {
                    gAppMdiContext.play_status_at_sent = AT_MDB_PAUSE_PLAYING;
                    newJob.dply.playModeAt = AT_MDB_PAUSE_PLAYING;
                    newJob.dply.mute = APP_MDI_MUTE_RELEASE;
                    SendDPLY = BT_TRUE;
                    gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                    AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(5000), APP_MDI_cbDPLYResponseTimeOut);
                }
            }
            break;
        case APP_MDI_NEXT_SONG:
            gAppMdiContext.play_status_at_sent = AT_MDB_NEXT_SONG;
            newJob.dply.playModeAt = AT_MDB_NEXT_SONG;
            newJob.dply.mute = APP_MDI_MUTE_NO_ACTION;
            SendDPLY = BT_TRUE;
            break;
        case APP_MDI_PREVIOUS_SONG:
            gAppMdiContext.play_status_at_sent = AT_MDB_PREVIOUS_SONG;
            newJob.dply.playModeAt = AT_MDB_PREVIOUS_SONG;
            newJob.dply.mute = APP_MDI_MUTE_NO_ACTION;
            SendDPLY = BT_TRUE;
            break;
        case APP_MDI_FAST_FORWARD:
            gAppMdiContext.play_status_at_sent = AT_MDB_F_FORWARD_PLAYING;
            newJob.dply.playModeAt = AT_MDB_F_FORWARD_PLAYING;
            newJob.dply.mute = APP_MDI_MUTE_NO_ACTION;
            SendDPLY = BT_TRUE;
            gAppMdiContext.bSendRelesaButtonAfterSeek = BT_TRUE;
            break;
        case APP_MDI_FAST_BACKWARD:
            gAppMdiContext.play_status_at_sent = AT_MDB_F_REWIND_PLAYING;
            newJob.dply.playModeAt = AT_MDB_F_REWIND_PLAYING;
            newJob.dply.mute = APP_MDI_MUTE_NO_ACTION;
            SendDPLY = BT_TRUE;
            gAppMdiContext.bSendRelesaButtonAfterSeek = BT_TRUE;
            break;
        default:
            // return an error
             DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetPlayMode: unsup disco %d",
                           discoMode );
             return FAIL;
    }

    if(BT_TRUE == SendDPLY)
    {
        DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "SetPlayMode: set %d -> %d",
                gAppMdiContext.play_status_at_sent, newJob.dply.playModeAt );

        _APP_MDI_vJobManager( &newJob );

        gAppMdiContext.retry_counter = 0;
        gAppMdiContext.playstate_consistent = BT_TRUE;
    }
    else
    {
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "SetPlayMode: status not changed, %d", gAppMdiContext.play_status_at_sent );
        gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
    }

    /* stop alarm always */
    AppTask_vClearAlarm(APP_TASK_ALARM_MDI_PLAYSTATE);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfSetSkip                                       */
/**
    \brief      set the disco player status for media player in the CK5050

    \param      cb
                Callback to notify the caller after finishing

    \param      discoMode
                MDI_RANDOM_DISABLE
                MDI_RANDOM_ENABLE

    \return     SUCCESS
                FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetSkip( APP_MDI_enDiscoStatusValue discoMode, Uint8Type counter)
{
    AppMdiJobType newJob;

    newJob.common.id        = APP_MDI_JOB_SKIP;
    newJob.skip.counter     = counter;

    if(gAppMdiContext.avrcp_device_id == AT_INVALID_DEVICE_ID)
    {
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetPlayMode: no avrcp" );

        return FAIL;
    }

    switch(discoMode)
    {
        case APP_MDI_NEXT_SONG:
            newJob.skip.skipModeAt = AT_MDB_NEXT_SONG;
            break;
        case APP_MDI_PREVIOUS_SONG:
            newJob.skip.skipModeAt = AT_MDB_PREVIOUS_SONG;
            break;
        default:
            // return an error
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetSkip: unsup skip %d",
                          discoMode );
            return FAIL;
    }

    DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "SetSkip: set %d -> %d",
                  discoMode, newJob.skip.skipModeAt );

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfGetPgmt                                             */
/**
    \brief      Used to request the supported meta data type of the connected player

    \param      deviceId
                Get Pgmt for that device


    \return     SuccessFailType
                SUCCESS
                FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfGetPgmt(AT_DeviceId deviceId)
{
    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_PGMT;
    newJob.pgmt.deviceId    = deviceId;

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_sfGetDged                                           */
/**
    \brief      Activates current file info and current file name information

    \param      deviceId
                Enable DGED for that device

    \return     SuccessFailType
                SUCCCES
                FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfGetDged(AT_DeviceId deviceId, Uint8Type fieldmask)
{
    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_DGED;
    newJob.dged.deviceId    = deviceId;
    newJob.dged.fieldmask   = fieldmask;

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}   /* END OF APP_MDI_sfGetDged()                                             */

/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_sfGetDgps                                           */
/**
    \brief      Requests track information

    \param      deviceId
                Request DGPS for that device

    \return     SuccessFailType
                SUCCCES
                FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfGetDgps(AT_DeviceId deviceId)
{
    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_DGPS;
    newJob.dgps.deviceId    = deviceId;

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}   /* END OF APP_MDI_sfGetDgps()                                             */



/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfSetRandomMode                                       */
/**
    \brief      Set the random mode in the CK5050

    \param      randomMode
                MDI_RANDOM_DISABLE
                MDI_RANDOM_ENABLE

    \return     SuccessFailType
                SUCCCES
                FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetRandomMode(APP_MDI_enRandomMode randomMode)
{
    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_DSRA;

    switch (randomMode)
    {
        case MDI_RANDOM_DISABLE:
        {
            newJob.dsra.RandomMode = AT_DEVRANDOM_DISABLE;
            break;
        }
        case MDI_RANDOM_ENABLE:
        {
            newJob.dsra.RandomMode = AT_DEVRANDOM_ENABLE;
            break;
        }
        case MDI_RANDOM_FOLDER_PLAYLISTS:
        {
            newJob.dsra.RandomMode = AT_DEVRANDOM_ENABLE_ALBUM;
            break;
        }
        default:
        {
            DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetRandomMode: default: not supported");
            return FAIL;
        }
    }

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfSetRepeatMode                                       */
/**
    \brief      Set the repeat mode in the CK5050

    \param      repeatMode
                MDI_REPEAT_ONE_TRACK
                MDI_REPEAT_ALL_TRACK
                MDI_REPEAT_DISABLE

    \return     SuccessFailType
                SUCCCES
                FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetRepeatMode(APP_MDI_enRepeatMode repeatMode)
{

    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_DSRE;

    switch(repeatMode)
    {
        case MDI_REPEAT_DISABLE:
            newJob.dsre.repeatMode = AT_PLAYER_REPEAT_DISABLED;
            break;
        case MDI_REPEAT_ONE_TRACK:
            newJob.dsre.repeatMode = AT_PLAYER_REPEAR_ONETRACK;
            break;
        case MDI_REPEAT_ALL_TRACK:
            newJob.dsre.repeatMode = AT_PLAYER_REPEAR_ALLTRACK;
            break;
        case MDI_REPEAT_GROUP:
            newJob.dsre.repeatMode = AT_PLAYER_REPEAR_GROUP;
            break;
        default:
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "sfSetRepeatMode: inval mode %d",
                          repeatMode );
            return FAIL;
    }

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_sfSetAutomaticNotificationMode                         */
/**
  \brief

                This command is used by the HOST to configure if the CK must
                send to the Host the event track position automatically. If
                activated, HOST will receive an unsollicited DGPS event each
                second. This command is only available for device which support AVRCP 1.3.

    \param      autoNotMode
                MDI_AUTO_NOT_DISABLE
                MDI_AUTO_NOT_ENABLE

    \return     SuccessFailType
                SUCCCES
                FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetAutomaticNotificationMode(APP_MDI_enAutoNotMode autoNotMode)
{
    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_DCPN;

    switch(gAppMdiContext.dev_meta_type)
    {
        case APP_MDI_AVRCP_1_3:
        case APP_MDI_AVRCP_1_4:
        {
            newJob.dcpn.notifyMode = (autoNotMode == MDI_AUTO_NOT_ENABLE) ?
                                        AT_AUTOMATIC_NOTIFICATION_DISABLED : AT_AUTOMATIC_NOTIFICATION_ENABLED;
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "SetAutoNoti: inval meta_type %d",
                          gAppMdiContext.dev_meta_type );
            return FAIL;
    }

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_rcSetPlayerEnabled                                   */
/**
  \brief        Diagnostic interface to enable/disable the MDI feature
  \brief        Description:\n

  \param        cb
                Callback to notify the caller after finishing

  \param        isEnabled
                Specifies whether the feature is enabled (BT_TRUE) or not (BT_FALSE)

    \return     SuccessFailType
                SUCCCES
                FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_sfSetPlayerVisible( BooleanType setVisible )
{
    AppMdiJobType newJob;
    newJob.common.id        = APP_MDI_JOB_PBSV;

    /* we start with A2DP SINK, the service A2DP SOURCE is set after received AT*OK */
    newJob.pbsv.service = AT_SERVICE_A2DP_SINK;
    newJob.pbsv.action  = (setVisible == BT_TRUE) ?
                            AT_PBSV_ACT_ENABLE_VISIBILTY : AT_PBSV_ACT_DISABLE_VISIBILTY;

    _APP_MDI_vJobManager( &newJob );

    return SUCCESS;
}   /* END OF APP_MDI_sfSetPlayerVisible()                                    */

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_vMutePlayer                                           */
/**
  \brief        Function to mute the player

  \return       BooleanType
                BT_TRUE in the case that the playback was paused
                BT_FALSE otherwise

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bMutePlayer(APP_MDI_enMuteReason muteReason)
{
    BooleanType result = BT_FALSE;

    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "bMutePlayer: reason %d", muteReason );
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "bMutePlayer: plyer mute value %d", gAppMdiContext.player_mute );//RT14519

    if(gAppMdiContext.player_mute == 0)
    {
        switch(gAppMdiContext.play_status)
        {
            case APP_MDI_ACT_STATE_STOP:
                gAppMdiContext.unmute_request = APP_MDI_STOP;
                break;
            case APP_MDI_ACT_STATE_PAUSE:
                gAppMdiContext.unmute_request = APP_MDI_PAUSE;
                break;
            case APP_MDI_ACT_STATE_PLAY:
            case APP_MDI_ACT_STATE_FAST_FORWARD:
            case APP_MDI_ACT_STATE_FAST_REWIND:
                gAppMdiContext.unmute_request = APP_MDI_RESUME;
                result = BT_TRUE;
                break;
            default:
                DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "bMutePlayer: unknown play_status %d",
                              gAppMdiContext.play_status );
                gAppMdiContext.unmute_request = APP_MDI_PAUSE;
                break;
        }

        DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "bMutePlayer: play_status %d -> unmute_request %d",
                      gAppMdiContext.play_status,
                      gAppMdiContext.unmute_request );


        /* because no PKEV will be received after ALCK=1, force the play status to be equal to the sent one */
        if((IsCK5050ResponseStillOngoing()) && (AT_MDB_RESUME_PLAYING == gAppMdiContext.play_status_at_sent))
        {
            gAppMdiContext.unmute_request = APP_MDI_RESUME;
            gAppMdiContext.play_status = APP_MDI_ACT_STATE_PLAY;

            /* reset the response outstanding flag since no response will be received after ALCK is set */
            gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
        }

        if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_PAUSE) &&
           (gAppMdiContext.play_status != APP_MDI_ACT_STATE_STOP))
        {
            /* change the media player state before muting only if not stopped or paused already*/
            if( APP_MDI_sfSetPlayMode( APP_MDI_PAUSE ) != SUCCESS)
            {
                DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "bMutePlayer: SetPlayMode( APP_MDI_PAUSE ) FAILED");
            }

            /* force the pause to the HU to avoid BAP indication "PlayStatus"
             * A problem occurs with some phone when the PKEV event is received later so the HU will
             * enforce the pause state from its site */

            gAppMdiContext.play_status = APP_MDI_ACT_STATE_PAUSE;
            APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
        }

        if(APP_MDI_sfSetALCKMode( MDI_AUDIO_LOCK_START ) != SUCCESS)
        {
            DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "bMutePlayer: APP_MDI_sfSetALCKMode( MDI_AUDIO_LOCK_START ) FAILED");
        }
    }
    gAppMdiContext.player_mute |= 1<<muteReason;

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_vUnmutePlayer                                         */
/**
  \brief        Function to mute the player

  \return       BooleanType
                BT_TRUE in the case that the player is restarted
                BT_FALSE otherwise

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bUnmutePlayer(APP_MDI_enMuteReason unmuteReason, TickType DelayCommandTime)
{
    BooleanType result = BT_FALSE;

    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "bUnmutePlayer: reason %d", unmuteReason );

    if(gAppMdiContext.player_mute == 0)
    {
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "bUnmutePlayer: called w/o mute -> ignored");
        return result;
    }

    if(gAppMdiContext.player_mute & 1<<unmuteReason)
    {
        gAppMdiContext.player_mute &= ~(1<<unmuteReason);
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "bUnmutePlayer: cleared mute %d", unmuteReason );
    }

    if(gAppMdiContext.player_mute == 0)
    {
        switch(gAppMdiContext.unmute_request)
        {
            case APP_MDI_RESUME:
            //case APP_MDI_FAST_FORWARD:
            //case APP_MDI_FAST_BACKWARD:
                result = BT_TRUE;
                break;
            default:
                result = BT_FALSE;
                break;
        }

        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "bUnmutePlayer: unmute_request %d", gAppMdiContext.unmute_request );

        /* change the media player state after unmuting only if necessary*/
        if( (gAppMdiContext.unmute_request != APP_MDI_PAUSE) &&
            (gAppMdiContext.unmute_request != APP_MDI_STOP) )
        {
            if((DelayCommandTime >0 )&&(ERM_u8GetActivePhone() == ERM_u8GetActivePlayer())&&(ERM_u8GetActivePhone() != AT_INVALID_DEVICE_ID))
            {/* delay play command only if different physical devices for phone and player */
			 /* changed on 12.04.2011 by nikolov, RT:10389 */
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Unmute the MP after %d ms", DelayCommandTime);

                /* force the play to the HU to avoid BAP indication for "PlayStatus"
                 * A problem occurs with HUs when after releasing the mute for phone
                 * If play command is postponed with 2 seconds inform the HU with play status */
                gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                APP_MDI_vDeferPlayMode(APP_MDI_RESUME);

                /* start the play back with delay(some phones synchronize the PB and call stacks directly after phone call) */
                /* in this case the phone does not execute the play command, it just neglects it and streaming is not started*/
                AppTask_vSetAlarm( APP_TASK_ALARM_MDI_DELAY_PLAY_COMMAND_AFTER_CALL,
                                   MSEC(DelayCommandTime),
                                   APP_MDI_cbSendDPLYDelayed);
            }
            else
            {
                /* start the streaming immediately after de-muting(e.g. after VCR or jingle tone, device search and so on)*/
                DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "Unmute the MP immediately");

                if( APP_MDI_sfSetPlayMode( gAppMdiContext.unmute_request ) != SUCCESS)
                {
                    DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "bUnmutePlayer: SetPlayMode( %d ) FAILED",
                                  gAppMdiContext.unmute_request);
                }
            }
        }

        if( APP_MDI_sfSetALCKMode( MDI_AUDIO_LOCK_STOP ) != SUCCESS )
        {
            DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "bUnmutePlayer: APP_MDI_sfSetALCKMode( MDI_AUDIO_LOCK_STOP ) FAILED");
        }
    }
    else
    {
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "bUnmutePlayer: mute still set %02X", gAppMdiContext.player_mute );
    }


    return result;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_bUnlockPlayer                                         */
/**
  \brief        Function to unlock the player

  \return       BooleanType
                BT_TRUE in the case that the player is restarted
                BT_FALSE otherwise

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bUnlockPlayer(void)
{
    DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "bUnlockPlayer: unlock the player!");
    if( APP_MDI_sfSetALCKMode( MDI_AUDIO_LOCK_STOP ) != SUCCESS )
    {
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "bUnmutePlayer: APP_MDI_sfSetALCKMode( MDI_AUDIO_LOCK_STOP ) FAILED");
        return BT_FALSE;
    }
    return BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_cbPlayStateTimeout                                    */
/**
  \brief        Alarm callback to retrigger a DGPS update

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_cbPlayStateTimeout()
{
    BooleanType bRetry = BT_FALSE;

    if( gAppMdiContext.retry_counter >= MAX_PLAY_STATE_RETRY_COUNTER )
    {
        DEBUG_VALUE2( APP_MDI_SWI_WNG, MOD_APP_MDI, "cbPlayStateTimeout: max retry exceeded %d,%d",
                      gAppMdiContext.play_status,
                      gAppMdiContext.play_status_at_sent );

        gAppMdiContext.playstate_consistent = BT_TRUE;

        return;
    }

    switch(gAppMdiContext.play_status_at_sent)
    {
        case AT_MDB_STOP_PLAYING:
            if(gAppMdiContext.play_status != APP_MDI_ACT_STATE_STOP)
            {
                bRetry = BT_TRUE;
            }
            break;
        case AT_MDB_PAUSE_PLAYING:
            if(gAppMdiContext.play_status != APP_MDI_ACT_STATE_PAUSE)
            {
                bRetry = BT_TRUE;
            }
            break;
        case AT_MDB_START_PLAYING:
        case AT_MDB_RESUME_PLAYING:
            if(gAppMdiContext.play_status != APP_MDI_ACT_STATE_PLAY)
            {
                bRetry = BT_TRUE;
            }
            break;
        case AT_MDB_F_FORWARD_PLAYING:
            if(gAppMdiContext.play_status != APP_MDI_ACT_STATE_FAST_FORWARD)
            {
                bRetry = BT_TRUE;
            }
            break;
        case AT_MDB_F_REWIND_PLAYING:
            if(gAppMdiContext.play_status != APP_MDI_ACT_STATE_FAST_REWIND)
            {
                bRetry = BT_TRUE;
            }
            break;
        case AT_MDB_NEXT_SONG:
        case AT_MDB_PREVIOUS_SONG:
        default:
            /* do nothing */
            break;
    }

    if(bRetry == BT_TRUE)
    {
        AppMdiJobType newJob;
        newJob.common.id        = APP_MDI_JOB_DPLY;
        newJob.dply.playModeAt  = gAppMdiContext.play_status_at_sent;
        if( gAppMdiContext.play_status_at_sent == AT_MDB_STOP_PLAYING )
        {
            newJob.dply.playModeAt = AT_MDB_PAUSE_PLAYING;
        }

        DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbPlayStateTimeout: retry %d,%d",
                      gAppMdiContext.play_status,
                      gAppMdiContext.play_status_at_sent );

        gAppMdiContext.retry_counter++;

        _APP_MDI_vJobManager( &newJob );
    }
    else
    {
        DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbPlayStateTimeout: NO retry %d,%d",
                      gAppMdiContext.play_status,
                      gAppMdiContext.play_status_at_sent );

        gAppMdiContext.playstate_consistent = BT_TRUE;
    }
}




/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_cbStartupUnlockAudi                                   */
/**
  \brief        Alarm callback to unlock the audio source

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_cbStartupUnlockAudio()
{
    DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbUnlockAudio: startup unlock" );

    if(APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_STOP) != SUCCESS)
    {

        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "cbUnlockAudio: SetALCKMode failed" );
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_cbGetPgmtDelayed                                    */
/**
  \brief        Alarm callback to delay the avrcp type request

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_cbGetPgmtDelayed()
{

    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbGetPgmtDelayed: dev %d",
                  gAppMdiContext.avrcp_device_id );

    if( gAppMdiContext.avrcp_device_id == AT_INVALID_DEVICE_ID ||
        APP_MDI_sfGetPgmt(gAppMdiContext.avrcp_device_id) != SUCCESS )
    {

        DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "cbGetPgmtDelayed: GetPgmt failed %d",
                      gAppMdiContext.avrcp_device_id );
    }
}
/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_cbSendDPLYDelayed                                     */
/**
  \brief        Alarm callback to delay the metadata request

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_cbSendDPLYDelayed(void)
{
    DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbSendDPLYDelayed called");

    if( APP_MDI_HandleDefer() != BT_FALSE)
    {
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "cbSendDPLYDelayed: Defer the play status after time out");
    }
    else
    {
        DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "cbSendDPLYDelayed: no play status defer, send play to the phone");
        APP_MDI_sfSetPlayMode(gAppMdiContext.unmute_request);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_cbDgpsRequestAlarm                                    */
/**
  \brief        Alarm callback to retrigger a DGPS update

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_cbGetDgps()
{
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbGetDgps: dev %d",
                  gAppMdiContext.avrcp_device_id );

    if( gAppMdiContext.avrcp_device_id == AT_INVALID_DEVICE_ID ||
        APP_MDI_sfGetDgps(gAppMdiContext.avrcp_device_id) != SUCCESS )
    {
        DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "cbGetDgps: GetDgps failed %d",
                      gAppMdiContext.avrcp_device_id );
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_vDeferPlayMode                                    */
/**
  \brief        Defers BAP playmode for AVRCP1.0 devices only
                Only Pause/Play will be supported

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_vDeferPlayMode(APP_MDI_enDiscoStatusValue discoMode)
{
    DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "Determine defer value");

    switch(discoMode)
    {
        case APP_MDI_PAUSE:
        {
            if( gAppMdiContext.defer_play_status == APP_MDI_DEFER_RESUME )
            {
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Clear defer %d", gAppMdiContext.defer_play_status );
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_INIT;
            }
            if (gAppMdiContext.play_status == APP_MDI_ACT_STATE_PAUSE)
            {
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_PAUSE;
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Defer %d", gAppMdiContext.defer_play_status );
            }
            /* needed for the start up behavior */
            if (gAppMdiContext.enOldRequest == APP_MDI_DEFER_INIT)
            {
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_PAUSE;
            }
            gAppMdiContext.enOldRequest = APP_MDI_DEFER_PAUSE;
            break;
        }

        case APP_MDI_RESUME:
        {
            if( gAppMdiContext.defer_play_status == APP_MDI_DEFER_PAUSE )
            {
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Clear defer %d", gAppMdiContext.defer_play_status );
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_INIT;
            }
            if (gAppMdiContext.play_status == APP_MDI_ACT_STATE_PLAY)
            {
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_RESUME;
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Defer %d", gAppMdiContext.defer_play_status );
            }
            /* needed for the start up behavior */
            if (gAppMdiContext.enOldRequest == APP_MDI_DEFER_INIT)
            {
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_RESUME;
            }
            gAppMdiContext.enOldRequest = APP_MDI_DEFER_RESUME;

            return;
        }

        case APP_MDI_FAST_FORWARD:
        {
            if ((gAppMdiContext.play_status == APP_MDI_ACT_STATE_PAUSE)||
                (gAppMdiContext.play_status == APP_MDI_ACT_STATE_PLAY))
            {
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_FAST_FORWARD;
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Defer %d", gAppMdiContext.defer_play_status );
            }
            break;
        }

        case APP_MDI_FAST_BACKWARD:
        {
            if ((gAppMdiContext.play_status == APP_MDI_ACT_STATE_PAUSE)||
               (gAppMdiContext.play_status == APP_MDI_ACT_STATE_PLAY))
            {
                gAppMdiContext.defer_play_status = APP_MDI_DEFER_FAST_BACKWARD;
                DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "Defer %d", gAppMdiContext.defer_play_status );
            }
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   IsCK5050ResponseStillOngoing                                  */
/**
  \brief        Function giving the state of the CK5050 response
                after Play/Pause command sent

  \return       void

 */
/*----------------------------------------------------------------------------*/
BooleanType IsCK5050ResponseStillOngoing(void)
{
    return gAppMdiContext.IsCK5050ResponseOngoing;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_cbDPLYResponseTimeOut                                 */
/**
  \brief        In case no response from CK5050 set the flag back to avoid blocking
                of the MOD_APP_MDI_BAP

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_cbDPLYResponseTimeOut(void)
{
    SuccessFailType result;
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "DPLYResponseTimeOut: No response to command %d", gAppMdiContext.play_status_at_sent);
    gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;

    /* set the sent command indicator to idle  => no further command is sent and user interaction on MP device */
    if(gAppMdiContext.play_status_at_sent != AT_MDB_NONE)
        gAppMdiContext.play_status_at_sent = AT_MDB_NONE;

    /* After discussing this with Parrot(RT #7667) if no response to DPLY for a defined amount of time
     * retrieve the PASA value to determine the player state to avoid inconsistency between CK and BAP */

    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "DPLYResponseTimeOut: determine PASA %d and send player state"
                 ,APP_DEVICE_GetPASAEvent());
    if((gAppMdiContext.play_status_at_sent != AT_MDB_F_FORWARD_PLAYING)||(gAppMdiContext.play_status_at_sent != AT_MDB_F_REWIND_PLAYING))
    {
        switch(APP_DEVICE_GetPASAEvent())
        {
        case AT_AUDIO_STATE_STREAMING_A2DP:

            if(APP_DEVICE_AMGetActiveSource() != APP_CAN_enAudioStream)
            {
                result = APP_DEVICE_AM_sfGetMute(NULL, APP_CAN_enAudioStream, __LINE__ );
                if(result == SUCCESS)
                {
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_PLAY;
                }
                else
                {
                    gAppMdiContext.play_status =APP_MDI_ACT_STATE_PAUSE;
                }
            }
            else
            {
                gAppMdiContext.play_status = APP_MDI_ACT_STATE_PLAY;
            }
            APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
            ERM_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
            break;
        default:
            gAppMdiContext.play_status =APP_MDI_ACT_STATE_PAUSE;
            APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
            ERM_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
            APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enAudioStream, __LINE__);
            break;
        }
    }
    /* afterwards handle the defer value in case there was user interaction from BAP */
    APP_MDI_HandleDefer();
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_HandleDefer                            */
/**
  \brief        Checks if the actual play status defers from the requested

  \param        void

  \return       BooleanType
                This parameter gives if defer state is to be hanldes or not
 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_HandleDefer(void)
{
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "sfHandlePKEV: Handle defer %d",
                 gAppMdiContext.defer_play_status);
    // Handle deferred play states
    if( gAppMdiContext.defer_play_status != APP_MDI_DEFER_INIT)
    {
        if( gAppMdiContext.defer_play_status == APP_MDI_DEFER_PAUSE)
        {
            APP_MDI_sfSetPlayMode(APP_MDI_PAUSE);
        }
        else
        if( gAppMdiContext.defer_play_status == APP_MDI_DEFER_RESUME)
        {
            APP_MDI_sfSetPlayMode(APP_MDI_RESUME);
        }
        else
        if(gAppMdiContext.defer_play_status == APP_MDI_DEFER_FAST_FORWARD)
        {
            APP_MDI_sfSetPlayMode(APP_MDI_FAST_FORWARD);
        }
        else
        if(gAppMdiContext.defer_play_status == APP_MDI_DEFER_FAST_BACKWARD)
        {
            APP_MDI_sfSetPlayMode(APP_MDI_FAST_BACKWARD);
        }
        else
        {
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "sfHandlePKEV: invalid defer %d",
                 gAppMdiContext.defer_play_status);
        }

        gAppMdiContext.defer_play_status = APP_MDI_DEFER_INIT;
        // Play state is going to old play state, so don't mute/demute
        return BT_TRUE;
    }
    else
    {
        return BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_bIsSeekingInProgress                                  */
/**
  \brief        Checks if the seeking(FF or FR) in in progress

  \param        void

  \return       BooleanType
                This parameter is true when the seeking is in progress
 */
/*----------------------------------------------------------------------------*/
BooleanType APP_MDI_bIsSeekingInProgress(void)
{
   return gAppMdiContext.bSendRelesaButtonAfterSeek;
}
//RT14519
/*----------------------------------------------------------------------------*/
/* Function :   MDI_vOnAppCshdlNotification                                  */
/**
  \brief        Sends pause command if it gets CSHDL power off notification

  \param        APP_CSHDL_NotificationType

  \return       void

 */
/*----------------------------------------------------------------------------*/
void MDI_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    switch(notification)
    {
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:
            if(gAppMdiContext.play_status = APP_MDI_ACT_STATE_PLAY)
            {
                if(APP_MDI_sfSetPlayMode(APP_MDI_PAUSE) == SUCCESS)
                {
                    DEBUG_TEXT(APP_MDI_SWI_TRC, MOD_APP_MDI, "14519-Mute test");
                    gAppMdiContext.bPauseMdiPoweroffCK5050 = BT_TRUE;
                }
            }
        break;
        default:
        break;
    }
}
/* End Of File APP_MDI_C1.c */


