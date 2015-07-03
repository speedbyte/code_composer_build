/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekrsrc.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.38
|
|  Description: osek API-functions for handling resources
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

/* CovComment 0:  file: osekrsrc.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

#if (osdNumberOfAllResources > 0)

#if STATUS_LEVEL == EXTENDED_STATUS
#define osdResAccessMaskBits (8U*sizeof(osResAccessMaskType))
#endif

#ifndef osdGetResourceNotUsed
/*-+--------------------------------------------------------------------------
 / GetResource
 / ===========
 /
 / Syntax: StatusType GetResource ( ResourceType <ResID> )
 / Parameter (In):
 / ResID Reference to resource
 / Parameter (Out): none
 / Description: This call serves to enter critical sections in the code that are
 / assigned to the resource referenced by <ResID>. A critical
 / section must always be left using ReleaseResource.
 / Particularities: The OSEK priority ceiling protocol for resource management is
 / described in chapter 8.5.
 / Nested resource occupation is only allowed if the inner critical
 / sections are completely executed within the surrounding critical
 / section (strictly stacked, see chapter 8.2, Restrictions when
 / using resources). Nested occupation of one and the same
 / resource is also forbidden!
 / Corresponding calls to GetResource and ReleaseResource
 / should appear within the same function on the same function
 / level.
 / Services which are points of rescheduling for non preemptable
 / tasks (TerminateTask, ChainTask, Schedule and WaitEvent,
 / see chapter 4.6.2) must not be used in critical sections.
 / Additionally, critical sections must be left before completion of
 / an interrupt service routine.
 / Generally speaking, critical sections should be short.
 / The service may be called from an ISR and from task level (see
 / Figure 12-1).
 / Status:
 / Standard: - No error, E_OK
 / Extended: - Resource <ResID> is invalid, E_OS_ID
 /           - Attempt to get resource which is already occupied by any task
 /             or ISR, or the statically assigned priority of the calling task or
 /             interrupt routine is higher than the calculated ceiling priority,
 /             E_OS_ACCESS
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osGetResource(ResourceType ResId)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osGetResource */
   osPrioType ceilingPrio; /* task priority or interrupt level depending on type of resource */
#if (osdNumberOfInterruptResources > 0)
   /* KB begin osekHWosGRLocalLevelDecl */
   osPrioType currentLevel;
   /* KB end osekHWosGRLocalLevelDecl */
#endif

   OS_GR_ENTRY()
#if STATUS_LEVEL == EXTENDED_STATUS

   if ((ResId >= osrNumberOfAllResources)  
#if osdNumberOfInternalResources>0
   ||  (((ResId < osrNumberOfTaskResources) && 
         (ResId >= (osrNumberOfTaskResources- osrNumberOfInternalResources))
#if osdLib
   && (osrNumberOfInternalResources != 0)
#endif
      ))
#endif
      )
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      COVER(0x0324)
      #endif
      osAPIError(E_OS_ID, osdErrGRWrongResourceID);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x0325)

#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      COVER(0x0326)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrGRIntAPIDisabled);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0327)
#endif

   /* checks if resource is already occupied */
   if (osResOccupationCounter[ResId] != 0)
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveGetResource_ResID(ResId)
      COVER(0x0328)
      #endif
      osAPIError(E_OS_ACCESS, osdErrGRResourceOccupied);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_GR_EXIT()
      return  E_OS_ACCESS;
   }
   ELSEWITHCOVER(0x0329)

#endif

   ceilingPrio= oskResCeilingPrio[ResId];

