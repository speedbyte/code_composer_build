/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandlerData.h
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Data
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler manage and built the different
                  message for the Parrot module - this file include data defs
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Cmd_Handler/trunk/inc/ATCmdHandlerData.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATCMDHANDLER_DATA_H
#define _ATCMDHANDLER_DATA_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ATCommonData.h"


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ATCmdHandler_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Enumerations                                                        */
/*----------------------------------------------------------------------------*/

/* NOTE: a request is defined as command FROM HOST to CK5050                  */
typedef enum
{
    AT_REQ_UNDEFINED = 0,
    /* UHV enum                  CK5050 code                     direction    */

    /* general commands   - see chapter 5 of SW host IF                       */
    AT_REQ_BT_ANTENNA,           /*   *PBTA                   CK5050 <-> HOST */
    AT_REQ_SET_BAUD,             /*   *PSBR                   CK5050 <-> HOST */
    AT_REQ_MANUFAC_ID,           /*   +CGMI                   CK5050 <-> HOST */
    AT_REQ_MODEL_ID,             /*   +CGMM                   CK5050 <-> HOST */
    AT_REQ_MODEL_REV,            /*   +CGMR                   CK5050 <-> HOST */
    AT_REQ_MODEL_PF_CODE,        /*   +CGMR                   CK5050 <-> HOST */
    AT_REQ_MODEL_REV_EXT,        /*   +CGMREX                 CK5050 <-> HOST */
    AT_REQ_POWER_OFF,            /*   *POFF                   CK5050 <-> HOST */
    AT_REQ_SET_BT_NAME,          /*   *PSFN                   CK5050 <-> HOST */
    AT_REQ_SET_BT_ADDR,          /*   *PBDADDR                CK5050 <-> HOST */
    AT_REQ_PLAY_RINGTONE,        /*   *PPRT                   CK5050 <-> HOST */
    AT_REQ_INBOUND_RINGTONE,     /*   *PIRT                   CK5050 <-> HOST */
    AT_REQ_DTMF_PLAYING,         /*   *PCDD                   CK5050 <-> HOST */
    AT_REQ_SET_EXT_ERROR_CODES, /*    *PECA                   CK5050 <-> HOST */

    /* BT pairing commands - see chapter 6 of SW host IF                      */
    AT_REQ_INQUIRY,              /*   *PBDI                   CK5050 <-> HOST */
    AT_REQ_GET_INQUIRY_RESULT,   /*   *PGIR                   CK5050 <-> HOST */
    AT_REQ_INQUIRY_EXT_RESULT,   /*   *PGIREX                 CK5050 <-> HOST */
    AT_REQ_HDL_PAIR_REQUEST,     /*   *PPAU                   CK5050 <-> HOST */
    AT_REQ_IN_PAIR_REQUEST,      /*   *PPRQ                   CK5050 <-> HOST */
    AT_REQ_GET_PAIRED_DEVICES,   /*   *PLPD                   CK5050 <-> HOST */
    AT_REQ_PAIR_DELETE,          /*   *PPDE                   CK5050 <-> HOST */
    AT_REQ_SET_DISCOVER_MODE,    /*   *PSDM                   CK5050 <-> HOST */
    AT_REQ_SET_MAX_PAIRED_DEVICES, /* *PSMD                   CK5050 <-> HOST */
    AT_REQ_SPECIAL_DEV_LIST,     /*   *PSDR                   CK5050 <-> HOST */
    AT_REQ_SPECIAL_DEV_SELECT,   /*   *PLSD                   CK5050 <-> HOST */
    AT_REQ_BT_CHIP_VERSION,      /*   *PBCV                   CK5050 <-> HOST */
    AT_REQ_IMSI,                 /*   *PSSN                   CK5050 <-> HOST */
    AT_REQ_SSP_SETUP,            /*   *PSSP                   CK5050 <-> HOST */
    AT_REQ_SSP_REQUEST,          /*   *PSPR                   CK5050 <-> HOST */
    AT_REQ_BT_SUP_FEAT,          /*   *PBSF                   CK5050 <-> HOST */

    /* BT connection commands - see chapter 7 of SW host IF                   */
    AT_REQ_BT_SELECT_DEV,        /*   *PSBD                   CK5050 <-> HOST */
    AT_REQ_BT_SERVICE_CONN,      /*   *PBSC                   CK5050 <-> HOST */
    AT_REQ_BT_READ_RSSI,         /*   *PBRSSI                 CK5050 <-> HOST */
    AT_REQ_BT_LED_ENDIS,         /*   *PLED                   CK5050 <-  HOST */
    AT_REQ_BT_AG_MODE,           /*   *PAGM                   CK5050 <-> HOST */
    AT_REQ_BT_AG_TRANSFER,       /*   *PAGT                   CK5050 <-> HOST */

    /* BT connection settings - see chapter 8 of SW host IF                   */
    AT_REQ_BT_SET_AC_MODE,       /*   *PSCM                   CK5050 <-> HOST */
    AT_REQ_BT_SERVICE_AC,        /*   *PBSA                   CK5050 <-> HOST */
    AT_REQ_BT_AC_MODE_STATUS,    /*   *PCMS                   CK5050 <-> HOST */
    AT_REQ_BT_SERVICE_MODE,      /*   *PBSM                   CK5050 <-> HOST */
    AT_REQ_BT_SET_CONN_AUTH,     /*   *PSCA                   CK5050 <-> HOST */
    AT_REQ_BT_SERVICE_VISIBILITY,/*   *PBSV                   CK5050 <-> HOST */

    /* Phonebook mgmt commands - see chapter 9 of SW host IF                  */
    AT_REQ_PB_STACK_CHANGE,      /*   *PPBCEX                 CK5050 <-> HOST */
    AT_REQ_PB_UPDATE_STATUS,     /*   *PPUS                   CK5050 <-> HOST */
    AT_REQ_PB_SELECT,            /*   *PPBS                   CK5050 <-> HOST */
    AT_REQ_PB_READ_ENTRIES,      /*   *PPBR                   CK5050 <-> HOST */
    AT_REQ_PB_READ_NUMBERS,      /*   *PPBP                   CK5050 <-> HOST */
    AT_REQ_PB_CALL_HISTORY,      /*   *PPCH                   CK5050 <-> HOST */
    AT_REQ_PB_TOTAL_COUNT,       /*   *PPNC                   CK5050 <-> HOST */
    AT_REQ_PB_ALPHA_SEARCH,      /*   *PPAS                   CK5050 <-> HOST */
    AT_REQ_PB_ALPHA_SEARCH_EXT,  /*   *PPASEX                 CK5050 <-> HOST */
    AT_REQ_PB_SEARCH_OPTION,     /*   *PPSO                   CK5050 <-  HOST */
    AT_REQ_PB_SEARCH,            /*   *PPSH                   CK5050 <-> HOST */
    AT_REQ_PB_OBJECT_PUSH_MODE,  /*   *POPM                   CK5050 <-  HOST */
    AT_REQ_PB_OBJECT_PUSH_ACC,   /*   *POPA                   CK5050 <-  HOST */
    AT_REQ_PB_EN_DWNLOAD_STATUS, /*   *PEDS                   CK5050 <-> HOST */
    AT_REQ_PB_DISABLE_AUTO_SYNC, /*   *PDAS                   CK5050 <-> HOST */
    AT_REQ_PB_START_MANUAL_SYNC, /*   *PPMS                   CK5050  -> HOST */
    AT_REQ_PB_DELETE_ENTRY_ALL,  /*   *PPBD                   CK5050 <-> HOST */
    AT_REQ_PB_ADD_ENTRY,         /*   *PPAD                   CK5050 <-> HOST */
    AT_REQ_PB_NAME_ORDER_SELECT, /*   *PPNO                   CK5050 <-> HOST */
    AT_REQ_PB_GET_ALPHA_COUNT,   /*   *PPAC                   CK5050 <-> HOST */

    /* Call management commands - see chapter 10 of SW host IF                 */
    AT_REQ_CALL_PHONE_NUBER,     /*   ATD                     CK5050 <-  HOST */
    AT_REQ_CALL_PB_INDEX,        /*   ATDI                    CK5050 <-  HOST */
    AT_REQ_CALL_PB_NUMBER_TYPE,  /*   ATDP                    CK5050 <-  HOST */
    AT_REQ_CALL_LAST_NUMBER,     /*   +BLDN                   CK5050 <-  HOST */
    AT_REQ_CALL_PB_MEM_INDEX,    /*   *PDPM                   CK5050 <-  HOST */
    AT_REQ_PICK_UP_CALL,         /*   ATA                     CK5050 <-  HOST */
    AT_REQ_CALL_STATUS_IND_INDEX,/*   *PLCC                   CK5050 <-> HOST */
    AT_REQ_EXT_CALL_LIST,        /*   *PLCCEX                 CK5050 <-> HOST */
    AT_REQ_CURR_CALL_INFO,       /*   *PCCI                   CK5050 <-> HOST */
    AT_REQ_CALL_HANG_UP,         /*   +CHUP                   CK5050 <-  HOST */
    AT_REQ_SWITCH_CALLS,         /*   *PSWC                   CK5050 <-  HOST */
    AT_REQ_AUDIO_TRANS_DISCRETE, /*   *PATR                   CK5050 <-> HOST */
    AT_REQ_DTMF,                 /*   +VTS                    CK5050 <-  HOST */
    AT_REQ_PHONE_STATUS_IND,     /*   *PIND=?                 CK5050 <-> HOST */
    AT_REQ_PHONE_STATUS_IND_VAL, /*   *PIND?                  CK5050 <-> HOST */
    AT_REQ_CLASS_DEVICE_REQUEST, /*   *PBCD                   CK5050 <-> HOST */
    AT_REQ_GET_PHONE_FEATURES,   /*   *PPSF                   CK5050 <-> HOST */
    AT_REQ_MULTI_PARTY_CALL,     /*   *PMPC                   CK5050 <-> HOST */
    AT_REQ_SET_MAX_CALLS,        /*   *PSMC                   CK5050 <-> HOST */

    /* Audio Microphone commands - see chapter 11 of SW host IF                */
    AT_REQ_MIC_ALGO_SELECT,      /*   *PALGOSELECT            CK5050 <-> HOST */
    AT_REQ_MIC_CHOICE_SET,       /*   *PALGOSETMIC            CK5050 <-> HOST */
    AT_REQ_MIC_GAIN_SET,         /*   +VGM                    CK5050 <-> HOST */
    AT_REQ_MIC_DISTANCE_SET,     /*   *PALGOMICDIST           CK5050 <-  HOST */
    AT_REQ_MIC_TUNE_NR,          /*   *PALGONR                CK5050 <-  HOST */
    AT_REQ_MIC_EQUALIZE,         /*   *PALGOTXEQ              CK5050 <-  HOST */
    AT_REQ_MIC_ADC_CONF,         /*   *PALGOADC               CK5050 <-  HOST */

    /* Audio speaker settings - see chapter 12 of SW host IF                  */
    AT_REQ_SPK_RX_ALGO_SELECT,     /*   *PALGOSELECTRX        CK5050 <-  HOST */
    AT_REQ_SPK_DYN_COMPRESSION,    /*   *PALGOCOMPRESSOR      CK5050 <-  HOST */
    AT_REQ_SPK_EQUALIZE,           /*   *PALGORXEQ            CK5050 <-  HOST */
    AT_REQ_SPK_TUNE_NR,            /*   *PALGONRRX            CK5050 <-  HOST */
    AT_REQ_SPK_LIM_HIGHPASS,       /*   *PALGORX              CK5050 <-  HOST */
    AT_REQ_SPK_RX_ALC,             /*   *PALGOALCRX           CK5050 <-> HOST */
    AT_REQ_SPK_RX_SL,              /*   *PALGOSLRX            CK5050 <-> HOST */
    AT_REQ_SPK_TX_SL,              /*   *PALGOSLTX            CK5050 <-> HOST */

    /* Audio echo settings - see chapter 13 of SW host IF                     */
    AT_REQ_ECHO_TUNE_CANCELLATION, /*   *PALGOAEC             CK5050 <-  HOST */
    AT_REQ_ECHO_DELAY_CANCELLATION,/*   *PALGOAECDELAY        CK5050 <-> HOST */
    AT_REQ_ECHO_NOISE_COMFORT,     /*   *PALGONOISECOMFORT    CK5050 <-> HOST */
    AT_REQ_ECHO_CHECK_NOISE,       /*   *PALGONOISECHK        CK5050 <-> HOST */

    /* Voice Tag Mgmt commands - see chapter 14 of SW host IF                 */
    AT_REQ_VT_RECORD_INDEX,        /*   *PRVP                 CK5050 <-> HOST */
    AT_REQ_VT_RECORD_KEY,          /*   *PRVK                 CK5050 <-> HOST */
    AT_REQ_VT_TEST_PRESENCE_INDEX, /*   *PTVP                 CK5050 <-> HOST */
    AT_REQ_VT_TEST_PRESENCE_KEY,   /*   *PTVK                 CK5050 <-> HOST */
    AT_REQ_VT_PLAYBACK_INDEX,      /*   *PPVP                 CK5050 <-> HOST */
    AT_REQ_VT_PLAYBACK_KEY,        /*   *PPVK                 CK5050 <-> HOST */
    AT_REQ_VT_DELETE_INDEX,        /*   *PDVP                 CK5050 <-> HOST */
    AT_REQ_VT_DELETE_KEY,          /*   *PDVK                 CK5050 <-> HOST */
    AT_REQ_VT_GET_NUMBER_TAGS,     /*   *PNVP                 CK5050 <-> HOST */
    AT_REQ_VT_AUTO_TAG,            /*   *PAVP                 CK5050 <-> HOST */

    /* Voice Recognition commands - see chapter 15 of SW host IF              */
    AT_REQ_VR_TYPE,                /*   *PVRA                 CK5050 <-> HOST */
    AT_REQ_VR_ACTIVATE,            /*   +BVRA                 CK5050 <-> HOST */

    /* Audio Streaming commands - see chapter 16 of SW host IF                */
    AT_REQ_AS_KEY_EVENT_INDICATOR, /*   *PKEV                 CK5050 <-> HOST */
    AT_REQ_AS_GET_META_TYPE,       /*   *PGMT                 CK5050 <-> HOST */

    /* UART multiplexing system commands - see chapter 17 of SW host IF       */
    AT_REQ_UART_MUX_INIT,          /*   +CMUX                 CK5050 <-  HOST */
    AT_REQ_UART_GET_CHAN_INFO_MUX, /*   *PCIM                 CK5050 <-> HOST */

    /* Test commands - see chapter 18 of SW host IF                           */
    AT_REQ_TEST_AUDIO_LOOPBACK,    /*   *PLOOPBACK            CK5050 <-  HOST */
    AT_REQ_TEST_AUDIO_LOOP_TEST,   /*   *PTEST                CK5050 <-  HOST */
    AT_REQ_TEST_BT_TEST_MODE,      /*   *PBTESTMODE           CK5050 <-  HOST */
    AT_REQ_TEST_OUT_FREQ_CHAN,     /*   *PBFREQ               CK5050 <-  HOST */
    AT_REQ_TEST_OUT_FREQ_CHAN_EXT, /*   *PBFREQEX             CK5050 <-  HOST */
    AT_REQ_TEST_SW_RESET,          /*   *PRST                 CK5050 <-  HOST */
    AT_REQ_ALTERNATING_SIGNAL,     /*   *AGSW                 CK5050 <-  HOST */

    /* SMS commands - see chapter 19 of SW host IF                            */
    AT_REQ_SMS_GET_SUPPORT,        /*   *PMFT                 CK5050 <-> HOST */
    AT_REQ_SMS_LIST,               /*   *PMLS                 CK5050 <-> HOST */
    AT_REQ_SMS_CHARSET,            /*   *PMCS                 CK5050 <-  HOST */
    AT_REQ_SMS_MAX_FOLDER_SIZE,    /*   *PMFS                 CK5050 <-> HOST */
    AT_REQ_SMS_MAX_FOLDER_SIZE_GET,/*   *PMFS?                CK5050 <-  HOST */
    AT_REQ_SMS_READING,            /*   *PMRD                 CK5050 <-> HOST */
    AT_REQ_SMS_READING_GET,        /*   *PMRD?                CK5050 <   HOST */
    AT_REQ_SMS_SEND,               /*   *PMSD                 CK5050 <-  HOST */
    AT_REQ_SMS_CONC_SEND,          /*   *PMSDEX               CK5050 <-  HOST */
    AT_REQ_SMS_DELETE,             /*   *PMDE                 CK5050 <-  HOST */
    AT_REQ_SMS_ENABLE,             /*   *PMEN                 CK5050 <-> HOST */

    /* Flash management commands - see chapter 21 of SW host IF               */
    AT_REQ_FLASH_SW_UPDATE_BT,     /*   *PBSU                 CK5050 <-> HOST */
    AT_REQ_FLASH_SW_UPDATE_DUN,    /*   *PDSU                 CK5050 <-> HOST */
    AT_REQ_FLASH_ERASE_USERDATA,   /*   *PFLASHERASE          CK5050 <-> HOST */

    /* Speaker independant VR commands - see chapter 22 of SW host IF         */
    AT_REQ_SIVR_ENGINE_MGMT,       /*   *RREC                 CK5050 <-  HOST */
    AT_REQ_SIVR_PREPARE_VR,        /*   *RPRE                 CK5050 <-  HOST */
    AT_REQ_SIVR_MANAGE_PROMTS,     /*   *RPMT                 CK5050 <-  HOST */
    AT_REQ_SIVR_MANAGE_VOICE_TAG,  /*   *RVTA                 CK5050 <-> HOST */
    AT_REQ_SIVR_GET_SUPPORTED_LANG,/*   *RGSL                 CK5050 <-> HOST */
    AT_REQ_SIVR_SET_CURRENT_LANG,  /*   *RSCL                 CK5050 <-> HOST */
    AT_REQ_SIVR_SET_SYSTEM_PROMPT, /*   *RPDR                 CK5050 <-> HOST */
    AT_REQ_SIVR_SET_RESULT_FORMAT, /*   *RSRF                 CK5050 <-> HOST */
    /* below messages are NOT longer in HSIS for SW version 0170RC2 but kept here
     * as not clear if those are really removed */
    AT_REQ_SIVR_GET_SUPPORTED_FEAT,/*   *RGSF                 CK5050 <-> HOST */
    AT_REQ_SIVR_SET_T2S_PB_MODE,   /*   *RSTM                 CK5050 <-> HOST */
    AT_REQ_SIVR_LOAD_CONTEXT,      /*   *RLDC                 CK5050 <-> HOST */
    AT_REQ_SIVR_START_VR_ENGINE,   /*   *RSVR                 CK5050 <-  HOST */
    AT_REQ_SIVR_PLAY_TEXT,         /*   *RPTX                 CK5050 <-  HOST */

    /* Disco database mgmt commands - see chapter 23 of SW host IF            */
    AT_REQ_DISCO_OPEN_DB,          /*   *DOMD                 CK5050 <-> HOST */
    AT_REQ_DISCO_SET_GLOBAL,       /*   *DSGS                 CK5050 <-  HOST */
    AT_REQ_DISCO_SET_EMPTY_STRING, /*   *DSES                 CK5050 <-  HOST */
    AT_REQ_DISCO_SET_MAX_STRING_LEN,/*  *DMLS                 CK5050 <-> HOST */
    AT_REQ_DISCO_DELETE_DB,        /*   *DDDB                 CK5050 <-> HOST */

    /* Disco browsing commands - see chapter 24 of SW host IF                 */
    AT_REQ_DISCO_SET_BROWSE_MODE,  /*   *DSBM                 CK5050 <-  HOST */
    AT_REQ_DISCO_GET_BROWSE_MODE,  /*   *DGBM                 CK5050 <-> HOST */
    AT_REQ_DISCO_SET_CURRENT_DIR,  /*   *DSCD                 CK5050 <-  HOST */
    AT_REQ_DISCO_GET_CURRENT_DIR,  /*   *DGCD                 CK5050 <-> HOST */
    AT_REQ_DISCO_GET_ENTRY_COUNT,  /*   *DGEC                 CK5050 <-> HOST */
    AT_REQ_DISCO_LIST_ENTRIES,     /*   *DLSE                 CK5050 <-> HOST */
    AT_REQ_DISCO_GET_ENTRY_DATA,   /*   *DGED                 CK5050 <-> HOST */
    AT_REQ_DISCO_ALPHA_SEARCH,     /*   *DASH                 CK5050 <-> HOST */

    /* Playing commands - see chapter 25 of SW host IF                        */
    AT_REQ_PLAYER_PLAY_MODE,       /*   *DPLY                 CK5050 <-  HOST */
    AT_REQ_PLAYER_GET_STATUS,      /*   *DGPS                 CK5050 <-> HOST */
    AT_REQ_PLAYER_SET_RANDOM_MODE, /*   *DSRA                 CK5050 <-  HOST */
    AT_REQ_PLAYER_SET_REPEAT_MODE, /*   *DSRE                 CK5050 <-  HOST */
    AT_REQ_PLAYER_FAST_PLAY_SET,   /*   *DFPS                 CK5050 <-> HOST */
    AT_REQ_PLAYER_NOTIFICATION ,   /*   *DCPN                 CK5050 <-> HOST */

    /* Device management commands - see chapter 26 of SW host IF              */
    AT_REQ_DMGMT_LIST_MMD,         /*   *DLMD                 CK5050 <-> HOST */

    /* Audio management commands - see chapter 27 of SW host IF               */
    AT_REQ_AMGMT_SRD_ACTIVE_IND,   /*   *PASA                 CK5050 <-> HOST */
    AT_REQ_AMGMT_OUT_VOLUME_SET,   /*   *PNVS                 CK5050 <-> HOST */
    AT_REQ_AMGMT_SET_RINGTONE,     /*   *PSRT                 CK5050 <-> HOST */
    AT_REQ_AMGMT_MIC_MUTE,         /*   *PMMU                 CK5050 <-> HOST */
    AT_REQ_AMGMT_I2S_AUDIO,        /*   *PAFH                 CK5050 <-> HOST */
    AT_REQ_AMGMT_UART_AUDIO_IN,    /*   *PSFH                 CK5050 <-> HOST */
    AT_REQ_AMGMT_AUDIO_LINE_IN,    /*   *ALIN                 CK5050 <-> HOST */
    AT_REQ_AMGMT_ENABLE_CODEC,     /*   *PEIC                 CK5050 <-  HOST */
    AT_REQ_AMGMT_AUDIO_LOCK,       /*   *ALCK                 CK5050 <-  HOST */
    AT_REQ_AMGMT_SPEAKER_VOLUME,   /*   *PEVN                 CK5050 <-  HOST */

    AT_REQ_NUMBER_COMMAND_IDS      /*   used as indicator for max number
                                    of available command IDs if necessary     */
} ATRequestEnum;


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Structures                                                          */
/*----------------------------------------------------------------------------*/

