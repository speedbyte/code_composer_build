/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_MessageHandler.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-09-15 11:29:49 +0200 (Do, 15 Sep 2011) $
* $Rev: 21734 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_MessageHandler.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "common.h"
#include "ATCmdHandler.h"
#include "ATParser.h"
#include "ERM.h"
#include "UART.h"
#include "APP_COMMON_Utils.h"
#include "APP_PHONE.h"
#include "POOL.h"
#include "stdio.h"    //  "(void)snprintf" without this include we get compiler warnings !

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "VCR.h"
#include "VCR_CE.h"
#include "VCR_CI.h"
#include "VCR_StateHandler.h"
#include "VCR_MessageHandler.h"
#include "VCR_Utils.h"

#include "VCR_MessageHandler.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern VcrModGlobalDataType _VCR_gData;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
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

static SuccessFailType  _VCR_sFSendATMessage(const SCI_ST*     p_uart,
        VCRSystemMsgEnum  msg_type);

static APP_COMMON_enResponseCode  _VCR_sFSendAPPMessage(VCRSystemMsgEnum  msg_type);
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static Uint8Type _VCR_sysPerfTraceDone = 0;

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Methods                                                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vInitMsgCtrlData                                        */
/**
    \brief      function to initialise the message ctrl variables

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vInitMsgCtrlData(void)
{
    (void)memset (&(_VCR_gData.stCtrlInfo),0x00,sizeof(VcrControlStructType));
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vInitLanguageData                                       */
/**
    \brief      function to initialise the language data variables

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vInitLanguageData(void)
{
    Uint32Type i = 0u;

    for (i = 0u; i < (Uint32Type)VCR_MAX_LANG_TABLE; i++)
    {
        _VCR_gData.eLangTable[i] = VCR_LANG_UNDEF;
    }
    _VCR_gData.u8CurrentLangIdx   = (Uint8Type)AT_SIRV_LANG_MAXNUM;
    _VCR_gData.eCurrentLang       = AT_SIRV_LANG_MAXNUM;
    _VCR_gData.bTrigLangChange    = BT_FALSE;
    _VCR_gData.eRequiredLang      = VCR_DEFAULT_LANGUAGE;
    _VCR_gData.bReqLangSetFromExt = BT_FALSE;
    _VCR_gData.u8NumSupLang       = 0;

    _VCR_gData.bInitial_init_done = BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSetRequiredLanguage                                    */
/**
    \brief      internal set function for required language

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSetRequiredLanguage(ATSIVRLanguagesEnum newLangId)
{
    _VCR_gData.eRequiredLang = newLangId;
    _VCR_gData.bReqLangSetFromExt = BT_TRUE;
}




/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vHandleErmOkErrorEvent                                  */
/**
    \brief      handle OK or ERROR from ERM event

    \brief      process ok or ERROR from ERM and if required trigger state changes

    \param      BooleanType isOK - BT_TRUE if event was OK, ERROR otherwise

    \return     none
   */
/*----------------------------------------------------------------------------*/
void  _VCR_vHandleErmOkErrorEvent(BooleanType isOK)
{
    /* reset indication to wait for OK */
    _VCR_gData.stCtrlInfo.bOkPending      = BT_FALSE;
    /* stop OK supervision */
    _VCR_vStopSupervision();

    if (BT_TRUE == isOK)
    {
        DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Response OK - LastMsgSend %d",  _VCR_gData.stCtrlInfo.eLastSend);
    }
    else
    {
        DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Response ERROR - LastMsgSend %d", _VCR_gData.stCtrlInfo.eLastSend);
    }

    if (BT_TRUE == isOK)
    {
        switch (_VCR_gData.stCtrlInfo.eLastSend)
        {
            case VCR_MSG_GET_LANG_NUM:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_LANGNUM_AVAIL,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_GET_LANG_MAP:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_LANGMAP_AVAIL,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_GET_CURRENT_LANG:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_CUR_LANG_AVAIL,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_SET_CURRENT_LANG:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_CUR_LANG_CHANGED,
                        AT_SIVR_CMD_UNKNOWN);
                /* start async supervision for language change */
                _VCR_vStartSupervision(VCR_SUP_ASYNC);
                break;
            }
            case VCR_MSG_PREPARE_MENU:
            {  /* trigger processing state maching in case of PREPARE*/
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_PREP_OK,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_DIAL:
            {   /* successful dialed - trigger main machine */
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_DIAL_OK,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_REDIAL:
            {   /* send back redial done event to main */
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_REDIAL_DONE,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_STOP_ENGINE:
            {
                _VCR_gData.bStopOKReceived = BT_TRUE;
                /* start async supervision for stop of engine */
                _VCR_vStartSupervision(VCR_SUP_ASYNC);
                break;
            }
            case VCR_MSG_START_ENGINE:
            {
                /* start async supervision for start of engine */
                _VCR_vStartSupervision(VCR_SUP_ASYNC);
                break;
            }
            case VCR_MSG_NAME_VOICE_TAG_ADD:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_VT_ADD_DONE,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }

            case VCR_MSG_NAME_VOICE_TAG_DELETE_ALL:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_VT_DELETE_ALL_DONE,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_NAME_VOICE_TAG_DELETE:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_VT_DELETE_DONE,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            case VCR_MSG_NAME_VOICE_TAG_QUERY:
            {
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_VT_QUERY_DONE,
                        AT_SIVR_CMD_UNKNOWN);
                break;
            }
            default:
            {   /* init processing machine and send error event to mail */
                DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Response OK not handled - LastMsgSend %d ",  _VCR_gData.stCtrlInfo.eLastSend);
                break; /* do nothing */
            }
        }
    }
    else
    {   /* we received an ERROR - stop the machine!!! */
        /* for some send messages we first check if this might be
           due to signal crossing */
        switch (_VCR_gData.stCtrlInfo.eLastSend)
        {
            case VCR_MSG_RESTART_PROMTP_NORECO:
            case VCR_MSG_STOP_RTPROMPT:
            case VCR_MSG_STOP_STR_PRMT_PTT:
            case VCR_MSG_STOP_STR_PRMT_CALL:
            case VCR_MSG_STOP_STR_PRMT_OK:
            case VCR_MSG_STOP_STR_PRMT_RESTART:
            case VCR_MSG_STOP_ENGINE: /*RT#4042*/
            {   /* if engine state indicate that prompt is stopped,
                   just ignore the error and do nothing */
                if (AT_SIVR_ENG_END_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Response ERROR - LastMsgSend %d EngineState %d -> ignore",
                                                                               _VCR_gData.stCtrlInfo.eLastSend,
                                                                               _VCR_gData.stCtrlInfo.eEngineStatus);
                }
                else
                {
                    DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"Response ERROR - LastMsgSend %d -> Stop Engine",
                                                                               _VCR_gData.stCtrlInfo.eLastSend);
                   _VCR_vSystemStopToIdle(BT_FALSE);
                }
                break;
            }
            case VCR_MSG_NAME_VOICE_TAG_DELETE_ALL:
                // Parrot answers with Error if no VT was present -> Ignore -> handle as OK
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_VT_DELETE_ALL_DONE,
                        AT_SIVR_CMD_UNKNOWN);
                break;

            case VCR_MSG_NAME_VOICE_TAG_QUERY:
                // RT#4467 Parrot answers with Error if Sync is started -> handle as retry
                if (BT_TRUE == _VCR_gData.bSivrSyncStarted)
                {
                      // retry
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_NAME_VOICE_TAG_QUERY);
                }
                else
                {
                     DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Response ERROR - LastMsgSend = VCR_MSG_NAME_VOICE_TAG_QUERY -> Stop Engine");
                    _VCR_vSystemStopToIdle(BT_FALSE);
                }
                break;

            case VCR_MSG_START_ENGINE:
                // RT#6311 Parrot answers with Error if Sync is started -> handle as retry
                if (BT_TRUE == _VCR_gData.bSivrSyncStarted)
                {
                      // retry
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_START_ENGINE);
                }
                else
                {
                     DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Response ERROR - LastMsgSend = VCR_MSG_START_ENGINE -> Stop Engine");
                    _VCR_vSystemStopToIdle(BT_FALSE);
                }
                break;

            case VCR_MSG_START_RECO:
                /* RT#6938 VR ends without jingle
                   Timing problem:
                   While switching from READCALLSTACKS -> CALLSTACKOPTIONS
                   After a "stop playing prompt" cmd triggerd  in menu READCALLSTACKS a Event *RSTS:2 is received after we swichted the
                   engine to menu CALLSTACKOPTIONS. Because we are not allowed to trigger a prompt in State *RSTS:2 = beginning of the VR acquisition,
                   we retry till end of VR acquisition
                */
                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                      // retry
                     (void)_VCR_sFMessageToBeSend(VCR_MSG_START_RECO);
                }
                else
                {
                     DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Response ERROR - LastMsgSend = VCR_MSG_START_RECO -> Stop Engine");
                    _VCR_vSystemStopToIdle(BT_FALSE);
                }
                break;


            default:
            {
                DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"Response ERROR - LastMsgSend %d -> Stop Engine",
                                                                           _VCR_gData.stCtrlInfo.eLastSend);
                _VCR_vSystemStopToIdle(BT_FALSE);
                break;
            }
        }

    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFHandleResult                                          */
/**
    \brief      wrapper for performance measures of results

 */
