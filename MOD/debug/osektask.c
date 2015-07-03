/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osektask.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.47
|
|  Description: osek API-functions for controlling tasks
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

/* CovComment 0:  file: osektask.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

#if (osdNumberOfAllTasks > 0)
/*-+--------------------------------------------------------------------------
 / ActivateTask
 / ============
 /
 / Syntax: StatusType ActivateTask ( TaskType <TaskID> )
 / Parameter (In):
 / TaskID Task reference
 / Parameter (Out): none
 / Description: The task <TaskID> is transferred from the suspended state into
 / the ready state 12 . The operating system ensures that the task
 / code is being executed from the first statement.
 / Particularities: The service may be called from interrupt level, from task level
 / and the hook routine StartupHook (see Figure 12-1).
 / Rescheduling after the call to ActivateTask depends on the
 / place it is called from (ISR, non preemptable task, preemptable task).
 / If E_OS_LIMIT is returned the activation is ignored.
 / When an extended task is transferred from suspended state
 / into ready state all its events are cleared.
 / Status:
 / Standard: - No error, E_OK
 /           - Too many task activations of <TaskID>, E_OS_LIMIT
 / Extended: - Task <TaskID> is invalid, E_OS_ID
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

#ifdef osdSysAPI
#undef osdSysAPI
#endif

#ifndef osdActivateTaskNotUsed

osqFunc1 StatusType osqFunc2 osActivateTask( TaskType taskIndex)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osActivateTask */
   /* KB begin actTaskBody */
   
      osPrioType priority;
   
      OS_AT_ENTRY()
      COVER(0x0100)
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && !defined osdSysAPI)
      COVER(0x0101)
      if (taskIndex >= osrNumberOfAllTasks)
      {
         COVER(0x0102)
         #if osdErrorHook
         COVER(0x0103)
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_ID, osdErrATWrongTaskID);
         #if osdErrorHook
         COVER(0x0104)
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_ID;
      }
      ELSEWITHCOVER(0x0105)
   #if osdEnableAssertions
      COVER(0x0106)
      if (osIntAPIStatus != 0)
      {
         COVER(0x0107)
         #if osdErrorHook
         COVER(0x0108)
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_SYS_DIS_INT, osdErrATIntAPIDisabled);
         #if osdErrorHook 
         COVER(0x0109)
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_SYS_DIS_INT;
      }
      ELSEWITHCOVER(0x010A)
   #endif
   #endif
   
      COVER(0x010B)
      priority = oskTcbHomePrio[taskIndex];
      osSysErrAssert((priority < osrNumberOfPriorities), osdErrATWrongTaskPrio)
   
   #ifndef osdSysAPI
      COVER(0x010C)
      /* KB begin osekHWosSaveDisableLevel */
      osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
      /* KB end osekHWosSaveDisableLevel */
      OS_AT_START_CRITICAL()
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x010D)
      if (osTcbActivationCount[taskIndex] >= oskTcbAllowedActivations[taskIndex])
   #else
      COVER(0x010E)
      if (osTcbTaskState[taskIndex] != SUSPENDED)
   #endif
      {
         COVER(0x010F)
         osSaveActivateTask_TaskID(taskIndex)
   #ifndef osdSysAPI
         COVER(0x0110)
         osAPIError(E_OS_LIMIT, osdErrATMultipleActivation);
   #else
         COVER(0x0111)
     #if STATUS_LEVEL == EXTENDED_STATUS
         #ifndef osdSuppressFilenames
         COVER(0x0112)
         osActiveTaskLineNumber = 0;  /* not assigned for ActivateTask from alarm management */
         osActiveTaskModule = 0;
         #endif
     #endif
         osAPIError(E_OS_LIMIT, osdErrATAlarmMultipleActivation);
   #endif
   #ifndef osdSysAPI
         COVER(0x0113)
         OS_AT_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevel */
         osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
         /* KB end osekHWosRestoreEnableLevel */
   #endif
         OS_AT_EXIT()
         return E_OS_LIMIT;
      }
      ELSEWITHCOVER(0x0114)
   
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x0115)
      if (osTcbActivationCount[taskIndex] == 0)
      {
         COVER(0x0116)
         osTcbTaskState[taskIndex] = PRE_READY;
      }
      ELSEWITHCOVER(0x0117)
      osTcbActivationCount[taskIndex]++;
   #else
      COVER(0x0118)
      osTcbTaskState[taskIndex] = PRE_READY;
   #endif
   
   #if (osdNumberOfExtendedTasks > 0)
   #if (osdNumberOfExtendedTasks < osdNumberOfAllTasks)
         COVER(0x0119)
         if (taskIndex<osdNumberOfExtendedTasks)
         {
            COVER(0x011A)
            osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
         }
         ELSEWITHCOVER(0x011B)
   #else
         /*all tasks are extended*/
         COVER(0x011C)
         osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
   #endif
   #endif /* (osdNumberOfExtendedTasks > 0) */
   
      COVER(0x011D)
      /* KB begin osQInsertAtPrio */
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x011E)
      oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; /* PRQA S 3689 */ /* index is validated by assertion */
      #else
      COVER(0x011F)
      osQReadyTask[priority]= taskIndex;   /* PRQA S 3689 */ /* index is validated by assertion */
      #endif
      
      #if (osdRTSize > 1)
      COVER(0x0120)
      osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
      #else
      COVER(0x0121)
      osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
      #endif
      
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x0122)
      osQReadyTaskTail[priority]++;   /* PRQA S 3689 */ /* index is validated by assertion */
      if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])  /* PRQA S 3689 */ /* index is validated by assertion */
      {
         COVER(0x0123)
         osQReadyTaskTail[priority]= 0;   /* PRQA S 3689 */ /* index is validated by assertion */
      }
      #endif
      
      
      /* KB end osQInsertAtPrio */
   
      /* start dispatcher only if new task has greater prio */
   
      if (priority < osHighReadyTaskPrio)
      {
   
         COVER(0x0124)
         osHighReadyTaskPrio   = priority;
         osHighReadyTaskIndex  = taskIndex;
         osStartDispatcher= TRUE;     /* for return from idle loop */
   
   #if osdFullPreempt || osdMixedPreempt
         COVER(0x0125)
         if (osLockDispatcher == 0)
         {
            COVER(0x0126)
   #ifndef osdSysAPI
            COVER(0x0127)
            OS_AT_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel */
            osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
            /* KB end osekHWosRestoreEnableLevel */
   #endif
            OS_AT_EXIT()
            OS_START_DISPATCH()
            /* KB begin osekHWosDispatcher */
            osDispatch();
            /* KB end osekHWosDispatcher */
            return E_OK;
         }
         ELSEWITHCOVER(0x0128)
   #endif /* osdFullPreempt || osdMixedPreempt */
      }
      ELSEWITHCOVER(0x0129)
   #ifndef osdSysAPI
      OS_AT_END_CRITICAL()
   #endif
      OS_AT_EXIT()
   #ifndef osdSysAPI
      COVER(0x012A)
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
   #endif
      COVER(0x012B)
      return E_OK;
   
   /* KB end actTaskBody */
}

#endif


#define osdSysAPI
/* version for OS internal use, not using osDisable/osEnable */
osqFunc1 StatusType osqFunc2 osSysActivateTask( TaskType taskIndex)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osSysActivateTask */

   /* KB begin actTaskBody */
   
      osPrioType priority;
   
      OS_AT_ENTRY()
      COVER(0x012C)
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && !defined osdSysAPI)
      COVER(0x012D)
      if (taskIndex >= osrNumberOfAllTasks)
      {
         COVER(0x012E)
         #if osdErrorHook
         COVER(0x012F)
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_ID, osdErrATWrongTaskID);
         #if osdErrorHook
         COVER(0x0130)
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_ID;
      }
      ELSEWITHCOVER(0x0131)
   #if osdEnableAssertions
      COVER(0x0132)
      if (osIntAPIStatus != 0)
      {
         COVER(0x0133)
         #if osdErrorHook
         COVER(0x0134)
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveActivateTask_TaskID(taskIndex)
         #endif
         osAPIError(E_OS_SYS_DIS_INT, osdErrATIntAPIDisabled);
         #if osdErrorHook 
         COVER(0x0135)
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_AT_EXIT()
         return E_OS_SYS_DIS_INT;
      }
      ELSEWITHCOVER(0x0136)
   #endif
   #endif
   
      COVER(0x0137)
      priority = oskTcbHomePrio[taskIndex];
      osSysErrAssert((priority < osrNumberOfPriorities), osdErrATWrongTaskPrio)
   
   #ifndef osdSysAPI
      COVER(0x0138)
      /* KB begin osekHWosSaveDisableLevel */
      osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
      /* KB end osekHWosSaveDisableLevel */
      OS_AT_START_CRITICAL()
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x0139)
      if (osTcbActivationCount[taskIndex] >= oskTcbAllowedActivations[taskIndex])
   #else
      COVER(0x013A)
      if (osTcbTaskState[taskIndex] != SUSPENDED)
   #endif
      {
         COVER(0x013B)
         osSaveActivateTask_TaskID(taskIndex)
   #ifndef osdSysAPI
         COVER(0x013C)
         osAPIError(E_OS_LIMIT, osdErrATMultipleActivation);
   #else
         COVER(0x013D)
     #if STATUS_LEVEL == EXTENDED_STATUS
         #ifndef osdSuppressFilenames
         COVER(0x013E)
         osActiveTaskLineNumber = 0;  /* not assigned for ActivateTask from alarm management */
         osActiveTaskModule = 0;
         #endif
     #endif
         osAPIError(E_OS_LIMIT, osdErrATAlarmMultipleActivation);
   #endif
   #ifndef osdSysAPI
         COVER(0x013F)
         OS_AT_END_CRITICAL()
         /* KB begin osekHWosRestoreEnableLevel */
         osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
         /* KB end osekHWosRestoreEnableLevel */
   #endif
         OS_AT_EXIT()
         return E_OS_LIMIT;
      }
      ELSEWITHCOVER(0x0140)
   
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x0141)
      if (osTcbActivationCount[taskIndex] == 0)
      {
         COVER(0x0142)
         osTcbTaskState[taskIndex] = PRE_READY;
      }
      ELSEWITHCOVER(0x0143)
      osTcbActivationCount[taskIndex]++;
   #else
      COVER(0x0144)
      osTcbTaskState[taskIndex] = PRE_READY;
   #endif
   
   #if (osdNumberOfExtendedTasks > 0)
   #if (osdNumberOfExtendedTasks < osdNumberOfAllTasks)
         COVER(0x0145)
         if (taskIndex<osdNumberOfExtendedTasks)
         {
            COVER(0x0146)
            osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
         }
         ELSEWITHCOVER(0x0147)
   #else
         /*all tasks are extended*/
         COVER(0x0148)
         osTcbSetEventMask[taskIndex]= 0;    /* with extended tasks clear event field */
   #endif
   #endif /* (osdNumberOfExtendedTasks > 0) */
   
      COVER(0x0149)
      /* KB begin osQInsertAtPrio */
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x014A)
      oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; /* PRQA S 3689 */ /* index is validated by assertion */
      #else
      COVER(0x014B)
      osQReadyTask[priority]= taskIndex;   /* PRQA S 3689 */ /* index is validated by assertion */
      #endif
      
      #if (osdRTSize > 1)
      COVER(0x014C)
      osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
      #else
      COVER(0x014D)
      osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
      #endif
      
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x014E)
      osQReadyTaskTail[priority]++;   /* PRQA S 3689 */ /* index is validated by assertion */
      if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])  /* PRQA S 3689 */ /* index is validated by assertion */
      {
         COVER(0x014F)
         osQReadyTaskTail[priority]= 0;   /* PRQA S 3689 */ /* index is validated by assertion */
      }
      #endif
      
      
      /* KB end osQInsertAtPrio */
   
      /* start dispatcher only if new task has greater prio */
   
      if (priority < osHighReadyTaskPrio)
      {
   
         COVER(0x0150)
         osHighReadyTaskPrio   = priority;
         osHighReadyTaskIndex  = taskIndex;
         osStartDispatcher= TRUE;     /* for return from idle loop */
   
   #if osdFullPreempt || osdMixedPreempt
         COVER(0x0151)
         if (osLockDispatcher == 0)
         {
            COVER(0x0152)
   #ifndef osdSysAPI
            COVER(0x0153)
            OS_AT_END_CRITICAL()
            /* KB begin osekHWosRestoreEnableLevel */
            osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
            /* KB end osekHWosRestoreEnableLevel */
   #endif
            OS_AT_EXIT()
            OS_START_DISPATCH()
            /* KB begin osekHWosDispatcher */
            osDispatch();
            /* KB end osekHWosDispatcher */
            return E_OK;
         }
         ELSEWITHCOVER(0x0154)
   #endif /* osdFullPreempt || osdMixedPreempt */
      }
      ELSEWITHCOVER(0x0155)
   #ifndef osdSysAPI
      OS_AT_END_CRITICAL()
   #endif
      OS_AT_EXIT()
   #ifndef osdSysAPI
      COVER(0x0156)
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
   #endif
      COVER(0x0157)
      return E_OK;
   
   /* KB end actTaskBody */

}

