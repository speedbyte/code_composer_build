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
|  Filename:  ADLATUS_DataPrepareAudi.c                                        |
|                                                                              |
|  Comment:   This file includes all routines for data encryption or           |
|             decryption.                                                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_DataPrepareAudi.c 4508 2008-11-28 12:28:20Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/20100706_S1NN_TMS470S_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_DataPrepareAudi.c $
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
#include "ADLATUS_Global_cdf.h"                /* must be the first include   */
#include "ADLATUS_ConfigSystem_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_DataPrepareAudi_tdf.h"
#include "ADLATUS_ConfigProject_cdf.h" 
#include "ADLATUS_Adr_Info.h"
#include "ADLATUS_Global_tdf.h"
#include "ADLATUS_Timer_tdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches:  NO ENTRY                                         */

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

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRIES                                                                 */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

#ifdef cs_Decryption_Aktiv

  /*--------------------------------------------------------------------------*/
  /* AES Key used for decryption                                              */
  /*--------------------------------------------------------------------------*/
  const UBYTE g_DecryptKey_AUB[EscAes_KEY_BYTES] =
  {
    0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U,
    0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U
  };

#endif

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
#ifdef cs_Decryption_Aktiv

  /*--------------------------------------------------------------------------*/
  /* AES Initialisation Vector used for decryption                            */
  /*--------------------------------------------------------------------------*/
  const UBYTE g_DecryptIv_AUB[EscAes_KEY_BYTES] =
  {
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
  };

  /*--------------------------------------------------------------------------*/
  /* array used to save iv during decryption of one block                     */
  /*--------------------------------------------------------------------------*/
  UBYTE g_DecryptIvTemp_AUB[EscAes_KEY_BYTES];

#endif

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCdp_DataPrepareAudi_UB                                 |
|                                                                              |
|  Prototype at:   ADLATUS_DataPrepareAudi_tdf.h                               |
|                                                                              |
|  Description:    This routine decrypts the data given at the Encryptbuffer.  |
|                  The decrypted data are stored at the Decryptbuffer.         |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_EncryptBuffer_PBUF .. Ptr to Encrypt data   |
|                  PTR_BUFFER_UB t_DecryptBuffer_PBUF .. Ptr to Decrypt data   |
                   ULONG t_LengthInfo_UL .. Nbr of Bytes within buffer         |
|                                                                              |
|  Return Value:   UBYTE         d_CCglo_Ready  .. Encryption OK               |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE  FUN_CCdp_DataPrepareAudi_UB ( PTR_BUFFER_UB t_EncryptBuffer_PBUF, \
                                     PTR_BUFFER_UB t_DecryptBuffer_PBUF, \
                                     ULONG t_LengthInfo_UL)
{
  UBYTE t_Result_UB;
  ULONG t_Index_UL;

#ifdef cs_Decryption_Aktiv
  BOOL t_Status_B;
  ULONG t_DecryptIvIndex_UL;
#endif

  /*--------------------------------------------------------------------------*/
  /* Set return value - Index 00                                              */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex00);


#ifndef cs_Decryption_Aktiv
  /*==========================================================================*/
  /* -1- copy data                                                            */
  /*==========================================================================*/
  for (t_Index_UL = 0x00ul; t_Index_UL < t_LengthInfo_UL; t_Index_UL++)
  {
    /*------------------------------------------------------------------------*/
    /* copy data                                                              */
    /*------------------------------------------------------------------------*/
    t_DecryptBuffer_PBUF[t_Index_UL] = t_EncryptBuffer_PBUF[t_Index_UL];

    /*------------------------------------------------------------------------*/
    /* clear up the buffer                                                    */
    /*------------------------------------------------------------------------*/
    t_EncryptBuffer_PBUF[t_Index_UL] = (UBYTE) 0x00u;

  } /* -1- END OF for(..) ----------------------------------------------------*/
#else
  /*--------------------------------------------------------------------------*/
  /* initialise return value                                                  */
  /*--------------------------------------------------------------------------*/
  t_Status_B = FALSE;

  /*==========================================================================*/
  /* -1- decode one page                                                      */
  /*==========================================================================*/
  for (t_Index_UL = 0x00ul; t_Index_UL < t_LengthInfo_UL; t_Index_UL = t_Index_UL + 0x10ul )
  {
    /*------------------------------------------------------------------------*/
    /* decode                                                                 */
    /*                                                                        */
    /*      !!! definition of boolean value 'TRUE' is not the same !!!        */
    /*      !!! in the kryptolib as in the ADLATUS. Don't use it!  !!!        */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    t_Status_B |= EscAes_DecryptCbc( \
                  &g_DecryptKey_AUB[0], \
                  (const UBYTE *) &g_DecryptIvTemp_AUB[0], \
                  &t_DecryptBuffer_PBUF[t_Index_UL], \
                  &t_EncryptBuffer_PBUF[t_Index_UL], \
                  0x10ul );

    /*------------------------------------------------------------------------*/
    /* check the system timer                                                 */
    /*------------------------------------------------------------------------*/
    FUN_CCtim_SystemTimerCheck_V ();

    /*------------------------------------------------------------------------*/
    /* -2- save last 0x10 Byte of last encrypted block as new IV              */
    /*------------------------------------------------------------------------*/
    for (t_DecryptIvIndex_UL = 0x00ul; t_DecryptIvIndex_UL < (ULONG) EscAes_BLOCK_BYTES; t_DecryptIvIndex_UL++)
    {
      g_DecryptIvTemp_AUB[t_DecryptIvIndex_UL] = t_EncryptBuffer_PBUF[t_Index_UL + t_DecryptIvIndex_UL];

    } /* -2- END OF for(..) --------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* check the system timer                                                 */
    /*------------------------------------------------------------------------*/
    FUN_CCtim_SystemTimerCheck_V ();

  } /* -1- END OF for(..) ----------------------------------------------------*/

  /*==========================================================================*/
  /* -1- an error occured during decryption                                   */
  /*==========================================================================*/
  if (FALSE != t_Status_B)                          
  {
    /*------------------------------------------------------------------------*/
    /* Set return value - Index 02                                            */
    /*------------------------------------------------------------------------*/
    t_Result_UB = ( d_CCglo_Ready | d_CCglo_ResponseIndex02);

  } /* -1- END OF if(..) -----------------------------------------------------*/
#endif

  
  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCdp_DataPrepareAudi_UB' +-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
