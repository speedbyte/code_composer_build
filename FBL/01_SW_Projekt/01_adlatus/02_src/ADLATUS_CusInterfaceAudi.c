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
|  Filename:  ADLATUS_CustomerInterfaceAudi.c                                  |
|                                                                              |
|  Comment:   Interface between SMART ADLATUS and the customer Software.       |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CusInterfaceAudi.c 4477 2008-11-27 12:11:59Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_CusInterfaceAudi.c $
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
#include "ADLATUS_CusInterfaceAudi_cdf.h"
#include "ADLATUS_NvmHandlerAudi_cdf.h"
#include "ADLATUS_ConfigProject_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include */
#include "ADLATUS_CusInterfaceAudi_tdf.h"

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
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRIES                                                                 */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */


/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA CusInt.01                                           **/
/**     ----------------------------------------------------------------     **/
/**     S Y S T E M   C O N F I G U R A T I O N                              **/
/**     ----------------------------------------------------------------     **/
/**     Adlatus Version                                                      **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/
#ifdef cs_CChal_variable_CANID
const UBYTE c_CCcusint_AdlatusVersion_AUB[d_CCconprj_LengthBootSWIdent] =
{
  /*-----------+---------------------+---------+------------------------------*/
  /* Byte No   | Comment             | coding  | Value                        */
  /*-----------+---------------------+---------+------------------------------*/
  /* 00 (high) | released version    | hex    */ 0x30,
  /*-----------+---------------------+---------+------------------------------*/
  /* 01 (high) | released version    | hex    */ 0x30,
  /*-----------+---------------------+---------+------------------------------*/
  /* 02 (mid)  | compatible changes  | hex    */ 0x30,                   
  /*-----------+---------------------+---------+------------------------------*/
  /* 03 (low)  | bugfix version      | hex    */ 0x34                         
  /*-----------+---------------------+---------+------------------------------*/

}; /* --- END OF TABLE -------------------------------------------------------*/

#else

const UBYTE c_CCcusint_AdlatusVersion_AUB[d_CCconprj_LengthBootSWIdent] =
{
  /*-----------+---------------------+---------+------------------------------*/
  /* Byte No   | Comment             | coding  | Value                        */
  /*-----------+---------------------+---------+------------------------------*/
  /* 00 (high) | released version    | hex    */ 0x30,
  /*-----------+---------------------+---------+------------------------------*/
  /* 01 (high) | released version    | hex    */ 0x30,
  /*-----------+---------------------+---------+------------------------------*/
  /* 02 (mid)  | compatible changes  | hex    */ 0x30,                   
  /*-----------+---------------------+---------+------------------------------*/
  /* 03 (low)  | bugfix version      | hex    */ 0x34                         
  /*-----------+---------------------+---------+------------------------------*/

}; /* --- END OF TABLE -------------------------------------------------------*/
#endif
/******************************************************************************/
/* -- -- END OF APPLICATION AREA   FswInt.01    -- -- -- -- -- -- -- -- -- -- */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
/* constants for service 0x22 ReadDataByIdentifier                            */
/* these constants can be used if the eeprom access was unsuccessful or       */
/* the software block is invalid where the data shall be read from flash      */
/*---------------++-----------------------------------------------------------*/

/*typedef struct 
{
  UBYTE ProgState_AUB[1];
  UBYTE SoftwareVersionNbr_AUB[4];
  UBYTE RepairShopCodeSerNo_AUB[6];
  UBYTE ProgDate_AUB[3];
  UBYTE CntProgrAttempts_AUB[2];
  UBYTE SucProgrAttempts_AUB[2];
  UBYTE MaxProgrAttempts_AUB[2];  
} FSW_SWBlockIdent_ST;*/

const FSW_SWBlockIdent_ST C_CCcusint_AlternativeBootIdent_ST =
{
  {0x00},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0'},
  {0x00, 0x00, 0x00},
  {0x00, 0x00},
  {0x00, 0x00},
  {0xFF, 0xFF}
};

const FSW_SWBlockIdent_ST C_CCcusint_AlternativeAppIdent_ST =
{
  {0x00},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0'},
  {0x00, 0x00, 0x00},
  {0x00, 0x00},
  {0x00, 0x00},
  {0xFF, 0xFE}
};

const FSW_SWBlockIdent_ST C_CCcusint_AlternativeDataIdent_ST =
{
  {0x00},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0'},
  {0x00, 0x00, 0x00},
  {0x00, 0x00},
  {0x00, 0x00},
  {0xFF, 0xFE}
};

const FSW_SWBlockIdent_ST C_CCcusint_AlternativeEepromIdent_ST =
{
  {0x00},
  {'0', '0', '0', '0'},
  {'0', '0', '0', '0', '0', '0'},
  {0x00, 0x00, 0x00},
  {0x00, 0x00},
  {0x00, 0x00},
  {0xFF, 0xFE}
};


/*typedef struct 
{
  0xF1DF -- UBYTE ECUProgramInfo_UB;
  0x0405 -- UBYTE StateOfFlash_UB;
  0xF191 -- UBYTE ECUHardwareNbr_AUB[11];
  0xF1A3 -- UBYTE ECUHardwareVersionNbr_AUB[3];
  0xF187 -- UBYTE VWSparePartNbr_AUB[11];
  0xF189 -- UBYTE AppSWversionNbr[4];  
  0xF1DF -- UBYTE ECUSerialNumber_AUB[14];
  0xF190 -- UBYTE VIN_AUB[17];
  0xF197 -- UBYTE SysNameEngineType_AUB[13];
  0xF19E -- UBYTE AsamOdxFileID_AUB[5];
  0xF1A2 -- UBYTE AsamOdxFileVersion_AUB[6];
} FSW_ECUIdent_ST;*/

const FSW_ECUIdent_ST C_CCcusint_AlternativeECUIdent_ST =
{
  0x40,
  0x00,  
  {'7', 'P', '6', '0', '3', '5', '7', '3', '0', ' ', ' '}, /*7P6035730*/
  {'H', '0', '1'},
  {'7', 'P', '6', '0', '3', '5', '7', '3', '0', ' ', ' '},
  {'X', '0', '2', '0'},
  {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
  {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
  {'U', 'H', 'V', '_', 'N', 'A', 'R', '-', '-', '-', '-', '-', '-'},
  {'U', 'H', 'V', 'N', 'A', 0x00, '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}, /*EV_UHVNAR*/
  {'S', '0', '2', '0', '0', '1'},
};


#ifdef cs_Signatur_Aktiv
const UBYTE C_CCcusint_FDS_AUB[d_CCconprj_LengthFDSProjectData] = \
  {
    0x00, \
    /* FDS Project ID */    0x00,0x00,0x10,0x00, \
    /* HASH Value     */    0x00
};
#endif

const UBYTE C_CCcusint_AlternativeBootSWProgState_UB        = 0x01;
const UBYTE C_CCcusint_AlternativeAppProgState_UB           = 0x01;
const UBYTE C_CCcusint_AlternativeDataProgState_UB          = 0x01;
const UBYTE C_CCcusint_AlternativeTimelock_UB               = 0x00;
const UBYTE C_CCcusint_AlternativeSysProgrammed_UB          = 0x00;

            
/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
