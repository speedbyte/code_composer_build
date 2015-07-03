/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_MessageHandler.h
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2010-03-09 14:29:55 +0100 (Di, 09 Mrz 2010) $
* $Rev: 11461 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/inc/VCR_MessageHandler.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _VCR_MESSAGE_HANDLER_H
#define _VCR_MESSAGE_HANDLER_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ATParser.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "VCR_StateHandler.h"


/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern void                 _VCR_vInitMsgCtrlData(void);

extern void                 _VCR_vInitLanguageData(void);


extern SuccessFailType      VCR_sFHandleATMessage(const ATRspMessageType * p_msg);

extern void                 _VCR_vSetRequiredLanguage(ATSIVRLanguagesEnum newLangId);

extern void                 _VCR_vHandleErmOkErrorEvent(BooleanType isOK);

extern void                 _VCR_vCheckAndSendATMessage(EventMaskType events);

extern SuccessFailType  _VCR_sFHandleResult(
        BooleanType      is_event,
        VCRMenuEventEnum event,
        ATSIVRCmdIdEnum  cmd);

extern void _VCR_vSendASRMessage(
        VCRPerfMeasStatusTypeEnum type);

extern void _VCR_vPrepareASRCmdMessage(void);
extern SuccessFailType _VCR_vASRGetPbInfo(void);
extern void VCR_vCBGetPBInfoASR(APP_COMMON_enResponseCode response);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/


#endif /* _VCR_MESSAGE_HANDLER_H */

/* End Of File VCR_MessageHandler.h */
