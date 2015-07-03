/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_Connection_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_DEVICE
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP and
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2011-12-15 13:03:30 +0100 (Do, 15 Dez 2011) $
* $Rev: 24617 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/src/APP_DEVICE_Connection_C1.c $
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
#include "APP_DEVICE_AudioManager.h"
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
#include "APP_DEVICE_AT.h"


#include "APP_DEVICE_Connection_C1.id"
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
#define MAX_ALLOWED_STEPS_MD_ACTIVATE 25
#define MAX_ALLOWED_STEPS_MD_DEACTIVATE 10
#define MAX_ALLOWED_STEPS_PHONE_ACTIVATE 10
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/* Phone Event Handler */
static void _APP_DEVICE_Connection_vHandlePhoneStarted(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandlePhoneStopped(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandlePhoneStartFailed(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandlePhoneStopFailed(const ERMNotificationPbsnType * pPbsn);

/* A2DP Event Handler */
static void _APP_DEVICE_Connection_vHandleA2dpStarted(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandleA2dpStopped(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandleA2dpStartFailed(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandleA2dpStopFailed(const ERMNotificationPbsnType * pPbsn);

/* AVRCP Event Handler */
static void _APP_DEVICE_Connection_vHandleAvrcpStarted(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandleAvrcpStopped(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandleAvrcpStartFailed(const ERMNotificationPbsnType * pPbsn);
static void _APP_DEVICE_Connection_vHandleAvrcpStopFailed(const ERMNotificationPbsnType * pPbsn);

/* Pairing Event Handler */
static void _APP_DEVICE_Connection_vHandlePairingFailed(const ERMNotificationPprsType *pPprs);

/* BT (Dis-)Connection Event Handler */
static void _APP_DEVICE_Connection_vHandleConnectFailed(const ERMNotificationPsbdType * pPbsd);
static void _APP_DEVICE_Connection_vHandleConnected(const ERMNotificationPsbdType *pPsbd);
static void _APP_DEVICE_Connection_vHandleDisconnected(const ERMNotificationPsbdType *pPsbd);
//static void _APP_DEVICE_Connection_vHandleDisconnectFailed(const ERMNotificationPsbdType *pPsbd);

///* handler to check if all requirements to fulfill fctId are met */
//static BooleanType _APP_DEVICE_Connection_bMdActivateFinished(AT_DeviceId devId);
//static BooleanType _APP_DEVICE_Connection_bMdDeactivateFinished(AT_DeviceId devId);


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
/* Function:    APP_DEVICE_sfActivatePhoneDevice                                  */
/**
    \brief      activates phone service for given device Id

    \param      deviceId
                AT_DeviceId
                id of paired phone device which shall be activated as phone

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_sfActivatePhoneDevice( AT_DeviceId deviceId,
                                                            void(*cb)(APP_COMMON_enResponseCode))
{

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        if (gAppDeviceContext.fct.common.fctType == APP_DEVICE_PHONE_SERVICE_ACTIVATION)
        {
            DEBUG_VALUE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"sfActivatePhoneDevice: Activation pending for devId %d , requested id %d",gAppDeviceContext.fct.serviceAct.deviceId,deviceId);
            if (deviceId == gAppDeviceContext.fct.serviceAct.deviceId)
            {
                DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"sfActivatePhoneDevice: Activation pending for same device, return OK");
                return APP_COMMON_RSP_OK;
            }
            else
            {
                DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"sfActivatePhoneDevice: Activation pending for other device, return ERROR");
                return APP_COMMON_RSP_ERROR;
            }
        }
        else
        {
            return APP_COMMON_RSP_ERROR_BUSY;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"sfActivatePhoneDevice: Nothing pending, start deviceId %d",deviceId);
    }

    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_PHONE_SERVICE_ACTIVATION;
    gAppDeviceContext.fct.common.callBack               = cb;
    gAppDeviceContext.fct.serviceAct.deviceId           = deviceId;
    gAppDeviceContext.fct.serviceAct.maxAllowedSteps    = MAX_ALLOWED_STEPS_PHONE_ACTIVATE;
    gAppDeviceContext.fct.serviceAct.state              = APP_DEVICE_PHONE_STOP_SERVICE_NOT_SENT;


    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF _APP_DEVICE_sfHandlePlayStatus()                               */

/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_sfDeactivatePhoneService( AT_DeviceId deviceId,
                                                            void(*cb)(APP_COMMON_enResponseCode))
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        gAppDeviceContext.deativatePhoneServiceDeviceId = deviceId;
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_PHONE_SERVICE_DEACTIVATION;
    gAppDeviceContext.fct.common.callBack               = cb;
    
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    :APP_DEVICE_sfSetAutoConnectionMode                                                              */
/**
    \brief      sets the autoconnection mode on or off

    \param      enable
                BooleanType
                whether to activate or deactivate

    \param      cb
                callback of calling application to be notified when function has
                been completely processed

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_sfSetAutoConnectionMode( BooleanType enable,
                                                              void(*cb)(APP_COMMON_enResponseCode))
{

    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {

        return APP_COMMON_RSP_ERROR_BUSY;
    }

    gAppDeviceContext.fct.common.fctType     = APP_DEVICE_PHONE_SET_AUTO_CONN_MODE;
    gAppDeviceContext.fct.common.callBack    = cb;
    gAppDeviceContext.fct.autoConn.state     = APP_DEVICE_SET_MODE_NOT_SENT;
    gAppDeviceContext.fct.autoConn.enable    = enable;


    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF ()                               */


/*----------------------------------------------------------------------------*/
/* Function    :APP_DEVICE_sfStopRingtoneNonBlock                             */
/**
    \brief      Sends a ringtone stop command to the CK5050, returns no BUSY


    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcStopRingtoneNonBlock( )
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        /* save pending request, so interface is non blocking */
        gAppDeviceContext.stopRingtoneNonBlock = BT_TRUE;
        return APP_COMMON_RSP_OK;
    }
    
    DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "StopRingtoneNonBlock called");
    gAppDeviceContext.fct.common.fctType = APP_DEVICE_STOP_RINGTONE_NON_BLOCK;
    gAppDeviceContext.fct.stopPlayRingtone.retry = 0;
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF ()                               */

/*----------------------------------------------------------------------------*/
/* Function    :APP_DEVICE_rcdisconnectDevice                             */
/**
    \brief     sends a disconnect device to ck5050, returns no BUSY


    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcDisconnectDevice(Uint16Type deviceId)
{
    gAppDeviceContext.disconnectDeviceId = deviceId;
    
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        /* save pending request, so interface is non blocking */
        gAppDeviceContext.disconnectDevice = BT_TRUE;
        return APP_COMMON_RSP_OK;
    }
    
    //DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "DisconnectDevice called");    
    gAppDeviceContext.fct.common.fctType = APP_DEVICE_DISCONNECT_DEVICE;
    
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
} 
/*----------------------------------------------------------------------------*/
/* Function    :APP_DEVICE_sfStartRingtoneNonBlock                             */
/**
    \brief      Sends a ringtone start command to the CK5050, returns no BUSY


    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcStartRingtoneNonBlock(ATRintoneModeEnum ringtone)
{
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "StartRingtoneNonBlock: fctType 0x%02x busy",
                     gAppDeviceContext.fct.common.fctType );

        /* save pending request, so interface is non blocking */
        gAppDeviceContext.startRingtoneNonBlock = BT_TRUE;
        gAppDeviceContext.ringtone = ringtone;
        return APP_COMMON_RSP_OK;
    }

    DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "StartRingtoneNonBlock called");
    gAppDeviceContext.fct.common.fctType         = APP_DEVICE_START_RINGTONE_NON_BLOCK;

    gAppDeviceContext.fct.startPlayRingtone.state = APP_DEVICE_START_RINGTONE_STOP_PLAYBACK;
    gAppDeviceContext.fct.startPlayRingtone.ringtone = ringtone;
    gAppDeviceContext.fct.startPlayRingtone.retry = 0;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF ()                               */


/*----------------------------------------------------------------------------*/
/* Function :   APP_DEVICE_rcActivateMdiDevice                                   */
/**
    \brief      Tries to start the A2DP and AVRCP services on the specified device

    \param      cb
                Callback to notify the caller after finishing

    \param      deviceId
                Device to start

    \return     APP_COMMON_enResponseCode
                OK, ERROR, ... 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcActivateMdiDevice( void(*cb)(APP_COMMON_enResponseCode),
                                             AT_DeviceId deviceId)
{
    const ERMBTDeviceTableType *pDevice = NULL;

    pDevice = ERM_GetBTDevice(deviceId);

    /* first check for valid input */
    if( pDevice == NULL )
    {

        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "ActMdiDev: inval devId %d",
                      deviceId );

        return APP_COMMON_RSP_ERROR;
    }

    /* check if app is busy */
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "ActMdiDev: application busy, fct in pending %d",
                      gAppDeviceContext.fct.common.fctType ); 

        return APP_COMMON_RSP_ERROR_BUSY;
    }

    /* check if device is already connected e.g. with another service */
    if(pDevice->device_connected != AT_DEVICE_CONNECTED)
    {   /* need to connect device on bt level, first */
        gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_CONNECT_NOT_SENT;
    }
    else
    {   /* immediately start service */
        gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_CONNECT_SENT;
    }
    gAppDeviceContext.fct.deactPlayer.maxAllowedSteps   = MAX_ALLOWED_STEPS_MD_ACTIVATE;
    gAppDeviceContext.fct.actPlayer.deviceId            = deviceId;
    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_ACTIVATE_PLAYER;
    gAppDeviceContext.fct.common.callBack               = cb;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
}   /* END OF APP_DEVICE_rcActivateMdiDevice()                               */




