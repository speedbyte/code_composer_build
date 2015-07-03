/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekcom.c 
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.93
|
|  Description: OSEK-COM 2.2 conformant inter-task communication
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* next define is set in the makefile
 * switches off the include of (generated) tcb.h, includes instead
 * the library config file
 */

#ifdef osdGenerateLib
#define osdIncludeLibConfig 1
#endif

#if defined USE_QUOTE_INCLUDES
 #include "osek.h"
#else
 #include <osek.h>
#endif

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

/* CovComment 0:  file: osekcom.c */

#if defined USE_QUOTE_INCLUDES
 #include "osekext.h"
#else
 #include <osekext.h>
#endif

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

#if osdNumberOfMsgs>0

/* KB begin comHWMemCopy */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif

osqFunc1 void osqFunc2 osMemCpy2(uint8 *dest, uint8 *src, uint32 count);
osqFunc1 void osqFunc2 osMemCpy2(uint8 *dest, uint8 *src, uint32 count)
{
   /* CovComment 4:  Function: osMemCpy2 */
   while (count)
   {
      *dest++ = *src++;
      count--;
      COVER(0x0373)
   }
   COVER(0x0374)
}
#define osMemCpy(x, y, z) osMemCpy2((uint8 *) x, (uint8 *) y, (uint32)z)

#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif

/* KB end comHWMemCopy */

#if osdCCC==osdCCCB

/* support function to determine the receiver queue */
/* contains operating system dependencies */
static osqFunc1 osqPROM1 osqPROM2 osComMsgQueue osqPROM3 * osqFunc2 osGetReceiverQueue(SymbolicName pMsg);

static osqFunc1 osqPROM1 osqPROM2 osComMsgQueue osqPROM3 * osqFunc2 osGetReceiverQueue(SymbolicName pMsg)
{
   /* CovComment 4:  Function: osGetReceiverQueue */
   uint16 i;
   uint8 found=0;
   osqPROM1 osqPROM2 osComMsgQueue osqPROM3 * pQ;

   COVER(0x0375)
   /* KB begin osekHWcomGetRecInitVariables */
   /* KB end osekHWcomGetRecInitVariables */

   pQ=pMsg->dataPtr.queue;

   /* determine the receiver using the operating system */
   if(pMsg->rcvNum>1)
   {
      COVER(0x0376)
      #if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions)
      COVER(0x0377)
      if(osIntNestingDepth>0)
      {
         COVER(0x0378)
         ShutdownOS(E_COM_SYS_NOT_SUPPORTED);
      }
      else
      #endif
      {
         COVER(0x0379)
         /* call on task level - find active task in task list */
         for(i=0;i<pMsg->rcvNum;i++)
         {
            COVER(0x037A)
            if(((pMsg->taskList)+i)->task==osActiveTaskIndex)
            {
               COVER(0x037B)
               found=1;
               break;
            }
            ELSEWITHCOVER(0x037C)
            pQ++;
         }
      }
      if(found==0)
      {
         COVER(0x037D)
         return (osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)osdNULL;
      }
      ELSEWITHCOVER(0x037E)
      COVER(0x037F)
   }
   ELSEWITHCOVER(0x0380)

   /* return found queue */
   return pQ;
}
#endif /* osdCCCB */

#endif /* osdNumberOfMsgs */

/*****************************************************************************
Service name: StartCOM
Syntax: StatusType StartCOM (void)
Parameter(In): none
Parameter(Out): none
Description: The StartCOM service starts the OSEK communication module. This
routine shall perform the initialisation of OSEK COM implementation
specific internal states and variables. StartCOM may call the
MessageInit function provided by the application programmer if the
latter is used to initialise the application specific message objects.
If StartCOM or one of the routines called fail then initialisation of the
OSEK COM module shall abort and StartCOM shall return a status
code as specified below.
StartCOM must be called from within a task if an OSEK-compliant
operating system is used.
Caveats: InitCOM must be called to initialise hardware before StartCOM can
be called. Failure to call InitCOM before StartCOM will result in
undefined behaviour.
Status:
Standard and Extended:
- A1 : The service shall return E_OK if the initialisation completed
       successfully.
- A2 : The service shall return an implementation or application
       specific error code if the initialisation did not complete 
       successfully.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osStartCOM(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osStartCOM */
