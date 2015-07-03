/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM.c
*-----------------------------------------------------------------------------
* Module Name:    External resource manager
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-12-15 13:03:30 +0100 (Do, 15 Dez 2011) $
* $Rev: 24617 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/src/ERM.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "osek.h"
#include "common.h"
#include "UART.h"
#include "ATParser.h"
#include "ATCmdHandler.h"
#include "POOL.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONEBOOK_ERM.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_SMS_ERM.h"
#include "APP_MDI_ERM.h"
#include "APP_MDI_BAP.h"
#include "APP_SD.h"
#include "APP_MDI.h"
#include "APP_MDI_CI.h"
#include "APP_DEVICE.h"
#include "APP_COMMON_Utils.h"
#include "OSEK_AppTask_H1.h"
#include "ATCommonData.h"
#include "APP_PHONE_BAP_REQUEST.h"
#include "CSHDL.h"
#include "VCR_Utils.h"
#include "APP_MDI_AT.h"
#include "APP_DTC.h"
#include "VCR_MessageHandler.h"
#include "ERM.id"
#include "TRACE.h"
#include "APP_DEVICE_AudioManager.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "OSEK_AppTask_Alarm.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_Utils.h"
#include "ERM_AudioSets.h"
#include "ERM_MessageHandler.h"
#include "ERM_DeviceHandler.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/

static void _ERM_Init(void);

#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)
static void _ERM_vIntAutoconnOffCallback(APP_COMMON_enResponseCode rsp);
static void _ERM_vIntDeleteAllCallback(APP_COMMON_enResponseCode rsp);
#endif

static APP_COMMON_enResponseCode _ERM_rcAddPlayerHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        APP_DEVICE_DeviceType deviceType,
        const Uint8Type *pName, Uint16Type length );

static APP_COMMON_enResponseCode _ERM_rcDeletePlayerHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile,
        APP_DEVICE_DeviceType deviceType);

static APP_COMMON_enResponseCode ERM_rcUpdateActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex,
                APP_DEVICE_DeviceType deviceType);


static APP_COMMON_enResponseCode _ERM_rcSetPlayerHeadsetAuthMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                APP_DEVICE_DeviceType deviceType,
                                Uint8Type profileIndex,
                                BooleanType bAuthNeeded);

static void _ERM_vReleaseEepromContext(APP_COMMON_enResponseCode rsp);
static APP_COMMON_enResponseCode _ERM_rcHandlePendingEepromMsg(void);

static BooleanType _ERM_bSendExtMsgCheckIsAllowed(void);

void _ERM_vOnCk5050PowerOff_Cb(APP_COMMON_enResponseCode rspCode);

static void _ERM_vOnA2dpVolumeSetCb(APP_COMMON_enResponseCode rsp);

static void _ERM_vResetSSPRequest(ERMSspReqType *pstSSpRequest);

static void _ERM_vJingleStateMachine(void);

#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE)  || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)
static void _ERM_vBluetoothTestmodeStateMachine(ERM_enBTTestmodeParams Parameter);
static void _ERM_vBluetoothTestmodeCallback(APP_COMMON_enResponseCode rsp);
static ERM_enBTTestmodeStates _ERM_BluetoothTestmodeState;
#endif

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

static Uint8Type            _ERM_bBTVisBarring     = ERM_BARRING_OFF;

static ATBTSWUpdateRecEnum _ERM_BluetoothUpdateRequest;
static ERM_enBTUpdateStates _ERM_BluetoothUpdateState;
static TickType _ERM_BluetoothUpdateStartSendFileTime;
static TickType _ERM_ManagerTime=0;
static Uint8Type _ERM_BluetoothUpdateActive = (Uint8Type)BT_FALSE;
static Uint8Type _ERM_sysPerfStartUpEventTraceDone = 0;
static Uint8Type _ERM_sysPerfBTAdddressTraceDone = 0;

static ERM_enSSPConfigStates _ERM_enSSPConfigStates;

static BooleanType bBTAckSignalActive = BT_FALSE;

extern ERMBTDeviceTableType _ERM_stBTDeviceTable[];


/* INIT is done by _ERM_Init !!! */
ERMContextType        _ERM_gContext =
{
    ERM_SW_UNDEF,             /* eCK5050SWVersion */
    FALSE,                    /* u8Active             */
    BT_FALSE,                 /* bStartupDone         */
    BT_FALSE,                 /* bReInitAudioParmeter */
    ERM_STATE_IDLE,           /* eState               */
    ERM_BTVIS_NONE,           /* eBTvisibilityAct     */
    BT_FALSE,                 /* bBluetoothVisible  */
    BT_TRUE,                  /* bBTVisIsInitial   */
    0,                        /* u8PlayConnectJingle     */
    0,                        /* u8layDisconnectJingle     */
    BT_FALSE,                 /* bEnableDisconnectPlayer   */
    BT_FALSE,                 /* bMuteACK */
    ERM_JINGLE_NO_JINGLE_ACTIVE,/* eJingleState  */
    0,                        /*    JingleStateTime */
    AT_INVALID_DEVICE_ID,     /*  u8PhoneIdConnected   */
    AT_INVALID_DEVICE_ID,     /* u8PlayerIdConnected */
    BT_FALSE,                 /* bSendA2dpAudioVolume */
    BT_FALSE,                 /* bSendVcrAudioVolume */
    BT_FALSE,                 /* bForceDisconnect */
    BT_FALSE,                 /* bForceDisconnectPlayer */
    BT_FALSE,                 /* bPrepareCK5050Off */
    NULL,                     /* pUartPtr             */
    APP_SUB_NONE,             /* eUartOwner           */
    0,                        /* u8StartupCnt         */
    0,                        /* reqRetryCnt          */
    ERM_REQ_NONE,             /* reqERMRequest        */
    ERM_REQ_NONE,             /* lastERMRequest       */
    NULL,                     /* *cb_atOkErrCBReq     */
    NULL,                     /* *cb_atOkErrCBLast    */
    AT_INVALID_DEVICE_ID,     /* u8ActiveBTId      */
    AT_INVALID_DEVICE_ID,     /* u8ActiveMDIId;       */
    AT_INVALID_DEVICE_ID,     /* u8ActiveHeadsetId;   */
    AT_INVALID_DEVICE_ID,     /* u8DeviceIdForMsg     */
    AT_INVALID_DEVICE_ID,     /* u8DeleteDeviceId     */
    AT_INVALID_DEVICE_ID,     /* u8LastPairedDeviceId */
    AT_INVALID_DEVICE_ID,     /* u8LastServiceDeviceId */
    {AT_INVALID_DEVICE_ID},   /* pu8ActiveUserList[AT_MAX_DEVICE_ID] */
    {0,0,0},                      /* stSWVersion          */
    {0},                      /* stPF_Code          */
    BT_FALSE,                 /* bPF_CodeStringAvail */
    BT_FALSE,                 /* bCKSWVersionStringAvail */
    {0,0,0,0,'\0'},           /* stBTPinCode          */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,'\0'}, /*stBTName     */
    1,                        /* u8TestMicMode - differential mic */
    0,                        /* u8TestMicGain        */
    BT_FALSE,                 /* bTestLoopbackOn      */
    AT_TEST_FREQ_TX_CHAN0,    /* eTestFreqMode        */
    {'0','0','0','0','0','0','0','0','0','0','0','0'}, /* btAddrAudioLoop */
    0,                        /* u16AudioLoopDelayLocal */
    0,                        /* u16AudioLoopDelayBT */
    NULL,                     /* cb_TestModeExt       */
    {{AT_REQ_UNDEFINED},0,0,
     AT_FALSE,AT_FALSE,0,0},  /* alternatingSignal */
    0,                        /* u8TotalInquiryResult     */
    0,                        /* u8PhoneInquiryResult     */
    0,                        /* u8PlayerInquiryResult     */
    0,                        /* u8HeadsetInquiryResult   */
    ERM_NO_RESET,             /* eResetState */
    { ERM_REQ_NONE,           /* req.enLastId */
      {0} },                  /* req.ctx */
    ERM_REQ_NONE,             /* ePendingEpromMsg */
    NULL,                     /* cb_epromWriteFinished */
    {ERM_EPR_WRITE_STATE_UNDEFINED,
     BT_FALSE, BT_FALSE, 0,
     BT_FALSE, BT_FALSE, 0,
     BT_FALSE, BT_FALSE, 0,
     BT_FALSE, BT_FALSE, BT_FALSE, BT_FALSE, BT_FALSE,
     {0}, {0}, {0}, {0}},     /* epr_tmp */
    ERM_CC_INVALID,           /* eConsistCheckState */
    {0},                      /* stBTAddress */
#if defined(ERM_USE_SIGNED_BT_UPDATE)
    {0},                      /* stMd5Key */
    {0},                      /* stMd5KeyEncryted*/
#endif
    BT_FALSE,                 /* bSecurityCheckOK */
    {BT_FALSE,{0},{0},{0}},   /* ERMBtChipVersionType */
    BT_FALSE,                 /* bDisconnectWillFollow */
    BT_FALSE,                 /* bgetImsi */
    BT_FALSE,                 /* isCDMAPhone */
    BT_TRUE,                  /* doPlayDisconnectJingleForPlayer */
    BT_FALSE,                 /* bgetProvider */
    APP_DEVICE_enTYPE_UNKNOWN, /*deviceType for Pairing */  
    ERM_EXT_ERR_NO_ERROR,      /* extendedErrorCode */
    BT_FALSE,
    {AT_INVALID_DEVICE_ID, AT_INQUIRY_DEVICE, {0}, 0}, /* stPairRequest */
    {AT_INVALID_DEVICE_ID, AT_INQUIRY_DEVICE, {0}, AT_SSP_ENABLE, AT_PARAM_SSP_IOCAP_INVALID },  /* stSspRequest */
    AT_SSP_ENABLE,                 /* eSSPMode */
    AT_PARAM_SSP_IOCAP_INVALID,     /* eSSPIoCap */
    BT_FALSE						/*RingtoneStatus*/
};

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Methods                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : ERM_Startup                                                  */
/**
    \brief      Trigger ERM shartup

    \brief      function API to allow system controller to trigger ERM shartup

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_Startup(void)
{
    SuccessFailType sF = SUCCESS;
    /* run ERM internal INIT */
    _ERM_Init();
    /* initialise the UART */
    _ERM_gContext.pUartPtr = UART2;
    /* apply required UART values */
    _ERM_vChangeState(ERM_STATE_START_UP);
    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"startup done");
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_sysPerfResetStartUpEventTraceDone                        */
/**
    \brief      Reset _ERM_sysPerfStartUpEventTraceDone variable

    \param      Uint8Type
                reset
                0

    \return     none
   */
/*----------------------------------------------------------------------------*/
void ERM_sysPerfResetStartUpEventTraceDone(Uint8Type reset)
{
    _ERM_sysPerfStartUpEventTraceDone = reset;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_Shutdown                                                 */
/**
    \brief      Trigger ERM shutdown

    \brief      function API to allow system controller to trigger ERM shutdown

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_Shutdown(void)
{
    /* we need to ensure that UART is freed and also that possible application
     * that ownes the UART get an error back to not block the application     */
    if (APP_SUB_NONE != _ERM_gContext.eUartOwner)
    {   /* someone has still blocked the UART - send error trigger and
           release UART */
        BooleanType ownerErm = BT_FALSE;
        switch (_ERM_gContext.eUartOwner)
        {   /* trigger respective application module if necessary */
            case APP_SUB_TEL:
            {
                APP_PHONE_vHandleErmATErrorEvent();
                break;
            }
            case APP_SUB_PHONEBOOK:
            {
                APP_PHONEBOOK_vHandleErmATError();
                break;
            }
            case APP_SUB_SMS:
            {
                APP_SMS_vHandleErmATError();
                break;
            }
            case APP_SUB_MDI:
            {
                APP_MDI_vHandleErmATResultEvent(APP_MDI_enResultError);
                break;
            }
            case APP_SUB_VCR:
            {
                _VCR_vHandleErmOkErrorEvent(BT_FALSE);
                break;
            }
            case APP_SUB_ERM:
            {
                ownerErm = BT_TRUE;
                 break;
            }
            case APP_SUB_NONE:
            default:
                /* do nothing */
                break;
        }
        /* now free the UART and stop supervision*/
        _ERM_vStopUartSupervision();
        _ERM_gContext.eUartOwner = APP_SUB_NONE;
        if (BT_TRUE == ownerErm)
        {
            (void)_ERM_sfHandleOkErrorRsp(AT_RSP_ERROR);
        }
    }
    /* we need to block the UART if APP want to access although ERM is shut
     * down, we do so via active indication */
    _ERM_gContext.u8Active            = FALSE;
    /* trigger state change to all applications - as idle is not
     * handled yet by application we use state ERM_STATE_CK5050_ACTIVE
     * to indicate that no BT device is available */
    _ERM_vChangeState(ERM_STATE_CK5050_ACTIVE);
    /* now change the state to idle and completely init the ERM data via
     * init */
    _ERM_Init();
    return SUCCESS;
}


SuccessFailType ERM_Shutdown_BTUpdate(void)
{
    /* we need to ensure that UART is freed and also that possible application
     * that ownes the UART get an error back to not block the application     */
    if (APP_SUB_NONE != _ERM_gContext.eUartOwner)
    {   /* someone has still blocked the UART - send error trigger and
           release UART */
        BooleanType ownerErm = BT_FALSE;
        switch (_ERM_gContext.eUartOwner)
        {   /* trigger respective application module if necessary */
            case APP_SUB_TEL:
            {
                APP_PHONE_vHandleErmATErrorEvent();
                break;
            }
            case APP_SUB_PHONEBOOK:
            {
                APP_PHONEBOOK_vHandleErmATError();
                break;
            }
            case APP_SUB_SMS:
            {
                APP_SMS_vHandleErmATError();
                break;
            }
            case APP_SUB_MDI:
            {
                APP_MDI_vHandleErmATResultEvent(APP_MDI_enResultError);
                break;
            }
            case APP_SUB_VCR:
            {
                _VCR_vHandleErmOkErrorEvent(BT_FALSE);
                break;
            }
            case APP_SUB_ERM:
            {
                ownerErm = BT_TRUE;
                 break;
            }
            case APP_SUB_NONE:
            default:
                /* do nothing */
                break;
        }
        /* now free the UART and stop supervision*/
        _ERM_vStopUartSupervision();
        _ERM_gContext.eUartOwner = APP_SUB_NONE;
        if (BT_TRUE == ownerErm)
        {
            (void)_ERM_sfHandleOkErrorRsp(AT_RSP_ERROR);
        }
    }
    /* we need to block the UART if APP want to access although ERM is shut
     * down, we do so via active indication */
    _ERM_gContext.u8Active            = FALSE;
    /* trigger state change to all applications - as idle is not
     * handled yet by application we use state ERM_STATE_CK5050_ACTIVE
     * to indicate that no BT device is available */

#if 0

    _ERM_vChangeState(ERM_STATE_CK5050_ACTIVE);
    /* now change the state to idle and completely init the ERM data via
     * init */
    _ERM_Init();
#endif
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_vCb3BM_BTVisibility                                      */
/**
    \brief      Callback for bluetooth visibility

    \brief      this CB is called by 3 button module manager to enable BT
                BT visibility again for given amount of time

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void            ERM_vCb3BM_BTVisibility(void)
{
    ERMNotificationType notification;
    notification.common.id = ERM_NOTIFY_BT_VISIBILITY_ON;

    DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"BT Visibility ON");

    _ERM_gContext.eBTvisibilityAct  = ERM_BTVIS_ON;
    /* trigger application task processing */
    (void)SetEvent(OsAppTask_event_100ms, evAppTaskERMPending);

    _ERM_vNotifyApplication(&notification);
#ifdef ERM_TEST_AUDIO
    _ERM_vMainTestFunction();
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_BTVisibilityBarringOn                                    */
/**
    \brief      Set Bluetooth visibility barring due to speed

    \brief      this API can be called from speed manager to barr / unbarr
                BT visibility setting

    \param      BooleanType value
                BT_TRUE  -> barring on
                BT_FALSE -> barring off

    \return     none
   */
/*----------------------------------------------------------------------------*/
void            ERM_BTVisibilityBarringOn(Uint8Type value)
{
    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"BT Visibility Barring %d -> %d", _ERM_bBTVisBarring, value);

    _ERM_bBTVisBarring = value;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_vBTVisibilityOff                                         */
/**
    \brief      API to immediately switch BT visibility off if it is on

    \brief      this API can be called e.g. by car speed manager if speed
                is increased so that BT visibility needs to be switched off

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void            ERM_vBTVisibilityOff(void)
{
#if defined (EMV_TEST_UHV_ALWAYS_BT_VISIBE)
    // no cancelling
    DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"EMV-TEST SW - STAY VISIBLE");

#else
    DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"BT Visibility OFF");

    /* we do so by setting visibility timer to 0 -> system trigger
     * expiry of the timer towards the application ERM running in and ERM
     * will send BT visibility off to CK5050                             */
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_BT_VISIBILITY_TIMER,
            0, 0);
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_FlashErase                                               */
/**
    \brief      API to trigger Flash erease towards CK5050

    \brief      the function set internal indication for ERM to trigger a
                flash erase towards CK5050. If the erase is started ERM will
                block all further messages towards CK5050 via UART control and
                also change to initial init state as CK5050 will do internal
                reset after the command is completely processed.
                Note: The command has priority over all other pending commands
                and will also clean up possible pending commands.

    \brief      If the API is called, ERM will check if CK5050 is active at all
                and only if so the command can be processed and function will return
                SUCCESS, otherwise function will return FAIL

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_FlashErase(void)
{
    SuccessFailType sF = FAIL;

    if (TRUE == _ERM_gContext.u8Active)
    {
        /* trigger flash erase */
        if (APP_COMMON_RSP_OK ==
            _ERM_SendMessage(ERM_REQ_FLASH_ERASE_USERDATA,NULL)
            )
        {
            DEBUG_TEXT(ERM_SWI_DBG, MOD_ERM,"Flash Erase OK");

            sF = SUCCESS;
        }
        else
        {
            DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"Flash Erase failed - no CK5050+");

        }
    }
    else
    {
        DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"Flash Erase failed - no CK5050+");

    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_enGetBTAddress                                           */
/**
    \brief      provide BT address if valid

    \brief

    \param      pointer to BR address array

    \return     APP_COMMON_enResponseCode

   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_enGetBTAddress(
        AT_BT_Address * p_address)
{
    APP_COMMON_enResponseCode enRet = APP_COMMON_RSP_OK;

    if (_ERM_gContext.eState == ERM_STATE_CK5050_ACTIVE ||
        _ERM_gContext.eState == ERM_STATE_CK5050_BT_ACTIVE)
    {   /* ADDRESS is valid */
        (void)memcpy(p_address,&_ERM_gContext.stBTAddress[0],AT_BT_ADDRESS_LEN);
    }
    else
    {
        enRet = APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }
    return enRet;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcResetToFactorySetting                                   */
/**
    \brief      trigger reset to factory settings

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode

   */
