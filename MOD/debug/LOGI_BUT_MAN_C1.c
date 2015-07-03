/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	LOGI_BUT_MAN_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-11-26 16:38:44 +0100 (Fr, 26 Nov 2010) $
* $Rev: 16561 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LOGI_BUT_MAN/trunk/src/LOGI_BUT_MAN_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h" // includes the 3bm enums
#include "ERM.h"
#include "CONTEXT_MAN.h"
#include "VCR.h"
#include "APP_SD.h"
#include "APP_PHONE.h"
#include "APP_PHONE_AT_PNCC.h"
#include "CSHDL.h"
#include "APP_DTC.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "LOGI_BUT_MAN.h"
#include "LOGI_BUT_MAN_CI.h"
#include "LOGI_BUT_MAN_C1.id"
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
static LOGI_BUT_MAN_ST_DEVICE LOGI_BUT_MAN_st3BMDevice;
static LOGI_BUT_MAN_ST_DEVICE LOGI_BUT_MAN_stCradleDevice;
static LOGI_BUT_MAN_ST_DEVICE LOGI_BUT_MAN_stMflDevice;
static PHY_BUT_DRV_ST_MESSAGE LOGI_BUT_MAN_stSendButtonData; 
static LOGI_BUT_MAN_ENUM_BUTTON_NAMES LOGI_BUT_MAN_enLastButtonPressed = enLogiButManNoButton;
static Uint8Type _LOGI_BUT_MAN_u8MflCountLongPress = 0;

static void _LOGI_BUT_MAN_vResetParameters (LOGI_BUT_MAN_PST_DEVICE  LOGI_BUT_MAN_pstDeviceSelection);
static void _LOGI_BUT_MAN_vDetectButtonPress(LOGI_BUT_MAN_PST_DEVICE  LOGI_BUT_MAN_pstDeviceSelection);
static SuccessFailType LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButton, LOGI_BUT_MAN_ENUM_PRESSTYPE enPressType);

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfInit                                                    */
/**
    \brief      initialise Mute manager to default

    \brief      this function initialise all Mute manager variables back to
                the specific default settings.

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType LOGI_BUT_MAN_sfInit(void)
{
    SuccessFailType sF              = SUCCESS;
    (void)LOGI_BUT_MAN_3BM_sfInit();
    (void)LOGI_BUT_MAN_CRADLE_sfInit();
    LOGI_BUT_MAN_enLastButtonPressed = enLogiButManNoButton;
    _LOGI_BUT_MAN_vResetParameters(&LOGI_BUT_MAN_st3BMDevice);
    _LOGI_BUT_MAN_vResetParameters(&LOGI_BUT_MAN_stCradleDevice);
    _LOGI_BUT_MAN_vResetParameters(&LOGI_BUT_MAN_stMflDevice);
    LOGI_BUT_MAN_stSendButtonData.enButtonName = enLogiButManNoButton;
    LOGI_BUT_MAN_stSendButtonData.enDeviceName = enLogiButManNoDevice;
    LOGI_BUT_MAN_stSendButtonData.enButtonState = enDefaultState;
    LOGI_BUT_MAN_stSendButtonData.enButtonPressType = enLogiButManNotPressed;
    LOGI_BUT_MAN_st3BMDevice.u8BorderLongPress = 40U;
    LOGI_BUT_MAN_stCradleDevice.u8BorderLongPress = 40U;
    LOGI_BUT_MAN_stMflDevice.u8BorderLongPress = 20U;
	return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    :   _APP3BM_vResetParameters                                     */
/**
    \brief      polls button states and sends button pressed events  

    \brief      Every time the host application calls this routine the 
                HW state of the 3 button module is checked. If a button 
		was pressed the fitting event message is sent to the 
		application task. 
		If the voltage fron the resistor network is not in the 
		allowed range the diagnostic informations are updated.

    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _LOGI_BUT_MAN_vResetParameters (LOGI_BUT_MAN_PST_DEVICE  LOGI_BUT_MAN_pstDeviceSelection)
{
	LOGI_BUT_MAN_pstDeviceSelection->enDeviceButtonProperty = enShortPressAndLongPress;
	LOGI_BUT_MAN_pstDeviceSelection->bLongPressMessageSend = BT_FALSE;
	LOGI_BUT_MAN_pstDeviceSelection->bShortPressMessageSend = BT_FALSE;
	LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging = 0U;
	LOGI_BUT_MAN_pstDeviceSelection->enDeviceState_pre = enNoTransition;
}


/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_sfhandle                                        */
/**
    \brief      handle a "button pressed" event

    \brief      Every time the event loop in the gets a fitting event 
                it calls this routine to process the event

    \param      enButton 
                LOGI_BUT_MAN_ENUM_BUTTON_NAMES 
                the button which was pressed

    \param      enPressType
                LOGI_BUT_MAN_ENUM_PRESSTYPE 
                what type is pressed

    \return     SuccessFailType sF 
                SUCCESS or FAIL
   */
