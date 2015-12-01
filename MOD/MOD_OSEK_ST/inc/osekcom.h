/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekcom.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.71
|
|  Description: OSEK-COM 2.2 conformant inter-task communication
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH               All rights reserved.
|****************************************************************************/


/* double include preventer */
#ifndef __OSEKCOM__
#define __OSEKCOM__

/*lint -save Messages inhibited in this file, will be re-enabled at the end of file */
/*lint -e539 Did not expect positive indentation */

/* Vector release management */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif
/* KB begin vrmReleaseNumber */
/* Source release number */
#define osdVrmMajRelNum 3
#define osdVrmMinRelNum 10
/* KB end vrmReleaseNumber */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif

/* needed for operating system constants and types */
#if defined USE_QUOTE_INCLUDES
 #include "osek.h"
#else
 #include <osek.h>
#endif

/* message definitions generated by OIL-tool - part 1 */
#ifdef osdIncludeLibConfig

   #if defined USE_QUOTE_INCLUDES
    #include "msglib1.h" /* (not variant dependant) */
   #else
    #include <msglib1.h>
   #endif

   #if (osdLibVariant == 1)
   #endif
   
   #if (osdLibVariant == 2)
   #endif
   
   #if (osdLibVariant == 3)
   #endif
   
   #if (osdLibVariant == 4)
   #endif

   #if ((osdLibVariant < 1) || (osdLibVariant > 4))
   #error unknown library variant/configuration file
   #endif

#else /*osdIncludeLibConfig*/

   #include "msg.h"

#endif /*osdIncludeLibConfig*/



/* magic number for messages */
#define osdMAGIC_MSG 0x55AB

/* base for mandatory COM error codes */
#define E_COM_ERROR_CODE_BASE 32
/* base for implementation specific COM error codes */
#define E_COM_SYS_ERROR_CODE_BASE 48

/* error codes */
#define E_COM_BUSY (E_COM_ERROR_CODE_BASE)
#define E_COM_ID (E_COM_ERROR_CODE_BASE+1)
#define E_COM_LIMIT (E_COM_ERROR_CODE_BASE+2)
#define E_COM_LOCKED (E_COM_ERROR_CODE_BASE+3)
#define E_COM_NOMSG (E_COM_ERROR_CODE_BASE+4)
#define E_COM_RX_ON (E_COM_ERROR_CODE_BASE+5)    /* for compatibilty reasons */

#define E_COM_SYS_NOT_SUPPORTED (E_COM_SYS_ERROR_CODE_BASE)
#define E_COM_SYS_NOTIFY_FAILED (E_COM_SYS_ERROR_CODE_BASE+1)
#define E_COM_SYS_QUEUE_NOT_FOUND (E_COM_SYS_ERROR_CODE_BASE+2)

/* shutdown modes */
#define COM_SHUTDOWN_IMMEDIATE 0

/* actions */
#define osdCOM_ACTION_NONE 0
#define osdCOM_ACTION_TASK 1
#define osdCOM_ACTION_EVENT 2
#define osdCOM_ACTION_CALLBACK 3
#define osdCOM_ACTION_FLAG 4

/* generic RAM part of all messages */
#if osdUseMessageResource
typedef struct osComMsgRamGenericStruct
{
   uint8 resource; /* message resource */
} osComMsgRamGeneric;
#endif

/* message queue for queued messages - RAM part */
typedef struct osComMsgQueueRamStruct
{
   StatusType status;  /* queue status */
   DataRefType w; /* pointer for writing */
   DataRefType r;   /* pointer for reading */
   uint8 msgCount; /* number of stored messages */
} osComMsgQueueRam;

/* message queue for queued messages -  ROM part */
typedef struct osComMsgQueueStruct
{
   DataRefType first;  /* first element in queue */
   DataRefType last;  /* last element in queue */
   osComMsgQueueRam * pQRam;  /* RAM part */
} osComMsgQueue;

typedef union  /* pointer to data part - message can either be queued or unqueued */
{
   osqPROM1 osqPROM2 osComMsgQueue osqPROM3 *queue;  /* list of queues in ROM part of queued message */
   DataRefType msg; /* RAM part of unqueued message */
} osComMsgData;

/* notification */
typedef void (*osComCbFct)(void);
/* typedef uint8 FlagType; *//* FlagType moved to Common.h T.B. 17.10.2008 */
typedef uint8 FlagValue;
typedef struct osComNotificationStruct
{
   uint8 action;  /* action */
   TaskType task;  /* task to activate or post event to */
   EventMaskType event; /* event */
   #if osdNumberOfMsgsCallbackNotifics
   osComCbFct callback;
   #endif
   #if osdNumberOfMsgsFlagNotifics
   FlagType flag;
   #endif
}  osComNotification;

/* structure for task list */
typedef struct osComTaskPtrStruct
{
   TaskType task;
   uint8 withoutCopy;
} osComTaskPtr;

