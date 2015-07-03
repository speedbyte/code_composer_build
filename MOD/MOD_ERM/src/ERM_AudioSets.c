/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_AudioSets.c
*-----------------------------------------------------------------------------
* Module Name:    External resource manager
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-09-27 13:27:48 +0200 (Di, 27 Sep 2011) $
* $Rev: 21940 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/src/ERM_AudioSets.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "common.h"
#include "UART.h"
#include "ATParser.h"
#include "ATCmdHandler.h"
#include "POOL.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONEBOOK_ERM.h"
#include "APP_MDI_ERM.h"
#include "APP_MDI_BAP.h"
#include "APP_COMMON_Utils.h"
#include "VCR.h"
#include "ATCommonData.h"
#include "APP_COMMON_Utils.h"
#include "EEPROM_LAYOUT_CFG.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_AudioSets.h"
#include "ERM_AudioMappingTab.h"

#include "ERM_Utils.h"
#include "ERM_MessageHandler.h"
#include "ERM_DeviceHandler.h"
#include "ERM_AudioSets.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern ERMContextType        _ERM_gContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/
static void _ERM_vIntAudioCallback(APP_COMMON_enResponseCode rsp);
static void _ERM_vReInitAudioCallback(APP_COMMON_enResponseCode rsp);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/* the default ROM table is included in this header file */
#include "ERM_AudioTable.h"

