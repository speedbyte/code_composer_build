/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekevnt.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.34
|
|  Description: osek API-functions for handling events
|               - SetEvent
|               - ClearEvent
|               - GetEvent
|               - WaitEvent
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2007 Vector Informatik GmbH               All rights reserved.
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
#if defined USE_QUOTE_INCLUDES
 #include "osekext.h"
#else
 #include <osekext.h>
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

/* CovComment 0:  file: osekevnt.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

#if ((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0)

/*-+--------------------------------------------------------------------------
 / SetEvent
 / ========          
 /
 / Syntax:          StatusType SetEvent ( TaskType <TaskID>
 /                                        EventMaskType <Mask> )
 / Parameter (In):  - TaskID Reference to the task for which one or 
 /                    several events are to be set.
 /                  - Mask Mask of the events to be set
 / Parameter (Out): none
 / Description:     The service might be called from an interrupt service 
 /                  routine and from the task level, but not from hook 
 /                  routines.
 /                  The events of task <TaskID> are set according to the 
 /                  event mask <Mask>. Calling SetEvent causes the task 
 /                  <TaskID> to be transferred to the ready state, if it
 /                  has been waiting for at least one of the events 
 /                  specified in <Mask>.
 / Particularities: Any events not set in the event mask remain unchanged.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Referenced task is no extended task, E_OS_ACCESS
 /                  - Events can not be set as the referenced task is in the
 /                    suspended state, E_OS_STATE
 / Conformance:     ECC1, ECC2
 / Context:         Task, Cat2-ISR, interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

#ifdef osdSysAPI
#undef osdSysAPI
#endif

#ifndef osdSetEventNotUsed
osqFunc1 StatusType osqFunc2 osSetEvent(TaskType taskIndex, EventMaskType setMask)
{
   /* CovComment 4:  Function: osSetEvent */
/* KB begin setEventBody */

   osPrioType priority;

   OS_SE_ENTRY()
   COVER(0x0000)

#ifndef osdSysAPI
   COVER(0x0001)
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0002)
   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x0003)
      #if osdErrorHook 
      COVER(0x0004)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ID, osdErrSEWrongTaskID);
      #if osdErrorHook 
      COVER(0x0005)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0006)

#if osdNumberOfExtendedTasks != osdNumberOfAllTasks
   COVER(0x0007)
   if (taskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x0008)
      #if osdErrorHook 
      COVER(0x0009)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSENotExtendedTask);
      #if osdErrorHook 
      COVER(0x000A)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x000B)
#endif

#if osdEnableAssertions
   COVER(0x000C)
   if (osIntAPIStatus != 0)
   {
      COVER(0x000D)
      #if osdErrorHook 
      COVER(0x000E)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrSEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x000F)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0010)
#endif
#endif   /* STATUS_LEVEL == EXTENDED_STATUS */

   COVER(0x0011)
   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_SE_START_CRITICAL()
#endif   /* osdSysAPI */

   COVER(0x0012)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0013)
   if (osTcbTaskState[taskIndex] == SUSPENDED)
   {
      COVER(0x0014)
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      osAPIError(E_OS_STATE, osdErrSETaskSuspended);
#ifndef osdSysAPI
      COVER(0x0015)
      OS_SE_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
#endif
      OS_SE_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x0016)
   COVER(0x0017)
