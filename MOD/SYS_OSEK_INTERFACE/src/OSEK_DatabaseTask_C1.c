/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     OSEK_DatabaseTask_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_OSEK
*-----------------------------------------------------------------------------
* Description:    file to provide the Init and Task body for the 
*                 DatabaseTask_event.
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 13:30:21 +0100 (Di, 02 Nov 2010) $
* $Rev: 16028 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_DatabaseTask_C1.c $
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
#include "EEPROM.h"
#include "OSEK_DatabaseTask_H1.h"
#include "FBL_Data.h"
#include "APP_COMMON_UTILS.h"
#include "WD_c1.h"
#include "GIO.h"
#include "SPI.h"
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
static EEDeviceControlBlockType EEDBControl;
/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : getEEDBControl                                             */
/** 
    \brief      Register the adress on the structure for EEcontrol for the
                database task

    \return     EEDeviceControlBlockType*

   */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType* getEEDBControl(void)
{
    return &EEDBControl;
}

/*----------------------------------------------------------------------------*/
/* Function    : InitDatabaseTask                                             */
/** 
    \brief      Callback function for initialization of OsDatabaseTask_event
  
    \brief      Description:
				Interface function to initialize the OsDatabaseTask_event,
				this function will be only called one time during first
                activation of OsDatabaseTask_event

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitDatabaseTask(void)
{
    SuccessFailType sF = SUCCESS;
    getEEDBControl()->priority = EEPromHIGH_PRIORITY;
    sF = EEPROM_sfInit(getEEDBControl());
    
    //Set hardware Pin for performance measurement
    SPI_vPinModify(SPI5, SpiSomiPin, PortIoOutHigh);

	(void)SetRelAlarm(CTA_WD, MSEC(2), MSEC(20));
    
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : DatabaseTask_event                                       */
/** 
    \brief      Callback function for OsDatabaseTask_event
  
    \brief      Description:
                Interface function from the OsDatabaseTask_event this
				function will be activated by event and can be restarted by
				the task self with an static alarm (CTA_DATABASE) which set
				an event

    \return     void
   */
/*----------------------------------------------------------------------------*/
void DatabaseTask_event(void)
{
    EventMaskType events;
    
	/* wait for event ... */
	(void)WaitEvent(evDatabaseCyclicEepromWrite |
	                evTriggerWD
	               );
	
	(void)GetEvent(OsDatabaseTask_event, &events);
	
	if (events & evDatabaseCyclicEepromWrite)
    {
        EEPROM_vManager();
	    (void)ClearEvent(evDatabaseCyclicEepromWrite);
    }
    
    if (events & evTriggerWD)
    {
        (void)WD_u8ToggleWD();
        (void)ClearEvent(evTriggerWD);
    }
    return;
}
/* End Of File OSEK_DatabaseTask_C1.c */
