/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_DEVICE
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP and
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2011-06-03 08:53:24 +0200 (Fr, 03 Jun 2011) $
* $Rev: 20014 $
* $Author: schmidt $
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
#ifdef UHV_ACTIVATE_FEATURE_LED
#include "LED.h"
#endif
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
#include "APP_DEVICE_Connection.h"
#include "APP_DEVICE_E2P_Backup_R2FS.h"
#include "APP_DEVICE_AudioManager.h"

#include "APP_DEVICE_C1.id"
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
AppDeviceContext gAppDeviceContext;


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
static SuccessFailType _APP_DEVICE_sfHandlePendingFunction(void);

static BooleanType _APP_DEVICE_bUartRequiredForPendingFctId();

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
/* Function    : APP_DEVICE_sfInitApp                                         */
/**
    \brief      Callback function for initialization of App DEVICE

    \return     SuccessFailType
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_DEVICE_sfInitApp(void)
{
    (void)memset(&gAppDeviceContext, 0, sizeof(gAppDeviceContext));

    /* auto conn mode is set on by default by ERM */
    gAppDeviceContext.autoConnMode = BT_TRUE;


    gAppDeviceContext.newActPlayerProfileIdx    = AT_INVALID_DEVICE_ID;
    gAppDeviceContext.newActPhoneProfileIdx     = AT_INVALID_DEVICE_ID;
    gAppDeviceContext.newActHeadsetProfileIdx   = AT_INVALID_DEVICE_ID;
    gAppDeviceContext.actPlayerPending.deviceId = AT_INVALID_DEVICE_ID;
    gAppDeviceContext.actPlayerPending.cb       = NULL;
    gAppDeviceContext.deactPlayerPending        = AT_INVALID_DEVICE_ID;
    gAppDeviceContext.deativatePhoneServiceDeviceId = AT_INVALID_DEVICE_ID;

    APP_DEVICE_AudioManager_sfInitApp();

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vReleaseContext                                  */
/**
    \brief      function to reset current function context
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vReleaseContext()
{
    (void)memset(&gAppDeviceContext.fct, 0, sizeof(gAppDeviceContext.fct));
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vSendResultReleaseContext                                         */
/**
    \brief      function to notify result of last processed function to calling
                application and to release the function context

    \param      result
                APP_COMMON_enResponseCode
                result of the recently processed function

    \param      error_line
                Uint32Type
                if an error occurred, this line will identify where the error
                happened
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_enResponseCode result, Uint32Type error_line)
{
    if( result == APP_COMMON_RSP_OK)
    {

        DEBUG_VALUE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "RelContext: fctId %d result %d",
                        gAppDeviceContext.fct.common.fctType,
                        result );
    }
    else
    {

        DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "SendResRelCtx: result 0x%02x line %d",
                        result,
                        error_line );
    }

    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "RelContext: called in line %d",
                                                    error_line);

    if(gAppDeviceContext.fct.common.callBack)
    {
        gAppDeviceContext.fct.common.callBack(result);
    }

    switch( gAppDeviceContext.fct.common.fctType )
    {
        case APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE:
            gAppDeviceContext.addMissingUserProfilePending = BT_FALSE;
            break;
        case APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE:
            gAppDeviceContext.addMissingPlayerProfilePending = BT_FALSE;
            break;
    }

    /* first check if we need to release uart before resetting fctType */
    if(BT_TRUE == _APP_DEVICE_bUartRequiredForPendingFctId())
    {   // release uart if we had an error
        ERM_ReleaseUartForDevice();
    }


    _APP_DEVICE_vReleaseContext();


    /* now check if there is any internal stuff pending and needs to be
     * processed next */

    //if( gAppDeviceContext.enableAutoConnPending == BT_TRUE)
    //{
    //    APP_DEVICE_sfSetAutoConnectionMode( BT_TRUE, NULL);
    //    gAppDeviceContext.enableAutoConnPending = BT_FALSE;
    //}
    //else if( gAppDeviceContext.disableAutoConnPending == BT_TRUE)
    //{
    //    APP_DEVICE_sfSetAutoConnectionMode( BT_FALSE, NULL);
    //    gAppDeviceContext.disableAutoConnPending = BT_FALSE;
    //}
    if( gAppDeviceContext.updateActiveProfilePending == BT_TRUE )
    {
        (void)_APP_DEVICE_sfUpdateErmActiveProfileTable(NULL);
        /* pending will be reset when all 3 profile index are
         * AT_INVALID_DEVICE_ID */
    }
    else if( gAppDeviceContext.addMissingUserProfilePending == BT_TRUE )
    {
        (void)_APP_DEVICE_sfAddMissingUserProfile(NULL);
        gAppDeviceContext.addMissingUserProfilePending = BT_FALSE;
    }
    else if( gAppDeviceContext.addMissingPlayerProfilePending == BT_TRUE )
    {
        (void)_APP_DEVICE_sfAddMissingPlayerProfile(NULL);
        gAppDeviceContext.addMissingPlayerProfilePending = BT_FALSE;
    }
    else if( gAppDeviceContext.initCK5050ForUserProfilePending == BT_TRUE )
    {
        (void)_APP_DEVICE_sfInitCK5050ForUserProfile(NULL);
        gAppDeviceContext.initCK5050ForUserProfilePending = BT_FALSE;
    }
    else if( gAppDeviceContext.associatePlayerToUserPending == BT_TRUE )
    {
        (void)_APP_DEVICE_rcAssociatePlayerToUser(NULL);
        gAppDeviceContext.associatePlayerToUserPending = BT_FALSE;
    }
    else if( gAppDeviceContext.associateHeadsetToUserPending == BT_TRUE )
    {
        (void)_APP_DEVICE_rcAssociateHeadsetToUser(NULL);
        gAppDeviceContext.associateHeadsetToUserPending = BT_FALSE;
    }
    else if( gAppDeviceContext.changeSortingOrderForProfile == BT_TRUE)
    {
        (void)_APP_DEVICE_vChangePBSortingOrder(NULL);
        gAppDeviceContext.changeSortingOrderForProfile = BT_FALSE;
    }
    else if ( AT_INVALID_DEVICE_ID != gAppDeviceContext.actPlayerPending.deviceId )
    {
        APP_DEVICE_rcActivateMdiDevice( gAppDeviceContext.actPlayerPending.cb, 
                                        gAppDeviceContext.actPlayerPending.deviceId);
        gAppDeviceContext.actPlayerPending.deviceId = AT_INVALID_DEVICE_ID;
        gAppDeviceContext.actPlayerPending.cb = NULL;
    }
    else if ( AT_INVALID_DEVICE_ID != gAppDeviceContext.deactPlayerPending )
    {
        APP_DEVICE_rcDeactivateMdiDevice(NULL);
        gAppDeviceContext.deactPlayerPending = AT_INVALID_DEVICE_ID;
    }
    else if( gAppDeviceContext.stopRingtoneNonBlock == BT_TRUE )
    {
        (void)APP_DEVICE_rcStopRingtoneNonBlock();
        gAppDeviceContext.stopRingtoneNonBlock = BT_FALSE;
    }
    else if( gAppDeviceContext.startRingtoneNonBlock == BT_TRUE )
    {
        (void)APP_DEVICE_rcStartRingtoneNonBlock(gAppDeviceContext.ringtone);
        gAppDeviceContext.startRingtoneNonBlock = BT_FALSE;
    }
    else if( gAppDeviceContext.disconnectDevice == BT_TRUE )
    {
        (void)APP_DEVICE_rcDisconnectDevice(gAppDeviceContext.disconnectDeviceId);
        gAppDeviceContext.disconnectDevice = BT_FALSE;
    }
    else if( gAppDeviceContext.updateDeletedPlayerInProfiles == BT_TRUE)
    {
        (void)_APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(gAppDeviceContext.updatedeletedPlayerinProfilesDeviceId);
        gAppDeviceContext.updateDeletedPlayerInProfiles = BT_FALSE;
    }
    else if( gAppDeviceContext.deativatePhoneServiceDeviceId != AT_INVALID_DEVICE_ID)
    {
        (void)APP_DEVICE_sfDeactivatePhoneService(gAppDeviceContext.deativatePhoneServiceDeviceId, NULL);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vHandleEvents                                        */
/**
    \brief      Task Function to handle events for DEVICE Application

    \brief      Description:\n
                Waits for incoming events and if one arrives function \n
                reads Messages received From BAP-Dispatcher and \n
                AT_Cmd_Parser and handles them. \n

    \param      event
                EventMaskType
                Bitmask containing all unhandled eventsERM_REQ_AUTOCONN_ON

    \return     void
*/
/*----------------------------------------------------------------------------*/

void APP_DEVICE_vHandleEvents(EventMaskType event)
{
    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************
    if( event & evAppTaskDeviceFctPending )
    {
        (void)ClearEvent( evAppTaskDeviceFctPending );

        if((ERM_bIsStartupDone() == BT_TRUE) ||
           (gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_RESET_TO_FACTORY_SETTING) ||
           (gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_UPDATE_BACKUP_EEPROM)     ||
           (gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_UPDATE_ACTIVE_PROFILE_LIST) ||
           (gAppDeviceContext.fct.common.fctType == APP_DEVICE_ERM_ASSOCIATE_PLAYER_TO_USER))
        {
            (void)_APP_DEVICE_sfHandlePendingFunction();
        }
        else
        {
            /* we are not able to to request the UART in this state */

            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "HandleEvents: CK5050_SHUTDOWN, drop pending, fct %d",
                          gAppDeviceContext.fct.common.fctType);

            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
        }
    }
}