#if (osdNumberOfInterruptResources > 0)

   if (ResId>=osrNumberOfTaskResources)
   {
      /* switch to higher processing level */
      #if STATUS_LEVEL == EXTENDED_STATUS
      /* KB begin osekHWosGRCompareIntLevel */
      /* check if level and ceiling level/prio match */
      if(osGetInitialLevel() > ceilingPrio)
      {
         COVER(0x032A)
         #if osdErrorHook 
         COVER(0x032B)
         osSaveDisableLevelNested();
         osSaveGetResource_ResID(ResId)
         #endif
         osAPIError(E_OS_ACCESS, osdErrGRWrongPrio);
         #if osdErrorHook
         COVER(0x032C)
         osRestoreEnableLevelNested();
         #endif
         OS_GR_EXIT()
         return  E_OS_ACCESS;
      }
      COVER(0x032D)
      /* KB end osekHWosGRCompareIntLevel */
      COVER(0x032E)
      #endif

      /* KB begin osekHWosSaveCurrentLevelLocal */
      COVER(0x032F)
      currentLevel = osGetLevel();
      /* KB end osekHWosSaveCurrentLevelLocal */

      /* KB begin osekHWosDisableLevel */
      osDisableLevel();    /* disable interrupts up to system level */
      /* KB end osekHWosDisableLevel */
      OS_GR_START_CRITICAL()

      /* KB begin osekHWosSaveCurrentLevelGlobal */
      COVER(0x0330)
      osResSavedPrio[ResId] = currentLevel;
      /* KB end osekHWosSaveCurrentLevelGlobal */

      /* KB begin osekIncDispatcherLock */
      #if (osdFullPreempt || osdMixedPreempt)
      COVER(0x0331)
      osLockDispatcher++;
      #endif
      /* KB end osekIncDispatcherLock */
      #if STATUS_LEVEL == EXTENDED_STATUS
      osResGlobalOccupationCounter++;
      osResOccupationCounter[ResId] = osResGlobalOccupationCounter;
      #endif

      #if STATUS_LEVEL == EXTENDED_STATUS
      /* counter needed only for mixed resource system (task, interrupt and internal) */
      
      /* increment counter only if task is accessing */
      if (osIntNestingDepth == 0)
      {
         osTcbResourceCounter[osActiveTaskIndex]++;
         COVER(0x0332)
      }
      ELSEWITHCOVER(0x0333)
      #endif
   

      #if osdORTIDebug
      if (osIntNestingDepth == 0)
      {
         /* save taskindex */
         osResActualTask[ResId]= osActiveTaskIndex;
         COVER(0x0334)
      }
      ELSEWITHCOVER(0x0335)
      #endif

      OS_GR_END_CRITICAL()
      /* KB begin osekHWosSetCeilingLevel */
      /* set level if ceiling level is above current level */
      if(currentLevel < ceilingPrio)
      {
         COVER(0x0336)
         osSetLevel(ceilingPrio);
      }
      else
      {
         COVER(0x0337)
         osSetLevel(currentLevel);
      }
      /* KB end osekHWosSetCeilingLevel */
   }
   else
   {
      /* stay on task level - business as usual */
      
#endif

#if STATUS_LEVEL == EXTENDED_STATUS

      if (osIntNestingDepth != 0)
      {
         #if osdErrorHook 
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         COVER(0x0338)
         #endif
         osAPIError(E_OS_ACCESS, osdErrGRNoAccessRights);
         #if osdErrorHook 
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_GR_EXIT()
         return E_OS_ACCESS;
      }
      ELSEWITHCOVER(0x0339)
   
      if (oskTcbHomePrio[osActiveTaskIndex] < ceilingPrio)
      {
         #if osdErrorHook 
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         COVER(0x033A)
         #endif
         osAPIError(E_OS_ACCESS, osdErrGRWrongPrio);
         #if osdErrorHook 
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_GR_EXIT()
         return  E_OS_ACCESS;
      }
      ELSEWITHCOVER(0x033B)

#if osdEnableAssertions
#if !osdLib
      /* checks access-rights of the task */

      if ((oskResAccessMask[ResId][osActiveTaskIndex/osdResAccessMaskBits] 
             & (osResAccessMaskType)((1U << (osdResAccessMaskBits-1U)) >> (uint8)(osActiveTaskIndex % osdResAccessMaskBits))) == 0U)  /* PRQA S 0501 */ /* range of shift operator is OK */
      {
         #if osdErrorHook 
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         COVER(0x033C)
         #endif
         osAPIError(E_OS_SYS_API_ERROR, osdErrGRNoAccessRights);
         #if osdErrorHook 
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_GR_EXIT()
         return  E_OS_SYS_API_ERROR;
      }
      ELSEWITHCOVER(0x033D)
#else

   {
      const osResAccessMaskType *maskPtr;
      osResAccessMaskType mask;

      maskPtr= &oskResAccessMask[0][0];
      maskPtr+= ResId * (((osrNumberOfAllTasks-1)/osdResAccessMaskBits)+1)
                + osActiveTaskIndex/osdResAccessMaskBits;

      mask= *maskPtr;
      if ((mask & (osResAccessMaskType)((1U << (osdResAccessMaskBits-1U)) >> (uint8)(osActiveTaskIndex % osdResAccessMaskBits))) == 0)  /* PRQA S 0501 */ /* range of shift operator is OK */
      {
         #if osdErrorHook 
         /* KB begin osekHWosSaveDisableLevelNested */
         osSaveDisableLevelNested();
         /* KB end osekHWosSaveDisableLevelNested */
         osSaveGetResource_ResID(ResId)
         COVER(0x033E)
         #endif
         osAPIError(E_OS_SYS_API_ERROR, osdErrGRNoAccessRights);
         #if osdErrorHook 
         /* KB begin osekHWosRestoreEnableLevelNested */
         osRestoreEnableLevelNested();
         /* KB end osekHWosRestoreEnableLevelNested */
         #endif
         OS_GR_EXIT()
         return  E_OS_SYS_API_ERROR;
      }
      ELSEWITHCOVER(0x033F)
   }

#endif /* osdLib */
#endif /* assertions */

#else
   if (osIntNestingDepth != 0)
   {
      COVER(0x0340)
      return E_OK;
   }
   ELSEWITHCOVER(0x0341)
#endif /* STANDARD_STATUS */


   /* next assertion checks if there is no bit set in the ready bits */
#if osdRTSize == 1
   COVER(0x0342)
   osSysErrAssert(0U==(osQReadyPrios & oskResCeilingPrioMask[ResId]), osdErrGRPriorityOccupied)
#else
   COVER(0x0343)
   osSysErrAssert(0U==(osQReadyPrios[oskResCeilingPrioOffset[ResId]] & oskResCeilingPrioMask[ResId])
                 , osdErrGRPriorityOccupied)
#endif

   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */
   OS_GR_START_CRITICAL()


#if (CC==BCC2) || (CC==ECC2)
   oskQActivationQueues[ceilingPrio][0]= osActiveTaskIndex;
   COVER(0x0344)
#else
   osQReadyTask[ceilingPrio]= osActiveTaskIndex;
   COVER(0x0345)
#endif

   osResSavedPrio[ResId]= osActiveTaskPrio;

   if (ceilingPrio < osActiveTaskPrio)
   {
      osActiveTaskPrio= ceilingPrio;
      COVER(0x0346)
   }
   ELSEWITHCOVER(0x0347)

   if (ceilingPrio < osHighReadyTaskPrio)
   {
      osHighReadyTaskPrio = ceilingPrio;
      osHighReadyTaskIndex = osActiveTaskIndex;
      COVER(0x0348)
   }
   ELSEWITHCOVER(0x0349)

#if osdORTIDebug
   osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
   COVER(0x034A)
#endif

#if osdRTSize == 1
   osQReadyPrios |= oskResCeilingPrioMask[ResId];
   COVER(0x034B)
#else
   osQReadyPrios[oskResCeilingPrioOffset[ResId]] |= oskResCeilingPrioMask[ResId];
   COVER(0x034C)
#endif

#if STATUS_LEVEL == EXTENDED_STATUS

   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
   /* counter needed only for mixed resource system (task, interrupt and internal) */
   
   /* increment counter only if task is accessing */
   #if (osdNumberOfInterruptResources > 0)
   if (osIntNestingDepth == 0)
   #endif
   {
      osTcbResourceCounter[osActiveTaskIndex]++;
      COVER(0x034D)
   }
   #endif

   osResGlobalOccupationCounter++;
#endif

   OS_GR_END_CRITICAL()
   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */

#if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug
   osResActualTask[ResId]= osActiveTaskIndex;
   COVER(0x034E)
#endif
#if STATUS_LEVEL == EXTENDED_STATUS
   osResOccupationCounter[ResId]= osResGlobalOccupationCounter;
   COVER(0x034F)
#endif

#if (osdNumberOfInterruptResources > 0)

   }
      
