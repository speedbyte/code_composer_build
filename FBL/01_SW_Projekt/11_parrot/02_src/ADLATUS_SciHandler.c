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
| $Id: ADLATUS_SciHandler.c 4522 2008-12-01 16:18:15Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20090305_S1NN_TMS470R_ParrotDriver_Hotfix/01_SW_Projekt/11_parrot/02_src/ADLATUS_SciHandler.c $
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
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_Scihandler_cdf.h"
#include "ADLATUS_ConfigSystem_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_Scihandler_tdf.h"

#include "tms470regs.h"


#include "ADLATUS_ConfigSystem_tdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*lint -e40 */    

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

/* ************************************************************************** */
/* ************************************************************************** */
/* **                                                                      ** */
/* **  S C I   C O N T R O L L E R    D E F I N E S                        ** */
/* **                                                                      ** */
/* ************************************************************************** */
/* ************************************************************************** */

/*----------------------------------------------------------------------------*/
/* DEFINITION FOR REGISTER INIT: SCIPC2 (8 bits)                              */
/*                                                                            */
/* Bit    7654 3210                                                           */
/*        |||| ||||                                                           */
/*        |||| |||+----  RX DATA DIR                                          */
/*        |||| ||+-----  RX FUNC                                              */
/*        |||| |+------  RX DATA OUT                                          */
/*        |||| +-------  RX DATA IN                                           */
/*        ||||                                                                */
/*        |||+---------  reserved                                             */
/*        ||+----------  reserved                                             */
/*        |+-----------  reserved                                             */
/*        +------------  reserved                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define INIT_VALUE__SCIPC2    (UBYTE) 0x02u

/*----------------------------------------------------------------------------*/
/* DEFINITION FOR REGISTER INIT: SCIPC3 (8 bits)                              */
/*                                                                            */
/* Bit    7654 3210                                                           */
/*        |||| ||||                                                           */
/*        |||| |||+----  TX DATA DIR                                          */
/*        |||| ||+-----  TX FUNC                                              */
/*        |||| |+------  TX DATA OUT                                          */
/*        |||| +-------  TX DATA IN                                           */
/*        ||||                                                                */
/*        |||+---------  reserved                                             */
/*        ||+----------  reserved                                             */
/*        |+-----------  reserved                                             */
/*        +------------  reserved                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define INIT_VALUE__SCIPC3    (UBYTE) 0x02u

/*----------------------------------------------------------------------------*/
/* DEFINITION FOR REGISTER INIT: SCICTL1 (8 bits)                             */
/*                                                                            */
/* Bit    7654 3210                                                           */
/*        |||| ||||                                                           */
/*        |||| |||+----  RX ENABLE                                            */
/*        |||| ||+-----  RX WAKE                                              */
/*        |||| |+------  RX READY                                             */
/*        |||| +-------  SLEEP                                                */
/*        ||||                                                                */
/*        |||+---------  IDLE                                                 */
/*        ||+----------  RX DMA ENABLE                                        */
/*        |+-----------  RX DMA ALL                                           */
/*        +------------  reserved                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define INIT_VALUE__SCICTL1   (UBYTE) 0x01u

/*----------------------------------------------------------------------------*/
/* DEFINITION FOR REGISTER INIT: SCICTL2 (8 bits)                             */
/*                                                                            */
/* Bit    7654 3210                                                           */
/*        |||| ||||                                                           */
/*        |||| |||+----  TX ENABLE                                            */
/*        |||| ||+-----  TX WAKE                                              */
/*        |||| |+------  TX READY                                             */
/*        |||| +-------  TX EMPTY                                             */
/*        ||||                                                                */
/*        |||+---------  reserved                                             */
/*        ||+----------  TX DMA ENABLE                                        */
/*        |+-----------  LOOP BACK                                            */
/*        +------------  CONTINUE                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define INIT_VALUE__SCICTL2   (UBYTE) 0x01u

/*----------------------------------------------------------------------------*/
/* DEFINITION FOR REGISTER INIT: SCICTL3 (8 bits)                             */
/*                                                                            */
/* Bit    7654 3210                                                           */
/*        |||| ||||                                                           */
/*        |||| |||+----  RX ERROR INTERRUPT ENABLE                            */
/*        |||| ||+-----  BREAK DT INTERRUPT ENABLE                            */
/*        |||| |+------  WAKE UP INTERRUPT ENABLE                             */
/*        |||| +-------  TX ACTION ENABLE                                     */
/*        ||||                                                                */
/*        |||+---------  RX ACTION ENABLE                                     */
/*        ||+----------  INTERNAL CLOCK                                       */
/*        |+-----------  POWER DOWN                                           */
/*        +------------  / SOFTWARE RESET                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define INIT_VALUE__SCICTL3   (UBYTE) 0x20u

/*----------------------------------------------------------------------------*/
/* DEFINITION FOR REGISTER INIT: SCICCR (8 bits)                              */
/*                                                                            */
/* Bit    7654 3210                                                           */
/*        |||| ||||                                                           */
/*        |||| |||+----  CHAR 0                                               */
/*        |||| ||+-----  CHAR 1                                               */
/*        |||| |+------  CHAR 2                                               */
/*        |||| +-------  COMM MODE                                            */
/*        ||||                                                                */
/*        |||+---------  TIMING MODE                                          */
/*        ||+----------  PARITY ENABLE                                        */
/*        |+-----------  PARITY                                               */
/*        +------------  STOPBITS                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define INIT_VALUE__SCICCR    (UBYTE) 0x17u

/*----------------------------------------------------------------------------*/
/* DEFINITION: Exponential Translation for Exponent = 6.                      */
/*----------------------------------------------------------------------------*/