/* common data structure to call the command handler                          */

/* data structure for all requests without data                               */
typedef struct {
    ATRequestEnum req_type;
} ATReqCommonType;

/* data structure for call hangup */
typedef struct {
    ATReqCommonType   common;/* the common is required for all structures!!!  */
    ATCallHangupEnum  mode;
} ATReqHangupType;

/* data structure to pass a telephone number */
typedef struct {
    ATReqCommonType   common;/* the common is required for all structures!!!  */
    ATPhoneNumberType number;
    Uint8Type         len;   /* len indicate the valid number of characters   */
} ATReqCallNumberType;

/* data structure to pass a telephone book index to be dialed */
typedef struct {
    ATReqCommonType   common;/* the common is required for all structures!!!  */
    Uint16Type        pbIndex;
    Uint16Type        pnIndex;
    ATBooleanEnum     pnIndex_avail;
} ATReqCallIndexType;

/* data structure to pass a telephone book index and number type to be dialed */
typedef struct {
    ATReqCommonType         common;/* the common is required for all structures!!!  */
    Uint16Type              pbIndex;
    ATPhoneNumberTypeEnum   numberType;
} ATReqCallTypeOfNumberType;

/* data structure to pass a telephone number */
typedef struct {
    ATReqCommonType   common;/* the common is required for all structures!!!  */
    ATPairingModeEnum mode;
    /* if mode is AT_PAIR_ACCEPT_INCOMING pin code can be given */
    Uint8Type         pin_digits; /* indicate valid pin code digits  */
    ATPinCodeBT       pin; /* pincode itself */
} ATReqSetPairModeType;