#endif

   OS_GR_EXIT()
   return E_OK;
} /* END OF osGetResource */
#endif


#ifndef osdReleaseResourceNotUsed
/*-+--------------------------------------------------------------------------
 / ReleaseResource
 / ===============
 /
 / Syntax: StatusType ReleaseResource ( ResourceType <ResID> )
 / Parameter (In):
 / ResID Reference to resource
 / Parameter (Out): none
 / Description: ReleaseResource is the counterpart of GetResource and
 / serves to leave critical sections in the code that are assigned to
 / the resource referenced by <ResID>.
 / Particularities: For information on nesting conditions, see particularities of
 / GetResource.
 / The service may be called from an ISR and from task level (see
 / Figure 12-1).
 / Status:
 / Standard: - No error, E_OK
 / Extended: - Resource <ResID> is invalid, E_OS_ID
 /           - Attempt to release a resource which is not occupied by any task
 /             or ISR, or another resource has to be released before,
 /             E_OS_NOFUNC
 /           - Attempt to release a resource which has a lower ceiling priority
 /             than the statically assigned priority of the calling task or
 /             interrupt routine, E_OS_ACCESS
 / Conformance: BCC1, BCC2, ECC1, ECC2
 /--------------------------------------------------------------------------*/

osqFunc1 StatusType osqFunc2 osReleaseResource(ResourceType ResId)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osReleaseResource */

   OS_RR_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS

   if ((ResId >= osrNumberOfAllResources) 
#if osdNumberOfInternalResources>0
   ||  (((ResId < osrNumberOfTaskResources) &&
         (ResId >= (osrNumberOfTaskResources- osrNumberOfInternalResources))
#if osdLib
   && (osrNumberOfInternalResources != 0)
#endif
      ))
#endif
      )
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      COVER(0x0350)
      #endif
      osAPIError(E_OS_ID, osdErrRRWrongResourceID);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return  E_OS_ID;
   }
   ELSEWITHCOVER(0x0351)

