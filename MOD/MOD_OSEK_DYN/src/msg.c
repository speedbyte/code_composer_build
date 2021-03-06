/* file: msg.c, automatically generated by GENTMS470.EXE, Version: 3.10 */
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

#if defined USE_QUOTE_INCLUDES
 #include "osekcom.h"
#else
 #include <osekcom.h>
#endif

#if osdLib
osqROM0 osqROM1 const osqROM2 uint16 osqROM3 oskNumberOfMsgsFlagNotifics= osdNumberOfMsgsFlagNotifics;
#endif
#if osdNumberOfMsgsFlagNotifics
osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[osdNumberOfMsgsFlagNotifics];
#else
   #if osdLib
   osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[1];
   #endif
#endif
/* unqueued user data */
PHY_BUT_DRV_ST_MESSAGE msgButtonStateInfoUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
static BapIndication msgPhoneBapIndicationUsrDataQ0[osdMsgQSizemsgPhoneBapIndication];
BapRequest msgPhoneBapRequestUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
static BapIndication msgPhoneBookBapIndicationUsrDataQ0[osdMsgQSizemsgPhoneBookBapIndication];
BapRequest msgPhoneBookBapRequestUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
static BapRequest msgMdiBapIndicationUsrDataQ0[osdMsgQSizemsgMdiBapIndication];
BapRequest msgMdiBapRequestUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
PWMGR_enVoltageStates msgPwrmgrStateUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
static CanSignalMessageType msgCANInfotainmentInfoUsrDataQ0[osdMsgQSizemsgCANInfotainmentInfo];
static APP_CAN_MsgStates msgAppCanStatesUsrDataQ0[osdMsgQSizemsgAppCanStates];
static BapIndication msgKpbBapIndicationUsrDataQ0[osdMsgQSizemsgKpbBapIndication];
BapRequest msgKpbBapRequestUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
APP_DIAG_ST_DIAG_RESPONSE msgAppDiagDiagnoseResponseUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
APP_DIAG_ST_DIAG_REQUEST msgAppDiagDiagnoseRequestUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
static PHY_BUT_DRV_ST_MESSAGE msgButtonStateInfo_MflUsrDataQ0[osdMsgQSizemsgButtonStateInfo_Mfl];
APP_PHONE_CanDataType msgTelefonStatesUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
VCRPerfMeasStatusMsgType msgHostTaskASRStatusUsrData;   /* PRQA S 1504 */  /* might be accessed from extern */
static CSHDL_enSystemStates msgAppTaskSystemStatesUsrDataQ0[osdMsgQSizemsgAppTaskSystemStates];
static ATRspMessageType msgApplATResponseUsrDataQ0[osdMsgQSizemsgApplATResponse];

/* queued user data */

/* queues with size >= 1 */

static osqTcbRAM1 osqTcbRAM2 osComMsgQueueRam osqTcbRAM3 osComQueueRam[osdNumberOfMsgsQueues];

