/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	UART_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-02-21 16:59:15 +0100 (Mo, 21 Feb 2011) $
* $Rev: 18078 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_UART/tags/MOD_UART_B_10.18_pre/src/UART_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

//#include"Debug.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "TRACE.h"
#include "UART_C1.id"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "UART_PL.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
#ifdef DO_DEBUG
/** 
	\brief	Global Variable to know how many bytes were actually read at the 
	UART1 Module.
*/
static Uint32Type totalcount_uart1 = 0U;
/** 
	\brief	Global Variable to know how many bytes were actually read at the
	UART2 Module.
*/
static Uint32Type totalcount_uart2 = 0U;
/** 
	\brief	Global Variable to know how many times the error occured on
	UART2 Module.
*/
static Uint32Type errorcount_uart2 = 0U;
/** 
	\brief	Global Variable to know how many times the error occured on
	UART1 Module.
*/
static Uint32Type errorcount_uart1 = 0U;

#endif
/** 
	\brief	Pointer to the structure for the Buffer Properties. 
*/
static UART_PST_BUFFERPROPERTIES UART_pstBufferProperties;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
	\brief	Buffer Properties for UART1 
*/
static UART_ST_BUFFERPROPERTIES UART_stBufferProperties1;
/** 
	\brief	Buffer Properties for UART2 
*/
static UART_ST_BUFFERPROPERTIES UART_stBufferProperties2;