/*----------------------------------------------------------------------------*/
// TODO: check if we can move it to APP_DEVICE
APP_COMMON_enResponseCode ERM_rcResetToFactorySetting(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* first check if ERM is busy with other message, if so called
     * need to do a retry */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - call flasherase */
        retVal = _ERM_SendMessage(ERM_REQ_FLASH_ERASE_USERDATA,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_sFSetBluetoothSetPINCode                                 */
/**
    \brief      Set the BT Pin Code

    \brief

    \param      Uint16Type pincode - valid numbers between 0000 and 9999

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_sFSetBluetoothSetPINCode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type pincode)
{
    Uint32Type i = 0;
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if ERM is busy with other message, if so called
     * need to do a retry */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - check the range and set the string */
        if (ERM_BT_PIN_MAX >= pincode)
        {   /* convert the number to a number array */
            _ERM_gContext.stBTPinCode[0] = 0;
            _ERM_gContext.stBTPinCode[1] = 0;
            _ERM_gContext.stBTPinCode[2] = 0;
            _ERM_gContext.stBTPinCode[3] = 0;
            for (i = (Uint32Type)AT_UHV_BT_PIN_CODE_LEN ; i > (Uint32Type)0U ;i--)
            {
                _ERM_gContext.stBTPinCode[i-(Uint32Type)1U] = pincode % 10;
                pincode = pincode - _ERM_gContext.stBTPinCode[i-(Uint32Type)1U];
                if (pincode >= 10)
                {
                    pincode = pincode / 10;
                }
                else
                {
                    break;
                }
            }
            retVal = _ERM_SendMessage(ERM_REQ_HDL_PAIR_REQUEST,cb);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_sFSetBluetoothSetPINCodeDiagnose                         */
/**
    \brief      Set the BT Pin Code

    \brief

    \param      pau8Pincode - pointer to Uint8Type array.

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_sFSetBluetoothSetPINCodeDiagnose(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type *pau8Pincode)
{
    Uint32Type i = 0;
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if ERM is busy with other message, if so called
     * need to do a retry */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - check the range and set the string */
        /* convert the number to a number array */
        for (i = 0; i < (Uint32Type)AT_UHV_BT_PIN_CODE_LEN  ;i++)
        {
            _ERM_gContext.stBTPinCode[i] = pau8Pincode[i];
        }
        retVal = _ERM_SendMessage(ERM_REQ_HDL_PAIR_REQUEST,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcBTSWUpdateCK5050                                          */
/**
    \brief      Trigger Bluetooth SW Update towards CK5050+
                (Activates the software update capability)
    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcBTSWUpdateCK5050(
        void (*cb)(APP_COMMON_enResponseCode) )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if ERM is busy with other message, if so called
     * need to do a retry */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - trigger update message */
        retVal = _ERM_SendMessage(ERM_REQ_BLUETOOTH_SW_UPDATE,cb);
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetConnectionAuthorization                             */
/**
    \brief     This command is used to allow other devices to connect to the module.
                (Activates the software update capability)
    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
// TODO: or used, so delete
APP_COMMON_enResponseCode ERM_rcSetConnectionAuthorization(
        void (*cb)(APP_COMMON_enResponseCode) )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if ERM is busy with other message, if so called
     * need to do a retry */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - trigger update message */
        retVal = _ERM_SendMessage(ERM_REQ_BT_SET_CONN_AUTH,cb);
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_sFSetUniversalPreparationForMobileTelephone              */
/**
    \brief      Refresh the CK5050 BT name

    \brief

    \param      Uint8Type * data - pointer to string
    \param      Uint16Type dataLen - lenght of data - if more than supported the
                 string is shortened!!!

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_sFSetUniversalPreparationForMobileTelephone(
        void (*cb)(APP_COMMON_enResponseCode),
        const Uint8Type* data,
        Uint16Type dataLen)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if ERM is busy with other message, if so called
     * need to do a retry */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - check the range and set the string */
        if (0 < dataLen)
        {   /* take only strings with len > 0 into account */
            if (dataLen > ERM_BT_DEVICE_NAME_MAXLEN)
            {
                dataLen = ERM_BT_DEVICE_NAME_MAXLEN;
            }
            if ('\0' != *data && 0xFF != *data)
            {
                (void)memcpy(&(_ERM_gContext.stBTName[0]),data,dataLen);
                /* ensure '\0' termination */
                _ERM_gContext.stBTName[dataLen] = '\0';
                retVal = _ERM_SendMessage(ERM_REQ_SET_BT_NAME,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_sFSetBluetoothAcknowledgementSignal                      */
/**
    \brief      Set the bluetooth ack signal

    \brief

    \param      Uint8Type value

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_sFSetBluetoothAcknowledgementSignal(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    bBTAckSignalActive = value;
    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Diagnose set BT AcknowledgementSignal to %d", value);

    cb(retVal);

    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_bGetBTAckSignalActive                                    */
/**
    \brief      Get indication if BTAckSignal is active

    \brief

    \param      none

    \return     BooleanType BT_TRUE if active BT_FALSE otherwise
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bGetBTAckSignalActive(void)
{
    return bBTAckSignalActive;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_sFSetReferenceChannelDelay                               */
/**
    \brief      Set the reference channel delay

    \brief

    \param      Uint8Type value

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_sFSetReferenceChannelDelay(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (FALSE  == _ERM_gContext.u8Active)
    {   /* only modify the value if in proper range and that's it */
        if (SUCCESS == _ERM_sFSetAECDelay(value))
        {   /* we also need to call the CB */
            cb(APP_COMMON_RSP_OK);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    else
    {   /* we have an active device - send the updated value */
        if (  BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the index */
            if (SUCCESS == _ERM_sFSetAECDelay(value))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_ECHO_DELAY_CANCELLATION,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetTelephoneBasicVolume                                 */
/**
    \brief      Set communication volume with PVNS in parrot.

    \param      Uint8Type value - new value to be used

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetTelephoneBasicVolumeComm(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;



    /* Trace output due to Ticket 1312*/

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM_eSetTelephoneBasicVolume TEL_VOLUME %d", value);


    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
    {   /* only modify the value if in proper range and that's it */
        if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_COMM))
        {   /* we also need to call the CB */
            cb(APP_COMMON_RSP_OK);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    else
    {   /* we have an active device - send the updated value */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the index */
            if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_COMM))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetTelephoneBasicVolume                                 */
/**
    \brief      Set communication volume via ALC values. Have to send 2 messages
                to parrot. PALGOALCRX and PALGOSLRX.

    \param      Uint8Type value - new value to be used

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetTelephoneBasicVolumeAlc(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* Trace output due to Ticket 1312*/
    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM_eSetTelephoneBasicVolume TEL_VOLUME %d", value);


    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
    {   /* only modify the value if in proper range and that's it */
        if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_ALC))
        {   /* we also need to call the CB */
            cb(APP_COMMON_RSP_OK);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    else
    {   /* we have an active device - send the updated value */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the index */
            if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_ALC))
            {
                retVal = _ERM_SendMessage(ERM_REQ_SPK_RX_ALC,NULL);
                if (APP_COMMON_RSP_OK == retVal)
                {
                    /* with the final ERM message also call the cb from ERM */
                    retVal = _ERM_SendMessage(ERM_REQ_SPK_RX_SL,cb);
                }

            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetVoiceOutputBasicVolume                               */
/**
    \brief      Set Voice Output Volume

    \brief

    \param      Uint8Type value - new value to be used

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetVoiceOutputBasicVolume(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */

#if 0
    // Bugfix fuer Bobby Haferburg Beschwerde (E-Mail 04.03.2010):
    // For VR prompt volume it is not necessarsy to check if a device is connected
    if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
    {   /* only modify the value if in proper range and that's it */
        if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_PROMPT))
        {   /* we also need to call the CB */
            cb(APP_COMMON_RSP_OK);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    else
#endif
    {   /* we have an active device - send the updated value */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the index */
            if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_PROMPT))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetMediaDeviceBasicVolume                               */
/**
    \brief      Set Streaming volume

    \brief

    \param      Uint8Type value - new value to be used

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetMediaDeviceBasicVolume(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (FALSE  == _ERM_gContext.u8Active)
    {   /* only modify the value if in proper range and that's it */
        if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_STREAMING))
        {   /* we also need to call the CB */
            cb(APP_COMMON_RSP_OK);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    else
    {   /* we have an active CK5050 - send the updated value */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveMDIId)
        {   /* we do not have active mdi device */
            if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_STREAMING))
            {   /* we also need to call the CB */
                cb(APP_COMMON_RSP_OK);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
        else
        {   /* ERM is NOT busy and we have media device - try to modify the index */
            if (SUCCESS == _ERM_sFSetVolumes(value,AT_AUDIOTYPE_STREAMING))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetActiveDeviceRingtone                               */
/**
    \brief      Set Streaming volume

    \brief

    \param      Uint8Type value - new value to be used

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
// TODO: check if this can be moved to APP_DEVICE
APP_COMMON_enResponseCode ERM_eSetActiveDeviceRingtone(
        void (*cb)(APP_COMMON_enResponseCode),
        ATRintoneModeEnum ringtone)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
    {   /* we do not have an active device - return only error */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* we have an active device - check if ERM busy with MSG */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the value */
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ERM_eSetActiveDeviceRingtone %d", ringtone);
            if (SUCCESS == _ERM_eIntSetActiveDeviceRingtone(ringtone))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_AMGMT_SET_RINGTONE,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_ePlayRingtone                                            */
/**
    \brief      Plays or Stops a ringtone

    \brief

    \param      action - play or stop a ringtone
    \param      ringtone - id of the ringtone to play,
                           if action is stop the ringtone id is unused

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
// TODO: check if this can be moved to APP_DEVICE
APP_COMMON_enResponseCode ERM_ePlayRingtone(
        void (*cb)(APP_COMMON_enResponseCode),
        ATPprtActionType action,
        ATRintoneModeEnum ringtone)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    {   /* we have an active device - check if ERM busy with MSG */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to send the message */

            switch(action)
            {
                case AT_PPRT_ACT_STOP:
                case AT_PPRT_ACT_PLAY:
                case AT_PPRT_ACT_PLAY_ONCE:
                    /* these action are accepted, go on */
                    break;
                default:
                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "ERM_ePlayRingtone: invalid act %d", action);

                    return APP_COMMON_RSP_ERROR;
            }


            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "ERM_ePlayRingtone: action %d ringtone %d", action, ringtone);
            _ERM_gContext.req.ctx.ringtonePlay.action = action;
            _ERM_gContext.req.ctx.ringtonePlay.ringtoneId = ringtone;

            if(BT_TRUE ==_APP_SD_blastRingtonestate())//RT14853
            {
            	_ERM_gContext.bRingtonestatus = BT_TRUE;
            }
            else
            {
            	_ERM_gContext.bRingtonestatus = BT_FALSE;
            }

            retVal = _ERM_SendMessage(ERM_REQ_RINGTONE_PLAY,cb);

            if(retVal != APP_COMMON_RSP_OK)
            {
                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "ERM_ePlayRingtone: critical error %d", retVal);

            }
        }
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetActiveDeviceRingtoneAlternate                               */
/**
    \brief      Set the alternate ringtone support for active profile

    \brief

    \param      CB from caller

    \param      BooleanType ringAlternate - alternate ringtone active / not active

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
// TODO: check if this can be moved to APP_DEVICE
APP_COMMON_enResponseCode ERM_eSetActiveDeviceRingtoneAlternate(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType ringAlternate)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
    {   /* we do not have an active device - return only error */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* we have an active device - check if ERM busy with MSG */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the value */
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ERM_eSetActiveDeviceRing alternation %d", ringAlternate);

            if (SUCCESS == _ERM_eIntSetActiveDeviceRingtoneAlternate(ringAlternate))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_AMGMT_SET_RINGTONE_ALTERNATE,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStartInquiry                               */
/**
    \brief      Start Inquiry

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
// TODO: Should be moved to APP_DEVICE
APP_COMMON_enResponseCode ERM_rcStartInquiry(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed() )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "StartInquiry");

        // reset number of found devices
        _ERM_gContext.u8TotalInquiryResult   = 0;
        _ERM_gContext.u8PhoneInquiryResult   = 0;
        _ERM_gContext.u8HeadsetInquiryResult = 0;
        _ERM_gContext.u8PlayerInquiryResult  = 0;
        retVal = _ERM_SendMessage(ERM_REQ_START_INQUIRY,cb);
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcCancelInquiry                               */
/**
    \brief      cancel Inquiry

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
// TODO: Should be moved to APP_DEVICE
APP_COMMON_enResponseCode ERM_rcCancelInquiry(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed() )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to modify the value */
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "CancelInquiry");

        retVal = _ERM_SendMessage(ERM_REQ_CANCEL_INQUIRY,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcGetInquiryResult                                       */
/**
    \brief      get Inquiry result

    \brief

    \param      CB from caller

    \param      start
                Uint8Type

    \param      number
                Uint8Type

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcGetInquiryResult(
                           void (*cb)(APP_COMMON_enResponseCode),
                           APP_DEVICE_DeviceType deviceType,
                           Uint8Type startIndex,
                           Uint8Type number)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to modify the value */

        // check endIndex
        Uint8Type endIndex  = startIndex + number - 1;
        Uint8Type numDevs   = ERM_u8GetTotalInquiryResult();

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "GetInquiryResult, type: %d, start: %d", deviceType, startIndex);
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "GetInquiryResult, nr.: %d", number);


        if( endIndex > numDevs)
        {
            endIndex = numDevs;
        }

        // reset fct context
        (void)memset( &_ERM_gContext.req, 0, sizeof(_ERM_gContext.req));

        // we have to request all inquiry results
        _ERM_gContext.req.ctx.inquiry.startIndex = 1;
        _ERM_gContext.req.ctx.inquiry.endIndex   = ERM_u8GetTotalInquiryResult();

        // copy the requested startIndex and endIndex
        _ERM_gContext.req.ctx.inquiry.reqStartIndex = startIndex;
        _ERM_gContext.req.ctx.inquiry.reqEndIndex   = endIndex;
        _ERM_gContext.req.ctx.inquiry.recvdCount    = 0;
        _ERM_gContext.req.ctx.inquiry.deviceType    = deviceType;

        retVal = _ERM_SendMessage(ERM_REQ_INQUIRY_RESULT,cb);
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcInitiatePairing                                        */
/**
    \brief      Initiates a pairing

    \brief      Initiates a pairing

    \param      CB from caller

    \param      start
                Uint8Type

    \param      number
                Uint8Type

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcInitiatePairing(
        AT_DeviceId           deviceId,
        ATPairingDeviceType   deviceType,
        const ATPinCodeBT           pin,
        Uint8Type             numPinDigits,
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to modify the value */
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "InitiatePairing");


        // check length of pin
        numPinDigits = ( numPinDigits > AT_MAX_PIN_CODE_LEN ) ?
                        (Uint8Type)AT_MAX_PIN_CODE_LEN : numPinDigits;

        // reset fct context
        (void)memset( &_ERM_gContext.req, 0, sizeof(_ERM_gContext.req));

        // copy pin, device id, etc.
        _ERM_gContext.req.ctx.pairing.command       = AT_PAIR_CMD_INITIALTE;
        _ERM_gContext.req.ctx.pairing.deviceId      = deviceId;
        _ERM_gContext.req.ctx.pairing.deviceType    = deviceType;
        _ERM_gContext.req.ctx.pairing.pinAvailable  = BT_TRUE;
        _ERM_gContext.req.ctx.pairing.numPinDigits  = numPinDigits;
        (void)memcpy(&_ERM_gContext.req.ctx.pairing.pin, pin, numPinDigits);

        retVal = _ERM_SendMessage(ERM_REQ_PAIRING,cb);
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcAcceptPairing                                        */
/**
    \brief      Accepts a pairing

    \brief      Accepts a pairing

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcAcceptPairing(
        AT_DeviceId           deviceId,
        ATPairingDeviceType   deviceType,
        const ATPinCodeBT           pin,
        Uint8Type             numPinDigits,
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to modify the value */
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "AcceptPairing");

        // check length of pin
        numPinDigits = ( numPinDigits > AT_MAX_PIN_CODE_LEN ) ?
                        (Uint8Type)AT_MAX_PIN_CODE_LEN : numPinDigits;

        // reset fct context
        (void)memset( &_ERM_gContext.req, 0, sizeof(_ERM_gContext.req));

        // copy pin, device id, etc.
        _ERM_gContext.req.ctx.pairing.command       = AT_PAIR_CMD_ACCEPT;
        _ERM_gContext.req.ctx.pairing.deviceId      = deviceId;
        _ERM_gContext.req.ctx.pairing.deviceType    = deviceType;
        _ERM_gContext.req.ctx.pairing.numPinDigits  = numPinDigits;
        (void)memcpy(&_ERM_gContext.req.ctx.pairing.pin, pin, numPinDigits);

        if(numPinDigits > 0)
        {
            _ERM_gContext.req.ctx.pairing.pinAvailable  = BT_TRUE;
        }
        else
        {
            _ERM_gContext.req.ctx.pairing.pinAvailable  = BT_FALSE;
        }

        retVal = _ERM_SendMessage(ERM_REQ_PAIRING,cb);

        /* reset the data of possible SSP request */
        _ERM_vResetSSPRequest(&_ERM_gContext.stSspRequest);
    }
    return retVal;

}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcRejectPairing                                        */
/**
    \brief      Rejects a pairing

    \brief      Rejects a pairing

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcRejectPairing(
        AT_DeviceId           deviceId,
        ATPairingDeviceType   deviceType,
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to modify the value */
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "RejectPairing");

        // reset fct context
        (void)memset( &_ERM_gContext.req, 0, sizeof(_ERM_gContext.req));

        // device id, etc.
        _ERM_gContext.req.ctx.pairing.command       = AT_PAIR_CMD_REJECT;
        _ERM_gContext.req.ctx.pairing.deviceId      = deviceId;
        _ERM_gContext.req.ctx.pairing.deviceType    = deviceType;
        _ERM_gContext.req.ctx.pairing.pinAvailable  = BT_FALSE;

        retVal = _ERM_SendMessage(ERM_REQ_PAIRING,cb);
        /* reset the data of possible SSP request */
        _ERM_vResetSSPRequest(&_ERM_gContext.stSspRequest);
    }
    return retVal;

}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStopPhoneService                                */
/**
    \brief      disconnects the phone service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStopPhoneService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StopPhoneService: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.discPhoneServ.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_STOP_PHONE_SERVICE,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStartPhoneService                                */
/**
    \brief      connects the phone service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStartPhoneService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StartPhoneService: %d", deviceId);
        // save device id
        _ERM_gContext.req.ctx.connPhoneServ.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_START_PHONE_SERVICE,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStopPlayerAvrcpService                                */
/**
    \brief      disconnects the avrcp player service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStopPlayerAvrcpService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StopAvrcpService: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.discPlayerAvrcpServ.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_STOP_PLAYER_AVRCP_SERVICE,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStartPlayerAvrcpService                                */
/**
    \brief      connects the avrcp service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStartPlayerAvrcpService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

        /* check if ERM busy with MSG */
        if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to start */
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StartPhoneService: %d", deviceId);

            // save device id
            _ERM_gContext.req.ctx.connPlayerAvrcpServ.deviceId = deviceId;
            retVal = _ERM_SendMessage(ERM_REQ_START_PLAYER_AVRCP_SERVICE,cb);
        }
        return retVal;
    }

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStopPlayerA2dpSinkService                                */
/**
    \brief      disconnects the s2dp sink player service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStopPlayerA2dpSinkService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StopA2dpSinkService: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.discPlayerA2dpSinkServ.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_STOP_PLAYER_A2DP_SINK_SERVICE,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStartPlayerA2dpService                                */
/**
    \brief      connects the a2dp service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStartPlayerA2dpService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
        if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to start */
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StartA2dpService: %d", deviceId);

            // save device id
            _ERM_gContext.req.ctx.connPlayerA2dpSinkServ.deviceId = deviceId;
            retVal = _ERM_SendMessage(ERM_REQ_START_PLAYER_A2DP_SINK_SERVICE,cb);
        }
        return retVal;
    }

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStopHeadsetService                                */
/**
    \brief      disconnects the headset service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStopHeadsetService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed() )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StopHeadSetService: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.discHeadsetServ.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_STOP_HEADSET_SERVICE,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcStartHeadsetService                                */
/**
    \brief      starts the headset service of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcStartHeadsetService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed() )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "StartHeadSetService: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.startHeadsetServ.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_START_HEADSET_SERVICE,cb);
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcConnectDevice                                          */
/**
    \brief      connects to a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcConnectDevice(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ConnectDevice: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.connectDevice.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_CONNECT_DEVICE,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcDisconnectDevice                                          */
/**
    \brief      disconnects a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcDisconnectDevice(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "DisconnectDevice: %d", deviceId);

        // save device id
        _ERM_gContext.req.ctx.disconnectDevice.deviceId = deviceId;
        //RT14519
        /* Delay Device disconnect during CKCM shutdown to send pause command*/
        if(_ERM_gContext.bPrepareCK5050Off == BT_TRUE)
        {
        	AppTask_vSetAlarm(APP_TASK_ALARM_ERM_SHUTDOWN_PAUSE_RESPONSE_TIMEOUT, MSEC(600), ERM_cbPauseSuhutdownDelaytimeOut);
        }
        else
        {
        	retVal = _ERM_SendMessage(ERM_REQ_DISCONNECT_DEVICE,cb);
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcDeactivatePhoneServiceAutoConn                         */
/**
    \brief      disables autoconenction of phone service

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcDeactivatePhoneServiceAutoConn(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "DeactivatePhoneServiceAutoConn");

        retVal = _ERM_SendMessage(ERM_REQ_DISABLE_PHONE_AUTO_CONN,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetAutoConnMode                                        */
/**
    \brief
    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode ERM_rcSetAutoConnMode( BooleanType enable,
                                                void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */

        if( enable == BT_TRUE )
        {
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SetAutoConnMode: on");

            retVal = _ERM_SendMessage(ERM_REQ_AUTOCONN_ON,cb);
        }
        else
        {

            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SetAutoConnMode: off");
            retVal = _ERM_SendMessage(ERM_REQ_TEST_AUTOCONN_OFF,cb);
        }

    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcDeleteDevice                                */
/**
    \brief      deletes pairing and associated user data of a given device id

    \brief

    \param      deviceId
                Uint16Type

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcDeleteDevice(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type deviceId)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "DeleteDevice pairing: %d", deviceId);
        // save device id
        _ERM_gContext.req.ctx.deleteDevice.deviceId = deviceId;
        retVal = _ERM_SendMessage(ERM_REQ_DELETE_DEVICE_ID,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetPlayCallerNameMode                                */
/**
    \brief      set current mode for playing caller name of a contact

    \brief

    \param      bPlayCallerName
                BooleanType

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetPlayCallerNameMode(void (*cb)(APP_COMMON_enResponseCode),
                                                      BooleanType bPlayCallerName)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetPlayerCallerNameMode: %d", bPlayCallerName);
        // save mode
        _ERM_gContext.req.ctx.setPlayCallerName.bPlayCallerName = bPlayCallerName;
        retVal = _ERM_SendMessage(ERM_REQ_SET_PLAY_CALLER_NAME,cb);
    }
    return retVal;
}
#if 0
/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcGetPlayCallerNameMode                                */
/**
    \brief      get current mode for playing caller name of a contact

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcGetPlayCallerNameMode(
                                        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* check if ERM busy with MSG */
    if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - try to start */

        // reset mode
        _ERM_gContext.req.ctx.getPlayCallerName.bPlayCallerName = BT_FALSE;
        retVal = _ERM_SendMessage(ERM_REQ_GET_PLAY_CALLER_NAME,cb);
    }
    return retVal;
}
#endif

APP_COMMON_enResponseCode ERM_rcUpdatePlayerActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex)
{

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "UpdatePlayerActiveProfileList: %d", profileIndex);
    return ERM_rcUpdateActiveProfileList(cb, profileIndex,
                                    APP_DEVICE_enTYPE_PLAYER);
}

APP_COMMON_enResponseCode ERM_rcUpdateHeadsetActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex)
{

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "UpdateHeadsetActiveProfileList: %d", profileIndex);
    return ERM_rcUpdateActiveProfileList(cb, profileIndex,
                                    APP_DEVICE_enTYPE_HEADSET);
}

APP_COMMON_enResponseCode ERM_rcUpdatePhoneActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex)
{

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "UpdatePhoneActiveProfileList: %d", profileIndex);
    return ERM_rcUpdateActiveProfileList(cb, profileIndex,
                                    APP_DEVICE_enTYPE_PHONE);
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcUpdatePhoneActiveProfileList                                */
/**
    \brief      update active user profile list with deviceId
    \brief

    \param      CB from caller

    \param      profileIndex
                Uint8Type
                new active profile

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcUpdateActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex,
                APP_DEVICE_DeviceType deviceType)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    const ERMBTDeviceTableType *devInfo    = NULL;
    const ERMPhysicalBTDeviceType *phyInfo = NULL;
    AT_DeviceId deviceIndex = AT_INVALID_DEVICE_ID;

    /* 1. check if there is a valid profile */
    switch( deviceType )
    {
        case APP_DEVICE_enTYPE_PHONE:
        {
            const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);
            if( NULL == profile || AT_INVALID_DEVICE_ID == profile->deviceIndex)
            {   /* not a valid profile */
                return APP_COMMON_RSP_ERROR;
            }
            deviceIndex = profile->deviceIndex;
            break;
        }
        case APP_DEVICE_enTYPE_HEADSET:
        case APP_DEVICE_enTYPE_PLAYER:
         {
            const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile(profileIndex);
            if( NULL == profile || AT_INVALID_DEVICE_ID == profile->deviceIndex)
            {   /* not a valid profile */
                return APP_COMMON_RSP_ERROR;
            }
            deviceIndex = profile->deviceIndex;
            break;
        }
        default:
            return APP_COMMON_RSP_ERROR;
    }

    /* 2. check if deviceIndex is existing in DeviceTable */
    devInfo = ERM_pGetDevice(deviceIndex);
    phyInfo = ERM_pGetPhyDevice(deviceIndex);
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == devInfo ||
             NULL == phyInfo ||
             devInfo->device_state != ERM_DEVICE_STATE_USED)
    {   /* not valid */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {
        /* prepare the eeprom access */
        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "UpdateActiveProfileList type: %d, index: %d", deviceType, deviceIndex);
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddActiveProfile(profileIndex,
                                                                 deviceType))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_UPDATE_PROFILE_LIST))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcAddUserProfile_InEeprom                                */
