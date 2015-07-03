/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osek.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.59
|
|  Description: contains osek prototypes and defines
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEK_H     /* PRQA S 0883 EOF */ /* workaround for wrong QAC warning (here is the double include protection) */
#define _OSEK_H

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

/* defines for all conformance classes */
#define BCC1   1
#define BCC2   2
#define ECC1   3
#define ECC2   4

#define STANDARD_STATUS 0
#define EXTENDED_STATUS 1

#ifndef osdSuppressPosixTypes

/* posix conform types */

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;

/* KB begin osekPosixTypesElse */
/* KB end osekPosixTypesElse */

#endif /*osdSuppressPosixTypes*/

/* Boolean definition */
/* KB begin osekBooleanDef */

#ifndef boolean
#define boolean uint8
#endif
/* KB end osekBooleanDef */

/* Type for boolean arrays */
/* KB begin osekBooleanArrayBaseTypeDef */
typedef boolean osBoolArrayBaseType;
/* KB end osekBooleanArrayBaseTypeDef */

/* Known Compilers */
/* KB begin osekCompilerDefines */
#define osdCompilerGHSARM7 1
#define osdCompilerTIARM7 2
/* KB end osekCompilerDefines */

#ifdef osdIncludeLibConfig

   #define osdPreTCB  1U        
   #define osdPostTCB 0U 

   #if (osdLibVariant == 1U)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib1.h"
   #else
    #include <oseklib1.h>
   #endif
   #endif
   
   #if (osdLibVariant == 2U)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib2.h"
   #else
    #include <oseklib2.h>
   #endif
   #endif
   
   #if (osdLibVariant == 3U)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib3.h"
   #else
    #include <oseklib3.h>
   #endif
   #endif
   
   #if (osdLibVariant == 4U)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib4.h"
   #else
    #include <oseklib4.h>
   #endif
   #endif

   #if ((osdLibVariant < 1) || (osdLibVariant > 4))
   #error unknown library variant/configuration file
   #endif

   #undef osdPreTCB 
   #undef osdPostTCB
   
#else /*osdIncludeLibConfig*/

   #define osdPreTCB  1        
   #define osdPostTCB 0 
   #if defined USE_QUOTE_INCLUDES
    #include "tcb.h"  /* include before */
   #else
    #include <tcb.h>
   #endif
   #undef osdPreTCB 
   #undef osdPostTCB

#endif /*osdIncludeLibConfig*/


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#define osdNULL ((osqPROM1 osqPROM2 void osqPROM3 *)0)

/* KB begin osekHwMemoryQualifierDefines */
/* define to nothing in the default case */


/* osqROM0 extern osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant;  */
/* osqROM0 osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant; */
#define osqROM0
#define osqROM1
#define osqROM2
#define osqROM3

/* extern osqPROM1 const osqPROM2 char osqPROM3 * osqROM1 const osqROM2 osqROM3 oskTcbTaskName[osdNumberOfAllTasks]; */
#define osqPROM1
#define osqPROM2
#define osqPROM3

/* osqRAM1 volatile osqRAM2 TickType osqRAM3 osSystemCounter; */
/* osqRAM1 osqRAM2 osDispatcherLockType osqRAM3 osLockDispatcher; */
#define osqRAM1 
#define osqRAM2 
#define osqRAM3 

/* osqPRAM1 char osqPRAM2 * osqPRAM3 osActiveTaskModule; */
#define osqPRAM1
#define osqPRAM2
#define osqPRAM3

/* osqBOOL1 volatile osqBOOL2 boolean osqBOOL3 osStartDispatcher; */
#define osqBOOL1 
#define osqBOOL2 
#define osqBOOL3 

/* osqTcbRAM1 volatile osqTcbRAM2 TickType osqTcbRAM3 osCounter[5]; */
/* extern osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskHead[osdNumberOfPriorities]; */
#define osqTcbRAM1 
#define osqTcbRAM2 
#define osqTcbRAM3 

/* extern (osqPTcbRAM1 osqPTcbRAM2 TaskType osqPTcbRAM3 *) osqROM1 const osqROM2 osqROM3 oskQActivationQueues[osdNumberOfPriorities]; */
#define osqPTcbRAM1 
#define osqPTcbRAM2 
#define osqPTcbRAM3 

/* osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack1[80]; */
#define osqStack1
#define osqStack2
#define osqStack3

/* osqProc1 void osqProc2 Procedure1(void); */
#define osqProc1
#define osqProc2 

/* osqFunc1 void osqFunc2 Function1(void); */
#define osqFunc1
#define osqFunc2 

/* qualifiers for inline functions (one qualifier for each function) */
#define osqInlineGetHighPrioBit
/* KB end osekHwMemoryQualifierDefines */

/* include hardware specific header files before OSEK Typedefinitions */
/* KB begin osekHWincludeHwHeaderPreTypes */
/* KB end osekHWincludeHwHeaderPreTypes */

#if defined USE_QUOTE_INCLUDES
 #include "osekasrt.h" /* posix types must be defined      */
#else
 #include <osekasrt.h>
#endif
#if defined USE_QUOTE_INCLUDES
 #include "osekerr.h"  /* definitions of all error numbers */
