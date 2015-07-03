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
|  Filename:  ADLATUS_ServiceHandler.c                                         |
|                                                                              |
|  Comment:   This file includes the service handler functions of the          |
|             SMART ADLATUS communication protocol stack.                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ServiceHandler.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/08_corelib/02_src/ADLATUS_ServiceHandler.c $
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
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_ServiceHandler_tdf.h"
 
/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches: No Entry                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef cs_CCsys_SingleChannelSystem
  #ifndef cs_CCsys_MultiChannelSystem 
       #error (undefined channel configuration)
  #endif
#endif
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
typedef struct _SERVICEHANDLERCONTROL
{
  CMDRUNTIMEINFO_ST *CmdRunTimeInfo_PST;  /*  4 Byte - ptr to global RuntimeInfoStructure */
                                          /* ---------------------------------------------*/
} SERVICEHANDLERCONTROL_ST;               /*  4 Byte                                      */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
SERVICEHANDLERCONTROL_ST   t_ServiceHandlerCtrl_ST;

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
|  Function Name:  FUN_CCsrv_InitServiceHandler_V                              |
|                                                                              |
|  Prototype at:   ADLATUS_ServiceHandler_TDF.h                                |
|                                                                              |
|  Description:    Initfunction for the service handler modul.                 |
|                                                                              |
|  Parameter:      const SRVHANDLERINTERFACE_ST* Interface_PST ... Ptr to interface |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsrv_InitServiceHandler_V (const SRVHANDLERINTERFACE_ST* Interface_PST)
{
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 1: Init Pointer to RuntimeInfo                */
  /*--------------------------------------------------------------------------*/
  t_ServiceHandlerCtrl_ST.CmdRunTimeInfo_PST = Interface_PST->CmdRunTimeInfo_PST;

  return;

} /*-+- END OF FUNCTION 'FUN_CCsrv_InitServiceHandler_V' -+-+-+-+-+-+-+-+-+-+-*/