static Uint8Type Tx_Buffer_for_UART1[UART1_SIZEOF_TX_RING_BUFFER];
static Uint8Type Tx_Buffer_for_UART2[UART2_SIZEOF_TX_RING_BUFFER];
static Uint8Type Rx_Buffer_for_UART1[UART1_SIZEOF_RX_RING_BUFFER];
static Uint8Type Rx_Buffer_for_UART2[UART2_SIZEOF_RX_RING_BUFFER];
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static UART_PST_BUFFERPROPERTIES _UART_pstGetBufferIndex(const SCI_ST *pstUart);
/*----------------------------------------------------------------------------*/
static SuccessFailType 	_UART_sfTxPutByte(const SCI_ST *pstUart, Uint8Type dasByte);
/*----------------------------------------------------------------------------*/
static Uint8Type 		_UART_u8RxGetByte(const SCI_ST *pstUart);
/*----------------------------------------------------------------------------*/
static void 			_UART_vSendFirstByte(SCI_ST *pstUart);
/*----------------------------------------------------------------------------*/
static SuccessFailType UART_sfInitialiseBuffer(const SCI_ST *pstUart, SCIFUNCTIONS u8Location);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function    : _UART_pstGetBufferIndex                                      */
/** 
    \brief      Get Buffer Index
  
    \brief      Description:
                This function returns the Buffer Index depending on which UART 
                selected

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     UART_PST_BUFFERPROPERTIES
                Index to the Buffer Properties
                UART_stBufferProperties1, UART_stBufferProperties2
*/
/*----------------------------------------------------------------------------*/
static UART_PST_BUFFERPROPERTIES _UART_pstGetBufferIndex(const SCI_ST *pstUart)
{
	/*TODO use terneray operator to select the Buffer */
	if ( pstUart == UART1)
	{
		return (&UART_stBufferProperties1);
	}
	else if ( pstUart == (SCI_PST)UART2)
	{
		return (&UART_stBufferProperties2);
	}
	else
		return (UART_PST_BUFFERPROPERTIES)FAIL;
}
/*----------------------------------------------------------------------------*/
/* Function    : _UART_sfTxPutByte                                            */
/** 
    \brief      Put Byte in Tx ring Buffer
  
    \brief      Description:
                Putting the byte in the Tx ring Buffer and incrementing the
                Putptr for the Ring Buffer and then wrap around.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \param      dasByte
    			Uint8Type
                Value between 0 and 255 
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _UART_sfTxPutByte(const SCI_ST *pstUart, Uint8Type dasByte)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type *temp;
    UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
    /*------------------------------------------------------------------------*/
    /* Buffered I/O is running so we write to the ring buffer.                */
    /*------------------------------------------------------------------------*/
    /* Is there room in the buffer?                                           */
    if (
    	(UART_pstBufferProperties->txPutCount
    			- UART_pstBufferProperties->txGetCount) 
    		< (UART_pstBufferProperties->sizeofTxBuffer)
        )
    {
        /* Yes, so write the byte then update the put pointer wrapping        */
        /* it if we reached the end back to the beginning of the buffer       */
    	temp = (UART_pstBufferProperties->txRingBuffer) + (UART_pstBufferProperties->sizeofTxBuffer);
        if (UART_pstBufferProperties->txPutPtr >= (temp))
        {
        	UART_pstBufferProperties->txPutPtr = UART_pstBufferProperties->txRingBuffer;
        }
    	*(UART_pstBufferProperties->txPutPtr) = dasByte;
    	UART_pstBufferProperties->txPutPtr++;
        /* Must increment the count                                        */
        UART_pstBufferProperties->txPutCount++;
    }
    else
    {
        /* Record error                                                    */
        sF = FAIL;
        UART_pstBufferProperties->txStatus = (Uint8Type)SciBufferOverflow;
        UART_pstBufferProperties->BufferoverflowCount++;
    } 
    return(sF);
}
/*----------------------------------------------------------------------------*/
/* Function    : _UART_u8RxGetByte                                            */
/** 
    \brief      Get Byte from Rx ring Buffer
  
    \brief      Description:
                Getting the byte from the Rx ring Buffer and incrementing the
                Getptr for the Ring Buffer and then wrap around.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     Uint8Type
                The Byte which was read from the ring buffer.
                Value btw 0 and 255.
*/
/*----------------------------------------------------------------------------*/
static Uint8Type _UART_u8RxGetByte(const SCI_ST *pstUart)
{
	Uint8Type dasByte = 0U;
	Uint8Type *temp;
	UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
	
    /*----------------------------------------------------------------------*/
    /* Call UartRxGetCount to ensure that there is a byte available         */
    /* before calling this method                                           */ 
    /*----------------------------------------------------------------------*/ 
    /* We have to check the count to make sure a byte is available           */
    if (abs(UART_pstBufferProperties->rxPutCount - UART_pstBufferProperties->rxGetCount ) != 0)
    {
    	/* Interrupts off, get the byte, decrement the count update the get  */
        /* pointer and wrap to the beginning if at the end of the buffer     */
        /* then turn interrupts back on.                                     */
    	//UART_vConfigureInterrupt(pstUart, SciReceive, DISABLE);
    	temp = UART_pstBufferProperties->rxRingBuffer + (UART_pstBufferProperties->sizeofRxBuffer);
    	if (UART_pstBufferProperties->rxGetPtr >= (temp))
        {
        	UART_pstBufferProperties->rxGetPtr = UART_pstBufferProperties->rxRingBuffer;
        }
        dasByte = *UART_pstBufferProperties->rxGetPtr;
        UART_pstBufferProperties->rxGetCount++;
        UART_pstBufferProperties->rxGetPtr++;
    }
    return(dasByte);
}
/*----------------------------------------------------------------------------*/
/* Function    : _UART_vSendFirstByte                                         */
/** 
    \brief      Send the First Byte from the Transmit Buffer. 
  
    \brief      Description:
                Since the sequence for the transmission of the byte is that first 
                the byte should be written to the HW Transmit Buffer and then 
                the interrupt should be enabled, it is a Chicken and Egg Problem 
                To solve it the "very" first Byte is written manually and the rest 
                is then taken care by the interrupt routine.
                This function is valid only when the Tx lines are idle.

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _UART_vSendFirstByte(SCI_ST *pstUart)
{
	// .
	Uint8Type *temp;
	if ( pstUart->ctl2_UN.ctl2_ST.TxEmpty_B1 == 1)
	{
		UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
		UART_vConfigureInterrupt(pstUart, SciTransmit, DISABLE);
	    /*----------------------------------------------------------------------*/
	    /* Since the Uart2TxISR is the only method to get a byte from the Tx    */
	    /* ring buffer, all processing is done.                                 */
	    /*----------------------------------------------------------------------*/
	    /* If the count is NOT zero there is buffered I/O                       */
		if ((UART_pstBufferProperties->txPutCount - UART_pstBufferProperties->txGetCount) != 0)
	    {
	        /* Get the byte then transmit it then update the get pointer        */
			UART_vHWTransmitByte(pstUart, *UART_pstBufferProperties->txGetPtr);
			UART_pstBufferProperties->txGetPtr++;
			temp = UART_pstBufferProperties->txRingBuffer + (UART_pstBufferProperties->sizeofTxBuffer);
	        /* Is it time to wrap the buffer?                                   */
	        if (UART_pstBufferProperties->txGetPtr >= (temp))
	        {
	        	UART_pstBufferProperties->txGetPtr = UART_pstBufferProperties->txRingBuffer;
	        }
	        /* Decrement the count                                              */
	        UART_pstBufferProperties->txGetCount++;
	        UART_vConfigureInterrupt(pstUart, SciTransmit, ENABLE);		
		}
    }
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_sfInitialiseBuffer                                         */
/** 
    \brief      Reinitialise or Initialise the buffer.
  
    \brief      Description:
                Resets the Buffer Properties with value = 0 

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \param      u8Location
    			SCIFUNCTIONS
                SciClock,SciReceive,SciTransmit,SciError
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType UART_sfInitialiseBuffer(const SCI_ST *pstUart, SCIFUNCTIONS u8Location)
{
	Uint32Type i = 0U;
	UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
	if ((UART_PST_BUFFERPROPERTIES)FAIL==UART_pstBufferProperties)
		return FAIL;
	if ( u8Location == SciReceive )
	{
		if ( pstUart == (SCI_PST) UART1)
		{
			UART_pstBufferProperties->sizeofRxBuffer = UART1_SIZEOF_RX_RING_BUFFER;
			UART_pstBufferProperties->rxRingBuffer = Rx_Buffer_for_UART1;
		}
		else if ( pstUart == (SCI_PST)UART2)
		{
			UART_pstBufferProperties->sizeofRxBuffer = UART2_SIZEOF_RX_RING_BUFFER;
			UART_pstBufferProperties->rxRingBuffer = Rx_Buffer_for_UART2;
		}
		
		UART_pstBufferProperties->rxStatus = 0U;
		UART_pstBufferProperties->rxGetCount = 0U;
		UART_pstBufferProperties->rxPutCount = 0U;
		UART_pstBufferProperties->rxPutPtr = &(UART_pstBufferProperties->rxRingBuffer)[0];
		UART_pstBufferProperties->rxGetPtr = &(UART_pstBufferProperties->rxRingBuffer)[0];
		for ( i = 0; i < (UART_pstBufferProperties->sizeofRxBuffer); i++)
			UART_pstBufferProperties->rxRingBuffer[i] = 0;
	}
	else if ( u8Location == SciTransmit )
	{
		if ( pstUart == (SCI_PST) UART1)
		{
			UART_pstBufferProperties->sizeofTxBuffer = UART1_SIZEOF_TX_RING_BUFFER;
			UART_pstBufferProperties->txRingBuffer = Tx_Buffer_for_UART1;
		}
		else if ( pstUart == (SCI_PST)UART2)
		{
			UART_pstBufferProperties->sizeofTxBuffer = UART2_SIZEOF_TX_RING_BUFFER;
			UART_pstBufferProperties->txRingBuffer = Tx_Buffer_for_UART2;
		}

		UART_pstBufferProperties->txStatus = 0U;
		UART_pstBufferProperties->txPutCount = 0U;
		UART_pstBufferProperties->txGetCount = 0U;
		UART_pstBufferProperties->txPutPtr = &(UART_pstBufferProperties->txRingBuffer)[0];
		UART_pstBufferProperties->txGetPtr = &(UART_pstBufferProperties->txRingBuffer)[0];
		UART_pstBufferProperties->BufferoverflowCount = 0;
		for ( i = 0; i < (UART_pstBufferProperties->sizeofTxBuffer); i++)
			UART_pstBufferProperties->txRingBuffer[i] = 0;
	}
	UART_pstBufferProperties->bInitDone = (Uint8Type)BT_TRUE;
	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : UART_sfReInitialiseBuffer                                         */
