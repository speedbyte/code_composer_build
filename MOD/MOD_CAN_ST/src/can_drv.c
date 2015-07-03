/* Kernbauer Version: 1.11 Konfiguration: can_driver Erzeugungsgangnummer: 344 */

/* STARTSINGLE_OF_MULTIPLE */


/* KB begin CanLL_Header */
/*****************************************************************************
| Project Name: C A N - D R I V E R
|    File Name: CAN_DRV.C
|
|  Description: Implementation of the CAN driver
|               Target systems: TMS320F281x with eCAN
|                               TMS470 all SCC and HECC derivatives
|                               OMAP all HECC derivatives
|                               TMS430 all SCC derivatives
|               Compiler:       Texas Instruments Compiler (TMS320, TMS470, OMAP)
|                               ARM Development Suite      (TMS470)
|                               IAR Compiler               (TMS430)
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
|   Copyright (c) by Vector Informatik GmbH.     All rights reserved.
|
|   This software is copyright protected and proprietary to
|   Vector Informatik GmbH. Vector Informatik GmbH grants to you
|   only those rights as set out in the license conditions. All
|   other rights remain with Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Ml           Patrick Markl             Vector Informatik GmbH
| Ard          Thomas Arnold             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Version   Author  Description
|----------  --------  ------  -----------------------------------------------
|2003-02-27  00.01.00    Ml     - Creation
|2003-07-07  00.02.00    Ml     - ESCAN00005987: added support for extended and mixed ID
|2003-07-31  00.03.00    Ml     - resolved issues found in code inspection
|2003-08-01  01.00.00    Ml     - Release for TMS320
|2003-08-03  01.01.00    Ml     - ESCAN00006259: resolved clearing of CANRMP register
|2003-08-04  02.00.00    Ml     - ESCAN00006262: added support for TMS470
|2003-12-04  03.00.00    Ml     - ESCAN00007131: added support for OMAP
|2004-01-13  03.01.00    Ml     - ESCAN00007253: fixed several Issues
|2004-01-18  03.02.00    Ml     - ESCAN00007282: removed warning
|2004-02-21  03.03.00    Ml     - ESCAN00007670: added CAN RAM check
|2004-03-03  03.04.00    Ml     - ESCAN00007632: IDE bit is not set in LAM register
|2004-03-31              Ml     - ESCAN00008069: HECC workaround is configurable
|2004-04-01              Ml     - ESCAN00008079: Resolved review issues
|2004-05-17  03.05.00    Ml     - ESCAN00008423: workaround for eCAN access conflicts
|2004-07-11  03.06.00    Ml     - ESCAN00008818: handling of RxTx IRQs in sleep mode
|                        Ml     - ESCAN00006634: no changes
|                        Ml     - ESCAN00008818: Sleepmode is checked in RxTx Interrupt
|                        Ml     - ESCAN00008878: changed handling of User and Systemmode
|                        Ml     - ESCAN00008986: Wakeup IRQ activated
|                        Ml     - ESCAN00007281: resolved compilerwarning in CanTransmit
|                        Ml     - ESCAN00008986: Wakeup interrupt is enabled always
|                        Ml     - ESCAN00009115: Support multiple configurations
|                        Ml     - ESCAN00010073: BusOff IRQ is fired
|                        Ml     - ESCAN00008885: Check Wakeup in RxTx IRQ
|2004-10-19  03.07.00    Ml     - ESCAN00009135: changed handling of LoopCheck Returnvalues
|                        Ml     - ESCAN00010203: Implementation of CanStart/CanStop changed
|                        Ml     - ESCAN00010047: Set BOIM flag
|                        Ml     - ESCAN00010625: Resolved compilererror
|                        Ml     - ESCAN00007215: Added memcpy support
|                        Ml     - ESCAN10626, ESCAN10630: resolved message doubling
|2005-01-20  04.00.00    Ml     - ESCAN00011416: added support for TMS430
|2005-04-04  04.01.00    Ml     - ESCAN00011745: Changes after review and MISRA check
|                        Ml     - ESCAN00011746: Rx masking also possible in SCC mode (TMS470)
|2005-11-04  04.02.00    Ml     - ESCAN00014177: Fixed channel parameter of the timer loop check functions
|2006-02-21  04.02.01    Ard    - ESCAN00015203: Read error/status flags from CANES register correctly
|                               - ESCAN00015158: RawID macros for Standard ID
|                               - ESCAN00015341: Set priority of low level message
|                               - ESCAN00015347: Improve reception of Rx Basic/Full CAN messages
|                               - ESCAN00015265: Fix TxWrite* macros
|2007-10-05  04.02.02    Ard    - ESCAN00016178: Fix compile error if C_ENABLE_CAN_RAM_CHECK is enabled
|                               - ESCAN00022615: Fix BusOff detection
|2008-04-17  04.02.03    Ard    - ESCAN00026069: Rework Rx data copy for FullCANs to avoid inconsistent data
|***************************************************************************/
/* KB end CanLL_Header */
/*****************************************************************************
|
|    ************    Version and change information of      **********
|    ************    high level part only                   **********
|
|    Please find the version number of the whole module in the previous
|    File header.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
| Pl           Georg Pfluegel            Vector Informatik GmbH
| Vg           Frank Voorburg            Vector CANtech, Inc.
| An           Ahmad Nasser              Vector CANtech, Inc.
| Ml           Patrick Markl             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------  ----------------------------------------------------
| 19-Jan-01  0.02  Ht     - derived form C16x V3.3
| 18-Apr-01  1.00  Pl     - derived for ARM7 TDMI
| 02-May-01  1.01  Ht     - adaption to LI1.2
|                         - change from code doupling to indexed
| 31-Oct-01  1.02  Ht     - support hash search
|                  Ht     - optimisation for message access (hardware index)
|                  Vg     - adaption for PowerPC
| 07-Nov-01  1.03  Ht     - remove some comments
|                         - support of basicCAN polling extended
| 12-Dez-01  1.04  Ht     - avoid compiler warnings for KEIL C166
|                         - ESCAN00001881: warning in CanInitPowerOn
|                         - ESCAN00001913: call of CanLL_TxEnd()
|                  Fz     - ESCAN00001914: CanInterruptRestore changed for M32C
| 02-Jan-02  1.05  Ht     - ESCAN00002009: support of polling mode improved
|                         - ESCAN00002010: Prototype of CanHL_TxConfirmation()
|                                          not available in every case.
| 12-Feb-02  1.06 Pl      - ESCAN00002279: - now it is possible to use only the error-task without the tx-task
|                                          - support of the makro  REENTRANT
|                                          - support of the makro C_HL_ENABLE_RX_INFO_STRUCT_PTR
|                                          - For better performance for the T89C51C there is a switch-case
|                                            instruction for direct call of the PreTransmitfunction
|                                          - add C_ENABLE_RX_BASICCAN_POLLING in CanInitPowerOn
| 18-Mai-02  1.07 Ht      - ESCAN....    : support Hash search with FullCAN controller
|                         - ESCAN00002707: Reception could went wrong if IL and Hash Search
|                         - ESCAN00002893: adaption to LI 1.3
| 29-Mai-02  1.08 Ht      - ESCAN00003028: Transmission could fail in Polling mode
|                         - ESCAN00003082: call Can_LL_TxEnd() in CanMsgTransmit()
|                         - ESCAN00003083: Hash search with extended ID
|                         - ESCAN00003084: Support C_COMP_METROWERKS_PPC
|                         - ESCAN00002164: Temporary vaiable "i" not defined in function CanBasicCanMsgReceived
|                         - ESCAN00003085: support C_HL_ENABLE_IDTYPE_IN_ID
| 25-Jun     1.08.01 Vg   - Declared localInterruptOldFlag in CanRxTask()
|                         - Corrected call to CanWakeUp for multichannel
| 11-Jul-02  1.08.02 Ht   - ESCAN00003203: Hash Search routine does not work will with extended IDs
|                         - ESCAN00003205: Support of ranges could went wrong on some platforms
| 08-Aug-02  1.08.03 Ht   - ESCAN00003447: Transmission without databuffer and pretransmit-function
| 08-Aug-02  1.08.04 An   - Added support to Green Hills
| 09-Sep-02  1.09    Ht   - ESCAN00003837: code optimication with KernelBuilder
|                         - ESCAN00004479: change the place oft the functioncall of CanLL_TxCopyMsgTransmit
|                                          in CanMsgTransmit
| 2002-12-06 1.10    Ht   -                Support consistancy for polling tasks
|                         - ESCAN00004567: Definiton of NULL pointer
|                         -                remove include of string.h
|                         -                support Dummy functions for indirect function call
|                         -                optimization for one single Tx mail box
| 2003-02-04 1.11    Ht   -                optimization for polling mode
| 2003-03-19 1.12    Ht   - ESCAN00005152: optimization of CanInit() in case of Direct Tx Objects
|                         - ESCAN00005143: CompilerWarning about function prototype
|                                          CanHL_ReceivedRxHandle() and CanHL_IndRxHandle
|                         - ESCAN00005130: Wrong result of Heash Search on second or higher channel
| 2003-05-12 1.13    Ht   - ESCAN00005624: support CantxMsgDestroyed for multichannel system
|                         - ESCAN00005209: Support confirmation and indication flags for EasyCAN4
|                         - ESCAN00004721: Change data type of Handle in CanRxInfoStruct
| 2003-06-18 1.20   Ht    - ESCAN00005908: support features of RI1.4
|                         - ESCAN: Support FJ16LX-Workaround for multichannel system
|                         - ESCAN00005671: Dynamic Transmit Objects: ID in extended ID frames is wrong
|                         - ESCAN00005863: Notification about cancelation failes in case of CanTxMsgDestroyed
| 2003-06-30 1.21   Ht   - ESCAN00006117: Common Confirmation Function: Write access to wrong memory location
|                        - ESCAN00006008: CanCanInterruptDisable in case of polling
|                        - ESCAN00006118: Optimization for Mixed ID and ID type in Own Register or ID type in ID Register
|                        - ESCAN00006100: transmission with wrong ID in mixed ID mode
|                        - ESCAN00006063: Undesirable hardware dependency for
|                                         CAN_HL (CanLL_RxBasicTxIdReceived)
| 2003-09-10 1.22   Ht   - ESCAN00006853: Support V_MEMROM0
|                        - ESCAN00006854: suppress some Compiler warnings
|                        - ESCAN00006856: support CanTask if only Wakeup in polling mode
|                        - ESCAN00006857: variable newDLC not defined in case of Variable DataLen
| 2003-10-14 1.23   Ht   - ESCAN00006858: support BrsTime for internal runtime measurement
|                        - ESCAN00006860: support Conditional Msg Receive
|                        - ESCAN00006865: support "Cancel in HW" with CanTask
|                        - ESCAN00006866: support Direct Tx Objects
|                        - ESCAN00007109: support new memory qualifier for const data and pointer to const
| 2004-01-05 1.24   Ml   - ESCAN00007206: resolved preprocessor error for Hitachi compiler
|                   Ml   - ESCAN00007254: several changes
| 2004-02-06 1.25   Ml   - ESCAN00007281: solved compilerwarning
|                   Ml   - removed compiler warnings
| 2004-02-21 1.26   Ml   - ESCAN00007670: CAN RAM check
|                   Ml   - ESCAN00007671: fixed dyn Tx object issue
|                   Ml   - ESCAN00007764: added possibility to adjust Rx handle in LL drv
|                   Ml   - ESCAN00007681: solved compilerwarning in CanHL_IndRxHandle
|                   Ml   - ESCAN00007272: solved queue transmission out of LowLevel object
|                   Ml   - ESCAN00008064: no changes
| 2004-04-16 1.27   Ml   - ESCAN00008204: no changes
|                   Ml   - ESCAN00008160: no changes
|                   Ml   - ESCAN00008266: changed name of parameter of function CanTxGetActHandle
|                   Fz   - ESCAN00008272: Compiler error due to missing array canPollingTaskActive
| 2004-05-10 1.28   Fz   - ESCAN00008328: Compiler error if cancel in hardware is active
|                        - ESCAN00008363: Hole closed when TX in interrupt and cancel in HW is used
|                        - ESCAN00008365: Switch C_ENABLE_APPLCANPREWAKEUP_FCT added
|                        - ESCAN00008391: Wrong parameter macro used in call of
|                                         CanLL_WakeUpHandling
| 2004-05-24 1.29   Ht   - ESCAN00008441: Interrupt not restored in case of internal error if TX Polling is used
| 2004-09-21 1.30   Ht   - ESCAN00008914: CAN channel may stop transmission for a certain time
|                        - ESCAN00008824: check of reference implementation version added
|                        - ESCAN00008825: No call of ApplCanMsgCancelNotification during CanInit()
|                        - ESCAN00008826: Support asssertions for "Conditional Message Received"
|                   Ml   - ESCAN00008752: Added function qualifier macros
|                   Ht   - ESCAN00008823: compiler error due to array size 0
|                        - ESCAN00008977: label without instructions
|                        - ESCAN00009485: Message via Normal Tx Object will not be sent
|                        - ESCAN00009497: support of CommonCAN and RX queue added
|                        - ESCAN00009521: Inconsitancy in total polling mode
| 2004-09-28 1.31   Ht   - ESCAN00009703: unresolved functions CAN_POLLING_IRQ_DISABLE/RESTORE()
| 2004-11-25 1.32   Ht   - move fix for ESCAN00007671 to CAN-LL of DrvCan_MpcToucanHll
|                        - ESCAN00010350: Dynamic Tx messages are send always with Std. ID
|                        - ESCAN00010388: ApplCanMsgConfirmed will only be called if realy transmitted
|                    Ml  - ESCAN00009931: The HardwareLoopCheck should have a channelparameter in multichannel systems.
|                    Ml  - ESCAN00010093: lint warning: function type inconsistent "CanCheckMemory"
|                    Ht  - ESCAN00010811: remove Misra and compiler warnings
|                        - ESCAN00010812: support Multi ECU
|                        - ESCAN00010526: CAN interrupts will be disabled accidently
|                        - ESCAN00010584: ECU may crash or behave strange with Rx queue active
| 2005-01-20 1.33    Ht  - ESCAN00010877: ApplCanMsgTransmitConf() is called erronemous
|
|
|    ************    Version and change information of      **********
|    ************    high level part only                   **********
|
|    Please find the version number of the whole module in the previous
|    File header.
|
|***************************************************************************/

#define C_DRV_INTERNAL

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* KB begin canLL_Include */
/* KB end canLL_Include */

#include "can_inc.h"

/* KB begin canLL_PostInclude */
/* KB end canLL_PostInclude */

/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
/* KB begin canLL_VersionCheck */
#if(CAN_DRIVER_VERSION != 0x0402)
# error "Source and Header file are inconsistent!"
#endif

#if(CAN_BUGFIX_VERSION != 0x03)
# error "Source and Header file are inconsistent!"
#endif
/* KB end canLL_VersionCheck */

#if( C_VERSION_REF_IMPLEMENTATION != 0x140)
# error "Generated Data and CAN driver source file are inconsistent!"
#endif

#if( DRVCAN__COREHLL_VERSION != 0x0133)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN__COREHLL_RELEASE_VERSION != 0x00)
# error "Source and Header file are inconsistent!"
#endif


/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/

#if !defined(NULL)
# define NULL ((void *)0)
#endif

/* ##RI-1.1: Object parameters for Tx-Observe functions */
/* status of transmit objects */
#define kCanBufferFree            ((CanTransmitHandle)0xFFFFFFFFU)   /* mark a transmit object is free */
#define kCanBufferCancel          ((CanTransmitHandle)0xFFFFFFFEU)   /* mark a transmit object as canceled */
#define kCanBufferMsgDestroyed    ((CanTransmitHandle)0xFFFFFFFDU)   /* mark a transmit object as destroyed */
/* reserved value because of definition in header:    0xFFFFFFFCU */
/* valid transmit message handle:   0x0 to kCanNumberOfTxObjects   */

/* return values */
#define kCanHlFinishRx            ((canuint8)0x00)
#define kCanHlContinueRx          ((canuint8)0x01)

/* KB begin CanLL_Defines */
/*lint -e793 allow more than 511 distinct identifiers */

/* Mailbox Enable Register (CANME)                */
#define kCanDisableAllObj    ((tCanRegType)0x00000000)

/* Master Control Register (CANMC)                */
/*lint -esym(750, kCanMbcc,kCanTcc,kCanScm,kCanPdr,kCanDbo,kCanWuba,kCanCdr,kCanAbo,kCanStm) */
#define kCanMbcc      ((tCanRegType)0x00008000)
#define kCanTcc       ((tCanRegType)0x00004000)
#define kCanScm       ((tCanRegType)0x00002000)
#define kCanCcr       ((tCanRegType)0x00001000)
#define kCanPdr       ((tCanRegType)0x00000800)
#define kCanDbo       ((tCanRegType)0x00000400)
#define kCanWuba      ((tCanRegType)0x00000200)
#define kCanCdr       ((tCanRegType)0x00000100)
#define kCanAbo       ((tCanRegType)0x00000080)
#define kCanStm       ((tCanRegType)0x00000040)
#define kCanSres      ((tCanRegType)0x00000020)

/* Bit-Timing Configuration Register (CANBTC)     */
#define kCanTSegMask ((vuint32)0x0000007F)

/* Error and Status Register (CANES)              */
/*lint -esym(750, kCanFe,kCanBe,kCanSa1,kCanCrce,kCanSe,kCanAcke,kCanBo,kCanEp,kCanEw,kCanSma,kCanRm,kCanTm) */

# define kCanFe        ((tCanRegType)0x01000000)
# define kCanBe        ((tCanRegType)0x00800000)
# define kCanSa1       ((tCanRegType)0x00400000)
# define kCanCrce      ((tCanRegType)0x00200000)
# define kCanSe        ((tCanRegType)0x00100000)
# define kCanAcke      ((tCanRegType)0x00080000)
# define kCanBo        ((tCanRegType)0x00040000)
# define kCanEp        ((tCanRegType)0x00020000)
# define kCanEw        ((tCanRegType)0x00010000)
#define kCanSma       ((tCanRegType)0x00000020)
#define kCanCce       ((tCanRegType)0x00000010)
#define kCanPda       ((tCanRegType)0x00000008)
#define kCanRm        ((tCanRegType)0x00000002)
#define kCanTm        ((tCanRegType)0x00000001)
#define kCanErrorMask ((tCanRegType)0x01BF0000)

/* Global Interrupt Flag Register 0 CANGIF0       */
/*lint -esym(750, kCanMtof0,kCanTcof0,kCanGmif0,kCanAaif0,kCanWdif0,kCanWuif0,kCanRmlif0,kCanBoif0,kCanEpif0,kCanWlif0,kCanMivMask) */
#define kCanMtof0     ((tCanRegType)0x00020000)
#define kCanTcof0     ((tCanRegType)0x00010000)
#define kCanGmif0     ((tCanRegType)0x00008000)
#define kCanAaif0     ((tCanRegType)0x00004000)
#define kCanWdif0     ((tCanRegType)0x00002000)
#define kCanWuif0     ((tCanRegType)0x00001000)
#define kCanRmlif0    ((tCanRegType)0x00000800)
#define kCanBoif0     ((tCanRegType)0x00000400)
#define kCanEpif0     ((tCanRegType)0x00000200)
#define kCanWlif0     ((tCanRegType)0x00000100)
#define kCanMivMask   ((tCanRegType)0x0000001F)

/* Global Interrupt Flag Register 1 CANGIF1       */
/*lint -esym(750, kCanMtof1,kCanTcof1,kCanGmif1,kCanAaif1,kCanWdif1,kCanWuif1,kCanRmlif1,kCanBoif1,kCanEpif1,kCanWlif1) */
#define kCanMtof1     ((tCanRegType)0x00020000)
#define kCanTcof1     ((tCanRegType)0x00010000)
#define kCanGmif1     ((tCanRegType)0x00008000)
#define kCanAaif1     ((tCanRegType)0x00004000)
#define kCanWdif1     ((tCanRegType)0x00002000)
#define kCanWuif1     ((tCanRegType)0x00001000)
#define kCanRmlif1    ((tCanRegType)0x00000800)
#define kCanBoif1     ((tCanRegType)0x00000400)
#define kCanEpif1     ((tCanRegType)0x00000200)
#define kCanWlif1     ((tCanRegType)0x00000100)

/* Global Interrupt Mask Register (CANGIM)        */
/*lint -esym(750, kCanMtom,kCanTcom,kCanAaim,kCanWdim,kCanWuim,kCanRmlim,kCanBoim,kCanEpim,kCanWlim,kCanGil,kCanI1en,kCanI0en) */
#define kCanMtom      ((tCanRegType)0x00020000)
#define kCanTcom      ((tCanRegType)0x00010000)
#define kCanAaim      ((tCanRegType)0x00004000)
#define kCanWdim      ((tCanRegType)0x00002000)
#define kCanWuim      ((tCanRegType)0x00001000)
#define kCanRmlim     ((tCanRegType)0x00000800)
#define kCanBoim      ((tCanRegType)0x00000400)
#define kCanEpim      ((tCanRegType)0x00000200)
#define kCanWlim      ((tCanRegType)0x00000100)
#define kCanGil       ((tCanRegType)0x00000004)
#define kCanI1en      ((tCanRegType)0x00000002)
#define kCanI0en      ((tCanRegType)0x00000001)

/* Mailbox Interrupt Mask Register (CANMIM)       */
/* Mailbox Interrupt Level Register (CANMIL)      */
#define kCanAllLevel0 ((tCanRegType)0x00000000)
#define kCanAllLevel1 ((tCanRegType)0xFFFFFFFF)

/* I/O Tx Control Registers (CANTIOC)             */
/*lint -esym(750, kCanTxDir,kCanTxOut,kCanTxIn)   */
# define kCanTxFunc    ((tCanRegType)0x00000008)
# define kCanTxDir     ((tCanRegType)0x00000004)
# define kCanTxOut     ((tCanRegType)0x00000002)
# define kCanTxIn      ((tCanRegType)0x00000001)
/* I/O Rx Control Registers (CANRIOC)             */
/*lint -esym(750, kCanRxDir,kCanRxOut,kCanRxIn)   */
# define kCanRxFunc    ((tCanRegType)0x00000008)
# define kCanRxDir     ((tCanRegType)0x00000004)
# define kCanRxOut     ((tCanRegType)0x00000002)
# define kCanRxIn      ((tCanRegType)0x00000001)

/* Message Identifier Register (CANMID)           */
/*lint -esym(750, kCanIde,kCanAam) */
#define kCanIde       ((vuint32)0x80000000)
#define kCanIdeNeg    ((vuint32)0x7FFFFFFF)
#define kCanAme       ((vuint32)0x40000000)
#define kCanAam       ((vuint32)0x20000000)

#if defined(C_SINGLE_RECEIVE_CHANNEL)
# define kCanBasicCanMask ((tCanRegType)((~kCanRxFullCANObjMask) & (tCanRegType)0x0000000F))
#else
# define kCanBasicCanMask ((tCanRegType)((~(CanRxFullCANObjMask[channel])) & (tCanRegType)0x0000000F))
#endif

#if defined(C_COMP_TI_TMS470)
# define kCanNumberOfMaxMailboxes ((tCanRegType)32)
#endif


#if defined(C_COMP_TI_TMS470)
/* CPSR */
# define kModeBits     ((tCanRegType)0x0000000f)
# define kUserMode     ((tCanRegType)0x00000000)
# define kIrqFlag      ((tCanRegType)0x00000080)
#endif



/* KB end CanLL_Defines */

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

#if !(defined ( C_HL_DISABLE_RX_INFO_STRUCT_PTR ) || defined ( C_HL_ENABLE_RX_INFO_STRUCT_PTR ))
# define C_HL_ENABLE_RX_INFO_STRUCT_PTR
#endif

#if defined  ( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
# define CAN_HL_P_RX_INFO_STRUCT(channel)  (pCanRxInfoStruct)
#else
# define CAN_HL_P_RX_INFO_STRUCT(channel)  (&canRxInfoStruct[channel])
#endif


/* define datatype of local signed variables for message handles */
# if defined( C_CPUTYPE_8BIT ) && (kCanNumberOfTxObjects > 128)
#  define CanSignedTxHandle  cansint16
# else
#  define CanSignedTxHandle  CANSINTX
# endif

# if defined( C_CPUTYPE_8BIT ) && (kCanNumberOfRxObjects > 128)
#  define CanSignedRxHandle  cansint16
# else
#  if defined( C_CPUTYPE_8BIT ) && defined ( C_SEARCH_HASH )
#   if ((kHashSearchListCountEx > 128) || (kHashSearchListCount > 128))
#    define CanSignedRxHandle  cansint16
#   else
#    define CanSignedRxHandle  CANSINTX
#   endif
#  else
#   define CanSignedRxHandle  CANSINTX
#  endif
# endif


/*disabled - lint -emacro( (572,778), C_RANGE_MATCH) */


#if defined ( C_SINGLE_RECEIVE_CHANNEL )
# if (kCanNumberOfUsedCanRxIdTables == 1)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                                    \
                  (  ((idRaw0) & (tCanRxId0)~MK_RX_RANGE_MASK0(mask)) == MK_RX_RANGE_CODE0(code) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                                          \
                                (  (canuint8)(XT_ID_LO((idRaw0) & MK_RX_RANGE_MASK0(mask)) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 2)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                             \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK0(mask)) == MK_RX_RANGE_CODE0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK1(mask)) == MK_RX_RANGE_CODE1(code) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                                  \
                                (  (canuint8)(XT_ID_LO((idRaw0) & MK_RX_RANGE_MASK0(mask),  \
                                                       (idRaw1) & MK_RX_RANGE_MASK1(mask)) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 3)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                      \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK0(mask)) == MK_RX_RANGE_CODE0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK1(mask)) == MK_RX_RANGE_CODE1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK2(mask)) == MK_RX_RANGE_CODE2(code) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                           \
                                (  (canuint8)(XT_ID_LO((idRaw0) & MK_RX_RANGE_MASK0(mask),  \
                                                       (idRaw1) & MK_RX_RANGE_MASK1(mask),  \
                                                       (idRaw2) & MK_RX_RANGE_MASK2(mask)) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 4)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                      \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK0(mask)) == MK_RX_RANGE_CODE0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK1(mask)) == MK_RX_RANGE_CODE1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK2(mask)) == MK_RX_RANGE_CODE2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK3(mask)) == MK_RX_RANGE_CODE3(code) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                           \
                                (  (canuint8)(XT_ID_LO((idRaw0) & MK_RX_RANGE_MASK0(mask),  \
                                                       (idRaw1) & MK_RX_RANGE_MASK1(mask),  \
                                                       (idRaw2) & MK_RX_RANGE_MASK2(mask),  \
                                                       (idRaw3) & MK_RX_RANGE_MASK3(mask)) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 5)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                      \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK0(mask)) == MK_RX_RANGE_CODE0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK1(mask)) == MK_RX_RANGE_CODE1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK2(mask)) == MK_RX_RANGE_CODE2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK3(mask)) == MK_RX_RANGE_CODE3(code) ) && \
                    ( ((idRaw4) & (tCanRxId4)~ MK_RX_RANGE_MASK4(mask)) == MK_RX_RANGE_CODE4(code) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                           \
                                (  (canuint8)(XT_ID_LO((idRaw0) & MK_RX_RANGE_MASK0(mask),  \
                                                       (idRaw1) & MK_RX_RANGE_MASK1(mask),  \
                                                       (idRaw2) & MK_RX_RANGE_MASK2(mask),  \
                                                       (idRaw3) & MK_RX_RANGE_MASK3(mask),  \
                                                       (idRaw4) & MK_RX_RANGE_MASK4(mask)) ) )
# endif
#else     /* C_MULTIPLE_RECEIVE_CHANNEL */

# if (kCanNumberOfUsedCanRxIdTables == 1)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                                    \
                                (  ((idRaw0) & (tCanRxId0)~(mask[0])) == (code[0]) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                                         \
                                (  (canuint8)(XT_ID_LO((idRaw0) & mask[0])) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 2)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                             \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask[0])) == (code[0]) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask[1])) == (code[1]) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                                  \
                                (  (canuint8)(XT_ID_LO((idRaw0) & mask[0],  \
                                                       (idRaw1) & mask[1])) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 3)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                      \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask[0])) == (code[0]) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask[1])) == (code[1]) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~(mask[2])) == (code[2]) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                           \
                                (  (canuint8)(XT_ID_LO((idRaw0) & mask[0],  \
                                                       (idRaw1) & mask[1],  \
                                                       (idRaw2) & mask[2])) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 4)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                      \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask[0])) == (code[0]) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask[1])) == (code[1]) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~(mask[2])) == (code[2]) ) &&\
                                  ( ((idRaw3) & (tCanRxId3)~(mask[3])) == (code[3]) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                           \
                                (  (canuint8)(XT_ID_LO((idRaw0) & mask[0],  \
                                                       (idRaw1) & mask[1],  \
                                                       (idRaw2) & mask[2],  \
                                                       (idRaw3) & mask[3])) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 5)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)                      \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask[0])) == (code[0]) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask[1])) == (code[1]) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~(mask[2])) == (code[2]) ) &&\
                                  ( ((idRaw3) & (tCanRxId3)~(mask[3])) == (code[3]) ) &&\
                                  ( ((idRaw4) & (tCanRxId4)~(mask[4])) == (code[4]) ) )
#  define C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, mask)                           \
                                (  (canuint8)(XT_ID_LO((idRaw0) & mask[0],  \
                                                       (idRaw1) & mask[1],  \
                                                       (idRaw2) & mask[2],  \
                                                       (idRaw3) & mask[3],  \
                                                       (idRaw4) & mask[4])) )
# endif
#endif


#if (kCanNumberOfUsedCanRxIdTables == 1)
# define CAN_RX_IDRAW_PARA     idRaw0
#endif
#if (kCanNumberOfUsedCanRxIdTables == 2)
# define CAN_RX_IDRAW_PARA     idRaw0,idRaw1
#endif
#if (kCanNumberOfUsedCanRxIdTables == 3)
# define CAN_RX_IDRAW_PARA     idRaw0,idRaw1,idRaw2
#endif
#if (kCanNumberOfUsedCanRxIdTables == 4)
# define CAN_RX_IDRAW_PARA     idRaw0,idRaw1,idRaw2,idRaw3
#endif
#if (kCanNumberOfUsedCanRxIdTables == 5)
# define CAN_RX_IDRAW_PARA     idRaw0,idRaw1,idRaw2,idRaw3,idRaw4
#endif


#if defined ( C_SINGLE_RECEIVE_CHANNEL )
# define channel                                     ((CanChannelHandle)0)
# define canHwChannel                                ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STARTINDEX(channel)       ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STOPINDEX(channel)        ((CanChannelHandle)0)
# define CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel)  (kCanMsgTransmitObj)
# define CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)     (kCanHwTxNormalIndex)