BooleanType _APP_DEVICE_bUartRequiredForPendingFctId()
{
    BooleanType bRequired = BT_FALSE;

    switch( gAppDeviceContext.fct.common.fctType )
    {
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
        case APP_DEVICE_DEACTIVATE_PLAYER:
        case APP_DEVICE_ACTIVATE_PLAYER:
        case APP_DEVICE_PHONE_SERVICE_DEACTIVATION:
            bRequired = BT_TRUE;
            break;

        default:
            break;
    }

    return bRequired;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_sfHandlePendingFunction                                        */
/**
    \brief      Decides what needs to be done next depending on the currently
                processed function

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/

SuccessFailType _APP_DEVICE_sfHandlePendingFunction( )
{
    SuccessFailType result  = SUCCESS;
    const SCI_ST* puart     = NULL;

    //For some fctIds we need to request the uart first
    // add all affected fctIds into function _APP_DEVICE_bUartRequiredForPendingFctId()
    if(BT_TRUE == _APP_DEVICE_bUartRequiredForPendingFctId())
    {
        puart = ERM_GetUartForDevice();
        if( NULL == puart )
        {
            //we need to retry getting the uart
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(ERM_UART_BUSY_RETRY_TIME), 0);
            return FAIL;
        }
    }
    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "HandlePendingFunction: fctType: %d",
                                     gAppDeviceContext.fct.common.fctType);
    switch( gAppDeviceContext.fct.common.fctType )
    {
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            result = _APP_DEVICE_sfHandlePendingActivate(puart, &gAppDeviceContext.fct.serviceAct);
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            result = _APP_DEVICE_sfHandlePendingMdActivate(puart, &gAppDeviceContext.fct.actPlayer);
            break;
        case APP_DEVICE_DEACTIVATE_PLAYER:
            result = _APP_DEVICE_sfHandlePendingMdDeactivate(puart, &gAppDeviceContext.fct.deactPlayer);
            break;
        case APP_DEVICE_DISCONNECT_DEVICE:
            gAppDeviceContext.fct.disconnectDevice.state = APP_DEVICE_DISCONNECT_DEVICE_NOT_SENT;
            result = _APP_DEVICE_sfHandlePendingDisconnectDevice();
            break;
        case APP_DEVICE_PHONE_SET_AUTO_CONN_MODE:
            result = _APP_DEVICE_sfHandlePendingSetAutoConnMode(&gAppDeviceContext.fct.autoConn);
            break;
        case APP_DEVICE_ERM_ADD_PROFILE:
            result = _APP_DEVICE_sfHandlePendingAddProfile(&gAppDeviceContext.fct.addProfile);
            break;
        case APP_DEVICE_ERM_DELETE_PROFILE:
            result = _APP_DEVICE_sfHandlePendingDeleteProfile(&gAppDeviceContext.fct.delProfile);
            break;
        case APP_DEVICE_ERM_CHANGE_MAILBOX_NUMBER:
            result = _APP_DEVICE_sfHandlePendingChangeMailbox(&gAppDeviceContext.fct.changeMailbox);
            break;
        case APP_DEVICE_ERM_UPDATE_ACTIVE_PROFILE_LIST:
            result = _APP_DEVICE_sfHandlePendingUpdateActiveProfileList(&gAppDeviceContext.fct.actProfile);
            break;
        case APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE:
            result = _APP_DEVICE_sfHandlePendingAddMissingUserProfile(&gAppDeviceContext.fct.addMissProfile);
            break;
        case APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE:
            result = _APP_DEVICE_sfHandlePendingAddMissingPlayerProfile(&gAppDeviceContext.fct.addMissProfile);
            break;
        case APP_DEVICE_ERM_CHANGE_RINGTONE:
            result = _APP_DEVICE_sfHandlePendingChangeRingtone(&gAppDeviceContext.fct.changeRingtone);
            break;
        case APP_DEVICE_ERM_CHANGE_RINGTONE_MODE:
            result = _APP_DEVICE_sfHandlePendingChangeRingtoneMode(&gAppDeviceContext.fct.changeRingtoneMode);
            break;
        case APP_DEVICE_ERM_INIT_CK5050_FOR_USER:
            result = _APP_DEVICE_sfHandlePendingInitCK5050ForUser(&gAppDeviceContext.fct.initCk5050ForUser);
            break;
        case APP_DEVICE_ERM_ASSOCIATE_HEADSET_TO_USER:
            result = _APP_DEVICE_sfHandlePendingAssociateHeadsetToUser(&gAppDeviceContext.fct.changeAssocHeadset);
            break;
        case APP_DEVICE_ERM_ASSOCIATE_PLAYER_TO_USER:
            result = _APP_DEVICE_sfHandlePendingAssociatePlayerToUser(&gAppDeviceContext.fct.changeAssocPlayer);
            break;
        case APP_DEVICE_ERM_CHANGE_AUTH_MODE:
            result = _APP_DEVICE_sfHandlePendingChangeAuthMode(&gAppDeviceContext.fct.changeAuthMode);
            break;
        case APP_DEVICE_ERM_SET_PB_COMPLETELY_LOADED:
            result = _APP_DEVICE_sfHandlePendingSetPbCompletelyLoaded(&gAppDeviceContext.fct.setPbCompletelyLoaded);
            break;
        case APP_DEVICE_ERM_CHANGE_DEVICE_NAME:
            result = _APP_DEVICE_sfHandlePendingChangeDeviceName(&gAppDeviceContext.fct.changeDeviceName);
            break;
        case APP_DEVICE_ERM_RESET_TO_FACTORY_SETTING:
            result = _APP_DEVICE_sfHandlePendingResetToFactorySetting(&gAppDeviceContext.fct.factorySetting);
            break;
        case APP_DEVICE_ERM_SAVE_BT_NAME:
            result = _APP_DEVICE_sfHandlePendingSaveBtName(&gAppDeviceContext.fct.saveBtName);
            break;
        case APP_DEVICE_ERM_DELETE_ALL_PROFILES:
            result = _APP_DEVICE_sfHandlePendingDeleteAllProfiles(&gAppDeviceContext.fct.delAllProfiles);
            break;
        case APP_DEVICE_STOP_RINGTONE_NON_BLOCK:
            result = _APP_DEVICE_sfHandlePendingStopRingtoneNonBlock();
            break;
        case APP_DEVICE_START_RINGTONE_NON_BLOCK:
            result = _APP_DEVICE_sfHandlePendingStartRingtoneNonBlock(&gAppDeviceContext.fct.startPlayRingtone);
            break;
        case APP_DEVICE_ERM_UPDATE_BACKUP_EEPROM:
        	result = _APP_DEVICE_sfHandlePendingUpdateBackupAreaEeprom(&gAppDeviceContext.fct.backupAreaEeprom);
        	break;
        case APP_DEVICE_CHANGE_PB_ORDER_FOR_NEW_USER:
            result = _APP_DEVICE_sfHandlePendingChangeSortingOrderForNewUser(&gAppDeviceContext.fct.changePBOrder);
            break;
        case APP_DEVICE_ERM_UPDATE_DELETED_PLAYER_IN_PROFILES:
            result = _APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles();
            break;
        case APP_DEVICE_PHONE_SERVICE_DEACTIVATION:
            result = _APP_DEVICE_sfHandlePendingDeactivate(puart);
            break;
        default:
            result = FAIL;
            break;
    }

    if(FAIL == result)
    {

        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);

        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "FAIL for %d", gAppDeviceContext.fct.common.fctType);

    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_GetActiveServiceDeviceId                          */
/**
    \brief      returns device id for a given service if any device is
                connected through this service

    \param      service
                ATBTServiceEnum
                returns device id of given currently active service

    \return     AT_DeviceId
                device id which has the given service connected and active

*/
/*----------------------------------------------------------------------------*/
AT_DeviceId APP_DEVICE_GetActiveServiceDeviceId( ATBTServiceEnum service)
{
    AT_DeviceId i = 0;

    for(i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        const ERMBTDeviceTableType *pBtDevice = ERM_GetBTDevice(i);

        if(pBtDevice == NULL)
        {
            continue;
        }

        switch(service)
        {
            case AT_SERVICE_PHONE:
                if(pBtDevice->phone_active == AT_TRUE)
                {
                    return i;
                }
                break;
            case AT_SERVICE_A2DP_SINK:
                if(pBtDevice->a2dp_sink_active == AT_TRUE)
                {
                    return i;
                }
                break;
            case AT_SERVICE_AVRCP:
                if(pBtDevice->avrcp_active == AT_TRUE)
                {
                    return i;
                }
                break;
            default:

                DEBUG_VALUE1(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "GetActiveServiceDeviceId: unsup. service %d",
                                     service);
                break;
        }
    }
    return AT_INVALID_DEVICE_ID;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_bIsDeviceType                             */
/**
    \brief      checks if given class of device is of type deviceType

    \param      cod
                AT_BTClassOfDevice
                class of device byte mask

    \param      deviceType
                type of device

    \return     BooleanType
                BT_TRUE or BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_DEVICE_bIsDeviceType(AT_BTClassOfDevice cod, APP_DEVICE_DeviceType deviceType)
{
    BooleanType result = BT_FALSE;

    switch(deviceType)
    {
        case APP_DEVICE_enTYPE_PHONE:
            if( (APP_DEVICE_bIsMajorDevClass(cod, APP_DEVICE_COD_MAJOR_PHONE)       ) ||
                (APP_DEVICE_bIsMajorDevClass(cod, APP_DEVICE_COD_MAJOR_COMPUTER) &&
                   APP_DEVICE_bIsServiceClass(cod, APP_DEVICE_COD_SERVICE_TELEPHONY)) )
            {
                result = BT_TRUE;
            }
            break;
        case APP_DEVICE_enTYPE_HEADSET:
            if( APP_DEVICE_bIsMajorDevClass(cod, APP_DEVICE_COD_MAJOR_AUDIO) )
            {
                if( APP_DEVICE_bIsMinorDevClass(cod, APP_DEVICE_COD_MINOR_AUDIO_HEADSET) ||
                    APP_DEVICE_bIsMinorDevClass(cod, APP_DEVICE_COD_MINOR_AUDIO_HANDSFREE) )
                {
                    result = BT_TRUE;
                }
            }
            break;
        case APP_DEVICE_enTYPE_PLAYER:
            if( APP_DEVICE_bIsMajorDevClass(cod, APP_DEVICE_COD_MAJOR_PHONE) ||
                APP_DEVICE_bIsMajorDevClass(cod, APP_DEVICE_COD_MAJOR_COMPUTER) )
            {
                //result = APP_DEVICE_bIsServiceClass(cod, APP_DEVICE_COD_SERVICE_AUDIO);
                result = BT_TRUE;
            }
            if(APP_DEVICE_bIsMajorDevClass(cod, APP_DEVICE_COD_MAJOR_AUDIO))
            {
                /* RT#6666 iPad is not in the MP-devicelist
                   same problem has the Samsung player YP-T10
                   BTcod ipad:   0A041C
                   BTcod YP-T10: 38040C
                   accept now the 04 = MajorDevClass Audio
                */
                result = BT_TRUE;
            }
            break;
        default:
            result = BT_FALSE;
            break;
    }
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_bIsFactorySettingActive                           */
/**
    \brief      get the status of factory setting

    \return     BooleanType
                BT_TRUE or BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_DEVICE_bIsFactorySettingActive(void)
{
    if (APP_DEVICE_ERM_RESET_TO_FACTORY_SETTING == gAppDeviceContext.fct.common.fctType)
    {
        return BT_TRUE;
    }
    else
    {
        return BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_bIsServiceClass                             */
