/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATParser.h
*-----------------------------------------------------------------------------
* Module Name:    AT Parser
*-----------------------------------------------------------------------------
* Description:    The AT Parser search for carrage returns und line feeds.
                  Analysis the incomming data from the UART minterface.
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/inc/ATParser.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATPARSER_H
#define _ATPARSER_H

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
#include "ATCommonData.h"
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
/* Public Enumerations                                                        */
/*----------------------------------------------------------------------------*/


/* NOTE: a response is defined as command FROM CK5050 to HOST                */
typedef enum
{   /* enumerations with special meaning                                      */
/* 0 */   AT_UNUSED      = 0, /* 0 shall be unused to avoid problems if mem not set */

/* UHV enum                  CK5050 code                     direction    */
/* 1 */    AT_RSP_ERROR,                /* ERROR */
/* 2 */    AT_RSP_OK,                   /* OK    */

           /* general commands   - see chapter 3 of SW host IF                       */
/* 3  */    AT_RSP_STARTUP,              /*   *PSTU                   CK5050  -> HOST */
/* 4  */    AT_RSP_BT_ANTENNA,           /*   *PBTA                   CK5050 <-> HOST */
/* 5  */    AT_RSP_SET_BAUD,             /*   *PSBR                   CK5050 <-> HOST */
/* 6  */    AT_RSP_MANUFAC_ID,           /*   +CGMI                   CK5050 <-> HOST */
/* 7  */    AT_RSP_MODEL_ID,             /*   +CGMM                   CK5050 <-> HOST */
/* 8  */    AT_RSP_MODEL_REV,            /*   +CGMR                   CK5050 <-> HOST */
/* 9  */    AT_RSP_MODEL_REV_EXT,        /*   +CGMREX                 CK5050 <-> HOST */
/* 10 */    AT_RSP_POWER_OFF,            /*   *POFF                   CK5050 <-> HOST */
/* 11 */    AT_RSP_SET_BT_NAME,          /*   *PSFN                   CK5050 <-> HOST */
/* 12 */    AT_RSP_SET_BT_ADDR,          /*   *PBDADDR                CK5050 <-> HOST */
/* 13 */    AT_RSP_GEN_WARNING,          /*   *PWNG                   CK5050  -> HOST */
/* 14 */    AT_RSP_RINGTONE_END,         /*   *PPRT                   CK5050  -> HOST */

        /* BT pairing commands - see chapter 4 of SW host IF                      */
/* 15 */    AT_RSP_INQUIRY,              /*   *PBDI                   CK5050 <-> HOST */
/* 16 */    AT_RSP_GET_INQUIRY_RESULT,   /*   *PGIR                   CK5050 <-> HOST */
/* 17 */   AT_RSP_EXTENDED_INQUIRY,     /*   *PGIREX                 CK5050 <-> HOST */
/* 18 */    AT_RSP_HDL_PAIR_REQUEST,     /*   *PPAU                   CK5050 <-> HOST */
/* 19 */    AT_RSP_IN_PAIR_REQUEST,      /*   *PPRQ                   CK5050 <-> HOST */
/* 20 */    AT_RSP_PAIR_REQUEST_RESULT,  /*   *PPRS                   CK5050  -> HOST */
/* 21 */    AT_RSP_GET_PAIRED_DEVICES_NB,/*   *PLPD                   CK5050 <-> HOST */
/* 22 */    AT_RSP_GET_PAIRED_DEVICES_IN,/*   *PLPD                   CK5050 <-> HOST */
/* 23 */    AT_RSP_PAIR_DELETE,          /*   *PPDE                   CK5050 <-> HOST */
/* 24 */    AT_RSP_SET_DISCOVER_MODE,    /*   *PSDM                   CK5050 <-> HOST */
/* 25 */    AT_RSP_SET_MAX_PAIRED_DEVICES, /* *PSMD                   CK5050 <-> HOST */
/* 26 */    AT_RSP_SPECIAL_DEV_LIST,     /*   *PSDR                   CK5050 <-> HOST */
/* 27 */    AT_RSP_SPECIAL_DEV_SELECT,   /*   *PLSD                   CK5050 <-> HOST */
/* 28 */    AT_RSP_BT_CHIP_VERSION,      /*   *PBCV                   CK5050 <-> HOST */
/* 29 */   AT_RSP_IMSI           ,      /*   *PSSN                   CK5050 <-> HOST */

        /* BT connection commands - see chapter 5 of SW host IF                   */
/* 30 */    AT_RSP_BT_SELECT_DEV,        /*   *PSBD                   CK5050 <-> HOST */
/* 31 */    AT_RSP_BT_SERVICE_CONN,      /*   *PBSC                   CK5050 <-> HOST */
/* 32 */    AT_RSP_BT_SERVICE_NOT,       /*   *PBSN                   CK5050  -> HOST */
/* 33 */    AT_RSP_BT_SDP_NOT,           /*   *PSDP                   CK5050  -> HOST */
/* 34 */    AT_RSP_BT_READ_RSSI,         /*   *PBRSSI                 CK5050 <-> HOST */
/* 35 */    AT_RSP_BT_AG_MODE,           /*   *PAGM                   CK5050 <-> HOST */
/* 36 */    AT_RSP_BT_AG_TRANSFER,       /*   *PAGT                   CK5050 <-> HOST */

        /* BT connection settings - see chapter 6 of SW host IF                   */
/* 37 */    AT_RSP_BT_SET_AC_MODE,       /*   *PSCM                   CK5050 <-> HOST */
/* 38 */    AT_RSP_BT_SERVICE_AC,        /*   *PBSA                   CK5050 <-> HOST */
/* 39 */    AT_RSP_BT_AC_MODE_STATUS,    /*   *PCMS                   CK5050 <-> HOST */
/* 40 */    AT_RSP_BT_SERVICE_MODE,      /*   *PBSM                   CK5050 <-> HOST */
/* 41 */    AT_RSP_BT_SET_CONN_AUTH,     /*   *PSCA                   CK5050 <-> HOST */
/* 42 */    AT_RSP_BT_SERVICE_VISIBILITY,/*   *PBSV                   CK5050 <-> HOST */
/* 43 */    AT_RSP_BT_SSP_SETUP,         /*   *PSSP                   CK5050 <-> HOST */
/* 44 */    AT_RSP_BT_SSP_REQUEST,       /*   *PSPR                   CK5050  -> HOST */
/* 45 */    AT_RSP_BT_SUP_FEAT,          /*   *PBSF                   CK5050  -> HOST */

         /* Phonebook mgmt commands - see chapter 7 of SW host IF                  */
/* 45 */    AT_RSP_PB_CHANGE_IND_TYPE,   /*   *PPBC                   CK5050  -> HOST */
/* 46 */    AT_RSP_PB_CHANGE_IND_EVENT,  /*   *PPBU                   CK5050  -> HOST */
/* 47 */    AT_RSP_PB_AVAIL_NAV,         /*   *PFPA                   CK5050  -> HOST */
/* 48 */    AT_RSP_PB_UPDATE_STATUS,     /*   *PPUS                   CK5050 <-> HOST */
/* 49 */    AT_RSP_PB_SELECT,            /*   *PPBS                   CK5050 <-> HOST */
/* 50 */    AT_RSP_PB_READ_ENTRIES,      /*   *PPBR                   CK5050 <-> HOST */
/* 51 */    AT_RSP_PB_READ_NUMBERS,      /*   *PPBP                   CK5050 <-> HOST */
/* 52 */    AT_RSP_PB_TOTAL_COUNT,       /*   *PPNC                   CK5050 <-> HOST */
/* 53 */    AT_RSP_PB_CALL_HISTORY,      /*   *PPCH                   CK5050 <-> HOST */
/* 54 */    AT_RSP_PB_ALPHA_SEARCH,      /*   *PPAS                   CK5050 <-> HOST */
/* 55 */    AT_RSP_PB_ALPHA_SEARCH_EXT,  /*   *PPASEX                 CK5050 <-> HOST */
/* 56 */    AT_RSP_PB_SEARCH,            /*   *PPSH                   CK5050 <-> HOST */
/* 57 */    AT_RSP_PB_EN_DWNLOAD_STATUS, /*   *PEDS                   CK5050 <-> HOST */
/* 58 */    AT_RSP_PB_DWNLOAD_STATUS,    /*   *PPDS                   CK5050  -> HOST */
/* 59 */    AT_RSP_PB_DISABLE_AUTO_SYNC, /*   *PDAS                   CK5050 <-  HOST */
/* 60 */    AT_RSP_PB_DELETE_ENT_ALL_END,/*   *PPBD                   CK5050 <-> HOST */
/* 61 */    AT_RSP_PB_ADD_ENTRY,         /*   *PPAD                   CK5050 <-> HOST */
/* 62 */    AT_RSP_PB_NAME_ORDER_SELECT, /*   *PPNO                   CK5050 <-> HOST */
/* 63 */    AT_RSP_PB_GET_ALPHA_COUNT,   /*   *PPAC                   CK5050 <-> HOST */

         /* Call management commands - see chapter 8 of SW host IF                 */
/* 64 */    AT_RSP_CALL_STATUS_IND_COUNT,/*   *PNCC                   CK5050  -> HOST */
/* 65 */    AT_RSP_CALL_STATUS_IND_INDEX,/*   *PLCC                   CK5050 <-> HOST */
/* 66 */    AT_RSP_EXT_CALL_LIST,        /*   *PLCCEX                 CK5050 <-> HOST */
/* 67 */    AT_RSP_CURR_CALL_INFO,       /*   *PCCI                   CK5050 <-> HOST */
/* 68 */    AT_RSP_AUDIO_TRANS_DISCRETE, /*   *PATR                   CK5050 <-> HOST */
/* 69 */    AT_RSP_PHONE_STATUS_IND,     /*   *PIND=?                 CK5050 <-> HOST */
/* 70 */    AT_RSP_PHONE_STATUS_IND_VAL, /*   *PIND?                  CK5050 <-> HOST */
/* 71 */    AT_RSP_PHONE_STATUS_IND_EVT, /*   *PIEV                   CK5050  -> HOST */
/* 72 */    AT_RSP_CLASS_DEVICE_REQUEST, /*   *PBCD                   CK5050 <-> HOST */
/* 73 */    AT_RSP_GET_PHONE_FEATURES,   /*   *PPSF                   CK5050 <-> HOST */

         /* Audio Microphone commands - see chapter 9 of SW host IF                */
/* 74 */    AT_RSP_MIC_ALGO_SELECT,      /*   *PALGOSELECT            CK5050 <-> HOST */
/* 75 */    AT_RSP_MIC_CHOICE_SET,       /*   *PALGOSETMIC            CK5050 <-> HOST */
/* 76 */    AT_RSP_MIC_GAIN_SET,         /*   +VGM                    CK5050 <-> HOST */

         /* Audio speaker settings - see chapter 10 of SW host IF                  */
         /* note: no response messages needed here!!!                              */
/* 77  */    AT_RSP_SPK_ALC_RX,           /*   *PALGOALCRX            CK5050 <-> HOST */
/* 77  */    AT_RSP_SPK_SL_RX,            /*   *PALGOSLRX             CK5050 <-> HOST */
/* 77  */    AT_RSP_SPK_SL_TX,            /*   *PALGOSLTX             CK5050 <-> HOST */

         /* Audio echo settings - see chapter 11 of SW host IF                     */
/* 78 */    AT_RSP_ECHO_DELAY_CANCELLATION,/*   *PALGOAECDELAY        CK5050 <-> HOST */
/* 79 */    AT_RSP_ECHO_NOISE_COMFORT,     /*   *PALGONOISECOMFORT    CK5050 <-> HOST */
/* 80 */    AT_RSP_ECHO_CHECK_NOISE,       /*   *PALGONOISECHK        CK5050 <-> HOST */

/* 81 */    AT_RSP_ECHO_CANCELLATION,      /*   *PALGOAEC             CK5050 <-> HOST */

         /* Voice Tag Mgmt commands - see chapter 12 of SW host IF                 */
/* 82 */    AT_RSP_VT_RECORD_INDEX,        /*   *PRVP                 CK5050 <-> HOST */
/* 83 */    AT_RSP_VT_RECORD_KEY,          /*   *PRVK                 CK5050 <-> HOST */
/* 84 */    AT_RSP_VT_TEST_PRESENCE_INDEX, /*   *PTVP                 CK5050 <-> HOST */
/* 85 */    AT_RSP_VT_TEST_PRESENCE_KEY,   /*   *PTVK                 CK5050 <-> HOST */
/* 86 */    AT_RSP_VT_PLAYBACK_INDEX,      /*   *PPVP                 CK5050 <-> HOST */
/* 87 */    AT_RSP_VT_PLAYBACK_KEY,        /*   *PPVK                 CK5050 <-> HOST */
/* 88 */    AT_RSP_VT_DELETE_INDEX,        /*   *PDVP                 CK5050 <-> HOST */
/* 89 */    AT_RSP_VT_DELETE_KEY,          /*   *PDVK                 CK5050 <-> HOST */
/* 90 */    AT_RSP_VT_GET_NUMBER_TAGS,     /*   *PNVP                 CK5050 <-> HOST */
/* 91 */    AT_RSP_VT_AUTO_TAG,            /*   *PAVP                 CK5050 <-> HOST */

         /* Voice Recognition commands - see chapter 13 of SW host IF              */
/* 92 */    AT_RSP_VR_TYPE,                /*   *PVRA                 CK5050 <-> HOST */
/* 93 */    AT_RSP_VR_ACTIVATE,            /*   +BVRA                 CK5050 <-> HOST */
/* 94 */    AT_RSP_VR_STATUS,              /*   *PVRS                 CK5050  -> HOST */

         /* Audio Streaming commands - see chapter 14 of SW host IF                */
/* 95 */    AT_RSP_AS_KEY_EVENT_INDICATOR, /*   *PKEV                 CK5050 <-> HOST */
/* 96 */    AT_RSP_AS_GET_META_TYPE,       /*   *PGMT                 CK5050 <-> HOST */

         /* UART multiplexing system commands - see chapter 15 of SW host IF       */
/* 97 */    AT_RSP_UART_GET_CHAN_INFO_MUX, /*   *PCIM                 CK5050 <-> HOST */

    /* Test commands - see chapter 16 of SW host IF                           */
    /* note: no response messages needed here!!!                              */

        /* SMS commands - see chapter 17 of SW host IF                            */
/* 98  */    AT_RSP_SMS_SUPPORT_NOT,        /*   *PMST                 CK5050  -> HOST */
/* 99  */    AT_RSP_SMS_GET_SUPPORT,        /*   *PMFT                 CK5050 <-> HOST */
/* 100  */    AT_RSP_SMS_LIST,               /*   *PMLS                 CK5050 <-> HOST */
/* 101  */    AT_RSP_SMS_MAX_FOLDER_SIZE,    /*   *PMFS                 CK5050 <-> HOST */
/* 102  */    AT_RSP_SMS_READING_NUMBER,     /*   *PMRD response PMRD?  CK5050 <-> HOST */
/* 103  */    AT_RSP_SMS_READING,            /*   *PMRD                 CK5055 <-> HOST */
/* 104 */    AT_RSP_SMS_CONC_SEND,          /*   *PMSDEX               CK5050 <-> HOST */
/* 105 */    AT_RSP_SMS_NEW_NOT,            /*   *PMNW                 CK5050  -> HOST */
/* 106 */    AT_RSP_SMS_ENABLE,             /*   *PMEN                 CK5050 <-> HOST */

         /* Flash management commands - see chapter 18 of SW host IF               */
/* 107 */    AT_RSP_FLASH_FULL_EVENT,       /*   *PMFL                 CK5050  -> HOST */
/* 108 */    AT_RSP_FLASH_SW_UPDATE_BT,     /*   *PBSU                 CK5050 <-> HOST */
/* 109 */    AT_RSP_FLASH_SW_UPDATE_DUN,    /*   *PDSU                 CK5050 <-> HOST */
/* 110 */    AT_RSP_FLASH_ERASE_USERDATA,   /*   *PFLASHERASE          CK5050 <-> HOST */

          /* Speaker independant VR commands - see chapter 19 of SW host IF         */
/* 111 */    AT_RSP_SIVR_START_UP_EVENT,    /*   *RSTU                 CK5050  -> HOST */
/* 112 */    AT_RSP_SIVR_START_UP_EVENT_EXT,/*   *RSTUEX               CK5050  -> HOST */
/* 113 */    AT_RSP_SIVR_GET_SUPPORTED_FEAT,/*   *RGSF                 CK5050 <-> HOST */
/* 114 */    AT_RSP_SIVR_GET_SUPPORTED_LANG,/*   *RGSL                 CK5050 <-> HOST */
/* 115 */    AT_RSP_SIVR_SET_CURRENT_LANG,  /*   *RSCL                 CK5050 <-> HOST */
/* 116 */    AT_RSP_SIVR_SET_T2S_PB_MODE,   /*   *RSTM                 CK5050 <-> HOST */
/* 117 */    AT_RSP_SIVR_SET_RESULT_FORMAT, /*   *RSRF                 CK5050 <-> HOST */
/* 118 */    AT_RSP_SIVR_LOAD_CONTEXT,      /*   *RLDC                 CK5050 <-> HOST */
/* 119 */    AT_RSP_SIVR_START_VR_ENGINE,   /*   *RSVR                 CK5050 <-  HOST */
/* 120 */    AT_RSP_SIVR_VR_STATUS,         /*   *RSTS                 CK5050  -> HOST */
/* 121 */    AT_RSP_SIVR_PREPARE_VR,        /*   *RPRE                 CK5050 <-> HOST */
/* 122 */    AT_RSP_SIVR_VR_RESULT,         /*   *RRES                 CK5050  -> HOST */
/* 123 */    AT_RSP_SIVR_VR_INFO_DATA,      /*   *RDAT                 CK5050  -> HOST */
/* 124 */    AT_RSP_SIVR_PLAY_TEXT,         /*   *RPTX                 CK5050 <-  HOST */
/* 125 */    AT_RSP_SIVR_MGR_VT,            /*   *RVTA                 CK5050 <-> HOST */

          /* Disco database mgmt commands - see chapter 20 of SW host IF            */
/* 126 */    AT_RSP_DISCO_OPEN_DB,          /*   *DOMD                 CK5050 <-> HOST */
/* 127 */    AT_RSP_DISCO_DB_UPDATE,        /*   *DDBU                 CK5050  -> HOST */
/* 128 */    AT_RSP_DISCO_SET_MAX_STRING_LEN,/*  *DMLS                 CK5050 <-> HOST */
/* 129 */    AT_RSP_DISCO_DELETE_DB,        /*   *DDDB                 CK5050 <-> HOST */

          /* Disco browsing commands - see chapter 21 of SW host IF                 */
/* 130 */    AT_RSP_DISCO_GET_BROWSE_MODE,  /*   *DGBM                 CK5050 <-> HOST */
/* 131 */    AT_RSP_DISCO_GET_CURRENT_DIR,  /*   *DGCD                 CK5050 <-> HOST */
/* 132 */    AT_RSP_DISCO_GET_ENTRY_COUNT,  /*   *DGEC                 CK5050 <-> HOST */
/* 133 */    AT_RSP_DISCO_LIST_ENTRIES,     /*   *DLSE                 CK5050 <-> HOST */
/* 134 */    AT_RSP_DISCO_GET_ENTRY_DATA,   /*   *DGED                 CK5050 <-> HOST */
/* 135 */    AT_RSP_DISCO_ALPHA_SEARCH,     /*   *DASH                 CK5050 <-> HOST */
/* 136 */    AT_RSP_DISCO_METADATA_UPDATE,  /*   *PAMU                 CK5050  -> HOST */

          /* Playing commands - see chapter 22 of SW host IF                        */
/* 137 */    AT_RSP_PLAYER_GET_STATUS,      /*   *DGPS                 CK5050 <-> HOST */
/* 138 */    AT_RSP_PLAYER_FAST_PLAY_SET,   /*   *DFPS                 CK5050 <-> HOST */

          /* Device management commands - see chapter 23 of SW host IF              */
/* 139 */    AT_RSP_DMGMT_LIST_MMD,         /*   *DLMD                 CK5050 <-> HOST */
/* 140 */    AT_RSP_DMGMT_MMD_CONN,         /*   *DMDC                 CK5050  -> HOST */
/* 141 */    AT_RSP_DMGMT_MMD_NOT_SUPPORTED,/*   *DDNS                 CK5050  -> HOST */

          /* Audio management commands - see chapter 24 of SW host IF               */
/* 142 */    AT_RSP_AMGMT_SRD_ACTIVE_IND,   /*   *PASA                 CK5050 <-> HOST */
/* 143 */    AT_RSP_AMGMT_OUT_VOLUME_SET,   /*   *PNVS                 CK5050 <-> HOST */
/* 144 */    AT_RSP_AMGMT_SET_RINGTONE,     /*   *PSRT                 CK5050 <-> HOST */
/* 145 */    AT_RSP_AMGMT_MIC_MUTE,         /*   *PMMU                 CK5050 <-> HOST */
/* 146 */    AT_RSP_AMGMT_I2S_AUDIO,        /*   *PAFH                 CK5050 <-> HOST */
/* 147 */    AT_RSP_AMGMT_UART_AUDIO_IN,    /*   *PSFH                 CK5050 <-> HOST */
/* 148 */    AT_RSP_AMGMT_AUDIO_LINE_IN,    /*   *ALIN                 CK5050 <-> HOST */

    AT_RSP_NUMBER_COMMAND_IDS      /*   used as indicator for max number
                                    of available command IDs if necessary     */
} ATResponseEnum;


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Structures                                                          */
/*----------------------------------------------------------------------------*/
/* common data structure to call the command handler                          */