/* Offset which has to be added to change the hardware Tx handle into a logical handle, which is unique over all channels */
/*        Tx-Hardware-Handle - CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) */
# define CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)    ((vsintx)(0-kCanHwTxStartIndex))

# define CAN_HL_TX_STARTINDEX(channel)               ((CanTransmitHandle)0)
# define CAN_HL_TX_STAT_STARTINDEX(channel)          ((CanTransmitHandle)0)
# define CAN_HL_TX_DYN_ROM_STARTINDEX(channel)       (kCanNumberOfTxStatObjects)
# define CAN_HL_TX_DYN_RAM_STARTINDEX(channel)       ((CanTransmitHandle)0)
# define CAN_HL_RX_STARTINDEX(channel)               ((CanReceiveHandle)0)
/* index to access the ID tables - Basic index only for linear search
   for hash search this is the start index of the ??? */
# define CAN_HL_RX_BASIC_STARTINDEX(channel)         ((CanReceiveHandle)0)
# if defined( C_SEARCH_HASH )
#  define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)    ((CanReceiveHandle)0)
# else
#  define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)    (kCanNumberOfRxBasicCANObjects)
# endif
# define CAN_HL_INIT_OBJ_STARTINDEX(channel)         ((canuint8)0)
# define CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel)   ((CanObjectHandle)0)
# define CAN_HL_HW_TX_STARTINDEX(canHwChannel)       (kCanHwTxStartIndex)
# define CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)  (kCanHwRxFullStartIndex)
# define CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel) (kCanHwRxBasicStartIndex)
# define CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel)   (kCanHwUnusedStartIndex)

# define CAN_HL_TX_STOPINDEX(channel)                (kCanNumberOfTxObjects)
# define CAN_HL_TX_STAT_STOPINDEX(channel)           (kCanNumberOfTxStatObjects)
# define CAN_HL_TX_DYN_ROM_STOPINDEX(channel)        (kCanNumberOfTxObjects)
# define CAN_HL_TX_DYN_RAM_STOPINDEX(channel)        (kCanNumberOfTxDynObjects)
# define CAN_HL_RX_STOPINDEX(channel)                (kCanNumberOfRxObjects)
# define CAN_HL_RX_BASIC_STOPINDEX(channel)          (kCanNumberOfRxBasicCANObjects)
# if defined( C_SEARCH_HASH )
#  define CAN_HL_RX_FULL_STOPINDEX(canHwChannel)     (kCanNumberOfRxFullCANObjects)
# else
#  define CAN_HL_RX_FULL_STOPINDEX(canHwChannel)     (kCanNumberOfRxBasicCANObjects+kCanNumberOfRxFullCANObjects)
# endif
# define CAN_HL_INIT_OBJ_STOPINDEX(channel)          (kCanNumberOfInitObjects)
# define CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel)    (kCanNumberOfUsedTxCANObjects)
# define CAN_HL_HW_TX_STOPINDEX(canHwChannel)        (kCanHwTxStartIndex     +kCanNumberOfUsedTxCANObjects)
# define CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel)   (kCanHwRxFullStartIndex +kCanNumberOfRxFullCANObjects)
# define CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel)  (kCanHwRxBasicStartIndex+kCanNumberOfUsedRxBasicCANObjects)
# define CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel)    (kCanHwUnusedStartIndex +kCanNumberOfUnusedObjects)

#else
#  define canHwChannel                               (channel)
#  define CAN_HL_HW_CHANNEL_STARTINDEX(channel)      (channel)
#  define CAN_HL_HW_CHANNEL_STOPINDEX(channel)       (channel)

# define CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel)  (CanHwMsgTransmitIndex[(canHwChannel)])
# define CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)     (CanHwTxNormalIndex[(canHwChannel)])
/* Offset which has to be added to change the hardware Tx handle into a logical handle, which is unique over all channels */
/*        Tx-Hardware-Handle - CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) */
# define CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)    (CanTxOffsetHwToLog[(canHwChannel)])

# define CAN_HL_TX_STARTINDEX(channel)               (CanTxStartIndex[(channel)])
# define CAN_HL_TX_STAT_STARTINDEX(channel)          (CanTxStartIndex[(channel)])
# define CAN_HL_TX_DYN_ROM_STARTINDEX(channel)       (CanTxDynRomStartIndex[(channel)])
# define CAN_HL_TX_DYN_RAM_STARTINDEX(channel)       (CanTxDynRamStartIndex[(channel)])
# define CAN_HL_RX_STARTINDEX(channel)               (CanRxStartIndex[(channel)])
/* index to access the ID tables - Basic index only for linear search */
# define CAN_HL_RX_BASIC_STARTINDEX(channel)         (CanRxBasicStartIndex[(channel)])
# define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)     (CanRxFullStartIndex[(canHwChannel)])
# define CAN_HL_INIT_OBJ_STARTINDEX(channel)         (CanInitObjectStartIndex[(channel)])
# define CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel)   (CanLogHwTxStartIndex[(canHwChannel)])
# define CAN_HL_HW_TX_STARTINDEX(canHwChannel)       (CanHwTxStartIndex[(canHwChannel)])
# define CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)  (CanHwRxFullStartIndex[(canHwChannel)])
# define CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel) (CanHwRxBasicStartIndex[(canHwChannel)])
# define CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel)   (CanHwUnusedStartIndex[(canHwChannel)])

# define CAN_HL_TX_STOPINDEX(channel)                (CanTxStartIndex[(channel) + 1])
# define CAN_HL_TX_STAT_STOPINDEX(channel)           (CanTxDynRomStartIndex[(channel)])
# define CAN_HL_TX_DYN_ROM_STOPINDEX(channel)        (CanTxStartIndex[(channel) + 1])
# define CAN_HL_TX_DYN_RAM_STOPINDEX(channel)        (CanTxDynRamStartIndex[(channel) + 1])
# define CAN_HL_RX_STOPINDEX(channel)                (CanRxStartIndex[(channel) + 1])
/* index to access the ID tables - Basic index only for linear search */
# define CAN_HL_RX_BASIC_STOPINDEX(channel)          (CanRxFullStartIndex[CAN_HL_HW_CHANNEL_STARTINDEX(channel)])
# define CAN_HL_INIT_OBJ_STOPINDEX(channel)          (CanInitObjectStartIndex[(channel) + 1])
# define CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel)    (CanLogHwTxStartIndex[(canHwChannel) +1])
# define CAN_HL_HW_TX_STOPINDEX(canHwChannel)        (CanHwTxStopIndex[(canHwChannel)])
# define CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel)   (CanHwRxFullStopIndex[(canHwChannel)])
# define CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel)  (CanHwRxBasicStopIndex[(canHwChannel)])
# define CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel)    (CanHwUnusedStopIndex[(canHwChannel)])

#endif


#if defined ( C_SINGLE_RECEIVE_CHANNEL )

# define CANRANGE0ACCMASK(i)        C_RANGE0_ACC_MASK
# define CANRANGE0ACCCODE(i)        C_RANGE0_ACC_CODE
# define CANRANGE1ACCMASK(i)        C_RANGE1_ACC_MASK
# define CANRANGE1ACCCODE(i)        C_RANGE1_ACC_CODE
# define CANRANGE2ACCMASK(i)        C_RANGE2_ACC_MASK
# define CANRANGE2ACCCODE(i)        C_RANGE2_ACC_CODE
# define CANRANGE3ACCMASK(i)        C_RANGE3_ACC_MASK
# define CANRANGE3ACCCODE(i)        C_RANGE3_ACC_CODE

# define APPL_CAN_MSG_RECEIVED( i ) ApplCanMsgReceived( i )

# define APPLCANRANGE0PRECOPY( i )  ApplCanRange0Precopy( i )
# define APPLCANRANGE1PRECOPY( i )  ApplCanRange1Precopy( i )
# define APPLCANRANGE2PRECOPY( i )  ApplCanRange2Precopy( i )
# define APPLCANRANGE3PRECOPY( i )  ApplCanRange3Precopy( i )

# define APPL_CAN_BUSOFF( i )       ApplCanBusOff()
# define APPL_CAN_WAKEUP( i )       ApplCanWakeUp()

# if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
#  define APPLCANCANCELNOTIFICATION( i, j ) ApplCanCancelNotification( j )
# else
#  define APPLCANCANCELNOTIFICATION( i, j )
# endif
# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#  define APPLCANMSGCANCELNOTIFICATION( i ) ApplCanMsgCancelNotification()
# else
#  define APPLCANMSGCANCELNOTIFICATION( i )
# endif


#else

# define CANRANGE0ACCMASK(i)        (CanChannelObject[i].RangeMask[0])
# define CANRANGE0ACCCODE(i)        (CanChannelObject[i].RangeCode[0])
# define CANRANGE1ACCMASK(i)        (CanChannelObject[i].RangeMask[1])
# define CANRANGE1ACCCODE(i)        (CanChannelObject[i].RangeCode[1])
# define CANRANGE2ACCMASK(i)        (CanChannelObject[i].RangeMask[2])
# define CANRANGE2ACCCODE(i)        (CanChannelObject[i].RangeCode[2])
# define CANRANGE3ACCMASK(i)        (CanChannelObject[i].RangeMask[3])
# define CANRANGE3ACCCODE(i)        (CanChannelObject[i].RangeCode[3])

# define APPL_CAN_MSG_RECEIVED( i )    (CanChannelObject[(i)->Channel].ApplCanMsgReceivedFct(i))

# define APPLCANRANGE0PRECOPY( i )  (CanChannelObject[(i)->Channel].ApplCanRangeFct[0](i))
# define APPLCANRANGE1PRECOPY( i )  (CanChannelObject[(i)->Channel].ApplCanRangeFct[1](i))
# define APPLCANRANGE2PRECOPY( i )  (CanChannelObject[(i)->Channel].ApplCanRangeFct[2](i))
# define APPLCANRANGE3PRECOPY( i )  (CanChannelObject[(i)->Channel].ApplCanRangeFct[3](i))

# define APPL_CAN_BUSOFF( i )         (CanChannelObject[i].ApplCanBusOffFct(i))
# define APPL_CAN_WAKEUP( i )         (CanChannelObject[i].ApplCanWakeUpFct(i))

# if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
#  define APPLCANCANCELNOTIFICATION( i, j ) (CanChannelObject[i].ApplCanCancelNotificationFct( j ))
# else
#  define APPLCANCANCELNOTIFICATION( i, j )
# endif

# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#  define APPLCANMSGCANCELNOTIFICATION( i ) (CanChannelObject[i].ApplCanMsgTransmitCancelNotifyFct( i ))
# else
#  define APPLCANMSGCANCELNOTIFICATION( i )
# endif

#endif

#if defined ( C_SINGLE_RECEIVE_CHANNEL )
# define CAN_HW_CHANNEL_CANTYPE_ONLY           void
# define CAN_HW_CHANNEL_CANTYPE_FIRST
# define CAN_HW_CHANNEL_CANTYPE_LOCAL
# define CAN_HW_CHANNEL_CANPARA_ONLY
# define CAN_HW_CHANNEL_CANPARA_FIRST
#else
# define CAN_HW_CHANNEL_CANTYPE_ONLY          CanChannelHandle canHwChannel
# define CAN_HW_CHANNEL_CANTYPE_FIRST         CanChannelHandle canHwChannel,
# define CAN_HW_CHANNEL_CANTYPE_LOCAL         CanChannelHandle canHwChannel;
# define CAN_HW_CHANNEL_CANPARA_ONLY          canHwChannel
# define CAN_HW_CHANNEL_CANPARA_FIRST         canHwChannel,
#endif

#ifndef CAN_POLLING_IRQ_DISABLE
# define CAN_POLLING_IRQ_DISABLE(i)  CanCanInterruptDisable(i)
#endif
#ifndef CAN_POLLING_IRQ_RESTORE
# define CAN_POLLING_IRQ_RESTORE(i)  CanCanInterruptRestore(i)
#endif

/* mask for range enable status */
#define kCanRange0  ((canuint16)1)
#define kCanRange1  ((canuint16)2)
#define kCanRange2  ((canuint16)4)
#define kCanRange3  ((canuint16)8)


/* Assertions ----------------------------------------------------------------*/
#if defined( C_ENABLE_USER_CHECK )
# if defined ( C_SINGLE_RECEIVE_CHANNEL )
#  define assertUser(p,c,e)     if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertUser(p,c,e)     if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertUser(a,c,b)
#endif

#if defined ( C_ENABLE_GEN_CHECK )
# if defined ( C_SINGLE_RECEIVE_CHANNEL )
#  define assertGen(p,c,e)      if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertGen(p,c,e)      if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertGen(a,c,b)
#endif

#if defined ( C_ENABLE_HARDWARE_CHECK )
# if defined ( C_SINGLE_RECEIVE_CHANNEL )
#  define assertHardware(p,c,e) if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertHardware(p,c,e) if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertHardware(a,c,b)
#endif

#if defined ( C_ENABLE_INTERNAL_CHECK )
# if defined ( C_SINGLE_RECEIVE_CHANNEL )
#  define assertInternal(p,c,e) if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertInternal(p,c,e) if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertInternal(p,c,e)
#endif

/* KB begin CanLL_Macros */
/* Macros for global Interrupt enable/disable----------------------------------*/
/* between Disable and Restore there mustn't be any function calls */


#if defined(C_COMP_TI_TMS470)
# define CanLL_GlobalInterruptDisable(localInterruptOldFlagPtr)  \
{                                                                \
  *localInterruptOldFlagPtr = CanGetCpsr();                      \
  CanDisableIrq();                                               \
}

# define CanLL_GlobalInterruptRestore(localInterruptOldFlag)     \
{                                                                \
  if((localInterruptOldFlag & kIrqFlag) == 0x00000000){          \
  CanEnableIrq(); }                                              \
}

/* Macros for Can-Interrupt enable/disable-----------------------------------*/
/* this macros are only neccessary, if polling and interrupt is mixed or     */
/* if several receive interrupts are available.                              */

# define CanLL_CanInterruptDisable(channel, localInterruptOldFlagPtr)  /* supress warning */
# define CanLL_CanInterruptRestore(channel, localInterruptOldFlag)     /* not used for TMS470 */
#endif


#if defined(C_COMP_TI_TMS470)
# define CAN_IS_USERMODE_ACTIVE()  ((CanGetCpsr() & kModeBits) == kUserMode)
#endif

/*lint -e773 no parenthesis around macro*/
#if defined(C_COMP_TI_TMS470)
# if defined(C_ENABLE_NO_USERMODE)
#  define CAN_WRITE_REGISTER(a,m,v)  (*(volatile vuint32*)(a)) = (v)
# else
#  if defined(C_DISABLE_PRIVILEDGED_DRIVER)
#   define CAN_WRITE_REGISTER(a,m,v) CanWriteProtectedRegister((a),(m),(v))
#  else
#   define CAN_WRITE_REGISTER(a,m,v)  CanWritePriviledged((a),(v))
#  endif
# endif
#endif



/*lint +e773 no parenthesis around macro*/

#if defined(C_ENABLE_HW_LOOP_TIMER)
# define APPLCANTIMERSTART(a) ApplCanTimerStart(CAN_CHANNEL_CANPARA_FIRST (a))
# define APPLCANTIMERLOOP(a)  ApplCanTimerLoop(CAN_CHANNEL_CANPARA_FIRST (a))
# define APPLCANTIMEREND(a)   ApplCanTimerEnd(CAN_CHANNEL_CANPARA_FIRST (a))
#else
# define APPLCANTIMERSTART(a)
# define APPLCANTIMERLOOP(a)  1
# define APPLCANTIMEREND(a)
#endif

#if defined(C_SINGLE_RECEIVE_CHANNEL)
# define CAN_CNTRL_BASIS_ADR(channel)    ((tCanCntrlRegBlock MEMORY_CAN *)(kCanBasisAdr))
# define CAN_MAILBOX_BASIS_ADR(channel)  ((tCanMailBox MEMORY_CAN *)(kCanMailboxBasisAdr))
#else
# define CAN_CNTRL_BASIS_ADR(channel)    ((tCanCntrlRegBlock MEMORY_CAN *)(CanBasisAdr[channel]))
# define CAN_MAILBOX_BASIS_ADR(channel)  ((tCanMailBox MEMORY_CAN *)(CanMailboxBasisAdr[channel]))
#endif

/*lint -esym(750,CANTRR,CANAA,CANRFP,CANTEC,CANREC,CANLNT,CANTOC,CANTOS,CANGIF1)   */

#define CANME             (CAN_CNTRL_BASIS_ADR(channel) -> CanMe)
#define CANMD             (CAN_CNTRL_BASIS_ADR(channel) -> CanMd)
#define CANTRS            (CAN_CNTRL_BASIS_ADR(channel) -> CanTrs)
#define CANTRR            (CAN_CNTRL_BASIS_ADR(channel) -> CanTrr)
#define CANTA             (CAN_CNTRL_BASIS_ADR(channel) -> CanTa)
#define CANAA             (CAN_CNTRL_BASIS_ADR(channel) -> CanAa)
#define CANRMP            (CAN_CNTRL_BASIS_ADR(channel) -> CanRmp)
#define CANRML            (CAN_CNTRL_BASIS_ADR(channel) -> CanRml)
#define CANRFP            (CAN_CNTRL_BASIS_ADR(channel) -> CanRfp)
#define CANMC             (CAN_CNTRL_BASIS_ADR(channel) -> CanMc)
#define CANES             (CAN_CNTRL_BASIS_ADR(channel) -> CanEs)
#define CANGIM            (CAN_CNTRL_BASIS_ADR(channel) -> CanGim)
#define CANMIM            (CAN_CNTRL_BASIS_ADR(channel) -> CanMim)
#define CANOPC            (CAN_CNTRL_BASIS_ADR(channel) -> CanOpc)
#define CANGIF0           (CAN_CNTRL_BASIS_ADR(channel) -> CanGif0)

#if defined(C_COMP_TI_TMS470)
#  define CANGAM            (CAN_CNTRL_BASIS_ADR(channel) -> CanGam)
# define CANBTC            (CAN_CNTRL_BASIS_ADR(channel) -> CanBtc)
# define CANTEC            (CAN_CNTRL_BASIS_ADR(channel) -> CanTec)
# define CANREC            (CAN_CNTRL_BASIS_ADR(channel) -> CanRec)
# define CANGIF1           (CAN_CNTRL_BASIS_ADR(channel) -> CanGif1)
# define CANMIL            (CAN_CNTRL_BASIS_ADR(channel) -> CanMil)
# define CANTIOC           (CAN_CNTRL_BASIS_ADR(channel) -> CanTioc)
# define CANRIOC           (CAN_CNTRL_BASIS_ADR(channel) -> CanRioc)
# define CANLNT            (CAN_CNTRL_BASIS_ADR(channel) -> CanLnt)
# define CANTOC            (CAN_CNTRL_BASIS_ADR(channel) -> CanToc)
# define CANTOS            (CAN_CNTRL_BASIS_ADR(channel) -> CanTos)
# define CANLAM(i)         ((CAN_CNTRL_BASIS_ADR(channel) -> CanLam)[i])
#endif


#define CANMSGOBJ(i)      (CAN_MAILBOX_BASIS_ADR(channel)[i])


#if defined(C_SINGLE_RECEIVE_CHANNEL)
# define CAN_IS_RX_FULLCAN_PENDING()   ((CANRMP & (tCanRegType)kCanRxFullCANObjMask) != (tCanRegType)0x00000000)
#else
# define CAN_IS_RX_FULLCAN_PENDING()   ((CANRMP & (tCanRegType)(CanRxFullCANObjMask[channel])) != (tCanRegType)0x00000000)
#endif

#define CanLL_TxIsHWObjFree(channel, txObjHandle) \
        (((CANTRS & CanSlotMask[txObjHandle]) == (tCanRegType)0x00000000) ? kCanTrue : kCanFalse)

#if defined( C_ENABLE_SLEEP_WAKEUP )
# define CanLL_HwIsSleep(CAN_CHANNEL_CANPARA_ONLY)  ((CANES & kCanPda) == kCanPda)
#else
# define CanLL_HwIsSleep(CAN_CHANNEL_CANPARA_ONLY)  (kCanFalse)
#endif

# define CanLL_HwIsStop(CAN_CHANNEL_CANPARA_ONLY) ((CANES & kCanCce) == kCanCce)

#define CanLL_HwIsBusOff(CAN_CHANNEL_CANPARA_ONLY) ((CANES & kCanBo) == kCanBo)

#if defined( C_ENABLE_EXTENDED_STATUS )
# define CanLL_HwIsPassive(CAN_CHANNEL_CANPARA_ONLY) ((CANES & kCanEp) == kCanEp)

# define CanLL_HwIsWarning(CAN_CHANNEL_CANPARA_ONLY)  ((CANES & kCanEw) == kCanEw)
#endif
/* KB end CanLL_Macros */

/***************************************************************************/
/* Defines / data types / structs / unions                                 */
/***************************************************************************/


/* KB begin CanLL_DefinesDataTypesStructsUnions */

#define CAN_INLINE_FCT

/* KB end CanLL_DefinesDataTypesStructsUnions */

/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/* KB begin CanLL_Constants */
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCanMainVersion   = (vuint8)(( CAN_DRIVER_VERSION) >> 8);  /*lint !e572 !e778*/
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCanSubVersion    = (vuint8)( CAN_DRIVER_VERSION & 0x00FF );
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCanBugFixVersion = (vuint8)( CAN_BUGFIX_VERSION);

/* array of masks to access the slots of the different mailboxes */
V_MEMROM0 V_MEMROM1 static tCanRegType V_MEMROM2 CanSlotMask[kCanNumberOfMaxMailboxes] =
{
   (tCanRegType)0x00000001 ,(tCanRegType)0x00000002 ,(tCanRegType)0x00000004 ,(tCanRegType)0x00000008
  ,(tCanRegType)0x00000010 ,(tCanRegType)0x00000020 ,(tCanRegType)0x00000040 ,(tCanRegType)0x00000080
  ,(tCanRegType)0x00000100 ,(tCanRegType)0x00000200 ,(tCanRegType)0x00000400 ,(tCanRegType)0x00000800
  ,(tCanRegType)0x00001000 ,(tCanRegType)0x00002000 ,(tCanRegType)0x00004000 ,(tCanRegType)0x00008000

  ,(tCanRegType)0x00010000 ,(tCanRegType)0x00020000 ,(tCanRegType)0x00040000 ,(tCanRegType)0x00080000
  ,(tCanRegType)0x00100000 ,(tCanRegType)0x00200000 ,(tCanRegType)0x00400000 ,(tCanRegType)0x00800000
  ,(tCanRegType)0x01000000 ,(tCanRegType)0x02000000 ,(tCanRegType)0x04000000 ,(tCanRegType)0x08000000
  ,(tCanRegType)0x10000000 ,(tCanRegType)0x20000000 ,(tCanRegType)0x40000000 ,(tCanRegType)0x80000000
};

#if defined(C_COMP_TI_TMS470)
# if defined(C_ENABLE_CAN_RAM_CHECK)
V_MEMROM0 V_MEMROM1 static vuint32 V_MEMROM2 CanMemCheckValues[3][4] =
{
  0xAAAAAAAA, 0x00000008, 0xAAAAAAAA, 0xAAAAAAAA,
  0x55555555, 0x00000007, 0x55555555, 0x55555555,
  0x00000000, 0x00000000, 0x00000000, 0x00000000
};


V_MEMROM0 V_MEMROM1 static vuint32 V_MEMROM2 CanMemCheckMasks[4] =
{
  0xFFFFFFFF, 0x0000000F, 0xFFFFFFFF, 0xFFFFFFFF
};
# endif
#endif
/* KB end CanLL_Constants */

/***************************************************************************/
/* CAN-Hardware Data Definitions                                            */
/***************************************************************************/

/* KB begin CanLL_CanHardwareDataDefinitions */
/* KB end CanLL_CanHardwareDataDefinitions */

/***************************************************************************/
/* external declarations                                                    */
/***************************************************************************/

/* KB begin CanLL_ExternalDeclarations */
#if defined(C_COMP_TI_TMS470)
int CanGetCpsr(void); /* function from can_32.c */
#endif
/* KB end CanLL_ExternalDeclarations */

/***************************************************************************/
/* global data definitions                                                 */
/***************************************************************************/

tCanRxInfoStruct          canRxInfoStruct[kCanNumberOfChannels];

volatile CanReceiveHandle canRxHandle[kCanNumberOfChannels];

/* pCanChipMsgObject is a pointer to current receive object.
   This pointer is only valid during receive-interrupt routine,
   e.g. CanMsgReceived() or PreCopy(). The pointer is used to
   directly access to DLC, ID and DATA. The pointer has
   globally scope.  */
#if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )   && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
CanTransmitHandle          confirmHandle[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_CONFIRMATION_FLAG )
/* Msg(4:0750) A union type has been used. MISRA Rules 110 - no change */
volatile union CanConfirmationBits MEMORY_NEAR CanConfirmationFlags;
#endif

#if defined( C_ENABLE_INDICATION_FLAG )
/* Msg(4:0750) A union type has been used. MISRA Rules 110 - no change */
volatile union CanIndicationBits   MEMORY_NEAR CanIndicationFlags;
#endif

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/* ##RI1.4 - 3.31: Dynamic Receive DLC */
volatile canuint8 canVariableRxDataLen[kCanNumberOfRxObjects];
#endif

/* KB begin CanLL_GlobalDataDefinition */
#if defined(C_ENABLE_MULTIPLE_CONFIG)
volatile vuint8 canMcCurrentChannel;
#endif
/* KB end CanLL_GlobalDataDefinition */

/***************************************************************************/
/* local data definitions                                                  */
/***************************************************************************/

/* support for CAN driver features : */
static volatile CanTransmitHandle canHandleCurTxObj[kCanNumberOfUsedTxCANObjects];

#if defined( C_ENABLE_ECU_SWITCH_PASS )
static canuint8 canPassive[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_PART_OFFLINE )
static canuint8 canTxPartOffline[kCanNumberOfChannels];
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
static canuint8 canComStatus[kCanNumberOfChannels]; /* stores the decision of the App after the last CAN RAM check */
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )
static volatile canuint8 canTxDynObjReservedFlag[kCanNumberOfTxDynObjects];

 #if defined( C_ENABLE_DYN_TX_ID )
static tCanTxId0 canDynTxId0[kCanNumberOfTxDynObjects];
    #if (kCanNumberOfUsedCanTxIdTables > 1)
static tCanTxId1 canDynTxId1[kCanNumberOfTxDynObjects];
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 2)
static tCanTxId2 canDynTxId2[kCanNumberOfTxDynObjects];
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 3)
static tCanTxId3 canDynTxId3[kCanNumberOfTxDynObjects];
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 4)
static tCanTxId4 canDynTxId4[kCanNumberOfTxDynObjects];
    #endif
    #if defined( C_ENABLE_MIXED_ID )
     #if defined( C_HL_ENABLE_IDTYPE_IN_ID )
     #else
static tCanIdType                 canDynTxIdType[kCanNumberOfTxDynObjects];
     #endif
    #endif
 #endif

 #if defined( C_ENABLE_DYN_TX_DLC )
static canuint8                   canDynTxDLC[kCanNumberOfTxDynObjects];
 #endif
 #if defined( C_ENABLE_DYN_TX_DATAPTR )
static canuint8*                  canDynTxDataPtr[kCanNumberOfTxDynObjects];
 #endif
 #if defined( C_ENABLE_CONFIRMATION_FCT )
 #endif
#endif /* C_ENABLED_DYN_TX_OBJECTS */


#if defined(C_ENABLE_TX_MASK_EXT_ID)
static tCanTxId0 canTxMask0[kCanNumberOfChannels];
    #if (kCanNumberOfUsedCanTxIdTables > 1)
static tCanTxId1 canTxMask1[kCanNumberOfChannels];
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 2)
static tCanTxId2 canTxMask2[kCanNumberOfChannels];
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 3)
static tCanTxId3 canTxMask3[kCanNumberOfChannels];
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 4)
static tCanTxId4 canTxMask4[kCanNumberOfChannels];
    #endif
#endif

#if defined( C_ENABLE_VARIABLE_DLC )
static canuint8 canTxDLC_RAM[kCanNumberOfTxObjects];
#endif

#if defined( C_ENABLE_TRANSMIT_QUEUE )
static volatile canuint8 canTxQueueCnt[kCanNumberOfChannels];
/* tx queue flags          */
static canuint8 canTxQueueFlags[kCanNumberOfTxObjects];
# endif

static volatile canuint8 canStatus[kCanNumberOfChannels];

#if defined( C_ENABLE_INTCTRL_BY_APPL ) || \
    ( C_SECURITY_LEVEL == 0 )  || \
    ( defined ( C_ENABLE_OSEK_OS ) && defined ( C_ENABLE_OSEK_OS_INTCTRL ) )
#else
/* canInterruptOldStatus is necessary to save the status of IEN before
   disabling CAN interrupts and restore THIS(!) status when restoring
   CAN interrupts. */
/*disabled lint -esym(528,canInterruptOldStatus)*/
static tCanHLIntOld    canInterruptOldStatus;
#endif
static cansint8        canGlobalInterruptCounter;
static CANSINTX        canCanInterruptCounter[kCanNumberOfChannels];
#if defined(C_HL_ENABLE_CAN_IRQ_DISABLE)
static tCanLLCanIntOld canCanInterruptOldStatus[kCanNumberOfHwChannels];
#endif
#if defined(C_HL_ENABLE_LAST_INIT_OBJ)
static CanInitHandle lastInitObject[kCanNumberOfChannels];
#endif
#if defined( C_ENABLE_TX_POLLING )          || \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING )       || \
    defined( C_ENABLE_WAKEUP_POLLING )      || \
    defined( C_ENABLE_CANCEL_IN_HW )
# if defined(C_DISABLE_TASK_RECURSION_CHECK) && defined(C_DISABLE_USER_CHECK)
# else
static canuint8 canPollingTaskActive[kCanNumberOfChannels];
# endif
#endif

