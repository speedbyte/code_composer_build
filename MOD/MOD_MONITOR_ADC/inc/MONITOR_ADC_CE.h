/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	MONITOR_ADC_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file mainly contains:
 * 			1. constants to switch on or off the exported interfaces. 
 * 			2. configurable constants.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-09-15 14:44:38 +0200 (Mi, 15 Sep 2010) $
* $Rev: 15172 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_MONITOR_ADC/trunk/inc/MONITOR_ADC_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _MONITOR_ADC_CE_H
#define _MONITOR_ADC_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
    \brief	description of the type
*/
#define IMONITOR_ADC_READ
#define IMONITOR_ADC_INIT
#define IMONITOR_ADC_TASK
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))

#define AUDIO_DIAG_START SPI_vPinModify(SPI5, SpiSimoPin, PortIoOutHigh);  
#define AUDIO_DIAG_START_INIT SPI_vPinModify(SPI5, SpiSimoPin, PortIoOutHigh); 
#define AUDIO_DIAG_END SPI_vPinModify(SPI5, SpiSimoPin, PortIoOutLow);
#define AUDIO_DIAG_END_INIT SPI_vPinModify(SPI5, SpiSimoPin, PortIoOutLow);


#elif defined(PROCESSOR_TMS470R1VF45AA)

#define AUDIO_DIAG_START GIO_vPinWrite(PORT_AUDIO_DIAG_EN, TRUE, GIOB);  
#define AUDIO_DIAG_END GIO_vPinWrite(PORT_AUDIO_DIAG_EN, FALSE, GIOB);

#define AUDIO_DIAG_START_INIT GIO_vPinModify(GIOB, PORT_AUDIO_DIAG_EN, PortIoOutHigh);  
#define AUDIO_DIAG_END_INIT GIO_vPinModify(GIOB, PORT_AUDIO_DIAG_EN, PortIoOutLow);  

#endif

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#endif 
/* _MONITOR_ADC_CE_H */

/* End Of File MONITOR_ADC_CE.h */