/**
    \brief      add user profile

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcAddUserProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if deviceId is existing in DeviceTable */
    const ERMBTDeviceTableType *devInfo    = ERM_pGetDevice(deviceId);
    const ERMPhysicalBTDeviceType *phyInfo = ERM_pGetPhyDevice(deviceId);
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == devInfo ||
             NULL == phyInfo ||
             devInfo->device_state != ERM_DEVICE_STATE_USED ||
             BT_TRUE == ERM_bMaxNumberOfUserProfilesReached())
    {   /* not valid */
        return APP_COMMON_RSP_ERROR;
    }
    /* 2. check if adding a new user profile is possible */
    else
    {
        /* prepare the eeprom access */
        Uint8Type freeIndex             = AT_INVALID_DEVICE_ID;
        Uint8Type playerProfileIndex    = AT_INVALID_DEVICE_ID;
        Uint8Type headsetProfileIndex   = AT_INVALID_DEVICE_ID;

        /* check for active headset / player to associate it to user profile */
        const ERMPlayerHeadsetProfileType * headset = ERM_pGetActiveHeadsetProfile();
        const ERMPlayerHeadsetProfileType * player = ERM_pGetActivePlayerProfile();
        if( headset != NULL )
        {
            headsetProfileIndex = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(headset->deviceIndex);
        }
        if( player != NULL )
        {
            playerProfileIndex = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(player->deviceIndex);
        }


        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddUserProfile(deviceId, &freeIndex, playerProfileIndex, headsetProfileIndex))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddPhysicalDevice(deviceId,
                             pName, length))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }
        /* prepare the eeprom access */
        
        
        if (deviceId == ERM_u8GetActivePhone())
        {
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddActiveProfile(freeIndex,
                                                                     APP_DEVICE_enTYPE_PHONE))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
        }
        else
        {
            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "don´t set userprofile with device with id %d as active because another profilewith id , %d is active", deviceId, ERM_u8GetActivePhone());          
            
            
             if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddActivePhoneProfileWhenNotConnectedAsPhone(freeIndex))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            } 
        }
        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_ADD_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "AppUserProfile_InEeprom %d, %d", freeIndex, deviceId);
    }
    return retVal;


}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcDeleteUserProfile_InEeprom                                */
/**
    \brief      delete user profile

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcDeleteUserProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    const ERMBTDeviceTableType *devInfo    = NULL;
    const ERMPhysicalBTDeviceType *phyInfo = NULL;

    /* 1. check if deviceId is existing in DeviceTable */
    const ERMUserProfileType * profile = ERM_pGetUserProfile(indexOfProfile);
    if( NULL == profile )
    {   /* profile does not exists */
        return APP_COMMON_RSP_ERROR;
    }
    devInfo     = ERM_pGetDevice(profile->deviceIndex);
    phyInfo     = ERM_pGetPhyDevice(profile->deviceIndex);
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == devInfo ||
             NULL == phyInfo ||
             devInfo->device_state != ERM_DEVICE_STATE_USED )
    {   /* not valid */
        retVal = APP_COMMON_RSP_ERROR;
    }
    /* 2. prepare deletion of user profile */
    else
    {   /* fetch user profile to be deleted to be deleted */

        if (AT_INVALID_DEVICE_ID != indexOfProfile &&
            ERM_MAX_NUM_USER_PROFILES > indexOfProfile)
        {
            const ERMPlayerHeadsetProfileType * playHsProfile = NULL;
            Uint8Type playHsProfileIdx = AT_INVALID_DEVICE_ID;

            /* prepare the eeprom access */
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeleteUserProfile((Uint8Type)indexOfProfile))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeletePhysicalDevice(profile->deviceIndex))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeleteActiveProfile((Uint8Type)indexOfProfile, APP_DEVICE_enTYPE_PHONE))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
            // Do we need to delete player / headset profile too?
            playHsProfile = ERM_pGetPlayerHeadsetProfile_DevId(profile->deviceIndex);
            playHsProfileIdx = ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(profile->deviceIndex);
            if( NULL != playHsProfile )
            {   /* there is a player / headset profile for deviceId which needs to be deleted , too */
                if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeletePlayerHeadsetProfile(playHsProfileIdx))
                {
                    _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                    return APP_COMMON_RSP_ERROR;
                }
                if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeleteActiveProfile(playHsProfileIdx, playHsProfile->deviceType))
                {
                    _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                    return APP_COMMON_RSP_ERROR;
                }
            }

            /* set pending msg and callback and start */
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_DELETE_USER_PROFILE))
            {
                _ERM_gContext.cb_epromWriteFinished        = NULL;
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }


        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "DeleteUserProfile_InEeprom: %d, %d", indexOfProfile, profile->deviceIndex);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcAddHeadsetProfile_InEeprom                                */
/**
    \brief      add player profile

    \param      CB from caller
    \param      deviceId
                AT_DeviceId
                Id of device associated with new profile
    \param      pName
                const Uint8Type *
                Name of Profile / associated device
    \param      length
                Uint16Type
                length of device / profile name

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcAddHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    return _ERM_rcAddPlayerHeadsetProfile_InEeprom(cb, deviceId,
                                         APP_DEVICE_enTYPE_HEADSET,
                                         pName, length);
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcAddPlayerProfile_InEeprom                                */
/**
    \brief      add player profile

    \param      CB from caller
    \param      deviceId
                AT_DeviceId
                Id of device associated with new profile
    \param      pName
                const Uint8Type *
                Name of Profile / associated device
    \param      length
                Uint16Type
                length of device / profile name

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode ERM_rcAddPlayerProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length )
{
    return _ERM_rcAddPlayerHeadsetProfile_InEeprom(cb, deviceId,
                                         APP_DEVICE_enTYPE_PLAYER,
                                         pName, length);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcAddPlayerHeadsetProfile                                */
/**
    \brief      add player / headset profile, set type as given by caller

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcAddPlayerHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        APP_DEVICE_DeviceType deviceType,
        const Uint8Type *pName, Uint16Type length )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if deviceId is existing in DeviceTable */
    const ERMBTDeviceTableType *devInfo    = ERM_pGetDevice(deviceId);
    const ERMPhysicalBTDeviceType *phyInfo = ERM_pGetPhyDevice(deviceId);
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == devInfo ||
             NULL == phyInfo ||
             devInfo->device_state != ERM_DEVICE_STATE_USED ||
             BT_TRUE == ERM_bMaxNumberOfPlayerHeadsetProfilesReached())
    {   /* not valid */
        // TODO: delete the one which has not been used for the longest period
        // TODO: do not delete player profiles / devices which are used in user
        // profile table !!!!!!!!!!!
        // User _ERM_gContext.ePendingEpromMsg = ERM_REQ_ADD_PLAYER_HEADSET or similar to
        // mark that we need to call _ERM_rcAddPlayerHeadsetProfile again when
        // delete was done
        //_ERM_gContext.ePendingEpromMsg    = ERM_REQ_ADD_PLAYER_HEADSET_PROFILE;

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "AddPlayerHeadsetProfile_InEeprom: fail to add %d, %d", deviceId, deviceType);

        // FOR NOW: Just return an error
        retVal = APP_COMMON_RSP_ERROR;
    }
    /* 2. check if adding a new headset / player profile is possible */
    else
    {
        Uint8Type freeIndex             = AT_INVALID_DEVICE_ID;
        const ERMUserProfileType * user = NULL;

        /* check for valid deviceType */
        switch(deviceType)
        {
            case APP_DEVICE_enTYPE_HEADSET:
            case APP_DEVICE_enTYPE_PLAYER:
                /* go on */
                break;
            default:
                /* invalid deviceType */
                return APP_COMMON_RSP_ERROR;
         }


        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddPlayerHeadsetProfile(deviceId, deviceType, &freeIndex))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddPhysicalDevice(deviceId, pName, length))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_AddActiveProfile(freeIndex,
                                                                 deviceType))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* check if there is an active user */
        user = ERM_pGetActiveUserProfile();
        if( NULL != user )
        {
            // add new player / headset profile index to user profile
            APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
            Uint8Type activeUserIdx = ERM_u8GetUserProfileIndexForDeviceId(user->deviceIndex);

            switch( deviceType )
            {
                case APP_DEVICE_enTYPE_PLAYER:
                    rsp = _ERM_rcPrepare_ChangeUserProfile_AssociatedPlayer(activeUserIdx, freeIndex, BT_TRUE);
                    break;
                case APP_DEVICE_enTYPE_HEADSET:
                    rsp = _ERM_rcPrepare_ChangeUserProfile_AssociatedHeadset(activeUserIdx, freeIndex);
                    break;
                default:
                    /* no nothing */
                    break;
            }
            (void)rsp;
            /* ignore problems, cause player / headset profile shall be written
             * to eeprom in any case */
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_ADD_PLAYER_HEADSET_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "AddPlayerHeadsetProfile_InEeprom: freeIndex %d, deviceId %d", freeIndex, deviceId);
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "AddPlayerHeadsetProfile_InEeprom: deviceType %d", deviceType);
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_rcDeletePlayerProfile_InEeprom                                */
/**
    \brief      delete player profile from eeprom

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcDeletePlayerProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile)
{
    return _ERM_rcDeletePlayerHeadsetProfile_InEeprom (
            cb, indexOfProfile, APP_DEVICE_enTYPE_PLAYER);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_rcDeleteHeadsetProfile_InEeprom                                */
/**
    \brief      delete headset profile from eeprom

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcDeleteHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile)
{
    return _ERM_rcDeletePlayerHeadsetProfile_InEeprom (
            cb, indexOfProfile, APP_DEVICE_enTYPE_HEADSET);
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_rcDeletePlayerHeadsetProfile_InEeprom                                */
/**
    \brief      delete player / headset profile from eeprom

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcDeletePlayerHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile,
        APP_DEVICE_DeviceType deviceType)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    const ERMBTDeviceTableType *devInfo         = NULL;
    const ERMPhysicalBTDeviceType *phyInfo      = NULL;
    const ERMPlayerHeadsetProfileType * profile = NULL;

    switch( deviceType )
    {
        case APP_DEVICE_enTYPE_HEADSET:
        case APP_DEVICE_enTYPE_PLAYER:
            /* go on */
            break;
        default:
            return APP_COMMON_RSP_ERROR;
    }

    /* 1. check if deviceId is existing in DeviceTable */
    profile = ERM_pGetPlayerHeadsetProfile((Uint8Type)indexOfProfile);
    if( NULL == profile )
    {   /* profile does not exists */
        return APP_COMMON_RSP_ERROR;
    }
    if( deviceType != profile->deviceType )
    {   /* wrong device type */
        return APP_COMMON_RSP_ERROR;
    }
    devInfo     = ERM_pGetDevice(profile->deviceIndex);
    phyInfo     = ERM_pGetPhyDevice(profile->deviceIndex);
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == devInfo ||
             NULL == phyInfo ||
             devInfo->device_state != ERM_DEVICE_STATE_USED )
    {   /* not valid */
        retVal = APP_COMMON_RSP_ERROR;
    }
    /* 2. prepare deletion of user profile */
    else
    {   /* fetch profile */
        if (AT_INVALID_DEVICE_ID != indexOfProfile &&
                ERM_MAX_NUM_PLAYER_HS_TABLE > indexOfProfile)
        {
            Uint8Type userProfileIdx = AT_INVALID_DEVICE_ID;
            /* prepare the eeprom access */
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeletePlayerHeadsetProfile((Uint8Type)indexOfProfile))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeletePhysicalDevice(profile->deviceIndex))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeleteActiveProfile(profile->deviceIndex, deviceType ))
            {
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }
            // Do we need to delete user profile too?
            userProfileIdx = ERM_u8GetUserProfileIndexForDeviceId(profile->deviceIndex);
            if( ERM_MAX_NUM_USER_PROFILES > userProfileIdx )
            {
                if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeleteUserProfile(userProfileIdx))
                {
                    _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                    return APP_COMMON_RSP_ERROR;
                }
                if( APP_COMMON_RSP_OK != _ERM_rcPrepare_DeleteActiveProfile(profile->deviceIndex, APP_DEVICE_enTYPE_PHONE ))
                {
                    _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                    return APP_COMMON_RSP_ERROR;
                }
            }

            /* set pending msg and callback and start */
            if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_DELETE_PLAYER_HEADSET_PROFILE))
            {
                _ERM_gContext.cb_epromWriteFinished        = NULL;
                _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
                return APP_COMMON_RSP_ERROR;
            }


            DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM, "DeletePlayerHeadsetProfile_InEeprom: %d,%d, %d, %d",indexOfProfile, profile->deviceIndex, deviceType, userProfileIdx);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetVoiceMailboxNumber_InEeprom                         */
/**
    \brief      add user profile

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetVoiceMailboxNumber_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pNumber, Uint16Type length )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile )
    {   /* no valid profile */
        retVal = APP_COMMON_RSP_ERROR;
    }
    /* 2. change voice mailbox number for profile */
    else
    {
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_Mailboxnumber(profileIndex, pNumber, length))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetVoiceMailboxNumber_InEeprom: %d",profileIndex);
    }
    return retVal;

}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetRingtoneAlternate_InEeprom                                */
/**
    \brief      set in eeprom whether the callername and the ringtone shall
                be alternated when a call is incoming

    \brief

    \param      CB from caller
    \param      profileIndex
                user profile for which the alternate mode should be changed

    \param      bAlternate
                BooleanType
                alternate or not

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetRingtoneAlternate_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                BooleanType bAlternate)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile )
    {   /* no valid profile */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_RingtoneAlternate(profileIndex, bAlternate))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetRingtoneAlternate__InEeprom: %d",profileIndex);

    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetUserAssociatedPlayer_InEeprom                                */
/**
    \brief      set player associated to user profile
    \brief

    \param      cb
                CB from caller

    \param      userProfileIndex
                Uint8Type
                user profile the headset shall be associated to

    \param      playerProfileIndex
                Uint8Type
                player profile index

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetUserAssociatedPlayer_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type userProfileIndex,
                                Uint8Type playerProfileIndex,
                                BooleanType bConsiderInvalidPlayer)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile                  = ERM_pGetUserProfile(userProfileIndex);
    const ERMPlayerHeadsetProfileType * playerProfile   = ERM_pGetPlayerHeadsetProfile(playerProfileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile  || (bConsiderInvalidPlayer && NULL == playerProfile ))
    {   /* no valid profiles */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_AssociatedPlayer(userProfileIndex, playerProfileIndex,bConsiderInvalidPlayer))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "SetUserAssociatedPlayer_InEeprom: %d, %d",userProfileIndex, playerProfileIndex);
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetUserAssociatedHeadset_InEeprom                                */
/**
    \brief      set headset associated to user profile
    \brief

    \param      cb
                CB from caller

    \param      userProfileIndex
                Uint8Type
                user profile the headset shall be associated to

    \param      headsetProfileIndex
                Uint8Type
                headset profile index

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetUserAssociatedHeadset_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type userProfileIndex,
                                Uint8Type headsetProfileIndex)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile              = ERM_pGetUserProfile(userProfileIndex);
    const ERMPlayerHeadsetProfileType * hsProfile   = ERM_pGetPlayerHeadsetProfile(headsetProfileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile || NULL == hsProfile )
    {   /* no valid profiles */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_AssociatedHeadset(userProfileIndex, headsetProfileIndex))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "SetUserAssociatedHeadset_InEeprom: %d, %d",userProfileIndex, headsetProfileIndex);
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetRingtone_InEeprom                                */
/**
    \brief      set ringtone in eeprom

    \brief

    \param      CB from caller
    \param      profileIndex
                user profile for which the ringtone should be changed
    \param      ringtone
                ATRintoneModeEnum
                new ringtone

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetRingtone_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                ATRintoneModeEnum ringtone)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile )
    {   /* no valid profile */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_Ringtone(profileIndex, ringtone))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetRingtone_InEeprom: %d",profileIndex);

    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcChangeDeviceName_InEeprom                                */
/**
    \brief      change deviceName in eeprom

    \brief      change deviceName in eeprom

    \param      CB from caller

    \param      deviceId
                AT_DeviceId
                deviceId name should be changed for

    \param      name
                AT_DeviceNameString
                new name for device

    \param      nameLen
                Uint16Type
                length of new name

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcChangeDeviceName_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                AT_DeviceId deviceId,
                                AT_DeviceNameString name,
                                Uint16Type nameLen)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( ERM_MAX_NUM_PHYSICAL_DEVICES <= deviceId )
    {
        return APP_COMMON_RSP_ERROR_INVALID_DEVICE_ID;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangePhyTable_Name(deviceId, name, nameLen))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_PHY_TABLE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ChangeDeviceName_InEeprom: %d",deviceId);
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetPbCompletelyLoaded_InEeprom                         */
/**
    \brief      set pb completely loaded in eeprom

    \brief      set pb completely loaded in eeprom

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetPbCompletelyLoaded_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile )
    {   /* no valid profile */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_PbCompletelyLoaded(profileIndex))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetPbCompletelyLoaded_InEeprom: %d",profileIndex);
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetAudioMode_InEeprom                                */
/**
    \brief      set audio mode

    \brief

    \param      bAllAudio
                BooleanType

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetAudioMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                ERMUserAudioModeType audioMode)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid user profile  */
    const ERMUserProfileType * profile = ERM_pGetUserProfile(profileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile )
    {   /* no valid profile */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangeUserProfile_AudioMode(profileIndex, audioMode))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_USER_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetAudioMode_InEeprom: %d",profileIndex);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcChangeNumVoiceTagsForActiveUser_InEeprom                         */
/**
    \brief      modify the number of VTs for active user

    \brief      function shall modify the amount of VTs for active user.
                here increment / decrment value is passed. Function shall check
                that upper limit is not overshoot. For decrement if value
                gets below 0, 0 shall be set

    \param      CB from caller

    \param      Sint8Type value - indicate the amount for change, negative value
                 for decrement - if 0 is given error shall be returned

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcChangeNumVoiceTagsForActiveUser_InEeprom(
          void (*cb)(APP_COMMON_enResponseCode),
          Sint8Type  value)
{
    const ERMUserProfileType * profile  = NULL;
    const ERMPhysicalBTDeviceType * phy = NULL;
    Sint8Type numVoiceTags              = 0;

    /* check if we are busy */
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    /* check inc. / dec. value */
    if( 0 == value )
    {
        return APP_COMMON_RSP_ERROR;
    }

    /* check if we have an active user */
    profile = ERM_pGetActiveUserProfile();
    if( NULL == profile )
    {
        return APP_COMMON_RSP_ERROR;
    }
    /* get current number of voicetags for this user */
    phy = ERM_pGetPhyDevice(profile->deviceIndex);
    if( NULL == phy )
    {
        return APP_COMMON_RSP_ERROR;
    }

    /* check limits */
    numVoiceTags = (Sint8Type)phy->numVoiceTags + value ;
    if(numVoiceTags > ERM_MAX_NUM_VOICE_TAGS_PER_USER ||
       numVoiceTags < 0 )
    {
        return APP_COMMON_RSP_ERROR;
    }

    /* everything is fine, now prepare eeprom access */
    /* do not change bVTDeleteOngoing, use current value */
    if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangePhyTable_Voicetags(profile->deviceIndex,
                                                                     numVoiceTags,
                                                                     phy->bVTDeleteOngoing))
    {
        _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
        return APP_COMMON_RSP_ERROR;
    }

    /* set pending msg and callback and start */
    if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_PHY_TABLE))
    {
        _ERM_gContext.cb_epromWriteFinished        = NULL;
        _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
        return APP_COMMON_RSP_ERROR;
    }

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "hangeNumVoiceTagsForActiveUser_InEeprom: %d",profile->deviceIndex);
    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcResetNumVoiceTagsForActiveUser_InEeprom                          */
/**
    \brief      indicate start / finish of VT reset

    \brief      function indicate start / finish of VT for active user

    \param      CB from caller

    \param      BooleanType isStart - indication if reset is started or finished
                 BT_TRUE indicate reset started
                 BT_FALSE indicate reset finished
                 if reset is finished, for this user also number of VTs shall
                 be set internally to 0x00

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcResetNumVoiceTagsForActiveUser_InEeprom(
      void (*cb)(APP_COMMON_enResponseCode),
      BooleanType isStart)
{
    const ERMUserProfileType * profile  = NULL;
    const ERMPhysicalBTDeviceType * phy = NULL;
    Uint8Type numVoiceTags              = 0;

    /* check if we are busy */
    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    /* check if we have an active user */
    profile = ERM_pGetActiveUserProfile();
    if( NULL == profile )
    {
        return APP_COMMON_RSP_ERROR;
    }
    /* get current number of voicetags for this user */
    phy = ERM_pGetPhyDevice(profile->deviceIndex);
    if( NULL == phy )
    {
        return APP_COMMON_RSP_ERROR;
    }

    /* set number of voicetags */
    /* default do not change current value */
    numVoiceTags = phy->numVoiceTags;
    if( BT_FALSE == isStart )
    {   /* if we are finished, reset to zero */
        numVoiceTags = 0;
    }

    /* everything is fine, now prepare eeprom access */
    /* do not change bVTDeleteOngoing, use current value */
    if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangePhyTable_Voicetags(profile->deviceIndex,
                                                                     numVoiceTags,
                                                                     isStart))
    {
        _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
        return APP_COMMON_RSP_ERROR;
    }

    /* set pending msg and callback and start */
    if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_PHY_TABLE))
    {
        _ERM_gContext.cb_epromWriteFinished        = NULL;
        _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
        return APP_COMMON_RSP_ERROR;
    }

    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "ResetNumVoiceTagsForActiveUser_InEeprom: deviceIndex %d, isStart %d",profile->deviceIndex, isStart);
    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ResetNumVoiceTagsForActiveUser_InEeprom: numVoiceTags %d", numVoiceTags);
    return APP_COMMON_RSP_OK;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_bGetActiveUserAudioMode                                */
/**
    \brief      get current audio mode

    \brief

    \return     ERMUserAudioModeType
   */
/*----------------------------------------------------------------------------*/
ERMUserAudioModeType ERM_enGetActiveUserAudioMode(void)
{
    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
    if(profile!=NULL)
    {
        return profile->audio_mode;
    }
    return ERM_AUDIO_MODE_CALLS_ONLY;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetPlayerAuthMode_InEeprom                                */
/**
    \brief      set authentication mode for player profile

    \brief

    \param      CB from caller

    \param      profileIndex
                Uint8Type
                index of player profile

    \param      bAuthNeeded
                BooleanType

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetPlayerAuthMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                BooleanType bAuthNeeded)
{
    return _ERM_rcSetPlayerHeadsetAuthMode_InEeprom(
                                cb,
                                APP_DEVICE_enTYPE_PLAYER,
                                profileIndex,
                                bAuthNeeded);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSetHeadsetAuthMode_InEeprom                                */
/**
    \brief      set authentication mode for headset profile

    \brief

    \param      CB from caller

    \param      profileIndex
                Uint8Type
                index of player profile

    \param      bAuthNeeded
                BooleanType

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSetHeadsetAuthMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                BooleanType bAuthNeeded)
{
    return _ERM_rcSetPlayerHeadsetAuthMode_InEeprom(
                                cb,
                                APP_DEVICE_enTYPE_HEADSET,
                                profileIndex,
                                bAuthNeeded);
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_rcSetPlayerHeadsetAuthMode_InEeprom                                */
/**
    \brief      set authentication mode

    \brief

    \param      CB from caller

    \param      deviceType
                APP_DEVICE_DeviceType

    \param      profileIndex
                Uint8Type
                index of player / headset profile

    \param      bAuthNeeded
                BooleanType

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcSetPlayerHeadsetAuthMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                APP_DEVICE_DeviceType deviceType,
                                Uint8Type profileIndex,
                                BooleanType bAuthNeeded)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    /* 1. check if we are busy and if not if there is a valid profile  */
    const ERMPlayerHeadsetProfileType * profile = ERM_pGetPlayerHeadsetProfile(profileIndex);

    if( ERM_REQ_NONE != _ERM_gContext.ePendingEpromMsg )
    {
        retVal =  APP_COMMON_RSP_ERROR_BUSY;
    }
    else if( NULL == profile || deviceType != profile->deviceType )
    {   /* no valid profile */
        retVal = APP_COMMON_RSP_ERROR;
    }
    else
    {   /* ERM is NOT busy - try to start */
        /* prepare the eeprom access */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_ChangePlayerHeadsetProfile_AuthMode(profileIndex, bAuthNeeded))
        {
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        /* set pending msg and callback and start */
        if( APP_COMMON_RSP_OK != _ERM_rcPrepare_EepromUpdate(cb, ERM_REQ_CHANGE_PLAYHS_PROFILE))
        {
            _ERM_gContext.cb_epromWriteFinished        = NULL;
            _ERM_vReleaseEepromContext(APP_COMMON_RSP_ERROR);
            return APP_COMMON_RSP_ERROR;
        }

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SetPlayerHeadsetAuthMode_InEeprom: %d",profileIndex);
    }
    return retVal;
}





