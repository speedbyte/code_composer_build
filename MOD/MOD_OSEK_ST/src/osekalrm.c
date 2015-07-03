/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekalrm.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.37
|  Description: osek API-functions for handling alarms and counters
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

/* CovComment 0:  file: osekalrm.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

#if (osdNumberOfAlarms > 0)

#if (osdUseHeapAlarm != 0)
   #define osdSystemCounterMask    ((TickType)(0x80000000U))
#endif

#ifndef osdGetAlarmBaseNotUsed
/*-+--------------------------------------------------------------------------
 / GetAlarmBase
 / ============
 /
 / Syntax:          StatusType GetAlarmBase ( AlarmType <AlarmID>,
 /                                            AlarmBaseRefType <Info> )
 / Parameter (In):  AlarmID Reference to alarm
 / Parameter (Out): Info Reference to structure with constants of the alarm 
 /                  base.
 / Description:     The system service GetAlarmBase reads the alarm base
 /                  characteristics. The return value <Info> is a structure 
 /                  in which the information of data type AlarmBaseType is 
 /                  stored.
 / Particularities: Allowed on task level, ISR, and in several hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook,
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osGetAlarmBase(AlarmType AlarmId, AlarmBaseRefType Info)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetAlarmBase */
   OS_GB_ENTRY()
   COVER(0x022A)
#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x022B)
   if (AlarmId >= osrNumberOfAlarms)
   {
      #if osdErrorHook
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveGetAlarmBase_AlarmID(AlarmId)
       osSaveGetAlarmBase_Info(Info)
       COVER(0x022C)
      #endif
      osAPIError(E_OS_ID, osdErrGBWrongAlarmID);
      #if osdErrorHook
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x022D)
      #endif
      OS_GB_EXIT()
      COVER(0x022E)
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x022F)

#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveGetAlarmBase_AlarmID(AlarmId)
       osSaveGetAlarmBase_Info(Info)
       COVER(0x0230)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrGBIntAPIDisabled);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x0231)
      #endif
      OS_GB_EXIT()
      COVER(0x0232)
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0233)
#endif

#else
   osdDummyRead(AlarmId)  /* PRQA S 3199 */ /* suppress QAC warning about never used variable */
   COVER(0x0234)
#endif

   /* copy structure */
#if osdUseCounterAPI
   Info->maxallowedvalue= oskCounterMaxAllowedValue[oskAlarmCounterRef[AlarmId]];
   Info->ticksperbase   = oskCounterTicksPerBase[oskAlarmCounterRef[AlarmId]];
   Info->mincycle       = oskCounterMinCycle[oskAlarmCounterRef[AlarmId]];
   COVER(0x0235)
#else
   COVER(0x0236)
#if osdLib
   Info->maxallowedvalue= oskMaxAllowedValue;
   Info->ticksperbase   = oskTicksPerBase;
   Info->mincycle       = oskMinCycle;
   COVER(0x0237)
#else
   Info->maxallowedvalue= OSMAXALLOWEDVALUE;
   Info->ticksperbase   = OSTICKSPERBASE;
   Info->mincycle       = OSMINCYCLE;
   COVER(0x0238)
#endif
#endif

   OS_GB_EXIT()
   COVER(0x0239)
   return E_OK;
} /* END OF osGetAlarmBase */
#endif


#ifndef osdGetAlarmNotUsed
/*-+--------------------------------------------------------------------------
 / GetAlarm
 / ========
 /
 / Syntax:          StatusType GetAlarm ( AlarmType <AlarmID>, 
 /                                        TickRefType <Tick>)
 / Parameter (In):  AlarmID Reference to an alarm
 / Parameter (Out): Tick Relative value in ticks before the alarm <AlarmID> 
 /                  expires.
 / Description:     The system service GetAlarm returns the relative value in 
 /                  ticks before the alarm <AlarmID> expires.
 / Particularities: It is up to the application to decide whether for example 
 /                  a CancelAlarm may still be useful.
 /                  If <AlarmID> is not in use, <Tick> is not defined.
 /                  Allowed on task level, ISR, and in several hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> is not used, E_OS_NOFUNC
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, ErrorHook,
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osGetAlarm(AlarmType alarmID, TickRefType diffTickTime)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetAlarm */
   OS_GA_ENTRY()
   COVER(0x023A)

