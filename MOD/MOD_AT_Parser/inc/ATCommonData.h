/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCommonData.h
*-----------------------------------------------------------------------------
* Module Name:    AT Data which is common for Parser and Cmd Handler
*-----------------------------------------------------------------------------
* Description:    this file include all common data definitions for AT
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/inc/ATCommonData.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATCOMMON_DATA_H
#define _ATCOMMON_DATA_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ATParser_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Defines                                                             */
/*----------------------------------------------------------------------------*/


#define AT_MAX_PHONE_NUM_LEN 32 /* used for message */

/* MAX number of PINCODE LEN for UHV - also required number,
 * means all PINS with less digits are filled to this nuimber with leading
 * zeros */
#define AT_UHV_BT_PIN_CODE_LEN 4

/*
 * #define AT_MAX_PIN_CODE_LEN  16
 * maximum number of PIN code digits for CK5050 */
#define AT_MAX_PIN_CODE_LEN  AT_UHV_BT_PIN_CODE_LEN
/* maximum number of PIN code digits for secure simple pairing */
#define AT_MAX_SSP_PIN_CODE_LEN 6
/* max text length for VR prompt and options */
/* NOTE: the max prompt text len is 255 - due to memory reason we set it to
   SMS UNIcode len to be able to play one SMS with one text msg. If UTF8
   SMS are supported those are still split */
#define AT_SIVR_MAX_PROMPT_TEXT_LEN 200
#define AT_SIVR_MAX_PRMT_OPT_LEN 66
#define AT_SIVR_MAX_PRMT_NUM_LEN AT_SIVR_MAX_PRMT_OPT_LEN
#define AT_SIVR_MAX_PRMT_TRAIN_LEN AT_SIVR_MAX_PRMT_OPT_LEN

/* used for PALGOSETMIC */
#define AT_MIC_ID_MIN    0
#define AT_MIC_ID_MAX    1
#define AT_MIC_ID_DEFAULT    AT_MIC_ID_MIN
#define AT_MIC_ID_GET    (AT_MIC_ID_MAX + 1)

/* used for +VGM */
#define AT_MIC_GAIN_MIN       0
#define AT_MIC_GAIN_DEFAULT   8
#define AT_MIC_GAIN_MAX      15
#define AT_MIC_GAIN_GET      (AT_MIC_GAIN_MAX + 1)

#define AT_ADC_GAIN_MIN       0
#define AT_ADC_GAIN_MID      15
#define AT_ADC_GAIN_MAX      30

/* PALGOALCRX max, min value of level */
#define AT_ALC_RX_MIN         0
#define AT_ALC_RX_MAX        40
#define AT_ALC_RX_DEFAULT     6

/* PALGOALCRX max,min of setable mode */
#define AT_ALC_RX_MODE_MIN    0
#define AT_ALC_RX_MODE_MAX    2


/* PALGOSLRX value limits for level value */
#define AT_SL_RX_MIN         0
#define AT_SL_RX_MAX        40
#define AT_SL_RX_DEFAULT     4

/* PALGOSLTX value limits for level value */
#define AT_SL_TX_MIN         0
#define AT_SL_TX_MAX        40
#define AT_SL_TX_DEFAULT     2

/* ranges for Noise reduction settings */
/* MAX Attenuation */
#define AT_NR_MAXATT_MIN       0
#define AT_NR_MAXATT_MAX      25
#define AT_NR_MAXATT_DEFAULT  14

/* ranges input gain */
#define AT_NR_INPUTGAIN_MIN     -12
#define AT_NR_INPUTGAIN_MAX     +12
#define AT_NR_INPUTGAIN_DEFAULT   0

/* ranges qualizer gain */
#define AT_EQU_NUM_FREQ_BAND   9  /* number of available frequency bands */
#define AT_EQU_GAIN_MIN      -10
#define AT_EQU_GAIN_MAX      +10
#define AT_EQU_GAIN_DEFAULT    0

#define AT_RX_LIMITLEV_MIN     0
#define AT_RX_LIMITLEV_MAX    30
#define AT_RX_LIMITLEV_DEFAULT     AT_RX_LIMITLEV_MIN

#define AT_RX_HPFILTER_MIN     0
#define AT_RX_HPFILTER_MAX     9
#define AT_RX_HPFILTER_DEFAULT     AT_RX_HPFILTER_MIN

#define AT_RX_MAXATTAEC_MIN     0
#define AT_RX_MAXATTAEC_MAX    30
#define AT_RX_MAXATTAEC_DEFAULT 7
#define AT_RX_AEC_GET           (AT_RX_MAXATTAEC_MAX + 1)

#define AT_RX_SWITCHTIME_MIN        1
#define AT_RX_SWITCHTIME_MAX      100
#define AT_RX_SWITCHTIME_DEFAULT   23

#define AT_VOLUME_GEN_MIN        0
#define AT_VOLUME_GEN_MAX       20
#define AT_VOLUME_GEN_DEFAULT   10

#define AT_VOLUME_STREAM_MIN        0
#define AT_VOLUME_STREAM_MAX       63
#define AT_VOLUME_STREAM_DEFAULT   48


#define AT_VOLUME_PROMPT_MIN       40
#define AT_VOLUME_PROMPT_MAX      100
#define AT_VOLUME_PROMPT_DEFAULT   60

#define AT_AEC_DELAY_MIN        0
#define AT_AEC_DELAY_MAX       25
#define AT_AEC_DELAY_DEFAULT   AT_AEC_DELAY_MIN

/* used for RGSL */
#define AT_RGSL_MIN_START_INDEX 1

/* used for RPMT */
#define AT_RPMT_MAX_NUM_OPT      4
#define AT_RRES_MAX_NUM_OPT      2
/* used for RSRF */
#define AT_RSRF_MIN_RESULT          1
#define AT_RSRF_MAX_RESULT         10
#define AT_RSRF_MAX_REJECT_LEV    100
#define AT_RRES_MIN_CONF_LEV        0
#define AT_RRES_MAX_CONF_LEV    10000


#define AT_PBMODE_AUTOSYNC_ENABLED       0
#define AT_PBMODE_AUTOSYNC_DISABLED      1
#define AT_PBMODE_AUTOSYNC_GET           2

#define AT_PB_SORTORDER_FIRSTLAST        0
#define AT_PB_SORTORDER_LASTFIRST        1
#define AT_PB_SORTORDER_GET              2

#define PARROT_MODEL_CK5050_STRING   "CK5050"
#define PARROT_MODEL_CK5050P_STRING  "CK5050P"
#define PARROT_MODEL_UNKNOWN_STRING  "UNKNOWN"

#define AT_AUDIO_LOOP_TEST_MODE         10

#define AT_ALTERNATING_FREQ_MIN         200  /* TODO : input still missing */
#define AT_ALTERNATING_FREQ_MAX       20000  /* TODO : input still missing */

#define AT_ALTERNATING_LEVEL_MIN       0x0
#define AT_ALTERNATING_LEVEL_MAX       0x7FFF

#define AT_MAX_IMSI_LEN                15

/*----------------------------------------------------------------------------*/
/* Public Enumerations                                                        */
/*----------------------------------------------------------------------------*/


typedef enum {
    AT_FALSE = 0,
    AT_TRUE  = 1
} ATBooleanEnum;

typedef enum {
    APP_SUB_NONE,
    APP_SUB_ERM,
    APP_SUB_SMS,
    APP_SUB_TEL,
    APP_SUB_MDI,
    APP_SUB_PHONEBOOK,
    APP_SUB_VCR,
    APP_SUB_DEVICE
} APPSubModuleIdEnum;

/* enumeration to indicate if a message to application include directly the
 * data or a pointer to a pool element                                        */
typedef enum {
    AT_RSP_WITH_DATA = 1,
    AT_RSP_WITHOUT_DATA
} ATRspDataIndicationEnum;

/* eum to indicate if an optional parameter is available or not */
typedef enum {
    AT_OPT_NOT_AVAILABLE = 0,
    AT_OPT_AVAILABLE     = 1
}  ATOptParamAvailableEnum;


typedef enum {
    AT_VALUE_NOT_PRESENT = 0,
    AT_VALUE_PRESENT     = 1
} ATParamPresentEnum;


typedef enum {
    AT_DEVICE_INVALID       = -1,
    AT_DEVICE_NOT_CONNECTED = 0,
    AT_DEVICE_CONNECTED     = 1
} ATDevConnStatusEnum;

typedef enum {
    AT_NOT_SUPPORTED     = 0,
    AT_SUPPORTED         = 1
} ATSupportedEnum;

typedef enum {
  AT_SDP_SUCCEED = 1,
  AT_SDP_FAILED  = 2
} ATSDPStatusEnum;

typedef enum {
    AT_HANGUP_CALL_0   = 0,
    AT_HANGUP_CALL_1   = 1,
    AT_HANGUP_CALL_2   = 2,
    AT_HANGUP_NO_INDEX = 3
  } ATCallHangupEnum;

typedef enum {
    AT_CALL_INDEX_0     = 0,
    AT_CALL_INDEX_1     = 1,
    AT_CALL_INDEX_2     = 2,
    AT_CALL_INDEX_NONE  = 3,
    AT_CALL_INDEX_CONFERENCE = 4
} ATCallIndexEnum;

typedef enum {
    AT_MPC_ACTION_HANGUP_IDX_OR_ALL     = 0,
    AT_MPC_ACTION_ALL_ACTIVE_ON_HOLD    = 1,
    AT_MPC_ACTION_PUT_IDX_ACTIVE        = 2,
    AT_MPC_ACTION_HELD_CALLS_TO_CONF    = 3,
    AT_MPC_ACTION_JOIN_ACTIVE_CALLS     = 4
} ATMPCActionEnum;

typedef enum {
    AT_CS_ACTIVE_CALL      = 0,
    AT_CS_HELD_CALL        = 1,
    AT_CS_OUTCALL_DIAL_IPR = 2,
    AT_CS_OUTCALL_RING_IPR = 3,
    AT_CS_INCOMING         = 4,
    AT_CS_WAITING_CALL     = 5,
    AT_CS_CALL_TERMINATED  = 6
} ATCallStateEnum;

typedef enum {
    AT_PNT_GENERAL = 0,
    AT_PNT_HOME    = 1,
    AT_PNT_OFFICE  = 2,
    AT_PNT_MOBILE  = 3,
    AT_PNT_OTHER   = 4
} ATPhoneNumberTypeEnum;

typedef enum {
    AT_CD_OUTGOING = 0,
    AT_CD_INCOMING = 1,
    AT_CD_UNKNOWN  = 2
} ATCallDirTypeEnum;

typedef enum {
    AT_CC_NOT_CONFERENCE = 0,
    AT_CC_CONFERENCE     = 1
} ATCallConfTypeEnum;

typedef enum {
    AT_CM_VOICE_CALL = 0,
    AT_CM_DATA_CALL  = 1,
    AT_CM_FAX_CALL   = 2
} ATCallModeTypeEnum;

typedef enum {
    AT_LOC_HOME    = 1,
    AT_LOC_OFFICE  = 2,
    AT_LOC_MOBILE  = 4,
    AT_LOC_OTHER   = 8,
    AT_LOC_GENERAL = 16
} ATRresLocationEnum;

typedef enum {
    AT_INDI_BT_CONN_STAT   = 1,
    AT_INDI_NW_SERV_AVAIL  = 2,
    AT_INDI_ROAMING        = 3,
    AT_INDI_NW_SIGSTRENGHT = 4,
    AT_INDI_OPERATOR_NAME  = 5,
    AT_INDI_BATT_LEVEL     = 6,
    AT_INDI_NEW_SMS_REC    = 7,
    AT_INDI_VOICE_MAIL     = 8
} ATIndicatorIndexEnum;


typedef enum {
    AT_CONN_SUCC         = 0,
    AT_CONN_FAILED       = 1,
    AT_CONN_NOT_FOUND    = 2,
    AT_CONN_DISCON_SUCC  = 3,
    AT_CONN_DISCON_FAIL  = 4,
    AT_CONN_AUTO_NOT     = 5
} ATConnStatusEnum;

typedef enum {
    AT_WARN_FLASH_FULL           =  1,
    AT_WARN_BRUT_DISCONNECT      =  2,
    AT_WARN_RAM_FULL             =  3,
    AT_WARN_BLUETOOTH_CHIP_ERROR =  4,
    AT_WARN_DIAL_ERROR           = 11,
    AT_WARN_HGUP_ERROR           = 12,
    AT_WARN_PICK_ERROR           = 13,
    AT_WARN_2CALL_ERROR          = 14,
    AT_WARN_DTMF_ERROR           = 15,
    AT_WARN_AVRCP_ERROR          = 21,
    AT_WARN_A2DP_ERROR           = 31,
    AT_WARN_USB_ERROR            = 41
} ATWarningCodeEnum;

typedef enum {
    AT_MIC_MUTE       = 0,
    AT_MIC_UNMUTE     = 1,
    AT_MIC_STATUS_GET = 2
} ATMicModeEnum;

typedef enum {
    AT_AUDIO_DISCRETE_MODE    = 0,
    AT_AUDIO_HANDSFREE_MODE   = 1,
    AT_AUDIO_MODE_GET         = 2
} ATAudioTransferModeEnum;

typedef enum {
    AT_AUDIO_GW_MODE_OFF    = 0,
    AT_AUDIO_GW_MODE_ON     = 1,
    AT_AUDIO_GW_MODE_GET    = 2
} ATAudioGatewayModeEnum;

typedef enum {
    AT_AUDIO_GW_TRANSFER_MODE_OFF    = 0,
    AT_AUDIO_GW_TRANSFER_MODE_ON     = 1,
    AT_AUDIO_GW_TRANSFER_MODE_GET    = 2
} ATAudioGatewayTransferModeEnum;

