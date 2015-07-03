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
|  Filename:  ADLATUS_Kwp2000Audi.c                                            |
|                                                                              |
|  Comment:   This file includes all the provided KWP2000 services for the     |
|             ECU reprogramming protocol handling defined by AUDI.             |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_KWP2000Audi.c 4541 2008-12-02 10:47:09Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/09_audilib/02_src/ADLATUS_KWP2000Audi.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_CompilerSwitches_cdf.h"    /* must be the first CDF-include */
#include "ADLATUS_Global_cdf.h"         /* .. Global defines                  */
#include "ADLATUS_ConfigProject_cdf.h"  /* .. for address information         */
#include "ADLATUS_Kwp2000Base_cdf.h"    /* .. KWP2000 Basic Defines           */
#include "ADLATUS_ConfigSystem_cdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Customer Files                                                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_Kwp2000Audi_cdf.h"   /* .. audi specific KWP2000 defines  */
#include "ADLATUS_SequenceAudi_cdf.h"  /* .. audi specific Sequence defines */
#include "ADLATUS_SecurityAudi_cdf.h"  /* .. audi specific Security         */
#include "ADLATUS_CusInterfaceAudi_cdf.h"   /* .. ECU Identification              */
#include "ADLATUS_NvmHandlerAudi_cdf.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Project Files                                                              */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_EventHandlerAudi_cdf.h"  /* .. Event Handler                */

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"             /* must be the first include       */
#include "ADLATUS_Global_tdf.h"            /* .. Global defines               */
#include "ADLATUS_EventHandlerAudi_tdf.h"  /* Event Handler                   */
#include "ADLATUS_NvmHandler_tdf.h"        /* NVM Access                      */
#include "ADLATUS_Timer_tdf.h"             /* for system timer check          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Customer Files                                                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_Kwp2000Audi_tdf.h"
#include "ADLATUS_SecurityAudi_tdf.h"
#include "ADLATUS_CusInterfaceAudi_tdf.h"
#include "ADLATUS_ValidationAudi_tdf.h"    /* Checksum routines for downloads */
#include "ADLATUS_ValHandler_tdf.h"        /* direct use of on fly calculation */
#include "ADLATUS_ConfigProject_tdf.h"
#include "ADLATUS_DataPrepareAudi_tdf.h"
#include "ADLATUS_ApplicationInterface_tdf.h"
#include "ADLATUS_SSIHandler_tdf.h"

#include "ADLATUS_CheckDependAudi_tdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Project Files                                                              */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_HISflash.h"
#include "ADLATUS_ExternalWatchdog_tdf.h"

#include "ADLATUS_Adr_Info.h"

#include "ADLATUS_Global_CollectData_tdf.h"
/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches: No Entry                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/* (4)  D A T A   S E C T I O N   D I R E C T I V E S                         */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (5)  L O C A L   D E F I N E S                                             */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
#define d_CCkwp_PageWrite

/* -------------------------------------------------------------------------- */
/*      K W P   S T A T E   R E G I S T E R   D E F I N E S                   */
/*      -----------------------------------------------------------------     */
/*       7 6 5 4  3 2 1 0  7 6 5 4  3 2 1 0                                   */
/*       | | | |  | | | |  | | | |  | | | |                                   */
/*       | | | |  | | | |  | | | |  | | | +---  SecurityAccess granted        */
/*       | | | |  | | | |  | | | |  | | +-----  InternFlashDownload           */
/*       | | | |  | | | |  | | | |  | +-------  ExternFlashDownload           */
/*       | | | |  | | | |  | | | |  +---------  RamDownload                   */
/*       | | | |  | | | |  | | | |                                            */
/*       | | | |  | | | |  | | | +------------  RamDownloadDone               */
/*       | | | |  | | | |  | | +--------------  EepromDownload                */
/*       | | | |  | | | |  | +----------------  Routine is busy               */
/*       | | | |  | | | |  +------------------  ChecksumCorrect               */
/*       | | | |  | | | |                                                     */
/*       | | | |  | | | +---------------------  ECUResetPermitted             */
/*       | | | |  | | +-----------------------  ProgramDependenciesOk         */
/*       | | | |  | +-------------------------  ErasingDoneOk                 */
/*       | | | |  +---------------------------  FingerPrintDoneOk             */
/*       | | | |                                                              */
/*       | | | +------------------------------  SeedAlreadyRequested          */
/*       | | +--------------------------------  ReadyforCheckMemory           */
/*       | +----------------------------------  Do2ndRun                      */
/*       +------------------------------------  SecurityAccessDenied          */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define d_CCkwpaudi_InitKwpState                      0x0000u

#define d_CCkwpaudi_FlagSecurityAccessGranted         0x0001u
#define d_CCkwpaudi_FlagInternFlashDownload           0x0002u
#define d_CCkwpaudi_FlagExternFlashDownload           0x0004u
#define d_CCkwpaudi_FlagRamDownload                   0x0008u

#define d_CCkwpaudi_FlagRamDownloadDone               0x0010u
#define d_CCkwpaudi_FlagEepromDownload                0x0020u
#define d_CCkwpaudi_FlagRoutineBusy                   0x0040u
#define d_CCkwpaudi_FlagChecksumCorrect               0x0080u

#define d_CCkwpaudi_FlagRequestDownloadActive         0x0100u   /* NRC != 0x24 */
#define d_CCkwpaudi_FlagProgDependenciesOk            0x0200u
#define d_CCkwpaudi_FlagErasingDoneOk                 0x0400u
#define d_CCkwpaudi_FlagFPDoneOk                      0x0800u    /* [OPL328] */

#define d_CCkwpaudi_FlagSeedAlreadyRequested          0x1000u   /* [OPL308_309] */
#define d_CCkwpaudi_FlagReadyforCheckMemory           0x2000u
#define d_CCkwpaudi_FlagDo2ndRun                      0x4000u
#define d_CCkwpaudi_FlagSecurityAccessDenied          0x8000u

/*----------------------------------------------------------------------------*/
/*      Block size during data transfer                                       */
/*      -----------------------------------------------------------------     */
/*      NOTE: Think about the maximum buffer size !!                          */
/*----------------------------------------------------------------------------*/
 /* only DataLength */
#define  d_CCkwpaudi_MaxBlockLengthInternFlashDownload d_CCconsys_RxBufferLength00
 /* only DataLength */
#define  d_CCkwpaudi_MaxBlockLengthExternFlashDownload d_CCconsys_RxBufferLength00
 /* only DataLength */
#define  d_CCkwpaudi_MaxBlockLengthRamDownload         d_CCconsys_RxBufferLength00
 /* only DataLength */
#define  d_CCkwpaudi_MaxBlockLengthEepromDownload      d_CCconsys_RxBufferLength00
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*----------------------------------------------------------------------------*/
/*      SID $28 - Communication Control                                       */
/*      ---------------------------------------------------------             */
/*      Communication Type                                                    */
/*----------------------------------------------------------------------------*/
#define  d_CCkwpaudi_CommunicationTypeMask        (UBYTE)  0x03u
#define  d_CCkwpaudi_normalCommunicationMessages  (UBYTE)  0x01u

/*----------------------------------------------------------------------------*/
/*      SID $22 - ReadDataByIdentifier                                        */
/*      ---------------------------------------------------------             */
/*      number of entries belonging to a fingerprint                          */
/*----------------------------------------------------------------------------*/
#define d_CCkwpaudi_NbrFingerprintHandles         (UBYTE)  0x03u

/*----------------------------------------------------------------------------*/
/*      SID $31 - RoutineControl CheckMemory                                  */
/*      ---------------------------------------------------------             */
/*      programming state defines                                             */
/*----------------------------------------------------------------------------*/
#define d_CCkwpaudi_ProgStateSWInValid           (UBYTE)  0x01u
#define d_CCkwpaudi_ProgStateSWValid             (UBYTE)  0x00u

/*----------------------------------------------------------------------------*/
/* number of bytes to be erased at once in the eeprom                         */
/*----------------------------------------------------------------------------*/
#define d_CCkwpaudi_MaxEraseBytesEeprom          (UBYTE)  0x30u

/*----------------------------------------------------------------------------*/
/* erase pattern for eeprom erasing                                           */
/*----------------------------------------------------------------------------*/
#define d_CCkwpaudi_EepromErasePattern           (UBYTE)  0xFFu

/*----------------------------------------------------------------------------*/
/* packed length for signature check                                          */
/*----------------------------------------------------------------------------*/
#define d_CCkwpaudi_SignaturePackedLength        (UBYTE)  0x20u

/*----------------------------------------------------------------------------*/
/* validation depending on usage of crypto functions                          */
/*----------------------------------------------------------------------------*/
/* Signature | Decryption | DataFormatId | validation                         */
/*----------------------------------------------------------------------------*/
/*     -     |      -     |     0x00     | CRC32 at CheckMemory               */
/*----------------------------------------------------------------------------*/
/*     +     |      -     |     0x00     | Signature at CheckMemory           */
/*----------------------------------------------------------------------------*/
/*     +     |      +     |     0x01     | Signature during TransferData      */
/*           |            |              | CRC32 at CheckMemory               */
/*----------------------------------------------------------------------------*/
/*     -     |      +     |                   NOT ALLOWED                     */
/*----------------------------------------------------------------------------*/
#ifdef cs_Decryption_Aktiv
  #define d_CCkwpaudi_DoCrcAfterWrite
#else
  #ifndef cs_Signatur_Aktiv
    #define d_CCkwpaudi_DoCrcAfterWrite
  #endif
#endif




/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
typedef struct _KWP2000audiCONTROL
{
  ULONG   EraseStartAddress_UL; /*  4 Byte - Erase start address              */
  ULONG   EraseEndAddress_UL;   /*  4 Byte - Erase end address                */
  ULONG   EraseLength_UL;       /*  4 Byte - Erase length                     */

  UBYTE   EraseStartSector_UB;  /*  1 Byte - Erase start sector               */
  UBYTE   EraseEndSector_UB;    /*  1 Byte - Erase end sector                 */
  UBYTE   ActEraseSector_UB;    /*  1 Byte - current erase sector             */
  UBYTE   Reserved01_UB;        /*  1 Byte - reserved for ULONG alignment     */

  ULONG   ProgStartAddress_UL;  /*  4 Byte - Programming start address        */
  ULONG   ProgEndAddress_UL;    /*  4 Byte - Programming end address          */
  ULONG   ProgLength_UL;        /*  4 Byte - Programming data length          */
  ULONG   ActProgAddress_UL;    /*  4 Byte - Actual programming address       */

  ULONG   FFStartAddress_UL;    /*  4 Byte - FlashFunctions start addres      */
  ULONG   FFEndAddress_UL;      /*  4 Byte - FlashFunctions end addres        */

  ULONG   DLStartAddress_UL;    /*  4 Byte - Download start address           */
  ULONG   DLEndAddress_UL;      /*  4 Byte - Download end address             */

  UBYTE   DataFormatId_UB;      /*  1 Byte - Data format identifier           */
  UBYTE   MaxBlockLength_UB;    /*  1 Byte - block size during data transfer  */
  UBYTE   AddressPatch_UB;      /*  1 Byte - reserved for ULONG alignment     */
  UBYTE   BlockSeqCounter_UB;   /*  1 Byte - Block sequence counter           */


  UBYTE   DownloadMapIndex_UB;  /*  1 Byte - for storing the map index        */
  UBYTE   ValidationMapIndex_UB;/*  1 Byte - for storing the map index        */
  UWORD   State_UW;             /*  2 Byte - Kwp2000 state register           */

  ULONG   Tester_Checksum_UL;

  ULONG   Seed_UL;              /*  4 Byte - Seed of the S&K algorithm        */   /* [OPL308_309] */

  CMDRUNTIMEINFO_ST*  CmdRunTimeInfo_PST[1];
                                /*  4 Byte - Pointer to Command Runtime info  */
                                /* ------------------------------------------ */

  UWORD   MemoryInfo_UW;        /*  2 Byte - Memory information from MemoryMap*/

  const DOWNLOADMAP_ST* DownloadMap_PST;    /*  2 Byte - Ptr to download map of      */
                                         /*           config project              */
                                         /* ------------------------------------- */
  const VALIDATIONMAP_ST  *ValidationMap_PST;  /*  2 Byte - Ptr to validation map of    */
                                         /*           config project              */
                                         /* ------------------------------------- */

  const ADDRESSMODECONFIG_ST *AddrModeCfg_PST; /*  2 Byte - Ptr to address mode config  */
                                         /*           of config project           */
                                         /* ------------------------------------- */

  UBYTE   ActiveSession_UB;
  UBYTE   RoutineCallCounter_UB;

} KWP2000audiCONTROL_ST;        /* 44 Byte                                    */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
UWORD const c_VerSMART_Audilib_V113_UW = 0x0113u;

KWP2000audiCONTROL_ST  t_CCkwpaudi_KwpCtrl_ST; /* KWP2000 Control Structure */

ECUDataAppl_ST sECUDataAppl_ST;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/*UBYTE FUN_CCkwpaudi_CheckAddrMode_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \     */
/*                                       UBYTE t_RequAddrMode_UB);            */

UBYTE FUN_CCkwpaudi_EraseEeprom_UB(void);

UBYTE FUN_CCkwpaudi_AddressToMemoryMapIndex_UB ( ULONG t_StartAddress_UL, \
                                                 ULONG t_EndAddress_UL );
UBYTE FUN_CCkwpaudi_UpdateEraseBlockVal_UB ( UBYTE t_Index_UB );
UBYTE FUN_CCkwpaudi_UpdateBlockValadity_UB ( UBYTE t_ValidationIndex_UB, \
                                             UBYTE t_ProgrammigState_UB );

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
#ifdef cs_Signatur_Aktiv

  /*--------------------------------------------------------------------------*/
  /* context for signature calculation and signature verification             */
  /*--------------------------------------------------------------------------*/
  SecM_SignatureParamType g_SignatureType;

  /*--------------------------------------------------------------------------*/
  /* buffer for current hash value                                            */
  /*--------------------------------------------------------------------------*/
  UBYTE g_VerSigSigResultBuffer_AUB[SECM_MIN_SIGNATURE_TYPE_LENGTH];

  /*--------------------------------------------------------------------------*/
  /* current length of hashed data                                            */
  /*--------------------------------------------------------------------------*/
  ULONG g_VerSigCurrentDataLength_UL = 0x00ul;

#endif


#ifdef cs_Decryption_Aktiv

  /*--------------------------------------------------------------------------*/
  /* configured IV defined within other module                                */
  /*--------------------------------------------------------------------------*/
  extern const UBYTE g_DecryptIv_AUB[];

  /*--------------------------------------------------------------------------*/
  /* used IV for decryption (initialized within this module,                  */
  /* defined within other module                                              */
  /*--------------------------------------------------------------------------*/
  extern UBYTE g_DecryptIvTemp_AUB[];

#endif

/*----------------------------------------------------------------------------*/
/* used for buffer status during download of eeprom data                      */
/*----------------------------------------------------------------------------*/
UBYTE g_CCkwpaudi_EepromDataLeftOver_UB;
/*----------------------------------------------------------------------------*/
/* used for buffer on-the-fly crc calculation, set to init value.             */
/*----------------------------------------------------------------------------*/
ULONG g_CCkwpaudi_ParrotCRC32_UL = 0x00000000u;

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/* INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* Use the "Request Length Error" - Routine and the "Sequence Error" -        */
/* Routine from KWP2000Base.c . This is only for code reduction !!            */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/* INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO INFO */
/* -------------------------------------------------------------------------- */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwp_InitKwp2000_V                                     |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                   |
|                                                                              |
|  Description:    Initfunction for the KWP2000 layer.                         |
|                                                                              |
|  Parameter:      KWP2000INTERFACE* Interface_PST ... Ptr to interface        |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCkwpaudi_InitKwp2000_V (const KWP2000INTERFACE_ST* KWP2000Interface_PST)
{
  t_CCkwpaudi_KwpCtrl_ST.CmdRunTimeInfo_PST[0] = \
                                    KWP2000Interface_PST->CmdRunTimeInfo_PST;

  /*--------------------------------------------------------------------------*/
  /* Init Pointer to Download Map                                             */
  /*--------------------------------------------------------------------------*/
  t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST = KWP2000Interface_PST->DownloadMap_PST;

  /*--------------------------------------------------------------------------*/
  /* Init Pointer to Validation Map                                           */
  /*--------------------------------------------------------------------------*/
  t_CCkwpaudi_KwpCtrl_ST.ValidationMap_PST = KWP2000Interface_PST->ValidationMap_PST;

  /*--------------------------------------------------------------------------*/
  /* Init Pointer to Address Mode Config                                      */
  /*--------------------------------------------------------------------------*/
  t_CCkwpaudi_KwpCtrl_ST.AddrModeCfg_PST = KWP2000Interface_PST->AddrModeCfg_PST;

  FUN_CCkwpaudi_InitKwp2000internal_V();

}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_InitKwp2000internal_V                         |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                   |
|                                                                              |
|  Description:    Initfunction for the KWP2000 layer.                         |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCkwpaudi_InitKwp2000internal_V(void)
{
  /*--------------------------------------------------------------------------*/
  /* Init Pointer to RuntimeInfo                                              */
  /*                                                                          */
  /* (01) EraseStartAddress_UL                                                */
  /* (02) EraseEndAddress_UL                                                  */
  /* (03) EraseLength_UL                                                      */
  /* (04) EraseStartSector_UB                                                 */
  /* (05) EraseEndSector_UB                                                   */
  /* (06) ActEraseSector_UB                                                   */
  /* (07) Reserved01_UB                                                       */
  /* (08) ProgStartAddress_UL                                                 */
  /* (09) ProgEndAddress_UL                                                   */
  /* (10) ProgLength_UL                                                       */
  /* (11) ActProgAddress_UL                                                   */
  /* (12) State_UW                                                            */
  /* (13) DataFormatId_UB                                                     */
  /* (14) MaxBlockLength_UB                                                   */
  /* (15) Reserved02_UB                                                       */
  /* (16) CmdRunTimeInfo_PST[..]                                              */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL   = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.EraseLength_UL       = (ULONG) d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.EraseStartSector_UB  = d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.EraseEndSector_UB    = d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.ActEraseSector_UB    = d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL  = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL    = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL        = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL    = (ULONG) d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.State_UW             = (UWORD) d_CCkwpaudi_InitKwpState;
  t_CCkwpaudi_KwpCtrl_ST.DataFormatId_UB      = d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.MaxBlockLength_UB    = d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB   = d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.Tester_Checksum_UL   = (ULONG) d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.Seed_UL              = (ULONG) d_CCglo_InitToZero;   /* [OPL308_309] */

  t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW        = (UWORD) d_CCglo_InitToZero;


  /* Init the address info for deinit the flashfunctions later on      */
  t_CCkwpaudi_KwpCtrl_ST.FFStartAddress_UL    = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.FFEndAddress_UL      = (ULONG) d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.DLStartAddress_UL    = (ULONG) d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.DLEndAddress_UL      = (ULONG) d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.DownloadMapIndex_UB   = d_CCglo_InitToZero;
  t_CCkwpaudi_KwpCtrl_ST.ValidationMapIndex_UB = d_CCglo_InitToZero;

  t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB      = (UBYTE) 0x01u;
  t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = (UBYTE) 0x00u;

  /*==========================================================================*/
  /* -1- check if the programming session is active because of a flash        */  /* [OPL287] */
  /*     request.                                                             */
  /*==========================================================================*/
  if (d_CCglo_AllowedInProgSession == t_CCkwpaudi_KwpCtrl_ST.CmdRunTimeInfo_PST[0u]->AllowedSessions_UB )
  {
    /*------------------------------------------------------------------------*/
    /* Store active session information                                       */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB = d_CCkwpbase_LIDProgSession;

  } /* --1- END OF if(..) ----------------------------------------------------*/


  return;

} /*-+- END OF FUN_CCkwp_InitKwp2000_V -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_DSCDefaultSession_UB                         |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $10 - Diagnostic Session Control               |
|                  Local ID  : $01 - Default Session                          |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/  /* [OPL286] */
UBYTE FUN_CCkwpaudi_DSCDefaultSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                           PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /* The next line is only to prevent a compiler warning!!                    */
  t_InBuffer_PBUF[0u] |= (UBYTE) 0x00u;

  /*==========================================================================*/
  /* -1- we are already in the default session                                */
  /*==========================================================================*/
  if ( d_CCseqaudi_DefaultSession == t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB)
  {
    /*------------------------------------------------------------------------*/
    /* Set the DLC for the OutBuffer                                          */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC]                 = d_CCglo_06Byte;

    /*------------------------------------------------------------------------*/
    /* Send the positiv Response                                              */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x00u]  = d_CCkwpbase_SIDDiagSessionControl + (UBYTE) 0x40u;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]  = d_CCkwpbase_LIDDefaultSession;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u]  = c_CCconprj_DscServerTiming_ST.P2DefCanServerHigh;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x03u]  = c_CCconprj_DscServerTiming_ST.P2DefCanServerLow;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x04u]  = c_CCconprj_DscServerTiming_ST.P2EnhCanServerHigh;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x05u]  = c_CCconprj_DscServerTiming_ST.P2EnhCanServerLow;

    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

    /*========================================================================*/
    /* -2- positiv response shall be suppressed                               */
    /*========================================================================*/
    if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                    t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]) )
    {
      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* init state                                                             */
    /*------------------------------------------------------------------------*/
    FUN_CCkwpaudi_InitKwp2000internal_V();

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- we are not in the default session                                    */
  /*==========================================================================*/
  else
  {

    /*========================================================================*/
    /* -2- Function was called a first time                                   */
    /*========================================================================*/
    if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
      d_CCkwpaudi_FlagRoutineBusy )
    {
      /*----------------------------------------------------------------------*/
      /* Set the internal busy flag                                           */
      /*----------------------------------------------------------------------*/
      t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

      /*----------------------------------------------------------------------*/
      /* response pending                                                     */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex00);

    } /* -------2- END OF if(..) ---------------------------------------------*/

    /*========================================================================*/
    /* -2- Function was called one more time                                  */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* Reset the internal busy flag                                         */
      /*----------------------------------------------------------------------*/
      t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

      /*----------------------------------------------------------------------*/
      /* clear the flash functions                                            */
      /*----------------------------------------------------------------------*/
      FUN_CCkwpaudi_ClearFlashFunctions_V();

      /*----------------------------------------------------------------------*/
      /* suppress positive response                                           */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = (UWORD)d_CCglo_00Byte;

      /*----------------------------------------------------------------------*/
      /* write the ECU reset request pattern                                  */
      /*----------------------------------------------------------------------*/
      FUN_CCapplint_SetDefSessionRequ_V();

      /*----------------------------------------------------------------------*/
      /* Set reset flag                                                       */
      /*----------------------------------------------------------------------*/
      FUN_CCeventaudi_EventHandler_B ( d_CCeventaudi_SET_____SystemReset_UB );

    } /* -------2- END OF else(..) -------------------------------------------*/

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_DSCDefaultSession_UB          -+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $10 - Diagnostic session control                             */
/*  LOCAL ID   : $01 - Default Session                                        */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTDSCDefaultSession_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 02 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDDiagSessionControl,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 05 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 06 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseq_DefaultSessionDone,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_DSCProgrammingSession_UB                     |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $10 - Diagnostic Session Control               |
|                  Local ID  : $02 - Programming Session                      |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_DSCProgSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                        PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set channel information                                                  */
  /*--------------------------------------------------------------------------*/

  /* The next line is only to prevent a compiler warning!!                    */
  t_InBuffer_PBUF[0u] |= (UBYTE) 0x00u;


  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC]                 = d_CCglo_06Byte;


  /* first run through the function ? */
  if(d_CCkwpaudi_FlagDo2ndRun != (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagDo2ndRun))
  {
    /* set flag for next run */
    t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagDo2ndRun;

    t_Result_UB = (d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex01);
  }
  /*Second run through function -- now send the real response */
  else
  {

    /*--------------------------------------------------------------------------*/
    /* Send the positiv Response                                                */
    /*--------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x00u]  = d_CCkwpbase_SIDDiagSessionControl + (UBYTE) 0x40u;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]  = d_CCkwpbase_LIDProgSession;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u]  = c_CCconprj_DscServerTiming_ST.P2DefCanServerHigh;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x03u]  = c_CCconprj_DscServerTiming_ST.P2DefCanServerLow;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x04u]  = c_CCconprj_DscServerTiming_ST.P2EnhCanServerHigh;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x05u]  = c_CCconprj_DscServerTiming_ST.P2EnhCanServerLow;

    /*--------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state        */
    /*                                                                          */
    /* REMARK: it is not necessary to look for response supression because of   */
    /* the formerly sent response pending                                       */
    /*--------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagDo2ndRun;

    /*--------------------------------------------------------------------------*/
    /* clear the flash functions                                                */
    /*--------------------------------------------------------------------------*/
    FUN_CCkwpaudi_ClearFlashFunctions_V();

    /*--------------------------------------------------------------------------*/
    /* init kwp2000 state                                                       */
    /*--------------------------------------------------------------------------*/
    FUN_CCkwpaudi_InitKwp2000internal_V();
    t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagSecurityAccessDenied;
    /*------------------------------------------------------------------*/
    /* communication is active any communication break leads to a       */
    /* timeout                                                          */
    /*------------------------------------------------------------------*/
    FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____OpenChannel_UB);

    /*--------------------------------------------------------------------------*/
    /* Store active session information                                         */
    /*--------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.CmdRunTimeInfo_PST[0u]->AllowedSessions_UB = \
      d_CCglo_AllowedInProgSession;

    t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB = d_CCkwpbase_LIDProgSession;
  }

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_DSCProgSession_UB             -+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $10 - Start Diagnostic Session                               */
/*  LOCAL ID   : $02 - Programming Session                                    */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTDSCProgSession_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_ProgSessionDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCglo_NoPostCondDefined,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 04 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 05 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDDiagSessionControl,
  /* 06 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending

}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_DSCExtendedSession_UB                        |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $10 - Diagnostic Session Control               |
|                  Local ID  : $03 - Extended Session                         |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_DSCExtendedSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                            PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set channel information                                                  */
  /*--------------------------------------------------------------------------*/

  /* The next line is only to prevent a compiler warning!!                    */
  t_InBuffer_PBUF[0u] |= (UBYTE) 0x00u;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC]                 = d_CCglo_06Byte;

  /*--------------------------------------------------------------------------*/
  /* Send the positiv Response                                                */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x00u]  = d_CCkwpbase_SIDDiagSessionControl + (UBYTE) 0x40u;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]  = d_CCkwpbase_LIDExtendedSession;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u]  = c_CCconprj_DscServerTiming_ST.P2DefCanServerHigh;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x03u]  = c_CCconprj_DscServerTiming_ST.P2DefCanServerLow;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x04u]  = c_CCconprj_DscServerTiming_ST.P2EnhCanServerHigh;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x05u]  = c_CCconprj_DscServerTiming_ST.P2EnhCanServerLow;

  /*--------------------------------------------------------------------------*/
  /* set return value - will be overwritten by an error or other state        */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  /*==========================================================================*/
  /* -1- positiv response shall be suppressed                                 */
  /*==========================================================================*/
  if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                  t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]) )
  {
    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* init state                                                               */
  /*--------------------------------------------------------------------------*/
  FUN_CCkwpaudi_InitKwp2000internal_V();

  /*------------------------------------------------------------------*/
  /* communication is active any communication break leads to a       */
  /* timeout                                                          */
  /*------------------------------------------------------------------*/
  FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____OpenChannel_UB);

  /*--------------------------------------------------------------------------*/
  /* Store active session information                                         */
  /*--------------------------------------------------------------------------*/
  t_CCkwpaudi_KwpCtrl_ST.CmdRunTimeInfo_PST[0u]->AllowedSessions_UB = \
    d_CCglo_AllowedInExtSession;

  t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB = d_CCkwpbase_LIDExtendedSession;
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_DSCExtendedSession_UB         -+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $10 - Start Diagnostic Session                               */
/*  LOCAL ID   : $03 - Extended Session                                       */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTDSCExtendedSession_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_ExtendedSessionDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_ControlDTCSetting_UB                         |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $85 - Control DTC setting                      |
|                  Local ID  : $02 - DTCoff                                   |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ControlDTCSetting_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                           PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* The next line is only to prevent a compiler warning!!                    */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[0u] |= (UBYTE) 0x00u;


  if((0x00 != t_InBuffer_PBUF[d_CCglo_DLC + d_CCglo_03Byte]) || \
     (0x00 != t_InBuffer_PBUF[d_CCglo_DLC + d_CCglo_04Byte]) || \
     (0x00 != t_InBuffer_PBUF[d_CCglo_DLC + d_CCglo_05Byte]) )
  {
    /*========================================================================*/
    /* -1- positiv response shall be suppressed                               */
    /*========================================================================*/
    if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                    t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]) )
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

    } /* ---1- END OF if(..) -------------------------------------------------*/
    /*========================================================================*/
    /* -1- positiv response shall be send                                     */
    /*========================================================================*/
    else
    {
      if(d_CCkwpbase_LIDDTCOn == ((UBYTE) 0x7Fu & t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]))
      {
        /*--------------------------------------------------------------------*/
        /* set return value - will be overwritten by an error or other state  */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);
      }
      else
      {
        /*--------------------------------------------------------------------*/
        /* set return value - will be overwritten by an error or other state  */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
      }
    } /* ---1- END OF else (if(..)) ------------------------------------------*/
  }
  else
  {
    /*------------------------------------------------------------------------*/
    /* ERRRO invalid format                                                   */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);
  }
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_ControlDTCSetting_UB          -+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $85 - Control DTC setting                                    */
/*  LOCAL ID   : $02 - DTCoff                                                 */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTControlDTCSetting_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response for DTC off                                 */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_DTCSettingDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_02Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDControlDTCSetting + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDDTCOff,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Positive Response for DTC on                                  */
  /*--------------------------------------------------------------------------*/
  /* 04 - POST CONDIDTION  */  d_CCseq_DTCSettingDone,
  /* 05 - RESPONSE LENGTH  */  d_CCglo_02Byte,
  /* 06 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDControlDTCSetting + (UBYTE) 0x40u),
  /* 07 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDDTCOn,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Positive Response for DTC on                                  */
  /*--------------------------------------------------------------------------*/
  /* 08 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 09 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 10 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 11 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDControlDTCSetting,
  /* 12 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 13 - POST CONDIDTION  */  d_CCseq_DTCSettingDone,
  /* 14 - RESPONSE LENGTH  */  d_CCglo_00Byte

}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_CommunicationControl_UB                      |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $28 - Communication Control                    |
|                  Local ID  : $01 - enableRxAndDisableTx                     |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_CommunicationControl_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                              PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* The next line is only to prevent a compiler warning!                     */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[0u] |= (UBYTE) 0x00u;

  /*==========================================================================*/
  /* -1- positiv response shall be suppressed                                 */
  /*==========================================================================*/
  if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                  t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]) )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- positiv response shall be send                                       */
  /*==========================================================================*/
  else
  {
    if(d_CCkwpbase_LIDenableRxAndTx == (0x7F & t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]))
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);
    }
    else
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
    }

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*==========================================================================*/
  /* -1- The communication type is not correct                                */
  /*==========================================================================*/
  if (d_CCkwpaudi_normalCommunicationMessages !=  \
         t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u])
  {
    /*------------------------------------------------------------------------*/
    /* set return value - negativ repsonse index01                            */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

  } /* -1- END OF if(..) -----------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_CommunicationControl_UB       -+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $28 - Communication Control                                  */
/*  LOCAL ID   : $01 - enableRxAndDisableTx                                   */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTCommunicationControl_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_NDCDisableDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_02Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDCommunicationControl + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDenableRxAndDisableTx,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCglo_NoPostCondDefined,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_02Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDCommunicationControl + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDenableRxAndTx,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - communication tepy is invalid                                 */
  /*--------------------------------------------------------------------------*/
  /* 04 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 05 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 06 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 07 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDCommunicationControl,
  /* 08 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 09 - POST CONDIDTION  */  d_CCseq_NDCDisableDone,
  /* 10 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_WriteDataByID_UB                             |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $2E - WriteDataByIdentifier                    |
|                  Local ID  :  -                                             |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_WriteDataByID_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                       PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE  t_Result_UB;
  UNION2 t_ActID_U2;
  UBYTE  t_CheckFPC_UB;
  UBYTE  t_Count_UB;

  /*--------------------------------------------------------------------------*/
  /* check the addressing mode for functional addressing                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

  /*==========================================================================*/
  /* -1- service is functionally addressed                                    */
  /*==========================================================================*/
  if ( d_CCglo_Ready == t_Result_UB )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- service is not functionally addressed                                */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 2                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Set the correct positive response                                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC] = (UBYTE) 0x03u;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x00u] = (d_CCkwpbase_SIDWriteDataById + (UBYTE) 0x40u);
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u] = t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u];
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u] = t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u];

  /*==========================================================================*/
  /* -1- length of service is not correct, no recordDataIdentifier available  */
  /*==========================================================================*/
  if ((UBYTE) 0x03u > t_InBuffer_PBUF[d_CCglo_DLC] )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 01                                   */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

  } /* ---------1- END OF if(..) ---------------------------------------------*/
  /*==========================================================================*/
  /* -1- length of service is correct, recordDataIdentifier available         */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* get the actual recordDataIdentifier                                    */
    /*------------------------------------------------------------------------*/
    t_ActID_U2.UByte_ST.UByte0_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u];
    t_ActID_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x02u];
    /*========================================================================*/
    /* -2- the recordDataIdentifier is writeFingerprint                       */
    /*========================================================================*/
    if (c_CCconprj_WriteFingerprint_UW == t_ActID_U2.UWord_ST.UWord0_UW)
    {
      /*======================================================================*/
      /* -3- length of service is not correct                                 */
      /*     length should be   SID + 2 bytes recordDataIdentifer +           */
      /*     length of fingerprint + length of programmingData                */
      /*======================================================================*/
      if ( (0x03 + c_CCconprj_LengthRepairShopCode_UB +   \
              c_CCconprj_LengthProgDate_UB) != t_InBuffer_PBUF[d_CCglo_DLC])
      {
        /*--------------------------------------------------------------------*/
        /* set return value - response index 01                               */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

      } /* -----3- END OF if(..) ---------------------------------------------*/
      /*======================================================================*/
      /* -3- length of service is correct                                     */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* remark: the programming date is currently not checked!             */
        /*         (specified by VW/Audi)                                     */
        /*--------------------------------------------------------------------*/

        /*--------------------------------------------------------------------*/
        /* check the fingerprint, it has to be != 0                           */
        /*--------------------------------------------------------------------*/
        t_CheckFPC_UB = (UBYTE) 0u;

        /*====================================================================*/
        /* -4- do for all fingerprint bytes                                   */
        /*====================================================================*/
        for (t_Count_UB = 0; c_CCconprj_LengthRepairShopCode_UB > t_Count_UB; t_Count_UB++)
        {
          t_CheckFPC_UB |= t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x03u +  \
                             c_CCconprj_LengthProgDate_UB + t_Count_UB];
        } /* ---4- END OF for(..) --------------------------------------------*/

        /*====================================================================*/
        /* -4- the fingerPrint is invalid                                     */
        /*====================================================================*/
        if ( 0 == t_CheckFPC_UB )
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 02                             */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

        } /* ---4- END OF if(..) ---------------------------------------------*/
        /*====================================================================*/
        /* -4- the fingerPrint and the programmingDate is valid               */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* write programmingDate into ram                                   */
          /*------------------------------------------------------------------*/
          FUN_CCglo_PMemCopy_V(        &sECUDataAppl_ST.ProgDate_AUB[0],\
                                       &t_InBuffer_PBUF[d_CCglo_StartOfData + 0x03],  \
                                (ULONG)c_CCconprj_LengthProgDate_UB);

          /*------------------------------------------------------------------*/
          /* write fingerPrint into ram                                       */
          /*------------------------------------------------------------------*/
          FUN_CCglo_PMemCopy_V(  &sECUDataAppl_ST.FPC_Kennung_AUB[0],\
                                 &t_InBuffer_PBUF[d_CCglo_StartOfData + 0x03 + \
                                 c_CCconprj_LengthProgDate_UB], \
                                 (ULONG)c_CCconprj_LengthRepairShopCode_UB);

          t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagFPDoneOk; /* [OPL328] */
          /*------------------------------------------------------------------*/
          /* set return value 00                                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
        } /* ---4- END OF else (if(..)) --------------------------------------*/
      } /* -----3- END OF else (if(..)) --------------------------------------*/
    } /* -------2- END OF if(..) ---------------------------------------------*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* do nothing                                                           */
      /*----------------------------------------------------------------------*/
    }
  } /* ---------1- END OF else (if(..)) --------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_WriteDataByID_UB +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $2E - WriteDataByIdentifier                                  */