#if (STATUS_LEVEL == EXTENDED_STATUS)
   COVER(0x023B)
   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveGetAlarm_AlarmID(alarmID)
       osSaveGetAlarm_Tick(diffTickTime)
       COVER(0x023C)
      #endif
      osAPIError(E_OS_ID, osdErrGAWrongAlarmID);
      #if osdErrorHook
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x023D)
      #endif
      OS_GA_EXIT()
      COVER(0x023E)
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x023F)

#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveGetAlarm_AlarmID(alarmID)
       osSaveGetAlarm_Tick(diffTickTime)
       COVER(0x0240)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrGAIntAPIDisabled);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x0241)
      #endif
      OS_GA_EXIT()
      COVER(0x0242)
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0243)
#endif
   COVER(0x0244)
#endif

   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */
   OS_GA_START_CRITICAL()
   /* if timer is inactive, return at once */
#if (osdUseHeapAlarm != 0)
   COVER(0x0245)
   if (osAlarmActivated[alarmID] == 0)
#else
   COVER(0x0246)
   if (osAlarmTime[alarmID] == 0)
#endif
   {
      osSaveGetAlarm_AlarmID(alarmID)
      osSaveGetAlarm_Tick(diffTickTime)
      osAPIError(E_OS_NOFUNC, osdErrGANotActive);
      OS_GA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      OS_GA_EXIT()
      COVER(0x0247)
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x0248)

#if (osdUseHeapAlarm != 0)
   COVER(0x0249)
   *diffTickTime = osAlarmTime[alarmID] - osGetSystemCounter();
#else
   COVER(0x024A)
   *diffTickTime = osAlarmTime[alarmID];
#endif
   OS_GA_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */

   OS_GA_EXIT()
   COVER(0x024B)
   return E_OK;
} /* END OF osGetAlarm */
#endif


#ifndef osdSetRelAlarmNotUsed
/*-+--------------------------------------------------------------------------
 / SetRelAlarm
 / ===========
 /
 / Syntax:          StatusType SetRelAlarm ( AlarmType <AlarmID>,
 /                                           TickType <increment>,
 /                                           TickType <cycle> )
 / Parameter (In):  -AlarmID Reference to the alarm element
 /                  -increment Relative value in ticks
 /                  -cycle Cycle value in case of cyclic alarm. In case of 
 /                   single alarms, cycle has to be zero.
 / Parameter (Out): none
 / Description:     The system service occupies the alarm <AlarmID> element.
 /                  After <increment> ticks have elapsed, the task assigned 
 /                  to the alarm <AlarmID> is activated or the assigned event
 /                  (only for extended tasks) is set or the alarm-callback 
 /                  routine is called.
 / Particularities: A zero value for <increment> is interpreted as 1.
 /                  If the relative value <increment> is very small, 
 /                  the alarm may expire, and the task may become ready or 
 /                  the alarm-callback may be called before the system 
 /                  service returns to the user.
 /                  If <cycle> is unequal zero, the alarm element is logged 
 /                  on again immediately after expiry with the relative 
 /                  value <cycle>.
 /                  The alarm <AlarmID> must not already be in use.
 /                  To change values of alarms already in use the alarm has 
 /                  to be cancelled first.
 /                  If the alarm is already in use, this call is
 /                  ignored and the error E_OS_STATE is returned.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> is already in use, E_OS_STATE
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Value of <increment> outside of the admissible 
 /                    limits (less than zero or bigger than 
 /                    maxallowedvalue), E_OS_VALUE
 /                  - Value of <cycle> unequal to 0 and outside of the 
 /                    admissible counter limits (less than mincycle 
 /                    or bigger than maxallowedvalue), E_OS_VALUE
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, [StartupHook,]
 /                  interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

/* KB begin osekHWosSetRelAlarmHead */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID, TickType deltaTicks, TickType cycle)  /* PRQA S 1503 */ /* API functions might be never called */
#else
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID)  /* PRQA S 1503 */ /* API functions might be never called */
#endif
{
   OS_SA_ENTRY()
   COVER(0x024C)
/* KB end osekHWosSetRelAlarmHead */
   /* CovComment 4:  Function: osSetRelAlarm */

#if (STATUS_LEVEL == EXTENDED_STATUS)
   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveSetRelAlarm_AlarmID(alarmID)
       #if (osdNumberOfVarAlarms > 0)
        osSaveSetRelAlarm_increment(deltaTicks)
        osSaveSetRelAlarm_cycle(cycle)
        COVER(0x024D)
       #endif
       COVER(0x024E)
      #endif
      osAPIError(E_OS_ID, osdErrSAWrongAlarmID); 
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x024F)
      #endif
      OS_SA_EXIT()
      COVER(0x0250)
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x0251)
   COVER(0x0252)
