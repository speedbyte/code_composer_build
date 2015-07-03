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
|  Filename:  ADLATUS_Securityaudi.c                                           |
|                                                                              |
|  Comment:   This file includes the security routines for the audi Access.    |
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
| $Id: ADLATUS_SecurityAudi.c 4522 2008-12-01 16:18:15Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_SecurityAudi.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_SecurityAudi_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include */
#include "ADLATUS_SecurityAudi_tdf.h"
#include "ADLATUS_Global_tdf.h"

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

/*----------------------------------------------------------------------------*/
/* SecurityControlStructure                                                   */
/*----------------------------------------------------------------------------*/
typedef struct _SECURITYCONTROL
{
  UNION4 Seed_U;                          /*  4 Byte - Seed                   */
  UNION4 Key_U;                           /*  4 Byte - Key                    */  
  UNION4 Resp_U;                          /*  4 Byte - result from tester     */
} SECURITYCONTROL_ST;


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
static SECURITYCONTROL_ST t_CCsec_SecurityCtrl;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* No entry                                                                   */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* No entry                                                                   */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */








/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsecaudi_CreateSeed_V                                  |
|                                                                              |
|  Prototype at:   ADLATUS_Securityaudi_tdf.h                                  |
|                                                                              |
|  Description:    This function creats a seed vor security access.            |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB  t_SeedBuffer_PBUF .. Buffer for the seed     |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCsecaudi_CreateSeed_V ( PTR_BUFFER_UB t_SeedBuffer_PBUF  )
{
  UBYTE t_TempValue_UB;
  UBYTE Iterations = 6;
  
  ULONG Seed_1, SeedHelp;
  ULONG Carry;

  /*--------------------------------------------------------------------------*/
  /* Create a seed                                                            */
  /*--------------------------------------------------------------------------*/
  t_CCsec_SecurityCtrl.Seed_U.ULong_ST.ULong0_UL = \
                                            FUN_CCglo_GetRandomValue_UL ();


  Seed_1 = t_CCsec_SecurityCtrl.Seed_U.ULong_ST.ULong0_UL;
  Carry  = t_CCsec_SecurityCtrl.Seed_U.ULong_ST.ULong0_UL;

   while(Iterations >= 1u)
   {
      Seed_1 -= 0xA36D9BF1ul;
 
      Carry = 0u;
      Carry = (Seed_1 & 0x00000001ul);
      SeedHelp = Carry;
      Seed_1 >>= 1u;
      SeedHelp <<= 31u;
      Seed_1 |= SeedHelp;
      Iterations--;
      if(Carry == 1u)
      {
         Seed_1 ^=0x1B49CDDFul;
      }
   }

  t_CCsec_SecurityCtrl.Key_U.ULong_ST.ULong0_UL = Seed_1 ;

  /*--------------------------------------------------------------------------*/
  /* Write the random value into given buffer                                 */
  /*--------------------------------------------------------------------------*/
  t_TempValue_UB          = t_CCsec_SecurityCtrl.Seed_U.UByte_ST.UByte0_UB;
  t_SeedBuffer_PBUF[0x00u] = t_TempValue_UB;
  t_TempValue_UB          = t_CCsec_SecurityCtrl.Seed_U.UByte_ST.UByte1_UB;
  t_SeedBuffer_PBUF[0x01u] = t_TempValue_UB;
  t_TempValue_UB          = t_CCsec_SecurityCtrl.Seed_U.UByte_ST.UByte2_UB;
  t_SeedBuffer_PBUF[0x02u] = t_TempValue_UB;
  t_TempValue_UB          = t_CCsec_SecurityCtrl.Seed_U.UByte_ST.UByte3_UB;
  t_SeedBuffer_PBUF[0x03u] = t_TempValue_UB;

  return;

} /*-+- END OF FUNCTION 'FUN_CCsecaudi_CreateSeed_V' -+-+-+-+-+-+-+-+-+-+-+-+*/





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsecaudi_CheckKey_UB                                   |
|                                                                              |
|  Prototype at:   ADLATUS_Securityaudi_tdf.h                                  |
|                                                                              |
|  Description:    This function creats a seed vor security access.            |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB  t_SeedBuffer_PBUF .. Buffer for the seed     |
|                                                                              |
|  Return Value:   d_CCval_AccessGranted .. Key was correct                    |
|                  d_CCval_AccessDenied  .. Key was wrong                      |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE  FUN_CCsecaudi_CheckKey_UB (const PTR_BUFFER_UB t_KeyBuffer_PBUF )
{

  UBYTE  t_Result_UB;

  UBYTE  t_TempValue_UB;
  
  /*--------------------------------------------------------------------------*/
  /* Get the key from given buffer                                            */
  /*--------------------------------------------------------------------------*/
  t_TempValue_UB = t_KeyBuffer_PBUF[0x00];
  t_CCsec_SecurityCtrl.Resp_U.UByte_ST.UByte0_UB = t_TempValue_UB;
  t_TempValue_UB = t_KeyBuffer_PBUF[0x01];
  t_CCsec_SecurityCtrl.Resp_U.UByte_ST.UByte1_UB = t_TempValue_UB;
  t_TempValue_UB = t_KeyBuffer_PBUF[0x02];
  t_CCsec_SecurityCtrl.Resp_U.UByte_ST.UByte2_UB = t_TempValue_UB;
  t_TempValue_UB = t_KeyBuffer_PBUF[0x03];
  t_CCsec_SecurityCtrl.Resp_U.UByte_ST.UByte3_UB = t_TempValue_UB;


  /*==========================================================================*/
  /* -1- Check the correct key                                                */
  /*==========================================================================*/
  if (t_CCsec_SecurityCtrl.Resp_U.ULong_ST.ULong0_UL == t_CCsec_SecurityCtrl.Key_U.ULong_ST.ULong0_UL)
  {
    /*========================================================================*/
    /* -1- Check the correct key                                                */
    /*========================================================================*/
    t_Result_UB = d_CCsecaudi_AccessGranted;
  }
  else
  {
    /*========================================================================*/
    /* -1- Check the correct key                                                */
    /*========================================================================*/
    t_Result_UB = d_CCsecaudi_AccessDenied;
  }  

  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCsecaudi_CheckKey_UB' -+-+-+-+-+*/ /*l_int !e818 */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
