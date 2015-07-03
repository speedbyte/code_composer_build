/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_Sms.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Sms part
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

#include "ATCmdHandler_Sms.id"
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
/* Function    : AT_Cmd_Handler_Sms                                     */
/**
    \brief      Command handler for sms

    \brief      check members of given data structure for proper processing \n
                of sms application specific commands and build string for\n
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
SuccessFailType AT_Cmd_Handler_Sms(ATReqAllType   *ATRequest,
                                         const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type       cmdLen = 0;
    Uint8Type       cmdBuf[MAX_LEN_AT_COMMAND_BUFFER] = {0};

    if (SUCCESS == ERM_CheckUartOwnership(APP_SUB_SMS,(const SCI_ST*)UartTxBuffer))
    {
        switch (ATRequest->common.req_type)
        {
            /* handling for sms specific requests */
            case AT_REQ_SMS_READING_GET:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PMRD?\r");
                break;
            }
            case AT_REQ_SMS_READING:
            {
                /* check if type and index are in valid range */
                if (AT_SMS_TYPE_UNREAD <= ATRequest->pmrd.type &&
                    AT_SMS_TYPE_SEND   >= ATRequest->pmrd.type &&
                    AT_MAX_NUM_SMS_FOLDER_SIZE >= ATRequest->pmrd.index)
                {
                    /* check if maxlen not given */
                    if (AT_FALSE == ATRequest->pmrd.maxlen_avail)
                    {   /* only use type and index */
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PMRD=%d,%d\r",ATRequest->pmrd.type,ATRequest->pmrd.index);
                    }
                    else
                    {   /* maxlen available, check if also offset given */
                        if (AT_FALSE == ATRequest->pmrd.offset_avail)
                        {   /* only use type and index */
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PMRD=%d,%d,%d\r",ATRequest->pmrd.type,
                                ATRequest->pmrd.index,ATRequest->pmrd.maxlen);
                        }
                        else
                        {   /* all 4 parameters present */
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*PMRD=%d,%d,%d,%d\r",ATRequest->pmrd.type,
                                    ATRequest->pmrd.index,ATRequest->pmrd.maxlen,
                                    ATRequest->pmrd.offset);
                        }
                    }
                    /* NOTE: the case that maxlen in not added and offset is is
                     * an invalid case and therefore not taken into account */
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Pb oor %d %d",
                                                                             ATRequest->pmrd.type,
                                                                             ATRequest->pmrd.index);
                }
                break;
            }
            case AT_REQ_SMS_DELETE:
            {
                /* check if type and index are in valid range */
                if (AT_SMS_TYPE_UNREAD <= ATRequest->pmde.type &&
                    AT_SMS_TYPE_SEND   >= ATRequest->pmde.type &&
                    AT_MAX_NUM_SMS_FOLDER_SIZE >= ATRequest->pmde.index)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PMDE=%d,%d\r",ATRequest->pmde.type,ATRequest->pmde.index);
                }
                break;
            }
            case AT_REQ_SMS_SEND:
            {   /* TODO - check if char set relation and conversion needed!!! */
                /* check given lengths */
                if (AT_MAX_SMS_LEN_TOTAL >= ATRequest->pmsd.message_len &&
                    AT_SMS_PHONENUM_MAX_LEN >=  ATRequest->pmsd.phone_number_len   )
                {   /* ensure \0 termination */
                    ATRequest->pmsd.phone_number[ATRequest->pmsd.phone_number_len] = '\0';
                    ATRequest->pmsd.message[ATRequest->pmsd.message_len] = '\0';
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PMSD='%s',%s\r",
                            (char*)(ATRequest->pmsd.phone_number), (char*)(ATRequest->pmsd.message));
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Pb pmsd oor %d %d",
                                                                             ATRequest->pmsd.message_len,
                                                                             ATRequest->pmsd.phone_number_len);
                }
                break;
            }
            case AT_REQ_SMS_CONC_SEND:
            {
                /* check the mode */
                switch (ATRequest->pmsdex.mode)
                {
                    case  AT_SMSCONC_SET_DESTINATION:
                    {
                        if (AT_SMS_PHONENUM_MAX_LEN >= ATRequest->pmsdex.phone_number_len)
                        {   /* ensure \0 termination */
                            ATRequest->pmsdex.phone_number[ATRequest->pmsdex.phone_number_len] = '\0';
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PMSDEX=%d,%s\r",
                                    ATRequest->pmsdex.mode, (char*)(ATRequest->pmsdex.phone_number));
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Pb pmsdex oor %d",
                                                                                     ATRequest->pmsd.phone_number_len);
                        }
                            
                        break;
                    }
                    case  AT_SMSCONC_SEND:
                    {
                         if (AT_MAX_SMS_LEN_TOTAL >= ATRequest->pmsdex.message_len)
                        {   /* ensure \0 termination */
                            ATRequest->pmsdex.message[ATRequest->pmsdex.message_len] = '\0';
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PMSDEX=%d,%s\r",
                            ATRequest->pmsdex.mode, (char*)(ATRequest->pmsdex.message));
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Pb pmsdex oor %d",
                                                                                     ATRequest->pmsdex.message_len);
                        }
                        break;
                    }
                    case  AT_SMSCONC_SEND_END:
                    {
                        (void)snprintf((char *)cmdBuf,
                            MAX_LEN_AT_COMMAND_STRING,
                            "AT*PMSDEX=%d\r",
                            ATRequest->pmsdex.mode);
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Pb pmsdex mode oor %d",
                                                                                 ATRequest->pmsdex.mode);
                        break;
                    }
                }
                break;
            }
            case AT_REQ_SMS_CHARSET:
            {
                switch( ATRequest->pmcs.charset )
                {        
                    case AT_SMS_CHARSET_UTF8:
                    case AT_SMS_CHARSET_ISO:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PMCS=%d\r",ATRequest->pmcs.charset);
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Pb invalid charset %d",
                                                                                 ATRequest->pmcs.charset);
                        break;
                    }
                }
            }
            break;

            case AT_REQ_SMS_LIST:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PMLS\r");
                break;
            }
            
            default:
                sF = FAIL;
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Sms unsupported type %d",
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
                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Sms write err for msg %d len %d",
                                                                         ATRequest->common.req_type,cmdLen);
            }
            else
            {
                _ATCMD_bPbRequestIsVCR = 0x00;
            }
        }
        else
        {
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Sms build err for msg %d",
                                                                     ATRequest->common.req_type);
        }
    }
    else
    {
        DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_Handler_Sms res blocked");
    }
    return (sF);
}

/* End Of File ATCmdHandler_Sms.c */
