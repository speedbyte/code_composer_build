/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name: 	OSEK_TaskHandler_C1.c
*-----------------------------------------------------------------------------
* Module Name:	MOD_OSEK
*-----------------------------------------------------------------------------
* Description:	file to handle the OSEK Task's and supported the callback 
* 				functions
*-----------------------------------------------------------------------------
* $Date$
* $Rev$
* $Author$
* $URL$
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
#include "Common.h"
#include "OSEK_HI.h"
#include "OSEK_AppTask_H1.h"
#include "OSEK_DatabaseTask_H1.h"
#include "OSEK_DriverTaskCyclic_H1.h"
//#include "OSEK_InitTask_H1.h"
#include "OSEK_HostTask_H1.h"
#include "OSEK_IdleTask_H1.h"
#include "OSEK_MonitorTask_H1.h"
#include "OSEK_SystemTask_H1.h"

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

/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	the OSEK Task is initialized
*/
#define	OSEK_TASK_INITIALIZED		1
/** 
\brief	the OSEK Task is not initialized
*/
#define OSEK_TASK_UNINITIALIZED		0

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/
static Uint8Type gHostTaskFirstActivated		 = OSEK_TASK_UNINITIALIZED;
static Uint8Type gMonitorTaskFirstActivated		 = OSEK_TASK_UNINITIALIZED;
static Uint8Type gDriverTaskCyclicFirstActivated = OSEK_TASK_UNINITIALIZED;
static Uint8Type gAppTaskFirstActivated			 = OSEK_TASK_UNINITIALIZED;
static Uint8Type gDatabaseTaskFirstActivated	 = OSEK_TASK_UNINITIALIZED;
static Uint8Type gSystemTaskFirstActivated		 = OSEK_TASK_UNINITIALIZED;

/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsHostTask_10ms)                                        */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered cyclic \n

    \param      OS Task name
                OsHostTask_10ms
    \return     non
   */
/*----------------------------------------------------------------------------*/
TASK(OsHostTask_10ms)
{
	if(OSEK_TASK_UNINITIALIZED == gHostTaskFirstActivated)
	{
		/* RelAlarm for cyclic activation for HostTask_10ms with an event
		 * can be configured by OSEK_HI.h */ 
		(void)SetRelAlarm(CTA_HOST, MSEC(OSEK_HOSTTASK_ALARM_CYCLIC_TIME_MS), MSEC(OSEK_HOSTTASK_ALARM_DELAY_TIME_MS));
			 
		/* callback function for init the Variables of HostTask_10ms, it's 
		 * supported in file OSEK_HostTask_C1.c */
		(void)InitHostTask();

		gHostTaskFirstActivated = OSEK_TASK_INITIALIZED;
	}
	
	for(;;)
	{
	  /* callback function for the cyclic called user part, it's 
	   * supported in file OSEK_HostTask_C1.c */
	  HostTask_10ms();
	}
} /* END OF TASK(OsHostTask_10ms) */


/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsMonitorTask_50ms)                                     */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered cyclic \n

    \param      OS Task name
                OsMonitorTask_50ms
    \return     non
   */
/*----------------------------------------------------------------------------*/
TASK(OsMonitorTask_50ms)
{
	if(OSEK_TASK_UNINITIALIZED == gMonitorTaskFirstActivated)
	{
		/* RelAlarm for cyclic activation for MonitorTask_50ms with an event
		 * can be configured by OSEK_HI.h */ 
		(void)SetRelAlarm(CTA_MONITOR, MSEC(OSEK_MONITORTASK_ALARM_CYCLIC_TIME_MS), MSEC(OSEK_MONITORTASK_ALARM_DELAY_TIME_MS));

		/* callback function for init the Variables of MonitorTask_50ms, it's 
		 * supported in file OSEK_MonitorTask_C1.c */
		(void)InitMonitorTask();

		gMonitorTaskFirstActivated = OSEK_TASK_INITIALIZED;
	}
	
	for(;;)
	{
	  /* callback function for the cyclic called user part, it's 
	   * supported in file OSEK_MonitorTask_C1.c */
	  MonitorTask_50ms();
	}
} /* END OF TASK(OsMonitorTask_50ms) */


/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsDriverTask_1ms)                                     */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered cyclic \n

    \param      OS Task name
                OsDriverTask_1ms
    \return     non
   */
