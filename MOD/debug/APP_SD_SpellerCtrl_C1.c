/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SD_SpellerCtrl_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_SD 
*-----------------------------------------------------------------------------
* Description:    
*-----------------------------------------------------------------------------
* $Date: 2011-06-20 10:53:23 +0200 (Mo, 20 Jun 2011) $
* $Rev: 20286 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/src/APP_SD_SpellerCtrl_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "TRACE.h"
#include "APP_SD_SpellerCtrl_C1.id"

#include "APP_SD_FrameBuilder.h"
#include "APP_SD_SpellerCtrl.h"
#include "APP_SD_FrameDataBase.h"
#include "APP_SD_CI.h"
#include "APP_SD.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern AppSdContext gAppSdContext;
/** 
\brief	variable to save the current call management function currently in progress
*/
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
static void _APP_SD_SC_vButtonUpDown_DigitDial(AppSdScSpellerCtrlType *pCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown);
static void _APP_SD_SC_vButtonUpDown_VoiceMail(AppSdScSpellerCtrlType *pCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown);
static void _APP_SD_SC_vButtonOk_DigitDial(AppSdScSpellerCtrlType *pCtrl);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

const Uint8Type charSet_digitDial[APP_SD_SC_CHAR_SET_SIZE_DIGIT_DIAL] = 
            {   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '*', '#' };
