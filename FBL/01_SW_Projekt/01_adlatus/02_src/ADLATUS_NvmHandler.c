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
|  Filename:  ADLATUS_NvmHandler.c                                             |
|                                                                              |
|  Comment:   This file includes all functions for the Non Volatile Memory     |
|             (NVM) access.                                                    |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_NvmHandler.c 5801 2010-07-09 13:21:35Z werner $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/20100706_S1NN_TMS470S_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_NvmHandler.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_CompilerSwitches_cdf.h"    /* must be the first CDF-include */
#include "ADLATUS_ConfigProject_cdf.h"
#include "ADLATUS_EventHandlerAudi_cdf.h"  /* Event Handler                   */
#include "ADLATUS_NvmHandlerAudi_cdf.h"
#include "ADLATUS_CusInterfaceAudi_cdf.h"
#include "ADLATUS_Kwp2000Audi_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_NvmHandler_tdf.h"

#include "ADLATUS_ValidationAudi_tdf.h"        /* Checksum routines for downloads */

#include "ADLATUS_EventHandlerAudi_tdf.h"  /* Event Handler                   */
#include "ADLATUS_CusInterfaceAudi_tdf.h"
#include "ADLATUS_ValHandler_tdf.h"
#include "ADLATUS_SSIHandler_tdf.h"
#include "ADLATUS_Kwp2000Audi_tdf.h"
#include "ADLATUS_ConfigProject_tdf.h"

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
#define d_CCnvm_ScanRun   (UBYTE) 0x01u
#define d_CCnvm_ScanStop  (UBYTE) 0x00u

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

