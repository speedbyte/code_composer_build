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
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/src/ATParser.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "ATCmdHandler.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ATParser_CI.h"
#include "ATParser.h"
#include "ATParser_Utils.h"
#include "UART.h"
#include "POOL.h"

#include "ATParser.id"
#include "TRACE.h"


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

typedef enum {
    AT_PARSER_NO_ERROR,
    AT_PARSER_ERR_MEMORY,
    AT_PARSER_ERR_UNSUPPORTED,
    AT_PARSER_ERR_EXTRACT,
    AT_PARSER_ERR_SEND
}ATParserErrorEnum;

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/
static void             ATParserRxMsg(void);
static void             ATParserRxMsgInit(void);
static SuccessFailType  ATParserMsg(void);

static SuccessFailType  _ATParser_SendMessage(ATRspMessageType * p_msg);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/
/* FIX for error built */
#define RX_STATUS_INIT      0x00
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private variable
*/
static ParserRxMsgStateType parserRxMsgState = PAT_RX_MSG_STATE_WAIT_FOR_PRE_CR;

/* Depending upon the char set, maximum SMS message size ia 160 or 320 byes */
/* plus protocol overhead. Adjust size of buffers for SMS                   */
Uint8Type  parserRxMsgBuffer[SIZEOF_PAT_RX_MSG_BUFFER];
Uint16Type parserRxMsgCount = 0;


/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/****************************************************************************/

void ATParserTask(void)
{

    /* NOTE: the AT parser does not need state handling, all states are
       covered within ERM module                                            */
    ATParserRxMsg();

}

#define AT_MAX_IDLE_COUNTER 150
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*                         Parrot Receive Message                           */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/*  Method:     ATParseRxMsg                                                */
/****************************************************************************/
static void ATParserRxMsg(void)
{
    static Uint8Type idle_counter   = 0;
    Uint8Type       pleaseContinue  = TRUE;
    SuccessFailType sF              = SUCCESS;
    Uint8Type       dasByte;

    /* increment idle counter */
    idle_counter++;
    /*----------------------------------------------------------------------*/
    /* If available read, parse and process one message                     */
    /*----------------------------------------------------------------------*/
    while (pleaseContinue)
    {
        /* Is there a byte available from the data link?                    */
        if (UART_u8RxGetCount(UART2) != 0)
        {
            /* Yes, so get it                                               */
            dasByte = UART_u8ReadByte(UART2);

            if(dasByte == '\0')
            {
                DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "No '\\0' received");

                UART_PrintStatistic(UART2);
            }

            /* reset the idle counter */
            idle_counter = 0;
            switch(parserRxMsgState)
            {
                case PAT_RX_MSG_STATE_WAIT_FOR_PRE_CR:
                    if (ASCII_CR == dasByte)
                    {
                        parserRxMsgState = PAT_RX_MSG_STATE_WAIT_FOR_PRE_LF;
                        //parserRxMsgTimer = TimerGetElapsedTime() + TIMER_1S;
                    }
                    break;
                case PAT_RX_MSG_STATE_WAIT_FOR_PRE_LF:
                    if (ASCII_LF == dasByte)
                    {
                        parserRxMsgState = PAT_RX_MSG_STATE_WAIT_FOR_MSG;
                    }
                    else
                    {
                        ATParserRxMsgInit();
                    }
                    break;
                case PAT_RX_MSG_STATE_WAIT_FOR_MSG:
                    if (ASCII_CR != dasByte)
                    {
                        parserRxMsgBuffer[parserRxMsgCount] = dasByte;
                        parserRxMsgCount = parserRxMsgCount + 1;
                        /* Make sure we did not overflow                    */
                        if (parserRxMsgCount >= SIZEOF_PAT_RX_MSG_BUFFER)
                        {
                            /* Notify buffer overflow...                    */
                            /* ...and setup for assembling next message     */
                            ATParserRxMsgInit();
                        }
                    }
                    else
                    {   /* we got CR after message - now we shall wait for LF */
                        if (0 != parserRxMsgCount)
                        {
                            parserRxMsgState = PAT_RX_MSG_STATE_WAIT_FOR_POST_LF;
                        }
                        else
                        {
                            ATParserRxMsgInit();
                        }
                    }
                    break;
                case PAT_RX_MSG_STATE_WAIT_FOR_POST_LF:
                    if (ASCII_LF == dasByte)
                    {
                        /* Insert NUL character to terminate string                 */
                        parserRxMsgBuffer[parserRxMsgCount] = 0;
                        /* Process the message                                      */
                        sF = ATParserMsg();
                        /* Setup for assembling next message goes also back to init state */
                        ATParserRxMsgInit();
                    }
                    else
                    {   /* add trace here - expected LF after CR!! */
                        DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "expected LF after CR!!");

                    }
                    break;
            }
        } /* end if (UartGetRxParserCount() != 0)                              */
        else
        {
            pleaseContinue = FALSE;
        }
    } /* end while (pleaseContinue)                                         */
    if (AT_MAX_IDLE_COUNTER == idle_counter)
    {
        /* check in which state we are - if we wait for first message characters
         * this is a normal behaviour, but in all other states a strange
         * issue happened as we have not received a character for AT_MAX_IDLE_COUNTER
         * calls of ATParserRxMsg -> clear the current message buffer!! */
        if (PAT_RX_MSG_STATE_WAIT_FOR_PRE_CR != parserRxMsgState)
        {
            DEBUG_VALUE2(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Idle Counter %d in ATParserRxMsgState %d -> init buffer",
                    idle_counter,parserRxMsgState);

            ATParserRxMsgInit();
        }
        /* reset the idle counter */
        idle_counter = 0;
    }

    (void)sF;
}

/****************************************************************************/
/*  Method:     ParserReceiveMessageInit                                       */
/****************************************************************************/
static void ATParserRxMsgInit(void)
{
    (void)memset(parserRxMsgBuffer,0x00,SIZEOF_PAT_RX_MSG_BUFFER);
    parserRxMsgCount = 0;
    parserRxMsgState = PAT_RX_MSG_STATE_WAIT_FOR_PRE_CR;
}