#ifndef osdTerminateTaskNotUsed
/*-+--------------------------------------------------------------------------
 / TerminateTask
 / =============
 /
 / Syntax: StatusType TerminateTask ( void )
 / Parameter (In): none
 / Parameter (Out): none
 / Description: This service causes the termination of the calling task. The
 / calling task is transferred from the running state into the
 / suspended state 13 .
 / Particularities: An internal resource assigned to the calling task is
 / automatically released. Other resources occupied by the task
 / must have been released before the call to TerminateTask. If a
 / resource is still occupied in standard status the behaviour is
 / undefined.
 / If the call was successful, TerminateTask does not return to the
 / call level and the status can not be evaluated.
 / If the version with extended status is used, the service returns
 / in case of error, and provides a status which can be evaluated
 / in the application.
 / If the service TerminateTask is called successfully, it enforces a
 / rescheduling.
 / Ending a task function without call to TerminateTask or
 / ChainTask is strictly forbidden and may leave the system in an
 / undefined state.
 / Status:
 / Standard: No return to call level
 / Extended: - Task still occupies resources, E_OS_RESOURCE
 /           - Call at interrupt level, E_OS_CALLEVEL
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osTerminateTask ( void )  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osTerminateTask */
   COVER(0x0158)
   OS_TT_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0159)
   if (osIntNestingDepth != 0)
   {
      COVER(0x015A)
      #if osdErrorHook 
      COVER(0x015B)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrTTOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x015C)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x015D)
   COVER(0x015E)

#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   COVER(0x015F)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
#else
   COVER(0x0160)
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
#endif
   {
      COVER(0x0161)
      #if osdErrorHook 
      COVER(0x0162)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_RESOURCE, osdErrTTResourcesOccupied);
      #if osdErrorHook 
      COVER(0x0163)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x0164)
   COVER(0x0165)

#if osdEnableAssertions
   COVER(0x0166)
   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x0167)
      #if osdErrorHook 
      COVER(0x0168)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrTTDisabledInterrupts);
      #if osdErrorHook 
      COVER(0x0169)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_TT_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x016A)
   COVER(0x016B)
#endif
#endif

   COVER(0x016C)
   /* KB begin osekHWosDisableLevel */
   osDisableLevel();    /* disable interrupts up to system level */
   /* KB end osekHWosDisableLevel */
   OS_TT_START_CRITICAL()

#if osdPostTaskHook
   COVER(0x016D)
   /* call PostTaskHook before clearing osRunTaskIndex! */
   osPostTaskHook();
#endif

#if osdNumberOfInternalResources > 0
   COVER(0x016E)
   /* KB begin osReleaseInternalResource */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
      COVER(0x016F)
   #if osdRTSize == 1
      COVER(0x0170)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x0171)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]]
                    &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x0172)
      osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   #if osdORTIDebug
      COVER(0x0173)
      osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   }
   
   
   /* KB end osReleaseInternalResource */
