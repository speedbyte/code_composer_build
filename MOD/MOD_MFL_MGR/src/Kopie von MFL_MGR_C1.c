/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	MFL_MGR_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
 * The reference files to implement this code is
 * FunktionenMFL_V1.9_081110.xls
 * Vergleich_VW_Skoda_MFL_u_3TM_Commands_V4.xls
 * Ergänzung der tech Spez MF Tasten SK46 Nr 10 - 080107.pdf
 * The files are kept in the doc folder.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-06-21 18:35:59 +0200 (Mo, 21 Jun 2010) $
* $Rev: 13648 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_MFL_MGR/trunk/src/MFL_MGR_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "MFL_MGR_CI.h"
#include "MFL_MGR.h"
#include "MFL_MGR_C1.id"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
static PHY_BUT_DRV_ST_MESSAGE PHY_BUT_DRV_stButtonState_Mfl;
static BooleanType _MFL_MGR_bPositiveFlankeSend = BT_FALSE;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
#define MFL_VARIANT_LOW         0x09
#define MFL_VARIANT_NORMAL      0x06
#define MFL_VARIANT_NO_MFL      0x00
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
/* Function    : MFL_vParameterAnalysis                                       */
/** 
    \brief      Function analysis the pushed MFL key and decided what 
                the key do in the application task. Under the significance 
                the mobile status - active call, idle, etc....
    
    \brief      MLT_SYMBOL: Length: 0..8 
                Code zugeordnet nach Symbolaufdruck Botschaftsverlaengerung nur bei Senden des KeyReleaseCode (0hex)
				BAF bedeutet: bei Betaetigung einer Taste oder der Walze.
				Hier werden ab PQ 35 auch die MFA-Tasten uebertragen
				
				MLT_TICKZAEHLER, Length: 16..4
				Tickzähler für Rändelräder, wird zusammen mit dem entsprechende Symbolcode ausgewertet. 
				Überlaufender Zähler, bei positiver Drehrichtung wird der Zähler erhöht, 
				bei negativer Drehrichtung verkleinert. Nach Reset/Power up/LIN Wakeup startet der 
				Zähler mit. 
                
                MLT_MARKE, Length: 24..4
				Fahrzeughersteller nach Kenntnisstand des Senders
				
				MLT_VARIANTE, Length: 28..4
				Variante für unterschiedliche Tasten / Tastenbelegungen verschiedener MFL

    \param      u8Type
                Uint8Type
                new or old MFL type
    \param      enSymbol
                MFL_ENUM_TASTENSYMBOL
                current pushed button
    \param      u8TickCounter
                Uint8Type
                how long hold the key
    \param      enMarke
                MFL_ENUM_MARKE
                in what for a car obstruct
    \param      u8Variante
                Uint8Type
                in what for a car obstruct

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void MFL_vParameterAnalysis (   Uint8Type u8Type, 
								MFL_ENUM_TASTENSYMBOL enSymbol,
								Uint8Type u8TickCounter,
								MFL_ENUM_MARKE enMarke, 
                                Uint8Type u8Variante
                                )
{
#if (SKODA_MFL_SUPPORT == 1)
    static Uint8Type _MFL_MGR_u8OldTickCounter = 0;
    Uint8Type PosTicks = 0;
    Uint8Type NegTicks = 0;
#endif    
	PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enDefaultState;
	PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManNoButton;
	PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
    if (TYPE_TASTEN == u8Type)
    {
        switch (enSymbol)
        {
            case enMfaUp:
            	PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
        	    if ( APP_CAN_u8GetActiveLSM1Device() == enAppCanMflAvailable) 
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflUp;
            	}
            	else if ((APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAvailable) ||  
            	         (APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAndMflAvailable))
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManLssUp;	
            	}
            	PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
        		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
            	(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
                
            case enMfaDown:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
            	if ( APP_CAN_u8GetActiveLSM1Device() == enAppCanMflAvailable )
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflDown;
            	}
            	else if ((APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAvailable) || 
            	         (APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAndMflAvailable))
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManLssDown;	
            	}
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
                
            case enMfaOkReset:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
            	if ( APP_CAN_u8GetActiveLSM1Device() == enAppCanMflAvailable )
            	{
    	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflOk;
            	}
                	else if ((APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAvailable) ||  
                	         (APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAndMflAvailable))
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManLssOk;	
            	}
            	DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
            	PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
                
            case enMfaEscapeBack:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflBack;
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;

            case enMfaHook:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflHook;
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
                
            case enMfaPTTPushToTalk:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflPTT;
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;

            case enMfaRollRight:
            {
                /*
                      Spec: MF-Tasten SK46
                      Differene of Ticks is max +7 / -7 per message
                      Roll Left and Roll Right uses the same Byte ;-)
                      5c1 ID=14 0 Ticks=e 63 1f b9 0 b0
                      0x07
                      0x06
                      0x05
                      0x04
                      0x03
                      0x02
                      0x01
                      ----
                      0x0F
                      0x0E
                      0x0D
                      0x0C
                      0x0B
                      0x0A
                      0x09
                      0x08
                */
                if ( (u8TickCounter > 0x00) && (u8TickCounter <= 0x0F) )
                {
                    PosTicks = 0;
                    NegTicks = 0;
                    DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Last Saved Ticks %d ", _MFL_MGR_u8OldTickCounter);
                    if ( (u8TickCounter < _MFL_MGR_u8OldTickCounter) 
                         && ( (_MFL_MGR_u8OldTickCounter - u8TickCounter) > 7) 
                           && (_MFL_MGR_u8OldTickCounter > 8) )
                    {
                         // positiver Ueberlauf
                         // posTicks = new + (15 -  old) 
                        PosTicks =  u8TickCounter + (15 - _MFL_MGR_u8OldTickCounter) ; 
                        DEBUG_VALUE2(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Received Tick Counter, RollUp with Overflow wrap around %d %d", u8TickCounter, PosTicks);
                    }
                    else if (      (u8TickCounter > _MFL_MGR_u8OldTickCounter) 
                         && ( (u8TickCounter - _MFL_MGR_u8OldTickCounter) > 7) 
                         && (_MFL_MGR_u8OldTickCounter < 7)
                        )
                    {
                         // negativer Ueberlauf
                         // neg Ticks = (15 - new) + old
                        NegTicks = (15 - u8TickCounter) + _MFL_MGR_u8OldTickCounter; 
                        if ( _MFL_MGR_u8OldTickCounter == 0x00 ) 
                        {
                        	NegTicks = NegTicks + 1;
                        }
                        DEBUG_VALUE2(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Received Tick Counter, RollDn with overflow wrap around %d %d ", u8TickCounter, NegTicks);
                    }
                    else if (u8TickCounter > _MFL_MGR_u8OldTickCounter)
                    {
                        // positiv kein Ueberlauf
                        PosTicks = u8TickCounter - _MFL_MGR_u8OldTickCounter; 
                        DEBUG_VALUE2(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Received Tick Counter, RollUp %d %d", u8TickCounter, PosTicks);
                    }
                    else if (u8TickCounter < _MFL_MGR_u8OldTickCounter) 
                    {
                        // negativ kein Ueberlauf
                        NegTicks = _MFL_MGR_u8OldTickCounter - u8TickCounter; 
                        DEBUG_VALUE2(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Received Tick Counter, RollDown %d %d", u8TickCounter, NegTicks);
                    }
                    _MFL_MGR_u8OldTickCounter = u8TickCounter;  
                    
                    // map Skoda Roll to VW Key 
	                if ( PosTicks > 0)
                    {
						PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
						PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflRollUp;
						PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMflSkodaRad;
						PHY_BUT_DRV_stButtonState_Mfl.enTicksReceived = PosTicks;
						DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
						(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
						_MFL_MGR_bPositiveFlankeSend = BT_FALSE;
                    }
                    else if (NegTicks > 0)
                    {
						PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
						PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflRollDown;
						PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMflSkodaRad;
						PHY_BUT_DRV_stButtonState_Mfl.enTicksReceived = NegTicks;
						DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
						(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
						_MFL_MGR_bPositiveFlankeSend = BT_FALSE;
                    }
                }
                break;
            }

            case enMfaNoKey:
            {
            	DEBUG_TEXT(MFL_MGR_SWI_DBG, MOD_MFL_MGR, "MFL Tasten: Cyclic No button press CAN msg");
    			PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonReleased_NegFlanke;
    			PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManNoButton;
    			PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
    			/* This is to avoid sending events to app for fake release buttons */
    			if ( _MFL_MGR_bPositiveFlankeSend == BT_TRUE )
    			{
    				// Spec: Init after Reset / Wakeup
    				_MFL_MGR_u8OldTickCounter = u8TickCounter;
    				DEBUG_TEXT(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten: MFL Event Release");
    				(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
    			}
    			_MFL_MGR_bPositiveFlankeSend = BT_FALSE;
                if ( u8TickCounter == 0x0)
                {
                	_MFL_MGR_u8OldTickCounter = u8TickCounter;
                }
    			break;
            }
            default:
            {
                /* do nothing */
                break;
            }
         }
    }
    else /* if (MFL_TASTEN_KON == u8Type) */
    {       
    	switch (enSymbol)
        {
            case enMflUp:
            case enMflArrowAUpRight:
            case enMflArrowBUpRight:
            	PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
        	    if ( APP_CAN_u8GetActiveLSM1Device() == enAppCanMflAvailable)
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflUp;
            	}
            	else if ((APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAvailable) ||
            	         (APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAndMflAvailable))
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManLssUp;
            	}
            	PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);            	(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
                break;
            case enMflDown:
            case enMflArrowADownLeft:
            case enMflArrowBDownLeft:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
            	if ( APP_CAN_u8GetActiveLSM1Device() == enAppCanMflAvailable )
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflDown;
            	}
            	else if ((APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAvailable) ||
            	         (APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAndMflAvailable))
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManLssDown;
            	}
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
                break;
            case enMflPTTPushToTalk:
            case enMflPPTCancel:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflPTT;
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
                break;
            case enMflHook:
            case enMflHangup:
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflHook;
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
                break;
            case enMfaNoKey:
            {
            	DEBUG_TEXT(MFL_MGR_SWI_DBG, MOD_MFL_MGR, "MFL Tasten: Cyclic No button press CAN msg");
    			PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonReleased_NegFlanke;
    			PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManNoButton;
    			PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
    			/* This is to avoid sending events to app for fake release buttons */
    			if ( _MFL_MGR_bPositiveFlankeSend == BT_TRUE )
    			{
    				// Spec: Init after Reset / Wakeup
    				DEBUG_TEXT(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: MFL Event Release");
    				(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
    			}
    			_MFL_MGR_bPositiveFlankeSend = BT_FALSE;
    			break;
            }
            case enMflOKThumbWheelButton:
            {
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
            	if ( APP_CAN_u8GetActiveLSM1Device() == enAppCanMflAvailable )
            	{
    	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflOk;
            	}
                	else if ((APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAvailable) ||
                	         (APP_CAN_u8GetActiveLSM1Device() == enAppCanLssAndMflAvailable))
            	{
            		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManLssOk;
            	}
            	DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
            	PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
            }
            case enMflMenu:
            {
				PHY_BUT_DRV_stButtonState_Mfl.enButtonState= enButtonPressed_PosFlanke;
	    		PHY_BUT_DRV_stButtonState_Mfl.enButtonName = enLogiButManMflBack;
	    		PHY_BUT_DRV_stButtonState_Mfl.enDeviceName = enLogiButManDeviceMfl;
	    		DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: Event to Application for %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
	    		(void)SendMessage(msgButtonStateInfo_Mfl, &PHY_BUT_DRV_stButtonState_Mfl);
	    		_MFL_MGR_bPositiveFlankeSend = BT_TRUE;
	    		break;
            }
            default:
            	DEBUG_VALUE1(MFL_MGR_SWI_TRC, MOD_MFL_MGR, "MFL Tasten Kon: In default case %d", PHY_BUT_DRV_stButtonState_Mfl.enButtonName);
                break;
        }
        
                 
    }
    return;
     
}



/* End Of File MFL_MGR_C1.c */


