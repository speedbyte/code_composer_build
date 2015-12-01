/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   OSEK_DatabaseTask_H1.h
*-----------------------------------------------------------------------------
* Module Name:  MOD_OSEK
*-----------------------------------------------------------------------------
* Description:  Interface header for the DatabaseTask_event
*-----------------------------------------------------------------------------
* $Date: 2010-03-12 13:41:45 +0100 (Fr, 12 Mrz 2010) $
* $Rev: 11511 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/inc/OSEK_DatabaseTask_H1.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _DATABASETASK_H1
#define _DATABASETASK_H1

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "EEPROM.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : InitDatabaseTask                                                  */
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
SuccessFailType InitDatabaseTask(void);


/*----------------------------------------------------------------------------*/
/* Function    : DatabaseTask_event                                                  */
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
void DatabaseTask_event(void);


/*----------------------------------------------------------------------------*/
/* Function    : getEEDBControl                                             */
/** 
    \brief      Register the adress on the structure for EEcontrol for the
                database task

    \return     EEDeviceControlBlockType*

   */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType* getEEDBControl(void);


#endif /* _DATABASETASK_H1 */

/* End Of File OSEK_DatabaseTask_H1.h */