#if osdNumberOfMsgs>0
   StatusType retval;
#if osdNumberOfMsgsFlagNotifics
   uint8 i;
#endif      

   OS_SC_ENTRY()
   COVER(0x0381)

   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_SC_START_CRITICAL()

#if osdNumberOfMsgsFlagNotifics
   COVER(0x0382)
   for(i=0;i<osrNumberOfMsgsFlagNotifics;i++)
   {
      COVER(0x0383)
      osComFlags[i]=0;
   } 
#endif      
   /* internal message initialisation - generated automatically */
   osInternalStartCOM();

   retval=MessageInit();

   OS_SC_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   OS_SC_EXIT()

   return retval;
#else
   OS_SC_ENTRY()
   COVER(0x0384)
   OS_SC_EXIT()
   return E_OK;
#endif
}

/*****************************************************************************
Service name: StopCOM
Syntax: StatusType StopCOM (Scalar <ShutdownMode>)
Input Parameters: ShutdownMode:
COM_SHUTDOWN_IMMEDIATE
The shutdown will occur immediately without waiting for pending
operations to complete.
Output Parameters: none
Description: This services causes all OSEK COM activity to cease and all
resources used by COM to be returned or left in an inactive state. All
operations will cease immediately and hardware will be de-initialised
as appropriate. By implication, data will be lost. StopCOM will not
return until all pending COM operations have completed and
their resources can be released.
When StopCOM has completed successfully the system shall be left
in a state in which StartCOM can be called to re-initialise OSEK
COM.
Particularities : None.
Status:
Standard and Extended:
- A1 : The service shall return E_OK if OSEK COM was shut
       down successfully
- A2 : The service shall return E_COM_BUSY if OSEK COM could
       not shut down because an application (task) is currently using a
       resource owned by OSEK COM.
*****************************************************************************/

#if osdNumberOfMsgs>0
osqFunc1 StatusType osqFunc2 osStopCOM(Scalar ShutdownMode)  /* PRQA S 1503 */ /* API functions might be never called */
#else
osqFunc1 StatusType osqFunc2 osStopCOM(void)  /* PRQA S 1503 */ /* API functions might be never called */
#endif
{
   /* CovComment 4:  Function: osStopCOM */
#if osdNumberOfMsgs>0
   OS_TC_ENTRY()
   COVER(0x0385)
   if(ShutdownMode==COM_SHUTDOWN_IMMEDIATE)
   {
      COVER(0x0386)
#if osdCCC==osdCCCB
      COVER(0x0387)
#if osdUseMessageResource
      COVER(0x0388)
      if(osComResourcesAreTaken() != 0) /* automatically generated check function */
      {
         COVER(0x0389)
         OS_TC_EXIT()
         return E_COM_BUSY;
      }
      ELSEWITHCOVER(0x038A)
      COVER(0x038B)
#endif
#endif
      OS_TC_EXIT()
      return E_OK;
   }
   else
   {
      COVER(0x038C)
      OS_TC_EXIT()
      return E_COM_SYS_NOT_SUPPORTED;
   }
#else
   OS_TC_ENTRY()
   COVER(0x038D)
   OS_TC_EXIT()
   return E_OK;
#endif
}

/*****************************************************************************
Service name: MessageInit
Syntax: StatusType MessageInit (void)
Parameter(In): none
Parameter(Out): none
Description: This routine initialises all application specific message objects.
Particularities: This function has to be provided by the application programmer and
shall be called by the StartCOM routine only. Any other way to
initialise application specific message object is allowed provided it
does not hinder the functionality of the API services defined in this
specification.
Status:
Standard and Extended:
- A1 : The service shall return E_OK if the initialisation of the
       application specific message object has completed
       successfully.
- A2 : The service shall return an implementation or application
       specific error code if the initialisation did not complete
       successfully.
*****************************************************************************/