#define EXP_10EE06_FACTOR     1000000ul

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
typedef struct _SCICONTROL
{
  ULONG QuarzClockFrequency_UL; /*  4 Byte - Pointer to constant ULONG */

  ULONG DefaultBaudrate_UL;     /*  4 Byte - Pointer to const UWORD */

                                              
} SCICONTROL; 


SCICONTROL t_CCsci_SciCtrl_ST;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

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

/*============================================================================*/
/* FUNCTION:         FUN_PARsci_Init_V                                         */
/*----------------------------------------------------------------------------*/
/* Description:      Initialize the serial interface for K-Line transfer.     */
/*                                                                            */
/*============================================================================*/

void FUN_PARsci_Init_V(const SCIINTERFACE_ST* t_Interface_PST)
{
  t_CCsci_SciCtrl_ST.QuarzClockFrequency_UL =  *t_Interface_PST->IntClkFrequency_UL;
  t_CCsci_SciCtrl_ST.DefaultBaudrate_UL     =  *t_Interface_PST->DefaultBaudrate_UL;

  return;
}
  
void FUN_PARsci_InitInternal_V(void)
{

  /* Reset the UART controller, clears all rx and tx status flags */
  e_SCI2_ST.ctl3_UN.ctl3_UB &= (~(UBYTE) 0x80u);
  /* initialize the interrupt control and reset UART controller */
  e_SCI2_ST.ctl3_UN.ctl3_UB = INIT_VALUE__SCICTL3;
  /* initialize the RXD pin configuration */
  e_SCI2_ST.pc_PST[1].pc_UN.pc_UB = INIT_VALUE__SCIPC2;
  /* initialize the TXD pin configuration*/
  e_SCI2_ST.pc_PST[2].pc_UN.pc_UB = INIT_VALUE__SCIPC3;
  /* initialize the receiver DMA settings and enable UART receiving */
  e_SCI2_ST.ctl1_UN.ctl1_UB = INIT_VALUE__SCICTL1;
  /* initialize the transmitter DMA settings and enable UART transmission */
  e_SCI2_ST.ctl2_UN.ctl2_UB = INIT_VALUE__SCICTL2;

  /* set mode 8n1 */
  FUN_PARsci_SetSerialMode_V(UART_MODE_8N1);
  FUN_PARsci_SetSerialBaudrate_V();

  /* start the UART controller */
  e_SCI2_ST.ctl3_UN.ctl3_UB |= (UBYTE) 0x80u;
  return;
}