/*-------------------------------------------------------------------*/
static SuccessFailType LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButton, LOGI_BUT_MAN_ENUM_PRESSTYPE enPressType)
{
	switch(enButton)
    {
	    /* ------------------- 3BM ------------------ */
		case enLogiButMan3bmButton1: /* MICMUTE_BUTTON on 3BM*/
	    case enLogiButMan3bmButton2: /* PTT_BUTTON on 3BM */
        case enLogiButMan3bmButton3: /* BTVISIBLE_BUTTON on 3BM*/
        	if ( BT_TRUE == LOGI_BUT_MAN_bGetRooflineModuleState() )
	    	{
                if ( enPressType == enLogiButManLongPressRepeat )
                {
                	_LOGI_BUT_MAN_u8MflCountLongPress++;
        		    DEBUG_VALUE2(LOGI_BUT_MAN_SWI_DBG, MOD_LOGI_BUT_MAN , "3BM Button pressType %d, longPressCnt = %d",enPressType, _LOGI_BUT_MAN_u8MflCountLongPress);
                	if ((LOGI_BUT_MAN_10_SEC_LONG_PRESS_3BM) == _LOGI_BUT_MAN_u8MflCountLongPress)
                	{
                		enPressType = enLogiButManLongPressTenSeconds;
                		DEBUG_VALUE2(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"%d->CON,%d",enButton, enPressType);
                		(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
                		_LOGI_BUT_MAN_u8MflCountLongPress = 0;
                	}
                }
                else
                {
                	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
                }
	    	}
			break;

	    /* ------------------- Cradle ------------------ */
	    case enLogiButManCradleButton1: /* INFO Button on Cradle */ 
        case enLogiButManCradleButton2: /* SOS Button on Cradle */
        case enLogiButManCradleButtonBoth:
        	if ((enPressType == enLogiButManLongPress) || 
                (enPressType == enLogiButManShortPress) || 
                (enPressType == enLogiButManShortPressAtSingleFunction))
            {
        	    if ( BT_TRUE == LOGI_BUT_MAN_bGetBasePlateConnectivity())
        	    {
        	    	DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Cradle Button pressed");
        	    	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
        	    }
            }
	        break;
	    /* ------------------- MFL ------------------ */
        case enLogiButManMflDown:
        case enLogiButManMflUp:
        	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
            break;
        case enLogiButManMflRollUp:
        case enLogiButManMflRollDown:
        {
        	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
        	break;
        }
        case enLogiButManMflOk:
        	if ( (enPressType == enLogiButManLongPress) || (enPressType == enLogiButManLongPressRepeat) )
        	{
            	DEBUG_VALUE2(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"%d->SD,%d",enButton, enPressType);
        		APP_SD_vOnMflButton(enButton, enLogiButManLongPress);
        	}
        	else
        	{
        		(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
        	}
            break;
        case enLogiButManMflBack:
        	if ( enPressType == enLogiButManShortPress )
           	{
            	DEBUG_VALUE2(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"%d->SD,%d",enButton, enPressType);
        		APP_SD_vOnMflButton(enButton, enPressType);
           	}
            else if ( enPressType == enLogiButManLongPress )
            {
            	/* Resolves SKODA Back lange Taste druck */
            	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
            }
            break;
        case enLogiButManMflHook:
        	if ( enPressType == enLogiButManShortPress  )
           	{
        		DEBUG_VALUE2(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"%d->CON,%d",enButton, enPressType);
	            (void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
           	}
        	else if ( enPressType == enLogiButManShortPressAtSingleFunction )
        	{
        		_LOGI_BUT_MAN_u8MflCountLongPress = 0;
        	}
            else if ( enPressType == enLogiButManLongPress )
            {
            	DEBUG_VALUE2(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"%d->CON,%d",enButton, enPressType);
            	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
            }
            else if ( enPressType == enLogiButManLongPressRepeat )
            {
            	_LOGI_BUT_MAN_u8MflCountLongPress++;
            	if (LOGI_BUT_MAN_10_SEC_LONG_PRESS == _LOGI_BUT_MAN_u8MflCountLongPress)
            	{
            		DEBUG_VALUE2(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"%d->CON,%d",enButton, enPressType);
            		enPressType = enLogiButManLongPressTenSeconds;
            		(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
            		_LOGI_BUT_MAN_u8MflCountLongPress = 0;
            	}
            }
            break;
        case enLogiButManMflPTT:
        	if ( enPressType == enLogiButManShortPress )
           	{
        		(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
           	}
        	else if ( enPressType == enLogiButManLongPress )
        	{
        		DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Do nothing");

        	}
            break;
        case enLogiButManLssUp:
        case enLogiButManLssDown:
        	// this is a mixture between doppel and wiederhol function. hence all button presses
        	// short, long, shortatpress and repeat will be forwarded.
        	(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
        	break;
        case enLogiButManLssOk:
        	if ( (enPressType == enLogiButManLongPress) || (enPressType == enLogiButManShortPress) || (enPressType == enLogiButManShortPressAtSingleFunction) )
        	{
        		(void)CONTEXT_MANAGER_sfResolveFunctionCall(enButton, enPressType);
        	}
        	break;
        	
        default:
        	DEBUG_TEXT(LOGI_BUT_MAN_SWI_WNG, MOD_LOGI_BUT_MAN , "Unkn But,Do nothing");
            break;
    }
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_vHandleEvents                                         */
/** 
    \brief      Task Function to handle events for Telephone Application Task
  
    \brief      Description:
                Waits for incoming events and if one arrives function 
                reads Messages received From BAP-Dispatcher and  
                AT_Cmd_Parser and handles them. 

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void LOGI_BUT_MAN_vHandleEvents(EventMaskType event)
{
    // wait for interesting events
    // ********************************
    // look for events from PHY_BUT_DRV
    // ********************************  
	LOGI_BUT_MAN_PST_DEVICE  LOGI_BUT_MAN_pstDeviceSelection = &LOGI_BUT_MAN_stMflDevice;
	if( event & evLogiButMan_ButtonInfo )
    {
	    // check for new response messages from PHY_BUT_DRV
		(void)ClearEvent(evLogiButMan_ButtonInfo);	
		(void)ReceiveMessage(msgButtonStateInfo ,&LOGI_BUT_MAN_stSendButtonData);
		if ( LOGI_BUT_MAN_stSendButtonData.enDeviceName == enLogiButManDevice3BM)
    	{
            LOGI_BUT_MAN_pstDeviceSelection = &LOGI_BUT_MAN_st3BMDevice;
            LOGI_BUT_MAN_st3BMDevice.enDiagnoseButton = LOGI_BUT_MAN_stSendButtonData.enButtonName;
            _LOGI_BUT_MAN_vDetectButtonPress(LOGI_BUT_MAN_pstDeviceSelection);
            DEBUG_TEXT(LOGI_BUT_MAN_SWI_DBG, MOD_LOGI_BUT_MAN , "Got 3BM Device");
    	}
    	else if ( LOGI_BUT_MAN_stSendButtonData.enDeviceName == enLogiButManDeviceCradle)
    	{
            LOGI_BUT_MAN_pstDeviceSelection = &LOGI_BUT_MAN_stCradleDevice;
            LOGI_BUT_MAN_stCradleDevice.enDiagnoseButton = LOGI_BUT_MAN_stSendButtonData.enButtonName;
            _LOGI_BUT_MAN_vDetectButtonPress(LOGI_BUT_MAN_pstDeviceSelection);
            DEBUG_TEXT(LOGI_BUT_MAN_SWI_DBG, MOD_LOGI_BUT_MAN , "Got Cradle Device");
    	}
        
    }
	/* TODO: It could be a problem here because of else condition. Check for stability.*/
	else if(event & evAppTaskMFLKeys )
	{         
		(void)ClearEvent(evAppTaskMFLKeys);
        (void)ReceiveMessage( msgButtonStateInfo_Mfl, &LOGI_BUT_MAN_stSendButtonData);
        // check queue Status
        if( E_COM_NOMSG != GetMessageStatus(msgButtonStateInfo_Mfl))
        {
            // there are still some messages
            
        	DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Msgs");
        	(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_LOGI_BUT_MAN_ROLL_BUTTON,
                                               MSEC(250),
                                               MSEC(0));
        }
        if ( CSHDL_bGetBuetoothUpdateActive() == BT_FALSE )
       	{
            LOGI_BUT_MAN_pstDeviceSelection = &LOGI_BUT_MAN_stMflDevice;
        
            if ( (LOGI_BUT_MAN_pstDeviceSelection->bLongPressMessageSend == BT_TRUE) && (LOGI_BUT_MAN_stSendButtonData.enButtonName  != enLogiButManNoButton))
            {
            	if ( LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging <= LOGI_BUT_MAN_BORDER_WIEDERHOL )
            	{
            		LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging++;
            	}
            	else 
            	{
            		DEBUG_VALUE1(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "wiederhol %d",LOGI_BUT_MAN_stSendButtonData.enButtonName);
    				LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging = 0U;
    				(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, enLogiButManLongPressRepeat);
    			}
            }
            else
            {
            	if ( LOGI_BUT_MAN_stSendButtonData.enDeviceName == enLogiButManDeviceMflSkodaRad)
            	{
            		if( BT_TRUE == APP_COMMON_UTILS_bIsAlarmActive(CTA_MFL_LONGPRESS_TIMER) )
            		{
            			DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Timer is still running");
            		}
            		else
            		{

    					/*if ( LOGI_BUT_MAN_stSendButtonData.enTicksReceived >= 2 )
    					{
    						DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Ticks more or equal to 2");
    						(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, enLogiButManLongPress );
    					}
    					else if ( LOGI_BUT_MAN_stSendButtonData.enTicksReceived >= 2 )
    					{
    						Uint8Type u8TempTicksReceived = LOGI_BUT_MAN_stSendButtonData.enTicksReceived;
    						while ( u8TempTicksReceived > 0 )
    						{
    							u8TempTicksReceived--;
    							LOGI_BUT_MAN_stSendButtonData.enTicksReceived = 1;
    							DEBUG_VALUE1(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Ticks remaining %d", u8TempTicksReceived);
    							(void)SendMessage(msgButtonStateInfo_Mfl, &LOGI_BUT_MAN_stSendButtonData);
    						}
    					}*/
    					if ( LOGI_BUT_MAN_stSendButtonData.enTicksReceived == 1 )
    					{
    						DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Timer not running, got 1 tick");
    						(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, enLogiButManShortPress );
    					}
    					else
    					{
    						DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Got more than one tick and starting the 200ms timer");
    						(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_MFL_LONGPRESS_TIMER, MSEC(200), MSEC(0));
    					}
            		}
            	}
            	else
            	{
            		_LOGI_BUT_MAN_vDetectButtonPress(LOGI_BUT_MAN_pstDeviceSelection);
            	}
            }
            DEBUG_TEXT(LOGI_BUT_MAN_SWI_DBG, MOD_LOGI_BUT_MAN , "Got Mfl Device");
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _LOGI_BUT_MAN_vDetectButtonPress                                         */
/** 
    \brief      Common Fn to detect button press for both Cradle and 3BM
  
    \brief      Description:
                This function will be called depending on which device is presssed
                and calls the Handle function with the button name.

    \param      LOGI_BUT_MAN_pstDeviceSelection
                LOGI_BUT_MAN_PST_DEVICE

    \return     void 
*/
/*----------------------------------------------------------------------------*/
static void _LOGI_BUT_MAN_vDetectButtonPress(LOGI_BUT_MAN_PST_DEVICE  LOGI_BUT_MAN_pstDeviceSelection)
{

	/* Conditons to implement the Short press part of Double function buttons */
	if ((LOGI_BUT_MAN_stSendButtonData.enButtonState == enButtonReleased_NegFlanke) && (LOGI_BUT_MAN_pstDeviceSelection->enDeviceState_pre == enButtonPressed_PosFlanke))
    {
    	if ( (LOGI_BUT_MAN_pstDeviceSelection->enDeviceButtonProperty == enShortPressAndLongPress) )
    	{
    		DEBUG_VALUE1(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "press released %d",LOGI_BUT_MAN_enLastButtonPressed );
    		_LOGI_BUT_MAN_u8MflCountLongPress = 0;
        	if ( (LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging < LOGI_BUT_MAN_pstDeviceSelection->u8BorderLongPress) && (LOGI_BUT_MAN_pstDeviceSelection->bLongPressMessageSend == BT_FALSE))
    		{
        		LOGI_BUT_MAN_stSendButtonData.enButtonName = LOGI_BUT_MAN_enLastButtonPressed;
        		LOGI_BUT_MAN_stSendButtonData.enButtonPressType = enLogiButManShortPress;
        		DEBUG_VALUE1(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN,"Short pressed %d",LOGI_BUT_MAN_enLastButtonPressed );
        		(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, LOGI_BUT_MAN_stSendButtonData.enButtonPressType);
    		}
        	LOGI_BUT_MAN_stSendButtonData.enButtonState = enNoTransition;
    	}
    	LOGI_BUT_MAN_pstDeviceSelection->bLongPressMessageSend = BT_FALSE;
    	LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging = 0U;
    }
    /* Conditons to implement the Long press part of Double function buttons */
    else if ( (LOGI_BUT_MAN_pstDeviceSelection->enDeviceState_pre == enButtonPressed_PosFlanke)&& (LOGI_BUT_MAN_stSendButtonData.enButtonState == enButtonPressed_PosFlanke) )
    {
		if ( LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging <= LOGI_BUT_MAN_MIN_CNT_BUTTON_HANGING )
		{
			LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging++;
		}
		if ( LOGI_BUT_MAN_pstDeviceSelection->enDeviceButtonProperty == enShortPressAndLongPress)
		{
    		if ((LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging > LOGI_BUT_MAN_pstDeviceSelection->u8BorderLongPress))
            {
                if (LOGI_BUT_MAN_pstDeviceSelection->bLongPressMessageSend == BT_FALSE)
    		    {
    		    	DEBUG_VALUE1(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Long pressed %d",LOGI_BUT_MAN_stSendButtonData.enButtonName);
    		    	LOGI_BUT_MAN_pstDeviceSelection->bLongPressMessageSend = BT_TRUE;
    		    	LOGI_BUT_MAN_stSendButtonData.enButtonPressType = enLogiButManLongPress;
    		    	(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, LOGI_BUT_MAN_stSendButtonData.enButtonPressType);
    		    	if ( LOGI_BUT_MAN_stSendButtonData.enDeviceName == enLogiButManDeviceMfl)
    		    	{
    		    		LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging = 0U;
    		    	}
    		    }
                else
                {
        			(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, enLogiButManLongPressRepeat);
                }
            }
    	}
    }
    else if ( (LOGI_BUT_MAN_pstDeviceSelection->enDeviceState_pre == enNoTransition)&& (LOGI_BUT_MAN_stSendButtonData.enButtonState == enButtonPressed_PosFlanke))
    {
		DEBUG_VALUE1(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Got Positive Flanke %d",LOGI_BUT_MAN_stSendButtonData.enButtonName);
		LOGI_BUT_MAN_stSendButtonData.enButtonPressType = enLogiButManShortPressAtSingleFunction;
		(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, LOGI_BUT_MAN_stSendButtonData.enButtonPressType);
		LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging++;
    	LOGI_BUT_MAN_enLastButtonPressed = LOGI_BUT_MAN_stSendButtonData.enButtonName;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    	(void)DEBUG_Printf(UHV_DEBUG_2, UHV_LOGI_BUT_MAN , "Last state = %d", LOGI_BUT_MAN_pstDeviceSelection->enDeviceState_pre);
#endif

    }
    LOGI_BUT_MAN_pstDeviceSelection->enDeviceState_pre = LOGI_BUT_MAN_stSendButtonData.enButtonState;
}

/*----------------------------------------------------------------------------*/
/* Function    :   LOGI_BUT_MAN_sfGetFailureValue                                     */
/**
    \brief      gets the failure value if any button has been hanged. 

    \brief      This function will be called cyclic from the monitor task 
                and checked if the value returned is 
                SUCCESS - no failure
                FAIL - when any one of the button has hung

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType LOGI_BUT_MAN_sfGetFailureValue(LOGI_BUT_MAN_ENUM_DEVICES enDevice)
{
	LOGI_BUT_MAN_PST_DEVICE  LOGI_BUT_MAN_pstDeviceSelection;
	if ( enDevice == enLogiButManDevice3BM)
	{
		LOGI_BUT_MAN_pstDeviceSelection = &LOGI_BUT_MAN_st3BMDevice;
	}
	else if ( enDevice == enLogiButManDeviceCradle)
	{
		LOGI_BUT_MAN_pstDeviceSelection = &LOGI_BUT_MAN_stCradleDevice;
	}
	if ( LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging > LOGI_BUT_MAN_MIN_CNT_BUTTON_HANGING )
	{
			DEBUG_VALUE1(LOGI_BUT_MAN_SWI_ERR, MOD_LOGI_BUT_MAN , "Button blocked %d", LOGI_BUT_MAN_pstDeviceSelection->u16ButtonCountHanging);
			return FAIL;
	}
	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_u8GetButtonStatus                                 */
/**
    \brief      to get the status of buttons in 
    			1. 3 button module.
    			2. Cradle

    \brief      
				Status Value should be :
				$00: Pressed
				$01: Released
				
    \param      enButtonName
				LOGI_BUT_MAN_ENUM_BUTTON_NAMES
				
    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButtonName) 
{
	Uint8Type retVal = 0x10U;
	if 	( ( enButtonName > enLogiButManCradleCountBegin ) &&
    	  ( enButtonName < enLogiButManCradleCountEnd )
		)
	{
		if ( APP_DTC_u8GetStatusBaseplate() == APP_DIAG_DTC_OK )
		{
			if (LOGI_BUT_MAN_stCradleDevice.enDiagnoseButton == enButtonName)
		  	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
				(void)DEBUG_Printf(UHV_DEBUG_1, UHV_LOGI_BUT_MAN, "Cradle Button %d is pressed",enButtonName);
#endif

				retVal = 0x00U;
		  	}
		    else // Button is not pressed.
		    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		    	(void)DEBUG_Printf(UHV_DEBUG_1, UHV_LOGI_BUT_MAN, "Cradle Button %d is not pressed",enButtonName);
#endif

		    	retVal = 0x01U;
		    }
		}
	    else
	    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	    	(void)DEBUG_Printf(UHV_DEBUG_2, UHV_LOGI_BUT_MAN, "Cradle Button %d is not avlble",enButtonName);
#endif

	    	retVal = 0x10U;	
	    }
	}
	else if ( ( enButtonName > enLogiButMan3bmCountBegin ) &&
    	  ( enButtonName < enLogiButMan3bmCountEnd )
		)
	{
		if ( APP_DTC_u8GetStatus3ButtonModule() == APP_DIAG_DTC_OK )
		{
			if (LOGI_BUT_MAN_st3BMDevice.enDiagnoseButton == enButtonName)
		  	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
				(void)DEBUG_Printf(UHV_DEBUG_1, UHV_LOGI_BUT_MAN, "3BM Button %d is pressed",enButtonName);
#endif

				retVal = 0x00U;
		  	}
		    else // Button is not pressed.
		    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		    	(void)DEBUG_Printf(UHV_DEBUG_1, UHV_LOGI_BUT_MAN, "3BM Button %d is not pressed",enButtonName);
#endif

		    	retVal = 0x01U;
		    }
		}
	    else
	    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	    	(void)DEBUG_Printf(UHV_DEBUG_2, UHV_LOGI_BUT_MAN, "3BM Button %d is not avlble",enButtonName);
#endif

	    	retVal = 0x10U;	
	    }
	}
	else
	{
	}
	return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : LOGI_BUT_MAN_vRollRadDebouncingFunction                                 */
/**
    \brief      The gateway splits the roll rad ticks into
				various ticks in various messages.
				Hence wait for 100ms till all the messages have arrived
				and then decide if the tick received are more than 2.
				If yes, then its a long press.

    \param      event
				EventMaskType

    \return     void
   */
/*----------------------------------------------------------------------------*/
void LOGI_BUT_MAN_vRollRadDebouncingFunction(EventMaskType event)
{
	if( event & evAppTaskRollRadLongPressTimer)
    {
	    // check for new response messages from PHY_BUT_DRV
		(void)ClearEvent(evAppTaskRollRadLongPressTimer);
		DEBUG_TEXT(LOGI_BUT_MAN_SWI_TRC, MOD_LOGI_BUT_MAN , "Ticks more or equal to 2");
		(void)LOGI_BUT_MAN_sfhandle(LOGI_BUT_MAN_stSendButtonData.enButtonName, enLogiButManLongPress);
    }
}
/* End Of File LOGI_BUT_MAN_C1.c */
