/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_Utils.c
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-10-25 16:29:48 +0200 (Di, 25 Okt 2011) $
* $Rev: 22755 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/src/VCR_Utils.c $
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
#include "VCR_MessageHandler.h"
#include "OSEK_AppTask_H1.h"
#include "APP_SMS.h"
#include "APP_SD.h"   // without this include we get compiler warnings !
#include "stdio.h"    //  "snprintf"
#include "APP_DEVICE_AudioManager.h"
#include "EEPROM_LAYOUT_CFG.h"

#include "VCR_Utils.id"
#include "TRACE.h"



/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/




#include "ATParser_CE.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "VCR_CE.h"
#include "VCR_CI.h"
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

#define LANG_BUF_LEN 20

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _VCR_vDiagnoseTestTranslationBack(VcrModGlobalDataType *_VCR_gData,ATReqVCMgtPromtType *rpmt);

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* File local  Constant Definitions                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private  Constant
*/

ATSIVRLanguagesEnum _VCR_eDefaultLanguage = VCR_DEFAULT_LANGUAGE;

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vInitStateData                                          */
/**
    \brief      function to initialise state machine related data

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vInitStateData(void)
{
    Uint8Type u8DisableDevMode;

    (void)_VCR_sfInitMsgResultValues();
    (void)_VCR_sfInitResultValues();
    _VCR_gData.u8ProcErr = 0;
    _VCR_vResetNotRecogCmdErrFlags();  /*RT-13225 & RT-14043*/
    (void)_VCR_sfInitMenuValues();
    _VCR_vInitMsgCtrlData();

    _VCR_eDefaultLanguage       = VCR_DEFAULT_LANGUAGE;

    _VCR_gData.eSystemState     = VCR_SYSTEM_NOT_AVAILABLE;
    _VCR_gData.bSivrSyncStarted = BT_FALSE;

    _VCR_gData.eSIVRSupported = ERM_enIsVcrSupported();        /* RT 2429 */

    _VCR_gData.bVoiceFBOff      = _VCR_bGetVoiceFeedback();    /* RT 3407*/

    _VCR_gData.u16MuteDelay     = VCR_MUTE_DELAY_OFFSET;
    _VCR_gData.u16ListDelay     = VCR_LIST_DELAY;

    _VCR_gData.u16NumSMSMarkRead = 0;

    _VCR_gData.eMenuState       = AT_SIVR_MENU_IDLE;
    _VCR_gData.ePrevMenuState   = AT_SIVR_MENU_IDLE;
    _VCR_gData.eNextMenuState   = AT_SIVR_MENU_UNDEF;
    _VCR_gData.eCmdId           = AT_SIVR_CMD_UNKNOWN;
    _VCR_gData.eEventTrigMain   = VCR_MEVT_UNDEF;
    _VCR_gData.eCmdTrigMain     = AT_SIVR_CMD_UNKNOWN;

    _VCR_gData.bVCRSupervisionActive = BT_FALSE;

    _VCR_gData.bPTTunhandled = BT_FALSE;
    _VCR_gData.bHOOKunhandled  = BT_FALSE;
    _VCR_gData.PTT_contactNr = 0;
    _VCR_gData.bMuteACK = BT_FALSE;
    _VCR_gData.bMuteWaitACK = BT_FALSE;
    _VCR_gData.bPTTIgnore = BT_FALSE;
    _VCR_gData.bHOOKIgnore = BT_FALSE;
    _VCR_gData.bFallbackLanguageChosen = BT_FALSE;
    _VCR_gData.bPlayerStopForLanguageChange = BT_FALSE;

    _VCR_gData.u16TxPromptID = 0;

    (void)EEPROM_enGetSingleByteFromRamMirror(&u8DisableDevMode, EEPROM_LAYOUT_ENUM_DeveloperTestmode);
    if (  u8DisableDevMode == 0x0 )
    {
    	VCR_sfEnablePerformanceMeasures(BT_FALSE);
    }
    else if ( u8DisableDevMode == 0x1)
    {
    	VCR_sfEnablePerformanceMeasures(BT_TRUE);
    }
    else
    {
    	VCR_sfEnablePerformanceMeasures(BT_TRUE);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfInitMenuValues                                    */
/**
    \brief      init parameter values for processing state menues

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfInitMenuValues(void)
{
    SuccessFailType sF = SUCCESS;

    _VCR_gData.stProcMenu.ePromptType       = AT_SIVR_PROMPT_MENU;
    _VCR_gData.stProcMenu.ePromptId         = AT_SIVR_PRMT_UNDEF;
    _VCR_gData.stProcMenu.command           = AT_SIVR_CMD_UNKNOWN;

    DEBUG_TEXT(VCR_SWI_DBG, MOD_VCR,"Init Menu Values");

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfInitResultValues                                      */
/**
    \brief      init parameter values for processing state responses

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfInitResultValues(void)
{
    SuccessFailType sF = SUCCESS;

    (void)memset(&(_VCR_gData.resultInfo),0x00,sizeof(VCRResultUnionType));
    _VCR_gData.resultInfo.number.type = VCR_RESULT_TYPE_NONE;

    DEBUG_TEXT(VCR_SWI_DBG, MOD_VCR,"Init VR-Result Values");

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sfInitMsgResultValues                                   */
/**
    \brief      init parameter values for processing state responses

    \param      none

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_sfInitMsgResultValues(void)
{
    SuccessFailType sF = SUCCESS;

    _VCR_gData.stMsgRes.u8NbrResults          = VCR_INVALID_NUMRES;
    _VCR_gData.stMsgRes.u8NbrReceivedResults  = 0;
    _VCR_gData.stMsgRes.number_valid          = BT_FALSE;
    _VCR_gData.stMsgRes.number_len            = 0;
    (void)memset(&(_VCR_gData.stMsgRes.number[0]),0x00, sizeof(ATSIVRNumberText));
    (void)memset(&(_VCR_gData.stMsgRes.stResults[0]),0x00,
            VCR_MAXNUM_RES_STORE * sizeof(VcrResultStructType));

    DEBUG_TEXT(VCR_SWI_DBG, MOD_VCR,"Init VR-MsgResult Values");

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSetCK5050Available                                        */
/**
    \brief      set indication if CK5050 is available

    \param      BooleanType BT_TRUE if CK5050 is up , BT_FALSE otherwise

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSetCK5050Available(BooleanType isAvail)
{
    if (BT_TRUE != isAvail)
    {   /* trigger menu change to IDLE */
        (void)_VCR_sFHandleEvent(BT_TRUE,   /* internal VCR event */
                          VCR_MEVT_CK5050_DOWN, /* event */
                          AT_SIVR_CMD_UNKNOWN  /* cmd is unused */
                          );
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_eGetDefaultReqLanguage                                  */
/**
    \brief      returns the default language

    \brief

    \param      nonoe

    \return     none
   */
/*----------------------------------------------------------------------------*/
ATSIVRLanguagesEnum _VCR_eGetDefaultReqLanguage(void)
{
    /* TODO - check where we get the really required language, either
     * from EPROM or from SD - so far we only use the defined default
     * within VCR */
    return _VCR_eDefaultLanguage;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_eSetDefaultReqLanguage                                  */
/**
    \brief      modifies the default language

    \brief

    \param      nonoe

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_eSetDefaultReqLanguage(ATSIVRLanguagesEnum value)
{
    SuccessFailType sF = SUCCESS;
     if (AT_SIRV_LANG_MAXNUM > value)
     {   /* modify the default language */
         _VCR_eDefaultLanguage = value;
     }
     else
     {
         DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Default Language ID %d oor",value);

         sF = FAIL;
     }
     return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSystemStopToIdle                                       */
/**
    \brief      stop SIVR engine and go back to idle

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSystemStopToIdle(BooleanType bOusted)
{

    /* we also have to take into account sync state!!! */
    if (bOusted == BT_FALSE)
    {
       // wenn die VR durch die Phone-APP verdraengt wird, beendet sich die VR von Parrot selbst
       if ( AT_SIVR_ENG_IDLE != _VCR_gData.stCtrlInfo.eEngineStatus &&
            AT_SIVR_ENG_BEGIN_SYNC != _VCR_gData.stCtrlInfo.eEngineStatus)
       {
           (void)_VCR_sFMessageToBeSend(VCR_MSG_STOP_ENGINE );
       }
       _VCR_gData.eSystemState    = VCR_SYSTEM_AVAILABLE;
    }
    else
    {
        if( (_VCR_gData.eSystemState == VCR_SYSTEM_STARTED_LANG) &&
            (_VCR_gData.stCtrlInfo.eEngineStatus == AT_SIVR_ENG_BEGIN_SYNC)
          )
        {
            /*Do not change the system state*/ //RT14189
            DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"VCR_vSystemStopToIdle do not change the system state because new language will be loaded");
        }
        else
        {
            if( AT_SIVR_ENG_BEGIN_PROMPT == _VCR_gData.stCtrlInfo.eEngineStatus)    //RT14866
            {
                _VCR_sFMessageToBeSend(VCR_MSG_STOP_STR_PRMT_PTT ); //RT14187
                DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"VCR_vSystemStopToIdle stop playing prompt for immidiate ending VCR");
            }
            _VCR_gData.eSystemState    = VCR_SYSTEM_AVAILABLE;
        }
    }

    _VCR_gData.eHapticTrigType = VCR_HAPTIC_NONE;

    _VCR_gData.bPTTunhandled = BT_FALSE;
    _VCR_gData.bHOOKunhandled  = BT_FALSE;
    _VCR_gData.PTT_contactNr = 0;
    _VCR_gData.bPTTIgnore = BT_FALSE;
    _VCR_gData.bHOOKIgnore = BT_FALSE;
    _VCR_gData.resultInfo.cStack.number_restriced = BT_FALSE;


    DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"SystemStopToIdle -> ReleaseMute");

    /* new Audio Manager*/
    APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enVoicePrompts, __LINE__);
    _VCR_gData.bMuteWaitACK = BT_FALSE;


    _VCR_vNotifyApplication(VCR_NOTIFY_SIVR_DEACTIVATE);

	// RS: removed, is now handled in AudioManager
    //APP_MDI_sfSetALCKMode(MDI_AUDIO_LOCK_STOP);

    _VCR_vChangeMenuToIdle();

}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_bNumberOrCallStackAvail                                 */
/**
    \brief      wrapper to check availability of number for redial

    \brief

    \param      nonoe

    \return     none
   */
