/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_SD_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SD
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-11-23 08:25:57 +0100 (Mi, 23 Nov 2011) $
* $Rev: 24103 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/inc/APP_SD_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SD_CI_H
#define _APP_SD_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "APP_PHONEBOOK_CE.h"
#include "VCR.h"
#include "KPB.h"
#include "APP_DEVICE.h"
#include "APP_PHONE.h"
#include "APP_SMS.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "bap_defines.h"



/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_SD_FrameDataBase.h"
#include "APP_SD_FrameBuilder.h"
#include "APP_SD_FrameId.h"
#include "APP_SD_SpellerCtrl.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
//#define ITEMPLATE 100 /* CONFIGURATE EXPORT INTERFACE(ITEMPLATE) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/
//#define ITMPL_Func IUART_Func




/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/

//#define APP_SD_MAX_CTX_DEPTH 8
#define APP_SD_MAX_CTX_DEPTH 15

#define APP_SD_MS_TIMEOUT_300   300
#define APP_SD_MS_TIMEOUT_500   500
#define APP_SD_MS_TIMEOUT_1000  1000
#define APP_SD_MS_TIMEOUT_2000  2000
#define APP_SD_MS_TIMEOUT_4000  4000
#define APP_SD_MS_TIMEOUT_5000  5000
#define APP_SD_MS_TIMEOUT_10000 10000
#define APP_SD_MS_TIMEOUT_15000 15000
#define APP_SD_MS_TIMEOUT_20000  20000		//RT12883
#define APP_SD_MS_TIMEOUT_30000 30000
#define APP_SD_MS_TIMEOUT_45000 45000
#define APP_SD_MS_TIMEOUT_60000 60000

#define APP_SD_SEARCH_STR_SIZE  4

//#define APP_SD_EEPROM_TEST 1


typedef enum
{   
    APP_SD_MDI_MUTE_FOR_CONNECT = 0,
    APP_SD_MDI_MUTE_FOR_PAIRING,
    APP_SD_MDI_MUTE_FOR_SEARCH
}APP_SD_enMdiMuteReason;

typedef enum 
{
/*  0 */    APP_SD_IDLE_SCREEN_ALL = 0,
/*  1 */    APP_SD_IDLE_SCREEN_ACTIVE_PHONE,
/*  2 */    APP_SD_IDLE_SCREEN_INACTIVE_PHONE
}APP_SD_enIdleScreenState;


typedef enum
{
    APP_SD_CTX_FRAME_REQ_SUB_STATE_DATA = 0,
    APP_SD_CTX_FRAME_REQ_DATA,
    APP_SD_CTX_FRAME_WAIT_FOR_DATA,
    APP_SD_CTX_FRAME_WAIT_FOR_RESSOURCE,
    APP_SD_CTX_FRAME_SENT,
    APP_SD_CTX_FRAME_SENT_FAILED
}APP_SD_enSimpleCtxState;

typedef enum
{
    APP_SD_PENDING_NONE = 0,
    APP_SD_PENDING_INIT,
    APP_SD_PENDING_BUTTON
}APP_SD_enPendingType;

typedef enum 
{
    APP_SD_SEARCH = 0,
    APP_SD_CONNECT,
    APP_SD_PAIRING
}APP_SD_enChooseState;

typedef enum
{
    APP_SD_MDI_STOP = 0,
    APP_SD_MDI_PLAY
}APP_SD_enMDIState;


typedef enum 
{
/*  0 */    APP_SD_EVENT_INVALID = 0,
/*  1 */    APP_SD_EVENT_BUTTON,
/*  2 */    APP_SD_EVENT_PHONE,
/*  3 */    APP_SD_EVENT_SMS,
/*  4 */    APP_SD_EVENT_PHONEBOOK,
/*  5 */    APP_SD_EVENT_MDI,
/*  6 */    APP_SD_EVENT_VCR,
/*  7 */    APP_SD_EVENT_DEVICE,
/*  8 */    APP_SD_EVENT_TIMEOUT,
/*  9 */    APP_SD_EVENT_ERM_STATE,
/* 10 */    APP_SD_EVENT_ERM,
/* 11 */    APP_SD_EVENT_CAN_LANG,
/* 12 */    APP_SD_EVENT_CAN_SPEED,
/* 13 */    APP_SD_EVENT_KPB,
/* 14 */    APP_SD_EVENT_CSHDL
} APP_SD_enEventId;

typedef enum
{
    APP_SD_HISTORY_RESET,
    APP_SD_HISTORY_APPEND,
    APP_SD_HISTORY_REPLACE
}APP_SD_enHistoryAction;


typedef enum
{
    APP_SD_S001_CONNECT_NOT_SENT = 0,
    APP_SD_S001_CONNECT_SENT,
    APP_SD_S001_SHOW_ONLY
}APP_SD_enSubStateS001;

typedef enum 
{    
    APP_SD_S306_SEARCH_MAILBOX_NUMBER_NOT_SENT = 0,
    APP_SD_S306_SEARCH_MAILBOX_NUMBER_SENT,
    APP_SD_S306_REQUEST_FOUND_ENTRY_NOT_SENT,
    APP_SD_S306_REQUEST_FOUND_ENTRY_SENT,
    APP_SD_S306_SHOW_FRAME    
}APP_SD_enSubStateS306;       

typedef enum 
{
    APP_SD_S117_REQUEST_CONTACT_INFO_NOT_SENT       = 0,
    APP_SD_S117_REQUEST_CONTACT_INFO_SENT,
    APP_SD_S117_REQUEST_VOICE_TAG_NUMBER_NOT_SENT,
    APP_SD_S117_REQUEST_VOICE_TAG_NUMBER_SENT
}APP_SD_enSubStateS117;      

typedef APP_SD_enSubStateS117 APP_SD_enSubStateS115;

typedef enum
{
    APP_SD_S117_UNKNOWN = 0,
    APP_SD_S117_PLAY_TAG,
    APP_SD_S117_RECORD_TAG,
    APP_SD_S117_RESET_TAG
}APP_SD_enLastSelectedMenueS117;

typedef APP_SD_enLastSelectedMenueS117 APP_SD_enLastSelectedMenueS115;

typedef enum
{
    APP_SD_S406_SMS_NOT_REQUESTED = 0,
    APP_SD_S406_MARK_AS_READ_NOT_REQUESTED,
    APP_SD_S406_SMS_REQUESTED
}APP_SD_enSubStateS406;