/* data structure to pass microphone settings */
typedef struct {
    ATReqCommonType   common;/* the common is required for all structures!!!  */
    ATMicModeEnum     mode;
} ATReqMicMuteType;

/* data structure for extended call list setting  */
typedef struct {
    ATReqCommonType         common;/* required for all structures!!!            */
    ATAudioTransferModeEnum   mode;
} ATReqAudioTransferType;

typedef struct
{
    ATReqCommonType             common;/* required for all structures!!!            */
    ATAudioGatewayModeEnum      mode;
}ATReqAudioGatewayType;

typedef struct
{
    ATReqCommonType                     common;/* required for all structures!!!            */
    ATAudioGatewayTransferModeEnum      mode;
}ATReqAudioGatewayTransferType;



/* data structure to select phone book  */
typedef struct {
    ATReqCommonType       common;/* the common is required for all structures!!!  */
    ATPhoneBookTypeEnum   type;
    ATPhoneBookModeEnum   mode; /* optional if type == AT_PBTYPE_USER_PERSONAL    */
} ATReqSelectPbType;


/* data structure for extended call list setting  */
typedef struct {
    ATReqCommonType         common;/* required for all structures!!!            */
    ATExtendedCallModeEnum  mode;
} ATExtCallModeType;

typedef struct {
    ATReqCommonType         common;/* required for all structures!!!            */
    Uint8Type               value;
} ATReqSetMaxCallsType;