/*  LOCAL ID   :  -                                                           */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTWriteDataByID_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_WriteFingerprintDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - incorrect lenth of service request                            */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 04 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 05 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDWriteDataById,
  /* 06 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - requestOutOfRange                                             */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDWriteDataById,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - conditionsNotCorrect                                          */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDWriteDataById,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_TesterPresent_UB                             |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $3E - Tester Present                           |
|                  Local ID  : $00 - ZeroSubfunction                          |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_TesterPresent_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                       PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /* The next lines are only to prevent a compiler warning!!                  */
  t_InBuffer_PBUF[(UBYTE) 0u]  |= (UBYTE) 0x00u;
  t_OutBuffer_PBUF[(UBYTE) 0u] |= (UBYTE) 0x00u;

  /*==========================================================================*/
  /* -1- positiv response shall be suppressed                                 */
  /*==========================================================================*/
  if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                  t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]) )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- positiv response shall be send                                       */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;
}

/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $3E - Tester Present                                         */
/*  LOCAL ID   : $00 - ZeroSubfunction                                        */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTTesterPresent_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDITION   */  d_CCseqaudi_NoPostCondDefined,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_02Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDTesterPresent + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  0x00,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 03 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 04 - RESPONSE LENGTH  */  d_CCglo_00Byte

}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_ECUReset_UB                                  |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $11 - ECU Reset                                |
|                  Local ID  : $01 - hardReset                                |
|                  Command ID:  -                                             |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ECUReset_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                  PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;
  UBYTE t_EepromHelp_UB;

  /* The next lines are only to prevent a compiler warning!!                  */
  t_InBuffer_PBUF[0]  |= (UBYTE) 0x00;
  t_OutBuffer_PBUF[0] |= (UBYTE) 0x00;

  /*==========================================================================*/
  /* -1- the ECU reset is permitted                                           */
  /*==========================================================================*/
    /*========================================================================*/
    /* -2- Function was called a first time                                   */
    /*========================================================================*/
    if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
      d_CCkwpaudi_FlagRoutineBusy )
    {
      /*----------------------------------------------------------------------*/
      /* Set the internal busy flag                                           */
      /*----------------------------------------------------------------------*/
      t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

      /*----------------------------------------------------------------------*/
      /* response pending                                                     */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex00);

    } /* ---------2- END OF if(..) -------------------------------------------*/

    /*========================================================================*/
    /* -2- Function was called one more time                                  */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* Reset the internal busy flag                                         */
      /*----------------------------------------------------------------------*/
      t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

      /*----------------------------------------------------------------------*/
      /* clear the flash functions                                            */
      /*----------------------------------------------------------------------*/
      FUN_CCkwpaudi_ClearFlashFunctions_V();

      /*======================================================================*/
      /* -3- check programming dependencies was successfull                   */
      /*======================================================================*/
      if (d_CCkwpaudi_FlagProgDependenciesOk ==   \
         (d_CCkwpaudi_FlagProgDependenciesOk & t_CCkwpaudi_KwpCtrl_ST.State_UW) )
      {
        /*--------------------------------------------------------------------*/
        /* set application software valid in the eeprom                       */
        /*--------------------------------------------------------------------*/

        FUN_CCnvm_NvmAccess_UB (d_CCnvm_READ__SysProgrammed, &t_EepromHelp_UB);

        t_EepromHelp_UB = d_CCkwpaudi_AppProgrammed;

        FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_SysProgrammed, &t_EepromHelp_UB);

      } /* -------3- END OF if(..) -------------------------------------------*/

      /*----------------------------------------------------------------------*/
      /* suppress positive response                                           */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

      /*----------------------------------------------------------------------*/
      /* write the ECU reset request pattern                                  */
      /*----------------------------------------------------------------------*/
      FUN_CCapplint_SetResetRequest_V();

      /*----------------------------------------------------------------------*/
      /* Set reset flag                                                       */
      /*----------------------------------------------------------------------*/
      FUN_CCeventaudi_EventHandler_B ( d_CCeventaudi_SET_____SystemReset_UB );

    } /* ---------2- END OF else(..) -----------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_ECUReset_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $11 - ECU Reset                                              */
/*  LOCAL ID   : $01 - hardReset                                              */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTECUReset_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 02 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDEcuReset,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Sequence error                                                */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 02 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDEcuReset,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 03 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 04 - RESPONSE LENGTH  */  d_CCglo_00Byte


}; /*--- END OF TABLE --------------------------------------------------------*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_SaGetSeed_UB                                |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $27 - Security Access                         |
|                  Local ID  : $01 - Get Seed                                |
|                  Command ID:  -                                            |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/   /* [OPL308_309] */
UBYTE FUN_CCkwpaudi_SaGetSeed_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                   PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /* The next line is only to prevent a compiler warning!!                    */
  t_InBuffer_PBUF[0] |= (UBYTE) 0x00u;

  /*--------------------------------------------------------------------------*/
  /* set return value - will be overwritten by an error or other state        */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  /*--------------------------------------------------------------------------*/
  /* Send the correct positive response                                       */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC]                          = (0x02u + d_CCsecaudi_SeedLength);
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x00u]  = (d_CCkwpbase_SIDSecurityAccess + (UBYTE) 0x40u);
  t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]  = d_CCkwpaudi_LIDSaGetSeedMode1;

  /*==========================================================================*/
  /* -1- Security access is already granted                                   */
  /*==========================================================================*/
  if(d_CCkwpaudi_FlagSecurityAccessGranted == (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSecurityAccessGranted))
  {
    t_CCkwpaudi_KwpCtrl_ST.Seed_UL  = 0x00ul;

    /*------------------------------------------------------------------------*/
    /* Copy the seed into the outbuffer                                       */
    /*------------------------------------------------------------------------*/
    FUN_CCglo_PMemCopy_V( &t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE)0x02],
                          (FAR_PTR_UBYTE)&t_CCkwpaudi_KwpCtrl_ST.Seed_UL,
                          0x04ul);

    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);
  } /* ---------1- END OF if (..) --------------------------------------------*/
  /*==========================================================================*/
  /* -1- Security access is not granted                                       */
  /*==========================================================================*/
  else
  {
    /*========================================================================*/
    /* -2- a seed has already been read out yet                               */
    /*========================================================================*/
    if(0x00u != (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSeedAlreadyRequested))
    {
      /*----------------------------------------------------------------------*/
      /* Copy the seed into the outbuffer                                     */
      /*----------------------------------------------------------------------*/
      FUN_CCglo_PMemCopy_V( &t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE)0x02],
                            (FAR_PTR_UBYTE)&t_CCkwpaudi_KwpCtrl_ST.Seed_UL,
                            0x04ul);

    } /* -------2- END OF if (..) --------------------------------------------*/
    /*========================================================================*/
    /* -2- no seed has been read out yet                                      */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -3- Function was called a first time                                 */
      /*======================================================================*/
      if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
        d_CCkwpaudi_FlagRoutineBusy )
      {
        /*--------------------------------------------------------------------*/
        /* Set the internal busy flag                                         */
        /*--------------------------------------------------------------------*/
        t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

        /*--------------------------------------------------------------------*/
        /* response pending                                                   */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex03);

      } /* -----3- END OF if (..) --------------------------------------------*/

      /*======================================================================*/
      /* -3- Function was called one more time                                */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Reset the internal busy flag                                       */
        /*--------------------------------------------------------------------*/
        t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

        /* read NVM */
        FUN_CCkwpaudi_ReadTimelock_UB (&sECUDataAppl_ST.SecAccess_UB );

        /*====================================================================*/
        /* -4- Timelock is still running 0x37                                 */
        /*====================================================================*/
        if(FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_LOOKFOR_Timelock_UB) != FALSE)
        {
          /* Send Access count exceed 0x37 */
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

        } /* ---4- END OF if (..) --------------------------------------------*/
        /*====================================================================*/
        /* -4- Timelock is expired                                            */
        /*====================================================================*/
        else
        {
          /*==================================================================*/
          /* -5- If too many accesses have been tried                         */
          /* (no more Seed after 3x wrong Key)                                */
          /*==================================================================*/
          if ((sECUDataAppl_ST.SecAccess_UB & (UBYTE) 0x7Fu) >= (UBYTE) 0x03u)  /* [OPL288] */
          {
            /* Send Access count exceed 0x36 */
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

          } /* -5- END OF if (..) --------------------------------------------*/
          /*==================================================================*/
          /* -5- not to many accesses                                         */
          /*==================================================================*/
          else
          {
            /*----------------------------------------------------------------*/
            /* Get a seed                                                     */
            /*----------------------------------------------------------------*/
            FUN_CCsecaudi_CreateSeed_V ( (PTR_BUFFER_UB) &t_CCkwpaudi_KwpCtrl_ST.Seed_UL );

            /*----------------------------------------------------------------*/
            /* Copy the seed into the outbuffer                               */
            /*----------------------------------------------------------------*/
            FUN_CCglo_PMemCopy_V( &t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE)0x02],
                                  (FAR_PTR_UBYTE)&t_CCkwpaudi_KwpCtrl_ST.Seed_UL,
                                  0x04ul);

            /*----------------------------------------------------------------*/
            /* a seed had already been requested                              */
            /*----------------------------------------------------------------*/
            t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagSeedAlreadyRequested;

            /* reset flag because sequence starts again */
            t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagSecurityAccessDenied;

          } /* -5- END OF else(..) -------------------------------------------*/
        } /* ---4- END OF else(..) -------------------------------------------*/
      } /* -----3- END OF else(..) -------------------------------------------*/
    } /* -------2- END OF else(..) -------------------------------------------*/
  } /* ---------1- END OF else(..) -------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_SaGetSeed_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $27 - Security Access                                        */
/*  LOCAL ID   : $01 - Get Seed                                               */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTSecurityAccessGetSeed_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_SAGetSeedDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - too much unsuccessfully tries                                 */
  /*--------------------------------------------------------------------------*/
  /* 03 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 04 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 05 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 06 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDSecurityAccess,
  /* 07 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCExceedNumberOfAttempts,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Invalid Key (access denied)                                   */
  /*--------------------------------------------------------------------------*/
  /* 08 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 09 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 10 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 11 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDSecurityAccess,
  /* 12 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequiredTimeDelayNotExpired,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 13 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 14 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 15 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 16 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDSecurityAccess,
  /* 17 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 03 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 04 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_SaSendKey_UB                                |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $27 - Security Access                         |
|                  Local ID  : $02 - Send Key                                |
|                  Command ID:  -                                            |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_SaSendKey_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                   PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;
  UBYTE t_AccessState_UB;

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= (UBYTE) 0x00u;

  /*==========================================================================*/
  /* -1- Function was called a first time                                     */
  /*==========================================================================*/
  if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
    d_CCkwpaudi_FlagRoutineBusy )
  {
    /*------------------------------------------------------------------------*/
    /* Set the internal busy flag                                             */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

    /*------------------------------------------------------------------------*/
    /* response pending                                                       */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex03);

  } /* ---------1- END OF if(..) ---------------------------------------------*/

  /*==========================================================================*/
  /* -1- Function was called one more time                                    */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* Reset the internal busy flag                                           */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

    /*------------------------------------------------------------------------*/
    /* reset flag that a seed had already been requested                      */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagSeedAlreadyRequested;   /* [OPL308_309] */

    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

    /* look for a former access denied in the sequence */
    if (d_CCkwpaudi_FlagSecurityAccessDenied == \
       (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSecurityAccessDenied))
    {
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);
    }
    else
    {
      /*----------------------------------------------------------------------*/
      /* Calculate key too                                                    */
      /*----------------------------------------------------------------------*/
      t_AccessState_UB = \
        FUN_CCsecaudi_CheckKey_UB ( &t_InBuffer_PBUF[d_CCglo_StartOfData + (UBYTE)0x02] );

      /*======================================================================*/
      /* -2- Access denied                                                    */
      /*======================================================================*/
      if (t_AccessState_UB == d_CCsecaudi_AccessDenied)
      {
        /* read NVM */
        FUN_CCkwpaudi_ReadTimelock_UB ( &sECUDataAppl_ST.SecAccess_UB );

        /* count attempts */
        sECUDataAppl_ST.SecAccess_UB += 1;
        /* store it in NVM (because of power on reset) */
        FUN_CCkwpaudi_WriteTimelock_UB (&sECUDataAppl_ST.SecAccess_UB );

        /* look for entry attepts */
        if ((sECUDataAppl_ST.SecAccess_UB & 0x7F) >= 0x03)
        {
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____Timelock_UB);
        }

        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);
        t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagSecurityAccessDenied;
      } /* -2- END OF if(..) -------------------------------------------------*/
      else
      {
        /* reset all locks */
        sECUDataAppl_ST.SecAccess_UB = 0x00;

        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___Timelock_UB);

        /* change eeprom info */
        FUN_CCkwpaudi_WriteTimelock_UB ( &sECUDataAppl_ST.SecAccess_UB );

        t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagSecurityAccessGranted;
        t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagSecurityAccessDenied;
      }
    }
  } /* ---------1- END OF else(..) -------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_SaSendKey_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $27 - Security Access                                        */
