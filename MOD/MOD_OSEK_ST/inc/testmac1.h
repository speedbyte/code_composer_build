/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: testmac1.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.16
|
|  Description: test API macro definitions
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/


/* double include preventer */
#ifndef _TESTMAC1_H
#define _TESTMAC1_H

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

#if osdORTIDebug 

#define osdORTINoService              0

/*******************/
/* Task management */
/*******************/
#define osdORTIActivateTask           1
#define osdORTITerminateTask          2
#define osdORTIChainTask              3
#define osdORTISchedule               4
#define osdORTIGetTaskID              5
#define osdORTIGetTaskState           6

/**********************/
/* Interrupt handling */
/**********************/
#define osdORTIResumeAllInterrupts    7
#define osdORTISuspendAllInterrupts   8
#define osdORTIEnableAllInterrupts    12
#define osdORTIDisableAllInterrupts   13
#define osdORTIResumeOSInterrupts     14
#define osdORTISuspendOSInterrupts    15

/***********************/
/* Resource management */
/***********************/
#define osdORTIGetResource            16
#define osdORTIReleaseResource        17

/*****************/
/* Event control */
/*****************/
#define osdORTISetEvent               18
#define osdORTIClearEvent             19
#define osdORTIGetEvent               20
#define osdORTIWaitEvent              21

/**********/
/* Alarms */
/**********/
#define osdORTIGetAlarmBase           22
#define osdORTIGetAlarm               23
#define osdORTISetRelAlarm            24
#define osdORTISetAbsAlarm            25
#define osdORTICancelAlarm            26

/**************************************/
/* Operating system execution control */
/**************************************/
#define osdORTIGetActiveApplicationMode 27   
#define osdORTIStartOS                28
#define osdORTIShutdownOS             29

/*****************/
/* Hook routines */
/*****************/
#define osdORTIErrorHook              30
#define osdORTIPreTaskHook            31
#define osdORTIPostTaskHook           32
#define osdORTIStartupHook            33
#define osdORTIShutdownHook           34

/*********************/
/* OSEK COM messages */
/*********************/
#define osdORTIStartCOM               35
#define osdORTIStopCOM                36
#define osdORTISendMessage            37
#define osdORTIReceiveMessage         38
#define osdORTIGetMessageResource     39
#define osdORTIReleaseMessageResource 40
#define osdORTIGetMessageStatus       41
#define osdORTIReadFlag               42
#define osdORTIResetFlag              43

#endif /* osdORTIDebug */

#if osdORTIDebug 
 #if osdORTIVersion == 210
  #define osORTISetCurrentServiceId(x)   (osORTICurrentServiceId = (((x)<<1)+1) )
  #define osORTIResetCurrentServiceId(x) (osORTICurrentServiceId = ( (x)<<1   ) )
 #else
   #ifndef osdORTIDebugTest
      #define osORTISetCurrentServiceId(x) (osORTICurrentServiceId = (x))
      #define osORTISaveCurrentServiceId() (osORTISavedServiceId = osORTICurrentServiceId)
      #define osORTIRestoreCurrentServiceId() (osORTICurrentServiceId = osORTISavedServiceId)
   #else
      #define osORTISetCurrentServiceId(x) (osORTICurrentServiceId = (uint8)(( (x) == 0)? osORTICurrentServiceId : (x) ))
      #define osORTISaveCurrentServiceId() (osORTISavedServiceId = osORTICurrentServiceId)
      #define osORTIRestoreCurrentServiceId() (osORTISavedServiceId = osORTISavedServiceId)
   #endif
 #endif
#else
 #if osdORTIVersion == 200
   #define osORTISetCurrentServiceId(x) ((void) 0)
   #define osORTISaveCurrentServiceId() ((void) 0)
   #define osORTIRestoreCurrentServiceId() ((void) 0)
 #endif
#endif



#if osdORTIDebug 

/*******************/
/* Task management */
/*******************/

