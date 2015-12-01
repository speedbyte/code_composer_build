/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	SPI_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file mainly contains:
 * 			1. constants to switch on or off the exported interfaces. 
 * 			2. configurable constants.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-03-30 16:38:49 +0200 (Mo, 30 Mrz 2009) $
* $Rev: 5702 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_SPI/trunk/inc/SPI_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _SPI_CE_H
#define _SPI_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
// Schalter
#define ISPI_INIT
#define ISPI_TRANSMIT
#define ISPI_EXCHANGE
#define ISPI_CHECK
#define ISPI_MODIFY
#define ISPI_READ
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
	
/**
 \brief  To select  SPI1 Interface
*/
#define SPI1 						(SPI_PST)TMS470R1_BASEADDRESS_SPI1
/**
 \brief  To select SPI2 Interface
*/
#define SPI2 						(SPI_PST)TMS470R1_BASEADDRESS_SPI2
/**
 \brief  To select SPI3 Interface
*/
#define SPI3 						(SPI_PST)TMS470R1_BASEADDRESS_SPI3

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
/**
 \brief  To select SPI4 Interface
*/
#define SPI4 						(SPI_PST)TMS470R1_BASEADDRESS_SPI4
/**
 \brief  To select SPI5 Interface
*/
#define SPI5 						(SPI_PST)TMS470R1_BASEADDRESS_SPI5

#endif


#define SPI_INIT	if ( pstSpi == (SPI_PST)SPI2 )\
	{\
		SPI_vPinModify(pstSpi, SpiEnablePin,  PortIoOutHigh);\
		SPI_vPinModify(pstSpi, SpiClkPin,  PortFunctional);\
		SPI_vPinModify(pstSpi, SpiSimoPin,  PortFunctional);\
		SPI_vPinModify(pstSpi, SpiSomiPin,  PortFunctional);\
		SPI_vPinModify(pstSpi, SpiScsPin,  PortIoOutHigh);\
	}\
	else if (( pstSpi == (SPI_PST)SPI1 ) || ( pstSpi == (SPI_PST)SPI3 ) || ( pstSpi == (SPI_PST)SPI4 ) )\
	{\
		SPI_vPinModify(pstSpi, SpiEnablePin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiClkPin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiSimoPin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiSomiPin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiScsPin,  PortIoOut);\
	}\
	else if (( pstSpi == (SPI_PST)SPI5 ))\
	{\
		SPI_vPinModify(pstSpi, SpiEnablePin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiClkPin,  PortIoIn);\
		SPI_vPinModify(pstSpi, SpiSimoPin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiSomiPin,  PortIoOut);\
		SPI_vPinModify(pstSpi, SpiScsPin,  PortIoOut);\
	}\

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
#define SPI_BAUD_RATE SPI_3MHZ

#endif /* _SPI_CE_H */

/* End Of File SPI_CE.h */


