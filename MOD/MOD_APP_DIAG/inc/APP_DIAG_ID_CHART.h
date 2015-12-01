/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	APP_DIAG_ID_CHART.h
 * \brief	C File Header. 
 * 			This file must be included by the C File in order to use the 
 * 			constants, variables etc declared in this file. Detailed description found below. 
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-05-13 08:46:42 +0200 (Fr, 13 Mai 2011) $
* $Rev: 19661 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DIAG/trunk/inc/APP_DIAG_ID_CHART.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DIAG_ID_CHART_H
#define _APP_DIAG_ID_CHART_H

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

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
#define CASE_ALL_ID_SECURITY  case ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG : \
                              case ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES : \
                              case ID_2E_F1A1_VERSION_DES_PARAMETERSATZES : \
                              case ID_2E_F1A2_ASAM_ODX_DATEIVERSION : \
                              case ID_2E_F1A3_HARDWAREVERSION : \
                              case ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE :



/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*CalibrationDataWritable*/
// TODO: Clean up  0x0317, CASE_ALL_ID_ANPASSUNG. Service 31 should be
// called directly from sds_ext.c without the intervention of Service2E.

#define ID_2E_0500_ANALYSE_1              0x0500
#define ID_31_0317_RESET_TO_FACTORY_SETTING              0x0317
#define ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL              0x0909
#define ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL              0x090A
#define ID_2E_090B_SET_NUMBER_FOR_INFO_CALL              0x090B
#define ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL              0x090C
#define ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT              0x0911

#define ID_2E_0A07_CDMA_IMSI_ON_OFF                              0x0A07
#define ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL              0x0914
#define ID_2E_1001_DEVELOPER_TESTMODE              0x1001
#define ID_2E_2233_MASKING_OF_LANGUAGES              0x2233
#define ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE              0x2416
#define ID_2E_241C_TELEPHONE_BASIC_VOLUME              0x241C
#define ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME              0x241D
#define ID_2E_241E_MICROPHONE_SENSITIVITY              0x241E
#define ID_2E_2420_BLUETOOTH_SET_PIN_CODE              0x2420
#define ID_2E_2422_USER_PROFILES              0x2422
#define ID_2E_2423_MUTE_DELAY              0x2423
#define ID_2E_2424_REFERENCE_CHANNEL_DELAY              0x2424
#define ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME              0x2429
#define ID_2E_2448_EMERGENCY_NUMBER              0x2448
#define CASE_ALL_ID_ANPASSUNG       \
					 case ID_2E_0500_ANALYSE_1: \
					 case ID_31_0317_RESET_TO_FACTORY_SETTING: \
					 case ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL: \
					 case ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL: \
					 case ID_2E_090B_SET_NUMBER_FOR_INFO_CALL: \
					 case ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL: \
					 case ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT: \
					 case ID_2E_0A07_CDMA_IMSI_ON_OFF: \
					 case ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL: \
					 case ID_2E_1001_DEVELOPER_TESTMODE: \
					 case ID_2E_2233_MASKING_OF_LANGUAGES: \
					 case ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE: \
					 case ID_2E_241C_TELEPHONE_BASIC_VOLUME: \
					 case ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME: \
					 case ID_2E_241E_MICROPHONE_SENSITIVITY: \
					 case ID_2E_2420_BLUETOOTH_SET_PIN_CODE: \
					 case ID_2E_2422_USER_PROFILES: \
					 case ID_2E_2423_MUTE_DELAY: \
					 case ID_2E_2424_REFERENCE_CHANNEL_DELAY: \
					 case ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME: \
					 case ID_2E_2448_EMERGENCY_NUMBER: \
/*EcuIdentification*/
#define ID_22_0405_STATUS_DES_PROGRAMMSPEICHERS              0x0405
#define ID_22_0407_ANZAHL_DER_PROGRAMMIERVERSUCHE              0x0407
#define ID_22_0408_ANZAHL_ERFOLGREICHER_PROGRAMMIERVERSUCHE              0x0408
#define ID_22_0409_ANZAHL_DER_PARAMETRIERVERSUCHE              0x0409
#define ID_22_040A_ANZAHL_ERFOLGREICHER_PARAMETRIERVERSUCHE              0x040A
#define ID_22_040F_VW_LOGICAL_SOFTWARE_BLOCK_LOCK_VALUE              0x040F
#define ID_22_F15B_FINGERPRINT              0xF15B
#define ID_22_F17B_DATUM_DER_LETZTEN_CODIERUNG              0xF17B
#define ID_22_F17C_FAZIT_SERIENNUMMER              0xF17C
#define ID_22_F17E_HERSTELLERAENDERUNGSSTAND              0xF17E
#define ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE              0xF181
#define ID_22_F182_IDENTIFIKATION_DER_PARAMETRIERUNG              0xF182
#define ID_22_F187_VW_AUDI_TEILENUMMER              0xF187
#define ID_22_F189_SOFTWAREVERSION              0xF189
#define ID_22_F18C_SERIENNUMMER              0xF18C
#define ID_22_F191_HARDWARETEILENUMMER              0xF191
#define ID_22_F197_SYSTEMBEZEICHNUNG              0xF197
#define ID_22_F199_DATUM_DER_LETZTEN_UPDATE_PROGRAMMIERUNG              0xF199
#define ID_22_F19A_WERKSTATTCODE_DER_ANPASSUNG              0xF19A
#define ID_22_F19B_DATUM_DER_LETZTEN_ANPASSUNG              0xF19B
#define ID_22_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG              0xF19E
#define ID_22_F1A0_VARIANTE_DES_PARAMETERSATZES              0xF1A0
#define ID_22_F1A1_VERSION_DES_PARAMETERSATZES              0xF1A1
#define ID_22_F1A2_ASAM_ODX_DATEIVERSION              0xF1A2
#define ID_22_F1A3_HARDWAREVERSION              0xF1A3
#define ID_22_F1A4_FAHRZEUGAUSSTATTUNGSCODE              0xF1A4
#define ID_22_F1A5_WERKSTATTCODE_DER_CODIERUNG              0xF1A5
#define ID_22_F1A8_WERKSTATTCODE_DER_PARAMETRIERUNG              0xF1A8
#define ID_22_F1A9_DATUM_DER_PARAMETRIERUNG              0xF1A9
#define ID_22_F1AA_SYSTEMKURZZEICHEN              0xF1AA
#define ID_22_F1AB_VERSIONEN_DER_SOFTWAREMODULE              0xF1AB
#define ID_22_F1AC_BAUZUSTANDSDOKUMENTATION              0xF1AC
#define ID_22_F1DF_STATUS_DER_PROGRAMMIERBARKEIT              0xF1DF
#define ID_22_F1AE_NUMBERS_OF_LOGINS                        0xF1AE
#define ID_22_F1E0_KONSISTENZ_DER_EEPROM_DATEN              0xF1E0

/*ActuatorTest*/
#define ID_2F_016B_TESTING_SIGNAL_AUDIO              0x016B
#define ID_2F_016C_AUDIO_MUTE_BY_WIRE              0x016C
#define ID_2F_016D_AUDIO_MUTE_BY_CAN              0x016D
#define ID_2F_0170_TEST_VOICE_PROMPT              0x0170
#define ID_2F_0171_ILLUMINATION_TELEPHONE_OPERATING_UNIT              0x0171
#define ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING              0x0172
#define ID_2F_02E1_AUDIO_LOOP_BACK              0x02E1
#define ID_2F_0410_POWER_SUPPLY_CRADLE_ON_OFF_TEST              0x0410
#define ID_2F_0411_ILLUMINATION_TELEPHONE_OPERATION_UNIT_CONTROL_TEST              0x0411
#define ID_2F_0412_BLUETOOTH_AUDIO_PATH_LOOPBACK_CONTROL_TEST              0x0412
#define CASE_ALL_ID_STELLGLIEDTEST       \
					 case ID_2F_016B_TESTING_SIGNAL_AUDIO: \
					 case ID_2F_016C_AUDIO_MUTE_BY_WIRE: \
					 case ID_2F_016D_AUDIO_MUTE_BY_CAN: \
					 case ID_2F_016E_BLUETOOTH_SEARCH_DEVICES: \
					 case ID_2F_016F_BLUETOOTH_PAGING: \
					 case ID_2F_0170_TEST_VOICE_PROMPT: \
					 case ID_2F_0171_ILLUMINATION_TELEPHONE_OPERATING_UNIT: \
					 case ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING: \
					 case ID_2F_02E1_AUDIO_LOOP_BACK: \
					 case ID_2F_0410_POWER_SUPPLY_CRADLE_ON_OFF_TEST: \
					 case ID_2F_0411_ILLUMINATION_TELEPHONE_OPERATION_UNIT_CONTROL_TEST: \
					 case ID_2F_0412_BLUETOOTH_AUDIO_PATH_LOOPBACK_CONTROL_TEST: \