const Uint8Type charSet_voiceMail[APP_SD_SC_CHAR_SET_SIZE_VOICE_MAIL] = 
            {   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+' };
const Uint8Type charSet_dtmf[APP_SD_SC_CHAR_SET_SIZE_DTMF] = 
            {   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '#' };
const Uint8Type charSet_passkey[APP_SD_SC_CHAR_SET_SIZE_PASSKEY] = 
            {   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const Uint8Type charSet_alphaNumeric[APP_SD_SC_CHAR_SET_SIZE_ALPHA_NUMERIC] = 
            {   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
                'U', 'V', 'W', 'X', 'Y', 'Z',
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                ' ' /* Add Space CR15 */ 
            };
const APP_SD_SC_enSpellerControlId controlSet_digitDial[APP_SD_SC_CONTROL_SET_SIZE_DIGIT_DIAL] = 
            { APP_SD_SC_CONTROL_BACKSPACE, APP_SD_SC_CONTROL_DIAL, APP_SD_SC_CONTROL_ABORT };
const APP_SD_SC_enSpellerControlId controlSet_voiceMail[APP_SD_SC_CONTROL_SET_SIZE_VOICE_MAIL] = 
            { APP_SD_SC_CONTROL_BACKSPACE, APP_SD_SC_CONTROL_DIAL, APP_SD_SC_CONTROL_SAVE, APP_SD_SC_CONTROL_ABORT };
const APP_SD_SC_enSpellerControlId controlSet_dtmf[APP_SD_SC_CONTROL_SET_SIZE_DTMF] = 
            { APP_SD_SC_CONTROL_BACK };
const APP_SD_SC_enSpellerControlId controlSet_passkey[APP_SD_SC_CONTROL_SET_SIZE_PASSKEY] = 
            { APP_SD_SC_CONTROL_BACKSPACE, APP_SD_SC_CONTROL_OK, APP_SD_SC_CONTROL_ABORT };
const APP_SD_SC_enSpellerControlId controlSet_alphaNumeric[APP_SD_SC_CONTROL_SET_SIZE_ALPHA_NUMERIC] = 
            { APP_SD_SC_CONTROL_BACKSPACE, APP_SD_SC_CONTROL_OK, APP_SD_SC_CONTROL_ABORT };

const AppSdSpellerConfigType gSpellConfig[APP_SD_SC_LAST_INDEX]
    = { {   /* APP_SD_SC_DIGIT_DIALING */
            APP_SD_SC_CHAR_SET_SIZE_DIGIT_DIAL,
            APP_SD_SC_CONTROL_SET_SIZE_DIGIT_DIAL,
            // SUSANNE: 15.10.2009 Ck5050 allows 32 characters only, so limiting here too
            AT_MAX_PHONE_NUM_LEN, // 40,
            charSet_digitDial,
            controlSet_digitDial
        },
        {   /* APP_SD_SC_VOICE_MAIL */
            APP_SD_SC_CHAR_SET_SIZE_VOICE_MAIL,
            APP_SD_SC_CONTROL_SET_SIZE_VOICE_MAIL,
            21,
            charSet_voiceMail,
            controlSet_voiceMail
        },
        {   /* APP_SD_SC_DTMF */
            APP_SD_SC_CHAR_SET_SIZE_DTMF,
            APP_SD_SC_CONTROL_SET_SIZE_DTMF,
            40,
            charSet_dtmf,
            controlSet_dtmf
        },
        {   /* APP_SD_SC_BLUETOOTH_PASSKEY */
            APP_SD_SC_CHAR_SET_SIZE_PASSKEY,
            APP_SD_SC_CONTROL_SET_SIZE_PASSKEY,
            AT_MAX_PIN_CODE_LEN,
            charSet_passkey,
            controlSet_passkey
        },
        {   /* APP_SD_SC_ALPHA_NUMERIC */
            APP_SD_SC_CHAR_SET_SIZE_ALPHA_NUMERIC,
            APP_SD_SC_CONTROL_SET_SIZE_ALPHA_NUMERIC,
            APP_SD_SC_SPELL_BUFFER_SIZE,
            charSet_alphaNumeric,
            controlSet_alphaNumeric
        }
    
      };

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/* ------------ Module Internal Interfaces ----------------------*/

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vInitSpeller                                        */
/** 
  \brief        Initializes the speller control elements with the specified 
                speller type  

  \param        pCtrl
                Pointer to the current speller control structure

  \param        spellerId
                Specifies the speller type to use

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vInitSpeller(AppSdScSpellerCtrlType *pCtrl, APP_SD_SC_enSpellerId spellerId)
{
    const AppSdSpellerConfigType *pConfig = NULL;

    if(spellerId >= APP_SD_SC_LAST_INDEX)
    {
        DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "InitSpeller: id %d ooR", spellerId );

        spellerId = APP_SD_SC_DIGIT_DIALING;
    }

    pConfig = &gSpellConfig[spellerId];

    (void)memset(pCtrl, 0, sizeof(*pCtrl));
    
    pCtrl->common.spellerId  = spellerId;
    pCtrl->common.spellLimit = pConfig->spellLimit;

}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vSetSpellLimit                                      */
/** 
  \brief        Changes the number of maximum chars   

  \param        pCtrl
                Pointer to the current speller control structure

  \param        spellLimit
                The maximum number of spelled chars 

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vSetSpellLimit(AppSdScSpellerCtrlType *pCtrl, Uint16Type spellLimit)
{
    spellLimit = (spellLimit > APP_SD_SC_SPELL_BUFFER_SIZE) ? 
                                    (Uint16Type)APP_SD_SC_SPELL_BUFFER_SIZE : spellLimit;
    
    pCtrl->common.spellLimit = spellLimit;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vSetDefaultCtrlSelection                            */
/** 
  \brief        Sets the current selection to the default control element 
                (DIAL, OK, ...) 

  \param        pCtrl
                Pointer to the current speller control structure

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vSetDefaultCtrlSelection(AppSdScSpellerCtrlType *pCtrl)
{
    const AppSdSpellerConfigType *pConfig = &gSpellConfig[pCtrl->common.spellerId];

    /* +1 to set the selection to dial, ok, ... */
    pCtrl->common.selectedIdx = pConfig->charSetSize + 1;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vSetSpelledText                                     */
/** 
  \brief        Presets the speller buffer with a provided string  

  \param        pCtrl
                Pointer to the current speller control structure

  \param        text
                Pointer to byte array

  \param        length
                Length of the provided string

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vSetSpelledText(AppSdScSpellerCtrlType *pCtrl, const Uint8Type *text, Uint16Type length)
{
    const AppSdSpellerConfigType *pConfig =  &gSpellConfig[pCtrl->common.spellerId];

    length = (length > pCtrl->common.spellLimit) ?  pCtrl->common.spellLimit : length;

    pCtrl->common.spellBuffer.pos = length;
    (void)memcpy(pCtrl->common.spellBuffer.data, text, pCtrl->common.spellBuffer.pos);

    if(pCtrl->common.spellBuffer.pos >= pCtrl->common.spellLimit)
    {
        /* no more chars are accepted, set the selection to the control elements */
        pCtrl->common.selectedIdx = pConfig->charSetSize + 1;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vButtonUpDown                                       */
/** 
  \brief        Scrolls within the speller selection list  

  \param        pCtrl
                Pointer to the current speller control structure

  \param        upOrDown
                Specifies the scroll direction (up or down)

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vButtonUpDown(AppSdScSpellerCtrlType *pCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown)
{
    const AppSdSpellerConfigType *pConfig =  &gSpellConfig[pCtrl->common.spellerId];
    Uint16Type charSetSize      = pConfig->charSetSize;
    Uint16Type controlSetSize   = pConfig->controlSetSize;
    Uint16Type totalSize        = charSetSize + controlSetSize;

    if(upOrDown == enLogiButManMflDown)
    {
        if( pCtrl->common.spellBuffer.pos >= pCtrl->common.spellLimit ) 
        {
            if(pCtrl->common.selectedIdx > pConfig->charSetSize)
            {
                pCtrl->common.selectedIdx--;
            }
        }
        else 
        {   
            if(pCtrl->common.selectedIdx > 0)
            {
                pCtrl->common.selectedIdx--;
            }
            else				//RT #1922
            {
            	pCtrl->common.selectedIdx = (totalSize - 1);
            }
        }
    }
    else
    {
        if( pCtrl->common.spellBuffer.pos >= pCtrl->common.spellLimit ) 
        {
            if(pCtrl->common.selectedIdx < (totalSize - 1))
            {
                pCtrl->common.selectedIdx++;
            }
        }
        else 
        {   
            if(pCtrl->common.selectedIdx < (totalSize - 1))
            {
                pCtrl->common.selectedIdx++;
            }
            else				//RT #1922
            {
            	pCtrl->common.selectedIdx = 0;
            }
        }
    }

    /* do a post handling for some speller */
    switch(pCtrl->common.spellerId)
    {
        case APP_SD_SC_DIGIT_DIALING:
            _APP_SD_SC_vButtonUpDown_DigitDial(pCtrl, upOrDown);
            break;
        case APP_SD_SC_VOICE_MAIL:
            _APP_SD_SC_vButtonUpDown_VoiceMail(pCtrl, upOrDown);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vButtonOk                                   */
/** 
  \brief        Handles a pressed OK button  
  \brief        Description:\n
                Sets a char to the spelled buffer or execute a control element. If
                the OK button was pressed with selected control element, the control
                element is returned.

  \param        pCtrl
                Pointer to the current speller control structure

  \return       APP_SD_SC_enSpellerControlId
                The selected control element or APP_SD_SC_CONTROL_INVALID if no
                control element was selected

 */
/*----------------------------------------------------------------------------*/
APP_SD_SC_enSpellerControlId APP_SD_SC_vButtonOk(AppSdScSpellerCtrlType *pCtrl)
{
    const AppSdSpellerConfigType *pConfig = &gSpellConfig[pCtrl->common.spellerId];
    APP_SD_SC_enSpellerControlId controlCommand = APP_SD_SC_CONTROL_INVALID;
    

    if(pCtrl->common.selectedIdx < pConfig->charSetSize)
    {
        Uint8Type selectedChar = pConfig->charSet[pCtrl->common.selectedIdx];
        pCtrl->common.spellBuffer.data[pCtrl->common.spellBuffer.pos] = selectedChar;
        pCtrl->common.spellBuffer.pos++;

        if(pCtrl->common.spellBuffer.pos >= pCtrl->common.spellLimit)
        {
            /* +1 to set the selection to dial, ok, ...  */
            pCtrl->common.selectedIdx = pConfig->charSetSize + 1;
        }
    }
    else
    {
        Uint16Type ctrlIdx = pCtrl->common.selectedIdx - pConfig->charSetSize;
        controlCommand = pConfig->controlSet[ctrlIdx];

        if( controlCommand == APP_SD_SC_CONTROL_BACKSPACE)
        {
            BooleanType loop = BT_TRUE;
            while(pCtrl->common.spellBuffer.pos > 0 && loop)
            {
                UTF8_ByteType utf8Type = APP_COMMON_enGetUtf8Type(pCtrl->common.spellBuffer.data[pCtrl->common.spellBuffer.pos-1]);

                switch(utf8Type)
                {
                    case UTF8_FOLLOW_BYTE:
                        /* delete until we reach a start byte */
                        pCtrl->common.spellBuffer.pos--;
                        break;
                    case UTF8_START_BYTE_SIZE_1:
                    case UTF8_START_BYTE_SIZE_2:
                    case UTF8_START_BYTE_SIZE_3:
                    case UTF8_START_BYTE_SIZE_4:    
                        /* we are done */
                        pCtrl->common.spellBuffer.pos--;
                        loop = BT_FALSE;
                        break; 
                    default:
                        DEBUG_VALUE2( APP_SD_SWI_ERR, MOD_APP_SD, "APP_SD_SC_vButtonOk: invalid utf8Type %d byte 0x%02x",
                                      utf8Type,
                                      pCtrl->common.spellBuffer.data[pCtrl->common.spellBuffer.pos-1] );
                        loop = BT_FALSE;
                        break; 
                }
            }
        }
    }

    /* do a post handling for some speller */
    switch(pCtrl->common.spellerId)
    {
        case APP_SD_SC_DIGIT_DIALING:
        case APP_SD_SC_VOICE_MAIL:
            _APP_SD_SC_vButtonOk_DigitDial(pCtrl);
            break;
    }
    
    return controlCommand;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vReadSelectionText                                  */
/** 
  \brief        Returns a string of the selected speller element  

  \param        pCtrl
                Pointer to the current speller control structure

  \param        pSelectedText
                The selected item as string 

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vReadSelectionText(const AppSdScSpellerCtrlType *pCtrl, AppSdSpellerSelectType *pSelectedText)
{
    const AppSdSpellerConfigType *pConfig =  &gSpellConfig[pCtrl->common.spellerId];
    Sint32Type result = 0;
    Sint32Type size = sizeof(pSelectedText->data);

    if(pCtrl->common.selectedIdx < pConfig->charSetSize)
    {
        if(KPB_enGetKpbVersionNumber() == KPB_VERSION_1_0)
        {
            result = snprintf(  (char*)pSelectedText->data, size, "[ %c ]",
                                pConfig->charSet[pCtrl->common.selectedIdx]);
        }
        else
        {
            result = snprintf(  (char*)pSelectedText->data, size, "%c",
                                        pConfig->charSet[pCtrl->common.selectedIdx]);
        }

        if( (result > -1) && (result < size))
        {
            pSelectedText->length = result; 
        }
        else
        {
            /* error occurs during snprintf */
            pSelectedText->length = 0; 
        }
    }
    else
    {
        APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY txtId = APP_SD_enError;
        Uint16Type ctrlIdx = pCtrl->common.selectedIdx - pConfig->charSetSize;
        const char * pBracketLeft = "[ ";
        const char * pBracketRight = " ]";
        Uint16Type labelSizeLeft = strlen(pBracketLeft);
        Uint16Type labelSizeRight = strlen(pBracketRight);
        Uint16Type position = 0;

        switch(pConfig->controlSet[ctrlIdx])
        {
            case APP_SD_SC_CONTROL_BACKSPACE:
                txtId = APP_SD_en_S305_ta_backspace_Auswahl_1;
                break;
            case APP_SD_SC_CONTROL_OK:
                txtId = APP_SD_en_S535_ta_menu_ok_Auswahl_1;
                break;
            case APP_SD_SC_CONTROL_ABORT:
                txtId = APP_SD_en_S535_ta_menu_cancel_Auswahl_1;
                break;
            case APP_SD_SC_CONTROL_BACK:
                txtId = APP_SD_en_S804_ta_list_back_Auswahl_1;
                break;
            case APP_SD_SC_CONTROL_DIAL:
                txtId = APP_SD_en_S305_ta_menu_call_Auswahl_1;
                break;
            case APP_SD_SC_CONTROL_SAVE:
                txtId = APP_SD_en_S306_ta_menu_save_Auswahl_1;
                break;
            default:
                txtId = APP_SD_enError;
                break;
        }

        if(KPB_enGetKpbVersionNumber() == KPB_VERSION_1_0)
        {
            if (labelSizeLeft +labelSizeRight + gAppSdContext.prompts_len[txtId] <= size)
            {            
                // set Bracket Left
                (void)memcpy(pSelectedText->data, pBracketLeft, labelSizeLeft);
                position += labelSizeLeft;

                // set Text
                (void)memcpy(&pSelectedText->data[position], gAppSdContext.prompts[txtId], gAppSdContext.prompts_len[txtId]);
                position += gAppSdContext.prompts_len[txtId];

                // set Bracket Right
                (void)memcpy(&pSelectedText->data[position], pBracketRight, labelSizeRight);
                position += labelSizeRight;
                pSelectedText->length = position;
            }
            else
            {
                DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "APP_SD_SC_vReadSelectionText: data size to high: %d",
                  (labelSizeLeft +labelSizeRight + gAppSdContext.prompts_len[txtId]));
            }
       }
       else
       {
           (void)memcpy( &pSelectedText->data,
                   gAppSdContext.prompts[txtId],
                   gAppSdContext.prompts_len[txtId]);
           pSelectedText->length = gAppSdContext.prompts_len[txtId];
       }
    }

}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_vReadSpelledText                                    */
/** 
  \brief        Reads out the spelled buffer

  \param        pCtrl
                Pointer to the current speller control structure

  \param        pSpelledText
                The spelled text 

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_SD_SC_vReadSpelledText(const AppSdScSpellerCtrlType *pCtrl, AppSdSpelledTextType *pSpelledText)
{
    pSpelledText->length = pCtrl->common.spellBuffer.pos; 

    (void)memcpy(pSpelledText->data, pCtrl->common.spellBuffer.data, pSpelledText->length);
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_SC_bGetLastSpelledChar                                 */
/** 
  \brief        Returns the last char in the spelled buffer  

  \param        pCtrl
                Pointer to the current speller control structure

  \param        lastChar
                Pointer to the last char of spelled buffer

  \return       BooleanType
                BT_TRUE or BT_FALSE

 */