/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*                                                                            */
/* S I N G L E   C H A N N E L   S Y S T E M                                  */
/*                                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifdef cs_CCsys_SingleChannelSystem /* -1-                                    */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsrv_RxServiceHandler_UB                               |
|                                                                              |
|  Prototype at:   ADLATUS_ServiceHandler_tdf.h                                |
|                                                                              |
|  Description:    This routine calls the application function which was       |
|                  selected by the SessionRxFunction. Also the routine checks  |
|                  the result of the function call.                            |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  ... Ptr to Inputbuffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF ... Ptr to Outputbuffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    INDEX                         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCsrv_RxServiceHandler_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                      PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE t_Result_UB;
  UBYTE t_Index_UB;
  UBYTE (*t_PtrToFunction_PFUN) (PTR_BUFFER_UB t_InputBuffer_PBUF, \
                                 PTR_BUFFER_UB t_OutputBuffer_PBUF );

  /* ------------------------------------------------------------------------ */
  /* Init Result                                                              */
  /* ------------------------------------------------------------------------ */
  t_Result_UB = d_CCglo_Idle;

  /*==========================================================================*/
  /* -1- Inbuffer is valid and Outbuffer is invalid                           */
  /*==========================================================================*/
  if ( ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid ) == d_CCglo_BufferValid) &&
       ((t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid ) != d_CCglo_BufferValid) )
  {
    /* ---------------------------------------------------------------------- */
    /* Copy Channel Info to Outbuffer                                         */
    /* Note: The outbuffer SubSet must be incremented by one!! This is        */
    /*       necessary for the response identifier selection on TP and phys.  */
    /*       Layer!!                                                          */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] += 0x10;

    /* ---------------------------------------------------------------------- */
    /* Init Channel for the following process                                 */
    /* Note: Channel => [Subset + ChannelNbr]                                 */
    /* ---------------------------------------------------------------------- */
    /* Not necessary within single channel mode!                              */
    /* t_Channel_UB =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];                   */
    /* t_Channel_UB &= d_CCglo_ChannelMask;                                   */

    /* ---------------------------------------------------------------------- */
    /* Init Function Call                                                     */
    /* ---------------------------------------------------------------------- */
    t_PtrToFunction_PFUN = \
    t_ServiceHandlerCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST->Function_PFUN;

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      F U N C T I O N C A L L   V I A    P O I N T E R                  */
    /*                                                                        */
    /* -----------------------------------------------------------------------*/
    t_Result_UB = t_PtrToFunction_PFUN (t_InBuffer_PBUF, t_OutBuffer_PBUF );

    /*========================================================================*/
    /* -2- Result evaluation for buffer handling                              */
    /*                                                                        */
    /*     NOTE: Some buffer state operations are redundant, but some         */
    /*           compilers has done mystic optimisations. To prevent an error */
    /*           of such a compiler optimisation, each condition of the       */
    /*           return value is processed completly!!                        */
    /*========================================================================*/
    switch (t_Result_UB & d_CCglo_ResultInfoMask)
    {
      /*----------------------------------------------------------------------*/
      /* READY                                                                */
      /* [0x80]                                                               */
      /*----------------------------------------------------------------------*/
      case d_CCglo_Ready:
        /* Set inbuffer invalid                                               */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= ~d_CCglo_BufferValid;
        /* Set outbuffer valid                                                */
        t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /*----------------------------------------------------------------------*/
      /* BUSY                                                                 */
      /* [0x40]                                                               */
      /*----------------------------------------------------------------------*/
      case d_CCglo_Busy:
        /* Set inbuffer valid                                                 */
        t_InBuffer_PBUF[d_CCglo_BufferState]  |= d_CCglo_BufferValid;
        /* Set outbuffer invalid                                              */
        t_OutBuffer_PBUF[d_CCglo_BufferState] &= ~d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
      /*----------------------------------------------------------------------*/
      /* READY + BUSY                                                         */
      /* [0xC0]                                                               */
      /*----------------------------------------------------------------------*/
      case (d_CCglo_Ready | d_CCglo_Busy):
        /* Set inbuffer valid                                                 */
        t_InBuffer_PBUF[d_CCglo_BufferState]  |= d_CCglo_BufferValid;
        /* Set outbuffer valid                                                */
        t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /*----------------------------------------------------------------------*/
      /* IDLE                                                                 */
      /* [0x00]                                                               */
      /*----------------------------------------------------------------------*/
      default:
        /* Set inbuffer invalid                                               */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= ~d_CCglo_BufferValid;
        /* Set outbuffer invalid                                              */
        t_OutBuffer_PBUF[d_CCglo_BufferState] &= ~d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    } /* -2- END OF switch (..) ----------------------------------------------*/


    /*------------------------------------------------------------------------*/
    /* Result evaluation for communication stack                              */
    /* NOTE: This is necessary because the response index evaluation will be  */
    /*       done by the session layer.                                       */
    /*------------------------------------------------------------------------*/
    t_ServiceHandlerCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].ResponseIndex_UB = \
                                     (t_Result_UB & d_CCglo_RespIndexMask);

    /*========================================================================*/
    /* -2- Inbuffer is invalid - clear buffer value                           */
    /*========================================================================*/
    if ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid ) != d_CCglo_BufferValid)
    {
      /*======================================================================*/
      /* -3- Copy x data                                                      */
      /*======================================================================*/

      if (t_InBuffer_PBUF[d_CCglo_DLC] > t_InBuffer_PBUF[d_CCglo_MaxBufferLength])
      {
        t_InBuffer_PBUF[d_CCglo_DLC] = t_InBuffer_PBUF[d_CCglo_MaxBufferLength];
      }

      for (t_Index_UB =  0x00;
           t_Index_UB <  t_InBuffer_PBUF[d_CCglo_DLC];
           t_Index_UB ++ )
      {
        /* Init InBuffer                                                      */
        t_InBuffer_PBUF[ (d_CCglo_StartOfData + t_Index_UB) ] = 0x00;

      } /* -3- END OF for (..) -----------------------------------------------*/
    } /* ---2- END IF if(..) -------------------------------------------------*/

  } /* ---1- END IF if(..) ---------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCsrv_RxServiceHandler_UB' -+-+-+-+-+-+-+-+-+-+-+*/



