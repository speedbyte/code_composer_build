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
|  Filename:  startup.h                                                        |
|                                                                              |
|  Comment:   This file includes all defines used in the startup-file          |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   KA         Markus Karas              SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Adr_Info.h 4471 2008-11-26 14:37:39Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/projekte/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/2sw/5int/01_SW_Projekt/05_header/ext/ADLATUS_Adr_Info.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/
/***********************************************************************/
/* default values for SYSTEM MODULE REGISTERS                          */
/***********************************************************************/

#define S_CLKCNTL_UW     0x0040u          /* clockout = off ; src = mclk */


#define S_GLBCNTL_UW     0x0014u          /* mult = 8 ; div = 5         */

#define S_CRAMCR_UW      0x0000
#define S_SYSECR_UW      0x4000

/***********************************************************************/

/* Flash Module*/
#define S_MFBAHR0_UW     0x0000          /* Base address = 0x00000000  */
//#define S_MFBALR0_UW     0x00A0          /* Block size   = 512k        */
#define S_MFBALR0_UW     0x00B0          /* Block size   = 1M, for 768k flash, by ZZ        */

/* Flash Module*/
#define S_MFBAHR1_UW     0x0180          /* Base address = 0x00080000  */
// #define S_MFBALR1_UW     0x0090          /* Block size   = 256k        */
#define S_MFBALR1_UW     0x0040          /* Disable memory select 1, it is for OTP. by ZZ       */

/* Internal SRAM */
#define S_MFBAHR2_UW     0x0040          /* Base address = 0x00400000  */
#define S_MFBALR2_UW     0x0060          /* Block size   = 32k         */

/* Internal SRAM */
#define S_MFBAHR3_UW     0x0040          /* Base address = 0x00408000  */
#define S_MFBALR3_UW     0x8050          /* Block size   = 16k        */

/* HET1 RAM */
#define S_MFBAHR4_UW     0x0080          /* Base address = 0x00100000  */
#define S_MFBALR4_UW     0x0020          /* Block size   = 2k          */

/* C54X DSP API */
#define S_MFBAHR5_UW     0x0000          /* Base address = 0x00000000  */
#define S_MFBALR5_UW     0x0000          /* Block size   = none        */

/* connected to nCS0 */
#define S_MCBAHR0_UW     0x0000          /* Base address = 0x00000000  */
#define S_MCBALR0_UW     0x0000          /* Block size   = none        */

/* connected to nCS0 */
#define S_MCBAHR1_UW     0x0000          /* Base address = 0x00000000  */
#define S_MCBALR1_UW     0x0000          /* Block size   = none        */

/* connected to nCS5 */
#define S_MCBAHR2_UW     0x0038          /* Base address = 0x00380000  */
#define S_MCBALR2_UW     0x0010          /* Block size   = 32k         */

/* connected to nCS6 (MT IF) */
#define S_MCBAHR3_UW     0x0060          /* Base address = 0x00600000  */
#define S_MCBALR3_UW     0x0060          /* Block size   = 1M          */

/* connected to nCS7 (MIBSPI1 RAM) */
#define S_MCBAHR4_UW     0x0050          /* Base address = 0x00500000  */
#define S_MCBALR4_UW     0x0010          /* Block size   = 32k         */

/* connected to nCS8 (MIBSPI2 RAM) */
#define S_MCBAHR5_UW     0x0058          /* Base address = 0x00580000  */
#define S_MCBALR5_UW     0x0010          /* Block size   = 32k         */


/***********************************************************************/
/*                                                                     */
/*          Memory Controller Configuration                            */
/*                                                                     */
/***********************************************************************/

/* -------------------------- */
/* n.a                        */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 0                    */
/* End = Big                  */
/* Loc = Int                  */
/* DW  = 8Bits                */
#define S_SMCR0_UW       0x0000         

/* -------------------------- */
/* HET1 RAM                   */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 7                    */
/* End = Big                  */
/* Loc = Int                  */
/* DW  = 32Bits               */
#define S_SMCR1_UW       0x0072

/* -------------------------- */
/* n.a                        */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 0                    */
/* End = Big                  */
/* Loc = Int                  */
/* DW  = 8Bits                */
#define S_SMCR2_UW       0x0000

/* -------------------------- */
/* n.a                        */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 0                    */
/* End = Big                  */
/* Loc = Int                  */
/* DW  = 8Bits                */
#define S_SMCR3_UW       0x0000

/* -------------------------- */
/* n.a                        */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 0                    */
/* End = Big                  */
/* Loc = Int                  */
/* DW  = 8Bits                */
#define S_SMCR4_UW       0x0000

/* -------------------------- */
/* HET2 RAM                   */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 7                    */
/* End = Big                  */
/* Loc = ext                  */
/* DW  = 32Bits               */
#define S_SMCR5_UW       0x0076

/* -------------------------- */
/* MT IF                      */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 3                    */
/* End = Big                  */
/* Loc = Ext                  */
/* DW  = 32Bits               */
#define S_SMCR6_UW       0x0036

/* -------------------------- */
/* MIBSPI1 RAM                */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 7                    */
/* End = Big                  */
/* Loc = ext                  */
/* DW  = 32Bits               */
#define S_SMCR7_UW       0x0076

/* -------------------------- */
/* MIBSPI2 RAM                */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 7                    */
/* End = Big                  */
/* Loc = ext                  */
/* DW  = 32Bits               */
#define S_SMCR8_UW       0x0076

/* -------------------------- */
/* n.a                        */
/* -------------------------- */
/* ASC = 0                    */
/* TWS = 0                    */
/* WS  = 0                    */
/* End = Big                  */
/* Loc = Int                  */
/* DW  = 8Bits                */
#define S_SMCR9_UW       0x0000

/***********************************************************************/
/*                                                                     */
/*              Peripheral Select Configuration                        */
/*                                                                     */
/***********************************************************************/

#define S_WCR0_UW        0x0002     /* WTWOVR = 1                      */
                                    /* WBENABLE = 0                    */

#define S_PPROT_UW       0x0000u     /* all peripherals user accesible  */
#define S_PLR_UW         0x0000u     /* enable nPS0...nPS15             */

#define S_PCR0_UW        0x0001u     /* enable periheral control        */
                                    /* ICLK = SYSCLK                   */
#define ICLKdef 20