/*============================================================================*/
/* FUNCTION:         FUN_PARsci_CtrlSerialInterface_V                          */
/*----------------------------------------------------------------------------*/
/* Description:      This function enables or disables the receiver function  */
/*                   of the UART controller.                                  */
/*                   The function is needed to prevent collisions on half     */
/*                   duplex lines.                                            */
/*                                                                            */
/* Parameter 1:      TRUE:    receiver will be enabled                        */
/*                   FALSE:   receiver will be disabled                       */
/*                                                                            */
/*============================================================================*/

VOID FUN_PARsci_CtrlSerialInterface_V(BOOL l_Enable_SIO_B)
{
  /* if receive operation is needed, set the RX ENABLE bit */
  if (l_Enable_SIO_B != FALSE)
  {
    e_SCI2_ST.ctl1_UN.ctl1_UB |= (UBYTE) 0x01u;
  }
  else
  {
    /* disable the UART receiver, by clearing the RX ENABLE bit */
    e_SCI2_ST.ctl1_UN.ctl1_UB &= (~((UBYTE) 0x01u));
  }

  return;
}


/*============================================================================*/
/* FUNCTION:         FUN_PARsci_GetSendState_B                                 */
/*----------------------------------------------------------------------------*/
/* Description:      This function returns the send execution state of the    */
/*                   UART controller. If no character is clocked out of the   */
/*                   shift register, the return value is FALSE. While a       */
/*                   character is clocked out, the return value is TRUE,      */
/*                   until the last bit is transmitted.                       */
/*                   The function is needed to prevent collisions on half     */
/*                   duplex lines.                                            */
/*                                                                            */
/* Return Value:     TRUE:    while send execution                            */
/*                   FALSE:   if there is no send execution                   */
/*                                                                            */
/*============================================================================*/

UBYTE FUN_PARsci_GetSendState_B(VOID)
{
  /* if TX EMPTY flag is zero, no bits will be transmitted, so return TRUE */
  return (e_SCI2_ST.ctl2_UN.ctl2_UB & (UBYTE) 0x08u);
}

/*============================================================================*/
/* FUNCTION:         FUN_PARsci_SendByte_B                                     */
/*----------------------------------------------------------------------------*/
/* Description:      This function lets the UART send a character.            */
/*                                                                            */
/* Parameter 1:      a pointer to the character, which will be send           */
/* Return Value:     d_CCglo_Ready: the send operation was successful         */
/*                   d_CCglo_Error: send operation wont be done,              */
/*                                  because TXBUF is not empty                */
/*                                                                            */
/*============================================================================*/

UBYTE FUN_PARsci_SendByte_UB(PTR_BUFFER_UB l_TransferByte_PUB)
{
  UBYTE t_Result_UB;

  /* check, if TXBUF is able to get new data */
  if ((e_SCI2_ST.ctl2_UN.ctl2_UB & (UBYTE) 0x04u) == (UBYTE) 0x04u)
  {
    /* write the transfer byte to the transfer buffer */
    e_SCI2_ST.TxBuf_UB = *l_TransferByte_PUB;
    /* return ok, because the send operation was successfully */
    t_Result_UB = d_CCglo_Ready;
  }
  else
  {
    /* return error, because the send operation was not successfully */
    t_Result_UB = d_CCglo_Error;
  }

  return t_Result_UB;
}


/*============================================================================*/
/* FUNCTION:         FUN_PARsci_ReceiveData_B                                  */
/*----------------------------------------------------------------------------*/
/* Description:      This function gets a received character from the UART.   */
/*                                                                            */
/* Parameter 1:      a pointer to the buffer, which stores the character      */
/* Return Value:     d_CCglo_Ready: a character has been received             */
/*                   d_CCglo_Idle: no character was received                  */
/*                                                                            */
/*============================================================================*/

UBYTE FUN_PARsci_ReceiveByte_UB(PTR_BUFFER_UB l_TransferByte_PUB)
{
  UBYTE t_Result_UB;

  /* check, if a character was received by the UART controller */
  if ((e_SCI2_ST.ctl1_UN.ctl1_UB & (UBYTE) 0x04u) == (UBYTE) 0x04u)
  {
    /* get the received character from RXBUF register */
    *l_TransferByte_PUB = e_SCI2_ST.RxBuf_UB;

    t_Result_UB = d_CCglo_Ready;
  }
  else
  {
    /* return FALSE, because there was no character received by the UART */
    t_Result_UB = d_CCglo_Idle;
  }

  return t_Result_UB;
}