/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetVoiceOutputBasicVolume                               */
/**
    \brief      Set Voice Output Volume

    \brief

    \param      Uint8Type value - new value to be used

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetMicrophoneSensitivity(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* first check if we have active device or not, if not only update data,
     * otherwise we need to send the message immediately */
    if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
    {   /* only modify the value if in proper range and that's it */
        if (SUCCESS == _ERM_sFSetMicSensitivity(value))
        {   /* we also need to call the CB */
            cb(APP_COMMON_RSP_OK);
        }
        else
        {
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    else
    {   /* we have an active device - send the updated value */
        if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the index */
            if (SUCCESS == _ERM_sFSetMicSensitivity(value))
            {   /* we also need to call the CB */
                retVal = _ERM_SendMessage(ERM_REQ_MIC_GAIN_SET,cb);
            }
            else
            {
                retVal = APP_COMMON_RSP_ERROR;
            }
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcGetCKSWVersionString                                   */
/**
    \brief      Get CK5050 SW version string if available

    \brief

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcGetCKSWVersionString(ERMSWVersionStringType * p_version)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_gContext.bCKSWVersionStringAvail )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* copy string */
        (void)memcpy(p_version,&_ERM_gContext.stSWVersion, sizeof(ERMSWVersionStringType));
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcGetCKPFCodeString                                      */
/**
    \brief      Get PF code available from factory file in flash if available

    \brief      If Information not available in CK5050: UNKNOWN
    \brief      If Information not yet available of CK5050: APP_COMMON_RSP_ERROR_BUSY

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcGetPF_CodeString(ERMPF_CodeStringType * p_pfcode)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_gContext.bPF_CodeStringAvail )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* copy string */
        (void)memcpy(p_pfcode,&_ERM_gContext.stPF_Code, sizeof(ERMPF_CodeStringType));
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcGetBtChipVersion                                       */
/**
    \brief      Get Bt Chip version

    \brief      If Information not available bevor CK5050 statetd: UNKNOWN
    \brief      If Information available of CK5050: Chipversion 2.0 or 2.1

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcGetBtChipVersion(ERMBtChipVersionType * p_btChipVers)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_gContext.stBtChipVersion.bIsValid )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {
        (void)memcpy(p_btChipVers,&_ERM_gContext.stBtChipVersion, sizeof(ERMBtChipVersionType));
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetSspRequest                                       */
/**
    \brief      Get last secure simple pairing request

    \brief      Should only be called after notification, otherwise undefined content

    \return     const ERMSspReqType *
   */
/*----------------------------------------------------------------------------*/
const ERMSspReqType *ERM_pGetSspRequest()
{
    return &(_ERM_gContext.stSspRequest);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetPairRequest                                       */
/**
    \brief      Get last pairing request

    \brief      Should only be called after notification, otherwise undefined content

    \return     const ERMPairReqType *
   */
/*----------------------------------------------------------------------------*/
const ERMPairReqType *ERM_pGetPairRequest()
{
    return &(_ERM_gContext.stPairRequest);
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_sfIsSSPActive                                              */
/**
    \brief      Get the Secure Simple Pairin mode from ERM

    \return     BooleanType
    \retval     BT_TRUE if Secure Simple Pairing is active
    \retval     BT_FALSE if SSP is not active
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsSSPActive(void)
{
    BooleanType retVal = BT_FALSE;
    /* check if SSP is active in ERM's SSP mode */
    if (AT_SSP_ENABLE == _ERM_gContext.eSSPMode )
    {
        retVal = BT_TRUE;
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_rcSendSSPSupportedRequest                                */
/**
    \brief      Send request to inquiry device to check the SSP support.

    \return     APP_COMMON_enResponseCode
    \retval     APP_COMMON_RSP_OK request for getting the SSP support successful send.
    \retval     APP_COMMON_RSP_ERROR if SSP is not supported by device
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_rcSendSSPSupportedRequest(
        AT_DeviceId           deviceId,
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    if (ERM_bIsSSPActive())
    {
        /* check if ERM busy with MSG */
        if ( BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
        {
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {   /* ERM is NOT busy - try to modify the value */
            _ERM_gContext.req.ctx.pairing.deviceId      = deviceId+1;
            retVal = _ERM_SendMessage(ERM_REQ_SSP_SUPPORT,cb);
        }
    }
    return retVal;
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_eTestSetMicAdcMode                                       */
/**
    \brief      Set Mic Mode for audio tests

    \brief

    \param      Uint8Type value - new value to be used either 0 or 1

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eTestSetMicAdcMode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type               value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - check value */
        if (0x00 == value || 0x01 == value)
        {   /* value in range - update internal variable and trigger message */
            _ERM_gContext.u8TestMicMode = value;
            retVal = _ERM_SendMessage(ERM_REQ_TEST_MIC_ADCMODE,cb);
        }
        else
        {

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "TestSetMicAdcMode orr %d",value);
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_eTestSetMicAdcGain                                       */
/**
    \brief      Set Mic Gain for audio tests

    \brief

    \param      Uint8Type value - new value to be used either 0,15 or 30

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eTestSetMicAdcGain(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type               value)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - check value */
        if (0 == value || 15 == value || 30 == value)
        {   /* value in range - update internal variable and trigger message */
            _ERM_gContext.u8TestMicGain = value;
            retVal = _ERM_SendMessage(ERM_REQ_TEST_MIC_ADCGAIN,cb);
        }
        else
        {

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "TestSetMicAdcGain orr %d",value);
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_eTestSetLoopback                                         */
/**
    \brief      Set loopback mode

    \brief

    \param      BooleanType loopback_on BT_TRUE or BT_FALSE to switch on / off

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eTestSetLoopback(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType loopback_on)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - check value */
        if (BT_TRUE == loopback_on || BT_FALSE == loopback_on)
        {   /* value in range - update internal variable and trigger message */
            _ERM_gContext.bTestLoopbackOn = loopback_on;
            retVal = _ERM_SendMessage(ERM_REQ_TEST_LOOPBACK,cb);
        }
        else
        {

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "TestSetLoopback orr %d",loopback_on);
            retVal = APP_COMMON_RSP_ERROR;
        }
    }
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetAudioLoopTest                                        */
/**
    \brief      Trigger audio loop test

    \param      AT_BT_Address bt_addr     - BT address of CK5050 to be connected

    \param      Uint16Type    delay_local - local audio loop in seconds

    \param      Uint16Type    delay_BT    - audio loop by BT in seconds

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetAudioLoopTest(
        void (*cb)(APP_COMMON_enResponseCode),
        const AT_BT_Address bt_addr,
        Uint16Type    delay_local,
        Uint16Type    delay_BT )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {
        (void)memcpy(_ERM_gContext.btAddrAudioLoop,
               bt_addr,sizeof(_ERM_gContext.btAddrAudioLoop));

        _ERM_gContext.u16AudioLoopDelayLocal = delay_local;
        _ERM_gContext.u16AudioLoopDelayBT    = delay_BT;


        retVal = _ERM_SendMessage(ERM_REQ_TEST_AUDIOLOOP,cb);

    }

    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetAudioChannel                                         */
/**
    \brief      Trigger agsw

    \param      ATReqAlternatingSignalType agsw - test info

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetAudioChannel(
        void (*cb)(APP_COMMON_enResponseCode),
        ATReqAlternatingSignalType agsw )
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed() )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {
        _ERM_gContext.alternatingSignal.frequencyLeft     = agsw.frequencyLeft;
        _ERM_gContext.alternatingSignal.frequencyRight    = agsw.frequencyRight;
        _ERM_gContext.alternatingSignal.chanLeftOn        = agsw.chanLeftOn;
        _ERM_gContext.alternatingSignal.chanRightOn       = agsw.chanRightOn;
        _ERM_gContext.alternatingSignal.outputLevelLeft   = agsw.outputLevelLeft;
        _ERM_gContext.alternatingSignal.outputLevelRight  = agsw.outputLevelRight;

        retVal = _ERM_SendMessage(ERM_REQ_ALTERNATING_SIGNAL,cb);

    }

    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_eGetExtRevision                                          */
/**
    \brief      Query the extended SW revision from CK5050

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eGetExtRevision(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()  )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - trigger query for extended revision */
        retVal = _ERM_SendMessage(ERM_REQ_MODEL_REV_EXT,cb);
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetCK5050ToTestMode                                     */
/**
    \brief      Switch CK5050 to test mode

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)
APP_COMMON_enResponseCode ERM_eSetCK5050ToTestMode(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;
    /* check if ERM is active and not busy */
    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed()   )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {   /* ERM is NOT busy - we need to switch off auto detection mode first and
           then check if there are active devices - if so delete them and then
           switch to test mode */
        /* check if we have an active device */
        if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId)
        {   /* no - trigger test mode directly */
            retVal = _ERM_SendMessage(ERM_REQ_TEST_TESTMODE_ON,cb);
        }
        else
        {   /* first switch off auto connection mode and delete paired devices */
            _ERM_gContext.cb_TestModeExt = cb;
            retVal = _ERM_SendMessage(ERM_REQ_TEST_AUTOCONN_OFF,
                    _ERM_vIntAutoconnOffCallback);
            if (retVal != APP_COMMON_RSP_OK &&
                retVal != APP_COMMON_RSP_ERROR_UART_BLOCKED)
            {
                _ERM_gContext.cb_TestModeExt = NULL;
            }
        }
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_eSetCK5050ToTxRxTest                                     */
/**
    \brief      Switch CK5050 to TX/RX tests mode

    \brief

    \param      CB from caller

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode ERM_eSetCK5050ToTxRxTest(
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    if (BT_FALSE == _ERM_bSendExtMsgCheckIsAllowed() )
    {
        retVal = APP_COMMON_RSP_ERROR_BUSY;
    }
    else
    {
        retVal = _ERM_SendMessage(ERM_REQ_TEST_RX_TX,cb);
    }
    return retVal;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vIntAutoconnOffCallback                                 */
/**
    \brief      callback for auto connection mode off

    \brief

    \param      APP_COMMON_enResponseCode

    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _ERM_vIntAutoconnOffCallback(APP_COMMON_enResponseCode rsp)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "IntAutoconnOffCallback rsp=%d",rsp);
    if (APP_COMMON_RSP_OK == rsp)
    {   /* check if we have active devices - if so delete them one by one */
        if (AT_INVALID_DEVICE_ID != _ERM_gContext.u8ActiveBTId)
        {   /* yes - trigger delete of all active devices */
            retVal = _ERM_SendMessage(ERM_REQ_TEST_DELETE_ALL_DEVICES,
                    _ERM_vIntDeleteAllCallback);
        }
        else
        {   /* set to testmode with external callback */
            retVal = _ERM_SendMessage(ERM_REQ_TEST_TESTMODE_ON,
                    _ERM_gContext.cb_TestModeExt);
        }
        if (retVal != APP_COMMON_RSP_OK &&
            retVal != APP_COMMON_RSP_ERROR_UART_BLOCKED)
        {
            _ERM_gContext.cb_TestModeExt(retVal);
            _ERM_gContext.cb_TestModeExt = NULL;
        }
    }
    else
    {   /* call external callback with given response error */
        _ERM_gContext.cb_TestModeExt(rsp);
        _ERM_gContext.cb_TestModeExt = NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vIntDeleteAllCallback                                   */
/**
    \brief      callback for delete all paired devices due to testmode

    \brief

    \param      APP_COMMON_enResponseCode

    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _ERM_vIntDeleteAllCallback(APP_COMMON_enResponseCode rsp)
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "IntDeleteAllCallback rsp=%d",rsp);
    if (APP_COMMON_RSP_OK == rsp)
    {   /* check if we have active devices - if so delete them one by one */
        retVal = _ERM_SendMessage(ERM_REQ_TEST_TESTMODE_ON,
                _ERM_gContext.cb_TestModeExt);
        if (retVal != APP_COMMON_RSP_OK &&
            retVal != APP_COMMON_RSP_ERROR_UART_BLOCKED)
        {
            _ERM_gContext.cb_TestModeExt(retVal);
            _ERM_gContext.cb_TestModeExt = NULL;
        }
    }
    else
    {   /* call external callback with given response error */
        _ERM_gContext.cb_TestModeExt(rsp);
        _ERM_gContext.cb_TestModeExt = NULL;
    }
}
#endif




/*----------------------------------------------------------------------------*/
/* Function    : ERM_bPhoneAvailable                                          */
/**
    \brief      Check if an active BT phone is available

    \brief

    \param      none

    \return     BooleanType BT_TRUE is phone is active, BT_FALSE otherwise
   */
/*----------------------------------------------------------------------------*/
BooleanType     ERM_bPhoneAvailable(void)
{
    BooleanType bRetVal = BT_FALSE;
    if (AT_INVALID_DEVICE_ID != ERM_u8GetActivePhone())
    {
        bRetVal = BT_TRUE;
    }
    return bRetVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_enIsVcrSupported                                          */
/**
    \brief      Check if the VCR should be used

    \brief

    \param      none

    \return     ERMSIVRSupportEnum
   */
/*----------------------------------------------------------------------------*/
ERMSIVRSupportEnum ERM_enIsVcrSupported(void)      /* RT 2429 */
{
    ERMSIVRSupportEnum enRetVal = ERM_SIVR_FULL_SUPPORTED;
    Uint8Type u8VoiceControlCoded;
    BooleanType bVoiceControlSupport;
    Uint8Type u8VoiceControlSupport;
    /* TODO: the check if we are a UHV UltraLow must be added */
    (void)EEPROM_enGetSingleByteFromRamMirror(&u8VoiceControlSupport, EEPROM_LAYOUT_ENUM_VoiceRecognitionSupport);
    if ( u8VoiceControlSupport == 0x1 )
    {
        bVoiceControlSupport = BT_TRUE;
    }
    else if ( u8VoiceControlSupport == 0x0 )
    {
        bVoiceControlSupport = BT_FALSE;
    }
    else
    {
        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "VoiceControlSupport default on %d",__LINE__);
        bVoiceControlSupport = BT_TRUE;
    }


    if(BT_FALSE != bVoiceControlSupport)
    {
        EEPROM_LAYOUT_vGetVoiceControl(&u8VoiceControlCoded);
        if (SUCCESS == SUCCESS)
        {
           switch (u8VoiceControlCoded)
           {
              case 0x00:
                   enRetVal = ERM_SIVR_FULL_SUPPORTED;
              break;

              case 0x01:
              case 0x02:
                   enRetVal = ERM_SIVR_NOT_SUPPORTED;
              break;

//              case 0x02:
//                   enRetVal = ERM_SIVR_LIMITED_SUPPORT;
//              break;

              default:
                   // E2P is wrong init -> take my default
                  enRetVal = ERM_SIVR_FULL_SUPPORTED;
              break;
           }
        }
        else
        {
            //no succes  -> take my default
            enRetVal   = ERM_SIVR_FULL_SUPPORTED; /* full support, is default */

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "VoiceControlSupport default on %d",__LINE__);
        }
    }
    else
    {
        enRetVal = ERM_SIVR_NOT_SUPPORTED;
    }

    return enRetVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetTotalInquiryResult                                  */
/**
    \brief      get number of devices found through last inquiry

    \brief      get number of devices found through last inquiry depending on
                given device type

    \param      deviceType
                APP_DEVICE_DeviceType

    \return     number of devices found
                Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type ERM_u8GetTotalInquiryResult(void)
{
    /* det number of found devices depending on given device type */

    return _ERM_gContext.u8TotalInquiryResult;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetInquiryResult                                       */
/**
    \brief      get number of devices found through last inquiry

    \brief      get number of devices found through last inquiry depending on
                given device type

    \param      deviceType
                APP_DEVICE_DeviceType

    \return     number of devices found
                Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type ERM_u8GetInquiryResult(APP_DEVICE_DeviceType deviceType)
{
    /* det number of found devices depending on given device type */
    switch(deviceType)
    {
        case APP_DEVICE_enTYPE_PHONE:
            return _ERM_gContext.u8PhoneInquiryResult;
        case APP_DEVICE_enTYPE_HEADSET:
            return _ERM_gContext.u8HeadsetInquiryResult;
        case APP_DEVICE_enTYPE_PLAYER:
            return _ERM_gContext.u8PlayerInquiryResult;
        default:
            /* no default */
            break;
    }

    return 0;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetPgirContext                          */
/**
    \brief      returns pointer to current pgir context

    \brief      Description:\n
                returns pointer to current pgir context

    \param      void

    \return     const  *
*/
/*----------------------------------------------------------------------------*/
const ERMInquiryDataType * ERM_pGetPgirContext (Uint16Type index)
{
    if( (_ERM_gContext.req.enLastId == ERM_REQ_INQUIRY_RESULT ) &&
        (index < MAX_REQUESTED_INQUIRY_RESULT_ENTRIES) )
    {
        return &_ERM_gContext.req.ctx.inquiry.result[index];
    }
    else
    {
        return NULL;
    }
}

#if 0
/*----------------------------------------------------------------------------*/
/* Function    : ERM_bGetPlayCallerNameMode                                   */
/**
    \brief      returns result from get request via parrot

    \brief      Description:\n
                returns result from get request via parrot

    \param      void

    \return     mode
                BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bGetPlayCallerNameMode()
{
    if( _ERM_gContext.req.enLastId == ERM_REQ_GET_PLAY_CALLER_NAME )
    {
        return _ERM_gContext.req.ctx.getPlayCallerName.bPlayCallerName;
    }
    return BT_FALSE;
}
#endif


/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetBTPinCodeAsChar                                       */
/**
    \brief      returns result from get request via parrot

    \brief      Description:\n
                returns result from get request via parrot

    \param      void

    \return     mode
                BooleanType
*/
/*----------------------------------------------------------------------------*/
void ERM_GetBTPinCodeAsChar(ATPinCodeType *pinCode)
{
    Uint8Type i     = 0;

    // reset whole buffer
    (void)memset(pinCode, 0, sizeof(pinCode));

    // copy len number of digits
    for (i=0;i<AT_MAX_PIN_CODE_LEN;i++)
    {   /* convert number to ascii character */
        pinCode->pin[i] = (_ERM_gContext.stBTPinCode[i] + 0x30);
    }

    pinCode->pin_len = AT_MAX_PIN_CODE_LEN;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_Manager                                                  */
/**
    \brief      Main ERM manager function

    \brief      function where incoming events are processed and ERM internal\n
                state is handled

    \param      events
                EventMaskType
                all supported events from mask type

    \return     none
   */
/*----------------------------------------------------------------------------*/
void ERM_Manager(EventMaskType events)
{
    // remove event evAppTaskERMATResponse and message msgERMATResponse
    // from APP_PHONE_vHandleEvents to central AT-handling in
    // APP_COMMON_vHandleEvents

    if ( ERM_PENDING_RESET == _ERM_gContext.eResetState )
    {


        DEBUG_VALUE4(ERM_SWI_ERR, MOD_ERM, "TIMEOUT1 not rel %d %d %d %d",ERM_UART_SUPERVISION, _ERM_gContext.eUartOwner, _ERM_gContext.eState, _ERM_gContext.reqRetryCnt);
        DEBUG_VALUE2(ERM_SWI_ERR, MOD_ERM, "TIMEOUT2 not rel %d %d", _ERM_gContext.reqERMRequest, _ERM_gContext.lastERMRequest);
        _ERM_gContext.eResetState = ERM_EXECUTE_RESET;

        /* start Timer to ensure that debug is written before RESET */
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(100), 0);
    }
    else if ( ERM_EXECUTE_RESET == _ERM_gContext.eResetState )
    {
        while ( BT_TRUE )
        {
            /* do nothing to trigger RESET of system */
        }
    }
    else if ( ERM_EXECUTE_RESET_PWNG4 == _ERM_gContext.eResetState )
    {
       /* Parrot: If you receive a PWNG:4 we advise to reset the module (see HSI).*/

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM, "BLUETOOTH CHIP ERROR(PWNG4) -> RESET SYSTEM");


       _ERM_gContext.eResetState = ERM_EXECUTE_RESET;

       /* start Timer to ensure that debug is written before RESET */
       (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(100), 0);
    }
    else if ( ERM_EXECUTE_RESET_VR_WATCHDOG == _ERM_gContext.eResetState )
    {
        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "VR hangs RSTS=%d -> RESET SYSTEM", VCR_eGetVoiceRecognitionStatus());

       _ERM_gContext.eResetState = ERM_EXECUTE_RESET;

       /* start Timer to ensure that debug is written before RESET */
       (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(100), 0);
    }

    /* check if we got an event to process */
    if ( (events & evAppTaskERMBTVisibleTimer) )
    {   /* we need to set BT visibility back to invisible */
        (void)ClearEvent(evAppTaskERMBTVisibleTimer);

#if defined (EMV_TEST_UHV_ALWAYS_BT_VISIBE)
        // no cancelling
        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM, "EMV-TEST SW - STAY VISIBLE");
#else
        _ERM_gContext.eBTvisibilityAct = ERM_BTVIS_OFF;
#endif
    }
    /* check if ERM itself want to send another message */
    if ( APP_SUB_NONE == _ERM_gContext.eUartOwner )
    {
        if ( BT_TRUE == _ERM_gContext.bStartupDone )
        {
            if (ERM_BTVIS_ON == _ERM_gContext.eBTvisibilityAct)
            {   /* independant if BT VIS is on or not, due to
                   BT VIS button the timer shall be restarted */
                if (APP_COMMON_RSP_OK == _ERM_SendMessage(ERM_REQ_BT_VISIBILITY_ON,NULL))
                {   /*  start supervision timer */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_BT_VISIBILITY_TIMER,
                            SEC(ERM_CALLBACK_BT_VISIBILITY_SEC), 0);
                    /* set internal variable back */
                    _ERM_gContext.eBTvisibilityAct = ERM_BTVIS_NONE;

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "Started Bt Visibility");
                }
                /* else retry next time */
            }
            else if (ERM_BTVIS_OFF == _ERM_gContext.eBTvisibilityAct)
            {   /* check if visibility is ON */
                if (BT_TRUE == ERM_bIsBluetoothVisible())
                {
                    if (APP_COMMON_RSP_OK == _ERM_SendMessage(ERM_REQ_BT_VISIBILITY_OFF,NULL))
                    {
                        ERMNotificationType notification;
                        notification.common.id = ERM_NOTIFY_BT_VISIBILITY_OFF;
                        /* set internal variable back */
                        _ERM_gContext.eBTvisibilityAct = ERM_BTVIS_NONE;
                        _ERM_vNotifyApplication(&notification);
                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "Stop BT Visibility");
                    }
                    /* else retry next time */
                }
                /* else only reset */
                else
                {
                    _ERM_gContext.eBTvisibilityAct = ERM_BTVIS_NONE;

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "BT Visibility already OFF");
                }
            }
            else if( _ERM_gContext.bForceDisconnectPlayer == BT_TRUE)
            {
               if (_ERM_gContext.bEnableDisconnectPlayer == BT_TRUE)
               {
                   AT_DeviceId deviceIdPlayer = ERM_u8GetActivePlayer();

                   if( AT_INVALID_DEVICE_ID != deviceIdPlayer && ERM_u8GetActivePhone() == AT_INVALID_DEVICE_ID)
                   {
                      APP_COMMON_enResponseCode rsp = ERM_rcDisconnectDevice(deviceIdPlayer, _ERM_ForceDisconnect_Cb);

                      switch(rsp)
                      {
                          case APP_COMMON_RSP_OK:
                              _ERM_gContext.bForceDisconnectPlayer = BT_FALSE;
                              _ERM_gContext.bEnableDisconnectPlayer = BT_FALSE;
                              break;
                          case APP_COMMON_RSP_ERROR_BUSY:
                              /* else retry next time */
                              break;
                          default:

                              DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND Disconnect Phone failed (%d)", rsp);
                              _ERM_gContext.bForceDisconnectPlayer = BT_FALSE;
                              _ERM_gContext.bEnableDisconnectPlayer = BT_FALSE;
                              break;
                      }
                   }
                   else
                   {
                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, " don't yet disconnect player: status active phone: %d. status active player: %d.",ERM_u8GetActivePlayer(), ERM_u8GetActivePhone());
                   }
               }
               else
               {
                   DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "Wait until _ERM_gContext.bEnableDisconnectPlayer == BT_TRUE"); 
               }
            }
            else if( _ERM_gContext.bForceDisconnect == BT_TRUE)
            {
               AT_DeviceId deviceId = ERM_u8GetActivePhone();
               AT_DeviceId deviceIdPlayer = ERM_u8GetActivePlayer();
               BooleanType bForceDisconnectPlayer = (((deviceId != deviceIdPlayer) && (deviceIdPlayer != AT_INVALID_DEVICE_ID)) ?  BT_TRUE : BT_FALSE);

               _ERM_gContext.bForceDisconnectPlayer = bForceDisconnectPlayer;

               if( AT_INVALID_DEVICE_ID != deviceId )
               {
                  APP_COMMON_enResponseCode rsp = ERM_rcDisconnectDevice(deviceId, NULL);
                  _ERM_gContext.bEnableDisconnectPlayer = BT_FALSE;
                  switch(rsp)
                  {
                      case APP_COMMON_RSP_OK:
                          _ERM_gContext.bForceDisconnect = BT_FALSE;
                          break;
                      case APP_COMMON_RSP_ERROR_BUSY:
                          /* else retry next time */
                          break;
                      default:

                          DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND Disconnect Phone failed (%d)", rsp);
                          _ERM_gContext.bForceDisconnect = BT_FALSE;
                          break;
                  }
               }
               else
               {
                  _ERM_gContext.bEnableDisconnectPlayer = BT_TRUE;
               }
            }
            else if( _ERM_gContext.bSendA2dpAudioVolume == BT_TRUE )
            {
                if (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed() )
                {
                    AT_DeviceId deviceId = ERM_u8GetActivePlayer();
                    const ERMBTDeviceTableType *pBtDevice = ERM_GetBTDevice(deviceId);
                    if(pBtDevice != NULL && pBtDevice->bAudioVolumeSet == AT_FALSE)
                    {
                        /*RT#1441 the AVRCP-Volume is specific to each paired device, therfore we have to send it */
                        APP_COMMON_enResponseCode rsp = _ERM_SendMessage( ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM,
                                                                          _ERM_vOnA2dpVolumeSetCb);
                        switch(rsp)
                        {
                            case APP_COMMON_RSP_OK:
                                _ERM_gContext.bSendA2dpAudioVolume = BT_FALSE;
                                break;
                            case APP_COMMON_RSP_ERROR_BUSY:
                                /* else retry next time */
                                break;
                            default:

                                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND A2dp volume failed 1 (%d)", rsp);
                                _ERM_gContext.bSendA2dpAudioVolume = BT_FALSE;
                                break;
                        }
                    }
                }
                else
                {
                    /* else retry next time */

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SEND A2dp volume blocked 2");
                }
            }
            else if( _ERM_gContext.bSendVcrAudioVolume == BT_TRUE)
            {
                if (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed() )
                {
                    APP_COMMON_enResponseCode rsp = _ERM_SendMessage( ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT,
                                                                      NULL);
                    switch(rsp)
                    {
                        case APP_COMMON_RSP_OK:
                            _ERM_gContext.bSendVcrAudioVolume = BT_FALSE;
                            break;
                        case APP_COMMON_RSP_ERROR_BUSY:
                            /* else retry next time */
                            break;
                        default:

                            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND VCR volume failed (%d)", rsp);
                            _ERM_gContext.bSendVcrAudioVolume = BT_FALSE;
                            break;
                    }
                }
                else
                {
                    /* else retry next time */

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SEND VCR volume blocked");
                }
            }
            else if( _ERM_gContext.bgetImsi == BT_TRUE)
            {
                if (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed() )
                {

                    APP_COMMON_enResponseCode rsp = _ERM_SendMessage( ERM_REQ_GET_IMSI, NULL);

                    switch(rsp)
                    {
                        case APP_COMMON_RSP_OK:
                            _ERM_gContext.bgetImsi = BT_FALSE;
                            break;
                        case APP_COMMON_RSP_ERROR_BUSY:
                            // else retry next time
                            break;
                        default:

                            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND Get IMSI failed (%d)", rsp);
                            _ERM_gContext.bgetImsi = BT_FALSE;
                            break;
                    }
                }
                else
                {
                    /* else retry next time */

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SEND GET IMSI blocked");
                }
            }
            else if( _ERM_gContext.bgetProvider == BT_TRUE)
            {
                if (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed() )
                {

                    APP_COMMON_enResponseCode rsp = APP_PHONE_rcPhoneStatus(NULL);

                    switch(rsp)
                    {
                        case APP_COMMON_RSP_OK:
                            _ERM_gContext.bgetProvider = BT_FALSE;
                            break;
                        case APP_COMMON_RSP_ERROR_BUSY:
                            // else retry next time
                            break;
                        default:

                            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND Get PIND failed (%d)", rsp);
                            _ERM_gContext.bgetProvider = BT_FALSE;
                            break;
                    }
                }
                else
                {
                    /* else retry next time */

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SEND GET IMSI blocked");
                }
            }

            // do other jobs
            (void)_ERM_vJingleStateMachine();
        }
    }
    if ( (events & evAppTaskERMPending) )
    {
        (void)ClearEvent(evAppTaskERMPending);

        if(_ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE)
        {
            if ( APP_COMMON_RSP_OK != _ERM_rcHandlePendingEepromMsg())
            {
                // TODO: error handling,
                //_ERM_vReleaseEepromContext(rsp);
            }
        }

        if(_ERM_gContext.eConsistCheckState != ERM_CC_INVALID)
        {
            (void)_ERM_sfConsistencyCheck();
        }

        /***************************** END *****************************/

        /* trigger retry to send pending message if there is one */
        if (ERM_REQ_NONE != _ERM_gContext.reqERMRequest)
        {
            (void)_ERM_SendMessage(ERM_REQ_RETRY,NULL);
        }

        /* check if we need to switch off BT visibility due to Speed,
           this is only done for initial BT Fisibility and NOT for any
           follow up BT VIS button press!! */
        if (BT_TRUE == _ERM_gContext.bStartupDone &&
            BT_TRUE == _ERM_gContext.bBTVisIsInitial &&
            ERM_BARRING_ON == _ERM_bBTVisBarring &&
            BT_TRUE == ERM_bIsBluetoothVisible())
        {
            if (APP_COMMON_RSP_OK == _ERM_SendMessage(ERM_REQ_BT_VISIBILITY_OFF,NULL))
            {
                ERMNotificationType notification;
                notification.common.id = ERM_NOTIFY_BT_VISIBILITY_OFF;
                /* set internal variable back */
                _ERM_gContext.eBTvisibilityAct = ERM_BTVIS_NONE;
                _ERM_vNotifyApplication(&notification);

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "STOP Initial BT VIS Speed");
            }
        }

    }
    /* check if we have to write the user list to the eprom */
    //if(_ERM_gContext.u8UserListDirty == TRUE)
    //{
    //    _ERM_sfWritePersistentUserList();
    //}

    if (osGetSystemCounter() - _ERM_ManagerTime >= (TickType)1000U)
    {
       ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_CALL_CYCLICAL);