#endif

#if (osdNumberOfVarAlarms > 0)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x0253)
   if (alarmID < osrNumberOfVarAlarms)
   {
#endif
      /* set delta ticks at least to 1 */
      if (deltaTicks == 0)
      {
         deltaTicks = 1;
         COVER(0x0254)
      }
      ELSEWITHCOVER(0x0255)

#if (STATUS_LEVEL == EXTENDED_STATUS)
      /* check timer limits */
      #if osdUseCounterAPI
      if (cycle != 0)
      {
         if ((cycle > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]])
             || (cycle < oskCounterMinCycle[oskAlarmCounterRef[alarmID]]))
         {
            #if osdErrorHook 
             /* KB begin osekHWosSaveDisableLevelNested */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveSetRelAlarm_AlarmID(alarmID)
             #if (osdNumberOfVarAlarms > 0)
              osSaveSetRelAlarm_increment(deltaTicks)
              osSaveSetRelAlarm_cycle(cycle)
              COVER(0x0256)
             #endif
             COVER(0x0257)
            #endif
            osAPIError(E_OS_VALUE, osdErrSAWrongCycle);  
            #if osdErrorHook 
             /* KB begin osekHWosRestoreEnableLevelNested */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
             COVER(0x0258)
            #endif
            OS_SA_EXIT()
            COVER(0x0259)
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x025A)
         COVER(0x025B)
      }
      if (deltaTicks > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]]) 
      {
         #if osdErrorHook 
          /* KB begin osekHWosSaveDisableLevelNested */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */
          osSaveSetRelAlarm_AlarmID(alarmID)
          #if (osdNumberOfVarAlarms > 0)
           osSaveSetRelAlarm_increment(deltaTicks)
           osSaveSetRelAlarm_cycle(cycle)
           COVER(0x025C)
          #endif
          COVER(0x025D)
         #endif
         osAPIError(E_OS_VALUE, osdErrSAWrongDelta);
         #if osdErrorHook 
          /* KB begin osekHWosRestoreEnableLevelNested */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          COVER(0x025E)
         #endif
         OS_SA_EXIT()
         COVER(0x025F)
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x0260)
      COVER(0x0261)
      #else /* osdUseCounterAPI */
      #if ((OSMAXALLOWEDVALUE < TICKTYPE_MAX) || (OSMINCYCLE > 1))
      if (cycle != 0)
      {
         if (
           #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) 
              cycle > oskAlarmDefinitions.maxallowedvalue
           #else
              0
           #endif
           #if (OSMINCYCLE > 1)
              || (cycle < oskAlarmDefinitions.mincycle)
           #endif
         )
         {
            #if osdErrorHook 
             /* KB begin osekHWosSaveDisableLevelNested */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveSetRelAlarm_AlarmID(alarmID)
             #if (osdNumberOfVarAlarms > 0)
              osSaveSetRelAlarm_increment(deltaTicks)
              osSaveSetRelAlarm_cycle(cycle)
              COVER(0x0262)
             #endif
             COVER(0x0263)
            #endif
            osAPIError(E_OS_VALUE, osdErrSAWrongCycle);  
            #if osdErrorHook 
             /* KB begin osekHWosRestoreEnableLevelNested */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
             COVER(0x0264)
            #endif
            OS_SA_EXIT()
            COVER(0x0265)
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x0266)
         COVER(0x0267)
      }
      ELSEWITHCOVER(0x0268)
      COVER(0x0269)
      #endif /* #if ((OSMAXALLOWEDVALUE < TICKTYPE_MAX) || (OSMINCYCLE > 1)) */
      #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) 
      if (deltaTicks > oskAlarmDefinitions.maxallowedvalue)
      {
         #if osdErrorHook 
          /* KB begin osekHWosSaveDisableLevelNested */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */
          osSaveSetRelAlarm_AlarmID(alarmID)
          #if (osdNumberOfVarAlarms > 0)
           osSaveSetRelAlarm_increment(deltaTicks)
           osSaveSetRelAlarm_cycle(cycle)
           COVER(0x026A)
          #endif
          COVER(0x026B)
         #endif
         osAPIError(E_OS_VALUE, osdErrSAWrongDelta);
         #if osdErrorHook 
          /* KB begin osekHWosRestoreEnableLevelNested */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          COVER(0x026C)
         #endif
         OS_SA_EXIT()
         COVER(0x026D)
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x026E)
      COVER(0x026F)
      #endif /* #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) */
      COVER(0x0270)
   #endif /* osdUseCounterAPI */