static ERMAudioModValuesType _ERM_stAudioMod        = {0u, 0u, 0u, 0u, 0u, 0u, 0u};
static Uint8Type             _ERM_u8AudioSetIdx     = 0u;
static ERMIntMessageEnum     _ERM_eAudioMessageIPR  = ERM_REQ_NONE;
static void (*_ERM_atExtAudioCB)(APP_COMMON_enResponseCode) = NULL;
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
/* Function    : _ERM_vInitAudioValues                                        */
/**
    \brief      initialise Audio Values

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vInitAudioValues(void)
{
    _ERM_atExtAudioCB      = NULL;
    _ERM_eAudioMessageIPR  = ERM_REQ_NONE;
    /* try to fetch index from Diagnose, if not possible set to 0 */
    /* TODO add diagnose function to fetch index */
    EEPROM_LAYOUT_vGetAudioParameterSet(&_ERM_u8AudioSetIdx);

    if (_ERM_u8AudioSetIdx >= (Uint8Type)ERM_NUM_AUDIO_PARAM_SET)
    {
        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM inval audio init from SD %d >= %d set to 0",_ERM_u8AudioSetIdx,ERM_NUM_AUDIO_PARAM_SET);

        _ERM_u8AudioSetIdx     = 0u;
    }

    /* update values */
    // RT#5049 Volume Settings of Speech Dialog System
    // -> gleiches Problem besteht fuer alle Werte, die auch angepasst werden koennen

     // Test of MicGain -> same problem as in promptVolume see RT#5049
    _ERM_stAudioMod.mic_gain     = _ERM_InitMicGain();
    _ERM_stAudioMod.commVolume   = _ERM_InitCommVolume();
    _ERM_stAudioMod.promptVolume = _ERM_InitPromptVolume();
    _ERM_stAudioMod.streamVolume = _ERM_InitStreamVolume();
    _ERM_stAudioMod.aecDelay     = _ERM_InitAecDelay();
    _ERM_stAudioMod.alcMode     = _ERM_InitAlcMode();
    _ERM_stAudioMod.alcTarget   = _ERM_InitRXAlcTarget();
    _ERM_stAudioMod.slLevel     = _ERM_InitRXSLLevel();

    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ERM audio init done");
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_vReInitAudioParameterSet                                 */
/**
    \brief      Reinitialise AudioParameterSet

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/

void ERM_vReInitAudioParameterSet(Uint8Type u8NewAudioSetIdx)
{
    if (u8NewAudioSetIdx >= (Uint8Type)ERM_NUM_AUDIO_PARAM_SET)
    {
        u8NewAudioSetIdx     = 0u;
    }

    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ReInit AudioParameterSet from %d -> %d",_ERM_u8AudioSetIdx,u8NewAudioSetIdx);
    _ERM_u8AudioSetIdx = u8NewAudioSetIdx;

    /* update values mentioned in Konzeptansicht_AudioParameterEinstellungen.ppt */
    _ERM_stAudioMod.mic_gain        =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].mic_gain;
    _ERM_stAudioMod.commVolume      =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].commVolume;
    _ERM_stAudioMod.promptVolume    =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].promptVolume;
    _ERM_stAudioMod.streamVolume    =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].streamVolume;
    _ERM_stAudioMod.alcTarget   =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxtarget;
    _ERM_stAudioMod.alcMode     =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxmode;
    _ERM_stAudioMod.slLevel     =
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].slrxlevel;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFSetVolumes                                            */
/**
    \brief      set local volumes, called by diagnosis

    \brief      check that the specific value is in correct range

    \param      Uint8Type value - volume to be set
    \param      ATAudioVolumeTypeEnum type - specific volume type to be modified

    \return     SuccessFailType SUCCESS if type and value in valid range and set
                                FAIL otherwise
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFSetVolumes(Uint8Type value,
        ATAudioVolumeTypeEnum type)
{
    SuccessFailType sF = SUCCESS;
    /* evaluate different types */
    switch (type)
    {
        case AT_AUDIOTYPE_ALC:
        {   /* check if value is in valid range */
            if ((Uint8Type)AT_VOLUME_GEN_MIN <= value &&
                (Uint8Type)AT_VOLUME_GEN_MAX >= value)
            {
                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM new audio alcVolume %d",value);
                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio alcTarget %d -> %d",\
                        _ERM_stAudioMod.alcTarget, _ERM_TelephoneBasicVolumeALCtoSLMapping[value].ALCtarget);
                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio alcTarget %d -> %d",\
                        _ERM_stAudioMod.slLevel, _ERM_TelephoneBasicVolumeALCtoSLMapping[value].SLlevel);
                _ERM_stAudioMod.alcTarget = _ERM_TelephoneBasicVolumeALCtoSLMapping[value].ALCtarget;
                _ERM_stAudioMod.slLevel = _ERM_TelephoneBasicVolumeALCtoSLMapping[value].SLlevel;
            }
            else
            {

                DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"ERM audio alc oor %d",value);
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"ERM audio alc oor .... (%d..%d)",AT_VOLUME_GEN_MIN,AT_VOLUME_GEN_MAX);
                sF = FAIL;
            }
            break;
        }
        case AT_AUDIOTYPE_COMM:
        {   /* check if value is in valid range */
            if ((Uint8Type)AT_VOLUME_GEN_MIN <= value &&
                (Uint8Type)AT_VOLUME_GEN_MAX >= value)
            {

                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio comm %d ->%d",_ERM_stAudioMod.commVolume,value);
                _ERM_stAudioMod.commVolume = value;
            }
            else
            {

                DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"ERM audio comm oor %d",value);
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"ERM audio comm oor .... (%d..%d)",AT_VOLUME_GEN_MIN,AT_VOLUME_GEN_MAX);
                sF = FAIL;
            }
            break;
        }
        case AT_AUDIOTYPE_PROMPT:
        {   /* check if value is in valid range */
            if ((Uint8Type)AT_VOLUME_PROMPT_MIN <= value &&
                (Uint8Type)AT_VOLUME_PROMPT_MAX >= value)
            {

                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio prompt %d ->%d",_ERM_stAudioMod.promptVolume,value);
                _ERM_stAudioMod.promptVolume = value;
            }
            else
            {

                DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"ERM audio prompt oor %d",value);
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"ERM audio prompt oor .... (%d..%d)",AT_VOLUME_PROMPT_MIN,AT_VOLUME_PROMPT_MAX);
                sF = FAIL;
            }
            break;
        }
        case AT_AUDIOTYPE_STREAMING:
        {   /* check if value is in valid range */
            if ((Uint8Type)AT_VOLUME_STREAM_MIN <= value &&
                (Uint8Type)AT_VOLUME_STREAM_MAX >= value)
            {

                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio stream %d ->%d",_ERM_stAudioMod.streamVolume,value);
                _ERM_stAudioMod.streamVolume = value;
            }
            else
            {

                DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"ERM audio stream oor %d",value);
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"ERM audio stream oor .... (%d..%d)",AT_VOLUME_STREAM_MIN,AT_VOLUME_STREAM_MAX);
                sF = FAIL;
            }
            break;
        }
        case AT_AUDIOTYPE_INC_RING:
        case AT_AUDIOTYPE_GET_CURRENT:
        case AT_AUDIOTYPE_GET_MINMAX:
        case AT_AUDIOTYPE_BEEP:
        default:
        {   /* incorrect type - ignore and send fail */

            DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"ERM audio inco type %d", type);
            sF = FAIL;
            break;
        }
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFSetMicSensitivity                                     */
/**
    \brief      set microphone sensitivity

    \brief      check that the specific value is in correct range

    \param      Uint8Type value - volume to be set

    \return     SuccessFailType SUCCESS if value in valid range and set
                                FAIL otherwise
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFSetMicSensitivity(Uint8Type value)
{
    SuccessFailType sF = SUCCESS;
    /* evaluate different types */

    if ((Uint8Type)AT_MIC_GAIN_MAX >= value &&
        (Uint8Type)AT_MIC_GAIN_MIN <= value)
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio micsens %d ->%d",_ERM_stAudioMod.mic_gain,value);
        _ERM_stAudioMod.mic_gain = value;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_WNG, MOD_ERM,"ERM audio micsens oor %d",value);
        DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM,"ERM audio micsens oor .... (%d..%d)",AT_MIC_GAIN_MIN,AT_MIC_GAIN_MAX);
        sF = FAIL;
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFSetAECDelay                                           */
/**
    \brief      set AEC delay value

    \brief      check that the specific value is in correct range

    \param      Uint8Type value - delay to be set

    \return     SuccessFailType SUCCESS if value in valid range and set
                                FAIL otherwise
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFSetAECDelay(Uint8Type value)
{
    SuccessFailType sF = SUCCESS;
    /* evaluate different types */

    if ((Uint8Type)AT_AEC_DELAY_MAX >= value &&
        (Uint8Type)AT_AEC_DELAY_MIN <= value)
    {
        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio aecdelay %d ->%d",_ERM_stAudioMod.aecDelay,value);
        _ERM_stAudioMod.aecDelay = value;
    }
    else
    {
        DEBUG_VALUE4(ERM_SWI_WNG, MOD_ERM,"ERM audio aecdelay oor %d range (%d..%d) %c",
                     value,
                     AT_AEC_DELAY_MIN,
                     AT_AEC_DELAY_MAX,
                     ' ');
        sF = FAIL;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetAECDelay                                             */
/**
    \brief      get AEC delay value

    \param      none

    \return     Uint8Type current AEC delay setting
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_GetAECDelay(void)
{
	return (_ERM_stAudioMod.aecDelay);
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vReInitAudioCallback                                       */
/**
    \brief      internal callback for reinitialisation of audio parmaters update

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _ERM_vReInitAudioCallback(APP_COMMON_enResponseCode rsp)
{
    /*
        Sendereihenfolge ist die Paramterauflistung aus der Spec von Bobby Haferburg
        UHV NAR Audio Parameter Recommendation vom 02.02.2010
    */

    switch (_ERM_eAudioMessageIPR)
    {
        case ERM_REQ_MIC_ALGO_SELECT:     //PALGOSELECT
            _ERM_eAudioMessageIPR = ERM_REQ_MIC_ADC_CONF;
            _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_MIC_ADC_CONF:       // PALGOADC
             _ERM_eAudioMessageIPR = ERM_REQ_MIC_TUNE_NR;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

/*
        case ERM_REQ_xxxx:                 // PALGOALCTX
             // is not sent, default is correct
        break;
*/

        case ERM_REQ_MIC_TUNE_NR:       // PALGONR
             _ERM_eAudioMessageIPR = ERM_REQ_ECHO_TUNE_CANCELLATION;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_ECHO_TUNE_CANCELLATION:       // PALGOAEC
             _ERM_eAudioMessageIPR = ERM_REQ_AMGMT_COMFORT_NOISE;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_AMGMT_COMFORT_NOISE:         // PALGONOISECOMFORT
             _ERM_eAudioMessageIPR = ERM_REQ_MIC_EQUALIZE;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_MIC_EQUALIZE:                // PALGOTXEQ
             _ERM_eAudioMessageIPR = ERM_REQ_MIC_GAIN_SET;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_MIC_GAIN_SET:                // VGM
             _ERM_eAudioMessageIPR = ERM_REQ_SPK_RX_ALGO_SELECT;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_SPK_RX_ALGO_SELECT:          // PALGOSELECTRX
             _ERM_eAudioMessageIPR = ERM_REQ_SPK_EQUALIZE;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_SPK_EQUALIZE:                // PALGORXEQ
             _ERM_eAudioMessageIPR = ERM_REQ_ECHO_DELAY_CANCELLATION;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_ECHO_DELAY_CANCELLATION:     // PALGOAECDELAY
             _ERM_eAudioMessageIPR = ERM_REQ_SPK_RX_ALC;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;
        case ERM_REQ_SPK_RX_ALC:     // PALGOALCRX
             _ERM_eAudioMessageIPR = ERM_REQ_SPK_RX_SL;
             _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;
        case ERM_REQ_SPK_RX_SL:     // PALGOSLRX
            _ERM_eAudioMessageIPR = ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM;
            _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM:   // PNVS=1,x
            _ERM_eAudioMessageIPR = ERM_REQ_AMGMT_OUT_VOLUME_SET_RING;
            _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_AMGMT_OUT_VOLUME_SET_RING:  // PNVS=2,x
            _ERM_eAudioMessageIPR = ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM;
            _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM:  // PNVS=4,x
            _ERM_eAudioMessageIPR = ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT;
            _ERM_SendMessage(_ERM_eAudioMessageIPR, _ERM_vReInitAudioCallback);
        break;

        case ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT:     // PNVS=5,x
             // last command is sent -> we are finished
             _ERM_gContext.bReInitAudioParmeter = BT_FALSE;
             _ERM_eAudioMessageIPR  = ERM_REQ_NONE;

             if (NULL != _ERM_atExtAudioCB)
             {
                 DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Sending of AudioParamters finished - call external callback");
                 _ERM_atExtAudioCB(rsp);
             }
             else
             {
                 DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Sending of AudioParamters finished");
             }
        break;
    }

}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vIntAudioCallback                                       */
/**
    \brief      internal callback for audio update

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static void _ERM_vIntAudioCallback(APP_COMMON_enResponseCode rsp)
{
    APP_COMMON_enResponseCode sendNext = APP_COMMON_RSP_OK;
    BooleanType callExtCBrequired = BT_FALSE;
    /* check for different possible audio update that might be ongoing */

    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio callback rsp=%d %d",rsp,_ERM_eAudioMessageIPR);
    if (APP_COMMON_RSP_OK == rsp)
    {
        switch (_ERM_eAudioMessageIPR)
        {
            case ERM_REQ_MIC_ALGO_SELECT:
            case ERM_REQ_MIC_CHOICE_SET:
            case ERM_REQ_MIC_TUNE_NR:
            case ERM_REQ_MIC_EQUALIZE:
            case ERM_REQ_MIC_ADC_CONF:
            case ERM_REQ_SPK_RX_ALGO_SELECT:
            case ERM_REQ_SPK_EQUALIZE:
//            case ERM_REQ_SPK_LIM_HIGHPASS:				// RT 2276
            case ERM_REQ_ECHO_TUNE_CANCELLATION:
            case ERM_REQ_AMGMT_OUT_VOLUME_SET_RING:
            case ERM_REQ_AMGMT_COMFORT_NOISE:
            case ERM_REQ_ECHO_DELAY_CANCELLATION:
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            case ERM_REQ_SPK_RX_ALC:
#endif
            {
                _ERM_eAudioMessageIPR = (ERMIntMessageEnum)(_ERM_eAudioMessageIPR + 1);
                sendNext = _ERM_SendMessage(_ERM_eAudioMessageIPR,
                        _ERM_vIntAudioCallback);
                if (APP_COMMON_RSP_OK != sendNext)
                {
                    callExtCBrequired = BT_TRUE;
                    _ERM_eAudioMessageIPR  = ERM_REQ_NONE;

                    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM audio error %d send next",sendNext);
                }
                else
                {

                    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM audio send next %d = %d",_ERM_eAudioMessageIPR,sendNext);
                }
                break;
            }
            case ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT:
            case ERM_REQ_MIC_GAIN_SET:
            case ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM:
            case ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM:
            {
                callExtCBrequired = BT_TRUE;
                _ERM_eAudioMessageIPR  = ERM_REQ_NONE;

                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM audio finish %d",__LINE__);
                break;
            }
            default: /* shall normally not occur - same handling as for error rsp */
            {
                callExtCBrequired = BT_TRUE;
                _ERM_eAudioMessageIPR  = ERM_REQ_NONE;

                DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM audio finish %d",__LINE__);
                break;
            }
        }
    }
    else
    {   /* we stop the update in any case */
        callExtCBrequired = BT_TRUE;
        _ERM_eAudioMessageIPR  = ERM_REQ_NONE;

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM audio finish %d",__LINE__);
    }


    /* only call external CB if required and available */
    if (BT_TRUE == callExtCBrequired &&
        NULL    != _ERM_atExtAudioCB)
    {
        _ERM_atExtAudioCB(rsp);

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM audio ext cb %d",__LINE__);
    }
    else
    {

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM audio no ext cb %d",__LINE__);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_sFSetAudioParameterSet                                   */
/**
    \brief      Modify the current Audio Index - Interface to diagnoses

    \brief

    \param      Uint8Type index - new index for ROM table - valid index between 0 and ERM_NUM_AUDIO_PARAM_SET-1

    \return     SuccessFailType SUCCESS if index is in valid range
                FAIL if index is NOT in valid range
   */
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode ERM_sFSetAudioParameterSet(Uint8Type AudioParameterSet,
        void (*cb)(APP_COMMON_enResponseCode))
{
    APP_COMMON_enResponseCode retVal = APP_COMMON_RSP_OK;

    if ((Uint8Type)ERM_NUM_AUDIO_PARAM_SET > AudioParameterSet)
    {   /* if CK5050 is not yet active - only update the index */
        if ((Uint8Type)FALSE  == _ERM_gContext.u8Active)
        {

            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"AudioParameterSet %d->%d CK5050 inactiv",_ERM_u8AudioSetIdx,AudioParameterSet);
            _ERM_u8AudioSetIdx    = AudioParameterSet;
            /* call the CB with response OK, the values will in the normal startup phase  */
            cb(retVal);
        }
        /* check if audio update in progress */
        else if (BT_TRUE == _ERM_gContext.bReInitAudioParmeter)
        {
            DEBUG_TEXT(ERM_SWI_WNG, MOD_ERM,"Sending of AudioParamters is busy - Please retry");
            retVal = APP_COMMON_RSP_ERROR_BUSY;
        }
        else
        {

            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"AudioParameterSet %d->%d - Start Sending of AudioParamters",_ERM_u8AudioSetIdx, AudioParameterSet);
            /* store external callback to call when audio update is done */

            /* update index and trigger audio update */
            _ERM_u8AudioSetIdx    = AudioParameterSet;
            _ERM_gContext.bReInitAudioParmeter = BT_TRUE;
            _ERM_eAudioMessageIPR = ERM_REQ_MIC_ALGO_SELECT;
            _ERM_atExtAudioCB     = cb;
            /* trigger send message */
            retVal = _ERM_SendMessage(_ERM_eAudioMessageIPR,
                    _ERM_vReInitAudioCallback);
        }
    }
    else
    {
        DEBUG_VALUE2(ERM_SWI_ERR, MOD_ERM,"AudioParameterSet: %d - not accepted max = %d !!!",AudioParameterSet,(ERM_NUM_AUDIO_PARAM_SET-1));
        retVal = APP_COMMON_RSP_ERROR;
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetMicAlgotype                                          */
/**
    \brief      Get microphone algorithm type

    \brief

    \param      none

    \return     ATMicAlgoEnum
   */
/*----------------------------------------------------------------------------*/
ATMicAlgoEnum _ERM_GetMicAlgotype(void)
{
    return((ATMicAlgoEnum)
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].algotype);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetMicChoice                                            */
/**
    \brief      Get microphone choice

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetMicChoice(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].mic_id);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetMicGain                                              */
/**
    \brief      Get microphone gain

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     ERM_GetMicGain(void)
{
    return (_ERM_stAudioMod.mic_gain);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetTXGain                                               */
/**
    \brief      Get TX equalizer gain

    \brief

    \param      Uint8Type i - index of frequency range

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetTXGain(Uint8Type i)
{
    return ((Uint8Type)_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].txGain[i]);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetRXGain                                               */
/**
    \brief      Get RX equalizer gain

    \brief

    \param      Uint8Type i - index of frequency range

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetRXGain(Uint8Type i)
{
    return ((Uint8Type)_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].rxGain[i]);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetMicMaxAtt                                            */
/**
    \brief      Get Microphone Max Attenuation

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetMicMaxAtt(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].maxAttenuationNR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetMicInputGain                                         */
/**
    \brief      Get Microphone Input Gain

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetMicInputGain(void)
{
    return ((Uint8Type)_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].inputGain);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetMicDirection                                         */
/**
    \brief      Get Microphone Direction

    \brief

    \param      none

    \return     ATNRDirectionEnum
   */
/*----------------------------------------------------------------------------*/
ATNRDirectionEnum _ERM_GetMicDirection(void)
{
    return ((ATNRDirectionEnum)
            _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].direction);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetMicGainADC                                           */
/**
    \brief      Get Microphone ADC gain

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetMicGainADC(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].micGainADC);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetRXAlgo                                               */
/**
    \brief      Get RX speaker algorithm

    \brief

    \param      none

    \return     ATSpeakerAlgoEnum
   */
/*----------------------------------------------------------------------------*/
ATSpeakerAlgoEnum     _ERM_GetRXAlgo(void)
{
    ERMSwVersionType tmp_version = ERM_GetCK5050SWVersion();
    /* return depends on the current CK5050 SW version */

    if (ERM_SW_UNDEF == tmp_version ||
        ERM_SW_1xx   == tmp_version    )
    {
        return ((ATSpeakerAlgoEnum)
                _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].algoRXold);
    }
    else
    {
        return ((ATSpeakerAlgoEnum)
                _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].algoRX);
    }

}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetSpeakerLimit                                         */
/**
    \brief      Get RX speaker limit

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetSpeakerLimit(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].limitLevel);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetHPFilter                                             */
/**
    \brief      Get RX speaker high pass filter value

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetHPFilter(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].hPFilter);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetAECMaxAtt                                            */
/**
    \brief      Get Echo Cancellation max attenuation

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetAECMaxAtt(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].maxAttenuationAEC);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetAECSwitchTime                                        */
/**
    \brief      Get Echo Cancellation switch time

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetAECSwitchTime(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].switchTime);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetRingVolume                                           */
/**
    \brief      Get volume for ringtones

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_GetRingVolume(void)
{
     //    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].ringVolume);

  /* RT#7257 Adapt VR volume in Audio-Parameterset 
     Change Request: Jingle / Ringtone is dependent of VR-Volume*/


    return (_ERM_RingtoneMappingTable[ERM_GetPromptVolume()]);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetPromptVolume                                         */