/* ActivateTask */
#if osdORTIVersion == 210
#define OS_AT_ENTRY()            osORTISetCurrentServiceId(osdORTIActivateTask);
#define OS_AT_EXIT()             osORTIResetCurrentServiceId(osdORTIActivateTask);
#else
#define OS_AT_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIActivateTask));
#define OS_AT_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_AT_START_CRITICAL()
#define OS_AT_END_CRITICAL()

/* TerminateTask */
#if osdORTIVersion == 210
#define OS_TT_ENTRY()            osORTISetCurrentServiceId(osdORTITerminateTask);
#define OS_TT_EXIT()             osORTIResetCurrentServiceId(osdORTITerminateTask);
#else
#define OS_TT_ENTRY()            osORTISetCurrentServiceId(osdORTITerminateTask);
#define OS_TT_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_TT_START_CRITICAL()
#define OS_TT_END_CRITICAL()

/* ChainTask */
#if osdORTIVersion == 210
#define OS_HT_ENTRY()            osORTISetCurrentServiceId(osdORTIChainTask);
#define OS_HT_EXIT()             osORTIResetCurrentServiceId(osdORTIChainTask);
#else
#define OS_HT_ENTRY()            osORTISetCurrentServiceId(osdORTIChainTask);
#define OS_HT_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_HT_START_CRITICAL()
#define OS_HT_END_CRITICAL()

/* Schedule */
#if osdORTIVersion == 210
#define OS_SH_ENTRY()            osORTISetCurrentServiceId(osdORTISchedule);
#define OS_SH_EXIT()             osORTIResetCurrentServiceId(osdORTISchedule);
#else
#define OS_SH_ENTRY()            osORTISetCurrentServiceId(osdORTISchedule);
#define OS_SH_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_SH_START_CRITICAL()
#define OS_SH_END_CRITICAL()

/* osGetTaskID */
#if osdORTIVersion == 210
#define OS_GI_ENTRY()            osORTISetCurrentServiceId(osdORTIGetTaskID);
#define OS_GI_EXIT()             osORTIResetCurrentServiceId(osdORTIGetTaskID);
#else
#define OS_GI_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetTaskID));
#define OS_GI_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_GI_START_CRITICAL()
#define OS_GI_END_CRITICAL()
             
/* osGetTaskState */
#if osdORTIVersion == 210
#define OS_GS_ENTRY()            osORTISetCurrentServiceId(osdORTIGetTaskState);
#define OS_GS_EXIT()             osORTIResetCurrentServiceId(osdORTIGetTaskState);
#else
#define OS_GS_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetTaskState));
#define OS_GS_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_GS_START_CRITICAL()
#define OS_GS_END_CRITICAL()


/**********************/
/* Interrupt handling */
/**********************/

/* ResumeAllInterrupts */
#if osdORTIVersion == 210
#define OS_RA_ENTRY()            osORTISetCurrentServiceId(osdORTIResumeAllInterrupts);
#define OS_RA_EXIT()             osORTIResetCurrentServiceId(osdORTIResumeAllInterrupts);
#else
#define OS_RA_ENTRY()            {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIResumeAllInterrupts));
#define OS_RA_EXIT()                osORTIRestoreCurrentServiceId(); \
                                 }
#endif
#define OS_RA_START_CRITICAL()
#define OS_RA_END_CRITICAL()

/* SuspendAllInterrupts */
#if osdORTIVersion == 210
#define OS_PA_ENTRY()            osORTISetCurrentServiceId(osdORTISuspendAllInterrupts);
#define OS_PA_EXIT()             osORTIResetCurrentServiceId(osdORTISuspendAllInterrupts);
#else
#define OS_PA_ENTRY()            {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTISuspendAllInterrupts));
#define OS_PA_EXIT()                osORTIRestoreCurrentServiceId(); \
                                 }
#endif
#define OS_PA_START_CRITICAL()
#define OS_PA_END_CRITICAL()

/* EnableAllInterrupts */
#if osdORTIVersion == 210
#define OS_EA_ENTRY()            osORTISetCurrentServiceId(osdORTIEnableAllInterrupts);
#define OS_EA_EXIT()             osORTIResetCurrentServiceId(osdORTIEnableAllInterrupts);
#else
#define OS_EA_ENTRY()            {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIEnableAllInterrupts));
#define OS_EA_EXIT()                osORTIRestoreCurrentServiceId(); \
                                 }
