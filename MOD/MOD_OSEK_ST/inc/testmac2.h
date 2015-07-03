/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: testmac2.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.12
|
|  Description: test API macro definitions - trace functions
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2007 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _testmac2_H
#define _testmac2_H

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

/* may be customized if more than 256 tasks have to be traced or a 16 Bit time stamp is not sufficient */
#define osGetActiveTaskNo (uint8)osActiveTaskIndex /* get lower 8 Bits of number of active task */
#ifdef osdTraceUseUserTimestamp
/* this function has to be supplied by the application if user defined timestamp is selected */
osqFunc1 uint16 osqFunc2 osGetUserTimeStamp(void); 
#define osGetTimeStamp osGetUserTimeStamp()    /* get user defined time stamp counter */
#else
#define osGetTimeStamp (uint16)osGetSystemCounter() /* get lower 16 Bits of system tick counter */
#endif

#ifdef osdUseTrace

/* trace buffer */
typedef struct sOsTraceBuffer
{
   uint8 stateNo;
   uint8 taskNo;
#ifdef osdTraceUseTimestamp
   uint16 timeStamp;
#endif
} tOsTraceBuffer;

/* global variables of the trace */
extern tOsTraceBuffer osTraceBuffer[osdTraceDepth];
extern uint16 osTraceBufferIndex;

/* initialisation of trace */
osqFunc1 void osqFunc2 osInitTrace(void);
#ifdef osdTraceUsePrintout
osqFunc1 void osqFunc2 osPrintTrace(void);
#else
#define osPrintTrace()
#endif

/* trace "function" */
#ifdef osdTraceUseTimestamp
#define osInsertTimeStamp osTraceBuffer[osTraceBufferIndex].timeStamp=osGetTimeStamp;
#else
#define osInsertTimeStamp
#endif
#define osTrace(state) \
osSaveDisableLevelNested(); \
osTraceBuffer[osTraceBufferIndex].stateNo=state; \
osTraceBuffer[osTraceBufferIndex].taskNo=osGetActiveTaskNo; \
osInsertTimeStamp \
osTraceBufferIndex++; \
if(osTraceBufferIndex==osdTraceDepth) \
   {osTraceBufferIndex=0;} \
osRestoreEnableLevelNested(); 

#else
#define osTrace(state)
#define osInitTrace()
#endif /* osdUseTrace */

#define osdTraceNoService              0

/*******************/
/* Task management */
/*******************/
#define osdTraceActivateTask           1
#define osdTraceTerminateTask          2
#define osdTraceChainTask              3
#define osdTraceSchedule               4
#define osdTraceGetTaskID              5
#define osdTraceGetTaskState           6

/**********************/
/* Interrupt handling */
/**********************/
#define osdTraceResumeAllInterrupts    7     
#define osdTraceSuspendAllInterrupts   8     
#define osdTraceEnableAllInterrupts    12
#define osdTraceDisableAllInterrupts   13
#define osdTraceResumeOSInterrupts     14
#define osdTraceSuspendOSInterrupts    15

/***********************/
/* Resource management */
/***********************/
#define osdTraceGetResource            16
#define osdTraceReleaseResource        17

/*****************/
/* Event control */
/*****************/
#define osdTraceSetEvent               18
#define osdTraceClearEvent             19
#define osdTraceGetEvent               20
#define osdTraceWaitEvent              21

/**********/
/* Alarms */
/**********/
#define osdTraceGetAlarmBase           22
#define osdTraceGetAlarm               23
#define osdTraceSetRelAlarm            24
#define osdTraceSetAbsAlarm            25
#define osdTraceCancelAlarm            26

/**************************************/
/* Operating system execution control */
/**************************************/
#define osdTraceGetActiveApplicationMode 27   
#define osdTraceStartOS                28
#define osdTraceShutdownOS             29

/*****************/
/* Hook routines */
/*****************/
#define osdTraceErrorHook              30
#define osdTracePreTaskHook            31 
#define osdTracePostTaskHook           32
#define osdTraceStartupHook            33
#define osdTraceShutdownHook           34

/*********************/
/* OSEK COM messages */
/*********************/
#define osdTraceStartCOM               35
#define osdTraceStopCOM                36
#define osdTraceSendMessage            37
#define osdTraceReceiveMessage         38
#define osdTraceGetMessageResource     39
#define osdTraceReleaseMessageResource 40
#define osdTraceGetMessageStatus       41
#define osdTraceReadFlag               42
#define osdTraceResetFlag              43

/*****************/
/* Misc routines */
/*****************/
#define osdTraceIdleEntry              44
#define osdTraceSytemTimer             45
#define osdTraceStartDispatch          46


