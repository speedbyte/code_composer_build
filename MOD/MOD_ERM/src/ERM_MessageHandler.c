/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_MessageHandler.c
*-----------------------------------------------------------------------------
* Module Name:    External resource manager - message part
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-12-05 15:03:36 +0100 (Mo, 05 Dez 2011) $
* $Rev: 24429 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/src/ERM_MessageHandler.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "osek.h"
#include "osekext.h"
#include "common.h"
#include "UART.h"
#include "ATParser.h"
#include "ATCmdHandler.h"
#include "POOL.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONEBOOK_ERM.h"
#include "APP_MDI_ERM.h"
#include "APP_MDI_BAP.h"
#include "APP_MDI.h"
#include "APP_MDI_CI.h"
#include "APP_COMMON_Utils.h"
#include "OSEK_AppTask_H1.h"
#include "VCR.h"
#include "ATCommonData.h"
#include "APP_SMS_ERM.h"
#include "CSHDL.h"
#include "VCR_MessageHandler.h"
#include "APP_DEVICE.h"
#include "EEPROM_LAYOUT_CFG.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_AudioSets.h"
#include "ERM_Utils.h"
#include "ERM_MessageHandler.h"
#include "ERM_DeviceHandler.h"


#include "ERM_MessageHandler.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern ERMContextType        _ERM_gContext;
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

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

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
/* Function    : _ERM_ProcessMsg                                              */
/**
    \brief      process a message received from AT_Parser

    \brief      this is the main function to process messages received by ERM\n
                from AT Parser resp. CK5050 module.\n
                If OK or ERROR is received the respective application that\n
                owns the UART at the moment is informed about the response\n
                via an Event. The UART ownership is then set back to NONE.\n

    \param      pst_msg - pointer to response message from CK5050
                ATRspMessageType *
                all values allowed

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_ProcessMsg(const ATRspMessageType *pst_msg)
{
    SuccessFailType sF = SUCCESS;
    BooleanType ownerErm = BT_FALSE;

    /* processing of different message types that needs to be handed inside ERM */
    switch (pst_msg->common.rsp_type)
    {
        case AT_RSP_OK:
        /* the resource UART needs to be freed to allow next request */
        {
            APPSubModuleIdEnum lastOwner = _ERM_gContext.eUartOwner;

            DEBUG_VALUE1(ERM_SWI_DBG, MOD_ERM,"AT Response OK owner = %d",_ERM_gContext.eUartOwner);

            /* now free the UART and stop supervision*/
            _ERM_vStopUartSupervision();
            _ERM_gContext.eUartOwner = APP_SUB_NONE;
            switch (lastOwner)
            {   /* trigger respective application module if necessary */
                case APP_SUB_TEL:
                {
                    APP_PHONE_vHandleErmATOkEvent();
                    break;
                }
                case APP_SUB_PHONEBOOK:
                {
                    APP_PHONEBOOK_vHandleErmATOk();
                    break;
                }
                case APP_SUB_MDI:
                {
                    APP_MDI_vHandleErmATResultEvent(APP_MDI_enResultOk);
                    break;
                }
                case APP_SUB_SMS:
                {
                    APP_SMS_vHandleErmATOk();
                    break;
                }
                case APP_SUB_VCR:
                {
                    _VCR_vHandleErmOkErrorEvent(BT_TRUE);
                    break;
                }
                case APP_SUB_DEVICE:
                {
                    APP_DEVICE_vHandleErmATResultEvent(APP_COMMON_RSP_OK,ERM_EXT_ERR_NO_ERROR);
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
            if (BT_TRUE == ownerErm)
            {
                (void)_ERM_sfHandleOkErrorRsp(AT_RSP_OK);
            }
            break;
        }
        case AT_RSP_ERROR:
        /* trigger error event to respective application */
        {
            APPSubModuleIdEnum lastOwner = _ERM_gContext.eUartOwner;

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"AT Response ERROR owner = %d",_ERM_gContext.eUartOwner);

            /* store error Code */
            _ERM_gContext.extendedErrorCode = (ERMExtendedErrorCode)pst_msg->data->peca.errorCode;
            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"AT Response Extended Error Code: %d",ERM_uGetLastExtendedErrorCode());
            
            /* now free the UART and stop supervision*/
            _ERM_vStopUartSupervision();
            _ERM_gContext.eUartOwner = APP_SUB_NONE;
            switch (lastOwner)
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
                case APP_SUB_MDI:
                {
                    APP_MDI_vHandleErmATResultEvent(APP_MDI_enResultError);
                    break;
                }
                case APP_SUB_SMS:
                {
                    APP_SMS_vHandleErmATError();
                    break;
                }
                case APP_SUB_VCR:
                {
                    _VCR_vHandleErmOkErrorEvent(BT_FALSE);
                    break;
                }
                case APP_SUB_DEVICE:
                {
                    APP_DEVICE_vHandleErmATResultEvent(APP_COMMON_RSP_ERROR, _ERM_gContext.extendedErrorCode);
                    break;
                }
                case APP_SUB_ERM:
                {
                	if(_ERM_gContext.lastERMRequest==ERM_REQ_RINGTONE_PLAY && _ERM_gContext.bRingtonestatus ==BT_FALSE )//RT13185
                    {
                        (void)_ERM_RINGTONE_sfHandleOkErrorRsp(AT_RSP_ERROR,_ERM_gContext.extendedErrorCode);
                    }
                    else
                    {
                        ownerErm = BT_TRUE;
                    }
                    break;
                }
                case APP_SUB_NONE:
                default:
                    /* do nothing */
                    break;
            }
            if (BT_TRUE == ownerErm)
            {
                (void)_ERM_sfHandleOkErrorRsp(AT_RSP_ERROR);
            }
            break;
        }
        case AT_RSP_FLASH_ERASE_USERDATA:
        {   /* Flash erase ended - CK5050 does internal reset now -> init ERM */
            ERMNotificationType notification;

            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Flasherase ended - CK5050 does internal reset now -> init ERM");
            (void)_ERM_vNotifyApplication((notification.common.id = ERM_NOTIFY_FLASHERASE_END, &notification));
            break;
        }
        case AT_RSP_FLASH_SW_UPDATE_BT:
        {   /* Update Event */
            switch (pst_msg->data->pbsu.update_event)
            {
                case AT_BT_SW_UPDATE_START_TRANSFER_OF_FILE:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: START of the transfer of the update file");
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_START_TRANSFER_OF_FILE);
                    break;
                }
                case AT_BT_SW_UPDATE_ERROR_IN_THE_UPDATE_REBOOT:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: ERROR in the update (reboot will follow)");
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_ERROR_IN_THE_UPDATE_REBOOT);
                    break;
                }
                case AT_BT_SW_UPDATE_SUCCESS_OF_THE_UPDATE_REBOOT:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: SUCCESS of the update (reboot will follow)");
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_SUCCESS_OF_THE_UPDATE_REBOOT);
                    break;
                }
                case AT_BT_SW_UPDATE_ERROR_IN_THE_TRANSFER_OF_THE_FILE_NO_REBOOT:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: ERROR in the transfer of the update file (no reboot)");
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_ERROR_IN_THE_TRANSFER_OF_THE_FILE_NO_REBOOT);
                    break;
                }
                case AT_BT_SW_UPDATE_SUCCESS_IN_THE_TRANSFER:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: SUCCESS in the transfer of the update file");
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_SUCCESS_IN_THE_TRANSFER);
                    break;
                }
                case AT_BT_SW_UPDATE_FILE_VERSION_INFOS:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: Update file version infos");


                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_FILE_VERSION_INFOS);
                    break;
                }
                case AT_BT_SW_UPDATE_MD5_KEY_RESULT_OK:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: md5 keys result is OK");

#if defined(ERM_USE_SIGNED_BT_UPDATE)
                    (void)memcpy(&_ERM_gContext.stMd5Key[0], pst_msg->data->pbsu.Md5Key, sizeof(_ERM_gContext.stMd5Key));
                    (void)memcpy(&_ERM_gContext.stMd5KeyEncryted[0], pst_msg->data->pbsu.Md5KeyEncryted, sizeof(_ERM_gContext.stMd5KeyEncryted));
