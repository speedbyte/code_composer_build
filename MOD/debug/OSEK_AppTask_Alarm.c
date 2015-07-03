/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     OSEK_AppTask_Alarm.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_OSEK
*-----------------------------------------------------------------------------
* Description:    provides the APP_TASK alarm callback handling
*-----------------------------------------------------------------------------
* $Date: 2009-11-20 16:20:09 +0100 (Fr, 20 Nov 2009) $
* $Rev: 9948 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_AppTask_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "OSEK_AppTask_Alarm.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Global const Definitions                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of private Methods                                              */
/*----------------------------------------------------------------------------*/
static void _AppTask_vStartAlarm();
/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/
static AppTaskAlarmEntryType alarmTable[APP_TASK_ALARM_MAX];

/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : AppTask_vInitAlarm                                           */
/** 
    \brief      
  
    \return     void
   */
/*----------------------------------------------------------------------------*/
void _AppTask_vInitAlarm()
{
    memset(&alarmTable[0], 0, sizeof(alarmTable));
}


/*----------------------------------------------------------------------------*/
/* Function    : AppTask_vHandleEvent                                         */
/** 
    \brief      
  
    \return     void
   */
/*----------------------------------------------------------------------------*/
void _AppTask_vHandleAlarm()
{
    Uint16Type i = 0;

    for( i=0; i<APP_TASK_ALARM_MAX; i++)
    {
        if( alarmTable[i].expires > 0 &&
            alarmTable[i].expires <= osGetSystemCounter())
        {
            alarmTable[i].expires = 0;
            alarmTable[i].cb();

#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf( UHV_TRACE, UHV_APP_COMMON, 
	                            "APP_TASK: call %d", 
                                i);
#endif

        }
    } 

    _AppTask_vStartAlarm();
}

/*----------------------------------------------------------------------------*/
/* Function    : AppTask_vStartAlarm                                          */
/** 
    \brief      
  
    \return     void
   */
/*----------------------------------------------------------------------------*/
void _AppTask_vStartAlarm()
{
    TickType curTicks = osGetSystemCounter();
    TickType timerTicks = 0;
    Uint16Type i = 0;

    if( GetAlarm(CTA_APP, &timerTicks) == E_OK )
    {
        /* the alarm is running, so cancel it */
        (void)CancelAlarm(CTA_APP);
    }
    else
    {
        /* no alarm is running, set to default */
        timerTicks = MSEC(CTA_APP_CYCLIC_TIME);
    }

    for( i=0; i<APP_TASK_ALARM_MAX; i++)
    {
        if( (alarmTable[i].expires - curTicks) < timerTicks)
        {
            timerTicks = alarmTable[i].expires - curTicks;

#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf( UHV_TRACE, UHV_APP_COMMON, 
	                            "APP_TASK: StartAlarm %d,%d", 
                                timerTicks, i);
#endif

        }
    } 
    

    (void)SetRelAlarm(CTA_APP, timerTicks, 0);
}

/*----------------------------------------------------------------------------*/
/* Function    : AppTask_vSetAlarm                                            */
/** 
    \brief      
  
    \param      alarmType
                APP_TASK_enAlarmType

    \param      ms
                TickType

    \param      cb
                void (*)(void)

    \return     void
   */
/*----------------------------------------------------------------------------*/
void AppTask_vSetAlarm(APP_TASK_enAlarmType alarmType, TickType ms, AppTaskAlarmCbType cb)
{
    if( (alarmType < APP_TASK_ALARM_MAX) && (cb != NULL) )
    {
        alarmTable[alarmType].expires = osGetSystemCounter() + ms;
        alarmTable[alarmType].cb = cb;

#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf( UHV_TRACE, UHV_APP_COMMON, 
	                        "APP_TASK: SetAlarm %d, %d, %d", 
                            alarmType,
                            ms,
                            alarmTable[alarmType].expires);
#endif

    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf( UHV_ERROR, UHV_APP_COMMON, 
	                        "APP_TASK: SetAlarm failed %d", 
                            alarmType);
#endif


    }

    _AppTask_vStartAlarm();
}


/*----------------------------------------------------------------------------*/
/* Function    : AppTask_vClearAlarm                                          */
/** 
    \brief      
  
    \param      alarmType
                APP_TASK_enAlarmType

    \return     void
   */
/*----------------------------------------------------------------------------*/
void AppTask_vClearAlarm(APP_TASK_enAlarmType alarmType)
{
    if( alarmType < APP_TASK_ALARM_MAX )
    {
        alarmTable[alarmType].expires = 0;

#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf( UHV_TRACE, UHV_APP_COMMON, 
	                        "APP_TASK: ClearAlarm %d", 
                            alarmType);
#endif

    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf( UHV_ERROR, UHV_APP_COMMON, 
	                        "APP_TASK: ClearAlarm failed %d", 
                            alarmType);
#endif

    }
}


/* End Of File OSEK_AppTask_Alarm.c */
