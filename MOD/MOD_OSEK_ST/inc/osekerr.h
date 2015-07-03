/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekerr.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.16
|
|  Description: definitions of all errors for assertions and OSError
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2007 Vector Informatik GmbH               All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEKERR_H
#define _OSEKERR_H

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

/* All errors of API services */
#define E_OS_ACCESS     1
#define E_OS_CALLEVEL   2
#define E_OS_ID         3
#define E_OS_LIMIT      4
#define E_OS_NOFUNC     5
#define E_OS_RESOURCE   6
#define E_OS_STATE      7
#define E_OS_VALUE      8

#define E_OS_SYS_ASSERTION         9
#define E_OS_SYS_ABORT            10
#define E_OS_SYS_DIS_INT          11
#define E_OS_SYS_API_ERROR        12
#define E_OS_SYS_ALARM_MANAGEMENT 13
#define E_OS_SYS_WARNING          14


/*
 * Syntax of errornumbers:
 * 0xgfee
 *   ||+--- consecutive error number
 *   |+---- number of function in the function group
 *   +----- number of function group
 */

/* Group: Task management 1 */
/*    function ActivateTask:           AT: 1 */
/*    function TerminateTask:          TT: 2 */
/*    function ChainTask:              HT: 3 */
/*    function Schedule:               SH: 4 */
/*    function GetTaskState:           GS: 5 */
/*    function GetTaskID:              GI: 6 */
/*    function osMissingTerminateError MT: 7 */

#define osdErrATWrongTaskID           0x1101
#define osdErrATWrongTaskPrio         0x1102
#define osdErrATMultipleActivation    0x1103
#define osdErrATIntAPIDisabled        0x1104
#define osdErrATAlarmMultipleActivation 0x1105

#define osdErrTTDisabledInterrupts    0x1201
#define osdErrTTResourcesOccupied     0x1202
#define osdErrTTNotActivated          0x1203
#define osdErrTTOnInterruptLevel      0x1204
#define osdErrTTNoImmediateTaskSwitch 0x1205

#define osdErrHTInterruptsDisabled    0x1301
#define osdErrHTResourcesOccupied     0x1302
#define osdErrHTWrongTaskID           0x1303
#define osdErrHTNotActivated          0x1304
#define osdErrHTMultipleActivation    0x1305
#define osdErrHTOnInterruptLevel      0x1306
#define osdErrHTWrongTaskPrio         0x1307
#define osdErrHTNoImmediateTaskSwitch 0x1308
                                        
#define osdErrSHInterruptsDisabled    0x1401
#define osdErrSHOnInterruptLevel      0x1402
#define osdErrSHScheduleNotAllowed    0x1403
#define osdErrSHInContextOverlayProc  0x1404
#define osdErrSHResourcesOccupied     0x1405
                                        
#define osdErrGSWrongTaskID           0x1501
#define osdErrGSIntAPIDisabled        0x1502

#define osdErrGIIntAPIDisabled        0x1601

#define osdErrMTMissingTerminateTask  0x1701


/* Group: Interrupt handling 2 */
/*    function EnableAllInterrupts        EA: 4 */
/*    function DisableAllInterrupts       DA: 5 */
/*    function ResumeOSInterrupts         RI: 6 */
/*    function SuspendOSInterrupts        SI: 7 */
/*    function osUnhandledException       UE: 8 */
/*    function osSaveDisableLevelNested   SD: 9 */
/*    function osRestoreEnableLevelNested RE: A */
/*    function osSaveDisableLevelGlobal   SG: B */
/*    function osRestoreEnableLevelGlobal RG: C */
/*    function ResumeAllInterrupts        RA: D */
/*    function SuspendAllInterrupts       SA: E */

#define osdErrEAIntAPIWrongSequence   0x2401
#define osdErrDAIntAPIDisabled        0x2501

#define osdErrUEUnhandledException    0x2801

#define osdErrSDWrongCounter          0x2901
#define osdErrREWrongCounter          0x2A01

#define osdErrSGWrongCounter          0x2B01
#define osdErrRGWrongCounter          0x2C01

/* Group: Resource management 3 */
/*    function GetResource            GR:  1 */
/*    function ReleaseResource        RR:  2 */