/*============================================================================*/
/* FUNCTION:         FUN_BB_HW_DRV_SetSerialMode_V                            */
/*----------------------------------------------------------------------------*/
/* Description:      This function sets the data format configuration to      */
/*                   the UART controller.                                     */
/*                                                                            */
/* Parameter 1:      defined data format byte                                 */
/*                                                                            */
/*============================================================================*/

void FUN_PARsci_SetSerialMode_V(UBYTE l_SerialMode_UB)
{
  /* wait, if UART controller is busy */
  while ( ((e_SCI2_ST.rxst_UN.rxst_UB & (UBYTE) 0x80u) == (UBYTE) 0x80u) ||
          ((e_SCI2_ST.ctl2_UN.ctl2_UB & (UBYTE) 0x08u) == 0) )
  { 
    ; 
  }
  /* stop the UART controller */
  e_SCI2_ST.ctl3_UN.ctl3_UB &= (~((UBYTE) 0x80u));
  /* set all mode bits to zero */
  e_SCI2_ST.ccr_UN.ccr_UB = INIT_VALUE__SCICCR;

  /* start the UART controller */
  e_SCI2_ST.ctl3_UN.ctl3_UB |= (UBYTE) 0x80u;

  return;
}

/*============================================================================*/
/* FUNCTION:         FUN_PARsci_SetSerialBaudrate_V                            */
/*----------------------------------------------------------------------------*/
/* Description:      This function calculates the values for the baudrate     */
/*                   registers and changes the baudrate.                      */
/*                                                                            */
/* Parameter 1:      baudrate value                                           */
/*                                                                            */
/*============================================================================*/

void FUN_PARsci_SetSerialBaudrate_V(void)
{
  UNION4   l_BaudRegister_U4;
  ULONG    l_ArithmeticDivisor_UL;

    /* wait, if UART controller is busy */
    while ( ((e_SCI2_ST.rxst_UN.rxst_UB & (UBYTE) 0x80u) == (UBYTE) 0x80u) ||
          ((e_SCI2_ST.ctl2_UN.ctl2_UB & (UBYTE) 0x08u) == 0) )
    { 
      ; 
    }

    /* stop the UART controller */
    e_SCI2_ST.ctl3_UN.ctl3_UB &= (~(UBYTE) 0x80u);

    /*-------- calculate the new baudrate value --------*/

    l_ArithmeticDivisor_UL = (t_CCsci_SciCtrl_ST.DefaultBaudrate_UL << 3);
    /* initialize temporary baudrate register */
    l_BaudRegister_U4.ULong_ST.ULong0_UL = 0;
    /* divide the arithmetic variable, which contains the system clock frequency,
       by the calculated divisor */
    while (t_CCsci_SciCtrl_ST.QuarzClockFrequency_UL > l_ArithmeticDivisor_UL)
    {
      t_CCsci_SciCtrl_ST.QuarzClockFrequency_UL -= l_ArithmeticDivisor_UL;
      l_BaudRegister_U4.ULong_ST.ULong0_UL ++;
    }

    /* decrement the actual baudrate register value */
    l_BaudRegister_U4.ULong_ST.ULong0_UL--;
    /* transfer temporary baudrate register to the UART baudrate registers */
    e_SCI2_ST.HBaud_UB = l_BaudRegister_U4.UByte_ST.UByte1_UB;
    e_SCI2_ST.MBaud_UB = l_BaudRegister_U4.UByte_ST.UByte2_UB;
    e_SCI2_ST.LBaud_UB = l_BaudRegister_U4.UByte_ST.UByte3_UB;


    /* start the UART controller */
    e_SCI2_ST.ctl3_UN.ctl3_UB |= (UBYTE) 0x80u;

  return;
}

/*lint +e40 */    
  
/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */



