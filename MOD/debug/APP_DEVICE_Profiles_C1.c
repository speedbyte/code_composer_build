/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_Profiles_C1.c
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
#include "APP_SD.h"
#include "APP_MDI.h"
#include "APP_PHONEBOOK.h"
#include "APP_PHONEBOOK_BAP.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_PHONE_BAP.h"
#include "APP_DIAG.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "ATParser_CE.h"
#include "ERM_Utils.h"
#include "ERM_MessageHandler.h"
#include "ERM_DeviceHandler.h"
#include "OSEK_AppTask_H1.h"
#include "Startup.h"
#include "CSHDL.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_defines.h"    // vw
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_DEVICE.h"
#include "APP_DEVICE_CI.h"
#include "APP_DEVICE_Profiles.h"

#include "APP_DEVICE_Profiles_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/**
\brief	variable to save the current device function in progress
*/
extern AppDeviceContext gAppDeviceContext;


/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the type
*/


static APP_COMMON_enResponseCode APP_DEVICE_rcAddProfile(
                        void (*cb)(APP_COMMON_enResponseCode),
                        APP_DEVICE_DeviceType deviceType, AT_DeviceId deviceId,
                        const Uint8Type *pName, Uint16Type length );

static APP_COMMON_enResponseCode _APP_DEVICE_rcDeleteProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        APP_DEVICE_DeviceType deviceType,
        AT_DeviceId deviceId,
        Uint16Type indexOfProfile);


static APP_COMMON_enResponseCode APP_DEVICE_rcChangeDeviceName(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length  );


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
/* Function    :   APP_DEVICE_rcAddPlayerProfile                               */
/**
    \brief      adds a player profile in eeprom

    \brief      Description:
                adds a player profile in eeprom

    \param      cb
                callback of calling application

    \param      deviceId
                AT_DeviceId
                deviceId of associated bt device

    \param      pName
                const Uint8Type
                name of new profile

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcAddPlayerProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    return APP_DEVICE_rcAddProfile(cb, APP_DEVICE_enTYPE_PLAYER,
                                   deviceId, pName, length);
}
/*----------------------------------------------------------------------------*/
/* Function    :   APP_DEVICE_rcAddHeadsetProfile                               */
/**
    \brief      adds a headset profile in eeprom

    \param      cb
                callback of call

    \param      deviceId
                AT_DeviceId
                deviceId of associated bt device

    \param      pName
                const Uint8Type *
                name of new profile

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcAddHeadsetProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    return APP_DEVICE_rcAddProfile(cb, APP_DEVICE_enTYPE_HEADSET,
                                   deviceId, pName, length);
}
/*----------------------------------------------------------------------------*/
/* Function    :   APP_DEVICE_rcAddUserProfile                               */
/**
    \brief      adds a user profile in eeprom

    \brief      Description:
                adds a user profile in eeprom

    \param      cb
                callback of call

    \param      deviceId
                AT_DeviceId
                deviceId of associated bt device

    \param      pName
                const Uint8Type
                name of new profile

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcAddUserProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    return APP_DEVICE_rcAddProfile(cb, APP_DEVICE_enTYPE_PHONE,
                                   deviceId, pName, length);
}

/*----------------------------------------------------------------------------*/
/* Function    :   APP_DEVICE_rcAddProfile                               */
/**
    \brief      adds a profile in eeprom

    \brief      Description:
                adds a profile in eeprom

    \param      cb
                callback of call

    \param      deviceType
                APP_DEVICE_DeviceType
                is used to select the type of profile to be added

    \param      deviceId
                AT_DeviceId
                deviceId of associated bt device

    \param      pName
                const Uint8Type
                name of new profile

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcAddProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        APP_DEVICE_DeviceType deviceType,
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "AddProfile: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType        = APP_DEVICE_ERM_ADD_PROFILE;
    gAppDeviceContext.fct.common.callBack       = cb;
    gAppDeviceContext.fct.addProfile.deviceType = deviceType;
    gAppDeviceContext.fct.addProfile.deviceId   = deviceId;

    gAppDeviceContext.fct.addProfile.state      = APP_DEVICE_ADD_PROFILE_IN_EEPROM_NOT_SENT;

    // copy name and length params
    (void)memset(gAppDeviceContext.fct.addProfile.name, 0x00, sizeof(gAppDeviceContext.fct.addProfile.name));
    if( length > sizeof(gAppDeviceContext.fct.addProfile.name))
    {
        length = sizeof(gAppDeviceContext.fct.addProfile.name);
    }
    (void)memcpy(gAppDeviceContext.fct.addProfile.name,pName,length);
    gAppDeviceContext.fct.addProfile.nameLen    = length;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :   APP_DEVICE_rcAddProfile                               */
/**
    \brief      save the BT name in eeprom

    \param      cb
                callback of call

    \param      pData
                const Uint8Type
                name of new profile

    \param      dataLen
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSaveBtName(
        void (*cb)(APP_COMMON_enResponseCode),
        const Uint8Type* pData,
        Uint16Type dataLen )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SaveBtName: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType        = APP_DEVICE_ERM_SAVE_BT_NAME;
    gAppDeviceContext.fct.common.callBack       = cb;
    if ( dataLen > AT_DEVICE_NAME_MAX_LEN )
    {
        dataLen = AT_DEVICE_NAME_MAX_LEN;
    }
    (void)memcpy(gAppDeviceContext.fct.saveBtName.btName, pData, dataLen);
    gAppDeviceContext.fct.saveBtName.btName[dataLen] = (Uint8Type)0x00;
    gAppDeviceContext.fct.saveBtName.length = dataLen + 1;

    gAppDeviceContext.fct.saveBtName.state      = APP_DEVICE_WRITE_BT_NAME_TO_EEPROM;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcDeleteUserProfile                              */
/**
    \brief      deletes a user profile in from eeprom and deletes the pairing of
                the associated bt device
                if there are player profiles associated to deviceIndex of user
                profile, these will be deleted too.

    \param      cb
                callback of call

    \param      indexOfProfile
                Uint16Type
                index of the user profile to be deleted

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcDeleteUserProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile)
{
    const ERMUserProfileType * profile = ERM_pGetUserProfile((Uint8Type)indexOfProfile);
    if( NULL == profile )
    {   /* profile does not exists */
        return APP_COMMON_RSP_ERROR;
    }
    return _APP_DEVICE_rcDeleteProfile(cb,
                                       APP_DEVICE_enTYPE_PHONE,
                                       profile->deviceIndex, indexOfProfile);
}
/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcDeletePlayerProfile                              */
/**
    \brief      deletes a player profile from eeprom and deletes the pairing of
                the associated bt device

    \brief      Description:
                deletes a player profile in from eeprom and deletes the pairing of
                the associated bt device
                user profiles which are associated to the device are also
                deleted. calling app has to check first, whether a user profile exists , if it
                should not be deleted

    \param      cb
                callback of call

    \param      indexOfProfile
                Uint16Type
                index of the player profile to be deleted

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcDeletePlayerProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile)
{
    const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile((Uint8Type)indexOfProfile);
    if( NULL == profile )
    {   /* profile does not exists */
        return APP_COMMON_RSP_ERROR;
    }
    if(APP_DEVICE_enTYPE_PLAYER != profile->deviceType )
    {
        return APP_COMMON_RSP_ERROR;
    }
    return _APP_DEVICE_rcDeleteProfile(cb,
                                       APP_DEVICE_enTYPE_PLAYER,
                                       profile->deviceIndex, indexOfProfile);
}
/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcDeleteHeadsetProfile                           */
/**
    \brief      deletes a headset profile from eeprom and deletes the pairing of
                the associated bt device

    \param      cb
                callback of call

    \param      indexOfProfile
                Uint16Type
                index of the headset profile to be deleted

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcDeleteHeadsetProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile)
{
    const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile((Uint8Type)indexOfProfile);
    if( NULL == profile )
    {   /* profile does not exists */
        return APP_COMMON_RSP_ERROR;
    }
    if(APP_DEVICE_enTYPE_HEADSET != profile->deviceType )
    {
        return APP_COMMON_RSP_ERROR;
    }
    return _APP_DEVICE_rcDeleteProfile(cb,
                                       APP_DEVICE_enTYPE_HEADSET,
                                       profile->deviceIndex, indexOfProfile);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_rcDeleteUserProfile                              */
/**
    \brief      deletes a user profile in from eeprom and deletes the pairing of
                the associated bt device

    \brief      Description:
                deletes a user profile in from eeprom and deletes the pairing of
                the associated bt device

    \param      cb
                callback of call

    \param      deviceType
                kind of profile to be deleted

    \param      deviceId
                deviceId of associated device

    \param      indexOfProfile
                Uint16Type
                index of the user profile to be deleted

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_rcDeleteProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        APP_DEVICE_DeviceType deviceType,
        AT_DeviceId deviceId,
        Uint16Type indexOfProfile)
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "APP_DEVICE_rcDeleteUserProfile: devType: %d, fctType 0x%02x busy",
                        deviceType, gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    // TODO: Add handling to delete associations of a deleted profile / device
    // and a user profile, this is relevant only when a headset or player
    // profile is deleted and one or more user profiles use the deleted profile
    // as associated headset or player
    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_DELETE_PROFILE;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.delProfile.state          = APP_DEVICE_DELETE_PROFILE_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.delProfile.profileIndex   = indexOfProfile;
    gAppDeviceContext.fct.delProfile.deviceId       = deviceId;
    gAppDeviceContext.fct.delProfile.deviceType     = deviceType;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_rcDeleteAllProfiles                             */
/**
    \brief      deletes all user profiles in eeprom and deletes the pairing of
                the associated bt device

    \param      cb
                callback of call

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcDeleteAllProfiles(
        void (*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "APP_DEVICE_rcDeleteAllProfiles: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_DELETE_ALL_PROFILES;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.delAllProfiles.state      = APP_DEVICE_DELETE_ALL_PROFILES_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.delAllProfiles.profileIndex   = 0;
    gAppDeviceContext.fct.delAllProfiles.deviceType     = APP_DEVICE_enTYPE_PHONE;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetRingtone                              */