/*----------------------------------------------------------------------------*/
BooleanType _VCR_bNumberOrCallStackAvail(void)
{
    return BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_bCheckLangChangeReq                                     */
/**
    \brief      function to check if language change is required

    \param      none

    \return     BooleanType BT_TRUE if language need to be changed
   */
/*----------------------------------------------------------------------------*/
BooleanType _VCR_bCheckLangChangeReq(void)
{
    BooleanType retVal = BT_FALSE;
    Uint8Type  i = 0;
    Uint8Type   tmpIdx    = (Uint8Type)AT_SIRV_LANG_MAXNUM;
    ATSIVRLanguagesEnum   defaultLang = AT_SIRV_LANG_MAXNUM;

    /* check if previous trigger has been done, if so only reset trigger
     * but do NOT check again and again */
    if (BT_TRUE != _VCR_gData.bTrigLangChange )
    {
        /* compare current language with required one due to initial Eprom readout
         * or change from VR itself or secondary display */
        if (_VCR_gData.eRequiredLang != _VCR_gData.eCurrentLang)
        {   /* we need to check if the new required language is supported ->
               search in current map */
            for (i=1U; (i <= _VCR_gData.u8NumSupLang && i < (Uint8Type)VCR_MAX_LANG_TABLE);i++)
            {
                if (_VCR_gData.eRequiredLang == _VCR_gData.eLangTable[i])
                {   /* the requested language is found, set index */
                    tmpIdx = i;

                    DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"requested language found %d",i);

                    retVal = BT_TRUE;
                    break;
                }
            }
            if (BT_FALSE == retVal)
            {   /* we do NOT have found the required language, check for
                   default language!! */

                defaultLang = _VCR_eGetDefaultReqLanguage();

                if (defaultLang != _VCR_gData.eCurrentLang)
                {
                    for (i=1U; (i <= _VCR_gData.u8NumSupLang && i < (Uint8Type)VCR_MAX_LANG_TABLE); i++)
                    {
                        if (defaultLang == _VCR_gData.eLangTable[i])
                        {   /* the requested language is found, set index */
                            tmpIdx = i;
                            retVal = BT_TRUE;
                            DEBUG_VALUE1(VCR_SWI_TRC, MOD_VCR,"default language found %d",i);

                            break;
                        }
                    }

                    if ( (Uint8Type)AT_SIRV_LANG_MAXNUM == tmpIdx )
                    {
                       DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"default language not found %d",defaultLang);
                    }
                }
                /* else no search and no change */
            }
            if (BT_TRUE == retVal)
            {   /* we need to set the new current language
                   before we trigger the change */
                _VCR_gData.u8CurrentLangIdx = tmpIdx;
                _VCR_gData.eCurrentLang     = _VCR_gData.eLangTable[tmpIdx];
            }
            else
            {
                if (_VCR_gData.eRequiredLang != _VCR_gData.eCurrentLang)
                {
                    for (i=0U; i< _VCR_gData.u8NumSupLang ;i++)
                    {
                         DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"%d eLangTable%d",
                                i,_VCR_gData.eLangTable[i]);
                    }
                }

            }
            DEBUG_VALUE4(VCR_SWI_TRC, MOD_VCR,"Languange change req %d cur %d cur_idx %d default %d",
                   _VCR_gData.eRequiredLang,
                   _VCR_gData.eCurrentLang, _VCR_gData.u8CurrentLangIdx,
                   defaultLang );

        }
        else
        {   /* current and required language shall map */
            if (BT_TRUE == _VCR_gData.bPlayerStopForLanguageChange)
            {
                _VCR_gData.bPlayerStopForLanguageChange = BT_FALSE;
                APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enVoicePrompts, __LINE__);
            }
            DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Current Language %d and required %d shall map",
                    _VCR_gData.eCurrentLang,_VCR_gData.eRequiredLang);

        }
    }

    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vChangeMenuToIdle                                       */
/**
    \brief      handler to go back to idle menu as this is needed several times

    \brief

    \param      nonoe

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vChangeMenuToIdle(void)
{
    (void)_VCR_sfInitMsgResultValues();
    (void)_VCR_sfInitResultValues();
    _VCR_gData.u8ProcErr = 0;
    (void)_VCR_sfInitMenuValues();
    _VCR_gData.eMenuState = AT_SIVR_MENU_IDLE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vChangeState                                            */
/**
    \brief      handler to change the state and take all required settings into
                account

    \brief

    \param      ATSIVRPromptEnum newState - new state to be set

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vChangeState(ATSIVRPromptEnum newState)
{
    _VCR_gData.eMenuState = newState;
    _VCR_gData.stProcMenu.ePromptId   = newState;
    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_MENU;
    _VCR_gData.u8ProcErr  = 0;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vChangeMenuToMain                                       */