/**
    \brief      checks if given class of device is of type serviceClass

    \param      cod
                AT_BTClassOfDevice
                class of device byte mask

    \param      serviceClass
                type of service

    \return     BooleanType
                BT_TRUE or BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_DEVICE_bIsServiceClass(AT_BTClassOfDevice cod, APP_DEVICE_CodServiceClass serviceClass)
{
    /* make sure that we have 32 bit range */
    Uint32Type serviceClass32 = (Uint32Type)serviceClass;

    if( ((cod & APP_DEVICE_COD_SERVICE_MASK) & serviceClass32) != 0)
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_bIsMajorDevClass                             */
/**
    \brief      checks if given class of device is of type majorDevClass

    \param      cod
                AT_BTClassOfDevice
                class of device byte mask

    \param      majorDevClass
                type of major device class

    \return     BooleanType
                BT_TRUE or BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_DEVICE_bIsMajorDevClass(AT_BTClassOfDevice cod, APP_DEVICE_CodMajorDeviceClass majorDevClass)
{
    /* make sure that we have 32 bit range */
    Uint32Type majorDevClass32 = (Uint32Type)majorDevClass;

    if( ((cod & APP_DEVICE_COD_MAJOR_MASK) & majorDevClass32) != 0)
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_bIsMinorDevClass                             */
/**
    \brief      checks if given class of device is of type minorDevClass

    \param      cod
                AT_BTClassOfDevice
                class of device byte mask

    \param      minorDevClass
                type of minor device class

    \return     BooleanType
                BT_TRUE or BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_DEVICE_bIsMinorDevClass(AT_BTClassOfDevice cod, APP_DEVICE_CodMinorDeviceClass minorDevClass)
{
    /* make sure that we have 32 bit range */
    Uint32Type minorDevClass32 = (Uint32Type)minorDevClass;

    if( ((cod & APP_DEVICE_COD_MINOR_MASK) & minorDevClass32) != 0)
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vOnAppCshdlNotification                             */
/**
    \brief      handles notifications from App Cshdl module

    \param      notification
                APP_CSHDL_NotificationType
                notification received from App Cshdl module
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OnAppCshdlNot: notId %d",notification);

    switch(notification)
    {
        case APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN:
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:

            gAppDeviceContext.deactPlayerPending = AT_INVALID_DEVICE_ID;

            if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
            {

                DEBUG_VALUE1(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnAppCshdlNot: pending fct %d dropped",
                                     gAppDeviceContext.fct.common.fctType);


                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
        default:

            DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OnAppCshdlNot: ignor notId %d",
                         notification);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vOnAppPhoneNotification                           */
