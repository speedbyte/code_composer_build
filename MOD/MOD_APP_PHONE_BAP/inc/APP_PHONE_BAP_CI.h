/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_PHONE_BAP_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE_BAP
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-07-04 15:34:00 +0200 (Mo, 04 Jul 2011) $
* $Rev: 20591 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/inc/APP_PHONE_BAP_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_BAP_CI_H
#define _APP_PHONE_BAP_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "bap_defines.h"
#include "app_phone.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h" 


/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define ITEMPLATE 100 /* CONFIGURATE EXPORT INTERFACE(ITEMPLATE) */

#ifndef MODULE_TEST
#define MODULE_TEST
#endif 

#define MAX_PLPD_ELEMENTS 5
#define MAX_DTMF_CHARS 39 
#define BAP_SIZE_NETWORK_PROVIDER 40U

#define CKCM_ERROR_CODE_DIALING_OR_WAITING_CALL     252
#define CKCM_ERROR_CODE_TOO_MANY_ONGOING_CALLS      255

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
#define ITMPL_Func IUART_Func
/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/
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
 /* Bit 23 */   Uint8Type SignalQuality:1;
 /* Bit 22 */   Uint8Type NetworkProvider:1;
 /* Bit 21 */   Uint8Type PhoneEnable:1;
 /* Bit 20 */   Uint8Type LockState:1;
 /* Bit 19 */   Uint8Type RegisterState:1;
 /* Bit 18 */   Uint8Type ActiveUser:1;
 /* Bit 17 */   Uint8Type UserList:1;
 /* Bit 16 */   Uint8Type MobileServiceSupport:1;
                /* Byte 4 */
 /* Bit 31 */   Uint8Type CallHold:1;
 /* Bit 30 */   Uint8Type AcceptCall:1;
 /* Bit 29 */   Uint8Type HangupCall:1;
 /* Bit 28 */   Uint8Type DialNumber:1;
 /* Bit 27 */   Uint8Type CallInfo:1;
 /* Bit 26 */   Uint8Type CallState:1;
 /* Bit 25 */   Uint8Type LineState:1;
 /* Bit 24 */   Uint8Type NetInfo:1;
                /* Byte 5 */
 /* Bit 39 */   Uint8Type MPSwap:1;
 /* Bit 38 */   Uint8Type MPReleaseActiveCallAcceptWaitingCall:1;
 /* Bit 37 */   Uint8Type MPReleaseActiveCallAcceptHeldCall:1;
 /* Bit 36 */   Uint8Type SendDTMF:1;
 /* Bit 35 */   Uint8Type RingToneMuteOnOff:1;
 /* Bit 34 */   Uint8Type MicroMuteOnOff:1;
 /* Bit 33 */   Uint8Type HandsFreeOnOff:1;
 /* Bit 32 */   Uint8Type ResumeCall:1;
                /* Byte 6 */
 /* Bit 47 */   Uint8Type SupplementaryServices:1;
 /* Bit 46 */   Uint8Type PhoneOnOff:1;
 /* Bit 45 */   Uint8Type Keypad:1;
 /* Bit 44 */   Uint8Type CCSplit:1;
 /* Bit 43 */   Uint8Type CCJoin:1;
 /* Bit 42 */   Uint8Type MPExplicitCallTransfer:1;
 /* Bit 41 */   Uint8Type MPReleaseAllCallsAcceptWaitingCall:1;
 /* Bit 40 */   Uint8Type MPCallHoldAcceptWaitingCall:1;
                /* Byte 7 */
 /* Bit 55 */   Uint8Type :1;
 /* Bit 54 */   Uint8Type :1;
 /* Bit 53 */   Uint8Type :1;
 /* Bit 52 */   Uint8Type :1;
 /* Bit 51 */   Uint8Type :1;
 /* Bit 50 */   Uint8Type :1;
 /* Bit 49 */   Uint8Type :1;
 /* Bit 48 */   Uint8Type RestoreFactorySettings:1;
                /* Byte 8 */
 /* Bit 63 */   Uint8Type :1;
 /* Bit 62 */   Uint8Type :1;
 /* Bit 61 */   Uint8Type :1;
 /* Bit 60 */   Uint8Type :1;
 /* Bit 59 */   Uint8Type :1;
 /* Bit 58 */   Uint8Type :1;
 /* Bit 57 */   Uint8Type :1;
 /* Bit 56 */   Uint8Type :1;
}PhoneMobileServiceSupport;