#endif
#define OS_EA_START_CRITICAL()
#define OS_EA_END_CRITICAL()
 
/* DisableAllInterrupts */
#if osdORTIVersion == 210
#define OS_DA_ENTRY()            osORTISetCurrentServiceId(osdORTIDisableAllInterrupts);
#define OS_DA_EXIT()             osORTIResetCurrentServiceId(osdORTIDisableAllInterrupts);
#else
#define OS_DA_ENTRY()            {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIDisableAllInterrupts));
#define OS_DA_EXIT()                osORTIRestoreCurrentServiceId(); \
                                 }
#endif
#define OS_DA_START_CRITICAL()
#define OS_DA_END_CRITICAL()

/* ResumeOSInterrupts */
#if osdORTIVersion == 210
#define OS_RI_ENTRY()            osORTISetCurrentServiceId(osdORTIResumeOSInterrupts);
#define OS_RI_EXIT()             osORTIResetCurrentServiceId(osdORTIResumeOSInterrupts);
#else
#define OS_RI_ENTRY()            {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIResumeOSInterrupts));
#define OS_RI_EXIT()                osORTIRestoreCurrentServiceId(); \
                                 }
#endif
#define OS_RI_START_CRITICAL()
#define OS_RI_END_CRITICAL()

/* SuspendOSInterrupts */
#if osdORTIVersion == 210
#define OS_SI_ENTRY()            osORTISetCurrentServiceId(osdORTISuspendOSInterrupts);
#define OS_SI_EXIT()             osORTIResetCurrentServiceId(osdORTISuspendOSInterrupts);
#else
#define OS_SI_ENTRY()            {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTISuspendOSInterrupts));
#define OS_SI_EXIT()                osORTIRestoreCurrentServiceId(); \
                                 }
#endif
#define OS_SI_START_CRITICAL()
#define OS_SI_END_CRITICAL()


/***********************/
/* Resource management */
/***********************/

/* GetResource */
#if osdORTIVersion == 210
#define OS_GR_ENTRY()            osORTISetCurrentServiceId(osdORTIGetResource);
#define OS_GR_EXIT()             osORTIResetCurrentServiceId(osdORTIGetResource);
#else
#define OS_GR_ENTRY()            osORTISetCurrentServiceId(osdORTIGetResource);
#define OS_GR_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_GR_START_CRITICAL()
#define OS_GR_END_CRITICAL()

/* ReleaseResource */
#if osdORTIVersion == 210
#define OS_RR_ENTRY()            osORTISetCurrentServiceId(osdORTIReleaseResource);
#define OS_RR_EXIT()             osORTIResetCurrentServiceId(osdORTIReleaseResource);
#else
#define OS_RR_ENTRY()            osORTISetCurrentServiceId(osdORTIReleaseResource);
#define OS_RR_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_RR_START_CRITICAL()
#define OS_RR_END_CRITICAL()



/*****************/
/* Event control */
/*****************/

/* SetEvent */
#if osdORTIVersion == 210
#define OS_SE_ENTRY()            osORTISetCurrentServiceId(osdORTISetEvent);
#define OS_SE_EXIT()             osORTIResetCurrentServiceId(osdORTISetEvent);
#else
#define OS_SE_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTISetEvent));
#define OS_SE_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_SE_START_CRITICAL()
#define OS_SE_END_CRITICAL()

/* ClearEvent */
#if osdORTIVersion == 210
#define OS_CE_ENTRY()            osORTISetCurrentServiceId(osdORTIClearEvent);
#define OS_CE_EXIT()             osORTIResetCurrentServiceId(osdORTIClearEvent);
#else
#define OS_CE_ENTRY()            osORTISetCurrentServiceId(osdORTIClearEvent);
#define OS_CE_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_CE_START_CRITICAL()
#define OS_CE_END_CRITICAL()
                           