#else
 #include <osekerr.h>
#endif
#if defined USE_QUOTE_INCLUDES
 #include "osekcov.h"  /* coverage definitions */
#else
 #include <osekcov.h>
#endif

/* state of tasks */
#define PRE_READY       0
#define RUNNING         1
#define WAITING         2
#define READY           3
#define SUSPENDED       4
#define INVALID_TASK ((TaskType)(0xFFFFFFFFUL))  


/* KB begin osekHwTASKmacro */
/* OSEK-conform task-function-macro */
#define TASK(x)	 void x##func(void)
/* KB end osekHwTASKmacro */
/* KB begin osekHWISRmacro */
/* OSEK-conform interrupt-function-macro */
#define ISR(x) void x(void)
/* KB end osekHWISRmacro */
/* KB begin osekHWAlrmCallbckmacro */
/* OSEK-conform function-macro for alarm callbacks */
#define ALARMCALLBACK(x) void x(void)
/* KB end osekHWAlrmCallbckmacro */
/* Default application mode is generated (OSEK 2.2) */

/* implementation specific constants */
/* KB begin osekConstants */
#define osdPrioBitsPerWord 32
/* KB end osekConstants */


/* typesdefs  */
/* KB begin osekHWosTaskFctType */
typedef void                (*osTaskFctType)(void);
/* KB end osekHWosTaskFctType */
typedef uint16              osStackUsageType;
typedef uint8               osTaskStateType;
/* KB begin osekHWosStackPtrType */
typedef void *              osStackPtrType;
/* KB end osekHWosStackPtrType */
typedef uint8               osResCounterType;

typedef uint32             TickType;
typedef int32              osHeapTimeDiffType;


/* implementation specific types */
/* KB begin osekTypes */
/*
    !!! Max priorities: Min(2^(sizeof(osPrioType)*8), 2^(sizeof(osPrioOffsetType)*8) * sizeof(osPrioFlagType)*8)

*/
typedef uint16       osPrioType;         /* has to be unsigned (invalid is higher than others) */
typedef uint8        osPrioOffsetType;
typedef uint32       osPrioFlagType;
typedef uint32       osResAccessMaskType;

typedef uint32       osStartFlagType;
typedef uint8        osTypeIntLevel;
typedef uint8        osContextOverlayType;
typedef uint8        osTaskPreemptionType;
typedef uint8        osTaskFlagType;
typedef uint8        osQEntryCountType;
typedef uint8        osActivationCountType;
typedef uint32       osStackDataType;
typedef uint32       osStackSizeType;
typedef uint16       osTaskIndexType;
typedef uint16       osResourceIndexType;
typedef uint16       osAlarmIndexType;

/* implementation specific OSEK types */
/* KB end osekTypes */

/* OSEK types */
/* implementation specific OSEK types */
/* KB begin osekOSEKTypes */
typedef uint32             osStdFlagType;
typedef osTaskIndexType    TaskType;
typedef osResourceIndexType ResourceType;
typedef osAlarmIndexType   AlarmType;
#define osdAutomaticEventMaskType /* enables automatic size calculation */

typedef uint8              osIntLevelType;


/* KB end osekOSEKTypes */
/* KB begin osekDispatcherLockType */
typedef uint8               osDispatcherLockType;
/* KB end osekDispatcherLockType */

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK            0
typedef uint8               StatusType;
#endif
typedef uint8               TaskStateType;
typedef uint8               AppModeType;
typedef TaskType            *TaskRefType;
typedef TaskStateType       *TaskStateRefType;
typedef TickType            *TickRefType;
typedef EventMaskType       *EventMaskRefType;

/* KB begin osekOSEKdeclarations */
#define DeclareTask(TaskId)     extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed1##TaskId
#define DeclareResource(ResId)  extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed2##ResId
#define DeclareEvent(Event)     extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed3##Event
#define DeclareAlarm(AlarmId)   extern osqRAM1 osqRAM2 TaskType osqRAM3 osNotUsed4##AlarmId
/* KB end osekOSEKdeclarations */

/* structure for AlarmBase */
typedef struct AlarmBaseTypeStruct
{
   TickType maxallowedvalue;/* Maximum possible allowed count value (for example */
                            /* in ticks or angle degrees) */
   TickType ticksperbase;   /* Number of ticks required to reach a counter-specific */
                            /* (significant) unit. */
   TickType mincycle;       /* Minimum allowed number of ticks for a cyclic alarm */
                            /* (only for systems with Extended Status). */
} AlarmBaseType;

typedef AlarmBaseType     *AlarmBaseRefType;

typedef AlarmType         osHeapType;

typedef uint16              OSServiceIdType;
typedef void                (*osAlarmCallbackType)(void);

typedef union osAPIParam1TypeUnion
{
   TaskType      osTaskType;     
   TaskRefType   osTaskRefType;   
   ResourceType  osResourceType;  
   EventMaskType osEventMaskType; 
   AlarmType     osAlarmType;  
} osAPIParam1Type;

