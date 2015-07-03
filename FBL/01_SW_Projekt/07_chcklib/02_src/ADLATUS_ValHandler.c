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
|  Filename:  ADLATUS_ValHandler.c                                             |
|             Standard interface to ADLATUS validation functions.              |
|                                                                              |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ValHandler.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_ValHandler.c $
|                                                                          |
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
#include "ADLATUS_Hal_cdf.h"                 
#include "ADLATUS_ConfigSystem_cdf.h"

#include "ADLATUS_ValHandler_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */

#include "ADLATUS_Timer_tdf.h"               /* Prototypes for the modul      */
#include "ADLATUS_Hal_tdf.h"

#include "ADLATUS_ValHandler_tdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
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

/* ========================================================================== */
/* I N T E R N A L                                                            */
/* ========================================================================== */
#define d_CCVhnd_InternalTimeSlot_UL ( (((ULONG)(d_CCVhnd_TimeSlot_1ms_UW)) << 8u) \
                                        |(ULONG) d_CCVhnd_TimeSlot_4us_UB )

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/* -------------------- */
/* Interface to validation handler */
/* -------------------- */
typedef struct _VALHANDLERCNTRL_ST
{
  const VALIDATIONMAP_ST* MemoryMap_PST;         /* Pointer to validationmap */
                                                 /* in ConfigProject */
  PTR_TO_FKT_VAL CalcRoutine_PFUN;               /* Pointer to Validation function */
  VALCONTROL_ST *WorkingParameters_PST;          /* Pointer to Addressinfo struct */
  VHND_PTR_BUF_UBYTE RangeStartAddress_PBUF;     /* Start address of check area */
  VHND_PTR_BUF_UBYTE RangeEndAddress_PBUF;       /* End address of check area */
  VHND_PTR_BUF_UBYTE CurrAddress_PBUF;           /* Current address in check area */

  ULONG ReferenceTimeTicks_UL;                   /* System timer ticks per (1ms/0xFF) */
  ULONG ChunkCalcTime_UL;                        /* Used timer ticks for this */

  UWORD SystemStartupState_UW;                   /* System State */
  UWORD CurrentChunkSize_UW;                     /* Current chunk size for calculation */                                                    
                                                 /* chunk size (n * 10us)*/

  UBYTE State_UB;                                /* Internal State */

} VALHANDLERCNTRL_ST;

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
UWORD const c_VerSMART_Chcklib_V110_UW = 0x0110u;

VALHANDLERCNTRL_ST CCVhnd_HndlCtrlInt_ST;
VALCONTROL_ST      CCVhnd_WorkingInfoInt_ST;
ULONG              CCVhnd_StartTime_UL;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
static UBYTE FUN_CCVhnd_RunCalculationInternal_V (void);

static void FUN_CCVhnd_DeInit_V (void);

static ULONG FUN_CCVhnd_GetCaptureTimer_UL (void);
static void FUN_CCVhnd_StartCaptureTimer_V (void);

