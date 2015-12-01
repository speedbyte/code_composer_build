/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_AudioSets.h
*-----------------------------------------------------------------------------
* Module Name:    ERM
*-----------------------------------------------------------------------------
* Description:    This header file include static Audio Set Table
*-----------------------------------------------------------------------------
* $Date: 2011-08-19 17:06:13 +0200 (Fr, 19 Aug 2011) $
* $Rev: 21287 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/inc/ERM_AudioSets.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ERM_AUDIO_SETS_H
#define _ERM_AUDIO_SETS_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CE.h"
#include "ERM_Utils.h"
#include "ERM.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/* NOTE: due to limited space we use the minimum byte type, although for some
 * of the data this means casting to specific enum types may be necessary !! */
typedef struct {
    Uint8Type           version;           /* to allow different versions    */
    /* values for PALGOSELECT */
    Uint8Type           algotype;          /* range 0..7, default 5          */
    /* values for PALGOSETMIC */
    Uint8Type           mic_id;            /* range 0..1, default 0          */
    /* values for VGM */
    Uint8Type           mic_gain;          /* range 0..15, default 8         */
    /* values for PALGONR */
    Uint8Type           maxAttenuationNR;  /* range 0..25, default 14        */
    Uint8Type           direction;         /* range 0..4, default 1          */
    Sint8Type           inputGain;         /* range -12..+12, default 0      */
    /* values for PALGOTXEQ */
    Sint8Type           txGain[AT_EQU_NUM_FREQ_BAND]; /* range -10..10, default 0 */
    /* values for PALGOADC */
    Uint8Type           micGainADC;        /* values 0,15,30 default 0       */
    Uint8Type           micPowerSupply;    /* range 0..1, default 1          */
    Uint8Type           micSingleEnded;    /* range 0..1, default 1          */
    /* values for PALGOSELECTRX */
    Uint8Type           algoRX;            /* range 0..2, default 0 SW >= 200  */
    Uint8Type           algoRXold;         /* range 0..2, default 0 SW before 200  */
    /* values for PALGORXEQ */
    Sint8Type           rxGain[AT_EQU_NUM_FREQ_BAND]; /* range -10..10, default 0 */
    /* values for PALGORX */
    Uint8Type           limitLevel;        /* range 0..30, default 0         */
    Uint8Type           hPFilter;          /* range 0..9, default 0          */
    /* values for PALGOAEC */
    Uint8Type           maxAttenuationAEC; /* range 0..30, default 7         */
    Uint8Type           switchTime;        /* range 1..100, default 23       */
    /* values for PNVS */
    Uint8Type           commVolume;        /* range 0..15, default 15        */
    Uint8Type           ringVolume;        /* range 0..15, default 8         */
    Uint8Type           promptVolume;      /* range 0..15, default 8         */
    Uint8Type           streamVolume;      /* range 0..63, default 48        */
    /* values for PALGOALCRX */
    Uint8Type           alcrxmode;         /* range 0..3, default 0          */
    Uint8Type           alcrxtarget;       /* for ALC 1.0: 0 (0 dBFS) to 15 (-15dBFS)
                                              for ALC 3.0: 0 (0 dB) to 40 (-40dB)*/

    /* values for PALGOSLRX */
    Uint8Type           slrxlevel;          /* receive soft limiter level in -(x)dB */

    /* values for PALGOSLPGTX */
    Uint8Type           sltxmode;           /* softlimiter mode (1=enabled/0=disabled) */
    Uint8Type           sltxlevel;          /* active softlimiter in -(x)dB */

    /* values for PALGOCOMPRESSOR */
    Uint8Type           compressormode;    /* range 0..1, default 0          */
    /* values for PALGONRRX */
    Uint8Type           nrLevel;           /* range 0..25, default 14        */
    /* values for PALGOCOMFORTNOISE */
    Uint8Type           comfortNoise;      /* range 0..1 for off / on        */
    /* values for PALGOAECDELAY */
    Uint8Type           aecDelay;          /* range 0..25int(0..100ms) def 0 */
    Uint8Type           alcRx;             /* not use any more                */
    /* spare data */
    Uint8Type           spare_data[ERM_SPARE_AUDIO_BYTES];
} ERMAudioDataSetType;


/* FLASH / ROM table structure */
typedef struct {
    Uint32Type            numberEntries; /* available number of audio sets */
    Uint32Type            sizeofEntry;   /* size of one entry              */
    Uint32Type            startAddress;  /* address of first byte of set 0 */
    Uint32Type            endAddress;    /* end address of this structure  */
    ERMAudioDataSetType   audioSet[ERM_NUM_AUDIO_PARAM_SET];
} ERMAudioTableType;

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/


void              _ERM_vInitAudioValues(void);



SuccessFailType _ERM_sFSetMicSensitivity(Uint8Type value);
SuccessFailType _ERM_sFSetVolumes(Uint8Type value,
        ATAudioVolumeTypeEnum type);

ATMicAlgoEnum     _ERM_GetMicAlgotype(void);
Uint8Type         _ERM_GetMicChoice(void);
Uint8Type         _ERM_GetMicGain(void);
Uint8Type         _ERM_GetTXGain(Uint8Type i);
Uint8Type         _ERM_GetRXGain(Uint8Type i);
Uint8Type         _ERM_GetMicInputGain(void);
Uint8Type         _ERM_GetMicMaxAtt(void);
ATNRDirectionEnum _ERM_GetMicDirection(void);
Uint8Type         _ERM_GetMicGainADC(void);
ATSpeakerAlgoEnum _ERM_GetRXAlgo(void);
Uint8Type         _ERM_GetSpeakerLimit(void);
Uint8Type         _ERM_GetHPFilter(void);
Uint8Type         _ERM_GetAECMaxAtt(void);
Uint8Type         _ERM_GetAECSwitchTime(void);
Uint8Type         _ERM_GetRingVolume(void);
ATBooleanEnum     _ERM_GetComfortNoise(void);
SuccessFailType   _ERM_sFSetAECDelay(Uint8Type value);
Uint8Type         _ERM_GetAECDelay(void);
Uint8Type         _ERM_GetRXAlcTarget(void);
Uint8Type         _ERM_GetRXAlcMode(void);
Uint8Type         _ERM_GetAlcVolumeFromALCtoSLMappingTable(Uint8Type, Uint8Type);
Uint8Type         _ERM_GetRXSLLevel(void);
BooleanType       _ERM_GetTXSLMode(void);
Uint8Type         _ERM_GetTXSLLevel(void);
Uint8Type     _ERM_GetPromptVolume(void);
Uint8Type     _ERM_GetCommVolume(void);
Uint8Type     _ERM_GetStreamVolume(void);

Uint8Type     _ERM_GetRXAlcTarget(void);
Uint8Type     _ERM_GetRXSLLevel(void);

Uint8Type     _ERM_InitPromptVolume(void);
Uint8Type     _ERM_InitMicGain(void);
Uint8Type     _ERM_InitCommVolume(void);
Uint8Type     _ERM_InitStreamVolume(void);
Uint8Type     _ERM_InitAecDelay(void);
Uint8Type     _ERM_InitAlcVolume(void);
Uint8Type     _ERM_InitAlcMode(void);
Uint8Type     _ERM_InitRXAlcTarget(void);
Uint8Type     _ERM_InitRXSLLevel(void);

#endif /* _ERM_AUDIO_SETS_H */

/* End Of File ERM_AudioSets.h */
