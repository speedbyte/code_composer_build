/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler.h
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-09-21 09:41:46 +0200 (Mi, 21 Sep 2011) $
* $Rev: 21861 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/inc/VCR_StateHandler.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _VCR_STATE_HANDLER_H
#define _VCR_STATE_HANDLER_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "APP_PHONEBOOK.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "VCR.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* VCR Private Types                                                              */
/*----------------------------------------------------------------------------*/

/* VCR internal events for menu handling */
typedef enum {
    VCR_MEVT_UNDEF              = 0,
    VCR_MEVT_SIVR_SUPPORTED     = 1,
    VCR_MEVT_SIVR_STARTED       = 2,
    VCR_MEVT_SIVR_STOPPED       = 3,
    VCR_MEVT_LANGNUM_AVAIL      = 4,
    VCR_MEVT_LANGMAP_AVAIL      = 5,
    VCR_MEVT_CUR_LANG_AVAIL     = 6,
    VCR_MEVT_CUR_LANG_CHANGED   = 7,
    VCR_MEVT_LANG_CHANG_REG     = 8,
    VCR_MEVT_STRING_PRMT_OK     = 9,
    VCR_MEVT_START_PROMTP_END   = 10,
    VCR_MEVT_STOP_PROMPT_END    = 11,
    VCR_MEVT_CONNECT_PROMTP_END = 12,
    VCR_MEVT_PROC_ERROR         = 13,
    VCR_MEVT_PTT                = 14,
    VCR_MEVT_CALL               = 15,
    VCR_MEVT_OK                 = 16,
    VCR_MEVT_VRSTARTED          = 17,
    VCR_MEVT_DIAL_OK            = 18,
    /* CK5050 is down - go back to idle state */
    VCR_MEVT_CK5050_DOWN        = 19,
    VCR_MEVT_ON_MUTE_DELAY      = 20,
    VCR_MEVT_MDI_PAUSED         = 21,         // -> not used anymore -> Audiomanager do this Job now
    VCR_MEVT_PREP_OK            = 22,
    VCR_MEVT_REDIAL_PROMPT_END  = 23,
    VCR_MEVT_REDIAL_DONE        = 24,
    VCR_MEVT_ERROR              = 25,
    VCR_MEVT_PBENTRY_DATA_AVAIL = 26,
    VCR_MEVT_APP_TRIG_PB_QUERY  = 27,
    VCR_MEVT_READ_SMS           = 28,
    VCR_MEVT_SMS_INFO_AVAIL     = 29,
    VCR_MEVT_MANUAL_PB_SYNC     = 30,
    VCR_MEVT_MANUAL_PB_SYNC_OK  = 31,
    VCR_MEVT_DELAY_LIST_READ    = 32,
    VCR_MEVT_STR_PRMT_END_PTT   = 33,
    VCR_MEVT_STR_PRMT_END_CALL  = 34,
    VCR_MEVT_STR_PRMT_END_OK    = 35,
    VCR_MEVT_PRMT_END_RESTART   = 36,
    VCR_MEVT_APP_TRIGGER        = 37,
    VCR_MEVT_NAME_RECORDED      = 38,
    VCR_MEVT_NAME_RECORD_FAIL   = 39,
    VCR_MEVT_NAME_RECORD_FAIL_SHORT        = 40,
    VCR_MEVT_NAME_RECORD_FAIL_NOT_UNIQUE   = 41,
    VCR_MEVT_VT_ADD_DONE        = 42,
    VCR_MEVT_VT_DELETE_DONE     = 43,
    VCR_MEVT_VT_QUERY_DONE      = 44,
    VCR_MEVT_HAPTIC             = 45,
    VCR_MEVT_INCOMING_CALL      = 46,
    VCR_MEVT_REC_TRAINING_INFO  = 47,
    VCR_MEVT_TRAINING_REJECT    = 48,
    VCR_MEVT_TRAINING_ACCEPT    = 49,
    VCR_MEVT_TRAINING_FINISH    = 50,
    VCR_MEVT_ADDCONTACT_NUMBER  = 51,
    VCR_MEVT_SELECT_ENTRY       = 52,
    VCR_MEVT_MARK_SMS_READ      = 53,
    VCR_MEVT_ASR_RETRIGGER      = 54,
    VCR_MEVT_OUTGOING_CALL      = 55,
    VCR_MEVT_CK5050_POWER_OFF   = 56,
    VCR_MEVT_REQUEST_MUTE       = 57,      // new Audiomanager-Events
    VCR_MEVT_WAIT_MUTE_ACK      = 58,
    VCR_MEVT_MUTE_ACK_START_ENGINE = 59,
    VCR_MEVT_VT_DELETE_ALL_DONE = 60,
    VCR_MEVT_APP_TRIG_PB_SEARCH_ID =  61,
    VCR_MEVT_APP_PB_ID_AVAIL   =  62,
    VCR_MEVT_CALL_PB_SELECTION   =  63,
    VCR_MEVT_MEDIAPAYER         = 64

} VCRMenuEventEnum;