/*----------------------------------------------------------------------------*/
/* Function :   APP_DEVICE_rcDeactivateMdiDevice                                 */
/**
    \brief      Tries to stop the A2DP and AVRCP services

    \param      cb
                Callback to notify the caller after finishing

    \return     APP_COMMON_enResponseCode
                OK, ERROR, ... 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_DEVICE_rcDeactivateMdiDevice( void(*cb)(APP_COMMON_enResponseCode))
{
    const ERMBTDeviceTableType *pDevice = NULL;

    AT_DeviceId deviceId = ERM_u8GetActivePlayer();
    DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "RT14519: first DeactMdiDev: no a2dp source act (%d), search for avrcp",
                                  deviceId );
    /*(changed on 24.03.2011 by Nikolov / RT: #9905), recognition for MP if only avrcp is started. In this case disconnect it */
    if (AT_INVALID_DEVICE_ID == deviceId)
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "DeactMdiDev: no a2dp source act (%d), search for avrcp",
                              deviceId );
        deviceId = ERM_u8GetActiveDeviceAVRCP();

        if(AT_INVALID_DEVICE_ID != deviceId)
        {
            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "DeactMdiDev: only avrcp started for devID (%d)",
                                          deviceId );
        }
    }
    pDevice = ERM_GetBTDevice(deviceId);

    /* check if input is valid */
    if( pDevice == NULL )
    {

        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "DeactMdiDev: no dev act (%d)",
                      deviceId );

        return APP_COMMON_RSP_ERROR;
    }

    /* check if app is busy */
    if(gAppDeviceContext.fct.common.fctType != APP_DEVICE_INVALID_FCT_ID)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if(pDevice->avrcp_active == AT_TRUE)
    {
        /* check if media player is playing: yes -> stop media player first */
        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "RT:14519 outside-DeactMdiDev: dev act (%d)",
                                          deviceId );
        if ( BT_TRUE == APP_MDI_bPlayerActive())
        {
            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "RT:14519-DeactMdiDev: dev act (%d)",
                                  deviceId );
            APP_MDI_sfSetPlayMode(APP_MDI_PAUSE);
        }
    }

    gAppDeviceContext.fct.deactPlayer.deviceId          = deviceId;
    gAppDeviceContext.fct.deactPlayer.maxAllowedSteps   = MAX_ALLOWED_STEPS_MD_DEACTIVATE;
    gAppDeviceContext.fct.common.fctType                = APP_DEVICE_DEACTIVATE_PLAYER;
    gAppDeviceContext.fct.common.callBack               = cb;
    gAppDeviceContext.fct.deactPlayer.state              = APP_DEVICE_MDI_SERVICE_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);

    return APP_COMMON_RSP_OK;

}   /* END OF APP_DEVICE_rcDeactivateMdiDevice()                               */


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingStopRingtoneNonBlock             */
/**
    \brief      handle pending function to delete a profile

    \param      pDel
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingStopRingtoneNonBlock()
{
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
    
    rsp = ERM_ePlayRingtone(_APP_DEVICE_StopRingtone_Cb, 
                                AT_PPRT_ACT_STOP, 
                                AT_RTMODE_DEACTIVATE);
    /* check result code */
    switch( rsp )
    {
        case APP_COMMON_RSP_ERROR_BUSY:
            /* try again later */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
            break;
        case APP_COMMON_RSP_OK:
        default:
            /* wait of callback before release context */
            break;
    }
    
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingDisconnectDevice            */
/**
    \brief      handle pending function to disconnect a device (after reconnect)

    \param      puart
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingDisconnectDevice()
{   
    SuccessFailType result = SUCCESS;
    APP_COMMON_enResponseCode rspCode = APP_COMMON_RSP_ERROR_BUSY;

    switch( gAppDeviceContext.fct.disconnectDevice.state )
    {
        case APP_DEVICE_DISCONNECT_DEVICE_NOT_SENT:
            rspCode = ERM_rcDisconnectDevice(gAppDeviceContext.disconnectDeviceId,_APP_DEVICE_DisconnectDevice_Cb);
            break;        
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "DisconnectDeviceReq: unsup state %d",
                           gAppDeviceContext.fct.disconnectDevice.state );
            result = FAIL;
            break;
    }
    
    /* check result code */
    switch( rspCode )
    {
        case APP_COMMON_RSP_ERROR_BUSY:
            /* try again later */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
            break;
        case APP_COMMON_RSP_OK:
        gAppDeviceContext.fct.disconnectDevice.state = APP_DEVICE_DISCONNECT_DEVICE_SENT;
            break;
        default:
            /* wait of callback before release context */
            break;
    }
    
    return result;
}
/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingStartRingtoneNonBlock            */
/**
    \brief      handle pending function to delete a profile

    \param      pDel
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingStartRingtoneNonBlock(AppDeviceStartPlayRingtone* pStartRingtonePlay)
{
    switch (pStartRingtonePlay->state)
    {
        case APP_DEVICE_START_RINGTONE_STOP_PLAYBACK:
        {
            APP_COMMON_enResponseCode rsp = ERM_ePlayRingtone(_APP_DEVICE_StopBeforePlayRingtone_Cb, 
                                                              AT_PPRT_ACT_STOP, 
                                                              AT_RTMODE_DEACTIVATE);
            break;
        }
        case APP_DEVICE_START_RINGTONE_MUTE_SENT:
        {
            /* Get Mute */
            SuccessFailType sF = APP_DEVICE_AM_sfGetMute(_APP_DEVICE_GetMuteRingtone_Cb,
                                                         APP_CAN_enRing,
                                                         __LINE__);
            if (SUCCESS != sF)
            {
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
        }    
        case APP_DEVICE_START_RINGTONE_PLAYBACK_SENT:
        {
            /* start playback of the selected ringtone */
            APP_COMMON_enResponseCode rsp = ERM_ePlayRingtone( _APP_DEVICE_PlayRingtone_Cb,
                                                               AT_PPRT_ACT_PLAY,
                                                               pStartRingtonePlay->ringtone);
            /* check result code */
            switch( rsp )
            {
                case APP_COMMON_RSP_ERROR_BUSY:
                    /* try again later */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
                    break;
                case APP_COMMON_RSP_OK:
                default:
                    /* wait of callback before release context */
                    break;
            }
            break;
        }
        default:
            /* do nothing */
            break;
    }
    
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_GetMuteRingtone_Cb                              */
/**
    \brief      callback indicating that request to mute the Headunit has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                rsp
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_GetMuteRingtone_Cb(APP_COMMON_enResponseCode rsp)
{
    if(gAppDeviceContext.fct.common.fctType == APP_DEVICE_START_RINGTONE_NON_BLOCK)
    {
        if (APP_COMMON_RSP_OK == rsp)
        {
            gAppDeviceContext.fct.startPlayRingtone.state = APP_DEVICE_START_RINGTONE_PLAYBACK_SENT;
        }
        /* try again later or try again in new state*/
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
    }
    else
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "_APP_DEVICE_GetMuteRingtone_Cb: invalid fctType %d",
                      gAppDeviceContext.fct.common.fctType  );
    }
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_StopBeforePlayRingtone_Cb                       */
/**
    \brief      callback indicating that request to start ringtone playback has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                rsp
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_StopBeforePlayRingtone_Cb(APP_COMMON_enResponseCode rsp)
{
    if(gAppDeviceContext.fct.common.fctType == APP_DEVICE_START_RINGTONE_NON_BLOCK)
    {
        if (APP_COMMON_RSP_OK == rsp)
        {
            gAppDeviceContext.fct.startPlayRingtone.state = APP_DEVICE_START_RINGTONE_MUTE_SENT;
            gAppDeviceContext.fct.startPlayRingtone.retry = 0;
        }
        else
        {
            /* try again later or try again in new state*/
            if (gAppDeviceContext.fct.startPlayRingtone.retry <= 10)
            {
                gAppDeviceContext.fct.startPlayRingtone.retry++;
            }
            else
            {
                DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "StopBeforePlayRingtone rsp ERROR retries %d",
                             gAppDeviceContext.fct.startPlayRingtone.retry);
                gAppDeviceContext.fct.startPlayRingtone.state = APP_DEVICE_START_RINGTONE_MUTE_SENT;
                gAppDeviceContext.fct.startPlayRingtone.retry = 0;
            }
        }
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
    }
    else
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "_APP_DEVICE_StopBeforePlayRingtone_Cb: invalid fctType %d",
                      gAppDeviceContext.fct.common.fctType  );
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_PlayRingtone_Cb                                 */
/**
    \brief      callback indicating that request to start ringtone playback has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                rsp
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_PlayRingtone_Cb(APP_COMMON_enResponseCode rsp)
{
    if (rsp != APP_COMMON_RSP_OK)
    {
        /* try again later or try again in new state*/
        if (gAppDeviceContext.fct.startPlayRingtone.retry <= 10)
        {
            gAppDeviceContext.fct.startPlayRingtone.retry++;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
        }
        else
        {
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "PlayRingtone_Cb rsp ERROR retries %d",
                       gAppDeviceContext.fct.startPlayRingtone.retry);
            gAppDeviceContext.fct.startPlayRingtone.retry = 0;
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
        }
    }
    else
    {
        gAppDeviceContext.fct.startPlayRingtone.retry = 0;
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_StopRingtone_Cb                                 */
/**
    \brief      callback indicating that request to stop ringtone playback has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                rsp
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_StopRingtone_Cb(APP_COMMON_enResponseCode rsp)
{
    if (rsp != APP_COMMON_RSP_OK)
    {
        /* try again later or try again in new state*/
        if (gAppDeviceContext.fct.stopPlayRingtone.retry <= 10)
        {
            gAppDeviceContext.fct.stopPlayRingtone.retry++;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(10), 0);
        }
        else
        {
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "StopRingtone_Cb rsp ERROR retries %d",
                       gAppDeviceContext.fct.stopPlayRingtone.retry);
            gAppDeviceContext.fct.stopPlayRingtone.retry = 0;
            (void)APP_DEVICE_AM_vReleaseMute(NULL,APP_CAN_enRing,__LINE__);
            _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
        }
    }
    else
    {
        (void)APP_DEVICE_AM_vReleaseMute(NULL,APP_CAN_enRing,__LINE__);
        gAppDeviceContext.fct.stopPlayRingtone.retry = 0;
        _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_DisconnectDevice_Cb                                 */
/**
    \brief      callback indicating that request to stop ringtone playback has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                rsp
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_DisconnectDevice_Cb(APP_COMMON_enResponseCode rsp)
{   
    _APP_DEVICE_vSendResultReleaseContext(rsp, __LINE__);
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vDeactivateMdiFromAvrcpStarted_Cb                   */
/**
    \brief      callback indicating that request to stop media player services has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                rsp
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vDeactivateMdiFromAvrcpStarted_Cb(APP_COMMON_enResponseCode rsp)
{   
    if (rsp == APP_COMMON_RSP_OK)
    {
        APP_COMMON_enResponseCode rspAct = APP_COMMON_RSP_ERROR_BUSY;
        DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "_APP_DEVICE_vDeactivateMdiFromAvrcpStarted_Cb: activate mdidevice now");
        rspAct = APP_DEVICE_rcActivateMdiDevice(NULL,gAppDeviceContext.actPlayerWhenWrongPlayerWasStarted);              
        
        if (rspAct == APP_COMMON_RSP_ERROR_BUSY)
        {
            gAppDeviceContext.actPlayerPending.deviceId = gAppDeviceContext.actPlayerWhenWrongPlayerWasStarted;
            gAppDeviceContext.actPlayerPending.cb = NULL;
            DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "_APP_DEVICE_vDeactivateMdiFromAvrcpStarted_Cb: busy for activate mdi queue event");
        }
    }    
    else
    {
        DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "_APP_DEVICE_vDeactivateMdiFromAvrcpStarted_Cb error rsp: %d",rsp);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingActivate           */
/**
    \brief      handle pending function to activate a user profile / connect a phone

    \param      pAct
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingActivate(const SCI_ST * puart, AppDeviceServiceActivateType *pActSvc)
{
    AT_DeviceId phoneDev    = AT_INVALID_DEVICE_ID;
    AT_DeviceId MPDev    = AT_INVALID_DEVICE_ID;

    DEBUG_VALUE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneActivate: starting dev: %d, state 0x%02x",
                pActSvc->deviceId, pActSvc->state);


    /* check if we are already done */
    if(BT_TRUE == _APP_DEVICE_Connection_bPhoneActivateFinished(pActSvc->deviceId))
    {
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        return SUCCESS;
    }

    /* not yet finished */

    /* did we try to often, already ?? */

    pActSvc->maxAllowedSteps--;
    
    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneActivate: steps still allowed: %d",
                      pActSvc->maxAllowedSteps);

    if(0 == pActSvc->maxAllowedSteps)
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneActivate: too many retries state 0x%02x",
                      pActSvc->state);

        /* we should stop all services ??? */
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
        return FAIL;
    }

    /* see what we still have to do ... */
    phoneDev = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE);

    MPDev = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK);

    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneActivate: phoneDev: %d",phoneDev);

    if((AT_INVALID_DEVICE_ID != phoneDev) &&
            (phoneDev != gAppDeviceContext.fct.serviceAct.deviceId) && 
            (APP_DEVICE_PHONE_WAIT_FOR_SERVICE_STOPPED != pActSvc->state ))
    {   /* another phone is connected, we have to disconnect it first */

        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneActivate: disconnect phoneDev: %d first",phoneDev);

        if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(puart, phoneDev))
        {   /* cannot stop service */
            return FAIL;
        }
        pActSvc->state = APP_DEVICE_PHONE_WAIT_FOR_SERVICE_STOPPED;
    }
    else if((AT_INVALID_DEVICE_ID == phoneDev)&&
            (APP_DEVICE_PHONE_START_SERVICE_SENT != pActSvc->state)&&
            (APP_DEVICE_PHONE_DISCONNECT_MP_DEVICE != pActSvc->state))
    {   /* try to start phone service for wanted device */
        if(FAIL == _APP_DEVICE_sfStartServiceATRequest(puart, pActSvc->deviceId, AT_SERVICE_PHONE))
        {   /* cannot stop service */
            return FAIL;
        }
        pActSvc->state = APP_DEVICE_PHONE_START_SERVICE_SENT;
    }
    else if((AT_INVALID_DEVICE_ID != MPDev) &&
            APP_DEVICE_PHONE_DISCONNECT_MP_DEVICE == pActSvc->state)
    {   /* the player of the old profile is still connected(mixed profile),
           we have to disconnect at the end if the connection to the new phone device fails */
        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneActivate: disconnect MPDev: %d, new phone device not connected",MPDev);

        if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(puart, MPDev))
        {   /* cannot stop service */
            return FAIL;
        }
        pActSvc->state = APP_DEVICE_PHONE_WAIT_FOR_MP_DEVICE_DISCONNECTED;
    }
    else if (pActSvc->state = APP_DEVICE_PHONE_START_SERVICE_SENT)
    {
        DEBUG_TEXT(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE,"sfHandlePendingActivate: wait for start result and release uart.");
        ERM_ReleaseUartForDevice();
    }
    else
    {
        DEBUG_TEXT(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE,"sfHandlePendingActivate: this should never occur.");
        /* we should stop all services ??? */
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
    }


    return SUCCESS;
}

