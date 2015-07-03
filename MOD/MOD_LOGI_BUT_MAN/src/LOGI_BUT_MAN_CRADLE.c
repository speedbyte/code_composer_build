/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	LOGI_BUT_MAN_CRADLE.c
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
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LOGI_BUT_MAN/trunk/src/LOGI_BUT_MAN_CRADLE.c $
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
#include "GIO.h"
#include "PORT.h"
#include "SPI.h"
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

/**
\brief flag to lock multiple initialisations
*/
static Uint8Type _LOGI_BUT_MAN_bCradleInitDone = (Uint8Type)BT_FALSE;


/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfInit                                          */
/**
    \brief      Initialise the LOGI_BUT_MAN module

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType LOGI_BUT_MAN_CRADLE_sfInit(void)
{
	if ( _LOGI_BUT_MAN_bCradleInitDone )
	{
		return FAIL;
    }
    _LOGI_BUT_MAN_bCradleInitDone = (Uint8Type)BT_TRUE;
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfGetBasePlateConnectivity                          */
/**
    \brief      Get configured value for Base Plate Diagnose.

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
BooleanType LOGI_BUT_MAN_bGetBasePlateConnectivity(void)
{
	BooleanType bValue;
	EEPROM_LAYOUT_vGetBaseplateDiag(&bValue);
	return (bValue);
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfGetCradleProtection                          */
/**
    \brief      Get configured value for Cradle protection

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
BooleanType LOGI_BUT_MAN_bGetCradleProtection(void)
{
	BooleanType bValue;
	EEPROM_LAYOUT_vGetCradleProtection(&bValue);
	return (bValue);
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfGetAntennaDiagnose                          */
/**
    \brief      Get configured value for Antenna Diagnose.

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
BooleanType LOGI_BUT_MAN_bGetAntennaDiagnose(void)
{
	BooleanType bValue;
	EEPROM_LAYOUT_vGetAntennaDiag(&bValue);
	return (bValue);
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_vInitBaseplateAndAntenna                        */
/**
    \brief      Get Coding Values for Baseplate and Antenna during Startup.

    \return     void

   */
/*----------------------------------------------------------------------------*/
void LOGI_BUT_MAN_vInitBaseplateAndAntenna(void)
{
	;
}


/* End Of File LOGI_BUT_MAN_CRADLE.c */
