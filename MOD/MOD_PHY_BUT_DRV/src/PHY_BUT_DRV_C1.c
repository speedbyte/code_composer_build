/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	PHY_BUT_DRV_C1.c
 * \brief	This file handles with detection of buttons.
 * \brief	This is the central file to call Button detection mechanism for 
 * 				 1. Cradle
 * 				 2. Drei Button module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-02-19 18:33:49 +0100 (Do, 19 Feb 2009) $
* $Rev: 4741 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_PHY_BUT_DRV/trunk/src/PHY_BUT_DRV_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

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
/* Function    : PHY_BUT_DRV_sfInit                                          */
/**
    \brief      Initialise the PHY_BUT_DRV module

    \brief      This function will be called by the SYS INIT during the Startup phase.
    
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType PHY_BUT_DRV_sfInit(void)
{
    SuccessFailType sF = SUCCESS;
    static Uint8Type init_done = 0;
    if ( init_done ) 
    {
    	return FAIL;
    }
    PHY_BUT_DRV_sfPL_Init();
    init_done = 1;
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    :   PHY_BUT_DRV_vhandle                                     */
/**
    \brief      Fn Call from OSEK. This is called every 10ms.

    \brief      This function is called from the OSEK every 10ms.
    			The Button detection mechanism starts at this point.

    \param      enDevice
                LOGI_BUT_MAN_ENUM_DEVICES
                Enum for the devices handled by this module.
                
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType PHY_BUT_DRV_vhandle (LOGI_BUT_MAN_ENUM_DEVICES enDevice)
{
	PHY_BUT_DRV_ST_MESSAGE stNewState;
	if ( enDevice == enLogiButManDevice3BM)
		{
		stNewState = _PHY_BUT_DRV_enGetState_3BM();
		}
	else if ( enDevice == enLogiButManDeviceCradle)
	{
	stNewState = _PHY_BUT_DRV_enGetState_Cradle();
	}
	else{
		return FAIL;
	}
 	PHY_BUT_DRV_vPL_handle (enDevice, stNewState);
	return SUCCESS;
}


/* End Of File PHY_BUT_DRV_C1.c */