#define osdErrGRWrongResourceID       0x3101
#define osdErrGRPriorityOccupied      0x3102
#define osdErrGRResourceOccupied      0x3103
#define osdErrGRNoAccessRights        0x3104
#define osdErrGRWrongPrio             0x3105
#define osdErrGRIntAPIDisabled        0x3106

#define osdErrRRWrongResourceID       0x3201
#define osdErrRRCeilingPriorityNotSet 0x3202
#define osdErrRRWrongTask             0x3203
#define osdErrRRWrongPrio             0x3204
#define osdErrRRNotOccupied           0x3206
#define osdErrRRWrongSequence         0x3207
#define osdErrRRIntAPIDisabled        0x3208

/* Group: Event control 4 */
/*    function SetEvent               SE:  1 */
/*    function ClearEvent             CE:  2 */
/*    function GetEvent               GE:  3 */
/*    function WaitEvent              WE:  4 */

#define osdErrSEWrongTaskID           0x4101
#define osdErrSENotExtendedTask       0x4102
#define osdErrSETaskSuspended         0x4103
#define osdErrSEWrongTaskPrio         0x4104
#define osdErrSEIntAPIDisabled        0x4105

#define osdErrCENotExtendedTask       0x4201
#define osdErrCEOnInterruptLevel      0x4202
#define osdErrCEIntAPIDisabled        0x4203

#define osdErrGEWrongTaskID           0x4301
#define osdErrGENotExtendedTask       0x4302
#define osdErrGETaskSuspended         0x4303
#define osdErrGEIntAPIDisabled        0x4304

#define osdErrWENotExtendedTask       0x4401
#define osdErrWEResourcesOccupied     0x4402
#define osdErrWEInterruptsDisabled    0x4403
#define osdErrWEOnInterruptLevel      0x4404
#define osdErrWEInContextOverlayProc  0x4405


/* Group: Alarms 5 */
/*    function GetAlarmBase           GB:  1 */
/*    function GetAlarm               GA:  2 */
/*    function SetRelAlarm            SA:  3 */
/*    function SetAbsAlarm            SL:  4 */
/*    function CancelAlarm            CA:  5 */
/*    function osWorkAlarms           WA:  6 */

#define osdErrGBWrongAlarmID          0x5101
#define osdErrGBIntAPIDisabled        0x5102

#define osdErrGAWrongAlarmID          0x5201
#define osdErrGANotActive             0x5202
#define osdErrGAIntAPIDisabled        0x5203

#define osdErrSAWrongAlarmID          0x5301
#define osdErrSAAlreadyActive         0x5302
#define osdErrSAWrongCycle            0x5303
#define osdErrSAWrongDelta            0x5304
#define osdErrSAIntAPIDisabled        0x5305
                                        
#define osdErrSLWrongAlarmID          0x5401
#define osdErrSLAlreadyActive         0x5402
#define osdErrSLWrongCycle            0x5403
#define osdErrSLWrongStart            0x5404
#define osdErrSLIntAPIDisabled        0x5405

#define osdErrCAWrongAlarmID          0x5501
#define osdErrCANotActive             0x5502
#define osdErrCAIntAPIDisabled        0x5503
#define osdErrCAAlarmInternal         0x5504


/* Group: Operating system execution control 6 */
/*    function osCheckStackOverflow   SO:  1 */
/*    function osSchedulePrio         SP:  2 */
/*    function osGetStackUsage        SU:  3 */
/*    function osCheckLibraryVers..   CL:  4 */
/*    function osErrorHook            EH:  5 */
/*    function StartOS                ST:  6 */

#define osdErrSOStackOverflow         0x6101

#define osdErrSPInterruptsEnabled     0x6201

#define osdErrSUWrongTaskID           0x6301

#define osdErrCLWrongLibrary          0x6401

#define osdErrEHInterruptsEnabled     0x6501

#define osdErrSTMemoryError           0x6601
#define osdErrSTNoImmediateTaskSwitch 0x6602


/* Group: Platform specific error codes A */
/* KB begin osekHwErrorCodes */
#define     osdErrYOStackOverflow 0xA101U
#define     osdErrHWDefVect       0xA201U