#endif
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_MD5_KEY_RESULT_OK);
                    break;
                }
                case AT_BT_SW_UPDATE_MD5_KEY_RESULT_NOT_OK_REBOOT:
                {

                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT UPDATE EVENT: md5 keys result is not OK");
                    ERM_vBluetoothUpdateProcessCK5050Status(AT_BT_SW_UPDATE_MD5_KEY_RESULT_NOT_OK_REBOOT);
                    break;
                }
                default:
                {


                    DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"Unkonwn Bluteooth Update Event");
                    break;
                }
            }
            break;
        }
        case AT_RSP_BT_SET_CONN_AUTH:
        {

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Current Connection Mode %d", pst_msg->data->psca.CurConnectMode);
            break;
        }
        case AT_RSP_BT_SELECT_DEV: /* may be device connected / disconnected */
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                ERMNotificationType notification;

                switch (pst_msg->data->psbd.status)
                {
                    case AT_CONN_SUCC:
                    {   /* we have connected device */
                        (void)_ERM_sFBTDeviceConnectionUpdate(pst_msg->data->psbd.id,AT_TRUE);
                        /* set internal device id for next command */
                        _ERM_gContext.u8DeviceIdForMsg = pst_msg->data->psbd.id;
                        (void)_ERM_SendMessage(ERM_REQ_GET_PAIRED_DEVICES,NULL);
                        break;
                    }
                    case AT_CONN_DISCON_SUCC:
                    {   /* we have disconnect device */
                        (void)_ERM_sFBTDeviceConnectionUpdate(pst_msg->data->psbd.id,AT_FALSE);
                        break;
                    }
                    case AT_CONN_NOT_FOUND:
                    case AT_CONN_FAILED:
                    {   /* we have disconnect device */
                        (void)_ERM_sFBTDeviceConnectionUpdate(pst_msg->data->psbd.id,AT_FALSE);
                        break;
                    }
                    case AT_CONN_DISCON_FAIL:
                    case AT_CONN_AUTO_NOT:
                    default:
                    {   /* no change in status */
                        break;
                    }
                }

                notification.common.id = ERM_NOTIFY_PSBD;
                notification.psbd.device_id = pst_msg->data->psbd.id;
                notification.psbd.status = pst_msg->data->psbd.status;
                _ERM_vNotifyApplication( &notification );

                /* check update of device table */
                _ERM_vCheckDeviceTable();


                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Device id %d -> Connection Status %d", _ERM_gContext.u8ActiveBTId, pst_msg->data->psbd.status);
            }
            else
            {   /* problem - normally data shall be here!!! */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"Got (*PSBD) but no data %d", pst_msg->dataInd);
                sF = FAIL;
            }

            break;
        }
        case AT_RSP_BT_SERVICE_NOT:
        {   /* handle service notification - update device table */
            /* check if this is service activation for phone service, if so trigger
             * setting of communication volume */
            if (AT_SERVICE_PHONE == pst_msg->data->pbsn.service &&
                AT_SERVRES_START_SUCCEED == pst_msg->data->pbsn.result )
            {   /* check that index is unused */
                if (APP_COMMON_RSP_OK ==  _ERM_SendMessage(
                        ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM,NULL))
                {

                   DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"SUCCESS triggering COMM and MIC vol");

                }
                else
                {

                   DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"FAILED triggering COMM and MIC vol");
                }
            }
            if (SUCCESS == _ERM_sFBTDeviceServiceUpdate(&(pst_msg->data->pbsn) ) )
            {
                /* check update of device table */
                _ERM_vCheckDeviceTable();

                /* check if there is a device to release */
                //deviceId = _ERM_u8GetReleasedDevice();

                //if( deviceId != AT_INVALID_DEVICE_ID )
                //{
                //    _ERM_sfDeleteDevice(deviceId);
                //}
            }
			
            // define and send the PBSN notification to the applications
            switch(pst_msg->data->pbsn.service)
            {
                case AT_SERVICE_PHONE:
                {
                    ERMNotificationType notification;
                    notification.common.id = ERM_NOTIFY_PBSN_PHONE;
                    notification.pbsn_phone.device_id =  pst_msg->data->pbsn.device_id;
                    notification.pbsn_phone.result = pst_msg->data->pbsn.result;
                    _ERM_vNotifyApplication(&notification);
                    break;
                }
                case AT_SERVICE_A2DP_SINK:
                {
                    ERMNotificationType notification;
                    notification.common.id = ERM_NOTIFY_PBSN_A2DP;
                    notification.pbsn_a2dp.device_id =  pst_msg->data->pbsn.device_id;
                    notification.pbsn_a2dp.result = pst_msg->data->pbsn.result;
                    _ERM_vNotifyApplication(&notification);
                    break;
                }
                case AT_SERVICE_AVRCP:
                {
                    ERMNotificationType notification;
                    notification.common.id = ERM_NOTIFY_PBSN_AVRCP;
                    notification.pbsn_avrcp.device_id =  pst_msg->data->pbsn.device_id;
                    notification.pbsn_avrcp.result = pst_msg->data->pbsn.result;
                    _ERM_vNotifyApplication(&notification);
                    break;
                }
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM, "PBSN: unknown service %d", pst_msg->data->pbsn.service);
                    break;
            }
            break;
        }
        case AT_RSP_GET_PAIRED_DEVICES_IN:
        {   /* handle plpd - update device table */
            if (SUCCESS == _ERM_sFBTFillDeviceInfo(&(pst_msg->data->plpd_info)) )
            {
                /* if the LastAtRequest is set we are waiting for all PLPDs
                 * before acting*/
                //if(_ERM_gContext.lastAtRequest != AT_REQ_GET_PAIRED_DEVICES)
                //{
                //    /* check update of device table */
                //    //_ERM_vCheckDeviceTable();
                //    /* trigger service information */
                //    //_ERM_gContext.u8DeviceIdForMsg = pst_msg->data->plpd_info.device_id;
                //    //_ERM_SendMessage(ERM_REQ_BT_SERVICE_CONN,NULL);
                //}
            }
            break;
        }
        case AT_RSP_PAIR_DELETE:
        {   /* handle ppde */
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                AT_DeviceId deviceId = pst_msg->data->ppde.device_id;
                if( deviceId <= AT_MAX_DEVICE_ID)
                {
                    // delete info about this device
                    (void)_ERM_sFInitBTDeviceValues(deviceId);

                    if( _ERM_gContext.req.enLastId == ERM_REQ_DELETE_DEVICE_ID )
                    {
                        ERMNotificationType notification;
                        // delete was triggered by app
                        notification.common.id = ERM_NOTIFY_DELETED_DEVICE;
                        _ERM_vNotifyApplication(&notification);
                    }
                    else
                    {
                        /* set back the global device id */
                        _ERM_gContext.u8DeleteDeviceId = AT_INVALID_DEVICE_ID;

                        if(_ERM_gContext.eConsistCheckState == ERM_CC_WAIT_FOR_RESPONSE)
                        {
                            /* go on with consistency check */
                            _ERM_OnConsistencyCheckCb(APP_COMMON_RSP_OK);
                        }
                    }
                }
            }
            break;
        }
        case AT_RSP_MODEL_REV:
        {   /* feedback for model revision - store and trace out */
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                NULL != pst_msg->data                    )
            {   /* check len of given SW version in allowed range */

               if (  ( (AT_BT_PF_CODE_MAX_LEN >= pst_msg->data->cgmr.version_len) &&
                      ((pst_msg->data->cgmr.version[0] == 'U') && (pst_msg->data->cgmr.version[1] == 'N')) ||
                      ((pst_msg->data->cgmr.version[0] == 'P') && (pst_msg->data->cgmr.version[1] == 'F'))
                      )
                   )

                {
                     (void)memset(&(_ERM_gContext.stPF_Code.pfcode),0x00, sizeof(ERMPF_CodeStringType));

                     if (AT_BT_PF_CODE_MAX_LEN < pst_msg->data->cgmr.version_len)
                     {
                         _ERM_gContext.stPF_Code.pfcode_len = AT_BT_PF_CODE_MAX_LEN;
                     }
                     else
                     {
                         _ERM_gContext.stPF_Code.pfcode_len = pst_msg->data->cgmr.version_len;
                     }

                     (void)memcpy(&(_ERM_gContext.stPF_Code.pfcode),&(pst_msg->data->cgmr.version),_ERM_gContext.stPF_Code.pfcode_len);

                     /* ensure string termination */
                     _ERM_gContext.stPF_Code.pfcode[_ERM_gContext.stPF_Code.pfcode_len] = '\0';
                     /* now print a trace */

                     //ToDo neues Tarce Konzept nur die wichtigen Bytes des PF-Code Strings  trace

                     _ERM_gContext.bPF_CodeStringAvail = BT_TRUE;


                }
                else if (AT_BT_SW_VERSION_MAX_LEN >= pst_msg->data->cgmr.version_len)
                {   /* first clear internal var, then copy values */
                    (void)memset(&(_ERM_gContext.stSWVersion.version),0x00, sizeof(ERMSWVersionStringType));

                    if (AT_BT_SW_VERSION_MAX_LEN == pst_msg->data->cgmr.version_len)
                    {   /* reduce for the blank */
                        _ERM_gContext.stSWVersion.version_len = AT_BT_SW_VERSION_MAX_LEN - 1;
                    }
                    else
                    {
                        _ERM_gContext.stSWVersion.version_len = pst_msg->data->cgmr.version_len;
                    }
                    (void)memcpy(&(_ERM_gContext.stSWVersion.version),&(pst_msg->data->cgmr.version),_ERM_gContext.stSWVersion.version_len);
                    /* add a blank */
                    _ERM_gContext.stSWVersion.version[_ERM_gContext.stSWVersion.version_len] = 0x20;
                    _ERM_gContext.stSWVersion.version_len = _ERM_gContext.stSWVersion.version_len + 1;
                    /* ensure string termination */
                    _ERM_gContext.stSWVersion.version[_ERM_gContext.stSWVersion.version_len] = '\0';
                    /* now print a trace */


                    DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"CK5050+ CGMR Version = H%c%c%c%c",
                                 _ERM_gContext.stSWVersion.version[1], _ERM_gContext.stSWVersion.version[2],
                                 _ERM_gContext.stSWVersion.version[3], _ERM_gContext.stSWVersion.version[4]);

                    DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"CK5050+ CGMR Version = S%c%c%c%c",
                                 _ERM_gContext.stSWVersion.version[7], _ERM_gContext.stSWVersion.version[8],
                                 _ERM_gContext.stSWVersion.version[9], _ERM_gContext.stSWVersion.version[10]);
                    //ToDo neues Tarce Konzept nur die wichtigen Bytes der SW Version tracen -> auch per Diagnose auslebar

                }
                else
                {

                   DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"received AT_RSP_MODEL_REV inc len %d", pst_msg->data->cgmr.version_len);
                }
            }
            else
            {   /* problem - normally data shall be here!!! */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"received AT_RSP_MODEL_REV without data %d", pst_msg->dataInd);
                sF = FAIL;
            }
            break;
        }
        case AT_RSP_MODEL_REV_EXT:
        {   /* feedback for extended model revision - trace out only*/
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                NULL != pst_msg->data                    )
            {   /* check len of given SW version in allowed range */
                char * p_char = (char *)pst_msg->data->cgmrex.extRev;
                char * p_len  = NULL;
                Uint16Type len = 0;
                if (AT_CMGREX_MAX_LEN >= pst_msg->data->cgmrex.extRev_len)
                {   /* now print a trace */

                    DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Parrot SW CGMREX:  %c.%c%c.%c",
                                 pst_msg->data->cgmrex.extRev[1], pst_msg->data->cgmrex.extRev[3],
                                 pst_msg->data->cgmrex.extRev[4], pst_msg->data->cgmrex.extRev[6]);
                }
                else
                {

                   DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"received AT_RSP_MODEL_REV_EXT inc len %d", pst_msg->data->cgmrex.extRev_len);
                }
                _ERM_gContext.eCK5050SWVersion = ERM_SW_NEWER;
                /* to to concatenate SW minor info to ERM SW version */
                p_char++; /* ignore leading ' */
                p_len = strchr(p_char,AT_ASCII_QUOTE);
                if (NULL != p_len)
                {   /* found end, check len and copy */
                    len = p_len-p_char;
                    if ( (len + _ERM_gContext.stSWVersion.version_len) > ERM_CK5050SW_VERSION_MAX_LEN )
                    {
                        len = ERM_CK5050SW_VERSION_MAX_LEN - _ERM_gContext.stSWVersion.version_len;
                    }
                    (void)memcpy(&(_ERM_gContext.stSWVersion.version[_ERM_gContext.stSWVersion.version_len]),p_char,len);
                    _ERM_gContext.stSWVersion.version_len = _ERM_gContext.stSWVersion.version_len + len;
                    _ERM_gContext.stSWVersion.version[_ERM_gContext.stSWVersion.version_len] = '\0';
                    /* ticket 12157: by MM - extract software description from */
                    p_char = p_char + (len +2); // put pointer to first char behind ','
                    _ERM_gContext.stSWVersion.u16Description = atoi(p_char);
                    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "Parrot software description = %d ", _ERM_gContext.stSWVersion.u16Description);
                }
            }
            else
            {   /* problem - normally data shall be here!!! */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"received AT_RSP_MODEL_REV_EXT without data %d", pst_msg->dataInd);
                sF = FAIL;
            }
            /* set CKSW string available to true */
            _ERM_gContext.bCKSWVersionStringAvail = BT_TRUE;
            break;
        }
        case AT_RSP_PAIR_REQUEST_RESULT:
        {   /* handle pprs */

            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                ERMNotificationType notification;

                notification.common.id = ERM_NOTIFY_PPRS;
                notification.pprs.device_id = AT_INVALID_DEVICE_ID;
                notification.pprs.result = pst_msg->data->pprs.result;

                switch( pst_msg->data->pprs.result)
                {
                    case AT_PAIRING_SUCCEEDED:
                        /* save position of new device */
                        _ERM_gContext.u8LastPairedDeviceId = pst_msg->data->pprs.device_id;
                        notification.pprs.device_id = pst_msg->data->pprs.device_id;
                        
                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"pairng successful: ERM_u8GetActivePhone() %d", ERM_u8GetActivePhone());
                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"pairng successful: pst_msg->data->pprs.device_id %d", pst_msg->data->pprs.device_id);
                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"pairng successful: _ERM_gContext.req.ctx.initPairing.deviceType %d", _ERM_gContext.req.ctx.pairing.deviceType);
                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"pairng successful: deviceType: %d", _ERM_gContext.deviceTypeForPairing);
                        
                        if (pst_msg->data->pprs.device_id != ERM_u8GetActivePlayer() &&
                            pst_msg->data->pprs.device_id != ERM_u8GetActivePhone())
                        {
                            if( (APP_DEVICE_enTYPE_PHONE == _ERM_gContext.deviceTypeForPairing) &&
                               (ERM_u8GetActivePhone() != AT_INVALID_DEVICE_ID))
                            {   
                                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"pairing successful deactivate phone service of old device");
                                (void) APP_DEVICE_sfDeactivatePhoneService(ERM_u8GetActivePhone(), NULL);
                            }
                            else if((APP_DEVICE_enTYPE_PLAYER == _ERM_gContext.deviceTypeForPairing) &&
                                    (ERM_u8GetActivePlayer() != AT_INVALID_DEVICE_ID))
                            {
                                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"pairing successful deactivate player services of old device");
                                (void) APP_DEVICE_rcDeactivateMdiDevice(NULL);
                            }
                            else
                            {
                                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"pairing successful dont deactivate anything");
                            }
                            
                        }
                        else
                        {
                            DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"pairing successful but deviceid equal to a connected device");
                        }
                        break;
                    default:
                        /* clear former ssp request response */
                        _ERM_gContext.stSspRequest.remote_id = AT_INVALID_DEVICE_ID;
                        break;
                }

                switch(pst_msg->data->pprs.ext_device.device_type)
                {
                    case AT_INITIATED_DEVICE:
                    case AT_DELETED_DEVICE:
                    {
                        AT_DeviceId deviceId = pst_msg->data->pprs.device_id;

                        if( (pst_msg->data->pprs.result == AT_PAIRING_SUCCEEDED) &&
                            (deviceId != AT_INVALID_DEVICE_ID) )
                        {
                            /* trigger a PLPD request for the paired device */
                            _ERM_gContext.u8DeviceIdForMsg = deviceId;
                            (void)_ERM_SendMessage(ERM_REQ_GET_PAIRED_DEVICES,NULL);
                        }
                        /* else: ignore the message */
                        break;
                    }
                    default:
                    /* unsupported device type */
                        break;
                }

                _ERM_vNotifyApplication(&notification);
            }
            break;
        }
        case AT_RSP_BT_SDP_NOT:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                NULL != pst_msg->data                    )
            {
                switch(pst_msg->data->psdp.status)
                {
                    case AT_SDP_SUCCEED:
                    {
                        _ERM_sFBTSdpUpdate(&pst_msg->data->psdp);
                        /* trigger service information */
                        _ERM_gContext.u8DeviceIdForMsg = pst_msg->data->psdp.id;
                        (void)_ERM_SendMessage(ERM_REQ_BT_SERVICE_CONN,NULL);
                        break;
                    }
                    default:
                        /* no action */
                        break;
                }
            }
            break;
        }
        case AT_RSP_BT_SERVICE_CONN:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                NULL != pst_msg->data                    )
            {
                (void)_ERM_sFBTSupportedServiceUpdate(&pst_msg->data->pbsc);
                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"call servicesupport changed");
            }
            break;
        }
        case AT_RSP_STARTUP:
        {   /* NOTE: This shall normally not occur, but in case of flasherase
               CK5050 may restart - to handle this first possible blocked
               UART is freed with error and applications are triggered, then
               init is done and state is changed to ERM_STATE_WAIT_ADDRESS */


            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"CK5050 restart in state %d", _ERM_gContext.eState);

            /* if a bluetooth update was active it is finished now */
            CSHDL_vCK5050_Startup();
            ERM_vBluetoothUpdateStateMachine(ERM_BT_UPDATE_CK5050_REBOOT);

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
                        //(void)SetEvent(OsAppTask_event_100ms,evAppTaskVCRAT_ERROR );
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
            /* trigger state change to all applications - as idle is not
             * handled yet by application we use state ERM_STATE_CK5050_ACTIVE
             * to indicate that no BT device is available */
            _ERM_vChangeState(ERM_STATE_CK5050_ACTIVE);
            /* trigger idle state towards VCR */
            VCR_sfErmIndicationCk5050(BT_FALSE);
            /* now initialise ERM completely */
            (void)ERM_Startup();
            /* now change state back to ERM_STATE_WAIT_ADDRESS */
            _ERM_vChangeState(ERM_STATE_WAIT_ADDRESS);


            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"CK5050 restart DONE in state %d", _ERM_gContext.eState);

            /* IMPORTANT - we need to set fail as return code
             * to avoid that caller check for active devices,
             * otherwise incorrect state may be set          */
            sF = FAIL;
            break;
        }
        case AT_RSP_INQUIRY:
        {   /* handle pbdi */
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                if( AT_TRUE == pst_msg->data->pbdi.is_end )
                {
                    ERMNotificationType notification;
                    notification.common.id = ERM_NOTIFY_INQUIRY_END;
                    _ERM_vNotifyApplication(&notification);
                }
                else
                {
                    /* a device was found */
                    if(APP_DEVICE_bIsDeviceType(pst_msg->data->pbdi.bt_ClassOfDevice,
                                                APP_DEVICE_enTYPE_PHONE) == BT_TRUE)
                    {
                        _ERM_gContext.u8PhoneInquiryResult++;

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"inquiry device id %d is PHONE", pst_msg->data->pbdi.device_id);

                    }
                    if(APP_DEVICE_bIsDeviceType(pst_msg->data->pbdi.bt_ClassOfDevice,
                                                APP_DEVICE_enTYPE_HEADSET) == BT_TRUE)
                    {
                        _ERM_gContext.u8HeadsetInquiryResult++;

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"inquiry device id %d is HEADSET", pst_msg->data->pbdi.device_id);

                    }
                    if(APP_DEVICE_bIsDeviceType(pst_msg->data->pbdi.bt_ClassOfDevice,
                                                APP_DEVICE_enTYPE_PLAYER) == BT_TRUE)
                    {
                        _ERM_gContext.u8PlayerInquiryResult++;

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"inquiry device id %d is PLAYER", pst_msg->data->pbdi.device_id);
                    }

                    _ERM_gContext.u8TotalInquiryResult++;
                }
            }
            break;
        }
        case AT_RSP_IN_PAIR_REQUEST:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                ERMNotificationType notification;

                _ERM_gContext.stPairRequest.remote_id = pst_msg->data->pprq.remote_id;
                _ERM_gContext.stPairRequest.device_type = pst_msg->data->pprq.device_type;
                memcpy( _ERM_gContext.stPairRequest.device_name, 
                        pst_msg->data->pprq.name,
                        pst_msg->data->pprq.name_len+1 );
                _ERM_gContext.stPairRequest.name_len = pst_msg->data->pprq.name_len;

                notification.common.id = ERM_NOTIFY_PPRQ;
                _ERM_vNotifyApplication(&notification);

            }
            break;
        }    
        case AT_RSP_GET_INQUIRY_RESULT:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                if( AT_TRUE == pst_msg->data->pgir.is_count_only )
                {
                    /* update number of devices found through last inquiry */
                    _ERM_gContext.u8TotalInquiryResult = pst_msg->data->pgir.count;
                }
                else
                {
                    APP_DEVICE_DeviceType deviceType = _ERM_gContext.req.ctx.inquiry.deviceType;
                    Uint32Type cod = pst_msg->data->pgir.bt_ClassOfDevice;

                    if( APP_DEVICE_bIsDeviceType(cod, deviceType) == BT_TRUE )
                    {
                        /* we have the right device type */
                        _ERM_gContext.req.ctx.inquiry.recvdCount++;
                        if( (_ERM_gContext.req.ctx.inquiry.recvdCount >= _ERM_gContext.req.ctx.inquiry.reqStartIndex) &&
                            (_ERM_gContext.req.ctx.inquiry.recvdCount <= _ERM_gContext.req.ctx.inquiry.reqEndIndex) )
                        {
                            // we need at least name, bt_addr and class of device??
                            Uint8Type idx   = 0;
                            // get free idx
                            for( idx=0; idx<MAX_REQUESTED_INQUIRY_RESULT_ENTRIES; idx++)
                            {
                                if( BT_FALSE == _ERM_gContext.req.ctx.inquiry.result[idx].isValid )
                                {
                                    break;
                                }
                            }
                            // copy device info into ERM buffer
                            (void)memcpy( &_ERM_gContext.req.ctx.inquiry.result[idx].data,
                                    &pst_msg->data->pgir,
                                    sizeof(ATRspGetInquiryResType));
                            _ERM_gContext.req.ctx.inquiry.result[idx].isValid = BT_TRUE;
                        }
                    }
                }
            }
            break;
        }
        case AT_RSP_BT_CHIP_VERSION:
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                _ERM_gContext.stBtChipVersion.bIsValid = BT_TRUE;
                // copy device info into ERM buffer
                (void)memcpy( &_ERM_gContext.stBtChipVersion.hw_version,
                                &pst_msg->data->pbcv.hw_version,
                                pst_msg->data->pbcv.hw_version_len+1);
                (void)memcpy( &_ERM_gContext.stBtChipVersion.sw_version,
                                &pst_msg->data->pbcv.sw_version,
                                pst_msg->data->pbcv.sw_version_len+1);
                 (void)memcpy( &_ERM_gContext.stBtChipVersion.manufacturer,
                                &pst_msg->data->pbcv.manufacturer,
                                pst_msg->data->pbcv.manufacturer_len+1);
            }
            break;
        case AT_RSP_VT_AUTO_TAG:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                // copy mode
                _ERM_gContext.req.ctx.getPlayCallerName.bPlayCallerName =
                                             (BooleanType)pst_msg->data->pavp.mode_enable;
            }
            break;
        }
        case AT_RSP_IMSI:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                _ERM_gContext.bisCDMAPhone= BT_FALSE;

                switch (pst_msg->data->pssn.imsi_len)
                {
                    case 0:
                        _ERM_gContext.bisCDMAPhone= BT_TRUE;
                    break;
                    default:
                    {
                        if(BT_TRUE == _NetworkOperatorCodeIsVerizonOrAlltelOrSprint(pst_msg->data->pssn.imsi, pst_msg->data->pssn.imsi_len))
                        {
                            _ERM_gContext.bisCDMAPhone= BT_TRUE;
                        }
                        else
                        {
                            _ERM_gContext.bisCDMAPhone= BT_FALSE;
                        }
                    }
                    break;
                }
                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"IsCDMAPhone: %d",_ERM_gContext.bisCDMAPhone);
            }
            break;
        }
        case AT_RSP_FLASH_FULL_EVENT:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                if (pst_msg->data->pmfl.free_space_indicator)
                {
                   DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"Parrot Flash full event - free space is less than 10%");
                }
                else
                {
                   DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Parrot Flash full event - free space is above than 10 %");
                }
            }
            break;
        }
        case AT_RSP_GEN_WARNING:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                /* send notification depending on value of warning */
                switch( pst_msg->data->pwng.code)
                {
                    case AT_WARN_DIAL_ERROR     :
                    {
                        ERMNotificationType notification;
                        notification.common.id = ERM_NOTIFY_WARN_DIAL_ERROR;
                        _ERM_vNotifyApplication(&notification);
                        break;
                    }
                    case AT_WARN_BLUETOOTH_CHIP_ERROR:
                        /* Problem:
                           We get two tickets with a Bleutooth Chip Error Event.
                           -> Dead BT-Connection, freezed screens etc.
                           According to Parrot: we  should do a Reset.*/
                        (void) ERM_PWNG4_Execute_HW_Reset();
                        break;
                    case AT_WARN_HGUP_ERROR     :
                    {
                        ERMNotificationType notification;
                        notification.common.id = ERM_NOTIFY_WARN_HANGUP_ERROR;
                        _ERM_vNotifyApplication(&notification);
                        break;
                    }
                    case AT_WARN_PICK_ERROR     :
                    {
                        ERMNotificationType notification;
                        notification.common.id = ERM_NOTIFY_WARN_PICKUP_ERROR;
                        _ERM_vNotifyApplication(&notification);
                        break;
                    }
                    case AT_WARN_2CALL_ERROR    :
                    {
                        ERMNotificationType notification;
                        notification.common.id = ERM_NOTIFY_WARN_2ND_CALL_ERROR;
                        _ERM_vNotifyApplication(&notification);
                        break;
                    }
                    case AT_WARN_DTMF_ERROR     :
                    {
                        ERMNotificationType notification;
                        notification.common.id = ERM_NOTIFY_WARN_DTMF_ERROR;
                        _ERM_vNotifyApplication(&notification);
                        break;
                    }
                    case AT_WARN_AVRCP_ERROR    :
                    case AT_WARN_A2DP_ERROR     :
                    case AT_WARN_USB_ERROR      :
                    case AT_WARN_FLASH_FULL     :
                    case AT_WARN_BRUT_DISCONNECT:
                    case AT_WARN_RAM_FULL       :
                        break;
                }
            }

            break;
        }
        case AT_RSP_BT_SSP_SETUP:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data   )
            {
                if ((pst_msg->data->pssp.mode != pst_msg->data->pssp.nextMode)) {
                    /* actual mode and next mode not equal, means reset */
                    /* setup changed with next reboot */
                    DEBUG_VALUE4(ERM_SWI_ERR, MOD_ERM,"Reset necessary and usefull! mode: %d, iocap: %d, next mode: %d, next iocap: %d",
                                        pst_msg->data->pssp.mode, pst_msg->data->pssp.IOCap, pst_msg->data->pssp.nextMode, pst_msg->data->pssp.nextIOCap);
                }
                else {
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
                    if ((_ERM_gContext.eSSPMode != pst_msg->data->pssp.mode)\
                            || (_ERM_gContext.eSSPIoCap != pst_msg->data->pssp.IOCap))
                    {
                        /* we have to change the mode and/or iocap */
                        DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "Change SSP to eSSPMode = %d / eSSPIOCap = %d ", _ERM_gContext.eSSPMode,_ERM_gContext.eSSPIoCap);
                    }
                    else
                    {
                        DEBUG_TEXT(ERM_SWI_DBG, MOD_ERM,"Setup of SSP not needed.");
                    }
                }
            }
            break;
        }
        case AT_RSP_BT_SUP_FEAT:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd && NULL != pst_msg->data )
            {
                ERMNotificationType notification;

                notification.pbsf.device_id = pst_msg->data->pbsf.device_id;
                notification.pbsf.feature = pst_msg->data->pbsf.feature;
                notification.pbsf.result = pst_msg->data->pbsf.result;
                notification.common.id = ERM_NOTIFY_PBSF;
                _ERM_vNotifyApplication(&notification);
            }
            break;
        }
        case AT_RSP_BT_SSP_REQUEST:
        {
            if (AT_RSP_WITH_DATA == pst_msg->dataInd &&
                    NULL != pst_msg->data                    )
            {
                ERMNotificationType notification;

                _ERM_gContext.stSspRequest.remote_id = pst_msg->data->pspr.deviceId;
                _ERM_gContext.stSspRequest.device_type = pst_msg->data->pspr.deviceType;
                _ERM_gContext.stSspRequest.mode = pst_msg->data->pspr.mode;
                _ERM_gContext.stSspRequest.capability = pst_msg->data->pspr.capability;

                if(pst_msg->data->pspr.passAvail)
                {
                    memcpy( _ERM_gContext.stSspRequest.passphrase, 
                            pst_msg->data->pspr.passphrase,
                            sizeof(pst_msg->data->pspr.passphrase) );
                }
                notification.common.id = ERM_NOTIFY_PSPR;
                _ERM_vNotifyApplication(&notification);
            }
            break;
        }    
        case AT_RSP_POWER_OFF:
            APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_ERM);