/*---------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_sfHandlePendingMdDeactivate                          */
/**
    \brief      Responsible to coordinate the deactivation of player services

    \param      pUart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingMdDeactivate(const SCI_ST* pUart, AppDeviceDeactPlayerType* pDeactSvc)
{
    SuccessFailType result = SUCCESS;

    AT_DeviceId a2dpDev                 = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK);
    AT_DeviceId avrcpDev                = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_AVRCP);
    AT_DeviceId phoneDev                = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE);

    /* see what we still have to do ... */

    /* check if we are already done */
    if(BT_TRUE == _APP_DEVICE_Connection_bMdDeactivateFinished(pDeactSvc->deviceId))
    {
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        return SUCCESS;
    }

    /* not yet finished */

    /* did we try to often, already ?? */
    pDeactSvc->maxAllowedSteps--;
    
    DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate: device stopped: %d, steps still allowed: %d",
                    pDeactSvc->deviceId,pDeactSvc->maxAllowedSteps);

    if(0 == pDeactSvc->maxAllowedSteps)
    {

        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate: too many retries, state 0x%02x",
                      pDeactSvc->state);

        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
        return FAIL;
    }
    else
    {
        /* try to stop connected player services */
        const ERMBTDeviceTableType *pDevice = ERM_GetBTDevice(pDeactSvc->deviceId);

        if(pDevice == NULL)
        {
            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "SendPendMdDeactivate: no dev %d avail",
                          pDeactSvc->deviceId);
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
        }

        /* check if we need to stop services on currently active device -> this case will occure if we have mixed profile*/
        if(((AT_INVALID_DEVICE_ID != a2dpDev) && (a2dpDev != phoneDev ))&&
           ((AT_INVALID_DEVICE_ID != avrcpDev) && (avrcpDev != phoneDev )))
        {   /* another player is connected with a2dp and avrcp, we have to disconnect it first physically */
            if(APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED != pDeactSvc->state)
            {
                DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate: disconnect MP dev:%d(equal to phone dev)",pDeactSvc->deviceId);

                if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(pUart, a2dpDev))
                {   /* cannot stop service */
                    return FAIL;
                }
            }
            pDeactSvc->state = APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED;
        }
                /* if only a2dp started to the MP device and avrcp still not started due to any reasons */
        else if(((AT_INVALID_DEVICE_ID != a2dpDev) && (AT_INVALID_DEVICE_ID == avrcpDev) && (a2dpDev != phoneDev))||
                /* in case of mixed profile and avrcp connected to the wrong devide(the phone device) -> disconnect physically only the a2dp device and avrcp should be only stopped*/
                ((AT_INVALID_DEVICE_ID != a2dpDev) && (AT_INVALID_DEVICE_ID != avrcpDev) && (a2dpDev != phoneDev) &&(avrcpDev == phoneDev)))
        {
            if(APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED != pDeactSvc->state)
            {
                DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate: disconnect MP dev:%d(equal to phone dev)",pDeactSvc->deviceId);

                if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(pUart, a2dpDev))
                {   /* cannot stop service */
                    return FAIL;
                }
            }
            pDeactSvc->state = APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED;
        }
                /* if only avrcp started to the MP device and a2dp still not started due to any reasons */
        else if(((AT_INVALID_DEVICE_ID == a2dpDev) && (AT_INVALID_DEVICE_ID != avrcpDev) && (avrcpDev != phoneDev ))||
                /* in case of mixed profile and a2dp connected to the wrong devide(the phone device) -> disconnect physically only the avrcp device*/
                ((AT_INVALID_DEVICE_ID != a2dpDev) && (AT_INVALID_DEVICE_ID != avrcpDev) && (a2dpDev == phoneDev) &&(avrcpDev != phoneDev)))
        {
            if(APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED != pDeactSvc->state)
            {
                DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate: disconnect MP dev:%d(equal to phone dev)",pDeactSvc->deviceId);

                if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(pUart, avrcpDev))
                {   /* cannot stop service */
                    return FAIL;
                }
            }
            pDeactSvc->state = APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED;
        }
        /* this is the case of profile of phones and MP services to the same device -> in this case only stop the services and dont disconnect the device*/
        else if((AT_INVALID_DEVICE_ID != a2dpDev) && (a2dpDev == phoneDev ))
        {
            /* stop the AVRCP service */
            DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate:deactivate A2DP %d avail, requested device %d",
                          a2dpDev, pDeactSvc->deviceId);

            result = _APP_DEVICE_sfStopServiceATRequest(pUart,
                                                        a2dpDev,
                                                        AT_SERVICE_A2DP_SINK);
            pDeactSvc->state = APP_DEVICE_MDI_SERVICE_A2DP_STOP_SENT;
        }
        else if((AT_INVALID_DEVICE_ID != avrcpDev) && (avrcpDev == phoneDev ))
        {
            /* stop the A2DP service */
            DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdDeactivate:deactivate AVRCP %d avail, requested device %d",
                          avrcpDev, pDeactSvc->deviceId);

            result = _APP_DEVICE_sfStopServiceATRequest(pUart,
                                                        avrcpDev,
                                                        AT_SERVICE_AVRCP);
                                                        
            pDeactSvc->state = APP_DEVICE_MDI_SERVICE_AVRCP_STOP_SENT;
        }
        else
        {
            /* waiting for service to be started */
            DEBUG_TEXT( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "SendPendMdDeactivate: waiting for service to be started, this should never occure");
            (void)ERM_ReleaseUartForDevice();
        }
    }
    return result;
}