/**
    \brief      changes ringtone alternate mode of given user profile index

    \brief      Description:
                changes ringtone alternate mode of given user profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where ringtone alternate mode should be changed for

    \param      bAlternate
                BooleanType
                whether to alternate or not

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetRingtoneAlternate(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        BooleanType bAlternate)
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "APP_DEVICE_rcSetRingtoneAlternate: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                    = APP_DEVICE_ERM_CHANGE_RINGTONE_MODE;
    gAppDeviceContext.fct.common.callBack                   = cb;
    gAppDeviceContext.fct.changeRingtoneMode.state          = APP_DEVICE_CHANGE_RINGTONE_MODE_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.changeRingtoneMode.profileIndex   = profileIndex;

    /* copy new ringtone */
    gAppDeviceContext.fct.changeRingtoneMode.bAlternate     = bAlternate;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetPbCompletelyLoaded                              */
/**
    \brief      save that pb has been loaded completely for active user profile

    \brief      Description:
                save that pb has been loaded completely for active user profile

    \param      cb
                callback of call

    \param      profileIndex
                Uint8Type
                profile index where ringtone alternate mode should be changed for

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetPbCompletelyLoaded(void (*cb)(APP_COMMON_enResponseCode),
                                                             Uint8Type profileIndex)
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "rcSetPbCompletelyLoaded: fctType 0x%02x busy",
                     gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                    = APP_DEVICE_ERM_SET_PB_COMPLETELY_LOADED;
    gAppDeviceContext.fct.common.callBack                   = cb;
    gAppDeviceContext.fct.setPbCompletelyLoaded.state       = APP_DEVICE_SET_PB_COMPLETELY_LOADED_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.setPbCompletelyLoaded.profileIndex= profileIndex;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetHeadsetAuthMode                              */
/**
    \brief      changes authentication mode of given headset profile index

    \brief      Description:
                changes authentication mode of given headset profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where authentication mode should be changed for

    \param      bAuthNeeded
                BooleanType
                authentication needed or not

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetHeadsetAuthMode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        BooleanType bAuthNeeded )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "APP_DEVICE_rcSetHeadsetAuthMode: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_ERM_CHANGE_AUTH_MODE;
    gAppDeviceContext.fct.common.callBack               = cb;
    gAppDeviceContext.fct.changeAuthMode.state          = APP_DEVICE_CHANGE_AUTH_MODE_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.changeAuthMode.profileIdx     = profileIndex;
    gAppDeviceContext.fct.changeAuthMode.profileType    = APP_DEVICE_enTYPE_HEADSET;

    /* copy new mode */
    gAppDeviceContext.fct.changeAuthMode.bAuthNeeded    = bAuthNeeded;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetPlayerAuthMode                              */
/**
    \brief      changes authentication mode of given player profile index

    \brief      Description:
                changes authentication mode of given player profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where authentication mode should be changed for

    \param      bAuthNeeded
                BooleanType
                authentication needed or not

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetPlayerAuthMode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        BooleanType bAuthNeeded )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetPlAuthMode: fctType 0x%02x busy",
                     gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_ERM_CHANGE_AUTH_MODE;
    gAppDeviceContext.fct.common.callBack               = cb;
    gAppDeviceContext.fct.changeAuthMode.state          = APP_DEVICE_CHANGE_AUTH_MODE_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.changeAuthMode.profileIdx     = profileIndex;
    gAppDeviceContext.fct.changeAuthMode.profileType    = APP_DEVICE_enTYPE_PLAYER;

    /* copy new mode */
    gAppDeviceContext.fct.changeAuthMode.bAuthNeeded    = bAuthNeeded;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetPlayerHsName                              */
/**
    \brief      changes name of given headset/player profile index

    \brief      Description:
                changes name of given headset/player profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where name should be changed for

    \param      pName
                const Uint8Type *
                new name of device

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetPlayerHsName(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pName, Uint16Type length  )
{
    const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile(profileIndex);

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetPlayerHsName: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if( NULL == profile )
    {
        return APP_COMMON_RSP_ERROR_PROFILE_NOT_EXISTING;
    }
    if( ERM_MAX_NUM_PHYSICAL_DEVICES <= profile->deviceIndex )
    {
        return APP_COMMON_RSP_ERROR_INVALID_DEVICE_ID;
    }

    // change name via device id
    return APP_DEVICE_rcChangeDeviceName(cb, profile->deviceIndex, pName, length);
}


/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetUserName                                    */
/**
    \brief      changes name of given user profile index

    \brief      Description:
                changes name of given user profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where name should be changed for

    \param      pName
                const Uint8Type *
                new name of device

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetUserName(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pName, Uint16Type length  )
{
    const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetUserName: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if( NULL == profile )
    {
        return APP_COMMON_RSP_ERROR_PROFILE_NOT_EXISTING;
    }
    if( ERM_MAX_NUM_PHYSICAL_DEVICES <= profile->deviceIndex )
    {
        return APP_COMMON_RSP_ERROR_INVALID_DEVICE_ID;
    }

    // change name via device id
    return APP_DEVICE_rcChangeDeviceName(cb, profile->deviceIndex, pName, length);
}



/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcChangeDeviceName                              */
/**
    \brief      changes name of given deviceId

    \brief      Description:
                changes name of given deviceId

    \param      cb
                callback of call

    \param      deviceId
                AT_DeviceId
                deviceId where name should be changed for

    \param      pName
                const Uint8Type *
                new name of device

    \param      length
                Uint16Type
                length of name

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcChangeDeviceName(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length  )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "ChangeDeviceName: fctType 0x%02x busy",
                    gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_ERM_CHANGE_DEVICE_NAME;
    gAppDeviceContext.fct.common.callBack               = cb;
    gAppDeviceContext.fct.changeDeviceName.state        = APP_DEVICE_CHANGE_DEVICE_NAME_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.changeDeviceName.deviceId     = deviceId;

    // check length
    if( length > sizeof(AT_DeviceNameString))
    {
        length = sizeof(AT_DeviceNameString);
    }

    // copy name
    gAppDeviceContext.fct.changeDeviceName.nameLen      = length;
    (void)memset(gAppDeviceContext.fct.changeDeviceName.name, 0x00, sizeof(AT_DeviceNameString));
    (void)memcpy(gAppDeviceContext.fct.changeDeviceName.name, pName, length);

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetRingtone                              */
/**
    \brief      changes ringtone of given user profile index

    \brief      Description:
                changes ringtone of given user profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where ringtone should be changed for

    \param      ringtone
                ATRintoneModeEnum
                new ringtone

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetRingtone(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        ATRintoneModeEnum ringtone )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetRingt: fctType 0x%02x busy",
                    gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_ERM_CHANGE_RINGTONE;
    gAppDeviceContext.fct.common.callBack               = cb;
    gAppDeviceContext.fct.changeRingtone.state          = APP_DEVICE_CHANGE_RINGTONE_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.changeRingtone.profileIndex   = profileIndex;

    /* copy new ringtone */
    gAppDeviceContext.fct.changeRingtone.ringtone       = ringtone;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    :  APP_DEVICE_rcSetVoiceMailboxNumber                              */