/*  LOCAL ID   : $02 - Send Key                                               */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTSecurityAccessSendKey_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_SASendKeyDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_02Byte,
  /* 02 - RESPONSE BYTE 01 */  (d_CCkwpbase_SIDSecurityAccess + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 02 */  d_CCkwpaudi_LIDSaSendKeyMode1,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Invalid Key (access denied)                                   */
  /*--------------------------------------------------------------------------*/
  /* 04 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 05 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 06 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 07 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDSecurityAccess,
  /* 08 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCInvalidKey,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Invalid Key (access denied)                                   */
  /*--------------------------------------------------------------------------*/
  /* 04 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 05 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 06 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 07 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDSecurityAccess,
  /* 08 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestSequenceError0x24,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 09 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 10 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 11 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 12 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDSecurityAccess,
  /* 13 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending
}; /*--- END OF TABLE --------------------------------------------------------*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_RequestDownload_UB                          |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $34 - Request Download                        |
|                  Local ID  :  -                                            |
|                  Command ID:  -                                            |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RequestDownload_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                         PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;
  tHis_Header  *pHisHeader;
  void        (*_FlashInit) (tFlashParam*);
  UBYTE t_NbrAddressBytes_UB;
  UBYTE t_NbrSizeBytes_UB;
  UBYTE t_ReturnValue_UB;
  UNION2 t_MemoryIndex_U2;
  UNION4 t_MemoryLength_U4;
  UBYTE t_Index_UB;

#ifdef cs_Signatur_Aktiv
  UBYTE t_count_UB;
  ULONG t_SegmentInfo_AUL[2];
#endif

#ifdef cs_Decryption_Aktiv
  UWORD t_DecryptIvIndex_UW;
#endif

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= 0x00;

  /*--------------------------------------------------------------------------*/
  /* check the addressing mode for functional addressing                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

  /*==========================================================================*/
  /* -1- service is functionally addressed                                    */
  /*==========================================================================*/
  if ( d_CCglo_Ready == t_Result_UB )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* -------------1- END OF if(..) -----------------------------------------*/
  /*==========================================================================*/
  /* -1- service is not functionally addressed                                */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 1                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

  } /* -------------1- END OF else (if(..)) ----------------------------------*/

  /*==========================================================================*/
  /* -1- function runs 2. time                                                */
  /*==========================================================================*/
  if (d_CCkwpaudi_FlagRoutineBusy == \
  (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy))
  {
    /* clear the busy flag */
    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;
    /*----------------------------------------------------*/
    /* Parrot Init                            */
    /*----------------------------------------------------*/

    /*----------------------------------------------------*/
    /* Init the HIS Driver                                */
    /*----------------------------------------------------*/
    pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
    _FlashInit = pHisHeader->pfnFlashInit;

    _FlashInit (&FlashParam);

    if (d_CCpar_FlashOK != FlashParam.errorcode)
    {
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex06);
    }
    else
    {
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
      g_CCkwpaudi_ParrotCRC32_UL = 0x00000000u;
    }
  }
  /*==========================================================================*/
  /* -1- function runs 1. time                                                */
  /*==========================================================================*/
  else
  {
    /*--------------------------------------------------------------------------*/
    /* evaluate the addressAndLengthFormatIdentifier                            */
    /*--------------------------------------------------------------------------*/
    t_NbrAddressBytes_UB = 0x0F & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02];
    t_NbrSizeBytes_UB    = (0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02]) >> 0x04;

    /*--------------------------------------------------------------------------*/
    /* init the block sequence counter                                          */
    /*--------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB = d_CCglo_InitToZero;

    /*--------------------------------------------------------------------------*/
    /* NOTE:                                                                    */
    /*     The address is used as an INDEX for the specified blocks in the      */
    /*     download map                                                         */
    /*--------------------------------------------------------------------------*/

    /*--------------------------------------------------------------------------*/
    /* Check all Parameter:                                                     */
    /* (a) Sequence       - checked by the session handler                      */
    /* (b) FormatID                                                             */
    /* (c) Service Length                                                       */
    /* (d) Format ID                                                            */
    /* (e) Valid Address ranges                                                 */
    /*--------------------------------------------------------------------------*/

    /*==========================================================================*/
    /* -2- The number of address bytes or memory size bytes is not correct;     */
    /*     more than 4 bytes are not supported                                  */
    /*==========================================================================*/
    if ( (0 == t_NbrAddressBytes_UB) || ( 0x02 < t_NbrAddressBytes_UB ) ||
         (0 == t_NbrSizeBytes_UB)    || ( 0x04 < t_NbrSizeBytes_UB ) )
    {
      /*========================================================================*/
      /* -3- The length of the service request is wrong                         */
      /*========================================================================*/
      /* [OPL375] check length of addressandlengthformatiidentifier             */
      if ( (t_NbrAddressBytes_UB + t_NbrSizeBytes_UB + 0x03) != t_InBuffer_PBUF[d_CCglo_DLC] )
      {
        /*----------------------------------------------------------------------*/
        /* set return value                                                     */
        /*----------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);
      } /* -----------2- END OF else (if(..)) ----------------------------------*/
      /*========================================================================*/
      /* -3- The length of the service request is correct                       */
      /*========================================================================*/
      else
      {
      /*------------------------------------------------------------------------*/
      /* set return value 2                                                     */
      /*------------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);
      }

    } /* -------------1- END OF if(..) -----------------------------------------*/
    /*==========================================================================*/
    /* -2- The number of address bytes and memory size bytes is correct         */
    /*==========================================================================*/
    else
    {
      /*========================================================================*/
      /* -3- The length of the service request is wrong                         */
      /*========================================================================*/
      /* [OPL375] check length of addressandlengthformatiidentifier             */
      if ( (t_NbrAddressBytes_UB + t_NbrSizeBytes_UB + 0x03) != t_InBuffer_PBUF[d_CCglo_DLC] )
      {
        /*----------------------------------------------------------------------*/
        /* set return value                                                     */
        /*----------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);
      } /* -----------2- END OF else (if(..)) ----------------------------------*/
      /*========================================================================*/
      /* -3- The length of the service request is correct                       */
      /*========================================================================*/
      else
      {
        /*======================================================================*/
        /* -4- Memory index consists of 1 byte                                  */
        /*======================================================================*/
        if ( 1 == t_NbrAddressBytes_UB )
        {
          /*--------------------------------------------------------------------*/
          /* Get the memory index                                               */
          /*--------------------------------------------------------------------*/
          t_MemoryIndex_U2.UByte_ST.UByte0_UB = 0;
          t_MemoryIndex_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x03];
        } /* ---------3- END OF if(..) -----------------------------------------*/
        /*======================================================================*/
        /* -4- Memory index consists of 2 bytes                                 */
        /*======================================================================*/
        else
        {
          /*--------------------------------------------------------------------*/
          /* Get the memory index                                               */
          /*--------------------------------------------------------------------*/
          t_MemoryIndex_U2.UByte_ST.UByte0_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x03];
          t_MemoryIndex_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04];
        } /* ---------3- END OF else (if(..)) ----------------------------------*/

        /*----------------------------------------------------------------------*/
        /* get the start address for programming                                */
        /*----------------------------------------------------------------------*/
        t_ReturnValue_UB = FUN_CCkwpaudi_ResolveMemoryMapIndex_UB(  \
                                     &t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL,
                                     &t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL,
                                      t_MemoryIndex_U2.UWord_ST.UWord0_UW);

        /*======================================================================*/
        /* -4- the memory index was not correct                                 */
        /*======================================================================*/
        if ( d_CCglo_Ready != t_ReturnValue_UB )
        {
          /*--------------------------------------------------------------------*/
          /* set return value - response index 1                                */
          /*--------------------------------------------------------------------*/

        } /* ---------3- END OF if(..) -----------------------------------------*/
        /*======================================================================*/
        /* -4- the memory index was correct                                     */
        /*======================================================================*/
        else
        {
          /*====================================================================*/
          /* -5- Get the programming length dependant on the nmber of byte      */
          /*====================================================================*/
          switch (t_NbrSizeBytes_UB)
          {
            /*------------------------------------------------------------------*/
            /* Address consists of 4 bytes                                      */
            /*------------------------------------------------------------------*/
          case 0x04:
            t_MemoryLength_U4.UByte_ST.UByte0_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x03 + t_NbrAddressBytes_UB];

            t_MemoryLength_U4.UByte_ST.UByte1_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x04 + t_NbrAddressBytes_UB];

            t_MemoryLength_U4.UByte_ST.UByte2_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x05 + t_NbrAddressBytes_UB];

            t_MemoryLength_U4.UByte_ST.UByte3_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x06 + t_NbrAddressBytes_UB];

            break;
            /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

            /*------------------------------------------------------------------*/
            /* Address consists of 3 bytes                                      */
            /*------------------------------------------------------------------*/
          case 0x03:
            t_MemoryLength_U4.UByte_ST.UByte0_UB = 0x00;

            t_MemoryLength_U4.UByte_ST.UByte1_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x03 + t_NbrAddressBytes_UB];

            t_MemoryLength_U4.UByte_ST.UByte2_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x04 + t_NbrAddressBytes_UB];

            t_MemoryLength_U4.UByte_ST.UByte3_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x05 + t_NbrAddressBytes_UB];
            break;
            /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

            /*------------------------------------------------------------------*/
            /* Address consists of 2 bytes                                      */
            /*------------------------------------------------------------------*/
          case 0x02:

            t_MemoryLength_U4.UByte_ST.UByte0_UB = 0x00;

            t_MemoryLength_U4.UByte_ST.UByte1_UB = 0x00;

            t_MemoryLength_U4.UByte_ST.UByte2_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x03 + t_NbrAddressBytes_UB];

            t_MemoryLength_U4.UByte_ST.UByte3_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x04 + t_NbrAddressBytes_UB];

            break;
            /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

            /*------------------------------------------------------------------*/
            /* Address consists of 1 bytes                                      */
            /*------------------------------------------------------------------*/
          default:
            t_MemoryLength_U4.UByte_ST.UByte0_UB = 0x00;

            t_MemoryLength_U4.UByte_ST.UByte1_UB = 0x00;

            t_MemoryLength_U4.UByte_ST.UByte2_UB = 0x00;

            t_MemoryLength_U4.UByte_ST.UByte3_UB = t_InBuffer_PBUF  \
                           [d_CCglo_StartOfData + 0x03 + t_NbrAddressBytes_UB];
            break;
            /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          } /* -------5- END OF switch(..) -------------------------------------*/

          /*--------------------------------------------------------------------*/
          /* Get the dataFormatIdentifier                                       */
          /*--------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL = t_MemoryLength_U4.ULong_ST.ULong0_UL;

          /*--------------------------------------------------------------------*/
          /* Get the dataFormatIdentifier                                       */
          /*--------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.DataFormatId_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01];

          /*--------------------------------------------------------------------*/
          /* Calculate ProgEndAddress (32 Bit)                                  */
          /*--------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL = ( t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL + \
            (t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL - 0x01ul) );

          /* save addresses for later verify (check memory) */
          t_CCkwpaudi_KwpCtrl_ST.DLStartAddress_UL = \
              t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL;
          t_CCkwpaudi_KwpCtrl_ST.DLEndAddress_UL = \
              t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL;
          /*--------------------------------------------------------------------*/
          /* Do address check and get memory information                        */
          /*--------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW = FUN_CCkwpaudi_LogicalAddressCheck_UW (      \
            t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL, \
            t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL,   \
            d_CCglo_Download);

          /*====================================================================*/
          /* -5- Logical Addresscheck was not correct                           */
          /*====================================================================*/
          if ( d_CCglo_AddressCheckNOK == \
            (d_CCglo_AddressCheckMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
          {
            /*==================================================================*/
            /* -6- the memory is not programmable                               */
            /*==================================================================*/
            if ( d_CCglo_NotProgramable == \
              (d_CCglo_ProgramStateMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
            {
              /*----------------------------------------------------------------*/
              /* set return value                                               */
              /*----------------------------------------------------------------*/
              /* imx [OPL367] von $22 auf $31 */
              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

            } /* -----6- END OF if(..) -----------------------------------------*/
            else
            {
              /*----------------------------------------------------------------*/
              /* set return value 1                                             */
              /*----------------------------------------------------------------*/

            } /* -----6- END OF else (if(..)) ----------------------------------*/
          } /* -------5- END OF if(..) -----------------------------------------*/

          /*====================================================================*/
          /* -5- Logical AddressCheck was correct                               */
          /*====================================================================*/
          else
          {

#ifdef cs_Signatur_Aktiv
            /*------------------------------------------------------------------*/
            /* initialise the signature result buffer                           */
            /*------------------------------------------------------------------*/
            for (t_count_UB = (UBYTE) 0x00u; t_count_UB < (UBYTE) SECM_MIN_SIGNATURE_TYPE_LENGTH; t_count_UB++)
            {
              g_VerSigSigResultBuffer_AUB[t_count_UB] = (UBYTE) 0x00u;
            }

            /*------------------------------------------------------------------*/
            /* set data of result buffer                                        */
            /*------------------------------------------------------------------*/
            g_SignatureType.currentHash.sigResultBuffer = \
                                        (ULONG) &g_VerSigSigResultBuffer_AUB[0];
            g_SignatureType.currentHash.length = \
                                        SECM_MIN_SIGNATURE_TYPE_LENGTH;

            /*------------------------------------------------------------------*/
            /* set pointer to current length of the hashed data                 */
            /*------------------------------------------------------------------*/
            g_SignatureType.currentDataLength = &g_VerSigCurrentDataLength_UL;

            /*------------------------------------------------------------------*/
            /* set pointer to watchdog function                                 */
            /*------------------------------------------------------------------*/
            g_SignatureType.wdTriggerFct = \
                    (FL_WDTriggerFctType) FUN_CCwtdog_TriggerWatchdogProgMem_V;

            /*==================================================================*/
            /* Do Initialization                                                */
            /*==================================================================*/
            g_SignatureType.sigState = kHashInit;
            SecM_VerifySignature(&g_SignatureType);

            /*------------------------------------------------------------------*/
            /* write memory index and programming length into t_SegmentInfo_AUL */
            /*------------------------------------------------------------------*/
            t_SegmentInfo_AUL[0] = ((ULONG)t_MemoryIndex_U2.UWord_ST.UWord0_UW) << 0x18;
            t_SegmentInfo_AUL[1] = ((t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL & 0xFF000000ul)>>0x18) +
                             ((t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL & 0x00FF0000ul)>>0x08) +
                             ((t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL & 0x0000FF00ul)<<0x08) +
                             ((t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL & 0x000000FFul)<<0x18);

            /*------------------------------------------------------------------*/
            /* set address of t_SegmentInfo_AUL and number of bytes             */
            /*------------------------------------------------------------------*/
            g_SignatureType.sigSourceBuffer = (const UBYTE *) &t_SegmentInfo_AUL;
            g_SignatureType.sigByteCount    = 0x08u;

            /*==================================================================*/
            /* Do Calculation                                                   */
            /* (start hash calculation with block index and programming length) */
            /*==================================================================*/
            g_SignatureType.sigState = kHashCompute;
            SecM_VerifySignature(&g_SignatureType);
#endif

#ifdef cs_Decryption_Aktiv
            /*------------------------------------------------------------------*/
            /* -6- initialise IV for next block                                 */
            /*------------------------------------------------------------------*/
            for (t_DecryptIvIndex_UW = 0x00u; t_DecryptIvIndex_UW < EscAes_BLOCK_BYTES; t_DecryptIvIndex_UW++)
            {
              /*----------------------------------------------------------------*/
              /* copy one byte                                                  */
              /*----------------------------------------------------------------*/
              g_DecryptIvTemp_AUB[t_DecryptIvIndex_UW] = g_DecryptIv_AUB[t_DecryptIvIndex_UW];

            } /* -----6- END OF for(..) ----------------------------------------*/

            /*==================================================================*/
            /* -6- block is not encrypted -> not allowed                        */
            /*==================================================================*/
            if ( (UBYTE) 0x01u != t_CCkwpaudi_KwpCtrl_ST.DataFormatId_UB )
            {
              /*----------------------------------------------------------------*/
              /*  set return value                                              */
              /*----------------------------------------------------------------*/
              t_Result_UB = (d_CCglo_Ready | d_CCglo_ResponseIndex01);

            } /* -----6- END OF if(..) -----------------------------------------*/
            /*==================================================================*/
            /* -6- block is encrypted -> ok                                     */
            /*==================================================================*/
            else
            {
#else
            /*==================================================================*/
            /* -6- block is encrypted -> not allowed                            */
            /*==================================================================*/
            if ( (UBYTE) 0x00u != t_CCkwpaudi_KwpCtrl_ST.DataFormatId_UB )
            {
              /*----------------------------------------------------------------*/
              /*  set return value                                              */
              /*----------------------------------------------------------------*/
              t_Result_UB = (d_CCglo_Ready | d_CCglo_ResponseIndex01);

            } /* -----6- END OF if(..) -----------------------------------------*/
            /*==================================================================*/
            /* -6- block is not encrypted -> ok                                 */
            /*==================================================================*/
            else
            {
#endif

              /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
              /*                                                                */
              /* NOTE: The Flashfunctions are not onboard. So at first the      */
              /*       flash-functions must be downloaded into the RAM and then */
              /*       a normal download to the flash memory could be started!! */
              /*                                                                */
              /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

              /*================================================================*/
              /*                                                                */
              /* -7- Download to RAM                                            */
              /*                                                                */
              /*================================================================*/
              if ( d_CCglo_RamMemory == \
                (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
              {
                /* Addresscheck ok - Send poditive response                     */
                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                /* Set the max block length during a data transfer              */
                t_CCkwpaudi_KwpCtrl_ST.MaxBlockLength_UB = \
                  d_CCkwpaudi_MaxBlockLengthRamDownload;
                /* Set the programming start address                            */
                t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL = \
                  t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL;
                /* Inform about download to ram                                 */
                t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagRamDownload;
                t_CCkwpaudi_KwpCtrl_ST.State_UW &=   \
                    ~(d_CCkwpaudi_FlagInternFlashDownload | d_CCkwpaudi_FlagEepromDownload);

                /*--------------------------------------------------------------*/
                /* Store the addresses of the flash functions                   */
                /* These are used for deleting the flash functions before reset */
                /*--------------------------------------------------------------*/
                t_CCkwpaudi_KwpCtrl_ST.FFStartAddress_UL = t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL;
                t_CCkwpaudi_KwpCtrl_ST.FFEndAddress_UL = t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL;

                /*--------------------------------------------------------------*/
                /* Init the collect data function                               */
                /*--------------------------------------------------------------*/
                #ifdef d_CCkwp_PageWrite
                  FUN_CCglo_InitCollectData_V ();
                #endif

                /*----------------------------------------------------------------*/
                /* Preparations meet to begin download                            */
                /* Request: A SequenceError of RequestDownload should bring a     */
                /* NRC 0x22. Flag is active till RequestTransferExit is executed  */
                /*----------------------------------------------------------------*/
                t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagRequestDownloadActive;

              } /* -----7- END OF else (if(..)) --------------------------------*/

              /*================================================================*/
              /*                                                                */
              /* -7- other download                                             */
              /*                                                                */
              /*================================================================*/
              else
              {
                /*==============================================================*/
                /*                                                              */
                /* -8- Download to flash or parrot memory                       */
                /*                                                              */
                /*==============================================================*/
                if (( d_CCglo_FlashMemory == \
                  (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))\
                  ||( d_CCglo_Parrot == \
                   (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))\
                  ||( d_CCglo_Parrot_Boot == \
                   (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))\
                  ||( d_CCglo_Parrot_Upd == \
                   (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW)))
                {

                  /*============================================================*/
                  /* -9- special handling for flash memory					  */
                  /* the memory size is not multiple of the page length         */
                  /*============================================================*/
                  if(( d_CCglo_FlashMemory == \
                      (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))\
                  && (0x00ul != (t_MemoryLength_U4.ULong_ST.ULong0_UL % (ULONG)d_CCconprj_PageLength)))  /* [OPL321] */
                  {
                    /*----------------------------------------------------------*/
                    /* set return value 2                                       */
                    /*----------------------------------------------------------*/

                  } /* -9- END OF if(..) ---------------------------------------*/
                  /*============================================================*/
                  /* -9- the memory size is a multiple of the page length       */
                  /*============================================================*/
                  else
                  {
                    /*----------------------------------------------------------*/
                    /* Check the HIS-Driver Interface-Header                    */
                    /*----------------------------------------------------------*/
                    t_Index_UB = t_CCkwpaudi_KwpCtrl_ST.DownloadMapIndex_UB;
                    t_Result_UB = FUN_CChal_HISDriverCheck_UB((tHis_Header*) \
                    &t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].ubInterfaceVersion);

                    /*==========================================================*/
                    /* -a- Result evaluation                                    */
                    /*==========================================================*/
                    switch ( t_ReturnValue_UB )
                    {
                      /*--------------------------------------------------------*/
                      /* No HIS-Driver available                                */
                      /*--------------------------------------------------------*/
                    case (d_CCglo_Error | 0x01):
                      /* Send negative response 02                              */

                      break;
                      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - */

                      /*--------------------------------------------------------*/
                      /* Wrong HIS-Driver available                             */
                      /*--------------------------------------------------------*/
                    case (d_CCglo_Error | 0x02):
                      /* Send negative response 02                              */

                      break;
                      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - */

                      /*--------------------------------------------------------*/
                      /* Everything is OK                                       */
                      /*--------------------------------------------------------*/
                    default:
                      /* Addresscheck ok - Set return index 00                  */
                      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
                      /* Set the max block length during a data transfer      */
                      t_CCkwpaudi_KwpCtrl_ST.MaxBlockLength_UB = \
                        d_CCkwpaudi_MaxBlockLengthInternFlashDownload;
                      /* Set the programming start address                    */
                      t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL = \
                        t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL;
                      /* Inform about download to internal flash memory       */
                      t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagInternFlashDownload;

                      /*--------------------------------------------------------*/
                      /* clear former flags                                     */
                      /*--------------------------------------------------------*/
                      t_CCkwpaudi_KwpCtrl_ST.State_UW &=   \
                        ~(d_CCkwpaudi_FlagRamDownload | d_CCkwpaudi_FlagEepromDownload);

                      /*--------------------------------------------------------*/
                      /* Preparations meet to begin download                    */
                      /* Request: A SequenceError of RequestDownload should     */
                      /* bring a NRC 0x22. Flag is active till                  */
                      /* RequestTransferExit is executed.                       */
                      /*----------------------------------------------------------------*/
                      t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagRequestDownloadActive;

                      /*--------------------------------------------------------*/
                      /* Init the collect data function                         */
                      /*--------------------------------------------------------*/
                      #ifdef d_CCkwp_PageWrite
                        FUN_CCglo_InitCollectData_V ();
                      #endif
                      /*======================================================*/
                      /* -b- special for parrot modul                         */
                      /* FlashParams has to be given to init function         */
                      /*------------------------------------------------------*/
                      if (( d_CCglo_Parrot == \
                          (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))\
                        ||( d_CCglo_Parrot_Upd == \
                          (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW)))
                      {
                        FlashParam.length = t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL;
                        FlashParam.Delaytime_UL = c_CCconprj_ClockFrequency_UL;
                        FlashParam.address = d_CCconprj_PageLength;
                        /* means application download */
                        FlashParam.Command_UB = FALSE;
                        FlashParam.wdTriggerFct =(tWDTriggerFct)FUN_CCwtdog_TriggerWatchdogEraseMem_V;
                        /*--------------------------------------------------*/
                        /* NOTE: Now is the rigth time to send our first    */
                        /*       "ResponsePending". After sending we will   */
                        /*       have P3max instead of P2max for timeout.   */
                        /*       This is necessary because of timedelays in */
                        /*       parrots init function.                     */
                        /*--------------------------------------------------*/
                        /*--------------------------------------------------*/
                        /* Set the internal busy flag                       */
                        /*--------------------------------------------------*/
                        t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

                        /*--------------------------------------------------*/
                        /* set return value - response index 2              */
                        /*--------------------------------------------------*/
                        t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex05);
                      }
                      /*======================================================*/
                      /* -b- special for parrot modul                         */
                      else
                      {
                        /*====================================================*/
                        /* -c- special for parrot boot modul                  */
                        if ( d_CCglo_Parrot_Boot == \
                           (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
                        {
                          FlashParam.length = t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL;
                          FlashParam.Delaytime_UL = c_CCconprj_ClockFrequency_UL;
                          FlashParam.address = d_CCconprj_PageLength;
                          /* means bootloader download */
                          FlashParam.Command_UB = TRUE;
                          FlashParam.wdTriggerFct =(tWDTriggerFct)FUN_CCwtdog_TriggerWatchdogEraseMem_V;
                          /*--------------------------------------------------*/
                          /* Set the internal busy flag                       */
                          /*--------------------------------------------------*/
                          t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

                          /*--------------------------------------------------*/
                          /* set return value - response index 2              */
                          /*--------------------------------------------------*/
                          t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex05);
                        }
                        /*====================================================*/
                        /* -c- flashmemory his init call                      */
                        else
                        {
                          /*--------------------------------------------------*/
                          /* normal FlashMemory Init                          */
                          /*--------------------------------------------------*/
                          /*--------------------------------------------------*/
                          /* Init the HIS Driver                              */
                          /*--------------------------------------------------*/
                          pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
                          _FlashInit = pHisHeader->pfnFlashInit;

                          _FlashInit (&FlashParam);
                        }/* c- END OF else (if(..)) --------------------------*/

                      }/* --b- END OF else (if(..)) ------------------------------*/
                      break;
                    } /* -a- END OF switch(..) ---------------------------------*/
                  } /* ---9- END OF else (if(..)) ------------------------------*/
                } /* -----8- END OF if(..) -------------------------------------*/
                /*==============================================================*/
                /*                                                              */
                /* -8- Download to Eeprom memory                                */
                /*                                                              */
                /*==============================================================*/
                else
                {
                  /* Addresscheck ok - Send poditive response                   */
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                  /* Set the max block length during a data transfer            */
                  t_CCkwpaudi_KwpCtrl_ST.MaxBlockLength_UB = \
                    d_CCkwpaudi_MaxBlockLengthEepromDownload;
                  /* Set the programming start address                          */
                  t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL = \
                    t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL;
                  /* Inform about download to ram                               */
                  t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagEepromDownload;
                  t_CCkwpaudi_KwpCtrl_ST.State_UW &=   \
                      ~(d_CCkwpaudi_FlagInternFlashDownload | d_CCkwpaudi_FlagRamDownload);

                  /*----------------------------------------------------------------*/
                  /* Preparations meet to begin download                            */
                  /* Request: A SequenceError of RequestDownload should bring a     */
                  /* NRC 0x22. Flag is active till RequestTransferExit is executed  */
                  /*----------------------------------------------------------------*/
                  t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagRequestDownloadActive;

                  /*------------------------------------------------------------*/
                  /* Init the collect data function                             */
                  /*------------------------------------------------------------*/
                  #ifdef d_CCkwp_PageWrite
                    FUN_CCglo_InitCollectData_V ();
                  #endif

                  /*------------------------------------------------------------*/
                  /* reset number of data bytes within data collect buffer      */
                  /*------------------------------------------------------------*/
                  g_CCkwpaudi_EepromDataLeftOver_UB = (UBYTE) 0x00u;

                } /* -8- END OF else (if(..)) ----------------------------------*/
              } /* ---7- END OF else (if(..)) ----------------------------------*/
            } /* -----6- END OF else(..) ---------------------------------------*/
          } /* -----5- END OF else(..) ---------------------------------------*/
        } /* -------4- END OF else (if(..)) ----------------------------------*/
      } /* ---------3- END OF else (if(..)) ----------------------------------*/
    } /* -----------2- END OF else (if(..)) ----------------------------------*/
  } /* -------------1- END OF else (if(..)) ----------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_RequestDownload_UB' +-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $34 - RequestDownload                                        */
/*  LOCAL ID   :  -                                                           */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRequestDownload_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_RequestDownloadDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_04Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRequestDownload + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  0x20,
  /* 04 - RESPONSE BYTE 02 */  0x00,
  /* 05 - RESPONSE BYTE 03 */  d_CCkwpaudi_MaxBlockLengthInternFlashDownload,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - parameter not correct                                         */
  /*--------------------------------------------------------------------------*/
  /* 11 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 12 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 13 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 14 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRequestDownload,
  /* 15 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - invalid format 0x13                                           */
  /*--------------------------------------------------------------------------*/
  /* 16 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 17 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 18 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 19 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRequestDownload,
  /* 20 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - conditions not correct                                        */
  /*--------------------------------------------------------------------------*/
  /* 16 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 17 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 18 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 19 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRequestDownload,
  /* 20 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 21 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 22 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 05 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 23 - POST CONDIDTION  */  d_CCglo_NoPostCondDefined,
  /* 24 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 25 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 26 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRequestDownload,
  /* 27 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 06 - Program Failure                                              */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRequestDownload,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCGeneralProgFailure
}; /*--- END OF TABLE --------------------------------------------------------*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_TransferData_UB                             |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $36 - TransferData                            |
|                  Local ID  :  -                                            |
|                  Command ID:  -                                            |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_TransferData_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                      PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE         t_Result_UB;
  UBYTE         t_NumberOfData_UB;
  UWORD         t_NumberOfIntermediateBuffer_UW;
  UWORD         t_sendBytes_UW;
  UWORD         t_remain_length_UW;

#ifdef d_CCkwp_PageWrite
  UWORD         t_CollectDataLength_UW;
  PTR_BUFFER_UB t_DataPtr_PUB;
#endif

  NVMACCESS_ST  t_NVMDirectAccess_ST;

  VALCONTROL_ST t_CCkwpaudi_ValWorking_ST;

  tHis_Header   *pHisHeader;
  void         (*_FlashWrite) (tFlashParam*);

#ifdef cs_Signatur_Aktiv
  #ifdef cs_Decryption_Aktiv
    ULONG t_Address_UL;
  #endif
#endif

  UBYTE t_CCkwpaudi_IndexByte_UB;
  UWORD t_CCkwpaudi_ActualDataWithinBuffer_UW;
  UWORD t_CCkwpaudi_ActualDataForWriting_UW;

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= 0x00;

  /*--------------------------------------------------------------------------*/
  /* check the addressing mode for functional addressing                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

  /*==========================================================================*/
  /* -1- service is functionally addressed                                    */
  /*==========================================================================*/
  if ( d_CCglo_Ready == t_Result_UB )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* -----------------1- END OF if(..) -------------------------------------*/
  /*==========================================================================*/
  /* -1- service is not functionally addressed                                */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 1                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

  } /* -----------------1- END OF else(..) -----------------------------------*/

  /*==========================================================================*/
  /* -1- Too few data bytes; there have to be three bytes at least:       */
  /*     SID + blockSequenceCounter + 1 data byte else NRC0x31                */
  /*==========================================================================*/
  if ( (UBYTE)0x03u > t_InBuffer_PBUF[d_CCglo_DLC] ) /* [OPL336] */
  {
    /*========================================================================*/
    /* -2- Too few data bytes; there have to be two bytes at least:           */
    /*     SID + blockSequenceCounter + 1 data byte else NRC 0x13             */
    /*========================================================================*/
    if ( (UBYTE)0x02u > t_InBuffer_PBUF[d_CCglo_DLC] )
    {
      /*----------------------------------------------------------------------*/
      /* Set return value - Index 04 [OPL368] imx                             */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);
    } /* -------------2- END OF if(..) ---------------------------------------*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* Set return value - Index 01 [OPL368] imx                           */
      /*----------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);
    } /* -----------2- END OF if(..) -----------------------------------------*/
  } /* -------------1- END OF if(..) -----------------------------------------*/

  /*==========================================================================*/
  /* -1- check the block sequence counter: it is not the previous block       */
  /*==========================================================================*/
  else
  {
    /*==========================================================================*/
    /* -2- check the block sequence counter: this is the previous block which   */
    /*     had already been programmed  --> ignore it                           */
    /*==========================================================================*/
    if (t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01])
    {
    /*------------------------------------------------------------------------*/
    /* Set the DLC for the OutBuffer                                          */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_02Byte;

    /*------------------------------------------------------------------------*/
    /* Send the positiv Response                                              */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] =   \
                               d_CCkwpbase_SIDTransferData + (UBYTE) 0x40u;
    t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] =   \
                        t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB;

    /*------------------------------------------------------------------------*/
    /* Set return value - Index 00                                            */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

    } /* -----------------1- END OF if(..) -------------------------------------*/
    /*==========================================================================*/
    /* -1- check the block sequence counter: it is not the previous block       */
    /*==========================================================================*/
    else
    {
      /*========================================================================*/
      /* -2- check the block sequence counter: it is not the expected block     */
      /*========================================================================*/
      if ( (UBYTE)(t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB + 0x01) != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01])
      {
        /*----------------------------------------------------------------------*/
        /* Set return value - Index 03                                          */
        /*----------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

      } /* ---------------2- END OF if(..) -------------------------------------*/
      /*======================================================================*/
      /* -2- Byte count correct                                               */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Get the number of data which should be programed                   */
        /*--------------------------------------------------------------------*/
        t_NumberOfData_UB = ((t_InBuffer_PBUF[d_CCglo_DLC]) - 0x02);

        /*--------------------------------------------------------------------*/
        /* Get the number of data in Intermediate Buffer                      */
        /*--------------------------------------------------------------------*/
        #ifdef d_CCkwp_PageWrite
          t_NumberOfIntermediateBuffer_UW = FUN_CCglo_GetIntermediateBufferNumbers_V ();
        #else
          t_NumberOfIntermediateBuffer_UW = 0x00;
        #endif

        /*====================================================================*/
        /* -4- Too much data - bytecount not correct                          */
        /*     NOTE: NumberOgBytes = DLC - SID - blockSequenceCounter         */
        /*           ActEndAddress = ActAddress + NbrOfData - 1               */
        /*====================================================================*/
        if ( t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL  <                     \
           ( (t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL +  (ULONG) t_NumberOfData_UB + (ULONG) t_NumberOfIntermediateBuffer_UW) - 0x00000001ul))
        {
          /*==================================================================*/
          /* All Data transfered? [OPL371] imx                                */
          /*==================================================================*/
          if (t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL == \
              t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL - (ULONG)0x01u)
          {
            /*----------------------------------------------------------------*/
            /* All Data transfered, send a sequenceError [OPL371] imx         */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex07);
          }
          else
          {
            /*----------------------------------------------------------------*/
            /* Set return value - Index 01                                    */
            /*----------------------------------------------------------------*/
          }
        } /* -----------4- END OF if(..) -------------------------------------*/

        /*====================================================================*/
        /* -4- Bytecount is correct                                           */
        /*====================================================================*/
        else
        {
          /*==================================================================*/
          /*                                                                  */
          /* -5- Download to internal flash memory                            */
          /*                                                                  */
          /*==================================================================*/
          if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagInternFlashDownload)  \
            ==  d_CCkwpaudi_FlagInternFlashDownload )
          {
            /*================================================================*/
            /* -6- Too much data during one message                           */
            /*     data length - SID - blockSequenceCounter                   */
            /*================================================================*/
            if ( (d_CCkwpaudi_MaxBlockLengthInternFlashDownload - 2 )< t_NumberOfData_UB)
            {
              /* Set return value - Index 01                                  */


            } /* -------6- END OF if(..) -------------------------------------*/

            /*================================================================*/
            /* -6- Data length is ok                                          */
            /*================================================================*/
            else
            {

            #ifdef d_CCkwp_PageWrite

              t_CollectDataLength_UW =
              FUN_CCglo_CollectData_UB ( &t_InBuffer_PBUF[0], \
                                         (PTR_BUFFER_UB) &t_DataPtr_PUB  );

              if (t_CollectDataLength_UW < d_CCconprj_PageLength)
              {
                t_CollectDataLength_UW = FUN_CCglo_GetIntermediateBufferNumbers_V();
              }
              /*==============================================================*/
              /* -7- collected data has page length                           */
              /*     === parrot last block doesnt need pagesize ====          */
              /*==============================================================*/
              if ((t_CollectDataLength_UW == d_CCconprj_PageLength)\
                ||((t_CollectDataLength_UW) == \
                       (t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL\
                       - t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL+1)))
              {

#ifdef cs_Signatur_Aktiv
  #ifdef cs_Decryption_Aktiv
                /*============================================================*/
                /* -8- calculate over whole collected data                    */
                /*============================================================*/
                for (t_Address_UL = 0x00ul; \
                     t_Address_UL < (ULONG)t_CollectDataLength_UW; \
                     t_Address_UL = t_Address_UL + (ULONG) d_CCkwpaudi_SignaturePackedLength)
                {
                  /*----------------------------------------------------------*/
                  /* set address and length of each block                     */
                  /*----------------------------------------------------------*/
                  g_SignatureType.sigSourceBuffer = (const UBYTE *) (t_DataPtr_PUB + t_Address_UL);
                  g_SignatureType.sigByteCount    = (UWORD) d_CCkwpaudi_SignaturePackedLength;
                  /*==========================================================*/
                  /* Do Calculation                                           */
                  /*==========================================================*/
                  SecM_VerifySignature(&g_SignatureType);
                  /*----------------------------------------------------------*/
                  /* check the system timer                                   */
                  /*----------------------------------------------------------*/
                  FUN_CCtim_SystemTimerCheck_V ();
                } /* --8- END OF for(..) -------------------------------------*/
  #endif
#endif
                /*------------------------------------------------------------*/
                /* Decrypt data if necessary                                  */
                /*------------------------------------------------------------*/
                t_Result_UB = FUN_CCdp_DataPrepareAudi_UB ((PTR_BUFFER_UB)t_DataPtr_PUB, \
                                               &g_CCdp_WorkingBuffer_BUF[0], \
                                              (ULONG) t_CollectDataLength_UW);

                /*==============================================================*/
                /* -8- Checksum calculation on-the-fly                          */
                /*==============================================================*/
                if( d_CCglo_FlashMemory != \
                (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
                {
                  /* calculate CRC on CollectBuffer, result saved in global   */
                  /* variable g_CCkwpaudi_ParrotCRC32_UL                      */
                  t_CCkwpaudi_ValWorking_ST.StartAddress_PBUF = (UBYTE *)&g_CCdp_WorkingBuffer_BUF[0];
                  t_CCkwpaudi_ValWorking_ST.EndAddress_PBUF =  (UBYTE *)&g_CCdp_WorkingBuffer_BUF[t_CollectDataLength_UW-1];

                  FUN_CCval_CRC32_onFly_UB(&t_CCkwpaudi_ValWorking_ST,&g_CCkwpaudi_ParrotCRC32_UL);
                }

                /*------------------------------------------------------------*/
                /* Init the data structure for next programming cycle         */
                /*------------------------------------------------------------*/
                FlashParam.length  = t_CollectDataLength_UW;
                FlashParam.data    = &g_CCdp_WorkingBuffer_BUF[0];
            #else
                FlashParam.length  = ((t_InBuffer_PBUF[d_CCglo_DLC]) - 0x02);
                FlashParam.data    =  &t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02];
            #endif
                FlashParam.address = t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL;

                pHisHeader  = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
                _FlashWrite = pHisHeader->pfnFlashWrite;

                FlashParam.wdTriggerFct = (tWDTriggerFct)FUN_CCwtdog_TriggerWatchdogProgMem_V;

                _FlashWrite (&FlashParam);

                /*============================================================*/
                /* -8- Download failed                                        */
                /*============================================================*/
                if (  FlashParam.errorcode != kFlashOK )
                {
                  /* Set return value - Index 02                              */
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

                } /* ---8- END OF if(..) -------------------------------------*/
                /*============================================================*/
                /* -8- Download OK                                            */
                /*============================================================*/
                else
                {
                  /*----------------------------------------------------------*/
                  /* Update actual startaddress                               */
                  /*----------------------------------------------------------*/
                #ifdef d_CCkwp_PageWrite
                  t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL += (ULONG)t_CollectDataLength_UW;
                  t_CollectDataLength_UW = FUN_CCglo_GetIntermediateBufferNumbers_V();
                #else
                  t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL += (ULONG)t_NumberOfData_UB;
                #endif

                  /*----------------------------------------------------------*/
                  /* Update the block sequence counter                        */
                  /*----------------------------------------------------------*/
                  t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB++;

                  /*----------------------------------------------------------*/
                  /* Set the DLC for the OutBuffer                            */
                  /*----------------------------------------------------------*/
                  t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_02Byte;

                  /*----------------------------------------------------------*/
                  /* Send the positiv Response                                */
                  /*----------------------------------------------------------*/
                  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] =   \
                                             d_CCkwpbase_SIDTransferData + (UBYTE) 0x40u;
                  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] =   \
                                      t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB;

                  /*----------------------------------------------------------*/
                  /* Set return value - Index 00                              */
                  /*----------------------------------------------------------*/
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
                  /*----------------------------------------------------------*/
                  /* -9- special parrot handling                              */
                  /* 1) save flashfunctions result in kwp control structure   */
                  /* 2) call function again if data isnt send complete        */
                  /*----------------------------------------------------------*/
                  if( d_CCglo_FlashMemory != \
                    (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
                  {
#ifdef d_CCkwp_PageWrite
                  /* the remaining prog length is to short for a page and (but)*/
                    /* the dlc shows enough data in inputbuffer               */
                    if ((d_CCconprj_PageLength>(t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL\
                    -t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL))\
                    && ((ULONG)t_CollectDataLength_UW>=(t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL\
                    -t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL)))
                    {
                      /* send response pending and call this function again */
                      t_Result_UB = ( d_CCglo_Busy | d_CCglo_Ready | d_CCglo_ResponseIndex06);
                      /* copy remaining data to start of data in inputbuffer */
                      t_remain_length_UW = (t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL - t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL+1);
                      t_sendBytes_UW = (t_InBuffer_PBUF[d_CCglo_DLC]+4)-t_remain_length_UW;

                      for(t_CCkwpaudi_IndexByte_UB=0x00;t_CCkwpaudi_IndexByte_UB<t_remain_length_UW;t_CCkwpaudi_IndexByte_UB++)
                      {
                        t_InBuffer_PBUF[(UWORD)d_CCglo_StartOfData+2+t_CCkwpaudi_IndexByte_UB]\
                        = t_InBuffer_PBUF[t_sendBytes_UW+t_CCkwpaudi_IndexByte_UB];

                      }
                      t_InBuffer_PBUF[d_CCglo_DLC]=t_remain_length_UW+2;
                      /*------------------------------------------------------*/
                      /* decrement the blocksequence counter to force handling*/
                      /* as a new block.                                      */
                      /*------------------------------------------------------*/
                      t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB--;
                      FUN_CCglo_InitCollectData_V ();

                    }
#endif
                    /*--------------------------------------------------------*/
                    /* -A- store result of flashfunctions in kwp2000 control  */
                    /* structure.                                             */
                    if (  FlashParam.errorcode != kFlashOK )
                    {
                      t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagChecksumCorrect;

                    } /* -A- END OF if(..) -----------------------------------*/
                    else
                    {
                      t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagChecksumCorrect;
                    } /* -A- END OF else(if(..)) -----------------------------*/
                  } /* -9- END OF if(..) -------------------------------------*/


                } /* -8- END OF if(..) ---------------------------------------*/

            #ifdef d_CCkwp_PageWrite

              } /* -----7- END OF if(..) -------------------------------------*/
              /*==============================================================*/
              /* -7- collected data doesn't have page length                  */
              /*==============================================================*/
              else
              {
                /*------------------------------------------------------------*/
                /* Update the block sequence counter                          */
                /*------------------------------------------------------------*/
                t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB++;

                /*------------------------------------------------------------*/
                /* Set the DLC for the OutBuffer                              */
                /*------------------------------------------------------------*/
                t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_02Byte;

                /*------------------------------------------------------------*/
                /* Send the positiv Response                                  */
                /*------------------------------------------------------------*/
                t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] =   \
                                           d_CCkwpbase_SIDTransferData + (UBYTE) 0x40u;
                t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] =   \
                                    t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB;

                /*------------------------------------------------------------*/
                /* Set return value - Index 00                                */
                /*------------------------------------------------------------*/
                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

              } /* -----7- END OF else (if(..)) ------------------------------*/

            #endif

            } /* -------6- END OF else (if(..)) ------------------------------*/
          } /* ---------5- END OF if(..) -------------------------------------*/

          /*==================================================================*/
          /*                                                                  */
          /* -5- other Download                                               */
          /*                                                                  */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /*                                                                */
            /* -6- Download to program ram                                    */
            /*                                                                */
            /*================================================================*/
            if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRamDownload)  \
                   == d_CCkwpaudi_FlagRamDownload )
            {
              /*==============================================================*/
              /* -7- Too much data during one message                         */
              /*     data length - SID - blockSequenceCounter                 */
              /*==============================================================*/
              if ( (d_CCkwpaudi_MaxBlockLengthRamDownload - 2 )< t_NumberOfData_UB)
              {
                /* Set return value - Index 01                                */

              } /* -----7- END OF if(..) -------------------------------------*/

              /*==============================================================*/
              /* -7- Data length is ok                                        */
              /*==============================================================*/
              else
              {

                t_CollectDataLength_UW =
                FUN_CCglo_CollectData_UB (                &t_InBuffer_PBUF[0], \
                                          (PTR_BUFFER_UB) &t_DataPtr_PUB  );

                /*------------------------------------------------------------*/
                /* Set return value - Index 00                                */
                /*------------------------------------------------------------*/
                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                /*============================================================*/
                /* -8- collected data has page length                         */
                /*============================================================*/
                if (t_CollectDataLength_UW == d_CCconprj_PageLength)
                {

#ifdef cs_Signatur_Aktiv
  #ifdef cs_Decryption_Aktiv
                  /*----------------------------------------------------------*/
                  /* set address and length of each block                     */
                  /*----------------------------------------------------------*/
                  g_SignatureType.sigSourceBuffer = (const UBYTE *) t_DataPtr_PUB;
                  g_SignatureType.sigByteCount    = t_CollectDataLength_UW;

                  /*==========================================================*/
                  /* Do Calculation                                           */
                  /*==========================================================*/
                  SecM_VerifySignature(&g_SignatureType);
  #endif
#endif

                  /*----------------------------------------------------------*/
                  /* Decrypt data if necessary                                */
                  /*----------------------------------------------------------*/
                  t_Result_UB = FUN_CCdp_DataPrepareAudi_UB ((PTR_BUFFER_UB)t_DataPtr_PUB, \
                                               &g_CCdp_WorkingBuffer_BUF[0], \
                                              (ULONG) d_CCconprj_PageLength);

                  /*==========================================================*/
                  /* Download to program ram                                  */
                  /*==========================================================*/
                  FUN_CCglo_PMemCopy_V( (FAR_PTR_UBYTE) t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL,
                                                        &g_CCdp_WorkingBuffer_BUF[0],
                                                        (ULONG) d_CCconprj_PageLength);

                  /*----------------------------------------------------------*/
                  /* Update actual startaddress                               */
                  /*----------------------------------------------------------*/
                  t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL += (ULONG)d_CCconprj_PageLength;

                } /* -8- END OF else if(..) ----------------------------------*/

                /*------------------------------------------------------------*/
                /* Update the block sequence counter                          */
                /*------------------------------------------------------------*/
                t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB++;

                /*------------------------------------------------------------*/
                /* Set the DLC for the OutBuffer                              */
                /*------------------------------------------------------------*/
                t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_02Byte;

                /*------------------------------------------------------------*/
                /* Send the positiv Response                                  */
                /*------------------------------------------------------------*/
                t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] =   \
                                           d_CCkwpbase_SIDTransferData + (UBYTE) 0x40u;
                t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] =   \
                                    t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB;

              } /* -----7- END OF else (if(..)) ------------------------------*/
            } /* -------6- END OF else (if(..)) ------------------------------*/
            /*================================================================*/
            /*                                                                */
            /* -6- EEPROM Download                                            */
            /*                                                                */
            /*================================================================*/
            else
            {
              /*============================================================*/
              /* -8- Too much data during one message                       */
              /*     data length - SID - blockSequenceCounter               */
              /*============================================================*/
              if ( (d_CCkwpaudi_MaxBlockLengthEepromDownload - 2 )< t_NumberOfData_UB)
              {
                /* Set return value - Index 01                              */

              } /* -----8- END OF if(..) -----------------------------------*/

              /*============================================================*/
              /* -8- Data length is ok                                      */
              /*============================================================*/
              else
              {
                /*==========================================================*/
                /* -9- Function was called a first time                     */
                /*==========================================================*/
                if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
                  d_CCkwpaudi_FlagRoutineBusy )
                {
                  /*--------------------------------------------------------*/
                  /* Set the internal busy flag                             */
                  /*--------------------------------------------------------*/
                  t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagRoutineBusy;

                  /*--------------------------------------------------------*/
                  /* response pending                                       */
                  /*--------------------------------------------------------*/
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex06);

                } /* ---9- END OF if(..) -----------------------------------*/

                /*==========================================================*/
                /* -9- Function was called one more time                    */
                /*==========================================================*/
                else
                {
                  /*--------------------------------------------------------*/
                  /* Reset the internal busy flag                           */
                  /*--------------------------------------------------------*/
                  t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

                  /*--------------------------------------------------------*/
                  /* Get the number of data which should be programed       */
                  /*--------------------------------------------------------*/
                  t_NumberOfData_UB = ((t_InBuffer_PBUF[d_CCglo_DLC]) - 0x02);    /* [OPL323] */

                  /*--------------------------------------------------------*/
                  /* -A- copy all data from input buffer into collect buffer*/
                  /*     === this is necessary because of the limitation ===*/
                  /*     === of the AES algorithm, that there could only ===*/
                  /*     === be decrypted 16Bytes at once                ===*/
                  /*--------------------------------------------------------*/
                  for ( t_CCkwpaudi_IndexByte_UB = (UBYTE) 0x00u; \
                        t_CCkwpaudi_IndexByte_UB < t_NumberOfData_UB; \
                        t_CCkwpaudi_IndexByte_UB++ )
                  {
                    /*------------------------------------------------------*/
                    /* copy one byte                                        */
                    /*------------------------------------------------------*/
                    t_CC_CollectBuffer_BUF[(UWORD)g_CCkwpaudi_EepromDataLeftOver_UB + (UWORD)t_CCkwpaudi_IndexByte_UB] = \
                      t_InBuffer_PBUF[(UWORD)d_CCglo_StartOfData + 0x0002u + (UWORD)t_CCkwpaudi_IndexByte_UB];

                  } /* ---A- END OF for(..) --------------------------------*/

                  /*--------------------------------------------------------*/
                  /* get size of current data within adlatus                */
                  /*--------------------------------------------------------*/
                  t_CCkwpaudi_ActualDataWithinBuffer_UW = \
                        (  (UWORD) g_CCkwpaudi_EepromDataLeftOver_UB + \
                           (UWORD) t_NumberOfData_UB  );

#ifdef cs_Signatur_Aktiv
                  /*----------------------------------------------------------*/
                  /* update EepromDataLeftOver                                */
                  /*----------------------------------------------------------*/
                  g_CCkwpaudi_EepromDataLeftOver_UB = \
                      (UBYTE) (t_CCkwpaudi_ActualDataWithinBuffer_UW % 0x10u);

  #ifdef cs_Decryption_Aktiv
                  /*----------------------------------------------------------*/
                  /* set address and length of each block                     */
                  /*----------------------------------------------------------*/
                  g_SignatureType.sigSourceBuffer = (const UBYTE *) &t_CC_CollectBuffer_BUF[0x00u];
                  g_SignatureType.sigByteCount    = (t_CCkwpaudi_ActualDataWithinBuffer_UW - (UWORD) g_CCkwpaudi_EepromDataLeftOver_UB);

                  /*==========================================================*/
                  /* Do Calculation                                           */
                  /*==========================================================*/
                  SecM_VerifySignature(&g_SignatureType);
  #endif
#endif

                  /*--------------------------------------------------------*/
                  /* Decrypt data if necessary                              */
                  /*--------------------------------------------------------*/
                  t_Result_UB = FUN_CCdp_DataPrepareAudi_UB ((PTR_BUFFER_UB) &t_CC_CollectBuffer_BUF[0x00u], \
                                      &g_CCdp_WorkingBuffer_BUF[0], \
                                      (ULONG) (t_CCkwpaudi_ActualDataWithinBuffer_UW - (UWORD) g_CCkwpaudi_EepromDataLeftOver_UB));

                  /*--------------------------------------------------------*/
                  /* Initialise return value                                */
                  /*--------------------------------------------------------*/
                  t_Result_UB = d_CCglo_NoError;

                  /*--------------------------------------------------------*/
                  /* Set the number of total bytes to be written            */
                  /* in the actual transferdata session                     */
                  /*--------------------------------------------------------*/
                  t_NVMDirectAccess_ST.TotalBytes_UW = \
                              (UWORD) t_CCkwpaudi_KwpCtrl_ST.ProgLength_UL;

                  /*--------------------------------------------------------*/
                  /* byte used for offset in case of multiple write access  */
                  /*--------------------------------------------------------*/
                  t_CCkwpaudi_IndexByte_UB = (UBYTE) 0x00u;

                  /*--------------------------------------------------------*/
                  /* get number of bytes for writing                        */
                  /*--------------------------------------------------------*/
                  t_CCkwpaudi_ActualDataForWriting_UW = \
                          t_CCkwpaudi_ActualDataWithinBuffer_UW \
                          - (UWORD) g_CCkwpaudi_EepromDataLeftOver_UB;

                  /*--------------------------------------------------------*/
                  /* -A- write eeprom data within 1-2 nvm access            */
                  /*     === this is necessary because of the limitation ===*/
                  /*     === of the NVM handler, that there could only   ===*/
                  /*     === be written 0xFF Bytes at most               ===*/
                  /*--------------------------------------------------------*/
                  do
                  {
                    /*------------------------------------------------------*/
                    /* Init Eeprom Download                                 */
                    /*------------------------------------------------------*/
                    t_NVMDirectAccess_ST.Data_PUB  = \
                            &g_CCdp_WorkingBuffer_BUF[t_CCkwpaudi_IndexByte_UB];
                    t_NVMDirectAccess_ST.Address_UW  = \
                            (UWORD) (t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL \
                                        + (ULONG) t_CCkwpaudi_IndexByte_UB);

                    /*------------------------------------------------------*/
                    /* -B- data could not be written within one nvm access  */
                    /*------------------------------------------------------*/
                    if (t_CCkwpaudi_ActualDataForWriting_UW > 0xF0u)
                    {
                      /*----------------------------------------------------*/
                      /* write maximum size within first access             */
                      /*----------------------------------------------------*/
                      t_NVMDirectAccess_ST.NbrBytes_UB = (UBYTE) 0xF0u;

                      /*----------------------------------------------------*/
                      /* reduce size of data that should be written         */
                      /*----------------------------------------------------*/
                      t_CCkwpaudi_ActualDataForWriting_UW -= 0xF0u;

                      /*----------------------------------------------------*/
                      /* set offset index for second write access           */
                      /*----------------------------------------------------*/
                      t_CCkwpaudi_IndexByte_UB = (UBYTE) 0xF0u;

                    } /* -B- END OF if(..) ---------------------------------*/
                    /*------------------------------------------------------*/
                    /* -B- data could be written within one nvm access      */
                    /*------------------------------------------------------*/
                    else
                    {
                      /*----------------------------------------------------*/
                      /* write all bytes within buffer                      */
                      /*----------------------------------------------------*/
                      t_NVMDirectAccess_ST.NbrBytes_UB = (UBYTE) t_CCkwpaudi_ActualDataForWriting_UW;

                      /*----------------------------------------------------*/
                      /* reset size of data within buffer                   */
                      /*----------------------------------------------------*/
                      t_CCkwpaudi_ActualDataForWriting_UW = 0x00;

                    } /* -B- END OF else(..) -------------------------------*/

                    /*------------------------------------------------------*/
                    /* Call the NVM access                                  */
                    /*------------------------------------------------------*/
                    t_Result_UB |= FUN_CCnvm_NvmDirectWriteAccess_UB (t_NVMDirectAccess_ST);

                  } /* -A- END OF do(..) -----------------------------------*/
                  /*--------------------------------------------------------*/
                  /* recure if there are more data which should be written  */
                  /*--------------------------------------------------------*/
                  while (t_CCkwpaudi_ActualDataForWriting_UW > 0x00u);

                  /*========================================================*/
                  /* -A- NVM access was successful                          */
                  /*========================================================*/
                  if (d_CCglo_NoError == t_Result_UB)
                  {
                    /*------------------------------------------------------*/
                    /* -B- copy last bytes to start of collect buffer       */
                    /*------------------------------------------------------*/
                    for ( t_CCkwpaudi_IndexByte_UB = (UBYTE) 0x00u; \
                          t_CCkwpaudi_IndexByte_UB < g_CCkwpaudi_EepromDataLeftOver_UB; \
                          t_CCkwpaudi_IndexByte_UB++ )
                    {
                      /*----------------------------------------------------*/
                      /* copy one byte                                      */
                      /*----------------------------------------------------*/
                      t_CC_CollectBuffer_BUF[t_CCkwpaudi_IndexByte_UB] = \
                      t_CC_CollectBuffer_BUF[(t_CCkwpaudi_ActualDataWithinBuffer_UW - (UWORD)g_CCkwpaudi_EepromDataLeftOver_UB) + (UWORD) t_CCkwpaudi_IndexByte_UB];
                    } /* ---B- END OF for(..) ------------------------------*/

                    /*------------------------------------------------------*/
                    /* Update actual startaddress                           */
                    /*------------------------------------------------------*/
                    t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL += (ULONG) (t_CCkwpaudi_ActualDataWithinBuffer_UW - (UWORD) g_CCkwpaudi_EepromDataLeftOver_UB);

                    /*------------------------------------------------------*/
                    /* Update the block sequence counter                    */
                    /*------------------------------------------------------*/
                    t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB++;

                    /*--------------------------------------------------------*/
                    /* Set the DLC for the OutBuffer                          */
                    /*--------------------------------------------------------*/
                    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_02Byte;

                    /*--------------------------------------------------------*/
                    /* Send the positiv Response                              */
                    /*--------------------------------------------------------*/
                    t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] =   \
                                  d_CCkwpbase_SIDTransferData + (UBYTE) 0x40u;
                    t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] =   \
                                  t_CCkwpaudi_KwpCtrl_ST.BlockSeqCounter_UB;

                    /*--------------------------------------------------------*/
                    /* Set return value - Index 00                            */
                    /*--------------------------------------------------------*/
                    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                  } /* -A- END OF if(..) -------------------------------------*/
                  /*==========================================================*/
                  /* -A- NVM access was not successful                        */
                  /*==========================================================*/
                  else
                  {
                    /*--------------------------------------------------------*/
                    /* Set return value - Index 02                            */
                    /*--------------------------------------------------------*/
                    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

                  } /* -A- END OF else (if(..)) ------------------------------*/
                } /* ---9- END OF else (..) ----------------------------------*/
              } /* -----8- END OF else (if(..)) ------------------------------*/
            } /* -------6- END OF else (if(..)) ------------------------------*/
          } /* ---------5- END OF else (if(..)) ------------------------------*/
        } /* -----------4- END OF else (if(..)) ------------------------------*/
      } /* -------------3- END OF else (if(..)) ------------------------------*/
    } /* ---------------2- END OF else (if(..)) ------------------------------*/
  } /* -----------------1- END OF else (if(..)) ------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_TransferData_UB' -+-+-+-+-+-+-+-+-+-+-+-*/