typedef struct _NVMCONTROL
{
  UBYTE               NvmState_UB;         /*  1 Byte - State register        */
  const NVMENTRY_ST*  NvmAddressMap_PST;   /*  2 Byte - Ptr to address map    */
                                           /* ------------------------------- */
} NVMCONTROL_ST;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
NVMCONTROL_ST       t_CCnvm_NvmCtrl_ST;          /* Control Structure                    */
NVMRUNTIMEENTRY_ST  t_CCnvm_NvmRunTimeEntry_ST; /* Runtime Structure - Interface         */
                                                /* between NvmHandler and EEPROM Handler */


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
extern const FSW_ECUIdent_ST C_CCcusint_AlternativeECUIdent_ST;
extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeBootIdent_ST;
extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeAppIdent_ST;
extern const FSW_SWBlockIdent_ST C_CCcusint_AlternativeDataIdent_ST;
extern const UBYTE C_CCcusint_AlternativeBootSWProgState_UB;
extern const UBYTE C_CCcusint_AlternativeAppProgState_UB;
extern const UBYTE C_CCcusint_AlternativeDataProgState_UB;
extern const UBYTE C_CCcusint_AlternativeTimelock_UB;

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCnvm_InitNvmHandler_V                                  |
|                                                                              |
|  Prototype at:   ADLATUS_NvmHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function is the init function of the nvm handler       |
|                  modul.                                                      |
|                                                                              |
|  Parameter:      const NVMINTERFACE_ST* t_NvmHandlerInterface_PST .. Ptr to  |
|                                                                Interface     |
|                                                                              |
|  Return Value:  --                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCnvm_InitNvmHandler_V (const NVMINTERFACE_ST* t_NvmHandlerInterface_PST)
{
  UBYTE t_Dummy_UB;
  UBYTE t_EepromHelp_UB;
  UBYTE t_AdlatusVersion_AUB[d_CCconprj_LengthBootSWIdent];  /* [OPL263] */
  UBYTE t_Count_UB;
  UBYTE t_NvmResult_UB;  /* [OPL340] */
  UBYTE t_VersionMismatch_UB = d_CCglo_NoError;

  /*--------------------------------------------------------------------------*/
  /* (1) Init base address of nvm address map                                 */
  /*--------------------------------------------------------------------------*/
  t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST = t_NvmHandlerInterface_PST->NvmAddressMap_PST;
  /*--------------------------------------------------------------------------*/
  /* (2) Init nvm state register                                              */
  /*--------------------------------------------------------------------------*/
  t_CCnvm_NvmCtrl_ST.NvmState_UB = d_CCglo_InitNvmState;

/* #define NVMINIT */
 /**************************************************/
/* EEPROM Init only for debugging purpose */
 
#ifdef NVMINIT
{
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWProgState, (PTR_BUFFER_UB)&C_CCcusint_AlternativeBootIdent_ST.ProgState_AUB[0]);

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWRepairShopCodeSerNo, (PTR_BUFFER_UB)&C_CCcusint_AlternativeBootIdent_ST.RepairShopCodeSerNo_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWProgDate, (PTR_BUFFER_UB)&C_CCcusint_AlternativeBootIdent_ST.ProgDate_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWCntProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeBootIdent_ST.CntProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWSucProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeBootIdent_ST.SucProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWMaxProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeBootIdent_ST.MaxProgrAttempts_AUB[0]);

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWProgState, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.ProgState_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWLogicalBlockVersion, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.SoftwareVersionNbr_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWRepairShopCodeSerNo, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.RepairShopCodeSerNo_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWProgDate, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.ProgDate_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWCntProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.CntProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWSucProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.SucProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AswSWMaxProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeAppIdent_ST.MaxProgrAttempts_AUB[0]);

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWProgState, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.ProgState_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWLogicalBlockVersion, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.SoftwareVersionNbr_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWRepairShopCodeSerNo, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.RepairShopCodeSerNo_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWProgDate, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.ProgDate_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWCntProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.CntProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWSucProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.SucProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ParrotSWMaxProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.MaxProgrAttempts_AUB[0]);

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWProgState, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.ProgState_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWLogicalBlockVersion, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.SoftwareVersionNbr_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWRepairShopCodeSerNo, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.RepairShopCodeSerNo_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWProgDate, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.ProgDate_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWCntProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.CntProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWSucProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.SucProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_DataSWMaxProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeDataIdent_ST.MaxProgrAttempts_AUB[0]);

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWProgState, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.ProgState_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWLogicalBlockVersion, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.SoftwareVersionNbr_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWRepairShopCodeSerNo, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.RepairShopCodeSerNo_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWProgDate, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.ProgDate_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWCntProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.CntProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWSucProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.SucProgrAttempts_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_EepromSWMaxProgrAttempts, (PTR_BUFFER_UB)&C_CCcusint_AlternativeEepromIdent_ST.MaxProgrAttempts_AUB[0]);

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ECUProgramInfo, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.ECUProgramInfo_UB);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_StateOfFlash, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.StateOfFlash_UB);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ECUHardwareNbr, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.ECUHardwareNbr_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ECUHardwareVersionNbr, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.ECUHardwareVersionNbr_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_SysNameEngineType, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.SysNameEngineType_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_ECUSerialNumber, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.ECUSerialNumber_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AsamOdxFileID, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.AsamOdxFileID_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AsamOdxFileVersion, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.AsamOdxFileVersion_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_TIMELOCK, (PTR_BUFFER_UB)&C_CCcusint_AlternativeTimelock_UB);
#ifdef cs_CChal_variable_CANID
  {
     UBYTE dummy=0x02;
     FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_CanId, (PTR_BUFFER_UB)&dummy);
  }
#endif

/* FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_SysProgrammed, (PTR_BUFFER_UB)&C_CCcusint_AlternativeSysProgrammed_UB); */

  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_AppSoftwareVersionNbr, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.AppSWversionNbr[0]);