/**
    \brief      changes voice mailbox number of given user profile index

    \brief      Description:
                changes voice mailbox number of given user profile index

    \param      cb
                callback of call

    \param      profileIndex
                profile index where number should be changed for

    \param      pNumber
                const Uint8Type *
                new voicemailbox number

    \param      len
                Uint16Type
                length of new voicemailbox number

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcSetVoiceMailboxNumber(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pNumber, Uint16Type len )
{
    Uint16Type length = 0;

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetVoiceMailbNum: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_CHANGE_MAILBOX_NUMBER;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.changeMailbox.state       = APP_DEVICE_CHANGE_MAILBOX_IN_EEPROM_NOT_SENT;
    gAppDeviceContext.fct.changeMailbox.profileIndex= profileIndex;

    /* copy new number */
    length = ( len > sizeof( ATPhoneNumberType )) ?
                    sizeof( ATPhoneNumberType ) : len;
    (void)memset(gAppDeviceContext.fct.changeMailbox.number,0x00, sizeof(ATPhoneNumberType));
    (void)memcpy(gAppDeviceContext.fct.changeMailbox.number, pNumber, length);
    gAppDeviceContext.fct.changeMailbox.length = length;


    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;


}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfAddMissingUserProfile                                                            */
/**
    \brief      an active phone has been detected and there is no user profile
                existing and also no SD available to trigger user adding a
                profile,
                so try to add it on ourself

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR  */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_sfAddMissingUserProfile(void(*cb)(APP_COMMON_enResponseCode))
{
    AT_DeviceId deviceId    = ERM_u8GetActivePhone();
    const ERMBTDeviceTableType * btDev = ERM_GetBTDevice(deviceId);

    if( AT_INVALID_DEVICE_ID == deviceId || NULL == btDev )
    {
        return APP_COMMON_RSP_ERROR;
    }

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "AddMisUserProf: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.addMissProfile.deviceId   = deviceId;

    gAppDeviceContext.fct.addMissProfile.nameLen = btDev->device_name_len;
    if( gAppDeviceContext.fct.addMissProfile.nameLen > sizeof(AT_DeviceNameString))
    {
        gAppDeviceContext.fct.addMissProfile.nameLen = sizeof(AT_DeviceNameString);
    }
    (void)memcpy(gAppDeviceContext.fct.addMissProfile.name, btDev->device_name,
            gAppDeviceContext.fct.addMissProfile.nameLen);

    /* check which state we need to start with */
    if( BT_TRUE == ERM_bMaxNumberOfUserProfilesReached() )
    {   /* start to delete oldest */
        gAppDeviceContext.fct.addMissProfile.state = APP_DEVICE_DELETE_OLDEST_PROFILE_IN_EEPROM_NOT_SENT;
    }
    else
    {   /* immediately add the new profile */
        gAppDeviceContext.fct.addMissProfile.state = APP_DEVICE_ADD_NEW_PROFILE_NOT_SENT;
    }

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF ()                               */


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfAddMissingPlayerProfile                                                            */
/**
    \brief      an active player has been detected and there is no player profile
                existing, and also no SD available to trigger user adding a
                profile,
                so try to add it on ourself

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_sfAddMissingPlayerProfile(void(*cb)(APP_COMMON_enResponseCode))
{
    AT_DeviceId deviceId    = ERM_u8GetActivePlayer();
    const ERMBTDeviceTableType * btDev = ERM_GetBTDevice(deviceId);

    if( AT_INVALID_DEVICE_ID == deviceId || NULL == btDev )
    {
        return APP_COMMON_RSP_ERROR;
    }

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "AddMisPlProf: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.addMissProfile.deviceId   = deviceId;

    gAppDeviceContext.fct.addMissProfile.nameLen = btDev->device_name_len;
    if( gAppDeviceContext.fct.addMissProfile.nameLen > sizeof(AT_DeviceNameString))
    {
        gAppDeviceContext.fct.addMissProfile.nameLen = sizeof(AT_DeviceNameString);
    }
    (void)memcpy(gAppDeviceContext.fct.addMissProfile.name, btDev->device_name,
            gAppDeviceContext.fct.addMissProfile.nameLen);

    /* check which state we need to start with */
    if( BT_TRUE == ERM_bMaxNumberOfPlayerProfilesReached() ||
        BT_TRUE == ERM_bMaxNumberOfPlayerHeadsetProfilesReached())
    {   /* start to delete oldest cause max. number of players is reached or profile table is full  */
        gAppDeviceContext.fct.addMissProfile.state = APP_DEVICE_DELETE_OLDEST_PROFILE_IN_EEPROM_NOT_SENT;
    }
    else
    {   /* immediately add the new profile */
        gAppDeviceContext.fct.addMissProfile.state = APP_DEVICE_ADD_NEW_PROFILE_NOT_SENT;
    }

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF ()                               */


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfInitCK5050ForUserProfile                                                            */
/**
    \brief      a new user is connected, we need to intialize ck5050 with the
                profile settings of this user profile

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                OK, ERROR or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_sfInitCK5050ForUserProfile(void(*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "InitCKForUserProf: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_INIT_CK5050_FOR_USER;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.initCk5050ForUser.state   = APP_DEVICE_INIT_CK5050_RINGTONE_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_rcAssociateHeadsetToUser                                                            */
/**
    \brief      associate currently connected headset to active user profile

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                OK, ERROR or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_rcAssociateHeadsetToUser(void(*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "AssocHsToUser: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_ASSOCIATE_HEADSET_TO_USER;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.changeAssocHeadset.state  = APP_DEVICE_CHANGE_ASSOC_HEADSET_IN_EEPROM_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vChangePBSortingOrder                                                            */
/**
    \brief      handles the sorting order change for phone book

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                OK, ERROR or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_vChangePBSortingOrder(void(*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
        {

            DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "vChangePBSortingOrder: fctType 0x%02x busy",
                            gAppDeviceContext.fct.common.fctType );
            return APP_COMMON_RSP_ERROR_BUSY;
        }

        gAppDeviceContext.fct.common.fctType            = APP_DEVICE_CHANGE_PB_ORDER_FOR_NEW_USER;
        gAppDeviceContext.fct.common.callBack           = cb;
        gAppDeviceContext.fct.changePBOrder.state       = APP_DEVICE_CHANGE_PB_ORDER_FOR_NEW_USER_NOT_SENT;

        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

        return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_rcAssociatePlayerToUser                                                            */
/**
    \brief      associate currently connected player to active user profile

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                OK, ERROR or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_rcAssociatePlayerToUser(void(*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "AssocPlToUser: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType            = APP_DEVICE_ERM_ASSOCIATE_PLAYER_TO_USER;
    gAppDeviceContext.fct.common.callBack           = cb;
    gAppDeviceContext.fct.changeAssocPlayer.state   = APP_DEVICE_CHANGE_ASSOC_PLAYER_IN_EEPROM_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(                                                           */
/**
    \brief      associate currently connected player to active user profile

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                OK, ERROR or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(Uint8Type deviceId)
{
    
     Uint8Type counter = 0;
    gAppDeviceContext.updatedeletedPlayerinProfilesDeviceId = deviceId;
    gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer = 0;
    
    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "UpdatedeletedPlayerInUserProfiles called");
    for (counter=0; counter < ERM_MAX_NUM_USER_PROFILES; counter++)
    {
        Uint8Type profileId = ERM_GetLastActiveProfileId( counter,
                                                      APP_DEVICE_enTYPE_PHONE);
        const ERMUserProfileType *pUserProfile = ERM_pGetUserProfile(profileId);
        
        if (NULL == pUserProfile)
        {
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles: actual pUserProfile empty, finished ");            
            break;
        }       

        if (pUserProfile->playerIndex == deviceId)
        {          
            DEBUG_VALUE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles: pUserProfile->deviceIndex %d has also player deviceId %d",
             pUserProfile->deviceIndex , deviceId);
             
            gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer++; 
        }
        DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles:numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer %d ",
                     gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer );
    }
    
    if (gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer > 0)
    {   
        if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
        {

            DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "UpdatedeletedPlayerInUserProfiles: fctType 0x%02x busy",
                            gAppDeviceContext.fct.common.fctType );
            gAppDeviceContext.updateDeletedPlayerInProfiles = BT_TRUE;
            
        }
        else
        {
            gAppDeviceContext.fct.common.fctType = APP_DEVICE_ERM_UPDATE_DELETED_PLAYER_IN_PROFILES;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);
        }    
    }
    
    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfUpdateErmActiveProfileTable                                                            */
/**
    \brief      update Active profile list so we always know which was the last
                connected device ( phone, headset, player )

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                OK, ERROR or BUSY
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _APP_DEVICE_sfUpdateErmActiveProfileTable(void(*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

    	DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "UpErmActProfTable: fctType 0x%02x busy",
                        gAppDeviceContext.fct.common.fctType );
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType     = APP_DEVICE_ERM_UPDATE_ACTIVE_PROFILE_LIST;
    gAppDeviceContext.fct.common.callBack    = cb;
    gAppDeviceContext.fct.actProfile.state   = APP_DEVICE_UPDATE_PROFILE_LIST_IN_EEPROM_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF ()                               */


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingAddMissingPlayerProfile                                                            */
/**
    \brief      handle pending function to add a missing player profile

    \param      pMiss
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingAddMissingPlayerProfile(AppDeviceAddMissingProfileType *pMiss)
{
    SuccessFailType result = SUCCESS;
    switch(pMiss->state)
    {
        case APP_DEVICE_DELETE_OLDEST_PROFILE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            // get the player profile which is the last in active player profile
            // list, skip player profiles which are associated to user profiles
            const ERMPlayerHeadsetProfileType * oldestProfile   = NULL;
            APP_DEVICE_DeviceType profileType   = APP_DEVICE_enTYPE_PLAYER;
            Uint8Type oldestProfileIdx          = ERM_u8GetOldestPlayerProfileId();

            /* check if got a valid profile index */
            if( AT_INVALID_DEVICE_ID == oldestProfileIdx )
            {   /* there was no oldest player profile, so check for oldest headset profile to be deleted instead */
                oldestProfileIdx = ERM_u8GetOldestHeadsetProfileId();
                profileType      = APP_DEVICE_enTYPE_HEADSET;
            }

            oldestProfile = ERM_pGetPlayerHeadsetProfile(oldestProfileIdx);
            if( NULL == oldestProfile )
            {

                DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "Add miss. player prof. failed. Invalid val for oldest prof.: %d",
                             oldestProfileIdx );
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }
            /* delete profile from player / headset table and save deviceId to
             * delete pairing later */
            pMiss->oldestProfileDeviceId = oldestProfile->deviceIndex;
            switch(profileType)
            {
                case APP_DEVICE_enTYPE_PLAYER:
                    rsp = ERM_rcDeletePlayerProfile_InEeprom(_APP_DEVICE_vOnAddMissingProfile_Cb,
                                                             oldestProfileIdx);

                    DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "Add miss. player: Deleting oldest player profile: %d",
                                 oldestProfileIdx );
                    break;
                case APP_DEVICE_enTYPE_HEADSET:
                    rsp = ERM_rcDeleteHeadsetProfile_InEeprom(_APP_DEVICE_vOnAddMissingProfile_Cb,
                                                              oldestProfileIdx);

                    DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "Add miss. player: Deleting oldest headset profile: %d",
                                 oldestProfileIdx );
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
                    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_DELETE_OLDEST_PROFILE_PAIRING_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcDeleteDevice(
                                                _APP_DEVICE_vOnAddMissingProfile_Cb,
                                                pMiss->oldestProfileDeviceId);
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
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_ADD_NEW_PROFILE_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcAddPlayerProfile_InEeprom(
                                          _APP_DEVICE_vOnAddMissingProfile_Cb,
                                          pMiss->deviceId, pMiss->name, pMiss->nameLen);
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
                    return FAIL;
            }
            break;
        }
        default:
            break;
    }
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingAddMissingUserProfile                                                            */
/**
    \brief      handle pending function to add a missing user profile

    \param      pMiss
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingAddMissingUserProfile(AppDeviceAddMissingProfileType *pMiss)
{
    SuccessFailType result = SUCCESS;
    switch(pMiss->state)
    {
        case APP_DEVICE_DELETE_OLDEST_PROFILE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            // get the user profile which is the last in active user profile list
            Uint8Type oldestProfileIdx               = ERM_u8GetOldestUserProfileId();
            const ERMUserProfileType * oldestProfile = ERM_pGetUserProfile(oldestProfileIdx);
            if( NULL == oldestProfile )
            {

                DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "Add miss. user prof. failed. Invalid val for oldest prof.: %d",
                             oldestProfileIdx);
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }
            /* delete user profile */
            pMiss->oldestProfileDeviceId = oldestProfile->deviceIndex;
            rsp = ERM_rcDeleteUserProfile_InEeprom(_APP_DEVICE_vOnAddMissingProfile_Cb,
                                                   oldestProfileIdx);
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
                    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_DELETE_OLDEST_PROFILE_PAIRING_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcDeleteDevice(
                                                _APP_DEVICE_vOnAddMissingProfile_Cb,
                                                pMiss->oldestProfileDeviceId);
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
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_ADD_NEW_PROFILE_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcAddUserProfile_InEeprom(
                                          _APP_DEVICE_vOnAddMissingProfile_Cb,
                                          pMiss->deviceId, pMiss->name, pMiss->nameLen);
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
                    return FAIL;
            }
            break;
        }
        default:
            break;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingUpdateActiveProfileLis           */
/**
    \brief      handle pending function to update active profile list

    \param      pUp
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingUpdateActiveProfileList(const AppDeviceUpdateActiveProfileListType * pUp)
{
    SuccessFailType result = SUCCESS;
    switch(pUp->state)
    {
        case APP_DEVICE_UPDATE_PROFILE_LIST_IN_EEPROM_NOT_SENT:
        {
            /* check which of the types we need to update */
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            if( AT_INVALID_DEVICE_ID != gAppDeviceContext.newActPhoneProfileIdx )
            {
                rsp = ERM_rcUpdatePhoneActiveProfileList( _APP_DEVICE_vOnAddActiveProfileToEeprom_Cb,
                                                          gAppDeviceContext.newActPhoneProfileIdx);
                /* check result code */
                switch( rsp )
                {
                    case APP_COMMON_RSP_OK:
                        /* wait for response */
                        gAppDeviceContext.newActPhoneProfileIdx = AT_INVALID_DEVICE_ID;
                        break;
                    case APP_COMMON_RSP_ERROR_BUSY:
                        /* try again later */
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                        break;
                    default:
                        gAppDeviceContext.newActPhoneProfileIdx = AT_INVALID_DEVICE_ID;
                        break;
                }
            }
            else if( AT_INVALID_DEVICE_ID != gAppDeviceContext.newActPlayerProfileIdx)
            {
                rsp = ERM_rcUpdatePlayerActiveProfileList(_APP_DEVICE_vOnAddActiveProfileToEeprom_Cb,
                                                          gAppDeviceContext.newActPlayerProfileIdx);
                /* check result code */
                switch( rsp )
                {
                    case APP_COMMON_RSP_OK:
                        /* wait for response */
                        gAppDeviceContext.newActPlayerProfileIdx = AT_INVALID_DEVICE_ID;
                        break;
                    case APP_COMMON_RSP_ERROR_BUSY:
                        /* try again later */
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                        break;
                    default:
                        gAppDeviceContext.newActPlayerProfileIdx = AT_INVALID_DEVICE_ID;
                        break;
                }
            }
            else if( AT_INVALID_DEVICE_ID != gAppDeviceContext.newActHeadsetProfileIdx)
            {
                rsp = ERM_rcUpdateHeadsetActiveProfileList( _APP_DEVICE_vOnAddActiveProfileToEeprom_Cb,
                                                          gAppDeviceContext.newActHeadsetProfileIdx);
                if( APP_COMMON_RSP_OK == rsp )
                {
                    gAppDeviceContext.newActHeadsetProfileIdx = AT_INVALID_DEVICE_ID;
                }
                /* check result code */
                switch( rsp )
                {
                    case APP_COMMON_RSP_OK:
                        /* wait for response */
                        gAppDeviceContext.newActHeadsetProfileIdx = AT_INVALID_DEVICE_ID;
                        break;
                    case APP_COMMON_RSP_ERROR_BUSY:
                        /* try again later */
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                        break;
                    default:
                        gAppDeviceContext.newActHeadsetProfileIdx = AT_INVALID_DEVICE_ID;
                        break;
                }
            }
            if( APP_COMMON_RSP_OK != rsp && APP_COMMON_RSP_ERROR_BUSY != rsp )
            {   /* check if we can reset pending value */
                if( gAppDeviceContext.newActPhoneProfileIdx   == AT_INVALID_DEVICE_ID &&
                    gAppDeviceContext.newActPlayerProfileIdx  == AT_INVALID_DEVICE_ID &&
                    gAppDeviceContext.newActHeadsetProfileIdx == AT_INVALID_DEVICE_ID )
                {
                    gAppDeviceContext.updateActiveProfilePending = BT_FALSE;
                }
                _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
            }
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingAddProfile           */
/**
    \brief      handle pending function to add a new profile

    \param      pAdd
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingAddProfile(const AppDeviceAddProfileType *pAdd)
{
    SuccessFailType result = SUCCESS;
    switch(pAdd->state)
    {
        case APP_DEVICE_ADD_PROFILE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_ERROR;

            /* add profile depending on deviceType */
            switch( pAdd->deviceType )
            {
                case APP_DEVICE_enTYPE_PHONE:
                    rsp = ERM_rcAddUserProfile_InEeprom(
                                          _APP_DEVICE_vOnAddProfileToEeprom_Cb,
                                          pAdd->deviceId, pAdd->name, pAdd->nameLen);
                    break;
                case APP_DEVICE_enTYPE_HEADSET:
                    rsp = ERM_rcAddHeadsetProfile_InEeprom(
                                          _APP_DEVICE_vOnAddProfileToEeprom_Cb,
                                          pAdd->deviceId, pAdd->name, pAdd->nameLen);
                    break;
                case APP_DEVICE_enTYPE_PLAYER:
                    rsp = ERM_rcAddPlayerProfile_InEeprom(
                                           _APP_DEVICE_vOnAddProfileToEeprom_Cb,
                                           pAdd->deviceId, pAdd->name, pAdd->nameLen);
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
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_ADD_PROFILE_IN_EEPROM_SENT:
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingAssociateHeadsetToUser           */
/**
    \brief      handle pending function to associate currently active headset
                to currently active user profile

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingAssociateHeadsetToUser(const AppDeviceChangeAssociatedHeadsetType * pChange )
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_ASSOC_HEADSET_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp   = APP_COMMON_RSP_OK;
            Uint8Type userProfileIndex      = AT_INVALID_DEVICE_ID;
            Uint8Type headsetProfileIndex   = AT_INVALID_DEVICE_ID;

            /* get active user profile index */
            const ERMUserProfileType * user             = ERM_pGetActiveUserProfile();
            /* get active headset profile index */
            const ERMPlayerHeadsetProfileType * headset = ERM_pGetActiveHeadsetProfile();

            if( user == NULL || headset == NULL )
            {
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }
            userProfileIndex    = ERM_u8GetUserProfileIndexForDeviceId(user->deviceIndex);
            headsetProfileIndex = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(headset->deviceIndex);

            if( userProfileIndex == AT_INVALID_DEVICE_ID || headsetProfileIndex == AT_INVALID_DEVICE_ID )
            {
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }

            /* associate headset profile with user profile */
            rsp = ERM_rcSetUserAssociatedHeadset_InEeprom(
                                                    _APP_DEVICE_vOnChangeAssociatedHeadset_Cb,
                                                    userProfileIndex,
                                                    headsetProfileIndex);
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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingChangeSortingOrderForNewUser     */
/**
    \brief      handle pending function to change the sorting order for new user
                depending on the country(NAR-> given name first OR rest of world starting with surname)

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingChangeSortingOrderForNewUser(const AppDeviceChangePBOrder * pChange )
{
    SuccessFailType result = SUCCESS;

    APP_COMMON_enResponseCode rspCode   = APP_COMMON_RSP_OK;

    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_PB_ORDER_FOR_NEW_USER_NOT_SENT:
        {
            /* change PB sorting order depending on the coding value */
            rspCode = APP_PHONEBOOK_rcSetPbSortOrder(_APP_DEVICE_vOnChangePBSortingOrder_Cb,
                                                     AT_PB_SORTORDER_FIRSTLAST,
                                                     BT_TRUE);
            /* check result code */
            switch( rspCode )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for response */
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:
                    /* try again later */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                    break;
                default:
                    _APP_DEVICE_vSendResultReleaseContext(rspCode, __LINE__);
                    break;
            }
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingAssociatePlayerToUser           */
/**
    \brief      handle pending function to associate currently active player
                to currently active user profile

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingAssociatePlayerToUser(const AppDeviceChangeAssociatedPlayerType * pChange )
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_ASSOC_PLAYER_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp   = APP_COMMON_RSP_OK;
            Uint8Type userProfileIndex      = AT_INVALID_DEVICE_ID;
            Uint8Type playerProfileIndex    = AT_INVALID_DEVICE_ID;

            /* get active user profile index */
            const ERMUserProfileType * user             = ERM_pGetActiveUserProfile();
            /* get active headset profile index */
            const ERMPlayerHeadsetProfileType * player  = ERM_pGetActivePlayerProfile();

            if( user == NULL || player == NULL )
            {
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }
            userProfileIndex    = ERM_u8GetUserProfileIndexForDeviceId(user->deviceIndex);
            playerProfileIndex  = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(player->deviceIndex);

            if( userProfileIndex == AT_INVALID_DEVICE_ID || playerProfileIndex == AT_INVALID_DEVICE_ID )
            {
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }

            /* associate headset profile with user profile */
            rsp = ERM_rcSetUserAssociatedPlayer_InEeprom(
                                                    _APP_DEVICE_vOnChangeAssociatedPlayer_Cb,
                                                    userProfileIndex,
                                                    playerProfileIndex,BT_TRUE);
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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles           */
/**
    \brief      handle pending function to delete a recently deleted player
                in all user profiles that contain this player

    \param      
                

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles()
{
    Uint8Type deviceId = gAppDeviceContext.updatedeletedPlayerinProfilesDeviceId;
    Uint8Type counter = 0;

    for (counter=0; counter < ERM_MAX_NUM_USER_PROFILES; counter++)
    {
        Uint8Type profileId = ERM_GetLastActiveProfileId( counter,
                                                      APP_DEVICE_enTYPE_PHONE);
        const ERMUserProfileType *pUserProfile = ERM_pGetUserProfile(profileId);
        
        if (NULL == pUserProfile)
        {
            DEBUG_TEXT(ERM_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles(actual pUserProfile empty, finished ");
            
            if (gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer == 0)
            {
                DEBUG_TEXT(ERM_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer == 0 release context");
            }
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
            break;
        }       

        if (pUserProfile->playerIndex == deviceId)
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_ERROR_BUSY;
            
            DEBUG_VALUE2(ERM_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles: delete in userprofile with deviceindex %d the playerdeviceId %d",
             pUserProfile->deviceIndex , deviceId);

            rsp = ERM_rcSetUserAssociatedPlayer_InEeprom(
                                                    _APP_DEVICE_vOnChangeAssociatedPlayerAfterDeletedPlayer_Cb,
                                                    pUserProfile->deviceIndex,
                                                    AT_INVALID_DEVICE_ID,BT_FALSE);
            /* check result code */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for response */
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:
                    /* try again later */
                    DEBUG_TEXT(ERM_SWI_TRC, MOD_APP_DEVICE,"_APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles: busy: try again");
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                    
                    break;
                default:
                    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                    break;
            }
            break;
        }
    }    
    
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingChangeRingtoneMode           */
/**
    \brief      handle pending function to associate new ringtone mode
                ( alternate playing of ringtone and caller name )
                to currently active user profile

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingChangeRingtoneMode(const AppDeviceChangeRingtoneAlternateType * pChange )
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_RINGTONE_MODE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcSetRingtoneAlternate_InEeprom(
                                                                _APP_DEVICE_vOnChangeRingtoneMode_Cb,
                                                                (Uint8Type)pChange->profileIndex,
                                                                pChange->bAlternate);
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
            break;
        }
        case APP_DEVICE_CHANGE_RINGTONE_MODE_IN_CK5050_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcSetPlayCallerNameMode(
                                                     _APP_DEVICE_vOnChangeRingtoneMode_Cb,
                                                     pChange->bAlternate);
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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;



}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingInitCK5050ForUser           */
/**
    \brief      handle pending function to init CK5050 with profile parameters
                of recently connected user profile

    \param      pInit
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingInitCK5050ForUser(const AppDeviceInitCK5050ForUserType * pInit)
{
    SuccessFailType result = SUCCESS;
    switch( pInit->state )
    {
        case APP_DEVICE_INIT_CK5050_RINGTONE_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            /* get active user profile and its ringtone */
            const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
            if( NULL == profile )
            {   /* no active user profile */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }
            rsp = ERM_eSetActiveDeviceRingtone(_APP_DEVICE_vOnInitCK5050ForUser_Cb,
                                               profile->ringtone);
            /* check result code */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for response */

                    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "PendingInitCK5050ForUser: ringtone %d",
                                profile->ringtone);
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:
                    /* try again later */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                    break;
                default:
                    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                    break;
            }
            break;
        }
        case APP_DEVICE_INIT_CK5050_ALTERNATE_RINGTONE_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            /* get active user profile and its ringtone */
            const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
            if( NULL == profile )
            {   /* no active user profile */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                return FAIL;
            }
            rsp = ERM_eSetActiveDeviceRingtoneAlternate(_APP_DEVICE_vOnInitCK5050ForUser_Cb,
                                                        profile->ringtone_alternating);
            /* check result code */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for response */
                     DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"PendingInitCK5050ForUser: ringtoneAlt. %d", 
                                  profile->ringtone_alternating);
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:
                    /* try again later */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                    break;
                default:
                    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                    break;
            }
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingSetPbCompletelyLoaded           */
/**
    \brief      handle pending function to save that phonebook of currently
                active user has been comletely loaded

    \param      pSet
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingSetPbCompletelyLoaded(const AppDeviceSetPbCompletelyLoadedType * pSet)
{
    SuccessFailType result = SUCCESS;
    switch( pSet->state )
    {
        case APP_DEVICE_SET_PB_COMPLETELY_LOADED_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;

            /* get active user profile */
            const ERMUserProfileType * profile = ERM_pGetUserProfile((Uint8Type)pSet->profileIndex);
            if( NULL == profile )
            {

                DEBUG_TEXT(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "Cannot save pb compl. loaded cause of no active user");
                _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
            }

            rsp = ERM_rcSetPbCompletelyLoaded_InEeprom(_APP_DEVICE_vOnSetPbCompletelyLoaded_Cb,
                                                       (Uint8Type)pSet->profileIndex);

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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingChangeAuthMode           */
/**
    \brief      handle pending function to change authentication mode of currently
                active player / headset

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingChangeAuthMode(const AppDeviceChangeAuthModeType * pChange)
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_AUTH_MODE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            switch( pChange->profileType )
            {
                case APP_DEVICE_enTYPE_HEADSET:
                    rsp = ERM_rcSetHeadsetAuthMode_InEeprom(
                                                   _APP_DEVICE_vOnChangeAuthMode_Cb,
                                                   pChange->profileIdx,
                                                   pChange->bAuthNeeded);
                    break;
                case APP_DEVICE_enTYPE_PLAYER:
                    rsp = ERM_rcSetPlayerAuthMode_InEeprom(
                                                   _APP_DEVICE_vOnChangeAuthMode_Cb,
                                                   pChange->profileIdx,
                                                   pChange->bAuthNeeded);
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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingChangeDeviceName           */
/**
    \brief      handle pending function to change device name of currently
                active player / headset / phone

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingChangeDeviceName(AppDeviceChangeDeviceNameType * pChange )
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_DEVICE_NAME_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcChangeDeviceName_InEeprom(
                                                                _APP_DEVICE_vOnChangeDeviceName_Cb,
                                                                pChange->deviceId,
                                                                pChange->name,
                                                                pChange->nameLen);
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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingChangeRingtone           */
/**
    \brief      handle pending function to change ringtone of currently
                active user

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingChangeRingtone(const AppDeviceChangeRingtoneType * pChange)
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_RINGTONE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcSetRingtone_InEeprom(
                                                                _APP_DEVICE_vOnChangeRingtone_Cb,
                                                                (Uint8Type)pChange->profileIndex,
                                                                pChange->ringtone);
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
            break;
        }
        case APP_DEVICE_CHANGE_RINGTONE_IN_CK5050_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_eSetActiveDeviceRingtone(
                                                     _APP_DEVICE_vOnChangeRingtone_Cb,
                                                     pChange->ringtone);
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
            break;
        }
        case APP_DEVICE_UPDATE_RINGTONE_MODE_IN_CK5050_NOT_SENT:
        {
            // if alternate playing of ringtone and caller name is enable for user profile
            // we need to set it here again to make sure the correct time / length settings
            // are set for the currently chosen ringtone
            const ERMUserProfileType * userProfile = ERM_pGetActiveUserProfile();
            if( userProfile != NULL &&
                BT_TRUE == userProfile->ringtone_alternating)
            {
                APP_COMMON_enResponseCode rsp = ERM_rcSetPlayCallerNameMode(
                                                     _APP_DEVICE_vOnChangeRingtone_Cb,
                                                     userProfile->ringtone_alternating);
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
                break;
            }
            else
            {   // nothing to be done here, so function is finished.
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
            }
            break;
        }

        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingChangeMailbox           */
/**
    \brief      handle pending function to change mailbox number of currently
                active user

    \param      pChange
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingChangeMailbox(const AppDeviceChangeMailboxType * pChange)
{
    SuccessFailType result = SUCCESS;
    switch( pChange->state )
    {
        case APP_DEVICE_CHANGE_MAILBOX_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcSetVoiceMailboxNumber_InEeprom(
                                                                _APP_DEVICE_vOnChangeMailboxInEeprom_Cb,
                                                                (Uint8Type)pChange->profileIndex,
                                                                pChange->number,
                                                                pChange->length);
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
            break;
        }
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingDeleteProfile           */
/**
    \brief      handle pending function to delete a profile

    \param      pDel
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingDeleteProfile(const AppDeviceDeleteProfileType *pDel)
{
    // TODO: Add handling to delete associations of a deleted profile / device
    // and a user profile, this is relevant only when a headset or player
    // profile is deleted and one or more user profiles use the deleted profile
    // as associated headset or player
    SuccessFailType result = SUCCESS;
    switch(pDel->state)
    {
        case APP_DEVICE_DELETE_PROFILE_IN_EEPROM_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_ERROR;

            /* delete profile depending on deviceType */
            switch( pDel->deviceType )
            {
                case APP_DEVICE_enTYPE_PHONE:
                    rsp = ERM_rcDeleteUserProfile_InEeprom(
                                          _APP_DEVICE_vOnDeleteProfileInEeprom_Cb,
                                          pDel->profileIndex );
                    break;
                case APP_DEVICE_enTYPE_HEADSET:
                    rsp = ERM_rcDeleteHeadsetProfile_InEeprom(
                                          _APP_DEVICE_vOnDeleteProfileInEeprom_Cb,
                                          pDel->profileIndex);
                    break;
                case APP_DEVICE_enTYPE_PLAYER:
                    rsp = ERM_rcDeletePlayerProfile_InEeprom(
                                           _APP_DEVICE_vOnDeleteProfileInEeprom_Cb,
                                           pDel->profileIndex);
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
            break;
        }
        case APP_DEVICE_DELETE_DEVICE_PAIRING_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcDeleteDevice(
                                                _APP_DEVICE_vOnDeleteDevice_Cb,
                                                pDel->deviceId);
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    
                    /* if player device was deleted update pther profiles (delte this player in other profiles) */
                    if (APP_DEVICE_enTYPE_PLAYER == pDel->deviceType)
                    {
                        _APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(pDel->deviceId);
                    }
                    else if ((APP_DEVICE_enTYPE_PHONE == pDel->deviceType) && (ERM_pGetUserProfile_DevId(pDel->deviceId)->playerIndex != AT_INVALID_DEVICE_ID))
                    {
                        _APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(pDel->deviceId);
                    }
                    
                    /* wait for response */
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:
                    /* try again later */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                    break;
                default:
                    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_DELETE_DEVICE_PAIRING_SENT:
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingDeleteAllProfiles                */
/**
    \brief      handle pending function to delete all profiles

    \param      pDel
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingDeleteAllProfiles(const AppDeviceDeleteAllProfilesType *pDelAll)
{
    SuccessFailType result = SUCCESS;
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_ERROR;


    DEBUG_VALUE2(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "DelAllProfs state %x type %x", pDelAll->state, pDelAll->deviceType);

    switch(pDelAll->state)
    {
        case APP_DEVICE_DELETE_ALL_PROFILES_IN_EEPROM_NOT_SENT:
        {
            /* delete profile depending on deviceType */
            switch( pDelAll->deviceType )
            {
                case APP_DEVICE_enTYPE_PHONE:
                {
                    if (pDelAll->profileIndex < ERM_MAX_NUM_USER_PROFILES)
                    {
                        const ERMUserProfileType * profile = ERM_pGetUserProfile((Uint8Type)pDelAll->profileIndex);
                        if( (ERM_MAX_PHYSICAL_DEVICE_IDX < profile->deviceIndex) || (NULL == profile) )
                        {
                            /* no user profile at this position available */
                            gAppDeviceContext.fct.delAllProfiles.profileIndex++;
                            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
                            return SUCCESS;
                        }
                        else
                        {   /* profile exists */
                            /* It exits a profile and readout the physical device Id from the static
                               table to delete the profile on the CK5050+ */
                            gAppDeviceContext.fct.delAllProfiles.deviceId = profile->deviceIndex;

                            DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "Delete user profile %d", pDelAll->profileIndex);
                            rsp = ERM_rcDeleteUserProfile_InEeprom(
                                              _APP_DEVICE_vOnDeleteAllProfilesInEeprom_Cb,
                                              pDelAll->profileIndex );
                        }
                    }
                    else
                    {
                        /* arrive max user profile and jump to next state */
                        rsp = APP_COMMON_RSP_OK;
                        gAppDeviceContext.fct.delAllProfiles.profileIndex = 0;
                        gAppDeviceContext.fct.delAllProfiles.deviceType   = APP_DEVICE_enTYPE_HEADSET;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
                    }
                    break;
                }
                case APP_DEVICE_enTYPE_PLAYER:
                case APP_DEVICE_enTYPE_HEADSET:
                {
                    if (pDelAll->profileIndex < ERM_MAX_NUM_PLAYER_HS_TABLE)
                    {
                        const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile((Uint8Type)pDelAll->profileIndex);

                        if ( (ERM_MAX_PHYSICAL_DEVICE_IDX < profile->deviceIndex) || (NULL == profile) )
                        {
                            /* no headset or media player profile at this position available */
                            gAppDeviceContext.fct.delAllProfiles.profileIndex++;
                            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(5), 0);
                            return SUCCESS;
                        }
                        else
                        {
                            gAppDeviceContext.fct.delAllProfiles.deviceId = profile->deviceIndex;
                            gAppDeviceContext.fct.delAllProfiles.deviceType = profile->deviceType;
                            if (APP_DEVICE_enTYPE_HEADSET == profile->deviceType)
                            {   /* headset profile exists */

                                DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "Delete headset profile %d", pDelAll->profileIndex);
                                rsp = ERM_rcDeleteHeadsetProfile_InEeprom(
                                                _APP_DEVICE_vOnDeleteAllProfilesInEeprom_Cb,
                                                pDelAll->profileIndex);
                            }
                            else /* if (APP_DEVICE_enTYPE_PLAYER == profile->deviceType) */
                            {
                                /* media player profile exists */

                                DEBUG_VALUE1(APP_DEVICE_SWI_TRC,MOD_APP_DEVICE, "Delete player profile %d", pDelAll->profileIndex);
                                rsp = ERM_rcDeletePlayerProfile_InEeprom(
                                                _APP_DEVICE_vOnDeleteAllProfilesInEeprom_Cb,
                                                pDelAll->profileIndex);
                            }
                        }
                    }
                    else
                    {
                        /* arrive max user profile and jump to next state */
                        rsp = APP_COMMON_RSP_OK;
                        gAppDeviceContext.fct.delAllProfiles.profileIndex = 0;
                        APP_SD_vOnAppDeviceNotification(APP_DEVICE_NOTIFY_ALL_USER_PROFILE_DELETED);    //
                        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
                    }
                    break;
                }
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
            break;
        }
        case APP_DEVICE_DELETE_ALL_DEVICE_PAIRING_NOT_SENT:
        {
            APP_COMMON_enResponseCode rsp = ERM_rcDeleteDevice(
                                                _APP_DEVICE_vOnDeleteAllDevice_Cb,
                                                pDelAll->deviceId);
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
                    return FAIL;
            }
            break;
        }
        case APP_DEVICE_DELETE_ALL_DEVICE_PAIRING_SENT:
        default:
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
    }
    return result;
}