/* user specific message initialisation */
/* this function has to be provided by the application */
/* prototype, see header file */

#if osdNumberOfMsgs>0

/*****************************************************************************
Service name: SendMessage
Syntax: StatusType SendMessage (
SymbolicName <Message>,
AccessNameRef <Data>
)
Parameter (In):
Message Symbolic name of the message
Data Reference to the message data field to be transmitted
Parameter (Out): none
Description: The service shall update the message object identified by
<Message> depending on the message copy configuration and
request transmission of the message object depending on the
transmission mode specified. The service shall not verify whether the
message object has been initialised.
1. In case of WithCopy :
The service shall update the message object identified by
<Message> with the message copy referenced by the <Data>
parameter.
2. In case of WithoutCopy :
No update of the message object shall be performed since no
message copy is used to interface with the message object
identified by <Message>.
Particularities: None
Status:
Standard:
- A1 : The service shall return E_COM_LOCKED whenever the
       message object identified by <Message> is locked. This
       condition has higher precedence over all other conditions
       defined in section A2 to A3 of this section.
- A2 : The service shall return E_COM_LOCKED if the copy
       configuration of <Data> is WithCopy and the message is set
       BUSY.
- A3 : The service shall return E_OK if the service operation has
       completed successfully.
Extended:
- A1 to A3 status codes defined under the "Standard" section
       shall be supported.
- A4 : The service shall return E_COM_ID if the parameter
       <Message> is invalid.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osSendMessage(SymbolicName pMsg, AccessNameRef pData)  /* PRQA S 3673 */  /* ignore const argument warning */  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osSendMessage */
   uint16 i;
#if osdCCC==osdCCCB
   osqPROM1 osqPROM2 osComMsgQueue osqPROM3 * pQ;
#endif
   osqPROM1 const osqPROM2 osComNotification osqPROM3 * pNot;

   OS_SM_ENTRY()
   COVER(0x038E)

   #if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x038F)
   /* check message */
   if(pMsg->magic!=osdMAGIC_MSG)
   {
      COVER(0x0390)
      OS_SM_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x0391)
   COVER(0x0392)
   #endif

   /* lock message */
   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_SM_START_CRITICAL()

#if osdCCC==osdCCCB
   COVER(0x0393)
   if(pMsg->fifoSize==0)
   {
      COVER(0x0394)
#endif
      /* unqueued message */

      /* update message object */
      if(pMsg->sendWithoutCopy == 0)
      {
         COVER(0x0395)
#if osdCCC==osdCCCB
         COVER(0x0396)
#if osdUseMessageResource
         COVER(0x0397)

         /* check if message is BUSY in withcopy configuration */
         if(pMsg->ramPtrGen->resource!=0)
         {
            COVER(0x0398)
            /* KB begin osekHWosRestoreEnableLevelNested */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            OS_SM_EXIT()
            return E_COM_LOCKED;
         }
         ELSEWITHCOVER(0x0399)
         COVER(0x039A)
#endif
#endif
         (void) osMemCpy(pMsg->dataPtr.msg, pData, pMsg->msgLen);
      }
      ELSEWITHCOVER(0x039B)
#if osdCCC==osdCCCB
      COVER(0x039C)
   }
   else
   {
      COVER(0x039D)
      /* queued message */

      /* update message object */
      pQ=pMsg->dataPtr.queue;
      for(i=0;i<pMsg->rcvNum;i++)  /* transmit to all receivers */
      {
         COVER(0x039E)
         if(pMsg->fifoSize!=pQ->pQRam->msgCount)
         {
            COVER(0x039F)
            /* at least one empty message found */
            (void) osMemCpy(pQ->pQRam->w, pData, pMsg->msgLen);
            pQ->pQRam->msgCount++;
            pQ->pQRam->w=(DataRefType)((uint8*)(pQ->pQRam->w) + (pMsg->msgLen)); /* PRQA S 3305 */  /* might be casted to other alignment */
            if(pQ->pQRam->w > pQ->last)
            {
               COVER(0x03A0)
               pQ->pQRam->w=pQ->first;
            } 
            ELSEWITHCOVER(0x03A1)
            pQ->pQRam->status=E_OK;         
         }
         else
         {
            COVER(0x03A2)
            /* fifo full */
            pQ->pQRam->status=E_COM_LIMIT;         
         }
         pQ++;  /* go to next queue */
      }
   }

#endif

   COVER(0x03A3)
   /* unlock message */
   OS_SM_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   /* notification if required - go through list of activations */
   for(i=0;i<pMsg->numOfNotifications;i++)
   {
      COVER(0x03A4)
      /* go through list of activations */
      pNot=(osqPROM1 const osqPROM2 osComNotification osqPROM3 *)(&(pMsg->pNotification[i]));
      if(pNot->action==osdCOM_ACTION_TASK)
      {
         COVER(0x03A5)
         /* activate task */
         #if STATUS_LEVEL == EXTENDED_STATUS
         COVER(0x03A6)
         if(ActivateTask(pNot->task)!=E_OK)
         {
            COVER(0x03A7)
            OS_SM_EXIT()
            return E_COM_SYS_NOTIFY_FAILED;
         }
         ELSEWITHCOVER(0x03A8)
         COVER(0x03A9)
         #else
         COVER(0x03AA)
         (void) ActivateTask(pNot->task); 
         #endif
      }
#if (CC == ECC1) || (CC == ECC2)
#if (osdNumberOfExtendedTasks > 0)
      else if(pNot->action==osdCOM_ACTION_EVENT)
      {
         COVER(0x03AB)
         /* set event */
         #if STATUS_LEVEL == EXTENDED_STATUS
         COVER(0x03AC)
         if (SetEvent(pNot->task,pNot->event)!=E_OK)
         {
            COVER(0x03AD)
            OS_SM_EXIT()
            return E_COM_SYS_NOTIFY_FAILED;
         }
         ELSEWITHCOVER(0x03AE)
         COVER(0x03AF)
         #else
         COVER(0x03B0)
         (void) SetEvent(pNot->task,pNot->event); 
         #endif
      }
#endif
#endif
#if osdNumberOfMsgsCallbackNotifics
      else if(pNot->action==osdCOM_ACTION_CALLBACK)
      {
         COVER(0x03B1)
         /* call the callback function */
         pNot->callback();
      }
#endif
#if osdNumberOfMsgsFlagNotifics
      else /* if(pNot->action==osdCOM_ACTION_FLAG) */
      {
         COVER(0x03B2)
         osComFlags[pNot->flag]=TRUE;
      }
#else
      else 
      {
         COVER(0x03B3)
      }
#endif
   }

   COVER(0x03B4)
   OS_SM_EXIT()

   return E_OK;
}

/*****************************************************************************
Service name: ReceiveMessage
Syntax: StatusType ReceiveMessage (
SymbolicName <Message>,
AccessNameRef <Data>
)
Parameter (In):
Message Symbolic name of the message
Parameter (Out):
Data Reference to the message data field to store the received data
Description: The service shall deliver the message data associated with the
message object <Message> depending on the message copy
configuration.
1. In case of WithCopy :
The service shall update the message referenced by <Data>
with the message object identified by <Message>.
2. In case of WithoutCopy :
The service shall return only a service status since the
application accesses directly the message object.
The service shall return message data according to the behaviour
specified in the message chapter of the interaction layer.
Particularities: None
Status:
Standard:
- A1 : The service shall return E_COM_LOCKED whenever the
       message object identified by <Message> is locked. This
       condition has higher precedence over all other conditions
       defined in section A2 to A5 of this section.
- A2 : The service shall return E_COM_LOCKED if the copy
       configuration of <Data> is WithCopy and the message is set
       BUSY.
- A3 : The service shall return E_OK if data of a queued or
       unqueued message identified by <Message> is available and
       returned to the application successfully.
- A4 : The service shall return E_COM_NOMSG if the queued
       message identified by <Message> is empty.
- A5 : The service shall return E_COM_LIMIT if an overflow of
       the FIFO of the queued message identified by <Message>
       occurred since the last call to “Service_Name” for that particular
       <Message>, E_COM_LIMIT indicates that at least one queued
       message has been lost and discarded since the FIFO was full
       upon the queued message arrival.
Extended:
- A1 to A5 status codes defined under the "Standard" section
       shall be supported.
- A6 : The service shall return E_COM_ID if the parameter
       <Message> is invalid.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osReceiveMessage(SymbolicName pMsg, AccessNameRef pData)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osReceiveMessage */