/* data structure for all responses without data                              */
typedef struct {
    ATResponseEnum       rsp_type;
    APPSubModuleIdEnum   receiver; /* include the sub module receiver id      */
} ATRspCommonType;


typedef struct {
    ATRspCommonType       common;
    AT_BT_Address         value;
} ATRspPbAddressType;

/* PBTA */
typedef struct {
    ATRspCommonType       common;
    ATParamPresentEnum    value;
} ATRspBTAntennaType;

/* PNCC message struct */
typedef struct {
    ATRspCommonType       common;
    AT_CallCount          value;
} ATRspCallStatCountType;

/* PLCC message struct */
typedef struct {
    ATRspCommonType           common;
    AT_CallIndex              index;
    ATCallStateEnum           state;
    ATBooleanEnum             id_avail; /* indicate if caller_id available    */
    AT_CallerId               caller_id;
    Uint8Type                 caller_id_len;
    ATBooleanEnum             type_avail; /* indicate if call_type avail      */
    ATPhoneNumberTypeEnum     call_type;
    ATBooleanEnum             ext_avail;
    AT_CallerIdExt            call_id_ext;
    Uint8Type                 caller_id_ext_len;
    ATCallDirTypeEnum         call_dir_type;
    ATCallConfTypeEnum        call_conf_type;
    ATCallModeTypeEnum        call_mode_type;
    /* note: call type can only be available if caller id is available, to have
     * structure more simple no substruct is used. Application shall check
     * type only if id is available and type availability is set              */
} ATRspCallStatValType;