/**
    \brief      handler to go back to main menu as this is needed several times

    \brief

    \param      nonoe

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vChangeMenuToMain(void)
{
    /* clean old result values and start processing SM */
    (void)_VCR_sfInitMsgResultValues();
    _VCR_vChangeState(AT_SIVR_MENU_MAIN);
    _VCR_vSendCmdToMainEventHandler(AT_SIVR_CMD_MAIN);
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vT1SorryRepeat                                          */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vT1SorryRepeat(void)
{
    _VCR_gData.stProcMenu.ePromptId   = AT_SIVR_PRMT_T1_SORRY;
    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vCancelStopFor3rdError                                  */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vCancelStopFor3rdError(void)
{
    /* set back error counter */
    _VCR_gData.u8ProcErr = 0;
    _VCR_gData.u8NumPromptRequired = 2;
    _VCR_gData.u8NumPromptSend = 0;
    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_PROCESS_CANCELLED;
    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_SYSTEM_STOP;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );

}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vCancelGotoMain                                         */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vCancelGotoMain(void)
{
    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend = 0;
    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_PROCESS_CANCELLED;
    _VCR_vChangeState(AT_SIVR_MENU_MAIN);
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );


}
/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vFunctionNotAvailableStop                               */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vFunctionNotAvailableStop(void)
{
    /* send prompt not supported */
    _VCR_gData.u8NumPromptRequired = 2;
    _VCR_gData.u8NumPromptSend = 0;
    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_FUNCTION_NOT_AVAILABLE;
    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_SYSTEM_STOP;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vFunctionNotAvailableMain                               */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vFunctionNotAvailableMain(void)
{
    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_FUNCTION_NOT_AVAILABLE;
    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend     = 0;
    _VCR_gData.eMenuState          = AT_SIVR_MENU_MAIN;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vFunctionNotAvailableMain                               */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vNumberNotAvailableMain(void)
{
    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_NUMBER_NOT_AVAIlABLE;
    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend     = 0;
    _VCR_gData.eMenuState          = AT_SIVR_MENU_MAIN;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vFunctionNotAvailableMain                               */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSmsDeletedMain(void)
{
    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SMS_DELETED;
    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend     = 0;
    _VCR_gData.eMenuState          = AT_SIVR_MENU_MAIN;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vPhoneNotAvailableStop                                  */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vPhoneNotAvailableStop(void)
{
    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_PHONE_NOT_AVAILABLE;
    _VCR_gData.ePromtTable.tab[1]  = AT_SIVR_PRMT_SYSTEM_STOP;
    _VCR_gData.u8NumPromptRequired = 2;
    _VCR_gData.u8NumPromptSend     = 0;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT );
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vFunctionNotSupported                                   */
/**
    \brief

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vFunctionNotSupportedStop(void)
{
    /* send prompt not supported */
    _VCR_gData.u8NumPromptRequired = 2;
    _VCR_gData.u8NumPromptSend = 0;
    _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_FUNCTION_NOT_AVAILABLE;
    _VCR_gData.ePromtTable.tab[1] = AT_SIVR_PRMT_SYSTEM_STOP;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vConnectPrompt                                          */
/**
    \brief     play connect prompt

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vConnectPrompt(void)
{
    DEBUG_TEXT (VCR_SWI_TRC, MOD_VCR,"ConnectPrompt no Categories");

    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend = 0;
    switch (_VCR_gData.resultInfo.number.type)
    {
         case VCR_RESULT_TYPE_CONTACT_UNI:
         {
             _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_CONNECT_NAME;
             break;
         }
         default:
         {
             _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_CONNECT;
             break;
         }
    }
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_CONNECT_PROMPT );
}


void _VCR_vConnectPromptCategory(void)
{
    DEBUG_TEXT (VCR_SWI_TRC, MOD_VCR,"ConnectPrompt with Categories");

    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend = 0;
    switch (_VCR_gData.resultInfo.number.type)
    {
         case VCR_RESULT_TYPE_CONTACT_UNI:
         {
             _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_CONNECT_NAME_CAT;
             break;
         }
         default:
         {
             _VCR_gData.ePromtTable.tab[0] = AT_SIVR_PRMT_CONNECT;
             break;
         }
    }
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_CONNECT_PROMPT );
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSinglePrompt                                           */
/**
    \brief     set required values to play a single prompt

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSinglePrompt(ATSIVRPromptEnum prompt)
{
    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend = 0;
    _VCR_gData.ePromtTable.tab[0] = prompt;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_PROMPT );
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSinglePrepare                                          */
/**
    \brief     set required values to prepare menu prompt

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vSinglePrepare(ATSIVRPromptEnum prompt)
{
    _VCR_gData.stProcMenu.ePromptId = prompt;
    _VCR_gData.stProcMenu.ePromptType = AT_SIVR_PROMPT_SYSTEM;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PREPARE_MENU );
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_bFCheckMDINeedToBePaused                                */
/**
    \brief      check if MDI is playing and need to be paused

    \brief

    \param      global data pointer

    \return     booleanType BT_TRUE if MDI need to be paused, BT_FALSE otherwise
   */
/*----------------------------------------------------------------------------*/
BooleanType _VCR_bFCheckMDINeedToBePaused(void)
{
    BooleanType bRetVal = BT_FALSE;
    /* TODO add check function from MDI here to get MDI status */
    if (BT_TRUE == APP_MDI_bPlayerActive())
    {   /* MDI need to be paused, set global reminder and return BT_TRUE */
        bRetVal = BT_TRUE;
    }
    /* else do nothing as we return default FALSE */

    return bRetVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFProcResultsUnique                                        */
/**
    \brief      check if there is result from msg available and copy to res
                storage

    \brief      here also for more than one result related to confidence and
                current context it is checked what to do.

    \param      pointer to BooleanType to indicate if result is unique

    \return     booleanType BT_TRUE if command is unique, BT_FALSE otherwise
   */
/*----------------------------------------------------------------------------*/
SuccessFailType   _VCR_sFProcResultsUnique(BooleanType * is_unique)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type i = 0;
    Uint8Type j = 0;
    Uint8Type k = 0;

    /* first additional cross check that there is at least one result from
     * msg storage available - if so go on */
    if (0 < _VCR_gData.stMsgRes.u8NbrResults &&
        0 < _VCR_gData.stMsgRes.u8NbrReceivedResults )
    {   /* we have at least one result */
        if (1 < _VCR_gData.stMsgRes.u8NbrReceivedResults)
        {   /* more than one result - check related to state and conf how to proceed */
            /* only for ambiguous names we take more than one result into account if the
             * result include the same command id */
            _VCR_gData.stMsgRes.u8NbrResults = 1;
        	if ( (AT_SIVR_MENU_DIALNAMECONFIRM == _VCR_gData.eMenuState ||
        	      AT_SIVR_MENU_DIALNAMECONFIRM_L == _VCR_gData.eMenuState ) &&
        	      AT_SIVR_CMD_DIAL == _VCR_gData.eCmdId)
        	{   /* check if more than one time DIAL command is available and align
        	       all dial commands */
        		for (i = 1U; i < _VCR_gData.stMsgRes.u8NbrReceivedResults; i++)
        		{
        			if (AT_SIVR_CMD_DIAL == _VCR_gData.stMsgRes.stResults[i].result_value)
        			{   /* we found another match, just increment number and go on */
        				_VCR_gData.stMsgRes.u8NbrResults = _VCR_gData.stMsgRes.u8NbrResults + 1;
        			}
        			else
        			{   /* check if we have already reached the last index, if so only clear */
                   DEBUG_VALUE4 (VCR_SWI_TRC, MOD_VCR,"remove result no dial %d NbrRecResults %d NbrResults %d",
                       i,
                       _VCR_gData.stMsgRes.u8NbrReceivedResults,
                       _VCR_gData.stMsgRes.u8NbrResults,
                       0);

        				if (i == _VCR_gData.stMsgRes.u8NbrReceivedResults - 1U)
        				{
        					(void)memset(&(_VCR_gData.stMsgRes.stResults[i]),
        							0x00,sizeof(VcrResultStructType));
        				}
        				else
        				{   /* there is an entry behind - copy last to this */
        					_VCR_gData.stMsgRes.u8NbrReceivedResults =
        						_VCR_gData.stMsgRes.u8NbrReceivedResults - 1;
        					(void)memcpy(&(_VCR_gData.stMsgRes.stResults[i]),
        						   &(_VCR_gData.stMsgRes.stResults[_VCR_gData.stMsgRes.u8NbrReceivedResults]),
        						   sizeof(VcrResultStructType));
        					(void)memset(&(_VCR_gData.stMsgRes.stResults[_VCR_gData.stMsgRes.u8NbrReceivedResults]),
        							0x00,sizeof(VcrResultStructType));
        					i--;
        				}
        			}
        		}
        		/* now also remove duplicate PB indexes due to CK5050 error */
        		/* loop over all elements */
        		for (j = 0U; j < _VCR_gData.stMsgRes.u8NbrResults; j++)
        		{
        		    k = j + 1U;
        		    for (i = j + 1U; i < _VCR_gData.stMsgRes.u8NbrResults; i++)
        		    {   /* check if index of i == index of j -> if so remove */
        		        if ( (_VCR_gData.stMsgRes.stResults[j].u.id.type) == AT_SIVR_RES_IDs &&
        			         (_VCR_gData.stMsgRes.stResults[j].u.id.type ==
        			          _VCR_gData.stMsgRes.stResults[k].u.id.type ) &&
        			         (AT_TRUE == _VCR_gData.stMsgRes.stResults[j].u.id.id1_available) &&
        			         (AT_TRUE == _VCR_gData.stMsgRes.stResults[k].u.id.id1_available) &&
        			         (_VCR_gData.stMsgRes.stResults[j].u.id.id1 ==
        			          _VCR_gData.stMsgRes.stResults[k].u.id.id1)                 )
        			    {   /* same PB index found - remove this entry */
        			        _VCR_gData.stMsgRes.u8NbrResults =
            		                _VCR_gData.stMsgRes.u8NbrResults - 1U;
            			    if (k == _VCR_gData.stMsgRes.u8NbrResults)
            				{   /* this was last index - just do memset */
            					(void)memset(&(_VCR_gData.stMsgRes.stResults[k]),
            							0x00,sizeof(VcrResultStructType));

                             DEBUG_VALUE4 (VCR_SWI_TRC, MOD_VCR,"remove LAST result same pbidx %d %d %d %d",
                               k,j,_VCR_gData.stMsgRes.stResults[j].u.id.id1,
                               _VCR_gData.stMsgRes.u8NbrResults);
        			            break; /* finish for loop */
            				}
            				else
            				{   /* there is an entry behind - copy last to this */
            					(void)memcpy(&(_VCR_gData.stMsgRes.stResults[k]),
            						   &(_VCR_gData.stMsgRes.stResults[_VCR_gData.stMsgRes.u8NbrResults]),
            						   sizeof(VcrResultStructType));
            					(void)memset(&(_VCR_gData.stMsgRes.stResults[_VCR_gData.stMsgRes.u8NbrResults]),
            							0x00,sizeof(VcrResultStructType));

                             DEBUG_VALUE4 (VCR_SWI_TRC, MOD_VCR,"remove NOT LAST result same pbidx %d %d %d %d",
                               k,j,_VCR_gData.stMsgRes.stResults[j].u.id.id1,
                               _VCR_gData.stMsgRes.u8NbrResults);
            					/* do not increment k to check the copied index again */
            				}
        		        }
        		        else
        		        {
        		            k++;
        		        }
        		        /* else do next element */
        		    }
        	    }
        		_VCR_gData.stMsgRes.u8NbrReceivedResults = _VCR_gData.stMsgRes.u8NbrResults;
        	}
        	else
        	{
        		_VCR_gData.stMsgRes.u8NbrReceivedResults = 1;
                _VCR_gData.stMsgRes.u8NbrResults         = 1;
        	}
        }

        if (1 < _VCR_gData.stMsgRes.u8NbrResults)
        {
            *is_unique = BT_FALSE;
        }
        else
        {
            *is_unique = BT_TRUE;
        }
    }
    else
    {
        sF = FAIL;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSendTriggerToMainEventHandler                          */
/**
    \brief      trigger state machine

    \brief

    \param      VCRMenuEventEnum
                trigger

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _VCR_vSendTriggerToMainEventHandler(VCRMenuEventEnum trigger)
{
	/* check trigger not yet pending */
    if (VCR_MEVT_UNDEF == _VCR_gData.eEventTrigMain)
    {
        _VCR_gData.eEventTrigMain = trigger;

        DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Event to MainEventHandler %d",_VCR_gData.eEventTrigMain);

        switch (trigger)
        {
            case VCR_MEVT_DELAY_LIST_READ:
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,
                        MSEC(_VCR_gData.u16ListDelay),   0);
                break;
            }
            case VCR_MEVT_WAIT_MUTE_ACK:  // Fallthrough !
            case VCR_MEVT_MUTE_ACK_START_ENGINE:
            case VCR_MEVT_CALL_PB_SELECTION:
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,
                        MSEC(50),   0);
                break;
            }
            default:
            {
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,
                        MSEC(VCR_PENDING_TIMER),   0);
                break;
            }
        }
    }
    else
    {
        DEBUG_VALUE1(VCR_SWI_ERR, MOD_VCR,"Event to MainEventHandler %d could not handled !!!",_VCR_gData.eEventTrigMain);

        return FAIL;
    }
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vSendCmdToMainEventHandler                              */
/**
    \brief      "send" a command retrigger to avoid deep function call

    \brief

    \param      none

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
void  _VCR_vSendCmdToMainEventHandler(
        ATSIVRCmdIdEnum          cmd)
{
    /* check trigger not yet pending */
    if (AT_SIVR_CMD_UNKNOWN == _VCR_gData.eCmdTrigMain)
    {
        _VCR_gData.eCmdTrigMain = cmd;
        DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Cmd to MainEventHandler %d",_VCR_gData.eCmdTrigMain);

        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_VCR_PENDING,  MSEC(1),   0);
    }
    else
    {
        DEBUG_VALUE1(VCR_SWI_ERR, MOD_VCR,"Cmd to MainEventHandler %d could not handled !!!",_VCR_gData.eCmdTrigMain);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFCheckResultAvailable                                  */
/**
    \brief      check that there is a result available and try to process it

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType  _VCR_sFCheckResultAvailable(void)
{
    SuccessFailType       sF = SUCCESS;
    VcrResultStructType   * p_res =   &(_VCR_gData.stMsgRes.stResults[0]);
    /* we have received result -> reset error counter */
    _VCR_gData.u8ProcErr = 0;
    /* note: this machine does NOT check if there are more than one
     * possible result codes, this will be done by the main menu machine
     * within the function _VCR_sFProcessResults where also the message
     * data results are filled to the menu data stRes itself which is then
     * used for the next available prompt reco */

    /* just check that there is at least one command */
    if (0 < _VCR_gData.stMsgRes.u8NbrResults &&
        0 < _VCR_gData.stMsgRes.u8NbrReceivedResults)
    {
        _VCR_gData.stProcMenu.command = p_res->result_value;

        if (AT_SIVR_CMD_STORE_NAME_REC >= _VCR_gData.stProcMenu.command)
        {
            _VCR_gData.eCmdId  = _VCR_gData.stProcMenu.command;
        }
        else
        {
            _VCR_gData.eCmdId = AT_SIVR_CMD_UNKNOWN;
        }
        DEBUG_VALUE2(VCR_SWI_TRC, MOD_VCR,"Cmd %d u8NbrResults %d",
                _VCR_gData.stProcMenu.command, _VCR_gData.stMsgRes.u8NbrResults);
    }
    else /* error case - cancel VR */
    {
        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR," No Command received - cancel VR");

        _VCR_gData.eCmdId = AT_SIVR_CMD_UNKNOWN;
        sF = FAIL;
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_sFBuildPrompt                                           */
/**
    \brief      build prompt with required options due to prompt id

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType  _VCR_sFBuildPrompt(ATReqVCMgtPromtType * rpmt)
{
    SuccessFailType       sF = SUCCESS;
    VCRResultUnionType  * p_res    = &(_VCR_gData.resultInfo);
    Uint32Type i = 0;
    Uint16Type PromptErrorLine = 0;
    ATPinCodeType pinCodeAsChar;

    /* check prompt type */

    switch (rpmt->prmt.other.prompt_type)
    {
        case AT_SIVR_PROMPT_MENU:
        {   /* list all those menu prompts here where additional info is required */
            rpmt->prmt.other.num_opt_avail = 0;
            switch (rpmt->prmt.other.prompt_id)
            {

                case AT_SIVR_MENU_CALLSTACKS:
                case AT_SIVR_MENU_CALLSTACKS_L:
                {
                    rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;

                    switch (_VCR_gData.eCurrentLang)
                    {

                        case AT_SIVR_LANG_GERMAN:
                        {   /* german */
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Anruflisten");
                            break;
                        }
                        case AT_SIVR_LANG_FRENCH:
                        {   /* french */
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"listes d'appels");
                            break;
                        }
                        case AT_SIVR_LANG_FRENCH_CAN:
                        {   /* french */
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"historique d'appels");
                            break;
                        }
                        case AT_SIVR_LANG_SPANISH:
                        case AT_SIVR_LANG_US_SPANISH:
                        {   /* spain */
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Registro de llamadas");
                            break;
                        }
                        case AT_SIVR_LANG_ITALIAN:
                        {
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"registri delle chiamate");
                            break;
                        }
                        case AT_SIVR_LANG_DUTCH:
                        {
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"oproeplijsten");
                            break;
                        }
                        case AT_SIVR_LANG_PORTUGUES:
                        {
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"listas de chamadas");
                            break;
                        }
///* ToDo
                        case AT_SIVR_LANG_RUSSIAN:                                                                              // RT#818 Anruflisten Tested OK mit Vasil
                        {
                        (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD1,0x81,
																																							0xD0,0xBF,
																																							0xD0,0xB8,
																																							0xD1,0x81,
																																							0xD0,0xBA,
																																							0xD0,0xB8,
																																							0x20,
                                                                                                                                                            0xD0,0xB2,
																																							0xD1,0x8B,
                                                                                                                                                            0xD0,0xB7,
																																							0xD0,0xBE,
                                                                                                                                                            0xD0,0xB2,
																																							0xD0,0xBE,
                                                                                                                                                            0xD0,0xB2);
                            break;
                        }
//*/
                        case AT_SIVR_LANG_CZECH:
                        {
//                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"seznamy volání");
                            // http://rishida.net/tools/conversion/
                            /* RT#2377*/
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"seznamy vol%c%cn%c%c",0xC3,0xA1,0xC3,0xAD);
                            break;
                        }
                        case AT_SIVR_LANG_UK_ENG:
                        case AT_SIVR_LANG_US_ENG:
                        default:
                        {   /* english as default */
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"call history");
                            break;
                        }
                    }
                    rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                    break;
                }
                case AT_SIVR_MENU_DIALNAMECONFIRM:
                case AT_SIVR_MENU_DIALNAMECONFIRM_L:
                {   /* we need to add v_chosen item and v_chosenLocation and therefore also item type */
                    /* check that current dialinfo type match */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 3;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.item_id);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.item_type);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        /* check for location - avoid sending invalid location, set fix to default */
                        if (BT_TRUE == p_res->contact_uni.location_avail)
                        {
                            // RT#4940
                            if ( p_res->contact_uni.n > 1)
                            {
                               (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.location_id);
                            }
                            else
                            {
                                if ( (p_res->contact_uni.n == 1) && (p_res->contact_uni.n_location[0] != 0))
                                {
                                   /*RT#3611 VR says "At Home" instead of "mobile"*/
                                   /* if we have only one number, I will check the phone number type and add it  */
                                  (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.n_location[0]);
                                }
                                else
                                {
                                   /* set fixed to default, this does not speak anything */
                                   (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",AT_SIVR_PRMT_LOC_DEFAULT);
                                }
                            }
                        }
                        else
                        {
                            // RT#5518 Wrong Phone number category
                            // speak the loaction only if the user spoke the command dial(d_name)_(d_category)

                            (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",AT_SIVR_PRMT_LOC_DEFAULT);
                        }
                        rpmt->prmt.other.opt_len[2] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[2][0]));
                    }
                    else
                    {
                        sF = FAIL;
                         PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_DIALNUMBERCORRECT:
                case AT_SIVR_MENU_DIALNUMBERCORRECT_L:
                {
                    /* check that current dialinfo type match */
                    if (VCR_RESULT_TYPE_NUMBER == p_res->number.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 1;

                        rpmt->prmt.other.opt_len[0]    = 0;
                        /* we have to add blanks for the output string */

                        /* rangecheck */
                        if (p_res->number.number_startRead > AT_MAX_PHONE_NUM_LEN)
                        {
                           p_res->number.number_startRead = 0;
                        }

                        for (i=p_res->number.number_startRead; ( i < p_res->number.number_len &&
                                   i < (Uint32Type)AT_SIVR_MAX_PRMT_OPT_LEN/2);i++)
                        {
                            rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]]   = 0x20;
                            rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]+1] = p_res->number.number[i];
                            rpmt->prmt.other.opt_len[0] = rpmt->prmt.other.opt_len[0] + 2;
                        }
                        rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]] = '\0';

                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES:
                case AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES_L:
                {   /* add available categories - TODO do we need to split in 2 params? */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
#ifdef UHV_ACTIVATE_FEATURE_VCR_SOP3_PROMPTS
                        switch (_VCR_gData.eCurrentLang)
                        {
                            case AT_SIRV_LANG_MAXNUM:
                            {   // todo: dummy item for possible language which are not change to SOP3 specification
                                rpmt->prmt.other.num_opt_avail = 1;
                                (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.avail_loc);
                                rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                                break;
                            }
                            default:
                            {
                                rpmt->prmt.other.num_opt_avail = 3;
                                (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.item_id);
                                rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                                (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.item_type);
                                rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                                (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.avail_loc);
                                rpmt->prmt.other.opt_len[2] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[2][0]));
                                break;
                            }
                        }  // end switch case VCR_gData.eCurrentLang