#endif

#if ((CC == ECC2) || (CC == BCC2))
   COVER(0x0174)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x0175)
   osSysErrAssert(osTcbActivationCount[osActiveTaskIndex] != 0, osdErrTTNotActivated)
#endif
   
   COVER(0x0176)
   osTcbActivationCount[osActiveTaskIndex]--;
   
   if (osTcbActivationCount[osActiveTaskIndex] != 0)
   {  
      COVER(0x0177)
      osTcbTaskState[osActiveTaskIndex] = PRE_READY;
   }
   else
   {
      COVER(0x0178)
      osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
   }
#else
   COVER(0x0179)
   osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
#endif

   COVER(0x017A)
   /* KB begin osQDeleteFromPrio */
   
   #if ((CC == ECC2) || (CC == BCC2))
   COVER(0x017B)
   /* delete task out of the prio queue */
   osQReadyTaskHead[osActiveTaskPrio]++;
   if (osQReadyTaskHead[osActiveTaskPrio] > oskQMaxActivations[osActiveTaskPrio])
   {
      COVER(0x017C)
      osQReadyTaskHead[osActiveTaskPrio]= 0;
   }
   
   if (osQReadyTaskHead[osActiveTaskPrio] == osQReadyTaskTail[osActiveTaskPrio])
   {  /* queue empty */
      COVER(0x017D)
   #endif
   
   #if (osdRTSize > 1)
      COVER(0x017E)
      osQReadyPrios[oskTcbReadyPrioOffset[osActiveTaskIndex]]&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #else
      COVER(0x017F)
      osQReadyPrios&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
   }
   #endif
   
   /* KB end osQDeleteFromPrio */

   /* KB begin osPrepareForStackCorrection */
   /* KB end osPrepareForStackCorrection */

#if osdStackCheck
   COVER(0x0180)
   osLastActiveTaskIndex = osActiveTaskIndex;
#endif
   COVER(0x0181)
   osActiveTaskIndex = osdNoActiveTask;
   osActiveTaskPrio = osdNoActivePrio;

   /* for TerminateTask, osSchedulePrio returns always TRUE! */
   /* (same task is never running after TerminateTask) */
   (void) osSchedulePrio();

   OS_TT_END_CRITICAL()
   OS_TT_EXIT()

   /* KB begin osekHWosEnableLevel */
   osEnableLevel();    /* enable task level interrupt */
   /* KB end osekHWosEnableLevel */

   /* start dispatcher */
   OS_START_DISPATCH()
   /* KB begin osekHWosDispatcher */
   osDispatch();
   /* KB end osekHWosDispatcher */

   /* KB begin osekHWosRollBackStack */
   /* KB end osekHWosRollBackStack */

   /* KB begin osekHWAfterOsDispatchInTT */
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
      osDisableGlobal();
   #ifndef osdSuppressFilenames
      osActiveTaskModule = osd_FILE_Cast __FILE__;
      osActiveTaskLineNumber = __LINE__;
   #endif  /* osdSuppressFilenames */
      osAssertFailed_(osdErrTTNoImmediateTaskSwitch);
      return E_OK;/*lint !e527 Unreachable code, just prevent from compiler warnings*/
   #else /* if STATUS_LEVEL ... */
      for(;;)
      {
         ;
      }
   #endif /* STATUS_LEVEL ... */
   /* KB end osekHWAfterOsDispatchInTT */


} /* END OF osTerminateTask */

#endif /*osdTerminateTaskNotUsed*/

#ifndef osdChainTaskNotUsed
/*-+--------------------------------------------------------------------------
 / ChainTask
 / =========
 /
 / Syntax: StatusType ChainTask ( TaskType <TaskID> )
 / Parameter (In):
 / TaskID Reference to the sequential succeeding task to be activated.
 / Parameter (Out): none
 / Description: This service causes the termination of the calling task. After
 / termination of the calling task a succeeding task <TaskID> is
 / activated. Using this service, it ensures that the succeeding
 / task starts to run at the earliest after the calling task has been
 / terminated.
 / Particularities: If the succeeding task is identical with the current task, this
 / does not result in multiple requests. The task is not transferred
 / to the suspended state.
 / An internal resource assigned to the calling task is
 / automatically released, even if the succeeding task is identical
 / with the current task. Other resources occupied by the calling
 / task must have been released before ChainTask is called. If a
 / resource is still occupied in standard status the behaviour is
 / undefined.
 / If called successfully, ChainTask does not return to the call
 / level and the status can not be evaluated.
 / In case of error the service returns to the calling task and
 / provides a status which can then be evaluated in the
 / application.
 / If the service ChainTask is called successfully, this enforces a
 / rescheduling.
 / Ending a task function without call to TerminateTask or
 / ChainTask is strictly forbidden and may leave the system in an
 / undefined state.
 / If E_OS_LIMIT is returned the activation is ignored.
 / When an extended task is transferred from suspended state
 / into ready state all its events are cleared.
 / Status:
 / Standard:  - No return to call level
 /            - Too many task activations of <TaskID>, E_OS_LIMIT
 / Extended:  - Task <TaskID> is invalid, E_OS_ID
 /            - Calling task still occupies resources, E_OS_RESOURCE
 /            - Call at interrupt level, E_OS_CALLEVEL
 / Conformance: BCC1, BCC2, ECC1, ECC2  /
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osChainTask(TaskType taskIndex)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osChainTask */
   osPrioType priority;

   COVER(0x0182)
   OS_HT_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0183)
   if (osIntNestingDepth != 0)
   {
      COVER(0x0184)
      #if osdErrorHook 
      COVER(0x0185)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrHTOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x0186)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x0187)
   COVER(0x0188)