#endif

   COVER(0x0018)
   osTcbSetEventMask[taskIndex]|= setMask; /* set event flag(s)    */

   if (osTcbTaskState[taskIndex] == WAITING)
   {
      COVER(0x0019)
      /* task is waiting */
      if ((osTcbSetEventMask[taskIndex] & osTcbWaitEventMask[taskIndex]) != 0)
      {
         COVER(0x001A)
         /*  satisfied, change from waiting to ready */

         /* get priority of continued task */
         priority = oskTcbHomePrio[taskIndex];
#if (osdNumberOfPriorities > 0)
         COVER(0x001B)
         osSysErrAssert((priority < osrNumberOfPriorities), osdErrSEWrongTaskPrio)
#endif

         osTcbTaskState[taskIndex] = READY;
         
         /* KB begin osQInsertAtPrio */
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x001C)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; /* PRQA S 3689 */ /* index is validated by assertion */
         #else
         COVER(0x001D)
         osQReadyTask[priority]= taskIndex;   /* PRQA S 3689 */ /* index is validated by assertion */
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x001E)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x001F)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0020)
         osQReadyTaskTail[priority]++;   /* PRQA S 3689 */ /* index is validated by assertion */
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])  /* PRQA S 3689 */ /* index is validated by assertion */
         {
            COVER(0x0021)
            osQReadyTaskTail[priority]= 0;   /* PRQA S 3689 */ /* index is validated by assertion */
         }
         #endif
         
         
         /* KB end osQInsertAtPrio */

         if (priority < osHighReadyTaskPrio)
         {
            COVER(0x0022)
            osHighReadyTaskPrio   = priority;
            osHighReadyTaskIndex  = taskIndex;
            osStartDispatcher= TRUE;

#ifndef osdSysAPI
            COVER(0x0023)
            OS_SE_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel */
            osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
            /* KB end osekHWosRestoreEnableLevel */
#endif
#if osdFullPreempt || osdMixedPreempt
            COVER(0x0024)
            if (osLockDispatcher == 0)
            {
               COVER(0x0025)
               OS_START_DISPATCH()
               OS_SE_EXIT()
               /* KB begin osekHWosDispatcher */
               osDispatch();
               /* KB end osekHWosDispatcher */
               return E_OK;
            }
            ELSEWITHCOVER(0x0026)
            COVER(0x0027)
#endif
            OS_SE_EXIT()
            return E_OK;
         }
         ELSEWITHCOVER(0x0028)
      }
      ELSEWITHCOVER(0x0029)
   }
   ELSEWITHCOVER(0x002A)
#ifndef osdSysAPI
   COVER(0x002B)
   OS_SE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */
#endif
   OS_SE_EXIT()
   return E_OK;


/* KB end setEventBody */
} /* END OF osSetEvent */
#endif


#if (osdNumberOfAlarmSetEvent  > 0)
#define osdSysAPI
osqFunc1 StatusType osqFunc2  osSysSetEvent(TaskType taskIndex, EventMaskType setMask)
{
   /* CovComment 4:  Function: osSysSetEvent */
/* KB begin setEventBody */

   osPrioType priority;

   OS_SE_ENTRY()
   COVER(0x002C)

#ifndef osdSysAPI
   COVER(0x002D)
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x002E)
   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x002F)
      #if osdErrorHook 
      COVER(0x0030)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ID, osdErrSEWrongTaskID);
      #if osdErrorHook 
      COVER(0x0031)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0032)

#if osdNumberOfExtendedTasks != osdNumberOfAllTasks
   COVER(0x0033)
   if (taskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x0034)
      #if osdErrorHook 
      COVER(0x0035)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrSENotExtendedTask);
      #if osdErrorHook 
      COVER(0x0036)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0037)
#endif

#if osdEnableAssertions
   COVER(0x0038)
   if (osIntAPIStatus != 0)
   {
      COVER(0x0039)
      #if osdErrorHook 
      COVER(0x003A)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrSEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x003B)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SE_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x003C)
#endif
#endif   /* STATUS_LEVEL == EXTENDED_STATUS */

   COVER(0x003D)
   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_SE_START_CRITICAL()
#endif   /* osdSysAPI */

   COVER(0x003E)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x003F)
   if (osTcbTaskState[taskIndex] == SUSPENDED)
   {
      COVER(0x0040)
      osSaveSetEvent_TaskID(taskIndex)
      osSaveSetEvent_Mask(setMask)
      osAPIError(E_OS_STATE, osdErrSETaskSuspended);
#ifndef osdSysAPI
      COVER(0x0041)
      OS_SE_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
#endif
      OS_SE_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x0042)
   COVER(0x0043)
