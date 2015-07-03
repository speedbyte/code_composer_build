/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	MFL_MGR.h
 * \brief	Module Header File. 
 * 			This file must be included by the application/system in order to use
 * 			the interfaces provided by this module. Detailed description found below. 
 * \brief	This file mainly contains:
 * 			1. prototypes of all the functions that can be used by other modules.
 * 			2. enum, structure, constant definition etc. that can be used by other 
 * 			modules.  
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-04-29 10:12:40 +0200 (Mi, 29 Apr 2009) $
* $Rev: 5988 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_MFL_MGR/trunk/inc/MFL_MGR.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _MFL_MGR_H
#define _MFL_MGR_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "MFL_MGR_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum
{
    enMfaNoKey = 0x00,
    enMfaUp = 0x22,
    enMfaDown = 0x23,
    enMfaOkReset = 0x28,
    enMfaMenuUp = 0x0A,
    enMfaMenuDown = 0x09,
    enMfaEscapeBack = 0x29,
    enMfaRollRight = 0x14,
    enMfaPushRollRight = 0x28,
    enMfaVolUp = 0x06,
    enMfaVolDown = 0x07,
    enMfaArrowAUp = 0x02,
    enMfaArrowADown = 0x03,
    enMfaMute = 0x2B,
    enMfaMode = 0x01,
    enMfaRollLeft = 0x13,
    enMfaHook = 0x1A,
    enMfaPTTPushToTalk = 0x2A,
    enMfaInitMfl = 0xF0
}MFL_ENUM_TASTENSYMBOL;

/* brief 
	0 = VW
	1=  AU
	2 = SE
	3 = SK
	4 = VN
	5 = Bugatti
	6 = Lamborghini
	7 = Bentley
	8 = Rolls Royce
	14 = Ford
	15 = Porsche
*/
typedef enum 
{
	enMflVW = 0x0,                 // No event
	enMflAudi = 0x1,               // Taste pressed
	enMflSeat = 0x2, 
	enMflSkoda = 0x3,              // Taste double click
	enMflVWNutfahrzeuge  = 0x4,    // Taste long press 1 [ 800 ms ] 
	enMflBugatti = 0x5,            //  Taste long press 2 [ 2 s ] 
	enMflLamborghini  = 0x6,       // Taste long press 3 [ 3 s ]
	enMflBentley = 0x7, 
	enMflRollsRoyce  = 0x8, 
	enMflQuattro = 0x9, 
	enMflKeinHersteller1 = 0xA, 
	enMflKeinHersteller2 = 0xB, 
	enMflKeinHersteller3 = 0xC, 
	enMflKeinHersteller4 = 0xD, 
	enMflFord = 0xE, 
	enMflPorsche = 0xF
} MFL_ENUM_MARKE;

typedef enum
{
	enMflKeyReleasedNoKey = 0x00,
	enMflMenu = 0x01,
	enMflMenuUpNextScreen = 0x02,
	enMflMenuDownPreviousScreen = 0x03,
	enMflUp = 0x04,
	enMflDown = 0x05,
	enMflUpDownThumbWheel = 0x06,
	enMflOKThumbWheelButton = 0x07,
	enMflCancelEscape = 0x08,
	enMflVolumeUp = 0x10,
	enMflVolumeDown = 0x11,
	enMflVolumeUpDownthumbWheel = 0x12,
	enMflVolumeThumbWheelButton = 0x13,
	enMflAudioSource = 0x14,
	enMflArrowAUpRight = 0x15,
	enMflArrowADownLeft = 0x16,
	enMflArrowBUpRight = 0x17,
	enMflArrowBDownLeft = 0x18,
	enMflPTTPushToTalk = 0x19,
	enMflPPTCancel = 0x1A,
	enMflRouteInfo = 0x1B,
	enMflHook = 0x1C,
	enMflHangup = 0x1D,
	enMflOffHook = 0x1E,
	enMflLightOnOff = 0x1F,
	enMflMute = 0x20,
	enMflJoker1 = 0x21,
	enMflJoker2 = 0x22,
	enMflStartupReset = 0xF0
}MFL_ENUM_TASTENCODE1;


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern void MFL_vParameterAnalysis (   Uint8Type u8Type, 
								MFL_ENUM_TASTENSYMBOL enSymbol,
								Uint8Type u8TickCounter,
								MFL_ENUM_MARKE enMarke, 
                                Uint8Type u8Variante
                                );
#ifdef IMFL_TASK_HOST

extern void MFL_HandleMsgTimer(EventMaskType event);                             

#endif
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#define TYPE_TASTEN         0x00
#define TYPE_TASTEN_KON     0x01

#define SKODA_MFL_SUPPORT  1

#endif /* _MFL_MGR_H */

/* End Of File MFL_MGR.h */

