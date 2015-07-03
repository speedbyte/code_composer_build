/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_VCR.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler VCR part
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
#include "VCR_StateHandler.h"

#include "ATCmdHandler_VCR.id"
#include "Trace.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern VcrModGlobalDataType _VCR_gData;

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

/* used by Parser to identify if VCR or PB has send the request -
 * workaround as long as PB redesign is not done */
BooleanType ATCMD_bPCheckPbRequestIsVCR(void)
{
    if (0x01 == _ATCMD_bPbRequestIsVCR)
    {
        return BT_TRUE;
    }
    else
    {
        return BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : AT_Cmd_Handler_VCR                                           */
/**
    \brief      Command handler for VCR

    \brief      check members of given data structure for proper processing \n
                of VCR application specific commands and build string for\n
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
SuccessFailType AT_Cmd_Handler_VCR(ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type       cmdLen = 0;
    Uint8Type       cmdBuf[MAX_LEN_AT_COMMAND_BUFFER] = {0};
    Uint8Type       i = 0;

    if (SUCCESS == ERM_CheckUartOwnership(APP_SUB_VCR,(const SCI_ST*)UartTxBuffer))
    {

        switch (ATRequest->common.req_type)
        {
            case AT_REQ_PB_CALL_HISTORY:
            {
                switch (ATRequest->ppch.action)
                {
                    case AT_PB_CALLHIST_DEL_FLASH:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PPCH=%d\r",ATRequest->ppch.action);
                        break;
                    }
                    case AT_PB_CALLHIST_GET:
                    case AT_PB_CALLHIST_RAM:
                    case AT_PB_CALLHIST_FLASH:
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
            case AT_REQ_VT_RECORD_INDEX:
            {
                switch (ATRequest->prvp.action)
                {
                    case AT_VTREC_ACT_ABORT:
                    case AT_VTREC_ACT_START:
                    case AT_VTREC_ACT_CONTINUE:
                    {
                        if (AT_TRUE == ATRequest->prvp.pb_idx_avail)
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PRVP=%d,%d\r",ATRequest->prvp.action,
                                ATRequest->prvp.pb_idx);
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PRVP=%d\r",ATRequest->prvp.action);
                        }
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr unsup vtaction %d",
                                                                                 ATRequest->prvp.action);
                    }
                }
                break;
            }
            case AT_REQ_VT_RECORD_KEY:
            {
                switch (ATRequest->prvk.action)
                {
                    case AT_VTREC_ACT_ABORT:
                    case AT_VTREC_ACT_START:
                    case AT_VTREC_ACT_CONTINUE:
                    {
                        if (AT_TRUE == ATRequest->prvk.kw_idx_avail)
                        {
                            if (AT_VTREC_KWIDX_GENERAL <= ATRequest->prvk.kw_idx &&
                                AT_VTREC_KWIDX_OTHER >= ATRequest->prvk.kw_idx    )
                            {
                                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                        "AT*PRVP=%d,%d\r",ATRequest->prvp.action,
                                        ATRequest->prvk.kw_idx);
                            }
                            else
                            {
                                sF = FAIL;
                                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr unsup vkidx %d",
                                                                                         ATRequest->prvk.kw_idx);
                            }
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PRVK=%d\r",ATRequest->prvk.action);
                        }
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr unsup vkaction %d",
                                                                                 ATRequest->prvk.action);
                    }
                }
                break;
            }
            case AT_REQ_VT_TEST_PRESENCE_INDEX:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PTVP=%d\r",ATRequest->ptvp.pb_idx);
                break;
            }
            case AT_REQ_VT_TEST_PRESENCE_KEY:
            {
                if (AT_VTREC_KWIDX_GENERAL <= ATRequest->ptvk.kw_idx &&
                    AT_VTREC_KWIDX_OTHER >= ATRequest->ptvk.kw_idx    )
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PTVK=%d,%d\r",ATRequest->prvp.action,
                                ATRequest->ptvk.kw_idx);
                    }
                    else
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr PTVK unsup vkidx %d",
                                                                                 ATRequest->ptvk.kw_idx);
                    }
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PTVK=%d\r",ATRequest->ptvp.pb_idx);
                break;
            }
            case AT_REQ_VT_PLAYBACK_INDEX:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PPVP=%d\r",ATRequest->ppvp.pb_idx);
                break;
            }
            case AT_REQ_VT_PLAYBACK_KEY:
            {
                if (AT_VTREC_KWIDX_GENERAL <= ATRequest->ppvk.kw_idx &&
                    AT_VTREC_KWIDX_OTHER >= ATRequest->ppvk.kw_idx    )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PPVK=%d,%d\r",ATRequest->prvp.action,
                            ATRequest->ppvk.kw_idx);
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr PPVK unsup vkidx %d",
                                                                             ATRequest->ppvk.kw_idx);
                }
                break;
            }
            case AT_REQ_VT_DELETE_INDEX:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PDVP=%d\r",ATRequest->pdvp.pb_idx);
                break;
            }
            case AT_REQ_VT_DELETE_KEY:
            {
                if (AT_VTREC_KWIDX_GENERAL <= ATRequest->pdvk.kw_idx &&
                    AT_VTREC_KWIDX_OTHER >= ATRequest->pdvk.kw_idx    )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PDVK=%d,%d\r",ATRequest->prvp.action,
                            ATRequest->pdvk.kw_idx);
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr PDVK unsup vkidx %d",
                                                                             ATRequest->pdvk.kw_idx);
                }
                break;
            }
            case AT_REQ_VT_GET_NUMBER_TAGS:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PNVP\r");
                break;
            }
            /* command handling for RREC */
            case AT_REQ_SIVR_ENGINE_MGMT:
            {
                if (AT_TRUE == ATRequest->rrec.start_engine)
                {   /* start the engine */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*RREC=1\r");
                }
                else
                {   /* stop the engine */
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*RREC=0\r");
                }
                break;
            }
            /* command handling for RPRE */
            case AT_REQ_SIVR_PREPARE_VR:
            {
                if (AT_SIVR_MENU_STORENAME_L >= ATRequest->rpre.menu_id )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*RPRE=%d\r",ATRequest->rpre.menu_id);
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr unsupported menuid %d",
                                                                             ATRequest->rpre.menu_id);
                }
                break;
            }
            /* command handling for RPMT */
            case AT_REQ_SIVR_MANAGE_PROMTS:
            {   /* prepare the string related to the given prompt id and options */
                switch (ATRequest->rpmt.prmt.custom.prompt_type)
                {
                    case AT_SIVR_PROMPT_UNDEF:
                    {   /* this is only allowed to stop a prompt with specific command */
                        if (AT_FALSE == ATRequest->rpmt.start_play &&
                            0x0000   == ATRequest->rpmt.prmt.custom.prompt_id)
                        {
                            /* set char after text to \0 to avoid unterminated string */
                            ATRequest->rpmt.prmt.custom.text[
                                    ATRequest->rpmt.prmt.custom.text_len] = '\0';
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*RPMT=0,%d,0,0\r",
                                    ATRequest->rpmt.beep_after);
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdVcr prompt inco %d %d %d\r\n",
                                                                                     ATRequest->rpmt.start_play,
                                                                                     ATRequest->rpmt.beep_after,
                                                                                     ATRequest->rpmt.prmt.custom.prompt_type,0);
                        }
                        break;
                    }
                    case AT_SIVR_PROMPT_CUSTOM: /* text is mandatory */
                    {   /* check text len > 0 */
                        if (0 < ATRequest->rpmt.prmt.custom.text_len &&
                            AT_SIVR_MAX_PROMPT_TEXT_LEN >= ATRequest->rpmt.prmt.custom.text_len)
                        {
                            /* set char after text to \0 to avoid unterminated string */
                            ATRequest->rpmt.prmt.custom.text[
                                    ATRequest->rpmt.prmt.custom.text_len] = '\0';
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*RPMT=%d,%d,%d,'%s'\r",
                                    ATRequest->rpmt.start_play,
                                    ATRequest->rpmt.beep_after,
                                    ATRequest->rpmt.prmt.custom.prompt_type,
                                    ATRequest->rpmt.prmt.custom.text);
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr prompt oor %d\r\n",
                                                                                     ATRequest->rpmt.prmt.custom.prompt_type);
                        }
                        break;
                    }
                    /* check up to 4 optional params */
                    case AT_SIVR_PROMPT_MENU:
                    case AT_SIVR_PROMPT_HELP:
                    case AT_SIVR_PROMPT_SYSTEM:
                    {   /* check prompt id is in range */
                        if ((Uint16Type)AT_SIVR_PRMT_CK5050_MAXVALUE >= ATRequest->rpmt.prmt.other.prompt_id)
                        {   /* check optional parameters */
                            for (i=0;
                                     (i< ATRequest->rpmt.prmt.other.num_opt_avail &&
                                      i< AT_RPMT_MAX_NUM_OPT ); i++)
                            {   /* check text len and add string termination and if necessary modify num */
                                if (0 < ATRequest->rpmt.prmt.other.opt_len[i] &&
                                    AT_SIVR_MAX_PRMT_OPT_LEN >= ATRequest->rpmt.prmt.other.opt_len[i])
                                {
                                    ATRequest->rpmt.prmt.other.opt[i][
                                        ATRequest->rpmt.prmt.other.opt_len[i]] = '\0';
                                }
                                else
                                {   /* invalid length!!! remove all further params by modification of num */
                                    ATRequest->rpmt.prmt.other.num_opt_avail = i;
                                    DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr prompt opt inval %d %d",
                                                                                             i,
                                                                                             ATRequest->rpmt.prmt.other.opt_len[i]);
                                }
                            }
                            switch (ATRequest->rpmt.prmt.other.num_opt_avail)
                            {
                                case 0:
                                {
                                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*RPMT=%d,%d,%d,%d\r",
                                            ATRequest->rpmt.start_play, ATRequest->rpmt.beep_after,
                                            ATRequest->rpmt.prmt.other.prompt_type,
                                            ATRequest->rpmt.prmt.other.prompt_id);
                                    break;
                                }
                                case 1:
                                {
                                     (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*RPMT=%d,%d,%d,%d,'%s'\r",
                                            ATRequest->rpmt.start_play, ATRequest->rpmt.beep_after,
                                            ATRequest->rpmt.prmt.other.prompt_type,
                                            ATRequest->rpmt.prmt.other.prompt_id,
                                            ATRequest->rpmt.prmt.other.opt[0]);
                                    break;
                                }
                                case 2:
                                {
                                     (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*RPMT=%d,%d,%d,%d,'%s','%s'\r",
                                            ATRequest->rpmt.start_play, ATRequest->rpmt.beep_after,
                                            ATRequest->rpmt.prmt.other.prompt_type,
                                            ATRequest->rpmt.prmt.other.prompt_id,
                                            ATRequest->rpmt.prmt.other.opt[0],
                                            ATRequest->rpmt.prmt.other.opt[1]);
                                    break;
                                }
                                case 3:
                                {
                                     (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*RPMT=%d,%d,%d,%d,'%s','%s','%s'\r",
                                            ATRequest->rpmt.start_play, ATRequest->rpmt.beep_after,
                                            ATRequest->rpmt.prmt.other.prompt_type,
                                            ATRequest->rpmt.prmt.other.prompt_id,
                                            ATRequest->rpmt.prmt.other.opt[0],
                                            ATRequest->rpmt.prmt.other.opt[1],
                                            ATRequest->rpmt.prmt.other.opt[2]);
                                    break;
                                }
                                case 4:
                                {
                                     (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                            "AT*RPMT=%d,%d,%d,%d,'%s','%s','%s','%s'\r",
                                            ATRequest->rpmt.start_play, ATRequest->rpmt.beep_after,
                                            ATRequest->rpmt.prmt.other.prompt_type,
                                            ATRequest->rpmt.prmt.other.prompt_id,
                                            ATRequest->rpmt.prmt.other.opt[0],
                                            ATRequest->rpmt.prmt.other.opt[1],
                                            ATRequest->rpmt.prmt.other.opt[2],
                                            ATRequest->rpmt.prmt.other.opt[3]);
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr numopt oor %d",
                                                                                             ATRequest->rpmt.prmt.other.num_opt_avail);
                                }
                            }
                            // store pomptID for tracing
                            _VCR_gData.u16TxPromptID = ATRequest->rpmt.prmt.other.prompt_id;
                        }
                        else
                        {
                            sF = FAIL;
                            DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr prompt oor %d %d",
                                                                                     ATRequest->rpmt.prmt.other.prompt_type,
                                                                                     ATRequest->rpmt.prmt.other.prompt_id);
                        }
                        break;
                    }
                    default:
                    {
                        sF = FAIL;
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr unsupported prompt %d",
                                                                                 ATRequest->rpmt.prmt.other.prompt_type);
                        break;
                    }
                }
                break;
            }
            /* command handling for RGSL */
            case AT_REQ_SIVR_GET_SUPPORTED_LANG:
            {
                if (AT_TRUE == ATRequest->rgsl.is_get_lang)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*RGSL?\r");
                }
                else
                {
                    if (AT_FALSE == ATRequest->rgsl.index_used)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*RGSL\r");
                    }
                    else
                    {   /* range check, if index is invalid, use query without index */
                        if (AT_RGSL_MIN_START_INDEX <= ATRequest->rgsl.start_index &&
                            ATRequest->rgsl.start_index <= ATRequest->rgsl.stop_index &&
                            (Uint8Type)AT_RGSL_MAX_STOP_INDEX >= ATRequest->rgsl.stop_index)
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                    "AT*RGSL=%d,%d\r",
                                    ATRequest->rgsl.start_index,ATRequest->rgsl.stop_index);
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*RGSL\r");
                        }
                    }
                }
                break;
            }
            /* command handling for RSCL */
            case AT_REQ_SIVR_SET_CURRENT_LANG:
            {
                if (AT_TRUE == ATRequest->rscl.is_get_current_lang)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*RSCL?\r");
                }
                else
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*RSCL=%d\r",ATRequest->rscl.lang_index);
                }
                break;
            }
            /* command handling for RSRF */
            case AT_REQ_SIVR_SET_RESULT_FORMAT:
            {
                if (AT_TRUE == ATRequest->rsrf.is_get_result_format)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,"AT*RSRF?\r");
                }
                else
                {   /* check ranges */
                    if (AT_RSRF_MIN_RESULT <= ATRequest->rsrf.max_result &&
                        AT_RSRF_MAX_RESULT >= ATRequest->rsrf.max_result &&
                        AT_RSRF_MAX_REJECT_LEV >= ATRequest->rsrf.rejection_level)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*RSRF=%d,%d\r",ATRequest->rsrf.max_result,ATRequest->rsrf.rejection_level);
                    }
                    else
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr format oor %d %d",
                                                                                 ATRequest->rsrf.max_result,
                                                                                 ATRequest->rsrf.rejection_level);
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
                        _ATCMD_bPbRequestIsVCR = 0x01;
                    }
                    else
                    {
                        sF = FAIL;
                        DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "AT_Cmd_HandlerVCR ppbr idx error start_idx %d end_idx %d",
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
                    _ATCMD_bPbRequestIsVCR = 0x01;
                }
                else
                {
                    sF = FAIL;
                    DEBUG_VALUE4(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr pppb idx error %d %d %d",
                                                                             ATRequest->ppbp.pb_idx,
                                                                             ATRequest->ppbp.pn_idx1,
                                                                             ATRequest->ppbp.pn_idx2,0);
                }
                break;
            }
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
                            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "ATCmdVcr inco mode %d",
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
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "ATCmdVcr inco type %d",
                                                                                 ATRequest->ppbs.type);
                        break;
                    }
                }
                if (sF == SUCCESS)
                {
                    _ATCMD_bPbRequestIsVCR = 0x01;
                }
                break;
            }
            case AT_REQ_SIVR_MANAGE_VOICE_TAG:
            {
                switch (ATRequest->rvta.action)
                {
                    case  AT_VTMGR_ACT_RETRIEVE:
                    case  AT_VTMGR_ACT_STORE:
                    case  AT_VTMGR_ACT_DELETE:
                    {
                        if (AT_TRUE == ATRequest->rvta.type_avail)
                        {
                            switch (ATRequest->rvta.type)
                            {
                                case AT_SIVR_ITEMTYPE_CONTACT:
                                case AT_SIVR_ITEMTYPE_ARTIST:
                                case AT_SIVR_ITEMTYPE_ALBUM:
                                case AT_SIVR_ITEMTYPE_GENRE:
                                case AT_SIVR_ITEMTYPE_SONGTITLE:
                                case AT_SIVR_ITEMTYPE_PHONE:
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
                                    DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "ATCmdVcr inco type %d",
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
                    case AT_VTMGR_ACT_DELETE_ALL:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*RVTA=2,-1,2\r");
                        break;
                    }
                    default:
                    {
                        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "ATCmdVcr inco action %d",
                                                                                 ATRequest->rvta.action);
                        break;
                    }
                }
                break;
            }
            default:
                sF = FAIL;
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr unsupported type %d",
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
                /* reset in case sending failed */
                _ATCMD_bPbRequestIsVCR = 0x00;
                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr write err for msg %d len %d",
                                                                         ATRequest->common.req_type,
                                                                         cmdLen);
            }
        }
        else
        {
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr build err for msg %d",
                                                                   ATRequest->common.req_type);
        }
    }
    else
    {
        DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "ATCmdVcr res blocked");
    }
    return (sF);
}

/* End Of File ATCmdHandler_VCR.c */
