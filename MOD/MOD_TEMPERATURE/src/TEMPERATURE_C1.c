/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	TEMPERATURE_C1.c
 * \brief	This file consists of functions to control the device temperature 
 * Detailed description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-05-21 17:33:48 +0200 (Fr, 21 Mai 2010) $
* $Rev: 13282 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_TEMPERATURE/trunk/src/TEMPERATURE_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
#include <osek.h>
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "MONITOR_ADC.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ADC.h"

#include "TEMPERATURE.h"
#include "TEMPERATURE_CI.h"

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
static Uint8Type _TEMPERATURE_u8ConvertTemperature(Uint16Type u16ADC_Value);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
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
static Uint8Type  _TEMPERATURE_u8Temperature;
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
/* Function    : TEMPERATURE_sfInit                                                    */
/**
    \brief      initialise Temperatur manager to default

    \brief      this function initialise all Temperature manager variables back to\n
                the specific default settings.

    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType TEMPERATURE_sfInit(void)
{
  SuccessFailType          sF      = SUCCESS;

  _TEMPERATURE_u8Temperature = 0;
	return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : TEMPERATURE_sfManager                                                    */
/**
    \brief      initialise Temperatur manager to default

    \brief      this function initialise all Temperature manager variables back to\n
                the specific default settings.

    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType TEMPERATURE_sfManager(void)
{
  SuccessFailType          sF      = SUCCESS;
  Uint16Type u16Temperature  = MONITOR_ADC_u16GetMovingAverageADCValueTemperature();
  _TEMPERATURE_u8Temperature = _TEMPERATURE_u8ConvertTemperature(u16Temperature);
  return sF;
}
/*----------------------------------------------------------------------------*/
/* Function    : _TEMPERATURE_u8ConvertTemperature                                   */
/**
    \brief      convert to ADC value to a temperature.

    \return     Uint8Type
	            value of converted temperature

   */
/*----------------------------------------------------------------------------*/
static Uint8Type _TEMPERATURE_u8ConvertTemperature(Uint16Type u16ADC_Value)
{
  Uint8Type u8Temperature = 0;
  int i;

  if (u16ADC_Value > _TEMPERATURE_TableValues[64].ADC)   // 20°C
  {
    i = 64;
    while (i < MAX_TEMPERATURE_VALUES)
    {
      if (u16ADC_Value < _TEMPERATURE_TableValues[i].ADC)
        break;
      i++;
    }
  }
  else
  {
    i = 63;
    while (i > 0)
    {
      if (u16ADC_Value > _TEMPERATURE_TableValues[i].ADC)
        break;
      i--;
    }
  }
  u8Temperature = _TEMPERATURE_TableValues[i].Temperature + 100;
  return (u8Temperature);
}


/*----------------------------------------------------------------------------*/
/* Function    : TEMPERATURE_u8GetTemperature                                   */
/**
    \brief      returns the current device temperature.

    \return     Uint8Type
	            value of temperature 
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type TEMPERATURE_u8GetTemperature(void)
{   
    return  (_TEMPERATURE_u8Temperature);
}

/* End Of File TEMPERATURE_C1.c */