/**
    \brief      Get volume for prompts

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     ERM_GetPromptVolume(void)
{
    return (_ERM_stAudioMod.promptVolume);
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_InitPromptVolume                                         */
/**
    \brief      Get volume for prompts (read from E2P),
                in case of E2P error use ROM as default.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type _ERM_InitPromptVolume(void)
{
   static Uint8Type u8PromptVolume;

   CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8PromptVolume, EEPROM_LAYOUT_ENUM_VoiceOutputBasicVolume);

   if( COMMON_RSP_OK != sF )
   {
       // Default: take ROM value
       u8PromptVolume = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].promptVolume;
       DEBUG_VALUE1(ERM_SWI_ERR,MOD_ERM,"EEPROM Error prompt volume= %d, taken from audio parameter set",u8PromptVolume);
   }
   else
   {
       // Rangecheck, otherwise E2P value is OK
       if (AT_VOLUME_PROMPT_MIN > u8PromptVolume ||
           AT_VOLUME_PROMPT_MAX < u8PromptVolume)
       {
           // Value of of E2P is invalid  -> take ROM value
           u8PromptVolume = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].promptVolume;
           DEBUG_VALUE1(ERM_SWI_ERR,MOD_ERM,"Range error prompt volume= %d, taken from audio parameter set",u8PromptVolume);
       }
       else
       {
           DEBUG_VALUE1(ERM_SWI_ERR,MOD_ERM,"ERM prompt volume= %d, taken from from EEPROM",u8PromptVolume);
       }
   }

   return(u8PromptVolume);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_InitMicGain                                         */