/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $36 - TransferData                                           */
/*  LOCAL ID   :  -                                                           */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTTransferData_AUB[] = \
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_TransferDataDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01                                                                 */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 04 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 05 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDTransferData,
  /* 06 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Programming failed                                            */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDTransferData,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCGeneralProgFailure,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - wrong block sequence counter                                  */
  /*--------------------------------------------------------------------------*/
  /* 12 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 13 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 14 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 15 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDTransferData,
  /* 16 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCwrongBlockSequenceCounter,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 - invalid format                                                */
  /*--------------------------------------------------------------------------*/
  /* 12 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 13 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 14 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 15 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDTransferData,
  /* 16 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat,
  /*--------------------------------------------------------------------------*/
  /* INDEX 05 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 17 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 18 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 06 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 19 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 20 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 21 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 22 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDTransferData,
  /* 23 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 07 - Sequence Error                                                */
  /*--------------------------------------------------------------------------*/
  /* 24 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 25 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 26 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 27 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDTransferData,
  /* 28 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestSequenceError0x24
}; /*--- END OF TABLE --------------------------------------------------------*/




/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_RequestTransferExit_UB                      |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $37 - RequestTransferExit                     |
|                  Local ID  :  -                                            |
|                  Command ID:  -                                            |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RequestTransferExit_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                             PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;
  tHis_Header   *pHisHeader;
  void         (*_FlashDeinit) (tFlashParam*);

  /* The next lines are only to prevent a compiler warning!!                  */
  t_InBuffer_PBUF[0]  |= 0x00;
  t_OutBuffer_PBUF[0] |= 0x00;

  /*--------------------------------------------------------------------------*/
  /* set return value - will be overwritten by an error or other state        */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  /*==========================================================================*/
  /* -1- Not all data subscripted by request download are transmitted by      */
  /*     trasfer data                                                         */
  /*==========================================================================*/
  if ( (t_CCkwpaudi_KwpCtrl_ST.ActProgAddress_UL - (ULONG) 0x01) != \
                                        t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL)
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 1 [OPL372] imx, 0x22 -> 0x24         */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

  } /* -1- END OF if(..) -----------------------------------------------------*/
  /*==========================================================================*/
  /* -1- All data subscripted by request download are transmitted by          */
  /*     transfer data                                                        */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* Allow RequestDownload again, after a successfull download              */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~(d_CCkwpaudi_FlagRequestDownloadActive);
    /*------------------------------------------------------------------------*/
    /* Allow CheckMemory now                                                  */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagReadyforCheckMemory;
    /*========================================================================*/
    /* -2- Inform about succesful RAM Download                                */
    /*========================================================================*/
    if ( ( d_CCkwpaudi_FlagRamDownload     ==                                      \
          (d_CCkwpaudi_FlagRamDownload     &  t_CCkwpaudi_KwpCtrl_ST.State_UW )) && \
         ( d_CCkwpaudi_FlagRamDownloadDone !=                                      \
          (d_CCkwpaudi_FlagRamDownloadDone &  t_CCkwpaudi_KwpCtrl_ST.State_UW ))    )
    {
      /* Set Flag                                                             */
      t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagRamDownloadDone;

    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- FLASH Download - Deinit HIS Driver                                 */
    /*========================================================================*/
    if ( (d_CCkwpaudi_FlagInternFlashDownload ==                                     \
         (d_CCkwpaudi_FlagInternFlashDownload  &  t_CCkwpaudi_KwpCtrl_ST.State_UW )) ||
         (d_CCkwpaudi_FlagExternFlashDownload ==                                    \
         (d_CCkwpaudi_FlagExternFlashDownload  &  t_CCkwpaudi_KwpCtrl_ST.State_UW ))  )
    {
      pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
      _FlashDeinit = pHisHeader->pfnFlashDeinit;

      _FlashDeinit (&FlashParam);
    } /* -2- END OF if(..) ---------------------------------------------------*/
  } /* ---1- END OF else (if(..)) --------------------------------------------*/


  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_RequestTransferExit_UB' -+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $37 - RequestTransferExit                                    */
/*  LOCAL ID   :  -                                                           */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRequestTransferExit_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_RequestTransferExitDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_01Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRequestTransferExit + (UBYTE) 0x40u),
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Data Count Error                                              */
  /*--------------------------------------------------------------------------*/
  /* 03 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 04 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 05 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 06 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRequestTransferExit,
  /* 07 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestSequenceError0x24
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_RcCheckProgramPreCond_UB                     |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    Service Function for the KWP2000 Service:                  |
|                                                                             |
|                  Service ID: $31 - Routine control                          |
|                  Local ID  : $xx - Routine control type                     |
|                  Routine ID: $02 -+                                         |
|                  Routine ID: $03 -+- Check programming preconditions        |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RcCheckProgramPreCond_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                               PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /* The next lines are only to prevent a compiler warning!!                  */
  t_OutBuffer_PBUF[0] |= 0x00;

  /*==========================================================================*/
  /* -1- check the routine control type                                       */
  /*==========================================================================*/
  switch (d_CCglo_LIDSubFunctionMask & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01])
  {
    /*------------------------------------------------------------------------*/
    /* start routine                                                          */
    /*------------------------------------------------------------------------*/
  case d_CCkwpbase_LIDStartRoutine:

    /*========================================================================*/
    /* -2- positiv response shall be suppressed                               */
    /*========================================================================*/
    if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                 t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) )
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

    } /* ---2- END OF if(..) -------------------------------------------------*/
    /*========================================================================*/
    /* -2- positiv response shall be send                                     */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

    } /* ---2- END OF else (if(..)) ------------------------------------------*/


    break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /* all other routine control types are not supported                      */
    /*------------------------------------------------------------------------*/
  default:

    /*------------------------------------------------------------------------*/
    /* check the addressing mode for functional addressing                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

    /*========================================================================*/
    /* -2- service is functionally addressed                                  */
    /*========================================================================*/
    if ( d_CCglo_Ready == t_Result_UB )
    {
      /*------------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state      */
      /*------------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

      /*------------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                   */
      /*------------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

    } /* ---2- END OF if(..) -------------------------------------------------*/
    /*========================================================================*/
    /* -2- service is not functionally addressed                              */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* set return value - response index 1                                  */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    } /* ---2- END OF else (if(..)) ------------------------------------------*/


    break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  } /* -1- END OF switch (..) ------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_RcCheckProgramPreCond_UB +-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $31 - RoutineControl                                         */
/*  LOCAL ID   : $xx                                                          */
/*  Routine ID : $02 -+                                                       */
/*  Routine ID : $01 -+- Check programming preconditions                      */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRcCheckProgramPreCond_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_04Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 04 - Response Byte 02 */  d_CCkwpaudi_RIDchkProgPreCondB0,
  /* 05 - Response Byte 02 */  d_CCkwpaudi_RIDchkProgPreCondB1,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - subfunction not supported                                     */
  /*--------------------------------------------------------------------------*/
  /* 06 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 07 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 08 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 09 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 10 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSubFunctionNotSupported,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 11 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 12 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 13 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 14 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/



/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_RcEraseMemory_UB                            |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $31 - Routine control                         |
|                  Local ID  : $xx - Routine control type                    |
|                  Routine ID: $FF -+                                        |
|                  Routine ID: $00 -+- Erase memory                          |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/

UBYTE FUN_CCkwpaudi_RcEraseMemory_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                       PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE  t_Result_UB;
  UBYTE  t_NbrIndexBytes_UB;
  UNION2 t_MemoryIndex_U2;
  UBYTE  t_Index_UB;
  UBYTE  t_ScanState_UB;


  tHis_Header          *pHisHeader;
  void (*_FlashInit)   (tFlashParam*);
  void (*_FlashErase)  (tFlashParam*);
  void (*_FlashDeinit) (tFlashParam*);

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= 0x00;


  /*==========================================================================*/
  /* -1- erasing is done                                                      */
  /*==========================================================================*/
  if (d_CCkwpaudi_FlagErasingDoneOk == (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagErasingDoneOk))
  {
    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagErasingDoneOk;
    t_Result_UB = 0;

  } /* ---------------------1- END OF if(..) ---------------------------------*/
  /*==========================================================================*/
  /* -1- erasing is not done completely                                       */
  /*==========================================================================*/
  else
  {
    /*========================================================================*/
    /* -2- the routine identifier is not correct (check has to be done        */
    /*     because routine identifer includes a 0xFF which cannot be checked  */
    /*     in the session handler                                             */
    /*========================================================================*/
    if( (d_CCkwpaudi_RIDeraseMemoryByte0 != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02]) ||
        (d_CCkwpaudi_RIDeraseMemoryByte1 != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x03])  )
    {
      /*======================================================================*/
      /* -3- must be more than 4 bytes                                        */
      /*======================================================================*/
      if (t_InBuffer_PBUF[d_CCglo_DLC] < (UBYTE)0x05u)
      {
        /*----------------------------------------------------------------------*/
        /* set return value - response index 4                                  */
        /*----------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);
      }
      else
      {
        /*----------------------------------------------------------------------*/
        /* set return value - response index 3                                  */
        /*----------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
      }


    } /* -------------------2- END OF if(..) ---------------------------------*/
    /*========================================================================*/
    /* -2- the routine identifier is correct                                  */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -3- check the routine control type                                   */
      /*======================================================================*/
      switch (d_CCglo_LIDSubFunctionMask & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01])
      {
        /*--------------------------------------------------------------------*/
        /* start routine                                                      */
        /*--------------------------------------------------------------------*/
        case d_CCkwpbase_LIDStartRoutine:

        /*--------------------------------------------------------------------*/
        /* NOTE: Erasing the flash memory needs some time. Each flash memory  */
        /*       has a diffrent characteristic in order to the erase time.    */
        /*       In fact of erase time is longer as the communication timeout */
        /*       the tester will detect a communication timeout. To prevent   */
        /*       this case we will send ResponsePending. The first Response   */
        /*       Pending is send from this routine. The further ones are send */
        /*       from the watchdog routine.                                   */
        /*--------------------------------------------------------------------*/

        /*====================================================================*/
        /* -4- evaluate how many calls had been made of this function         */
        /*====================================================================*/
        switch (t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB)
        {

          /*------------------------------------------------------------------*/
          /*                                                                  */
          /*  first call                                                      */
          /*                                                                  */
          /*  For RAM: delete RAM --> ready                                   */
          /*  For Flash and EEPROM: send Response Pending                     */
          /*                        --> RoutineCallCounter = 0x01u            */
          /*------------------------------------------------------------------*/
        case 0x00u:

          /*------------------------------------------------------------------*/
          /* evaluate the LengthInformationIdentifier                         */
          /*------------------------------------------------------------------*/
          t_NbrIndexBytes_UB = (0x0F & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]);

          /*==================================================================*/
          /* -5- The number of bytes for the index of the sw block is         */
          /*     incorrect or the number of memory size bytes is not zero     */
          /*==================================================================*/
          if ( (0x00 == t_NbrIndexBytes_UB) || ( 0x02 < t_NbrIndexBytes_UB ) ||
            ( 0x00 != ( 0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04] ) ) )
          {
            /*================================================================*/
            /* -6- Service length is not correct                              */
            /*================================================================*/
            /* [OPL355], Routine Control has problem with check length        */
            if ( ((d_CCglo_05Byte + t_NbrIndexBytes_UB) != t_InBuffer_PBUF[d_CCglo_DLC]) && \
                 (((0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]) >> 4) == 0))
            {
              /*--------------------------------------------------------------*/
              /* set return value - response index 4                          */
              /*--------------------------------------------------------------*/
              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);

            }
            else
            {
              /*--------------------------------------------------------------*/
              /* set return value - response index 3                          */
              /*--------------------------------------------------------------*/
              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
            }

          } /* -------------5- END OF if(..) ---------------------------------*/
          /*==================================================================*/
          /* -5- The number of bytes for the index of the sw block is correct */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /* -6- Service length is not correct                              */
            /*================================================================*/
            /* [OPL355], Routine Control has problem with check length        */
            if ( ((d_CCglo_05Byte + t_NbrIndexBytes_UB) != t_InBuffer_PBUF[d_CCglo_DLC]) && \
                 (((0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]) >> 4) == 0))
            {
              /* Set return value - Index 04                                  */
              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);

            } /* -----------6- END OF if(..) ---------------------------------*/

            /*================================================================*/
            /* -6- Service length is correct                                  */
            /*================================================================*/
            else
            {
              /*==============================================================*/
              /* -7- We are not in the programming session                    */
              /*==============================================================*/
              if (t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB !=  (UBYTE)0x02u)
              {
                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
              }
              /*==============================================================*/
              /* -7- We are in the programming session                        */
              /*==============================================================*/
              else
              {
                /*============================================================*/
                /* -8- We dont pass security access                           */
                /*============================================================*/
                if(d_CCkwpaudi_FlagSecurityAccessGranted != \
                  (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSecurityAccessGranted))
                {
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex10);
                }
                /*============================================================*/
                /* -8- We pass security access                                */
                /*============================================================*/
                else
                {
                  /*==========================================================*/
                  /* -9- We dont pass fingerprint                             */
                  /*==========================================================*/
                  if (d_CCkwpaudi_FlagFPDoneOk != \
                   (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagFPDoneOk))
                  {
                    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex11);
                  }
                  /*==========================================================*/
                  /* -9- We pass fingerprint                                  */
                  /*==========================================================*/
                  else
                  {
                    /*========================================================*/
                    /* -A- Memory index consists of 1 byte                    */
                    /*========================================================*/
                    if ( (UBYTE)0x01u == t_NbrIndexBytes_UB )
                    {
                      /*------------------------------------------------------*/
                      /* Get the memory index                                 */
                      /*------------------------------------------------------*/
                      t_MemoryIndex_U2.UByte_ST.UByte0_UB = 0;
                      t_MemoryIndex_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x05];

                    } /* ---------A- END OF if(..) ---------------------------*/
                    /*========================================================*/
                    /* -A- Memory index consists of 2 bytes                   */
                    /*========================================================*/
                    else
                    {
                      /*------------------------------------------------------*/
                      /* Get the memory index                                 */
                      /*------------------------------------------------------*/
                      t_MemoryIndex_U2.UByte_ST.UByte0_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x05];
                      t_MemoryIndex_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x06];

                    } /* ---------A- END OF else(..) -------------------------*/

                    /*--------------------------------------------------------*/
                    /* Init internal variables                                */
                    /*--------------------------------------------------------*/
                    t_ScanState_UB = d_CCkwpaudi_ScanRun;
                    t_Index_UB     = 0x00;

                    /*========================================================*/
                    /* -A- Check the complete Table                           */
                    /*========================================================*/
                    while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
                            ( d_CCglo_LastEntry !=                 \
                            (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.  \
                            DownloadMap_PST[t_Index_UB].MemoryInfo_UW )))
                    {
                      /*======================================================*/
                      /* -B- Memory index found                               */
                      /*======================================================*/
                      if ( t_MemoryIndex_U2.UWord_ST.UWord0_UW == t_CCkwpaudi_KwpCtrl_ST.  \
                               DownloadMap_PST[t_Index_UB].MemoryIndex_UW )
                      {
                        /*----------------------------------------------------*/
                        /* stop scan                                          */
                        /*----------------------------------------------------*/
                        t_ScanState_UB   = d_CCkwpaudi_ScanStop;
                        t_MemoryIndex_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x05];

                      } /* -------8- END OF if(..) ---------------------------*/

                      /*======================================================*/
                      /* -B- Memory index not found                           */
                      /*======================================================*/
                      else
                      {
                        /*----------------------------------------------------*/
                        /* Check next entry                                   */
                        /*----------------------------------------------------*/
                        t_Index_UB += 0x01;

                      } /* -------B- END OF else(..) -------------------------*/
                    } /* ---------A- END OF while(..) ------------------------*/

                    /*========================================================*/
                    /* -A- Memory index NOT found in the download map         */
                    /*========================================================*/
                    if ( d_CCglo_LastEntry ==                 \
                        (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.  \
                         DownloadMap_PST[t_Index_UB].MemoryInfo_UW ))
                    {
                      /*------------------------------------------------------*/
                      /* set return value - response index 3                  */
                      /*------------------------------------------------------*/
                      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

                    } /* ---------A- END OF if(..) ---------------------------*/
                    /*========================================================*/
                    /* -A- Memory index found in the download map             */
                    /*========================================================*/
                    else
                    {

                      /*------------------------------------------------------*/
                      /* store the actual used download map index             */
                      /*------------------------------------------------------*/
                       t_CCkwpaudi_KwpCtrl_ST.DownloadMapIndex_UB = t_Index_UB;

                      /*======================================================*/
                      /*                                                      */
                      /* -B- erasing of RAM memory                            */
                      /*                                                      */
                      /*======================================================*/
                      if ( d_CCglo_RamMemory == (d_CCglo_MemoryTypeMask &  \
                            t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW))
                     {
                        /*====================================================*/
                        /* -C- positiv response shall be suppressed           */
                        /*====================================================*/
                        if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                                  t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) )
                        {
                          /*--------------------------------------------------*/
                          /* set return value - will be overwritten by an     */
                          /* error or other state                             */
                          /*--------------------------------------------------*/
                          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex08);

                          /*--------------------------------------------------*/
                          /* Set the DLC of the OutBuffer to zero             */
                          /*--------------------------------------------------*/
                          t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;


                        } /* -----C- END OF if(..) ---------------------------*/
                        /*====================================================*/
                        /* -C- positiv response shall be send                 */
                        /*====================================================*/
                        else
                        {
                          /*--------------------------------------------------*/
                          /* Send a positive response                         */
                          /*--------------------------------------------------*/
                          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                        } /* -----C- END OF else(..) -------------------------*/
                      } /* -------B- END OF if(..) ---------------------------*/

                      /*======================================================*/
                      /*                                                      */
                      /* -B- other memory type flash or eeprom                */
                      /*                                                      */
                      /*======================================================*/
                      else
                      {
                        /*----------------------------------------------------*/
                        /* write start address of memory entry                */
                        /*----------------------------------------------------*/
                        t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL =   \
                        t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL;

                        /*----------------------------------------------------*/
                        /* write end address of memory entry                  */
                        /*----------------------------------------------------*/
                        t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL =   \
                          t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL;

                        /*----------------------------------------------------*/
                        /* Calculate the erase length                         */
                        /*----------------------------------------------------*/
                        t_CCkwpaudi_KwpCtrl_ST.EraseLength_UL = \
                        (t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL - t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL);
                        t_CCkwpaudi_KwpCtrl_ST.EraseLength_UL += 0x01ul;

                        /*====================================================*/
                        /* -C- End address is lower than start address        */
                        /*====================================================*/
                        if ( t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL < t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL)
                        {
                          /*--------------------------------------------------*/
                          /* set return value - response index 5              */
                          /*--------------------------------------------------*/
                          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

                        } /* -----C- END OF if(..) ---------------------------*/

                        /*====================================================*/
                        /* -C- EraseAddresses are valid                       */
                        /*====================================================*/
                        else
                        {
                          /*==================================================*/
                          /*                                                  */
                          /* -D- erasing of flash memory                      */
                          /*                                                  */
                          /*==================================================*/
                          if (( d_CCglo_FlashMemory == (d_CCglo_MemoryTypeMask &  \
                              t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW))\
                            || ( d_CCglo_Parrot == (d_CCglo_MemoryTypeMask &  \
                              t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW))\
                            || ( d_CCglo_Parrot_Boot == (d_CCglo_MemoryTypeMask &  \
                              t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW))\
                            || ( d_CCglo_Parrot_Upd == (d_CCglo_MemoryTypeMask &  \
                              t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW)))
                          {
                            /*------------------------------------------------*/
                            /* Check SW protected areas                       */
                            /*------------------------------------------------*/
                            t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW = FUN_CCkwpaudi_LogicalAddressCheck_UW (     \
                              t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL,  \
                              t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL,    \
                              d_CCglo_Erase                               );

                            /*================================================*/
                            /* -E- Addresscheck was not correct               */
                            /*================================================*/
                            if ( d_CCglo_AddressCheckNOK == \
                             (d_CCglo_AddressCheckMask & t_CCkwpaudi_KwpCtrl_ST.MemoryInfo_UW))
                            {
                              /*----------------------------------------------*/
                              /* set return value - response index 5          */
                              /*----------------------------------------------*/
                              /* [OPL359]ResponseIndex03    */
                              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

                            } /* -E- END OF if(..) ---------------------------*/

                            /*================================================*/
                            /* -E- AddressCheck was correct - area is not protected */
                            /*================================================*/
                            else
                            {
                              /*----------------------------------------------*/
                              /* Check the HIS-Driver Interface-Header        */
                              /*----------------------------------------------*/
                              t_Result_UB = FUN_CChal_HISDriverCheck_UB((tHis_Header*) \
                                  &t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].ubInterfaceVersion);

                              /*==============================================*/
                              /* -F- Result evaluation                        */
                              /*==============================================*/
                              switch ( t_Result_UB )
                              {
                                /*--------------------------------------------*/
                                /* No HIS-Driver available                    */
                                /*--------------------------------------------*/
                              case (d_CCglo_Error | 0x01):

                                /*--------------------------------------------*/
                                /* set return value - response index 5        */
                                /*--------------------------------------------*/
                                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);
                                break;
                                /*- - - - - - - - - - - - - - - - - - - - - - */

                                /*--------------------------------------------*/
                                /* Wrong HIS-Driver available                 */
                                /*--------------------------------------------*/
                              case (d_CCglo_Error | 0x02):

                                /*--------------------------------------------*/
                                /* set return value - response index 5        */
                                /*--------------------------------------------*/
                                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);
                                break;
                                /*- - - - - - - - - - - - - - - - - - - - - - */

                                /*--------------------------------------------*/
                                /* Everything is OK                           */
                                /*--------------------------------------------*/
                              default:

                                /*--------------------------------------------*/
                                /* Set the RoutineCallCounter                 */
                                /*--------------------------------------------*/
                                t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = 0x01u;

                                /*--------------------------------------------*/
                                /* set return value - response index 2        */
                                /*--------------------------------------------*/
                                t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);

                              } /* -F- END OF switch(..) ---------------------*/
                            } /* -E- END OF else (..) ------------------------*/
                          } /* ---D- END OF if(..) ---------------------------*/
                          /*==================================================*/
                          /*                                                  */
                          /* -D- erasing of eeprom                            */
                          /*                                                  */
                          /*==================================================*/
                          else
                          {
                            /*------------------------------------------------*/
                            /* Set the RoutineCallCounter                     */
                            /*------------------------------------------------*/
                            t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = 0x01u;

                            /*------------------------------------------------*/
                            /* set return value - response index 2            */
                            /*------------------------------------------------*/
                            t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);

                          } /* ---D- END OF else(..) -------------------------*/
                        } /* -----C- END OF else(..) -------------------------*/
                      } /* -------B- END OF else(..) -------------------------*/
                    } /* ---------A- END OF else(..) -------------------------*/
                  } /* -----------9- END OF else(..) -------------------------*/
                } /* -------------8- END OF else(..) -------------------------*/
              } /* ---------------7- END OF else(..) -------------------------*/
            } /* -----------------6- END OF else(..) -------------------------*/
          } /* -------------------5- END OF else(..) -------------------------*/
          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


          /*------------------------------------------------------------------*/
          /*                                                                  */
          /*  second call:                                                    */
          /*                                                                  */
          /*  For Flash: do the initialisation of the HIS container and the   */
          /*             nvm access  --> RoutineCallCounter = 0x02u           */
          /*  For EEPROM: delete the EEPROM --> ready                         */
          /*                                                                  */
          /*                                                                  */
          /*------------------------------------------------------------------*/
        case 0x01u:

          /*------------------------------------------------------------------*/
          /* Set the RoutineCallCounter                                       */
          /*------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = 0x00u;

          /*------------------------------------------------------------------*/
          /* restore the actual used download map index                       */
          /*------------------------------------------------------------------*/
           t_Index_UB = t_CCkwpaudi_KwpCtrl_ST.DownloadMapIndex_UB;

          /*------------------------------------------------------------------*/
          /* call the function that updates the EEPROM values                 */
          /*------------------------------------------------------------------*/
          t_Result_UB = FUN_CCkwpaudi_UpdateEraseBlockVal_UB(t_Index_UB);

          /*==================================================================*/
          /* -5- nvm access was not successful                                */
          /*==================================================================*/
          if ( d_CCglo_Idle != t_Result_UB )
          {
            /*----------------------------------------------------------------*/
            /* t_Result_UB was set in the called function                     */
            /*----------------------------------------------------------------*/

          } /* -------------5- END OF if(..) ---------------------------------*/
          /*==================================================================*/
          /* -5- nvm access was successful                                    */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /*                                                                */
            /* -6- erasing of flash memory                                    */
            /*                                                                */
            /*================================================================*/
            if ( d_CCglo_FlashMemory == (d_CCglo_MemoryTypeMask &  \
               t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW))
            {
              FUN_CCwtdog_InitExtWatchdogIntern_V();
              /*--------------------------------------------------------------*/
              /* Init the HIS Driver                                          */
              /*--------------------------------------------------------------*/
              pHisHeader  = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
              _FlashInit  = pHisHeader->pfnFlashInit;

              _FlashInit  (&FlashParam);

              /*--------------------------------------------------------------*/
              /* Set the RoutineCallCounter                                   */
              /*--------------------------------------------------------------*/
              t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = 0x02u;

              /*--------------------------------------------------------------*/
              /* set return value - function has to be called                 */
              /* a further time                                               */
              /*--------------------------------------------------------------*/
              t_Result_UB = ( d_CCglo_Busy );

            } /* -----------6- END OF else(..) -------------------------------*/
            else
            {
              /*================================================================*/
              /*                                                                */
              /* -7- erasing of eeprom                                          */
              /*                                                                */
              /*================================================================*/
              if ( d_CCglo_EepromMemory == (d_CCglo_MemoryTypeMask &  \
                            t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW))
              {
                /*--------------------------------------------------------------*/
                /* Erase the EEPROM                                             */
                /*--------------------------------------------------------------*/
                t_Result_UB = FUN_CCkwpaudi_EraseEeprom_UB();

                /*==============================================================*/
                /* -8- Erasing was not successful                               */
                /*==============================================================*/
                if ( t_Result_UB != d_CCglo_NoError )
                {
                  /*------------------------------------------------------------*/
                  /* set return value - response index 7 - programming          */
                  /* failure                                                    */
                  /*------------------------------------------------------------*/
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex07);

                } /* ---------7- END OF if(..) ---------------------------------*/
                /*==============================================================*/
                /* -8- Erasing was successful                                   */
                /*==============================================================*/
                else
                {
                  /*------------------------------------------------------------*/
                  /* allways send a positive response not depending on          */
                  /* suppressPosRespIndBit, because of previous                 */
                  /* RespPending                                                */
                  /*------------------------------------------------------------*/
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                } /* -------8- END OF else(..) -------------------------------*/
              } /* ---------7- END OF if(..)   -------------------------------*/
              else
              {
                /*============================================================*/
                /*                                                            */
                /* -7- erasing of parrot                                      */
                /*                                                            */
                /*============================================================*/
                /*------------------------------------------------------------*/
                /* allways send a positive response not depending on          */
                /* suppressPosRespIndBit, because of previous                 */
                /* RespPending                                                */
                /*------------------------------------------------------------*/
                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
              } /* ---------7- END OF else(..) -------------------------------*/
            } /* -----------6- END OF else(..) -------------------------------*/
          } /* -------------5- END OF else(..) -------------------------------*/
          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          /*------------------------------------------------------------------*/
          /*                                                                  */
          /*  third call:                                                     */
          /*                                                                  */
          /*  For Flash: erase the actual sector                              */
          /*                --> busy if more sectors to be deleted            */
          /*                                                                  */
          /*                --> ready if all sectors are deleted -->          */
          /*                             RoutineCallCounter = 0x00u           */
          /*                                                                  */
          /*------------------------------------------------------------------*/
        default:

          /*------------------------------------------------------------------*/
          /* Set the RoutineCallCounter                                       */
          /* no more calls of this function; can be overwritten if            */
          /* necessary.                                                       */
          /*------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = 0x00u;


          /* trigger timeout timer */
          FUN_CCeventaudi_SetProjectTimer_V (c_CCconprj_EraseResetTime_T16);

          /*------------------------------------------------------------------*/
          /* Init the HIS Driver for Erase flash                              */
          /*------------------------------------------------------------------*/
          pHisHeader  = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
          _FlashErase = pHisHeader->pfnFlashErase;
          /*------------------------------------------------------------------*/
          /* Init the Parameter structure                                     */
          /*------------------------------------------------------------------*/
          FlashParam.address = t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL;
          FlashParam.length  = t_CCkwpaudi_KwpCtrl_ST.EraseLength_UL;

          FlashParam.wdTriggerFct = (tWDTriggerFct)FUN_CCwtdog_TriggerWatchdogEraseMem_V;

          /*------------------------------------------------------------------*/
          /* Erase the current sector                                         */
          /*------------------------------------------------------------------*/
          _FlashErase(&FlashParam);

          /*==================================================================*/
          /*                                                                  */
          /* -5- Result evaluation of "erase memory"                          */
          /*                                                                  */
          /*==================================================================*/
          switch ( FlashParam.errorcode )
          {
            /*----------------------------------------------------------------*/
            /* READY - Routine has erased all required sectors                */
            /*----------------------------------------------------------------*/
          case kFlashOK:

            /*----------------------------------------------------------------*/
            /* Deinit the HIS Flash Driver                                    */
            /*----------------------------------------------------------------*/
            pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
            _FlashDeinit = pHisHeader->pfnFlashDeinit;

            _FlashDeinit (&FlashParam);

            t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagErasingDoneOk;
            /*----------------------------------------------------------------*/
            /* Send a positive response (independent of the                   */
            /* suppressPosRspMsgIndicationBit because of previous Rsp pending)*/
            /*----------------------------------------------------------------*/

            t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex00);

            break;
            /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

            /*----------------------------------------------------------------*/
            /* BUSY - Routine was interrupted. Not all required sectors       */
            /* erased.                                                        */
            /*----------------------------------------------------------------*/
          case kFlashEraseBusy:
            /*----------------------------------------------------------------*/
            /* NOTE: "ResponsePending" is sent out of the RAM. So only        */
            /*       prepare function to be called again.                     */
            /*----------------------------------------------------------------*/

            /*----------------------------------------------------------------*/
            /* Set the RoutineCallCounter                                     */
            /* Function has to called a fruther time                          */
            /*----------------------------------------------------------------*/
            t_CCkwpaudi_KwpCtrl_ST.RoutineCallCounter_UB = 0x02u;

            /*----------------------------------------------------------------*/
            /* Set return value - function is still busy                      */
            /*----------------------------------------------------------------*/
            t_Result_UB = (d_CCglo_Busy);
            break;
            /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

            /*----------------------------------------------------------------*/
            /* ERROR - erase failed                                           */
            /*----------------------------------------------------------------*/
          case kFlashEraseFailed:

            /*----------------------------------------------------------------*/
            /* Set return value - positive response with incorrect result     */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex06);

            break;
            /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

            /*----------------------------------------------------------------*/
            /* ERROR - all other errors which are possible and checked by     */
            /* HIS-API                                                        */
            /*----------------------------------------------------------------*/
          default:

            /*----------------------------------------------------------------*/
            /* Deinit the HIS Flash Driver                                    */
            /*----------------------------------------------------------------*/
            pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;
            _FlashDeinit = pHisHeader->pfnFlashDeinit;

            _FlashDeinit (&FlashParam);

            /*----------------------------------------------------------------*/
            /* Set return value - Index 03                                    */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

            break;
            /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
          } /*--------------5- END OF switch(..) -----------------------------*/

          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        } /* ---------------4- END OF switch(..) -----------------------------*/


        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        /*--------------------------------------------------------------------*/
        /* all other routine control types are not supported                  */
        /*--------------------------------------------------------------------*/
      default:

        /*--------------------------------------------------------------------*/
        /* check the addressing mode for functional addressing                */
        /*--------------------------------------------------------------------*/
        t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                     (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

        /*====================================================================*/
        /* -4- service is functionally addressed                              */
        /*====================================================================*/
        if ( d_CCglo_Ready == t_Result_UB )
        {
          /*------------------------------------------------------------------*/
          /* set return value - will be overwritten by an error or other      */
          /* state                                                            */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex09);

          /*------------------------------------------------------------------*/
          /* Set the DLC of the OutBuffer to zero                             */
          /*------------------------------------------------------------------*/
          t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

        } /* ---------------4- END OF if(..) ---------------------------------*/
        /*====================================================================*/
        /* -4- service is not functionally addressed                          */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 1                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

        } /* ---------------4- END OF else (if(..)) --------------------------*/

        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      } /* -----------------3- END OF switch (..) ----------------------------*/
    } /* -------------------2- END OF else () --------------------------------*/
  } /* ---------------------1- END OF else () --------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_RcEraseMemory_UB' +-+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $31 - RoutineControl                                         */