#if defined(CRADLE_RESTRICTION_ACTIVE)
            CSHDL_vCK5050_PowerOFF_ACK();
#endif
            break;
        default: /* unsupported / invalid ERM type - trace and failure */
            sF = FAIL;

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"unsupported message: $ATResponseEnum$ = %d", pst_msg->common.rsp_type);
            break;
    }

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_SendMessage                                             */
/**
    \brief      Process the sending of ERM specific messages

    \brief      within startup ERM needs to send different messages to Parrot\n
                to do some init settings or fetch parrot name etc.\n
                this method use a static set table and specific values\n
                for the different required messages to be triggered.

    \param      to be updated

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode  _ERM_SendMessage(
        ERMIntMessageEnum msg_idx,
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal  = APP_COMMON_RSP_OK;
    ATReqAllType    msg;
    Uint32Type      i  = 0;
    BooleanType     is_retry = BT_FALSE;
//    BooleanType     bPlayerCoding;     /* player coding by diagnosis */

    if (ERM_REQ_RETRY == msg_idx)
    {   /* this is a retry triggered by ERM - check if values match */
        if (ERM_REQ_NONE != _ERM_gContext.reqERMRequest)
        {   /* valid for retry */
            msg_idx = _ERM_gContext.reqERMRequest;
            cb      = _ERM_gContext.cb_atOkErrCBReq;
            _ERM_gContext.reqERMRequest = ERM_REQ_NONE;
            is_retry = BT_TRUE;
        }
        else
        {   /* invalid retry - just discard */
            retVal = APP_COMMON_RSP_ERROR;

            DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"_ERM_SendMessage Request %d lastRequest %d -> invalid retry", _ERM_gContext.reqERMRequest,_ERM_gContext.lastERMRequest);
        }
    }

    /* first check if ERM is busy with another message - if so return busy */
    if (ERM_REQ_NONE != _ERM_gContext.reqERMRequest )
    {   /* ERM has still pending messge due to UART retries or is
           still waiting for OK or error response - block sending */
        retVal = APP_COMMON_RSP_ERROR_BUSY;

        DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"_ERM_SendMessage Request %d lastRequest %d -> busy", _ERM_gContext.reqERMRequest,_ERM_gContext.lastERMRequest);
    }
    /* previous checks are OK - process message */
    if (APP_COMMON_RSP_OK == retVal)
    {   /* check if the index is in allowed range  */
        if (ERM_MAX_NUM_INTERNAL_MSG > msg_idx)
        {   /* we can try to send the message */
            _ERM_gContext.reqERMRequest   = msg_idx;
            _ERM_gContext.cb_atOkErrCBReq = cb;


            DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM,"_ERM_SendMessage Request %d Callback %d -> try send", _ERM_gContext.reqERMRequest, (Uint16Type)_ERM_gContext.cb_atOkErrCBReq);

            /* check if we have to wait for OK of previous message - if
             * so do a retry here */
            if (ERM_REQ_NONE != _ERM_gContext.lastERMRequest)
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(
                        CTA_ERM_PENDING,  MSEC(10),   0);
            }
            else
            {   /* try to build the message */
                (void)memset(&msg,0x00,sizeof(ATReqAllType));
                switch (_ERM_gContext.reqERMRequest)
                {
                    case ERM_REQ_NONE:
                    case ERM_STARTUP_NUM_MESSAGES:
                    case ERM_STARTUP_NUM_MESSAGES_WO_SD:
                    case ERM_STARTUP_NUM_MESSAGES_WO_SD_MP_DISABLED: /* changed on 6.4.2011 by nikolov/RT:10180*/
                    case ERM_STARTUP_NUM_MESSAGES_WITH_SD:
                    {   /* this shall normally not occur and NOT trigger any message -
                           use error case BAP_PARSE here */
                        retVal = APP_COMMON_RSP_ERROR_BAP_PARSE;

                        DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"_ERM_SendMessage invalid message %d", _ERM_gContext.reqERMRequest);

                        break;
                    }
                    case ERM_REQ_CK5050_POWER_OFF:
                    {
                        msg.common.req_type = AT_REQ_POWER_OFF;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Prepare CK5050 Power off");
                        break;
                    }
                    case ERM_REQ_CK5050_SW_RESET:
                    {
                        msg.common.req_type = AT_REQ_TEST_SW_RESET;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"SW Reset CK5050");
                        break;
                    }
                    case ERM_REQ_BLUETOOTH_SW_UPDATE:
                    {
                        msg.pbsu.common.req_type = AT_REQ_FLASH_SW_UPDATE_BT;
                        msg.pbsu.BTSWupdate = (Uint8Type)ERM_enGetBluetoothUpdateRequest();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"BT SW Update mode %d", msg.pbsu.BTSWupdate);
                        break;
                    }
                    case ERM_REQ_SET_EXTENDED_ERROR:
                    {
                        msg.pbsu.common.req_type  = AT_REQ_SET_EXT_ERROR_CODES;
                        msg.peca.action      = AT_PECA_ACTIVATE;

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"set Peca mode:  %d",msg.peca.action);
                        break;
                    }
                    case ERM_REQ_BT_SET_CONN_AUTH:
                    {
                        msg.psca.common.req_type = AT_REQ_BT_SET_CONN_AUTH;
                        msg.psca.ConnectMode = 1;

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Connectable mode %d", msg.psca.ConnectMode);
                        break;
                    }
                    case ERM_REQ_FLASH_ERASE_USERDATA:
                    {
                        msg.common.req_type = AT_REQ_FLASH_ERASE_USERDATA;
                        break;
                    }
                    /* audio settings for initial CK5050 setup */
                    case ERM_REQ_MIC_ALGO_SELECT:
                    {
                        msg.palgoselect.common.req_type = AT_REQ_MIC_ALGO_SELECT;
                        msg.palgoselect.algotype        = _ERM_GetMicAlgotype();


                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Microphone Algorithm %d", msg.palgoselect.algotype);
                        break;
                    }
                    case ERM_REQ_MIC_CHOICE_SET:
                    {
                        msg.palgosetmic.common.req_type = AT_REQ_MIC_CHOICE_SET;
                        msg.palgosetmic.mic_id  = _ERM_GetMicChoice();


                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Microphone Choise %d", msg.palgosetmic.mic_id);
                        break;
                    }
                    case ERM_REQ_MIC_GAIN_SET:
                    {
                        msg.vgm.common.req_type = AT_REQ_MIC_GAIN_SET;
                        msg.vgm.mic_gain        = ERM_GetMicGain();


                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Microphone Gain %d", msg.vgm.mic_gain);
                        break;
                    }
                    case ERM_REQ_SPEAKER_VOLUME_NOTIFICATION:
                    {
                         msg.pevn.common.req_type = AT_REQ_AMGMT_SPEAKER_VOLUME;
                         msg.pevn.SpeakerVolumeNotification = AT_PEVN_SPEAKER_VOLUME_NOTIFICATION_DISABLE;
                         msg.pevn.ApplySpeakerVolume = AT_PEVN_DONT_APPLY_NEW_VOLUME_ON_SPEAKER;
                         DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Speaker Volume Mode %d ApplySpkVol %d", msg.pevn.SpeakerVolumeNotification, msg.pevn.ApplySpeakerVolume);
                         break;
                    }
                    case ERM_REQ_MIC_TUNE_NR:
                    {
                        msg.palgonr.common.req_type = AT_REQ_MIC_TUNE_NR;
                        msg.palgonr.maxAttNr        = _ERM_GetMicMaxAtt();
                        msg.palgonr.direction_avail = AT_TRUE;
                        msg.palgonr.direction       = _ERM_GetMicDirection();
                        msg.palgonr.inputGain_avail = AT_TRUE;
                        msg.palgonr.inputGain       = (Sint8Type)_ERM_GetMicInputGain();


                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Microphone NR Max %d Dir %d InGain %d", msg.palgonr.maxAttNr, msg.palgonr.direction, msg.palgonr.inputGain, 0);

                        break;
                    }
                    case ERM_REQ_MIC_EQUALIZE:
                    {
                        msg.palgotxeq.common.req_type = AT_REQ_MIC_EQUALIZE;
                        for (i = (Uint32Type)0U; i < (Uint32Type)AT_EQU_NUM_FREQ_BAND; i++)
                        {
                            msg.palgotxeq.Gain[i] = (Sint8Type)_ERM_GetTXGain(i);
                        }


                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Microphone Equ0-3 %d %d %d %d", msg.palgotxeq.Gain[0],msg.palgotxeq.Gain[1],msg.palgotxeq.Gain[2],msg.palgotxeq.Gain[3]);
                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Microphone Equ4-7 %d %d %d %d", msg.palgotxeq.Gain[4],msg.palgotxeq.Gain[5],msg.palgotxeq.Gain[6],msg.palgotxeq.Gain[7]);

                        break;
                    }
                    case ERM_REQ_MIC_ADC_CONF:
                    {
                        msg.palgoadc.common.req_type = AT_REQ_MIC_ADC_CONF;
                        msg.palgoadc.parameter       = AT_ADC_MIC_ALGO_GAIN;
                        msg.palgoadc.value           = _ERM_GetMicGainADC();


                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Microphone ADC GAIN %d %d", msg.palgoadc.parameter,msg.palgoadc.value);
                        break;
                    }
                    case ERM_REQ_TEST_MIC_ADCMODE:
                    {
                        msg.palgoadc.common.req_type = AT_REQ_MIC_ADC_CONF;
                        msg.palgoadc.parameter       = AT_ADC_MIC_ALGO_MODE;
                        msg.palgoadc.value           = _ERM_gContext.u8TestMicMode;


                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Testcommand ADC CONF %d %d", msg.palgoadc.parameter,msg.palgoadc.value);
                        break;
                    }
                    case ERM_REQ_TEST_MIC_ADCGAIN:
                    {
                        msg.palgoadc.common.req_type = AT_REQ_MIC_ADC_CONF;
                        msg.palgoadc.parameter       = AT_ADC_MIC_ALGO_GAIN;
                        msg.palgoadc.value           = _ERM_gContext.u8TestMicGain;


                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Testcommand MIC ADC GAIN %d %d", msg.palgoadc.parameter,msg.palgoadc.value);
                        break;
                    }
                    case ERM_REQ_TEST_LOOPBACK:
                    {
                        msg.ploopback.common.req_type = AT_REQ_TEST_AUDIO_LOOPBACK;
                        msg.ploopback.start_loop      = (ATBooleanEnum)_ERM_gContext.bTestLoopbackOn;
                        /* set mode fix to MIC1 */
                        msg.ploopback.mode            = AT_TEST_LOOPBACK_MIC1;


                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Testcommand LOOPBACK Start %d Mode %d", msg.ploopback.start_loop,msg.ploopback.mode);
                        break;
                    }
                    case ERM_REQ_TEST_AUDIOLOOP:
                    {
                        msg.paudioloop.common.req_type = AT_REQ_TEST_AUDIO_LOOP_TEST;
                        msg.paudioloop.mode            = AT_AUDIO_LOOP_TEST_MODE;
                        msg.paudioloop.bt_available    = AT_TRUE;
                        (void)memcpy(msg.paudioloop.bt_addr, _ERM_gContext.btAddrAudioLoop,
                               sizeof(_ERM_gContext.btAddrAudioLoop));
                        msg.paudioloop.delay_available = AT_TRUE;
                        msg.paudioloop.delay_local     = _ERM_gContext.u16AudioLoopDelayLocal;
                        msg.paudioloop.delay_BT        = _ERM_gContext.u16AudioLoopDelayBT;



                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Testcommand AUDIOLOOP");
                        break;
                    }
                    case ERM_REQ_TEST_TESTMODE_ON:
                    {
                        msg.common.req_type = AT_REQ_TEST_BT_TEST_MODE;


                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Testcommand TESTMODE ON");
                        break;
                    }
                    case ERM_REQ_TEST_FREQUENCY:
                    {
                        msg.common.req_type  = AT_REQ_TEST_OUT_FREQ_CHAN;
                        msg.pbfreq.mode      = _ERM_gContext.eTestFreqMode;


                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Testcommand TEST_FREQ %d", msg.pbfreq.mode);
                        break;
                    }
                    case ERM_REQ_AUTOCONN_ON:
                    case ERM_REQ_AUTOCONN_ON_MP_DISABLED: /* changed on 6.4.2011 by nikolov/RT:10180*/
                    {   /* in startup set autoconn on */
                        msg.common.req_type  = AT_REQ_BT_SET_AC_MODE;
                        msg.pscm.is_get            = AT_FALSE;
                        msg.pscm.action_available  = AT_FALSE;
                        msg.pscm.mode_disable      = AT_FALSE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"AUTO CONN ON");
                        break;
                    }
                    case ERM_REQ_TEST_AUTOCONN_OFF:
                    case ERM_REQ_AUTOCONN_OFF:
                    {   /* in set autoconn off */
                        msg.common.req_type  = AT_REQ_BT_SET_AC_MODE;
                        msg.pscm.is_get            = AT_FALSE;
                        msg.pscm.action_available  = AT_FALSE;
                        msg.pscm.mode_disable      = AT_TRUE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"AUTO CONN OFF");
                        break;
                    }
                    case ERM_REQ_SET_MAX_PAIRED_DEVICES:
                    {   /* in startup set max number of pairing devices */

                        msg.common.req_type  = AT_REQ_SET_MAX_PAIRED_DEVICES;
                        msg.psmd.is_get_number          = AT_FALSE;
                        msg.psmd.max_num_paired_device  = AT_PARROT_MAX_DEVICE_ID;

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"SET MAX PAIRED DEVICES %d", AT_PARROT_MAX_DEVICE_ID);
                        break;
                    }