/* data structure to pass dtmf characters */
typedef struct {
    ATReqCommonType     common;/* the common is required for all structures!!!  */
    Uint8Type           value;
} ATReqDtmfType;

/* data structure to pass PLPD parameter / request  */
typedef struct {
    ATReqCommonType     common;/* the common is required for all structures!!!  */
    ATPlpdReqTypeEnum   mode;
    AT_DeviceId         device_id; /* only used if mode = AT_PLPD_GET_DEVICE    */
} ATReqPlpdType;

/* data structure to pass ppch characters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType           common;
    ATPhoneBookCallHistEnum   action;
} ATReqPBCallHistType;

/* data structure to pass ppbcex characters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType                     common;
    ATPhoneBookCallStackeChangeModeEnum mode;
} ATReqPBCallStackChangeType;


/* data structure for multi party call */
typedef struct {

    ATReqCommonType           common;
    ATMPCActionEnum           action;
    ATBooleanEnum             idx_available;
    ATCallIndexEnum           index;

}ATReqMultiPartyCallType;



/* data structure to pass ppbr parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType           common;
    /* indicate if query is to get back size of phonebook */
    ATBooleanEnum             is_get_pb_size;
    /* additional 2 parameters only required if is_get_pb_size FALSE */
    Uint16Type                start_idx; /* range 1.. pb_size */
    Uint16Type                end_idx; /* range start_idx to pb_size */
    /* NOTE!!! to avoid long response times diff between start and end
     * shall not exceed MAX_NUM_PB_QUERY_SIZE */

    // SUSANNE: 20.10.2009 added pbSize field cause we need to check against this
    Uint16Type                pbSize;
} ATReqPBEntryType;