#if osdCCC==osdCCCB
   osqPROM1 osqPROM2 osComMsgQueue osqPROM3 * pQ;
#endif
   uint8 i, receiveIsWithoutCopy=0; /* copy per default */

   OS_RM_ENTRY()
   COVER(0x03B5)

   #if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x03B6)
   /* check message */
   if(pMsg->magic!=osdMAGIC_MSG)
   {
      COVER(0x03B7)
      OS_RM_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x03B8)
   COVER(0x03B9)
   #endif

#if osdCCC==osdCCCB
   COVER(0x03BA)
   if(pMsg->fifoSize==0)
   {
      COVER(0x03BB)
#endif
      /* unqueued message */

      /* lock message */
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      OS_RM_START_CRITICAL()

      /* on interrupt level reception is always with copy */
      if(osIntNestingDepth==0)
      {
         COVER(0x03BC)
         /* call on task level - find active task in task list */
         for(i=0;i<pMsg->rcvNum;i++)
         {
            COVER(0x03BD)
            if(((pMsg->taskList)+i)->task==osActiveTaskIndex)
            {
               COVER(0x03BE)
               /* task found -> set flag and exit for loop */
               receiveIsWithoutCopy = ((pMsg->taskList)+i)->withoutCopy;
               break;
            }
            ELSEWITHCOVER(0x03BF)
         }
      }
      ELSEWITHCOVER(0x03C0)
      /* a message received by a task may not have to be copied */
      if(receiveIsWithoutCopy == 0)
      {
         COVER(0x03C1)
#if osdCCC==osdCCCB
         COVER(0x03C2)
#if osdUseMessageResource
         COVER(0x03C3)
         /* check if message is BUSY in withcopy configuration */
         if(pMsg->ramPtrGen->resource!=0)
         {
            COVER(0x03C4)
            OS_RM_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevelNested */
            osRestoreEnableLevelNested();
            /* KB end osekHWosRestoreEnableLevelNested */
            OS_RM_EXIT()
            return E_COM_LOCKED;
         }
         ELSEWITHCOVER(0x03C5)
         COVER(0x03C6)
#endif
#endif
         COVER(0x03C7)
         /* update message object  */
         (void) osMemCpy(pData, pMsg->dataPtr.msg, pMsg->msgLen);
      }
      ELSEWITHCOVER(0x03C8)
#if osdCCC==osdCCCB
   }
   else
   {
      COVER(0x03C9)
      /* queued message */

      /* lock message  */
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */

      /* update message object  */

      pQ=osGetReceiverQueue(pMsg);
      osSysErrAssert(pQ!=osdNULL, E_COM_SYS_QUEUE_NOT_FOUND)   /* PRQA S 0850 */  /* workaround for QAC problem, argument isn't empty */
      /* PRQA S 0505 L_osReceiveMessage1 */ /* pointer is validated by assertion */
      if(pQ->pQRam->msgCount != 0)
      {
         COVER(0x03CA)
         /* message present */
         (void) osMemCpy(pData, pQ->pQRam->r, pMsg->msgLen);
         pQ->pQRam->msgCount--;
         pQ->pQRam->r=(DataRefType)((uint8*)(pQ->pQRam->r)+(pMsg->msgLen));  /* PRQA S 3305 */  /* might be casted to other alignment */
         if(pQ->pQRam->r > pQ->last)
         {
            COVER(0x03CB)
            pQ->pQRam->r=pQ->first;
         }
         ELSEWITHCOVER(0x03CC)
      }
      else
      {
         COVER(0x03CD)
         /* fifo empty */
         OS_RM_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         OS_RM_EXIT()
         return E_COM_NOMSG;
      }
      COVER(0x03CE)

      if(pQ->pQRam->status==E_COM_LIMIT)
      {
         COVER(0x03CF)
         /* message lost */
         if(pQ->pQRam->msgCount==0)
         {
            COVER(0x03D0)
            pQ->pQRam->status=E_COM_NOMSG;
         }
         else
         {
            COVER(0x03D1)
            pQ->pQRam->status=E_OK;
         }
         OS_RM_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         OS_RM_EXIT()
         return E_COM_LIMIT;
      }
      ELSEWITHCOVER(0x03D2)
      COVER(0x03D3)
      if(pQ->pQRam->msgCount==0)
      {
         COVER(0x03D4)
         pQ->pQRam->status=E_COM_NOMSG;
      }
      ELSEWITHCOVER(0x03D5)
      /* PRQA L:L_osReceiveMessage1 */ 
   }
