/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_ERM.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONEBOOK
*-----------------------------------------------------------------------------
* $Date: 2010-03-19 16:00:11 +0100 (Fr, 19 Mrz 2010) $
* $Rev: 11681 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/inc/APP_PHONEBOOK_ERM.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_ERM_H
#define _APP_PHONEBOOK_ERM_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"

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
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

void APP_PHONEBOOK_vHandleErmATError(void);

/** 
\brief	description of the interface IAPP_PHONEBOOK_ERM_INTERNAL
*/
#ifdef IAPP_PHONEBOOK_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_ERM_INTERNAL) */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_vHandleErmATOkEvent                                   */
/** 
    \brief      handle event from ERM when AT Parser sent OK 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK \n

    \return     void 
   
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_vHandleErmATOkEvent(void);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_vHandleErmATErrorEvent                                   */
/** 
    \brief      handle event from ERM when AT Parser sent ERROR 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT ERROR \n

    \return     void 
   
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_vHandleErmATErrorEvent(void);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM to Update e.g. FSG OperationState
  
    \brief      Description:\n
                handle State change of ERM to Update e.g. FSG OperationState \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_ERM_vHandleErmStateChange(ERMStateType newState);

void APP_PHONEBOOK_vOnErmNotification(const ERMNotificationType * pNotification);

#endif /* END DECLARE INTERFACE(IAPP_PHONEBOOK_ERM_INTERNAL) */

void APP_PHONEBOOK_vHandleErmATOk(void);

#endif /* _APP_PHONE_ERM_H */

/* End Of File APP_PHONE_ERM.h */