/* VCR internal transition messages to be send */
typedef enum {
    VCR_MSG_NONE                      = 0,
    VCR_MSG_GET_LANG_NUM              = 1,
    VCR_MSG_GET_LANG_MAP              = 2,
    VCR_MSG_GET_CURRENT_LANG          = 3,
    VCR_MSG_SET_CURRENT_LANG          = 4,
    VCR_MSG_PLAY_PROMPT               = 5,
    VCR_MSG_PLAY_REDIAL_PROMPT        = 6,
    VCR_MSG_PLAY_SYSSTART_PROMPT      = 7,
    VCR_MSG_PLAY_SYSSTOP_PROMPT       = 8,
    VCR_MSG_START_ENGINE              = 9,
    VCR_MSG_STOP_ENGINE               = 10,
    VCR_MSG_PREPARE_MENU              = 11,
    VCR_MSG_START_RECO                = 12,
    VCR_MSG_DIAL                      = 13,
    VCR_MSG_REDIAL                    = 14,
    VCR_MSG_HELP_MENU                 = 15,
    VCR_MSG_PLAY_PROMPT_MENU          = 16,
    VCR_MSG_PLAY_CONNECT_PROMPT       = 17,
    VCR_MSG_STOP_STR_PRMT_PTT         = 18,
    VCR_MSG_STOP_STR_PRMT_CALL        = 19,
    VCR_MSG_STOP_STR_PRMT_OK          = 20,
    VCR_MSG_STOP_STR_PRMT_RESTART     = 21,
    VCR_MSG_DELETE_CALL_STACKS        = 22,
    VCR_MSG_NAME_VOICE_TAG_ADD        = 23,
    VCR_MSG_NAME_VOICE_TAG_DELETE     = 24,
    VCR_MSG_NAME_VOICE_TAG_QUERY      = 25,
    VCR_MSG_STOP_RTPROMPT             = 26,
    VCR_MSG_RESTART_PROMPT_RECO       = 27,
    VCR_MSG_RESTART_PROMTP_NORECO     = 28,
    VCR_MSG_NAME_VOICE_TAG_DELETE_ALL = 29,
    VCR_MSG_MUSIC_SYSSTART_PROMPT     = 30,
    VCR_MSG_INFO_SYSSTART_PROMPT      = 31, //RT13214
    VCR_NUM_MESSAGES                    /* must bee the last entry in enum! */
}VCRSystemMsgEnum;

typedef enum {
    VCR_SYSTEM_NOT_AVAILABLE  = 0,
    VCR_SYSTEM_AVAILABLE      = 1,
    VCR_SYSTEM_STARTED_LANG   = 2,
    VCR_SYSTEM_STARTED_PTT    = 3,
    VCR_SYSTEM_STARTED_HAPTIC = 4,
    VCR_SYSTEM_STOP_PTT       = 5,
    VCR_SYSTEM_STOP_LANG      = 6,
    VCR_SYSTEM_FLASHERASE_IPR = 7,
    VCR_SYSTEM_STOP_VTRECORD  = 8
}VCRSystemStateEnum;