typedef enum {
    AT_RTM_DEACTIVATE_LOCAL = 0,
    AT_RTM_ACTIVATE_LOCAL   = 1,
    AT_RTM_GET              = 2
} ATRingToneModeEnum;

typedef enum {
    AT_PBTYPE_USER_PERSONAL = 0,
    AT_PBTYPE_LAST_DIALLED  = 1,
    AT_PBTYPE_LAST_RECEIVED = 2,
    AT_PBTYPE_MISSED        = 3,
    AT_PBTYPE_GET           = 4
} ATPhoneBookTypeEnum;

typedef enum {
    AT_PB_CALLHIST_RAM        = 0,
    AT_PB_CALLHIST_FLASH      = 1,
    AT_PB_CALLHIST_DEL_FLASH  = 2,
    AT_PB_CALLHIST_GET        = 3
} ATPhoneBookCallHistEnum;

typedef enum {
    AT_PBMODE_ALL       = 0,
    AT_PBMODE_ONLY_CTAG = 1
} ATPhoneBookModeEnum;

typedef enum {
    AT_PPBCEX_DISABLE   = 0,
    AT_PPBCEX_ENABLE    = 1,
    AT_PPBCEX_GET       = 2
} ATPhoneBookCallStackeChangeModeEnum;


typedef enum
{
    AT_PB_LOADING           = 0x00,
    AT_PB_LOAD_FINISHED     = 0x01,
    AT_PB_LOAD_FINISHED_BUT = 0x02
} ATPhoneBookDwnlState;

typedef enum {
    AT_PAIR_REFUSE_INCOMING  = 0,
    AT_PAIR_FORWARD_INCOMING = 1,
    AT_PAIR_ACCEPT_INCOMING  = 2,
    AT_PAIR_EXT_MODE1        = 3,
    AT_PAIR_EXT_MODE2        = 4,
    AT_PAIR_GET_MODE         = 5
} ATPairingModeEnum;

typedef enum {
    AT_PAIR_CMD_INITIALTE    = 0,
    AT_PAIR_CMD_ACCEPT       = 1,
    AT_PAIR_CMD_REJECT       = 2
} ATPairingCommandEnum;

typedef enum {
    AT_SSP_DISABLE          = 0,
    AT_SSP_ENABLE           = 1,
    AT_SSP_REQUEST          = 2
} ATSSPModeEnum;

typedef enum {
    AT_PARAM_SSP_IOCAP_DISPLAY          = 0,    /* only Display                     */
    AT_PARAM_SSP_IOCAP_DISPLAY_YESNO    = 1,    /* Display & 2 buttons (yes/no)     */
    AT_PARAM_SSP_IOCAP_KEYBOARD         = 2,    /* Display & Keyboard with digits   */
    AT_PARAM_SSP_IOCAP_NOINNOOUT        = 3,    /* No Input/Output capabilitys      */
    AT_PARAM_SSP_IOCAP_INVALID          = 4
}ATSSPIoCapParameterEnum;

typedef enum {
    AT_EXTCALL_DISABLE      = 0,
    AT_EXTCALL_ENABLE       = 1,
    AT_EXTCALL_ENABLE_MORE  = 2,
    AT_EXTCALL_GET          = 3
} ATExtendedCallModeEnum;

/* enumeration used for PLPD request to indicate specific type */
typedef enum {
    AT_PLPD_GET_NUMBER = 0, /* trigger PLPD=?*/
    AT_PLPD_GET_DEVICE = 1, /* trigger PLPD=deviceId*/
    AT_PLPD_GET_ALL    = 2  /* trigger PLPD */
} ATPlpdReqTypeEnum;


/* enumeration to indicate the SMS charset */
typedef enum {
    AT_SMS_CHARSET_ISO  = 0, /* use ISO8859-15 charset  */
    AT_SMS_CHARSET_UTF8 = 1  /* use UTF8 charset        */
} ATSmsCharsetEnum;

/* enumeration to indicate the SMS type */
typedef enum {
    AT_SMS_TYPE_INVALID = 0,
    AT_SMS_TYPE_UNREAD  = 1,
    AT_SMS_TYPE_READ    = 2,
    AT_SMS_TYPE_UNSENT  = 3,
    AT_SMS_TYPE_SEND    = 4
} ATSmsTypeEnum;

typedef enum {
    AT_DEVICE_BLUETOOTH = 0,
    AT_DEVICE_PLAYER    = 1
} ATDeviceTypeEnum;

typedef enum {
    AT_DEVCODED_SBC     = 0,
    AT_DEVCODED_MP3     = 1,
    AT_DEVCODED_PCM     = 2,
    AT_DEVCODED_OGG     = 3,
    AT_DEVCODED_WMA     = 4,
    AT_DEVCODED_NONE    = 7
} ATDevicePlayerCodedEnum;

typedef enum {
    AT_DEVREPEAT_DISABLE                    = 0,
    AT_DEVREPEAT_ENABLE_ONE_SONG            = 1,
    AT_DEVREPEAT_ENABLE_PLAYLIST            = 2,
    AT_DEVREPEAT_UNKNOWN                    = 3,
    AT_DEVREPEAT_ENABLE_CURR_GROUP_SONG     = 4
} ATRepeatModeStatusEnum;

typedef enum {
    AT_DEVRANDOM_DISABLE        = 0,
    AT_DEVRANDOM_ENABLE         = 1,
    AT_DEVRANDOM_ENABLE_ALBUM   = 2,
    AT_DEVRANDOM_UNKNOWN        = 3
} ATRandomModeStatusEnum;

typedef enum {
    AT_MDB_STOP_PLAYING       = 0,
    AT_MDB_START_PLAYING      = 1,
    AT_MDB_PAUSE_PLAYING      = 2,
    AT_MDB_RESUME_PLAYING     = 3,
    AT_MDB_NEXT_SONG          = 4,
    AT_MDB_PREVIOUS_SONG      = 5,
    AT_MDB_F_FORWARD_PLAYING  = 6,
    AT_MDB_F_REWIND_PLAYING   = 7,
    AT_MDB_NONE               = 8
} ATDMDBActionEnum;

typedef enum {
    AT_DGPS_STATUS_STOP             = 0,
    AT_DGPS_STATUS_PLAY             = 1,
    AT_DGPS_STATUS_PAUSE            = 2,
    AT_DGPS_STATUS_FAST_REWIND      = 3,
    AT_DGPS_STATUS_FAST_FORWARD     = 4,
    AT_DGPS_STATUS_TRACK_CHANGED    = 5
} ATDGPSStatusEnum;

typedef enum {
    AT_PLAYER_REPEAT_DISABLED = 0,
    AT_PLAYER_REPEAR_ONETRACK = 1,
    AT_PLAYER_REPEAR_ALLTRACK = 2,
    AT_PLAYER_REPEAR_GROUP = 4
} ATPlayerRepeatModeEnum;

typedef enum {
    AT_AUTOMATIC_NOTIFICATION_DISABLED = 0,
    AT_AUTOMATIC_NOTIFICATION_ENABLED = 1
} ATReqConfigPositionNotiEnum;

typedef enum {
    AT_ENTRY_MASK_FILEPATH    = 0,
    AT_ENTRY_MASK_TITLE       = 1,
    AT_ENTRY_MASK_ARTIST      = 2,
    AT_ENTRY_MASK_ALBUM       = 3,
    AT_ENTRY_MASK_TRACKNUMBER = 4,
    AT_ENTRY_MASK_GENRE       = 5
} ATEntryFieldMaskEnum;

typedef enum {
    AT_SERVICE_ACT_STOP            = 0,
    AT_SERVICE_ACT_START           = 1,
    AT_SERVICE_ACT_QUERY_SUPPORTED = 2,
    AT_SERVICE_ACT_QUERY_CONNECTED = 3
} ATServiceActionEnum;

typedef enum {
    AT_SERVICE_STATUS_STOPPED = 0,
    AT_SERVICE_STATUS_STARTED = 1
} ATServiceStatusEnum;

typedef enum {
    AT_SERVICE_PHONE         = 1,
    AT_SERVICE_DUN           = 2,
    AT_SERVICE_A2DP_SINK     = 3,
    AT_SERVICE_A2DP_SOURCE   = 4,
    AT_SERVICE_AVRCP         = 5,
    AT_SERVICE_AUDIO_GATEWAY = 6,
    AT_SERVICE_SPP           = 7
} ATBTServiceEnum;


typedef enum {
    AT_CODPROF_NONE           = 0,
    AT_CODPROF_HEADSET        = 1,
    AT_CODPROF_HANDSFREE_0_96 = 2,
    AT_CODPROF_HANDSFREE_1_00 = 3,
    AT_CODPROF_HANDSFREE_1_50 = 4
} ATCoDProfileEnum;


typedef enum {
    AT_SERVRES_START_SUCCEED = 0,
    AT_SERVRES_START_FAILED  = 1,
    AT_SERVRES_STOP_SUCCEED  = 3,
    AT_SERVRES_STOP_FAILED   = 4
} ATBTServiceResultEnum;

typedef enum {
    AT_INQUIRY_DEVICE   = 0,
    AT_DELETED_DEVICE   = 1,
    AT_INITIATED_DEVICE = 2
} ATPairingDeviceType;

typedef enum {
    AT_SSP_FEATURE   = 0
} ATBTFeatureEnum;

typedef enum {
    AT_PAIRING_FAILED_UNKNOWN       = 0,
    AT_PAIRING_SUCCEEDED            = 1,
    AT_PAIRING_FAILED               = 2,
    AT_DEVICE_NOT_FOUND             = 3,
    AT_MAX_PAIRED_DEVICES_REACHED   = 4
} ATPairingResultEnum;

typedef enum {
    AT_REQ_KEY_PREVIOUS   = 4,
    AT_REQ_KEY_PLAY       = 5,
    AT_REQ_KEY_PAUSE      = 6,
    AT_REQ_KEY_STOP       = 7,
    AT_REQ_KEY_NEXT       = 8,
    AT_REQ_KEY_F_FORWARD  = 9,
    AT_REQ_KEY_F_BACKWARD = 10
} ATReqKeyboardKeysEnum;

typedef enum {
    AT_RSP_KEY_AUDIO_RESUMED   = 5,
    AT_RSP_KEY_AUDIO_SUSPENDED = 6
} ATRspKeyboardKeysEnum;

typedef enum {
	AT_REQ_KEYEVT_RELEASE = 0,
    AT_REQ_KEYEVT_PRESS   = 1
} ATReqKeyboardEventEnum;

typedef enum {
    AT_META_NOT_SUPPORTED   = 0,
    AT_META_SONY_ERICCSON   = 1,
    AT_META_AVRCP_1_3       = 2,
    AT_META_AVRCP_1_4       = 3
} ATMetadataTypeEnum;

typedef enum {
    AT_AUDIO_STATE_NO_ACTIVE      = 0,
    AT_AUDIO_STATE_CK5050         = 1,
    AT_AUDIO_STATE_COMMUNICATION  = 2,
    AT_AUDIO_STATE_STREAMING_A2DP = 3,
    AT_AUDIO_STATE_BT_AUDIO       = 4,
    AT_AUDIO_STATE_I2S            = 5,
    AT_AUDIO_STATE_STREAMING_UART = 6,
    AT_AUDIO_STATE_STREAMING_USB  = 7,
    AT_AUDIO_STATE_SIVR_PROMPTS   = 8
} ATAudioSourceActStateEnum;

typedef enum {
    AT_AUTOCONN_STOPPED     = 0,
    AT_AUTOCONN_IN_PROGRESS = 1
} ATAutoConnStatusEnum;

typedef enum {
    AT_DEVSEL_DISCONNECT    = 0,
    AT_DEVSEL_CONNECT       = 1
} ATBTDeviceSelectEnum;

typedef enum {
    AT_AUTOCONNEVT_STOPPING   = 0,
    AT_AUTOCONNEVT_STARTING   = 1
} ATAutoConnStatusEventEnum;

typedef enum {
    AT_VRCTX_NAME_DIALING   = 0,
    AT_VRCTX_DIGIT_DIALING  = 1,
    AT_VRCTX_MENU_SELECTION = 2,
    AT_VRCTX_VTAG_RECORDING = 3,
    AT_VRCTX_NO_CONTEXT     = 4
} ATVRContextTypeEnum;

typedef enum {
    AT_VR_STATUS_IDLE         = 0,
    AT_VR_STATUT_STARTED      = 1,
    AT_VR_STATUS_BEGIN_SPEAK  = 2,
    AT_VR_STATUS_END_SPEAK    = 3,
    AT_VR_STATUS_BEGIN_PROMPT = 4,
    AT_VR_STATUS_END_PROMPT   = 5,
    AT_VR_STATUS_TAG_ACCEPT   = 6,
    AT_VR_STATUS_TAG_REJECT   = 7,
    AT_VR_STATUS_UTTER        = 8,
    AT_VR_STATUS_TIMEOUT      = 9,
    AT_VR_STATUS_BREAK        = 10,
    AT_VR_STATUS_ERROR        = 11
} ATVRStatusEnum;


typedef enum {
    AT_VR_VTT_PHONE_NAME    = 0,
    AT_VR_VTT_CONTACT_NAME  = 1
} ATVRVoiceTagTypeEnum;