/* data structure to pass ppbp parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType           common;
    Uint16Type                pb_idx;
    Uint16Type                pn_idx1;
    Uint16Type                pn_idx2;
} ATReqPBNameType;

/* data structure to pass pmcs sms char set */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATSmsCharsetEnum   charset;
} ATReqSmsCharSetType;

/* data structure to pass pmrd sms read parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATSmsTypeEnum      type;
    Uint16Type         index;
    ATBooleanEnum      maxlen_avail;
    Uint16Type         maxlen;
    /* NOTE: Offset is only possible if maxlen is available!!! */
    ATBooleanEnum      offset_avail;
    Uint16Type         offset;
} ATReqSmsReadType;


/* data structure to pass pmrd sms send parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATSmsPhoneNumber   phone_number;
    Uint16Type         phone_number_len;
    ATSmsString        message;
    Uint16Type         message_len;
} ATReqSmsSendType;

/* data structure to pass pmsdex sms send parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATSmsConcModeEnum  mode;
    ATSmsPhoneNumber   phone_number;
    Uint16Type         phone_number_len;
    ATSmsString        message;
    Uint16Type         message_len;
} ATReqSmsConcSendType;

/* data structure to pass pmde sms delete parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATSmsTypeEnum      type;
    Uint16Type         index;
} ATReqSmsDeleteType;


/* data structure for dgps player status parameters */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATDeviceTypeEnum   device_type;
    AT_DeviceId        device_id;
    ATBooleanEnum      ext_info_requested;
} ATReqGetPlayerStatusType;

/* data structure for dply playing */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATDeviceTypeEnum   device_type;
    AT_DeviceId        device_id;
    Uint16Type         mdb_id; /* music database id */
    ATDMDBActionEnum   action;
    ATBooleanEnum      index_avail;
    Uint16Type         index;
} ATReqPlayerPlayingType;

/* data structure for dsra set random mode */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType    common;
    ATDeviceTypeEnum   device_type;
    AT_DeviceId        device_id;
    ATRandomModeStatusEnum random_enabled;
} ATReqPlayerSetRandomType;

/* data structure for dsre set repeat mode */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATDeviceTypeEnum        device_type;
    AT_DeviceId             device_id;
    ATPlayerRepeatModeEnum  repeat_mode;
} ATReqPlayerSetRepeatType;

/* data structure for dged get entry data */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATDeviceTypeEnum        device_type;
    AT_DeviceId             device_id;
    ATBooleanEnum           filepath_requested;
    ATBooleanEnum           title_requested;
    ATBooleanEnum           artist_requested;
    ATBooleanEnum           album_requested;
    ATBooleanEnum           tracknumber_requested;
    ATBooleanEnum           genre_requested;
    ATBooleanEnum           index_avail;
    Uint16Type              index;
} ATReqPlayerGetEntDataType;

/* data structure for PBSC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    AT_DeviceId             device_id;
    ATServiceActionEnum     action;
    ATBooleanEnum           service_avail;
    ATBTServiceEnum         service;
} ATReqBTServiceConnType;

/* data structure for PKEV */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATReqKeyboardKeysEnum   key;
    ATReqKeyboardEventEnum  event;
} ATReqKeyboardKeyType;

/* data structure for PGMT */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    AT_DeviceId             device_id;
} ATReqGetMetadataType;

/* data structure for DCPN */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATReqConfigPositionNotiEnum           automatic_notification;
} ATReqConfigPositionNotiType;


/* data structure for PSBD */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    AT_DeviceId             device_id;
    ATBTDeviceSelectEnum    value;
} ATReqBTDeviceSelectType;

/* data structure for PSFN */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           is_get_name;
    AT_BTDeviceName         name;
    Uint16Type              name_len;
} ATReqBTSetBtNameType;


/* data structure for PSDM */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBTDiscoverModeEnum    mode;
} ATReqBTSetDiscoverType;

