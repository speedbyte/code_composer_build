/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_CAN_Speed_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_APP_CAN
*-----------------------------------------------------------------------------
* Description:    
*-----------------------------------------------------------------------------
* $Date: 2010-03-02 15:00:06 +0100 (Di, 02 Mrz 2010) $
* $Rev: 11302 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SPEED_MGR/trunk/src/SpeedMgr_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "APP_CAN_Speed.h"
#include "APP_CAN.h"

#include "APP_CAN_Speed_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern APP_CAN_MsgStates canMsgStates;
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
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
Uint8Type         u8_HighSpeedBarringOn = FALSE;
Uint16Type        vehicleSpeed = 0U;
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
/* Function    : APP_CAN_Speed_vSpeedHandler                                  */
/** 
    \brief      Analysis the current vehicle speed for BT \n
                visibility.
  
    \param      speed
                Uint16Type
                no value limits
                
    \return     void
                value limits
   */
/*----------------------------------------------------------------------------*/
void APP_CAN_Speed_vSpeedHandler(Uint16Type speed)
{
    vehicleSpeed = speed;
    /* check speed limit for Bluetooth visability */
    if (U16_SPEED_LIMIT_HIGH <= speed)
    {
        if (u8_HighSpeedBarringOn == FALSE)
        {
            canMsgStates.common = SPEED;
            canMsgStates.speed.speed = overSpeed;
            SendMessage(msgAppCanStates, &canMsgStates);
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_TRACE, UHV_APP_CAN, "High speed %d > %d",
                                speed ,U16_SPEED_LIMIT_HIGH);
#endif

            DEBUG_VALUE2(SPEED_MGR_SWI_TRC, MOD_APP_CAN, "High speed %d > %d", speed ,U16_SPEED_LIMIT_HIGH);
            u8_HighSpeedBarringOn = TRUE;
        }
    }
    else if (U16_SPEED_LIMIT_LOW >= speed)
    {
        if (u8_HighSpeedBarringOn == TRUE)
        {
            canMsgStates.common = SPEED;
            canMsgStates.speed.speed = underSpeed;
            SendMessage(msgAppCanStates, &canMsgStates);
            u8_HighSpeedBarringOn = FALSE;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_TRACE, UHV_APP_CAN, "Low speed %d < %d",
                            vehicleSpeed ,U16_SPEED_LIMIT_LOW);
#endif

            DEBUG_VALUE2(SPEED_MGR_SWI_TRC, MOD_APP_CAN, "Low speed %d < %d", vehicleSpeed ,U16_SPEED_LIMIT_LOW);
         }
    }
    else
    {
        /* speed is in hysterese limit for BT visibility */
    }
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_Speed_un16CurrentVehicleSpeed                        */
/** 
    \brief      Give the current vehicle speed for BT \n
                visibility.

    \return     Uint16Type
                no value limits
                
   */
/*----------------------------------------------------------------------------*/
Uint16Type APP_CAN_Speed_un16CurrentVehicleSpeed(void)
{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_TRACE, UHV_APP_CAN, "Speed = %d", vehicleSpeed);
#endif

	DEBUG_VALUE1(SPEED_MGR_SWI_TRC, MOD_APP_CAN, "Speed = %d", vehicleSpeed);
	return (vehicleSpeed);
}


/* End Of File SpeedMgr.C1.c */
