/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_AudioManager_CI.h
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

#ifndef _APP_DEVICE_AUDIOMANAGER_CI_H
#define _APP_DEVICE_AUDIOMANAGER_CI_H

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
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

typedef enum 
{
    APP_DEVICE_AM_enCooperativeMute,
    APP_DEVICE_AM_enNoCooperativeMute
} APP_DEVICE_AM_CooperativeMuteType;



typedef struct
{
    void(*callbackGet)(APP_COMMON_enResponseCode);
    void(*callbackRelease)(APP_COMMON_enResponseCode);
    APP_CAN_enSourceIdType          enActiveSource;
    APP_PHONE_CanDataType           sendMuteDataToCan;
    APP_DEVICE_AM_CooperativeMuteType enCooperativeMute;
    Uint16Type                      muteDelay;
    Uint8Type                       radioCanAck;
    Uint8Type                       radioOnOff;
    BooleanType                     waitForPasa;
}AppDeviceAudioManagerType;
#endif /* _APP_DEVICE_AUDIOMANAGER_CI_H */

/* End Of File APP_DEVICE_AudioManager_CI.h */