/*****************/
/* User routines */
/*****************/
#define osdTraceUser00              100
#define osdTraceUser01              101
#define osdTraceUser02              102
#define osdTraceUser03              103
#define osdTraceUser04              104
#define osdTraceUser05              105
#define osdTraceUser06              106
#define osdTraceUser07              107
#define osdTraceUser08              108
#define osdTraceUser09              109
#define osdTraceUser10              110
#define osdTraceUser11              111
#define osdTraceUser12              112
#define osdTraceUser13              113
#define osdTraceUser14              114
#define osdTraceUser15              115
#define osdTraceUser16              116
#define osdTraceUser17              117
#define osdTraceUser18              118
#define osdTraceUser19              119

/* ########################################################################## */
/* ######################### adapt this if required ######################### */
/* ########################################################################## */
/* Insert or remove osTrace macros in the debug macros for specific           */
/* application dependent tracing. The osdTraceUser macros  may be used        */
/* in the application for additional trace points.                            */

/*******************/
/* Task management */
/*******************/

/* ActivateTask */
#define OS_AT_ENTRY()            osTrace(osdTraceActivateTask)
#define OS_AT_EXIT()             
#define OS_AT_START_CRITICAL()
#define OS_AT_END_CRITICAL()

/* TerminateTask */
#define OS_TT_ENTRY()            osTrace(osdTraceTerminateTask)
#define OS_TT_EXIT()             
#define OS_TT_START_CRITICAL()
#define OS_TT_END_CRITICAL()

/* ChainTask */
#define OS_HT_ENTRY()            osTrace(osdTraceChainTask)
#define OS_HT_EXIT()             
#define OS_HT_START_CRITICAL()
#define OS_HT_END_CRITICAL()

/* Schedule */
#define OS_SH_ENTRY()            osTrace(osdTraceSchedule)
#define OS_SH_EXIT()             
#define OS_SH_START_CRITICAL()
#define OS_SH_END_CRITICAL()

/* osGetTaskID */
#define OS_GI_ENTRY()            osTrace(osdTraceGetTaskID)
#define OS_GI_EXIT()             
#define OS_GI_START_CRITICAL()
#define OS_GI_END_CRITICAL()
             
/* osGetTaskState */
#define OS_GS_ENTRY()            osTrace(osdTraceGetTaskState)
#define OS_GS_EXIT()             
#define OS_GS_START_CRITICAL()
#define OS_GS_END_CRITICAL()


/**********************/
/* Interrupt handling */
/**********************/

/* ResumeAllInterrupts */
#define OS_RA_ENTRY()            osTrace(osdTraceResumeAllInterrupts)
#define OS_RA_EXIT() 
#define OS_RA_START_CRITICAL()
#define OS_RA_END_CRITICAL()

/* SuspendAllInterrupts */
#define OS_PA_ENTRY()            osTrace(osdTraceSuspendAllInterrupts)
#define OS_PA_EXIT()
#define OS_PA_START_CRITICAL()
#define OS_PA_END_CRITICAL()

/* EnableAllInterrupts */
#define OS_EA_ENTRY()            osTrace(osdTraceEnableAllInterrupts)
#define OS_EA_EXIT()             
#define OS_EA_START_CRITICAL()
#define OS_EA_END_CRITICAL()
 
/* DisableAllInterrupts */
#define OS_DA_ENTRY()            osTrace(osdTraceDisableAllInterrupts)
#define OS_DA_EXIT()             
#define OS_DA_START_CRITICAL()
#define OS_DA_END_CRITICAL()

/* ResumeOSInterrupts */
#define OS_RI_ENTRY()            osTrace(osdTraceResumeOSInterrupts)
#define OS_RI_EXIT()             
#define OS_RI_START_CRITICAL()
#define OS_RI_END_CRITICAL()

/* SuspendOSInterrupts */
#define OS_SI_ENTRY()            osTrace(osdTraceSuspendOSInterrupts)
#define OS_SI_EXIT()             
#define OS_SI_START_CRITICAL()
#define OS_SI_END_CRITICAL()


/***********************/
/* Resource management */
/***********************/

/* GetResource */
#define OS_GR_ENTRY()            osTrace(osdTraceGetResource)
#define OS_GR_EXIT()             
#define OS_GR_START_CRITICAL()
#define OS_GR_END_CRITICAL()

/* ReleaseResource */
#define OS_RR_ENTRY()            osTrace(osdTraceReleaseResource)
#define OS_RR_EXIT()             
#define OS_RR_START_CRITICAL()
#define OS_RR_END_CRITICAL()

/*****************/
/* Event control */
/*****************/

/* SetEvent */
#define OS_SE_ENTRY()            osTrace(osdTraceSetEvent)
#define OS_SE_EXIT()             
#define OS_SE_START_CRITICAL()
#define OS_SE_END_CRITICAL()
                           
/* ClearEvent */
#define OS_CE_ENTRY()            osTrace(osdTraceClearEvent)
#define OS_CE_EXIT()             
#define OS_CE_START_CRITICAL()
#define OS_CE_END_CRITICAL()
                           
/* GetEvent */
#define OS_GE_ENTRY()            osTrace(osdTraceGetEvent)
#define OS_GE_EXIT()             
#define OS_GE_START_CRITICAL()
#define OS_GE_END_CRITICAL()
                           
