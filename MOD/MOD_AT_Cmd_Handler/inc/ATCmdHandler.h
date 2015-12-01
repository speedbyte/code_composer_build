/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler.h
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler manage and built the different
                  message for the Parrot module.
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Cmd_Handler/trunk/inc/ATCmdHandler.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATCMDHANDLER_H
#define _ATCMDHANDLER_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ATCmdHandler_CE.h"
#include "ATCmdHandlerData.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Enumerations                                                        */
/*----------------------------------------------------------------------------*/

/* see ATCmdHandlerData.h                                                     */

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/* see ATCmdHandlerData.h                                                     */


/*----------------------------------------------------------------------------*/
/* Public Structures                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

extern BooleanType ATCMD_bPCheckPbRequestIsVCR(void);


/*----------------------------------------------------------------------------*/
/* Function    : AT_Cmd_Handler_Tel                                           */
/**
    \brief      Command handler for Tel application to jump in the
                diffent functions

    \brief      Select the correct state to call the function. \n
                The ID of the selected functions comes from the called \n
                function parameter. \n

    \param      pointer to request from application including data
                ATReqAllType
                all value allowed
    \param      Pointer to UartTxBuffer
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType AT_Cmd_Handler_Tel(ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer);

/*----------------------------------------------------------------------------*/
/* Function    : AT_Cmd_Handler_Device                                           */
/**
    \brief      Command handler for device application to jump in the
                diffent functions

    \brief      Select the correct state to call the function. \n
                The ID of the selected functions comes from the called \n
                function parameter. \n

    \param      pointer to request from application including data
                ATReqAllType
                all value allowed
    \param      Pointer to UartTxBuffer
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType AT_Cmd_Handler_Device(const ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer);




/*----------------------------------------------------------------------------*/
/* Function    : AT_Cmd_Handler_Phonebook                                     */
/**
    \brief      Command handler for Tel application to jump in the\n
                diffent functions

    \brief      Select the correct state to call the function. \n
                The ID of the selected functions comes from the called \n
                function parameter. \n

    \param      pointer to request from application including data
                ATReqAllType
                all value allowed
    \param      Pointer to UartTxBuffer
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType AT_Cmd_Handler_Phonebook(ATReqAllType   *ATRequest,
                                               const SCI_ST*  UartTxBuffer);

/*----------------------------------------------------------------------------*/
/* Function    : AT_Cmd_Handler_SMS                                           */
/**
    \brief      Command handler for Sms application to jump in the
                diffent functions

    \brief      Select the correct state to call the function. \n
                The ID of the selected functions comes from the called \n
                function parameter. \n

    \param      pointer to request from application including data
                ATReqAllType
                all value allowed
    \param      Pointer to UartTxBuffer
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType AT_Cmd_Handler_Sms(ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer);


/*----------------------------------------------------------------------------*/
/* Function    : AT_Cmd_Handler_ERM                                           */
/**
    \brief      Command handler for Resource Manager application to jump in the
                diffent functions

    \brief      Select the correct state to call the function. \n
                The ID of the selected functions comes from the called \n
                function parameter. \n

    \param      pointer to request from application including data
                ATReqAllType
                all value allowed
    \param      Pointer to UartTxBuffer
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType AT_Cmd_Handler_ERM(ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer);


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
extern SuccessFailType AT_Cmd_Handler_MDI(const ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer);


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
extern SuccessFailType AT_Cmd_Handler_VCR(ATReqAllType   *ATRequest,
                                          const SCI_ST*  UartTxBuffer);

#endif /* _ATCMDHANDLER_H */

/* End Of File ATCmdHandler.h */