typedef enum
{
    APP_SD_S407_STOP_BT_SEARCH_NOT_SENT = 0,
    APP_SD_S407_STOP_BT_SEARCH_SENT,
    APP_SD_S407_STOP_BT_SEARCH_SKIPPED
}APP_SD_enSubStateS407;


typedef enum
{
    APP_SD_S408_WAIT_FOR_SERVICE_SUPPORT_CHANGED = 0,
    APP_SD_S408_RECEIVED_SERVICE_SUPPORT_CHANGED,
    APP_SD_S408_RECEIVED_PAIRING_FAILED,
    APP_SD_S408_NOT_SEND_SSP_SUPPORT,
    APP_SD_S408_WAIT_FOR_SSP_SUPPORT,
    APP_SD_S408_RECEIVED_SSP_SUPPORT
}APP_SD_enSubStateS408;

typedef enum 
{    
    APP_SD_S520_START_INQUIRY_NOT_SENT = 0,
    APP_SD_S520_START_INQUIRY_SENT,
    APP_SD_S520_SHOW_ONLY
}APP_SD_enSubStateS520;  

typedef enum 
{    
    APP_SD_S522_PAUSE_STATE_NOT_SENT = 0,
    APP_SD_S522_PAUSE_STATE_SENT
}APP_SD_enSubStateS522;

typedef enum 
{    
    APP_SD_S642_PAUSE_PLAYER_NOT_SENT,
    APP_SD_S642_PLAY_RINGTONE_NOT_SENT,
    APP_SD_S642_PLAY_RINGTONE_SENT
}APP_SD_enSubStateS642;       

typedef enum
{
    APP_SD_S801_NUMBER_NOT_REQUESTED = 0,
    APP_SD_S801_NUMBER_REQUESTED,
    APP_SD_S801_NUMBER_DIALED
}APP_SD_enSubStateS801;

typedef enum
{
    APP_SD_S945_NUMBER_NOT_REQUESTED = 0,
    APP_SD_S945_NUMBER_REQUESTED,
    APP_SD_S945_NUMBER_DIALED
}APP_SD_enSubStateS945;

typedef enum
{
    APP_SD_SC12_DISCONNECT_NOT_SENT = 0,
    APP_SD_SC12_DISCONNECT_SENT,
    APP_SD_SC12_CONNECT_NOT_SENT, 
    APP_SD_SC12_CONNECT_SENT,
    APP_SD_SC12_SHOW_ONLY
}APP_SD_enSubStateSC12;

/* SubStateBtn */
typedef enum
{
    APP_SD_S104_PB_SEARCH_NOT_SENT = 0,
    APP_SD_S104_PB_SEARCH_SENT,
    APP_SD_S104_PB_REQUEST_ENTRIES_NOT_SEND
}APP_SD_enSubStateBtnS104;

typedef enum
{
    APP_SD_SC20_HANGUP_NOT_SENT = 0,
    APP_SD_SC20_HANGUP_SENT,
    APP_SD_SC20_HANDSFREE_NOT_SENT,
    APP_SD_SC20_HANDSFREE_SENT
}APP_SD_enSubStateBtnSC20;

typedef enum 
{    
    APP_SD_S404_REQUEST_ORIGIN_NOT_SENT = 0,
    APP_SD_S404_REQUEST_ORIGIN_SENT
}APP_SD_enSubStateBtnS404;       

typedef enum 
{    
    APP_SD_S405_REQUEST_SMS_NOT_SENT = 0,
    APP_SD_S405_REQUEST_SMS_SENT
}APP_SD_enSubStateBtnS405;       

typedef enum 
{    
    APP_SD_S306_SAVE_MAILBOX_NUMBER_NOT_SENT = 0,
    APP_SD_S306_DIAL_MAILBOX_NUMBER_NOT_SENT
}APP_SD_enSubStateBtnS306;       



typedef enum 
{    
    APP_SD_S413_DELETE_SMS_NOT_SENT = 0,
    APP_SD_S413_DELETE_SMS_SENT
}APP_SD_enSubStateBtnS413;       

typedef enum
{
    APP_SD_S51D_PAIRING_NOT_SENT = 0,
    APP_SD_S51D_PAIRING_SENT,
    APP_SD_S51D_REJECT_NOT_SENT,
    APP_SD_S51D_REJECT_SENT
}APP_SD_enSubStateBtnS51D;       

typedef enum
{
    APP_SD_S52C_SSP_NOT_CONFIRMED = 0,
    APP_SD_S52C_SSP_CONFIRMED
}APP_SD_enSubStateBtnS52C;

typedef enum
{
    APP_SD_S52C_TIMEOUT_NOT_GONE = 0,
    APP_SD_S52C_TIMEOUT_GONE,
    APP_SD_S52C_PPRS_RECEIVED,
    APP_SD_S52C_PPRS_FAILED
}APP_SD_enSubStateS52C;

typedef enum
{
    APP_SD_S52D_TIMEOUT_NOT_GONE = 0,
    APP_SD_S52D_PPRS_NOT_RECEIVED,
    APP_SD_S52D_TIMEOUT_GONE,
    APP_SD_S52D_PPRS_RECEIVED,
    APP_SD_S52D_PPRS_FAILED
}APP_SD_enSubStateS52D;

typedef enum 
{    
    APP_SD_S581_START_INQUIRY_NOT_SENT = 0,
    APP_SD_S581_START_INQUIRY_SENT
}APP_SD_enSubStateS581;       

typedef enum 
{    
    APP_SD_S581_CANCEL_INQUIRY_NOT_SENT = 0,
    APP_SD_S581_CANCEL_INQUIRY_SENT
}APP_SD_enSubStateBtnS581;       

typedef enum 
{    
    APP_SD_SC13_START_INQUIRY_NOT_SENT = 0,
    APP_SD_SC13_START_INQUIRY_SENT,
    APP_SD_SC13_SHOW_ONLY
}APP_SD_enSubStateSC13;       

typedef enum
{
//    APP_SD_S534_WAIT_FOR_USER_PROFILE_ADDED_CB = 0,
    APP_SD_S534_WAIT_FOR_TIMEOUT = 0,
    APP_SD_S534_ADD_NEW_PROFILE_NOT_SENT,
    APP_SD_S534_ADD_NEW_PROFILE_SENT,
    APP_SD_S534_ADD_PLAYER_PROFILE_NOT_SENT,
    APP_SD_S534_ADD_PLAYER_PROFILE_SENT
}APP_SD_enSubStateS534;