#endif

   COVER(0x0044)
   osTcbSetEventMask[taskIndex]|= setMask; /* set event flag(s)    */

   if (osTcbTaskState[taskIndex] == WAITING)
   {
      COVER(0x0045)
      /* task is waiting */
      if ((osTcbSetEventMask[taskIndex] & osTcbWaitEventMask[taskIndex]) != 0)
      {
         COVER(0x0046)
         /*  satisfied, change from waiting to ready */

         /* get priority of continued task */
         priority = oskTcbHomePrio[taskIndex];
#if (osdNumberOfPriorities > 0)
         COVER(0x0047)
         osSysErrAssert((priority < osrNumberOfPriorities), osdErrSEWrongTaskPrio)
#endif

         osTcbTaskState[taskIndex] = READY;
         
         /* KB begin osQInsertAtPrio */
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x0048)
         oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; /* PRQA S 3689 */ /* index is validated by assertion */
         #else
         COVER(0x0049)
         osQReadyTask[priority]= taskIndex;   /* PRQA S 3689 */ /* index is validated by assertion */
         #endif
         
         #if (osdRTSize > 1)
         COVER(0x004A)
         osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
         #else
         COVER(0x004B)
         osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
         #endif
         
         
         #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x004C)
         osQReadyTaskTail[priority]++;   /* PRQA S 3689 */ /* index is validated by assertion */
         if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])  /* PRQA S 3689 */ /* index is validated by assertion */
         {
            COVER(0x004D)
            osQReadyTaskTail[priority]= 0;   /* PRQA S 3689 */ /* index is validated by assertion */
         }
         #endif
         
         
         /* KB end osQInsertAtPrio */

         if (priority < osHighReadyTaskPrio)
         {
            COVER(0x004E)
            osHighReadyTaskPrio   = priority;
            osHighReadyTaskIndex  = taskIndex;
            osStartDispatcher= TRUE;

#ifndef osdSysAPI
            COVER(0x004F)
            OS_SE_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel */
            osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
            /* KB end osekHWosRestoreEnableLevel */
#endif
#if osdFullPreempt || osdMixedPreempt
            COVER(0x0050)
            if (osLockDispatcher == 0)
            {
               COVER(0x0051)
               OS_START_DISPATCH()
               OS_SE_EXIT()
               /* KB begin osekHWosDispatcher */
               osDispatch();
               /* KB end osekHWosDispatcher */
               return E_OK;
            }
            ELSEWITHCOVER(0x0052)
            COVER(0x0053)
#endif
            OS_SE_EXIT()
            return E_OK;
         }
         ELSEWITHCOVER(0x0054)
      }
      ELSEWITHCOVER(0x0055)
   }
   ELSEWITHCOVER(0x0056)
#ifndef osdSysAPI
   COVER(0x0057)
   OS_SE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */
#endif
   OS_SE_EXIT()
   return E_OK;


/* KB end setEventBody */
} /* END OF osSysSetEvent */
#endif


#ifndef osdClearEventNotUsed
/*-+--------------------------------------------------------------------------
 / ClearEvent
 / =========
 /
 / Syntax: StatusType ClearEvent ( EventMaskType <Mask> )
 / Parameter (In):  Mask Mask of the events to be cleared
 / Parameter (Out): none
 / Description:     The events of the extended task calling ClearEvent are 
 /                  cleared according to the event mask <Mask>.
 / Particularities: The system service ClearEvent is restricted to extended 
 /                  tasks which own the event.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Call not from extended task, E_OS_ACCESS
 /                  - Call at interrupt level, E_OS_CALLEVEL
 / Conformance:     ECC1, ECC2
 / Context:         Extended task, interrupts enabled
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osClearEvent (EventMaskType Mask )
{
   /* CovComment 4:  Function: osClearEvent */
   COVER(0x0058)
   OS_CE_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0059)
   if (osIntNestingDepth != 0)
   {
      COVER(0x005A)
      #if osdErrorHook 
      COVER(0x005B)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrCEOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x005C)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x005D)

   if (osActiveTaskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x005E)
      #if osdErrorHook 
      COVER(0x005F)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrCENotExtendedTask);
      #if osdErrorHook 
      COVER(0x0060)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0061)

#if osdEnableAssertions
   COVER(0x0062)
   if (osIntAPIStatus != 0)
   {
      COVER(0x0063)
      #if osdErrorHook 
      COVER(0x0064)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveClearEvent_Mask(Mask)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrCEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x0065)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_CE_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0066)
#endif

#endif

   COVER(0x0067)
   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_CE_START_CRITICAL()
   osTcbSetEventMask[osActiveTaskIndex]&= ((EventMaskType)(~Mask));
   OS_CE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */

   COVER(0x0068)
   OS_CE_EXIT()
   return E_OK;
} /* END OF osClearEvent */
#endif


#ifndef osdGetEventNotUsed
/*-+--------------------------------------------------------------------------
 / GetEvent
 / ========
 /
 / Syntax:          StatusType GetEvent ( TaskType <TaskID>, 
 /                                        EventMaskRefType <Event> )
 / Parameter (In):  TaskID Task whose event mask is to be returned.
 / Parameter (Out): Event Reference to the memory of the return data.
 / Description:     This service returns the current state of all event bits 
 /                  of the task <TaskID>, not the events that the task is 
 /                  waiting for.
 /                  The service might be called from interrupt service 
 /                  routines, task level and some hook routines.
 /                  The current status of the event mask of task <TaskID> is
 /                  copied to <Event>.
 / Particularities: The referenced task must be an extended task.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Task <TaskID> is invalid, E_OS_ID
 /                  - Referenced task <TaskID> is not an extended task, 
 /                    E_OS_ACCESS
 /                  - Referenced task <TaskID> is in the suspended state, 
 /                    E_OS_STATE
 / Conformance:     ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook,
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osGetEvent(TaskType taskIndex, EventMaskRefType Event)
{
   /* CovComment 4:  Function: osGetEvent */
   OS_GE_ENTRY()
   COVER(0x0069)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x006A)
   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x006B)
      #if osdErrorHook 
      COVER(0x006C)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_ID, osdErrGEWrongTaskID);
      #if osdErrorHook 
      COVER(0x006D)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x006E)
   COVER(0x006F)

