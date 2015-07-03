/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|
|    File Name: osektime.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.34
|
|  Description: osek timer handling
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

/* CovComment 0:  file: osektime.c */
/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */



#if (osdNumberOfAlarms > 0)
#if (osdUseHeapAlarm != 0) && (osdNumberOfSysAlarms > 0)


#define osGetHeapTime(x) (osAlarmTime[x])
#define osGetFirstAlarm() (osAlarmHeap[0])
#define osGetActiveAlarmCount() (osAlarmHeapCount)


/*-+--------------------------------------------------------------------------
 / osInitMinHeap
 / Funktion:
 /   - initialize empty minimum heap
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static osqFunc1 void osqFunc2 osInitMinHeap(void);

static osqFunc1 void osqFunc2 osInitMinHeap(void)
{
   /* CovComment 4:  Function: osInitMinHeap */
   COVER(0x02EB)
   osAlarmHeapCount = 0;
}


/*-+--------------------------------------------------------------------------
 / osClimbUpMinHeap
 / Funktion:
 /   - move last element up to correct position
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static osqFunc1 void osqFunc2 osClimbUpMinHeap(osAlarmIndexType i);

static osqFunc1 void osqFunc2 osClimbUpMinHeap(osAlarmIndexType i)
{
   /* CovComment 4:  Function: osClimbUpMinHeap */
   osHeapType hItem;
   osAlarmIndexType iPreIdx;

   hItem = osAlarmHeap[i];  /* copy (last) item */
   while (i != 0)
   {
      /* calculate index of predecessor */
      iPreIdx = (osAlarmIndexType)(((osAlarmIndexType)(i-1)) >> 1U);
   
      osdIF_COUNTER( osGetHeapTime(hItem), >=, osGetHeapTime(osAlarmHeap[iPreIdx]))
      {
         COVER(0x02EC)
         break;   /* correct position found */
      }osdENDIF_COUNTER

      /* swap items */
      osAlarmHeap[i] = osAlarmHeap[iPreIdx];
      i = iPreIdx;
      COVER(0x02ED)
   }
   osAlarmHeap[i] = hItem;   /* insert at correct position */
   COVER(0x02EE)
}


/*-+--------------------------------------------------------------------------
 / osClimbDownMinHeap
 / Funktion:
 /   - move element i down to correct position
 / Parameter:     i = element index
 / Returnvalue:
 /--------------------------------------------------------------------------*/

static osqFunc1 void osqFunc2 osClimbDownMinHeap(osAlarmIndexType i);

static osqFunc1 void osqFunc2 osClimbDownMinHeap(osAlarmIndexType i)
{
   /* CovComment 4:  Function: osClimbDownMinHeap */
   osHeapType hItem;
   osAlarmIndexType iPostIdx;
   iPostIdx = (((osAlarmIndexType)2*i) + (osAlarmIndexType)1);/*lint !e734*/
   /*
    * Info 734 Loss of precision (assignment)
    * The number of alarms is checked to be less than 2^(sizeof(osAlarmIndexType)-1).
    * So no overflow can occur here.
    */
   hItem = osAlarmHeap[i];  /* copy root item */
   while (iPostIdx < osAlarmHeapCount)
   {
      osdIF_COUNTER(osGetHeapTime(osAlarmHeap[iPostIdx]), > , osGetHeapTime(osAlarmHeap[iPostIdx+1]))
      {
         COVER(0x02EF)
         iPostIdx++;   /* set iPostIdx to smaller successor */

      }osdENDIF_COUNTER
      osdIF_COUNTER(osGetHeapTime(osAlarmHeap[iPostIdx]), >= , osGetHeapTime(hItem))
      {
         COVER(0x02F0)
         break;        /* correct position found */

      }osdENDIF_COUNTER
      COVER(0x02F1)
      osAlarmHeap[i] = osAlarmHeap[iPostIdx];
      i = iPostIdx;
      iPostIdx = (((osAlarmIndexType)2*i) + (osAlarmIndexType)1);/*lint !e734, same arguments as above*/
   }
   COVER(0x02F2)
   osAlarmHeap[i] = hItem;
}