#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   COVER(0x0189)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
#else
   COVER(0x018A)
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
#endif
   {
      COVER(0x018B)
      #if osdErrorHook 
      COVER(0x018C)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_RESOURCE, osdErrHTResourcesOccupied);
      #if osdErrorHook 
      COVER(0x018D)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x018E)
   COVER(0x018F)

   if (taskIndex >= osrNumberOfAllTasks)
   {
      COVER(0x0190)
      #if osdErrorHook 
      COVER(0x0191)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_ID, osdErrHTWrongTaskID);
      #if osdErrorHook 
      COVER(0x0192)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0193)
   COVER(0x0194)

#if osdEnableAssertions
   COVER(0x0195)
   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x0196)
      #if osdErrorHook 
      COVER(0x0197)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveChainTask_TaskID(taskIndex)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrHTInterruptsDisabled);
      #if osdErrorHook 
      COVER(0x0198)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_HT_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0199)
   COVER(0x019A)
#endif
#endif

   COVER(0x019B)
   /* KB begin osekHWosDisableLevel */
   osDisableLevel();    /* disable interrupts up to system level */
   /* KB end osekHWosDisableLevel */
   OS_HT_START_CRITICAL()

   if (taskIndex != osActiveTaskIndex)
   {
#if ((CC == ECC2) || (CC == BCC2))
      COVER(0x019C)
      if (osTcbActivationCount[taskIndex] >= oskTcbAllowedActivations[taskIndex])
#else
      COVER(0x019D)
      if (osTcbTaskState[taskIndex] != SUSPENDED)
#endif
      {
         COVER(0x019E)
         osSaveChainTask_TaskID(taskIndex)
         osAPIError(E_OS_LIMIT, osdErrHTMultipleActivation);
         OS_HT_END_CRITICAL()
         OS_HT_EXIT()
         /* KB begin osekHWosEnableLevel */
         osEnableLevel();    /* enable task level interrupt */
         /* KB end osekHWosEnableLevel */
         return E_OS_LIMIT;
      }
      ELSEWITHCOVER(0x019F)
      COVER(0x01A0)
   }
   ELSEWITHCOVER(0x01A1)

   /*************************************************************************
    * terminate old task
    *************************************************************************/

   /* call hook routine for the terminated task */
#if osdPostTaskHook
   COVER(0x01A2)
   /* call PostTaskHook before clearing osActiveTaskIndex! */
   osPostTaskHook();
#endif

#if osdNumberOfInternalResources > 0
   COVER(0x01A3)
   /* KB begin osReleaseInternalResource */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
      COVER(0x01A4)
   #if osdRTSize == 1
      COVER(0x01A5)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x01A6)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]]
                    &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x01A7)
      osActiveTaskPrio=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   #if osdORTIDebug
      COVER(0x01A8)
      osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   }
   
   
   /* KB end osReleaseInternalResource */
#endif

#if ((CC == ECC2) || (CC == BCC2))
   COVER(0x01A9)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x01AA)
   osSysErrAssert(osTcbActivationCount[osActiveTaskIndex] != 0, osdErrHTNotActivated)
#endif

   COVER(0x01AB)
   osTcbActivationCount[osActiveTaskIndex]--;

   if (osTcbActivationCount[osActiveTaskIndex] != 0)
   {
      COVER(0x01AC)
      osTcbTaskState[osActiveTaskIndex] = PRE_READY;
   }
   else
   {
      COVER(0x01AD)
      osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
   }
#else
   COVER(0x01AE)
   osTcbTaskState[osActiveTaskIndex] = SUSPENDED;
#endif

   COVER(0x01AF)

   /* KB begin osQDeleteFromPrio */
   
   #if ((CC == ECC2) || (CC == BCC2))
   COVER(0x01B0)
   /* delete task out of the prio queue */
   osQReadyTaskHead[osActiveTaskPrio]++;
   if (osQReadyTaskHead[osActiveTaskPrio] > oskQMaxActivations[osActiveTaskPrio])
   {
      COVER(0x01B1)
      osQReadyTaskHead[osActiveTaskPrio]= 0;
   }
   
   if (osQReadyTaskHead[osActiveTaskPrio] == osQReadyTaskTail[osActiveTaskPrio])
   {  /* queue empty */
      COVER(0x01B2)
   #endif
   
   #if (osdRTSize > 1)
      COVER(0x01B3)
      osQReadyPrios[oskTcbReadyPrioOffset[osActiveTaskIndex]]&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #else
      COVER(0x01B4)
      osQReadyPrios&= ~oskTcbReadyPrioMask[osActiveTaskIndex];
   #endif
   
   #if ((CC == ECC2) || (CC == BCC2))
   }
   #endif
   
   /* KB end osQDeleteFromPrio */

   /* KB begin osPrepareForStackCorrection */
   /* KB end osPrepareForStackCorrection */

#if osdStackCheck
   COVER(0x01B5)
   osLastActiveTaskIndex = osActiveTaskIndex;
