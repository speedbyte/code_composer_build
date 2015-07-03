/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     Pwrmgr.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_POWERMANAGEMENT
*-----------------------------------------------------------------------------
* Description:    Power Management methods.
*-----------------------------------------------------------------------------
* $Date: 2011-05-02 11:13:43 +0200 (Mo, 02 Mai 2011) $
* $Rev: 19426 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POWERMANAGER/trunk/src/Pwrmgr_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "Pwrmgr.h"
#include "ADC.h"
#include "APP_CAN.h"

#include "Pwrmgr_C1.id"
#include "TRACE.h"
#include "MONITOR_ADC.h"
#include "APP_DTC.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/**
    \brief	example
*/


/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the type
*/
typedef enum
{
	enLowVoltage,
	enHighVoltage
} ENUM_POWER_STAGE;
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _PwrmgrEnterStateNormalLowRecover(void);
static void _PwrmgrEnterStateNormalHighRecover(void);
static void _PwrmgrEnterStateNormalLow(void);
static void _PwrmgrEnterStateNormalHigh(void);
static void _PwrmgrEnterStateNormal(void);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief	 description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private variable
*/

static PwrmgrStateType         pwrmgrState = PWRMGR_STATE_NORMAL;
static PWMGR_enVoltageStates   voltageStates;
static FlagType                _pwrmgrCurrentScope = FALSE;
static FlagType                _pwrmgrCurrentScopeEigenDiag = FALSE;
static Uint16Type              pwrmgrVoltage = 0;
static Uint16Type              pwrmgrVoltageForRapidVoltageChanges = 0;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function    : PwrmgrManager                                  */
/** 
    \brief      check current Powermanager state
  
    \brief      Description:
                function checks the current Powermanager state and changes
                the powermanager state. Attention - Cyclic call over monitor task every 2s.

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType Pwrmgr_Manager(void)
{
    SuccessFailType sF = SUCCESS;

	pwrmgrVoltage = MONITOR_ADC_u16GetMovingAverageADCValueUBatt();
	DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "ADCValueUBatt = %d", pwrmgrVoltage);

    switch (pwrmgrState)
    {
        /* voltage range normal 8,5V - 15,5V            */
        /* after over or under voltage 9V - 15V         */
        case PWRMGR_STATE_NORMAL:
            if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_HIGH)
            {
                _PwrmgrEnterStateNormalHigh();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _pwrmgrCurrentScope = TRUE;
            }
            else if (pwrmgrVoltage < BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormalLow();
            }
            else
            {
                /* do nothing */
            }
            break;

        /* voltage range normal 8,5V - 9V               */
        case PWRMGR_STATE_NORMAL_LOW_RECOVER:
            if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_HIGH)
            {
                _PwrmgrEnterStateNormalHigh();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW_RECOVER)
            {
                _PwrmgrEnterStateNormal();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW)
            {
            }            
            else if (pwrmgrVoltage < BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormalLow();
            }
            else
            {
                /* do nothing */
            }
            break;

        /* voltage range normal 15,5V - 15V               */
        case PWRMGR_STATE_HIGH_RECOVER:
            if (pwrmgrVoltage > BATTERY_VOLTAGE_NORMAL_HIGH)
            {
                _PwrmgrEnterStateNormalHigh();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_HIGH_RECOVER)
            {
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormal();
            }
            else if (pwrmgrVoltage < BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormalLow();
            }
            else
            {
                /* do nothing */
            }
            break;

        case PWRMGR_STATE_NORMAL_LOW:
            if (pwrmgrVoltage > BATTERY_VOLTAGE_NORMAL_HIGH)
            {
                _PwrmgrEnterStateNormalHigh();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW_RECOVER)
            {
                _PwrmgrEnterStateNormal();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormalLowRecover();
            }
            else if (pwrmgrVoltage < BATTERY_VOLTAGE_NORMAL_LOW)
            {
            }
            else
            {
                /* do nothing */
            }
            break;
            
        case PWRMGR_STATE_NORMAL_HIGH:
            if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_HIGH)
            {
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_HIGH_RECOVER)
            {
                _PwrmgrEnterStateNormalHighRecover();
            }
            else if (pwrmgrVoltage >= BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormal();
            }
            else if (pwrmgrVoltage < BATTERY_VOLTAGE_NORMAL_LOW)
            {
                _PwrmgrEnterStateNormalLow();
            }
            else
            {
                /* do nothing */
            }
            break;

        default:
            break;
    }
    return(sF);
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_Manager_Handler_For_Rapid_Voltage_Changes             */
/**
    \brief      check current Power manager state in case rapid voltage changes

    \brief      Description:
                function checks the current power manager state and changes
                the powermanager state n case rapid voltage changes.
                Cyclic call over monitor task every 50ms.

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType Pwrmgr_Manager_Handler_For_Rapid_Voltage_Changes(void)
{
    SuccessFailType sF = SUCCESS;

    /* Get UBATT direct from ADC */
	pwrmgrVoltageForRapidVoltageChanges = ADC_U16GETADCVALUE(ADC_UBATT);
	DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "ADCValueUBatt direct= %d", pwrmgrVoltageForRapidVoltageChanges);

    if (pwrmgrVoltageForRapidVoltageChanges > (BATTERY_VOLTAGE_EIGENDIAG_HIGH))
    {
    	_pwrmgrCurrentScopeEigenDiag = FALSE;
    }
    else if (pwrmgrVoltageForRapidVoltageChanges < (BATTERY_VOLTAGE_EIGENDIAG_LOW_9_15))
    {
        _pwrmgrCurrentScopeEigenDiag = FALSE;
    }
    else if ((pwrmgrVoltageForRapidVoltageChanges >= (BATTERY_VOLTAGE_EIGENDIAG_LOW_RECOVER)) &&
    		(pwrmgrVoltageForRapidVoltageChanges <= (BATTERY_VOLTAGE_EIGENDIAG_HIGH_RECOVER)))
    {

    	_pwrmgrCurrentScopeEigenDiag = TRUE;
    }
    else
    {
    	/* Do nothing - we are between the hysterese borders                                      */
    	/* No change of the _pwrmgrCurrentScopeEigenDiag state necessary  because old state is OK */
    }

    /* no Eigendiagnose - Reset all necessary counters  */
    if(FALSE == _pwrmgrCurrentScopeEigenDiag)
	{
		(void)APP_CAN_sfReInitTOVariables();   // additional for weiche Kommunikation as part of start/stopp
	}

    return(sF);
}