//  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_VIN, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.VIN_AUB[0]);
  FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_VWSparePartNbr, (PTR_BUFFER_UB)&C_CCcusint_AlternativeECUIdent_ST.VWSparePartNbr_AUB[0]);
}
#endif
/* END of EEPROM Init only for debugging purpose  */
/**************************************************/



  /*--------------------------------------------------------------------------*/
  /* read out the timelock                                                    */
  /*--------------------------------------------------------------------------*/
  t_NvmResult_UB = FUN_CCkwpaudi_ReadTimelock_UB (&sECUDataAppl_ST.SecAccess_UB );  /* [OPL340] */

  /*--------------------------------------------------------------------------*/
  /* Dummy for elapsed security access tries (if eeprom is not available)     */
  /*--------------------------------------------------------------------------*/
  /* sECUDataAppl_ST.SecAccess_UB = 0; */

  /*==========================================================================*/
  /* -1- NVM access was successful                                            */
  /*==========================================================================*/
  if (d_CCglo_NoError == t_NvmResult_UB)
  {
    /*========================================================================*/
    /* -2- timelock is running                                                */
    /*========================================================================*/
    if((sECUDataAppl_ST.SecAccess_UB & (UBYTE) 0x7Fu) >= (UBYTE) 0x03u)  /* [OPL288] */
    {
      /*----------------------------------------------------------------------*/
    /* set timelock flag                                                      */
      /*----------------------------------------------------------------------*/
     FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____Timelock_UB);

    } /* -2- END OF if(..) ---------------------------------------------------*/
  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- NVM access was unsuccessful                                          */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* do nothing                                                             */
    /*------------------------------------------------------------------------*/

  } /* ---1- END OF else(..) -------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* read out the fsw valid entry                                             */
  /*--------------------------------------------------------------------------*/
  t_NvmResult_UB = FUN_CCnvm_NvmAccess_UB (d_CCnvm_READ__ECUProgramInfo,&t_EepromHelp_UB);  /* [OPL340] */

  /*==========================================================================*/
  /* -1- NVM access was successful                                            */
  /*==========================================================================*/
  if (d_CCglo_NoError == t_NvmResult_UB)
  {
    /*========================================================================*/
    /* -1- fsw is invalid                                                     */
    /*========================================================================*/
  if (d_CCkwpaudi_AppConsistent != (d_CCkwpaudi_AppInconsistent & t_EepromHelp_UB))
  {
      /*----------------------------------------------------------------------*/
    /* read out if the system is freshly programmed                             */
      /*----------------------------------------------------------------------*/
      t_NvmResult_UB = FUN_CCnvm_NvmAccess_UB (d_CCnvm_READ__SysProgrammed,&t_EepromHelp_UB);  /* [OPL340] */

      /*======================================================================*/
      /* -4- NVM access was successful                                        */
      /*======================================================================*/
      if (d_CCglo_NoError == t_NvmResult_UB)
      {
        /*====================================================================*/
    /* -1- fsw is invalid                                                       */
        /*====================================================================*/
    if (d_CCkwpaudi_AppProgrammed != (d_CCkwpaudi_AppProgrammed & t_EepromHelp_UB))
    {
          /*------------------------------------------------------------------*/
          /* set fsw invalid flag                                             */
          /*------------------------------------------------------------------*/
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____FSWInvalid_UB);

        } /* -4- END OF if(..) -----------------------------------------------*/
      } /* ---3- END OF if(..) -----------------------------------------------*/
      /*======================================================================*/
      /* -1- NVM access was unsuccessful                                      */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* set fsw invalid flag                                               */
        /*--------------------------------------------------------------------*/
        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____FSWInvalid_UB);

      } /* ---3- END OF else(..) ---------------------------------------------*/
    } /* -----2- END OF if(..) -----------------------------------------------*/
  } /* -------1- END OF if(..) -----------------------------------------------*/
  /*==========================================================================*/
  /* -1- NVM access was unsuccessful                                          */
  /*==========================================================================*/
  else
  {
      /*------------------------------------------------------------------------*/
      /* set fsw invalid flag                                                   */
      /*------------------------------------------------------------------------*/
      FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____FSWInvalid_UB);

  } /* -------1- END OF else(..) ---------------------------------------------*/

  /*==========================================================================*/
  /* -1- fsw is marked valid in eeprom                                        */
  /*==========================================================================*/
  if(FALSE == FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_LOOKFOR_FSWInvalid_UB) )  /* [OPL340] */
  {
    /*------------------------------------------------------------------------*/
    /* do the fsw quickcheck                                                  */
    /*------------------------------------------------------------------------*/
    t_Dummy_UB = FUN_CCVhnd_CheckValidationMap_UB ( \
          (UWORD)(d_CCglo_AswArea | d_CCglo_FlashMemory | d_CCglo_DoQuickCheck), \
          (UWORD)(d_CCglo_AdlatusAreaMask | d_CCglo_MemoryTypeMask | d_CCglo_QuickCheckMask) );

    /*========================================================================*/
    /* -2- Application Software is not programmed                             */
    /*========================================================================*/
    if ( d_CCglo_FswInSystem != t_Dummy_UB)
    {
      /*----------------------------------------------------------------------*/
      /* set fsw invalid flag                                                 */
      /*----------------------------------------------------------------------*/
       FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____FSWInvalid_UB);

    } /* -2- END OF if(..) ---------------------------------------------------*/
  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Check the Adlatus version number and update it if necessary              */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Read out the actual adlatus version                                      */
  /*--------------------------------------------------------------------------*/
  t_NvmResult_UB = FUN_CCnvm_NvmAccess_UB (d_CCnvm_READ__BootSWLogicalBlockVersion,   \
                                        (PTR_BUFFER_UB)t_AdlatusVersion_AUB);

  /*==========================================================================*/
  /* -1- NVM access was successful                                            */
  /*==========================================================================*/
  if (d_CCglo_NoError == t_NvmResult_UB)
  {
    /*========================================================================*/
    /* -2- for all bytes of the adlatus identification                        */
    /*========================================================================*/
    for (t_Count_UB = 0u; t_Count_UB < c_CCconprj_LengthBootSWIdent_UB; t_Count_UB++)
    {
      /*======================================================================*/
      /* -3- the adlatus identification of the eeprom is not up-to-date       */
      /*======================================================================*/
    if (c_CCcusint_AdlatusVersion_AUB[t_Count_UB] != t_AdlatusVersion_AUB[t_Count_UB])
    {
      t_VersionMismatch_UB = d_CCglo_Error;

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF for(..) ------------------------------------------------*/

    /*========================================================================*/
    /* -2- the adlatus identification of the eeprom is not up-to-date         */
    /*========================================================================*/
  if (d_CCglo_NoError != t_VersionMismatch_UB)
  {
      /*----------------------------------------------------------------------*/
    /* update the adlatus identification                                      */
      /*----------------------------------------------------------------------*/
    FUN_CCnvm_NvmAccess_UB (d_CCnvm_WRITE_BootSWLogicalBlockVersion,  \
                            (PTR_BUFFER_UB)c_CCcusint_AdlatusVersion_AUB);

    } /* ---2- END OF if(..) -------------------------------------------------*/
  } /* -----1- END OF if(..) -------------------------------------------------*/
  /*==========================================================================*/
  /* -1- NVM access was unsuccessful                                          */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* do nothing                                                             */
    /*------------------------------------------------------------------------*/

  } /* -----1- END OF else(..) -----------------------------------------------*/

  return;

} /*-+- END OF FUNCTION 'FUN_CCnvm_InitNvmHandler_V' -+-+-+-+-+-+-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCnvm_ScanNvmMemoryTable_UB                             |
|                                                                              |
|  Prototype at:   ADLATUS_NvmHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function scans the NVM memory table for the correct    |
|                  entry for EEPROM access.                                    |
|                                                                              |
|  Parameter:      UBYTE t_Handle_UB .. Handle for scan                        |
|                                                                              |
|  Return Value:   UBYTE t_Index_UB  .. Index of correct NVM memory table      |
|                                       entry.                                 |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCnvm_ScanNvmMemoryTable_UB (UBYTE t_Handle_UB)
{
  UBYTE t_Index_UB;
  UBYTE t_ScanState_UB;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Index_UB     = 0x00;
  t_ScanState_UB = d_CCnvm_ScanRun;

  /*==========================================================================*/
  /* -1- Search for entry in structure                                        */
  /*==========================================================================*/
  while (d_CCnvm_ScanStop != t_ScanState_UB  )
  {
    /*========================================================================*/
    /* -2- End of table found                                                 */
    /*========================================================================*/
    if( (d_CCglo_NoEntry & (UBYTE) 0x7Fu) == \
        (t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST[t_Index_UB].Name_UB & (UBYTE) 0x7Fu) )
    {
      /* Stop scanning                                                        */
      t_ScanState_UB = d_CCnvm_ScanStop;
      t_Index_UB     = d_CCglo_NoEntry;
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- Normal entry found                                                 */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -3- Entry is correct                                                 */
      /*======================================================================*/
      if( (t_Handle_UB & (UBYTE) 0x7Fu) == \
          (t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST[t_Index_UB].Name_UB & (UBYTE) 0x7Fu))
      {
        /* Stop scanning                                                      */
        t_ScanState_UB = d_CCnvm_ScanStop;
      } /* -3- END OF if(..) -------------------------------------------------*/

      /*======================================================================*/
      /* -3- Entry is not correct                                             */
      /*======================================================================*/
      else
      {
        /* Check next entry                                                   */
        t_Index_UB++;

      } /* -3- END OF else (if(..)) ------------------------------------------*/
    } /* ---2- END OF else (if(..)) ------------------------------------------*/
  } /* -----1- END OF while (..) ---------------------------------------------*/

 return (t_Index_UB);

} /*-+- END OF FUNCTION 'FUN_CCnvm_ScanNvmMemoryTable_UB' -+-+-+-+-+-+-+-+-+-+*/



