/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler_PHONE.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 18:42:21 +0200 (Di, 04 Okt 2011) $
* $Rev: 22044 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_StateHandler_PHONE.c $
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

#include "VCR_StateHandler_PHONE.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "VCR_CE.h"
#include "VCR_CI.h"
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

static Uint8Type DBlock_Array[AT_MAX_PHONE_NUM_LEN] = {0};
static Uint8Type DBlock_Index = 0;



/*----------------------------------------------------------------------------*/
/* File local  Constant Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private  Constant
*/
static void _VCR_vSortingOfAmbiNames(Uint16Type *item_id, Uint8Type Num_Res); /* RT-13210 : Ambiguous names sorting*/
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Methods                                                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vRedial                                                 */
/**
    \brief      redial function - as there is no specific menu this will be
                called by main directly

    \brief


    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vRedial(void)
{
    if (BT_TRUE == ERM_bPhoneAvailable())
    {
        /* VW spec asked for availability of number and call stack
         * this needs to be clarified - so far the included sub every time
         * return TRUE*/
//        if (BT_TRUE == APP_PHONE_bRedialNumberAvailable())

        if(0!= APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enLastDialed))
        {   /* send the redial command via APP - if sending success, send prompt,
               otherwise trigger main again to retry */
            _VCR_gData.ePromtTable.tab[0]    = AT_SIVR_PRMT_I_REDIAL;
            _VCR_gData.ePromtTable.tab[1]    = AT_SIVR_PRMT_CONNECT;
            _VCR_gData.u8NumPromptRequired   = 2;
            _VCR_gData.u8NumPromptSend       = 0;
            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_REDIAL_PROMPT );
            /* when this prompt is played - indication given to main
             * handler, when in main, the redial APP function is called and
               system is stopped */

        }
        else
        {
            /* prepare prompt output and send prompt - back to main menu */
            _VCR_vSinglePrompt(AT_SIVR_PRMT_NUMBER_NOT_AVAIlABLE);
        }
    }
    else
    {   /* prepare prompt output and send end prompt, back to idle */
        _VCR_vPhoneNotAvailableStop();
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vHandleDialNameConfEvent                                    */
/**
    \brief      event handler for event received in DialNameConf state

    \brief

    \param      BooleanType * ptr to handled indication to indicate if event hdl
                VCRMenuEventEnum event that is received

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfHandleDialNameConfEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    BooleanType result_unique = BT_TRUE;
    Uint32Type i = 0;
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;

    *handled = BT_TRUE;

    if (is_event)
    {
        switch (event)
        {
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
                        if (BT_FALSE == _VCR_gData.resultInfo.contact_uni.location_avail)
                        {
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME);
                        }
                        else
                        {
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_CAT);
                       }
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
            case VCR_MEVT_PBENTRY_DATA_AVAIL:
            {   /* process the received data for next decisions */
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
                    {
                        //RT#4940
                        if((1 == _VCR_gData.resultInfo.contact_uni.n_cat) || (BT_FALSE == _VCR_gData.resultInfo.contact_uni.location_avail))
                        {
                           /* only one number!!! - check for voice feedback and confidence */
                        if ( (BT_TRUE == _VCR_gData.bVoiceFBOff) &&
                             (VCR_CONFIDENCE_HIGH_THRESHOLD <=
                                _VCR_gData.resultInfo.contact_uni.confidence_rate ))
                        {   /* directly trigger the call */
                            _VCR_vConnectPrompt();
                        }
                        else
                        {   /* confirm in here */
                            _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRM;
                            _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                        }

                    }
                        else
                        {
                           _VCR_gData.u8NumPromptRequired = 1;
                           _VCR_gData.u8NumPromptSend     = 0;
                           _VCR_gData.ePromtTable.tab[0] =
                               AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;

                           (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                        }
                    }
                    /* more than one number for the contact available */
                    else
                    {   /* check if command was given with category */
                        if (BT_FALSE == _VCR_gData.resultInfo.contact_uni.location_avail)
                        {   /* check if only one category */
                            if (1 == _VCR_gData.resultInfo.contact_uni.c)
                            {   /* check if instrument cluster available */
                                if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
                                {   /* trigger SD with index for multiple numbers */
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
                                {   /* goto resolve multiple num per category */
                                    _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                    break;
                                }
                            }
                            else
                            {   /* goto resolve multiple num per category */
                                _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES);
                                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                break;
                            }
                        }
                        else
                        {   /* command was with category - check ncat */
                            if (1 == _VCR_gData.resultInfo.contact_uni.n_cat)
                            {   /* confirm with category */
                                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRM;
                                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                            }
                            else if (0 == _VCR_gData.resultInfo.contact_uni.n_cat)
                            {
                                if (1 == _VCR_gData.resultInfo.contact_uni.n)
                                {
                                 // this code will never reached, reviewed Brust Braun
                                    _VCR_gData.u8NumPromptRequired = 2;
                                    _VCR_gData.u8NumPromptSend     = 0;
                                    _VCR_gData.ePromtTable.tab[0] =
                                        AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                    _VCR_gData.ePromtTable.tab[1] =
                                        AT_SIVR_PRMT_DO_YOU_WANT_TO_DIAL_NAME_CATEGORY;
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                                }
                                else
                                {
                                    if (1 == _VCR_gData.resultInfo.contact_uni.c)
                                    {
                                        if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
                                        {
                                            _VCR_vNotifyApplication(VCR_NOTIFY_MULTIPLE_NUMBERS);
                                            _VCR_gData.u8NumPromptRequired = 4;
                                            _VCR_gData.u8NumPromptSend     = 0;
                                            _VCR_gData.ePromtTable.tab[0] =
                                                AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                            _VCR_gData.ePromtTable.tab[1] =
                                                AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE;
                                            _VCR_gData.ePromtTable.tab[2] =
                                                AT_SIVR_PRMT_MULTIPLE_NUMBERS_POSSIBLE_CHOOSE_HAPTICAL;
                                            _VCR_gData.ePromtTable.tab[3] =
                                                AT_SIVR_PRMT_SYSTEM_STOP;
                                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                                        }
                                        else
                                        {
                                            /* change menu after prompt output*/
                                            // RT#4808 The VR does not resolve multiple numbers - bis hierher OK
                                            // Bugfix: so tun als ob keine location gesprochen wurde
                                            _VCR_gData.resultInfo.contact_uni.location_avail = BT_FALSE;

                                            _VCR_gData.eNextMenuState = AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY;
                                            _VCR_gData.u8NumPromptRequired = 2;
                                            _VCR_gData.u8NumPromptSend     = 0;
                                            _VCR_gData.ePromtTable.tab[0] =
                                                AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                            _VCR_gData.ePromtTable.tab[1] =
                                                AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE;
                                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                                        }
                                    }
                                    else
                                    {   /* change menu after prompt output*/
                                        _VCR_gData.eNextMenuState = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                                        _VCR_gData.u8NumPromptRequired = 2;
                                        _VCR_gData.u8NumPromptSend     = 0;
                                        _VCR_gData.ePromtTable.tab[0] =
                                            AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                        _VCR_gData.ePromtTable.tab[1] =
                                            AT_SIVR_PRMT_CHOOSE_OTHER_CATEGORY_INSTEAD;
                                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                                    }
                                }
                            }
                            else
                            {
                                if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
                                {   /* trigger SD with index for multiple numbers */
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
                                {   /* goto resolve multiple num per category */
                                    _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {   /* strange error here -> stop machine!!! */
                    _VCR_PlayError(__LINE__ );
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {   /* dial number and go back to idle */
                if (1 < _VCR_gData.resultInfo.contact_uni.n)
                {
                   /* RT#1380 multiple numbers */
                   _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                }
                else
                {
                   _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_FALSE;
                }

                (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* do not change the state */
                if (AT_SIVR_MENU_UNDEF == _VCR_gData.eNextMenuState)
                {
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRM;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                else
                {   /* go to next state */
                    _VCR_vChangeState(_VCR_gData.eNextMenuState);
                    _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                    if(AT_SIVR_MENU_MAIN == _VCR_gData.eMenuState)
                    {
                        _VCR_vChangeMenuToMain();
                    }
                    else
                    {
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                    }
                }
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
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                *handled = BT_FALSE;
                break;
            }
            default:
            {
                sF = FAIL;
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {   /* entry command from main menu */
            case AT_SIVR_CMD_DIAL:
            {   /* check if a result was received and if it was unique */
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRM;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                if (BT_TRUE == ERM_bPhoneAvailable())
                {
#if (1 == VCR_CHECK_NETWORK_AVAILABILITY)
                    if( 0 != APP_PHONE_GetNetworkAvailable())   // no change in functionality, call only same function as APP_Phone
#else
                    if(BT_TRUE)
#endif
                    {   /* query address book status */
                        switch (_VCR_eUserPhonebookStatusGet())
                        {
                            case VCR_USER_PHONEBOOK_STATUS_AVAILABLE:
                            {   /* now process the result values */
                                if (SUCCESS == _VCR_sFProcResultsUnique(&result_unique ))
                                {
                                    if ((BT_TRUE == result_unique )  || (_VCR_gData.PTT_contactNr != 0))
                                    {   /* get item id from first response */
                                        //RT#4940
                                        if (_VCR_gData.PTT_contactNr != 0)
                                        {
                                            _VCR_gData.PTT_contactNr = 0;

                                            _VCR_gData.resultInfo.contact_uni.type = VCR_RESULT_TYPE_CONTACT_UNI;
                                                _VCR_gData.resultInfo.contact_uni.item_type  = AT_SIVR_ITEMTYPE_CONTACT;

                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"Bugfix 4940 VCR callname %d",
                                                   _VCR_gData.resultInfo.contact_uni.item_id);

                                        }
                                        else
                                        {
                                           if (AT_SIVR_RES_IDs == _VCR_gData.stMsgRes.stResults[0].u.id.type)
                                           {
                                               _VCR_gData.resultInfo.contact_uni.type = VCR_RESULT_TYPE_CONTACT_UNI;
                                               _VCR_gData.resultInfo.contact_uni.confidence_rate =
                                                   _VCR_gData.stMsgRes.stResults[0].confidence_rate;
                                               if (AT_TRUE == _VCR_gData.stMsgRes.stResults[0].u.id.id1_available)
                                               {   /* id 1 is the item id -> fill into data */
                                                   _VCR_gData.resultInfo.contact_uni.item_id = _VCR_gData.stMsgRes.stResults[0].u.id.id1;
                                                   _VCR_gData.resultInfo.contact_uni.item_type  = AT_SIVR_ITEMTYPE_CONTACT;
                                                   /* check if optional location is available in id2 */
                                                   if (AT_TRUE == _VCR_gData.stMsgRes.stResults[0].u.id.id2_available &&
                                                       0x00    != _VCR_gData.stMsgRes.stResults[0].u.id.id2)
                                                   {
                                                       _VCR_gData.resultInfo.contact_uni.location_id = _VCR_gData.stMsgRes.stResults[0].u.id.id2;
                                                       _VCR_gData.resultInfo.contact_uni.location_avail = BT_TRUE;
                                                   }
                                                   else
                                                   {
                                                       _VCR_gData.resultInfo.contact_uni.location_avail = BT_FALSE;
                                                   }
                                               }
                                               DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCR callname %d",
                                                       _VCR_gData.resultInfo.contact_uni.item_id);
                                           }
                                        }

                                        /* fetch the details for the received PB entry */
                                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                                    }
                                    else
                                    {   /* we have ambiguous results!!! - check for further processing */
                                        /* fill data into result storage */
                                        _VCR_gData.resultInfo.contact_amb.type      =
                                            VCR_RESULT_TYPE_CONTACT_AMB;
                                        _VCR_gData.resultInfo.contact_amb.item_type =
                                            AT_SIVR_ITEMTYPE_CONTACT;
                                        if (AT_TRUE == _VCR_gData.stMsgRes.stResults[0].u.id.id2_available  &&
                                            0x00    != _VCR_gData.stMsgRes.stResults[0].u.id.id2)
                                        {
                                            _VCR_gData.resultInfo.contact_amb.location_avail = BT_TRUE;
                                        }
                                        if (BT_TRUE == _VCR_gData.resultInfo.contact_amb.location_avail)
                                        {
                                            _VCR_gData.resultInfo.contact_amb.location_id =
                                                _VCR_gData.stMsgRes.stResults[0].u.id.id2;
                                        }
                                        _VCR_gData.resultInfo.contact_amb.num_ids =
                                            _VCR_gData.stMsgRes.u8NbrResults;
                                        for (i = 0; i < _VCR_gData.resultInfo.contact_amb.num_ids; i++)
                                        {
                                            _VCR_gData.resultInfo.contact_amb.item_id[i] =
                                                _VCR_gData.stMsgRes.stResults[i].u.id.id1;
                                        }
                                        /* check if Icliuster available */
                                        if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
                                        {   /* send the information to iCluster and stop the system */
                                            /* now we copy the information to the ambi result type */

                                            _VCR_vNotifyApplication(VCR_NOTIFY_CONTACT_AMBIGUOUS);
                                            _VCR_gData.u8NumPromptRequired = 2;
                                            _VCR_gData.u8NumPromptSend     = 0;
                                            _VCR_gData.ePromtTable.tab[0] =
                                                AT_SIVR_PRMT_MULTIPLE_NAMES_POSSIBLE_CHOOSE_HAPTICAL;
                                            _VCR_gData.ePromtTable.tab[1] =
                                                AT_SIVR_PRMT_SYSTEM_STOP;
                                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                                        }
                                        else
                                        {
                                            _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES);
                                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                            break;
                                        }
                                    }
                                }
                                else
                                {   /* just retry */
                                    _VCR_gData.u8ProcErr++;
                                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRM;
                                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                                }
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
                    {
                        /* prepare prompt output and send end prompt */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NETWORK_NOT_AVAILABLE;
                        _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
                        _VCR_gData.u8NumPromptRequired = 2;
                        _VCR_gData.u8NumPromptSend     = 0;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                    }
                }
                else
                {   /* prepare prompt output and send end prompt */
                    _VCR_vPhoneNotAvailableStop();
                }
                break;
            }
            case AT_SIVR_CMD_DIAL_DIALNAMECONFIRM:
            case AT_SIVR_CMD_YES_DIALNAMECONFIRM:
            {   /* call the name - first indicate connect prompt */
                if ((BT_TRUE == _VCR_gData.resultInfo.contact_uni.location_avail) && (1 == _VCR_gData.resultInfo.contact_uni.n_cat))
                {
                    //  RT#1380
                   _VCR_vConnectPromptCategory();
                }
                else
                {
                   _VCR_vConnectPrompt();
                }
                break;
            }
            case AT_SIVR_CMD_NO_DIALNAMECONFIRM:
            {   /* go back to main */
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
            {
                _VCR_vFunctionNotAvailableStop();
                break;
            }
        }
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vHandleDialNameConfCatEvent                                    */
/**
    \brief      event handler for event received in DialNameConfCat state

    \brief

    \param      BooleanType * ptr to handled indication to indicate if event hdl
                VCRMenuEventEnum event that is received

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfHandleDialNameConfCatEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type i = 0;
    BooleanType index_found = BT_FALSE;
    BooleanType result_unique = BT_TRUE;
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

                       // Specaenderung auf 2009.12.03
                       /*
                          <I_dialname_confirm_(d_categories[x])>
                          Sagen Sie (d_categories[x=1...x=n-1]) oder (d_categories[n]).
                          soll imer gesprochen werden.


                          anstatt:
                          <T2_dialname_confirm_(d_name)>
                          Mögliche Kommandos sind  ja, nein oder Hilfe.




                          ToDo:
                          - Laenge von 40 auf 33  -> MAXNUMBER = 40
                          - Network not available -> Check faellt heraus ->hatte Herr Kavemann mit Marc P
                          - Phone not available -> Mainmenu
                          -

                       */
#if 0
                        if (BT_FALSE == _VCR_gData.resultInfo.contact_uni.location_avail)
                        {
/*
Long:
Mögliche Kommandos sind  ja, nein oder Hilfe.
*/


                            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME);
                        }
                        else
                        {
/*
Long:
Mögliche Kommandos sind  ja, nein oder Hilfe.

*/

                            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_CAT);
                        }

#endif

                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );

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
            {   /* do not change the state */
                if (AT_SIVR_MENU_UNDEF == _VCR_gData.eNextMenuState)
                {
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                else
                {   /* go to next state */
                    _VCR_vChangeState(_VCR_gData.eNextMenuState);
                    _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                    if(AT_SIVR_MENU_MAIN == _VCR_gData.eMenuState)
                    {
                        _VCR_vChangeMenuToMain();
                    }
                    else
                    {
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                    }
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {   /* dial number and go back to idle */
                _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                sF = SUCCESS;
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
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_DIAL:
            {   /* entry command - check environment */
                if (BT_TRUE == ERM_bPhoneAvailable())
                {   /* query address book status */
                    switch (_VCR_eUserPhonebookStatusGet())
                    {
                        case VCR_USER_PHONEBOOK_STATUS_AVAILABLE:
                        {   /* now start the menu processing */
                            _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                            _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
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
                    _VCR_vPhoneNotAvailableStop();
                }
                break;
            }
            case AT_SIVR_CMD_CATEGORY:
            {   /* process the received category */
                if (SUCCESS == _VCR_sFProcResultsUnique(&result_unique ) )
                {   /* update the location id from first result if available */
                    if (AT_TRUE == _VCR_gData.stMsgRes.stResults[0].u.id.id1_available)
                    {
                        _VCR_gData.resultInfo.contact_uni.location_avail = BT_TRUE;
                        _VCR_gData.resultInfo.contact_uni.location_id =
                            _VCR_gData.stMsgRes.stResults[0].u.id.id1;
                        /* check if this category is available and how often */
                        _VCR_gData.resultInfo.contact_uni.n_cat = 0;
                        for (i=0;i<_VCR_gData.resultInfo.contact_uni.n;i++)
                        {   /* search through all given phone numbers */
                            if (_VCR_gData.resultInfo.contact_uni.location_id ==
                                _VCR_gData.resultInfo.contact_uni.n_location[i])
                            {
                                _VCR_gData.resultInfo.contact_uni.n_cat =
                                    _VCR_gData.resultInfo.contact_uni.n_cat + 1;
                            }
                        }
                        /* now do processing according to n_cat */
                        if (0 == _VCR_gData.resultInfo.contact_uni.n_cat)
                        {   /* category not avilable - retry */
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE);
                        }
                        else if (1 == _VCR_gData.resultInfo.contact_uni.n_cat)
                        {   /* call the number with respective category index, first identify
                               the index to be used and store in current_num */
                            index_found = BT_FALSE;
                            for (i = 0; i < _VCR_gData.resultInfo.contact_uni.n;i++)
                            {
                                if (_VCR_gData.resultInfo.contact_uni.location_id ==
                                    _VCR_gData.resultInfo.contact_uni.n_location[i])
                                {   /* match found - call with this index  */
                                    index_found = BT_TRUE;
                                    _VCR_gData.resultInfo.contact_uni.current_num = i;
                                    break;
                                }
                            }
                            if (BT_TRUE == index_found)
                            {
                                if ((BT_TRUE == _VCR_gData.resultInfo.contact_uni.location_avail) && (1 == _VCR_gData.resultInfo.contact_uni.n_cat))
                                {
                                    //  RT#1380
                                   _VCR_vConnectPromptCategory();
                                }
                                else
                                {
                                   _VCR_vConnectPrompt();
                                }
                            }
                            else
                            {   /* use index 0*/
                                _VCR_gData.resultInfo.contact_uni.current_num = 0;
                            }
                        }
                        else /* for n_cat > 1 */
                        {   /* check if instrument cluster available */
                            if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
                            {   /* send the information to iCluster and stop the system */
                                /* now we copy the information to the ambi result type */
                                _VCR_vNotifyApplication(VCR_NOTIFY_MULTIPLE_NUMBERS);
                                _VCR_gData.u8NumPromptRequired = 2;
                                _VCR_gData.u8NumPromptSend     = 0;
                                _VCR_gData.ePromtTable.tab[0] =
                                    AT_SIVR_PRMT_MULTIPLE_NUMBERS_POSSIBLE_CHOOSE_HAPTICAL;     //RT#5137
                                _VCR_gData.ePromtTable.tab[1] =
                                    AT_SIVR_PRMT_SYSTEM_STOP;
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                            }
                            else
                            {   /* go to resolve multiple numbers */
                                _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                break;
                            }
                        }
                    }
                    else
                    {
                        _VCR_gData.resultInfo.contact_uni.location_avail = BT_FALSE;
                        /* trigger retry here */
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                    }
                }
                else
                {   /* just retry */
                    _VCR_gData.u8ProcErr++;
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
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
/* Function    : _VCR_vHandleDialNameResAmbiEvent                             */
/**
    \brief      event handler for event received in DialNameResAmbi state

    \brief

    \param      BooleanType * ptr to handled indication to indicate if event hdl
                VCRMenuEventEnum event that is received

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfHandleDialNameResAmbiEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    static VCRMenuEventEnum button = VCR_MEVT_UNDEF;
    static BooleanType call_pending = BT_FALSE;
    SuccessFailType sF = SUCCESS;
    Uint8Type i = 0;
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;
    BooleanType index_found = BT_FALSE;

    VCRResultContactAmbType tmp_ambi;
    (void)memset(&tmp_ambi,0x00,sizeof(VCRResultContactAmbType));

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
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_RESOLVE_AMBIGUOUS_NAMES_OPTIONS);
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
            case VCR_MEVT_DELAY_LIST_READ:
            {
                /*RT#4625 List of proposals when the name is not recognized*/
                /* check if PTT ok OK or CALL was already pressed,
                   if so do NOT increment but retrigger */
                if (_VCR_gData.bPTTunhandled == BT_TRUE)
                {
                    _VCR_gData.bPTTunhandled = BT_FALSE;
                    if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY))
                    {
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
                    if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY))
                    {
                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"unhandled HOOK is handled now");
                    }
                    else
                    {
                       DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"unhandled HOOK could not handled");
                    }
                }
                else if (BT_TRUE == VCR_bCheckIsPTTPressed() ||
                    BT_TRUE == VCR_bCheckIsCALLPressed()  )
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0);
                }
                else
                {   /* normal prompt call - increment name idx and go on */
                    /* check if the end was reached - start prep */
                    if (_VCR_gData.resultInfo.contact_amb.current_num >=
                        _VCR_gData.resultInfo.contact_amb.num_ids)
                    {
                        _VCR_gData.stProcMenu.ePromptId =
                                AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                    }
                    else
                    {
                        _VCR_gData.resultInfo.contact_amb.current_num =
                            _VCR_gData.resultInfo.contact_amb.current_num + 1;
                        if (_VCR_gData.resultInfo.contact_amb.current_num >=
                            _VCR_gData.resultInfo.contact_amb.num_ids)
                        {   /* end of list reached - read out end of list prompt and ask if
                               list shall be restarted */
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_END_OF_NAME_LIST_BUTTON_LIST_IS_REPEAT);
                        }
                        else
                        {   /* send next name */
                            _VCR_gData.u8NumPromptRequired = 2;
                            _VCR_gData.u8NumPromptSend = 0;
                            _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_AMBIGUOUS_NAME;
                            _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_D_TEXTTAG;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                        }
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* do not change the state */
                if (AT_SIVR_MENU_UNDEF == _VCR_gData.eNextMenuState)
                {
                    /* handle the prompt if name was read */
                    /* increment counter and check if last element was reached */
                    if (BT_FALSE == call_pending)
                    {
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_DELAY_LIST_READ);
                    }
                    else
                    {   /* connect is pending - call dial */
                        _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_FALSE;
                        _VCR_vConnectPrompt();
                        call_pending = BT_FALSE;
                    }
                }
                else
                {   /* go to next state */
                    _VCR_vChangeState(_VCR_gData.eNextMenuState);
                    _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                    if(AT_SIVR_MENU_MAIN == _VCR_gData.eMenuState)
                    {
                        _VCR_vChangeMenuToMain();
                    }
                    else
                    {
                        _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                    }
                }
                break;
            }
            case VCR_MEVT_PTT:
            {   /* set local variable to store received button event */
                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                     // RT#4092 SDS gong not played after resolving ambiguous name
                    /* system waits for user input, but user presses PTT -> stop the complete dialog */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                }
                else if (_VCR_gData.resultInfo.contact_amb.current_num >= _VCR_gData.resultInfo.contact_amb.num_ids)
                {
                    /* End of List, system reads help prompt => Stop prompt => generic handling */
                    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Ingore PTT at End of List of ambiguous Names");
                    *handled = BT_FALSE;
                }
                else
                {
                   button = event;
                   /* modify ambi info to uni info and fetch further infos for this entry */
                   (void)memcpy(&tmp_ambi,&(_VCR_gData.resultInfo.contact_amb), sizeof(VCRResultContactAmbType));
                   (void)memset(&(_VCR_gData.resultInfo.contact_uni),0x00, sizeof(VCRResultContactUniType));
                   /* copy needed information */
                   _VCR_gData.resultInfo.contact_uni.type           =  VCR_RESULT_TYPE_CONTACT_UNI;
                   _VCR_gData.resultInfo.contact_uni.item_type      =  tmp_ambi.item_type;
                   _VCR_gData.resultInfo.contact_uni.location_avail =  tmp_ambi.location_avail;
                   _VCR_gData.resultInfo.contact_uni.location_id    =  tmp_ambi.location_id;
                   _VCR_gData.resultInfo.contact_uni.item_id        =  tmp_ambi.item_id[tmp_ambi.current_num];
                   _VCR_gData.resultInfo.contact_uni.avail_loc      =  0;
                   (void)memset(&(_VCR_gData.resultInfo.contact_uni.n_location[0]),0x00,VCR_MAXNUM_RES_STORE*sizeof(Uint16Type));
                   _VCR_gData.resultInfo.contact_uni.confidence_rate = 0;
                   _VCR_gData.resultInfo.contact_uni.n = 0;
                   _VCR_gData.resultInfo.contact_uni.c = 0;
                   _VCR_gData.resultInfo.contact_uni.n_cat = 0;
                   /* fetch the details for the received PB entry */
                   if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY))
                   {
                       // OK no Event problem
                   }
                   else
                   {
                       _VCR_gData.bHOOKunhandled = BT_FALSE;
                       _VCR_gData.bPTTunhandled = BT_TRUE;
                       DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"handle PTT later");
                   }
                }
                break;
            }
            case VCR_MEVT_OK:
            case VCR_MEVT_CALL:
            {   /* set local variable to store received button event */
                if (_VCR_gData.resultInfo.contact_amb.current_num >= _VCR_gData.resultInfo.contact_amb.num_ids)
                {
                   /* End of List reading of contacts is over, ignore the button */
                   DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Ingore HOOK at End of List");
                   *handled = BT_FALSE;
                }
                else
                {
                   button = event;
                   /* modify ambi info to uni info and fetch further infos for this entry */
                   (void)memcpy(&tmp_ambi,&(_VCR_gData.resultInfo.contact_amb), sizeof(VCRResultContactAmbType));
                   (void)memset(&(_VCR_gData.resultInfo.contact_uni),0x00, sizeof(VCRResultContactUniType));
                   /* copy needed information */
                   _VCR_gData.resultInfo.contact_uni.type           =  VCR_RESULT_TYPE_CONTACT_UNI;
                   _VCR_gData.resultInfo.contact_uni.item_type      =  tmp_ambi.item_type;
                   _VCR_gData.resultInfo.contact_uni.location_avail =  tmp_ambi.location_avail;
                   _VCR_gData.resultInfo.contact_uni.location_id    =  tmp_ambi.location_id;
                   _VCR_gData.resultInfo.contact_uni.item_id        =  tmp_ambi.item_id[tmp_ambi.current_num];
                   _VCR_gData.resultInfo.contact_uni.avail_loc      =  0;
                   (void)memset(&(_VCR_gData.resultInfo.contact_uni.n_location[0]),0x00,VCR_MAXNUM_RES_STORE*sizeof(Uint16Type));
                   _VCR_gData.resultInfo.contact_uni.confidence_rate = 0;
                   _VCR_gData.resultInfo.contact_uni.n = 0;
                   _VCR_gData.resultInfo.contact_uni.c = 0;
                   _VCR_gData.resultInfo.contact_uni.n_cat = 0;
                   /* fetch the details for the received PB entry */
                   if (SUCCESS == _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY))
                   {
                       // OK no Event problem
                   }
                   else
                   {
                       _VCR_gData.bHOOKunhandled = BT_TRUE;
                       _VCR_gData.bPTTunhandled = BT_FALSE;
                       DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"handle HOOK later");
                   }
                }
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
            {   /* we have received the data for the entry - now due to pressed button
                   take decision how to proceed */
                if (VCR_RESULT_TYPE_CONTACT_UNI ==
                    _VCR_gData.resultInfo.contact_uni.type)
                {   /* check amount of available numbers */
                    if (1 < _VCR_gData.resultInfo.contact_uni.n)
                    {   /* multiple numbers - pressed button is don't care */
                        if (1 == _VCR_gData.resultInfo.contact_uni.c)
                        {   /* check if category available or not */
                            if (BT_TRUE == _VCR_gData.resultInfo.contact_uni.location_avail)
                            {   /* check n_cat */
                                if (0 == _VCR_gData.resultInfo.contact_uni.n_cat)
                                {   /* prompt output and change state */
                                    _VCR_gData.u8NumPromptRequired = 2;
                                    _VCR_gData.u8NumPromptSend = 0;
                                    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE;
                                    _VCR_gData.eNextMenuState     = AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY;
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                                }
                                else if ((1 == _VCR_gData.resultInfo.contact_uni.n_cat) &&
                                         (VCR_MEVT_CALL == button))
                                {   /* directly call with category */
                                    index_found = BT_FALSE;
                                    for (i = 0; i < _VCR_gData.resultInfo.contact_uni.n;i++)
                                    {
                                        if (_VCR_gData.resultInfo.contact_uni.location_id ==
                                            _VCR_gData.resultInfo.contact_uni.n_location[i])
                                        {   /* match found - call with this index  */
                                            index_found = BT_TRUE;
                                            _VCR_gData.resultInfo.contact_uni.current_num = i;
                                            break;
                                        }
                                    }
                                    if (BT_TRUE == index_found)
                                    {
                                        _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                                        _VCR_vConnectPrompt();
                                    }
                                    else
                                    {   /* use index 0*/
                                        _VCR_gData.resultInfo.contact_uni.current_num = 0;
                                    }
                                }
                                else
                                {   /* goto resolve multiple numbers per cat */
                                    _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                }
                            }
                            else
                            {   /* directly goto resolve multiple numbers per cat */
                                _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                            }
                        }
                        else
                        {
                            if (BT_TRUE == _VCR_gData.resultInfo.contact_uni.location_avail)
                            {
                                if (0 == _VCR_gData.resultInfo.contact_uni.n_cat)
                                {   /* prompt output and change state */
                                    _VCR_gData.u8NumPromptRequired = 2;
                                    _VCR_gData.u8NumPromptSend = 0;
                                    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_CHOOSE_OTHER_CATEGORY_INSTEAD;
                                    _VCR_gData.eNextMenuState     = AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES;
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                                }
                                else
                                {   /* goto resolve multiple numbers per cat */
                                    _VCR_vChangeState(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                }

                            }
                            else
                            {   /* directly goto resolve multiple numbers per cat */
                                _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES);
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                            }
                        }
                    }
                    else if (1 == _VCR_gData.resultInfo.contact_uni.n)
                    {   /* only one number, check button */
                        if (VCR_MEVT_CALL == button)
                        {   /* check if location available */
                            if (BT_TRUE == _VCR_gData.resultInfo.contact_uni.location_avail)
                            {
                                if (1 == _VCR_gData.resultInfo.contact_uni.n_cat)
                                {   /* directly call with category */
                                    index_found = BT_FALSE;
                                    for (i = 0; i < _VCR_gData.resultInfo.contact_uni.n;i++)
                                    {
                                        if (_VCR_gData.resultInfo.contact_uni.location_id ==
                                            _VCR_gData.resultInfo.contact_uni.n_location[i])
                                        {   /* match found - call with this index  */
                                            index_found = BT_TRUE;
                                            _VCR_gData.resultInfo.contact_uni.current_num = i;
                                            break;
                                        }
                                    }
                                    if (BT_TRUE == index_found)
                                    {
                                        _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                                        _VCR_vConnectPrompt();
                                    }
                                    else
                                    {   /* use index 0*/
                                        _VCR_gData.resultInfo.contact_uni.current_num = 0;
                                    }
                                }
                                else
                                {   /* give out prompt - and remind that connect is pending */
                                    _VCR_gData.u8NumPromptRequired = 2;
                                    _VCR_gData.u8NumPromptSend = 0;
                                    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE;
                                    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE;
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                                    call_pending = BT_TRUE;
                                }
                            }
                            else
                            {
                                /* call dial and back to idle */
                                _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_FALSE;
                                _VCR_vConnectPrompt();
                            }
                        }
                        else
                        {   /* this was PTT or OK - check if category available */
                            if (BT_TRUE == _VCR_gData.resultInfo.contact_uni.location_avail)
                            {   /* check n_cat */
                                if (0 == _VCR_gData.resultInfo.contact_uni.n_cat)
                                {
                                    /*  RT#4940 go directly to DialNameConfirm */
                                    _VCR_gData.PTT_contactNr = 1;

                                    _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRM);
                                    _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                                }
                                else
                                {   /* goto DialNameConfirm */
                                    _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRM);
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                                }

                            }
                            else
                            {   /* goto DialNameConfirm */
                                _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRM);
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                            }
                        }
                    }
                    else /* no number -> strange -> stop */
                    {
                        _VCR_PlayError(__LINE__ );
                    }
                }
                else
                {   /* strange error here -> stop machine!!! */
                    _VCR_PlayError(__LINE__ );
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
                sF = SUCCESS;
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
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */
            /* entry command */
            case AT_SIVR_CMD_DIAL:
            {   /* reset button */
                button = VCR_MEVT_UNDEF;
                call_pending = BT_FALSE;
                /* check for correct type */
                if (VCR_RESULT_TYPE_CONTACT_AMB == _VCR_gData.resultInfo.contact_amb.type)
                {   /* set current run counter to 0 */
                    _VCR_gData.resultInfo.contact_amb.current_num = 0;

                    /*RT-13210 : Sorting of ambiguous names in alphabetical order */
                    _VCR_vSortingOfAmbiNames(_VCR_gData.resultInfo.contact_amb.item_id,_VCR_gData.resultInfo.contact_amb.num_ids);

                    /* we need to give out complete list first via prompts */
                    /* only for last in list we need to start reco so that user can decide how
                     * to proceed!!! */
                    _VCR_gData.u8NumPromptRequired = 3;
                    _VCR_gData.u8NumPromptSend     = 0;
                    if (_VCR_gData.bVoiceFBOff)
                    {
                       // short dialog
                       _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES;   //RT#4071
                    }
                    else
                    {
                       _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES_L;
                    }
                    _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_AMBIGUOUS_NAME;
                    _VCR_gData.ePromtTable.tab[2]  = AT_SIVR_PRMT_D_TEXTTAG;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                }
                else
                {   /* something failed - stop system */
                    _VCR_gData.u8ProcErr = 0;
                    _VCR_PlayError(__LINE__ );
                }
                break;
            }
            case AT_SIVR_CMD_YES_RESOLVEAMBINAME:
            case AT_SIVR_CMD_REPEAT_NAMES:
            {   /* set name counter back to 0 and read names again */
                _VCR_gData.u8ProcErr                          = 0;
                _VCR_gData.resultInfo.contact_amb.current_num = 0;
                /* we need to give out complete list first via prompts */
                /* only for last in list we need to start reco so that user can decide how
                 * to proceed!!! */
                _VCR_gData.resultInfo.contact_amb.current_num = 0;
                _VCR_gData.u8NumPromptRequired = 2;
                _VCR_gData.u8NumPromptSend = 0;
                _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_AMBIGUOUS_NAME;
                _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_D_TEXTTAG;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                break;
            }
            case AT_SIVR_CMD_NO_RESOLVEAMBINAME:
            {   /* only go back to main menu without any prompt */
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
/* Function    : _VCR_vHandleDialNumberEvent                                    */
/**
    \brief      event handler for event received in DialNumber state

    \brief

    \param      BooleanType * ptr to handled indication to indicate if event hdl
                VCRMenuEventEnum event that is received

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfHandleDialNumberEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    BooleanType result_unique = BT_TRUE;
    *handled = BT_TRUE;

    if (is_event)
    {
        switch (event)
        {
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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_NUMBER_PLEASE);
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
            {
                if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                {
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBER;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                else
                {
                    _VCR_vSinglePrepare(AT_SIVR_PRMT_I_NEW_CONTACT_NUMBER_PLEASE);
                }

                break;
            }
            case VCR_MEVT_DIAL_OK: /* go back to idle via generic handling */
            {
                sF = SUCCESS;
                break;
            }
            case VCR_MEVT_ADDCONTACT_NUMBER:
            {
                if (BT_TRUE == ERM_bPhoneAvailable())
                {   /* process received values*/
                    if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                    {
                        _VCR_PlayError(__LINE__);
                    }
                    else
                    {   /* number is for new contact */
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_I_NEW_CONTACT_NUMBER_PLEASE);
                    }
                }
                else
                {   /* prepare prompt output and send end prompt */
                    _VCR_vPhoneNotAvailableStop();
                }
                break;
            }
            default:
            {
                sF = FAIL;
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {   /* entry command from main or add contact */
            case AT_SIVR_CMD_DIAL_NUMBER:
            {
                if (SUCCESS == _VCR_sFProcResultsUnique(&result_unique ) )
                {   /* change to DIAL NUMBER state */
                    if (BT_TRUE == ERM_bPhoneAvailable())
                    {   /* process received values*/
                        if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                        {
                            if (BT_TRUE == _VCR_gData.stMsgRes.number_valid)
                            {   /* copy the number to resultInfo */
                                _VCR_gData.resultInfo.number.type = VCR_RESULT_TYPE_NUMBER;
                                _VCR_gData.resultInfo.number.number_len =
                                    _VCR_gData.stMsgRes.number_len;
                                (void)memcpy(_VCR_gData.resultInfo.number.number,
                                        _VCR_gData.stMsgRes.number,
                                        _VCR_gData.resultInfo.number.number_len);
                                DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCR res number_len %d",
                                       _VCR_gData.resultInfo.number.number_len);

                                if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                                {
                                    _VCR_vNotifyApplication(VCR_NOTIFY_DIAL_NUMBER_CHANGED);
                                }
                                /* directly go to next state */
                                _VCR_vChangeState(AT_SIVR_MENU_DIALNUMBERCORRECT);
                                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIALNUM_D_BLOCK );
                            }
                            else
                            {   /* no yet number received - ask for number */
                                /* start to fetch number */
                                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBER;
                                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                            }
                        }
                        else
                        {   /* number is for new contact */
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_I_NEW_CONTACT_NUMBER_PLEASE);
                        }
                    }
                    else
                    {   /* prepare prompt output and send end prompt */
                        _VCR_vPhoneNotAvailableStop();
                    }
                }
                else
                {   /* error handling - just retry */
                    _VCR_gData.u8ProcErr++;
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBER;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                break;
            }
            case AT_SIVR_CMD_DIALNUM_D_BLOCK:
            {
                _VCR_gData.eMenuState = AT_SIVR_MENU_DIALNUMBERCORRECT;
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIALNUM_D_BLOCK );
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
/* Function    : _VCR_eHandleDialNumberCorrectEvent                                    */
/**
    \brief      event handler for event received in DialNumberCorrect state

    \brief

    \param      BooleanType * ptr to handled indication to indicate if event hdl
                VCRMenuEventEnum event that is received

    \return     none
   */
/*----------------------------------------------------------------------------*/
Uint8Type _VCR_u8GetSumLengthDBlocks (void)
{
    Uint8Type i;
    Uint8Type SumLength=0;

    for (i=0; i<AT_MAX_PHONE_NUM_LEN; i++)
    {
       SumLength+=DBlock_Array[i];
    }
    return(SumLength);
}

void _VCR_vDebugDBlocks (void)
{
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index0_3:   %d %d %d %d",  DBlock_Array[0],DBlock_Array[1],DBlock_Array[2],DBlock_Array[3] );

    if (DBlock_Index > 3)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index4_7:   %d %d %d %d",  DBlock_Array[4],DBlock_Array[5],DBlock_Array[6],DBlock_Array[7] );
    }
    if (DBlock_Index > 7)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index8_11:  %d %d %d %d",  DBlock_Array[8],DBlock_Array[9],DBlock_Array[10],DBlock_Array[11] );
    }
    if (DBlock_Index > 11)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index12_15: %d %d %d %d",  DBlock_Array[12],DBlock_Array[13],DBlock_Array[14],DBlock_Array[15] );
    }
    if (DBlock_Index > 15)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index16_19: %d %d %d %d",  DBlock_Array[16],DBlock_Array[17],DBlock_Array[18],DBlock_Array[19] );
    }
    if (DBlock_Index > 19)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index20_23: %d %d %d %d",  DBlock_Array[20],DBlock_Array[21],DBlock_Array[22],DBlock_Array[23] );
    }
    if (DBlock_Index > 23)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index24_27: %d %d %d %d",  DBlock_Array[24],DBlock_Array[25],DBlock_Array[26],DBlock_Array[27] );
    }
    if (DBlock_Index > 27)
    {
        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"DBlock_Index28_31: %d %d %d %d",  DBlock_Array[28],DBlock_Array[29],DBlock_Array[30],DBlock_Array[31] );
    }
}


