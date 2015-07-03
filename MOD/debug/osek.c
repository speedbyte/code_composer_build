/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osek.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.66
|
|  Description: osek startup, scheduler and dispatcher
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

/* CovComment 0:  file: osek.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

/* KB begin osekHWVariableSectionStart */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#endif
/* KB end osekHWVariableSectionStart */

/* control variables */
osqRAM1 osqRAM2 osPrioType osqRAM3 osActiveTaskPrio;     /* Priority of actually active task */
osqRAM1 osqRAM2 osPrioType osqRAM3 osHighReadyTaskPrio;  /* Priority of ready task with highest priority */
osqRAM1 osqRAM2 TaskType   osqRAM3 osActiveTaskIndex;    /* inicates the currently running task */
osqRAM1 osqRAM2 TaskType   osqRAM3 osHighReadyTaskIndex; /* ready task with highest priority */
osqBOOL1 volatile osqBOOL2 boolean osqBOOL3 osStartDispatcher;
osqRAM1 osqRAM2 uint8 osqRAM3 osIntNestingDepth;   /* PRQA S 1504 */ /* might be used by other modules */
osqRAM1 osqRAM2 uint8 osqRAM3 osIntSaveDisableCounter;   /* PRQA S 1504 */ /* might be used by other modules */

#if osdStackCheck
osqRAM1 osqRAM2 TaskType   osqRAM3 osLastActiveTaskIndex;    
#endif

#if defined osdEnableCoverage
osqRAM1 osqRAM2 uint8 osqRAM3 osCoverage[osdCoverageArraySize];
#endif

#if STATUS_LEVEL == EXTENDED_STATUS
#ifndef osdSuppressFilenames 
/* variables for error handling */
osqRAM1 osqRAM2 uint16 osqRAM3 osActiveTaskLineNumber;
/* 
   The following line will violate MISRA rule 6.3 but the char data type is necessary 
   because the assigned __FILE__ will otherwise cause errors on some compilers
*/
osqPROM1 const osqPROM2 char osqPROM3 * osqPRAM3 osActiveTaskModule; /* PRQA S 5013 */ 
#endif
#if osdEnableAssertions
volatile osqRAM1 osqRAM2 uint8 osqRAM3 osIntAPIStatus;
#endif
osqRAM1 osqRAM2 osResCounterType osqRAM3 osResGlobalOccupationCounter;
#endif

#if (osdUseCounterAPI==0)
osqRAM1 volatile osqRAM2 TickType osqRAM3 osSystemCounter;
#endif

/* KB begin osekHwDeclLockDispatcher */
#if (osdFullPreempt || osdMixedPreempt)
osqRAM1 volatile osqRAM2 osDispatcherLockType osqRAM3 osLockDispatcher;
#endif
/* KB end osekHwDeclLockDispatcher */

/* KB begin osekHwSavedIntLevelVariables */
osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntLevel;        /* PRQA S 1504 */ /* might be used by other modules */
osqRAM1 osqRAM2 osIntLevelType osqRAM3 osSavedIntLevelNested;  /* PRQA S 1504 */ /* might be used by other modules */
/* KB end osekHwSavedIntLevelVariables */

#if osdErrorHook
osqBOOL1 osqBOOL2 boolean osqBOOL3 osErrorFlag;
#endif

#if (osdNumberOfAlarms > 0)
   #ifndef osdUseGeneratedFastAlarm
   osqRAM1 osqRAM2 osAlarmIndexType osqRAM3 osAlarmHeapCount;    /* Number of heap elements */
   #endif
#endif

#if osdORTIDebug
osqRAM1 volatile osqRAM2 uint8 osqRAM3 osORTICurrentServiceId;
#endif

#if osdNumberOfAppModes > 1
osqRAM1 osqRAM2 uint8 osqRAM3 osActiveAppMode;
#endif
#if osdParameterAccessMacros && osdErrorHook
osqRAM1 osqRAM2 osAPIParam1Type osqRAM3 osAPIParam1;
osqRAM1 osqRAM2 osAPIParam2Type osqRAM3 osAPIParam2;
osqRAM1 osqRAM2 TickType osAPIParam3;
#endif

#if osdErrorHook
/* KB begin osekGlobalErrorVariables */
osqRAM1 osqRAM2 uint16 osqRAM3 osLastError;
#if (osdORTIDebug || defined(osdGHSDebugSupport))
osqRAM1 osqRAM2 StatusType osqRAM3 osLastErrorCode;
#endif
/* KB end osekGlobalErrorVariables */
#endif

/* KB begin dummyVariables */
/* KB end dummyVariables */

/* KB begin osekHWVariableDefinition */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#endif

uint8 osSavedGlobalInt;               /* PRQA S 1504 */ /* might be used by other modules or assembler code */
uint8 osSavedIntGlobalNested;         /* PRQA S 1504 */ /* might be used by other modules or assembler code */
uint8 osIntSaveDisableCounterGlobal;  /* PRQA S 1504 */ /* might be used by other modules or assembler code */

#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#endif

#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osstack"
#endif

#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#endif

#if (osdNumberOfInterruptResources > 0)
uint8 osInitialIntLevel;
#endif

/* KB end osekHWVariableDefinition */

/* KB begin osekHWVariableSectionEnd */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#endif
/* KB end osekHWVariableSectionEnd */

