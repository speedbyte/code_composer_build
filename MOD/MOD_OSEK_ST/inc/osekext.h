/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekext.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.41
|
|  Description: external declarations (only for system internal use)
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH        All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEKEXT_H     /* PRQA S 0883 EOF */ /* workaround for wrong QAC warning (here is the double include protection) */
#define _OSEKEXT_H

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


/* KB begin osekHwDefinitions */

#define osdOSEKTaskLevel        0     /* has to be 0! */

#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif

/* KB end osekHwDefinitions */

osqFunc1 void osqFunc2 osErrorHook(StatusType ErrorCode);

/* prototypes for OSEK internal functions */
#if (osdNumberOfAlarms > 0)
osqFunc1 void osqFunc2 osInitAlarms(void);
#endif
osqFunc1 void osqFunc2 osStartOSc(void);
osqFunc1 boolean osqFunc2 osSchedulePrio(void);
osqFunc1 StatusType osqFunc2 osSysActivateTask( TaskType taskIndex);

#if (osdNumberOfAlarmSetEvent > 0)
osqFunc1 StatusType osqFunc2 osSysSetEvent(TaskType taskIndex, EventMaskType setMask);
#endif


#ifdef osdUseGeneratedFastAlarm
osqFunc1 void osqFunc2 osCheckAlarms(void);
#else
osqFunc1 void osqFunc2 osWorkAlarms(void);
osqFunc1 void osqFunc2 osInsertMinHeap(osHeapType newItem);
osqFunc1 uint8 osqFunc2 osRemoveItemMinHeap(osHeapType aItem);
#define osInsertMinHeapWithCheck(x) osInsertMinHeap(x)
#endif

osqFunc1 void osqFunc2 osInitTimer(void);

#if (STATUS_LEVEL == EXTENDED_STATUS)
osqFunc1 void osqFunc2 osMissingTerminateError(void);
#endif

osqFunc1 void osqFunc2 osUnhandledException(void);

#if osdStackCheck
osqFunc1 void osqFunc2 osStackOverflow(void);
#endif


/* KB begin osekHwPrototypes */

#if (osdCompiler==osdCompilerGHSARM7)
  #define osDispatch() __asm("   swi #0x81")
#else
  #define osDispatch() osCallDispatcher()
#endif

#define osGetLevel osCheckInterruptsDisabled

#define osSetLevel(lvl) { if((lvl)==1) osDisableLevel(); else osEnableLevel(); }


#if (osdCompiler==osdCompilerGHSARM7)

#if (defined(__THUMB) && (osdThumbMode != 1))
   #error "OSEK configured for ARM mode but compiled for THUMB mode"
#endif
#if (!defined(__THUMB) && (osdThumbMode == 1))
   #error "OSEK configured for THUMB mode but compiled for ARM mode"
#endif

#else

#if (defined(__16bis__) && (osdThumbMode != 1))
   #error "OSEK configured for ARM mode but compiled for THUMB mode"
#endif
#if (!defined(__16bis__) && (osdThumbMode == 1))
   #error "OSEK configured for THUMB mode but compiled for ARM mode"
#endif

/* call dispatcher by SWI 0x81 */
#pragma SWI_ALIAS(osCallDispatcher, 0x81);
void osCallDispatcher(void);

#endif


/* KB end osekHwPrototypes */


/* OSEK internal defines */

/* Task flags */
#define osdTaskFlagScheduleAllowed  ((osTaskFlagType)0)
#define osdTaskFlagNotUsingSchedule ((osTaskFlagType)2)


/* Preemption Type */
#define osdTaskTypeNonPreempt       ((osTaskPreemptionType) 0)
#define osdTaskTypePreempt          ((osTaskPreemptionType) 1)

/* idle definition for task */
#define osdNoActiveTask   ((TaskType)(0xFFFFFFFFUL))

/* idle definition for prio */
#define osdNoActivePrio   ((osPrioType)(0xFFFFFFFFUL))

/* alarm action */
#define osdAlarmActionSetEvent     ((uint8)0)
#define osdAlarmActionActivateTask ((uint8)1)
#define osdAlarmActionCallback     ((uint8)2)
#if (osdUseHeapAlarm != 0)
/* KB begin osekHWosDiffTickTypeComparison */
typedef union
{
   TickType           unsignedNumber;
   osHeapTimeDiffType signedNumber;
}TickTypeTmp;

/*
 * Do a comparison of counter values according to the operator "op".
 * A counter is considered to cyclically increase its n-bit value.
 * The counter overflows from the value (2^n)-1 to 0. Future values
 * are considered to be bigger than actual values. Past values are 
 * smaller. A future value can be reached within 2^(n-1)-1 increments
 * from an actual value, this is guaranteed externally.
 */
#define osdIF_COUNTER( min, op, sub ) \
{\
   volatile osHeapTimeDiffType Tmp;\
   TickTypeTmp osHeapDiffCaster;\
   osHeapDiffCaster.unsignedNumber = (min) - (sub);\
   Tmp = osHeapDiffCaster.signedNumber;\
   if( Tmp op ((osHeapTimeDiffType) 0))    /* PRQA S 3410 */ /* op must be without () */

#define osdENDIF_COUNTER \
}

/* KB end osekHWosDiffTickTypeComparison */
#endif

/* KB begin osekAssignStack */
/* stack grows from high to lower addresses; decrement before push */
#define osAssignStack(stack)    ((osStackDataType*)((&(stack)[0])+(sizeof(stack)/sizeof((stack)[0])))) 
/* KB end osekAssignStack */
/* KB begin osekBottomofStack */
#define osBottomOfStack(stack)  ((osStackDataType*)(stack))
/* KB end osekBottomofStack */

/* KB begin osekHWVariableSectionStartHeader */
/* KB end osekHWVariableSectionStartHeader */

#if osdStackCheck
extern osqRAM1 osqRAM2 TaskType   osqRAM3 osLastActiveTaskIndex;    
#endif

/* KB begin osekGlobalOccCounterDecl */
#if (STATUS_LEVEL == EXTENDED_STATUS)
extern osqRAM1 osqRAM2 osResCounterType osqRAM3 osResGlobalOccupationCounter;
#endif
/* KB end osekGlobalOccCounterDecl */

/* KB begin osekHwExternDeclLockDispatcher */
#if (osdFullPreempt || osdMixedPreempt)
extern osqRAM1 volatile osqRAM2 osDispatcherLockType osqRAM3 osLockDispatcher;
#endif
/* KB end osekHwExternDeclLockDispatcher */

/* KB begin osekHwExternSavedIntLevelVariables */
extern osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntLevel;
extern osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntLevelNested;
/* KB end osekHwExternSavedIntLevelVariables */

#if osdErrorHook
   extern osqBOOL1 osqBOOL2 boolean osqBOOL3 osErrorFlag;
#endif

#if (osdNumberOfAlarms > 0)
   #ifndef osdUseGeneratedFastAlarm
   extern osqRAM1 osqRAM2 osAlarmIndexType osqRAM3 osAlarmHeapCount;    /* Number of heap elements */
   #endif
#endif

/* KB begin osekHwExternalDeclarations */
/* KB end osekHwExternalDeclarations */

/* KB begin osekHWVariableSectionEndHeader */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section data=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekHWVariableSectionEndHeader */

/* KB begin osekEndOfHModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section data=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfHModule */

/*lint -restore re-enable messages*/

#endif /* _OSEKEXT_H, double include preventer */

/* END OF HEADER osekext.h */