//                    case ERM_REQ_TEST_AUTOCONN_OFF:
//                    {
//                        msg.common.req_type  = AT_REQ_BT_SET_AC_MODE;
//                        msg.pscm.is_get            = AT_FALSE;
//                        msg.pscm.action_available  = AT_FALSE;
//                        msg.pscm.mode_disable      = AT_TRUE;

//                        break;
//                    }
                    case ERM_REQ_SPK_RX_ALGO_SELECT:
                    {
                        msg.palgoselectrx.common.req_type = AT_REQ_SPK_RX_ALGO_SELECT;
                        msg.palgoselectrx.algoRX          = _ERM_GetRXAlgo();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Speaker Algorithm %d", msg.palgoselectrx.algoRX);
                        break;
                    }
                    case ERM_REQ_SPK_EQUALIZE:
                    {
                        msg.palgorxeq.common.req_type = AT_REQ_SPK_EQUALIZE;
                        for (i = (Uint32Type)0U; i < (Uint32Type)AT_EQU_NUM_FREQ_BAND; i++)
                        {
                            msg.palgorxeq.Gain[i] = (Sint8Type)_ERM_GetRXGain(i);
                        }


                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Speaker Equ0-3 %d %d %d %d", msg.palgorxeq.Gain[0],
                                                                                        msg.palgorxeq.Gain[1],
                                                                                        msg.palgorxeq.Gain[2],
                                                                                        msg.palgorxeq.Gain[3]);
                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Speaker Equ4-7 %d %d %d %d", msg.palgorxeq.Gain[4],
                                                                                        msg.palgorxeq.Gain[5],
                                                                                        msg.palgorxeq.Gain[6],
                                                                                        msg.palgorxeq.Gain[7]);
                        break;
                    }
//                    case ERM_REQ_SPK_LIM_HIGHPASS:				// This case will be removed, because it's not required
//																    // with current settings. Case will be used for initial
//																	// settings for ticket RT 2276
//                    {
//                        msg.palgorx.common.req_type = AT_REQ_SPK_LIM_HIGHPASS;
//                        msg.palgorx.limitLevel      = _ERM_GetSpeakerLimit();
//                        msg.palgorx.hpFilter_avail  = AT_TRUE;
//                        msg.palgorx.hpFilter        = _ERM_GetHPFilter();

//                        break;
//                    }
                    case ERM_REQ_ECHO_TUNE_CANCELLATION:
                    {
                        msg.palgoaec.common.req_type  = AT_REQ_ECHO_TUNE_CANCELLATION;
                        msg.palgoaec.maxAttAec        = _ERM_GetAECMaxAtt();
                        msg.palgoaec.switchTime_avail = AT_TRUE;
                        msg.palgoaec.switchTime       = _ERM_GetAECSwitchTime();


                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Echo Cancellation Algorithm %d %d %d %d",msg.palgoaec.maxAttAec,msg.palgoaec.switchTime_avail, msg.palgoaec.switchTime, 0);
                        break;
                    }
                    case ERM_REQ_AMGMT_OUT_VOLUME_SET_RING:
                    {
                        msg.pnvs.common.req_type  = AT_REQ_AMGMT_OUT_VOLUME_SET;
                        msg.pnvs.type             = AT_AUDIOTYPE_INC_RING;
                        msg.pnvs.volume           = _ERM_GetRingVolume();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Ringtone volume %d",msg.pnvs.volume);
                        break;
                    }
                    case ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT:
                    {
                        msg.pnvs.common.req_type  = AT_REQ_AMGMT_OUT_VOLUME_SET;
                        msg.pnvs.type             = AT_AUDIOTYPE_PROMPT;
                        msg.pnvs.volume           = ERM_GetPromptVolume();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prompt volume %d",msg.pnvs.volume);
                        break;
                    }
                    case ERM_REQ_GET_IMSI:
                    {
                        msg.common.req_type = AT_REQ_IMSI;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"REQ_GET_IMSI");
                        break;
                    }
                    case ERM_REQ_AMGMT_COMFORT_NOISE:
                    {
                    	msg.palognoisecomfort.common.req_type  = AT_REQ_ECHO_NOISE_COMFORT;
                        msg.palognoisecomfort.is_get           = AT_FALSE;
                        msg.palognoisecomfort.mode_activated   = _ERM_GetComfortNoise();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Comfort noise active %d",msg.palognoisecomfort.mode_activated);
                        break;
                    }

#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
                    case ERM_REQ_SPK_RX_ALC:
                    {
                        msg.palgoalcrx.common.req_type  = AT_REQ_SPK_RX_ALC;
                        msg.palgoalcrx.is_get           = AT_FALSE;
                        msg.palgoalcrx.mode_activated   = AT_TRUE; /* soft limiter in Rx */
                        msg.palgoalcrx.target           = _ERM_GetRXAlcTarget();
                        msg.palgoalcrx.mode             = _ERM_GetRXAlcMode();

                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"RX ALC active %d mode %d",msg.palgoalcrx.mode_activated,msg.palgoalcrx.target);
                        break;
                    }
                    case ERM_REQ_SPK_RX_SL:
                    {
                        msg.palgoslrx.common.req_type  = AT_REQ_SPK_RX_SL;
                        msg.palgoslrx.is_get           = AT_FALSE;

                        msg.palgoslrx.mode_activated   = AT_TRUE; /* soft limiter in TX */
                        msg.palgoslrx.level           = _ERM_GetRXSLLevel();


                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"RX soft limiter level - %d dB ",msg.palgoslrx.level);
                        break;
                    }
                    case ERM_REQ_SPK_TX_SL:
                    {
                        msg.palgosltx.common.req_type  = AT_REQ_SPK_TX_SL;
                        msg.palgosltx.is_get           = AT_FALSE;
                        if ( BT_TRUE == _ERM_GetTXSLMode())
                        {
                        msg.palgosltx.mode_activated  = AT_TRUE; /* soft limiter in TX enabled */
                        msg.palgosltx.level           = _ERM_GetTXSLLevel();
                        }
                        else
                        {
                            msg.palgoslrx.mode_activated   = AT_TRUE; /* no soft limiter in TX */
                        }

                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"TX soft limiter level= %d, mode= %d",msg.palgosltx.level,msg.palgosltx.mode_activated);
                        break;
                    }