typedef union osAPIParam2TypeUnion
{
   TaskStateRefType osTaskStateRefType; 
   EventMaskType    osEventMaskType;    
   EventMaskRefType osEventMaskRefType; 
   AlarmBaseRefType osAlarmBaseRefType; 
   TickType         osTickType;         
   TickRefType      osTickRefType;      
} osAPIParam2Type;



#if osdORTIDebug
 #define osdTestMacros 1 
#endif

#ifdef osdUseTrace
 #define osdTestMacros 2
#endif

#ifndef osdTestMacros
   #if defined USE_QUOTE_INCLUDES
    #include "emptymac.h"          
   #else
    #include <emptymac.h>
   #endif
#else                           
   #if (osdTestMacros == 1)
      #if defined USE_QUOTE_INCLUDES
       #include "testmac1.h"
      #else
       #include <testmac1.h>
      #endif
   #endif
   #if (osdTestMacros == 2)
      #if defined USE_QUOTE_INCLUDES
       #include "testmac2.h"
      #else
       #include <testmac2.h>
      #endif
   #endif
   #if (osdTestMacros == 3)
      #if defined USE_QUOTE_INCLUDES
       #include "testmac3.h"
      #else
       #include <testmac3.h>
      #endif
   #endif
   #if (osdTestMacros == 4)
      #if defined USE_QUOTE_INCLUDES
       #include "testmac4.h"
      #else
       #include <testmac4.h>
      #endif
   #endif
#endif /*osdTestMacros*/


/* KB begin osekHWVariableSectionStartHeader */
/* KB end osekHWVariableSectionStartHeader */

/* KB begin osekCtrlVarDecl */
/* control variables */
extern osqRAM1 osqRAM2 osPrioType osqRAM3 osActiveTaskPrio;     /* Priority of actually active task */
extern osqRAM1 osqRAM2 TaskType   osqRAM3 osActiveTaskIndex;    /* indicates the currently running task */
extern osqRAM1 osqRAM2 osPrioType osqRAM3 osHighReadyTaskPrio;  /* Priority of ready task with highest priority */
extern osqRAM1 osqRAM2 TaskType   osqRAM3 osHighReadyTaskIndex; /* ready task with highest priority */
extern osqBOOL1 volatile osqBOOL2 boolean osqBOOL3 osStartDispatcher;
extern osqRAM1 osqRAM2 uint8 osqRAM3 osIntNestingDepth;
extern osqRAM1 osqRAM2 uint8 osqRAM3 osIntSaveDisableCounter;
/* KB end osekCtrlVarDecl */

/* error handling                      */
#if STATUS_LEVEL == EXTENDED_STATUS
#ifndef osdSuppressFilenames 
/* variables for error handling */
extern osqRAM1 osqRAM2 uint16 osqRAM3 osActiveTaskLineNumber;
/* 
   The following line will violate MISRA rule 6.3 but the char data type is necessary 
   because the assigned __FILE__ will otherwise cause errors on some compilers
*/
extern osqPROM1 const osqPROM2 char osqPROM3 * osqPRAM3 osActiveTaskModule; /* PRQA S 5013 */
#endif
#endif

#if osdORTIDebug
extern osqRAM1 volatile osqRAM2 uint8 osqRAM3 osORTICurrentServiceId;
#endif

#if (STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions)
extern volatile osqRAM1 osqRAM2 uint8 osqRAM3 osIntAPIStatus;
#endif

/* KB begin osekHWDeclareOsSystemCounterExtern */
#if (osdUseCounterAPI==0)
extern osqRAM1 volatile osqRAM2 TickType osqRAM3 osSystemCounter;
#endif
/* KB end osekHWDeclareOsSystemCounterExtern */


/* KB begin osekHWDefineDummyRead */
#define osdDummyRead(x) (x) = (x); /* to avoid compiler warning */
/* KB end osekHWDefineDummyRead */

#if osdNumberOfAppModes > 1
extern osqRAM1 osqRAM2 uint8 osqRAM3 osActiveAppMode;
#endif
#if osdParameterAccessMacros && osdErrorHook
extern osqRAM1 osqRAM2 osAPIParam1Type osqRAM3 osAPIParam1;
extern osqRAM1 osqRAM2 osAPIParam2Type osqRAM3 osAPIParam2;
extern osqRAM1 osqRAM2 TickType osAPIParam3;
#endif

#if osdErrorHook
/* KB begin osekExternGlobalErrorVariables */
extern osqRAM1 osqRAM2 uint16 osqRAM3 osLastError;
#if (osdORTIDebug || defined(osdGHSDebugSupport))
extern osqRAM1 osqRAM2 StatusType osqRAM3 osLastErrorCode;
#endif
/* KB end osekExternGlobalErrorVariables */
#endif

/* KB begin osekHWVariableSectionEndHeader */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section data=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekHWVariableSectionEndHeader */


osqFunc1 void osqFunc2 osAbortSystem(uint16 uiError);

/* KB begin osekBNCdispatcherHeader */
/* KB end osekBNCdispatcherHeader */

/* OSEK-Prototypes */
/* system services */
#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdActivateTaskNotUsed
#define ActivateTask(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osActivateTask(x))
#endif

#ifndef osdTerminateTaskNotUsed
#define TerminateTask() (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osTerminateTask())
#endif

