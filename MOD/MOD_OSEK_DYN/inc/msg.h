/* PRQA S 0883 EOF */ /* this file is intentionally included multiple times */
/* file: msg.h, automatically generated by GENTMS470.EXE, Version: 3.10 */
/* from: C:\PROGRAMME\ECLIPSE-CPP\ECLIPSE\WORKSPACE\UHV-NAR\MOD\MOD_OSEK_DYN\CFG\UHV_NAR.oil */
/* at  : Thu Nov 10 09:24:19 2011 */
/* License 2400100185 for S1nn GmbH u. Co. KG, Infotainment and Connectivity */
/* Implementation: TMS470R1 */
/* Version of general code: 3.54b */

#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif
#define osdVrmGenMajRelNum 3
#define osdVrmGenMinRelNum 10
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif

#ifndef __MSG_1__
#define __MSG_1__

/* first part - included at the beginning of osekcom.h */

/* conformance class */
#define osdCCCA 1
#define osdCCCB 2
#define osdCCC osdCCCB

/* number of all messages */
#define osdNumberOfMsgs 19

/* number of all unqueued messages */
#define osdNumberOfMsgsUnQueuedMsgs 10

/* number of all queued messages */
#define osdNumberOfMsgsQueuedMsgs 9

/* number of all queues with size >= 1 */
#define osdNumberOfMsgsQueues 9

/* number of all receiving tasks */
#define osdNumberOfMsgsRcvTasks 3

/* number of all notifications */
#define osdNumberOfMsgsNotifications 19

/* number of callback notifications */
#define osdNumberOfMsgsCallbackNotifics 0

/* number of flag notifications */
#define osdNumberOfMsgsFlagNotifics 0

/* usage of message resource */
#define osdUseMessageResource 1

/* usage of message status */
#define osdUseMessageStatus 1

#define osdMsgQSizemsgPhoneBapIndication 5
#define osdMsgQSizemsgPhoneBookBapIndication 5
#define osdMsgQSizemsgMdiBapIndication 5
#define osdMsgQSizemsgCANInfotainmentInfo 3
#define osdMsgQSizemsgAppCanStates 3
#define osdMsgQSizemsgKpbBapIndication 5
#define osdMsgQSizemsgButtonStateInfo_Mfl 4
#define osdMsgQSizemsgAppTaskSystemStates 2
#define osdMsgQSizemsgApplATResponse 8
#ifndef osdAsmFile

/* user defined structures for messages */
#if osdNumberOfMsgs > 0
#if defined USE_QUOTE_INCLUDES
 #include "umsgtype.h"
#else
 #include <umsgtype.h>
#endif
#else
extern uint8 ucComDummy;
#endif

/* collection of all user defined structures */
typedef union
{
   PHY_BUT_DRV_ST_MESSAGE PHY_BUT_DRV_ST_MESSAGE_obj;
   BapIndication BapIndication_obj;
   BapRequest BapRequest_obj;
   PWMGR_enVoltageStates PWMGR_enVoltageStates_obj;
   CanSignalMessageType CanSignalMessageType_obj;
   APP_CAN_MsgStates APP_CAN_MsgStates_obj;
   APP_DIAG_ST_DIAG_RESPONSE APP_DIAG_ST_DIAG_RESPONSE_obj;
   APP_DIAG_ST_DIAG_REQUEST APP_DIAG_ST_DIAG_REQUEST_obj;
   APP_PHONE_CanDataType APP_PHONE_CanDataType_obj;
   VCRPerfMeasStatusMsgType VCRPerfMeasStatusMsgType_obj;
   CSHDL_enSystemStates CSHDL_enSystemStates_obj;
   ATRspMessageType ATRspMessageType_obj;
} DataType;

/* a reference to the user defined data will be used */
typedef DataType* DataRefType;

#endif

#else
#ifndef __MSG_2__
#define __MSG_2__
/* second part - included at the end of osekcom.h */

/* externals for messages */
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgButtonStateInfo;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBapIndication;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBapRequest;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBookBapIndication;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBookBapRequest;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgMdiBapIndication;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgMdiBapRequest;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgPwrmgrState;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgCANInfotainmentInfo;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgAppCanStates;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgKpbBapIndication;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgKpbBapRequest;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgAppDiagDiagnoseResponse;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgAppDiagDiagnoseRequest;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgButtonStateInfo_Mfl;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgTelefonStates;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgHostTaskASRStatus;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgAppTaskSystemStates;
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 msgApplATResponse;

