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
|  Filename:  ADLATUS_Eep.c                                                    |
|                                                                              |
|  Interface between SSI-Handler and Low-Level EEPROM Driver                   |
|  The interface is for NVM Devices with two byte addressing                   |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Eep.c 4477 2008-11-27 12:11:59Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_Eep.c $
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

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_SSIHandler_tdf.h"

#include  "ADLATUS_Eep.h"
#include  "ADLATUS_Timer_tdf.h"           /* .. SW Timer handling              */

#include "dtyp.h"                                                              
#include "lld.h"
#include "lld_SPI.h"

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
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define EEPROM_NORMAL_LENGTH  


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
|  Function Name:  FUN_BioEepBusy                                              |
|                                                                              |
|  Prototype at:   ADLATUS_Eep.h                                               |
|                                                                              |
|  Description:    Reads and returns the busy status of the EEPROM;            |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           | 
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_BioEepBusy(void)
{

   UBYTE t_returnvalue_UB;

   /* Check if EEPROM is ready */
   /* Insert status read command in transmit data queue */
   t_returnvalue_UB = 0x00;


   /* Check if SPI interface is busy */
   if (OCSPI_Busy() == IO_E_BUSY)
   {
      /* SPI interface is not free for EEPROM access */
      t_returnvalue_UB = EEPROM_STATUS_WIP; 
      /* Tell that EEPROM is busy */
   }

   /* Return write in progress bit of EEPROM; 
      it is set when EEPROM is busy writing data */
   return(t_returnvalue_UB );

}
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_BioEepInit                                              |
|                                                                              |
|  Prototype at:   ADLATUS_Eep.h                                               |
|                                                                              |
|  Description:    Reads and returns the busy status of the EEPROM;            |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           | 
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_BioEepInit(void)
{

  OCSPI_InitSync((const void*)0);

}
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_BioEepRead                                              |
|                                                                              |
|  Prototype at:   ADLATUS_Eep.h                                               |
|                                                                              |
|  Description:    reads data bytes from the EEPROM                            |
|                  ensure that EEPROM is not busy (check with BioEepBusy()     |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           | 
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_BioEepRead(UBYTE *EepBuffer, UWORD EepAddress, UBYTE DataLength)
{

   ULONG temp;
   UBYTE buffer[0x44];

     /* EEPROM must be ready; should have be checked by calling BioEepBusy() */

     /* Insert read command in transmit data queue */
     buffer[0] = EEPROM_COM_READ;
     /* Insert start address in transmit data queue */
   
#ifdef EEPROM_NORMAL_LENGTH
     buffer[1] = (UBYTE)(EepAddress >> 8);

     /* Insert start address in transmit data queue */
     buffer[2] = (UBYTE)EepAddress; 
#else     
     if (EepAddress >= 0x100u)
     {
        buffer[0] |= 0x08u;
     }
     buffer[1] = (UBYTE)EepAddress;
#endif
     /* trigger timing as long the  EEPROM is not accessable */
     /* otherwise a watchdog reset will occur */
     do
     {
       FUN_CCtim_SystemTimerCheck_V ();
     }
     while ( EEPROM_STATUS_WIP == (EEPROM_STATUS_WIP & FUN_BioEepBusy()) );
   
     /* Read datablock from EEPROM; number of bytes is DataLength + command(1) + address(1) */
     OCSPI_ReadSync((UWORD)0u,buffer,(UWORD)((UWORD)DataLength + (UWORD)EEPROM_DATA_OFFSET));
 
     /* Copy data from SPI to destination */
     for (temp = 0ul; temp < (ULONG)DataLength; temp++)
     {
       *(EepBuffer + temp) = buffer[temp + (ULONG) EEPROM_DATA_OFFSET];
     }

   return(DataLength);
}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_BioEepWrite                                             |
|                                                                              |
|  Prototype at:   ADLATUS_Eep.h                                               |
|                                                                              |
|  Description:    writes data bytes to the EEPROM                             |
|                  ensure that EEPROM is not busy (check with BioEepBusy()     |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           | 
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_BioEepWrite(UBYTE *EepBuffer, UWORD EepAddress, UBYTE DataLength_UB)
{
  UWORD PageRest_UW;
  UBYTE t_result_UB;
  UBYTE counter_UB;
  UBYTE Status_UB;
  UBYTE buffer[0x44];

  t_result_UB = DataLength_UB;
 
  /* !! Achtung !! Beim Page-Write darf nicht über Pagegrenzen */
  /* geschrieben werden !!!                                    */
  PageRest_UW =(UWORD) (EepAddress % WR_PAGESIZE); /* Rest Page */
  PageRest_UW =(UWORD) (WR_PAGESIZE - PageRest_UW);

  do
  {
    if(PageRest_UW > (UWORD)DataLength_UB)
    {
      PageRest_UW = DataLength_UB;
    }
 
    DataLength_UB -= (UBYTE)PageRest_UW;

    /* Insert write enable command in transmit data queue */
    buffer[0] = EEPROM_COM_WREN;

    OCSPI_WriteSync((UWORD)0u,buffer,(UWORD)1u);

    do
    {
      FUN_CCtim_SystemTimerCheck_V ();    
    }
    while ( EEPROM_STATUS_WIP == (EEPROM_STATUS_WIP & FUN_BioEepBusy()) );

    /* Insert write command in transmit data queue */
    buffer[0] = EEPROM_COM_WRITE;

    buffer[1] = (UBYTE)(EepAddress >> 8);
    buffer[2] = (UBYTE)EepAddress;

    for (counter_UB = 0; counter_UB < (UBYTE)PageRest_UW; counter_UB++) /* misra */
    {
      buffer[counter_UB+(UBYTE)EEPROM_DATA_OFFSET] = *(EepBuffer + counter_UB);	
    }

    OCSPI_WriteSync((UWORD)0u,buffer,(UWORD)(PageRest_UW + EEPROM_DATA_OFFSET));

    do
    {
      for (counter_UB=0;counter_UB<0x20;counter_UB++)
      {
        FUN_CCtim_SystemTimerCheck_V ();
      }

      Status_UB = FUN_BioEepBusy();     
    }
    while (Status_UB == EEPROM_STATUS_WIP);

    EepAddress += PageRest_UW; /* Auf die PageGrenze setzen */
    EepBuffer  += PageRest_UW;  

    PageRest_UW = WR_PAGESIZE;
  }
  while (DataLength_UB > 0); 

  return t_result_UB;
}

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