/**
    \brief      handles notifications from App Phone module

    \param      notification
                APP_PHONE_enNotificationId
                notification received from App Phone module
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_PHONE_CALL_STATE_CHANGED:
            // check if initialization is still pending due to a call during connect
            //
            if(BT_TRUE == gAppDeviceContext.initCK5050ForUserProfilePendingDueToCallTakeOver &&
               BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
            {
                gAppDeviceContext.initCK5050ForUserProfilePendingDueToCallTakeOver = BT_FALSE;
                // init ck5050 depending on user profile settings, e.g. set // ringtone
                if( _APP_DEVICE_sfInitCK5050ForUserProfile(NULL) == APP_COMMON_RSP_ERROR_BUSY)
                {
                    gAppDeviceContext.initCK5050ForUserProfilePending = BT_TRUE;
                }
            }
            break;
        default:

            DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OnAppPhoneNot: ignor notId %d",
                         notification);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_vOnAppPhoneBookNotification                       */
/**
    \brief      handles notifications from App PhoneBook module

    \param      notification
                APP_PB_enNotificationId
                notification received from App PhoneBook module
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_vOnAppPhoneBookNotification(APP_PB_enNotificationId notification)
{
    switch(notification)
    {
        case APP_PB_DOWNLOAD_STATE_CHANGED_RETRY_PB_SORTING_FOR_NEWUSER:
        {
            gAppDeviceContext.PBSortingOrderRetryCounter++;
            if(gAppDeviceContext.PBSortingOrderRetryCounter <= APP_DEVICE_MAX_NUMBER_RETRIES_PB_SORTING)
            {
                DEBUG_TEXT(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vOnAppPhoneBookNotification: retry changing PB sorting order, PB was synced");

                if( _APP_DEVICE_vChangePBSortingOrder(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                {   /* set the waiting flag if application is busy */
                    gAppDeviceContext.changeSortingOrderForProfile = BT_TRUE;
                }
            }
        }
        break;
        default:

            DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OnAppPhoneBookNot: ignor notId %d",
                         notification);
        break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vNotifyApps                                      */