/*----------------------------------------------------------------------------*/
SuccessFailType  _VCR_sFHandleResult(
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType bRetVal = SUCCESS;
    /* evaluate if we need to send performance measures - if so we may need
       to collect some data before we can go on with the normal behaviour */
    if (BT_TRUE == _VCR_gData.bPerfMeasOn)
    {   /* we need to check all received results if some optional params are
           available that need additional messaging to be converted, e.g.
           contact IDs into real names, if so store the current received
           input params in result structure for perf meas and fetch the
           required info with specific handling for perf measures */
        if (BT_FALSE == is_event) /* internal events are ignored!! */
        {   /* call prepare ASR message - here ASR message will be build
               and if completely build it will be send and then this
               trigger will be forwarded */
            /* store received event settings */
            _VCR_gData.bPerfMeasStoreIsEvent = is_event;
            _VCR_gData.ePerfMeasStoreEvent   = event;
            _VCR_gData.ePerfMeasStoreCmd     = cmd;
            /* trigger prepare and return success as
             * PrepareASR will handle completely */
            _VCR_vPrepareASRCmdMessage();
        }
        else
        {   /* directly forward */
            bRetVal = _VCR_sFHandleEvent(is_event,event,cmd);
        }
    }
    else
    {   /* only directly call HandleEvent */
        bRetVal = _VCR_sFHandleEvent(is_event,event,cmd);
    }
    return (bRetVal);
}
/*----------------------------------------------------------------------------*/
/* Function    : VCR_sFHandleATMessage                                       */
/**
    \brief      process response or event message from CK5050

    \brief      process response or event message from CK5050

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType  VCR_sFHandleATMessage(const ATRspMessageType * p_msg)
{
    SuccessFailType sF     = FAIL;
    ATRspAllType  * p_data = NULL;
    Uint32Type i = 0;
    VcrProcessingResultType * p_res = &(_VCR_gData.stMsgRes);

    if (AT_RSP_WITH_DATA == p_msg->dataInd &&
        NULL != p_msg->data    )
    {
        p_data = p_msg->data;
    }
    switch (p_msg->common.rsp_type)
    {
        case AT_RSP_SIVR_START_UP_EVENT:
        {   /* trigger menu to indicate SIVR is supported */
            if (_VCR_sysPerfTraceDone == 0)
            {
               _VCR_sysPerfTraceDone = 1;
               /* Trace only the first startup */
               DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"SYS_PERF: VCR Startup Event: %d ms",osGetSystemCounter() );
            }

            (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_SIVR_SUPPORTED,
                    AT_SIVR_CMD_UNKNOWN);
            /* supervision may be runing, e.g. if language was changed */
            _VCR_vStopSupervision();
            break;
        }
        case AT_RSP_SIVR_START_UP_EVENT_EXT:
        {   /* print trace nothing to do with this info
               this message is sent as extension of RSTU not instead of RSTU */

            _VCR_vNotifyApplication(VCR_NOTIFY_RSTUEX_RECEIVED);

            break;
        }
        case AT_RSP_SIVR_GET_SUPPORTED_LANG:
        {   /* check which status received */
            if (NULL != p_data)
            {
                if (AT_TRUE == p_data->rgsl.is_lang_count)
                {   /* this is number only - store and trigger request of each lang */
                    /* avoid to have more than max numbers */
                    if ((Uint8Type)AT_SIRV_LANG_MAXNUM >= p_data->rgsl.lang_count)
                    {
                        _VCR_gData.u8NumSupLang = p_data->rgsl.lang_count;
                    }
                    else
                    {
                        _VCR_gData.u8NumSupLang = (Uint8Type)AT_SIRV_LANG_MAXNUM;
                    }
                }
                else
                {   /* index and lang id for one language, store in table */
                    if ((Uint8Type)VCR_MAX_LANG_TABLE > p_data->rgsl.lang_index)
                    {   /* store index in table*/
                        _VCR_gData.eLangTable[p_data->rgsl.lang_index] = p_data->rgsl.lang_id;

#if 0
                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR LangIndex %d LangID %d",
                               p_data->rgsl.lang_index,_VCR_gData.eLangTable[p_data->rgsl.lang_index]);
#endif


                       switch (p_data->rgsl.lang_id)
                       {
                          case AT_SIVR_LANG_US_ENG:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = US-English", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_UK_ENG:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = UK-English", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_FRENCH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = French", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_FRENCH_CAN:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Canadian French", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_GERMAN:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = German", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_ITALIAN:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Italien", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_SPANISH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Spanish", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_DUTCH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Dutsch", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_CHINESE_M:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Mandarin Chinese", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_US_SPANISH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = US-Spanish", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_KOREAN:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Korean", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_JAPANESE:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Japanese", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_PORTUGUES:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Portuguese", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_BRAZILIAN:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Brazilian", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_RUSSIAN:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Russian", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_POLISH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Polish", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_TURKEY:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Turkey", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_DANISH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Danish", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_SWEDISH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Swedish", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          case AT_SIVR_LANG_CZECH:
                               DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"*RGSL:%d,%d = Czech", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                          default:
                               DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"*RGSL:%d,%d = unknown Language", p_data->rgsl.lang_index,p_data->rgsl.lang_id);
                               break;

                       }

                    }
                }
            }
            else
            {   /* severe issue as message shall contain data!! */
                 DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VCR missing data in msg GET_SUPPORTED_LANG");
            }
            break;
        }
        case AT_RSP_SIVR_SET_CURRENT_LANG:
        {   /* check if language setting ended */
            if (NULL != p_data)
            {
                if (AT_FALSE == p_data->rscl.is_end)
                {   /* set current parrot language and index*/
                    _VCR_gData.u8CurrentLangIdx = p_data->rscl.lang_index;
                    _VCR_gData.eCurrentLang     = p_data->rscl.lang_id;
                    if (BT_FALSE == _VCR_gData.bInitial_init_done)
                    {   /* choose this as initial required lang
                           only if language change is required  by SIVR or SD
                           or diagnose we will change the language  */
                        if (BT_FALSE == _VCR_gData.bReqLangSetFromExt)
                        {   /* if Req lang has not been modified from external
                               - set to current language */
                            _VCR_gData.eRequiredLang = _VCR_gData.eCurrentLang;
                        }
                        _VCR_gData.bInitial_init_done = BT_TRUE;
                    }
                }
                /* else we ignore the END */
            }
            else
            {   /* severe issue as message shall contain data!! */
                DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VCR missing data in msg SET_CURRENT_LANG");
            }
            break;
        }
        /* received RSTS */
        case AT_RSP_SIVR_VR_STATUS:
        {
            /* supervision may be runing, e.g. if machine was started, stopped */
            _VCR_vStopSupervision();
            /* check which status received */
            if (NULL != p_data)
            {

                switch (p_data->rsts.status)
                {
                    case AT_SIVR_ENG_IDLE:
                         DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d = VR engine is in IDLE (not started)", p_data->rsts.status);
                         break;
                    case AT_SIVR_ENG_STARTED:
                         DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d = VR engine is started", p_data->rsts.status);
                         break;
                    case AT_SIVR_ENG_BEGIN_AQU:
                         DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d = beginning of the VR acquisition", p_data->rsts.status);
                         break;
                    case AT_SIVR_ENG_END_AQU:
                         DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d = end of the VR acquisition", p_data->rsts.status);
                         break;
                    case AT_SIVR_ENG_BEGIN_PROMPT:
                         _VCR_gData.stCtrlInfo.bStopPending = BT_FALSE;
                         DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d->$ATSIVREngStatusEnum$", p_data->rsts.status);
                         DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"Beginning of the prompt: %d->$ATSIVRPromptEnum$", _VCR_gData.u16TxPromptID);
                         break;
                    case AT_SIVR_ENG_END_PROMPT:
                         _VCR_gData.stCtrlInfo.bStopPending = BT_FALSE;
                         DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d->$ATSIVREngStatusEnum$", p_data->rsts.status);
                         DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"End of prompt: %d->$ATSIVRPromptEnum$", _VCR_gData.u16TxPromptID);
                         break;
                    case AT_SIVR_ENG_BEGIN_SYNC:
                         DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d = beginning of SIVR synchronisation", p_data->rsts.status);
                         break;
                    case AT_SIVR_ENG_END_SYNC:
                         DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"*RSTS:%d = end of SIVR synchronisation", p_data->rsts.status);
                         break;
                    default:
                         DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"*RSTS:%d = unknown SIVR State",p_data->rsts.status);
                    break;
               }

                _VCR_gData.stCtrlInfo.eEngineStatus = p_data->rsts.status;


                switch (p_data->rsts.status)
                {
                    case AT_SIVR_ENG_IDLE:
                    {
                        _VCR_gData.bStopOKReceived = BT_FALSE;
                        (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_SIVR_STOPPED,
                                AT_SIVR_CMD_UNKNOWN);

                        break;
                    }
                    case AT_SIVR_ENG_STARTED:
                    {
                        (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_SIVR_STARTED,
                                AT_SIVR_CMD_UNKNOWN);
                         break;
                    }

                    case AT_SIVR_ENG_BEGIN_SYNC:
                    {
                        _VCR_gData.bSivrSyncStarted = BT_TRUE;
                        break;
                    }
                    case AT_SIVR_ENG_END_SYNC:
                    {
                        _VCR_gData.bSivrSyncStarted = BT_FALSE;
                        /* now we are in similar state than idle */
                        _VCR_gData.stCtrlInfo.eEngineStatus = AT_SIVR_ENG_IDLE;
                        break;
                    }
                    case AT_SIVR_ENG_END_PROMPT:
                    {   /* trigger procssing SM that a promt has ended */
                        /* check whom to trigger with prompt end */
                        _VCR_gData.bPTTIgnore = BT_FALSE;

                        if (VCR_MSG_RESTART_PROMTP_NORECO == _VCR_gData.stCtrlInfo.eLastSend)
                        {   /* special handling needed to recover */
                            _VCR_gData.stCtrlInfo.eLastSend = _VCR_gData.stCtrlInfo.eLastSendForPrmtRestart;
                            _VCR_gData.stCtrlInfo.eLastSendForPrmtRestart = VCR_MSG_NONE;
                            /* to finish in case of several prompts */
                            _VCR_gData.u8NumPromptSend = _VCR_gData.u8NumPromptRequired - 1;
                        }
                        DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"eLastSend:%d",_VCR_gData.stCtrlInfo.eLastSend);
                        /* now process due to eLastSend*/
                        switch (_VCR_gData.stCtrlInfo.eLastSend)
                        {
                            case VCR_MSG_PLAY_SYSSTART_PROMPT:
                            {
                                _VCR_gData.u8NumPromptSend++;
                                /* check if last one already send */
                                DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"PromptRequired:%d = %d PromptSend",_VCR_gData.u8NumPromptSend, _VCR_gData.u8NumPromptRequired);
                                if (_VCR_gData.u8NumPromptRequired == _VCR_gData.u8NumPromptSend)
                                {                                    
                                    _VCR_gData.u8NumPromptRequired = 0;
                                    _VCR_gData.u8NumPromptSend     = 0;
                                    (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_START_PROMTP_END,
                                            AT_SIVR_CMD_UNKNOWN);
                                }
                                else

                                {

                                    /* send pending */
                                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTART_PROMPT);
                                        DEBUG_TEXT(VCR_SWI_TRC,MOD_VCR,"Sequence of system start prompts resumed");

                                }
                                break;
                            }
                            case VCR_MSG_PLAY_SYSSTOP_PROMPT:
                            {
                                _VCR_gData.u8NumPromptSend++;
                                /* check if last one already send */
                                if (_VCR_gData.u8NumPromptRequired == _VCR_gData.u8NumPromptSend)
                                {
                                    _VCR_gData.u8NumPromptRequired = 0;
                                    _VCR_gData.u8NumPromptSend     = 0;
                                    (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STOP_PROMPT_END,
                                            AT_SIVR_CMD_UNKNOWN);
                                }
                                else
                                {   /* send pending */
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
                                }

                                break;
                            }
                            case VCR_MSG_STOP_RTPROMPT:
                            {
                                _VCR_gData.u8NumPromptRequired = 0;
                                _VCR_gData.u8NumPromptSend     = 0;
                                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STOP_PROMPT_END,
                                         AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case VCR_MSG_MUSIC_SYSSTART_PROMPT:
                            {
                                _VCR_gData.u8NumPromptSend++;
                                /* check if last one already send */
                                if (_VCR_gData.u8NumPromptRequired == _VCR_gData.u8NumPromptSend)
                                {
                                    _VCR_gData.u8NumPromptRequired = 0;
                                    _VCR_gData.u8NumPromptSend     = 0;
                                    (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_MEDIAPAYER, AT_SIVR_CMD_UNKNOWN);
                                }
                                else
                                {
                                    /* send pending */
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_MUSIC_SYSSTART_PROMPT);
                                }
                                break;
                            }
                            case VCR_MSG_INFO_SYSSTART_PROMPT: //RT13214
                            {
                                _VCR_gData.u8NumPromptSend++;
                                /* check if last one already send */
                                if (_VCR_gData.u8NumPromptRequired == _VCR_gData.u8NumPromptSend)
                                {
                                    _VCR_gData.u8NumPromptRequired = 0;
                                    _VCR_gData.u8NumPromptSend     = 0;
                                    (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_SIVR_STOPPED, AT_SIVR_CMD_UNKNOWN);
                                }
                                else
                                {
                                    /* send pending */
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_INFO_SYSSTART_PROMPT);
                                }
                                break;
                            }
                            case VCR_MSG_PLAY_PROMPT:
                            case VCR_MSG_PLAY_PROMPT_MENU:
                            {
                                _VCR_gData.u8NumPromptSend++;
                                /* check if last one already send */
                                if (_VCR_gData.u8NumPromptRequired == _VCR_gData.u8NumPromptSend)
                                {
                                    /* special handling in case of haptic - name, sms or single */

                                    if ( (VCR_HAPTIC_PLAY_NAME == _VCR_gData.eHapticTrigType && AT_SIVR_PRMT_D_NAME == _VCR_gData.ePromtTable.tab[0] ) ||
                                         (VCR_HAPTIC_PLAY_TEST == _VCR_gData.eHapticTrigType && AT_SIVR_PRMT_TEST_DIAGNOSE == _VCR_gData.ePromtTable.tab[0] ) ||
                                         (VCR_HAPTIC_READ_SMS  == _VCR_gData.eHapticTrigType && AT_SIVR_PRMT_SMS_XPLUSPLUS == _VCR_gData.ePromtTable.tab[0] )
                                       )
                                    {   /* finish SIVR without stop prompt */
                                        _VCR_gData.u8NumPromptRequired = 0;
                                        _VCR_gData.u8NumPromptSend     = 0;
                                        (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STOP_PROMPT_END,
                                                AT_SIVR_CMD_UNKNOWN);
                                    }
                                    else
                                    {
                                        _VCR_gData.u8NumPromptRequired = 0;
                                        _VCR_gData.u8NumPromptSend     = 0;
                                        (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STRING_PRMT_OK,
                                                AT_SIVR_CMD_UNKNOWN);
                                    }

                                }
                                else
                                {   /* send pending */
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT);
                                }
                                break;
                            }
                            case VCR_MSG_STOP_STR_PRMT_PTT:
                            {
                                _VCR_gData.u8NumPromptRequired = 0;
                                _VCR_gData.u8NumPromptSend     = 0;
                                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STR_PRMT_END_PTT,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case VCR_MSG_STOP_STR_PRMT_CALL:
                            {
                                _VCR_gData.u8NumPromptRequired = 0;
                                _VCR_gData.u8NumPromptSend     = 0;
                                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STR_PRMT_END_CALL,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case VCR_MSG_STOP_STR_PRMT_OK:
                            {
                                _VCR_gData.u8NumPromptRequired = 0;
                                _VCR_gData.u8NumPromptSend     = 0;
                                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_STR_PRMT_END_OK,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case VCR_MSG_STOP_STR_PRMT_RESTART:
                            {
                                _VCR_gData.u8NumPromptRequired = 0;
                                _VCR_gData.u8NumPromptSend     = 0;
                                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_PRMT_END_RESTART,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case VCR_MSG_PLAY_CONNECT_PROMPT:
                            {
                                _VCR_gData.u8NumPromptRequired = 0;
                                _VCR_gData.u8NumPromptSend     = 0;
                                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_CONNECT_PROMTP_END,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case VCR_MSG_PLAY_REDIAL_PROMPT:
                            {   /* here only single prompt is send */
                                _VCR_gData.u8NumPromptSend++;
                                /* check if last one already send */
                                if (_VCR_gData.u8NumPromptRequired == _VCR_gData.u8NumPromptSend)
                                {
                                    _VCR_gData.u8NumPromptRequired = 0;
                                    _VCR_gData.u8NumPromptSend     = 0;
                                    (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_REDIAL_PROMPT_END,
                                            AT_SIVR_CMD_UNKNOWN);
                                }
                                else
                                {   /* send pending */
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_REDIAL_PROMPT);
                                }
                                break;
                            }
                            /* suppress in case of RECO */
                            case VCR_MSG_START_RECO:
                            {
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                    case AT_SIVR_ENG_BEGIN_AQU:
                    case AT_SIVR_ENG_END_AQU:
                    {   /* we need to update the ASR status */
                        if (BT_TRUE == _VCR_gData.bPerfMeasOn)
                        {   /* send ASR status update for changed menu id */
                            _VCR_vSendASRMessage(VCR_PERFSTAT_STATUS);
                        }
                        break;
                    }
                    case AT_SIVR_ENG_BEGIN_PROMPT:
                    default:
                    {   /* do nothing */
                        break;
                    }
                }
            }
            else
            {   /* severe issue as message shall contain data!! */
                DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"VCR missing data in msg  %d", p_msg->common.rsp_type);
            }
            break;
        }
        /* process result messages */
        case AT_RSP_SIVR_VR_RESULT:
        {   /* complex evaluation of the results needed, first await message
               with number of expected results, if this is 0 -> immediately
               trigger error handling */
            VcrResultStructType *p_result = NULL;
            if (NULL != p_data)
            {
                if (AT_TRUE == p_data->rres.is_nbr)
                {   /* number of responses, shall be in optimal case 1, if 0 this is an
                       error case and system message shall be indicated - in this case also
                       one additional RRES with problem indications follow, if > 1 there are
                       more possible solutions, first store number in control  */
                    /* reset error counters if more than 0 results will be received
                     * and also clean up the result memory  */
                    if (0 < p_data->rres.nb_of_result)
                    {
                        _VCR_gData.u8ProcErr = 0;
                    }
                    p_res->u8NbrResults         = p_data->rres.nb_of_result;
                    p_res->u8NbrReceivedResults = 0;
                }
                else
                {   /* check which result number received and if necessary store */
                    p_result = &(p_res->stResults[p_res->u8NbrReceivedResults]);

                    if ((0x00 == p_res->u8NbrResults) && (p_data->rres.result_type != AT_SIVR_RES_VT_REJECTED))    // RT#4716
                    {   /* evaluate result type and set error counters, promt and menu
                           accordingly */
                        p_result->menu_id             = (Uint16Type)p_data->rres.menu_id;
                        p_result->result_index        = p_data->rres.result_index;
                        p_result->result_type         = p_data->rres.result_type;
                        p_result->confidence_rate     = p_data->rres.confidence_rate;
                        if (AT_SIVR_RES_NO_SPEECH == p_result->result_type ||
                            AT_SIVR_RES_NOT_RECO == p_result->result_type    )
                        {
                            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"RT-13225: Cmd not recongnized  Procerr %d  PrevProcerr %d",_VCR_gData.u8ProcErr, _VCR_gData.u8PrevProcErr);
                            _VCR_gData.u8ProcErr = _VCR_gData.u8ProcErr + 1;
                        }
                        (void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_PROC_ERROR,
                                AT_SIVR_CMD_UNKNOWN);
                    }
                    else
                    {
                        if ( (AT_SIVR_RES_USER_CMD == p_data->rres.result_type) &&
                             ( ( AT_SIVR_PRMT_CK5050_MAXVALUE < p_data->rres.menu_id )
                                || ( AT_SIVR_CMD_UNKNOWN == p_data->rres.result_value ) )
                            )
                        {
                            /* workaround for RT-705 */
                            p_data->rres.result_type = AT_SIVR_RES_NOT_RECO;
                        }


                        switch (p_data->rres.result_type)
                        {
                            case AT_SIVR_RES_USER_CMD:
                            {
                                /* store up to VCR_MAXNUM_RES_STORE results */
                                if (VCR_MAXNUM_RES_STORE > p_res->u8NbrReceivedResults)
                                {
                                    p_result->menu_id             = (Uint16Type)p_data->rres.menu_id;
                                    p_result->result_index        = p_data->rres.result_index;
                                    p_result->result_type         = p_data->rres.result_type;
                                    p_result->confidence_rate     = p_data->rres.confidence_rate;
                                    p_result->result_value        = p_data->rres.result_value;
                                    /* copy optional parameters but check number  */
                                    if (0 == p_res->u8NbrReceivedResults)
                                    {   /* check if this is name string, if so fill */
                                        /* copy received data */
                                        (void)memcpy(&(p_result->u),&(p_data->rres.opt),
                                                sizeof(ATResSIVROptUnionType));
                                        if (AT_SIVR_RES_NUMBER == p_data->rres.opt.nb.type)
                                        {   /* remove possible white spaces from string */
                                            p_res->number_len   = 0;
                                            if (p_data->rres.opt.nb.number_len > 0 &&
                                                p_data->rres.opt.nb.number_len <= AT_SIVR_MAX_PRMT_NUM_LEN)
                                            {
                                                for (i = 0; i < p_data->rres.opt.nb.number_len ; i++)
                                                {
                                                    if (0x20 != p_data->rres.opt.nb.number[i])
                                                    {
                                                        p_res->number[p_res->number_len] =
                                                            p_data->rres.opt.nb.number[i];
                                                            p_res->number_len = p_res->number_len + 1;
                                                    }
                                                }
                                                if (0 < p_res->number_len)
                                                {
                                                    p_res->number_valid = BT_TRUE;
                                                }
                                            }
                                        }
                                        /* else no more than copy required */
                                    }
                                    else
                                    {   /* only copy received data but not number */
                                        (void)memcpy(&(p_result->u),&(p_data->rres.opt),
                                                sizeof(ATResSIVROptUnionType));
                                    }

                                    switch (p_result->result_value)
                                    {

                                        case AT_SIVR_CMD_HELP:
                                             DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd %d = HELP",
                                                    p_result->result_value);
                                             break;
                                        case AT_SIVR_CMD_CANCEL:
                                             DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd %d = CANCEL",
                                                    p_result->result_value);
                                             break;
                                        /* commands from main menu */
                                        case AT_SIVR_CMD_DIAL:
                                             DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = DIAL",
                                                    p_result->result_value);
                                             break;
                                        case AT_SIVR_CMD_DIAL_NUMBER:
                                             DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = DIAL_NUMBER",
                                                    p_result->result_value);
                                             break;
                                        case AT_SIVR_CMD_REDIAL:
                                             DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = REDIAL",
                                                    p_result->result_value);
                                             break;

                                       case AT_SIVR_CMD_MUSIC:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = MUSIC",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_MORE_OPTIONS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = MORE_OPTIONS",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PLAY_ADDRESSBOOK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = PLAY_ADDRESSBOOK",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_READ_SMS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = READ_SMS",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_CALL_STACKS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = CALL_STACKS",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_SETTINGS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = SETTINGS",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_VOICE_MEMOS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = VOICE_MEMOS",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_TRAINING:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MainMenu %d = TRAINING",
                                                         p_result->result_value);
                                            break;
                                       /* commands from DialNameConfirm Menu */
                                       case AT_SIVR_CMD_DIAL_DIALNAMECONFIRM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirm %d = DIAL_DIALNAMECONFIRM",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_YES_DIALNAMECONFIRM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirm %d = YES_DIALNAMECONFIRM",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_NO_DIALNAMECONFIRM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirm %d = NO_DIALNAMECONFIRM",
                                                         p_result->result_value);
                                            break;

                                       /* commands from DialNameConfirmCategories Menu */
                                       case AT_SIVR_CMD_CATEGORY:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirm %d = CATEGORY",
                                                         p_result->result_value);
                                            break;

                                       /* commands from DialNameConfirmResolveAmbiguouseName Menu */
                                       case AT_SIVR_CMD_YES_RESOLVEAMBINAME:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirmResolveAmbiguouseNames %d = YES_RESOLVEAMBINAME",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_REPEAT_NAMES:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirmResolveAmbiguouseNames %d = REPEAT_NAMES",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_NO_RESOLVEAMBINAME:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNameConfirmResolveAmbiguouseNames %d = NO_RESOLVEAMBINAME",
                                                         p_result->result_value);
                                            break;

                                       /* commands from DialNumber Menu */
                                       case AT_SIVR_CMD_DIALNUM_D_BLOCK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumber %d = DIALNUM_D_BLOCK",
                                                         p_result->result_value);
                                            break;

                                       /* commands from DialNumberCorrect Menu */
                                       case AT_SIVR_CMD_DIAL_DIALNUMCORRECT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumberCorrect %d = DIAL_DIALNUMCORRECT",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_SAVE_DIALNUMCORRECT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumberCorrect %d = SAVE_DIALNUMCORRECT",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_NUMCORRECT_D_BLOCK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumberCorrect %d = NUMCORRECT_D_BLOCK",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_CORRECTION:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumberCorrect %d = CORRECTION",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_DELETE_DIALNUMCORRECT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumberCorrect %d = DELETE_DIALNUMCORRECT",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_REPEAT_DIALNUMCORRECT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DialNumberCorrect %d = REPEAT_DIALNUMCORRECT",
                                                         p_result->result_value);
                                            break;

                                      /* commands from Music Menu */
                                      case AT_SIVR_CMD_MUSIC_PLAY:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Music %d = MUSIC_PLAY",
                                                  p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_MUSIC_PAUSE:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Music %d = MUSIC_PAUSE",
                                                   p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_MUSIC_STOP:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Music %d = MUSIC_STOP",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_MUSIC_NEXT_TRACK:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Music %d = MUSIC_NEXT_TRACK",
                                                  p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_MUSIC_PREV_TRACK:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Music %d = MUSIC_PREV_TRACK",
                                                   p_result->result_value);
                                           break;

                                       /* commands from options menu */
                                       case AT_SIVR_CMD_OPT_PLAY_ADDBOOK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MoreOptions %d = PLAY_ADDBOOK",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_OPT_READ_SMS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MoreOptions %d = READ_SMS",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_OPT_CALL_STACKS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MoreOptions %d = CALL_STACKS",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_OPT_SETTINGS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MoreOptions %d = SETTINGS",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_OPT_VOICE_MEMOS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MoreOptions %d = VOICE_MEMOS",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_OPT_TRAINING:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of MoreOptions %d = TRAINING",
                                                    p_result->result_value);
                                            break;

                                       /* commands from play Addressbook menu */
                                       case AT_SIVR_CMD_PAB_ADD_NEW_CONTACT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbook %d = PAB_ADD_NEW_CONTACT",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_NAME_PLAY_ADDBOOK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbook %d = PAB_NAME_PLAY_ADDBOOK",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_READ_ALL:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbook %d = PAB_READ_ALL",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_REFRESH_ADDBOOK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbook %d = REFRESH_ADDBOOK",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_QS_PLAY_ADDBOOK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbook %d = PLAY_ADDBOOK",
                                                    p_result->result_value);
                                            break;

                                       /* commands from play addressbook entry menu */
                                       case AT_SIVR_CMD_PAB_YES_PLAY_PAB_ENTRY:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookEntry %d = PAB_YES_PLAY_PAB_ENTRY",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_REPEAT_PAB_ENTRY:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookEntry %d = PAB_REPEAT_PAB_ENTRY",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_NO_PLAY_PAB_ENTRY:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookEntry %d = PAB_NO_PLAY_PAB_ENTRY",
                                                         p_result->result_value);
                                            break;

                                       /* commands from play addressbook options */
                                       case AT_SIVR_CMD_PAB_OPT_DIAL:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = CMD_PAB_OPT_DIAL",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_OPT_CHANGE_NAME:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = PAB_OPT_CHANGE_NAME",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_OPT_RESET_NAME:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = PAB_OPT_RESET_NAME",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_OPT_GO_AHEAD:
                                       case AT_SIVR_CMD_PAB_OPT_GO_AHEAD_OLD:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = PAB_OPT_GO_AHEAD",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_OPT_GO_BACK:
                                       case AT_SIVR_CMD_PAB_OPT_GO_BACK_OLD:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = PAB_OPT_GO_BACK",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_OPT_NEXT_LETTER:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = PAB_OPT_NEXT_LETTER",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_PAB_OPT_PREV_LETTER:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of PlayAddressbookOptions %d = PAB_OPT_PREV_LETTER",
                                                              p_result->result_value);
                                            break;

                                       /* commands from resolve multiple numbers menu */
                                       case AT_SIVR_CMD_YES_RES_MULTIPLENUM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ResolveMultipleNumbersPerCategory %d = RES_MULTIPLENUM",
                                                   p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_REPEAT_NUMBERS:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ResolveMultipleNumbersPerCategory %d = RES_MULTIPLENUM",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_NO_RES_MULTIPLENUM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ResolveMultipleNumbersPerCategory %d = NO_RES_MULTIPLENUM",
                                                         p_result->result_value);
                                            break;
                                       /* commands from reset name confirm menu */
                                       case AT_SIVR_CMD_YES_RESETNAMECONFIRM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ResetNameConfirm %d = YES_RESETNAMECONFIRM",
                                                    p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_NO_RESETNAMECONFIRM:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ResetNameConfirm %d = NO_RESETNAMECONFIRM",
                                                         p_result->result_value);
                                            break;

                                      /* commands from call stacks menu */
                                      case AT_SIVR_CMD_CSTACK_RECEIVED_CALL:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacks %d = CSTACK_RECEIVED_CALL",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_CSTACK_MISSED_CALL:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacks %d = CSTACK_MISSED_CALL",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_CSTACK_DIALLED_NUMBERS:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacks %d = CSTACK_DIALLED_NUMBERS",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_CSTACK_CLEAR_ALL_STACK:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacks %d = CSTACK_CLEAR_ALL_STACK",
                                                        p_result->result_value);
                                           break;

                                      /* commands from read call stacks menu */
                                      case AT_SIVR_CMD_CSTACK_YES_READ:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadCallStacks %d = CSTACK_YES_READ",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_CSTACK_REPEAT:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadCallStacks %d = CSTACK_REPEAT",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_CSTACK_NO_READ:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadCallStacks %d = CSTACK_NO_READ",
                                                        p_result->result_value);
                                           break;

                                       /* commands from call stack options menu */
                                       case AT_SIVR_CMD_CSTACK_DIAL_CSOPT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacksOptions %d = CSTACK_DIAL_CSOPT",
                                                         p_result->result_value);
                                            break;

                                       /* commands from call stack reset confirm menu */
                                       case AT_SIVR_CMD_CSTACK_YES_RESETCONF:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacksResetConfirm %d = CSTACK_YES_RESETCONF",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_CSTACK_NO_RESETCONF:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallStacksResetConfirm %d = CSTACK_NO_RESETCONF",
                                                         p_result->result_value);
                                            break;

                                      /* commands from settings menu */
                                      case AT_SIVR_CMD_SET_VF_OFF:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = VOICE_FEEDBACK_OFF",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_SET_VF_ON:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings  %d = VOICE_FEEDBACK_ON",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_SET_BT_OPTIONS:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = BLUETOOTH_OPTIONS",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_SET_RINGTONE:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = RINGTONE",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_SET_CHG_LANGUAGE:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = CHANGE_LANGUAGE",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_SET_CHG_VM_NUMBER:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = CHANGE_VM_NUMBER",
                                                        p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_SET_DELETE_VTAGS:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = DELETE_VOICE_TAGS",
                                                        p_result->result_value);
                                           break;

                                       case AT_SIVR_CMD_SET_RESTORE_FACTORY:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of Settings %d = RESTORE_FACTORY",
                                                         p_result->result_value);
                                            break;

                                       /* commands from BT options menu */
                                       case AT_SIVR_CMD_BTOPT_BT_VISIBLE:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of BluetoothOptions %d = BLUETOOTH_VISIBLE",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_BTOPT_CHG_PIN:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of BluetoothOptions %d = BLUETOOTH_CHANGE_PIN",
                                                         p_result->result_value);
                                            break;

                                       /* commands from ChangeBluetoothPIN menu */
                                       case AT_SIVR_CMD_BTPIN_BLOCK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ChangeBluetoothPIN %d = BLUETOOTH_PIN_BLOCK",
                                                         p_result->result_value);
                                            break;

                                       /* commands from ChangeBluetoothPINCorrect menu */
                                       case AT_SIVR_CMD_BTPIN_COR_SAVE:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ChangeBluetoothPINCorrect %d = BTPIN_COR_SAVE",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_BTPIN_COR_BLOCK:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ChangeBluetoothPINCorrect %d = BTPIN_COR_BLOCK",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_BTPIN_COR_CORRECT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ChangeBluetoothPINCorrect %d = BTPIN_COR_CORRECT",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_BTPIN_COR_REPEAT:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ChangeBluetoothPINCorrect %d = BTPIN_COR_REPEAT",
                                                          p_result->result_value);
                                            break;

                                       /* commands for CallBackMissedCalls */
                                       case AT_SIVR_CMD_MISSED_CALL_DIAL:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallBackMissedCalls %d = MISSED_CALL_DIAL",
                                                          p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_MISSED_CALL_YES:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallBackMissedCalls %d = MISSED_CALL_YES",
                                                         p_result->result_value);
                                            break;

                                       case AT_SIVR_CMD_MISSED_CALL_NO:
                                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallBackMissedCalls %d = MISSED_CALL_NO",
                                                          p_result->result_value);
                                            break;

                                      /* commands from RestoreFactoryDefaultsConfirm menu */
                                      case AT_SIVR_CMD_RESTOREFACTORY_YES:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of RestoreFactoryDefaultsConfirm %d = RESTOREFACTORY_YES",
                                                         p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_RESTOREFACTORY_NO:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of RestoreFactoryDefaultsConfirm %d = RESTOREFACTORY_NO",
                                                        p_result->result_value);
                                           break;


                                      /* commands from DeleteAllVoiceTagsConfirm menu */
                                      case AT_SIVR_CMD_DELETEVT_CONF_YES:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DeleteAllVoiceTagsConfirm %d = DELETEVT_CONF_YES",
                                                         p_result->result_value);
                                           break;

                                      case AT_SIVR_CMD_DELETEVT_CONF_NO:
                                           DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DeleteAllVoiceTagsConfirm %d = DELETEVT_CONF_NO",
                                                        p_result->result_value);
                                           break;

                                     /* commands from ReadSMS menu */
                                     case AT_SIVR_CMD_READSMS_YES:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMS %d = READSMS_YES",
                                                        p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_READSMS_REPEAT:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMS %d = READSMSOPT_REPEAT",
                                                       p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_READSMS_NO:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMS %d = READSMS_NO",
                                                       p_result->result_value);
                                          break;

                                     /* commands from ReadSMSOptions menu */
                                     case AT_SIVR_CMD_READSMSOPT_CALL_YES:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMSOptions %d = READSMSOPT_CALL_YES",
                                                        p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_READSMSOPT_REPEAT:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMSOptions %d = READSMSOPT_REPEAT",
                                                       p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_READSMSOPT_DELETE:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMSOptions %d = READSMSOPT_DELETE",
                                                       p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_READSMSOPT_PREVIOUS:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMSOptions %d = READSMSOPT_PREVIOUS",
                                                       p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_READSMSOPT_NEXT:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of ReadSMSOptions %d = READSMSOPT_NEXT",
                                                       p_result->result_value);
                                          break;

                                     /* commands from DeleteSMSConfirm menu */
                                     case AT_SIVR_CMD_DELETESMS_YES:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DeleteSMSConfirm %d = DELETESMS_YES",
                                                        p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_DELETESMS_DELETE:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DeleteSMSConfirm %d = DELETESMS_DELETE",
                                                       p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_DELETESMS_NO:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of DeleteSMSConfirm %d = DELETESMS_NO",
                                                       p_result->result_value);
                                          break;

                                     /* commands from CallBackSMSConfirm menu */
                                     case AT_SIVR_CMD_CALLBACKSMS_YES:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallBackSMSConfirm %d = CALLBACKSMS_YES",
                                                        p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_CALLBACKSMS_DIAL:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallBackSMSConfirm %d = CALLBACKSMS_DIAL",
                                                       p_result->result_value);
                                          break;

                                     case AT_SIVR_CMD_CALLBACKSMS_NO:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of CallBackSMSConfirm %d = CALLBACKSMS_NO",
                                                       p_result->result_value);
                                          break;

                                     /* commands from StoreName menu */
                                     case AT_SIVR_CMD_STORE_NAME_REC:
                                          DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"User cmd of StoreName %d = STORE_NAME_REC",
                                                       p_result->result_value);
                                          break;

                                     /* commands from VoiceMemos menu */




                                      /* not used -> short tarce */
                                      /* commands from Ringtone menu */
                                      case AT_SIVR_CMD_RINGTONE_YES:
                                      case AT_SIVR_CMD_RINGTONE_REPEAT:
                                      case AT_SIVR_CMD_RINGTONEOPT_CONFIRM:
                                      case AT_SIVR_CMD_RINGTONEOPT_GOAHEAD:
                                      /* commands from ChangeLanguage menu */
                                      case AT_SIVR_CMD_CHGLANG_GERMAN:
                                      case AT_SIVR_CMD_CHGLANG_FRENCH:
                                      case AT_SIVR_CMD_CHGLANG_ENGLISH:
                                      case AT_SIVR_CMD_CHGLANG_CZECH:
                                      case AT_SIVR_CMD_CHGLANG_ITALIAN:
                                      case AT_SIVR_CMD_CHGLANG_RUSSIAN:
                                      case AT_SIVR_CMD_CHGLANG_DUTCH:
                                      /* commands from ChangeLanguageConfirm menu */
                                      case AT_SIVR_CMD_CHANGLANGCONF_YES:
                                      case AT_SIVR_CMD_CHANGLANGCONF_NO:
                                      /* commands from VoiceMemos menu */
                                      case AT_SIVR_CMD_VMEMO_NEW:
                                      case AT_SIVR_CMD_VMEMO_PLAY:
                                      case AT_SIVR_CMD_VMEMO_DELETE:
                                      /* commands from VoiceMemosPlayMemos menu */
                                      case AT_SIVR_CMD_VMEMO_OPT_NEXT:
                                      case AT_SIVR_CMD_VMEMO_OPT_PREVIOUS:
                                      case AT_SIVR_CMD_VMEMO_OPT_DELETE:
                                      /* commands from VoiceMemosConfirmDelete menu */
                                      case AT_SIVR_CMD_VMEMO_CONF_YES:
                                      case AT_SIVR_CMD_VMEMO_CONF_DELETE:
                                      case AT_SIVR_CMD_VMEMO_CONF_NO:
                                      /* commands from VoiceMemosConfirmDeleteAll menu */
                                      case AT_SIVR_CMD_VMEMO_CONFDEL_YES:
                                      case AT_SIVR_CMD_VMEMO_CONFDEL_DELETE:
                                      case AT_SIVR_CMD_VMEMO_CONFDEL_NO:
                                      /* commands from Training menu */
                                      case AT_SIVR_CMD_TRAINING_YES:
                                      case AT_SIVR_CMD_TRAINING_NO:


                                      default:
                                              DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR result %d %d",
                                                    p_result->result_value,p_data->rres.opt.id.type);
                                              break;
                                    }


                                }

                                if (0x01 == p_res->u8NbrResults)
                                {   /* only one matching result exists, reset error counters */
                                    _VCR_gData.u8ProcErr   = 0;
                                    /* write received values to result index 0 */
                                }
                                p_res->u8NbrReceivedResults++;
                                if (p_res->u8NbrReceivedResults >= p_res->u8NbrResults)
                                {   /* evaluate error and silence counters */
                                    if (0x00 == _VCR_gData.u8ProcErr &&
                                        0x00 <  p_res->u8NbrResults)
                                    {
                                        if (SUCCESS == _VCR_sFCheckResultAvailable())
                                        {
                                            _VCR_gData.u8ProcErr = 0;
                                            /* check for correct system state - otherwise ignore message */
                                            if (VCR_SYSTEM_STARTED_PTT == _VCR_gData.eSystemState)
                                            {
                                            	(void)_VCR_sFHandleResult(BT_FALSE,VCR_MEVT_UNDEF,
                                            	        _VCR_gData.eCmdId);
                                            }
                                            sF = SUCCESS;
                                            break;
                                        }
                                        else
                                        {
                                            if (VCR_SYSTEM_STARTED_PTT == _VCR_gData.eSystemState)
                                            {
                                            	(void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_PROC_ERROR,
                                                        AT_SIVR_CMD_UNKNOWN);
                                            }
                                            else
                                            {
                                                _VCR_gData.u8ProcErr = 0;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (VCR_SYSTEM_STARTED_PTT == _VCR_gData.eSystemState)
                                        {
                                        	(void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_PROC_ERROR,
                                                    AT_SIVR_CMD_UNKNOWN);
                                        }
                                        else
                                        {
                                            _VCR_gData.u8ProcErr = 0;
                                        }
                                    }
                                    DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"VCR rsp trigger %d %d",
                                            p_res->u8NbrResults,_VCR_gData.u8ProcErr);

                                }
                                else
                                {
                                    DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"VCR rsp %d %d",
                                            p_res->u8NbrReceivedResults,p_res->u8NbrResults);
                                }
                                break;
                            }
                            case AT_SIVR_RES_VT_RECORDED:
                            {
                            	(void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_NAME_RECORDED,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case AT_SIVR_RES_VT_REJECTED:
                            {

                                /* evaluate reject cause */
                                switch (p_data->rres.result_value)
                                {
                                    case AT_VTREC_RES_TOO_CLOSE:
                                    {
                                        // store names
                                        _VCR_gData.u16SimiliarNameVT = p_data->rres.opt.id.id1;

                                        (void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_NAME_RECORD_FAIL_NOT_UNIQUE,
                                            AT_SIVR_CMD_UNKNOWN);
                                        break;
                                    }
                                    case AT_VTREC_RES_TOO_SHORT:
                                    {
                                        (void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_NAME_RECORD_FAIL_SHORT,
                                            AT_SIVR_CMD_UNKNOWN);
                                        break;
                                    }
                                    case AT_VTREC_RES_TOO_SOFT:
                                    case AT_VTREC_RES_TOO_LOUD:
                                    case AT_VTREC_RES_TOO_BAD:
                                    default:
                                    {
                                        (void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_NAME_RECORD_FAIL,
                                            AT_SIVR_CMD_UNKNOWN);
                                        break;
                                    }
                                }
                                break;
                            }
                            case AT_SIVR_RES_TRAIN_ACCEPT:
                            {   /* to ignore erronous follow up result reject, mark
                                   reception of accept here */
                                _VCR_gData.resultInfo.training.ignore_error = BT_TRUE;
                                (void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_TRAINING_ACCEPT,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case AT_SIVR_RES_TRAIN_REJECT:
                            {
                                if (BT_TRUE == _VCR_gData.resultInfo.training.ignore_error)
                                {
                                    _VCR_gData.resultInfo.training.ignore_error = BT_FALSE;
                                }
                                else
                                {
                                	(void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_TRAINING_REJECT,
                                            AT_SIVR_CMD_UNKNOWN);
                                }
                                break;
                            }
                            case AT_SIVR_RES_TRAIN_FINISH:
                            {
                            	(void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_TRAINING_FINISH,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                            case AT_SIVR_RES_NO_SPEECH:
                            case AT_SIVR_RES_NOT_RECO:
                            default:
                            {   /* shall never happen - retry */
                                DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"VCR inval result type %d",
                                        p_data->rres.result_type);

                                (void)_VCR_sFHandleResult(BT_TRUE,VCR_MEVT_PROC_ERROR,
                                        AT_SIVR_CMD_UNKNOWN);
                                break;
                            }
                        }
                    }
                }
                DEBUG_VALUE4(VCR_SWI_DBG, MOD_VCR,"VCR rsp cnt %d %d %d",
                       p_res->u8NbrReceivedResults, p_res->u8NbrResults,
                       _VCR_gData.u8ProcErr, 0);

            }
            else
            {
                /* severe issue as message shall contain data!! */
                DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"VCR mis data msg %d",
                        p_msg->common.rsp_type);
            }
            break;
        }
        case AT_RSP_SIVR_PREPARE_VR:
        {   /* handle training string prompt */
            if (VCR_RESULT_TYPE_TRAINING == _VCR_gData.resultInfo.training.type)
            {   /* copy to training info */
                _VCR_gData.resultInfo.training.text_len =
                    p_data->rpre.text_len;
                (void)memcpy(&(_VCR_gData.resultInfo.training.text),
                        &(p_data->rpre.text),
                        _VCR_gData.resultInfo.training.text_len);
                _VCR_gData.resultInfo.training.text[
                     _VCR_gData.resultInfo.training.text_len] = '\0';
                /* trigger the SM to send the text */
                (void)_VCR_sFHandleEvent(BT_TRUE,VCR_MEVT_REC_TRAINING_INFO,
                        AT_SIVR_CMD_UNKNOWN);
            }
            else
            {
                sF = FAIL;
            }
            break;
        }
        case AT_RSP_SIVR_MGR_VT:
        {   /* identify if a voice tag exists for a given contact - this is
               only used in phonebook */
            if (VCR_RESULT_TYPE_PLAYPB == _VCR_gData.resultInfo.playPB.type)
            {
                if (0 == p_data->rvta.nb_voice_tag)
                {
                    _VCR_gData.resultInfo.playPB.voice_tag_avail = BT_FALSE;
                }
                else
                {
                    _VCR_gData.resultInfo.playPB.voice_tag_avail = BT_TRUE;
                }
            }
            else
            {
                sF = FAIL;
                DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"VCR msg %d inco restype %d",
                        p_msg->common.rsp_type,
                        _VCR_gData.resultInfo.playPB.type);
            }
            break;
        }
        default:
        {
            sF = FAIL;
            DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"VCR unsup msg %d",
                    p_msg->common.rsp_type);
            break;
        }
    }
    if (SUCCESS == sF)
    {
        DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCR proc msg %d",
                p_msg->common.rsp_type);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vCheckAndSendATMessage                                  */
