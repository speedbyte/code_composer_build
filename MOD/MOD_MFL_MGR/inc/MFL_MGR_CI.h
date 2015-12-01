/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	MFL_MGR_CI.h
 * \brief	CI - Configuration Internal File.  Detailed description found below.
 * \brief	This file mainly contains of:
 * 			1. constants to switch on or off the internal interfaces. 
 * 			2. module internal constants that can be configured by the module owner.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-03-30 17:29:19 +0200 (Mo, 30 Mrz 2009) $
* $Rev: 5710 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_MFL_MGR/trunk/inc/MFL_MGR_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _MFL_MGR_CI_H
#define _MFL_MGR_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                                          */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/

/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/
/* brief 
	Walze:
	0 = kein Tick
	1...7 = +1...+7 Ticks
	8 = Fehlerwert
	9...15 = -7...-1 Ticks
	
	Taste:
	0 = no Event
	1 = pressed
	3 = double-click
	4 = long-press 1 [800ms]
	5 = long-press 2 [2s]
	6 = long-press 3 [3s]
	
	Tickzaehler od. Eventcode Walze 1
*/
typedef enum 
{
	enNo_Event = 0x0,       // No event
	en1_Tick_up = 0x1,      // Taste pressed
	en2_Ticks_up = 0x2, 
	en3_Ticks_up = 0x3,     // Taste double click
	en4_Ticks_up  = 0x4,    // Taste long press 1 [ 800 ms ] 
	en5_Ticks_up = 0x5,     //  Taste long press 2 [ 2 s ] 
	en6_Ticks_up  = 0x6,    // Taste long press 3 [ 3 s ]
	en7_Ticks_up = 0x7, 
	enError  = 0x8, 
	en7_Ticks_down = 0x9, 
	en6_Ticks_down = 0xA, 
	en5_Ticks_down = 0xB, 
	en4_Ticks_down = 0xC, 
	en3_Ticks_down = 0xD, 
	en2_Ticks_down = 0xE, 
	en1_Tick_down = 0xF
}ENUM_MFL_EVENTCODE1;



/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

#endif /* _MFL_MGR_CI_H */

/* End Of File MFL_MGR_CI.h */