/** 
    \brief      Reinitialise or Initialise the buffer.
  
    \brief      Description:
                Resets the Buffer Properties with value = 0 

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \param      u8Location
    			SCIFUNCTIONS
                SciClock,SciReceive,SciTransmit,SciError
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType UART_sfReInitialiseBuffer(const SCI_ST *pstUart, SCIFUNCTIONS u8Location)
{
	Uint32Type i = 0U;
	UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
	if ((UART_PST_BUFFERPROPERTIES)FAIL==UART_pstBufferProperties)
		return FAIL;
	if ( u8Location == SciReceive )
	{
		UART_pstBufferProperties->rxStatus = 0U;
		UART_pstBufferProperties->rxGetCount = 0U;
		UART_pstBufferProperties->rxPutCount = 0U;
		UART_pstBufferProperties->rxPutPtr = &(UART_pstBufferProperties->rxRingBuffer)[0];
		UART_pstBufferProperties->rxGetPtr = &(UART_pstBufferProperties->rxRingBuffer)[0];
		for ( i = 0; i < (UART_pstBufferProperties->sizeofRxBuffer); i++)
			UART_pstBufferProperties->rxRingBuffer[i] = 0;
	}
	else if ( u8Location == SciTransmit )
	{
		UART_pstBufferProperties->txStatus = 0U;
		UART_pstBufferProperties->txPutCount = 0U;
		UART_pstBufferProperties->txGetCount = 0U;
		UART_pstBufferProperties->txPutPtr = &(UART_pstBufferProperties->txRingBuffer)[0];
		UART_pstBufferProperties->txGetPtr = &(UART_pstBufferProperties->txRingBuffer)[0];;
		for ( i = 0; i < (UART_pstBufferProperties->sizeofTxBuffer); i++)
			UART_pstBufferProperties->txRingBuffer[i] = 0;
	}
	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : UART_u8GetBufferStatus                                     */