#endif /* (STATUS_LEVEL == EXTENDED_STATUS) */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
      COVER(0x0271)
   }
   ELSEWITHCOVER(0x0272)
#endif
#endif /* (osdNumberOfVarAlarms > 0) */

#if (STATUS_LEVEL == EXTENDED_STATUS)
#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveSetRelAlarm_AlarmID(alarmID)
       #if (osdNumberOfVarAlarms > 0)
        osSaveSetRelAlarm_increment(deltaTicks)
        osSaveSetRelAlarm_cycle(cycle)
        COVER(0x0273)
       #endif
       COVER(0x0274)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrSAIntAPIDisabled);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x0275)
      #endif
      OS_SA_EXIT()
      COVER(0x0276)
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0277)
   COVER(0x0278)
#endif /* osdEnableAssertions */
   COVER(0x0279)
#endif /* (STATUS_LEVEL == EXTENDED_STATUS) */

   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_SA_START_CRITICAL()

   /* timer running? */
#if (osdUseHeapAlarm != 0)
   COVER(0x027A)
   if (osAlarmActivated[alarmID] != 0)
#else
   COVER(0x027B)
   if (osAlarmTime[alarmID] != 0)
#endif
   {
      osSaveSetRelAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
       osSaveSetRelAlarm_increment(deltaTicks)
       osSaveSetRelAlarm_cycle(cycle)
       COVER(0x027C)
      #endif
      osAPIError(E_OS_STATE, osdErrSAAlreadyActive);
      OS_SA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
      OS_SA_EXIT()
      COVER(0x027D)
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x027E)

   /*start alarm*/
#if (osdNumberOfVarAlarms > 0)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x027F)
#endif
      /*alarm time is variable*/
      osAlarmCycleTime[alarmID] = cycle;
#if (osdUseHeapAlarm != 0)
      COVER(0x0280)
      osAlarmTime[alarmID] = deltaTicks + osGetSystemCounter();
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
#else
      COVER(0x0281)
      osAlarmTime[alarmID] = deltaTicks;
#endif
#endif /* (osdNumberOfVarAlarms > 0) */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   COVER(0x0282)
#if (osdNumberOfVarAlarms > 0)
   COVER(0x0283)
   }
   else
#endif
   {
      COVER(0x0284)
      /* start timer */
#if (osdUseHeapAlarm != 0)
      COVER(0x0285)
      osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] + osGetSystemCounter();
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
#else
      COVER(0x0286)
      osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID];
#endif
   }
#endif

   OS_SA_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */
   OS_SA_EXIT()
   COVER(0x0287)
   return E_OK;
} /* END OF osSetRelAlarm */
#endif

#ifndef osdSetAbsAlarmNotUsed

#define osdSetAlarmTimeCorrected( dest, src )                                                            \
{                                                                                                        \
   TickType actualTime = osGetSystemCounter();                                                           \
   if ((src) > (actualTime & ~(osdSystemCounterMask)))                                                   \
   {                                                                                                     \
      (dest) = (src) | ( actualTime & (TickType)osdSystemCounterMask);                                   \
      COVER(0x0288)                                                                                            \
   }                                                                                                     \
   else                                                                                                  \
   {                                                                                                     \
      (dest) = (src) | ((actualTime-(TickType)osdSystemCounterMask) & (TickType)osdSystemCounterMask);   \
      COVER(0x0289)                                                                                            \
   }                                                                                                     \
}