/*---------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_sfHandlePendingMdActivate                            */
/**
    \brief      Responsible to coordinate the activation of player services

    \param      pUart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingMdActivate(const SCI_ST* pUart, AppDeviceActPlayerType* pActSvc)
{
    SuccessFailType result = SUCCESS;
    AT_DeviceId a2dpDev                 = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK);
    AT_DeviceId avrcpDev                = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_AVRCP);
    AT_DeviceId phoneDev                = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE);

    /* check if we are already done */
    if(BT_TRUE == _APP_DEVICE_Connection_bMdActivateFinished(pActSvc->deviceId))
    {
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
        return SUCCESS;
    }

    /* not yet finished */

    /* did we try to often, already ?? */
    
    pActSvc->maxAllowedSteps--;
    
    DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: activate dev: %d, steps still allowed: %d",
                    pActSvc->deviceId,pActSvc->maxAllowedSteps);
    if(0 == pActSvc->maxAllowedSteps)
    {
        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: too many retries state 0x%02x",
                      pActSvc->state);

        /* we should stop all services ??? */
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
        return FAIL;
    }

    /* first check if we need to stop services on currently active device */
    if((AT_INVALID_DEVICE_ID != a2dpDev) &&(AT_INVALID_DEVICE_ID != avrcpDev)&&
       /* if both services are started to the same physical device */
       (a2dpDev == avrcpDev)&&
       /* if the new MP device not equal to the phone device connected at the time */
       (pActSvc->deviceId != phoneDev )&&
       /* if the current connected MP is not equal to the phone device */
       (a2dpDev != phoneDev))
    {
        /* in this case just disconnect the device */
        if(APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED != pActSvc->state)
        {
            DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: disconnect MP dev:%d, new dev is :%d",
                                                            a2dpDev, pActSvc->deviceId);

            if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(pUart, a2dpDev))
            {   /* cannot stop service */
                return FAIL;
            }
        }
        pActSvc->state = APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED;
    }
    else if(((AT_INVALID_DEVICE_ID != a2dpDev) ||(AT_INVALID_DEVICE_ID != avrcpDev))&&
            /* if the new MP device not equal to the phone device connected at the time */
            (pActSvc->deviceId != a2dpDev ) && (pActSvc->deviceId != avrcpDev ))

    {   /* another player is connected(equal physical device as phone), just stop the services*/
        if((AT_INVALID_DEVICE_ID != a2dpDev) && 
           (pActSvc->deviceId != a2dpDev ))
        {   /* need to stop foreign a2dp service */
            if(APP_DEVICE_MDI_SERVICE_A2DP_STOP_SENT != pActSvc->state)
            {
                DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: stop A2DP of dev:%d", a2dpDev);

                result = _APP_DEVICE_sfStopServiceATRequest(pUart, a2dpDev, AT_SERVICE_A2DP_SINK);
                pActSvc->state  = APP_DEVICE_MDI_SERVICE_A2DP_STOP_SENT;
            }
            else
            {
                /* wait for a2dp stop */
                (void)ERM_ReleaseUartForDevice();
            }
        }
        else
        {
            DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: wanted A2DP dev:%d connected as dev: &d", pActSvc->deviceId, a2dpDev);
        }
        if((AT_INVALID_DEVICE_ID != avrcpDev) &&
                (pActSvc->deviceId != avrcpDev ))
        {    /* need to stop foreign avrcp service */
            if(APP_DEVICE_MDI_SERVICE_AVRCP_STOP_SENT != pActSvc->state)
            {
                DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: stop AVRCP of dev:%d", avrcpDev);

                result = _APP_DEVICE_sfStopServiceATRequest(pUart, avrcpDev, AT_SERVICE_AVRCP);

                pActSvc->state  = APP_DEVICE_MDI_SERVICE_AVRCP_STOP_SENT;
            }
            else
            {
                /* wait for avrcp stop */
                (void)ERM_ReleaseUartForDevice();
            }
        }
        else
        {
            DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: wanted AVRCP dev:%d connected as dev: &d", pActSvc->deviceId, a2dpDev);
        }
    }
    else
    {   /* try to start supported services for wanted device */
        const ERMBTDeviceTableType *pDevice = ERM_GetBTDevice(pActSvc->deviceId);

        if(pDevice == NULL)
        {
            DEBUG_VALUE1( APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "SendPendMdActivate: no dev %d avail",
                          pActSvc->deviceId);
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
        }

        if( /* start the avrcp service if the device is connected */
            (pDevice->device_connected ==AT_DEVICE_CONNECTED)&&
            /* already a2dp is started */
            (pDevice->a2dp_sink_active == AT_TRUE)&&
            /* avrcp is supported by the MP device */
            (pDevice->avrcp_supported == AT_SUPPORTED) &&
            /* avrcp is still not started */
            (pDevice->avrcp_active != AT_TRUE) &&
            (pActSvc->state != APP_DEVICE_MDI_SERVICE_AVRCP_START_SENT)&&
            (gAppDeviceContext.fct.actPlayer.bIsAVRCPOfWrongDeviceStarted == BT_FALSE))
        {
            /* we will start both, the A2DP_Sink and the AVRCP service */
            DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: start avrcp of dev:%d", pActSvc->deviceId);

            result = _APP_DEVICE_sfStartServiceATRequest( pUart,
                                                       pActSvc->deviceId,
                                                       AT_SERVICE_AVRCP);
            pActSvc->state  = APP_DEVICE_MDI_SERVICE_AVRCP_START_SENT;
        }
        else if((pActSvc->state != APP_DEVICE_MDI_SERVICE_A2DP_START_SENT)&&
                (gAppDeviceContext.fct.actPlayer.bIsA2DPOfWrongDeviceStarted == BT_FALSE))
                //&&
                ///* if the MP device is still not connected(mixed profile) */
                //(pDevice->device_connected == AT_DEVICE_NOT_CONNECTED)||
                ///* if the MP device is same device as phone(device already connected) */
                //((pDevice->device_connected == AT_DEVICE_CONNECTED)&&(pActSvc->deviceId == phoneDev)))
        {
            DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: start a2dp of dev:%d", pActSvc->deviceId);

            /* we will first try to start the A2DP_Sink service(recommended by BT SIG white paper "Simultaneous_Use_of_HFP_A2DP_and_AVRCP_WP_V11.pdf") */
            result = _APP_DEVICE_sfStartServiceATRequest( pUart,
                                                       pActSvc->deviceId,
                                                       AT_SERVICE_A2DP_SINK);
            pActSvc->state = APP_DEVICE_MDI_SERVICE_A2DP_START_SENT;
        }
        else
        {   /* waiting for service to be started */
            DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendMdActivate: waiting for service to be started");
            (void)ERM_ReleaseUartForDevice();
        }
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingDeactivate           */
/**
    \brief      handle pending function to activate a user profile / connect a phone

    \param      pAct
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingDeactivate(const SCI_ST * puart)
{
    SuccessFailType result  = SUCCESS;
    AT_DeviceId phoneDev    = AT_INVALID_DEVICE_ID;  
  

    /* see what we still have to do ... */
    phoneDev = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE);
    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneDeactivate: phoneDev: %d",phoneDev);
    
    if(AT_INVALID_DEVICE_ID != phoneDev)
    {   
        DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneDeactivate: disconnect dev: %d",phoneDev);

        if(FAIL == _APP_DEVICE_sfDisconnectDeviceATRequest(puart, phoneDev))
        {   /* cannot stop service */
            DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendPendPhoneDeactivate: cannot stop phone service of device %d",phoneDev);
            gAppDeviceContext.deativatePhoneServiceDeviceId = AT_INVALID_DEVICE_ID ;
            _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            return FAIL;
        }
    }
    else
    {
        gAppDeviceContext.deativatePhoneServiceDeviceId = AT_INVALID_DEVICE_ID ;
        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
    }
    return result;
}



