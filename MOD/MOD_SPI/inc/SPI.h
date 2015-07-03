/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SPI.h
 * \brief	Module Header File. 
 * 			This file must be included by the application/system in order to use
 * 			the interfaces provided by this module. Detailed description found below. 
 * \brief	This file mainly contains:
 * 			1. prototypes of all the functions that can be used by other modules.
 * 			2. enum, structure, constant definition etc. that can be used by other 
 * 			modules.  
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-11-30 10:36:43 +0100 (Di, 30 Nov 2010) $
* $Rev: 16585 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_SPI/trunk/inc/SPI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _SPI_H
#define _SPI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "PORT.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "SPI_CE.h"

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
typedef enum
{
    SPI_DEVICE_AD1940 = 0,
    SPI_DEVICE_AD1939,
    SPI_DEVICE_M95160
} SPIDEVICETYPE;

typedef enum _SPIFUNCTIONS
{
    SpiEnablePin = 0x01,
    SpiClkPin = 0x02,
	SpiSimoPin = 0x04,
	SpiSomiPin = 0x08,
    SpiScsPin = 0x10,
    SpiReceive,
    SpiError
} SPIFUNCTIONS;

typedef enum _SPIEXPT
{
	SpiNoError,
	SpiPending,
	SpiOverrun,
	SpiTimeout,
	SpiDmaError,
	SpiModeError
} SPIEXPT;


/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef ISPI_INIT

SuccessFailType SPI_sfInit(SPI_ST *pstSpi);

/*----------------------------------------------------------------------------*/
#endif
/*----------------------------------------------------------------------------*/
#ifdef ISPI_TRANSMIT

SuccessFailType SPI_sfWriteBlock(SPI_ST *pstSpi, Uint16Type BlockSize_UW, Uint8Type *Src_PUB, FlagType exitWithCSEnabled);

#endif
/*----------------------------------------------------------------------------*/

#ifdef ISPI_EXCHANGE

SuccessFailType SPI_sfExchangeBlock(SPI_ST *pstSpi, Uint16Type BlockSize_UW, Uint8Type *Exchange_PUB);

#endif

#ifdef ISPI_CHECK

SuccessFailType SPI_sfIsDeviceReady(SPIDEVICETYPE SpiDevice);

#endif

#ifdef ISPI_MODIFY

void SPI_vPinModify(SPI_ST *pstSpi, SPIFUNCTIONS PinMask_N, PORTQUAL PinQual_N);

#endif

#ifdef ISPI_READ

BooleanType SPI_bPinRead(const SPI_ST *pstSpi, SPIFUNCTIONS PinMask_N);

#endif


/*----------------------------------------------------------------------------*/
#endif /* _SPI_H */

/* End Of File SPI.h */

