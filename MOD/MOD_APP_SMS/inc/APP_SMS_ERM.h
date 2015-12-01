/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_SMS_ERM.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SMS    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_SMS
*-----------------------------------------------------------------------------
* $Date: 2010-03-19 16:00:11 +0100 (Fr, 19 Mrz 2010) $
* $Rev: 11681 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SMS/trunk/inc/APP_SMS_ERM.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SMS_ERM_H
#define _APP_SMS_ERM_H

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
#include "APP_SMS_CI.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
#ifndef IAPP_SMS_INTERNAL
#define IAPP_SMS_INTERNAL
#endif
/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
//#ifdef IAPP_SMS_ERM_EXTERNAL /* BEGIN DECLARE INTERFACE(IAPP_SMS_ERM_EXTERNAL) */
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleErmATOk                                   */
/** 
    \brief      handle event from ERM when AT Parser sent OK 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleErmATOk(void);

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleErmATError                                   */
/** 
    \brief      handle event from ERM when AT Parser sent ERROR 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT ERROR \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleErmATError(void);

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM to Update e.g. FSG OperationState
  
    \brief      Description:\n
                handle State change of ERM to Update e.g. FSG OperationState \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleErmStateChange(ERMStateType newState);


void APP_SMS_vOnErmNotification(const ERMNotificationType * pNotification);
void APP_SMS_vHandleErmAtResult(APP_SMS_enResult result);

//#endif /* END DECLARE INTERFACE(IAPP_SMS_ERM_EXTERNAL) */


#ifdef IAPP_SMS_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_SMS_INTERNAL) */

void _APP_SMS_vOnSendConcSmsResult(APP_SMS_enResult result);
void _APP_SMS_vOnReadSmsResult(APP_SMS_enResult result);
void _APP_SMS_vOnReadSmsPreviewResult(APP_SMS_enResult result);
void _APP_SMS_vOnSmsInitResult(APP_SMS_enResult result);
void _APP_SMS_vInitSmsContext(void);

#endif /* END DECLARE INTERFACE(IAPP_SMS_INTERNAL) */

void APP_SMS_ERM_vHandleErmStateChange(ERMStateType newState);

#endif /* _APP_SMS_ERM_H */

/* End Of File APP_SMS_ERM.h */