#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE)
       _ERM_vBluetoothTestmodeStateMachine(ERM_BT_TESTMODE_SEND_PBTESTMODE);
#endif

#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)
       _ERM_vBluetoothTestmodeStateMachine(ERM_BT_TESTMODE_SEND_PBFREQEX);
#endif

       _ERM_ManagerTime = osGetSystemCounter();
    }

    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_vHandleATResponse                                   */
/**
    \brief      Handles response messages from AtCmdParser

    \param      p_msg
                ATRspAllType *

    \return     void

*/
/*----------------------------------------------------------------------------*/
void ERM_vHandleATResponse(const ATRspMessageType * p_msg)
{
    if ((ERM_STATE_START_UP != _ERM_gContext.eState) && (AT_RSP_STARTUP == p_msg->common.rsp_type))
    {
        /* here we land e.g. in case of factory reset */
        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Software RESET from CK5050+ %d ms", osGetSystemCounter());
        APP_DEVICE_InitPASAEvent();
        /* RT 14039
         * set not discoverable if reset from parrot was received, without reset
         * of TMS to prevent, that UHV is visible with parrot default BT freindly name.
         */
        _ERM_gContext.u8Active = TRUE;
        _ERM_SendMessage(ERM_REQ_BT_VISIBILITY_OFF, NULL);
    }
    /* special handling for Shutdown as this has to be processed in all
       states */
    switch (_ERM_gContext.eState)
    {
        case ERM_STATE_IDLE:
        {
            /* this is default state after SW reset
               wait for external trigger SW trigger
               ERM keeps Parrot in RESET             */
            /* ERM waits for call of ERM_Startup and do nothing else */

            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "IDLE - discard msg %d state =%d",p_msg->common.rsp_type,_ERM_gContext.eState);
            break;
        }
        case ERM_STATE_START_UP: /* wait for PSTU from BT */
        {
            if (AT_RSP_STARTUP == p_msg->common.rsp_type)
            {
                 if (_ERM_sysPerfStartUpEventTraceDone == 0)
                 {
                     /* Trace only real the first startup */
                     _ERM_sysPerfStartUpEventTraceDone = 1;

                     DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SYS_PERF: Parrot Startup Event: %d ms (AT ready)", osGetSystemCounter());
                 }
                 // RT14039
                 _ERM_vChangeState(ERM_STATE_PSDM);
                 //_ERM_vChangeState(ERM_STATE_WAIT_ADDRESS);
            }
            else
            {

                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "discard msg %d state =%d",p_msg->common.rsp_type,_ERM_gContext.eState);
            }
            break;
        }
        //RT14039
        case ERM_STATE_PSDM:
        {
            ERM_ReleaseUartForErm();
            _ERM_gContext.lastERMRequest = ERM_REQ_NONE;
            _ERM_gContext.u8Active = TRUE;
            _ERM_SendMessage(ERM_REQ_BT_VISIBILITY_OFF, NULL);
            _ERM_vChangeState(ERM_STATE_WAIT_ADDRESS);
        }
        case ERM_STATE_WAIT_ADDRESS:
        {
            if (AT_RSP_SET_BT_ADDR == p_msg->common.rsp_type)
            {
                if ((p_msg->dataInd == AT_RSP_WITH_DATA) && (p_msg != 0))
                {
                    (void)memcpy(_ERM_gContext.stBTAddress,
                            p_msg->data->address.value,
                            sizeof(AT_BT_Address));
                }
                else
                {
                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM, "AT Response BT-Addr without Data");
                    (void)memset(_ERM_gContext.stBTAddress, 0x00, sizeof(AT_BT_Address));
                }

                if (_ERM_sysPerfBTAdddressTraceDone == 0)
                {
                    /* Trace only real the first startup */
                    _ERM_sysPerfBTAdddressTraceDone = 1;

                    DEBUG_VALUE4(ERM_SWI_ERR, MOD_ERM, "SYS_PERF: BT-Address1 %c%c%c%c",_ERM_gContext.stBTAddress[0],_ERM_gContext.stBTAddress[1],_ERM_gContext.stBTAddress[2],_ERM_gContext.stBTAddress[3]);
                    DEBUG_VALUE4(ERM_SWI_ERR, MOD_ERM, "SYS_PERF: BT-Address2 %c%c%c%c",_ERM_gContext.stBTAddress[4],_ERM_gContext.stBTAddress[5],_ERM_gContext.stBTAddress[6],_ERM_gContext.stBTAddress[7]);
                    DEBUG_VALUE4(ERM_SWI_ERR, MOD_ERM, "SYS_PERF: BT-Address3 %c%c%c%c",_ERM_gContext.stBTAddress[8],_ERM_gContext.stBTAddress[9],_ERM_gContext.stBTAddress[10],_ERM_gContext.stBTAddress[11]);
                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "SYS_PERF: BT-Address %d ms", osGetSystemCounter());
                }

                /* now its time to check the SSP configuration */
                ERM_ReleaseUartForErm();
                _ERM_gContext.lastERMRequest = ERM_REQ_NONE;
                _ERM_vChangeState(ERM_STATE_SSP_CONFIG);
                _ERM_enSSPConfigStates = ERM_SSP_CONFIG_START;
                _ERM_gContext.u8Active = TRUE;
                _ERM_SendMessage(ERM_REQ_SSP_SETUP, NULL);
            }
            else
            {

                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "discard msg %d state = %d",p_msg->common.rsp_type,_ERM_gContext.eState);
            }
            /* Note for future improvement -
               now we can increase UART speed                     */
            break;
        }
        case ERM_STATE_SSP_CONFIG:
        {
            /* this case handles the secure simple pairing setup of ck5050. To
             * activate the setup a reset is needed and the state of ERM will
             * set back to startup.
             */
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"SSP Config state active!");
            switch (p_msg->common.rsp_type)
            {
                case AT_RSP_BT_SSP_SETUP:
                {
                    if (AT_RSP_WITH_DATA == p_msg->dataInd &&
                            NULL != p_msg->data   )
                    {
                        if (p_msg->data->pssp.mode != p_msg->data->pssp.nextMode)
                        {
                            /* actual mode and next mode not equal, means reset */
                            /* setup changed with next reboot */
                            DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Reset necessary and usefull! mode: %d, iocap: %d, next mode: %d, next iocap: %d",
                                    p_msg->data->pssp.mode, p_msg->data->pssp.IOCap, p_msg->data->pssp.nextMode, p_msg->data->pssp.nextIOCap);
                            _ERM_enSSPConfigStates = ERM_SSP_CONFIG_RESET;
                        }
                        else
                        {
                            /* if the mode is equal, we have to check if it fits to our
                             * wished setup.
                             */
                            if (BT_TRUE == KPB_bIsAsgAvailable())
                            {
                                _ERM_gContext.eSSPIoCap = AT_PARAM_SSP_IOCAP_DISPLAY_YESNO;
                            }
                            else
                            {
                                _ERM_gContext.eSSPIoCap = AT_PARAM_SSP_IOCAP_NOINNOOUT;
                            }

                            if ((_ERM_gContext.eSSPMode != p_msg->data->pssp.mode)\
                                    || (_ERM_gContext.eSSPIoCap != p_msg->data->pssp.IOCap))
                            {
                                _ERM_enSSPConfigStates = ERM_SSP_CONFIG_CHANGED;
                            }
                            else
                            {
                                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Setup of SSP not needed.");
                                _ERM_enSSPConfigStates = ERM_SSP_CONFIG_DONE;
                            }
                        }
                    }
                    else
                    {
                        _ERM_enSSPConfigStates = ERM_SSP_CONFIG_DONE;
                        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"AT_RSP_BT_SSP_SETUP: no data");
                    }
                    break;
                }
                case AT_RSP_ERROR:
                {
                    _ERM_enSSPConfigStates = ERM_SSP_CONFIG_DONE;
                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"AT_RSP_BT_SSP_SETUP: ERROR");
                    /* -fallthrough */
                }
                case AT_RSP_OK: /* case AT_RSP_ERROR: */
                {
                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Release UART!");
                    ERM_ReleaseUartForErm();
                    _ERM_gContext.lastERMRequest = ERM_REQ_NONE;

                    switch (_ERM_enSSPConfigStates)
                    {
                        case ERM_SSP_CONFIG_CHANGED:
                        {
                            _ERM_SendMessage(ERM_REQ_SSP_CONFIG, NULL);
                            DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "Change SSP to eSSPMode = %d / eSSPIOCap = %d ", _ERM_gContext.eSSPMode,_ERM_gContext.eSSPIoCap);
                            _ERM_enSSPConfigStates = ERM_SSP_CONFIG_ASK;
                            break;
                        }
                        case ERM_SSP_CONFIG_ASK:
                        {
                            _ERM_SendMessage(ERM_REQ_SSP_SETUP, NULL);
                            break;
                        }
                        case ERM_SSP_CONFIG_RESET:
                        {
                            _ERM_vChangeState(ERM_STATE_START_UP);
                            _ERM_SendMessage(ERM_REQ_CK5050_SW_RESET, NULL);
                            break;
                        }
                        case ERM_SSP_CONFIG_DONE:
                        {
                            /* sequence till break copied from ERM_STATE_WAIT_ADDRESS */
                            _ERM_vChangeState(ERM_STATE_CK5050_ACTIVE);
                            _ERM_vPrintTableSize();
                            /* set variables to trigger first startup message */
                            (void)_ERM_SendMessage(ERM_REQ_MODEL_REV,NULL);
                            /* indicate to VCR that CK5050 is active now */
                            VCR_sfErmIndicationCk5050(BT_TRUE);
                            break;
                        }
                        default:
                        {
                            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Handle unknown SSP config state $ERM_enSSPConfigStates$ = %d",_ERM_enSSPConfigStates);
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Handle unknown AT-Response %d",p_msg->common.rsp_type);
                    break;
                }
            }
            break;
        }
        case ERM_STATE_CK5050_ACTIVE:
        {   /* here we set up some specific sub parts for BT comm */
            if (SUCCESS == _ERM_ProcessMsg(p_msg))
            {   /* check if we have active BT device, if so change state */
                //if (SUCCESS == _ERM_CheckActiveBTDevice())
                //{
                //    _ERM_vChangeState(ERM_STATE_CK5050_BT_ACTIVE);
                //}
                /* else - state remains unchanged */
            }
            else
            {
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "Process Err msg %d state = %d",p_msg->common.rsp_type,_ERM_gContext.eState);
            }
            break;
        }
        //case ERM_STATE_CK5050_BT_ACTIVE:
        //{
        //    if (SUCCESS == _ERM_ProcessMsg(&msg))
        //    {   /* check if we have active BT device, if so change state */
        //        //if (FAIL == _ERM_CheckActiveBTDevice())
        //        //{
        //        //    _ERM_vChangeState(ERM_STATE_CK5050_ACTIVE);
        //        //}
        //        /* else - state remains unchanged */
        //    }
        //    else
        //    {

        //    }
        //    break;
        //}
        default:
        {

            DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "UNEXP msg %d state = %d", p_msg->common.rsp_type,_ERM_gContext.eState);
            break;
        }

    }
}


void _ERM_vReleaseEepromContext(APP_COMMON_enResponseCode rsp)
{
    // reset eeprom context and call callback if available
    _ERM_gContext.ePendingEpromMsg      = ERM_REQ_NONE;
    //_ERM_gContext.epr_tmp.deviceId      = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.u8UserIdx     = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.u8PlayHSIdx   = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.u8PhyIdx      = AT_INVALID_DEVICE_ID;

    _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded       = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdateUserRamNeeded          = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded     = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded        = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded        = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded           = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded    = BT_FALSE;

    _ERM_gContext.epr_tmp.bActUserProfileListChanged    = BT_FALSE;
    _ERM_gContext.epr_tmp.bActPlayerProfileListChanged  = BT_FALSE;
    _ERM_gContext.epr_tmp.bActHeadsetProfileListChanged = BT_FALSE;

    // Last but not least call cb of App which intiated the User Profile change
    if( _ERM_gContext.cb_epromWriteFinished )
    {
        _ERM_gContext.cb_epromWriteFinished(rsp);
        _ERM_gContext.cb_epromWriteFinished = NULL;
    }
}

void _ERM_vEepromWriteFinished_Cb(CommonResponseCodeType rsp)
{
    if(rsp != COMMON_RSP_OK)
    {

        DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "EepromWriteFinished_Cb: failed %d", rsp);
    }

    /* eeprom write has been finished, set state */
    if( _ERM_gContext.epr_tmp.state == ERM_EPR_WRITE_STATE_WAITRESULT)
    {
        _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_FINSIHED;
    }
    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_PENDING, MSEC(5), MSEC(0));
}

static APP_COMMON_enResponseCode _ERM_rcHandlePendingEepromMsg(void)
{
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
    // make sure we are pending
    if(_ERM_gContext.ePendingEpromMsg == ERM_REQ_NONE)
    {
        return APP_COMMON_RSP_ERROR;
    }

    /* check what we have to do next */
    if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded )
    {
        switch( _ERM_gContext.epr_tmp.state)
        {
            case ERM_EPR_WRITE_STATE_UNDEFINED:
            case ERM_EPR_WRITE_STATE_WRITE_PENDING:
                rsp = _ERM_rcWritePersistentUserProfile();
                if(APP_COMMON_RSP_OK == rsp )
                {
                    _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_WAITRESULT;
                }
                break;
            case ERM_EPR_WRITE_STATE_WAITRESULT:
                /* do nothing */
                break;
            case ERM_EPR_WRITE_STATE_FINSIHED:
                _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_UNDEFINED;
                _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded = BT_FALSE;
                _ERM_gContext.epr_tmp.bUpdateUserRamNeeded = BT_TRUE;
                break;
            default:

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "HaPendEepMsg: UpUsrEepNe failed %d", _ERM_gContext.epr_tmp.state);
                break;
        }
    }
    else if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded )
    {
        switch( _ERM_gContext.epr_tmp.state)
        {
            case ERM_EPR_WRITE_STATE_UNDEFINED:
            case ERM_EPR_WRITE_STATE_WRITE_PENDING:
                rsp = _ERM_rcWritePersistentPlayerHsProfile();
                if(APP_COMMON_RSP_OK == rsp )
                {
                    _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_WAITRESULT;
                }
                break;
            case ERM_EPR_WRITE_STATE_WAITRESULT:
                /* do nothing */
                break;
            case ERM_EPR_WRITE_STATE_FINSIHED:
                _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_UNDEFINED;
                _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded = BT_FALSE;
                _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded = BT_TRUE;
                break;
            default:

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "HaPendEepMsg: UpPlHSEepNe failed %d", _ERM_gContext.epr_tmp.state);
                break;
        }
    }
    else if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded )
    {
        switch( _ERM_gContext.epr_tmp.state)
        {
            case ERM_EPR_WRITE_STATE_UNDEFINED:
            case ERM_EPR_WRITE_STATE_WRITE_PENDING:
                rsp = _ERM_rcWritePersistentPhysicalDevice();
                if(APP_COMMON_RSP_OK == rsp )
                {
                    _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_WAITRESULT;
                }
                break;
            case ERM_EPR_WRITE_STATE_WAITRESULT:
                /* do nothing */
                break;
            case ERM_EPR_WRITE_STATE_FINSIHED:
                _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_UNDEFINED;
                _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded = BT_FALSE;
                _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded = BT_TRUE;
                break;
            default:

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "HaPendEepMsg: UpPhyEepNe failed %d", _ERM_gContext.epr_tmp.state);
                break;
        }
    }
    else if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded)
    {
        switch( _ERM_gContext.epr_tmp.state)
        {
            case ERM_EPR_WRITE_STATE_UNDEFINED:
            case ERM_EPR_WRITE_STATE_WRITE_PENDING:
                rsp = _ERM_rcWritePersistentActiveProfileList();
                if(APP_COMMON_RSP_OK == rsp )
                {
                    _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_WAITRESULT;
                }
                break;
            case ERM_EPR_WRITE_STATE_WAITRESULT:
                /* do nothing */
                break;
            case ERM_EPR_WRITE_STATE_FINSIHED:
                _ERM_gContext.epr_tmp.state = ERM_EPR_WRITE_STATE_UNDEFINED;
                _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded = BT_FALSE;
                _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded    = BT_TRUE;

                _ERM_gContext.epr_tmp.bActUserProfileListChanged    = BT_FALSE;
                _ERM_gContext.epr_tmp.bActPlayerProfileListChanged  = BT_FALSE;
                _ERM_gContext.epr_tmp.bActHeadsetProfileListChanged = BT_FALSE;
                break;
            default:

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "HaPendEepMsg: UpActProEepNe failed %d", _ERM_gContext.epr_tmp.state);
                break;
        }
    }



    if( BT_FALSE == _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded &&
        BT_FALSE == _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded &&
        BT_FALSE == _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded &&
        BT_FALSE == _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded )
    {
        /* copy updated data to ram */
        if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdateUserRamNeeded )
        {
            _ERM_vCopyTmpUserProfileToRam();
        }
        if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded )
        {
            _ERM_vCopyTmpPlayerHsProfileToRam();
        }
        if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded )
        {
            _ERM_vCopyTmpPhyDeviceToRam();
        }
        if( BT_TRUE == _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded )
        {
            _ERM_vCopyTmpActProfileListToRam();
        }

        /* all eeprom tables are updated */
        // TODO: call internal callback
        _ERM_vReleaseEepromContext(APP_COMMON_RSP_OK);
    }
    else
    {
        /* not yet finished */
        if( _ERM_gContext.epr_tmp.state != ERM_EPR_WRITE_STATE_WAITRESULT )
        {
            // try again later
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_PENDING, MSEC(5), MSEC(0));
        }
    }

    /* this is not an error case, just try again */
    if( rsp == APP_COMMON_RSP_ERROR_BUSY )
    {
        rsp = APP_COMMON_RSP_OK;
    }
    return rsp;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForTel                                        */