static void FUN_CCVhnd_UpdateWorkingInfo_V (void);
/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_UpdateWorkingInfo_V                              |
|                                                                              |
|  Prototype at:   internal                                                    |
|                                                                              |
|  Description:    Calculate new address range for checksum calculation        |
|                   (internal)                                                 |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+------------------------------------------------------------------------------+*/
static void FUN_CCVhnd_UpdateWorkingInfo_V (void)
{
  /* -------------------- */
  /* Calculate new start and stop address */
  /* -------------------- */
  CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->StartAddress_PBUF = \
    CCVhnd_HndlCtrlInt_ST.CurrAddress_PBUF;
    
  CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->EndAddress_PBUF = \
    ((CCVhnd_HndlCtrlInt_ST.CurrAddress_PBUF + \
    CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW) - 1);

  /* -------------------- */
  /* Check for range end overflow */
  /* -------------------- */
  if ((CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->EndAddress_PBUF > 
       CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF) ||                   /*l_int !e946 */              
      (CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->EndAddress_PBUF < 
       CCVhnd_HndlCtrlInt_ST.RangeStartAddress_PBUF))                   /*l_int !e946 */
  {
    CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->EndAddress_PBUF = \
      CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF;
  }
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_StartCaptureTimer_V                              |
|                                                                              |
|  Prototype at:   internal                                                    |
|                                                                              |
|  Description:    Capture start value from timer (internal)                   |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+------------------------------------------------------------------------------+*/
static void FUN_CCVhnd_StartCaptureTimer_V (void)
{
  CCVhnd_StartTime_UL = FUN_CChal_GetPerformanceCounter_UL ();
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_StartCaptureTimer_V                              |
|                                                                              |
|  Prototype at:   internal                                                    |
|                                                                              |
|  Description:    Capture stop value from timer and return delta              |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   ULONG      Time delta to last call of StartCaptureTimer()   |
|                              ((time_in_ms << 8) | (ms / 0xFF))               |
|                                                                              |
+------------------------------------------------------------------------------+*/
static ULONG FUN_CCVhnd_GetCaptureTimer_UL (void)
{
  ULONG t_ReturnBuffer_UL;

  /* -------------------- */
  /* Get stop timer */
  /* -------------------- */
  t_ReturnBuffer_UL = FUN_CChal_GetPerformanceCounter_UL ();

  /* -------------------- */
  /* overflow detect */
  /* -------------------- */
  if (t_ReturnBuffer_UL < CCVhnd_StartTime_UL)
  {
    t_ReturnBuffer_UL += (0xFFFFFFFFul - CCVhnd_StartTime_UL);
  }
  else 
  {
    t_ReturnBuffer_UL -= CCVhnd_StartTime_UL;
  }

  t_ReturnBuffer_UL /= CCVhnd_HndlCtrlInt_ST.ReferenceTimeTicks_UL;

  return t_ReturnBuffer_UL;
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_InitCtrlST_UB                                    |
|                                                                              |
|  Prototype at:   ADLATUS_ValHandler_tdf.h                                    |
|                                                                              |
|  Description:    Init the internal validation control struct with the        |
|                   working parameters and set init conditions.                |
|                                                                              |
|  Parameter:      VALCONTROL_ST  *t_Parameters_PST                            |
|                           Pointer to validation working parameters           |
|                  PTR_TO_FKT_VAL t_ChecksumRoutine_PFUN                       |
|                           Pointer to validation funktion                     |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+------------------------------------------------------------------------------+*/
UBYTE FUN_CCVhnd_InitCtrlST_UB (const VALCONTROL_ST  *t_Parameters_PST,
                                 PTR_TO_FKT_VAL t_ChecksumRoutine_PFUN)
{
  UBYTE t_ReturnBuffer_UB;
  
  t_ReturnBuffer_UB = d_CCglo_Error;
  
  CCVhnd_HndlCtrlInt_ST.State_UB = d_CCVhnd_Idle;

  /* -------------------- */
  /* Check pointer and set state */
  /* -------------------- */
  if(t_Parameters_PST != 0ul) 
  {
    if(t_ChecksumRoutine_PFUN != 0ul)
    {
      CCVhnd_HndlCtrlInt_ST.CalcRoutine_PFUN = t_ChecksumRoutine_PFUN;
      
      CCVhnd_HndlCtrlInt_ST.State_UB = d_CCVhnd_Init;

      /* -------------------- */
      /* Init the control structs */
      /* -------------------- */
      CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW    = d_CCVhnd_InitChunkSize_UW;
      
      CCVhnd_HndlCtrlInt_ST.ChunkCalcTime_UL       = 0x00000000ul;

      CCVhnd_HndlCtrlInt_ST.ReferenceTimeTicks_UL  = FUN_CChal_GetPerfCntTicks_UL ();
      
      CCVhnd_HndlCtrlInt_ST.ReferenceTimeTicks_UL  = \
        (CCVhnd_HndlCtrlInt_ST.ReferenceTimeTicks_UL >> 8);

      CCVhnd_HndlCtrlInt_ST.RangeStartAddress_PBUF = t_Parameters_PST->StartAddress_PBUF;
      CCVhnd_HndlCtrlInt_ST.CurrAddress_PBUF       = t_Parameters_PST->StartAddress_PBUF;
      CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF   = t_Parameters_PST->EndAddress_PBUF;

      CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST  = &CCVhnd_WorkingInfoInt_ST;
      
      CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->ChecksumAddress_PBUF = \
        t_Parameters_PST->ChecksumAddress_PBUF;
      CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->WorkingBuffer_UL =\
        t_Parameters_PST->WorkingBuffer_UL;

      /* -------------------- */
      /* Init control flags */
      /* -------------------- */
      CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->Control_UB = d_CCVhnd_InitControlFlags;

      FUN_CCVhnd_UpdateWorkingInfo_V();

      /* -------------------- */
      /* No need for calibration, just run once */
      /* -------------------- */
      if(CCVhnd_HndlCtrlInt_ST.RangeStartAddress_PBUF == \
        CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF) 
      {
        CCVhnd_HndlCtrlInt_ST.State_UB |= d_CCVhnd_CalibrationDone;
      }

      t_ReturnBuffer_UB = d_CCglo_Ready;
    }
  }

  return t_ReturnBuffer_UB;
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_StartCaptureTimer_V                              |
|                                                                              |
|  Prototype at:   internal                                                    |
|                                                                              |
|  Description:    Deinit the internal validation control struct.              |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+------------------------------------------------------------------------------+*/
static void FUN_CCVhnd_DeInit_V (void)
{
  /* -------------------- */
  /* DeInit the control structs */
  /* -------------------- */
  CCVhnd_HndlCtrlInt_ST.State_UB               = d_CCVhnd_Idle;

  CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST  = 0x00000000ul;

  CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW    = 0x00000000ul;
  CCVhnd_HndlCtrlInt_ST.ChunkCalcTime_UL       = 0x00000000ul;

  CCVhnd_HndlCtrlInt_ST.ReferenceTimeTicks_UL  = 0x00000000ul;

  CCVhnd_HndlCtrlInt_ST.RangeStartAddress_PBUF = 0x00000000ul;
  CCVhnd_HndlCtrlInt_ST.CurrAddress_PBUF       = 0x00000000ul;
  CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF   = 0x00000000ul;
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_StartCaptureTimer_V                              |
|                                                                              |
|  Prototype at:   internal                                                    |
|                                                                              |
|  Description:    Calculate the checksum with a given maximum time.           |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+------------------------------------------------------------------------------+*/
static UBYTE FUN_CCVhnd_RunCalculationInternal_V (void)
{

  UBYTE t_ReturnBuffer_UB;

  ULONG t_LoopTime_UL;
  
  t_LoopTime_UL = 0x00000000ul;

  t_ReturnBuffer_UB = d_CCVhnd_Idle;

  if(d_CCVhnd_CalibrationDone != \
    (CCVhnd_HndlCtrlInt_ST.State_UB & d_CCVhnd_CalibrationDone)) 
  {
    while(d_CCVhnd_Break != (t_ReturnBuffer_UB & d_CCVhnd_Break)) 
    {
      /* -------------------- */
      /* Get start timer */
      /* -------------------- */
      FUN_CCVhnd_StartCaptureTimer_V();

      /* -------------------- */
      /* Call the calc routine */
      /* -------------------- */
      t_ReturnBuffer_UB = \
        CCVhnd_HndlCtrlInt_ST.CalcRoutine_PFUN(&CCVhnd_WorkingInfoInt_ST);

      /* -------------------- */
      /* Calculate time */
      /* -------------------- */
      CCVhnd_HndlCtrlInt_ST.ChunkCalcTime_UL = FUN_CCVhnd_GetCaptureTimer_UL();


      /* -------------------- */
      /* Update address pointer */
      /* -------------------- */
      CCVhnd_HndlCtrlInt_ST.CurrAddress_PBUF += CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW;

      /* -------------------- */
      /* Evaluate results and set state */
      /* -------------------- */
      if(d_CCVhnd_Ready == (t_ReturnBuffer_UB & d_CCVhnd_Ready)) 
      {
        if((CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->EndAddress_PBUF == \
          CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF) ||
          (d_CCVhnd_CheckNOK == (t_ReturnBuffer_UB & d_CCVhnd_CheckNOK)) ||
          (d_CCVhnd_CheckOK == (t_ReturnBuffer_UB & d_CCVhnd_CheckOK))) 
        {
          CCVhnd_HndlCtrlInt_ST.State_UB |=  d_CCVhnd_Ready;
          CCVhnd_HndlCtrlInt_ST.State_UB &= ~d_CCVhnd_Busy;
          t_ReturnBuffer_UB |= d_CCVhnd_Break;
        }
        else 
        {
          CCVhnd_HndlCtrlInt_ST.State_UB = d_CCVhnd_Busy;
        }
      }
      else 
      {
        CCVhnd_HndlCtrlInt_ST.State_UB = d_CCVhnd_Error;
        t_ReturnBuffer_UB |= d_CCVhnd_Break;
      }

      if (CCVhnd_HndlCtrlInt_ST.State_UB == d_CCVhnd_Busy) 
      {
        /* Not ready, calculate time and chunk size -> if time is left, restart*/

        /* -------------------- */
        /* Calculate new chunk size */
        /* -------------------- */
        if(CCVhnd_HndlCtrlInt_ST.ChunkCalcTime_UL < \
          (d_CCVhnd_InternalTimeSlot_UL >> 2ul)) 
        {
          /* -------------------- */
          /* Less than 25% used -> Multiply by 4 */
          /* -------------------- */
          CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW = \
            (CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW << 2u);
        }
        else
        {
          if (CCVhnd_HndlCtrlInt_ST.ChunkCalcTime_UL < \
            (d_CCVhnd_InternalTimeSlot_UL >> 1ul))
          {

            /* -------------------- */
            /* Less than 50% used -> Multiply by 1,5 */
            /* -------------------- */
            CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW = \
              CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW + \
              (CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW >> 1u);
          }
          else 
          {
            if (CCVhnd_HndlCtrlInt_ST.ChunkCalcTime_UL < \
              (d_CCVhnd_InternalTimeSlot_UL - (d_CCVhnd_InternalTimeSlot_UL >> 3ul))) 
            {
              /* -------------------- */
              /* Used between 50% and 87.5% -> Increase chunk by 12.5% */
              /* -------------------- */
              CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW += \
                (CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW >> 3u);
            }
            else 
            {
              /* -------------------- */
              /* We reached at least 87.5% of the give time slot - thats enough? */
              /* -------------------- */
              CCVhnd_HndlCtrlInt_ST.State_UB |= d_CCVhnd_CalibrationDone;
              t_ReturnBuffer_UB |= d_CCVhnd_Break;
            }
          }
        } /* End calculate chunk new size */

        /* -------------------- */
        /* delete the two LSBs that we are 4-byte aligned */
        /* -------------------- */
        CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW &= 0xFFFCu;

        /*====================================================================*/
        /* -4- the chunck size had been enlarged too much that it is zero now */
        /*====================================================================*/
        if (0x0000u == CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW)
        {
          /* -------------------- */
          /* set the largest chuck size */
          /* -------------------- */
          CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW = 0x8000u;
          CCVhnd_HndlCtrlInt_ST.State_UB |= d_CCVhnd_CalibrationDone;

        } /* -4- END OF if(..) -----------------------------------------------*/

        /* -------------------- */
        /* time left for one more go? */
        /* -------------------- */
        t_LoopTime_UL += FUN_CCVhnd_GetCaptureTimer_UL();
        if (t_LoopTime_UL > (d_CCVhnd_InternalTimeSlot_UL >> 2ul)) 
        {
          /* -------------------- */
          /* If we already used more than 25% of our time we come back later */
          /* -------------------- */
          t_ReturnBuffer_UB |= d_CCVhnd_Break;
        }
      } /* END: if (Busy) ... */

      FUN_CCVhnd_UpdateWorkingInfo_V();

    } /* END: while (!Break) ... */
    t_ReturnBuffer_UB &= ~d_CCVhnd_Break;
  } /* END: if (!CalibrationDone) ...*/
  else 
  {
    /* -------------------- */
    /* Call the calc routine */
    /* -------------------- */
    t_ReturnBuffer_UB = \
      CCVhnd_HndlCtrlInt_ST.CalcRoutine_PFUN(&CCVhnd_WorkingInfoInt_ST);

    /* -------------------- */
    /* Update address pointer */
    /* -------------------- */
    CCVhnd_HndlCtrlInt_ST.CurrAddress_PBUF += \
      CCVhnd_HndlCtrlInt_ST.CurrentChunkSize_UW;

    /* -------------------- */
    /* Evaluate results and set state */
    /* -------------------- */
    if(d_CCVhnd_Ready == (t_ReturnBuffer_UB & d_CCVhnd_Ready)) 
    {
      if((CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->EndAddress_PBUF == \
          CCVhnd_HndlCtrlInt_ST.RangeEndAddress_PBUF) || \
      (d_CCVhnd_CheckNOK == (t_ReturnBuffer_UB & d_CCVhnd_CheckNOK)) || \
      (d_CCVhnd_CheckOK == (t_ReturnBuffer_UB & d_CCVhnd_CheckOK))) 
      {
        CCVhnd_HndlCtrlInt_ST.State_UB |=  d_CCVhnd_Ready;
        CCVhnd_HndlCtrlInt_ST.State_UB &= ~d_CCVhnd_Busy;
      }
      else 
      {
        FUN_CCVhnd_UpdateWorkingInfo_V();
        CCVhnd_HndlCtrlInt_ST.State_UB |= d_CCVhnd_Busy;
        CCVhnd_HndlCtrlInt_ST.State_UB &=  ~d_CCVhnd_Ready;
      }
    }
    else 
    {
      CCVhnd_HndlCtrlInt_ST.State_UB |= d_CCVhnd_Error;
      CCVhnd_HndlCtrlInt_ST.State_UB &= ~d_CCVhnd_Busy;
    }
  }

  if ( (d_CCVhnd_Busy != (CCVhnd_HndlCtrlInt_ST.State_UB & d_CCVhnd_Busy)) ) 
  {
    /* -------------------- */
    /* Make last call with StartAdr == EndAdr -> validate checksum? */
    /* -------------------- */
    CCVhnd_HndlCtrlInt_ST.WorkingParameters_PST->Control_UB |= d_CCVhnd_CheckForResult;
    
    t_ReturnBuffer_UB = \
      CCVhnd_HndlCtrlInt_ST.CalcRoutine_PFUN(&CCVhnd_WorkingInfoInt_ST);
    
    /* -------------------- */
    /* Not Busy anymore -> Deinit everything! */
    /* -------------------- */
    t_ReturnBuffer_UB = (CCVhnd_HndlCtrlInt_ST.State_UB | \
      (t_ReturnBuffer_UB & (d_CCVhnd_CheckNOK | d_CCVhnd_CheckOK)));
    
    FUN_CCVhnd_DeInit_V();
  }
  return t_ReturnBuffer_UB;
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_CalcChecksum_UB                                  |
|                                                                              |
|  Prototype at:   ADLATUS_ValHandler_tdf.h                                    |
|                                                                              |
|  Description:    Calculates the checksum with triggering the internal timer. |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   UBYTE  Bits:  7 6 5 4  3 2 1 0                              |
|                                | | | |  | | | |                              |
|                                | | | |  | | | +- Checksum OK                 |
|                                | | | |  | | +--- Checksum not OK             |
|                                | | | |  | +----- Routine Break (internal)    |
|                                | | | |  +------- Calibration done (internal) |
|                                | | | |                                       |
|                                | | | +---------- Init done (internal)        |
|                                | | +------------ Error during calculation    |
|                                | +-------------- Calculation busy            |
|                                +---------------- Calculation done            |
|                                                                              |
+------------------------------------------------------------------------------+*/
UBYTE FUN_CCVhnd_CalcChecksum_UB (void)
{
  UBYTE t_ReturnBuffer_UB;

  t_ReturnBuffer_UB = d_CCVhnd_Busy;

  /* -------------------- */
  /* Check for correct conditions */
  /* -------------------- */
  if(d_CCVhnd_Init == (d_CCVhnd_Init & CCVhnd_HndlCtrlInt_ST.State_UB)) 
  {
    while ((d_CCVhnd_Busy == (d_CCVhnd_Busy & CCVhnd_HndlCtrlInt_ST.State_UB)) || \
           (d_CCVhnd_Init == (d_CCVhnd_Init & CCVhnd_HndlCtrlInt_ST.State_UB))) 
    {
      /* -------------------- */
      /* Do the calculation */
      /* -------------------- */
      t_ReturnBuffer_UB = FUN_CCVhnd_RunCalculationInternal_V();

      /* -------------------- */
      /* Check the system clock                                             */
      /* -------------------- */
      FUN_CCtim_SystemTimerCheck_V();
    }
  }
  else 
  {
    /* -------------------- */
    /* Running conditions not set -> return ERROR */
    /* -------------------- */
    t_ReturnBuffer_UB = d_CCVhnd_Error;
  }

  return t_ReturnBuffer_UB;
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_CalcChksumChunk_UB                               |
|                                                                              |
|  Prototype at:   ADLATUS_ValHandler_tdf.h                                    |
|                                                                              |
|  Description:    Calculates the checksum, returns after elapsed time with    |
|                   busy. In this case function has to be called again,        |
|                   otherwise it returns the result of the calculation.        |
|                                                                              |
|  Parameter:      none                                                        |
|                                                                              |
|  Return Value:   UBYTE    d_CCglo_Busy - Routine is not Ready calculating    |
|                           d_CCglo_ChecksumError - Checksum wrong or internal |
|                                                    error occured.            |
|                           d_CCglo_Ready - Finished calculating, Checksum OK. |
|                                                                              |
+------------------------------------------------------------------------------+*/
UBYTE FUN_CCVhnd_CalcChksumChunk_UB (void)
{
  UBYTE t_ReturnBuffer_UB;

  t_ReturnBuffer_UB = d_CCglo_Busy;

  /* -------------------- */
  /* Check for correct conditions */
  /* -------------------- */
  if ((d_CCVhnd_Init == (d_CCVhnd_Init & CCVhnd_HndlCtrlInt_ST.State_UB)) || \
      (d_CCVhnd_Busy == (d_CCVhnd_Busy & CCVhnd_HndlCtrlInt_ST.State_UB))) 
  {
     /* -------------------- */
     /* Do the calculation */
     /* -------------------- */
     t_ReturnBuffer_UB = FUN_CCVhnd_RunCalculationInternal_V();

     /* -------------------- */
     /* Mapping of return values for compatibility */
     /* -------------------- */
     if ((d_CCVhnd_CheckNOK == (d_CCVhnd_CheckNOK & t_ReturnBuffer_UB)) || \
         (d_CCVhnd_Error == (d_CCVhnd_Error & t_ReturnBuffer_UB))) 
     {
       t_ReturnBuffer_UB = d_CCglo_ChecksumError;
     }
     else 
     {
       /* -------------------- */
       /* Still busy? */
       /* -------------------- */
       if (d_CCVhnd_Busy == (d_CCVhnd_Busy & CCVhnd_HndlCtrlInt_ST.State_UB)) 
       {
         t_ReturnBuffer_UB = (d_CCglo_Busy );
       }
       else 
       {
          t_ReturnBuffer_UB = (d_CCglo_Ready );
       }
     }
  }
  else 
  {
    /* -------------------- */
    /* Running conditions not set -> return ERROR */
    /* -------------------- */
    t_ReturnBuffer_UB = d_CCglo_Error;
  }

  return t_ReturnBuffer_UB;
}


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCVhnd_CheckValidationMap_UB                            |
|                                                                              |
|  Prototype at:   ADLATUS_ValHandler_tdf.h                                    |
|                                                                              |
|  Description:    Checks the validation map for a matching entry by the       |
|                   given compare mask and value. This function returns after  |
|                   finishing with returning the result of the operation.      |
|                                                                              |
|  Parameter:      UWORD    t_Value_UW - Compare Value for matching entries.   |
|                  UWORD    t_Mask_UW  - Compare mask for matching entries.    |
|                                                                              |
|  Return Value:                                                               |
|                                                                              |
|  UBYTE      d_CCglo_NoError              Positive result, Calculation OK.    |
|             d_CCglo_NoFswInSystem        Only QuickCheck: ASW check failed   |
|             d_CCglo_AdlatusChecksumError Only Checksum: Adlatus check failed |
|             d_CCglo_FSWChecksumError     Only Checksum: ASW check failed     |
|             d_CCglo_RamCheckError        Only RAM-check: Check failed.       |
|                                                                              |
+------------------------------------------------------------------------------+*/
UBYTE FUN_CCVhnd_CheckValidationMap_UB (UWORD t_Value_UW, UWORD t_Mask_UW)
{
  UBYTE         t_Result_UB;
  UBYTE         t_TableIndex_UB;
  const VALIDATIONMAP_ST* t_MemoryMap_PST;


  if((ULONG)CCVhnd_HndlCtrlInt_ST.MemoryMap_PST != 0x00000000ul)  /*l_int !e923 */
  {
    /* -------------------- */
    /*  Init the local variables                                                */
    /* -------------------- */
    t_TableIndex_UB = 0x00;
    t_Result_UB = d_CCglo_NoError;
    t_MemoryMap_PST = CCVhnd_HndlCtrlInt_ST.MemoryMap_PST;

    while( (d_CCglo_NormalEntry == (d_CCglo_EntryMask & \
      t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW )) && \
      (d_CCglo_NoError     ==  t_Result_UB) )
    {
      /*========================================================================*/
      /* -2- Conditions OK                                                      */
      /*========================================================================*/
      if (t_Value_UW == (t_Mask_UW & t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW ))
      {
        /* We do a QuickCheck if configured - if this was successful, we check for more val funs */

        /*======================================================================*/
        /* -5- QuickCheck is enabled and requested                              */
        /*======================================================================*/
        if ( (d_CCglo_DoQuickCheck == 
          (d_CCglo_QuickCheckMask & t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW)) &&
          (d_CCglo_DoQuickCheck == (d_CCglo_QuickCheckMask & t_Value_UW))) 
        {
          CCVhnd_WorkingInfoInt_ST.StartAddress_PBUF    = 
            (VHND_PTR_BUF_UBYTE) t_MemoryMap_PST[t_TableIndex_UB].StartAddress_UL;       /*l_int !e923 */
          CCVhnd_WorkingInfoInt_ST.EndAddress_PBUF      = 
            (VHND_PTR_BUF_UBYTE) t_MemoryMap_PST[t_TableIndex_UB].StopAddress_UL;        /*l_int !e923 */
          CCVhnd_WorkingInfoInt_ST.ChecksumAddress_PBUF = 
            (VHND_PTR_BUF_UBYTE) t_MemoryMap_PST[t_TableIndex_UB].QuickCheckAddress_UL;  /*l_int !e923 */
          CCVhnd_WorkingInfoInt_ST.WorkingBuffer_UL     = 0x00000000ul;
          CCVhnd_WorkingInfoInt_ST.CheckCondition_UW    = 
            t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW;

          if ( d_CCglo_AlternateMethod == 
            (d_CCglo_ChecksumMask & t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW)) 
          {
            if ( d_CCglo_AlternateMethod2 == 
              (d_CCglo_ChecksumMask2 & t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW)) 
            {
              t_Result_UB = FUN_CCVhnd_InitCtrlST_UB ( &CCVhnd_WorkingInfoInt_ST, 
                c_CCVhnd_Alternate2Quickcheck_FUN);
            }
            else 
            {
              t_Result_UB = FUN_CCVhnd_InitCtrlST_UB ( &CCVhnd_WorkingInfoInt_ST, \
                c_CCVhnd_AlternateQuickcheck_FUN);
            }
          }
          else 
          {
            t_Result_UB = FUN_CCVhnd_InitCtrlST_UB ( &CCVhnd_WorkingInfoInt_ST, \
              c_CCVhnd_Quickcheck_FUN);
          }

          t_Result_UB = FUN_CCVhnd_CalcChecksum_UB ();
          /*====================================================================*/
          /* -6- QuickCheck failed                                              */
          /*====================================================================*/
          if ( d_CCVhnd_CheckOK != (t_Result_UB & d_CCVhnd_CheckOK)) {
            /* -------------------- */
            /* Set the System Startup State                                     */
            /* -------------------- */
            t_Result_UB = d_CCglo_NoFswInSystem;
          } /* -6- END OF if (..) ----------------------------------------------*/
          else {
            t_Result_UB = d_CCglo_NoError;
          }

        } /* -5- END OF if (..) ------------------------------------------------*/

        /*======================================================================*/
        /* -3- Check is enabled                                                 */
        /*======================================================================*/
        if ( (d_CCglo_CheckEnabled == \
          (d_CCglo_MemoryCheckMask & t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW)) && \
          (d_CCglo_CheckEnabled == (d_CCglo_MemoryCheckMask & t_Value_UW)) && \
          ( d_CCglo_NoError == t_Result_UB))
        {
          /*--------------------------------------------------------------------*/
          /*  Call the test routine given by the memory table entry             */
          /*--------------------------------------------------------------------*/
          CCVhnd_WorkingInfoInt_ST.StartAddress_PBUF    = 
            (VHND_PTR_BUF_UBYTE) t_MemoryMap_PST[t_TableIndex_UB].StartAddress_UL;    /*l_int !e923 */
          CCVhnd_WorkingInfoInt_ST.EndAddress_PBUF      = 
            (VHND_PTR_BUF_UBYTE) t_MemoryMap_PST[t_TableIndex_UB].StopAddress_UL;     /*l_int !e923 */ 
          CCVhnd_WorkingInfoInt_ST.ChecksumAddress_PBUF = 
            (VHND_PTR_BUF_UBYTE) t_MemoryMap_PST[t_TableIndex_UB].ChecksumAddress_UL; /*l_int !e923 */
          CCVhnd_WorkingInfoInt_ST.WorkingBuffer_UL = 0x00000000ul;
          CCVhnd_WorkingInfoInt_ST.CheckCondition_UW = \
            t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW;

          t_Result_UB = FUN_CCVhnd_InitCtrlST_UB ( &CCVhnd_WorkingInfoInt_ST, \
            t_MemoryMap_PST[t_TableIndex_UB].FUN_CheckRoutine_UB);


          t_Result_UB = FUN_CCVhnd_CalcChecksum_UB ();

          /*====================================================================*/
          /* -4- Checksum not correct                                           */
          /*====================================================================*/
          if ( d_CCVhnd_CheckOK != (t_Result_UB & d_CCVhnd_CheckOK))
          {

            /* -------------------- */
            /* Set the System Startup State                                     */
            /* -------------------- */
            CCVhnd_HndlCtrlInt_ST.SystemStartupState_UW = \
              (UWORD) t_MemoryMap_PST[t_TableIndex_UB].ErrorMessageEntry_UW;

            if (d_CCglo_FlashMemory == \
              (d_CCglo_MemoryTypeMask & t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW)) 
            {

              /* -------------------- */
              /* Flash entry... */
              /* -------------------- */
              if(d_CCglo_AdlatusArea == (d_CCglo_AdlatusAreaMask & \
                t_MemoryMap_PST[t_TableIndex_UB].CheckCondition_UW)) 
              {
                /* -------------------- */
                /* this was a ADLATUS Selfcheck */
                /* -------------------- */
                t_Result_UB = d_CCglo_AdlatusChecksumError;
              }
              else
              {
                /* -------------------- */
                /* this was an ASW check */
                /* -------------------- */
                t_Result_UB = d_CCglo_FSWChecksumError;
              }
            }
            else
            {
              /* -------------------- */
              /* Non Flash entry... */
              /* -------------------- */
              t_Result_UB = d_CCglo_RamCheckError;
            }
          } /* -4- END OF if (..) ----------------------------------------------*/
          else
          {
            t_Result_UB = d_CCglo_NoError;
          }

          } /* ---3- END OF if (..) --------------------------------------------*/
        } /* -2- END OF if (..) ------------------------------------------------*/

      /*------------------------------------------------------------------------*/
      /* Check the next table entry                                             */
      /*------------------------------------------------------------------------*/
      t_TableIndex_UB += 0x01;

    } /* -1- END OF while (..) -------------------------------------------------*/
  }
  else
  {
    /* -------------------- */
    /* Validation struct not valid */
    /* -------------------- */
    t_Result_UB = d_CCglo_Error;
  }

  return t_Result_UB;

}

/*************************** ALTLASTEN ?! ************************************/

void FUN_CCVhnd_InitValidation_V (const VALIDATIONMAP_ST* t_MemoryMap_PST )
{
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry:                                              */
  /* (1) Init System Startup State                                            */
  /* (2) Init Pointer to Memory Map Table                                     */
  /*--------------------------------------------------------------------------*/
  CCVhnd_HndlCtrlInt_ST.SystemStartupState_UW      = d_CCglo_NoError;
  CCVhnd_HndlCtrlInt_ST.MemoryMap_PST = t_MemoryMap_PST;

  return;

} /*-+- END OF FUNCTION 'FUN_CCVhnd_InitValidation_V' +-+-+-+-+-+-+-+-+-+-+-+-+*/


UWORD FUN_CCVhnd_GetStartupState_UW (void)
{
  /* -------------------- */
  /* Send the startup state                                                   */
  /* -------------------- */
  return (CCVhnd_HndlCtrlInt_ST.SystemStartupState_UW);
} /*-+- END OF FUNCTION 'FUN_CCVhnd_GetStartupState_UW' +-+-+-+-+-+-+-+-+-+-+-+*/