typedef enum {
    AT_VR_ERROR_FEATURE_NOTIMPL = 1,
    AT_VR_ERROR_ACT_NOT_ALLOWED = 2,
    AT_VR_ERROR_RECORDING_ERROR = 3,
    AT_VR_ERROR_AUDIO_INPUT     = 5,
    AT_VR_ERROR_AUDIO_OUTPUT    = 6,
    AT_VR_ERROR_AUTO_SPEECH_REC = 7,
    AT_VR_ERROR_TEST_TO_SPEECH  = 8,
    AT_VR_ERROR_CONFIG          = 9,
    AT_VR_ERROR_RESERVED        = 10,
    AT_VR_ERROR_PB_LOOKUP       = 11,
    AT_VR_ERROR_INVALID_MENUID  = 12
} ATVRErrorTypeEnum;


typedef enum {
    AT_DISCOVER_MODE_DISALLOW = 0,
    AT_DISCOVER_MODE_ALLOW    = 1,
    AT_DISCOVER_MODE_GET      = 2
} ATBTDiscoverModeEnum;

typedef enum {
    AT_MODEL_CK5050   = 0,
    AT_MODEL_CK5050P  = 1,
    AT_MODEL_UNKNOWN  = 2
} ATCK5050ModelEnum;

typedef enum {
    AT_MIC_ALGO_NONE                = 0,
    AT_MIC_ALGO_MONO_AEC            = 1,
    AT_MIC_ALGO_MONO_NR2            = 2,
    AT_MIC_ALGO_MONO_AEC_NR2        = 3,
    AT_MIC_ALGO_DBL_AEC_NR1         = 4,
    AT_MIC_ALGO_MONO_AEC_NR3        = 5,
    AT_MIC_ALGO_DBL_AEC_NR3         = 6,
    AT_MIC_ALGO_DBL_AEC_NR3_AMS     = 7,
    AT_MIC_ALGO_DBL_AEC_STEREO_NR2  = 8,
    AT_MIC_ALGO_MONO_AEC2_NR2       = 9,
    AT_MIC_ALGO_MONO_AEC2_32BIT_NR2 = 10,
    AT_MIC_ALGO_MONO_AEC2_32BIT_NR3 = 11,
    AT_MIC_ALGO_GET                 = 12
} ATMicAlgoEnum;

typedef enum {
    AT_ADC_MIC_ALGO_GAIN   = 1,
    AT_ADC_MIC_ALGO_POWER  = 2,
    AT_ADC_MIC_ALGO_MODE   = 3
} ATADCMicAlgoEnum;

typedef enum {
    AT_SPEAKER_LIMIT_HP        = 0,
    AT_SPEAKER_COMPRESS        = 1,
    AT_SPEAKER_NR_EQU_COMPRESS = 2
} ATSpeakerAlgoEnum;

typedef enum {
    AT_NRDIR_ALL           = 0,
    AT_NRDIR_CENTRE        = 1,
    AT_NRDIR_LEFT_CENTER   = 2,
    AT_NRDIR_RIGHT_CENTER  = 3,
    AT_NRDIR_CENTER_DIST10 = 4
} ATNRDirectionEnum;

typedef enum {
    AT_AUDIOTYPE_COMM           = 1,
    AT_AUDIOTYPE_INC_RING       = 2,
    AT_AUDIOTYPE_BEEP           = 3,
    AT_AUDIOTYPE_STREAMING      = 4,
    AT_AUDIOTYPE_PROMPT         = 5,
    AT_AUDIOTYPE_GET_CURRENT    = 6,
    AT_AUDIOTYPE_GET_MINMAX     = 7,
    AT_AUDIOTYPE_ALC            = 8
} ATAudioVolumeTypeEnum;


typedef enum {
    AT_TEST_LOOPBACK_MIC1     = 1,
    AT_TEST_BT_AUDIOLOOP      = 2,
    AT_TEST_LOOPBACK_MIC2     = 3
}ATTestLoopbackModeEnum;

typedef enum {
    AT_SMSCONC_SET_DESTINATION  = 0,
    AT_SMSCONC_SEND             = 1,
    AT_SMSCONC_SEND_END         = 2
}ATSmsConcModeEnum;

typedef enum {
    AT_TEST_FREQ_TX_CHAN0               = 0,
    AT_TEST_FREQ_TX_CHAN39              = 1,
    AT_TEST_FREQ_TX_CHAN78              = 2,
    AT_TEST_FREQ_TX_DH5_CHAN0           = 3,
    AT_TEST_FREQ_TX_DH5_CHAN39          = 4,
    AT_TEST_FREQ_TX_DH5_CHAN78          = 5,
    AT_TEST_FREQ_REC_CHAN_39            = 6,
    AT_TEST_FREQ_TELEC_AFH_LOW          = 10,
    AT_TEST_FREQ_TELEC_AFH_MID          = 11,
    AT_TEST_FREQ_TELEC_AFH_HIGH         = 12,
    AT_TEST_FREQ_PMB8753_TX_CHAN19      = 15,
    AT_TEST_FREQ_PMB8753_TX_CHAN19_GFSK = 16,
    AT_TEST_FREQ_PMB8753_REC_CHAN19     = 17,
    AT_TEST_FREQ_PMB8753_REC_CHAN0      = 18,
    AT_TEST_FREQ_PMB8753_REC_CHAN78     = 19,
    AT_TEST_FREQ_PMB8753_TX_DH1_CHAN39  = 30,
    AT_TEST_FREQ_PMB8753_TX_DH3_CHAN39  = 31,
    AT_TEST_FREQ_PMB8753_TX_DH1_CHAN0   = 32,
    AT_TEST_FREQ_PMB8753_TX_DH3_CHAN0   = 33,
    AT_TEST_FREQ_PMB8753_TX_DH1_CHAN78  = 34,
    AT_TEST_FREQ_PMB8753_TX_DH3_CHAN78  = 35,
    AT_TEST_FREQ_PMB8753_TX_DH1_AFH     = 36,
    AT_TEST_FREQ_PMB8753_TX_DH3_AFH     = 37,
    AT_TEST_FREQ_PMB8753_TX_DH5_AFH     = 38,
    AT_TEST_FREQ_PMB8753_TX_2DH1_AFH    = 39,
    AT_TEST_FREQ_PMB8753_TX_2DH3_AFH    = 40,
    AT_TEST_FREQ_PMB8753_TX_2DH5_AFH    = 41,
    AT_TEST_FREQ_PMB8753_TX_3DH1_AFH    = 42,
    AT_TEST_FREQ_PMB8753_TX_3DH3_AFH    = 43,
    AT_TEST_FREQ_PMB8753_TX_3DH5_AFH    = 44
}ATTestFreqModeEnum;


typedef enum {
    AT_SIVR_ENG_IDLE         = 0,
    AT_SIVR_ENG_STARTED      = 1,
    AT_SIVR_ENG_BEGIN_AQU    = 2,
    AT_SIVR_ENG_END_AQU      = 3,
    AT_SIVR_ENG_BEGIN_PROMPT = 4,
    AT_SIVR_ENG_END_PROMPT   = 5,
    AT_SIVR_ENG_BEGIN_SYNC   = 6,
    AT_SIVR_ENG_END_SYNC     = 7
} ATSIVREngStatusEnum;

typedef enum {
    AT_SIVR_PROMPT_UNDEF   = 0,
    AT_SIVR_PROMPT_MENU    = 1,
    AT_SIVR_PROMPT_HELP    = 2,
    AT_SIVR_PROMPT_SYSTEM  = 3,
    AT_SIVR_PROMPT_CUSTOM  = 4
} ATSIVRPromptTypeEnum;

typedef enum {
    AT_SIVR_RES_NO_SPEECH    = 0,
    AT_SIVR_RES_NOT_RECO     = 1,
    AT_SIVR_RES_USER_CMD     = 2,
    AT_SIVR_RES_VT_RECORDED  = 3,
    AT_SIVR_RES_VT_REJECTED  = 4,
    AT_SIVR_RES_TRAIN_ACCEPT = 5,
    AT_SIVR_RES_TRAIN_REJECT = 6,
    AT_SIVR_RES_TRAIN_FINISH = 7
} ATSIVRResultEnum;


/* enums for voice tag recording */
typedef enum {
    AT_VTREC_ACT_ABORT    = 0,
    AT_VTREC_ACT_START    = 1,
    AT_VTREC_ACT_CONTINUE = 2
} ATVTRecActionEnum;

typedef enum {
    AT_VTREC_KWIDX_GENERAL    = 0,
    AT_VTREC_KWIDX_HOME       = 1,
    AT_VTREC_KWIDX_OFFICE     = 2,
    AT_VTREC_KWIDX_MOBILE     = 3,
    AT_VTREC_KWIDX_OTHER      = 4
} ATVTRecKWIdxEnum;

typedef enum {
    AT_VTREC_RESULT_INPROG        = 20,
    AT_VTREC_RESULT_REPEAT        = 21,
    AT_VTREC_RESULT_TOO_DIFFERENT = 22,
    AT_VTREC_RESULT_TOO_SOFT      = 23,
    AT_VTREC_RESULT_TOO_LOUD      = 24,
    AT_VTREC_RESULT_TOO_LONG      = 25,
    AT_VTREC_RESULT_END           = 0xFF /* to indicate END of recording */
} ATVTRecResultEnum;


typedef enum {
    AT_VTREC_RES_TOO_CLOSE = 0,
    AT_VTREC_RES_TOO_SHORT = 1,
    AT_VTREC_RES_TOO_SOFT  = 2,
    AT_VTREC_RES_TOO_LOUD  = 3,
    AT_VTREC_RES_TOO_BAD   = 4
} ATVTRecResEnum;



typedef enum {
    AT_BT_AUTO_CONN_MODE_NONE               = 0,
    AT_BT_AUTO_CONN_MODE_ONLY_THE_LAST      = 1,
    AT_BT_AUTO_CONN_MODE_ALL                = 2,
    AT_BT_AUTO_CONN_MODE_BITMASK_OF_DEVS    = 3,
    AT_BT_AUTO_CONN_MODE_QUERY_SETTINGS     = 4
}ATBTAutoConnMode;


typedef enum {
    AT_PHONEBOOK_NO_UPDATE_IN_PROGRESS      = 0,
    AT_PHONEBOOK_UPDATE_IN_PROGRESS         = 1
}ATPhonebookUpdateStatusResEnum;

typedef enum {
    AT_BT_SW_UPDATE_DEACTIVATE_SW_CAPABILITY                     = 0,
    AT_BT_SW_UPDATE_ACTIVATE_SW_CAPABILITY                       = 1,
    AT_BT_SW_UPDATE_ACTIVATE_SW_CAPABILITY_IN_SECURED_MODE       = 2,
    AT_BT_SW_UPDATE_ASK_FOR_UPDATE_FILE_VERSIONS_INFOS           = 3,
    AT_BT_SW_UPDATE_CONTINUE_UPDATING_SECURITY_CHECKS_ARE_OK     = 4,
    AT_BT_SW_UPDATE_ABORT_UPDATING_SECURITY_CHECKS_NOT_OK_REBOOT = 5
}ATBTSWUpdateRecEnum;

typedef enum {
    AT_BT_SW_UPDATE_START_TRANSFER_OF_FILE                       = 1,
    AT_BT_SW_UPDATE_ERROR_IN_THE_UPDATE_REBOOT                   = 2,
    AT_BT_SW_UPDATE_SUCCESS_OF_THE_UPDATE_REBOOT                 = 3,
    AT_BT_SW_UPDATE_ERROR_IN_THE_TRANSFER_OF_THE_FILE_NO_REBOOT  = 4,
    AT_BT_SW_UPDATE_SUCCESS_IN_THE_TRANSFER                      = 5,
    AT_BT_SW_UPDATE_FILE_VERSION_INFOS                           = 6,
    AT_BT_SW_UPDATE_MD5_KEY_RESULT_OK                            = 7,
    AT_BT_SW_UPDATE_MD5_KEY_RESULT_NOT_OK_REBOOT                 = 8
}ATBTSWUpdateResEnum;


typedef enum {
    AT_BT_CONN_AUTH_NOT_CONNECTABLE                              = 0,
    AT_BT_CONN_AUTH_CONNECTABLE                                  = 1,
    AT_BT_CONN_AUTH_NOT_YET_PAIRED_DECIVES_CONNECTABLE           = 2
}ATBTConnAuthorizationResEnum;

typedef enum {
    AT_RTMODE_DEACTIVATE     = 0,
    AT_RTMODE_ACTIVATE       = 1,
    AT_RTMODE_CUSTOM8        = 8,
    AT_RTMODE_CUSTOM9        = 9,
    AT_RTMODE_VIBORBELLZ     = 10,
    AT_RTMODE_BIMBAM         = 11,
    AT_RTMODE_EASYLISTEN     = 12,
    AT_RTMODE_FUTUREBEAT     = 13,
    AT_RTMODE_ENDLESS        = 14,
    AT_RTMODE_RINGRING       = 15,
    AT_RTMODE_NOSTALGIC      = 16,
    AT_RTMODE_ORCHESTRA1     = 17,
    AT_RTMODE_ORCHESTRA2     = 18,
    AT_RTMODE_SWEETSOFT      = 19,
    AT_RTMODE_TUTTUT         = 20,
    AT_RTMODE_SYSTEM_STOP    = 21,
    AT_RTMODE_SYSTEM_START   = 22,
    AT_RTMODE_DIS_BLUETOOTH  = 23,
    AT_RTMODE_APP_BLUETOOTH  = 24,
    AT_RTMODE_INVALID        = 25
//    AT_RTMODE_VW_12          = 21,
//    AT_RTMODE_VW_13          = 22,
//    AT_RTMODE_VW_14          = 23,
//    AT_RTMODE_VW_15          = 24,
} ATRintoneModeEnum;