/* GetEvent */
#if osdORTIVersion == 210
#define OS_GE_ENTRY()            osORTISetCurrentServiceId(osdORTIGetEvent);
#define OS_GE_EXIT()             osORTIResetCurrentServiceId(osdORTIGetEvent);
#else
#define OS_GE_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetEvent));
#define OS_GE_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_GE_START_CRITICAL()
#define OS_GE_END_CRITICAL()
                           
/* WaitEvent */
#if osdORTIVersion == 210
#define OS_WE_ENTRY()            osORTISetCurrentServiceId(osdORTIWaitEvent);
#define OS_WE_EXIT()             osORTIResetCurrentServiceId(osdORTIWaitEvent);
#else
#define OS_WE_ENTRY()            osORTISetCurrentServiceId(osdORTIWaitEvent);
#define OS_WE_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_WE_START_CRITICAL()
#define OS_WE_END_CRITICAL()


/**********/
/* Alarms */
/**********/

/* GetAlarmBase */
#if osdORTIVersion == 210
#define OS_GB_ENTRY()            osORTISetCurrentServiceId(osdORTIGetAlarmBase);
#define OS_GB_EXIT()             osORTIResetCurrentServiceId(osdORTIGetAlarmBase);
#else
#define OS_GB_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetAlarmBase));
#define OS_GB_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_GB_START_CRITICAL()
#define OS_GB_END_CRITICAL()
                  
/* SetRelAlarm */
#if osdORTIVersion == 210
#define OS_SA_ENTRY()            osORTISetCurrentServiceId(osdORTISetRelAlarm);
#define OS_SA_EXIT()             osORTIResetCurrentServiceId(osdORTISetRelAlarm);
#else
#define OS_SA_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTISetRelAlarm));
#define OS_SA_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_SA_START_CRITICAL()
#define OS_SA_END_CRITICAL()

/* SetAbsAlarm */
#if osdORTIVersion == 210
#define OS_SL_ENTRY()            osORTISetCurrentServiceId(osdORTISetAbsAlarm);
#define OS_SL_EXIT()             osORTIResetCurrentServiceId(osdORTISetAbsAlarm);
#else
#define OS_SL_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTISetAbsAlarm));
#define OS_SL_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_SL_START_CRITICAL()
#define OS_SL_END_CRITICAL()

/* CancelAlarm */
#if osdORTIVersion == 210
#define OS_CA_ENTRY()            osORTISetCurrentServiceId(osdORTICancelAlarm);
#define OS_CA_EXIT()             osORTIResetCurrentServiceId(osdORTICancelAlarm);
#else
#define OS_CA_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTICancelAlarm));
#define OS_CA_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_CA_START_CRITICAL()
#define OS_CA_END_CRITICAL()

/* GetAlarm */
#if osdORTIVersion == 210
#define OS_GA_ENTRY()            osORTISetCurrentServiceId(osdORTIGetAlarm);
#define OS_GA_EXIT()             osORTIResetCurrentServiceId(osdORTIGetAlarm);
#else
#define OS_GA_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetAlarm));
#define OS_GA_EXIT()             osORTIRestoreCurrentServiceId();
#endif
#define OS_GA_START_CRITICAL()
#define OS_GA_END_CRITICAL()
                           
               
/**************************************/
/* Operating system execution control */
/**************************************/
                 
/* StartOS */
#if osdORTIVersion == 210
#define OS_ST_ENTRY()            osORTISetCurrentServiceId(osdORTIStartOS);
#define OS_ST_EXIT()             osORTIResetCurrentServiceId(osdORTIStartOS);
#else
#define OS_ST_ENTRY()            osORTISetCurrentServiceId(osdORTIStartOS);
#define OS_ST_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_ST_START_CRITICAL()
#define OS_ST_END_CRITICAL()

/* ShutdownOS */
#if osdORTIVersion == 210
#define OS_SD_ENTRY()            osORTISetCurrentServiceId(osdORTIShutdownOS);
#define OS_SD_EXIT()             osORTIResetCurrentServiceId(osdORTIShutdownOS);
#else
#define OS_SD_ENTRY()            osORTISetCurrentServiceId(osdORTIShutdownOS);
#define OS_SD_EXIT()             osORTISetCurrentServiceId(osdORTINoService);
#endif
#define OS_SD_START_CRITICAL()
#define OS_SD_END_CRITICAL()


