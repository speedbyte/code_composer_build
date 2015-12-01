/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   OSEK_SystemTask_H1.h
*-----------------------------------------------------------------------------
* Module Name:  MOD_OSEK
*-----------------------------------------------------------------------------
* Description:  Interface header for the SystemTask_event
*-----------------------------------------------------------------------------
* $Date: 2009-03-17 10:33:33 +0100 (Di, 17 Mrz 2009) $
* $Rev: 5312 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/inc/OSEK_SystemTask_H1.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _SYSTEMTASK_H1
#define _SYSTEMTASK_H1

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
/* Function    : InitSystemTask                                               */
/** 
    \brief      Callback function for initialization of OsSystemTask_event
  
    \brief      Description:\n
				Callback function to initialize the OsSystemTask_event, this \n
				function will be only called one time during first activation \n
				of OsSystemTask_event \n

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitSystemTask(void);


/*----------------------------------------------------------------------------*/
/* Function    : SystemTask_event                                             */
/** 
    \brief      Callback function for OsSystemTask_event
  
    \brief      Description:\n
                Callback function from the OsSystemTask_event this function \n
				will be activated by event \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void SystemTask_event(void);


#endif /* _SYSTEMTASK_H1 */

/* End Of File OSEK_SystemTask_H1.h */
