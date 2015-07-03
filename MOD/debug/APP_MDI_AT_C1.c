/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_MDI_AT_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MDI Application
*-----------------------------------------------------------------------------
* Description:    Handles all mdi specific requests/responses to/from
                  AT Cmd Parser
*-----------------------------------------------------------------------------
* $Date: 2011-09-15 15:32:41 +0200 (Do, 15 Sep 2011) $
* $Rev: 21755 $
* $Author: maennel $
* $URL:
* http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_MDI_AT_C1.c $
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
#include "APP_MDI_BAP.h"
#include "OSEK_AppTask_Alarm.h"
#include "TRACE.h"
#include "APP_MDI_AT_C1.id"

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

static SuccessFailType _APP_MDI_AT_sfHandlePKEV( const ATResKeyboardKeyType *pPkev );
static SuccessFailType _APP_MDI_AT_sfHandleDGED( const ATResPlayerGetEntDataType *pDged );
static SuccessFailType _APP_MDI_AT_sfHandleDGPS( const ATRspGetPlayerStatusType *pDgps );
static SuccessFailType _APP_MDI_AT_sfHandlePGMT( const ATResGetMetadataType *pPgmt );
static SuccessFailType _APP_MDI_AT_sfHandlePAMU( const ATResMetaDataUpdateType *pPamu );
static APP_MDI_enRandomMode _APP_MDI_AT_enAtToMDIChangeRandomMode( ATRandomModeStatusEnum enAtRandomMode );
static APP_MDI_enRepeatMode _APP_MDI_AT_enAtToMDIChangeRepeatMode( ATRepeatModeStatusEnum enAtRepeatMode );
//static BooleanType _APP_MDI_AT_bIsPlayStateConsistent( APP_MDI_enDiscoStatusValue state1,
//                                                       APP_MDI_enDiscoStatusValue state2 );

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
/* Function    : APP_MDI_vHandleATResponse                                   */
/**
    \brief      Handles response messages from AtCmdParser

    \param      atRsp
                ATRspAllType *

    \return     void

*/
/*----------------------------------------------------------------------------*/
void APP_MDI_vHandleATResponse(const ATRspMessageType *atRsp)
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
        case AT_RSP_AS_KEY_EVENT_INDICATOR:     /* PKEV */
           (void)_APP_MDI_AT_sfHandlePKEV( &atRsp->data->pkev );
            break;
