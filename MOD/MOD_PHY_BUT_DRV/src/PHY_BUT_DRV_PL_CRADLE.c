/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	PHY_BUT_DRV_PL_CRADLE.c
 * \brief	This file is the physical layer for cradle button handling.
 * \brief	This file fulfills the following requirements:
 * 			1. To get the ADC Values of Baseplate for Skoda and VW
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-02-04 14:53:31 +0100 (Do, 04 Feb 2010) $
* $Rev: 11062 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_PHY_BUT_DRV/branch/MOD_PHY_BUT_DRV_POLO_branch/src/PHY_BUT_DRV_PL_CRADLE.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>
#include "Common.h"
#include "OSEK_HostTask_H1.h"
#include "ADC.h"
#include "SPI.h"
#include "APP_CAN.h"
#include "APP_CSHDL.h"
#include "sds_ext.h"
#include "GIO.h"
#include "MONITOR_ADC.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "PHY_BUT_DRV_PL_CRADLE.id"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "PHY_BUT_DRV.h"
#include "PHY_BUT_DRV_CI.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define DEBUG_CRADLE 1
/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
static Uint8Type _PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
static PHY_BUT_DRV_ST_MESSAGE PHY_BUT_DRV_stButtonState_Cradle;
static TriStateType _PHY_BUT_DRV_bIsCradleConnected = TS_UNKNOWN;
static BooleanType _PHY_BUT_DRV_bIsBasePlateConnected = BT_FALSE;
static BooleanType _PHY_BUT_DRV_bIsTelephoneConnected = BT_FALSE;
static BooleanType _PHY_BUT_DRV_bChargingAllowed = BT_FALSE;
static Uint8Type _PHY_BUT_DRV_u8CurrentVariantType;
#ifdef DEBUG_CRADLE
static BooleanType _PHY_BUT_DRV_InitDone = BT_FALSE;
#endif
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
/* Function    : PHY_BUT_DRV_sfPL_Cradle_Init                                      */
/**
    \brief      Initialise the Cradle for PHY_BUT_DRV module

    \brief      This function will be called by the SYS_INIT during the Startup Phase.
    			All static variables are initialised here.
    
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType PHY_BUT_DRV_sfPL_Cradle_Init(void)
{
    SuccessFailType sF = SUCCESS;
    _PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
    _PHY_BUT_DRV_bIsTelephoneConnected = BT_FALSE;
    _PHY_BUT_DRV_bIsCradleConnected = TS_UNKNOWN;
    _PHY_BUT_DRV_bIsBasePlateConnected = BT_FALSE;
    _PHY_BUT_DRV_bChargingAllowed = BT_FALSE;
	PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enDefaultState;
	PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManNoButton;
	PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
#ifdef DEBUG_CRADLE
	_PHY_BUT_DRV_InitDone = BT_FALSE;
#endif
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    :   _PHY_BUT_DRV_enGetState_Cradle                                        */
/**
    \brief      reports in which voltage band the resitor array is.

    \brief      the ADC reports a voltage level dependend which button is pressed.
                these voltage levels are mapped to button state by the voltage
				boundaries.

    \return     PHY_BUT_DRV_ST_MESSAGE

   */
/*----------------------------------------------------------------------------*/
PHY_BUT_DRV_ST_MESSAGE _PHY_BUT_DRV_enGetState_Cradle(void)
{
    Uint16Type u16AdcValue = 0;
	Uint16Type u16AdcValueCradle = ADC_U16GETADCVALUE(ADC_BASEPLATE);
    Uint16Type u16AdcValueSpannung = ADC_U16GETADCVALUE(ADC_UBATT);
    u16AdcValue = u16AdcValueCradle;
#if 0  // For B2 muster ( cradle was anyways not available here. Still for some self modified boards, it is needed.)
    u16AdcValue = (Uint16Type) ((u16AdcValueCradle * 666)/(u16AdcValueSpannung));
#endif
	PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enDefaultState;
	PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManNoButton;
	PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_DEBUG_3, UHV_PHY_BUT_DRV , "Cradle ADC Value = %d", u16AdcValue);