/* KB begin osekConstSectionCodeStart */
/* KB end osekConstSectionCodeStart */
osqROM1 const osqROM2 osVersionVariantCodingType osqROM3 oskVersionVariant=    /* PRQA S 1514 */ /* might be used by other modules */
{
   0xAF, /* magic number */
   0xFE,
   0xDE,
   0xAD,
   (uint8)osdVrmMajRelNumRef, /* version of operating system, Major, defined in vrm.h */
   (uint8)osdVrmMinRelNumRef, /* version of operating system, Minor */

   (uint8)osdVrmGenMajRelNumRef, /* version of code generator */
   (uint8)osdVrmGenMinRelNumRef,

   /* variant used */
   /* Bit 0..1:   Conformance Class */
   /* Bit 2:      Status level */
   /* Bit 3..4:   Scheduling policy */
   /* Bit 5:      Stackcheck */
   /* Bit 6:      EnableInternalAssertions */
   /* Bit 7:      API errors with file names */

   (uint8) (((osdVariantCC)                    ) |
            ((osdVariantStatus)            << 2) |
            ((osdVariantSchedule)          << 3) |
            ((osdVariantStackcheck)        << 5) |
            ((osdVariantModulnamesEnabled) << 6) |
            ((osdVariantAssertionsEnabled) << 7)),
   /* KB begin osekStructSpecVersionVariant */
   (uint16) (((osdORTIVersion / 10) << 8) | (osdVariantDerivative << 4) | (osdVariantRegisterOpt << 2) | (osdVariantORTI << 1) | (osdVariantThumbMode))
   /* KB end osekStructSpecVersionVariant */

};
/* KB begin osekConstSectionCodeEnd */
/* KB end osekConstSectionCodeEnd */

/* KB begin osekHWosGetHighPrioBit */

/*-+--------------------------------------------------------------------------
 / osGetHighPrioBit
 / Funktion:
 /   - calculates index of first set (priority) bit
 / Parameter:     uiReadyTasks   bit field to search
 / Returnvalue:   index of first set bit (0 = MSB)
 /--------------------------------------------------------------------------*/

inline osqFunc1 uint8 osqFunc2 osGetHighPrioBit(osPrioFlagType uiReadyTasks)     /* PRQA S 3408 */  /* PRQA S 1505 */ /* suppress QAC warnings because this is an inline function */
{
  osqROM1 static const osqROM2 uint8 osqROM3 oskPrioNibbleField[16]=
  {
   3,     /* 0000 */
   3,     /* 0001 */
   2,     /* 0010 */
   2,     /* 0011 */
   1,     /* 0100 */
   1,     /* 0101 */
   1,     /* 0110 */
   1,     /* 0111 */
   0,     /* 1000 */
   0,     /* 1001 */
   0,     /* 1010 */
   0,     /* 1011 */
   0,     /* 1100 */
   0,     /* 1101 */
   0,     /* 1110 */
   0      /* 1111 */
  };

#if (osdNumberOfPriorities > 4)
   uint8 prioTask= 0;
#endif

#if (osdPrioBitsPerWord > 16)
#if (osdNumberOfPriorities > 16)
   if ((uiReadyTasks & 0xFFFF0000U) != 0)
   {
      uiReadyTasks >>= 16;
   }
   else
   {
      prioTask+= (uint8)16;
   }
#else
#if (osdNumberOfPriorities > 8)
   uiReadyTasks >>= 16;                  
#endif
#endif
#endif


#if osdPrioBitsPerWord > 8
#if (osdNumberOfPriorities > 8)
   if ((uiReadyTasks & 0xFF00U) != 0)
   {
      uiReadyTasks >>= 8;
   }
   else
   {
      prioTask+= (uint8)8;
   }
#else
#if (osdNumberOfPriorities > 4)
   uiReadyTasks >>= 24;                 
#endif
#endif
#endif


#if (osdNumberOfPriorities > 4)
   if ((uiReadyTasks & 0xF0U) != 0)
   {
      uiReadyTasks >>= 4;
   }
   else
   {
      prioTask+= (uint8)4;
   }
   return (uint8)(prioTask + oskPrioNibbleField[uiReadyTasks]);
#else
   return (uint8)(oskPrioNibbleField[uiReadyTasks >> 28]);
#endif
}
/* KB end osekHWosGetHighPrioBit */


/*-+--------------------------------------------------------------------------
 / osSchedulePrio
 / Funktion:
 /   - calculate ready task with highest priority
 /   - set osHighReadyTaskPrio and osHighReadyTaskIndex for ready task with highest
 /     priority
 /   - osLockDispatcher must be unlocked (0)
 / Parameter:
 / Returnvalue: TRUE, if task with highest priority != active task or no task ready
 /--------------------------------------------------------------------------*/

osqFunc1 boolean osqFunc2 osSchedulePrio(void)
{
   /* CovComment 4:  Function: osSchedulePrio */
   /* calculate ready task with highest priority */
   osPrioType hiPriorTask;

#if osdRTSize > 1
   osqPRAM1 osqPRAM2 osPrioFlagType osqPRAM3 *ptr;
   osPrioFlagType uiTestPattern;
#endif

   COVER(0x00B0)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x00B1)
   osSysErrAssert(osCheckInterruptsDisabled() != 0, osdErrSPInterruptsEnabled)
#endif

#if osdRTSize > 1
   COVER(0x00B2)
   ptr= osQReadyPrios;
   uiTestPattern= *ptr;
   hiPriorTask= 0;
   while (!uiTestPattern)
   {
      COVER(0x00B3)
      ptr++;
      if (ptr > &osQReadyPrios[osrRTSize-1])
      {  /* prepare for IDLE-Loop */
         COVER(0x00B4)
         osHighReadyTaskPrio = osdNoActivePrio;
         osHighReadyTaskIndex = osdNoActiveTask;
         return TRUE;
      }
      ELSEWITHCOVER(0x00B5)
      uiTestPattern = *ptr;
      hiPriorTask += osdPrioBitsPerWord;
   }
   COVER(0x00B6)
   hiPriorTask += osGetHighPrioBit(uiTestPattern);