/****************************************************************************/
/*  Method:     ATParseMsg                                                 */
/****************************************************************************/
static SuccessFailType ATParserMsg(void)
{
    SuccessFailType  sF                  = SUCCESS;
    ATParserErrorEnum err = AT_PARSER_NO_ERROR;
    Uint16Type       count = 0;
    Uint16Type       string_len = 0;
    Uint8Type        i = 0;
    Uint8Type        *ptrBuffer          = parserRxMsgBuffer;
    Uint8Type        *p_tmp = NULL;
    ATRspMessageType rsp_msg;

    /* Extract first tokem which is either a command / response identifier  */
    /* like *PFOO or a result(either OK or ERROR)                           */
    string_len = (Uint16Type)strlen((const char*)ptrBuffer);
    count      = _ATParser_FirstToken(ptrBuffer);
    (void)memset(&rsp_msg, 0x00,sizeof(ATRspMessageType));

    if(string_len != parserRxMsgCount)
    {
        DEBUG_VALUE2(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Msg length mismatch StringLen %d ATParserRxMsgCount %d",
                    string_len, parserRxMsgCount);

        UART_PrintStatistic(UART2);
    }



    if (0 == count)
    {
        sF = FAIL;
    }
    else
    {
        /* TODO - performance algorithm to avoid nested IF / ELSE needed*/
        DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "New message");

        /* first filter out OK result as this will be the most likely one   */
        if (0 == strncmp((char *)ptrBuffer, "OK", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_OK;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        /* ERROR result                                                     */
        else if (0 == strncmp((char *)ptrBuffer, "ERROR", count))
        {   /* TODO - maybe some data after this!!! */
            rsp_msg.common.rsp_type = AT_RSP_ERROR;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        /* ERROR:<extended error code> result                                                     */
         else if (0 == strncmp((char *)ptrBuffer, "ERROR:", count))
        {   /* TODO - maybe some data after this!!! */
            rsp_msg.common.rsp_type = AT_RSP_ERROR;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                                (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                        &(rsp_msg.data->peca.errorCode),0,
                        999))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }

        }
        /* *PSTU: startup event must be received before sending AT commands */
        else if (0 == strncmp((char *)ptrBuffer, "*PSTU", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_STARTUP;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        /* PBTS to indicate BT antenna */
        else if (0 == strncmp((char *)ptrBuffer, "*PBTA:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_ANTENNA;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* get memory pool element */
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                               (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                rsp_msg.data->pbta.value = (ATParamPresentEnum)
                    _ATParser_u8GetFromAsciiAndCheck(ptrBuffer,
                                      (Uint8Type)AT_VALUE_PRESENT, (Uint8Type)AT_VALUE_NOT_PRESENT);
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* PBSU: notify the Host about the update process. */
        else if (0 == strncmp((char *)ptrBuffer, "*PBSU:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_FLASH_SW_UPDATE_BT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* get memory pool element */
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                               (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                rsp_msg.data->pbsu.update_event = (ATBTSWUpdateResEnum)
                    _ATParser_u8GetFromAsciiAndCheck(ptrBuffer,
                                      (Uint8Type)AT_BT_SW_UPDATE_MD5_KEY_RESULT_NOT_OK_REBOOT,
                                      (Uint8Type)AT_BT_SW_UPDATE_START_TRANSFER_OF_FILE);

                /* we need to identify ife the response has several parameters
                 * search for ' within the string */
                if (rsp_msg.data->pbsu.update_event == 7)
                {
                   // first step use only the interesting infos
                   p_tmp = (Uint8Type*)strchr((const char *)ptrBuffer,AT_ASCII_QUOTE);

                   if (NULL == p_tmp)
                   { /* only Update Event */

                   }
                   else
                   {
                       // after ":" if AT command is in buffer
                       (void)memcpy(rsp_msg.data->pbsu.Md5Key, &ptrBuffer[3], AT_MD5_KEY_LEN);
                       (void)memcpy(rsp_msg.data->pbsu.Md5KeyEncryted, &ptrBuffer[38], AT_MD5_KEY_ENCRYTED_LEN);
                   }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* PPUS: notify the Host about the the phoneboob update progresss */
        else if (0 == strncmp((char *)ptrBuffer, "*PPUS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_UPDATE_STATUS;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* get memory pool element */
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                               (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                rsp_msg.data->ppus.PhonebookUpdateInProgress = (Uint8Type)
                    _ATParser_u8GetFromAsciiAndCheck(ptrBuffer,
                                      (Uint8Type)AT_PHONEBOOK_UPDATE_IN_PROGRESS, (Uint8Type)AT_PHONEBOOK_NO_UPDATE_IN_PROGRESS);

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSCA:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SET_CONN_AUTH;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* get memory pool element */
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                               (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                rsp_msg.data->psca.CurConnectMode = (ATBTConnAuthorizationResEnum)
                    _ATParser_u8GetFromAsciiAndCheck(ptrBuffer,
                                      (Uint8Type)AT_BT_CONN_AUTH_NOT_CONNECTABLE,
                                      (Uint8Type)AT_BT_CONN_AUTH_NOT_YET_PAIRED_DECIVES_CONNECTABLE);

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* *PBDADDR: initalize event must be received before sending AT     */
        /* commands AT*PSFN, AT*PBDI, AT*PPAU, AT*PSCA, AT*PSDM, AT*PSBD,   */
        /* AT*PBSC and AT*PBSV - ERM shall take care of this!!!             */
        else if (0 == strncmp((char *)ptrBuffer, "*PBDADDR:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SET_BT_ADDR;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer  += count;
                string_len -= count;
                (void)memcpy (rsp_msg.data->address.value, ptrBuffer,
                        AT_BT_ADDRESS_LEN);
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }

        else if (0 == strncmp((char *)ptrBuffer, "*PSBD:", count))
        {
            Uint32Type         tmp_value = 0;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd              = AT_RSP_WITH_DATA;
            rsp_msg.common.rsp_type = AT_RSP_BT_SELECT_DEV;
            /* extract device id and status */
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer  += count;
                string_len -= count;
                if (SUCCESS != _ATParser_GetDeviceAndValue(ptrBuffer,&(rsp_msg.data->psbd.id),
                                        &(tmp_value), (Uint8Type)AT_CONN_AUTO_NOT, string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->psbd.status = (ATConnStatusEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PLPD:", count))
        {
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer  += count;
                string_len -= count;
                /* we need to identify the response if it is for nb or complete
                 * device info - search for ' within the string */
                p_tmp = (Uint8Type*)strchr((const char *)ptrBuffer,AT_ASCII_QUOTE);
                if (NULL != p_tmp)
                {   /* this is response with complete device info */
                    rsp_msg.common.rsp_type = AT_RSP_GET_PAIRED_DEVICES_IN;
                    /* extract the device values */
                    if (SUCCESS != _ATParser_ExtPlpdInfo(&(rsp_msg.data->plpd_info),
                                                         ptrBuffer,string_len))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else /* response with device number */
                {
                    rsp_msg.common.rsp_type = AT_RSP_GET_PAIRED_DEVICES_NB;
                    /* extract number of devices */
                    rsp_msg.data->plpd_nb.nb_devices = (Uint8Type)atoi((const char *)ptrBuffer);
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }


        }
        /* Status receive AT command message for mobile indicators:         */
        /* BT-Phone, Network, Roaming, SignalLevel, OperationName,          */
        /* BatteryLevel, SMS received and voice mail                        */
        else if (0 == strncmp((char *)ptrBuffer, "*PIEV:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PHONE_STATUS_IND_EVT;
            rsp_msg.common.receiver = APP_SUB_TEL;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPievInfo(&(rsp_msg.data->piev),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                /* no else path needed as sF default is SUCCESS */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBCD:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_CLASS_DEVICE_REQUEST;
            rsp_msg.common.receiver = APP_SUB_TEL;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPbcd(&(rsp_msg.data->pbcd),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                /* no else path needed as sF default is SUCCESS */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* Mobile indicators                                                */
        else if (0 == strncmp((char *)ptrBuffer, "*PIND:", count))
        {
            rsp_msg.common.receiver = APP_SUB_TEL;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                /* response value are related to send req!!! */
                /* we need to identify the response if it is for support or values
                 * search for ' within the string */
                ptrBuffer+= count;
                string_len -= count;
                p_tmp = (Uint8Type*)strchr((const char *)ptrBuffer,AT_ASCII_QUOTE);
                if (NULL == p_tmp)
                {   /* only indicators included */
                    rsp_msg.common.rsp_type = AT_RSP_PHONE_STATUS_IND;
                    if (SUCCESS != _ATParser_ExtPindSup(&(rsp_msg.data->pind_sup),
                            ptrBuffer,string_len))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    rsp_msg.common.rsp_type = AT_RSP_PHONE_STATUS_IND_VAL;
                    if (SUCCESS != _ATParser_ExtPindVal(&(rsp_msg.data->pind_val),
                            ptrBuffer,string_len))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PNCC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_CALL_STATUS_IND_COUNT;
            rsp_msg.common.receiver = APP_SUB_TEL;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                rsp_msg.data->pncc.value =
                   _ATParser_u8GetFromAsciiAndCheck(ptrBuffer,0x02,0x00);
               /* TODO add enum or #defines to replace magic numbers */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PLCC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_CALL_STATUS_IND_INDEX;
            rsp_msg.common.receiver = APP_SUB_TEL;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPlccInfo(&(rsp_msg.data->plcc),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* phonebook change indicator */
        else if (0 == strncmp((char *)ptrBuffer, "*PPBC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_CHANGE_IND_TYPE;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* TODO - this message needs to be routed to phonebook */
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(rsp_msg.data->ppbc.type),AT_PARSER_MIN_PPBC_VALUE,
                        AT_PARSER_MAX_PPBC_VALUE))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* phonebook change indicator */
        else if (0 == strncmp((char *)ptrBuffer, "*PPBU:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_CHANGE_IND_EVENT;
            rsp_msg.dataInd              = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(rsp_msg.data->ppbu.type),AT_PARSER_MIN_PPBU_VALUE,
                        AT_PARSER_MAX_PPBU_VALUE))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* phonebook change indicator after call take over  */
        else if (0 == strncmp((char *)ptrBuffer, "*PFPA", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_AVAIL_NAV;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        /* SDP notification message */
        else if (0 == strncmp((char *)ptrBuffer, "*PSDP:", count))
        {
            Uint32Type    tmp_value = 0;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.rsp_type = AT_RSP_BT_SDP_NOT;
            /* TODO - clearify where to route this info */
            rsp_msg.common.receiver = APP_SUB_ERM;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                if (SUCCESS != _ATParser_GetDeviceAndValue(ptrBuffer,
                        &(rsp_msg.data->psdp.id), &(tmp_value),
                        (Uint8Type)AT_SDP_FAILED, string_len) )
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {   /* check status for min value */
                    rsp_msg.data->psdp.status = (ATSDPStatusEnum)tmp_value;
                    if (AT_SDP_SUCCEED > rsp_msg.data->psdp.status)
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* Audio transfer / discrete mode */
        else if (0 == strncmp((char *)ptrBuffer, "*PATR:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AUDIO_TRANS_DISCRETE;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_TEL;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(tmp_value), (Uint8Type)AT_AUDIO_DISCRETE_MODE,
                        (Uint8Type)AT_AUDIO_HANDSFREE_MODE))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->patr.mode = (ATAudioTransferModeEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* audio gateway settings */
        else if (0 == strncmp((char *)ptrBuffer, "*PAGM:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_AG_MODE;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_TEL;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(tmp_value), (Uint8Type)AT_AUDIO_GW_MODE_OFF,
                        (Uint8Type)AT_AUDIO_GW_MODE_ON))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->pagm.mode = (ATAudioGatewayModeEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* audio gateway transfer mode settings */
        else if (0 == strncmp((char *)ptrBuffer, "*PAGT:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_AG_TRANSFER;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_TEL;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(tmp_value), (Uint8Type)AT_AUDIO_GW_TRANSFER_MODE_OFF,
                        (Uint8Type)AT_AUDIO_GW_TRANSFER_MODE_ON))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->pagt.mode = (ATAudioGatewayTransferModeEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }

        /* Microphone Mute mode  */
        else if (0 == strncmp((char *)ptrBuffer, "*PMMU:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AMGMT_MIC_MUTE;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_TEL;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &tmp_value, (Uint8Type)AT_MIC_MUTE, (Uint8Type)AT_MIC_UNMUTE))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->pmmu.mode = (ATMicModeEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* Select phonebook  */
        else if (0 == strncmp((char *)ptrBuffer, "*PPBS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_SELECT;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* TODO - route to phonebook if queue available */
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &tmp_value, (Uint8Type)AT_PBTYPE_USER_PERSONAL, (Uint8Type)AT_PBTYPE_MISSED))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->ppbs.type = (ATPhoneBookTypeEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* Phone book call history */
        else if (0 == strncmp((char *)ptrBuffer, "*PPCH:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_CALL_HISTORY;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &tmp_value, (Uint8Type)AT_PB_CALLHIST_RAM, (Uint8Type)AT_PB_CALLHIST_DEL_FLASH))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->ppch.storage = (ATPhoneBookCallHistEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* SMS support notification */
        else if (0 == strncmp((char *)ptrBuffer, "*PMST:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SMS_SUPPORT_NOT;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_SMS;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &tmp_value, (Uint8Type)AT_FALSE, (Uint8Type)AT_TRUE))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->pmst.sms_supported = (ATBooleanEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* new SMS notification */
        else if (0 == strncmp((char *)ptrBuffer, "*PMNW:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SMS_NEW_NOT;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
            rsp_msg.common.receiver = APP_SUB_SMS;
        }
        /* SMS synchro Finished notification */
        else if (0 == strncmp((char *)ptrBuffer, "*PMLS:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*PMLS:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_SMS_LIST;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
                rsp_msg.common.receiver = APP_SUB_SMS;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PMRD:", count))
        {
            rsp_msg.common.receiver = APP_SUB_SMS;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                /* response value are related to send req!!! */
                /* we need to identify the response if it is for sms numbers or
                 * specifc sms - search for ' within the string */
                ptrBuffer+= count;
                string_len -= count;
                p_tmp = (Uint8Type*)strchr((const char *)ptrBuffer,AT_ASCII_QUOTE);
                if (NULL == p_tmp)
                {   /* only numbers included */
                    rsp_msg.common.rsp_type = AT_RSP_SMS_READING_NUMBER;
                    if (SUCCESS != _ATParser_ExtPmrdNum(&(rsp_msg.data->pmrd_num),
                            ptrBuffer,string_len))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {   /* message itself included */
                    rsp_msg.common.rsp_type = AT_RSP_SMS_READING;
                    if (SUCCESS != _ATParser_ExtPmrdMsg(&(rsp_msg.data->pmrd_msg),
                            ptrBuffer,string_len))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PMSDEX:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SMS_CONC_SEND;
            rsp_msg.common.receiver = APP_SUB_SMS;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                /* response value are related to send req!!! */
                /* we need to identify the response, how much pdu would be sended */
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPmsdexMsg(&(rsp_msg.data->pmsdex),
                        ptrBuffer, (Uint32Type)string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* set ring tone response */
        else if (0 == strncmp((char *)ptrBuffer, "*PSRT:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AMGMT_SET_RINGTONE;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            rsp_msg.common.receiver = APP_SUB_ERM;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* here one optional parameter may exists!! */
                (void)_ATParser_CheckNumberParam(ptrBuffer,0,&i,string_len);
                /* message has either one or 2 parameters */
                switch (i)
                {
                    case 1:
                    {
                        if (SUCCESS != _ATParser_sFExtRintoneModeValue(&ptrBuffer,
                                (ATRintoneModeEnum *)&(rsp_msg.data->pstr.mode_def),AT_RTMODE_DEACTIVATE,
                                AT_RTMODE_APP_BLUETOOTH))
                        {
                            err = AT_PARSER_ERR_EXTRACT;
                        }
                        else
                        {
                            rsp_msg.data->pstr.active_avail = AT_FALSE;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (SUCCESS == _ATParser_sFExtRintoneModeValue(&ptrBuffer,
                                (ATRintoneModeEnum *)&(rsp_msg.data->pstr.mode_def),AT_RTMODE_DEACTIVATE,
                                AT_RTMODE_APP_BLUETOOTH) &&
                            SUCCESS == _ATParser_sFExtRintoneModeValue(&ptrBuffer,
                                (ATRintoneModeEnum *)&(rsp_msg.data->pstr.active),AT_RTMODE_DEACTIVATE,
                                AT_RTMODE_APP_BLUETOOTH)    )
                        {
                            rsp_msg.data->pstr.active_avail = AT_TRUE;
                        }
                        else
                        {
                            err = AT_PARSER_ERR_EXTRACT;
                        }
                        break;
                    }
                    default:
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                        break;
                    }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPBP:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_READ_NUMBERS;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (BT_TRUE == ATCMD_bPCheckPbRequestIsVCR())
            {
                rsp_msg.common.receiver = APP_SUB_VCR;
            }
            else
            {
                rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            }
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPpbp(&(rsp_msg.data->ppbp),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                 /* else not required here */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPBR:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_READ_ENTRIES;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            /* check to whom to send the data */
            if (BT_TRUE == ATCMD_bPCheckPbRequestIsVCR())
            {
                rsp_msg.common.receiver = APP_SUB_VCR;
            }
            else
            {
                rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            }
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPpbr(&(rsp_msg.data->ppbr),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                 /* else not required here */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBSC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SERVICE_CONN;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPbsc(&(rsp_msg.data->pbsc),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PGMT:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AS_GET_META_TYPE;
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPgmt(&(rsp_msg.data->pgmt),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBSN:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SERVICE_NOT;
            /* TODO - check which application need this info!! */
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPbsn(&(rsp_msg.data->pbsn),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSBD:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SELECT_DEV;
            /* TODO - check which application need this info!! */
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPsbd(&(rsp_msg.data->psbd),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*DGPS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PLAYER_GET_STATUS;
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtDgps(&(rsp_msg.data->dgps),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*DGED:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_DISCO_GET_ENTRY_DATA;
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtDged(&(rsp_msg.data->dged),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PASA:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AMGMT_SRD_ACTIVE_IND;
            /* TODO check if only MDI need this indication */
            rsp_msg.common.receiver = APP_SUB_DEVICE;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPasa(&(rsp_msg.data->pasa),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PCMS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_AC_MODE_STATUS;
            /* TODO check if only MDI need this indication */
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPcms(&(rsp_msg.data->pcms),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSDM:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SET_DISCOVER_MODE;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtDiscoverModeValue(&ptrBuffer,
                        &(rsp_msg.data->psdm.mode),AT_DISCOVER_MODE_DISALLOW,
                        AT_DISCOVER_MODE_ALLOW))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSMD:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SET_MAX_PAIRED_DEVICES;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                                &(rsp_msg.data->psmd.max_num_paired_device),
                                AT_PARROT_MIN_DEVICE_ID,
                                AT_PARROT_MAX_DEVICE_ID))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "+CGMI", count))
        {   /* send as "empty message indication" only as this is fixed PARROT */
            rsp_msg.common.rsp_type = AT_RSP_MANUFAC_ID;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        else if (0 == strncmp((char *)ptrBuffer, "*POFF:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*POFF:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_POWER_OFF;
                rsp_msg.common.receiver = APP_SUB_ERM;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSFN:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SET_BT_NAME;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtQuotedString(&ptrBuffer,
                        (Uint8Type*)(rsp_msg.data->psfn.name),&(rsp_msg.data->psfn.name_len),
                        AT_BT_DEVICE_NAME_MAX_LEN))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "+CGMR:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_MODEL_REV;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtUnquotedString(&ptrBuffer,
                        (Uint8Type*)(rsp_msg.data->cgmr.version),&(rsp_msg.data->cgmr.version_len),
                        AT_BT_PF_CODE_MAX_LEN))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "+CGMM:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_MODEL_ID;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (0 == strncmp((char *)ptrBuffer,
                        PARROT_MODEL_CK5050P_STRING, string_len))
                {
                    rsp_msg.data->cgmm.model = AT_MODEL_CK5050P;
                }
                else if (0 == strncmp((char *)ptrBuffer,
                        PARROT_MODEL_CK5050_STRING, string_len))
                {
                    rsp_msg.data->cgmm.model = AT_MODEL_CK5050;
                }
                else
                {
                    rsp_msg.data->cgmm.model = AT_MODEL_UNKNOWN;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* *PWNG: unsolicited warning with 2 args                           */
        else if (0 == strncmp((char *)ptrBuffer, "*PWNG:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_GEN_WARNING;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPwng(&(rsp_msg.data->pwng),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* *PMFL: unsolicited event taht indicates that there is less than 10 % of free space in FLASH */
        else if (0 == strncmp((char *)ptrBuffer, "*PMFL:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_FLASH_FULL_EVENT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                Uint8Type tmp_value = 0;
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                                &tmp_value, (Uint8Type)AT_FALSE, (Uint8Type)AT_TRUE))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    rsp_msg.data->pmfl.free_space_indicator = (ATBooleanEnum)tmp_value;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* *PPDE: unsolicited event                           */
        /* IMPORTANT!!! First the PPDE:END has to be compared to avoid that
         * "END" is interpreted as device id!!! */
        else if (0 == strncmp((char *)ptrBuffer, "*PPDE:", count))
        {
            Uint8Type deviceId= 0;
            if (0 == strncmp((char *)ptrBuffer, "*PPDE:END", string_len))
            {
                DEBUG_TEXT(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "Parser IGNORE PPDE END");

                sF = FAIL;
            }
            else
            {
                DEBUG_TEXT(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "Parser simple PPDE");

                rsp_msg.common.rsp_type = AT_RSP_PAIR_DELETE;
                rsp_msg.common.receiver = APP_SUB_ERM;
                rsp_msg.dataInd         = AT_RSP_WITH_DATA;
                if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                        (void**)&(rsp_msg.data)))
                {
                    ptrBuffer += count;
                    if (SUCCESS == _ATParser_ExtDeviceId(&ptrBuffer, &deviceId))
                    {
                        rsp_msg.data->ppde.device_id = deviceId;
                    }
                    else
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_MEMORY;
                }
            }
        }
        /* *PPRS: unsolicited event                           */
        else if (0 == strncmp((char *)ptrBuffer, "*PPRS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PAIR_REQUEST_RESULT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;


            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPprs( &(rsp_msg.data->pprs),
                                                  ptrBuffer,
                                                  (Uint32Type)string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* VOICE recognition started */
        else if (0 == strncmp((char *)ptrBuffer, "*RSTU", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_START_UP_EVENT;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        /* to support SW200 RC6 and newer */
        else if (0 == strncmp((char *)ptrBuffer, "*RSTU:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_START_UP_EVENT;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
        }
        /* to support SW200 RC7 and newer */
        else if (0 == strncmp((char *)ptrBuffer, "*RSTUEX:", count))
        {

            rsp_msg.common.rsp_type = AT_RSP_SIVR_START_UP_EVENT_EXT;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;

            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;

                if (SUCCESS != _ATParser_ExtRstu(&(rsp_msg.data->rstuex),
                                                   ptrBuffer,string_len))
                {
                    DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "*RSTUEX Parsing Error");
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
               DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "*RSTUEX Parsing Error - memory error");
                err = AT_PARSER_ERR_MEMORY;
            }

        }
        /* voice recognition engine status */
        else if (0 == strncmp((char *)ptrBuffer, "*RSTS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_VR_STATUS;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* extract enumeration */
                if (SUCCESS != _ATParser_sFExtSIVREngStatusValue(&ptrBuffer,
                        &(rsp_msg.data->rsts.status),
                        AT_SIVR_ENG_IDLE,AT_SIVR_ENG_END_SYNC))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* voice recognition number supported languages */
        else if (0 == strncmp((char *)ptrBuffer, "*RGSL:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_GET_SUPPORTED_LANG;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtRgsl( &(rsp_msg.data->rgsl),
                        ptrBuffer,
                        string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*RRES:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_VR_RESULT;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtRres( &(rsp_msg.data->rres),
                        ptrBuffer,
                        string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*RSCL:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_SET_CURRENT_LANG;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtRscl( &(rsp_msg.data->rscl),
                        ptrBuffer,
                        string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBCV:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_CHIP_VERSION;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sfExtPbcv( &(rsp_msg.data->pbcv),
                        ptrBuffer,
                        string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*RSRF:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_GET_SUPPORTED_LANG;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtRsrf( &(rsp_msg.data->rsrf),
                        ptrBuffer,
                        string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* handling for PALGOSELECT - MIC algorithm */
        else if (0 == strncmp((char *)ptrBuffer, "*PALGOSELECT:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_MIC_ALGO_SELECT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPalgoselect( &(rsp_msg.data->palgoselect),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* handling for PALGOSETMIC - MIC id */
        else if (0 == strncmp((char *)ptrBuffer, "*PALGOSETMIC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_MIC_CHOICE_SET;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPalgoSetMic( &(rsp_msg.data->palgosetmic),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "+VGM:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_MIC_GAIN_SET;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtMicGain( &(rsp_msg.data->vgm),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PALGOAEC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_ECHO_CANCELLATION;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPalgoAEC( &(rsp_msg.data->palgoaec),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PNVS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AMGMT_OUT_VOLUME_SET;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtVolumeSet( &(rsp_msg.data->pnvs),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "+CGMREX:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_MODEL_REV_EXT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* copy string completely */
                if (string_len > AT_CMGREX_MAX_LEN)
                {
                    string_len = AT_CMGREX_MAX_LEN;
                }
                rsp_msg.data->cgmrex.extRev_len = (Uint8Type)string_len;
                (void)memcpy((rsp_msg.data->cgmrex.extRev), ptrBuffer,string_len);
                rsp_msg.data->cgmrex.extRev[string_len] = '\0';
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPNC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_TOTAL_COUNT;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* extract total count 16 bit value */
                /* extract enumeration */
                if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                        &(rsp_msg.data->ppnc.total),
                        0x0000,0xFFFF))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PDAS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_DISABLE_AUTO_SYNC;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* extract total count 16 bit value */
                /* extract enumeration */
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(rsp_msg.data->pdas.mode),
                        AT_PBMODE_AUTOSYNC_ENABLED,AT_PBMODE_AUTOSYNC_DISABLED))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPAC:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_GET_ALPHA_COUNT;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* extract total count 16 bit value */
                /* extract enumeration */
                if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                        &(rsp_msg.data->ppac.count),
                        0x0000,0xFFFF))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPNO:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_NAME_ORDER_SELECT;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* extract total count 16 bit value */
                /* extract enumeration */
                if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                        &(rsp_msg.data->ppno.mode),
                        AT_PB_SORTORDER_FIRSTLAST,AT_PB_SORTORDER_LASTFIRST))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPAD:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_ADD_ENTRY;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* extract total count 16 bit value */
                /* extract enumeration */
                if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                        &(rsp_msg.data->ppad.pb_index),
                        0x0000,0xFFFF))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PKEV:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_AS_KEY_EVENT_INDICATOR;
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtKeyRangeValue(&ptrBuffer,
                        &(rsp_msg.data->pkev.key),
                        AT_RSP_KEY_AUDIO_RESUMED,
                        AT_RSP_KEY_AUDIO_SUSPENDED))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPBD:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*PPBD:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_PB_DELETE_ENT_ALL_END;
                rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PFLASHERASE:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*PFLASHERASE:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_FLASH_ERASE_USERDATA;
                rsp_msg.common.receiver = APP_SUB_ERM;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPAS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_ALPHA_SEARCH;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extrat the pb index */
                    if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                            &(rsp_msg.data->ppas.pb_index),
                            0x0000,0xFFFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPASEX:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_ALPHA_SEARCH_EXT;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* two mandatory paramter - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        2,&i,string_len)  )
                {   /* extrat the pb index */
                    if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                            &(rsp_msg.data->ppasex.pb_index),
                            0x0000,0xFFFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                    /* extract number of matching contacts */
                    if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                            &(rsp_msg.data->ppasex.numMatchingContacts),
                            0x0000,0xFFFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }

        else if (0 == strncmp((char *)ptrBuffer, "*PSCM:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SET_AC_MODE;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the mode enumeration */
                    if (SUCCESS != _ATParser_ExtBool(&ptrBuffer,
                            &(rsp_msg.data->pscm.auto_conn_active)))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PRVP:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_VT_RECORD_INDEX;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                if (0 == strncmp((char *)ptrBuffer, "*PRVP:END", string_len))
                {
                    rsp_msg.data->prvp.result = AT_VTREC_RESULT_END;
                }
                else
                {   /* extract parameter */
                    ptrBuffer+= count;
                    string_len -= count;
                    /* one mandatory paramter result - check */
                    if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                            1,&i,string_len)  )
                    {   /* extract the result */
                        if (SUCCESS != _ATParser_sFExtVTResultValue(&ptrBuffer,
                                &(rsp_msg.data->prvp.result),
                                AT_VTREC_RESULT_INPROG,
                                AT_VTREC_RESULT_TOO_LONG))
                        {
                            err = AT_PARSER_ERR_EXTRACT;
                        }
                    }
                    else
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PRVK:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_VT_RECORD_KEY;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                if (0 == strncmp((char *)ptrBuffer, "*PRVK:END", string_len))
                {
                    rsp_msg.data->prvk.result = AT_VTREC_RESULT_END;
                }
                else
                {   /* extract parameter */
                    ptrBuffer+= count;
                    string_len -= count;
                    /* one mandatory paramter result - check */
                    if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                            1,&i,string_len)  )
                    {   /* extract the result */
                        if (SUCCESS != _ATParser_sFExtVTResultValue(&ptrBuffer,
                                &(rsp_msg.data->prvk.result),
                                AT_VTREC_RESULT_INPROG,
                                AT_VTREC_RESULT_TOO_LONG))
                        {
                            err = AT_PARSER_ERR_EXTRACT;
                        }
                    }
                    else
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PTVP:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_VT_TEST_PRESENCE_INDEX;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_ExtBool(&ptrBuffer,
                            &(rsp_msg.data->ptvp.voice_tag_exists)))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PTVP:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_VT_TEST_PRESENCE_KEY;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_ExtBool(&ptrBuffer,
                            &(rsp_msg.data->ptvk.voice_tag_exists)))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPVP:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*PPVP:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_VT_PLAYBACK_INDEX;
                rsp_msg.common.receiver = APP_SUB_VCR;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPVK:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*PPVK:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_VT_PLAYBACK_KEY;
                rsp_msg.common.receiver = APP_SUB_VCR;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PNVP:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_VT_GET_NUMBER_TAGS;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                            &(rsp_msg.data->pnvp.nb_voice_tags),
                              0x0000, 0xFFFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PAVP:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_VT_AUTO_TAG;
            //rsp_msg.common.receiver = APP_SUB_VCR;
            /* Susanne: Need this in ERM, VCR is currently not using this response */
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_ExtBool(&ptrBuffer,
                            &(rsp_msg.data->pavp.mode_enable)))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*RPRE:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_PREPARE_VR;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_sFExtQuotedString(&ptrBuffer,
                            (Uint8Type*)(rsp_msg.data->rpre.text),
                            &(rsp_msg.data->rpre.text_len),
                            AT_SIVR_MAX_PROMPT_TEXT_LEN))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*RVTA:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SIVR_MGR_VT;
            rsp_msg.common.receiver = APP_SUB_VCR;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_sFExtu16RangeValue(&ptrBuffer,
                            &(rsp_msg.data->rvta.nb_voice_tag),
                            0x0000, 0xFFFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PALGONOISECOMFORT:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_ECHO_NOISE_COMFORT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_ExtBool(&ptrBuffer,
                            &(rsp_msg.data->palgonoisecomfort.mode_activated)))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PALGOAECDELAY:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_ECHO_DELAY_CANCELLATION;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                            &(rsp_msg.data->palgoaecdelay.delay),
                            AT_AEC_DELAY_MIN,AT_AEC_DELAY_MAX))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PALGOALCRX:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_SPK_ALC_RX;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameters */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                (void)_ATParser_CheckNumberParam(ptrBuffer,
                        AT_PARSER_MIN_PALGOALCRX_PARAM,&i,string_len);
                if (AT_PARSER_MIN_PALGOALCRX_PARAM <= i &&
                    AT_PARSER_MAX_PALGOALCRX_PARAM >= i    )
                {   /* extract mandatory mode */
                    if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                            &(rsp_msg.data->palgoalcrx.mode),
                            AT_ALC_RX_MODE_MIN,AT_ALC_RX_MODE_MAX))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                    else
                    {   /* check if second param */
                        if (AT_PARSER_MAX_PALGOALCRX_PARAM == i)
                        {
                            if (SUCCESS != _ATParser_sFExtu8RangeValue(&ptrBuffer,
                                    &(rsp_msg.data->palgoalcrx.target),
                                    AT_ALC_RX_MIN,AT_ALC_RX_MAX))
                            {
                                err = AT_PARSER_ERR_EXTRACT;
                            }
                        }
                        /* else no other param !!! */
                    }
                }
                else
                {   /* inco number of params */
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBDI:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_INQUIRY;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* first check if this is END */
                if (0 == strncmp((char *)ptrBuffer, "*PBDI:END", string_len))
                {
                    rsp_msg.data->pbdi.is_end = AT_TRUE;
                    /* all other params do not need to be filled and shall
                     * be ignored by receiving module */
                }
                else
                {
                    rsp_msg.data->pbdi.is_end = AT_FALSE;
                    ptrBuffer+= count;
                    string_len -= count;
                    if (SUCCESS != _ATParser_ExtPbdi(&(rsp_msg.data->pbdi),
                            ptrBuffer,string_len))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                     /* else not required here */
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }

        }
        else if (0 == strncmp((char *)ptrBuffer, "*PGIR:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_GET_INQUIRY_RESULT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPgir(&(rsp_msg.data->pgir),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                 /* else not required here */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }

        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPRQ:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_IN_PAIR_REQUEST;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPprq(&(rsp_msg.data->pprq),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                 /* else not required here */
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }

        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBSM:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SERVICE_MODE;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_ExtBool(&ptrBuffer,
                            &(rsp_msg.data->pbsm.aHFPActivated)))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PPDS:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_PB_DWNLOAD_STATUS;
            rsp_msg.common.receiver = APP_SUB_PHONEBOOK;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* one mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        1,&i,string_len)  )
                {   /* extract the result */
                    if (SUCCESS != _ATParser_sFExtu16RangeValue(
                                                &ptrBuffer,
                                                &(rsp_msg.data->ppds.dl_status),
                                                1,
                                                0xFFFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        /* Ringtone playing Finished notification */
        else if (0 == strncmp((char *)ptrBuffer, "*PPRT:", count))
        {
            if (0 == strncmp((char *)ptrBuffer, "*PPRT:END", string_len))
            {
                rsp_msg.common.rsp_type = AT_RSP_RINGTONE_END;
                rsp_msg.dataInd         = AT_RSP_WITHOUT_DATA;
                rsp_msg.common.receiver = APP_SUB_ERM;
            }
            else
            {
                err = AT_PARSER_ERR_EXTRACT;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSSN:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_IMSI;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                if (SUCCESS != _ATParser_sFExtQuotedString(&ptrBuffer,
                        (Uint8Type*)(rsp_msg.data->pssn.imsi),&(rsp_msg.data->pssn.imsi_len),
                        AT_MAX_IMSI_LEN))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if(0 == strncmp((char *)ptrBuffer, "*PBSA:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SERVICE_AC;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {   /* extract parameter */
                ptrBuffer+= count;
                string_len -= count;
                /* two mandatory paramter result - check */
                if (SUCCESS == _ATParser_CheckNumberParam(ptrBuffer,
                        2,&i,string_len)  )
                {   /* extract the result */
                    /* get service */
                    Uint8Type tmpVal = 0;
                    if (SUCCESS != _ATParser_sFExtu8RangeValue(
                                                &ptrBuffer,
                                                &tmpVal,
                                                1,
                                                0xFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                    else
                    {
                        rsp_msg.data->pbsa.service = (ATBTServiceEnum)tmpVal;
                    }
                    /* get mode */
                    if (SUCCESS != _ATParser_sFExtu8RangeValue(
                                                &ptrBuffer,
                                                &tmpVal,
                                                1,
                                                0xFF))
                    {
                        err = AT_PARSER_ERR_EXTRACT;
                    }
                    else
                    {
                        rsp_msg.data->pbsa.mode = (ATBTAutoConnMode)tmpVal;
                    }
                    if( 3 == i )
                    {
                        /* extract bitmask */
                        if (SUCCESS != _ATParser_sFExtu8RangeValue(
                                                &ptrBuffer,
                                                &(rsp_msg.data->pbsa.bitmask),
                                                1,
                                                0xFF))
                        {
                            err = AT_PARSER_ERR_EXTRACT;
                        }
                    }
                }
                else
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }

            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSSP:", count))
        {
            /* PSSP response handling */
            rsp_msg.common.rsp_type = AT_RSP_BT_SSP_SETUP;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                if (SUCCESS != _ATParser_sfExtPsspInfo(&(rsp_msg.data->pssp), ptrBuffer, string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PSPR:", count))
        {
            /* PSPR response handling */
            rsp_msg.common.rsp_type = AT_RSP_BT_SSP_REQUEST;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer += count;
                string_len -= count;
                if (SUCCESS != _ATParser_sfExtPsprInfo(&(rsp_msg.data->pspr), ptrBuffer, string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PAMU:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_DISCO_METADATA_UPDATE;
            /* TODO - check which application need this info!! */
            rsp_msg.common.receiver = APP_SUB_MDI;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_ExtPamu(&(rsp_msg.data->pamu),
                        ptrBuffer,string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else if (0 == strncmp((char *)ptrBuffer, "*PBSF:", count))
        {
            rsp_msg.common.rsp_type = AT_RSP_BT_SUP_FEAT;
            rsp_msg.common.receiver = APP_SUB_ERM;
            rsp_msg.dataInd         = AT_RSP_WITH_DATA;
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(rsp_msg.data)))
            {
                ptrBuffer+= count;
                string_len -= count;
                if (SUCCESS != _ATParser_sfExtPbsfInfo(&(rsp_msg.data->pbsf),ptrBuffer, string_len))
                {
                    err = AT_PARSER_ERR_EXTRACT;
                }
                else
                {
                    DEBUG_VALUE2(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"PSBF: feature = %d, result = %d",rsp_msg.data->pbsf.feature,rsp_msg.data->pbsf.result);
                    DEBUG_VALUE2(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"PSBF: device = %d, type = %d",rsp_msg.data->pbsf.device_id,rsp_msg.data->pbsf.deviceType);
                }
            }
            else
            {
                err = AT_PARSER_ERR_MEMORY;
            }
        }
        else
        {   /* unsupported message */
            err = AT_PARSER_ERR_UNSUPPORTED;
        }
    }
    /* end of string compare section. Now work the response */

    if (AT_PARSER_NO_ERROR == err)
    {   /* message processed and filled in data structure, send the message */
        /* here we need send function from OSCan - prototype needed !!!     */
        /* before sending we check if message is with data, if so we need
         * to copy the common part also in the data pointer */
        if (NULL != rsp_msg.data && AT_RSP_WITH_DATA == rsp_msg.dataInd)
        {
            rsp_msg.data->common.rsp_type = rsp_msg.common.rsp_type;
            rsp_msg.data->common.receiver = rsp_msg.common.receiver;
        }

        // SUSANNE: #2075 need to duplicate RVTA event so APP_PHONEBOOK can also use it to get
        // number of voice tags
        if (AT_RSP_SIVR_MGR_VT == rsp_msg.common.rsp_type )
        {   /* create new message and get new pool and send to APP_PHONEBOOK */
            ATRspMessageType dup_msg;
            memset(&dup_msg,0x00,sizeof(ATRspMessageType));
            memcpy(&dup_msg,&rsp_msg,sizeof(ATRspMessageType));
            if (SUCCESS == POOL_sfAllocate(POOL_enATRspAllType,
                    (void**)&(dup_msg.data)))
            {   /* copy contents */
                memcpy(dup_msg.data,rsp_msg.data,sizeof(ATRspAllType));
                /* set receiver to ERM */
                dup_msg.common.receiver = APP_SUB_PHONEBOOK;
                dup_msg.data->common.receiver = APP_SUB_PHONEBOOK;
                /* now send message */
                if (SUCCESS !=  _ATParser_SendMessage(&dup_msg))
                {   /* send failed - need to release pool element */
                    POOL_sfFree(dup_msg.data);
                    DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"send failed for dupl message %d",
                                 rsp_msg.common.rsp_type);
                }
            }
            else
            {
                DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"alloc failed for dupl message %d",
                              rsp_msg.common.rsp_type);
            }
        }


        if (SUCCESS != _ATParser_SendMessage(&rsp_msg))
        {
            err = AT_PARSER_ERR_SEND;
        }
    }
    /* in failure case check if rsp_msg.data allocated, if so free!!! */
    if ((AT_PARSER_NO_ERROR != err) && (NULL != rsp_msg.data))
    {
        if(AT_PARSER_ERR_UNSUPPORTED != err)
        {
            DEBUG_STATE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"Command received: $ATResponseEnum$ = %d, parsing failed",
                     rsp_msg.common.rsp_type);
        }
        (void)POOL_sfFree(rsp_msg.data);
    }
    switch (err)
    {
        case AT_PARSER_ERR_MEMORY:
        {
            DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER, "mem error %d", POOL_enGetAllocateErrorCode());
            sF = FAIL;
            break;
        }
        case AT_PARSER_ERR_UNSUPPORTED:
        {
            DEBUG_TEXT(AT_PARSER_SWI_WNG, MOD_AT_PARSER, "Unsupported message");
            sF = FAIL;
            break;
        }
        case AT_PARSER_ERR_SEND:
        {
            sF = FAIL;
            DEBUG_VALUE2(AT_PARSER_SWI_WNG, MOD_AT_PARSER, "Send msg %d to %d ",
                    rsp_msg.common.rsp_type,rsp_msg.common.receiver);
            break;
        }
        case AT_PARSER_ERR_EXTRACT:
        {
            DEBUG_STATE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER, "Command $ATResponseEnum$ = %d", rsp_msg.common.rsp_type);
            DEBUG_TEXT(AT_PARSER_SWI_WNG, MOD_AT_PARSER, "extract fail");
            sF = FAIL;
            break;
        }
        case AT_PARSER_NO_ERROR:
        default:
        {
            sF = SUCCESS;
            break;
        }
    }
    return(sF);
}


/****************************************************************************/
/*  Method:     _ATParser_SendMessage                                       */
/****************************************************************************/
static SuccessFailType _ATParser_SendMessage(ATRspMessageType * p_msg)
{
    SuccessFailType  sF      = SUCCESS;
    /* no else needed as sF default is success */
    if (E_OK != SendMessage(msgApplATResponse, p_msg))
    {
        sF = FAIL;
        DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "SendMesssage buffer is FULL");
    }
    return sF;
}


/* End Of File ATParser.c */