//        case AT_RSP_AMGMT_SRD_ACTIVE_IND:       /* PASA */
//            (void)_APP_MDI_AT_sfHandlePASA( &atRsp->data->pasa );
//            break;
        case AT_RSP_PLAYER_GET_STATUS:          /* DGPS */
            (void)_APP_MDI_AT_sfHandleDGPS( &atRsp->data->dgps );
            break;
        case AT_RSP_AS_GET_META_TYPE:           /* PGMT */
            (void)_APP_MDI_AT_sfHandlePGMT( &atRsp->data->pgmt );
           break;
        case AT_RSP_DISCO_GET_ENTRY_DATA:       /* DGED */
            (void)_APP_MDI_AT_sfHandleDGED( &atRsp->data->dged );
            break;
        case AT_RSP_DISCO_METADATA_UPDATE:       /* PAMU */
            (void)_APP_MDI_AT_sfHandlePAMU( &atRsp->data->pamu );
            break;
        default:            
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "_APP_MDI_vHandleATResponse no valid response type: %d", atRsp->common.rsp_type);
            break;
    }
}/* END OF APP_MDI_vHandleATResponse() */

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_sfHandlePKEV                                      */
/**
  \brief        Parse and process incoming PKEV messages

  \param        pPkev
                ATResKeyboardKeyType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_AT_sfHandlePKEV( const ATResKeyboardKeyType *pPkev )
{
    APP_MDI_enActiveStatusValue oldPlayStatus = gAppMdiContext.play_status;


    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "sfHandlePKEV: %d", pPkev->key);

    switch(pPkev->key)
    {
        case AT_RSP_KEY_AUDIO_RESUMED:
        {
            switch(gAppMdiContext.play_status_at_sent)
            {
                case AT_MDB_PAUSE_PLAYING:
                    if( gAppMdiContext.IsCK5050ResponseOngoing == BT_TRUE )
                    {
                        gAppMdiContext.play_status = APP_MDI_ACT_STATE_INVALID;
                        /* wait again for some time and if no PKEV received try to evaluate PASA*/
                        AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(1500), APP_MDI_cbDPLYResponseTimeOut);
                        gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                        DEBUG_VALUE2(APP_MDI_SWI_TRC, MOD_APP_MDI, "Inconsistent play state, send DPLY:%d, received PKEV:%d",
                                gAppMdiContext.play_status_at_sent, pPkev->key);
                    }
                    break;

                default:
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_PLAY;
            }
            break;
        }
        case AT_RSP_KEY_AUDIO_SUSPENDED:
        {
            switch(gAppMdiContext.play_status_at_sent)
            {
                case AT_MDB_F_FORWARD_PLAYING:
                    /* pause indicates that ff mode was entered */
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_FAST_FORWARD;
                    break;
                case AT_MDB_F_REWIND_PLAYING: 
                    /* pause indicates that fr mode was entered */
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_FAST_REWIND;
                    break;
                case AT_MDB_RESUME_PLAYING:
                    if( gAppMdiContext.IsCK5050ResponseOngoing == BT_TRUE )
                    {
                        gAppMdiContext.play_status = APP_MDI_ACT_STATE_INVALID;

                        /* wait again for some time and if no PKEV received try to evaluate PASA*/
                        AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT, MSEC(1500), APP_MDI_cbDPLYResponseTimeOut);
                        gAppMdiContext.IsCK5050ResponseOngoing = BT_TRUE;
                        DEBUG_VALUE2(APP_MDI_SWI_TRC, MOD_APP_MDI, "Inconsistent play state, send DPLY:%d, received PKEV:%d",
                                gAppMdiContext.play_status_at_sent, pPkev->key);
                    }
                    break;
                case AT_MDB_STOP_PLAYING:
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_PAUSE;
                    break;
                default:
                    gAppMdiContext.play_status = APP_MDI_ACT_STATE_PAUSE;
                    break;
            }
            break;
        }
        default:
        {
            /* ignore other key events */
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "sfHandlePKEV: invalid key %d",
                          pPkev->key);

            return FAIL;
        }
    }

    /* cancel alarm for CKCM repsonse */
    if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_INVALID)&&(gAppMdiContext.IsCK5050ResponseOngoing == BT_TRUE))
    {
        gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
    }

    /* set the sent command indicator to idle  => no further command is sent and user interaction on MP device */
    if(gAppMdiContext.play_status_at_sent != AT_MDB_NONE)
        gAppMdiContext.play_status_at_sent = AT_MDB_NONE;

    DEBUG_VALUE2(APP_MDI_SWI_TRC, MOD_APP_MDI, "old play state %d, new play status %d",
            oldPlayStatus, gAppMdiContext.play_status);

    /* RT 13539: some android players send their state very late. in this case
     * the deferred play from HU will force another state change.
     * Only execute the deferred player status function, if the actual play
     * status is not invalid. The state is valid, after receiving PASA during
     * the initial sequence.
     */
    if (gAppMdiContext.play_status != APP_MDI_ACT_STATE_INVALID)
    {
        /* first handle the defer state*/
        if((APP_MDI_HandleDefer() == BT_FALSE) && (oldPlayStatus != gAppMdiContext.play_status))
        {
            _APP_MDI_vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED);
        }
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_sfHandleDGED                                      */
/**
  \brief        Parse and process incoming DGED messages

  \param        pDged
                ATResPlayerGetEntDataType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_AT_sfHandleDGED( const ATResPlayerGetEntDataType *pDged )
{

    switch(pDged->field_id)
    {
        case AT_ENTRY_MASK_FILEPATH:
            if( (pDged->data_string_len != gAppMdiContext.fileName.length) ||
                (0 != memcmp(&(gAppMdiContext.fileName.data), pDged->data_string, gAppMdiContext.fileName.length)) )
            {
                if(pDged->data_string_len <= AT_MDI_ENTRY_DATA_LEN)
                {
                    gAppMdiContext.fileName.length = pDged->data_string_len;
                }
                else
                {
                    gAppMdiContext.fileName.length = AT_MDI_ENTRY_DATA_LEN;
                }
                (void)memset(&gAppMdiContext.fileName.data, 0, AT_MDI_ENTRY_DATA_LEN);
                (void)memcpy(&(gAppMdiContext.fileName.data),
                              pDged->data_string,
                              gAppMdiContext.fileName.length);
                _APP_MDI_vNotifyApps(APP_MDI_PATHNAME_CHANGED);
            }
            break;
        case AT_ENTRY_MASK_TITLE:
            if( (pDged->data_string_len != gAppMdiContext.title.length) ||
                (0 != memcmp(&(gAppMdiContext.title.data), pDged->data_string, gAppMdiContext.title.length)) )
            {
                if(pDged->data_string_len <= AT_MDI_ENTRY_DATA_LEN)
                {
                    gAppMdiContext.title.length = pDged->data_string_len;
                }
                else
                {
                    gAppMdiContext.title.length = AT_MDI_ENTRY_DATA_LEN;
                }
                (void)memset(&gAppMdiContext.title.data, 0, AT_MDI_ENTRY_DATA_LEN);
                (void)memcpy(&(gAppMdiContext.title.data),
                              pDged->data_string,
                              gAppMdiContext.title.length);
                // It must be sent Album here, because BAP supports album containing title, artist and album.
                _APP_MDI_vNotifyApps(APP_MDI_ALBUM_CHANGED);
            }
            break;
        case AT_ENTRY_MASK_ALBUM:
            if( (pDged->data_string_len != gAppMdiContext.album.length) ||
                (0 != memcmp(&(gAppMdiContext.album.data), pDged->data_string, gAppMdiContext.album.length)) )
            {
                if(pDged->data_string_len <= AT_MDI_ENTRY_DATA_LEN)
                {
                    gAppMdiContext.album.length = pDged->data_string_len;
                }
                else
                {
                    gAppMdiContext.album.length = AT_MDI_ENTRY_DATA_LEN;
                }
                (void)memset(&gAppMdiContext.album.data, 0, AT_MDI_ENTRY_DATA_LEN);
                (void)memcpy(&(gAppMdiContext.album.data),
                              pDged->data_string,
                              gAppMdiContext.album.length);
                _APP_MDI_vNotifyApps(APP_MDI_ALBUM_CHANGED);
            }
            break;
        case AT_ENTRY_MASK_ARTIST:
            if( (pDged->data_string_len != gAppMdiContext.artist.length) ||
                (0 != memcmp(&(gAppMdiContext.artist.data), pDged->data_string, gAppMdiContext.artist.length)) )
            {
                if(pDged->data_string_len <= AT_MDI_ENTRY_DATA_LEN)
                {
                    gAppMdiContext.artist.length = pDged->data_string_len;
                }
                else
                {
                    gAppMdiContext.artist.length = AT_MDI_ENTRY_DATA_LEN;
                }
                (void)memset(&gAppMdiContext.artist.data, 0, AT_MDI_ENTRY_DATA_LEN);
                (void)memcpy(&(gAppMdiContext.artist.data),
                              pDged->data_string,
                              gAppMdiContext.artist.length);
                // It must be sent Album here, because BAP supports album containing title, artist and album.
                _APP_MDI_vNotifyApps(APP_MDI_ALBUM_CHANGED);
            }
            break;
        default:
            /* ignore other key events */
            DEBUG_VALUE1( APP_MDI_SWI_WNG, MOD_APP_MDI, "DGED: invalid id %d", pDged->field_id);
            break;
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_sfHandlePAMU                                      */
/**
  \brief        Parse and process incoming PAMU messages

  \param        pPamu
                ATResGetMetadataType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_AT_sfHandlePAMU( const ATResMetaDataUpdateType *pPamu )
{
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "cbGetDged: dev %d",
                  gAppMdiContext.avrcp_device_id );

    if( gAppMdiContext.avrcp_device_id == AT_INVALID_DEVICE_ID ||
        APP_MDI_sfGetDged(gAppMdiContext.avrcp_device_id, pPamu->fieldmask) != SUCCESS )
    {
        DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "cbGetDged: GetDged failed %d",
                      gAppMdiContext.avrcp_device_id );

        return FAIL;
    }

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_sfHandleDGPS                                      */
/**
  \brief        Parse and process incoming DGPS messages

  \param        pDgps
                ATRspGetPlayerStatusType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_AT_sfHandleDGPS( const ATRspGetPlayerStatusType *pDgps )
{
    if(pDgps->ext_info_available)
    {

        DEBUG_VALUE4( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleDGPS: [EXT] status %d, repeat_mode %d, random_mode %d", 
                      pDgps->status,
                      pDgps->repeat_mode,
                      pDgps->random_mode,
                      0);

        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleDGPS: [EXT] cur_pos   %d", pDgps->pos);
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleDGPS: [EXT] total_len %d", pDgps->total_len);
    }
    else
    {
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleDGPS: status  %d", pDgps->status);
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandleDGPS: cur_pos %d", pDgps->pos);
    }

    switch(pDgps->status)
    {
        case AT_DGPS_STATUS_STOP:
        case AT_DGPS_STATUS_PAUSE:
        {
            /* After discussing with Parrot the DGPS is not the essential event for player state*
             * evaluate only PKEV since now for play and pause state to avoid inconsistency !!! */

            //APP_MDI_enActiveStatusValue newPlayState = APP_MDI_ACT_STATE_PAUSE;

            /* first handle the defer state*/
            //if((APP_MDI_HandleDefer() == BT_FALSE) && (newPlayState != gAppMdiContext.play_status))
            //{
            //    gAppMdiContext.play_status = newPlayState;
            //    _APP_MDI_vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED);
            //}
            break;
        }
        case AT_DGPS_STATUS_PLAY:
        {
            /* After discussing with Parrot the DGPS is not the essential event for player state*
             * evaluate only PKEV since now for play and pause state to avoid inconsistency !!! */

            APP_MDI_enActiveStatusValue newPlayState = APP_MDI_ACT_STATE_PLAY;

            /* first handle the defer state*/
            /* for AVRCP devices after executing skip, FF FW PASA remains to 3(A2DP streams) then evaluate the DGPS event for the player state*/
            if((APP_DEVICE_GetPASAEvent() == AT_AUDIO_STATE_STREAMING_A2DP)&&(newPlayState != gAppMdiContext.play_status))
            {
                gAppMdiContext.play_status = newPlayState;
                _APP_MDI_vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED);
                AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DGPS_REQUEST, MSEC(APP_MDI_DGPS_REQUEST_DELAY), APP_MDI_cbGetDgps);
                /* cancel alarm for CKCM repsonse */
                if(gAppMdiContext.IsCK5050ResponseOngoing == BT_TRUE)
                {
                    gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
                    AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
                }

                /* set the sent command indicator to idle  => no further command is sent and user interaction on MP device */
                if(gAppMdiContext.play_status_at_sent != AT_MDB_NONE)
                    gAppMdiContext.play_status_at_sent = AT_MDB_NONE;
            }
            break;
        }
        case AT_DGPS_STATUS_TRACK_CHANGED:
        {
            if(gAppMdiContext.play_status_at_sent != AT_MDB_PAUSE_PLAYING)
            {
                AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DGPS_REQUEST, MSEC(APP_MDI_DGPS_REQUEST_DELAY), APP_MDI_cbGetDgps);
                gAppMdiContext.trackTime = APP_MDI_TOTAL_TRACK_TIME_INVALID;

                /* set the sent command indicator to idle  => no further command is sent and user interaction on MP device */
                if((gAppMdiContext.play_status_at_sent == AT_MDB_NEXT_SONG)||(gAppMdiContext.play_status_at_sent == AT_MDB_PREVIOUS_SONG))
                    gAppMdiContext.play_status_at_sent = AT_MDB_NONE;
            }
            break;
        }
        case AT_DGPS_STATUS_FAST_REWIND:
        {
            APP_MDI_enActiveStatusValue newPlayState = APP_MDI_ACT_STATE_FAST_REWIND;
            if(newPlayState != gAppMdiContext.play_status)
            {
                gAppMdiContext.play_status = newPlayState;
                _APP_MDI_vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED);

                /* set the sent command indicator to idle  => no further command is sent and user interaction on MP device */
                if(gAppMdiContext.play_status_at_sent == AT_MDB_F_REWIND_PLAYING)
                    gAppMdiContext.play_status_at_sent = AT_MDB_NONE;

                /* cancel alarm for CKCM repsonse */
                if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_INVALID)&&(gAppMdiContext.IsCK5050ResponseOngoing == BT_TRUE))
                {
                    gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
                    AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
                }
            }
            break;
        }
        case AT_DGPS_STATUS_FAST_FORWARD:
        {
            APP_MDI_enActiveStatusValue newPlayState = APP_MDI_ACT_STATE_FAST_FORWARD;
            if(newPlayState != gAppMdiContext.play_status)
            {
                gAppMdiContext.play_status = newPlayState;
                _APP_MDI_vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED);

                /* set the sent command indicator to idle  => no further command is sent and user interaction on MP device */
                if(gAppMdiContext.play_status_at_sent == AT_MDB_F_FORWARD_PLAYING)
                    gAppMdiContext.play_status_at_sent = AT_MDB_NONE;

                /* cancel alarm for CKCM repsonse */
                if((gAppMdiContext.play_status != APP_MDI_ACT_STATE_INVALID)&&(gAppMdiContext.IsCK5050ResponseOngoing == BT_TRUE))
                {
                    gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
                    AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
                }
            }
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "HandleDGPS: unknown status  %d", pDgps->status);
            break;
    }

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
    if( (pDgps->pos != gAppMdiContext.timePosition) || (pDgps->pos == APP_MDI_TOTAL_TRACK_TIME_INVALID))
    {
        gAppMdiContext.timePosition = pDgps->pos;
        _APP_MDI_vNotifyApps(APP_MDI_TIME_POSITION_CHANGED);
    }
