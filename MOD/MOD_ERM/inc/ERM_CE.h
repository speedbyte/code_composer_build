/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_CE.h
*-----------------------------------------------------------------------------
* Module Name:    ERM External Resource Manager
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/inc/ERM_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ERM_CE_H
#define _ERM_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ATParser_CE.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/* configuration for ALC RX support */
/* #define ERM_ALC_RX_SUPPORTED 1 */

/* configuration for signed BT-Update */
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/**
    \brief      ERM_CHECK_UART_OWNERSHIP indicate if UART ownership check\n
                function shall really check or only indicate SUCCESS if\
                the define is NOT included!
    */
#define ERM_UART_BUSY_RETRY_TIME              20
#define ERM_CHECK_UART_OWNERSHIP 1

#define ERM_BT_DEVICE_NAME "UHV_BT"      // RT#1659
#define ERM_BT_DEVICE_NAME_MAXLEN 31      // Length as in E2P-Layout

#define ERM_BT_PIN_MAX 9999

/* 5 minute start up visibility */
#define ERM_INITIAL_BT_VISIBILITY_SEC 300
/* 3 minute callback 3button module visibility */
#define ERM_CALLBACK_BT_VISIBILITY_SEC 180

#define ERM_CALLBACK_STOP_JINGLE_MSEC 2000

#define ERM_UART_SUPERVISION  180   /* reset system if UART is not released within 3 min */


#define ERM_NUM_AUDIO_PARAM_SET  10
#define ERM_SPARE_AUDIO_BYTES    7

#define ERM_MAX_NUM_VOICE_TAGS_PER_USER        50  /* change for tests to lower : has to be 50 */
#define ERM_MAX_NUM_VOICE_TAGS_OVERALL        200  /* change for test to lower : has to be 200 */

#ifdef UHV_SHOW_UP_TO30_DEVICES
#define MAX_REQUESTED_INQUIRY_RESULT_ENTRIES    30
#else
#define MAX_REQUESTED_INQUIRY_RESULT_ENTRIES    5
#endif

/* CONSTANTS for the device and user profile table */
#define ERM_MAX_NUM_PHYSICAL_DEVICES (AT_MAX_PARIRING_DEVICE_ID + 1)
#define ERM_MAX_PHYSICAL_DEVICE_IDX  AT_MAX_PARIRING_DEVICE_ID

#define ERM_MAX_NUM_USER_PROFILES     4
#define ERM_MAX_USER_PROFILE_IDX      (ERM_MAX_NUM_USER_PROFILES - 1)

#define ERM_MAX_NUM_PLAYER_HS_TABLE  10
#define ERM_MAX_PLAYER_HS_TABLE_IDX    (ERM_MAX_NUM_PLAYER_HS_TABLE - 1)

#define ERM_MAX_NUM_PLAYER_PROFILE   ERM_MAX_NUM_PHYSICAL_DEVICES
#define ERM_MAX_PLAYER_IDX           (ERM_MAX_NUM_PLAYER_PROFILE - 1)

#define ERM_MAX_NUM_HEADSET_PROFILE  6
#define ERM_MAX_HEADSET_IDX          (ERM_MAX_NUM_HEADSET_PROFILE - 1)

#define ERM_MAX_NUM_PLCC             2
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
typedef enum
{
/* 0  */    ERM_NOTIFY_STARTUP_DONE,
/* 1  */    ERM_NOTIFY_PBSN_PHONE,
/* 2  */    ERM_NOTIFY_PBSN_A2DP,
/* 3  */    ERM_NOTIFY_PBSN_AVRCP,
/* 4  */    ERM_NOTIFY_PSBD,
/* 5  */    ERM_NOTIFY_PPRS,
/* 6  */    ERM_NOTIFY_SERVICE_SUPPORT_CHANGED,
/* 7  */    ERM_NOTIFY_SERVICE_SUPPORT_CHANGED_AFTER_SDP,
/* 8  */    ERM_NOTIFY_INQUIRY_END,
/* 9  */    ERM_NOTIFY_BT_VISIBILITY_ON,
/* 10  */    ERM_NOTIFY_BT_VISIBILITY_OFF,
/* 11 */    ERM_NOTIFY_DELETED_DEVICE,
/* 12 */    ERM_NOTIFY_ACTIVE_DEVICE_NAME_CHANGED,
/* 13 */    ERM_NOTIFY_MAILBOX_NUMBER_CHANGED,
/* 14 */    ERM_NOTIFY_WARN_DIAL_ERROR,
/* 15 */    ERM_NOTIFY_WARN_HANGUP_ERROR,
/* 16 */    ERM_NOTIFY_WARN_PICKUP_ERROR,
/* 17 */    ERM_NOTIFY_WARN_2ND_CALL_ERROR,
/* 18 */    ERM_NOTIFY_WARN_DTMF_ERROR,
/* 19 */    ERM_NOTIFY_WARN_AVRCP_ERROR,
/* 20 */    ERM_NOTIFY_FLASHERASE_END,
/* 21 */    ERM_NOTIFY_PPRQ,
/* 22 */    ERM_NOTIFY_PSPR,
/* 23 */    ERM_NOTIFY_PBSF
} ERMNotificationIdType;
//typedef enum
//{
///* 0  */    ERM_NOTIFY_STARTUP_DONE,
///* 1  */    ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED,
///* 2  */    ERM_NOTIFY_PLAYER_ACTIVE_STATE_CHANGED,
///* 3  */    ERM_NOTIFY_HEADSET_ACTIVE_STATE_CHANGED,
///* 4  */    ERM_NOTIFY_SERVICE_SUPPORT_CHANGED,
///* 5  */    ERM_NOTIFY_MDI_SERVICE_CHANGED,
///* 6  */    ERM_NOTIFY_INQUIRY_END,
///* 7  */    ERM_NOTIFY_BT_VISIBILITY_ON,
///* 8  */    ERM_NOTIFY_BT_VISIBILITY_OFF,
///* 9  */    ERM_NOTIFY_DELETED_DEVICE,
///* 10 */    ERM_NOTIFY_ACTIVE_DEVICE_NAME_CHANGED,
///* 11 */    ERM_NOTIFY_CONNECT_FAILED,
///* 12 */    ERM_NOTIFY_PAIRING_SUCCEEDED,
///* 13 */    ERM_NOTIFY_PAIRING_MAX_DEVICES_REACHED,
///* 14 */    ERM_NOTIFY_PAIRING_FAILED,
///* 15 */    ERM_NOTIFY_MAILBOX_NUMBER_CHANGED,
///* 16 */    ERM_NOTIFY_WARN_DIAL_ERROR,
///* 17 */    ERM_NOTIFY_WARN_HANGUP_ERROR,
///* 18 */    ERM_NOTIFY_WARN_PICKUP_ERROR,
///* 19 */    ERM_NOTIFY_WARN_2ND_CALL_ERROR,
///* 20 */    ERM_NOTIFY_WARN_DTMF_ERROR,
///* 21 */    ERM_NOTIFY_WARN_AVRCP_ERROR,
///* 22 */    ERM_NOTIFY_FLASHERASE_END,
///* 23 */    ERM_NOTIFY_RINGTONE_PLAY_END,
///* 24 */    ERM_NOTIFY_DISCONNECT_SUCC,
///* 25 */    ERM_NOTIFY_DISCONNECT_FAIL
//} ERMNotificationType;


#endif /* _ERM_CE_H */

/* End Of File ERM_CE.h */