#else
   COVER(0x00B7)
   if (osQReadyPrios == 0)
   {  /* prepare for IDLE-Loop */
      COVER(0x00B8)
      osHighReadyTaskPrio = osdNoActivePrio;
      osHighReadyTaskIndex = osdNoActiveTask;
      return TRUE;
   }
   ELSEWITHCOVER(0x00B9)
   hiPriorTask= osGetHighPrioBit(osQReadyPrios);
#endif

#if ((CC == BCC2) || (CC == ECC2))
   COVER(0x00BA)
   osHighReadyTaskIndex  = oskQActivationQueues[hiPriorTask][osQReadyTaskHead[hiPriorTask]];
#else
   COVER(0x00BB)
   osHighReadyTaskIndex  = osQReadyTask[hiPriorTask];
#endif

   osHighReadyTaskPrio = hiPriorTask;

   /* KB begin osekHWosSchedulePrioReaction */
      COVER(0x00BC)
      if (osHighReadyTaskIndex != osActiveTaskIndex)
      {
         COVER(0x00BD)
         return TRUE;
      }
      else
      {
         COVER(0x00BE)
         return FALSE;
      }
   /* KB end osekHWosSchedulePrioReaction */
} /* END OF osSchedulePrio */


/*-+--------------------------------------------------------------------------
 / osInitTCBs
 / Funktion:
 /   - inits all tasks as SUSPENDED
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if (osdNumberOfAllTasks>0)
static osqFunc1 void osqFunc2 osInitTCBs(void);

static osqFunc1 void osqFunc2 osInitTCBs(void)
{
   /* CovComment 4:  Function: osInitTCBs */
   osPrioType np;

#if ((STATUS_LEVEL==EXTENDED_STATUS) ||(CC==BCC2) || (CC==ECC2) || osdORTIDebug || osdEnableContextOverlay)
   osTaskIndexType nt;
#endif

   COVER(0x00BF)
#if ((STATUS_LEVEL==EXTENDED_STATUS) ||(CC==BCC2) || (CC==ECC2) || osdORTIDebug)
   COVER(0x00C0)
   for (nt=0; nt<osrNumberOfAllTasks; nt++)
   {
      COVER(0x00C1)
      #if (CC==BCC2) || (CC==ECC2)
         COVER(0x00C2)
         osTcbActivationCount[nt]= 0;
      #endif

      #if osdORTIDebug
      COVER(0x00C3)
      osTcbActualPrio[nt] = oskTcbHomePrio[nt];
      osTcbORTIServiceId[nt] = osdORTINoService;
      #endif
   }
#endif

#if osdEnableContextOverlay
   COVER(0x00C4)
   for (nt= 0; nt < osrNumberOfAllTasks; nt++)
   {
      COVER(0x00C5)
      osTcbReturnAddressState[nt]=0;
   }
#endif   

#if (osdRTSize > 1)
   COVER(0x00C6)
   for (np=0; np<osrRTSize; np++)
   {
      COVER(0x00C7)
      osQReadyPrios[np] = 0;
   }
#else
   COVER(0x00C8)
   osQReadyPrios = 0;
#endif

   /* all activation queues are empty */
   for (np= 0; np<osrNumberOfPriorities; np++)
   {
   #if (CC==BCC2) || (CC==ECC2)
      COVER(0x00C9)
      osQReadyTaskHead[np]= osQReadyTaskTail[np]= 0;
   #else
      COVER(0x00CA)
      osQReadyTask[np]= osdNoActiveTask;
   #endif
   }

   /* KB begin osekHWosInitTcbExtension */
   /* KB end osekHWosInitTcbExtension */
} /* END OF osInitTCBs */

#endif

#if osdLib

/*-+--------------------------------------------------------------------------
 / osCheckLibraryVersionAndVariant
 / Funktion:
 /   - checks if version of library and the variant coding are the same
 /     as the user selected in the OIL configurator
 /   - if the variant differs, the system is shut down
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osCheckLibraryVersionAndVariant(void)
{
   /* CovComment 4:  Function: osCheckLibraryVersionAndVariant */
   osqPROM1 osqPROM2 uint8 osqPROM3 * ptr1;
   osqPROM1 osqPROM2 uint8 osqPROM3 * ptr2;
   uint8 n;

   COVER(0x00CB)
   /* defined in tcb.c, generated */
   ptr1= (osqPROM1 osqPROM2 uint8 osqPROM3 *) &oskLibVersionVariant; 
   /* defined in osek.c, always visible */
   ptr2= (osqPROM1 osqPROM2 uint8 osqPROM3 * ) &oskVersionVariant;    
   
   for (n= 0; n < sizeof(osVersionVariantCodingType); n++)
   {
      COVER(0x00CC)
      if (*ptr1++ != *ptr2++)
      {
         COVER(0x00CD)
         osAbortSystem(osdErrCLWrongLibrary);
      }
      ELSEWITHCOVER(0x00CE)
   }
} /* END OF osCheckLibraryVersionAndVariant */

#endif

/* KB begin osekHWosStartOSasmSrc */
/* KB end osekHWosStartOSasmSrc */

#if osdStackCheck
/* KB begin osekHwosFillTaskStacksPrototype */
osqFunc1 void osqFunc2 osFillTaskStacks(void); /* PRQA S 3210 */   /* PRQA S 3408 */ /* function might be called from assembler code only */
/* KB end osekHwosFillTaskStacksPrototype */
#endif