#if defined ( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
static tCanTxConfInfoStruct    txInfoStructConf[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
static volatile canuint8 canMsgCondRecState[kCanNumberOfChannels];
#endif


/* KB begin CanLL_LocalDataDefinition */


/* temporary messageobject for reception */
tCanMailBox canTempCanObj[kCanNumberOfChannels];

/* temporary messageobject for transmission */
tCanTempPreTransmitObj canTempPreTransmitObj[kCanNumberOfUsedTxCANObjects];

#if defined(C_COMP_TI_TMS470)
/* pointers to the structures containing Rx and Tx data */
tCanTempObj* canRDSRxPtr[kCanNumberOfChannels];
tCanTempObj* canRDSTxPtr[kCanNumberOfUsedTxCANObjects];
#endif

#if defined(C_ENABLE_HECC_WORKAROUND_1) || defined(C_ENABLE_TX_FULLCAN_DELAYED_START)
vuint32 gCANTRS[kCanNumberOfChannels];
#endif

/* KB end CanLL_LocalDataDefinition */

/***************************************************************************/
/*  local prototypes                                                       */
/***************************************************************************/
#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )  || \
    defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
static canuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_FIRST CanReceiveHandle rxHandle );
# if defined( C_ENABLE_INDICATION_FLAG ) || \
     defined( C_ENABLE_INDICATION_FCT )
static void CanHL_IndRxHandle( CanReceiveHandle rxHandle );
# endif
#endif
#if defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
static void CanBasicCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
#endif
#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
static void CanFullCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
#endif

static void CanHL_TxConfirmation(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);
static canuint8 CanCopyDataAndStartTransmission(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle, CanTransmitHandle txHandle)  C_API_3;   /*lint !e14 !e31*/

#if defined(C_ENABLE_CAN_RAM_CHECK)
static canuint8 CanCheckMemory(CAN_CHANNEL_CANTYPE_ONLY);
#endif

#if defined( C_ENABLE_TRANSMIT_QUEUE )
static void CanDelQueuedObj( CAN_CHANNEL_CANTYPE_ONLY );
#endif
#if defined ( C_HL_ENABLE_TX_MSG_DESTROYED )
static CANUINTX CanTxMsgDestroyed ( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);
#endif

static void CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY );

/* KB begin CanLL_LocalPrototypes */
void CanLL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY);

#if defined(C_COMP_TI_TMS470)
# if defined(C_ENABLE_PRIVILEDGED_DRIVER)
static void CanWritePriviledged(volatile vuint32* pAddr, vuint32 nVal);
# endif
#endif


#if !defined(C_ENABLE_TX_POLLING)          || \
    !defined(C_ENABLE_RX_FULLCAN_POLLING)  || \
    !defined(C_ENABLE_RX_BASICCAN_POLLING) || \
    !defined(C_ENABLE_ERROR_POLLING)       || \
    !defined(C_ENABLE_WAKEUP_POLLING)
void CanMessageIsr(CAN_CHANNEL_CANTYPE_ONLY);
void CanSystemIsr(CAN_CHANNEL_CANTYPE_ONLY);
#endif

/* KB end CanLL_LocalPrototypes */

/***************************************************************************/
/*  Error Check                                                            */
/***************************************************************************/
#if defined( C_CPUTYPE_8BIT )
# if (kCanNumberOfTxObjects > 250)
#  error "Too many transmit messages. This driver can only handle up to 250 transmit messages"
# endif
# if (kCanNumberOfRxObjects > 250)
#  error "Too many receive messages. This driver can only handle up to 250 receive messages"
# endif
#endif

#if defined ( C_SEARCH_HASH )
# if defined( C_ENABLE_EXTENDED_ID )
#  if (((kHashSearchListCountEx >  8192) && (kHashSearchMaxStepsEx == 1)) || \
     ((kHashSearchListCountEx > 16384) && (kHashSearchMaxStepsEx <  1)))
#   error "Hash table for extended ID is too large"
#  endif
# endif
# if defined(C_ENABLE_MIXED_ID) || !defined( C_ENABLE_EXTENDED_ID )
#  if (((kHashSearchListCount >  8192) && (kHashSearchMaxSteps == 1)) || \
     ((kHashSearchListCount > 16384) && (kHashSearchMaxSteps <  1)))
#   error "Hash table for standard ID is too large"
#  endif
# endif
#endif


/* KB begin CanLL_ErrorCheck */
#if defined(C_ENABLE_TX_FULLCAN_DELAYED_START) && defined(C_ENABLE_HECC_WORKAROUND_1)
# error "HECC 1 workaround cannot be combined with delayed Tx FullCAN start"
#endif
/* KB end CanLL_ErrorCheck */

/* KB begin canLL_MacrosAndFunctions */
#if defined(C_COMP_TI_TMS470)
# define ApplCanInterruptDisable() CanDisableIrq()
# define ApplCanInterruptRestore() CanEnableIrq()
#endif
/* KB end canLL_MacrosAndFunctions */

/***************************************************************************/
/*  Functions                                                              */
/***************************************************************************/

/* KB begin CanLL_Functions */
/***************************************************************************/
/* functions                                                               */
/***************************************************************************/

#if defined(C_COMP_TI_TMS470)
# if defined(C_ENABLE_CAN_RAM_CHECK)
vuint8 CanLL_IsMailboxCorrupt(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle)
{
  tCanRegType oldCanmeValue;
  tCanRegType tmpCanME;
  vuint8 retval;
  vuint8 i;

  oldCanmeValue = CANME;                           /* save CAN mailbox enable register */
  tmpCanME = CANME & (~CanSlotMask[hwObjHandle]);  /* disable mailbox to be checked */
  CANME = tmpCanME;

  retval = kCanFalse; /* we assume the mailbox is not corrupt */

  /* at this point the mailbox is not initialized therefore the contents are not saved! */
  for(i = 0; i < 3; i++)
  {
    CANMSGOBJ(hwObjHandle).CanMid               = CanMemCheckValues[i][0];
    CANMSGOBJ(hwObjHandle).CanMcf               = CanMemCheckValues[i][1];
    CANMSGOBJ(hwObjHandle).CanMsg.dwords.CanMdl = CanMemCheckValues[i][2];
    CANMSGOBJ(hwObjHandle).CanMsg.dwords.CanMdh = CanMemCheckValues[i][3];

    if( ((CANMSGOBJ(hwObjHandle).CanMid               & CanMemCheckMasks[0]) != CanMemCheckValues[i][0]) ||
        ((CANMSGOBJ(hwObjHandle).CanMcf               & CanMemCheckMasks[1]) != CanMemCheckValues[i][1]) ||
        ((CANMSGOBJ(hwObjHandle).CanMsg.dwords.CanMdl & CanMemCheckMasks[2]) != CanMemCheckValues[i][2]) ||
        ((CANMSGOBJ(hwObjHandle).CanMsg.dwords.CanMdh & CanMemCheckMasks[3]) != CanMemCheckValues[i][3]) )
    {
      retval = kCanTrue; /* the mailbox is corrupt */
      break;
    }
  }

  CANME = oldCanmeValue; /* restore old CANME value */

  return(retval);
}
# endif
#endif

#if defined (C_ENABLE_MEMCOPY_SUPPORT)
# if defined(C_COMP_TI_TMS470)

/****************************************************************************
| NAME:             CanCopyFromCan
| CALLED BY:        Application
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: void *             dst        | pointer to destionation buffer
|                   CanChipDataPtr     src        | pointer to CAN buffer
|                   vuint8           len        | number of bytes to copy
|
| RETURN VALUES:    -
|
| DESCRIPTION:      copy data from CAN receive buffer to RAM.
|
| ATTENTION:
****************************************************************************/
void CanCopyFromCan(void *dst, CanChipDataPtr src, vuint8 len)
{
  while(len > (vuint8)0)
  {
    len--;
    ((vuint8*)dst)[len] = ((vuint8*)src)[len];
  }
}

/****************************************************************************
| NAME:             CanCopyToCan
| CALLED BY:        Application
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: void *             src        | pointer to source buffer
|                   CanChipDataPtr     dst        | pointer to CAN buffer
|                   vuint8           len        | number of bytes to copy
|
| RETURN VALUES:    -
|
| DESCRIPTION:      copy data from CAN receive buffer to RAM.
|
| ATTENTION:
****************************************************************************/
void CanCopyToCan(CanChipDataPtr dst, void * src, vuint8 len)
{
  while(len > (vuint8)0)
  {
    len--;
    ((vuint8*)dst)[len] = ((vuint8*)src)[len];
  }
}

# endif
#endif

#if defined(C_ENABLE_SLEEP_WAKEUP)
/****************************************************************************
| NAME:             CanLL_WakeUpHandling
| CALLED BY:        CanMessageIsr(), CanWakeUpTask()
| PRECONDITIONS:
| INPUT PARAMETERS: none in case of single channel, otherwise the CAN channel
|                   on which the interrupt occured.
| RETURN VALUES:    none
| DESCRIPTION:      This function handles a receive or transmit interrupt
****************************************************************************/
void CanLL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY)
{
# if defined(C_ENABLE_APPLCANPREWAKEUP_FCT)
  ApplCanPreWakeUp(CAN_CHANNEL_CANPARA_ONLY);
# endif
  (void)CanWakeUp(CAN_CHANNEL_CANPARA_ONLY);
  APPL_CAN_WAKEUP(channel);
  CANGIF0 = kCanWuif0;  /* clear the interrupt flag */
}
# endif

#if !defined(C_ENABLE_TX_POLLING)          || \
    !defined(C_ENABLE_RX_FULLCAN_POLLING)  || \
    !defined(C_ENABLE_RX_BASICCAN_POLLING) || \
    !defined(C_ENABLE_ERROR_POLLING)       || \
    !defined(C_ENABLE_WAKEUP_POLLING)
/****************************************************************************
| NAME:             CanMessageIsr
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none in case of single channel, otherwise the CAN channel
|                   on which the interrupt occured.
| RETURN VALUES:    none
| DESCRIPTION:      This function handles a receive or transmit interrupt
****************************************************************************/
void CanMessageIsr(CAN_CHANNEL_CANTYPE_ONLY)
{
# if !defined(C_ENABLE_TX_POLLING)                                                    || \
      defined(C_ENABLE_RX_BASICCAN_OBJECTS) && !defined(C_ENABLE_RX_BASICCAN_POLLING) || \
      defined(C_ENABLE_RX_FULLCAN_OBJECTS)  && !defined(C_ENABLE_RX_FULLCAN_POLLING)
  cansint16 mb;    /* mailbox iterator */
# endif

# if defined(C_ENABLE_SLEEP_WAKEUP)
  if((CANES & kCanPda) == kCanPda) /* check if we are in power down mode */
  {
    CanLL_WakeUpHandling(CAN_CHANNEL_CANPARA_ONLY);             /* if yes -> wakeup the system and return */
    CanInit(CAN_CHANNEL_CANPARA_FIRST lastInitObject[channel]);
    return;
  }
# else
  assertInternal(((CANMIL == kCanAllLevel0) && (CANGIM & kCanI0en)), channel, kErrorWrongInterruptLine);
# endif

  CanSystemIsr(CAN_CHANNEL_CANPARA_ONLY);

# if !defined(C_ENABLE_TX_POLLING)
  mb = CAN_HL_HW_TX_STOPINDEX(channel) - 1;
#  if defined(C_ENABLE_CANCEL_IN_HW)
  while((mb >= CAN_HL_HW_TX_STARTINDEX(channel)) && ((CANTA | CANAA) != (tCanRegType)0x00000000))
  {
    if( (CANTA & CanSlotMask[mb]) || (CANAA & CanSlotMask[mb]) ) /* if successful transmitted or aborted */
#  else
  while((mb >= CAN_HL_HW_TX_STARTINDEX(channel)) && (CANTA != (tCanRegType)0x00000000))
  {
    if(CANTA & CanSlotMask[mb])
#  endif
    {
      CanHL_TxConfirmation(CAN_CHANNEL_CANPARA_FIRST (CanObjectHandle)mb);
    }
    mb--;
  }
# endif
# if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
#  if !defined(C_ENABLE_RX_FULLCAN_POLLING)
  mb = CAN_HL_HW_RX_FULL_STOPINDEX(channel) - 1;
  while((mb >= CAN_HL_HW_RX_FULL_STARTINDEX(channel)) && CAN_IS_RX_FULLCAN_PENDING())
  {
    if(CANRMP & CanSlotMask[mb])
    {
      CanFullCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST (CanObjectHandle)mb);
    }
    mb--;
  }
#  endif
# endif
# if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
#  if !defined(C_ENABLE_RX_BASICCAN_POLLING)
  mb = CAN_HL_HW_RX_BASIC_STOPINDEX(channel) - 1;
  while((mb >= CAN_HL_HW_RX_BASIC_STARTINDEX(channel)))
  {
    if(CANRMP & CanSlotMask[mb])
    {
      CanBasicCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST (CanObjectHandle)mb);
    }
    mb--;
  }
#  endif
# endif
}

/****************************************************************************
| NAME:             CanSystemIsr
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none in case of single channel, otherwise the CAN channel
|                   on which the interrupt occured.
| RETURN VALUES:    none
| DESCRIPTION:      This function handles a system interrupt
****************************************************************************/
void CanSystemIsr(CAN_CHANNEL_CANTYPE_ONLY)
{
#  if !defined(C_ENABLE_ERROR_POLLING)
  CanHL_ErrorHandling(CAN_CHANNEL_CANPARA_ONLY);
#  endif

#  if defined(C_ENABLE_SLEEP_WAKEUP)
#   if !defined(C_ENABLE_WAKEUP_POLLING)
  if(CANGIF0 & kCanWuif0)
  {
#    if defined(C_ENABLE_APPLCANPREWAKEUP_FCT)
    ApplCanPreWakeUp(CAN_CHANNEL_CANPARA_ONLY);
#    endif
    (void)CanWakeUp(CAN_CHANNEL_CANPARA_ONLY);
    APPL_CAN_WAKEUP( channel );
    CANGIF0 = kCanWuif0;  /* clear the interrupt flag */
  }
#   endif
#  endif
}


/****************************************************************************
| NAME:             Can0Interrupt
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Interrupt service functions according to the CAN controller
|                   interrupt stucture
|                   - check for the interrupt reason ( interrupt source )
|                   - work appropriate interrupt:
|                     + status interrupt (busoff, wakeup, error warning)
|                     + basic can receive
|                     + full can receive
|                     + full can transmit
****************************************************************************/
/*lint +rw(interrupt) +fem interrupt keyword */

#  if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_INT_OSCAT2)
ISR(Can0Interrupt)
#  else
CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Can0Interrupt(void)
#  endif /* C_ENABLE_OSEK_OS */
{
#  if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanMessageIsr();
#  else
#   if defined (C_ENABLE_MULTIPLE_CONFIG)
  CanMessageIsr(canMcCurrentChannel);
#   else
  CanMessageIsr(0);
#   endif
#  endif
}

#  if defined(C_SINGLE_RECEIVE_CHANNEL)
#  else
/****************************************************************************
| NAME:             Can1Interrupt
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Interrupt service functions according to the CAN controller
|                   interrupt stucture
|                   - check for the interrupt reason ( interrupt source )
|                   - work appropriate interrupt:
|                     + status interrupt (busoff, wakeup, error warning)
|                     + basic can receive
|                     + full can receive
|                     + full can transmit
****************************************************************************/
#    if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_INT_OSCAT2)
ISR(Can1Interrupt)
#    else
CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Can1Interrupt(void)
#    endif /* C_ENABLE_OSEK_OS */
{
  CanMessageIsr(1);
}
#  endif

#endif
/*lint -fem interrupt keyword*/


#if defined(C_COMP_TI_TMS470)
# if defined(C_ENABLE_PRIVILEDGED_DRIVER)
/****************************************************************************
| NAME:             CanWritePriviledged
| CALLED BY:        different functions accessing protected registers
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: pAddr: Address of the register to be written
|                   nVal: Value to be written
|
| RETURN VALUES:    none
|
| DESCRIPTION:      Writes data to a protected register
****************************************************************************/
static void CanWritePriviledged(volatile vuint32* pAddr, vuint32 nVal)
{
  CanDeclareGlobalInterruptOldStatus;
  CanSingleGlobalInterruptDisable();

  if(CAN_IS_USERMODE_ACTIVE())
  {
    CanSwitchToPrivilegedMode();   /* switch to systemmode */
    (*(vuint32*)pAddr) = nVal;
    CanSwitchToUserMode();         /* resstore usermode */
  }
  else
  {
    (*(vuint32*)pAddr) = nVal;
  }
  CanSingleGlobalInterruptRestore();
}
# endif
#endif

/* KB end CanLL_Functions */

/****************************************************************************
| NAME:             CanInit
| CALLED BY:        CanInitPowerOn(), Network management
| PRECONDITIONS:    none
| INPUT PARAMETERS: Handle to initstructure
| RETURN VALUES:    none
| DESCRIPTION:      initialization of chip-hardware
|                   initialization of receive and transmit message objects
****************************************************************************/
C_API_1 void C_API_2 CanInit( CAN_CHANNEL_CANTYPE_FIRST CanInitHandle initObject )
{
  CanObjectHandle        hwObjHandle;
#if defined( C_ENABLE_TX_FULLCAN_OBJECTS )      ||\
    defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  CanTransmitHandle      txHandle;
#endif
#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
  CanReceiveHandle       rxHandle;
#endif
  CanObjectHandle        logTxObjHandle;

  /* KB begin CanLL_InitLocalParameter */
  tCanRegType tmpCanMC;
  tCanRegType tmpCanMD;
  tCanRegType tmpCanME;
  tCanRegType tmpCanMIM;
  tCanRegType tmpCanGIM;
  /* KB end CanLL_InitLocalParameter */

#if defined(C_HL_ENABLE_LAST_INIT_OBJ)
  lastInitObject[channel] = initObject;
#endif

#if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
/* Msg(4:3759) Implicit conversion: int to unsigned short. MISRA Rule 43 - no change in RI 1.4 */
  initObject  += CAN_HL_INIT_OBJ_STARTINDEX(channel);
#endif


  assertUser(initObject < CAN_HL_INIT_OBJ_STOPINDEX(channel), channel, kErrorInitObjectHdlTooLarge);
  assertGen(initObject < kCanNumberOfInitObjects, channel, kErrorInitObjectHdlTooLarge);
#if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, channel, kErrorChannelHdlTooLarge);
#endif
#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif


  {
    /* KB begin CanLL_InitBegin */
    tmpCanGIM = 0;
    #if defined(C_ENABLE_HECC_WORKAROUND_1) || defined(C_ENABLE_TX_FULLCAN_DELAYED_START)
    gCANTRS[channel] = 0;
    #endif


    /* set CANTx and CANRx pins to CAN function (default after reset is GPIO) */
    CAN_WRITE_REGISTER(&CANTIOC, (tCanRegType)0xFFFFFFFF, (tCanRegType)kCanTxFunc);
    CAN_WRITE_REGISTER(&CANRIOC, (tCanRegType)0xFFFFFFFF, (tCanRegType)kCanRxFunc);

    tmpCanMC = kCanCcr;

    CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, tmpCanMC);   /* set change configuration request */

    APPLCANTIMERSTART(kCanLoopWaitConfigChangeEnable);
    while((CANES & kCanCce) != kCanCce)
    {
    #if defined(C_ENABLE_HW_LOOP_TIMER)
      if(0 == APPLCANTIMERLOOP(kCanLoopWaitConfigChangeEnable))
      {
        APPLCANTIMEREND(kCanLoopWaitConfigChangeEnable);

    # if defined(C_COMP_TI_TMS470)
        /* disconnect the CAN controller from the bus in case the initialization failed */
        CAN_WRITE_REGISTER(&CANTIOC, (tCanRegType)0xFFFFFFFF, (tCanRegType)0x00000000);
        CAN_WRITE_REGISTER(&CANRIOC, (tCanRegType)0xFFFFFFFF, (tCanRegType)0x00000000);
    # endif
        return;
      }
      else
    #endif
      {
        ; /* wait until CAN controller allows configuration access */
      }
    }
    APPLCANTIMEREND(kCanLoopWaitConfigChangeEnable);

    /* software reset to flush all CAN registers -> kCanSres bit is reset automatically by hardware */
    CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, (tCanRegType)(tmpCanMC | kCanSres));

    #if defined(C_COMP_TI_TMS470)
    /*
    If the SCC is used the lower three mailboxes are configured as BasicCANs, because
    the first acceptance mask (LAM0) is used for these three mailboxes.
    In HECC mode every mailbox has its own acceptance mask, therefore in HECC mode
    there are only two BasicCAN mailboxes.
    */
    # if defined(C_SINGLE_RECEIVE_CHANNEL)
    #  if(kCanControllerMode == kCanSCCMode)
    tmpCanMC &= ~kCanScm;   /* activate SCC mode */
    #  else
    tmpCanMC |= kCanScm;   /* activate HECC mode */
    #  endif
    # else
    if(CanControllerMode[channel] == kCanSCCMode)
    {
      tmpCanMC &= ~kCanScm;   /* activate SCC mode */
    }
    else
    {
      tmpCanMC |= kCanScm;   /* activate HECC mode */
    }
    # endif
    #endif



    /* initialize the temporary variables with the register values */
    tmpCanMD = CANMD;
    tmpCanME = CANME;

    /* lock the first BasicCan mailslot */
    # if defined(C_SINGLE_RECEIVE_CHANNEL)
    #  if(kCanControllerMode == kCanSCCMode)
    CANOPC = (tCanRegType)0x00000006;
    #  else
    CANOPC = (tCanRegType)0x00000002;
    #  endif
    # else
    if(CanControllerMode[channel] == kCanSCCMode)
    {
      CANOPC = (tCanRegType)0x00000006;
    }
    else
    {
      CANOPC = (tCanRegType)0x00000002;
    }
    #  endif

    /* if TSeg1 or TSeg2 are equal to 0 the CAN controller will not leave config mode! */
    assertGen(((CanInitBTC[initObject] & kCanTSegMask) > 0), channel, kErrorWrongTSegValue);

    /* set bus timing register */
    CAN_WRITE_REGISTER(&CANBTC, (tCanRegType)0xFFFFFFFF, (tCanRegType)CanInitBTC[initObject]);
    CANMIL = (vuint32)kCanAllLevel0;    /* all mailbox IRQs on level 0 */
    CAN_WRITE_REGISTER(&CANMIM, (tCanRegType)0xFFFFFFFF, (tCanRegType)0x00000000);    /* disable IRQs for all mailboxes */

    tmpCanMIM = (tCanRegType)0x00000000;
    CANME     = kCanDisableAllObj;                                     /* disable all mailboxes to have write access */
    tmpCanME  = kCanDisableAllObj;
    /* KB end CanLL_InitBegin */

    /* Init Tx-Objects -------------------------------------------------------- */
    /* init saved Tx handles: */
    /* in case of CommonCAN, transmission is always on the frist HW channel of a CommonCAN channel */

    for (hwObjHandle=CAN_HL_HW_TX_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_TX_STOPINDEX(canHwChannel); hwObjHandle++ )
    {
      logTxObjHandle = (CanObjectHandle)((vsintx)hwObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) || \
    defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
      if((canStatus[channel] & kCanHwIsInit) != 0)                    /*lint !e661*/
      {
        /* inform application, if a pending transmission is canceled */
        txHandle = canHandleCurTxObj[logTxObjHandle];

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
        if( txHandle < kCanNumberOfTxObjects )
        {
          APPLCANCANCELNOTIFICATION(channel, txHandle);
        }
# endif
# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
        if( txHandle == kCanBufferMsgTransmit)
        {
          APPLCANMSGCANCELNOTIFICATION(channel);
        }
# endif
      }
#endif

      canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                   /* MsgObj is free */

      /* KB begin CanLL_InitTxObj */
      tmpCanMD &= ~CanSlotMask[hwObjHandle]; /* set to Tx */
      tmpCanME |= CanSlotMask[hwObjHandle];  /* enable mailbox */
      #if defined(C_ENABLE_TX_POLLING)
      tmpCanMIM &= ~CanSlotMask[hwObjHandle];
      #else
      tmpCanMIM |= CanSlotMask[hwObjHandle];
      #endif
      /* KB end CanLL_InitTxObj */
    }

#if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
    {
      for (txHandle=CAN_HL_TX_STAT_STARTINDEX(channel); txHandle<CAN_HL_TX_STAT_STOPINDEX(channel); txHandle++ )    /*lint !e661*/
      {
        hwObjHandle = CanGetTxHwObj(txHandle);
        if ( hwObjHandle  != CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) )    /* is message <txHandle> a direct message? */
        {
          /* KB begin CanLL_InitTxSetId */
          tmpCanMD &= ~CanSlotMask[hwObjHandle];  /* set to Tx */
          #if defined(C_ENABLE_HECC_WORKAROUND_2)
          CanLockGlobalInterrupt();
          CANMSGOBJ(hwObjHandle).CanMid = CanGetTxId0(txHandle);
          CANMSGOBJ(hwObjHandle).CanMid = CanGetTxId0(txHandle);
          CanUnlockGlobalInterrupt();
          #else
          CANMSGOBJ(hwObjHandle).CanMid = CanGetTxId0(txHandle);
          #endif
          /* KB end CanLL_InitTxSetId */
          /* KB begin CanLL_InitTxSetDlc */
          # if defined(C_ENABLE_HECC_WORKAROUND_2)
          CanLockGlobalInterrupt();
          CANMSGOBJ(hwObjHandle).CanMcf = CanGetTxDlc(txHandle);
          CANMSGOBJ(hwObjHandle).CanMcf = CanGetTxDlc(txHandle);
          CanUnlockGlobalInterrupt();
          # else
          CANMSGOBJ(hwObjHandle).CanMcf = CanGetTxDlc(txHandle);
          # endif
          #if defined(C_ENABLE_TX_POLLING)
          tmpCanMIM &= ~CanSlotMask[hwObjHandle];
          #else
          tmpCanMIM |= CanSlotMask[hwObjHandle];
          #endif
          tmpCanME  |= CanSlotMask[hwObjHandle]; /* enable mailbox */
          /* KB end CanLL_InitTxSetDlc */

# if defined( C_HL_ENABLE_IDTYPE_OWN_REG )
          /* KB begin CanLL_InitTxSetIdType */
          # error "CanLL_InitTxSetIdType not used"
          /* KB end CanLL_InitTxSetIdType */
# endif
        }
      }
    }