#if osdNumberOfExtendedTasks != osdNumberOfAllTasks
   COVER(0x0070)
   if (taskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x0071)
      #if osdErrorHook 
      COVER(0x0072)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGENotExtendedTask);
      #if osdErrorHook 
      COVER(0x0073)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0074)
#endif

   if (osTcbTaskState[taskIndex] == SUSPENDED)
   {
      COVER(0x0075)
      #if osdErrorHook 
      COVER(0x0076)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_STATE, osdErrGETaskSuspended);
      #if osdErrorHook 
      COVER(0x0077)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x0078)

#if osdEnableAssertions
   COVER(0x0079)
   if (osIntAPIStatus != 0)
   {
      COVER(0x007A)
      #if osdErrorHook 
      COVER(0x007B)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetEvent_TaskID(taskIndex)
      osSaveGetEvent_Event(Event)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrGEIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x007C)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GE_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x007D)
#endif
#endif

#ifndef osdEventMaskTypeSize
#error "osdEventMaskTypeSize not defined"
#endif

#if( osdEventMaskTypeSize > 1 )
   /* CovComment 2:  Config: EventMaskType > MachineWord */
   COVER(0x007E)
   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_GE_START_CRITICAL()
   /* CovComment 2:  */
#endif

   COVER(0x007F)
   *Event= osTcbSetEventMask[taskIndex];

#if( osdEventMaskTypeSize > 1 )
   /* CovComment 2:  Config: EventMaskType > MachineWord */
   COVER(0x0080)
   OS_GE_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
   /* CovComment 2:  */
#endif

   COVER(0x0081)
   OS_GE_EXIT()
   return E_OK;
} /* END OF osGetEvent */
#endif


#ifndef osdWaitEventNotUsed
/*-+--------------------------------------------------------------------------
 / WaitEvent
 / =========
 /
 / Syntax:          StatusType WaitEvent ( EventMaskType <Mask> )
 / Parameter (In):  Mask Mask of the events waited for.
 / Parameter (Out): none
 / Description:     The state of the calling task is set to waiting, unless 
 /                  at least one of the events specified in <Mask> has 
 /                  already been set.
 / Particularities: This call enforces rescheduling, if the wait condition 
 /                  occurs. If rescheduling takes place, the internal 
 /                  resource of the task is released while the task is in 
 /                  the waiting state.
 /                  This service must only be called from the extended task 
 /                  owning the event.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Calling task is not an extended task, E_OS_ACCESS
 /                  - Calling task occupies resources, E_OS_RESOURCE
 /                  - Call at interrupt level, E_OS_CALLEVEL
 / Conformance: ECC1, ECC2
 / Context:         Extended task, interrupts enabled, no resources occupied
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osWaitEvent(EventMaskType mask)
{
   /* CovComment 4:  Function: osWaitEvent */
   COVER(0x0082)
#if osdEnableContextOverlay
   COVER(0x0083)
   /* KB begin osekHWosSaveReturnAddress */
   /* KB end osekHWosSaveReturnAddress */
#endif

   OS_WE_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0084)
   if (osIntNestingDepth != 0)
   {
      COVER(0x0085)
      #if osdErrorHook 
      COVER(0x0086)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrWEOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x0087)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0088)

   if (osActiveTaskIndex >= osrNumberOfExtendedTasks)
   {
      COVER(0x0089)
      #if osdErrorHook 
      COVER(0x008A)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_ACCESS, osdErrWENotExtendedTask);
      #if osdErrorHook 
      COVER(0x008B)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x008C)

#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   COVER(0x008D)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
#else
   COVER(0x008E)
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
#endif
   {
      COVER(0x008F)
      #if osdErrorHook 
      COVER(0x0090)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_RESOURCE, osdErrWEResourcesOccupied);
      #if osdErrorHook 
      COVER(0x0091)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x0092)