#define AT_SIVR_RINGTONE_MIN            AT_RTMODE_VIBORBELLZ
#define AT_SIVR_RINGTONE_MAX            AT_RTMODE_TUTTUT
#define AT_SIVR_NUMBER_OF_RINGTONES     (AT_SIVR_RINGTONE_MAX - AT_SIVR_RINGTONE_MIN +1)


typedef enum {
    AT_RTACT_CHGD_DEV_ALL    = 0, /* this is default if no action given */
    AT_RTACT_CHGD_DEV_NEW    = 1,
    AT_RTACT_CHGD_ACT_ONLY   = 2
} ATRingtoneReqActionEnum;


typedef enum {
    AT_SIVR_LANG_US_ENG      = 0,
    AT_SIVR_LANG_UK_ENG      = 1,
    AT_SIVR_LANG_FRENCH      = 2,
    AT_SIVR_LANG_FRENCH_CAN  = 3,
    AT_SIVR_LANG_GERMAN      = 4,
    AT_SIVR_LANG_ITALIAN     = 5,
    AT_SIVR_LANG_SPANISH     = 6,
    AT_SIVR_LANG_DUTCH       = 7,
    AT_SIVR_LANG_CHINESE_M   = 8,
    AT_SIVR_LANG_US_SPANISH  = 9,
    AT_SIVR_LANG_KOREAN     = 10,
    AT_SIVR_LANG_JAPANESE   = 11,
    AT_SIVR_LANG_PORTUGUES  = 12,
    AT_SIVR_LANG_BRAZILIAN  = 13,
    AT_SIVR_LANG_RUSSIAN    = 14,
    AT_SIVR_LANG_POLISH     = 15,
    AT_SIVR_LANG_TURKEY     = 16,
    AT_SIVR_LANG_DANISH     = 17,
    AT_SIVR_LANG_SWEDISH    = 18,
    AT_SIVR_LANG_CZECH      = 19,
    AT_SIRV_LANG_MAXNUM     = 20 /* use for language index table */
} ATSIVRLanguagesEnum;
#define AT_RGSL_MAX_STOP_INDEX  (AT_SIRV_LANG_MAXNUM)

/* NOTE - sort lowest to highest number to allow range check */
typedef enum {
    AT_SIVR_CMD_UNKNOWN           =     0, /* to allow proper range check for not reco cmd */
    AT_SIVR_CMD_HELP              =     1,
    AT_SIVR_CMD_CANCEL            =     2,
    AT_SIVR_CMD_MAIN              =     3, /* not a valid CK5050 command but used in VCR */
    AT_SIVR_CMD_IDLE              =     4, /* not a valid CK5050 command but used in VCR */
    /* commands from main menu */
    AT_SIVR_CMD_DIAL              =   101,
    AT_SIVR_CMD_DIAL_NUMBER       =   102,
    AT_SIVR_CMD_REDIAL            =   103,
    AT_SIVR_CMD_MUSIC             =   104,
    AT_SIVR_CMD_MORE_OPTIONS      =   105,
    AT_SIVR_CMD_PLAY_ADDRESSBOOK  =   106,
    AT_SIVR_CMD_READ_SMS          =   107,
    AT_SIVR_CMD_CALL_STACKS       =   108,
    AT_SIVR_CMD_SETTINGS          =   109,
    AT_SIVR_CMD_VOICE_MEMOS       =   110,
    AT_SIVR_CMD_TRAINING          =   111,

    /* commands from DialNameConfirm Menu */
    AT_SIVR_CMD_DIAL_DIALNAMECONFIRM   =   201,
    AT_SIVR_CMD_YES_DIALNAMECONFIRM    =   202,
    AT_SIVR_CMD_NO_DIALNAMECONFIRM     =   203,

    /* commands from DialNameConfirmCategories Menu */
    AT_SIVR_CMD_CATEGORY               =   301,

    /* commands from DialNameConfirmResolveAmbiguouseName Menu */
    AT_SIVR_CMD_YES_RESOLVEAMBINAME    =   401,
    AT_SIVR_CMD_REPEAT_NAMES           =   402,
    AT_SIVR_CMD_NO_RESOLVEAMBINAME     =   403,

    /* commands from DialNumber Menu */
    AT_SIVR_CMD_DIALNUM_D_BLOCK        =   501,

    /* commands from DialNumberCorrect Menu */
    AT_SIVR_CMD_DIAL_DIALNUMCORRECT    =   601, // wählen
    AT_SIVR_CMD_SAVE_DIALNUMCORRECT    =   602, //
    AT_SIVR_CMD_NUMCORRECT_D_BLOCK     =   603, // weitere ziffern
    AT_SIVR_CMD_CORRECTION             =   604, // letzte löschen
    AT_SIVR_CMD_DELETE_DIALNUMCORRECT  =   605, // löschen
    AT_SIVR_CMD_REPEAT_DIALNUMCORRECT  =   606, // wiederholen

    /* commands from Music Menu */
    AT_SIVR_CMD_MUSIC_PLAY             =   701,
    AT_SIVR_CMD_MUSIC_PAUSE            =   702,
    AT_SIVR_CMD_MUSIC_STOP             =   703,
    AT_SIVR_CMD_MUSIC_NEXT_TRACK       =   704,
    AT_SIVR_CMD_MUSIC_PREV_TRACK       =   705,

    /* commands from options menu */
    AT_SIVR_CMD_OPT_PLAY_ADDBOOK       =   801,
    AT_SIVR_CMD_OPT_READ_SMS           =   802,
    AT_SIVR_CMD_OPT_CALL_STACKS        =   803,
    AT_SIVR_CMD_OPT_SETTINGS           =   804,
    AT_SIVR_CMD_OPT_VOICE_MEMOS        =   805,
    AT_SIVR_CMD_OPT_TRAINING           =   806,

    /* commands from play Addressbook menu */
    AT_SIVR_CMD_PAB_ADD_NEW_CONTACT    =   901,
    AT_SIVR_CMD_PAB_NAME_PLAY_ADDBOOK  =   902,
    AT_SIVR_CMD_PAB_READ_ALL           =   903,
    AT_SIVR_CMD_PAB_REFRESH_ADDBOOK    =   904,
    AT_SIVR_CMD_PAB_QS_PLAY_ADDBOOK    =   905,

    /* commands from play addressbook entry menu */
    AT_SIVR_CMD_PAB_YES_PLAY_PAB_ENTRY =  1001,
    AT_SIVR_CMD_PAB_REPEAT_PAB_ENTRY   =  1002,
    AT_SIVR_CMD_PAB_NO_PLAY_PAB_ENTRY  =  1003,

    /* commands from play addressbook options */
    AT_SIVR_CMD_PAB_OPT_DIAL           =  1101,
    AT_SIVR_CMD_PAB_OPT_CHANGE_NAME    =  1102,
    AT_SIVR_CMD_PAB_OPT_RESET_NAME     =  1103,
 /*   AT_SIVR_CMD_PAB_OPT_NEW_NAME       =  1104,
    AT_SIVR_CMD_PAB_OPT_QS             =  1105, */
    AT_SIVR_CMD_PAB_OPT_GO_AHEAD       =  1104,
    AT_SIVR_CMD_PAB_OPT_GO_BACK        =  1105,
    AT_SIVR_CMD_PAB_OPT_GO_AHEAD_OLD   =  1106,
    AT_SIVR_CMD_PAB_OPT_GO_BACK_OLD    =  1107,
    AT_SIVR_CMD_PAB_OPT_NEXT_LETTER    =  1108,
    AT_SIVR_CMD_PAB_OPT_PREV_LETTER    =  1109,

    /* commands from resolve multiple numbers menu */
    AT_SIVR_CMD_YES_RES_MULTIPLENUM    =  1201,
    AT_SIVR_CMD_REPEAT_NUMBERS         =  1202,
    AT_SIVR_CMD_NO_RES_MULTIPLENUM     =  1203,

    /* commands from reset name confirm menu */
    AT_SIVR_CMD_YES_RESETNAMECONFIRM   =  1301,
    AT_SIVR_CMD_NO_RESETNAMECONFIRM    =  1302,

    /* commands from call stacks menu */
    AT_SIVR_CMD_CSTACK_RECEIVED_CALL   =  1401,
    AT_SIVR_CMD_CSTACK_MISSED_CALL     =  1402,
    AT_SIVR_CMD_CSTACK_DIALLED_NUMBERS =  1403,
    AT_SIVR_CMD_CSTACK_CLEAR_ALL_STACK =  1404,

    /* commands from read call stacks menu */
    AT_SIVR_CMD_CSTACK_YES_READ        =  1501,
    AT_SIVR_CMD_CSTACK_REPEAT          =  1502,
    AT_SIVR_CMD_CSTACK_NO_READ         =  1503,

    /* commands from call stack options menu */
    AT_SIVR_CMD_CSTACK_DIAL_CSOPT      =  1601,

    /* commands from call stack reset confirm menu */
    AT_SIVR_CMD_CSTACK_YES_RESETCONF   =  1701,
    AT_SIVR_CMD_CSTACK_NO_RESETCONF    =  1702,

    /* commands from settings menu */
    AT_SIVR_CMD_SET_VF_OFF             =  1801,
    AT_SIVR_CMD_SET_VF_ON              =  1802,
    AT_SIVR_CMD_SET_BT_OPTIONS         =  1803,
    AT_SIVR_CMD_SET_RINGTONE           =  1804,
    AT_SIVR_CMD_SET_CHG_LANGUAGE       =  1805,
    AT_SIVR_CMD_SET_CHG_VM_NUMBER      =  1806,
    AT_SIVR_CMD_SET_DELETE_VTAGS       =  1807,
    AT_SIVR_CMD_SET_RESTORE_FACTORY    =  1808,

    /* commands from BT options menu */
    AT_SIVR_CMD_BTOPT_BT_VISIBLE       =  1901,
    AT_SIVR_CMD_BTOPT_CHG_PIN          =  1902,

    /* commands from ChangeBluetoothPIN menu */
    AT_SIVR_CMD_BTPIN_BLOCK            =  2001,

    /* commands from ChangeBluetoothPINCorrect menu */
    AT_SIVR_CMD_BTPIN_COR_SAVE         =  2101,
    AT_SIVR_CMD_BTPIN_COR_BLOCK        =  2102,
    AT_SIVR_CMD_BTPIN_COR_CORRECT      =  2103,
    AT_SIVR_CMD_BTPIN_COR_DELETE       =  2104,
    AT_SIVR_CMD_BTPIN_COR_REPEAT       =  2105,

    /* commands for CallBackMissedCalls */
    AT_SIVR_CMD_MISSED_CALL_DIAL       =  2201,
    AT_SIVR_CMD_MISSED_CALL_YES        =  2202,
    AT_SIVR_CMD_MISSED_CALL_NO         =  2203,

    /* commands from RestoreFactoryDefaultsConfirm menu */
    AT_SIVR_CMD_RESTOREFACTORY_YES     =  2301,
    AT_SIVR_CMD_RESTOREFACTORY_NO      =  2302,

    /* commands from Ringtone menu */
    AT_SIVR_CMD_RINGTONE_YES           =  2401,
    AT_SIVR_CMD_RINGTONE_REPEAT        =  2402,
    AT_SIVR_CMD_RINGTONE_NO            =  2403,

    /* commands from RingtoneOptions menu */
    AT_SIVR_CMD_RINGTONEOPT_CONFIRM    =  2501,
    AT_SIVR_CMD_RINGTONEOPT_GOAHEAD    =  2502,
    AT_SIVR_CMD_RINGTONEOPT_GOBACK     =  2503,

    /* commands from ChangeLanguage menu */
    AT_SIVR_CMD_CHGLANG_GERMAN         =  2601,
    AT_SIVR_CMD_CHGLANG_SPANISH        =  2602,
    AT_SIVR_CMD_CHGLANG_FRENCH         =  2603,
    AT_SIVR_CMD_CHGLANG_ENGLISH        =  2604,
    AT_SIVR_CMD_CHGLANG_CZECH          =  2605,
    AT_SIVR_CMD_CHGLANG_ITALIAN        =  2606,
    AT_SIVR_CMD_CHGLANG_PORTUGEST      =  2607,
    AT_SIVR_CMD_CHGLANG_RUSSIAN        =  2608,
    AT_SIVR_CMD_CHGLANG_DUTCH          =  2609,

    /* commands from ChangeLanguageConfirm menu */
    AT_SIVR_CMD_CHANGLANGCONF_YES      =  2701,
    AT_SIVR_CMD_CHANGLANGCONF_NO       =  2702,

    /* commands from DeleteAllVoiceTagsConfirm menu */
    AT_SIVR_CMD_DELETEVT_CONF_YES      =  2801,
    AT_SIVR_CMD_DELETEVT_CONF_NO       =  2802,

    /* commands from VoiceMemos menu */
    AT_SIVR_CMD_VMEMO_NEW              =  2901,
    AT_SIVR_CMD_VMEMO_PLAY             =  2902,
    AT_SIVR_CMD_VMEMO_DELETE           =  2903,

    /* commands from VoiceMemosPlayMemos menu */
    AT_SIVR_CMD_VMEMO_PLAY_YES         =  3001,
    AT_SIVR_CMD_VMEMO_PLAY_REPEAT      =  3002,
    AT_SIVR_CMD_VMEMO_PLAY_NO          =  3003,

    /* commands from VoiceMemosPlayMemosOptions menu */
    AT_SIVR_CMD_VMEMO_OPT_NEXT         =  3101,
    AT_SIVR_CMD_VMEMO_OPT_PREVIOUS     =  3102,
    AT_SIVR_CMD_VMEMO_OPT_DELETE       =  3103,

    /* commands from VoiceMemosConfirmDelete menu */
    AT_SIVR_CMD_VMEMO_CONF_YES         =  3201,
    AT_SIVR_CMD_VMEMO_CONF_DELETE      =  3202,
    AT_SIVR_CMD_VMEMO_CONF_NO          =  3203,

    /* commands from VoiceMemosConfirmDeleteAll menu */
    AT_SIVR_CMD_VMEMO_CONFDEL_YES      =  3301,
    AT_SIVR_CMD_VMEMO_CONFDEL_DELETE   =  3302,
    AT_SIVR_CMD_VMEMO_CONFDEL_NO       =  3303,

    /* commands from Training menu */
    AT_SIVR_CMD_TRAINING_YES           =  3401,
    AT_SIVR_CMD_TRAINING_NO            =  3402,

    /* commands from ReadSMS menu */
    AT_SIVR_CMD_READSMS_YES            =  3501,
    AT_SIVR_CMD_READSMS_REPEAT         =  3502,
    AT_SIVR_CMD_READSMS_NO             =  3503,

    /* commands from ReadSMSOptions menu */
    AT_SIVR_CMD_READSMSOPT_CALL_YES    =  3601,
    AT_SIVR_CMD_READSMSOPT_REPEAT      =  3602,
    AT_SIVR_CMD_READSMSOPT_DELETE      =  3603,
    AT_SIVR_CMD_READSMSOPT_PREVIOUS    =  3604,
    AT_SIVR_CMD_READSMSOPT_NEXT        =  3605,

    /* commands from DeleteSMSConfirm menu */
    AT_SIVR_CMD_DELETESMS_YES          =  3701,
    AT_SIVR_CMD_DELETESMS_DELETE       =  3702,
    AT_SIVR_CMD_DELETESMS_NO           =  3703,

    /* commands from CallBackSMSConfirm menu */
    AT_SIVR_CMD_CALLBACKSMS_YES        =  3801,
    AT_SIVR_CMD_CALLBACKSMS_DIAL       =  3802,
    AT_SIVR_CMD_CALLBACKSMS_NO         =  3803,

    /* commands from StoreName menu */
    AT_SIVR_CMD_STORE_NAME_REC         =  4001
} ATSIVRCmdIdEnum;


