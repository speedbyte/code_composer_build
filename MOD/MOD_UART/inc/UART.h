/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	UART.h
 * \brief	Module Header File. 
 * 			This file must be included by the application/system in order to use
 * 			the interfaces provided by this module. Detailed description found below. 
 * \brief	This file mainly contains:
 * 			1. prototypes of all the functions that can be used by other modules.
 * 			2. enum, structure, constant definition etc. that can be used by other 
 * 			modules.  
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-02-21 16:59:15 +0100 (Mo, 21 Feb 2011) $
* $Rev: 18078 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_UART/tags/MOD_UART_B_10.17_pre/inc/UART.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _UART_H
#define _UART_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "UART_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

typedef enum _SCIFUNCTIONS
{
    SciClock,
    SciReceive,
    SciTransmit,
    SciError
} SCIFUNCTIONS;

typedef enum _SCIQUAL
{
    SciFunctional,
    SciIoIn,
    SciIoOut,
    SciIoOutHigh,
    SciIoOutLow
} SCIQUAL;

typedef enum _SCIEXPT
{
    SciNoError,
    SciPending,
    SciOverrun,
    SciParity,
    SciFrame,
    SciBreak,
    SciBusBusy,
    SciTimeout,
    SciDmaError, 
    SciModeError
} SCIEXPT;

typedef enum _SCIBUFFEREXPT
{
	SciTxInit = 0,
	SciRxInit = 0,
	SciBufferOverflow = 1,
	SciReceiveError = 2
} SCIBUFFEREXPT;

/**
 \brief  To select  SCI1 Interface
*/
#define UART1 						(SCI_PST)TMS470R1_BASEADDRESS_SCI1
/**
 \brief  To select SCI2 Interface
*/
#define UART2 						(SCI_PST)TMS470R1_BASEADDRESS_SCI2
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef IUART_INIT /* BEGIN DECLARE INTERFACE(IUART_INIT) */

SuccessFailType UART_sfInit(SCI_ST *pstUart);  

/*----------------------------------------------------------------------------*/
#endif /* END DECLARE INTERFACE(IUART_INIT) */
/*----------------------------------------------------------------------------*/
#ifdef IUART_TRANSMIT

Uint16Type UART_u16WriteBlock(SCI_ST *pstUart, const Uint8Type *pau8Block, Uint8Type u8SizeOfBlock);  

SuccessFailType UART_sfWriteByte(SCI_ST *pstUart, Uint8Type u8Byte);  

#endif
/*----------------------------------------------------------------------------*/
#ifdef IUART_RECEIVE
Uint8Type UART_u8ReadByte(SCI_ST *pstUart);	

Uint8Type UART_u8RxGetCount(const SCI_ST *pstUart);

Uint8Type UART_u8ReadBlock(SCI_ST *pstUart, Uint8Type *applRecvBlock, Uint8Type length);
/*----------------------------------------------------------------------------*/
#endif
/*----------------------------------------------------------------------------*/
#ifdef IUART_CHECK

SuccessFailType UART_sfReInitialiseBuffer(const SCI_ST *pstUart, SCIFUNCTIONS u8Location);  

Uint8Type UART_u8GetBufferStatus(const SCI_ST *pstUart, SCIFUNCTIONS u8Location);
/*----------------------------------------------------------------------------*/
#endif
/*----------------------------------------------------------------------------*/

void UART_PrintStatistic(const SCI_ST *pstUart);

extern BooleanType UART_bGetUartHardwareErrorStatus(void);

#endif /* _UART_H */

/* End Of File UART.h */