/*-+--------------------------------------------------------------------------
 / osInsertMinHeap
 / Funktion:
 /   - insert new item
 / Parameter:     newItem = item to insert
 / Returnvalue:
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osInsertMinHeap(osHeapType newItem)
{
   /* CovComment 4:  Function: osInsertMinHeap */
   osAlarmHeap[osAlarmHeapCount] = newItem;  /* insert new item at last position */
   osAlarmHeapCount++;
   osClimbUpMinHeap((osAlarmIndexType)(osAlarmHeapCount-1));  /* climb up until correct position reached */
   COVER(0x02F3)
}



/*-+--------------------------------------------------------------------------
 / osRemoveMinItemMinHeap
 / Funktion:
 /   - get minimum item and remove item from heap
 / Parameter:
 / Returnvalue: index of minimum item
 /--------------------------------------------------------------------------*/
static osqFunc1 osHeapType osqFunc2 osRemoveMinItemMinHeap(void);

static osqFunc1 osHeapType osqFunc2 osRemoveMinItemMinHeap(void)
{
   /* CovComment 4:  Function: osRemoveMinItemMinHeap */
   osHeapType hItem;

   hItem = osAlarmHeap[0];   /* copy first = minimum item */
   osAlarmHeapCount--;
   osAlarmHeap[0] = osAlarmHeap[osAlarmHeapCount];   /* move last element to first position */
   osClimbDownMinHeap(0);   /* climb down until correct position reached */
   COVER(0x02F4)
   return hItem;
}


/*-+--------------------------------------------------------------------------
 / osRemoveItemMinHeap
 / Funktion:
 /   - find and remove Item
 / Parameter:    aItem = Item to remove
 / Returnvalue:  0     = item to remove not found in heap, 
 /               1     = item found and removed from heap
 /--------------------------------------------------------------------------*/
osqFunc1 uint8 osqFunc2 osRemoveItemMinHeap(osHeapType aItem)
{
   /* CovComment 4:  Function: osRemoveItemMinHeap */
   osAlarmIndexType iPreIdx;
   osAlarmIndexType i = 0;

   osAlarmHeap[osAlarmHeapCount] = aItem;
   /* now the following loop will always terminate without range control */

   for (;;)
   {
      if (osAlarmHeap[i] == aItem)
      {
         COVER(0x02F5)
         break;   /* found! */
      }
      ELSEWITHCOVER(0x02F6)
      i++;
   }
#if osdEnableAssertions
   if (i >= osAlarmHeapCount)
   {
       COVER(0x02F7)
       return 0;/*heap item not found*/
   }
   ELSEWITHCOVER(0x02F8)
#endif
   osAlarmHeapCount--;
   osAlarmHeap[i] = osAlarmHeap[osAlarmHeapCount];  /* move last item to found item */
   if (i > 0) /* (if heap element has predecessor) check array boundaries */
   {
      iPreIdx =  (osAlarmIndexType)(((osAlarmIndexType)(i-1)) >> 1U);

      osdIF_COUNTER(osGetHeapTime(osAlarmHeap[i]), < , osGetHeapTime(osAlarmHeap[iPreIdx]))
      {
         osClimbUpMinHeap(i);
         COVER(0x02F9)
         return 1;
      }osdENDIF_COUNTER
      COVER(0x02FA)
   }
   ELSEWITHCOVER(0x02FB)
   
   osClimbDownMinHeap(i);  /* climb down until correct position reached */
   COVER(0x02FC)
   return 1;
}


