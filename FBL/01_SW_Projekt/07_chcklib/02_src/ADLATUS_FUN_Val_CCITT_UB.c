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
|  Filename:  ADLATUS_FUN_Val_CCITT_UB.c                                       |
|                                                                              |
|  Comment:   This file includes a checksum calculations functions.            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Val_CCITT_UB.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_FUN_Val_CCITT_UB.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      G L O B A L   C O M P I L E R   S W I T C H E S                       */
/* ========================================================================== */
/* DONT REMOVE: This file has to be included to all ADLATUS project files     */
/*               and has to be the first include!                             */
#include "ADLATUS_CompilerSwitches_cdf.h"
/* = END:DONT REMOVE ======================================================== */

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_Global_cdf.h"              /* global defines                */
#include "ADLATUS_Hal_cdf.h"              /* global defines                */
#include "ADLATUS_ValHandler_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"               /* must be the first TDF include */
#include "ADLATUS_Hal_tdf.h"
#include "ADLATUS_ValHandler_tdf.h"


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
#define  d_CCval_StopScan                       (UBYTE) 0x00u
#define  d_CCval_ScanRunning                    (UBYTE) 0x01u

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

UBYTE t_BufferFlag_UB = 0u;

extern const UWORD c_CCconprj_StartValue_CRC16_UW;