typedef enum 
{    
    APP_SD_S517_START_SERIVCE_NOT_SENT = 0,
    APP_SD_S517_START_SERIVCE_SENT
}APP_SD_enSubStateS517;       

typedef enum 
{    
    APP_SD_S520_CANCEL_INQUIRY_NOT_SENT = 0,
    APP_SD_S520_CANCEL_INQUIRY_SENT
}APP_SD_enSubStateBtnS520;       


typedef enum 
{    
    APP_SD_SC13_CANCEL_INQUIRY_NOT_SENT = 0,
    APP_SD_SC13_CANCEL_INQUIRY_SENT
}APP_SD_enSubStateBtnSC13;       

typedef enum 
{    
    APP_SD_S556_DEVICE_NAME_NOT_SENT = 0,
    APP_SD_S556_DEVICE_NAME_SENT
}APP_SD_enSubStateBtnS556;      

typedef enum 
{    
    APP_SD_S542_DISCONNECT_NOT_SENT = 0,
    APP_SD_S542_DISCONNECT_SENT,
    APP_SD_S542_DISCONNECT_PLAYER_STILL_TO_BE_SEND,
    APP_SD_S542_DISCONNECT_PLAYER_SENT
}APP_SD_enSubStateBtnS542;       

typedef enum 
{    
    /* ticket 8480: this substate is no more needed APP_SD_S580_DELETE_OLDEST_USER_PROFILE_NOT_SENT = 0,*/
    APP_SD_S580_DELETE_OLDEST_PLAYER_PROFILE_NOT_SENT = 0,
    APP_SD_S580_ADD_NEW_PLAYER_PROFILE_NOT_SEND,
    APP_SD_S580_ADD_NEW_USER_PROFILE_NOT_SEND,
    APP_SD_S580_DELETE_CURRENT_PROFILE_NOT_SENT
}APP_SD_enSubStateBtnS580;       

typedef enum 
{    
    APP_SD_S575_CHANGE_PROFILE_NOT_SEND = 0,
    APP_SD_S575_CHANGE_PROFILE_SEND
}APP_SD_enSubStateBtnS575;       

typedef enum 
{    
    APP_SD_S535_ADD_NEW_PROFILE_NOT_SENT = 0,
    APP_SD_S535_ADD_NEW_PROFILE_SENT,
    APP_SD_S535_DELETE_DEVICE_NOT_SENT,
    APP_SD_S535_DELETE_DEVICE_SENT,
    APP_SD_S535_DELETE_NOT_SENT,
    APP_SD_S535_DELETE_SENT
}APP_SD_enSubStateBtnS535;     

typedef enum 
{    
    APP_SD_S536_CHANGE_PROFILE_NOT_SEND = 0,
    APP_SD_S536_CHANGE_PROFILE_SEND
}APP_SD_enSubStateBtnS536;   

typedef enum 
{    
    APP_SD_S563_DISCONNECT_NOT_SENT = 0,
    APP_SD_S563_DISCONNECT_SENT
}APP_SD_enSubStateBtnS563;       

typedef enum 
{    
    APP_SD_S572_DISCONNECT_NOT_SENT = 0,
    APP_SD_S572_DISCONNECT_SENT,
    APP_SD_S572_DISCONNECT_SUCCESSFUL,  /*RT13181: 4 digit pin shown instead of SSP*/
    APP_SD_S572_PAIRING_SENT            /*RT13181: 4 digit pin shown instead of SSP*/
}APP_SD_enSubStateBtnS572;       



typedef enum 
{    
    APP_SD_S512_DELETE_NOT_SENT = 0,
    APP_SD_S512_DELETE_SENT
}APP_SD_enSubStateBtnS512;

typedef enum 
{    
    APP_SD_S513_DELETE_NOT_SENT = 0,
    APP_SD_S513_DELETE_SENT
}APP_SD_enSubStateBtnS513;       

typedef enum 
{    
    APP_SD_S555_DELETE_NOT_SENT = 0,
    APP_SD_S555_DELETE_SENT
}APP_SD_enSubStateBtnS555;       

typedef enum 
{    
    APP_SD_S576_DELETE_NOT_SENT = 0,
    APP_SD_S576_DELETE_SENT
}APP_SD_enSubStateBtnS576;       

typedef enum 
{    
    APP_SD_S579_DELETE_NOT_SENT = 0,
    APP_SD_S579_DELETE_SENT
}APP_SD_enSubStateBtnS579;       

typedef enum 
{    
    APP_SD_S642_OK_RINGTONE_SELECT_NOT_SENT = 0,
    APP_SD_S642_OK_RINGTONE_SELECT_SENT,
    APP_SD_S642_OK_RINGTONE_STOP_NOT_SENT,
    APP_SD_S642_UP_DOWN_STOP_NOT_SENT,
    APP_SD_S642_UP_DOWN_STOP_SENT,
    APP_SD_S642_BACK_STOP_NOT_SENT,
    APP_SD_S642_LOST_CTX_STOP_NOT_SENT,
    APP_SD_S642_VCR_ACTIVE_STOP_NOT_SENT
}APP_SD_enSubStateBtnS642;       

typedef enum 
{    
    APP_SD_S661_START_SYNCHRO_NOT_SENT = 0,
    APP_SD_S661_START_SYNCHRO_SENT
}APP_SD_enSubStateBtnS661;       

typedef enum 
{    
    APP_SD_S669_SORTORDER_SELECT_NOT_SENT = 0,
    APP_SD_S669_SORTORDER_SELECT_SENT
}APP_SD_enSubStateBtnS669;       

typedef enum 
{    
    APP_SD_SC16_SET_PLAY_CALLER_NAME_SELECT_NOT_SENT = 0,
    APP_SD_SC16_SET_PLAY_CALLER_NAME_SELECT_SENT
}APP_SD_enSubStateBtnSC16;       

typedef enum 
{    
    APP_SD_SC16_GET_PLAY_CALLER_NAME_SELECT_NOT_SENT = 0,
    APP_SD_SC16_GET_PLAY_CALLER_NAME_SELECT_SENT
}APP_SD_enSubStateSC16;       

typedef enum 
{    
    APP_SD_S602_SET_AUDIO_MODE_NOT_SENT = 0,
    APP_SD_S602_SET_AUDIO_MODE_SENT
}APP_SD_enSubStateBtnS602;   
    
typedef enum 
{    
    APP_SD_S602_GET_AUDIO_MODE_NOT_SENT = 0,
    APP_SD_S602_GET_AUDIO_MODE_SENT
}APP_SD_enSubStateS602;       