#endif   
   COVER(0x03D6)
   /* unlock message  */
   OS_RM_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   OS_RM_EXIT()

   return E_OK;
}

#if osdCCC==osdCCCB

#if osdUseMessageResource

/*****************************************************************************
Service name: GetMessageResource
Syntax: StatusType GetMessageResource (
SymbolicName <Message>
)
Parameter (In):
Message Symbolic name of the message object
Parameter (Out): none
Description: The service GetMessageResource shall set the message object
<Message> status as busy.
Particularities: It is recommended that corresponding calls to Get- and
ReleaseMessageResource should appear within the same function
on the same function level. Before terminating the task or entering
the wait state the corresponding service ReleaseMessageResource
shall be called by the application layer.
This service can only be used to support the transfer of a message
identified by <Message> whose copy configuration is "WithoutCopy".
Status:
Standard:
- A1 : The service shall return E_COM_LOCKED whenever the
       message identified by <Message> is locked. This condition has
       higher precedence over all other conditions defined in section
       A2 to A3 of this section.
- A2 : The service shall return E_OK if the message identified by
       <Message> has been set to BUSY successfully.
- A3 : The service shall return E_COM_BUSY if the message
       identified by <Message> is already set to BUSY.
Extended:
- A1 to A3 status codes defined under the "Standard" section
       shall be supported.
- A4 : The service shall return E_COM_ID if the <Message>
       parameter is invalid.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osGetMessageResource(SymbolicName pMsg)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetMessageResource */
   OS_GG_ENTRY()
   COVER(0x03D7)

   #if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x03D8)
   /* check message */
   if(pMsg->magic!=osdMAGIC_MSG)
   {
      COVER(0x03D9)
      OS_GG_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x03DA)
   COVER(0x03DB)
   #endif

   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_GG_START_CRITICAL()

   if(pMsg->ramPtrGen->resource != 0)
   {
      COVER(0x03DC)
      OS_GG_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      OS_GG_EXIT()
      return E_COM_BUSY;
   }
   else
   {
      COVER(0x03DD)
      pMsg->ramPtrGen->resource=1;
      OS_GG_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      OS_GG_EXIT()
      return E_OK;
   }
}

