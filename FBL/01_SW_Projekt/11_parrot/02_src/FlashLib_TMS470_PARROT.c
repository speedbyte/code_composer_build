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
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: FlashLib_TMS470_PARROT.c 4950 2009-03-06 08:40:25Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20090305_S1NN_TMS470R_ParrotDriver_Hotfix/01_SW_Projekt/11_parrot/02_src/FlashLib_TMS470_PARROT.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S                     */
/* ========================================================================== */
#include "ADLATUS_CompilerSwitches_cdf.h"       /* must be the first include   */
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_PARHal_cdf.h"
#include "ADLATUS_SCIHandler_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_PARHal_tdf.h"
#include "ADLATUS_SCIHandler_tdf.h"

#include "ADLATUS_HISFlash.h"
#include "tms470regs.h"

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
/* Speedcode defines */
#define d_Parrot_SpeedCode_Invalid  (UBYTE) 0xA1u
#define d_Parrot_SpeedCode_115200   (UBYTE) 0xA3u
#define d_Parrot_SpeedCode_230400   (UBYTE) 0xA5u
#define d_Parrot_SpeedCode_460800   (UBYTE) 0xA7u
#define d_Parrot_SpeedCode_921600   (UBYTE) 0xA9u
#define d_Parrot_SpeedCode_Confirm  (UBYTE) 0xA8u
#define d_Parrot_SpeedCode_Decline  (UBYTE) 0xAAu
#define d_Parrot_Speed_115200       (ULONG)(115200ul)
#define d_Parrot_Speed_230400       (ULONG)(230400ul)

/* error Code Parrot – HIS */
#define d_Parrot_ProgramSuccessful  (UBYTE) 0x55u /*download and programming is done and OK */
#define d_Parrot_E_BadChecksum      (UBYTE) 0xB0u /* bad checksum */
#define d_Parrot_E_NotEnoughBytes   (UBYTE) 0xB1u /* not enough bytes received */
#define d_Parrot_E_Flash            (UBYTE) 0xB2u /* error in flash */
#define d_Parrot_E_Device           (UBYTE) 0xB3u /* not targeted device */
#define d_Parrot_E_OutOfMemory      (UBYTE) 0xB4u /* not enough memory */
#define d_Parrot_E_BytesOverflow    (UBYTE) 0xB5u /* too many bytes received */
#define d_Parrot_E_PLF              (UBYTE) 0xB6u /* error in plf */
#define d_Parrot_E_Address          (UBYTE) 0xB7u /* forbidden flash address */

/* download defines */
/* depending on update behavior the downloadlength is 2 or 4 bytes long       */
#define d_Parrot_DownloadLengthBoot (UBYTE) 0x02u /* length of download in 2 Bytes    */
#define d_Parrot_DownloadLengthApp  (UBYTE) 0x04u /* length of download in 4 Bytes    */

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/*----------------------------------------------------------------------------*/
/* Basic defines                                                              */
/*----------------------------------------------------------------------------*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */

SCIINTERFACE_ST c_CCconsys_SciInterface_ST;

tFlashParam *fp;
/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
void Feed_Watchdog_V(void);

void TriggerWatchdog(void);


/*----------------------------------------------------------------------------*/
/*                     HIS-Header                                             */
/* must be linked as ROM constants but located into RAM area                  */
/*----------------------------------------------------------------------------*/

const tHis_Header HisHeader =
{
    FLASH_DRIVER_VERSION_INTERFACE_PARROT,
    0x00,
    FLASH_DRIVER_VERSION_MASKTYPE_PARROT,
    FLASH_DRIVER_VERSION_MCUTYPE_PARROT,
    &PARROT_Init,
    &PARROT_Deinit,
    &PARROT_Erase,
    &PARROT_Write
} ;

volatile UBYTE t_txbuffer_UB;
volatile UBYTE t_rxbuffer_UB;

/* modul variable to hold blocksize */
UWORD t_ParrotBlockSize_UW;
UWORD t_BootloadersCrc_UW;
ULONG t_ParrotLength_UL;
ULONG t_ParrotActLength_UL;