/** 
    \brief      Tx Ring Buffer Status
  
    \brief      Description:
                This function returns what is the current bufferstatus of the 
                UART Transmit Buffer is

    \param      pstUart
    			Pointer to the Peripheral
                UART1 / UART2 
    \param      u8Location
    			SCIFUNCTIONS
                SciClock,SciReceive,SciTransmit,SciError                
    \return     Uint8Type
                Buffer Status.
                0 = OK, 1 = Buffer Overflow, 2 = Hardware Error.
*/
/*----------------------------------------------------------------------------*/
Uint8Type UART_u8GetBufferStatus(const SCI_ST *pstUart, SCIFUNCTIONS u8Location)
{
    /*----------------------------------------------------------------------*/
    /* Buffer overflow is indicative of loss of synchrization or glitches   */
    /* A parity error might be just a glitch.                               */
    /* Remember the Uart driver only deals with data link errors.           */
    /* The application needs to deal with application message problems like */
    /* bad message checksum (transport layer issue) and loss-of-synch due   */
    /* to dropped bytes on the link or detected link errors.                */
    /*----------------------------------------------------------------------*/
	Uint8Type status = 0U;
	UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
	if ((UART_PST_BUFFERPROPERTIES)FAIL==UART_pstBufferProperties)
		return (Uint8Type)FAIL;
	if ( u8Location == SciReceive )
	{
		status = UART_pstBufferProperties->rxStatus;
	}
	else /* ( u8Location == SciTransmit ) */
	{
		status = UART_pstBufferProperties->txStatus;
	}
	return(status);
}

/*----------------------------------------------------------------------------*/
/* Function    : UART_sfInit                                         */
/** 
    \brief      Initialise the UART
  
    \brief      Description:
                Initialise the UART to a correct state. 

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType UART_sfInit(SCI_ST *pstUart)
{
	UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
	UART_pstBufferProperties->bInitDone = (Uint8Type)BT_FALSE;
	UART_vResetEnter(pstUart);  
	UART_vSetClockModeSource(pstUart);
	UART_vConfigureCommControlReg(pstUart);
	UART_vConfigureBaudRate(pstUart, UART_BAUD_RATE); 
    // To use transmit interrupt functionality, TX_DMA_ENA must be cleared
    // To use receive interrutp functionalit, RX_DMA_ENA must be cleared and RX_DMA_ALL, RX_DMA_ENA must be cleared
    UART_vConfigureInterrupt(pstUart, SciReceive, ENABLE);
    UART_vConfigureInterrupt(pstUart, SciTransmit, DISABLE);
    UART_vConfigureInterrupt(pstUart, SciError, ENABLE);
    // UART_sfConfigureDma(pstUart, (TXDMADIS|RXDMADIS|RXDMAALLDIS));
    UART_vConfigurePeripheral(pstUart, SciReceive, ENABLE);
    UART_vConfigurePeripheral(pstUart, SciTransmit, ENABLE);

    UART_INIT

    UART_vResetExit(pstUart);
    /*----------------------------------------------------------------------*/
    /* Initialize the ring buffers                                          */
    /*----------------------------------------------------------------------*/    
    (void)UART_sfInitialiseBuffer(pstUart, SciReceive);
    (void)UART_sfInitialiseBuffer(pstUart, SciTransmit);
    return(SUCCESS);
}