/* externals for unqueued user data (needed for "WithoutCopy") */
extern PHY_BUT_DRV_ST_MESSAGE msgButtonStateInfoUsrData;
extern BapRequest msgPhoneBapRequestUsrData;
extern BapRequest msgPhoneBookBapRequestUsrData;
extern BapRequest msgMdiBapRequestUsrData;
extern PWMGR_enVoltageStates msgPwrmgrStateUsrData;
extern BapRequest msgKpbBapRequestUsrData;
extern APP_DIAG_ST_DIAG_RESPONSE msgAppDiagDiagnoseResponseUsrData;
extern APP_DIAG_ST_DIAG_REQUEST msgAppDiagDiagnoseRequestUsrData;
extern APP_PHONE_CanDataType msgTelefonStatesUsrData;
extern VCRPerfMeasStatusMsgType msgHostTaskASRStatusUsrData;

/* access names - global buffers for copies */
extern PHY_BUT_DRV_ST_MESSAGE RecLogiButMan_ButtonInfo;
extern PHY_BUT_DRV_ST_MESSAGE SentPhyButDriver_ButtonInfo;
extern BapIndication SentPhoneBapIndQueue;
extern BapIndication RecPhoneBapIndQueue;
extern BapRequest RecPhoneBapReqQueue;
extern BapRequest SentPhoneBapReqQueue;
extern BapIndication SentPhoneBookBapIndQueue;
extern BapIndication RecPhoneBookBapIndQueue;
extern BapRequest RecPhoneBookBapReqQueue;
extern BapRequest SentPhoneBookBapReqQueue;
extern BapRequest SentMdiBapIndQueue;
extern BapRequest RecMdiBapIndQueue;
extern BapRequest RecMdiBapReqQueue;
extern BapRequest SentMdiBapReqQueue;
extern PWMGR_enVoltageStates RecPWMGR_enVoltageStates;
extern PWMGR_enVoltageStates SentPWMGR_enVoltageStates;
extern CanSignalMessageType SentBSGmessageType;
extern CanSignalMessageType RecBSGmessageType;
extern APP_CAN_MsgStates SentAPP_CAN_MsgStates;
extern APP_CAN_MsgStates recAPP_CAN_MsgStates;
extern BapIndication SentBapIndicationQueue;
extern BapIndication RecBapIndicationQueue;
extern BapRequest RecBapReqQueue;
extern BapRequest SentBapRequestQueue;
extern APP_DIAG_ST_DIAG_RESPONSE RecAppDiagResp;
extern APP_DIAG_ST_DIAG_RESPONSE SentAppDiagResp;
extern APP_DIAG_ST_DIAG_REQUEST SentAppDiagReq;
extern APP_DIAG_ST_DIAG_REQUEST RecAppDiagReq;
extern PHY_BUT_DRV_ST_MESSAGE SentMflKeyCodeType;
extern PHY_BUT_DRV_ST_MESSAGE RecMflKeyCodeType;
extern APP_PHONE_CanDataType RecTelMuteBehaviorType;
extern APP_PHONE_CanDataType SentTelMuteBehaviorType;
extern VCRPerfMeasStatusMsgType RecVCRPerfMeasStatusMsgType;
extern VCRPerfMeasStatusMsgType SentVCRPerfMeasStatusMsgType;
extern CSHDL_enSystemStates RecCSHDL_enSystemStates;
extern CSHDL_enSystemStates SentCSHDL_enSystemStates;
extern ATRspMessageType RecApplATResponse;
extern ATRspMessageType SentApplATResponse;

/* access names - direct access to message object */

/* prototypes for callback functions */

/* defines for flag access */
#if osdNumberOfMsgsFlagNotifics
extern osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[osdNumberOfMsgsFlagNotifics];
#endif

#endif

#endif

#ifndef osrNumberOfMsgsFlagNotifics /*double include preventer*/
# if osdLib
#  define osrNumberOfMsgsFlagNotifics oskNumberOfMsgsFlagNotifics
extern const uint16 oskNumberOfMsgsFlagNotifics;
# else
#  define osrNumberOfMsgsFlagNotifics osdNumberOfMsgsFlagNotifics
# endif
#endif

/* END OF C:\PROGRAMME\ECLIPSE-CPP\ECLIPSE\WORKSPACE\UHV-NAR\MOD\MOD_OSEK_DYN\CFG\tcb\msg.h */

