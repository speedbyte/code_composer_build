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
|  Filename:  FlasLib_TMS470.c                                                 |
|                                                                              |
|  Comment:   This file implements the HIS Flashfunctions for the TMS470       |
|   		                             				                                   |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: FlashLib_TMS470.c 4477 2008-11-27 12:11:59Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/10_his/02_src/FlashLib_TMS470.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S                     */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include */
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_HISflash.h"
#include "tms470regs.h"

/* Defined for located the following code into ROM and compile for RAM -------*/

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/*----------------------------------------------------------------------------*/
/* Basic defines                                                              */
/*----------------------------------------------------------------------------*/
#define  d_NoEntry          (UBYTE) 0x00u
#define  FLASHTYPE_UNKNOWN  (UBYTE) 0x00u
#define  OTP                (UBYTE) 0x01u
#define  FLASH              (UBYTE) 0x02u
#define  UNKNOWN_STATUS     (UBYTE) 0x00u
#define  ERASE_SUCCESSFUL   (UBYTE) 0x01u
#define  d_Value4                   4

/*----------------------------------------------------------------------------*/
/* Core defines                                                               */
/*----------------------------------------------------------------------------*/
#define  d_Core0       (UBYTE) 0x00u
#define  d_Core1       (UBYTE) 0x01u
#define  d_Core2       (UBYTE) 0x02u
#define  d_Core3       (UBYTE) 0x03u

/*----------------------------------------------------------------------------*/
/* Sector defines    												                                  */
/*----------------------------------------------------------------------------*/
#define  d_Sector00    (UBYTE) 0x00u
#define  d_Sector01    (UBYTE) 0x01u
#define  d_Sector02    (UBYTE) 0x02u
#define  d_Sector03    (UBYTE) 0x03u
#define  d_Sector04    (UBYTE) 0x04u
#define  d_Sector05    (UBYTE) 0x05u
#define  d_Sector06    (UBYTE) 0x06u
#define  d_Sector07    (UBYTE) 0x07u
#define  d_Sector08    (UBYTE) 0x08u
#define  d_Sector09    (UBYTE) 0x09u
#define  d_Sector10    (UBYTE) 0x0Au
#define  d_Sector11    (UBYTE) 0x0Bu
#define  d_Sector12    (UBYTE) 0x0Cu
#define  d_Sector13    (UBYTE) 0x0Du
#define  d_Sector14    (UBYTE) 0x0Eu
#define  d_Sector15    (UBYTE) 0x0Fu
#define  d_Sector16    (UBYTE) 0x10u
#define  d_Sector17    (UBYTE) 0x11u

/*----------------------------------------------------------------------------*/
/* First and last Sector for erasing   TODO: lösch-BAR oder lösch-ERLAUBT ?   */
/*----------------------------------------------------------------------------*/
#define d_FirstEraseableSector    d_Sector02
#define d_LastEraseableSector     d_Sector10    /* TMS470R1VF55BB */

#define FLASH_END       0x000BFFFFUL            /* TMS470R1VF55BB */

#define d_Bank0End 0x0007FFFFUL
#define d_Bank1End 0x000BFFFFUL
/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      T Y P E - D E F I N E S                                               */
/*      --------------------------------------------------------------------- */
/*      Flash memory table                                                    */
/*                                                                            */
/* -------------------------------------------------------------------------- */
typedef struct _FLASH
{
  ULONG  Start_UL;       /*  4 Byte - Sector start address                    */
  ULONG  Length_UL;      /*  4 Byte - Sector length                           */
  UWORD  Sector_UW;      /*  2 Byte - sector information                      */
  UBYTE  Core_UB;        /*  1 Byte - Core information                        */
  UBYTE  FlashType_UB;   /*  1 Byte - ProgramFlash or DataFlash               */
                         /*-------------------------------------------------- */
} FLASH_ST;              /* 12 Byte                                           */

typedef volatile ULONG * FLASH_ARRAY_ST;