typedef enum  {
    VCR_SUP_RETRIG    = 0,
    VCR_SUP_OK        = 1,
    VCR_SUP_ASYNC     = 2
} VCRSupervisionEnum;

typedef struct {
    ATSIVRPromptEnum tab[VCR_MAX_NUM_PROMPTS];
}VCRPrompTable;

typedef struct {
    VCRResultTypeEnum  type;   /* = VCR_RESULT_TYPE_NUMBER */
    BooleanType        is_for_contact; /* indicate if number is for contact or dial */
    BooleanType        stop_for_contact;
    Uint16Type         voice_tag_idx;
    Uint16Type         contact_idx;
    Uint16Type         number_startRead; /*without the last DBlock*/
    Uint16Type         number_len;
    ATPhoneNumberType  number;
}VCRResultNumberType;


typedef struct {
    VCRResultTypeEnum   type;   /* = VCR_RESULT_TYPE_LANGUAGE */
    ATSIVRLanguagesEnum value;
}VCRResultLanguageType;

typedef struct {
    VCRResultTypeEnum   type;   /* = VCR_RESULT_TYPE_READSMS */
    BooleanType         is_first_read_sms; /* indicates first entry in read sms menu*/
    BooleanType         is_haptic_trigger; /* indicate if this was called due to haptic in idle */
    Uint16Type          total_num_sms;   /* amount of available sms */
    Uint16Type          unread_sms;      /* amount of unread sms */
    Uint16Type          read_sms;        /* amount of read sms */
    Uint16Type          current_num_sms; /* current sms number */
    /* indicate number of unread sms that are read by VCR and are marked read */

    /* information to call the SMS origin */
    ATSmsOrigin         origin;
    Uint16Type          origin_len;
    ATSmsString         message;
    Uint16Type          message_len;
    Uint16Type          offset_len;
}VCRResultReadSMSType;


typedef struct {
    VCRResultTypeEnum   type;   /* = VCR_RESULT_TYPE_BT_PIN */
    ATPinCodeBT         pin;
    Uint8Type           pin_len; /* len of pin code, pin must be 4 digits!!! */
    Uint16Type          u16Pin;  /* pin as 16 bit value for calling ERM */
}VCRResultBTPinType;

typedef struct {
    VCRResultTypeEnum   type;   /* = VCR_RESULT_TYPE_BT_PIN */
    ATRintoneModeEnum   cur_ringtone;
}VCRResultRingtoneType;

typedef struct {
    VCRResultTypeEnum   type;   /* = VCR_RESULT_TYPE_PLAYPB */
    BooleanType         is_haptic_trigger;
    BooleanType         is_add_new_contact;
    Uint16Type          num_pb_entries;
    Uint16Type          cur_entry;
    BooleanType         name_avail;
    BooleanType         voice_tag_avail;
    BooleanType         is_vt_overwrite;
    Uint16Type          name_idx;
}VCRResultPlayPBType;


typedef struct {
    VCRResultTypeEnum     type;   /* = VCR_RESULT_TYPE_CALLSTACKS */
    APP_PHONEBOOK_enSelectedPbType   stack_type;
    Uint16Type            num_missed_calls;
    Uint16Type            num_entries;
    Uint16Type            cur_entry;
    ATPhoneNumberType     phone_number;
    Uint16Type            phone_number_len;
    AT_CallerId           name;   /* RT#1476*/
    Uint16Type            name_len;
    BooleanType           name_avail;
    Uint16Type            cur_entryPBindex;
    BooleanType           number_restriced;
}VCRResultCallstackType;


typedef struct {
    VCRResultTypeEnum     type;   /* = VCR_RESULT_TYPE_TRAINING */
    BooleanType           entry_from_main;
    BooleanType           ignore_error;
    ATSIVRTrainingText    text;
    Uint16Type            text_len;
    Uint16Type            error_num;

}VCRResultTrainingType;