/*----------------------------------------------------------------------------*/
/* Function    : PwrmgrInit                                                   */
/** 
    \brief      initialize Powermanager
  
    \brief      Description:
                initialization for Powermanager.

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType Pwrmgr_Init(void)
{
    SuccessFailType sF = SUCCESS;

    _PwrmgrEnterStateNormal();
    _pwrmgrCurrentScope = FALSE;

    return(sF);
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_Handler_Init_For_Rapid_Voltage_Changes                 */
/**
    \brief      initialize Powermanager for rapid voltage changes

    \brief      Description:
                initialization for Powermanager for rapid voltage changes

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType Pwrmgr_Handler_Init_For_Rapid_Voltage_Changes(void)
{
    SuccessFailType sF = SUCCESS;

    _pwrmgrCurrentScopeEigenDiag = FALSE;

    return(sF);
}

/****************************************************************************/
/*  Method:     _PwrmgrEnterStateNormalLowRecover                            */
/****************************************************************************/
static void _PwrmgrEnterStateNormalLowRecover(void)
{
    /* depending on what the requirements set 
       mute the Micro or Loudspeaker
    */
    voltageStates = PWMGR_NormalLowRecoverVoltage;
    _pwrmgrCurrentScope = FALSE;
    pwrmgrState        = PWRMGR_STATE_NORMAL_LOW_RECOVER;

    DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "_PwrmgrEnterStateNormalLowRecover _pwrmgrCurrentScope %d", _pwrmgrCurrentScope);
    if (E_OK != SendMessage(msgPwrmgrState, &voltageStates))
    {
        /* write a trace here */
    }
}
/****************************************************************************/
/*  Method:     _PwrmgrEnterStateNormal                                      */
/****************************************************************************/
static void _PwrmgrEnterStateNormal(void)
{  
    voltageStates      = PWMGR_NormalVoltage;
    _pwrmgrCurrentScope = TRUE;
    pwrmgrState        = PWRMGR_STATE_NORMAL;

    DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "_PwrmgrEnterStateNormal _pwrmgrCurrentScope %d", _pwrmgrCurrentScope);
    if (E_OK != SendMessage(msgPwrmgrState, &voltageStates))
    {
        /* write a trace here */
    }
}
/****************************************************************************/
/*  Method:     _PwrmgrEnterStateNormalHighRecover                                 */
/****************************************************************************/
static void _PwrmgrEnterStateNormalHighRecover(void)
{
    voltageStates      = PWMGR_NormalHighRecoverVoltage;
    _pwrmgrCurrentScope = FALSE;
    pwrmgrState        = PWRMGR_STATE_HIGH_RECOVER;

    DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "_PwrmgrEnterStateNormalHighRecover _pwrmgrCurrentScope %d", _pwrmgrCurrentScope);
    if (E_OK != SendMessage(msgPwrmgrState, &voltageStates))
    {
        /* write a trace here */
    }
}