/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  FlashInit                                                   |
|                                                                              |
|  Prototype at:   FlashLib_TMS470.h                                           |
|  Description:    Initialization of the flash parameter structure.            |
|  Parameter:      -                                                           |
|  Return Value:   void                                                        |
+-----------------------------------------------------------------------------*/
void PARROT_Init (tFlashParam *flashParam)
{

  ULONG t_tempbuffer_UL;
  UBYTE t_endlessLoopProtection_UB;
  UBYTE t_index_UB;
  fp = flashParam;

  /*==========================================================================*/
  /* -1- Initialize Flash params                                              */
  /*==========================================================================*/
  flashParam->majornumber  = FLASH_DRIVER_VERSION_MAJOR;
  flashParam->minornumber  = FLASH_DRIVER_VERSION_MINOR;
  flashParam->patchlevel   = FLASH_DRIVER_VERSION_PATCH;
  flashParam->reserved1    = 0x00;
  flashParam->errorcode    = d_CCpar_FlashOK;
  flashParam->data         = 0x00;
  /* Watchdog address will be written by KWP erase memory function */
  /*flashParam->wdTriggerFct = 0x00000000;*/ /* (tWDTriggerFct)FlashExtWatchdog_V; */
  flashParam->Direction_UB    = 0;  /* direction byte */
  flashParam->Format_UB       = 0;  /* format byte */
//  flashParam->Millisec_UB      = 0;  /* alignment byte */

  flashParam->StartSector_US  = 0;  /* command byte */
  flashParam->EndSector_US    = 0;  /* command byte */
  flashParam->NbrOfSectors_US = 0;  /* command byte */
  flashParam->ActualSector_US = 0;  /* command byte */

  /* get blocksize */
  t_ParrotBlockSize_UW = flashParam->address;
  t_BootloadersCrc_UW = 0x0000u;
  t_ParrotLength_UL = flashParam->length;
  t_ParrotActLength_UL = 0x0000ul;
  /* this hole block, UART init and pining is only done when downloading the */
  /* updater or flasher. */
  if (FALSE != flashParam->Command_UB)
  {
    FUN_PARhal_InitHardwareTimer_V(flashParam->errorAddress);

    /*==========================================================================*/
    /* -1- Initialize the SCIHandler                                            */
    /*==========================================================================*/
    c_CCconsys_SciInterface_ST.IntClkFrequency_UL = &(flashParam->Delaytime_UL);
    t_tempbuffer_UL = d_Parrot_Speed_115200;
    c_CCconsys_SciInterface_ST.DefaultBaudrate_UL = (FAR_PTR_ULONG)&t_tempbuffer_UL;
    FUN_PARsci_Init_V(&c_CCconsys_SciInterface_ST);

    /*==========================================================================*/
    /* -1- Initialize the SCI2 modul                                            */
    /*==========================================================================*/
    FUN_PARsci_InitInternal_V();
    Feed_Watchdog_V();
    /*==========================================================================*/
    /* -1- Set Parrot to bootmode                                               */
    /*==========================================================================*/
    e_GIO_ST.Port_ST[3].Dclr_UL = 0x0004ul; /* NRESET low */
    FUN_PARhal_WaitTime_V((UWORD)1u,d_PARhal_UnitMS_B);
    e_GIO_ST.Port_ST[3].Dclr_UL = 0x0002ul; /* BOOTS low */
    FUN_PARhal_WaitTime_V((UWORD)10u,d_PARhal_UnitMS_B);
    e_GIO_ST.Port_ST[3].Dset_UL = 0x00004ul; /* NRESET high */
    FUN_PARhal_WaitTime_V((UWORD)11u,d_PARhal_UnitMS_B);
    e_GIO_ST.Port_ST[3].Dset_UL = 0x00002ul; /* BOOTS high */
  }


  /*==========================================================================*/
  /* -1- send speedcode to parrot and receive response                        */
  /*==========================================================================*/
  /* fastest possible baudrate with 24MHz CPU freq*/
  t_txbuffer_UB = d_Parrot_SpeedCode_115200;
  t_rxbuffer_UB = (UBYTE)0x00u;
  /* setup loop counter to allow 254 tries */
  t_endlessLoopProtection_UB = (UBYTE) 0xFEu;
  while ((t_rxbuffer_UB != d_Parrot_SpeedCode_Confirm)&&\
        (t_endlessLoopProtection_UB != (UBYTE) 0x00u))
  {
    Feed_Watchdog_V();
    /* decrease loop counter */
    t_endlessLoopProtection_UB--;
    /* Send speed code */
    while(d_CCglo_Ready!=FUN_PARsci_SendByte_UB((PTR_BUFFER_UB)&t_txbuffer_UB))
    {}
    /* wait 100ms */
    FUN_PARhal_WaitTime_V((UWORD)100u,d_PARhal_UnitMS_B);
    /* receive byte */
    FUN_PARsci_ReceiveByte_UB((PTR_BUFFER_UB)&t_rxbuffer_UB);
    Feed_Watchdog_V();
  }

  /* parrot response received or endless loop hit */
  if (((UBYTE) 0x00u == t_endlessLoopProtection_UB) \
    || (t_rxbuffer_UB != d_Parrot_SpeedCode_Confirm))
  {
    flashParam->errorcode = d_CCpar_AccessError;
  }
  else
  {
    t_rxbuffer_UB = (UBYTE) 0xFFu;
    /* Bootmode activ, next send length of downloadfile */
    if (FALSE != flashParam->Command_UB)
    {
      /* for bootloader download length is 2 bytes long */
      for (t_index_UB=0x00u;t_index_UB < d_Parrot_DownloadLengthBoot;t_index_UB++)
      {
        t_txbuffer_UB = (UBYTE)((flashParam->length>>(t_index_UB*8u))&0xFFu);
        /* calc checksum over length */
        t_BootloadersCrc_UW += (UWORD)t_txbuffer_UB;

        while(d_CCglo_Ready!=FUN_PARsci_SendByte_UB((PTR_BUFFER_UB)&t_txbuffer_UB))
        {}
        Feed_Watchdog_V();
        FUN_PARsci_ReceiveByte_UB ((PTR_BUFFER_UB)&t_rxbuffer_UB);
        /* if parrot responses, break the loop */
        if ((UBYTE) 0xFFu != t_rxbuffer_UB)
        {
          t_index_UB = d_Parrot_DownloadLengthBoot;
        }
      }
    }
    else
    {
      /* for application download length is 4 bytes long */
      for (t_index_UB=0x00u;t_index_UB < d_Parrot_DownloadLengthApp; t_index_UB++)
      {
        t_txbuffer_UB = (UBYTE)((flashParam->length>>(t_index_UB*8u))&0xFFu);

        while(d_CCglo_Ready!=FUN_PARsci_SendByte_UB((PTR_BUFFER_UB)&t_txbuffer_UB))
        {}
        Feed_Watchdog_V();
        FUN_PARsci_ReceiveByte_UB ((PTR_BUFFER_UB)&t_rxbuffer_UB);
        /* if parrot responses, break the loop */
        if ((UBYTE) 0xFFu != t_rxbuffer_UB)
        {
          t_index_UB = d_Parrot_DownloadLengthApp;
        }
      }
    }

    /*  evalutate parrot possible response */
    if ((UBYTE) 0xFFu  != t_rxbuffer_UB)
    {
      flashParam->errorcode = d_CCpar_AccessError;
    }
  }
}

