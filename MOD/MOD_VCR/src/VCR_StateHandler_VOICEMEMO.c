/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler_VOICEMEMO.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2010-04-19 14:48:47 +0200 (Mo, 19 Apr 2010) $
* $Rev: 12327 $
* $Author: braun $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_StateHandler_VOICEMEMO.c $
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
/* Function    : _VCR_sfHandleDeleteAllVTConfirmEvent                         */
/**
    \brief      event handler for event received in delete all voice tags

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
SuccessFailType _VCR_sfHandleDeleteAllVTConfirmEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;

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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_DELETE_ALL_VOICETAGS_CONFIRM);
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
            {   /* back to main */
                if (0 !=  _VCR_gData.resultInfo.playPB.cur_entry)
                {
                    _VCR_vChangeMenuToMain();
                }
                else
                {
                    _VCR_gData.resultInfo.playPB.cur_entry = 1;
//                    (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_DELETE );
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_DELETE_ALL );
                }
                break;
            }
            case VCR_MEVT_VT_DELETE_ALL_DONE:
            {   /* check for additional entries */

/*
                if (_VCR_gData.resultInfo.playPB.cur_entry <
                    _VCR_gData.resultInfo.playPB.num_pb_entries)
                {
                    _VCR_gData.resultInfo.playPB.cur_entry =
                        _VCR_gData.resultInfo.playPB.cur_entry + 1;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_DELETE );
                }
                else
                {
*/
                    appRsp = ERM_rcResetNumVoiceTagsForActiveUser_InEeprom(
                            VCR_vCBResetStopVoiceTagForActiveUser,BT_FALSE);
                    switch (appRsp)
                    {
                        case APP_COMMON_RSP_OK:
                        {   /* do nothing but wait for callback */
                            break;
                        }
                        case APP_COMMON_RSP_ERROR_BUSY:
                        {   /* we need to retry - send trigger event in here */
                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_VT_DELETE_ALL_DONE);
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
//                    }
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
            case AT_SIVR_CMD_SET_DELETE_VTAGS:
            {   /* entry from settings */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_DELETEVT_CONF_YES:
            {   /* delete all voice tags for active user - we need
                   to know the number of entries in personal address
                   book and query one by one for possible voice tags and delete
                   them */
                _VCR_gData.resultInfo.playPB.type = VCR_RESULT_TYPE_PLAYPB;
                _VCR_gData.resultInfo.playPB.num_pb_entries =
                    APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
                _VCR_gData.resultInfo.playPB.cur_entry = 0;
                if (0 < _VCR_gData.resultInfo.playPB.num_pb_entries)
                {
                    _VCR_gData.resultInfo.playPB.cur_entry = 0;
                    /* trigger reset start */
                    appRsp = ERM_rcResetNumVoiceTagsForActiveUser_InEeprom(
                            VCR_vCBResetStartVoiceTagForActiveUser,BT_TRUE);
                    switch (appRsp)
                    {
                        case APP_COMMON_RSP_OK:
                        {   /* do nothing but wait for callback */
                            break;
                        }
                        case APP_COMMON_RSP_ERROR_BUSY:
                        {   /* we need to retry - send trigger event in here */
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DELETEVT_CONF_YES);
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
                {
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_ALL_AUTOMATIC_TRANSCRIPTIONS_RESTORED);
                }
                break;
            }
            case AT_SIVR_CMD_DELETEVT_CONF_NO:
            {   /* back to settings */
                _VCR_vChangeState(AT_SIVR_MENU_SETTINGS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_OPT_SETTINGS );
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
/* Function    : _VCR_sfHandleVoiceMemosEvent                                 */
/**
    \brief      event handler for event received in voice memos

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
SuccessFailType _VCR_sfHandleVoiceMemosEvent(
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
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }
            case AT_SIVR_CMD_OPT_VOICE_MEMOS:
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
/* Function    : _VCR_sfHandleVoiceMemosPlayEvent                             */
/**
    \brief      event handler for event received in voice memo play

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
SuccessFailType _VCR_sfHandleVoiceMemosPlayEvent(
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
/* Function    : _VCR_sfHandleVoiceMemosPlayOptionsEvent                      */
/**
    \brief      event handler for event received in voice memo play options

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
SuccessFailType _VCR_sfHandleVoiceMemosPlayOptionsEvent(
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
/* Function    : _VCR_sfHandleVoiceMemosConfDeleteEvent                       */
/**
    \brief      event handler for event received in voice memo confirm delete

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
SuccessFailType _VCR_sfHandleVoiceMemosConfDeleteEvent(
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
/* Function    : _VCR_sfHandleVoiceMemosConfDeleteAllEvent                    */
/**
    \brief      event handler for event received in voice memo confirm delete all

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
SuccessFailType _VCR_sfHandleVoiceMemosConfDeleteAllEvent(
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








/* End Of File VCR_StateHandler_VOICEMEMO.c */