/*DtcOverview*/
#define ID_DTCS_B104811_MIKROFON_FUER_TELEFON_KURZSCHLUSS_NACH_MASSE              0x04811
#define ID_DTCS_B104812_MIKROFON_FUER_TELEFON_KURZSCHLUSS_NACH_PLUS              0x04812
#define ID_DTCS_B104813_MIKROFON_FUER_TELEFON_UNTERBRECHUNG              0x04813
#define ID_DTCS_B104911_STUMMSCHALTUNG_RADIO_KURZSCHLUSS_NACH_MASSE              0x04911
#define ID_DTCS_B104912_STUMMSCHALTUNG_RADIO_KURZSCHLUSS_NACH_PLUS              0x04912
#define ID_DTCS_B104913_STUMMSCHALTUNG_RADIO_UNTERBRECHUNG              0x04913
#define ID_DTCS_B104E11_AUDIOKANAL_RECHTS_KURZSCHLUSS_NACH_MASSE              0x04E11
#define ID_DTCS_B104E12_AUDIOKANAL_RECHTS_KURZSCHLUSS_NACH_PLUS              0x04E12
#define ID_DTCS_B104E13_AUDIOKANAL_RECHTS_UNTERBRECHUNG              0x04E13
#define ID_DTCS_B104EF0_AUDIOKANAL_RECHTS_KURZSCHLUSS_UNTEREINANDER              0x04EF0
#define ID_DTCS_B104F11_AUDIOKANAL_LINKS_KURZSCHLUSS_NACH_MASSE              0x04F11
#define ID_DTCS_B104F12_AUDIOKANAL_LINKS_KURZSCHLUSS_NACH_PLUS              0x04F12
#define ID_DTCS_B104F13_AUDIOKANAL_LINKS_UNTERBRECHUNG              0x04F13
#define ID_DTCS_B104FF0_AUDIOKANAL_LINKS_KURZSCHLUSS_UNTEREINANDER              0x04FF0
#define ID_DTCS_B105000_FUNKTIONSEINSCHRAENKUNG_DURCH_UEBERTEMPERATUR              0x05000
#define ID_DTCS_B105111_HALTERUNG_FUER_HANDYHALTER__BASEPLATE__KURZSCHLUSS_NACH_MASSE              0x05111
#define ID_DTCS_B105112_HALTERUNG_FUER_HANDYHALTER__BASEPLATE__KURZSCHLUSS_NACH_PLUS              0x05112
#define ID_DTCS_B105113_HALTERUNG_FUER_HANDYHALTER__BASEPLATE__UNTERBRECHUNG              0x05113
#define ID_DTCS_B105207_BEDIENEINHEIT_FUER_HANDYVORBEREITUNG__DREI_TASTEN_MODULE__MECHANISCHER_FEH__LER              0x05207

#define ID_DTCS_B105211_BEDIENEINHEIT_FUER_HANDYVORBEREITUNG__DREI_TASTEN_MODULE__KURZSCHLUSS_NACH_MASSE              0x05211

#define ID_DTCS_B105212_BEDIENEINHEIT_FUER_HANDYVORBEREITUNG__DREI_TASTEN_MODULE__KURZSCHLUSS_NACH_PLUS              0x05212

#define ID_DTCS_B105213_BEDIENEINHEIT_FUER_HANDYVORBEREITUNG__DREI_TASTEN_MODULE__UNTERBRECHUNG              0x05213
#define ID_DTCS_B105311_GSM_ANTENNE_KURZSCHLUSS_NACH_MASSE              0x05311
#define ID_DTCS_B105315_GSM_ANTENNE_UNTERBRECHUNG_KURZSCHLUSS_NACH_PLUS              0x05315
#define ID_DTCS_B105707_HANDYHALTER__CRADLE__MECHANISCHER_FEHLER              0x05707
#define ID_DTCS_B200045_STEUERGERAET_DEFEKT              0x00045
#define ID_DTCS_B200046_STEUERGERAET_DEFEKT_EEPROM_FEHLER              0x00046
#define ID_DTCS_B200047_STEUERGERAET_DEFEKT              0x00047
#define ID_DTCS_B200500_UNGUELTIGER_DATENSATZ              0x00500
#define ID_DTCS_U001000_DATENBUS_KOMFORT_DEFEKT              0x01000
#define ID_DTCS_U001100_DATENBUS_KOMFORT_KEINE_KOMMUNIKATION              0x01100
#define ID_DTCS_U006400_INFOTAINMENT_CAN_DEFEKT              0x06400
#define ID_DTCS_U006500_INFOTAINMENT_CAN_KEINE_KOMMUNIKATION              0x06500
#define ID_DTCS_U014000_BODYCOMPUTER_1__BORDNETZSTEUERGERAET___EL              0x14000
#define ID_DTCS_U014600_DIAGNOSE_INTERFACE_SG_KEINE_KOMMUNIKATION              0x14600
#define ID_DTCS_U015500_STEUERGERAET_FUER_SCHALTTAFELEINSATZ_KEINE_KOMMUNIKATION              0x15500
#define ID_DTCS_U021200_STEUERGERAET_FUER_LENKSAEULENELEKTRONIK_KEINE_KOMMUNIKATION              0x21200
#define ID_DTCS_U101100_VERSORGUNGSSPANNUNG_SPANNUNG_ZU_NIEDRIG              0x01100
#define ID_DTCS_U101200_VERSORGUNGSSPANNUNG_SPANNUNG_ZU_HOCH              0x01200
#define ID_DTCS_U101300_STEUERGERAET_NICHT_CODIERT              0x01300
#define ID_DTCS_U101400_STEUERGERAET_FALSCH_CODIERT              0x01400
#define ID_DTCS_U106600_STEUERGERAET_RADIO__RNS__KEIN_SIGNAL_KOMMUNIKATION              0x06600
#define ID_DTCS_U140000_FUNKTIONSEINSCHRAENKUNG_DURCH_UNTERSPANNUNG              0x40000
#define ID_DTCS_U140100_FUNKTIONSEINSCHRAENKUNG_DURCH_UEBERSPANNUNG              0x40100
/*CalibrationData*/
#define ID_22_0500_ANALYSE_1              0x0500
#define ID_22_0905_RESET_TO_FACTORY_SETTING              0x0905
#define ID_22_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL              0x0909
#define ID_22_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL              0x090A
#define ID_22_090B_SET_NUMBER_FOR_INFO_CALL              0x090B
#define ID_22_090C_SET_NUMBER_FOR_SERVICE_CALL              0x090C
#define ID_22_0911_SMS_FUNCTIONALITY_OPERATING_UNIT              0x0911

#define ID_22_0A07_CDMA_IMSI_ON_OFF              0x0A07
#define ID_22_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL              0x0914
#define ID_22_1001_DEVELOPER_TESTMODE              0x1001
#define ID_22_2233_MASKING_OF_LANGUAGES              0x2233
#define ID_22_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE              0x2416
#define ID_22_241C_TELEPHONE_BASIC_VOLUME              0x241C
#define ID_22_241D_VOICE_OUTPUT_BASIC_VOLUME              0x241D
#define ID_22_241E_MICROPHONE_SENSITIVITY              0x241E
#define ID_22_2420_BLUETOOTH_SET_PIN_CODE              0x2420
#define ID_22_2422_USER_PROFILES              0x2422
#define ID_22_2423_MUTE_DELAY              0x2423
#define ID_22_2424_REFERENCE_CHANNEL_DELAY              0x2424
#define ID_22_2429_MEDIA_DEVICE_BASIC_VOLUME              0x2429
#define ID_22_2448_EMERGENCY_NUMBER              0x2448
/*IdentifierMeasurementValue*/
#define ID_22_0286_SPANNUNG_KLEMME_30              0x0286
#define ID_22_028D_STEUERGERAETETEMPERATUR              0x028D
#define ID_22_0300_SOFTWAREVERSION              0x0300
#define ID_22_1100_BLUETOOTH_DEVICE_MAC_ADDRESS              0x1100
#define ID_22_2400_MICROPHONE_STATUS              0x2400
#define ID_22_2401_MICROPHONE_CURRENT_DRAIN              0x2401
#define ID_22_2402_TELEPHONE_SIGNAL_STRENGTH              0x2402
#define ID_22_2403_AUDIO_OUTPUT_LEFT              0x2403
#define ID_22_2404_AUDIO_OUTPUT_RIGHT              0x2404
#define ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP              0x2408
#define ID_22_240A_BLUETOOTH_CONNECTED_HEADSET              0x240A
#define ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP              0x240B
#define ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES              0x240D
#define ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE              0x240F
#define ID_22_2411_BLUETOOTH_PAIRED_HEADSETS              0x2411
#define ID_22_2417_AERIAL_CONNECTION_STATUS              0x2417
#define ID_22_2418_MEDIA_DEVICE_CONNECTION_STATUS              0x2418
#define ID_22_2428_BLUETOOTH_VISIBILITY              0x2428
#define ID_22_2435_TELEPHONE_CONNECTIVITY              0x2435
#define ID_22_245E_MICROPHONE_MUTE_BUTTON_SERVICE_CALL_BUTTON              0x245E
#define ID_22_245F_BLUETOOTH_BUTTON_INFO_CALL_BUTTON              0x245F
#define ID_22_2460_ANSWER_TELEPHONE_BUTTON_VOICE_CONTROL_BUTTON              0x2460
#define ID_22_2466_SERVICE_CALL_BUTTON_EMERGENCY_CALL_BUTTON_CRADLE              0x2466
#define ID_22_2467_INFO_CALL_BUTTON_VOICE_CONTROL_BUTTON_CRADLE              0x2467
#define CASE_ALL_ID_MESSWERT       \
					 case ID_22_0286_SPANNUNG_KLEMME_30: \
					 case ID_22_028D_STEUERGERAETETEMPERATUR: \
					 case ID_22_0300_SOFTWAREVERSION: \
					 case ID_22_1100_BLUETOOTH_DEVICE_MAC_ADDRESS: \
					 case ID_22_2400_MICROPHONE_STATUS: \
					 case ID_22_2401_MICROPHONE_CURRENT_DRAIN: \
					 case ID_22_2402_TELEPHONE_SIGNAL_STRENGTH: \
					 case ID_22_2403_AUDIO_OUTPUT_LEFT: \
					 case ID_22_2404_AUDIO_OUTPUT_RIGHT: \
					 case ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP: \
					 case ID_22_240A_BLUETOOTH_CONNECTED_HEADSET: \
					 case ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP: \
					 case ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES: \
					 case ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE: \
					 case ID_22_2411_BLUETOOTH_PAIRED_HEADSETS: \
					 case ID_22_2417_AERIAL_CONNECTION_STATUS: \
					 case ID_22_2418_MEDIA_DEVICE_CONNECTION_STATUS: \
					 case ID_22_2428_BLUETOOTH_VISIBILITY: \
					 case ID_22_2435_TELEPHONE_CONNECTIVITY: \
					 case ID_22_245E_MICROPHONE_MUTE_BUTTON_SERVICE_CALL_BUTTON: \
					 case ID_22_245F_BLUETOOTH_BUTTON_INFO_CALL_BUTTON: \
					 case ID_22_2460_ANSWER_TELEPHONE_BUTTON_VOICE_CONTROL_BUTTON: \
					 case ID_22_2466_SERVICE_CALL_BUTTON_EMERGENCY_CALL_BUTTON_CRADLE: \
					 case ID_22_2467_INFO_CALL_BUTTON_VOICE_CONTROL_BUTTON_CRADLE: \
/*IdentifierVariantCoding*/
#define ID_22_0600_CODIERUNG              0x0600
#define ID_22_0601_LAENGE_DER_CODIERUNG              0x0601
/*EcuIdentificationWritable*/
#define ID_2E_F15A_FINGERPRINT              0xF15A
#define ID_2E_F197_SYSTEMBEZEICHNUNG              0xF197
#define ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS              0xF198
#define ID_2E_F199_PROGRAMMING_DATE              0xF199
#define ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG              0xF19E
#define ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES              0xF1A0
#define ID_2E_F1A1_VERSION_DES_PARAMETERSATZES              0xF1A1
#define ID_2E_F1A2_ASAM_ODX_DATEIVERSION              0xF1A2
#define ID_2E_F1A3_HARDWAREVERSION              0xF1A3
#define ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE              0xF1A4
#define ID_2E_F1AC_BAUZUSTANDSDOKUMENTATION              0xF1AC
#define CASE_ALL_ID_ECU_IDENTIFICATION_WRITEABLE       \
					 case ID_2E_F197_SYSTEMBEZEICHNUNG: \
					 case ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG: \
					 case ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES: \
					 case ID_2E_F1A1_VERSION_DES_PARAMETERSATZES: \
					 case ID_2E_F1A2_ASAM_ODX_DATEIVERSION: \
					 case ID_2E_F1A3_HARDWAREVERSION: \
					 case ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE: \
					 case ID_2E_F1AC_BAUZUSTANDSDOKUMENTATION: \

					 
					 
/*CodierungWritable*/
#define ID_2E_0600_CODIERUNG              0x0600
#define CASE_ALL_ID_CODIERUNG       \
					 case ID_2E_0600_CODIERUNG: \

#endif /* _APP_DIAG_ID_CHART_H */

/* End Of File APP_DIAG_ID_CHART.h */

