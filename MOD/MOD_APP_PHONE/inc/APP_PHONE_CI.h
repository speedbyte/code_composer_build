/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_PHONE_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-08-30 16:53:44 +0200 (Di, 30 Aug 2011) $
* $Rev: 21443 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_CI_H
#define _APP_PHONE_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONE.h"
#include "APP_COMMON_Utils.h"
#include "ATCmdHandler.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define ITEMPLATE 100 /* CONFIGURATE EXPORT INTERFACE(ITEMPLATE) */

#ifndef MODULE_TEST
#define MODULE_TEST
#endif

#define MAX_PLPD_ELEMENTS 5
#define APP_PHONE_PATR_TIMEOUT 2000            
#define APP_PHONE_MAX_CALL_STACK_SIZE 20

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

typedef enum
{
    APP_PHONE_INVALID_FCT = 0,
    APP_PHONE_DIAL_NUMBER,
    APP_PHONE_DIAL_INDEX,
    APP_PHONE_REDIAL,
    APP_PHONE_DIAL_NUMBER_TYPE,
    APP_PHONE_ACCEPT_CALL,
    APP_PHONE_HANGUP_CALL,
    APP_PHONE_HANGUP_ALL_CALL,
    APP_PHONE_PHONE_STATUS,
    APP_PHONE_CALL_STATE,
    APP_PHONE_CALL_INFO,
    APP_PHONE_SEND_DTMF,
    APP_PHONE_PLPD_REQUEST,
    APP_PHONE_CALL_HOLD,
    APP_PHONE_CALL_REPLACE,
    APP_PHONE_RESUME_CALL,
    APP_PHONE_HANDS_FREE_ON_OFF,
    APP_PHONE_MICRO_MUTE_ON_OFF,
    APP_PHONE_SWITCH_CALLS,
    APP_PHONE_JOIN_CONFERENCE,
    APP_PHONE_SPLIT_CONFERENCE,
//    APP_PHONE_RELEASE_ACTIVE_ACCEPT_WAITING_CALL,
    APP_PHONE_PUT_CALLS_THROUGH,
    APP_PHONE_REQUEST_HFP_VERSION,
    APP_PHONE_SILENT_RINGING,
    APP_PHONE_HEADSET_ON_OFF
}APP_PHONE_enFctId;

typedef enum
{
    APP_PHONE_COMMAND_NOT_SENT = 0,
    APP_PHONE_COMMAND_SENT,
    APP_PHONE_NEXT_COMMAND_NOT_SENT
}APP_PHONE_enSingleCommandState;

typedef enum
{
    APP_PHONE_GET_MUTE_NOT_SENT = 0,
    APP_PHONE_GET_MUTE_SENT,
    APP_PHONE_DIAL_COMMAND_NOT_SENT,
    APP_PHONE_DIAL_COMMAND_SENT
}APP_PHONE_enDoubleCommandState;



typedef enum
{
    APP_PHONE_ACCEPT_NOT_SENT = 0,
    APP_PHONE_ACCEPT_SENT,
    APP_PHONE_RELEASE_SENT
}APP_PHONE_enRelActAccWaitCallState;


///* data structure UNION                                                       */
//typedef union {
//    /* the common is required for mandatory req type */
//    ATRspCommonType                 common;
//    ATRspCallStatCountType          pncc;
//    ATRspCallStatValType            plcc;
//    ATRspPhoneStatusIndEventType    piev;
//    ATRspPhoneStatusIndSupportType  pind_sup;
//    ATRspPhoneStatusIndValueType    pind_val;
//    ATRspAudioTransferType          patr;
//    ATRspSetRingtoneType            pstr;
//    ATRspMicroMuteType              pmmu;
//    /* add all other data types below                                         */
//} ATRspPhoneAllType;
//
//typedef struct
//{
//    ATRspCommonType                 common;
//    ATRspDataIndicationEnum         dataInd;
//    /* NOTE: common part within data is duplicate to common above to allow
//     * easy access for the application */
//    ATRspPhoneAllType               *data;
//}ATRspPhoneMessageType;
typedef Uint8Type DtmfCharsType[40];

typedef struct
{
    APP_PHONE_enFctId fctId;
    void (*callback)(APP_COMMON_enResponseCode);
}AppPhoneCommonType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enDoubleCommandState      state;
    ATPhoneNumberType                   number;
    Uint8Type                           number_len;
}AppPhoneDialType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enDoubleCommandState      state;
    Uint16Type                          pbIndex;
    Uint16Type                          pnIndex;
}AppPhoneDialIndexType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enDoubleCommandState      state;
    Uint16Type                          pbIndex;
    ATPhoneNumberTypeEnum               type;
}AppPhoneDialNumberType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
    ATCallHangupEnum                    callIndex;
}AppPhoneHangUpCallType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
}AppPhoneSingleCommandStateType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
    DtmfCharsType                       value;
    Uint16Type                          length;
    Uint16Type                          sendIdx;
}AppPhoneDtmfType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
    ATAudioTransferModeEnum             mode;
}AppPhoneHandsFreeOnOffType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
    ATAudioGatewayModeEnum      mode;
}AppPhoneHeadsetOnOffType;


typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
    ATMicModeEnum                       mode;
}AppPhoneMicroMuteOnOffType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enSingleCommandState      state;
    ATCallIndexEnum                     callIndex;
}AppPhoneSplitConferenceType;

typedef struct
{
    AppPhoneCommonType                  common;
    APP_PHONE_enRelActAccWaitCallState  state;
    ATCallHangupEnum                    call;

}AppPhoneRelActAccWaitCallType;


typedef struct
{
    BooleanType     toggleMicMute;
    BooleanType     requestHfpVersion;
    BooleanType     resetHandsfreeToOn;
    BooleanType     resetHandsfreeToOff;
    BooleanType     unmuteMicro;
    BooleanType     resetHeadsetToOff;		// RT 2276
    BooleanType     indicateOutgCallError;
    BooleanType     patrTimeout;
}InternalFctPending;

/* module internal struct */
typedef struct
{
    //ATRequestEnum                   lastATRequest;
    // some values are needed just as long as the calling app has requested the
    // data
    //ATRspMessageType * lastATRsp;
    //ATRspPhoneMessageType * lastATRsp;

    // some values have to be available at any time
    //ATRspCallStatCountType          pncc;
    //ATRspCallStatValType            plcc;
    AT_IndicatorValue               piev_signalStrength;
    AT_IndicatorValue               piev_Roaming;
    AT_IndicatorValue               piev_serviceSupport;
    AT_IndicatorValue               piev_batteryLevel;
    AT_NetworkProvider              piev_networkProvider;
    BooleanType                     isMicMuted;
    BooleanType                     isHandsFreeOff;
    BooleanType                     isHeadsetOn;
    BooleanType                     isHangUpSent;
    BooleanType                     isCDMAPhone;
    ATCoDProfileEnum                hfpVersion;

    BooleanType                     bWaitForOutgCallIndication;
    BooleanType                     bRestartPlayerWhenIdleAgain;

    //Uint16Type                      u16MissedCallsDuringKl15Off;
    Uint32Type                      u32MissedCallerIdHashTable[APP_PHONE_MAX_CALL_STACK_SIZE];
    Uint16Type                      u16MissedCalls;
    Uint16Type                      u16MissedNumbers;

    InternalFctPending              pending;

    union
    {
        AppPhoneCommonType              common;
        AppPhoneDialType                dial;
        AppPhoneDialIndexType           dialIndex;
        AppPhoneDialType                redial;
        AppPhoneDialNumberType          dialNumberType;
        AppPhoneSingleCommandStateType  acceptCall;
        AppPhoneHangUpCallType          hangUpCall;
        AppPhoneSingleCommandStateType  phoneStatus;
        AppPhoneSingleCommandStateType  callState;
        AppPhoneSingleCommandStateType  callInfo;
        AppPhoneDtmfType                dtmf;
        AppPhoneSingleCommandStateType  plpdReq;
        AppPhoneSingleCommandStateType  callHold;
        AppPhoneSingleCommandStateType  callReplace;
        AppPhoneSingleCommandStateType  resumeCall;
        AppPhoneHandsFreeOnOffType      handsFreeOnOff;
        AppPhoneHeadsetOnOffType        headsetOnOff;
        AppPhoneMicroMuteOnOffType      microMuteOnOff;
        AppPhoneSingleCommandStateType  switchCalls;
        AppPhoneSingleCommandStateType  joinConference;
        AppPhoneSplitConferenceType     splitConference;
//        AppPhoneRelActAccWaitCallType   relActAccWaitCall;
        AppPhoneSingleCommandStateType  putCallsThrough;
        AppPhoneSingleCommandStateType  requestHfpVersion;

    }fct;

}AppPhoneContext;



/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/**
\brief  sends a BAP Error message with the given BAP fctId and errorCode
*/
void _APP_PHONE_vNotifyApp(APP_PHONE_enNotificationId notify);
void _APP_PHONE_vSendResultReleaseContext(APP_COMMON_enResponseCode result);
void _APP_PHONE_vReleaseFctContext();
void _APP_PHONE_vReInitApp(void);
void _APP_PHONE_vRestartPlayer(void);
Uint16Type _APP_PHONE_AT_PNCC_u16GetValidPlccNumber(void);

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


#endif /* _APP_PHONE_CI_H */

/* End Of File APP_PHONE_CI.h */
