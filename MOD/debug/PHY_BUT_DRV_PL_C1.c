/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     PHY_BUT_DRV.c
*-----------------------------------------------------------------------------
* Module Name:  PHY_BUT_DRV
*-----------------------------------------------------------------------------
* Description: Driver part (host task) of 3 Button module handling
*-----------------------------------------------------------------------------
* $Date: 2009-03-03 19:11:14 +0100 (Di, 03 Mrz 2009) $
* $Rev: 4912 $
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
static void _PHY_BUT_DRV_vSendMessage(PHY_BUT_DRV_ST_MESSAGE stMessageToApp);
static PHY_BUT_DRV_PST_DEVICE PHY_BUT_DRV_stGetDeviceId(LOGI_BUT_MAN_ENUM_DEVICES enDevice);
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
\brief house keeping for the buttons on 3 Button Module.
*/
static PHY_BUT_DRV_ST_DEVICE PHY_BUT_DRV_stDevice_3ButtonModule;
/**
\brief house keeping for the buttons on Cradle.
*/
static PHY_BUT_DRV_ST_DEVICE PHY_BUT_DRV_stDevice_Cradle;
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
/* Function    : PHY_BUT_DRV_sfPL_Init                                      */
/**
    \brief      Initialise the Phyical layer for PHY_BUT_DRV module

    \brief      This function will be called by the SYS_INIT during the Startup Phase.
    			All static variables are initialised here.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType PHY_BUT_DRV_sfPL_Init(void)
{
    SuccessFailType sF = SUCCESS;
    PHY_BUT_DRV_stDevice_3ButtonModule.u8ValidityDownCount = 255;
    PHY_BUT_DRV_stDevice_3ButtonModule.enModuleInternalState = enPhyButDrvButtonReleased;
    PHY_BUT_DRV_stDevice_Cradle.u8ValidityDownCount = 255;
    PHY_BUT_DRV_stDevice_Cradle.enModuleInternalState = enPhyButDrvButtonReleased;
    PHY_BUT_DRV_sfPL_3BM_Init();
    PHY_BUT_DRV_sfPL_Cradle_Init();
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    :   PHY_BUT_DRV_stGetDeviceId                                     */
/**
    \brief      This fn is called to get the context in which the program is running.

    \brief      The context in which the program is running can be either Cradle or 3BM.
    			This function returns the current context.

    \param      enDevice
                LOGI_BUT_MAN_ENUM_DEVICES
                Enum for the devices handled by this module.

    \return     PHY_BUT_DRV_PST_DEVICE
    			Pointer to the Device structure.
   */
/*----------------------------------------------------------------------------*/
static PHY_BUT_DRV_PST_DEVICE PHY_BUT_DRV_stGetDeviceId(LOGI_BUT_MAN_ENUM_DEVICES enDevice)
{
	PHY_BUT_DRV_PST_DEVICE stDevice = &PHY_BUT_DRV_stDevice_3ButtonModule;
	if ( enDevice == enLogiButManDevice3BM)
	{
		stDevice = &PHY_BUT_DRV_stDevice_3ButtonModule;
	}	
	else if ( enDevice == enLogiButManDeviceCradle)
	{
		stDevice = &PHY_BUT_DRV_stDevice_Cradle;
	}	
	return(stDevice); 
}


/*----------------------------------------------------------------------------*/
/* Function    :   _PHY_BUT_DRV_vSendMessage                                        */
/**
    \brief      sends a message to the application part  

    \brief      The button enum value is sent to the application 
                via an OSEK message

    \param      stMessageToApp
                PHY_BUT_DRV_ST_MESSAGE
                
    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _PHY_BUT_DRV_vSendMessage(PHY_BUT_DRV_ST_MESSAGE stMessageToApp)
{
	SendMessage(msgButtonStateInfo, &stMessageToApp);
}


/*----------------------------------------------------------------------------*/
/* Function    :   PHY_BUT_DRV_vhandle                                     */
/**
    \brief      Handling of Button detection mechanism.

    \brief      This function handles the states of the Button Presses.
    			The state can be either noTransition, PositiveFlanke or 
    			NegativeFlanke.

    \param      enDevice
                LOGI_BUT_MAN_ENUM_DEVICES
                Enum for the devices handled by this module.

    \param      stNewState
                PHY_BUT_DRV_ST_MESSAGE
                Message Structure to be send to the other Tasks.
                                
    \return     none
   */
/*----------------------------------------------------------------------------*/
void PHY_BUT_DRV_vPL_handle (LOGI_BUT_MAN_ENUM_DEVICES enDevice, PHY_BUT_DRV_ST_MESSAGE stNewState)
{
	PHY_BUT_DRV_PST_DEVICE       stDevice;
	stDevice = PHY_BUT_DRV_stGetDeviceId(enDevice);
	
	switch (stNewState.enButtonState) 
    {
        case enNoTransition:
        	if ( stDevice->u8ValidityDownCount == 255)
        	{
        		break;
        	}
			stDevice->u8ValidityDownCount--;
	        if (stDevice->u8ValidityDownCount == 0)
    		{
	       		if (stDevice->enModuleInternalState == enPhyButDrvButtonPressed) 
	       		{
		        	stNewState.enButtonState= enButtonReleased_NegFlanke;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
					(void)DEBUG_Printf(UHV_TRACE, UHV_PHY_BUT_DRV , "Button released");
#endif

					stDevice->enModuleInternalState = enPhyButDrvButtonReleased;
					stDevice->u8ValidityDownCount = PHY_BUT_DRV_DOWN_COUNTER;
				}
	       		else
	       		{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        			(void)DEBUG_Printf(UHV_DEBUG_2, UHV_PHY_BUT_DRV , "Button Idle");
#endif

      			}
	       		_PHY_BUT_DRV_vSendMessage(stNewState);
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	       		(void)DEBUG_Printf(UHV_DEBUG_1, UHV_PHY_BUT_DRV , "Device %d",stNewState.enDeviceName);
#endif

    		}
        	break;
        case enButtonPressed_PosFlanke:
        	_PHY_BUT_DRV_vSendMessage(stNewState);
        	stDevice->enModuleInternalState = enPhyButDrvButtonPressed;
        	stDevice->u8ValidityDownCount = PHY_BUT_DRV_DOWN_COUNTER;
        default:
        	break;
    }
}

/* End Of File PHY_BUT_DRV_C1.c */
