/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_MDI_ERM.h
*-----------------------------------------------------------------------------
* Module Name:  APP_MDI    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_MDI
*-----------------------------------------------------------------------------
* $Date: 2010-03-19 16:00:49 +0100 (Fr, 19 Mrz 2010) $
* $Rev: 11682 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/inc/APP_MDI_ERM.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_MDI_ERM_H
#define _APP_MDI_ERM_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_MDI.h"
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

/** 
\brief	description of the interface IAPP_MDI_ERM_INTERNAL
*/
//#ifdef IAPP_MDI_ERM_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_ERM_INTERNAL) */

/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_vHandleErmATResultEvent                              */
/** 
    \brief      handle event from ERM when AT Parser sent OK 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK \n

    \return     void 
   
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_vHandleErmATResultEvent(APP_MDI_enResult result);


/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_ERM_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM to Update e.g. FSG OperationState
  
    \brief      Description:\n
                handle State change of ERM to Update e.g. FSG OperationState \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_ERM_vHandleErmStateChange(ERMStateType newState);


/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_ERM_vHandleErmNotification                           */
/** 
    \brief      handle State change of ERM to Update e.g. FSG OperationState
  
    \brief      Description:\n
                handle State change of ERM to Update e.g. FSG OperationState \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_ERM_vHandleErmNotification(const ERMNotificationType * pNotification);

//#endif /* END DECLARE INTERFACE(IAPP_MDI_ERM_INTERNAL) */


#endif /* _APP_PHONE_ERM_H */

/* End Of File APP_PHONE_ERM.h */