/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnDeleteAllProfilesInEeprom_Cb                  */
/**
    \brief      callback indicating that request to deleze a profilefrom eeprom
                has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnDeleteAllProfilesInEeprom_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        if( APP_DEVICE_DELETE_ALL_PROFILES_IN_EEPROM_NOT_SENT == gAppDeviceContext.fct.delAllProfiles.state )
        {
            /* successfully finished deleting profile from eeprom */
            switch(gAppDeviceContext.fct.delAllProfiles.deviceType)
            {
                case APP_DEVICE_enTYPE_PHONE:
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_USER_PROFILE_DELETED);
                    break;
                case APP_DEVICE_enTYPE_HEADSET:
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_HEADSET_PROFILE_DELETED);
                    break;
                case APP_DEVICE_enTYPE_PLAYER:
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_PLAYER_PROFILE_DELETED);
                    break;
                default:
                    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                    break;
            }
            gAppDeviceContext.fct.delAllProfiles.state = APP_DEVICE_DELETE_ALL_DEVICE_PAIRING_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
        }
        else
        {
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
        }
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnDeleteAllDevice_Cb                         */
/**
    \brief      callback indicating that request to delete a device
                has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnDeleteAllDevice_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        if (( APP_DEVICE_enTYPE_HEADSET == gAppDeviceContext.fct.delAllProfiles.deviceType) &&
            ( ERM_MAX_NUM_PLAYER_HS_TABLE == gAppDeviceContext.fct.delAllProfiles.profileIndex))
        {
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        }
        else
        {
            /* increase the profile index and wait of PPDE:END to set the next state */
            gAppDeviceContext.fct.delAllProfiles.profileIndex++;
        }
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }

}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingSaveBtName           */
/**
    \brief      handle pending function to write BT Name to CK5050+ and EEPROM

    \param      pDel
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingSaveBtName(const AppDeviceSaveBtNameType *pBtName)
{
    SuccessFailType result          = SUCCESS;
    APP_COMMON_enResponseCode rsp   = APP_COMMON_RSP_ERROR;

    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SaveBtName: state %d", pBtName->state);
    switch(pBtName->state)
    {
        case APP_DEVICE_WRITE_BT_NAME_TO_EEPROM:
        {
            EEDeviceControlBlockType *pEEApp = (EEDeviceControlBlockType *)getEEAppTaskControl();
            (void)EEPROM_sfRamWriteOnly(
                                    (Uint8Type*)gAppDeviceContext.fct.saveBtName.btName,
                                    EEPROM_LAYOUT_ENUM_UniversalPreparationForMobileTelephone);
            rsp = APP_COMMON_RSP_OK;
            gAppDeviceContext.fct.saveBtName.state = APP_DEVICE_WRITE_BT_NAME_TO_CK5050P;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
            break;
        }
        case APP_DEVICE_WRITE_BT_NAME_TO_CK5050P:
        {
            rsp = ERM_sFSetUniversalPreparationForMobileTelephone(_APP_DEVICE_vOnSaveBtName_Cb,
                                        					pBtName->btName,
                                        					pBtName->length);
            break;
        }
        default:
        {
            rsp   = APP_COMMON_RSP_ERROR;
            break;
        }
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

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnSaveBtName_Cb                  */
/**
    \brief      callback indicating that request to add an active profile to
                list has been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnSaveBtName_Cb(APP_COMMON_enResponseCode rsp )
{
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnAddActiveProfileToEeprom_Cb                  */
/**
    \brief      callback indicating that request to add an active profile to
                list has been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnAddActiveProfileToEeprom_Cb(APP_COMMON_enResponseCode response )
{
    if( AT_INVALID_DEVICE_ID == gAppDeviceContext.newActPhoneProfileIdx  &&
        AT_INVALID_DEVICE_ID == gAppDeviceContext.newActPlayerProfileIdx &&
        AT_INVALID_DEVICE_ID == gAppDeviceContext.newActHeadsetProfileIdx)
    {
        gAppDeviceContext.updateActiveProfilePending = BT_FALSE;
    }
    /* we are finished */
    _APP_DEVICE_vSendResultReleaseContext(response, __LINE__);
}



/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnAddProfileToEeprom_Cb                  */
/**
    \brief      callback indicating that request to add a new profile and save
                it in eeprom has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnAddProfileToEeprom_Cb(APP_COMMON_enResponseCode rsp)
{   /* nothing more to do, forward result to calling app */

    if( APP_COMMON_RSP_OK == rsp )
    {
        switch(gAppDeviceContext.fct.addProfile.deviceType)
        {
            case APP_DEVICE_enTYPE_PHONE:
                if(gAppDeviceContext.fct.addProfile.deviceId == ERM_u8GetActivePhone())
                {
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_USER_PROFILE_ADDED);
    
                    // CK5050 needs to be initialized for the new user profile so
                    // e.g. default ringtone is set from the beginning
                    // init ck5050 depending on user profile settings, e.g. set // ringtone
                    // SUSANNE : #3076 we must differ whether there is a call in progress or not
                    // some settings like ringtone are not possible during call
                    //gAppDeviceContext.initCK5050ForUserProfilePending = BT_TRUE;
                    if( BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
                    {
                        if( _APP_DEVICE_sfInitCK5050ForUserProfile(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                        {
                            gAppDeviceContext.initCK5050ForUserProfilePending = BT_TRUE;
                        }
                    }
                    else
                    {   // set ringtone e.g. is not possible during call
                        gAppDeviceContext.initCK5050ForUserProfilePendingDueToCallTakeOver = BT_TRUE;
                    }
                }
                else
                {
                    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OnAddProfToEep_Cb: devT PHONE added, but not active! so we do no update over bap");
                }
                // variable will be checked immeadiately due to the call to
                // _APP_DEVICE_vSendResultReleaseContext
                break;
            case APP_DEVICE_enTYPE_PLAYER:
                /* notify all apps about new profile */
                _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_PLAYER_PROFILE_ADDED);
                _APP_DEVICE_vHandlePlayerProfileAdded();
                break;
            case APP_DEVICE_enTYPE_HEADSET:
                _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_HEADSET_PROFILE_ADDED);
                break;
            default:

                DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OnAddProfToEep_Cb: invalid devT %d",
                                gAppDeviceContext.fct.addProfile.deviceType);
                break;
        }
    }
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

