/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_AudioManager_Wire.h
*-----------------------------------------------------------------------------
* Module Name:  APP_DEVICE    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_DEVICE
*-----------------------------------------------------------------------------
* $Date: 2010-03-15 12:13:46 +0100 (Mo, 15 Mrz 2010) $
* $Rev: 11558 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/inc/APP_DEVICE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DEVICE_AUDIOMANAGER_WIRE_H
#define _APP_DEVICE_AUDIOMANAGER_WIRE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "PORT.h"
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

#define APP_DEVICE_AM_MUTE_ON_BY_WIRE       CAN_vPortClear(CAN3, TX);\
                                            GIO_vPinWrite(PORT_MUTE_TB, TRUE, GIOB);\
						                    GIO_vPinWrite(PORT_MUTE_TB, FALSE, GIOB);

#define APP_DEVICE_AM_MUTE_OFF_BY_WIRE		CAN_vPortSet(CAN3, TX);\
						                    GIO_vPinWrite(PORT_MUTE_TB, TRUE, GIOB);\
						                    GIO_vPinWrite(PORT_MUTE_TB, FALSE, GIOB);

#define APP_DEVICE_AM_MUTE_ON_BY_WIRE_INIT	CAN_vPortClear(CAN3, TX);\
						                    GIO_vPinModify(GIOB, PORT_MUTE_TB, PortIoOutHigh);\
						                    GIO_vPinModify(GIOB, PORT_MUTE_TB, PortIoOutLow);

#define APP_DEVICE_AM_MUTE_OFF_BY_WIRE_INIT	CAN_vPortSet(CAN3, TX);\
						                    GIO_vPinModify(GIOB, PORT_MUTE_TB, PortIoOutHigh);\
						                    GIO_vPinModify(GIOB, PORT_MUTE_TB, PortIoOutLow);
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#endif /* _APP_DEVICE_AUDIOMANAGER_WIRE_H */

/* End Of File APP_DEVICE_AudioManager_Wire.h */

