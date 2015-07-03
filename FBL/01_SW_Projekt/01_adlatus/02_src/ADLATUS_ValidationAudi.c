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
|  Filename:  ADLATUS_Validation.c                                             |
|                                                                              |
|  Comment:   This file includes all interface functions for checksum          |
|             calculations and system validation.                              |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   RS         Ralf Schmidgall           SMART Electronic Development GmbH     |
|   KA         Markus Karas              SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ValidationAudi.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_ValidationAudi.c $
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
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_ValHandler_cdf.h"
/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"               /* must be the first include     */
#include "ADLATUS_ValHandler_tdf.h"          /* Prototypes for the check Fkts */
#include "ADLATUS_ValidationAudi_tdf.h"      /* Prototypes for the modul      */
#include "ADLATUS_Timer_tdf.h"               /* Prototypes for Timer Fkts     */
#include "ADLATUS_NvmHandler_tdf.h"          /*                               */


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

  /* max number of bytes to read from eeprom at one time     */
#define d_CCvalaudi_MaxReadBytesEeprom    0x0010u
                                               

/*----------------------------------------------------------------------------*/
/* LoopCounter for Function FUN_CCvalaudi_CalcCsumFlashDownload_UB            */
/*----------------------------------------------------------------------------*/
#define d_CCvalaudi_ResponsePendingTime       0x09C4ul  /* [-] */

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
VALCONTROL_ST      CCValAudi_WorkingInfoInt_ST;
VALCONTROL_ST      CCValAudi_Eeprom_ST;
UBYTE              t_FlashCheckState_UB;

