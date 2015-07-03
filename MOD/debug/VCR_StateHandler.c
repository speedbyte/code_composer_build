/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_StateHandler.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-09-21 09:41:46 +0200 (Mi, 21 Sep 2011) $
* $Rev: 21861 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_StateHandler.c $
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
#include "APP_SD.h"
#include "APP_MDI.h"
#include "APP_MDI_AT.h"
#include "APP_DEVICE_AudioManager.h"
#include "VCR_StateHandler.id"
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
static SuccessFailType _VCR_sfHandleEventGeneric(
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);
static SuccessFailType _VCR_sfHandleIdleEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);
static SuccessFailType _VCR_sfHandleMainMenuEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

static SuccessFailType _VCR_eHandle_VCRSystemStartedOverPTT(void);

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
/* Function    : _VCR_sFHandleEvent                                           */
/**
    \brief      main event handler machine

    \brief

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
/*
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='UNDEF                       '   DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='SIVR_SUPPORTED              '   DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='SIVR_STARTED                '   DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='SIVR_STOPPED                '   DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='LANGNUM_AVAIL               '   DEBUG_STATE_NUMBER='4'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='LANGMAP_AVAIL               '   DEBUG_STATE_NUMBER='5'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CUR_LANG_AVAIL              '   DEBUG_STATE_NUMBER='6'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CUR_LANG_CHANGED            '   DEBUG_STATE_NUMBER='7'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='LANG_CHANG_REG              '   DEBUG_STATE_NUMBER='8'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='STRING_PRMT_OK              '   DEBUG_STATE_NUMBER='9'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='START_PROMTP_END            '   DEBUG_STATE_NUMBER='10'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='STOP_PROMPT_END             '   DEBUG_STATE_NUMBER='11'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CONNECT_PROMTP_END          '   DEBUG_STATE_NUMBER='12'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='PROC_ERROR                  '   DEBUG_STATE_NUMBER='13'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='PTT                         '   DEBUG_STATE_NUMBER='14'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CALL                        '   DEBUG_STATE_NUMBER='15'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='OK                          '   DEBUG_STATE_NUMBER='16'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='VRSTARTED                   '   DEBUG_STATE_NUMBER='17'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='DIAL_OK                     '   DEBUG_STATE_NUMBER='18'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CK5050_DOWN                 '   DEBUG_STATE_NUMBER='19'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='ON_MUTE_DELAY               '   DEBUG_STATE_NUMBER='20'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='MDI_PAUSED                  '   DEBUG_STATE_NUMBER='21'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='PREP_OK                     '   DEBUG_STATE_NUMBER='22'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='REDIAL_PROMPT_END           '   DEBUG_STATE_NUMBER='23'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='REDIAL_DONE                 '   DEBUG_STATE_NUMBER='24'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='ERROR                       '   DEBUG_STATE_NUMBER='25'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='PBENTRY_DATA_AVAIL          '   DEBUG_STATE_NUMBER='26'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='APP_TRIG_PB_QUERY           '   DEBUG_STATE_NUMBER='27'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='READ_SMS                    '   DEBUG_STATE_NUMBER='28'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='SMS_INFO_AVAIL              '   DEBUG_STATE_NUMBER='29'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='MANUAL_PB_SYNC              '   DEBUG_STATE_NUMBER='30'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='MANUAL_PB_SYNC_OK           '   DEBUG_STATE_NUMBER='31'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='DELAY_LIST_READ             '   DEBUG_STATE_NUMBER='32'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='STR_PRMT_END_PTT            '   DEBUG_STATE_NUMBER='33'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='STR_PRMT_END_CALL           '   DEBUG_STATE_NUMBER='34'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='STR_PRMT_END_OK             '   DEBUG_STATE_NUMBER='35'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='PRMT_END_RESTART            '   DEBUG_STATE_NUMBER='36'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='APP_TRIGGER                 '   DEBUG_STATE_NUMBER='37'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='NAME_RECORDED               '   DEBUG_STATE_NUMBER='38'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='NAME_RECORD_FAIL            '   DEBUG_STATE_NUMBER='39'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='NAME_RECORD_FAIL_SHORT      '   DEBUG_STATE_NUMBER='40'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='NAME_RECORD_FAIL_NOT_UNIQUE '   DEBUG_STATE_NUMBER='41'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='VT_ADD_DONE                 '   DEBUG_STATE_NUMBER='42'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='VT_DELETE_DONE              '   DEBUG_STATE_NUMBER='43'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='VT_QUERY_DONE               '   DEBUG_STATE_NUMBER='44'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='HAPTIC                      '   DEBUG_STATE_NUMBER='45'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='INCOMING_CALL               '   DEBUG_STATE_NUMBER='46'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='REC_TRAINING_INFO           '   DEBUG_STATE_NUMBER='47'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='TRAINING_REJECT             '   DEBUG_STATE_NUMBER='48'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='TRAINING_ACCEPT             '   DEBUG_STATE_NUMBER='49'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='TRAINING_FINISH             '   DEBUG_STATE_NUMBER='50'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='ADDCONTACT_NUMBER           '   DEBUG_STATE_NUMBER='51'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='SELECT_ENTRY                '   DEBUG_STATE_NUMBER='52'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='MARK_SMS_READ               '   DEBUG_STATE_NUMBER='53'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='ASR_RETRIGGER               '   DEBUG_STATE_NUMBER='54'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='OUTGOING_CALL               '   DEBUG_STATE_NUMBER='55'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CK5050_POWER_OFF            '   DEBUG_STATE_NUMBER='56'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='REQUEST_MUTE                '   DEBUG_STATE_NUMBER='57'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='WAIT_MUTE_ACK               '   DEBUG_STATE_NUMBER='58'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='MUTE_ACK_START_ENGINE       '   DEBUG_STATE_NUMBER='59'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='VT_DELETE_ALL_DONE          '   DEBUG_STATE_NUMBER='60'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='APP_TRIG_PB_SEARCH_ID       '   DEBUG_STATE_NUMBER='61'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='APP_PB_ID_AVAIL             '   DEBUG_STATE_NUMBER='62'
DEBUG_STATE_ENUM_NAME='VCRMenuEventEnum'   DEBUG_STATE_NAME='CALL_PB_SELECTION           '   DEBUG_STATE_NUMBER='63'


*/
SuccessFailType  _VCR_sFHandleEvent(BooleanType      is_event,
                                    VCRMenuEventEnum event,
                                    ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType  sF = SUCCESS;
    BooleanType      event_handled = BT_FALSE;
    ATSIVRPromptEnum  old_state = _VCR_gData.eMenuState;
    BooleanType      process = BT_TRUE;


    /* check if the machine has an event to itself via _VCR_gData.eEventTrigMain */
    if (BT_TRUE == is_event && VCR_MEVT_UNDEF == event)
    {
        if (VCR_MEVT_UNDEF != _VCR_gData.eEventTrigMain )
        {
            DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"internal Event %d MenuState %d",
                  _VCR_gData.eEventTrigMain,_VCR_gData.eMenuState);

            event = _VCR_gData.eEventTrigMain;
            _VCR_gData.eEventTrigMain = VCR_MEVT_UNDEF;
        }
    }
    else
    {   /* check pending command */
        if (AT_SIVR_CMD_UNKNOWN != _VCR_gData.eCmdTrigMain )
        {
            DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"internal Command %d MenuState %d",
                   _VCR_gData.eCmdTrigMain,_VCR_gData.eMenuState);

            cmd = _VCR_gData.eCmdTrigMain;
            _VCR_gData.eCmdTrigMain = AT_SIVR_CMD_UNKNOWN;
        }
    }

    /* in some special state we need only to accept specific events and no
     * commands at all - filter out here!!! */
    switch (_VCR_gData.eSystemState)
    {
        case VCR_SYSTEM_STOP_PTT:
        {   /* ignore all commands */
        	if (BT_FALSE == is_event)
        	{
        		process = BT_FALSE;
        	}
        	else
        	{   /* only allow some events */
        		switch (event)
        		{
        		    case VCR_MEVT_SIVR_STARTED:
        		    case VCR_MEVT_SIVR_STOPPED:
        		    case VCR_MEVT_PTT:
        		    case VCR_MEVT_STOP_PROMPT_END:
        		    {   /* allowed - use default set for process */
        		    	break;
        		    }
        		    /* all others will be ignored */
	        		default:
	        		{
	        			process = BT_FALSE;
	        			break;
	        		}
        		}
        	}
        	break;
        }
        default:
        {   /* do nothing and process via default setting of process */
        	break;
        }
    }


    /* now do the processing if still valid */
    if (BT_TRUE == process)
    {
    	switch (_VCR_gData.eMenuState)
        {
            case    AT_SIVR_MENU_IDLE:
            {
                sF = _VCR_sfHandleIdleEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case    AT_SIVR_MENU_MAIN:
            case    AT_SIVR_MENU_MAIN_L:
            {
                sF = _VCR_sfHandleMainMenuEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case    AT_SIVR_MENU_DIALNAMECONFIRM:
            case    AT_SIVR_MENU_DIALNAMECONFIRM_L:
            {
                sF = _VCR_sfHandleDialNameConfEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case    AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES:
            case    AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES_L:
            {
                sF = _VCR_sfHandleDialNameConfCatEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case    AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES:
            case    AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES_L:
            {
                sF = _VCR_sfHandleDialNameResAmbiEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case    AT_SIVR_MENU_DIALNUMBER:
            case    AT_SIVR_MENU_DIALNUMBER_L:
            {
                sF = _VCR_sfHandleDialNumberEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case    AT_SIVR_MENU_DIALNUMBERCORRECT:
            case    AT_SIVR_MENU_DIALNUMBERCORRECT_L:
            {
                sF = _VCR_sfHandleDialNumberCorrectEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }
            case AT_SIVR_MENU_MUSIC:
            case AT_SIVR_MENU_MUSIC_L:
            {
                sF = _VCR_sfHandleMusicEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_MOREOPTIONS:
            case AT_SIVR_MENU_MOREOPTIONS_L:
            {
                sF = _VCR_sfHandleMoreOptionsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_PLAYADDRESSBOOK:
            case AT_SIVR_MENU_PLAYADDRESSBOOK_L:
            {
                sF = _VCR_sfHandlePlayAddressbookEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_PLAYADDRESSBOOKENTRY:
            case AT_SIVR_MENU_PLAYADDRESSBOOKENTRY_L:
            {
                sF = _VCR_sfHandlePlayAddressbookEntryEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS:
            case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS_L:
            {
                sF = _VCR_sfHandlePlayAddressbookOptionsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY:
            case AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY_L:
            {
                sF = _VCR_sfHandleResolveMultipleNumCatEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_RESETNAMECONFIRM:
            case AT_SIVR_MENU_RESETNAMECONFIRM_L:
            {
                sF = _VCR_sfHandleResetNameConfirmEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CALLSTACKS:
            case AT_SIVR_MENU_CALLSTACKS_L:
            {
                sF = _VCR_sfHandleCallStacksEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_READCALLSTACKS:
            case AT_SIVR_MENU_READCALLSTACKS_L:
            {
                sF = _VCR_sfHandleReadCallStacksEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CALLSTACKSOPTIONS:
            case AT_SIVR_MENU_CALLSTACKSOPTIONS_L:
            {
                sF = _VCR_sfHandleCallStackOptionsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CALLSTACKSRESETCONFIRM:
            case  AT_SIVR_MENU_CALLSTACKSRESETCONFIRM_L:
            {
                sF = _VCR_sfHandleCallStackResetConfirmEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_SETTINGS:
            case AT_SIVR_MENU_SETTINGS_L:
            {
                sF = _VCR_sfHandleSettingsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_BLUETOOTHOPTIONS:
            case AT_SIVR_MENU_BLUETOOTHOPTIONS_L:
            {
                sF = _VCR_sfHandleBluetoothOptionsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CHANGEBLUETOOTHPIN:
            case AT_SIVR_MENU_CHANGEBLUETOOTHPIN_L:
            {
                sF = _VCR_sfHandleBluetoothPinEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT:
            case AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT_L:
            {
                sF = _VCR_sfHandleBluetoothPinCorrectEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CALLBACKMISSEDCALLS:
            case AT_SIVR_MENU_CALLBACKMISSEDCALLS_L:
            {
                sF = _VCR_sfHandleCallbackMissedCallsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM:
            case AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM_L:
            {
                sF = _VCR_sfHandleRestoreFactorySettingsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_DELETEALLVOICETAGSCONFIRM:
            case AT_SIVR_MENU_DELETEALLVOICETAGSCONFIRM_L:
            {
                sF = _VCR_sfHandleDeleteAllVTConfirmEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_VOICEMEMOS:
            case AT_SIVR_MENU_VOICEMEMOS_L:
            {
                sF = _VCR_sfHandleVoiceMemosEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_VOICEMEMOSPLAYMEMOS:
            case AT_SIVR_MENU_VOICEMEMOSPLAYMEMOS_L:
            {
                sF = _VCR_sfHandleVoiceMemosPlayEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_VOICEMEMOSPLAYMEMOSOPTIONS:
            case AT_SIVR_MENU_VOICEMEMOSPLAYMEMOSOPTIONS_L:
            {
                sF = _VCR_sfHandleVoiceMemosPlayOptionsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETE:
            case AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETE_L:
            {
                sF = _VCR_sfHandleVoiceMemosConfDeleteEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETEALL:
            case AT_SIVR_MENU_VOICEMEMOSCONFIRMDELETEALL_L:
            {
                sF = _VCR_sfHandleVoiceMemosConfDeleteAllEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_READSMS:
            case AT_SIVR_MENU_READSMS_L:
            {
                sF = _VCR_sfHandleReadSMSEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_READSMSOPTIONS:
            case AT_SIVR_MENU_READSMSOPTIONS_L:
            {
                sF = _VCR_sfHandleReadSMSOptionsEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_DELETESMSCONFIRM:
            case AT_SIVR_MENU_DELETESMSCONFIRM_L:
            {
                sF = _VCR_sfHandleDeleteSMSConfirmEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_CALLBACKSMSCONFIRM:
            case AT_SIVR_MENU_CALLBACKSMSCONFIRM_L:
            {
                sF = _VCR_sfHandleCallBackSMSConfirmEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            case AT_SIVR_MENU_STORENAME:
            case AT_SIVR_MENU_STORENAME_L:
            {
                sF = _VCR_sfHandleStorenameEvent(
                        &event_handled,is_event,event,cmd );
                break;
            }

            default:
            {
                sF = FAIL;
                break;
            }
        }
        /* check if for this event a generic handling is needed */
        if (BT_FALSE == event_handled)
        {
            sF = _VCR_sfHandleEventGeneric(is_event,event,cmd );
        }

        if (is_event)
        {
            DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR, "VCRMenuEvent: $VCRMenuEventEnum$=%d", event);
        }

        if (old_state != _VCR_gData.eMenuState)
        {
            _VCR_gData.ePrevMenuState = old_state;
            if (BT_TRUE == _VCR_gData.bPerfMeasOn)
            {   /* send ASR status update for changed menu id */
                _VCR_vSendASRMessage(VCR_PERFSTAT_STATUS);
            }


/*
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='IDLE                                  '   DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='MAIN                                  '   DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DIALNAMECONFIRM                       '   DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DIALNAMECONFIRMCATEGORIES             '   DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DIALNAMECONFIRMRESOLVEAMBINAMES       '   DEBUG_STATE_NUMBER='4'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DIALNUMBER                            '   DEBUG_STATE_NUMBER='5'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DIALNUMBERCORRECT                     '   DEBUG_STATE_NUMBER='6'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='MUSIC                                 '   DEBUG_STATE_NUMBER='7'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='MOREOPTIONS                           '   DEBUG_STATE_NUMBER='8'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='PLAYADDRESSBOOK                       '   DEBUG_STATE_NUMBER='9'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='PLAYADDRESSBOOKENTRY                  '   DEBUG_STATE_NUMBER='10'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='PLAYADDRESSBOOKOPTIONS                '   DEBUG_STATE_NUMBER='11'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='RESOLVEMULTIPLENUMPERCATEGORY         '   DEBUG_STATE_NUMBER='12'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='RESETNAMECONFIRM                      '   DEBUG_STATE_NUMBER='13'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CALLSTACKS                            '   DEBUG_STATE_NUMBER='14'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='READCALLSTACKS                        '   DEBUG_STATE_NUMBER='15'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CALLSTACKSOPTIONS                     '   DEBUG_STATE_NUMBER='16'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CALLSTACKSRESETCONFIRM                '   DEBUG_STATE_NUMBER='17'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='SETTINGS                              '   DEBUG_STATE_NUMBER='18'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='BLUETOOTHOPTIONS                      '   DEBUG_STATE_NUMBER='19'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CHANGEBLUETOOTHPIN                    '   DEBUG_STATE_NUMBER='20'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CHANGEBLUETOOTHPINCORRECT             '   DEBUG_STATE_NUMBER='21'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CALLBACKMISSEDCALLS                   '   DEBUG_STATE_NUMBER='22'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='RESTOREFACTORYDEFAULTSCONFIRM         '   DEBUG_STATE_NUMBER='23'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='RINGTONE                              '   DEBUG_STATE_NUMBER='24'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='RINGTONEOPTIONS                       '   DEBUG_STATE_NUMBER='25'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CHANGELANGUAGE                        '   DEBUG_STATE_NUMBER='26'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CHANGELANGUAGECONFIRM                 '   DEBUG_STATE_NUMBER='27'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DELETEALLVOICETAGSCONFIRM             '   DEBUG_STATE_NUMBER='28'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='VOICEMEMOS                            '   DEBUG_STATE_NUMBER='39'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='VOICEMEMOSPLAYMEMOS                   '   DEBUG_STATE_NUMBER='30'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='VOICEMEMOSPLAYMEMOSOPTIONS            '   DEBUG_STATE_NUMBER='31'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='VOICEMEMOSCONFIRMDELETE               '   DEBUG_STATE_NUMBER='32'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='VOICEMEMOSCONFIRMDELETEALL            '   DEBUG_STATE_NUMBER='33'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='TRAINING                              '   DEBUG_STATE_NUMBER='34'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='READSMS                               '   DEBUG_STATE_NUMBER='35'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='READSMSOPTIONS                        '   DEBUG_STATE_NUMBER='36'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='DELETESMSCONFIRM                      '   DEBUG_STATE_NUMBER='37'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='CALLBACKSMSCONFIRM                    '   DEBUG_STATE_NUMBER='38'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='TRAININGREADPROMPT                    '   DEBUG_STATE_NUMBER='49'
DEBUG_STATE_ENUM_NAME='VCRMenuTreeEnum'   DEBUG_STATE_NAME='STORENAME                             '   DEBUG_STATE_NUMBER='40'


*/


            DEBUG_STATE2(VCR_SWI_TRC, MOD_VCR,"Menu $VCRMenuTreeEnum$ -> $VCRMenuTreeEnum$ (%d->%d)", old_state,_VCR_gData.eMenuState);

#if 0
            switch (_VCR_gData.eMenuState)
            {
               case AT_SIVR_MENU_IDLE:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = Idle", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_MAIN:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = MainMenu", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DIALNAMECONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DialNameConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DialNameConfirmCategories", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DialNameConfirmResolveAmbiguouseNames", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DIALNUMBER:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DialNumber", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DIALNUMBERCORRECT:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DialNumberCorrect", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_MUSIC:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = Music", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_MOREOPTIONS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = MoreOptions", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_PLAYADDRESSBOOK:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = PlayAddressbook", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_PLAYADDRESSBOOKENTRY:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = PlayAddressbookEntry", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = PlayAddressbookOptions", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ResolveMultipleNumbersPerCategory", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_RESETNAMECONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ResetNameConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CALLSTACKS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = CallStacks", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_READCALLSTACKS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ReadCallStacks", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CALLSTACKSOPTIONS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = CallStacksOptions", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CALLSTACKSRESETCONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = CallStacksResetConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_SETTINGS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = Settings", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_BLUETOOTHOPTIONS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = BluetoothOptions", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CHANGEBLUETOOTHPIN:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ChangeBluetoothPIN", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ChangeBluetoothPINCorrect", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CALLBACKMISSEDCALLS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = CallBackMissedCalls", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_RESTOREFACTORYDEFAULTSCONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = RestoreFactoryDefaultsConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_RINGTONE:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = Ringtone", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_RINGTONEOPTIONS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = RingtoneOptions", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CHANGELANGUAGE:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ChangeLanguage", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CHANGELANGUAGECONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ChangeLanguageConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DELETEALLVOICETAGSCONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DeleteAllVoiceTagsConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_TRAINING:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = Training", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_READSMS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ReadSMS", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_READSMSOPTIONS:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = ReadSMSOptions", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_DELETESMSCONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = DeleteSMSConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_CALLBACKSMSCONFIRM:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = CallBackSMSConfirm", old_state,_VCR_gData.eMenuState);
                  break;

               case AT_SIVR_MENU_STORENAME:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> %d = StoreName", old_state,_VCR_gData.eMenuState);
                  break;

               default:
                  DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"MenuState %d -> MenuState %d", old_state,_VCR_gData.eMenuState);

                  break;
            }
#endif
        }
    }
    else
    {
       DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"NOT processed isEvent %d Event %d Cmd %d", is_event,event,cmd, 0);
//       DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"NOT processed State %d -> State %d        ", old_state,_VCR_gData.eMenuState);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFHandleEventGeneric                                    */
/**
    \brief      event handler for generic events

    \brief

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

// These defines are used for Retries
// To know the time it multiplied with the Alarm
#define MAX_WAIT_MUTE_DELAY_RETRIES    400
#define MAX_WAIT_START_ENGINE_RETRIES  2250 // 45 sec is sync max time for 1000 ontacts -> 2500 contacts -> 112,5 seconds


static SuccessFailType _VCR_sfHandleEventGeneric(
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{

    static Uint16Type u16MuteWaitDelay = 0;
    static Uint16Type u16StartEngineDelay = 0;
    SuccessFailType sF = SUCCESS;
    APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
    if (is_event)
    {
        switch (event)
        {
            case VCR_MEVT_ASR_RETRIGGER:
            {   /* there was new info for ASR - call prepare func */
                _VCR_vPrepareASRCmdMessage();
                break;
            }
            case VCR_MEVT_PRMT_END_RESTART:
            {
                (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                break;
            }
            case VCR_MEVT_MARK_SMS_READ:
            {   /* check if there are SMS to be marked read, if so mark as read */
                rsp = APP_SMS_rcMarkAsRead( VCR_vCBMarkSMSRead, 1u);
                switch (rsp)
                {
                    case APP_COMMON_RSP_OK:
                    {   /* do nothing but wait for callback */
                        break;
                    }
                    case APP_COMMON_RSP_ERROR_BUSY:
                    {   /* we need to retry - send trigger event in here */
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_MARK_SMS_READ);
                        break;
                    }
                    case APP_COMMON_RSP_ERROR:
                    case APP_COMMON_RSP_ERROR_BAP_PARSE:
                    case APP_COMMON_RSP_ERROR_UART_BLOCKED:
                    case APP_COMMON_RSP_ERROR_AT_SEND:
                    case APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT:
                    case APP_COMMON_RSP_ERROR_NOT_INITIALIZED:
                    default:
                    {   /* an error occured - set mark back to 0 */
                        DEBUG_VALUE2(VCR_SWI_WNG, MOD_VCR,"Mark SMS %d Read Error Resp %d - set mark back to 0",_VCR_gData.u16NumSMSMarkRead, rsp);

                        _VCR_gData.u16NumSMSMarkRead = 0u;
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_REQUEST_MUTE:
            {
                _VCR_gData.bMuteACK = BT_FALSE;        // clear before request
                _VCR_gData.bMuteWaitACK = BT_TRUE;     // Signalize that we wait
                if(APP_DEVICE_AM_sfGetMute(VCR_cbMuteGetAck, APP_CAN_enVoicePrompts, __LINE__ ) != SUCCESS)
                {
                    DEBUG_TEXT( VCR_SWI_WNG, MOD_VCR, "SIVR MuteRequest not accepted !");
                    // no further action, because request is cancelled

                    // set back to System available, otherwise we are blocked !!!
                    // maybe we have to memorize the trigger PTT / Haptic
                    _VCR_gData.eSystemState    = VCR_SYSTEM_AVAILABLE;
                }
                else
                {
                    // Mute request was accepted, wait for ack
                    u16MuteWaitDelay = 0;
                    DEBUG_TEXT( VCR_SWI_TRC, MOD_VCR, "SIVR MuteRequest accepted -> wait for Mute ACK");
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_WAIT_MUTE_ACK);
                }
                break;
            }
            case VCR_MEVT_WAIT_MUTE_ACK:
            {
                if (BT_TRUE == _VCR_gData.bMuteWaitACK)
                {
                    if (BT_TRUE == _VCR_gData.bMuteACK)
                    {
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_MUTE_ACK_START_ENGINE);
                    }
                    else
                    {
                       if (u16MuteWaitDelay < MAX_WAIT_MUTE_DELAY_RETRIES)
                       {
                           // check again
                           u16MuteWaitDelay++;
                           DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"SIVR still waits for Mute ACK - Retry %d Max %d", u16MuteWaitDelay, MAX_WAIT_MUTE_DELAY_RETRIES);
                           _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_WAIT_MUTE_ACK);
                       }
                       else
                       {
                           // no ack received !!!-> stop system
                           // RT#5998
                           // Stopping the system is better in this case, beause otherwise we a have a active screen but no audio of the VCR


                           DEBUG_VALUE1(VCR_SWI_ERR, MOD_VCR,"SIVR didn't get a Mute ACK after %d Retries !!! -> give up", u16MuteWaitDelay);

                           u16MuteWaitDelay = 0;


                           _VCR_vSystemStopToIdle(BT_FALSE);
                       }
                    }

                }
                else
                {
                     // a other Error occured, or maybe we was ousted by a other application
                     DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"SIVR had a Error or was ousted -> give up");
                    _VCR_vSystemStopToIdle(BT_FALSE);
                }
                break;
            }
            case VCR_MEVT_MUTE_ACK_START_ENGINE:
            {
                switch (_VCR_gData.stCtrlInfo.eEngineStatus)
                {
                  case AT_SIVR_ENG_BEGIN_SYNC:
                     if (u16StartEngineDelay < MAX_WAIT_START_ENGINE_RETRIES)
                     {
                         u16StartEngineDelay++;
                         _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_MUTE_ACK_START_ENGINE);
                         DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"SIVR still in SYNC - Retry %d Max %d",u16StartEngineDelay, MAX_WAIT_START_ENGINE_RETRIES);
                     }
                     else
                     {
                         sF = _VCR_sFMessageToBeSend(VCR_MSG_START_ENGINE );
                         _VCR_vNotifyApplication(VCR_NOTIFY_SIVR_ACTIVATE);
                         u16StartEngineDelay = 0;
                     }
                     break;

                  default:
                         sF = _VCR_sFMessageToBeSend(VCR_MSG_START_ENGINE );
                         _VCR_vNotifyApplication(VCR_NOTIFY_SIVR_ACTIVATE);
                         u16StartEngineDelay = 0;
                     break;
                }
                break;
            }
            case VCR_MEVT_SIVR_STARTED:
            {   /* check if this start is due to language change */
                switch (_VCR_gData.eSystemState)
                {
                    case VCR_SYSTEM_STARTED_HAPTIC:
                    {   /* this case was already handled in state idle!!! */
                        break;
                    }
                    case VCR_SYSTEM_STARTED_PTT:
                    {
                        sF = _VCR_eHandle_VCRSystemStartedOverPTT();
                        break;
                    }
                    case VCR_SYSTEM_STOP_PTT:
                    {   /* start after system stop to give out stop prompt */
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                        _VCR_gData.u8NumPromptRequired = 1u;
                        _VCR_gData.u8NumPromptSend     = 0u;
                        sF =  _VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                        break;
                    }
                    case VCR_SYSTEM_STARTED_LANG:
                    {
                        /* need additional identifier if lang reconf via SDS */
                        if ((_VCR_gData.eCurrentLang == _VCR_gData.eRequiredLang)  && (_VCR_gData.bFallbackLanguageChosen == BT_FALSE))   //RT#5138
                        {
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_LANGUAGE_FOR;
                            _VCR_gData.ePromtTable.tab[2]  = AT_SIVR_PRMT_SYSTEM_STOP;
                            _VCR_gData.u8NumPromptRequired = 3u;
                            _VCR_gData.u8NumPromptSend     = 0u;

                        }
                        else
                        {
                            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
                            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SELECTED_LANGUAGE_NOT_AVAILABLE;
                            _VCR_gData.ePromtTable.tab[2]  = AT_SIVR_PRMT_LANGUAGE_FOR;
                            _VCR_gData.ePromtTable.tab[3]  = AT_SIVR_PRMT_SYSTEM_STOP;
                            _VCR_gData.u8NumPromptRequired = 4u;
                            _VCR_gData.u8NumPromptSend     = 0u;

                            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Language current %d required %d",
                                    _VCR_gData.eCurrentLang,_VCR_gData.eRequiredLang);
                        }
                        /* set all languages to current */
                        _VCR_gData.eRequiredLang = _VCR_gData.eCurrentLang;
                        /* fill the prompts according to language infos,
                         * update the language infos */
                        sF =  _VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                        break;
                    }
                    case VCR_SYSTEM_FLASHERASE_IPR:
                    {
                        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
                        _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_ORIGINAL_SETTINGS_RESTORED;
                        _VCR_gData.u8NumPromptRequired = 2u;
                        _VCR_gData.u8NumPromptSend     = 0u;
                        sF =  _VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTART_PROMPT );
                        _VCR_gData.eSystemState = VCR_SYSTEM_STARTED_PTT;
                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Original settings restored");

                        /* change to main menu */
                        break;
                    }
                    case VCR_SYSTEM_STOP_LANG:
                    case VCR_SYSTEM_AVAILABLE:
                    case VCR_SYSTEM_NOT_AVAILABLE:
                    default:
                    {   /* we ignore the START - only trace */

                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"VR START ignored MenuState %d SysState",
                                _VCR_gData.eMenuState,_VCR_gData.eSystemState);
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_SIVR_STOPPED:
            {   /* we need to check the current system state */
                switch (_VCR_gData.eSystemState)
                {
                    case VCR_SYSTEM_STOP_PTT:
                    {   /* now the system is stopp, start again and with the start
                           send "end" prompt */
                        /* check if we need to pause MDI */
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_START_ENGINE );
                        break;
                    }
                    case VCR_SYSTEM_STARTED_PTT:
                    case VCR_SYSTEM_STARTED_HAPTIC:
                    {   /* NOTE: This shall normally not occur, but looks like
                          some voice commands may lead to stop of the system
                          -> if this happens, print trace and send stopped so that
                          system can go back to IDLE */

                        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"STOP INVALID MenuState %d SysState %d",
                                        _VCR_gData.eMenuState,_VCR_gData.eSystemState);

                        _VCR_gData.eSystemState = VCR_SYSTEM_AVAILABLE;
                        _VCR_vSystemStopToIdle(BT_FALSE);
                        break;
                    }
                    case VCR_SYSTEM_AVAILABLE:
                    {   /* system was stopped and is waiting for next start, check if we need
                           to mark SMS as read */
                        if (0u < _VCR_gData.u16NumSMSMarkRead)
                        {
                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"start SMS Mark Read %d",
                                    _VCR_gData.u16NumSMSMarkRead);

                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_MARK_SMS_READ);
                        }
                        break;
                    }
                    default:
                    {   /* we ignore the STOP */

                        DEBUG_VALUE2(VCR_SWI_DBG, MOD_VCR,"STOP MenuState %d SysState %d  - ignored",
                                        _VCR_gData.eMenuState,_VCR_gData.eSystemState);
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_START_PROMTP_END:
            {
                _VCR_vChangeMenuToMain();
                break;
            }
            case VCR_MEVT_MEDIAPAYER:
            {
                DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Goto menu music");
                (void)_VCR_sfInitMsgResultValues();
                _VCR_vChangeState(AT_SIVR_MENU_MUSIC);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_MUSIC);
                break;
            }
            case VCR_MEVT_PREP_OK:
            {   /* start RECO */
                /* first clean up old message result values */
                (void)_VCR_sfInitMsgResultValues();
                (void)_VCR_sFMessageToBeSend(VCR_MSG_START_RECO );
                break;
            }
            case VCR_MEVT_ERROR:
            {
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                break;
            }
            case VCR_MEVT_REDIAL_DONE: /* back to idle without stop prompt */
            case VCR_MEVT_DIAL_OK: /* back to idle without stop prompt */
            {
                DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"received DIAL OK");
                /* NOTE: NO break here to process same as for stop prompt end!!! */
            }
            case VCR_MEVT_STOP_PROMPT_END:
            {   /* stop system and go to idle */
                _VCR_vSystemStopToIdle(BT_FALSE);
                break;
            }
            case VCR_MEVT_OUTGOING_CALL:  // Fallthrough RT#3713
            case VCR_MEVT_INCOMING_CALL:
            case VCR_MEVT_CK5050_POWER_OFF:
            {
                switch (_VCR_gData.eSystemState)
                {
                    case VCR_SYSTEM_NOT_AVAILABLE:
                    case VCR_SYSTEM_STOP_PTT:
                    case VCR_SYSTEM_AVAILABLE:
                    {   /* nothing to do as we are inactive or already stopping */
                        break;
                    }
                    default:
                    {   /* in all other states - stop system */
                        _VCR_vSystemStopToIdle(BT_FALSE);
                        break;
                    }
                }

                DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"Stop VR-Call/POFF in MenuState %d SysState %d EngineState %d",
                                        _VCR_gData.eMenuState,_VCR_gData.eSystemState,_VCR_gData.stCtrlInfo.eEngineStatus, 0);

                break;
            }
            case VCR_MEVT_PTT:
            {   /* need to be taken into account here related to machine state */
                /* first of all handle as system stop indication */
                switch (_VCR_gData.eSystemState)
                {
                    case VCR_SYSTEM_AVAILABLE:
                    {   /* this is a start trigger which need to be taken into acount */
                        /* check if we need to pause MDI */
                        _VCR_gData.eSystemState = VCR_SYSTEM_STARTED_PTT;

                        // RS: removed, is now handled in AudioManager
                        //APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_START);
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_REQUEST_MUTE);
                        break;
                    }
                    case VCR_SYSTEM_STARTED_PTT:
                    case VCR_SYSTEM_STARTED_HAPTIC:
                    {   /* we need to evaluate the current engine
                           state to check how to proceed */
                        /* in case the system waits for a user feedback we stop immediately
                         * therefore first send engine stop and then start and give out
                         * end prompt  */
                        if (AT_SIVR_ENG_BEGIN_AQU == _VCR_gData.stCtrlInfo.eEngineStatus)
                        {   /* system waits for user input */
                            _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                            /* send engine stop */
                        }
                        /* otherwise a prompt is send, dialogue shall continue
                         * therefore first stop and then start the engine again at same
                         * menu */
                        else if (AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)
                        {   /* check if this was start or stop prompt, if so ignore */
                            if (_VCR_gData.stCtrlInfo.eLastSend == VCR_MSG_PLAY_SYSSTOP_PROMPT ||
                                _VCR_gData.stCtrlInfo.eLastSend == VCR_MSG_PLAY_SYSSTART_PROMPT	)
                            {
                                DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"PTT ignored in MenuState: $ATSIVRPromptEnum$ = %d",_VCR_gData.eMenuState);
                                DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"SysState: $VCRSystemStateEnum$ = %d",_VCR_gData.eSystemState);
                                DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"  EngineState: $ATSIVREngStatusEnum$ = %d", _VCR_gData.stCtrlInfo.eEngineStatus);

                            }
                            else if (_VCR_gData.stCtrlInfo.eLastSend == VCR_MSG_INFO_SYSSTART_PROMPT) //RT13214
                            {
                                _VCR_gData.eSystemState = VCR_SYSTEM_STOP_PTT;
                                (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                            }
                            else
                            {   /* send specific prompt stop depending if reco was started
                                   or not */
                                if (VCR_MSG_START_RECO == _VCR_gData.stCtrlInfo.eLastSend)
                                {
                                    (void)_VCR_sFMessageToBeSend(VCR_MSG_RESTART_PROMPT_RECO );
                                }
                                else if(VCR_MSG_RESTART_PROMPT_RECO == _VCR_gData.stCtrlInfo.eLastSend)
                                {
                                    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Ignore PTT Press");//RT 12571
                                }
                                else
                                {
                                    _VCR_gData.stCtrlInfo.eLastSendForPrmtRestart =
                                            _VCR_gData.stCtrlInfo.eLastSend;
                                    /*Bugfix: Schnelldruecker Wolfgang*/
                                    if (_VCR_gData.stCtrlInfo.bStopPending == BT_TRUE)
                                    {
                                        /* don't send a second stop if a first is pending*/
                                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"PTT but Pending Stop - Stop Engine");

                                        (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
                                    }
                                    if (_VCR_gData.stCtrlInfo.bStopPending == BT_FALSE)
                                    {
                                        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"PTT pressed - Stop Engine");
                                        (void)_VCR_sFMessageToBeSend(VCR_MSG_RESTART_PROMTP_NORECO );
                                        _VCR_gData.stCtrlInfo.bStopPending = BT_TRUE;
                                    }
                                }
                            }
                        }
                        else /* ignore the pressing as neither prompt nor aquisition ongoing */
                        {

                            DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"PTT ignored in MenuState: $ATSIVRPromptEnum$ = %d",_VCR_gData.eMenuState);
                            DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"SysState: $VCRSystemStateEnum$ = %d",_VCR_gData.eSystemState);
                            DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"  EngineState: $ATSIVREngStatusEnum$ = %d", _VCR_gData.stCtrlInfo.eEngineStatus);

                        }
                        break;
                    }
                    case VCR_SYSTEM_STOP_PTT:
                    {   /* check if we already have send the stop to the system
                           but system is still not in idle state */
                        if (((BT_TRUE == _VCR_gData.bStopOKReceived) &&
                            (AT_SIVR_ENG_IDLE != _VCR_gData.stCtrlInfo.eEngineStatus)) )
                        {   /* issue - why have we not yet received the stop signal ?
                               retry to stop the system */
                            (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );

                            DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"RETRY SYSSTOP in MenuState %d SysState %d EngineState %d",
                                    _VCR_gData.eMenuState,_VCR_gData.eSystemState,_VCR_gData.stCtrlInfo.eEngineStatus, 0);

                        }
                        break;
                    }
                    case VCR_SYSTEM_NOT_AVAILABLE:
                    case VCR_SYSTEM_STARTED_LANG:
                    case VCR_SYSTEM_STOP_LANG:
                    default:
                    {   /* we ignore the PTT - only trace */

                        DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"PTT ignored in MenuState %d SysState %d EngineState %d",
                                _VCR_gData.eMenuState,_VCR_gData.eSystemState,_VCR_gData.stCtrlInfo.eEngineStatus, 0);
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_OK:
                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"OK ignored in MenuState %d SysState %d",
                        _VCR_gData.eMenuState,_VCR_gData.eSystemState);
                break;
            case VCR_MEVT_CALL:
                DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Hook ignored in MenuState %d SysState %d",
                        _VCR_gData.eMenuState,_VCR_gData.eSystemState);
                break;
            default:
            {   /* in case of not handled a event retrigger machine to NOT miss out
                   some events */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0u);
                sF = FAIL;
                break;
            }
        }
    }
    else
    {
        switch (cmd)
        {
            case AT_SIVR_CMD_CANCEL:
            {
                _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
                _VCR_gData.u8NumPromptRequired = 1u;
                _VCR_gData.u8NumPromptSend     = 0u;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
                break;
            }
            case AT_SIVR_CMD_HELP:
            {   /* change menu type to help menu and retrigger prep */
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_HELP;
                _VCR_gData.stProcMenu.ePromptId   = _VCR_gData.eMenuState;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            default:
            {
                /* in case of not handled a event retrigger machine to NOT miss out
                   some cmds */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0u);
                sF = FAIL;
                break;
            }
        }
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_eHandleIdleEvent                                        */
/**
    \brief      event handler for event received in IDLE state

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
static SuccessFailType _VCR_sfHandleIdleEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;

    *handled = BT_TRUE;

    if (is_event)
    {
        switch (event)
        {
            case VCR_MEVT_SIVR_SUPPORTED:
            {   /* we need to ask for the language number */
                sF = _VCR_sFMessageToBeSend(VCR_MSG_GET_LANG_NUM );
                break;
            }
            case VCR_MEVT_LANGNUM_AVAIL:
            {   /* we need to ask for the language settings */
                sF = _VCR_sFMessageToBeSend(VCR_MSG_GET_LANG_MAP );
                break;
            }
            case VCR_MEVT_LANGMAP_AVAIL:
            {   /* ask for current set language */
                sF = _VCR_sFMessageToBeSend(VCR_MSG_GET_CURRENT_LANG );
                break;
            }
            case VCR_MEVT_LANG_CHANG_REG:
            {
                if ( (VCR_SYSTEM_NOT_AVAILABLE == _VCR_gData.eSystemState) ||
                     (BT_FALSE == _VCR_gData.bInitial_init_done))
                {
                    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Language change trig ignore %d SysState %d",
                        _VCR_gData.bInitial_init_done,_VCR_gData.eSystemState);
                    break;
                }
                /* else no break to do similar handling like VCR_MEVT_CUR_LANG_AVAIL*/
                /* NO BREAK HERE!!! DO NOT MOVE CODE between here
                   and case VCR_MEVT_CUR_LANG_AVAI !!!!!! */
            }
            case VCR_MEVT_CUR_LANG_AVAIL:
            {   /* maybe we have to change the current language */
                /* check if current language equals required */
                if (BT_FALSE == _VCR_bCheckLangChangeReq())
                {   /* check if we have a language change */
                    if (BT_TRUE == _VCR_gData.bTrigLangChange)
                    {
                        _VCR_gData.bTrigLangChange = BT_FALSE;
                        /* we need to start machine for prompt output of the
                         * language information */
                        _VCR_gData.eSystemState = VCR_SYSTEM_STARTED_LANG;
						// RS: removed, is now handled in AudioManager
                        //APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_START);
                        // muss eventuell noch angepasst werden
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_REQUEST_MUTE);
                    }
                    else
                    {   /* check if we have a Flash Erase - read out */
                        if (VCR_SYSTEM_FLASHERASE_IPR == _VCR_gData.eSystemState)
                        {
							// RS: removed, is now handled in AudioManager
                            //APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_START);
                            // muss eventuell noch angepasst werden
                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_REQUEST_MUTE);
                        }
                        else
                        {
                            /* next state is idle and system is available now */
                            _VCR_gData.eSystemState = VCR_SYSTEM_AVAILABLE;
                            _VCR_vChangeMenuToIdle();
                        }
                    }
                }
                else
                {   /* set new language - here SIVR will send another RSTU!! */
                    _VCR_gData.bTrigLangChange = BT_TRUE;
                    sF =  _VCR_sFMessageToBeSend(VCR_MSG_SET_CURRENT_LANG );
                }
                break;
            }
            case VCR_MEVT_CUR_LANG_CHANGED:
            {   /* next state is idle - activate SIVR */
                /* here a restart of the SIVR will happen and machine will receive
                 * another VCR_MEVT_SIVR_SUPPORTED */
                DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Language changed");
                break;
            }
            case VCR_MEVT_START_PROMTP_END:
            {   /* check if SD Haptic is available */
                if (BT_TRUE == APP_SD_bHapticalInterfaceAvailable())
                {   /* goto main */
                    _VCR_vChangeMenuToMain();
                }
                else
                {
#ifdef VCR_SUPPORT_MISSED_CALLS
                    /* check for missed calls */
                    tmp_missed = APP_PHONE_u16NewMissedCallsAvailable();
                    if (0 < tmp_missed)
                    {   /* we have missed calls, fill info in CStack storage and
                           change state to callBackMissedCalls */
                        (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                        _VCR_gData.resultInfo.cStack.type = VCR_RESULT_TYPE_CALLSTACKS;
                        _VCR_gData.resultInfo.cStack.num_missed_calls = tmp_missed;
                        _VCR_gData.resultInfo.cStack.stack_type = APP_PHONEBOOK_enMissedCalls;
                        _VCR_vChangeState(AT_SIVR_MENU_CALLBACKMISSEDCALLS);
                        _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_START_PROMTP_END );
                    }
                    else
                    {
                        /* goto main */
                        _VCR_vChangeMenuToMain();
                    }
#else
                    _VCR_vChangeMenuToMain();
#endif
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            case VCR_MEVT_VT_DELETE_DONE:
            {   /* stop the system again */
                _VCR_vSystemStopToIdle(BT_FALSE);
                break;
            }
            case VCR_MEVT_SIVR_STARTED:
            {   /* check for start due to haptic */
                if (VCR_SYSTEM_STARTED_HAPTIC == _VCR_gData.eSystemState)
                {   /* check the specific haptic type */
                    /* change system state to normal active one */
                    _VCR_gData.eSystemState = VCR_SYSTEM_STARTED_PTT;
                    /* check if this was for playing BT connect prompt only */
                    switch (_VCR_gData.eHapticTrigType)
                    {
                        case VCR_HAPTIC_PLAY_TEST:
                        {   /* play test prompt */
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_TEST_DIAGNOSE);
                            break;
                        }
                        case VCR_HAPTIC_PLAY_NAME:
                        {   /* play name of given haptic index and stop */
                            _VCR_vSinglePrompt(AT_SIVR_PRMT_D_NAME);
                            break;
                        }
                        case VCR_HAPTIC_STORE_NAME:
                        {   /* fill haptic info to addressbook info and
                               goto store name */
                            (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                            _VCR_gData.resultInfo.playPB.type = VCR_RESULT_TYPE_PLAYPB;
                            _VCR_gData.resultInfo.playPB.is_haptic_trigger   = BT_TRUE;
                            _VCR_gData.resultInfo.playPB.is_add_new_contact  = BT_FALSE;
                            _VCR_gData.resultInfo.playPB.cur_entry = _VCR_gData.u16HapticIdx;
                            _VCR_vChangeState(AT_SIVR_MENU_STORENAME);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_PAB_OPT_CHANGE_NAME );
                            break;
                        }
                        case VCR_HAPTIC_DELETE_NAME:
                        {   /* delete voice tag for given index and stop */
                            _VCR_gData.resultInfo.playPB.type = VCR_RESULT_TYPE_PLAYPB;
                            _VCR_gData.resultInfo.playPB.is_haptic_trigger = BT_TRUE;
                            _VCR_gData.resultInfo.playPB.cur_entry =
                                _VCR_gData.u16HapticIdx;
                            _VCR_vChangeState(AT_SIVR_MENU_RESETNAMECONFIRM);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_YES_RESETNAMECONFIRM );
                            break;
                        }
                        case VCR_HAPTIC_READ_NEW_SMS:
                        {   /* goto readSMS */
                            _VCR_gData.resultInfo.readSMS.is_first_read_sms = BT_TRUE;
                            _VCR_gData.resultInfo.readSMS.is_haptic_trigger = BT_TRUE;
                            _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                            _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
                            break;
                        }
                        case VCR_HAPTIC_READ_SMS:
                        {   /* read only the indicated SMS */
                            _VCR_gData.resultInfo.readSMS.type = VCR_RESULT_TYPE_READSMS;
                            _VCR_gData.resultInfo.readSMS.is_haptic_trigger = BT_TRUE;
                            _VCR_gData.resultInfo.readSMS.current_num_sms =
                                _VCR_gData.u16HapticIdx - 1u;
                             _VCR_gData.resultInfo.readSMS.is_first_read_sms = BT_TRUE;
                            _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                            _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_READ_SMS );
                            break;
                        }
                        case VCR_HAPTIC_PLAY_VMEMO:
                        {   /* delete voice tag for given index and stop */
                            _VCR_vFunctionNotAvailableStop();
                            break;
                        }
                        default:
                        {   /* incorrect haptic type - error ! */
                            _VCR_PlayError(__LINE__);
                        }
                    }
                }
                else
                {   /* handle via generic*/
                    *handled = BT_FALSE;
                }
                break;
            }
            case VCR_MEVT_HAPTIC:
            {   /* we need to start the voice recognition */
                switch (_VCR_gData.eSystemState)
                {
                    case VCR_SYSTEM_AVAILABLE:
                    {   /* this is a start trigger which need to be taken into acount */
                        /* check if we need to pause MDI */
                        _VCR_gData.eSystemState = VCR_SYSTEM_STARTED_HAPTIC;
						// RS: removed, is now handled in AudioManager
                         //APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_START);

                         // muss eventuell noch angepasst werden
                         _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_REQUEST_MUTE);
                        break;
                    }
                    case VCR_SYSTEM_STARTED_PTT:
                    case VCR_SYSTEM_STARTED_HAPTIC:
                    case VCR_SYSTEM_STOP_PTT:
                    case VCR_SYSTEM_NOT_AVAILABLE:
                    case VCR_SYSTEM_STARTED_LANG:
                    case VCR_SYSTEM_STOP_LANG:
                    default:
                    {   /* we ignore the haptic - only trace */
                         DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"PTT ignored in MenuState %d SysState %d EngineState %d",
                                _VCR_gData.eMenuState,_VCR_gData.eSystemState,_VCR_gData.stCtrlInfo.eEngineStatus, 0);
                        break;
                    }
                }
                break;
            }
            case VCR_MEVT_PTT:
            {   /* handle via generic implementation but return success */
                *handled = BT_FALSE;
                break;
            }
            default:
            {   /* unexpected trigger in this state - return FAIL */
                sF = FAIL;
                *handled = BT_FALSE;
            }
        }
    }
    else
    {
        /* no commands from SIVR to be processed in this state */
        *handled = BT_FALSE;
        sF = FAIL;
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_eHandleMainMenuEvent                                    */
/**
    \brief      event handler for event received in MAIN_MENU state

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

static SuccessFailType _VCR_sfHandleMainMenuEvent(BooleanType * handled,
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd)
{
    SuccessFailType sF = SUCCESS;
    *handled = BT_TRUE;

    DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"HandleMainMenuEvent event: %d is_event %d",
                      event,is_event);

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
#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_MUSIC)
                        if (BT_TRUE == _VCR_bGetMusicMenuAvailable())
                        {
                            // Music is coded
                            _VCR_gData.stProcMenu.ePromptId = AT_SIVR_PRMT_T1_MAINMENU;
                        }
                        else
                        {
                            // Music is NOT coded
                            _VCR_gData.stProcMenu.ePromptId = AT_SIVR_PRMT_T1_MAINMENU_WITHOUT_MUSIC;
                        }
#else
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_PRMT_T1_MAINMENU;
#endif
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
                        (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                        break;
                    }
                    case 2:
                    {
                        _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_PRMT_T2_MAINMENU_OPTIONS;
                        _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
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
            case VCR_MEVT_REDIAL_PROMPT_END:
            {   /* previous command was redial and we have send redial prompt -
                   now trigger redial function towards APP */
                if (SUCCESS != _VCR_sFMessageToBeSend(VCR_MSG_REDIAL ))
                {   /* retry */
                    _VCR_vSendTriggerToMainEventHandler(VCR_MEVT_REDIAL_PROMPT_END );
                }
                break;
            }
            case VCR_MEVT_STRING_PRMT_OK:
            {   /* entered main from other state via prompt - restart prep */
                _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_MAIN;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case VCR_MEVT_REDIAL_DONE:
            {   /* stop machine via generic handling */
                *handled = BT_FALSE;
                sF = SUCCESS;
                break;
            }
            /* RT #9238 */
            case VCR_MEVT_PTT:
            case VCR_MEVT_CALL:
            {
                if ( ( (2056 == _VCR_gData.u16TxPromptID) || (1056 == _VCR_gData.u16TxPromptID) ) &&
                     (AT_SIVR_ENG_END_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus) )
                {
                    DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"HandleMainMenuEvent VCR_MEVT_PTT: simulate STRING_PRMT_OK, bugfix USA-BMG SOP2");
                    /* entered main from other state via prompt - restart prep */
                    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_MENU_MAIN;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;

                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                }
                else
                {
                    *handled = BT_FALSE;
                }
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
            /* possible result command codes from processing state machine */
            case AT_SIVR_CMD_MAIN:
            {   /* entry command from other menus */
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_gData.stProcMenu.ePromptId = AT_SIVR_MENU_MAIN;
                _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
                (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
                break;
            }
            case AT_SIVR_CMD_DIAL_NUMBER:
            {   /* go to next state and send retrigger for processing */
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_gData.resultInfo.number.type = VCR_RESULT_TYPE_NUMBER;
                _VCR_gData.resultInfo.number.is_for_contact = BT_FALSE;
                _VCR_vChangeState(AT_SIVR_MENU_DIALNUMBER);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL_NUMBER );
                break;
            }
            case AT_SIVR_CMD_DIAL:
            {
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vChangeState(AT_SIVR_MENU_DIALNAMECONFIRM);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_DIAL );
                break;
            }
            case AT_SIVR_CMD_REDIAL:
            {   /* so far no submenu available - handle directly here but use
                   subfunction located within phone sources */
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vRedial();
                break;
            }
            case AT_SIVR_CMD_MUSIC:
            {

#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_MUSIC)
                if (BT_TRUE == _VCR_bGetMusicMenuAvailable())
                {
                    // Music is coded
                    (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                    _VCR_vChangeState(AT_SIVR_MENU_MUSIC);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_MUSIC );
                }
                else
                {
                    // Music is NOT coded
                    _VCR_vT1SorryRepeat();
                }
#else
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vChangeState(AT_SIVR_MENU_MUSIC);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_MUSIC );
#endif

                break;
            }
            case AT_SIVR_CMD_SETTINGS:
            {
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));

                /* RT-13225: Reset Bluetooth Options error flags */
                DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"RT-13225: Main Menu Settings command identified");

                _VCR_vResetNotRecogCmdErrFlags(); /*RT-13225 & RT-14043*/

                _VCR_vChangeState(AT_SIVR_MENU_SETTINGS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_SETTINGS );
                break;
            }
            case AT_SIVR_CMD_MORE_OPTIONS:
            {
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vChangeState(AT_SIVR_MENU_MOREOPTIONS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_MORE_OPTIONS );
                break;
            }
            case AT_SIVR_CMD_PLAY_ADDRESSBOOK:
            {
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vChangeState(AT_SIVR_MENU_PLAYADDRESSBOOK);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_PLAY_ADDRESSBOOK );
                break;
            }
            case AT_SIVR_CMD_READ_SMS:
            {
#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_SMS)
                if (BT_TRUE == _VCR_bGetSmsMenuAvailable())
                {
                     // mobile supports SMS
                    (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                    _VCR_gData.resultInfo.readSMS.is_first_read_sms = BT_TRUE;
                    _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
                }
                else
                {
                    /* Featureverschiebung */
                    _VCR_vT1SorryRepeat();
                }
#else
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_gData.resultInfo.readSMS.is_first_read_sms = BT_TRUE;
                _VCR_vChangeState(AT_SIVR_MENU_READSMS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_READ_SMS );
#endif
                break;
            }
            case AT_SIVR_CMD_CALL_STACKS:
            {
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vChangeState(AT_SIVR_MENU_CALLSTACKS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_CALL_STACKS );
                break;
            }
            case AT_SIVR_CMD_VOICE_MEMOS:
            {
                (void)memset(&_VCR_gData.resultInfo,0x00,sizeof(VCRResultUnionType));
                _VCR_vChangeState(AT_SIVR_MENU_VOICEMEMOS);
                _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_VOICE_MEMOS );
                break;
            }
            case AT_SIVR_CMD_TRAINING:
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
#if (1 == VCR_SUPPORT_MAIN_MENU_WITHOUT_MUSIC)
                if (BT_TRUE == _VCR_bGetMusicMenuAvailable())
                {
                    // Music is coded
                    *handled = BT_FALSE;
                }
                else
                {
                    // Music is NOT coded
                    _VCR_gData.stProcMenu.ePromptId = AT_SIVR_PRMT_MAINMENU_WITHOUT_MUSIC;
                    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
                    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
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
/* Function    : _VCR_eHandle_VCRSystemStartedOverPTT                             */
/**
    \brief      main event handler machine

    \param      none

    \return     none
 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _VCR_eHandle_VCRSystemStartedOverPTT(void)
{
    SuccessFailType sF = SUCCESS;

    /* start was due to PTT - set string prompt and give out */

#ifdef UHV_ACTIVATE_FEATURE_VCR_SOP3_PROMPTS
    if (BT_TRUE == ERM_bPhoneAvailable())
    {
         /* Phone is available next state is main - HFP is available */
        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
         _VCR_gData.u8NumPromptRequired = 1u;
         _VCR_gData.u8NumPromptSend     = 0u;
         sF =  _VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTART_PROMPT );
    }
    else if (BT_TRUE == APP_MDI_bPlayerAvailable())
    {   /* Player is available A2DP connection is active */

        if (BT_TRUE == APP_MDI_bRemoteControlAvailable())
        {
            /* Remote control is available next state is music menu - AVRCP is available */
            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
            _VCR_gData.u8NumPromptRequired = 1u;
            _VCR_gData.u8NumPromptSend     = 0u;
            sF = _VCR_sFMessageToBeSend(VCR_MSG_MUSIC_SYSSTART_PROMPT);
        }
        else
        {
            /* only A2DP is available - no AVRCP and no HFP is available */
            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_ONLY_MP_CONNECTED_BUT_NO_REMOTE_CONTROL;
            _VCR_gData.ePromtTable.tab[2]  = AT_SIVR_PRMT_SYSTEM_STOP;
            _VCR_gData.u8NumPromptRequired = 3u;
            _VCR_gData.u8NumPromptSend     = 0u;
            sF = _VCR_sFMessageToBeSend(VCR_MSG_INFO_SYSSTART_PROMPT );
        }
    }
    else
    {
        if (BT_TRUE == VCR_bGetInstrumentClusterAvailable())
        {
            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_NO_DEVICE_CONNECTED_WITH_IC_CONTEXT;
            _VCR_gData.ePromtTable.tab[2]  = AT_SIVR_PRMT_SYSTEM_STOP;
            _VCR_gData.u8NumPromptRequired = 3u;
            _VCR_gData.u8NumPromptSend     = 0u;
            sF = _VCR_sFMessageToBeSend(VCR_MSG_INFO_SYSSTART_PROMPT );
        }
        else
        {
            _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
            _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS;
            _VCR_gData.ePromtTable.tab[2]  = AT_SIVR_PRMT_SYSTEM_STOP;
            _VCR_gData.u8NumPromptRequired = 3u;
            _VCR_gData.u8NumPromptSend     = 0u;
            sF = _VCR_sFMessageToBeSend(VCR_MSG_INFO_SYSSTART_PROMPT );
        }
        /* At end of end of the sequence - Set bluetooth visible for 3 minutes */
        ERM_vCb3BM_BTVisibility();
    }
#else
    if (BT_TRUE == ERM_bPhoneAvailable())
    {
        /* Phone is available next state is main - HFP is available */
        _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
        _VCR_gData.u8NumPromptRequired = 1u;
        _VCR_gData.u8NumPromptSend     = 0u;
        sF = _VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTART_PROMPT );
    }
    else
    {
         /* RT#3645 VR_missed_prompt_in_mm_without_connected_phone */
         _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_START;
         _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_PHONE_NOT_AVAILABLE;
         _VCR_gData.u8NumPromptRequired = 2u;
         _VCR_gData.u8NumPromptSend     = 0u;
         sF =  _VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTART_PROMPT );
    }
#endif

    return sF;
}

/* End Of File VCR_StateHandler.c */