#endif

	if ( _PHY_BUT_DRV_u8CurrentVariantType == 0x01 )
	{
		if ( APP_CAN_enGetMarke() != APP_CAN_SK_Skoda  )
		{
			if ( _PHY_BUT_DRV_bChargingAllowed == BT_TRUE )
			{
				if ( (u16AdcValue > _PHY_BUT_DRV_CRADLE_MINBUTTON_BOTH_VW) && (u16AdcValue < _PHY_BUT_DRV_CRADLE_MINBUTTON1_VW) )
			    {

					_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Both buttons = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
			    		PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButtonBoth;
			    		PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
			    		_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
			    		_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_CRADLE_MINBUTTON2_VW )
			    {

					_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Info = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
						PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButton1;
						PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
						_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
						_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_CRADLE_MINNOTPRESSED_VW )
			    {
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Service = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
						PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButton2;
						PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
						_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
						_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_CRADLE_MAXNOTPRESSED_VW )
			    {
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == 0)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_2, UHV_PHY_BUT_DRV , "Charging Cradle ADC Value, VCradle Present / No Button pressed = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enNoTransition;
						PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManNoButton;
						PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
						_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
				    	_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
				    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
#ifdef DEBUG_CRADLE
				    	if ( _PHY_BUT_DRV_InitDone == BT_FALSE )
				    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
				    		(void)DEBUG_Printf(UHV_SPECIAL, UHV_OUTPUT, "SYS_PERF:Cradle Detekt:%dms,%d",osGetSystemCounter(), __LINE__);
#endif
				    		DEBUG_VALUE1(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "SYS_PERF: Detection of Cradle in Charging Mode without Button press:%dms",osGetSystemCounter());
				    		_PHY_BUT_DRV_InitDone = BT_TRUE;
				    	}
#endif
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MAX_CRADLE_PRESENT_VW)
			    {
			    	_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
			    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
#ifdef DEBUG_CRADLE
				    	if ( _PHY_BUT_DRV_InitDone == BT_FALSE )
				    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
				    		DEBUG_VALUE1(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "SYS_PERF: Detection of Cradle in Charging Mode without Button press:%dms",osGetSystemCounter());
#endif
				    	}
#endif
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MAX_BASEPLATE_PRESENT_VW)
			    {
			    	_PHY_BUT_DRV_bIsCradleConnected = TS_FALSE;
			    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
#ifdef DEBUG_CRADLE
			    	_PHY_BUT_DRV_InitDone = BT_FALSE;
#endif
			    }
			    else
			    {
			    	_PHY_BUT_DRV_bIsCradleConnected = TS_UNKNOWN;
			    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_FALSE;
#ifdef DEBUG_CRADLE
			    	_PHY_BUT_DRV_InitDone = BT_FALSE;
#endif

			    }
			}
			else if ( _PHY_BUT_DRV_bChargingAllowed == BT_FALSE )
			{
				if ( (u16AdcValue > _PHY_BUT_DRV_CRADLESHUT_MINBUTTON_BOTH_VW) && (u16AdcValue < _PHY_BUT_DRV_CRADLESHUT_MINBUTTON1_VW) )
			    {

					_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Both buttons = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
			    		PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButtonBoth;
			    		PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
			    		_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
			    		_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_CRADLESHUT_MINBUTTON2_VW )
			    {

					_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Info = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
						PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButton1;
						PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
						_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
						_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MIN_CRADLE_PRESENT_VW )
			    {
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
					DEBUG_VALUE1(PHY_BUT_DRV_SWI_DBG, MOD_PHY_BUT_DRV, "Counter = %d", _PHY_BUT_DRV_u8ValidityUpCount_Cradle);
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Service = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
						PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButton2;
						PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
						_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
						_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MAX_CRADLE_PRESENT_VW )
			    {
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
					if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == 0)
			    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
						(void)DEBUG_Printf(UHV_DEBUG_2, UHV_PHY_BUT_DRV , "Non Charging Cradle ADC Value, VCradle Present / No Button pressed = %d", u16AdcValue);
#endif

						PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enNoTransition;
						PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManNoButton;
						PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
						_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
				    	_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
				    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
#ifdef DEBUG_CRADLE
				    	if ( _PHY_BUT_DRV_InitDone == BT_FALSE )
				    	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
				    		(void)DEBUG_Printf(UHV_SPECIAL, UHV_OUTPUT, "SYS_PERF:Cradle Detekt:%dms,%d",osGetSystemCounter(), __LINE__);
#endif
				    		DEBUG_VALUE1(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "SYS_PERF: Detection of Cradle in Uncharging Mode without Button press:%dms",osGetSystemCounter());
				    		_PHY_BUT_DRV_InitDone = BT_TRUE;
				    	}
