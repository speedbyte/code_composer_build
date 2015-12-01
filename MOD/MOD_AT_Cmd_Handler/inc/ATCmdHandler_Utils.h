/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_Utils.h
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Utilities
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler Utilities provide check function
*                 and other AT Cmd Handler internal functionalities
*-----------------------------------------------------------------------------
* $Date: 2008-11-28 09:48:03 +0100 (Fr, 28 Nov 2008) $
* $Rev: 3029 $
* $Author: schroeder $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Cmd_Handler/trunk/inc/ATCmdHandler_Utils.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATCMDHANDLER_UTILS_H
#define _ATCMDHANDLER_UTILS_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ATCmdHandler_CE.h"


/*----------------------------------------------------------------------------*/
/* Private Methods                                                            */
/*----------------------------------------------------------------------------*/

extern Uint8Type  _ATCMD_bPbRequestIsVCR;
extern SuccessFailType _AT_CheckPhoneNumer(ATReqCallNumberType  req);
extern void            _AT_sFCmdReInitialiseUart(const SCI_ST*  UartTxBuffer);
extern SuccessFailType _AT_CheckDTMFValue(Uint8Type  value);

#endif /* _ATCMDHANDLER_UTILS_H */

/* End Of File _ATCMDHANDLER_UTILS_H.h */
