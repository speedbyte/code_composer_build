/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	APP_DTC_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * can be used by other modules. It also contains various constants that can be 
 * configured by external people.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-05-20 10:10:11 +0200 (Do, 20 Mai 2010) $
* $Rev: 13262 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DTC/trunk/inc/APP_DTC_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DTC_CE_H
#define _APP_DTC_CE_H

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
#define IAPP_DTC_DIAG_AUDIO
#define IAPP_DTC_DIAG_3BM
#define IAPP_DTC_DIAG_RADIO
#define IAPP_DTC_DIAG_BASEPLATE
#define IAPP_DTC_TASK_APP
#define IAPP_DTC_INIT
#define IAPP_DTC_VARIANT_TYPE
#define IAPP_DTC_CODING
#define IAPP_DTC_TEMPERATURE
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
    \brief	example
*/
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/** 
    \brief	example
*/
typedef enum {
	APP_DTC_RADIOMUTE_KURZSCHLUSSNACHMASSE = 602,
	APP_DTC_RADIOMUTE_KURZSCHLUSSNACHPLUS  = 602,
	APP_DTC_RADIOMUTE_UNTERBRECHNUNG_MAX   = 993,
	APP_DTC_RADIOMUTE_UNTERBRECHNUNG_MIN   = 602,
	APP_DTC_RADIOMUTE_NOPROBLEM_LOW        = 602,
	APP_DTC_RADIOMUTE_NOPROBLEM_HIGH       = 993
} ENUM_APP_DTC_RADIOMUTE;

typedef enum {
	APP_DTC_MICROPHONE_KURZSCHLUSSNACHMASSE_PLUS    = 200,
	APP_DTC_MICROPHONE_NOPROBLEM_PLUS_MIN           = 200,
	APP_DTC_MICROPHONE_NOPROBLEM_PLUS_MAX           = 333,
	APP_DTC_MICROPHONE_UNTERBRECHNUNG_PLUS_MIN      = 333,
	APP_DTC_MICROPHONE_UNTERBRECHNUNG_PLUS_MAX      = 398,
	APP_DTC_MICROPHONE_KURZSCHLUSSNACHPLUS_PLUS     = 398
} ENUM_APP_DTC_MIKROTELEFON;

typedef enum 
{
	APP_DTC_AUDIO_LEFT_NOPROBLEM_PLUS_MIN          = 150,
	APP_DTC_AUDIO_LEFT_NOPROBLEM_PLUS_MAX          = 292,
	APP_DTC_AUDIO_LEFT_NOPROBLEM_MINUS_MIN         =  30,
	APP_DTC_AUDIO_LEFT_NOPROBLEM_MINUS_MAX         = 210,
	APP_DTC_AUDIO_LEFT_UNTERBRECHNUNG_PLUS_MIN         = 292,
	APP_DTC_AUDIO_LEFT_UNTERBRECHNUNG_PLUS_MAX         = 394,
	APP_DTC_AUDIO_LEFT_UNTERBRECHNUNG_MINUS_MAX        =  30,
	APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHPLUS_PLUS_MIN    = 394,
	APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHPLUS_MINUS_MIN   =  210,
	APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHMASSE_PLUS_MAX   = 292,
	APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHMASSE_MINUS_MAX  =  30,
	APP_DTC_AUDIO_LEFT_NOPROBLEM_DELTA_PLUS_MINUS  =  24 
} ENUM_APP_DTC_AUDIOLEFT;

typedef enum 
{
	APP_DTC_AUDIO_RIGHT_NOPROBLEM_PLUS_MIN          = 150,
	APP_DTC_AUDIO_RIGHT_NOPROBLEM_PLUS_MAX          = 292,
	APP_DTC_AUDIO_RIGHT_NOPROBLEM_MINUS_MIN         =  30,
	APP_DTC_AUDIO_RIGHT_NOPROBLEM_MINUS_MAX         = 210,
	APP_DTC_AUDIO_RIGHT_UNTERBRECHNUNG_PLUS_MIN         = 292,
	APP_DTC_AUDIO_RIGHT_UNTERBRECHNUNG_PLUS_MAX         = 394,
	APP_DTC_AUDIO_RIGHT_UNTERBRECHNUNG_MINUS_MAX        =  30,
	APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHPLUS_PLUS_MIN    = 394,
	APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHPLUS_MINUS_MIN    = 210,
	APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHMASSE_PLUS_MAX   = 292,
	APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHMASSE_MINUS_MAX  =  30,
	APP_DTC_AUDIO_RIGHT_NOPROBLEM_DELTA_PLUS_MINUS  =  24 
} ENUM_APP_DTC_AUDIORIGHT;

typedef enum {
	APP_DTC_3BM_MAXUNDERVOLTAGE = 254,
	APP_DTC_3BM_MINOPENCIRCUIT = 783,
	APP_DTC_3BM_MINHIGHVOLTAGE = 873
} ENUM_APP_DTC_3BM;

typedef enum {
	APP_DTC_BASEPLATE_MAXUNDERVOLTAGE_VW = 104,    
	APP_DTC_BASEPLATE_MINOPENCIRCUIT_VW = 753,
	APP_DTC_BASEPLATE_MINHIGHVOLTAGE_VW = 877
}ENUM_APP_DTC_BASEPLATE_VW;

typedef enum {
	APP_DTC_ANTENNA_MAXUNDERVOLTAGE_VW = 178,    
	APP_DTC_ANTENNA_MINHIGHVOLTAGE_VW = 539
}ENUM_APP_DTC_ANTENNA_VW;

typedef enum {
	APP_DTC_BASEPLATE_MAXUNDERVOLTAGE_SKODA = 107,    
	APP_DTC_BASEPLATE_MINOPENCIRCUIT_SKODA = 753,
	APP_DTC_BASEPLATE_MINHIGHVOLTAGE_SKODA = 877
}ENUM_APP_DTC_BASEPLATE_SKODA;

typedef enum {
	APP_DTC_ANTENNA_MAXUNDERVOLTAGE_SKODA = 178,    
	APP_DTC_ANTENNA_MINHIGHVOLTAGE_SKODA = 539
}ENUM_APP_DTC_ANTENNA_SKODA;

#endif 
/* _APP_DTC_CE_H */

/* End Of File APP_DTC_CE.h */