typedef enum 
{    
    APP_SD_S577_SET_AUTH_MODE_NOT_SENT = 0,
    APP_SD_S577_SET_AUTH_MODE_SENT
}APP_SD_enSubStateBtnS577;  


typedef enum
{
    APP_SD_S801_HANGUP_NOT_SENT = 0,
    APP_SD_S801_HANGUP_SENT,
    APP_SD_S801_HEADSET_NOT_SENT,
    APP_SD_S801_HEADSET_SENT,
    APP_SD_S801_PRIVATE_MODE_NOT_SENT,
    APP_SD_S801_PRIVATE_MODE_SENT
}APP_SD_enSubStateBtnS801;

typedef enum
{
    APP_SD_S408_HANGUP_NOT_SENT = 0,
    APP_SD_S408_HANGUP_SENT
}APP_SD_enSubStateBtnS408;

typedef enum
{
    APP_SD_S803_HANGUP_NOT_SENT = 0,
    APP_SD_S803_HANGUP_SENT,
    APP_SD_S803_HOLD_NOT_SENT,
    APP_SD_S803_HOLD_SENT,
    APP_SD_S803_HEADSET_NOT_SENT,
    APP_SD_S803_HEADSET_SENT,
    APP_SD_S803_HANDSFREE_NOT_SENT,
    APP_SD_S803_HANDSFREE_SENT,
    APP_SD_S803_MICROMUTE_NOT_SENT,
    APP_SD_S803_MICROMUTE_SENT
}APP_SD_enSubStateBtnS803;

typedef enum
{
    APP_SD_S804_DTMF_NOT_SENT = 0,
    APP_SD_S804_DTMF_SENT
}APP_SD_enSubStateBtnS804;

typedef enum
{
    APP_SD_S811_RESUME_NOT_SENT = 0,
    APP_SD_S811_RESUME_SENT,
    APP_SD_S811_HANGUP_NOT_SENT,
    APP_SD_S811_HANGUP_SENT
}APP_SD_enSubStateBtnS811;

typedef enum
{
    APP_SD_S812_HANGUP_NOT_SENT = 0,
    APP_SD_S812_HANGUP_SENT,
    APP_SD_S812_SWITCH_NOT_SENT,
    APP_SD_S812_SWITCH_SENT,
    APP_SD_S812_CONFERENCE_NOT_SENT,
    APP_SD_S812_CONFERENCE_SENT,
    APP_SD_S812_MICROMUTE_NOT_SENT,
    APP_SD_S812_MICROMUTE_SENT,
    APP_SD_S812_HANGUP_BOTH_NOT_SENT,
    APP_SD_S812_HANGUP_BOTH_SENT,
    APP_SD_S812_HEADSET_NOT_SENT,
    APP_SD_S812_HEADSET_SENT
}APP_SD_enSubStateBtnS812;

typedef enum
{
    APP_SD_SC24_HANGUP_NOT_SENT = 0,
    APP_SD_SC24_HANGUP_SENT,
    APP_SD_SC24_SWITCH_NOT_SENT,
    APP_SD_SC24_SWITCH_SENT,
    APP_SD_SC24_CONFERENCE_NOT_SENT,
    APP_SD_SC24_CONFERENCE_SENT,
    APP_SD_SC24_MICROMUTE_NOT_SENT,
    APP_SD_SC24_MICROMUTE_SENT
}APP_SD_enSubStateBtnSC24;



typedef enum
{
    APP_SD_S815_HANGUP_NOT_SENT = 0,
    APP_SD_S815_HANGUP_SENT,
    APP_SD_S815_MICROMUTE_NOT_SENT,
    APP_SD_S815_MICROMUTE_SENT,
    APP_SD_S815_HEADSET_NOT_SENT,
    APP_SD_S815_HEADSET_SENT,
    APP_SD_S815_HOLD_NOT_SENT,
    APP_SD_S815_HOLD_SENT
}APP_SD_enSubStateBtnS815;

typedef enum
{
    APP_SD_S901_ACCEPT_NOT_SENT = 0,
    APP_SD_S901_ACCEPT_SENT,
    APP_SD_S901_HANGUP_NOT_SENT,
    APP_SD_S901_HANGUP_SENT,
    APP_SD_S901_MUTE_NOT_SENT,
    APP_SD_S901_MUTE_SENT,
    APP_SD_S901_HEADSET_NOT_SENT,
    APP_SD_S901_HEADSET_SENT
}APP_SD_enSubStateBtnS901;

typedef enum
{
    APP_SD_S902_ACCEPT_NOT_SENT = 0,
    APP_SD_S902_ACCEPT_SENT,
    APP_SD_S902_HANGUP_NOT_SENT,
    APP_SD_S902_HANGUP_SENT,
    APP_SD_S902_RELEASE_ACCEPT_NOT_SENT,
    APP_SD_S902_RELEASE_ACCEPT_SENT
//RT:3133
//    APP_SD_S902_HOLD_NOT_SENT,
//    APP_SD_S902_HOLD_SENT
}APP_SD_enSubStateBtnS902;

typedef enum
{
    APP_SD_S933_REJECT_NOT_SENT = 0,
    APP_SD_S933_REJECT_SENT
}APP_SD_enSubStateBtnS933;

typedef enum
{
    APP_SD_S941_ACCEPT_NOT_SENT = 0,
    APP_SD_S941_ACCEPT_SENT
}APP_SD_enSubStateBtnS941;

typedef enum
{
    APP_SD_S942_HANGUP_NOT_SENT = 0,
    APP_SD_S942_HANGUP_SENT
}APP_SD_enSubStateBtnS942;

typedef enum
{
    APP_SD_S945_HANGUP_NOT_SENT = 0,
    APP_SD_S945_HANGUP_SENT
}APP_SD_enSubStateBtnS945;

typedef enum
{
    APP_SD_enS802_DIAL_ERROR = 0
}APP_SD_S802_ErrorType;    

typedef enum
{
    APP_SD_UHV_NOT_IN_ASG_VIEW = 0,
    APP_SD_UHV_IN_ASG_VIEW,
    APP_SD_WAIT_BACK_TO_ASG_MENU,
    APP_SD_START_BACK_TO_ASG_MENU,
    APP_SD_UHV_ACTIVE_TRIGGER_NOT_IN_ASG_VIEW
}APP_SD_enContextState;