SuccessFailType _VCR_sfHandleDialNumberCorrectEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;
    static BooleanType is_eprom_error = BT_FALSE;


    BooleanType result_unique = BT_TRUE;
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
                    {   /* send first prompt only and aftewr prompt stop trigger restart */
                        /* set back error counter */
                        if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                        {
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_T2_DIALNUMBERCORRECT);
                        }
                        else
                        {
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_T2_DIALNUMBERCORRECTSAVE);
                        }
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
            case VCR_MEVT_PBENTRY_DATA_AVAIL:
            {   /* the PB entry was added and we know the index - link via RVTA */

                 if (AT_SIVR_ENG_BEGIN_SYNC != _VCR_gData.stCtrlInfo.eEngineStatus)
                 {
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_ADD);
                 }
                 else
                 {
                    // Voicetagrecording, if SIVR synchro is in progress - try again (RT1344 and RT1351 should work if 250 RC8 is used)
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_PBENTRY_DATA_AVAIL);
                 }
                break;
            }
            case VCR_MEVT_VT_ADD_DONE:
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
            case VCR_MEVT_APP_TRIGGER:
            {
                /* we need to give out complete list first via prompts, therefore
                 * we need to fetch the phone book information */
                appRsp = APP_PHONEBOOK_rcRequestAddPbEntry(
                        VCR_vCBAddContact,
                        (const Uint8Type *)&(_VCR_gData.resultInfo.number.number),
                        _VCR_gData.resultInfo.number.number_len,
                        (const Uint8Type *)&(_VCR_gData.resultInfo.number.number),
                        _VCR_gData.resultInfo.number.number_len,
                        AT_PNT_GENERAL);
                switch (appRsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
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
            case VCR_MEVT_SIVR_STARTED:
            {
                if (BT_TRUE == _VCR_gData.resultInfo.number.stop_for_contact)
                {
                    if (BT_TRUE == is_eprom_error)
                    {
                        is_eprom_error = BT_FALSE;
                        _VCR_PlayError(__LINE__);
                    }
                    else
                    {
                        _VCR_gData.resultInfo.number.stop_for_contact = BT_FALSE;
                        _VCR_gData.eNextMenuState = AT_SIVR_MENU_MAIN;
                        _VCR_vSinglePrompt(AT_SIVR_PRMT_CONTACT_SUCCESSFULLY_ENTERED);
                    }

                }
                else
                {   /* this was due to PTT restart - handle via generic */
                    *handled = BT_FALSE;
                }
                break;
            }
            case VCR_MEVT_SIVR_STOPPED:
            {
                if (BT_TRUE == _VCR_gData.resultInfo.number.stop_for_contact)
                {   /* add entry */
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIGGER );
                }
                else
                {   /* this was due to PTT restart - handle via generic */
                    *handled = BT_FALSE;
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* do not change the state */
                if (BT_TRUE == _VCR_gData.resultInfo.number.stop_for_contact)
                {   /* we need to stop the system to create the new contact entry */
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                }
                else
                {
                    if (AT_SIVR_MENU_UNDEF == _VCR_gData.eNextMenuState)
                    {
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                    }
                    else
                    {   /* go to next state */
                        _VCR_vChangeState(_VCR_gData.eNextMenuState);
                        _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                        if(AT_SIVR_MENU_MAIN == _VCR_gData.eMenuState)
                        {
                            _VCR_vChangeMenuToMain();
                        }
                        else
                        {
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL_NUMBER );
                        }
                    }
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
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        switch (cmd)
        {   /* we wait for end of start prompt, if received we trigger processing */
            /* possible result command codes from processing state machine */

            /* entry command from DIALNUMBER */
            case AT_SIVR_CMD_DIALNUM_D_BLOCK:
            {   /* check that we received at least one valid command */
                if (SUCCESS == _VCR_sFProcResultsUnique(&result_unique ) )
                {   /* process the received values from response 0 */
                    if (BT_TRUE == _VCR_gData.stMsgRes.number_valid)
                    {   /* copy the number to resultInfo */
                        _VCR_gData.resultInfo.number.type = VCR_RESULT_TYPE_NUMBER;
                        _VCR_gData.resultInfo.number.number_len = _VCR_gData.stMsgRes.number_len;

                        // Init Array and Index
                        (void)memset(&DBlock_Array[0],0,sizeof(DBlock_Array));
                        DBlock_Index = 0;
                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Init DBlock_Array again");
                        _VCR_gData.resultInfo.number.number_startRead = 0;


                        // Init with first recognized length
                        DBlock_Array[DBlock_Index] = _VCR_gData.stMsgRes.number_len;


                        (void)memcpy(_VCR_gData.resultInfo.number.number,_VCR_gData.stMsgRes.number, _VCR_gData.resultInfo.number.number_len);

                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"DIALNUM_D_BLOCK number_len=%d result_len=%d -> Init again",
                                       _VCR_gData.resultInfo.number.number_len,
                                       _VCR_gData.stMsgRes.number_len);

                        if (0 < _VCR_gData.resultInfo.number.number_len &&
                            AT_MAX_PHONE_NUM_LEN >= _VCR_gData.resultInfo.number.number_len )
                        {
                            _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                            _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                            if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                            {
                                _VCR_vNotifyApplication(VCR_NOTIFY_DIAL_NUMBER_CHANGED);
                            }
                        }
                        else
                        {   /* give out prompt and stay in this menu */
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NUMBER_TOO_LONG;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_NUMBER_DELETED;
                            _VCR_gData.u8NumPromptRequired = 2;
                            _VCR_gData.u8NumPromptSend     = 0;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                        }
                    }
                    else
                    {   /* error handling - just retry */
                        _VCR_gData.u8ProcErr++;
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                    }
                }
                else
                {   /* error handling - just retry */
                    _VCR_gData.u8ProcErr++;
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                break;
            }
            case AT_SIVR_CMD_DIAL_DIALNUMCORRECT:
            {   /* trigger dial here and if dialled go back to idle */
#if (1 == VCR_CHECK_NETWORK_AVAILABILITY)
                if( 0 != APP_PHONE_GetNetworkAvailable())
#else
                if(BT_TRUE)
#endif
                {
                     // Network avail
                     if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                     {
                         _VCR_vConnectPrompt();
                     }
                     else
                     {
                         _VCR_vFunctionNotAvailableStop();
                     }
                }
                else
                {
                    /* prepare prompt output and send end prompt */
                    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NETWORK_NOT_AVAILABLE;
                    _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
                    _VCR_gData.u8NumPromptRequired = 2;
                    _VCR_gData.u8NumPromptSend     = 0;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                }
                break;
            }
            case AT_SIVR_CMD_SAVE_DIALNUMCORRECT:
            {
                /* if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                {
                    _VCR_vFunctionNotAvailableStop();
                }
                else
                {    save contact number and assign voice tag to it
                    _VCR_gData.resultInfo.number.stop_for_contact = BT_TRUE;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                } */
                _VCR_vT1SorryRepeat();
                break;
            }
            case AT_SIVR_CMD_NUMCORRECT_D_BLOCK:
            {   /* add the additional digit(s) */
                /* check if we can add the digit due to length, if so add digit,
                 * otherwise indicate digit will result in ntl - deleted digit and
                 * continue menu  */
                if (SUCCESS == _VCR_sFProcResultsUnique(&result_unique ) )
                {
                    if ( AT_MAX_PHONE_NUM_LEN >= (_VCR_gData.resultInfo.number.number_len+
                            _VCR_gData.stMsgRes.number_len))
                    {   /* concat the numbers and restart the menu */
                        (void)memcpy(&(_VCR_gData.resultInfo.number.number[_VCR_gData.resultInfo.number.number_len]),
                                _VCR_gData.stMsgRes.number,
                                _VCR_gData.stMsgRes.number_len);


                        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"NUMCORRECT_D_BLOCK number_len=%d result_len=%d new_number_len=%d",
                                       _VCR_gData.resultInfo.number.number_len,
                                       _VCR_gData.stMsgRes.number_len,
                                       (_VCR_gData.resultInfo.number.number_len + _VCR_gData.stMsgRes.number_len),
                                       0);


                        // Index increment and store new length of DBLock

                        _VCR_gData.resultInfo.number.number_startRead =  _VCR_u8GetSumLengthDBlocks();

                        DBlock_Index ++;
                        DBlock_Array[DBlock_Index] = _VCR_gData.stMsgRes.number_len;


                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"DBlock_Index is INC to %d  SumLenth  %d",DBlock_Index, _VCR_u8GetSumLengthDBlocks());


                        _VCR_vDebugDBlocks();


                        // new length
                        _VCR_gData.resultInfo.number.number_len = _VCR_gData.resultInfo.number.number_len + _VCR_gData.stMsgRes.number_len;

                        if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                        {
                            _VCR_vNotifyApplication(VCR_NOTIFY_DIAL_NUMBER_CHANGED);
                        }
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                    }
                    else
                    {
                        _VCR_gData.u8NumPromptRequired = 2;
                        _VCR_gData.u8NumPromptSend = 0;
                        _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_NUMBER_TOO_LONG;
                        _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_NUMBER_DELETED;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
                    }
                }
                else
                {   /* error handling - just retry */
                    _VCR_gData.u8ProcErr++;
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                break;
            }
            case AT_SIVR_CMD_CORRECTION:
            {   /* check length and if <= 1 clear completely and go back to dial,
                   otherwise only clear last digit and retry */

                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"DBlock_Index is %d SumLenth  %d",DBlock_Index, _VCR_u8GetSumLengthDBlocks());

                _VCR_vDebugDBlocks();

                if (DBlock_Index == 0)
                {
                    // new end of number
                    _VCR_gData.resultInfo.number.number[0] = '\0';
                    _VCR_gData.resultInfo.number.number_len = 0;

                    //
                    DBlock_Index = 0;

                    // new start of number
                    _VCR_gData.resultInfo.number.number_startRead = 0;
                    (void)memset(&DBlock_Array[0],0,sizeof(DBlock_Array));



                    // Trigger SD
                    if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                    {
                         _VCR_vNotifyApplication(VCR_NOTIFY_DIAL_NUMBER_DELETE);
                    }

                    // State Change
                    (void)_VCR_sfInitResultValues();
                    _VCR_vChangeState(AT_SIVR_MENU_DIALNUMBER);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL_NUMBER );

                }
                else if (DBlock_Index >= 1)
                {
                   // new end of number
                   _VCR_gData.resultInfo.number.number[_VCR_gData.resultInfo.number.number_len-DBlock_Array[DBlock_Index]] = '\0';
                   _VCR_gData.resultInfo.number.number_len = _VCR_gData.resultInfo.number.number_len - DBlock_Array[DBlock_Index];

                   // delete last DBlock
                   DBlock_Array[DBlock_Index] = 0;
                   DBlock_Index --;

                   // new start of number
                   _VCR_gData.resultInfo.number.number_startRead = (_VCR_u8GetSumLengthDBlocks() - DBlock_Array[DBlock_Index]);

                   // Trigger SD
                   if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                   {
                       _VCR_vNotifyApplication(VCR_NOTIFY_DIAL_NUMBER_CHANGED);
                   }


                   // Trigger VR:  directly next prep
                   _VCR_gData.u8ProcErr = 0;
                   _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                   _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                   (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );

                }
                break;
            }
            case AT_SIVR_CMD_DELETE_DIALNUMCORRECT:
            {   /* clear complete number and go back to Dial Number */
                _VCR_gData.eNextMenuState = AT_SIVR_MENU_DIALNUMBER;

                /*
                    RT#1350
                    don't Init the following values in case of adding a contact, because we need still the data in the next turn
                */
                if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                {
                    (void)_VCR_sfInitResultValues();
                }


                if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                {
                    _VCR_vNotifyApplication(VCR_NOTIFY_DIAL_NUMBER_DELETE);
                }

                _VCR_vSinglePrompt(AT_SIVR_PRMT_NUMBER_DELETED);
                break;
            }
            case AT_SIVR_CMD_REPEAT_DIALNUMCORRECT:
            {   /* give out complete number and stay in this state */

// RT#4005 Dial number is repeated twice
//                _VCR_vSinglePrompt(AT_SIVR_PRMT_X_NUMBER);

               /* GOTO: DialNumberCorrect */

//RT#6449 VRS-dial number -"repeat" doesnt repeat all digits
               _VCR_gData.resultInfo.number.number_startRead = 0;


                _VCR_gData.u8ProcErr = 0;
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_DIALNUMBERCORRECT;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );

                break;
            }
            case AT_SIVR_CMD_HELP:
            {
                if (BT_FALSE == _VCR_gData.resultInfo.number.is_for_contact)
                {
                     //RT#7897  VR dial_number_help_one_prompt_is_missing
                     // add a special handling for this case, beause normally after the Help prompt we get directly the Beep
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_HELP_DIAL_NAME_CONFIRM);
                }
                else
                {
                    _VCR_vSinglePrepare(AT_SIVR_PRMT_H_DIALNUMBERCORRECTSAVE);
                }

                break;
            }
            case AT_SIVR_CMD_CANCEL:
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
/* Function    : _VCR_sfHandleResolveMultipleNumCatEvent                      */
/**
    \brief      event handler for event received in resolve multiple numbers
                per category

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
SuccessFailType _VCR_sfHandleResolveMultipleNumCatEvent(
        BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    static BooleanType is_first_try = BT_FALSE;
    BooleanType match_found = BT_FALSE;
    Uint8Type i = 0;
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
                    {   /* send first prompt only and aftewr prompt stop trigger restart */
                        /* set back error counter */
                        //RT#3520
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_RESOLVE_MULTIPLE_NUMBERS);
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
            case VCR_MEVT_DELAY_LIST_READ:
            {
                if (BT_TRUE == _VCR_gData.bPTTunhandled)
                {
                    _VCR_gData.bPTTunhandled = BT_FALSE;
                    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"unhandled PTT is handled now");

                   _VCR_vConnectPrompt();
                   _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                }
                else if (BT_TRUE == VCR_bCheckIsPTTPressed() ||
                    BT_TRUE == VCR_bCheckIsCALLPressed()  )
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0);
                }
                else
                {
                    /* do not change the state */
                    if (AT_SIVR_MENU_UNDEF == _VCR_gData.eNextMenuState)
                    {   /* handle the prompt if number was read */
                        if (BT_TRUE == is_first_try)
                        {   /* we do not need to increment as only menu headling was prompted */
                            is_first_try = BT_FALSE;
                            _VCR_gData.resultInfo.contact_uni.current_num = 0;
                        }
                        else
                        {
                            _VCR_gData.resultInfo.contact_uni.current_num = _VCR_gData.resultInfo.contact_uni.current_num + 1;
                        }
                        /* increment counter and check if last element was reached */
                        for (i=_VCR_gData.resultInfo.contact_uni.current_num;i<_VCR_gData.resultInfo.contact_uni.n;i++)
                        {
                            if (_VCR_gData.resultInfo.contact_uni.location_id == _VCR_gData.resultInfo.contact_uni.n_location[i])
                            {
                                _VCR_gData.resultInfo.contact_uni.current_num = i;
                                match_found = BT_TRUE;
                                break;
                            }
                        }
                        if (BT_TRUE == match_found)
                        {   /* query phone book information */
                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                        }
                        else
                        {   /* we reached the end of the list!!! */
                            //RT#3520
                            // "end_of_name_list_please_press_button_while_list_is_read_repeat" was spoken instead of "nd_of_numbers_list_please_press_button_while_list_is_read_repeat"
                            _VCR_vSinglePrepare(AT_SIVR_PRMT_EON_LIST_PRESS_BUTTON_IS_READ_REPEAT);
                        }
                    }
                    else
                    {   /* go to next state */
                        _VCR_vChangeState(_VCR_gData.eNextMenuState);
                        _VCR_gData.eNextMenuState = AT_SIVR_MENU_UNDEF;
                        if(AT_SIVR_MENU_MAIN == _VCR_gData.eMenuState)
                        {
                            _VCR_vChangeMenuToMain();
                        }
                        else
                        {
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                        }
                    }
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {
                _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_DELAY_LIST_READ);
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
            {   /* we have data for the first entry - trigger prompt output */
                if (BT_TRUE == is_first_try)
                {   /* we need to trigger reco without pre to get the menu output */
                    _VCR_gData.u8NumPromptRequired = 1;
                    _VCR_gData.u8NumPromptSend = 0;
                    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT_MENU );

                }
                else
                {

                    _VCR_vSinglePrompt(AT_SIVR_PRMT_X_NUMBER);
                    _VCR_vSinglePrepare(AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY);
                }

                break;
            }
            case VCR_MEVT_STR_PRMT_END_PTT:
            {
                _VCR_vConnectPrompt();
                _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                _VCR_gData.bPTTIgnore = BT_TRUE;
                break;
            }
            case VCR_MEVT_PTT:
            case VCR_MEVT_CALL:
            case VCR_MEVT_OK:
            {   /* dial with current index                */
                /* index is already stored in current_num */
                if ((AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus) && (VCR_MEVT_PTT == event))
                {
                    /* system waits for user input, but user presses PTT -> stop the complete dialog */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                    /* send engine stop */
                }
                else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    if (BT_TRUE == _VCR_gData.bPTTIgnore)
                    {
                       //RT#4882 Multiple number - prompt iteration
                       *handled = BT_FALSE;
                       DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Selection is already done - ignore PTT");
                    }
                    else
                    {
                        if (_VCR_gData.stProcMenu.ePromptId == AT_SIVR_PRMT_T2_OPTIONS_RESOLVE_MULTIPLE_NUMBERS ||
                            _VCR_gData.stProcMenu.ePromptId == AT_SIVR_PRMT_EON_LIST_PRESS_BUTTON_IS_READ_REPEAT   )
                        {

                            (void)_VCR_sFMessageToBeSend(VCR_MSG_RESTART_PROMPT_RECO);
                        }
                        else
                        {
                            /*Current prompt is numb read prompt*/
                           (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_PTT );
                        }


                    }
                }
                else
                {
                    _VCR_vConnectPrompt();
                    _VCR_gData.resultInfo.contact_uni.is_dial_index = BT_TRUE;
                }
                break;
            }
            case VCR_MEVT_CONNECT_PROMTP_END:
            {   /* dial number and go back to idle */
                (void)_VCR_sFMessageToBeSend(VCR_MSG_DIAL );
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
            /* entry command from other menu */
            case AT_SIVR_CMD_DIAL:
            {
                is_first_try = BT_TRUE;
                /* check for correct type */
                if (VCR_RESULT_TYPE_CONTACT_UNI == _VCR_gData.resultInfo.contact_uni.type)
                {   /* set current run counter to first index that match */
                    _VCR_gData.resultInfo.contact_uni.current_num = 0;
                    match_found = BT_FALSE;
                    /* if no location is available so far, means n_cat = 0,
                     * we set location to first location */
                    if (BT_FALSE == _VCR_gData.resultInfo.contact_uni.location_avail)
                    {
                        _VCR_gData.resultInfo.contact_uni.location_avail = BT_TRUE;
                        _VCR_gData.resultInfo.contact_uni.location_id =
                            _VCR_gData.resultInfo.contact_uni.n_location[0];
                        _VCR_gData.resultInfo.contact_uni.n_cat = 0;
                        for (i=0; i< _VCR_gData.resultInfo.contact_uni.n;i++)
                        {
                            if (_VCR_gData.resultInfo.contact_uni.location_id ==
                                _VCR_gData.resultInfo.contact_uni.n_location[i])
                            {
                                _VCR_gData.resultInfo.contact_uni.n_cat =
                                    _VCR_gData.resultInfo.contact_uni.n_cat + 1;
                            }
                        }
                    }
                    for (i=0; i< _VCR_gData.resultInfo.contact_uni.n;i++)
                    {
                        if (_VCR_gData.resultInfo.contact_uni.location_id ==
                            _VCR_gData.resultInfo.contact_uni.n_location[i])
                        {
                            _VCR_gData.resultInfo.contact_uni.current_num = i + 1;
                            match_found = BT_TRUE;
                            break;
                        }
                    }
                    if (BT_TRUE == match_found)
                    {
                        /* we need to give out complete list first via prompts, therefore
                         * we need to fetch the number from phone book */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                    }
                    else
                    {   /* we have no match !!!  - stop system */
                        _VCR_gData.u8ProcErr = 0;
                        _VCR_PlayError(__LINE__ );
                    }

                }
                else
                {   /* something failed - stop system */
                    _VCR_gData.u8ProcErr = 0;
                    _VCR_gData.u8NumPromptRequired = 1;
                    _VCR_gData.u8NumPromptSend = 0;
                    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_SYSTEM_STOP;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                }
                break;
            }
            case AT_SIVR_CMD_YES_RES_MULTIPLENUM:
            case AT_SIVR_CMD_REPEAT_NUMBERS:
            {   /* set name counter back to 0 and read names again */
                _VCR_gData.u8ProcErr                          = 0;
                /* like when first entering this Events reset the is_first_try variable
                 * in order to restart the repeating of names corrects.
                 */
                /* we need to give out complete list first via prompts */
                /* only for last in list we need to start reco so that user can decide how
                 * to proceed!!! */
                _VCR_gData.resultInfo.contact_uni.current_num = 0;
                match_found = BT_FALSE;
                for (i=0; i< _VCR_gData.resultInfo.contact_uni.n;i++)
                {
                    if (_VCR_gData.resultInfo.contact_uni.location_id == _VCR_gData.resultInfo.contact_uni.n_location[i])
                    {
                        // dont't increment index, we have the right number
                        _VCR_gData.resultInfo.contact_uni.current_num = i;
                        match_found = BT_TRUE;
                        break;
                    }
                }
                if (BT_TRUE == match_found)
                {
                    /* we need to give out complete list first via prompts, therefore
                     * we need to fetch the number from phone book */
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                }
                else
                {   /* we have no match !!!  - stop system */
                    _VCR_gData.u8ProcErr = 0;
                    _VCR_PlayError(__LINE__ );
                }
                break;
            }
            case AT_SIVR_CMD_NO_RES_MULTIPLENUM:
            {   /* only go back to main menu without any prompt */
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
/* Function    : _VCR_sfHandleCallbackMissedCallsEvent                        */
/**
    \brief      event handler for event received in call back missed calls

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
SuccessFailType _VCR_sfHandleCallbackMissedCallsEvent(
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
                        _VCR_vSinglePrepare(AT_SIVR_PRMT_T2_OPTIONS_CALLBACKMISSEDCALLS);
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
            case VCR_MEVT_START_PROMTP_END:
            {   /* entry from idle - check how many new calls and
                   if call id transmitted */
                if (1 == _VCR_gData.resultInfo.cStack.num_missed_calls)
                {   /* we need to check if the number is available, read info */
                    _VCR_gData.resultInfo.cStack.cur_entry = 1;
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_APP_TRIG_PB_QUERY);
                }
                else
                {   /* more than one call prompt output */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_MISSED_CALL_SEVERAL);
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
            {   /* check if number is available */
                if (0 < _VCR_gData.resultInfo.cStack.phone_number_len)
                {   /* phone number available!! */
                    _VCR_vSinglePrepare(AT_SIVR_PRMT_MISSED_CALL_FROM);
                }
                else
                {   /* no phone number available */
                    _VCR_vSinglePrompt(AT_SIVR_PRMT_MISSED_CALL_FROM_UNKNOWN);
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* check number to identify how we reached this state */
                if (1 == _VCR_gData.resultInfo.cStack.num_missed_calls)
                {   /* phone number not available - prompt and stop */
                    _VCR_gData.u8NumPromptRequired = 2;
                    _VCR_gData.u8NumPromptSend = 0;
                    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_MISSED_CALL_FROM_UNKNOWN;
                    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_SYSTEM_STOP;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                }
                else
                {   /* goto read call stacks */
                    _VCR_vChangeState(AT_SIVR_MENU_READCALLSTACKS);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CSTACK_MISSED_CALL );
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
            case AT_SIVR_CMD_MISSED_CALL_DIAL:
            case AT_SIVR_CMD_MISSED_CALL_YES:
            {   /* dial */
                _VCR_vConnectPrompt();
                break;
            }
            case AT_SIVR_CMD_MISSED_CALL_NO:
            {   /* back to idle */
                _VCR_gData.u8NumPromptRequired = 1;
                _VCR_gData.u8NumPromptSend = 0;
                _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_SYSTEM_STOP;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
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
/* Function    : _VCR_vSortingOfAmbiNames                        */
/**
    \brief      Sorts the ambiguous names in alphabetical order based on the V-card
                index

    \param      Uint16Type *item_id
                Holds the contact IDs

    \param      Uint8Type Num_of_Results


    \return     void
   */
/*----------------------------------------------------------------------------*/
/* RT-13210 : Ambiguous names sorting*/

static void _VCR_vSortingOfAmbiNames(Uint16Type *item_id, Uint8Type Num_Res)
{
    Uint8Type i;
    Uint8Type j;

    if(Num_Res != 0)
    {
        for (i = 0; i < (Num_Res-1); i++)
        {
            for (j = 0; j < (Num_Res-(i + 1)); j++)
            {
                if(item_id[j] > item_id[j+1])
                {
                    item_id[j]   = item_id[j] + item_id[j+1];
                    item_id[j+1] = item_id[j] - item_id[j+1];
                    item_id[j]   = item_id[j] - item_id[j+1];
                }
            }
         }
    }
}
/* End Of File VCR_StateHandler_PHONE.c */