/**
    \brief      free UART resource occupied by phone application

    \brief      wrapper for phone which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForTel()
{
    return _ERM_ReleaseUart(APP_SUB_TEL);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForVCR                                        */
/**
    \brief      free UART resource occupied by VCR application

    \brief      wrapper for phone which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForVCR()
{
    return _ERM_ReleaseUart(APP_SUB_VCR);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForMDI                                        */
/**
    \brief      free UART resource occupied by MDI

    \brief      wrapper for MDI which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForMDI()
{
    return _ERM_ReleaseUart(APP_SUB_MDI);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForDevice                                        */
/**
    \brief      free UART resource occupied by Device

    \brief      wrapper for Device which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForDevice()
{
    return _ERM_ReleaseUart(APP_SUB_DEVICE);
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForSMS                                        */
/**
    \brief      free UART resource occupied by SMS

    \brief      wrapper for SMS which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForSMS()
{
    return _ERM_ReleaseUart(APP_SUB_SMS);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForPhonebook                                  */
/**
    \brief      free UART resource occupied by phonebook

    \brief      wrapper for phonebook which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForPhonebook()
{
    return _ERM_ReleaseUart(APP_SUB_PHONEBOOK);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_ReleaseUartForErm                                        */
/**
    \brief      free UART resource occupied by ERM

    \brief      wrapper for ERM which calls _ERM_ReleaseUart

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_ReleaseUartForErm()
{
    return _ERM_ReleaseUart(APP_SUB_ERM);
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForERM                                            */
/**
    \brief      try to reserve the resource UART for ERM

    \brief      wrapper for ERM which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForERM(void)
{
    return _ERM_GetUart(APP_SUB_ERM);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForTel                                            */
/**
    \brief      try to reserve the resource UART for phone application

    \brief      wrapper for phone which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForTel(void)
{
    return _ERM_GetUart(APP_SUB_TEL);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForVCR                                            */
/**
    \brief      try to reserve the resource UART for voice recognition

    \brief      wrapper for phone which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForVCR(void)
{
    return _ERM_GetUart(APP_SUB_VCR);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForMDI                                            */
/**
    \brief      try to reserve the resource UART for MDI

    \brief      wrapper for MDI which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForMDI(void)
{
    return _ERM_GetUart(APP_SUB_MDI);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForSMS                                            */
/**
    \brief      try to reserve the resource UART for SMS

    \brief      wrapper for SMS which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForSMS(void)
{
    return _ERM_GetUart(APP_SUB_SMS);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForPhonebook                                      */
/**
    \brief      try to reserve the resource UART for phonebook

    \brief      wrapper for phonebook which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForPhonebook(void)
{
    return _ERM_GetUart(APP_SUB_PHONEBOOK);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUartForDevice                                      */
/**
    \brief      try to reserve the resource UART for device

    \brief      wrapper for device which calls _ERM_GetUart

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST*  ERM_GetUartForDevice(void)
{
    return _ERM_GetUart(APP_SUB_DEVICE);
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_CheckUartOwnership                                       */
/**
    \brief      check current UART ownership

    \brief      this function check the current UART ownership and if the\n
                given pointer to the UART base address is correct, mean match\n
                the ERM internal pointer. If so the function return SUCCESS\n
                and allow the called the access to the UART respective the\n
                AT_CmdHandler subfunction.
                NOTE: the define ERM_CHECK_UART_OWNERSHIP will be used \n
                to indicate if the check shall be done or if the function\n
                shall return SUCCESS without any check for testing purpose!

    \param      owner
                APPSubModuleIdEnum
                valid member of SubModule enumeration

    \param      ptr
                const SCI_ST*
                const pointer to UART base address for respective UART

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_CheckUartOwnership(APPSubModuleIdEnum owner,const SCI_ST* ptr )
{
#ifdef ERM_CHECK_UART_OWNERSHIP
    SuccessFailType sF = FAIL;
    if (owner == _ERM_gContext.eUartOwner && ptr == _ERM_gContext.pUartPtr)
    {
        sF = SUCCESS;
    }
    return sF;
#else
    return SUCCESS;
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetSWVersion                                             */
/**
    \brief      return the ERM SW version fetched from CK5050

    \brief      this function initialise all ERM global variables back to\n
                the specific default settings.

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
ERMSwVersionType ERM_GetCK5050SWVersion(void)
{
    return _ERM_gContext.eCK5050SWVersion;
}

void _ERM_vResetSSPRequest(ERMSspReqType *pstSSpRequest)
{
    pstSSpRequest->capability = AT_PARAM_SSP_IOCAP_INVALID;
    pstSSpRequest->remote_id = AT_INVALID_DEVICE_ID;
    pstSSpRequest->device_type = AT_INQUIRY_DEVICE;
    (void)memset(&pstSSpRequest->passphrase, '0', sizeof (pstSSpRequest->passphrase));
    pstSSpRequest->mode = AT_SSP_ENABLE;

    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_Init                                                    */
/**
    \brief      initialise ERM internal variables to default

    \brief      this function initialise all ERM global variables back to\n
                the specific default settings.

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_Init(void)
{
    Uint32Type i = 0;
    /* set all internal variables to default */
    Uint8Type u8BTAckSignalActive = 0x0;
    (void)memset(&_ERM_gContext,0x00,sizeof(ERMContextType));
    /* init device and user profile tables */
    _ERM_vInitBTDeviceTable();
    _ERM_vInitPhysicalDeviceTable();
    _ERM_vInitUserProfileTable();
    _ERM_vInitPlayerHeadsetProfileTable();
    _ERM_vInitActiveProfileList();

    _ERM_bBTVisBarring         = ERM_BARRING_OFF;
    _ERM_vChangeState(ERM_STATE_IDLE);

    /* set specific global context values */
    _ERM_gContext.eCK5050SWVersion      = ERM_SW_UNDEF;
    _ERM_gContext.eBTvisibilityAct      = ERM_BTVIS_NONE;
    _ERM_gContext.bBTVisible            = BT_FALSE;
    _ERM_gContext.bBTVisIsInitial       = BT_TRUE;
    _ERM_gContext.u8PlayConnectJingle    = 0;
    _ERM_gContext.u8PlayDisconnectJingle = 0;
    _ERM_gContext.bEnableDisconnectPlayer = BT_FALSE;
    _ERM_gContext.bMuteACK = BT_FALSE;
    _ERM_gContext.eJingleState = ERM_JINGLE_NO_JINGLE_ACTIVE;
    _ERM_gContext.JingleStateTime = 0;

    _ERM_gContext.u8PhoneIdConnected = AT_INVALID_DEVICE_ID;
    _ERM_gContext.u8PlayerIdConnected = AT_INVALID_DEVICE_ID;


    _ERM_gContext.bReInitAudioParmeter = BT_FALSE;
    _ERM_gContext.bSendA2dpAudioVolume  = BT_FALSE;
    _ERM_gContext.bForceDisconnect  = BT_FALSE;
    _ERM_gContext.bForceDisconnectPlayer = BT_FALSE;
    _ERM_gContext.pUartPtr     = NULL;
    _ERM_gContext.eUartOwner   = APP_SUB_NONE;
    _ERM_gContext.bStartupDone = BT_FALSE;
    _ERM_gContext.u8Active     = FALSE;
    _ERM_gContext.u8ActiveBTId = AT_INVALID_DEVICE_ID;
    _ERM_gContext.u8ActiveMDIId     = AT_INVALID_DEVICE_ID;
    _ERM_gContext.u8ActiveHeadsetId = AT_INVALID_DEVICE_ID;
    _ERM_gContext.u8DeviceIdForMsg  = AT_INVALID_DEVICE_ID;
    /* Pick name from EEPROM, in case of EEPROM Read Error, the name will be set to UHV_BT by Diag module*/
    (void)EEPROM_enGetByteSequenceFromRamMirror(&(_ERM_gContext.stBTName[0]), (AT_DEVICE_NAME_MAX_LEN+1), EEPROM_LAYOUT_ENUM_UniversalPreparationForMobileTelephone);
    //memcpy(_ERM_gContext.stBTName, "RS Audio", strlen("RS Audio"));
    /* Pick Pin from EEPROM, in case of EEPROM Read Error, the name will be set to 0000 by Diag Module*/
    (void)EEPROM_enGetBluetoothPin(&_ERM_gContext.stBTPinCode[0], ( AT_MAX_PIN_CODE_LEN + 1));
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

        DEBUG_TEXT(ERM_SWI_WNG, MOD_APP_DEVICE, "ResetToFactorySetting defaultBTAck-True");
    }

    /* secure simple pairing initialization
     * - read from eeprom mode and io capability setup
     */
    _ERM_gContext.eSSPMode = AT_SSP_ENABLE;
    _ERM_gContext.eSSPIoCap = AT_PARAM_SSP_IOCAP_INVALID;

    _ERM_vResetSSPRequest(&_ERM_gContext.stSspRequest);
    _ERM_gContext.bCKSWVersionStringAvail = BT_FALSE;
    _ERM_gContext.bPF_CodeStringAvail = BT_FALSE;

    (void)memset(&(_ERM_gContext.stSWVersion),0x00,sizeof(ERMSWVersionStringType));

    (void)memset(&(_ERM_gContext.stPF_Code),0x00,sizeof(ERMPF_CodeStringType));

    /* test mode settings */
    _ERM_gContext.u8TestMicMode = 1;
    _ERM_gContext.u8TestMicGain = 0;
    _ERM_gContext.bTestLoopbackOn = BT_FALSE;
    _ERM_gContext.eTestFreqMode = AT_TEST_FREQ_TX_CHAN0;
    _ERM_gContext.u16AudioLoopDelayLocal = 0;
    _ERM_gContext.u16AudioLoopDelayBT = 0;
    (void)memset(&_ERM_gContext.btAddrAudioLoop, '0', sizeof (_ERM_gContext.btAddrAudioLoop));
    _ERM_gContext.cb_TestModeExt  = NULL;
    (void)memset(&_ERM_gContext.alternatingSignal, 0, sizeof (ATReqAlternatingSignalType));

    _ERM_gContext.u8TotalInquiryResult = 0;
    _ERM_gContext.u8PhoneInquiryResult = 0;
    _ERM_gContext.u8PlayerInquiryResult = 0;
    _ERM_gContext.u8HeadsetInquiryResult = 0;

    _ERM_gContext.eResetState = ERM_NO_RESET;

    _ERM_gContext.req.enLastId = ERM_REQ_NONE;
    (void)memset(&_ERM_gContext.req.ctx, 0x00, sizeof (_ERM_gContext.req.ctx));
    (void)memset(&_ERM_gContext.stBTAddress[0], '0', sizeof (AT_BT_Address));
#if defined(USE_SIGNED_BT_UPDATE)
    (void)memset(&_ERM_gContext.stMd5Key[0],'0',sizeof(AT_Md5KeyType));
    (void)memset(&_ERM_gContext.stMd5KeyEncryted[0],'0',sizeof(AT_Md5KeyEncrytedType));
#endif
    _ERM_BluetoothUpdateActive = (Uint8Type)BT_FALSE;
    _ERM_ManagerTime = 0;
    _ERM_gContext.bSecurityCheckOK = BT_FALSE;
    _ERM_BluetoothUpdateRequest = AT_BT_SW_UPDATE_DEACTIVATE_SW_CAPABILITY;
    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_NOT_STARTED;

#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE)  || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)
    _ERM_BluetoothTestmodeState = ERM_BT_TESTMODE_OFF;
#endif

    _ERM_vInitAudioValues();
    /* get modifiable audio values from fixed ROM table */


    /* global data for EEPROM handling */
    _ERM_gContext.ePendingEpromMsg      = ERM_REQ_NONE;
    _ERM_gContext.cb_epromWriteFinished = NULL;

    _ERM_vInitTmpEpromData();

    /* */
    if( _ERM_sfReadPersistentActiveProfiles() != SUCCESS )
    {   /* init is already done above, print trace for failure */

        DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM, "Read active profile failure");
    }

    for (i = 0; i < (Uint32Type)ERM_MAX_NUM_USER_PROFILES; i++)
    {
        if( _ERM_sfReadPersistentUserProfile((Uint8Type)i) != SUCCESS )
        {   /* init is already done above, print trace for failure */

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "user prof %d fail",i);
            break;
        }
    }
    for (i = 0; i < (Uint32Type)ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        if( _ERM_sfReadPersistentPlayerHsProfile((Uint8Type)i) != SUCCESS )
        {   /* init is already done above, print trace for failure */

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "hsplay prof %d fail",i);
            break;
        }
    }
    for (i = 0; i < (Uint32Type)ERM_MAX_NUM_PHYSICAL_DEVICES; i++)
    {
        if( _ERM_sfReadPersistentPhysicalDevice((Uint8Type)i) != SUCCESS )
        {   /* init is already done above, print trace for failure */

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM, "phy tab %d fail",i);
            break;
        }
    }

    if( _ERM_sfReadPersistentUserList() != SUCCESS )
    {
        /* that should not happen */
        (void)memset( _ERM_gContext.pu8ActiveUserList,
                AT_INVALID_DEVICE_ID,
                sizeof(_ERM_gContext.pu8ActiveUserList));
    }

    // TODO: remove these lines if the persistent user list is initialized before
    // first time use
    if( _ERM_bUserListWasInitialized() == BT_FALSE)
    {
        (void)memset( _ERM_gContext.pu8ActiveUserList,
                AT_INVALID_DEVICE_ID,
                sizeof(_ERM_gContext.pu8ActiveUserList));
    }

    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "Init done");
}






/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vChangeState                                            */
/**
    \brief      function to change the ERM internal state

    \brief      inside this function state change is indicated to phone\n
                application to trigger update of the device list if necessary

    \param      new_state - new ERM internal state to set
                ERMStateType
                values of enum state type

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
void _ERM_vChangeState(ERMStateType new_state)
{
    static BooleanType btModInitialized = BT_FALSE;

    _ERM_gContext.eState = new_state;

    switch(_ERM_gContext.eState)
    {
        case ERM_STATE_WAIT_ADDRESS:
            if(btModInitialized == BT_FALSE)
            {
                /* we received the first command from the BtModule */
                (void)SetEvent(OsSystemTask_event, evSystemStateBtModInitiated);
                btModInitialized = BT_TRUE;
            }
            break;
        case ERM_STATE_IDLE:
        {
            btModInitialized = BT_FALSE;
            /* trigger idle state towards VCR */
            VCR_sfErmIndicationCk5050(BT_FALSE);
            break;
        }
        default:
            /* do nothing */
            break;
    }


    /* trigger application to take state change into account */
    APP_PHONE_vHandleErmStateChange(_ERM_gContext.eState);
    APP_PHONE_BAP_ERM_vHandleErmStateChange(_ERM_gContext.eState);
    APP_PHONEBOOK_ERM_vHandleErmStateChange(_ERM_gContext.eState);
    APP_MDI_ERM_vHandleErmStateChange(_ERM_gContext.eState);
    APP_SMS_ERM_vHandleErmStateChange(_ERM_gContext.eState);
//RS: disabled, the notification is not needed by APP_SD
//    APP_SD_ERM_vHandleErmStateChange(_ERM_gContext.eState);
    APP_DEVICE_vHandleErmStateChange(_ERM_gContext.eState);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bIsStartupDone                                           */
/**
    \brief      This function returns BT_TRUE in the case that the CK5050 is up
                and the UART is available to the applications, otherwise BT_FALSE

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsStartupDone(void)
{
    return ( _ERM_gContext.bStartupDone      == BT_TRUE &&
             _ERM_gContext.u8Active          == TRUE    &&
             _ERM_gContext.bPrepareCK5050Off != BT_TRUE    ) ? BT_TRUE : BT_FALSE;
}

void _ERM_vOnA2dpVolumeSetCb(APP_COMMON_enResponseCode rsp)
{
    if(rsp == APP_COMMON_RSP_OK)
    {
        AT_DeviceId deviceId = ERM_u8GetActivePlayer();
        if( deviceId <= AT_MAX_DEVICE_ID )
        {
            _ERM_stBTDeviceTable[deviceId].bAudioVolumeSet = AT_TRUE;
        }
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "vOnA2dpVolumeSetCb: failed %d",rsp);
    }
}
void ERM_vOnAppMdiNotification(APP_MDI_enNotificationId event)
{
}

void ERM_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification)
{
    AT_DeviceId deviceId;
    const ERMBTDeviceTableType *pBtDevice;

    switch(notification)
    {
        case APP_DEVICE_NOTIFY_FACTORY_SETTING_RESET:
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "ERM_vOnAppDeviceNotification: Set APP_DEVICE_NOTIFY_FACTORY_SETTING_RESET");
            _ERM_vInitBTDeviceTable();
            _ERM_vInitPhysicalDeviceTable();
            _ERM_vInitUserProfileTable();
            _ERM_vInitPlayerHeadsetProfileTable();
            _ERM_vInitActiveProfileList();
            break;

       case APP_DEVICE_NOTIFY_A2DP_STREAMING_ACTIVE:
            deviceId = ERM_u8GetActivePlayer();
            pBtDevice = ERM_GetBTDevice(deviceId);

            if(pBtDevice != NULL && pBtDevice->bAudioVolumeSet == AT_FALSE)
            {
                /*RT#1441 the AVRCP-Volume is specific to each paired device, therfore we have to send it */
                APP_COMMON_enResponseCode rsp = _ERM_SendMessage( ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM,
                                                                  _ERM_vOnA2dpVolumeSetCb);
                switch(rsp)
                {
                    case APP_COMMON_RSP_OK:
                        /* nothing more to do */
                        break;
                    case APP_COMMON_RSP_ERROR_BUSY:
                        /* set flag to retry */
                        _ERM_gContext.bSendA2dpAudioVolume = BT_TRUE;
                        break;
                    default:
                        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "set ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM failed %d",rsp);
                        break;
                }
            }
        break;

        default:
            /* ignore other notifications */
            break;
    }
}
void ERM_vOnVcrNotification(VCRNotificationIdEnum notification)
{
    switch(notification)
    {
        case VCR_NOTIFY_RSTUEX_RECEIVED:
            _ERM_gContext.bSendVcrAudioVolume  = BT_TRUE;
            /* ERM is called at least every 100 ms, so there is no need for another SetEvent */
            break;
        default:
            /* ignore other notifications */
            break;
    }
}

void ERM_vOnPhonebookNotification(APP_PB_enNotificationId notification)
{
    switch(notification)
    {

        case APP_PB_DOWNLOAD_STATE_CHANGED:
        {
            _ERM_gContext.bgetImsi = BT_TRUE;
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "vOnPhonebookNotification: get Operater delay, notification = %d", notification);
            AppTask_vSetAlarm( APP_TASK_ALARM_ERM_OPERATER_GET_DELAY,
                               MSEC(10000),
                               _ERM_cbGetOperaterDelay);
            break;
        }
        default:
            /* ignore other notifications */
            break;
    }
}

void ERM_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_PHONE_ROAMING_CHANGED:
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "vOnAppPhoneNotification: get Operater delay, notification = %d", notification);
            AppTask_vSetAlarm( APP_TASK_ALARM_ERM_OPERATER_GET_DELAY,
                               MSEC(5000),
                               _ERM_cbGetOperaterDelay);
            break;
        case APP_PHONE_SERV_AVAIL_CHANGED:
            if (BT_TRUE == APP_PHONE_bNetworkAvailable())
            {
                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "vOnPhonebookNotification: get Operater delay, notification = %d", notification);
                AppTask_vSetAlarm( APP_TASK_ALARM_ERM_OPERATER_GET_DELAY,
                                   MSEC(5000),
                                   _ERM_cbGetOperaterDelay);
            }
            break;
        default:
            /* ignore other notifications */
            break;
    }
}
void ERM_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    switch(notification)
    {
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:
                _ERM_gContext.bForceDisconnect = BT_TRUE;
            _ERM_gContext.bPrepareCK5050Off = BT_TRUE;
            break;
#if defined(CRADLE_RESTRICTION_ACTIVE)
        case APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES:
            _ERM_gContext.bForceDisconnect = BT_TRUE;
            _ERM_gContext.bPrepareCK5050Off = BT_TRUE;
            break;
#endif
        case APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN:

            if (_ERM_gContext.u8Active == (Uint8Type)BT_TRUE)
            {
                /* try to send message AT*POFF */
                if (BT_TRUE == CSHDL_bGetBuetoothUpdateActive())
                {
                    // don't send POFF commnad, beacuse Update was tiggered

                }
                else
                {
                   if( APP_COMMON_RSP_ERROR_BUSY == _ERM_SendMessage(ERM_REQ_CK5050_POWER_OFF, _ERM_vOnCk5050PowerOff_Cb))
                   {
                       /* try again later */
                       /* check if there is a callback for reqERMRequest */
                       if( _ERM_gContext.cb_atOkErrCBReq )
                       {   /* will not be sent, cause of shutdown */
                           _ERM_gContext.cb_atOkErrCBReq(APP_COMMON_RSP_ERROR);
                           _ERM_gContext.cb_atOkErrCBReq = NULL;
                       }
                       _ERM_gContext.reqERMRequest     = ERM_REQ_CK5050_POWER_OFF;
                       _ERM_gContext.cb_atOkErrCBReq   = _ERM_vOnCk5050PowerOff_Cb;

                   }
                }
            }
            else
            {

                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "vOnAppCshdlNotification: nothing to do, u8Active %d", _ERM_gContext.u8Active);
                APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_ERM);
                _ERM_gContext.bPrepareCK5050Off = BT_FALSE;
            }
            break;
        case APP_CSHDL_NOTIFY_KL_15_OFF:
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON:
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF:
        default:

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "vOnAppCshdlNotification: ignoring notId %d ", notification);
            break;
    }
}

void _ERM_cbGetOperaterDelay()
{
    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "cbGetOperaterDelay: called" );
    _ERM_gContext.bgetProvider = BT_TRUE;
}


void _ERM_vOnCk5050PowerOff_Cb(APP_COMMON_enResponseCode rspCode)
{
    // POFF was sent
    /* set bStartUpDone and u8Active to FALSE, so no app will be able to request uart */
    //_ERM_gContext.bStartupDone  = BT_FALSE;   // will trigger consistency check
    _ERM_gContext.u8Active          = (Uint8Type)BT_FALSE;
    _ERM_gContext.bPrepareCK5050Off = BT_FALSE;
}


