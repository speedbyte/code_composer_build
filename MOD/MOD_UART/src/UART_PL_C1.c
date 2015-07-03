/******************************************************************************/
/*****************  Copyright (c) 2007 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	UART_PL_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
 * $Date: 2011-01-11 17:10:33 +0100 (Di, 11 Jan 2011) $
 * $Rev: 17243 $
 * $Author: agrawal $
 * $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_UART/tags/MOD_UART_B_10.18_pre/src/UART_PL_C1.c $
 *--------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "UART_PL.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Definitions                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of private Methods                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/* Function    : UART_vResetEnter                                      */
/** 
    \brief      Enter Reset Mode of UART Peripheral
  
    \brief      Description:
                This function enters into the Reset Mode of the peripheral.  
                In the reset mode, the functionality of the UART is disabled.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vResetEnter(SCI_ST *pstUart)
{
    /* reset the hardware */
    pstUart->ctl3_UN.ctl3_ST.SwReset_B1 = 0;
    pstUart->ctl3_UN.ctl3_ST.RxErrIntEN_B1  = 0;
    pstUart->ctl3_UN.ctl3_ST.BrkdtIntEN_B1  = 0;
    pstUart->ctl3_UN.ctl3_ST.WakeupIntEN_B1 = 0;
    pstUart->ctl3_UN.ctl3_ST.TxActEN_B1     = 0;
    pstUart->ctl3_UN.ctl3_ST.RxActEN_B1     = 0;
    pstUart->ctl3_UN.ctl3_ST.PowerDown_B1   = 0;
    pstUart->ctl1_UN.ctl1_ST.Sleep_B1       = 0;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vResetExit                                      */
/** 
    \brief      Exit Reset Mode of UART Peripheral
  
    \brief      Description:
                This function exits from the Reset Mode of the peripheral.  
                The functionality of the UART is enabled through this.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vResetExit(SCI_ST *pstUart)
{
    /* reset the hardware */
    pstUart->ctl3_UN.ctl3_ST.SwReset_B1 = 1;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vSetClockModeSource                                      */
/** 
    \brief      Set Clock.
  
    \brief      Description:
                This function sets the clock to Internal or External.  

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vSetClockModeSource(SCI_ST *pstUart)
{
    /* reset the hardware */
    pstUart->ctl3_UN.ctl3_ST.Clock_B1 = (REG_32)SciInternalClock;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vConfigureCommControlReg                                */
/** 
    \brief      Configure Communication Registers.
  
    \brief      Description:
                In this function following registers are initialised. 
                Mode of communication, Timing Register, No of Stop bits
                If parity enabled?, and No of characters in Transmission.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vConfigureCommControlReg(SCI_ST *pstUart)
{
	pstUart->ccr_UN.ccr_ST.Mode_B1 = (REG_32)SciIdleLineMode;
	pstUart->ccr_UN.ccr_ST.Async_B1   = (REG_32)SciAsyncTiming;
    pstUart->ccr_UN.ccr_ST.Stop_B1 = (REG_32)SciOneStopBit;
    pstUart->ccr_UN.ccr_ST.ParityEN_B1 = DISABLE;
    pstUart->ccr_UN.ccr_ST.CharLength_B3 = (REG_32)SciEightChar;
    return;
}
/**
\msc
	system,appl,uartdriver,uartll, hw;
	uartdriver=>uartll [label = "UART_u8Configurebaudrate(BAUD)", textcolour="red"];
	uartll=>hw [label="Change hardware registers"];
	...;
	uartll>>uartdriver [ label = "SuccessFailType" ];
\endmsc
*/
/*----------------------------------------------------------------------------*/
/* Function    : UART_vConfigureBaudRate                                */
/** 
    \brief      Configure Baud Rate
  
    \brief      Description:
                The Baud rate is initialised through this register. Since only 
                integer values can be stored in the register, there are always
                an Error between the actual baud rate and calculated.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \param      u32BaudValue
    			Uint32Type
                A valid Baud value is required. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vConfigureBaudRate(SCI_ST *pstUart, Uint32Type u32BaudValue)
{
	Uint32Type temp;

    temp = TMS470_ICLOCK / ( 8U * u32BaudValue );
    u32BaudValue = temp - 1U;
    
    pstUart->HBaud_UB = (Uint8Type)(u32BaudValue >> 16);//brr >> 16;
    pstUart->MBaud_UB = (Uint8Type)(u32BaudValue >> 8);//brr >> 8;
    pstUart->LBaud_UB = (Uint8Type)(u32BaudValue);//brr;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vConfigureInterrupt                                */