UBYTE              t_NVMResult_UB;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ==================================+======================================== */
UBYTE FUN_CCvalaudi_CalcCsumEepromInternal_UB ( VALCONTROL_ST *t_Parameters_PST);

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
|  Function Name:  FUN_CCval_InitCalcChecksumFlashDownload_UB                  |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates the checksum after a download      |
|                  into Flash memory.                                          |
|                                                                              |
|  Parameter:      ULONG  t_StartAddress_UL                                    |
|                  ULONG  t_StartAddress_UL                                    |
|                  ULONG  t_Checksum_UL                                        |
|                                                                              |
|  Return Value:   d_CCglo_NoError       .. 0x00                               |
|                  d_CCglo_ChecksumError .. 0xFF                               |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCvalaudi_InitCalcCsumFlashDownload_UB ( ULONG  t_StartAddress_UL, \
                                                   ULONG  t_StopAddress_UL,    \
                                                   ULONG  t_ChecksumAddress_UL )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  CCValAudi_WorkingInfoInt_ST.StartAddress_PBUF    = (VHND_PTR_BUF_UBYTE) t_StartAddress_UL;     /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.EndAddress_PBUF      = (VHND_PTR_BUF_UBYTE) t_StopAddress_UL;      /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.ChecksumAddress_PBUF = (VHND_PTR_BUF_UBYTE) t_ChecksumAddress_UL;  /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.WorkingBuffer_UL     = 0x00000000ul;

  t_Result_UB = FUN_CCVhnd_InitCtrlST_UB (&CCValAudi_WorkingInfoInt_ST, FUN_CCval_CRC32_UB);

  if(d_CCglo_Ready == t_Result_UB)
  {
      t_Result_UB = d_CCglo_Busy ;
      t_FlashCheckState_UB = d_CCglo_Busy;      
  }
  else 
  {
    t_Result_UB = d_CCglo_Error;
    t_FlashCheckState_UB = d_CCglo_Error;
  }


  return (t_Result_UB);

} /*-+- END OF FUNCTION FUN_CCvalaudi_InitCalcCsumFlashDownload_UB -+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCvalaudi_CalcCsumFlashDownload_UB                      |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates the checksum after a download      |
|                  into Flash memory.                                          |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   d_CCglo_NoError       .. 0x00                               |
|                  d_CCglo_ChecksumError .. 0xFF                               |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCvalaudi_CalcCsumFlashDownload_UB (void )
{
  ULONG t_LoopCounter_UL;

  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/

  /* response pending time in ms */
  t_LoopCounter_UL = d_CCvalaudi_ResponsePendingTime;
  
  t_FlashCheckState_UB = d_CCglo_Busy;
  /*==========================================================================*/
  /* -1- Calculate checksum                                                   */
  /*     but interrupt the calculation for checking communication             */
  /*==========================================================================*/
  while ((t_LoopCounter_UL != 0x00000000ul) && (t_FlashCheckState_UB == d_CCglo_Busy)) 
  {
    /*------------------------------------------------------------------------*/
    /* Calculate Checksum                                                     */
    /*------------------------------------------------------------------------*/

    /* Returns Busy as long as Calculation is not finished */
    t_FlashCheckState_UB = FUN_CCVhnd_CalcChksumChunk_UB();

    /*========================================================================*/
    /* Time to check the system clock                                         */
    /*========================================================================*/

    FUN_CCtim_SystemTimerCheck_V ();
    t_LoopCounter_UL --;
  }


  return (t_FlashCheckState_UB);

} /*-+- END OF FUNCTION FUN_CCvalaudi_CalcCsumFlashDownload_UB      +-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCvalaudi_CalcCsumRamDownload_UB                        |
|                                                                              |
|  Prototype at:   ADLATUS_ValidationAudi_tdf.h                                |
|                                                                              |
|  Description:    This function calculates the checksum after a download      |
|                  into RAM.                                                   |
|                                                                              |
|  Parameter:      ULONG  t_StartAddress_UL                                    |
|                  ULONG  t_StartAddress_UL                                    |
|                  ULONG  t_ChecksumAddress_UL                                 |
|                                                                              |
|  Return Value:   d_CCglo_NoError       .. 0x00                               |
|                  d_CCglo_ChecksumError .. 0xFF                               |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCvalaudi_CalcCsumRamDownload_UB ( ULONG  t_StartAddress_UL,   \
                                             ULONG  t_StopAddress_UL,    \
                                             ULONG  t_ChecksumAddress_UL )
{
  UBYTE  t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Init local variables                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB                 = d_CCglo_NoError;

  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  CCValAudi_WorkingInfoInt_ST.StartAddress_PBUF    = (FAR_PTR_UBYTE) t_StartAddress_UL;     /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.EndAddress_PBUF      = (FAR_PTR_UBYTE) t_StopAddress_UL;      /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.ChecksumAddress_PBUF = (FAR_PTR_UBYTE) t_ChecksumAddress_UL;  /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.WorkingBuffer_UL     = 0x00000000ul;

  t_Result_UB = FUN_CCVhnd_InitCtrlST_UB (&CCValAudi_WorkingInfoInt_ST, FUN_CCval_CRC32_UB);

  /*--------------------------------------------------------------------------*/
  /* Call Checksum Routine                                                    */
  /*--------------------------------------------------------------------------*/
  if(d_CCglo_Ready == t_Result_UB)
  {
    t_Result_UB = FUN_CCVhnd_CalcChecksum_UB();

    /* Mapping of return values for compatibility */
    if ((d_CCVhnd_CheckNOK == (d_CCVhnd_CheckNOK & t_Result_UB)) ||
      (d_CCVhnd_Error == (d_CCVhnd_Error & t_Result_UB)))
    {
      t_Result_UB = d_CCglo_ChecksumError;
      t_FlashCheckState_UB = d_CCglo_Error;
    }
    else
    {
      t_Result_UB = d_CCglo_NoError ;
      t_FlashCheckState_UB = d_CCglo_Busy;
    }
  }
  else 
  {
    t_Result_UB = d_CCglo_Error;
    t_FlashCheckState_UB = d_CCglo_Error;
  }
  
    return  t_Result_UB;

} /*-+- END OF FUNCTION FUN_CCvalAudi_CalcCsumRamDownload_UB +-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCvalaudi_InitCalcCsumEepromDownload_UB                 |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates the checksum after a download      |
|                  into Eeprom memory.                                         |
|                                                                              |
|  Parameter:      ULONG  t_StartAddress_UL                                    |
|                  ULONG  t_StartAddress_UL                                    |
|                  ULONG  t_Checksum_UL                                        |
|                                                                              |
|  Return Value:   d_CCglo_NoError       .. 0x00                               |
|                  d_CCglo_ChecksumError .. 0xFF                               |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCvalaudi_InitCalcCsumEepromDownload_UB ( ULONG  t_StartAddress_UL, \
                                               ULONG  t_StopAddress_UL,    \
                                               ULONG  t_ChecksumAddress_UL )
{
  UBYTE t_Result_UB;

  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  CCValAudi_WorkingInfoInt_ST.StartAddress_PBUF    = (VHND_PTR_BUF_UBYTE) t_StartAddress_UL;    /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.EndAddress_PBUF      = (VHND_PTR_BUF_UBYTE) t_StopAddress_UL;     /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.ChecksumAddress_PBUF = (VHND_PTR_BUF_UBYTE) t_ChecksumAddress_UL; /*l_int !e923 */
  CCValAudi_WorkingInfoInt_ST.WorkingBuffer_UL     = 0x00000000ul;

  /*--------------------------------------------------------------------------*/
  /* Init local EEPROM struct                                                 */
  /*--------------------------------------------------------------------------*/
  CCValAudi_Eeprom_ST.ChecksumAddress_PBUF = (VHND_PTR_BUF_UBYTE) t_ChecksumAddress_UL;         /*l_int !e923 */
  CCValAudi_Eeprom_ST.WorkingBuffer_UL     = 0x00000000ul;
  CCValAudi_Eeprom_ST.Control_UB           = d_CCVhnd_InitControlFlags;

  /*--------------------------------------------------------------------------*/
  /* Init EEPROM error variable                                               */
  /*--------------------------------------------------------------------------*/
  t_NVMResult_UB = d_CCglo_NoError;
  
  t_Result_UB = FUN_CCVhnd_InitCtrlST_UB (&CCValAudi_WorkingInfoInt_ST, FUN_CCvalaudi_CalcCsumEepromInternal_UB);

  if(d_CCglo_Ready == t_Result_UB) 
  {
    t_Result_UB = d_CCglo_NoError;
    t_FlashCheckState_UB = d_CCglo_Busy;
  }
  else 
  {
    t_Result_UB = d_CCglo_Error;
    t_FlashCheckState_UB = d_CCglo_Error;
  }

  return (t_Result_UB);

} /*-+- END OF FUNCTION FUN_CCvalaudi_InitCalcCsumEepromDownload_UB +-+-+-*/