#if osdEnableAssertions
   if (osIntAPIStatus != 0)
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      COVER(0x0352)
      #endif
      osAPIError(E_OS_SYS_DIS_INT, osdErrRRIntAPIDisabled);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_SYS_DIS_INT;
   }
   ELSEWITHCOVER(0x0353)
#endif

   if (osResOccupationCounter[ResId] == 0)
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      COVER(0x0354)
      #endif
      osAPIError(E_OS_NOFUNC, osdErrRRNotOccupied);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x0355)

   if (osResOccupationCounter[ResId] != osResGlobalOccupationCounter)
   {
      #if osdErrorHook 
      /* KB begin osekHWosSaveDisableLevelNested */
      osSaveDisableLevelNested();
      /* KB end osekHWosSaveDisableLevelNested */
      osSaveReleaseResource_ResID(ResId)
      COVER(0x0356)
      #endif
      osAPIError(E_OS_NOFUNC, osdErrRRWrongSequence);
      #if osdErrorHook 
      /* KB begin osekHWosRestoreEnableLevelNested */
      osRestoreEnableLevelNested();
      /* KB end osekHWosRestoreEnableLevelNested */
      #endif
      OS_RR_EXIT()
      return E_OS_NOFUNC;
   }
   ELSEWITHCOVER(0x0357)

#endif