/*----------------------------------------------------------------------------*/
/* CCITT coding table                                                         */
/*----------------------------------------------------------------------------*/
const UWORD CRC16CCITTtab[256u] =
{
        /* 0      1      2      3     4      5      6      7         8      9     A      B       C      D      E      F */
/* 0 */ 0x0000u,0x1189u,0x2312u,0x329Bu,0x4624u,0x57ADu,0x6536u,0x74BFu,  0x8C48u,0x9DC1u,0xAF5Au,0xBED3u,0xCA6Cu,0xDBE5u,0xE97Eu,0xF8F7u,
/* 1 */ 0x1081u,0x0108u,0x3393u,0x221Au,0x56A5u,0x472Cu,0x75B7u,0x643Eu,  0x9CC9u,0x8D40u,0xBFDBu,0xAE52u,0xDAEDu,0xCB64u,0xF9FFu,0xE876u,
/* 2 */ 0x2102u,0x308Bu,0x0210u,0x1399u,0x6726u,0x76AFu,0x4434u,0x55BDu,  0xAD4Au,0xBCC3u,0x8E58u,0x9FD1u,0xEB6Eu,0xFAE7u,0xC87Cu,0xD9F5u,
/* 3 */ 0x3183u,0x200Au,0x1291u,0x0318u,0x77A7u,0x662Eu,0x54B5u,0x453Cu,  0xBDCBu,0xAC42u,0x9ED9u,0x8F50u,0xFBEFu,0xEA66u,0xD8FDu,0xC974u,
/* 4 */ 0x4204u,0x538Du,0x6116u,0x709Fu,0x0420u,0x15A9u,0x2732u,0x36BBu,  0xCE4Cu,0xDFC5u,0xED5Eu,0xFCD7u,0x8868u,0x99E1u,0xAB7Au,0xBAF3u,
/* 5 */ 0x5285u,0x430Cu,0x7197u,0x601Eu,0x14A1u,0x0528u,0x37B3u,0x263Au,  0xDECDu,0xCF44u,0xFDDFu,0xEC56u,0x98E9u,0x8960u,0xBBFBu,0xAA72u,
/* 6 */ 0x6306u,0x728Fu,0x4014u,0x519Du,0x2522u,0x34ABu,0x0630u,0x17B9u,  0xEF4Eu,0xFEC7u,0xCC5Cu,0xDDD5u,0xA96Au,0xB8E3u,0x8A78u,0x9BF1u,
/* 7 */ 0x7387u,0x620Eu,0x5095u,0x411Cu,0x35A3u,0x242Au,0x16B1u,0x0738u,  0xFFCFu,0xEE46u,0xDCDDu,0xCD54u,0xB9EBu,0xA862u,0x9AF9u,0x8B70u,
/* 8 */ 0x8408u,0x9581u,0xA71Au,0xB693u,0xC22Cu,0xD3A5u,0xE13Eu,0xF0B7u,  0x0840u,0x19C9u,0x2B52u,0x3ADBu,0x4E64u,0x5FEDu,0x6D76u,0x7CFFu,
/* 9 */ 0x9489u,0x8500u,0xB79Bu,0xA612u,0xD2ADu,0xC324u,0xF1BFu,0xE036u,  0x18C1u,0x0948u,0x3BD3u,0x2A5Au,0x5EE5u,0x4F6Cu,0x7DF7u,0x6C7Eu,
/* A */ 0xA50Au,0xB483u,0x8618u,0x9791u,0xE32Eu,0xF2A7u,0xC03Cu,0xD1B5u,  0x2942u,0x38CBu,0x0A50u,0x1BD9u,0x6F66u,0x7EEFu,0x4C74u,0x5DFDu,
/* B */ 0xB58Bu,0xA402u,0x9699u,0x8710u,0xF3AFu,0xE226u,0xD0BDu,0xC134u,  0x39C3u,0x284Au,0x1AD1u,0x0B58u,0x7FE7u,0x6E6Eu,0x5CF5u,0x4D7Cu,
/* C */ 0xC60Cu,0xD785u,0xE51Eu,0xF497u,0x8028u,0x91A1u,0xA33Au,0xB2B3u,  0x4A44u,0x5BCDu,0x6956u,0x78DFu,0x0C60u,0x1DE9u,0x2F72u,0x3EFBu,
/* D */ 0xD68Du,0xC704u,0xF59Fu,0xE416u,0x90A9u,0x8120u,0xB3BBu,0xA232u,  0x5AC5u,0x4B4Cu,0x79D7u,0x685Eu,0x1CE1u,0x0D68u,0x3FF3u,0x2E7Au,
/* E */ 0xE70Eu,0xF687u,0xC41Cu,0xD595u,0xA12Au,0xB0A3u,0x8238u,0x93B1u,  0x6B46u,0x7ACFu,0x4854u,0x59DDu,0x2D62u,0x3CEBu,0x0E70u,0x1FF9u,
/* F */ 0xF78Fu,0xE606u,0xD49Du,0xC514u,0xB1ABu,0xA022u,0x92B9u,0x8330u,  0x7BC7u,0x6A4Eu,0x58D5u,0x495Cu,0x3DE3u,0x2C6Au,0x1EF1u,0x0F78u

};


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

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCval_CCITT_UB                                          |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates a checksum by using the            |
|                  algorithm of CCITT.                                         |
|                                                                              |
|  Parameter:      ULONG t_StartAddress_UL    .. calculation start address     |
|                  ULONG t_StopAddress_UL     .. calculation stop address      |
|                  ULONG t_ChecksumAddress_UL .. checksum address              |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB .. 0x00 - No Error                        |
|                                       0x01 - Checksum Error                  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCval_CCITT_UB (VALCONTROL_ST  *Parameters )
{
  UBYTE          t_Result_UB;           /* Result of check                 */

  FAR_PTR_UBYTE  t_TempAddress_PUB;

  ULONG   t_TempAddress_UL;      /* temp address */
  
  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )
  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB            = d_CCVhnd_Ready;
  t_TempAddress_UL       = (ULONG)(Parameters->StartAddress_PBUF);  /*l_int !e923 */

  t_TempAddress_PUB      = Parameters->StartAddress_PBUF;
  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & Parameters->Control_UB) )
  {
    t_BufferFlag_UB = 0u;
    if ( (UWORD)Parameters->WorkingBuffer_UL != *((VHND_PTR_BUF_UWORD)Parameters->ChecksumAddress_PBUF))
    {
      t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
    }
    else 
    {
      t_Result_UB = (d_CCVhnd_CheckOK | d_CCVhnd_Ready);
    }
  }
  else 
  {
    if(!(t_BufferFlag_UB))
    {
      t_BufferFlag_UB = 0xFF;
      Parameters->WorkingBuffer_UL = c_CCconprj_StartValue_CRC16_UW;
    }
    /*==========================================================================*/
    /* -1- Calculate Checksum                                                   */
    /*==========================================================================*/
    while ((((VHND_PTR_BUF_UBYTE)t_TempAddress_UL) <=  Parameters->EndAddress_PBUF) )  /*l_int !e923 !e946 */
    {
      /*------------------------------------------------------------------------*/
      /* Calculate Checksum                                                     */
      /*------------------------------------------------------------------------*/
      Parameters->WorkingBuffer_UL = 
         ((ULONG)CRC16CCITTtab[(UBYTE) Parameters->WorkingBuffer_UL ^ \
         *t_TempAddress_PUB  ] ^ 
         (Parameters->WorkingBuffer_UL >> 0x08ul));     /*l_int !e912 */
    
      /* Increment Address and counter                                          */
      t_TempAddress_UL++;
      
    } /* ---1- END OF while (..) -----------------------------------------------*/
    t_Result_UB = (d_CCVhnd_Ready);
  }
  
  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return ( t_Result_UB );
} /*-+- END OF FUNCTION 'FUN_CCval_CCITT_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