/* WaitEvent */
#define OS_WE_ENTRY()            osTrace(osdTraceWaitEvent)
#define OS_WE_EXIT()             
#define OS_WE_START_CRITICAL()
#define OS_WE_END_CRITICAL()


/**********/
/* Alarms */
/**********/

/* GetAlarmBase */
#define OS_GB_ENTRY()            osTrace(osdTraceGetAlarmBase)
#define OS_GB_EXIT()             
#define OS_GB_START_CRITICAL()
#define OS_GB_END_CRITICAL()
                  
/* SetRelAlarm */
#define OS_SA_ENTRY()            osTrace(osdTraceSetRelAlarm)
#define OS_SA_EXIT()             
#define OS_SA_START_CRITICAL()
#define OS_SA_END_CRITICAL()

/* SetAbsAlarm */
#define OS_SL_ENTRY()            osTrace(osdTraceSetAbsAlarm)
#define OS_SL_EXIT()             
#define OS_SL_START_CRITICAL()
#define OS_SL_END_CRITICAL()

/* CancelAlarm */
#define OS_CA_ENTRY()            osTrace(osdTraceCancelAlarm)
#define OS_CA_EXIT()             
#define OS_CA_START_CRITICAL()
#define OS_CA_END_CRITICAL()

/* GetAlarm */
#define OS_GA_ENTRY()            osTrace(osdTraceGetAlarm)
#define OS_GA_EXIT()             
#define OS_GA_START_CRITICAL()
#define OS_GA_END_CRITICAL()
                           
               
/**************************************/
/* Operating system execution control */
/**************************************/
                 
/* StartOS */
#define OS_ST_ENTRY()            osTrace(osdTraceStartOS)
#define OS_ST_EXIT()             
#define OS_ST_START_CRITICAL()
#define OS_ST_END_CRITICAL()

/* ShutdownOS */
#define OS_SD_ENTRY()            osTrace(osdTraceShutdownOS)
#define OS_SD_EXIT()             
#define OS_SD_START_CRITICAL()
#define OS_SD_END_CRITICAL()

/*****************/
/* Hook routines */
/*****************/

/* ErrorHook */
#define OS_ERHOOK_ENTRY()        osTrace(osdTraceErrorHook) 
#define OS_ERHOOK_EXIT()         
                                                
/* StartupHook */
#define OS_STHOOK_ENTRY()        osTrace(osdTraceStartupHook) 
#define OS_STHOOK_EXIT()         

/* ShutdownHook */               
#define OS_SDHOOK_ENTRY()        osTrace(osdTraceShutdownHook) 
#define OS_SDHOOK_EXIT()         

/* PreTaskHook */
#define OS_PREHOOK_ENTRY()       osTrace(osdTracePreTaskHook) 
#define OS_PREHOOK_EXIT()        

/* PostTaskHook */
#define OS_POSTHOOK_ENTRY()      osTrace(osdTracePostTaskHook) 
#define OS_POSTHOOK_EXIT()       

/*********************/
/* OSEK COM messages */
/*********************/

/* StartCOM */
#define OS_SC_ENTRY()             osTrace(osdTraceStartCOM)  
#define OS_SC_EXIT()            
#define OS_SC_START_CRITICAL()
#define OS_SC_END_CRITICAL()
                           
/* StopCOM */
#define OS_TC_ENTRY()             osTrace(osdTraceStopCOM)
#define OS_TC_EXIT()             
                           
/* SendMessage */
#define OS_SM_ENTRY()            osTrace(osdTraceSendMessage)
#define OS_SM_EXIT()             
#define OS_SM_START_CRITICAL()
#define OS_SM_END_CRITICAL()
                           
/* ReceiveMessage */
#define OS_RM_ENTRY()            osTrace(osdTraceReceiveMessage)
#define OS_RM_EXIT()             
#define OS_RM_START_CRITICAL()
#define OS_RM_END_CRITICAL()

/* GetMessageResource */
#define OS_GG_ENTRY()            osTrace(osdTraceGetMessageResource)
#define OS_GG_EXIT()             
#define OS_GG_START_CRITICAL()
#define OS_GG_END_CRITICAL()
                           
/* ReleaseMessageResource */
#define OS_RG_ENTRY()            osTrace(osdTraceReleaseMessageResource)
#define OS_RG_EXIT()             
#define OS_RG_START_CRITICAL()
#define OS_RG_END_CRITICAL()

/* GetMessageStatus */
#define OS_MS_ENTRY()            osTrace(osdTraceGetMessageStatus)
#define OS_MS_EXIT()             
                           
/* ReadFlag */
#define OS_RF_ENTRY()            osTrace(osdTraceReadFlag)
#define OS_RF_EXIT()             
                           
/* ResetFlag */
#define OS_ZF_ENTRY()            osTrace(osdTraceResetFlag)
#define OS_ZF_EXIT()             
                           
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


/* ########################################################################## */
/* ########################################################################## */
/* ########################################################################## */


#endif /* _TESTMAC2_H */