#endif

                    case ERM_REQ_ACTIVATE_SW_UPDATE_CAPABILITY:
                    {
                        msg.pbsu.common.req_type  = AT_REQ_FLASH_SW_UPDATE_BT;
#if defined(ERM_USE_SIGNED_BT_UPDATE)
                        msg.pbsu.BTSWupdate       = AT_BT_SW_UPDATE_ACTIVATE_SW_CAPABILITY_IN_SECURED_MODE;
#else
                        msg.pbsu.BTSWupdate       = AT_BT_SW_UPDATE_ACTIVATE_SW_CAPABILITY;
#endif

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"BT SW Update mode %d",msg.pbsu.BTSWupdate);
                        break;
                    }
                    /* device specific audio settings */
                    case ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM:
                    {
                        msg.pnvs.common.req_type  = AT_REQ_AMGMT_OUT_VOLUME_SET;
                        msg.pnvs.type             = AT_AUDIOTYPE_COMM;
                        msg.pnvs.volume           = ERM_GetCommVolume();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Communication Volume %d", msg.pnvs.volume);
                        break;
                    }
                    case ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM:
                    {
                        msg.pnvs.common.req_type  = AT_REQ_AMGMT_OUT_VOLUME_SET;
                        msg.pnvs.type             = AT_AUDIOTYPE_STREAMING;
                        msg.pnvs.volume           = ERM_GetStreamVolume();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Streaming Volume %d", msg.pnvs.volume);
                        break;
                    }

                    case ERM_REQ_BT_SERVICE_CONN:
                    {
                        msg.pbsc.common.req_type = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.action          = AT_SERVICE_ACT_QUERY_SUPPORTED;
                        msg.pbsc.device_id       = _ERM_gContext.u8DeviceIdForMsg;
                        msg.pbsc.service_avail   = AT_FALSE;
                        break;
                    }
                    case ERM_REQ_INIT_GET_PAIRED_DEVICES:
                    {
                        msg.plpd.common.req_type = AT_REQ_GET_PAIRED_DEVICES;
                        msg.plpd.mode            = AT_PLPD_GET_ALL;
                        msg.plpd.device_id       = AT_INVALID_DEVICE_ID;
                        break;
                    }
                    case ERM_REQ_GET_PAIRED_DEVICES:
                    {   /* fill in data*/
                        if(_ERM_gContext.u8DeviceIdForMsg != AT_INVALID_DEVICE_ID)
                        {
                            msg.plpd.common.req_type = AT_REQ_GET_PAIRED_DEVICES;
                            msg.plpd.mode            = AT_PLPD_GET_DEVICE;
                            msg.plpd.device_id       = _ERM_gContext.u8DeviceIdForMsg;
                        }
                        else
                        {
                            msg.plpd.common.req_type = AT_REQ_GET_PAIRED_DEVICES;
                            msg.plpd.mode            = AT_PLPD_GET_ALL;
                            msg.plpd.device_id       = AT_INVALID_DEVICE_ID;
                            /* the response is handled by callback */
                        }
                        break;
                    }
                    case ERM_REQ_BT_VISIBILITY_ON:
                    case ERM_REQ_BT_VISIBILITY_OFF:
                    {   /* input index will indicate if visibilty shall
                           be switched on or off */
                        msg.psdm.common.req_type = AT_REQ_SET_DISCOVER_MODE;
                        if (ERM_REQ_BT_VISIBILITY_OFF == msg_idx)
                        {
                            msg.psdm.mode = AT_DISCOVER_MODE_DISALLOW;
                            _ERM_gContext.bBTVisible = BT_FALSE;


                            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"STOP Bluetooth Visibility %d -> %d", msg_idx, _ERM_gContext.bBTVisible);

                            if (BT_TRUE == _ERM_gContext.bBTVisIsInitial)
                            {
                                _ERM_gContext.bBTVisIsInitial = BT_FALSE;
                            }
                        }
                        else
                        {
                            msg.psdm.mode = AT_DISCOVER_MODE_ALLOW;
                            _ERM_gContext.bBTVisible = BT_TRUE;


                            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"START Bluetooth Visibility %d -> %d", msg_idx, _ERM_gContext.bBTVisible);
                        }
                        break;
                    }
                    case ERM_REQ_MODEL_REV:
                    {   /* this is the startup message - increment counter */
                        if ((Uint32Type)0U == _ERM_gContext.u32StartupCnt)
                        {
                            _ERM_gContext.u32StartupCnt = 1;
                        }
                        msg.common.req_type = AT_REQ_MODEL_REV;
                        break;
                    }
                    case ERM_REQ_MODEL_REV_EXT:
                    {
                        msg.common.req_type = AT_REQ_MODEL_REV_EXT;
                        break;
                    }
                    case ERM_REQ_MODEL_REV_PF_CODE:
                    {
                        msg.common.req_type = AT_REQ_MODEL_PF_CODE;
                        break;
                    }
                    case ERM_REQ_BT_CHIP_VERSION:
                    {
                        msg.common.req_type = AT_REQ_BT_CHIP_VERSION;
                        break;
                    }
                    case ERM_REQ_HDL_PAIR_REQUEST:
                    case ERM_REQ_HDL_PAIR_REQUEST_MP_DISABLED:
                    {
                        /* at least we shall indicate connection mode and PIN!!! */
                        msg.ppau.common.req_type = AT_REQ_HDL_PAIR_REQUEST;
                        msg.ppau.mode            = AT_PAIR_ACCEPT_INCOMING;
                        msg.ppau.pin_digits      = AT_UHV_BT_PIN_CODE_LEN;
                        for (i = (Uint32Type)0U; i < (Uint32Type)AT_UHV_BT_PIN_CODE_LEN; i++)
                        {
                            msg.ppau.pin[i] = _ERM_gContext.stBTPinCode[i];
                        }
                        break;
                    }
                    case ERM_REQ_EXT_CALL_LIST:
                    {
                        /* at least we shall indicate connection mode and PIN!!! */
                        msg.plccex.common.req_type = AT_REQ_EXT_CALL_LIST;
                        msg.plccex.mode            = AT_EXTCALL_ENABLE_MORE;
                        break;
                    }
                    case ERM_REQ_EXT_CALL_STACK:
                    {
                        msg.ppbcex.common.req_type = AT_REQ_PB_STACK_CHANGE;
                        msg.ppbcex.mode            = AT_PPBCEX_ENABLE;
                        break;
                    }
                    case ERM_REQ_SET_MAX_CALLS:
                    {
                        /* set maximum number of parallel calls */
                        msg.psmc.common.req_type = AT_REQ_SET_MAX_CALLS;
                        msg.psmc.value           = ERM_MAX_NUM_PLCC;
                        break;
                    }
                    case ERM_REQ_DISABLE_INBAND_RINGTONE:
                    {
                        /* disable the inband ringtone */
                        msg.pirt.common.req_type = AT_REQ_INBOUND_RINGTONE;
                        msg.pirt.action          = AT_PIRT_ACT_DISABLE;
                        break;
                    }
                    case ERM_REQ_DISABLE_DTMF_AFTER_DIALING:
                    {
                        /* disable the inband ringtone */
                        msg.pcdd.common.req_type = AT_REQ_DTMF_PLAYING;
                        msg.pcdd.state          = AT_DTMF_AFTER_DIALING_DISABLE;
                        break;
                    }
                    case ERM_REQ_AUDIO_LOCK:
                    {
                        /* lock a audio source */
                        msg.alck.common.req_type = AT_REQ_AMGMT_AUDIO_LOCK;
                        msg.alck.locked          = AT_ALCK_LOCK_START;
                        break;
                    }
                    case ERM_REQ_SET_OBJECT_PUSH:
                    {
                        /* disable oject push */
                        msg.popm.common.req_type = AT_REQ_PB_OBJECT_PUSH_MODE;
                        msg.popm.mode            = AT_POPM_OOP_REJECT_ALL_TRANSFER;
                        break;
                    }
                    case ERM_REQ_TEST_RX_TX:
                    {
                        /* lock a audio source */
                        msg.pbfreqex.common.req_type = AT_REQ_TEST_OUT_FREQ_CHAN_EXT;
                        msg.pbfreqex.testscenario = AT_TX_BURST_MODE_IS_SWITCHED_ON;
                        msg.pbfreqex.packettype =  15;
                        msg.pbfreqex.bitpattern = AT_PRBS9_SEQUENCE;
                        msg.pbfreqex.singlefrequency = 0;
                        msg.pbfreqex.txburstperiod =  2;
                        msg.pbfreqex.scramblermode = AT_SCRAMBLER_SWITCHED_OFF;
                        msg.pbfreqex.powerlevel =  AT_POWER_LEVEL_HIGHEST;
                        msg.pbfreqex.packetsize =  341;
                        break;
                    }

                    case ERM_REQ_SET_BT_NAME:
                    {
                        /* at least we shall indicate connection mode and PIN!!! */
                        msg.psfn.common.req_type = AT_REQ_SET_BT_NAME;
                        msg.psfn.is_get_name     = AT_FALSE;                        
                        
                        (void)snprintf((char *)&(msg.psfn.name[0]),AT_BT_DEVICE_NAME_MAX_LEN,
                                "%s",_ERM_gContext.stBTName);
                        msg.psfn.name_len = (Uint16Type)strlen((const char *)&(msg.psfn.name[0]));
                        
                        // check for stable system : ensure BT_NAME is valid
                        if ((Uint16Type)strlen((const char *)&(msg.psfn.name[0])) < AT_BT_DEVICE_NAME_MIN_LEN
                          || (Uint16Type)strlen((const char *)&(msg.psfn.name[0])) > AT_BT_DEVICE_NAME_MAX_LEN)
                        {
                            _ERM_gContext.stBTName[0] = 86;  //V
                            _ERM_gContext.stBTName[1] = 87;  //W
                            _ERM_gContext.stBTName[2] = 95;  //_
                            _ERM_gContext.stBTName[3] = 80;  //P
                            _ERM_gContext.stBTName[4] = 72;  //H
                            _ERM_gContext.stBTName[5] = 79;  //O
                            _ERM_gContext.stBTName[6] = 78;  //N
                            _ERM_gContext.stBTName[7] = 69;  //E
                            _ERM_gContext.stBTName[10] = 0x0;  //\0 Nullterminierung
                            (void)snprintf((char *)&(msg.psfn.name[0]),AT_BT_DEVICE_NAME_MAX_LEN,
                                "%s",_ERM_gContext.stBTName);
                            msg.psfn.name_len = (Uint16Type)strlen((const char *)&(msg.psfn.name[0]));
                            
                            DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"BT_NAME wrong check eeprom!");
                        }                        
                        
                        


                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Set BT Name0_3 to %c%c%c%c",msg.psfn.name[0], msg.psfn.name[1], msg.psfn.name[2], msg.psfn.name[3]);
                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Set BT Name4_7 to %c%c%c%c",msg.psfn.name[4], msg.psfn.name[5], msg.psfn.name[6], msg.psfn.name[7]);
                        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Set BT Name8_11 to %c%c%c%c",msg.psfn.name[8], msg.psfn.name[9], msg.psfn.name[10], msg.psfn.name[11]);
                        break;
                    }
                    case ERM_REQ_AUDIO_GATEWAY:					// RT 2276
                    {
                        msg.pagm.common.req_type = AT_REQ_BT_AG_MODE;
                        msg.pagm.mode = AT_AUDIO_GW_MODE_OFF;


                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Audio gateway mode switch to UHV");
                        break;
                    }
                    case ERM_REQ_TEST_DELETE_ALL_DEVICES:
                    {
                        msg.ppde.common.req_type    = AT_REQ_PAIR_DELETE;
                        /* delete all paired devices */
                        msg.ppde.device_available   = AT_FALSE;


                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Delete all paired devices");
                        break;
                    }
                    case ERM_REQ_DELETE_DEVICE:
                    {
                        msg.ppde.common.req_type    = AT_REQ_PAIR_DELETE;
                        msg.ppde.device_available   = AT_TRUE;
                        msg.ppde.device_id          =
                            _ERM_gContext.u8DeleteDeviceId;



                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Delete device id %d", msg.ppde.device_id);
                        break;
                    }
                    case ERM_REQ_AMGMT_SET_RINGTONE:
                    {   /* apply ringtone setting for active device only */
                        msg.psrt.common.req_type = AT_REQ_AMGMT_SET_RINGTONE;
                        msg.psrt.is_get = AT_FALSE;
                        msg.psrt.mode = _ERM_eGetActiveDeviceRingtone();
                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Ringtone Mode %d", msg.psrt.mode);
                        break;
                    }
                    case ERM_REQ_AMGMT_SET_RINGTONE_ALTERNATE:
                    {   /* apply alternate ringtone setting for active device only */
                        msg.rpdr.common.req_type = AT_REQ_SIVR_SET_SYSTEM_PROMPT;
                        msg.rpdr.is_get = AT_FALSE;
                        msg.rpdr.mode_enable = (ATBooleanEnum)_ERM_eGetActiveDeviceRingtoneAlternate();
                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Alternative Ringtone Enable %d", msg.rpdr.mode_enable);
                        break;
                    }
                    case ERM_REQ_ECHO_DELAY_CANCELLATION:
                    {
                        msg.palgoaecdelay.common.req_type = AT_REQ_ECHO_DELAY_CANCELLATION;
                        msg.palgoaecdelay.is_get = AT_FALSE;
                        msg.palgoaecdelay.delay  = _ERM_GetAECDelay();

                        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Echo Cancellation Delay %d", msg.palgoaecdelay.delay);
                    break;
                    }
                    case ERM_REQ_START_INQUIRY:
                    {
                        _ERM_gContext.req.enLastId = ERM_REQ_START_INQUIRY;
                        msg.common.req_type  = AT_REQ_INQUIRY;
                        msg.pbdi.is_initiate = AT_TRUE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Starting inquiry");
                        break;
                    }
                    case ERM_REQ_CANCEL_INQUIRY:
                    {
                        _ERM_gContext.req.enLastId = ERM_REQ_CANCEL_INQUIRY;
                        msg.common.req_type  = AT_REQ_INQUIRY;
                        msg.pbdi.is_initiate = AT_FALSE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Cancelling inquiry");
                        break;
                    }
                    case ERM_REQ_INQUIRY_RESULT:
                    {
                        // TODO: use params given with ERM_rcGetInquiryResult
                        Uint8Type startIndex = (Uint8Type)_ERM_gContext.req.ctx.inquiry.startIndex;
                        Uint8Type endIndex   = (Uint8Type)_ERM_gContext.req.ctx.inquiry.endIndex;

                        //if((endIndex - startIndex) > MAX_REQUESTED_INQUIRY_RESULT_ENTRIES)
                        //{
                        //    endIndex = startIndex + MAX_REQUESTED_INQUIRY_RESULT_ENTRIES - 1;
                        //}
                        msg.common.req_type         = AT_REQ_GET_INQUIRY_RESULT;
                        msg.pgir.is_get             = AT_FALSE;
                        msg.pgir.startIndex         = startIndex;
                        msg.pgir.endIndex           = endIndex;
                        _ERM_gContext.req.enLastId  = ERM_REQ_INQUIRY_RESULT;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Get inquiry result");
                        break;
                    }
                    case ERM_REQ_STOP_HEADSET_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.discHeadsetServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_STOP;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_AUDIO_GATEWAY;

                        _ERM_gContext.req.enLastId  = ERM_REQ_STOP_HEADSET_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to STOP HEADSET service");
                        break;
                    }
                    case ERM_REQ_START_HEADSET_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.startHeadsetServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_START;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_AUDIO_GATEWAY;

                        _ERM_gContext.req.enLastId  = ERM_REQ_START_HEADSET_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to START HEADSET service");
                        break;
                    }
                    case ERM_REQ_STOP_PLAYER_AVRCP_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.discPlayerAvrcpServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_STOP;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_AVRCP;

                        _ERM_gContext.req.enLastId  = ERM_REQ_STOP_PLAYER_AVRCP_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to STOP AVRCP service");
                        break;
                    }
                    case ERM_REQ_START_PLAYER_AVRCP_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.connPlayerAvrcpServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_START;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_AVRCP;

                        _ERM_gContext.req.enLastId  = ERM_REQ_START_PLAYER_AVRCP_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to START AVRCP service");
                        break;
                    }
                    case ERM_REQ_STOP_PLAYER_A2DP_SINK_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.discPlayerA2dpSinkServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_STOP;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_A2DP_SINK;

                        _ERM_gContext.req.enLastId  = ERM_REQ_STOP_PLAYER_A2DP_SINK_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to STOP A2DP service");

                        break;
                    }
                    case ERM_REQ_START_PLAYER_A2DP_SINK_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.connPlayerA2dpSinkServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_START;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_A2DP_SINK;

                        _ERM_gContext.req.enLastId  = ERM_REQ_START_PLAYER_A2DP_SINK_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to START A2DP service");
                        break;
                    }
                    case ERM_REQ_STOP_PHONE_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id       = (Uint8Type)_ERM_gContext.req.ctx.discPhoneServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_STOP;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_PHONE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_STOP_PHONE_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to STOP PHONE service");
                        break;
                    }
                    case ERM_REQ_START_PHONE_SERVICE:
                    {
                        msg.common.req_type     = AT_REQ_BT_SERVICE_CONN;
                        msg.pbsc.device_id      = (Uint8Type)_ERM_gContext.req.ctx.connPhoneServ.deviceId;
                        msg.pbsc.action         = AT_SERVICE_ACT_START;
                        msg.pbsc.service_avail  = AT_TRUE;
                        msg.pbsc.service        = AT_SERVICE_PHONE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_START_PHONE_SERVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to START PHONE service");
                        break;
                    }
                    case ERM_REQ_DELETE_DEVICE_ID:
                    {
                        msg.common.req_type         = AT_REQ_PAIR_DELETE;
                        msg.ppde.device_available   = AT_TRUE;
                        msg.ppde.device_id          = (Uint8Type)_ERM_gContext.req.ctx.deleteDevice.deviceId;

                        _ERM_gContext.req.enLastId  = ERM_REQ_DELETE_DEVICE_ID;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to DELETE DEVICE pairing");
                        break;
                    }
                    case ERM_REQ_CONNECT_DEVICE:
                    {
                        msg.common.req_type         = AT_REQ_BT_SELECT_DEV;
                        msg.psbd.device_id          = (Uint8Type)_ERM_gContext.req.ctx.connectDevice.deviceId;
                        msg.psbd.value              = AT_DEVSEL_CONNECT;

                        _ERM_gContext.req.enLastId  = ERM_REQ_CONNECT_DEVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to CONNECT DEVICE");
                        break;
                    }
                    case ERM_REQ_DISCONNECT_DEVICE:
                    {
                        msg.common.req_type         = AT_REQ_BT_SELECT_DEV;
                        msg.psbd.device_id          = (Uint8Type)_ERM_gContext.req.ctx.connectDevice.deviceId;
                        msg.psbd.value              = AT_DEVSEL_DISCONNECT;

                        _ERM_gContext.req.enLastId  = ERM_REQ_DISCONNECT_DEVICE;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to DISCONNECT DEVICE");
                        break;
                    }
                    case ERM_REQ_SET_PLAY_CALLER_NAME:
                    {
                        msg.common.req_type         = AT_REQ_SIVR_SET_SYSTEM_PROMPT;
                        msg.pavp.is_get             = AT_FALSE;
                        msg.pavp.mode_enable        = (ATBooleanEnum)_ERM_gContext.req.ctx.setPlayCallerName.bPlayCallerName;

                        _ERM_gContext.req.enLastId  = ERM_REQ_SET_PLAY_CALLER_NAME;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to set set PLAY CALLER NAME mode");
                        break;
                    }
                    case ERM_REQ_GET_PLAY_CALLER_NAME:
                    {
                        msg.common.req_type         = AT_REQ_SIVR_SET_SYSTEM_PROMPT;
                        msg.pavp.is_get             = AT_TRUE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_GET_PLAY_CALLER_NAME;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Req play caller name mode");
                        break;
                    }
                    case ERM_REQ_PAIRING:
                    {
                        msg.common.req_type         = AT_REQ_IN_PAIR_REQUEST;
                        msg.pprq.pairingCmd         = _ERM_gContext.req.ctx.pairing.command;
                        msg.pprq.device_id          = _ERM_gContext.req.ctx.pairing.deviceId;
                        msg.pprq.device_type        = _ERM_gContext.req.ctx.pairing.deviceType;
                        
                        if(_ERM_gContext.req.ctx.pairing.pinAvailable)
                        {
                            msg.pprq.pin_avail          = AT_TRUE;
                            msg.pprq.pin_digits         = _ERM_gContext.req.ctx.pairing.numPinDigits;

                            (void)memset( &msg.pprq.pin, 0, sizeof( msg.pprq.pin ));
                            (void)memcpy( &msg.pprq.pin, _ERM_gContext.req.ctx.pairing.pin,
                                                  _ERM_gContext.req.ctx.pairing.numPinDigits);
                        }
                        else
                        {
                            msg.pprq.pin_avail          = AT_FALSE;
                        }

                        _ERM_gContext.req.enLastId  = ERM_REQ_PAIRING;

                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"PAIRING device id %d, cmd %d", 
                                    msg.pprq.device_id, _ERM_gContext.req.ctx.pairing.command);
                        break;
                    }
                    case ERM_REQ_SSP_SETUP:
                    case ERM_REQ_SSP_CONFIG:
                    {
                        msg.common.req_type = AT_REQ_SSP_SETUP;
                        if (ERM_REQ_SSP_SETUP == _ERM_gContext.reqERMRequest) {
                            msg.pssp.mode = AT_SSP_REQUEST;
                        }
                        else {
                            msg.pssp.mode = _ERM_gContext.eSSPMode;
                            msg.pssp.IOCap = _ERM_gContext.eSSPIoCap;
                        }
                        _ERM_gContext.req.enLastId  = ERM_REQ_SSP_SETUP;
                        break;
                    }
                    case ERM_REQ_SSP_SUPPORT:
                    {
                        msg.common.req_type = AT_REQ_BT_SUP_FEAT;
                        msg.pbsf.device_id = _ERM_gContext.req.ctx.pairing.deviceId;
                        msg.pbsf.feature = AT_SSP_FEATURE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_SSP_SUPPORT;
                        break;
                    }
                    case ERM_REQ_INQUIRY_EXT_RESULT:
                    {
                        DEBUG_TEXT(ERM_SWI_TRC,MOD_ERM,"Extented Inquiry Results 0x9D activated!");
                        msg.common.req_type = AT_REQ_INQUIRY_EXT_RESULT;
                        _ERM_gContext.req.enLastId = ERM_REQ_INQUIRY_EXT_RESULT;

                        break;
                    }
                    case ERM_REQ_INIT_DISABLE_PHONE_AUTO_CONN:
                    case ERM_REQ_DISABLE_PHONE_AUTO_CONN:
                    {
                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
                        msg.pbsa.service    = AT_SERVICE_PHONE;
                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_NONE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_DISABLE_PHONE_AUTO_CONN;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to DISABLE AUTO-CONNECTION for PHONE service");
                        break;
                    }
                    case ERM_REQ_INIT_DISABLE_A2DP_AUTO_CONN:
                    case ERM_REQ_INIT_DISABLE_A2DP_AUTO_CONN_MP_DISABLED: /* changed on 6.4.2011 by nikolov/RT:10180*/
                    {
                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
                        msg.pbsa.service    = AT_SERVICE_A2DP_SINK;
                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_NONE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_DISABLE_A2DP_AUTO_CONN;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to DISABLE AUTO-CONNECTION for A2DP service");
                        break;
                    }
                    case ERM_REQ_INIT_DISABLE_AVRCP_AUTO_CONN:
                    case ERM_REQ_INIT_DISABLE_AVRCP_AUTO_CONN_MP_DISABLED: /* changed on 6.4.2011 by nikolov/RT:10180*/
                    {
                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
                        msg.pbsa.service    = AT_SERVICE_AVRCP;
                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_NONE;

                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_DISABLE_AVRCP_AUTO_CONN;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to DISABLE AUTO-CONNECTION for AVRCP service");
                        break;
                    }
                    case ERM_REQ_HDL_PAIR_REQUEST_FORWARD:
                    {
                        /* at least we shall indicate connection mode and PIN!!! */
                        msg.ppau.common.req_type = AT_REQ_HDL_PAIR_REQUEST;
                        msg.ppau.mode            = AT_PAIR_FORWARD_INCOMING;
                        msg.ppau.pin_digits      = 0;
                        break;
                    }
                    /* changed on 6.4.2011 by nikolov/RT:10180(headset functionality not supported any more)*/