typedef struct _FLASH_STATUS_ST
{ ULONG stat1;
  ULONG stat2;
  ULONG stat3;
  ULONG stat4;

} FLASH_STATUS_ST;

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
extern tFlashParam FlashParam;

tFlashParam *fp;

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
void  Feed_Watchdog_V(void);

void  FlashWrite(tFlashParam *flashParam);

void  TriggerWatchdog(void);

UWORD GetEraseStartSector (   ULONG t_StartAddress_UL,
                              ULONG t_Length_UL);

UWORD GetEraseEndSector(      ULONG t_StartAddress_UL);

UBYTE FlashEraseSector(       const tFlashParam *flashParam);

extern BOOL Flash_Start_Command_B(ULONG *start,
                                  ULONG core,
                                  ULONG delay,
                                  FLASH_ARRAY_ST cntl,
                                  UWORD cmnd,
                                  UWORD data
                                  );

UWORD Flash_Erase_Status_U16( FLASH_ARRAY_ST cntl);

extern BOOL Flash_Prog_Data_B(ULONG *start,
                              ULONG *buff,
                              ULONG length,
                              ULONG core,
                              ULONG delay,
                              FLASH_ARRAY_ST cntl,
                              FLASH_STATUS_ST *status,
                              ULONG buflen,
                              UWORD cmnd
                              );

/*----------------------------------------------------------------------------*/
/*                     HIS-Header                                             */
/* must be linked as ROM constants but located into RAM area                  */
/*----------------------------------------------------------------------------*/
const tHis_Header HisHeader =
{
    FLASH_DRIVER_VERSION_INTERFACE,
    0x00,
    FLASH_DRIVER_VERSION_MASKTYPE,
    FLASH_DRIVER_VERSION_MCUTYPE,
    &FlashInit,
    &FlashDeinit,
    &FlashErase,
    &FlashWrite
} ;

/*----------------------------------------------------------------------------*/
/*     F L A S H                                                              */
/*     M E M O R Y   O R G A N I S A T I O N                                  */
/*     -------------------------------------------------                      */
/*     Manufacturer : TI                                                      */
/*     Memory       : TMS 470 R1 VF338/VF348 F05                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* TMS470R1SF55 */
const FLASH_ST c_CCflash_SectorMap_AST[] = { \
/*--+--------------+-------------+-----------+----------+----------*/
/*  | start        | length      | sector    |  core    | Flashtype*/
/*--+--------------+-------------+-----------+----------+----------*/
    { 0x00000000UL, 0x00001000UL, d_Sector00, d_Core0    ,FLASH},
    { 0x00004000UL, 0x00001000UL, d_Sector01, d_Core0    ,FLASH},
    { 0x00008000UL, 0x00001000UL, d_Sector02, d_Core0    ,FLASH},
    { 0x0000C000UL, 0x00001000UL, d_Sector03, d_Core0    ,FLASH},
    { 0x00010000UL, 0x00002000UL, d_Sector04, d_Core0    ,FLASH},
    { 0x00018000UL, 0x00002000UL, d_Sector05, d_Core0    ,FLASH},
    { 0x00020000UL, 0x00008000UL, d_Sector06, d_Core0    ,FLASH},
    { 0x00040000UL, 0x00008000UL, d_Sector07, d_Core0    ,FLASH},
    { 0x00060000UL, 0x00008000UL, d_Sector08, d_Core0    ,FLASH},
    
    { 0x00080000UL, 0x00008000UL, d_Sector09, d_Core1    ,FLASH},
    { 0x000A0000UL, 0x00008000UL, d_Sector10, d_Core1    ,FLASH}
};

