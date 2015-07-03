/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_E2P_Backup_R2FS.h
*-----------------------------------------------------------------------------
* Module Name:  APP_DEVICE    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_DEVICE
*-----------------------------------------------------------------------------
* $Date: 2009-12-22 08:15:54 +0100 (Di, 22 Dez 2009) $
* $Rev: 10353 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/inc/APP_DEVICE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DEVICE_E2P_BACKUP_R2FS_H
#define _APP_DEVICE_E2P_BACKUP_R2FS_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "APP_COMMON_Utils.h"
#include "APP_CSHDL.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_DEVICE_CE.h"
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
/* Prototypes of module internal Methods                                           */
/*----------------------------------------------------------------------------*/
 SuccessFailType
_APP_DEVICE_sfHandlePendingResetToFactorySetting(const AppDeviceFactorySettingType *pFactSet);
 SuccessFailType
_APP_DEVICE_sfHandlePendingUpdateBackupAreaEeprom(const AppDeviceBackupAreaEepromType *pBackupAreaEeprom);
 SuccessFailType 
_APP_DEVICE_sfHandlePendingStopRingtoneNonBlock();


void _APP_DEVICE_E2P_Backup_R2FS_vOnErmNotification(const ERMNotificationType * pNotification);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _APP_DEVICE_E2P_BACKUP_R2FS_H */

/* End Of File APP_DEVICE_E2P_Backup_R2FS.h */