/**
    \brief     check if AT message to send and if so prepare and send it

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vCheckAndSendATMessage(EventMaskType events)
{
    BooleanType       retry = BT_FALSE;
    const SCI_ST*     p_Uart = NULL;
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;
    /* NOTE: To avoid consecutive fast retries, first time is directly tried,
     * if retry is needed, timer is set to trigger evAppTaskVCRPending */
    if (VCR_MSG_NONE != _VCR_gData.stCtrlInfo.eReqToBeSend &&
        BT_FALSE == _VCR_gData.stCtrlInfo.bOkPending &&
        (0x0000 == _VCR_gData.stCtrlInfo.uNumRetry || (events & evAppTaskVCRPending) ))
    {   /* cehck if we need to send to APP or AT directly */
        if (BT_TRUE == _VCR_gData.stCtrlInfo.bIsAPPMsg)
        {   /* send via APP function */
            appRsp = _VCR_sFSendAPPMessage(_VCR_gData.stCtrlInfo.eReqToBeSend);
            switch (appRsp)
            {
                case APP_COMMON_RSP_OK:
                {   /* send success!!! do nothing */
                    break;
                }
                case APP_COMMON_RSP_ERROR_BUSY:
                {   /* retry */
                    retry = BT_TRUE;
                    break;
                }
                case APP_COMMON_RSP_ERROR:
                case APP_COMMON_RSP_ERROR_BAP_PARSE:
                case APP_COMMON_RSP_ERROR_AT_SEND:
                case APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT:
                case APP_COMMON_RSP_ERROR_UART_BLOCKED:
                default:
                {   /* an error happened - discard */
                    DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"error %d discard msg %d",
                            appRsp,_VCR_gData.stCtrlInfo.eReqToBeSend);
                    _VCR_gData.stCtrlInfo.eReqToBeSend = VCR_MSG_NONE;
                    _VCR_gData.stCtrlInfo.uNumRetry    = 0x0000;
                    _VCR_gData.stCtrlInfo.bOkPending   = BT_FALSE;
                    break;
                }
            }
        }
        else
        {   /* check if we get the UART */
            p_Uart = ERM_GetUartForVCR();
            if (NULL != p_Uart)
            {   /* clear previous UART get counter */
                DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"VCR get UART %d",_VCR_gData.stCtrlInfo.eReqToBeSend);

                /* prepare and send the message */
                if (SUCCESS != _VCR_sFSendATMessage(p_Uart,_VCR_gData.stCtrlInfo.eReqToBeSend))
                {   /* message send failed - UART is released in _VCR_sFSendATMessage */
                    /* discard */
                    DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"error AT discard msg %d",
                           _VCR_gData.stCtrlInfo.eReqToBeSend);

                    /* this is a severe error, if the system is activated and audio
                     * is already switched in here, we stop the system and switch audio
                     * back!!! first clear the current sending info */
                    _VCR_gData.stCtrlInfo.eReqToBeSend = VCR_MSG_NONE;
                    _VCR_gData.stCtrlInfo.uNumRetry    = 0x0000;
                    _VCR_gData.stCtrlInfo.bOkPending   = BT_FALSE;
                    if (VCR_SYSTEM_NOT_AVAILABLE != _VCR_gData.eSystemState &&
                            VCR_SYSTEM_AVAILABLE != _VCR_gData.eSystemState    )
                    {   /* system is in a started state - stop, change to idle and
                           demute */
                        DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"Error - stop system");

                       _VCR_vSystemStopToIdle(BT_FALSE);
                    }
                }
                _VCR_gData.stCtrlInfo.uNumRetry= 0x0000;
            }
            else
            {   /* retry next time if retry count is not reached */
                retry = BT_TRUE;
            }
        }
    }
    else
    {
        /* write trace here */
    }
    /* now check for retry */
    if (BT_TRUE == retry)
    {
        if (VCR_MAX_UART_RETRIES > _VCR_gData.stCtrlInfo.uNumRetry)
        {
            // RT#1322
            // The time for further Retries should be the same as in other applications (10ms)
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(ERM_UART_BUSY_RETRY_TIME),   0);
            _VCR_gData.stCtrlInfo.uNumRetry++;
        }
        else
        {   /* do no further retries here!!! */
            DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"Discard Msg %d Retry %d ",
                _VCR_gData.stCtrlInfo.eReqToBeSend,
                _VCR_gData.stCtrlInfo.uNumRetry);

            _VCR_gData.stCtrlInfo.eReqToBeSend = VCR_MSG_NONE;
            _VCR_gData.stCtrlInfo.uNumRetry    = 0x0000;
            _VCR_gData.stCtrlInfo.bOkPending   = BT_FALSE;
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFMessageToBeSend                                       */
/**
    \brief     fill message send data with respective values

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType  _VCR_sFMessageToBeSend(VCRSystemMsgEnum msg_type)
{
    SuccessFailType sF = FAIL;
    /* check that no other message is in progress */
    if (VCR_MSG_NONE     == _VCR_gData.stCtrlInfo.eReqToBeSend &&
        VCR_MSG_NONE     !=  msg_type &&
        VCR_NUM_MESSAGES >   msg_type)
    {   /* we can queue the new message as previous is already send - do so */
        _VCR_gData.stCtrlInfo.eReqToBeSend    = msg_type;
        _VCR_gData.stCtrlInfo.uNumRetry       = 0x0000;
        //_VCR_gData.stCtrlInfo.bOkPending      = BT_FALSE;
        switch (msg_type)
        {
            case VCR_MSG_DIAL:
            case VCR_MSG_REDIAL:
            {
                _VCR_gData.stCtrlInfo.bIsAPPMsg = BT_TRUE;
                break;
            }
            default:
            {
                _VCR_gData.stCtrlInfo.bIsAPPMsg = BT_FALSE;
                break;
            }
        }
        sF = SUCCESS;
        /* trigger pending event */
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(10),   0);
    }
    else
    {
        DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"msg send failed - msg_type %d msg %d ",
                msg_type,_VCR_gData.stCtrlInfo.eReqToBeSend);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFSendAPPMessage                                        */