/*-+--------------------------------------------------------------------------
 / StartOS 
 / =======
 / Syntax void StartOS ( AppModeType <Mode> )
 / Parameter (In):
 / Mode application mode
 / Parameter (Out): none
 / Description: The user can call this system service to start the operating
 / system in a specific mode, see chapter 5, Application modes.
 / Particularities: Only allowed outside of the operating system, therefore
 / implementation specific restrictions may apply. See also
 / chapter 11.3, System start-up, especially with respect to
 / systems where OSEK and OSEKtime coexist. This call does
 / not need to return.
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osStartOSc(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osStartOSc */
#if (osdNumberOfAllTasks > 0)
   osTaskIndexType nt;
#endif
   
#if (STATUS_LEVEL == EXTENDED_STATUS) || osdORTIDebug
#if (osdNumberOfAllResources > 0)
   osResourceIndexType nr;
#endif
#endif

#if defined osdEnableCoverage
   uint16 i;
#endif

#if defined osdEnableCoverage
   /* during initialization of the coverage array no coverage is possible */
   /* initialize the coverage array */
   for(i=0; i<osdCoverageArraySize; i++)
   {
      osCoverage[i] = 0;
   }
#endif

   COVER(0x00CF)
   /* in case of active trace first trace macro is possible after trace
      and other variables are initialised  */
#ifndef osdUseTrace
   COVER(0x00D0)
   OS_ST_ENTRY()
#endif

/* KB begin dummyVariablesInit */
/* KB end dummyVariablesInit */

#if osdLib
   COVER(0x00D1)
   osCheckLibraryVersionAndVariant();
#endif

#if (osdNumberOfAllTasks>0)
   COVER(0x00D2)
   osInitTCBs();
#else
   COVER(0x00D3)
   osQReadyPrios = 0;
#endif

#if osdStackCheck
   COVER(0x00D4)
#if (osdNumberOfAllTasks > 0)
   COVER(0x00D5)
   /* KB begin osekHWosFillTaskStacks */
   /* KB end osekHWosFillTaskStacks */
#endif
   osLastActiveTaskIndex=0;    
#endif

   osActiveTaskPrio = osHighReadyTaskPrio = osdNoActivePrio; /* mark as invalid for a nested interrupt */
   osActiveTaskIndex = osHighReadyTaskIndex = osdNoActiveTask;

#if osdErrorHook
   COVER(0x00D6)
   osErrorFlag=0;
   /* KB begin osekInitGlobalError */
   #if (osdORTIDebug || defined(osdGHSDebugSupport))
   osLastErrorCode=0;
   #ifndef osdOSEK2_2
   osLastError=0;
   #endif
   #endif
   /* KB end osekInitGlobalError */
#endif

   osIntNestingDepth = 0;
   osIntSaveDisableCounter = 0;
   osStartDispatcher = 0;

#if (osdFullPreempt || osdMixedPreempt)
   osLockDispatcher = 1; /* lock dispatcher during StartOS */
   COVER(0x00D7)
#endif

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x00D8)
#if osdEnableAssertions
   COVER(0x00D9)
   osIntAPIStatus = 0;
   /* check memory access */
   osSysErrAssert(0==osStartDispatcher, osdErrSTMemoryError)

#if (osdFullPreempt || osdMixedPreempt)
   osSysErrAssert(osLockDispatcher==1, osdErrSTMemoryError)
   COVER(0x00DA)
#endif

   osSysErrAssert(osActiveTaskIndex==osdNoActiveTask, osdErrSTMemoryError)
#endif
   osResGlobalOccupationCounter =0;
   COVER(0x00DB)
#endif

#if (osdNumberOfAllResources > 0)
   COVER(0x00DC)
#if (STATUS_LEVEL == EXTENDED_STATUS) || osdORTIDebug
   COVER(0x00DD)
   for (nr=0; nr<osrNumberOfAllResources; nr++)
   {
      COVER(0x00DE)
#endif
#if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x00DF)
      osResOccupationCounter[nr] = 0;
#endif
#if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug
      COVER(0x00E0)
      osResActualTask[nr] = osdNoActiveTask;
#endif
#if (STATUS_LEVEL == EXTENDED_STATUS) || osdORTIDebug
   }
#endif

#endif

#if (osdNumberOfAllTasks > 0)
   COVER(0x00E1)
   /* activate all autostart tasks */
   for (nt= 0; nt < osrNumberOfAllTasks; nt++)
   {
      COVER(0x00E2)
      osTcbTaskState[nt] = SUSPENDED;

#if osdEnableProcedures
      COVER(0x00E3)
#if osdEnableAssertions
      COVER(0x00E4)
      osIsInProcedure[nt]=FALSE;
#endif
#endif

      #if STATUS_LEVEL == EXTENDED_STATUS
      COVER(0x00E5)
      #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
      COVER(0x00E6)
      osTcbResourceCounter[nt]=0;
      #endif
      #endif

      #if osdNumberOfAppModes > 1
      COVER(0x00E7)
      if ((oskTcbAutoStart[nt] & osActiveAppMode) != 0)
      #else
      COVER(0x00E8)
      if (oskTcbAutoStart[nt] != 0)
      #endif
      {
         COVER(0x00E9)
         (void) osSysActivateTask(nt); 
      }
      ELSEWITHCOVER(0x00EA)
   }
#endif

   /* initialise trace if in use */
#ifdef osdUseTrace
   COVER(0x00EB)
   osInitTrace();
   /* in case of active trace first trace macro is possible after trace
      and other variables are initialised  */
   OS_ST_ENTRY()
#endif

   /* KB begin osekHWosDisableLevel */
   osDisableLevel();    /* disable interrupts up to system level */
   /* KB end osekHWosDisableLevel */
   OS_ST_START_CRITICAL()

   /* KB begin osekHWStartupBeforeInitAlarms */
   /* KB end osekHWStartupBeforeInitAlarms */

#if (osdNumberOfAlarms > 0)
   COVER(0x00EC)
   osInitAlarms();
#endif

   /* KB begin osekHWStartupSpecial */
   #if (osdNumberOfInterruptResources > 0)
      osInitialIntLevel = 0;  
   #endif				  
   osIntSaveDisableCounterGlobal = 0;
   
   osInitISRprios(); /* set special ISR priorities if selected */
   /* KB end osekHWStartupSpecial */

#if osdStartupHook
   COVER(0x00ED)
   /* call hook-routine for system startup (initialization of drivers etc.) */
   OS_STHOOK_ENTRY()
   osInterStartupHook();
   OS_STHOOK_EXIT()
#endif

#if (osdFullPreempt || osdMixedPreempt)
   /* free dispatcher (after osSysActivateTask for AutoStart-Tasks and after StartupHook */
   osLockDispatcher = 0;
   COVER(0x00EE)
#endif

   OS_ST_END_CRITICAL()
   OS_ST_EXIT()
   /* KB begin osekHWosEnableLevelInStartOS */
      osEnableLevel();
   /* KB end osekHWosEnableLevelInStartOS */

   OS_START_DISPATCH()
   /* KB begin osekHWosDispatcher */
   osDispatch();
   /* KB end osekHWosDispatcher */

   /* KB begin osekHWAfterOsDispatchInST */
   #if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
      osDisableGlobal();
   #ifndef osdSuppressFilenames
      osActiveTaskModule = osd_FILE_Cast __FILE__;
      osActiveTaskLineNumber = __LINE__;
   #endif  /* osdSuppressFilenames */
      osAssertFailed_(osdErrSTNoImmediateTaskSwitch);
   #else /* if STATUS_LEVEL ...*/
      for(;;)
      {
         ;
      } 
   #endif /* STATUS_LEVEL ... */
   /* KB end osekHWAfterOsDispatchInST */
} /* END OF osStartOSc */


/*-+--------------------------------------------------------------------------
 / ShutdownOS
 / ==========
 /
 / Syntax void ShutdownOS ( StatusType <Error> )
 / Parameter (In):
 / Error error occurred
 / Parameter (Out): none
 / Description: The user can call this system service to abort the overall
 / system (e.g. emergency off). The operating system also calls
 / this function internally, if it has reached an undefined internal
 / state and is no longer ready to run.
 / If a ShutdownHook is configured the hook routine
 / ShutdownHook is always called (with <Error> as argument)
 / before shutting down the operating system.
 / If ShutdownHook returns, further behaviour of ShutdownOS is
 / implementation specific.
 / In case of a system where OSEK OS and OSEKtime OS
 / coexist, ShutdownHook must return.
 / <Error> must be a valid error code supported by OSEK OS. In
 / case of a system where OSEK OS and OSEKtime OS coexist,
 / <Error> might also be a value accepted by OSEKtime OS. In
 / this case, if enabled by an OSEKtime configuration parameter,
 / OSEKtime OS will be shut down after OSEK OS shutdown.
 / Particularities: After this service the operating system is shut down.
 / Allowed at task level, ISR level, in ErrorHook and StartupHook,
 / and also called internally by the operating system.
 / If the operating system calls ShutdownOS it never uses E_OK
 / as the passed parameter value.
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

#if osdShutdownHook
osqFunc1 void osqFunc2 osShutdownOS(StatusType Error)  /* PRQA S 1503 */ /* API functions might be never called */
#else
osqFunc1 void osqFunc2 osShutdownOS(void)  /* PRQA S 1503 */ /* API functions might be never called */
#endif
{
   /* CovComment 4:  Function: osShutdownOS */
   /* KB begin osekReadUnusedVarsAndConstsAux */
      uint8 ucTmp;
   /* KB end osekReadUnusedVarsAndConstsAux */

   OS_SD_ENTRY()
   /* KB begin osekHWosDisableGlobal */
   osDisableGlobal();        /* disable all interrupts, no nesting */
   /* KB end osekHWosDisableGlobal */

   /* call shutdown hook routine */
#if osdShutdownHook
   COVER(0x00EF)
   OS_SDHOOK_ENTRY()
   osInterShutdownHook(Error);
   OS_SDHOOK_EXIT()
#endif
   OS_SD_EXIT()
   
   COVER(0x00F0)
   /* KB begin osekReadUnusedVarsAndConsts */
   /* force the compiler to link these 'unused' variables, too */
   ucTmp  = (uint8)oskVersionVariant.ucMagicNumber1;
   #if osdErrorHook
   ucTmp |= (uint8)osLastError;
   #endif
   #if osdORTIDebug
   ucTmp |= (uint8)osTcbActualPrio[0];
    #if osdErrorHook
   ucTmp |= (uint8)osLastErrorCode;
    #endif
   #endif
   osdDummyRead(ucTmp)  /* PRQA S 3199 */ /* suppress QAC warning about never used variable */
   /* KB end osekReadUnusedVarsAndConsts */
   /* KB begin osekHWosShutdownReactionSrc */
   for(;;)
   {
      ;
   }
   /* KB end osekHWosShutdownReactionSrc */
} /* END OF osShutdownOS */

/* KB begin BNCdispatcherSrc */
/* KB end BNCdispatcherSrc */

#if osdStackCheck

/* KB begin osekHWosFillSystemStackSrc */
/*-+--------------------------------------------------------------------------
 / osFillSystemStack
 / Funktion:
 /   - fill system stack with empty pattern
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

void osFillSystemStack(void) /* PRQA S 1503 */  /* PRQA S 3408 */ /* function might be called from assembler code only */
{
   uint32 i;
    
   for (i=0; i<(osdSystemStackSize / sizeof(osStackDataType)); i++)
   {
      osSystemStack[i] = 0xAAAAAAAAUL;
   }
}
/* KB end osekHWosFillSystemStackSrc */


#if (osdNumberOfAllTasks > 0)
/* KB begin osekHwosFillTaskStacksSrc */
/* KB end osekHwosFillTaskStacksSrc */
#endif

/*-+--------------------------------------------------------------------------
 / osStackOverflow
 / Funktion:
 /   - called on task stack overflow: abort system
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osStackOverflow(void)   /* PRQA S 1503 */ /* function might be called from assembler code only */
{
   /* CovComment 4:  Function: osStackOverflow */
   COVER(0x00F1)
   osAbortSystem(osdErrSOStackOverflow); /*we do not return here*/
}


/* KB begin osekHWosSystemStackOverflowSrc */
/*-+--------------------------------------------------------------------------
 / osSystemStackOverflow
 / Funktion:
 /   - called on system stack overflow: abort system
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

void osSystemStackOverflow(void) /* PRQA S 1503 */  /* PRQA S 3408 */ /* function might be called from assembler code only */
{
   osAbortSystem(osdErrYOStackOverflow);
}
/* KB end osekHWosSystemStackOverflowSrc */


#if (osdNumberOfAllTasks > 0)
/* KB begin osekOsGetStackUsage */
/*-+--------------------------------------------------------------------------
 / osGetStackUsage
 / Funktion:
 /   - calculate maximum stack usage of task <taskId>
 / Parameter:
 / Returnvalue: calculated stack usage (in byte)
 /--------------------------------------------------------------------------*/

osqFunc1 osStackUsageType osqFunc2 osGetStackUsage(TaskType taskId)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetStackUsage */
   osqPRAM1 osqPRAM2 uint8 osqPRAM3 *stack;

   COVER(0x00F2)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x00F3)
   osSysErrAssert(taskId < osrNumberOfAllTasks, osdErrSUWrongTaskID)
#endif
   stack = (osqPRAM1 osqPRAM2 uint8 osqPRAM3 *)oskTcbStackBottom[taskId];   /* PRQA S 3689 */ /* index is validated by assertion */
   while ((stack < (osqPRAM1 osqPRAM2 uint8 osqPRAM3 *)(oskTcbStackTop[taskId])) && (*stack == 0xAA))    /* PRQA S 3689 */ /* index is validated by assertion */
   {
      COVER(0x00F4)
      stack++;
   }
   return (osStackUsageType)((osqPRAM1 osqPRAM2 uint8 osqPRAM3 *)(oskTcbStackTop[taskId]) - stack); /* PRQA S 3689 */ /* index is validated by assertion */
} /* end of osGetStackUsage */
/* KB end osekOsGetStackUsage */
#endif

/* KB begin osekStackUsageSrc */
/*-+--------------------------------------------------------------------------
 / osGetSystemStackUsage
 / Funktion:
 /   - calculate maximum system stack usage
 / Parameter:
 / Returnvalue: calculated stack usage (in byte)
 /--------------------------------------------------------------------------*/

osStackUsageType osGetSystemStackUsage(void)   /* PRQA S 1503 */ /* API functions might be never called */
{
   uint8 *stack;

   stack = (uint8*)osSystemStack;
   while ((stack < (uint8*)(((uint32)osSystemStack) + ((uint32)osdSystemStackSize )))
	   && (*stack == 0xAA))
   {
      stack++;
   }
   return (osStackUsageType)((uint8*)(((uint32)osSystemStack) + ((uint32)osdSystemStackSize )) - stack);
} /* end of osGetSystemStackUsage */

/* KB end osekStackUsageSrc */

#endif

/* KB begin osekHWosCheckInterruptsEnabledSrc */
/* KB end osekHWosCheckInterruptsEnabledSrc */

/* KB begin osekHWosCheckInterruptsDisabledSrc */
/* KB end osekHWosCheckInterruptsDisabledSrc */

/* KB begin osekHWosDisableLevelSrc */
/* KB end osekHWosDisableLevelSrc */

/* KB begin osekHWosEnableLevelSrc */

void osDefVectWarning(void)	 /* PRQA S 3408 */ /* PRQA S 1503 */ /* function might be called from assembler code only */		   
{
     osAPIError(E_OS_SYS_WARNING, osdErrHWDefVect);
}
/* KB end osekHWosEnableLevelSrc */

/* KB begin osekHWosSetAlarmLevelSrc */
/* KB end osekHWosSetAlarmLevelSrc */

/* KB begin osekHWosSaveDisableLevelSrc */
/* KB end osekHWosSaveDisableLevelSrc */

/* KB begin osekHWosRestoreEnableLevelSrc */
/* KB end osekHWosRestoreEnableLevelSrc */

/* KB begin osekHWosDisableGlobalSrc */
/* KB end osekHWosDisableGlobalSrc */

/* KB begin osekHWosEnableGlobalSrc */
/* KB end osekHWosEnableGlobalSrc */