/*----------------------------------------------------------------------------*/
BooleanType APP_SD_SC_bGetLastSpelledChar(const AppSdScSpellerCtrlType *pCtrl, Uint8Type *lastChar)
{
    if(pCtrl->common.spellBuffer.pos > 0)
    {
        *lastChar = pCtrl->common.spellBuffer.data[pCtrl->common.spellBuffer.pos-1];

        return BT_TRUE;
    }

    return BT_FALSE;
}




/* ---------------- Internal Member -------------------------*/

void _APP_SD_SC_vButtonUpDown_DigitDial(AppSdScSpellerCtrlType *pCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown)
{
    const AppSdSpellerConfigType *pConfig =  &gSpellConfig[pCtrl->common.spellerId];
    
    if( pCtrl->common.spellBuffer.pos > 0 )
    {
        if(pCtrl->common.selectedIdx < pConfig->charSetSize)
        {
            /* we have spelled text and are in the range of the 
             * char elements, so prevent from displaying '+' */
            Uint8Type curChar = pConfig->charSet[pCtrl->common.selectedIdx];
            if( curChar == '+' )
            {
                if(upOrDown == enLogiButManMflDown)
                {
                    pCtrl->common.selectedIdx--;    
                }
                else
                {
                    pCtrl->common.selectedIdx++;
                }
            }
        }
    }
    else
    {
        if(pCtrl->common.selectedIdx > pConfig->charSetSize)
        {
            Uint16Type idx = pCtrl->common.selectedIdx - pConfig->charSetSize;

            if( (idx < APP_SD_SC_CONTROL_SET_SIZE_DIGIT_DIAL) &&
                (pConfig->controlSet[idx] == APP_SD_SC_CONTROL_DIAL) )
            {
                if(upOrDown == enLogiButManMflDown)
                {
                    pCtrl->common.selectedIdx--;    
                }
                else
                {
                    pCtrl->common.selectedIdx++;
                }
            }
        }
    }
}