/*----------------------------------------------------------------------------*/
TASK(OsDriverTask_1ms)
{
	if(OSEK_TASK_UNINITIALIZED == gDriverTaskCyclicFirstActivated)
	{
		/* RelAlarm for cyclic activation for DriverTask_1ms with an event
		 * can be configured by OSEK_HI.h */ 
		(void)SetRelAlarm(CTA_DRIVER, MSEC(OSEK_DRIVERTASKCYCLIC_ALARM_CYCLIC_TIME_MS), MSEC(OSEK_DRIVERTASKCYCLIC_ALARM_DELAY_TIME_MS));

		/* callback function for init the Variables of DriverTask_1ms, it's 
		 * supported in file OSEK_DriverTaskCyclic_C1.c */
		(void)InitDriverTaskCyclic();

		gDriverTaskCyclicFirstActivated = OSEK_TASK_INITIALIZED;
	}
	
	for(;;)
	{
	  /* callback function for the cyclic called user part, it's 
	   * supported in file OSEK_DriverTaskCyclic_C1.c */
	  DriverTask_1ms();
	}
} /* END OF TASK(OsDriverTask_1ms) */


/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsAppTask_event_100ms)                                    */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered cyclic \n

    \param      OS Task name
                OsAppTask_event_100ms
    \return     non
   */
/*----------------------------------------------------------------------------*/
TASK(OsAppTask_event_100ms)
{
	if(OSEK_TASK_UNINITIALIZED == gAppTaskFirstActivated)
	{
		/* callback function for init the Variables of AppTask_event_100ms, it's
		 * supported in file OSEK_AppTask_C1.c */
		(void)InitAppTask();

		gAppTaskFirstActivated = OSEK_TASK_INITIALIZED;
	}
	
	for(;;)
	{
	  /* callback function for the cyclic called user part, it's 
	   * supported in file OSEK_AppTask_C1.c */
	  AppTask_event_100ms();
	}
} /* END OF TASK(OsAppTask_event_100ms) */


/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsDatabaseTask_event)                               */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered by event \n

    \param      OS Task name
                OsDatabaseTask_event
    \return     non
   */
/*----------------------------------------------------------------------------*/
TASK(OsDatabaseTask_event)
{
	if(OSEK_TASK_UNINITIALIZED == gDatabaseTaskFirstActivated)
	{
		/* callback function for init the Variables of DatabaseTask_event, it's
		 * supported in file OSEK_DatabaseTask_C1.c */
		(void)InitDatabaseTask();

		gDatabaseTaskFirstActivated = OSEK_TASK_INITIALIZED;
	}
	
	for(;;)
	{
	  /* callback function for the cyclic called user part, it's 
	   * supported in file OSEK_DatabaseTask_C1.c */
	  DatabaseTask_event();
	}
} /* END OF TASK(OsDatabaseTask_event) */


/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsSystemTask_event)                                     */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered by event \n

    \param      OS Task name
                OsSystemTask_event
    \return     non
   */
/*----------------------------------------------------------------------------*/
TASK(OsSystemTask_event)
{
	if(OSEK_TASK_UNINITIALIZED == gSystemTaskFirstActivated)
	{
		/* callback function for init the Variables of SystemTask_event, it's 
		 * supported in file OSEK_SystemTask_C1.c */
		(void)InitSystemTask();

		gSystemTaskFirstActivated = OSEK_TASK_INITIALIZED;
	}
	
	for(;;)
	{
	  /* callback function for the cyclic called user part, it's 
	   * supported in file OSEK_SystemTask_C1.c */
	  SystemTask_event();
	}
} /* END OF TASK(OsSystemTask_event) */



/*----------------------------------------------------------------------------*/
/* Function    : TASK(OsIdleTask_bg)                                          */
/** 
    \brief      OSEK Task
  
    \brief      Description:\n
                OSEK Task for CAN function which must triggered by event \n

    \param      OS Task name
                OsIdleTask_bg
    \return     non
   */
/*----------------------------------------------------------------------------*/
//TASK(OsIdleTask_bg)
//{
//	if(OSEK_TASK_UNINITIALIZED == gIdleTaskFirstActivated)
//	{
//		/* callback function for init the Variables of IdleTask_bg, it's 
//		 * supported in file OSEK_IdleTask_C1.c */
//		(void)InitIdleTask();
//
//		gIdleTaskFirstActivated = OSEK_TASK_INITIALIZED;
//	}
//	
//	for(;;)
//	{
//	  /* callback function for the cyclic called user part, it's 
//	   * supported in file OSEK_IdleTask_C1.c */
//	  IdleTask_bg();
//	}
//} /* END OF TASK(OsIdleTask_bg) */

/* End Of File OSEK_TaskHandler_C1.c */
