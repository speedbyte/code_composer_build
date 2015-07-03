/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	LOGI_BUT_MAN.h
 * \brief	Module Header File. 
 * 			This file must be included by the application/system in order to use
 * 			the interfaces provided by this module. Detailed description found below. 
 * \brief	This file mainly contains:
 * 			1. prototypes of all the functions that can be used by other modules.
 * 			2. enum, structure, constant definition etc. that can be used by other 
 * 			modules.  
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-08-24 09:46:27 +0200 (Di, 24 Aug 2010) $
* $Rev: 14683 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LOGI_BUT_MAN/trunk/inc/LOGI_BUT_MAN.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _LOGI_BUT_MAN_H
#define _LOGI_BUT_MAN_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "LOGI_BUT_MAN_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef enum {
	/* 3 Button start */
	enLogiButMan3bmCountBegin = 0,
	enLogiButMan3bmButton1 = 1,
    enLogiButMan3bmButton2 = 2,
    enLogiButMan3bmButton3 = 3,
    enLogiButMan3bmCountEnd = 4,
    /* 3 Button end */
    /* Cradle start */
    enLogiButManCradleCountBegin = 5,
    enLogiButManCradleButton1 = 6,
    enLogiButManCradleButton2 = 7,
    enLogiButManCradleButtonBoth = 8,
    enLogiButManCradleCountEnd = 9,
    /* Cradle end*/
    /* MFL start */
    enLogiButManMflCountBegin = 10,
    enLogiButManMflUp = 11,
    enLogiButManMflDown = 12,
    enLogiButManMflOk = 13,
    enLogiButManMflReset = 14,
    enLogiButManMflHook = 15,
    enLogiButManMflHookDisallow = 16,
    enLogiButManMflPTT = 17,
    enLogiButManMflBack = 18,
    enLogiButManMflCountEnd = 19,
    /* MFL end */
    /* Virtual Buttons start */
    enLogiButManVirtualButtonInfo = 20,
    enLogiButManVirtualButtonService = 21,
    enLogiButManVirtualButtonSos = 22,
    /* Virtual Buttons end */
    /* LSS Button Start*/
    enLogiButManLssUp = 23,
    enLogiButManLssDown = 24,
    enLogiButManLssOk = 25,
    /* LSS Button End*/
    /* Roll Rad Skoda Start*/
    enLogiButManMflRollUp = 26,
    enLogiButManMflRollDown = 27,
    /* Roll Rad Skoda End*/
    enLogiButManNoButton = 0xFF
}LOGI_BUT_MAN_ENUM_BUTTON_NAMES;


typedef enum {
	enLogiButManDevice3BM = 0,
	enLogiButManDeviceCradle = 1,
	enLogiButManDeviceMfl = 2,
	enLogiButManDeviceMflSkodaRad = 3,
    enLogiButManDeviceCount = 4,
    enLogiButManNoDevice = 0xFF
}LOGI_BUT_MAN_ENUM_DEVICES;


typedef enum {
	enLogiButManShortPress,
	enLogiButManLongPress,
	enLogiButManLongPressRepeat,
	enLogiButManShortPressAtSingleFunction,
	enLogiButManLongPressTenSeconds,	
	enLogiButManNotPressed = 0xFF
} LOGI_BUT_MAN_ENUM_PRESSTYPE;
/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef ILOGI_BUT_MAN_INIT

extern SuccessFailType LOGI_BUT_MAN_sfInit(void);

extern void LOGI_BUT_MAN_vInit3BM(void);

void LOGI_BUT_MAN_vInitBaseplateAndAntenna(void);

#endif

#ifdef ILOGI_BUT_MAN_TASK_APP

extern void LOGI_BUT_MAN_vHandleEvents(EventMaskType event);

#endif


#ifdef ILOGI_BUT_MAN_DIAGNOSE_3BM

extern SuccessFailType LOGI_BUT_MAN_sfSwitch_3Button(BooleanType b3ButtonActiveDiagnoseValue);

extern SuccessFailType LOGI_BUT_MAN_sfSetMode_3Button(Uint8Type u83ButtonModeDiagnoseValue);

extern Uint8Type LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButtonName);

SuccessFailType LOGI_BUT_MAN_sfGetFailureValue(LOGI_BUT_MAN_ENUM_DEVICES enDevice);

extern void LOGI_BUT_MAN_vRollRadDebouncingFunction(EventMaskType event);

#endif 

#ifdef ILOGI_BUT_MAN_DIAGNOSE_CRADLE

SuccessFailType LOGI_BUT_MAN_sfGetFailureValue(LOGI_BUT_MAN_ENUM_DEVICES enDevice);

BooleanType LOGI_BUT_MAN_bGetAntennaDiagnose(void);

BooleanType LOGI_BUT_MAN_bGetBasePlateConnectivity(void);

SuccessFailType LOGI_BUT_MAN_sfSetCradleProtectionStatus(BooleanType bValue);

#endif 


#ifdef ILOGI_BUT_MAN_GET_3BM

extern Uint8Type LOGI_BUT_MAN_u8GetMode_Roofline(void);

extern BooleanType LOGI_BUT_MAN_bGetRooflineModuleState(void);

#endif

extern BooleanType LOGI_BUT_MAN_bGetCradleProtection(void);

#endif /* _LOGI_BUT_MAN_H */

/* End Of File LOGI_BUT_MAN.h */
