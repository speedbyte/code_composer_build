/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SPI_PL_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-08-24 14:21:58 +0200 (Mo, 24 Aug 2009) $
* $Rev: 8018 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_SPI/trunk/src/SPI_PL_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SPI.h"
#include "SPI_PL.h"
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
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/

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
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : SPI_vPinModify                                */
/** 
    \brief      Modify the pin to Peripheral or GPIO
  
    \brief      Description:
				This function changes the pin functionality to either GPIO
				(Input or Output) or Peripheral. 

    \param      pstSpi
    			SPI_PST
                 
    \param      PinMask_N
    			SPIFUNCTIONS ( enum )
    			SpiEnablePin, SpiClkPin, SpiSimoPin, SpiSomiPin, SpiScsPin,
    \param      PinQual_N
    			PORTQUAL ( enum )
				SpiFunctional,    SpiIoIn,    SpiIoOut,    SpiIoOutHigh,
				SpiIoOutLow
    \return     void
*/
/*----------------------------------------------------------------------------*/
void SPI_vPinModify(SPI_ST *pstSpi, SPIFUNCTIONS PinMask_N, PORTQUAL PinQual_N)
{
    switch (PinQual_N)
    {
        case PortFunctional:
            pstSpi->pc6_UN.pc6_UL |=  (REG_32)PinMask_N;
            break;
    
        case PortIoIn:
            pstSpi->pc6_UN.pc6_UL &= ~(REG_32)PinMask_N;
            pstSpi->pc1_UN.pc1_UL &= ~(REG_32)PinMask_N;
            break;
    
        case PortIoOut:
            pstSpi->pc6_UN.pc6_UL &= ~(REG_32)PinMask_N;
            pstSpi->pc1_UN.pc1_UL |=  (REG_32)PinMask_N;
            break;
    
        case PortIoOutLow:
            pstSpi->pc6_UN.pc6_UL &= ~(REG_32)PinMask_N;
            pstSpi->pc5_UN.pc5_UL  =  (REG_32)PinMask_N;
            pstSpi->pc1_UN.pc1_UL |=  (REG_32)PinMask_N;
            break;
    
        case PortIoOutHigh:
            pstSpi->pc6_UN.pc6_UL &= ~(REG_32)PinMask_N;
            pstSpi->pc4_UN.pc4_UL  =  (REG_32)PinMask_N;
            pstSpi->pc1_UN.pc1_UL |=  (REG_32)PinMask_N;
            break;
           
        default:
            break;
    }
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : SPI_vResetEnter                                      */
/** 
    \brief      Enter Reset Mode of SPI Peripheral
  
    \brief      Description:
                This function enters into the Reset Mode of the peripheral.  
                In the reset mode, the functionality of the SPI is disabled.

    \param      pstSpi
    			SPI_PST
                 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void SPI_vResetEnter(SPI_ST *pstSpi)
{
    /* reset the hardware */
    pstSpi->ctrl2_UN.ctrl2_ST.SpiEN_B1 = 0;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vResetExit                                      */
/** 
    \brief      Exit Reset Mode of SPI Peripheral
  
    \brief      Description:
                This function exits from the Reset Mode of the peripheral.  
                The functionality of the SPI is enabled through this.

    \param      pstSpi
    			SPI_PST
                 
    \return     void
*/
/*----------------------------------------------------------------------------*/

void SPI_vResetExit(SPI_ST *pstSpi)
{
    pstSpi->ctrl2_UN.ctrl2_ST.SpiEN_B1 = 1;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vConfigureBaudRate                                */
/** 
    \brief      Configure Baud Rate
  
    \brief      Description:
                The Baud rate is initialised through this register. Since only 
                integer values can be stored in the register, there are always
                an Error between the actual baud rate and calculated.

    \param      pstSpi
    			SPI_PST
                 
    \param      u32BaudValue
    			Uint32Type
                A valid Baud value is required. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
void SPI_vConfigureBaudRate(SPI_ST *pstSpi, Uint32Type u32BaudValue)
{
	Uint32Type temp;
	Uint8Type pscale = 0U;
    /* Baud = ICLK / ( prescale + 1 ) */
	temp = TMS470_ICLOCK / ( u32BaudValue );
    pscale = temp - 1U;
    pstSpi->ctrl1_UN.ctrl1_ST.TapSel_B8 = pscale;
    return;	
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vSetCharLength                                */
/** 
    \brief      Configure Character Length of SPI.
  
    \brief      Description:
                In this function following parameters are initialised. 
                No of Characters to be shifted. If less <= 8, then 8
                chars are shifted otherwise 16.
    \param      pstSpi
    			SPI_PST
                 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void SPI_vSetCharLength(SPI_ST *pstSpi)
{
	pstSpi->ctrl1_UN.ctrl1_UL |= (REG_32)SpiEightChar;
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vSetMode                                */
/** 
    \brief      Configure Mode of SPI.
  
    \brief      Description:
                In this function following parameters are initialised. 
                SPICTRL2
				Clock Source
                Device is Master or Slave
                If Power Down mode is enabled
                At what Edge is Bit Out - POLARITY
                Is there a delay between receive and transmit - PHASE
                SPICTRL3
                Are the pins tristated
                Is DMA Enabled?
    \param      pstSpi
    			SPI_PST
                 
    \param      ModeInit_N
    			Uint16Type

    \return     void
*/
/*----------------------------------------------------------------------------*/
void SPI_vSetMode(SPI_ST *pstSpi, Uint16Type ModeInit_N)
{
	pstSpi->ctrl2_UN.ctrl2_UL = (ModeInit_N >> 8U);
	pstSpi->ctrl3_UN.ctrl3_UL =  ModeInit_N;
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vConfigureInterrupt                                */
/** 
    \brief      Enable or Disable interrupt.
  
    \brief      Description:
                The Baud rate is initialised through this register. Since only 
                integer values can be stored in the register, there are always
                an Error between the actual baud rate and calculated.

    \param      pstSpi
    			SPI_PST
                 
    \param      u8Location
    			SPIFUNCTIONS ( enum )
    			SpiClock,    SpiReceive,    SpiTransmit,    SpiError
    \param      u8Value
    			Uint8Type
    			Value between 0 an 255. 
             
    \return     void
*/
/*----------------------------------------------------------------------------*/
void SPI_vConfigureInterrupt(SPI_ST *pstSpi, SPIFUNCTIONS u8Location, Uint8Type u8Value)
{
	if (SpiReceive == u8Location)
	{
		pstSpi->ctrl3_UN.ctrl3_ST.IntEN_B1 = u8Value;
	}
	else /* (SpiError == u8Location) */
	{
		pstSpi->ctrl3_UN.ctrl3_ST.OvrnIntEN_B1 = u8Value;
	}
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vPortClear			                              */
/** 
    \brief      clear the pin to 0 when conf as SPI
  
    \brief      Description:
                this function clears the pin i.e sets 0 when this pin is 
                configured as GPIO.
                
    \param      pstSpi
    			SPI_PST

    \param      PinMask_N
    			SPIFUNCTIONS
                
    \return     void
                void
*/
/*----------------------------------------------------------------------------*/
void SPI_vPortClear(SPI_ST *pstSpi, SPIFUNCTIONS PinMask_N)
{
	pstSpi->pc5_UN.pc5_UL = (REG_32)PinMask_N;
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_vPortSet			                              */
/** 
    \brief      sets the pin to 1 when conf as GIO
  
    \brief      Description:
                this function sets the pin i.e sets 1 when this pin is 
                configured as GIO.
                
    \param      pstSpi
    			SPI_PST

    \param      PinMask_N
    			SPIFUNCTIONS

    \return     void
                void
*/
/*----------------------------------------------------------------------------*/
void SPI_vPortSet(SPI_ST *pstSpi, SPIFUNCTIONS PinMask_N)
{
    pstSpi->pc4_UN.pc4_UL = (REG_32)PinMask_N;
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : SPI_PinRead_B                                				  */
/** 
    \brief      This function gets the value from the Data
				input Register ( SPIDIN )
  
    \brief      Description:
				Detail: Effected Register: SPIDIN

    \param      pstSpi
    			SPI_PST
    			
    \param      PinMask_N
    			Uint8Type
				port to be modified
				
    \return     BooleanType
    			BT_TRUE if the bit is set, otherwise BT_FALSE.
*/
/*----------------------------------------------------------------------------*/
BooleanType SPI_bPinRead(const SPI_ST *pstSpi, SPIFUNCTIONS PinMask_N)
{
	Uint32Type u32PortRead;
	u32PortRead = (pstSpi->pc2_UN.pc2_UL)& (Uint32Type)PinMask_N;
	if ( u32PortRead == (Uint32Type)PinMask_N)
	{
		return BT_TRUE;
	}
	else
	{
		return BT_FALSE;
	}
}/* End Of File SPI_PL_C1.c */