/* KB begin osekHWosSaveDisableGlobalSrc */
#if ((osdThumbMode == 0) && (osdTasksInUserMode==0))
void osSaveDisableGlobal(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* KB begin osekHWosSaveDisableGlobalBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSavedGlobalInt = (uint8)_disable_interrupts();
   #else
   osSaveDisableGlobal();   /* call asm function */
   #endif
   /* KB end osekHWosSaveDisableGlobalBody */
}
#endif
/* KB end osekHWosSaveDisableGlobalSrc */

/* KB begin osekHWosRestoreEnableGlobalSrc */
#if ((osdThumbMode == 0) && (osdTasksInUserMode==0))
void osRestoreEnableGlobal(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* KB begin osekHWosRestoreEnableGlobalBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   _set_CPSR((_get_CPSR() & ~0x000000c0UL) | ((uint32)osSavedGlobalInt & 0xc0UL));
   #else
   osRestoreEnableGlobal();   /* call asm function */
   #endif
   /* KB end osekHWosRestoreEnableGlobalBody */
}
#endif
/* KB end osekHWosRestoreEnableGlobalSrc */

/* KB begin osekHWosSaveDisableLevelNestedSrc */
#if ((osdThumbMode == 0) && (osdTasksInUserMode==0))
void osSaveDisableLevelNested(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* KB begin osekHWosSaveDisableLevelNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounter < 255, osdErrSDWrongCounter)
   if (osIntSaveDisableCounter == 0) 
   {
      osSavedIntLevelNested = (uint8)_disable_IRQ();
   }
   osIntSaveDisableCounter++;
   #else
   osSaveDisableLevelNested();   /* call asm function */
   #endif
   /* KB end osekHWosSaveDisableLevelNestedBody */
}
#endif
/* KB end osekHWosSaveDisableLevelNestedSrc */

/* KB begin osekHWosRestoreEnableLevelNestedSrc */
#if ((osdThumbMode == 0) && (osdTasksInUserMode==0))
void osRestoreEnableLevelNested(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* KB begin osekHWosRestoreEnableLevelNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounter!=0, osdErrREWrongCounter)
   osIntSaveDisableCounter--;
   if (osIntSaveDisableCounter == 0)
   {  
      if ((osSavedIntLevelNested & 0x80) == 0)
      {
         _enable_IRQ();
      }
   }
   #else
   osRestoreEnableLevelNested();   /* call asm function */
   #endif
   /* KB end osekHWosRestoreEnableLevelNestedBody */
}                                               
#endif
/* KB end osekHWosRestoreEnableLevelNestedSrc */

/* KB begin osekHWosSaveDisableGlobalNestedSrc */
#if ((osdThumbMode == 0) && (osdTasksInUserMode==0))
void osSaveDisableGlobalNested(void)   /* PRQA S 1503 */ /* API functions might be never called */ 
{
   /* KB begin osekHWosSaveDisableGlobalNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounterGlobal < 255, osdErrSGWrongCounter)
   if (osIntSaveDisableCounterGlobal == 0) 
   {
      osSavedIntGlobalNested = (uint8)_disable_interrupts();
   }
   osIntSaveDisableCounterGlobal++;
   #else
   osSaveDisableGlobalNested();   /* call asm function */
   #endif
   /* KB end osekHWosSaveDisableGlobalNestedBody */
}
#endif
/* KB end osekHWosSaveDisableGlobalNestedSrc */

/* KB begin osekHWosRestoreEnableGlobalNestedSrc */
#if ((osdThumbMode == 0) && (osdTasksInUserMode==0))
void osRestoreEnableGlobalNested(void)   /* PRQA S 1503 */ /* API functions might be never called */
{
   /* KB begin osekHWosRestoreEnableGlobalNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounterGlobal!=0, osdErrRGWrongCounter)
   osIntSaveDisableCounterGlobal--;
   if (osIntSaveDisableCounterGlobal == 0)
   {  
      _set_CPSR((_get_CPSR() & ~0x000000c0UL) | ((uint32)osSavedIntGlobalNested & 0xc0UL));
   }
   #else
   osRestoreEnableGlobalNested();   /* call asm function */
   #endif
   /* KB end osekHWosRestoreEnableGlobalNestedBody */
}                                               
#endif
/* KB end osekHWosRestoreEnableGlobalNestedSrc */

/* KB begin osekHWosDispatcherSrc */
#ifdef osdOSEK2_2
void osOccupyInternalResource(void)   /* PRQA S 3408 */ /* PRQA S 1503 */ /* function might be called from assembler code only */
{
   /* KB begin osOccupyInternalResource */
   #if (osdNumberOfInternalResources > 0)
   COVER(0x00F5)
   if (oskResInternalCeilingPrioMask[osActiveTaskIndex] != 0)
   {
      osPrioType IntResCeilingPrio= oskResInternalCeilingPrio[osActiveTaskIndex];
      COVER(0x00F6)
   
   #if (CC==BCC2) || (CC==ECC2)
      COVER(0x00F7)
      oskQActivationQueues[IntResCeilingPrio][0]= osActiveTaskIndex;
   #else
      COVER(0x00F8)
      osQReadyTask[IntResCeilingPrio]= osActiveTaskIndex;
   #endif
   
   #if osdRTSize == 1
      COVER(0x00F9)
      osQReadyPrios |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #else
      COVER(0x00FA)
      osQReadyPrios[oskResInternalCeilingPrioOffset[osActiveTaskIndex]]
                    |= oskResInternalCeilingPrioMask[osActiveTaskIndex];
   #endif
   
      /* correct osHighReadyTaskPrio if required */
      if(IntResCeilingPrio<osHighReadyTaskPrio)
      {
         COVER(0x00FB)
         osHighReadyTaskPrio = IntResCeilingPrio;
         osActiveTaskPrio  =   IntResCeilingPrio;
      }
      else
      {
         COVER(0x00FC)
         osActiveTaskPrio  = osHighReadyTaskPrio;
      }
   }
   else
   {
      COVER(0x00FD)
   #endif
      COVER(0x00FE)
      osActiveTaskPrio  = osHighReadyTaskPrio;
   #if (osdNumberOfInternalResources > 0)
   }
   
   #if osdORTIDebug
   COVER(0x00FF)
   osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
   #endif
   
   #endif
   
   
   /* KB end osOccupyInternalResource */
}
#endif
/* KB end osekHWosDispatcherSrc */

