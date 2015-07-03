/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler_SETTINGS.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-09-21 09:41:46 +0200 (Mi, 21 Sep 2011) $
* $Rev: 21861 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_StateHandler_SETTINGS.c $
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
#include "ERM.h"

#include <stdlib.h>         // atoi


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "VCR_CE.h"
#include "VCR_StateHandler.h"
#include "VCR_MessageHandler.h"
#include "VCR_Utils.h"

#include "VCR_StateHandler_SETTINGS.id"  /*RT-13225*/
#include "TRACE.h"          /*RT-13225*/
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
static void _VCR_vNotRecogCmdHandling(void); /*RT-13225 & RT-14043*/

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
/* Function    : _VCR_sfHandleMoreOptionsEvent                                */
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
SuccessFailType _VCR_sfHandleMoreOptionsEvent(
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
#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_SMS)
                        if (BT_TRUE == _VCR_bGetSmsMenuAvailable())
                        {
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_MORE_OPTIONS);
                        }
                        else
                        {
                            // mobile supports no SMS
                           _VCR_vSinglePrepare(AT_SIVR_PRMT_MORE_T2_OPTIONS_WITHOUT_READ_SMS);
                        }
#else
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_MORE_OPTIONS);
#endif
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
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_MORE_OPTIONS:
            {   /* entry trigger from main */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU);
                break;
            }
            case AT_SIVR_CMD_OPT_PLAY_ADDBOOK:
            {
                _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOK);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_PLAY_ADDBOOK );
                break;
            }
            case AT_SIVR_CMD_OPT_READ_SMS:
            {
#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_SMS)
                if (BT_TRUE == _VCR_bGetSmsMenuAvailable())
                {
                    _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_READ_SMS );
                }
                else
                {
                    // mobile supports no SMS
                    /* Featureverschiebung */
                    _VCR_vT1SorryRepeat();
                }
#else
                _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_READ_SMS );