#else
                        rpmt->prmt.other.num_opt_avail = 1;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.avail_loc);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));

#endif
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT:
                case AT_SIVR_MENU_CHANGEBLUETOOTHPINCORRECT_L:
                {
                    if (VCR_RESULT_TYPE_BT_PIN == p_res->bTPin.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 1;
                        /* fill in BT pin but add blanks */
                        rpmt->prmt.other.opt_len[0] = 0;
                        for (i=0U; ( i < p_res->bTPin.pin_len &&
                                i < (Uint32Type)AT_SIVR_MAX_PRMT_OPT_LEN/2);i++)
                        {
                            rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]]   = 0x20;
                            rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]+1] = p_res->bTPin.pin[i];
                            rpmt->prmt.other.opt_len[0] = rpmt->prmt.other.opt_len[0] + 2;
                        }
                        rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]] = '\0';
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_RESETNAMECONFIRM:
                case AT_SIVR_MENU_RESETNAMECONFIRM_L:
                case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS:
                case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS_L:
                {
                    if (VCR_RESULT_TYPE_PLAYPB == p_res->playPB.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 2;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->playPB.cur_entry);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",AT_SIVR_ITEMTYPE_CONTACT);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_MUSIC:
                case AT_SIVR_MENU_MUSIC_L:
                {
                    /* only if a media player is connected with remote control but no telephone is connected */
                    /* the  the promptid  change to AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED                  */
                    if((BT_FALSE == ERM_bPhoneAvailable()) && (BT_TRUE == APP_MDI_bPlayerAvailable()) &&\
                       (BT_TRUE == APP_MDI_bRemoteControlAvailable()))
                    {
                        rpmt->prmt.other.num_opt_avail = 0;
                        if (_VCR_gData.bVoiceFBOff)
                        {
                            /* short dialog */
                            rpmt->prmt.other.prompt_id = AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED;
                        }
                        else
                        {
                            /* long dialog */
                            rpmt->prmt.other.prompt_id = AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED_L;
                        }
                        rpmt->prmt.other.prompt_type = AT_SIVR_PROMPT_SYSTEM;
                        }
                    break;
                }
                default:
                {   /* do not add any information */
                    break;
                }

            }
            break;
        }
        case AT_SIVR_PROMPT_HELP:
        {
            rpmt->prmt.other.num_opt_avail = 0;
            switch (rpmt->prmt.other.prompt_id)
            {

                case AT_SIVR_MENU_DIALNAMECONFIRM:
                case AT_SIVR_MENU_DIALNAMECONFIRM_L:
                {
                    /* RT#2464 add (d_name) and [(d_category)]  */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 3;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_id);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_type);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        /* check for location - avoid sending invalid location, set fix to default */
                        if (BT_TRUE == p_res->contact_uni.location_avail)
                        {
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "%d",p_res->contact_uni.location_id);
                        }
                        else
                        {   /* set fixed to default */
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "%d",AT_SIVR_PRMT_LOC_DEFAULT);
                        }
                        rpmt->prmt.other.opt_len[2] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[2][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES:
                case AT_SIVR_MENU_DIALNAMECONFIRMCATEGORIES_L:
                {   /* add available categories */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
#ifdef UHV_ACTIVATE_FEATURE_VCR_SOP3_PROMPTS
                        rpmt->prmt.other.num_opt_avail = 3;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.item_id);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.item_type);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.avail_loc);
                        rpmt->prmt.other.opt_len[2] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[2][0]));
#else
                        rpmt->prmt.other.num_opt_avail = 1;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.avail_loc);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