/*----------------------------------------------------------------------------*/
/* module internal structs                                                    */
/*----------------------------------------------------------------------------*/

typedef struct /* used for mailbox texts */
{
    const Uint8Type * pu8PromptsToDisplay;
    Uint16Type        u16LengthOfPrompt;
}APP_SD_ST_LOOKUP_TABLE_MAILBOX_PROMPTS;



typedef struct
{
    Uint16Type         item_id[VCR_MAXNUM_RES_STORE];
    Uint8Type          num_ids;   /* number of available ids  */
}AppSdResultContactAmbType;

typedef struct
{
    Uint16Type         pbIdx;
}AppSdResultContactUniType;


typedef struct
{
    APP_SD_enEventId        type;
}AppSdCommonEventType;

typedef struct
{
    AppSdCommonEventType    common;
    ERMNotificationType     notId;       
}AppSdErmEventType;


typedef struct
{
    AppSdCommonEventType        common;
    APP_DEVICE_enNotificationId notId;       
}AppSdDeviceEventType;


typedef struct
{
    AppSdCommonEventType    common;
    ERMStateType            state;       
}AppSdErmStateEventType;

typedef struct
{
    AppSdCommonEventType    common;
    VCRNotificationIdEnum   notId;
    union
    { 
        AppSdResultContactAmbType contactAmb;      
        AppSdResultContactUniType contactUni;
    }                       data;
}AppSdVcrEventType;

typedef struct
{
    AppSdCommonEventType        common;
    APP_PHONE_enNotificationId  notId;       
}AppSdPhoneEventType;

typedef struct
{
    AppSdCommonEventType            common;
    APP_PB_enNotificationId  notId;       
}AppSdPhoneBookEventType;

typedef struct
{
    AppSdCommonEventType        common;
    APP_SMS_enNotificationId    notId;       
}AppSdSmsEventType;

typedef struct
{
    AppSdCommonEventType            common;
    APP_CSHDL_NotificationType      notId;       
}AppSdCshdlEventType;

typedef struct
{
    AppSdCommonEventType            common;
    LOGI_BUT_MAN_ENUM_BUTTON_NAMES  id;
    LOGI_BUT_MAN_ENUM_PRESSTYPE     pressType;
}AppSdBtnEventType;

typedef struct
{
    AppSdCommonEventType        common;
    APP_SD_enContextId          ctxId;  
}AppSdTimeoutEventType;

typedef struct
{
    AppSdCommonEventType        common;
    APP_CAN_enLang              newCanLang;
}AppSdCanLangEventType;

typedef struct
{
    AppSdCommonEventType        common;
    SPEED_enSpeed               notId;
}AppSdCanSpeedEventType;


typedef struct
{
    AppSdCommonEventType    common;
    KPB_enNotificationId    notId;       
}AppSdKpbEventType;


typedef union
{
    AppSdCommonEventType    common;
    AppSdBtnEventType       btn;
    AppSdErmStateEventType  ermState;
    AppSdErmEventType       erm;
    AppSdDeviceEventType    device;
    AppSdVcrEventType       vcr;
    AppSdPhoneEventType     phone;
    AppSdPhoneBookEventType phonebook;
    AppSdSmsEventType       sms;
    AppSdTimeoutEventType   timeout;
    AppSdCanLangEventType   canLang;
    AppSdCanSpeedEventType  canSpeed;
    AppSdKpbEventType       kpb;
    AppSdCshdlEventType     cshdl;
}AppSdEventCtxType;

typedef struct AppSdEventQueueStruct
{
    AppSdEventCtxType event;
    struct AppSdEventQueueStruct *next;
}AppSdEventQueueType;


//typedef struct
//{
//    KPB_enFrameIdType   frameId;
//}AppSdStaticFrameInfoType;

/* Context structs */
    // backup
//typedef struct
//{
//    APP_SD_enContextId          ctxId;
//    APP_SD_enPendingType        pendingType;
//    AppSdFrameDataType          frameData;
//    APP_SD_enSimpleCtxState     state;
//}AppSdCommonCtxType;

typedef struct
{
    APP_SD_enContextId          ctxId;
    APP_SD_enPendingType        pendingType;
    APP_SD_enSimpleCtxState     state;
    BooleanType                 bActivation;
    AppSdFrameDataListCommon    listData;
    AppSdFrameDataType          *pFrameData;
    Uint32Type                  timeout;
    // this is used to set, whether a frame should be replaced 
    // when another frame overlays it ( no matter if this is the
    // correct next frame or an asynchronous popup 
    BooleanType                 bReplaceFrame;
}AppSdCommonCtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enContextId          lastCtxId;
}AppSdBaseCtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateS001       subState;
    Uint16Type                  connectAttempt;
    BooleanType                 abort;
    BooleanType                 abortPressed;
}AppSdS001CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    BooleanType                 playerAvail;
}AppSdS002CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    BooleanType                 messagesAvail;
}AppSdS101CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnSC20    subStateBtn;
    ATCallIndexEnum             plccIndex;
    ATCallConfTypeEnum          confState;         // RT #3554
}AppSdSC20CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS104    subStateBtn;
    APP_SD_enContextId          initiator;
    Uint8Type                   searchString[APP_SD_SEARCH_STR_SIZE];
    Uint8Type                   searchStringSize;
    Uint16Type                  pbSize;
    LOGI_BUT_MAN_ENUM_BUTTON_NAMES             searchDirection;
    Uint16Type                  selectedIndex;
}AppSdS104CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    Uint16Type                  pbIndex;
    Uint8Type                   pnSize;
}AppSdS105CtxType;

//typedef struct
//{
//    AppSdCommonCtxType          common;
//    Uint16Type                  pbIndex;
//    Uint16Type                  pnIndex;
//}AppSdS111CtxType;

typedef struct
{
    AppSdCommonCtxType              common;
    APP_SD_enSubStateS117           subState;
    Uint16Type                      pbIndex;
    BooleanType                     bVoiceTagAvailable;
    APP_SD_enLastSelectedMenueS117  lastSelMenue;
}AppSdS117CtxType;

typedef AppSdS117CtxType AppSdS116CtxType;
typedef AppSdS117CtxType AppSdS114CtxType;

typedef struct
{
    AppSdCommonCtxType              common;
    // we use substate defined for s117
    APP_SD_enSubStateS115           subState;
    Uint16Type                      pbIndex;
    BooleanType                     bVoiceTagAvailable;
    // we use selected menue enum defined for s117
    APP_SD_enLastSelectedMenueS115  lastSelMenue;
    BooleanType                     deletePressed;
}AppSdS115CtxType;


