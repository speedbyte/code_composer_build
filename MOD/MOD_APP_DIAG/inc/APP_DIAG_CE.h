/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file	APP_DIAG_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * can be used by other modules. It also contains various constants that can be
 * configured by external people.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-10-18 14:21:54 +0200 (Mo, 18 Okt 2010) $
* $Rev: 15771 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DIAG/trunk/inc/APP_DIAG_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DIAG_CE_H
#define _APP_DIAG_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "APP_DIAG_ID_CHART.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
#define IAPP_DIAG_INIT
#define IAPP_DIAG_CONVERT
#define IAPP_DIAG_RECEIVE

#undef UHV_LED_DIAG_SUPPORT
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/**
    \brief	maximum length of Diagnosis Data.(
    due to Universal preparation for mobile telephone)
*/
#define APP_DIAG_MAX_DATA_LENGTH	              ERM_MAX_NUM_HEADSET_PROFILE *(AT_DEVICE_NAME_MAX_LEN+1)
#define APP_DIAG_MAX_PINCODE_ALLOWED            9999
#define MIN_NAME_VALID                          ' '  //0x20   // Leerzeichen
#define MAX_NAME_VALID                          'z'  //0x80   // 'z'
#define APP_DIAG_BT_NAME_SEPARATOR              0x00 // separator between two BT-Names

#define APP_DIAG_BT_NAME_SEPARATOR_LEN          2    // separator length between two BT-Names

#define APP_DIAG_ROOFLINE_MODULE_TYPE_NAR                        0x00        // $00: NAR
#define APP_DIAG_ROOFLINE_MODULE_TYPE_RDW                        0x01        // $01: RdW
#define APP_DIAG_ROOFLINE_MODULE_TYPE_NA                         0x10        // $10: not available


#define RESET_TO_FACTORY_SETTING_ON                    0           // on
#define USER_PROFILES_DELETE                           1           // delete

// maximum for Record Data Identifier Calibration Data
#define MAX_VALUE_DEVELOPER_TESTMODE                   1

#define MEDIA_DEVICE_CONNECTED      0x00
#define MEDIA_DEVICE_NOT_CONNECTED  0x01

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/**
 * \brief The below constants are defined in the odx specification for Diagnose
 */
#define APP_DIAG_DTC_OK                             0x11
#define APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND        0x01
#define APP_DIAG_DTC_SHORT_CIRCUIT_AMONG_THEMSELVES 0x02
#define APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY       0x00
#define APP_DIAG_DTC_MECH_FAILURE                   0x08
#define APP_DIAG_DTC_OPEN_CIRCUIT                   0x10
#define APP_DIAG_DTC_INVALID						0xFF


// Masking of Languages Aktivieren/Deaktivieren der Sprache
// Byte 0
#define APP_DIAG_LANG_SUPP_GERMAN                       0x0100  // Byte0 Bit0
#define APP_DIAG_LANG_SUPP_ENGLISH                      0x0200  // Byte0 Bit1
#define APP_DIAG_LANG_SUPP_FRENCH                       0x0400  // Byte0 Bit2
#define APP_DIAG_LANG_SUPP_ITALIAN                      0x0800  // Byte0 Bit3
#define APP_DIAG_LANG_SUPP_SPANISH                      0x1000  // Byte0 Bit4
#define APP_DIAG_LANG_SUPP_PORTUGUESE                   0x2000  // Byte0 Bit5
#define APP_DIAG_LANG_SUPP_CZECH                        0x4000  // Byte0 Bit6
#define APP_DIAG_LANG_SUPP_RUSSIAN                      0x8000  // Byte0 Bit7
// Byte 1
#define APP_DIAG_LANG_SUPP_US_ENGLISH                   0x0001  // Byte1 Bit0
#define APP_DIAG_LANG_SUPP_FRENCH_CANADIAN              0x0002  // Byte1 Bit1
#define APP_DIAG_LANG_SUPP_US_SPANISH                   0x0004  // Byte1 Bit2
#define APP_DIAG_LANG_SUPP_DUTCH                        0x0008  // Byte1 Bit3
#define APP_DIAG_LANG_SUPP_SWEDISH                      0x0010  // Byte1 Bit4
#define APP_DIAG_LANG_SUPP_POLISH                       0x0020  // Byte1 Bit5
#define APP_DIAG_LANG_SUPP_NORWEGIAN                    0x0030  // Byte1 Bit6
#define APP_DIAG_LANG_SUPP_TURKISH                      0x0040  // Byte1 Bit7

// fallback languages
#define APP_DIAG_FALLBACK_LANG_NONE                     0x00
#define APP_DIAG_FALLBACK_LANG_GERMAN                   0x01
#define APP_DIAG_FALLBACK_LANG_ENGLISH                  0x02
#define APP_DIAG_FALLBACK_LANG_FRENCH                   0x03
#define APP_DIAG_FALLBACK_LANG_ITALIAN                  0x04
#define APP_DIAG_FALLBACK_LANG_SPANISH                  0x05
#define APP_DIAG_FALLBACK_LANG_PORTUGUESE               0x06
#define APP_DIAG_FALLBACK_LANG_CZECH                    0x07
#define APP_DIAG_FALLBACK_LANG_DUTCH                    0x08
#define APP_DIAG_FALLBACK_LANG_SWEDISH                  0x09
#define APP_DIAG_FALLBACK_LANG_US_ENGLISH               0x0A
#define APP_DIAG_FALLBACK_LANG_RUSSIAN                  0x0B
#define APP_DIAG_FALLBACK_LANG_FRENCH_CANADIAN          0x0C
#define APP_DIAG_FALLBACK_LANG_US_SPANISH               0x0D
#define APP_DIAG_FALLBACK_LANG_POLISH                   0x0E
#define APP_DIAG_FALLBACK_LANG_NORWEGIAN                0x0F
#define APP_DIAG_FALLBACK_LANG_TURKISH                  0x10

// Stellglied Tests

#endif
/* _APP_DIAG_CE_H */

/* End Of File APP_DIAG_CE.h */