#endif
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS:
                case AT_SIVR_MENU_PLAYADDRESSBOOKOPTIONS_L:
                {   /* add name as item */
                	if (VCR_RESULT_TYPE_PLAYPB == p_res->playPB.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 2;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->playPB.cur_entry);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                        		"%d",AT_SIVR_ITEMTYPE_CONTACT);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                	break;
                }
                default:
                {   /* no further infos to be added to prompt */
                    break;
                }
            }
            break;
        }
        /* not custom prompts are mapped within system prompts within SW, therefore specific
         * ids are defined */
        case AT_SIVR_PROMPT_CUSTOM:
        case AT_SIVR_PROMPT_SYSTEM:
        {   /* list all those menu prompts here where additional info is required */
            /* NOTE: As long prompt causes several issues not used so far!!! */

            if ( ((Uint16Type)AT_SIVR_LONG_SYSPROMPT_OFFSET <= rpmt->prmt.other.prompt_id &&
                  (Uint16Type)AT_SIVR_PRMT_CK5050_MAXVALUE  >= rpmt->prmt.other.prompt_id)  )
            {
                switch (rpmt->prmt.other.prompt_id)
                {
                    case AT_SIVR_PRMT_CHOSEN_LIST:
                    case AT_SIVR_PRMT_L_READ_LIST:
                    case AT_SIVR_PRMT_AMBIGUOUS_NAME:
                    case AT_SIVR_PRMT_X_NUMBER:
                    {   /* no long prompt supported */
                        break;
                    }
                    default:
                    {
                        if (BT_TRUE != _VCR_gData.bVoiceFBOff)
                        {   /* use the long system prompt */
                            rpmt->prmt.other.prompt_id = (rpmt->prmt.other.prompt_id +
                                    (Uint16Type)AT_SIVR_LONG_SYSPROMPT_OFFSET);
                        }
                        break;
                    }
                }
            }

            switch (rpmt->prmt.other.prompt_id)
            {
                case AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES:
                case AT_SIVR_MENU_DIALNAMECONFIRMRESOLVEAMBINAMES_L:
                {   /* remap type */
                    rpmt->prmt.other.prompt_type = AT_SIVR_PROMPT_MENU;
                    break;
                }
                case AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY:
                case AT_SIVR_MENU_RESOLVEMULTIPLENUMPERCATEGORY_L:
                {   /* NOTE: if this menu prompt is indicated as custom it
                       mean we only shall start reco with blank string */
                    rpmt->prmt.other.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                	(void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN," ");
                    rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                	break;
                }
                case AT_SIVR_PRMT_D_NAME:
                case AT_SIVR_PRMT_D_NAME_L:
                {   /* use haptical index to play contact */
                    rpmt->prmt.other.num_opt_avail = 2;
                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                            "%d",_VCR_gData.u16HapticIdx);
                    rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                    (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                            "%d",AT_SIVR_ITEMTYPE_CONTACT);
                    rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                    break;
                }
                case AT_SIVR_PRMT_D_TEXTTAG:
                case AT_SIVR_PRMT_D_TEXTTAG_L:
                case AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK_OPTIONS:
                case AT_SIVR_PRMT_T2_OPTIONS_PLAY_ADDRESSBOOK_OPTIONS_L:
                {   /* add item id and item type - fixed contact */
                    if (VCR_RESULT_TYPE_PLAYPB == p_res->playPB.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 2;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->playPB.cur_entry);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",AT_SIVR_ITEMTYPE_CONTACT);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                    }
                    else if (VCR_RESULT_TYPE_CONTACT_AMB == _VCR_gData.resultInfo.contact_amb.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 2;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_amb.item_id[p_res->contact_amb.current_num]);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",AT_SIVR_ITEMTYPE_CONTACT);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }

                    break;
                }
                case AT_SIVR_PRMT_CHOSEN_LIST_VOICETAG:
                {
                    rpmt->prmt.other.prompt_id = AT_SIVR_PRMT_D_TEXTTAG;
                    rpmt->prmt.other.num_opt_avail = 2;
                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                            "%d",_VCR_gData.resultInfo.cStack.cur_entryPBindex);
                    rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                    (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                            "%d",AT_SIVR_ITEMTYPE_CONTACT);
                    rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));

                    break;
                }
                //RT#7897  VR dial_number_help_one_prompt_is_missing
                // add a special handling for this case, beause normally after the Help prompt we get directly the Beep
                case AT_SIVR_PRMT_HELP_DIAL_NAME_CONFIRM:
                {
                     rpmt->prmt.other.num_opt_avail = 0;
                     if (_VCR_gData.bVoiceFBOff)
                     {
                        rpmt->prmt.other.prompt_id = AT_SIVR_MENU_DIALNUMBERCORRECT;
                     }
                     else
                     {
                        rpmt->prmt.other.prompt_id = AT_SIVR_MENU_DIALNUMBERCORRECT_L;
                     }
                     rpmt->prmt.other.prompt_type = AT_SIVR_PROMPT_HELP;

                     break;
                }
                case AT_SIVR_PRMT_T2_OPTIONS_RESET_NAME_CONFIRM:
                case AT_SIVR_PRMT_T2_OPTIONS_RESET_NAME_CONFIRM_L:
                case AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME:
                case AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_L:
                case AT_SIVR_PRMT_CONNECT_NAME:
                case AT_SIVR_PRMT_CONNECT_NAME_L:
                case AT_SIVR_PRMT_NAME_STORED:
                case AT_SIVR_PRMT_NAME_STORED_L:
                case AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE:
                case AT_SIVR_PRMT_NAME_CATEGORY_NOT_AVAILABLE_L:
                case AT_SIVR_PRMT_I_DIALNAME_CONFIRM_NAME:
                case AT_SIVR_PRMT_I_DIALNAME_CONFIRM_NAME_L:
                {   /* add name */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 2;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_id);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_type);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        /* check for location - avoid sending invalid location, set fix to default */

                    }
                    else if (VCR_RESULT_TYPE_PLAYPB == p_res->playPB.type)
                    {   /* use the item id from play addressbook */
                        rpmt->prmt.other.num_opt_avail = 2;
                        if (BT_FALSE == _VCR_gData.resultInfo.playPB.is_add_new_contact)
                        {
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "%d",p_res->playPB.cur_entry);
                            rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "%d",AT_SIVR_ITEMTYPE_CONTACT);
                            rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        }
                        else
                        {
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "0");
                            rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "0");
                            rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        }
                        /* check for location - avoid sending invalid location, set fix to default */
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_PRMT_DO_YOU_WANT_TO_DIAL_NAME_CATEGORY:
                case AT_SIVR_PRMT_DO_YOU_WANT_TO_DIAL_NAME_CATEGORY_L:
                {   /* we need to add a available category here */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 3;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_id);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_type);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        /* the given location was not available - check if there is different
                         * one available and use this instead */
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.n_location[0]);
                        rpmt->prmt.other.opt_len[2] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[2][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }



                    break;
                }
                case AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE:
                case AT_SIVR_PRMT_ONLY_CATEGORY_AVAILABLE_L:  // RT#4808
                {
                    /* we need to add only the available category here -> One Paramter  */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
                       rpmt->prmt.other.num_opt_avail = 1;

                       if (p_res->contact_uni.n_location[0] != 0)
                       {
                          /* if we have only one category  I will add the fist (because all should be the same) */
                         (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",p_res->contact_uni.n_location[0]);
                       }
                       else
                       {
                          /* set fixed to default, this does not speak anything */
                          (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",AT_SIVR_PRMT_LOC_DEFAULT);
                       }
                       rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME:
                case AT_SIVR_PRMT_SIMILAR_TO_NAME1_TRAINED_NAME_L:
                {
                    /* we need to add Two Paramter  */
                    rpmt->prmt.other.num_opt_avail = 2;

                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",_VCR_gData.u16SimiliarNameVT);
                    (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%d",AT_SIVR_ITEMTYPE_CONTACT);

                    rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                    rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                    break;
                }
                case AT_SIVR_PRMT_CONNECT_NAME_CAT:
                case AT_SIVR_PRMT_CONNECT_NAME_CAT_L:
                case AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_CAT:
                case AT_SIVR_PRMT_T2_DIALNAME_CONFIRM_NAME_CAT_L:
                {
                    if (VCR_RESULT_TYPE_CONTACT_UNI == p_res->number.type)
                    {
                        rpmt->prmt.other.num_opt_avail = 3;
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_id);
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                        (void)snprintf((char *)&(rpmt->prmt.other.opt[1][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                "%d",p_res->contact_uni.item_type);
                        rpmt->prmt.other.opt_len[1] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[1][0]));
                        /* check for location - avoid sending invalid location, set fix to default */
                        if (BT_TRUE == p_res->contact_uni.location_avail)
                        {
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "%d",p_res->contact_uni.location_id);
                        }
                        else
                        {   /* set fixed to default */
                            (void)snprintf((char *)&(rpmt->prmt.other.opt[2][0]),AT_SIVR_MAX_PRMT_OPT_LEN,
                                    "%d",AT_SIVR_PRMT_LOC_DEFAULT);
                        }
                        rpmt->prmt.other.opt_len[2] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[2][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_PRMT_AMBIGUOUS_NAME:
                case AT_SIVR_PRMT_AMBIGUOUS_NAME_L:
                {   /* indicate the index of the name */
                    if (VCR_RESULT_TYPE_CONTACT_AMB == p_res->contact_amb.type)
                    {

                        rpmt->prmt.other.num_opt_avail = 1;
                        switch (_VCR_gData.eCurrentLang)
                        {
                            case AT_SIVR_LANG_GERMAN:
                            {
                                /*3522*/
                                /* Ambiguous names 2nd not correctly named "zwoter" was spoken*/
                                if (p_res->contact_amb.current_num+1 == 2)
                                {
                                   (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                            AT_SIVR_MAX_PRMT_OPT_LEN,
                                            "zweiter");
                                }
                                else
                                {
                                   (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                            AT_SIVR_MAX_PRMT_OPT_LEN,
                                            "%d..",
                                            p_res->contact_amb.current_num+1);

                                }
                                break;
                            }
                            case AT_SIVR_LANG_CZECH:
                            {
                                /*2857 CZ SDS: Dial name - used wrong prompt  */
                                /* Adding correct Czech translations  */
                                switch ( p_res->contact_amb.current_num+1 )
                                {
                                    case 1:
                                    {
                                        // první
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                                 AT_SIVR_MAX_PRMT_OPT_LEN,
                                                 "prvn%c%c", 0xC3,0xAD);
                                        break;
                                    }
                                    case 2:
                                    {
                                        // druhé
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                                 AT_SIVR_MAX_PRMT_OPT_LEN,
                                                 "druh%c%c", 0xC3, 0xA9);
                                        break;
                                    }
                                    case 3:
                                    {
                                        // tr{with wedge}etí
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                                 AT_SIVR_MAX_PRMT_OPT_LEN,
                                                 "t%c%cet%c%c", 0xC5,0x99, 0xC3,0xAD);
                                        break;
                                    }
                                    case 4:
                                    {
                                        // c{with wedge}tvrté
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                                 AT_SIVR_MAX_PRMT_OPT_LEN,
                                                 "%c%ctvrt%c%c", 0xC4,0x8D, 0xC3, 0xA9);
                                        break;
                                    }
                                    case 5:
                                    {
                                        // páté
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                                 AT_SIVR_MAX_PRMT_OPT_LEN,
                                                 "p%c%ct%c%c", 0xC3,0xA1, 0xC3,0xA9);
                                        break;
                                    }

                                    default:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                                 AT_SIVR_MAX_PRMT_OPT_LEN,
                                                 "%d.",
                                                 p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                }
                                break;
                            }
                            case AT_SIVR_LANG_US_ENG:
                            case AT_SIVR_LANG_UK_ENG:
                            {
                                switch ( p_res->contact_amb.current_num+1 )
                                {
                                    case 1:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                             AT_SIVR_MAX_PRMT_OPT_LEN,
                                             "%dst",
                                             p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                    case 2:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                             AT_SIVR_MAX_PRMT_OPT_LEN,
                                             "%dnd",
                                             p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                    case 3:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                             AT_SIVR_MAX_PRMT_OPT_LEN,
                                             "%drd",
                                             p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                    default:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                             AT_SIVR_MAX_PRMT_OPT_LEN,
                                             "%dth",
                                             p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                }
                                break;
                            }
                            case AT_SIVR_LANG_US_SPANISH:
                            case AT_SIVR_LANG_SPANISH:
                            {
                                (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                         AT_SIVR_MAX_PRMT_OPT_LEN,
                                         "%do",
                                         p_res->contact_amb.current_num+1);
                                break;
                            }
                            case AT_SIVR_LANG_FRENCH_CAN:
                            case AT_SIVR_LANG_FRENCH:
                            {
                                switch ( p_res->contact_amb.current_num+1 )
                                {
                                    case 1:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                             AT_SIVR_MAX_PRMT_OPT_LEN,
                                             "%der",
                                             p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                    default:
                                    {
                                        (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                             AT_SIVR_MAX_PRMT_OPT_LEN,
                                             "%deme",
                                             p_res->contact_amb.current_num+1);
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),
                                         AT_SIVR_MAX_PRMT_OPT_LEN,
                                         "%d.",
                                         p_res->contact_amb.current_num+1);
                                break;
                            }
                        }
                        rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)&(rpmt->prmt.other.opt[0][0]));
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }


                /* workaround as AT_SIVR_PRMT_X_NUMBER does not work so far!!! */
                case AT_SIVR_PRMT_X_NUMBER:
                case AT_SIVR_PRMT_X_NUMBER_L:
                {   /* copy number from uni code number  */
                    /* check that current dialinfo type match */
                    /* and use custom type */
                    rpmt->prmt.custom.prompt_type  = AT_SIVR_PROMPT_CUSTOM;

                    /* check for type */
                    if (VCR_RESULT_TYPE_CONTACT_UNI == _VCR_gData.resultInfo.contact_uni.type)
                    {   /* get current num value */
                        VCRResultContactUniType * p_cur = &_VCR_gData.resultInfo.contact_uni;
                        if (0 < p_cur->phone_number_len[p_cur->current_num])
                        {   /* copy */
                            rpmt->prmt.custom.text_len = 0;
                            /* we have to add blanks for the output string */
                            for (i=0U; ( i< p_cur->phone_number_len[p_cur->current_num] &&
                                       i <  (Uint32Type)AT_SIVR_MAX_PROMPT_TEXT_LEN/2);i++)
                            {
                                rpmt->prmt.custom.text[rpmt->prmt.custom.text_len]   = 0x20;
                                rpmt->prmt.custom.text[rpmt->prmt.custom.text_len+1] = p_cur->phone_number[p_cur->current_num][i];
                                rpmt->prmt.custom.text_len = rpmt->prmt.custom.text_len + 2;
                            }
                            rpmt->prmt.custom.text[rpmt->prmt.custom.text_len] = '\0';
                        }
                        else
                        {
                            sF = FAIL;
                            PromptErrorLine = __LINE__;
                        }
                    }
                    else if (VCR_RESULT_TYPE_NUMBER == _VCR_gData.resultInfo.number.type)
                    {   /* we can directly use the number */
                        VCRResultNumberType * p_cur = &_VCR_gData.resultInfo.number;
                        if (0 < p_cur->number_len)
                        {   /*  we have to add blanks for the output string */
                            rpmt->prmt.custom.text_len = 0;
                            /* we have to add blanks for the output string */
                            for (i=0U; ( i< p_cur->number_len &&
                                       i < (Uint32Type)AT_SIVR_MAX_PROMPT_TEXT_LEN/2);i++)
                            {
                                rpmt->prmt.custom.text[rpmt->prmt.custom.text_len]   = 0x20;
                                rpmt->prmt.custom.text[rpmt->prmt.custom.text_len+1] = p_cur->number[i];
                                rpmt->prmt.custom.text_len = rpmt->prmt.custom.text_len + 2;
                            }
                            rpmt->prmt.custom.text[rpmt->prmt.custom.text_len] = '\0';
                        }
                        else
                        {
                            sF = FAIL;
                            PromptErrorLine = __LINE__;
                        }
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_PRMT_SMS_XPLUSPLUS:
                case AT_SIVR_PRMT_SMS_XPLUSPLUS_L:
                {   /* add sms text stored in result to custom text */
                    VCRResultReadSMSType * p_cur = &_VCR_gData.resultInfo.readSMS;
                    rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                    if (VCR_RESULT_TYPE_READSMS == p_cur->type)
                    {   /* check if we need to split the SMS */
                        if (AT_SIVR_MAX_PROMPT_TEXT_LEN < (p_cur->message_len - p_cur->offset_len))
                        {
                            rpmt->prmt.custom.text_len = AT_SIVR_MAX_PROMPT_TEXT_LEN;
                        }
                        else
                        {
                            rpmt->prmt.custom.text_len = p_cur->message_len;
                        }
                        (void)memcpy(&(rpmt->prmt.custom.text),&(p_cur->message[p_cur->offset_len]),rpmt->prmt.custom.text_len);
                        p_cur->offset_len = rpmt->prmt.custom.text_len;
                        /* remove ' from prompt text */
                        for (i = 0U; i < rpmt->prmt.custom.text_len; i++)
                        {
                            if (AT_ASCII_QUOTE == rpmt->prmt.custom.text[i])
                            {   /* set quote to blank */
                                rpmt->prmt.custom.text[i] = 0x20;
                            }
                        }
                        rpmt->prmt.custom.text[rpmt->prmt.custom.text_len] = '\0';
                        _VCR_gData.resultInfo.readSMS.is_first_read_sms = BT_FALSE;
                    }
                    else
                    {
                        sF = FAIL;
                        PromptErrorLine = __LINE__;
                    }
                    break;
                }
                case AT_SIVR_PRMT_CHOSEN_LIST:
                {
                    if (VCR_RESULT_TYPE_CALLSTACKS == _VCR_gData.resultInfo.cStack.type)
                    {   /* we can directly use the number */
                        VCRResultCallstackType * p_cur = &_VCR_gData.resultInfo.cStack;

                        //RT#1476 ansetzen
                        if (p_cur->name_avail == BT_TRUE)
                        {
                            /* use name */
                            rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                            (void)memcpy(&rpmt->prmt.custom.text, &p_cur->name,p_cur->name_len);
                            rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                        }
                        else if (0 < p_cur->phone_number_len)
                        {
                            /* no name available use number */
                            /* copy but use custom prompt */

                            rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                            /* copy string to text */
                            rpmt->prmt.custom.text_len = 0;
                            /* we have to add blanks for the output string */
                            for (i=0U; ( i< p_cur->phone_number_len &&
                                       i < (Uint32Type)AT_SIVR_MAX_PROMPT_TEXT_LEN/2);i++)
                            {
                                rpmt->prmt.custom.text[rpmt->prmt.custom.text_len]   = 0x20;
                                rpmt->prmt.custom.text[rpmt->prmt.custom.text_len+1] = p_cur->phone_number[i];
                                rpmt->prmt.custom.text_len = rpmt->prmt.custom.text_len + 2;
                            }
                            rpmt->prmt.custom.text[rpmt->prmt.custom.text_len] = '\0';
                        }
                        else
                        {
                            sF = FAIL;
                            PromptErrorLine = __LINE__;
                        }
                    }
                    break;
                }

                case AT_SIVR_PRMT_NO_ENTRY_IN_CHOSEN_LIST:          /*RT#2673*/
                case AT_SIVR_PRMT_NO_ENTRY_IN_CHOSEN_LIST_L:
                case AT_SIVR_PRMT_NO_FURTHER_ENTRIES_IN_LIST_REPEAT:
                case AT_SIVR_PRMT_NO_FURTHER_ENTRIES_IN_LIST_REPEAT_L:
                {
                    rpmt->prmt.other.num_opt_avail = 1;
                    switch (_VCR_gData.eCurrentLang)
                    {
                        case AT_SIVR_LANG_GERMAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Gewaehlte Rufnummern");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Angenommene Anrufe");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Anrufe in Abwesenheit");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;

                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_FRENCH:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Numéros composés");
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Num%c%cros compos%c%cs", 0xC3, 0xA9, 0xC3, 0xA9);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Appels reçus");
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Appels re%c%cus", 0xC3, 0xA7);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)(char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Appels en absence");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_FRENCH_CAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"appels composés");
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"appels compos%c%cs", 0xC3, 0xA9);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Appels reçus");
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Appels re%c%cus", 0xC3, 0xA7);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Appels en absence");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_SPANISH:
                        case AT_SIVR_LANG_US_SPANISH:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Últimas llamadas");
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%c%cltimas llamadas", 0xC3, 0x9A);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Llamadas recibidas");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Llamadas perdidas");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_ITALIAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"chiamate effettuate");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Chiamate ricevute");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"chiamate perse");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_DUTCH:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"gekozen nummers");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Ontvangen oproepen");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"gemiste oproepen");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_PORTUGUES:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"números marcados");
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"n%c%cmeros marcados", 0xC3, 0xBA);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Chamadas aceites");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"chamadas perdidas");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
//                        /* Todo                                                                                              RT#818
                        case AT_SIVR_LANG_RUSSIAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed: // Dialed tested OK mit Vasil
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD0,0x9D,
																																													0xD0,0xB0,
																																													0xD0,0xB1,
																																													0xD1,0x80,
																																													0xD0,0xB0,
																																													0xD0,0xBD,
																																													0xD0,0xBD,
																																													0xD1,0x8B,
																																													0xD0,0xB5,
																																													0x20,
																																													0xD0,0xBD,
																																													0xD0,0xBE,
																																													0xD0,0xBC,
																																													0xD0,0xB5,
																																													0xD1,0x80,
																																													0xD0,0xB0);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD0,0x9F,
																																												0xD1,0x80,
																																												0xD0,0xB8,
																																												0xD0,0xBD,
																																												0xD1,0x8F,
																																												0xD1,0x82,
																																												0xD1,0x8B,
																																												0xD0,0xB5,
																																												0x20,
																																												0xD0,0xB2,
																																												0xD1,0x8B,
																																												0xD0,0xB7,
																																												0xD0,0xBE,
																																												0xD0,0xB2,
																																												0xD1,0x8B);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD0,0xBF,
                                                                                                                                                                                0xD1,0x80,
                                                                                                                                                                                0xD0,0xBE,
                                                                                                                                                                                0xD0,0xBF,
                                                                                                                                                                                0xD1,0x83,
                                                                                                                                                                                0xD1,0x89,
                                                                                                                                                                                0xD0,0xB5,
                                                                                                                                                                                0xD0,0xBD,
                                                                                                                                                                                0xD0,0xBD,
                                                                                                                                                                                0xD1,0x8B,
                                                                                                                                                                                0xD0,0xB5,
                                                                                                                                                                                0x20,
                                                                                                                                                                                0xD0,0xB2,
                                                                                                                                                                                0xD1,0x8B,
                                                                                                                                                                                0xD0,0xB7,
                                                                                                                                                                                0xD0,0xBE,
                                                                                                                                                                                0xD0,0xB2,
                                                                                                                                                                                0xD1,0x8B);
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
//                        */
                        case AT_SIVR_LANG_CZECH:
                        {
                           switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"volaná císla");
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"volan%c%c c%c%csla", 0xC3, 0xA1, 0xC3, 0xAD);
// RT#3363
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"volan%c%c %c%c%c%csla", 0xC3, 0xA1, 0xC4, 0x8D, 0xC3, 0xAD);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Prijaté hovory");
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Prijat%c%c hovory", 0xC3, 0xA9);

                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"P%c%cijat%c%c hovory",0xC5, 0x99, 0xC3, 0xA9);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"zmeškané hovory"); // RT#2468
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"zme%c%ckan%c%c hovory",0xC5, 0xA1, 0xC3, 0xA9 );
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_UK_ENG:
                        case AT_SIVR_LANG_US_ENG:
                        default: /* english as default */
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"dialled numbers");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Received calls");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"missed calls");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    rpmt->prmt.other.opt_len[0] = (Uint16Type)strlen((const char *)rpmt->prmt.other.opt[0]);
                    break;
                }
                case AT_SIVR_PRMT_L_READ_LIST:
                {
                    rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                    switch (_VCR_gData.eCurrentLang)
                    {
                        case AT_SIVR_LANG_GERMAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Gewaehlte Rufnummern");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Angenommene Anrufe");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Anrufe in Abwesenheit");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;

                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_FRENCH:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"Numéros composés");
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Num%c%cros compos%c%cs",0xC3, 0xA9,0xC3, 0xA9 );
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Appels reçus");
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Appels re%c%cus", 0xC3, 0xA7);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Appels en absence");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_FRENCH_CAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"appels composés");
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"appels compos%c%cs", 0xC3, 0xA9);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Appels reçus");
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Appels re%c%cus", 0xC3, 0xA7);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Appels en absence");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_SPANISH:
                        case AT_SIVR_LANG_US_SPANISH:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Últimas llamadas");
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"%c%cltimas llamadas", 0xC3, 0x9A);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Llamadas recibidas");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Llamadas perdidas");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_ITALIAN:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"chiamate effettuate");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"Chiamate ricevute");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"chiamate perse");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_DUTCH:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"gekozen nummers");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"Ontvangen oproepen");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"gemiste oproepen");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_PORTUGUES:
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
//                                    (void)snprintf((char *)rpmt->prmt.custom.text),AT_SIVR_MAX_PRMT_OPT_LEN,"números marcados");
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"n%c%cmeros marcados", 0xC3, 0xBA);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"Chamadas aceites");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"chamadas perdidas");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
//                        /* Todo
                        case AT_SIVR_LANG_RUSSIAN:                                                       //   RT#818
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD0,0x9D,
                                                                                                           							0xD0,0xB0,
                                                                                                           							0xD0,0xB1,
                                                                                                           							0xD1,0x80,
                                                                                                           							0xD0,0xB0,
                                                                                                           							0xD0,0xBD,
                                                                                                           							0xD0,0xBD,
                                                                                                           							0xD1,0x8B,
                                                                                                           							0xD0,0xB5,
                                                                                                           							0x20,
                                                                                                           							0xD0,0xBD,
                                                                                                           							0xD0,0xBE,
                                                                                                           							0xD0,0xBC,
                                                                                                           							0xD0,0xB5,
                                                                                                           							0xD1,0x80,
                                                                                                           							0xD0,0xB0);
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD0,0x9F,
                                                                                                           						0xD1,0x80,
                                                                                                           						0xD0,0xB8,
                                                                                                           						0xD0,0xBD,
                                                                                                           						0xD1,0x8F,
                                                                                                           						0xD1,0x82,
                                                                                                           						0xD1,0x8B,
                                                                                                           						0xD0,0xB5,
                                                                                                           						0x20,
                                                                                                           						0xD0,0xB2,
                                                                                                           						0xD1,0x8B,
                                                                                                           						0xD0,0xB7,
                                                                                                           						0xD0,0xBE,
                                                                                                           						0xD0,0xB2,
                                                                                                           						0xD1,0x8B);
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xD0,0xBF,
                                                                                                           									0xD1,0x80,
                                                                                                           									0xD0,0xBE,
                                                                                                           									0xD0,0xBF,
                                                                                                           									0xD1,0x83,
                                                                                                           									0xD1,0x89,
                                                                                                           									0xD0,0xB5,
                                                                                                           									0xD0,0xBD,
                                                                                                           									0xD0,0xBD,
                                                                                                           									0xD1,0x8B,
                                                                                                           									0xD0,0xB5,
                                                                                                           									0x20,
                                                                                                           									0xD0,0xB2,
                                                                                                           									0xD1,0x8B,
                                                                                                           									0xD0,0xB7,
                                                                                                           									0xD0,0xBE,
                                                                                                           									0xD0,0xB2,
                                                                                                           									0xD1,0x8B);
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
//                        */
                        case AT_SIVR_LANG_CZECH:
                        {
                           switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {

//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"volaná císla");

//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"volan%c%c c%c%csla", 0xC3, 0xA1, 0xC3, 0xAD);
// RT#3363
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"volan%c%c %c%c%c%csla", 0xC3, 0xA1, 0xC4, 0x8D, 0xC3, 0xAD);



                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"Prijaté hovory");