static osqROM0 osqROM1 const osqROM2 osComMsgQueue osqROM3 oskComQueue[osdNumberOfMsgsQueues] =
{
{
   (DataRefType)msgPhoneBapIndicationUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgPhoneBapIndicationUsrDataQ0[osdMsgQSizemsgPhoneBapIndication-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[0]  /* RAM part */
},
{
   (DataRefType)msgPhoneBookBapIndicationUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgPhoneBookBapIndicationUsrDataQ0[osdMsgQSizemsgPhoneBookBapIndication-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[1]  /* RAM part */
},
{
   (DataRefType)msgMdiBapIndicationUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgMdiBapIndicationUsrDataQ0[osdMsgQSizemsgMdiBapIndication-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[2]  /* RAM part */
},
{
   (DataRefType)msgCANInfotainmentInfoUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgCANInfotainmentInfoUsrDataQ0[osdMsgQSizemsgCANInfotainmentInfo-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[3]  /* RAM part */
},
{
   (DataRefType)msgAppCanStatesUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgAppCanStatesUsrDataQ0[osdMsgQSizemsgAppCanStates-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[4]  /* RAM part */
},
{
   (DataRefType)msgKpbBapIndicationUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgKpbBapIndicationUsrDataQ0[osdMsgQSizemsgKpbBapIndication-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[5]  /* RAM part */
},
{
   (DataRefType)msgButtonStateInfo_MflUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgButtonStateInfo_MflUsrDataQ0[osdMsgQSizemsgButtonStateInfo_Mfl-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[6]  /* RAM part */
},
{
   (DataRefType)msgAppTaskSystemStatesUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgAppTaskSystemStatesUsrDataQ0[osdMsgQSizemsgAppTaskSystemStates-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[7]  /* RAM part */
},
{
   (DataRefType)msgApplATResponseUsrDataQ0,  /* first element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   (DataRefType)&msgApplATResponseUsrDataQ0[osdMsgQSizemsgApplATResponse-1],  /* last element in queue */ /* PRQA S 3305 */  /* might be casted to other alignment */
   &osComQueueRam[8]  /* RAM part */
},
};

/* access names - global buffers for copies */

PHY_BUT_DRV_ST_MESSAGE RecLogiButMan_ButtonInfo;
PHY_BUT_DRV_ST_MESSAGE SentPhyButDriver_ButtonInfo;
BapIndication SentPhoneBapIndQueue;
BapIndication RecPhoneBapIndQueue;
BapRequest RecPhoneBapReqQueue;
BapRequest SentPhoneBapReqQueue;
BapIndication SentPhoneBookBapIndQueue;
BapIndication RecPhoneBookBapIndQueue;
BapRequest RecPhoneBookBapReqQueue;
BapRequest SentPhoneBookBapReqQueue;
BapRequest SentMdiBapIndQueue;
BapRequest RecMdiBapIndQueue;
BapRequest RecMdiBapReqQueue;
BapRequest SentMdiBapReqQueue;
PWMGR_enVoltageStates RecPWMGR_enVoltageStates;
PWMGR_enVoltageStates SentPWMGR_enVoltageStates;
CanSignalMessageType SentBSGmessageType;
CanSignalMessageType RecBSGmessageType;
APP_CAN_MsgStates SentAPP_CAN_MsgStates;
APP_CAN_MsgStates recAPP_CAN_MsgStates;
BapIndication SentBapIndicationQueue;
BapIndication RecBapIndicationQueue;
BapRequest RecBapReqQueue;
BapRequest SentBapRequestQueue;
APP_DIAG_ST_DIAG_RESPONSE RecAppDiagResp;
APP_DIAG_ST_DIAG_RESPONSE SentAppDiagResp;
APP_DIAG_ST_DIAG_REQUEST SentAppDiagReq;
APP_DIAG_ST_DIAG_REQUEST RecAppDiagReq;
PHY_BUT_DRV_ST_MESSAGE SentMflKeyCodeType;
PHY_BUT_DRV_ST_MESSAGE RecMflKeyCodeType;
APP_PHONE_CanDataType RecTelMuteBehaviorType;
APP_PHONE_CanDataType SentTelMuteBehaviorType;
VCRPerfMeasStatusMsgType RecVCRPerfMeasStatusMsgType;
VCRPerfMeasStatusMsgType SentVCRPerfMeasStatusMsgType;
CSHDL_enSystemStates RecCSHDL_enSystemStates;
CSHDL_enSystemStates SentCSHDL_enSystemStates;
ATRspMessageType RecApplATResponse;
ATRspMessageType SentApplATResponse;

/* receiving tasks */

static osqROM0 osqROM1 const osqROM2 osComTaskPtr osqROM3 oskRcvTasks[osdNumberOfMsgsRcvTasks] =
{
{
   OsAppTask_event_100ms, /* task */
   0 /* WithoutCopy flag */
},
{
   OsHostTask_10ms, /* task */
   0 /* WithoutCopy flag */
},
{
   OsSystemTask_event, /* task */
   0 /* WithoutCopy flag */
},
};

/* notifications */

static osqROM0 osqROM1 const osqROM2 osComNotification osqROM3 oskNotifications[osdNumberOfMsgsNotifications] =
{
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evLogiButMan_ButtonInfo,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskPhoneBapInd,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskPhoneBapReq,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskPhoneBookBapInd,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskPhoneBookBapReq,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskMdiBapInd,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskMdiBapReq,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsSystemTask_event,  /* task to activate or post event to */
   evSystemTaskVoltageState,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsSystemTask_event,  /* task to activate or post event to */
   evSystemTaskCANInfotainmentInfo,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskCanStates,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskKpbBapInd,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskKpbBapReq,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskDiagnoseResponse,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskDiagnoseRequest,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskMFLKeys,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskTelefonStates,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsHostTask_10ms,  /* task to activate or post event to */
   evHostTaskASRStatus,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskSystemStates,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
{
   osdCOM_ACTION_EVENT,  /* action */
   OsAppTask_event_100ms,  /* task to activate or post event to */
   evAppTaskApplATResponse,  /* event */
#if osdNumberOfMsgsCallbackNotifics
   (osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
   0,  /* flag */
#endif
},
};

/* generic RAM part */
#if osdUseMessageResource
static osqTcbRAM1 osqTcbRAM2 osComMsgRamGeneric osqTcbRAM3 osMsgRamGeneric[osdNumberOfMsgs];
#endif

/* ROM parts of all messages */

static osqROM0 osqROM1 const osqROM2 osComMsgRom osqROM3 oskMsgRom[osdNumberOfMsgs] =
{
{
   sizeof(PHY_BUT_DRV_ST_MESSAGE),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[0],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgButtonStateInfoUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgButtonStateInfo",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[0],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapIndication),  /* message length */
   osdMsgQSizemsgPhoneBapIndication,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[1],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[0])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgPhoneBapIndication",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[1],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapRequest),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[2],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgPhoneBapRequestUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgPhoneBapRequest",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[2],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapIndication),  /* message length */
   osdMsgQSizemsgPhoneBookBapIndication,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[3],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[1])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgPhoneBookBapIndication",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[3],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapRequest),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[4],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgPhoneBookBapRequestUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgPhoneBookBapRequest",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[4],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapRequest),  /* message length */
   osdMsgQSizemsgMdiBapIndication,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[5],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[2])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgMdiBapIndication",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[5],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapRequest),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[6],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgMdiBapRequestUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgMdiBapRequest",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[6],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(PWMGR_enVoltageStates),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[7],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgPwrmgrStateUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgPwrmgrState",  /* name of the message */
   #endif
   &oskRcvTasks[2], /* pointer to list of receiving tasks */
   &oskNotifications[7],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(CanSignalMessageType),  /* message length */
   osdMsgQSizemsgCANInfotainmentInfo,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[8],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[3])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgCANInfotainmentInfo",  /* name of the message */
   #endif
   &oskRcvTasks[2], /* pointer to list of receiving tasks */
   &oskNotifications[8],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(APP_CAN_MsgStates),  /* message length */
   osdMsgQSizemsgAppCanStates,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[9],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[4])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgAppCanStates",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[9],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapIndication),  /* message length */
   osdMsgQSizemsgKpbBapIndication,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[10],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[5])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgKpbBapIndication",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[10],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(BapRequest),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[11],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgKpbBapRequestUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgKpbBapRequest",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[11],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(APP_DIAG_ST_DIAG_RESPONSE),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[12],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgAppDiagDiagnoseResponseUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgAppDiagDiagnoseResponse",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[12],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(APP_DIAG_ST_DIAG_REQUEST),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[13],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgAppDiagDiagnoseRequestUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgAppDiagDiagnoseRequest",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[13],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(PHY_BUT_DRV_ST_MESSAGE),  /* message length */
   osdMsgQSizemsgButtonStateInfo_Mfl,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[14],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[6])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgButtonStateInfo_Mfl",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[14],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(APP_PHONE_CanDataType),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[15],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgTelefonStatesUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgTelefonStates",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[15],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(VCRPerfMeasStatusMsgType),  /* message length */
   0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[16],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&msgHostTaskASRStatusUsrData)},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 3305 */  /* might be casted to other alignment */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgHostTaskASRStatus",  /* name of the message */
   #endif
   &oskRcvTasks[1], /* pointer to list of receiving tasks */
   &oskNotifications[16],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(CSHDL_enSystemStates),  /* message length */
   osdMsgQSizemsgAppTaskSystemStates,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[17],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[7])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgAppTaskSystemStates",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[17],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
{
   sizeof(ATRspMessageType),  /* message length */
   osdMsgQSizemsgApplATResponse,  /* fifo size - also used to distinguisch queued and unqueued msgs */
   1,  /* number of receivers */
   #if osdUseMessageResource
   &osMsgRamGeneric[18],  /* pointer to generic RAM part */
   #endif
   {(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&oskComQueue[8])},  /* pointer to data part - message can either be queued or unqueued */ /* PRQA S 0311 */  /* const might be casted to var */
   #if STATUS_LEVEL == EXTENDED_STATUS
   osdMAGIC_MSG,  /* magic number */
   #endif
   #if (osdGHSDebugSupport==1)
   "msgApplATResponse",  /* name of the message */
   #endif
   &oskRcvTasks[0], /* pointer to list of receiving tasks */
   &oskNotifications[18],  /* pointer to list of notifications */
   1,  /* number of notifications */
   0  /* send message without copying data */
},
};