typedef enum {
    /* INVALID MENU ID USED for SIVR state handling    */
    AT_SIVR_MENU_IDLE                             =    0,
    /* REAL SIVR MENU IDs */
    AT_SIVR_MENU_MAIN                               =    1,
    AT_SIVR_MENU_DIALNAMECONFIRM                    =    2,
    AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES          =    3,
    AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES    =    4,
    AT_SIVR_MENU_DIALNUMBER                         =    5,
    AT_SIVR_MENU_DIALNUMBERCORRECT                  =    6,
    AT_SIVR_MENU_MUSIC                              =    7,
    AT_SIVR_MENU_MOREOPTIONS                        =    8,
    AT_SIVR_MENU_PLAYADDRESSBOOK                    =    9,
    AT_SIVR_MENU_PLAYADDRESSBOOKENTRY               =    10,
    AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS             =    11,
    AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY      =    12,
    AT_SIVR_MENU_RESETNAMECONFIRM                   =    13,
    AT_SIVR_MENU_CALLSTACKS                         =    14,
    AT_SIVR_MENU_READCALLSTACKS                     =    15,
    AT_SIVR_MENU_CALLSTACKSOPTIONS                  =    16,
    AT_SIVR_MENU_CALLSTACKSRESETCONFIRM             =    17,
    AT_SIVR_MENU_SETTINGS                           =    18,
    AT_SIVR_MENU_BLUETOOTHOPTIONS                   =    19,
    AT_SIVR_MENU_CHANGEBLUETOOTHPIN                 =    20,
    AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT          =    21,
 /*   AT_SIVR_MENU_CHANGEVOICEMAILNUMBER              =    22,*/
    AT_SIVR_MENU_CALLBACKMISSEDCALLS                =    22,
    AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM      =    23,
    AT_SIVR_MENU_RINGTONE                           =    24,
    AT_SIVR_MENU_RINGTONEOPTIONS                    =    25,
    AT_SIVR_MENU_CHANGELANGUAGE                     =    26,
    AT_SIVR_MENU_CHANGELANGUAGECONFIRM              =    27,
    AT_SIVR_MENU_DELETEALLVOICETAGSCONFIRM          =    28,
    AT_SIVR_MENU_VOICEMEMOS                         =    29,
    AT_SIVR_MENU_VOICEMEMOSPLAYMEMOS                =    30,
    AT_SIVR_MENU_VOICEMEMOSPLAYMEMOSOPTIONS         =    31,
    AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETE            =    32,
    AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETEALL         =    33,
    AT_SIVR_MENU_TRAINING                           =    34,
    AT_SIVR_MENU_READSMS                            =    35,
    AT_SIVR_MENU_READSMSOPTIONS                     =    36,
    AT_SIVR_MENU_DELETESMSCONFIRM                   =    37,
    AT_SIVR_MENU_CALLBACKSMSCONFIRM                 =    38,
    AT_SIVR_MENU_TRAININGREADPROMPT                 =    39,
    AT_SIVR_MENU_STORENAME                          =    40,

    /* add an offset definition similar to last short menu */
    AT_SIVR_LONG_MENU_OFFSET                        =    AT_SIVR_MENU_STORENAME,
    AT_SIVR_MENU_MAIN_L                             =    41,
    AT_SIVR_MENU_DIALNAMECONFIRM_L                  =    42,
    AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES_L        =    43,
    AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES_L  =    44,
    AT_SIVR_MENU_DIALNUMBER_L                       =    45,
    AT_SIVR_MENU_DIALNUMBERCORRECT_L                =    46,
    AT_SIVR_MENU_MUSIC_L                            =    47,
    AT_SIVR_MENU_MOREOPTIONS_L                      =    48,
    AT_SIVR_MENU_PLAYADDRESSBOOK_L                  =    49,
    AT_SIVR_MENU_PLAYADDRESSBOOKENTRY_L             =    50,
    AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS_L           =    51,
    AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY_L    =    52,
    AT_SIVR_MENU_RESETNAMECONFIRM_L                 =    53,
    AT_SIVR_MENU_CALLSTACKS_L                       =    54,
    AT_SIVR_MENU_READCALLSTACKS_L                   =    55,
    AT_SIVR_MENU_CALLSTACKSOPTIONS_L                =    56,
    AT_SIVR_MENU_CALLSTACKSRESETCONFIRM_L           =    57,
    AT_SIVR_MENU_SETTINGS_L                         =    58,
    AT_SIVR_MENU_BLUETOOTHOPTIONS_L                 =    59,
    AT_SIVR_MENU_CHANGEBLUETOOTHPIN_L               =    60,
    AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT_L        =    61,
  /*  AT_SIVR_MENU_CHANGEVOICEMAILNUMBER_L            =    62,*/
    AT_SIVR_MENU_CALLBACKMISSEDCALLS_L              =    62,
    AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM_L    =    63,
    AT_SIVR_MENU_RINGTONE_L                         =    64,
    AT_SIVR_MENU_RINGTONEOPTIONS_L                  =    65,
    AT_SIVR_MENU_CHANGELANGUAGE_L                   =    66,
    AT_SIVR_MENU_CHANGELANGUAGECONFIRM_L            =    67,
    AT_SIVR_MENU_DELETEALLVOICETAGSCONFIRM_L        =    68,
    AT_SIVR_MENU_VOICEMEMOS_L                       =    69,
    AT_SIVR_MENU_VOICEMEMOSPLAYMEMOS_L              =    70,
    AT_SIVR_MENU_VOICEMEMOSPLAYMEMOSOPTIONS_L       =    71,
    AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETE_L          =    72,
    AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETEALL_L       =    73,
    AT_SIVR_MENU_TRAINING_L                         =    74,
    AT_SIVR_MENU_READSMS_L                          =    75,
    AT_SIVR_MENU_READSMSOPTIONS_L                   =    76,
    AT_SIVR_MENU_DELETESMSCONFIRM_L                 =    77,
    AT_SIVR_MENU_CALLBACKSMSCONFIRM_L               =    78,
    AT_SIVR_MENU_TRAININGREADPROMPT_L               =    79,
    AT_SIVR_MENU_STORENAME_L                        =    80,

    AT_SIVR_MENU_UNDEF                              =  0xFFFF,

    /* now the general prompts / system prompts */
    AT_SIVR_PRMT_UNDEF                                       =    0,
/* system context */
    /* TODO check if prompts 1..602 are needed at all */
    AT_SIVR_PRMT_LOC_HOME                                    =    1,    // "zu Hause"
    AT_SIVR_PRMT_LOC_WORK                                    =    2,    // "arbeit"
    AT_SIVR_PRMT_LOC_MOBILE                                  =    4,    // "mobil"
    AT_SIVR_PRMT_LOC_OTHER                                   =    8,    // "allgemein"
    AT_SIVR_PRMT_LOC_DEFAULT                                 =   16,    // "default" location is not allowed by VW recognition specification, so this location does not match any prompt
    AT_SIVR_PRMT_BEEP                                        =   10,
    AT_SIVR_PRMT_RECO_OFF                                    =   11,
    AT_SIVR_PRMT_TIMEOUT                                     =   12,
    AT_SIVR_PRMT_PBDL_IPR                                    =   14,
    AT_SIVR_PRMT_PBDL_DONE                                   =   15,
    AT_SIVR_PRMT_NO_PAIRED                                   =   17,
    AT_SIVR_PRMT_NO_CONNECTED                                =   18,
    AT_SIVR_PRMT_NUMBER_FULL                                 =   19,
    AT_SIVR_PRMT_WAIT                                        =   20,
    AT_SIVR_PRMT_DIALING                                     =  602,

    AT_SIVR_LONG_SYSPROMPT_OFFSET                            = 1000,
    AT_SIVR_PRMT_SELECTED_LANGUAGE_NOT_AVAILABLE             = 1000,
    AT_SIVR_PRMT_NETWORK_NOT_AVAILABLE                       = 1001,
    AT_SIVR_PRMT_CALL_STACKS_RESET                           = 1002,
    AT_SIVR_PRMT_NAME_LIST_FULL_PLEASE_DELETE                = 1003,
    AT_SIVR_PRMT_MULTIPLE_NUMBERS_POSSIBLE_CHOOSE_HAPTICAL   = 1004,
    AT_SIVR_PRMT_T2_DIALNUMBERCORRECTSAVE                    = 1005,
    AT_SIVR_PRMT_ADDRESSBOOK_EMPTY                           = 1006,
    AT_SIVR_PRMT_TRAINING_FAILED                             = 1007,
    AT_SIVR_PRMT_VOICE_MEMO_SUCCESSFULLY_STORED              = 1008,
    AT_SIVR_PRMT_CHOOSE_LANGUAGE_FROM_SETTINGS               = 1009,
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOSCONFIRMDELETEALL       = 1010,
    AT_SIVR_PRMT_NAME_TOO_SHORT                              = 1011,
    AT_SIVR_PRMT_T2_OPTIONS_RESET_NAME_CONFIRM               = 1012,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_BLUETOOTH_PIN_CORRECT     = 1013,
    AT_SIVR_PRMT_NUMBER_STORED_FOR_VOICE_MAIL_CHANGE_IT      = 1014,
    AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK_OPTIONS         = 1015,
    AT_SIVR_PRMT_PHONE_NOT_AVAILABLE                         = 1016,
    AT_SIVR_PRMT_NO_FURTHER_ENTRIES_IN_LIST_REPEAT           = 1017,
    AT_SIVR_PRMT_USE_PLAYADDRESSBOOK                         = 1018,
    AT_SIVR_PRMT_PIN_DELETED                                 = 1019,
    AT_SIVR_PRMT_ALL_MEMOS_DELETED                           = 1020,
    AT_SIVR_PRMT_H_DIALNUMBERCORRECTSAVE                     = 1021,
    AT_SIVR_PRMT_CONNECT_NAME_CAT                            = 1022,
    AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS                      = 1023,
    AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME                    = 1024,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_LANGUAGE_CONFIRM          = 1025,
    AT_SIVR_PRMT_T2_OPTIONS_CALLBACK_SMS_CONFIRM             = 1026,
    AT_SIVR_PRMT_PIN_TOO_LONG                                = 1027,
    AT_SIVR_PRMT_END_OF_NAME_LIST_BUTTON_LIST_IS_REPEAT      = 1028,
    AT_SIVR_PRMT_T2_OPTIONS_TRAINING                         = 1029,
    AT_SIVR_PRMT_CONNECT_NAME                                = 1030,
    AT_SIVR_PRMT_MUSIC_PAUSE                                 = 1031,
    AT_SIVR_PRMT_I_ADD_NEW_CONTACT                           = 1032,
    AT_SIVR_PRMT_REFRESH_ADDRESSBOOK                         = 1033,
    AT_SIVR_PRMT_I_NEW_CONTACT_NUMBER_PLEASE                 = 1034,
    AT_SIVR_PRMT_NAME_STORED                                 = 1035,
    AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK                 = 1036,
    AT_SIVR_PRMT_T1_MAINMENU                                 = 1037,
    AT_SIVR_PRMT_END_OF_ADDRESSBOOK_REPEAT                   = 1038,
    AT_SIVR_PRMT_T2_OPTIONS_RINGTONEOPTIONS                  = 1039,
    AT_SIVR_PRMT_D_NAME                                      = 1040,
    AT_SIVR_PRMT_T2_OPTIONS_MUSIC                            = 1041,
    AT_SIVR_PRMT_NUMBER_NOT_AVAIlABLE                        = 1042,
    AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE                     = 1043,
    AT_SIVR_PRMT_T2_READ_SMS_OPTIONS                         = 1044,
    /*  AT_SIVR_PRMT_T2_OPTIONS_CHANGE_VOICE_MAIL_NUMBER     = 1045, */
    AT_SIVR_PRMT_TRAINING_SUCCESSFULLY_COMPLETED             = 1046,
    AT_SIVR_PRMT_NO_FURTHER_SMS                              = 1047,
    AT_SIVR_PRMT_MEMORY_FULL                                 = 1048,
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOS                       = 1049,
    AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_CAT                = 1050,
    AT_SIVR_PRMT_T2_OPTIONS_RESOLVE_MULTIPLE_NUMBERS         = 1051,
    AT_SIVR_PRMT_ADDRESSBOOK_DOWNLOAD_IN_PROGRESS            = 1052,
    AT_SIVR_PRMT_T2_NUMBER_PLEASE                            = 1053,
    AT_SIVR_PRMT_D_TEXTTAG_X_MINUS1                          = 1054,
    AT_SIVR_PRMT_BLUETOOTH_VISIBLE                           = 1055,
    AT_SIVR_PRMT_SYSTEM_START                                = 1056,
    AT_SIVR_PRMT_NEW_BLUETOOTH_PIN_SAVED                     = 1057,
    /*  AT_SIVR_PRMT_VOICE_MAIL_NUMBER_NOT_AVAILABLE             = 1058, */
    AT_SIVR_PRMT_T2_RESOLVE_AMBIGUOUS_NAMES_OPTIONS          = 1059,
    AT_SIVR_PRMT_T2_DIALNUMBERCORRECT                        = 1060,
    AT_SIVR_PRMT_PRESS_PTI_TO_STOP                           = 1061,
    AT_SIVR_PRMT_WRONG_PIN_LENGTH                            = 1062,
    AT_SIVR_PRMT_NUMBER_TOO_LONG                             = 1063,
    AT_SIVR_PRMT_SMS                                         = 1064,
    AT_SIVR_PRMT_T2_OPTIONS_RINGTONE                         = 1065,
    AT_SIVR_PRMT_PLEASE_CONTINUE_D_BLOCK                     = 1066,
    AT_SIVR_PRMT_MULTIPLE_NAMES_POSSIBLE_CHOOSE_HAPTICAL     = 1067,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_BLUETOOTH_PIN             = 1068,
    AT_SIVR_PRMT_ALL_AUTOMATIC_TRANSCRIPTIONS_RESTORED       = 1069,
    AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_RESET_CONFIRM        = 1070,
    AT_SIVR_PRMT_T2_MAINMENU_OPTIONS                         = 1071,
    AT_SIVR_PRMT_MEMO_DELETED                                = 1072,
/*    AT_SIVR_PRMT_AMBIGUOUS_NAME                              = 1073, */
    AT_SIVR_PRMT_LANGUAGE_FOR                                = 1074,
    AT_SIVR_PRMT_RINGTONE_SELECTED                           = 1075,
    AT_SIVR_PRMT_T2_OPTIONS_RESTOREFACTORYDEFAULTSCONFIRM    = 1076,
    AT_SIVR_PRMT_T2_OPTIONS_DELETE_ALL_VOICETAGS_CONFIRM     = 1077,
    AT_SIVR_PRMT_RESET_NOT_POSSIBLE                          = 1078,
/*    AT_SIVR_PRMT_PRESS_PTT_QUICK_SEARCH                      = 1079, */
    AT_SIVR_PRMT_T2_OPTIONS_MORE_OPTIONS                     = 1080,
    AT_SIVR_PRMT_MUSIC_STOP                                  = 1081,
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOSCONFIRMDELETE          = 1082,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_LANGUAGE                  = 1083,
    AT_SIVR_PRMT_NOT_ENOUGH_SPACE_DELETE_MEMO                = 1084,
    AT_SIVR_PRMT_I_START_SMS_PLAYBACK                        = 1085,
    AT_SIVR_PRMT_SMS_XPLUSPLUS                               = 1086,
    AT_SIVR_PRMT_VOICE_FEEDBACK_ON                           = 1087,
    AT_SIVR_PRMT_EON_LIST_PRESS_BUTTON_IS_READ_REPEAT        = 1088,
/*    AT_SIVR_PRMT_I_VOICE_MAIL_NUMBER_PLEASE                  = 1089, */
    AT_SIVR_PRMT_LESS_THAN_ONE_MINUTE_OF_RECORDING_TIME      = 1090,
    AT_SIVR_PRMT_D_TEXTTAG                                   = 1091,
    AT_SIVR_PRMT_T2_OPTIONS_DIALNAME_CONFIRM_CAT             = 1092,
    AT_SIVR_PRMT_NO_PLAYER_AVAILABLE                         = 1093,
    AT_SIVR_PRMT_H_DIALNAME                                  = 1094,
    AT_SIVR_PRMT_T2_OPTIONS_DELETE_SMS_CONFIRM               = 1095,
    AT_SIVR_PRMT_SMS_DELETED                                 = 1096,
    AT_SIVR_PRMT_TRAINING_NOT_SUCCESSFUL                     = 1097,
    AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_OPTIONS              = 1098,
    AT_SIVR_PRMT_NO_FURTHER_NEW_SMS                          = 1099,
    AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE                 = 1100,
    AT_SIVR_PRMT_T2_OPTIONS_PLAYMEMOSOPTIONS                 = 1101,
    AT_SIVR_PRMT_I_CALLBACK_SENDER                           = 1102,
    AT_SIVR_PRMT_I_DIALNAME_CONFIRM_NAME                     = 1103,
    AT_SIVR_PRMT_NUMBER_DELETED                              = 1104,
    AT_SIVR_PRMT_NO_SMS                                      = 1105,
    AT_SIVR_PRMT_NO_FURTHER_VOICE_MEMOS                      = 1106,
    AT_SIVR_PRMT_NO_ENTRY_IN_CHOSEN_LIST                     = 1107,
    AT_SIVR_PRMT_NAME_TOO_LONG                               = 1108,
    AT_SIVR_PRMT_Y_MINUTES_OF_RECORDING_TIME_LEFT            = 1109,
    AT_SIVR_PRMT_T2_OPTIONS_BLUETOOTH_OPTIONS                = 1110,
    AT_SIVR_PRMT_T2_NAME_NOT_FOUND_NAME_PLEASE               = 1111,
    AT_SIVR_PRMT_REPEAT_SMS                                  = 1112,
    AT_SIVR_PRMT_SAY_TRAINING_PROMPT                         = 1113,
    AT_SIVR_PRMT_SYSTEM_STOP                                 = 1114,
    AT_SIVR_PRMT_CONTACT_SUCCESSFULLY_ENTERED                = 1115,
    AT_SIVR_PRMT_ORIGINAL_SETTINGS_RESTORED                  = 1116,
    AT_SIVR_PRMT_I_REDIAL                                    = 1117,
    AT_SIVR_PRMT_PRESS_PTT                                   = 1118,
    AT_SIVR_PRMT_FUNCTION_NOT_AVAILABLE                      = 1119,
/*    AT_SIVR_PRMT_RINGTONE_X                                  = 1120,*/
    AT_SIVR_PRMT_DO_YOU_WANT_TO_DIAL_NAME_CATEGORY           = 1121,
    AT_SIVR_PRMT_NO_DEVICE                                   = 1122,
    AT_SIVR_PRMT_T2_OPTIONS_READSMS                          = 1123,
    AT_SIVR_PRMT_T2_OPTIONS_SETTINGS                         = 1124,
    AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME               = 1125,
    AT_SIVR_PRMT_VOICE_FEEDBACK_OFF                          = 1126,
    AT_SIVR_PRMT_NO_NEXT_MESSAGE                             = 1127,
    AT_SIVR_PRMT_NEW_VOICE_MAIL_NUMBER_SAVED                 = 1128,
    AT_SIVR_PRMT_CHOOSE_OTHER_CATEGORY_INSTEAD               = 1129,
    AT_SIVR_PRMT_NO_VOICE_MEMO_AVAILABLE                     = 1130,
    AT_SIVR_PRMT_D_TEXTTAG_X                                 = 1131,
/*    AT_SIVR_PRMT_X_NUMBER                                    = 1132,*/
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOSPLAYMEMOS              = 1133,
    AT_SIVR_PRMT_NO_PREVIOUS_MESSAGE                         = 1134,
    AT_SIVR_PRMT_T2_OPTIONS_STORENAME                        = 1135,
/*    AT_SIVR_PRMT_I_DIAL_NAME_NAME_PLEASE                     = 1136, */
    AT_SIVR_PRMT_T1_SORRY                                    = 1137,
    AT_SIVR_PRMT_END_OF_RINGTONE_LIST_REPEAT                 = 1138,
/*    AT_SIVR_PRMT_ENTER_IT_NOW                                = 1139, */
    AT_SIVR_PRMT_AUTOMATIC_TRANSCRIPTION_RESTORED            = 1140,
    AT_SIVR_PRMT_NO_REMOTE_CONTROL_POSSIBLE                  = 1141,
    AT_SIVR_PRMT_CONNECT                                     = 1142,
    AT_SIVR_PRMT_PROCESS_CANCELLED                           = 1143,
    AT_SIVR_PRMT_CHANGIN_LANGUAGE_PLEASE_WAIT                = 1144,
    AT_SIVR_PRMT_T2_OPTIONS_READ_CALL_STACKS                 = 1145,
    AT_SIVR_PRMT_MISSED_CALL_FROM                            = 1146,
    AT_SIVR_PRMT_MISSED_CALL_FROM_UNKNOWN                    = 1147,
    AT_SIVR_PRMT_MISSED_CALL_SEVERAL                         = 1148,
    AT_SIVR_PRMT_T2_OPTIONS_CALLBACKMISSEDCALLS              = 1149,
    AT_SIVR_PRMT_START_DELETE_ALL_VT                         = 1150,
    AT_SIVR_PRMT_RTMODE_RINGRING                             = 1151,
    AT_SIVR_PRMT_RTMODE_EASYLISTEN                           = 1152,
    AT_SIVR_PRMT_RTMODE_ENDLESS                              = 1153,
    AT_SIVR_PRMT_RTMODE_FUTUREBEAT                           = 1154,
    AT_SIVR_PRMT_RTMODE_NOSTALGIC                            = 1155,
    AT_SIVR_PRMT_RTMODE_ORCHESTRA1                           = 1156,
    AT_SIVR_PRMT_RTMODE_ORCHESTRA2                           = 1157,
    AT_SIVR_PRMT_RTMODE_SWEETSOFT                            = 1158,
    AT_SIVR_PRMT_RTMODE_BIMBAM                               = 1159,
    AT_SIVR_PRMT_RTMODE_TUTTUT                               = 1160,
    AT_SIVR_PRMT_RTMODE_VIBORBELLZ                           = 1161,
    AT_SIVR_PRMT_RTMODE_VW_12                                = 1162,
    AT_SIVR_PRMT_RTMODE_VW_13                                = 1163,
    AT_SIVR_PRMT_RTMODE_VW_14                                = 1164,
    AT_SIVR_PRMT_RTMODE_VW_15                                = 1165,
    AT_SIVR_PRMT_PAIRING_BLUETOOTH                           = 1166,
    AT_SIVR_PRMT_DISABLE_BLUETOOTH                           = 1167,

    AT_SIVR_PRMT_MAINMENU_WITHOUT_MUSIC                      = 1168,
    AT_SIVR_PRMT_T1_MAINMENU_WITHOUT_MUSIC                   = 1169,

    AT_SIVR_PRMT_MORE_OPTIONS_WITHOUT_READ_SMS               = 1171,
    AT_SIVR_PRMT_MORE_T2_OPTIONS_WITHOUT_READ_SMS            = 1172,
    AT_SIVR_PRMT_LANGUAGE_ENGLISH                            = 1173,
    AT_SIVR_PRMT_LANGUAGE_SPANISH                            = 1174,
    AT_SIVR_PRMT_LANGUAGE_FRENCH                             = 1175,
    AT_SIVR_PRMT_LANGUAGE_CZECH                              = 1176,
    AT_SIVR_PRMT_LANGUAGE_ITALIAN                            = 1177,
    AT_SIVR_PRMT_LANGUAGE_PORTUGUESE                         = 1178,
    AT_SIVR_PRMT_LANGUAGE_RUSSIAN                            = 1179,
    AT_SIVR_PRMT_LANGUAGE_DUTCH                              = 1180,
    AT_SIVR_PRMT_LANGUAGE_GERMAN                             = 1181,

    AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED                    = 1193,
    AT_SIVR_PRMT_ONLY_MP_CONNECTED_BUT_NO_REMOTE_CONTROL       = 1194,
    AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS    = 1195,
    AT_SIVR_PRMT_NO_DEVICE_CONNECTED_WITH_IC_CONTEXT           = 1196,

    AT_SIVR_PRMT_AMBIGUOUS_NAME                              = 2073,
    AT_SIVR_PRMT_X_NUMBER                                    = 2132,

    /* LONG prompts */
    AT_SIVR_PRMT_SELECTED_LANGUAGE_NOT_AVAILABLE_L             = 2000,
    AT_SIVR_PRMT_NETWORK_NOT_AVAILABLE_L                       = 2001,
    AT_SIVR_PRMT_CALL_STACKS_RESET_L                           = 2002,
    AT_SIVR_PRMT_NAME_LIST_FULL_PLEASE_DELETE_L                = 2003,
    AT_SIVR_PRMT_MULTIPLE_NUMBERS_POSSIBLE_CHOOSE_HAPTICAL_L   = 2004,
    AT_SIVR_PRMT_T2_DIALNUMBERCORRECTSAVE_L                    = 2005,
    AT_SIVR_PRMT_ADDRESSBOOK_EMPTY_L                           = 2006,
    AT_SIVR_PRMT_TRAINING_FAILED_L                             = 2007,
    AT_SIVR_PRMT_VOICE_MEMO_SUCCESSFULLY_STORED_L              = 2008,
    AT_SIVR_PRMT_CHOOSE_LANGUAGE_FROM_SETTINGS_L               = 2009,
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOSCONFIRMDELETEALL_L       = 2010,
    AT_SIVR_PRMT_NAME_TOO_SHORT_L                              = 2011,
    AT_SIVR_PRMT_T2_OPTIONS_RESET_NAME_CONFIRM_L               = 2012,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_BLUETOOTH_PIN_CORRECT_L     = 2013,
    AT_SIVR_PRMT_NUMBER_STORED_FOR_VOICE_MAIL_CHANGE_IT_L      = 2014,
    AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK_OPTIONS_L         = 2015,
    AT_SIVR_PRMT_PHONE_NOT_AVAILABLE_L                         = 2016,
    AT_SIVR_PRMT_NO_FURTHER_ENTRIES_IN_LIST_REPEAT_L           = 2017,
    AT_SIVR_PRMT_USE_PLAYADDRESSBOOK_L                         = 2018,
    AT_SIVR_PRMT_PIN_DELETED_L                                 = 2019,
    AT_SIVR_PRMT_ALL_MEMOS_DELETED_L                           = 2020,
    AT_SIVR_PRMT_H_DIALNUMBERCORRECTSAVE_L                     = 2021,
    AT_SIVR_PRMT_CONNECT_NAME_CAT_L                            = 2022,
    AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_L                      = 2023,
    AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_L                    = 2024,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_LANGUAGE_CONFIRM_L          = 2025,
    AT_SIVR_PRMT_T2_OPTIONS_CALLBACK_SMS_CONFIRM_L             = 2026,
    AT_SIVR_PRMT_PIN_TOO_LONG_L                                = 2027,
    AT_SIVR_PRMT_END_OF_NAME_LIST_BUTTON_LIST_IS_REPEAT_L      = 2028,
    AT_SIVR_PRMT_T2_OPTIONS_TRAINING_L                         = 2029,
    AT_SIVR_PRMT_CONNECT_NAME_L                                = 2030,
    AT_SIVR_PRMT_MUSIC_PAUSE_L                                 = 2031,
    AT_SIVR_PRMT_I_ADD_NEW_CONTACT_L                           = 2032,
    AT_SIVR_PRMT_REFRESH_ADDRESSBOOK_L                         = 2033,
    AT_SIVR_PRMT_I_NEW_CONTACT_NUMBER_PLEASE_L                 = 2034,
    AT_SIVR_PRMT_NAME_STORED_L                                 = 2035,
    AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK_L                 = 2036,
    AT_SIVR_PRMT_T1_MAINMENU_L                                 = 2037,
    AT_SIVR_PRMT_END_OF_ADDRESSBOOK_REPEAT_L                   = 2038,
    AT_SIVR_PRMT_T2_OPTIONS_RINGTONEOPTIONS_L                  = 2039,
    AT_SIVR_PRMT_D_NAME_L                                      = 2040,
    AT_SIVR_PRMT_T2_OPTIONS_MUSIC_L                            = 2041,
    AT_SIVR_PRMT_NUMBER_NOT_AVAIlABLE_L                        = 2042,
    AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE_L                     = 2043,
    AT_SIVR_PRMT_T2_READ_SMS_OPTIONS_L                         = 2044,
    /*  AT_SIVR_PRMT_T2_OPTIONS_CHANGE_VOICE_MAIL_NUMBER_L     = 2045, */
    AT_SIVR_PRMT_TRAINING_SUCCESSFULLY_COMPLETED_L             = 2046,
    AT_SIVR_PRMT_NO_FURTHER_SMS_L                              = 2047,
    AT_SIVR_PRMT_MEMORY_FULL_L                                 = 2048,
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOS_L                       = 2049,
    AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_CAT_L                = 2050,
    AT_SIVR_PRMT_T2_OPTIONS_RESOLVE_MULTIPLE_NUMBERS_L         = 2051,
    AT_SIVR_PRMT_ADDRESSBOOK_DOWNLOAD_IN_PROGRESS_L            = 2052,
    AT_SIVR_PRMT_T2_NUMBER_PLEASE_L                            = 2053,
    AT_SIVR_PRMT_D_TEXTTAG_X_MINUS1_L                          = 2054,
    AT_SIVR_PRMT_BLUETOOTH_VISIBLE_L                           = 2055,
    AT_SIVR_PRMT_SYSTEM_START_L                                = 2056,
    AT_SIVR_PRMT_NEW_BLUETOOTH_PIN_SAVED_L                     = 2057,
    /*  AT_SIVR_PRMT_VOICE_MAIL_NUMBER_NOT_AVAILABLE_L            = 2058, */
    AT_SIVR_PRMT_T2_RESOLVE_AMBIGUOUS_NAMES_OPTIONS_L          = 2059,
    AT_SIVR_PRMT_T2_DIALNUMBERCORRECT_L                        = 2060,
    AT_SIVR_PRMT_PRESS_PTI_TO_STOP_L                           = 2061,
    AT_SIVR_PRMT_WRONG_PIN_LENGTH_L                            = 2062,
    AT_SIVR_PRMT_NUMBER_TOO_LONG_L                             = 2063,
    AT_SIVR_PRMT_SMS_L                                         = 2064,
    AT_SIVR_PRMT_T2_OPTIONS_RINGTONE_L                         = 2065,
    AT_SIVR_PRMT_PLEASE_CONTINUE_D_BLOCK_L                     = 2066,
    AT_SIVR_PRMT_MULTIPLE_NAMES_POSSIBLE_CHOOSE_HAPTICAL_L     = 2067,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_BLUETOOTH_PIN_L             = 2068,
    AT_SIVR_PRMT_ALL_AUTOMATIC_TRANSCRIPTIONS_RESTORED_L       = 2069,
    AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_RESET_CONFIRM_L        = 2070,
    AT_SIVR_PRMT_T2_MAINMENU_OPTIONS_L                         = 2071,
    AT_SIVR_PRMT_MEMO_DELETED_L                                = 2072,
/*    AT_SIVR_PRMT_AMBIGUOUS_NAME_L                              = 2073, */
    AT_SIVR_PRMT_LANGUAGE_FOR_L                                = 2074,
    AT_SIVR_PRMT_RINGTONE_SELECTED_L                           = 2075,
    AT_SIVR_PRMT_T2_OPTIONS_RESTOREFACTORYDEFAULTSCONFIRM_L    = 2076,
    AT_SIVR_PRMT_T2_OPTIONS_DELETE_ALL_VOICETAGS_CONFIRM_L     = 2077,
    AT_SIVR_PRMT_RESET_NOT_POSSIBLE_L                          = 2078,
/*    AT_SIVR_PRMT_PRESS_PTT_QUICK_SEARCH_L                      = 2079, */
    AT_SIVR_PRMT_T2_OPTIONS_MORE_OPTIONS_L                     = 2080,
    AT_SIVR_PRMT_MUSIC_STOP_L                                  = 2081,
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOSCONFIRMDELETE_L          = 2082,
    AT_SIVR_PRMT_T2_OPTIONS_CHANGE_LANGUAGE_L                  = 2083,
    AT_SIVR_PRMT_NOT_ENOUGH_SPACE_DELETE_MEMO_L                = 2084,
    AT_SIVR_PRMT_I_START_SMS_PLAYBACK_L                        = 2085,
    AT_SIVR_PRMT_SMS_XPLUSPLUS_L                               = 2086,
    AT_SIVR_PRMT_VOICE_FEEDBACK_ON_L                           = 2087,
    AT_SIVR_PRMT_EON_LIST_PRESS_BUTTON_IS_READ_REPEAT_L        = 2088,
/*    AT_SIVR_PRMT_I_VOICE_MAIL_NUMBER_PLEASE_L                  = 2089, */
    AT_SIVR_PRMT_LESS_THAN_ONE_MINUTE_OF_RECORDING_TIME_L      = 2090,
    AT_SIVR_PRMT_D_TEXTTAG_L                                   = 2091,
    AT_SIVR_PRMT_T2_OPTIONS_DIALNAME_CONFIRM_CAT_L             = 2092,
    AT_SIVR_PRMT_NO_PLAYER_AVAILABLE_L                         = 2093,
    AT_SIVR_PRMT_H_DIALNAME_L                                  = 2094,
    AT_SIVR_PRMT_T2_OPTIONS_DELETE_SMS_CONFIRM_L               = 2095,
    AT_SIVR_PRMT_SMS_DELETED_L                                 = 2096,
    AT_SIVR_PRMT_TRAINING_NOT_SUCCESSFUL_L                     = 2097,
    AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_OPTIONS_L              = 2098,
    AT_SIVR_PRMT_NO_FURTHER_NEW_SMS_L                          = 2099,
    AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE_L                 = 2100,
    AT_SIVR_PRMT_T2_OPTIONS_PLAYMEMOSOPTIONS_L                 = 2101,
    AT_SIVR_PRMT_I_CALLBACK_SENDER_L                           = 2102,
    AT_SIVR_PRMT_I_DIALNAME_CONFIRM_NAME_L                     = 2103,
    AT_SIVR_PRMT_NUMBER_DELETED_L                              = 2104,
    AT_SIVR_PRMT_NO_SMS_L                                      = 2105,
    AT_SIVR_PRMT_NO_FURTHER_VOICE_MEMOS_L                      = 2106,
    AT_SIVR_PRMT_NO_ENTRY_IN_CHOSEN_LIST_L                     = 2107,
    AT_SIVR_PRMT_NAME_TOO_LONG_L                               = 2108,
    AT_SIVR_PRMT_Y_MINUTES_OF_RECORDING_TIME_LEFT_L            = 2109,
    AT_SIVR_PRMT_T2_OPTIONS_BLUETOOTH_OPTIONS_L                = 2110,
    AT_SIVR_PRMT_T2_NAME_NOT_FOUND_NAME_PLEASE_L               = 2111,
    AT_SIVR_PRMT_REPEAT_SMS_L                                  = 2112,
    AT_SIVR_PRMT_SAY_TRAINING_PROMPT_L                         = 2113,
    AT_SIVR_PRMT_SYSTEM_STOP_L                                 = 2114,
    AT_SIVR_PRMT_CONTACT_SUCCESSFULLY_ENTERED_L                = 2115,
    AT_SIVR_PRMT_ORIGINAL_SETTINGS_RESTORED_L                  = 2116,
    AT_SIVR_PRMT_I_REDIAL_L                                   = 2117,
    AT_SIVR_PRMT_PRESS_PTT_L                                   = 2118,
    AT_SIVR_PRMT_FUNCTION_NOT_AVAILABLE_L                      = 2119,
/*    AT_SIVR_PRMT_RINGTONE_X_L                                  = 2120,*/
    AT_SIVR_PRMT_DO_YOU_WANT_TO_DIAL_NAME_CATEGORY_L           = 2121,
    AT_SIVR_PRMT_NO_DEVICE_L                                   = 2122,
    AT_SIVR_PRMT_T2_OPTIONS_READSMS_L                          = 2123,
    AT_SIVR_PRMT_T2_OPTIONS_SETTINGS_L                         = 2124,
    AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME_L               = 2125,
    AT_SIVR_PRMT_VOICE_FEEDBACK_OFF_L                          = 2126,
    AT_SIVR_PRMT_NO_NEXT_MESSAGE_L                             = 2127,
    AT_SIVR_PRMT_NEW_VOICE_MAIL_NUMBER_SAVED_L                 = 2128,
    AT_SIVR_PRMT_CHOOSE_OTHER_CATEGORY_INSTEAD_L               = 2129,
    AT_SIVR_PRMT_NO_VOICE_MEMO_AVAILABLE_L                     = 2130,
    AT_SIVR_PRMT_D_TEXTTAG_X_L                                 = 2131,
/*    AT_SIVR_PRMT_X_NUMBER_L                                    = 2132,*/
    AT_SIVR_PRMT_T2_OPTIONS_VOICEMEMOSPLAYMEMOS_L              = 2133,
    AT_SIVR_PRMT_NO_PREVIOUS_MESSAGE_L                         = 2134,
    AT_SIVR_PRMT_T2_OPTIONS_STORENAME_L                        = 2135,
/*    AT_SIVR_PRMT_I_DIAL_NAME_NAME_PLEASE_L                     = 2136, */
    AT_SIVR_PRMT_T1_SORRY_L                                    = 2137,
    AT_SIVR_PRMT_END_OF_RINGTONE_LIST_REPEAT_L                 = 2138,
/*    AT_SIVR_PRMT_ENTER_IT_NOW_L                                = 2139, */
    AT_SIVR_PRMT_AUTOMATIC_TRANSCRIPTION_RESTORED_L            = 2140,
    AT_SIVR_PRMT_NO_REMOTE_CONTROL_POSSIBLE_L                  = 2141,
    AT_SIVR_PRMT_CONNECT_L                                     = 2142,
    AT_SIVR_PRMT_PROCESS_CANCELLED_L                           = 2143,
    AT_SIVR_PRMT_CHANGIN_LANGUAGE_PLEASE_WAIT_L                = 2144,
    AT_SIVR_PRMT_T2_OPTIONS_READ_CALL_STACKS_L                 = 2145,
    AT_SIVR_PRMT_MISSED_CALL_FROM_L                            = 2146,
    AT_SIVR_PRMT_MISSED_CALL_FROM_UNKNOWN_L                    = 2147,
    AT_SIVR_PRMT_MISSED_CALL_SEVERAL_L                         = 2148,
    AT_SIVR_PRMT_T2_OPTIONS_CALLBACKMISSEDCALLS_L              = 2149,
    AT_SIVR_PRMT_START_DELETE_ALL_VT_L                         = 2150,
    AT_SIVR_PRMT_RTMODE_RINGRING_L                             = 2151,
    AT_SIVR_PRMT_RTMODE_EASYLISTEN_L                           = 2152,
    AT_SIVR_PRMT_RTMODE_ENDLESS_L                              = 2153,
    AT_SIVR_PRMT_RTMODE_FUTUREBEAT_L                           = 2154,
    AT_SIVR_PRMT_RTMODE_NOSTALGIC_L                            = 2155,
    AT_SIVR_PRMT_RTMODE_ORCHESTRA1_L                           = 2156,
    AT_SIVR_PRMT_RTMODE_ORCHESTRA2_L                           = 2157,
    AT_SIVR_PRMT_RTMODE_SWEETSOFT_L                            = 2158,
    AT_SIVR_PRMT_RTMODE_BIMBAM_L                               = 2159,
    AT_SIVR_PRMT_RTMODE_TUTTUT_L                               = 2160,
    AT_SIVR_PRMT_RTMODE_VIBORBELLZ_L                           = 2161,
    AT_SIVR_PRMT_RTMODE_VW_12_L                                = 2162,
    AT_SIVR_PRMT_RTMODE_VW_13_L                                = 2163,
    AT_SIVR_PRMT_RTMODE_VW_14_L                                = 2164,
    AT_SIVR_PRMT_RTMODE_VW_15_L                                = 2165,

    AT_SIVR_PRMT_MAINMENU_WITHOUT_MUSIC_L                      = 2168,
    AT_SIVR_PRMT_T1_MAINMENU_WITHOUT_MUSIC_L                   = 2169,

    AT_SIVR_PRMT_MORE_OPTIONS_WITHOUT_READ_SMS_L               = 2171,
    AT_SIVR_PRMT_MORE_T2_OPTIONS_WITHOUT_READ_SMS_L            = 2172,

    AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED_L                  = 2193,
    AT_SIVR_PRMT_ONLY_MP_CONNECTED_BUT_NO_REMOTE_CONTROL_L     = 2194,
    AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS_L  = 2195,
    AT_SIVR_PRMT_NO_DEVICE_CONNECTED_WITH_IC_CONTEXT_L         = 2196,
    AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS_BIG_L = 2198,

    AT_SIVR_PRMT_AMBIGUOUS_NAME_L                              = 3073,
    AT_SIVR_PRMT_X_NUMBER_L                                    = 3132,

    AT_SIVR_PRMT_CK5050_MAXVALUE                             = AT_SIVR_PRMT_X_NUMBER_L,

    /* internal UHZV prompts - custom with hard coded text */
    AT_SIVR_PRMT_STRING                                      = 10000,
    AT_SIVR_PRMT_ERROR                                       = 10001,
    AT_SIVR_PRMT_RINGTONE_X                                  = 10002,
    AT_SIVR_PRMT_CHOSEN_LIST                                 = 10003,
    AT_SIVR_PRMT_L_READ_LIST                                 = 10004,
    AT_SIVR_PRMT_TEST_DIAGNOSE                               = 10006,
    AT_SIVR_PRMT_CHOSEN_LIST_VOICETAG                        = 10007,
    AT_SIVR_PRMT_HELP_DIAL_NAME_CONFIRM                      = 10008
} ATSIVRPromptEnum;