typedef enum
{
    PHONE_BAP_INVALID_FCT_ID    = 0x00
}APP_PHONE_BAP_enFunctionIdType;


/* module internal struct */
typedef struct
{
        Uint8Type unusedBits    : 7;
        Uint8Type muteBit       : 1;
} MicroMuteType;

typedef struct
{
        Uint8Type unusedBits    : 7;
        Uint8Type hfBit         : 1;
} HandsFreeType;


typedef struct 
{
    Uint8Type count;
    Uint8Type dtmfChars[MAX_DTMF_CHARS + 1];    // one for \0
}DtmfContext;

typedef struct
{
    Uint8Type lastAtServiceValue;
    Uint8Type lastAtRoamingValue;
}RegisterStateContext;


typedef struct
{
    APP_PHONE_BAP_enFunctionIdType  fctId;
    BapIndication                   bapInd;
}AppPhoneCommon;

typedef enum
{
    CK5050_RESULT_RECEIVED = 0,
    BAP_LINESTATE_SENT,
    BAP_CALLSTATE_SENT,
    BAP_CALLINFO_SENT,
    BAP_RESULT_ERROR
} BAP_enResultProcessing;

typedef struct
{
    BooleanType CK5050result;
    BooleanType linestate;
    BooleanType callinfo;
    BooleanType callstate;
}
CallResultState;

typedef struct
{   
    AT_DeviceId             activeDeviceId;
    Uint8Type               bToggleMicroMutePending; 
    BooleanType             bUpdateActiveUserPending;
    BooleanType             bDialNumberPending;
    BooleanType             bResumeCallPending;
    BooleanType             bAcceptCallPending;
    BooleanType             bHangUpCallPending;
    BooleanType             bMPCallHoldAcceptWaitingCallPending;
    BooleanType             bMPSwapPending;
    BooleanType             bCallHoldPending;
    BooleanType             bCCJoinPending;
    BooleanType             bMPReleaseActiveCallAcceptWaitingCallPending;
    BooleanType             CallStateChangedPending;
    BooleanType             CallInfoPending;
    CallResultState         DialNumberStates;
    CallResultState         ResumeCallStates;
    CallResultState         AcceptCallStates;
    CallResultState         HangUpCallStates;
    CallResultState         MPCallHoldAcceptWaitingCallStates;
    CallResultState         MPSwapStates;
    CallResultState         CallHoldStates;
    CallResultState         CCJoinStates;
    CallResultState         MPReleaseActiveCallAcceptWaitingCallStates;
    ATCallHangupEnum        bHangUpCallIndex;
    ATAudioTransferModeEnum HandsFreeModeAlreadySent;
    APP_PHONE_enNotificationId      notification;

//    HandsFreeMode           handsFreeMode;
    RegisterStateContext    registerState;
//    union
//    {
//        AppCommon          common;
//        AppCommonArray     commonArray;
//        // add struct for each fct which can be pending and having
//        // more than one state
//    }fctCtx;
    AppPhoneCommon         common;
    BooleanType            bUserIsBeingActivated;
	BooleanType			   bSignalQualityAvailableAfterConnect;
}AppPhoneBapContext;



/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief  sends a BAP Error message with the given BAP fctId and errorCode	
*/


/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
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
//static BapExtArrayHeaderInfo callInfoRequest;

/** 
\brief	variable to save the current call management function currently in progress
*/
//extern AppPhoneContext gAppPhoneContext;




/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the #define
*/
//#define	EXAMPLE_DEFINE		1


#endif /* _APP_PHONE_BAP_CI_H */

/* End Of File APP_PHONE_BAP_CI.h */
