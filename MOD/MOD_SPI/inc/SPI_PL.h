/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SPI_PL.h
 * \brief	C File Header. 
 * 			This file must be included by the C File in order to use the 
 * 			constants, variables etc declared in this file. Detailed description found below. 
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-03-30 16:38:49 +0200 (Mo, 30 Mrz 2009) $
* $Rev: 5702 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_SPI/trunk/inc/SPI_PL.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _SPI_PL_H
#define _SPI_PL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SPI_CI.h"

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
#define SPINOMODE	   0XFFFF
#define SPIINTCLOCK    0X2000
#define SPIEXTCLOCK    0X0000
#define SPIMASTER      0X0800
#define SPISLAVE       0X0000
#define SPIPOWERDOWN   0X0400
#define SPINOPOWERDOWN 0X0000
/** 
    \brief	Input data is latched on the falling edge of SPICLK
*/
#define SPIFALLINGEDGE 0X0200

/** 
    \brief	Input data is latched on the rising edge of SPICLK
*/
#define SPIRISINGEDGE  0X0000
/** 
    \brief	Data is output one-half cycle before the first rising of SPICLK and on subsequent falling
edges of SPICLK
*/
#define SPIWITHDELAY   0X0100
/** 
    \brief	Data is output on the rising edge of SPICLK
*/
#define SPINODELAY     0X0000
#define SPITRISTATEEN  0X0020
#define SPITRISTATEDIS 0X0000
#define SPIDMAREQEN    0X0010
#define SPIDMAREQDIS   0X0000
#define SPIOVRINTEN    0X0008
#define SPIOVRINTDIS   0X0000
#define SPIRXINTEN     0X0002
#define SPIRXINTDIS    0X0000

#define SPINODIR       0XFF
#define SPISCSOUT      0X010
#define SPISCSIN       0X000
#define SPISOMIOUT     0X008
#define SPISOMIIN      0X000
#define SPISIMOOUT     0X004
#define SPISIMOIN      0X000
#define SPICLKOUT      0X002
#define SPICLKIN       0X000
#define SPIENABLEOUT   0X001
#define SPIENABLEIN    0X000

#define SPINOFUN       0XFF
#define SPINOCHANGE    (-1)

#define SPIDMAHIGH     0X00
#define SPIDMALOW      0X08

typedef enum _SCICONFIGURATIONS
{
    SpiOneStopBit = 0,
    SpiTwoStopBit = 1,
    SpiIdleLineMode = 0,
    SpiAddressLineMode = 1,
    SpiIsosyncTiming = 0,
    SpiAsyncTiming = 1,
    SpiExternalClock = 0,
    SpiInternalClock = 1,
    SpiOneChar = 1,
    SpiTwoChar,
    SpiThreeChar,
    SpiFourChar,
    SpiFiveChar,
    SpiSixChar,
    SpiSevenChar,
    SpiEightChar,
    SpiNineChar,
    SpiTenChar,
    SpiElevenChar,
    SpiTwelveChar,
    SpiThirteenChar,
    SpiFourteenChar,
    SpiFifteenChar,
    SpiSixteenChar
} SCICONFIGURATIONS;

/** 
\brief	SPI clock - 1 MHz
*/
#define SPI_1MHZ        1000000
/** 
\brief	SPI clock - 2 MHz
*/
#define SPI_2MHZ        2000000
/** 
\brief	SPI clock - 3 MHz
*/
#define SPI_3MHZ        3000000
/** 
\brief	SPI clock - 4 MHz
*/
#define SPI_4MHZ        4000000
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef ISPI_PL
/*----------------------------------------------------------------------------*/
/* Module Global Methods                                                      */
/*----------------------------------------------------------------------------*/
void SPI_vResetEnter(SPI_ST *pstSpi);

void SPI_vResetExit(SPI_ST *pstSpi);

void SPI_vSetMode(SPI_ST *pstSpi, Uint16Type ModeInit_N);

void SPI_vSetCharLength(SPI_ST *pstSpi);

void SPI_vConfigureBaudRate(SPI_ST *pstSpi, Uint32Type u32BaudRate);

void SPI_vConfigureInterrupt(SPI_ST *pstSpi, SPIFUNCTIONS u8Location, Uint8Type u8Value);

void SPI_vPinModify(SPI_ST *pstSpi, SPIFUNCTIONS PinID_N, PORTQUAL PinQual_N);

void SPI_vPortSet(SPI_ST *pstSPI, SPIFUNCTIONS PinMask_N);

void SPI_vPortClear(SPI_ST *pstSPI, SPIFUNCTIONS PinMask_N);

/*----------------------------------------------------------------------------*/
#endif // ISPI_PL

#endif /* _SPI_PL_H */