/*  LOCAL ID   : $xx                                                          */
/*  Routine ID : $FF -+                                                       */
/*  Routine ID : $00 -+- Erase memory                                         */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRcEraseMemory_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_RcEraseMemoryDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_05Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpaudi_RIDeraseMemoryByte0,
  /* 05 - RESPONSE BYTE 03 */  d_CCkwpaudi_RIDeraseMemoryByte1,
  /* 06 - RESPONSE BYTE 04 */  d_CCkwpaudi_correctResult,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - SubFunction not supported                                     */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSubFunctionNotSupported,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 12 - POST CONDIDTION  */  d_CCglo_NoPostCondDefined,
  /* 13 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 14 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 15 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 16 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - parameter not correct                                         */
  /*--------------------------------------------------------------------------*/
  /* 17 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 18 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 19 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 20 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 21 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 -  Length Error                                                 */
  /*--------------------------------------------------------------------------*/
  /* 22 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 23 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 24 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 25 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 26 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat,
  /*--------------------------------------------------------------------------*/
  /* INDEX 05 -  conditions not correct                                       */
  /*--------------------------------------------------------------------------*/
  /* 27 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 28 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 29 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 30 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 31 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect,
  /*--------------------------------------------------------------------------*/
  /* INDEX 06 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 32 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 33 - RESPONSE LENGTH  */  d_CCglo_05Byte,
  /* 34 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 35 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 36 - RESPONSE BYTE 02 */  d_CCkwpaudi_RIDeraseMemoryByte0,
  /* 37 - RESPONSE BYTE 03 */  d_CCkwpaudi_RIDeraseMemoryByte1,
  /* 38 - RESPONSE BYTE 04 */  d_CCkwpaudi_incorrectResult,
  /*--------------------------------------------------------------------------*/
  /* INDEX 07 -  eeprom programming failed                                    */
  /*--------------------------------------------------------------------------*/
  /* 39 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 40 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 41 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 42 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 43 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCGeneralProgFailure,
  /*--------------------------------------------------------------------------*/
  /* INDEX 08 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 44 - POST CONDIDTION  */  d_CCseq_RcEraseMemoryDone,
  /* 45 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 09 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 46 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 47 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 0A - Security access denied                                        */
  /*--------------------------------------------------------------------------*/
  /* 45 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 46 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 47 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 48 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 49 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSecurityAccessDenied,
  /*--------------------------------------------------------------------------*/
  /* INDEX 0B - conditions not correct                                        */
  /*--------------------------------------------------------------------------*/
  /* 50 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 51 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 52 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 53 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 54 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect

}; /*--- END OF TABLE --------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_UpdateEraseBlockVal_UB                      |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000Audi.c                                     |
|                                                                            |
|  Description:    This function updates the EEPROM values in the            |
|                  EraseMemory function.                                     |
|                                                                            |
|  Parameter:      t_Index_UB  Index of the actual block                     |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_UpdateEraseBlockVal_UB ( UBYTE t_Index_UB )
{
  UBYTE  t_Result_UB;
  UBYTE  t_EepromHelp_UB;
  UBYTE  t_CounterHelp_AUB[2];
  UNION2 t_MaximumProgramAttempts_U2;
  UNION2 t_CountProgramAttempts_U2;

  /*--------------------------------------------------------------------------*/
  /* get the name (= index of the array                                       */
  /* c_CCconprj_LogBlockEEPROM_AUB) of the block                              */
  /*--------------------------------------------------------------------------*/
  t_Index_UB = t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].Name_UB;  /* [OPL: 302; 1 von 3] 3. in CheckDepend! */

  /*==========================================================================*/
  /* -0- index in downloadmap is valid index for c_CCconprj_LogBlockEEPROM_AUB*/
  /*==========================================================================*/
  if (d_CCglo_NoEntry == t_Index_UB)
  {
    t_Result_UB = ( d_CCglo_Idle );
  }
  /*==========================================================================*/
  /* -0- index in downloadmap gives valid index                               */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* read out the maximum number of programming attempts                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = FUN_CCnvm_NvmAccess_UB ( \
      c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWMaxProgrAttemptsIndex],  \
      &t_CounterHelp_AUB[0]);

    /*------------------------------------------------------------------------*/
    /* use union for right endianess                                          */
    /*------------------------------------------------------------------------*/
    t_MaximumProgramAttempts_U2.UByte_ST.UByte0_UB = t_CounterHelp_AUB[0];
    t_MaximumProgramAttempts_U2.UByte_ST.UByte1_UB = t_CounterHelp_AUB[1];

    /*========================================================================*/
    /* -1- the nvm access was not successful                                  */
    /*========================================================================*/
    if ( d_CCglo_NoError != t_Result_UB )
    {
      t_MaximumProgramAttempts_U2.UWord_ST.UWord0_UW = 0xFFFFu;

    } /* -----1- END OF if(..) -----------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* read out the actual number of programming attempts                     */
    /*------------------------------------------------------------------------*/
    t_Result_UB = FUN_CCnvm_NvmAccess_UB ( \
      c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWCntProgrAttemptsIndex],  \
      &t_CounterHelp_AUB[0]);

    /*------------------------------------------------------------------------*/
    /* use union for right endianess                                          */
    /*------------------------------------------------------------------------*/
    t_CountProgramAttempts_U2.UByte_ST.UByte0_UB = t_CounterHelp_AUB[0];
    t_CountProgramAttempts_U2.UByte_ST.UByte1_UB = t_CounterHelp_AUB[1];

    /*========================================================================*/
    /* -1- if the maximum is 0x0000 we want to have the same                  */
    /*      behaviour as with 0xFFFF                                          */
    /*========================================================================*/
    if (0x0000u == t_MaximumProgramAttempts_U2.UWord_ST.UWord0_UW)
    {
      t_MaximumProgramAttempts_U2.UWord_ST.UWord0_UW = 0xFFFFu;

    } /* -----1- END OF if(..) -----------------------------------------------*/

    /*========================================================================*/
    /* -1-  the maximum number of programming attempts has                    */
    /*      been reached                                                      */
    /*========================================================================*/
    if (t_CountProgramAttempts_U2.UWord_ST.UWord0_UW >=
           t_MaximumProgramAttempts_U2.UWord_ST.UWord0_UW)
    {

      /*----------------------------------------------------------------------*/
      /* set return value - response index 5                                  */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

    } /* -----9- END OF else(..) ---------------------------------------------*/
    /*========================================================================*/
    /* -1-  a further programming attempt is allowed                          */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -2- 0xffff programming cycles                                        */
      /*======================================================================*/
      if  (t_CountProgramAttempts_U2.UWord_ST.UWord0_UW == 0xFFFFu)
      {
        /*--------------------------------------------------------------------*/
        /* set return value - response index 5                                */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

      } /* ---2- END OF if(..) -----------------------------------------------*/
      /*======================================================================*/
      /* -2-                                                                  */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* set the application software invalid                               */
        /*--------------------------------------------------------------------*/
        t_Result_UB = FUN_CCnvm_NvmAccess_UB (d_CCnvm_READ__ECUProgramInfo,  \
          &t_EepromHelp_UB);

        /* t_EepromHelp_UB &= ~d_CCkwpaudi_AppInconsistentMask; */
        t_EepromHelp_UB |= d_CCkwpaudi_AppInconsistent;

        t_Result_UB |= FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ECUProgramInfo,  \
         &t_EepromHelp_UB);

        /* get internal inconsistence bit and declare system as not valid */
        t_EepromHelp_UB = ~d_CCkwpaudi_AppProgrammed;

        t_Result_UB |= FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_SysProgrammed, &t_EepromHelp_UB);

        /*--------------------------------------------------------------------*/
        /* set the software block invalid                                     */
        /*--------------------------------------------------------------------*/
        t_EepromHelp_UB =  d_CCkwpaudi_ProgStateSWInValid;

        t_Result_UB |= FUN_CCnvm_NvmAccess_UB ((UBYTE)( d_CCglo_Write |  \
         c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWProgStateIndex] ),  \
         &t_EepromHelp_UB);

        /*--------------------------------------------------------------------*/
        /* increment the programming counter                                  */
        /*--------------------------------------------------------------------*/
        t_CountProgramAttempts_U2.UWord_ST.UWord0_UW++;

        /*--------------------------------------------------------------------*/
        /* use union for right endianess                                      */
        /*--------------------------------------------------------------------*/
        t_CounterHelp_AUB[0] = t_CountProgramAttempts_U2.UByte_ST.UByte0_UB;
        t_CounterHelp_AUB[1] = t_CountProgramAttempts_U2.UByte_ST.UByte1_UB;

        t_Result_UB |= FUN_CCnvm_NvmAccess_UB ((UBYTE)( d_CCglo_Write |  \
         c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWCntProgrAttemptsIndex]),  \
         &t_CounterHelp_AUB[0]);

        /*--------------------------------------------------------------------*/
        /* write RepairShopCodeSerNo into eeprom                              */
        /*--------------------------------------------------------------------*/
        t_Result_UB |= FUN_CCnvm_NvmAccess_UB ((UBYTE)( d_CCglo_Write |  \
         c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWRepairShopCodeSerNoIndex] ),  \
         &sECUDataAppl_ST.FPC_Kennung_AUB[0]);

        /*--------------------------------------------------------------------*/
        /* write ProgrammingDate into eeprom                                  */
        /*--------------------------------------------------------------------*/
        t_Result_UB |= FUN_CCnvm_NvmAccess_UB ((UBYTE)( d_CCglo_Write |  \
         c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWProgDateIndex] ),  \
         &sECUDataAppl_ST.ProgDate_AUB[0]);

        /*====================================================================*/
        /* -3- nvm access was not successful                                  */
        /*====================================================================*/
        if ( d_CCglo_NoError != t_Result_UB )
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 7                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex07);

        } /* -3- END OF if(..) -----------------------------------------------*/

        /*====================================================================*/
        /* -3- nvm access was successful                                      */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* set return value - function has not detected                     */
          /* any error                                                        */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Idle );

        } /* -3- END OF else(..) ---------------------------------------------*/
      } /* ---2- END OF else(..) ---------------------------------------------*/
    } /* -----1- END OF else(..) ---------------------------------------------*/
  } /* -------0- END OF else(..) ---------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return (t_Result_UB );

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_UpdateEraseBlockVal_UB' +-+-+-+-+-+-+-+*/

/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_RcCheckMemory_UB                            |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $31 - Start Routine By Local ID               |
|                  Local ID  : $xx - Routine control type                    |
|                  Routine ID: $02 -+                                        |
|                  Routine ID: $02 -+- Check memory                          |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RcCheckMemory_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                       PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE  t_Result_UB;
  UNION2 t_MemoryIndex_U2;
  UBYTE  t_NbrIndexBytes_UB;
  UBYTE  t_ValidationIndex_UB;
  UBYTE  t_CheckState_UB;
  UNION2 t_ChecksumNbrBytes_U2;
  UNION4 t_Checksum_U4;

  UBYTE  t_ReturnValue_UB;
  UBYTE  t_ResultCheckSignation_UB;
  UWORD  t_ChecksumNbrBytes_UW;
  UBYTE  t_ChecksumOffset_UB = (UBYTE) 0x00;

  #ifndef d_CCkwpaudi_DoCrcAfterWrite
    ULONG t_Address_UL;
    ULONG t_Length_UL;
    UBYTE DataBuffer_UB[d_CCkwpaudi_SignaturePackedLength];
    /* Runtime Structure - Interface                                          */
    NVMRUNTIMEENTRY_ST  t_CCnvm_NvmRunTimeEntry_ST;
  #endif

  /* The next lines are only to prevent a compiler warning!!                  */
  t_OutBuffer_PBUF[0] |= 0x00;

  t_Result_UB = 0;

#ifdef cs_Signatur_Aktiv
  #ifdef cs_Decryption_Aktiv
        /*--------------------------------------------------------------------*/
        /* encrypted data, checksum length = Signature + CRC                  */
        /*--------------------------------------------------------------------*/
        t_ChecksumNbrBytes_UW = c_CCconprj_SignatureLength_UW + c_CCconprj_ChecksumLength_UW;
  #else
        /*--------------------------------------------------------------------*/
        /* no encrypted data, checksum length = Signature                     */
        /*--------------------------------------------------------------------*/
        t_ChecksumNbrBytes_UW = c_CCconprj_SignatureLength_UW;
  #endif
#else
        /*--------------------------------------------------------------------*/
        /* no signature, use only crc32, checksum length = CRC                */
        /*--------------------------------------------------------------------*/
        t_ChecksumNbrBytes_UW = c_CCconprj_ChecksumLength_UW;
#endif

  /*==========================================================================*/
  /* -1- check the routine control type                                       */
  /*==========================================================================*/
  switch (d_CCglo_LIDSubFunctionMask & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01])
  {
    /*------------------------------------------------------------------------*/
    /* start routine                                                          */
    /*------------------------------------------------------------------------*/
  case d_CCkwpbase_LIDStartRoutine:

    /*========================================================================*/
    /* -2- Function was called a first time                                   */
    /*========================================================================*/
    if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
                                                      d_CCkwpaudi_FlagRoutineBusy )
    {
      /*----------------------------------------------------------------------*/
      /* evaluate the LengthInformationIdentifier                             */
      /*----------------------------------------------------------------------*/
      t_NbrIndexBytes_UB = (0x0F & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]);

      /*======================================================================*/
      /* -3- The number of bytes for the index of the sw block is incorrect   */
      /*     or the number of memory size bytes is not zero                   */
      /*======================================================================*/
      if ( (0x00 == t_NbrIndexBytes_UB) || ( 0x02 < t_NbrIndexBytes_UB ) ||
           ( 0x00 != ( 0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04] ) ) )
      {
        /*====================================================================*/
        /* -4- Service length is not correct                                  */
        /*====================================================================*/
        /* imx [OPL355]:                                                      */
        /* d_CCglo_07Byte = {31,01,ff,00,t_NbrIndexBytes_UB,Checksumlength}   */
        if ( ((d_CCglo_07Byte + t_NbrIndexBytes_UB + t_ChecksumNbrBytes_UW) != \
              t_InBuffer_PBUF[d_CCglo_DLC]) && \
             (((0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]) >> 4) == 0))
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 4                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);
        }
        else
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 3                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

        } /* -4- END OF else(..) ---------------------------------------------*/

      } /* -------------3- END OF if(..) -------------------------------------*/
      /*======================================================================*/
      /* -3- The number of bytes for the index of the sw block is correct     */
      /*======================================================================*/
      else
      {
        /*====================================================================*/
        /* -4- Service length is not correct                                  */
        /*====================================================================*/
        /* imx [OPL355]:                                                      */
        /* d_CCglo_07Byte = {31,01,ff,00,t_NbrIndexBytes_UB,Checksumlength}   */
        if ( ((d_CCglo_07Byte + t_NbrIndexBytes_UB + t_ChecksumNbrBytes_UW) != \
                  t_InBuffer_PBUF[d_CCglo_DLC]) && \
             (((0xF0 & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]) >> 4) == 0))
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 4                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);
        }
        else
        {
          /*==================================================================*/
          /* -5- We are not in the programming session                        */
          /*==================================================================*/
          if (t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB !=  (UBYTE)0x02u)
          {
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
          }
          /*==================================================================*/
          /* -5- We are in the programming session                            */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /* -6- We dont pass security access                               */
            /*================================================================*/
            if(d_CCkwpaudi_FlagSecurityAccessGranted != \
              (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSecurityAccessGranted))
            {
              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex10);
            }
            /*================================================================*/
            /* -6- We pass security access                                    */
            /*================================================================*/
            else
            {
              /*==============================================================*/
              /* -7- We dont pass fingerprint                                 */
              /*==============================================================*/
              if (d_CCkwpaudi_FlagFPDoneOk != \
               (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagFPDoneOk))
              {
                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex11);
              }
              /*==============================================================*/
              /* -7- We pass fingerprint                                      */
              /*==============================================================*/
              else
              {
                /*============================================================*/
                /* -8- We dont pass RequestTransferExit                       */
                /*============================================================*/
                if (d_CCkwpaudi_FlagReadyforCheckMemory != \
                   (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagReadyforCheckMemory))
                {
                  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex12);
                }
                /*============================================================*/
                /* -8- We pass RequestTransferExit                            */
                /*============================================================*/
                else
                {

                  /*==========================================================*/
                  /* -9- Memory index consists of 1 byte                      */
                  /*==========================================================*/
                  if ( 1 == t_NbrIndexBytes_UB )
                  {
                    /*--------------------------------------------------------*/
                    /* Get the memory index                                   */
                    /*--------------------------------------------------------*/
                    t_MemoryIndex_U2.UByte_ST.UByte0_UB = 0;
                    t_MemoryIndex_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x05];

                  } /* -----------5- END OF if(..) ---------------------------*/
                  /*==========================================================*/
                  /* -9- Memory index consists of 2 bytes                     */
                  /*==========================================================*/
                  else
                  {
                    /*--------------------------------------------------------*/
                    /* Get the memory index                                   */
                    /*--------------------------------------------------------*/
                    t_MemoryIndex_U2.UByte_ST.UByte0_UB = \
                                t_InBuffer_PBUF[d_CCglo_StartOfData + 0x05];
                    t_MemoryIndex_U2.UByte_ST.UByte1_UB = \
                                t_InBuffer_PBUF[d_CCglo_StartOfData + 0x06];

                  } /* -----------5- END OF else (if(..)) --------------------*/

                  /*----------------------------------------------------------*/
                  /* Get the number of byte of the checksum                   */
                  /*----------------------------------------------------------*/
                  t_ChecksumNbrBytes_U2.UByte_ST.UByte0_UB = t_InBuffer_PBUF  \
                                  [d_CCglo_StartOfData + 0x05 + t_NbrIndexBytes_UB];
                  t_ChecksumNbrBytes_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF  \
        	                  [d_CCglo_StartOfData + 0x06 + t_NbrIndexBytes_UB];

                  /*==========================================================*/
                  /* -9- the number of checksum bytes is not correct          */
                  /*==========================================================*/
                  if ( t_ChecksumNbrBytes_UW != t_ChecksumNbrBytes_U2.UWord_ST.UWord0_UW )
                  {
                    /*--------------------------------------------------------*/
                    /* set return value - response index 3                    */
                    /*--------------------------------------------------------*/
                    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

                  } /* -------------5- END OF if(..) -------------------------*/
                  /*==========================================================*/
                  /* -9- the number of checksum byte is correct               */
                  /*==========================================================*/
                  else
                  {
                    /*--------------------------------------------------------*/
                    /* Get the memory index                                   */
                    /*--------------------------------------------------------*/
                    t_ValidationIndex_UB = FUN_CCkwpaudi_ResolveValidationMapIndex_UB  \
                                               ( t_MemoryIndex_U2.UWord_ST.UWord0_UW);

                    /*========================================================*/
                    /* -A- Memory index NOT found in the validation map       */
                    /*========================================================*/
                    if ( d_CCglo_NoEntry == t_ValidationIndex_UB )
                    {
                      /*------------------------------------------------------*/
                      /* set return value - response index 3                  */
                      /*------------------------------------------------------*/
                      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
                    } /* -------6- END OF if(..) -----------------------------*/

                    /*========================================================*/
                    /* -A- Memory index found in the validation map           */
                    /*========================================================*/
                    else
                    {
                      /*------------------------------------------------------*/
                      /* store the validation map index                       */
                      /*------------------------------------------------------*/
                      t_CCkwpaudi_KwpCtrl_ST.ValidationMapIndex_UB = t_ValidationIndex_UB;

                      /*------------------------------------------------------*/
                      /* get start of Checksum                                */
                      /*------------------------------------------------------*/
                      t_ChecksumOffset_UB = \
                        d_CCglo_StartOfData + (UBYTE)d_CCglo_07Byte + t_NbrIndexBytes_UB;

#ifdef cs_Signatur_Aktiv
          #ifdef cs_Decryption_Aktiv
                      /*------------------------------------------------------*/
                      /* get start of Checksum when crc32 is concatenated     */
                      /* signature                                            */
                      /*------------------------------------------------------*/
                      t_ChecksumOffset_UB += (UBYTE) 0x80;
          #endif
#endif
                      /*------------------------------------------------------*/
                      /* Get CRC depending on configured checksum length      */
                      /*------------------------------------------------------*/
                      switch (c_CCconprj_ChecksumLength_UW)
                      {
                        /*----------------------------------------------------*/
                        /* 01 Byte CRC                                        */
                        /*----------------------------------------------------*/
                        case (UWORD)d_CCglo_01Byte:

                          t_Checksum_U4.UByte_ST.UByte3_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x00u];

                          break;
                        /*----------------------------------------------------*/
                        /* 02 Byte CRC                                        */
                        /*----------------------------------------------------*/
                        case (UWORD)d_CCglo_02Byte:

                          t_Checksum_U4.UByte_ST.UByte2_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x00u];
                          t_Checksum_U4.UByte_ST.UByte3_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x01u];

                          break;
                        /*----------------------------------------------------*/
                        /* 04 Byte CRC                                        */
                        /*----------------------------------------------------*/
                        case (UWORD)d_CCglo_04Byte:

                          t_Checksum_U4.UByte_ST.UByte0_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x00u];
                          t_Checksum_U4.UByte_ST.UByte1_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x01u];
                          t_Checksum_U4.UByte_ST.UByte2_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x02u];
                          t_Checksum_U4.UByte_ST.UByte3_UB = \
                                    t_InBuffer_PBUF[t_ChecksumOffset_UB + (UBYTE)0x03u];

                      break;
                        /*----------------------------------------------------*/
                        /* default - does not occur                           */
                        /*----------------------------------------------------*/
                        default:

                          break;
                      }

                      /*------------------------------------------------------*/
                      /* copy CRC into KwpCtrl                                */
                      /*------------------------------------------------------*/
                      t_CCkwpaudi_KwpCtrl_ST.Tester_Checksum_UL  = t_Checksum_U4.ULong_ST.ULong0_UL;

                      /*------------------------------------------------------*/
                      /* read out the start and end address                   */
                      /*------------------------------------------------------*/
                      t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL = t_CCkwpaudi_KwpCtrl_ST.  \
                                ValidationMap_PST[t_ValidationIndex_UB].StartAddress_UL;

                      t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL   = t_CCkwpaudi_KwpCtrl_ST.  \
                                 ValidationMap_PST[t_ValidationIndex_UB].StopAddress_UL;

                      /*======================================================*/
                      /* -B- verify addresses                                 */
                      /*======================================================*/
                      if((t_CCkwpaudi_KwpCtrl_ST.DLStartAddress_UL != \
                         t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL) && \
                        (t_CCkwpaudi_KwpCtrl_ST.DLEndAddress_UL != \
                         t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL))
                      {
                        /*----------------------------------------------------*/
                        /* set return value - response index 3                */
                        /*----------------------------------------------------*/
                        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
                      }
                      else
                      {
                        /*====================================================*/
                        /* -C- table entry is a parrot entry                  */
                        /*====================================================*/
                        if ( d_CCglo_Parrot == \
                             (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.\
                             ValidationMap_PST[t_ValidationIndex_UB].CheckCondition_UW ))
                        {
                          /*====================================================*/
                          /* -D- get result from kwp2000 control structure and  */
                          /* compare received CRC with on-the-fly calculated CRC*/
                          /*====================================================*/
                          if (((d_CCkwpaudi_FlagInternFlashDownload|d_CCkwpaudi_FlagChecksumCorrect)==\
                              (t_CCkwpaudi_KwpCtrl_ST.State_UW & \
                              (d_CCkwpaudi_FlagInternFlashDownload|d_CCkwpaudi_FlagChecksumCorrect)))\
                            &&(g_CCkwpaudi_ParrotCRC32_UL == t_CCkwpaudi_KwpCtrl_ST.Tester_Checksum_UL))
                          {
                            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);
                            /*------------------------------------------------*/
                            /* Update the EEPROM: set the block valid         */
                            /*------------------------------------------------*/
                            t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                                               t_ValidationIndex_UB,  \
                                                               d_CCkwpaudi_ProgStateSWValid );

                            /*================================================*/
                            /* -5- an error has accoured                      */
                            /*================================================*/
                            if (d_CCglo_Idle != t_ReturnValue_UB)
                            {
                              t_Result_UB = t_ReturnValue_UB;

                            } /* ---------5- END OF if(..) -------------------*/

                            /* Set/reset flags                                */
                            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagReadyforCheckMemory;
                            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
                            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;
                          }
                          else
                          {
                            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);
                            /*------------------------------------------------*/
                            /* Update the EEPROM: set the block invalid       */
                            /*------------------------------------------------*/
                            t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                                               t_ValidationIndex_UB,  \
                                                               d_CCkwpaudi_ProgStateSWInValid );
                            /*================================================*/
                            /* -E- an error has accoured                      */
                            /*================================================*/
                            if (d_CCglo_Idle != t_ReturnValue_UB)
                            {
                              t_Result_UB = t_ReturnValue_UB;

                            } /* ---------E- END OF if(..) -------------------*/

                            /* reset flags                                    */
                            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagReadyforCheckMemory;
                            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
                            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

                            /*------------------------------------------------*/
                            /* clear the flash functions                      */
                            /*------------------------------------------------*/
                            FUN_CCkwpaudi_ClearFlashFunctions_V();

                          }
                        }
                        /*====================================================*/
                        /* -C- table entry is a parrot entry                  */
                        /*====================================================*/
                        else
                        {
                          /*----------------------------------------------------*/
                          /* Reset checksum calculation state                   */
                          /*----------------------------------------------------*/
                          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) \
                                     ~(d_CCkwpaudi_FlagChecksumCorrect);

                          /*====================================================*/
                          /* -D- table entry is a flash entry                   */
                          /*====================================================*/
                          if ( d_CCglo_FlashMemory == \
                             (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.  \
                                                ValidationMap_PST[t_ValidationIndex_UB].CheckCondition_UW ))
                          {

  #ifdef d_CCkwpaudi_DoCrcAfterWrite
                            /*--------------------------------------------------*/
                            /* Init checksum calculation                        */
                            /*--------------------------------------------------*/
                            t_CheckState_UB = FUN_CCvalaudi_InitCalcCsumFlashDownload_UB ( \
                                                t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL, \
                                                t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL, \
                                                (ULONG) &t_CCkwpaudi_KwpCtrl_ST.Tester_Checksum_UL );
  #endif

                            /*--------------------------------------------------*/
                            /* NOTE: Now is the rigth time to send our first    */
                            /*       "ResponsePending". After sending we will   */
                            /*       have P3max instead of P2max for timeout.   */
                            /*--------------------------------------------------*/
                            /*--------------------------------------------------*/
                            /* Set the internal busy flag                       */
                            /*--------------------------------------------------*/
                            t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

                            /*--------------------------------------------------*/
                            /* set return value - response index 2              */
                            /*--------------------------------------------------*/
                            t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);

                          }
                          /*====================================================*/
                          /* -D- table entry is not a flash entry               */
                          /*====================================================*/
                          else
                          {
                            /*==================================================*/
                            /* -E- table entry is a ram memory entry            */
                            /*==================================================*/
                            if ( d_CCglo_RamMemory == \
                               (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.  \
                                                  ValidationMap_PST[t_ValidationIndex_UB].CheckCondition_UW ))
                            {
                              /*------------------------------------------------*/
                              /* Set the internal busy flag                     */
                              /*------------------------------------------------*/
                              t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

                              /*------------------------------------------------*/
                              /* set return value - response index 2            */
                              /*------------------------------------------------*/
                              t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);
                            } /* -E- End OF if (..) --------------------------*/
                            else
                            {
                              /*================================================*/
                              /* -F- table entry is an eeprom memory entry      */
                              /*================================================*/
                              if ( d_CCglo_EepromMemory == \
                                 (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.  \
                                  ValidationMap_PST[t_ValidationIndex_UB].CheckCondition_UW ))
                              {

  #ifdef d_CCkwpaudi_DoCrcAfterWrite
                                /*----------------------------------------------*/
                                /* Init checksum calculation                    */
                                /*----------------------------------------------*/
                                t_CheckState_UB = FUN_CCvalaudi_InitCalcCsumEepromDownload_UB ( \
                                                 t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL, \
                                                 t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL, \
                                                 (ULONG) &t_CCkwpaudi_KwpCtrl_ST.Tester_Checksum_UL );
  #endif

                                /*----------------------------------------------*/
                                /* NOTE: Now is the rigth time to send our      */
                                /*       first "ResponsePending". After         */
                                /*       sending we will have P3max instead     */
                                /*       of P2max for timeout.                  */
                                /*----------------------------------------------*/
                                /*----------------------------------------------*/
                                /* Set the internal busy flag                   */
                                /*----------------------------------------------*/
                                t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

                                /*----------------------------------------------*/
                                /* set return value - response index 2          */
                                /*----------------------------------------------*/
                                t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);

                              } /* -F- END OF if(..) ---------------------------*/
                              /*================================================*/
                              /* -F- table entry is not an eeprom memory entry  */
                              /*================================================*/
                              else
                              {
                                /*----------------------------------------------*/
                                /* set return value - response index 3          */
                                /*----------------------------------------------*/
                                t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);

                              } /* -F- END OF else (if(..)) ------------------*/
                            } /* -E- END OF else (if(..)) --------------------*/
                          } /* ---D- END OF else (if(..)) --------------------*/
                        } /* -----C- END OF else (if(..)) --------------------*/
                      } /* -------B- END OF else (if(..)) --------------------*/
                    } /* ---------A- END OF else (if(..)) --------------------*/
                  } /* -----------9- END OF else (if(..)) --------------------*/
                } /* -------------8- END OF else (if(..)) --------------------*/
              } /* ---------------7- END OF else (if(..)) --------------------*/
            } /* -----------------6- END OF else (if(..)) --------------------*/
          } /* -------------------5- END OF else (if(..)) --------------------*/
        } /* ---------------------4- END OF else (if(..)) --------------------*/
      } /* -----------------------3- END OF else (if(..)) --------------------*/
    } /* -------------------------2- END OF if(..) ---------------------------*/

    /*========================================================================*/
    /* -2- Function was called one more time                                  */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* restore the validation map index                                     */
      /*----------------------------------------------------------------------*/
      t_ValidationIndex_UB = t_CCkwpaudi_KwpCtrl_ST.ValidationMapIndex_UB;

      /*======================================================================*/
      /* -3- table entry is a flash entry                                     */
      /*======================================================================*/
      if ( d_CCglo_FlashMemory == \
           (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.  \
                              ValidationMap_PST[t_ValidationIndex_UB].CheckCondition_UW ))
      {

#ifdef d_CCkwpaudi_DoCrcAfterWrite
        /*-------------------------------------------------------------------*/
        /* Call the checksum calculation routine for flash download           */
        /*--------------------------------------------------------------------*/
        t_CheckState_UB = FUN_CCvalaudi_CalcCsumFlashDownload_UB ();
#else
        /*--------------------------------------------------------------------*/
        /* initialise return value                                            */
        /*--------------------------------------------------------------------*/
        t_CheckState_UB = d_CCglo_Ready;
#endif

        /*====================================================================*/
        /* -4- CheckState evaluation                                          */
        /*====================================================================*/
        switch (t_CheckState_UB)
        {
          /*------------------------------------------------------------------*/
          /* Checksum calculation finished and Checksum is correct            */
          /*------------------------------------------------------------------*/
        case d_CCglo_Ready:

          /*------------------------------------------------------------------*/
          /* Send a positive response (independent of the                     */
          /* suppressPosRspMsgIndicationBit because of previous Rsp pending)  */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

          t_ResultCheckSignation_UB = (UBYTE) 0x00u;

#ifdef cs_Signatur_Aktiv
  #ifndef cs_Decryption_Aktiv
          /*------------------------------------------------------------------*/
          /* init TXDelayCounter_UL for responsePending to zero               */
          /*------------------------------------------------------------------*/
          FUN_CCwtdog_InitExtWatchdogIntern_V();

          /*==================================================================*/
          /* -5- calculate over whole block                                   */
          /*==================================================================*/
          for (t_Address_UL = t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL; \
               t_Address_UL < t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL; \
               t_Address_UL = t_Address_UL + (ULONG) d_CCkwpaudi_SignaturePackedLength)
          {
            /*----------------------------------------------------------------*/
            /* set address and length of each block                           */
            /*----------------------------------------------------------------*/
            g_SignatureType.sigSourceBuffer = (const UBYTE *) t_Address_UL;
            g_SignatureType.sigByteCount    = (UWORD) d_CCkwpaudi_SignaturePackedLength;

            /*================================================================*/
            /* Do Calculation                                                 */
            /*================================================================*/
            SecM_VerifySignature(&g_SignatureType);

            /*----------------------------------------------------------------*/
            /* trigger watchdog                                               */
            /*----------------------------------------------------------------*/
            FUN_CCwtdog_TriggerWatchdogEraseMem_V ();

          } /* --5- END OF for(..) -------------------------------------------*/
  #endif

          /*==================================================================*/
          /* finalize the hash calculation                                    */
          /*==================================================================*/
          g_SignatureType.sigState = kHashFinalize;
          SecM_VerifySignature(&g_SignatureType);

          /*------------------------------------------------------------------*/
          /* write signature and length for verification                      */
          /*------------------------------------------------------------------*/
          g_SignatureType.sigSourceBuffer = (const UBYTE *)&t_InBuffer_PBUF[d_CCglo_StartOfData + d_CCglo_07Byte + \
                                                          t_InBuffer_PBUF[d_CCglo_StartOfData +  0x04]];
          g_SignatureType.sigByteCount    = 0x80u;

          /*------------------------------------------------------------------*/
          /* set pointer to watchdog function                                 */
          /*------------------------------------------------------------------*/
          g_SignatureType.wdTriggerFct = \
                  (FL_WDTriggerFctType) FUN_CCwtdog_TriggerWatchdogEraseMem_V;

          /*==================================================================*/
          /* do verification                                                  */
          /*==================================================================*/
          g_SignatureType.sigState = kSigVerify;
          t_ResultCheckSignation_UB = SecM_VerifySignature(&g_SignatureType);
#endif

          if ((UBYTE) 0x00u != t_ResultCheckSignation_UB)
          {
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

            /*----------------------------------------------------------------*/
            /* Update the EEPROM: set the block invalid                         */
            /*----------------------------------------------------------------*/
            t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                               t_ValidationIndex_UB,  \
                                               d_CCkwpaudi_ProgStateSWInValid );

            /*================================================================*/
            /* -5- an error has accoured                                        */
            /*================================================================*/
            if (d_CCglo_Idle != t_ReturnValue_UB)
            {
              t_Result_UB = t_ReturnValue_UB;

            } /* ---------5- END OF if(..) -----------------------------------*/

            /* reset flags                                                        */
            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;
          }
          else
          {

            /*----------------------------------------------------------------*/
            /* Update the EEPROM: set the block valid                           */
            /*----------------------------------------------------------------*/
            t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                               t_ValidationIndex_UB,  \
                                               d_CCkwpaudi_ProgStateSWValid );

            /*================================================================*/
            /* -5- an error has accoured                                        */
            /*================================================================*/
            if (d_CCglo_Idle != t_ReturnValue_UB)
            {
              t_Result_UB = t_ReturnValue_UB;

            } /* ---------5- END OF if(..) -----------------------------------*/

            /* Set/reset flags                                                  */
            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagReadyforCheckMemory;
            t_CCkwpaudi_KwpCtrl_ST.State_UW |= (UWORD) d_CCkwpaudi_FlagChecksumCorrect;
            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;
          }
          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          /*------------------------------------------------------------------*/
          /* Checksum calculation interrupted                                 */
          /*------------------------------------------------------------------*/
        case d_CCglo_Busy:
          /* Set/reset flags                                                  */
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
          t_CCkwpaudi_KwpCtrl_ST.State_UW |= (UWORD) d_CCkwpaudi_FlagRoutineBusy;

          /*------------------------------------------------------------------*/
          /* set return value - response index 2                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);

          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          /*------------------------------------------------------------------*/
          /* Checksum calculation finished but checksum is not correct        */
          /*------------------------------------------------------------------*/
        default:

          /*------------------------------------------------------------------*/
          /* Send a positive response (incorrect result; independent of the   */
          /* suppressPosRspMsgIndicationBit because of previous Rsp pending)  */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

          /*------------------------------------------------------------------*/
          /* Update the EEPROM: set the block invalid                         */
          /*------------------------------------------------------------------*/
          t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                             t_ValidationIndex_UB,  \
                                             d_CCkwpaudi_ProgStateSWInValid );

          /*==================================================================*/
          /* -5- an error has accoured                                        */
          /*==================================================================*/
          if (d_CCglo_Idle != t_ReturnValue_UB)
          {
            t_Result_UB = t_ReturnValue_UB;

          } /* ---------5- END OF if(..) -------------------------------------*/

          /* reset flags                                                        */
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        } /* -----------4- END OF switch (..) --------------------------------*/
      }

      /*======================================================================*/
      /* -3- table entry is a ram memory entry                                */
      /*======================================================================*/
      else if ( d_CCglo_RamMemory == \
                    (d_CCglo_MemoryTypeMask & t_CCkwpaudi_KwpCtrl_ST.  \
                    ValidationMap_PST[t_ValidationIndex_UB].CheckCondition_UW ))
      {

#ifdef d_CCkwpaudi_DoCrcAfterWrite
        /*--------------------------------------------------------------------*/
        /* Call the checksum calculation routine for ram download             */
        /*--------------------------------------------------------------------*/
        t_Result_UB = FUN_CCvalaudi_CalcCsumRamDownload_UB ( \
                          t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL, \
                          t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL, \
                          (ULONG) &t_CCkwpaudi_KwpCtrl_ST.Tester_Checksum_UL);
#else
        /*--------------------------------------------------------------------*/
        /* initialise return value                                            */
        /*--------------------------------------------------------------------*/
        t_Result_UB = d_CCglo_NoError;
#endif

        /*====================================================================*/
        /* -4- Checksum is correct                                            */
        /*====================================================================*/
        if (t_Result_UB == d_CCglo_NoError)
        {
          t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagChecksumCorrect;

          /*==================================================================*/
          /* -5- positiv response shall be suppressed                         */
          /*==================================================================*/
          if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                          t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) )
          {
            /*----------------------------------------------------------------*/
            /* set return value - will be overwritten by an error             */
            /* or other state                                                 */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex07);

            /*----------------------------------------------------------------*/
            /* Set the DLC of the OutBuffer to zero                           */
            /*----------------------------------------------------------------*/
            t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

          } /* -5- END OF if(..) ---------------------------------------------*/

          /*==================================================================*/
          /* -5- positiv response shall be sent                               */
          /*==================================================================*/
          else
          {
            t_ResultCheckSignation_UB = (UBYTE) 0x00u;

#ifdef cs_Signatur_Aktiv
  #ifndef cs_Decryption_Aktiv
            /*----------------------------------------------------------------*/
            /* init TXDelayCounter_UL for responsePending to zero             */
            /*----------------------------------------------------------------*/
            FUN_CCwtdog_InitExtWatchdogIntern_V();

            /*================================================================*/
            /* -6- calculate over whole block                                 */
            /*================================================================*/
            for (t_Address_UL = t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL; \
                t_Address_UL < t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL; \
                t_Address_UL = t_Address_UL + (ULONG) d_CCkwpaudi_SignaturePackedLength)
            {
              /*--------------------------------------------------------------*/
              /* set address and length of each block                         */
              /*--------------------------------------------------------------*/
              g_SignatureType.sigSourceBuffer = (const UBYTE *) t_Address_UL;
              g_SignatureType.sigByteCount    = (UWORD) d_CCkwpaudi_SignaturePackedLength;

              /*==============================================================*/
              /* Do Calculation                                               */
              /*==============================================================*/
              SecM_VerifySignature(&g_SignatureType);

              /*--------------------------------------------------------------*/
              /* trigger watchdog                                             */
              /*--------------------------------------------------------------*/
              FUN_CCwtdog_TriggerWatchdogEraseMem_V ();

            } /* --6- END OF for(..) -----------------------------------------*/
  #endif

            /*================================================================*/
            /* finalize the hash calculation                                  */
            /*================================================================*/
            g_SignatureType.sigState = kHashFinalize;
            SecM_VerifySignature(&g_SignatureType);

            /*----------------------------------------------------------------*/
            /* write signature and length for verification                    */
            /*----------------------------------------------------------------*/
            g_SignatureType.sigSourceBuffer = (const UBYTE *)&t_InBuffer_PBUF[d_CCglo_StartOfData + d_CCglo_07Byte + \
                                                          t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]];
            g_SignatureType.sigByteCount    = 0x80u;

            /*================================================================*/
            /* do verification                                                */
            /*================================================================*/
            g_SignatureType.sigState = kSigVerify;
            t_ResultCheckSignation_UB = SecM_VerifySignature(&g_SignatureType);