/* data structure for PSMD */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           is_get_number;
    Uint8Type               max_num_paired_device;
}ATReqMaxNumPairedDevType;

/* data structure for PPDE */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           device_available;
    AT_DeviceId             device_id;
} ATReqDeletePairingType;

/* data structure for PALGOSELECT */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATMicAlgoEnum           algotype;
} ATReqMicAlgoSelType;

/* data structure for PALGOSETMIC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               mic_id;
} ATReqMicIdType;

/* data structure for +VGM */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               mic_gain;
} ATReqMicGainType;

/* data structure for PALGOADC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATADCMicAlgoEnum        parameter;
    Uint8Type               value;
} ATReqAlgoAdcType;

/* data structure for PALGONR */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               maxAttNr;
    ATBooleanEnum           direction_avail;
    ATNRDirectionEnum       direction;         /* direction only valid if AT_TRUE == direction_avail*/
    ATBooleanEnum           inputGain_avail;
    Sint8Type               inputGain;         /* inputGain only valid if AT_TRUE == inputGain_avail*/
} ATReqAlgoNRType;

/* data structure for PALGOTXEQ */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Sint8Type               Gain[AT_EQU_NUM_FREQ_BAND];
} ATReqAlgoEquType;

/* data structure for PALGOSELECTRX */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATSpeakerAlgoEnum       algoRX;
} ATReqAlgoSelectRXType;

/* data structure for PALGORX - limitier and HP filter */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               limitLevel;
    ATBooleanEnum           hpFilter_avail;
    Uint8Type               hpFilter;
} ATReqAlgoRXType;

/* data structure for PALGOAEC - echo cancellation */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               maxAttAec;
    ATBooleanEnum           switchTime_avail;
    Uint8Type               switchTime;
} ATReqAlgoAecType;


/* data structure for PNVS - volume setting */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATAudioVolumeTypeEnum   type; /* note: for type == AT_AUDIOTYPE_GET volume is ignored */
    Uint8Type               volume;
} ATReqAudioVolumeType;
/* VOICE RECOGNITION CONTROL structures */

/* data structure for RREC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           start_engine;
} ATReqVCEngMgmtType;


/* data structure for RPPE */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATSIVRPromptEnum        menu_id;
} ATReqVCSetMenuType;

/* data structure for PSSP */
typedef struct {
    ATReqCommonType                 common;
    ATSSPModeEnum                   mode;
    ATSSPIoCapParameterEnum         IOCap;
}ATReqSSPSetupType;

/* data structure for pbsf */
typedef struct {
    ATReqCommonType                 common;
    AT_DeviceId                     device_id;
    ATPairingDeviceType             device_type;
    ATBTFeatureEnum                 feature;
}ATReqBTSupportFeatureType;

typedef struct {
    ATSIVRPromptTypeEnum    prompt_type;
    Uint16Type              prompt_id; /* for VCR backward compatibility */
    Uint16Type              text_len; /* shall be set to 0 if no text available!!! */
    ATSIVRPromptText        text;
} ATReqCustomPrmtType;

typedef struct {
    ATSIVRPromptTypeEnum    prompt_type;
    Uint16Type              prompt_id;    /* only valid id included if type != AT_SIVR_PROMPT_CUSTOM*/
    Uint8Type               num_opt_avail; /* between 0 and 4 */
    Uint16Type              opt_len[AT_RPMT_MAX_NUM_OPT];
    ATSIVRPrmtOptText       opt[AT_RPMT_MAX_NUM_OPT];
} ATReqOtherPrmtType;

typedef union {
    ATReqCustomPrmtType     custom;
    ATReqOtherPrmtType      other;
} ATReqPromptUnion;


/* data structure for RPMT */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           start_play;
    ATBooleanEnum           beep_after;
    ATReqPromptUnion        prmt;
} ATReqVCMgtPromtType;

/* data structure for RGSL */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           is_get_lang; /* if set true next params are not used */
    ATBooleanEnum           index_used;  /* if index used fetch langs from start to stop
                                            otherwise query all */
    Uint8Type               start_index; /* between 1 and max num lang */
    Uint8Type               stop_index;  /* between start and max num lang */
} ATReqVCGetLangType;

/* data structure for RSCL */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           is_get_current_lang; /* if set true next params are not used */
    Uint8Type               lang_index;
} ATReqVCSetLangType;

/* data structure for RSRF */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           is_get_result_format; /* if set true next params are not used */
    Uint8Type               max_result;
    Uint8Type               rejection_level;
} ATReqVCResultFormatType;

/* data structure for PDAS */
typedef struct {
    ATReqCommonType         common;
    Uint8Type               mode;
} ATReqPBSyncModeType;

/* data structure for PPBD */
typedef struct {
    ATReqCommonType         common;
    ATBooleanEnum           index_avail;
    Uint16Type              index;
} ATReqPBDelEntryType;


/* data structure for PPNO */
typedef struct {
    ATReqCommonType         common;
    Uint8Type               mode;
} ATReqPBSortModeType;

/* data structure for PPAC */
typedef struct {
    ATReqCommonType         common;
    Uint8Type               letter;
} ATReqPBAlphaCountType;

/* data structure for PPAD */
typedef struct {
    ATReqCommonType           common;
    AT_CallerId               name;
    ATPhoneNumberType         phone_number;
    ATPhoneNumberTypeEnum     pn_type;
    Uint16Type                name_len;
    Uint16Type                phone_number_len;
} ATReqPBAddEntryType;

/* data structure for PSRT */
typedef struct {
    ATReqCommonType           common;
    ATBooleanEnum             is_get; /* only query current setting, all other
                                         parameters are ignored */
    ATRintoneModeEnum         mode;
    ATBooleanEnum             action_available; /* if set action is available */
    ATRingtoneReqActionEnum   action;
} ATReqRingtoneType;


/* data structure for PPAS */
typedef struct {
    ATReqCommonType           common;
    AT_CallerId               initial;
    Uint16Type                initial_len;
} ATReqPbAlphaSearchType;

/* data structure for PPASEX */
typedef struct {
    ATReqCommonType           common;
    AT_CallerId               initial;
    Uint16Type                initial_len;
} ATReqPbAlphaSearchExtendedType;



/* data structure for PLOOPBACK */
typedef struct {
    ATReqCommonType           common;
    ATBooleanEnum             start_loop;
    ATTestLoopbackModeEnum    mode;
} ATReqTestLoopbackType;

