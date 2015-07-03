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
|  Filename:  ADLATUS_ConfigProject_cdf.h                                      |
|                                                                              |
|  Comment:   This file includes all defines for the current project           |
|             configuration of the SMART ADLATUS.                              |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ConfigProject_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_ConfigProject_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_CONFIGPROJECT_
#define __CDF_CONFIGPROJECT_
/* ========================================================================== */


/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA PMA.01                                              **/
/**     ----------------------------------------------------------------     **/
/**     P R O J E C T   C O N F I G U R A T I O N                            **/
/**     ----------------------------------------------------------------     **/
/**     Project memory addresses                                             **/
/**                                                                          **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* FLASH MEMORY                                                               */
/*----------------------------------------------------------------------------*/
#define  d_CCconprj_PageLength                0x100u

/******************************************************************************/
/* -- -- END OF APPLICATION AREA PMA.01 -- -- -- -- -- -- -- -- ---- -- -- -- */
/******************************************************************************/


/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA   Nvm.03                                            **/
/**     ----------------------------------------------------------------     **/
/**     N V M   C O N F I G U R A T I O N                                    **/
/**     ----------------------------------------------------------------     **/
/**     ReadDataByIdentifier (Service 0x22)                                  **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/



/*----------------------------------------------------------------------------*/
/* 0x22 ReadDataByIdentifier (RecordDataIdentifier)                           */
/*----------------------------------------------------------------------------*/
#define d_CCconprj_ECUProgramInfo             0xF1DFu
#define d_CCconprj_StateOfFlash               0x0405u
#define d_CCconprj_ECUHardwareNbr             0xF191u
#define d_CCconprj_ECUHardwareVersionNbr      0xF1A3u
#define d_CCconprj_SysNameEngineType          0xF197u
#define d_CCconprj_VWSparePartNbr             0xF187u
#define d_CCconprj_AppSoftwareVersionNbr      0xF189u
#define d_CCconprj_AppMaxProgrAttempts        0x0407u
#define d_CCconprj_AppCntProgrAttempts        0x0408u
#define d_CCconprj_AppRepairShopCodeSerNo     0xF184u
#define d_CCconprj_AppProgDate                0xF1A6u

#define d_CCconprj_ECUSerialNumber            0xF18Cu
#define d_CCconprj_AsamOdxFileID              0xF19Eu
#define d_CCconprj_AsamOdxFileVersion         0xF1A2u

#define d_CCconprj_DataSetNbr                 0xF1A0u             
#define d_CCconprj_DataVersionNbr             0xF1A1u          

#define d_CCconprj_VWCodingValue              0x0600u
#define d_CCconprj_VWCodingInformation        0x0601u

#define d_CCconprj_VWWorkshopSystemName       0xF1AAu

#define d_CCconprj_IdentifiedSlaveSystem      0x0606u   
#define d_CCconprj_VehEquipCodePRNbr          0xF1A4u       

#define d_CCconprj_RepairShopCodeTSNbr        0xF198u      
#define d_CCconprj_ProgDate                   0xF199u           
#define d_CCconprj_StateOfFlash               0x0405u           

#define d_CCconprj_ActiveDiagSession          0xF186u       
#define d_CCconprj_VIN                        0xF190u                  

#define d_CCconprj_SWProgDate                 0xF1A6u
#define d_CCconprj_SWRepairShopCodeSerNo      0xF184u
#define d_CCconprj_SWLogicalBlockVersion      0xF1ABu
#define d_CCconprj_SWCntProgrAttempts         0x0407u
#define d_CCconprj_SWSucProgrAttempts         0x0408u  
#define d_CCconprj_SWMaxProgrAttempts         0x040Fu 

#define d_CCconprj_ReadFingerprint            0xF15Bu

#define d_CCconprj_FDSProjectData             0xF1D5u

/*----------------------------------------------------------------------------*/
/* 0x2E WriteDataByIdentifier (RecordDataIdentifier)                          */
/*----------------------------------------------------------------------------*/
#define d_CCconprj_WriteFingerprint           0xF15Au


/*----------------------------------------------------------------------------*/
/* 0x22 ReadDataByIdentifier (max number of multiple requests)                */
/*----------------------------------------------------------------------------*/
#define d_CCconprj_MaxNbrRecordDataIDs        0x0008u

/*----------------------------------------------------------------------------*/
/* 0x22 ReadDataByIdentifier length defines                                   */
/*----------------------------------------------------------------------------*/
#define d_CCconprj_LengthRepairShopCode        0x06u
#define d_CCconprj_LengthProgDate              0x03u
#define d_CCconprj_LengthProgState             0x01u
#define d_CCconprj_LengthBootSWIdent           0x04u 
#define d_CCconprj_LengthECUHardwareNbr        0x0Bu