/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_sfHandlePendingSetAutoConnMode                  */
/**
    \brief      handle pending function to change auto connection mode

    \param      pAutoConn
                pointer to current function context

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_DEVICE_sfHandlePendingSetAutoConnMode(const AppDeviceSetAutoConnModeType *pAutoConn)
{
    switch(pAutoConn->state)
    {
        case APP_DEVICE_SET_MODE_NOT_SENT:
        {
                APP_COMMON_enResponseCode rspCode = ERM_rcSetAutoConnMode( pAutoConn->enable, NULL);
                switch( rspCode )
                {
                    case APP_COMMON_RSP_OK:
                        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, 0);
                        gAppDeviceContext.autoConnMode = pAutoConn->enable;
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
            break;
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vOnServiceStartCb                  */
/**
    \brief      callback indicating that request to start phone service has
                been processed with result given with response

    \param      response
                APP_COMMON_enResponseCode
                result
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vOnServiceStartCb(APP_COMMON_enResponseCode response)
{
    /* we are finished */
    _APP_DEVICE_vSendResultReleaseContext(response, __LINE__);
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleA2dpNotification              */
/**
    \brief      dispatcher for the A2DP notifications

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleA2dpNotification(const ERMNotificationPbsnType * pPbsn)
{
    switch(pPbsn->result)
    {
        case AT_SERVRES_START_SUCCEED:
            _APP_DEVICE_Connection_vHandleA2dpStarted(pPbsn);
            break;
        case AT_SERVRES_START_FAILED:
            _APP_DEVICE_Connection_vHandleA2dpStartFailed(pPbsn);
            break;


        case AT_SERVRES_STOP_SUCCEED:
            _APP_DEVICE_Connection_vHandleA2dpStopped(pPbsn);
            break;
        case AT_SERVRES_STOP_FAILED:
            _APP_DEVICE_Connection_vHandleA2dpStopFailed(pPbsn);
            break;
    }

}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleAvrcpNotification             */
/**
    \brief      dispatcher for the AVRCP notifications

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleAvrcpNotification(const ERMNotificationPbsnType * pPbsn)
{
    switch(pPbsn->result)
    {
        case AT_SERVRES_START_SUCCEED:
            _APP_DEVICE_Connection_vHandleAvrcpStarted(pPbsn);
            break;
        case AT_SERVRES_START_FAILED:
            _APP_DEVICE_Connection_vHandleAvrcpStartFailed(pPbsn);
            break;

        case AT_SERVRES_STOP_SUCCEED:
            _APP_DEVICE_Connection_vHandleAvrcpStopped(pPbsn);
            break;
        case AT_SERVRES_STOP_FAILED:
            _APP_DEVICE_Connection_vHandleAvrcpStopFailed(pPbsn);
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleBtConnectionNotification      */
/**
    \brief      handles the bluetooth connection notification

    \param      pPsbd
                ERMNotificationPsbdType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleBtConnectionNotification(const ERMNotificationPsbdType * pPsbd)
{
    switch(pPsbd->status)
    {
        case AT_CONN_FAILED:
        case AT_CONN_NOT_FOUND:
            _APP_DEVICE_Connection_vHandleConnectFailed(pPsbd);
            break;
        case AT_CONN_SUCC:
            _APP_DEVICE_Connection_vHandleConnected(pPsbd);
            break;
        case AT_CONN_DISCON_SUCC:
            _APP_DEVICE_Connection_vHandleDisconnected(pPsbd);
            break;
        case AT_CONN_DISCON_FAIL:
            //_APP_DEVICE_Connection_vHandleDisconnectFailed(&pNotification->psbd);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandlePairingNotification           */