/* data structure for PTEST */
typedef struct {
    ATReqCommonType           common;
    Uint8Type                 mode;         /* fix set to 10 */
    ATBooleanEnum             bt_available;
    AT_BT_Address             bt_addr;
    ATBooleanEnum             delay_available;
    Uint16Type                delay_local;  /* local audio loop in second */
    Uint16Type                delay_BT;     /* audio loop by BT in second */
} ATReqTestAudioLoopType;


/* data structure for PFREQ */
typedef struct {
    ATReqCommonType       common;
    ATTestFreqModeEnum    mode;
} ATReqTestFreqType;

/* data structure for PSCM */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get; /* for fetching current PSCM mode form CK5050 */
    ATBooleanEnum         mode_disable;
    ATBooleanEnum         action_available;
    ATBooleanEnum         action_enable_no_start;
} ATReqSetAutoConnType;

/* data structure for PRVP */
typedef struct {
    ATReqCommonType       common;
    ATVTRecActionEnum     action;
    ATBooleanEnum         pb_idx_avail;
    Uint16Type            pb_idx;
} ATReqRecordVTType;

/* data structure for PRVK */
typedef struct {
    ATReqCommonType       common;
    ATVTRecActionEnum     action;
    ATBooleanEnum         kw_idx_avail;
    ATVTRecKWIdxEnum      kw_idx;
} ATReqRecordVKType;

/* data structure for PTVP, PPVP, PDVP */
typedef struct {
    ATReqCommonType       common;
    Uint16Type            pb_idx;
} ATReqPBIdxVTType;

/* data structure for PTVK, PPVK, PDVK*/
typedef struct {
    ATReqCommonType       common;
    ATVTRecKWIdxEnum      kw_idx;
} ATReqKWIdxVKType;

/* data structure for PAVP, RPDR */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get;
    ATBooleanEnum         mode_enable;
} ATReqAutoVTType;


/* data structure for RVTA */
typedef struct {
    ATReqCommonType       common;
    ATVTMgrActionType     action;
    Uint16Type            identifier;
    ATBooleanEnum         type_avail;
    ATSIVRItemTypeEnum    type;
} ATReqSIVRMgtVTType;


/* data structure for PALGONOISECOMFORT */
typedef struct {
	ATReqCommonType       common;
	ATBooleanEnum         is_get;
	ATBooleanEnum         mode_activated;
} ATReqPalgoNoiceComfType;

/* data structure for PALGOAECDELAY */
typedef struct {
	ATReqCommonType       common;
	ATBooleanEnum         is_get;
	Uint8Type             delay;
} ATReqPalgoAECDelayType;

/* data structure for PALGOALCRX */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get;          /* indicate if this is query for cur set */
    ATBooleanEnum         mode_activated;  /* only needed if is_get AT_FALSE - give mode */
    Uint8Type             target;          /* give target value if mode activated AT_TRUE */
    Uint8Type             mode;            /* give the ALC mode aka version (no, 1.0, 2.0, 3.0) */
} ATReqPalgoAlcRXType;

/* data structure for PALGOSLRX */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get;          /* indicate if this is query for cur set */
    ATBooleanEnum         mode_activated;  /* only needed if is_get AT_FALSE */
    Uint8Type             level;           /* value for softlimiter in reception*/
} ATReqPalgoSlRXType;

/* data structure for PALGOSLTX */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get;          /* indicate if this is query for cur set */
    ATBooleanEnum         mode_activated;  /* only needed if is_get AT_FALSE */
    Uint8Type             level;           /* level in dB for the limiter in transmission */
} ATReqPalgoSlTXType;

/* data structure for PBDI */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_initiate;
} ATReqInquiryType;

/* data structure for PGIR */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get;
    Uint8Type             startIndex;
    Uint8Type             endIndex;
} ATReqGetInquiryResultType;

/* data structure for PPRQ */
typedef struct {
    ATReqCommonType       common;
    AT_DeviceId           device_id;
    ATPairingDeviceType   device_type;
    ATPairingCommandEnum  pairingCmd;
    ATBooleanEnum         pin_avail;
    Uint8Type             pin_digits; /* indicate valid pin code digits  */
    ATPinCodeBT           pin;
} ATReqInitAcRejPairType;

/* data structure for PPRQ */
typedef struct {
    ATReqCommonType       common;
    AT_DeviceId           device_id;
} ATReqCoDProfileType;


/* data structure for PBSM */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         is_get;
    ATBooleanEnum         aHFPActivated;
} ATReqBTServiceModeType;

/* data structure for PEDS */
typedef struct {
    ATReqCommonType       common;
    ATBooleanEnum         enable;
} ATReqEnablePbDlStateType;

typedef struct {
    ATReqCommonType         common;
    ATBTServiceEnum         service;
    ATBTAutoConnMode        mode;
    Uint8Type               bitmask;
}ATReqBTServAutoConnModeType;

/* data structure for AGSW */
typedef struct {
    ATReqCommonType         common;
    Uint16Type              frequencyLeft;
    Uint16Type              frequencyRight;
    ATBooleanEnum           chanLeftOn;
    ATBooleanEnum           chanRightOn;
    Uint16Type              outputLevelLeft;
    Uint16Type              outputLevelRight;
}ATReqAlternatingSignalType;


/* data structure for PBSU */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               BTSWupdate;
} ATReqBTSWupdateType;


/* data structure for PPUS */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    ATBooleanEnum           PhonebookUpdate;
} ATReqPhonebookUpdateStatusType;


/* data structure for PSCA */
typedef struct {
    /* the common is required for all structures!!!  */
    ATReqCommonType         common;
    Uint8Type               ConnectMode;
} ATReqSetConAuthType;



/* data structure for PBSV */
typedef struct {
    ATReqCommonType         common;
    ATBTServiceEnum         service;
    ATPbsvActionType        action;
} ATReqBTServiceVisabilityType;

/* data structure for PECA */
typedef struct {
    ATReqCommonType         common;
    ATPecaActionType        action;
} ATReqErrorMessagesOnOffType;

typedef struct {
    ATReqCommonType         common;
    ATRintoneModeEnum       ringtoneId;
    ATPprtActionType        action;
}ATReqPlayRingtoneType;

typedef struct {
    ATReqCommonType         common;
    ATPirtActionType        action;
} ATReqInbandRingtoneType;

typedef struct {
    ATReqCommonType         common;
    ATPopmModeType          mode;
}ATReqObjectPushType;