#endif

    /* init unused msg objects ------------------------------------------------ */
    for (hwObjHandle=CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel); hwObjHandle++ )  /*lint !e681*/
    {
      /* KB begin CanLL_InitUnusedObj */
      tmpCanME &= ~CanSlotMask[hwObjHandle];
      /* KB end CanLL_InitUnusedObj */
    }


    #if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
    /* init full can receive msg objects: ------------------------------------- */
    for (hwObjHandle=CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel); hwObjHandle++ )
    {
      /* brackets to avoid lint info 834 */
      rxHandle = (hwObjHandle-CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel))+CAN_HL_RX_FULL_STARTINDEX(canHwChannel);

      /* KB begin CanLL_InitFullCANObj */
      #if defined(C_COMP_TI_TMS470)
      /*
         The acceptance mask for the FullCAN is set to must match. If it is a SCC
         controller LAM3 and GAM are set only otherwise every mailbox specific LAM register
      */
      # if defined(C_SINGLE_RECEIVE_CHANNEL)
      #  if(kCanControllerMode == kCanSCCMode)
      #   if defined( C_ENABLE_RX_MASK_EXT_ID )
      CANLAM(3) = ((vuint32)MK_EXTID0(~C_MASK_EXT_ID)) & ((vuint32)0x1FFFFFFF);
      CANGAM    = ((vuint32)MK_EXTID0(~C_MASK_EXT_ID)) & ((vuint32)0x1FFFFFFF);
      #   else
      CANLAM(3) = (vuint32)0x00000000;
      CANGAM    = (vuint32)0x00000000;
      #   endif
      #  else
      #   if defined(C_ENABLE_RX_MASK_EXT_ID)
      if((((vuint32)((CanGetRxId0(rxHandle)))) & C_EXT_ID) == kCanIdTypeExt)
      {
        CANLAM(hwObjHandle) = ((vuint32)MK_EXTID0(~C_MASK_EXT_ID)) & ((vuint32)0x1FFFFFFF);
      }
      else
      #   endif
      {
        CANLAM(hwObjHandle) = (vuint32)0x00000000;
      }
      #  endif
      # else
      if(CanControllerMode[channel] == kCanSCCMode)
      {
      #  if defined(C_ENABLE_RX_MASK_EXT_ID)
        CANLAM(3) = ((vuint32)MK_EXTID0(~C_MASK_EXT_ID)) & ((vuint32)0x1FFFFFFF);
        CANGAM    = ((vuint32)MK_EXTID0(~C_MASK_EXT_ID)) & ((vuint32)0x1FFFFFFF);
      #  else
        CANLAM(3) = (vuint32)0x00000000;
        CANGAM    = (vuint32)0x00000000;
      #  endif
      }
      else
      {
      #  if defined(C_ENABLE_RX_MASK_EXT_ID)
        if((((vuint32)((CanGetRxId0(rxHandle)))) & C_EXT_ID) == kCanIdTypeExt)
        {
          CANLAM(hwObjHandle) = ((vuint32)MK_EXTID0(~C_MASK_EXT_ID)) & ((vuint32)0x1FFFFFFF);
        }
        else
      #  endif
        {
          CANLAM(hwObjHandle) = (vuint32)0x00000000;
        }
      }
      # endif
      #endif



      tmpCanMD |= CanSlotMask[hwObjHandle];        /* set to Rx */
      # if !defined(C_ENABLE_RX_FULLCAN_POLLING)
      tmpCanMIM |= CanSlotMask[hwObjHandle];       /* mailbox generates interrupts on events */
      # endif

      #if defined(C_COMP_TI_TMS470)
        /*
        If the SCC is used the lower three mailboxes share one acceptance mask. The
        first two mailboxes are used as BasicCAN. The third mailbox is a FullCAN which does
        not use the acceptance mask.
        */
      #  if defined(C_SINGLE_RECEIVE_CHANNEL)
      #   if(kCanControllerMode == kCanSCCMode)
      #    if defined( C_ENABLE_RX_MASK_EXT_ID )
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)((CanGetRxId0(rxHandle)) | kCanAme); /* SCC uses acceptance mask for Rx masking */
      #    else
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)((CanGetRxId0(rxHandle))); /* Acceptance mask is not used! */
      #    endif
      #   else
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)((CanGetRxId0(rxHandle)) | kCanAme); /* HECC uses acceptance mask */
      #   endif
      #  else
      if(CanControllerMode[channel] == kCanSCCMode)
      {
      #   if defined( C_ENABLE_RX_MASK_EXT_ID )
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)((CanGetRxId0(rxHandle)) | kCanAme); /* SCC uses acceptance mask for Rx masking */
      #   else
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)((CanGetRxId0(rxHandle))); /* Acceptance mask is not used! */
      #   endif
      }
      else
      {
        CANMSGOBJ(hwObjHandle).CanMid = (vuint32)((CanGetRxId0(rxHandle)) | kCanAme);
      }
      #  endif
      # endif



      tmpCanME |= CanSlotMask[hwObjHandle];        /* enable mailbox */
      /* KB end CanLL_InitFullCANObj */
    }
    #endif

    #if defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
    /* init basic can receive msg object: ------------------------------------- */
    for (hwObjHandle=CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); hwObjHandle++ )
    {
      /* the LL driver has to know which ID tpyes have to be received by which object */
      /* KB begin CanLL_InitBasicCanObj */
      tmpCanMD |= CanSlotMask[hwObjHandle];                                         /* set to Rx */
      #if defined(C_COMP_TI_TMS470)
      # if defined(C_SINGLE_RECEIVE_CHANNEL)
      #  if(kCanControllerMode == kCanSCCMode)
      CANLAM(0) = (vuint32)CanInitLAM0[initObject];             /* set acceptance mask */
      #  else
      CANLAM(hwObjHandle) = (vuint32)CanInitLAM0[initObject];   /* set acceptance mask */
      #  endif
      # else
      if(CanControllerMode[channel] == kCanSCCMode)
      {
        CANLAM(0) = (vuint32)CanInitLAM0[initObject];           /* set acceptance mask */
      }
      else
      {
        CANLAM(hwObjHandle) = (vuint32)CanInitLAM0[initObject]; /* set acceptance mask */
      }
      # endif
      #endif


      #if defined(C_ENABLE_HECC_WORKAROUND_2)
      CanLockGlobalInterrupt();
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)CanInitMID1[initObject] | kCanAme; /* set code register   */
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)CanInitMID1[initObject] | kCanAme;
      CanUnlockGlobalInterrupt();
      #else
      CANMSGOBJ(hwObjHandle).CanMid = (vuint32)CanInitMID1[initObject] | kCanAme; /* set code register   */
      #endif

      # if !defined(C_ENABLE_RX_BASICCAN_POLLING)
      tmpCanMIM |= CanSlotMask[hwObjHandle];   /* mailbox generates interrupts on events */
      # endif
      tmpCanME |= CanSlotMask[hwObjHandle];    /* enable mailbox */
      /* KB end CanLL_InitBasicCanObj */
    }
    #endif

    /* KB begin CanLL_InitEnd */
    tmpCanGIM = 0;

    #if defined(C_ENABLE_SLEEP_WAKEUP)
    # if !defined(C_ENABLE_WAKEUP_POLLING)
    tmpCanGIM |= kCanWuim;
    # endif
    #endif

    #if !defined(C_ENABLE_ERROR_POLLING)
    /* overrun is polled within the receive function */
    tmpCanGIM |= kCanBoim; /* enable bus off interrupt */
    # if defined(C_COMP_TI_TMS470)
    #  if defined(C_ENABLE_CANCEL_IN_HW)
    tmpCanGIM |= kCanAaim; /* enable abort acknowledge and bus off interrupts */
    #  endif
    # endif
    #endif

    #if !defined(C_ENABLE_TX_POLLING)          || \
        !defined(C_ENABLE_RX_FULLCAN_POLLING)  || \
        !defined(C_ENABLE_RX_BASICCAN_POLLING) || \
        !defined(C_ENABLE_ERROR_POLLING)       || \
        !defined(C_ENABLE_WAKEUP_POLLING)
    tmpCanGIM |= kCanI0en; /* enable all CAN interrupts */
    #endif
    /* now set the register values using 32 bit accesses */
    CANMD  = tmpCanMD;
    CANME  = tmpCanME;
    CAN_WRITE_REGISTER(&CANMIM, (vuint32)0xFFFFFFFF, tmpCanMIM);
    CAN_WRITE_REGISTER(&CANGIM, (vuint32)0xFFFFFFFF, tmpCanGIM);

    # if defined(C_COMP_TI_TMS470)
    #  if !defined(C_ENABLE_TX_POLLING)          || \
          !defined(C_ENABLE_RX_FULLCAN_POLLING)  || \
          !defined(C_ENABLE_RX_BASICCAN_POLLING) || \
          !defined(C_ENABLE_ERROR_POLLING)       || \
          !defined(C_ENABLE_WAKEUP_POLLING)
    assertInternal(((CANMIL == kCanAllLevel0) && (CANGIM & kCanI0en)) , channel, kErrorWrongInterruptLine);
    # endif
    #endif

    tmpCanMC &= ~kCanCcr;  /* clear configuration change request bit */
    CAN_WRITE_REGISTER(&CANMC, 0xFFFFFFFF, tmpCanMC);

    APPLCANTIMERSTART(kCanLoopWaitConfigChangeEnable);
    while((CANES & kCanCce) == kCanCce)
    {
    #if defined(C_ENABLE_HW_LOOP_TIMER)
      if(0 == APPLCANTIMERLOOP(kCanLoopWaitConfigChangeEnable))
      {
        break; /* exit loop */
      }
    #else
      ; /* wait until CAN controller leaves configuration mode */
    #endif
    }
    APPLCANTIMEREND(kCanLoopWaitConfigChangeEnable);

    /* KB end CanLL_InitEnd */
  } /* end of loop over all hw channels */

  #if defined ( C_ENABLE_TX_OBSERVE )
  ApplCanInit( CAN_CHANNEL_CANPARA_FIRST CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel), CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel) );
  #endif
  #ifdef C_ENABLE_MSG_TRANSMIT_CONF_FCT
  ApplCanMsgTransmitInit( CAN_CHANNEL_CANPARA_ONLY );
  #endif


} /* END OF CanInit */



/****************************************************************************
| NAME:             CanInitPowerOn
| CALLED BY:        Application
| PRECONDITIONS:    This function must be called by the application before
|                   any other CAN driver function
|                   Interrupts must be disabled
| INPUT PARAMETERS: Handle to initstructure
| RETURN VALUES:    none
| DESCRIPTION:      Initialization of the CAN chip
****************************************************************************/
C_API_1 void C_API_2 CanInitPowerOn( void )
{

  #if defined( C_ENABLE_VARIABLE_DLC )     || \
      defined( C_ENABLE_DYN_TX_OBJECTS )   || \
      defined( C_ENABLE_INDICATION_FLAG )  || \
      defined( C_ENABLE_CONFIRMATION_FLAG )
  CanSignedTxHandle i;
  #endif
  CAN_CHANNEL_CANTYPE_LOCAL

#if defined ( MISRA_CHECK )
  canCanInterruptOldStatus[0] = canCanInterruptOldStatus[0] ;
# if defined(C_HL_ENABLE_LAST_INIT_OBJ)
  lastInitObject[0]= lastInitObject[0];
# endif
#endif

  canGlobalInterruptCounter = 0;

  #if defined( C_ENABLE_VARIABLE_DLC )
  for (i= kCanNumberOfTxObjects-1; i >= 0; i--)
  {
    assertGen(XT_TX_DLC(CanGetTxDlc(i))<9, kCanAllChannels, kErrorTxROMDLCTooLarge);
    canTxDLC_RAM[i] = CanGetTxDlc(i);
  }
  #endif

  #if defined(  C_ENABLE_DYN_TX_OBJECTS )
  /*  Reset dynamic transmission object management -------------------------- */
  for (i= kCanNumberOfTxDynObjects-1; i >= 0; i--)
  {
    /*  Reset management information  */
    canTxDynObjReservedFlag[i] = 0;
  }
  #endif /* C_ENABLE_DYN_TX_OBJECTS */

  #if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  /* ##RI1.4 - 3.31: Dynamic Receive DLC */
  /*  Initialize the dynamic dlc for reception -------------------------- */
  for (i= kCanNumberOfRxObjects-1; i >= 0; i--)
  {
    canVariableRxDataLen[i] = CanGetRxDataLen(i);
  }
  #endif

  #if defined( C_ENABLE_INDICATION_FLAG )
  for (i = 0; i < kCanNumberOfIndBytes; i++) {
    CanIndicationFlags._c[i] = 0;
  }
  #endif

  #if defined( C_ENABLE_CONFIRMATION_FLAG )
  for (i = 0; i < kCanNumberOfConfBytes; i++) {
    CanConfirmationFlags._c[i] = 0;
  }
  #endif


  /* KB begin CanLL_InitPowerOn */
  #if defined(C_COMP_TI_TMS470)
  {
    vuint16 j;

  # if defined (C_ENABLE_MULTIPLE_CONFIG)
    j = (vuint16)canMcCurrentChannel;
  # else
    for(j = 0; j < kCanNumberOfChannels; j++)
  #endif
    {
      canRDSRxPtr[j] = (tCanTempObj*)&(canTempCanObj[j].CanMsg.bytes); /*lint !e740 cast okay! */
    }

    for(j = 0; j < kCanNumberOfUsedTxCANObjects; j++)
    {
      canRDSTxPtr[j] = (tCanTempObj*)&(canTempPreTransmitObj[j].a[4]); /*lint !e740 cast okay! */
    }
  }
  #endif

  /* KB end CanLL_InitPowerOn */

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  for (channel=0; channel<kCanNumberOfChannels; channel++)
  #endif
  {

    canStatus[channel]              = kCanTxOff;

#if defined ( C_ENABLE_TRANSMIT_QUEUE )
  /* clear all Tx queue flags */
    CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
#endif

#if defined ( C_ENABLE_CAN_TX_CONF_FCT )
    txInfoStructConf[channel].Channel = channel;
#endif
    canRxInfoStruct[channel].Channel  = channel;
    canCanInterruptCounter[channel] = 0;

  #if defined( C_ENABLE_TX_POLLING )          || \
      defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
      defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
      defined( C_ENABLE_ERROR_POLLING )       || \
      defined( C_ENABLE_WAKEUP_POLLING )      || \
      defined( C_ENABLE_CANCEL_IN_HW )
# if defined(C_DISABLE_TASK_RECURSION_CHECK) &&\
     defined(C_DISABLE_USER_CHECK)
# else
    canPollingTaskActive[channel] = 0;
# endif
  #endif

#if defined(  C_ENABLE_DYN_TX_OBJECTS )
  /*  Reset dynamic transmission object management -------------------------- */
    #if defined( C_ENABLE_CONFIRMATION_FCT ) && \
        defined( C_ENABLE_TRANSMIT_QUEUE )
    confirmHandle[channel] = kCanBufferFree;
    #endif
#endif

#if defined(C_ENABLE_TX_MASK_EXT_ID)
    canTxMask0[channel] = 0;
    #if (kCanNumberOfUsedCanTxIdTables > 1)
    canTxMask1[channel] = 0;
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 2)
    canTxMask2[channel] = 0;
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 3)
    canTxMask3[channel] = 0;
    #endif
    #if (kCanNumberOfUsedCanTxIdTables > 4)
    canTxMask4[channel] = 0;
    #endif
#endif

#ifdef C_ENABLE_ECU_SWITCH_PASS
    canPassive[channel]             = 0;
#endif

#if defined(C_ENABLE_PART_OFFLINE)
    canTxPartOffline[channel]       = kCanTxPartInit;
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
    canComStatus[channel] = kCanEnableCommunication;
    if(kCanRamCheckFailed == CanCheckMemory(CAN_CHANNEL_CANPARA_ONLY))
    {
      canComStatus[channel] = ApplCanMemCheckFailed(CAN_CHANNEL_CANPARA_ONLY); /* let the application decide if communication is disabled */
    }
#endif

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
    if ( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 )
#endif
    {
      CanInit( CAN_CHANNEL_CANPARA_FIRST 0 );

      /* canStatus is only set to init and online, if CanInit() is called for this channel. */
      canStatus[channel]              |= (kCanHwIsInit | kCanTxOn);
    }

#if defined( C_ENABLE_COND_RECEIVE_FCT )
    canMsgCondRecState[channel]     = kCanTrue;
#endif
  }

} /* END OF CanInitPowerOn */


#if defined( C_ENABLE_TRANSMIT_QUEUE )
/************************************************************************
* NAME:               CanDelQueuedObj
* CALLED BY:
* PRECONDITIONS:
* PARAMETER:          notify: if set to 1 for every deleted obj the appl is notified
* RETURN VALUE:       -
* DESCRIPTION:        Resets the bits with are set to 0 in mask
*                     Clearing the Transmit-queue
*************************************************************************/
static void CanDelQueuedObj( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanTransmitHandle i;

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  /* Attention: QueueCounter must be cleard befor clearing the flags,
                to supress an assertion in CanFullcanTxInterrupt()  */
   canTxQueueCnt[channel] = 0;

   for (i = CAN_HL_TX_STARTINDEX(channel); i < CAN_HL_TX_STOPINDEX(channel); i++)
   {
# if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
     if((canStatus[channel] & kCanHwIsInit) != 0)
     {
       if(canTxQueueFlags[i] != 0)
       {
         APPLCANCANCELNOTIFICATION(channel, i);
       }
     }
# endif
     canTxQueueFlags[i] = 0;
   }
}
#endif

#if defined( C_ENABLE_CAN_TRANSMIT )
/****************************************************************************
| NAME:             CanCancelTransmit
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: Tx-Msg-Handle
| RETURN VALUES:    none
| DESCRIPTION:      delete on Msg-Object
****************************************************************************/
C_API_1 void C_API_2 CanCancelTransmit( CanTransmitHandle txHandle )
{
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL
  CanObjectHandle        logTxObjHandle;
  /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
#if defined(C_ENABLE_CANCEL_IN_HW)
  CanObjectHandle        txObjHandle;
#endif

  if (txHandle < kCanNumberOfTxObjects)         /* legal txHandle ? */
  {
  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
    channel = CanGetChannelOfTxObj(txHandle);
  #endif

#if defined ( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel , kErrorDisabledTxMessage);
#endif

    CanNestedGlobalInterruptDisable();
#ifdef C_ENABLE_TRANSMIT_QUEUE
    if ( canTxQueueFlags[txHandle] != 0 )        /*lint !e661*/
    {
      canTxQueueCnt[channel]--;
      canTxQueueFlags[txHandle] = 0;             /*lint !e661*/
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }
#endif

#if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
    logTxObjHandle = (CanObjectHandle)((vsintx)CanGetTxHwObj(txHandle) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
#else
    logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
#endif/* C_ENABLE_TX_FULLCAN_OBJECTS */
    if (canHandleCurTxObj[logTxObjHandle] == txHandle)
    {
      canHandleCurTxObj[logTxObjHandle] = kCanBufferCancel;

      /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
#if defined(C_ENABLE_CANCEL_IN_HW)
# if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
      txObjHandle = CanGetTxHwObj(txHandle);
# else
      txObjHandle = CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel);
# endif /* C_ENABLE_TX_FULLCAN_OBJECTS */
      /* KB begin CanLL_CancelInHw */
      CANTRR = CanSlotMask[txObjHandle]; /* set transmission reset request flag */
      /* KB end CanLL_CancelInHw */
#endif /* C_ENABLE_CANCEL_IN_HW */
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }

    CanNestedGlobalInterruptRestore();
  } /* if (txHandle < kCanNumberOfTxObjects) */
}

# if defined ( C_HL_ENABLE_TX_MSG_DESTROYED )
/****************************************************************************
| NAME:             CanTxMsgDestroyed
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: Hardware Object number and channel
| RETURN VALUES:    kCanOk, if the status has been changed
|                   kCanFailed, if the status was already free.
| DESCRIPTION:      set status of HW Msg-Object to destroyed, notify application
|                   and supress confirmation for this object
****************************************************************************/
#  if defined ( MISRA_CHECK )
   /* suppress misra message about multiple return */
#   pragma PRQA_MESSAGES_OFF 2006
#  endif

static CANUINTX CanTxMsgDestroyed ( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle )
{
#  if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
  CanTransmitHandle txHandle;
#  endif


  if ((canHandleCurTxObj[txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)]) != kCanBufferFree)
  {
#  if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
    txHandle = canHandleCurTxObj[txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)];
#  endif
    canHandleCurTxObj[txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] = kCanBufferMsgDestroyed;

#  if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)                || \
      defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#   if defined( C_ENABLE_MSG_TRANSMIT )
    if (txObjHandle == CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel))
    {
      APPLCANMSGCANCELNOTIFICATION(channel);
    }
    else
#   endif
    {
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }
#  endif

    return  kCanOk;
  }
  return kCanFailed;
}

#  if defined ( MISRA_CHECK )
#   pragma PRQA_MESSAGES_ON 2006
#  endif
# endif
#endif /* if defined( C_ENABLE_CAN_TRANSMIT ) */


#ifdef C_ENABLE_MSG_TRANSMIT_CONF_FCT
/****************************************************************************
| NAME:             CanCancelMsgTransmit
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    none
| DESCRIPTION:      delete on Msg-Object
****************************************************************************/
C_API_1 void C_API_2 CanCancelMsgTransmit( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
  CanObjectHandle  logTxObjHandle;
#if defined(C_ENABLE_CANCEL_IN_HW)
  CanObjectHandle  txObjHandle;    /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
#endif

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  CanNestedGlobalInterruptDisable();
  if (canHandleCurTxObj[logTxObjHandle] == kCanBufferMsgTransmit)
  {
    canHandleCurTxObj[logTxObjHandle] = kCanBufferCancel;

    /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
#if defined(C_ENABLE_CANCEL_IN_HW)
    txObjHandle = CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel);
    /* KB begin CanLL_CancelInHw */
    CANTRR = CanSlotMask[txObjHandle]; /* set transmission reset request flag */
    /* KB end CanLL_CancelInHw */
#endif
    APPLCANMSGCANCELNOTIFICATION(channel);
  }
  CanNestedGlobalInterruptRestore();
}
#endif


#if defined( C_ENABLE_CAN_TRANSMIT )
# if defined( C_ENABLE_VARIABLE_DLC )
/****************************************************************************
| NAME:             CanTransmitVarDLC
| CALLED BY:        Netmanagement, application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle to Tx message, DLC of Tx message
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
C_API_1 canuint8 C_API_2 CanTransmitVarDLC(CanTransmitHandle txHandle, canuint8 dlc)
{
  assertUser(dlc<9, kCanAllChannels, kErrorTxVarDLCTooLarge);

  canTxDLC_RAM[ txHandle ] = (canTxDLC_RAM[ txHandle ] & CanLL_DlcMask) | MK_TX_DLC(dlc);

  return CanTransmit( txHandle );
} /* END OF CanTransmitVarDLC */
# endif

/****************************************************************************
| NAME:             CanTransmit
| CALLED BY:        application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle of the transmit object to be send
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return */
#  pragma PRQA_MESSAGES_OFF 2006
# endif

C_API_1 canuint8 C_API_2 CanTransmit(CanTransmitHandle txHandle)  C_API_3   /*lint !e14 !e31*/
{
  CanDeclareGlobalInterruptOldStatus

#  if defined(C_ENABLE_TX_POLLING)        && \
      defined(C_ENABLE_TRANSMIT_QUEUE)    && \
     !defined(C_ENABLE_TX_FULLCAN_OBJECTS)
#  else
    CanObjectHandle      txObjHandle;
#  endif
#  if !defined( C_ENABLE_TX_POLLING )        || \
      !defined( C_ENABLE_TRANSMIT_QUEUE )    || \
       defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  CanObjectHandle      logTxObjHandle;
#  endif
# if !defined( C_ENABLE_TX_POLLING )        || \
     !defined( C_ENABLE_TRANSMIT_QUEUE )    || \
      defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  canuint8             rc;
# endif
  CAN_CHANNEL_CANTYPE_LOCAL

  /* KB begin CanLL_TransmitLocalParameter */
  /* KB end CanLL_TransmitLocalParameter */



  assertUser(txHandle<kCanNumberOfTxObjects, kCanAllChannels , kErrorTxHdlTooLarge);

# if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);          /*lint !e661*/
# endif

#if defined ( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel , kErrorDisabledTxMessage);
#endif

  /* test offline ---------------------------------------------------------- */
  if ( (canStatus[channel] & kCanTxOn) == kCanTxOff )           /* transmit path switched off */
  {
    return kCanTxFailed;
  }

#ifdef C_ENABLE_PART_OFFLINE
  if ( (canTxPartOffline[channel] & CanTxSendMask[txHandle]) != 0)   /*lint !e661*/ /* CAN off ? */
  {
    return (kCanTxPartOffline);
  }
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    return(kCanCommunicationDisabled);
  }
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )
  assertInternal(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
#endif
  assertInternal((canStatus[channel] & kCanHwIsStop) != kCanHwIsStop, channel, kErrorCanStop);

  /* passive mode ---------------------------------------------------------- */
  #if defined( C_ENABLE_ECU_SWITCH_PASS )
  if ( canPassive[channel] != 0)                             /*  set passive ? */
  {
#if defined ( C_ENABLE_CAN_TX_CONF_FCT ) || \
    defined( C_ENABLE_CONFIRMATION_FCT )
    CanCanInterruptDisable(CAN_CHANNEL_CANPARA_ONLY);      /* avoid CAN Rx interruption */
#endif

#if defined ( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
    txInfoStructConf[channel].Channel = channel;
    txInfoStructConf[channel].Handle  = txHandle;
    /* KB begin CanLL_AddTxConfStructMember */
    /* CanLL_AddTxConfStructMember not used */
    /* KB end CanLL_AddTxConfStructMember */
    ApplCanTxConfirmation(&txInfoStructConf[channel]);
#endif

    #if defined ( C_ENABLE_CONFIRMATION_FLAG )       /* set transmit ready flag  */
      #if C_SECURITY_LEVEL > 20
    CanSingleGlobalInterruptDisable();
      #endif
    CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] |= CanGetConfirmationMask(txHandle);      /*lint !e661*/
      #if C_SECURITY_LEVEL > 20
    CanSingleGlobalInterruptRestore();
      #endif
    #endif

    #if defined( C_ENABLE_CONFIRMATION_FCT )
    {
      #if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
      #else
      if ( CanGetApplConfirmationPtr(txHandle) != NULL )
      #endif
      {
         (CanGetApplConfirmationPtr(txHandle))(txHandle);   /* call completion routine  */
      }
    }
    #endif /* C_ENABLE_CONFIRMATION_FCT */

#if defined ( C_ENABLE_CAN_TX_CONF_FCT ) || \
    defined( C_ENABLE_CONFIRMATION_FCT )
    CanCanInterruptRestore(CAN_CHANNEL_CANPARA_ONLY);
#endif

    return kCanTxOk;
  }
  #endif


   /* can transmit enabled ================================================== */

   /* ----------------------------------------------------------------------- */
   /* ---  transmit queue with one objects ---------------------------------- */
   /* ---  transmit using fullcan objects ----------------------------------- */
   /* ----------------------------------------------------------------------- */

# if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  txObjHandle = CanGetTxHwObj(txHandle);    /*lint !e661*/
# else
#  if !defined(C_ENABLE_TX_POLLING)        ||\
      !defined(C_ENABLE_TRANSMIT_QUEUE)
  txObjHandle = CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel);                                          /* msg in object 0 */
#  endif
# endif

# if !defined( C_ENABLE_TX_POLLING )        || \
     !defined( C_ENABLE_TRANSMIT_QUEUE )    || \
     defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
# endif

#  if defined(C_ENABLE_TX_DIRECT_OBJECTS)
  assertUser (CanTxDirectTransmitSupport[logTxObjHandle] != kCanTxDirectObj, channel, kErrorIllNonDirectTxHandle);
#  endif

  CanNestedGlobalInterruptDisable();

  /* test offline after interrupt disable ---------------------------------- */
  if ( (canStatus[channel] & kCanTxOn) == kCanTxOff )                /* transmit path switched off */
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

  #if defined( C_ENABLE_TRANSMIT_QUEUE )
    #if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  if ( txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) )                                  /*disabled - lint !e774 */
    #endif
  {
    #if defined( C_ENABLE_TX_POLLING )
     #if defined( C_ENABLE_INDIVIDUAL_POLLING )
    if ( (
           /* KB begin CanLL_CurrentObjIsHandledInPolling */
           /* CanLL_CurrentObjIsHandledInPolling not used */
           /* KB end CanLL_CurrentObjIsHandledInPolling */
         ) || (canHandleCurTxObj[logTxObjHandle] != kCanBufferFree) )    /* MsgObj used?  */
     #else
     #endif
    #else
    if ( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree)    /* MsgObj used?  */
    #endif
    {
      /* tx object 0 used -> set msg in queue: -----------------------------*/

      if (canTxQueueFlags[txHandle] == 0 ) /*lint !e661*/ /* msg already in queue? */
      {
        canTxQueueFlags[txHandle] = 1;     /*lint !e661*/ /* set message in queue */
        canTxQueueCnt[channel]++;
      }
      CanNestedGlobalInterruptRestore();
      return kCanTxOk;
    }
  }
  #endif

  #if !defined( C_ENABLE_TX_POLLING )        || \
      !defined( C_ENABLE_TRANSMIT_QUEUE )    || \
       defined( C_ENABLE_TX_FULLCAN_OBJECTS )

    #if defined( C_ENABLE_TRANSMIT_QUEUE ) &&  \
        defined( C_ENABLE_TX_FULLCAN_OBJECTS )
  else
    #endif
  {
  /* check for transmit message object free ---------------------------------*/
    if (( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree)    /* MsgObj used?  */
       || ( !CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) )

      /* hareware-txObject is not free --------------------------------------*/
       )  /* end of if question */

    {  /* object used */
      /* tx object n used, quit with error */
      CanNestedGlobalInterruptRestore();
      return kCanTxFailed;
    }
  }

  /* Obj, pMsgObject points to is free, transmit msg object: ----------------*/
  canHandleCurTxObj[logTxObjHandle] = txHandle;/* Save hdl of msgObj to be transmitted*/
  CanNestedGlobalInterruptRestore();

  rc = CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle, txHandle);

#  if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
  if ( rc == kCanTxNotify)
  {
    rc = kCanTxFailed;      /* ignore notification if calls of CanCopy.. is performed within CanTransmit */
  }
#  endif


  return(rc);

  #else
  #endif /* !C_ENABLE_TX_POLLING || !C_ENABLE_TRANSMIT_QUEUE || ...  */

} /* END OF CanTransmit */
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif



/****************************************************************************
| NAME:             CanCopyDataAndStartTransmission
| CALLED BY:        CanTransmit and CanTransmitQueuedObj
| PRECONDITIONS:    - Can driver must be initialized
|                   - canTxCurHandle[logTxObjHandle] must be set
|                   - the hardwareObject (txObjHandle) must be free
| INPUT PARAMETERS: txHandle: Handle of the transmit object to be send
|                   txObjHandle:  Nr of the HardwareObjects to use
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about goto */
#  pragma PRQA_MESSAGES_OFF 2001
# endif
static canuint8 CanCopyDataAndStartTransmission( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle, CanTransmitHandle txHandle) C_API_3   /*lint !e14 !e31*/
{
   CanDeclareGlobalInterruptOldStatus
   canuint8             rc;
   CanObjectHandle      logTxObjHandle;
    #if defined( C_ENABLE_COPY_TX_DATA )
   TxDataPtr   CanMemCopySrcPtr;
    #endif
   #if defined(  C_ENABLE_DYN_TX_OBJECTS )
   CanTransmitHandle    dynTxObj;
   #endif /* C_ENABLE_DYN_TX_OBJECTS */
   #if defined( C_ENABLE_PRETRANSMIT_FCT )
   CanTxInfoStruct      txStruct;
   #endif

   /* KB begin CanLL_TxTransmissionLocalParameter */
   tCanRegType   tmpCanME;
   #if defined(C_ENABLE_PRETRANSMIT_FCT) || defined(C_ENABLE_COPY_TX_DATA)
   vuint32   tmpTxBuf[2];
   #endif
   #if defined(C_CLIENT_VECTOR)
   # if defined(C_COMP_TI_TMS470)
   #  if(kCanControllerMode == kCanSCCMode)
   while(CANES & kCanRm)
   {
     ; /* while CAN cell is receiving a message */
   }
   #  endif
   # endif
   #endif
   /* KB end CanLL_TxTransmissionLocalParameter */

   #if defined(  C_ENABLE_DYN_TX_OBJECTS )
   if (txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
   {
     dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);
   }
   else
   {
     dynTxObj = 0;
   }
   #endif /* C_ENABLE_DYN_TX_OBJECTS */

   logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

   assertHardware( CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ), channel, kErrorTxBufferBusy);

   /* KB begin CanLL_TxBegin */
   tmpCanME = CANME & (tCanRegType)~CanSlotMask[txObjHandle];  /* disable mailbox to get write access */
   CANME    = tmpCanME;
   /* KB end CanLL_TxBegin */

   /* set id and dlc  -------------------------------------------------------- */
#if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
   if ( txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) )