/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCnvm_NvmAccess_UB                                      |
|                                                                              |
|  Prototype at:   ADLATUS_NvmHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function takes access to the NVM (e.g. EEPROM).        |
|                                                                              |
|  Parameter:      UBYTE          t_Handle_UB    .. Read / Write               |
|                  PTR_BUFFER_UB  t_Buffer_PBUF  .. Pointer to Buffer          |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCnvm_NvmAccess_UB (UBYTE t_Handle_UB, PTR_BUFFER_UB t_Buffer_PBUF )
{
  UBYTE t_Result_UB;
  UBYTE t_Index_UB;

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = d_CCglo_NoError;

  /*--------------------------------------------------------------------------*/
  /* Scan the NVM memory map for correct entry                                */
  /*--------------------------------------------------------------------------*/
  t_Index_UB = FUN_CCnvm_ScanNvmMemoryTable_UB (t_Handle_UB);

  /*==========================================================================*/
  /* -1- No Correct entry found                                               */
  /*==========================================================================*/
  if ( d_CCglo_NoEntry == t_Index_UB )
  {
    t_Result_UB = (UBYTE) ~d_CCglo_NoError;
  } /* -1- END OF if (..) ----------------------------------------------------*/

  /*==========================================================================*/
  /* -1- Correct entry found                                                  */
  /*==========================================================================*/
  else
  {
    /*--------------------------------------------------------------------*/
    /* copy all access data to runtime entry structure                    */
    /*--------------------------------------------------------------------*/
    t_CCnvm_NvmRunTimeEntry_ST.Access_UB      = (t_Handle_UB & (UBYTE) 0x80u);
    t_CCnvm_NvmRunTimeEntry_ST.StorageType_UB = t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST[t_Index_UB].StorageType_UB;
    t_CCnvm_NvmRunTimeEntry_ST.DataLength_UW  = t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST[t_Index_UB].DataLength_UW;
    t_CCnvm_NvmRunTimeEntry_ST.TotalLength_UW = t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST[t_Index_UB].TotalLength_UW;
    t_CCnvm_NvmRunTimeEntry_ST.Address_UW     = t_CCnvm_NvmCtrl_ST.NvmAddressMap_PST[t_Index_UB].Address_UW;
    t_CCnvm_NvmRunTimeEntry_ST.Buffer_UB      = t_Buffer_PBUF;
    /*--------------------------------------------------------------------*/
    /* Call the specific EEPROM Handler                                   */
    /*--------------------------------------------------------------------*/

    /* for active EEPROM */
    t_Result_UB = FUN_CCssi_EepromHandler_UB((const NVMRUNTIMEENTRY_ST*)&t_CCnvm_NvmRunTimeEntry_ST);
  }

  /************************************************/
  /* for deactivated EEPROM error handling        */
  /* Comment this out for a real existing EEPROM  */
  /************************************************/
  /* t_Result_UB = 1; */

  return (t_Result_UB);
} /*-+- END OF FUNCTION 'FUN_CCnvm_NvmAccess_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/



/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCnvm_NvmDirectWriteAccess_UB                           |
|                                                                              |
|  Prototype at:   ADLATUS_NvmHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function takes direct write access to the NVM          |
|                  (e.g. EEPROM).                                              |
|                                                                              |
|  Parameter:      NVMACCESS_ST: write parameters                              |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCnvm_NvmDirectWriteAccess_UB (NVMACCESS_ST t_NVMDirectAccess_ST)
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* copy all access data to runtime entry structure                          */
  /*--------------------------------------------------------------------------*/
  t_CCnvm_NvmRunTimeEntry_ST.Access_UB      = d_CCglo_Write;
  t_CCnvm_NvmRunTimeEntry_ST.StorageType_UB = d_CCglo_SingleBuffering;
  t_CCnvm_NvmRunTimeEntry_ST.DataLength_UW  = t_NVMDirectAccess_ST.NbrBytes_UB;
  t_CCnvm_NvmRunTimeEntry_ST.TotalLength_UW = t_NVMDirectAccess_ST.TotalBytes_UW;
  t_CCnvm_NvmRunTimeEntry_ST.Address_UW     = t_NVMDirectAccess_ST.Address_UW;

  t_CCnvm_NvmRunTimeEntry_ST.Buffer_UB = t_NVMDirectAccess_ST.Data_PUB;

  /*--------------------------------------------------------------------------*/
  /* Call the specific EEPROM Handler                                         */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCssi_EepromHandler_UB ((const NVMRUNTIMEENTRY_ST*)&t_CCnvm_NvmRunTimeEntry_ST);


  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCnvm_NvmDirectWriteAccess_UB' +-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCnvm_NvmDirectReadAccess_UB                            |