typedef struct {
    ATReqCommonType         common;
    ATPcddStateType         state;
} ATReqDTMFPlayingType;


typedef struct {
    ATReqCommonType         common;
    ATAlckLockType          locked;
} ATReqAudioLockedType;

typedef struct {
    ATReqCommonType         common;
    ATPevnModeType          SpeakerVolumeNotification;
    ATPevnApplySpkVolType   ApplySpeakerVolume;
} ATReqSpeakerVolumeType;

/* data structure for PBFREQEXinfineon */
typedef struct {
    ATReqCommonType         common;
    ATTestScenarioType      testscenario;
    Uint8Type               packettype;
    ATTestBitPatternType    bitpattern;
    Uint8Type               singlefrequency;
    Uint8Type               txburstperiod;
    ATTestScramblerModeType scramblermode;
    ATTestPowerLevelType    powerlevel;
    Uint16Type              packetsize;
} ATReqRxTxTestType;


/* data structure UNION                                                         */
typedef union {
    /* the common is required for mandatory req type */
    ATReqCommonType                 common;
    ATReqHangupType                 chup;
    ATReqCallNumberType             callNumber;
    ATReqCallIndexType              atdi;
    ATReqCallTypeOfNumberType       atdp;
    ATReqDtmfType                   dtmf;
    ATReqMicMuteType                micMute;
    ATReqSetPairModeType            ppau;
    ATExtCallModeType               plccex;
    ATReqSetMaxCallsType            psmc;
    ATReqPlpdType                   plpd;
    ATReqAudioTransferType          patr;
    ATReqAudioGatewayType           pagm;
    ATReqAudioGatewayTransferType   pagt;
    ATReqSelectPbType               ppbs;
    ATReqPBCallHistType             ppch;
    ATReqPBCallStackChangeType      ppbcex;
    ATReqMultiPartyCallType         pmpc;
    ATReqPBEntryType                ppbr;
    ATReqSmsCharSetType             pmcs;
    ATReqSmsReadType                pmrd;
    ATReqSmsSendType                pmsd;
    ATReqSmsConcSendType            pmsdex;
    ATReqSmsDeleteType              pmde;
    ATReqPBNameType                 ppbp;
    ATReqPBSyncModeType             pdas;
    ATReqPBDelEntryType             ppbd;
    ATReqPBSortModeType             ppno;
    ATReqPBAlphaCountType           ppac;
    ATReqPBAddEntryType             ppad;
    ATReqGetPlayerStatusType        dgps;
    ATReqPlayerPlayingType          dply;
    ATReqPlayerSetRandomType        dsra;
    ATReqPlayerSetRepeatType        dsre;
    ATReqPlayerGetEntDataType       dged;
    ATReqConfigPositionNotiType     dcpn;
    ATReqBTServiceConnType          pbsc;
    ATReqKeyboardKeyType            pkev;
    ATReqGetMetadataType            pgmt;
    ATReqBTDeviceSelectType         psbd;
    ATReqBTSetBtNameType            psfn;
    ATReqBTSetDiscoverType          psdm;
    ATReqMaxNumPairedDevType        psmd;
    ATReqDeletePairingType          ppde;
    ATReqMicAlgoSelType             palgoselect;
    ATReqMicIdType                  palgosetmic;
    ATReqMicGainType                vgm;
    ATReqAlgoNRType                 palgonr;
    ATReqAlgoEquType                palgotxeq;
    ATReqAlgoAdcType                palgoadc;
    ATReqAlgoSelectRXType           palgoselectrx;
    ATReqAlgoEquType                palgorxeq;
    ATReqAlgoRXType                 palgorx;
    ATReqAlgoAecType                palgoaec;
    ATReqPalgoNoiceComfType         palognoisecomfort;
    ATReqPalgoAECDelayType          palgoaecdelay;
    ATReqPalgoAlcRXType             palgoalcrx;
    ATReqPalgoSlRXType              palgoslrx;
    ATReqPalgoSlTXType              palgosltx;
    ATReqAudioVolumeType            pnvs;
    ATReqVCEngMgmtType              rrec;
    ATReqVCSetMenuType              rpre;
    ATReqVCMgtPromtType             rpmt;
    ATReqVCGetLangType              rgsl;
    ATReqVCSetLangType              rscl;
    ATReqVCResultFormatType         rsrf;
    ATReqRingtoneType               psrt;
    ATReqPbAlphaSearchType          ppas;
    ATReqPbAlphaSearchExtendedType  ppasex;
    ATReqTestLoopbackType           ploopback;
    ATReqTestAudioLoopType          paudioloop;
    ATReqTestFreqType               pbfreq;
    ATReqSetAutoConnType            pscm;
    ATReqRecordVTType               prvp;
    ATReqRecordVKType               prvk;
    ATReqPBIdxVTType                ptvp;
    ATReqKWIdxVKType                ptvk;
    ATReqPBIdxVTType                ppvp;
    ATReqKWIdxVKType                ppvk;
    ATReqPBIdxVTType                pdvp;
    ATReqKWIdxVKType                pdvk;
    ATReqAutoVTType                 pavp;
    ATReqAutoVTType                 rpdr;
    ATReqSIVRMgtVTType              rvta;
    ATReqInquiryType                pbdi;
    ATReqGetInquiryResultType       pgir;
    ATReqInitAcRejPairType          pprq;
    ATReqBTServiceModeType          pbsm;
    ATReqEnablePbDlStateType        peds;
    ATReqBTServAutoConnModeType     pbsa;
    ATReqCoDProfileType             pbcd;
    ATReqAlternatingSignalType      agsw;
    ATReqBTSWupdateType             pbsu;
    ATReqBTServiceVisabilityType    pbsv;
    ATReqErrorMessagesOnOffType     peca;
    ATReqSetConAuthType             psca;
    ATReqPhonebookUpdateStatusType  ppus;
    ATReqPlayRingtoneType           pprt;
    ATReqInbandRingtoneType         pirt;
    ATReqObjectPushType             popm;
    ATReqDTMFPlayingType            pcdd;
    ATReqAudioLockedType            alck;
    ATReqRxTxTestType               pbfreqex;
    ATReqSpeakerVolumeType          pevn;
    ATReqSSPSetupType               pssp;
    ATReqBTSupportFeatureType       pbsf;
} ATReqAllType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/* see ATCmdHandler.h                                                         */


#endif /* _ATCMDHANDLER_DATA_H */

/* End Of File ATCmdHandlerData.h */