//                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"Prijat%c%c hovory", 0xC3, 0xA9);
// RT#3362
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"P%c%cijat%c%c hovory",0xC5, 0x99, 0xC3, 0xA9);

                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
//                                    (void)snprintf((char *)&(rpmt->prmt.other.opt[0][0]),AT_SIVR_MAX_PRMT_OPT_LEN,"zmeškané hovory");  // RT#2468
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PRMT_OPT_LEN,"zme%c%ckan%c%c hovory",0xC5, 0xA1, 0xC3, 0xA9);

                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                        case AT_SIVR_LANG_UK_ENG:
                        case AT_SIVR_LANG_US_ENG:
                        default: /* english as default */
                        {
                            switch (_VCR_gData.resultInfo.cStack.stack_type)
                            {
                                case  APP_PHONEBOOK_enLastDialed:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"dialled numbers");
                                    break;
                                }
                                case  APP_PHONEBOOK_enReceivedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Received calls");
                                    break;
                                }
                                case  APP_PHONEBOOK_enMissedCalls:
                                {
                                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"missed calls");
                                    break;
                                }
                                default:
                                {
                                    sF = FAIL;
                                    PromptErrorLine = __LINE__;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                    break;
                }
                /* SPECIAL HANDLING for OWN string prompts!!! */
                case AT_SIVR_PRMT_STRING:
                {   /* remap prompt type */
                    rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                    (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Not Supported");
                    rpmt->prmt.custom.text_len = strlen((const char *)rpmt->prmt.custom.text);
                    break;
                }
                case AT_SIVR_PRMT_TEST_DIAGNOSE:
                {
                    /* remap prompt type */
                    rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                    /*  Give "diagnose test" text translation back in current Language */
                    _VCR_vDiagnoseTestTranslationBack(&_VCR_gData,rpmt);
                    rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                    break;
                }
                case AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED:
                case AT_SIVR_PRMT_ONLY_MEDIAPLAYER_CONNECTED_L:
                {   /* SOP3 - Ticket - 12013 */
                    switch (_VCR_gData.eCurrentLang)
                    {
                        case AT_SIRV_LANG_MAXNUM:
                        {   // todo: dummy item for possible languages which are not change to SOP3 specification
                            // We use here per default English
                            rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,\
                                   "Only a music player is connected. \
                                    For possible commands, say help.");
                            rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                            break;
                        }
                        default:
                        {
                            // Parrot do all jobs for use
                            break;
                        }
                    }  // end switch case VCR_gData.eCurrentLang
                    break;
                }
                case AT_SIVR_PRMT_ONLY_MP_CONNECTED_BUT_NO_REMOTE_CONTROL:
                case AT_SIVR_PRMT_ONLY_MP_CONNECTED_BUT_NO_REMOTE_CONTROL_L:
                {   /* SOP3 - Ticket - 12013 */
                    switch (_VCR_gData.eCurrentLang)
                    {
                        case AT_SIRV_LANG_MAXNUM:
                        {   // todo: dummy item for possible languages which are not change to SOP3 specification
                            // We use here per default English
                            rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,\
                                  "Only a music player is connected, but remote control of this device is not possible.");
                            rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                            break;
                        }
                        default:
                        {
                            // Parrot do all jobs for use
                            break;
                        }
                    }  // end switch case VCR_gData.eCurrentLang
                    break;
                }
                case AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS:
                case AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS_L:
                {   /* SOP3 - Ticket - 12013 */
                    ERM_GetBTPinCodeAsChar(&pinCodeAsChar);
                    /* For this long promptid a remap is necessary. This is a special case, */
                    /*  because offset between long and short prompt is not 1000 */
                    if (rpmt->prmt.other.prompt_id == AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS_L)
                    {
                        rpmt->prmt.other.prompt_id = AT_SIVR_PRMT_NO_DEVICE_CONNECTED_PLUS_PAIR_INSTRUCTIONS_BIG_L;
                    }
                    switch (_VCR_gData.eCurrentLang)
                    {
                        case AT_SIRV_LANG_MAXNUM:
                        {   // todo: dummy item for possible languages which are not change to SOP3 specification
                            // We use here per default English
                            rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,\
                                  "No phone connected. Please connect a paired phone. \
                                   In case, your phone asks for a PIN, please enter  %c %c %c %c",
                                   pinCodeAsChar.pin[0],pinCodeAsChar.pin[1],pinCodeAsChar.pin[2],pinCodeAsChar.pin[3]);
                            rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                            break;
                        }
                        default:
                        {
                            // set additional parameter to Parrot
                            rpmt->prmt.other.num_opt_avail = 1;
                            /* fill in BT pin but add blanks */
                            rpmt->prmt.other.opt_len[0] = 0;
                            for (i=0U; ( i < pinCodeAsChar.pin_len && i < (Uint32Type)AT_SIVR_MAX_PRMT_OPT_LEN/2);i++)
                            {
                                rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]]   = 0x20;
                                rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]+1] = pinCodeAsChar.pin[i];
                                rpmt->prmt.other.opt_len[0] = rpmt->prmt.other.opt_len[0] + 2;
                            }
                            rpmt->prmt.other.opt[0][rpmt->prmt.other.opt_len[0]] = '\0';
                            break;
                        }
                    }  // end switch case VCR_gData.eCurrentLang
                    break;
                }
                case AT_SIVR_PRMT_NO_DEVICE_CONNECTED_WITH_IC_CONTEXT:
                case AT_SIVR_PRMT_NO_DEVICE_CONNECTED_WITH_IC_CONTEXT_L:
                {   /* SOP3 - Ticket - 12013 */
                    switch (_VCR_gData.eCurrentLang)
                    {
                        case AT_SIRV_LANG_MAXNUM:
                        {   // todo: dummy item for possible languages which are not change to SOP3 specification
                            // We use here per default English
                            rpmt->prmt.custom.prompt_type = AT_SIVR_PROMPT_CUSTOM;
                            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,\
                                   "No phone connected. Please connect a paired phone. \
                                    In order to pair a new phone, follow the instructions in the phone menu of your car.");
                            rpmt->prmt.custom.text_len = (Uint16Type)strlen((const char *)rpmt->prmt.custom.text);
                            break;
                        }
                        default:
                        {
                            // Parrot do all jobs for use
                            break;
                        }
                    }  // end switch case VCR_gData.eCurrentLang
                    break;
                }
                case AT_SIVR_PRMT_START_DELETE_ALL_VT:
                {
                    rpmt->prmt.other.prompt_type = AT_SIVR_PROMPT_SYSTEM;
                    break;
                }
                default:
                {   /* do nothing */
                    break;
                }
            }
            break;
        }
        default:
        {   /* no further infos to be added to prompt */
            break;
        }
    }
    DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"Prompt type:%d->$ATSIVRPromptTypeEnum$",rpmt->prmt.other.prompt_type );
    DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"PromptID: %d->$ATSIVRPromptEnum$",rpmt->prmt.other.prompt_id);
    DEBUG_STATE1(VCR_SWI_TRC, MOD_VCR,"Current Language: %d->$ATSIVRLanguagesEnum$", _VCR_gData.eCurrentLang);

    if (sF == FAIL)
    {
         DEBUG_VALUE2(VCR_SWI_ERR, MOD_VCR,"Error in building PromptID %d in line %d",
                     rpmt->prmt.custom.prompt_id,
                     PromptErrorLine);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_PlayError                                               */
/**
    \brief      function to give out an error prompt and line via VRS

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_PlayError (Uint16Type line)
{
    /* Removed playing of errors via VCR, do only a Trace */
    DEBUG_VALUE1(VCR_SWI_WNG, MOD_VCR,"Error occurred in Line %d", line);

    /* back to idle via stop prompt */
    _VCR_gData.ePromtTable.tab[0]  = AT_SIVR_PRMT_SYSTEM_STOP;
    _VCR_gData.u8NumPromptRequired = 1;
    _VCR_gData.u8NumPromptSend     = 0;
    (void)_VCR_sFMessageToBeSend(VCR_MSG_PLAY_SYSSTOP_PROMPT);
}