#endif

    if(AT_TRUE == pDgps->ext_info_available)
    {


        APP_MDI_enRepeatMode newRepeatMode = _APP_MDI_AT_enAtToMDIChangeRepeatMode( pDgps->repeat_mode );
        APP_MDI_enRandomMode newRandomMode = _APP_MDI_AT_enAtToMDIChangeRandomMode( pDgps->random_mode );
        
        DEBUG_VALUE2( APP_MDI_SWI_ERR, MOD_APP_MDI, "HandleDGPS: extInfo status: gAppMdiContext.repeat: %d   newRepeatMode: %d ", gAppMdiContext.repeat, newRepeatMode);
        DEBUG_VALUE2( APP_MDI_SWI_ERR, MOD_APP_MDI, "HandleDGPS: extInfo status: gAppMdiContext.random: %d   newRandomMode: %d ", gAppMdiContext.random, newRandomMode);
        
#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
        if((gAppMdiContext.trackTime != pDgps->total_len) || (pDgps->total_len == APP_MDI_TOTAL_TRACK_TIME_INVALID))
        {
            gAppMdiContext.trackTime = pDgps->total_len;
            _APP_MDI_vNotifyApps(APP_MDI_ABS_TIME_CHANGED);
        }
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
        if(newRepeatMode != gAppMdiContext.repeat)
        {

            gAppMdiContext.repeat = newRepeatMode;
            _APP_MDI_vNotifyApps(APP_MDI_REPEAT_CHANGED);
        }

        if(newRandomMode != gAppMdiContext.random)
        {
            gAppMdiContext.random = newRandomMode;
            _APP_MDI_vNotifyApps(APP_MDI_RANDOM_CHANGED);
        }
#endif
    }


    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_sfHandlePGMT                                      */
