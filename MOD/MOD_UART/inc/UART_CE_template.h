/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	UART_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file mainly contains:
 * 			1. constants to switch on or off the exported interfaces. 
 * 			2. configurable constants.
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-03-17 14:37:26 +0100 (Do, 17 Mrz 2011) $
* $Rev: 18547 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_UART/tags/MOD_UART_B_10.17_pre/inc/UART_CE_template.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _UART_CE_H
#define _UART_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define IUART_INIT
#define IUART_TRANSMIT
#define IUART_RECEIVE
#define IUART_CHECK
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
	
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/*
 \brief	Baud rate of UART.
*/
#define UART_BAUD_RATE     				UART_BAUD_115200
// UART_BAUD_38400
// UART_BAUD_9600
/*
 \brief	size of UART RX ring Buffer. please dont make it 0
*/
#define UART1_SIZEOF_RX_RING_BUFFER      	10
/*
 \brief	size of UART TX ring Buffer. please dont make it 0
*/
#define UART1_SIZEOF_TX_RING_BUFFER      	1024
/*
 \brief	size of UART RX ring Buffer. please dont make it 0
*/
#define UART2_SIZEOF_RX_RING_BUFFER      	200
/*
 \brief	size of UART TX ring Buffer. please dont make it 0
*/
#define UART2_SIZEOF_TX_RING_BUFFER      	500

/*
 \brief	define this value, if the buffer status should be "automatically" set to
        ready after all the bytes in the buffer has been transmitted.
        In case it is not set, then the function UART_sfInitialiseBuffer has to be used
        to use the Uart functionality.
*/
#define UART_RESET_TXSTATUS_UART1_AFTER_BUFFER_EMPTY

/*
 \brief	define this value, if the buffer status should be "automatically" set to
        ready after all the bytes in the buffer has been transmitted.
        In case it is not set, then the function UART_sfInitialiseBuffer has to be used
        to use the Uart functionality.
*/
#undef UART_RESET_TXSTATUS_UART2_AFTER_BUFFER_EMPTY

#define UART_TRACE UART1

#define UART_PARROT UART2

#endif /* _UART_CE_H */

/* End Of File UART_CE.h */