/*****************/
/* Hook routines */
/*****************/

/* ErrorHook */
#if osdORTIVersion == 210
#define OS_ERHOOK_ENTRY()        osORTISetCurrentServiceId(osdORTIErrorHook);
#define OS_ERHOOK_EXIT()         osORTIResetCurrentServiceId(osdORTIErrorHook);
                                                
/* StartupHook */
#define OS_STHOOK_ENTRY()        osORTISetCurrentServiceId(osdORTIStartupHook);
#define OS_STHOOK_EXIT()         osORTIResetCurrentServiceId(osdORTIStartupHook);

/* ShutdownHook */               
#define OS_SDHOOK_ENTRY()        osORTISetCurrentServiceId(osdORTIShutdownHook);
#define OS_SDHOOK_EXIT()         osORTIResetCurrentServiceId(osdORTIShutdownHook);

/* PreTaskHook */
#define OS_PREHOOK_ENTRY()       osORTISetCurrentServiceId(osdORTIPreTaskHook);
#define OS_PREHOOK_EXIT()        osORTIResetCurrentServiceId(osdORTIPreTaskHook);

/* PostTaskHook */
#define OS_POSTHOOK_ENTRY()      osORTISetCurrentServiceId(osdORTIPostTaskHook);
#define OS_POSTHOOK_EXIT()       osORTIResetCurrentServiceId(osdORTIPostTaskHook);
#else
#define OS_ERHOOK_ENTRY()        {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIErrorHook)) ;
#define OS_ERHOOK_EXIT()            osORTIRestoreCurrentServiceId(); \
                                 }
                                                
/* StartupHook */
#define OS_STHOOK_ENTRY()        {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIStartupHook)) ;
#define OS_STHOOK_EXIT()            osORTIRestoreCurrentServiceId(); \
                                 }

/* ShutdownHook */               
#define OS_SDHOOK_ENTRY()        {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIShutdownHook)) ;
#define OS_SDHOOK_EXIT()            osORTIRestoreCurrentServiceId(); \
                                 }

/* PreTaskHook */
#define OS_PREHOOK_ENTRY()       {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIPreTaskHook)); 
#define OS_PREHOOK_EXIT()           osORTIRestoreCurrentServiceId(); \
                                 }

/* PostTaskHook */
#define OS_POSTHOOK_ENTRY()      {  \
                                    register uint8 osORTISavedServiceId; \
                                    (osORTISaveCurrentServiceId(), \
                                    osORTISetCurrentServiceId(osdORTIPostTaskHook)); 
#define OS_POSTHOOK_EXIT()          osORTIRestoreCurrentServiceId(); \
                                 }
#endif

/*********************/
/* OSEK COM messages */
/*********************/

/* StartCOM */
#if osdORTIVersion == 210
#define OS_SC_ENTRY()            osORTISetCurrentServiceId(osdORTIStartCOM);
#define OS_SC_EXIT()             osORTIResetCurrentServiceId(osdORTIStartCOM);
#else
#define OS_SC_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIStartCOM)) ;          
#define OS_SC_EXIT()             osORTIRestoreCurrentServiceId();            
#endif
#define OS_SC_START_CRITICAL()
#define OS_SC_END_CRITICAL()
                           
/* StopCOM */
#if osdORTIVersion == 210
#define OS_TC_ENTRY()            osORTISetCurrentServiceId(osdORTIStopCOM);
#define OS_TC_EXIT()             osORTIResetCurrentServiceId(osdORTIStopCOM);
#else
#define OS_TC_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIStopCOM));            
#define OS_TC_EXIT()             osORTIRestoreCurrentServiceId();             
#endif
                           
/* SendMessage */
#if osdORTIVersion == 210
#define OS_SM_ENTRY()            osORTISetCurrentServiceId(osdORTISendMessage);
#define OS_SM_EXIT()             osORTIResetCurrentServiceId(osdORTISendMessage);
#else
#define OS_SM_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTISendMessage));            
#define OS_SM_EXIT()             osORTIRestoreCurrentServiceId();             
#endif
#define OS_SM_START_CRITICAL()
#define OS_SM_END_CRITICAL()
                           
