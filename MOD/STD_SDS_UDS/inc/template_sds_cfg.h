/**************************************************************************
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: SDS_CFG.H
 **************************************************************************
 *  PURPOSE:
 *  Configuration H-File for SDS Module
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.10.2006 - first version 
 *  V_00_01_01 Ki 02.11.2006 - no change
 *  V_00_01_02 Ki 12.12.2006 - new: SDS_A_DTCStatusAvailabilityMask
 *  V_00_01_03 Ki 19.12.2006 - check memory allocation _I2S_SDS_C_
 *  V_00_01_04 Ki 10.01.2007 - no change
 *  V_00_01_05 Ki 12.01.2007 - i2s_types.h included
 *                           - const byte* --> byte* at cs_DataIdItem 
 *  V_00_01_06 Ki 02.03.2007 - general revision, review by VW
 *  V_01_00_00 Ki 30.03.2007 - SDS_ENABLED, SDS_DISABLED moved into sds.h
 *                           - SDS_85_ZeroVehicleOrEngineSpeed new
 *  V_01_00_01 Ki 11.04.2007 - SDS_USAGE deleted
 *  V_01_00_02 Ki 24.04.2007 - no change
 *  V_01_00_03 Ki 02.07.2007 - SDS_22_FFFF_TerminationValue deleted
 *  V_01_00_04 Ki 11.07.2007 - no change
 *  V_01_00_05 Ki 23.07.2007 - no change
 *  V_01_00_06 Ki 02.11.2007 - 10,02 optional --> #define SDS_ECU_PROGRAMABLE
 *  V_01_00_07 Ki 04.02.2008 - no change
 *  V_01_00_08 Ki 18.06.2008 - new: data identifier F1AF
 *  V_01_00_09 Ki 24.06.2008 - no change 
 *  V_01_00_09 Ki 23.07.2008 - comment for SDS_85_ZeroVehicleOrEngineSpeed 
 *  V_01_01_00 Ki 08.08.2008 - new: SDS_10_ZeroVehicleOrEngineSpeed 
 *                           - new: SDS_28_ZeroVehicleOrEngineSpeed 
 *************************************************************************/

#ifndef  _I2S_SDS_CFG_H_
#define  _I2S_SDS_CFG_H_

/* ----------------------------------------------------------------------- */
/*  - C o n s t a n t s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */

#define SDS_UDS_CLIENT_ID                   0            /* uds client id for SDS */

#define SDS_ECU_PROGRAMABLE                 0            /* 0=not programable, 1=programable */ 

#define SDS_A_DTCStatusAvailabilityMask     0x19         /* 0x19 for ECUs without Warning Lamp */
                                                         /* 0x99 for ECUs with Warning Lamp */
                                                         /* 0xDF for OBD ECUs */

#define SDS_10_ZeroVehicleOrEngineSpeed     SDS_DISABLED /* check/not-check v and n in servcice $10 */
#define SDS_28_ZeroVehicleOrEngineSpeed     SDS_DISABLED /* check/not-check v and n in servcice $28 */
#define SDS_85_ZeroVehicleOrEngineSpeed     SDS_DISABLED /* check/not-check v and n in servcice $85 */

#define SDS_A_START_DIAGNOSTIC_SESSION      SDS_ENABLED
#define SDS_A_RESET_OR_DISCONNECT           SDS_ENABLED
#define SDS_A_CLEAR_DIAGNOSTIC_INF          SDS_ENABLED
#define SDS_A_READ_DTC_BY_STATUS            SDS_ENABLED
#define SDS_A_READ_DATA_BY_IDENTIFIER       SDS_ENABLED
#define SDS_A_COMMUNICATION_CONTROL         SDS_ENABLED
#define SDS_A_TESTER_PRESENT                SDS_ENABLED
#define SDS_A_CTRL_DTC_SETTINGS             SDS_ENABLED

#define SDS_11_POWER_DOWN                   SDS_ENABLED
#define SDS_11_TimeToPowerDown              (byte)5      /* 1..30 s, 255=not supported */   