/****************************************************************************/
/*  Method:     _PwrmgrEnterStateNormalLow                                  */
/****************************************************************************/
static void _PwrmgrEnterStateNormalLow(void)
{
    voltageStates      = PWMGR_NormalLowVoltage;
    _pwrmgrCurrentScope = FALSE;
    pwrmgrState        = PWRMGR_STATE_NORMAL_LOW;

    DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "_PwrmgrEnterStateNormalLow _pwrmgrCurrentScope %d", _pwrmgrCurrentScope);
    if (E_OK != SendMessage(msgPwrmgrState, &voltageStates))
    {
        /* write a trace here */
    }
}

/****************************************************************************/
/*  Method:     _PwrmgrEnterStateNormalHigh                                  */
/****************************************************************************/
static void _PwrmgrEnterStateNormalHigh(void)
{
    voltageStates      = PWMGR_NormalHighVoltage;
    _pwrmgrCurrentScope = FALSE;
    pwrmgrState        = PWRMGR_STATE_NORMAL_HIGH;

    DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, "_PwrmgrEnterStateNormalHigh _pwrmgrCurrentScope %d", _pwrmgrCurrentScope);
    if (E_OK != SendMessage(msgPwrmgrState, &voltageStates))
    {
        /* write a trace here */
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_GetPowerManagerState                                  */
/** 
    \brief      Get current Powermanager state
  
    \brief      Description:
                function deliver the current Powermanager state to other modules

    \return     PwrmgrStateType 
                PWRMGR_STATE_NORMAL, PWRMGR_STATE_NORMAL_LOW_RECOVER,
                PWRMGR_STATE_HIGH_RECOVER, PWRMGR_STATE_NORMAL_HIGH,
                PWRMGR_STATE_NORMAL_LOW, PWRMGR_STATE_SHUTDOWN,
                PWRMGR_STATE_IS_SHUTDOWN
*/
/*----------------------------------------------------------------------------*/
PwrmgrStateType Pwrmgr_GetPowerManagerState(void)
{
    return (pwrmgrState);
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_GetCurrentScope                                  */
/** 
    \brief      Get currentScope state
  
    \brief      Description:
                function deliver the current Scope state to other modules

    \return     FlagType 
*/
/*----------------------------------------------------------------------------*/
FlagType Pwrmgr_GetCurrentScope(void)
{
    return (_pwrmgrCurrentScope);
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_GetCurrentScope                                  */
/**
    \brief      Get currentScope state

    \brief      Description:
                function deliver the current Scope state to other modules

    \return     FlagType
*/
/*----------------------------------------------------------------------------*/
FlagType Pwrmgr_GetCurrentScopeEigenDiag(void)
{
	DEBUG_VALUE1(POWERMANAGER_SWI_DBG, MOD_POWERMANAGER, " PwrMgr CurrentScopeEigenDiag %d", _pwrmgrCurrentScopeEigenDiag);
	return (_pwrmgrCurrentScopeEigenDiag);
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_GetVoltageState                                  */
/**
    \brief      Get currentvoltage state

    \brief      Description:
                function deliver the current voltage state to other modules

    \return     UinPWMGR_enVoltageStatest16Type
*/
/*----------------------------------------------------------------------------*/
PWMGR_enVoltageStates Pwrmgr_GetVoltageState(void)
{
    return (voltageStates);
}

/*----------------------------------------------------------------------------*/
/* Function    : Pwrmgr_GetRapidPwrmgrVoltage                                     */
/**
    \brief      Get current voltage info use in function Pwrmgr_Manager_Handler_For_Rapid_Voltage_Changes

    \brief      Description:
                function deliver the current voltage info dectected use in function
                Pwrmgr_Manager_Handler_For_Rapid_Voltage_Changes to other modules

    \return     UinPWMGR_enVoltageStatest16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type Pwrmgr_GetRapidPwrmgrVoltage(void)
{
    return (pwrmgrVoltageForRapidVoltageChanges);
}