/**
    \brief      send message to other application module

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode  _VCR_sFSendAPPMessage(VCRSystemMsgEnum  msg_type)
{
    APP_COMMON_enResponseCode sF    = APP_COMMON_RSP_OK;
    BooleanType bDialInProgress     = BT_FALSE;
    Uint16Type i                    = 0;
    VCRResultUnionType * p_res      = &(_VCR_gData.resultInfo);

    ATReqAllType    req_msg;
    (void)memset(&req_msg,0x00,sizeof(ATReqAllType));

    bDialInProgress = bDialInProgress;

    switch (msg_type)
    {
        case VCR_MSG_DIAL:
        {   /* check which dial type to be used */
            switch (p_res->number.type)
            {
                case VCR_RESULT_TYPE_NUMBER:
                {
                    for (i = 0; i< p_res->number.number_len;i++)
                    {
                        if (p_res->number.number[i] != 0x20 &&
                            req_msg.callNumber.len < AT_MAX_PHONE_NUM_LEN)
                        {
                            req_msg.callNumber.number[req_msg.callNumber.len] = p_res->number.number[i];
                            req_msg.callNumber.len++;
                        }
                        /* else do nothing */
                    }
                    DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCR APP call APP_PHONE_rcDialNumber numberLen %d ", req_msg.callNumber.len);

                    sF = APP_PHONE_rcDialNumber(VCR_vCBSendMsgViaAPP,
                                                &(req_msg.callNumber.number[0]),
                                                req_msg.callNumber.len);
                    if( APP_COMMON_RSP_OK == sF )
                    {
                        bDialInProgress = BT_TRUE;
                    }
                    break;
                }
                case VCR_RESULT_TYPE_CONTACT_UNI:
                {
                    /* check if we call with index or number */
                    if (BT_TRUE == _VCR_gData.resultInfo.contact_uni.is_dial_index)
                    {
                        if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->contact_uni.type &&
                                AT_SIVR_ITEMTYPE_CONTACT == p_res->contact_uni.item_type  )
                        {
                            /* RT#1380 multiple numbers */
                            /* search phonebook index, that matches this type */
                            for (i = 0; ((i<p_res->contact_uni.n) && (i<VCR_MAXNUM_RES_STORE));i++)
                            {
                                if (_VCR_gData.resultInfo.contact_uni.location_id ==
                                    _VCR_gData.resultInfo.contact_uni.n_location[i])
                                {   /* match found - call with this index  */
                                    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"n_location %d->PPBP index=%d ", i,(i+1));
                                    break;
                                }
                            }
                             /* correct type - now dial with index */
                            /*
                                Problem: the function above searches the first entry, but in dialog ResolveMultipleNumCatEvent
                                we need maybe not the first entry with this category

                            */
                            /* SOP1-Last minute Fix*/

                            if ((_VCR_gData.eMenuState == AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY) || (_VCR_gData.eMenuState == AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY_L))
                            {

                                 sF = APP_PHONE_rcDialPnIndex(VCR_vCBSendMsgViaAPP,
                                                          p_res->contact_uni.item_id,p_res->contact_uni.current_num+1);

                                 DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR APP call APP_PHONE_rcDialPnIndex pbid %d idx %d (MultipleNumbersPerCat)",
                                         p_res->contact_uni.item_id,
                                         p_res->contact_uni.current_num+1);
                            }
                            else
                            {
                            sF = APP_PHONE_rcDialPnIndex(VCR_vCBSendMsgViaAPP,
                                                         p_res->contact_uni.item_id,i+1);

                                 DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VCR APP call APP_PHONE_rcDialPnIndex pbid %d pnidx %d",
                                         p_res->contact_uni.item_id,p_res->contact_uni.current_num+1);
                            }
                            if( APP_COMMON_RSP_OK == sF )
                            {
                                bDialInProgress = BT_TRUE;
                            }
                        }
                        else
                        {
                            /* PROBLEM - shall normally not occur here!!! */
                            DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VCR inco dial status");

                            sF = APP_COMMON_RSP_ERROR;
                        }
                    }
                    else
                    {
                        /* check if we call with item only or also with location */
                        if (AT_SIVR_ITEMTYPE_CONTACT == p_res->contact_uni.item_type)
                        {
                            sF = APP_PHONE_rcDialPbIndex(VCR_vCBSendMsgViaAPP,
                                                         p_res->contact_uni.item_id);

                            if( APP_COMMON_RSP_OK == sF )
                            {
                                bDialInProgress = BT_TRUE;
                            }
                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCR APP call APP_PHONE_rcDialPbIndex pbid %d",
                                    p_res->contact_uni.item_id);

                        }
                        else
                        {
                            /* PROBLEM - shall normally not occur here!!! */
                             DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VCR inco dial");

                            sF = APP_COMMON_RSP_ERROR;
                        }
                    }
                    break;
                }
                case VCR_RESULT_TYPE_CALLSTACKS:
                {
                    for (i = 0; i< p_res->cStack.phone_number_len;i++)
                    {
                        if (p_res->cStack.phone_number[i] != 0x20 &&
                            req_msg.callNumber.len < AT_MAX_PHONE_NUM_LEN)
                        {
                            req_msg.callNumber.number[req_msg.callNumber.len] = p_res->cStack.phone_number[i];
                            req_msg.callNumber.len++;
                        }
                        /* else do nothing */
                    }

                    sF = APP_PHONE_rcDialNumber(VCR_vCBSendMsgViaAPP,
                                                &(req_msg.callNumber.number[0]),
                                                req_msg.callNumber.len);

                    DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCR APP call APP_PHONE_rcDialNumber numberLen %d ", req_msg.callNumber.len);

                    if( APP_COMMON_RSP_OK == sF )
                    {
                        bDialInProgress = BT_TRUE;
                    }
                    break;
                }
                case VCR_RESULT_TYPE_NONE:
                default:
                {   /* PROBLEM - shall normally not occur here!!! */
                    DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VCR inco dial");

                    sF = APP_COMMON_RSP_ERROR;
                    break;
                }
            }

            break;
        }
        case VCR_MSG_REDIAL:
        {   /* call the redial function */
            sF = APP_PHONE_rcRedial(VCR_vCBSendMsgViaAPP);

            if( APP_COMMON_RSP_OK == sF )
            {
                bDialInProgress = BT_TRUE;
                DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"VCR APP call APP_PHONE_rcRedial");
            }
            else if (APP_COMMON_RSP_ERROR == sF)
            {
                 DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VCR APP call APP_PHONE_rcRedial -> Redial failed");
                 // ensure calling of Callback, otherwise VR stays active
                 // function in app phone was changed
                 VCR_vCBSendMsgViaAPP(APP_COMMON_RSP_ERROR);
            }
            break;
        }
        default:
        {
            /* unsupported message */
            sF = APP_COMMON_RSP_ERROR;
            break;
        }
    }
    /* if sending failed or was success, update control elements */
    if (APP_COMMON_RSP_OK == sF)
    {
        _VCR_gData.stCtrlInfo.eLastSend    = _VCR_gData.stCtrlInfo.eReqToBeSend;
        _VCR_gData.stCtrlInfo.eReqToBeSend = VCR_MSG_NONE;
        _VCR_gData.stCtrlInfo.bOkPending   = BT_TRUE;
        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"_VCR_sFSendAPPMessage succes msg_type %d req_type %d",
                msg_type,req_msg.rrec.common.req_type);

        /* SUSANNE: Restart Player after Calls Idle again is now handled by Audio Manager */
    }
    else
    {
        DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"_VCR_sFSendAPPMessage failed msg_type %d req_type %d",
                msg_type,req_msg.rrec.common.req_type);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFSendATMessage                                         */