typedef union {
    VCRResultNumberType     number;
    VCRResultContactUniType contact_uni;
    VCRResultContactAmbType contact_amb;
    VCRResultLanguageType   lang;
    VCRResultReadSMSType    readSMS;
    VCRResultBTPinType      bTPin;
    VCRResultRingtoneType   ringtone;
    VCRResultPlayPBType     playPB;
    VCRResultCallstackType  cStack;
    VCRResultTrainingType   training;
}VCRResultUnionType;

typedef struct {
    Uint16Type              menu_id;
    Uint16Type              confidence_rate;
    Uint8Type               result_index;
    ATSIVRResultEnum        result_type;
    ATSIVRCmdIdEnum         result_value;
    ATResSIVROptUnionType   u;
} VcrResultStructType;


typedef struct {
    VcrResultStructType     stResults[VCR_MAXNUM_RES_STORE];
    /* store number sting only if received with first result */
    BooleanType             number_valid;
    ATSIVRNumberText        number;
    Uint16Type              number_len;
    Uint8Type               u8NbrResults;
    Uint8Type               u8NbrReceivedResults;
} VcrProcessingResultType;

typedef struct {
    ATSIVRPromptTypeEnum    ePromptType;
    ATSIVRPromptEnum        ePromptId;
    ATSIVRCmdIdEnum         command;
} VcrProcessingMenuType;

typedef struct {
    /* control part for message sending */
    VCRSystemMsgEnum        eReqToBeSend;
    VCRSystemMsgEnum        eLastSend;
    VCRSystemMsgEnum        eLastSendForPrmtRestart;
    VCRSystemMsgEnum        eReqPending;
    Uint16Type              uNumRetry;
    BooleanType             bIsAPPMsg; /* indicate if this message is APP or AT*/
    BooleanType             bOkPending;
    /* control part for SIVR engine */
    BooleanType             bStopPending ;
    ATSIVREngStatusEnum     eEngineStatus;
} VcrControlStructType;

/* structure which include all data that is needed globally within VCR
 * to be passed via ptr to all functions */