typedef enum {
    AT_SIVR_ITEMTYPE_NONE      =    0, /* use as default */
    AT_SIVR_ITEMTYPE_CONTACT   =    2,
    AT_SIVR_ITEMTYPE_ARTIST    =    4,
    AT_SIVR_ITEMTYPE_ALBUM     =    8,
    AT_SIVR_ITEMTYPE_GENRE     =   16,
    AT_SIVR_ITEMTYPE_SONGTITLE =   32,
    AT_SIVR_ITEMTYPE_PHONE     =   64
}ATSIVRItemTypeEnum;


typedef enum {
    AT_VTMGR_ACT_RETRIEVE   = 0,
    AT_VTMGR_ACT_STORE      = 1,
    AT_VTMGR_ACT_DELETE     = 2,
    AT_VTMGR_ACT_DELETE_ALL = 3
}ATVTMgrActionType;

typedef enum {
    AT_SIVR_RES_NONE   = 0,
    AT_SIVR_RES_NUMBER = 1,
    AT_SIVR_RES_IDs    = 2
} ATSIVRResultOptEnum;

typedef enum {
    AT_PBSV_ACT_DISABLE_VISIBILTY = 0,
    AT_PBSV_ACT_ENABLE_VISIBILTY  = 1,
    AT_PBSV_ACT_QUERY_VISIBILTY   = 2
//    AT_PBSV_ACT_DISABLE_VISIBILTY                             = 0,
//    AT_PBSV_ACT_ENABLE_VISIBILTY_AND_CONNECTABILITY           = 1,
//    AT_PBSV_ACT_QUERY_VISIBILTY                               = 2,
//    AT_PBSV_ACT_DISABLE_VISIBILTY_AND_CONNECTABILITY          = 3,
//    AT_PBSV_ACT_QUERY_CONNECTABILITY                          = 4
}ATPbsvActionType;

