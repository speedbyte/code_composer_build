/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_MDI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_MDI
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_MDI
*-----------------------------------------------------------------------------
* $Date: 2011-06-15 14:52:02 +0200 (Mi, 15 Jun 2011) $
* $Rev: 20164 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/inc/APP_MDI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_MDI_H
#define _APP_MDI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "APP_COMMON_Utils.h"
//#include "ERM.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_MDI_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define UNMUTE_DELAYED_AFTER_CALL     3000     /* delay the unmute after call after  */
#define UNMUTE_IMMEDIATELY            0        /* affects the unmuting of the player immediately */


typedef enum
{
    APP_MDI_enResultOk      = 0x00,
    APP_MDI_enResultError   = 0x01
}APP_MDI_enResult;


typedef enum
{
    APP_MDI_PLAY_STATUS_CHANGED,
    APP_MDI_TIME_POSITION_CHANGED,
    APP_MDI_ABS_TIME_CHANGED,
    APP_MDI_REPEAT_CHANGED,
    APP_MDI_RANDOM_CHANGED,
    APP_MDI_TITLE_CHANGED,
    APP_MDI_PATHNAME_CHANGED,
    APP_MDI_ALBUM_CHANGED,
    APP_MDI_ARTIST_CHANGED,
    APP_MDI_METADATATYPE_CHANGED,
    APP_MDI_A2DP_STREAMING_ACTIVE
} APP_MDI_enNotificationId;

typedef enum
{
    APP_MDI_MUTE_AUDIO_MANAGER = 0,
    APP_MDI_MUTE_DEVICE_CONNECT,
    APP_MDI_MUTE_DEVICE_SEARCH
} APP_MDI_enMuteReason;

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
#include "APP_MDI_BAP.h"

SuccessFailType APP_MDI_sfInitApp(void);
SuccessFailType APP_MDI_sfReInitApp(void);
void APP_MDI_vHandleEvents(EventMaskType event);

BooleanType                   APP_MDI_bIsInitialized(void);
BooleanType                   APP_MDI_bPlayerAvailable(void);
BooleanType                   APP_MDI_bRemoteControlAvailable(void);
BooleanType                   APP_MDI_bPlayerActive(void);
AT_DeviceId                   APP_MDI_GetActiveServiceDeviceId(ATBTServiceEnum service);
APP_MDI_enActiveStatusValue   APP_MDI_GetPlayStatus(void);
APP_MDI_enActiveStatusValue   APP_MDI_GetSavePauseState(void);
APP_MDI_enRandomMode          APP_MDI_GetRandomMode(void);
APP_MDI_enRepeatMode          APP_MDI_GetRepeatMode(void);
Uint32Type                    APP_MDI_GetTrackTime(void);
Uint32Type                    APP_MDI_GetTrackTimePosition(void);
const AppMdiFileNameDataType* APP_MDI_GetFileName( void );
const AppMdiFileInfoDataType* APP_MDI_GetArtist( void );
const AppMdiFileInfoDataType* APP_MDI_GetTitle( void );
const AppMdiFileInfoDataType* APP_MDI_GetAlbum( void );
APP_MDI_enMetaDataType        APP_MDI_GetMetadataType(void);

//APP_COMMON_enResponseCode APP_MDI_vPausePlayerForVR(void (*cb)(APP_COMMON_enResponseCode));
//APP_COMMON_enResponseCode APP_MDI_vStopPlayerForVR(void (*cb)(APP_COMMON_enResponseCode));
//APP_COMMON_enResponseCode APP_MDI_vRestartPlayerAfterVR(void (*cb)(APP_COMMON_enResponseCode));

SuccessFailType APP_MDI_sfSetALCKMode(APP_MDI_enAudioLock lockState);
SuccessFailType APP_MDI_sfSetPlayMode(APP_MDI_enDiscoStatusValue discoMode);
SuccessFailType APP_MDI_sfVcrSetPlayMode(APP_MDI_enDiscoStatusValue discoMode);
SuccessFailType APP_MDI_sfSetSkip(APP_MDI_enDiscoStatusValue discoMode, Uint8Type counter);
SuccessFailType APP_MDI_sfGetPgmt(AT_DeviceId deviceId);
SuccessFailType APP_MDI_sfGetDged(AT_DeviceId deviceId, Uint8Type fieldmask);
SuccessFailType APP_MDI_sfGetDgps(AT_DeviceId deviceId);
SuccessFailType APP_MDI_sfSetRandomMode(APP_MDI_enRandomMode randomMode);
SuccessFailType APP_MDI_sfSetRepeatMode(APP_MDI_enRepeatMode repeatMode);
SuccessFailType APP_MDI_sfSetAutomaticNotificationMode(APP_MDI_enAutoNotMode autoNotMode);
SuccessFailType APP_MDI_sfSetPlayerVisible(BooleanType setVisible);
void APP_MDI_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification);
void APP_MDI_vHandleATResponse(const ATRspMessageType *atRsp);
void APP_MDI_cbMuteGetAck(APP_COMMON_enResponseCode rsp);
BooleanType APP_MDI_bMutePlayer(APP_MDI_enMuteReason muteReason);
BooleanType APP_MDI_bUnmutePlayer(APP_MDI_enMuteReason unmuteReason, TickType DelayCommandTime);
BooleanType APP_MDI_bUnlockPlayer(void);
void APP_MDI_vDeferPlayMode(APP_MDI_enDiscoStatusValue discoMode);
BooleanType IsCK5050ResponseStillOngoing(void);
BooleanType APP_MDI_HandleDefer(void);
void APP_MDI_cbDPLYResponseTimeOut(void);
void APP_MDI_cbSendDPLYDelayed(void);
BooleanType APP_MDI_bIsSeekingInProgress(void);
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _APP_MDI_H */

/* End Of File APP_MDI.h */