typedef struct {

    BooleanType              bInitial_init_done;
    BooleanType              bStopOKReceived;/* workaround if CK5050 does not
                                                indicate stopped machine      */
    BooleanType              bVoiceFBOff;    /* Voice Feedback off            */
    ERMSIVRSupportEnum       eSIVRSupported; /* RT 2429, indication if SIVR is supported at
                                                all - to switch off /limit by UDS */
    BooleanType              bPerfMeasOn;    /* indicate if Performance
                                                Measures to be send           */
    /* perf measurement temp storage for result command */
    VCRPerfMeasCmdValueType   stASRCmdMsg;
    BooleanType               bPerfMeasStoreIsEvent;
    VCRMenuEventEnum          ePerfMeasStoreEvent;
    ATSIVRCmdIdEnum           ePerfMeasStoreCmd;
    Uint16Type                u16PerfMeasStoreCurCnt;
    Uint16Type                u16CurPbIndex;

    VcrControlStructType     stCtrlInfo;     /* control structure             */
    ATSIVRPromptEnum         eMenuState;     /* current menu state            */
    ATSIVRPromptEnum         ePrevMenuState; /* previous menu state           */
    ATSIVRPromptEnum         eNextMenuState; /* next menu state, needed for
                                                state change if more processing
                                                like prompts needed           */
    VCRHapticTriggerEnum     eHapticTrigType;
    Uint16Type               u16HapticIdx;
    VCRMenuEventEnum         eEventTrigMain; /* event trigger for main handler*/
    ATSIVRCmdIdEnum          eCmdTrigMain;   /* cmd trigger for main handler  */
    ATSIVRCmdIdEnum          eCmdId;         /* command id received from SIVR */
    VcrProcessingMenuType    stProcMenu;     /* processing menu items */
    VCRResultUnionType       resultInfo;
    Uint16Type               u16NumSMSMarkRead; /* number of SMS which needs to be marked
                                                   read */
    VcrProcessingResultType  stMsgRes;       /* values for each SIVR result msg */

    Uint16Type               u16MuteDelay;   /* mute delay in ms */
    Uint16Type               u16ListDelay;   /* list delay in ms */

    Uint8Type                u8ProcErr;      /* processing error */
    Uint8Type                u8NotRecogCmdProcErr; /* processing error */  /*RT-13225 & RT-14043*/
    Uint8Type                u8PrevProcErr;  /* processing error */  /*RRT-13225 & RT-14043*/
    Uint8Type                u8CurrentLangIdx;
    Uint8Type                u8NumSupLang;
    BooleanType              bTrigLangChange;
    VCRSystemStateEnum       eSystemState;
    BooleanType              bSivrSyncStarted;
    BooleanType              bReqLangSetFromExt;
    ATSIVRLanguagesEnum      eCurrentLang;
    ATSIVRLanguagesEnum      eRequiredLang;
    ATSIVRLanguagesEnum      eLangTable[VCR_MAX_LANG_TABLE];

    /* prompt data */
    Uint8Type                u8NumPromptRequired;
    Uint8Type                u8NumPromptSend;
    VCRPrompTable            ePromtTable;

    BooleanType              bVCRSupervisionActive;

    BooleanType              bPTTunhandled; /* needed in Lists if Event is already in use*/
    BooleanType              bPTTIgnore;

    BooleanType              bHOOKunhandled; /* needed in Lists if Event is already in use*/
    BooleanType              bHOOKIgnore;

    Uint8Type                PTT_contactNr;
    BooleanType              bMuteACK;       /*Mute ACK received from AudioManager*/
    BooleanType              bMuteWaitACK;   /* Bit to signalize that we wait for a Mute ACK*/

    Uint16Type               u16SimiliarNameVT;
    BooleanType              bFallbackLanguageChosen; // flag to  select the wright prompt
    BooleanType              bPlayerStopForLanguageChange; // flag stop player for lamguage change
    APP_CAN_enLang           QueuedCANLangID; // selected Language on CAN-Bus
    Uint16Type               u16TxPromptID; // last transmitted PromptID to Parrot VR

} VcrModGlobalDataType;
/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern SuccessFailType  _VCR_sFMessageToBeSend(
        VCRSystemMsgEnum msg_type);

extern void _VCR_vInitStateData(void);

extern void _VCR_vSetCK5050Available(
        BooleanType isAvail);

extern SuccessFailType  _VCR_sFHandleEvent(
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);


extern SuccessFailType _VCR_sfHandleDialNameConfEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);
extern SuccessFailType _VCR_sfHandleDialNameConfCatEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);
extern SuccessFailType _VCR_sfHandleDialNameResAmbiEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);
extern SuccessFailType _VCR_sfHandleDialNumberEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);
extern SuccessFailType _VCR_sfHandleDialNumberCorrectEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern void _VCR_vRedial(void);

extern void _VCR_vResetNotRecogCmdErrFlags(void); /*RT-13225 & RT-14043*/

extern SuccessFailType _VCR_sfHandleMusicEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleMoreOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandlePlayAddressbookEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandlePlayAddressbookEntryEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandlePlayAddressbookOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleResolveMultipleNumCatEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleResetNameConfirmEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleCallStacksEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleReadCallStacksEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleCallStackOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleCallStackResetConfirmEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleSettingsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleBluetoothOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleBluetoothPinEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleBluetoothPinCorrectEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleCallbackMissedCallsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleRestoreFactorySettingsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);


extern SuccessFailType _VCR_sfHandleChangeLanguageEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);


extern SuccessFailType _VCR_sfHandleDeleteAllVTConfirmEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);


extern SuccessFailType _VCR_sfHandleVoiceMemosEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleVoiceMemosPlayEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleVoiceMemosPlayOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleVoiceMemosConfDeleteEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleVoiceMemosConfDeleteAllEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleReadSMSEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleReadSMSOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleDeleteSMSConfirmEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleCallBackSMSConfirmEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern SuccessFailType _VCR_sfHandleStorenameEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/


#endif /* _VCR_STATE_HANDLER_H */

/* End Of File VCR_StateHandler.h */