#endif
   COVER(0x01B6)
   osActiveTaskIndex = osdNoActiveTask;
   osActiveTaskPrio = osdNoActivePrio;


   /*************************************************************************
    * now activate the new task
    *************************************************************************/

#if (STATUS_LEVEL == STANDARD_STATUS)
   COVER(0x01B7)
   #if ((CC == ECC2) || (CC == BCC2))
   COVER(0x01B8)
   if (osTcbActivationCount[taskIndex] < oskTcbAllowedActivations[taskIndex])
   #else
   COVER(0x01B9)
   if (osTcbTaskState[taskIndex] == SUSPENDED)
   #endif
#endif
   {
      COVER(0x01BA)
      priority = oskTcbHomePrio[taskIndex];
      osSysErrAssert(priority < osrNumberOfPriorities, osdErrHTWrongTaskPrio)
      /* PRQA S 3689 L_osChainTask1 */ /* index is validated by assertion */

#if (osdNumberOfExtendedTasks > 0)
      COVER(0x01BB)
#if (osdNumberOfExtendedTasks < osdNumberOfAllTasks)
      COVER(0x01BC)
      /* with extended tasks clear event field */
      if (taskIndex<osdNumberOfExtendedTasks)
      {
         COVER(0x01BD)
         osTcbSetEventMask[taskIndex]= 0;
      }
      ELSEWITHCOVER(0x01BE)
#else
      COVER(0x01BF)
      /*no basic tasks, so for all tasks clear event field */
      osTcbSetEventMask[taskIndex]= 0;
#endif
#endif /* (osdNumberOfExtendedTasks > 0) */

#if ((CC == ECC2) || (CC == BCC2))
      COVER(0x01C0)
      if (osTcbActivationCount[taskIndex] == 0)
      {
         COVER(0x01C1)
         osTcbTaskState[taskIndex] = PRE_READY;
      }
      ELSEWITHCOVER(0x01C2)
      osTcbActivationCount[taskIndex]++;
#else
      COVER(0x01C3)
      osTcbTaskState[taskIndex] = PRE_READY;
#endif

      COVER(0x01C4)
      /* KB begin osQInsertAtPrio */
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x01C5)
      oskQActivationQueues[priority][osQReadyTaskTail[priority]]= taskIndex; /* PRQA S 3689 */ /* index is validated by assertion */
      #else
      COVER(0x01C6)
      osQReadyTask[priority]= taskIndex;   /* PRQA S 3689 */ /* index is validated by assertion */
      #endif
      
      #if (osdRTSize > 1)
      COVER(0x01C7)
      osQReadyPrios[oskTcbReadyPrioOffset[taskIndex]]|= oskTcbReadyPrioMask[taskIndex];
      #else
      COVER(0x01C8)
      osQReadyPrios|= oskTcbReadyPrioMask[taskIndex];
      #endif
      
      
      #if ((CC == ECC2) || (CC == BCC2))
      COVER(0x01C9)
      osQReadyTaskTail[priority]++;   /* PRQA S 3689 */ /* index is validated by assertion */
      if (osQReadyTaskTail[priority] > oskQMaxActivations[priority])  /* PRQA S 3689 */ /* index is validated by assertion */
      {
         COVER(0x01CA)
         osQReadyTaskTail[priority]= 0;   /* PRQA S 3689 */ /* index is validated by assertion */
      }
      #endif
      
      
      /* KB end osQInsertAtPrio */
      /* PRQA L:L_osChainTask1 */ 
   }

   COVER(0x01CB)
   (void) osSchedulePrio();
   OS_HT_END_CRITICAL()
   OS_HT_EXIT()

   /* KB begin osekHWosEnableLevel */
   osEnableLevel();    /* enable task level interrupt */
   /* KB end osekHWosEnableLevel */

   /* start dispatcher */
   OS_START_DISPATCH()
   /* KB begin osekHWosDispatcher */
   osDispatch();
   /* KB end osekHWosDispatcher */

   /* KB begin osekHWosRollBackStack */
   /* KB end osekHWosRollBackStack */

   /* KB begin osekHWAfterOsDispatchInHT */
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
      osDisableGlobal();
   #ifndef osdSuppressFilenames
      osActiveTaskModule = osd_FILE_Cast __FILE__;
      osActiveTaskLineNumber = __LINE__;
   #endif  /* osdSuppressFilenames */
      osAssertFailed_(osdErrHTNoImmediateTaskSwitch);
      return E_OK;/*lint !e527 Unreachable code, just prevent from compiler warnings*/
   #else /* if STATUS_LEVEL ... */
      for(;;)
      {
         ;
      }
   #endif /* STATUS_LEVEL ... */
   /* KB end osekHWAfterOsDispatchInHT */


} /* END OF osChainTask */
#endif /* osdChainTaskNotUsed */