/**
  \brief        Parse and process incoming PGMT messages

  \param        pPgmt
                ATResGetMetadataType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_AT_sfHandlePGMT( const ATResGetMetadataType *pPgmt )
{
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HandlePGMT: rcvd %d", pPgmt->type);

    switch (pPgmt->type)
    {
        case AT_META_NOT_SUPPORTED:
            gAppMdiContext.dev_meta_type = APP_MDI_AVRCP_BASE;
            // We have to trust incoming PKEV event. If it doesn't occur because player is already in
            // pause then play status must be faked here.
            //_APP_MDI_vNotifyApps(APP_MDI_PLAY_STATUS_CHANGED);
            if(APP_MDI_ACT_STATE_PAUSE != gAppMdiContext.play_status)
            {
                gAppMdiContext.play_status = APP_MDI_ACT_STATE_PAUSE;
                APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_PLAY_STATUS_CHANGED);
            }
            break;
        case AT_META_SONY_ERICCSON:
            gAppMdiContext.dev_meta_type = APP_MDI_SONY_ERICCSON;
            //Ticket #7145 (first track does not show ID3-Tag) was caused by requesting ID3 information (AT*DGED) to early
            //when AVRCP was connected already and A2DP not yet connected.
            //AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DGED_REQUEST, MSEC(APP_MDI_DGED_REQUEST_DELAY), APP_MDI_cbGetDgedDelayed);
            AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DGPS_REQUEST, MSEC(APP_MDI_DGPS_REQUEST_DELAY), APP_MDI_cbGetDgps);
            break;
        case AT_META_AVRCP_1_3:
        case AT_META_AVRCP_1_4:
            gAppMdiContext.dev_meta_type = (pPgmt->type == AT_META_AVRCP_1_3) ? APP_MDI_AVRCP_1_3 : APP_MDI_AVRCP_1_4;
            (void)APP_MDI_sfSetAutomaticNotificationMode(MDI_AUTO_NOT_DISABLE);
            //Ticket #7145 (first track does not show ID3-Tag) was caused by requesting ID3 information (AT*DGED) to early
            //when AVRCP was connected already and A2DP not yet connected.
            //AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DGED_REQUEST, MSEC(APP_MDI_DGED_REQUEST_DELAY), APP_MDI_cbGetDgedDelayed);
            AppTask_vSetAlarm(APP_TASK_ALARM_MDI_DGPS_REQUEST, MSEC(APP_MDI_DGPS_REQUEST_DELAY), APP_MDI_cbGetDgps);
            break;
        default:
            gAppMdiContext.dev_meta_type = APP_MDI_ONLY_A2DP;
            DEBUG_VALUE1( APP_MDI_SWI_WNG, MOD_APP_MDI, "HandlePGMT: unsupported pgmt %d", pPgmt->type);
            return FAIL;
    }

    _APP_MDI_vNotifyApps(APP_MDI_METADATATYPE_CHANGED);

    return SUCCESS;

}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_enAtToMDIChangeRandomMode                         */
/**
  \brief        Parse and process incoming DGPS messages

  \param        enAtRandomMode
                ATRandomModeStatusEnum

  \return       APP_MDI_enRandomMode

 */