/*-+--------------------------------------------------------------------------
 / osWorkAlarms
 / Funktion:
 /   - inserts or reinserts an alarm into timer class
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osWorkAlarms(void)
{
   /* CovComment 4:  Function: osWorkAlarms */
   AlarmType alarmID;
   osHeapType t;

   COVER(0x02FD)
   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_TI_START_CRITICAL()
   while (osGetActiveAlarmCount() != 0)
   {
      COVER(0x02FE)
      alarmID = osGetFirstAlarm();

      /* is timer expired? */
      osdIF_COUNTER(osGetSystemCounter(), >= , osAlarmTime[alarmID])
      {
         COVER(0x02FF)
         t = osRemoveMinItemMinHeap();  /* remove alarm */
         /* alarm has become due now: work actions */

#if (osdNumberOfVarAlarms > 0)
         COVER(0x0300)
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
         COVER(0x0301)
         if (alarmID < osrNumberOfVarAlarms)
         {
            COVER(0x0302)
#endif
            if (osAlarmCycleTime[alarmID] != 0)
            {
               COVER(0x0303)
               /* cyclic alarm */
               osAlarmTime[alarmID] += osAlarmCycleTime[alarmID];
               osInsertMinHeap(t);      /* reinsert      */
            }
            else
            {
               COVER(0x0304)
               osAlarmActivated[alarmID]= 0;
            }
#endif
#if (osdNumberOfVarAlarms < osdNumberOfAlarms)
            COVER(0x0305)
#if (osdNumberOfVarAlarms > 0)
         COVER(0x0306)
         }
         else
#endif
         {
            COVER(0x0307)
            if (oskStaticAlarmCycleTime[alarmID] != 0)
            {
               /* cyclic alarm  */
               COVER(0x0308)
               osAlarmTime[alarmID] += oskStaticAlarmCycleTime[alarmID];
               osInsertMinHeap(t);      /* reinsert      */
            }
            else
            {
               COVER(0x0309)
               osAlarmActivated[alarmID]= 0;
            }
         }
#endif

#if (osdNumberOfAlarmSetEvent > 0)
         COVER(0x030A)
         if (oskAlarmAction[alarmID] == osdAlarmActionSetEvent)
         {
            COVER(0x030B)
            /* event timer */
            (void) osSysSetEvent(oskAlarmTask[alarmID], oskAlarmEvent[alarmID]); 
         }
         else
#endif
#if (osdNumberOfAlarmCallback > 0)
         if (oskAlarmAction[alarmID] == osdAlarmActionCallback)
         {
            COVER(0x030C)
            (oskAlarmCallback[alarmID])(); 
         }
         else
#endif
         {
#if (osdNumberOfAlarmActivateTask > 0)
            COVER(0x030D)
            (void) osSysActivateTask(oskAlarmTask[alarmID]);
#endif
         }
      }
      else /* osdIF_COUNTER */
      {
         {
            COVER(0x030E)
            break;
         }
      }osdENDIF_COUNTER
      COVER(0x030F)
      /* After each activated alarm, higher prio interrupts are enabled for 
         a brief moment. This gives the possibility to execute pending interrupts 
         to decreases latency.
      */
      OS_TI_END_CRITICAL()
      /* KB begin osekHWosRestoreEnableLevel */
      osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
      /* KB end osekHWosRestoreEnableLevel */
      /* KB begin osekHWdelayTilEnableTookEffect */
      /* KB end osekHWdelayTilEnableTookEffect */
      /* KB begin osekHWosSaveDisableLevel */
      osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
      /* KB end osekHWosSaveDisableLevel */
      OS_TI_START_CRITICAL()
   }/*while*/

   OS_TI_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */
   COVER(0x0310)

} /* END OF osWorkAlarms */
#endif /*!osdUseHeapAlarm || !osdNumberOfSysAlarms*/


/*-+--------------------------------------------------------------------------
 / osInitAlarms
 / Funktion:
 /   - prepares the RAM part of all alarms
 / Parameter:
 / Returnvalue:
 /--------------------------------------------------------------------------*/