/*----------------------------------------------------------------------------*/
/* The Flash control register                                                 */
/*----------------------------------------------------------------------------*/
const FLASH_ARRAY_ST C_FlashControl_ST = (FLASH_ARRAY_ST) 0xFFF7CC00ul;

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
void FlashInit (tFlashParam *flashParam)
{
  fp = flashParam;

  flashParam->majornumber  = FLASH_DRIVER_VERSION_MAJOR;
  flashParam->minornumber  = FLASH_DRIVER_VERSION_MINOR;
  flashParam->patchlevel   = FLASH_DRIVER_VERSION_PATCH;
  flashParam->errorcode    = kFlashOK;
  flashParam->address      = 0x00;
  flashParam->length       = 0x00;
  flashParam->data         = 0x00;
  /* Watchdog address will be written by KWP erase memory function */
  flashParam->wdTriggerFct = 0x00000000; /* (tWDTriggerFct)FlashExtWatchdog_V; */

  flashParam->StartSector_US  = 0;  /* command byte */
  flashParam->EndSector_US    = 0;  /* command byte */
  flashParam->NbrOfSectors_US = 0;  /* command byte */
  flashParam->ActualSector_US = 0;  /* command byte */

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /* -+- END OF FUNCTION 'FlashInit' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  FlashDeinit                                                 |
|                                                                              |
|  Prototype at:   FlashLib_TMS470.h                                           |
|  Description:    Clearing of flash parameter structure after Flash           |
|                  erase/write                                                 |
|  Parameter:      -                                                           |
|  Return Value:   void                                                        |
+-----------------------------------------------------------------------------*/
void FlashDeinit (tFlashParam *flashParam)
{
  flashParam->majornumber = 0x00;
  flashParam->minornumber = 0x00;
  flashParam->patchlevel  = 0x00;
  flashParam->errorcode   = 0x00;
  flashParam->address     = 0x00;
  flashParam->length      = 0x00;
  flashParam->data        = 0x00;
  flashParam->wdTriggerFct = 0x00000000ul;

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /* -+- END OF FUNCTION 'FlashDeinit' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


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
void FlashErase(tFlashParam *flashParam)
{
   ULONG ulAdr;

   volatile ULONG t_Start_UL;
   volatile ULONG t_Core_UL;
   volatile ULONG t_Delay_UL;
   volatile ULONG t_Control_UL;

   ulAdr = flashParam->address;

  /*==========================================================================*/
  /* -1- further call -> do erasing                                           */
  /*==========================================================================*/
  if ( (flashParam->errorcode) & kFlashEraseBusy )
  {
    /*------------------------------------------------------------------------*/
    /* reset error code                                                       */
    /*------------------------------------------------------------------------*/
    flashParam->errorcode = kFlashOK;

    /*------------------------------------------------------------------------*/
    /* reset milli second counter for next sector                             */
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /* erase sector                                                           */
    /*------------------------------------------------------------------------*/
    flashParam->errorcode = FlashEraseSector(flashParam);

    /*========================================================================*/
    /* -2- erasing of sector was successful                                   */
    /*========================================================================*/
    if ((UWORD)ERASE_SUCCESSFUL == flashParam->errorcode)
    {
      /*----------------------------------------------------------------------*/
      /* reset errorcode                                                      */
      /*----------------------------------------------------------------------*/
      flashParam->errorcode = kFlashOK;

      /*======================================================================*/
      /* -3- there are further sectors to erase                               */
      /*======================================================================*/
      if ( flashParam->ActualSector_US > flashParam->EndSector_US )
      {
        /*--------------------------------------------------------------------*/
        /* set busy flag to ensure further call of function                   */
        /*--------------------------------------------------------------------*/
        flashParam->errorcode |= kFlashEraseBusy;

        /*--------------------------------------------------------------------*/
        /* erase next lower sector                                            */
        /*--------------------------------------------------------------------*/
        flashParam->ActualSector_US--;

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/
  } /* -----1- END OF if(..) -------------------------------------------------*/
  /*==========================================================================*/
  /* -1- first call -> do init and compacting                                 */
  /*==========================================================================*/
  else
  {
    /*==========================================================================*/
    /* -2- Address range check                                                  */
    /*==========================================================================*/
    if (ulAdr <= FLASH_END)  /* ProgramFlash */
    {
      /*----------------------------------------------------------------------*/
      /* get start and end sector (erase from highest to lowest sector)       */
      /*----------------------------------------------------------------------*/
      flashParam->StartSector_US = GetEraseStartSector(flashParam->address, flashParam->length );
      flashParam->EndSector_US   = GetEraseEndSector(flashParam->address);

      /*----------------------------------------------------------------------*/
      /* start with highest sector                                            */
      /*----------------------------------------------------------------------*/
      flashParam->ActualSector_US = flashParam->StartSector_US;

      /*----------------------------------------------------------------------*/
      /* set busy flag to ensure further call of function                     */
      /*----------------------------------------------------------------------*/
      flashParam->errorcode |= kFlashEraseBusy;

    } /* ---2- END OF if(..) -------------------------------------------------*/
  } /* -----1- END OF else(..) -----------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /* -+- END OF FUNCTION 'FlashErase' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  FlashEraseSector                                            |
|                                                                              |
|  Prototype at:   FlashLib_TMS470.h                                           |
|  Description:    This function actually erases one flash sector according to |
|                  flashParam struct values                                    |
|  Parameter:      -                                                           |
|  Return Value:                                                               |
+-----------------------------------------------------------------------------*/
UBYTE FlashEraseSector(const tFlashParam *flashParam )
{
  volatile ULONG t_Start_UL;
  volatile ULONG t_Core_UL;
  volatile ULONG t_Delay_UL;

  /*--------------------------------------------------------------------------*/
  /* init return value in case of start of erasing fails or erasing returns   */
  /* an error                                                                 */
  /*--------------------------------------------------------------------------*/
  UBYTE Statusflag_UB = UNKNOWN_STATUS;

  /*--------------------------------------------------------------------------*/
  /* store constants in ram because of non-accessability of flash             */
  /*--------------------------------------------------------------------------*/
  t_Start_UL =   (ULONG) c_CCflash_SectorMap_AST[flashParam->ActualSector_US].Start_UL;
  t_Core_UL  =   (ULONG) c_CCflash_SectorMap_AST[flashParam->ActualSector_US].Core_UB;
  t_Delay_UL =   (ULONG) flashParam->Delaytime_UL;

  /*--------------------------------------------------------------------*/
  /* compact sector                                                     */
  /*--------------------------------------------------------------------*/
  Flash_Start_Command_B (
              (ULONG*) t_Start_UL,
              (ULONG)  t_Core_UL,
              (ULONG)  t_Delay_UL,
              (ULONG*) C_FlashControl_ST,
              0x000Eu,
              0x0000u);
  /*========================================================================*/
  /* -2- wait till finished erasing                                         */
  /*========================================================================*/
  while((Flash_Status_U16((ULONG*)C_FlashControl_ST) & 0x0100u) != 0u)
  {
    /*----------------------------------------------------------------------*/
    /* trigger watchdog                                                     */
    /*----------------------------------------------------------------------*/
    Feed_Watchdog_V();

  } /* -2- END OF while(..) ------------------------------------------------*/
  /*==========================================================================*/
  /* -1- start erasing this sector                                            */
  /*==========================================================================*/
  if((Flash_Start_Command_B (
              (ULONG*) t_Start_UL,
              (ULONG)  t_Core_UL,
              (ULONG)  t_Delay_UL,
              (ULONG*) C_FlashControl_ST,
              0x0006u,
              0xFFFFu)) != 0)
  {
    /*========================================================================*/
    /* -2- wait till finished erasing                                         */
    /*========================================================================*/
    while((Flash_Status_U16((ULONG*)C_FlashControl_ST) & 0x0100u) != 0u)
    {
      /*----------------------------------------------------------------------*/
      /* trigger watchdog                                                     */
      /*----------------------------------------------------------------------*/
      Feed_Watchdog_V();

    } /* -2- END OF while(..) ------------------------------------------------*/

    /*========================================================================*/
    /* -2- look for success                                                   */
    /*========================================================================*/
    if (Flash_Status_U16((ULONG*)C_FlashControl_ST) == 0u)
    {
      /*----------------------------------------------------------------------*/
      /* erasing done                                                         */
      /*----------------------------------------------------------------------*/
      Statusflag_UB = ERASE_SUCCESSFUL;

    } /* -2- END OF if(..) ---------------------------------------------------*/
  } /* ---1- END OF if(..) ---------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* return result                                                            */
  /*--------------------------------------------------------------------------*/
  return Statusflag_UB;

} /* -+- END OF FUNCTION 'FlashEraseSector' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  FlashWriteData                                              |
|                                                                              |
|  Prototype at:   TODO				                                           |
|  Description:    This function writes one WORD to an address of the Flash.   |
|                  The value to write is specified by the pointer wdata.       |
|                  The programming address is specified by the pointer         |
|                  ProgAddress_PUW                                             |
|  Parameter:      -                                                           |
|  Return Value:   WRITE_SUCCESSFUL (= 0x01)  Write succesfull                 |
|                  TIMEOUT_ERROR    (= 0x02)  Write timeout                    |
+-----------------------------------------------------------------------------*/
void FlashWrite(tFlashParam *flashParam)
{
  BOOL t_FlashReturnValue_B = FALSE;

  volatile ULONG t_Delay_UL;
  FLASH_ARRAY_ST t_Control_UL;
  volatile ULONG t_Core_UL;

  ULONG t_AlignmentCheck_UL;
  FLASH_STATUS_ST vol_ptdFlashStatus;
  UBYTE t_count_UB;

  /*--------------------------------------------------------------------------*/
  /* initialize errorcode                                                     */
  /*--------------------------------------------------------------------------*/
  flashParam->errorcode = UNKNOWN_STATUS;

  /*--------------------------------------------------------------------------*/
  /* initialize parameter                                                     */
  /*--------------------------------------------------------------------------*/
  t_Delay_UL = flashParam->Delaytime_UL;
  t_Control_UL = C_FlashControl_ST;

  /*==========================================================================*/
  /* -1- check length for valid size and length                               */
  /*==========================================================================*/
  if(flashParam->length >= d_Value4)
  {
    /*------------------------------------------------------------------------*/
    /* look for valid start address                                           */
    /*------------------------------------------------------------------------*/
    t_AlignmentCheck_UL = flashParam->address / (ULONG)d_Value4;

    /*========================================================================*/
    /* -2- address has the correct alignment                                  */
    /*========================================================================*/
    if((t_AlignmentCheck_UL * (ULONG)d_Value4) == flashParam->address)
    {
      /*----------------------------------------------------------------------*/
      /* look for a valid buffer length                                       */
      /*----------------------------------------------------------------------*/
      t_AlignmentCheck_UL = flashParam->length / (ULONG)d_Value4;

      /*======================================================================*/
      /* -3- buffer size is valid                                             */
      /*======================================================================*/
      if((t_AlignmentCheck_UL * (ULONG)d_Value4) == flashParam->length)
      {
        /*--------------------------------------------------------------------*/
        /* init watchdog counter                                              */
        /*--------------------------------------------------------------------*/
        t_count_UB = (UBYTE)0x80u;

        /*====================================================================*/
        /* -4- continue till length is zero                                   */
        /*====================================================================*/
        while (flashParam->length >= (ULONG)d_Value4)
        {
          /*------------------------------------------------------------------*/
          /* decrement watchdog counter                                       */
          /*------------------------------------------------------------------*/
          t_count_UB--;

          /*==================================================================*/
          /* -5- counter elapsed                                              */
          /*==================================================================*/
          if(t_count_UB == 0x00)
          {
            /*----------------------------------------------------------------*/
            /* trigger watchdog                                               */
            /*----------------------------------------------------------------*/
            Feed_Watchdog_V();

            /*----------------------------------------------------------------*/
            /* reset watchdog counter                                         */
            /*----------------------------------------------------------------*/
            t_count_UB     = (UBYTE)0x80u;

          } /* -5- END OF if(..) ---------------------------------------------*/
          
          /*==================================================================*/
          /* -5- Bank selection                                               */
          /*==================================================================*/
          if (flashParam->address <= d_Bank0End)
          {
            t_Core_UL = 0x00u;
          }
          else
          {
            t_Core_UL = 0x01u;
          } /* -5- END OF if(..) ---------------------------------------------*/

          t_FlashReturnValue_B = Flash_Prog_Data_B( (ULONG*)flashParam->address,
                                               (ULONG*)flashParam->data,
                                               (ULONG)1,
                                               (ULONG)t_Core_UL,
                                               t_Delay_UL,
                                               t_Control_UL,
                                               &vol_ptdFlashStatus,
                                               (ULONG)1,
                                               0x0002u);

          /*==================================================================*/
          /* -5- everything's fine                                            */
          /*==================================================================*/
          if (t_FlashReturnValue_B != FALSE)
          {
            /*----------------------------------------------------------------*/
            /* select next buffer content                                     */
            /*----------------------------------------------------------------*/
            flashParam->address += (ULONG)d_Value4;
            flashParam->length -= (ULONG)d_Value4;
            flashParam->data += (ULONG)d_Value4;

          } /* -5- END OF if(..) ---------------------------------------------*/
          /*==================================================================*/
          /* -5- an error occured                                             */
          /*==================================================================*/
          else
          {
            /*----------------------------------------------------------------*/
            /* set result to fail and build break condition                   */
            /*----------------------------------------------------------------*/
            flashParam->errorcode = kFlashFailed;
            flashParam->length = 0;

          } /* -5- END OF else(..) -------------------------------------------*/
        } /* ---4- END OF while(..) ------------------------------------------*/
      } /* -----3- END OF if(..) ---------------------------------------------*/
      /*======================================================================*/
      /* -3- wrong buffersize (not divideable by 4)                           */
      /*======================================================================*/
      else
      {
        flashParam->errorcode = kFlashFailed;

      } /* -----3- END OF else(..) -------------------------------------------*/
    } /* -------2- END OF if(..) ---------------------------------------------*/
    /*========================================================================*/
    /* -2- wrong startadress (not divideable by 4)                            */
    /*========================================================================*/
    else
    {
      flashParam->errorcode = kFlashFailed;

    } /* -------2- END OF else(..) -------------------------------------------*/
  } /* ---------1- END OF if(..) ---------------------------------------------*/
  /*==========================================================================*/
  /* -1- startadress is less than endadress                                   */
  /*==========================================================================*/
  else
  {
    flashParam->errorcode = kFlashFailed;

  } /* ---------1- END OF else(..) -------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /* -+- END OF FUNCTION 'FlashWrite' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  GetEraseEndSector                                           |
|                                                                              |
|  Prototype at:   ADLATUS_FlashFunctions_TDF.h                                |
|  Description:    This function returns the erase start sector                |
|  Parameter:      ULONG t_StartAddress_UL                                     |
|                  ULONG t_Length_UL                                           |
|  Return Value:   UBYTE  .. Start Sector                                      |
+-----------------------------------------------------------------------------*/
UWORD GetEraseEndSector ( ULONG ulStartAddress )
{
  /*--------------------------------------------------------------------------*/
  /* start with first erasable sector                                         */
  /*--------------------------------------------------------------------------*/
  UWORD uwTempSector = d_FirstEraseableSector;

  /*==========================================================================*/
  /* continue while StartAdress is bigger than endaddress of current sector   */
  /*==========================================================================*/
  while( ulStartAddress > ((c_CCflash_SectorMap_AST[uwTempSector].Start_UL \
                          + c_CCflash_SectorMap_AST[uwTempSector].Length_UL) - 1ul) )
  {
    /*------------------------------------------------------------------------*/
    /* increment sector                                                       */
    /*------------------------------------------------------------------------*/
    uwTempSector++;

  } /* -1- END OF while(..) --------------------------------------------------*/

  /*==========================================================================*/
  /* this sector is not valid any more (therefore d_LastEraseableSector must  */
  /* be smaller than last entry in sector map)                                */
  /*==========================================================================*/
  if( uwTempSector > (UWORD)d_LastEraseableSector )
  {
    /*------------------------------------------------------------------------*/
    /* return last sector that is erasable                                    */
    /*------------------------------------------------------------------------*/
    return (UWORD)d_LastEraseableSector;

  } /* -1- END OF if(..) -----------------------------------------------------*/
  /*==========================================================================*/
  /* regularly case: return sector which contains given address               */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* return found sector                                                    */
    /*------------------------------------------------------------------------*/
    return uwTempSector;

  } /* -1- END OF else(..) ---------------------------------------------------*/

} /* -+- END OF FUNCTION 'GetEraseEndSector' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  GetEraseStartSector                                         |
|                                                                              |
|  Prototype at:   ADLATUS_FlashFunctions_TDF.h                                |
|  Description:    This function returns the erase start sector                |
|  Parameter:      ULONG t_StartAddress_UL                                     |
|  Return Value:   UBYTE  .. End Sector                                        |
|                         .. Sector out of range                               |
+-----------------------------------------------------------------------------*/
UWORD GetEraseStartSector( ULONG ulStartAdr, ULONG ulLength )
{
  /*--------------------------------------------------------------------------*/
  /* start with last (highest) sector that is eraseable                       */
  /*--------------------------------------------------------------------------*/
  UWORD uwTempSector = d_LastEraseableSector;

  /*==========================================================================*/
  /* continue until end address of current sector is smaller than highest     */
  /* required flash address                                                   */
  /*==========================================================================*/
  while( ((ulStartAdr + ulLength) -1ul) < c_CCflash_SectorMap_AST[uwTempSector].Start_UL )
  {
    /*------------------------------------------------------------------------*/
    /* decrement sector                                                       */
    /*------------------------------------------------------------------------*/
    uwTempSector--;

  } /* -1- END OF while(..) --------------------------------------------------*/

  /*==========================================================================*/
  /* this sector is not valid any more (therefore d_FirstEraseableSector must */
  /* be bigger than first entry in sector map)                                */
  /*==========================================================================*/
  if( uwTempSector < (UWORD)d_FirstEraseableSector)  /* Lower Limit */
  {
    /*------------------------------------------------------------------------*/
    /* return first sector that is erasable                                   */
    /*------------------------------------------------------------------------*/
    return (UWORD)d_FirstEraseableSector;

  } /* -1- END OF if(..) -----------------------------------------------------*/
  /*==========================================================================*/
  /* regularly case: return sector which contains given address + length      */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* return found sector                                                    */
    /*------------------------------------------------------------------------*/
    return uwTempSector;

  } /* -1- END OF else(..) ---------------------------------------------------*/

} /* -+- END OF FUNCTION 'GetEraseStartSector' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|  Function Name:  Feed_Watchdog_V                                             |
|                                                                              |
|  Prototype at:   this file                                                   |
|  Description:    This function triggers internal and external watchdog       |
|  Parameter:      -                                                           |
|  Return Value:   void                                                        |
+-----------------------------------------------------------------------------*/
void Feed_Watchdog_V()
{
  ULONG *t_TempSrc_pul;

  /*--------------------------------------------------------------------------*/
  /* get pointer on watchdog function                                         */
  /*--------------------------------------------------------------------------*/
  t_TempSrc_pul  = (ULONG*)fp->wdTriggerFct;

  /*==========================================================================*/
  /* pointer is not a NULL pointer                                            */
  /*==========================================================================*/
  if (t_TempSrc_pul != 0x00000000UL)
  {
    /*------------------------------------------------------------------------*/
    /* trigger external watchdog                                              */
    /*------------------------------------------------------------------------*/
    fp->wdTriggerFct();

  } /* -1- END OF if(..) -----------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* trigger internal watchdog                                                */
  /* Reload the internal watchdog value register                              */
  /* no watchdog has to be initialized, but we trigger the wd                 */
  /*--------------------------------------------------------------------------*/
  e_SARRTI_ST.WKEY_UL = 0xE5;
  e_SARRTI_ST.WKEY_UL = 0xA3;

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /* -+- END OF FUNCTION 'Feed_Watchdog_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