#define SDS_22_0405_StateOfFlashMemory                                           SDS_ENABLED
#define SDS_22_0407_VWLogicalSoftwareBlockCounterOfProgrammingAttempts           SDS_ENABLED
#define SDS_22_0408_VWLogicalSoftwareBlockCounterOfSuccessfulProgrammingAttempts SDS_ENABLED
#define SDS_22_0409_VWDataCounterOfProgrammingAttempts                           SDS_ENABLED
#define SDS_22_040A_VWDataCounterOfSuccessfulProgrammingAttempts                 SDS_ENABLED
#define SDS_22_040F_VWLogicalSoftwareBlockLockValue                              SDS_ENABLED
#define SDS_22_0600_VWCodingValue                                                SDS_ENABLED
#define SDS_22_0601_VWTesterCodingInformation                                    SDS_ENABLED
#define SDS_22_0606_IdentifiedSlaveSystems                                       SDS_ENABLED
#define SDS_22_0760_LINIdentificationTable                                       SDS_ENABLED
#define SDS_22_F15A_FingerprintAndProgrammingDate                                SDS_ENABLED
#define SDS_22_F15B_FingerprintAndProgrammingDateOfLogicalSoftwareBlocks         SDS_ENABLED
#define SDS_22_F179_ECUSupplierNumber                                            SDS_ENABLED
#define SDS_22_F17B_VWCodingDate                                                 SDS_ENABLED
#define SDS_22_F17C_VWFAZITIdentificationString                                  SDS_ENABLED
#define SDS_22_F17D_ECUProductionTestSystemNumber                                SDS_ENABLED
#define SDS_22_F17E_ECUProductionChangeNumber                                    SDS_ENABLED
#define SDS_22_F17F_VWSupplierNumber                                             SDS_ENABLED
#define SDS_22_F181_VWStandardApplicationSoftwareIdentification                  SDS_ENABLED
#define SDS_22_F182_VWApplicationDataIdentification                              SDS_ENABLED
#define SDS_22_F186_ActiveDiagnosticSession                                      SDS_ENABLED
#define SDS_22_F187_VWSparePartNumber                                            SDS_ENABLED
#define SDS_22_F189_VWSoftwareVersionNumber                                      SDS_ENABLED
#define SDS_22_F18B_ECUManufacturingDate                                         SDS_ENABLED
#define SDS_22_F18C_ECUSerialNumber                                              SDS_ENABLED
#define SDS_22_F190_VIN                                                          SDS_ENABLED
#define SDS_22_F191_VWECUHardwareNumber                                          SDS_ENABLED
#define SDS_22_F196_ExhaustRegulationOrTypeApprovalNumber                        SDS_ENABLED
#define SDS_22_F197_SystemNameOrEngineType                                       SDS_ENABLED
#define SDS_22_F198_RepairShopCodeOrTesterSerialNumber                           SDS_ENABLED
#define SDS_22_F199_ProgrammingDate                                              SDS_ENABLED
#define SDS_22_F19A_VWCalibrationRepairShopCodeOrSerialNumber                    SDS_ENABLED
#define SDS_22_F19B_VWCalibrationDate                                            SDS_ENABLED
#define SDS_22_F19E_ASAMODXFileIdentifier                                        SDS_ENABLED
#define SDS_22_F1A0_VWDataSetNumberOrECUDataContainerNumber                      SDS_ENABLED
#define SDS_22_F1A1_VWDataSetVersionNumber                                       SDS_ENABLED
#define SDS_22_F1A2_ASAMODXFileVersion                                           SDS_ENABLED
#define SDS_22_F1A3_VWECUHardwareVersionNumber                                   SDS_ENABLED
#define SDS_22_F1A4_VehicleEquipmentCodeAndPRNumberCombination                   SDS_ENABLED
#define SDS_22_F1A5_VWCodingRepairShopCodeOrSerialNumber                         SDS_ENABLED
#define SDS_22_F1A6_VWLogicalSoftwareBlockSoftwareProgrammingDate                SDS_ENABLED
#define SDS_22_F1A8_VWDataSetRepairShopCodeOrSerialNumber                        SDS_ENABLED
#define SDS_22_F1A9_VWDataSetProgrammingDate                                     SDS_ENABLED
#define SDS_22_F1AA_VWWorkshopSystemName                                         SDS_ENABLED
#define SDS_22_F1AB_VWLogicalSoftwareBlockVersion                                SDS_ENABLED
#define SDS_22_F1AC_VWEOLConfiguration                                           SDS_ENABLED
#define SDS_22_F1AD_EngineCodeLetters                                            SDS_ENABLED
#define SDS_22_F1AF_AUTOSARStandardApplicationSoftwareIdentification             SDS_ENABLED
#define SDS_22_F1DF_ECUProgrammingInformation                                    SDS_ENABLED
#define SDS_22_F1E0_ECUDataProgrammingInformation                                SDS_ENABLED