#ifndef osdChainTaskNotUsed
#define ChainTask(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osChainTask(x))
#endif

#ifndef osdScheduleNotUsed
#define Schedule() (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSchedule())
#endif

#ifndef osdGetTaskIDNotUsed
#define GetTaskID(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskID(x))
#endif

#ifndef osdGetTaskStateNotUsed
#define GetTaskState(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetTaskState((x), (y)))
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdActivateTaskNotUsed
#define ActivateTask(x) (osActivateTask(x))
#endif
#ifndef osdTerminateTaskNotUsed
#define TerminateTask() osTerminateTask()
#endif
#ifndef osdChainTaskNotUsed
#define ChainTask(x) (osChainTask(x))
#endif
#ifndef osdScheduleNotUsed
#define Schedule() osSchedule()
#endif
#ifndef osdGetTaskIDNotUsed
#define GetTaskID(x) (osGetTaskID(x))
#endif
#ifndef osdGetTaskStateNotUsed
#define GetTaskState(x, y) (osGetTaskState((x), (y)))
#endif

#endif /*STATUS_LEVEL...*/

#ifndef osdActivateTaskNotUsed
osqFunc1 StatusType osqFunc2 osActivateTask( TaskType taskIndex);
#endif
#ifndef osdTerminateTaskNotUsed
osqFunc1 StatusType osqFunc2 osTerminateTask ( void );
#endif
#ifndef osdChainTaskNotUsed
osqFunc1 StatusType osqFunc2 osChainTask ( TaskType taskIndex );
#endif
#ifndef osdScheduleNotUsed
osqFunc1 StatusType osqFunc2 osSchedule ( void );              
#endif
#ifndef osdGetTaskIDNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskID ( TaskRefType TaskId); 
#endif
#ifndef osdGetTaskStateNotUsed
osqFunc1 StatusType osqFunc2 osGetTaskState(TaskType TaskId, TaskStateRefType State);
#endif


/* interrupt handling */

osqFunc1 void osqFunc2 osAssertFailed(void);
/* KB begin osekHwOsekGlobalPrototypes */

/* implementation specific prototypes */

boolean osCheckInterruptsEnabled(void);    /* return TRUE if interrupts enabled */
boolean osCheckInterruptsDisabled(void);   /* return TRUE if interrupts disabled */

#if (osdNumberOfInterruptResources > 0)
extern uint8 osInitialIntLevel;
#define osGetInitialLevel() osInitialIntLevel
#endif

osqFunc1 void osqFunc2 osInitISRprios(void);

#define osSetAlarmLevel() osEnableLevel()    /* set to level of alarm interrupt */

extern uint8 osSavedGlobalInt;

extern osqRAM1 osqRAM2 uint8 osqRAM3 osIntSaveDisableCounter;
extern uint8 osSavedIntGlobalNested;
extern uint8 osIntSaveDisableCounterGlobal;

boolean osGlobalInterruptsEnabled(void);

#if (osdCompiler==osdCompilerGHSARM7)
void osEnableLevel(void);
void osDisableLevel(void);
#endif

#if ((osdThumbMode == 1) || (osdCompiler==osdCompilerTIARM7))

/* prototypes for interrupt handling functions implemented as functions for THUMB mode only */
#if ((osdTasksInUserMode==1) || (osdCompiler==osdCompilerTIARM7))
void osEnableLevel(void);
void osDisableLevel(void);
void osSaveDisableLevel(void);              /* save level and disable up to system level, no nesting */
void osRestoreEnableLevel(void);              /* save level and disable up to system level, no nesting */
void osSaveDisableLevelNested(void);              /* save level and disable up to system level, no nesting */
void osRestoreEnableLevelNested(void);              /* save level and disable up to system level, no nesting */
#endif

void osDisableGlobal(void);                 /* disable all interrupts, no nesting */
void osEnableGlobal(void);                  /* enable all interrupts, no nesting */
void osSaveDisableLevelNested(void);        /* save level and disable up to system level, with nesting */
void osSaveDisableGlobal(void);             /* save and disable all interrupts, no nesting */
void osRestoreEnableGlobal(void);           /* restore and enable all interrupts, no nesting */
void osSaveDisableGlobalNested(void);       /* disable all interrupts, with nesting */
void osRestoreEnableGlobalNested(void);     /* restore all interrupts, with nesting */

#else

/* interrupt handling functions implemented as inline assembler macros for ARM mode only */

#if (osdCompiler==osdCompilerGHSARM7)

asm void osSaveDisableLevel()        /* save level and disable up to system level, no nesting */
{
%lab mylbl

  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWISaveDisableLevel
  swieq #0x80
  beq mylbl
#endif
  ldr a2, =osSavedIntLevel
  mov a3, a1
  orr a3, a3, #0x80
  msr CPSR_c, a3
  strb a1, [a2]
mylbl:
}


asm void osRestoreEnableLevel()        /* restore level and disable up to system level, no nesting */
{
%lab mylbl

  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWIRestoreEnableLevel
  swieq #0x80
  beq mylbl
#endif
  ldr a2, =osSavedIntLevel 
  ldrb a3, [a2]              
  bic a1, a1, #0b10000000
  bic a3, a3, #0b01111111
  orr a1, a1, a3
  msr CPSR_c, a1
mylbl:
}

