/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   OSEK_AppTask_H1.h
*-----------------------------------------------------------------------------
* Module Name:  MOD_OSEK
*-----------------------------------------------------------------------------
* Description:  Interface header for the AppTask_event_100ms
*-----------------------------------------------------------------------------
* $Date: 2010-03-12 13:41:45 +0100 (Fr, 12 Mrz 2010) $
* $Rev: 11511 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/inc/OSEK_AppTask_H1.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APPTASK_H1
#define _APPTASK_H1

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM.h"
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
/* Function    : InitAppTask                                                  */
/** 
    \brief      Callback function for initialization of OsAppTask_event_100ms
  
    \brief      Description:\n
                Interface function to initialize the OsAppTask_event_100ms, this \n
				function will be only called one time during first activation \n
				of OsAppTask_event_100ms \n

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitAppTask(void);


/*----------------------------------------------------------------------------*/
/* Function    : AppTask_event_100ms                                                  */
/** 
    \brief      Callback function for OsAppTask_event_100ms
  
    \brief      Description:\n
                Interface function from the OsAppTask_event_100ms this function \n
				will be activated by event and can be restarted by the task \n
				self with an static alarm (CTA_APP) which set an event\n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void AppTask_event_100ms(void);

/*----------------------------------------------------------------------------*/
/* Function    : getEEAppTaskControl                                          */
/** 
    \brief      Description:\n
                Register the adress on the structure for getEEAppTaskControl\n
                for the OSEK_AppTask task\n

    \return     EEDeviceControlBlockType*
   */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType* getEEAppTaskControl();


#endif /* _APPTASK_H1 */

/* End Of File OSEK_AppTask_H1.h */