/* PSBD message struct */
typedef struct {
    ATRspCommonType           common;
    AT_DeviceId               id;
    ATConnStatusEnum          status;
} ATRspSelectBTDeviceType;

/* PIEV message struct */
typedef struct {
    ATRspCommonType           common;
    ATIndicatorIndexEnum      index;
    AT_IndicatorValue         value;
    Uint8Type                 len; /* string len, 0 if no string present      */
    AT_IndicatorString        value_str;
    /* note: related to index either value or value_str is available          */
} ATRspPhoneStatusIndEventType;

/* PIND =? message struct enum if indicators are supported */
typedef struct {
    ATRspCommonType           common;
    AT_DeviceId               device_id;
    AT_BTClassOfDevice        bt_ClassOfDevice;
    ATBooleanEnum             profile_avail;
    ATCoDProfileEnum          profile;
} ATRspClassOfDeviceRequestType;

/* PIND =? message struct enum if indicators are supported */
typedef struct {
    ATRspCommonType           common;
    ATSupportedEnum           bt_conn;
    ATSupportedEnum           service;
    ATSupportedEnum           roaming;
    ATSupportedEnum           signal_level;
    ATSupportedEnum           operator_name;
    ATSupportedEnum           battery_level;
    ATSupportedEnum           sms_receive;
    ATSupportedEnum           voice_mail;
} ATRspPhoneStatusIndSupportType;