void _APP_SD_SC_vButtonUpDown_VoiceMail(AppSdScSpellerCtrlType *pCtrl, LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown)
{
    const AppSdSpellerConfigType *pConfig =  &gSpellConfig[pCtrl->common.spellerId];
    
    if( pCtrl->common.spellBuffer.pos > 0 )
    {
        if(pCtrl->common.selectedIdx < pConfig->charSetSize)
        {
            /* we have spelled text and are in the range of the 
             * char elements, so prevent from displaying '+' */
            Uint8Type curChar = pConfig->charSet[pCtrl->common.selectedIdx];
            if( curChar == '+' )
            {
                if(upOrDown == enLogiButManMflDown)
                {
                    pCtrl->common.selectedIdx--;    
                }
                else
                {
                    pCtrl->common.selectedIdx++;
                }
            }
        }
    }
    else
    {
        if(pCtrl->common.selectedIdx >= pConfig->charSetSize)
        {
            /* we have no spelled text and are in the range of the 
             * control elements, so prevent from displaying 'Anrufen'
             * and 'Speichern' */
            Uint16Type ctrlIdx = pCtrl->common.selectedIdx - pConfig->charSetSize;
            switch(pConfig->controlSet[ctrlIdx])
            {
                case APP_SD_SC_CONTROL_DIAL:
                    if(upOrDown == enLogiButManMflDown)
                    {
                        pCtrl->common.selectedIdx--;    
                    }
                    else
                    {
                        pCtrl->common.selectedIdx++;
                        pCtrl->common.selectedIdx++;
                    }
                    break;
                case APP_SD_SC_CONTROL_SAVE:
                    if(upOrDown == enLogiButManMflDown)
                    {
                        pCtrl->common.selectedIdx--;    
                        pCtrl->common.selectedIdx--;    
                    }
                    else
                    {
                        pCtrl->common.selectedIdx++;
                    }
                    break;
                default:
                    /* do nothing */
                    break;
            }
        }
    }
}

void _APP_SD_SC_vButtonOk_DigitDial(AppSdScSpellerCtrlType *pCtrl)
{
    const AppSdSpellerConfigType *pConfig =  &gSpellConfig[pCtrl->common.spellerId];
    Uint8Type selectedChar = pConfig->charSet[pCtrl->common.selectedIdx];

    if(selectedChar == '+')
    {
        /* after specialChar '+' set the current idx to 1 */
        pCtrl->common.selectedIdx = 1;
    }
}

/* End Of File APP_SD_SpellerCtrl_C1.c */