/**
    \brief      Get volume for prompts (read from E2P),
                in case of E2P error use ROM as default.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type _ERM_InitMicGain(void)
{
   static Uint8Type u8MicGain;

   CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8MicGain, EEPROM_LAYOUT_ENUM_MicrophoneSensitivity);

   if( COMMON_RSP_OK != sF )
   {
       // Default: take ROM value
       u8MicGain = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].mic_gain;
   }
   else
   {
       // Rangecheck, otherwise E2P value is OK
       if (u8MicGain  > AT_MIC_GAIN_MAX)
       {
           // Value of of E2P is invalid  -> take ROM value
           u8MicGain = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].mic_gain;
       }
   }
   return(u8MicGain);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_InitCommVolume                                         */
/**
    \brief      Get value for volume communcation. Default value from active
                AudioParameterSet is used.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_InitCommVolume(void)
{
    return _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].commVolume;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_InitStreamVolume                                         */
/**
    \brief      Get volume for audio streamning (read from E2P),
                in case of E2P error use ROM as default.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_InitStreamVolume(void)
{
   static Uint8Type u8StreamVolume;

   CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8StreamVolume, EEPROM_LAYOUT_ENUM_MediaDeviceBasicVolume);

   if( COMMON_RSP_OK != sF )
   {
       // Default: take ROM value
       u8StreamVolume = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].streamVolume;
   }
   else
   {
       // Rangecheck, otherwise E2P value is OK
       if (u8StreamVolume  > AT_VOLUME_STREAM_MAX)
       {
           // Value of of E2P is invalid  -> take ROM value
           u8StreamVolume = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].streamVolume;
       }
   }
   return(u8StreamVolume);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_InitAecDelay                                             */