#endif

            if ((UBYTE) 0x00u != t_ResultCheckSignation_UB)
            {
              /*--------------------------------------------------------------*/
              /* clear the flash functions                                    */
              /*--------------------------------------------------------------*/
              FUN_CCkwpaudi_ClearFlashFunctions_V();

              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);
            }
            else
            {
              /*--------------------------------------------------------------*/
              /* set return value - response index 0                          */
              /*--------------------------------------------------------------*/
              t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

            } /* -6- END OF else (if(..)) ------------------------------------*/

            t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

          } /* -5- END OF else(..) -------------------------------------------*/
        } /* ---4- END OF if(..) ---------------------------------------------*/
        /*====================================================================*/
        /* -4- Checksum is incorrect                                          */
        /*====================================================================*/
        else
        {
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;

          /*------------------------------------------------------------------*/
          /* clear the flash functions                                        */
          /*------------------------------------------------------------------*/
          FUN_CCkwpaudi_ClearFlashFunctions_V();

          /*==================================================================*/
          /* -5- positiv response shall be suppressed                         */
          /*==================================================================*/
          if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                          t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) )
          {
            /*----------------------------------------------------------------*/
            /* set return value - will be overwritten by an error             */
            /* or other state                                                 */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex08);

            /*----------------------------------------------------------------*/
            /* Set the DLC of the OutBuffer to zero                           */
            /*----------------------------------------------------------------*/
            t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

          } /* -5- END OF if(..) ---------------------------------------------*/

          /*==================================================================*/
          /* -5- positiv response shall be sent                               */
          /*==================================================================*/
          else
          {
            /*----------------------------------------------------------------*/
            /* set return value - response index 5                            */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

          } /* -5- END OF else (if(..)) --------------------------------------*/

          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

        } /* -4- END OF else (if(..)) ----------------------------------------*/
      } /* ---3- END OF else if (..) -----------------------------------------*/

      /*======================================================================*/
      /* -3- table entry is not a ram memory entry                            */
      /*======================================================================*/

      /*======================================================================*/
      /* -3- table entry is an eeprom entry                                   */
      /*======================================================================*/
      else
      {

#ifdef d_CCkwpaudi_DoCrcAfterWrite
        /*--------------------------------------------------------------------*/
        /* Call the checksum calculation routine for flash download           */
        /*--------------------------------------------------------------------*/
        t_CheckState_UB = FUN_CCvalaudi_CalcCsumFlashDownload_UB ();
#else
        /*--------------------------------------------------------------------*/
        /* initialise return value                                            */
        /*--------------------------------------------------------------------*/
        t_CheckState_UB = d_CCglo_Ready;
#endif

        /*====================================================================*/
        /* -4- CheckState evaluation                                          */
        /*====================================================================*/
        switch (d_CCglo_ResultInfoMask & t_CheckState_UB)
        {
          /*------------------------------------------------------------------*/
          /* Checksum calculation finished and Checksum is correct            */
          /*------------------------------------------------------------------*/
        case d_CCglo_Ready:

          /* Set/reset flags                                                  */
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagReadyforCheckMemory;
          t_CCkwpaudi_KwpCtrl_ST.State_UW |= (UWORD) d_CCkwpaudi_FlagChecksumCorrect;
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

          /*------------------------------------------------------------------*/
          /* Send a positive response (independent of the                     */
          /* suppressPosRspMsgIndicationBit because of previous Rsp pending)  */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

#ifdef cs_Signatur_Aktiv
  #ifndef cs_Decryption_Aktiv
          /*------------------------------------------------------------------*/
          /* init TXDelayCounter_UL for responsePending to zero               */
          /*------------------------------------------------------------------*/
          FUN_CCwtdog_InitExtWatchdogIntern_V();

          /*------------------------------------------------------------------*/
          /* get start address of block                                       */
          /*------------------------------------------------------------------*/
          t_Address_UL = t_CCkwpaudi_KwpCtrl_ST.ProgStartAddress_UL;

          /*------------------------------------------------------------------*/
          /* -5- do with whole block                                          */
          /*------------------------------------------------------------------*/
          do
          {
            /*----------------------------------------------------------------*/
            /* set default length for read access                             */
            /*----------------------------------------------------------------*/
            t_Length_UL = (ULONG) d_CCkwpaudi_SignaturePackedLength;

            /*----------------------------------------------------------------*/
            /* -6- last part is smaller than default length                   */
            /*----------------------------------------------------------------*/
            if (((t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL - t_Address_UL) + 0x01ul) <  t_Length_UL)
            {
              /*--------------------------------------------------------------*/
              /* set length on actual size of last block                      */
              /*--------------------------------------------------------------*/
              t_Length_UL = (t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL - t_Address_UL) + 0x01ul;

            } /* -6- END OF if (..) ------------------------------------------*/

            /*----------------------------------------------------------------*/
            /* copy all access data to runtime entry structure                */
            /*----------------------------------------------------------------*/
            t_CCnvm_NvmRunTimeEntry_ST.Access_UB      = d_CCglo_Read;
            t_CCnvm_NvmRunTimeEntry_ST.StorageType_UB = d_CCglo_SingleBuffering;
            t_CCnvm_NvmRunTimeEntry_ST.DataLength_UW  = (UWORD) t_Length_UL;
            t_CCnvm_NvmRunTimeEntry_ST.TotalLength_UW = (UWORD) t_Length_UL;
            t_CCnvm_NvmRunTimeEntry_ST.Address_UW     = (UWORD) t_Address_UL;
            t_CCnvm_NvmRunTimeEntry_ST.Buffer_UB      = (PTR_BUFFER_UB) &DataBuffer_UB;

            /*----------------------------------------------------------------*/
            /* Call the specific EEPROM Handler                               */
            /*----------------------------------------------------------------*/
            t_Result_UB = FUN_CCssi_EepromHandler_UB((const NVMRUNTIMEENTRY_ST*)&t_CCnvm_NvmRunTimeEntry_ST);

            /*----------------------------------------------------------------*/
            /* set address and length                                         */
            /*----------------------------------------------------------------*/
            g_SignatureType.sigSourceBuffer = (const UBYTE *) DataBuffer_UB;
            g_SignatureType.sigByteCount    = (UWORD) t_Length_UL;

            /*================================================================*/
            /* Do Calculation                                                 */
            /*================================================================*/
            SecM_VerifySignature(&g_SignatureType);

            /*----------------------------------------------------------------*/
            /* inkrement address                                              */
            /*----------------------------------------------------------------*/
            t_Address_UL = t_Address_UL + t_Length_UL;

            /*----------------------------------------------------------------*/
            /* trigger watchdog                                               */
            /*----------------------------------------------------------------*/
            FUN_CCwtdog_TriggerWatchdogEraseMem_V ();

          } /* ---5- END OF do (..) ------------------------------------------*/

          /*------------------------------------------------------------------*/
          /* continue till reached end of block                               */
          /*------------------------------------------------------------------*/
          while (t_Address_UL <= t_CCkwpaudi_KwpCtrl_ST.ProgEndAddress_UL);

  #endif

          /*==================================================================*/
          /* finalize the hash calculation                                    */
          /*==================================================================*/
          g_SignatureType.sigState = kHashFinalize;
          SecM_VerifySignature(&g_SignatureType);

          /*------------------------------------------------------------------*/
          /* write signature and length for verification                      */
          /*------------------------------------------------------------------*/
          g_SignatureType.sigSourceBuffer = (const UBYTE *)&t_InBuffer_PBUF[d_CCglo_StartOfData + d_CCglo_07Byte + \
                                                        t_InBuffer_PBUF[d_CCglo_StartOfData + 0x04]];
          g_SignatureType.sigByteCount    = 0x80u;

          /*==================================================================*/
          /* do verification                                                  */
          /*==================================================================*/
          g_SignatureType.sigState  = kSigVerify;
          t_ResultCheckSignation_UB = SecM_VerifySignature(&g_SignatureType);

          /*==================================================================*/
          /* -5- signature ok                                                 */
          /*==================================================================*/
          if (0x00 == t_ResultCheckSignation_UB)
          {
            /*----------------------------------------------------------------*/
            /* Send a positive response (independent of the                   */
            /* suppressPosRspMsgIndicationBit because of previous Rsp pending)*/
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

            /*------------------------------------------------------------------*/
            /* Update the EEPROM: set the block valid                           */
            /*------------------------------------------------------------------*/
            t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                               t_ValidationIndex_UB,  \
                                               d_CCkwpaudi_ProgStateSWValid );

            /*==================================================================*/
            /* -4- an error has accoured                                        */
            /*==================================================================*/
            if (d_CCglo_Idle != t_ReturnValue_UB)
            {
              t_Result_UB = t_ReturnValue_UB;

            } /* ---------4- END OF if(..) -------------------------------------*/

          } /* ---5- END OF if (..) ------------------------------------------*/
          /*==================================================================*/
          /* -5- signature not ok                                             */
          /*==================================================================*/
          else
          {
            t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

            /*------------------------------------------------------------------*/
            /* Update the EEPROM: set the block invalid                         */
            /*------------------------------------------------------------------*/
            t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                               t_ValidationIndex_UB,  \
                                               d_CCkwpaudi_ProgStateSWInValid );

            /*==================================================================*/
            /* -4- an error has accoured                                        */
            /*==================================================================*/
            if (d_CCglo_Idle != t_ReturnValue_UB)
            {
              t_Result_UB = t_ReturnValue_UB;

            } /* ---------4- END OF if(..) -------------------------------------*/

          } /* ---5- END OF else (..) ----------------------------------------*/
#else
          /*------------------------------------------------------------------*/
          /* Update the EEPROM: set the block valid                           */
          /*------------------------------------------------------------------*/
          t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                             t_ValidationIndex_UB,  \
                                             d_CCkwpaudi_ProgStateSWValid );

          /*==================================================================*/
          /* -4- an error has accoured                                        */
          /*==================================================================*/
          if (d_CCglo_Idle != t_ReturnValue_UB)
          {
            t_Result_UB = t_ReturnValue_UB;

          } /* ---------4- END OF if(..) -------------------------------------*/

#endif

          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          /*------------------------------------------------------------------*/
          /* Checksum calculation interrupted                                 */
          /*------------------------------------------------------------------*/
        case d_CCglo_Busy:
          /* Set/reset flags                                                  */
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
          t_CCkwpaudi_KwpCtrl_ST.State_UW |= (UWORD) d_CCkwpaudi_FlagRoutineBusy;

          /*------------------------------------------------------------------*/
          /* set return value - response index 2                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex02);

          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

          /*------------------------------------------------------------------*/
          /* Checksum calculation finished but an error occured               */
          /*------------------------------------------------------------------*/
        default:

          /*------------------------------------------------------------------*/
          /* Send a positive response (incorrect result; independent of the   */
          /* suppressPosRspMsgIndicationBit because of previous Rsp pending)  */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

          /*------------------------------------------------------------------*/
          /* Update the EEPROM: set the block invalid                         */
          /*------------------------------------------------------------------*/
          t_ReturnValue_UB = FUN_CCkwpaudi_UpdateBlockValadity_UB ( \
                                             t_ValidationIndex_UB,  \
                                             d_CCkwpaudi_ProgStateSWInValid );

          /*==================================================================*/
          /* -4- an error has accoured                                        */
          /*==================================================================*/
          if (d_CCglo_Idle != t_ReturnValue_UB)
          {
            t_Result_UB = t_ReturnValue_UB;

          } /* ---------4- END OF if(..) -------------------------------------*/


          /* reset flags                                                      */
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagChecksumCorrect;
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

          break;
          /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        } /* -----------4- END OF switch (..) --------------------------------*/
      } /* -------------3- END OF else (if(..)) ------------------------------*/
    } /* ---------------2- END OF else (if(..)) ------------------------------*/

    break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /* all other routine control types are not supported                      */
    /*------------------------------------------------------------------------*/
  default:

    /*------------------------------------------------------------------------*/
    /* check the addressing mode for functional addressing                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

    /*========================================================================*/
    /* -2- service is functionally addressed                                  */
    /*========================================================================*/
    if ( d_CCglo_Ready == t_Result_UB )
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex09);

      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

    } /* ---2- END OF if(..) -------------------------------------------------*/
    /*========================================================================*/
    /* -2- service is not functionally addressed                              */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* set return value - response index 1                                  */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    } /* ---2- END OF else (if(..)) ------------------------------------------*/

    t_CCkwpaudi_KwpCtrl_ST.State_UW &= (UWORD) ~d_CCkwpaudi_FlagRoutineBusy;

    break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  } /* -----------------1- END OF switch (..) --------------------------------*/


  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_RcCheckMemory_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $31 - RoutineControl                                         */
/*  LOCAL ID   : $xx                                                          */
/*  Routine ID : $02 -+                                                       */
/*  Routine ID : $02 -+- Check memory                                         */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRcCheckMemory_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_RcCheckMemoryDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_05Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpaudi_RIDcheckMemoryByte0,
  /* 05 - RESPONSE BYTE 03 */  d_CCkwpaudi_RIDcheckMemoryByte1,
  /* 06 - RESPONSE BYTE 04 */  d_CCkwpaudi_correctResult,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - SubFunction not supported                                     */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSubFunctionNotSupported,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 12 - POST CONDIDTION  */  d_CCglo_NoPostCondDefined,
  /* 13 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 14 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 15 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 16 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - parameter not correct                                         */
  /*--------------------------------------------------------------------------*/
  /* 17 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 18 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 19 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 20 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 21 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 -  Length Error                                                 */
  /*--------------------------------------------------------------------------*/
  /* 22 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 23 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 24 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 25 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 26 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat,
  /*--------------------------------------------------------------------------*/
  /* INDEX 05 - Checksum Error                                                */
  /*--------------------------------------------------------------------------*/
  /* 27 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 28 - RESPONSE LENGTH  */  d_CCglo_05Byte,
  /* 29 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 30 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 31 - RESPONSE BYTE 02 */  d_CCkwpaudi_RIDcheckMemoryByte0,
  /* 32 - RESPONSE BYTE 03 */  d_CCkwpaudi_RIDcheckMemoryByte1,
  /* 33 - RESPONSE BYTE 04 */  d_CCkwpaudi_incorrectResult,
  /*--------------------------------------------------------------------------*/
  /* INDEX 06 - Programming failure                                           */
  /*--------------------------------------------------------------------------*/
  /* 34 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 35 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 36 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 37 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 38 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCGeneralProgFailure,
  /*--------------------------------------------------------------------------*/
  /* INDEX 07 - Suppressed Positive Response                                  */
  /*--------------------------------------------------------------------------*/
  /* 39 - POST CONDIDTION  */  d_CCseq_RcEraseMemoryDone,
  /* 40 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 08 - Suppressed Positive Response, check not correct               */
  /*--------------------------------------------------------------------------*/
  /* 41 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 42 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 09 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 43 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 44 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 0A - Security access denied                                        */
  /*--------------------------------------------------------------------------*/
  /* 45 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 46 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 47 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 48 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 49 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSecurityAccessDenied,
  /*--------------------------------------------------------------------------*/
  /* INDEX 0B - conditions not correct                                        */
  /*--------------------------------------------------------------------------*/
  /* 50 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 51 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 52 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 53 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 54 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect,
  /*--------------------------------------------------------------------------*/
  /* INDEX 0C - conditions not correct                                        */
  /*--------------------------------------------------------------------------*/
  /* 55 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 56 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 57 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 58 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 59 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestSequenceError0x24
}; /*--- END OF TABLE --------------------------------------------------------*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_UpdateBlockValadity_UB                      |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000Audi.c                                     |
|                                                                            |
|  Description:    This function updates the EEPROM values in the            |
|                  CheckMemory function. Block will be set valid or invalid. |
|                                                                            |
|  Parameter:      t_ValidationIndex_UB  Index of the actual block           |
|                  t_ProgrammigState_UB  valid or invalid ...                |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_UpdateBlockValadity_UB ( UBYTE t_ValidationIndex_UB, \
                                             UBYTE t_ProgrammigState_UB )
{
  UBYTE  t_Result_UB;

  UBYTE  t_CounterHelp_AUB[2];
  UNION2 t_CountProgram_U2;

  UBYTE  t_Index_UB;
  UBYTE  t_ScanState_UB;
  UBYTE  t_ReturnValue_UB;

  /*--------------------------------------------------------------------------*/
  /* set return value - function has not detected                             */
  /* any error                                                                */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Idle );

  /*==========================================================================*/
  /* -1- the checked memory has an corresponding entry in the                 */
  /*     download map                                                         */
  /*==========================================================================*/
  if ( d_CCglo_NoEntry != t_CCkwpaudi_KwpCtrl_ST.ValidationMap_PST \
                                   [t_ValidationIndex_UB].DownloadName_UW)
  {
    /*------------------------------------------------------------------------*/
    /* Init internal variables                                                */
    /*------------------------------------------------------------------------*/
    t_ScanState_UB   = d_CCkwpaudi_ScanRun;
    t_Index_UB       = 0x00;
    t_ReturnValue_UB = d_CCglo_NoError;

    /*========================================================================*/
    /* -2- Check the complete Table                                           */
    /*========================================================================*/
    while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
      ( d_CCglo_LastEntry !=                 \
      (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW )))
    {
      /*======================================================================*/
      /* -3- entry found in the download map                                  */
      /*======================================================================*/
      if ( t_CCkwpaudi_KwpCtrl_ST.ValidationMap_PST[t_ValidationIndex_UB].  \
           DownloadName_UW == t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].Name_UB )
      {
        /*--------------------------------------------------------------------*/
        /* stop scan                                                          */
        /*--------------------------------------------------------------------*/
        t_ScanState_UB   = d_CCkwpaudi_ScanStop;

        /*--------------------------------------------------------------------*/
        /* get the name (= index of the array                                 */
        /* c_CCconprj_LogBlockEEPROM_AUB) of the block                        */
        /*--------------------------------------------------------------------*/
        t_Index_UB = t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].Name_UB;  /* [OPL: 302; 2 von 3] 3. in CheckDepend! */

        /*====================================================================*/
        /* -4- the block is valid now                                         */
        /*====================================================================*/
        if ( d_CCkwpaudi_ProgStateSWValid == t_ProgrammigState_UB)
        {

          /*------------------------------------------------------------------*/
          /* read out the actual number of programming attempts               */
          /*------------------------------------------------------------------*/
          FUN_CCnvm_NvmAccess_UB ( \
                c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWSucProgrAttemptsIndex],  \
                &t_CounterHelp_AUB[0]);

          /*------------------------------------------------------------------*/
          /* use union for right endianess                                    */
          /*------------------------------------------------------------------*/
          t_CountProgram_U2.UByte_ST.UByte0_UB = t_CounterHelp_AUB[0];
          t_CountProgram_U2.UByte_ST.UByte1_UB = t_CounterHelp_AUB[1];

          /*------------------------------------------------------------------*/
          /* increment the programming counter                                */
          /*------------------------------------------------------------------*/
          t_CountProgram_U2.UWord_ST.UWord0_UW++;
          /*------------------------------------------------------------------*/
          /* use union for right endianess                                    */
          /*------------------------------------------------------------------*/
          t_CounterHelp_AUB[0] = t_CountProgram_U2.UByte_ST.UByte0_UB;
          t_CounterHelp_AUB[1] = t_CountProgram_U2.UByte_ST.UByte1_UB;

          FUN_CCnvm_NvmAccess_UB ((UBYTE)( d_CCglo_Write |  \
                  c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWSucProgrAttemptsIndex]),  \
                  &t_CounterHelp_AUB[0]);

        } /* ---4- END OF if(..) ---------------------------------------------*/

        /*--------------------------------------------------------------------*/
        /* set the software block valid or invalid and write it into          */
        /* the EEPROM                                                         */
        /*--------------------------------------------------------------------*/
        t_ReturnValue_UB |= FUN_CCnvm_NvmAccess_UB ((UBYTE)( d_CCglo_Write |  \
                                 c_CCconprj_LogBlockEEPROM_AUB[t_Index_UB][d_CCconprj_SWProgStateIndex] ),  \
                                 &t_ProgrammigState_UB);

        /*====================================================================*/
        /* -4- the nvm access was not successful                              */
        /*====================================================================*/
        if ( d_CCglo_NoError != t_ReturnValue_UB )
        {
          /*------------------------------------------------------------------*/
          /* set return value - response index 6                              */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex06);

        } /* ---4- END OF if(..) ---------------------------------------------*/

      } /* -----3- END OF if(..) ---------------------------------------------*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Check next entry                                                   */
        /*--------------------------------------------------------------------*/
        t_Index_UB++;

      } /* -----3- END OF else (if(..)) --------------------------------------*/
    } /* -------2- END OF while(..) ------------------------------------------*/
  } /* ---------1- END OF if(..) ---------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return (t_Result_UB );

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_UpdateBlockValadity_UB' +-+-+-+-+-+-+-+*/



/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_RcChkProgDependencies_UB                    |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $31 - Routine control                         |
|                  Local ID  : $xx - Routine control type                    |
|                  Routine ID: $FF -+                                        |
|                  Routine ID: $01 -+- check programming dependencies        |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RcChkProgDependencies_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                               PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE  t_Result_UB;
  UBYTE  t_ReturnValue_UB;

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= 0x00;
  t_InBuffer_PBUF[0] |= 0x00;

  /*==========================================================================*/
  /* -1- the routine identifier is not correct (check has to be done          */
  /*     because routine identifer includes a 0xFF which cannot be checked    */
  /*     in the session handler                                               */
  /*==========================================================================*/
  if( (d_CCkwpaudi_RIDchkProgDependB0 != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02]) ||
      (d_CCkwpaudi_RIDchkProgDependB1 != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x03])  )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 4                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);

  } /* -----------1- END OF if(..) -------------------------------------------*/
  /*==========================================================================*/
  /* -1- the routine identifier is correct                                    */
  /*==========================================================================*/
  else
  {
    if (t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB !=  (UBYTE)0x02u)
    {
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex03);
    }
    /*------------------------------------------------------------------------*/
    /* init variables                                                         */
    /*------------------------------------------------------------------------*/
    t_ReturnValue_UB = d_CCglo_NoError;

    /*========================================================================*/
    /* -2- check the routine control type                                     */
    /*========================================================================*/
    switch (d_CCglo_LIDSubFunctionMask & t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01])
    {
      /*----------------------------------------------------------------------*/
      /* start routine                                                        */
      /*----------------------------------------------------------------------*/
    case d_CCkwpbase_LIDStartRoutine:

      /*======================================================================*/
      /* -2- Function was called a first time                                 */
      /*======================================================================*/
      if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
        d_CCkwpaudi_FlagRoutineBusy )
      {
        /*--------------------------------------------------------------------*/
        /* Set the internal busy flag                                         */
        /*--------------------------------------------------------------------*/
        t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

        /*--------------------------------------------------------------------*/
        /* response pending                                                   */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex06);

      } /* ---------2- END OF if(..) -----------------------------------------*/
      /*======================================================================*/
      /* -2- Function was called one more time                                */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Reset the internal busy flag                                       */
        /*--------------------------------------------------------------------*/
        t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

        /*--------------------------------------------------------------------*/
        /* set return value - will be overwritten by an error or other state  */
        /* Does not depend on the addressing mode, because a response         */
        /* pending has been sent.                                             */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

        /*--------------------------------------------------------------------*/
        /* set Flag that the check of programming dependencies was successful.*/
        /* Will be resetted in case of an error.                              */
        /*--------------------------------------------------------------------*/
        t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagProgDependenciesOk;

        /*====================================================================*/
        /* NOTE:                                                              */
        /*       the whole applicationsoftware is only valid if each          */
        /*       softwareblock is valid itself.                               */
        /*       The Adlatus software block is neglected in this check.       */
        /*====================================================================*/

        t_ReturnValue_UB = FUN_CCcd_CheckDependAudi_UB ( (DOWNLOADMAP_ST*) &t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[0]);

        /*====================================================================*/
        /* -3- one block of the application is invalid                        */
        /*====================================================================*/
        if (d_CCglo_NoError != t_ReturnValue_UB)
        {
          /*------------------------------------------------------------------*/
          /* reset the flag that checkProgrammingDependencies was successful  */
          /*------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~(UWORD)d_CCkwpaudi_FlagProgDependenciesOk;

          /*==================================================================*/
          /* -4- positiv response shall be suppressed                         */
          /*==================================================================*/
          if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
                                       t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) )
          {
            /*----------------------------------------------------------------*/
            /* set return value - will be overwritten by an error or other    */
            /* state                                                          */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | t_ReturnValue_UB);

            /*----------------------------------------------------------------*/
            /* Set the DLC of the OutBuffer to zero                           */
            /*----------------------------------------------------------------*/
            t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

          } /* -----4- END OF if(..) -----------------------------------------*/
          /*==================================================================*/
          /* -4- positiv response shall be send                               */
          /*==================================================================*/
          else
          {
            /*----------------------------------------------------------------*/
            /* set return value - will be overwritten by an error or other    */
            /* state                                                          */
            /*----------------------------------------------------------------*/
            t_Result_UB = ( d_CCglo_Ready | t_ReturnValue_UB);

          } /* -----4- END OF else (if(..)) ----------------------------------*/
        } /* -------3- END OF if(..) -----------------------------------------*/
      } /* ---------2- END OF else(..) ---------------------------------------*/


      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /*----------------------------------------------------------------------*/
      /* all other routine control types are not supported                    */
      /*----------------------------------------------------------------------*/
    default:

      /*----------------------------------------------------------------------*/
      /* check the addressing mode for functional addressing                  */
      /*----------------------------------------------------------------------*/
      t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                   (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

      /*======================================================================*/
      /* -3- service is functionally addressed                                */
      /*======================================================================*/
      if ( d_CCglo_Ready == t_Result_UB )
      {
        /*--------------------------------------------------------------------*/
        /* set return value - will be overwritten by an error or other state  */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

        /*--------------------------------------------------------------------*/
        /* Set the DLC of the OutBuffer to zero                               */
        /*--------------------------------------------------------------------*/
        t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

      } /* -------3- END OF if(..) -------------------------------------------*/
      /*======================================================================*/
      /* -3- service is not functionally addressed                            */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* set return value - response index 1                                */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

      } /* -------3- END OF else (if(..)) ------------------------------------*/

      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    } /* ---------2- END OF switch (..) --------------------------------------*/
  } /* -----------1- END OF else (if(..)) ------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_RcChkProgDependencies_UB'      -+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $31 - RoutineControl                                         */
/*  LOCAL ID   : $xx                                                          */
/*  Routine ID : $FF -+                                                       */
/*  Routine ID : $01 -+- check programming dependencies                       */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRcChkProgDependencies_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseq_RcChkProgDependenciesDone,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_05Byte,
  /* 02 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpaudi_RIDchkProgDependB0,
  /* 05 - RESPONSE BYTE 03 */  d_CCkwpaudi_RIDchkProgDependB1,
  /* 06 - RESPONSE BYTE 04 */  d_CCkwpaudi_correctResult,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - SubFunction not supported                                     */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSubFunctionNotSupported,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Positive Response with negative check result                  */
  /*--------------------------------------------------------------------------*/
  /* 12 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 13 - RESPONSE LENGTH  */  d_CCglo_05Byte,
  /* 14 - RESPONSE BYTE 00 */  (d_CCkwpbase_SIDRoutineControl + (UBYTE) 0x40u),
  /* 15 - RESPONSE BYTE 01 */  d_CCkwpbase_LIDStartRoutine,
  /* 16 - RESPONSE BYTE 02 */  d_CCkwpaudi_RIDchkProgDependB0,
  /* 17 - RESPONSE BYTE 03 */  d_CCkwpaudi_RIDchkProgDependB1,
  /* 18 - RESPONSE BYTE 04 */  d_CCkwpaudi_incorrectResult,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - conditions not correct                                        */
  /*--------------------------------------------------------------------------*/
  /* 19 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 20 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 21 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 22 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 23 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 - request out of range                                          */
  /*--------------------------------------------------------------------------*/
  /* 24 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 25 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 26 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 27 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 28 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 05 - Suppress Negative Repsonse, parameter not supported           */
  /*--------------------------------------------------------------------------*/
  /* 29 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 30 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 06 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 31 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 32 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 33 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 34 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 35 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending

}; /*--- END OF TABLE --------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_RcRoutineIdNotSupported_UB                    |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Service Function for the KWP2000 Service:                   |
|                                                                              |
|                  Service ID: $31 - Routine control                           |
|                  Local ID  : $xx - Routine control type                      |
|                  Routine ID: $XX -+                                          |
|                  Routine ID: $XX -+- unknown routine identifier              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    RESPONSE INDEX                |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RcRoutineIdNotSupported_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                 PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= 0x00;
  t_InBuffer_PBUF[0] |= 0x00;

  /*--------------------------------------------------------------------------*/
  /* set return value - response index 0                                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00); /* [OPL331] */

  if( (UBYTE)0x03u > t_InBuffer_PBUF[d_CCglo_DLC] )
  {
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);
  }
  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_RcRoutineIdNotSupported_UB' -+-+-+-+-+-*/

/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $31 - RoutineControl                                         */
/*  LOCAL ID   : $xx                                                          */
/*  Routine ID : $XX -+                                                       */
/*  Routine ID : $XX -+- unkonwn routine identifier                           */
/*                                                                            */
/*  NOTE: Response Length $00 indicates, that the response should be taken    */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRcRoutineIdNotSupported_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - request out of range                                          */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 02 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - invalid format                                                */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 02 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 03 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDRoutineControl,
  /* 04 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_RequestLengthError_UB                        |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    Standard Service Function for all "Request Length Errors", |
|                  if no especially response code or postcondition is         |
|                  required by the customer.                                  |
|                                                                             |
|                  Service ID: $xx - all                                      |
|                  Local ID  : $xx - all                                      |
|                  Command ID: $xx - all                                      |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_RequestLengthError_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                            PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /* Note: A KWP2000 negative response has got 3 byte!                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_03Byte;

  /*--------------------------------------------------------------------------*/
  /* Write the standard negative response for request length error:           */
  /* Byte 0 - 0x7F  "Negative Response"                                       */
  /* Byte 1 - SID   "Individual Service ID"                                   */
  /* Byte 2 - 0x13  "Incorrect Message Length or Invalid Format"              */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] = d_CCkwpbase_SIDNegativResponseID;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00];
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02] = d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat;

  /*--------------------------------------------------------------------------*/
  /* set return value - we are ready, send the response                       */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_RequestLengthError_UB' +-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - all                                                    */
/*  LOCAL ID   : $xx - all                                                    */
/*  CMD        : $xx - all                                                    */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTRequestLengthError_AUB[] = \
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Request Length Error                                          */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_SecurityAccessDenied_UB                      |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    Standard Service Function for all "S&K errors",            |
|                  if no especially response code or postcondition is         |
|                  required by the customer.                                  |
|                                                                             |
|                  Service ID: $xx - all                                      |
|                  Local ID  : $xx - all                                      |
|                  Command ID: $xx - all                                      |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_SecurityAccessDenied_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                            PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /* Note: A KWP2000 negative response has got 3 byte!                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_03Byte;
  t_CCkwpaudi_KwpCtrl_ST.State_UW |= d_CCkwpaudi_FlagSecurityAccessDenied;
  /*--------------------------------------------------------------------------*/
  /* Write the standard negative response for request length error:           */
  /* Byte 0 - 0x7F  "Negative Response"                                       */
  /* Byte 1 - SID   "Individual Service ID"                                   */
  /* Byte 2 - 0x33  "Security Access denied"                                  */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] = d_CCkwpbase_SIDNegativResponseID;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00];
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02] = d_CCkwpbase_NRCSecurityAccessDenied;

  /*--------------------------------------------------------------------------*/
  /* set return value - we are ready, send the response                       */
  /*--------------------------------------------------------------------------*/
  if(d_CCkwpbase_SIDStartRoutineByLid == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00])
  {
    if( ((UBYTE)0x04u > t_InBuffer_PBUF[d_CCglo_DLC]) && (t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB !=  (UBYTE)0x01u)) /* [OPL333_334] */
    {
      t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat;
    }
    else
    {
      if (((UBYTE)0x01u != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) && (t_InBuffer_PBUF[d_CCglo_DLC] > (UBYTE)0x01u)) /* [OPL339] */
      {
        t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCSubFunctionNotSupported;
      }
    }
  }
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+-+ END OF FUNCTION 'FUN_CCkwpaudi_SecurityAccessDenied_UB' +-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - all                                                    */
/*  LOCAL ID   : $xx - all                                                    */
/*  CMD        : $xx - all                                                    */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTSecurityAccessDenied_AUB[] = \
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Request Length Error                                          */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/