void _VCR_vNotifyApplication(VCRNotificationIdEnum notification)
{
    if (notification == VCR_NOTIFY_SIVR_ACTIVATE)
    {
        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"NOTIFY_SIVR_ACTIVATE -> SD");
    }
    else if (notification == VCR_NOTIFY_SIVR_DEACTIVATE)
    {
        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"NOTIFY_SIVR_DEACTIVATE -> SD");
    }

    APP_SD_vOnVcrNotification(notification);
    ERM_vOnVcrNotification(notification);
    /* check if perf measures are active, if so for activation / deactivation
     * status update is needed */
    if (BT_TRUE == _VCR_gData.bPerfMeasOn)
    {   /* send ASR status update */
        _VCR_vSendASRMessage(VCR_PERFSTAT_STATUS);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vStartSupervision                                       */
/**
    \brief      start supervision timer with values related to given type

    \brief

    \param      VCRSupervisionEnum type to indicate short or long wait

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vStartSupervision(VCRSupervisionEnum type)
{
    switch (type)
    {
        case  VCR_SUP_OK:
        {   /* 30 seconds for OK responses */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(
                   CTA_VCR_SUPERVISOR,  SEC(VCR_SUPERVISION_TIMER_OK),   0);
            break;
        }
        case  VCR_SUP_ASYNC:
        {   /* 90 seconds for asyn like language change */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(
                   CTA_VCR_SUPERVISOR,  SEC(VCR_SUPERVISION_TIMER_ASYNC),   0);
            break;
        }
        case  VCR_SUP_RETRIG:
        default:
        {   /* only 10 msec time used for retrigger */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(
                   CTA_VCR_SUPERVISOR,  MSEC(VCR_PENDING_TIMER),   0);
            break;
        }
    }
    _VCR_gData.bVCRSupervisionActive = BT_TRUE;

    DEBUG_VALUE1(VCR_SWI_DBG, MOD_VCR,"Start VCR Supervisortype %d",type);
}

