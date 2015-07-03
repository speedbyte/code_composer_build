/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_PhoneBook.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Phone Book part
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

#include "ATCmdHandler_PhoneBook.id"
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
/* Function    : AT_Cmd_Handler_Phonebook                                     */
/**
    \brief      Command handler for phonebook

    \brief      check members of given data structure for proper processing \n
                of phonebook application specific commands and build string for\n
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
SuccessFailType AT_Cmd_Handler_Phonebook(ATReqAllType   *ATRequest,
                                         const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type       cmdLen = 0;
    Uint8Type       cmdBuf[MAX_LEN_AT_COMMAND_BUFFER] = {0};

    if (SUCCESS == ERM_CheckUartOwnership(APP_SUB_PHONEBOOK,(const SCI_ST*)UartTxBuffer))
    {
        switch (ATRequest->common.req_type)
        {
            /* handling for phonebook specific requests */
            case AT_REQ_PB_SELECT:
            {   /* first check if given data matches the requirements */
                switch (ATRequest->ppbs.type)
                {
                    case AT_PBTYPE_USER_PERSONAL:
					{   /* check that mode is in correct range */
                        if (AT_PBMODE_ALL       <= ATRequest->ppbs.mode &&
                            AT_PBMODE_ONLY_CTAG >= ATRequest->ppbs.mode)
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PPBS=%d,%d\r",ATRequest->ppbs.type,ATRequest->ppbs.mode);
                        }
                        else
                        {
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHldPB inco mode %d",
                                                                                     ATRequest->ppbs.mode);
                            sF = FAIL;
                        }
                        break;
                    }
                    case AT_PBTYPE_LAST_DIALLED:
                    case AT_PBTYPE_LAST_RECEIVED:
                    case AT_PBTYPE_MISSED:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PPBS=%d\r",ATRequest->ppbs.type);
                        break;
                    }
                    case AT_PBTYPE_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPBS?\r");
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHldPB inco type %d",
                                                                                 ATRequest->ppbs.type);
                        break;
                    }
                }
                break;
            }
            case AT_REQ_PB_CALL_HISTORY:
            {
                switch (ATRequest->ppch.action)
                {
                    case AT_PB_CALLHIST_RAM:
                    case AT_PB_CALLHIST_FLASH:
                    case AT_PB_CALLHIST_DEL_FLASH:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PPCH=%d\r",ATRequest->ppch.action);
                        break;
                    }
                    case AT_PB_CALLHIST_GET:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PPCH?\r");
                        break;
                    }
                    default:
                    {   /* inco mute mode - raise trace and set to fail */
                        sF= FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHldPB inco call hist act %d",
                                                                                 ATRequest->ppch.action);
                        break;
                    }
                }
                break;
            }
            case AT_REQ_PB_READ_ENTRIES:
            {   /* check if get size is set */
                if (AT_TRUE == ATRequest->ppbr.is_get_pb_size)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPBR=?\r");
                }
                else
                {   /* TODO - ISSUE - we need to check against pb_size!!! */
                    /* check given index value for valid range           */
                    // SUSANNE: To fulfill TODO i added an extra field pbSize
                    // which can be checked here instead of AT_MAX_PHONEBOOK_ENTRIES
                    if (ATRequest->ppbr.start_idx > 0 &&
                        ATRequest->ppbr.end_idx >= ATRequest->ppbr.start_idx &&
                        ATRequest->ppbr.end_idx <= ATRequest->ppbr.pbSize /*AT_MAX_PHONEBOOK_ENTRIES*/ &&
                        MAX_NUM_PB_QUERY_SIZE >=
                            (ATRequest->ppbr.end_idx - ATRequest->ppbr.start_idx) )
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPBR=%d,%d\r",
                            ATRequest->ppbr.start_idx,ATRequest->ppbr.end_idx);
                    }
                    else
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHldPB ppbr idx error start_idx %d end_idx %d",
                                                                                 ATRequest->ppbr.start_idx,
                                                                                 ATRequest->ppbr.end_idx);
                    }
                }
                break;
            }
            case AT_REQ_PB_READ_NUMBERS:
            {   /* check parameters for valid range as far as possible */
                if (0 < ATRequest->ppbp.pb_idx &&
                    0 < ATRequest->ppbp.pn_idx1 &&
                    ATRequest->ppbp.pn_idx1 <= ATRequest->ppbp.pn_idx2)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPBP=%d,%d,%d\r",
                            ATRequest->ppbp.pb_idx,
                            ATRequest->ppbp.pn_idx1,
                            ATRequest->ppbp.pn_idx2);
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHldPB pppb idx error %d %d %d",
                                                                             ATRequest->ppbp.pb_idx,
                                                                             ATRequest->ppbp.pn_idx1,
                                                                             ATRequest->ppbp.pn_idx2,0);
                }
                break;
            }
            case AT_REQ_PB_TOTAL_COUNT:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPNC\r");
                break;
            }
            
            case AT_REQ_PB_DISABLE_AUTO_SYNC:
            {
                /* check for the mode */
                switch (ATRequest->pdas.mode)
                {
                    case AT_PBMODE_AUTOSYNC_ENABLED:
                    case AT_PBMODE_AUTOSYNC_DISABLED:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PDAS=%d\r",
                                ATRequest->pdas.mode);
                        break;
                    }
                    case AT_PBMODE_AUTOSYNC_GET:
                    default:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PDAS?\r");
                        break;
                    }
                }
                break;
            }
            
            
            case AT_REQ_PB_START_MANUAL_SYNC:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPMS\r");
                break;
            }
            
            case AT_REQ_PB_DELETE_ENTRY_ALL:
            {
                if (AT_FALSE == ATRequest->ppbd.index_avail)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPBD\r");
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPBD=%d\r",
                            ATRequest->ppbd.index);
                }
                break;
            }
            
            case AT_REQ_PB_NAME_ORDER_SELECT:
            {
                switch (ATRequest->ppno.mode)
                {
                    case AT_PB_SORTORDER_FIRSTLAST:
                    case AT_PB_SORTORDER_LASTFIRST:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPNO=%d\r",
                                ATRequest->ppno.mode);
                        break;
                    }
                    case AT_PB_SORTORDER_GET:
                    default:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPNO?\r");
                        break;
                    }
                }
                break;
            }
            
            case AT_REQ_PB_GET_ALPHA_COUNT:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPAC='%c'\r",
                        ATRequest->ppac.letter);
                break;
            }

            case AT_REQ_PB_EN_DWNLOAD_STATUS:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PEDS=%d\r",
                        (ATRequest->peds.enable == AT_TRUE ) ? 1 : 0 );
                break;
            }
            
            case AT_REQ_PB_ADD_ENTRY:
            {   /* ensure that the string values are terminated with \0 */
                ATRequest->ppad.name[ATRequest->ppad.name_len] = '\0';
                ATRequest->ppad.phone_number[ATRequest->ppad.phone_number_len] = '\0';
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*PPAD='%s','%s',%d\r",
                        ATRequest->ppad.name,ATRequest->ppad.phone_number,ATRequest->ppad.pn_type);
                break;
            }

            case AT_REQ_PB_ALPHA_SEARCH:
            {   /* check the length > 0 and less equal max */
                if (0 < ATRequest->ppas.initial_len &&
                    AT_CALLER_ID_STRING_MAX_LEN >= ATRequest->ppas.initial_len    )
                {   /* length match - ensure '\0' termination */
                    ATRequest->ppas.initial[ATRequest->ppas.initial_len] = '\0';
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PPAS='%s'\r",ATRequest->ppas.initial);
                }
                else
                {
                    sF = FAIL;
                    /* add trace here - unsupported enum value  */
                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel PPAS oor %d",
                                                                             ATRequest->ppas.initial_len);
                }
                break;
            }
            case AT_REQ_PB_ALPHA_SEARCH_EXT:
            {   /* check the length > 0 and less equal max */
                if (0 < ATRequest->ppasex.initial_len &&
                    AT_CALLER_ID_STRING_MAX_LEN >= ATRequest->ppasex.initial_len    )
                {   /* length match - ensure '\0' termination */
                    ATRequest->ppasex.initial[ATRequest->ppasex.initial_len] = '\0';
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PPASEX='%s'\r",ATRequest->ppasex.initial);
                }
                else
                {
                    sF = FAIL;
                    /* add trace here - unsupported enum value  */
                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CmdHdlTel PPASEX oor %d",
                                                                             ATRequest->ppasex.initial_len);
                }
                break;
            }
            case AT_REQ_SIVR_MANAGE_VOICE_TAG:
            {
                switch (ATRequest->rvta.action)
                {
                    case  AT_VTMGR_ACT_RETRIEVE:
                    {
                        if (AT_TRUE == ATRequest->rvta.type_avail)
                        {
                            switch (ATRequest->rvta.type)
                            {
                                case AT_SIVR_ITEMTYPE_CONTACT:
                                {
                                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*RVTA=%d,%d,%d\r",
                                            ATRequest->rvta.action,
                                            ATRequest->rvta.identifier,
                                            ATRequest->rvta.type);
                                    break;
                                }
                                case AT_SIVR_ITEMTYPE_NONE:	/*Fall through. PC-Lint Info 788.*/
                                default:
                                {
                                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "ATCmdVcr inco rvta type %d",
                                                                                             ATRequest->rvta.type);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*RVTA=%d,%d\r",
                                ATRequest->rvta.action,ATRequest->rvta.identifier);
                        }
                        break;
                    }
                    default:
                    {
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "ATCmdVcr inco rvta action %d",
                                                                                 ATRequest->rvta.action);
                        break;
                    }
                }
                break;
            }



            default:
                sF = FAIL;
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHldPB unsupported type %d",
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
                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHldPB werr msg %d len %d",
                                                                         ATRequest->common.req_type,cmdLen);
            }
            else
            {
                _ATCMD_bPbRequestIsVCR = 0x00;
            }
        }
        else
        {
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHldPB berr msg %d",
                                                                     ATRequest->common.req_type);
        }
    }
    else
    {
        DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHldPB res blocked");
    }
    return (sF);
}

/* End Of File ATCmdHandler_PhoneBook.c */