/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_GetSecurityLevel_UB                          |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    This function is a callback-function  provited by KWP2000  |
|                  to eval the S&K status from another SW-modul               |
|                                                                             |
|  Parameter:      none                                                       |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         U U U U  U U U S                                    |
|                         | | | |  | | | |                                    |
|                         |-+-+-+--+-+-+-+---    UNUSED                       |
|                         +------------------    Access denied                |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_GetSecurityLevel_UB()
{
  UBYTE t_result_UB = d_CCglo_NoError;

  if ((d_CCkwpaudi_FlagSecurityAccessDenied == \
      t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSecurityAccessDenied) ||
     ( 0x00u == (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagSecurityAccessGranted)))
  {
    t_result_UB = 0x80;
  }

  return t_result_UB;
 }



/*----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                  |
+-----------------------------------------------------------------------------+
|                                                                             |
|  Function Name:  FUN_CCkwpaudi_SequenceError_UB                             |
|                                                                             |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                  |
|                                                                             |
|  Description:    Standard Service Function for all "Sequence Errors",       |
|                  if no especially response code or postcondition is         |
|                  required by the customer.                                  |
|                                                                             |
|                  Service ID: $xx - all                                      |
|                  Local ID  : $xx - all                                      |
|                  Command ID: $xx - all                                      |
|                                                                             |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer      |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer     |
|                                                                             |
|  Return Value:   UBYTE  t_Result_UB                                         |
|                                                                             |
|                         R B E I  I I I I                                    |
|                         | | | |  | | | |                                    |
|                         | | | +--+-+-+-+---    RESPONSE INDEX               |
|                         | | +--------------    ERROR                        |
|                         | +----------------    BUSY                         |
|                         +------------------    READY                        |
|                                                                             |
+----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_SequenceError_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                       PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /* Note: A KWP2000 negative response has got 3 byte!                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_03Byte;

  /*--------------------------------------------------------------------------*/
  /* Write the standard negative response for sequence error:                 */
  /* Byte 0 - 0x7F  "Negative Response"                                       */
  /* Byte 1 - SID   "Individual Service ID"                                   */
  /* Byte 2 - 0x24  "Sequence error"                                          */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00] = d_CCkwpbase_SIDNegativResponseID;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01] = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00];
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02] = d_CCkwpbase_NRCRequestSequenceError0x24;

  /*==========================================================================*/
  /* -1- SID is not SecurityAccess and LID is not SendKey                     */
  /*==========================================================================*/
  if ((d_CCkwpbase_SIDSecurityAccess != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00]) && \
      (d_CCkwpbase_LIDSendKey        != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]))
    {
    /* [OPL328] */
    if (d_CCkwpaudi_FlagFPDoneOk != \
        (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagFPDoneOk))
      {
         t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02] = d_CCkwpbase_NRCConditionsNotCorrect;
    }
    else
      {
        if (d_CCkwpbase_SIDDiagSessionControl == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00])
        {
          t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02] = d_CCkwpbase_NRCConditionsNotCorrect;
      }

    }

    if (d_CCkwpbase_SIDStartRoutineByLid == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00])
    {
      /* [OPL332_335] */

      if( (UBYTE)0x04u > t_InBuffer_PBUF[d_CCglo_DLC] )
    {
      t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat;
    }

    else
    {

        if ((UBYTE)0x01u != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) /* [OPL339] */
        {
          t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCSubFunctionNotSupported;
        }

      }
    }
    /*========================================================================*/
    /* -2- RequestDownload after successfull RequestDownload                  */
    /*========================================================================*/
    if (((t_CCkwpaudi_KwpCtrl_ST.State_UW &
          d_CCkwpaudi_FlagRequestDownloadActive) == d_CCkwpaudi_FlagRequestDownloadActive) &&
        (d_CCkwpbase_SIDRequestDownload  == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00]))
    {
      t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02] = d_CCkwpbase_NRCConditionsNotCorrect;
    }
  } /* -1- END OF if(..) -----------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* set return value - we are ready, send the response                       */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_SequenceError_UB' -+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - all                                                    */
/*  LOCAL ID   : $xx - all                                                    */
/*  CMD        : $xx - all                                                    */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTSequenceError_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Request Sequence Error                                        */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_ServiceNotSupported_UB                        |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Standard Service Function for all "Service Not Supported    |
|                  Errors", if no especially response code or postcondition    |
|                  is required by the customer.                                |
|                                                                              |
|                  Service ID: $xx -  ServiceNotSupported                      |
|                  Local ID  :  -                                              |
|                  Command ID:  -                                              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    RESPONSE INDEX                |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ServiceNotSupported_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                             PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /* Note: A KWP2000 negative response has got 3 byte!                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC]                 = d_CCglo_03Byte;

  /*--------------------------------------------------------------------------*/
  /* Send the correct negative Response                                       */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00]  = d_CCkwpbase_SIDNegativResponseID;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01]  = t_InBuffer_PBUF[d_CCglo_StartOfData];
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCServiceNotSupported;

  /*--------------------------------------------------------------------------*/
  /* check the addressing mode for functional addressing                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

  /*==========================================================================*/
  /* -1- service is functionally addressed                                    */
  /*==========================================================================*/
  if ( d_CCglo_Ready == t_Result_UB )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- service is not functionally addressed                                */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 0                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_ServiceNotSupported_UB' -+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - Service Not Supported                                  */
/*  LOCAL ID   :  -                                                           */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length $00 indicates, that the response should be taken    */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTServiceNotSupported_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Request Service not supported                                 */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_ServiceNotAllowed_UB                          |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Standard Service Function for all "Function not supported   |
|                  in active sessioin", if no especially response code or      |
|                  postcondition is required by the customer.                  |
|                                                                              |
|                  Service ID: $xx -  each SID                                 |
|                  Local ID  : $xx -  each LID                                 |
|                  Command ID:  -                                              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    RESPONSE INDEX                |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ServiceNotAllowed_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                           PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /* Note: A KWP2000 negative response has got 3 byte!                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC]                 = d_CCglo_03Byte;
  /*--------------------------------------------------------------------------*/
  /* Send the correct negative Response                                       */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00]  = d_CCkwpbase_SIDNegativResponseID;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01]  = t_InBuffer_PBUF[d_CCglo_StartOfData];
  /* [OPL329_330] */
  if((d_CCkwpbase_SIDDiagSessionControl == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00]) &&
     ((d_CCkwpbase_LIDProgSession     == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) || \
      (d_CCkwpbase_LIDExtendedSession == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) ))
  {
     t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRC_SubSrvNSInActSession;
  }
  else
  {
     if(d_CCkwpbase_SIDStartRoutineByLid == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00])
     {
       if (t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB ==  (UBYTE)0x01u) /* [OPL333_334] */
       {
         /* [AUDItest ID229] */
         t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRC_SrvNSInActSession;
       }
       else
       {
         if ((UBYTE)0x04u != t_InBuffer_PBUF[d_CCglo_DLC])
         {
           t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat;
         }
         else
         {
           if (((UBYTE)0x01u != t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01]) && \
                (t_InBuffer_PBUF[d_CCglo_DLC] > (UBYTE)0x01u)) /* [OPL339] */
           {
             t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCSubFunctionNotSupported;
           }
           else
           {
             t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCRequestOutOfRange;
           }
         }
       }
     }
     else /* [OPL337_338] */
     {
       if((d_CCkwpbase_SIDCommunicationControl == t_InBuffer_PBUF[d_CCglo_StartOfData + 0x00]) &&
          ((UBYTE)0x01u < t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02]))
       {
         t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCRequestOutOfRange;
       }
       else
       {
         t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRC_SrvNSInActSession;
       }
     }
  }
  /*--------------------------------------------------------------------------*/
  /* check the addressing mode for functional addressing                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

  /*==========================================================================*/
  /* -1- service is functionally addressed                                    */
  /*==========================================================================*/
  if ( d_CCglo_Ready == t_Result_UB )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- service is not funtionally addressed                                 */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 0                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_ServiceNotAllowed_UB'       -+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E   ( RCT )                           */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - each SID                                               */
/*  LOCAL ID   : $xx - Service not allowed in actual session                  */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length $00 indicates, that the response should be taken    */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTServiceNotAllowed_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Request Service not allowed                                   */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_AddressingNotAllowed_UB                       |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Standard Service Function for all "Addressing method        |
|                  not supported", if no especially response code or           |
|                  postcondition is required by the customer.                  |
|                                                                              |
|                  Service ID: $xx -  each SID                                 |
|                  Local ID  : $xx -  each LID                                 |
|                  Command ID:  -                                              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    RESPONSE INDEX                |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_AddressingNotAllowed_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                              PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /* The next line is only to prevent a compiler warning!!                    */
  t_OutBuffer_PBUF[0] |= 0x00;
  t_InBuffer_PBUF[0] |= 0x00;

  /*--------------------------------------------------------------------------*/
  /* Note: when a service shall be physically addressed, but is functionally  */
  /*       addressed, the service shall be ignored.                           */
  /*                                                                          */
  /*       when a service shall be functionally addressed, but is physically  */
  /*       addressed, the service shall be performed as usual (then this      */
  /*       function may not be called -> error in the service request table)  */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* set return value                                                         */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Idle | d_CCglo_ResponseIndex00);

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_AddressingNotAllowed_UB'    -+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E   ( RCT )                           */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - each SID                                               */
/*  LOCAL ID   : $xx - each LID                                               */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length $00 indicates, that the response should be taken    */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTAddressingNotAllowed_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - no response                                                   */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_SubfunctionNotSupported_UB                    |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Standard Service Function for all "Subfunction Not          |
|                  Supported Errors", if no especially response code or        |
|                  postcondition is required by the customer.                  |
|                                                                              |
|                  Service ID: $xx -  each SID                                 |
|                  Local ID  : $xx -  LID not supported                        |
|                  Command ID:  -                                              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    RESPONSE INDEX                |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_SubfunctionNotSupported_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                 PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Set the DLC for the OutBuffer                                            */
  /* Note: A KWP2000 negative response has got 3 byte!                        */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_DLC]                 = d_CCglo_03Byte;
  /*--------------------------------------------------------------------------*/
  /* Send the correct negative Response                                       */
  /*--------------------------------------------------------------------------*/
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x00]  = d_CCkwpbase_SIDNegativResponseID;
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x01]  = t_InBuffer_PBUF[d_CCglo_StartOfData];
  t_OutBuffer_PBUF[d_CCglo_StartOfData + 0x02]  = d_CCkwpbase_NRCSubFunctionNotSupported;

  /*--------------------------------------------------------------------------*/
  /* check the addressing mode for functional addressing                      */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

  /*==========================================================================*/
  /* -1- service is functionally addressed                                    */
  /*==========================================================================*/
  if ( d_CCglo_Ready == t_Result_UB )
  {
    /*------------------------------------------------------------------------*/
    /* set return value - will be overwritten by an error or other state      */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    /*------------------------------------------------------------------------*/
    /* Set the DLC of the OutBuffer to zero                                   */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- service is not funtionally addressed                                 */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 0                                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_SubfunctionNotSupported_UB' -+-+-+-+-+-*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E   ( RCT )                           */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $xx - each SID                                               */
/*  LOCAL ID   : $xx - LID not supported                                      */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length $00 indicates, that the response should be taken    */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Request Subfunction not supported                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCseqaudi_NoPostCondDefined,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_00Byte
}; /*--- END OF TABLE --------------------------------------------------------*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_LogicalAddressCheck_UW                        |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Routine for address check of download or erase addresses    |
|                                                                              |
|  Parameter:      ULONG Startaddress .. start address given by a request      |
|                  ULONG Endaddress   .. end address given by a request        |
|                  UBYTE Handle       .. Erase or Download                     |
|                                                                              |
|  Return Value:   UWORD t_Result_UB  ..                                       |
|                             0000 - AddressCheckNotOK                         |
|                            (0001 .. AddressCheckOK) v (xxxx .. MemoryState)  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UWORD FUN_CCkwpaudi_LogicalAddressCheck_UW ( ULONG t_StartAdress_UL, \
                                             ULONG t_EndAddress_UL,  \
                                             UBYTE t_Handle_UB       )
{
  UWORD t_Result_UW;
  UBYTE t_ScanState_UB;
  UBYTE t_Index_UB;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Result_UW    = d_CCglo_AddressCheckNOK;
  t_ScanState_UB = (UBYTE) d_CCkwpaudi_ScanRun;
  t_Index_UB     = (UBYTE) 0x00;

  /*==========================================================================*/
  /* -1- Check the complete Table                                             */
  /*==========================================================================*/
  while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
          ( d_CCglo_LastEntry !=                 \
           (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW )))
  {
    /*========================================================================*/
    /* -2- Check address range                                                */
    /*========================================================================*/
    if ( d_CCglo_CheckRange == \
         (d_CCglo_CheckStateMask & t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW ))
    {
      /*======================================================================*/
      /* -3- Address range is correct                                         */
      /*======================================================================*/
      if ( (t_StartAdress_UL >= \
            t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL ) && \
           (t_StartAdress_UL <= \
            t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL   ) && \
           (t_EndAddress_UL  >= \
            t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL ) && \
           (t_EndAddress_UL   <= \
            t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL   )    )
      {
        /* Stop scan                                                        */
        t_ScanState_UB   = d_CCkwpaudi_ScanStop;
        t_Result_UW      = d_CCglo_AddressCheckOK;
      } /* -3- END OF if(..) -----------------------------------------------*/

      /*====================================================================*/
      /* -3- Address range is not correct                                   */
      /*====================================================================*/
      else
      {
        /* Check next entry                                                 */
        t_Index_UB += 0x01;
      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* -2- END OF if(..) -------------------------------------------------*/

    /*======================================================================*/
    /* -2- Check address Boundary                                           */
    /*======================================================================*/
    else
    {
      /*====================================================================*/
      /* -3- Address boundary is correct                                    */
      /*====================================================================*/
      if ( (t_StartAdress_UL == \
             t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL ) && \
            (t_EndAddress_UL   == \
             t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL   )    )
      {
        /* Stop scan                                                        */
        t_ScanState_UB  = d_CCkwpaudi_ScanStop;
        t_Result_UW     = d_CCglo_AddressCheckOK;
      } /* -3- END OF if(..) -----------------------------------------------*/

      /*====================================================================*/
      /* -3- Address range is not correct                                   */
      /*====================================================================*/
      else
      {
        /* Check next entry                                                 */
        t_Index_UB += 0x01;

      } /* -3- END OF else (if(..)) ----------------------------------------*/
    } /* ---2- END OF else (if(..)) ----------------------------------------*/
  } /* -----1- END OF while (..) -------------------------------------------*/


  /*==========================================================================*/
  /* -1- AddressCheck is OK - check next condition                            */
  /*==========================================================================*/
  if (d_CCglo_AddressCheckOK == t_Result_UW)
  {
    /* Fill in the memory info                                                */
    t_Result_UW  |= t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW;

    /*========================================================================*/
    /* -2- Address range error                                                */
    /*     EndAddress < StartAddress                                          */
    /*========================================================================*/
    if ( t_EndAddress_UL < t_StartAdress_UL )
    {
      /* set result                                                           */
      t_Result_UW = (UWORD) (d_CCglo_AddressCheckNOK | d_CCglo_EndaddressIncorrect);
    } /* -2- END OF if(..) ---------------------------------------------------*/


    /*========================================================================*/
    /* -2- Erasing not allowed                                                */
    /*     CheckCondition: (Handle == Erase) ^ (State == ErasingNotAllowed)   */
    /*========================================================================*/
    if ( ( t_Handle_UB          ==  d_CCglo_Erase                        ) && \
         ( d_CCglo_NotErasable  == (d_CCglo_EraseStateMask & t_Result_UW))    )
    {
      /* set result                                                           */
      t_Result_UW = (UWORD) (d_CCglo_AddressCheckNOK | d_CCglo_ProtectedEraseArea);
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- Programing not allowed                                             */
    /*     CheckCondition: (Handle == Erase) ^ (State == ErasingNotAllowed)   */
    /*========================================================================*/
    if ( ( t_Handle_UB             ==  d_CCglo_Download                       ) && \
         ( d_CCglo_NotProgramable  == (d_CCglo_ProgramStateMask & t_Result_UW))    )
    {
      /* set result                                                           */
      t_Result_UW = (UWORD) (d_CCglo_AddressCheckNOK | d_CCglo_ProtectedDownloadArea);
    } /* -2- END OF if(..) ---------------------------------------------------*/
  } /* ---1- END OF if(..) ---------------------------------------------------*/

  return t_Result_UW;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_LogicalAddressCheck_UW' +-+-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_ResolveMemoryMapIndex_UB                      |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Routine for address check of download or erase addresses    |
|                                                                              |
|  Parameter:      ULONG *Startaddress .. corresponding start address          |
|                  ULONG *Endaddress   .. corresponding end address            |
|                  UWORD MemoryIndex   .. memory index to be looked up         |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E x  x x x x                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ResolveMemoryMapIndex_UB ( ULONG *t_StartAddress_UL, \
                                               ULONG *t_EndAddress_UL,  \
                                               UWORD t_MemoryIndex_UW  )
{
  UBYTE t_Result_UB;
  UBYTE t_ScanState_UB;
  UBYTE t_Index_UB;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Result_UB    = d_CCglo_Error;
  t_ScanState_UB = d_CCkwpaudi_ScanRun;
  t_Index_UB     = 0x00;

  /*==========================================================================*/
  /* -1- Check the complete Table                                             */
  /*==========================================================================*/
  while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
          ( d_CCglo_LastEntry !=                 \
           (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW )))
  {
    /*========================================================================*/
    /* -2- Memory index found                                                 */
    /*========================================================================*/
    if ( t_MemoryIndex_UW ==  t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryIndex_UW )
    {
      /*----------------------------------------------------------------------*/
      /* stop scan                                                            */
      /*----------------------------------------------------------------------*/
      t_ScanState_UB   = d_CCkwpaudi_ScanStop;
      t_Result_UB      = d_CCglo_Ready;

      /*----------------------------------------------------------------------*/
      /* write start address of memory entry                                  */
      /*----------------------------------------------------------------------*/
      *t_StartAddress_UL = t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL;

      /*----------------------------------------------------------------------*/
      /* write end address of memory entry                                    */
      /*----------------------------------------------------------------------*/
      *t_EndAddress_UL = t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL;

    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- Memory index not found                                             */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* Check next entry                                                     */
      /*----------------------------------------------------------------------*/
      t_Index_UB += 0x01;
    } /* ---2- END OF else (if(..)) ------------------------------------------*/
  } /* -----1- END OF while (..) ---------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_ResolveMemoryMapIndex_UB' +-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_ResolveValidationMapIndex_UB                  |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Routine for resolving an index in the validation map        |
|                                                                              |
|  Parameter:      ULONG *Startaddress .. corresponding start address          |
|                  ULONG *Endaddress   .. corresponding end address            |
|                  UWORD MemoryIndex   .. memory index to be looked up         |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB  .. table index                          |
|                                                                              |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ResolveValidationMapIndex_UB ( UWORD t_MemoryIndex_UW  )
{
  UBYTE t_Result_UB;
  UBYTE t_ScanState_UB;
  UBYTE t_Index_UB;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Result_UB    = d_CCglo_NoEntry;
  t_ScanState_UB = d_CCkwpaudi_ScanRun;
  t_Index_UB     = 0x00;

  /*==========================================================================*/
  /* -1- Check the complete Table                                             */
  /*==========================================================================*/
  while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
          ( d_CCglo_LastEntry !=                 \
           (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.ValidationMap_PST[t_Index_UB].CheckCondition_UW )))
  {
    /*========================================================================*/
    /* -2- Memory index found                                                 */
    /*========================================================================*/
    if ( t_MemoryIndex_UW == t_CCkwpaudi_KwpCtrl_ST.ValidationMap_PST[t_Index_UB].MemoryIndex_UW )
    {
      /*----------------------------------------------------------------------*/
      /* stop scan                                                            */
      /*----------------------------------------------------------------------*/
      t_ScanState_UB   = d_CCkwpaudi_ScanStop;
      t_Result_UB      = t_Index_UB;

    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- Memory index not found                                             */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* Check next entry                                                     */
      /*----------------------------------------------------------------------*/
      t_Index_UB += 0x01;
    } /* ---2- END OF else (if(..)) ------------------------------------------*/
  } /* -----1- END OF while (..) ---------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_ResolveValidationMapIndex_UB' +-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCkwpaudi_AddressToMemoryMapIndex_UB                    |
|                                                                              |
|  Prototype at:   ADLATUS_KWP2000AUDI_tdf.h                                   |
|                                                                              |
|  Description:    Routine gives out the index of the downloadMap entry for    |
|                  the requested address range                                 |
|                                                                              |
|  Parameter:      ULONG Startaddress  .. start address                        |
|                  ULONG Endaddress    .. end address                          |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB : index of the downloadMap entry         |
|                                       d_CCglo_NoEntry if nothing found       |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_AddressToMemoryMapIndex_UB ( ULONG t_StartAddress_UL, \
                                                 ULONG t_EndAddress_UL )
{
  UBYTE t_Result_UB;
  UBYTE t_ScanState_UB;
  UBYTE t_Index_UB;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Result_UB    = d_CCglo_NoEntry;
  t_ScanState_UB = d_CCkwpaudi_ScanRun;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables, do not check the adlatus                        */
  /*--------------------------------------------------------------------------*/
  t_Index_UB     = 0x01;

  /*==========================================================================*/
  /* -1- Check the complete Table                                             */
  /*==========================================================================*/
  while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
          ( d_CCglo_LastEntry !=                 \
           (d_CCglo_EntryMask & t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].MemoryInfo_UW )))
  {
    /*========================================================================*/
    /* -2- Address range is correct                                           */
    /*========================================================================*/
    if ( (t_StartAddress_UL >= \
          t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL ) && \
         (t_StartAddress_UL <= \
          t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL   ) && \
         (t_EndAddress_UL  >= \
          t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].StartAddress_UL ) && \
         (t_EndAddress_UL   <= \
          t_CCkwpaudi_KwpCtrl_ST.DownloadMap_PST[t_Index_UB].EndAddress_UL   )    )
    {
      /* Stop scan                                                            */
      t_ScanState_UB   = d_CCkwpaudi_ScanStop;
      t_Result_UB      = t_Index_UB;
    } /* ---2- END OF if(..) -------------------------------------------------*/

    /*========================================================================*/
    /* -2- Address range is not correct                                       */
    /*========================================================================*/
    else
    {
      /* Check next entry                                                     */
      t_Index_UB += 0x01;
    } /* ---3- END OF if(..) -------------------------------------------------*/
  } /* -----1- END OF while (..) ---------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCkwpaudi_AddressToMemoryMapIndex_UB' +-+-+-+-+-+*/

typedef struct _READDATABYID_ST
{
  UBYTE  t_TableIndexs_UB;
  UBYTE  t_ActDLCs_UB;
  UBYTE  t_Datalength_UB;

} READDATABYID_ST;

void FUN_CCkwpaudi_CopyEntry_V(READDATABYID_ST* ReadDataById_ST, PTR_BUFFER_UB t_OutBuffer_PBUF);