/**
    \brief      send message to CK5050

    \brief      send message to CK5050

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType  _VCR_sFSendATMessage(const SCI_ST*     p_uart,
                                             VCRSystemMsgEnum  msg_type)
{
    SuccessFailType sF = SUCCESS;
    VcrProcessingMenuType   * p_menu   = &(_VCR_gData.stProcMenu);

    ATReqAllType    req_msg;
    (void)memset(&req_msg,0x00,sizeof(ATReqAllType));

    switch (msg_type)
    {
        case VCR_MSG_GET_LANG_NUM:
        {   /* fill message parameters */
            req_msg.rgsl.common.req_type = AT_REQ_SIVR_GET_SUPPORTED_LANG;
            req_msg.rgsl.is_get_lang     = AT_TRUE;
            req_msg.rgsl.index_used      = AT_FALSE;
            break;
        }
        case VCR_MSG_GET_LANG_MAP:
        {   /* fill message parameters */
            req_msg.rgsl.common.req_type = AT_REQ_SIVR_GET_SUPPORTED_LANG;
            req_msg.rgsl.is_get_lang     = AT_FALSE;
            req_msg.rgsl.index_used      = AT_FALSE;
            break;
        }
        case VCR_MSG_GET_CURRENT_LANG:
        {   /* fill message parameters */
            req_msg.rscl.common.req_type = AT_REQ_SIVR_SET_CURRENT_LANG;
            req_msg.rscl.is_get_current_lang = AT_TRUE;
            break;
        }
        case VCR_MSG_SET_CURRENT_LANG:
        {   /* fill message parameters */
            req_msg.rscl.common.req_type     = AT_REQ_SIVR_SET_CURRENT_LANG;
            req_msg.rscl.is_get_current_lang = AT_FALSE;
            if ((Uint8Type)AT_SIRV_LANG_MAXNUM != _VCR_gData.u8CurrentLangIdx)
            {
                req_msg.rscl.lang_index      = _VCR_gData.u8CurrentLangIdx;
            }
            else
            {   /* at least index 1 shall be available */
                req_msg.rscl.lang_index      = 0x01;
            }
            break;
        }
        case VCR_MSG_START_ENGINE:
        {   /* fill message parameters */
            req_msg.rrec.common.req_type = AT_REQ_SIVR_ENGINE_MGMT;
            req_msg.rrec.start_engine    = AT_TRUE;
            break;
        }
        case VCR_MSG_STOP_ENGINE:
        {   /* fill message parameters */
            req_msg.rrec.common.req_type = AT_REQ_SIVR_ENGINE_MGMT;
            req_msg.rrec.start_engine    = AT_FALSE;
            break;
        }
        case VCR_MSG_PREPARE_MENU:
        {   /* fill message parameters */
            req_msg.rpre.common.req_type = AT_REQ_SIVR_PREPARE_VR;
            /* we need to take into account long / short menu as the internal
             * machine only uses the short state */
            if (BT_TRUE == _VCR_gData.bVoiceFBOff)
            {
                req_msg.rpre.menu_id         = _VCR_gData.eMenuState;
            }
            else
            {
                req_msg.rpre.menu_id = (ATSIVRPromptEnum)(_VCR_gData.eMenuState
                                                 + AT_SIVR_LONG_MENU_OFFSET);
            }
            break;
        }
        case VCR_MSG_MUSIC_SYSSTART_PROMPT:
        {   /* fill message parameters */
            req_msg.rpmt.common.req_type    = AT_REQ_SIVR_MANAGE_PROMTS;
            req_msg.rpmt.start_play         = AT_TRUE;
            req_msg.rpmt.beep_after         = AT_FALSE;
            req_msg.rpmt.prmt.other.prompt_type  = AT_SIVR_PROMPT_SYSTEM;
            req_msg.rpmt.prmt.other.prompt_id    = _VCR_gData.ePromtTable.tab[_VCR_gData.u8NumPromptSend];
            /* build prompt parameters */
            sF = _VCR_sFBuildPrompt(&(req_msg.rpmt));
            break;
           }

        /* stop current played system prompt with beep */
        case VCR_MSG_RESTART_PROMPT_RECO:
        {
            req_msg.rpmt.common.req_type         = AT_REQ_SIVR_MANAGE_PROMTS;
            req_msg.rpmt.start_play              = AT_FALSE;
            req_msg.rpmt.prmt.custom.prompt_type = AT_SIVR_PROMPT_UNDEF;
            req_msg.rpmt.prmt.other.prompt_id    = 0;
            req_msg.rpmt.beep_after              = AT_TRUE;
            break;
        }
        /* stop current played system prompt without beep */
        case VCR_MSG_RESTART_PROMTP_NORECO:
        case VCR_MSG_STOP_RTPROMPT:
        case VCR_MSG_STOP_STR_PRMT_PTT:
        case VCR_MSG_STOP_STR_PRMT_CALL:
        case VCR_MSG_STOP_STR_PRMT_OK:
        case VCR_MSG_STOP_STR_PRMT_RESTART:
        {
            req_msg.rpmt.common.req_type         = AT_REQ_SIVR_MANAGE_PROMTS;
            req_msg.rpmt.start_play              = AT_FALSE;
            req_msg.rpmt.prmt.custom.prompt_type = AT_SIVR_PROMPT_UNDEF;
            req_msg.rpmt.prmt.other.prompt_id    = 0;
            req_msg.rpmt.beep_after              = AT_FALSE;
            break;
        }
        case VCR_MSG_START_RECO:
        {
            req_msg.rpmt.common.req_type    = AT_REQ_SIVR_MANAGE_PROMTS;
            req_msg.rpmt.start_play         = AT_TRUE;
            req_msg.rpmt.beep_after         = AT_TRUE;
            if (AT_SIVR_PROMPT_MENU == p_menu->ePromptType ||
                AT_SIVR_PROMPT_HELP == p_menu->ePromptType    )
            {
                if (BT_TRUE == _VCR_gData.bVoiceFBOff)
                {
                    req_msg.rpmt.prmt.other.prompt_id = (Uint16Type)p_menu->ePromptId;
                }
                else
                {   /* use the long menu */

                    req_msg.rpmt.prmt.other.prompt_id =
                              (Uint16Type)p_menu->ePromptId
                                                  + (Uint16Type)AT_SIVR_LONG_MENU_OFFSET;

                }
            }
            else
            {
                req_msg.rpmt.prmt.other.prompt_id   = (Uint16Type)p_menu->ePromptId;
            }
            req_msg.rpmt.prmt.other.prompt_type = p_menu->ePromptType;
            /* build prompt parameters */
            sF = _VCR_sFBuildPrompt(&(req_msg.rpmt));
            break;
        }
        case VCR_MSG_PLAY_PROMPT_MENU:
        {
            req_msg.rpmt.common.req_type    = AT_REQ_SIVR_MANAGE_PROMTS;
            req_msg.rpmt.start_play         = AT_TRUE;
            req_msg.rpmt.beep_after         = AT_FALSE;
            req_msg.rpmt.prmt.other.prompt_type = AT_SIVR_PROMPT_MENU;
            if (BT_TRUE == _VCR_gData.bVoiceFBOff)
            {
                req_msg.rpmt.prmt.other.prompt_id   =
                    _VCR_gData.ePromtTable.tab[_VCR_gData.u8NumPromptSend];
            }
            else
            {   /* use the long menu */

                req_msg.rpmt.prmt.other.prompt_id =
                         (Uint16Type)_VCR_gData.ePromtTable.tab[_VCR_gData.u8NumPromptSend]
                       + (Uint16Type)AT_SIVR_LONG_MENU_OFFSET;

            }
            /* build prompt parameters */
            sF = _VCR_sFBuildPrompt(&(req_msg.rpmt));
            break;
        }
        case VCR_MSG_PLAY_PROMPT:
        case VCR_MSG_PLAY_SYSSTOP_PROMPT:
        case VCR_MSG_PLAY_SYSSTART_PROMPT:
        case VCR_MSG_PLAY_REDIAL_PROMPT:
        case VCR_MSG_PLAY_CONNECT_PROMPT:
        case VCR_MSG_INFO_SYSSTART_PROMPT:  //RT13214
        {
            req_msg.rpmt.common.req_type    = AT_REQ_SIVR_MANAGE_PROMTS;
            req_msg.rpmt.start_play         = AT_TRUE;
            req_msg.rpmt.beep_after         = AT_FALSE;
            req_msg.rpmt.prmt.other.prompt_type  = AT_SIVR_PROMPT_SYSTEM;
            req_msg.rpmt.prmt.other.prompt_id    = _VCR_gData.ePromtTable.tab[_VCR_gData.u8NumPromptSend];
            /* build prompt parameters */
            sF = _VCR_sFBuildPrompt(&(req_msg.rpmt));
            break;
        }
        case VCR_MSG_DELETE_CALL_STACKS:
        {
            req_msg.common.req_type    = AT_REQ_PB_CALL_HISTORY;
            req_msg.ppch.action        = AT_PB_CALLHIST_DEL_FLASH;
            break;
        }
        case VCR_MSG_NAME_VOICE_TAG_ADD:
        {
            req_msg.common.req_type    = AT_REQ_SIVR_MANAGE_VOICE_TAG;
            req_msg.rvta.action        = AT_VTMGR_ACT_STORE;
            if (VCR_RESULT_TYPE_PLAYPB == _VCR_gData.resultInfo.playPB.type)
            {
                req_msg.rvta.identifier    = _VCR_gData.resultInfo.playPB.cur_entry;
                req_msg.rvta.type_avail    = AT_TRUE;
                req_msg.rvta.type          = AT_SIVR_ITEMTYPE_CONTACT;
            }
            else if (VCR_RESULT_TYPE_NUMBER == _VCR_gData.resultInfo.number.type)
            {
                req_msg.rvta.identifier    = _VCR_gData.resultInfo.number.contact_idx;
                req_msg.rvta.type_avail    = AT_FALSE;
            }
            else
            {
                DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"VCT VTAdd inco type %d",
                        _VCR_gData.resultInfo.playPB.type);
                sF = FAIL;
            }
            break;
        }
        case VCR_MSG_NAME_VOICE_TAG_DELETE:
        {
            req_msg.common.req_type    = AT_REQ_SIVR_MANAGE_VOICE_TAG;
            req_msg.rvta.action        = AT_VTMGR_ACT_DELETE;
            req_msg.rvta.identifier    = _VCR_gData.resultInfo.playPB.cur_entry;
            req_msg.rvta.type_avail    = AT_TRUE;
            req_msg.rvta.type          = AT_SIVR_ITEMTYPE_CONTACT;
            break;
        }
        case VCR_MSG_NAME_VOICE_TAG_DELETE_ALL:
        {
            req_msg.common.req_type    = AT_REQ_SIVR_MANAGE_VOICE_TAG;
            req_msg.rvta.action        = AT_VTMGR_ACT_DELETE_ALL;
            req_msg.rvta.identifier    = _VCR_gData.resultInfo.playPB.cur_entry;
            req_msg.rvta.type_avail    = AT_TRUE;
            req_msg.rvta.type          = AT_SIVR_ITEMTYPE_CONTACT;
            break;
        }
        case VCR_MSG_NAME_VOICE_TAG_QUERY:
        {
            req_msg.common.req_type    = AT_REQ_SIVR_MANAGE_VOICE_TAG;
            req_msg.rvta.action        = AT_VTMGR_ACT_RETRIEVE;
            req_msg.rvta.identifier    = _VCR_gData.resultInfo.playPB.cur_entry;
            req_msg.rvta.type_avail    = AT_TRUE;
            req_msg.rvta.type          = AT_SIVR_ITEMTYPE_CONTACT;
            break;
        }
        default:
        {
            /* unsupported message */
            sF = FAIL;
            break;
        }
    }
    if (SUCCESS == sF)
    {
         sF = AT_Cmd_Handler_VCR(&req_msg,p_uart);
    }
    /* if sending failed or was success, update control elements */
    if (SUCCESS == sF)
    {
        _VCR_gData.stCtrlInfo.eLastSend    = _VCR_gData.stCtrlInfo.eReqToBeSend;
        _VCR_gData.stCtrlInfo.eReqToBeSend = VCR_MSG_NONE;
        _VCR_gData.stCtrlInfo.bOkPending   = BT_TRUE;
        DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"SendAT succes msg_type %d req_type %d",
                msg_type,req_msg.rrec.common.req_type);

        /* start OK supervision */
        _VCR_vStartSupervision(VCR_SUP_OK);
    }
    else
    {
        DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"SendAT failed msg_type %d req_type %d -> Release UART",
                msg_type,req_msg.rrec.common.req_type);
        /* in addition release the UART pointer */
        (void)ERM_ReleaseUartForVCR();
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vPrepareASRCmdMessage                                   */
/**
    \brief      prepare ASR command message and if prep done, call send

    \brief      prepare ASR command message and if prep done, call send

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vPrepareASRCmdMessage(void)
{
    VCRPerfMeasCmdValueType * p_ASRCmdMsg = &_VCR_gData.stASRCmdMsg;
    BooleanType send_trigger = BT_FALSE;
    BooleanType fetch_pb     = BT_FALSE;
    Uint16Type i = 0;

    if (NULL != p_ASRCmdMsg)
    {   /* fill data from result storage to command element */
        VcrProcessingResultType * p_res = &(_VCR_gData.stMsgRes);
        p_ASRCmdMsg->num_cmd = p_res->u8NbrResults;
        for (i = _VCR_gData.u16PerfMeasStoreCurCnt; i < p_res->u8NbrResults ; i++)
        {   /* later on we need to check here if fetch of PB data is required,
               if so we need to recall this method */
            p_ASRCmdMsg->info[i].cmd_id          = p_res->stResults[i].result_value;
            p_ASRCmdMsg->info[i].confidence_rate = p_res->stResults[i].confidence_rate;
            /* evaluate out of option type */
            switch (p_res->stResults[i].u.nb.type)
            {
                case AT_SIVR_RES_NUMBER:
                {
                    p_ASRCmdMsg->info[i].num_sub_cmd     = 1;
                    /* copy given string and that's it */
                    if (AST_SUB_TEXT_MAX_LEN >= p_res->stResults[i].u.nb.number_len)
                    {
                        p_ASRCmdMsg->info[i].sub_len[0] = p_res->stResults[i].u.nb.number_len;
                    }
                    else
                    {
                        p_ASRCmdMsg->info[i].sub_len[0] = AST_SUB_TEXT_MAX_LEN;
                    }
                    (void)memcpy(&(p_ASRCmdMsg->info[i].sub[0]),
                           &(p_res->stResults[i].u.nb.number),
                           p_ASRCmdMsg->info[i].sub_len[0]);
                    break;
                }
                case AT_SIVR_RES_IDs:
                {   /* we need to evaluate number of available options */
                    if (AT_TRUE == p_res->stResults[i].u.id.id1_available)
                    {
                        /* now we need to take into account cmd id to check if
                         * we need to query e.g. phonebook data */
                        switch (p_ASRCmdMsg->info[i].cmd_id)
                        {   /* add all cases where we need to fetch PB info */
                            case AT_SIVR_CMD_DIAL:
                            case AT_SIVR_CMD_PAB_NAME_PLAY_ADDBOOK:
                            {   /* commands with name and optional location */
                                if (AT_TRUE == p_res->stResults[i].u.id.id2_available)
                                {
                                    p_ASRCmdMsg->info[i].num_sub_cmd     = 2;
                                    switch (p_res->stResults[i].u.id.id2)
                                    {
                                        case AT_LOC_HOME:
                                        {
                                            (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[1][0]),
                                                    AST_SUB_TEXT_MAX_LEN,"HOME");
                                            break;
                                        }
                                        case AT_LOC_OFFICE:
                                        {
                                            (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[1][0]),
                                                    AST_SUB_TEXT_MAX_LEN,"OFFICE");
                                            break;
                                        }
                                        case AT_LOC_MOBILE:
                                        {
                                            (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[1][0]),
                                                    AST_SUB_TEXT_MAX_LEN,"MOBILE");
                                            break;
                                        }
                                        case AT_LOC_OTHER:
                                        {
                                            (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[1][0]),
                                                    AST_SUB_TEXT_MAX_LEN,"OTHER");
                                            break;
                                        }
                                        case AT_LOC_GENERAL:
                                        default:
                                        {
                                            (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[1][0]),
                                                    AST_SUB_TEXT_MAX_LEN,"GENERAL");
                                            break;
                                        }
                                    }
                                    p_ASRCmdMsg->info[i].sub_len[1] =
                                        (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[i].sub[1]);
                                    p_ASRCmdMsg->info[i].sub[1][p_ASRCmdMsg->info[i].sub_len[1]] = '\0';
                                }
                                else
                                {
                                    p_ASRCmdMsg->info[i].num_sub_cmd     = 1;
                                }
                                /* we need to fetch the name from the phonebook */
                                fetch_pb = BT_TRUE;
                                _VCR_gData.u16PerfMeasStoreCurCnt = i;
                                _VCR_gData.u16CurPbIndex = p_res->stResults[i].u.id.id1;
                                if (SUCCESS != _VCR_vASRGetPbInfo())
                                {   /* problem to fetch - copy only number */
                                    (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                            AST_SUB_TEXT_MAX_LEN,"%d",p_res->stResults[i].u.id.id1);
                                    p_ASRCmdMsg->info[i].sub_len[0] =
                                        (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[i].sub[0]);
                                    p_ASRCmdMsg->info[i].sub[0][p_ASRCmdMsg->info[i].sub_len[0]] = '\0';
                                    fetch_pb = BT_FALSE;
                                }
                                (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                        AST_SUB_TEXT_MAX_LEN,"%d",p_res->stResults[i].u.id.id1);
                                p_ASRCmdMsg->info[i].sub_len[0] =
                                    (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[i].sub[0]);
                                p_ASRCmdMsg->info[i].sub[0][p_ASRCmdMsg->info[i].sub_len[0]] = '\0';
                                break;
                            }
                            case AT_SIVR_CMD_CATEGORY:
                            {   /* map the category only - here only one param needed */
                                p_ASRCmdMsg->info[i].num_sub_cmd     = 1;
                                switch (p_res->stResults[i].u.id.id1)
                                {
                                    case AT_LOC_HOME:
                                    {
                                        (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                                AST_SUB_TEXT_MAX_LEN,"HOME");
                                        break;
                                    }
                                    case AT_LOC_OFFICE:
                                    {
                                        (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                                AST_SUB_TEXT_MAX_LEN,"OFFICE");
                                        break;
                                    }
                                    case AT_LOC_MOBILE:
                                    {
                                        (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                                AST_SUB_TEXT_MAX_LEN,"MOBILE");
                                        break;
                                    }
                                    case AT_LOC_OTHER:
                                    {
                                        (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                                AST_SUB_TEXT_MAX_LEN,"OTHER");
                                        break;
                                    }
                                    case AT_LOC_GENERAL:
                                    default:
                                    {
                                        (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                                AST_SUB_TEXT_MAX_LEN,"GENERAL");
                                        break;
                                    }
                                }
                                p_ASRCmdMsg->info[i].sub_len[0] =
                                    (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[i].sub[0]);
                                p_ASRCmdMsg->info[i].sub[0][p_ASRCmdMsg->info[i].sub_len[0]] = '\0';
                                break;
                            }
                            default:
                            {   /* for all others this is inco matching - only copy ids*/
                                p_ASRCmdMsg->info[i].num_sub_cmd     = 1;
                                (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[0][0]),
                                        AST_SUB_TEXT_MAX_LEN,"%d",p_res->stResults[i].u.id.id1);
                                p_ASRCmdMsg->info[i].sub_len[0] =
                                    (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[i].sub[0]);
                                p_ASRCmdMsg->info[i].sub[0][p_ASRCmdMsg->info[i].sub_len[0]] = '\0';
                                if (AT_TRUE == p_res->stResults[i].u.id.id2_available)
                                {
                                    p_ASRCmdMsg->info[i].num_sub_cmd     = 2;
                                    (void)snprintf((char*)&(p_ASRCmdMsg->info[i].sub[1][0]),
                                            AST_SUB_TEXT_MAX_LEN,"%d",p_res->stResults[i].u.id.id2);
                                    p_ASRCmdMsg->info[i].sub_len[1] =
                                        (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[i].sub[1]);
                                    p_ASRCmdMsg->info[i].sub[1][p_ASRCmdMsg->info[i].sub_len[1]] = '\0';
                                }
                                break;
                            }
                        }
                    }
                    else
                    {   /* shall normally not occur!! set num sub to 0 */
                        p_ASRCmdMsg->info[i].num_sub_cmd     = 0;
                    }
                    break;
                }
                case AT_SIVR_RES_NONE:
                default:
                {
                    p_ASRCmdMsg->info[i].num_sub_cmd     = 0;
                    break;
                }
            }
            if (BT_TRUE == fetch_pb)
            {   /* stop for loop, remind the current index */
                break;
            }
        }
        if ( BT_FALSE == fetch_pb )
        {   /* all results filled into the buffer - send message */
            _VCR_vSendASRMessage(VCR_PERFSTAT_CMD);
            /* trigger main machine */
            send_trigger = BT_TRUE;
        }
    }
    else
    {   /* trigger main machine */
        send_trigger = BT_TRUE;
    }

    /* we need to trigger main machine */
    if (BT_TRUE == send_trigger)
    {
        (void)_VCR_sFHandleEvent(_VCR_gData.bPerfMeasStoreIsEvent,
                           _VCR_gData.ePerfMeasStoreEvent,
                           _VCR_gData.ePerfMeasStoreCmd);

        /* set back global storage */
        (void)memset(&_VCR_gData.stASRCmdMsg,0x00,sizeof(_VCR_gData.stASRCmdMsg));
        _VCR_gData.bPerfMeasStoreIsEvent  = BT_FALSE;
        _VCR_gData.ePerfMeasStoreEvent    = VCR_MEVT_UNDEF;
        _VCR_gData.ePerfMeasStoreCmd      = AT_SIVR_CMD_UNKNOWN;
        _VCR_gData.u16PerfMeasStoreCurCnt = 0;
        _VCR_gData.u16CurPbIndex          = 0;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vASRGetPbInfo                                           */
/**
    \brief      get PB info for ASR

    \brief      get PB info for ASR

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_vASRGetPbInfo(void)
{
    APP_COMMON_enResponseCode appRsp = APP_COMMON_RSP_OK;
    SuccessFailType sF = SUCCESS;

    appRsp = APP_PHONEBOOK_rcRequestCompletePbEntries(
            VCR_vCBGetPBInfoASR,
            APP_PHONEBOOK_enUsersPB,
            _VCR_gData.u16CurPbIndex,1);
    switch (appRsp)
    {
        case APP_COMMON_RSP_OK:
        {   /* do nothing but wait for callback */
            break;
        }
        case APP_COMMON_RSP_ERROR_BUSY:
        {   /* we need to retry - send trigger event in here */
            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_ASR_RETRIGGER);
            break;
        }
        case APP_COMMON_RSP_ERROR:
        case APP_COMMON_RSP_ERROR_BAP_PARSE:
        case APP_COMMON_RSP_ERROR_UART_BLOCKED:
        case APP_COMMON_RSP_ERROR_AT_SEND:
        case APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT:
        case APP_COMMON_RSP_ERROR_NOT_INITIALIZED:
        default:
        {   /* an error occured caller will proceed and only copy number */
            sF = FAIL;
            break;
        }
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : VCR_vCBGetPBInfoASR                                          */
/**
    \brief      callback for ASR PB query

    \brief      callback for ASR PB query

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void VCR_vCBGetPBInfoASR(APP_COMMON_enResponseCode response)
{
    Uint32Type idx = _VCR_gData.u16PerfMeasStoreCurCnt;
    VCRPerfMeasCmdValueType * p_ASRCmdMsg = &_VCR_gData.stASRCmdMsg;

    const AppPbEntryType * p_pb_entry = NULL;

    switch (response)
    {
        case APP_COMMON_RSP_OK:
        {   /* query the required information from APP PB */
            p_pb_entry = APP_PHONEBOOK_pGetPpbrContext(0);
            if (NULL != p_pb_entry)
            {   /* check entry is valid*/
                if (BT_TRUE == p_pb_entry->isValid)
                {   /* copy caller name */
                    p_ASRCmdMsg->info[idx].sub_len[0] = p_pb_entry->ppbr.name_len;
                    if (AST_SUB_TEXT_MAX_LEN <=  p_ASRCmdMsg->info[idx].sub_len[0])
                    {
                        p_ASRCmdMsg->info[idx].sub_len[0] = AST_SUB_TEXT_MAX_LEN;
                    }
                    /* copy name */
                    (void)memcpy( &(p_ASRCmdMsg->info[idx].sub[0][0]),
                            &(p_pb_entry->ppbr.name[0]),
                            p_ASRCmdMsg->info[idx].sub_len[0]);
                    p_ASRCmdMsg->info[idx].sub[0][p_ASRCmdMsg->info[idx].sub_len[0]] = '\0';
                }
                else
                {   /* problem to fetch - copy only number */
                    (void)snprintf((char*)&(p_ASRCmdMsg->info[idx].sub[0][0]),
                            AST_SUB_TEXT_MAX_LEN,"%d",_VCR_gData.u16CurPbIndex);
                    p_ASRCmdMsg->info[idx].sub_len[0] =
                        (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[idx].sub[0]);
                    p_ASRCmdMsg->info[idx].sub[0][p_ASRCmdMsg->info[idx].sub_len[0]] = '\0';
                }
            }
            else
            {   /* problem to fetch - copy only number */
                (void)snprintf((char*)&(p_ASRCmdMsg->info[idx].sub[0][0]),
                        AST_SUB_TEXT_MAX_LEN,"%d",_VCR_gData.u16CurPbIndex);
                p_ASRCmdMsg->info[idx].sub_len[0] =
                    (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[idx].sub[0]);
                p_ASRCmdMsg->info[idx].sub[0][p_ASRCmdMsg->info[idx].sub_len[0]] = '\0';
            }
            break;
        }
        case APP_COMMON_RSP_ERROR_BUSY:
        case APP_COMMON_RSP_ERROR:
        case APP_COMMON_RSP_ERROR_BAP_PARSE:
        case APP_COMMON_RSP_ERROR_UART_BLOCKED:
        case APP_COMMON_RSP_ERROR_AT_SEND:
        case APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT:
        case APP_COMMON_RSP_ERROR_NOT_INITIALIZED:
        default:
        {   /* problem to fetch - copy only number */
            (void)snprintf((char*)&(p_ASRCmdMsg->info[idx].sub[0][0]),
                    AST_SUB_TEXT_MAX_LEN,"%d",_VCR_gData.u16CurPbIndex);
            p_ASRCmdMsg->info[idx].sub_len[0] =
                (Uint8Type)strlen((const char *)p_ASRCmdMsg->info[idx].sub[0]);
            p_ASRCmdMsg->info[idx].sub[0][p_ASRCmdMsg->info[idx].sub_len[0]] = '\0';
            break;
        }
    }
    _VCR_gData.u16PerfMeasStoreCurCnt = _VCR_gData.u16PerfMeasStoreCurCnt + 1;
    /* call ASP prep again */
    _VCR_vPrepareASRCmdMessage();
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSendASRMessage                                         */
/**
    \brief      build and send ASR message to CAN driver

    \brief      build and send ASR message to CAN driver

    \param      VCRPerfMeasStatusTypeEnum type - indicate the type to send
                VCRPerfMeasCmdValueType * p_cmd - pointer to command buffer or NULL

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSendASRMessage(VCRPerfMeasStatusTypeEnum type)
{
    VCRPerfMeasCmdValueType * p_cmd = NULL;
    BooleanType  bsend = BT_TRUE;
    StatusType result = E_OK;
//    Uint32Type i = 0;
//    Uint32Type j = 0;
    VCRPerfMeasStatusMsgType msg;

    (void)memset(&msg,0x00,sizeof(VCRPerfMeasStatusMsgType));

    switch (type)
    {
        case VCR_PERFSTAT_STATUS:
        {   /* prepare status message */
            msg.u.status.type = VCR_PERFSTAT_STATUS;
            if (VCR_SYSTEM_NOT_AVAILABLE == _VCR_gData.eSystemState ||
                VCR_SYSTEM_AVAILABLE     == _VCR_gData.eSystemState    )
            {   /* system not active */
                msg.u.status.status  = VCR_PERFSIVR_STATUS_INACTIVE;
                msg.u.status.menu_id = AT_SIVR_MENU_IDLE;
            }
            else
            {   /* SIVR is active, we need to check current engine state */
                if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                {
                    msg.u.status.status = VCR_PERFSIVR_STATUS_ACTIVE_RECO;
                }
                else
                {
                    msg.u.status.status = VCR_PERFSIVR_STATUS_ACTIVE_NORECO;
                }
            }
            /* fill current menu id */
            if (AT_SIVR_MENU_IDLE == _VCR_gData.eMenuState)
            {
                msg.u.status.menu_id = AT_SIVR_MENU_IDLE;
            }
            else
            {
                if (BT_TRUE == _VCR_gData.bVoiceFBOff)
                {
                    msg.u.status.menu_id = _VCR_gData.eMenuState;
                }
                else
                {   /* use the long menu */

                  msg.u.status.menu_id =
                              (ATSIVRPromptEnum)(   _VCR_gData.eMenuState
                                                  + AT_SIVR_LONG_MENU_OFFSET);

                }
            }
            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"_VCR_vSendASRMessage reco_status %d menu_id %d",
                    msg.u.status.status,msg.u.status.menu_id);
            break;
        }
        case VCR_PERFSTAT_CMD:
        {   /* prepare command message */
            msg.u.cmd.type = VCR_PERFSTAT_CMD;
            if (SUCCESS != POOL_sfAllocate(POOL_enVCRPerfMeasCmdType,
                    (void**)&(p_cmd)))
            {
                bsend = BT_FALSE; /* do not send */
                DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"ASR command pool alloc failed");
             }
            else
            {
                (void)memcpy(p_cmd,&(_VCR_gData.stASRCmdMsg),sizeof(VCRPerfMeasCmdValueType));
                msg.u.cmd.cmd  = p_cmd;

                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"ASR command num_cmd %d cmd_id %d",
                        p_cmd->num_cmd,p_cmd->info[0].cmd_id);
            }


            break;
        }
        default:
        {   /* unsupported type - do nothing */
            bsend = BT_FALSE;
            DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"ASR unsup type %d",type);
        }
    }

    if (BT_TRUE == bsend)
    {   /* send the message to CAN driver */
        result = SendMessage(msgHostTaskASRStatus, &msg);
    }
    if ( NULL != p_cmd &&
         (E_OK != result || BT_FALSE == bsend)
       )
    {   /* release element */
        DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"ASR free pool");
        (void)POOL_sfFree(p_cmd);
    }
}



/* End Of File VCR_MessageHandler.c */