/*****************************************************************************
Service name: ReleaseMessageResource
Syntax: StatusType ReleaseMessageResource (
SymbolicName <Message>
)
Parameter (In):
Message Symbolic name of the message object
Parameter (Out): none
Description: The service ReleaseMessageResource shall unconditionally set the
message object <Message> to NOT_BUSY.
Particularities: It is recommended that corresponding calls to Get- and
ReleaseMessageResource appear within the same function on the
same function level. Before terminating the task or entering the wait
state the corresponding service ReleaseMessageResource shall be
used.
This service can only be used to support the transfer of a message
identified by <Message> whose copy configuration is "WithoutCopy".
Status:
Standard:
- A1 : The service shall return E _OK after the message has
       been set to NOT_BUSY.
Extended:
- A1 : status code defined under the "Standard" section shall be
       supported.
- A2 : The service shall return E_COM_ID if the <Message>
       parameter is invalid.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osReleaseMessageResource(SymbolicName pMsg)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osReleaseMessageResource */
   OS_RG_ENTRY()
   COVER(0x03DE)
   #if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x03DF)
   /* check message */
   if(pMsg->magic!=osdMAGIC_MSG)
   {
      COVER(0x03E0)
      OS_RG_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x03E1)
   COVER(0x03E2)
   #endif

   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_RG_START_CRITICAL()
   pMsg->ramPtrGen->resource=0;
   OS_RG_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   OS_RG_EXIT()
   return E_OK;
}
#endif /* osdUseMessageResource */

#if osdUseMessageStatus

/*****************************************************************************
Service name: GetMessageStatus
Syntax: StatusType GetMessageStatus (
SymbolicName <Message>
)
Parameter (In):
Message Symbolic name of the message object
Parameter (Out): none
Description: The service GetMessageStatus shall return the current status of the
message object <Message>. If this service call fails, it shall return an
implementation specific error code that shall be distinguishable from
all other return values.
Particularities: none
Status:
Standard:
- A1 : The service shall return E_COM_LOCKED whenever the
       message identified by <Message> is locked. This condition has
       higher precedence over all other conditions defined in section
       A2 to A6 this section.
- A2 : The service shall return E_COM_BUSY if the message is
       currently set to BUSY.
- A3 : The service shall return E_COM_NOMSG if the FIFO of
       the queued message identified by <Message> is empty.
- A4 : The service shall return E_COM_LIMIT if an overflow of
       the FIFO of the queued message identified by <Message>
       occurred.
- A5 : The service shall return E_OK if none of the conditions
       specified in A1 to A6 of this section is applicable nor fulfilled
       and no indication of error is present.
Extended:
- A1 to A5 status codes defined under the "Standard" section
       shall be supported.
- A7 : The service shall return E_COM_ID if the <Message>
       parameter is invalid.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osGetMessageStatus(SymbolicName pMsg)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetMessageStatus */
   uint16 i;
   uint16 empty=0;
   osqPROM1 osqPROM2 osComMsgQueue osqPROM3 * pQ;
   
   OS_MS_ENTRY()
   COVER(0x03E3)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x03E4)
   /* check message */
   if(pMsg->magic!=osdMAGIC_MSG)
   {
      COVER(0x03E5)
      OS_MS_EXIT()
      return E_COM_ID;
   }
   ELSEWITHCOVER(0x03E6)
   COVER(0x03E7)
#endif

#if osdUseMessageResource
   COVER(0x03E8)
   if(pMsg->ramPtrGen->resource != 0)
   {
      COVER(0x03E9)
      OS_MS_EXIT()
      return E_COM_BUSY;
   }
   ELSEWITHCOVER(0x03EA)
   COVER(0x03EB)