/* references to the messages used */

osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgButtonStateInfo = (osComMsg)(&(oskMsgRom[0]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBapIndication = (osComMsg)(&(oskMsgRom[1]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBapRequest = (osComMsg)(&(oskMsgRom[2]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBookBapIndication = (osComMsg)(&(oskMsgRom[3]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgPhoneBookBapRequest = (osComMsg)(&(oskMsgRom[4]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgMdiBapIndication = (osComMsg)(&(oskMsgRom[5]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgMdiBapRequest = (osComMsg)(&(oskMsgRom[6]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgPwrmgrState = (osComMsg)(&(oskMsgRom[7]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgCANInfotainmentInfo = (osComMsg)(&(oskMsgRom[8]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgAppCanStates = (osComMsg)(&(oskMsgRom[9]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgKpbBapIndication = (osComMsg)(&(oskMsgRom[10]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgKpbBapRequest = (osComMsg)(&(oskMsgRom[11]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgAppDiagDiagnoseResponse = (osComMsg)(&(oskMsgRom[12]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgAppDiagDiagnoseRequest = (osComMsg)(&(oskMsgRom[13]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgButtonStateInfo_Mfl = (osComMsg)(&(oskMsgRom[14]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgTelefonStates = (osComMsg)(&(oskMsgRom[15]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgHostTaskASRStatus = (osComMsg)(&(oskMsgRom[16]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgAppTaskSystemStates = (osComMsg)(&(oskMsgRom[17]));
osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 msgApplATResponse = (osComMsg)(&(oskMsgRom[18]));

#if osdNumberOfMsgs > 0

osqFunc1 void osqFunc2 osInternalStartCOM(void)
{
   /* some auxiliary variables */
   uint16 i;

   /* initialise unqueued messages */
   #if osdUseMessageResource
   for(i=0;i<osdNumberOfMsgs;i++)
   {
      osMsgRamGeneric[i].resource=0;
   }

   #endif
   /* initialise queues with size >= 1 */
   for(i=0;i<osdNumberOfMsgsQueues;i++)
   {
      osComQueueRam[i].w=oskComQueue[i].first;
      osComQueueRam[i].r=oskComQueue[i].first;
      osComQueueRam[i].msgCount=0;
      osComQueueRam[i].status=E_COM_NOMSG;
   }

}

#if osdCCC==osdCCCB
#if osdUseMessageResource
osqFunc1 uint8 osqFunc2 osComResourcesAreTaken(void)
{
   uint16 i;
   for(i=0;i<osdNumberOfMsgs;i++)
   {
      if(osMsgRamGeneric[i].resource!=0)
      {return 1;}
   }
    return 0;
}
#endif
#endif

#endif /* osdNumberOfMsgs */


/* END OF C:\PROGRAMME\ECLIPSE-CPP\ECLIPSE\WORKSPACE\UHV-NAR\MOD\MOD_OSEK_DYN\CFG\tcb\msg.c */