#endif
			    	}
			    }
			    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MAX_BASEPLATE_PRESENT_VW)
			    {
			    	_PHY_BUT_DRV_bIsCradleConnected = TS_FALSE;
			    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
#ifdef DEBUG_CRADLE
			    	_PHY_BUT_DRV_InitDone = BT_FALSE;
#endif

			    }
			    else
			    {
			    	_PHY_BUT_DRV_bIsCradleConnected = TS_UNKNOWN;
			    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_FALSE;
#ifdef DEBUG_CRADLE
			    	_PHY_BUT_DRV_InitDone = BT_FALSE;
#endif
			    }
			}
		}
		else if ( APP_CAN_enGetMarke() == APP_CAN_SK_Skoda )
		{
		    if ( (u16AdcValue > _PHY_BUT_DRV_CRADLE_MINBUTTON1_SKODA ) && (u16AdcValue < _PHY_BUT_DRV_CRADLE_MINBUTTON2_SKODA )) 
		    {
		    	
				_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
				if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)	
		    		{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
					(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Info = %d", u16AdcValue);
#endif

					PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
					PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButton1;
					PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
		    		}
		    } 
		    else if ( u16AdcValue < _PHY_BUT_DRV_CRADLE_MINNOTPRESSED_SKODA ) 
		    {
				_PHY_BUT_DRV_u8ValidityUpCount_Cradle++;
				if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == PHY_BUT_DRV_UP_COUNTER)	
		    		{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
					(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Cradle ADC Value, Sos = %d", u16AdcValue);
#endif

					PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enButtonPressed_PosFlanke;
					PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManCradleButton2;
					PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
		    		}
		    } 
		    else if ( u16AdcValue < _PHY_BUT_DRV_CRADLE_MAXNOTPRESSED_SKODA ) 
		    {
				_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
				if (_PHY_BUT_DRV_u8ValidityUpCount_Cradle == 0)
		    		{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
					(void)DEBUG_Printf(UHV_DEBUG_2, UHV_PHY_BUT_DRV , "Cradle ADC Value, VCradle Present / No Button pressed = %d", u16AdcValue);
#endif

					PHY_BUT_DRV_stButtonState_Cradle.enButtonState= enNoTransition;
					PHY_BUT_DRV_stButtonState_Cradle.enButtonName = enLogiButManNoButton;
					PHY_BUT_DRV_stButtonState_Cradle.enDeviceName = enLogiButManDeviceCradle;
					_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 0;
			    	_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
			    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
		    		}
		    }
		    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MAX_CRADLE_PRESENT_SKODA)
		    {
		    	_PHY_BUT_DRV_bIsCradleConnected = TS_TRUE;
		    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
		    }
		    else if ( u16AdcValue < _PHY_BUT_DRV_BUTTON_MAX_BASEPLATE_PRESENT_SKODA)
		    {
		    	_PHY_BUT_DRV_bIsCradleConnected = TS_FALSE;
		    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_TRUE;
		    }
		    else
		    {
		    	_PHY_BUT_DRV_bIsCradleConnected = TS_UNKNOWN;
		    	_PHY_BUT_DRV_bIsBasePlateConnected = BT_FALSE;
		    }   
		}
	}
	else
	{
		_PHY_BUT_DRV_bIsCradleConnected = TS_FALSE;
		_PHY_BUT_DRV_bIsBasePlateConnected = BT_FALSE;
		_PHY_BUT_DRV_bIsTelephoneConnected = BT_FALSE;
	}	
    return (PHY_BUT_DRV_stButtonState_Cradle);
}

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_bGetTelephoneConnectivity                      */
/**
    \brief      Get Value if the telephone is connected or not.
    			According to the HW Documentation:
				Cradle Wake-up:
				Cell phone inserted: GPIO(SPI5CLK) = Low
				No cell phone: GPIO(SPI5CLK) = High

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
BooleanType PHY_BUT_DRV_bGetTelephoneConnectivity(void)
{
	BooleanType bReadWakeUpPin = SPI_bPinRead(SPI5, SpiClkPin) ; 
	(bReadWakeUpPin == BT_FALSE) ? (_PHY_BUT_DRV_bIsTelephoneConnected = BT_TRUE) : (_PHY_BUT_DRV_bIsTelephoneConnected = BT_FALSE) ;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Telephone is %d", _PHY_BUT_DRV_bIsTelephoneConnected);
#endif
	DEBUG_VALUE1(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Telephone is %d", _PHY_BUT_DRV_bIsTelephoneConnected);
	return (_PHY_BUT_DRV_bIsTelephoneConnected);
}

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_bGetCradleConnectivity                      */
/**
    \brief      Get Value if the cradle is connected or not.


    \return     TriStateType

   */
