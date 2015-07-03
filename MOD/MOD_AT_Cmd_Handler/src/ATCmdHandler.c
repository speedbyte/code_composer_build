/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler manage and built the different
                  message for the Parrot module.
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Cmd_Handler/trunk/src/ATCmdHandler.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ATCommonData.h"
#include "ATCmdHandler_CI.h"
#include "ATCmdHandler.h"
#include "ATCmdHandler_Utils.h"
#include "UART.h"
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_Utils.h"
#include "ERM_DeviceHandler.h"

#include "ATCmdHandler.id"
#include "Trace.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

Uint8Type  _ATCMD_bPbRequestIsVCR  = 0x00; /* use 0x00 for false and 0x01 for true */

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the type
*/

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
/**
\brief  description of the private variable
*/

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
/* Function    : AT_Cmd_Handler_ERM                                           */
/**
    \brief      Command handler for external resource manager

    \brief      check members of given data structure for proper processing \n
                of ERM application specific commands and build string for  \n
                UART out of received paramters in request data. \n

    \param      pointer to request from application including data
                ATReqAllType
                all value allowed
    \param      Pointer to UartTxBuffer
                const SCI_ST *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType AT_Cmd_Handler_ERM(ATReqAllType   *ATRequest,
                                   const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type       cmdLen = 0;
    Uint8Type       i = 0;
    Uint8Type       cmdBuf[MAX_LEN_AT_COMMAND_BUFFER] = {0};
    Uint8Type       u8char[AT_TEMP_STRING_MAX_LEN] = {0};
    Uint8Type       tmp_string[AT_BT_ADDRESS_LEN+1];

    if (SUCCESS == ERM_CheckUartOwnership(APP_SUB_ERM,(const SCI_ST*)UartTxBuffer))
    {
        switch (ATRequest->common.req_type)
        {   /* handling for FLASHERASE - message without data */
            case AT_REQ_FLASH_ERASE_USERDATA:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PFLASHERASE\r");
                break;
            }
            case AT_REQ_FLASH_SW_UPDATE_BT:
            {
                (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                           "AT*PBSU=%d\r",
                          ATRequest->pbsu.BTSWupdate);
                break;
            }
            
            case AT_REQ_SET_EXT_ERROR_CODES:
            {
                (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                           "AT*PECA=%d\r",
                          ATRequest->peca.action);
                break;
            }

            case AT_REQ_BT_SET_CONN_AUTH:
            {
                (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                           "AT*PSCA=%d\r",
                          ATRequest->psca.ConnectMode);
                break;
            }

                        /* handling for BT device connect */
            case AT_REQ_BT_SERVICE_CONN:
            {
                switch (ATRequest->pbsc.action)
                {
                    case AT_SERVICE_ACT_STOP:
                    case AT_SERVICE_ACT_START:
                    {   /* conditional parameter service needed - check */
                        if ( (AT_TRUE == ATRequest->pbsc.service_avail) &&
                             (AT_SERVICE_PHONE <= ATRequest->pbsc.service) &&
                             (AT_SERVICE_SPP   >= ATRequest->pbsc.service)    )
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PBSC=P%d,%d,%d\r",
                                    ATRequest->pbsc.device_id+1,
                                    ATRequest->pbsc.action,
                                    ATRequest->pbsc.service);
                        }
                        else
                        {
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ppau inco digit num %d",
                                                                                    ATRequest->ppau.pin_digits);

                            sF = FAIL;
                        }
                        break;
                    }
                    case AT_SERVICE_ACT_QUERY_SUPPORTED:
                    case AT_SERVICE_ACT_QUERY_CONNECTED:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PBSC=P%d,%d\r",
                                ATRequest->pbsc.device_id+1,
                                ATRequest->pbsc.action);
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        break;
                    }
                }
                break;
            }
            /* handling for telephone specific requests */
            case AT_REQ_HDL_PAIR_REQUEST:
            {
                switch (ATRequest->ppau.mode)
                {
                    case AT_PAIR_REFUSE_INCOMING:
                    case AT_PAIR_FORWARD_INCOMING:
                    case AT_PAIR_EXT_MODE1:
                    case AT_PAIR_EXT_MODE2:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPAU=%d\r",ATRequest->ppau.mode);
                        break;
                    }
                    case AT_PAIR_ACCEPT_INCOMING:
                    {
                        /* add given mode value */
                        if (ATRequest->ppau.pin_digits > 0            &&
                            ATRequest->ppau.pin_digits <= AT_MAX_PIN_CODE_LEN)
                        {   /* add digits */
                            for (i=0;i<ATRequest->ppau.pin_digits;i++)
                            {   /* convert number to ascii character */
                                u8char[i] = ATRequest->ppau.pin[i] + 48;
                            }
                            u8char[ATRequest->ppau.pin_digits] = '\0';
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PPAU=%d,'%s'\r",ATRequest->ppau.mode,u8char);
                        }
                        else
                        {
                            sF= FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ppau inco digit num %d", ATRequest->ppau.pin_digits);

                        }
                        break;
                    }
                    case AT_PAIR_GET_MODE:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PPAU?\r");
                        break;
                    }
                    default:
                    {
                        sF= FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "inco ppau mode %d", ATRequest->ppau.mode);

                        break;
                    }
                }
                break;
            }
            /* handling for extended call list */
            case AT_REQ_EXT_CALL_LIST:
            {
                switch (ATRequest->plccex.mode)
                {
                    case AT_EXTCALL_DISABLE:
                    case AT_EXTCALL_ENABLE:
                    case AT_EXTCALL_ENABLE_MORE:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PLCCEX=%d\r",ATRequest->plccex.mode);
                        break;
                    }
                    case AT_EXTCALL_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PLCCEX?\r");
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "inco plccex mode %d", ATRequest->plccex.mode);

                        break;
                    }
                }
                break;
            }
            /* handling for set max number of calls */
            case AT_REQ_SET_MAX_CALLS:
            {
                (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                          "AT*PSMC=%d\r",
                          ATRequest->psmc.value);
                break;
            }
            /* set audio gateway mode */			// RT 2276
            case AT_REQ_BT_AG_MODE:
            {
                (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                          "AT*PAGM=%d\r",
                          ATRequest->pagm.mode);
                break;
            }
            case AT_REQ_GET_PAIRED_DEVICES:
            {
                switch(ATRequest->plpd.mode)
                {
                    case AT_PLPD_GET_NUMBER:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PLPD=?\r");
                        break;
                    }
                    case AT_PLPD_GET_ALL:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING, "AT*PLPD\r");
                        break;
                    }
                    case AT_PLPD_GET_DEVICE:
                    {   /* check if given value is in allowed range      */
                        /* NOTE: internally device id range is 0..4 which needs
                         * to be mapped to 1..5 for Parrot!!! therefore +1 to be done here */
                        if (AT_MAX_DEVICE_ID >= ATRequest->plpd.device_id  )
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PLPD=P%d\r",ATRequest->plpd.device_id+1);
                        }
                        else
                        {
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Tel inco dev id %d", ATRequest->plpd.device_id+1);

                            sF = FAIL;
                        }
                        break;
                    }
                    default:
                    {   /* inco mode - write trace here!!! */
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Tel inco pair mode %d", ATRequest->plpd.mode);

                        sF = FAIL;
                        break;
                    }
                }
                break;
            }
            case AT_REQ_AMGMT_SRD_ACTIVE_IND:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PASA\r");
                break;
            }
            case AT_REQ_BT_AC_MODE_STATUS:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PCMS?\r");
                break;
            }
            case AT_REQ_BT_SELECT_DEV:
            {   /* check given value in range */
                if ( (AT_DEVSEL_DISCONNECT <= ATRequest->psbd.value) &&
                     (AT_DEVSEL_CONNECT    >= ATRequest->psbd.value)   )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PSBD=P%d,%d\r",ATRequest->psbd.device_id+1,
                        ATRequest->psbd.value);
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_MODEL_REV:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+CGMR\r");
                break;
            }
            case AT_REQ_MODEL_PF_CODE:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+CGMR=1\r");
                break;
            }
            case AT_REQ_MODEL_REV_EXT:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+CGMREX\r");
                break;
            }
            case AT_REQ_MANUFAC_ID:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+CGMI\r");
                break;
            }
            case AT_REQ_MODEL_ID:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+CGMM\r");
                break;
            }
            case AT_REQ_POWER_OFF:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*POFF\r");
                break;
            }
            case AT_REQ_SET_BT_NAME:
            {
                if (AT_TRUE == ATRequest->psfn.is_get_name)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PSFN?\r");
                }
                else
                {
                    /* check max len */
                    if (ATRequest->psfn.name_len <= AT_BT_DEVICE_NAME_MAX_LEN &&
                        ATRequest->psfn.name_len >= AT_BT_DEVICE_NAME_MIN_LEN    )
                    {
                        /* set the name to CK5050, first ensure /0 termination */
                        ATRequest->psfn.name[ATRequest->psfn.name_len]= '\0';
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSFN='%s'\r",ATRequest->psfn.name);
                    }
                    else
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "inval len %d (%d)", ATRequest->psfn.name_len,__LINE__);

                    }

                }
                break;
            }
            case AT_REQ_SET_DISCOVER_MODE:
            {
                switch (ATRequest->psdm.mode)
                {
                    case AT_DISCOVER_MODE_DISALLOW:
                    case AT_DISCOVER_MODE_ALLOW:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSDM=%d\r",ATRequest->psdm.mode);
                        break;
                    }
                    case AT_DISCOVER_MODE_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PSDM?\r");
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "unsupported mode %d (%d)", ATRequest->psdm.mode,__LINE__);

                        break;
                    }
                }
                break;
            }
            case AT_REQ_SET_MAX_PAIRED_DEVICES:
            {
                if  (AT_TRUE == ATRequest->psmd.is_get_number)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSMD=?\r");
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSMD=%d\r",
                                ATRequest->psmd.max_num_paired_device);

                }
                break;
            }
            case AT_REQ_PAIR_DELETE:
            {
                if (AT_TRUE == ATRequest->ppde.device_available)
                {
                    (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPDE=P%d\r", ATRequest->ppde.device_id+1);
                }
                else
                {   /* delete all devices */
                    (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPDE\r");
                }
                break;
            }
            case AT_REQ_MIC_ADC_CONF:
            {
                switch (ATRequest->palgoadc.parameter)
                {
                    case AT_ADC_MIC_ALGO_POWER:
                    case AT_ADC_MIC_ALGO_MODE:
                    {   /* range check */
                        if (ATRequest->palgoadc.value == 0x00 ||
                            ATRequest->palgoadc.value == 0x01    )
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGOADC=%d,%d\r",
                                    ATRequest->palgoadc.parameter,
                                    ATRequest->palgoadc.value);
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "unsupported value %d %d (1)",
                                                                                    ATRequest->palgoadc.parameter,
                                                                                    ATRequest->palgoadc.value);

                        }
                        break;
                    }
                    case AT_ADC_MIC_ALGO_GAIN:
                    {
                        if (ATRequest->palgoadc.value == AT_ADC_GAIN_MIN ||
                            ATRequest->palgoadc.value == AT_ADC_GAIN_MID ||
                            ATRequest->palgoadc.value == AT_ADC_GAIN_MAX )
                            {
                                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PALGOADC=%d,%d\r",
                                        ATRequest->palgoadc.parameter,
                                        ATRequest->palgoadc.value);
                            }
                            else
                            {
                                sF = FAIL;
                                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "unsupported value %d %d (2)",
                                                                                        ATRequest->palgoadc.parameter,
                                                                                        ATRequest->palgoadc.value);

                            }
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "unsupported param %d (%d)",
                                                                                ATRequest->palgoadc.parameter,__LINE__);

                        break;
                    }
                }
                break;
            }
            case AT_REQ_MIC_ALGO_SELECT:
            {
                if (ATRequest->palgoselect.algotype < AT_MIC_ALGO_GET  )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSELECT=%d\r",ATRequest->palgoselect.algotype);
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSELECT?\r");
                }
                break;
            }
            case AT_REQ_MIC_CHOICE_SET:
            {
                if (AT_MIC_ID_GET ==  ATRequest->palgosetmic.mic_id)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSETMIC?\r");
                }
                else if (AT_MIC_ID_MIN <= ATRequest->palgosetmic.mic_id &&
                         AT_MIC_ID_MAX >= ATRequest->palgosetmic.mic_id  )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSETMIC=%d\r",ATRequest->palgosetmic.mic_id);
                }
                else
                {
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "micid oor %d (%d-%d)",
                                                                            ATRequest->palgosetmic.mic_id,
                                                                            AT_MIC_ID_MIN,AT_MIC_ID_MAX,0);

                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_MIC_GAIN_SET:
            {
                if (AT_MIC_GAIN_GET ==  ATRequest->vgm.mic_gain)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT+VGM?\r");
                }
                else if (AT_MIC_GAIN_MIN <= ATRequest->vgm.mic_gain &&
                         AT_MIC_GAIN_MAX >= ATRequest->vgm.mic_gain  )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT+VGM=%d\r",ATRequest->vgm.mic_gain);
                }
                else
                {
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "micgain oor %d (%d-%d)",
                                                                            ATRequest->vgm.mic_gain,
                                                                            AT_MIC_GAIN_MIN,AT_MIC_GAIN_MAX,0);

                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_MIC_TUNE_NR:
            {
                if ( AT_NR_MAXATT_MIN <= ATRequest->palgonr.maxAttNr &&
                     AT_NR_MAXATT_MAX >= ATRequest->palgonr.maxAttNr)
                {
                    /* check for optional paramters */
                    if (AT_FALSE == ATRequest->palgonr.direction_avail)
                    {   /* no further param -> build message */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PALGONR=%d\r",ATRequest->palgonr.maxAttNr);
                    }
                    else
                    {   /* check direction */
                        if (AT_NRDIR_ALL           <= ATRequest->palgonr.direction &&
                            AT_NRDIR_CENTER_DIST10 >= ATRequest->palgonr.direction)
                        {   /* check if also input gain available */
                            if (AT_FALSE == ATRequest->palgonr.inputGain_avail)
                            {
                                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PALGONR=%d,%d\r",ATRequest->palgonr.maxAttNr,
                                        ATRequest->palgonr.direction);
                            }
                            else
                            {
                                if (AT_NR_INPUTGAIN_MIN <= ATRequest->palgonr.inputGain &&
                                    AT_NR_INPUTGAIN_MAX >= ATRequest->palgonr.inputGain)
                                {
                                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*PALGONR=%d,%d,%d\r",ATRequest->palgonr.maxAttNr,
                                            ATRequest->palgonr.direction,ATRequest->palgonr.inputGain);
                                }
                                else
                                {
                                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "NR InGain oor %d (AT_NR_INPUTGAIN_MIN-AT_NR_INPUTGAIN_MAX)",
                                                                                            ATRequest->palgonr.inputGain);

                                    sF = FAIL;
                                }
                            }

                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "NR dir oor %d (%d-%d)",
                                                                                    ATRequest->palgonr.direction,
                                                                                    AT_NRDIR_ALL,AT_NRDIR_CENTER_DIST10,0);

                            sF = FAIL;
                        }
                    }
                }
                else
                {
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "NR att oor %d (%d-%d)",
                                                                            ATRequest->palgonr.maxAttNr,
                                                                            AT_NR_MAXATT_MIN,AT_NR_MAXATT_MAX,0);

                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_MIC_EQUALIZE:
            {
                /* first check all given values for proper range */
                for (i=0;i<AT_EQU_NUM_FREQ_BAND;i++)
                {
                    if (AT_EQU_GAIN_MIN > ATRequest->palgotxeq.Gain[i] ||
                        AT_EQU_GAIN_MAX < ATRequest->palgotxeq.Gain[i]    )
                    {
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "TXEQ %d oor %d (AT_EQU_GAIN_MIN-AT_EQU_GAIN_MAX)",
                                                                                i,ATRequest->palgotxeq.Gain[i]);

                        sF = FAIL;
                        break; /* immediately finish for loop */
                    }
                }
                if (SUCCESS == sF)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOTXEQ=%d,%d,%d,%d,%d,%d,%d,%d,%d\r",
                            ATRequest->palgotxeq.Gain[0],ATRequest->palgotxeq.Gain[1],
                            ATRequest->palgotxeq.Gain[2],ATRequest->palgotxeq.Gain[3],
                            ATRequest->palgotxeq.Gain[4],ATRequest->palgotxeq.Gain[5],
                            ATRequest->palgotxeq.Gain[6],ATRequest->palgotxeq.Gain[7],
                            ATRequest->palgotxeq.Gain[8]);
                }
                break;
            }
            case AT_REQ_SPK_RX_ALGO_SELECT:
            {
                if (AT_SPEAKER_LIMIT_HP        <= ATRequest->palgoselectrx.algoRX &&
                    AT_SPEAKER_NR_EQU_COMPRESS >= ATRequest->palgoselectrx.algoRX)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSELECTRX=%d\r",ATRequest->palgoselectrx.algoRX);
                }
                else
                {
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "RX algo oor %d (%d-%d)",
                                                                            ATRequest->palgoselectrx.algoRX,
                                                                            AT_SPEAKER_LIMIT_HP,AT_SPEAKER_NR_EQU_COMPRESS,0);

                }
                break;
            }
            case AT_REQ_SPK_EQUALIZE:
            {
                /* first check all given values for proper range */
                for (i=0;i<AT_EQU_NUM_FREQ_BAND;i++)
                {
                    if (AT_EQU_GAIN_MIN > ATRequest->palgorxeq.Gain[i] ||
                        AT_EQU_GAIN_MAX < ATRequest->palgorxeq.Gain[i]    )
                    {
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "RXEQ %d oor %d (AT_EQU_GAIN_MIN-AT_EQU_GAIN_MAX)",
                                                                                i,ATRequest->palgorxeq.Gain[i]);

                        sF = FAIL;
                        break; /* immediately finish for loop */
                    }
                }
                if (SUCCESS == sF)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGORXEQ=%d,%d,%d,%d,%d,%d,%d,%d,%d\r",
                            ATRequest->palgorxeq.Gain[0],ATRequest->palgorxeq.Gain[1],
                            ATRequest->palgorxeq.Gain[2],ATRequest->palgorxeq.Gain[3],
                            ATRequest->palgorxeq.Gain[4],ATRequest->palgorxeq.Gain[5],
                            ATRequest->palgorxeq.Gain[6],ATRequest->palgorxeq.Gain[7],
                            ATRequest->palgorxeq.Gain[8]);
                }
                break;
            }
            case AT_REQ_SPK_LIM_HIGHPASS:
            {
                if ( AT_RX_LIMITLEV_MIN <= ATRequest->palgorx.limitLevel &&
                     AT_RX_LIMITLEV_MAX >= ATRequest->palgorx.limitLevel)
                {
                    /* check for optional paramters */
                    if (AT_FALSE == ATRequest->palgorx.hpFilter_avail)
                    {   /* no further param -> build message */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PALGORX=%d\r",ATRequest->palgorx.limitLevel);
                    }
                    else
                    {   /* check HP Filter */
                        if (AT_RX_HPFILTER_MIN <= ATRequest->palgorx.hpFilter &&
                            AT_RX_HPFILTER_MAX >= ATRequest->palgorx.hpFilter)
                        {   /* check if also input gain available */
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGORX=%d,%d\r",ATRequest->palgorx.limitLevel,
                                    ATRequest->palgorx.hpFilter);
                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "RX hpf oor %d (%d-%d)",
                                                                                    ATRequest->palgorx.hpFilter,
                                                                                    AT_RX_HPFILTER_MIN,AT_RX_HPFILTER_MAX,0);

                            sF = FAIL;
                        }
                    }
                }
                else
                {
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "RX limlev oor %d (%d-%d)",
                                                                            ATRequest->palgorx.limitLevel,
                                                                            AT_RX_LIMITLEV_MIN,AT_RX_LIMITLEV_MAX,0);

                    sF = FAIL;
                }
                break;
            }

            case AT_REQ_ECHO_TUNE_CANCELLATION:
            {
                if (AT_RX_AEC_GET == ATRequest->palgoaec.maxAttAec)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOAEC?\r");
                }
                else if ( AT_RX_MAXATTAEC_MIN <= ATRequest->palgoaec.maxAttAec &&
                          AT_RX_MAXATTAEC_MAX >= ATRequest->palgoaec.maxAttAec)
                   {
                       /* check for optional paramters */
                       if (AT_FALSE == ATRequest->palgoaec.switchTime_avail)
                       {   /* no further param -> build message */
                           (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*PALGOAEC=%d\r",ATRequest->palgoaec.maxAttAec);
                       }
                       else
                       {   /* check Switch Time */
                           if (AT_RX_SWITCHTIME_MIN <= ATRequest->palgoaec.switchTime &&
                               AT_RX_SWITCHTIME_MAX >= ATRequest->palgoaec.switchTime)
                           {
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                       "AT*PALGOAEC=%d,%d\r",ATRequest->palgoaec.maxAttAec,
                                       ATRequest->palgoaec.switchTime);
                           }
                           else
                           {
                                DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AEC swiTime oor %d (%d-%d)",
                                                                                       ATRequest->palgoaec.maxAttAec,
                                                                                       AT_RX_SWITCHTIME_MIN,AT_RX_SWITCHTIME_MAX,0);

                               sF = FAIL;
                           }
                       }
                   }
                   else
                   {
                        DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AEC att oor %d (%d-%d)",
                                                                               ATRequest->palgoaec.maxAttAec,
                                                                               AT_RX_MAXATTAEC_MIN,AT_RX_MAXATTAEC_MAX,0);

                       sF = FAIL;
                   }
                break;
            }

            case AT_REQ_AMGMT_OUT_VOLUME_SET:
            {
                switch (ATRequest->pnvs.type)
                {
                    case  AT_AUDIOTYPE_GET_CURRENT:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PNVS?\r");
                        break;
                    }
                    case  AT_AUDIOTYPE_GET_MINMAX:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PNVS=?\r");
                        break;
                    }
                    /* 3 types below have same valid volume range */
                    case  AT_AUDIOTYPE_COMM:
                    case  AT_AUDIOTYPE_INC_RING:
                    {
                        if ( AT_VOLUME_GEN_MIN <= ATRequest->pnvs.volume &&
                             AT_VOLUME_GEN_MAX >= ATRequest->pnvs.volume)
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PNVS=%d,%d\r",ATRequest->pnvs.type,ATRequest->pnvs.volume);
                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "VOL oor %d (%d-%d) type %d",
                                                                                    ATRequest->pnvs.volume,
                                                                                    AT_VOLUME_GEN_MIN,AT_VOLUME_GEN_MAX,ATRequest->pnvs.type);

                            sF = FAIL;
                        }
                        break;
                    }
                    /* special prompt volume range */
                    case  AT_AUDIOTYPE_PROMPT:
                    {
                        if ( AT_VOLUME_PROMPT_MIN <= ATRequest->pnvs.volume &&
                             AT_VOLUME_PROMPT_MAX >= ATRequest->pnvs.volume)
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PNVS=%d,%d\r",ATRequest->pnvs.type,ATRequest->pnvs.volume);
                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "VOL oor %d (%d-%d) prompt",
                                                                                    ATRequest->pnvs.volume,
                                                                                    AT_VOLUME_STREAM_MIN,AT_VOLUME_STREAM_MAX,0);

                            sF = FAIL;
                        }
                        break;
                    }
                    /* special streaming volume range */
                    case  AT_AUDIOTYPE_STREAMING:
                    {
                        if ( AT_VOLUME_STREAM_MIN <= ATRequest->pnvs.volume &&
                             AT_VOLUME_STREAM_MAX >= ATRequest->pnvs.volume)
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PNVS=%d,%d\r",ATRequest->pnvs.type,ATRequest->pnvs.volume);
                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "VOL oor %d (%d-%d) stream",
                                                                                    ATRequest->pnvs.volume,
                                                                                    AT_VOLUME_STREAM_MIN,AT_VOLUME_STREAM_MAX,0);

                            sF = FAIL;
                        }
                        break;
                    }
                    default:
                    {
                        DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "VOL type oor %d (%d-%d)",
                                                                                ATRequest->pnvs.type,
                                                                                AT_AUDIOTYPE_COMM,AT_AUDIOTYPE_GET_MINMAX,0);

                        sF = FAIL;
                        break;
                    }
                }
                break;
            }
            case AT_REQ_ECHO_NOISE_COMFORT:
            {
            	if (AT_TRUE == ATRequest->palognoisecomfort.is_get)
            	{
            		(void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PALGONOISECOMFORT?\r");
            	}
            	else
            	{
            		if (AT_TRUE == ATRequest->palognoisecomfort.mode_activated)
            		{
            			(void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGONOISECOMFORT=1\r");
            		}
            		else
            		{
            			(void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGONOISECOMFORT=0\r");
            		}
            	}
            	break;
            }
            case AT_REQ_ECHO_DELAY_CANCELLATION:
            {
            	if (AT_TRUE == ATRequest->palgoaecdelay.is_get)
            	{
            		(void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PALGOAECDELAY?\r");
            	}
            	else
            	{
            		if (AT_AEC_DELAY_MIN <= ATRequest->palgoaecdelay.delay &&
            		    AT_AEC_DELAY_MAX >= ATRequest->palgoaecdelay.delay	)
            		{
            			(void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOAECDELAY=%d\r",ATRequest->palgoaecdelay.delay);
            		}
            		else
            		{
            			sF = FAIL;
                        DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AEC delay oor %d (%d %d)",
                                                                                ATRequest->palgoaecdelay.delay,
                                                                                AT_AEC_DELAY_MIN,AT_AEC_DELAY_MAX,0);

                        break;
            		}
            	}
            	break;
            }
            case AT_REQ_SPK_RX_ALC:
            {
                if (AT_TRUE == ATRequest->palgoalcrx.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PALGOALCRX?\r");
                }
                else
                {
                    if (AT_TRUE == ATRequest->palgoalcrx.mode_activated)
                    {
                        if (AT_ALC_RX_MIN <= ATRequest->palgoalcrx.target &&
                            AT_ALC_RX_MAX >= ATRequest->palgoalcrx.target  )
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGOALCRX=%d,%d\r",
                                    ATRequest->palgoalcrx.mode,
                                    ATRequest->palgoalcrx.target);
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ALC target oor %d (%d %d)",
                                                                                    ATRequest->palgoalcrx.target,
                                                                                    AT_ALC_RX_MIN,AT_ALC_RX_MAX,0);

                            break;
                        }
                    }
                    else
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOALCRX=0\r");
                    }
                }
                break;
            }
            /* receive soft limiter */
            case AT_REQ_SPK_RX_SL:
            {
                if (AT_TRUE == ATRequest->palgoslrx.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PALGOSLRX?\r");
                }
                else
                {
                    if (AT_TRUE == ATRequest->palgoslrx.mode_activated)
                    {
                        if (AT_SL_RX_MIN <= ATRequest->palgoslrx.level &&\
                            AT_SL_RX_MAX >= ATRequest->palgoslrx.level  )
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGOSLRX=%d\r",
                                    ATRequest->palgoslrx.level);
                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "SL RX limiter level out of range %d (%d %d), using default %d",
                                                                                    ATRequest->palgoslrx.level,
                                                                                    AT_SL_RX_MIN,AT_SL_RX_MAX,AT_SL_RX_DEFAULT);
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGOSLRX=%d\r",
                                    AT_SL_RX_DEFAULT);
                        }
                    }
                    else
                    {
                        DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER,"SL RX limiter disabled with 0 value");
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSLRX=0\r");
                    }
                }
                break;
            }
            /* transmission soft limiter */
            case AT_REQ_SPK_TX_SL:
            {
                if (AT_TRUE == ATRequest->palgosltx.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PALGOSLPGTX?\r");
                }
                else
                {
                    if (AT_TRUE == ATRequest->palgosltx.mode_activated)
                    {
                        if (AT_SL_TX_MIN <= ATRequest->palgosltx.level &&\
                                AT_SL_TX_MAX >= ATRequest->palgosltx.level  )
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGOSLPGTX=1,%d\r",
                                    ATRequest->palgosltx.level);
                        }
                        else
                        {
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "SL TX limiter level out of range %d (%d %d), using default %d",
                                                                                    ATRequest->palgosltx.level,
                                                                                    AT_SL_TX_MIN,AT_SL_TX_MAX,AT_SL_TX_DEFAULT);
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PALGOSLPGTX=1,%d\r",
                                    AT_SL_TX_DEFAULT);
                        }
                    }
                    else
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PALGOSLPGTX=0\r");
                    }
                }
                break;
            }
            case AT_REQ_AMGMT_SET_RINGTONE:
            {
                if (AT_TRUE == ATRequest->psrt.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PSRT\r");
                }
                else
                {   /* check mode values */
                    switch (ATRequest->psrt.mode)
                    {
                        case   AT_RTMODE_DEACTIVATE:
                        case   AT_RTMODE_ACTIVATE:
                        case   AT_RTMODE_CUSTOM8:
                        case   AT_RTMODE_CUSTOM9:
                        case   AT_RTMODE_VIBORBELLZ:
                        case   AT_RTMODE_BIMBAM:
                        case   AT_RTMODE_EASYLISTEN:
                        case   AT_RTMODE_FUTUREBEAT:
                        case   AT_RTMODE_ENDLESS:
                        case   AT_RTMODE_RINGRING:
                        case   AT_RTMODE_NOSTALGIC:
                        case   AT_RTMODE_ORCHESTRA1:
                        case   AT_RTMODE_ORCHESTRA2:
                        case   AT_RTMODE_SWEETSOFT:
                        case   AT_RTMODE_TUTTUT:
                        case   AT_RTMODE_SYSTEM_STOP:
                        case   AT_RTMODE_SYSTEM_START:
                        case   AT_RTMODE_DIS_BLUETOOTH:
                        case   AT_RTMODE_APP_BLUETOOTH:
                        {
                            if (AT_FALSE == ATRequest->psrt.action_available)
                            {
                                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSRT=%d\r",ATRequest->psrt.mode);
                            }
                            else
                            {   /* check the action */
                                switch (ATRequest->psrt.action)
                                {
                                    case AT_RTACT_CHGD_DEV_ALL:
                                    case AT_RTACT_CHGD_DEV_NEW:
                                    case AT_RTACT_CHGD_ACT_ONLY:
                                    {
                                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                                "AT*PSRT=%d,%d\r",ATRequest->psrt.mode,
                                                ATRequest->psrt.action);
                                        break;
                                    }
                                    default:
                                    {
                                        sF = FAIL;
                                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "PRST inval action %d",
                                                                                                ATRequest->psrt.action);

                                        break;
                                    }
                                }

                            }
                            break;
                        }
                        case AT_RTMODE_INVALID: /* Fall through */
                        default:
                        {
                            sF = FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "PRST inval mode %d",
                                                                                    ATRequest->psrt.mode);

                            break;
                        }
                    }

                }
                break;
            }
            case AT_REQ_TEST_BT_TEST_MODE:
            {   /* set BT device in testmode */
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PBTESTMODE\r");
                break;
            }

            case AT_REQ_AMGMT_SPEAKER_VOLUME:
            {   /* keeps Downlink volume at max. level */
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PEVN=%d,%d\r", ATRequest->pevn.SpeakerVolumeNotification,
                                       ATRequest->pevn.ApplySpeakerVolume);
                break;
            }

            case AT_REQ_TEST_OUT_FREQ_CHAN_EXT:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PBFREQEX=%d,%d,%d,%d,%d,%d,%d,%d\r", ATRequest->pbfreqex.testscenario,
                                                             ATRequest->pbfreqex.packettype,
                                                             ATRequest->pbfreqex.bitpattern,
                                                             ATRequest->pbfreqex.singlefrequency,
                                                             ATRequest->pbfreqex.txburstperiod,
                                                             ATRequest->pbfreqex.scramblermode,
                                                             ATRequest->pbfreqex.powerlevel,
                                                             ATRequest->pbfreqex.packetsize);
                break;

            }
            case AT_REQ_TEST_AUDIO_LOOPBACK:
            {
                if (AT_TRUE == ATRequest->ploopback.start_loop)
                {   /* start the loop - check the mode */
                    switch (ATRequest->ploopback.mode)
                    {
                        case  AT_TEST_LOOPBACK_MIC1:
                        case  AT_TEST_BT_AUDIOLOOP:
                        case  AT_TEST_LOOPBACK_MIC2:
                        {
                            (void)snprintf((char *)cmdBuf,
                                MAX_LEN_AT_COMMAND_STRING,
                                "AT*PLOOPBACK=1,%d\r",
                                ATRequest->ploopback.mode);
                            break;
                        }
                        default:
                        {
                            sF = FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "audioloop on inval mode %d",
                                                                                    ATRequest->ploopback.mode);

                            break;
                        }
                    }
                }
                else
                {   /* stop the loop */
                    switch (ATRequest->ploopback.mode)
                    {
                        case  AT_TEST_LOOPBACK_MIC1:
                        case  AT_TEST_BT_AUDIOLOOP:
                        case  AT_TEST_LOOPBACK_MIC2:
                        {
                            (void)snprintf((char *)cmdBuf,
                                MAX_LEN_AT_COMMAND_STRING,
                                "AT*PLOOPBACK=0,%d\r",
                                ATRequest->ploopback.mode);
                            break;
                        }
                        default:
                        {
                            sF = FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "audioloop off inval mode %d",
                                                                                    ATRequest->ploopback.mode);

                            break;
                        }
                    }
                }
                break;
            }
            case AT_REQ_TEST_AUDIO_LOOP_TEST:
            {
                if (AT_AUDIO_LOOP_TEST_MODE == ATRequest->paudioloop.mode)
                {
                    if (AT_TRUE == ATRequest->paudioloop.bt_available)
                    {
                        for (i=0;i< AT_BT_ADDRESS_LEN;i++)
                        {
                            tmp_string[i] = ATRequest->paudioloop.bt_addr[i];
                        }
                        tmp_string[AT_BT_ADDRESS_LEN] = '\0';
                        if (AT_TRUE == ATRequest->paudioloop.delay_available)
                        {
                            (void)snprintf((char *)cmdBuf,
                                    MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PTEST=%d,'%s',%d,%d\r",
                                    ATRequest->paudioloop.mode,
                                    tmp_string,
                                    ATRequest->paudioloop.delay_local,
                                    ATRequest->paudioloop.delay_BT
                                    );
                            break;
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,
                                    MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PTEST=%d,'%s'\r",
                                    ATRequest->paudioloop.mode,
                                    tmp_string
                                    );
                            break;
                        }
                    }
                    else
                    {
                            (void)snprintf((char *)cmdBuf,
                                MAX_LEN_AT_COMMAND_STRING,
                                "AT*PTEST=%d\r",
                                ATRequest->paudioloop.mode
                                );
                            break;
                    }
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "audioloop test inval mode %d",
                                                                            ATRequest->paudioloop.mode);

                   break;
                }
            }
            case AT_REQ_TEST_OUT_FREQ_CHAN:
            {
                switch (ATRequest->pbfreq.mode)
                {
                    case    AT_TEST_FREQ_TX_CHAN0:
                    case    AT_TEST_FREQ_TX_CHAN39:
                    case    AT_TEST_FREQ_TX_CHAN78:
                    case    AT_TEST_FREQ_TX_DH5_CHAN0:
                    case    AT_TEST_FREQ_TX_DH5_CHAN39:
                    case    AT_TEST_FREQ_TX_DH5_CHAN78:
                    case    AT_TEST_FREQ_REC_CHAN_39:
                    case    AT_TEST_FREQ_TELEC_AFH_LOW:
                    case    AT_TEST_FREQ_TELEC_AFH_MID:
                    case    AT_TEST_FREQ_TELEC_AFH_HIGH:
                    case    AT_TEST_FREQ_PMB8753_TX_CHAN19:
                    case    AT_TEST_FREQ_PMB8753_TX_CHAN19_GFSK:
                    case    AT_TEST_FREQ_PMB8753_REC_CHAN19:
                    case    AT_TEST_FREQ_PMB8753_REC_CHAN0:
                    case    AT_TEST_FREQ_PMB8753_REC_CHAN78:
                    case    AT_TEST_FREQ_PMB8753_TX_DH1_CHAN39:

                    case    AT_TEST_FREQ_PMB8753_TX_DH3_CHAN39:
                    case    AT_TEST_FREQ_PMB8753_TX_DH1_CHAN0:
                    case    AT_TEST_FREQ_PMB8753_TX_DH3_CHAN0:
                    case    AT_TEST_FREQ_PMB8753_TX_DH1_CHAN78:
                    case    AT_TEST_FREQ_PMB8753_TX_DH3_CHAN78:
                    case    AT_TEST_FREQ_PMB8753_TX_DH1_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_DH3_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_DH5_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_2DH1_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_2DH3_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_2DH5_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_3DH1_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_3DH3_AFH:
                    case    AT_TEST_FREQ_PMB8753_TX_3DH5_AFH:
                    {
                        (void)snprintf((char *)cmdBuf,
                            MAX_LEN_AT_COMMAND_STRING,
                            "AT*PBFREQ=%d\r",
                            ATRequest->pbfreq.mode);
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pbfreq inval mode %d",
                                                                                ATRequest->pbfreq.mode);

                        break;
                    }

                }
                break;
            }
            case AT_REQ_BT_SET_AC_MODE:
            {
                if (AT_TRUE == ATRequest->pscm.is_get)
                {   /* query ck5050 for current auto connection mode */
                    (void)snprintf((char *)cmdBuf,
                            MAX_LEN_AT_COMMAND_STRING,
                            "AT*PSCM?\r");
                }
                else
                {   /* set a mode */
                    if (AT_FALSE == ATRequest->pscm.action_available)
                    {
                        if (AT_TRUE == ATRequest->pscm.mode_disable)
                        {
                            (void)snprintf((char *)cmdBuf,
                                    MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PSCM=0\r");
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,
                                    MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PSCM=1\r");
                        }
                    }
                    else
                    {
                        if (AT_TRUE == ATRequest->pscm.mode_disable)
                        {
                            if (AT_TRUE == ATRequest->pscm.action_enable_no_start)
                            {
                                (void)snprintf((char *)cmdBuf,
                                        MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PSCM=0,0\r");
                            }
                            else
                            {
                                (void)snprintf((char *)cmdBuf,
                                        MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PSCM=0,1\r");
                            }

                        }
                        else
                        {
                            if (AT_TRUE == ATRequest->pscm.action_enable_no_start)
                            {
                                (void)snprintf((char *)cmdBuf,
                                        MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PSCM=1,0\r");
                            }
                            else
                            {
                                (void)snprintf((char *)cmdBuf,
                                        MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PSCM=1,1\r");
                            }
                        }
                    }
                }
                break;
            }
            case AT_REQ_TEST_SW_RESET:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PRST\r");
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_TRC, MOD_AT_CMD_HANDLER, "TMS470 RESET the CK5050+ %d ms",
                                                                        osGetSystemCounter());

                break;
            }
            case AT_REQ_VT_AUTO_TAG:
            {
                if (AT_TRUE == ATRequest->pavp.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PAVP?\r");
                }
                else
                {
                    if (AT_TRUE == ATRequest->pavp.mode_enable)
                    {   /* enable automatic play */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PAVP=1\r");
                    }
                    else
                    {   /* disable automatic play */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PAVP=0\r");
                    }
                }
                break;
            }
            case AT_REQ_SIVR_SET_SYSTEM_PROMPT:
            {
                if (AT_TRUE == ATRequest->rpdr.mode_enable)
                {   /* enable automatic play */

                       /* RT#2094 / RT#1281
                         The Ring tone should be played out completely before the caller name is played.
                         Problem: VW ringtones have differnet length, therefore this handling
                       */
                       switch (_ERM_eGetActiveDeviceRingtone())
                       {
                           // long ringtones
                           case   AT_RTMODE_VIBORBELLZ:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=10,8,1040\r");
                               break;
                           case   AT_RTMODE_BIMBAM:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=5,5,1040\r");
                               break;
                           case   AT_RTMODE_EASYLISTEN:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=10,8,1040\r");
                               break;
                           case   AT_RTMODE_FUTUREBEAT:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=10,8,1040\r");
                               break;
                           case   AT_RTMODE_ENDLESS:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=10,8,1040\r");
                               break;
                           case   AT_RTMODE_RINGRING:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=4,3,1040\r");
                               break;
                           case   AT_RTMODE_NOSTALGIC:
                              (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                  "AT*RPDR=4,3,1040\r");
                              break;
                           case   AT_RTMODE_ORCHESTRA1:
                              (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                  "AT*RPDR=11,9,1040\r");
                              break;
                           case   AT_RTMODE_ORCHESTRA2:
                              (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                  "AT*RPDR=11,9,1040\r");
                              break;
                           case   AT_RTMODE_SWEETSOFT:
                              (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                  "AT*RPDR=10,8,1040\r");
                              break;
                           case   AT_RTMODE_TUTTUT:
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=4,3,1040\r");
                               break;
                           default:
                           {
                               (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                   "AT*RPDR=4,3,1040\r");
                               break;
                           }
                       }
                }
                else
                {   /* disable automatic play */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*RPDR=0,0,1040\r");
                }
                break;
            }
            case AT_REQ_INQUIRY:
            {
                if (AT_TRUE == ATRequest->pbdi.is_initiate )
                {   /* Initiate the inquiry. */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PBDI=1\r");
                }
                else
                {   /* Cancel the inquiry. */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PBDI=0\r");
                }
                break;
            }
            case AT_REQ_INQUIRY_EXT_RESULT:
            {
                /* activate the extended inquiry result for service SSP */
                /* ask for Phone, A2dp (sink and source), avrcp, ssp    */
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PGIREX=1,128\r");
                break;
            }
            case AT_REQ_BT_CHIP_VERSION:
                 (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PBCV?\r");
                 break;
            case AT_REQ_IMSI:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PSSN\r");
                break;
            }
            case AT_REQ_GET_INQUIRY_RESULT:
            {
                if (AT_TRUE == ATRequest->pgir.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PGIR?\r");
                }
                else
                {   /* check that startindex is less equal stopindex and
                       start is > 0 */
                    if (0 < ATRequest->pgir.startIndex &&
                        ATRequest->pgir.startIndex <= ATRequest->pgir.endIndex)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PGIR=%d,%d\r",
                                ATRequest->pgir.startIndex,
                                ATRequest->pgir.endIndex);
                    }
                    else
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pgir idx inval %d %d",
                                                                                ATRequest->pgir.startIndex,
                                                                                ATRequest->pgir.endIndex);
                    }
                }
                break;
            }
            case AT_REQ_IN_PAIR_REQUEST:
            {
                Uint8Type device_char = 0;
                switch (ATRequest->pprq.device_type)
                {
                    case   AT_INQUIRY_DEVICE:
                    {
                        device_char = 'I';
                        break;
                    }
                    case   AT_DELETED_DEVICE:
                    {
                        device_char = 'P';
                        break;
                    }
                    case   AT_INITIATED_DEVICE:
                    {
                        device_char = 'R';
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pprq inco device type %d",
                                                                                ATRequest->pprq.device_type);

                        break;
                    }
                }
                if (SUCCESS == sF)
                {   /* go on */
#ifdef UHV_SHOW_UP_TO30_DEVICES
                if (AT_MAX_DEVICES_AFTER_SEARCH >= ATRequest->pprq.device_id  )
#else
                if (AT_MAX_PARIRING_DEVICE_ID >= ATRequest->pprq.device_id  )
#endif                
                    {
                        switch (ATRequest->pprq.pairingCmd)
                        {
                            case  AT_PAIR_CMD_INITIALTE:
                            case  AT_PAIR_CMD_ACCEPT:
                            case  AT_PAIR_CMD_REJECT:
                            {   /* valid type - check if optional BT pin */
                                if (AT_FALSE == ATRequest->pprq.pin_avail)
                                {
                                    /* ticket 12177: initiate device index starts with 0 */
                                    if ( AT_INITIATED_DEVICE != ATRequest->pprq.device_type)
                                    {
                                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                                "AT*PPRQ=%c%d,%d\r",
                                                device_char,
                                                ATRequest->pprq.device_id+1,
                                                ATRequest->pprq.pairingCmd);
                                    }
                                    else
                                    {
                                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                                "AT*PPRQ=%c%d,%d\r",
                                                device_char,
                                                ATRequest->pprq.device_id,
                                                ATRequest->pprq.pairingCmd);
                                    }
                                }
                                else
                                {
                                    if (ATRequest->pprq.pin_digits > 0            &&
                                            ATRequest->pprq.pin_digits <= AT_MAX_PIN_CODE_LEN)
                                    {   /* add digits */
                                        for (i=0;i<ATRequest->pprq.pin_digits;i++)
                                        {   /* convert number to ascii character is not needed */
                                            u8char[i] = ATRequest->pprq.pin[i];
                                        }
                                        /* ticket 12177: initiate device index starts with 0 */
                                        if ( AT_INITIATED_DEVICE != ATRequest->pprq.device_type)
                                        {
                                            u8char[ATRequest->pprq.pin_digits] = '\0';
                                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                                    "AT*PPRQ=%c%d,%d,'%s'\r",
                                                    device_char,
                                                    ATRequest->pprq.device_id+1,
                                                    ATRequest->pprq.pairingCmd,
                                                    u8char);
                                        }
                                        else
                                        {
                                            u8char[ATRequest->pprq.pin_digits] = '\0';
                                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                                    "AT*PPRQ=%c%d,%d,'%s'\r",
                                                    device_char,
                                                    ATRequest->pprq.device_id,
                                                    ATRequest->pprq.pairingCmd,
                                                    u8char);
                                        }
                                    }
                                    else
                                    {
                                        sF= FAIL;
                                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pprq inco digit num %d",
                                                                                                ATRequest->pprq.pin_digits);

                                    }
                                }
                                break;
                            }
                            default:
                            {
                                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "pprq inco cmd %d",
                                                                                        ATRequest->pprq.pairingCmd);

                                sF = FAIL;
                                break;
                            }
                        }
                    }
                    else
                    {
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "pprq inco dev id %d",
                                                                                ATRequest->pprq.device_id+1);

                        sF = FAIL;
                    }
                }
                break;
            }
            case AT_REQ_SSP_SETUP:
                /* determine if we send the request to get the current setup
                 * or want to set up a defined configuration.
                 */

                if ( AT_SSP_REQUEST == ATRequest->pssp.mode){
                    /* send AT*PSSP? */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PSSP?\r");
                    DEBUG_TEXT(AT_CMD_HANDLER_SWI_TRC, MOD_AT_CMD_HANDLER, "PSSP request send in AT_CMD_Handler ");
                }
                else {
                    if (AT_SSP_ENABLE == ATRequest->pssp.mode)
                    {/* send AT*PSSP=x,x */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSSP=%d,%d\r",
                                ATRequest->pssp.mode,
                                ATRequest->pssp.IOCap);
                        DEBUG_TEXT(AT_CMD_HANDLER_SWI_TRC, MOD_AT_CMD_HANDLER, "PSSP config send in AT_CMD_Handler ");
                    }
                    else {/* send AT*PSSP=x */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PSSP=%d\r",
                                ATRequest->pssp.mode);
                        DEBUG_TEXT(AT_CMD_HANDLER_SWI_TRC, MOD_AT_CMD_HANDLER, "PSSP config send in AT_CMD_Handler ");
                    }
                }
                break;
            case AT_REQ_BT_SERVICE_MODE:
            {
                if (AT_TRUE == ATRequest->pbsm.is_get)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PBSM?\r");
                }
                else
                {
                    if (AT_TRUE == ATRequest->pbsm.aHFPActivated)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PBSM=1\r");
                    }
                    else
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PBSM=0\r");
                    }
                }
                break;
            }
            case AT_REQ_BT_SERVICE_AC:
            {
                if(AT_BT_AUTO_CONN_MODE_BITMASK_OF_DEVS == ATRequest->pbsa.mode)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PBSA=%d,%d,%d\r",
                        ATRequest->pbsa.service,
                        ATRequest->pbsa.mode,
                        ATRequest->pbsa.bitmask);
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PBSA=%d,%d\r",
                        ATRequest->pbsa.service,
                        ATRequest->pbsa.mode);
                }
                break;
            }
            case AT_REQ_ALTERNATING_SIGNAL:
            {
                Uint8Type tmpLeft  = 0;
                Uint8Type tmpRight = 0;

                if ( AT_TRUE == ATRequest->agsw.chanLeftOn )
                {
                    tmpLeft = 1;
                }
                else
                {
                    tmpLeft = 0;
                }

                if ( AT_TRUE == ATRequest->agsw.chanRightOn )
                {
                    tmpRight = 1;
                }
                else
                {
                    tmpRight = 0;
                }

                if (    ( AT_ALTERNATING_FREQ_MIN > ATRequest->agsw.frequencyLeft )
                     || ( AT_ALTERNATING_FREQ_MAX < ATRequest->agsw.frequencyLeft )
                     || ( AT_ALTERNATING_FREQ_MIN > ATRequest->agsw.frequencyRight )
                     || ( AT_ALTERNATING_FREQ_MAX < ATRequest->agsw.frequencyRight )

                     || ( AT_ALTERNATING_LEVEL_MIN > ATRequest->agsw.outputLevelLeft )
                     || ( AT_ALTERNATING_LEVEL_MAX < ATRequest->agsw.outputLevelLeft )
                     || ( AT_ALTERNATING_LEVEL_MIN > ATRequest->agsw.outputLevelRight )
                     || ( AT_ALTERNATING_LEVEL_MAX < ATRequest->agsw.outputLevelRight )  )
                {
                    sF = FAIL;
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "agsw %d invalid param : frequencyLeft = %d, frequencyRight = %d",
                                                                            ATRequest->common.req_type,
                                                                            ATRequest->agsw.frequencyLeft,
                                                                            ATRequest->agsw.frequencyRight,0);
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "agsw %d invalid param : outputLevelLeft = %d, outputLevelRight = %d",
                                                                            ATRequest->common.req_type,
                                                                            ATRequest->agsw.outputLevelLeft,
                                                                            ATRequest->agsw.outputLevelRight,0);
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*AGSW=%d,%d,%d,%d,%d,%d\r",
                        ATRequest->agsw.frequencyLeft,
                        ATRequest->agsw.frequencyRight,
                        tmpLeft,
                        tmpRight,
                        ATRequest->agsw.outputLevelLeft,
                        ATRequest->agsw.outputLevelRight);
                }
                break;
            }
            case AT_REQ_PLAY_RINGTONE:
            {
                switch(ATRequest->pprt.action)
                {
                    case AT_PPRT_ACT_PLAY_ONCE:
                    case AT_PPRT_ACT_PLAY:
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PPRT=%d,%d\r",
                                        ATRequest->pprt.action,
                                        ATRequest->pprt.ringtoneId);
                        break;
                    case AT_PPRT_ACT_STOP:
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PPRT=%d\r",
                                        ATRequest->pprt.action);
                        break;
                    default:
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pprt: unsup act %d",
                                                                                ATRequest->pprt.action);

                        sF = FAIL;
                        break;
                }
                break;
            }
            case AT_REQ_AMGMT_AUDIO_LOCK:
            {   /* check Lock is in given range */
                if ( (AT_ALCK_LOCK_STOP <= ATRequest->alck.locked) &&
                     (AT_ALCK_LOCK_START >= ATRequest->alck.locked) )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                     "AT*ALCK=1,%d\r",ATRequest->alck.locked);
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_INBOUND_RINGTONE:
            {
                switch(ATRequest->pirt.action)
                {
                    case AT_PIRT_ACT_DISABLE:
                    case AT_PIRT_ACT_ENABLE:
                        (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PIRT=%d\r",
                                        ATRequest->pirt.action);
                        break;
                    default:
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pirt: unsup act %d",
                                                                                ATRequest->pprt.action);

                        sF = FAIL;
                        break;
                }
                break;
            }
            case AT_REQ_PB_OBJECT_PUSH_MODE:
            {
                switch (ATRequest->popm.mode)
                {
                    case AT_POPM_OOP_ACTIVE_DEVICE_ONLY:
                    case AT_POPM_OOP_ANY_PAIRED_DEVICE:
                    case AT_POPM_OOP_OTHER_PAIRED_DEVICE:
                    case AT_POPM_OOP_REJECT_ALL_TRANSFER:
                        (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*POPM=%d\r",
                                        ATRequest->popm.mode);
                        break;
                    default:
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "popm: unsup mode %d",
                                                                                ATRequest->popm.mode);

                        sF = FAIL;
                        break;
                }
                break;
            }
            case AT_REQ_DTMF_PLAYING:
            {
                switch(ATRequest->pcdd.state)
                {
                    case AT_DTMF_AFTER_DIALING_DISABLE:
                    case AT_DTMF_AFTER_DIALING_ENABLE:
                        (void)snprintf( (char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PCDD=%d\r",
                                        ATRequest->pcdd.state);
                        break;
                    default:
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "pccd: unsup new state %d",
                                                                                ATRequest->pcdd.state);

                        sF = FAIL;
                        break;
                }
                break;
            }
            case AT_REQ_PB_STACK_CHANGE:
            {
                /* check for the mode */
                switch (ATRequest->ppbcex.mode)
                {
                    case AT_PPBCEX_DISABLE:
                    case AT_PPBCEX_ENABLE:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPBCEX=%d\r",
                                ATRequest->ppbcex.mode);
                        break;
                    }
                    case AT_PPBCEX_GET:
                    default:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPBCEX?\r");
                        break;
                    }
                }
                break;
            }
            case AT_REQ_BT_SUP_FEAT:
            {
                /* AT*PBSF=<Id>,<Feature> */
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PBSF=I%d,%d\r",
                        ATRequest->pbsf.device_id,ATRequest->pbsf.feature);
                break;
            }
            default:
                sF = FAIL;
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "unsupported type %d",
                                                                        ATRequest->common.req_type);

                break;
        }
        /* check if command is successful build together, if so call UART */
        if (SUCCESS == sF)
        {
            cmdLen = (Uint8Type)strlen((char *)cmdBuf);
            if (UART_u16WriteBlock((SCI_ST*)UartTxBuffer, cmdBuf, cmdLen) != cmdLen)
            {
                sF = FAIL;
                _AT_sFCmdReInitialiseUart((SCI_ST*)UartTxBuffer);
                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "write err for msg %d len %d",
                                                                        ATRequest->common.req_type,cmdLen);

            }
        }
        else
        {
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "build err for msg %d",
                                                                    ATRequest->common.req_type);

        }
    }
    else
    {
                DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "res blocked");

    }
    return (sF);
}

/* End Of File ATCmdHandler.c */