#endif
                break;
            }
            case AT_SIVR_CMD_OPT_CALL_STACKS:
            {
                _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKS);
               _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_CALL_STACKS );
                break;
            }
            case AT_SIVR_CMD_OPT_SETTINGS:
            {
                _VCR_vChangeState(AT_SIVR_MENU_SETTINGS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_SETTINGS );
                break;
            }
            case AT_SIVR_CMD_OPT_VOICE_MEMOS:
            {
                _VCR_vChangeState(AT_SIVR_MENU_VOICEMEMOS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_VOICE_MEMOS );
                break;
            }
            case AT_SIVR_CMD_OPT_TRAINING:
            {
                /* Featureverschiebung */
                _VCR_vT1SorryRepeat();
                break;
            }

            case AT_SIVR_CMD_CANCEL:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }

            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_SMS)
                if (BT_TRUE == _VCR_bGetSmsMenuAvailable())
                {
                    *handled = BT_FALSE;
                }
                else
                {
                    _VCR_gData.stProcMenu.ePromptId = AT_SIVR_PRMT_MORE_OPTIONS_WITHOUT_READ_SMS;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
                    _VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
#else
                *handled = BT_FALSE;
#endif
                break;

            }
            default:
            {   /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleSettingsEvent                                   */
/**
    \brief      event handler for event received in SETTING state

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
SuccessFailType _VCR_sfHandleSettingsEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    *handled = BT_TRUE;

    if (is_event)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"RT-13225: Entry to event handling  Procerr %d  PrevProcerr %d is_event %d event %ds",_VCR_gData.u8ProcErr, _VCR_gData.u8PrevProcErr, is_event, event);


        switch (event)
        {   /* we wait for result from reco, in case of error we check error counter and
               trigger retry if necessary */
            case VCR_MEVT_PROC_ERROR:
            {

                if(_VCR_gData.u8NotRecogCmdProcErr  != 0)    /*RT-13225 & RT-14043*/
                {
                    _VCR_gData.u8ProcErr = _VCR_gData.u8ProcErr +_VCR_gData.u8NotRecogCmdProcErr ;
                    _VCR_gData.u8NotRecogCmdProcErr  = 0;
                }

                _VCR_gData.u8PrevProcErr = _VCR_gData.u8ProcErr;

                switch (_VCR_gData.u8ProcErr)
                {
                    case 1:
                    {
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: First event error  Procerr %d  PrevProcerr %d",_VCR_gData.u8ProcErr, _VCR_gData.u8PrevProcErr);

                        _VCR_vT1SorryRepeat();
                        break;
                    }
                    case 2:
                    {
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: Second event error  Procerr %d  PrevProcerr %d",_VCR_gData.u8ProcErr, _VCR_gData.u8PrevProcErr);

                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_SETTINGS);
                         break;
                    }
                    case 3:
                    default:
                    {
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: Third event error  Procerr %d  PrevProcerr %d",_VCR_gData.u8ProcErr, _VCR_gData.u8PrevProcErr);

//                        _VCR_vCancelStopFor3rdError();
                        // RT#5416 wrong jump after timeout in 'settings' menu
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_SETTINGS;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }

            default:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {
            /* entry command from main menu or more options  */
            case AT_SIVR_CMD_SETTINGS:
            case AT_SIVR_CMD_OPT_SETTINGS:
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                /* only trigger prep */
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_SETTINGS;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_SET_VF_OFF:  // Kurzdialog
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                /* change voice feedback flag and give out prompt - back to main */
                _VCR_gData.bVoiceFBOff = BT_TRUE;
                // RT#3407
                _VCR_sfSetVoiceFeedback(_VCR_gData.bVoiceFBOff);
                _VCR_gData.u8ProcErr = 0;
                _VCR_vSinglePrompt(AT_SIVR_PRMT_VOICE_FEEDBACK_OFF);
                /* goto main */
                _VCR_vChangeMenuToMain();
                break;
            }
            case AT_SIVR_CMD_SET_VF_ON:  // Langdialog
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                /* change voice feedback flag and give out prompt - back to main */
                _VCR_gData.bVoiceFBOff = BT_FALSE;
                _VCR_sfSetVoiceFeedback(_VCR_gData.bVoiceFBOff);
                _VCR_gData.u8ProcErr = 0;
                _VCR_vSinglePrompt(AT_SIVR_PRMT_VOICE_FEEDBACK_ON);
                /* goto main */
                _VCR_vChangeMenuToMain();
                break;
            }
            case AT_SIVR_CMD_SET_RESTORE_FACTORY:
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                _VCR_vChangeState(AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_SET_RESTORE_FACTORY );
                break;
            }
            case AT_SIVR_CMD_SET_BT_OPTIONS:
            {
#if (1 == VCR_SUPPORT_SETTING_MENU_WITHOUT_BLUETOOTH_OPTION)

                /*_VCR_vT1SorryRepeat();*/ /* RT-13225 & RT-14043*/
                _VCR_vNotRecogCmdHandling();

#else
                _VCR_vChangeState(AT_SIVR_MENU_BLUETOOTHOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_SET_BT_OPTIONS );
#endif
                break;
            }
            case AT_SIVR_CMD_SET_CHG_LANGUAGE:
            {
                /* Featureverschiebung*/
                /*_VCR_vT1SorryRepeat();*/ /* RT-13225 & RT-14043*/
                _VCR_vNotRecogCmdHandling();
                break;
            }
            case AT_SIVR_CMD_SET_DELETE_VTAGS:
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                _VCR_vChangeState(AT_SIVR_MENU_DELETEALLVOICETAGSCONFIRM);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_SET_DELETE_VTAGS );
                break;
            }
            case AT_SIVR_CMD_SET_RINGTONE:
            {
                /* Featureverschiebung*/
                /*_VCR_vT1SorryRepeat();*/ /* RT-13225 & RT-14043*/
                _VCR_vNotRecogCmdHandling();
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }

          /*  case AT_SIVR_CMD_SET_CHG_VM_NUMBER:*/
            default:
            {
                /* RT-13225 & RT-14043: Reset Not recognized command error flags */
                _VCR_vResetNotRecogCmdErrFlags();

                /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }

    }


    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleRestoreFactorySettingsEvent                     */
/**
    \brief      event handler for event received in RestoreFactorySettngs state

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
SuccessFailType _VCR_sfHandleRestoreFactorySettingsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
    *handled = BT_TRUE;


    if (is_event)
    {
        switch (event)
        {   /* we wait for result from reco, in case of error we check error counter and
               trigger retry if necessary */
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
                    {
                        _VCR_vSinglePrepare(
                                AT_SIVR_PRMT_T2_OPTIONS_RESTOREFACTORYDEFAULTSCONFIRM);
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
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* go to main menu */
                _VCR_vChangeMenuToMain();
                break;
            }

            default:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {
            /* entry command from main menu */
            case AT_SIVR_CMD_SET_RESTORE_FACTORY:
            {   /* only trigger prep */
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
             /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_RESTOREFACTORY_YES:
            {   /* NOTE: as the flasherase will result in CK5050 restart, we do the following:
                   we trigger the flasherase immediately and change the system state to flasherase
                   when due to flasherase startup happens, the prompt has to be given and system
                   shall immediately switch to Main */
                rsp = APP_DEVICE_rcResetToFactorySetting(VCR_vCBRestoreFactory);
                switch (rsp)
                {
                    case  APP_COMMON_RSP_OK:
                    {   /* command was triggered - wait for callback */
                        break;
                    }
                    case  APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry here */
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_RESTOREFACTORY_YES );
                        break;
                    }
                    case  APP_COMMON_RSP_ERROR:
                    case  APP_COMMON_RSP_ERROR_BAP_PARSE:
                    case  APP_COMMON_RSP_ERROR_UART_BLOCKED:
                    case  APP_COMMON_RSP_ERROR_AT_SEND:
                    case  APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT:
                    case  APP_COMMON_RSP_ERROR_NOT_INITIALIZED:
                    default:
                    {   /* an error occured, error output and stop system */
                        _VCR_PlayError(__LINE__);
                        break;
                    }
                }
                break;
            }
            case AT_SIVR_CMD_RESTOREFACTORY_NO:
            {   /* change back to settings */
                _VCR_vChangeState(AT_SIVR_MENU_SETTINGS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_SETTINGS );
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }
            default:
            {   /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }


    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleBluetoothOptionsEvent                           */
/**
    \brief      event handler for event received in Bluetooth Options state

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
SuccessFailType _VCR_sfHandleBluetoothOptionsEvent(
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
        {   /* we wait for result from reco, in case of error we check error counter and
               trigger retry if necessary */
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
                    {
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_BLUETOOTH_OPTIONS);
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
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* go to main menu */
                _VCR_vChangeMenuToMain();
                break;
            }

            default:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {
            /* entry command from settings menu */
            case AT_SIVR_CMD_SET_BT_OPTIONS:
            {   /* only trigger prep */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_BTOPT_BT_VISIBLE:
            {   /* switch BT visibility on */
                ERM_vCb3BM_BTVisibility();
                _VCR_gData.u8ProcErr = 0;
                _VCR_vSinglePrompt(AT_SIVR_PRMT_BLUETOOTH_VISIBLE);
                break;
            }
            case AT_SIVR_CMD_BTOPT_CHG_PIN:
            {
                _VCR_vChangeState(AT_SIVR_MENU_CHANGEBLUETOOTHPIN);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_BTOPT_CHG_PIN );
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }
            default:
            {   /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }


    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleBluetoothPinEvent                               */
/**
    \brief      event handler for event received in Bluetooth PIN state

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
SuccessFailType _VCR_sfHandleBluetoothPinEvent(
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
        {   /* we wait for result from reco, in case of error we check error counter and
               trigger retry if necessary */
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
                    {
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_CHANGE_BLUETOOTH_PIN);
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
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* restart this menu */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            default:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {
            /* entry command from settings menu */
            case AT_SIVR_CMD_BTOPT_CHG_PIN:
            {   /* only trigger prep */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_BTPIN_BLOCK:
            {   /* received pin digits - check length */
                if (BT_TRUE == _VCR_gData.stMsgRes.number_valid)
                {
                    _VCR_gData.u8ProcErr = 0;
                    /* clear resultInfo */
                    (void)memset(&(_VCR_gData.resultInfo.bTPin),
                            0x00,sizeof(VCRResultBTPinType));
                    _VCR_gData.resultInfo.bTPin.type = VCR_RESULT_TYPE_BT_PIN;
                    /* check received length */
                    if (0 < _VCR_gData.stMsgRes.number_len &&
                            AT_MAX_PIN_CODE_LEN >= _VCR_gData.stMsgRes.number_len)
                    {   /* number fit in internal array - copy */
                        _VCR_gData.resultInfo.bTPin.pin_len =
                            (Uint8Type)_VCR_gData.stMsgRes.number_len;
                       (void)memcpy(_VCR_gData.resultInfo.bTPin.pin,
                                _VCR_gData.stMsgRes.number,
                                _VCR_gData.resultInfo.bTPin.pin_len);
                        _VCR_gData.resultInfo.bTPin.pin[_VCR_gData.resultInfo.bTPin.pin_len] = '\0';
                        /* check the digit len */
                        if (AT_UHV_BT_PIN_CODE_LEN < _VCR_gData.resultInfo.bTPin.pin_len)
                        {   /* indicate pin too long and restart this menu */
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_PIN_TOO_LONG;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_PIN_DELETED;
                            _VCR_gData.u8NumPromptRequired = 2;
                            _VCR_gData.u8NumPromptSend     = 0;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                        }
                        else
                        {   /* change state to PIN confirm */
                            _VCR_vChangeState(AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_BTPIN_BLOCK );
                        }

                    }
                    else if (0 == _VCR_gData.stMsgRes.number_len)
                    {   /* somthing failed - stop system */
                        _VCR_PlayError(__LINE__ );
                    }
                    else
                    {   /* indicate pin too long and restart this menu */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_PIN_TOO_LONG;
                        _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_PIN_DELETED;
                        _VCR_gData.u8NumPromptRequired = 2;
                        _VCR_gData.u8NumPromptSend     = 0;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                    }

                }
                else
                {   /* somthing failed - stop system */
                    _VCR_PlayError(__LINE__ );
                }
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }
            /* possible result command codes from processing state machine */
            default:
            {   /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleBluetoothPinCorrectEvent                        */
/**
    \brief      event handler for event received in BT pin correct

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
SuccessFailType _VCR_sfHandleBluetoothPinCorrectEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;

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
                    {
                        _VCR_vSinglePrompt(AT_SIVR_PRMT_T2_OPTIONS_CHANGE_BLUETOOTH_PIN_CORRECT);
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
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* happens for some prompt outputs as also 2nd error - prepare again */
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
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
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_BTPIN_BLOCK:
            {   /* entry from ChangeBTPin */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_BTPIN_COR_SAVE:
            {   /* check the pin length */
                if (AT_UHV_BT_PIN_CODE_LEN == _VCR_gData.resultInfo.bTPin.pin_len)
                {   /* pin length correct - convert the pin string to number and trigger ERM */
                    _VCR_gData.resultInfo.bTPin.pin[AT_UHV_BT_PIN_CODE_LEN] = '\0';
                    _VCR_gData.resultInfo.bTPin.u16Pin = (Uint16Type)atoi((const char *)_VCR_gData.resultInfo.bTPin.pin);

                    rsp = ERM_sFSetBluetoothSetPINCode( VCR_vCBSetBTPin,
                            _VCR_gData.resultInfo.bTPin.u16Pin);
                    switch (rsp)
                    {
                        case APP_COMMON_RSP_OK:
                        {   /* do nothing but wait for callback */
                            break;
                        }
                        case APP_COMMON_RSP_ERROR_BUSY:
                        {   /* we need to retry - send specific event */
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_BTPIN_COR_SAVE);
                            break;
                        }
                        case APP_COMMON_RSP_ERROR:
                        case APP_COMMON_RSP_ERROR_BAP_PARSE:
                        case APP_COMMON_RSP_ERROR_UART_BLOCKED:
                        case APP_COMMON_RSP_ERROR_AT_SEND:
                        case APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT:
                        case APP_COMMON_RSP_ERROR_NOT_INITIALIZED:
                        default:
                        {   /* an error occured - stop machine */
                            _VCR_PlayError(__LINE__);
                            break;
                        }
                    }
                }
                else
                {   /* pin len inco */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_WRONG_PIN_LENGTH);
                }
                break;
            }
            case AT_SIVR_CMD_BTPIN_COR_BLOCK:
            {   /* add received block if length allows */
                if (BT_TRUE == _VCR_gData.stMsgRes.number_valid)
                {
                    _VCR_gData.u8ProcErr = 0;
                    /* clear resultInfo */
                    /* check received length */
                    if (0 < _VCR_gData.stMsgRes.number_len &&
                            AT_MAX_PIN_CODE_LEN >=
                            (_VCR_gData.stMsgRes.number_len +
                             _VCR_gData.resultInfo.bTPin.pin_len) )
                    {   /* number fit in internal array - copy */

                        (void)memcpy(&(_VCR_gData.resultInfo.bTPin.pin[
                                   _VCR_gData.resultInfo.bTPin.pin_len]),
                                _VCR_gData.stMsgRes.number,
                                _VCR_gData.stMsgRes.number_len);
                        _VCR_gData.resultInfo.bTPin.pin_len =
                            _VCR_gData.resultInfo.bTPin.pin_len +
                            (Uint8Type)_VCR_gData.stMsgRes.number_len;
                        _VCR_gData.resultInfo.bTPin.pin[_VCR_gData.resultInfo.bTPin.pin_len] = '\0';
                        /* calculate correct number of real digits without blanks */

                        /* now check the real digit len */
                        if (AT_UHV_BT_PIN_CODE_LEN < _VCR_gData.resultInfo.bTPin.pin_len)
                        {   /* indicate pin too long and restart this menu */
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_PIN_TOO_LONG;
                            _VCR_gData.u8NumPromptRequired = 1;
                            _VCR_gData.u8NumPromptSend     = 0;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                        }
                        else
                        {   /* change state to PIN confirm */

                            _VCR_vChangeState(AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_BTPIN_BLOCK );
                        }

                    }
                    else if (0 == _VCR_gData.stMsgRes.number_len)
                    {   /* somthing failed - stop system */
                        _VCR_PlayError(__LINE__ );
                    }
                    else
                    {   /* indicate pin too long and restart this menu */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_PIN_TOO_LONG;
                        _VCR_gData.u8NumPromptRequired = 1;
                        _VCR_gData.u8NumPromptSend     = 0;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                    }
                }
                else
                {   /* somthing failed - stop system */
                    _VCR_PlayError(__LINE__ );
                }
                break;
            }
            case AT_SIVR_CMD_BTPIN_COR_CORRECT:
            {
                if (1 == _VCR_gData.resultInfo.bTPin.pin_len)
                {   /* clear pin info */
                    (void)memset(&(_VCR_gData.resultInfo.bTPin),
                            0x00,sizeof(VCRResultBTPinType));
                    /* back to changebluetoothpin */
                    _VCR_vChangeState(AT_SIVR_MENU_CHANGEBLUETOOTHPIN);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_BTOPT_CHG_PIN );
                }
                else
                {   /* clear only last digit and retry */
                    _VCR_gData.resultInfo.bTPin.pin_len =
                        _VCR_gData.resultInfo.bTPin.pin_len - 1;
                    _VCR_gData.resultInfo.bTPin.pin[_VCR_gData.resultInfo.bTPin.pin_len] = '\0';
                    // SUSANNE 29.12.2009: #4297
                    // The pin was changed. But now we have to read it again
                    // currently only the help prompt is played. So do the same as for repeating the pin
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    // END: SUSANNE
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                break;
            }
            case AT_SIVR_CMD_BTPIN_COR_DELETE:
            {   /* just prompt pin deleted and back to main */
//                _VCR_vChangeState(AT_SIVR_MENU_MAIN);
                /* RT#2413 VCR: SDS Spec 4.5.6.3 wrong menu jump after bluetooth pin */
                _VCR_vSinglePrompt(AT_SIVR_PRMT_PIN_DELETED);

                /* back to changebluetoothpin */
                _VCR_vChangeState(AT_SIVR_MENU_CHANGEBLUETOOTHPIN);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_BTOPT_CHG_PIN );

                break;
            }
            case AT_SIVR_CMD_BTPIN_COR_REPEAT:
            {   /* just restart the menu here */
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }
            default:
            {   /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vNotRecogCmdHandling                        */
/**
    \brief      Function to handle the not recognized commands in the host

    \brief

    \param      None

    \return     None
   */
/*----------------------------------------------------------------------------*/
/*RT-13225 & RT-14043*/
static void _VCR_vNotRecogCmdHandling(void)
{
    if(_VCR_gData.u8PrevProcErr != 0)
    {
        _VCR_gData.u8NotRecogCmdProcErr = _VCR_gData.u8PrevProcErr;
        _VCR_gData.u8PrevProcErr = 0;
    }

    _VCR_gData.u8NotRecogCmdProcErr = _VCR_gData.u8NotRecogCmdProcErr + 1;

    switch (_VCR_gData.u8NotRecogCmdProcErr)
    {
        case 1:
        {
            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: First cmd  NRCmdProcerr %d  PrevProcerr %d",_VCR_gData.u8NotRecogCmdProcErr, _VCR_gData.u8PrevProcErr);

            _VCR_vT1SorryRepeat();
            break;
        }
        case 2:
        {
            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: Second cmd  NRCmdProcerr %d  PrevProcerr %d",_VCR_gData.u8NotRecogCmdProcErr, _VCR_gData.u8PrevProcErr);

            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_SETTINGS);
            break;
        }
        case 3:
        default:
        {
            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: Third cmd  NRCmdProcerr %d  PrevProcerr %d",_VCR_gData.u8NotRecogCmdProcErr, _VCR_gData.u8PrevProcErr);

            //_VCR_vCancelStopFor3rdError();
            // RT#5416 wrong jump after timeout in 'settings' menu
            _VCR_gData.u8NotRecogCmdProcErr = 0;
            _VCR_vCancelGotoMain();
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vResetNotRecogCmdErrFlags                        */
/**
    \brief      Function to reset not recognized command handling error flags

    \brief

    \param      None

    \return     None
   */
/*----------------------------------------------------------------------------*/
/*RT-13225 & RT-14043*/
void _VCR_vResetNotRecogCmdErrFlags(void)
{
     _VCR_gData.u8PrevProcErr = 0;
     _VCR_gData.u8NotRecogCmdProcErr = 0;
}

/* End Of File VCR_StateHandler_SETTINGS.c */
