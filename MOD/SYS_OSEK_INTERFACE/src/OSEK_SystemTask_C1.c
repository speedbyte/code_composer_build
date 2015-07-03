/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     OSEK_SystemTask_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_OSEK
*-----------------------------------------------------------------------------
* Description:    file to provide the Init and Task body for the 
*                 SystemTask_event.
*-----------------------------------------------------------------------------
* $Date: 2010-03-12 13:54:09 +0100 (Fr, 12 Mrz 2010) $
* $Rev: 11512 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_SystemTask_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "OSEK_SystemTask_H1.h"
#include "can_inc.h"
#include "SYS_INIT.h"

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

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Function    : InitSystemTask                                               */
/** 
    \brief      Callback function for initialization of OsSystemTask_event
  
    \brief      Description:
				Callback function to initialize the OsSystemTask_event, this
				function will be only called one time during first activation
				of OsSystemTask_event

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitSystemTask(void)
{
    SuccessFailType sF = SUCCESS;

//    (void)ActivateTask(OsHostTask_10ms);      /* activation for HostTask_10ms */
    (void)ActivateTask(OsMonitorTask_50ms);   /* activation for MonitorTask_50ms */
    (void)ActivateTask(OsDriverTask_1ms);     /* activation for DriverTask_1ms */
   
    //sF = SYSINIT_sfSwInit();	/* initialize the Software parts of the System. */
    
    sF = CSHDL_init();
    (void)SetRelAlarm(CTA_SYSTEM, MSEC(20), MSEC(20));

    return(sF);
}

/*----------------------------------------------------------------------------*/
/* Function    : SystemTask_event                                             */
/** 
    \brief      Callback function for OsSystemTask_event
  
    \brief      Description:
                Callback function from the OsSystemTask_event this function
				will be activated by event

    \return     void
   */
/*----------------------------------------------------------------------------*/
void SystemTask_event(void)
{
	/* wait for event ... */
	(void)WaitEvent(evActivateSystemTask_event);
	(void)ClearEvent(evActivateSystemTask_event);
	
	(void)CSHDL_manager();

    return;
}

/* End Of File OSEK_SystemTask_C1.c */