/*----------------------------------------------------------------------------*/
/* Function    : _VCR_vStopSupervision                                        */
/**
    \brief      stop a runing supervision timer

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _VCR_vStopSupervision(void)
{
    if ( BT_TRUE == _VCR_gData.bVCRSupervisionActive )
    {
        DEBUG_TEXT(VCR_SWI_DBG, MOD_VCR,"Stop VCR Supervisor");

        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_VCR_SUPERVISOR);
        _VCR_gData.bVCRSupervisionActive = BT_FALSE;
    }
}



/*----------------------------------------------------------------------------*/
/* Function    : _VCR_bGetMusicMenuAvailable                                  */
/**
    \brief      checks if music menu is coded by diagnosis
                (Feature-ID FT_DIAG_0037)
    \brief

    \param      none

    \return     BT_TRUE if Music Menu is "available"
   */
/*----------------------------------------------------------------------------*/
BooleanType _VCR_bGetMusicMenuAvailable( void )
{
    BooleanType bMusicMenu;     /* menu coding by diagnosis */
    EEPROM_LAYOUT_vGetPlayerEnabled(&bMusicMenu);
    if (SUCCESS == SUCCESS )
    {
        if (BT_TRUE == bMusicMenu)
        {
            // Music is coded
            return(BT_TRUE);
        }
        else
        {
            // Music is NOT coded
            DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Music is not coded");

            return(BT_FALSE);
        }
    }
    else
    {   // Music is coded as default
        return(BT_TRUE);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _VCR_bGetSmsMenuAvailable                                  */
/**
    \brief      checks if SMS of the mobile is supported
                (Feature-ID FT_DIAG_0011)
    \brief

    \param      none

    \return     BT_TRUE if SMS is supported
   */
/*----------------------------------------------------------------------------*/
BooleanType _VCR_bGetSmsMenuAvailable( void )
{
    if(APP_SMS_enGetSmsSupportState() == APP_SMS_INITIALIZED_AND_SUPPORTED)
    {
        // SMS is available
        return(BT_TRUE);
    }
    else
    {
       // SMS not available
       DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"SMS is not supported by mobile");

       return(BT_FALSE);
    }
}


SuccessFailType _VCR_sfSetVoiceFeedback(BooleanType bVoiceShortDialog)
{
    EEDeviceControlBlockType    *pEEApp = (EEDeviceControlBlockType *)getEEAppTaskControl();

    static Uint8Type u8VoiceFeedbackWrite;  // muss static sein wegen EEPROM_Write Funktion

    if (bVoiceShortDialog)
    {
        // short dialog
        u8VoiceFeedbackWrite = 0x01;
    }
    else
    {
        // long dialog
        u8VoiceFeedbackWrite = 0x00;
    }

    (void)EEPROM_sfRamWriteOnly(
                        &u8VoiceFeedbackWrite,
                        EEPROM_LAYOUT_ENUM_VoiceFeedback);

    if (bVoiceShortDialog)
    {
        // short dialog
        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Short Dialog E2P-Write successful");
    }
    else
    {
        // long dialog
        DEBUG_TEXT(VCR_SWI_TRC, MOD_VCR,"Long Dialog E2P-Write successful");
    }
    return(SUCCESS);
}


BooleanType _VCR_bGetVoiceFeedback(void)
{
   BooleanType bVoiceFeedback = BT_FALSE;  // default = long dialog

   EEDeviceControlBlockType *pEEApp = getEEAppTaskControl();

   static Uint8Type u8VoiceFeedbackRead;

   CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8VoiceFeedbackRead, EEPROM_LAYOUT_ENUM_VoiceFeedback);

   if( COMMON_RSP_OK != sF )
   {
       DEBUG_TEXT(VCR_SWI_WNG, MOD_VCR,"VoiceFeedback E2P Read FAIL");
   }
   else
   {
       switch (u8VoiceFeedbackRead)
       {
          case 0x01:
               bVoiceFeedback = BT_TRUE;   // short dialog
          break;

          case 0x00:                 /*Fallthrough*/
          case 0xFF:
          default:
               bVoiceFeedback = BT_FALSE;  // long dialog
          break;
       }
   }
   return(bVoiceFeedback);
}



VCR_USER_PHONEBOOK_enStatusCode _VCR_eUserPhonebookStatusGet(void)
{
  // map to old function "APP_PHONEBOOK_AT_ePhonebookStatusGet"

/*
typedef enum
{
    APP_PHONEBOOK_STATUS_NOT_AVAILABLE,     ->    VCR_USER_PHONEBOOK_STATUS_NOT_AVAILABLE,
    APP_PHONEBOOK_STATUS_DOWNLOAD_IPR,      ->    VCR_USER_PHONEBOOK_STATUS_DOWNLOAD_IPR,
    APP_PHONEBOOK_STATUS_EMPTY,             ->    VCR_USER_PHONEBOOK_STATUS_EMPTY,
    APP_PHONEBOOK_STATUS_AVAILABLE          ->    VCR_USER_PHONEBOOK_STATUS_AVAILABLE
}APP_PHONEBOOK_enStatusCode;
*/

  VCR_USER_PHONEBOOK_enStatusCode eVCR_UserPb = VCR_USER_PHONEBOOK_STATUS_NOT_AVAILABLE;

  Uint16Type pbUserSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
  BooleanType pbUserValid = APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB);
  BooleanType pbUserUpdateInProgress = APP_PHONEBOOK_bPbUpdateInProgress();


  if ((BT_TRUE == pbUserValid) && (pbUserSize > 0))
  {
     eVCR_UserPb = VCR_USER_PHONEBOOK_STATUS_AVAILABLE;
  }
  else if ((BT_TRUE == pbUserValid) && (pbUserSize == 0))
  {
     eVCR_UserPb = VCR_USER_PHONEBOOK_STATUS_EMPTY;
  }
  else if (BT_TRUE == pbUserUpdateInProgress)
  {
     eVCR_UserPb = VCR_USER_PHONEBOOK_STATUS_DOWNLOAD_IPR;
  }
  else if (BT_FALSE == pbUserValid)
  {
     eVCR_UserPb = VCR_USER_PHONEBOOK_STATUS_NOT_AVAILABLE;
  }
  return(eVCR_UserPb);
}

/*----------------------------------------------------------------------------*/
/* Function    : VCR_vDiagnoseTestTranslationBack                            */
/**
    \brief      Give diagnose test text translation back in current Language

    \brief

    \param      _VCR_gData.eCurrentLang, rpmt->prmt.custom.text

    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _VCR_vDiagnoseTestTranslationBack(VcrModGlobalDataType *_VCR_gData,ATReqVCMgtPromtType *rpmt)
{
    switch (_VCR_gData->eCurrentLang)
    {
        case AT_SIVR_LANG_GERMAN:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"Diagnose  Test");
            break;
        }
        case AT_SIVR_LANG_FRENCH:
        case AT_SIVR_LANG_FRENCH_CAN:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"test de diagnostic");
            break;
        }
        case AT_SIVR_LANG_SPANISH:
        case AT_SIVR_LANG_US_SPANISH:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"prueba de diagnostico");
            break;
        }
        case AT_SIVR_LANG_ITALIAN:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"test diagnostico");
            break;
        }
        case AT_SIVR_LANG_DUTCH:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"diagnostische test");
            break;
        }
        case AT_SIVR_LANG_PORTUGUES:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"teste de diagnostico");
            break;
        }
        case AT_SIVR_LANG_RUSSIAN:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"diagnosticheski  test");
            break;
        }
        case AT_SIVR_LANG_CZECH:
        {
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"diagnosticky test");
            break;
        }
        case AT_SIVR_LANG_UK_ENG:
        case AT_SIVR_LANG_US_ENG:
        default:
        {   /* english as default */
            (void)snprintf((char *)rpmt->prmt.custom.text,AT_SIVR_MAX_PROMPT_TEXT_LEN,"diagnostic  test");
            break;
        }
    }  // end switch case VCR_gData.eCurrentLang
    return;
}

/* End Of File VCR_Utils.c */