osqFunc1 void osqFunc2 osInitAlarms(void)
{
   /* CovComment 4:  Function: osInitAlarms */
   #if osdUseCounterAPI
   uint8 i;
   #endif
   osAlarmIndexType alarmID;

   /* has to be done before activation of auto start alarms */
   #if osdUseCounterAPI
   COVER(0x0311)
   for(i=0;i<osdNumberOfCounters;i++)
   {
      COVER(0x0312)
      osCounter[i]=0;
   }
   #else
   COVER(0x0313)
   osSystemCounter = 0;
   #endif

   #if (osdUseHeapAlarm != 0) && (osdNumberOfSysAlarms > 0)
   COVER(0x0314)
   osInitMinHeap();
   #endif

#if (osdNumberOfAlarms > 0)
   for (alarmID= 0; alarmID < osrNumberOfAlarms; alarmID++)
   {
      #if osdNumberOfAppModes > 1
      COVER(0x0315)
      if ((oskAlarmAutoStart[alarmID] & osActiveAppMode) != 0)
      #else
      COVER(0x0316)
      if (oskAlarmAutoStart[alarmID] != 0)
      #endif
      {
#if (osdUseHeapAlarm != 0)
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID];
         osAlarmActivated[alarmID] = 1;
         osInsertMinHeap(alarmID);
         COVER(0x0317)
#else
         osAlarmTime[alarmID] = oskStaticAlarmTime[alarmID];
         COVER(0x0318)
#endif
      }
      else
      {
#if (osdUseHeapAlarm != 0)
         COVER(0x0319)
         osAlarmActivated[alarmID] = 0;
#else
         COVER(0x031A)
         osAlarmTime[alarmID] = 0;
#endif
      }
   }
#endif /*osdNumberOfAlarms*/
#if (osdNumberOfVarAlarms > 0)
   /*complete the init of all variable alarms*/
   for (alarmID= 0; alarmID < osrNumberOfVarAlarms; alarmID++)
   {
      #if osdNumberOfAppModes > 1
      COVER(0x031B)
      if ((oskAlarmAutoStart[alarmID] & osActiveAppMode) != 0)
      #else
      COVER(0x031C)
      if (oskAlarmAutoStart[alarmID] != 0)
      #endif
      {
         osAlarmCycleTime[alarmID] = oskStaticAlarmCycleTime[alarmID];
         COVER(0x031D)
      }
      else
      {
         osAlarmCycleTime[alarmID] = 0;
         COVER(0x031E)
      }
   }
   COVER(0x031F)
#endif
#if (osdUserDefinedSystemTimer == 0) && ((osdNumberOfSysAlarms > 0) || (osdProvideTimeStamp != 0))
   osInitTimer();
   COVER(0x0320)
#endif
} /* END OF osInitAlarms */

#else  /*!osdNumberOfAlarms*/

osqFunc1 void osqFunc2 osInitAlarms(void) /*short version for the zero alarms variant*/
{
   /* CovComment 4:  Function: osInitAlarms */
#if osdUseCounterAPI
   uint8 i;
   for (i=0; i<osdNumberOfCounters; i++)
   {
      osCounter[i]=0;
      COVER(0x0321)
   }
#else
   osSystemCounter = 0;
   COVER(0x0322)
#endif
#if (0==osdUserDefinedSystemTimer) && ((osdNumberOfSysAlarms > 0) || (osdProvideTimeStamp != 0))
   osInitTimer();
   COVER(0x0323)
#endif
} /* END OF osInitAlarms */

/* KB begin osekHWosOsekTimeDummy */
/* KB end osekHWosOsekTimeDummy */

#endif  /* (osdNumberOfAlarms > 0) */


#if (0==osdUserDefinedSystemTimer) && ((osdNumberOfSysAlarms > 0) || (osdProvideTimeStamp != 0))
/* KB begin osekHWInitTimerSrc */
/* KB end osekHWInitTimerSrc */
#endif


#if (0==osdUserDefinedSystemTimer) && ((osdNumberOfSysAlarms > 0) || (osdProvideTimeStamp != 0))
/* KB begin osekHWTimerInterruptSrc */
/* KB end osekHWTimerInterruptSrc */
#endif

/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */

/* END OF MODULE osektime.c */