/* KB end osekHwErrorCodes */

/* definitions to save the parameters in global variables before calling the service */
#if osdParameterAccessMacros && osdErrorHook
   #define osSaveActivateTask_TaskID(x)   (osAPIParam1.osTaskType = (x));
   #define osSaveChainTask_TaskID(x)      (osAPIParam1.osTaskType = (x));
   #define osSaveGetTaskID_TaskID(x)     (osAPIParam1.osTaskRefType = (x));
   #define osSaveGetTaskState_TaskID(x)   (osAPIParam1.osTaskType = (x));
   #define osSaveGetTaskState_State(y)    (osAPIParam2.osTaskStateRefType = (y));
   #define osSaveGetResource_ResID(x)     (osAPIParam1.osResourceType = (x));
   #define osSaveReleaseResource_ResID(x) (osAPIParam1.osResourceType = (x));
   #define osSaveSetEvent_TaskID(x)       (osAPIParam1.osTaskType = (x));
   #define osSaveSetEvent_Mask(y)         (osAPIParam2.osEventMaskType = (y));
   #define osSaveGetEvent_TaskID(x)       (osAPIParam1.osTaskType = (x));
   #define osSaveGetEvent_Event(y)        (osAPIParam2.osEventMaskRefType = (y));
   #define osSaveClearEvent_Mask(x)       (osAPIParam1.osEventMaskType = (x));
   #define osSaveWaitEvent_Mask(x)        (osAPIParam1.osEventMaskType = (x));
   #define osSaveGetAlarmBase_AlarmID(x)  (osAPIParam1.osAlarmType = (x));
   #define osSaveGetAlarmBase_Info(y)     (osAPIParam2.osAlarmBaseRefType = (y));
   #define osSaveSetRelAlarm_AlarmID(x)   (osAPIParam1.osAlarmType = (x)) ;
   #define osSaveSetRelAlarm_increment(y) (osAPIParam2.osTickType = (y));
   #define osSaveSetRelAlarm_cycle(z)     (osAPIParam3 = (z));
   #define osSaveSetAbsAlarm_AlarmID(x)   (osAPIParam1.osAlarmType = (x));
   #define osSaveSetAbsAlarm_start(y)     (osAPIParam2.osTickType = (y));
   #define osSaveSetAbsAlarm_cycle(z)     (osAPIParam3 = (z));
   #define osSaveCancelAlarm_AlarmID(x)   (osAPIParam1.osAlarmType = (x));
   #define osSaveGetAlarm_AlarmID(x)      (osAPIParam1.osAlarmType = (x));
   #define osSaveGetAlarm_Tick(y)         (osAPIParam2.osTickRefType = (y));

#else
   #define osSaveActivateTask_TaskID(x)
   #define osSaveChainTask_TaskID(x)
   #define osSaveGetTaskID_TaskID(x)
   #define osSaveGetTaskState_TaskID(x)
   #define osSaveGetTaskState_State(y)
   #define osSaveGetResource_ResID(x)
   #define osSaveReleaseResource_ResID(x)
   #define osSaveSetEvent_TaskID(x)
   #define osSaveSetEvent_Mask(y)
   #define osSaveGetEvent_TaskID(x)
   #define osSaveGetEvent_Event(y)
   #define osSaveClearEvent_Mask(x)
   #define osSaveWaitEvent_Mask(x)
   #define osSaveGetAlarmBase_AlarmID(x)
   #define osSaveGetAlarmBase_Info(y)
   #define osSaveSetRelAlarm_AlarmID(x)
   #define osSaveSetRelAlarm_increment(y)
   #define osSaveSetRelAlarm_cycle(z)
   #define osSaveSetAbsAlarm_AlarmID(x)
   #define osSaveSetAbsAlarm_start(y)
   #define osSaveSetAbsAlarm_cycle(z)
   #define osSaveCancelAlarm_AlarmID(x)
   #define osSaveGetAlarm_AlarmID(x)
   #define osSaveGetAlarm_Tick(y)