asm void osSaveDisableLevelNested()        /* save level and disable up to system level, with nesting */
{
%lab mylbl

  ldr a3, =osIntSaveDisableCounter  ; a3 = &osIntSaveDisableCounter
  ldrb a2, [a3]                  ; a2 = osIntSaveDisableCounter
  tst a2, a2

  bne mylbl
  ; if osIntSaveDisableCounter == 0 save level
  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWISaveDisableLevelNested
  swieq #0x80
  beq mylbl
#endif
  mov a4, a1
  orr a1, a1, #0x80
  msr CPSR_c, a1  
  ldr a1, =osSavedIntLevelNested
  strb a4, [a1]         

mylbl:
  ; increment osIntSaveDisableCounter
  add a2,a2,#1
  strb a2,[a3]
}


asm void osRestoreEnableLevelNested()        /* restore level and disable up to system level, with nesting */
{
%lab mylbl

  ldr a3, =osIntSaveDisableCounter  ; a3 = &osIntSaveDisableCounter
  ldrb a4, [a3]                  ; a4 = osIntSaveDisableCounter
  subs a4,a4,#1                    ; decrement osIntSaveDisableCounter
  strb a4, [a3]
  bne mylbl

  ; osIntSaveDisableCounter == 0 restore level0
  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWIRestoreEnableLevelNested
  swieq #0x80
  beq mylbl
#endif
  ldr a2, =osSavedIntLevelNested 
  ldrb a3, [a2]

  bic a1, a1, #0b10000000
  bic a3, a3, #0b01111111
  orr a1, a1, a3
  msr CPSR_c, a1

mylbl:
}


asm void osSaveDisableGlobalNested()        /* save and disable all interrupts, with nesting */
{
%lab mylbl

  ldr a3, =osIntSaveDisableCounterGlobal  ; a3 = &osIntSaveDisableCounterGlobal
  ldrb a2, [a3]                  ; a2 = osIntSaveDisableCounterGlobal
  tst a2, a2

  bne mylbl
  ; if osIntSaveDisableCounterGlobal == 0 save ints
  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWISaveDisableGlobalNested
  swieq #0x80
  beq mylbl
#endif
  mov a4, a1
  orr a1, a1, #0xC0
  msr CPSR_c, a1  
  ldr a1, =osSavedIntGlobalNested
  strb a4, [a1]         

mylbl:
  ; increment osIntSaveDisableCounterGlobal
  add a2,a2,#1
  strb a2,[a3]
}


asm void osRestoreEnableGlobalNested()        /* restore all interrupts, with nesting */
{
%lab mylbl

  ldr a3, =osIntSaveDisableCounterGlobal  ; a3 = &osIntSaveDisableCounterGlobal
  ldrb a4, [a3]                  ; a4 = osIntSaveDisableCounterGlobal
  subs a4,a4,#1                    ; decrement osIntSaveDisableCounterGlobal
  strb a4, [a3]
  bne mylbl

  ; osIntSaveDisableCounterGlobal == 0 restore ints
  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWIRestoreEnableGlobalNested
  swieq #0x80
  beq mylbl
#endif
  ldr a2, =osSavedIntGlobalNested 
  ldrb a3, [a2]

  bic a1, a1, #0b11000000
  bic a3, a3, #0b00111111
  orr a1, a1, a3
  msr CPSR_c, a1

mylbl:
}


asm void osDisableGlobal()        /* disable all interrupts, no nesting */
{
%lab mylbl

  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWIDisableGlobal
  swieq #0x80
  beq mylbl
#endif
  orr a1, a1, #0b11000000
  msr CPSR_c, a1
  nop
mylbl:
}


asm void osEnableGlobal()        /* Enable all interrupts, no nesting */
{
%lab mylbl

  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWIEnableGlobal
  swieq #0x80
  beq mylbl
#endif
  bic a1, a1, #0b11000000
  msr CPSR_c, a1
mylbl:
}


asm void osSaveDisableGlobal()       /*   save and disable all interrupts, no nesting */
{
%lab mylbl

  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWISaveDisableGlobal
  swieq #0x80
  beq mylbl
#endif
  ldr a2, =osSavedGlobalInt
  mov a3, a1
  orr a3, a3, #0b11000000
  msr CPSR_c, a3
  strb a1, [a2]
mylbl:
}


asm void osRestoreEnableGlobal()     /*   restore and enable all interrupts, no nesting */
{
%lab mylbl

  mrs a1, CPSR
#if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,=osSWIRestoreEnableGlobal
  swieq #0x80
  beq mylbl
#endif
  ldr a2, =osSavedGlobalInt 
  ldrb a3, [a2]              
  bic a1, a1, #0b11000000
  bic a3, a3, #0b00111111
  orr a1, a1, a3
  msr CPSR_c, a1
mylbl:
}
#endif /* #if (osdCompiler==osdCompilerGHSARM7) */

#endif


/* KB end osekHwOsekGlobalPrototypes */

