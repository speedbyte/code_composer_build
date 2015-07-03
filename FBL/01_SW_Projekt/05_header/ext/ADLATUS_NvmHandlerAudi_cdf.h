/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_NvmHandlerAudi_cdf.h                                     |
|                                                                              |
|  Comment:   This file includes all defines and constants for the NVMHandler  |
|             SMART ADLATUS.                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_NvmHandlerAudi_cdf.h 4408 2008-11-06 17:15:01Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_NvmHandlerAudi_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_NVMHANDLERAUDI_
#define __CDF_NVMHANDLERAUDI_



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */

/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area 18                                                        */
/*  ------------------------------------------------------------------        */
/*  NVM Handles                                                               */
/*                                                                            */
/*                                                                            */
/*  A T T E N T I O N :                                                       */
/*  ------------------------------------------------------------------        */
/*  Do NOT change any of the existing NVM Handles!                            */
/*  If necessary other handles can be added.                                  */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */
/* #undef NVM_CODINGSUPPORT */
/* #undef NVM_SLAVESUPPORT */

/* Standard ECU Identifications */

/* 0xF187 */
#define d_CCnvm_READ__VWSparePartNbr              (UBYTE) 0x01u
#define d_CCnvm_WRITE_VWSparePartNbr              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__VWSparePartNbr)

/* 0xF189 */
#define d_CCnvm_READ__AppSoftwareVersionNbr       (UBYTE) 0x02u
#define d_CCnvm_WRITE_AppSoftwareVersionNbr       (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AppSoftwareVersionNbr)

/* 0xF191 */
#define d_CCnvm_READ__ECUHardwareNbr              (UBYTE) 0x03u
#define d_CCnvm_WRITE_ECUHardwareNbr              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ECUHardwareNbr)

/* 0xF1A3 */
#define d_CCnvm_READ__ECUHardwareVersionNbr       (UBYTE) 0x04u
#define d_CCnvm_WRITE_ECUHardwareVersionNbr       (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ECUHardwareVersionNbr)

/* 0xF19E */
#define d_CCnvm_READ__AsamOdxFileID               (UBYTE) 0x05u
#define d_CCnvm_WRITE_AsamOdxFileID               (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AsamOdxFileID)

/* 0xF1A2 */
#define d_CCnvm_READ__AsamOdxFileVersion          (UBYTE) 0x06u
#define d_CCnvm_WRITE_AsamOdxFileVersion          (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AsamOdxFileVersion)

#ifdef NVM_CODINGSUPPORT
  /* 0x0601 */
  #define d_CCnvm_READ__VWTesterCodingInfo        (UBYTE) 0x07u
  #define d_CCnvm_WRITE_VWTesterCodingInfo        (UBYTE) (d_CCglo_Write | d_CCnvm_READ__VWTesterCodingInfo)

  /* 0x0600 */
  #define d_CCnvm_READ__VWTesterCodingValue       (UBYTE) 0x08u
  #define d_CCnvm_WRITE_VWTesterCodingValue       (UBYTE) (d_CCglo_Write | d_CCnvm_READ__VWTesterCodingValue)
#endif

/* 0xF1A0 */
#define d_CCnvm_READ__DataSetNbr                  (UBYTE) 0x09u
#define d_CCnvm_WRITE_DataSetNbr                  (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSetNbr)

/* 0xF1A1 */
#define d_CCnvm_READ__DataVersionNbr              (UBYTE) 0x0Au
#define d_CCnvm_WRITE_DataVersionNbr              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataVersionNbr)

/* 0xF1AA */
#define d_CCnvm_READ__VWWorkshopSystemName        (UBYTE) 0x0Bu
#define d_CCnvm_WRITE_VWWorkshopSystemName        (UBYTE) (d_CCglo_Write | d_CCnvm_READ__VWWorkshopSystemName)

