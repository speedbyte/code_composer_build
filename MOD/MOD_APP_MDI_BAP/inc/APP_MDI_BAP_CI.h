/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_MDI_BAP_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_MDI_BAP
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-02-03 11:12:29 +0100 (Do, 03 Feb 2011) $
* $Rev: 17644 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI_BAP/trunk/inc/APP_MDI_BAP_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_MDI_BAP_CI_H
#define _APP_MDI_BAP_CI_H

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
//#include "APP_COMMON_Utils.h" 

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

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

#define APP_MDI_BAP_CURRENT_FILE_NAME_MAX 150
#define APP_MDI_BAP_CURRENT_FILE_INFO_MAX 90

#define BAP_MD_NAME_MAX_LEN             30

#define BAP_MD_ACTIVE_MD_TYPE_DB_LOGIC  0x80
#define BAP_MD_ACTIVE_MD_TYPE_MS_LOGIC  0x40
#define BAP_MD_ACTIVE_MD_TYPE_AUX       0x20

#define BAP_MD_TRACK_TIME_UNAVAILABLE           0xFFFFU

/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/

typedef enum
{
    APP_MDI_BAP_enResultOk      = 0x00,
    APP_MDI_BAP_enResultNotOk   = 0x01
}APP_MDI_BAP_enBapResult;


typedef enum
{
    MDI_BAP_INVALID_FCT_ID      = 0,
    MDI_BAP_MEDIA_EVENT         = BapFct_MDI_MediaEvent,
    MDI_BAP_PLAY_STATUS         = BapFct_MDI_PlayStatus,
    MDI_BAP_SKIP_FORWARD        = BapFct_MDI_SkipForward,
    MDI_BAP_SKIP_BACKWARD       = BapFct_MDI_SkipBackward,
//    MDI_BAP_MD_LIST             = BapFct_MDI_MDlist,
//    MDI_BAP_MD_ACTIVATE         = BapFct_MDI_MDactivate,
    MDI_BAP_MD_ACTIVE           = BapFct_MDI_MDactive,

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
    MDI_BAP_TIME_POSITION       = BapFct_MDI_TimePosition,
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
    MDI_BAP_RANDOM              = BapFct_MDI_Random,
    MDI_BAP_REPEAT              = BapFct_MDI_Repeat,
#endif
    MDI_BAP_CURRENT_FILE_NAME   = BapFct_MDI_CurrentFileName,
    MDI_BAP_CURRENT_FILE_INFO   = BapFct_MDI_CurrentFileInfo
} APP_MDI_BAP_enFunctionIdType;

typedef enum
{
    MDI_BAP_OP_TYPE_INVALID = 0,
    MDI_BAP_OP_TYPE_GET,
    MDI_BAP_OP_TYPE_SET,
    MDI_BAP_OP_TYPE_SET_GET,
    MDI_BAP_OP_TYPE_STATUS,
    MDI_BAP_OP_TYPE_START_RESULT,
    MDI_BAP_OP_TYPE_ABORT
} APP_MDI_BAP_enOpType;

typedef enum
{
    APP_MDI_BAP_enNotActive,
    APP_MDI_BAP_enActive
}APP_MDI_BAP_enActivationState;
        
typedef enum
{
    APP_MDI_enNoMediaDeviceActive = 0x00,
    APP_MDI_enInitializing,
    APP_MDI_enMdNotFunctional,
    APP_MDI_enMdNotSupported,
    APP_MDI_en_NoPlayableFiles,
    APP_MDI_enReadyToOperate
}APP_MDI_enOpState;

typedef enum
{
    MDI_BAP_AUX_IN_OFF = 0,
    MDI_BAP_AUX_IN_ON
} APP_MDI_BAP_enAuxInOnOff;

typedef enum
{
    MDI_BAP_RANDOM_OFF = 0,
    MDI_BAP_RANDOM_ALL,
    MDI_BAP_RANDOM_FOLDER_PLAYLISTS,
    MDI_BAP_RANDOM_ALL_PLAYLISTS,
    /* 0x04 .. 0xFE not supported */
    MDI_BAP_RANDOM_NOT_SUPPORTED = 0xFF
}APP_MDI_BAP_enRandomMode;

