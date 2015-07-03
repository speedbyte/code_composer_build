/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler_MDI.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-07-05 10:59:25 +0200 (Di, 05 Jul 2011) $
* $Rev: 20623 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_StateHandler_MDI.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "common.h"
#include "ATCommonData.h"
#include "APP_PHONEBOOK_AT.h"
#include "APP_MDI.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "VCR_StateHandler.h"
#include "VCR_MessageHandler.h"
#include "VCR_Utils.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern VcrModGlobalDataType _VCR_gData;
/*----------------------------------------------------------------------------*/
/* Private Manifest  Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* File local  Constant Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private  Constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Methods                                                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleMusicEvent                                    */
/**
    \brief      event handler for event received in MUSIC state

    \brief

    \param      BooleanType *handled
                indicate to calling function if the event or command was
                handled within the function or need to be handed via generic
    \param      BooleanType is_event - indicator if either event param or cmd
                                       param shall be used for processing
    \param      VCRMenuEventEnum event - event number to be handled if
                                         is_event == BT_TRUE, otherwise ignore
    \param      ATSIVRCmdIdEnum cmd - cmd number to be handled if
                                         is_event == BT_FALSE, otherwise ignore

    \return     SuccessFailType SUCCESS if processing was successful
                                FAIL if an error occured
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfHandleMusicEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    *handled = BT_TRUE;
    if (is_event)
    {
        switch (event)
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */
            case VCR_MEVT_PROC_ERROR:
            {
                switch (_VCR_gData.u8ProcErr)
                {
                    case 1:
                    {
                        _VCR_vT1SorryRepeat();
                        break;
                    }
                    case 2:
                    {   /* check if only name or name and category available */
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_PRMT_T2_OPTIONS_MUSIC;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelStopFor3rdError();
                        break;
                    }
                }
                break;
            }
            default:
            {   /* used for all events that do not need to be handled */
                sF = FAIL;
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {   /* entry from Main menu */
            case AT_SIVR_CMD_MUSIC:
            {   /* check if player available */
                if (BT_TRUE == APP_MDI_bPlayerAvailable())
                {   /* check if remote control available */
                    if (BT_TRUE == APP_MDI_bRemoteControlAvailable())
                    {   /* start reco */
                        _VCR_vChangeState(AT_SIVR_MENU_MUSIC);
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU);
                    }
                    else
                    {
#ifdef UHV_ACTIVATE_FEATURE_VCR_SOP3_PROMPTS                      
                        /* prepare prompt output and send end prompt */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NO_REMOTE_CONTROL_POSSIBLE;
                        _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
                        _VCR_gData.u8NumPromptRequired = 2u;
                        _VCR_gData.u8NumPromptSend     = 0u;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
#else                        
                        /* prepare prompt output and go back to main */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NO_REMOTE_CONTROL_POSSIBLE;
                        _VCR_gData.u8NumPromptRequired = 1u;
                        _VCR_gData.u8NumPromptSend     = 0u;
                        _VCR_vChangeState(AT_SIVR_MENU_MAIN);
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT);
#endif
                    }
                }
                else
                {   /* prepare prompt output and send end prompt */
                    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NO_PLAYER_AVAILABLE;
                    _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
                    _VCR_gData.u8NumPromptRequired = 2u;
                    _VCR_gData.u8NumPromptSend     = 0u;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                }
                break;
            }
            case AT_SIVR_CMD_MUSIC_PLAY:
            {   /* trigger command to MDI and go back to idle via stop prompt */
                APP_MDI_sfVcrSetPlayMode(APP_MDI_RESUME);
                /* back to idle via stop prompt */
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                break;
            }
            case AT_SIVR_CMD_MUSIC_PAUSE: /* Fallthrough: RT1962 don't do twice a Pause*/
            {   /* trigger command to MDI and go back to idle via stop prompt */
                APP_MDI_sfVcrSetPlayMode(/*NULL,*/APP_MDI_PAUSE);
                /* back to idle via stop prompt */
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                break;
            }
            case AT_SIVR_CMD_MUSIC_STOP:
            {   /* trigger command to MDI and go back to idle via stop prompt */
                APP_MDI_sfVcrSetPlayMode(/*NULL,*/APP_MDI_STOP);
                /* back to idle via stop prompt */
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                break;
            }
            case AT_SIVR_CMD_MUSIC_NEXT_TRACK:
            {   /* trigger command to MDI and go back to idle via stop prompt */
                APP_MDI_sfVcrSetPlayMode(/*NULL,*/APP_MDI_NEXT_SONG);
                /* back to idle via stop prompt */
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                break;
            }
            case AT_SIVR_CMD_MUSIC_PREV_TRACK:
            {   /* trigger command to MDI and go back to idle via stop prompt */
                APP_MDI_sfVcrSetPlayMode(/*NULL,*/APP_MDI_PREVIOUS_SONG);   /* back to idle via stop prompt */
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* handle via generic handling */
                *handled = BT_FALSE;
                break;
            }
            default:
            {
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }

    return sF;
}

/* End Of File VCR_StateHandler_MDI.c */