#ifdef NVM_SLAVESUPPORT
  /* 0x0606 */
  #define d_CCnvm_READ__IdentifiedSlaveSystem     (UBYTE) 0x0Cu
  #define d_CCnvm_WRITE_IdentifiedSlaveSystem     (UBYTE) (d_CCglo_Write | d_CCnvm_READ__IdentifiedSlaveSystem)
#endif

/* 0xF1A4 */
#define d_CCnvm_READ__VehEquipCodePRNbr           (UBYTE) 0x0Du
#define d_CCnvm_WRITE_VehEquipCodePRNbr           (UBYTE) (d_CCglo_Write | d_CCnvm_READ__VehEquipCodePRNbr)

/* 0xF1DF */
#define d_CCnvm_READ__ECUProgramInfo              (UBYTE) 0x0Eu
#define d_CCnvm_WRITE_ECUProgramInfo              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ECUProgramInfo)

/* 0xF197 */
#define d_CCnvm_READ__SysNameEngineType           (UBYTE) 0x0Fu
#define d_CCnvm_WRITE_SysNameEngineType           (UBYTE) (d_CCglo_Write | d_CCnvm_READ__SysNameEngineType)


/* --------------------- */
/* ECU Identifications for update programing without sw block ids */

/* 0xF198 */
#define d_CCnvm_READ__RepairShopCodeTSNbr         (UBYTE) 0x10u
#define d_CCnvm_WRITE_RepairShopCodeTSNbr         (UBYTE) (d_CCglo_Write | d_CCnvm_READ__RepairShopCodeTSNbr)

/* 0xF199 */
#define d_CCnvm_READ__ProgDate                    (UBYTE) 0x11u
#define d_CCnvm_WRITE_ProgDate                    (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ProgDate)

/* 0x0405 */
#define d_CCnvm_READ__StateOfFlash                (UBYTE) 0x12u
#define d_CCnvm_WRITE_StateOfFlash                (UBYTE) (d_CCglo_Write | d_CCnvm_READ__StateOfFlash)

/* 0xF18C */
#define d_CCnvm_READ__ECUSerialNumber             (UBYTE) 0x13u
#define d_CCnvm_WRITE_ECUSerialNumber             (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ECUSerialNumber)

#if 0
/* 0xF190 */
#define d_CCnvm_READ__VIN                         (UBYTE) 0x15u
#define d_CCnvm_WRITE_VIN                         (UBYTE) (d_CCglo_Write | d_CCnvm_READ__VIN)
#endif
/****************************************************************************/
/* logical block index 0 -- bootloader */

/* Internal information for each block */
#define d_CCnvm_READ__BootSWProgState             (UBYTE)  0x20u
#define d_CCnvm_WRITE_BootSWProgState             (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWProgState)

/* Identification Data for logical Blocks */

/* 0xF1A6 */
#define d_CCnvm_READ__BootSWProgDate              (UBYTE) 0x21u
#define d_CCnvm_WRITE_BootSWProgDate              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWProgDate)

/* 0xF184 */
#define d_CCnvm_READ__BootSWRepairShopCodeSerNo   (UBYTE) 0x22u
#define d_CCnvm_WRITE_BootSWRepairShopCodeSerNo   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWRepairShopCodeSerNo)

/* 0xF1AB */
#define d_CCnvm_READ__BootSWLogicalBlockVersion   (UBYTE) 0x23u
#define d_CCnvm_WRITE_BootSWLogicalBlockVersion   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWLogicalBlockVersion)

/* 0x0407 */
#define d_CCnvm_READ__BootSWCntProgrAttempts      (UBYTE) 0x24u
#define d_CCnvm_WRITE_BootSWCntProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWCntProgrAttempts)

/* 0x0408  */
#define d_CCnvm_READ__BootSWSucProgrAttempts      (UBYTE) 0x25u
#define d_CCnvm_WRITE_BootSWSucProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWSucProgrAttempts)

/* 0x040F */
#define d_CCnvm_READ__BootSWMaxProgrAttempts      (UBYTE) 0x26u
#define d_CCnvm_WRITE_BootSWMaxProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__BootSWMaxProgrAttempts)

/****************************************************************************/
/* logical block index 1 -- application */

/* Internal information for each block */
#define d_CCnvm_READ__AswSWProgState             (UBYTE) 0x30u
#define d_CCnvm_WRITE_AswSWProgState             (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWProgState)

/* Identification Data for logical Blocks */

/* 0xF1A6 */
#define d_CCnvm_READ__AswSWProgDate              (UBYTE) 0x31u
#define d_CCnvm_WRITE_AswSWProgDate              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWProgDate)

/* 0xF184 */
#define d_CCnvm_READ__AswSWRepairShopCodeSerNo   (UBYTE) 0x32u
#define d_CCnvm_WRITE_AswSWRepairShopCodeSerNo   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWRepairShopCodeSerNo)

/* 0xF1AB */
#define d_CCnvm_READ__AswSWLogicalBlockVersion   (UBYTE) 0x33u
#define d_CCnvm_WRITE_AswSWLogicalBlockVersion   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWLogicalBlockVersion)

/* 0x0407 */
#define d_CCnvm_READ__AswSWCntProgrAttempts      (UBYTE) 0x34u
#define d_CCnvm_WRITE_AswSWCntProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWCntProgrAttempts)

/* 0x0408  */
#define d_CCnvm_READ__AswSWSucProgrAttempts      (UBYTE) 0x35u
#define d_CCnvm_WRITE_AswSWSucProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWSucProgrAttempts)

/* 0x040F */
#define d_CCnvm_READ__AswSWMaxProgrAttempts      (UBYTE) 0x36u
#define d_CCnvm_WRITE_AswSWMaxProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__AswSWMaxProgrAttempts)

/****************************************************************************/
/* logical block index 2 -- application */

/* Internal information for each block */
#define d_CCnvm_READ__ParrotSWProgState             (UBYTE) 0x40u
#define d_CCnvm_WRITE_ParrotSWProgState             (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWProgState)
/*------+------------------------------+---------------------*/
/* Identification Data for logical Blocks */
/* 0xF1A6 */
#define d_CCnvm_READ__ParrotSWProgDate              (UBYTE) 0x41u
#define d_CCnvm_WRITE_ParrotSWProgDate              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWProgDate)

/* 0xF184 */
#define d_CCnvm_READ__ParrotSWRepairShopCodeSerNo   (UBYTE) 0x42u
#define d_CCnvm_WRITE_ParrotSWRepairShopCodeSerNo   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWRepairShopCodeSerNo)

/* 0xF1AB */
#define d_CCnvm_READ__ParrotSWLogicalBlockVersion   (UBYTE) 0x43u
#define d_CCnvm_WRITE_ParrotSWLogicalBlockVersion   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWLogicalBlockVersion)

/* 0x0407 */
#define d_CCnvm_READ__ParrotSWCntProgrAttempts      (UBYTE) 0x44u
#define d_CCnvm_WRITE_ParrotSWCntProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWCntProgrAttempts)

/* 0x0408  */
#define d_CCnvm_READ__ParrotSWSucProgrAttempts      (UBYTE) 0x45u
#define d_CCnvm_WRITE_ParrotSWSucProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWSucProgrAttempts)

/* 0x040F */
#define d_CCnvm_READ__ParrotSWMaxProgrAttempts      (UBYTE) 0x46u
#define d_CCnvm_WRITE_ParrotSWMaxProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__ParrotSWMaxProgrAttempts)
/****************************************************************************/
/* logical block index 3 -- application */

/* Internal information for each block */
#define d_CCnvm_READ__DataSWProgState             (UBYTE) 0x50u
#define d_CCnvm_WRITE_DataSWProgState             (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWProgState)
/*------+------------------------------+---------------------*/
/* Identification Data for logical Blocks */
/* 0xF1A6 */
#define d_CCnvm_READ__DataSWProgDate              (UBYTE) 0x51u
#define d_CCnvm_WRITE_DataSWProgDate              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWProgDate)

/* 0xF184 */
#define d_CCnvm_READ__DataSWRepairShopCodeSerNo   (UBYTE) 0x52u
#define d_CCnvm_WRITE_DataSWRepairShopCodeSerNo   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWRepairShopCodeSerNo)

/* 0xF1AB */
#define d_CCnvm_READ__DataSWLogicalBlockVersion   (UBYTE) 0x53u
#define d_CCnvm_WRITE_DataSWLogicalBlockVersion   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWLogicalBlockVersion)

/* 0x0407 */
#define d_CCnvm_READ__DataSWCntProgrAttempts      (UBYTE) 0x54u
#define d_CCnvm_WRITE_DataSWCntProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWCntProgrAttempts)

/* 0x0408  */
#define d_CCnvm_READ__DataSWSucProgrAttempts      (UBYTE) 0x55u
#define d_CCnvm_WRITE_DataSWSucProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWSucProgrAttempts)

/* 0x040F */
#define d_CCnvm_READ__DataSWMaxProgrAttempts      (UBYTE) 0x56u
#define d_CCnvm_WRITE_DataSWMaxProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__DataSWMaxProgrAttempts)

/****************************************************************************/
/* logical block index 4 -- eeprom */

/* Internal information for each block */
#define d_CCnvm_READ__EepromSWProgState             (UBYTE) 0x60u
#define d_CCnvm_WRITE_EepromSWProgState             (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWProgState)
/*------+------------------------------+---------------------*/
/* Identification Data for logical Blocks */
/* 0xF1A6 */
#define d_CCnvm_READ__EepromSWProgDate              (UBYTE) 0x61u
#define d_CCnvm_WRITE_EepromSWProgDate              (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWProgDate)

/* 0xF184 */
#define d_CCnvm_READ__EepromSWRepairShopCodeSerNo   (UBYTE) 0x62u
#define d_CCnvm_WRITE_EepromSWRepairShopCodeSerNo   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWRepairShopCodeSerNo)

/* 0xF1AB */
#define d_CCnvm_READ__EepromSWLogicalBlockVersion   (UBYTE) 0x63u
#define d_CCnvm_WRITE_EepromSWLogicalBlockVersion   (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWLogicalBlockVersion)

/* 0x0407 */
#define d_CCnvm_READ__EepromSWCntProgrAttempts      (UBYTE) 0x64u
#define d_CCnvm_WRITE_EepromSWCntProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWCntProgrAttempts)

/* 0x0408  */
#define d_CCnvm_READ__EepromSWSucProgrAttempts      (UBYTE) 0x65u
#define d_CCnvm_WRITE_EepromSWSucProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWSucProgrAttempts)

/* 0x040F */
#define d_CCnvm_READ__EepromSWMaxProgrAttempts      (UBYTE) 0x66u
#define d_CCnvm_WRITE_EepromSWMaxProgrAttempts      (UBYTE) (d_CCglo_Write | d_CCnvm_READ__EepromSWMaxProgrAttempts)

/* -------------------------------- */
/* Defines for internal nvm handles */

#define d_CCnvm_READ__TIMELOCK                    (UBYTE) 0x70u
#define d_CCnvm_WRITE_TIMELOCK                    (UBYTE) (d_CCglo_Write | d_CCnvm_READ__TIMELOCK)

#define d_CCnvm_READ__SysProgrammed               (UBYTE) 0x71u
#define d_CCnvm_WRITE_SysProgrammed               (UBYTE) (d_CCglo_Write | d_CCnvm_READ__SysProgrammed)

#ifdef cs_CChal_variable_CANID
#define d_CCnvm_READ__CanId                       (UBYTE) 0x72u
#define d_CCnvm_WRITE_CanId                       (UBYTE) (d_CCglo_Write | d_CCnvm_READ__CanId)
#endif
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
