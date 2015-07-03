/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_E2P_Backup_R2FS_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_DEVICE
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP and
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2010-02-23 16:46:53 +0100 (Di, 23 Feb 2010) $
* $Rev: 11251 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/src/APP_DEVICE_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "APP_COMMON_Utils.h"
#include "OSEK_AppTask_H1.h"
#include "APP_SD.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "CSHDL.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_DEVICE.h"
#include "APP_DEVICE_CI.h"
#include "VCR.h"

#include "APP_DEVICE_E2P_Backup_R2FS_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
#define MAX_USER_PROFILE_ENTRY  960
/**
\brief	variable to save the current device function in progress
*/
extern AppDeviceContext gAppDeviceContext;

const Uint8Type _APP_DEVICE_Default_UserTable[MAX_USER_PROFILE_ENTRY] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const char VwBt[AT_DEVICE_NAME_MAX_LEN + 1] = "VW PHONE";
const char SkodaBt[AT_DEVICE_NAME_MAX_LEN + 1] = "SKODA_BT";
const char SeatBt[AT_DEVICE_NAME_MAX_LEN + 1]  = "SEAT_BT";
const char AudiBt[AT_DEVICE_NAME_MAX_LEN + 1]  = "AUDI_BT";

const Uint8Type VwPin[2]    = {0x00, 0x00};
const Uint8Type SkodaPin[2] = {0x12, 0x34};
const Uint8Type SeatPin[2]  = {0x12, 0x34};
const Uint8Type AudiPin[2]  = {0x11, 0x11};

typedef struct
{
    APP_CAN_ENUM_MARKE   brandIdentifier;
    const Uint8Type  *pin;
    const char *name;
} t_APP_DEVICE_BackupAreaEeprom;

static const t_APP_DEVICE_BackupAreaEeprom _APP_DEVICE_BackupAreaTable[5] = {
/*       |------------------------------------------------------- Marke Identifier
         |                        |------------------------------ BT Pin
         |                        |                |------------- BT Name  */
{ APP_CAN_VW_Volkswagen, 		VwPin,          VwBt},
{ APP_CAN_AU_Audi,              AudiPin,        AudiBt},
{ APP_CAN_SE_Seat,              SeatPin,        SeatBt},
{ APP_CAN_SK_Skoda,             SkodaPin,       SkodaBt},
{ APP_CAN_VN_VWNutzfzg,         VwPin,          VwBt}
};


