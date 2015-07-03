/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	UART_PL.h
 * \brief	C File Header. 
 * 			This file must be included by the C File in order to use the 
 * 			constants, variables etc declared in this file. Detailed description found below. 
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-06-03 16:49:48 +0200 (Mi, 03 Jun 2009) $
* $Rev: 6457 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_UART/tags/MOD_UART_B_10.17_pre/inc/UART_PL.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _UART_PL_H
#define _UART_PL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "UART_CI.h"
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
typedef enum _SCICONFIGURATIONS
{
    SciOneStopBit = 0,
    SciTwoStopBit = 1,
    SciIdleLineMode = 0,
    SciAddressLineMode = 1,
    SciIsosyncTiming = 0,
    SciAsyncTiming = 1,
    SciExternalClock = 0,
    SciInternalClock = 1,
    SciOneChar = 0,
    SciTwoChar,
    SciThreeChar,
    SciFourChar,
    SciFiveChar,
    SciSixChar,
    SciSevenChar,
    SciEightChar
} SCICONFIGURATIONS;

/** 
\brief	Baud rate - 115200
*/
#define UART_BAUD_115200       				(Uint32Type)115200
/** 
\brief	Baud rate - 38400
*/
#define UART_BAUD_38400       				(Uint32Type)38400
/** 
\brief	Baud rate - 9600
*/
#define UART_BAUD_9600    				    (Uint32Type)9600
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#ifdef IUART_PL

typedef struct _UART_ST_BUFFERPROPERTIES
{
	Uint16Type sizeofRxBuffer;
	Uint16Type sizeofTxBuffer;
	/**
	 \brief  Transmit Ring Buffer 
	*/
	Uint8Type *txRingBuffer;
	/**
	 \brief  Shows the Status of the Transmit Ring Buffer 
	*/
	Uint8Type txStatus;
	/**
	 \brief  Distance between Txputptr and txgetptr ( incl Wrapping )\n
	         if they are together means the ring buffer has all the space.
	*/
	Uint16Type txGetCount;
	Uint16Type txPutCount;
	/**
	 \brief  Shows where the pointer locates in the Ring buffer.
	 		 It should not cross the boundary.
	*/
	Uint8Type *txPutPtr;
	Uint8Type BufferoverflowCount;
	/**
	 \brief  Here the byte willbe taken out from the Ring buffer from the Interrupt routine.
	*/
	Uint8Type *txGetPtr;
	Uint8Type *rxRingBuffer;
	Uint8Type rxStatus;
	Uint8Type rxGetCount;
	Uint8Type rxPutCount;
	Uint8Type *rxPutPtr;
	Uint8Type *rxGetPtr;
	Uint8Type bInitDone;
} UART_ST_BUFFERPROPERTIES, *UART_PST_BUFFERPROPERTIES;

/*----------------------------------------------------------------------------*/
/* Module Global Methods                                                      */
/*----------------------------------------------------------------------------*/

void UART_vResetEnter(SCI_ST *pstUart); 

void UART_vResetExit(SCI_ST *pstUart); 

void UART_vSetClockModeSource(SCI_ST *pstUart);  

void UART_vConfigureCommControlReg(SCI_ST *pstUart);  

void UART_vConfigureBaudRate(SCI_ST *pstUart, Uint32Type u16BaudRate);   

void UART_vConfigureInterrupt(SCI_ST *pstUart, SCIFUNCTIONS u8Location, Uint8Type u8Value); 

void UART_vPinModify(SCI_ST *pstUart, SCIFUNCTIONS PinID_N, SCIQUAL PinQual_N);

void UART_vHWTransmitByte(SCI_ST *pstUart, Uint8Type u8Byte);	

Uint8Type UART_u8HWReceiveByte(const SCI_ST *pstUart);   

Uint8Type UART_u8GetRxStatus(const SCI_ST *pstUart);		

void UART_vConfigurePeripheral(SCI_ST *pstUart, SCIFUNCTIONS u8Location, Uint8Type u8Value);	

void UART_vClearErrors(SCI_ST *pstUart);

#endif // IUART_PL

#endif /* _UART_PL_H */

/* End Of File UART_PL.h */