/**
    \brief      handles the pairing notification

    \param      pPprs
                ERMNotificationPprsType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePairingNotification(const ERMNotificationPprsType * pPprs)
{
    switch(pPprs->result)
    {
        case AT_PAIRING_FAILED:
            _APP_DEVICE_Connection_vHandlePairingFailed(pPprs);
            break;           
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleServiceSupportNotification    */
/**
    \brief      handles the service support notification

    \param      pPbsc
                ERMNotificationPbscType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleServiceSupportNotification(const ERMNotificationPbscType * pPbsc)
{
    // TODO: special functions for PBSC 
    switch(gAppDeviceContext.fct.common.fctType)
    {
        default:
            /* ignore the notification */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_bMdActivateFinished                  */
/**
    \brief      checks if the MP activation is finished for the respective devID

    \param      devId
                AT_DeviceId

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType _APP_DEVICE_Connection_bMdActivateFinished(AT_DeviceId devId)
{
    /* check if all services are started */
    AT_DeviceId a2dpDev                 = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK);
    AT_DeviceId avrcpDev                = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_AVRCP); 
    const ERMBTDeviceTableType * pDev   = ERM_pGetDevice(devId);

    DEBUG_VALUE2( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "bMdActivateFinished: a2dpDev: %d / avrcpDev: %d",a2dpDev,avrcpDev);
    DEBUG_VALUE1( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "bMdActivateFinished: devID: %d",devId);
    if(a2dpDev != devId)    // a2dp is always required
    {
        return BT_FALSE;
    }

    if((pDev != NULL) && (AT_TRUE == pDev->a2dp_sink_active) && 
       ( (AT_SUPPORTED == pDev->avrcp_supported) && (AT_TRUE == pDev->avrcp_active)) || (AT_NOT_SUPPORTED == pDev->avrcp_supported) )
    {   /* all services started, final state reached */
        DEBUG_TEXT( APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "bMdActivateFinished: final state reached");
        return BT_TRUE;
    }
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_bMdDeactivateFinished                */
/**
    \brief      checks if the MP de-activation is finished for the respective devID

    \param      devId
                AT_DeviceId

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType _APP_DEVICE_Connection_bMdDeactivateFinished(AT_DeviceId devId)
{
    /* check if all services are started */
    AT_DeviceId a2dpDev                 = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK);
    AT_DeviceId avrcpDev                = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_AVRCP); 

    if((BT_FALSE == ERM_bDevConnectedOnBtLevel(devId)) || 
       ((a2dpDev != devId) && (avrcpDev != devId)))
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_bPhoneActivateFinished               */
/**
    \brief      checks if the phone activation is finished for the respective devID

    \param      devId
                AT_DeviceId

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType _APP_DEVICE_Connection_bPhoneActivateFinished(AT_DeviceId devId)
{
    /* check if all services are started */
    AT_DeviceId phoneDev    = APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE);

    if(phoneDev == devId)
    {
        return BT_TRUE;
    }
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandlePhoneStarted                  */
/**
    \brief      handles the phone started event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePhoneStarted(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStarted: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if(pPbsn->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {   /* seems we are finished */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
            }
            else
            {
                /* another device started its phone service :( retry or release with error ???? */
                // lets test with retry first
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(200), 0);
            }
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {                
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
    
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStarted: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandlePhoneStopped                  */
/**
    \brief      handles the phone stopped event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePhoneStopped(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStopped: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if(pPbsn->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {   /* seems we are finished */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
        case APP_DEVICE_PHONE_SERVICE_DEACTIVATION:
            /* set event to check for next state e.g. try to start the phone service on the wanted device */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(0), 0);
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStopped: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandlePhoneStartFailed              */
/**
    \brief      handles the phone start failed event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePhoneStartFailed(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStartFailed: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if(pPbsn->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {   /* reset state and try again */
                gAppDeviceContext.fct.serviceAct.state = APP_DEVICE_PHONE_STOP_SERVICE_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
            }
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStartFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandlePhoneStopFailed               */
/**
    \brief      handles the phone stop failed event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePhoneStopFailed(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStopFailed: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_PHONE_SERVICE_DEACTIVATION:
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            /* set event to check for next state e.g. retry */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePhoneStopFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleA2dpStarted                   */
/**
    \brief      handles the A2DP started event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleA2dpStarted(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStarted: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
        {
            if(pPbsn->device_id != gAppDeviceContext.fct.actPlayer.deviceId)
            {
                if(gAppDeviceContext.fct.actPlayer.state == APP_DEVICE_MDI_SERVICE_A2DP_START_SENT)

                {   /* set event to check for next state */
                    gAppDeviceContext.fct.actPlayer.bIsA2DPOfWrongDeviceStarted = BT_TRUE;
                }
                else
                {
                    /* retry to start the whole procedure again */
                     gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_SERVICE_NOT_SENT;
                }
            }

            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);

            break;
        }
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStarted: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleA2dpStopped                   */
/**
    \brief      handles the A2DP stopped event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleA2dpStopped(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStopped: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_DEACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.deactPlayer.deviceId)
            {   /* set event to check for next state */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            /* we might need to stop player service of unwanted device -> go on */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStopped: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleA2dpStartFailed               */
/**
    \brief      handles the A2DP start failed event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleA2dpStartFailed(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStartFailed: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {   /* start A2dp failed for wanted devId */
                gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_CONNECT_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
            }
            else
            {
                DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStartFailed: devId_pbsn: %d, devId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.actPlayer.deviceId);
            }
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStartFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleA2dpStopFailed                */
/**
    \brief      handles the A2DP stop failed event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleA2dpStopFailed(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStopFailed: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_DEACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.deactPlayer.deviceId)
            {   /* set event to check for next state e.g. retry */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            /* we might need to stop player service of unwanted device -> go on e.g. retry */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStopFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleAvrcpStarted                  */
/**
    \brief      handles the AVRCP started event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleAvrcpStarted(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStarted: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPbsn->device_id != gAppDeviceContext.fct.actPlayer.deviceId)
            {
               if(gAppDeviceContext.fct.actPlayer.state == APP_DEVICE_MDI_SERVICE_AVRCP_START_SENT)

                {   /* set event to check for next state */
                    gAppDeviceContext.fct.actPlayer.bIsAVRCPOfWrongDeviceStarted = BT_TRUE;
                }

               else if(gAppDeviceContext.fct.actPlayer.state != APP_DEVICE_MDI_SERVICE_AVRCP_START_SENT)
               {
                    /* if the a2dp start to the right device is sent, dont send it again the service to the wrong device stopped */
                    if(gAppDeviceContext.fct.actPlayer.state == APP_DEVICE_MDI_SERVICE_A2DP_START_SENT)
                    {   /* set event to check for next state */
                        gAppDeviceContext.fct.actPlayer.bIsA2DPOfWrongDeviceStarted = BT_TRUE;
                    }

                    /* retry to start the whole procedure again */
                    gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_SERVICE_NOT_SENT;
               }
            }

            /* service started for a device Id, set event to check for next step */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);

            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStarted: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleAvrcpStopped              */
/**
    \brief      handles the AVRCP stopped event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleAvrcpStopped(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStopped: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_DEACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.deactPlayer.deviceId)
            {   /* set event to check next step */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            /* we might need to stop player service of unwanted device -> go on */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStopped: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleAvrcpStartFailed              */
/**
    \brief      handles the AVRCP start failed event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleAvrcpStartFailed(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStartFailed: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
         case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {   /* service start failed for wanted device Id, check for next state */
                gAppDeviceContext.fct.actPlayer.state = APP_DEVICE_MDI_CONNECT_NOT_SENT;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(500), 0);
            }
            else
            {
                DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleA2dpStartFailed: devId_pbsn: %d, devId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.actPlayer.deviceId);
            }
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStartFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleAvrcpStopFailed               */
/**
    \brief      handles the AVRCP stop failed event from the target

    \param      pPbsn
                ERMNotificationPbsnType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleAvrcpStopFailed(const ERMNotificationPbsnType * pPbsn)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStopFailed: devId: %d, fctId: %d",
                        pPbsn->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_DEACTIVATE_PLAYER:
            if(pPbsn->device_id == gAppDeviceContext.fct.deactPlayer.deviceId)
            {   /* set event to check next step, e.g. retry */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            /* we might need to stop player service of unwanted device -> go on e.g. retry */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpStopFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandlePairingFailed                     */