/*-------------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                     |
+--------------------------------------------------------------------------------+
|  Function Name:  FlashDeinit                                                   |
|                                                                                |
|  Prototype at:   FlashLib_TMS470.h                                             |
|  Description:    Clearing of flash parameter structure after Flash erase/write |
|  Parameter:      -                                                             |
|  Return Value:   void                                                          |
+-------------------------------------------------------------------------------*/
void PARROT_Deinit (tFlashParam *flashParam)
{
  flashParam->majornumber = 0x00u;
  flashParam->minornumber = 0x00u;
  flashParam->patchlevel  = 0x00u;
  flashParam->errorcode   = 0x00u;
  flashParam->address     = 0x00u;
  flashParam->length      = 0x00u;
  flashParam->data        = 0x00u;
  flashParam->wdTriggerFct = 0x00000000ul;

  /* reset parrot module */
  /*e_GIO_ST.Port_ST[3].Dclr_UL = 0x0004u;*/    /* NRESET low */
  /*FUN_PARhal_WaitTime_V((UWORD)1,d_PARhal_UnitMS_B);*/ /* low time must be above 50us */
  /*e_GIO_ST.Port_ST[3].Dset_UL = 0x00004u;*/   /* NRESET high */
}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  FlashErase                                                  |
|                                                                              |
|  Prototype at:   FlashLib_TMS470.h                                           |
|  Description:    This function actually erases the Flash according to        |
|                  values in flashParam structure				                       |
|  Parameter:      -                                                           |
|  Return Value: flashParam->errorcode = kFlashOK (= 0x00)     Erase succesful |
|                flashParam->errorcode = kFlashFailed (= 0x01) Erase timeout   |
+-----------------------------------------------------------------------------*/
void PARROT_Erase (tFlashParam *flashParam)
{
  flashParam->errorcode = kFlashOK;
  return;
}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  FlashWriteData                                              |
|                                                                              |
|  Prototype at:   This file				                                           |
|  Description:    This function sends one Byte via UART to the parrot modul.  |
|                  If the Downloadfile is a bootloader an CRC is calculated.   |
|                  After send 1 Byte the UART will be read for an answer. All  |
|                  response unequal 0x55 will force flash abort.               |
|  Parameter:      flashParam->length                                          |
|                  flashParam->data                                            |
|                                                                              |
|  Return Value:   flashParam->errorcode = kFlashOK      Erase succesful       |
|                  flashParam->errorcode = kFlashFailed  Erase timeout         |
+-----------------------------------------------------------------------------*/
void PARROT_Write(tFlashParam *flashParam)
{
  UWORD t_index_UW;
  UBYTE t_endlessLoopProtection_UB;
  UBYTE t_return_read_UB = d_CCglo_Idle;

  t_rxbuffer_UB = 0x00u;
  /* send all byte on UART */
  for (t_index_UW=0x00u;((t_index_UW < flashParam->length)\
      &&(t_return_read_UB == d_CCglo_Idle));t_index_UW++)
  {
    /* download bootloader ? */
    if (FALSE != flashParam->Command_UB)
    {
      /* calc checksum */
      t_BootloadersCrc_UW += (UWORD)*((flashParam->data) + t_index_UW);
    }

    while(d_CCglo_Ready!=FUN_PARsci_SendByte_UB(\
        (PTR_BUFFER_UB)(flashParam->data) + t_index_UW))
        {}
    Feed_Watchdog_V();

    t_return_read_UB = FUN_PARsci_ReceiveByte_UB((PTR_BUFFER_UB)&t_rxbuffer_UB);
  }

  /* loop end because of response from parrot received */
  if (d_CCglo_Ready == t_return_read_UB)
  {
    flashParam->errorcode = d_CCpar_DefaultError;
    switch(t_rxbuffer_UB)
    {
      case d_Parrot_E_BadChecksum: /* bad checksum */
      {
        flashParam->errorcode = d_CCpar_VerificationError;
        break;
      }
      case d_Parrot_E_NotEnoughBytes:/* not enough bytes received */
      {
        flashParam->errorcode = d_CCpar_NotEnoughData;
        break;
      }
      case d_Parrot_E_Flash: /* error in flash */
      {
        flashParam->errorcode = d_CCpar_FlashFailed;
        break;
      }
      case d_Parrot_E_Device:/* not targeted device */
      {
        flashParam->errorcode = d_CCpar_DeviceOutOfRange;
        break;
      }
      case d_Parrot_E_OutOfMemory:/* not enough memory */
      {
        flashParam->errorcode = d_CCpar_MemoryOverflow;
        break;
      }
      case d_Parrot_E_BytesOverflow:/* too many bytes received */
      {
        flashParam->errorcode = d_CCpar_ToMuchData;
        break;
      }
      case d_Parrot_E_PLF: /* error in plf */
      {
        flashParam->errorcode = d_CCpar_AccessError;
        break;
      }
      case d_Parrot_E_Address: /* forbidden flash address */
      {
        flashParam->errorcode = d_CCpar_ProtectionViolation;
        break;
      }
      case d_Parrot_ProgramSuccessful:
      {
        flashParam->errorcode = d_CCpar_FlashOK;
        t_ParrotActLength_UL += flashParam->length;
        break;
      }
      default:
      {
        if ((t_rxbuffer_UB == (UBYTE)(t_BootloadersCrc_UW&0x00FFu))\
          &&(FALSE != flashParam->Command_UB))
        {
          flashParam->errorcode = d_CCpar_FlashOK;
          t_ParrotActLength_UL += flashParam->length;
        }
        break;
      }
    }
  }
  else
  {/* no response from parrot, every thing ist okay */
    flashParam->errorcode = d_CCpar_FlashOK;
    t_ParrotActLength_UL += flashParam->length;
  }

  Feed_Watchdog_V();
  /* a normal update ends with response 0x55 by parrot */
  if ((t_ParrotLength_UL) <= t_ParrotActLength_UL)
  {
    if (FALSE == flashParam->Command_UB)
    {
    /* setup loop counter to allow 32 tries */
      t_endlessLoopProtection_UB = 0x20u;
      while(d_CCglo_Ready != t_return_read_UB)
      {
        FUN_PARhal_WaitTime_V((UWORD)10u,d_PARhal_UnitMS_B);
        t_return_read_UB = FUN_PARsci_ReceiveByte_UB((PTR_BUFFER_UB)&t_rxbuffer_UB);
        t_endlessLoopProtection_UB--;
        Feed_Watchdog_V();
      }
      if (d_Parrot_ProgramSuccessful == t_rxbuffer_UB)
      {
        flashParam->errorcode = d_CCpar_FlashOK;
      }
      else
      {
        if (0x00 == t_endlessLoopProtection_UB)
        {
          flashParam->errorcode = d_CCpar_DefaultError;
        }
        else
        {
          flashParam->errorcode = d_CCpar_DefaultError;
        }
      }
    }
    else
    {
      /* setup loop counter to allow 32 tries */
      t_endlessLoopProtection_UB = 0x20u;
      while(d_CCglo_Ready != t_return_read_UB)
      {
        FUN_PARhal_WaitTime_V((UWORD)10u,d_PARhal_UnitMS_B);
        t_return_read_UB = FUN_PARsci_ReceiveByte_UB((PTR_BUFFER_UB)&t_rxbuffer_UB);
        t_endlessLoopProtection_UB--;
        Feed_Watchdog_V();
      }
      if (t_rxbuffer_UB == (UBYTE)(t_BootloadersCrc_UW&0x00FFu))
      {
        t_txbuffer_UB = d_Parrot_ProgramSuccessful;
        flashParam->errorcode = d_CCpar_FlashOK;
      }
      else
      {
        t_txbuffer_UB = d_Parrot_SpeedCode_Decline;
        flashParam->errorcode = d_CCpar_VerificationError;
      }
      while(d_CCglo_Ready!=FUN_PARsci_SendByte_UB((PTR_BUFFER_UB)&t_txbuffer_UB))
      {}
    }
  }

  return;
}

void Feed_Watchdog_V()
{
  ULONG *t_TempSrc_pul;

  t_TempSrc_pul  = (ULONG*)fp->wdTriggerFct;

  if (t_TempSrc_pul != 0x00000000UL)
  {
     fp->wdTriggerFct();
   }
  /* trigger internal wdt */
  e_SARRTI_ST.WKEY_UL = 0xE5;
  e_SARRTI_ST.WKEY_UL = 0xA3;
}



/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