/*-+--------------------------------------------------------------------------
 / SetAbsAlarm
 / ===========
 /
 / Syntax:          StatusType SetAbsAlarm ( AlarmType <AlarmID>, 
 /                                           TickType <start>,
 /                                           TickType <cycle> )
 / Parameter (In):  -AlarmID Reference to the alarm element
 /                  -start Absolute value in ticks
 /                  -cycle Cycle value in case of cyclic alarm. For
 /                   single shot alarms, cycle has to be zero.
 / Parameter (Out): none
 / Description: The system service occupies the alarm <AlarmID> element.
 / When <start> ticks are reached, the task assigned to the alarm
 / <AlarmID> is activated or the assigned event (only for extended
 / tasks) is set or the alarm-callback routine is called.
 / Particularities: If the absolute value <start> is very close to the current counter
 / value, the alarm may expire, and the task may become ready or
 / the alarm-callback may be called before the system service
 / returns to the user.
 / If the absolute value <start> already was reached before the
 / system call, the alarm will only expire when the absolute value
 / <start> will be reached again, i.e. after the next overrun of the
 / counter (High Resolution Timer has a special handling to avoid this).
 / If <cycle> is unequal zero, the alarm element is logged on
 / again immediately after expiry with the relative value <cycle>.
 / The alarm <AlarmID> must not already be in use.
 / To change values of alarms already in use the alarm has to be
 / cancelled first.
 / If the alarm is already in use, this call will be ignored and the
 / error E_OS_STATE is returned.
 / Allowed on task level and in ISR, but not in hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> is already in use, E_OS_STATE
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 /                  - Value of <start> outside of the admissible counter 
 /                    limit (less than zero or bigger than maxallowedvalue),
 /                    E_OS_VALUE
 /                  - Value of <cycle> unequal to 0 and outside of the 
 /                    admissible counter limits (less than mincycle or 
 /                    bigger than maxallowedvalue), E_OS_VALUE
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

/* KB begin osekHWosSetAbsAlarmHead */
#if (osdNumberOfVarAlarms > 0)
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID, TickType start, TickType cycle)  /* PRQA S 1503 */ /* API functions might be never called */
#else
osqFunc1 StatusType osqFunc2 osSetAbsAlarm(AlarmType alarmID)  /* PRQA S 1503 */ /* API functions might be never called */
#endif
{
   OS_SL_ENTRY()
   COVER(0x028A)
/* KB end osekHWosSetAbsAlarmHead */
   /* CovComment 4:  Function: osSetAbsAlarm */

#if (STATUS_LEVEL == EXTENDED_STATUS)
   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveSetAbsAlarm_AlarmID(alarmID)
       #if (osdNumberOfVarAlarms > 0)
        osSaveSetAbsAlarm_start(start)
        osSaveSetAbsAlarm_cycle(cycle)
        COVER(0x028B)
       #endif
       COVER(0x028C)
      #endif
      osAPIError(E_OS_ID, osdErrSLWrongAlarmID);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x028D)
      #endif
      OS_SL_EXIT()
      COVER(0x028E)
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x028F)
#endif

