/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_Utils.h
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2010-04-14 14:43:41 +0200 (Mi, 14 Apr 2010) $
* $Rev: 12193 $
* $Author: braun $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/inc/VCR_Utils.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _VCR_UTILS_H
#define _VCR_UTILS_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ATCmdHandlerData.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "VCR_StateHandler.h"
#include "VCR_ASR.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* VCR Private Types                                                              */
/*----------------------------------------------------------------------------*/
typedef enum
{
    VCR_USER_PHONEBOOK_STATUS_NOT_AVAILABLE,
    VCR_USER_PHONEBOOK_STATUS_DOWNLOAD_IPR,
    VCR_USER_PHONEBOOK_STATUS_EMPTY,
    VCR_USER_PHONEBOOK_STATUS_AVAILABLE
}VCR_USER_PHONEBOOK_enStatusCode;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern void _VCR_vInitStateData(void);

extern void _VCR_vSetCK5050Available(
        BooleanType isAvailb);

extern void _VCR_PlayError (
        Uint16Type line);

extern BooleanType _VCR_bNumberOrCallStackAvail(void);

extern BooleanType _VCR_bFCheckMDINeedToBePaused(void);

extern ATSIVRLanguagesEnum _VCR_eGetDefaultReqLanguage(void);

extern BooleanType          _VCR_bCheckLangChangeReq(void);

extern SuccessFailType  _VCR_vSendTriggerToMainEventHandler(
        VCRMenuEventEnum trigger);

extern void  _VCR_vSendCmdToMainEventHandler(
        ATSIVRCmdIdEnum          cmd);

extern SuccessFailType   _VCR_sFProcResultsUnique(
        BooleanType * is_unique);

extern SuccessFailType _VCR_sFCheckResultAvailable(void);
extern void            _VCR_vChangeState(ATSIVRPromptEnum newState);
extern void            _VCR_vChangeMenuToIdle(void);

extern void            _VCR_vChangeMenuToMain(void);
extern void            _VCR_vCancelStopFor3rdError(void);
extern void            _VCR_vCancelGotoMain(void);
extern void            _VCR_vFunctionNotAvailableStop(void);
extern void            _VCR_vFunctionNotAvailableMain(void);
extern void            _VCR_vNumberNotAvailableMain(void);
extern void            _VCR_vSmsDeletedMain(void);

extern void            _VCR_vPhoneNotAvailableStop(void);
extern void            _VCR_vSinglePrompt(ATSIVRPromptEnum prompt);
extern void            _VCR_vSinglePrepare(ATSIVRPromptEnum prompt);
extern void            _VCR_vT1SorryRepeat(void);

extern SuccessFailType _VCR_sfInitMenuValues(void);

extern SuccessFailType _VCR_sfInitMsgResultValues(void);

extern SuccessFailType _VCR_sfInitResultValues(void);

extern SuccessFailType  _VCR_sFBuildPrompt(
        ATReqVCMgtPromtType * rpmt);

extern void _VCR_vConnectPrompt(void);

extern void _VCR_vConnectPromptCategory(void);

extern SuccessFailType _VCR_eSetDefaultReqLanguage(ATSIVRLanguagesEnum value);

extern void _VCR_vSystemStopToIdle(BooleanType bOusted);

extern void _VCR_vNotifyApplication(VCRNotificationIdEnum notification);

extern void _VCR_vStartSupervision(VCRSupervisionEnum type);
extern void _VCR_vStopSupervision(void);
extern BooleanType _VCR_bGetMusicMenuAvailable(void);
extern BooleanType _VCR_bGetSmsMenuAvailable(void);
extern BooleanType _VCR_bGetVoiceFeedback(void);
extern SuccessFailType _VCR_sfSetVoiceFeedback(BooleanType bVoiceShortDialog);
extern VCR_USER_PHONEBOOK_enStatusCode _VCR_eUserPhonebookStatusGet(void);



/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/


#endif /* _VCR_UTILS_H */

/* End Of File VCR_Utils.h */