typedef struct
{
    AppSdCommonCtxType              common;
    APP_PHONEBOOK_enSelectedPbType  pbType;
    Uint16Type                      pbSize;
}AppSdS211CtxType;

typedef struct
{
    AppSdCommonCtxType              common;
    APP_PHONEBOOK_enSelectedPbType  pbType;
    Uint16Type                      pbIndex;
}AppSdS212CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    Uint16Type                  smsIndex;
}AppSdS213CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AppSdScSpellerCtrlType      spellerCtrl;
}AppSdS305CtxType;


typedef struct
{
    AppSdCommonCtxType                                  common;
    AppSdScSpellerCtrlType                              spellerCtrl;
    APP_SD_enSubStateBtnS306                            subStateBtn;
    APP_SD_enSubStateS306                               subState;
    Uint16Type                                          curKeywordIdx;
    Uint16Type                                          entryToBeRequested;    
    const APP_SD_ST_LOOKUP_TABLE_MAILBOX_PROMPTS *      curKeywordArray;
    LOGI_BUT_MAN_ENUM_BUTTON_NAMES                      buttonId;
}AppSdS306CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS404    subStateBtn;
    Uint16Type                  numSms;
}AppSdS404CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS405    subStateBtn;
    Uint16Type                  smsIndex;
    ATPhoneNumberType           origin;
    Uint16Type                  origin_len;
}AppSdS405CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateS406       subState;
    Uint16Type                  smsIndex;
    ATSmsTypeEnum               smsType;
}AppSdS406CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateS407       subState;
}AppSdS407CtxType;

typedef struct
{
    AppSdCommonCtxType              common;
    APP_SD_enSubStateS408           subState;
    APP_SD_enSubStateBtnS408        subStateBtn;
    APP_DEVICE_DeviceType           deviceType;
    AT_DeviceId                     deviceId;
    ATBTFeatureSupportResultEnum    sspSupported;
    Uint8Type                       retryCounter;
}AppSdS408CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS413    subStateBtn;
    Uint16Type                  smsIndex;
}AppSdS413CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    Uint16Type                  smsIndex;
}AppSdSC14CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    BooleanType                 playerAvail;
}AppSdS502CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    Uint8Type                   numDevs;
    APP_SD_enSubStateBtnS512    subStateBtn;
    AT_DeviceId                 deviceId;
}AppSdS512CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    Uint8Type                   numDevs;
    APP_DEVICE_DeviceType       deviceType;
    ATRspGetInquiryResType      inqData;
}AppSdS515CtxType;

typedef AppSdS512CtxType AppSdS541CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    Uint8Type                   numDevs;
}AppSdS567CtxType;

typedef AppSdS567CtxType AppSdS5B2CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS51D    subStateBtn;
    AppSdScSpellerCtrlType      spellerCtrl;
    Uint8Type                   deviceId;
    ATPairingDeviceType         remoteDeviceType;
    APP_DEVICE_DeviceType       deviceType;    
}AppSdS51DCtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    // to start inquiry during init
    APP_SD_enSubStateS520       subState;
    // to cancel inquiry
    APP_SD_enSubStateBtnS520    subStateBtn;
    BooleanType                 inquiryEndRcvd;
}AppSdS520CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enContextId          lastCtxId;
    // to retry if MDI state change not 
    //APP_SD_enSubStateS522       subState;
    APP_SD_enMdiMuteReason      enMuteReason;    
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;    
    BooleanType                 bConnect;
}AppSdS522CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS52C    subStateBtn;
    APP_SD_enSubStateS52C       subState;
    AppSdScSpellerCtrlType      spellerCtrl;
    Uint8Type                   deviceId;
    ATPairingDeviceType         pairingDeviceType;
    APP_DEVICE_DeviceType       appDeviceType;
}AppSdS52CCtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateS52D       subState;
    Uint8Type                   deviceId;
    APP_DEVICE_DeviceType       appDeviceType;
}AppSdS52DCtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    // to start inquiry during init
    APP_SD_enSubStateS581       subState;
    // to cancel inquiry
    APP_SD_enSubStateBtnS581    subStateBtn;
    BooleanType                 inquiryEndRcvd;
    
}AppSdS581CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AppSdResultContactAmbType   contactAmb;
}AppSdSC01CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    // to start inquiry during init
    APP_SD_enSubStateSC13       subState;
    // to cancel inquiry
    APP_SD_enSubStateBtnSC13    subStateBtn;
    BooleanType                 inquiryEndRcvd;
}AppSdSC13CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AppSdScSpellerCtrlType      spellerCtrl;
    AT_DeviceId                 deviceId;
    APP_SD_enSubStateBtnS536    subStateBtn;
}AppSdS536CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    AppSdScSpellerCtrlType      spellerCtrl;
    AT_DeviceId                 deviceId;
    APP_SD_enSubStateBtnS535    subStateBtn;
}AppSdS535CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    BooleanType                 bIsActive;    
    APP_SD_enSubStateBtnS542    subStateBtn;
}AppSdS542CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AppSdScSpellerCtrlType      spellerCtrl;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    APP_SD_enSubStateBtnS575    subStateBtn;
}AppSdS575CtxType;


typedef struct 
{
    AppSdCommonCtxType          common;
    AppSdScSpellerCtrlType      spellerCtrl;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    APP_SD_enSubStateBtnS580    subStateBtn;
}AppSdS580CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;    
    APP_SD_enSubStateBtnS563    subStateBtn;
}AppSdS563CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;    
    BooleanType                 isInquiryId;    
    APP_SD_enSubStateBtnS572    subStateBtn;
}AppSdS572CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;    
    //APP_SD_enSubStateBtnS542    subStateBtn;
}AppSdS571CtxType;



typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
}AppSdS543CtxType;

typedef AppSdS543CtxType AppSdS573CtxType;
typedef AppSdS543CtxType AppSdS574CtxType;
typedef AppSdS543CtxType AppSdS934CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_SD_enSubStateS534       subState;    
    BooleanType                 addPlayerCbRcvd;    
}AppSdS534CtxType;


typedef struct
{
    AppSdCommonCtxType          common;
    // trigger connect of phone service of deviceId during init 
    APP_SD_enSubStateS517       subState;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;    
}AppSdS517CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
//    APP_DEVICE_DeviceType       deviceType;    
}AppSdS518CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_DEVICE_DeviceType       deviceType;    
}AppSdS565CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    BooleanType                 bIsActive;    
    AT_DeviceId                 deviceId;
    AT_BTDeviceName             btName;
    Uint16Type                  btName_len;    
}AppSdS514CtxType;    

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    AT_BTDeviceName             btName;
    Uint16Type                  btName_len;    
}AppSdS578CtxType;    



typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    AT_BTDeviceName             btName;
    Uint16Type                  btName_len;    
    APP_SD_enSubStateBtnS513    subStateBtn;
    BooleanType                 deletePressed;
}AppSdS513CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    AT_BTDeviceName             btName;
    Uint16Type                  btName_len;    
    APP_SD_enSubStateBtnS555    subStateBtn;
    BooleanType                 deletePressed;
}AppSdS555CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    AT_BTDeviceName             btName;
    Uint16Type                  btName_len;    
    APP_SD_enSubStateBtnS576    subStateBtn;
    BooleanType                 deletePressed;
}AppSdS576CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    AT_BTDeviceName             btName;
    Uint16Type                  btName_len;    
    APP_SD_enSubStateBtnS579    subStateBtn;
    BooleanType                 deletePressed;
}AppSdS579CtxType;



typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS556    subStateBtn;
    AppSdScSpellerCtrlType      spellerCtrl;
}AppSdS556CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
}AppSdS597CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    ATRintoneModeEnum           ringtone;
    BooleanType                 errorOccured;
}AppSdS642CtxType;



typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS661    subStateBtn;
}AppSdS661CtxType;



typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS669    subStateBtn;
    Uint8Type                   sortOrder;
}AppSdS669CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateSC16       subState;
    APP_SD_enSubStateBtnSC16    subStateBtn;
    BooleanType                 bPlayCallerName;
}AppSdSC16CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS602    subStateBtn;
    APP_SD_enSubStateS602       subState;
    ERMUserAudioModeType        audioMode;
}AppSdS602CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    BooleanType                 bAuthNeeded;
    APP_SD_enSubStateBtnS577    subStateBtn;
}AppSdS577CtxType;



typedef struct
{
    AppSdCommonCtxType              common;
    APP_SD_enSubStateS801           subState;
    APP_SD_enSubStateBtnS801        subStateBtn;
    APP_PHONEBOOK_enSelectedPbType  pbType;
    Uint16Type                      pbIndex;
    Uint16Type                      pnIndex;
    ATPhoneNumberType               number;
    Uint8Type                       number_len;
    ATCallIndexEnum                 plccIndex;
}AppSdS801CtxType;

typedef struct
{
    AppSdCommonCtxType              common;
    APP_SD_S802_ErrorType           error;
}AppSdS802CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS803    subStateBtn;
    ATCallIndexEnum             plccIndex;
    BooleanType                 bholdSupported;
}AppSdS803CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS804    subStateBtn;
    AppSdScSpellerCtrlType      spellerCtrl;
}AppSdS804CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS811    subStateBtn;
    ATCallIndexEnum             plccIndex;
}AppSdS811CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS812    subStateBtn;
    ATCallIndexEnum             activePlccIdx;
    ATCallIndexEnum             heldPlccIdx;
}AppSdS812CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnSC24    subStateBtn;
    ATCallIndexEnum             activePlccIdx;
    ATCallIndexEnum             outgoingPlccIdx;
}AppSdSC24CtxType;



typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS815    subStateBtn;
}AppSdS815CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS901    subStateBtn;
    ATCallIndexEnum             plccIndex;
    BooleanType                 muteActivated;
}AppSdS901CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS902    subStateBtn;
    ATCallIndexEnum             activePlccIdx;
    ATCallIndexEnum             waitingPlccIdx;
}AppSdS902CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
}AppSdS914CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    ATPhoneNumberType           number;
    Uint16Type                  number_len;
}AppSdS929CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS933    subStateBtn;
    BooleanType                 pprsFailed;
}AppSdS933CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS941    subStateBtn;
    ATCallIndexEnum             plccIndex;
}AppSdS941CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    APP_SD_enSubStateBtnS942    subStateBtn;
    ATCallIndexEnum             plccIndex;
}AppSdS942CtxType;

typedef struct
{
    AppSdCommonCtxType              common;
    APP_SD_enSubStateS945           subState;
    APP_SD_enSubStateBtnS945        subStateBtn;
    APP_PHONEBOOK_enSelectedPbType  pbType;
    Uint16Type                      pbIndex;
    Uint16Type                      pnIndex;
    ATPhoneNumberType               number;
    Uint8Type                       number_len;
}AppSdS945CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
}AppSdSC12CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
}AppSdSC17CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    AT_DeviceId                 deviceId;
}AppSdS007CtxType;



typedef AppSdBaseCtxType AppSdS_18CtxType;
typedef AppSdBaseCtxType AppSdS103CtxType;
typedef AppSdBaseCtxType AppSdS663CtxType;

typedef struct
{
    AppSdCommonCtxType          common;
    BooleanType                 bVersion;
}AppSdS601CtxType;