/* ROM part */
typedef struct osComMsgRomStruct
{
   uint16 msgLen;  /* message length */
   uint8 fifoSize;      /* fifo size - also used to distinguish queued and unqueued msgs */
   uint8 rcvNum;     /* number of receivers */

#if osdUseMessageResource
   osqPRAM1 osqPRAM2 osComMsgRamGeneric osqPRAM3 * ramPtrGen;  /* pointer to generic RAM part */
#endif

   osComMsgData dataPtr; /* pointer to data part - message can either be queued or unqueued */

#if STATUS_LEVEL == EXTENDED_STATUS
   uint16  magic;    /* magic number  */
#endif

   /* KB begin osekHwComDebugMsg */
   #if (osdGHSDebugSupport==1)
      osqPROM1 const osqPROM2 char osqPROM3 *name;	/* name of the message */  /* PRQA S 5013 */ /* keep char type for debugger interface */
   #endif
   /* KB end osekHwComDebugMsg */

   osqPROM1 const osqPROM2 osComTaskPtr osqPROM3 * taskList;  /* pointer to list of receiving tasks */
   osqPROM1 const osqPROM2 osComNotification osqPROM3 * pNotification;  /* pointer to list of notifications */
   uint8 numOfNotifications;  /* number of notifications */

   uint8 sendWithoutCopy;  /* send message without copying data */
} osComMsgRom;

/* COM data types */
typedef osqPROM1 const osqPROM2 osComMsgRom osqPROM3 * SymbolicName;
typedef SymbolicName osComMsg; /* for compatibility reasons */
typedef void * AccessNameRef;
typedef uint8 Scalar;

/* prototypes */
/* KB begin osekCallBackSectionStart */
/* KB end osekCallBackSectionStart */
osqFunc1 StatusType osqFunc2 MessageInit(void);
/* KB begin osekCallBackSectionEnd */
/* KB end osekCallBackSectionEnd */

osqFunc1 StatusType osqFunc2 osStartCOM(void);
#if osdNumberOfMsgs>0
osqFunc1 StatusType osqFunc2 osStopCOM(Scalar ShutdownMode);
#else
osqFunc1 StatusType osqFunc2 osStopCOM(void);
#endif

osqFunc1 void osqFunc2 osInternalStartCOM(void);

osqFunc1 StatusType osqFunc2 osSendMessage(SymbolicName pMsg, AccessNameRef pData);
osqFunc1 StatusType osqFunc2 osReceiveMessage(SymbolicName pMsg, AccessNameRef pData);
osqFunc1 StatusType osqFunc2 osGetMessageResource(SymbolicName pMsg);
osqFunc1 StatusType osqFunc2 osReleaseMessageResource(SymbolicName pMsg);
osqFunc1 StatusType osqFunc2 osGetMessageStatus(SymbolicName pMsg);

#if osdNumberOfMsgs>0
#if osdCCC==osdCCCB
#if osdUseMessageResource 
osqFunc1 uint8 osqFunc2 osComResourcesAreTaken(void);
#endif
#endif
#endif

#if osdNumberOfMsgsFlagNotifics
osqFunc1 FlagValue osqFunc2 osReadFlag(FlagType FlagName);
osqFunc1 StatusType osqFunc2 osResetFlag(FlagType FlagName);
extern osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[osdNumberOfMsgsFlagNotifics];
#define ReadFlag(x) (osReadFlag(x))
#define ResetFlag(x) (osResetFlag(x))
#endif


#define StartCOM osStartCOM
#if osdNumberOfMsgs>0
#define StopCOM(x) (osStopCOM(x))
#else
#define StopCOM(x) (osStopCOM())
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#define SendMessage(x, y) (osSendMessage((x), (y)))
#define ReceiveMessage(x, y) (osReceiveMessage((x), (y)))
#define GetMessageResource(x) (osGetMessageResource(x))
#define ReleaseMessageResource(x) (osReleaseMessageResource(x))
#define GetMessageStatus(x) (osGetMessageStatus(x))


/* used to return E_OK always */
osqFunc1 StatusType osqFunc2 osReturnEOK(void);

/* dummy macros for InitCOM and CloseCOM functions which are not relevant
   for intertask communication */
#define InitCOM() osReturnEOK()
#define CloseCOM() osReturnEOK()


/* message definitions generated by OIL-tool - part 2 */

#ifdef osdIncludeLibConfig

   #if defined USE_QUOTE_INCLUDES
    #include "msglib1.h" /* (not variant dependant) */
   #else
    #include <msglib1.h>
   #endif

   #if (osdLibVariant == 1)
   #endif
   
   #if (osdLibVariant == 2)
   #endif
   
   #if (osdLibVariant == 3)
   #endif
   
   #if (osdLibVariant == 4)
   #endif

   #if ((osdLibVariant < 1) || (osdLibVariant > 4))
   #error unknown library variant/configuration file
   #endif

#else /*osdIncludeLibConfig*/

   #include "msg.h"

#endif /*osdIncludeLibConfig*/

/*lint -restore re-enable messages*/

#endif /* double include preventer */