#define osdIntAPIDisableAll 0x80
 
#if (STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions)
   #define osSetIntAPIStatus(x) (osIntAPIStatus = (x))
   #define osIncIntAPIStatus() (osIntAPIStatus++)
   #define osDecIntAPIStatus() (osIntAPIStatus--)
#else
   #define osSetIntAPIStatus(x)
   #define osIncIntAPIStatus()
   #define osDecIntAPIStatus()
#endif

#ifndef osdEnableAllInterruptsNotUsed
osqFunc1 void osqFunc2 EnableAllInterrupts(void);
#endif

#ifndef osdDisableAllInterruptsNotUsed
osqFunc1 void osqFunc2 DisableAllInterrupts(void);
#endif

#ifndef osdResumeOSInterruptsNotUsed
osqFunc1 void osqFunc2 ResumeOSInterrupts(void);
#endif

#ifndef osdSuspendOSInterruptsNotUsed
osqFunc1 void osqFunc2 SuspendOSInterrupts(void);
#endif

#ifndef osdResumeAllInterruptsNotUsed
osqFunc1 void osqFunc2 ResumeAllInterrupts(void);
#endif

#ifndef osdSuspendAllInterruptsNotUsed
osqFunc1 void osqFunc2 SuspendAllInterrupts(void);
#endif


/* resource management */

#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdGetResourceNotUsed
#define GetResource(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetResource(x))
#endif


#ifndef osdReleaseResourceNotUsed
#define ReleaseResource(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osReleaseResource(x))
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdGetResourceNotUsed
#define GetResource(x) (osGetResource(x))
#endif
#ifndef osdReleaseResourceNotUsed
#define ReleaseResource(x) (osReleaseResource(x))
#endif

#endif /*STATUS_LEVEL...*/


#ifndef osdGetResourceNotUsed
osqFunc1 StatusType osqFunc2 osGetResource(ResourceType ResId);
#endif
#ifndef osdReleaseResourceNotUsed
osqFunc1 StatusType osqFunc2 osReleaseResource(ResourceType ResId); 
#endif

/* KB begin osekSemaphores */
/* KB end osekSemaphores */
/* event control */
#if (CC == ECC1) || (CC == ECC2)

#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdSetEventNotUsed
#define SetEvent(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetEvent((x), (y)))
#endif

#ifndef osdGetEventNotUsed
#define GetEvent(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetEvent((x), (y)))
#endif

#ifndef osdClearEventNotUsed
#define ClearEvent(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osClearEvent(x))
#endif
   
#ifndef osdWaitEventNotUsed
#define WaitEvent(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osWaitEvent(x))
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdSetEventNotUsed
#define SetEvent(x, y) (osSetEvent((x), (y)))
#endif

#ifndef osdGetEventNotUsed
#define GetEvent(x, y) (osGetEvent((x), (y)))
#endif

#ifndef osdClearEventNotUsed
#define ClearEvent(x) (osClearEvent(x))
#endif

#ifndef osdWaitEventNotUsed
#define WaitEvent(x) (osWaitEvent(x))
#endif

#endif /*STATUS_LEVEL...*/

#ifndef osdSetEventNotUsed
osqFunc1 StatusType osqFunc2 osSetEvent(TaskType taskIndex, EventMaskType setMask);
#endif
#ifndef osdClearEventNotUsed
osqFunc1 StatusType osqFunc2 osClearEvent (EventMaskType Mask );                  
#endif
#ifndef osdGetEventNotUsed
osqFunc1 StatusType osqFunc2 osGetEvent(TaskType taskIndex, EventMaskRefType Event); 
#endif
#ifndef osdWaitEventNotUsed
osqFunc1 StatusType osqFunc2 osWaitEvent(EventMaskType mask);                    
#endif

#else /*CC...*/
#ifndef osdSetEventNotUsed
#define SetEvent(x, y) (SetEvent with wrong conformance class)
#endif
#ifndef osdClearEventNotUsed
#define ClearEvent (x) (SetEvent with wrong conformance class)
#endif
#ifndef osdGetEventNotUsed
#define GetEvent(x, y) (SetEvent with wrong conformance class)
#endif
#ifndef osdWaitEventNotUsed
#define WaitEvent(x)   (SetEvent with wrong conformance class)
#endif

#endif /*CC...*/


/* alarms        */
#if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)

#ifndef osdGetAlarmBaseNotUsed
#define GetAlarmBase(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetAlarmBase((x), (y)))
#endif

#ifndef osdSetRelAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetRelAlarmDefine1 */
#define SetRelAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetRelAlarm((x), (y), (z)))
/* KB end osekHwSetRelAlarmDefine1 */
#else
#define SetRelAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetRelAlarm(x))
#endif
#endif

#ifndef osdSetAbsAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetAbsAlarmDefine1 */
#define SetAbsAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetAbsAlarm((x), (y), (z)))
/* KB end osekHwSetAbsAlarmDefine1 */
#else
#define SetAbsAlarm(x, y, z) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osSetAbsAlarm(x))
#endif
#endif

#ifndef osdCancelAlarmNotUsed
#define CancelAlarm(x) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osCancelAlarm(x))
#endif
   