#if (osdNumberOfInterruptResources > 0)

   if (ResId>=osrNumberOfTaskResources)
   {
      /* switch from higher processing level */
      #if STATUS_LEVEL == EXTENDED_STATUS
      /* KB begin osekHWosRRCompareIntLevel */
      /* check if level and ceiling level/prio match */
      if(osGetInitialLevel() > oskResCeilingPrio[ResId])
      {
         COVER(0x0358)
         #if osdErrorHook
         COVER(0x0359)
         osSaveDisableLevelNested();
         osSaveReleaseResource_ResID(ResId)
         #endif
         osAPIError(E_OS_ACCESS, osdErrRRWrongPrio);
         #if osdErrorHook 
         COVER(0x035A)
         osRestoreEnableLevelNested();
         #endif
         OS_RR_EXIT()
         return  E_OS_ACCESS;
      }
      COVER(0x035B)
      /* KB end osekHWosRRCompareIntLevel */
      COVER(0x035C)
      #endif

      /* KB begin osekHWosDisableLevel */
      osDisableLevel();    /* disable interrupts up to system level */
      /* KB end osekHWosDisableLevel */
      OS_RR_START_CRITICAL()

      /* KB begin osekDecDispatcherLock */
      #if (osdFullPreempt || osdMixedPreempt)
      COVER(0x035D)
      osLockDispatcher--;
      #endif
      /* KB end osekDecDispatcherLock */
      #if STATUS_LEVEL == EXTENDED_STATUS

      /* counter needed only for mixed resource system (task, interrupt and internal) */

      /* increment counter only if task is accessing */
      if (osIntNestingDepth == 0)
      {
         osTcbResourceCounter[osActiveTaskIndex]--;
         COVER(0x035E)
      }

      osResGlobalOccupationCounter--;
      osResOccupationCounter[ResId]= 0;
      #endif
      
      #if osdORTIDebug
      osResActualTask[ResId]= osdNoActiveTask;
      COVER(0x035F)
      #endif
      
      #if osdFullPreempt || osdMixedPreempt
      if (osLockDispatcher == 0)
      {
         if (osSchedulePrio() != 0)
         {
            OS_RR_END_CRITICAL()
            OS_RR_EXIT()
            /* start dispatcher */
            OS_START_DISPATCH()
            /* KB begin osekHWosRestoreSavedLevel */
            COVER(0x0360)
            osSetLevel(osResSavedPrio[ResId]);
            /* KB end osekHWosRestoreSavedLevel */
            /* KB begin osekHWosDispatcher */
            osDispatch();
            /* KB end osekHWosDispatcher */
            COVER(0x0361)
            return E_OK;
         }
         ELSEWITHCOVER(0x0362)
      }
      else
      {
         if (osSchedulePrio() != 0)
         {
            osStartDispatcher=TRUE;
            COVER(0x0363)
         }
      }
      #endif

      OS_RR_END_CRITICAL()
      /* KB begin osekHWosRestoreSavedLevel */
      COVER(0x0364)
      osSetLevel(osResSavedPrio[ResId]);
      /* KB end osekHWosRestoreSavedLevel */
   }
   else
   {
      /* stay on task level - business as usual */
      
#endif

#if STATUS_LEVEL == STANDARD_STATUS
   if (osIntNestingDepth != 0)
   {
      COVER(0x0365)
      return E_OK;
   }
   ELSEWITHCOVER(0x0366)
#endif

   /* next assertion checks if releasing task is the same as the occupiing task */
   osSysErrAssert(osResActualTask[ResId] == osActiveTaskIndex, osdErrRRWrongTask)

   /* next assertion checks whether a bit is set in the ready bits */
#if osdRTSize == 1
   COVER(0x0367)
   osSysErrAssert((osQReadyPrios & oskResCeilingPrioMask[ResId]) != 0U
                 ,osdErrRRCeilingPriorityNotSet)
#else
   COVER(0x0368)
   osSysErrAssert((osQReadyPrios[oskResCeilingPrioOffset[ResId]]
                 & oskResCeilingPrioMask[ResId]) != 0U, osdErrRRCeilingPriorityNotSet)
#endif

#if ((STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions) || osdORTIDebug
   osResActualTask[ResId]= osdNoActiveTask;
   COVER(0x0369)
#endif
#if STATUS_LEVEL == EXTENDED_STATUS

   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
   /* counter needed only for mixed resource system (task, interrupt and internal) */
   
   /* increment counter only if task is accessing */
   #if (osdNumberOfInterruptResources > 0)
   COVER(0x036A)
   if (osIntNestingDepth == 0)
   #endif
   {
      osTcbResourceCounter[osActiveTaskIndex]--;
      COVER(0x036B)
   }
   #endif

   osResOccupationCounter[ResId]= 0; /* not occupied */
#endif
   
   /* KB begin osekHWosSaveDisableLevel */
   osSaveDisableLevel();        /* save level and disable up to system level, no nesting */
   /* KB end osekHWosSaveDisableLevel */

   OS_RR_START_CRITICAL()
   
#if STATUS_LEVEL == EXTENDED_STATUS
   osResGlobalOccupationCounter--;
   COVER(0x036C)
#endif

   osActiveTaskPrio= osResSavedPrio[ResId];

#if osdORTIDebug
   osTcbActualPrio[osActiveTaskIndex]=osActiveTaskPrio;
   COVER(0x036D)
#endif

#if osdRTSize == 1
   osQReadyPrios&= ~oskResCeilingPrioMask[ResId];
   COVER(0x036E)
#else
   osQReadyPrios[oskResCeilingPrioOffset[ResId]]&= ~oskResCeilingPrioMask[ResId];
   COVER(0x036F)
#endif


#if osdFullPreempt || osdMixedPreempt
   if (osLockDispatcher == 0)
   {
      if (osSchedulePrio() != 0)
      {
         OS_RR_END_CRITICAL()
         OS_RR_EXIT()

         /* KB begin osekHWosRestoreEnableLevel */
         osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
         /* KB end osekHWosRestoreEnableLevel */

         /* start dispatcher */
         OS_START_DISPATCH()
         /* KB begin osekHWosDispatcher */
         osDispatch();
         /* KB end osekHWosDispatcher */
         COVER(0x0370)
         return E_OK;
      }
      ELSEWITHCOVER(0x0371)
   }
   ELSEWITHCOVER(0x0372)
#endif
   OS_RR_END_CRITICAL()
   OS_RR_EXIT()

   /* KB begin osekHWosRestoreEnableLevel */
   osRestoreEnableLevel();         /* restore level saved by osSaveDisableLevel */
   /* KB end osekHWosRestoreEnableLevel */

#if (osdNumberOfInterruptResources > 0)

   }

#endif

   return E_OK;
} /* END OF osReleaseResource */
#endif

#else
   /* avoid error message for empty file */
   /* KB begin osekHWosOsekRsrcDummy */
   /* KB end osekHWosOsekRsrcDummy */
#endif /* (osdNumberOfAllResources > 0) */


/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */
/* END OF MODULE osekrsrc.c */
