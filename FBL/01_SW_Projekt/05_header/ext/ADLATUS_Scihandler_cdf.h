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
|  Filename:  ADLATUS_CanHandler_cdf.h                                         |
|                                                                              |
|  Comment:   This file includes all defines of the                            |
|             CAN handler for the SMART ADLATUS.                               |
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
| $Id: ADLATUS_Scihandler_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Scihandler_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_SCINHANDLER_
#define __CDF_SCIHANDLER_

/* OPEN_INCLUDE_PROTECTION_END:                                               */

/* ========================================================================== */
/*                                                                            */
/*      S C I   D E F I N E S                                                 */
/*                                                                            */
/* ========================================================================== */    

/*----------------------------------------------------------------------------*/
/* DEFINITION: HW_DRV____RXD_PIN                                              */
/*----------------------------------------------------------------------------*/
/* Description:      This definition makes the RXD Pin of the UART controller */
/*                   accessible for the higher level communication driver.    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define HW_DRV____RXD_PIN               (e_SCI0_ST.pc_PST[1].pc_UN.pc_UB & MBIT3)


/*----------------------------------------------------------------------------*/
/* DEFINITION: HW_DRV____MIN_BAUDRATE & HW_DRV____MAX_BAUDRATE                */
/*----------------------------------------------------------------------------*/
/* Description:      This definitions prevent incorrect configuration of the  */
/*                   UART baudrate settings and a division by zero at         */
/*                   calculation of baudregister values.                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define HW_DRV____MIN_BAUDRATE          5ul         /* must not be zero !!!! */
#define HW_DRV____MAX_BAUDRATE          200000ul


/*----------------------------------------------------------------------------*/
/* DEFINITION: UART_MODE_XXX                                                  */
/*----------------------------------------------------------------------------*/
/* Description:      This is a global standard definition for setting a data  */
/*                   format to the UART controller.                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

                                /* BIT3:7/8Bits  BIT2:Parity  BIT1:Even(0)/Odd(1)  BIT0:1/2Stopbits */

#define UART_MODE_7N1 (UBYTE) 0X00u  /* 0 0 0 0    7 Bits, no Parity, default Even, 1 Stopbit */
#define UART_MODE_7N2 (UBYTE) 0X01u  /* 0 0 0 1    7 Bits, no Parity, default Even, 2 Stopbits */
#define UART_MODE_7E1 (UBYTE) 0X04u  /* 0 1 0 0    7 Bits, Parity, Even, 1 Stopbit */
#define UART_MODE_7E2 (UBYTE) 0X05u  /* 0 1 0 1    7 Bits, Parity, Even, 2 Stopbits */
#define UART_MODE_7O1 (UBYTE) 0X06u  /* 0 1 1 0    7 Bits, Parity, Odd, 1 Stopbit */
#define UART_MODE_7O2 (UBYTE) 0X07u  /* 0 1 1 1    7 Bits, Parity, Odd, 2 Stopbits */
#define UART_MODE_8N1 (UBYTE) 0X08u  /* 1 0 0 0    8 Bits, no Parity, default Even, 1 Stopbit */
#define UART_MODE_8N2 (UBYTE) 0X09u  /* 1 0 0 1    8 Bits, no Parity, default Even, 2 Stopbits */
#define UART_MODE_8E1 (UBYTE) 0X0Cu  /* 1 1 0 0    8 Bits, Parity, Even, 1 Stopbit */
#define UART_MODE_8E2 (UBYTE) 0X0Du  /* 1 1 0 1    8 Bits, Parity, Even, 2 Stopbits */
#define UART_MODE_8O1 (UBYTE) 0X0Eu  /* 1 1 1 0    8 Bits, Parity, Odd, 1 Stopbit */
#define UART_MODE_8O2 (UBYTE) 0X0Fu  /* 1 1 1 1    8 Bits, Parity, Odd, 2 Stopbits */

/*----------------------------------------------------------------------------*/
/* DEFINITION: HW_DRV____ERROR_XX_XX                                          */
/*----------------------------------------------------------------------------*/
/* Description:      This definitions associate a combination of error flags  */
/*                   with a unique hardware driver error handle.              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define HW_DRV____ERROR_FE        (UBYTE)  0X01u
#define HW_DRV____ERROR_PE        (UBYTE)  0X02u
#define HW_DRV____ERROR_FE_PE     (UBYTE)  0X03u

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