#endif
   {
 #if defined(  C_ENABLE_DYN_TX_DLC ) || \
      defined(  C_ENABLE_DYN_TX_ID )
     if (txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
     {           /* set dynamic part of dynamic objects ----------------------*/
  #if defined( C_ENABLE_DYN_TX_ID )
        /* KB begin CanLL_TxSetId */
        #if defined(C_ENABLE_HECC_WORKAROUND_2)
        CanLockGlobalInterrupt();
        CANMSGOBJ(txObjHandle).CanMid = canDynTxId0[dynTxObj];
        CANMSGOBJ(txObjHandle).CanMid = canDynTxId0[dynTxObj];
        CanUnlockGlobalInterrupt();
        #else
        CANMSGOBJ(txObjHandle).CanMid = canDynTxId0[dynTxObj];
        #endif
        /* KB end CanLL_TxSetId */
  #endif

  #if defined( C_ENABLE_DYN_TX_DLC )
        /* KB begin CanLL_TxSetDlc */
        #if defined(C_ENABLE_HECC_WORKAROUND_2)
        CanLockGlobalInterrupt();
        CANMSGOBJ(txObjHandle).CanMcf = (canDynTxDLC[dynTxObj]);
        CANMSGOBJ(txObjHandle).CanMcf = (canDynTxDLC[dynTxObj]);
        CanUnlockGlobalInterrupt();
        #else
        CANMSGOBJ(txObjHandle).CanMcf = (canDynTxDLC[dynTxObj]);
        #endif
        /* KB end CanLL_TxSetDlc */
  #endif
     }
     else
     {          /* set part of static objects assocciated the dynamic --------*/
  #if defined( C_ENABLE_DYN_TX_ID )
    #if defined( C_ENABLE_TX_MASK_EXT_ID )
     #if defined( C_ENABLE_MIXED_ID )
        if (CanGetTxIdType(txHandle)==kCanIdTypeStd)
        {
          /* KB begin CanLL_TxSetId */
          #if defined(C_ENABLE_HECC_WORKAROUND_2)
          CanLockGlobalInterrupt();
          CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
          CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
          CanUnlockGlobalInterrupt();
          #else
          CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
          #endif
          /* KB end CanLL_TxSetId */
        }
        else
     #endif
        {
          /* mask extened ID */
          /* KB begin CanLL_TxSetId */
          #if defined(C_ENABLE_HECC_WORKAROUND_2)
          CanLockGlobalInterrupt();
          CANMSGOBJ(txObjHandle).CanMid = (CanGetTxId0(txHandle)|canTxMask0[channel]);
          CANMSGOBJ(txObjHandle).CanMid = (CanGetTxId0(txHandle)|canTxMask0[channel]);
          CanUnlockGlobalInterrupt();
          #else
          CANMSGOBJ(txObjHandle).CanMid = (CanGetTxId0(txHandle)|canTxMask0[channel]);
          #endif
          /* KB end CanLL_TxSetId */
        }
    #else
        /* KB begin CanLL_TxSetId */
        #if defined(C_ENABLE_HECC_WORKAROUND_2)
        CanLockGlobalInterrupt();
        CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
        CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
        CanUnlockGlobalInterrupt();
        #else
        CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
        #endif
        /* KB end CanLL_TxSetId */
    #endif
  #endif

  #if defined( C_ENABLE_DYN_TX_DLC )
    #if defined( C_ENABLE_VARIABLE_DLC )
        /* init DLC, RAM */
        /* KB begin CanLL_TxSetDlc */
        #if defined(C_ENABLE_HECC_WORKAROUND_2)
        CanLockGlobalInterrupt();
        CANMSGOBJ(txObjHandle).CanMcf = (canTxDLC_RAM[txHandle]);
        CANMSGOBJ(txObjHandle).CanMcf = (canTxDLC_RAM[txHandle]);
        CanUnlockGlobalInterrupt();
        #else
        CANMSGOBJ(txObjHandle).CanMcf = (canTxDLC_RAM[txHandle]);
        #endif
        /* KB end CanLL_TxSetDlc */
    #else
        /* init DLC, ROM */
        /* KB begin CanLL_TxSetDlc */
        #if defined(C_ENABLE_HECC_WORKAROUND_2)
        CanLockGlobalInterrupt();
        CANMSGOBJ(txObjHandle).CanMcf = CanGetTxDlc(txHandle);
        CANMSGOBJ(txObjHandle).CanMcf = CanGetTxDlc(txHandle);
        CanUnlockGlobalInterrupt();
        #else
        CANMSGOBJ(txObjHandle).CanMcf = CanGetTxDlc(txHandle);
        #endif
        /* KB end CanLL_TxSetDlc */
    #endif
  #endif
     }
 #endif
     /* set static part commen for static and dynamic objects ----------------*/
 #if defined( C_ENABLE_DYN_TX_ID )
 #else
  #if defined( C_ENABLE_TX_MASK_EXT_ID )
   #if defined( C_ENABLE_MIXED_ID )
     if (CanGetTxIdType(txHandle)==kCanIdTypeStd)
     {
       /* KB begin CanLL_TxSetId */
       #if defined(C_ENABLE_HECC_WORKAROUND_2)
       CanLockGlobalInterrupt();
       CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
       CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
       CanUnlockGlobalInterrupt();
       #else
       CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
       #endif
       /* KB end CanLL_TxSetId */
     }
     else
   #endif
     {
       /* mask extened ID */
       /* KB begin CanLL_TxSetId */
       #if defined(C_ENABLE_HECC_WORKAROUND_2)
       CanLockGlobalInterrupt();
       CANMSGOBJ(txObjHandle).CanMid = (CanGetTxId0(txHandle)|canTxMask0[channel]);
       CANMSGOBJ(txObjHandle).CanMid = (CanGetTxId0(txHandle)|canTxMask0[channel]);
       CanUnlockGlobalInterrupt();
       #else
       CANMSGOBJ(txObjHandle).CanMid = (CanGetTxId0(txHandle)|canTxMask0[channel]);
       #endif
       /* KB end CanLL_TxSetId */
     }
  #else
     /* KB begin CanLL_TxSetId */
     #if defined(C_ENABLE_HECC_WORKAROUND_2)
     CanLockGlobalInterrupt();
     CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
     CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
     CanUnlockGlobalInterrupt();
     #else
     CANMSGOBJ(txObjHandle).CanMid = CanGetTxId0(txHandle);
     #endif
     /* KB end CanLL_TxSetId */
  #endif
 #endif
 #if defined( C_ENABLE_DYN_TX_DLC )
 #else
  #if defined( C_ENABLE_VARIABLE_DLC )
     /* init DLC, RAM */
     /* KB begin CanLL_TxSetDlc */
     #if defined(C_ENABLE_HECC_WORKAROUND_2)
     CanLockGlobalInterrupt();
     CANMSGOBJ(txObjHandle).CanMcf = (canTxDLC_RAM[txHandle]);
     CANMSGOBJ(txObjHandle).CanMcf = (canTxDLC_RAM[txHandle]);
     CanUnlockGlobalInterrupt();
     #else
     CANMSGOBJ(txObjHandle).CanMcf = (canTxDLC_RAM[txHandle]);
     #endif
     /* KB end CanLL_TxSetDlc */
  #else
     /* init DLC, ROM */
     /* KB begin CanLL_TxSetDlc */
     #if defined(C_ENABLE_HECC_WORKAROUND_2)
     CanLockGlobalInterrupt();
     CANMSGOBJ(txObjHandle).CanMcf = CanGetTxDlc(txHandle);
     CANMSGOBJ(txObjHandle).CanMcf = CanGetTxDlc(txHandle);
     CanUnlockGlobalInterrupt();
     #else
     CANMSGOBJ(txObjHandle).CanMcf = CanGetTxDlc(txHandle);
     #endif
     /* KB end CanLL_TxSetDlc */
  #endif
 #endif

#if defined( C_ENABLE_MIXED_ID )
 #if defined( C_HL_ENABLE_IDTYPE_IN_ID )
 #else
  #if defined(  C_ENABLE_DYN_TX_DLC ) || \
       defined(  C_ENABLE_DYN_TX_ID )
     if (txHandle >=  CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
     {                      /* set dynamic part of dynamic objects */
   #if defined( C_ENABLE_DYN_TX_ID )
       /* KB begin CanLL_TxSetIdType */
       #if defined(C_ENABLE_HECC_WORKAROUND_2)
       if(canDynTxIdType[dynTxObj] == C_EXT_ID)
       {
         CanLockGlobalInterrupt();
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
         CanUnlockGlobalInterrupt();
       }
       else
       {
         CanLockGlobalInterrupt();
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
         CanUnlockGlobalInterrupt();
       }
       #else
       if(canDynTxIdType[dynTxObj] == C_EXT_ID)
       {
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
       }
       else
       {
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
       }
       #endif
       /* KB end CanLL_TxSetIdType */
   #else
       /* KB begin CanLL_TxSetIdType */
       #if defined(C_ENABLE_HECC_WORKAROUND_2)
       if(CanGetTxIdType(txHandle) == C_EXT_ID)
       {
         CanLockGlobalInterrupt();
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
         CanUnlockGlobalInterrupt();
       }
       else
       {
         CanLockGlobalInterrupt();
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
         CanUnlockGlobalInterrupt();
       }
       #else
       if(CanGetTxIdType(txHandle) == C_EXT_ID)
       {
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
       }
       else
       {
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
       }
       #endif
       /* KB end CanLL_TxSetIdType */
   #endif
     }
   #if defined( C_HL_ENABLE_IDTYPE_OWN_REG )
     else
     {
       /* KB begin CanLL_TxSetIdType */
       #if defined(C_ENABLE_HECC_WORKAROUND_2)
       if(CanGetTxIdType(txHandle) == C_EXT_ID)
       {
         CanLockGlobalInterrupt();
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
         CanUnlockGlobalInterrupt();
       }
       else
       {
         CanLockGlobalInterrupt();
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
         CanUnlockGlobalInterrupt();
       }
       #else
       if(CanGetTxIdType(txHandle) == C_EXT_ID)
       {
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
       }
       else
       {
         CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
       }
       #endif
       /* KB end CanLL_TxSetIdType */
     }
   #endif
  #else
   #if defined( C_HL_ENABLE_IDTYPE_OWN_REG )
     /* KB begin CanLL_TxSetIdType */
     #if defined(C_ENABLE_HECC_WORKAROUND_2)
     if(CanGetTxIdType(txHandle) == C_EXT_ID)
     {
       CanLockGlobalInterrupt();
       CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
       CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
       CanUnlockGlobalInterrupt();
     }
     else
     {
       CanLockGlobalInterrupt();
       CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
       CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
       CanUnlockGlobalInterrupt();
     }
     #else
     if(CanGetTxIdType(txHandle) == C_EXT_ID)
     {
       CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid | kCanIde);
     }
     else
     {
       CANMSGOBJ(txObjHandle).CanMid = (CANMSGOBJ(txObjHandle).CanMid & kCanIdeNeg);
     }
     #endif
     /* KB end CanLL_TxSetIdType */
   #endif
  #endif
 #endif
#endif

   }


 /* call pretransmit function ----------------------------------------------- */
 #if defined( C_ENABLE_PRETRANSMIT_FCT )

   /* pointer needed for other modules */
   /* KB begin CanLL_TxGetCANDataPtr */
   #if defined(C_ENABLE_HECC_WORKAROUND_2)
   CanLockGlobalInterrupt();
   canTempPreTransmitObj[logTxObjHandle].s.CanMid = CANMSGOBJ(txObjHandle).CanMid;
   canTempPreTransmitObj[logTxObjHandle].s.CanMid = CANMSGOBJ(txObjHandle).CanMid;
   canTempPreTransmitObj[logTxObjHandle].s.CanMcf = CANMSGOBJ(txObjHandle).CanMcf;
   canTempPreTransmitObj[logTxObjHandle].s.CanMcf = CANMSGOBJ(txObjHandle).CanMcf;
   CanUnlockGlobalInterrupt();
   #else
   canTempPreTransmitObj[logTxObjHandle].s.CanMid = CANMSGOBJ(txObjHandle).CanMid;
   canTempPreTransmitObj[logTxObjHandle].s.CanMcf = CANMSGOBJ(txObjHandle).CanMcf;
   #endif
   (txStruct.pChipData) = (CanChipMsgPtr)&(canTempPreTransmitObj[logTxObjHandle].a[4]);
   /* KB end CanLL_TxGetCANDataPtr */
   txStruct.Handle      = txHandle;

   {
    #if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
    #else
    /* Is there a PreTransmit function ? ------------------------------------- */
    if ( CanGetApplPreTransmitPtr(txHandle) != NULL )    /* if PreTransmit exists */
    #endif
    {
      if ( (CanGetApplPreTransmitPtr(txHandle)) (txStruct) == kCanNoCopyData)
      {

        /* KB begin CanLL_TxPretransmitCopyToCan */
        /*lint -e740 -e826 cast okay */
        #if defined(C_COMP_TI_TMS470)
        tmpTxBuf[0] = *(vuint32*)&(canTempPreTransmitObj[logTxObjHandle].a[4]);
        tmpTxBuf[1] = *(vuint32*)&(canTempPreTransmitObj[logTxObjHandle].a[6]);
        #endif
        #if defined(C_ENABLE_HECC_WORKAROUND_2)
        CanLockGlobalInterrupt();
        CANMSGOBJ(txObjHandle).CanMid = canTempPreTransmitObj[logTxObjHandle].s.CanMid;
        CANMSGOBJ(txObjHandle).CanMid = canTempPreTransmitObj[logTxObjHandle].s.CanMid;
        CANMSGOBJ(txObjHandle).CanMcf = canTempPreTransmitObj[logTxObjHandle].s.CanMcf;
        CANMSGOBJ(txObjHandle).CanMcf = canTempPreTransmitObj[logTxObjHandle].s.CanMcf;
        CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = tmpTxBuf[0];
        CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = tmpTxBuf[0];
        CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = tmpTxBuf[1];
        CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = tmpTxBuf[1];
        CanUnlockGlobalInterrupt();
        #else
        CANMSGOBJ(txObjHandle).CanMid = canTempPreTransmitObj[logTxObjHandle].s.CanMid;
        CANMSGOBJ(txObjHandle).CanMcf = canTempPreTransmitObj[logTxObjHandle].s.CanMcf;
        CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = tmpTxBuf[0];
        CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = tmpTxBuf[1];
        #endif
        /*lint +e740 +e826 cast okay */
        /* KB end CanLL_TxPretransmitCopyToCan */

        /* Do not copy the data - already done by the PreTransmit-function */
        /* --- start transmission --- */
        goto startTransmission;
      }
    }
   }
 #endif /* C_ENABLE_PRETRANSMIT_FCT */

 /* copy data --------------------------------------------------------------- */
 #if defined( C_ENABLE_COPY_TX_DATA )
   #if defined(  C_ENABLE_DYN_TX_DATAPTR )
   if (txHandle >=  CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
   {
      CanMemCopySrcPtr = canDynTxDataPtr[dynTxObj];
   }
   else
   #endif
   {
     CanMemCopySrcPtr = CanGetTxDataPtr(txHandle);
   }
 /* copy via index in MsgObj data field, copy always 8 bytes -----------*/
   if ( CanMemCopySrcPtr != NULL )   /* copy if buffer exists */
   {
     #if C_SECURITY_LEVEL > 10
     CanSingleGlobalInterruptDisable();
     #endif

     /* KB begin CanLL_TxCopyToCan */
     /*lint -e740 -e826 cast okay */
     #if defined(C_COMP_TI_TMS470)
     ((vuint8*)tmpTxBuf)[0] = ((vuint8*)CanMemCopySrcPtr)[0];
     ((vuint8*)tmpTxBuf)[1] = ((vuint8*)CanMemCopySrcPtr)[1];
     ((vuint8*)tmpTxBuf)[2] = ((vuint8*)CanMemCopySrcPtr)[2];
     ((vuint8*)tmpTxBuf)[3] = ((vuint8*)CanMemCopySrcPtr)[3];
     ((vuint8*)tmpTxBuf)[4] = ((vuint8*)CanMemCopySrcPtr)[4];
     ((vuint8*)tmpTxBuf)[5] = ((vuint8*)CanMemCopySrcPtr)[5];
     ((vuint8*)tmpTxBuf)[6] = ((vuint8*)CanMemCopySrcPtr)[6];
     ((vuint8*)tmpTxBuf)[7] = ((vuint8*)CanMemCopySrcPtr)[7];
     #endif
     #if defined(C_ENABLE_HECC_WORKAROUND_2)
     CanLockGlobalInterrupt();
     # if defined( C_ENABLE_PRETRANSMIT_FCT )
     CANMSGOBJ(txObjHandle).CanMid = canTempPreTransmitObj[logTxObjHandle].s.CanMid;
     CANMSGOBJ(txObjHandle).CanMid = canTempPreTransmitObj[logTxObjHandle].s.CanMid;
     CANMSGOBJ(txObjHandle).CanMcf = canTempPreTransmitObj[logTxObjHandle].s.CanMcf;
     CANMSGOBJ(txObjHandle).CanMcf = canTempPreTransmitObj[logTxObjHandle].s.CanMcf;
     # endif
     CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = tmpTxBuf[0];
     CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = tmpTxBuf[0];
     CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = tmpTxBuf[1];
     CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = tmpTxBuf[1];
     CanUnlockGlobalInterrupt();
     #else
     # if defined( C_ENABLE_PRETRANSMIT_FCT )
     CANMSGOBJ(txObjHandle).CanMid = canTempPreTransmitObj[logTxObjHandle].s.CanMid;
     CANMSGOBJ(txObjHandle).CanMcf = canTempPreTransmitObj[logTxObjHandle].s.CanMcf;
     # endif
     CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = tmpTxBuf[0];
     CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = tmpTxBuf[1];
     #endif
     /*lint +e740 +e826 cast okay */
     /* KB end CanLL_TxCopyToCan */

     #if C_SECURITY_LEVEL > 10
     CanSingleGlobalInterruptRestore();
     #endif
   }
 #endif /* ( C_ENABLE_COPY_TX_DATA ) */

#if defined( C_ENABLE_PRETRANSMIT_FCT )
/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
startTransmission:
#endif

   /* test offline and handle and start transmission ------------------------ */
   CanNestedGlobalInterruptDisable();
   /* If CanTransmit was interrupted by a re-initialization or CanOffline */
   /* no transmitrequest of this action should be started      */
   if ((canHandleCurTxObj[logTxObjHandle] == txHandle) && ((canStatus[channel] & kCanTxOn) != kCanTxOff))
   {
     /* KB begin CanLL_TxStart */
     tmpCanME  = CANME  | CanSlotMask[txObjHandle];  /* enable mailbox again */
     CANME  = tmpCanME;
     #if defined(C_ENABLE_HECC_WORKAROUND_1)
     if(((CANES & kCanTm) != (tCanRegType)0) && (CANTRS != (tCanRegType)0)) /* check if hardware is not busy with Tx */
     {
       gCANTRS[channel] |= CanSlotMask[txObjHandle];  /* set Tx request flag */
     }
     else
     #endif
     {
       CANTRS = CanSlotMask[txObjHandle];  /* set Tx request flag */
     }
     /* KB end CanLL_TxStart */

     #if defined( C_ENABLE_TX_OBSERVE )
     ApplCanTxObjStart( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
     #endif
     rc = kCanTxOk;
   }
   else
   {
# if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
     if (canHandleCurTxObj[logTxObjHandle] == txHandle)
     {
       /* only CanOffline was called on higher level */
       rc = kCanTxNotify;
     }
     else
#endif
     {
       rc = kCanTxFailed;
     }
     canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* release TxHandle (CanOffline) */
   }

   CanNestedGlobalInterruptRestore();

   /* KB begin CanLL_TxEnd */
   tmpCanME  = CANME | CanSlotMask[txObjHandle];  /* enable mailbox again */
   CANME     = tmpCanME;
   /* KB end CanLL_TxEnd */

   return (rc);

} /* END OF CanCopyDataAndStartTransmission */
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2001
# endif
#endif /* if defined( C_ENABLE_CAN_TRANSMIT ) */


#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING ) || \
    defined( C_ENABLE_WAKEUP_POLLING ) || \
    defined( C_ENABLE_CANCEL_IN_HW )
/****************************************************************************
| NAME:             CanTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task,
|                   - polling error bus off
|                   - polling Tx objects
|                   - polling Rx objects
****************************************************************************/
C_API_1 void C_API_2 CanTask(void)
{
  CAN_CHANNEL_CANTYPE_LOCAL

#if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  for (channel = 0; channel < kCanNumberOfChannels; channel++)
#endif
  {
#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
    if ( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 )
#endif
    {
#if defined( C_ENABLE_ERROR_POLLING )
      CanErrorTask(CAN_CHANNEL_CANPARA_ONLY);
#endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
#  if defined( C_ENABLE_WAKEUP_POLLING )
      CanWakeUpTask(CAN_CHANNEL_CANPARA_ONLY);
#  endif
# endif

#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_CANCEL_IN_HW )
      CanTxTask(CAN_CHANNEL_CANPARA_ONLY);
#endif

#if defined( C_ENABLE_RX_FULLCAN_POLLING ) && \
    defined( C_ENABLE_RX_FULLCAN_OBJECTS )
      CanRxFullCANTask(CAN_CHANNEL_CANPARA_ONLY);
#endif

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_BASICCAN_POLLING )
      CanRxBasicCANTask(CAN_CHANNEL_CANPARA_ONLY);
#endif
    }
  }
}
#endif

#if defined( C_ENABLE_ERROR_POLLING )
/****************************************************************************
| NAME:             CanErrorTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task,
|                   - polling error status
****************************************************************************/
C_API_1 void C_API_2 CanErrorTask( CAN_CHANNEL_CANTYPE_ONLY )
{

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
  canPollingTaskActive[channel]++;
  assertUser((canPollingTaskActive[channel] == 1), channel, kErrorPollingTaskRecursion);
#  endif
# else
  if (canPollingTaskActive[channel] == 0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;
# endif

    {
      CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
      CanHL_ErrorHandling(CAN_HW_CHANNEL_CANPARA_ONLY);
      CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
    }

# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
    canPollingTaskActive[channel]--;
#  endif
# else
    canPollingTaskActive[channel] = 0;
  }
# endif
}
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )
# if defined( C_ENABLE_WAKEUP_POLLING )
/****************************************************************************
| NAME:             CanWakeUpTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task,
|                   - polling CAN wakeup event
****************************************************************************/
C_API_1 void C_API_2 CanWakeUpTask(CAN_CHANNEL_CANTYPE_ONLY)
{
  /* KB begin CanLL_WakeUpTaskLocalParameter */
  /* CanLL_WakeUpTaskLocalParameter not used */
  /* KB end CanLL_WakeUpTaskLocalParameter */

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

#  if defined(C_DISABLE_TASK_RECURSION_CHECK)
#   if defined(C_ENABLE_USER_CHECK)
  canPollingTaskActive[channel]++;
  assertUser((canPollingTaskActive[channel] == 1), channel, kErrorPollingTaskRecursion);
#   endif
#  else
  if (canPollingTaskActive[channel] == 0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;
#  endif

    /* KB begin CanLL_if_WakeUpOccured */
    if((CANGIF0 & kCanWuif0) == kCanWuif0)
    {
    /* KB end CanLL_if_WakeUpOccured */
      CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
      CanLL_WakeUpHandling(CAN_CHANNEL_CANPARA_ONLY);
      CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
    }
#  if defined(C_DISABLE_TASK_RECURSION_CHECK)
#   if defined(C_ENABLE_USER_CHECK)
    canPollingTaskActive[channel]--;
#   endif
#  else
    canPollingTaskActive[channel] = 0;
  }
#  endif
}
# endif /* C_ENABLE_WAKEUP_POLLING */
#endif /* C_ENABLE_SLEEP_WAKEUP */

#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_CANCEL_IN_HW )
/****************************************************************************
| NAME:             CanTxTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task,
|                   - polling Tx objects
****************************************************************************/
C_API_1 void C_API_2 CanTxTask( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanSignedTxHandle    i;
#if defined(C_ENABLE_TRANSMIT_QUEUE) && \
    defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
  canuint8             rc;
#endif

  /* KB begin CanLL_TxTaskLocalParameter */
  /* KB end CanLL_TxTaskLocalParameter */

  #if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanDeclareGlobalInterruptOldStatus
  #endif


#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
  canPollingTaskActive[channel]++;
  assertUser((canPollingTaskActive[channel] == 1), channel, kErrorPollingTaskRecursion);
#  endif
# else
  if (canPollingTaskActive[channel] == 0)      /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;
# endif


    /*--  polling Tx objects ----------------------------------------*/

# if defined( C_ENABLE_TX_POLLING )
    /* check for global confirmation Pending and may be reset global pending confirmation */
    /* KB begin CanLL_if_TxIsGlobalConfPending */
    # if defined(C_COMP_TI_TMS470)
    #  if defined(C_ENABLE_CANCEL_IN_HW)
    if((CANTA | CANAA) != (tCanRegType)0x00000000)
    #  else
    if(CANTA != (tCanRegType)0x00000000)
    #  endif
    # else
    if(CANTA != (tCanRegType)0x00000000)
    #endif
    {
    /* KB end CanLL_if_TxIsGlobalConfPending */
      for ( i = (CanSignedTxHandle)CAN_HL_HW_TX_STARTINDEX(canHwChannel); i < (CanSignedTxHandle)CAN_HL_HW_TX_STOPINDEX(canHwChannel) ; i++ )
      {
        /* check for dedicated confirmation pending */
        /* KB begin CanLL_if_TxIsObjConfPending */
        if((CANTA & CanSlotMask[i]) == CanSlotMask[i])
        {
        /* KB end CanLL_if_TxIsObjConfPending */
          CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
          /* do tx confirmation */
          CanHL_TxConfirmation(CAN_HW_CHANNEL_CANPARA_FIRST (CanObjectHandle) i );
          CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
        }
      }
    }
# endif /*( C_ENABLE_TX_POLLING ) */

    #if defined( C_ENABLE_CANCEL_IN_HW ) && \
        defined( C_HL_ENABLE_CANCEL_IN_HW_TASK )
    /* search for pending cancelation */

    for ( i = (CanSignedTxHandle)CAN_HL_HW_TX_STARTINDEX(canHwChannel); i < (CanSignedTxHandle)CAN_HL_HW_TX_STOPINDEX(canHwChannel) ; i++ )
    {
      /* check for dedicated cancel pending */
      /* KB begin CanLL_if_TxIsObjCanceledAcknolaged */
      # error "CanLL_if_TxIsObjCanceledAcknolaged not used"
      /* KB end CanLL_if_TxIsObjCanceledAcknolaged */
        canHandleCurTxObj[i+ CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] = kCanBufferFree;     /* release the hardware buffer */
      }
    }
    #endif

    #if defined( C_ENABLE_TRANSMIT_QUEUE )
    if (canHandleCurTxObj[CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] == kCanBufferFree)
    {
      if (canTxQueueCnt[channel] != 0)
      {
        /* Transmit Queued Objects ( instead of function CanTransmitQueuedObj() */
        for (i = (CanSignedTxHandle)CAN_HL_TX_STOPINDEX(channel) - (CanSignedTxHandle)1; i >= (CanSignedTxHandle)CAN_HL_TX_STARTINDEX(channel); i--)
        {                           /*look for obj ready to transmit*/
          if ((canTxQueueFlags[i]) != 0)
          {
            CanSingleGlobalInterruptDisable();
            if ((canHandleCurTxObj[CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] == kCanBufferFree)
                                                                                               && (canTxQueueCnt[channel] != 0))
            {
              canTxQueueFlags[i] = 0;        /* remove msg from queue */
              canTxQueueCnt[channel]--;
              /* Save hdl of msgObj to be transmitted*/
              canHandleCurTxObj[CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] = (CanTransmitHandle)i;
              CanSingleGlobalInterruptRestore();
#  if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
              rc = CanCopyDataAndStartTransmission(CAN_CHANNEL_CANPARA_FIRST CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel), (CanTransmitHandle)i);
              if ( rc == kCanTxNotify)
              {
                APPLCANCANCELNOTIFICATION(channel, (CanTransmitHandle)i);
              }
#  else
              (void)CanCopyDataAndStartTransmission(CAN_CHANNEL_CANPARA_FIRST CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel), (CanTransmitHandle)i);
#  endif
              break;
            }
            else
            {
              CanSingleGlobalInterruptRestore();
              break;
            }
          }
        }
      }
    }
    #endif /*  C_ENABLE_TRANSMIT_QUEUE */


# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
    canPollingTaskActive[channel]--;
#  endif
# else
    canPollingTaskActive[channel] = 0;
  }
# endif


} /* END OF CanTxTask */
#endif /* C_ENABLE_TX_POLLING */

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_FULLCAN_POLLING )
/****************************************************************************
| NAME:             CanRxFullCANTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task,
|                   - polling Rx FullCAN objects
****************************************************************************/
C_API_1 void C_API_2 CanRxFullCANTask(CAN_CHANNEL_CANTYPE_ONLY)
{
  /* KB begin CanLL_RxFullCANTaskLocalParameter */
  /* KB end CanLL_RxFullCANTaskLocalParameter */

  register CanObjectHandle     rxObjHandle;


#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
  canPollingTaskActive[channel]++;
  assertUser((canPollingTaskActive[channel] == 1), channel, kErrorPollingTaskRecursion);
#  endif
# else
  if (canPollingTaskActive[channel] == 0)           /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;
# endif

    {
      /*--  polling fullcan Rx objects ----------------------------------------*/

      /* check for global fullCan Rx indication pending and may be reset global */
      /* indication pending */
      /* KB begin CanLL_if_RxFullIsGlobalIndPending */
      if(CANRMP != (tCanRegType)0x00000000)
      {
      /* KB end CanLL_if_RxFullIsGlobalIndPending */
        for (rxObjHandle=CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel); rxObjHandle<CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel); rxObjHandle++ )
        {
          /* check for dedicated indication pending */
          /* KB begin CanLL_if_RxFullIsObjIndPending */
          if((CANRMP & CanSlotMask[rxObjHandle]) == CanSlotMask[rxObjHandle])
          {
          /* KB end CanLL_if_RxFullIsObjIndPending */
            CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
            CanFullCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
            CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
          }
        }
      } /* if (global pending) */
    }  /* for (all associated HW channel) */
# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
    canPollingTaskActive[channel]--;
#  endif
# else
    canPollingTaskActive[channel] = 0;
  }
# endif

} /* END OF CanRxTask */
#endif /*  C_ENABLE_RX_FULLCAN_OBJECTS && C_ENABLE_RX_FULLCAN_POLLING */

#if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanRxBasicCANTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task,
|                   - polling Rx BasicCAN objects
****************************************************************************/
C_API_1 void C_API_2 CanRxBasicCANTask(CAN_CHANNEL_CANTYPE_ONLY)
{
  /* KB begin CanLL_RxBasicCANTaskLocalParameter */
  /* CanLL_RxBasicCANTaskLocalParameter not used */
  /* KB end CanLL_RxBasicCANTaskLocalParameter */

  register CanObjectHandle     rxObjHandle;


#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
  canPollingTaskActive[channel]++;
  assertUser((canPollingTaskActive[channel] == 1), channel, kErrorPollingTaskRecursion);
#  endif
# else
  if (canPollingTaskActive[channel] == 0)           /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;
# endif


    /* KB begin CanLL_if_RxBasicIsGlobalIndPending */
    if((CANRMP & kCanBasicCanMask) != (tCanRegType)0x00000000)
    {
    /* KB end CanLL_if_RxBasicIsGlobalIndPending */
      for (rxObjHandle=CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); rxObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); rxObjHandle++ )
      {
        /* check for dedicated indication pending */
        /* KB begin CanLL_if_RxBasicIsObjIndPending */
        if((CANRMP & CanSlotMask[rxObjHandle]) == CanSlotMask[rxObjHandle])
        {
        /* KB end CanLL_if_RxBasicIsObjIndPending */
          CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
          CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
          CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
        }
      }
    }
# if defined(C_DISABLE_TASK_RECURSION_CHECK)
#  if defined(C_ENABLE_USER_CHECK)
    canPollingTaskActive[channel]--;
#  endif
# else
    canPollingTaskActive[channel] = 0;
  }
# endif

} /* END OF CanRxTask */
#endif /* C_ENABLE_RX_BASICCAN_POLLING && C_ENABLE_RX_BASICCAN_OBJECTS */

/****************************************************************************
| NAME:             CanHL_ErrorHandling
| CALLED BY:        CanISR(), CanErrorTask()
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      - error interrupt (busoff, error warning,...)
****************************************************************************/
static void CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* KB begin CanLL_ErrorHandlingLocalParameter */
  /* CanLL_ErrorHandlingLocalParameter not used */
  /* KB end CanLL_ErrorHandlingLocalParameter */


  /* KB begin CanLL_ErrorHandlingBegin */
  /* CanLL_ErrorHandlingBegin not used */
  /* KB end CanLL_ErrorHandlingBegin */

  /* check for status register (bus error)--*/
  /* KB begin CanLL_if_BusOffOccured */
  if((CANGIF0 & kCanBoif0) == kCanBoif0) /* check for bus off */
  {
    CANGIF0 = kCanBoif0;                                    /* clear bus off flag */
  /* KB end CanLL_if_BusOffOccured */
    /*==BUS OFF ERROR=========================*/
    APPL_CAN_BUSOFF( CAN_CHANNEL_CANPARA_ONLY );            /* call application specific function */
  }

} /* END OF CanStatusInterrupt */