/* PIND? message struct - values included here */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             bt_conn; /* range 0..1*/
    Uint8Type             service; /* range 0..1*/
    Uint8Type             roaming; /* range 0..1*/
    Uint8Type             signal_level; /* range 0..5 */
    Uint8Type             operator_name_len;
    AT_IndicatorString2   operator_name;
    Uint8Type             battery_level; /* range 0..5 */
    Uint8Type             sms_receive; /* range??? number of received SMS messages */
    Uint8Type             voice_mail; /* range 0..1*/
} ATRspPhoneStatusIndValueType;


/* PWNG message struct */
typedef struct {
    ATRspCommonType       common;
    ATWarningCodeEnum     code;
    ATBooleanEnum         device_id_avail; /* indicate if id available    */
    AT_DeviceId           device_id;
} ATRspWarningType;

/* PLPD message struct */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             nb_devices;
} ATRspPlpdNbType;

/* PPBC message struct */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             type;
} ATRspPpbcType;

/* PPBU message struct */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             type;
} ATRspPpbuType;

/* PECA message struct */
typedef struct {
    ATRspCommonType       common;
    Uint16Type            errorCode;
} ATRspPecaType;

/* PLPD message struct */
typedef struct {
    ATRspCommonType       common;
    AT_DeviceId           device_id;
    Uint8Type             device_name_len;
    AT_DeviceNameString   device_name;
    AT_BT_Address         bt_addr;
    AT_BTClassOfDevice    bt_ClassOfDevice;
    ATSupportedEnum       support_dial;
    ATSupportedEnum       support_pb_access;
    ATBooleanEnum         is_connected;
    ATBooleanEnum         is_active;
} ATRspPlpdInfoType;