/** 
    \brief      Enable or Disable interrupt.
  
    \brief      Description:
                The Baud rate is initialised through this register. Since only 
                integer values can be stored in the register, there are always
                an Error between the actual baud rate and calculated.

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \param      u8Location
    			SCIFUNCTIONS ( enum )
    			SciClock,    SciReceive,    SciTransmit,    SciError
    \param      u8Value
    			Uint8Type
                Value between 0 an 255. 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vConfigureInterrupt(SCI_ST *pstUart, SCIFUNCTIONS u8Location, Uint8Type u8Value)
{
	if (SciReceive == u8Location)
	{
		pstUart->ctl3_UN.ctl3_ST.RxActEN_B1 = u8Value;
	}
	else if (SciTransmit == u8Location)
	{
		pstUart->ctl3_UN.ctl3_ST.TxActEN_B1 = u8Value;
	}
	else if (SciError == u8Location)
	{
		pstUart->ctl3_UN.ctl3_ST.WakeupIntEN_B1 = u8Value;
		pstUart->ctl3_UN.ctl3_ST.BrkdtIntEN_B1 = u8Value;
		pstUart->ctl3_UN.ctl3_ST.RxErrIntEN_B1 = u8Value;
	}
	else
	{
    }
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vPinModify                                */
/** 
    \brief      Modify the pin to Peripheral or GPIO
  
    \brief      Description:
				This function changes the pin functionality to either GPIO
				(Input or Output) or Peripheral. 

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \param      PinID_N
    			SCIFUNCTIONS ( enum )
    			SciClock,    SciReceive,    SciTransmit,    SciError
    \param      PinQual_N
    			SCIQUAL ( enum )
				SciFunctional,    SciIoIn,    SciIoOut,    SciIoOutHigh,
				SciIoOutLow
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vPinModify(SCI_ST *pstUart, SCIFUNCTIONS PinID_N, SCIQUAL PinQual_N)
{
    switch (PinQual_N)
    {
        case SciFunctional:
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.Func_B1 = 1; // functional
            break;
            
        case SciIoOutLow:
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.Func_B1 = 0; // digital IO
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.DataDir_B1 = 0; // output
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.DataOut_B1 = 0; // output low
            break;
    
        case SciIoOutHigh:
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.Func_B1 = 0; // digital IO
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.DataDir_B1 = 0; // output
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.DataOut_B1 = 1; // output low
            break;
    
        case SciIoOut:
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.Func_B1 = 0; // digital IO
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.DataDir_B1 = 0; // output
            break;
           
        case SciIoIn:
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.Func_B1 = 0; // digital IO
        	pstUart->pc_PST[PinID_N].pc_UN.pc_ST.DataDir_B1 = 1; // input
            break;
            
        default:
            break;
    }
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vHWTransmitByte			                              */
/** 
    \brief      Put the Byte out in the HW Transmit Register.
  
    \brief      Description:
                Put the Byte out in the HW Transmit Register.
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \param      u8Byte
    			Uint8Type
                Value between 0 an 255. 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vHWTransmitByte(SCI_ST *pstUart, Uint8Type u8Byte)
{
	pstUart->TxBuf_UB  = u8Byte;
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vHWReceiveByte			                              */
/** 
    \brief      Take the Byte out from the HW receive Register.
  
    \brief      Description:
                Take the Byte out from the HW receive Register.
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     Uint8Type
                Value between 0 an 255. 
*/
/*----------------------------------------------------------------------------*/
Uint8Type UART_u8HWReceiveByte(const SCI_ST *pstUart)
{
	return(pstUart->RxBuf_UB);
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vConfigurePeripheral                                */
/** 
    \brief      Configure the Peripheral
  
    \brief      Description:
				This function changes the pin functionality to either GPIO
				(Input or Output) or Peripheral. 

    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \param      u8Location
    			SCIFUNCTIONS ( enum )
    			SciClock,    SciReceive,    SciTransmit,    SciError
    \param      u8Value
    			Uint8Type
				ENABLE or DISABLE
    \return     void
*/
/*----------------------------------------------------------------------------*/
void UART_vConfigurePeripheral(SCI_ST *pstUart, SCIFUNCTIONS u8Location, Uint8Type u8Value)
{
    if ( SciReceive == u8Location )
    {
    	pstUart->ctl1_UN.ctl1_ST.RxEN_B1 = u8Value;
    }
    else /* ( SciTransmit == u8Location ) */
    {
    	pstUart->ctl2_UN.ctl2_ST.TxEN_B1 = u8Value;
    }
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : UART_u8GetRxStatus			                              */
/** 
    \brief      Returns the Rx Status 
  
    \brief      Description:
                this function returns if there was an Error. It doesnt describe
                what kind of error was produced.
                8            7     6  5  4    3    2     1
                BUS_BUSY Reserved FE OE PE WAKEUP BRKDT RXERR
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     Uint8Type
                TRUE or FALSE
*/
/*----------------------------------------------------------------------------*/

Uint8Type UART_u8GetRxStatus(const SCI_ST *pstUart)
{
	return (Uint8Type)(pstUart->rxst_UN.rxst_ST.RxErr_B1);
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_u8GetInterruptStatus			                              */
/** 
    \brief      Returns the Interrupt Status
  
    \brief      Description:
                this function returns what is the Status of the Interrupt
                If the interrupt is enabled or disabled. The return value is
                the complete register.
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     Uint8Type
                Register ctl3.
*/
/*----------------------------------------------------------------------------*/
static Uint8Type UART_u8GetInterruptStatus(const SCI_ST *pstUart)
{
	return (pstUart->ctl3_UN.ctl3_UB);
}
/*---------------------------------------------------------------------------*/
/* Function    : UART_vClearErrors			                              */
/** 
    \brief      Clears all the errors.
  
    \brief      Description:
                this function clears all the errors
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2 
    \return     void
                void
*/
/*----------------------------------------------------------------------------*/
void UART_vClearErrors(SCI_ST *pstUart)
{
	pstUart->rxst_UN.rxst_ST.FE_B1 = 1;
	pstUart->rxst_UN.rxst_ST.OE_B1 = 1;
	pstUart->rxst_UN.rxst_ST.PE_B1 = 1;
	pstUart->rxst_UN.rxst_ST.Wakeup_B1 = 1;
	pstUart->rxst_UN.rxst_ST.Brkdt_B1 = 1;
	pstUart->rxst_UN.rxst_ST.RxErr_B1 = 1;
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vPortClear			                              */
/** 
    \brief      clear the pin to 0 when conf as GIO
  
    \brief      Description:
                this function clears the pin i.e sets 0 when this pin is 
                configured as GPIO.
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2
    \param      PinMask_N
    			SCIFUNCTIONS

    \return     void
                void
*/
/*----------------------------------------------------------------------------*/

static void UART_vPortClear(SCI_ST *pstUart, SCIFUNCTIONS PinMask_N)
{
	pstUart->pc_PST[PinMask_N].pc_UN.pc_ST.DataOut_B1 = 0;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : UART_vPortSet			                              */
/** 
    \brief      sets the pin to 1 when conf as GIO
  
    \brief      Description:
                this function sets the pin i.e sets 1 when this pin is 
                configured as GIO.
                
    \param      pstUart
    			SCI_PST
                UART1 / UART2
    \param      PinMask_N
    			SCIFUNCTIONS
                
    \return     void
                void
*/
/*----------------------------------------------------------------------------*/
static void UART_vPortSet(SCI_ST *pstUart, SCIFUNCTIONS PinMask_N)
{
    pstUart->pc_PST[PinMask_N].pc_UN.pc_ST.DataOut_B1 = 1;
    return;
}

/* End Of File UART_PL_C1.c */
