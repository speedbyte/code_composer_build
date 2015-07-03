/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_Service22_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-08-16 13:44:23 +0200 (Di, 16 Aug 2011) $
* $Rev: 21241 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service22_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "tms470r1.h"
#include <string.h>
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "sds.h"
#include "uds.h"
#include "tp.h"
#include "deh.h"
#include "VCR.h"
#include "sds_ext.h"
#include "ISRMask_CB_cfg.h"
#include "APP_DIAG.h"
#include "can_def.h"
#include "can_cfg.h"
#include "crcmgr.h"
#include "deh_dtc_cfg.h"
#include "sds_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_Service22_C1.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDSext_ENABLED )

/************** A T T E N T I O N *****************************
   because of speed optimization binary search algorithm is used
   which requires a sorted (ascending !!!) data identifier table */

const t_ReadDataIdCfgItem cs_ReadDataIdItem[] =
{{ 0x0100, SDSext_USER, 1, (void*)0 },
{ 0x0102, SDSext_USER, 1, (void*)0 },
{ 0x0103, SDSext_USER, 1, (void*)0 },
{ ID_22_0286_SPANNUNG_KLEMME_30, SDSext_USER, ID_22_0286_SPANNUNG_KLEMME_30_MAX_LEN, (void*)0 },
{ ID_22_028D_STEUERGERAETETEMPERATUR, SDSext_USER, ID_22_028D_STEUERGERAETETEMPERATUR_MAX_LEN, (void*)0 },
{ ID_22_0300_SOFTWAREVERSION, SDSext_USER, ID_22_0300_SOFTWAREVERSION_MAX_LEN, (void*)0 },
{ ID_22_0405_STATUS_DES_PROGRAMMSPEICHERS, SDSext_EEPROM, ID_22_0405_STATUS_DES_PROGRAMMSPEICHERS_MAX_LEN, (void*) VWDB_EE_ADDDRESS_StateOfFlash },
{ ID_22_0407_ANZAHL_DER_PROGRAMMIERVERSUCHE, SDSext_EEPROM, ID_22_0407_ANZAHL_DER_PROGRAMMIERVERSUCHE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_BootSWCntProgrAttempts },
{ ID_22_0408_ANZAHL_ERFOLGREICHER_PROGRAMMIERVERSUCHE, SDSext_EEPROM, ID_22_0408_ANZAHL_ERFOLGREICHER_PROGRAMMIERVERSUCHE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_BootSWSucProgrAttempts },
{ ID_22_0409_ANZAHL_DER_PARAMETRIERVERSUCHE, SDSext_EEPROM, ID_22_0409_ANZAHL_DER_PARAMETRIERVERSUCHE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWDataCounterOfProgrammingAttempts },
{ ID_22_040A_ANZAHL_ERFOLGREICHER_PARAMETRIERVERSUCHE, SDSext_EEPROM, ID_22_040A_ANZAHL_ERFOLGREICHER_PARAMETRIERVERSUCHE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWDataCounterOfSuccessfulProgrammingAttempts },
{ ID_22_040F_VW_LOGICAL_SOFTWARE_BLOCK_LOCK_VALUE, SDSext_EEPROM, ID_22_040F_VW_LOGICAL_SOFTWARE_BLOCK_LOCK_VALUE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_BootSWMaxProgrAttempts },
{ ID_22_0500_ANALYSE_1, SDSext_EEPROM, ID_22_0500_ANALYSE_1_MAX_LEN, (void*) VWDB_EE_ADDDRESS_DTC_Maskierung },
{ ID_22_0600_CODIERUNG, SDSext_EEPROM, ID_22_0600_CODIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWCodingValue },
{ ID_22_0601_LAENGE_DER_CODIERUNG, SDSext_EEPROM, ID_22_0601_LAENGE_DER_CODIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWTesterCodingInformation },
{ ID_22_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL,  SDSext_EEPROM, ID_22_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL_MAX_LEN, (void*) VWDB_EE_ADDDRESS_SetNumberRoamingForInfoCall },
{ ID_22_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL, SDSext_EEPROM, ID_22_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL_MAX_LEN, (void*) VWDB_EE_ADDDRESS_SetNumberRoamingForServiceCall },
{ ID_22_090B_SET_NUMBER_FOR_INFO_CALL, SDSext_EEPROM, ID_22_090B_SET_NUMBER_FOR_INFO_CALL_MAX_LEN, (void*) VWDB_EE_ADDDRESS_SetNumberForInfoCall },
{ ID_22_090C_SET_NUMBER_FOR_SERVICE_CALL, SDSext_EEPROM, ID_22_090C_SET_NUMBER_FOR_SERVICE_CALL_MAX_LEN, (void*) VWDB_EE_ADDDRESS_SetNumberForServiceCall },
#ifdef UHV_ACTIVATE_FEATURE_SMS
{ ID_22_0911_SMS_FUNCTIONALITY_OPERATING_UNIT,  SDSext_EEPROM, ID_22_0911_SMS_FUNCTIONALITY_OPERATING_UNIT_MAX_LEN, (void*) VWDB_EE_ADDDRESS_SMSWriting },
#endif
{ ID_22_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL, SDSext_EEPROM, ID_22_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL_MAX_LEN, (void*) VWDB_EE_ADDDRESS_BluetoothAcknowledgementSignal },
{ ID_22_0A07_CDMA_IMSI_ON_OFF,  SDSext_EEPROM, VWDB_EE_ADDDRESS_CDMA_Detection_LEN, (void*) VWDB_EE_ADDDRESS_CDMA_Detection },
{ ID_22_1001_DEVELOPER_TESTMODE, SDSext_EEPROM, ID_22_1001_DEVELOPER_TESTMODE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_DeveloperTestmode },
{ ID_22_1100_BLUETOOTH_DEVICE_MAC_ADDRESS, SDSext_USER, ID_22_1100_BLUETOOTH_DEVICE_MAC_ADDRESS_MAX_LEN, (void*)0 },
{ ID_22_2233_MASKING_OF_LANGUAGES, SDSext_EEPROM, ID_22_2233_MASKING_OF_LANGUAGES_MAX_LEN, (void*) VWDB_EE_ADDDRESS_MaskingOfLanguages },
{ ID_22_2400_MICROPHONE_STATUS, SDSext_USER, ID_22_2400_MICROPHONE_STATUS_MAX_LEN, (void*)0 },
{ ID_22_2401_MICROPHONE_CURRENT_DRAIN, SDSext_USER, ID_22_2401_MICROPHONE_CURRENT_DRAIN_MAX_LEN, (void*)0 },
{ ID_22_2402_TELEPHONE_SIGNAL_STRENGTH, SDSext_USER, ID_22_2402_TELEPHONE_SIGNAL_STRENGTH_MAX_LEN, (void*)0 },
{ ID_22_2403_AUDIO_OUTPUT_LEFT, SDSext_USER, ID_22_2403_AUDIO_OUTPUT_LEFT_MAX_LEN, (void*)0 },
{ ID_22_2404_AUDIO_OUTPUT_RIGHT, SDSext_USER, ID_22_2404_AUDIO_OUTPUT_RIGHT_MAX_LEN, (void*)0 },
{ ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP, SDSext_USER, ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP_MAX_LEN, (void*)0 },
{ ID_22_240A_BLUETOOTH_CONNECTED_HEADSET, SDSext_USER, ID_22_240A_BLUETOOTH_CONNECTED_HEADSET_MAX_LEN, (void*)0 },
{ ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP, SDSext_USER, ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP_MAX_LEN, (void*)0 },
{ ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES, SDSext_USER, ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES_MAX_LEN, (void*)0 },
{ ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE, SDSext_USER, ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE_MAX_LEN, (void*)0 },
{ ID_22_2411_BLUETOOTH_PAIRED_HEADSETS, SDSext_USER, ID_22_2411_BLUETOOTH_PAIRED_HEADSETS_MAX_LEN, (void*)0 },
{ ID_22_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE, SDSext_EEPROM, ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_UniversalPreparationForMobileTelephone },
{ ID_22_2417_AERIAL_CONNECTION_STATUS, SDSext_USER, ID_22_2417_AERIAL_CONNECTION_STATUS_MAX_LEN, (void*)0 },
{ ID_22_2418_MEDIA_DEVICE_CONNECTION_STATUS, SDSext_USER, ID_22_2418_MEDIA_DEVICE_CONNECTION_STATUS_MAX_LEN, (void*)0 },
{ ID_22_241C_TELEPHONE_BASIC_VOLUME, SDSext_EEPROM, ID_2E_241C_TELEPHONE_BASIC_VOLUME_MAX_LEN, (void*) VWDB_EE_ADDDRESS_TelephoneBasicVolume },
{ ID_22_241D_VOICE_OUTPUT_BASIC_VOLUME, SDSext_EEPROM, ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VoiceOutputBasicVolume },
{ ID_22_241E_MICROPHONE_SENSITIVITY, SDSext_EEPROM, ID_2E_241E_MICROPHONE_SENSITIVITY_MAX_LEN, (void*) VWDB_EE_ADDDRESS_MicrophoneSensitivity },
{ ID_22_2420_BLUETOOTH_SET_PIN_CODE, SDSext_EEPROM, ID_2E_2420_BLUETOOTH_SET_PIN_CODE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_BluetoothSetPINCode },
{ ID_22_2422_USER_PROFILES, SDSext_USER, ID_2E_2422_USER_PROFILES_MAX_LEN, (void*)0 },
{ ID_22_2423_MUTE_DELAY, SDSext_EEPROM, ID_2E_2423_MUTE_DELAY_MAX_LEN, (void*) VWDB_EE_ADDDRESS_MuteDelay },
{ ID_22_2424_REFERENCE_CHANNEL_DELAY, SDSext_EEPROM, ID_2E_2424_REFERENCE_CHANNEL_DELAY_MAX_LEN, (void*) VWDB_EE_ADDDRESS_Reference_Channel_Delay },
{ ID_22_2428_BLUETOOTH_VISIBILITY, SDSext_USER, ID_22_2428_BLUETOOTH_VISIBILITY_MAX_LEN, (void*)0 },
{ ID_22_2429_MEDIA_DEVICE_BASIC_VOLUME, SDSext_EEPROM, ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME_MAX_LEN, (void*) VWDB_EE_ADDDRESS_MediaDeviceBasicVolume },
{ ID_22_2435_TELEPHONE_CONNECTIVITY, SDSext_USER, ID_22_2435_TELEPHONE_CONNECTIVITY_MAX_LEN, (void*)0 },
{ ID_22_2448_EMERGENCY_NUMBER, SDSext_EEPROM, ID_2E_2448_EMERGENCY_NUMBER_MAX_LEN, (void*) VWDB_EE_ADDDRESS_EmergencyNumber },
{ ID_22_245E_MICROPHONE_MUTE_BUTTON_SERVICE_CALL_BUTTON, SDSext_USER, ID_22_245E_MICROPHONE_MUTE_BUTTON_SERVICE_CALL_BUTTON_MAX_LEN, (void*)0 },
{ ID_22_245F_BLUETOOTH_BUTTON_INFO_CALL_BUTTON, SDSext_USER, ID_22_245F_BLUETOOTH_BUTTON_INFO_CALL_BUTTON_MAX_LEN, (void*)0 },
{ ID_22_2460_ANSWER_TELEPHONE_BUTTON_VOICE_CONTROL_BUTTON, SDSext_USER, ID_22_2460_ANSWER_TELEPHONE_BUTTON_VOICE_CONTROL_BUTTON_MAX_LEN, (void*)0 },
{ ID_22_2466_SERVICE_CALL_BUTTON_EMERGENCY_CALL_BUTTON_CRADLE, SDSext_USER, ID_22_2466_SERVICE_CALL_BUTTON_EMERGENCY_CALL_BUTTON_CRADLE_MAX_LEN, (void*)0 },
{ ID_22_2467_INFO_CALL_BUTTON_VOICE_CONTROL_BUTTON_CRADLE, SDSext_USER, ID_22_2467_INFO_CALL_BUTTON_VOICE_CONTROL_BUTTON_CRADLE_MAX_LEN, (void*)0 },
{ ID_22_F15B_FINGERPRINT, SDSext_SPECIAL_EEPROM, ID_22_F15B_FINGERPRINT_MAX_LEN, (void*) VWDB_EE_ADDDRESS_RESERVED_01 },
{ ID_22_F17B_DATUM_DER_LETZTEN_CODIERUNG, SDSext_EEPROM, ID_22_F17B_DATUM_DER_LETZTEN_CODIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWCodingDate },
{ ID_22_F17C_FAZIT_SERIENNUMMER, SDSext_EEPROM, ID_22_F17C_FAZIT_SERIENNUMMER_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWFAZITIdentificationString },
{ ID_22_F17E_HERSTELLERAENDERUNGSSTAND, SDSext_EEPROM, ID_22_F17E_HERSTELLERAENDERUNGSSTAND_MAX_LEN, (void*) VWDB_EE_ADDDRESS_ECUProductionChangeNumber },
{ ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE, SDSext_USER, ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE_MAX_LEN, (void*)0 },
{ ID_22_F182_IDENTIFIKATION_DER_PARAMETRIERUNG,  SDSext_ASCII, ID_22_F182_IDENTIFIKATION_DER_PARAMETRIERUNG_MAX_LEN, (void*) "101" },
{ 0xF186, SDSext_USER, 1, (void*)0 },
{ ID_22_F187_VW_AUDI_TEILENUMMER,  SDSext_EEPROM, ID_22_F187_VW_AUDI_TEILENUMMER_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWSparePartNbr },
{ ID_22_F189_SOFTWAREVERSION,  SDSext_ASCII, ID_22_F189_SOFTWAREVERSION_MAX_LEN, (void*) SWVersion },
{ ID_22_F18C_SERIENNUMMER,  SDSext_EEPROM, ID_22_F18C_SERIENNUMMER_MAX_LEN, (void*) VWDB_EE_ADDDRESS_ECUSerialNumber },
{ ID_22_F191_HARDWARETEILENUMMER,  SDSext_EEPROM, ID_22_F191_HARDWARETEILENUMMER_MAX_LEN, (void*) VWDB_EE_ADDDRESS_ECUHardwareNbr },
{ ID_22_F197_SYSTEMBEZEICHNUNG,  SDSext_EEPROM, ID_22_F197_SYSTEMBEZEICHNUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_SysNameEngineType },
{ ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS, SDSext_USER,  ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MAX_LEN, (void*) 0 },
{ ID_22_F199_DATUM_DER_LETZTEN_UPDATE_PROGRAMMIERUNG,  SDSext_EEPROM, ID_22_F199_DATUM_DER_LETZTEN_UPDATE_PROGRAMMIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_AswSWProgDate },
{ ID_22_F19A_WERKSTATTCODE_DER_ANPASSUNG,  SDSext_EEPROM, ID_22_F19A_WERKSTATTCODE_DER_ANPASSUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWCalibrationRepairShopCodeOrSerialNumber },
{ ID_22_F19B_DATUM_DER_LETZTEN_ANPASSUNG,  SDSext_EEPROM, ID_22_F19B_DATUM_DER_LETZTEN_ANPASSUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWCalibrationDate },
{ ID_22_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG,  SDSext_EEPROM, ID_22_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_AsamOdxFileID },
{ ID_22_F1A0_VARIANTE_DES_PARAMETERSATZES,  SDSext_EEPROM, ID_22_F1A0_VARIANTE_DES_PARAMETERSATZES_MAX_LEN, (void*) VWDB_EE_ADDDRESS_DataSetNbr },
{ ID_22_F1A1_VERSION_DES_PARAMETERSATZES,  SDSext_EEPROM, ID_22_F1A1_VERSION_DES_PARAMETERSATZES_MAX_LEN, (byte*) VWDB_EE_ADDDRESS_DataVersionNbr },
{ ID_22_F1A2_ASAM_ODX_DATEIVERSION,  SDSext_EEPROM, ID_22_F1A2_ASAM_ODX_DATEIVERSION_MAX_LEN, (void*) VWDB_EE_ADDDRESS_AsamOdxFileVersion },
{ ID_22_F1A3_HARDWAREVERSION,  SDSext_EEPROM, ID_22_F1A3_HARDWAREVERSION_MAX_LEN, (void*) VWDB_EE_ADDDRESS_ECUHardwareVersionNbr },
{ ID_22_F1A4_FAHRZEUGAUSSTATTUNGSCODE,  SDSext_EEPROM, ID_22_F1A4_FAHRZEUGAUSSTATTUNGSCODE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VehEquipCodePRNbr },
{ ID_22_F1A5_WERKSTATTCODE_DER_CODIERUNG,  SDSext_EEPROM, ID_22_F1A5_WERKSTATTCODE_DER_CODIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWCodingRepairShopCodeOrSerialNumber },
{ ID_22_F1A8_WERKSTATTCODE_DER_PARAMETRIERUNG,  SDSext_EEPROM, ID_22_F1A8_WERKSTATTCODE_DER_PARAMETRIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWDataSetRepairShopCodeOrSerialNumber },
{ ID_22_F1A9_DATUM_DER_PARAMETRIERUNG,  SDSext_EEPROM, ID_22_F1A9_DATUM_DER_PARAMETRIERUNG_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWDataSetProgrammingDate },
{ ID_22_F1AA_SYSTEMKURZZEICHEN,  SDSext_EEPROM, ID_22_F1AA_SYSTEMKURZZEICHEN_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWWorkshopSystemName },
{ ID_22_F1AB_VERSIONEN_DER_SOFTWAREMODULE,  SDSext_EEPROM, ID_22_F1AB_VERSIONEN_DER_SOFTWAREMODULE_MAX_LEN, (void*) VWDB_EE_ADDDRESS_BootSWLogicalBlockVersion },
{ ID_22_F1AC_BAUZUSTANDSDOKUMENTATION,  SDSext_EEPROM, ID_22_F1AC_BAUZUSTANDSDOKUMENTATION_MAX_LEN, (void*) VWDB_EE_ADDDRESS_VWEOLConfiguration },
{ ID_22_F1AE_NUMBERS_OF_LOGINS,  SDSext_USER, ID_22_F1AE_NUMBERS_OF_LOGINS_MAX_LEN, (void*)0 },
{ ID_22_F1DF_STATUS_DER_PROGRAMMIERBARKEIT,  SDSext_EEPROM, ID_22_F1DF_STATUS_DER_PROGRAMMIERBARKEIT_MAX_LEN, (void*) VWDB_EE_ADDDRESS_ECUProgramInfo },
{ ID_22_F1E0_KONSISTENZ_DER_EEPROM_DATEN,  SDSext_EEPROM, ID_22_F1E0_KONSISTENZ_DER_EEPROM_DATEN_MAX_LEN, (void*) VWDB_EE_ADDDRESS_ECUDataProgrammingInformation }
};