#if (STATUS_LEVEL == EXTENDED_STATUS)
#if (osdNumberOfVarAlarms > 0)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x0290)
#endif
      /* check timer limits */
      #if osdUseCounterAPI
      if (cycle != 0)
      {
         if ((cycle > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]])
             || (cycle < oskCounterMinCycle[oskAlarmCounterRef[alarmID]]))
         {
            #if osdErrorHook 
             /* KB begin osekHWosSaveDisableLevelNested */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveSetAbsAlarm_AlarmID(alarmID)
             #if (osdNumberOfVarAlarms > 0)
              osSaveSetAbsAlarm_start(start)
              osSaveSetAbsAlarm_cycle(cycle)
              COVER(0x0291)
             #endif
             COVER(0x0292)
            #endif
            osAPIError(E_OS_VALUE, osdErrSLWrongCycle);  
            #if osdErrorHook 
             /* KB begin osekHWosRestoreEnableLevelNested */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
             COVER(0x0293)
            #endif
            OS_SA_EXIT()
            COVER(0x0294)
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x0295)
      }
      ELSEWITHCOVER(0x0296)
      if (start > oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]]) 
      {
         #if osdErrorHook 
          /* KB begin osekHWosSaveDisableLevelNested */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */
          osSaveSetAbsAlarm_AlarmID(alarmID)
          #if (osdNumberOfVarAlarms > 0)
           osSaveSetAbsAlarm_start(start)
           osSaveSetAbsAlarm_cycle(cycle)
           COVER(0x0297)
          #endif
          COVER(0x0298)
         #endif
         osAPIError(E_OS_VALUE, osdErrSLWrongStart);
         #if osdErrorHook 
          /* KB begin osekHWosRestoreEnableLevelNested */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          COVER(0x0299)
         #endif
         OS_SA_EXIT()
         COVER(0x029A)
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x029B)
      COVER(0x029C)
      #else /* osdUseCounterAPI */
      #if ((OSMAXALLOWEDVALUE < TICKTYPE_MAX) || (OSMINCYCLE > 1))
      if (cycle != 0)
      {
         if (
           #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX) 
              cycle > oskAlarmDefinitions.maxallowedvalue
           #else
              0
           #endif
           #if (OSMINCYCLE > 1)
              || (cycle < oskAlarmDefinitions.mincycle)
           #endif
         )
         {
            #if osdErrorHook 
             /* KB begin osekHWosSaveDisableLevelNested */
             osSaveDisableLevelNested();
             /* KB end osekHWosSaveDisableLevelNested */
             osSaveSetAbsAlarm_AlarmID(alarmID)
             #if (osdNumberOfVarAlarms > 0)
              osSaveSetAbsAlarm_start(start)
              osSaveSetAbsAlarm_cycle(cycle)
              COVER(0x029D)
             #endif
             COVER(0x029E)
            #endif
            osAPIError(E_OS_VALUE, osdErrSLWrongCycle);  
            #if osdErrorHook 
             /* KB begin osekHWosRestoreEnableLevelNested */
             osRestoreEnableLevelNested();
             /* KB end osekHWosRestoreEnableLevelNested */
             COVER(0x029F)
            #endif
            OS_SL_EXIT()
            COVER(0x02A0)
            return E_OS_VALUE;
         }
         ELSEWITHCOVER(0x02A1)
         COVER(0x02A2)
      }
      ELSEWITHCOVER(0x02A3)
      COVER(0x02A4)
      #endif
      #if (OSMAXALLOWEDVALUE < TICKTYPE_MAX)  
      if (start > oskAlarmDefinitions.maxallowedvalue)
      {
         #if osdErrorHook 
          /* KB begin osekHWosSaveDisableLevelNested */
          osSaveDisableLevelNested();
          /* KB end osekHWosSaveDisableLevelNested */
          osSaveSetAbsAlarm_AlarmID(alarmID)
          #if (osdNumberOfVarAlarms > 0)
           osSaveSetAbsAlarm_start(start)
           osSaveSetAbsAlarm_cycle(cycle)
           COVER(0x02A5)
          #endif
          COVER(0x02A6)
         #endif
         osAPIError(E_OS_VALUE, osdErrSLWrongStart);
         #if osdErrorHook 
          /* KB begin osekHWosRestoreEnableLevelNested */
          osRestoreEnableLevelNested();
          /* KB end osekHWosRestoreEnableLevelNested */
          COVER(0x02A7)
         #endif
         OS_SL_EXIT()
         COVER(0x02A8)
         return E_OS_VALUE;
      }
      ELSEWITHCOVER(0x02A9)
      COVER(0x02AA)
      #endif
      COVER(0x02AB)
      #endif /* osdUseCounterAPI */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
      COVER(0x02AC)
   }
   ELSEWITHCOVER(0x02AD)
#endif
   COVER(0x02AE)
#endif /* (osdNumberOfVarAlarms > 0) */

#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveSetAbsAlarm_AlarmID(alarmID)
       #if (osdNumberOfVarAlarms > 0)
        osSaveSetAbsAlarm_start(start)
        osSaveSetAbsAlarm_cycle(cycle)
        COVER(0x02AF)
       #endif
       COVER(0x02B0)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrSLIntAPIDisabled);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x02B1)
      #endif
      OS_SL_EXIT()
      COVER(0x02B2)
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x02B3)
   COVER(0x02B4)
#endif
COVER(0x02B5)
#endif

   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_SL_START_CRITICAL()

   /* timer running? */
#if (osdUseHeapAlarm != 0)
   COVER(0x02B6)
   if (osAlarmActivated[alarmID] != 0)
#else
   COVER(0x02B7)
   if (osAlarmTime[alarmID] != 0)
#endif
   {
      osSaveSetAbsAlarm_AlarmID(alarmID)
      #if (osdNumberOfVarAlarms > 0)
       osSaveSetAbsAlarm_start(start)
       osSaveSetAbsAlarm_cycle(cycle)
       COVER(0x02B8)
      #endif
      osAPIError(E_OS_STATE, osdErrSLAlreadyActive);
      OS_SL_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
      OS_SL_EXIT()
      COVER(0x02B9)
      return E_OS_STATE;
   }
   ELSEWITHCOVER(0x02BA)

   /* start alarm */