/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcBackupEeprom                           */
/**
    \brief      Write to back up memory in eeprom

    \param      cb
                callback of call

    \param      brand
                APP_CAN_ENUM_MARKEl

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcBackupEeprom(
                                    void (*cb)(APP_COMMON_enResponseCode),
                                    APP_CAN_ENUM_MARKE brand)
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "rcBckupE2p: fctT 0x%02x busy",
                     gAppDeviceContext.fct.common.fctType );

        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_UPDATE_BACKUP_EEPROM;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.backupAreaEeprom.state    = APP_DEVICE_WRITE_BACK_BACKUP_BT_PIN;

    if (brand > APP_CAN_VN_VWNutzfzg)
    {
        DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "rcBckupE2p: inval brand %x", 
                     brand );
        gAppDeviceContext.fct.backupAreaEeprom.marke = APP_CAN_VW_Volkswagen;
    }
    else
    {
        gAppDeviceContext.fct.backupAreaEeprom.marke    = brand;
    }
    /* set BT-PIN and BT-Name of the current "Marke" */
    (void)memcpy(gAppDeviceContext.fct.backupAreaEeprom.name, 
                 (const void *)_APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].name,
                 (AT_DEVICE_NAME_MAX_LEN + 1));
    (void)memcpy(gAppDeviceContext.fct.backupAreaEeprom.pin, 
                 (const void *)_APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].pin, 
                 2);

    DEBUG_VALUE4(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "rcBckupE2p: BT-Name %c%c%c%c ... ",
                 gAppDeviceContext.fct.backupAreaEeprom.name[0],
                 gAppDeviceContext.fct.backupAreaEeprom.name[1],
                 gAppDeviceContext.fct.backupAreaEeprom.name[2],
                 gAppDeviceContext.fct.backupAreaEeprom.name[3]);
    DEBUG_VALUE4(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "rcBckupE2p: BT-Pin %02x%02x Marke %d %c",
                 gAppDeviceContext.fct.backupAreaEeprom.pin[0], gAppDeviceContext.fct.backupAreaEeprom.pin[1],
                 gAppDeviceContext.fct.backupAreaEeprom.marke, ' ');  // fourth param is only a dummy to be able to use macro

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(50), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcResetToFactorySetting                           */
/**
    \brief      Reset all parameters back to factory setting

    \param      cb
                callback of call

    \param      factoryCall
                function call reason

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcResetToFactorySetting(
                                    void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "APP_DEVICE_rcResetToFactorySetting: fctType %d busy, emergency release to start R2FS",
                           gAppDeviceContext.fct.common.fctType );
        APP_DEVICE_vEmergencyReleaseContextFromAppSd();
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    
    /* set VCR state to not started, because the CK5050p will be started new and the VCR is in reinitialization */
    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "APP_DEVICE called VCR reset state for RestoreFactorySettings");

    /* function added to fix ticket #8968 */
    (void)VCR_vStatusResetForRestoreFactorySettings();

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_RESET_TO_FACTORY_SETTING;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.factorySetting.state      = APP_DEVICE_NOTIFY_FACTORY_SETTING;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingUpdateBackupAreaEeprom           */
/**
    \brief      handle pending function to delete a profile

    \param      pBackupAreaEeprom
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingUpdateBackupAreaEeprom(const AppDeviceBackupAreaEepromType *pBackupAreaEeprom)
{
    SuccessFailType           sF        = SUCCESS;
    APP_COMMON_enResponseCode rsp       = APP_COMMON_RSP_ERROR;

    DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "UpdateBackupAreaEeprom status %d", pBackupAreaEeprom->state);

    switch( pBackupAreaEeprom->state )
    {
		case APP_DEVICE_WRITE_BACK_BACKUP_BT_PIN:

            DEBUG_VALUE2(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "UpdateBackupAreaEeprom write BT-PIN %02x%02x",
                                        _APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].pin[0],
                                        _APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].pin[1]);
            (void)EEPROM_sfRamWriteOnly(
                                (Uint8Type*)gAppDeviceContext.fct.backupAreaEeprom.pin,
                                EEPROM_LAYOUT_ENUM_WerkseinstellungBluetoothSetPINCode);
            rsp = APP_COMMON_RSP_OK;
            gAppDeviceContext.fct.backupAreaEeprom.state = APP_DEVICE_WRITE_BACK_BACKUP_BT_NAME;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(2), 0);
            break;

        case APP_DEVICE_WRITE_BACK_BACKUP_BT_NAME:
            DEBUG_VALUE4(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "UpdateBackupAreaEeprom write BT-NAME: %c%c%c%c...",
                         _APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].name[0],
                         _APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].name[1],
                         _APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].name[2],
                         _APP_DEVICE_BackupAreaTable[gAppDeviceContext.fct.backupAreaEeprom.marke].name[3]);
            (void)EEPROM_sfRamWriteOnly(
                                (Uint8Type*)gAppDeviceContext.fct.backupAreaEeprom.name,
                                EEPROM_LAYOUT_ENUM_WerkseinstellungUniversalPreparationForMobileTelephone);
            rsp = APP_COMMON_RSP_OK;
            gAppDeviceContext.fct.backupAreaEeprom.state = APP_DEVICE_WRITE_BACK_BACKUP_BRAND;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(2), 0);
            break;

        case APP_DEVICE_WRITE_BACK_BACKUP_BRAND:
            DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "UpdateBackupAreaEeprom write Marke: %d", 
                         gAppDeviceContext.fct.backupAreaEeprom.marke);
            (void)EEPROM_sfRamWriteOnly(
                            (Uint8Type*)&gAppDeviceContext.fct.backupAreaEeprom.marke,
                            EEPROM_LAYOUT_ENUM_LastSaveMarkeSelection);
            rsp = APP_COMMON_RSP_OK;
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
            break;

        default:
            rsp = APP_COMMON_RSP_ERROR;
            break;

    }
    switch( rsp )
    {
        case APP_COMMON_RSP_OK:
            /* wait for response */
            break;

        case APP_COMMON_RSP_ERROR_BUSY:
            /* try again later */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
            break;

        default:
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
            break;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingResetToFactorySetting           */