typedef enum
{
    MDI_BAP_REPEAT_OFF = 0,
    MDI_BAP_REPEAT_ALL_MEDIUM,
    MDI_BAP_REPEAT_FOLDER_PLAYLIST,
    MDI_BAP_REPEAT_ALL_PLAYLISTS,
    MDI_BAP_REPEAT_FILE_TRACK,
    /* 0x05 .. 0x0F reserved */
    MDI_BAP_REPEAT_TITLE = 0x10,
    MDI_BAP_REPEAT_CHAPTER,
    MDI_BAP_REPEAT_AB,
    /* 0x13 .. 0xFE reserved */
    MDI_BAP_REPEAT_NOT_SUPPORTED
}APP_MDI_BAP_enRepeatMode;

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/




void _APP_MDI_BAP_vOnPlayStatusCb(APP_COMMON_enResponseCode result);
void _APP_MDI_BAP_vOnSkipForwardCb(APP_COMMON_enResponseCode result);
void _APP_MDI_BAP_vOnSkipBackwardCb(APP_COMMON_enResponseCode result);
void _APP_MDI_BAP_vOnMdActivateCb(APP_COMMON_enResponseCode result);
void _APP_MDI_BAP_vOnRandomModeCb(APP_COMMON_enResponseCode result);
void _APP_MDI_BAP_vOnRepeatModeCb(APP_COMMON_enResponseCode result);

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
//#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/




/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

typedef struct
{
    APP_MDI_BAP_enFunctionIdType    fctType;
    APP_MDI_BAP_enOpType            opType;
} MdiBapIndCommonType;


typedef struct
{
    MdiBapIndCommonType             common; 
    APP_MDI_BAP_enPlayStatusValue   value;
}MdiBapPlayStatus;

typedef struct
{
    MdiBapIndCommonType     common; 
    APP_MDI_BAP_enSkipMode  mode;
    Uint8Type               counter;
}MdiBapSkip;

typedef struct
{
    MdiBapIndCommonType      common; 
    APP_MDI_BAP_enAuxInOnOff value;
}MdiBapAuxInOnOffType;

typedef struct
{
    MdiBapIndCommonType     common; 
    Sint8Type               value;
}MdiBapAuxInOffsetType;

typedef struct
{
    MdiBapIndCommonType     common; 
    Uint16Type              trackTime;
    Uint16Type              trackTimePosition;
}MdiBapTimePostionType;

typedef struct
{
    MdiBapIndCommonType         common; 
    APP_MDI_BAP_enRandomMode    mode;
}MdiBapRandomType;

typedef struct
{
    MdiBapIndCommonType         common; 
    APP_MDI_BAP_enRepeatMode    mode;
}MdiBapRepeatType;

typedef union
{
    MdiBapIndCommonType     common;
    MdiBapPlayStatus        playStatus;
    MdiBapSkip              skipForward;
    MdiBapSkip              skipBackward;
    MdiBapIndCommonType     mdActive;
    MdiBapAuxInOnOffType    auxInOnOff;
    MdiBapAuxInOffsetType   auxInOffset;
    MdiBapTimePostionType   timePositon;
    MdiBapRandomType        random;
    MdiBapRepeatType        repeat;
}MdiBapIndType;


typedef struct
{
    Uint8Type       retryCounter;
    MdiBapIndType   mdiInd;
}AppMdiBapContext;