/*----------------------------------------------------------------------------*/
TriStateType PHY_BUT_DRV_bGetCradleConnectivity(void)
{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_DEBUG_3, UHV_PHY_BUT_DRV, "Cradle is %d", _PHY_BUT_DRV_bIsCradleConnected);
#endif

	return (_PHY_BUT_DRV_bIsCradleConnected);
}

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_bGetCradleConnectivity                      */
/**
    \brief      Get Value if the cradle is connected or not.


    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
BooleanType PHY_BUT_DRV_bGetBasePlateConnectivity(void)
{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Baseplate is %d", _PHY_BUT_DRV_bIsBasePlateConnected);
#endif
	DEBUG_VALUE1(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Baseplate is %d", _PHY_BUT_DRV_bIsBasePlateConnected);
	return (_PHY_BUT_DRV_bIsBasePlateConnected);
}

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_vOnAppCshdlNotification                                         */
/**
    \brief      to control Baseplate Voltage.

    \brief		Akkulader/Bseplate is switched on in state KL15 ON and UHV-AKTIV otherwise it is OFF.

    \param      notification

    \return     void
   */
/*----------------------------------------------------------------------------*/
void PHY_BUT_DRV_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV, "App Cshdl Notifctin = %d", notification);
#endif

	switch(notification)
    {
	    case APP_CSHDL_NOTIFY_KL_15_OFF:  // Transition from KL15 ein to KL15 Aus / Radio Ein.
	    case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF: // Transition from KL15 Aus / Radio Aus to KL15 Aus / Radio Ein.
			if ( APP_CAN_enGetMarke()  != APP_CAN_SK_Skoda  )
			{
				LOGI_BUT_MAN_CRADLE_OFF  // The SOS call and charging mechansim is different for VW Cradles.
			}
			else
			{
				LOGI_BUT_MAN_CRADLE_ON // we have to do this because otherwise, SOS call would not be made.
			}
			LOGI_BUT_MAN_RLADDER_ON
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		    (void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Switch on Baseplate without charge");
#endif
			DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Switch on Baseplate without charge");
			/*
			 * \bugfix : RT#5318, 23.04.2010, VA.
			 */
			_PHY_BUT_DRV_u8ValidityUpCount_Cradle = 245;
			_PHY_BUT_DRV_bChargingAllowed = BT_FALSE;
			break;
	    case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF:  // Transition from KL15 ein to KL15 Aus / Radio Aus.
			LOGI_BUT_MAN_CRADLE_OFF
			LOGI_BUT_MAN_RLADDER_OFF
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		    (void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Switch off Baseplate");
#endif
			DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Switch off Baseplate");
			_PHY_BUT_DRV_bChargingAllowed = BT_FALSE;
			break;
		case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON:  // Direct Transition in KL15 ein.
			LOGI_BUT_MAN_CRADLE_ON
			LOGI_BUT_MAN_RLADDER_ON
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Switch on Baseplate with Charge");
#endif
			DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Switch on Baseplate with Charge");
			_PHY_BUT_DRV_bChargingAllowed = BT_TRUE;
			break;
		default:
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		    (void)DEBUG_Printf(UHV_DEBUG, UHV_PHY_BUT_DRV, "ignoring cshdl notiId %d", notification);
#endif

		    break;
	}
}

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_sfSwitchBasePlateVoltage                        */
/**
    \brief      Switch on or Off Voltage on Baseplate

    \param 		u8Value

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
SuccessFailType PHY_BUT_DRV_sfSwitchBasePlateVoltageDiagnostics(Uint8Type u8Value)
{
	SuccessFailType sF = SUCCESS;
	if ( u8Value == SERVICE_2F_FKT_START_EIN)
	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Cradle On");
#endif
		DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Cradle On");
		LOGI_BUT_MAN_CRADLE_ON
		// _PHY_BUT_DRV_bIsDiagnosticsOn = BT_TRUE; could be used if the baseplate fn shud be worked out too.
	}
	else if ( u8Value == SERVICE_2F_FKT_START_AUS )
	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Cradle Off");
#endif
		DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Cradle Off");
		LOGI_BUT_MAN_CRADLE_OFF
		// _PHY_BUT_DRV_bIsDiagnosticsOn = BT_FALSE; could be used if the baseplate fn shud be worked out too.
	}
	else if ( u8Value == SERVICE_2F_FKT_STOP_LAST_STATE )
	{
		if ( _PHY_BUT_DRV_bIsBasePlateConnected == BT_TRUE )
		{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Restore Cradle to On");
#endif
			DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Restore Cradle to On");
			LOGI_BUT_MAN_CRADLE_ON
		}
		else if ( _PHY_BUT_DRV_bIsBasePlateConnected == BT_FALSE )
		{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV, "Restore Cradle to Off");
#endif
			DEBUG_TEXT(PHY_BUT_DRV_SWI_TRC, MOD_PHY_BUT_DRV, "Restore Cradle to Off");
			LOGI_BUT_MAN_CRADLE_OFF
		}
	}
	return sF;
}

void PHY_BUT_DRV_vStartup(void)
{
	(void)EEPROM_enGetSingleByteFromRamMirror(&_PHY_BUT_DRV_u8CurrentVariantType, EEPROM_LAYOUT_ENUM_UHV_Variant);
}

/*----------------------------------------------------------------------------*/
/* Function    : PHY_BUT_DRV_u8GetVariantType                                   */
/**
    \brief      returns the UHV Variant

    \brief

0: UHV ohne Cradle mit Sprachbedienung
1: UHV mit Cradle und Sprachbedienung
2: UHV ohne Cradle und ohne Sprachbedienung
3 bis 255: Reserviert

    \return		Uint8Type
				Variant Type


   */
/*----------------------------------------------------------------------------*/
Uint8Type PHY_BUT_DRV_u8GetVariantType(void)
{
    return  (_PHY_BUT_DRV_u8CurrentVariantType);
}


/* End Of File PHY_BUT_DRV_PL_CRADLE.c */