void _APP_DEVICE_vHandlePlayerProfileAdded()
{
    BooleanType playerEnabled   = BT_TRUE;
    Uint8Type profileId         = AT_INVALID_DEVICE_ID;
    const ERMPlayerHeadsetProfileType *pProfile = NULL;


    /* try to start all services, this is needed cause some player do not start all their services */
    /* !!! code was moved from APP_MDI_C1.c to here */

    DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HandlePlayerProfileAdded" );

    profileId = APP_DEVICE_u8GetAddedProfileId(APP_DEVICE_enTYPE_PLAYER);
    pProfile = ERM_pGetPlayerHeadsetProfile(profileId);
    EEPROM_LAYOUT_vGetPlayerEnabled(&playerEnabled);

    if((pProfile != NULL) && (BT_TRUE == playerEnabled))
    {
        APP_COMMON_enResponseCode res = APP_DEVICE_rcActivateMdiDevice(NULL, pProfile->deviceIndex);

        switch(res)
        {
            case APP_COMMON_RSP_OK:
                /* in progress */
                break;
            case APP_COMMON_RSP_ERROR_BUSY:
                /* check if all the supported services are already started, else set pending reminder */

                gAppDeviceContext.actPlayerPending.deviceId = pProfile->deviceIndex;
                gAppDeviceContext.actPlayerPending.cb = NULL;
                break;
            default:
            {

                DEBUG_VALUE1( APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "OnAppDevNot: ActMdiDev %d failed",
                              pProfile->deviceIndex);
            }
            break;
        }
    }
    else
    {

        DEBUG_VALUE2( APP_MDI_SWI_ERR, MOD_APP_MDI, "OnAppDevNot: invalid %d or no player enabled %d",
                      profileId, playerEnabled );
    }

}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnDeleteProfileInEeprom_Cb                  */
/**
    \brief      callback indicating that request to deleze a profilefrom eeprom
                has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnDeleteProfileInEeprom_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        if( gAppDeviceContext.fct.delProfile.state == APP_DEVICE_DELETE_PROFILE_IN_EEPROM_NOT_SENT )
        {
            /* successfully finished deleting profile from eeprom */
            if( APP_COMMON_RSP_OK == rsp && APP_DEVICE_enTYPE_PHONE == gAppDeviceContext.fct.delProfile.deviceType )
            {
                _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_USER_PROFILE_DELETED);
            }

            gAppDeviceContext.fct.delProfile.state = APP_DEVICE_DELETE_DEVICE_PAIRING_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
        }
        else
        {
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
        }
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnAddMissingProfile_Cb                  */
/**
    \brief      callback indicating that request to add a missing profile to eeprom
                has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnAddMissingProfile_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        switch( gAppDeviceContext.fct.addMissProfile.state )
        {
            case APP_DEVICE_DELETE_OLDEST_PROFILE_IN_EEPROM_NOT_SENT:
                gAppDeviceContext.fct.addMissProfile.state = APP_DEVICE_DELETE_OLDEST_PROFILE_PAIRING_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
                break;
            case APP_DEVICE_DELETE_OLDEST_PROFILE_PAIRING_NOT_SENT:
                /* deleted user profile from eeprom */
                if( gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE )
                {
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_USER_PROFILE_DELETED);
                }
                gAppDeviceContext.fct.addMissProfile.state = APP_DEVICE_ADD_NEW_PROFILE_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
                break;
            default:
                 // if profile was added, send notification
                if( gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE )
                {
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_USER_PROFILE_ADDED);
                }
                else if( gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE )
                {
                    _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_PLAYER_PROFILE_ADDED);
                }
                _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                break;
        }
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeAssociatedPlayer_Cb                  */
/**
    \brief      callback indicating that request to associate a player to a
                user profile in eeprom has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeAssociatedPlayer_Cb(APP_COMMON_enResponseCode rsp)
{
    /* nothing more to be done */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeAssociatedPlayerAfterDeletedPlayer_Cb  */
/**
    \brief      callback indicating that request to associate a player to a
                user profile in eeprom has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeAssociatedPlayerAfterDeletedPlayer_Cb(APP_COMMON_enResponseCode rsp)
{
    /* nothing more to be done */
    
    gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer--;
    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "_APP_DEVICE_vOnChangeAssociatedPlayerAfterDeletedPlayer_Cb: numberOfProfiles to be updated %d ",
                gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer);


   _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    
    if (gAppDeviceContext.numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer > 0)
    {
       (void)_APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(gAppDeviceContext.updatedeletedPlayerinProfilesDeviceId); 
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangePBSortingOrder_Cb                      */
/**
    \brief      callback indicating that request to change the phone book order
                has been processed with result given with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangePBSortingOrder_Cb(APP_COMMON_enResponseCode rsp)
{
    /* nothing more to be done */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeAssociatedHeadset_Cb                  */
/**
    \brief      callback indicating that request to associate a headset to a
                user profile in eeprom has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeAssociatedHeadset_Cb(APP_COMMON_enResponseCode rsp)
{
    /* nothing more to be done */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeRingtoneMode_Cb                  */
/**
    \brief      callback indicating that request to change ringtone mode of a
                user profile in eeprom has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeRingtoneMode_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {   /* check what to do next */
        switch( gAppDeviceContext.fct.changeRingtoneMode.state )
        {
            case APP_DEVICE_CHANGE_RINGTONE_MODE_IN_EEPROM_NOT_SENT:
                gAppDeviceContext.fct.changeRingtoneMode.state = APP_DEVICE_CHANGE_RINGTONE_MODE_IN_CK5050_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
                break;
            default:
                _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                break;
        }
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnSetPbCompletelyLoaded_Cb                  */
/**
    \brief      callback indicating that request to save in eeprom that
                phonebook has been completely loaded of a
                user profile in eeprom has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnSetPbCompletelyLoaded_Cb(APP_COMMON_enResponseCode rsp)
{
    /* nothing more to be done */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnInitCK5050ForUser_Cb                         */
/**
    \brief      callback indicating that request to initialize
                user profile in eeprom has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnInitCK5050ForUser_Cb(APP_COMMON_enResponseCode rsp)
{
    // check if we got an error or not.
    // if we were not able to set the ringtone, we assume that there is a call in progress
    // even if the first check for AllCallsIdle was negative.
    if(APP_COMMON_RSP_OK != rsp)
    {
        // release context and save that we have to try again when all calls go back to idle mode
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
        // flag will be checked in vOnAppPhoneNotification when call state changed and we are idle again
        gAppDeviceContext.initCK5050ForUserProfilePendingDueToCallTakeOver = BT_TRUE;
        DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "InitCk_CB: err st. %d, set idle retr. flag",
                                                gAppDeviceContext.fct.initCk5050ForUser.state);
    }
    else
    {
        if (gAppDeviceContext.fct.initCk5050ForUser.state == APP_DEVICE_INIT_CK5050_RINGTONE_NOT_SENT)
        {
            gAppDeviceContext.fct.initCk5050ForUser.state = APP_DEVICE_INIT_CK5050_ALTERNATE_RINGTONE_NOT_SENT;
            /* reactivate the device function pending */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);
        }
        else if(gAppDeviceContext.fct.initCk5050ForUser.state == APP_DEVICE_INIT_CK5050_ALTERNATE_RINGTONE_NOT_SENT)
        {
            /* nothing more to be done */
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeAuthMode_Cb                         */
/**
    \brief      callback indicating that request to authentication mode of
                profile has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeAuthMode_Cb(APP_COMMON_enResponseCode rsp)
{
    /* nothing more to be done */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeDeviceName_Cb                         */
/**
    \brief      callback indicating that request to change device name of
                profile has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeDeviceName_Cb(APP_COMMON_enResponseCode rsp)
{
    _APP_DEVICE_vNotifyApps (APP_DEVICE_NOTIFY_USER_PROFILE_NAME_SAVED);
    /* nothing more to be done */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeRingtone_Cb                         */
/**
    \brief      callback indicating that request to change ringtone of
                profile has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeRingtone_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {   /* check what to do next */
        switch( gAppDeviceContext.fct.changeRingtone.state )
        {
            case APP_DEVICE_CHANGE_RINGTONE_IN_EEPROM_NOT_SENT:
                gAppDeviceContext.fct.changeRingtone.state = APP_DEVICE_CHANGE_RINGTONE_IN_CK5050_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
                break;
            case APP_DEVICE_CHANGE_RINGTONE_IN_CK5050_NOT_SENT:
                gAppDeviceContext.fct.changeRingtone.state = APP_DEVICE_UPDATE_RINGTONE_MODE_IN_CK5050_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(300), 0);
                break;
            default:
                _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
                break;
        }
    }
    else
    {
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnChangeMailboxInEeprom_Cb                         */
/**
    \brief      callback indicating that request to change mailbox of
                profile has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnChangeMailboxInEeprom_Cb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        _APP_DEVICE_vNotifyApps(APP_DEVICE_NOTIFY_MAILBOX_NUMBER_CHANGED);
    }

    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnDeleteDevice_Cb                         */
/**
    \brief      callback indicating that request to delete a device
                has been processed with result given
                with response

    \param      rsp
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnDeleteDevice_Cb(APP_COMMON_enResponseCode rsp)
{   /* nothing more to do, forward result to calling app */
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);

    /* TODO: wait for notification that device pairing has been deleted,
     * then ReleaseContext */
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vAddMissingUserProfileImplicitlyBySD              */
/**
    \brief      if a new phone connects and there is no existing user profile
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vAddMissingUserProfileImplicitlyBySD()
{
    /* There is an instrument cluster, but also there was a call disturbing the user to create
       a profile for a new phone. So we need to create the profile implicitly for the user to be able
       to show all the call states etc in SD and HU */
    if( APP_COMMON_RSP_ERROR_BUSY == _APP_DEVICE_sfAddMissingUserProfile(NULL))
    {
        // make sure that we are not trigger the same function twice
        if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE)
        {
            gAppDeviceContext.addMissingUserProfilePending = BT_TRUE;
        }
    }

    gAppDeviceContext.addMissingPlayerProfilePending = BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vAddMissingPlayertProfileImplicitlyBySD              */