/**
    \brief      handle pending function to delete a profile

    \param      pDel
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingResetToFactorySetting(const AppDeviceFactorySettingType *pFactSet)
{

    SuccessFailType              sF  = SUCCESS;
    APP_COMMON_enResponseCode    rsp = APP_COMMON_RSP_ERROR;
    CommonResponseCodeType        rC = COMMON_RSP_ERROR;
    BooleanType   bBTAckSignalActive = BT_FALSE;
    Uint8Type u8BTAckSignalActive    = 0x0;
    EEDeviceControlBlockType *pEEApp = (EEDeviceControlBlockType *)getEEAppTaskControl();
    FlagType                    flag = EEPROM_flagIsEEFree(pEEApp);

    DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting status %d", pFactSet->state);

    /* delete profile depending on deviceType */
    switch( pFactSet->state )
    {
        case APP_DEVICE_NOTIFY_FACTORY_SETTING:

            if ((BT_FALSE == gAppDeviceContext.ck5050pIsReady) || (0 == CSHDL_vGetCkOnOffStatus()))
            {
                rsp = APP_COMMON_RSP_ERROR;
                DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting CK not ready");
            }
            else
            {
                DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "Start ResetToFactorySetting");
                _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_FACTORY_SETTING_RESET);
                gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_PHONE;
                rsp = APP_COMMON_RSP_OK;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            break;

        case APP_DEVICE_DISCONNECT_ACTIVE_PHONE:
        {
            if (AT_INVALID_DEVICE_ID == ERM_u8GetActivePhone())
            {
                /* no active phone connected */
                gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_A2DP;
                rsp = APP_COMMON_RSP_OK;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            else
            {
                DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting DisConnect active phone");
                rsp = ERM_rcStopPhoneService(ERM_u8GetActivePhone(),
                                            _APP_DEVICE_vDisconnectProfile_Cb);
                if ( APP_COMMON_RSP_OK == rsp )
                {
                    gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_A2DP;
                }
            }
            break;
        }
        case APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_A2DP:
        {
             if (AT_INVALID_DEVICE_ID == ERM_u8GetActivePlayer())
            {
                /* no vaild active player */
                gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_HEADSET;
                rsp = APP_COMMON_RSP_OK;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            else
            {
                DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting DisConnect active MP a2dp");
                rsp = ERM_rcStopPlayerA2dpSinkService(ERM_u8GetActivePlayer(),
                                                        _APP_DEVICE_vDisconnectProfile_Cb);
                if ( APP_COMMON_RSP_OK == rsp )
                {
                    /* check if active device have an avrcp player */
                    const ERMBTDeviceTableType *pDevice = ERM_GetBTDevice(ERM_u8GetActivePlayer());
                    if ((pDevice != NULL) && (pDevice->avrcp_active == AT_TRUE))
                    {
                        /* disconnect active avrcp player */
                        gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_AVRCP;
                    }
                    else
                    {
                        /* no active avrcp player */
                        DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting DisConnect no active avrcp player");
                        gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_HEADSET;
                    }
                }
            }
            break;
        }
        case APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_AVRCP:
        {
            if (AT_INVALID_DEVICE_ID == ERM_u8GetActivePlayer())
            {
                /* no vaild active player */
                gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_HEADSET;
                rsp = APP_COMMON_RSP_OK;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            else
            {

                DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting DisConnect active MP avrcp");
                rsp = ERM_rcStopPlayerAvrcpService(ERM_u8GetActivePlayer(),
                                                    _APP_DEVICE_vDisconnectProfile_Cb);
                if ( APP_COMMON_RSP_OK == rsp )
                {
                    gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DISCONNECT_ACTIVE_HEADSET;
                }
            }
            break;
        }
        case APP_DEVICE_DISCONNECT_ACTIVE_HEADSET:
            if (AT_INVALID_DEVICE_ID == ERM_u8GetActiveHeadSet())
            {
                /* no vaild active headset */
                gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_USER_PROFILES_RAM;
                rsp = APP_COMMON_RSP_OK;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            }
            else
            {
                DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting DisConnect active Headset");
                rsp = ERM_rcStopHeadsetService(ERM_u8GetActiveHeadSet(),
                                               _APP_DEVICE_vDisconnectProfile_Cb);
                if ( APP_COMMON_RSP_OK == rsp )
                {
                    gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_USER_PROFILES_RAM;
                }
            }
            break;

        case APP_DEVICE_DELETE_ALL_USER_PROFILES_RAM:
            /* delete all user profiles from RAM */
            ERM_vOnAppDeviceNotification(APP_DEVICE_NOTIFY_FACTORY_SETTING_RESET);
            APP_SD_vOnAppDeviceNotification(APP_DEVICE_NOTIFY_FACTORY_SETTING_RESET);
            (void)EEPROM_enGetSingleByteFromRamMirror(&u8BTAckSignalActive, EEPROM_LAYOUT_ENUM_BluetoothAcknowledgementSignal);
			if ( u8BTAckSignalActive == 0x0 )
			{
				bBTAckSignalActive = BT_TRUE;
			}
			else if ( u8BTAckSignalActive == 0x1)
			{
				bBTAckSignalActive = BT_FALSE;
			}
			else
            {
            	bBTAckSignalActive = BT_TRUE;
            	DEBUG_TEXT(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "ResetToFactorySetting defaultBTAck-True");
            }
            gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_SETTINGS_CK5050P;
            rsp = APP_COMMON_RSP_OK;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, SEC(bBTAckSignalActive*12), 0);
            break;

        case APP_DEVICE_DELETE_ALL_SETTINGS_CK5050P:
            DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting delete CK5050+ user profiles");
            rsp = ERM_rcResetToFactorySetting(_APP_DEVICE_vOnDeleteAllUserProfileCK5050p_Cb);
            break;

        case APP_DEVICE_WRITE_BACK_BT_PIN_NAME:
            /* read Backup BT-Pin */
            (void)EEPROM_enGetByteSequenceFromRamMirror(gAppDeviceContext.fct.factorySetting.pin,
                                                        VWDB_EE_ADDDRESS_WerkseinstellungBluetoothSetPINCode_LEN, 
                                                        EEPROM_LAYOUT_ENUM_WerkseinstellungBluetoothSetPINCode);
            /* read Backup BT-Name */
            (void)EEPROM_enGetByteSequenceFromRamMirror(gAppDeviceContext.fct.factorySetting.name,
                                                        VWDB_EE_ADDDRESS_WerkseinstellungUniversalPreparationForMobileTelephone_LEN, 
                                                        EEPROM_LAYOUT_ENUM_WerkseinstellungUniversalPreparationForMobileTelephone);
            /* write Backup BT-Pin */
            (void)EEPROM_sfRamWriteOnly((Uint8Type*)gAppDeviceContext.fct.factorySetting.pin,
                                        EEPROM_LAYOUT_ENUM_BluetoothSetPINCode);
            DEBUG_VALUE2(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE,"ResetToFactorySetting write BT-PIN %02x%02x",
                                                        gAppDeviceContext.fct.factorySetting.pin[0],
                                                        gAppDeviceContext.fct.factorySetting.pin[1]);
            /* write Backup BT-Name */
            (void)EEPROM_sfRamWriteOnly((Uint8Type*)gAppDeviceContext.fct.factorySetting.name,
                                        EEPROM_LAYOUT_ENUM_UniversalPreparationForMobileTelephone);
            DEBUG_VALUE4(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting write BT-NAME %c%c%c%c",
                                                        gAppDeviceContext.fct.factorySetting.name[0],
                                                        gAppDeviceContext.fct.factorySetting.name[1],
                                                        gAppDeviceContext.fct.factorySetting.name[2],
                                                        gAppDeviceContext.fct.factorySetting.name[3]);
            rsp = APP_COMMON_RSP_OK;
            gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_PROFILES_EEPROM;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
            break;

        case APP_DEVICE_DELETE_ALL_PROFILES_EEPROM:
            /* delete all user profiles from EEPROM */
            if (TRUE == flag)
            {
                rC = EEPROM_LAYOUT_rcWrite(pEEApp,
                                VWDB_EE_ADDDRESS_LASTACTIVEPROFILELISTS,
                                (Uint8Type*)_APP_DEVICE_Default_UserTable,
                                (Uint16Type)(MAX_USER_PROFILE_ENTRY),
                                 _APP_DEVICE_vOnWriteDefaultInEeprom_Cb);
                if (COMMON_RSP_OK == rC)
                {
                    DEBUG_TEXT(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "ResetToFactorySetting delete EEPROM user profiles");
                    gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_WAIT_FOR_EEPROM_FINISHED;
                    rsp = APP_COMMON_RSP_OK;
                }
                else
                {
                    rsp = APP_COMMON_RSP_ERROR_BUSY;
                }
            }
            else
            {
                rsp = APP_COMMON_RSP_ERROR_BUSY;
            }
            break;

        case APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_WAIT_FOR_EEPROM_FINISHED:
            /* do nothing, just wait */
            rsp  = APP_COMMON_RSP_OK;
            break;

        case APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_SUCCESS:
            /* #4345: correct response value to release function */
            rsp  = APP_COMMON_RSP_OK;
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
            break;

        case APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_FAILED:
            /* write eeprom failed */
            break;

        default:
            rsp = APP_COMMON_RSP_ERROR;
            break;
    }
    /* check result code */

    switch( rsp )
    {
        case APP_COMMON_RSP_OK:
            /* wait for response */
            break;

        case APP_COMMON_RSP_ERROR_BUSY:
            /* try again later */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
            break;

        default:
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
            break;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnWriteDefaultInEeprom_Cb                      */
/**
    \brief      callback indicating that request to deleze a profilefrom eeprom
                has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnWriteDefaultInEeprom_Cb(CommonResponseCodeType rC)
{
    /* extra case cause eeprom write and its callback is running in a separate task, so
       release context here */
    /* BE CAREFUL AND DO NOT CALL RELEASE FUNCTIONS DIRECTLY FROM EEPROM CONTEXT ( DataBase Task !!!! ) */
    if( COMMON_RSP_OK == rC )
    {
        gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_SUCCESS;
    }
    else
    {
        gAppDeviceContext.fct.factorySetting.state = APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_FAILED;
    }
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnDeleteAllUserProfileCK5050p_Cb               */
/**
    \brief      callback indicating that request to delete all
                from ck5050p has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnDeleteAllUserProfileCK5050p_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        /* wait of FLASHERASE:END from CK5050+ to skip to next state */
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vHandleErmStateChange                             */
/**
    \brief      handle state changes of ERM module

    \param      state
                ERMStateType
                new state of ERM module
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vHandleErmStateChange(ERMStateType state)
{
    switch(state)
    {
        case ERM_STATE_CK5050_ACTIVE:
        {
            gAppDeviceContext.ck5050pIsReady = BT_TRUE;
            break;
        }
        default:
            gAppDeviceContext.ck5050pIsReady = BT_FALSE;
            /* ignore the event */
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vDisconnectUser_Cb                      */
/**
    \brief      callback indicating that request to deleze a profilefrom eeprom
                has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vDisconnectProfile_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(2000), 0);
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/* End Of File APP_DEVICE_E2P_Backup_R2FS_C1.c */