static BooleanType _ERM_bSendExtMsgCheckIsAllowed(void)
{
    BooleanType bRetVal = BT_TRUE;

    if (FALSE        == _ERM_gContext.u8Active ||
        ERM_REQ_NONE != _ERM_gContext.reqERMRequest ||
        ERM_REQ_NONE != _ERM_gContext.lastERMRequest ||
        BT_FALSE     == _ERM_gContext.bStartupDone)
    {
        bRetVal = BT_FALSE;
    }
    return bRetVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_enGetBluetoothUpdateRequest                               */
/**
    \brief      function to get the next mode of the bluetooth update mode

   \param      void
   \return     0 = no BT Update
               1 = BT Update (unsigned PLF)
               2 = secure BT Update (signed PLF)
   */
/*----------------------------------------------------------------------------*/

ATBTSWUpdateRecEnum ERM_enGetBluetoothUpdateRequest (void)
{
    return (_ERM_BluetoothUpdateRequest);
}

Uint8Type ERM_u8GetBluetoothUpdateActive (void)
{
    return (_ERM_BluetoothUpdateActive);
}

#define MAX_RETRIES 10

void ERM_vBluetoothUpdateStateMachine(ERM_enBTUpdateParams Parameter)
{
    static APP_COMMON_enResponseCode lastAppRsp = APP_COMMON_RSP_OK;
    static Uint8Type RetryCounter = 0;
    static Uint8Type OldState = 0;
    static Uint8Type lastErmCmd = 0;


    switch  (_ERM_BluetoothUpdateState)
    {
        case ERM_BT_UPDATE_NOT_STARTED:
            if (ERM_BT_UPDATE_CALL_CYCLICAL == Parameter)
            {
                // Polling of Debugtrigger -> copability with old handling
                if (BT_UPDATE_NORMAL_MODE  == CSHDL_u8GetBluetoothUpdateTrigger())
                {
                     Parameter = ERM_BT_UPDATE_TRIGGER_NORMAL_MODE;
                }
                else if (BT_UPDATE_SECURED_MODE == CSHDL_u8GetBluetoothUpdateTrigger())
                {
                     Parameter = ERM_BT_UPDATE_TRIGGER_SECURED_MODE;
                }
            }
            if ((ERM_BT_UPDATE_TRIGGER_NORMAL_MODE == Parameter) || (ERM_BT_UPDATE_TRIGGER_SECURED_MODE == Parameter))
            {
                if (ERM_BT_UPDATE_TRIGGER_NORMAL_MODE == Parameter)
                {
                    _ERM_BluetoothUpdateRequest = AT_BT_SW_UPDATE_ACTIVATE_SW_CAPABILITY;
                }
                else
                {
                    _ERM_BluetoothUpdateRequest = AT_BT_SW_UPDATE_ACTIVATE_SW_CAPABILITY_IN_SECURED_MODE;
                }


                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "BT Update possible - Waiting 3 min for update file");
                lastAppRsp = ERM_rcBTSWUpdateCK5050(NULL);
                lastErmCmd = 1;

                if (APP_COMMON_RSP_OK == lastAppRsp)
                {
                    // OK

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_NOT_STARTED -> MODE_ACTIVATED");

// Workaround for BT Flashing

//                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_MODE_ACTIVATED;
                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_WORKAROUND_CK5050;
                }
                else
                {
                    RetryCounter = 0;
                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_MODE_RETRY_TRIGGER;

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_NOT_STARTED -> MODE_RETRY_TRIGGER");
                }
            }



            if (ERM_BT_UPDATE_START_TRANSFERING_OF_FILE == Parameter)
            {
                // Free uart
                ERM_Shutdown_BTUpdate();
                // sends file update looks good up to now
                _ERM_BluetoothUpdateStartSendFileTime = osGetSystemCounter();
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_TRANSFERING_FILE;

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_NOT_STARTED -> UPDATE_TRANSFERING_FILE (BT Update active)");
            }
            break;

        case ERM_BT_UPDATE_MODE_RETRY_TRIGGER:
            if (APP_COMMON_RSP_OK == lastAppRsp)
            {
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_MODE_ACTIVATED;
            }
            else if (ERM_BT_UPDATE_CALL_CYCLICAL == Parameter)
            {
                if (++RetryCounter < MAX_RETRIES)
                {
                    if (lastErmCmd == 1)
                    {
                       lastAppRsp = ERM_rcBTSWUpdateCK5050(NULL);
                    }
                    if (lastErmCmd == 2)
                    {
                       lastAppRsp = ERM_rcSetConnectionAuthorization(NULL);
                    }

                }
                else
                {
                    // bad, no ACK  go to init state
                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;
                }
            }
            else if (ERM_BT_UPDATE_CK5050_REBOOT == Parameter)
            {

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "NORMAL_MODE_TRIGGERED -> UPDATE_ERROR (Reboot)");
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;
            }
            break;

        case ERM_BT_UPDATE_WORKAROUND_CK5050:

            lastAppRsp = ERM_rcSetConnectionAuthorization(NULL);
            lastErmCmd = 2;

            if (APP_COMMON_RSP_OK == lastAppRsp)
            {
                // OK

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "WORKAROUND_CK5050 -> MODE_ACTIVATED");
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_MODE_ACTIVATED;
            }
            else
            {
                RetryCounter = 0;
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_MODE_RETRY_TRIGGER;

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_NOT_STARTED -> MODE_RETRY_TRIGGER");
            }

            break;

        case ERM_BT_UPDATE_MODE_ACTIVATED:
            if (ERM_BT_UPDATE_START_TRANSFERING_OF_FILE == Parameter)
            {
                // sends file update looks good up to now
                _ERM_BluetoothUpdateStartSendFileTime = osGetSystemCounter();
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_TRANSFERING_FILE;

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "MODE_ACTIVATED -> UPDATE_TRANSFERING_FILE (BT Update active)");

            }
            else if (ERM_BT_UPDATE_CK5050_REBOOT == Parameter)
            {

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "MODE_ACTIVATED -> UPDATE_ERROR (Reboot)");
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;
            }
            else if (ERM_BT_UPDATE_DEACTIVATE_SW_CAPABILITY == Parameter)
            {

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "BT Update deactivated");
                _ERM_BluetoothUpdateRequest = AT_BT_SW_UPDATE_DEACTIVATE_SW_CAPABILITY;
                (void)ERM_rcBTSWUpdateCK5050(NULL);
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_NOT_STARTED;
            }
            break;

        case ERM_BT_UPDATE_TRANSFERING_FILE:
            if (ERM_BT_UPDATE_SUCCESS_TRANSFERING_OF_FILE == Parameter)
            {
               // transfer finishes looks good up to now
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_TRANSFER_FINISHED;

                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Transfertime %d sec for update file", ((osGetSystemCounter() -_ERM_BluetoothUpdateStartSendFileTime)/(TickType)1000U));


                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_TRANSFERING_FILE -> TRANSFER_FINISHED");

                // don't block uart anymore
                _ERM_gContext.u8Active   = TRUE;

            }

            if (ERM_BT_UPDATE_MD5_KEY_RESULT_OKAY == Parameter)
            {
               // decision if we want to update
                if (ERM_bIsSecurityCheckOK())
                {
                    _ERM_BluetoothUpdateRequest =  AT_BT_SW_UPDATE_CONTINUE_UPDATING_SECURITY_CHECKS_ARE_OK;
                }
                else
                {
                    _ERM_BluetoothUpdateRequest =  AT_BT_SW_UPDATE_ABORT_UPDATING_SECURITY_CHECKS_NOT_OK_REBOOT;
                }


                // don't block uart anymore
                _ERM_gContext.u8Active   = TRUE;

                lastAppRsp = ERM_rcBTSWUpdateCK5050(NULL);

                if (APP_COMMON_RSP_OK == lastAppRsp)
                {
                    // OK

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_TRANSFERING_FILE -> TRANSFER_FINISHED");
                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_TRANSFER_FINISHED;
                }
                else
                {
                    RetryCounter = 0;
                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_DECISION_RETRY_TRIGGER;

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "UPDATE_TRANSFERING_FILE -> DECISION_RETRY_TRIGGER");
                }
            }

            _VCR_vStopSupervision();
            _ERM_vStopUartSupervision();

            if (osGetSystemCounter() - _ERM_BluetoothUpdateStartSendFileTime >= (TickType)BT_UPDATE_TRANSFER_AND_FLASH_TIMEOUT)
            {
               // Timeout 60 min for a update file
               _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;
               DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "TRANSFERING_FILE -> UPDATE_ERROR (Timeout for transfer and flash)");

                // don't block uart anymore
                _ERM_gContext.u8Active   = TRUE;
            }
            break;

        case ERM_BT_UPDATE_DECISION_RETRY_TRIGGER:
            if (++RetryCounter < MAX_RETRIES)
            {

                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Retry:%d", RetryCounter);
                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Retry: ... at Time %d", osGetSystemCounter());
                lastAppRsp = ERM_rcBTSWUpdateCK5050(NULL);
                if (APP_COMMON_RSP_OK == lastAppRsp)
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "DECISION_RETRY_TRIGGER -> TRANSFER_FINISHED");
                    _ERM_BluetoothUpdateState = ERM_BT_UPDATE_TRANSFER_FINISHED;
                }
            }
            else
            {
                // bad, no ACK  go to init state

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "DECISION_RETRY_TRIGGER -> UPDATE_ERROR");
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;
            }


            if (ERM_BT_UPDATE_CK5050_REBOOT == Parameter)
            {

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "NORMAL_MODE_TRIGGERED -> UPDATE_ERROR (Reboot)");
                _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;
            }

            if (osGetSystemCounter() - _ERM_BluetoothUpdateStartSendFileTime >= (TickType)BT_UPDATE_TRANSFER_AND_FLASH_TIMEOUT)
            {
               // Timeout 60 min for a update file
               _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "TRANSFERING_FILE -> UPDATE_ERROR (Timeout for transfer and flash)");
            }
            break;


        case ERM_BT_UPDATE_TRANSFER_FINISHED:
            if (ERM_BT_UPDATE_CK5050_REBOOT == Parameter)
            {
               // transfer was finished looks good up to now  -> go to init state
               _ERM_BluetoothUpdateState = ERM_BT_UPDATE_NOT_STARTED;
               _ERM_BluetoothUpdateActive = (Uint8Type)BT_FALSE;

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "TRANSFER_FINISHED -> UPDATE_FINISHED");
            }

            if (osGetSystemCounter() - _ERM_BluetoothUpdateStartSendFileTime >= (TickType)BT_UPDATE_TRANSFER_AND_FLASH_TIMEOUT)
            {
               // Timeout 60 min for a update file
               _ERM_BluetoothUpdateState = ERM_BT_UPDATE_ERROR;

                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "TRANSFERING_FILE -> UPDATE_ERROR (Timeout for transfer and flash)");
            }
            break;

        case ERM_BT_UPDATE_FINISHED:

            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "END OF BLUETOOTH SW-UPDATE");
            _ERM_BluetoothUpdateActive = (Uint8Type)BT_FALSE;
            _ERM_BluetoothUpdateState = ERM_BT_UPDATE_NOT_STARTED;
            break;




        case ERM_BT_UPDATE_ERROR:  // Fallthrough
        default:
            _ERM_BluetoothUpdateActive = (Uint8Type)BT_FALSE;
            _ERM_BluetoothUpdateState = ERM_BT_UPDATE_NOT_STARTED;
            break;

    }

    if (OldState != (Uint8Type)_ERM_BluetoothUpdateState)
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "BTupState: %d BTupActive %d", _ERM_BluetoothUpdateState, _ERM_BluetoothUpdateActive );
        OldState = (Uint8Type)_ERM_BluetoothUpdateState;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_vBluetoothUpdateProcessCK5050Status                              */
/**
    \brief      handler function that handles the important update Events

   \param      UpdateEvent of PBSU message
   \return     void
   */
/*----------------------------------------------------------------------------*/
void ERM_vBluetoothUpdateProcessCK5050Status(ATBTSWUpdateResEnum UpdateEvent)
{
    switch (UpdateEvent)
    {
        case AT_BT_SW_UPDATE_START_TRANSFER_OF_FILE:
            // User sends a update File, that's our start
            _ERM_BluetoothUpdateActive = (Uint8Type)BT_TRUE;
            ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_START_TRANSFERING_OF_FILE);
            break;

        case AT_BT_SW_UPDATE_SUCCESS_IN_THE_TRANSFER:
            ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_SUCCESS_TRANSFERING_OF_FILE);
            break;

        case AT_BT_SW_UPDATE_MD5_KEY_RESULT_OK:
            ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_MD5_KEY_RESULT_OKAY);
            break;

        case AT_BT_SW_UPDATE_SUCCESS_OF_THE_UPDATE_REBOOT:
        case AT_BT_SW_UPDATE_ERROR_IN_THE_UPDATE_REBOOT:
        case AT_BT_SW_UPDATE_MD5_KEY_RESULT_NOT_OK_REBOOT:
            ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_CK5050_REBOOT);
            break;

        default:
            break;
    }

}




void ERM_vOnErmNotification(const ERMNotificationType *pNotify)
{
    Uint8Type  u8Debug = 0;

    switch(pNotify->common.id)
    {
        case ERM_NOTIFY_BT_VISIBILITY_ON:
#if defined(ERM_USE_SIGNED_BT_UPDATE)
             ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_TRIGGER_SECURED_MODE);
#else
             ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_TRIGGER_NORMAL_MODE);
#endif
             break;

        case ERM_NOTIFY_BT_VISIBILITY_OFF:
             ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_DEACTIVATE_SW_CAPABILITY);
             break;


#ifdef ENABLE_ERM_JINGLE
        case ERM_NOTIFY_PSBD:
            if (pNotify->psbd.status == AT_CONN_SUCC)
             {
    /*             if (BT_TRUE == ERM_bGetBTAckSignalActive())
                 {
                    u8Debug = _ERM_gContext.u8PlayConnectJingle;
                    if (_ERM_gContext.u8PlayConnectJingle <= 1)
                    {
                      _ERM_gContext.u8PlayConnectJingle ++;
                    }
                    DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "BT-DeviceConnect, play Connect-Jingle %d -> %d" ,u8Debug, _ERM_gContext.u8PlayConnectJingle);
                 }
*/
                _ERM_gContext.bEnableDisconnectPlayer = BT_FALSE;
             }
             else if (pNotify->psbd.status == AT_CONN_DISCON_SUCC)
             {

/*                 if (BT_TRUE == ERM_bGetBTAckSignalActive())
                 {
                    u8Debug = _ERM_gContext.u8PlayDisconnectJingle;
                    if (_ERM_gContext.u8PlayDisconnectJingle <= 1)
                    {
                      _ERM_gContext.u8PlayDisconnectJingle ++;
                    }
                    DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "BT-DeviceDisconnect, play Disconnection-Jingle %d -> %d", u8Debug, _ERM_gContext.u8PlayDisconnectJingle);
                }
*/
                 _ERM_gContext.bEnableDisconnectPlayer = BT_TRUE;
             }
             break;

             
             // Don't use PBSN-Notifications for Jingle-playiny requests, use the PSBD-Notification instead


        case ERM_NOTIFY_PBSN_PHONE:
            {
                if (BT_TRUE == ERM_bGetBTAckSignalActive())
                {
                     DEBUG_VALUE4(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "*PBSN PhoneId: %d ServiceNotification: %d - old PhoneId: %d PlayerId: %d",
                                                                                            pNotify->pbsn_phone.device_id,
                                                                                            pNotify->pbsn_phone.result,
                                                                                            _ERM_gContext.u8PhoneIdConnected,
                                                                                            _ERM_gContext.u8PlayerIdConnected);

                     // Jingle is coded
                     if (AT_SERVRES_START_SUCCEED == pNotify->pbsn_phone.result)
                     {
                        // start service succeeded
                       /*  if (AT_INVALID_DEVICE_ID == _ERM_gContext.u8PhoneIdConnected)
                        {
                           // no phone was connected before, now a phone is connected, play connect jingle
                           u8Debug = _ERM_gContext.u8PlayConnectJingle;
                           if (_ERM_gContext.u8PlayConnectJingle <= 1)
                           {
                               _ERM_gContext.u8PlayConnectJingle ++;
                           }
                           DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "PHONE-Connect, play Connect-Jingle %d -> %d for this mobile",u8Debug,_ERM_gContext.u8PlayConnectJingle);

                           // store this Id
                           _ERM_gContext.u8PhoneIdConnected = pNotify->pbsn_phone.device_id;
                        }
                        */
                         
                        u8Debug = _ERM_gContext.u8PlayConnectJingle;
                       
                        if (pNotify->pbsn_phone.device_id == ERM_u8GetActivePlayer())
                        {                            
                            DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "pbsn_phone-connected, set _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_FALSE");
                            _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_FALSE;
                        }
                        else
                        {
                            const ERMUserProfileType * user = ERM_pGetActiveUserProfile();  
                            const ERMPlayerHeadsetProfileType *pAssociatedPlayerProfile = ERM_pGetPlayerHeadsetProfile(user->playerIndex);
                            if (_ERM_gContext.u8PlayConnectJingle <= 1)
                            {
                                _ERM_gContext.u8PlayConnectJingle ++;
                            }
                            
                            if (ERM_u8GetActivePlayer() ==  pAssociatedPlayerProfile->deviceIndex)
                            {
                                DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "pbsn_phone-Connect, set _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_TRUE");
                                _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_TRUE;
                            }
                        }
                        DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "BT-DeviceConnect, play Connect-Jingle %d -> %d" ,u8Debug, _ERM_gContext.u8PlayConnectJingle);
                         
                     }
                     else if (AT_SERVRES_STOP_SUCCEED == pNotify->pbsn_phone.result)
                     {
                      /*   // stop service succeeded
                        if (AT_INVALID_DEVICE_ID != _ERM_gContext.u8PhoneIdConnected)
                        {
                           // a phone was connected before, now a phone diconnecs, play disconnect jingle

                           u8Debug = _ERM_gContext.u8PlayDisconnectJingle;
                           if (_ERM_gContext.u8PlayDisconnectJingle <= 1)
                           {
                               _ERM_gContext.u8PlayDisconnectJingle ++;
                           }
                           DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "PHONE-Disconnect, play Disconnect-Jingle %d -> %d for this mobile",u8Debug,_ERM_gContext.u8PlayDisconnectJingle);

                           // reinit Id
                           _ERM_gContext.u8PhoneIdConnected = AT_INVALID_DEVICE_ID;
                        }
                        */
                         
                        u8Debug = _ERM_gContext.u8PlayDisconnectJingle;
                        if (_ERM_gContext.u8PlayDisconnectJingle <= 1)
                        {
                          _ERM_gContext.u8PlayDisconnectJingle ++;
                        }
                        DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "BT-DeviceDisconnect, play Disconnection-Jingle %d -> %d", u8Debug, _ERM_gContext.u8PlayDisconnectJingle);
                        
                        if (pNotify->pbsn_phone.device_id == ERM_u8GetActivePlayer())
                        {
                            DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "pbsn_phone-disconnect, set _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_FALSE");
                            _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_FALSE;
                        }
                        else
                        {
                            DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "pbsn_phone-disconnect, set _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_TRUE");
                            _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_TRUE;
                        }
                        
                       
                     }
                 }

             }
             break;

        case ERM_NOTIFY_PBSN_A2DP:
             {
                 if (BT_TRUE == ERM_bGetBTAckSignalActive())
                 {
                     DEBUG_VALUE4(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "*PBSN PlayerId: %d ServiceNotification: %d - old PlayerId: %d  PhoneId: %d",
                                                                                            pNotify->pbsn_a2dp.device_id,
                                                                                            pNotify->pbsn_a2dp.result,
                                                                                            _ERM_gContext.u8PlayerIdConnected,
                                                                                            _ERM_gContext.u8PhoneIdConnected);

                     // Jingle is coded
                     if (AT_SERVRES_START_SUCCEED == pNotify->pbsn_a2dp.result)
                     {
                        // start service succeeded
                        // According to Skoda, a Connect Jingle for the Player should be played only if we have another DeviceID
                /*        if (_ERM_gContext.u8PhoneIdConnected == pNotify->pbsn_a2dp.device_id)
                        {

                           // we already played for this mobile a conenct jingle
                            DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "A2DP-Connect, but we already played a connect jingle for this mobile");

                           _ERM_gContext.u8PlayerIdConnected = pNotify->pbsn_a2dp.device_id;

                        }
                        else if (_ERM_gContext.u8PhoneIdConnected != pNotify->pbsn_a2dp.device_id)
                        {
                           // this is another device play connect jingle
                           u8Debug = _ERM_gContext.u8PlayConnectJingle;
                           if (_ERM_gContext.u8PlayConnectJingle <= 1)
                           {
                               _ERM_gContext.u8PlayConnectJingle ++;
                           }
                           DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "A2DP-Connect, play Connect-Jingle %d -> %d for this player",u8Debug,_ERM_gContext.u8PlayConnectJingle);

                           // store this Id
                           _ERM_gContext.u8PlayerIdConnected = pNotify->pbsn_a2dp.device_id;
                        }
                */
                        //const ERMUserProfileType * user             = ERM_pGetActiveUserProfile();                        
                        
                        if (ERM_u8GetActivePhone() != ERM_u8GetActivePlayer())
                        {
                            u8Debug = _ERM_gContext.u8PlayConnectJingle;
                            _ERM_gContext.doPlayDisconnectJingleForPlayer = BT_TRUE;
                            if (_ERM_gContext.u8PlayConnectJingle <= 1)
                            {
                              _ERM_gContext.u8PlayConnectJingle ++;
                            }
                            DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "BT-DeviceConnect, play Connect-Jingle %d -> %d" ,u8Debug, _ERM_gContext.u8PlayConnectJingle);
                        }
                      
                     }
                     else if (AT_SERVRES_STOP_SUCCEED == pNotify->pbsn_a2dp.result)
                     {
                        // stop service succeeded,

                /*        // Fall: Telefon und Player im gleichen Geraet, es soll kein Jingle gespielt werden
                        if (            (_ERM_gContext.u8PhoneIdConnected == pNotify->pbsn_a2dp.device_id)
                                     && (_ERM_gContext.u8PhoneIdConnected != AT_INVALID_DEVICE_ID))
                        {
                           // we already played for this mobile a connect jingle
                            DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "A2DP-Disconnect, but we already played a disconnect jingle for this mobile");

                           _ERM_gContext.u8PlayerIdConnected = pNotify->pbsn_a2dp.device_id;

                        }
                        // Fall:kein Telefon verbunden, Media-Player faellt raus
                        else if ( (_ERM_gContext.u8PhoneIdConnected == pNotify->pbsn_a2dp.device_id)
                                && (_ERM_gContext.u8PhoneIdConnected == AT_INVALID_DEVICE_ID) )
                        {
                           // this is another device play disconnect jingle
                           u8Debug = _ERM_gContext.u8PlayDisconnectJingle;
                           if (_ERM_gContext.u8PlayDisconnectJingle <= 1)
                           {
                               _ERM_gContext.u8PlayDisconnectJingle ++;
                           }
                           DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "A2DP-Disconnect, play Disconnect-Jingle %d -> %d for this player",u8Debug,_ERM_gContext.u8PlayDisconnectJingle);


                           // reinit Id
                           _ERM_gContext.u8PlayerIdConnected = AT_INVALID_DEVICE_ID;
                        }
                  */   
                        //const ERMUserProfileType * user             = ERM_pGetActiveUserProfile();
                        //Uint8Type deviceId = ERM_u8GetUserProfileIndexForDeviceId(pNotify->pbsn_a2dp.device_id);                        
                        
                        DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "lastactivephoneId %d, lastaxctivePlayerId: %d",  ERM_u8GetLastActivePhone(), ERM_u8GetLastActivePlayer());
                        DEBUG_VALUE1(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "_ERM_gContext.doPlayDisconnectJingleForPlayer: %d",_ERM_gContext.doPlayDisconnectJingleForPlayer);

                        if ( (ERM_u8GetLastActivePhone() != ERM_u8GetLastActivePlayer()) && (_ERM_gContext.doPlayDisconnectJingleForPlayer == BT_TRUE))
                        {
                            
                            u8Debug = _ERM_gContext.u8PlayDisconnectJingle;
                            if (_ERM_gContext.u8PlayDisconnectJingle <= 1)
                            {
                              _ERM_gContext.u8PlayDisconnectJingle ++;
                            }
                            DEBUG_VALUE2(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "BT-DeviceDisconnect, play Disconnection-Jingle %d -> %d", u8Debug, _ERM_gContext.u8PlayDisconnectJingle);
                        }
                     }
                     
                     
                 }

             }
             break;
//*/
#endif


        default:
            break;
    }
}


