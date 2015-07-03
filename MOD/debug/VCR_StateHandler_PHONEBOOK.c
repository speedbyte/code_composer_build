/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler_PHONEBOOK.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-01-10 13:26:17 +0100 (Mo, 10 Jan 2011) $
* $Rev: 17209 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_StateHandler_PHONEBOOK.c $
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
#include "APP_PHONE.h"
#include "APP_SMS.h"

#include "VCR_StateHandler_PHONEBOOK.id"
#include "TRACE.h"

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
/* Function    : _VCR_sfHandlePlayAddressbookEvent                            */
/**
    \brief      event handler for event received in play addressbook

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
SuccessFailType _VCR_sfHandlePlayAddressbookEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;

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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {
                _VCR_vChangeState(_VCR_gData.eNextMenuState);
                switch (_VCR_gData.eNextMenuState)
                {
                    case AT_SIVR_MENU_MAIN:
                    {
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_MAIN );
                        break;
                    }
                    case AT_SIVR_MENU_STORENAME:
                    {
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_PAB_OPT_CHANGE_NAME );
                        break;
                    }
                    case AT_SIVR_MENU_PLAYADDRESSBOOKENTRY:
                    {
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_PAB_NAME_PLAY_ADDBOOK );
                        break;
                    }
                    case AT_SIVR_MENU_UNDEF:
                    default:
                    {   /* shall not occur - send error */
                        _VCR_PlayError(__LINE__);
                        break;
                    }
                }
                _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                break;
            }
            case VCR_MEVT_MANUAL_PB_SYNC:
            {   /* PB sync */
                res = APP_PHONEBOOK_rcRequestManualSync(VCR_vCBPBSyncOK);
                if (APP_COMMON_RSP_ERROR_BUSY == res)
                {   /* we need to retry */
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_MANUAL_PB_SYNC);
                }
                /* otherwise we wait for feedback */
                break;
            }
            case VCR_MEVT_MANUAL_PB_SYNC_OK:
            {
                _VCR_vSinglePrompt(AT_SIVR_PRMT_REFRESH_ADDRESSBOOK);
                _VCR_gData.eNextMenuState = AT_SIVR_MENU_MAIN;
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
            case AT_SIVR_CMD_PLAY_ADDRESSBOOK:
            case AT_SIVR_CMD_OPT_PLAY_ADDBOOK:
            {   /* entry command from main or more options */
                if (BT_TRUE == ERM_bPhoneAvailable())
                {
                    switch (_VCR_eUserPhonebookStatusGet())
                    {
                        case VCR_USER_PHONEBOOK_STATUS_AVAILABLE:
                        {
                            (void)memset(&(_VCR_gData.resultInfo),0x00,sizeof(VCRResultUnionType));
                            _VCR_gData.resultInfo.playPB.type = VCR_RESULT_TYPE_PLAYPB;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                            break;
                        }
                        case VCR_USER_PHONEBOOK_STATUS_EMPTY: /* address book empty */
                        {   /* prepare prompt output and send string prompt, back to main */
                            _VCR_gData.eNextMenuState      = AT_SIVR_MENU_MAIN;
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_ADDRESSBOOK_EMPTY);
                            break;
                        }
                        case VCR_USER_PHONEBOOK_STATUS_DOWNLOAD_IPR: /* address book empty */
                        {   /* prepare prompt output and send string prompt, back to main */
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_ADDRESSBOOK_EMPTY;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_ADDRESSBOOK_DOWNLOAD_IN_PROGRESS;
                            _VCR_gData.u8NumPromptRequired = 2;
                            _VCR_gData.u8NumPromptSend     = 0;
                            _VCR_gData.eNextMenuState      = AT_SIVR_MENU_MAIN;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                            break;
                        }
                        case VCR_USER_PHONEBOOK_STATUS_NOT_AVAILABLE: /* address book empty */
                        default: /* address book empty as default */
                        {   /* prepare prompt output and send string prompt, back to main */
                            _VCR_vFunctionNotAvailableMain();
                            break;
                        }
                    }
                }
                else
                {   /* prepare prompt output and send end prompt */
                    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_PHONE_NOT_AVAILABLE;
                    _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
                    _VCR_gData.u8NumPromptRequired = 2;
                    _VCR_gData.u8NumPromptSend     = 0;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                }
                break;
            }
            case AT_SIVR_CMD_PAB_ADD_NEW_CONTACT:
            {   /* we need to read " add new contact" and go to StoreName */
                /* Featureverschiebung */
                _VCR_vT1SorryRepeat();
                break;
            }
            case AT_SIVR_CMD_PAB_NAME_PLAY_ADDBOOK:
            {
                _VCR_gData.resultInfo.playPB.num_pb_entries =
                    APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
                /* check if the result is with name index - if so set cur = idx */
                if (AT_SIVR_RES_IDs == _VCR_gData.stMsgRes.stResults[0].u.id.type)
                {
                    if (AT_TRUE == _VCR_gData.stMsgRes.stResults[0].u.id.id1_available)
                    {   /* id 1 is the item id -> fill into data */
                        _VCR_gData.resultInfo.playPB.name_avail = BT_TRUE;
                        _VCR_gData.resultInfo.playPB.name_idx =
                            _VCR_gData.stMsgRes.stResults[0].u.id.id1;
                    }
                    else
                    {
                        _VCR_gData.resultInfo.playPB.name_avail = BT_FALSE;
                        _VCR_gData.resultInfo.playPB.name_idx = 0;
                    }
                }
                else
                {   /* no name - handle as read all */
                    _VCR_gData.resultInfo.playPB.name_avail = BT_FALSE;
                    _VCR_gData.resultInfo.playPB.name_idx = 0;
                }
                _VCR_gData.resultInfo.playPB.cur_entry = _VCR_gData.resultInfo.playPB.name_idx;
                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR readpb name pb_entries %d  name_idx %d",
                         _VCR_gData.resultInfo.playPB.num_pb_entries,
                         _VCR_gData.resultInfo.playPB.name_idx);


                _VCR_vSinglePrompt(AT_SIVR_PRMT_PRESS_PTT);
                _VCR_gData.eNextMenuState = AT_SIVR_MENU_PLAYADDRESSBOOKENTRY;
                break;
            }
            case AT_SIVR_CMD_PAB_READ_ALL:
            {   /* store the name to be used as start point and fetch pb entry numbers */
                _VCR_gData.resultInfo.playPB.num_pb_entries =
                    APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
                _VCR_gData.resultInfo.playPB.name_avail = BT_FALSE;
                _VCR_gData.resultInfo.playPB.cur_entry = 0;
                _VCR_gData.resultInfo.playPB.name_idx = 0;

                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR readpb name pb_entries %d  name_avail %d",
                       _VCR_gData.resultInfo.playPB.num_pb_entries,
                       _VCR_gData.resultInfo.playPB.name_avail);

                _VCR_vSinglePrompt(AT_SIVR_PRMT_PRESS_PTT);
                _VCR_gData.eNextMenuState = AT_SIVR_MENU_PLAYADDRESSBOOKENTRY;
                break;
            }

            case AT_SIVR_CMD_PAB_REFRESH_ADDBOOK:
            {   /* check if addressbook download in progress */
/*
                if (APP_PHONEBOOK_STATUS_DOWNLOAD_IPR ==
                    APP_PHONEBOOK_AT_ePhonebookStatusGet())
*/
                if(BT_TRUE == APP_PHONEBOOK_bPbUpdateInProgress())   // automatic or manual sync
                {

                   /* hier noch alte Funktion nutzen */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_ADDRESSBOOK_DOWNLOAD_IN_PROGRESS);
                    _VCR_gData.eNextMenuState = AT_SIVR_MENU_MAIN;
                }
                else
                {   /* trigger PB manual sync */
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_MANUAL_PB_SYNC);
                }

                break;
            }
            case AT_SIVR_CMD_PAB_QS_PLAY_ADDBOOK:
            {
                _VCR_vFunctionNotAvailableStop();
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
/* Function    : _VCR_sfHandlePlayAddressbookEntryEvent                       */
/**
    \brief      event handler for event received in play addressbook entry

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
SuccessFailType _VCR_sfHandlePlayAddressbookEntryEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;

    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;
    Uint16Type tmp_idx = 0;
    *handled = BT_TRUE;

    if (is_event)
    {
        switch (event)
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */
            /* RT#1089*/
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
                        /* RT#6132: VR: Change PlayAdressbookEntry according to new Spec V11F*/
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_END_OF_ADDRESSBOOK_REPEAT);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }

            case VCR_MEVT_STRING_PRMT_OK:
            {   /* delay next list read */
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_DELAY_LIST_READ);
                break;
            }
            case VCR_MEVT_CALL_PB_SELECTION:
            case VCR_MEVT_DELAY_LIST_READ:
            {
                if (_VCR_gData.bPTTunhandled == BT_TRUE)
                {
                    _VCR_gData.bPTTunhandled = BT_FALSE;
                    /* RT#3567 SDS: No reaction on PTT button while PB is read */
                    if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_SELECT_ENTRY))
                    {
                        _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS);
                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"unhandled PTT is handled now");
                    }
                    else
                    {
                       DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"unhandled PTT could not handled");
                    }
                }
                else if (_VCR_gData.bHOOKunhandled == BT_TRUE)
                {
                    _VCR_gData.bHOOKunhandled = BT_FALSE;
                     DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"unhandled HOOK is handled now in PB");

                    /* we need to fetch the details for this contact - change the
                    type from read addressbook to VCR_RESULT_TYPE_CONTACT_UNI */
                    tmp_idx = _VCR_gData.resultInfo.playPB.cur_entry;
                    (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                    _VCR_gData.resultInfo.contact_uni.type = VCR_RESULT_TYPE_CONTACT_UNI;
                    _VCR_gData.resultInfo.contact_uni.item_type = AT_SIVR_ITEMTYPE_CONTACT;
                    _VCR_gData.resultInfo.contact_uni.item_id   = tmp_idx;
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                }
                else if (BT_TRUE == VCR_bCheckIsPTTPressed() ||
                    BT_TRUE == VCR_bCheckIsCALLPressed()  )
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0);
                }
                else
                {
                    /* increment number and if end not reached give out prompt */
                    _VCR_gData.resultInfo.playPB.cur_entry  = _VCR_gData.resultInfo.playPB.cur_entry + 1;
                    if (_VCR_gData.resultInfo.playPB.cur_entry <= _VCR_gData.resultInfo.playPB.num_pb_entries)
                    {
                        /* RT 3618
                           do checking for phone availability also while playing phonebook entries
                        */
                        if (BT_TRUE == ERM_bPhoneAvailable())
                        {
                           _VCR_vSinglePrompt(AT_SIVR_PRMT_D_TEXTTAG);
                        }
                        else
                        {
                           _VCR_vPhoneNotAvailableStop();
                        }
                    }
                    else
                    {   /* trigger prep for end of addressbook repeat */

                        /* RT#6132: VR: Change PlayAdressbookEntry according to new Spec V11F*/
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_END_OF_ADDRESSBOOK_REPEAT);

                    }
                }
                break;
            }
            case VCR_MEVT_PTT:
            case VCR_MEVT_OK:
            {
                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    /* RT#1379 */
                    /* system waits for user input, but user presses PTT -> stop the complete dialog */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                    /* send engine stop */
                }
                else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                     /* stop play if in progress and goto play addressbook options */

                     if (_VCR_gData.resultInfo.playPB.cur_entry <= _VCR_gData.resultInfo.playPB.num_pb_entries)
                     {
                         /* RT#3536 VR off after PTT at end of PB */
                         /* force stateswitch to MENU_PLAYADDRESSBOOKOPTIONS only if we are in the list - before listend */
                         (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_PTT );
                     }
                     else
                     {
                         *handled = BT_FALSE;
                     }
                }
                else
                {
                    // TB 24.10.2009, bugfix for phonebook event problem, if a Event
                    // was currently in pending state, the state was set but the event
                    // for this state was ignored. The VCR hangs in an unstable status
                    // and can't be finished. Now if a event problem occurs the old state
                    // will be continued and the new request will be ignored.
                    if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_SELECT_ENTRY))
                    {
                        _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS);
                    }
                    else
                    {
                        _VCR_gData.bPTTunhandled = BT_TRUE;
                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"handle PTT later");
                    }
                }
                break;
            }
            case VCR_MEVT_STR_PRMT_END_PTT:
            {
                _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS);
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_SELECT_ENTRY);
                break;
            }
            case VCR_MEVT_CALL:
            {
                if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_CALL );
                }
                else
                {
//RT#4017
                    _VCR_gData.bPTTunhandled = BT_FALSE;
                    _VCR_gData.bHOOKunhandled = BT_TRUE;

//RT#6321
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_CALL_PB_SELECTION);
                }
                break;
            }
            case VCR_MEVT_STR_PRMT_END_CALL:
            {
                /* we need to fetch the details for this contact - change the
                    type from read addressbook to VCR_RESULT_TYPE_CONTACT_UNI */
                    tmp_idx = _VCR_gData.resultInfo.playPB.cur_entry;
                    (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                    _VCR_gData.resultInfo.contact_uni.type = VCR_RESULT_TYPE_CONTACT_UNI;
                    _VCR_gData.resultInfo.contact_uni.item_type = AT_SIVR_ITEMTYPE_CONTACT;
                    _VCR_gData.resultInfo.contact_uni.item_id   = tmp_idx;
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                break;
            }
            case VCR_MEVT_APP_TRIG_PB_QUERY:
            {
                /* we need to give out complete list first via prompts, therefore
                 * we need to fetch the phone book information */
                appRsp = APP_PHONEBOOK_rcRequestCompletePbEntries(
                        VCR_vCBGetPBInfoSingleEntryUserPB,
                        APP_PHONEBOOK_enUsersPB,
                        _VCR_gData.resultInfo.contact_uni.item_id,1);
                switch (appRsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
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
                break;
            }
            case VCR_MEVT_PBENTRY_DATA_AVAIL:
            {   /* check the available data */
                if (VCR_RESULT_TYPE_CONTACT_UNI == _VCR_gData.resultInfo.contact_uni.type)
                {   /* check amount of available numbers */
                    /* print out data */
                    DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"VCR name data n=%d c=%d n_cat=%d loc_avail=%d",
                           _VCR_gData.resultInfo.contact_uni.n,
                           _VCR_gData.resultInfo.contact_uni.c,
                           _VCR_gData.resultInfo.contact_uni.n_cat,
                           _VCR_gData.resultInfo.contact_uni.location_avail);

                    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR name data location_id=%d IC=%d",
                           _VCR_gData.resultInfo.contact_uni.location_id,
                           VCR_bGetInstrumentClusterAvailable());

                    if (1 == _VCR_gData.resultInfo.contact_uni.n)
                    {   /* connect to this contact , play connect prompt */
                        _VCR_vConnectPrompt();
                    }
                    else
                    {   /* n > 1 - check c */
                        if (1 == _VCR_gData.resultInfo.contact_uni.c)
                        {   /* goto resolve multiple numbers */
                            _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                        }
                        else
                        {   /* goto dialnameconfirmcat */
                            _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                        }
                    }
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {   /* dial number and go back to idle */
                _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_FALSE;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
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
            case AT_SIVR_CMD_PAB_NAME_PLAY_ADDBOOK:
            {   /* entry from Play addressbook - play current index */
                if (0 == _VCR_gData.resultInfo.playPB.cur_entry)
                {
                    _VCR_gData.resultInfo.playPB.cur_entry = 1;
                }
                _VCR_vSinglePrompt(AT_SIVR_PRMT_D_TEXTTAG);
                break;
            }
            case AT_SIVR_CMD_PAB_YES_PLAY_PAB_ENTRY:
            case AT_SIVR_CMD_PAB_REPEAT_PAB_ENTRY:
            {   /* set counter back to 0 and start again */
                _VCR_gData.resultInfo.playPB.cur_entry = 0;
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_STRING_PRMT_OK);
                break;
            }
            case AT_SIVR_CMD_PAB_NO_PLAY_PAB_ENTRY:
            {   /* go back to main menu */
                _VCR_vChangeMenuToMain();
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            }
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                if (_VCR_gData.resultInfo.playPB.cur_entry == (_VCR_gData.resultInfo.playPB.num_pb_entries + 1))
                {
                     /* RT#3559 VR end of PB wrong help prompt
                        we are at the end ofthe list, we have to handle this specifi
                     */
                     DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"HELP CMD AFTER LIST END");

                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_HELP;
                    _VCR_gData.stProcMenu.ePromptId = _VCR_gData.eMenuState;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                else
                {
                    *handled = BT_FALSE;
                }
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
/* Function    : _VCR_sfHandlePlayAddressbookOptionsEvent                     */
/**
    \brief      event handler for event received in play addressbook options

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
SuccessFailType _VCR_sfHandlePlayAddressbookOptionsEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;


    Uint16Type tmp_idx = 0;
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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK_OPTIONS);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_SELECT_ENTRY:
            {   /* entry from PlayAddressBookEntry - start prepare */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* increment number and if end not reached give out prompt */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case VCR_MEVT_APP_TRIG_PB_QUERY:
            {
                /* we need to give out complete list first via prompts, therefore
                 * we need to fetch the phone book information */
                appRsp = APP_PHONEBOOK_rcRequestCompletePbEntries(
                        VCR_vCBGetPBInfoSingleEntryUserPB,
                        APP_PHONEBOOK_enUsersPB,
                        _VCR_gData.resultInfo.contact_uni.item_id,1);
                switch (appRsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
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
                break;
            }
            case VCR_MEVT_PBENTRY_DATA_AVAIL:
            {   /* check the available data */
                if (VCR_RESULT_TYPE_CONTACT_UNI == _VCR_gData.resultInfo.contact_uni.type)
                {   /* check amount of available numbers */
                    /* print out data */

                    DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"VCR name data n=%d c=%d n_cat=%d loc_avail=%d",
                           _VCR_gData.resultInfo.contact_uni.n,
                           _VCR_gData.resultInfo.contact_uni.c,
                           _VCR_gData.resultInfo.contact_uni.n_cat,
                           _VCR_gData.resultInfo.contact_uni.location_avail);

                    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR name data location_id=%d IC=%d",
                           _VCR_gData.resultInfo.contact_uni.location_id,
                           VCR_bGetInstrumentClusterAvailable());



                    if (1 == _VCR_gData.resultInfo.contact_uni.n)
                    {   /* connect to this contact , play connect prompt */
                        _VCR_vConnectPrompt();
                    }
                    else
                    {   /* n > 1 - check c */
                        if (1 == _VCR_gData.resultInfo.contact_uni.c)
                        {   /* check if instrument cluster available */
                            if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
                            {   /* use the ICluster */
                                /* trigger SD with index for multiple numbers */
                                _VCR_vNotifyApplication(VCR_NOTIFY_MULTIPLE_NUMBERS);
                                _VCR_gData.u8NumPromptRequired = 2;
                                _VCR_gData.u8NumPromptSend     = 0;
                                _VCR_gData.ePromtTable.tab[0] =
                                    AT_SIVR_PRMT_MULTIPLE_NUMBERS_POSSIBLE_CHOOSE_HAPTICAL;
                                _VCR_gData.ePromtTable.tab[1] =
                                    AT_SIVR_PRMT_SYSTEM_STOP;
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                            }
                            else
                            {   /* goto resolve multiple numbers */
                                _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                            }
                        }
                        else
                        {   /* goto dialnameconfirmcat */
                            _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                        }
                    }
                }
                else
                {
                    _VCR_PlayError(__LINE__);
                }
                break;
            }
            case VCR_MEVT_VT_QUERY_DONE:
            {
                if (BT_TRUE == _VCR_gData.resultInfo.playPB.voice_tag_avail)
                {   /* goto reset name confirm */
                    _VCR_vChangeState(AT_SIVR_MENU_RESETNAMECONFIRM);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_PAB_OPT_RESET_NAME );
                }
                else
                {   /* play prompt and retry */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_RESET_NOT_POSSIBLE);
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {   /* dial number and go back to idle */
                _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_FALSE;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
                break;
            }
            case VCR_MEVT_PTT:
            {
                /* RT #4469 */
                /* VR Beep missing */

                /* RT #5555 */
                /* VRS: Prompt cannot be stopped (record voice tag) */
                /* The flag is _VCR_gData.bPTTIgnore is also set to false in state AT_SIVR_ENG_END_PROMPT  */

                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    _VCR_gData.bPTTIgnore = BT_FALSE;
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                    /* send engine stop */
                    _VCR_vCancelGotoMain();
                }
                else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                     /* stop play if in progress */
                     if (BT_FALSE == _VCR_gData.bPTTIgnore)
                     {
                        *handled = BT_FALSE;
                     }
                     else
                     {
                          DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Ignore further PTT press");
                        // ignorefurther presses on PTT, wait for
                     }
                     _VCR_gData.bPTTIgnore = BT_TRUE;
                }
                else
                {
                     _VCR_gData.bPTTIgnore = BT_FALSE;
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
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
            case AT_SIVR_CMD_NO_RESETNAMECONFIRM:
            {   /* entry from ResetNameConfirm */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case  AT_SIVR_CMD_PAB_OPT_DIAL:
            {   /* fetch the dial infos for this entry */
                /* we need to fetch the details for this contact - change the
                   type from read addressbook to VCR_RESULT_TYPE_CONTACT_UNI */
                tmp_idx = _VCR_gData.resultInfo.playPB.cur_entry;
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_gData.resultInfo.contact_uni.type = VCR_RESULT_TYPE_CONTACT_UNI;
                _VCR_gData.resultInfo.contact_uni.item_type = AT_SIVR_ITEMTYPE_CONTACT;
                _VCR_gData.resultInfo.contact_uni.item_id   = tmp_idx;
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                break;
            }
            case  AT_SIVR_CMD_PAB_OPT_CHANGE_NAME:
            {   /* goto store name */
                _VCR_vChangeState(AT_SIVR_MENU_STORENAME);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_PAB_OPT_CHANGE_NAME );
                break;
            }
            case  AT_SIVR_CMD_PAB_OPT_RESET_NAME:
            {   /* we need to fetch the info if for this contact a voice tag is available */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_QUERY);
                break;
            }
            case  AT_SIVR_CMD_PAB_OPT_GO_AHEAD:
            case  AT_SIVR_CMD_PAB_OPT_GO_AHEAD_OLD:
            {   /* go back to play addressbook index, here increment is done */

                if ((_VCR_gData.resultInfo.playPB.cur_entry > 1) && (_VCR_gData.resultInfo.playPB.cur_entry == _VCR_gData.resultInfo.playPB.num_pb_entries))
                {
                   /* RT#3539 backward brosing: last Entry->first entry*/
                   _VCR_gData.resultInfo.playPB.cur_entry = 0;
                }
                _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOKENTRY);
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_STRING_PRMT_OK );
                break;
            }
            case  AT_SIVR_CMD_PAB_OPT_GO_BACK:
            case  AT_SIVR_CMD_PAB_OPT_GO_BACK_OLD:
            {   /* decrement current index and go back to play addressbook index */
                if (_VCR_gData.resultInfo.playPB.cur_entry > 1 )
                {
                    _VCR_gData.resultInfo.playPB.cur_entry =
                        _VCR_gData.resultInfo.playPB.cur_entry - 2;
                }
                else if ((_VCR_gData.resultInfo.playPB.cur_entry == 1) && (_VCR_gData.resultInfo.playPB.num_pb_entries > 0))
                {
                    /* RT#3539 backward brosing: first entry->last Entry*/
                    _VCR_gData.resultInfo.playPB.cur_entry =
                        _VCR_gData.resultInfo.playPB.num_pb_entries - 1;
                }

                /* else we do NOT modify the entry */
                _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOKENTRY);
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_STRING_PRMT_OK );
                break;
            }
            case  AT_SIVR_CMD_PAB_OPT_NEXT_LETTER:
            case  AT_SIVR_CMD_PAB_OPT_PREV_LETTER:
            {
                _VCR_vFunctionNotAvailableStop();
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
/* Function    : _VCR_sfHandleCallStacksEvent                                 */
/**
    \brief      event handler for event received in call stacks

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
SuccessFailType _VCR_sfHandleCallStacksEvent(
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
                    {
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
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
            case AT_SIVR_CMD_CALL_STACKS:
            case AT_SIVR_CMD_OPT_CALL_STACKS:
            {   /* entry from main menu - check availability of phone and function */
                if (BT_TRUE == ERM_bPhoneAvailable())
                {   /* check if callstack function available */
                    /* at the moment no ERM function availbale to get the information if a mobile supports callstacks  */
                    /* Workaround for Ticket 2468: if all callsatcks are zero it looks like Mobile does not support callstacks*/
                    if( (0 < APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enMissedCalls)) ||
                        (0 < APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enReceivedCalls)) ||
                        (0 < APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enLastDialed))
                      )
                    {
                        (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                    }
                    else
                    {
                        _VCR_vFunctionNotAvailableMain();
                    }
                }
                else
                {
                    _VCR_vPhoneNotAvailableStop();
                }
                break;
            }
            case AT_SIVR_CMD_CSTACK_RECEIVED_CALL:
            {
                _VCR_gData.resultInfo.cStack.type = VCR_RESULT_TYPE_CALLSTACKS;
                _VCR_gData.resultInfo.cStack.stack_type = APP_PHONEBOOK_enReceivedCalls;
                _VCR_vChangeState(AT_SIVR_MENU_READCALLSTACKS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CSTACK_RECEIVED_CALL );
                break;
            }
            case AT_SIVR_CMD_CSTACK_MISSED_CALL:
            {
                _VCR_gData.resultInfo.cStack.type = VCR_RESULT_TYPE_CALLSTACKS;
                _VCR_gData.resultInfo.cStack.stack_type = APP_PHONEBOOK_enMissedCalls;
                _VCR_vChangeState(AT_SIVR_MENU_READCALLSTACKS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CSTACK_MISSED_CALL );
                break;
            }
            case AT_SIVR_CMD_CSTACK_DIALLED_NUMBERS:
            {
                _VCR_gData.resultInfo.cStack.type = VCR_RESULT_TYPE_CALLSTACKS;
                _VCR_gData.resultInfo.cStack.stack_type = APP_PHONEBOOK_enLastDialed;
                _VCR_vChangeState(AT_SIVR_MENU_READCALLSTACKS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CSTACK_DIALLED_NUMBERS );
                break;
            }
            case AT_SIVR_CMD_CSTACK_CLEAR_ALL_STACK:
            {   /* goto call stacks reset confirm */
                _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKSRESETCONFIRM);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CSTACK_CLEAR_ALL_STACK );
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
/* Function    : _VCR_sfHandleReadCallStacksEvent                             */
/**
    \brief      event handler for event received in read call stacks

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
SuccessFailType _VCR_sfHandleReadCallStacksEvent(
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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_READ_CALL_STACKS);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {
                if (0 == _VCR_gData.resultInfo.cStack.num_entries)
                {   /* this was due to prompt no entry - back to main */
                    _VCR_vChangeMenuToMain();
                }
                else if (0 == _VCR_gData.resultInfo.cStack.cur_entry)
                {   /* initial entry from upper menu */
                    _VCR_gData.resultInfo.cStack.cur_entry  =  1;
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                }
                else
                {   /* this was due to read prompt - delay next prompt */
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_DELAY_LIST_READ);
                }
                break;
            }
            case VCR_MEVT_CALL_PB_SELECTION:
            case VCR_MEVT_DELAY_LIST_READ:
            {   /* check if PTT ok OK or CALL was already pressed,
                   if so do NOT increment but retrigger */
                if (_VCR_gData.bPTTunhandled == BT_TRUE)
                {
                    _VCR_gData.bPTTunhandled = BT_FALSE;
                    if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_PTT ))
                    {
                       DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"unhandled PTT is handled now in CS");

                       _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKSOPTIONS);
                    }
                    else
                    {
                       DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"unhandled PTT could not handled in CS");
                    }
                }
                else if (_VCR_gData.bHOOKunhandled == BT_TRUE)
                {
                    _VCR_gData.bHOOKunhandled  = BT_FALSE;
                    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"unhandled HOOK is handled now in CS");
					//RT#9085 unbekannt in VR Anrufliste falsches Namematching
                    if(BT_TRUE == _VCR_gData.resultInfo.cStack.number_restriced)
                    {
                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Bugfix VCR, Nummer unbekannt, HOOK button pressed");
                        _VCR_vNumberNotAvailableMain();
                    }
                    else
                    {
                        _VCR_gData.bHOOKIgnore = BT_TRUE;
                        _VCR_vConnectPrompt();
                    }
                }
                else if (BT_TRUE == VCR_bCheckIsPTTPressed() ||
                    BT_TRUE == VCR_bCheckIsCALLPressed()  )
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0);
                }
                else
                {
                    /* increment number and if end not reached fetch next data */
                    _VCR_gData.resultInfo.cStack.cur_entry  =
                        _VCR_gData.resultInfo.cStack.cur_entry + 1;
                    if ((_VCR_gData.resultInfo.cStack.cur_entry <=
                        _VCR_gData.resultInfo.cStack.num_entries ) &&
                        (_VCR_gData.resultInfo.cStack.cur_entry <= 10)) // Spec: the maximum number of entries per call stack is: 10  RT#4816
                    {
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                    }
                    else
                    {   /* trigger prep for end of addressbook repeat */
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_NO_FURTHER_ENTRIES_IN_LIST_REPEAT);
                    }
                }
                break;
            }
            case VCR_MEVT_APP_TRIG_PB_QUERY:
            {   /* fetch PB info */
                appRsp = APP_PHONEBOOK_rcRequestCompletePbEntries(
                        VCR_vCBGetPBInfoSingleEntryCallStack,
                        _VCR_gData.resultInfo.cStack.stack_type,
                        _VCR_gData.resultInfo.cStack.cur_entry,1);
                switch (appRsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
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
                break;
            }
            case VCR_MEVT_PBENTRY_DATA_AVAIL:
            {   /* number available - read it */
                if (_VCR_gData.resultInfo.cStack.name_avail)
                {
                      // we have a name search for the contact ID, maybe we have a voice tag
                      _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_SEARCH_ID);
                }
                else
                {
                      // we have no name search, play the number only
                      _VCR_vSinglePrompt(AT_SIVR_PRMT_CHOSEN_LIST);
                }
                break;
            }
            // RT#4416VR wrong name tag in call stack
            case VCR_MEVT_APP_TRIG_PB_SEARCH_ID:
            {
                /* fetch PB Index  */
                appRsp = APP_PHONEBOOK_rcRequestPbSearch(
                        VCR_vCBGetPBIndexEntryCallStack,
                        _VCR_gData.resultInfo.cStack.name,
                        _VCR_gData.resultInfo.cStack.name_len);
                switch (appRsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_SEARCH_ID);
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
                break;
            }
            case VCR_MEVT_APP_PB_ID_AVAIL:
            {
				//RT#9085 unbekannt in VR Anrufliste falsches Namematching
                if ((_VCR_gData.resultInfo.cStack.cur_entryPBindex) && (BT_FALSE == _VCR_gData.resultInfo.cStack.number_restriced))
                {

                    // we have a name in the phonebook for this entry of the callstack
                    // play over this special prompt
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_CHOSEN_LIST_VOICETAG);
                }
                else
                {
                    // no entry in phonebook
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_CHOSEN_LIST);
                }
                
                break;
            }
            case VCR_MEVT_PTT:
            case VCR_MEVT_OK:
            {   /* goto callStackoptions */
                if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_PTT );
                }
                else
                {
                    if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(event))
                    {
                        _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKSOPTIONS);
                    }
                    else
                    {
                       _VCR_gData.bPTTunhandled = BT_TRUE;
                       DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"handle PTT later in CS");

                    }
                }
                break;
            }
            case VCR_MEVT_STR_PRMT_END_PTT:
            case VCR_MEVT_STR_PRMT_END_OK:
            {
                _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKSOPTIONS);
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_PTT );
                break;
            }
            case VCR_MEVT_CALL:
            {   /* connect to this call stack entry */
                if (BT_FALSE == _VCR_gData.bHOOKIgnore)
                {
                   //RT#6937 phone key press repeats dial prompt

                   if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                   {
                       (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_CALL );
                   }
                   else
                   {
                       _VCR_gData.bPTTunhandled = BT_FALSE;
   //RT#4015 Bug of Testdrive: Eventcall ausloesen in Pause zwischen den Namen
                       _VCR_gData.bHOOKunhandled = BT_TRUE;
   //                    _VCR_vConnectPrompt();
                       _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_CALL_PB_SELECTION);

                   }
                }
                else
                {
                    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Hook Button ignored in ReadCallStacksEvent");
                }
                break;
            }
            case VCR_MEVT_STR_PRMT_END_CALL:
            {
                //#RT#6936 VR wrong prompt for empty callstack
                _VCR_gData.resultInfo.cStack.num_entries = APP_PHONEBOOK_u16GetCurrentPBSize(_VCR_gData.resultInfo.cStack.stack_type);
                //RT#9085 unbekannt in VR Anrufliste falsches Namematching
				if ((0 == _VCR_gData.resultInfo.cStack.num_entries) 
                    || (_VCR_gData.resultInfo.cStack.phone_number_len == 0)
                    || (BT_TRUE == _VCR_gData.resultInfo.cStack.number_restriced))
                {
                    _VCR_vNumberNotAvailableMain();
                }
                else
                {
                    _VCR_gData.bHOOKIgnore = BT_TRUE;
                    _VCR_vConnectPrompt();
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {   /* dial number and go back to idle */
                 (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
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
            case AT_SIVR_CMD_CSTACK_RECEIVED_CALL:
            case AT_SIVR_CMD_CSTACK_MISSED_CALL:
            case AT_SIVR_CMD_CSTACK_DIALLED_NUMBERS:
            {   /* entry from CallStacks - check phone */
                if (BT_TRUE == ERM_bPhoneAvailable())
                {   /* TODO - is there a possibility that the "function" is not avail? */
//                {   /* check if callstack function available */
                    /* at the moment no ERM function availbale to get the information if a mobile supports callstacks  */
//                    if (TRUE)
//                    {   /* check if there are entries in the choosen list */
                        _VCR_gData.resultInfo.cStack.num_entries =
                            APP_PHONEBOOK_u16GetCurrentPBSize(
                                    _VCR_gData.resultInfo.cStack.stack_type);
                        if (0 == _VCR_gData.resultInfo.cStack.num_entries)
                        {
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_NO_ENTRY_IN_CHOSEN_LIST);
                        }
                        else
                        {   /* start reading of entries in ReadCallStacks */
                            _VCR_gData.resultInfo.cStack.cur_entry = 0;
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_L_READ_LIST);
                        }
//                    }
//                    else
//                    {
//                        _VCR_vFunctionNotAvailableMain();
//                    }
                }
                else
                {
                    _VCR_vPhoneNotAvailableStop();
                }
                break;
            }
            case AT_SIVR_CMD_CSTACK_YES_READ:
            case AT_SIVR_CMD_CSTACK_REPEAT:
            {   /* set counter back to first entry and restart reading */
                _VCR_gData.resultInfo.cStack.cur_entry = 0;
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_DELAY_LIST_READ);
                break;
            }
            case AT_SIVR_CMD_CSTACK_NO_READ:
            {   /* back to main menu */
                _VCR_vChangeMenuToMain();
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
/* Function    : _VCR_sfHandleCallStackOptionsEvent                           */
/**
    \brief      event handler for event received in call stacks options

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
SuccessFailType _VCR_sfHandleCallStackOptionsEvent(
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
                    {
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_OPTIONS);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_PTT:
            case VCR_MEVT_OK:
            {   /* entry from ReadCallStacks */

                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    /* RT #1325 */
                    /* system waits for user input, but user presses PTT -> stop the complete dialog */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                    /* send engine stop */
                }
                else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                   /* RT #1972 is correct handled in generic handling  */
                   *handled = BT_FALSE;
                }
                else
                {
                   (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {
                (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
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
            case AT_SIVR_CMD_CSTACK_DIAL_CSOPT:
            {   /* connect to this contact , fetch information from PB */

                // RT#4941 unknown number in VR callstacks calls next entry
                // Since Parrot-SW 3.11.9 "unbekannt" is also synced
                // => For thhe VR -Application this looks like a valid name
                // Fix: check now also teh number length

                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"name_avail=%d phone_number_len%d",
                _VCR_gData.resultInfo.cStack.name_avail,
                _VCR_gData.resultInfo.cStack.phone_number_len);

                if (_VCR_gData.resultInfo.cStack.phone_number_len >= 1)
                {
                    _VCR_vConnectPrompt();
                }
                else
                {
                    _VCR_vNumberNotAvailableMain();
                }
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
/* Function    : _VCR_sfHandleCallStackResetConfirmEvent                      */
/**
    \brief      event handler for event received in call stacks reset confirm

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
SuccessFailType _VCR_sfHandleCallStackResetConfirmEvent(
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
                    {
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_CALL_STACKS_RESET_CONFIRM);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* call stacks deleted - back to main*/
                _VCR_vChangeMenuToMain();
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
            case AT_SIVR_CMD_CSTACK_CLEAR_ALL_STACK:
            {   /* entry from CallStacks */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
            case AT_SIVR_CMD_CSTACK_YES_RESETCONF:
            {   /* clear all call stacks and then go back to main TODO add APP function here */
                _VCR_vSinglePrompt(AT_SIVR_PRMT_CALL_STACKS_RESET);
                break;
            }
            case AT_SIVR_CMD_CSTACK_NO_RESETCONF:
            {   /* go back to CallStacks */
                _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CALL_STACKS );
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
/* Function    : _VCR_sfHandleReadSMSEvent                                    */
/**
    \brief      event handler for event received in read sms

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
SuccessFailType _VCR_sfHandleReadSMSEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
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
                    {   /* send first prompt only and after prompt stop trigger restart */
                        /* set back error counter */
                        _VCR_vSinglePrompt(AT_SIVR_PRMT_T2_OPTIONS_READSMS);
                        break;
                    }
                    case 3:
                    default:
                    {
                        _VCR_vCancelGotoMain();
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* check if complete SMS was read */
                if (_VCR_gData.resultInfo.readSMS.message_len ==
                    _VCR_gData.resultInfo.readSMS.offset_len)
                {   /* SMS is completely read */
                    /* check if this was "unread" and we have to remind to mark
                     * this as read */
                    if ((0 < _VCR_gData.resultInfo.readSMS.unread_sms) &&
                        (_VCR_gData.resultInfo.readSMS.current_num_sms <
                            _VCR_gData.resultInfo.readSMS.unread_sms ) &&
                         (_VCR_gData.resultInfo.readSMS.current_num_sms >=
                             _VCR_gData.u16NumSMSMarkRead ))
                    {
                        _VCR_gData.u16NumSMSMarkRead =
                            _VCR_gData.resultInfo.readSMS.current_num_sms + 1;
                    }

                    /* check if this was haptic read */
                    if (BT_TRUE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                    {   /* switch off with system stop prompt */
                        _VCR_vSystemStopToIdle(BT_FALSE);
                    }
                    else
                    {   /* trigger next read */
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Trigger next read Current %d Total %d",
                                                        _VCR_gData.resultInfo.readSMS.current_num_sms,
                                                        _VCR_gData.resultInfo.readSMS.total_num_sms);
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_DELAY_LIST_READ);
                    }
                }
                else
                {   /* only trigger read of next text part immediately */
                    if (_VCR_gData.resultInfo.readSMS.current_num_sms == _VCR_gData.resultInfo.readSMS.total_num_sms)
                    {
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Read %d of %d - no next read",
                                                      _VCR_gData.resultInfo.readSMS.current_num_sms,
                                                      _VCR_gData.resultInfo.readSMS.total_num_sms);
                        /* RT#1135 ask for retry */
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_PRMT_REPEAT_SMS;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU);
                    }
                    else
                    {
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Trigger next read immediately Current %d Total %d",
                                                        _VCR_gData.resultInfo.readSMS.current_num_sms,
                                                        _VCR_gData.resultInfo.readSMS.total_num_sms);

                       _VCR_vSinglePrompt(AT_SIVR_PRMT_SMS_XPLUSPLUS);
                    }
                }
                break;
            }
            case VCR_MEVT_DELAY_LIST_READ:
            {   /* prompt and delay ended - check for further SMS */
                if (BT_TRUE == VCR_bCheckIsPTTPressed() ||
                    BT_TRUE == VCR_bCheckIsCALLPressed()  )
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0);
                }
                else
                {
                    /* prompt and delay ended - check for further SMS */
                    /* first reset offset len */
                    _VCR_gData.resultInfo.readSMS.offset_len = 0;
                    if (_VCR_gData.resultInfo.readSMS.current_num_sms <
                         ( _VCR_gData.resultInfo.readSMS.total_num_sms - 1) )
                    {   /* there are more SMS to be read - read next */
                        _VCR_gData.resultInfo.readSMS.current_num_sms =
                            _VCR_gData.resultInfo.readSMS.current_num_sms + 1;
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
                    }
                    else if (_VCR_gData.resultInfo.readSMS.current_num_sms ==
                            ( _VCR_gData.resultInfo.readSMS.total_num_sms - 1) )
                    {   /* end of the list reached */
                        if (BT_TRUE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                        {
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NO_FURTHER_NEW_SMS;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
                            _VCR_gData.u8NumPromptRequired = 2;
                            _VCR_gData.u8NumPromptSend     = 0;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                        }
                        else
                        {
                            /* triger */
                             _VCR_gData.resultInfo.readSMS.current_num_sms++;
                             _VCR_vSinglePrompt(AT_SIVR_PRMT_NO_FURTHER_SMS);
                        }
                    }
                    else
                    {   /* after read prompt and number bigger than maxnum -> trigger prepare */
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU);
                    }
                }
                break;
            }
            case VCR_MEVT_READ_SMS:
            {
                rsp = APP_SMS_rcReadSms( VCR_vCBReadSMS,
                        _VCR_gData.resultInfo.readSMS.current_num_sms + 1 , 1);
                switch (rsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
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
                break;
            }
            case VCR_MEVT_SMS_INFO_AVAIL:
            {   /* the SMS info is available - read it */
                if (0 == _VCR_gData.resultInfo.readSMS.current_num_sms)
                {   /* this was first read - set 2 prompts */

                    if (BT_TRUE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                    {
                        if (VCR_HAPTIC_READ_SMS == _VCR_gData.eHapticTrigType)
                        {
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SMS_XPLUSPLUS;
                            _VCR_gData.u8NumPromptRequired = 1;
                            _VCR_gData.u8NumPromptSend     = 0;
                        }
                        else
                        {
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_I_START_SMS_PLAYBACK;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SMS_XPLUSPLUS;
                            _VCR_gData.u8NumPromptRequired = 2;
                            _VCR_gData.u8NumPromptSend     = 0;
                        }
                    }
                    else
                    {
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_I_START_SMS_PLAYBACK;
                        _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SMS_XPLUSPLUS;
                        _VCR_gData.u8NumPromptRequired = 2;
                        _VCR_gData.u8NumPromptSend     = 0;
                    }
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                }
                else
                {
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_SMS_XPLUSPLUS);
                }
                break;
            }
            case VCR_MEVT_PTT:
            case VCR_MEVT_OK:
            {   /* stop current prompt and GOTO read SMS options */
                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                     /* RT#1136*/
                    /* system waits for user input, but user presses PTT -> stop the complete dialog */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                    /* send engine stop */
                }
                else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                      if (_VCR_gData.resultInfo.readSMS.is_first_read_sms == BT_TRUE)
                      {
                          *handled = BT_FALSE;
                      }
                      else
                      {
                         (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_PTT );
                      }
                }
                else
                {
                    _VCR_vChangeState(AT_SIVR_MENU_READSMSOPTIONS);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
                }
                break;
            }
            case VCR_MEVT_STR_PRMT_END_PTT:
            case VCR_MEVT_STR_PRMT_END_OK:
            {
                _VCR_vChangeState(AT_SIVR_MENU_READSMSOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
                break;
            }
            case VCR_MEVT_CALL:
            {
                if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_CALL );
                }
                else
                {
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIGGER);
                }
                break;
            }
            case VCR_MEVT_STR_PRMT_END_CALL:
            {
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIGGER);
                break;
            }
            case VCR_MEVT_APP_TRIGGER:
            {   /* we need to call the sender of the current SMS */
                rsp = APP_PHONE_rcDialNumber(VCR_vCBSendMsgViaAPP,
                        (const Uint8Type *)&(_VCR_gData.resultInfo.readSMS.origin),
                        _VCR_gData.resultInfo.readSMS.origin_len);
                switch (rsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send specific event */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIGGER);
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
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
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

            case AT_SIVR_CMD_READ_SMS:
            case AT_SIVR_CMD_OPT_READ_SMS:
            {   /* entry trigger from main or more options */
                /* check if phone available */
                if (BT_TRUE == ERM_bPhoneAvailable())
                {   /* phone available - check if SMS is supported */
                    if (BT_TRUE == APP_SMS_bIsSmsInitialized())
                    {   /* check availability of SMS */
                        /* TODO - do we need only unread or also read SMS? */
                        _VCR_gData.resultInfo.readSMS.type = VCR_RESULT_TYPE_READSMS;
                        _VCR_gData.resultInfo.readSMS.unread_sms = APP_SMS_u16GetNumUnreadSms();
                        if (BT_FALSE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                        {
                            _VCR_gData.resultInfo.readSMS.read_sms = APP_SMS_u16GetNumReadSms();
                        }
                        else
                        {
                          _VCR_gData.resultInfo.readSMS.read_sms = 0;
                        }
                        _VCR_gData.resultInfo.readSMS.total_num_sms = _VCR_gData.resultInfo.readSMS.unread_sms + _VCR_gData.resultInfo.readSMS.read_sms;
                        _VCR_gData.resultInfo.readSMS.current_num_sms = 0;
                        _VCR_gData.resultInfo.readSMS.offset_len = 0;

                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR SMS numbers unread %d read %d",
                                                          _VCR_gData.resultInfo.readSMS.unread_sms,
                                                          _VCR_gData.resultInfo.readSMS.read_sms);


                        if (0 < _VCR_gData.resultInfo.readSMS.total_num_sms)
                        {   /* trigger read of SMS */
                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
                        }
                        else
                        {   /* no SMS - read prompt and back to main */
                            _VCR_vChangeState(AT_SIVR_MENU_MAIN);
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_NO_SMS);
                        }
                    }
                    else
                    {   /* function not available - back to main */
                        _VCR_vFunctionNotAvailableMain();
                    }
                }
                else
                {   /* phone not available and stop system - back to idle */
                    _VCR_vPhoneNotAvailableStop();
                }
                break;
            }
            case AT_SIVR_CMD_READSMS_YES:
            case AT_SIVR_CMD_READSMS_REPEAT:
            {   /* reset current number and restart */
                _VCR_gData.u8ProcErr  = 0;
                _VCR_gData.resultInfo.readSMS.current_num_sms = 0;
                /* trigger read again */
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
                break;
            }
            case AT_SIVR_CMD_READSMS_NO:
            {   /* back to main menu */
                _VCR_vChangeMenuToMain();
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
/* Function    : _VCR_sfHandleReadSMSOptionsEvent                             */
/**
    \brief      event handler for event received in rad sms option

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
SuccessFailType _VCR_sfHandleReadSMSOptionsEvent(
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
                    {   /* send first prompt only and after prompt stop trigger restart */
                        /* set back error counter */
                        _VCR_vSinglePrompt(AT_SIVR_PRMT_T2_READ_SMS_OPTIONS);
                        break;
                    }
                    case 3:
                    default:
                    {
                        if (BT_TRUE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                        {
                            _VCR_vCancelStopFor3rdError();
                        }
                        else
                        {
                            _VCR_vCancelGotoMain();
                        }
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* we will only run in here if single prompt for no next
                   or no previous is called - back to read menu with prepare */

                 // RT#5020 VCR: missing vcr prompt when say vorherige nachricht during first message is read

                _VCR_vChangeState(AT_SIVR_MENU_READSMSOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );

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
            case AT_SIVR_CMD_READ_SMS:
            {   /* entry from Read SMS */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_READSMSOPT_CALL_YES:
            {   /* goto call back SMS confirm */
                _VCR_vChangeState(AT_SIVR_MENU_CALLBACKSMSCONFIRM);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READSMSOPT_CALL_YES );
                break;
            }
            case AT_SIVR_CMD_READSMSOPT_REPEAT:
            {   /* go back to Read SMS and re-read current SMS */
                _VCR_gData.resultInfo.readSMS.offset_len = 0;
                _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
                break;
            }
            case AT_SIVR_CMD_READSMSOPT_DELETE:
            {   /* goto delete SMS confirm */
                _VCR_vChangeState(AT_SIVR_MENU_DELETESMSCONFIRM);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READSMSOPT_DELETE );
                break;
            }
            case AT_SIVR_CMD_READSMSOPT_PREVIOUS:
            {   /* check if there is a previous SMS, if so set prev and go back */
                _VCR_gData.resultInfo.readSMS.offset_len = 0;
                if (0 < _VCR_gData.resultInfo.readSMS.current_num_sms)
                {
                    _VCR_gData.resultInfo.readSMS.current_num_sms =
                        _VCR_gData.resultInfo.readSMS.current_num_sms - 1;
                    _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
                }
                else
                {   /* prompt no previous message */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_NO_PREVIOUS_MESSAGE);
                }
                break;
            }
            case AT_SIVR_CMD_READSMSOPT_NEXT:
            {
                _VCR_gData.resultInfo.readSMS.offset_len = 0;
                if (_VCR_gData.resultInfo.readSMS.current_num_sms <
                        ( _VCR_gData.resultInfo.readSMS.total_num_sms - 1))
                {
                    _VCR_gData.resultInfo.readSMS.current_num_sms =
                        _VCR_gData.resultInfo.readSMS.current_num_sms + 1;
                    _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS);
                }
                else
                {   /* prompt no next message */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_NO_NEXT_MESSAGE);
                }
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
/* Function    : _VCR_sfHandleDeleteSMSConfirmEvent                           */
/**
    \brief      event handler for event received in delete sms confirm

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
SuccessFailType _VCR_sfHandleDeleteSMSConfirmEvent(
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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_DELETE_SMS_CONFIRM);
                        break;
                    }
                    case 3:
                    default:
                    {
                        if (BT_TRUE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                        {
                            _VCR_vCancelStopFor3rdError();
                        }
                        else
                        {
                            _VCR_vCancelGotoMain();
                        }
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
            case AT_SIVR_CMD_READSMSOPT_DELETE:
            {   /* entry event - trigger prep */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_DELETESMS_YES:
            case AT_SIVR_CMD_DELETESMS_DELETE:
            {   /* delete SMS */
                rsp = APP_SMS_rcDeleteSms( VCR_vCBDeleteSMS,
                        _VCR_gData.resultInfo.readSMS.current_num_sms + 1 , 1, APP_SMS_enRspVcr);
                switch (rsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DELETESMS_YES);
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
                break;
            }
            case AT_SIVR_CMD_DELETESMS_NO:
            {   /* back to Read SMS options */
                _VCR_vChangeState(AT_SIVR_MENU_READSMSOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
                break;
            }
            case AT_SIVR_CMD_CANCEL:
            case AT_SIVR_CMD_HELP:
            {   /* used for all events that do not need to be handled */
                *handled = BT_FALSE;
                break;
            } default:
            {   /* send prompt not supported */
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfHandleCallBackSMSConfirmEvent                         */
/**
    \brief      event handler for event received in call back sms confirm

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
SuccessFailType _VCR_sfHandleCallBackSMSConfirmEvent(
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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_CALLBACK_SMS_CONFIRM);
                        break;
                    }
                    case 3:
                    default:
                    {
                        if (BT_TRUE == _VCR_gData.resultInfo.readSMS.is_haptic_trigger)
                        {
                            _VCR_vCancelStopFor3rdError();
                        }
                        else
                        {
                            _VCR_vCancelGotoMain();
                        }
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
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
            case AT_SIVR_CMD_READSMSOPT_CALL_YES:
            {   /* entry trigger */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_CALLBACKSMS_YES:
            case AT_SIVR_CMD_CALLBACKSMS_DIAL:
            {   /* call the SMS origin */
                rsp = APP_PHONE_rcDialNumber(VCR_vCBSendMsgViaAPP,
                        (const Uint8Type *)&(_VCR_gData.resultInfo.readSMS.origin),
                        _VCR_gData.resultInfo.readSMS.origin_len);
                switch (rsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send specific event */
                        _VCR_vSendCmdToMainEventHandler(cmd);
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
                break;
            }
            case AT_SIVR_CMD_CALLBACKSMS_NO:
            {   /* back to Read SMS options */
                _VCR_vChangeState(AT_SIVR_MENU_READSMSOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
            }
            case AT_SIVR_CMD_CANCEL:                // Fallthrough
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
/* Function    : _VCR_sfHandleStorenameEvent                                  */
/**
    \brief      event handler for event received in store name

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
SuccessFailType _VCR_sfHandleStorenameEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    static BooleanType is_eprom_error = BT_FALSE;
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;

    static BooleanType stay_Storename_after_prompt = BT_FALSE;

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
                        /*RT#2412 VCR: SDS Spec 4.5.3.2 prompt T2 not spoken */
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_STORENAME);
                        break;
                    }
                    case 3:
                    default:
                    {
                        if (BT_TRUE == _VCR_gData.resultInfo.playPB.is_haptic_trigger)
                        {
                            _VCR_vCancelStopFor3rdError();
                        }
                        else
                        {
                            _VCR_vCancelGotoMain();
                        }
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_NAME_RECORD_FAIL:
            {   /* recording fails - retry */
                _VCR_gData.u8ProcErr = _VCR_gData.u8ProcErr + 1;
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_PROC_ERROR);
                break;
            }
            case VCR_MEVT_NAME_RECORD_FAIL_SHORT:
            {   /* recording fails - retry */
                _VCR_gData.u8ProcErr = _VCR_gData.u8ProcErr + 1;
                if (_VCR_gData.u8ProcErr <= 2)
                {


//                     _VCR_vSinglePrepare(AT_SIVR_PRMT_NAME_TOO_SHORT);
                    stay_Storename_after_prompt = BT_TRUE;
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_NAME_TOO_SHORT);
                }
                else
                {   /* reached maximum error counter -> stop */
                    if (BT_TRUE == _VCR_gData.resultInfo.playPB.is_haptic_trigger)
                    {
                        _VCR_gData.u8ProcErr = 0;
                        _VCR_gData.u8NumPromptRequired = 3;
                        _VCR_gData.u8NumPromptSend = 0;
                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_TOO_SHORT;
                        _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_PROCESS_CANCELLED;
                        _VCR_gData.ePromtTable.tab[2] = AT_SIVR_PRMT_SYSTEM_STOP;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                    }
                    else
                    {
                        _VCR_gData.u8ProcErr = 0;
                        _VCR_gData.u8NumPromptRequired = 2;
                        _VCR_gData.u8NumPromptSend = 0;
                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_TOO_SHORT;
                        _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_PROCESS_CANCELLED;
                        _VCR_vChangeState(AT_SIVR_MENU_MAIN);
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                    }
                }
                break;
            }
            case VCR_MEVT_NAME_RECORD_FAIL_NOT_UNIQUE:
            {   /* recording fails - retry */
                _VCR_gData.u8ProcErr = _VCR_gData.u8ProcErr + 1;
                if (_VCR_gData.u8ProcErr <= 2)
                {
                    /* TODO problem with trained name param - use all the time name to short so far */
                      stay_Storename_after_prompt = BT_TRUE;
                      _VCR_vSinglePrompt(AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME);
//                      _VCR_vSinglePrompt(AT_SIVR_PRMT_NAME_TOO_SHORT);

                }
                else
                {   /* reached maximum error counter -> stop */
                    if (BT_TRUE == _VCR_gData.resultInfo.playPB.is_haptic_trigger)
                    {
                        _VCR_gData.u8ProcErr = 0;
                        _VCR_gData.u8NumPromptRequired = 3;
                        _VCR_gData.u8NumPromptSend = 0;
                        /* TODO  problem with trained name param - use all the time name to short so far */
                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME;
//                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_TOO_SHORT;
                        _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_PROCESS_CANCELLED;
                        _VCR_gData.ePromtTable.tab[2] = AT_SIVR_PRMT_SYSTEM_STOP;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                    }
                    else
                    {
                        _VCR_gData.u8ProcErr = 0;
                        _VCR_gData.u8NumPromptRequired = 2;
                        _VCR_gData.u8NumPromptSend = 0;
                        /* TODO problem with trained name param - use all the time name to short so far */
                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME;
//                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_TOO_SHORT;
                        _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_PROCESS_CANCELLED;
                        _VCR_vChangeState(AT_SIVR_MENU_MAIN);
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                    }
                }
                break;
            }
            case VCR_MEVT_NAME_RECORDED:
            {   /* recording success - check if this is new contact or existing one */
                if (BT_TRUE == _VCR_gData.resultInfo.playPB.is_add_new_contact)
                {
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_NAME_STORED);
                }
                else
                { /* - we need to store the name to the current
                    contact - therefore we first need to stop SIVR */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_VTRECORD;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                }

                break;
            }
            case VCR_MEVT_SIVR_STOPPED:
            {   /* now we need to link the recorded TAG to the current index,
                   but first we need to check if we have to delete already existing
                   VT, therefore we need to query the PB index first  */
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_QUERY);
                break;
            }
            case VCR_MEVT_VT_QUERY_DONE:
            {
                if (BT_TRUE == _VCR_gData.resultInfo.playPB.voice_tag_avail)
                {   /* we need to delete current VT first */
                    _VCR_gData.resultInfo.playPB.is_vt_overwrite = BT_TRUE;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_DELETE );
                }
                else
                {   /* add new VT */
                    _VCR_gData.resultInfo.playPB.is_vt_overwrite = BT_FALSE;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_ADD );
                }
                break;
            }
            case VCR_MEVT_VT_DELETE_DONE:
            {   /* now map new VT after delete of old one */

                // RT#1736
                // try again if Snchro is in progress, similiar problem as in older tickets

                if (AT_SIVR_ENG_BEGIN_SYNC != _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_ADD);
                }
                else
                {
                   // Voicetagrecording, if SIVR synchro is in progress - try again
                   _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_VT_DELETE_DONE);
                }

                break;
            }
            case VCR_MEVT_VT_ADD_DONE:
            {
                if (AT_SIVR_ENG_BEGIN_SYNC == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    // RT#3139 VR: [ITA] Cancels after voicetag recording
                    // Reason: VR sync has started, after adding, we have to wait for end
                    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"VR still in SYNC -> wait");

                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_VT_ADD_DONE);
                }
                else
                {
                /* here we need to increment the number of VTs for the current active user,
                 * if it was NOT overwriting of VT */
                if (BT_TRUE == _VCR_gData.resultInfo.playPB.is_vt_overwrite)
                {   /* start the engine again without modification of the VT number */
                    _VCR_gData.resultInfo.playPB.is_vt_overwrite = BT_FALSE;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_START_ENGINE );
                }
                else
                {   /* trigger increment of VT number */
                    is_eprom_error = BT_FALSE;
                    appRsp = ERM_rcChangeNumVoiceTagsForActiveUser_InEeprom(
                            VCR_vCBIncNumVoiceTagForActiveUser,1);
                    switch (appRsp)
                    {
                        case APP_COMMON_RSP_OK:
                        {   /* do nothing but wait for callback */
                            break;
                        }
                        case APP_COMMON_RSP_ERROR_BUSY:
                        {   /* we need to retry - send trigger event in here */
                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_VT_ADD_DONE);
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
                            is_eprom_error = BT_TRUE;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_START_ENGINE );
                            break;
                        }
                    }
                    break;
                     }
                }
                break;
            }
            case VCR_MEVT_SIVR_STARTED:
            {   /* prompt output */
                _VCR_gData.eSystemState = VCR_SYSTEM_STARTED_PTT;
                if (BT_TRUE == is_eprom_error)
                {
                    is_eprom_error = BT_FALSE;
                    _VCR_PlayError(__LINE__);
                }
                else
                {
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_NAME_STORED);
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* check if name for new or existing contact -> back to respective menu */
                if (stay_Storename_after_prompt)
                {
                     stay_Storename_after_prompt = BT_FALSE;
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                else if (BT_FALSE == _VCR_gData.resultInfo.playPB.is_add_new_contact)
                {   /* check if this was due to haptic or not */
                    if (VCR_HAPTIC_STORE_NAME != _VCR_gData.eHapticTrigType)
                    {   /* back to main */
                        _VCR_vChangeMenuToMain();
                    }
                    else
                    {   /* back to idle via stop prompt */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                        _VCR_gData.u8NumPromptRequired = 1;
                        _VCR_gData.u8NumPromptSend     = 0;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                    }

                }
                else
                {   /* modify the result type */
                    (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                    _VCR_gData.resultInfo.number.type = VCR_RESULT_TYPE_NUMBER;
                    _VCR_gData.resultInfo.number.is_for_contact = BT_TRUE;
                    _VCR_gData.resultInfo.number.voice_tag_idx  = 0;
                    /* go to DialNumber */
                    _VCR_vChangeState(AT_SIVR_MENU_DIALNUMBER);
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_ADDCONTACT_NUMBER );

                    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR store name change to numbertype %d is_for_contact %d",
                            _VCR_gData.resultInfo.number.type,
                            _VCR_gData.resultInfo.number.is_for_contact);
                }
                break;
            }
            case VCR_MEVT_PTT:
            {
                /* RT #1924 */
                /* system waits for user input, but user presses PTT -> stop the complete dialog */
                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                    /* send engine stop */
                    _VCR_vCancelGotoMain();
                }
                else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                     /* stop play if in progress and goto play addressbook options */
                    *handled = BT_FALSE;
                }
                else
                {
                   (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
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
            case AT_SIVR_CMD_PAB_OPT_CHANGE_NAME:
            {   /* entry from ReadAddressbookOptions, first check if we can add
                   further names */
                if (BT_TRUE == ERM_bMaxVoiceTagsForUserRecorder())
                {   /* prompt output and back to main */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_NAME_LIST_FULL_PLEASE_DELETE);
                }
                else
                {   /* go on */
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
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
/* Function    : _VCR_sfHandleResetNameConfirmEvent                           */
/**
    \brief      event handler for event received in reset name confirm

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
SuccessFailType _VCR_sfHandleResetNameConfirmEvent(
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
                    {   _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_RESET_NAME_CONFIRM);
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
                if (BT_FALSE == _VCR_gData.resultInfo.playPB.is_haptic_trigger)
                {
                    _VCR_vChangeMenuToMain();
                }
                else
                {   /* stop system */
                    _VCR_vSystemStopToIdle(BT_FALSE);
                }
                break;
            }
            case VCR_MEVT_VT_DELETE_DONE:
            {
                /* trigger decrement of VT number */
                appRsp = ERM_rcChangeNumVoiceTagsForActiveUser_InEeprom(
                        VCR_vCBDecNumVoiceTagForActiveUser,-1);
                switch (appRsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_VT_DELETE_DONE);
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
            case AT_SIVR_CMD_PAB_OPT_RESET_NAME:
            {   /* entry from PlayAddressbook Option */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_YES_RESETNAMECONFIRM:
            {   /* we need to delete the voicetag for this name */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_DELETE );
                break;
            }
            case AT_SIVR_CMD_NO_RESETNAMECONFIRM:
            {   /* go back to playaddressbookoptions */
                _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_NO_RESETNAMECONFIRM);
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

/* End Of File VCR_StateHandler_PHONEBOOK.c */