/*----------------------------------------------------------------------------*/
#endif /* -1- cs_CCsys_SingleChannelSystem                                    */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/*                                                                            */
/*      E N D   O F   S I N G L E   C H A N N E L   S Y S T E M               */
/*                                                                            */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*                                                                            */
/*      M U L T I   C H A N N E L   S Y S T E M                               */
/*                                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifdef cs_CCsys_MultiChannelSystem /* -1-                                     */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsrv_RxServiceHandler_UB                               |
|                                                                              |
|  Prototype at:   ADLATUS_ServiceHandler_tdf.h                                |
|                                                                              |
|  Description:    This routine calls the application function which was       |
|                  selected by the SessionRxFunction. Also the routine checks  |
|                  the result of the function call.                            |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  ... Ptr to Inputbuffer       |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF ... Ptr to Outputbuffer      |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    INDEX                         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCsrv_RxServiceHandler_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                      PTR_BUFFER_UB t_OutBuffer_PBUF )
{
  UBYTE t_Channel_UB;
  UBYTE t_Result_UB;
  UBYTE t_Index_UB;
  UBYTE (*t_PtrToFunction_PFUN) (PTR_BUFFER_UB t_InputBuffer_PBUF, \
                                 PTR_BUFFER_UB t_OutputBuffer_PBUF );

  /* ------------------------------------------------------------------------ */
  /* Init Result                                                              */
  /* ------------------------------------------------------------------------ */
  t_Result_UB = d_CCglo_Idle;

  /*==========================================================================*/
  /* -1- Inbuffer is valid and Outbuffer is invalid                           */
  /*==========================================================================*/
  if ( ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid ) == d_CCglo_BufferValid) &&
       ((t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid ) != d_CCglo_BufferValid) )
  {
    /* ---------------------------------------------------------------------- */
    /* Copy Channel Info to Outbuffer                                         */
    /* Note: The outbuffer SubSet must be incremented by one!! This is        */
    /*       necessary for the response identifier selection on TP and phys.  */
    /*       Layer!!                                                          */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] += 0x10;

    /* ---------------------------------------------------------------------- */
    /* Init Channel for the following process                                 */
    /* Note: Channel => [Subset + ChannelNbr]                                 */
    /* ---------------------------------------------------------------------- */
    t_Channel_UB =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];
    t_Channel_UB &= d_CCglo_ChannelMask;

    /* ---------------------------------------------------------------------- */
    /* Init Function Call                                                     */
    /* ---------------------------------------------------------------------- */
    t_PtrToFunction_PFUN = \
    t_ServiceHandlerCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST->Function_PFUN;


    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      F U N C T I O N C A L L   V I A    P O I N T E R                  */
    /*                                                                        */
    /* -----------------------------------------------------------------------*/
    t_Result_UB = t_PtrToFunction_PFUN (t_InBuffer_PBUF, t_OutBuffer_PBUF );

    /*========================================================================*/
    /* -2- Result evaluation for buffer handling                              */
    /*                                                                        */
    /*     NOTE: Some buffer state operations are redundant, but some         */
    /*           compilers has done mystic optimisations. To prevent an error */
    /*           of such a compiler optimisation, each condition of the       */
    /*           return value is processed completly!!                        */
    /*========================================================================*/
    switch (t_Result_UB & d_CCglo_ResultInfoMask)
    {
      /*----------------------------------------------------------------------*/
      /* READY                                                                */
      /* [0x80]                                                               */
      /*----------------------------------------------------------------------*/
      case d_CCglo_Ready:
        /* Set inbuffer invalid                                               */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        /* Set outbuffer valid                                                */
        t_OutBuffer_PBUF[d_CCglo_BufferState] |= (UBYTE) d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /*----------------------------------------------------------------------*/
      /* BUSY                                                                 */
      /* [0x40]                                                               */
      /*----------------------------------------------------------------------*/
      case d_CCglo_Busy:
        /* Set inbuffer valid                                                 */
        t_InBuffer_PBUF[d_CCglo_BufferState]  |= (UBYTE) d_CCglo_BufferValid;
        /* Set outbuffer invalid                                              */
        t_OutBuffer_PBUF[d_CCglo_BufferState] &= (UBYTE) ~d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
      /*----------------------------------------------------------------------*/
      /* READY + BUSY                                                         */
      /* [0xC0]                                                               */
      /*----------------------------------------------------------------------*/
      case (d_CCglo_Ready | d_CCglo_Busy):
        /* Set inbuffer valid                                                 */
        t_InBuffer_PBUF[d_CCglo_BufferState]  |= (UBYTE) d_CCglo_BufferValid;
        /* Set outbuffer valid                                                */
        t_OutBuffer_PBUF[d_CCglo_BufferState] |= (UBYTE) d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /*----------------------------------------------------------------------*/
      /* IDLE                                                                 */
      /* [0x00]                                                               */
      /*----------------------------------------------------------------------*/
      default:
        /* Set inbuffer invalid                                               */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        /* Set outbuffer invalid                                              */
        t_OutBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        break;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    } /* -2- END OF switch (..) ----------------------------------------------*/


    /*------------------------------------------------------------------------*/
    /* Result evaluation for communication stack                              */
    /* NOTE: This is necessary because the response index evaluation will be  */
    /*       done by the session layer.                                       */
    /*------------------------------------------------------------------------*/
    t_ServiceHandlerCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].ResponseIndex_UB = \
                                     (t_Result_UB & d_CCglo_RespIndexMask);


    /*========================================================================*/
    /* -2- Inbuffer is invalid - clear buffer value                           */
    /*========================================================================*/
    if ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid ) != d_CCglo_BufferValid)
    {
      if (t_InBuffer_PBUF[d_CCglo_DLC] > t_InBuffer_PBUF[d_CCglo_MaxBufferLength])
      {
        t_InBuffer_PBUF[d_CCglo_DLC] = t_InBuffer_PBUF[d_CCglo_MaxBufferLength];
      }

      /*======================================================================*/
      /* -3- Copy x data                                                      */
      /*======================================================================*/
      for (t_Index_UB =  0x00;
           t_Index_UB <  t_InBuffer_PBUF[d_CCglo_DLC];
           t_Index_UB ++ )
      {
        /* Init InBuffer                                                      */
        t_InBuffer_PBUF[ (d_CCglo_StartOfData + t_Index_UB) ] = 0x00;

      } /* -3- END OF for (..) -----------------------------------------------*/
    } /* ---2- END IF if(..) -------------------------------------------------*/
  } /* -----1- END IF if(..) -------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return                                                                   */
  /*--------------------------------------------------------------------------*/
  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCsrv_RxServiceHandler_UB' -+-+-+-+-+-+-+-+-+-+-+*/



/*----------------------------------------------------------------------------*/
#endif /* -1- cs_CCsys_MultiChannelSystem                                     */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/*                                                                            */
/*      E N D   O F   M U L T I    C H A N N E L   S Y S T E M                */
/*                                                                            */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