ALARMCALLBACK(ctaERMStopJingle_cb)
{
    (void)SetEvent(OsAppTask_event_100ms, evAppTaskERMPending);
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_vBluetoothUpdateSecurityCheck                            */
/**
    \brief      Check signed Bluetooth firmware according to Spec: UHV_Signed_BT_update_2009_11_4.doc

    \param      none

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/

#define  MD5_HASH_CHECK_SIZE 16

#define ACCEPT_SIGNED_BT_128MBIT  1

void ERM_vBluetoothUpdateSecurityCheck (void)
{
#if defined(ERM_USE_SIGNED_BT_UPDATE)
    Uint8Type i;

    BooleanType bParrotProjektname = BT_FALSE;
    BooleanType bParrotHardware = BT_FALSE;
    BooleanType bCompatibilityAT = BT_FALSE;
    BooleanType bCompatibilitySpeech = BT_FALSE;
    BooleanType bCompatibilityReserved = BT_FALSE;
    BooleanType bCompatibilityMD5_Hash = BT_FALSE;
    Uint8Type   u8CompatibilityMD5_Hash = 0;

    Uint8Type u8VariantType = APP_DTC_u8GetVariantType();

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "VariantType of Eeprom:0x%X", u8VariantType);

    _ERM_gContext.stMd5Key[AT_MD5_KEY_LEN] = '\0';

    //DEBUG_STRING(ERM_SWI_TRC, MOD_ERM, "MD5Key %s", _ERM_gContext.stMd5Key);

    _ERM_gContext.stMd5KeyEncryted[AT_MD5_KEY_ENCRYTED_LEN] = '\0';

    //DEBUG_STRING(ERM_SWI_TRC, MOD_ERM, "Md5KeyEncryted %s", _ERM_gContext.stMd5KeyEncryted);

    // STEP 1 - check Parrot Projektname
    if ((_ERM_gContext.stMd5KeyEncryted[0] == '0') && (_ERM_gContext.stMd5KeyEncryted[1] == '1'))
    {
       // This is VW S1NN
       bParrotProjektname = BT_TRUE;
    }
/*
    else if ((_ERM_gContext.stMd5KeyEncryted[0] == '0') && (_ERM_gContext.stMd5KeyEncryted[1] == '2'))
    {
        // This is AUDI S1NN -> Todo
       bParrotProjektname = BT_TRUE;
    }
*/


    // STEP 2 - check Parrot Hardware
    if ((_ERM_gContext.stMd5KeyEncryted[2] == '0') && (_ERM_gContext.stMd5KeyEncryted[3] == '1'))
    {
       // 01 = 64 MBit
       // if there are new variant Types defined we have add them here !!!
       if (u8VariantType == 0x02)
       {
          bParrotHardware = BT_TRUE;
       }
    }
    else if ((_ERM_gContext.stMd5KeyEncryted[2] == '0') && (_ERM_gContext.stMd5KeyEncryted[3] == '2'))
    {
        // 02 = 128 MBit
        // -> not defined in Eeprom, therefore check negativ
#if (ACCEPT_SIGNED_BT_128MBIT == 1)
       if (u8VariantType == 0x02)
       {
          // this is a 64 Mbit -> check negativ
       }
       else
       {
          // -> not defined in Eeprom, but 128 MBit HW is available at customer
          bParrotHardware = BT_TRUE;
       }
#endif
    }
    else if ((_ERM_gContext.stMd5KeyEncryted[2] == '0') && (_ERM_gContext.stMd5KeyEncryted[3] == '3'))
    {
        // 03 = 256 MBit
        // if there are new variant Types defined we have add them here !!!
        if ((u8VariantType == 0x00) || (u8VariantType == 0x01))
        {
           bParrotHardware = BT_TRUE;
        }
    }
    else if ((_ERM_gContext.stMd5KeyEncryted[2] == '0') && (_ERM_gContext.stMd5KeyEncryted[3] == '4'))
    {
        // 04 = 512 MBit
        // -> not defined in Eeprom, therefore check negativ
    }

    // STEP 3 - check AT-Interface
    if ((_ERM_gContext.stMd5KeyEncryted[4] == '0') && (_ERM_gContext.stMd5KeyEncryted[5] == '0'))
    {
       // 00 = AT-Interface is compatible to SOP1 CW13/2010
       bCompatibilityAT = BT_TRUE;
    }

    // STEP 4 - check Speech Files
    if ((_ERM_gContext.stMd5KeyEncryted[6] == '0') && (_ERM_gContext.stMd5KeyEncryted[7] == '0'))
    {
       // 00 = Parrot Firmware is compatible to Speech Files of SOP1 CW13/2010
       bCompatibilitySpeech = BT_TRUE;
    }

    // STEP 5 - Reserved
    bCompatibilityReserved = BT_TRUE;

    // STEP 6 Compatibility MD5-Hash
    for (i=0;i<MD5_HASH_CHECK_SIZE;i++)
    {
       if ((_ERM_gContext.stMd5KeyEncryted[MD5_HASH_CHECK_SIZE+i] == _ERM_gContext.stMd5Key[i]))
       {
          u8CompatibilityMD5_Hash++;
       }
    }
    if (MD5_HASH_CHECK_SIZE == u8CompatibilityMD5_Hash)
    {
       bCompatibilityMD5_Hash = BT_TRUE;
    }



    DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM, "PN %d PHW %d AT %d SF %d", bParrotProjektname, bParrotHardware, bCompatibilityAT, bCompatibilitySpeech);
    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "R %d MD5 %d", bCompatibilityReserved, bCompatibilityMD5_Hash);
    // ALL CHECKS SUCCESSFUL ?
    if ((bParrotProjektname)&&(bParrotHardware)&&(bCompatibilityAT)&&(bCompatibilitySpeech)&&(bCompatibilityReserved)&&(bCompatibilityMD5_Hash))
    {
        _ERM_gContext.bSecurityCheckOK = BT_TRUE;
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SECURITY CHECKS SUCCESFUL -> CONTINUE UPDATE");
    }
    else
    {
        _ERM_gContext.bSecurityCheckOK = BT_FALSE;

        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "SECURITY CHECKS NOT SUCCESFUL -> ABORT UPDATE");
    }
#else
   // no signed update security check is right
   _ERM_gContext.bSecurityCheckOK = BT_TRUE;
#endif
}


#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)
void _ERM_vBluetoothTestmodeStateMachine(ERM_enBTTestmodeParams Parameter)
{
    static APP_COMMON_enResponseCode lastAppRsp = APP_COMMON_RSP_OK;

    if (_ERM_gContext.eState == ERM_STATE_CK5050_ACTIVE)
    {
        switch (_ERM_BluetoothTestmodeState)
        {
            case ERM_BT_TESTMODE_OFF:
                 if (ERM_BT_TESTMODE_SEND_PBTESTMODE == Parameter)
                 {
                     DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "BT_TESTMODE_OFF -> BT_PBTESTMODE_SENDING");

                     lastAppRsp == ERM_eSetCK5050ToTestMode(NULL);
                     if (APP_COMMON_RSP_OK == lastAppRsp)
                     {
                         _ERM_BluetoothTestmodeState = ERM_BT_PBTESTMODE_SENDING;
                     }
                 }
                 if (ERM_BT_TESTMODE_SEND_PBFREQEX == Parameter)
                 {


                     DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "BT_TESTMODE_OFF -> BT_PBFREQEX_SENDING");
                     lastAppRsp == ERM_eSetCK5050ToTxRxTest(NULL);
                     if (APP_COMMON_RSP_OK == lastAppRsp)
                     {
                         _ERM_BluetoothTestmodeState = ERM_BT_PBFREQEX_SENDING;
                     }
                 }
            break;

            case ERM_BT_PBTESTMODE_SENDING:
                 lastAppRsp == ERM_eSetCK5050ToTestMode(NULL);
                 if (APP_COMMON_RSP_OK == lastAppRsp)
                 {
                     _ERM_BluetoothTestmodeState = ERM_BT_TESTMODE_ACTIVE;
                 }
            break;

            case ERM_BT_PBFREQEX_SENDING:
                 lastAppRsp == ERM_eSetCK5050ToTxRxTest(NULL);
                 if (APP_COMMON_RSP_OK == lastAppRsp)
                 {
                     _ERM_BluetoothTestmodeState = ERM_BT_TESTMODE_ACTIVE;
                 }
            break;

            case ERM_BT_TESTMODE_ACTIVE:
                 // Dummy State
            break;

            default:
            break;
        }
    }
}

void _ERM_vBluetoothTestmodeCallback(APP_COMMON_enResponseCode response)
{
    switch (response)
    {
        case APP_COMMON_RSP_OK:

             DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "BT_TESTMODE_SENDING -> ERM_BT_TESTMODE_ACTIVE");
             _ERM_BluetoothTestmodeState = ERM_BT_TESTMODE_ACTIVE;
        break;

        default:
        break;
    }
}
#endif



void _ERM_vJingleStateMachine(void)
{
    switch (_ERM_gContext.eJingleState)
    {
        case ERM_JINGLE_NO_JINGLE_ACTIVE:
        {
            if( ((_ERM_gContext.u8PlayDisconnectJingle > 0) || (_ERM_gContext.u8PlayConnectJingle > 0)) &&
                  (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed()) )
             {
                if ( (AT_SIVR_ENG_IDLE == VCR_eGetVoiceRecognitionStatus() || AT_SIVR_ENG_END_SYNC == VCR_eGetVoiceRecognitionStatus()) &&
                     (BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()) )
                {
                    if (_ERM_gContext.u8PlayDisconnectJingle > 0)
                    {

                       DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "NO_JINGLE_ACTIVE->WAIT_MUTE_ACK_DISCONNECT_JINGLE");
                       _ERM_gContext.eJingleState = ERM_JINGLE_WAIT_MUTE_ACK_DISCONNECT_JINGLE_CMD;
                    }
                    else if (_ERM_gContext.u8PlayConnectJingle > 0)
                    {

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "JINGLE_NO_JINGLE_ACTIVE->WAIT_MUTE_ACK_CONNECT_JINGLE");
                       _ERM_gContext.eJingleState = ERM_JINGLE_WAIT_MUTE_ACK_CONNECT_JINGLE_CMD;
                    }
                    // beriets hier muten, damit HU Zeit hat zum Umschalten
                    _ERM_gContext.bMuteACK = BT_FALSE;
                    if(APP_DEVICE_AM_sfGetMute(ERM_cbMuteGetAck, APP_CAN_enRing, __LINE__ ) != SUCCESS)
                    {
                        DEBUG_TEXT(ERM_JINGLE_SWI_WNG, MOD_ERM_JINGLE, "Jingle MuteRequest not accepted !");
                        // no further action, because request is cancelled
                    }


                    _ERM_gContext.JingleStateTime = osGetSystemCounter();
                }
                else
                {
                    // requests verwerfen

                    DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "JINGLE REQUEST DROPPED - (ActiveCall or ActiveVR)");
                    _ERM_gContext.u8PlayDisconnectJingle = 0;
                    _ERM_gContext.u8PlayConnectJingle = 0;
                }
             }
        }
        break;

        case ERM_JINGLE_WAIT_MUTE_ACK_DISCONNECT_JINGLE_CMD:
        {
            if (BT_TRUE == _ERM_gContext.bMuteACK)
              {
                  _ERM_gContext.req.ctx.ringtonePlay.action     = AT_PPRT_ACT_PLAY_ONCE;
                  _ERM_gContext.req.ctx.ringtonePlay.ringtoneId = AT_RTMODE_DIS_BLUETOOTH;

                if((_ERM_gContext.u8PlayDisconnectJingle > 0) &&
                   (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed()) &&
                   (AT_SIVR_ENG_IDLE == VCR_eGetVoiceRecognitionStatus() || AT_SIVR_ENG_END_SYNC == VCR_eGetVoiceRecognitionStatus()) &&
                   (BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()))
                  {
                      if (APP_COMMON_RSP_OK == _ERM_SendMessage(ERM_REQ_RINGTONE_PLAY,NULL))
                      {
                          _ERM_gContext.eJingleState = ERM_JINGLE_SEND_DISCONNECT_JINGLE_CMD;
                          _ERM_gContext.JingleStateTime = osGetSystemCounter();


                        DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "WAIT_MUTE_ACK_DISCONNECT_JINGLE->DISCONNECT_JINGLE_CMD");
                      }
                  }
              }
              if ((osGetSystemCounter() - _ERM_gContext.JingleStateTime) > ERM_JINGLE_WAIT_MUTE_ACK_TIMEOUT)
              {
                   /* new Audio Manager*/
                 APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);
                 DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "WAIT_MUTE_ACK_DISCONNECT_JINGLE->NO_JINGLE_ACTIVE (Timeout)");
                 _ERM_gContext.u8PlayConnectJingle = 0;
                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
              }
        }
        break;


        case ERM_JINGLE_WAIT_MUTE_ACK_CONNECT_JINGLE_CMD:
        {
             if (BT_TRUE == _ERM_gContext.bMuteACK)
             {
                _ERM_gContext.req.ctx.ringtonePlay.action     = AT_PPRT_ACT_PLAY_ONCE;
                _ERM_gContext.req.ctx.ringtonePlay.ringtoneId = AT_RTMODE_APP_BLUETOOTH;

                if((_ERM_gContext.u8PlayConnectJingle > 0) &&
                   (BT_TRUE == _ERM_bSendExtMsgCheckIsAllowed()) &&
                   (AT_SIVR_ENG_IDLE == VCR_eGetVoiceRecognitionStatus() || AT_SIVR_ENG_END_SYNC == VCR_eGetVoiceRecognitionStatus()) &&
                   (BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()))
                  {
                      if (APP_COMMON_RSP_OK == _ERM_SendMessage(ERM_REQ_RINGTONE_PLAY,NULL))
                      {
                          _ERM_gContext.eJingleState = ERM_JINGLE_SEND_CONNECT_JINGLE_CMD;
                          _ERM_gContext.JingleStateTime = osGetSystemCounter();


                        DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "WAIT_MUTE_ACK_CONNECT_JINGLE->CONNECT_JINGLE_CMD");
                      }
                  }
              }

              if ((osGetSystemCounter() - _ERM_gContext.JingleStateTime) > ERM_JINGLE_WAIT_MUTE_ACK_TIMEOUT)
              {
                   APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);


                   DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "WAIT_MUTE_ACK_CONNECT_JINGLE->NO_JINGLE_ACTIVE (Timeout)");
                  _ERM_gContext.u8PlayConnectJingle = 0;
                  _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
              }
        }
        break;


        case ERM_JINGLE_SEND_CONNECT_JINGLE_CMD:
        {
             if(AT_AUDIO_STATE_CK5050 == APP_DEVICE_GetPASAEvent())
             {

                 DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "CONNECT_JINGLE_CMD->JINGLE_IS_PLAYING");
                 _ERM_gContext.eJingleState = ERM_JINGLE_CONNECT_JINGLE_IS_PLAYING;
                 _ERM_gContext.JingleStateTime = osGetSystemCounter();
             }
             if ((osGetSystemCounter() - _ERM_gContext.JingleStateTime) > ERM_JINGLE_PLAY_REQUEST_TIMEOUT)
             {
                  APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);


                DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "CONNECT_JINGLE_IS_PLAYING->NO_JINGLE_ACTIVE (Timeout)");
                 _ERM_gContext.u8PlayConnectJingle = 0;
                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
             }
        }
        break;

        case ERM_JINGLE_CONNECT_JINGLE_IS_PLAYING:
        {
             if(AT_AUDIO_STATE_NO_ACTIVE == APP_DEVICE_GetPASAEvent())
             {
                 // ConnectJingle is not playing anymore -> clear Request -> back to Init-State

                 DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "JINGLE_IS_PLAYING->NO_JINGLE_ACTIVE");

                 if (_ERM_gContext.u8PlayConnectJingle >= 1)
                 {
                    _ERM_gContext.u8PlayConnectJingle --;
                 }

                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;

                 APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);
             }
             if ((osGetSystemCounter() - _ERM_gContext.JingleStateTime) > ERM_JINGLE_PLAY_REQUEST_TIMEOUT)
             {
                  APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);

                 // Timeout -> clear both Requests -> back to Init-State

               DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "CONNECT_JINGLE_IS_PLAYING->NO_JINGLE_ACTIVE (Timeout)");
                 _ERM_gContext.u8PlayConnectJingle = 0;
                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
             }
        }
        break;

        case ERM_JINGLE_SEND_DISCONNECT_JINGLE_CMD:
        {
             if(AT_AUDIO_STATE_CK5050 == APP_DEVICE_GetPASAEvent())
             {
                 // Jingle seems to play

                DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "DISCONNECT_JINGLE_CMD->JINGLE_IS_PLAYING");
                 _ERM_gContext.eJingleState = ERM_JINGLE_DISCONNECT_JINGLE_IS_PLAYING;
                 _ERM_gContext.JingleStateTime = osGetSystemCounter();
             }
             if ((osGetSystemCounter() - _ERM_gContext.JingleStateTime) > ERM_JINGLE_PLAY_REQUEST_TIMEOUT)
             {
                 APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);

                 // Timeout -> clear both Requests -> back to Init-State
                 DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "DISCONNECT_JINGLE_CMD->NO_JINGLE_ACTIVE (Timeout)");
                 _ERM_gContext.u8PlayDisconnectJingle = 0;
                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
             }
        }
        break;

        case ERM_JINGLE_DISCONNECT_JINGLE_IS_PLAYING:
        {
             if(AT_AUDIO_STATE_NO_ACTIVE == APP_DEVICE_GetPASAEvent())
             {
                 // DisconnectJingle is not playing anymore -> clear Request -> back to Init-State
                  DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "DISCONNECT_JINGLE_IS_PLAYING->NO_JINGLE_ACTIVE");

                  if (_ERM_gContext.u8PlayDisconnectJingle >= 1)
                  {
                     _ERM_gContext.u8PlayDisconnectJingle --;
                  }

                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
                 APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);
             }
             if ((osGetSystemCounter() - _ERM_gContext.JingleStateTime) > ERM_JINGLE_PLAY_REQUEST_TIMEOUT)
             {
                 APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enRing, __LINE__);

                 // Timeout -> clear both Requests -> back to Init-State

                  DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "DISCONNECT_JINGLE_IS_PLAYING->NO_JINGLE_ACTIVE (Timeout)");
                 _ERM_gContext.u8PlayDisconnectJingle = 0;
                 _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
             }
        }
        break;

        default:
             _ERM_gContext.eJingleState =  ERM_JINGLE_NO_JINGLE_ACTIVE;
        break;
    }
}


ERMJingleStates ERM_GetJingleState (void)
{
    return(_ERM_gContext.eJingleState);
}




void ERM_cbMuteGetAck(APP_COMMON_enResponseCode rsp)
{
   if(rsp == APP_COMMON_RSP_OK)
   {
       _ERM_gContext.bMuteACK = BT_TRUE;
       DEBUG_TEXT(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "ERM_cbMuteGetAck ACK -> set Event");

       (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(0), 0);
   }
   else
   {
       DEBUG_VALUE1(ERM_JINGLE_SWI_TRC, MOD_ERM_JINGLE, "ERM_cbMuteGetAck: NO Mute ACK for ERM - ResponseCode %d", rsp);
   }
}

void ERM_vSetDeviceTypeForPairing(APP_DEVICE_DeviceType deviceType)
{
    _ERM_gContext.deviceTypeForPairing = deviceType;
}

/*----------------------------------------------------------------------------*/
/* Function    : _NetworkOperatorCodeIsVerizonOrAlltelOrSprint                                  */
/**
    \brief      returns true ifprovider is a Verizon,  Alltel or Sprint

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/

BooleanType _NetworkOperatorCodeIsVerizonOrAlltelOrSprint(const Uint8Type *imsi, Uint16Type length)
{
    BooleanType bIsExpectedProviderFound = BT_FALSE;

    if(length >= 6)  // set to 5 for vodafone germany test
    {
        if(
        ((imsi[0] == 0x33) && (imsi[1] == 0x31) && (imsi[2] == 0x30) &&         /*check is sprint: ( MCC:310 MNC:120 ) */
        ( imsi[3] == 0x31) && (imsi[4] == 0x32) && (imsi[5] == 0x30)) ||

        ((imsi[0] == 0x33) && (imsi[1] == 0x31) && (imsi[2] == 0x30) &&          /*check is verizon: ( MCC:310 MNC:012 )  */
        ( imsi[3] == 0x30) && (imsi[4] == 0x31) && (imsi[5] == 0x32)) ||

        ((imsi[0] == 0x33) && (imsi[1] == 0x31) && (imsi[2] == 0x30) &&          /*check is verizon: ( MCC:310 MNC:004 ) */
        ( imsi[3] == 0x30) && (imsi[4] == 0x30) && (imsi[5] == 0x34)) ||

        ((imsi[0] == 0x32) && (imsi[1] == 0x30) && (imsi[2] == 0x30) &&          /*check is alltel: ( MCC:200 MNC:054 ) */
        ( imsi[3] == 0x30) && (imsi[4] == 0x35) && (imsi[5] == 0x34)) ||

        ((imsi[0] == 0x33) && (imsi[1] == 0x31) && (imsi[2] == 0x30) &&          /*check is alltel: ( MCC:310 MNC:500 ) */
        ( imsi[3] == 0x35) && (imsi[4] == 0x30) && (imsi[5] == 0x30))      /* ||

        ((imsi[0] == 0x32) && (imsi[1] == 0x36) && (imsi[2] == 0x32) &&        just for test vodafone germany : ( MCC:262 MNC:02 )
        ( imsi[3] == 0x30) && (imsi[4] == 0x32)) */
        )
        {
            bIsExpectedProviderFound = BT_TRUE;
        }
    }
    else
    {
        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM, "_NetworkOperatorCodeIsVerizonOrAlltelOrSprint: imsi shorter than 6");
    }


    return bIsExpectedProviderFound;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bIsCDMAPhone                                  */
/**
    \brief      returns true if connected phone is a CDMA phone

    \brief      returns true if connected phone is a CDMA phone

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsCDMAPhone(void)
{
    Uint8Type u8CdmaDetection = 0x00;
    /* check coding value */
    EEPROM_LAYOUT_vGetCDMADetection(&u8CdmaDetection);
    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ERM_bIsCDMAPhone: Mode %d", u8CdmaDetection);
    if (0x01 == u8CdmaDetection)
    {
        return _ERM_gContext.bisCDMAPhone;
    }
    else
    {
        return BT_FALSE;
    }
}

void _ERM_ForceDisconnect_Cb(APP_COMMON_enResponseCode result)
{
    if( result != APP_COMMON_RSP_OK)
    {
        AT_DeviceId deviceId = ERM_u8GetActivePlayer();
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "_ERM_ForceDisconnect_Cb result NOK, try again");

       if( AT_INVALID_DEVICE_ID != deviceId )
       {
          //APP_COMMON_enResponseCode rsp = APP_DEVICE_rcDeactivateMdiDevice(NULL);
          APP_COMMON_enResponseCode rsp = ERM_rcDisconnectDevice(deviceId,_ERM_ForceDisconnect_Cb);

          switch(rsp)
          {
              case APP_COMMON_RSP_OK:
                  break;
              case APP_COMMON_RSP_ERROR_BUSY:
                  APP_DEVICE_vSetDeactivatePlayerPendingFlag(deviceId);
                  /* else retry next time */
                  break;
              default:

                  DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "SEND Disconect Player failed (%d)", rsp);
                  break;
          }
       }
    }
    else
    {
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "_ERM_ForceDisconnect_Cb result OK");
    }
}

void ERM_set_AutoConnectionMode(BooleanType bauto)
{
     if(bauto == BT_TRUE)
    {
        (void)_ERM_SendMessage(ERM_REQ_AUTOCONN_ON,NULL);
    }
    else
    {
        (void)_ERM_SendMessage(ERM_REQ_AUTOCONN_OFF,NULL);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_uGetLastExtendedErrorCode                                  */
/**
    \brief      returns the last extended error code received from ckcm

    \return     Uint16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type ERM_uGetLastExtendedErrorCode(void)
{
    return _ERM_gContext.extendedErrorCode;
}
//RT14519
/*----------------------------------------------------------------------------*/
/* Function    : ERM_cbPauseSuhutdownDelaytimeOut()                                 */
/**
    \brief      Device disconnection after pause command during power off

    \return
*/
/*----------------------------------------------------------------------------*/
void ERM_cbPauseSuhutdownDelaytimeOut()
{
	(void)_ERM_SendMessage(ERM_REQ_DISCONNECT_DEVICE,NULL);
}
/* End Of File ERM.c */