#if osdEnableAssertions
   COVER(0x0093)
   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x0094)
      #if osdErrorHook 
      COVER(0x0095)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveWaitEvent_Mask(mask)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrWEInterruptsDisabled);
      #if osdErrorHook 
      COVER(0x0096)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_WE_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0097)
#endif

#if osdEnableProcedures
   COVER(0x0098)
#if osdEnableAssertions
   COVER(0x0099)
   if(oskTcbContextOverlay[osActiveTaskIndex]>0)
   {
      COVER(0x009A)
      osSysErrAssert(osIsInProcedure[osActiveTaskIndex]==FALSE, osdErrWEInContextOverlayProc)
   }
   ELSEWITHCOVER(0x009B)
#endif
#endif

#endif

   COVER(0x009C)
   /* KB begin osekHWosDisableLevel */
   osDisableLevel();    /* disable interrupts up to system level */
   /* KB end osekHWosDisableLevel */
   OS_WE_START_CRITICAL()

   osTcbWaitEventMask[osActiveTaskIndex]= mask;

   if ((osTcbSetEventMask[osActiveTaskIndex] & mask) == 0)
   {
      COVER(0x009D)
      /* no expected event set, set task into waiting state */

      /* call hook routine for the waiting task */
#if osdPostTaskHook
      COVER(0x009E)
      osPostTaskHook();
#endif

#if osdNumberOfInternalResources > 0
      COVER(0x009F)
      /* KB begin osReleaseInternalResource */
      if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
      {
         COVER(0x00A0)
      #if osdRTSize == 1
         COVER(0x00A1)
         osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
      #else
         COVER(0x00A2)
         osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]]
                       &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
      #endif
      #if ((CC == ECC2) || (CC == BCC2))
         COVER(0x00A3)
         osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
      #endif
      
      #if osdORTIDebug
         COVER(0x00A4)
         osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
      #endif
      
      }
      
      
      /* KB end osReleaseInternalResource */
#endif

      COVER(0x00A5)
      /* change task state from ready to waiting */
      osTcbTaskState[osActiveTaskIndex] = WAITING;

      /* KB begin osQDeleteFromPrio */
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x00A6)
      /* delete task out of the prio queue */
      osQReadyTaskHead[osActiveTaskPrio]++;
      if (osQReadyTaskHead[osActiveTaskPrio] > oskQMaxActivations[osActiveTaskPrio])
      {
         COVER(0x00A7)
         osQReadyTaskHead[osActiveTaskPrio]= 0;
      }
      
      if (osQReadyTaskHead[osActiveTaskPrio] == osQReadyTaskTail[osActiveTaskPrio])
      {  /* queue empty */
         COVER(0x00A8)
      #endif
      
      #if (osdRTSize > 1)
         COVER(0x00A9)
         osQReadyPrios[oskTcbReadyPrioOffset[osActiveTaskIndex]]&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
      #else
         COVER(0x00AA)
         osQReadyPrios&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
      #endif
      
      #if ((CC == ECC2) || (CC == BCC2))
      }
      #endif
      
      /* KB end osQDeleteFromPrio */

      /* for WaitEvent, osSchedulePrio returns always TRUE! */
      /* (same task is never running when reaching this point of code) */
      (void) osSchedulePrio(); 

#if osdEnableContextOverlay
      COVER(0x00AB)
      if(oskTcbContextOverlay[osActiveTaskIndex]==2)
      {
         COVER(0x00AC)
         osTcbReturnAddressState[osActiveTaskIndex]=1;
      }
      ELSEWITHCOVER(0x00AD)
#endif

      COVER(0x00AE)
      OS_WE_END_CRITICAL()
      OS_WE_EXIT()
      /* KB begin osekHWosEnableLevel */
      osEnableLevel();    /* enable task level interrupt */
      /* KB end osekHWosEnableLevel */

      /* start dispatcher */
      OS_START_DISPATCH()
      /* KB begin osekHWosDispatcher */
      osDispatch();
      /* KB end osekHWosDispatcher */

      return E_OK;
   }
   ELSEWITHCOVER(0x00AF)

   OS_WE_END_CRITICAL()
   OS_WE_EXIT()
   /* KB begin osekHWosEnableLevel */
   osEnableLevel();    /* enable task level interrupt */
   /* KB end osekHWosEnableLevel */

   return E_OK;
} /* END OF osWaitEvent */
#endif

#else  /* End of ((CC == ECC1) || (CC == ECC2)) && (osdNumberOfExtendedTasks > 0) */
/* KB begin osekHWosOsekEvntDummy */
/* KB end osekHWosOsekEvntDummy */
#endif

/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */

/* END OF MODULE osekevnt.c */