/**
    \brief      if a new player connects and there is no existing user profile
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vAddMissingPlayerProfileImplicitlyBySD()
{
    /* There is an instrument cluster, but also there was a call disturbing the user to create
       a profile for a new phone. So we need to create the profile implicitly for the user to be able
       to show all the call states etc in SD and HU */
    if( APP_COMMON_RSP_ERROR_BUSY == _APP_DEVICE_sfAddMissingPlayerProfile(NULL))
    {
        // make sure that we are not trigger the same function twice
        if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE)
        {
            gAppDeviceContext.addMissingPlayerProfilePending = BT_TRUE;
        }
    }    
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandleNoUserProfileAvailable                             */
/**
    \brief      if a new phone connects and there is not existing user profile
                and also no SD is available to trigger the user adding a
                profile, we need to add it on our own.
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vHandleNoUserProfileAvailable()
{
    if( BT_TRUE == APP_SD_bHapticalInterfaceAvailable())
    {   /* should be handled by SD correctly */

        DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HaeNoUserProfAvail: SD avail, skip");
        return;
    }


    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HaeNoUserProfAvail: no SD, add prof");
    /* no sd available, create user profile for active phone device */
    if( APP_COMMON_RSP_ERROR_BUSY == _APP_DEVICE_sfAddMissingUserProfile(NULL))
    {
        gAppDeviceContext.addMissingUserProfilePending = BT_TRUE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandleNoPlayerProfileAvailable                             */
/**
    \brief      if a new player connects and there is not existing player profile
                and also no SD is available to trigger the user adding a
                profile, we need to add it on our own.
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vHandleNoPlayerProfileAvailable()
{
    if( BT_TRUE == APP_SD_bHapticalInterfaceAvailable())
    {   /* should be handled by SD correctly */

        DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HaNoPlProfAvail: SD avail, skip");
        return;
    }


    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HaNoPlProfAvail: no SD, add prof");
    /* no sd available, create player profile for active player device */
    if( APP_COMMON_RSP_ERROR_BUSY == _APP_DEVICE_sfAddMissingPlayerProfile(NULL))
    {
        gAppDeviceContext.addMissingPlayerProfilePending = BT_TRUE;
    }

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandleNoHeadsetProfileAvailable                 */
/**
    \brief      if a new headset connects and there is not existing headset
                profile and also no SD is available to trigger the user
                adding a profile, we need to add it on our own.
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vHandleNoHeadsetProfileAvailable()
{
    if( BT_TRUE == APP_SD_bHapticalInterfaceAvailable())
    {   /* should be handled by SD correctly */

        DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HaNoHsProfAvail: SD avail, skip");
        return;
    }


    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HaNoHsProfAvail: no SD, but SKIP");
    /* just ignore this case for now */
}

/* Function    : APP_DEVICE_u8GetAddedProfileId                                      */
/**
    \brief      returns profile id of recently added profile of type deviceType

    \param      deviceType
                APP_DEVICE_DeviceType
                device type

    \return     Uint8Type
                profileId
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_DEVICE_u8GetAddedProfileId(APP_DEVICE_DeviceType deviceType)
{   /* returns last added profile id for given device type */
    AT_DeviceId deviceId = AT_INVALID_DEVICE_ID;
    Uint8Type  profileId = AT_INVALID_DEVICE_ID;

    switch(deviceType)
    {
        case APP_DEVICE_enTYPE_HEADSET:
        case APP_DEVICE_enTYPE_PLAYER:
            if( (gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_PROFILE) &&
                (gAppDeviceContext.fct.addProfile.deviceType == deviceType) )
            {
                deviceId = gAppDeviceContext.fct.addProfile.deviceId;
                profileId = ERM_u8GetPlayerHeadsetProfileId(deviceId);
            }
            else if(gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE)
            {
                deviceId = gAppDeviceContext.fct.addMissProfile.deviceId;
                profileId = ERM_u8GetPlayerHeadsetProfileId(deviceId);
            }
            else
            {

                DEBUG_VALUE4(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "GetAddedProfId: failed devT %d (fct %d, devT %d) %c",
                               deviceType, gAppDeviceContext.fct.common.fctType,
                               gAppDeviceContext.fct.addProfile.deviceType,
                                ' ');   // just a dummy parameter to able to use macro
            }
            break;
        case APP_DEVICE_enTYPE_PHONE:
            if( (gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_PROFILE ) &&
                (gAppDeviceContext.fct.addProfile.deviceType == deviceType) )
            {
                deviceId = gAppDeviceContext.fct.addProfile.deviceId;
                profileId = ERM_u8GetUserProfileId(deviceId);
            }
            else if(gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE)
            {
                deviceId = gAppDeviceContext.fct.addMissProfile.deviceId;
                profileId = ERM_u8GetUserProfileId(deviceId);
            }
            else
            {

                DEBUG_VALUE4( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "GetAddedProfId: failed devT %d (fct %d, devT %d) %c",
                               deviceType, gAppDeviceContext.fct.common.fctType,
                               gAppDeviceContext.fct.addProfile.deviceType,
                                ' ');   // just a dummy parameter to able to use macro
            }
            break;
        default:

            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "GetAddedProfId: inv. devType %d",
                           deviceType);
            break;
    }


    DEBUG_VALUE4(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetAddedProfId: devT %d devId %d -> profId %d %c",
                   deviceType, deviceId, profileId, ' '); // 4th is just a dummy

    return profileId;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandlePhoneStateChanged                             */
/**
    \brief      handles connection state change of phone
*/
/*----------------------------------------------------------------------------*/

void _APP_DEVICE_Profiles_vHandlePhoneStateChanged(const ERMNotificationType * pNotification)
{
    /* see if we have a new connected phone */
    AT_DeviceId deviceId = ERM_u8GetActivePhone();
    if( AT_INVALID_DEVICE_ID != deviceId )
    {
        /* get profile idx for deviceId and deviceType  */
        Uint8Type profileIdx = ERM_u8GetUserProfileIndexForDeviceId(deviceId);
        const ERMUserProfileType *profile = ERM_pGetUserProfile(profileIdx);



        if(profile != NULL)
        {
            /* we need to update the profile list */
            gAppDeviceContext.newActPhoneProfileIdx = profileIdx;
            if( _APP_DEVICE_sfUpdateErmActiveProfileTable(NULL) == APP_COMMON_RSP_ERROR_BUSY)
            {
                gAppDeviceContext.updateActiveProfilePending = BT_TRUE;
            }
            // init ck5050 depending on user profile settings, e.g. set // ringtone
            if( BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
            {
                if( _APP_DEVICE_sfInitCK5050ForUserProfile(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                {
                    gAppDeviceContext.initCK5050ForUserProfilePending = BT_TRUE;
                }
            }
            else
            {   // set ringtone e.g. is not possible during call
                gAppDeviceContext.initCK5050ForUserProfilePendingDueToCallTakeOver = BT_TRUE;
            }

            /* check if there is an active player profile. if yes, associate
             * the currently connected player to recently connected user profile */
            if( NULL != ERM_pGetActivePlayerProfile())
            {   /* try to associated player to user profile */
                if( _APP_DEVICE_rcAssociatePlayerToUser(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                {
                    gAppDeviceContext.associatePlayerToUserPending = BT_TRUE;
                }
            }
            /* check if there is an active headset profile. if yes, associate
             * the currently connected headset to the recently connected user profile */
            if( NULL != ERM_pGetActiveHeadsetProfile())
            {   /* try to associated headset to user profile */
                if( _APP_DEVICE_rcAssociateHeadsetToUser(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                {
                    gAppDeviceContext.associateHeadsetToUserPending = BT_TRUE;
                }
            }
        }
        else
        {
            /* active phone but no user profile available for this device                                           */
            /* first change the PB sorting order(NAR: given name first, rest of world surname first)                */
            /* Change the sorting order if differs from the default value by the coding, default LastName-FirstName */
			/* implemented by nikolov, RT:11046 */
            APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE SortingOrderCoded;

            SortingOrderCoded = APP_DIAG_GetDiag_PhonebookSortOrder();

            /* set the retry counter to zero, proceed 1 retries */
            gAppDeviceContext.PBSortingOrderRetryCounter = 0;

            if(enAppDiag_PHONEBOOK_SORTORDER_PRIO_FIRSTTNAME == SortingOrderCoded)
            {
                DEBUG_TEXT(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "HandlePhoneStateChanged: change PB sorting order for new user");

                if( _APP_DEVICE_vChangePBSortingOrder(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                {
                    gAppDeviceContext.changeSortingOrderForProfile = BT_TRUE;
                }
                /* notify the applications that new phone was connected */
            }
            _APP_DEVICE_vHandleNoUserProfileAvailable();
        }
    }
    else
    {
        /* no device connected */
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandlePlayerStateChanged                             */
/**
    \brief      handles connection state change of player
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Profiles_vHandlePlayerStateChanged(const ERMNotificationType * pNotification)
{
    /* see if we have a new connected player */
    AT_DeviceId deviceId = ERM_u8GetActivePlayer();
    if( AT_INVALID_DEVICE_ID != deviceId )
    {
        /* get profile idx for deviceId and deviceType  */
        Uint8Type profileIdx = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(deviceId);
        const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile(profileIdx);



        if(profile != NULL)
        {
            if(profile->deviceType == APP_DEVICE_enTYPE_PLAYER)
            {
                /* we need to update the profile list */
                gAppDeviceContext.newActPlayerProfileIdx = profileIdx;
                if( _APP_DEVICE_sfUpdateErmActiveProfileTable(NULL) == APP_COMMON_RSP_ERROR_BUSY)
                {
                    gAppDeviceContext.updateActiveProfilePending = BT_TRUE;
                }
                /* check if there is an active user profile. if yes, associate
                 * the currently connected player to this user profile */
                if( NULL != ERM_pGetActiveUserProfile())
                {   /* try to associated player to user profile */
                    if( _APP_DEVICE_rcAssociatePlayerToUser(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                    {
                        gAppDeviceContext.associatePlayerToUserPending = BT_TRUE;
                    }
                }
            }
        }
        else
        {
            /* active player but no profile available for this device */
            _APP_DEVICE_vHandleNoPlayerProfileAvailable();
        }
    }
}


/* End Of File APP_DEVICE_Profiles_C1.c */