typedef struct
{
                /* Byte 1 */
/* Bit 7 */     Uint8Type :1;
/* Bit 6 */     Uint8Type :1;
/* Bit 5 */     Uint8Type :1;
/* Bit 4 */     Uint8Type Heartbeat:1;
/* Bit 3 */     Uint8Type FunctionList:1;
/* Bit 2 */     Uint8Type BapConfig:1;
/* Bit 1 */     Uint8Type GetAll:1;
/* Bit 0 */     Uint8Type :1;
                /* Byte 2 */
 /* Bit 15 */   Uint8Type FsgOpState:1;
 /* Bit 14 */   Uint8Type FsgSetup:1;
 /* Bit 13 */   Uint8Type :1;
 /* Bit 12 */   Uint8Type :1;
 /* Bit 11 */   Uint8Type :1;
 /* Bit 10 */   Uint8Type :1;
 /* Bit 9  */   Uint8Type :1;
 /* Bit 8  */   Uint8Type :1;
                /* Byte 3 */
 /* Bit 23 */   Uint8Type TimePosition:1;
 /* Bit 22 */   Uint8Type MediaEvent:1;
 /* Bit 21 */   Uint8Type PlayMode:1;
 /* Bit 20 */   Uint8Type PlayStatus:1;
 /* Bit 19 */   Uint8Type MDactivate:1;
 /* Bit 18 */   Uint8Type MDlist:1;
 /* Bit 17 */   Uint8Type MDactive:1;
 /* Bit 16 */   Uint8Type MdServiceSupport:1;
                /* Byte 4 */
 /* Bit 31 */   Uint8Type CurrentFileInfo:1;
 /* Bit 30 */   Uint8Type CurrentFileName:1;
 /* Bit 29 */   Uint8Type Repeat:1;
 /* Bit 28 */   Uint8Type Scan:1;
 /* Bit 27 */   Uint8Type Random:1;
 /* Bit 26 */   Uint8Type SkipBackward:1;
 /* Bit 25 */   Uint8Type SkipForward:1;
 /* Bit 24 */   Uint8Type TrackPosition:1;
                /* Byte 5 */
 /* Bit 39 */   Uint8Type FClistInfo:1;
 /* Bit 38 */   Uint8Type AuxInOffset:1;
 /* Bit 37 */   Uint8Type AuxInOff:1;
 /* Bit 36 */   Uint8Type FCcache:1;
 /* Bit 35 */   Uint8Type FCcontrol:1;
 /* Bit 34 */   Uint8Type FCpath:1;
 /* Bit 33 */   Uint8Type FClist:1;
 /* Bit 32 */   Uint8Type FileChooser:1;
                /* Byte 6 */
 /* Bit 47 */   Uint8Type MDidentity:1;
 /* Bit 46 */   Uint8Type FCfileView:1;
 /* Bit 45 */   Uint8Type FCfileNameMaxSize:1;
 /* Bit 44 */   Uint8Type FCpostion:1;
 /* Bit 43 */   Uint8Type MDlist2:1;
 /* Bit 42 */   Uint8Type MDactive2:1;
 /* Bit 41 */   Uint8Type FileInfo:1;
 /* Bit 40 */   Uint8Type CurrentFileInfo_advanced:1;
                /* Byte 7 */
 /* Bit 55 */   Uint8Type :1;
 /* Bit 54 */   Uint8Type :1;
 /* Bit 53 */   Uint8Type :1;
 /* Bit 52 */   Uint8Type :1;
 /* Bit 51 */   Uint8Type :1;
 /* Bit 50 */   Uint8Type :1;
 /* Bit 49 */   Uint8Type :1;
 /* Bit 48 */   Uint8Type :1;
                /* Byte 8 */
 /* Bit 63 */   Uint8Type :1;
 /* Bit 62 */   Uint8Type :1;
 /* Bit 61 */   Uint8Type :1;
 /* Bit 60 */   Uint8Type :1;
 /* Bit 59 */   Uint8Type :1;
 /* Bit 58 */   Uint8Type :1;
 /* Bit 57 */   Uint8Type :1;
 /* Bit 56 */   Uint8Type :1;
}MdServiceSupport;




//typedef struct
//{
//    MdiBapIndCommonType     common; 
//    BapArrayHeaderType      arrayHeader;
//}MdiBapMdListType;
//
//typedef struct
//{
//    MdiBapIndCommonType     common; 
//    AT_DeviceId             posMdList;
//}MdiBapMdActivateType;

#endif /* _APP_MDI_BAP_CI_H */

/* End Of File APP_MDI_BAP_CI.h */