#ifdef  _I2S_SDS_C_
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )

/************** A T T E N T I O N ***************************** 
   because of speed optimization binary search algorithm is used
   which requires a sorted (ascending !!!) data identifier table */
static const t_DataIdCfgItem cs_DataIdItem[] = {
#if( SDS_22_0405_StateOfFlashMemory == SDS_ENABLED )
{ 0x0405, SDS_BINARY, 1, (void*)"0F" }, /* 1 byte hex */
#endif
#if( SDS_22_0407_VWLogicalSoftwareBlockCounterOfProgrammingAttempts == SDS_ENABLED )
{ 0x0407, SDS_ROM, 10, (void*)0 },
#endif
#if( SDS_22_0408_VWLogicalSoftwareBlockCounterOfSuccessfulProgrammingAttempts == SDS_ENABLED )
{ 0x0408, SDS_RAM, 10, (void*)0 },
#endif
#if( SDS_22_0409_VWDataCounterOfProgrammingAttempts == SDS_ENABLED )
{ 0x0409, SDS_EEPROM, 10, (void*)0 },
#endif
#if( SDS_22_040A_VWDataCounterOfSuccessfulProgrammingAttempts == SDS_ENABLED )
{ 0x040A, SDS_USER, 10, (void*)0 },
#endif
#if( SDS_22_040F_VWLogicalSoftwareBlockLockValue == SDS_ENABLED )
{ 0x040F, SDS_USER, 10, (void*)0 },
#endif
#if( SDS_22_0600_VWCodingValue == SDS_ENABLED )
{ 0x0600, SDS_BINARY, 0, (void*)0 },
#endif
#if( SDS_22_0601_VWTesterCodingInformation == SDS_ENABLED )
{ 0x0601, SDS_BINARY, 1, (void*)"00" }, /* 1 byte hex */
#endif
#if( SDS_22_0606_IdentifiedSlaveSystems == SDS_ENABLED )
{ 0x0606, SDS_BINARY, 0, (void*)0 },
#endif
#if( SDS_22_0760_LINIdentificationTable == SDS_ENABLED )
{ 0x0760, SDS_BINARY, 0, (void*)0 },
#endif
#if( SDS_22_F15A_FingerprintAndProgrammingDate == SDS_ENABLED )
{ 0xF15A, SDS_BINARY, 0, (void*)0 },
#endif
#if( SDS_22_F15B_FingerprintAndProgrammingDateOfLogicalSoftwareBlocks == SDS_ENABLED )
{ 0xF15B, SDS_BINARY, 0, (void*)0 },
#endif
#if( SDS_22_F179_ECUSupplierNumber == SDS_ENABLED )
{ 0xF179, SDS_ASCII, 4, (void*)"----" }, /* 4 byte ASCII */
#endif
#if( SDS_22_F17B_VWCodingDate == SDS_ENABLED )
{ 0xF17B, SDS_BINARY, 3, (void*)"000000" }, /* 3 byte BCD */
#endif
#if( SDS_22_F17C_VWFAZITIdentificationString == SDS_ENABLED )
{ 0xF17C, SDS_ASCII, 23, (void*)"hhh-kkkdd.mm.yyppppnnnn" }, /* 23 byte ASCII */
#endif                           
#if( SDS_22_F17D_ECUProductionTestSystemNumber == SDS_ENABLED )
{ 0xF17D, SDS_ASCII, 4, (void*)"---" },  /* 4 byte ASCII */
#endif
#if( SDS_22_F17E_ECUProductionChangeNumber == SDS_ENABLED )
{ 0xF17E, SDS_ASCII, 8, (void*)"-------" },  /* 8 byte ASCII */
#endif
#if( SDS_22_F17F_VWSupplierNumber == SDS_ENABLED )
{ 0xF17F, SDS_ASCII, 3, (void*)"--" },  /* 3 byte ASCII */
#endif
#if( SDS_22_F181_VWStandardApplicationSoftwareIdentification == SDS_ENABLED )
{ 0xF181, SDS_ASCII, 0, (void*)0 },
#endif
#if( SDS_22_F182_VWApplicationDataIdentification == SDS_ENABLED )
{ 0xF182,  SDS_ASCII, 0, (void*)0 },
#endif
#if( SDS_22_F186_ActiveDiagnosticSession == SDS_ENABLED )
{ 0xF186,  SDS_USER, 1, (void*)0 }, /* 1 byte HEX */
#endif
#if( SDS_22_F187_VWSparePartNumber == SDS_ENABLED )
{ 0xF187,  SDS_ASCII, 11, (void*)"XXXXXXXXXXX" },  /* 11 byte ASCII */
#endif
#if( SDS_22_F189_VWSoftwareVersionNumber == SDS_ENABLED )
{ 0xF189,  SDS_ASCII, 4, (void*)"U001" },  /* 4 byte ASCII */
#endif
#if( SDS_22_F18B_ECUManufacturingDate == SDS_ENABLED )
{ 0xF18B,  SDS_BINARY, 3, (void*)"000000" },  /* 3 byte BCD */
#endif
#if( SDS_22_F18C_ECUSerialNumber == SDS_ENABLED )
{ 0xF18C,  SDS_ASCII, 0, (void*)0 },
#endif
#if( SDS_22_F190_VIN == SDS_ENABLED )
{ 0xF190,  SDS_ASCII, 17, (void*)"-----------------" },  /* 17 byte ASCII */
#endif
#if( SDS_22_F191_VWECUHardwareNumber == SDS_ENABLED )
{ 0xF191,  SDS_ASCII, 11, (void*)"4E0000000__" },  /* 11 byte ASCII */
#endif
#if( SDS_22_F196_ExhaustRegulationOrTypeApprovalNumber == SDS_ENABLED )
{ 0xF196,  SDS_ASCII, 7, (void*)"-------" },  /* 7 byte ASCII */
#endif
#if( SDS_22_F197_SystemNameOrEngineType == SDS_ENABLED )
{ 0xF197,  SDS_ASCII, 13, (void*)"-------------" },  /* 13 byte ASCII */
#endif
#if( SDS_22_F198_RepairShopCodeOrTesterSerialNumber == SDS_ENABLED )
{ 0xF198,  SDS_ASCII, 6, (void*)0 },
#endif
#if( SDS_22_F199_ProgrammingDate == SDS_ENABLED )
{ 0xF199,  SDS_ASCII, 3, (void*)0 },
#endif
#if( SDS_22_F19A_VWCalibrationRepairShopCodeOrSerialNumber == SDS_ENABLED )
{ 0xF19A,  SDS_BINARY, 6, (void*)"000000000000" },  /* 6 byte HEX */
#endif
#if( SDS_22_F19B_VWCalibrationDate == SDS_ENABLED )
{ 0xF19B,  SDS_BINARY, 3, (void*)"000000" },  /* 3 byte BCD */
#endif
#if( SDS_22_F19E_ASAMODXFileIdentifier == SDS_ENABLED )
{ 0xF19E,  SDS_ASCII, 0, (void*)0 },
#endif
#if( SDS_22_F1A0_VWDataSetNumberOrECUDataContainerNumber == SDS_ENABLED )
{ 0xF1A0,  SDS_ASCII, 11, (void*)"4F0000000__" },  /* 11 byte ASCII */
#endif
#if( SDS_22_F1A1_VWDataSetVersionNumber == SDS_ENABLED )
{ 0xF1A1,  SDS_ASCII, 4, (byte*)"U001" },  /* 4 byte ASCII */
#endif
#if( SDS_22_F1A2_ASAMODXFileVersion == SDS_ENABLED )
{ 0xF1A2,  SDS_ASCII, 6, (void*)"S01001" },  /* 6 byte ASCII */
#endif
#if( SDS_22_F1A3_VWECUHardwareVersionNumber == SDS_ENABLED )
{ 0xF1A3,  SDS_ASCII, 3, (void*)"U01" },  /* 3 byte ASCII */
#endif
#if( SDS_22_F1A4_VehicleEquipmentCodeAndPRNumberCombination == SDS_ENABLED )
{ 0xF1A4,  SDS_BINARY, 8, (void*)"1122334455667788" },  /* 8 byte HEX */
#endif
#if( SDS_22_F1A5_VWCodingRepairShopCodeOrSerialNumber == SDS_ENABLED )
{ 0xF1A5,  SDS_BINARY, 6, (void*)"000000000000" },  /* 6 byte HEX */
#endif
#if( SDS_22_F1A6_VWLogicalSoftwareBlockSoftwareProgrammingDate == SDS_ENABLED )
{ 0xF1A6,  SDS_ASCII, 0, (void*)0 },
#endif
#if( SDS_22_F1A8_VWDataSetRepairShopCodeOrSerialNumber == SDS_ENABLED )
{ 0xF1A8,  SDS_BINARY, 6, (void*)"000000000000" },  /* 6 byte HEX */
#endif
#if( SDS_22_F1A9_VWDataSetProgrammingDate == SDS_ENABLED )
{ 0xF1A9,  SDS_BINARY, 3, (void*)"000000" },  /* 3 byte BCD */
#endif
#if( SDS_22_F1AA_VWWorkshopSystemName == SDS_ENABLED )
{ 0xF1AA,  SDS_ASCII, 5, (void*)"---- " },  /* 5 byte ASCII (one blank!) */
#endif
#if( SDS_22_F1AB_VWLogicalSoftwareBlockVersion == SDS_ENABLED )
{ 0xF1AB,  SDS_ASCII, 0, (void*)0 },
#endif
#if( SDS_22_F1AC_VWEOLConfiguration == SDS_ENABLED )
{ 0xF1AC,  SDS_ASCII, 18, (void*)"------------------" },  /* 18 byte ASCII */
#endif
#if( SDS_22_F1AD_EngineCodeLetters == SDS_ENABLED )
{ 0xF1AD,  SDS_ASCII, 4, (void*)"----" },  /* 4 byte ASCII */
#endif
#if( SDS_22_F1AF_AUTOSARStandardApplicationSoftwareIdentification == SDS_ENABLED )
{ 0xF1AF,  SDS_USER, 0, (void*)0 },
#endif
#if( SDS_22_F1DF_ECUProgrammingInformation == SDS_ENABLED )
{ 0xF1DF,  SDS_BINARY, 1, (void*)"00" },  /* 1 byte HEX */
#endif
#if( SDS_22_F1E0_ECUDataProgrammingInformation == SDS_ENABLED )
{ 0xF1E0,  SDS_BINARY, 1, (void*)"00" },  /* 1 byte HEX */
#endif
};
#endif /* SDS_A_READ_DATA_BY_IDENTIFIER */
#endif /* _I2S_SDS_C_ */

#endif /* _I2S_SDS_CFG_H_ */