#if defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanBasicCanMsgReceived
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: internal can chip number
| RETURN VALUES:    none
| DESCRIPTION:      - basic can receive
****************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return and usage of goto */
#  pragma PRQA_MESSAGES_OFF 2006,2001,2015
# endif
static void CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle)
{
# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
# endif

# if !defined (C_SEARCH_HASH) || \
     defined ( C_HL_ENABLE_HW_RANGES_FILTER ) || \
     defined ( C_ENABLE_RANGE_0 ) || \
     defined ( C_ENABLE_RANGE_1 ) || \
     defined ( C_ENABLE_RANGE_2 ) || \
     defined ( C_ENABLE_RANGE_3 )
  tCanRxId0 idRaw0;
#  if (kCanNumberOfUsedCanRxIdTables > 1)
  tCanRxId1 idRaw1;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 2)
  tCanRxId2 idRaw2;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 3)
  tCanRxId3 idRaw3;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 4)
  tCanRxId4 idRaw4;
#  endif
# endif

  #if defined( C_SEARCH_HASH )
    #if defined( C_ENABLE_EXTENDED_ID )
  canuint32          id;
  canuint32          wintern;        /* prehashvalue         */
    #else
  canuint16          id;
  canuint16          wintern;        /* prehashvalue         */
    #endif
  canuint16          i_increment;    /* delta for next step  */
  cansint16          count;
  #endif

  /* KB begin CanLL_RxBasicCanMsgReceivedLocalParameter */
  #if defined(C_ENABLE_OVERRUN)
  tCanRegType bOverrunPending;
  #endif
  /* KB end CanLL_RxBasicCanMsgReceivedLocalParameter */

#if defined( C_ENABLE_GENERIC_PRECOPY ) || \
    defined( C_ENABLE_PRECOPY_FCT )     || \
    defined( C_ENABLE_COPY_RX_DATA )    || \
    defined( C_ENABLE_INDICATION_FLAG ) || \
    defined( C_ENABLE_INDICATION_FCT )  || \
    defined( C_ENABLE_DLC_CHECK )       || \
    defined( C_ENABLE_NOT_MATCHED_FCT )

# if defined( C_SEARCH_HASH )
  CanReceiveHandle        i = (CanReceiveHandle)kCanBufferFree;
# else
  CanSignedRxHandle        i = (CanSignedRxHandle)kCanBufferFree;
# endif
#endif



  /* KB begin CanLL_RxBasicMsgReceivedBegin */
  #if defined (C_ENABLE_OVERRUN)
  bOverrunPending = ((CANRML & CanSlotMask[rxObjHandle]) == CanSlotMask[rxObjHandle]) ? 1 : 0;
  #endif
  CANRMP = CanSlotMask[rxObjHandle]; /* reset interrupt flag */
  /* Copy data */
  #if defined(C_COMP_TI_TMS470)
  # if defined(C_ENABLE_HECC_WORKAROUND_2)
  CanLockGlobalInterrupt();
  canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
  canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
  CanUnlockGlobalInterrupt();
  # else
  canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
  # endif
  #endif
  APPLCANTIMERSTART(kCanLoopCopyRxBasicMailBox);
  while((CANRMP & CanSlotMask[rxObjHandle]) == CanSlotMask[rxObjHandle])
  {
  #if defined(C_ENABLE_HW_LOOP_TIMER)
    if(0 == APPLCANTIMERLOOP(kCanLoopCopyRxBasicMailBox))
    {
      break;
    }
  #endif
    CANRMP = CanSlotMask[rxObjHandle]; /* reset interrupt flag */
  /* Copy data */
  #if defined(C_COMP_TI_TMS470)
  # if defined(C_ENABLE_HECC_WORKAROUND_2)
    CanLockGlobalInterrupt();
    canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
    canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
    CanUnlockGlobalInterrupt();
  # else
    canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
  # endif
  #endif
  #if defined (C_ENABLE_OVERRUN)
    bOverrunPending = 1;  /* overrun occured */
  #endif
  }
  APPLCANTIMEREND(kCanLoopCopyRxBasicMailBox);
  /* KB end CanLL_RxBasicMsgReceivedBegin */

#if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
  /* KB begin CanLL_RxBasicGetCANObjPtr */
  (pCanRxInfoStruct->pChipMsgObj) = (CanChipMsgPtr)&(canTempCanObj[channel]);
  /* KB end CanLL_RxBasicGetCANObjPtr */
  /* KB begin CanLL_RxBasicGetCANDataPtr */
  (pCanRxInfoStruct->pChipData) = (CanChipMsgPtr)&(canTempCanObj[channel].CanMsg.bytes);
  /*lint +e740 +e826 cast okay */
  /* KB end CanLL_RxBasicGetCANDataPtr */
  pCanRxInfoStruct->Handle      = kCanRxHandleNotUsed;
#else
  /* KB begin CanLL_RxBasicGetCANObjPtr */
  (canRxInfoStruct[channel].pChipMsgObj) = (CanChipMsgPtr)&(canTempCanObj[channel]);
  /* KB end CanLL_RxBasicGetCANObjPtr */
  /* KB begin CanLL_RxBasicGetCANDataPtr */
  (canRxInfoStruct[channel].pChipData) = (CanChipMsgPtr)&(canTempCanObj[channel].CanMsg.bytes);
  /*lint +e740 +e826 cast okay */
  /* KB end CanLL_RxBasicGetCANDataPtr */
  canRxInfoStruct[channel].Handle      = kCanRxHandleNotUsed;
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    goto finishBasicCan; /* ignore reception */
  }
#endif

  /* reject messages with unallowed ID type */
  #if defined( C_ENABLE_EXTENDED_ID )
  # if defined( C_ENABLE_MIXED_ID )
  # else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanIdTypeExt)
  {
    goto finishBasicCan;
  }
  # endif
  #else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanIdTypeStd)
  {
    goto finishBasicCan;
  }
  #endif


    #if defined( C_ENABLE_OVERRUN )
  /* KB begin CanLL_if_RxBasicCanOverrun */
  if(bOverrunPending != 0)
  {
  /* KB end CanLL_if_RxBasicCanOverrun */
    ApplCanOverrun( CAN_CHANNEL_CANPARA_ONLY );
  }
    #endif

  #if defined( C_ENABLE_COND_RECEIVE_FCT )
  if(canMsgCondRecState[channel]==kCanTrue)
  {
    ApplCanMsgCondReceived( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
  #endif

  #if defined( C_ENABLE_RECEIVE_FCT )
  if (APPL_CAN_MSG_RECEIVED( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
  {
    goto finishBasicCan;
  }
  #endif

# if !defined (C_SEARCH_HASH) || \
     defined ( C_HL_ENABLE_HW_RANGES_FILTER ) || \
     defined ( C_ENABLE_RANGE_0 ) || \
     defined ( C_ENABLE_RANGE_1 ) || \
     defined ( C_ENABLE_RANGE_2 ) || \
     defined ( C_ENABLE_RANGE_3 )
#  if defined( C_ENABLE_EXTENDED_ID )
#   if defined( C_ENABLE_MIXED_ID )
  if (CanRxActualIdType(CAN_HL_P_RX_INFO_STRUCT(channel)) == kCanIdTypeExt)
#   endif
  {
#   if defined( C_ENABLE_RX_MASK_EXT_ID )
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID0(C_MASK_EXT_ID);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID1(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID2(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID3(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID4(C_MASK_EXT_ID);
#    endif
#   else
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID0(0x1FFFFFFF);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID1(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID2(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID3(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID4(0x1FFFFFFF);
#    endif
#   endif /*  C_ENABLE_RX_MASK_EXT_ID */
  }
#   if defined( C_ENABLE_MIXED_ID )
  else
  {
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID0(0x7FF);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID1(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID2(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID3(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID4(0x7FF);
#    endif
  }
#   endif
#  else /* C_ENABLE_EXTENDED_ID */
  {
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID0(0x7FF);
#   if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID1(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID2(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID3(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID4(0x7FF);
#   endif
  }
#  endif /* C_ENABLE_EXTENDED_ID */
# endif /* !defined (C_SEARCH_HASH) || ...  defined ( C_ENABLE_RANGE_3 )*/

# if defined(C_HL_ENABLE_HW_RANGES_FILTER)

  /* KB begin CanLL_RxHWRangeFilter */
  /* CanLL_RxHWRangeFilter not used */
  /* KB end CanLL_RxHWRangeFilter */

# else

  #if defined( C_ENABLE_MIXED_ID )
  #  if defined( C_ENABLE_RANGE_EXTENDED_ID )
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
  #  else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
  #  endif
  #endif
  {
    #if defined( C_ENABLE_RANGE_0 )
    # if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange0) != 0 )
    # endif
    {
      if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
      {
      #if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
        pCanRxInfoStruct->EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel));
      #else
        canRxInfoStruct[channel].EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel));
      #endif
        (void)APPLCANRANGE0PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) );  
        goto finishBasicCan;
      }
    }
    #endif  /* C_ENABLE_RANGE_0 */

    #if defined( C_ENABLE_RANGE_1 )
    # if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange1) != 0 )
    # endif
    {
      if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
      {
      #if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
        pCanRxInfoStruct->EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel));
      #else
        canRxInfoStruct[channel].EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel));
      #endif
        (void)APPLCANRANGE1PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) );
        goto finishBasicCan;
      }
    }
    #endif

    #if defined( C_ENABLE_RANGE_2 )
    # if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange2) != 0 )
    # endif
    {
      if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
      {
      #if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
        pCanRxInfoStruct->EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel));
      #else
        canRxInfoStruct[channel].EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel));
      #endif
        (void)APPLCANRANGE2PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) );
        goto finishBasicCan;
      }
    }
    #endif

    #if defined( C_ENABLE_RANGE_3 )
    # if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange3) != 0 )
    # endif
    {
      if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
      {
      #if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
        pCanRxInfoStruct->EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel));
      #else
        canRxInfoStruct[channel].EcuNumber = C_MK_ECU_NUMBER(CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel));
      #endif
        (void)APPLCANRANGE3PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) );
        goto finishBasicCan;
      }
    }
    #endif
  }

# endif /* defined(C_HL_ENABLE_HW_RANGES_FILTER) */

#if defined( C_ENABLE_GENERIC_PRECOPY ) || \
    defined( C_ENABLE_PRECOPY_FCT )     || \
    defined( C_ENABLE_COPY_RX_DATA )    || \
    defined( C_ENABLE_INDICATION_FLAG ) || \
    defined( C_ENABLE_INDICATION_FCT )  || \
    defined( C_ENABLE_DLC_CHECK )       || \
    defined( C_ENABLE_NOT_MATCHED_FCT )
   /* search the received id in ROM table: */

  /* KB begin CanLL_HwSpezificAcceptanceFilter */
  /* CanLL_HwSpezificAcceptanceFilter not used */
  /* KB end CanLL_HwSpezificAcceptanceFilter */

# if defined( C_SEARCH_LINEAR )
  /* ************* Linear search ******************************************** */
  for (i = (CanSignedRxHandle)CAN_HL_RX_BASIC_STOPINDEX(channel)-(CanSignedRxHandle)1;
                                      i >= (CanSignedRxHandle)CAN_HL_RX_BASIC_STARTINDEX(channel) ;i--)
  {
    if( idRaw0 == CanGetRxId0(i) )
    {
#  if (kCanNumberOfUsedCanRxIdTables > 1)
      if( idRaw1 == CanGetRxId1(i) )
#  endif
      {
#  if (kCanNumberOfUsedCanRxIdTables > 2)
        if( idRaw2 == CanGetRxId2(i) )
#  endif
        {
#  if (kCanNumberOfUsedCanRxIdTables > 3)
          if( idRaw3 == CanGetRxId3(i) )
#  endif
          {
#  if (kCanNumberOfUsedCanRxIdTables > 4)
            if( idRaw4 == CanGetRxId4(i) )
#  endif
            {
#  if defined(C_ENABLE_MIXED_ID)
#   if defined(C_HL_ENABLE_IDTYPE_IN_ID)
#   else
              /* verify ID type, if not already done with the ID raw */
              if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CanGetRxIdType(i))
#   endif
#  endif
              {
                break;    /*exit loop with index i */
              }
            }
          }
        }
      }
    }
  }
# endif

# if defined ( C_SEARCH_HASH )
  /* ************* Hash search ********************************************* */
#  if defined ( C_CPUTYPE_8BIT )
#   error "Hash search with 8-Bit CPU not tested yet"
#  endif

#  if defined( C_ENABLE_EXTENDED_ID )
  /* one or more Extended ID listed */
#   if defined(C_ENABLE_MIXED_ID)
  if((CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) )) == kCanIdTypeExt)
#   endif
#   if (kHashSearchListCountEx > 0)
  {
  /* calculate the logical ID */
#    if defined(C_ENABLE_RX_MASK_EXT_ID)
    id          = (CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) ) &  C_MASK_EXT_ID ) | \
                                                                               ((canuint32)channel << 29);
#    else
    id          = CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) )| ((canuint32)channel << 29);
#    endif

    wintern     = id + kHashSearchRandomNumberEx;
    i           = (CanReceiveHandle)(wintern % kHashSearchListCountEx);
#    if (kHashSearchListCountEx == 1)
    i_increment = 0;
#    else
    i_increment = (wintern % (kHashSearchListCountEx - 1) + 1);
#    endif
    count       = kHashSearchMaxStepsEx-1;

    while(id != CanRxHashIdEx[i])
    {
      if(count == 0)
      {
#    if defined(C_ENABLE_NOT_MATCHED_FCT)
        ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#    endif
        goto finishBasicCan;
      }
      count--;
      i += i_increment;
      if( i >= (canuint16)kHashSearchListCountEx )
      {
        i -= kHashSearchListCountEx;
      }
    }
    i = CanRxMsgIndirection[i+kHashSearchListCount];
  }
#   else /* (kHashSearchListCountEx > 0) */
  {
#    if defined(C_ENABLE_NOT_MATCHED_FCT)
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#    endif
    goto finishBasicCan;
  }
#   endif /* (kHashSearchListCountEx > 0) */

#   if defined(C_ENABLE_MIXED_ID)
  else if((CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) )) == kCanIdTypeStd)
#   endif
#  endif /* IF defined( C_ENABLE_EXTENDED_ID ) */

#  if defined(C_ENABLE_MIXED_ID) || !defined( C_ENABLE_EXTENDED_ID )
#   if (kHashSearchListCount > 0)
  {
    id          = (canuint16)(CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) )
                               | ((canuint16)channel << 11));                /* calculate the logical ID */
    wintern     = id + kHashSearchRandomNumber;
    i           = (CanReceiveHandle)(wintern % kHashSearchListCount);
#    if (kHashSearchListCount == 1)
    i_increment = 0;
#    else
    i_increment = (wintern % (kHashSearchListCount - 1) + 1);
#    endif
    count       = kHashSearchMaxSteps-1;

    /* type of CanRxHashId table depends on the used type of Id */
    while ( id != CanRxHashId[i])
    {
      if (count == 0)
      {
        #if defined ( C_ENABLE_NOT_MATCHED_FCT )
        ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
        #endif
        goto finishBasicCan;
      }
      count--;
      i += i_increment;
      if ( i >= kHashSearchListCount )
      {
        i -= kHashSearchListCount;
      }
    }
    i = CanRxMsgIndirection[i];
  }
#   else /* (kHashSearchListCount > 0) */
  {
#    if defined(C_ENABLE_NOT_MATCHED_FCT)
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#    endif
    goto finishBasicCan;
  }
#   endif /* (kHashSearchListCount > 0) */
#  endif /* defined(C_ENABLE_MIXED_ID) || !defined( C_ENABLE_EXTENDED_ID ) */
# endif /* defined ( C_SEARCH_HASH ) */

# if defined ( C_SEARCH_HASH )
  assertInternal((i <= kCanNumberOfRxObjects), kCanAllChannels , kErrorRxHandleWrong);  /* legal txHandle ? */
# else
  if ( i >= (CanSignedRxHandle)CAN_HL_RX_BASIC_STARTINDEX(channel))
# endif
  {
    /* ID found in table */
    #if defined ( C_SEARCH_HASH )
    #else
     #if defined( C_ENABLE_RX_MSG_INDIRECTION )
    i = CanRxMsgIndirection[i];       /* indirection for special sort-algoritms */
     #endif
    #endif
    if (CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST (CanReceiveHandle)i ) == kCanHlContinueRx)
    {
      #if defined( C_ENABLE_INDICATION_FLAG ) || \
          defined( C_ENABLE_INDICATION_FCT )
      /* KB begin CanLL_RxBasicReleaseObj */
      /* KB end CanLL_RxBasicReleaseObj */

      CanHL_IndRxHandle( (CanReceiveHandle)i );

      /* KB begin CanLL_RxBasicMsgReceivedEnd */
      /* CanLL_RxBasicMsgReceivedEnd not used */
      /* KB end CanLL_RxBasicMsgReceivedEnd */

      return;
      #endif
    }
  }
  #if defined ( C_ENABLE_NOT_MATCHED_FCT )
    #if defined( C_SEARCH_HASH )
    #else
  else
  {
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
    #endif
  #endif

#endif

  goto finishBasicCan;     /* to avoid compiler warning */

/* if defined( C_ENABLE_RANGE_0 ) || \
       defined( C_ENABLE_RANGE_1 ) || \
       defined( C_ENABLE_RANGE_2 ) || \
       defined( C_ENABLE_RANGE_3 ) || \
       defined( C_ENABLE_RECEIVE_FCT ) || \
       defined( C_SEARCH_HASH )  */
/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishBasicCan:
/* endif */

  /* make receive buffer free*/
  /* KB begin CanLL_RxBasicReleaseObj */
  /* KB end CanLL_RxBasicReleaseObj */
  /* KB begin CanLL_RxBasicMsgReceivedEnd */
  /* CanLL_RxBasicMsgReceivedEnd not used */
  /* KB end CanLL_RxBasicMsgReceivedEnd */


  return;    /* to avoid compiler warnings about label without code */

} /* end of BasicCan */
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006,2001,2015
# endif
#endif

#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
/****************************************************************************
| NAME:             CanFullCanMsgReceived
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: internal can chip number
| RETURN VALUES:    none
| DESCRIPTION:      - full can receive
****************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return and usage of goto */
#  pragma PRQA_MESSAGES_OFF 2006,2001,2015
# endif
static void CanFullCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle )
{
  CanReceiveHandle   rxHandle;

#if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
#endif

  /* KB begin CanLL_RxFullCanMsgReceivedLocalParameter */
  #if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
  # if defined(C_ENABLE_RX_BASICCAN_POLLING)
  # endif
  #endif
  #if defined(C_ENABLE_FULLCAN_OVERRUN)
  tCanRegType bOverrunPending;
  #endif
  /* KB end CanLL_RxFullCanMsgReceivedLocalParameter */



  /* KB begin CanLL_RxFullMsgReceivedBegin */
  # if defined(C_ENABLE_FULLCAN_OVERRUN)
  bOverrunPending = ((CANRML & CanSlotMask[rxObjHandle]) == CanSlotMask[rxObjHandle]) ? 1 : 0;
  # endif
  APPLCANTIMERSTART(kCanLoopCopyRxMailBox);
  while(CANRMP & CanSlotMask[rxObjHandle])
  {
  # if defined(C_ENABLE_HW_LOOP_TIMER)
    if(0 == APPLCANTIMERLOOP(kCanLoopCopyRxMailBox))
    {
      break;
    }
  # endif
    CANRMP = CanSlotMask[rxObjHandle]; /* reset interrupt flag */
  # if defined(C_COMP_TI_TMS470)
    canTempCanObj[channel] = *(tCanMailBox*)&(CANMSGOBJ(rxObjHandle));
  # endif

  }
  APPLCANTIMEREND(kCanLoopCopyRxMailBox);
  /* KB end CanLL_RxFullMsgReceivedBegin */

#if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  /* pointer needed for other modules */
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
  /* KB begin CanLL_RxFullGetCANObjPtr */
  (pCanRxInfoStruct->pChipMsgObj) = (CanChipMsgPtr)&(canTempCanObj[channel]);
  /* KB end CanLL_RxFullGetCANObjPtr */
  /* KB begin CanLL_RxFullGetCANDataPtr */
  (pCanRxInfoStruct->pChipData) = (CanChipDataPtr)&(canTempCanObj[channel].CanMsg.bytes);
  /* KB end CanLL_RxFullGetCANDataPtr */
#else
  /* KB begin CanLL_RxFullGetCANObjPtr */
  (canRxInfoStruct[channel].pChipMsgObj) = (CanChipMsgPtr)&(canTempCanObj[channel]);
  /* KB end CanLL_RxFullGetCANObjPtr */
  /* KB begin CanLL_RxFullGetCANDataPtr */
  (canRxInfoStruct[channel].pChipData) = (CanChipDataPtr)&(canTempCanObj[channel].CanMsg.bytes);
  /* KB end CanLL_RxFullGetCANDataPtr */
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    goto finishRxFullCan; /* ignore reception */
  }
#endif

#if defined( C_ENABLE_EXTENDED_ID )
# if defined( C_ENABLE_MIXED_ID )
# else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanIdTypeExt)
  {
    goto finishRxFullCan;
  }
# endif
#else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanIdTypeStd)
  {
    goto finishRxFullCan;
  }
#endif

# if defined( C_ENABLE_FULLCAN_OVERRUN )

  /* KB begin CanLL_if_RxFullCanOverrun */
  if(bOverrunPending)
  {
  /* KB end CanLL_if_RxFullCanOverrun */
     ApplCanFullCanOverrun( CAN_CHANNEL_CANPARA_ONLY );
  }
# endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
  if(canMsgCondRecState[channel]==kCanTrue)
  {
    ApplCanMsgCondReceived( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
#endif

#if defined( C_ENABLE_RECEIVE_FCT )
  if (APPL_CAN_MSG_RECEIVED( CAN_HL_P_RX_INFO_STRUCT(channel) )==kCanNoCopyData)
  {
     goto finishRxFullCan;
  }
#endif

  /* calculate the message handle to access the generated data for the received message */

  /* brackets to avoid lint info 834 */
  rxHandle = (rxObjHandle - CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel))
#if defined( C_SEARCH_HASH )
                         + kHashSearchListCount
                         + kHashSearchListCountEx
#endif
                         + CAN_HL_RX_FULL_STARTINDEX(canHwChannel);

#if defined(C_HL_ENABLE_ADJUST_RXHANDLE)
  /* KB begin CanLL_AdjustRxHandle */
  /* CanLL_AdjustRxHandle not used */
  /* KB end CanLL_AdjustRxHandle */
#endif

# if defined( C_ENABLE_RX_MSG_INDIRECTION ) || \
     defined( C_SEARCH_HASH )
  rxHandle = CanRxMsgIndirection[rxHandle];
# endif


  if (rxHandle != kCanBufferFree)      /* if msg exists in ROM table */
  {
    if (CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST rxHandle ) == kCanHlContinueRx)
    {
      #if defined( C_ENABLE_INDICATION_FLAG ) || \
          defined( C_ENABLE_INDICATION_FCT )
      /* KB begin CanLL_RxFullReleaseObj */
      ; /* needed because of the label! */
      /* KB end CanLL_RxFullReleaseObj */

      CanHL_IndRxHandle( rxHandle );

      /* KB begin CanLL_RxFullMsgReceivedEnd */
      /* CanLL_RxFullMsgReceivedEnd not used */
      /* KB end CanLL_RxFullMsgReceivedEnd */

      return;
      #endif
    }
  }

  goto finishRxFullCan;     /* to avoid compiler warning */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishRxFullCan:

  /* make receive buffer free*/
  /* KB begin CanLL_RxFullReleaseObj */
  ; /* needed because of the label! */
  /* KB end CanLL_RxFullReleaseObj */
  /* KB begin CanLL_RxFullMsgReceivedEnd */
  /* CanLL_RxFullMsgReceivedEnd not used */
  /* KB end CanLL_RxFullMsgReceivedEnd */


  return;    /* to avoid compiler warnings about label without code */
}
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006,2001,2015
# endif
#endif

#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )  || \
    defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanHL_ReceivedRxHandle
| CALLED BY:        CanBasicCanMsgReceived, CanFullCanMsgReceived
| PRECONDITIONS:
| INPUT PARAMETERS: Handle of received Message to access generated data
| RETURN VALUES:    none
| DESCRIPTION:      DLC-check, Precopy and copy of Data for received message
****************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006
# endif
static canuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_FIRST CanReceiveHandle rxHandle )
{
#if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
#endif

  #if defined( C_ENABLE_COPY_RX_DATA )
    #if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
    #endif
  #endif

# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
  pCanRxInfoStruct->Handle = rxHandle;
# else
  canRxInfoStruct[channel].Handle = rxHandle;
# endif

#if defined ( C_ENABLE_MULTI_ECU_PHYS )
  if ( (CanRxIdentityAssignment[rxHandle] & V_ACTIVE_IDENTITY) == (tVIdentity)0 )
  {
    /* message is not a receive message in the active indentity */
    return  kCanHlFinishRx;
  }
#endif


#if defined( C_ENABLE_DLC_CHECK )
# if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  /* ##RI1.4 - 3.31: Dynamic Receive DLC */
  if ( canVariableRxDataLen[rxHandle]       > CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel) ) )
# else
  if ( (CanGetRxDataLen(rxHandle)) > CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel) ) )
# endif
  {
    /* ##RI1.4 - 2.7: Callbackfunction-DLC-Check */
# if defined ( C_ENABLE_DLC_FAILED_FCT )
    ApplCanMsgDlcFailed( CAN_HL_P_RX_INFO_STRUCT(channel) );
# endif  /*C_ENABLE_DLC_FAILED_FCT */
    return  kCanHlFinishRx;
  }
#endif

  #if defined( C_ENABLE_GENERIC_PRECOPY )
  if ( ApplCanGenericPrecopy( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanCopyData)
  {
    return kCanHlFinishRx;
  }
  #endif

  #if defined( C_ENABLE_PRECOPY_FCT )
    #if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
    #else
  if ( CanGetApplPrecopyPtr(rxHandle) != NULL )    /*precopy routine */
    #endif
  {
    /* canRxHandle in indexed drivers only for consistancy check in higher layer modules */
    canRxHandle[channel] = rxHandle;

    if ( CanGetApplPrecopyPtr(rxHandle)( CAN_HL_P_RX_INFO_STRUCT(channel) )==kCanNoCopyData )
    {  /* precopy routine returns kCanNoCopyData:   */
      return  kCanHlFinishRx;
    }                      /* do not copy data check next irpt */
  }
  #endif

  #if defined( C_ENABLE_COPY_RX_DATA )
  /* no precopy or precopy returns kCanCopyData : copy data -- */
  /* copy via index -------------------------------------------*/
  if ( CanGetRxDataPtr(rxHandle) != NULL )      /* copy if buffer exists */
  {
    /* copy data ---------------------------------------------*/
    #if C_SECURITY_LEVEL > 20
    CanSingleGlobalInterruptDisable();
    #endif
# if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  /* ##RI1.4 - 3.31: Dynamic Receive DLC */
    #if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
    /* KB begin CanLL_RxCopyFromCan */
    /*lint -e616 -e825 fallthrough wanted */
    #if defined(C_COMP_TI_TMS470)
    switch(canVariableRxDataLen[rxHandle])
    {
    case 8:  ((vuint8*)CanGetRxDataPtr(rxHandle))[7] = ((vuint8*)pCanRxInfoStruct->pChipData)[7];
    case 7:  ((vuint8*)CanGetRxDataPtr(rxHandle))[6] = ((vuint8*)pCanRxInfoStruct->pChipData)[6];
    case 6:  ((vuint8*)CanGetRxDataPtr(rxHandle))[5] = ((vuint8*)pCanRxInfoStruct->pChipData)[5];
    case 5:  ((vuint8*)CanGetRxDataPtr(rxHandle))[4] = ((vuint8*)pCanRxInfoStruct->pChipData)[4];
    case 4:  ((vuint8*)CanGetRxDataPtr(rxHandle))[3] = ((vuint8*)pCanRxInfoStruct->pChipData)[3];
    case 3:  ((vuint8*)CanGetRxDataPtr(rxHandle))[2] = ((vuint8*)pCanRxInfoStruct->pChipData)[2];
    case 2:  ((vuint8*)CanGetRxDataPtr(rxHandle))[1] = ((vuint8*)pCanRxInfoStruct->pChipData)[1];
    case 1:  ((vuint8*)CanGetRxDataPtr(rxHandle))[0] = ((vuint8*)pCanRxInfoStruct->pChipData)[0];
    case 0:  break;
    default: break;
    }
    #endif
    /*lint +e616 +e825 fallthrough wanted */
    /* KB end CanLL_RxCopyFromCan */
    #else
    /* KB begin CanLL_RxCopyFromCan */
    /*lint -e616 -e825 fallthrough wanted */
    #if defined(C_COMP_TI_TMS470)
    switch(canVariableRxDataLen[rxHandle])
    {
    case 8:  ((vuint8*)CanGetRxDataPtr(rxHandle))[7] = ((vuint8*)canRxInfoStruct[channel].pChipData)[7];
    case 7:  ((vuint8*)CanGetRxDataPtr(rxHandle))[6] = ((vuint8*)canRxInfoStruct[channel].pChipData)[6];
    case 6:  ((vuint8*)CanGetRxDataPtr(rxHandle))[5] = ((vuint8*)canRxInfoStruct[channel].pChipData)[5];
    case 5:  ((vuint8*)CanGetRxDataPtr(rxHandle))[4] = ((vuint8*)canRxInfoStruct[channel].pChipData)[4];
    case 4:  ((vuint8*)CanGetRxDataPtr(rxHandle))[3] = ((vuint8*)canRxInfoStruct[channel].pChipData)[3];
    case 3:  ((vuint8*)CanGetRxDataPtr(rxHandle))[2] = ((vuint8*)canRxInfoStruct[channel].pChipData)[2];
    case 2:  ((vuint8*)CanGetRxDataPtr(rxHandle))[1] = ((vuint8*)canRxInfoStruct[channel].pChipData)[1];
    case 1:  ((vuint8*)CanGetRxDataPtr(rxHandle))[0] = ((vuint8*)canRxInfoStruct[channel].pChipData)[0];
    case 0:  break;
    default: break;
    }
    #endif
    /*lint +e616 +e825 fallthrough wanted */
    /* KB end CanLL_RxCopyFromCan */
    #endif
# else
    #if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
    /* KB begin CanLL_RxCopyFromCan */
    /*lint -e616 -e825 fallthrough wanted */
    #if defined(C_COMP_TI_TMS470)
    switch(CanGetRxDataLen(rxHandle))
    {
    case 8:  ((vuint8*)CanGetRxDataPtr(rxHandle))[7] = ((vuint8*)pCanRxInfoStruct->pChipData)[7];
    case 7:  ((vuint8*)CanGetRxDataPtr(rxHandle))[6] = ((vuint8*)pCanRxInfoStruct->pChipData)[6];
    case 6:  ((vuint8*)CanGetRxDataPtr(rxHandle))[5] = ((vuint8*)pCanRxInfoStruct->pChipData)[5];
    case 5:  ((vuint8*)CanGetRxDataPtr(rxHandle))[4] = ((vuint8*)pCanRxInfoStruct->pChipData)[4];
    case 4:  ((vuint8*)CanGetRxDataPtr(rxHandle))[3] = ((vuint8*)pCanRxInfoStruct->pChipData)[3];
    case 3:  ((vuint8*)CanGetRxDataPtr(rxHandle))[2] = ((vuint8*)pCanRxInfoStruct->pChipData)[2];
    case 2:  ((vuint8*)CanGetRxDataPtr(rxHandle))[1] = ((vuint8*)pCanRxInfoStruct->pChipData)[1];
    case 1:  ((vuint8*)CanGetRxDataPtr(rxHandle))[0] = ((vuint8*)pCanRxInfoStruct->pChipData)[0];
    case 0:  break;
    default: break;
    }
    #endif
    /*lint +e616 +e825 fallthrough wanted */
    /* KB end CanLL_RxCopyFromCan */
    #else
    /* KB begin CanLL_RxCopyFromCan */
    /*lint -e616 -e825 fallthrough wanted */
    #if defined(C_COMP_TI_TMS470)
    switch(CanGetRxDataLen(rxHandle))
    {
    case 8:  ((vuint8*)CanGetRxDataPtr(rxHandle))[7] = ((vuint8*)canRxInfoStruct[channel].pChipData)[7];
    case 7:  ((vuint8*)CanGetRxDataPtr(rxHandle))[6] = ((vuint8*)canRxInfoStruct[channel].pChipData)[6];
    case 6:  ((vuint8*)CanGetRxDataPtr(rxHandle))[5] = ((vuint8*)canRxInfoStruct[channel].pChipData)[5];
    case 5:  ((vuint8*)CanGetRxDataPtr(rxHandle))[4] = ((vuint8*)canRxInfoStruct[channel].pChipData)[4];
    case 4:  ((vuint8*)CanGetRxDataPtr(rxHandle))[3] = ((vuint8*)canRxInfoStruct[channel].pChipData)[3];
    case 3:  ((vuint8*)CanGetRxDataPtr(rxHandle))[2] = ((vuint8*)canRxInfoStruct[channel].pChipData)[2];
    case 2:  ((vuint8*)CanGetRxDataPtr(rxHandle))[1] = ((vuint8*)canRxInfoStruct[channel].pChipData)[1];
    case 1:  ((vuint8*)CanGetRxDataPtr(rxHandle))[0] = ((vuint8*)canRxInfoStruct[channel].pChipData)[0];
    case 0:  break;
    default: break;
    }
    #endif
    /*lint +e616 +e825 fallthrough wanted */
    /* KB end CanLL_RxCopyFromCan */
    #endif
# endif
    #if C_SECURITY_LEVEL > 20
    CanSingleGlobalInterruptRestore();
    #endif
  }
  #endif /* ( C_ENABLE_COPY_RX_DATA ) */

  return kCanHlContinueRx;
} /* end of CanReceivceRxHandle() */
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif

# if defined( C_ENABLE_INDICATION_FLAG ) || \
     defined( C_ENABLE_INDICATION_FCT )
/****************************************************************************
| NAME:             CanHL_IndRxHandle
| CALLED BY:        CanBasicCanMsgReceived, CanFullCanMsgReceived
| PRECONDITIONS:
| INPUT PARAMETERS: Handle of received Message to access generated data
| RETURN VALUES:    none
| DESCRIPTION:      DLC-check, Precopy and copy of Data for received message
****************************************************************************/
static void CanHL_IndRxHandle( CanReceiveHandle rxHandle )
{
#  if defined( C_ENABLE_INDICATION_FLAG )
#   if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#   endif
#  endif

#  if defined( C_ENABLE_INDICATION_FLAG )
#   if C_SECURITY_LEVEL > 20
  CanSingleGlobalInterruptDisable();
#   endif
  CanIndicationFlags._c[CanGetIndicationOffset(rxHandle)] |= CanGetIndicationMask(rxHandle);
#   if C_SECURITY_LEVEL > 20
  CanSingleGlobalInterruptRestore();
#   endif
#  endif

#  if defined( C_ENABLE_INDICATION_FCT )
#   if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
#   else
  if ( CanGetApplIndicationPtr(rxHandle) != NULL )
#   endif
  {
    CanGetApplIndicationPtr(rxHandle)(rxHandle);  /* call IndicationRoutine */
  }
#  endif
}
# endif /* C_ENABLE_INDICATION_FLAG || C_ENABLE_INDICATION_FCT  */
#endif


/****************************************************************************
| NAME:             CanHL_TxConfirmation
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: - internal can chip number
|                   - interrupt ID
| RETURN VALUES:    none
| DESCRIPTION:      - full can transmit
****************************************************************************/
#if defined ( MISRA_CHECK )
 /* suppress misra message about multiple return  */
# pragma PRQA_MESSAGES_OFF 2006,2001,2015
#endif
static void CanHL_TxConfirmation( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle)
{
  CanObjectHandle       logTxObjHandle;
  CanTransmitHandle     txHandle;


#if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle   i;
  CanDeclareGlobalInterruptOldStatus
#else
# if defined( C_ENABLE_CONFIRMATION_FLAG )
#  if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#  endif
# endif
#endif


#if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  canuint8 rc;
#endif



  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  /* KB begin CanLL_TxMsgConfBegin */
  {
  #if defined(C_ENABLE_HECC_WORKAROUND_1)
    if(CANTRS == (tCanRegType)0x00000000)
    {
  # if defined(C_DISABLE_RX_FULLCAN_POLLING)  || \
       defined(C_DISABLE_RX_BASICCAN_POLLING) && \
       defined(C_ENABLE_TX_POLLING)
      /* in case of Tx polling mode and Rx interrupt mode the following
         code has to be protected */
      CanSingleGlobalInterruptDisable();
  # endif
      CANTRS = gCANTRS[channel];
      gCANTRS[channel] = (tCanRegType)0x00000000;
  # if defined(C_DISABLE_RX_FULLCAN_POLLING)  || \
       defined(C_DISABLE_RX_BASICCAN_POLLING) && \
       defined(C_ENABLE_TX_POLLING)
      CanSingleGlobalInterruptRestore();
  # endif
    }
  #endif

    CANTA = CanSlotMask[txObjHandle];
  #if defined(C_COMP_TI_TMS470)
  # if defined(C_ENABLE_CANCEL_IN_HW)
    CANAA = CanSlotMask[txObjHandle];
    CANGIF0 = kCanAaif0; /* clear abort acknowledge interrupt */
  # endif
  #endif
  }
  /* KB end CanLL_TxMsgConfBegin */

  txHandle = canHandleCurTxObj[logTxObjHandle];           /* get saved handle */

#if defined ( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel , kErrorDisabledTxMessage);
#endif

#if defined(C_ENABLE_TX_DIRECT_OBJECTS)
  if (CanTxDirectTransmitSupport[logTxObjHandle] == kCanTxDirectObj)
  {
    goto finishCanHL_TxConfirmation;
  }
# endif

  /* check associated transmit handle */
  if (txHandle == kCanBufferFree)
  {
    assertInternal (0, channel, kErrorTxHandleWrong);          /*lint !e506 !e774*/
    goto finishCanHL_TxConfirmation;
  }

#if defined( C_ENABLE_TX_OBSERVE ) || \
    defined ( C_ENABLE_CAN_TX_CONF_FCT )
# if defined(C_ENABLE_CANCEL_IN_HW)
  /* KB begin CanLL_TxIsMsgTransmitted */
  /* CanLL_TxIsMsgTransmitted not used */
  /* KB end CanLL_TxIsMsgTransmitted */
# endif
  {
# if defined( C_ENABLE_TX_OBSERVE )
#  if defined ( C_HL_ENABLE_TX_MSG_DESTROYED )
    if (txHandle != kCanBufferMsgDestroyed)
#  endif
    {
      ApplCanTxObjConfirmed( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
    }
# endif

# if defined ( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
    txInfoStructConf[channel].Handle  = txHandle;
    /* KB begin CanLL_AddTxConfStructMember */
    /* CanLL_AddTxConfStructMember not used */
    /* KB end CanLL_AddTxConfStructMember */
    ApplCanTxConfirmation(&txInfoStructConf[channel]);
# endif
  }
#endif /* defined( C_ENABLE_TX_OBSERVE ) || defined ( C_ENABLE_CAN_TX_CONF_FCT ) */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_TX_FULLCAN_OBJECTS )  || \
     defined( C_ENABLE_MSG_TRANSMIT )
  if (txObjHandle != CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel))
  {
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                 /* free msg object of FullCAN or LowLevel Tx obj. */
  }
# endif
#else
  canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                   /* free msg object if queue is not used */
#endif

#if defined ( C_HL_ENABLE_TX_MSG_DESTROYED )
  if ((txHandle != kCanBufferCancel) && (txHandle != kCanBufferMsgDestroyed))
#else
  if (txHandle != kCanBufferCancel)
#endif
  {
#if defined( C_ENABLE_MSG_TRANSMIT )
    if (txObjHandle == CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel))
    {
#if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
      ApplCanMsgTransmitConf( CAN_CHANNEL_CANPARA_ONLY );
#endif

      goto finishCanHL_TxConfirmation;
    }
#endif

#if defined ( C_ENABLE_CONFIRMATION_FLAG )       /* set transmit ready flag  */
# if C_SECURITY_LEVEL > 20
    CanSingleGlobalInterruptDisable();
# endif
    CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] |= CanGetConfirmationMask(txHandle);
# if C_SECURITY_LEVEL > 20
    CanSingleGlobalInterruptRestore();
# endif
#endif

#if defined( C_ENABLE_CONFIRMATION_FCT )
    {
# if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
# else
      if ( CanGetApplConfirmationPtr(txHandle) != NULL )
# endif
      {
        (CanGetApplConfirmationPtr(txHandle))(txHandle);   /* call completion routine  */
      }
    }
#endif /* C_ENABLE_CONFIRMATION_FCT */

  } /* end if kCanBufferCancel */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_TX_FULLCAN_OBJECTS ) ||\
     defined( C_ENABLE_MSG_TRANSMIT )
  if (txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel))
# endif
  {
    CanSingleGlobalInterruptDisable();                /* ESCAN00008914 */
    if (canTxQueueCnt[channel] != 0)
    {
      CanSingleGlobalInterruptRestore();              /* ESCAN00008914 */
      /* Transmit Queued Objects ( instead of function CanTransmitQueuedObj() */
      /*look for obj ready to transmit*/
      for (i = (CanSignedTxHandle)CAN_HL_TX_STOPINDEX(channel) - (CanSignedTxHandle)1; i >= (CanSignedTxHandle)CAN_HL_TX_STARTINDEX(channel); i--)
      {
        if ((canTxQueueFlags[i]) != 0)
        {
          CanSingleGlobalInterruptDisable();
          if (canTxQueueCnt[channel] != 0) {
            canTxQueueFlags[i] = 0;        /* remove msg from queue */
            canTxQueueCnt[channel]--;
          }
          CanSingleGlobalInterruptRestore();
          canHandleCurTxObj[logTxObjHandle] = (CanTransmitHandle)i;/* Save hdl of msgObj to be transmitted*/
# if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
          rc = CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle,(CanTransmitHandle)i);
          if ( rc == kCanTxNotify)
          {
            APPLCANCANCELNOTIFICATION(channel, (CanTransmitHandle)i);
          }
# else  /* C_ENABLE_CAN_CANCEL_NOTIFICATION */
          (void)CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle,(CanTransmitHandle)i);
# endif  /* C_ENABLE_CAN_CANCEL_NOTIFICATION */

          goto finishCanHL_TxConfirmation;
        }
      }
      CanSingleGlobalInterruptDisable();                /* ESCAN00008914 */
    }
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* free msg object if queue is empty */
    CanSingleGlobalInterruptRestore();                 /* ESCAN00008914 */
  }
#endif
  /* check for next msg object in queue and transmit it */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishCanHL_TxConfirmation:

  /* KB begin CanLL_TxMsgConfEnd */
  /* CanLL_TxMsgConfEnd not used */
  /* KB end CanLL_TxMsgConfEnd */

  return;

} /* END OF CanTxInterrupt */
#if defined ( MISRA_CHECK )
# pragma PRQA_MESSAGES_ON 2006,2001,2015
#endif


#if defined( C_ENABLE_ECU_SWITCH_PASS )
/************************************************************************
* NAME:               CanSetActive
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Set the CAN driver into active mode
*************************************************************************/
C_API_1 void C_API_2 CanSetActive( CAN_CHANNEL_CANTYPE_ONLY )
{
  canPassive[channel] = 0;
} /* END OF CanSetActive */

/************************************************************************
* NAME:               CanSetPassive
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Set the can driver into passive mode
*************************************************************************/
C_API_1 void C_API_2 CanSetPassive( CAN_CHANNEL_CANTYPE_ONLY )
{
  canPassive[channel] = 1;

  #if defined( C_ENABLE_TRANSMIT_QUEUE )
  /* clear all Tx queue flags: */
  CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
  #endif


} /* END OF CanSetPassive */
#endif /* IF defined( C_ENABLE_ECU_SWITCH_PASS ) */


#if defined ( C_ENABLE_OFFLINE )
/************************************************************************
* NAME:               CanOnline( CanChannelHandle channel )
* CALLED BY:          netmanagement
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Switch on transmit path
*************************************************************************/
C_API_1 void C_API_2 CanOnline(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus

  CanSingleGlobalInterruptDisable();
  canStatus[channel] |= kCanTxOn;
  CanSingleGlobalInterruptRestore();
}


/************************************************************************
* NAME:               CanOffline( CanChannelHandle channel )
* CALLED BY:          netmanagement
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Switch off transmit path
*************************************************************************/
C_API_1 void C_API_2 CanOffline(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus

  CanSingleGlobalInterruptDisable();
  canStatus[channel] &= kCanTxNotOn;
  CanSingleGlobalInterruptRestore();

#ifdef C_ENABLE_TRANSMIT_QUEUE
  CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
#endif
}
#endif /* if defined ( C_ENABLE_OFFLINE ) */


#if defined ( C_ENABLE_PART_OFFLINE )
/************************************************************************
* NAME:               CanSetPartOffline
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          (channel), sendGroup
* RETURN VALUE:       none
* DESCRIPTION:        Switch partial off transmit path
*************************************************************************/
C_API_1 void C_API_2 CanSetPartOffline(CAN_CHANNEL_CANTYPE_FIRST canuint8 sendGroup)
{
  CanDeclareGlobalInterruptOldStatus

  CanSingleGlobalInterruptDisable();
  canTxPartOffline[channel] |= sendGroup;   /* set offlinestate and Save for use of CanOnline/CanOffline */
  CanSingleGlobalInterruptRestore();
}


/************************************************************************
* NAME:               CanSetPartOnline
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          (channel), invSendGroup
* RETURN VALUE:       none
* DESCRIPTION:        Switch partial on transmit path
*************************************************************************/
C_API_1 void C_API_2 CanSetPartOnline(CAN_CHANNEL_CANTYPE_FIRST canuint8 invSendGroup)
{
  CanDeclareGlobalInterruptOldStatus

  CanSingleGlobalInterruptDisable();
  canTxPartOffline[channel] &= invSendGroup;
  CanSingleGlobalInterruptRestore();
}


/************************************************************************
* NAME:               CanGetPartMode
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       canTxPartOffline
* DESCRIPTION:        return status of partoffline-Mode
*************************************************************************/
C_API_1 canuint8 C_API_2 CanGetPartMode(CAN_CHANNEL_CANTYPE_ONLY)
{
  return canTxPartOffline[channel];
}
#endif


/****************************************************************************
| NAME:             CanGetStatus
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanTxOff
|                   kCanTxOn
| DESCRIPTION:      returns the status of the transmit path and the CAN hardware.
|                   Only one of the statusbits Sleep,Busoff,Passiv,Warning is set.
|                   Sleep has the highest priority, error warning the lowerst.
****************************************************************************/
#if defined ( MISRA_CHECK )
 /* suppress misra message about multiple return  */
# pragma PRQA_MESSAGES_OFF 2006
#endif
C_API_1 canuint8 C_API_2 CanGetStatus( CAN_CHANNEL_CANTYPE_ONLY )
{

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

#if defined( C_ENABLE_EXTENDED_STATUS )
  /* KB begin CanLL_GetStatusBegin */
  /* Update content of CANES register */
  CANES = (CANES & kCanErrorMask);
  /* KB end CanLL_GetStatusBegin */
# if defined( C_ENABLE_SLEEP_WAKEUP )
  /***************************** verify Sleep mode *************************************/
  if ( CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY)   )  { return ( canStatus[channel] | kCanHwIsSleep ); }

# endif

  /***************************** verify Stop mode *************************************/
  if ( CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY)    )  { return ( canStatus[channel] | kCanHwIsStop ); }

  /***************************** verify Busoff *************************************/
  if ( CanLL_HwIsBusOff(CAN_HW_CHANNEL_CANPARA_ONLY)  )  { return ( canStatus[channel] | kCanHwIsBusOff ); }

  /***************************** verify Error Passiv *******************************/
  {
    if ( CanLL_HwIsPassive(CAN_HW_CHANNEL_CANPARA_ONLY) )  { return ( canStatus[channel] | kCanHwIsPassive ); }
  }

  /***************************** verify Error Warning ******************************/
  {
    if ( CanLL_HwIsWarning(CAN_HW_CHANNEL_CANPARA_ONLY) )  { return ( canStatus[channel] | kCanHwIsWarning ); }
  }
#endif
  return (canStatus[channel] & kCanTxOn);

} /* END OF CanGetStatus */
#if defined ( MISRA_CHECK )
# pragma PRQA_MESSAGES_ON 2006
#endif


/****************************************************************************
| NAME:             CanSleep
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanOk, if CanSleep was successfull
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      disable CAN
****************************************************************************/
C_API_1 canuint8 C_API_2 CanSleep(CAN_CHANNEL_CANTYPE_ONLY)
{
#if defined( C_ENABLE_SLEEP_WAKEUP )
  canuint8         canReturnCode;
#endif

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
  /* this has to be done, even if SLEEP_WAKEUP is not enabled */
  canMsgCondRecState[channel] = kCanTrue;
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )

  assertInternal((canStatus[channel] & kCanTxOn) == kCanTxOff, channel, kErrorCanOnline);
  assertInternal((canStatus[channel] & kCanHwIsStop) != kCanHwIsStop, channel, kErrorCanStop);

  {
    /* KB begin CanLL_Sleep */
    {
      tCanRegType tmpCanMC;


      tmpCanMC = CANMC | kCanPdr | kCanWuba;
      CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, tmpCanMC); /* set power down request bit */


      APPLCANTIMERSTART(kCanLoopWaitPowerDownMode);
      while(((CANES & kCanPda) != kCanPda) && ((CANMC & kCanPdr) == kCanPdr))
      {
    # if defined(C_ENABLE_HW_LOOP_TIMER)
        if(0 == APPLCANTIMERLOOP(kCanLoopWaitPowerDownMode))
        {
          break;
        }
    # else
        ; /* wait until CAN controller allows configuration access */
    # endif
      }
      APPLCANTIMEREND(kCanLoopWaitPowerDownMode);

      if((CANES & kCanPda) != kCanPda)
      {
        canReturnCode = kCanFailed;
      }
      else
      {
        canReturnCode = kCanOk;
      }
    }
    /* KB end CanLL_Sleep */
  }
  return canReturnCode;
#else
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL) && \
     defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;
# endif
  return kCanNotSupported;
#endif
} /* END OF CanSleep */

/****************************************************************************
| NAME:             CanWakeUp
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanOk, if CanWakeUp was successfull
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      enable CAN
****************************************************************************/
C_API_1 canuint8 C_API_2 CanWakeUp( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_ENABLE_SLEEP_WAKEUP )
  canuint8         canReturnCode;


#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  {
    /* KB begin CanLL_WakeUp */
    tCanRegType tmpCanMC;


    tmpCanMC = CANMC & ~kCanPdr;
    CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, tmpCanMC); /* clear power down request bit */


    APPLCANTIMERSTART(kCanLoopWaitLeavePowerDownMode);
    while((CANES & kCanPda) == kCanPda)
    {
    #if defined(C_ENABLE_HW_LOOP_TIMER)
      if(0 == APPLCANTIMERLOOP(kCanLoopWaitLeavePowerDownMode))
      {
        break;
      }
    #else
      ; /* wait until CAN controller allows configuration access */
    #endif
    }
    APPLCANTIMEREND(kCanLoopWaitLeavePowerDownMode);

    if((CANES & kCanPda) == kCanPda)
    {
      canReturnCode = kCanFailed;
    }
    else
    {
      canReturnCode = kCanOk;
    }
    /* KB end CanLL_WakeUp */
  }
  return canReturnCode;
#else
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL) && \
     defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;
# endif
  return kCanNotSupported;
#endif /* C_ENABLE_SLEEP_WAKEUP */
} /* END OF CanWakeUp */


#if defined ( C_ENABLE_STOP )
/****************************************************************************
| NAME:             CanStop
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    kCanOk, if success
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      stop CAN-controller
****************************************************************************/
C_API_1 canuint8 C_API_2 CanStop( CAN_CHANNEL_CANTYPE_ONLY )
{
  canuint8         canReturnCode;

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  {
    /* KB begin CanLL_Stop */
    tCanRegType tmpCanMC;
    CanDeclareGlobalInterruptOldStatus;
    canReturnCode = kCanOk;
    /* disconnect the CAN controller by switching into initmode */
    tmpCanMC = kCanCcr;
    /* set change configuration request */
    CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, tmpCanMC);   /*lint !e740 !e826 unusual pointer cast */

    APPLCANTIMERSTART(kCanLoopWaitCanStop);
    while((CANES & kCanCce) != kCanCce)
    {
    #if defined(C_ENABLE_HW_LOOP_TIMER)
      if(0 == APPLCANTIMERLOOP(kCanLoopWaitCanStop))
      {
        canReturnCode = kCanFailed;
        break; /* loop is terminated by user */
      }
      else
    #endif
      {
        ; /* wait until CAN controller allows configuration access */
      }
    }
    APPLCANTIMEREND(kCanLoopWaitCanStop);

    CanSingleGlobalInterruptDisable();
    if(canReturnCode == kCanOk)
    {
      canStatus[channel] |= kCanHwIsStop;
    }
    CanSingleGlobalInterruptRestore();
    /* KB end CanLL_Stop */
  }
  return canReturnCode;
}

/****************************************************************************
| NAME:             CanStart
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    kCanOk, if success
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      restart CAN-controller
****************************************************************************/
C_API_1 canuint8 C_API_2 CanStart( CAN_CHANNEL_CANTYPE_ONLY )
{
  canuint8         canReturnCode;

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  {
    /* KB begin CanLL_Start */
    tCanRegType tmpCanMC;
    CanDeclareGlobalInterruptOldStatus;
    canReturnCode = kCanOk;
    tmpCanMC = CANMC;
    tmpCanMC &= ~kCanCcr;  /* clear configuration change request bit */
    CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, tmpCanMC); /*lint !e740 !e826 unusual pointer cast */

    APPLCANTIMERSTART(kCanLoopWaitCanStart);
    while((CANES & kCanCce) == kCanCce)
    {
    #if defined(C_ENABLE_HW_LOOP_TIMER)
      if(0 == APPLCANTIMERLOOP(kCanLoopWaitCanStart))
      {
        canReturnCode = kCanFailed;
        break; /* loop is terminated by user */
      }
      else
    #endif
      {
        ; /* wait until CAN controller leaves configuration mode */
      }
    }
    APPLCANTIMEREND(kCanLoopWaitCanStart);

    /* stopmode clears the SCC/HECC configuration bit !! -> set to the correct value */
    #if defined(C_COMP_TI_TMS470)
    # if defined(C_SINGLE_RECEIVE_CHANNEL)
    #  if(kCanControllerMode == kCanSCCMode)
    tmpCanMC &= ~kCanScm;   /* activate SCC mode */
    #  else
    tmpCanMC |= kCanScm;   /* activate HECC mode */
    #  endif
    # else
    if(CanControllerMode[channel] == kCanSCCMode)
    {
      tmpCanMC &= ~kCanScm;   /* activate SCC mode */
    }
    else
    {
      tmpCanMC |= kCanScm;   /* activate HECC mode */
    }
    # endif
    #endif
    CAN_WRITE_REGISTER(&CANMC, (tCanRegType)0xFFFFFFFF, tmpCanMC); /*lint !e740 !e826 unusual pointer cast */

    CanSingleGlobalInterruptDisable();
    if(canReturnCode == kCanOk)
    {
      canStatus[channel] &= ~kCanHwIsStop;
    }
    CanSingleGlobalInterruptRestore();
    /* KB end CanLL_Start */
  }
  return canReturnCode;
}
#endif /* if defined ( C_ENABLE_STOP ) */


/****************************************************************************
| NAME:             CanGlobalInterruptDisable
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      disables global interrupts and stores old interrupt status
****************************************************************************/
C_API_1 void C_API_2 CanGlobalInterruptDisable(void) C_API_3
{
#if defined( C_ENABLE_INTCTRL_BY_APPL )
#else
  CanDeclareGlobalInterruptOldStatus
#endif

  /* local variable must reside on stack or registerbank, switched */
  /* in interrupt level                                            */
  /* disable global interrupt                                      */
  assertUser(canGlobalInterruptCounter<(cansint8)0x7f, kCanAllChannels, kErrorIntDisableTooOften);

  if (canGlobalInterruptCounter == (cansint8)0)  /* if 0 then save old interrupt status */
  {
#if defined( C_ENABLE_INTCTRL_BY_APPL )
    ApplCanInterruptDisable();
#else
    CanNestedGlobalInterruptDisable();
    CanGetGlobalInterruptOldStatus( canInterruptOldStatus );            /*lint !e530 */
#endif
  }

  canGlobalInterruptCounter++;               /* common for all platforms */

} /* END OF CanGlobalInterruptDisable */

/****************************************************************************
| NAME:             CanGlobalInterruptRestore
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      restores the old interrupt status if canGlobalInterruptCounter is zero
****************************************************************************/
C_API_1 void C_API_2 CanGlobalInterruptRestore(void)  C_API_3
{
# if defined( C_ENABLE_INTCTRL_BY_APPL )
# else
  CanDeclareGlobalInterruptOldStatus
# endif
  assertUser(canGlobalInterruptCounter>(cansint8)0, kCanAllChannels, kErrorIntRestoreTooOften);

  /* restore global interrupt */
  canGlobalInterruptCounter--;

  if (canGlobalInterruptCounter == (cansint8)0)         /* restore interrupt if canGlobalInterruptCounter=0 */
  {
#if defined( C_ENABLE_INTCTRL_BY_APPL )
    ApplCanInterruptRestore();
#else
    CanPutGlobalInterruptOldStatus( canInterruptOldStatus );
    CanNestedGlobalInterruptRestore();
#endif
  }

} /* END OF CanGlobalInterruptRestore */

/****************************************************************************
| NAME:             CanCanInterruptDisable
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      disables CAN interrupts and stores old interrupt status
****************************************************************************/
C_API_1 void C_API_2 CanCanInterruptDisable( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
#if defined(C_HL_ENABLE_CAN_IRQ_DISABLE)
  tCanLLCanIntOld localInterruptOldFlag;
#endif

  /* local variable must reside on stack or registerbank, switched */
  /* in interrupt level                                            */
  /* disable global interrupt                                      */
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
  assertUser(canCanInterruptCounter[channel]<(cansint8)0x7f, kCanAllChannels, kErrorIntDisableTooOften);
#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  CanNestedGlobalInterruptDisable();
  if (canCanInterruptCounter[channel] == (CANSINTX)0)  /* if 0 then save old interrupt status */
  {
#if defined(C_HL_ENABLE_CAN_IRQ_DISABLE)
    {
      CanLL_CanInterruptDisable(canHwChannel, &localInterruptOldFlag);
      canCanInterruptOldStatus[canHwChannel] = localInterruptOldFlag;            /*lint !e530 */
    }
#endif
#if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
    ApplCanAddCanInterruptDisable(channel);
#endif
  }
  canCanInterruptCounter[channel]++;               /* common for all platforms */

  CanNestedGlobalInterruptRestore();

} /* END OF CanCanInterruptDisable */

/****************************************************************************
| NAME:             CanCanInterruptRestore
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      restores the old interrupt status of the CAN interrupt if
|                   canCanInterruptCounter[channel] is zero
****************************************************************************/
C_API_1 void C_API_2 CanCanInterruptRestore( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
  assertUser(canCanInterruptCounter[channel]>(CANSINTX)0, kCanAllChannels, kErrorIntRestoreTooOften);
#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  CanNestedGlobalInterruptDisable();
  /* restore CAN interrupt */
  canCanInterruptCounter[channel]--;

  if (canCanInterruptCounter[channel] == (CANSINTX)0)         /* restore interrupt if canGlobalInterruptCounter=0 */
  {
#if defined(C_HL_ENABLE_CAN_IRQ_DISABLE)
    {
      CanLL_CanInterruptRestore(canHwChannel, canCanInterruptOldStatus[canHwChannel]);
    }
#endif

#if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
    ApplCanAddCanInterruptRestore(channel);
#endif
  }
  CanNestedGlobalInterruptRestore();
} /* END OF CanCanInterruptRestore */

