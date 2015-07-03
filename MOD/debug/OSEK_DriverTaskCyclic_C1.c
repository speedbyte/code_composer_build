/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     OSEK_DriverTaskCyclic_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_OSEK
*-----------------------------------------------------------------------------
* Description:    file to provide the Init and Task body for the 
*                 SystemTask_event.
*-----------------------------------------------------------------------------
* $Date: 2010-03-12 13:54:09 +0100 (Fr, 12 Mrz 2010) $
* $Rev: 11512 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_DriverTaskCyclic_C1.c $
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
#include "ADC.h"
#include "PHY_BUT_DRV.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "OSEK_DriverTaskCyclic_H1.h"


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
/* Function    : InitDriverTaskCyclic                                         */
/** 
    \brief      Callback function for initialization of OsDriverTask_1ms
  
    \brief      Description:
				Callback function to initialize the OsDriverTask_1ms,
				this function will be only called one time during first
                activation of OsDriverTask_1ms

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitDriverTaskCyclic(void)
{
    SuccessFailType sF = SUCCESS;
    PHY_BUT_DRV_vStartup();
    (void)SetRelAlarm(CTA_DRIVER_TIMER_10MS, MSEC(1), MSEC(10));

    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : DriverTask_1ms                                               */
/** 
    \brief      Callback function for OsDriverTask_1ms
  
    \brief      Description:
                Callback function from the OsDriverTask_1ms this function
				will be cyclic called all 1ms

    \return     void
   */
/*----------------------------------------------------------------------------*/
void DriverTask_1ms(void)
{
    EventMaskType events;
	
    /* wait for event ... */
	(void)WaitEvent(evActivateDriverTask_1ms |
			evDriverTask_TimerForButtonDetection_10ms);
	(void)ClearEvent(evActivateDriverTask_1ms);
	(void)GetEvent(OsDriverTask_1ms, &events);
    /* call ATParser */
    ATParserTask();
    
    /* call ADC converter */
    (void)ADC_sfConvStart(ADC1);

    /* activation for 3 Button and Cradle Press detection */
    if( events & evDriverTask_TimerForButtonDetection_10ms )
    {
    	(void)ClearEvent(evDriverTask_TimerForButtonDetection_10ms);
        (void)PHY_BUT_DRV_vhandle(enLogiButManDevice3BM); 
        (void)PHY_BUT_DRV_vhandle(enLogiButManDeviceCradle);
    }

    /* call 3BM handler */
  
    return;
}



/* End Of File OSEK_DriverTaskCyclic_C1.c */