#endif

   if(pMsg->fifoSize>=1)
   {
      COVER(0x03EC)
      /* queued message */
      pQ=osGetReceiverQueue(pMsg);

      if(pQ != 0)
      {
         COVER(0x03ED)
         /* called by receiver */
   
         /* check if message is available */
         if(pQ->pQRam->status==E_COM_NOMSG)
         {
            COVER(0x03EE)
            OS_MS_EXIT()
            return E_COM_NOMSG;
         }
         ELSEWITHCOVER(0x03EF)
         COVER(0x03F0)

         /* check all queues for overflow */
         if(pQ->pQRam->status==E_COM_LIMIT)
         {
            COVER(0x03F1)
           OS_MS_EXIT()
           return E_COM_LIMIT;
         }
         ELSEWITHCOVER(0x03F2)
         COVER(0x03F3)
      }
      else
      {
         COVER(0x03F4)
         /* called by sender or somebody else */

         pQ=pMsg->dataPtr.queue;
     
         /* loop through all queues */
         for(i=0;i<pMsg->rcvNum;i++)
         {
            COVER(0x03F5)
            if(pQ->pQRam->status==E_COM_LIMIT)
            {
               COVER(0x03F6)
               OS_MS_EXIT()
               return E_COM_LIMIT; /* at least one queue overflow */
            }
            ELSEWITHCOVER(0x03F7)
            COVER(0x03F8)
            if(pQ->pQRam->status==E_COM_NOMSG)
            {
               COVER(0x03F9)
               empty++; /* queue empty */
            }
            ELSEWITHCOVER(0x03FA)
            pQ++;
         }
         if(empty==pMsg->rcvNum)
         {
            COVER(0x03FB)
            OS_MS_EXIT()
            return E_COM_NOMSG;  /* all queues are empty */
         }
         ELSEWITHCOVER(0x03FC)
         COVER(0x03FD)
      }
   }
   ELSEWITHCOVER(0x03FE)
   COVER(0x03FF)
   OS_MS_EXIT()
   return E_OK;
}
#endif /* osdUseMessageStatus */

#endif /* osdCCCB */

#if osdNumberOfMsgsFlagNotifics


/*****************************************************************************
Service name: ReadFlag
Syntax: FlagValue ReadFlag (FlagType FlagName)
Parameter(In):
FlagName Message flag name
Parameter(Out):
FlagValue State of the flag <FlagName>
Description: This service returns the value of the specified notification flag
<FlagName>.
Particularities: This service is provided so that appropriate mechanism can be
implemented to ensure flag data consistency whilst enabling portable
access to the message notification flag. The flag meaning depends
to which notification class the specified flag <FlagName> is
associated with, eg Flag associated with Notification class 1 and set
at TRUE indicates that a message has arrived.
Status:
Standard and Extended::
- A1 : The service shall return TRUE if the conditions associated
       to the notification class to which the flag is associated are met.
- A2 : The service shall return FALSE if the conditions associated
       to the notification class to which the flag is associated are not
       met.
*****************************************************************************/

osqFunc1 FlagValue osqFunc2 osReadFlag(FlagType FlagName)
{
   /* CovComment 4:  Function: osReadFlag */
   OS_RF_ENTRY()
   COVER(0x0400)
   OS_RF_EXIT()
   return osComFlags[FlagName];
}

/*****************************************************************************
Service name: ResetFlag
Syntax: StatusType ResetFlag (FlagType FlagName)
Parameter(In):
FlagName Message flag name
Parameter(Out): none
Description: This service set the specified notification flag <FlagName> to FALSE.
Particularities: This service is provided so that appropriate mechanism can be
implemented to ensure flag data consistency whilst enabling portable
access to the message notification flag.
Status:
Standard and Extended:
- A1 : The service shall return E_OK if the flag reset completed
       successfully.
- A2 : The service shall return an implementation or application
       specific error code if the initialisation did not complete
       successfully.
*****************************************************************************/

osqFunc1 StatusType osqFunc2 osResetFlag(FlagType FlagName)
{
   /* CovComment 4:  Function: osResetFlag */
   OS_ZF_ENTRY()
   COVER(0x0401)
   osComFlags[FlagName]=FALSE;
   OS_ZF_EXIT()
   return E_OK;
}

#endif

#endif /* osdNumberOfMsgs */

/* used to return E_OK always */
osqFunc1 StatusType osqFunc2 osReturnEOK(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osReturnEOK */
   COVER(0x0402)
   return E_OK;
}


/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */
