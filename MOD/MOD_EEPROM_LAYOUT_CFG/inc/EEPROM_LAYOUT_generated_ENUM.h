typedef enum
{
/*0*/ EEPROM_LAYOUT_ENUM_VWSparePartNbr,
/*1*/ EEPROM_LAYOUT_ENUM_AppSoftwareVersionNbr,
/*2*/ EEPROM_LAYOUT_ENUM_AsamOdxFileVersion,
/*3*/ EEPROM_LAYOUT_ENUM_ECUProgramInfo,
/*4*/ EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation,
/*5*/ EEPROM_LAYOUT_ENUM_VWCodingValue,
/*6*/ EEPROM_LAYOUT_ENUM_VWTesterCodingInformation,
/*7*/ EEPROM_LAYOUT_ENUM_VWCodingRepairShopCodeOrSerialNumber,
/*8*/ EEPROM_LAYOUT_ENUM_VWCodingDate,
/*9*/ EEPROM_LAYOUT_ENUM_CRCCoding,
/*10*/ EEPROM_LAYOUT_ENUM_VWCalibrationRepairShopCodeOrSerialNumber,
/*11*/ EEPROM_LAYOUT_ENUM_VWCalibrationDate,
/*12*/ EEPROM_LAYOUT_ENUM_UHV_SoftwareInternByte,
/*13*/ EEPROM_LAYOUT_ENUM_SetNumberRoamingForInfoCall,
/*14*/ EEPROM_LAYOUT_ENUM_SetNumberRoamingForServiceCall,
/*15*/ EEPROM_LAYOUT_ENUM_SetNumberForInfoCall,
/*16*/ EEPROM_LAYOUT_ENUM_SetNumberForServiceCall,
/*17*/ EEPROM_LAYOUT_ENUM_SMSWriting,
/*18*/ EEPROM_LAYOUT_ENUM_BluetoothAcknowledgementSignal,
/*19*/ EEPROM_LAYOUT_ENUM_DeveloperTestmode,
/*20*/ EEPROM_LAYOUT_ENUM_MaskingOfLanguages,
/*21*/ EEPROM_LAYOUT_ENUM_UniversalPreparationForMobileTelephone,
/*22*/ EEPROM_LAYOUT_ENUM_TelephoneBasicVolume,
/*23*/ EEPROM_LAYOUT_ENUM_VoiceOutputBasicVolume,
/*24*/ EEPROM_LAYOUT_ENUM_MicrophoneSensitivity,
/*25*/ EEPROM_LAYOUT_ENUM_BluetoothSetPINCode,
/*26*/ EEPROM_LAYOUT_ENUM_CDMA_Detection,
/*27*/ EEPROM_LAYOUT_ENUM_MuteDelay,
/*28*/ EEPROM_LAYOUT_ENUM_Reference_Channel_Delay,
/*29*/ EEPROM_LAYOUT_ENUM_MediaDeviceBasicVolume,
/*30*/ EEPROM_LAYOUT_ENUM_EmergencyNumber,
/*31*/ EEPROM_LAYOUT_ENUM_VWDataSetRepairShopCodeOrSerialNumber,
/*32*/ EEPROM_LAYOUT_ENUM_VWDataSetProgrammingDate,
/*33*/ EEPROM_LAYOUT_ENUM_VWDataCounterOfProgrammingAttempts,
/*34*/ EEPROM_LAYOUT_ENUM_VWDataCounterOfSuccessfulProgrammingAttempts,
/*35*/ EEPROM_LAYOUT_ENUM_BootSWLogicalBlockVersion,
/*36*/ EEPROM_LAYOUT_ENUM_AswSWLogicalBlockVersion,
/*37*/ EEPROM_LAYOUT_ENUM_TIMELOCK,
/*38*/ EEPROM_LAYOUT_ENUM_SysProgrammed,
/*39*/ EEPROM_LAYOUT_ENUM_SecurityAccessState,
/*40*/ EEPROM_LAYOUT_ENUM_Service86ResponseOnEvent,
/*41*/ EEPROM_LAYOUT_ENUM_DTC_Maskierung,
/*42*/ EEPROM_LAYOUT_ENUM_WD_Pattern,
/*43*/ EEPROM_LAYOUT_ENUM_LastSaveLanguageSelection,
/*44*/ EEPROM_LAYOUT_ENUM_WerkseinstellungUniversalPreparationForMobileTelephone,
/*45*/ EEPROM_LAYOUT_ENUM_RESERVED_04,
/*46*/ EEPROM_LAYOUT_ENUM_WerkseinstellungBluetoothSetPINCode,
/*47*/ EEPROM_LAYOUT_ENUM_LastSaveMarkeSelection,
/*48*/ EEPROM_LAYOUT_ENUM_LastSaveLanguageVCR,
/*49*/ EEPROM_LAYOUT_ENUM_VoiceRecognitionSupport,
/*50*/ EEPROM_LAYOUT_ENUM_VoiceFeedback,
/*51*/ EEPROM_LAYOUT_ENUM_Backup_AppSoftwareVersionNbr,
/*52*/ EEPROM_LAYOUT_ENUM_Backup_AsamOdxFileVersion,
/*53*/ EEPROM_LAYOUT_ENUM_DEH_UnclearCounter,
/*54*/ EEPROM_LAYOUT_ENUM_KPB_Display_Status,
/*55*/ EEPROM_LAYOUT_ENUM_UHV_Variant,
/*56*/ EEPROM_LAYOUT_ENUM_UHV_EEPROM_Version,
ENUM_EEPROM_LAYOUT_MAX
} EEPROM_ENUM_GET_EEPROM; //EEPROM_LAYOUT_ENUM_ALL_ELEMENTS;




#define RAM_BLOCK_COUNT    18