/**
    \brief      Get volume for AEC delay (read from E2P),
                in case of E2P error use ROM as default.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     _ERM_InitAecDelay(void)
{
   static Uint8Type u8AecDelay;

   CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8AecDelay, EEPROM_LAYOUT_ENUM_Reference_Channel_Delay);

   if( COMMON_RSP_OK != sF )
   {
       // Default: take ROM value
       u8AecDelay = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].aecDelay;
   }
   else
   {
       // Rangecheck, otherwise E2P value is OK
       if (u8AecDelay  > AT_AEC_DELAY_MAX)
       {
           // Value of of E2P is invalid  -> take ROM value
           u8AecDelay = _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].aecDelay;
       }
   }
   return(u8AecDelay);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetCommVolume                                           */
/**
    \brief      Get volume for communication

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     ERM_GetCommVolume(void)
{
    return (_ERM_stAudioMod.commVolume);
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetCommVolume                                           */
/**
    \brief      Get ALC controlled volume value.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     ERM_GetAlcVolume(void)
{
    return _ERM_GetAlcVolumeFromALCtoSLMappingTable(_ERM_stAudioMod.alcTarget, _ERM_stAudioMod.slLevel);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetStreamVolume                                         */
/**
    \brief      Get volume for streaming

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type     ERM_GetStreamVolume(void)
{
    return (_ERM_stAudioMod.streamVolume);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetComfortNoise                                         */