//                    case ERM_REQ_INIT_DISABLE_AUDIO_GW_AUTO_CONN: 
//                    case ERM_REQ_INIT_DISABLE_AUDIO_GW_AUTO_CONNMP_DISABLED:
//                    {
//                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
//                        msg.pbsa.service    = AT_SERVICE_AUDIO_GATEWAY;
//                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_NONE;
//
//                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_DISABLE_AUDIO_GW_AUTO_CONN;
//
//                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to DISABLE AUTO-CONNECTION for AUDIO GW service");
//                        break;
//                    }
                    case ERM_REQ_INIT_ENABLE_PHONE_AUTO_CONN:
                    case ERM_REQ_INIT_ENABLE_PHONE_AUTO_CONN_MP_DISABLED:
                    {
                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
                        msg.pbsa.service    = AT_SERVICE_PHONE;
                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_ALL;

                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_ENABLE_PHONE_AUTO_CONN;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to ENABLE AUTO-CONNECTION for PHONE service");
                        break;
                    }
                    case ERM_REQ_INIT_ENABLE_A2DP_AUTO_CONN:
                    {
                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
                        msg.pbsa.service    = AT_SERVICE_A2DP_SINK;
                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_ALL;

                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_ENABLE_A2DP_AUTO_CONN;


                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to ENABLE AUTO-CONNECTION for A2DP service");
                        break;
                    }
                    case ERM_REQ_INIT_ENABLE_AVRCP_AUTO_CONN:
                    {
                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
                        msg.pbsa.service    = AT_SERVICE_AVRCP;
                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_ALL;

                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_ENABLE_AVRCP_AUTO_CONN;

                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to ENABLE AUTO-CONNECTION for AVRCP service");

                        break;
                    }/* changed on 6.4.2011 by nikolov/RT:10180(headset functionality not supported any more)*/