#define d_CCconprj_LengthFingerprint           (d_CCconprj_LengthProgDate + d_CCconprj_LengthRepairShopCode + d_CCconprj_LengthProgState)

#define d_CCconprj_LengthAppSoftwareVersionNbr 0x04u

#define d_CCconprj_LengthLogSWBlckVersionNbr   0x04u

#define d_CCconprj_LengthCounter               0x02u

/* Index (position of nvm entry in log block array */

#define d_CCconprj_SWProgDateIndex            (UBYTE) 0x00u
#define d_CCconprj_SWRepairShopCodeSerNoIndex (UBYTE) 0x01u
#define d_CCconprj_SWProgStateIndex           (UBYTE) 0x02u
#define d_CCconprj_SWLogicalBlockVersionIndex (UBYTE) 0x03u
#define d_CCconprj_SWCntProgrAttemptsIndex    (UBYTE) 0x04u
#define d_CCconprj_SWSucProgrAttemptsIndex    (UBYTE) 0x05u
#define d_CCconprj_SWMaxProgrAttemptsIndex    (UBYTE) 0x06u

#define d_CCconprj_LengthFDSProjectData        0x06u

/* THIS IS FOR UDS ---------> */
/* -------------------------------------------------------------------------- */
/*      R e a d D a t a B y I d e n t i f i e r  D E F I N E S                */
/*      -----------------------------------------------------------------     */
/*      Flags_UB  =    7 6 5 4  3 2 1 0                                       */
/*                     | | | |  | | | |                                       */
/*                     | | | |  | | | +---  use the alternative addresses     */
/*                     | | | |  | | |         if EEPROM access was not        */
/*                     | | | |  | | |         successful                      */
/*                     | | | |  | | |                                         */
/*                     | | | |  | | +-----  data shall be read from FLASH     */
/*                     | | | |  | |           memory, not from EEPROM.        */
/*                     | | | |  | |                                           */
/*                     | | | |  | |                                           */
/*                     | | | |  | |                                           */
/*                     | | | |  | +-------  not used                          */
/*                     | | | |  +---------  not used                          */
/*                     | | | |                                                */
/*                     | | | +------------  not used                          */
/*                     | | +--------------  not used                          */
/*                     | +----------------  not used                          */
/*                     +------------------  not used                          */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define d_CCconprj_UseAltAddress     (UBYTE) 0x01u
#define d_CCconprj_ReadFromFlash     (UBYTE) 0x02u

#define d_CCconprj_IndexAccess       (UBYTE) 0x04u
#define d_CCconprj_ActiveDiag        (UBYTE) 0x08u
/******************************************************************************/
/* -- -- END OF APPLICATION AREA   Nvm.03  - -- -- -- -- -- -- -- -- -- -- -- */
/******************************************************************************/
/* THIS IS FOR UDS <---------> */


/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA 27                                                  **/
/**     ----------------------------------------------------------------     **/
/**     P R O J E C T   C O N F I G U R A T I O N                            **/
/**     ----------------------------------------------------------------     **/
/**     Error Message Entry for ADLATUS Diagnostic                           **/
/**                                                                          **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/
#define  d_CCconprj_NoError                   0x0000u
#define  d_CCconprj_ChecksumFCNOK             0x0001u
#define  d_CCconprj_ChecksumFSWNOK            0x0002u
/*----------------------------------------------------------------------------*/
/* RAM CHECK ERROR MESSAGE ENTRY                                              */
/*----------------------------------------------------------------------------*/
#define  d_CCconprj_DataRamCheckNOK           0x0100u
#define  d_CCconprj_ProgRamCheckNOK           0x0200u
#define  d_CCconprj_CanRamCheckNOK            0x0400u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*----------------------------------------------------------------------------*/
/* FlashCore test defines                                                     */
/*----------------------------------------------------------------------------*/
#define  d_CCconprj_NoFswInSystem             0x1000u
#define  d_CCconprj_FswInSystem               0x0000u


/******************************************************************************/
/* -- -- END OF APPLICATION AREA 27 -- -- -- -- -- -- -- -- -- -- -- -- -- -- */
/******************************************************************************/

/* ========================================================================== */
/*                                                                            */
/*  P R O J E C T   D E F I N E S                                             */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Project.01                                               */
/*  ------------------------------------------------------------------        */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */
#define d_CCconprj_SessionCtrlRspTG (UBYTE) 0x00u
#define d_CCconprj_ECUResetRspTG    (UBYTE) 0x01u
#define d_CCconprj_DefSessionRspTG  (UBYTE) 0x02u  /* [OPL286] */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/




/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