#ifndef osdScheduleNotUsed
/*-+--------------------------------------------------------------------------
 / Schedule
 / ========
 /
 / Syntax: StatusType Schedule ( void )
 / Parameter (In): none
 / Parameter (Out): none
 / Description: If a higher-priority task is ready, the internal resource of the task
 / is released, the current task is put into the ready state, its
 / context is saved and the higher-priority task is executed.
 / Otherwise the calling task is continued.
 / Particularities: Rescheduling can only take place if an internal resource is
 / assigned to the calling task during system generation. For
 / these tasks, Schedule enables a processor assignment to other
 / tasks with lower priority than the ceiling priority of the internal
 / resource and higher priority than the priority of the calling task
 / in application-specific locations. When returning from Schedule,
 / the internal resource has been taken again.
 / This service has no influence on tasks with no internal resource
 / assigned (preemptable tasks).
 / Status:
 / Standard:  - No error, E_OK
 / Extended:  - Call at interrupt level, E_OS_CALLEVEL
 /            - Calling task occupies resources, E_OS_RESOURCE
 / Conformance: BCC1, BCC2, ECC1, ECC2 
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osSchedule(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osSchedule */

   COVER(0x01CC)
#if osdEnableContextOverlay
   COVER(0x01CD)
   /* KB begin osekHWosSaveReturnAddress */
   /* KB end osekHWosSaveReturnAddress */
#endif

   COVER(0x01CE)
   OS_SH_ENTRY()
#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x01CF)
   if (osIntNestingDepth != 0)
   {
      COVER(0x01D0)
      #if osdErrorHook 
      COVER(0x01D1)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_CALLEVEL, osdErrSHOnInterruptLevel);
      #if osdErrorHook 
      COVER(0x01D2)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_CALLEVEL;
   }
   ELSEWITHCOVER(0x01D3)
   COVER(0x01D4)

#if osdEnableAssertions
   COVER(0x01D5)
   if (osCheckInterruptsEnabled() == 0)
   {
      COVER(0x01D6)
      #if osdErrorHook 
      COVER(0x01D7)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrSHInterruptsDisabled);
      #if osdErrorHook 
      COVER(0x01D8)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x01D9)
   COVER(0x01DA)
#endif

#if (osdNumberOfInternalResources == 0) && (osdNumberOfInterruptResources == 0)
   COVER(0x01DB)
   /* check for task resources only system */
   if (osActiveTaskPrio != oskTcbHomePrio[osActiveTaskIndex])
#else
   COVER(0x01DC)
   /* check for mixed resource system (task, interrupt and internal) */
   if (osTcbResourceCounter[osActiveTaskIndex] != 0)
#endif
   {
      COVER(0x01DD)
      #if osdErrorHook 
      COVER(0x01DE)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      #endif
      osAPIError(E_OS_RESOURCE, osdErrSHResourcesOccupied);
      #if osdErrorHook 
      COVER(0x01DF)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_SH_EXIT()
      return E_OS_RESOURCE;
   }
   ELSEWITHCOVER(0x01E0)
   COVER(0x01E1)

   osSysErrAssert(0U==(oskTcbTaskFlags[osActiveTaskIndex] & osdTaskFlagNotUsingSchedule)
                  ,osdErrSHScheduleNotAllowed)

#if osdEnableProcedures
   COVER(0x01E2)
#if osdEnableAssertions
   COVER(0x01E3)
   if (oskTcbContextOverlay[osActiveTaskIndex]>0)
   {
      COVER(0x01E4)
      osSysErrAssert(osIsInProcedure[osActiveTaskIndex]==FALSE, osdErrSHInContextOverlayProc)
   }
   ELSEWITHCOVER(0x01E5)
#endif
#endif

#endif

   COVER(0x01E6)
   /* KB begin osekHWosDisableLevel */
   osDisableLevel();    /* disable interrupts up to system level */
   /* KB end osekHWosDisableLevel */
   OS_SH_START_CRITICAL()

#if osdNumberOfInternalResources > 0
   COVER(0x01E7)
   /* KB begin osReleaseInternalResourceSchedule */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
      COVER(0x01E8)
   #if osdRTSize == 1
      COVER(0x01E9)
      osQReadyPrios &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x01EA)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]]
                    &= ~oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   
   #if osdORTIDebug
      COVER(0x01EB)
      osTcbActualPrio[osActiveTaskIndex]=oskTcbHomePrio[osActiveTaskIndex];
   #endif
   
   }
   
   /* KB end osReleaseInternalResourceSchedule */
#endif

   COVER(0x01EC)
   if (osSchedulePrio() != 0)
   {
      COVER(0x01ED)
#if osdEnableContextOverlay
      COVER(0x01EE)
      if (oskTcbContextOverlay[osActiveTaskIndex]==2)
      {
         COVER(0x01EF)
         osTcbReturnAddressState[osActiveTaskIndex]=1;
      }
      ELSEWITHCOVER(0x01F0)
#endif

      OS_SH_END_CRITICAL()
      OS_SH_EXIT()
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
   ELSEWITHCOVER(0x01F1)
   COVER(0x01F2)

#if osdNumberOfInternalResources > 0
   COVER(0x01F3)
   /* KB begin osGetInternalResourceSchedule */
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
      COVER(0x01F4)
   #if osdRTSize == 1
      COVER(0x01F5)
      osQReadyPrios |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x01F6)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]]
                    |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   
      /* osHighReadyTaskPrio is modified in osSchedulePrio */
      osHighReadyTaskPrio = oskResInternalCeilingPrio[osActiveTaskIndex];
   
   #if osdORTIDebug
      COVER(0x01F7)
      osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
   #endif
   
   }
   /* KB end osGetInternalResourceSchedule */
#endif

   COVER(0x01F8)
   OS_SH_END_CRITICAL()
   OS_SH_EXIT()
   /* KB begin osekHWosEnableLevel */
   osEnableLevel();    /* enable task level interrupt */
   /* KB end osekHWosEnableLevel */
   return E_OK;
} /* END OF osSchedule */