#endif

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static Uint8Type SDSext_u8DeterminePhoneNumberLength(const Uint8Type *data,
		Uint16Type length);

static Uint8Type SDSext_ubFindVersionModuleID(Uint8Type VersionModuleID);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService3D
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/

#ifdef SDSEXT_SERVICE3D_ALLOWED
#define Service3DAddressAndLengthFormatID                RxTxBuf[0]
#define Service3DMemoryAddress                           ((RxTxBuf[1] << 8) + RxTxBuf[2])
#define Service3DMemorySize                              RxTxBuf[3]
#define Service3DDataRecord                              RxTxBuf[4]
#define Service3DSupportedAddressAndLengthFormatID       0x12  // es wird nur ein Uint16Type für die Adresse und ein Uint8Type (d.h. max. 255 Zeichen) unterstützt !!!
#define MAX_EEPROM_WRITE_RETRIES_SERVICE_3D              1000 //used by TransferData, Nr of retries to write in EEPROM
#endif

/*
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_SUCCESSFUL                     '  DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_FAILED                         '  DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_NOT_ALLOWED                    '  DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_OUT_OF_RANGE                   '  DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR         '  DEBUG_STATE_NUMBER='4'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT '  DEBUG_STATE_NUMBER='5'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_TRY_AGAIN                      '  DEBUG_STATE_NUMBER='6'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_REPEAT_FOR_COMPLETE_E2P_WRITE        '  DEBUG_STATE_NUMBER='7'
*/

Uint8Type UDS_ubDiagService3D(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg, Uint8Type Mode)
{
#ifdef SDSEXT_SERVICE3D_ALLOWED
	EEDeviceControlBlockType *pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
	static tSDSextWrite_Return EEPretVal = SDS_IO_USER_WRITE_FAILED;
	static Uint16Type _SDS_UDS_u16EEpromWriting_3D_Control = 0;

	/* Security check and length and format check */
	if ((ubSecState & MASK_SEC_ACTIVE) != MASK_SEC_ACTIVE)
	{
		_SDS_UDS_u16EEpromWriting_3D_Control = 0;
		EEPretVal = SDS_IO_USER_WRITE_FAILED;
		return UDScub_ACCESS_DENIED;
	}
	if( *LenOfMsg < (Uint16Type)5 )
	{
		Mode = Mode; /* prevent compiler warnings */
		_SDS_UDS_u16EEpromWriting_3D_Control = 0;
		EEPretVal = SDS_IO_USER_WRITE_FAILED;
		return UDScub_INCORRECT_MSG_LEN;
	}
	else
	{
		if (Service3DAddressAndLengthFormatID != Service3DSupportedAddressAndLengthFormatID)
		{
			_SDS_UDS_u16EEpromWriting_3D_Control = 0;
			EEPretVal = SDS_IO_USER_WRITE_FAILED;
			return UDScub_INCORRECT_MSG_LEN;
		}
		else
		{
			/* Start Write E2P */
			if (_SDS_UDS_u16EEpromWriting_3D_Control == 0)
			{
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service write E2P by address Started");
				EEPretVal = SDSext_EEPWriteUniversalData (Service3DMemoryAddress, Service3DMemorySize, &RxTxBuf[4]);
				_SDS_UDS_u16EEpromWriting_3D_Control = 1;
				DEBUG_STATE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "EEprom Write ReturnCode -> $tSDSextWrite_Return$=%d", EEPretVal );
			}
			else if (( EEPretVal == SDS_IO_USER_WRITE_TRY_AGAIN ) || ( EEPretVal == SDS_IO_USER_WRITE_FAILED))
			{
				/* retry attempt for write E2P */
				EEPretVal = SDSext_EEPWriteUniversalData (Service3DMemoryAddress, Service3DMemorySize, &RxTxBuf[4]);
				DEBUG_STATE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "EEprom Write ReturnCode -> $tSDSextWrite_Return$=%d", EEPretVal );
			}
			/* Check different return code cases off E2P write */
			if( EEPretVal == SDS_IO_USER_WRITE_SUCCESSFUL )
			{
                /* we have to guarantee that data have write complete to eeprom      */
				/* otherwise it is possible that next SDS request can overwrite data */
				if ( EEPROM_flagIsEEFree(pEEHost) )
				{
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service write E2P by address finish");
					_SDS_UDS_u16EEpromWriting_3D_Control = 0;
					return UDScub_FINISHED;
			    }
				else
				{
					/* Waiting that data is complete write to E2P */
					DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Still writing EE-Data. Try: %d", _SDS_UDS_u16EEpromWriting_3D_Control );
					if (MAX_EEPROM_WRITE_RETRIES_SERVICE_3D < _SDS_UDS_u16EEpromWriting_3D_Control)
					{
						_SDS_UDS_u16EEpromWriting_3D_Control = 0;
						DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "MAX EEPROM WRITE RETRIES reached. Stop.");
						return SDS_UDS_GENERAL_PROGRAMMING_FAILURE; // MAX_EEPROM_WRITE_RETRIES hat das Schreiben nicht geklappt => jetzt ist Schluss...
					}
					_SDS_UDS_u16EEpromWriting_3D_Control++;
					return UDScub_IN_PROCESS;
				}
			}
			/* In both cases we do a retry - both return codes describe a reason that E2P could not read a the moment but not why */
			/* Because of mixed return codes from lower level SW layers for E2P it necessary to do all things here                */
			else if (( EEPretVal == SDS_IO_USER_WRITE_TRY_AGAIN ) || ( EEPretVal == SDS_IO_USER_WRITE_FAILED))
			{
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Writing EE-Data. Try: %d", _SDS_UDS_u16EEpromWriting_3D_Control );
				if (MAX_EEPROM_WRITE_RETRIES_SERVICE_3D < _SDS_UDS_u16EEpromWriting_3D_Control)
				{
					_SDS_UDS_u16EEpromWriting_3D_Control = 0;
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "MAX EEPROM WRITE RETRIES reached. Stop.");
					return SDS_UDS_GENERAL_PROGRAMMING_FAILURE; // MAX_EEPROM_WRITE_RETRIES hat das Schreiben nicht geklappt => jetzt ist Schluss...
				}
				_SDS_UDS_u16EEpromWriting_3D_Control++;
				return UDScub_IN_PROCESS;
			}
			else
			{
				/* All other in future defined return codes !!! */
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Not implemented return code. Give Diag. error back");
				_SDS_UDS_u16EEpromWriting_3D_Control = 0;
				return SDS_UDS_GENERAL_PROGRAMMING_FAILURE;
			}
		}
	}
#else
	return UDScub_SUBFUNCTION_NOT_SUPPORTED;
#endif
}

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService86
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
Uint8Type UDS_ubDiagService86(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg,Uint8Type Mode)
{
#ifdef SDSEXT_SERVICE86_ALLOWED
	if( *LenOfMsg != (Uint16Type)6 )
	{
		Mode = Mode; /* prevent compiler warnings */
		return UDScub_INCORRECT_MSG_LEN;
	}

	switch (Service86EventTypeCAN)
	{
	case Service86EventType_StopResponseOnEvent:
		u8Service86DataValues[Service86EventType] = Service86EventType_StopResponseOnEvent;
		(void)EEPROM_sfRamWriteOnly(&u8Service86DataValues[0], EEPROM_LAYOUT_ENUM_Service86ResponseOnEvent);
		u8Service86DataValuesInitStatus = Service86inactive; // jetzt die Überwachung vin SDSext_vTask scharf schalten
		break;

	case Service86EventType_OnDTCStatusChange:
		if ((Service86EventWindowTimeCAN != Service86EventWindowTime_InfiniteTimeToResponse) && (Service86EventWindowTimeCAN != Service86EventWindowTime_ActualCycle))
			return UDScub_CONDITIONS_NOT_CORRECT;

		if ((Service86EventTypeRecordCAN != Service86EventTypeRecord_TestFailed) && (Service86EventTypeRecordCAN != Service86EventTypeRecord_ConfirmedDTC))
			return UDScub_CONDITIONS_NOT_CORRECT;

		// bis hier hin gekommen? => dann speichern
		u8Service86DataValues[Service86EventType] = Service86EventTypeCAN;
		u8Service86DataValues[Service86EventWindowTime] = Service86EventWindowTimeCAN;
		u8Service86DataValues[Service86EventTypeRecord] = Service86EventTypeRecordCAN;
		u8Service86DataValues[Service86ServiceToRespondToRecord] = Service86ServiceToRespondToRecordCAN;
		(void)EEPROM_sfRamWriteOnly(&u8Service86DataValues[0], EEPROM_LAYOUT_ENUM_Service86ResponseOnEvent);
		break;

	case Service86EventType_StartResponseOnEvent:
		if ((u8Service86DataValues[Service86ServiceToRespondToRecord] != Service86ServiceToRespondToRecord_ReportFirstTestFailed) && (u8Service86DataValues[Service86ServiceToRespondToRecord] != Service86ServiceToRespondToRecord_ReportFirstPending))
			return SDS_UDS_REQUEST_SEQUENCE_ERROR;
		u8Service86DataValues[Service86EventType] = Service86EventType_StartResponseOnEvent;
		(void)EEPROM_sfRamWriteOnly(&u8Service86DataValues[0], EEPROM_LAYOUT_ENUM_Service86ResponseOnEvent);
		u8Service86DataValuesInitStatus = Service86init; // jetzt die Überwachung via SDSext_vTask scharf schalten
		break;

	default:
		break;
	}

	RxTxBuf[2] = RxTxBuf[1]; // VW will es so
	RxTxBuf[1] = 0xFF;
	*LenOfMsg = 3;

	return UDScub_FINISHED;
#else
	return UDScub_SUBFUNCTION_NOT_SUPPORTED;
#endif
}

/*-----------------------------------------------------------------------*
 * function:     static void * sdsMemCpy( void * dest, const void * src , size_t count )
 * description   Function for memory copy
 * in:           -
 * out:          -
 * inout:        -
 * global_in:    -
 * global_out:   -
 * global_inout: -
 * return:       -
 *-----------------------------------------------------------------------*/
void * sdsMemCpy(void * dest, const void * src, size_t count)
{
	unsigned char *d = (unsigned char *) dest, *s = (unsigned char *) src;
	while (count !=0) {
		count--;
		(*d) = (*s);
		d++;
		s++;
	}
	return dest;
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_DIAG_vHandleEvents                                         		  */
/**
 \brief      Handle Events from App Diagnose.

 \brief      Description
 Handle events send by the Diagnose

 \return     SuccessFailType
 If the function was successful or failed
 SUCCESS, FAIL
 */
/*----------------------------------------------------------------------------*/
SuccessFailType SDSext_DIAG_vHandleEvents(EventMaskType event)
{
	static Uint16Type _SDS_UDS_u16BusyRetryCounter = 0;
	if (event & evHostTaskDiagnoseResponse)
	{
		(void)ClearEvent(evHostTaskDiagnoseResponse);
		// check for new message from Diagnose.
		(void)ReceiveMessage(msgAppDiagDiagnoseResponse,
				&_SDS_UDS_stResponsePacketToVas);


		if ( _SDS_UDS_stResponsePacketToVas.Identifier == ID_31_0317_RESET_TO_FACTORY_SETTING)
		{
			switch ( _SDS_UDS_stResponsePacketToVas.result )
			{
				case enAppDiagResponseWaitForCallBack:
				{
					_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_RUNNING;
					break;
				}
				case enAppDiagResponseErrorBusyImm:
				{
					if ( _SDS_UDS_u16BusyRetryCounter == 2000 ) // 20 seconds
					{
						_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_ABORTED;
						_SDS_UDS_u16TimerBasicSettings = 500;
						_SDS_UDS_u16BusyRetryCounter = 0;
						(void)CancelAlarm(CTA_HOST_R2FS_DIAG_RETRY);
					}
					else  // retry again
					{
						_SDS_UDS_u16BusyRetryCounter++;
						_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_RUNNING;
						(void)SetRelAlarm(CTA_HOST_R2FS_DIAG_RETRY, MSEC(10), MSEC(0));
					}
					break;
				}
				case enAppDiagResponseErrorImm:
				{
					_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_ABORTED;
					_SDS_UDS_u16TimerBasicSettings = 500;
					_SDS_UDS_u16BusyRetryCounter = 0;
					(void)CancelAlarm(CTA_HOST_R2FS_DIAG_RETRY);
					break;
				}
				case enAppDiagResponseOkImm:
				{
					_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_FINISHED;
					_SDS_UDS_u16TimerBasicSettings = 500;
					_SDS_UDS_u16BusyRetryCounter = 0;
					(void)CancelAlarm(CTA_HOST_R2FS_DIAG_RETRY);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		ubResponseFromAppTask = 1;
	}
	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_GetUhvInRestrictedMode                                         		  */
/**
 \brief      Handle the CRC restriction.

 \brief      Description
 Handle the CRC restriction

 \return     boolean
 If the function is allowed or not
 OFF, ON
 */
/*----------------------------------------------------------------------------*/
boolean SDSext_GetUhvInRestrictedMode(void)
{
	return ON;
}


/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vDiagTimeout                                         		  */
/**
 \brief      Description
 function to handling diagnosis timeout

 \return     void

 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vDiagTimeout(void)
{
	DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "diagnosis timeout or change diagnosis session");
	_SDS_UDS_u8UserProfile = DEFAULT_USER_PROFILES;
	_SDS_UDS_u8ResetToFactory = DEFAULT_RESET_TO_FACTORY_SETTING;
	SDS_UDS_vInitialiseService2F();
	DEH_SetDiagnosticSession(UDS_ubGetSession(UDS_CHANNEL_INDEX_2));// Inform DEH that the session has changed.
	if ((_SDS_UDS_bStopCommunicationMessages == BT_TRUE) && (UDS_ubGetSession(UDS_CHANNEL_INDEX_2) == UDScub_DEFAULT_SESSION))
	{
		(void)SDSext_ubComCtrl(SDS_28_EnableRxAndTx,SDS_28_NormalCommunicationMessagesOnly);
	}
}


ALARMCALLBACK(SDS_UDS_RetryMechanismR2FS)
{
	_SDS_UDS_stRequestPacketFromVas.Identifier= ID_31_0317_RESET_TO_FACTORY_SETTING;
	_SDS_UDS_stRequestPacketFromVas.length = 1;
	_SDS_UDS_stRequestPacketFromVas.data[0] = RESET_TO_FACTORY_SETTING_ON;
	(void)SendMessage(msgAppDiagDiagnoseRequest,
			&_SDS_UDS_stRequestPacketFromVas);
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_bResetOfAdaptionValuesStatus                                                 */
/**
 \brief      internal function to return the state of Basic settings"

 \param      None

 \return     _SDS_UDS_bResetOfAdaptionValuesState
 state of basic settings

 */
/*----------------------------------------------------------------------------*/
BooleanType _SDS_UDS_bResetOfAdaptionValuesStatus(void)
{
	return _SDS_UDS_bResetOfAdaptionValuesState;
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_vResetOfAdaptionValues                                                 */
/**
 \brief      internal function to control the functionality "reser to factory"

 \param      start
 start or stop function

 \param      BasicSetting
 BasicSetting (Werkseinstellung) or only $0905 (Anpassung)

 \return     None

 */
/*----------------------------------------------------------------------------*/
void _SDS_UDS_vResetOfAdaptionValues(BooleanType start,	BooleanType BasicSetting)
{
	if (start == BT_TRUE)
	{
		if (BasicSetting == BT_TRUE)
		{
			_SDS_UDS_bResetOfAdaptionValuesState = BT_TRUE;
			_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_RUNNING;

		}
		_SDS_UDS_stRequestPacketFromVas.Identifier
		= ID_31_0317_RESET_TO_FACTORY_SETTING;
		_SDS_UDS_stRequestPacketFromVas.length = 1;
		_SDS_UDS_stRequestPacketFromVas.data[0] = RESET_TO_FACTORY_SETTING_ON;
		(void)SendMessage(msgAppDiagDiagnoseRequest,
				&_SDS_UDS_stRequestPacketFromVas);
	}
	else
	{
		_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_NOT_RUNNING;
		_SDS_UDS_u16TimerBasicSettings = 0;
	}
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_u8GetStatusBasicSettings                                                 */
/**
 \brief      Get the status of Basic Settings in Routine Control

 \brief      Description:
 Get the status of Basic Settings in Routine Control

 \param      void

 \return     Uint8Type

 */
/*----------------------------------------------------------------------------*/

Uint8Type _SDS_UDS_u8GetStatusBasicSettings(void)
{
	return _SDS_UDS_u8Service31_BasicSettingsStatus;
}
/*----------------------------------------------------------------------------*/
/* Function    : SDSext_eReadUserAndIOData                                    */
/**
 \brief      Callback function for set interrupt bits

 \brief      Description:
 function to set bits for the diferent interrupts

 \param      data
 address of buffer

 \param      DataIdItem
 pointer to the structure with configuration data of Data-Identifier

 \return     SDSext_IO_USER_READ_SUCCESSFUL=0,
 SDSext_IO_USER_READ_FAILED,
 SDSext_IO_USER_READ_IN_PROGRESS
 */
/*----------------------------------------------------------------------------*/
tDEH_StdVersionInfo pVersionInfo;
I2S_VersionInfoType psDummyVersionInfo;
static Uint8Type logBlockCnt = 0;
tSDSextRead_Return SDSext_eReadUserAndIOData(Uint8Type* data,
		const t_ReadDataIdCfgItem* DataIdItem)
{
	int i;
	Uint8Type ubPosVersionModuleID;
	CommonResponseCodeType retVal_E2P = COMMON_RSP_OK;

	tSDSextRead_Return retVal = SDSext_IO_USER_READ_IN_PROGRESS;
	if (DataIdItem->extMem == SDSext_EEPROM)
	{
		EEDeviceControlBlockType *pEEDcb = (EEDeviceControlBlockType *)getEEHostControl();
		retVal_E2P = EEPROM_sfRead(pEEDcb, 
		                           (EEAddressType)DataIdItem->Address,
				                   (Uint8Type *)&data[0], 
				                   (Uint16Type)DataIdItem->Len);
		if ( retVal_E2P == COMMON_RSP_OK )
		{
			retVal = SDSext_IO_USER_READ_SUCCESSFUL;
			switch (DataIdItem->DataId)
			{
				case ID_22_0500_ANALYSE_1:
				{
					if ( ((UDScub_EXTENTED_DIAG_SESSION
							== UDS_ubGetSession(UDS_CHANNEL_INDEX_2))
							&& ((ubSecState & MASK_SEC_ACTIVE) == MASK_SEC_ACTIVE) )
							|| (UDScub_DEVELOPMENT_SESSION
									== UDS_ubGetSession(UDS_CHANNEL_INDEX_2))
									|| (UDScub_EOL_SESSION
											== UDS_ubGetSession(UDS_CHANNEL_INDEX_2)))
					{
						; //  Zugriff auf DTC Maskierung zulassen
					}
					else
					{
						return SDSext_IO_USER_READ_NOT_ALLOWED;
					}
					break;
				}
				case ID_22_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL:
				case ID_22_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL:
				case ID_22_090B_SET_NUMBER_FOR_INFO_CALL:
				case ID_22_090C_SET_NUMBER_FOR_SERVICE_CALL:
				case ID_22_2448_EMERGENCY_NUMBER:
				{
					u8PhoneNumberLength = SDSext_u8DeterminePhoneNumberLength(
							&data[0], DataIdItem->Len);
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
					// die Wertebereichskontrolle an dieser Stelle ist notwendig, damit keine ungültigen Werte an den
					// VAG Tester gesendet werden, weil dieser ansonsten einen Fehler anzeigt. Und es ist NICHT mehr möglich
					// den Wert mit dem Tester zu koorigieren. => Nur durch explizites Beschreiben der EEPROM Zelle kann
					// dieser Fehlerzustand behoben werden.

				case ID_22_241C_TELEPHONE_BASIC_VOLUME:
				{
					if (data[0] > AT_VOLUME_GEN_MAX)
                    {
						data[0] = AT_VOLUME_GEN_MAX;
                    }
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				case ID_22_241D_VOICE_OUTPUT_BASIC_VOLUME:
				{
					if (data[0] > AT_VOLUME_PROMPT_MAX)
                    {
						data[0] = AT_VOLUME_PROMPT_MAX;
                    }
					if (data[0] < AT_VOLUME_PROMPT_MIN)
                    {
                        data[0] = AT_VOLUME_PROMPT_MIN;
                    }
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				case ID_22_241E_MICROPHONE_SENSITIVITY:
				{
					if (data[0] > AT_MIC_GAIN_MAX)
                    {
						data[0] = AT_MIC_GAIN_MAX;
                    }
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				case ID_22_2429_MEDIA_DEVICE_BASIC_VOLUME:
				{
					if (data[0] > AT_VOLUME_STREAM_MAX)
                    {
						data[0] = AT_VOLUME_STREAM_MAX;
                    }
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				case ID_22_0601_LAENGE_DER_CODIERUNG:
				{
					// low nibble is length of coding and only this value is allowed to give back
					// high nibble is for coding write successful done not need here
					data[0] = data[0] & 0x0f;
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
			retVal = SDSext_IO_USER_READ_IN_PROGRESS;
			DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "E2P Retry");
		}
	}
	/* special data access type for diagnose id $F15B Fingerprint */
	else if (DataIdItem->extMem == SDSext_SPECIAL_EEPROM)
	{
		SuccessFailType sF = SUCCESS;
		EEDeviceControlBlockType *pEEDcb = (EEDeviceControlBlockType *)getEEHostControl();
		switch (logBlockState)
		{
		case SDS_UDS_PROG_DATE:
			/* check if it is the first duration */
			if (SDS_UDS_F15B_LOGICAL_BLOCK_MIN == logBlockCnt)
			{
				_SDS_UDS_stResponsePacketToVas.length = 0;
			}
			/* read programming date for the different logical blocks */
			retVal_E2P = EEPROM_sfRead(pEEDcb,
					(EEAddressType)(VWDB_EE_ADDDRESS_BootSWProgDate + (logBlockCnt*SDS_UDS_F15B_OFFSET_PROG_DATE)),
					&_SDS_UDS_stResponsePacketToVas.data[_SDS_UDS_stResponsePacketToVas.length],
					SDS_UDS_F15B_OFFSET_PROG_DATE);

			if ( retVal_E2P == COMMON_RSP_OK )
			{
				sF = SUCCESS;
			}
			else
			{
				sF = FAIL;
			}

			if (SUCCESS == sF)
			{
				/* increase state and add length */
				_SDS_UDS_stResponsePacketToVas.length += SDS_UDS_F15B_OFFSET_PROG_DATE;
				logBlockState = SDS_UDS_REPAIR_SHOP_CODE;
			}
			else
			{
				/* busy, do it again */
			}
			retVal = SDSext_IO_USER_READ_IN_PROGRESS;
			break;

		case SDS_UDS_REPAIR_SHOP_CODE:
			/* read repair shop code for the different logical blocks */
			retVal_E2P = EEPROM_sfRead(pEEDcb,
					(EEAddressType)(VWDB_EE_ADDDRESS_BootSWRepairShopCodeSerNo + (logBlockCnt * SDS_UDS_F15B_OFFSET_REPAIR_SHOP_CODE)),
					&_SDS_UDS_stResponsePacketToVas.data[_SDS_UDS_stResponsePacketToVas.length],
					SDS_UDS_F15B_OFFSET_REPAIR_SHOP_CODE);

			if ( retVal_E2P == COMMON_RSP_OK )
			{
				sF = SUCCESS;
			}
			else
			{
				sF = FAIL;
			}

			if (SUCCESS == sF)
			{
				/* increase state and add length */
				_SDS_UDS_stResponsePacketToVas.length += SDS_UDS_F15B_OFFSET_REPAIR_SHOP_CODE;
				logBlockState = SDS_UDS_PROG_STATE;
			}
			else
			{
				/* busy, do it again */
			}
			retVal = SDSext_IO_USER_READ_IN_PROGRESS;
			break;

		case SDS_UDS_PROG_STATE:
			/* read repair shop code for the different logical blocks */
			retVal_E2P = EEPROM_sfRead(pEEDcb,
					(EEAddressType)(VWDB_EE_ADDDRESS_BootSWProgState + (logBlockCnt * SDS_UDS_F15B_OFFSET_PROG_STATE)),
					&_SDS_UDS_stResponsePacketToVas.data[_SDS_UDS_stResponsePacketToVas.length],
					SDS_UDS_F15B_OFFSET_PROG_STATE);
			if ( retVal_E2P == COMMON_RSP_OK )
			{
				sF = SUCCESS;
			}
			else
			{
				sF = FAIL;
			}
			if (SUCCESS == sF)
			{
				/* last block arrived? */
				if (SDS_UDS_F15B_LOGICAL_BLOCK_MAX == logBlockCnt)
				{
					/* add length */
					_SDS_UDS_stResponsePacketToVas.length += SDS_UDS_F15B_OFFSET_PROG_STATE;
					/* copy received blocks to response buffer */
					for (i=0; i < _SDS_UDS_stResponsePacketToVas.length; i++)
					{
						data[i] = _SDS_UDS_stResponsePacketToVas.data[i];
					}
					/* reset logical block counter and length */
					logBlockCnt = SDS_UDS_F15B_LOGICAL_BLOCK_MIN;
					_SDS_UDS_stResponsePacketToVas.length = 0;
					/* positive response and we are finished */
					retVal = SDSext_IO_USER_READ_SUCCESSFUL;
				}
				else
				{
					/* increment counter to next block */
					logBlockCnt++;
					/* increase state and add length */
					_SDS_UDS_stResponsePacketToVas.length += SDS_UDS_F15B_OFFSET_PROG_STATE;
					retVal = SDSext_IO_USER_READ_IN_PROGRESS;
				}
				/* set state to startup for the next duration */
				logBlockState = SDS_UDS_PROG_DATE;
			}
			else
			{
				/* busy, do it again */
				retVal = SDSext_IO_USER_READ_IN_PROGRESS;
			}

			break;
		default:
			break;
		}
	}
	else /* SDSext_USER */
	{
		if (ub_RequestPending22 == 1)
		{
			if (ubResponseFromAppTask == 1)
			{
				ubResponseFromAppTask = 0;
				ubCounterResponseFromAppTask = 0;
				if (_SDS_UDS_stResponsePacketToVas.result
						== (Uint8Type)enAppDiagResponseErrorBusyImm)
				{
						ub_RequestPending22 = 0;
						ubCounterResponseFromAppTask = 0;
						return SDSext_IO_USER_READ_FAILED;
				}
				else if (_SDS_UDS_stResponsePacketToVas.result
						== (Uint8Type)enAppDiagResponseErrorImm)
				{
					ub_RequestPending22 = 0;
					return SDSext_IO_USER_READ_FAILED;
				}

					else if (_SDS_UDS_stResponsePacketToVas.result
							== (Uint8Type)enAppDiagResponseOkImm)
					{
						ub_RequestPending22 = 0;
						for (i=0; i < _SDS_UDS_stResponsePacketToVas.length; i++)
						{
							data[i] = _SDS_UDS_stResponsePacketToVas.data[i];
						}
						// jetzt noch sichershalber den Wertebereich prüfen
						switch (_SDS_UDS_stResponsePacketToVas.Identifier)
						{
							case ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP: // jeder Wert von 0..255 ist gültig
							case ID_22_240A_BLUETOOTH_CONNECTED_HEADSET: // jeder Wert von 0..255 ist gültig
							case ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE: // jeder Wert von 0..255 ist gültig
							case ID_22_2411_BLUETOOTH_PAIRED_HEADSETS: // jeder Wert von 0..255 ist gültig
							case ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP: // jeder Wert von 0..255 ist gültig
							case ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES: // jeder Wert von 0..255 ist gültig
							case ID_22_0286_SPANNUNG_KLEMME_30: // jeder Wert von 0..255 ist gültig
							case ID_22_028D_STEUERGERAETETEMPERATUR: // jeder Wert von 0..255 ist gültig
							case ID_22_2401_MICROPHONE_CURRENT_DRAIN: // jeder Wert von 0..255 ist gültig
							case ID_22_2402_TELEPHONE_SIGNAL_STRENGTH: // jeder Wert von 0..255 ist gültig
							case ID_22_0300_SOFTWAREVERSION: // jeder Wert von 0..255 ist gültig
							case ID_22_1100_BLUETOOTH_DEVICE_MAC_ADDRESS: // jeder Wert von 0..255 ist gültig
							{
								break;
							}

							case ID_22_2417_AERIAL_CONNECTION_STATUS: // Werte 0 und 1 sind gültig
							case ID_22_2418_MEDIA_DEVICE_CONNECTION_STATUS: // Werte 0 und 1 sind gültig
							case ID_22_2428_BLUETOOTH_VISIBILITY: // Werte 0 und 1 sind gültig
							{	if (data[0] > 1)
								{
									data[0] = DEFAULTMESSWERT;
								}
								break;
							}
							case ID_22_2435_TELEPHONE_CONNECTIVITY: // Werte 0 bis 7 sind gültig
							{
								if (data[0] > 7)
								{
									data[0] = DEFAULTTELEPHONECONNECTIVITYMESSWERT;
								}
								break;
							}
							case ID_22_2466_SERVICE_CALL_BUTTON_EMERGENCY_CALL_BUTTON_CRADLE: // Werte 0, 1 und 10 sind gültig
							case ID_22_2467_INFO_CALL_BUTTON_VOICE_CONTROL_BUTTON_CRADLE: // Werte 0, 1 und 10 sind gültig
							case ID_22_245F_BLUETOOTH_BUTTON_INFO_CALL_BUTTON: // Werte 0, 1 und 10 sind gültig
							case ID_22_245E_MICROPHONE_MUTE_BUTTON_SERVICE_CALL_BUTTON: // Werte 0, 1 und 10 sind gültig
							case ID_22_2460_ANSWER_TELEPHONE_BUTTON_VOICE_CONTROL_BUTTON: // Werte 0, 1 und 10 sind gültig
							{
								if ((data[0] == 0) || (data[0] == 1) || (data[0]== 0x10))
								{
									;
								}
								else
								{
									data[0] = DEFAULTMESSWERT;
								}
								break;
							}
							case ID_22_2400_MICROPHONE_STATUS: // Werte 0, 1, 10 und 11 sind gültig
							case ID_22_2403_AUDIO_OUTPUT_LEFT: // Werte 0, 1, 10 und 11 sind gültig
							case ID_22_2404_AUDIO_OUTPUT_RIGHT: // Werte 0, 1, 10 und 11 sind gültig
							{
								if ((data[0] == 0) || (data[0] == 1) || (data[0]== 0x10) || (data[0] == 0x11) || (data[0] == 0x02))
								{
									;
								}
								else
								{
									data[0] = DEFAULTMESSWERT;
								}
								break;
							}
							default:
							{
								retVal = SDSext_IO_USER_READ_FAILED;

								break;
							}
						}
						retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					}
			}
			else
			{
				if (ubCounterResponseFromAppTask > MAX_RESPONSE_FROM_AP_TASK)
				{
					ub_RequestPending22 = 0;
					ubCounterResponseFromAppTask = 0;
					return SDSext_IO_USER_READ_FAILED;
				}
				else
				{
					ubCounterResponseFromAppTask++;
					retVal = SDSext_IO_USER_READ_IN_PROGRESS;
				}
			}
		}
		else
		{
			switch (DataIdItem->DataId)
			{
				CASE_ALL_ID_MESSWERT
				{
					ub_RequestPending22 = 1; // den App-Task Routinen Zeit zum Antworten geben.
					_SDS_UDS_stRequestPacketFromVas.Identifier = DataIdItem->DataId;
					_SDS_UDS_stRequestPacketFromVas.length = DataIdItem->Len;
					for (i=0; i<_SDS_UDS_stRequestPacketFromVas.length; i++)
					{
						_SDS_UDS_stRequestPacketFromVas.data[i] = data[i];
					}
					(void)SendMessage(msgAppDiagDiagnoseRequest,
							&_SDS_UDS_stRequestPacketFromVas);
					retVal = SDSext_IO_USER_READ_IN_PROGRESS;
					break;
				}
				case 0x0100: //Actuator Test Status
				{
					data[0] = SDS_UDS_u8GetStatusActuatorTest();
			        retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				case 0x0102: //Basic Settings Status
				{
					data[0] = _SDS_UDS_u8GetStatusBasicSettings();
			        retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
        case 0x0103: //Security Counter Value
        {
            data[0] = (ubSecState & MASK_SEED_COUNTER);
            retVal = SDSext_IO_USER_READ_SUCCESSFUL;
        break;
        }

        case ID_22_F1AE_NUMBERS_OF_LOGINS: // number of login methods implemeted
        {
            data[0] = NUMBER_OF_LOGIN_METHODS_IMPLEMETED;
            retVal = SDSext_IO_USER_READ_SUCCESSFUL;
            break;
        }

				case ID_22_2422_USER_PROFILES:
				{
					data[0] = _SDS_UDS_u8UserProfile;
			        retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}

				case ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE:
				{
					for (i=0; i<ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE_MAX_LEN; i++)
					{
						data[i] = VERSION_VALUES[i];
					}

					UDS_vGetVersionInfo(&psDummyVersionInfo);
					ubPosVersionModuleID
					= SDSext_ubFindVersionModuleID((Uint8Type) psDummyVersionInfo.moduleID);
					data[ubPosVersionModuleID+1]
						 = (Uint8Type) psDummyVersionInfo.sw_major_version;
					data[ubPosVersionModuleID+2]
						 = (Uint8Type) psDummyVersionInfo.sw_minor_version;
					data[ubPosVersionModuleID+3]
						 = (Uint8Type) psDummyVersionInfo.sw_patch_version;

					DEH_GetVersionInfo(&pVersionInfo);
					ubPosVersionModuleID
					= SDSext_ubFindVersionModuleID((Uint8Type) pVersionInfo.moduleID);
					data[ubPosVersionModuleID+1]
						 = (Uint8Type) pVersionInfo.sw_major_version;
					data[ubPosVersionModuleID+2]
						 = (Uint8Type) pVersionInfo.sw_minor_version;
					data[ubPosVersionModuleID+3]
						 = (Uint8Type) pVersionInfo.sw_patch_version;

					TP_vGetVersionInfo(&psDummyVersionInfo);
					ubPosVersionModuleID
					= SDSext_ubFindVersionModuleID((Uint8Type) psDummyVersionInfo.moduleID);
					data[ubPosVersionModuleID+1]
						 = (Uint8Type) psDummyVersionInfo.sw_major_version;
					data[ubPosVersionModuleID+2]
						 = (Uint8Type) psDummyVersionInfo.sw_minor_version;
					data[ubPosVersionModuleID+3]
						 = (Uint8Type) psDummyVersionInfo.sw_patch_version;
			        retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
			    }
				case 0xF186: //ActiveDiagnosticSession
				{
					data[0] = UDS_ubGetSession(UDS_CHANNEL_INDEX_2);
			        retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}

				case ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS:
				{
					for (i=0; i<ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MAX_LEN; i++)
					{
						data[i] = _SDS_UDS_au8FingerprintValue[i];
					}
			        retVal = SDSext_IO_USER_READ_SUCCESSFUL;
					break;
				}
				default:
				{
					retVal = SDSext_IO_USER_READ_FAILED;
					break;
				}

			}
		}
	}

	return (retVal);
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_ubFindVersionModuleID                                    */
/**
 \brief      Callback function for set interrupt bits

 \brief      Description:
 function to set bits for the diferent interrupts

 \param      VersionModuleID


 \return
 */
/*----------------------------------------------------------------------------*/
static Uint8Type SDSext_ubFindVersionModuleID(Uint8Type VersionModuleID)
{
	int i;

	for (i=0; i<ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE_MAX_LEN; i+=4)
	{
		if (VERSION_VALUES[i] == VersionModuleID)
		{
			break;
		}

	}
	return (i);
}

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseService22                                       */
/**
 \brief      To reset the Static variables

 \brief      Description:
 this function resets the static variables.


 \return     void
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseService22(void)
{
	eReadInProgress = SDSext_IO_USER_READ_SUCCESSFUL;
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_u8DeterminePhoneNumberLength                          */
/**
 \brief      determine the phone numbers length

 \param      none

 \return     none
 */
/*----------------------------------------------------------------------------*/
Uint8Type SDSext_u8DeterminePhoneNumberLength(const Uint8Type *data,Uint16Type length)
{
	Uint8Type lencor = 0;
	int i = 0;

	while (i < length)
	{
		if ((data[i] & 0xF0) == 0xF0)
		{
			lencor = i;
			break;
		}
		else if ((data[i] & 0x0F) == 0x0F)
		{
			lencor = i+1;
			break;
		}
		else
		{
			i++;
		}
	}
	return lencor;
}



/* End Of File SDS_UDS_Service22_C1.c */
