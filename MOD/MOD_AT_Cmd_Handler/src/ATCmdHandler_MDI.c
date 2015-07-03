/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_MDI.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler MDI part
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

#include "ATCmdHandler_MDI.id"
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
/* Function    : AT_Cmd_Handler_MDI                                           */
/**
    \brief      Command handler for MDI

    \brief      check members of given data structure for proper processing \n
                of MDI application specific commands and build string for\n
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
SuccessFailType AT_Cmd_Handler_MDI(const ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type       cmdLen = 0;
    Uint8Type       cmdBuf[MAX_LEN_AT_COMMAND_BUFFER] = {0};

    if (SUCCESS == ERM_CheckUartOwnership(APP_SUB_MDI,(const SCI_ST*)UartTxBuffer))
    {
        switch (ATRequest->common.req_type)
        {
        	case AT_REQ_BT_SELECT_DEV:
            {   /* check given value in range */
                if ( (AT_DEVSEL_DISCONNECT <= ATRequest->psbd.value) &&
                     (AT_DEVSEL_CONNECT    >= ATRequest->psbd.value)   )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PSBD=P%d,%d\r",ATRequest->psbd.device_id + (Uint8Type)1U,
                        ATRequest->psbd.value);
                }
                else
                {
                    sF = FAIL;
                }
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
                                    ATRequest->pbsc.device_id + (Uint8Type)1U,
                                    ATRequest->pbsc.action,
                                    ATRequest->pbsc.service);
                        }
                        else
                        {
                            sF = FAIL;
                        }
                        break;
                    }
                    case AT_SERVICE_ACT_QUERY_SUPPORTED:
                    case AT_SERVICE_ACT_QUERY_CONNECTED:
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*PBSC=P%d,%d\r",
                                ATRequest->pbsc.device_id + (Uint8Type)1U,
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
            case AT_REQ_AS_KEY_EVENT_INDICATOR:
            {
                if ( (AT_REQ_KEY_PREVIOUS   <= ATRequest->pkev.key) &&
                     (AT_REQ_KEY_F_BACKWARD >= ATRequest->pkev.key) &&
                     (AT_REQ_KEYEVT_RELEASE <= ATRequest->pkev.event) &&
                     (AT_REQ_KEYEVT_PRESS   >= ATRequest->pkev.event))
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*PKEV=%d,%d\r", ATRequest->pkev.key,
                            ATRequest->pkev.event);
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_AS_GET_META_TYPE:
            {   /* only device id included - no check needed */
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PGMT=P%d\r", ATRequest->pgmt.device_id + (Uint8Type)1U);
                break;
            }
            case AT_REQ_AMGMT_SRD_ACTIVE_IND:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*PASA\r");
                break;
            }
            case AT_REQ_PLAYER_GET_STATUS:
            {
                /* NOTE: optional param extended info is filled all time */
                if (AT_DEVICE_BLUETOOTH == ATRequest->dgps.device_type)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*DGPS=P%d,%d\r",ATRequest->dgps.device_id + (Uint8Type)1U,
                        ATRequest->dgps.ext_info_requested);
                }
                else if ((Uint8Type)AT_DEVICE_PLAYER == (Uint8Type)ATRequest->dgps.device_type + (Uint8Type)1U)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*DGPS=D%d,%d\r",ATRequest->dgps.device_id + (Uint8Type)1U,
                        ATRequest->dgps.ext_info_requested);
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_PLAYER_PLAY_MODE:
            {
                if(AT_DEVICE_BLUETOOTH == ATRequest->dgps.device_type)
                {
                    if ( (AT_MDB_STOP_PLAYING     <= ATRequest->dply.action ) &&
                         (AT_MDB_F_REWIND_PLAYING >= ATRequest->dply.action )  )
                    {
                        if (AT_TRUE == ATRequest->dply.index_avail)
                        {   /* with index */
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DPLY=P%d,%d,%d\r",ATRequest->dply.device_id + (Uint8Type)1U,
                                ATRequest->dply.action,ATRequest->dply.index);
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DPLY=P%d,%d\r",ATRequest->dply.device_id + (Uint8Type)1U,
                                ATRequest->dply.action);
                        }
                    }
                    else
                    {
                        sF = FAIL;
                    }
                }
                else
                {
                    /* check action is in range */
                    if ( (AT_MDB_STOP_PLAYING     <= ATRequest->dply.action ) &&
                         (AT_MDB_F_REWIND_PLAYING >= ATRequest->dply.action )  )
                    {
                        if (AT_TRUE == ATRequest->dply.index_avail)
                        {   /* with index */
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DPLY=D%d,%d,%d\r",ATRequest->dply.mdb_id + (Uint8Type)1U,
                                ATRequest->dply.action,ATRequest->dply.index);
                        }
                        else
                        {
                            (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DPLY=D%d,%d\r",ATRequest->dply.mdb_id + (Uint8Type)1U,
                                ATRequest->dply.action);
                        }
                    }
                    else
                    {
                        sF = FAIL;
                    }
                }
                break;
            }
            case AT_REQ_PLAYER_SET_RANDOM_MODE:
            {
                if (AT_DEVICE_BLUETOOTH == ATRequest->dsra.device_type)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*DSRA=P%d,%d\r",ATRequest->dsra.device_id + (Uint8Type)1U,
                        ATRequest->dsra.random_enabled);
                }
                else if (AT_DEVICE_PLAYER == ATRequest->dsra.device_type)
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                        "AT*DSRA=D%d,%d\r",ATRequest->dsra.device_id + (Uint8Type)1U,
                        ATRequest->dsra.random_enabled);
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_PLAYER_NOTIFICATION:
            {   /* check mode is in given range */
                if ( (AT_AUTOMATIC_NOTIFICATION_DISABLED <= ATRequest->dcpn.automatic_notification) &&
                     (AT_AUTOMATIC_NOTIFICATION_ENABLED >= ATRequest->dcpn.automatic_notification) )
                {
                    (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                     "AT*DCPN=%d\r",ATRequest->dcpn.automatic_notification);
                }
                else
                {
                    sF = FAIL;
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
            case AT_REQ_PLAYER_SET_REPEAT_MODE:
            {   /* check mode is in given range */
                if ( (AT_PLAYER_REPEAT_DISABLED <= ATRequest->dsre.repeat_mode) &&
                     (AT_PLAYER_REPEAR_ALLTRACK >= ATRequest->dsre.repeat_mode) )
                {
                    if (AT_DEVICE_BLUETOOTH == ATRequest->dsre.device_type)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*DSRE=P%d,%d\r",ATRequest->dsre.device_id + (Uint8Type)1U,
                            ATRequest->dsre.repeat_mode);
                    }
                    else if (AT_DEVICE_PLAYER == ATRequest->dsre.device_type)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                            "AT*DSRE=D%d,%d\r",ATRequest->dsre.device_id + (Uint8Type)1U,
                            ATRequest->dsre.repeat_mode);
                    }
                    else
                    {
                        sF = FAIL;
                    }
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_DISCO_GET_ENTRY_DATA:
            {   /* build bitmask out of given enumerations */
                Uint8Type bitmask = 0;

                if (AT_TRUE == ATRequest->dged.filepath_requested)
                {
                    bitmask += 0x01;
                }
                if (AT_TRUE == ATRequest->dged.title_requested)
                {
                    bitmask += 0x02;
                }
                if (AT_TRUE == ATRequest->dged.artist_requested)
                {
                    bitmask += 0x04;
                }
                if (AT_TRUE == ATRequest->dged.album_requested)
                {
                    bitmask += 0x08;
                }
                if (AT_TRUE == ATRequest->dged.tracknumber_requested)
                {
                    bitmask += 0x10;
                }
                if (AT_TRUE == ATRequest->dged.genre_requested)
                {
                    bitmask += 0x20;
                }
                /* TODO check with tool how bit interpretation is passed to parrot */
                if (AT_DEVICE_BLUETOOTH == ATRequest->dged.device_type)
                {
                    if (AT_TRUE == ATRequest->dged.index_avail)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DGED=P%d,%d,%d\r",ATRequest->dged.device_id + (Uint8Type)1U,
                                bitmask,ATRequest->dged.index);
                    }
                    else
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DGED=P%d,%d\r",ATRequest->dged.device_id + (Uint8Type)1U,
                                bitmask);
                    }

                }
                else if (AT_DEVICE_PLAYER == ATRequest->dged.device_type)
                {
                    if (AT_TRUE == ATRequest->dged.index_avail)
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DGED=D%d,%d,%d\r",ATRequest->dged.device_id + (Uint8Type)1U,
                                bitmask,ATRequest->dged.index);
                    }
                    else
                    {
                        (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                                "AT*DGED=D%d,%d\r",ATRequest->dged.device_id + (Uint8Type)1U,
                                bitmask);
                    }
                }
                else
                {
                    sF = FAIL;
                }
                break;
            }
            case AT_REQ_BT_SERVICE_VISIBILITY:
            {
                (void)snprintf((char *)cmdBuf,MAX_LEN_AT_COMMAND_STRING,
                    "AT*PBSV=%d,%d\r",
                    ATRequest->pbsv.service,
                    ATRequest->pbsv.action);
                break;
            }
            default:
                sF = FAIL;
                DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlMDI unsupported type %d",
                                                                         ATRequest->common.req_type);

                break;
        }
        /* check if command is successful build together, if so call UART */
        if (SUCCESS == sF)
        {
            cmdLen = (Uint8Type)strlen((char *)cmdBuf);
            if (UART_u16WriteBlock((SCI_ST*)UartTxBuffer, cmdBuf, cmdLen) != cmdLen)
            {
                _AT_sFCmdReInitialiseUart((SCI_ST*)UartTxBuffer);
                sF = FAIL;
                DEBUG_VALUE2(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlMDI write err for msg %d len %d",
                                                                         ATRequest->common.req_type,cmdLen);
            }
        }
        else
        {
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlMDI build err for msg %d",
                                                                     ATRequest->common.req_type);
        }
    }
    else
    {
        DEBUG_TEXT(AT_CMD_HANDLER_SWI_WNG, MOD_AT_CMD_HANDLER, "CmdHdlMDI res blocked");
    }
    return (sF);
}

/* End Of File ATCmdHandler_MDI.c */