/*----------------------------------------------------------------------------*/
static APP_MDI_enRandomMode _APP_MDI_AT_enAtToMDIChangeRandomMode( ATRandomModeStatusEnum enAtRandomMode )
{
    APP_MDI_enRandomMode random = MDI_RANDOM_DISABLE;

    switch(enAtRandomMode)
    {
        case AT_DEVRANDOM_UNKNOWN:
        case AT_DEVRANDOM_DISABLE:
            random = MDI_RANDOM_DISABLE;
            break;
        case AT_DEVRANDOM_ENABLE:
        case AT_DEVRANDOM_ENABLE_ALBUM:
            random = MDI_RANDOM_ENABLE;
            break;
        default:
            break;
    }
    return random;
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_enAtToMDIChangeRepeatMode                                      */
/**
  \brief        Parse and process incoming DGPS messages

  \param        enAtRepeatMode
                ATReqPlayerSetRepeatType

  \return       APP_MDI_enRepeatMode

 */
/*----------------------------------------------------------------------------*/
static APP_MDI_enRepeatMode _APP_MDI_AT_enAtToMDIChangeRepeatMode( ATRepeatModeStatusEnum enAtRepeatMode )
{
    APP_MDI_enRepeatMode repeat = MDI_REPEAT_DISABLE;

    switch(enAtRepeatMode)
    {
        case AT_DEVREPEAT_ENABLE_ONE_SONG:
            repeat = MDI_REPEAT_ONE_TRACK;
            break;
        case AT_DEVREPEAT_ENABLE_CURR_GROUP_SONG:
        case AT_DEVREPEAT_ENABLE_PLAYLIST:
            repeat = MDI_REPEAT_ALL_TRACK;
            break;
        case AT_DEVREPEAT_DISABLE:
        case AT_DEVREPEAT_UNKNOWN:
            repeat = MDI_REPEAT_DISABLE;
            break;
        default:
            break;
    }
    return repeat;
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_AT_sfHandlePASA                                      */
/**
  \brief        Get actiev Audio Source of CK5050+

  \param        none


  \return       last received PASA Event of CK5050+

 */
/*----------------------------------------------------------------------------*/
//ATAudioSourceActStateEnum APP_MDI_GetPASAEvent(void)
//{
//    return (gAppMdiContext.AudioSource);
//}



//void APP_MDI_InitPASAEvent(void)
//{
//    /* Parrot doesn't send a initial PASA Event, this function should be called if Parrot does a restart */
//    gAppMdiContext.AudioSource = AT_AUDIO_STATE_NO_ACTIVE;
//}



/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendKeyboardEventATRequest                         */
/**
  \brief        Builds and sends a keyboard event AT request

  \param        key
                ATReqKeyboardKeysEnum

  \param        event
                ATReqKeyboardEventEnum

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendKeyboardEventATRequest(  const SCI_ST *pUart,
                                                        ATReqKeyboardKeysEnum key,
                                                        ATReqKeyboardEventEnum event)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_AS_KEY_EVENT_INDICATOR;
    request.pkev.key            = key;
    request.pkev.event          = event;

    return AT_Cmd_Handler_MDI(&request, pUart);
}



/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendRandomModeATRequest                            */
/**
  \brief        Builds and sends a Set Random Mode AT request


  \param        Random mode (1=random enabled, 0=random diabled)


  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendRandomModeATRequest(const SCI_ST *pUart, ATRandomModeStatusEnum random_mode)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_PLAYER_SET_RANDOM_MODE;
    request.dsra.device_type    = AT_DEVICE_BLUETOOTH;
    request.dsra.device_id      = gAppMdiContext.avrcp_device_id;
    request.dsra.random_enabled = random_mode;

    return AT_Cmd_Handler_MDI(&request, pUart);
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendPlayerStateATRequest                            */
/**
  \brief        Builds and sends a Set Player Mode AT request


  \param        Random mode (1=random enabled, 0=random diabled)


  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendPlayerStateATRequest(const SCI_ST *pUart, ATDMDBActionEnum disco_mode)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_PLAYER_PLAY_MODE;
    request.dply.action         = disco_mode;
    request.dply.device_type    = AT_DEVICE_BLUETOOTH;
    request.dply.device_id      = gAppMdiContext.avrcp_device_id;
    request.dply.mdb_id         = gAppMdiContext.avrcp_device_id;
    request.dply.index_avail    = AT_FALSE;

    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "SendPlayerStateATRequest: %d", disco_mode );

    //gAppMdiContext.play_status_at_sent = disco_mode;

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendRandomModeATRequest                            */
/**
  \brief        Builds and sends a Set Random Mode AT request


  \param        Repeat mode (2=Repeat all tracks enabled,  1=Repeat one track enabled, 0=Repeat diabled)


  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendRepeatModeATRequest( const SCI_ST *pUart, 
                                                    ATPlayerRepeatModeEnum repeat_mode)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_PLAYER_SET_REPEAT_MODE;
    request.dsre.device_type    = AT_DEVICE_BLUETOOTH;
    request.dsre.device_id      = gAppMdiContext.avrcp_device_id;
    request.dsre.repeat_mode    = repeat_mode;

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendAutomaticPlayerNotificationATRequest                            */
/**
  \brief        Builds and sends a Status of the automatic Mode AT request


  \param        0: Disable automatic notification
                1: Enable automatic notification.

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendAutomaticPlayerNotificationATRequest(const SCI_ST *pUart, 
                                                                    ATReqConfigPositionNotiEnum notification_mode)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_PLAYER_NOTIFICATION;
    request.dcpn.automatic_notification  = notification_mode;

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendRandomModeATRequest                            */
/**
  \brief        Builds and sends a Set Random Mode AT request


  \param        none


  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendGetMetadataTypeATRequest(const SCI_ST *pUart, 
                                                        AT_DeviceId deviceId)
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_AS_GET_META_TYPE;
    request.pgmt.device_id      = deviceId;

    return AT_Cmd_Handler_MDI(&request, pUart);
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendDgpsATRequest                                  */
/**
  \brief        Builds and sends a DGPS AT request

  \param        deviceId
                AT_DeviceId

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendDgpsATRequest(   const SCI_ST    *pUart,
                                                AT_DeviceId     deviceId)
{
    ATReqAllType request;

    request.common.req_type         = AT_REQ_PLAYER_GET_STATUS;
    request.dgps.device_type        = AT_DEVICE_BLUETOOTH;
    request.dgps.device_id          = deviceId;
    request.dgps.ext_info_requested = AT_TRUE;

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendDgedATRequest                                  */
/**
  \brief        Builds and sends a DGED AT request

  \param        deviceId
                AT_DeviceId

  \param        pUart
                const SCI_ST *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfSendDgedATRequest(   const SCI_ST    *pUart,
                                                AT_DeviceId     deviceId,
                                                Uint8Type       fieldmask)
{
    ATReqAllType request;

    request.common.req_type             = AT_REQ_DISCO_GET_ENTRY_DATA;
    request.dged.device_type            = AT_DEVICE_BLUETOOTH;
    request.dged.device_id              = deviceId;
    request.dged.filepath_requested     = ((((Uint8Type)(fieldmask>>AT_ENTRY_MASK_FILEPATH) & 0x1)) == 0x1)?(AT_TRUE):(AT_FALSE);
    request.dged.title_requested        = ((((Uint8Type)(fieldmask>>AT_ENTRY_MASK_TITLE) & 0x1)) == 0x1)?(AT_TRUE):(AT_FALSE);
    request.dged.artist_requested       = ((((Uint8Type)(fieldmask>>AT_ENTRY_MASK_ARTIST) & 0x1)) == 0x1)?(AT_TRUE):(AT_FALSE);
    request.dged.album_requested        = ((((Uint8Type)(fieldmask>>AT_ENTRY_MASK_ALBUM) & 0x1)) == 0x1)?(AT_TRUE):(AT_FALSE);
    request.dged.tracknumber_requested  = ((((Uint8Type)(fieldmask>>AT_ENTRY_MASK_TRACKNUMBER) & 0x1)) == 0x1)?(AT_TRUE):(AT_FALSE);
    request.dged.genre_requested        = ((((Uint8Type)(fieldmask>>AT_ENTRY_MASK_GENRE) & 0x1)) == 0x1)?(AT_TRUE):(AT_FALSE);

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendALCKATRequest                                  */
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
SuccessFailType _APP_MDI_sfSendALCKATRequest(   const SCI_ST    *pUart,
                                                ATAlckLockType  lockState)
{
    ATReqAllType request;

    request.common.req_type         = AT_REQ_AMGMT_AUDIO_LOCK;
    request.alck.locked             = lockState;

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfSendDisableMdiATRequest                            */
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
SuccessFailType _APP_MDI_sfSendPbsvATRequest( const SCI_ST *pUart,
                                              ATBTServiceEnum service,
                                              ATPbsvActionType action)
{
    ATReqAllType request;

    request.common.req_type  = AT_REQ_BT_SERVICE_VISIBILITY;
    request.pbsv.service     = service;
    request.pbsv.action      = action;

    return AT_Cmd_Handler_MDI(&request, pUart);
}

/* End Of File APP_MDI_AT_C1.c */