|                                                                              |
|  Prototype at:   ADLATUS_NvmHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function takes direct read access from the NVM         |
|                  (e.g. EEPROM).                                              |
|                                                                              |
|  Parameter:      NVMACCESS_ST: read parameters                               |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCnvm_NvmDirectReadAccess_UB (NVMACCESS_ST t_NVMDirectAccess_ST)
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* copy all access data to runtime entry structure                          */
  /*--------------------------------------------------------------------------*/
  t_CCnvm_NvmRunTimeEntry_ST.Access_UB      = d_CCglo_Read;
  t_CCnvm_NvmRunTimeEntry_ST.StorageType_UB = d_CCglo_SingleBuffering;
  t_CCnvm_NvmRunTimeEntry_ST.DataLength_UW  = t_NVMDirectAccess_ST.NbrBytes_UB;
  t_CCnvm_NvmRunTimeEntry_ST.TotalLength_UW = t_NVMDirectAccess_ST.TotalBytes_UW;
  t_CCnvm_NvmRunTimeEntry_ST.Address_UW     = t_NVMDirectAccess_ST.Address_UW;

  t_CCnvm_NvmRunTimeEntry_ST.Buffer_UB = t_NVMDirectAccess_ST.Data_PUB;

  /*--------------------------------------------------------------------------*/
  /* Call the specific EEPROM Handler                                         */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = FUN_CCssi_EepromHandler_UB ((const NVMRUNTIMEENTRY_ST*)&t_CCnvm_NvmRunTimeEntry_ST);


  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCnvm_NvmDirectReadAccess_UB' -+-+-+-+-+-+-+-+-+-*/



/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