typedef enum
{
    AT_PPRT_ACT_STOP        = 0,
    AT_PPRT_ACT_PLAY        = 1,
    AT_PPRT_ACT_PLAY_ONCE   = 2
}ATPprtActionType;

typedef enum
{
    AT_PECA_DEACTIVATE     = 0,
    AT_PECA_ACTIVATE       = 1,
    AT_PECA_GET_LAST_ERROR = 2
}ATPecaActionType;

typedef enum
{
    AT_PIRT_ACT_DISABLE = 0,
    AT_PIRT_ACT_ENABLE  = 1
}ATPirtActionType;

typedef enum
{
    AT_POPM_OOP_ACTIVE_DEVICE_ONLY = 0,
    AT_POPM_OOP_ANY_PAIRED_DEVICE = 1,
    AT_POPM_OOP_OTHER_PAIRED_DEVICE = 2,
    AT_POPM_OOP_REJECT_ALL_TRANSFER = 3
}ATPopmModeType;

typedef enum
{
    AT_DTMF_AFTER_DIALING_DISABLE = 0,
    AT_DTMF_AFTER_DIALING_ENABLE  = 1
}ATPcddStateType;

typedef enum
{
    AT_ALCK_LOCK_STOP = 0,
    AT_ALCK_LOCK_START  = 1
}ATAlckLockType;