/* ReceiveMessage */
#if osdORTIVersion == 210
#define OS_RM_ENTRY()            osORTISetCurrentServiceId(osdORTIReceiveMessage);
#define OS_RM_EXIT()             osORTIResetCurrentServiceId(osdORTIReceiveMessage);
#else
#define OS_RM_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIReceiveMessage));            
#define OS_RM_EXIT()             osORTIRestoreCurrentServiceId();             
#endif
#define OS_RM_START_CRITICAL()
#define OS_RM_END_CRITICAL()

/* GetMessageResource */
#if osdORTIVersion == 210
#define OS_GG_ENTRY()            osORTISetCurrentServiceId(osdORTIGetMessageResource);
#define OS_GG_EXIT()             osORTIResetCurrentServiceId(osdORTIGetMessageResource);
#else
#define OS_GG_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetMessageResource));            
#define OS_GG_EXIT()             osORTIRestoreCurrentServiceId();             
#endif
#define OS_GG_START_CRITICAL()
#define OS_GG_END_CRITICAL()
                           
/* ReleaseMessageResource */
#if osdORTIVersion == 210
#define OS_RG_ENTRY()            osORTISetCurrentServiceId(osdORTIReleaseMessageResource);
#define OS_RG_EXIT()             osORTIResetCurrentServiceId(osdORTIReleaseMessageResource);
#else
#define OS_RG_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIReleaseMessageResource));            
#define OS_RG_EXIT()             osORTIRestoreCurrentServiceId();             
#endif
#define OS_RG_START_CRITICAL()
#define OS_RG_END_CRITICAL()

#if osdORTIVersion == 210
/* GetMessageStatus */
#define OS_MS_ENTRY()            osORTISetCurrentServiceId(osdORTIGetMessageStatus);
#define OS_MS_EXIT()             osORTIResetCurrentServiceId(osdORTIGetMessageStatus);      
                           
/* ReadFlag */
#define OS_RF_ENTRY()            osORTISetCurrentServiceId(osdORTIReadFlag);
#define OS_RF_EXIT()             osORTIResetCurrentServiceId(osdORTIReadFlag);    
                           
/* ResetFlag */
#define OS_ZF_ENTRY()            osORTISetCurrentServiceId(osdORTIResetFlag);
#define OS_ZF_EXIT()             osORTIResetCurrentServiceId(osdORTIResetFlag);    
#else
/* GetMessageStatus */
#define OS_MS_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIGetMessageStatus));            
#define OS_MS_EXIT()             osORTIRestoreCurrentServiceId();             
                           
/* ReadFlag */
#define OS_RF_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIReadFlag));            
#define OS_RF_EXIT()             osORTIRestoreCurrentServiceId();             
                           
/* ResetFlag */
#define OS_ZF_ENTRY()            register uint8 osORTISavedServiceId; \
                                 (osORTISaveCurrentServiceId(), \
                                 osORTISetCurrentServiceId(osdORTIResetFlag));            
#define OS_ZF_EXIT()             osORTIRestoreCurrentServiceId();             
#endif
                           

/**************************/
/* miscellaneous routines */
/**************************/

/* osTimerInterrupt */             
#define OS_TI_ENTRY()            
#define OS_TI_EXIT()
#define OS_TI_START_CRITICAL()
#define OS_TI_END_CRITICAL()

/* TriggerCounter (Increment Counter) x = counter index */           
#define OS_IC_ENTRY(x)            
#define OS_IC_EXIT(x)
#define OS_IC_START_CRITICAL(x)
#define OS_IC_END_CRITICAL(x)

/* dispatcher */
#define OS_START_DISPATCH()

/* idle loop */
#define OS_IDLE_ENTRY()
#define OS_IDLE_PROC()      

/* Hardware specific macro */
/* KB begin osekHwEmptyMacro */
/* KB end osekHwEmptyMacro */

#endif /* osdORTIDebug */

#endif /* _TESTMAC1_H */