typedef union
{
    AppSdCommonCtxType  common;
    AppSdS001CtxType    s001;
    AppSdS002CtxType    s002;
    AppSdS007CtxType    s007;
    AppSdS_18CtxType    s_18;
    AppSdSC20CtxType    sc20;
    AppSdS101CtxType    s101;
    AppSdS103CtxType    s103;
    AppSdS104CtxType    s104;
    AppSdS105CtxType    s105;
//    AppSdS111CtxType    s111;
    AppSdS114CtxType    s114;
    AppSdS115CtxType    s115;
    AppSdS116CtxType    s116;
    AppSdS117CtxType    s117;
    AppSdS211CtxType    s211;
    AppSdS212CtxType    s212;
    AppSdS213CtxType    s213;
    AppSdS305CtxType    s305;
    AppSdS306CtxType    s306;
    AppSdS404CtxType    s404;
    AppSdS405CtxType    s405;
    AppSdS406CtxType    s406;
    AppSdS407CtxType    s407;
    AppSdS408CtxType    s408;
    AppSdS413CtxType    s413;
    AppSdSC14CtxType    sc14;
    AppSdS502CtxType    s502;
    AppSdS512CtxType    s512;
    AppSdS513CtxType    s513;
    AppSdS514CtxType    s514;
    AppSdS515CtxType    s515;
    AppSdS517CtxType    s517;
    AppSdS518CtxType    s518;
    AppSdS51DCtxType    s51D;
    AppSdS520CtxType    s520;
    AppSdS522CtxType    s522;
    AppSdS52CCtxType    s52C;
    AppSdS52DCtxType    s52D;
    AppSdS534CtxType    s534;
    AppSdS535CtxType    s535;
    AppSdS536CtxType    s536;
    AppSdS541CtxType    s541;
    AppSdS542CtxType    s542;
    AppSdS543CtxType    s543;
    AppSdS555CtxType    s555;
    AppSdS556CtxType    s556;
    AppSdS563CtxType    s563;
    AppSdS565CtxType    s565;
    AppSdS567CtxType    s567;
    AppSdS5B2CtxType    s5B2;
    AppSdS571CtxType    s571;
    AppSdS572CtxType    s572;
    AppSdS573CtxType    s573;
    AppSdS574CtxType    s574;
    AppSdS575CtxType    s575;
    AppSdS576CtxType    s576;
    AppSdS577CtxType    s577;
    AppSdS578CtxType    s578;
    AppSdS579CtxType    s579;
    AppSdS580CtxType    s580;
    AppSdS581CtxType    s581;
    AppSdS597CtxType    s597;
    AppSdS601CtxType    s601;
    AppSdS602CtxType    s602;
    AppSdS642CtxType    s642;
    AppSdS661CtxType    s661;
    AppSdS663CtxType    s663;
    AppSdS669CtxType    s669;
    AppSdS801CtxType    s801;
    AppSdS802CtxType    s802;
    AppSdS803CtxType    s803;
    AppSdS804CtxType    s804;
    AppSdS811CtxType    s811;
    AppSdS812CtxType    s812;
    AppSdS815CtxType    s815;
    AppSdS901CtxType    s901;
    AppSdS902CtxType    s902;
    AppSdS914CtxType    s914;
    AppSdS929CtxType    s929;
    AppSdS933CtxType    s933;
    AppSdS934CtxType    s934;
    AppSdS941CtxType    s941;
    AppSdS942CtxType    s942;
    AppSdS945CtxType    s945;
    AppSdSC01CtxType    sc01;
    AppSdSC12CtxType    sc12;
    AppSdSC13CtxType    sc13;
    AppSdSC16CtxType    sc16;
    AppSdSC17CtxType    sc17;
    AppSdSC24CtxType    sc24;
} AppSdCtxCtrlType; 

typedef struct
{
    BooleanType             bPending;
    APP_DEVICE_DeviceType   profileType;
    AT_DeviceId             deviceId;
}AppSdPendingProfileType;


typedef struct
{
    AT_DeviceId                 deviceId;
    APP_DEVICE_DeviceType       deviceType;
    BooleanType                 connect;
    APP_SD_enChooseState        state;
    APP_SD_enMDIState           playerState;
}AppSdSaveCtxType;

typedef struct
{
//    /* holds the ctxId that currently belongs to a frameId */
//    APP_SD_enContextId          frameTypeCtxId[KPB_FRAME_ID_7+1];    
    AppSdCtxCtrlType                ctxHistory[APP_SD_MAX_CTX_DEPTH];
    Uint16Type                      curCtxCtrlId;
    AppSdCtxCtrlType                *pCurCtxCtrl;
    AppSdEventQueueType             *pEventQueue;
    AppSdBtnEventType               btnEvent;
    AppSdPhoneEventType             callStateEvent;     // use for call state changed events only!
    AppSdPendingProfileType         pendingProfile;
    BooleanType                     manualPbSyncInProgress;    
    CONST_U8_PTR_CONST_T            *prompts;
    const Uint16Type                *prompts_len;
    APP_SD_ENUM_LANGUAGES           curLang;
    APP_SD_enContextId              ctxTimedOut;
    BooleanType                     bHeadSetActivated;
    APP_SD_enContextState           ctxInView;
    BooleanType                     activate;	
    BooleanType                     bSmsDelete;         //RT #5003
    BooleanType                     callStateIsActive;
    BooleanType                     preCallStateIsActive;
    BooleanType                     bPbUpdateComplete;
    BooleanType                     bManualPbUpdateFinished;
    Uint8Type                       connectCounter;
    Uint8Type                       connectCounter2;
    Uint8Type                       disconnectPlayerCounter;
    Uint8Type                       disconnectPhoneCounter;
    Uint8Type                       addProfileCounter;
    Uint8Type                       renameCounter;
    Uint8Type                       deleteDeviceCounter;
    Uint8Type                       setMailboxCounter;
    Uint8Type                       setRingtoneCounter;
    Uint8Type                       saveBtNameCounter;
    BooleanType                     disconnectPlayerWillFollow;
    APP_SD_enSubStateSC12           sc12SubState;
    AT_DeviceId                     sc12DeviceId;
    Uint8Type                       sc12profileId;
    Uint8Type                       sc12Counter;
    APP_SD_enContextId              searchStartedFromScreen;
    Uint8Type                       disconnectRequestedByUser;
    BooleanType                     outgoingCallViaVcrInProgress;
    BooleanType                     dialing_was_initiated_by_vcrOnceFlag;
    APP_SD_enContextId              deleteActionPressedinScreen;
    AT_DeviceId                     s408DelDeviceId;
    AT_BTDeviceName                 s408DelDeviceName;
    Uint16Type                      s408DelDeviceNameLength;
    APP_DEVICE_DeviceType           s408DelDeviceType;
    BooleanType                     ermStartupDone;
    //AppSdSaveCtxType                lastConnectionDevice;
    BooleanType						blastRingtoneindication;//RT14853
    BooleanType                     bDelepairingonlyondevice;//RT13844
}AppSdContext;



/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief  sends a BAP Error message with the given BAP fctId and errorCode	
*/

void _APP_SD_vEventManager(const AppSdEventCtxType *pEvent);
void _APP_SD_vSetCtxHistoryState(APP_SD_enContextState ctxHisState, Uint32Type line);
BooleanType _APP_SD_bIsKl15On(void);

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
//#define sqrt(x) ((x)*(x))


#define APP_SD_HANDLER_EXISTS(table, ctxId) \
    ( (ctxId < NUM_ELEMENTS_OF_ARRAY(table)) && \
      (table[ctxId] != NULL) )


#define CCASSERT(predicate, table_name) \
typedef Uint8Type constraint_violated_on_line_##table_name[(predicate)!=0]

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the #define
*/
//#define	EXAMPLE_DEFINE		1


#endif /* _APP_SD_CI_H */

/* End Of File APP_SD_CI.h */