/* PSDP message struct */
typedef struct {
    ATRspCommonType           common;
    AT_DeviceId               id;
    ATSDPStatusEnum           status;
} ATRspSdpNotType;

/* PATR message struct */
typedef struct {
    ATRspCommonType           common;
    ATAudioTransferModeEnum   mode;
} ATRspAudioTransferType;

/* PAGM message struct */
typedef struct {
    ATRspCommonType           common;
    ATAudioGatewayModeEnum    mode;
} ATRspAudioGatewayType;

/* PAGT message struct */
typedef struct {
    ATRspCommonType                 common;
    ATAudioGatewayTransferModeEnum  mode;
} ATRspAudioGatewayTransferType;


/* PSRT message struct */
typedef struct {
    ATRspCommonType           common;
    ATRingToneModeEnum        mode_def;
    ATBooleanEnum             active_avail;
    ATRingToneModeEnum        active;
} ATRspSetRingtoneType;

/* PMMU response */
typedef struct {
    ATRspCommonType           common;
    ATMicModeEnum             mode;
} ATRspMicroMuteType;

/* PMMU response */
typedef struct {
    ATRspCommonType           common;
    ATPhoneBookTypeEnum       type;
} ATRspSelectPBType;

/* PPCH response */
typedef struct {
    ATRspCommonType           common;
    ATPhoneBookCallHistEnum   storage;
} ATRspPBCallHistType;

/* PMST response */
typedef struct {
    ATRspCommonType           common;
    ATBooleanEnum             sms_supported;
} ATRspSmsSupNotType;

/* PMRD SMS receive number response */
typedef struct {
    ATRspCommonType           common;
    Uint16Type                unread;
    Uint16Type                read;
    Uint16Type                unsent;
    Uint16Type                sent;
} ATRspSmsReceiveNumType;

/* PMRD SMS receive number response */
typedef struct {
    ATRspCommonType           common;
    ATSmsOrigin               origin;
    Uint16Type                origin_len;
    ATSmsDate                 date;
    Uint16Type                date_len;
    ATSmsString               message;
    Uint16Type                message_len;
    ATBooleanEnum             length_avail;
    Uint16Type                totallength;
} ATRspSmsReceiveMsgType;

/* PMSDEX SMS receive number of sent PDU response */
typedef struct {
    ATRspCommonType           common;
    ATSmsConc                 NbOfSentPdu;
} ATRspSmsConcReceiveType;

/* PPBR read phonebook entry structure */
typedef struct {
    ATRspCommonType           common;
    ATBooleanEnum             is_size_only;
    /* if is_size_only is AT_TRUE, only pb_size include valid data, other params not */
    Uint16Type                pb_size;
    /* if is_size_only is AT_FALSE pb_size include invalid data, params below are filled */
    Uint16Type                pb_idx;
    AT_CallerId               name;
    Uint16Type                name_len;
    Uint8Type                 pn_size;
    ATBooleanEnum             voice_tag_recorded;
    ATBooleanEnum             pn_type_numer_available;
    ATPhoneNumberTypeEnum     pn_type;
    ATPhoneNumberType         phone_number;
    Uint16Type                phone_number_len;
} ATRspPBReadEntryType;

/* PPBP read phonebook number structure */
typedef struct {
    ATRspCommonType           common;
    Uint16Type                pb_idx;
    Uint16Type                pn_idx;
    ATPhoneNumberTypeEnum     pn_type;
    ATPhoneNumberType         phone_number;
    Uint16Type                phone_number_len;
} ATRspPBReadNumberType;

/* DGPS get player status structure */
typedef struct {
    ATRspCommonType           common;
    ATDeviceTypeEnum          device_type;
    AT_DeviceId               device_id;
    ATDGPSStatusEnum          status;
    Uint32Type                pos; /* current position in ms */
    ATBooleanEnum             ext_info_available;
    /* following parameters are only set if ext_info_available == AT_TRUE */
    Uint32Type                total_len; /* total time in ms */
    ATDevicePlayerCodedEnum   codec;
    ATRepeatModeStatusEnum    repeat_mode;
    ATRandomModeStatusEnum    random_mode;
} ATRspGetPlayerStatusType;

