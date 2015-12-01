/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_PHONE
*-----------------------------------------------------------------------------
* $Date: 2011-08-30 16:53:44 +0200 (Di, 30 Aug 2011) $
* $Rev: 21443 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_H
#define _APP_PHONE_H

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
#include "APP_PHONE_CE.h"
#include "APP_PHONE_CAN.h"
#include "APP_COMMON_Utils.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#ifndef IAPP_PHONE_EVENTS
#define IAPP_PHONE_EVENTS
#endif

typedef enum
{
    APP_PHONE_INVALID = 0x00,
    APP_PHONE_OPERATOR_NAME_CHANGED,
    APP_PHONE_NW_SIGSTRENGHT_CHANGED,
    APP_PHONE_ROAMING_CHANGED,
    APP_PHONE_SERV_AVAIL_CHANGED,
    APP_PHONE_BATT_LEVEL_CHANGED,
    APP_PHONE_CALL_STATE_CHANGED,
/*  APP_PHONE_CALL_STATE_ACTIVE_0,
    APP_PHONE_CALL_STATE_ACTIVE_1,
    APP_PHONE_CALL_STATE_HELD_0,
    APP_PHONE_CALL_STATE_HELD_1,
    APP_PHONE_CALL_STATE_OUT_DIALING_0,
    APP_PHONE_CALL_STATE_OUT_DIALING_1,
    APP_PHONE_CALL_STATE_OUT_RINGING_0,
    APP_PHONE_CALL_STATE_OUT_RINGING_1,
    APP_PHONE_CALL_STATE_INCOMING_0,
    APP_PHONE_CALL_STATE_INCOMING_1,
    APP_PHONE_CALL_STATE_WAITING_0,
    APP_PHONE_CALL_STATE_WAITING_1,
    APP_PHONE_CALL_STATE_TERMINATED_0,
    APP_PHONE_CALL_STATE_TERMINATED_1, */ // currently not used
    APP_PHONE_MIC_MUTE_CHANGED,
    APP_PHONE_HANDSFREE_MODE_CHANGED,
    APP_PHONE_HFP_VERSION_CHANGED,
    APP_PHONE_HEADSET_MODE_CHANGED,
    APP_PHONE_OUTG_CALL_INDICATION_TIMEOUT,
    APP_PHONE_OUTG_CALL_ERROR,
    // BEGIN Susanne: 06.08.2009 Changes for Missed Call Handling 
    //APP_PHONE_KL15OFF_MISSED_CALLS
    //APP_PHONE_MISSED_CALLS
    // END Susanne: 06.08.2009 Changes for Missed Call Handling 
    APP_PHONE_REDIAL_DENIED_DUE_TO_EMPTY_STACK
} APP_PHONE_enNotificationId;

typedef struct
{
    AT_IndicatorString2             name;
    Uint16Type                      name_len;
}AT_NetworkProvider;

//define for Hang up all calls mechanism
#define APP_PHONE_MAX_NB_RETRIES_HANGUP_CALL 2 //values 1..n, 1 = wait only for initial hang up call (2 = wait 2x for PNCC)
#define APP_PHONE_TIMEOUT_HANGUP_CALL 2000 //max time for waiting a call hang up execution (waiting for second CHUP execution)


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_sfInitApp(void)                   */   
/** 
    \brief      Inits the phone application 
  
    \brief      Description:\n
                Inits the Phone Application \n
    
    \return     SuccessFailType
                success or error
        
   */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONE_sfInitApp(void);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_sfRequestHfpVersion()                            */
/** 
    \brief      request hfp version of active user  
 
    \brief      Description:\n
                request hfp version of active user  \n
 
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_sfRequestHfpVersion();



/** 
\brief	description of the interface IAPP_PHONE_EVENTS
*/
#ifdef IAPP_PHONE_EVENTS /* BEGIN DECLARE INTERFACE(IAPP_PHONE_EVENTS) */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vHandleEvents()                                    */
/** 
    \brief      Handles incoming events 
  
    \brief      Description:\n
                Waits for incoming events from AT_Cmd_Parser and BAP-Dispatcher                
 
    \return     SuccessFailType
                success or error
        
   */
/*----------------------------------------------------------------------------*/
void APP_PHONE_vHandleEvents(EventMaskType events);
#endif /* END DECLARE INTERFACE(IAPP_PHONE_EVENTS) */


extern void APP_PHONE_vCbMicMute( void );
void _APP_PHONE_MuteAck_Cb(APP_COMMON_enResponseCode result);