typedef enum
{
    AT_PEVN_SPEAKER_VOLUME_NOTIFICATION_DISABLE = 0,
    AT_PEVN_SPEAKER_VOLUME_NOTIFICATION_ENABLE = 1
}ATPevnModeType;

typedef enum
{
    AT_PEVN_APPLY_NEW_VOLUME_ON_SPEAKER = 0,
    AT_PEVN_DONT_APPLY_NEW_VOLUME_ON_SPEAKER = 1
}ATPevnApplySpkVolType;


typedef enum
{
    AT_TESTMODE_IS_SWITCHED_OFF = 0,
    AT_TX_BURST_MODE_IS_SWITCHED_ON = 1,
    AT_RX_BURST_MODE_IS_SWITCHED_ON = 2,
    AT_RX_BURST_MODE_WITH_DATA = 3,
    AT_RX_BURST_MODE_WITH_BIT_ERROR_RATE = 4
}ATTestScenarioType;


typedef enum
{
    AT_ALL_ZERO_PATTERN = 1,
    AT_ALL_ONE_PATTERN = 2,
    AT_ONE_BIT_ALTERNATING_PATTERN= 3,
    AT_PRBS9_SEQUENCE = 4,
    AT_FOUR_BIT_ALTERNATING_PATTERN = 9,
    AT_TWO_BIT_ALTERNATING_PATTERN = 10
}ATTestBitPatternType;

typedef enum
{
    AT_SCRAMBLER_SWITCHED_OFF = 0,
    AT_SCRAMBLER_SWITCHED_ON = 1
}ATTestScramblerModeType;

typedef enum
{
    AT_POWER_LEVEL_LOWEST  = 0,
    AT_POWER_LEVEL_LOW     = 1,
    AT_POWER_LEVEL_MIDDLE  = 2,
    AT_POWER_LEVEL_HIGH    = 3,
    AT_POWER_LEVEL_HIGHEST = 4
}ATTestPowerLevelType;

typedef enum
{
    AT_BT_FEAT_UNSUPPORTED = 0,
    AT_BT_FEAT_SUPPORTED,
    AT_BT_FEAT_UNKNOWN
}ATBTFeatureSupportResultEnum;

typedef enum {
    AT_HK_NONE
} ATHkProfileStatusEnum;

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef Uint8Type AT_Md5KeyType[AT_MD5_KEY_LEN+1];
typedef Uint8Type AT_Md5KeyEncrytedType[AT_MD5_KEY_ENCRYTED_LEN+1];
typedef Uint8Type AT_BT_Address[AT_BT_ADDRESS_LEN];
//typedef Uint8Type AT_BTClassOfDevice[AT_BT_COD_LEN];
typedef Uint32Type AT_BTClassOfDevice;
typedef Uint8Type AT_CallCount; /* valid range 0..2 */
typedef Uint8Type AT_CallIndex; /* valid range 1..2 */
typedef Uint8Type AT_DeviceId;
typedef Uint8Type AT_IndicatorValue; /* range depends on indicator Index      */

/* NOTE: for all character array +1 is added to enable add on of '\0'*/
typedef Uint8Type AT_CallerId[AT_CALLER_ID_STRING_MAX_LEN+1]; /* UTF8 coded!!!  */
typedef Uint8Type AT_CallerIdExt[AT_CALLER_ID_STRING_MAX_LEN+1]; /* UTF8 coded!!!  */

typedef Uint8Type AT_BTDeviceName[AT_BT_DEVICE_NAME_MAX_LEN+1];
typedef Uint8Type AT_BTSWVersion[AT_BT_PF_CODE_MAX_LEN+1];


typedef Uint8Type AT_IndicatorString[AT_OPERATOR_NAME_STRING_MAX_LEN+1];
typedef Uint8Type AT_IndicatorString2[AT_OPERATOR_NAME_PIND_MAX_LEN+1];
typedef Uint8Type AT_DeviceNameString[AT_DEVICE_NAME_MAX_LEN+1];

typedef Uint8Type ATPhoneNumberType[AT_MAX_PHONE_NUM_LEN+1];
typedef Uint8Type ATPinCodeBT[AT_MAX_PIN_CODE_LEN+1];
typedef Uint8Type ATSSPPinCodeBT[AT_MAX_SSP_PIN_CODE_LEN+1];
typedef Uint8Type ATIMSI[AT_MAX_IMSI_LEN+1];

/* type definitions for SMS */
typedef Uint8Type ATSmsString[AT_MAX_SMS_LEN_TOTAL+1];
typedef Uint8Type ATSmsOrigin[AT_SMS_ORIGIN_MAX_LEN+1];
typedef Uint8Type ATSmsDate[AT_SMS_DATE_MAX_LEN+1];
typedef Uint8Type ATSmsPhoneNumber[AT_SMS_PHONENUM_MAX_LEN+1];
typedef Uint8Type ATExtModelRev[AT_CMGREX_MAX_LEN+1];
typedef Uint8Type ATSmsConc[AT_SMS_CONC_RESP_LEN+1];

/* type definitions for MDI */
typedef Uint8Type ATMdiFileInfo[AT_MDI_ENTRY_DATA_LEN+1];
typedef Uint8Type ATMdiFileName[AT_MDI_ENTRY_DATA_LEN+1];
typedef Uint8Type ATMdiEntryData[AT_MDI_ENTRY_DATA_LEN+1];


/* type definitions for Voice recognition */
typedef Uint8Type ATSIVRPromptText[AT_SIVR_MAX_PROMPT_TEXT_LEN+1];
typedef Uint8Type ATSIVRPrmtOptText[AT_SIVR_MAX_PRMT_OPT_LEN+1];
typedef Uint8Type ATSIVRNumberText[AT_SIVR_MAX_PRMT_NUM_LEN+1];
typedef Uint8Type ATSIVRTrainingText[AT_SIVR_MAX_PRMT_TRAIN_LEN+1];

/* type definitions for RSTUEX */

typedef Uint8Type ATExtCustomer[AT_CUSTOMER_MAX_LEN+1];
typedef Uint8Type ATExtProject[AT_PROJECT_MAX_LEN+1];
typedef Uint8Type ATExtVersionNum[AT_VERSION_NUM_MAX_LEN+1];
typedef Uint8Type ATExtLanguage[AT_LANGUAGE_MAX_LEN+1];
typedef Uint8Type ATExtLanguageVersion[AT_LANGUAGE_VERSION_MAX_LEN+1];

/* type definitions for PBCV */

typedef Uint8Type ATPbcvHwVersion[AT_PBCV_HW_VERSION_LEN+1];
typedef Uint8Type ATPbcvSwVersion[AT_PBCV_SW_VERSION_LEN+1];
typedef Uint8Type ATPbcvManufacturer[AT_PBCV_MANUFACTURER_LEN+1];

/*----------------------------------------------------------------------------*/
/* Public Structures                                                          */
/*----------------------------------------------------------------------------*/
typedef struct
{
    ATPinCodeBT     pin;
    Uint8Type       pin_len;
}ATPinCodeType;

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _ATCOMMON_DATA_H */

/* End Of File ATCommonData.h */