/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCvalaudi_CalcCsumEepromInternal_UB                     |
|                                                                              |
|  Prototype at:   this file                                                   |
|                                                                              |
|  Description:    This function splits the epprom access into pieces and      |
|                  gives a buffer to the real checksum function.               |
|                                                                              |
|  Parameter:      VALCONTROL_ST  *t_Parameters_PST                            |
|                                        parameters of checksum calculation    |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB    return value of the checksum function  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCvalaudi_CalcCsumEepromInternal_UB ( VALCONTROL_ST  *t_Parameters_PST )
{
  UBYTE        t_Result_UB;           /* Result of check routine             */
  NVMACCESS_ST t_NVMDirectAccess_ST;  /* sturcture for the eeprom access     */
  UWORD        t_BytesToRead_UW;
  UBYTE        t_Data_AUB[d_CCvalaudi_MaxReadBytesEeprom];
  ULONG        t_tempStartAddress_UL;
  ULONG        t_tempEndAddress_UL;

  t_Result_UB = d_CCVhnd_Init;
  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & t_Parameters_PST->Control_UB) )
  {
    CCValAudi_Eeprom_ST.Control_UB |= d_CCVhnd_CheckForResult;

    t_Result_UB = FUN_CCval_CRC32_UB (&CCValAudi_Eeprom_ST);

    /*========================================================================*/
    /* -2- NVM access was not successful                                      */
    /*========================================================================*/
    if (d_CCglo_NoError != t_NVMResult_UB)
    {
      /*----------------------------------------------------------------------*/
      /* set error as result                                                  */
      /*----------------------------------------------------------------------*/
      t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
    }

  }
  /*==========================================================================*/
  /* -1- do calculation                                                       */
  /*==========================================================================*/
  else
  {

    /*------------------------------------------------------------------------*/
    /* init the structure                                                     */
    /*------------------------------------------------------------------------*/
    t_NVMDirectAccess_ST.Data_PUB = &t_Data_AUB[0];

    t_tempStartAddress_UL = (ULONG)t_Parameters_PST->StartAddress_PBUF; /*l_int !e923 */
    t_tempEndAddress_UL   = (ULONG)t_Parameters_PST->EndAddress_PBUF;   /*l_int !e923 */

    CCValAudi_Eeprom_ST.StartAddress_PBUF = &t_Data_AUB[0];         /*l_int !e789 */

    /*==========================================================================*/
    /* -1- Calculate checksum                                                   */
    /*     but interrupt the calculation for checking communication             */
    /*==========================================================================*/
    while ( t_tempStartAddress_UL < t_tempEndAddress_UL )
    {
      /*------------------------------------------------------------------------*/
      /* Set the actual address for the eeprom access                           */
      /*------------------------------------------------------------------------*/
      t_NVMDirectAccess_ST.Address_UW = (UWORD) (t_tempStartAddress_UL);

      /*------------------------------------------------------------------------*/
      /* calculate the number of byte to be read out of the eeprom              */
      /*------------------------------------------------------------------------*/
      t_BytesToRead_UW = (UWORD) ((t_tempEndAddress_UL - t_tempStartAddress_UL) + 1ul);

      /*========================================================================*/
      /* -2- number of bytes to read is greater than maximum number             */
      /*========================================================================*/
      if (t_BytesToRead_UW > d_CCvalaudi_MaxReadBytesEeprom )
      {
        /*----------------------------------------------------------------------*/
        /* limit the number of bytes to read to d_CCvalaudi_MaxReadBytesEeprom  */
        /*----------------------------------------------------------------------*/
        t_BytesToRead_UW = d_CCvalaudi_MaxReadBytesEeprom;

      } /* -2- END OF if (..) --------------------------------------------------*/
    
      /*------------------------------------------------------------------------*/
      /* init the structure                                                     */
      /*------------------------------------------------------------------------*/
      t_NVMDirectAccess_ST.NbrBytes_UB = (UBYTE) t_BytesToRead_UW;
    
      /*------------------------------------------------------------------------*/
      /* call the eeprom access                                                 */
      /*------------------------------------------------------------------------*/
      t_NVMResult_UB |= FUN_CCnvm_NvmDirectReadAccess_UB (t_NVMDirectAccess_ST);

      /* CCValAudi_Eeprom_ST.EndAddress_PBUF isn't in use outside this function */
      CCValAudi_Eeprom_ST.EndAddress_PBUF = ((&t_Data_AUB[0] + t_BytesToRead_UW) - 1); /*l_int !e789 */

      t_Result_UB = FUN_CCval_CRC32_UB (&CCValAudi_Eeprom_ST);

      /*------------------------------------------------------------------------*/
      /* Increment Address                                                      */
      /*------------------------------------------------------------------------*/
      t_tempStartAddress_UL += (ULONG) t_BytesToRead_UW;

    } /* ---1- END OF while (..) -----------------------------------------------*/
  
  }

  return (t_Result_UB);

}  /*l_int !e818 */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
