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
* $Date: 2011-09-13 16:09:46 +0200 (Di, 13 Sep 2011) $
* $Rev: 21714 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/inc/ERM_AudioTable.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ERM_AUDIO_TABLE_H
#define _ERM_AUDIO_TABLE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CE.h"
#include "ERM_Utils.h"
#include "ERM.h"
#include "ERM_AudioSets.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
#include "ERM_AudioSets.h"
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/* #define ERM_AUDIO_SET_VERSION    1  initial version */
/* #define ERM_AUDIO_SET_VERSION    2  add palgonoisecomfort */
/* #define ERM_AUDIO_SET_VERSION    3  add aecDelay */
/* #define ERM_AUDIO_SET_VERSION    4   add alcRX */
/* #define ERM_AUDIO_SET_VERSION    5   provide RX algo for SW before 200 and others */
/* #define ERM_AUDIO_SET_VERSION    6*/  /* provide AlcRX paramters in correct form, activation of ALC3.0 */
#define ERM_AUDIO_SET_VERSION    7  /* enable soft limiter for receive and transmission, since parrot V3.24.2 */

//#pragma DATA_SECTION(_ERM_stAudioTable, ".UHV_AUDIO_SETS");

const ERMAudioTableType  _ERM_stAudioTable =
{   /* header of the structure */
    ERM_NUM_AUDIO_PARAM_SET,          /* number of the audio sets      */
    sizeof(ERMAudioDataSetType),      /* size of one set               */
    /* start address of first set    */
    (Uint32Type)&_ERM_stAudioTable + 4* sizeof(Uint32Type),
    /* end address of this structure */
    (Uint32Type)&_ERM_stAudioTable + 4* sizeof(Uint32Type) +
        ERM_NUM_AUDIO_PARAM_SET * sizeof(ERMAudioDataSetType),
    {
        /* VAS-Tester: Codierung -> Klartextcodierung -> AudioParameter */
        /* ------------ INDEX 0 = SET1 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            8,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            5,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,-2,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            15,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 1 = SET2 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            6,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            9,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,-2,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            15,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 2 = SET3 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            5,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            5,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,0,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            15,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 3 = SET4 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            10,                     /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            11,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
               0,2,2,-2,-2,0,-2,-2,2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            15,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 4 = SET5 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            8,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            10,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,-2,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            15,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 5 = SET6 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            6,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            9,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,-2,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            13,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* soft limiter enabled */
            2,                      /* soft limiter level */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 6 = SET7 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            5,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            5,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,0,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            12,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 7 = SET8 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            10,                     /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            11,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
               0,2,2,-2,-2,0,-2,-2,2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            11,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 8 = SET9 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            5,                      /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            5,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
              -2,0,2,0,-2,-2,-2,-2,-2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            10,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        },
        /* ------------ INDEX 9 = SET10 AUDIO PARAMETER -----------------*/
        {
            ERM_AUDIO_SET_VERSION,  /* version  */
            /* AT*PALGOSELECT */
            11,                     /* algotype */
            /* AT*PALGOSETMIC */
            0,                      /* mic id   */
            /* AT*VGM */
            10,                     /* mic_gain */
            /* AT*PALGONR */
            15,                     /* maxAttenuationNR */
            0,                      /* direction        */
            11,                      /* inputGain        */
            /* AT*PALGOTXEQ */
            {
               0,2,2,-2,-2,0,-2,-2,2  /* txGain */
            },
            /* AT*PALGOADC */
            AT_ADC_GAIN_MIN,        /* micGainADC */
            1,                      /* micPowerSupply */
            1,                      /* micSingleEnded */
            /* AT*PALGOSELECTRX */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
            2,                      /* algoRX for 200 and post */
#else
            1,                      /* algoRX for 200 and post */
#endif
            0,                      /* algoRX */
            /* AT*PALGORXEQ */
            {
              -4,0,0,0,0,0,0,0,0    /* rxGain */
            },
            /* AT*PALGORX */
            AT_RX_LIMITLEV_DEFAULT, /* LimitLevel */
            AT_RX_HPFILTER_DEFAULT, /* hPFilter */
            /* AT*PALGOAEC */
            17,                     /* maxAttenuationAEC */
            23,                     /* switchTime */
            /* AT*PNVS */
            9,                     /* commVolume */
            8,                      /* ringVolume */
            60,                     /* promptVolume */
            63,                     /* streamVolume */
            /* AT*PALGOALCRX */
            3,                      /* alcrxmode */
            10,                     /* alcrxtarget */
            /* AT*PALGOSLRX */
            4,                      /* soft limiter level */
            /* AT*PALGOSLPGTX */
            1,                      /* max soft limiter */
            2,                     /* active soft limiter */
            /* AT*PALGOCOMPRESSOR */
            0,                      /* compressormode */
            /* AT*PALGONRRX */
            14,                     /* nrLevel */
            /* AT*PALGOCOMFORTNOISE */
            1,                      /* palgonoisecomfort = on */
            /* AT*PALGOAECDELAY */
            0,                     /* aecDelay */
            8,                     /* canceled parameter - alcRX */
            /* spare data */
            {
                0,0,0,0,0,0,0       /* spare bytes */
            }
        }
    }
};

#endif /* _ERM_AUDIO_TABLE_H */

/* End Of File ERM_AudioTable.h */