#if (osdNumberOfVarAlarms > 0)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
   if (alarmID < osrNumberOfVarAlarms)
   {
      COVER(0x02BB)
#endif
      /*alarm time is variable*/
      osAlarmCycleTime[alarmID] = cycle;
#if (osdUseHeapAlarm != 0)
      if (start > (osdSystemCounterVar &~ (osdSystemCounterMask)))
      {
         COVER(0x02BC)
         start|= (osSystemCounter & (TickType)osdSystemCounterMask);
      }
      else
      {
         COVER(0x02BD)
         start|= (~osSystemCounter & (TickType)osdSystemCounterMask);
      }
      osAlarmTime[alarmID] = start;
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
      COVER(0x02BE)
#else /*!osdUseHeapAlarm*/
      /*all alarms are generated alarms (no heap)*/
      {
         TickType now;
#if osdUseCounterAPI
         now = osCounter[oskAlarmCounterRef[alarmID]];
         COVER(0x02BF)
#else
         now = osGetSystemCounter();
         COVER(0x02C0)
#endif
         /*check, if start is after the next overflow*/
         if ( start > now )
         {
            /*start is before next overflow*/
            osAlarmTime[alarmID] = start - now;
            COVER(0x02C1)
         }
         else if ( start == now )
         {
            /*start is now, delay it by one tick*/
            osAlarmTime[alarmID] = 1;
            COVER(0x02C2)
         }
         else
         {
            /*an overflow is necessary to reach start time*/
#if osdUseCounterAPI
            osAlarmTime[alarmID] = (TickType)(start + 1 + 
                                   ( oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]] - now ));
            COVER(0x02C3)
#else
            osAlarmTime[alarmID] = (TickType)(start + 1 + ( OSMAXALLOWEDVALUE - now ));
            COVER(0x02C4)
#endif
         }
         COVER(0x02C5)
      }
      COVER(0x02C6)
#endif
      COVER(0x02C7)
#endif /* (osdNumberOfVarAlarms > 0) */

#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
#if (osdNumberOfVarAlarms > 0)
      COVER(0x02C8)
   }
   else
#endif
   {
      /*alarm time is statically defined in the OIL-file*/
#if (osdUseHeapAlarm != 0)
      COVER(0x02C9)
      if ( oskStaticAlarmTime[alarmID] > (osdSystemCounterVar &~ (osdSystemCounterMask)))
      {
         COVER(0x02CA)
         osAlarmTime[alarmID] = ( osSystemCounter & (TickType)osdSystemCounterMask) | oskStaticAlarmTime[alarmID];
      }
      else
      {
         COVER(0x02CB)
         osAlarmTime[alarmID] = (~osSystemCounter & (TickType)osdSystemCounterMask) | oskStaticAlarmTime[alarmID];
      }
      osAlarmActivated[alarmID] = 1;
      osInsertMinHeapWithCheck(alarmID);
#else /*!osdUseHeapAlarm*/
      /*all alarms are generated alarms (no heap)*/
      {
         TickType now;
#if osdUseCounterAPI
         now = osCounter[oskAlarmCounterRef[alarmID]];
         COVER(0x02CC)
#else
         now = osGetSystemCounter();
         COVER(0x02CD)
#endif
         /*check, whether alarm time is after the next overflow*/
         if ( oskStaticAlarmTime[alarmID] > now )
         {
            /*alarm time is before next overflow*/
            osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID] - now;
            COVER(0x02CE)
         }
         else if ( oskStaticAlarmTime[alarmID] == now )
         {
            /*alarm time is now, delay it by one tick*/
            osAlarmTime[alarmID] = 1;
            COVER(0x02CF)
         }
         else
         {
            /*an overflow is necessary to reach start time*/
#if osdUseCounterAPI
            osAlarmTime[alarmID] = (TickType)(oskStaticAlarmTime[alarmID] + 1 + 
                                   ( oskCounterMaxAllowedValue[oskAlarmCounterRef[alarmID]] -
                                     now
                                   ));
            COVER(0x02D0)
#else
            osAlarmTime[alarmID] = (TickType)(oskStaticAlarmTime[alarmID] + 1 + ( OSMAXALLOWEDVALUE - now ));
            COVER(0x02D1)
#endif
         }
         COVER(0x02D2)
      }
      COVER(0x02D3)
#endif
      COVER(0x02D4)
   }
