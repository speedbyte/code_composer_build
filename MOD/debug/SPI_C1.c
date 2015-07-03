/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SPI_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-01-31 11:33:51 +0100 (Mo, 31 Jan 2011) $
* $Rev: 17579 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_SPI/trunk/src/SPI_C1.c $
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
/* Function    : SPI_sfInit                                         */
/** 
    \brief      Initialise the SPI
  
    \brief      Description:
                Initialise the SPI to a correct state. 

    \param      pstSpi
    			SPI_ST
                SPI1 / SPI2 / SPI3 
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType SPI_sfInit(SPI_ST *pstSpi)
{
	Uint8Type SpiBuf_UL;
	/* before we change the config, enter SPI reset state */
    /* initialise the SPI control regiosters */
	SPI_vResetEnter(pstSpi);  
    /* set the baud rate */
    SPI_vConfigureBaudRate(pstSpi, SPI_BAUD_RATE);  
    /* set the character size */
	SPI_vSetCharLength(pstSpi);
	SPI_vSetMode(pstSpi, (SPIINTCLOCK | SPIMASTER | SPINOPOWERDOWN | SPIRISINGEDGE | SPIWITHDELAY | SPITRISTATEDIS | SPIDMAREQDIS) );
	SPI_vConfigureInterrupt(pstSpi, SpiError, DISABLE);
	SPI_vConfigureInterrupt(pstSpi, SpiReceive,  DISABLE);
    /* initialise the function of the port pins */
	SPI_INIT
    /* finaly exit from the reset state */
    SPI_vResetExit(pstSpi);
    SpiBuf_UL = pstSpi->buf_UN.buf_ST.buf_UW; // To avoid overruns in the beginning.
    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_sfExchangeBlock                                         */
/** 
    \brief      Simultaneous send and receive a block of data
  
    \brief      Description:
                The application sends a pointer to an array of bytes which 
                needs to be send on the Bus and gets an array of Bytes which\
                is overwritten on the same pointer. 

    \param      pstSpi
    			SPI_ST
                SPI1 / SPI2 / SPI3 

    \param		BlockSize_UW
    			Uint16Type
    			Block size value between 0 and 65535.

    \param		Exchange_PUB
    			Uint8Type
    			Pointer to Uint8Type array. The value is overwritten by the 
    			received bytes from the SPI.
    			
    \param		exitWithCSEnabled
    			FlagType
    			
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/	
SuccessFailType SPI_sfExchangeBlock(SPI_ST *pstSpi, Uint16Type BlockSize_UW, Uint8Type *Exchange_PUB)
{
    SPIEXPT     status     = SpiNoError;
    Uint16Type  SpiBuf_UL;
    Uint8Type i;
    SuccessFailType sF = SUCCESS;
    Uint8Type      tempTimeoutCount;
    SPI_vPortClear(pstSpi, SpiScsPin);

        if (pstSpi->ctrl3_UN.ctrl3_ST.RcvrOvrn_B1 == 1U)
        {
            /* clear interrupts */
	        SpiBuf_UL = pstSpi->buf_UN.buf_ST.buf_UW;
	        status = SpiOverrun;
	        sF = FAIL;
        }
        else
        {
            do
            {
	           	tempTimeoutCount = 0; 
            	pstSpi->dat0_UW = (Uint8Type)*Exchange_PUB;
	            while (pstSpi->ctrl3_UN.ctrl3_ST.IntFlag_B1 == 0U)
	            {
	                 if (++tempTimeoutCount == 0)
	                 {
	                     /***********          ERROR HANDLING          ***********/
	                     sF = FAIL;
	                     break;
	                 }
	            }
	            if (FAIL == sF )
	            {
	            	break;
	            }
	            else
	            {
		             if ((pstSpi->buf_UN.buf_ST.RcvrOvrn_B1) == 1U)
		             {
		            	 status = SpiOverrun;
		                 break;
		             }
		             /* reading the buffer clears the flag and hence avoid overrun. */
		             SpiBuf_UL = pstSpi->buf_UN.buf_ST.buf_UW;
		             *Exchange_PUB++ = (Uint8Type)SpiBuf_UL;
	            }
            }
            while (--BlockSize_UW > 0);
            for (i = 0; i < 10; i++) ;
        }
        /* Disable (deassert) chip select                                   */
        for (i = 0; i < 3; i++);
        SPI_vPortSet(pstSpi, SpiScsPin);
    return sF;
}
/*----------------------------------------------------------------------------*/
/* Function    : SPI_sfWriteBlock                                         */
/** 
    \brief      Simultaneous send and receive a block of data
  
    \brief      Description:
                The application sends a pointer to an array of bytes which 

    \param      pstSpi
    			SPI_ST
                SPI1 / SPI2 / SPI3 
    \param		BlockSize_UW
    			Uint16Type
    			Block size value between 0 and 65535.
    \param		Src_PUB
    			Uint8Type
    			Pointer to Uint8Type array. The value is overwritten by the 
    			received bytes from the SPI.
    \param		exitWithCSEnabled
    			FlagType
    			
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType SPI_sfWriteBlock(SPI_ST *pstSpi, Uint16Type BlockSize_UW, Uint8Type *Src_PUB, FlagType exitWithCSEnabled)
{
	SPIEXPT     status     = SpiNoError;
    Uint16Type  SpiBuf_UL;
    Uint8Type i;
    SuccessFailType sF = SUCCESS;
    Uint8Type      tempTimeoutCount;
    SPI_vPortClear(pstSpi, SpiScsPin);
    
    if (pstSpi->ctrl3_UN.ctrl3_ST.RcvrOvrn_B1 == 1U)
    {
        /* clear interrupts */
        SpiBuf_UL = pstSpi->buf_UN.buf_ST.buf_UW;
        status = SpiOverrun;
        sF = FAIL;
    }
    else
    {
        do
        {
           	tempTimeoutCount = 0; 
        	pstSpi->dat0_UW = (Uint8Type)*Src_PUB++;
            while (pstSpi->ctrl3_UN.ctrl3_ST.IntFlag_B1 == 0U)
            {
                 if (++tempTimeoutCount == 0)
                 {
                     /***********          ERROR HANDLING          ***********/
                     sF = FAIL;
                     break;
                 }
            }
            if (FAIL == sF )
            {
            	 break;
            }
	            
            else
            {
	             if ((pstSpi->buf_UN.buf_ST.RcvrOvrn_B1) == 1U)
	             {
	            	 status = SpiOverrun;
	                 break;
	             }
	             /* reading the buffer clears the flag and hence avoid overrun. */
	             SpiBuf_UL = pstSpi->buf_UN.buf_ST.buf_UW;
            }
        }
        while (--BlockSize_UW > 0);
        for (i = 0; i < 10; i++) ;
    }
    /* Disable (deassert) chip select                                   */
    if (exitWithCSEnabled == DISABLE)
    {
        for (i = 0; i < 3; i++);
        SPI_vPortSet(pstSpi, SpiScsPin);
    }

    return sF;
}

/****************************************************************************/
/* Checks whether the device is ready                                       */
/****************************************************************************/
SuccessFailType SPI_sfIsDeviceReady(SPIDEVICETYPE SpiDevice)
{
    SpiDevice=SpiDevice;
    return SUCCESS;
}
/* -------------------------------------------------------------------------  */
/* End Of File SPI_C1.c */