#endif

 #if osdErrorHook
  #if osdGetServiceIdMacros

   #define OSServiceId_ActivateTask          0x1100
   #define OSServiceId_TerminateTask         0x1200
   #define OSServiceId_ChainTask             0x1300
   #define OSServiceId_Schedule              0x1400
   #define OSServiceId_GetTaskState          0x1500
   #define OSServiceId_GetTaskID             0x1600

   #define OSServiceId_EnableAllInterrupts   0x2400
   #define OSServiceId_DisableAllInterrupts  0x2500
   #define OSServiceId_ResumeOSInterrupts    0x2600
   #define OSServiceId_SuspendOSInterrupts   0x2700
   #define OSServiceId_ResumeAllInterrupts   0x2B00
   #define OSServiceId_SuspendAllInterrupts  0x2C00

   #define OSServiceId_GetResource           0x3100
   #define OSServiceId_ReleaseResource       0x3200

   #define OSServiceId_SetEvent              0x4100
   #define OSServiceId_ClearEvent            0x4200
   #define OSServiceId_GetEvent              0x4300
   #define OSServiceId_WaitEvent             0x4400

   #define OSServiceId_GetAlarmBase          0x5100
   #define OSServiceId_GetAlarm              0x5200
   #define OSServiceId_SetRelAlarm           0x5300
   #define OSServiceId_SetAbsAlarm           0x5400
   #define OSServiceId_CancelAlarm           0x5500


   #define OSErrorGetServiceId()       ((OSServiceIdType) (osLastError & 0xFF00))

  #endif

   #define OSErrorGetosCANError()      osLastError

   #if (STATUS_LEVEL == EXTENDED_STATUS) && (!defined(osdSuppressFilenames) )
    #define OSErrorGetosCANLineNumber() osActiveTaskLineNumber
    #define OSErrorGetosCANModulName()  osActiveTaskModule
   #endif

  #if osdParameterAccessMacros 
   #define OSError_ActivateTask_TaskID()   (osAPIParam1.osTaskType) 
   #define OSError_ChainTask_TaskID()      (osAPIParam1.osTaskType) 
   #define OSError_GetTaskID_TaskID()      (osAPIParam1.osTaskRefType) 
   #define OSError_GetTaskState_TaskID()   (osAPIParam1.osTaskType)
   #define OSError_GetTaskState_State()    (osAPIParam2.osTaskStateRefType)

   #define OSError_GetResource_ResID()     (osAPIParam1.osResourceType) 
   #define OSError_ReleaseResource_ResID() (osAPIParam1.osResourceType) 

   #define OSError_SetEvent_TaskID()       (osAPIParam1.osTaskType) 
   #define OSError_SetEvent_Mask()         (osAPIParam2.osEventMaskType) 
   #define OSError_GetEvent_TaskID()       (osAPIParam1.osTaskType) 
   #define OSError_GetEvent_Event()        (osAPIParam2.osEventMaskRefType) 
   #define OSError_ClearEvent_Mask()       (osAPIParam1.osEventMaskType) 
   #define OSError_WaitEvent_Mask()        (osAPIParam1.osEventMaskType) 

   #define OSError_GetAlarmBase_AlarmID()  (osAPIParam1.osAlarmType)
   #define OSError_GetAlarmBase_Info()     (osAPIParam2.osAlarmBaseRefType) 
   #define OSError_SetRelAlarm_AlarmID()   (osAPIParam1.osAlarmType)
   #define OSError_SetRelAlarm_increment() (osAPIParam2.osTickType) 
   #define OSError_SetRelAlarm_cycle()     ( (TickType) osAPIParam3) 
   #define OSError_SetAbsAlarm_AlarmID()   (osAPIParam1.osAlarmType)
   #define OSError_SetAbsAlarm_start()     (osAPIParam2.osTickType)
   #define OSError_SetAbsAlarm_cycle()     ( (TickType) osAPIParam3)
   #define OSError_CancelAlarm_AlarmID()   (osAPIParam1.osAlarmType)
   #define OSError_GetAlarm_AlarmID()      (osAPIParam1.osAlarmType)
   #define OSError_GetAlarm_Tick()         (osAPIParam2.osTickRefType)

  #endif
 #endif


/*lint -restore re-enable messages*/

#endif /* double include preventer */

/* END OF HEADER osekerr.h */