/**
    \brief      notify apps that something happened, e.g. a new profile has
                been created

    \param      new_notification
                APP_DEVICE_enNotificationId
                notification to be indicated
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vNotifyApps(APP_DEVICE_enNotificationId new_notification)
{
    APP_MDI_vOnAppDeviceNotification(new_notification);
    APP_PHONEBOOK_vOnAppDeviceNotification(new_notification);
    APP_PHONE_BAP_vOnAppDeviceNotification(new_notification);
    APP_PHONE_vOnAppDeviceNotification(new_notification);
    APP_PHONEBOOK_BAP_vOnAppDeviceNotification(new_notification);
    APP_SD_vOnAppDeviceNotification(new_notification);
#ifdef UHV_ACTIVATE_FEATURE_LED
    LED_vHandleAppDeviceNotification(new_notification);
#endif
    VCR_vOnAppDeviceNotification(new_notification);
}

void APP_DEVICE_vEmergencyReleaseContextFromAppSd(void)
{
    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
}

void APP_DEVICE_vSetDeactivatePlayerPendingFlag(AT_DeviceId deviceId)
{
    gAppDeviceContext.deactPlayerPending = deviceId;
}
void APP_DEVICE_vSetActivatePlayerPendingFlag(void(*cb)(APP_COMMON_enResponseCode), AT_DeviceId deviceId)   
{
    gAppDeviceContext.actPlayerPending.deviceId = deviceId;
    gAppDeviceContext.actPlayerPending.cb = cb;
}
/* End Of File APP_DEVICE_C1.c */