#ifndef osdGetAlarmNotUsed
#define GetAlarm(x, y) (osActiveTaskLineNumber= __LINE__,\
   osActiveTaskModule= __FILE__,\
   osGetAlarm((x), (y)))
#endif

#else /*STATUS_LEVEL...*/

#ifndef osdGetAlarmBaseNotUsed
#define GetAlarmBase(x, y) (osGetAlarmBase((x), (y)))
#endif

#ifndef osdSetRelAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetRelAlarmDefine2 */
#define SetRelAlarm(x, y, z) (osSetRelAlarm((x), (y), (z)))
/* KB end osekHwSetRelAlarmDefine2 */
#else
#define SetRelAlarm(x, y, z) (osSetRelAlarm(x))
#endif
#endif

#ifndef osdSetAbsAlarmNotUsed
#if (osdNumberOfVarAlarms > 0)
/* KB begin osekHwSetAbsAlarmDefine2 */
#define SetAbsAlarm(x, y, z) (osSetAbsAlarm((x), (y), (z)))
/* KB end osekHwSetAbsAlarmDefine2 */
#else
#define SetAbsAlarm(x, y, z) (osSetAbsAlarm(x))
#endif
#endif

#ifndef osdCancelAlarmNotUsed
#define CancelAlarm(x) (osCancelAlarm(x))
#endif
   
#ifndef osdGetAlarmNotUsed
#define GetAlarm(x, y) (osGetAlarm((x), (y)))
#endif

#endif /*STATUS_LEVEL...*/

#ifndef osdGetAlarmBaseNotUsed
osqFunc1 StatusType osqFunc2 osGetAlarmBase(AlarmType AlarmId, AlarmBaseRefType Info);
#endif
#ifndef osdSetRelAlarmNotUsed
/* KB begin osekHWosSetRelAlarmPrototype */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID, TickType deltaTicks,
               TickType cycle);
#else
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID);
#endif
/* KB end osekHWosSetRelAlarmPrototype */
#endif
#ifndef osdSetAbsAlarmNotUsed
/* KB begin osekHWosSetAbsAlarmPrototype */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID, TickType start,
               TickType cycle);
#else
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID);
#endif
/* KB end osekHWosSetAbsAlarmPrototype */
#endif
#ifndef osdCancelAlarmNotUsed
osqFunc1 StatusType osqFunc2 osCancelAlarm(AlarmType alarmID);
#endif
#ifndef osdGetAlarmNotUsed
osqFunc1 StatusType osqFunc2 osGetAlarm(AlarmType alarmID, TickRefType diffTickTime);  
#endif

/* definitions for static alarms */
#define OS_STATIC_ALARM_TIME 0
#define OS_STATIC_CYCLE_TIME 0

/* Operating system execution control */
#if osdShutdownHook
osqFunc1 void osqFunc2 osShutdownOS(StatusType Error);
#define ShutdownOS(x) (osShutdownOS(x))
#else
osqFunc1 void osqFunc2 osShutdownOS(void);
#define ShutdownOS(x) (osShutdownOS())
#endif

osqFunc1 void osqFunc2 osStartOSasm(void);

#if (osdNumberOfAppModes > 1)
#define StartOS(x) \
 osActiveAppMode=x; \
 osStartOSasm()
#else
 #define StartOS(x) osStartOSasm()
#endif

#ifndef osdGetActiveApplModeNotUsed
/* return application mode depending on numer of AppModes */
 #if (osdNumberOfAppModes > 1)
  #define GetActiveApplicationMode() (osActiveAppMode)
 #else
  #define GetActiveApplicationMode() (OSDEFAULTAPPMODE)
 #endif
#endif

/* KB begin osekCallBackSectionStart */
/* KB end osekCallBackSectionStart */

/* hook routines */
osqFunc1 void osqFunc2 ErrorHook (StatusType Error);
osqFunc1 void osqFunc2 StartupHook (void);
osqFunc1 void osqFunc2 ShutdownHook (StatusType Error);

#if defined osdTestMacros || osdLib
osqFunc1 void osqFunc2 PreTaskHook (void);
osqFunc1 void osqFunc2 PostTaskHook (void);
#endif
osqFunc1 void osqFunc2 osPreTaskHook (void);
osqFunc1 void osqFunc2 osPostTaskHook (void);

/* KB begin osekCallBackSectionEnd */
/* KB end osekCallBackSectionEnd */

#if osdStackCheck
osqFunc1 osStackUsageType osqFunc2 osGetStackUsage(TaskType taskId);
/* KB begin osekStackUsagePrototypes */
/* implementation specific prototypes for stack usage calculation */
osStackUsageType osGetSystemStackUsage(void);

/* KB end osekStackUsagePrototypes */
#endif

#ifdef osdUseGeneratedFastAlarm
#define osdUseHeapAlarm 0
#else
#define osdUseHeapAlarm 1
#endif

#if !defined osdProvideTimeStamp
#define osdProvideTimeStamp 0
#endif

/* KB begin osekHWosExternGetSystemCounter */
#if (osdUseCounterAPI!=0)
 #define osdSystemCounterVar osCounter[0]
