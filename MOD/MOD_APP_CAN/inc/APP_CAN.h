/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/******************************************************************************/
/* File Name:     APP_CAN.h
*-----------------------------------------------------------------------------
* Module Name:    APP_CAN
*-----------------------------------------------------------------------------
* Description:    Handle CAN related doings for the applications
*-----------------------------------------------------------------------------
* $Date: 2010-10-06 18:06:26 +0200 (Mi, 06 Okt 2010) $
* $Rev: 15606 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_CAN/trunk/inc/APP_CAN.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_CAN_H
#define _APP_CAN_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_CAN_CE.h"
#include "APP_CAN_CI.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define WEICHKOMCTR      300   /* Weiche Kommunikation: watching for received */  //#3715
                               /* messages after 300*10ms = 3000ms            */
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef struct
{
    Uint8Type MFAvorhanden;
    Uint8Type MFLvorhanden;
    Uint8Type MFAtasten;
    Uint8Type MFLtyp;
}LSMmessageType;



typedef union
{
    Uint32Type instr;
    struct
    {
        struct
        {
            Uint8Type klemme15:1;
            Uint8Type klemmeS:1;
            Uint8Type dummy:6;
        } BSG3;
        struct
        {
            Uint8Type warnInfo:1;
            Uint8Type ausInfo:1;
            Uint8Type dummy:6;
        } BSG2;
        struct
        {
            Uint8Type onOffState:1;
            Uint8Type dummy:7;
        } Radio;
        struct
        {
            Uint8Type onOffState:1;
            Uint8Type dummy:7;
        } Remote;
    }CanSignalMessageBit;
}CanSignalMessageType;

typedef union
{
    Uint8Type  instr;
    struct
    {
        Uint8Type   Gateway3      :1; /* mGateway_3 message missed       */
                                      /* 0:no missed 1:missed            */
        Uint8Type   Systeminfo1   :1; /* mSysteminfo_1 message missed    */
                                      /* 0:no missed 1:missed            */
        Uint8Type   Diagnose1     :1; /* mDiagnose_1 message missed      */
                                      /* 0:no missed 1:missed            */
        Uint8Type   GwKombi       :1; /* mGW_Kombi message missed        */
                                      /* 0:no missed 1:missed            */
        Uint8Type   Sollverbau    :1; /* mSollverbau_neu message missed  */
                                      /* 0:no missed 1:missed            */
        Uint8Type                 :3; /* not used                        */
                                      /* 0:default                       */
    } MessageBits;
} MissedMessageType;

typedef enum
{
    SPEED,
    SD_LANGUAGE,
    MARKE,
    RADIO
}enCommonStates;

typedef enum
{
    overSpeed,
    underSpeed
}SPEED_enSpeed;

typedef enum
{
/* 0  */ APP_CAN_keine_Sprachvar = 0,
/* 1  */ APP_CAN_Deutsch,
/* 2  */ APP_CAN_Englisch,
/* 3  */ APP_CAN_Franzoesisch,
/* 4  */ APP_CAN_Italienisch,
/* 5  */ APP_CAN_Spanisch,
/* 6  */ APP_CAN_Portugiesisch,
/* 7  */ /* not used */
/* 8  */ APP_CAN_Tschechisch = 8,
/* 9  */ APP_CAN_Chinesisch,
/* 10 */ APP_CAN_US_Englisch,
/* 11 */ APP_CAN_Niederlaendisch,
/* 12 */ APP_CAN_Japanisch,
/* 13 */ APP_CAN_Russisch,
/* 14 */ APP_CAN_Koreanisch,
/* 15 */ APP_CAN_Franko_Kanadisch,
/* 16 */ APP_CAN_Schwedisch,
/* 17 */ APP_CAN_Polnisch,
/* 18 */ APP_CAN_Tuerkisch,
/* 19 */ APP_CAN_US_Spanisch,
/* 20 */ APP_CAN_Norwegisch
} APP_CAN_enLang;

typedef enum
{
/*  0 */ APP_CAN_VW_Volkswagen = 0,
/*  1 */ APP_CAN_AU_Audi,
/*  2 */ APP_CAN_SE_Seat,
/*  3 */ APP_CAN_SK_Skoda,
/*  4 */ APP_CAN_VN_VWNutzfzg,
/*  5 */ APP_CAN_Bugatti,
/*  6 */ APP_CAN_Lamborghini,
/*  7 */ APP_CAN_Bentley,
/*  8 */ APP_CAN_RollsRoyce = 8,
/* 14 */ APP_CAN_Ford = 14,
/* 15 */ APP_CAN_Porsche = 15,
/* 255 */ APP_CAN_NoMarke = 0xFF
} APP_CAN_ENUM_MARKE;

typedef enum
{
/*  0 */ APP_CAN_Kurzheck_Multivan = 0x0,
/*  1 */ APP_CAN_Stufenheck_Nutz = 0x1,
/*  2 */ APP_CAN_Variant = 0x2,
/*  3 */ APP_CAN_Fliessheck = 0x3,
/*  4 */ APP_CAN_Coupe_Sportwagen = 0x4,
/*  5 */ APP_CAN_Cabrio_Roadstar_Spider = 0x5,
/*  6 */ APP_CAN_Offroad = 0x6,
/*  7 */ APP_CAN_CityVan_PickUp = 0x7,
/*  8 */ APP_CAN_MPV = 0x8,
/*  9 */ APP_CAN_Sonstige = 0x9,
/* 15 */ APP_CAN_NoDerivat = 0xF
} APP_CAN_ENUM_DERIVAT;

typedef enum
{
    enAppCanNothingAvailable = 0x00,
	enAppCanMflAvailable = 0x01,
	enAppCanLssAvailable = 0x02,
	enAppCanLssAndMflAvailable = 0x03
} APP_CAN_ENUM_LSM_VERBAU;

typedef enum
{
    APP_CAN_MFL_VarianteNormal = 0x06,
    APP_CAN_MFL_VarianteLow = 0x09,
    APP_CAN_MFL_NoVariante = 0x0F
}APP_CAN_ENUM_MFL_VARIANTE;

typedef struct
{
    enCommonStates common;
    SPEED_enSpeed speed;

}APP_CAN_SpeedStates;

typedef struct
{
    enCommonStates common;
    APP_CAN_enLang sd_language;
}APP_CAN_LanguageType;

typedef struct
{
    enCommonStates common;
    APP_CAN_ENUM_MARKE marke;
}APP_CAN_MarkeType;

typedef struct
{
    enCommonStates common;
    Uint8Type mute;
    Uint8Type onOff;
}APP_CAN_RadioType;

typedef union
{
    enCommonStates common;
    APP_CAN_SpeedStates speed;
    APP_CAN_LanguageType sd_lang;
    APP_CAN_MarkeType marke;
    APP_CAN_RadioType radio;
}APP_CAN_MsgStates;


typedef struct
{
    Uint16Type jear;
    Uint8Type month;
    Uint8Type day;
    Uint32Type kilometer;
    Uint8Type verlernzaehler;
}Diagnose1MessageType;


typedef enum
{
/* 0 */    APP_CAN_enTelephoneCall = 0,
/* 1 */    APP_CAN_enVoicePrompts = 1,
/* 2 */    APP_CAN_enEmergencyCall = 2,
/* 3 */    APP_CAN_enNavigation = 3,
/* 4 */    APP_CAN_enOperateCall = 4,
/* 5 */    APP_CAN_enRing = 5,
/* 6 */    APP_CAN_enBroadBandCall = 6,
/* 7 */    APP_CAN_enAudioStream = 7,
/* 254 */  APP_CAN_enForceUnmuteAtShutDown = 0xFE,
           APP_CAN_enInvalidSource = 0xFF 
}APP_CAN_enSourceIdType;


typedef enum
{
    APP_CAN_enAudioUnmute = 0,
    APP_CAN_enAudioMute
}APP_CAN_enAudioStatusType;
/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/


SuccessFailType APP_CAN_sfInit(void);


void APP_CAN_vHandleEvents(EventMaskType event);


SuccessFailType APP_CAN_sf10msCyclicCheckReceivedCANMessages(void);


SuccessFailType APP_CAN_sfCheckBusOff(void);


SuccessFailType APP_CAN_sfSendCyclicCanMessage(EventMaskType event);


Uint16Type APP_CAN_SleepDelayTimer(void);


void APP_CAN_ResetSleepDelayTimer(void);


Uint8Type APP_CAN_u8Klemme15Status(void);


Uint8Type APP_CAN_u8DTCStore(void);


Uint8Type APP_CAN_u8EigenDiagDTCStore(void);


APP_CAN_ENUM_LSM_VERBAU APP_CAN_u8GetActiveLSM1Device(void);


APP_CAN_ENUM_MARKE APP_CAN_enGetMarke(void);


APP_CAN_ENUM_MARKE APP_CAN_enGetDerivat(void);


Uint8Type APP_CAN_u8GetNWDF(void);


void APP_CAN_sfReInitTOVariables(void);


void APP_CAN_vResetLanguageDatasetdownload(void);


APP_CAN_ENUM_MFL_VARIANTE APP_CAN_u8GetMflVariante(void);


Diagnose1MessageType APP_CAN_vDiagnose_1_Data(void);


Uint8Type APP_CAN_sfDiagnose_1_MessageReceived(void);


APP_CAN_enAudioStatusType APP_CAN_enGetMuteStatus(void);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#endif /* _APP_CAN_H */

/* End Of File APP_CAN.h */