/**
    \brief      Get comfort noise setting

    \brief

    \param      none

    \return     ATBooleanEnum AT_TRUE if to switch on
                              AT_FALSE if to switch off
   */
/*----------------------------------------------------------------------------*/
ATBooleanEnum     _ERM_GetComfortNoise(void)
{
	ATBooleanEnum bRetVal = AT_TRUE;
	if (0x00 == _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].comfortNoise)
	{   /* 0 means off - set to FALSE*/
		bRetVal = AT_FALSE;
	}

    return bRetVal;

}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_InitRXAlcTarget                                         */
/**
    \brief      Intitiate the RX alc target value in moding audio structure.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_InitRXAlcTarget(void)
{
    Uint8Type u8TelephoneBasicVolumeFromODX;
    Uint8Type retVal = 0x00u;

    if ( 0x03 == _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxmode)
    {
        CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8TelephoneBasicVolumeFromODX, EEPROM_LAYOUT_ENUM_TelephoneBasicVolume);

        if( COMMON_RSP_OK != sF )
        {
            // Default: take ROM value
            retVal = (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxtarget);
        }
        else
        {
            // Rangecheck, otherwise E2P value is OK
            if (AT_VOLUME_GEN_MAX <= u8TelephoneBasicVolumeFromODX && AT_VOLUME_GEN_MIN > u8TelephoneBasicVolumeFromODX)
            {
                // Value of of E2P is invalid  -> take ROM value
                retVal = (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxtarget);
            }
            else
            {
                retVal = _ERM_TelephoneBasicVolumeALCtoSLMapping[u8TelephoneBasicVolumeFromODX].ALCtarget;
                DEBUG_VALUE2(ERM_SWI_TRC,MOD_ERM,"ODX value = %d, take value = %d",u8TelephoneBasicVolumeFromODX,retVal);
            }
        }
    }

    return retVal;
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetRXAlcTarget                                          */
/**
    \brief      Get RX alc target value.

                Decide if take from Dataset or the Audioparameter set.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_GetRXAlcTarget(void)
{
    return (_ERM_stAudioMod.alcTarget);
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetRXAlcMode                                                */
/**
    \brief      Get RX alc mode value.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_GetRXAlcMode(void)
{
    return _ERM_stAudioMod.alcMode;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_InitAlcMode                                                */
/**
    \brief      Get RX alc mode value.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_InitAlcMode(void)
{
    return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxmode);
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_InitRXSLLevel                                            */
/**
    \brief      Initiate the .

                Decide if take from Dataset or the Audioparameter set.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_InitRXSLLevel(void)
{
    Uint8Type u8TelephoneBasicVolumeFromODX;
    Uint8Type retVal = 0x00u;

    /* if ALC 3.0 is active, output volume is controlled with ALc and SL */
    if (0x03 == _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].alcrxmode)
    {
        CommonResponseCodeType sF = EEPROM_enGetSingleByteFromRamMirror(&u8TelephoneBasicVolumeFromODX, EEPROM_LAYOUT_ENUM_TelephoneBasicVolume);

        if( COMMON_RSP_OK != sF )
        {
            // Default: take ROM value
            retVal =  (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].slrxlevel);
        }
        else
        {
            // Rangecheck, otherwise E2P value is OK
            if (AT_VOLUME_GEN_MAX < u8TelephoneBasicVolumeFromODX  && AT_VOLUME_GEN_MIN > u8TelephoneBasicVolumeFromODX)
            {
                // Value of of E2P is invalid  -> take ROM value
                retVal =  (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].slrxlevel);
            }
            else
            {
                retVal =  _ERM_TelephoneBasicVolumeALCtoSLMapping[u8TelephoneBasicVolumeFromODX].SLlevel;
                DEBUG_VALUE2(ERM_SWI_TRC,MOD_ERM,"ODX value = %d, take value = %d",u8TelephoneBasicVolumeFromODX,retVal);
            }
        }

    }
    return retVal;
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetRXSLLevel                                            */
/**
    \brief      Get RX alc target value.

                Decide if take from Dataset or the Audioparameter set.

    \brief

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_GetRXSLLevel(void)
{
    return (_ERM_stAudioMod.slLevel);
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetTXSLMode                                             */
/**
    \brief      Get TX soft limiter mode value.

                decide if taken from dataset or the audioset table.

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
BooleanType         _ERM_GetTXSLMode(void)
{
    if (0x01 == _ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].sltxmode)
    {
        return BT_TRUE;
    }
    else
    {
        return BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetTXSLLevel                                            */