//                    case ERM_REQ_INIT_ENABLE_AUDIO_GW_AUTO_CONN:
//                    {
//                        msg.common.req_type = AT_REQ_BT_SERVICE_AC;
//                        msg.pbsa.service    = AT_SERVICE_AUDIO_GATEWAY;
//                        msg.pbsa.mode       = AT_BT_AUTO_CONN_MODE_ALL;
//
//                        _ERM_gContext.req.enLastId  = ERM_REQ_INIT_ENABLE_AUDIO_GW_AUTO_CONN;
//
//                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Trying to ENABLE AUTO-CONNECTION for  AUDIO GW service");
//                        break;
//                    }
                    case ERM_REQ_RINGTONE_PLAY:
                    {
                        msg.common.req_type = AT_REQ_PLAY_RINGTONE;
                        msg.pprt.action     = _ERM_gContext.req.ctx.ringtonePlay.action;
                        msg.pprt.ringtoneId = _ERM_gContext.req.ctx.ringtonePlay.ringtoneId;

                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Play Ringtone Action %d Index %d", msg.pprt.action,msg.pprt.ringtoneId);
                        break;
                    }
                    case ERM_REQ_ALTERNATING_SIGNAL:
                    {
                        msg.common.req_type = AT_REQ_ALTERNATING_SIGNAL;

                        /*
                        msg.agsw = _ERM_gContext.alternatingSignal;
                        */

                        msg.agsw.frequencyLeft    = _ERM_gContext.alternatingSignal.frequencyLeft;
                        msg.agsw.frequencyRight   = _ERM_gContext.alternatingSignal.frequencyRight;
                        msg.agsw.chanLeftOn       = _ERM_gContext.alternatingSignal.chanLeftOn;
                        msg.agsw.chanRightOn      = _ERM_gContext.alternatingSignal.chanRightOn;
                        msg.agsw.outputLevelLeft  = _ERM_gContext.alternatingSignal.outputLevelLeft;
                        msg.agsw.outputLevelRight = _ERM_gContext.alternatingSignal.outputLevelRight;



                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Testcommand Alternating Signal");
                        break;
                    }
                    default:
                    {   /* this is an unsupported message - use BAP_PARSE error code */
                        retVal = APP_COMMON_RSP_ERROR_BAP_PARSE;

                        break;
                    }
                }
                /* check successful preparation of the message */
                if (APP_COMMON_RSP_OK == retVal)
                {   /* now try to get the UART */
                    if (NULL != ERM_GetUartForERM())
                    {
                        if (SUCCESS != AT_Cmd_Handler_ERM(&msg,
                                _ERM_gContext.pUartPtr))
                        {   /* release UART pointer to avoid blocking */
                            (void)ERM_ReleaseUartForErm();

                            retVal = APP_COMMON_RSP_ERROR_AT_SEND;
                            /* clear retry counter as also requested msg */
                            _ERM_gContext.reqRetryCnt = 0x0000;
                            _ERM_gContext.reqERMRequest = ERM_REQ_NONE;
                            /* if retry we need to send cb */
                            if (BT_TRUE == is_retry &&
                                NULL != _ERM_gContext.cb_atOkErrCBReq)
                            {
                                _ERM_gContext.cb_atOkErrCBReq(retVal);
                                _ERM_gContext.cb_atOkErrCBReq = NULL;
                            }
                        }
                        else
                        {   /* the message is send - copy to last send */
                            _ERM_gContext.reqRetryCnt = 0x0000;
                            _ERM_gContext.lastERMRequest = _ERM_gContext.reqERMRequest;
                            _ERM_gContext.reqERMRequest = ERM_REQ_NONE;
                            /* also copy callback */
                            _ERM_gContext.cb_atOkErrCBLast = _ERM_gContext.cb_atOkErrCBReq;
                            _ERM_gContext.cb_atOkErrCBReq = NULL;

                            if (ERM_REQ_FLASH_ERASE_USERDATA == msg_idx)
                            {
                                /* check for flasherase if so, */
                                /* block UART by setting u8Active to FALSE */
                                /* if ERASE is done Init of ERM is triggered anyway */
                                _ERM_gContext.u8Active   = FALSE;
                            }


                           DEBUG_VALUE4(ERM_SWI_DBG, MOD_ERM,"SndMsg idx lastReq %d retVal %d ReqCnt %d",_ERM_gContext.lastERMRequest,
                                                                                                         retVal,
                                                                                                         _ERM_gContext.reqERMRequest,
                                                                                                         _ERM_gContext.u32StartupCnt);
                        }
                    }
                    else
                    {   /* we need to retry to get the UART - this will be done
                           internally in respect to the retry counter */
                        _ERM_gContext.reqRetryCnt = _ERM_gContext.reqRetryCnt + 1;
                        if (ERM_MAX_UART_RETRIES > _ERM_gContext.reqRetryCnt )
                        {   /* we will retry internally -
                               response with OK so far and start retry timer */
                            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(
                                    CTA_ERM_PENDING,  MSEC(ERM_UART_BUSY_RETRY_TIME),   0);
                            retVal = APP_COMMON_RSP_OK;

                            DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"SndMsg ReqRetry %d Req %d lastReq %d UartOwner %d",_ERM_gContext.reqRetryCnt,
                                                                                                                _ERM_gContext.reqERMRequest,
                                                                                                                _ERM_gContext.lastERMRequest,
                                                                                                                _ERM_gContext.eUartOwner);
                        }
                        else
                        {   /* max retries for this message reached, handle counter
                               and return with UART blocked */

                            retVal = APP_COMMON_RSP_ERROR_UART_BLOCKED;
                            _ERM_gContext.reqRetryCnt = 0x0000;
                            /* clear the not send message */
                            _ERM_gContext.reqERMRequest = ERM_REQ_NONE;
                            /* check if there exists a callback, if so call and
                             * clean */
                            if (BT_TRUE == is_retry &&
                                NULL != _ERM_gContext.cb_atOkErrCBReq)
                            {
                                _ERM_gContext.cb_atOkErrCBReq(APP_COMMON_RSP_ERROR_UART_BLOCKED);
                                _ERM_gContext.cb_atOkErrCBReq = NULL;
                            }
                        }
                    }
                }
                else
                {   /* in case of retry we need to call callback */
                    if (BT_TRUE == is_retry &&
                        NULL != _ERM_gContext.cb_atOkErrCBReq)
                    {
                        _ERM_gContext.cb_atOkErrCBReq(retVal);
                        _ERM_gContext.cb_atOkErrCBReq = NULL;
                    }
                    /* we need to clear the request and CB */
                    _ERM_gContext.cb_atOkErrCBReq = NULL;
                    _ERM_gContext.reqERMRequest   = ERM_REQ_NONE;
                }
            }
        }
        else
        {   /* return error!! */
            retVal = APP_COMMON_RSP_ERROR;

        }
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfHandleOkErrorRsp                                      */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfHandleOkErrorRsp(ATResponseEnum  rsp )
{
    SuccessFailType result = SUCCESS;
    ERMIntMessageEnum tmp_last = _ERM_gContext.lastERMRequest;
    /* we need to store the CB, as in internal CB next message can be called
     * -> then CB is newly set!! */
    void (*cb)(APP_COMMON_enResponseCode) = _ERM_gContext.cb_atOkErrCBLast;



    if (rsp == AT_RSP_ERROR)
    {
        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ATRespone ERROR Req %d lastReq %d",_ERM_gContext.reqERMRequest,
                                                                              _ERM_gContext.lastERMRequest);
    }
    else if (rsp == AT_RSP_OK)
    {
        DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM,"ATRespone OK Req %d lastReq %d",_ERM_gContext.reqERMRequest,
                                                                           _ERM_gContext.lastERMRequest);
    }

    if( (rsp != AT_RSP_ERROR) && (rsp != AT_RSP_OK) )
    {
        return FAIL;
    }
    else
    {   /* we received OK or ERROR for send message - check which
           was the send message */
        /* clear last send message as we received OK or ERROR */
        _ERM_gContext.lastERMRequest = ERM_REQ_NONE;
        _ERM_gContext.cb_atOkErrCBLast = NULL;
        /* check for callback */
        if (NULL != cb)
        {
            if ((AT_RSP_OK == rsp))
            {
                cb(APP_COMMON_RSP_OK);
            }
            else
            {
                cb(APP_COMMON_RSP_ERROR);
            }
        }
        /* check if last send was startup message */
        if ( tmp_last == (ERMIntMessageEnum)_ERM_gContext.u32StartupCnt)
        {   /* check if we started BT visibility in startup - start timer */
            if (BT_FALSE == _ERM_gContext.bStartupDone &&
                ERM_REQ_BT_VISIBILITY_ON == tmp_last )
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_BT_VISIBILITY_TIMER,
                        SEC(ERM_INITIAL_BT_VISIBILITY_SEC), 0);
                DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"STARTED INITIAL BT VISIBILITY");
            }
            /* this was a startup sending - increment counter */
            _ERM_gContext.u32StartupCnt = _ERM_gContext.u32StartupCnt + (Uint32Type)1U;
        }
        /* check for ongoing ERM startup */
        if (BT_FALSE == _ERM_gContext.bStartupDone)
        {
            if( (Uint32Type)ERM_STARTUP_NUM_MESSAGES_WO_SD == _ERM_gContext.u32StartupCnt ||
                (Uint32Type)ERM_STARTUP_NUM_MESSAGES_WITH_SD == _ERM_gContext.u32StartupCnt ||
                (Uint32Type)ERM_STARTUP_NUM_MESSAGES_WO_SD_MP_DISABLED == _ERM_gContext.u32StartupCnt)
            {
                /* SD Configuration specific intitial startup done */
                /* start the consistency check */
                if(_ERM_gContext.eConsistCheckState == ERM_CC_INVALID)
                {
                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Triggering the consistency check");

                    _ERM_gContext.eConsistCheckState = ERM_CC_ACTIVE;
                    (void)_ERM_sfConsistencyCheck();
                }
                //_ERM_gContext.bStartupDone      = BT_TRUE;
            }
            else
            {
                /* first check if the player is enabled*/
				/* changed on 6.4.2011 by nikolov/RT:10180*/
                BooleanType playerEnabled = BT_FALSE;
                EEPROM_LAYOUT_vGetPlayerEnabled(&playerEnabled );

                /* send next startup message */
                if ((Uint32Type)ERM_STARTUP_NUM_MESSAGES == _ERM_gContext.u32StartupCnt)
                {   /* initial startup done */
                    /* check if a SD is available and start the specific initialization */
                    if(KPB_bIsAsgAvailable() == BT_TRUE)
                    {
                        _ERM_gContext.u32StartupCnt = (Uint32Type)ERM_REQ_AUTOCONN_OFF;
                    }/* changed on 6.4.2011 by nikolov/RT:10180(if auto conn mode and MP disabled, set the right service auto conn. mode)*/
                    else if((KPB_bIsAsgAvailable() == BT_FALSE) && (playerEnabled == BT_FALSE))
                    {
                        _ERM_gContext.u32StartupCnt = (Uint32Type)ERM_REQ_AUTOCONN_ON_MP_DISABLED;
                    }
                    else
                        _ERM_gContext.u32StartupCnt = (Uint32Type)ERM_REQ_AUTOCONN_ON;
                }
                (void)_ERM_SendMessage((ERMIntMessageEnum)_ERM_gContext.u32StartupCnt,NULL);
            }
        }
        else
        {   /* process other messages and check if further message to be send */
            if ((ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM == tmp_last) && (BT_FALSE == _ERM_gContext.bReInitAudioParmeter))
            {   /* after device out volume send PEVN, this keeps Downlink volume at max. level  */
                (void)_ERM_SendMessage(ERM_REQ_SPEAKER_VOLUME_NOTIFICATION,NULL);
            }
            else if ((ERM_REQ_SPEAKER_VOLUME_NOTIFICATION == tmp_last) && (BT_FALSE == _ERM_gContext.bReInitAudioParmeter))
            {   /* after volume notification set micgain */
                (void)_ERM_SendMessage(ERM_REQ_MIC_GAIN_SET,NULL);
            }
            else if ((ERM_REQ_MIC_GAIN_SET == tmp_last) && (BT_FALSE == _ERM_gContext.bReInitAudioParmeter))
            {   /* after mic gain set ringtone setting */
                (void)_ERM_SendMessage(ERM_REQ_AMGMT_SET_RINGTONE,NULL);
            }
            else if ((ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT == tmp_last) && (BT_FALSE == _ERM_gContext.bReInitAudioParmeter))
            {   /* RT#7257 Adapt VR volume in Audio-Parameterset */
                /* Change Request: Jingle / Ringtone is dependent of VR-Volume*/
                (void)_ERM_SendMessage(ERM_REQ_AMGMT_OUT_VOLUME_SET_RING,NULL);
            }
            else
            {
                if (ERM_REQ_HDL_PAIR_REQUEST == tmp_last &&
                    AT_RSP_OK == rsp)
                {   /* update BT pin in EPROM in case it was changed via VCR */
                    Uint8Type au8BTPin[2];
                    /* trigger APP diag to write to eprom */
                    au8BTPin[0] = (_ERM_gContext.stBTPinCode[0] << 4) | _ERM_gContext.stBTPinCode[1];
                    au8BTPin[1] = (_ERM_gContext.stBTPinCode[2] << 4) | _ERM_gContext.stBTPinCode[3];
                    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Update BT PIN in Eeprom");
                    EEPROM_sfRamWriteOnly(&au8BTPin[0], EEPROM_LAYOUT_ENUM_BluetoothSetPINCode);
                }
                if (ERM_REQ_NONE != _ERM_gContext.reqERMRequest)
                {
                    (void)_ERM_SendMessage(ERM_REQ_RETRY,NULL);
                }
                else
                {
                    /*command successfully sent*/
                    /*handling for AT*PLPD request, additionally AT*PBSC should be requested too*/
                    if( ERM_REQ_GET_PAIRED_DEVICES == tmp_last )
                    {
                        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "After AT*PLPD Send AT*PBSC");
                        (void)_ERM_SendMessage(ERM_REQ_BT_SERVICE_CONN,NULL);
                    }

                }
            }

        }
    }
//    switch(_ERM_gContext.lastAtRequest)
//    {
//        case AT_REQ_GET_PAIRED_DEVICES:
//            result = _ERM_sfHandlePlpdResponse(rsp);
//            break;
//        default:
//            /* not handled */
//            break;
//    }

//    _ERM_gContext.lastAtRequest = AT_REQ_UNDEFINED;

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : The function returns the value if the securtity check        */
/*               of the bluetooth firmware update is valid                    */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsSecurityCheckOK(void)
{
    ERM_vBluetoothUpdateSecurityCheck();

    return ( _ERM_gContext.bSecurityCheckOK );
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bIsBluetoothVisible                                */
/**
    \brief      The function returns the value if the Bluetooth is in
    			visibility mode or not.

    \param      none

    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsBluetoothVisible(void)
{
    return ( _ERM_gContext.bBTVisible );
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_vsetActiveCallFlagForSettingWithoutCI                                */
/**
    \brief      The function stores the setting for active call without CI as a flag

    \param      BooleanType bActiveCall

    \return     none
   */
/*----------------------------------------------------------------------------*/
void ERM_vsetActiveCallFlagForSettingWithoutCI(BooleanType bActiveCall)
{
    _ERM_gContext.bActiveCallFlagFOrSettingWithoutCI = bActiveCall;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bgetActiveCallFlagForSettingWithoutCI                                */
/**
    \brief      The function gets the value of the flag

    \param      BooleanType bActiveCall

    \return     none
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bgetActiveCallFlagForSettingWithoutCI(void)
{
    return _ERM_gContext.bActiveCallFlagFOrSettingWithoutCI;
}
/*****************************************************************************/
//RT13185 Special handling for Error-318 for Ringtone
SuccessFailType _ERM_RINGTONE_sfHandleOkErrorRsp(ATResponseEnum  rsp,Uint16Type Errorcode )
{
    SuccessFailType result = SUCCESS;
    /* we need to store the CB, as in internal CB next message can be called
    * -> then CB is newly set!! */
    void (*cb)(APP_COMMON_enResponseCode) = _ERM_gContext.cb_atOkErrCBLast;

    if (rsp == AT_RSP_ERROR)
    {
        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ATRespone ERROR Req %d lastReq %d",_ERM_gContext.reqERMRequest,
                _ERM_gContext.lastERMRequest);
    }
    else if (rsp == AT_RSP_OK)
    {
        DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM,"ATRespone OK Req %d lastReq %d",_ERM_gContext.reqERMRequest,
                _ERM_gContext.lastERMRequest);
    }
    if( (rsp != AT_RSP_ERROR) && (rsp != AT_RSP_OK) )
    {
        return FAIL;
    }
    else
    {   /* we received OK or ERROR for send message - check which
                   was the send message */
        /* clear last send message as we received OK or ERROR */
        _ERM_gContext.lastERMRequest = ERM_REQ_NONE;
        _ERM_gContext.cb_atOkErrCBLast = NULL;
        /* check for callback */
        if (NULL != cb)
        {
            if ((AT_RSP_OK == rsp) || ((APP_COMMON_RSP_ERROR==rsp)&& ((Errorcode==ERM_EXT_ERR_AUDIO_SOURCE_NOT_ACTIVE)||(Errorcode==ERM_EXT_ERR_SOURCE_ACTIVE))))//RT13185
            {
                cb(APP_COMMON_RSP_OK);
            }
            else
            {
                cb(APP_COMMON_RSP_ERROR);
            }
        }
    }
    return result;
}

/* End Of File ERM_MessageHandler.c */
