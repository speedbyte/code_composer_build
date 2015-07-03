/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     PHY_BUT_DRV_PL_3BM.c
*-----------------------------------------------------------------------------
* Module Name:  PHY_BUT_DRV
*-----------------------------------------------------------------------------
* Description: To get the ADC Values of 3 Button Module
*-----------------------------------------------------------------------------
* $Date: 2009-02-09 20:53:33 +0100 (Mo, 09 Feb 2009) $
* $Rev: 4534 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_PHY_BUT_DRV/trunk/src/PHY_BUT_DRV_PL_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>
#include "ADC.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "PHY_BUT_DRV.h"
#include "PHY_BUT_DRV_CI.h"

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

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/
static Uint8Type _PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
static PHY_BUT_DRV_ST_MESSAGE PHY_BUT_DRV_stButtonState_3BM;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_sfPL_3BM_Init                                      */
/**
    \brief      Initialise the 3BM for PHY_BUT_DRV module

    \brief      This function will be called by the SYS_INIT during the Startup Phase.
    			All static variables are initialised here.
    
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType PHY_BUT_DRV_sfPL_3BM_Init(void)
{
    SuccessFailType sF = SUCCESS;
    _PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
	PHY_BUT_DRV_stButtonState_3BM.enButtonState= enDefaultState;
	PHY_BUT_DRV_stButtonState_3BM.enButtonName = enLogiButManNoButton;
	PHY_BUT_DRV_stButtonState_3BM.enDeviceName = enLogiButManDevice3BM;
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    :   _PHY_BUT_DRV_enGetState_3BM                                        */
/**
    \brief      reports in which voltage band the resitor array is.

    \brief      the ADC reports a voltage level dependend which button is pressed.
                these voltage levels are mapped to button state by the voltage
				boundaries.

    \return     PHY_BUT_DRV_ST_MESSAGE
    			button state
   */
/*----------------------------------------------------------------------------*/
PHY_BUT_DRV_ST_MESSAGE _PHY_BUT_DRV_enGetState_3BM(void)
{
    Uint16Type u16AdcValue = ADC_U16GETADCVALUE(ADC_3BM);
	PHY_BUT_DRV_stButtonState_3BM.enButtonState= enDefaultState;
	PHY_BUT_DRV_stButtonState_3BM.enButtonName = enLogiButManNoButton;
	PHY_BUT_DRV_stButtonState_3BM.enDeviceName = enLogiButManDevice3BM;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_DEBUG_2, UHV_PHY_BUT_DRV , "3BM ADC Value = %d", u16AdcValue);
#endif

	if ( (u16AdcValue > _PHY_BUT_DRV_3BM_MINBUTTON1) && (u16AdcValue < _PHY_BUT_DRV_3BM_MINBUTTON2) )
	{
		_PHY_BUT_DRV_u8ValidityUpCount_3BM++;
		if (_PHY_BUT_DRV_u8ValidityUpCount_3BM == PHY_BUT_DRV_UP_COUNTER)
			{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "ADC Value, Mute = %d", u16AdcValue);
#endif

			PHY_BUT_DRV_stButtonState_3BM.enButtonState= enButtonPressed_PosFlanke;
			PHY_BUT_DRV_stButtonState_3BM.enButtonName = enLogiButMan3bmButton1;
			PHY_BUT_DRV_stButtonState_3BM.enDeviceName = enLogiButManDevice3BM;
			_PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
			}
	}
	else if ( u16AdcValue < _PHY_BUT_DRV_3BM_MINBUTTON3 )
	{
		_PHY_BUT_DRV_u8ValidityUpCount_3BM++;
		if (_PHY_BUT_DRV_u8ValidityUpCount_3BM == PHY_BUT_DRV_UP_COUNTER)
			{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "ADC Value, VCR = %d", u16AdcValue);
#endif

			PHY_BUT_DRV_stButtonState_3BM.enButtonState= enButtonPressed_PosFlanke;
			PHY_BUT_DRV_stButtonState_3BM.enButtonName = enLogiButMan3bmButton2;
			PHY_BUT_DRV_stButtonState_3BM.enDeviceName = enLogiButManDevice3BM;
			_PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
			}
	}
	else if ( u16AdcValue < _PHY_BUT_DRV_3BM_MINNOTPRESSED)
	{
		_PHY_BUT_DRV_u8ValidityUpCount_3BM++;
		if (_PHY_BUT_DRV_u8ValidityUpCount_3BM == PHY_BUT_DRV_UP_COUNTER)
			{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "ADC Value, BT = %d", u16AdcValue);
#endif

			PHY_BUT_DRV_stButtonState_3BM.enButtonState= enButtonPressed_PosFlanke;
			PHY_BUT_DRV_stButtonState_3BM.enButtonName = enLogiButMan3bmButton3;
			PHY_BUT_DRV_stButtonState_3BM.enDeviceName = enLogiButManDevice3BM;
			_PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
			}
	}
	else if ( u16AdcValue < _PHY_BUT_DRV_3BM_MAXNOTPRESSED)
	{
		_PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
		if (_PHY_BUT_DRV_u8ValidityUpCount_3BM == 0)
			{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_DEBUG_2, UHV_PHY_BUT_DRV , "ADC Value, No button = %d", u16AdcValue);
#endif

			PHY_BUT_DRV_stButtonState_3BM.enButtonState= enNoTransition;
			PHY_BUT_DRV_stButtonState_3BM.enButtonName = enLogiButManNoButton;
			PHY_BUT_DRV_stButtonState_3BM.enDeviceName = enLogiButManDevice3BM;
			_PHY_BUT_DRV_u8ValidityUpCount_3BM = 0;
			}
	}
    return (PHY_BUT_DRV_stButtonState_3BM);
}

/* End Of File PHY_BUT_DRV_PL_3BM.c */