/**
    \brief      Get TX soft limiter level value.

                decide if taken from dataset or the audioset table.

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_GetTXSLLevel(void)
{
        return (_ERM_stAudioTable.audioSet[_ERM_u8AudioSetIdx].sltxlevel);
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetAlcVolumeFromALCtoSLMappingTable                                            */
/**
    \brief      Get TX soft limiter level value.

                decide if taken from dataset or the audioset table.

    \param      none

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type         _ERM_GetAlcVolumeFromALCtoSLMappingTable(Uint8Type alcTarget, Uint8Type slLevel)
{
    Uint8Type volume;

    for (volume = 0; volume < MAX_ALC_MAP_STEPS; volume++)
    {
        if ((_ERM_TelephoneBasicVolumeALCtoSLMapping[volume].ALCtarget == alcTarget)&&\
                (_ERM_TelephoneBasicVolumeALCtoSLMapping[volume].SLlevel == slLevel))
        {
            break;
        }
    }
    /* didn't found a matching setup, so set the default value */
    if (MAX_ALC_MAP_STEPS == volume)
    {
        volume = AT_VOLUME_GEN_DEFAULT;
        DEBUG_VALUE2(ERM_SWI_TRC,MOD_ERM,"No volume value found for alcTarget= %d, slLevel= %d",alcTarget,slLevel);
    }

    return volume;
}

/* End Of File ERM_AudioSets.c */