/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_ReadDataById_UB                             |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    Service Function for the KWP2000 Service:                 |
|                                                                            |
|                  Service ID: $22 - ReadDataByIdentifier                    |
|                  Local ID  :  -                                            |
|                  Command ID:  -                                            |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Input buffer     |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Output buffer    |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ReadDataById_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                      PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE  t_Result_UB;
  UBYTE  t_NbrIDBytes_UB;
  UBYTE  t_IDIndex_UB;
  UBYTE  t_ScanState_UB;
  UBYTE  t_TableIndex2_UB;
  UBYTE  t_ArrayIndex_UB;
  UBYTE  t_EEPROMIndex_UB;
  UNION2 t_ActID_U2;
  UBYTE  t_ActDLCLocal_UB;
  UBYTE  t_TableIndex_UB;
  UBYTE  t_ActDLC_UB;
  UBYTE  t_counter_UB;
  UBYTE  t_ResultEval_UB;
  BOOL   t_DataWritten_B;
  UBYTE  t_CorrectEntriesFound_UB = 0x00;

  READDATABYID_ST ReadDataById_ST;

  /*==========================================================================*/
  /* -1- Function was called a first time                                     */
  /*==========================================================================*/
  if ( (t_CCkwpaudi_KwpCtrl_ST.State_UW & d_CCkwpaudi_FlagRoutineBusy) != \
    d_CCkwpaudi_FlagRoutineBusy )
  {
    /*------------------------------------------------------------------------*/
    /* check the addressing mode for functional addressing                    */
    /*------------------------------------------------------------------------*/
    t_Result_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                   (t_InBuffer_PBUF, d_CCglo_FuncAddressAllowed);

    /*========================================================================*/
    /* -2- service is functionally addressed                                  */
    /*========================================================================*/
    if ( d_CCglo_Ready == t_Result_UB )
    {
      /*----------------------------------------------------------------------*/
      /* set return value - will be overwritten by an error or other state    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex05);

      /*----------------------------------------------------------------------*/
      /* Set the DLC of the OutBuffer to zero                                 */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = d_CCglo_00Byte;

    } /* -------------------2- END OF if(..) ---------------------------------*/
    /*========================================================================*/
    /* -2- service is not funtionally addressed                               */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* set return value - response index 1                                  */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    } /* -------------------2- END OF else (if(..)) --------------------------*/

    /*========================================================================*/
    /* -2- odd number of recordDataIdentifer bytes (even dlc with SID)        */
    /*========================================================================*/
    if( 0x00 == ( 0x01 & ( t_InBuffer_PBUF[d_CCglo_DLC] ) ) )
    {
      /*----------------------------------------------------------------------*/
      /* Send the negativ response (incorrectMessageLenth)                    */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);

    } /*--------------------2- END OF if(..) ---------------------------------*/

    /*========================================================================*/
    /* -2- even number of recordDataIdentifer bytes                           */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* set number of recordDataIdentifier bytes ( 2 bytes == 1 Identifier ) */
      /*----------------------------------------------------------------------*/
      t_NbrIDBytes_UB = t_InBuffer_PBUF[d_CCglo_DLC] - 1;

      /*======================================================================*/
      /* -3- number of recordDataIdentifier is wrong                          */
      /*======================================================================*/
      if( ( 0 == t_NbrIDBytes_UB ) ||    \
          ( c_CCconprj_MaxNbrRecordDataIDs_UB < ( t_NbrIDBytes_UB >> 1 ) ) )
      {
        /*--------------------------------------------------------------------*/
        /* Send the negativ response (incorrectMessageLenth)                  */
        /*--------------------------------------------------------------------*/
        t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex04);


      } /*------------------3- END OF if(..) ---------------------------------*/

      /*======================================================================*/
      /* -3- number of recordDataIdentifier is right                          */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* do a prescan of the record data identifiers; only if a known       */
        /* identifier was found a response pending may be sent.               */
        /* Otherwise a requestOutOfRange must be sent without a response      */
        /* pending.                                                           */
        /*--------------------------------------------------------------------*/

        /*====================================================================*/
        /* -4- do for all recordDataIdentifiers                               */
        /*====================================================================*/
        for ( t_IDIndex_UB = 0; t_IDIndex_UB < t_NbrIDBytes_UB; t_IDIndex_UB += 2 )
        {
          /*------------------------------------------------------------------*/
          /* get the actual recordDataIdentifier                              */
          /*------------------------------------------------------------------*/
          t_ActID_U2.UByte_ST.UByte0_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01 + t_IDIndex_UB];
          t_ActID_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02 + t_IDIndex_UB];


          /*==================================================================*/
          /* -5- the fingerprint is read out                                  */
          /*==================================================================*/
          if ( (c_CCconprj_ReadFingerprint_UW == t_ActID_U2.UWord_ST.UWord0_UW) )
          {
            /*----------------------------------------------------------------*/
            /* set marker that a known entry was found                        */
            /*----------------------------------------------------------------*/
            t_CorrectEntriesFound_UB |= 0xFF;

          } /* -------------5- END OF if(..) ---------------------------------*/
          /*==================================================================*/
          /* -5- an other record data identifier is read out                  */
          /*==================================================================*/
          else
          {

            /*----------------------------------------------------------------*/
            /* init scan variables                                            */
            /*----------------------------------------------------------------*/
            t_ScanState_UB  = (UBYTE) d_CCkwpaudi_ScanRun;
            t_TableIndex_UB = (UBYTE) 0x00u;


            /*================================================================*/
            /* -6- search the recordDataID entry in the table                 */
            /*================================================================*/
            while ( ( d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
                ( d_CCglo_NoEntry !=                 \
                ( c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].NvmEntryOrLenth_UB )))
            {
              /*==============================================================*/
              /* -7- it is the right entry                                    */
              /*==============================================================*/
              if ( c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].Identifier_UW ==  \
                  t_ActID_U2.UWord_ST.UWord0_UW )
              {
                /* Stop scan                                                  */
                t_ScanState_UB   = d_CCkwpaudi_ScanStop;

                /*------------------------------------------------------------*/
                /* set marker that a known entry was found                    */
                /*------------------------------------------------------------*/
                t_CorrectEntriesFound_UB |= (UBYTE) 0xFFu;

              } /*----------7- END OF if(..) ---------------------------------*/

              /*==============================================================*/
              /* -7- it is the wrong entry                                    */
              /*==============================================================*/
              else
              {
                /* Check next entry                                           */
                t_TableIndex_UB += 0x01;

              } /*----------7- END OF else (if(..)) --------------------------*/
            } /*------------6- END OF while(..) ------------------------------*/
          } /* -------------5- END OF else(..) -------------------------------*/
        } /* ---------------4- END OF for(..) --------------------------------*/

        /*====================================================================*/
        /* -4- a valid record data identifer had been found                   */
        /*====================================================================*/
        if ( (UBYTE) 0x00u != t_CorrectEntriesFound_UB)
        {

          /*------------------------------------------------------------------*/
          /* Set the internal busy flag                                       */
          /*------------------------------------------------------------------*/
          t_CCkwpaudi_KwpCtrl_ST.State_UW |=  d_CCkwpaudi_FlagRoutineBusy;

          /*------------------------------------------------------------------*/
          /* response pending                                                 */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_Busy | d_CCglo_ResponseIndex06);

        } /* ---------------4- END OF if(..) ---------------------------------*/

        /*====================================================================*/
        /* -4- no valid record data identifer had been found                  */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* Send the negativ response (request out of range)                 */
          /*------------------------------------------------------------------*/

        } /* ---------------4- END OF else(..) -------------------------------*/
      } /*------------------3- END OF else (if(..)) --------------------------*/
    } /*--------------------2- END OF else (if(..)) --------------------------*/
  } /* ---------------------1- END OF if(..) ---------------------------------*/

  /*==========================================================================*/
  /* -1- Function was called one more time                                    */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* set return value - response index 1 as default                         */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    /*------------------------------------------------------------------------*/
    /* Reset the internal busy flag                                           */
    /*------------------------------------------------------------------------*/
    t_CCkwpaudi_KwpCtrl_ST.State_UW &= ~d_CCkwpaudi_FlagRoutineBusy;

    /*------------------------------------------------------------------------*/
    /* init variable                                                          */
    /*------------------------------------------------------------------------*/
    t_EEPROMIndex_UB = 0x00;

    /*------------------------------------------------------------------------*/
    /* set number of recordDataIdentifier bytes ( 2 bytes == 1 Identifier )   */
    /*------------------------------------------------------------------------*/
    t_NbrIDBytes_UB = t_InBuffer_PBUF[d_CCglo_DLC] - 1;

    /*------------------------------------------------------------------------*/
    /* write positiv response SID                                             */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_StartOfData]  =   \
                                       (d_CCkwpbase_SIDReadDataByCommonId + (UBYTE) 0x40u);
    t_ActDLC_UB = 0x01u;

    /*========================================================================*/
    /* -2- do for all recordDataIdentifiers                                   */
    /*========================================================================*/
    for ( t_IDIndex_UB = 0; t_IDIndex_UB < t_NbrIDBytes_UB; t_IDIndex_UB += 2 )
    {
      /*----------------------------------------------------------------------*/
      /* get the actual recordDataIdentifier                                  */
      /*----------------------------------------------------------------------*/
      t_ActID_U2.UByte_ST.UByte0_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x01 + t_IDIndex_UB];
      t_ActID_U2.UByte_ST.UByte1_UB = t_InBuffer_PBUF[d_CCglo_StartOfData + 0x02 + t_IDIndex_UB];

      /*----------------------------------------------------------------------*/
      /* init scan variables                                                  */
      /*----------------------------------------------------------------------*/
      t_ScanState_UB  = (UBYTE) d_CCkwpaudi_ScanRun;
      t_TableIndex_UB = (UBYTE) 0x00u;
      t_DataWritten_B = FALSE;

      /*======================================================================*/
      /* -3- the recordDataIdentifier is ReadFingerprint                      */
      /*======================================================================*/
      if (c_CCconprj_ReadFingerprint_UW == t_ActID_U2.UWord_ST.UWord0_UW)
      {
        /*--------------------------------------------------------------------*/
        /* init the local DLC                                                 */
        /*--------------------------------------------------------------------*/
        t_ActDLCLocal_UB = 2;

        /*====================================================================*/
        /* -4- search downloadmap entry in the table                          */
        /*====================================================================*/
        while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
                (d_CCglo_NoEntry != c_CCconprj_LogBlockEEPROM_AUB[t_TableIndex_UB][0] ))
        {
          /*==================================================================*/
          /* -5- the outBuffer is to small                                    */
          /*==================================================================*/
          if ( t_OutBuffer_PBUF[d_CCglo_MaxBufferLength] < (c_CCconprj_LengthFingerprint_UB + \
               t_ActDLCLocal_UB + t_ActDLC_UB ))
          {
            /*----------------------------------------------------------------*/
            /* error, stop scanning downloadmap                               */
            /*----------------------------------------------------------------*/
            t_ScanState_UB = d_CCkwpaudi_ScanStop;

          } /*--------------5- END OF if(..) ---------------------------------*/
          /*==================================================================*/
          /* -5- the outBuffer is big enough                                  */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /* -6- for every part of the fingerprint                          */
            /*================================================================*/
            for ( t_counter_UB = 0;  \
                  t_counter_UB < d_CCkwpaudi_NbrFingerprintHandles;  \
                  t_counter_UB++)
            {
              /*--------------------------------------------------------------*/
              /* find the entry in EEPROM table                               */
              /*--------------------------------------------------------------*/
              t_EEPROMIndex_UB = FUN_CCnvm_ScanNvmMemoryTable_UB \
                (c_CCconprj_LogBlockEEPROM_AUB[t_TableIndex_UB][t_counter_UB]);

              /*==============================================================*/
              /* -7- no EEPROM entry found                                    */
              /*==============================================================*/
              if ( d_CCglo_NoEntry == t_EEPROMIndex_UB )
              {
                /*------------------------------------------------------------*/
                /* error, stop scanning downloadmap                           */
                /*------------------------------------------------------------*/
                t_ScanState_UB = d_CCkwpaudi_ScanStop;

              } /*----------7- END OF if(..) ---------------------------------*/
              /*==============================================================*/
              /* -7- correct EEPROM entry found                               */
              /*==============================================================*/
              else
              {
                /*------------------------------------------------------------*/
                /* error, stop scanning downloadmap                           */
                /*------------------------------------------------------------*/
                t_ResultEval_UB = FUN_CCnvm_NvmAccess_UB \
                   (c_CCconprj_LogBlockEEPROM_AUB[t_TableIndex_UB][t_counter_UB],  \
                   (PTR_BUFFER_UB)&t_OutBuffer_PBUF[d_CCglo_StartOfData +  \
                    t_ActDLC_UB + t_ActDLCLocal_UB] );


                /*============================================================*/
                /* -8- EEPROM access was not successful                       */
                /*============================================================*/
                if (d_CCglo_NoError != t_ResultEval_UB)
                {
                  /*----------------------------------------------------------*/
                  /* error, stop scanning downloadmap                         */
                  /*----------------------------------------------------------*/
                  t_ScanState_UB = d_CCkwpaudi_ScanStop;

                } /*--------8- END OF if(..) ---------------------------------*/
                /*============================================================*/
                /* -8- EEPROM access was successful                           */
                /*============================================================*/
                else
                {
                  /*----------------------------------------------------------*/
                  /* actualize the DLC                                        */
                  /*----------------------------------------------------------*/
                  t_ActDLCLocal_UB += (UBYTE)c_CCconprj_EEPROM_AST  \
                                          [t_EEPROMIndex_UB].DataLength_UW;

                  /*----------------------------------------------------------*/
                  /* set flag that data has been written                      */
                  /*----------------------------------------------------------*/
                  t_DataWritten_B = (BOOL) ~FALSE;

                } /*--------8- END OF else (..) ------------------------------*/
              } /*----------7- END OF else (..) ------------------------------*/
            } /*------------6- END OF for(..) --------------------------------*/
          } /*--------------5- END OF else(..) -------------------------------*/

          /*------------------------------------------------------------------*/
          /* Check next entry                                                 */
          /*------------------------------------------------------------------*/
          t_TableIndex_UB += 0x01;

        } /*----------------4- END OF while(..) ------------------------------*/

        /*====================================================================*/
        /* -4- the downloadmap had been completely processed without          */
        /*     errors and at least one fingerprint had been written           */
        /*====================================================================*/
        if (((UBYTE)~FALSE == t_DataWritten_B) && \
            (d_CCglo_NoEntry == c_CCconprj_LogBlockEEPROM_AUB[t_TableIndex_UB][0]))
        {
          /*------------------------------------------------------------------*/
          /* write the recordDataIdentifier into out buffer in front          */
          /* of the data                                                      */
          /*------------------------------------------------------------------*/
          t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB] =  \
            t_ActID_U2.UByte_ST.UByte0_UB;
          t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB + 1] =  \
            t_ActID_U2.UByte_ST.UByte1_UB;

          /*------------------------------------------------------------------*/
          /* actualize the DLC                                                */
          /*------------------------------------------------------------------*/
          t_ActDLC_UB += t_ActDLCLocal_UB;

          /*------------------------------------------------------------------*/
          /* set the positiv response                                         */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

          /*------------------------------------------------------------------*/
          /* reset the flag                                                   */
          /*------------------------------------------------------------------*/
          t_DataWritten_B = FALSE;

        } /*----------------4- END OF if(..) ---------------------------------*/
      } /*------------------3- END OF if(..) ---------------------------------*/

      /*======================================================================*/
      /* -3- it is a normal recordDataIdentifier                              */
      /*======================================================================*/
      else
      {
        /*====================================================================*/
        /* -4- search the recordDataID entry in the table                     */
        /*====================================================================*/
        while ( ( d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
            ( d_CCglo_NoEntry !=                 \
            ( c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].NvmEntryOrLenth_UB )))
        {
          /*==================================================================*/
          /* -5- it is the right entry                                        */
          /*==================================================================*/
          if ( c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].Identifier_UW ==  \
              t_ActID_U2.UWord_ST.UWord0_UW )
          {
            /* Stop scan                                                      */
            t_ScanState_UB   = d_CCkwpaudi_ScanStop;

          } /*--------------5- END OF if(..) ---------------------------------*/

          /*==================================================================*/
          /* -5- it is the wrong entry                                        */
          /*==================================================================*/
          else
          {
            /* Check next entry                                               */
            t_TableIndex_UB += 0x01;

          } /*--------------5- END OF else (..) ------------------------------*/
        } /*----------------4- END OF while(..) ------------------------------*/

        /*====================================================================*/
        /* -4- correct crosstable entry found                                 */
        /*====================================================================*/
        if ( d_CCglo_NoEntry !=   \
            c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].NvmEntryOrLenth_UB )
        {
          /*==================================================================*/
          /* -5- the data shall be read out of the flash                      */
          /*==================================================================*/
          if ( d_CCconprj_ReadFromFlash == (d_CCconprj_ReadFromFlash &  \
                      c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].Flags_UB) )
          {
            /*================================================================*/
            /* -6- the outBuffer is big enough                                */
            /*================================================================*/
            if ( t_OutBuffer_PBUF[d_CCglo_MaxBufferLength] >=  \
                      (UBYTE)((c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].  \
                                          NvmEntryOrLenth_UB) + t_ActDLC_UB +2))
            {

              /* Store length of entry */
              ReadDataById_ST.t_Datalength_UB = c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].NvmEntryOrLenth_UB;
              ReadDataById_ST.t_TableIndexs_UB = t_TableIndex_UB;
              ReadDataById_ST.t_ActDLCs_UB = t_ActDLC_UB;

              /* call copy function */
              FUN_CCkwpaudi_CopyEntry_V(&ReadDataById_ST, (PTR_BUFFER_UB)t_OutBuffer_PBUF);
              t_DataWritten_B = (BOOL) ~FALSE;

            } /*------------6- END OF if (..) --------------------------------*/

            /*================================================================*/
            /* -6- the outBuffer is to small                                  */
            /*================================================================*/
            else
            {
              /*--------------------------------------------------------------*/
              /* set negativ response                                         */
              /*--------------------------------------------------------------*/

            } /*------------6- END OF else (..) ------------------------------*/
          } /*--------------5- END OF if (..) --------------------------------*/

          /*==================================================================*/
          /* -5- the data shall be read out of the eeprom                     */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /* -6- check for indexed sw block entry                           */
            /*================================================================*/
            if( d_CCconprj_IndexAccess == (d_CCconprj_IndexAccess &   \
                  c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].Flags_UB) )
            {
              /*--------------------------------------------------------------*/
              /* Get index for table c_CCconprj_LogBlockEEPROM_AST            */
              /*--------------------------------------------------------------*/
              t_TableIndex2_UB = c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].NvmEntryOrLenth_UB;
              /* set array index to first element e.g. adlatus */
              t_ArrayIndex_UB = 0;

              /* save actual DLC for error rewinding */
              t_ActDLCLocal_UB = t_ActDLC_UB;

              t_ScanState_UB = d_CCkwpaudi_ScanRun ;
              /*--------------------------------------------------------------*/
              /* write the recordDataIdentifier into out buffer in front      */
              /* of the data                                                  */
              /*--------------------------------------------------------------*/
              t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB] =  \
                   t_ActID_U2.UByte_ST.UByte0_UB;
              t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB + 1] =  \
                   t_ActID_U2.UByte_ST.UByte1_UB;

              t_ActDLC_UB += 0x02;
              /*==============================================================*/
              /* -7- search the recordDataID entry in the table               */
              /*==============================================================*/
              while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB )  &&
                       ( d_CCglo_NoEntry != c_CCconprj_LogBlockEEPROM_AUB[t_ArrayIndex_UB][0] ))
              {
                /*------------------------------------------------------------*/
                /* find the entry in EEPROM table                             */
                /*------------------------------------------------------------*/
                t_EEPROMIndex_UB = FUN_CCnvm_ScanNvmMemoryTable_UB   \
                   (c_CCconprj_LogBlockEEPROM_AUB[t_ArrayIndex_UB][t_TableIndex2_UB]);

                /*============================================================*/
                /* -8- no EEPROM entry found                                  */
                /*============================================================*/
                if ( d_CCglo_NoEntry == t_EEPROMIndex_UB )
                {
                  /*----------------------------------------------------------*/
                  /* error, stop scanning downloadmap                         */
                  /*----------------------------------------------------------*/
                  t_ScanState_UB = d_CCkwpaudi_ScanStop;

                } /*--------8- END OF if(..) ---------------------------------*/
                /*============================================================*/
                /* -8- correct EEPROM entry found                             */
                /*============================================================*/
                else
                {
                  /*==========================================================*/
                  /* -9- the outBuffer is big enough                          */
                  /*==========================================================*/
                  if ( t_OutBuffer_PBUF[d_CCglo_MaxBufferLength] >=  \
                       ((UBYTE)c_CCconprj_EEPROM_AST[t_EEPROMIndex_UB].DataLength_UW + t_ActDLC_UB) )
                  {
                    /*========================================================*/
                    /* -A- the EEPROM access was not successful               */
                    /*========================================================*/
                    /* c_CCconprj_EEPROM_AST[].Address_UW */
                    if (d_CCglo_NoError != FUN_CCnvm_NvmAccess_UB (\
                        c_CCconprj_LogBlockEEPROM_AUB[t_ArrayIndex_UB][t_TableIndex2_UB], \
                        &t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB] ) )
                    {
                      /* if something went wrong rewind pointer */
                      t_ActDLC_UB = t_ActDLCLocal_UB ;

                    } /*----A- END OF if(..) ---------------------------------*/

                    /*========================================================*/
                    /* -A- the EEPROM access was successful                   */
                    /*========================================================*/
                    else
                    {
                      /*------------------------------------------------------*/
                      /* actualize the DLC                                    */
                      /*------------------------------------------------------*/
                      t_ActDLC_UB += (UBYTE)c_CCconprj_EEPROM_AST[t_EEPROMIndex_UB].DataLength_UW;

                      /*------------------------------------------------------*/
                      /* set the positiv response                             */
                      /*------------------------------------------------------*/
                      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

                    } /*----A- END OF else (..) ------------------------------*/
                  } /*------9- END OF if(..) ---------------------------------*/

                  /*==========================================================*/
                  /* -9- the outBuffer is to small                            */
                  /*==========================================================*/
                  else
                  {
                    /* if something went wrong rewind pointer */
                    t_ActDLC_UB = t_ActDLCLocal_UB ;

                  } /*------9- END OF else (..) ------------------------------*/
                } /*--------8- END OF else (..) ------------------------------*/

                /* next array element ( = next block) */
                t_ArrayIndex_UB++;

              } /* ---------7- END OF while(..) ------------------------------*/
            } /* -----------6- END OF if(..) ---------------------------------*/

            /*================================================================*/
            /* -6- normal entry (not for sw blocks                            */
            /*================================================================*/
            else
            {
              /*==============================================================*/
              /* -7- the recordDataIdentifier is ActiveDiagSession            */
              /*==============================================================*/
              if (d_CCconprj_ActiveDiagSession == t_ActID_U2.UWord_ST.UWord0_UW)
              {
                t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB] =  \
                      t_ActID_U2.UByte_ST.UByte0_UB;
                t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB + 1] =  \
                      t_ActID_U2.UByte_ST.UByte1_UB;
                /*------------------------------------------------------------*/
                /* add the 0x00 as end of the string                          */
                /*------------------------------------------------------------*/
                t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB + 2] = \
                      t_CCkwpaudi_KwpCtrl_ST.ActiveSession_UB;

                /*------------------------------------------------------------*/
                /* actualize the DLC                                          */
                /*------------------------------------------------------------*/
                t_ActDLC_UB +=3;

              } /*----------7- END OF if(..) ---------------------------------*/

              /*==============================================================*/
              /* -7- the recordDataIdentifier not ActiveDiagSession           */
              /*==============================================================*/
              else
              {
                /*------------------------------------------------------------*/
                /* find the entry in EEPROM table                             */
                /*------------------------------------------------------------*/
                t_EEPROMIndex_UB = FUN_CCnvm_ScanNvmMemoryTable_UB   \
                    (c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].NvmEntryOrLenth_UB);

                /*============================================================*/
                /* -8- correct EEPROM entry found                             */
                /*============================================================*/
                if ( d_CCglo_NoEntry != t_EEPROMIndex_UB )
                {
                  /*==========================================================*/
                  /* -9- the outBuffer is big enough                          */
                  /*==========================================================*/
                  if ( t_OutBuffer_PBUF[d_CCglo_MaxBufferLength] >= \
                          ((UBYTE)(c_CCconprj_EEPROM_AST[t_EEPROMIndex_UB].DataLength_UW \
                                               + 2u) + t_ActDLC_UB ))
                  {
                    /*========================================================*/
                    /* -A- the EEPROM access was not successful               */
                    /*========================================================*/
                    if (d_CCglo_NoError != FUN_CCnvm_NvmAccess_UB (c_CCconprj_ReadDataByID_AST  \
                        [t_TableIndex_UB].NvmEntryOrLenth_UB,   \
                        &t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB + 2] ) )
                    {
                      /*======================================================*/
                      /* -B- the alternative adresses shall be used           */
                      /*======================================================*/
                      if( d_CCconprj_UseAltAddress == (d_CCconprj_UseAltAddress &   \
                          c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].Flags_UB) )
                      {

                        /* store entry length */
                        ReadDataById_ST.t_Datalength_UB = (UBYTE) c_CCconprj_EEPROM_AST[t_EEPROMIndex_UB].DataLength_UW;
                        ReadDataById_ST.t_TableIndexs_UB = t_TableIndex_UB;
                        ReadDataById_ST.t_ActDLCs_UB = t_ActDLC_UB;

                        /* Copy data */
                        FUN_CCkwpaudi_CopyEntry_V(&ReadDataById_ST, (PTR_BUFFER_UB)t_OutBuffer_PBUF);
                        t_DataWritten_B = (BOOL) ~FALSE;

                      } /* -B- END OF if(..) ---------------------------------*/
                    } /* ---A- END OF if(..) ---------------------------------*/

                    /*========================================================*/
                    /* -A- the EEPROM access was successful                   */
                    /*========================================================*/
                    else
                    {
                      /*------------------------------------------------------*/
                      /* set flag that data has been written                  */
                      /*------------------------------------------------------*/
                      t_DataWritten_B = (BOOL) ~FALSE;

                    } /*----A- END OF else (if(..)) --------------------------*/
                  }  /*-----9- END OF if(..) ---------------------------------*/

                  /*==========================================================*/
                  /* -9- the outBuffer is to small                            */
                  /*==========================================================*/
                  else
                  {
                    /*--------------------------------------------------------*/
                    /* set negativ response                                   */
                    /*--------------------------------------------------------*/

                  } /*------9- END OF else (if (..)) -------------------------*/
                } /*--------8- END OF if (..) --------------------------------*/
              } /*----------7- END OF else (if (..)) -------------------------*/
            } /* -----------6- END OF else(..) -------------------------------*/
          } /* -------------5- END OF else(..) -------------------------------*/

          /*==================================================================*/
          /* -5- data has been written into the outbuffer                     */
          /*==================================================================*/
          if (FALSE != t_DataWritten_B )
          {
            /*----------------------------------------------------------------*/
            /* write the recordDataIdentifier into out buffer in front        */
            /* of the data                                                    */
            /*----------------------------------------------------------------*/
            t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB] =  \
                t_ActID_U2.UByte_ST.UByte0_UB;
            t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB + 1] =  \
                t_ActID_U2.UByte_ST.UByte1_UB;

            /*================================================================*/
            /* -6- the data had been read out of the flash                    */
            /*================================================================*/
            if ( d_CCconprj_ReadFromFlash == (d_CCconprj_ReadFromFlash &  \
                        c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].Flags_UB) )
            {
              /*--------------------------------------------------------------*/
              /* actualize the DLC                                            */
              /*--------------------------------------------------------------*/
              t_ActDLC_UB += (UBYTE)c_CCconprj_ReadDataByID_AST[t_TableIndex_UB].  \
                                          NvmEntryOrLenth_UB + 2;
            } /*------------6- END OF if (..) --------------------------------*/
            /*================================================================*/
            /* -6- the data had been read out of the eeprom                   */
            /*================================================================*/
            else
            {
              /*--------------------------------------------------------------*/
              /* actualize the DLC                                            */
              /*--------------------------------------------------------------*/
              t_ActDLC_UB += (UBYTE)c_CCconprj_EEPROM_AST[t_EEPROMIndex_UB].  \
                      DataLength_UW + 2;

            } /*------------6- END OF else (if (..)) -------------------------*/

            /*----------------------------------------------------------------*/
            /* post procession 'special' identifier                           */
            /*----------------------------------------------------------------*/
            /*================================================================*/
            /* -6- the recordDataIdentifier is ECU programming info           */
            /*================================================================*/
            if (c_CCconprj_ECUProgramInfo_UW == t_ActID_U2.UWord_ST.UWord0_UW)
            {
              /*==============================================================*/
              /* -7- the application software is invalid                      */
              /*==============================================================*/
              if(FALSE != FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_LOOKFOR_FSWInvalid_UB) )
              {
                /*------------------------------------------------------------*/
                /* set inconsistent bit in outBuffer                          */
                /*------------------------------------------------------------*/
                t_OutBuffer_PBUF[(t_ActDLC_UB + d_CCglo_StartOfData) - (UBYTE)1u]  \
                                             &= ~d_CCkwpaudi_AppInconsistentMask;
                t_OutBuffer_PBUF[(t_ActDLC_UB + d_CCglo_StartOfData) - (UBYTE)1u]  \
                                             |= d_CCkwpaudi_AppInconsistent;

              } /*----------7- END OF if (..) --------------------------------*/
            } /*------------8- END OF if (..) --------------------------------*/

            /*================================================================*/
            /* -6- the recordDataIdentifier is NOT ECU programming info       */
            /*================================================================*/
            else
            {
              /*==============================================================*/
              /* -7- the recordDataIdentifier is AsamOdxFileID                */
              /*==============================================================*/
              if (c_CCconprj_AsamOdxFileID_UW == t_ActID_U2.UWord_ST.UWord0_UW)
              {
                /*============================================================*/
                /* -8- the outBuffer is big enough                            */
                /*============================================================*/
                if ( t_OutBuffer_PBUF[d_CCglo_MaxBufferLength] >= (t_ActDLC_UB + (UBYTE)1u) )
                {
                  /*----------------------------------------------------------*/
                  /* add the 0x00 as end of the string                        */
                  /*----------------------------------------------------------*/
                  t_OutBuffer_PBUF[d_CCglo_StartOfData + t_ActDLC_UB] = 0x00;

                  /*----------------------------------------------------------*/
                  /* actualize the DLC                                        */
                  /*----------------------------------------------------------*/
                  t_ActDLC_UB++;

                } /*--------8- END OF if(..) ---------------------------------*/
                /*============================================================*/
                /* -8- the outBuffer too small                                */
                /*============================================================*/
                else
                {
                  /*----------------------------------------------------------*/
                  /* delete the whole record data identifer data              */
                  /*----------------------------------------------------------*/
                  t_ActDLC_UB -= ( (UBYTE)c_CCconprj_EEPROM_AST[t_EEPROMIndex_UB].  \
                  DataLength_UW + 3 );

                } /*--------8- END OF else (if(..)) --------------------------*/
              } /*----------7- END OF if(..) ---------------------------------*/
              /*==============================================================*/
              /* -7- the recordDataIdentifier is NOT AsamOdxFileID            */
              /*==============================================================*/
              else
              {
                /*------------------------------------------------------------*/
                /* do nothing                                                 */
                /*------------------------------------------------------------*/

              } /*----------7- END OF else (if(..)) --------------------------*/
            } /*------------6- END OF else (if(..)) --------------------------*/
          } /*--------------5- END OF if(..) ---------------------------------*/

          /*------------------------------------------------------------------*/
          /* set the positiv response                                         */
          /*------------------------------------------------------------------*/
          t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);

        } /*----------------4- END OF if(..) ---------------------------------*/

        /*--------------------------------------------------------------------*/
        /* check the system timer                                             */
        /*--------------------------------------------------------------------*/
        FUN_CCtim_SystemTimerCheck_V ();

      } /*------------------3- END OF else(..) -------------------------------*/
    } /*--------------------2- END OF for (..) -------------------------------*/

    /*========================================================================*/
    /* -2- data has been read out                                             */
    /*========================================================================*/
    if (t_ActDLC_UB > (UBYTE) 0x01u)
    {
      /*----------------------------------------------------------------------*/
      /* write DLC into outBuffer                                             */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = t_ActDLC_UB;

    } /* -------------------2- END OF if(..) ---------------------------------*/
    /*========================================================================*/
    /* -2- no data has been read out                                          */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* set the positiv response                                             */
      /*----------------------------------------------------------------------*/
      t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex01);

    } /* -------------------2- END OF if(..) ---------------------------------*/
  } /* ---------------------1- END OF else(..) -------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_ReadDataById_UB +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*  R E S P O N S E   C O D E   T A B L E  ( RCT )                            */
/* -------------------------------------------------------------------------- */
/*  SERVICE ID : $22 - ReadDataByIdentifier                                   */
/*  LOCAL ID   :  -                                                           */
/*  CMD        :  -                                                           */
/*                                                                            */
/*  NOTE: Response Length 0x00 indicates, that the response should be taken   */
/*        directly from outbuffer!                                            */
/* ========================================================================== */
const UBYTE c_CCkwpaudi_RCTReadDataById_AUB[] =
{
  /*--------------------------------------------------------------------------*/
  /* INDEX 00 - Positive Response                                             */
  /*--------------------------------------------------------------------------*/
  /* 00 - POST CONDITION   */  d_CCseqaudi_NoPostCondDefined,
  /* 01 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 01 - No recordDataID was supported                                 */
  /*--------------------------------------------------------------------------*/
  /* 02 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 03 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 04 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 05 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDReadDataByCommonId,
  /* 06 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestOutOfRange,
  /*--------------------------------------------------------------------------*/
  /* INDEX 02 - Illegal format of request                                     */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDReadDataByCommonId,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCConditionsNotCorrect,
  /*--------------------------------------------------------------------------*/
  /* INDEX 03 - Security access denied                                        */
  /*--------------------------------------------------------------------------*/
  /* 07 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 08 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 09 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 10 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDReadDataByCommonId,
  /* 11 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCSecurityAccessDenied,
  /*--------------------------------------------------------------------------*/
  /* INDEX 04 - conditions not correct                                        */
  /*--------------------------------------------------------------------------*/
  /* 12 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 13 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 14 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 15 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDReadDataByCommonId,
  /* 16 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat,
  /*--------------------------------------------------------------------------*/
  /* INDEX 05 - Suppress Negative Repsonse, no parameter supported            */
  /*--------------------------------------------------------------------------*/
  /* 17 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 18 - RESPONSE LENGTH  */  d_CCglo_00Byte,
  /*--------------------------------------------------------------------------*/
  /* INDEX 06 - Repsonse Pending                                              */
  /*--------------------------------------------------------------------------*/
  /* 19 - POST CONDIDTION  */  d_CCseqaudi_NegativeResponse,
  /* 20 - RESPONSE LENGTH  */  d_CCglo_03Byte,
  /* 21 - RESPONSE BYTE 00 */  d_CCkwpbase_SIDNegativResponseID,
  /* 22 - RESPONSE BYTE 01 */  d_CCkwpbase_SIDReadDataByCommonId,
  /* 23 - RESPONSE BYTE 02 */  d_CCkwpbase_NRCRequestCorrrectReceive_RespPending

}; /*--- END OF TABLE --------------------------------------------------------*/


void FUN_CCkwpaudi_CopyEntry_V(READDATABYID_ST* ReadDataById_ST, PTR_BUFFER_UB t_OutBuffer_PBUF)
{

  UBYTE t_EepromHelp_UB = 0;
  UBYTE  t_ResultEval_UB;
  UBYTE  t_DownloadMapIndex_UB;

  /*------------------------------------------------------*/
  /* get the corresponding download map entry for the     */
  /* address to be read from                              */
  /*------------------------------------------------------*/
  t_DownloadMapIndex_UB = FUN_CCkwpaudi_AddressToMemoryMapIndex_UB  \
         (c_CCconprj_ReadDataByID_AST[ReadDataById_ST->t_TableIndexs_UB].Address_UL,
          (c_CCconprj_ReadDataByID_AST[ReadDataById_ST->t_TableIndexs_UB].Address_UL +
          (ULONG)c_CCconprj_ReadDataByID_AST[ReadDataById_ST->t_TableIndexs_UB].NvmEntryOrLenth_UB) - 1ul);

   /*=====================================================*/
   /* -C-- a download map entry was found                  */
   /*======================================================*/
   if ( d_CCglo_NoEntry != t_DownloadMapIndex_UB )
   {

      /*----------------------------------------------------*/
      /* read out the status of the software block          */
      /*----------------------------------------------------*/
      t_ResultEval_UB = FUN_CCnvm_NvmAccess_UB ( \
                         c_CCconprj_LogBlockEEPROM_AUB[ReadDataById_ST->t_TableIndexs_UB][d_CCconprj_SWProgStateIndex],  \
                        (PTR_BUFFER_UB)t_EepromHelp_UB);

      /*====================================================*/
      /* -D-- the software block is valid                   */
      /*====================================================*/
      if ( (d_CCglo_NoError == t_ResultEval_UB) )
      {
         /*--------------------------------------------------*/
         /* do the memcopy                                   */
         /*--------------------------------------------------*/
         FUN_CCglo_PMemCopy_V( &(t_OutBuffer_PBUF[d_CCglo_StartOfData + ReadDataById_ST->t_ActDLCs_UB + 2]),
                               (FAR_PTR_UBYTE) (c_CCconprj_ReadDataByID_AST[ReadDataById_ST->t_TableIndexs_UB].Address_UL),
                               (ULONG) (ReadDataById_ST->t_Datalength_UB) );

      } /*-D-- END OF if(..) -------------------------------*/
      /*====================================================*/
      /* -D-- the application program is invalid            */
      /*====================================================*/
      else
      {
        /*--------------------------------------------------*/
        /* do the memcopy                                   */
        /*--------------------------------------------------*/
        FUN_CCglo_PMemCopy_V( &t_OutBuffer_PBUF[d_CCglo_StartOfData + ReadDataById_ST->t_ActDLCs_UB + 2],
                              (FAR_PTR_UBYTE) (c_CCconprj_ReadDataByID_AST[ReadDataById_ST->t_TableIndexs_UB].AltAddress_UL),
                              (ULONG) (ReadDataById_ST->t_Datalength_UB) );

      } /*-D-- END OF else (if(..)) ------------------------*/
    } /*-C-- END OF if(..) ---------------------------------*/
    /*======================================================*/
    /* -C-- no download map entry was found                 */
    /*======================================================*/
    else
    {
      /*----------------------------------------------------*/
      /* do the memcopy                                     */
      /*----------------------------------------------------*/
      FUN_CCglo_PMemCopy_V(  &t_OutBuffer_PBUF[d_CCglo_StartOfData + ReadDataById_ST->t_ActDLCs_UB + 2],\
                            (FAR_PTR_UBYTE) (c_CCconprj_ReadDataByID_AST[ReadDataById_ST->t_TableIndexs_UB].AltAddress_UL),  \
                            (ULONG) (ReadDataById_ST->t_Datalength_UB) );

    } /*-C-- END OF else (if(..)) --------------------------*/

  return;
}
/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_ReadTimelock_UB                             |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    This function reads the timelock of the security access   |
|                  out of the eeprom.                                        |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_Destination_PUB                           |
|                        Destination where the value shall be written        |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E x  x x x x                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    not used                    |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_ReadTimelock_UB ( PTR_BUFFER_UB t_Destination_PUB )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* read out the timelock                                                    */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCnvm_NvmAccess_UB (d_CCnvm_READ__TIMELOCK, t_Destination_PUB);

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_ReadTimelock_UB +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_WriteTimelock_UB                            |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    This function writes the timelock of the security access  |
|                  into the eeprom.                                          |
|                                                                            |
|  Parameter:      PTR_BUFFER_UB t_Source_PUB                                |
|                        Source where to get the timelock value              |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E x  x x x x                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    not used                    |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_WriteTimelock_UB ( PTR_BUFFER_UB t_Source_PUB )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* write the timelock                                                       */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_TIMELOCK, t_Source_PUB);

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Result_UB;

} /*-+- END OF FUN_CCkwpaudi_WriteTimelock_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_CheckAddrMode_UB                            |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    This function checks the addressing mode of the channel.  |
|                                                                            |
|  Parameter:      UBYTE  t_RequAddrMode_UB - address mode to be checked for |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E x  x x x x                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    not used                    |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_CheckAddrMode_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                       UBYTE t_RequAddrMode_UB)
{
  UBYTE t_Return_UB;
  UBYTE t_Index_UB;
  UBYTE t_ScanState_UB;

  /*--------------------------------------------------------------------------*/
  /* init scan variables                                                      */
  /*--------------------------------------------------------------------------*/
  t_Index_UB = 0x00;
  t_ScanState_UB = d_CCkwpaudi_ScanRun;

  /*==========================================================================*/
  /* -1- scan Structure for the correct channel number                        */
  /*==========================================================================*/
  while ( (d_CCkwpaudi_ScanRun == t_ScanState_UB ) &&
          ( d_CCglo_NoEntry != t_CCkwpaudi_KwpCtrl_ST.AddrModeCfg_PST  \
                                     [t_Index_UB].AddressMode_UB ) )
  {
    /*========================================================================*/
    /* -2- Channel number found                                               */
    /*========================================================================*/
    if ( t_InBuffer_PBUF[d_CCglo_ChannelNbr] == t_CCkwpaudi_KwpCtrl_ST. \
                          AddrModeCfg_PST[t_Index_UB].ChannelNbr_UB )
    {
      /*----------------------------------------------------------------------*/
      /* stop scan                                                            */
      /*----------------------------------------------------------------------*/
      t_ScanState_UB   = d_CCkwpaudi_ScanStop;

    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- Channel number not found                                           */
    /*========================================================================*/
    else
    {
      /*----------------------------------------------------------------------*/
      /* Check next entry                                                     */
      /*----------------------------------------------------------------------*/
      t_Index_UB += 0x01;

    } /* -2- END OF else (if(..)) --------------------------------------------*/
  } /* ---1- END OF while (..) -----------------------------------------------*/

  /*==========================================================================*/
  /* -1- Command does not have the required addressing mode                   */
  /*==========================================================================*/
  if ( ( t_RequAddrMode_UB & t_CCkwpaudi_KwpCtrl_ST.  \
                         AddrModeCfg_PST[t_Index_UB].AddressMode_UB) == 0)
  {
    /*------------------------------------------------------------------------*/
    /* Return error                                                           */
    /*------------------------------------------------------------------------*/
    t_Return_UB = d_CCglo_Error;

  } /* ---1- END OF if (..) --------------------------------------------------*/
  /*==========================================================================*/
  /* -3- Command has the required addressing mode                             */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* Return no error                                                        */
    /*------------------------------------------------------------------------*/
    t_Return_UB = d_CCglo_Ready;

  } /* ---1- END OF else (if (..)) -------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Return_UB;

} /*-+- END OF FUN_CCkwpaudi_CheckAddrMode_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_ClearFlashFunctions_V                       |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.h                                 |
|                                                                            |
|  Description:    This function deletes the flash functions from RAM.       |
|                                                                            |
|  Parameter:      -                                                         |
|                                                                            |
|  Return Value:   -                                                         |
|                                                                            |
+---------------------------------------------------------------------------*/
void FUN_CCkwpaudi_ClearFlashFunctions_V(void)
{
  ULONG t_ActAddress_UL;
  UBYTE *t_RamCell_PUB;

  /*==========================================================================*/
  /* -1- Deinit the flash functions - clear RAM                               */
  /*==========================================================================*/
  if((ULONG)d_CCglo_InitToZero != t_CCkwpaudi_KwpCtrl_ST.FFStartAddress_UL)
  {

    /*========================================================================*/
    /* -2- for the complete flash functions                                   */
    /*========================================================================*/
    for (t_ActAddress_UL = t_CCkwpaudi_KwpCtrl_ST.FFStartAddress_UL; \
         t_ActAddress_UL <= t_CCkwpaudi_KwpCtrl_ST.FFEndAddress_UL;  \
         t_ActAddress_UL ++                                          )
      {
      t_RamCell_PUB  = (UBYTE*) t_ActAddress_UL;
      *t_RamCell_PUB = 0x00;
    } /* -2- END OF for(..) --------------------------------------------------*/
  } /* ---1- END OF if(..) ---------------------------------------------------*/

} /*-+- END OF FUN_CCkwpaudi_ClearFlashFunctions_V +-+-+-+-+-+-+-+-+-+-+-+-+-+*/



/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCkwpaudi_EraseEeprom_UB                              |
|                                                                            |
|  Prototype at:   ADLATUS_KWP2000audi_tdf.c                                 |
|                                                                            |
|  Description:    This function erases the eeprom.                          |
|                                                                            |
|  Parameter:      -                                                         |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E x  x x x x                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    not used                    |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCkwpaudi_EraseEeprom_UB(void)
{
  UBYTE  t_Return_UB;
  UBYTE  t_EraseData_AUB[d_CCkwpaudi_MaxEraseBytesEeprom];
                                  /* Erase data to be written to EEPROM       */
  UBYTE  t_Count_UB;
  ULONG  t_EraseActAddress_UL;  /*  4 Byte - Erase start address              */
  UWORD  t_BytesToErase_UW;
  NVMACCESS_ST t_NVMDirectAccess_ST;   /* sturcture for the eeprom access     */


  t_Return_UB = d_CCglo_NoError;

  t_EraseActAddress_UL = t_CCkwpaudi_KwpCtrl_ST.EraseStartAddress_UL;

  for (t_Count_UB = 0x00; t_Count_UB < d_CCkwpaudi_MaxEraseBytesEeprom; t_Count_UB++)
  {
    t_EraseData_AUB[t_Count_UB] = d_CCkwpaudi_EepromErasePattern;

  }


  /*--------------------------------------------------------------------------*/
  /* init the structure                                                       */
  /*--------------------------------------------------------------------------*/
  t_NVMDirectAccess_ST.Data_PUB = t_EraseData_AUB;

  /*==========================================================================*/
  /* -1- do for all bytes to be erased                                        */
  /*==========================================================================*/
  while ( t_EraseActAddress_UL <= t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL)
  {
    /*------------------------------------------------------------------------*/
    /* calculate the number of byte to be read out of the eeprom              */
    /*------------------------------------------------------------------------*/
    t_BytesToErase_UW = (UWORD) ((t_CCkwpaudi_KwpCtrl_ST.EraseEndAddress_UL -  \
                                                      t_EraseActAddress_UL) + 1UL);

    /*========================================================================*/
    /* -2- number of bytes to erase is greater than maximum number            */
    /*========================================================================*/
    if (t_BytesToErase_UW > (UWORD)d_CCkwpaudi_MaxEraseBytesEeprom )
    {
      /*------------------------------------------------------------------------*/
      /* limit the number of bytes to erase to d_CCkwpaudi_MaxEraseBytesEeprom  */
      /*------------------------------------------------------------------------*/
      t_BytesToErase_UW = d_CCkwpaudi_MaxEraseBytesEeprom;

    } /* -2- END OF if (..) --------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* init the structure                                                     */
    /*------------------------------------------------------------------------*/
    t_NVMDirectAccess_ST.NbrBytes_UB = (UBYTE) t_BytesToErase_UW;

    t_NVMDirectAccess_ST.Address_UW  = (UWORD) t_EraseActAddress_UL;

    /*------------------------------------------------------------------------*/
    /* Set the number of total bytes to be written in this erase function.    */
    /*------------------------------------------------------------------------*/
    t_NVMDirectAccess_ST.TotalBytes_UW = (UWORD)t_CCkwpaudi_KwpCtrl_ST.EraseLength_UL;

    /*------------------------------------------------------------------------*/
    /* call the eeprom access                                                 */
    /*------------------------------------------------------------------------*/
    t_Return_UB |= FUN_CCnvm_NvmDirectWriteAccess_UB (t_NVMDirectAccess_ST);

    /*------------------------------------------------------------------------*/
    /* Increment Address                                                      */
    /*------------------------------------------------------------------------*/
    t_EraseActAddress_UL += (ULONG) t_BytesToErase_UW;

  }

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return t_Return_UB;

} /*-+- END OF FUN_CCkwpaudi_EraseEeprom_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