#endif


#ifndef osdGetTaskIDNotUsed
/*-+--------------------------------------------------------------------------
 / GetTaskID
 / =========
 /
 / Syntax: StatusType GetTaskID ( TaskRefType <TaskID> )
 / Parameter (In): none
 / Parameter (Out):
 / TaskID Reference to the task which is currently running
 / Description: GetTaskID returns the information about the TaskID of the task
 / which is currently running.
 / Particularities: Allowed on task level, ISR level and in several hook routines
 / (see Figure 12-1).
 / This service is intended to be used by library functions and
 / hook routines.
 / If <TaskID> can’t be evaluated (no task currently running), the
 / service returns INVALID_TASK as TaskType.
 / Status:
 / Standard:  - No error, E_OK
 / Extended:  - No error, E_OK
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osGetTaskID (TaskRefType TaskId)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetTaskID */
   OS_GI_ENTRY()
   COVER(0x01F9)

   *TaskId= osActiveTaskIndex;

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x01FA)
#if osdEnableAssertions
   COVER(0x01FB)
   if (osIntAPIStatus != 0)
   {
      COVER(0x01FC)
      #if osdErrorHook 
      COVER(0x01FD)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskID_TaskID(TaskId)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrGIIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x01FE)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GI_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x01FF)
   COVER(0x0200)
#endif
#endif

   COVER(0x0201)
   OS_GI_EXIT()
   return E_OK;
} /* END OF osGetTaskID */

#endif


#ifndef osdGetTaskStateNotUsed
/*-+--------------------------------------------------------------------------
 / GetTaskState
 / ============
 /
 / Syntax: StatusType GetTaskState ( TaskType <TaskID>,TaskStateRefType <State> )
 / Parameter (In): TaskID Task reference
 / Parameter (Out): State Reference to the state of the task <TaskID>
 / Description: Returns the state of a task (running, ready, waiting, suspended)
 / at the time of calling GetTaskState.
 / Particularities: The service may be called from interrupt service routines, task
 / level, and some hook routines (see Figure 12-1).
 / Within a full preemptive system, calling this operating system
 / service only provides a meaningful result if the task runs in an
 / interrupt disabling state at the time of calling.
 / When a call is made from a task in a full preemptive system,
 / the result may already be incorrect at the time of evaluation.
 / When the service is called for a task, which is multiply
 / activated, the state is set to running if any instance of the task
 / is running.
 / Status:
 / Standard: - No error, E_OK
 / Extended: - Task <TaskID> is invalid, E_OS_ID
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osGetTaskState(TaskType TaskId, TaskStateRefType State)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetTaskState */
   OS_GS_ENTRY()
   COVER(0x0202)

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0203)
   if (TaskId >= osrNumberOfAllTasks)
   {
      COVER(0x0204)
      #if osdErrorHook 
      COVER(0x0205)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskState_TaskID(TaskId)
      osSaveGetTaskState_State(State)
      #endif
      osAPIError(E_OS_ID, osdErrGSWrongTaskID);
      #if osdErrorHook 
      COVER(0x0206)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GS_EXIT()
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x0207)
   COVER(0x0208)

#if osdEnableAssertions
   COVER(0x0209)
   if (osIntAPIStatus != 0)
   {
      COVER(0x020A)
      #if osdErrorHook 
      COVER(0x020B)
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetTaskState_TaskID(TaskId)
      osSaveGetTaskState_State(State)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrGSIntAPIDisabled);
      #if osdErrorHook 
      COVER(0x020C)
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GS_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x020D)
   COVER(0x020E)
#endif
#endif

   COVER(0x020F)
   *State = osTcbTaskState[TaskId];

   /* change PRE_READY to READY */
   if ((*State) == PRE_READY)
   {
      COVER(0x0210)
      *State = READY;
   }
   ELSEWITHCOVER(0x0211)

   OS_GS_EXIT()
   return E_OK;
} /* END OF osGetTaskState */

#endif

#else
/* KB begin osekHWosOsekTaskDummy */
/* KB end osekHWosOsekTaskDummy */
#endif /* (osdNumberOfAllTasks > 0) */

/* wrapper functions for task hooks if testmacros are used */
#ifdef osdTestMacros

/* KB begin osekHWCodeSegmentDefault */
/* KB end osekHWCodeSegmentDefault */

#if osdPreTaskHook
osqFunc1 void osqFunc2 osTestMacrosPreTaskHook(void)
{
   /* CovComment 4:  Function: osTestMacrosPreTaskHook */
   COVER(0x0212)
   OS_PREHOOK_ENTRY()
#if osdLib
   COVER(0x0213)
   osInterPreTaskHook();
#else
   COVER(0x0214)
   PreTaskHook();
#endif
   OS_PREHOOK_EXIT()
}
#endif

#if osdPostTaskHook
osqFunc1 void osqFunc2 osTestMacrosPostTaskHook(void)
{
   /* CovComment 4:  Function: osTestMacrosPostTaskHook */
   COVER(0x0215)
   OS_POSTHOOK_ENTRY()
#if osdLib
   COVER(0x0216)
   osInterPostTaskHook();
#else
   COVER(0x0217)
   PostTaskHook();
#endif
   OS_POSTHOOK_EXIT()
}
#endif

#endif

/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */

/* END OF MODULE osektask.c */
