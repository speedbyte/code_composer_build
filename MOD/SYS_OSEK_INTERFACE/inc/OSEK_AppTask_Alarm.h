/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   OSEK_AppTask_Alarm.h
*-----------------------------------------------------------------------------
* Module Name:  MOD_OSEK
*-----------------------------------------------------------------------------
* Description:  Interface header for the AppTask_event_100ms
*-----------------------------------------------------------------------------
* $Date: 2009-03-17 10:33:33 +0100 (Di, 17 Mrz 2009) $
* $Rev: 5312 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/inc/OSEK_AppTask_H1.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APPTASK_ALARM
#define _APPTASK_ALARM

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define CTA_APP_CYCLIC_TIME 100

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef  void (*AppTaskAlarmCbType)( void );

typedef enum
{
    APP_TASK_ALARM_MDI_UNLOCK_AUDIO,
    APP_TASK_ALARM_MDI_PLAYSTATE,
    APP_TASK_ALARM_MDI_PGMT_REQUEST,
    APP_TASK_ALARM_MDI_STARTUP_DELAY,
    APP_TASK_ALARM_MDI_DGED_REQUEST,
    APP_TASK_ALARM_MDI_DGPS_REQUEST,
    APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT,
    APP_TASK_ALARM_MDI_DELAY_PLAY_COMMAND_AFTER_CALL,
    APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
    APP_TASK_ALARM_APP_DEVICE_RELEASE_MUTE,
    APP_TASK_ALARM_APP_PHONE_BAP_RESUMECALL,
    APP_TASK_ALARM_APP_PHONE_BAP_DIALNUMBER,
    APP_TASK_ALARM_APP_PHONE_BAP_ACCEPTCALL,
    APP_TASK_ALARM_APP_PHONE_BAP_HANGUPCALL,
    APP_TASK_ALARM_APP_PHONE_BAP_MPCALLHOLDACCEPTWAITINGCALL,
    APP_TASK_ALARM_APP_PHONE_BAP_MPSWAP,
    APP_TASK_ALARM_APP_PHONE_BAP_CALLHOLD,
    APP_TASK_ALARM_APP_PHONE_BAP_CCJOIN,
    APP_TASK_ALARM_APP_PHONE_BAP_MPRELEASEACTIVECALLACCEPTWAITINGCALL,
    APP_TASK_ALARM_APP_PHONE_BAP_DELAY_USER_ACTIVATION,
    APP_TASK_ALARM_KPB_STARTASGSUPERVISIONDELAYTIME,
    APP_TASK_ALARM_ERM_OPERATER_GET_DELAY,
    APP_TASK_ALARM_ERM_SHUTDOWN_PAUSE_RESPONSE_TIMEOUT,//RT14519

    /* max number of alarm */
    APP_TASK_ALARM_MAX

}APP_TASK_enAlarmType;


typedef struct
{
    AppTaskAlarmCbType cb;
    TickType expires;
}AppTaskAlarmEntryType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
void _AppTask_vInitAlarm();
void _AppTask_vHandleAlarm();
void AppTask_vSetAlarm(APP_TASK_enAlarmType alarmType, TickType ms, AppTaskAlarmCbType cb);
void AppTask_vClearAlarm(APP_TASK_enAlarmType alarmType);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/





#endif /* _APPTASK_ALARM */

/* End Of File OSEK_AppTask_Alarm.h */