/* data structure for dged get entry data response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATDeviceTypeEnum        device_type;
    AT_DeviceId             device_id;
    ATEntryFieldMaskEnum    field_id;
    ATBooleanEnum           data_value_avail;
    Uint16Type              data_value;
    ATMdiEntryData          data_string;
    Uint16Type              data_string_len; /* if no string included, len = 0!! */
    /* note: used string format with maximum supported length of SMS here!! */
} ATResPlayerGetEntDataType;


/* data structure for pbsc data response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_DeviceId             device_id;
    /* note - if query was for action 2 indication means supported
     * or not supported, for action 3 it means service stopped / started */
    ATSupportedEnum         res1;
    ATSupportedEnum         res2;
    ATSupportedEnum         res3;
    ATSupportedEnum         res4;
    ATSupportedEnum         res5;
    ATSupportedEnum         res6;
} ATResBTServiceConnType;

/* data structure for pkev data response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATRspKeyboardKeysEnum   key;
} ATResKeyboardKeyType;

/* data structure for pgmt data response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_DeviceId             device_id;
    ATMetadataTypeEnum      type;
} ATResGetMetadataType;

/* data structure for audio source activity response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType           common;
    ATAudioSourceActStateEnum state;
} ATResAudioSrcActType;

/* data structure for bluetooth service notification response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_DeviceId             device_id;
    ATBTServiceResultEnum   result;
    ATBTServiceEnum         service;
} ATResBTServiceNotType;

/* data structure for metadata update event */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_DeviceId             device_id;
    Uint8Type               fieldmask;
} ATResMetaDataUpdateType;

/* data structure for bluetooth support feature response event */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType                 common;
    AT_DeviceId                     device_id;
    ATDeviceTypeEnum                deviceType;
    ATBTFeatureEnum                 feature;
    ATBTFeatureSupportResultEnum    result;
} ATResBtFeatureSupportType;

/* data structure for auto connection mode response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATAutoConnStatusEnum    status;
} ATResBTAutoConnType;

/* data structure for voice recognition status response */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATBooleanEnum           end_set;
    /* if end set is AT_TRUE, all following parameters are not set!! */
    ATVRContextTypeEnum     context_type;
    ATVRStatusEnum          status;
    ATBooleanEnum           param1_avail;
    ATBooleanEnum           param2_avail;
    Uint8Type               param1; /* note - value ranges depends on type */
    Uint8Type               param2; /* note - value ranges depends on type */
} ATResVRStatusType;

/* data structure for BT discoverable */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATBTDiscoverModeEnum    mode;
} ATResBTDiscoverType;

/* data structure for maximum number of paired devices */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               max_num_paired_device;
}ATResMaxNumPairedDevType;

/* data structure for PSFN */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_BTDeviceName         name;
    Uint16Type              name_len;
} ATResBTNameType;

/* data structure for CGMM */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATCK5050ModelEnum       model;
} ATResCK5050ModelType;


/* data structure for CGMR */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_BTSWVersion          version;
    Uint16Type              version_len;
} ATResBTVersionType;

/* data structure for PPDE */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    AT_DeviceId             device_id;
} ATResDeletePairingType;

/* data structure for PPRS */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    struct{
        ATPairingDeviceType device_type;
        AT_DeviceId         device_id;
    } ext_device;
    ATPairingResultEnum     result;
    AT_DeviceId             device_id;
} ATResPairingResultType;

/* VOICE recognition control messages structures */

/* data structure for PPRS */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATSIVREngStatusEnum     status;
} ATResSIVRStatusType;

/* data structure for RRES */
typedef struct {
    ATSIVRResultOptEnum     type; /* equals AT_SIVR_RES_NUMBER */
    ATSIVRNumberText        number;
    Uint16Type              number_len;
} ATResSIVROptNumberType;

typedef struct {
    ATSIVRResultOptEnum   type; /* equals AT_SIVR_RES_IDs */
    ATBooleanEnum         id1_available;
    ATBooleanEnum         id2_available;
    Uint16Type            id1;
    Uint16Type            id2;
} ATResSIVROptIdType;


typedef union {
    ATResSIVROptNumberType  nb;
    ATResSIVROptIdType      id;
} ATResSIVROptUnionType;

typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATBooleanEnum           is_nbr;   /* if set AT_TRUE only nb_of_result filled*/
    Uint8Type               nb_of_result;
    /* all others filled if is_nbr == AT_FALSE */
    Uint8Type               result_index;
    Uint16Type              confidence_rate;
    ATSIVRPromptEnum        menu_id;
    ATSIVRResultEnum        result_type;
    ATSIVRCmdIdEnum         result_value;
    /* optional parameters, in general only 2 out of the 3 elements are available!!! */
    ATResSIVROptUnionType   opt;
} ATResSIVRResultType;

/* data structure for RGSL */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATBooleanEnum           is_lang_count;
    Uint8Type               lang_count; /* valid if is_lang_count == AT_TRUE */
    /* otherwise values below are filled and valid */
    Uint8Type               lang_index;
    ATSIVRLanguagesEnum     lang_id;
} ATResSIVRGetSupLangType;

/* data structure for RSCL */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATBooleanEnum           is_end;
    /* if is_end == AT_TRUE language change ended and other params not valid*/
    /* otherwise params below are set */
    Uint8Type               lang_index;
    ATSIVRLanguagesEnum     lang_id;
} ATResSIVRSetSupLangType;

/* data structure for RSRF */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               max_result;
    Uint8Type               rejection_level;
} ATResSIVRSetResFormatType;

/* data structure for PALGOSELECT */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATMicAlgoEnum           algotype;
} ATResPalgoSelectType;

/* data structure for PALGOSETMIC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               mic_id;
} ATResPalgoSetMicType;

/* data structure for +VGM */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               mic_gain;
} ATResMicGainType;

/* data structure for PALGOAEC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               maxAttAec;
    Uint8Type               switchTime;
} ATResPalgoAECType;

/* data structure for PNVS */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    /* note: if is_current == AT_TRUE the min and max values are both filled with current
     * volume values and the max values must be ignored. If variable set AT_FALSE,
     * min and max values are filled with volum level ranges for all types */
    ATBooleanEnum           is_current;
    Uint8Type               min_comm;
    Uint8Type               max_comm;
    Uint8Type               min_ring;
    Uint8Type               max_ring;
    Uint8Type               min_prompt;
    Uint8Type               max_prompt;
    Uint8Type               min_stream;
    Uint8Type               max_stream;
} ATResVolumeSetType;