/* KB begin osekHWAdditionalAPIFunctions */
/* KB end osekHWAdditionalAPIFunctions */

/* KB begin osekHWdelayTilEnableTookEffectSrc */
/* KB end osekHWdelayTilEnableTookEffectSrc */

#ifndef osdEnableAllInterruptsNotUsed
osqFunc1 void osqFunc2 EnableAllInterrupts(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   OS_EA_ENTRY()
   osSysErrAssert(osIntAPIStatus==osdIntAPIDisableAll,osdErrEAIntAPIWrongSequence)
   osSetIntAPIStatus(0);
   /* KB begin osekHWosRestoreEnableGlobalBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   _set_CPSR((_get_CPSR() & ~0x000000c0UL) | ((uint32)osSavedGlobalInt & 0xc0UL));
   #else
   osRestoreEnableGlobal();   /* call asm function */
   #endif
   /* KB end osekHWosRestoreEnableGlobalBody */
   OS_EA_EXIT()
}
#endif


#ifndef osdDisableAllInterruptsNotUsed
osqFunc1 void osqFunc2 DisableAllInterrupts(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   OS_DA_ENTRY()
   osSysErrAssert(0==osIntAPIStatus,osdErrDAIntAPIDisabled)
   /* KB begin osekHWosSaveDisableGlobalBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSavedGlobalInt = (uint8)_disable_interrupts();
   #else
   osSaveDisableGlobal();   /* call asm function */
   #endif
   /* KB end osekHWosSaveDisableGlobalBody */
   osSetIntAPIStatus(osdIntAPIDisableAll);
   OS_DA_EXIT()
}
#endif


#ifndef osdResumeOSInterruptsNotUsed
osqFunc1 void osqFunc2 ResumeOSInterrupts(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   OS_RI_ENTRY()
   osDecIntAPIStatus();
   /* KB begin osekHWosRestoreEnableLevelNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounter!=0, osdErrREWrongCounter)
   osIntSaveDisableCounter--;
   if (osIntSaveDisableCounter == 0)
   {  
      if ((osSavedIntLevelNested & 0x80) == 0)
      {
         _enable_IRQ();
      }
   }
   #else
   osRestoreEnableLevelNested();   /* call asm function */
   #endif
   /* KB end osekHWosRestoreEnableLevelNestedBody */
   OS_RI_EXIT()
}
#endif


#ifndef osdSuspendOSInterruptsNotUsed
osqFunc1 void osqFunc2 SuspendOSInterrupts(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   OS_SI_ENTRY()
   /* KB begin osekHWosSaveDisableLevelNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounter < 255, osdErrSDWrongCounter)
   if (osIntSaveDisableCounter == 0) 
   {
      osSavedIntLevelNested = (uint8)_disable_IRQ();
   }
   osIntSaveDisableCounter++;
   #else
   osSaveDisableLevelNested();   /* call asm function */
   #endif
   /* KB end osekHWosSaveDisableLevelNestedBody */
   osIncIntAPIStatus();
   OS_SI_EXIT()
}
#endif

#ifndef osdResumeAllInterruptsNotUsed
osqFunc1 void osqFunc2 ResumeAllInterrupts(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   OS_RA_ENTRY()
   osDecIntAPIStatus();
   /* KB begin osekHWosRestoreEnableGlobalNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounterGlobal!=0, osdErrRGWrongCounter)
   osIntSaveDisableCounterGlobal--;
   if (osIntSaveDisableCounterGlobal == 0)
   {  
      _set_CPSR((_get_CPSR() & ~0x000000c0UL) | ((uint32)osSavedIntGlobalNested & 0xc0UL));
   }
   #else
   osRestoreEnableGlobalNested();   /* call asm function */
   #endif
   /* KB end osekHWosRestoreEnableGlobalNestedBody */
   OS_RA_EXIT()
}
#endif


#ifndef osdSuspendAllInterruptsNotUsed
osqFunc1 void osqFunc2 SuspendAllInterrupts(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   OS_PA_ENTRY()
   /* KB begin osekHWosSaveDisableGlobalNestedBody */
   #if ((osdThumbMode == 0) && (osdCompiler==osdCompilerTIARM7) && (osdTasksInUserMode==0))
   osSysErrAssert(osIntSaveDisableCounterGlobal < 255, osdErrSGWrongCounter)
   if (osIntSaveDisableCounterGlobal == 0) 
   {
      osSavedIntGlobalNested = (uint8)_disable_interrupts();
   }
   osIntSaveDisableCounterGlobal++;
   #else
   osSaveDisableGlobalNested();   /* call asm function */
   #endif
   /* KB end osekHWosSaveDisableGlobalNestedBody */
   osIncIntAPIStatus();
   OS_PA_EXIT()
}
#endif



/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */

/* END OF MODULE osek.c */