/*----------------------------------------------------------------------------*/
/* Function    : UART_u16WriteBlock                                         */
/** 
    \brief      Send a block of bytes on the UART.
  
    \brief      Description:
                This function puts the application block on the TX Ring buffer
                and increases the Putptr. 

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \param      block
    			Uint8Type *
                Contains the address of the application buffer.
    \param      sizeofBlock
    			Uint8Type
                No of iterations to be done.
    \return     Uint16Type
                Returns the no of bytes which were actually written to the 
                Tx Ring buffer. In Success case, it should be the same as the 
                Application has requested.
*/
/*----------------------------------------------------------------------------*/
Uint16Type UART_u16WriteBlock(SCI_ST *pstUart, const Uint8Type *block, Uint8Type sizeofBlock)
{
    Uint8Type count = sizeofBlock;
    SuccessFailType sF = SUCCESS;
    UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
    /*----------------------------------------------------------------------*/
    /* As long as we do not fail because we overflowed the buffer, we write */
    /* the block of bytes. The return value is the number of bytes written  */
    /* in case the caller needs to know there was an error.                 */
    /*----------------------------------------------------------------------*/
    if (UART_pstBufferProperties->txStatus != 0)
    	return 0;
    while (count && (sF == SUCCESS) )
    {
        sF = _UART_sfTxPutByte(pstUart, *block);
        if (sF == SUCCESS)
        {
            block++;
            count--;
        }
        else
        {
        	return (sizeofBlock - count);
        }
    }
    _UART_vSendFirstByte(pstUart);
    return(sizeofBlock - count);
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_sfWriteByte                                         */
/** 
    \brief      Send a byte on the UART.
  
    \brief      Description:
                This function puts the byte on the TX Ring buffer
                and increases the Putptr. 

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \param      u8Byte
    			Uint8Type
                Value is between 0 and 255
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType UART_sfWriteByte(SCI_ST *pstUart, Uint8Type u8Byte)
{
    SuccessFailType sF = SUCCESS;
    sF = _UART_sfTxPutByte(pstUart, u8Byte);
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : UART_u8ReadByte                                         */
/** 
    \brief      Read a byte from the Rx ring Buffer.
  
    \brief      Description:
                This function gets a byte from the RX Ring buffer
                and increases the Getptr. 

    \param      pstUart
    			SCI_ST
                UART1 / UART2 
    \return     Uint8Type
                The Byte which was read from the ring buffer.
                Value btw 0 and 255.
*/
/*----------------------------------------------------------------------------*/
Uint8Type UART_u8ReadByte(SCI_ST *pstUart)
{
    return(_UART_u8RxGetByte(pstUart));
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_u8ReadBlock                                         */
/** 
    \brief      Read a block from the Rx ring Buffer.
  
    \brief      Description:
                This function gets a block from the RX Ring buffer
                and increases the Getptr. 

    \param      pstUart
    			SCI_ST
                UART1 / UART2
    \param      applRecvBlock
    			Uint8Type *
                Contains the address of the application buffer.
    \param      sizeofBlock
    			Uint8Type
                No of iterations to be done.
    \return     Uint8Type
                The Byte which was read from the ring buffer.
                In Success Case it should be either, size of block or the count
                value received from the Rx ring buffer at the time this function 
                was invoked.
*/
/*----------------------------------------------------------------------------*/
Uint8Type UART_u8ReadBlock(SCI_ST *pstUart, Uint8Type *applRecvBlock, Uint8Type sizeofBlock)
{
	Uint8Type iterations = 0U;
	Uint8Type Rxcount = UART_pstBufferProperties->rxGetCount; 
	while (( Rxcount-- != 0) && (iterations != sizeofBlock))
	{
		*applRecvBlock++= _UART_u8RxGetByte(pstUart);
		iterations++;
	}
	return iterations;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_u8RxGetCount                                        	  */
/** 
    \brief      Returns the number of unread bytes from the Rx ring Buffer.
  
    \brief      Description:
                This function returns the number of unread bytes i.e 
                a difference between the PutCount and the GetCount.

    \param      pstUart
    			SCI_ST
                UART1 / UART2
    \return     Uint8Type
                No of unread bytes in Rx Ring buffer.
*/
/*----------------------------------------------------------------------------*/
Uint8Type UART_u8RxGetCount(const SCI_ST *pstUart)
{
    Uint8Type temp;
    UART_pstBufferProperties = _UART_pstGetBufferIndex(pstUart);
    /*----------------------------------------------------------------------*/
    /* This allows the caller who wants to get bytes from the buffer know   */
    /* if there any available rather than handle a zero count error.        */    
    /*----------------------------------------------------------------------*/
    temp = (UART_pstBufferProperties->rxPutCount - UART_pstBufferProperties->rxGetCount );
    return(temp);
}

/*----------------------------------------------------------------------------*/
/* Function    : UART1_TxISR                                     */
/** 
    \brief      ISR for UART1 Transmit. Its a Type1 Interrupt.
  
    \brief      Description:
                This function is called when the TxRdy Pin goes from Low to 
                High, the Transmit interrupt for UART1 is enabled and 
                corresponding bit in ISR mask register is enabled.

    \return     void
*/
/*----------------------------------------------------------------------------*/
#pragma INTERRUPT(UART1_TxISR, IRQ)
void UART1_TxISR(void)
{
	Uint8Type *temp;
	UART_vConfigureInterrupt(UART1, SciTransmit, DISABLE);
    /*----------------------------------------------------------------------*/
    /* Since the Uart2TxISR is the only method to get a byte from the Tx    */
    /* ring buffer, all processing is done.                                 */
    /*----------------------------------------------------------------------*/
    /* If the count is NOT zero there is buffered I/O                       */
	if ((UART_stBufferProperties1.txPutCount - UART_stBufferProperties1.txGetCount)!= 0)
    {
        /* Get the byte then transmit it then update the get pointer        */
		UART_vHWTransmitByte(UART1, *UART_stBufferProperties1.txGetPtr);
		UART_stBufferProperties1.txGetPtr++;
		temp = UART_stBufferProperties1.txRingBuffer + (UART_stBufferProperties1.sizeofTxBuffer);
        /* Is it time to wrap the buffer?                                   */
        if (UART_stBufferProperties1.txGetPtr >= (temp))
        {
        	UART_stBufferProperties1.txGetPtr = UART_stBufferProperties1.txRingBuffer;
        }
        /* Decrement the count                                              */
        UART_stBufferProperties1.txGetCount++;
        UART_vConfigureInterrupt(UART1, SciTransmit, ENABLE);
    }
    else
    {
        /* Since there are no more bytes, disable the transmitter otherwise */
        /* TDRE will cause interrupts until we again load TDR2              */
#ifdef UART_RESET_TXSTATUS_UART1_AFTER_BUFFER_EMPTY
    	UART_stBufferProperties1.txStatus = 0;  // Restart the buffer again when all bytes are completely send.
#endif
    	UART_vConfigureInterrupt(UART1, SciTransmit, DISABLE);  
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : UART1_RxISR                                     */
/** 
    \brief      ISR for UART1 Receive. Its a Type1 Interrupt.
  
    \brief      Description:
                This function is called when the RxRdy Pin goes from Low to 
                High, the Receive interrupt for UART1 is enabled and 
                corresponding bit in ISR mask register is enabled.

    \return     void
*/
/*----------------------------------------------------------------------------*/
#pragma INTERRUPT(UART1_RxISR, IRQ)
void UART1_RxISR(void)
{
    Uint8Type dasByte;
    Uint8Type *temp;
	if (UART_stBufferProperties1.bInitDone == (Uint8Type)BT_TRUE )
		{
		UART_vConfigureInterrupt(UART1, SciReceive, DISABLE);
	    /*----------------------------------------------------------------------*/
	    /* Since the Uart2RxISR is the only method to put a byte in the Rx      */
	    /* ring buffer, all processing is done.                                 */
	    /*----------------------------------------------------------------------*/
	    /* Check for parity, overrun and framing errors                         */
	#ifdef DO_DEBUG
		totalcount_uart1++;
	#endif
		/* Read the received byte which automatically clear the interrupt   */
		dasByte = UART_u8HWReceiveByte(UART1);
	    /* Is it time to wrap the buffer?                                   */
	    temp = UART_stBufferProperties1.rxRingBuffer + (UART_stBufferProperties1.sizeofRxBuffer);
	    if (UART_stBufferProperties1.rxPutPtr >= (temp))
	    {
	    	UART_stBufferProperties1.rxPutPtr = UART_stBufferProperties1.rxRingBuffer;
	    }
	    /* Put the byte into the ring buffer                                */
	    *UART_stBufferProperties1.rxPutPtr = dasByte;
	    UART_stBufferProperties1.rxPutPtr++;
	    /* If we overflowed (overwrote) bytes that have not been read we    */
	    /* flag the condition. *** Note that we keep the recent data.       */
	    if ((UART_stBufferProperties1.rxPutCount - UART_stBufferProperties1.rxGetCount) >= (UART_stBufferProperties1.sizeofRxBuffer))
	    {
	    	UART_stBufferProperties1.rxStatus = (Uint8Type)SciBufferOverflow;
	    }
	    else
	    {
	   		/* It's okay to increment the count                             */
	    	UART_stBufferProperties1.rxPutCount++;
	    }
		UART_vConfigureInterrupt(UART1, SciReceive, ENABLE);
		}
}

/*----------------------------------------------------------------------------*/
/* Function    : UART2_TxISR                                     */
/** 
    \brief      ISR for UART2 Transmit. Its a Type1 Interrupt.
  
    \brief      Description:
                This function is called when the TxRdy Pin goes from Low to 
                High, the Transmit interrupt for UART1 is enabled and 
                corresponding bit in ISR mask register is enabled.

    \return     void
*/
/*----------------------------------------------------------------------------*/
#pragma INTERRUPT(UART2_TxISR, IRQ)
void UART2_TxISR(void)
{
	Uint8Type *temp;
	UART_vConfigureInterrupt(UART2, SciTransmit, DISABLE);
    /*----------------------------------------------------------------------*/
    /* Since the Uart2TxISR is the only method to get a byte from the Tx    */
    /* ring buffer, all processing is done.                                 */
    /*----------------------------------------------------------------------*/
    /* If the count is NOT zero there is buffered I/O                       */
	if ((UART_stBufferProperties2.txPutCount - UART_stBufferProperties2.txGetCount )!= 0)
    {
        /* Get the byte then transmit it then update the get pointer        */
		UART_vHWTransmitByte(UART2, *UART_stBufferProperties2.txGetPtr);
		UART_stBufferProperties2.txGetPtr++;
		temp = UART_stBufferProperties2.txRingBuffer + (UART_stBufferProperties2.sizeofTxBuffer);
        /* Is it time to wrap the buffer?                                   */
        if (UART_stBufferProperties2.txGetPtr >= (temp))
        {
        	UART_stBufferProperties2.txGetPtr = UART_stBufferProperties2.txRingBuffer;
        }
        /* Decrement the count                                              */
        UART_stBufferProperties2.txGetCount++;
        UART_vConfigureInterrupt(UART2, SciTransmit, ENABLE);
    }
    else
    {
        /* Since there are no more bytes, disable the transmitter otherwise */
        /* TDRE will cause interrupts until we again load TDR2              */
#ifdef UART_RESET_TXSTATUS_UART2_AFTER_BUFFER_EMPTY
    	UART_stBufferProperties2.txStatus = 0;  // Restart the buffer again when all bytes are completely send.
#endif
    	UART_vConfigureInterrupt(UART2, SciTransmit, DISABLE);  
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : UART1_RxISR                                     */
/** 
    \brief      ISR for UART2 Receive. Its a Type1 Interrupt.
  
    \brief      Description:
                This function is called when the RxRdy Pin goes from Low to 
                High, the Receive interrupt for UART1 is enabled and 
                corresponding bit in ISR mask register is enabled.

    \return     void
*/
/*----------------------------------------------------------------------------*/
#pragma INTERRUPT(UART2_RxISR, IRQ)
void UART2_RxISR(void)
{
    Uint8Type dasByte;
    Uint8Type *temp;
	if (UART_stBufferProperties2.bInitDone == (Uint8Type)BT_TRUE )
		{
		UART_vConfigureInterrupt(UART2, SciReceive, DISABLE);
	    /*----------------------------------------------------------------------*/
	    /* Since the Uart2RxISR is the only method to put a byte in the Rx      */
	    /* ring buffer, all processing is done.                                 */
	    /*----------------------------------------------------------------------*/
	    /* Check for parity, overrun and framing errors                         */
	#ifdef DO_DEBUG
		totalcount_uart2++;
	#endif
		/* Read the received byte which automatically clear the interrupt   */
		dasByte = UART_u8HWReceiveByte(UART2);
	    /* Is it time to wrap the buffer?                                   */
	    temp = UART_stBufferProperties2.rxRingBuffer + (UART_stBufferProperties2.sizeofRxBuffer);
	    if (UART_stBufferProperties2.rxPutPtr >= (temp))
	    {
	    	UART_stBufferProperties2.rxPutPtr = UART_stBufferProperties2.rxRingBuffer;
	    }
	    /* Put the byte into the ring buffer                                */
	    *UART_stBufferProperties2.rxPutPtr = dasByte;
	    UART_stBufferProperties2.rxPutPtr++;
	    /* If we overflowed (overwrote) bytes that have not been read we    */
	    /* flag the condition. *** Note that we keep the recent data.       */
	    if ((UART_stBufferProperties2.rxPutCount - UART_stBufferProperties2.rxGetCount) >= (UART_stBufferProperties2.sizeofRxBuffer))
	    {
	    	UART_stBufferProperties2.rxStatus = (Uint8Type)SciBufferOverflow;
	    }
	    else
	    {
	        /* It's okay to increment the count                             */
	    	UART_stBufferProperties2.rxPutCount++;
	    }
		UART_vConfigureInterrupt(UART2, SciReceive, ENABLE);
		}
}

/*----------------------------------------------------------------------------*/
/* Function    : UART_ErrorISR                                     */
/** 
    \brief      ISR for UART1 and UART2 Error. Its a Type1 Interrupt.
  
    \brief      Description:
                This function is called when the the RXERR bit in RXST  
                Register goes High, the Receive interrupt for UART1/UART2 is 
                enabled and corresponding bit in ISR mask register is enabled.

    \return     void
*/
/*----------------------------------------------------------------------------*/
#pragma INTERRUPT(UART_ErrorISR, IRQ)
void UART_ErrorISR(void)
{
	Uint8Type dasByte;
	if ((Uint8Type)1 == UART_u8GetRxStatus(UART2))
    {
		SCI_PST pstSci = UART2;
	#ifdef DO_DEBUG
		errorcount_uart2++;
	#endif
	/* Report the error                                                 */
        UART_stBufferProperties2.rxStatus = (Uint8Type)(pstSci->rxst_UN.rxst_UB);
        UART_vClearErrors(UART2);
        /* Read the received byte which automatically clear the interrupt   */
        dasByte = UART_u8HWReceiveByte(UART2);
        /* 8 + N + 1 + Tx enable + Rx ENABLE + Clear reception errors       */
    }
    if ((Uint8Type)1 == UART_u8GetRxStatus(UART1))
    {
    	SCI_PST pstSci = UART1;
	#ifdef DO_DEBUG
		errorcount_uart1++;
	#endif
    	/* Report the error                                                 */
        UART_stBufferProperties1.rxStatus = (Uint8Type)(pstSci->rxst_UN.rxst_UB);
        UART_vClearErrors(UART1);
        /* Read the received byte which automatically clear the interrupt   */
        dasByte = UART_u8HWReceiveByte(UART1);
        /* 8 + N + 1 + Tx enable + Rx ENABLE + Clear reception errors       */
    }
    dasByte = dasByte; // remove warning.
}

void UART_PrintStatistic(const SCI_ST *pstUart)
{
#ifdef DO_DEBUG
    char *pName = (pstUart == UART1) ? "UART1" : "UART2";
    Uint32Type errorcount = (pstUart == UART1) ? errorcount_uart1 : errorcount_uart2;
    Uint8Type rxStatus = (pstUart == UART1) ? UART_stBufferProperties1.rxStatus : UART_stBufferProperties2.rxStatus;
    Uint8Type txStatus = (pstUart == UART1) ? UART_stBufferProperties1.txStatus : UART_stBufferProperties2.txStatus;
    Uint32Type totalcount = (pstUart == UART1) ? totalcount_uart1 : totalcount_uart2;
    Uint8Type txPutCount =  (pstUart == UART1) ? UART_stBufferProperties1.txPutCount : UART_stBufferProperties2.txPutCount;
    Uint8Type txGetCount =  (pstUart == UART1) ? UART_stBufferProperties1.txGetCount : UART_stBufferProperties2.txGetCount;
    Uint8Type rxPutCount =  (pstUart == UART1) ? UART_stBufferProperties1.rxPutCount : UART_stBufferProperties2.rxPutCount;
    Uint8Type rxGetCount =  (pstUart == UART1) ? UART_stBufferProperties1.rxGetCount : UART_stBufferProperties2.rxGetCount;
    Uint8Type overFlowCount = (pstUart == UART1) ? UART_stBufferProperties1.BufferoverflowCount : UART_stBufferProperties2.BufferoverflowCount;

	if ( pstUart == UART1 )
	{
		DEBUG_TEXT(UART_SWI_TRC, MOD_UART, "UART1");
	}
	else if ( pstUart == UART2 )
	{
		DEBUG_TEXT(UART_SWI_TRC, MOD_UART, "UART2");
	}

    if ( errorcount != 0 )
    {
    	DEBUG_VALUE1(UART_SWI_ERR, MOD_UART, "ecnt%d", errorcount);
    }

    if ( rxStatus != 0 )
    {
    	DEBUG_VALUE1(UART_SWI_WNG, MOD_UART, "rxStatus %d", rxStatus);
    }

    if ( txStatus != 0 )
    {
    	DEBUG_VALUE1(UART_SWI_WNG, MOD_UART, "txStatus %d", txStatus);
    }
    if (( txPutCount - txGetCount)!= 0 )
    {
    	DEBUG_VALUE1(UART_SWI_TRC, MOD_UART, "txPutCnt %d", txPutCount);
    	DEBUG_VALUE1(UART_SWI_TRC, MOD_UART, "txGetCnt %d", txGetCount);
    }
    if (( rxPutCount - rxGetCount)!= 0 )
    {
    	DEBUG_VALUE1(UART_SWI_TRC, MOD_UART, "rxPutCnt %d", rxPutCount);
    	DEBUG_VALUE1(UART_SWI_TRC, MOD_UART, "rxGetCnt %d", rxGetCount);
    }
    if ( overFlowCount != 0 )
    {
    	DEBUG_VALUE1(UART_SWI_WNG, MOD_UART, "oFlwCnt %d", overFlowCount);
    }
#else
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    DEBUG_TEXT(UART_SWI_TRC, MOD_UART, "UART Debug Disabled");
#endif

#endif

}

/*----------------------------------------------------------------------------*/
/* Function    : UART_bGetUartHardwareErrorStatus                                     */
/**
    \brief      Function

    \brief      Description:
                This function reads the error count and if the error count is there,
                then it sets a flag. Normally there should never be an "hardware" error, but if it is there
                then a reset of the controller should be done because the communication
                between TMS and Parrot is based upon the UARt communication.

    \return     void
*/
/*----------------------------------------------------------------------------*/
BooleanType UART_bGetUartHardwareErrorStatus(void)
{
	if ( errorcount_uart2 != 0 )
	{
		return BT_TRUE;
	}
	else
	{
		return BT_FALSE;
	}
}

/* End Of File UART_C1.c */
