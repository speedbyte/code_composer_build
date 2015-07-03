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
|  Filename:  ADLATUS_CusInterfaceAudi_cdf.h                                   |
|                                                                              |
|  Comment:   This file includes all basic defines for the interface between   |
|             SMART ADLATUS and the customer software.                         |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   RS         Ralf Schmidgall           SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CusInterfaceAudi_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/projekte/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/2sw/5int/01_SW_Projekt/05_header/ext/ADLATUS_CusInterfaceAudi_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_CUSINTERFACEAUDI_
#define __CDF_CUSINTERFACEAUDI_
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Flash Request Pattern                                                      */
/*----------------------------------------------------------------------------*/
#define d_CCcusint_FlashRequestAudi           0x55AAF00FUL
#define d_CCcusint_EcuResetRequestAudi        0x55AA0FF0UL
#define d_CCcusint_DefSessionRequestAudi      0x55AA0AA0UL  /* [OPL286] */


/* Parrot adresses in EEPROM */
#define PARROTSWPROGSTATE                       0x110u /* 1 */
#define PARROTSWMAXPROGRATTEMPTS                0x111u /* 2 */
#define PARROTSWSUCPROGRATTEMPTS                0x113u /* 2 */
#define PARROTSWCNTPROGRATTEMPTS                0x115u /* 2 */
#define PARROTSWREPAIRSHOPCODESERNO             0x117u /* 6 */
#define PARROTSWLOGICALBLOCKVERSION             0x11Du /* 4 */
#define PARROTSWPROGDATE                        0x121u /* 3 */

#define d_CCcusint_VWSparePartNbrLength                     0x0Bu
#define d_CCcusint_ECUHardwareNbrLength                     0x0Bu
#define d_CCcusint_ECUHardwareVersionNbrLength              0x03u
#define d_CCcusint_AsamOdxFileIDLength                      0x17u
#define d_CCcusint_AsamOdxFileVersionLength                 0x06u
#define d_CCcusint_DataSetNbrLength                         0x0Bu
#define d_CCcusint_DataVersionNbrLength                     0x04u
#define d_CCcusint_VWWorkshopSystemNameLength               0x05u
#define d_CCcusint_VehEquipCodePRNbrLength                  0x08u
#define d_CCcusint_ECUProgramInfoLength                     0x01u
#define d_CCcusint_SysNameEngineTypeLength                  0x0Du
#define d_CCcusint_RepairShopCodeTSNbrLength                0x06u
#define d_CCcusint_ProgDateLength                           0x03u
#define d_CCcusint_StateOfFlashLength                       0x01u
#define d_CCcusint_ECUSerialNumberLength                    0x0Eu
#define d_CCcusint_VINLength                                0x11u
#define d_CCcusint_VWFazitSeriennummerLength                0x17u


#define d_CCcusint_VWSparePartNbrEEaddress                  0x000u
#define d_CCcusint_AppSoftwareVersionNbr                    0x00Bu
#define d_CCcusint_ECUHardwareNbrEEaddress                  0x00Fu
#define d_CCcusint_ECUHardwareVersionNbrEEaddress           0x01Au
#define d_CCcusint_AsamOdxFileIDEEaddress                   0x01Du
#define d_CCcusint_AsamOdxFileVersionEEaddress              0x035u
#define d_CCcusint_DataSetNbrEEaddress                      0x03Bu
#define d_CCcusint_DataVersionNbrEEaddress                  0x046u
#define d_CCcusint_VWWorkshopSystemNameEEaddress            0x04Au
#define d_CCcusint_VehEquipCodePRNbr                        0x04Fu
#define d_CCcusint_ECUProgramInfo                           0x057u
#define d_CCcusint_SysNameEngineType                        0x058u
#define d_CCcusint_VWFazitSeriennummerEEaddress             0x075u
#define d_CCcusint_RepairShopCodeTSNbrEEaddress             0x167u
#define d_CCcusint_ProgDate                                 0x179u
#define d_CCcusint_StateOfFlash                             0x065u
#define d_CCcusint_ECUSerialNumberEEaddress                 0x066u
/* #define d_CCcusint_VINEEaddress                             VIN */
#define d_CCcusint_BootSWProgDate                           0x176u
#define d_CCcusint_AswSWProgDate                            0x179u
#define d_CCcusint_DataSWProgDate                           0x17Cu
#define d_CCcusint_EepromSWProgDate                         0x17Fu
#define d_CCcusint_ParrotSWProgDate                         0x182u
#define d_CCcusint_BootSWRepairShopCodeSerNo                0x185u
#define d_CCcusint_AswSWRepairShopCodeSerNo                 0x18Bu
#define d_CCcusint_DataSWRepairShopCodeSerNo                0x191u
#define d_CCcusint_EepromSWRepairShopCodeSerNo              0x197u
#define d_CCcusint_ParrotSWRepairShopCodeSerNo              0x19Du
#define d_CCcusint_BootSWLogicalBlockVersion                0x1A3u
#define d_CCcusint_AswSWLogicalBlockVersion                 0x1A7u
#define d_CCcusint_DataSWLogicalBlockVersion                0x1ABu
#define d_CCcusint_EepromSWLogicalBlockVersion              0x1AFu
#define d_CCcusint_ParrotSWLogicalBlockVersion              0x1B3u
#define d_CCcusint_BootSWCntProgrAttempts                   0x1B7u
#define d_CCcusint_AswSWCntProgrAttempts                    0x1B9u
#define d_CCcusint_DataSWCntProgrAttempts                   0x1BBu
#define d_CCcusint_EepromSWCntProgrAttempts                 0x1BDu
#define d_CCcusint_ParrotSWCntProgrAttempts                 0x1BFu
#define d_CCcusint_BootSWSucProgrAttempts                   0x1C1u                               
#define d_CCcusint_AswSWSucProgrAttempts                    0x1C3u                               
#define d_CCcusint_DataSWSucProgrAttempts                   0x1C5u                               
#define d_CCcusint_EerpomSWSucProgrAttempts                 0x1C7u                               
#define d_CCcusint_ParrotSWSucProgrAttempts                 0x1C9u                               
#define d_CCcusint_BootSWMaxProgrAttempts                   0x1CBu                               
#define d_CCcusint_AswSWMaxProgrAttempts                    0x1CDu                               
#define d_CCcusint_DataSWMaxProgrAttempts                   0x1CFu                               
#define d_CCcusint_EerpomSWMaxProgrAttempts                 0x1D1u                               
#define d_CCcusint_ParrotSWMaxProgrAttempts                 0x1D3u                               
#define d_CCcusint_BootSWProgState                          0x1D5u                               
#define d_CCcusint_AswSWProgState                           0x1D6u                               
#define d_CCcusint_DataSWProgState                          0x1D7u                               
#define d_CCcusint_EerpomSWProgState                        0x1D8u                               
#define d_CCcusint_ParrotSWProgState                        0x1D9u                               
#define d_CCcusint_TIMELOCK                                 0x1DAu                               
#define d_CCcusint_SysProgrammed                            0x1DBu                               

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif                                 
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

