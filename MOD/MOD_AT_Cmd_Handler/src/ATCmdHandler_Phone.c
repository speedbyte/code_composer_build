/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_Phone.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Phone part
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler manage and built the different
                  message for the Parrot module.
*-----------------------------------------------------------------------------
* $Date: 2008-09-02 12:16:26 +0200 (Di, 02 Sep 2008) $
* $Rev: 1180 $
* $Author: schroeder $
* $URL: http://subversion-01.s1nn.int/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Cmd_Handler/trunk/src/ATCmdHandler.c $
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
#include "ATCmdHandler_CI.h"
#include "ATCmdHandler.h"
#include "ATCmdHandler_Utils.h"
#include "UART.h"
#include "ERM.h"

#include "ATCmdHandler_Phone.id"
#include "Trace.h"
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
/* Function    : AT_Cmd_Handler_Tel                                           */
/**
    \brief      Command handler for TEL application

    \brief      check members of given data structure for proper processing \n
                of tel application specific commands and build string for  \n
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

SuccessFailType AT_Cmd_Handler_Tel(ATReqAllType   *ATRequest,
                                   const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type       cmdLen = 0;
    Uint8Type       cmdBuf[MAX_LEN_AT_COMMAND_BUFFER] = {0};
    Uint8Type       u8char[AT_TEMP_STRING_MAX_LEN] = {0};

    if (SUCCESS == ERM_CheckUartOwnership(APP_SUB_TEL,(const SCI_ST*)UartTxBuffer))
    {
        switch (ATRequest->common.req_type)
        {
            /* handling for telephone specific requests */
            case AT_REQ_CALL_PHONE_NUBER:
            {
                /* first check if given data matches the requirements */
                /* NOTE: all check functions to be implemented in specific files */
                if (SUCCESS == _AT_CheckPhoneNumer(ATRequest->callNumber))
                {
                    (void)memcpy (u8char,ATRequest->callNumber.number,ATRequest->callNumber.len);
                    u8char[ATRequest->callNumber.len] = '\0';
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "ATD%s;\r",u8char);
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            
            case AT_REQ_CALL_PB_INDEX:
            {
                if( AT_FALSE == ATRequest->atdi.pnIndex_avail)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "ATDI%d\r",ATRequest->atdi.pbIndex);
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "ATDI%d,%d\r",ATRequest->atdi.pbIndex,
                                              ATRequest->atdi.pnIndex);
                }
                break;
            }

            case AT_REQ_CALL_PB_NUMBER_TYPE:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "ATDP%d,%d\r",ATRequest->atdp.pbIndex,
                                              ATRequest->atdp.numberType);
                break;
            }            

            case AT_REQ_CALL_LAST_NUMBER:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+BLDN\r");
                break;
            }

            case AT_REQ_PICK_UP_CALL:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"ATA\r");
                break;
            }

            case AT_REQ_CALL_HANG_UP:
            {
                switch (ATRequest->chup.mode)
                {
                    case AT_HANGUP_CALL_0:
                    case AT_HANGUP_CALL_1:
                    case AT_HANGUP_CALL_2:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT+CHUP=%d\r",ATRequest->chup.mode);
                        break;
                    }
                    case AT_HANGUP_NO_INDEX:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT+CHUP\r");
                        break;
                    }
                    default:
                    {
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco hangup mode %d",
                                                                                     ATRequest->chup.mode);
                        sF = FAIL;
                    }
                }
                break;
            }

            case AT_REQ_CALL_STATUS_IND_INDEX:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PLCC\r");
                break;
            }

            case AT_REQ_PHONE_STATUS_IND:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PIND=?\r");
                break;
            }

            case AT_REQ_PHONE_STATUS_IND_VAL:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PIND?\r");
                break;
            }

            case AT_REQ_SWITCH_CALLS:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PSWC\r");
                break;
            }

            case AT_REQ_DTMF:
            {
                if (SUCCESS == _AT_CheckDTMFValue(ATRequest->dtmf.value))
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT+VTS=%c\r",ATRequest->dtmf.value);
                }
                else
                {   /* upsupported DTMF character */
                    sF = FAIL;
                }
                break;
            }

            case AT_REQ_MULTI_PARTY_CALL:
            {
                if( AT_FALSE == ATRequest->pmpc.idx_available )
                {
                    (void)snprintf((char*)cmdBuf, MAX_LEN_AT_COMMAND_STRING,
                             "AT*PMPC=%d\r", ATRequest->pmpc.action );
                }
                else
                {
                    (void)snprintf((char*)cmdBuf, MAX_LEN_AT_COMMAND_STRING,
                             "AT*PMPC=%d,%d\r", ATRequest->pmpc.action, 
                                                ATRequest->pmpc.index);
                }
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
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco dev id %d",
                                                                                     ATRequest->plpd.device_id+1);
                            sF = FAIL;
                        }
                        break;
                    }
                    default:
                    {   /* inco mode - write trace here!!! */
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco pair mode %d\r\n",
                                                                                 ATRequest->plpd.mode);
                        sF = FAIL;
                        break;
                    }
                }
                break;
            }

            case AT_REQ_AMGMT_MIC_MUTE:
            {
                switch (ATRequest->micMute.mode)
                {
                    case AT_MIC_MUTE:
                    case AT_MIC_UNMUTE:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PMMU=%d\r",ATRequest->micMute.mode);
                        break;
                    }
                    case AT_MIC_STATUS_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PMMU?\r");
                        break;
                    }
                    default:
                    {   /* inco mute mode - raise trace and set to fail */
                        sF= FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco mute mode %d",
                                                                                ATRequest->micMute.mode);
                        break;
                    }

                }
                break;
            }

            case AT_REQ_AUDIO_TRANS_DISCRETE:
            {
                switch (ATRequest->patr.mode)
                {
                    case AT_AUDIO_DISCRETE_MODE:
                    case AT_AUDIO_HANDSFREE_MODE:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PATR=%d\r",ATRequest->patr.mode);
                        break;
                    }
                    case AT_AUDIO_MODE_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PATR?\r");
                        break;
                    }
                    default:
                    {   /* inco mute mode - raise trace and set to fail */
                        sF= FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco audio transfer %d",
                                                                                ATRequest->patr.mode);
                        break;
                    }
                }
                break;
            }
            case AT_REQ_BT_AG_MODE:
            {
                switch (ATRequest->pagm.mode)
                {
                    case AT_AUDIO_GW_MODE_OFF:
                    case AT_AUDIO_GW_MODE_ON:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PAGM=%d\r",ATRequest->pagm.mode);
                        break;
                    }
                    case AT_AUDIO_GW_MODE_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PAGM?\r");
                        break;
                    }
                    default:
                    {   /* inco mute mode - raise trace and set to fail */
                        sF= FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco audio gateway %d",
                                                                                ATRequest->pagm.mode);
                        break;
                    }
                }
                break;
            }
            case AT_REQ_BT_AG_TRANSFER:
            {
                switch (ATRequest->pagt.mode)
                {
                    case AT_AUDIO_GW_TRANSFER_MODE_OFF:
                    case AT_AUDIO_GW_TRANSFER_MODE_ON:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PAGT=%d\r",ATRequest->pagt.mode);
                        break;
                    }
                    case AT_AUDIO_GW_TRANSFER_MODE_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PAGT?\r");
                        break;
                    }
                    default:
                    {   /* inco mute mode - raise trace and set to fail */
                        sF= FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel inco audio gateway %d",
                                                                                ATRequest->pagt.mode);
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
            case AT_REQ_CLASS_DEVICE_REQUEST:
            {
                if (AT_MAX_DEVICE_ID >= ATRequest->pbcd.device_id )
                {   /* send command with device id + 1 as host use
                       numbering 0.. max and ck5050 1..max+1 */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                      "AT*PBCD=P%d\r",
                      ATRequest->pbcd.device_id +1);
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "_ERM deviceid oor %d (0..%d)",
                                                                            ATRequest->pbcd.device_id,
                                                                            AT_MAX_DEVICE_ID);
                }
                
                break;
            }
            default:
                sF = FAIL;
                /* add trace here - unsupported enum value  */
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel unsupported msg %d",
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
                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlTel write err for msg %d len %d",
                                                                        ATRequest->common.req_type,cmdLen);
            }
        }
        else
        {
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlTel build err for msg %d",
                                                                   ATRequest->common.req_type);
        }
    }
    else
    {
        /* add trace that given pointer is NULL */
        DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlTel res blocked");
    }
    return (sF);
}


/* End Of File ATCmdHandler_Phone.c */