#endif

   OS_SL_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */
   OS_SL_EXIT()
   COVER(0x02D5)
   return E_OK;
} /* END OF SetAbsAlarm */
#endif /* osdSetAbsAlarmNotUsed */


#ifndef osdCancelAlarmNotUsed
/*-+--------------------------------------------------------------------------
 / CancelAlarm
 / ===========
 /
 / Syntax:          StatusType CancelAlarm ( AlarmType <AlarmID> )
 / Parameter (In):  AlarmID Reference to an alarm
 / Parameter (Out): none
 / Description:     The system service cancels the alarm <AlarmID>.
 / Particularities: Allowed on task level and in ISR, but not in hook routines.
 / Status:
 /  -Standard:      - No error, E_OK
 /                  - Alarm <AlarmID> not in use, E_OS_NOFUNC
 /  -Extended:      - Alarm <AlarmID> is invalid, E_OS_ID
 / Conformance:     BCC1, BCC2, ECC1, ECC2
 / Context:         Task, Cat2-ISR, interrupts not disabled by the application
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osCancelAlarm(AlarmType alarmID)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osCancelAlarm */
   OS_CA_ENTRY()

#if (STATUS_LEVEL == EXTENDED_STATUS)
   /* check timer ID */
   if (alarmID >= osrNumberOfAlarms)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveCancelAlarm_AlarmID(alarmID)
       COVER(0x02D6)
      #endif
      osAPIError(E_OS_ID, osdErrCAWrongAlarmID);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x02D7)
      #endif
      OS_CA_EXIT()
      COVER(0x02D8)
      return E_OS_ID;
   }
   ELSEWITHCOVER(0x02D9)

#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
       /* KB begin osekHWosSaveDisableLevelNested */
       osSaveDisableLevelNested();
       /* KB end osekHWosSaveDisableLevelNested */
       osSaveCancelAlarm_AlarmID(alarmID)
       COVER(0x02DA)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrCAIntAPIDisabled);
      #if osdErrorHook 
       /* KB begin osekHWosRestoreEnableLevelNested */
       osRestoreEnableLevelNested();
       /* KB end osekHWosRestoreEnableLevelNested */
       COVER(0x02DB)
      #endif
      OS_CA_EXIT()
      COVER(0x02DC)
      return E_OS_SYS_DIS_INT;
   }
   COVER(0x02DD)
#endif
   COVER(0x02DE)
#endif

   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_CA_START_CRITICAL()

#if (0==osdUseHeapAlarm)
   COVER(0x02DF)
   /* if timer is inactive, return at once */
   if (osAlarmTime[alarmID] == 0)
   {
      COVER(0x02E0)
      osSaveCancelAlarm_AlarmID(alarmID)
      osAPIError(E_OS_NOFUNC, osdErrCANotActive);
      OS_CA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
      OS_CA_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x02E1)

   osAlarmTime[alarmID] = 0;
#else /*osdUseHeapAlarm*/
   COVER(0x02E2)
   /* if timer is inactive, return at once */
   if (osAlarmActivated[alarmID] == 0)
   {
      COVER(0x02E3)
      osSaveCancelAlarm_AlarmID(alarmID)
      osAPIError(E_OS_NOFUNC, osdErrCANotActive);
      OS_CA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
      OS_CA_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x02E4)

#if osdEnableAssertions
   COVER(0x02E5)
   if (osRemoveItemMinHeap(alarmID) != 0)
   {
      COVER(0x02E6)
      osAlarmActivated[alarmID] = 0; /* set alarm inactive */
   }
   else
   {
      COVER(0x02E7)
      OS_CA_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
      OS_CA_EXIT()
      osAbortSystem(osdErrCAAlarmInternal);
      return E_OS_NOFUNC;
   }
   COVER(0x02E8)
#else
   COVER(0x02E9)
   (void) osRemoveItemMinHeap(alarmID);
   osAlarmActivated[alarmID] = 0; /* set alarm inactive */
#endif

#endif /*osdUseHeapAlarm*/

   OS_CA_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */
   OS_CA_EXIT()
   COVER(0x02EA)
   return E_OK;

} /* END OF osCancelAlarm */
#endif

#else
/* KB begin osekHWosOsekAlrmDummy */
/* KB end osekHWosOsekAlrmDummy */
#endif /* (osdNumberOfAlarms > 0) */

/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */
/* END OF MODULE osekalrm.c */