/* data structure for +CGMREX */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATExtModelRev           extRev;
    Uint8Type               extRev_len;
} ATResRevExtType;

/* data structure for PPNC */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint16Type              total;
} ATResPbNumCountType;

/* data structure for PDAS */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               mode;
} ATResPBSyncModeType;

/* data structure for PPAC */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              count;
} ATResPBAlphaCountType;

/* data structure for PPNO */
typedef struct {
    ATRspCommonType         common;
    Uint8Type               mode;
} ATResPBSortModeType;

/* data structure for PPAD */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              pb_index;
} ATResPBAddEntryType;

/* data structure for PPAS */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              pb_index;
} ATResPBAlphaSearchType;

/* data structure for PPASEX */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              pb_index;
    Uint16Type              numMatchingContacts;
} ATResPBAlphaSearchExtType;



/* data structure for PSCM */
typedef struct {
    ATRspCommonType         common;
    ATBooleanEnum           auto_conn_active;
} ATResAutoConnType;


/* data structure for PRVP and PRVK */
typedef struct {
    ATRspCommonType         common;
    ATVTRecResultEnum       result;
} ATResRecordVTType;

/* data structure for PTVP and PTVK */
typedef struct {
    ATRspCommonType         common;
    ATBooleanEnum           voice_tag_exists;
} ATResVTTestPresenceType;

/* data structure for PTVP and PTVK */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              nb_voice_tags;
} ATResVTNumberTagsType;

/* data structure for PAVP */
typedef struct {
    ATRspCommonType         common;
    ATBooleanEnum           mode_enable;
} ATResVTAutoTagType;

/* data structure for PVTA */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              nb_voice_tag;
} ATResVTMgtType;


/* data structure for RPRE */
typedef struct {
    ATRspCommonType         common;
    ATSIVRPromptText        text;
    Uint16Type              text_len;
} ATResTrainingPrmtType;

/* data structure for PALGONOISECOMFORT */
typedef struct {
	ATRspCommonType       common;
	ATBooleanEnum         mode_activated;
} ATResPalgoNoiceComfType;

/* data structure for PALGOAECDELAY */
typedef struct {
	ATRspCommonType       common;
	Uint8Type             delay;
} ATResAECDelayType;

/* data structure for PALGOALCRX */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             mode;       /* ALC mode */
    Uint8Type             target;          /* give target value if mode activated AT_TRUE */
} ATResPalgoAlcRXType;

/* data structure for PALGOSLTX */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             mode;       /* ALC mode */
    Uint8Type             level;          /* give target value if mode activated AT_TRUE */
} ATResPalgoSLTXType;

/* data structure for PALGOSLRX */
typedef struct {
    ATRspCommonType       common;
    Uint8Type             level;          /* give target value if mode activated AT_TRUE */
} ATResPalgoSLRXType;

/* data structure for PBDI */
typedef struct {
    ATRspCommonType         common;
    ATBooleanEnum           is_end; /* indicate if inquiry was finished */
    AT_DeviceId             device_id;
    ATPairingDeviceType     device_type;
    AT_BTDeviceName         name;
    Uint16Type              name_len;
    AT_BT_Address           bt_addr;
    AT_BTClassOfDevice      bt_ClassOfDevice;
    ATBooleanEnum           hk_avail;
    ATHkProfileStatusEnum   hk_profile_status;
} ATRspInquiryType;


/* data structure for PGIR */
typedef struct {
    ATRspCommonType         common;
    ATBooleanEnum           is_count_only;
    Uint8Type               count;
    AT_DeviceId             device_id;
    ATPairingDeviceType     device_type;
    AT_BTDeviceName         name;
    Uint16Type              name_len;
    AT_BT_Address           bt_addr;
    AT_BTClassOfDevice      bt_ClassOfDevice;
    ATBooleanEnum           bitfileds_avail;
    Uint32Type              statusBitfield;
    Uint32Type              resultBitfield;
} ATRspGetInquiryResType;


/* data structure for PPRQ */
typedef struct {
    ATRspCommonType         common;
    AT_DeviceId             remote_id;
    ATPairingDeviceType     device_type;
    AT_BTDeviceName         name;
    Uint16Type              name_len;
    ATBooleanEnum           addr_avail;
    AT_BT_Address           bt_addr;
    ATBooleanEnum           cod_avail;
    AT_BTClassOfDevice      bt_ClassOfDevice;
} ATRspInPairRequestType;

/* data structure for PBSM */
typedef struct {
    ATRspCommonType         common;
    ATBooleanEnum           aHFPActivated;
} ATRspBTServiceModeType;

/* data structure for PPDS */
typedef struct {
    ATRspCommonType         common;
    Uint16Type              dl_status;
} ATRspPbDwnlStatusType;

typedef struct {
    ATRspCommonType         common;
    ATBTServiceEnum         service;
    ATBTAutoConnMode        mode;
    Uint8Type               bitmask;
}ATRspBTServAutoConnModeType;


typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATExtCustomer           customer;
    Uint16Type              customer_len;
    ATExtProject            project;
    Uint16Type              project_len;
    ATExtVersionNum         versionNum;
    Uint16Type              versionNum_len;
    ATExtLanguage           language;
    Uint16Type              language_len;
    ATExtLanguageVersion    languageVersion;
    Uint16Type              languageVersion_len;
}ATRspRstuexType;

typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATPbcvHwVersion         hw_version;
    Uint16Type              hw_version_len;
    ATPbcvSwVersion         sw_version;
    Uint16Type              sw_version_len;
    ATPbcvManufacturer      manufacturer;
    Uint16Type              manufacturer_len;
}ATRspPbcvType;

typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    ATIMSI                  imsi;
    Uint16Type              imsi_len;
}ATRspPssnType;

typedef struct {
    ATRspCommonType         common;
    Uint8Type               update_event;
    AT_Md5KeyType           Md5Key;
    AT_Md5KeyEncrytedType   Md5KeyEncryted;
}ATRspBTSWupdateType;

