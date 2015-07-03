/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_MDI_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_MDI
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-10-31 14:30:21 +0100 (Mo, 31 Okt 2011) $
* $Rev: 22910 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/inc/APP_MDI_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_MDI_CI_H
#define _APP_MDI_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
//#include "common.h"
//#include "ATCommonData.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/
//#define WORKAROUND_RT_2667_IGNORE_FIRST_PASA3_EVENT        1

#define MAX_PLAY_STATE_RETRY_COUNTER        2
#define APP_MDI_FCT_DISCO_RETRY_TIME        1000
#define APP_MDI_PGMT_REQUEST_DELAY          500
#define APP_MDI_DGED_REQUEST_DELAY          500
#define APP_MDI_DGPS_REQUEST_DELAY          1000
#define APP_MDI_ALCK_STARTUP_DELAY          5000



/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/

void _APP_MDI_vNotifyApps(APP_MDI_enNotificationId event);
void _APP_MDI_vSetPlayStatusNotifyApps(APP_MDI_BAP_enPlayStatusValue status);
void _APP_MDI_vSendResultReleaseContext(APP_COMMON_enResponseCode result, Uint32Type line);
void _APP_MDI_vReleaseContext();
void _APP_MDI_vReInitApp();

APP_COMMON_enResponseCode _APP_MDI_sfRequestMetadataType (AT_DeviceId mdDevice);
APP_COMMON_enResponseCode _APP_MDI_vSetAutomaticNotificationMode(void(*cb)(APP_COMMON_enResponseCode), APP_MDI_enAutoNotMode autoNotMode);
APP_COMMON_enResponseCode _APP_MDI_sfRequestDgps( void(*cb)(APP_COMMON_enResponseCode), AT_DeviceId deviceId);
APP_COMMON_enResponseCode _APP_MDI_sfRequestDged( void(*cb)(APP_COMMON_enResponseCode), AT_DeviceId deviceId);

SuccessFailType _APP_MDI_sfHandlePendingFunction( const SCI_ST* puart );


void APP_MDI_cbStartupUnlockAudio();
void APP_MDI_cbPlayStateTimeout();
void APP_MDI_cbGetPgmtDelayed();
void APP_MDI_cbGetDgps();

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/

typedef enum 
{
/* 0  */ APP_MDI_JOB_INVALID = 0,
/* 1  */ APP_MDI_JOB_DPLY,
/* 2  */ APP_MDI_JOB_SKIP,
/* 3  */ APP_MDI_JOB_ALCK,
/* 4  */ APP_MDI_JOB_PGMT,
/* 5  */ APP_MDI_JOB_DGED,
/* 6  */ APP_MDI_JOB_DGPS,
/* 7  */ APP_MDI_JOB_DSRA,
/* 8  */ APP_MDI_JOB_DSRE,
/* 9  */ APP_MDI_JOB_DCPN,
/* 10 */ APP_MDI_JOB_PBSV,
///* 11 */ APP_MDI_JOB_SVC_ACT,
///* 12 */ APP_MDI_JOB_SVC_DEACT,
/* -------------------------- */
        APP_MDI_JOB_MAX
} APP_MDI_enJobId;

typedef enum
{
    APP_MDI_MUTE_NO_ACTION = 0,
    APP_MDI_MUTE_GET,
    APP_MDI_MUTE_RELEASE
}AppMdiMuteActionType;

/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

typedef struct
{
    APP_MDI_enJobId         id;
}AppMdiCommonJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATDMDBActionEnum            playModeAt;
    AppMdiMuteActionType        mute;
}AppMdiDplyJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATAlckLockType              lockType;
}AppMdiAlckJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATDMDBActionEnum            skipModeAt;
    Uint8Type                   counter;
}AppMdiSkipJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    AT_DeviceId                 deviceId;
}AppMdiPgmtJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    AT_DeviceId                 deviceId;
    Uint8Type                   fieldmask;
}AppMdiDgedJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    AT_DeviceId                 deviceId;
}AppMdiDgpsJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATRandomModeStatusEnum      RandomMode;
}AppMdiDsraJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATPlayerRepeatModeEnum      repeatMode;
}AppMdiDsreJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATReqConfigPositionNotiEnum notifyMode;
}AppMdiDcpnJobType;

typedef struct
{
    AppMdiCommonJobType         common;
    ATBTServiceEnum             service;
    ATPbsvActionType            action;
}AppMdiPbsvJobType;


typedef union
{
    AppMdiCommonJobType     common;
    AppMdiDplyJobType       dply;
    AppMdiAlckJobType       alck;
    AppMdiSkipJobType       skip;
    AppMdiPgmtJobType       pgmt;
    AppMdiDgedJobType       dged;
    AppMdiDgpsJobType       dgps;
    AppMdiDsraJobType       dsra;
    AppMdiDsreJobType       dsre;
    AppMdiDcpnJobType       dcpn; 
    AppMdiPbsvJobType       pbsv;
}AppMdiJobType;





typedef struct
{
    APP_MDI_enActiveStatusValue     play_status;         // the ck5050 play state 
    ATDMDBActionEnum                play_status_at_sent; // our desired play state
    BooleanType                     playstate_consistent;
    Uint8Type                       retry_counter;
    APP_MDI_enMetaDataType          dev_meta_type;
    Uint32Type                      trackTime;
    Uint32Type                      timePosition;
    AppMdiFileNameDataType          fileName;
    AppMdiFileInfoDataType          artist;
    AppMdiFileInfoDataType          title;
    AppMdiFileInfoDataType          album;
    APP_MDI_enRandomMode            random;
    APP_MDI_enRepeatMode            repeat;
    AT_DeviceId                     avrcp_device_id;
    Uint8Type                       player_mute;
    APP_MDI_enDiscoStatusValue      unmute_request;
    APP_MDI_enDeferStatusValue      defer_play_status;
    Uint8Type                       audioSourceLockLocal;
    BooleanType                     IsCK5050ResponseOngoing; // gives the state of command processing(if still response from CK needed)
    APP_MDI_enDeferStatusValue      enOldRequest;
    Uint8Type                       PGMTErrorCounter;
    BooleanType                     bSendRelesaButtonAfterSeek;
    BooleanType                     bPauseMdiPoweroffCK5050;//RT14519
    AppMdiJobType                   fct;
}AppMdiContext;





void _APP_MDI_vJobManager(const AppMdiJobType *pJob);
SuccessFailType _APP_MDI_sfGetNextJob(AppMdiJobType *pJob);
void MDI_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification); //RT14519




#endif /* _APP_MDI_CI_H */

/* End Of File APP_MDI_CI.h */
