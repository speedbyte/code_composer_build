/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	LOGI_BUT_MAN_3BM.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-12-08 09:41:12 +0100 (Mi, 08 Dez 2010) $
* $Rev: 16812 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LOGI_BUT_MAN/trunk/src/LOGI_BUT_MAN_3BM.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h" // includes the 3bm enums
#include "PORT.h"
#include "GIO.h"
#include "VCR.h"
#include "APP_PHONE.h"
#include "PORT.h"
#include "APP_DIAG.h"
#include "EEPROM_LAYOUT_CFG.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "LOGI_BUT_MAN.h"
#include "LOGI_BUT_MAN_CI.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

static BooleanType _LOGI_BUT_MAN_b3BMInitDone = BT_FALSE;
/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfInit                                          */
/**
    \brief      Initialise the LOGI_BUT_MAN module

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType LOGI_BUT_MAN_3BM_sfInit(void)
{
	if ( _LOGI_BUT_MAN_b3BMInitDone ) 
	{
		return FAIL;
    }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG, UHV_LOGI_BUT_MAN, "Start Init");
#endif
    _LOGI_BUT_MAN_b3BMInitDone = BT_TRUE;
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_u8GetMode_Roofline                                 */
/**
    \brief      to set the mode of 3 button value.

    \brief      
				Get The mode which has been set by Diagnose.
				$00: vehicle detection for 3 button modul
				$01: PTT + Call-Handling
				$02: only PTT
				
    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type LOGI_BUT_MAN_u8GetMode_Roofline(void)
{
	Uint8Type u8RooflineModuleMode;
	EEPROM_LAYOUT_vGetRooflineMode(&u8RooflineModuleMode);
	return u8RooflineModuleMode;
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_bGetRooflineModuleState                                   */
/**
    \brief      to set the mode of 3 button value.

    \brief      
				Get The mode which has been set by Diagnose.
				$00: vehicle detection for 3 button modul
				$01: PTT + Call-Handling
				$02: only PTT
				
    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType LOGI_BUT_MAN_bGetRooflineModuleState(void)
{
	Uint8Type u8RooflineModuleMode;
	BooleanType bIsRooflineModuleActive ;
	EEPROM_LAYOUT_vGetRooflineMode(&u8RooflineModuleMode);
	if (( u8RooflineModuleMode == (Uint8Type)APP_DIAG_ROOFLINE_MODULE_TYPE_NAR ) ||
			( u8RooflineModuleMode == (Uint8Type)APP_DIAG_ROOFLINE_MODULE_TYPE_RDW )
	)
	{
		bIsRooflineModuleActive = BT_TRUE;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_DEBUG, UHV_LOGI_BUT_MAN, "switchon Roofline,Mode %d", u8RooflineModuleMode);
#endif
	}
	else if ( u8RooflineModuleMode == APP_DIAG_ROOFLINE_MODULE_TYPE_NA )
	{
		bIsRooflineModuleActive = BT_FALSE;
		//LOGI_BUT_MAN_RLADDER_OFF
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_DEBUG, UHV_LOGI_BUT_MAN, "switchoff Roofline");
#endif
	}
	else
	{
		bIsRooflineModuleActive = BT_FALSE;
		//LOGI_BUT_MAN_RLADDER_OFF
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_DEBUG, UHV_LOGI_BUT_MAN, "switchoff Roofline");
#endif
	}
	return bIsRooflineModuleActive;
}

void LOGI_BUT_MAN_vInit3BM(void)
{
	//EEPROM_LAYOUT_vGet3BMMode(&_LOGI_BUT_MAN_u8RooflineModuleMode);
}

/* End Of File LOGI_BUT_MAN_CRADLE.c */