#else
 #define osdSystemCounterVar osSystemCounter
#endif
 #define osGetSystemCounter() osdSystemCounterVar
/* KB end osekHWosExternGetSystemCounter */

/* structure for version and variant coding */
typedef struct osVersionVariantCodingTypeStruct
{
   uint8 ucMagicNumber1;     /* magic number (e.g. for identification in hex files) */
   uint8 ucMagicNumber2;     /* defined as uint8 for independancy of byte order */
   uint8 ucMagicNumber3;
   uint8 ucMagicNumber4;
   uint8 ucSysVersionMaj;    /* version of operating system, Major */
   uint8 ucSysVersionMin;    /* version of operating system, Minor */
   uint8 ucGenVersionMaj;    /* version of code generator */
   uint8 ucGenVersionMin;    /* version of code generator */
   uint8 ucSysVariant;       /* general variant coding    */
   /* KB begin osekTypeSpecVersionVariant */
   /* typedef of implementation specific variant coding */
   uint16 ucSpecVariant;
   /* KB end osekTypeSpecVersionVariant */
} osVersionVariantCodingType;

/* KB begin osekConstSectionHeaderStart */
/* KB end osekConstSectionHeaderStart */
extern osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant;
/* KB begin osekConstSectionHeaderEnd */
/* KB end osekConstSectionHeaderEnd */


/* setup defines for filling up the variant coding structure */
#if CC == BCC1
   #define osdVariantCC 0U
#elif CC == BCC2
   #define osdVariantCC 1U
#elif CC == ECC1
   #define osdVariantCC 2U
#elif CC == ECC2
   #define osdVariantCC 3U
#else
#error wrong conformance class
#endif

#if STATUS_LEVEL == EXTENDED_STATUS
   #define osdVariantStatus 1U
#else
   #define osdVariantStatus 0U
#endif

#if osdNonPreempt
   #define osdVariantSchedule 0U
#elif osdFullPreempt
   #define osdVariantSchedule 1U
#elif osdMixedPreempt
   #define osdVariantSchedule 2U
#else
#error wrong scheduling policy
#endif


#if osdStackCheck
   #define osdVariantStackcheck 1U
#else
   #define osdVariantStackcheck 0U
#endif


#ifndef osdSuppressFilenames
   #define osdVariantModulnamesEnabled 1U
#else
   #define osdVariantModulnamesEnabled 0U
#endif

#if osdEnableAssertions
   #define osdVariantAssertionsEnabled 1U
#else
   #define osdVariantAssertionsEnabled 0U
#endif

/* KB begin osekDefineSpecVersionVariant */
#if osdORTIDebug
   #define osdVariantORTI 1
#else
   #define osdVariantORTI 0
#endif

#if osdThumbMode
   #define osdVariantThumbMode 1
#else
   #define osdVariantThumbMode 0
#endif

#if osdRegisterOpt
   #define osdVariantRegisterOpt 1
#else
   #define osdVariantRegisterOpt 0
#endif

#if (osdTMS470R1==1)
   #define osdVariantDerivative 0
#endif
#if (osdTMS470R1_SE==1)
   #define osdVariantDerivative 1
#endif
#if (osdTMS470_PL2002EX==1)
   #define osdVariantDerivative 2
#endif
#if (osdTMS470PVF24==1)
   #define osdVariantDerivative 3
#endif

/* KB end osekDefineSpecVersionVariant */


#ifdef osdIncludeLibConfig

   /* include now tcb.h, but only for the application code
    * include both parts of the tcb, pre and post
    */

   #define osdPreTCB  0
   #define osdPostTCB 1

   #if (osdLibVariant == 1)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib1.h"
   #else
    #include <oseklib1.h>
   #endif
   #endif
   
   #if (osdLibVariant == 2)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib2.h"
   #else
    #include <oseklib2.h>
   #endif
   #endif
   
   #if (osdLibVariant == 3)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib3.h"
   #else
    #include <oseklib3.h>
   #endif
   #endif
   
   #if (osdLibVariant == 4)
   #if defined USE_QUOTE_INCLUDES
    #include "oseklib4.h"
   #else
    #include <oseklib4.h>
   #endif
   #endif

   #undef osdPreTCB 
   #undef osdPostTCB
      

#else

   #define osdPreTCB  0
   #define osdPostTCB 1
   #if defined USE_QUOTE_INCLUDES
    #include "tcb.h"
   #else
    #include <tcb.h>
   #endif
   #undef osdPreTCB 
   #undef osdPostTCB

#endif


#ifndef osdTestMacros
#define osTestMacrosPreTaskHook osInterPreTaskHook
#define osTestMacrosPostTaskHook osInterPostTaskHook
#endif

#if defined USE_QUOTE_INCLUDES
 #include "osekcom.h"
#else
 #include <osekcom.h>
#endif

/* include hardware specific header files after OSEK Typedefinitions */
/* KB begin osekHWincludeHwHeaderPostTypes */
/* KB end osekHWincludeHwHeaderPostTypes */

/* KB begin osekEndOfHModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section data=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfHModule */

/*lint -restore re-enable messages*/

#endif/* double include preventer */

/* END OF HEADER osek.h */