/* data structure for PPUS */
typedef struct {
    /* the common is required for all structures!!!  */
    ATRspCommonType         common;
    Uint8Type               PhonebookUpdateInProgress;
} ATRspPhonebookUpdateStatusType;


typedef struct {
    ATRspCommonType         common;
    Uint8Type               CurConnectMode;
}ATRspConAuthType;


/* PMFL response */
typedef struct {
    ATRspCommonType           common;
    ATBooleanEnum             free_space_indicator;
} ATRspFlashFullEventType;

/* data structure for PSSP */
typedef struct {
    ATRspCommonType                 common;
    ATSSPModeEnum                   mode;
    ATSSPIoCapParameterEnum         IOCap;
    ATSSPModeEnum                   nextMode;
    ATSSPIoCapParameterEnum         nextIOCap;
}ATRspSSPSetupType;

/* data structure for PSPR */
typedef struct {
    ATRspCommonType             common;
    ATPairingDeviceType         deviceType;
    AT_DeviceId                 deviceId;
    ATSSPModeEnum               mode;
    ATSSPIoCapParameterEnum     capability;
    ATBooleanEnum               passAvail;
    ATSSPPinCodeBT              passphrase;
}ATRspSSPRequestType;

/* data structure UNION                                                       */
typedef union {
    /* the common is required for mandatory response type */
    ATRspCommonType                 common;
    ATResRevExtType                 cgmrex;
    ATRspBTAntennaType              pbta;
    ATRspPbAddressType              address;
    ATRspCallStatCountType          pncc;
    ATRspCallStatValType            plcc;
    ATRspPhoneStatusIndEventType    piev;
    ATRspClassOfDeviceRequestType   pbcd;
    ATRspPhoneStatusIndSupportType  pind_sup;
    ATRspPhoneStatusIndValueType    pind_val;
    ATRspWarningType                pwng;
    ATRspSelectBTDeviceType         psbd;
    ATRspPlpdNbType                 plpd_nb;
    ATRspPlpdInfoType               plpd_info;
    ATRspPpbcType                   ppbc;
    ATRspPpbuType                   ppbu;
    ATRspPecaType                   peca;
    ATRspSdpNotType                 psdp;
    ATRspAudioTransferType          patr;
    ATRspAudioGatewayType           pagm;
    ATRspAudioGatewayTransferType   pagt;
    ATRspSetRingtoneType            pstr;
    ATRspMicroMuteType              pmmu;
    ATRspSelectPBType               ppbs;
    ATRspPBCallHistType             ppch;
    ATRspSmsSupNotType              pmst;
    ATRspSmsReceiveNumType          pmrd_num;
    ATRspSmsReceiveMsgType          pmrd_msg;
    ATRspSmsConcReceiveType         pmsdex;
    ATRspPBReadEntryType            ppbr;
    ATRspPBReadNumberType           ppbp;
    ATResPbNumCountType             ppnc;
    ATResPBSyncModeType             pdas;
    ATResPBAlphaCountType           ppac;
    ATResPBSortModeType             ppno;
    ATResPBAddEntryType             ppad;
    ATRspGetPlayerStatusType        dgps;
    ATResPlayerGetEntDataType       dged;
    ATResBTServiceConnType          pbsc;
    ATResKeyboardKeyType            pkev;
    ATResGetMetadataType            pgmt;
    ATResAudioSrcActType            pasa;
    ATResBTServiceNotType           pbsn;
    ATResBTAutoConnType             pcms;
    ATResVRStatusType               rvrs;
    ATResBTDiscoverType             psdm;
    ATResMaxNumPairedDevType        psmd;
    ATResBTNameType                 psfn;
    ATResCK5050ModelType            cgmm;
    ATResBTVersionType              cgmr;
    ATResDeletePairingType          ppde;
    ATResPairingResultType          pprs;
    ATResSIVRStatusType             rsts;
    ATResSIVRResultType             rres;
    ATResSIVRGetSupLangType         rgsl;
    ATResSIVRSetSupLangType         rscl;
    ATResSIVRSetResFormatType       rsrf;
    ATResPalgoSelectType            palgoselect;
    ATResPalgoSetMicType            palgosetmic;
    ATResPalgoNoiceComfType         palgonoisecomfort;
    ATResAECDelayType               palgoaecdelay;
    ATResPalgoAlcRXType             palgoalcrx;
    ATResPalgoSLRXType              palgoslrx;
    ATResPalgoSLTXType              palgosltx;
    ATResMicGainType                vgm;
    ATResPalgoAECType               palgoaec;
    ATResVolumeSetType              pnvs;
    ATResPBAlphaSearchType          ppas;
    ATResPBAlphaSearchExtType       ppasex;
    ATResAutoConnType               pscm;
    ATResRecordVTType               prvp;
    ATResRecordVTType               prvk;
    ATResVTTestPresenceType         ptvp;
    ATResVTTestPresenceType         ptvk;
    ATResVTNumberTagsType           pnvp;
    ATResVTAutoTagType              pavp;
    ATResVTMgtType                  rvta;
    ATResTrainingPrmtType           rpre;
    ATRspInquiryType                pbdi;
    ATRspGetInquiryResType          pgir;
    ATRspInPairRequestType          pprq;
    ATRspBTServiceModeType          pbsm;
    ATRspPbDwnlStatusType           ppds;
    ATRspBTServAutoConnModeType     pbsa;
    ATRspRstuexType                 rstuex;
    ATRspBTSWupdateType             pbsu;
    ATRspConAuthType                psca;
    ATRspPhonebookUpdateStatusType  ppus;
    ATRspPbcvType                   pbcv;
    ATRspPssnType                   pssn;
    ATRspFlashFullEventType         pmfl;
    ATRspSSPRequestType             pspr;
    ATRspSSPSetupType               pssp;
    ATResMetaDataUpdateType         pamu;
    ATResBtFeatureSupportType       pbsf;
    /* add all other data types below                                         */
} ATRspAllType;


typedef struct {
    ATRspCommonType         common;
    ATRspDataIndicationEnum dataInd;
    /* NOTE: common part within data is duplicate to common above to allow
     * easy access for the application */
    ATRspAllType            *data;
} ATRspMessageType;

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
void             ATParserTask(void);

#endif /* _ATPARSER_H */

/* End Of File ATParser.h */