APP_COMMON_enResponseCode APP_PHONE_rcDialNumber(  void (*cb)(APP_COMMON_enResponseCode),   
                                                   const Uint8Type * numberString,
                                                   Uint16Type numberLen );
APP_COMMON_enResponseCode APP_PHONE_rcDialPbIndex( void (*cb)(APP_COMMON_enResponseCode), 
                                                    Uint16Type pbIndex);
APP_COMMON_enResponseCode APP_PHONE_rcDialPnIndex( void (*cb)(APP_COMMON_enResponseCode), 
                                                    Uint16Type pbIndex,
                                                    Uint16Type pnIndex );
APP_COMMON_enResponseCode APP_PHONE_rcRedial( void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcDialNumberType( void (*cb)(APP_COMMON_enResponseCode),
                                                      Uint16Type pbIndex,
                                                      ATPhoneNumberTypeEnum numberType );
APP_COMMON_enResponseCode APP_PHONE_rcAcceptCall(void (*cb)(APP_COMMON_enResponseCode));   
APP_COMMON_enResponseCode APP_PHONE_rcHangUp( void (*cb)(APP_COMMON_enResponseCode),   
                                              ATCallHangupEnum callIndex);
APP_COMMON_enResponseCode APP_PHONE_rcHangUpAll( void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcPhoneStatus(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcCallState(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcCallInfo(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcSendDTMF(void (*cb)(APP_COMMON_enResponseCode),   
                                               const Uint8Type *dtmfChars,
                                               Uint16Type numChars );
APP_COMMON_enResponseCode APP_PHONE_rcPlpdRequest(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcCallHold(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcCallReplace(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcResumeCall(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcHandsFreeOnOff(void (*cb)(APP_COMMON_enResponseCode),   
                                                     ATAudioTransferModeEnum mode);
APP_COMMON_enResponseCode APP_PHONE_rcMicroMuteOnOff(void (*cb)(APP_COMMON_enResponseCode),   
                                                     ATMicModeEnum mode );
APP_COMMON_enResponseCode APP_PHONE_rcHeadsetOnOff( void (*cb)(APP_COMMON_enResponseCode),   
                                                    ATAudioGatewayModeEnum mode);
APP_COMMON_enResponseCode APP_PHONE_rcSwitchCalls(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcJoinToConference( void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcPutCallsThrough( void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcSplitCallFromConference( void (*cb)(APP_COMMON_enResponseCode),
                                                               ATCallIndexEnum callIndex);
//APP_COMMON_enResponseCode APP_PHONE_rcReleaseActiveCallAcceptWaitingCall(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcToggleMicMute(void (*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_PHONE_rcSilentRinging();


const AT_NetworkProvider *APP_PHONE_GetNetworkProvider(void);
AT_IndicatorValue APP_PHONE_GetSignalStrength(void);
AT_IndicatorValue APP_PHONE_GetBatteryLevel(void);
AT_IndicatorValue APP_PHONE_GetRoaming(void);
BooleanType       APP_PHONE_bNetworkAvailable(void);
AT_IndicatorValue APP_PHONE_GetNetworkAvailable();
BooleanType       APP_PHONE_bRedialNumberAvailable(void);
BooleanType       APP_PHONE_bIsMicMuted(void);
BooleanType       APP_PHONE_bIsHandsFreeOff(void);
BooleanType       APP_PHONE_bIsHeadsetOn(void);
ATCoDProfileEnum  APP_PHONE_enGetHfpVersion(void);
BooleanType APP_PHONE_bIsValidPhoneNumber( const Uint8Type *pNumber, Uint16Type numberLen); 
       
BooleanType APP_PHONE_bIsUnknownCallerId(const Uint8Type * pCallerId, Uint16Type callerIdLen);

extern Uint16Type APP_PHONE_u16MissedCalls(void);
extern Uint16Type APP_PHONE_u16MissedNumbers(void);
void APP_PHONE_vResetMissedCalls();

void APP_PHONE_vHandleATResponse(const ATRspMessageType *atRsp);

void APP_PHONE_vSetRestartPlayerOnCallIdleState( void );

#include "APP_DEVICE.h"
void APP_PHONE_vOnAppDeviceNotification(APP_DEVICE_enNotificationId new_notification);

#include "APP_CSHDL.h"
void APP_PHONE_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
#endif /* _APP_PHONE_H */

//AT response for Hang up all call handling
void APP_PHONE_HangUp_CallStatusChange(void);
//additional function for hang up all calls handling
void APP_PHONE_vResetHangUpAllCallsRetries(void);

/* End Of File APP_PHONE.h */