/**
    \brief      handles the pairing failed event for a new device

    \param      pPprs
                ERMNotificationPprsType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePairingFailed(const ERMNotificationPprsType *pPprs)
{

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandlePairingFailed: devId: %d, fctId: %d",
                        pPprs->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPprs->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {
                /* seems remote side forgot the passwd so we cannot connect anymore */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if(pPprs->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {
                /* seems remote side forgot the passwd so we cannot connect anymore */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleAvrcpPairingFailed: no valid function type fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleConnectFailed                 */
/**
    \brief      handles the connection failed event for a device(PSBD:1)

    \param      pPsbd
                ERMNotificationPsbdType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleConnectFailed(const ERMNotificationPsbdType * pPbsd)
{   
    /* notification that connection on bt level failed */

    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleConnectFailed: devId: %d, fctId: %d",
                        pPbsd->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPbsd->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {
                /* first check if device is already connected */
                if(BT_TRUE == ERM_bDevConnectedOnBtLevel(gAppDeviceContext.fct.actPlayer.deviceId))
                {   /* device connected on bt level, anyway */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                }
                else
                {
                    /* might happen due to device not in neighborhood */
                    _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                }
            }
            // else ignore 
            break;
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if(pPbsd->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {
                /* first check if device is already connected */
                if(BT_TRUE == ERM_bDevConnectedOnBtLevel(gAppDeviceContext.fct.serviceAct.deviceId))
                {   /* device connected on bt level, anyway */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                }
                else
                {
                    /* might happen due to device not in neighborhood */
                    if((AT_INVALID_DEVICE_ID != APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK))||
                        (AT_INVALID_DEVICE_ID != APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_AVRCP)))
                    {   /* check if the MP of the old profile is still connected, if yes disconnect it additionally*/
                        gAppDeviceContext.fct.serviceAct.state = APP_DEVICE_PHONE_DISCONNECT_MP_DEVICE;
                        /* the BT device not connected successfully, check if MP is still connected */
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
                    }
                    else
                    {
                        _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                    }
                }
            }
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleConnectFailed: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleConnected                     */
/**
    \brief      handles the connected event for a device(PSBD:0)

    \param      pPsbd
                ERMNotificationPsbdType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleConnected(const ERMNotificationPsbdType *pPsbd)
{
    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleConnected: devId: %d, fctId: %d",
                        pPsbd->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPsbd->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if(pPsbd->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {   /* handle pending fct shall decide for the next state */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            else
            {
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            break;
        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleConnected: no valid functiontype fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_Connection_vHandleDisconnected                  */
/**
    \brief      handles the disconnected event for a device(PSBD:3)

    \param      pPsbd
                ERMNotificationPsbdType
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandleDisconnected(const ERMNotificationPsbdType *pPsbd)
{
    DEBUG_VALUE2(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleDisconnected: devId: %d, fctId: %d",
            pPsbd->device_id, gAppDeviceContext.fct.common.fctType);

    switch(gAppDeviceContext.fct.common.fctType)
    {
        case APP_DEVICE_DEACTIVATE_PLAYER:
            if(pPsbd->device_id == gAppDeviceContext.fct.deactPlayer.deviceId)
            {   /* definetly disconnected services */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
            }
            else
            {   /* see what is to be done next in the appropriate handle pending function */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_ACTIVATE_PLAYER:
            if(pPsbd->device_id == gAppDeviceContext.fct.actPlayer.deviceId)
            {
                /* device disconnected also we are trying to start services on it */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            } // else ignore disconnection
            else
            {   /* see what is to be done next in the appropriate handle pending function */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_PHONE_SERVICE_ACTIVATION:
            if((pPsbd->device_id == gAppDeviceContext.fct.serviceAct.deviceId)||
               (APP_DEVICE_PHONE_WAIT_FOR_MP_DEVICE_DISCONNECTED == gAppDeviceContext.fct.serviceAct.state))
            {   /* release context and inform user connection is not successfully established */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
            }
            else
            {   /* see what is to be done next in the appropriate handle pending function */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;
        case APP_DEVICE_PHONE_SERVICE_DEACTIVATION:
            if(pPsbd->device_id == gAppDeviceContext.fct.serviceAct.deviceId)
            {   /* definetly disconnected services */
                _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_RSP_OK, __LINE__);
            }
            else
            {   /* see what is to be done next in the appropriate handle pending function */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_DEVICE_FCT_PENDING, MSEC(100), 0);
            }
            break;

        default:
            /* ignore the fctType */
            DEBUG_VALUE1(APP_DEVICE_SWI_ERR, MOD_APP_DEVICE, "vHandleDisconnect: no valid function type fctId: %d",
                        gAppDeviceContext.fct.common.fctType);
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandlePhoneStateChanged                             */
/**
    \brief      handles connection state change of phone
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_Connection_vHandlePhoneNotification(const ERMNotificationPbsnType * pPbsn)
{
     switch(pPbsn->result)
    {
        case AT_SERVRES_START_SUCCEED:
            _APP_DEVICE_Connection_vHandlePhoneStarted(pPbsn);
            break;
        case AT_SERVRES_START_FAILED:
            _APP_DEVICE_Connection_vHandlePhoneStartFailed(pPbsn);
            break;


        case AT_SERVRES_STOP_SUCCEED:
            _APP_DEVICE_Connection_vHandlePhoneStopped(pPbsn);
            break;
        case AT_SERVRES_STOP_FAILED:
            _APP_DEVICE_Connection_vHandlePhoneStopFailed(pPbsn);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_vHandleHeadsetStateChanged                             */
/**
    \brief      handles connection state change of headset
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vHandleHeadsetStateChanged()
{
    /* see if we have a new connected phone */
    AT_DeviceId deviceId = ERM_u8GetActiveHeadSet();
    if( AT_INVALID_DEVICE_ID != deviceId )
    {
        /* get profile idx for deviceId and deviceType  */
        Uint8Type profileIdx = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(deviceId);
        const ERMPlayerHeadsetProfileType *profile = ERM_pGetPlayerHeadsetProfile(profileIdx);



        if(profile != NULL)
        {
            if(profile->deviceType == APP_DEVICE_enTYPE_HEADSET)
            {
                /* we need to update the profile list */
                gAppDeviceContext.newActHeadsetProfileIdx = profileIdx;
                if( _APP_DEVICE_sfUpdateErmActiveProfileTable(NULL) == APP_COMMON_RSP_ERROR_BUSY)
                {
                    gAppDeviceContext.updateActiveProfilePending = BT_TRUE;
                }
                /* check if there is an active user profile. if yes, associate
                 * the currently connected headset to this user profile */
                if( NULL != ERM_pGetActiveUserProfile())
                {   /* try to associated headset to user profile */
                    if( _APP_DEVICE_rcAssociateHeadsetToUser(NULL) == APP_COMMON_RSP_ERROR_BUSY )
                    {
                        gAppDeviceContext.associateHeadsetToUserPending = BT_TRUE;
                    }
                }
            }
        }
        else
        {
            /* active headset but no profile available for this device */
            _APP_DEVICE_vHandleNoHeadsetProfileAvailable();
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DEVICE_vHandlePlayerDisconnected                  */
/**
    \brief      not implemented

    \param      void
*/
/*----------------------------------------------------------------------------*/
void _APP_DEVICE_vHandlePlayerDisconnected(void)
{
    /* still no implementation for this function */
}

/* End Of File APP_DEVICE_Connection_C1.c */