#ifdef C_ENABLE_MSG_TRANSMIT
/************************************************************************
* NAME:               CanMsgTransmit
* CALLED BY:          CanReceivedFunction
* PRECONDITIONS:      Called in Receive Interrupt
* PARAMETER:          Pointer to message buffer data block; This can either be
*                     a RAM structure data block or the receive buffer in the
*                     CAN chip
* RETURN VALUE:       The return value says that a transmit request was successful
*                     or not
* DESCRIPTION:        Transmit functions for gateway issues (with dynamic
*                     messages). If the transmit buffer is not free, the message
*                     is inserted in the FIFO ring buffer.
*************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006,2015
# endif
/* Msg(4:3673) The object addressed by the pointer "txMsgStruct" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
C_API_1 canuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST tCanMsgTransmitStruct *txMsgStruct )
{
  CanDeclareGlobalInterruptOldStatus
  canuint8                 rc;
  CanObjectHandle          txObjHandle;
  CanObjectHandle          logTxObjHandle;

  /* KB begin CanLL_TxTransmissionLocalParameter */
  tCanRegType   tmpCanME;
  #if defined(C_ENABLE_PRETRANSMIT_FCT) || defined(C_ENABLE_COPY_TX_DATA)
  vuint32   tmpTxBuf[2];
  #endif
  #if defined(C_CLIENT_VECTOR)
  # if defined(C_COMP_TI_TMS470)
  #  if(kCanControllerMode == kCanSCCMode)
  while(CANES & kCanRm)
  {
    ; /* while CAN cell is receiving a message */
  }
  #  endif
  # endif
  #endif
  /* KB end CanLL_TxTransmissionLocalParameter */

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif


  CanNestedGlobalInterruptDisable();

  /* --- test on CAN transmit switch --- */
  if ( (canStatus[channel] & kCanTxOn) == kCanTxOff )                /* transmit path switched off */
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

#if defined(C_ENABLE_CAN_RAM_CHECK)
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    CanNestedGlobalInterruptRestore();
    return(kCanCommunicationDisabled);
  }
#endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
  assertInternal(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
# endif
  assertInternal((canStatus[channel] & kCanHwIsStop) != kCanHwIsStop, channel, kErrorCanStop);

  /* --- check on passive state --- */
  #if defined( C_ENABLE_ECU_SWITCH_PASS )
  if ( canPassive[channel] != 0)                             /*  set passive ? */
  {
    CanNestedGlobalInterruptRestore();
    #if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
    ApplCanMsgTransmitConf( CAN_CHANNEL_CANPARA_ONLY );
    #endif
    return (kCanTxOk);
  }
  #endif /* C_ENABLE_ECU_SWITCH_PASS */

  /* calculate index for canhandleCurTxObj (logical object handle) */
  logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  /* check for transmit message object free ---------------------------------*/
  /* MsgObj used?  */
  if (( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree ))
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

  /* Obj, pMsgObject points to is free, transmit msg object: ----------------*/
  /* Save hdl of msgObj to be transmitted*/
  canHandleCurTxObj[logTxObjHandle] = kCanBufferMsgTransmit;
  CanNestedGlobalInterruptRestore();



  assertHardware( CanLL_TxIsHWObjFree(canHwChannel, CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) ), channel, kErrorTxBufferBusy);

  txObjHandle = CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel);
  /* KB begin CanLL_TxBegin */
  tmpCanME = CANME & (tCanRegType)~CanSlotMask[txObjHandle];  /* disable mailbox to get write access */
  CANME    = tmpCanME;
  /* KB end CanLL_TxBegin */

  CanNestedGlobalInterruptDisable();
  /* Copy all data into transmit object */


  /* If CanTransmit was interrupted by a re-initialization or CanOffline */
  /* no transmitrequest of this action should be started      */
  if ((canHandleCurTxObj[logTxObjHandle] == kCanBufferMsgTransmit) &&
                                   ( (canStatus[channel] & kCanTxOn) != kCanTxOff ) )
  {
     /* KB begin CanLL_TxCopyMsgTransmit */
     # if defined(C_ENABLE_HECC_WORKAROUND_2)
     CanLockGlobalInterrupt();
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).reserved2  = 0;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).reserved2  = 0;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).IdRaw      = (*(tCanMsgTransmitStruct*)txMsgStruct).IdRaw;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).IdRaw      = (*(tCanMsgTransmitStruct*)txMsgStruct).IdRaw;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).DlcRaw     = (*(tCanMsgTransmitStruct*)txMsgStruct).DlcRaw;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).DlcRaw     = (*(tCanMsgTransmitStruct*)txMsgStruct).DlcRaw;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).DataFld[0] = (*(tCanMsgTransmitStruct*)txMsgStruct).DataFld[0];
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).DataFld[0] = (*(tCanMsgTransmitStruct*)txMsgStruct).DataFld[0];
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).DataFld[1] = (*(tCanMsgTransmitStruct*)txMsgStruct).DataFld[1];
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))).DataFld[1] = (*(tCanMsgTransmitStruct*)txMsgStruct).DataFld[1];
     CanUnlockGlobalInterrupt();
     # else
     txMsgStruct->reserved2 = 0;
     (*(tCanMsgTransmitStruct*)(&CANMSGOBJ(txObjHandle))) = *txMsgStruct;
     # endif
     /* tmpTxBuf is used instead of tmpCanME in order to get rid of a compiler warning */
     tmpTxBuf[0] = CANME | CanSlotMask[txObjHandle];  /* enable mailbox again */
     CANME    = tmpTxBuf[0];
     /* KB end CanLL_TxCopyMsgTransmit */

     /* KB begin CanLL_TxStartMsgTransmit */
     #if defined(C_ENABLE_HECC_WORKAROUND_1)
     if(((CANES & kCanTm) != 0) && (CANTRS != 0)) /* check if hardware is not busy with Tx */
     {
       gCANTRS[channel] |= CanSlotMask[txObjHandle];  /* set Tx request flag */
     }
     else
     #endif
     {
       CANTRS = CanSlotMask[txObjHandle];  /* set Tx request flag */
     }
     /* KB end CanLL_TxStartMsgTransmit */

     #if defined( C_ENABLE_TX_OBSERVE )
     ApplCanTxObjStart( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
     #endif

     rc = kCanTxOk;
  }
  else
  {
    /* release TxHandle (CanOffline) */
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;
    rc = kCanTxFailed;
  }

  CanNestedGlobalInterruptRestore();

  /* KB begin CanLL_TxEnd */
  tmpCanME  = CANME | CanSlotMask[txObjHandle];  /* enable mailbox again */
  CANME     = tmpCanME;
  /* KB end CanLL_TxEnd */

  return rc;
} /*end of CanMsgTransmit() */
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006,2015
# endif
#endif


#if defined( C_ENABLE_DYN_TX_OBJECTS )
/************************************************************************
* NAME:           CanGetDynTxObj
* PARAMETER:      txHandle - Handle of the dynamic object to reserve
* RETURN VALUE:   kCanNoTxDynObjAvailable (0xFF) -
*                   object not available
*                 0..F0 -
*                   Handle to dynamic transmission object
* DESCRIPTION:    Function reserves and return a handle to a dynamic
*                   transmission object
*
*                 To use dynamic transmission, an application must get
*                 a dynamic object from CAN-driver.
*                 Before transmission, application must set all attributes
*                 (id, dlc, data, confirmation function/flag, pretransmission
*                 etc. - as configurated).
*                 Application can use a dynamic object for one or many
*                 transmissions (as it likes) - but finally, it must
*                 release the dynamic object by calling CanReleaseDynTxObj.
*************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006
# endif
C_API_1 CanTransmitHandle C_API_2 CanGetDynTxObj(CanTransmitHandle txHandle )
{
  CanTransmitHandle nTxDynObj;
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL

  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
  #endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);

  nTxDynObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

  CanSingleGlobalInterruptDisable();
  if ( canTxDynObjReservedFlag[nTxDynObj] != 0)
  {
    CanSingleGlobalInterruptRestore();
    return kCanNoTxDynObjAvailable;
  }
  /*  Mark dynamic object as used  */
  canTxDynObjReservedFlag[nTxDynObj] = 1;

  #if defined( C_ENABLE_CONFIRMATION_FLAG )
  CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] &=
                            (canuint8)(~CanGetConfirmationMask(txHandle));
  #endif
  CanSingleGlobalInterruptRestore();

  /* Initialize dynamic object */
  #if defined(  C_ENABLE_DYN_TX_DATAPTR )
  canDynTxDataPtr[nTxDynObj] = NULL;
  #endif


  return (txHandle);
}
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif

/************************************************************************
* NAME:           CanReleaseDynTxObj
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
* RETURN VALUE:   --
* DESCRIPTION:    Function releases dynamic transmission object
*                   which was reserved before (calling CanGetDynTxObj)
*
*                 After a transmission of one or more messages is finished,
*                 application must free the reserved resource, formally the
*                 dynamic transmission object calling this function.
*
*                 As the number of dynamic transmission object is limited,
*                 application should not keep unused dynamic transmission
*                 objects for a longer time.
*************************************************************************/
# if defined ( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006
# endif
C_API_1 canuint8 C_API_2 CanReleaseDynTxObj(CanTransmitHandle txHandle)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL

  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
  #endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

  assertInternal((canTxDynObjReservedFlag[dynTxObj] != 0), channel, kErrorReleasedUnusedDynObj);

#if defined( C_ENABLE_TRANSMIT_QUEUE )
  if ((canTxQueueFlags[txHandle]) == 0)      /*lint !e661 !e662*/
#endif
  {
    if (
#if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
         (confirmHandle[channel] == txHandle) ||       /* confirmation active ? */
#endif
         (canHandleCurTxObj[CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] != txHandle) )
    {
      /*  Mark dynamic object as not used  */
      canTxDynObjReservedFlag[dynTxObj] = 0;
      return(kCanDynReleased);
    }
  }
  return(kCanDynNotReleased);
}
# if defined ( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif
#endif /* C_ENABLE_DYN_TX_OBJECTS */


#if defined( C_ENABLE_DYN_TX_ID )
# if !defined( C_ENABLE_EXTENDED_ID ) ||\
     defined ( C_ENABLE_MIXED_ID )
/************************************************************************
* NAME:           CanDynTxObjSetId
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 id -
*                   Id (standard-format) to register with dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Function registers submitted id (standard format)
*                 with dynamic object referenced by handle.
*************************************************************************/
C_API_1 void C_API_2 CanDynTxObjSetId(CanTransmitHandle txHandle, canuint16 id)
{
  CanTransmitHandle dynTxObj;

  CAN_CHANNEL_CANTYPE_LOCAL

  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
#  endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

#  if defined( C_ENABLE_MIXED_ID )
#   if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#   else
  canDynTxIdType[dynTxObj]  = kCanIdTypeStd;
#   endif
#  endif

  canDynTxId0[dynTxObj] = MK_STDID0(id);
#  if (kCanNumberOfUsedCanTxIdTables > 1)
  canDynTxId1[dynTxObj] = MK_STDID1(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 2)
  canDynTxId2[dynTxObj] = MK_STDID2(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 3)
  canDynTxId3[dynTxObj] = MK_STDID3(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 4)
  canDynTxId4[dynTxObj] = MK_STDID4(id);
#  endif
}
# endif /* !defined( C_ENABLE_EXTENDED_ID ) || defined ( C_ENABLE_MIXED_ID ) */
#endif /* C_ENABLE_DYN_TX_ID */

#if defined( C_ENABLE_DYN_TX_ID ) && \
    defined( C_ENABLE_EXTENDED_ID )
/************************************************************************
* NAME:           CanDynTxObjSetExtId
* PARAMETER:      hTxObj -  Handle of dynamic transmission object
*                 idExtHi - Id low word (extended-format) to register with
*                                                         dynamic object
*                 idExtLo - Id high word (extended-format)
* RETURN VALUE:   --
* DESCRIPTION:    Function registers submitted id (standard format)
*                 with dynamic object referenced by handle.
*************************************************************************/
C_API_1 void C_API_2 CanDynTxObjSetExtId(CanTransmitHandle txHandle, canuint16 idExtHi, canuint16 idExtLo)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL

  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

# if defined( C_ENABLE_MIXED_ID )
#  if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#  else
  canDynTxIdType[dynTxObj] = kCanIdTypeExt;
#  endif
# endif

  canDynTxId0[dynTxObj]      = MK_EXTID0( ((canuint32)idExtHi<<16) | idExtLo );
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canDynTxId1[dynTxObj]      = MK_EXTID1( ((canuint32)idExtHi<<16) | idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canDynTxId2[dynTxObj]      = MK_EXTID2( ((canuint32)idExtHi<<16) | idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canDynTxId3[dynTxObj]      = MK_EXTID3( ((canuint32)idExtHi<<16) | idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canDynTxId4[dynTxObj]      = MK_EXTID4( ((canuint32)idExtHi<<16) | idExtLo );
# endif
}
#endif


#if defined( C_ENABLE_DYN_TX_DLC )
/************************************************************************
* NAME:           CanDynTxObjSetDlc
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 dlc -
*                   data length code to register with dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Function registers data length code with
*                 dynamic object referenced by submitted handle.
*************************************************************************/
C_API_1 void C_API_2 CanDynTxObjSetDlc(CanTransmitHandle txHandle, canuint8 dlc)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL

  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

# if defined(C_ENABLE_EXTENDED_ID)
  canDynTxDLC[dynTxObj] = MK_TX_DLC_EXT(dlc);
# else
  canDynTxDLC[dynTxObj] = MK_TX_DLC(dlc);
# endif
}

#endif /* C_ENABLE_DYN_TX_DLC */


#if defined( C_ENABLE_DYN_TX_DATAPTR )
/************************************************************************
* NAME:           CanDynTxObjSetData
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 pData -
*                   data reference to be stored in data buffer of dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Functions stores reference to data registered with
*                 dynamic object.
*
*                 The number of byte copied is (always) 8. The number of
*                 relevant (and consequently evaluated) byte is to be
*                 taken from function CanDynObjGetDLC.
*************************************************************************/
C_API_1 void C_API_2 CanDynTxObjSetDataPtr(CanTransmitHandle txHandle, void* pData)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL

  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
  #endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

  canDynTxDataPtr[dynTxObj] = pData;
}

#endif /* C_ENABLE_DYN_TX_DATAPTR */




#if defined(C_ENABLE_TX_DIRECT_OBJECTS)
/****************************************************************************
| NAME:             CanDirectTransmit
| CALLED BY:        application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle of the transmit object to be send
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      Starts Transmission of the direct handled TX mailbox
|                   If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
C_API_1 canuint8 C_API_2 CanDirectTransmit(CanTransmitHandle txHandle)   /*lint !e14 !e31*/
{
  CanObjectHandle      txObjHandle;
# if defined( C_ENABLE_USER_CHECK )
  CanObjectHandle      logTxObjHandle;
# endif
  CAN_CHANNEL_CANTYPE_LOCAL

  /* KB begin CanLL_DirectTransmitLocalParameter */
  /* CanLL_DirectTransmitLocalParameter not used */
  /* KB end CanLL_DirectTransmitLocalParameter */

  txObjHandle = CanGetTxHwObj(txHandle);
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
# endif

# if defined( C_ENABLE_USER_CHECK )
  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
  assertUser (CanTxDirectTransmitSupport[logTxObjHandle] == kCanTxDirectObj, channel, kErrorIllDirectTxHandle);
#  if defined ( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel , kErrorDisabledTxMessage);
#  endif
# endif


# if defined(C_ENABLE_CAN_RAM_CHECK)
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    return(kCanCommunicationDisabled);
  }
# endif

  /* transmit path switched on and Hardware object free ? */
  if ( ( (canStatus[channel] & kCanTxOn) == kCanTxOn ) &&
       CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) )
  {
    /* start transmission */
    /* KB begin CanLL_TxDirectStart */
    #if defined(C_ENABLE_HECC_WORKAROUND_1)
    if(((CANES & kCanTm) != 0) && (CANTRS != 0)) /* check if hardware is not busy with Tx */
    {
      gCANTRS[channel] |= CanSlotMask[txObjHandle];  /* set Tx request flag */
    }
    else
    #endif
    {
      CANTRS = CanSlotMask[txObjHandle];  /* set Tx request flag */
    }
    /* KB end CanLL_TxDirectStart */

    return(kCanTxOk);
  }
  else
  {
    return(kCanTxFailed);
  }
}

/************************************************************************
| NAME:             CanDirectTxCopyData
| CALLED BY:        application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: txHandle: Handle of the transmit object to be send,
|                   canMemCopySrcPtr:  Pointer to data which have to be copied
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      copies the transmit data into the direct handled TX mailbox
|                   If the CAN driver and controller is not ready, the application
|                   decide, whether the copy request is repeated or not.
*************************************************************************/
C_API_1 canuint8 C_API_2 CanDirectTxCopyData(CanTransmitHandle txHandle, TxDataPtr *canMemCopySrcPtr)
{
  CanObjectHandle      txObjHandle;
# if defined( C_ENABLE_USER_CHECK )
  CanObjectHandle      logTxObjHandle;
# endif
  CAN_CHANNEL_CANTYPE_LOCAL

  txObjHandle = CanGetTxHwObj(txHandle);
  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
  #endif

# if defined( C_ENABLE_USER_CHECK )
  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
  assertUser ( CanTxDirectTransmitSupport[logTxObjHandle] == kCanTxDirectObj, channel, kErrorIllDirectTxHandle);
  assertUser ( canMemCopySrcPtr != NULL, channel, kErrorNoDataBuffer);
#  if defined ( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel , kErrorDisabledTxMessage);
#  endif
# endif

  if ( CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) )
  {
    /* copy data (8 Byte) */
    /* KB begin CanLL_TxDirectCopyData */
    #if defined(C_ENABLE_HECC_WORKAROUND_2)
    CanLockGlobalInterrupt();
    CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = ((vuint32*)canMemCopySrcPtr)[0];
    CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = ((vuint32*)canMemCopySrcPtr)[0];
    CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = ((vuint32*)canMemCopySrcPtr)[1];
    CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = ((vuint32*)canMemCopySrcPtr)[1];
    CanUnlockGlobalInterrupt();
    #else
    CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdl = ((vuint32*)canMemCopySrcPtr)[0];
    CANMSGOBJ(txObjHandle).CanMsg.dwords.CanMdh = ((vuint32*)canMemCopySrcPtr)[1];
    #endif
    /* KB end CanLL_TxDirectCopyData */

    return(kCanTxOk);
  }
  else
  {
    return(kCanTxFailed);
  }
}

/************************************************************************
| NAME:             CanDirectTxGetStatus
| CALLED BY:        application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: txHandle: Handle of the transmit object to be send,
| RETURN VALUES:    kCanTxFree   : transmit object is free
|                   kCanTxAck    : transmit was succesful
|                   kCanTxPending: transmission is pending
| DESCRIPTION:      shows the status of a direct transmit object.
*************************************************************************/
C_API_1 canuint8 C_API_2 CanDirectTxGetStatus(CanTransmitHandle txHandle)
{
  CanObjectHandle      txObjHandle;
# if defined( C_ENABLE_USER_CHECK )
  CanObjectHandle      logTxObjHandle;
# endif
  CAN_CHANNEL_CANTYPE_LOCAL

  /* KB begin CanLL_DirectTxGetStatusLocalParameter */
  /* CanLL_DirectTxGetStatusLocalParameter not used */
  /* KB end CanLL_DirectTxGetStatusLocalParameter */

  txObjHandle = CanGetTxHwObj(txHandle);
  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
  channel = CanGetChannelOfTxObj(txHandle);
  #endif

# if defined( C_ENABLE_USER_CHECK )
  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
  assertUser ( CanTxDirectTransmitSupport[logTxObjHandle] == kCanTxDirectObj, channel, kErrorIllDirectTxHandle);
#  if defined ( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel , kErrorDisabledTxMessage);
#  endif
# endif

  /* KB begin CanLL_If_TxIsAcknoleged */
  if((CANTA & CanSlotMask[txObjHandle]) != (tCanRegType)0x00000000)
  {
    CANTA = CanSlotMask[txObjHandle]; /* acknowledge this transmission */
  /* KB end CanLL_If_TxIsAcknoleged */
    return (kCanTxAck);
  }
  else
  {
    /* KB begin CanLL_If_TxIsPending */
    #if defined(C_ENABLE_HECC_WORKAROUND_1)
    if((((CANTRS & CanSlotMask[txObjHandle]) != (tCanRegType)0x00000000)           ||  /* request set */
       ((gCANTRS[channel] & CanSlotMask[txObjHandle]) != (tCanRegType)0x00000000)) &&  /* request set in shadow */
       ((CANTA & CanSlotMask[txObjHandle]) == (tCanRegType)0x00000000))                /* ackwnowledge pending */
    #else
    if(((CANTRS & CanSlotMask[txObjHandle]) != (tCanRegType)0x00000000) ||  /* request set */
       ((CANTA & CanSlotMask[txObjHandle]) == (tCanRegType)0x00000000))     /* ackwnowledge pending */
    #endif
    {
    /* KB end CanLL_If_TxIsPending */
      return (kCanTxPending);
    }
    else
    {
      assertHardware( CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ), channel, kErrorTxBufferBusy);
      return (kCanTxFree);
    }
  }
}
#endif  /* defined(C_ENABLE_TX_DIRECT_OBJECTS) */

#if defined (C_ENABLE_TX_MASK_EXT_ID)
/************************************************************************
* NAME:               CanSetTxIdExtHi
* CALLED BY:
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
C_API_1 void C_API_2 CanSetTxIdExtHi( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask )
{
  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0x00FFFFFFUL)) | MK_EXTID0((canuint32)mask<<24);
  #if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0x00FFFFFFUL)) | MK_EXTID1((canuint32)mask<<24);
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0x00FFFFFFUL)) | MK_EXTID2((canuint32)mask<<24);
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0x00FFFFFFUL)) | MK_EXTID3((canuint32)mask<<24);
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0x00FFFFFFUL)) | MK_EXTID4((canuint32)mask<<24);
  #endif
}

/************************************************************************
* NAME:               CanSetTxIdExtMidHi
* CALLED BY:
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
C_API_1 void C_API_2 CanSetTxIdExtMidHi( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask )
{
  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFF00FFFFUL)) | MK_EXTID0((canuint32)mask<<16);   /*lint !e572*/
  #if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFF00FFFFUL)) | MK_EXTID1((canuint32)mask<<16);   /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFF00FFFFUL)) | MK_EXTID2((canuint32)mask<<16);   /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFF00FFFFUL)) | MK_EXTID3((canuint32)mask<<16);   /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFF00FFFFUL)) | MK_EXTID4((canuint32)mask<<16);   /*lint !e572*/
  #endif
}

/************************************************************************
* NAME:               CanSetTxIdExtMidLo
* CALLED BY:
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
C_API_1 void C_API_2 CanSetTxIdExtMidLo( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask )
{
  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFFFF00FFUL)) | MK_EXTID0((canuint32)mask<<8);    /*lint !e572*/
  #if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFFFF00FFUL)) | MK_EXTID1((canuint32)mask<<8);    /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFFFF00FFUL)) | MK_EXTID2((canuint32)mask<<8);    /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFFFF00FFUL)) | MK_EXTID3((canuint32)mask<<8);    /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFFFF00FFUL)) | MK_EXTID4((canuint32)mask<<8);    /*lint !e572*/
  #endif
}

/************************************************************************
* NAME:               CanSetTxIdExtLo
* CALLED BY:
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
C_API_1 void C_API_2 CanSetTxIdExtLo( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask )
{
  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFFFFFF00UL)) | MK_EXTID0((canuint32)mask);     /*lint !e572*/
  #if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFFFFFF00UL)) | MK_EXTID1((canuint32)mask);     /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFFFFFF00UL)) | MK_EXTID2((canuint32)mask);     /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFFFFFF00UL)) | MK_EXTID3((canuint32)mask);     /*lint !e572*/
  #endif
  #if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFFFFFF00UL)) | MK_EXTID4((canuint32)mask);    /*lint !e572*/
  #endif
}
#endif

#if defined( C_ENABLE_TX_OBSERVE )
/************************************************************************
* NAME:               CanTxGetActHandle
* CALLED BY:
* PRECONDITIONS:
* PARAMETER:          logical hardware object handle
* RETURN VALUE:       handle of the message in the assigned mailbox
* DESCRIPTION:        get transmit handle of the message, which is currently
*                     in the mailbox txHwObject.
*************************************************************************/
C_API_1 CanTransmitHandle C_API_2 CanTxGetActHandle( CanObjectHandle logicalTxHdl )
{
  assertUser(logicalTxHdl < kCanNumberOfUsedTxCANObjects, kCanAllChannels, kErrorTxHwHdlTooLarge);

  return (canHandleCurTxObj[logicalTxHdl]);       /*lint !e661*/
}
#endif

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/************************************************************************
* NAME:               CanSetVariableRxDatalen
* CALLED BY:
* PRECONDITIONS:
* PARAMETER:          rxHandle: Handle of receive Message for which the datalen has
*                               to be changed
*                     dataLen:  new number of bytes, which have to be copied to the
*                               message buffer.
* RETURN VALUE:       -
* DESCRIPTION:        change the dataLen of a receive message to copy a
*                     smaller number of bytes than defined in the database.
*                     the dataLen can only be decreased. If the parameter
*                     dataLen is bigger than the statically defined value
*                     the statically defined value will be set.
*************************************************************************/
C_API_1 void C_API_2 CanSetVariableRxDatalen (CanReceiveHandle rxHandle, canuint8 dataLen)
{
  assertUser(rxHandle < kCanNumberOfRxObjects, kCanAllChannels , kErrorRxHandleWrong);  /* legal txHandle ? */

  if (dataLen < CanGetRxDataLen(rxHandle))
  {
    canVariableRxDataLen[rxHandle]=dataLen;
  }
  else
  {
    canVariableRxDataLen[rxHandle] = CanGetRxDataLen(rxHandle);
  }
}
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
/************************************************************************
* NAME:               CanSetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:
* PARAMETER:          -.
* RETURN VALUE:       -
* DESCRIPTION:        The service function CanSetMsgReceivedConditional()
*                     enables the calling of ApplCanMsgCondReceived()
*************************************************************************/
C_API_1 void C_API_2 CanSetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canMsgCondRecState[channel] = kCanTrue;
}

/************************************************************************
* NAME:               CanResetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:
* PARAMETER:          -
* RETURN VALUE:       -
* DESCRIPTION:        The service function CanResetMsgReceivedCondition()
*                     disables the calling of ApplCanMsgCondReceived()
*************************************************************************/
C_API_1 void C_API_2 CanResetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canMsgCondRecState[channel] = kCanFalse;
}

/************************************************************************
* NAME:               CanSetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:
* PARAMETER:          -
* RETURN VALUE:       status of Conditional receive function:
*                     kCanTrue : Condition is set -> ApplCanMsgCondReceived
*                                will be called
*                     kCanFalse: Condition is not set -> ApplCanMsgCondReceived
*                                will not be called
* DESCRIPTION:        The service function CanGetMsgReceivedConditional()
*                     returns the status of the condition for calling
*                     ApplCanMsgCondReceived()
*************************************************************************/
C_API_1 canuint8 C_API_2 CanGetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  return( canMsgCondRecState[channel] );
}
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
/************************************************************************
* NAME:               CanCheckMemory
* CALLED BY:          CanInitPowerOn
* PRECONDITIONS:      Global Interrupts must be disabled
* PARAMETER:          -
* RETURN VALUE:       kCanRamCheckOk    : all mailboxes for this channel are okay
*                     kCanRamCheckFailed: at least one mailbox on this channel is corrupt
*
* DESCRIPTION:        The service function CanCheckMemory() checks if the
*                     mailboxes on this channel are corrupt. The function checks
*                     only those mailboxes which are used in the current configuration.
*************************************************************************/
static canuint8 CanCheckMemory(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanObjectHandle hwObjHandle;
  canuint8 retval;

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
  assertUser(( (CanChannelIdentityAssignment[channel] & V_ACTIVE_IDENTITY) != (tVIdentity)0 ), channel, kErrorDisabledChannel);
#endif

  retval = kCanRamCheckOk; /* assume everthing is okay */

  {
    for(hwObjHandle = CAN_HL_HW_TX_STARTINDEX(canHwChannel); hwObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel); hwObjHandle++)
    {
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
# if defined(C_ENABLE_NOTIFY_CORRUPT_MAILBOX)
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
# endif
        retval = kCanRamCheckFailed;
      }
    }

# if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
    for(hwObjHandle = CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel); hwObjHandle < CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel); hwObjHandle++)
    {
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
#  if defined(C_ENABLE_NOTIFY_CORRUPT_MAILBOX)
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
#  endif
        retval = kCanRamCheckFailed;
      }
    }
# endif

# if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
    for(hwObjHandle = CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); hwObjHandle++)
    {
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
#  if defined(C_ENABLE_NOTIFY_CORRUPT_MAILBOX)
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
#  endif
        retval = kCanRamCheckFailed;
      }
    }
# endif
  } /* for ( all HW channels ) */

  return(retval);
}
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/************************************************************************
* NAME:           ApplCanChannelDummy
* PARAMETER:      channel
*                 current receive channel
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
C_API_1 void C_API_2 ApplCanChannelDummy( CanChannelHandle channel )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;     /* to avoid lint warnings */
# endif
}
#endif   /* C_MULTIPLE_RECEIVE_CHANNEL */


#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) || \
    defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
/************************************************************************
* NAME:           ApplCanRxStructPtrDummy
* PARAMETER:      rxStruct
*                 pointer of CanRxInfoStruct
* RETURN VALUE:   kCanCopyData
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
C_API_1 canuint8 C_API_2 ApplCanRxStructPtrDummy( CanRxInfoStructPtr rxStruct )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  rxStruct = rxStruct;     /* to avoid lint warnings */
# endif
  return kCanCopyData;
}

/************************************************************************
* NAME:           ApplCanTxHandleDummy
* PARAMETER:      txHandle
*                 transmit handle
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
C_API_1 void C_API_2 ApplCanTxHandleDummy( CanTransmitHandle txHandle )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  txHandle = txHandle;     /* to avoid lint warnings */
# endif
}
#endif   /* C_MULTIPLE_RECEIVE_CHANNEL || C_HL_ENABLE_DUMMY_FCT_CALL */

#if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
/************************************************************************
* NAME:           ApplCanTxStructDummy
* PARAMETER:      txStruct
*                 pointer of CanTxInfoStruct
* RETURN VALUE:   kCanCopyData
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
C_API_1 canuint8 C_API_2 ApplCanTxStructDummy( CanTxInfoStruct txStruct )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  txStruct = txStruct;     /* to avoid lint warnings */
# endif
  return kCanCopyData;
}

/************************************************************************
* NAME:           ApplCanRxHandleDummy
* PARAMETER:      rxHandle
*                 receive handle
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
C_API_1 void C_API_2 ApplCanRxHandleDummy( CanReceiveHandle rxHandle )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  rxHandle = rxHandle;     /* to avoid lint warnings */
# endif
}
#endif /* C_HL_ENABLE_DUMMY_FCT_CALL */


/* End of channel */
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.2.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.0 Vector-Informatik GmbH  ************/
