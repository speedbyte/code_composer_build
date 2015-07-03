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
|  Filename:  ADLATUS_CusInterfaceAudi_tdf.h                                   |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the interface between SMART ADLATUS and the customer software.   |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CusInterfaceAudi_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_CusInterfaceAudi_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_CUSINTERFACEAUDI_
#define __TDF_CUSINTERFACEAUDI_
/* ========================================================================== */

typedef struct  /* Service 0x22 ReadDataByIdentifier */
{
  /* 0xF1DF -- */ UBYTE ECUProgramInfo_UB;
  /* 0x0405 -- */ UBYTE StateOfFlash_UB;
  /* 0xF191 -- */ UBYTE ECUHardwareNbr_AUB[11];
  /* 0xF1A3 -- */ UBYTE ECUHardwareVersionNbr_AUB[3];
  /* 0xF187 -- */ UBYTE VWSparePartNbr_AUB[11];
  /* 0xF189 -- */ UBYTE AppSWversionNbr[4];  
  /* 0xF1DF -- */ UBYTE ECUSerialNumber_AUB[14];
  /* 0xF190 -- */ UBYTE VIN_AUB[17];
  /* 0xF197 -- */ UBYTE SysNameEngineType_AUB[13];
  /* 0xF19E -- */ UBYTE AsamOdxFileID_AUB[24];
  /* 0xF1A2 -- */ UBYTE AsamOdxFileVersion_AUB[6];
} FSW_ECUIdent_ST;

typedef struct  /* Service 0x22 ReadDataByIdentifier */
{
  UBYTE ProgState_AUB[1];
  UBYTE SoftwareVersionNbr_AUB[4];
  UBYTE RepairShopCodeSerNo_AUB[6];
  UBYTE ProgDate_AUB[3];
  UBYTE CntProgrAttempts_AUB[2];
  UBYTE SucProgrAttempts_AUB[2];
  UBYTE MaxProgrAttempts_AUB[2];  
} FSW_SWBlockIdent_ST;




typedef struct
{
  UBYTE FPC_Kennung_AUB[6];  /* remeber to adjust the define                  */
                             /* 'd_CCconprj_LengthRepairShopCode' in the      */
                             /* cdf-header file if changing the length of the */
                             /* fingerprint!                                  */

  UBYTE SecAccess_UB;             /* Byte4: Bit 0..3 = Zaehler */
                                  /* Bit7 = Sperrfag- SecurityAccess */

  UBYTE ProgDate_AUB[3];        /* remember to adjust the define                 */
                                /* 'd_CCconprj_LengthProgrammingDate' in the     */
                                /* cdf-header file if changing the length of the */
                                /* programming date!                             */
} ECUDataAppl_ST;



/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */

extern ECUDataAppl_ST sECUDataAppl_ST;

extern const UBYTE c_CCcusint_AdlatusVersion_AUB[];

extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeBootIdent_ST;
extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeAppIdent_ST;
extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeDataIdent_ST;
extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeEepromIdent_ST;
extern const FSW_ECUIdent_ST C_CCcusint_AlternativeECUIdent_ST;
extern const UBYTE C_CCcusint_AlternativeTimelock_UB;
extern const UBYTE C_CCcusint_AlternativeSysProgrammed_UB;

extern const UBYTE C_CCcusint_FDS_AUB[];

/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
