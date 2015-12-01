/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	APP_DTC_CI.h
 * \brief	CI - Configuration Internal File.  Detailed description found below.
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * are used internally within the module. It also contains various constants that can be 
 * configured by the module owner.
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-11-09 15:53:14 +0100 (Mi, 09 Nov 2011) $
* $Rev: 23190 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DTC/trunk/inc/APP_DTC_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DTC_CI_H
#define _APP_DTC_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define IAPP_DTC_PL
#define APP_DTC_U7V5_CORR_VALUE   (668 << 4)  // Korrekturfaktor für die Referenzspannung U7V5
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
typedef enum
{
	enAppDtcStartMonitoringCodierung,
	enAppDtcReadValuesOnStartUp
} APP_DTC_ENUM_START_SEQUENCE;

typedef struct {
    Sint8Type  Temperature;
    Uint16Type ADC;
} TEMPERATURE_TableType;

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
#define TEMPERATURE_HIGH     70  // bei 70°C soll der DTC_905000_FUNKTIONEINSCHRAENKUNGDURCHUEBERTEMPERATUR eingetragen werden (ODX Spec.)

#endif /* _APP_DTC_CI_H */

/* End Of File APP_DTC_CI.h */






