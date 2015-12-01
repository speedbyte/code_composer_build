/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	LOGI_BUT_MAN_CI.h
 * \brief	CI - Configuration Internal File.  Detailed description found below.
 * \brief	This file mainly contains of:
 * 			1. constants to switch on or off the internal interfaces. 
 * 			2. module internal constants that can be configured by the module owner.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-11-26 15:36:15 +0100 (Fr, 26 Nov 2010) $
* $Rev: 16558 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LOGI_BUT_MAN/trunk/inc/LOGI_BUT_MAN_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _LOGI_BUT_MAN_CI_H
#define _LOGI_BUT_MAN_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

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
\brief	description of configured interface
*/
#define ILOGI_BUT_MAN_CRADLE_INIT
#define ILOGI_BUT_MAN_3BM_INIT

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
#ifdef ILOGI_BUT_MAN_3BM_INIT

extern SuccessFailType LOGI_BUT_MAN_3BM_sfInit(void);

#endif

#ifdef ILOGI_BUT_MAN_CRADLE_INIT

extern SuccessFailType LOGI_BUT_MAN_CRADLE_sfInit(void);

#endif

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/


/**
\brief	State of the resistor network reported by ADC
*/
typedef enum {
	enOnlyShortPress = 0,
	enShortPressAndLongPress = 1
}_LOGI_BUT_MAN_ENUM_BUTTON_STYLE;


typedef struct {
	BooleanType bLongPressMessageSend;
	BooleanType bShortPressMessageSend;
	Uint16Type u16ButtonCountHanging;
	_LOGI_BUT_MAN_ENUM_BUTTON_STYLE enDeviceButtonProperty;
	PHY_BUT_DRV_ENUM_DYNAMIC_STATE enDeviceState_pre;
	LOGI_BUT_MAN_ENUM_BUTTON_NAMES enDiagnoseButton;
	Uint8Type u8BorderLongPress;
} LOGI_BUT_MAN_ST_DEVICE, *LOGI_BUT_MAN_PST_DEVICE;


#endif /* _LOGI_BUT_MAN_CI_H */

/* End Of File LOGI_BUT_MAN.h */
