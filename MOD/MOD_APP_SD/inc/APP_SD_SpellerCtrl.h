/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_SD_FrameBuilder.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SD    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_SD
*-----------------------------------------------------------------------------
* $Date: 2011-06-20 10:53:23 +0200 (Mo, 20 Jun 2011) $
* $Rev: 20286 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/inc/APP_SD_SpellerCtrl.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SD_SPELLER_CTRL_H
#define _APP_SD_SPELLER_CTRL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#define APP_SD_SC_CHAR_SET_SIZE_DIGIT_DIAL          13
#define APP_SD_SC_CHAR_SET_SIZE_VOICE_MAIL          11
#define APP_SD_SC_CHAR_SET_SIZE_DTMF                12
#define APP_SD_SC_CHAR_SET_SIZE_PASSKEY             10
#define APP_SD_SC_CHAR_SET_SIZE_ALPHA_NUMERIC       37

#define APP_SD_SC_CONTROL_SET_SIZE_DIGIT_DIAL       3
#define APP_SD_SC_CONTROL_SET_SIZE_VOICE_MAIL       4
#define APP_SD_SC_CONTROL_SET_SIZE_DTMF             1
#define APP_SD_SC_CONTROL_SET_SIZE_PASSKEY          3
#define APP_SD_SC_CONTROL_SET_SIZE_ALPHA_NUMERIC    3

typedef enum
{
    APP_SD_SC_DIGIT_DIALING = 0,
    APP_SD_SC_VOICE_MAIL,
    APP_SD_SC_DTMF,
    APP_SD_SC_BLUETOOTH_PASSKEY,
    APP_SD_SC_ALPHA_NUMERIC,
    APP_SD_SC_LAST_INDEX
} APP_SD_SC_enSpellerId;

typedef enum
{
    APP_SD_SC_CONTROL_INVALID = 0,
    APP_SD_SC_CONTROL_BACKSPACE,
    APP_SD_SC_CONTROL_OK,
    APP_SD_SC_CONTROL_ABORT,
    APP_SD_SC_CONTROL_BACK,
    APP_SD_SC_CONTROL_DIAL,
    APP_SD_SC_CONTROL_SAVE
}APP_SD_SC_enSpellerControlId;


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef struct
{
    Uint8Type   data[APP_SD_SC_SPELL_BUFFER_SIZE];
    Uint16Type  pos;
}AppSdScSpellBufferType;

typedef struct
{
    APP_SD_SC_enSpellerId   spellerId;
    Uint16Type              spellLimit;
    Uint16Type              selectedIdx;
    AppSdScSpellBufferType  spellBuffer;
}AppSdScSpellerCommon;

typedef union
{
    AppSdScSpellerCommon            common;
}AppSdScSpellerCtrlType;


typedef struct
{
    Uint16Type charSetSize;
    Uint16Type controlSetSize;
    Uint16Type spellLimit;
    const Uint8Type *charSet;
    const APP_SD_SC_enSpellerControlId *controlSet;
}AppSdSpellerConfigType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vInitSpeller(AppSdScSpellerCtrlType *pCtrl, APP_SD_SC_enSpellerId spellerId);
void APP_SD_SC_vButtonUpDown(AppSdScSpellerCtrlType *pCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown);
APP_SD_SC_enSpellerControlId APP_SD_SC_vButtonOk(AppSdScSpellerCtrlType *pCtrl);

void        APP_SD_SC_vReadSelectionText(const AppSdScSpellerCtrlType *pCtrl, AppSdSpellerSelectType *pSelectedText);
void        APP_SD_SC_vReadSpelledText(const AppSdScSpellerCtrlType *pCtrl, AppSdSpelledTextType *pSpelledText);
BooleanType APP_SD_SC_bGetLastSpelledChar(const AppSdScSpellerCtrlType *pCtrl, Uint8Type *lastChar);
void        APP_SD_SC_vSetSpellLimit(AppSdScSpellerCtrlType *pCtrl, Uint16Type spellLimit);
void        APP_SD_SC_vSetSpelledText(AppSdScSpellerCtrlType *pCtrl, const Uint8Type *text, Uint16Type length);
void        APP_SD_SC_vSetDefaultCtrlSelection(AppSdScSpellerCtrlType *pCtrl);
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface IAPP_SD_ERM_INTERNAL
*/
//#ifdef IAPP_SD_ERM_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_ERM_INTERNAL) */



//#endif /* END DECLARE INTERFACE(IAPP_SD_LIST_CTRL_INTERNAL) */


#endif /* _APP_SD_SPELLER_CTRL_H */

/* End Of File APP_SD_LIST_CTRL.h */

