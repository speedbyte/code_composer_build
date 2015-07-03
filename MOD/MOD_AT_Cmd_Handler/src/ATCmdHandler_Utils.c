/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_Utils.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Utilities
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler Utilities provide check function
*                 and other AT Cmd Handler internal functionalities
*-----------------------------------------------------------------------------
* $Date: 2010-03-31 13:02:17 +0200 (Mi, 31 Mrz 2010) $
* $Rev: 11917 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Cmd_Handler/trunk/src/ATCmdHandler_Utils.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <string.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "UART.h"
#include "ATCmdHandler_CI.h"
#include "ATCmdHandler.h"
#include "ATCmdHandler_Utils.h"

#include "ATCmdHandler_Utils.id"
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
/* File local Code Macros                                                        */
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
/* Function    : _AT_CheckPhoneNumer                                          */
/**
    \brief      check telephone number given for processing

    \brief      check the request message for correct syntax / range of\n
                tel number \n

    \param      request from application including data
                ATReqCallNumberType

     \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/

SuccessFailType _AT_CheckPhoneNumer(ATReqCallNumberType  req)
{
    /* TODO fill function with correct checks */
    req = req; /* to avoid warnings from compiler */
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _AT_CheckDTMFValue                                          */
/**
    \brief      check DTMF value for correct range

    \brief      check the DTMF value  for correct range

    \param      value
                Uint8Type

     \return    SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _AT_CheckDTMFValue(Uint8Type  value)
{
    SuccessFailType sF = FAIL;

    switch (value)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case '*':
        case '#':
            sF = SUCCESS;
            break;
        default:
            DEBUG_VALUE1(AT_CMD_HANDLER_SWI_DBG, MOD_AT_CMD_HANDLER, "CheckDTFMValue inco DTMF value %d",
                                                                     value);
            break;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _AT_sFCmdReInitialiseUart                                    */
/**
    \brief      if UART overflow happened - reinit UART

    \return    SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
void _AT_sFCmdReInitialiseUart(const SCI_ST*  UartTxBuffer)
{
    SuccessFailType sF = SUCCESS;
    if ((Uint8Type)SciBufferOverflow == UART_u8GetBufferStatus( (SCI_ST*) UartTxBuffer, SciTransmit ) )
    {
        sF = UART_sfReInitialiseBuffer((SCI_ST*)UartTxBuffer, SciTransmit);   
        DEBUG_VALUE1(AT_CMD_HANDLER_SWI_ERR, MOD_AT_CMD_HANDLER, "ATCMD UART REINIT %d",
                                                                 sF);
    }
}



/* End Of File ATCmdHandler.c */
