/* Kernbauer Version: 1.11 Konfiguration: can_driver Erzeugungsgangnummer: 344 */

/* STARTSINGLE_OF_MULTIPLE */


/* KB begin CanLL_HeaderH */
/*****************************************************************************
| Project Name: C A N - D R I V E R
|    File Name: CAN_DEF.H
|
|  Description: Application-Interface of the CAN-Driver
|               Declaration of functions, variables, and constants
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
| --------     ---------------------     ------------------------------------
| Ml           Patrick Markl             Vector Informatik GmbH
| Ard          Thomas Arnold             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|Date        Version   Author  Description
|----------  --------  ------  -----------------------------------------------
|2003-02-27  00.01.00    Ml     - Creation
|2003-07-07  00.02.00    Ml     - ESCAN00005987: added support for extended and mixed ID
|2003-07-31  00.03.00    Ml     - resolved issues found in code inspection
|2003-08-01  01.00.00    Ml     - Release for TMS320
|2003-08-03  01.01.00    Ml     - ESCAN00006259: no changes
|2003-08-04  02.00.00    Ml     - ESCAN00006262: added support for TMS470
|2003-12-04  03.00.00    Ml     - ESCAN00007131: added support for OMAP
|2004-01-13  03.01.00    Ml     - ESCAN00007253: fixed several Issues
|2004-01-18  03.02.00    Ml     - ESCAN00007282: no changes
|2004-02-21  03.03.00    Ml     - ESCAN00007670: added CAN RAM check
|2004-03-03  03.04.00    Ml     - ESCAN00007632: no changes
|2004-03-31              Ml     - ESCAN00008069: HECC workaround is configurable   
|2004-04-01              Ml     - ESCAN00008079: Resolved review issues    
|2004-05-17  03.05.00    Ml     - ESCAN00008423: no changes
|2004-07-11  03.06.00    Ml     - ESCAN00008818: no changes
|                        Ml     - ESCAN00006634: resolved errorcounter access
|                        Ml     - ESCAN00008818: no changes
|                        Ml     - ESCAN00008878: no changes
|                        Ml     - ESCAN00008986: no changes
|                        Ml     - ESCAN00007281: no changes
|                        Ml     - ESCAN00008986: no changes
|                        Ml     - ESCAN00009115: Support multiple configurations
|                        Ml     - ESCAN00010073: no changes
|                        Ml     - ESCAN00008885: no changes
|2004-10-19  03.07.00    Ml     - ESCAN00009135: changed handling of LoopCheck Returnvalues 
|                        Ml     - ESCAN00010203: no changes
|                        Ml     - ESCAN00010047: no changes
|                        Ml     - ESCAN00010625: no changes
|                        Ml     - ESCAN00007215: Added memcpy support
|                        Ml     - ESCAN10626, ESCAN10630: no changes
|2005-01-20  04.00.00    Ml     - ESCAN00011416: added support for TMS430
|2005-04-04  04.01.00    Ml     - ESCAN00011745: Changes after review and MISRA check
|                        Ml     - ESCAN00011746: Rx masking also possible in SCC mode (TMS470)
|2005-11-04  04.02.00    Ml     - ESCAN00014177: no changes
|2006-02-21  04.02.01    Ard    - ESCAN00015203: Read error/status flags from CANES register correctly
|                               - ESCAN00015158: RawID macros for Standard ID
|                               - ESCAN00015341: Set priority of low level message
|                               - ESCAN00015347: Improve reception of Rx Basic/Full CAN messages
|                               - ESCAN00015265: Fix TxWrite* macros
|2007-10-05  04.02.02    Ard    - ESCAN00016178: Fix compile error if C_ENABLE_CAN_RAM_CHECK is enabled
|                               - ESCAN00022615: Fix BusOff detection
|2008-04-17  04.02.03    Ard    - ESCAN00026069: Rework Rx data copy for FullCANs to avoid inconsistent data
|***************************************************************************/
/* KB end CanLL_HeaderH */
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
| --------     ---------------------     ------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
| Pl           Georg Pfluegel             Vector Informatik GmbH
| Vg           Frank Voorburg            Vector CANtech, Inc.
| An           Ahmad Nasser              Vector CANtech, Inc.
| Ml           Patrick Markl             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------ -----------------------------------------------------
| ---------  ---  ------ -- --------------------------------------------------
| 24-Jan-01  0.02      Ht     - derived from C16x V3.3
| 18-Apr-01  1.00      Pl     - adaption for ARM7 TDMI
| 26-Apr-01  1.01      Ht     - adaption to LI 1.2
|                             - changed from code doupling to indexed
| 19-Jun-01            Ht     - define of CanSetActive/Passive changed
| 31-Oct-01  1.02      Ht     - support hash search
|                      Vg     - adaption for PowerPC
| 07-Nov-01  1.03      Ht     - remove some comments
| 12-Nov-01  1.04      Fz     - no changes
| 02-Jan-02  1.05      Ht     - ESCAN00002009: support of polling mode improved
| 12-Feb-02  1.06      Pl     - ESCAN00002279: - now it is possible to use only the error-task without the tx-task
|                                              - support of the makro  REENTRANT
|                                              - support of the makro C_HL_ENABLE_RX_INFO_STRUCT_PTR
|                                              - For better performance for the T89C51C there is a switch-case
|                                                instruction for direct call of the PreTransmitfunction
|                                              - insert Prototyp for CanErrorTask
| 18-Mai-02  1.07       Ht    - ESCAN....    : support Hash search with FullCAN controller
|                             - ESCAN00002707: Reception could went wrong if IL and Hash Search
|                             - ESCAN00002893: adaption to LI 1.3
| 29-Mai-02  1.08       Ht    - ESCAN00003028: Transmission could fail in Polling mode
|                             - ESCAN00003082: call Can_LL_TxEnd() in CanMsgTransmit()
|                             - ESCAN00003083: Hash search with extended ID
|                             - ESCAN00003084: Support C_COMP_METROWERKS_PPC
|                             - ESCAN00002164: Temporary vaiable "i" not defined in function CanBasicCanMsgReceived
|                             - ESCAN00003085: support C_HL_ENABLE_IDTYPE_IN_ID
| 25-Jun     1.08.01    Vg    - Declared localInterruptOldFlag in CanRxTask()
|                             - Corrected call to CanWakeUp for multichannel
| 11-Jul-02  1.08.02    Ht    - ESCAN00003203: Hash Search routine does not work will with extended IDs
|                             - ESCAN00003205: Support of ranges could went wrong on some platforms
| 08-Aug-02  1.08.03    Ht    - ESCAN00003447: Transmission without databuffer and pretransmit-function 
| 08-Aug-02  1.08.04    An      no changes
| 09-Sep-02  1.09       Ht    - ESCAN00003837: code optimication with KernelBuilder 
| 2002-12-12 1.10       Ht    -                support Dummy functions for indirect function call 
| 2003-02-04 1.11       Ht    -                optimization for polling mode
|                       Fz    - ESCAN00004600: Flags changed for V850 aFCAN only
| 2003-03-19 1.12       Ht    - ESCAN00005153: Wrong number of Indication bytes for V850 aFCAN only
|                             - ESCAN00005152: optimization of CanInit() in case of Direct Tx Objects
|                             - ESCAN00005143: CompilerWarning about function prototype 
|                                              CanHL_ReceivedRxHandle() and CanHL_IndRxHandle
|                             - ESCAN00005130: Wrong result of Heash Search on second or higher channel               
| 2003-05-12 1.13       Ht    - ESCAN00005624: support CantxMsgDestroyed for multichannel system
|                             - ESCAN00005209: Support confirmation and indication flags for EasyCAN4
|                             - ESCAN00004721: Change data type of Handle in CanRxInfoStruct
| 2003-06-18 1.20       Ht    - ESCAN00005908: support features of RI1.4
|                             - ESCAN: Support FJ16LX-Workaround for multichannel system
|                             - ESCAN00005671: Dynamic ID in extended ID system could fail
|                             - ESCAN00005863: Notification about cancelation failes in case of CanTxMsgDestroyed
| 2003-06-30 1.21       Ht   - ESCAN00006117: Common Confirmation Function: Write access to wrong memory location
|                            - ESCAN00006008: CanCanInterruptDisable in case of polling
|                            - ESCAN00006118: Optimization for Mixed ID and ID type in Own Register or ID type in ID Register
|                            - ESCAN00006063: Undesirable hardware dependency for 
|                                             CAN_HL (CanLL_RxBasicTxIdReceived)
| 2003-09-10 1.22       Ht   - ESCAN00006853: Support V_MEMROM0
|                            - ESCAN00006854: suppress some Compiler warnings
|                            - ESCAN00006856: support CanTask if only Wakeup in polling mode
|                            - ESCAN00006857: variable newDLC not defined in case of Variable DataLen
| 2003-10-14 1.23       Ht   - ESCAN00006858: support BrsTime for internal runtime measurement
|                            - ESCAN00006860: support Conditional Msg Receive
|                            - ESCAN00006865: support "Cancel in HW" with CanTask
|                            - ESCAN00006866: support Direct Tx Objects
|                            - ESCAN00007109: support new memory qualifier for const data and pointer to const
| 2004-01-05 1.24       Ml   - ESCAN00007206: no changes
|                       Ml   - ESCAN00007254: several changes
| 2004-02-06 1.25       Ml   - ESCAN00007281: no changes
|                       Ml   - removed compiler warnings
| 2004-02-21 1.26       Ml   - ESCAN00007670: CAN RAM check
|                       Ml   - ESCAN00007671: no changes
|                       Ml   - ESCAN00007764: no changes
|                       Ml   - ESCAN00007681: no changes
|                       Ml   - ESCAN00007272: no changes
|                       Ml   - ESCAN00008064: encapsulated CanRxHashId in case of array dimension == 0
| 2004-04-16 1.27       Ml   - ESCAN00008204: Optimized CanRxActualId for different ID modes
|                       Ml   - ESCAN00008160: encapsulated functions declared by tool in case of multichannel
|                       Ml   - ESCAN00008266: changed name of parameter of function CanTxGetActHandle
|                       Fz   - ESCAN00008272: Compiler error due to missing array canPollingTaskActive
| 2004-05-10 1.28       Fz   - ESCAN00008328: Compiler error if cancel in hardware is active
|                            - ESCAN00008363: Hole closed when TX in interrupt and cancel in HW is used
|                            - ESCAN00008365: Switch C_ENABLE_APPLCANPREWAKEUP_FCT added
|                            - ESCAN00008391: Wrong parameter macro used in call of 
|                                             CanLL_WakeUpHandling
| 2004-05-24 1.29       Ht   - ESCAN00008441: Interrupt not restored in case of internal error if TX Polling is used
| 2004-09-21 1.30       Ht   - ESCAN00008824: check of reference implementation version added
|                            - ESCAN00008825: No call of ApplCanMsgCancelNotification during CanInit()
|                            - ESCAN00008826: Support asssertions for "Conditional Message Received"  
|                       Ml   - ESCAN00008752: Added function qualifier macros
|                       Ht   - ESCAN00008823: compiler error due to array size 0
|                            - ESCAN00008977: label without instructions
|                            - ESCAN00009485: Message via Normal Tx Object will not be sent  
|                            - ESCAN00009497: support of CommonCAN and RX queue added
| 2004-09-28 1.31       Ht   - ESCAN00009703: unresolved functions CAN_POLLING_IRQ_DISABLE/RESTORE()
| 2004-11-25 1.32       Ht   - move fix for ESCAN00007671 to CAN-LL of DrvCan_MpcToucanHll
|                            - ESCAN00010350: Dynamic Tx messages are send always with Std. ID
|                            - ESCAN00010388: ApplCanMsgConfirmed will only be called if realy transmitted
|                       Ml   - ESCAN00009931: The HardwareLoopCheck should have a channelparameter in multichannel systems.
|                            - ESCAN00010093: lint warning: function type inconsistent "CanCheckMemory"
|                        Ht  - ESCAN00010811: remove Misra and compiler warnings
|                            - ESCAN00010812: support Multi ECU
|                            - ESCAN00010526: CAN interrupts will be disabled accidently
|                            - ESCAN00010584: ECU may crash or behave strange with Rx queue active
| 2005-01-20 1.33       Ht   - ESCAN00010877: ApplCanMsgTransmitConf() is called erronemous
|
|
|    ************    Version and change information of      **********        
|    ************    high level part only                   **********        
|
|    Please find the version number of the whole module in the previous 
|    File header.
|
|****************************************************************************/

#ifndef  CAN_HL_H
#define  CAN_HL_H


/***************************************************************************/
/* Version                  (abcd: Main version ab Sub Version cd )        */
/***************************************************************************/
/* KB begin CanLL_VersionH */
/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan_Tms470HeccsccHll CQComponent : Implementation */
#define CAN_DRIVER_VERSION 0x0402
#define CAN_BUGFIX_VERSION 0x03
/* KB end CanLL_VersionH */


/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan__coreHll CQComponent : Implementation */
#define DRVCAN__COREHLL_VERSION 0x0133
#define DRVCAN__COREHLL_RELEASE_VERSION 0x00


/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/

#include "v_def.h"

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/

/***************************************************************************/
/* Default switches                                                        */
/* Automatic define settings, depending on user configuration in can_cfg.h */
/***************************************************************************/
#if defined ( C_ENABLE_MULTICHANNEL_API )
# define C_MULTIPLE_RECEIVE_CHANNEL
# define MULTIPLE_RECEIVE_CHANNEL
#else
# define C_SINGLE_RECEIVE_CHANNEL
#endif

#if ( kCanNumberOfTxObjects > 0 )
# if !(defined( C_ENABLE_CAN_TRANSMIT ) || defined( C_DISABLE_CAN_TRANSMIT ))
#  define C_ENABLE_CAN_TRANSMIT
# endif
#endif

#if !(defined( C_ENABLE_OFFLINE ) || defined( C_DISABLE_OFFLINE ))
# define C_ENABLE_OFFLINE
#endif

#if !(defined( C_ENABLE_STOP ) || defined( C_DISABLE_STOP ))
# define C_ENABLE_STOP
#endif

#if !defined ( kCanNumberOfHwChannels )
/* compatibility for older tool versions */
# define kCanNumberOfHwChannels kCanNumberOfChannels
#endif

/* KB begin CanLL_DefaultSwitches */


/* in order to avoid to lose of a pending message the overrun is checked in the receive function */
#define C_HL_DISABLE_OVERRUN_IN_STATUS

/* Ranges are filtered by means of software */
#define C_HL_DISABLE_HW_RANGES_FILTER

/* ID type is stored in the identifier register */
#define C_HL_ENABLE_IDTYPE_IN_ID

#define C_HL_DISABLE_INTCTRL_BITACCESS

#if defined(C_HL_ENABLE_IDTYPE_IN_ID)
/* Specify the ID tables which contain the ID type */
# define C_LL_RX_IDTYPE_TABLE      CanRxId0
# define C_LL_TX_IDTYPE_TABLE      CanTxId0
#endif

/* this driver uses dummy calls */
#define C_HL_DISABLE_DUMMY_FCT_CALL

#define C_HL_DISABLE_TX_MSG_DESTROYED

/* no additional condition to exit CanTransmit for the TMSxxx */
#define C_HL_DISABLE_HW_EXIT_TRANSMIT  

# if defined(C_ENABLE_SLEEP_WAKEUP)
/* last init object is used in case of sleep wakeup */
#  define C_HL_ENABLE_LAST_INIT_OBJ
# endif

/* TMS470 driver does not disable CAN interrupts to avoid phantom interrupts */
# define C_HL_DISABLE_CAN_IRQ_DISABLE

/* TMS470 specific for CanCanInterruptDisable */
# define C_ENABLE_INTCTRL_ADD_CAN_FCT   
   
/* The driver does not need to adjust the computed handle */
#define C_HL_DISABLE_ADJUST_RXHANDLE

/* Cancel in hardware fires an interrupt */
#define C_HL_DISABLE_CANCEL_IN_HW_TASK

/* The hardwareloopcheck is channel specific */
#define C_HL_ENABLE_HW_LOOP_TIMER_CHANNEL_SPECIFIC

/* KB end CanLL_DefaultSwitches */

#if !defined(C_CALLBACK_1)
# define C_CALLBACK_1
#endif

#if !defined(C_CALLBACK_2)
# define C_CALLBACK_2
#endif

#if !defined(C_API_1)
# define C_API_1
#endif

#if !defined(C_API_2)
# define C_API_2
#endif

#if !defined(C_API_3)
# define C_API_3 REENTRANT
#endif

#if !defined(REENTRANT)
#  define REENTRANT
#endif

#if defined C_ENABLE_TX_POLLING
#  if !(defined ( C_ENABLE_ERROR_POLLING )  || defined ( C_DISABLE_ERROR_POLLING ))
#    define C_ENABLE_ERROR_POLLING 
#  endif
#endif

#if !defined(C_DISABLE_TASK_RECURSION_CHECK)
# define C_ENABLE_TASK_RECURSION_CHECK
#endif

/***************************************************************************/
/* return values                                                           */
/***************************************************************************/

/* return values for precopy-routines */
#define kCanNoCopyData     ((canuint8)0x00)
#define kCanCopyData       ((canuint8)0x01)

/* Bitmask of return value of CanGetStatus() */
#define kCanTxOff            ((canuint8)0x00)
#define kCanTxOn             ((canuint8)0x01)
#define kCanTxNotOn          ((canuint8)0xFE)              /* internal use only */
#define kCanHwIsStop         ((canuint8)0x02)
#define kCanHwIsInit         ((canuint8)0x04)
#define kCanHwIsInconsistent ((canuint8)0x08)              /* used of for common CAN */
#define kCanHwIsWarning      ((canuint8)0x10)
#define kCanHwIsPassive      ((canuint8)0x20)
#define kCanHwIsBusOff       ((canuint8)0x40)
#define kCanHwIsSleep        ((canuint8)0x80)

/* CanTransmit return values -----------------------------------------------*/
#define kCanTxFailed               ((canuint8)0x00)  /* Tx path switched off or no sending possible */
#define kCanTxOk                   ((canuint8)0x01)  /* msg transmitted or in queue                 */
#define kCanTxPartOffline          ((canuint8)0x02)  /* Tx path switched part off or           */
#define kCanCommunicationDisabled  ((canuint8)0x03)  /* if application has disabled com after memory check */

#define kCanTxNotify               ((canuint8)0x05)  /* internal returncode only - not used for the API */

/* CanGetDynTxObj return values -----------------------------------------------*/
#define kCanNoTxDynObjAvailable   ((CanTransmitHandle)0xFFFFFFFFU)

/* CanReleaseDynTxObj return values -------------------------------------------*/
#define kCanDynReleased             ((canuint8)0x00)
#define kCanDynNotReleased          ((canuint8)0x01)

/* return values of CanRxActualIdType */
#define kCanIdTypeStd               C_STD_ID
#define kCanIdTypeExt               C_EXT_ID

/* generel return values */
#define kCanFailed                  0x00  
#define kCanOk                      0x01  
#define kCanNotSupported            0x02 

#define kCanFalse                   0x00
#define kCanTrue                    0x01

/* CanTxGetActHandle return value - status of transmit objects */
#define kCanBufferMsgTransmit     ((CanTransmitHandle)0xFFFFFFFCU)   /* mark a transmit object as used by CanMsgTransmit */

/* return values of CanDirectTxGetStatus */
#define kCanTxFree            ((canuint8)0x00)  /* transmit object is free    */
#define kCanTxAck             ((canuint8)0x01)  /* transmit was succesful     */
#define kCanTxPending         ((canuint8)0x02)  /* transmission is pending    */

/* results of CAN RAM check */
#define kCanRamCheckFailed        ((canuint8)0x00)
#define kCanRamCheckOk            ((canuint8)0x01)
#define kCanDisableCommunication  ((canuint8)0x00)
#define kCanEnableCommunication   ((canuint8)0x01)


/* KB begin CanLL_ReturnValuesH */
/* KB end CanLL_ReturnValuesH */

/***************************************************************************/
/* parameter values                                                        */
/***************************************************************************/
/* parameters for partial offline */
#define kCanTxPartInit            ((canuint8)0x00)

/* paramater for assertions */
#define kCanAllChannels           ((CanChannelHandle)0xFFU)

/* canRxHandle does not contain a certain message handle or entry in table is not used */
#define kCanRxHandleNotUsed       ((CanReceiveHandle)  0xFFFFFFFFU)
#define kCanTxHandleNotUsed       ((CanTransmitHandle) 0xFFFFFFFFU)

/* entries for generated table CanTxDirectTransmitSupport[] */
#define kCanTxNoDirectObj        ((canuint8)0x00)
#define kCanTxDirectObj          ((canuint8)0x01)

/* entries for generated table CanHwMsgTransmitIndex[], CanHwTxNormalIndex[] - unsed in case of CommonCan */
/* table entry does not contain a HW object assignment */
#define kCanObjectHandleNotUsed  ((CanObjectHandle)0xFFU)

/* KB begin CanLL_ParameterValuesH */
/* parameter for ApplCanTimerStart(),-End(), -Loop()*/
#define kCanLoopWaitConfigChangeEnable     ((vuint8)0x00)
#define kCanLoopWaitPowerDownMode          ((vuint8)0x01)
#define kCanLoopWaitLeavePowerDownMode     ((vuint8)0x02)
#define kCanLoopCopyRxMailBox              ((vuint8)0x03)
#define kCanLoopWaitCanStop                ((vuint8)0x04)
#define kCanLoopWaitCanStart               ((vuint8)0x05)
#define kCanLoopCopyRxBasicMailBox         ((vuint8)0x06)
/* KB end CanLL_ParameterValuesH */

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

#ifdef C_SINGLE_RECEIVE_CHANNEL 
# define CAN_CHANNEL_CANTYPE_ONLY           void
# define CAN_CHANNEL_CANTYPE_FIRST
# define CAN_CHANNEL_CANTYPE_LOCAL         
# define CAN_CHANNEL_CANPARA_ONLY
# define CAN_CHANNEL_CANPARA_FIRST
#else
# define CAN_CHANNEL_CANTYPE_ONLY          CanChannelHandle channel
# define CAN_CHANNEL_CANTYPE_FIRST         CanChannelHandle channel,
# define CAN_CHANNEL_CANTYPE_LOCAL         CanChannelHandle channel;
# define CAN_CHANNEL_CANPARA_ONLY          channel
# define CAN_CHANNEL_CANPARA_FIRST         channel,
#endif


#if (kCanNumberOfUsedCanRxIdTables == 1)
# define MK_RX_RANGE_MASK(i)   MK_RX_RANGE_MASK0(i)
# define MK_RX_RANGE_CODE(i)   MK_RX_RANGE_CODE0(i)
#endif
#if (kCanNumberOfUsedCanRxIdTables == 2)
# define MK_RX_RANGE_MASK(i)   MK_RX_RANGE_MASK0(i),MK_RX_RANGE_MASK1(i)
# define MK_RX_RANGE_CODE(i)   MK_RX_RANGE_CODE0(i),MK_RX_RANGE_CODE1(i)
#endif
#if (kCanNumberOfUsedCanRxIdTables == 3)
# define MK_RX_RANGE_MASK(i)   MK_RX_RANGE_MASK0(i),MK_RX_RANGE_MASK1(i),MK_RX_RANGE_MASK2(i)
# define MK_RX_RANGE_CODE(i)   MK_RX_RANGE_CODE0(i),MK_RX_RANGE_CODE1(i),MK_RX_RANGE_CODE2(i)
#endif
#if (kCanNumberOfUsedCanRxIdTables == 4)
# define MK_RX_RANGE_MASK(i)   MK_RX_RANGE_MASK0(i),MK_RX_RANGE_MASK1(i),MK_RX_RANGE_MASK2(i),MK_RX_RANGE_MASK3(i)
# define MK_RX_RANGE_CODE(i)   MK_RX_RANGE_CODE0(i),MK_RX_RANGE_CODE1(i),MK_RX_RANGE_CODE2(i),MK_RX_RANGE_CODE3(i)
#endif
#if (kCanNumberOfUsedCanRxIdTables == 5)
# define MK_RX_RANGE_MASK(i)   MK_RX_RANGE_MASK0(i),MK_RX_RANGE_MASK1(i),MK_RX_RANGE_MASK2(i),MK_RX_RANGE_MASK3(i),MK_RX_RANGE_MASK4(i)
# define MK_RX_RANGE_CODE(i)   MK_RX_RANGE_CODE0(i),MK_RX_RANGE_CODE1(i),MK_RX_RANGE_CODE2(i),MK_RX_RANGE_CODE3(i),MK_RX_RANGE_CODE4(i)
#endif


/* calculate size of cond/ind-flags */
#define kCanNumberOfConfBytes   ((kCanNumberOfConfFlags + 7)/8)
#define kCanNumberOfConfWords   ((kCanNumberOfConfFlags + 15)/16)
#define kCanNumberOfConfDWords  ((kCanNumberOfConfFlags + 31)/32)
#define kCanNumberOfIndBytes    ((kCanNumberOfIndFlags + 7)/8)
#define kCanNumberOfIndWords    ((kCanNumberOfIndFlags + 15)/16)
#define kCanNumberOfIndDWords   ((kCanNumberOfIndFlags + 31)/32)

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */

#if defined(C_ENABLE_EXTENDED_ID)
# if defined(C_ENABLE_MIXED_ID)
#  define CanRxActualId(rxStruct)   ((CanRxActualIdType(rxStruct) == kCanIdTypeExt) ?      \
                                      CanRxActualExtId(rxStruct) : CanRxActualStdId(rxStruct))
# else
# define CanRxActualId(rxStruct)   CanRxActualExtId(rxStruct)
# endif
#else
# define CanRxActualId(rxStruct)   CanRxActualStdId(rxStruct)
#endif

/* Macros for CAN Status */
#define CanHwIsOk(state)               (((state) & (kCanHwIsWarning |     \
                                                    kCanHwIsPassive |     \
                                                    kCanHwIsBusOff) )    ? 0 : 1)
#define CanHwIsWarning(state)          (((state) & kCanHwIsWarning)      ? 1 : 0)
#define CanHwIsPassive(state)          (((state) & kCanHwIsPassive)      ? 1 : 0)
#define CanHwIsBusOff(state)           (((state) & kCanHwIsBusOff)       ? 1 : 0)
#define CanHwIsWakeup(state)           (((state) & kCanHwIsSleep)        ? 0 : 1)
#define CanHwIsSleep(state)            (((state) & kCanHwIsSleep)        ? 1 : 0)
#define CanHwIsStop(state)             (((state) & kCanHwIsStop)         ? 1 : 0)
#define CanHwIsStart(state)            (((state) & kCanHwIsStop)         ? 0 : 1)
#define CanIsOnline(state)             (((state) & kCanTxOn)             ? 1 : 0)
#define CanIsOffline(state)            (((state) & kCanTxOn)             ? 0 : 1)
#define CanHwIsInconsistent(state)     (((state) & kCanHwIsInconsistent) ? 1 : 0)



# if defined( C_DRV_INTERNAL)
#  define CanGetTxId0(i)               CanTxId0[i]
#  define CanGetTxId1(i)               CanTxId1[i]
#  define CanGetTxId2(i)               CanTxId2[i]
#  define CanGetTxId3(i)               CanTxId3[i]
#  define CanGetTxId4(i)               CanTxId4[i]
#  if defined (C_ENABLE_EXTENDED_ID)
#   if defined (C_ENABLE_MIXED_ID)
#    if defined(C_HL_ENABLE_IDTYPE_IN_ID)
#     define CanGetTxIdType(i)         (C_LL_TX_IDTYPE_TABLE[i] & kCanIdTypeExt)
#    else
#     if defined(C_HL_ENABLE_IDTYPE_IN_DLC)
#      define CanGetTxIdType(i)        (CanTxDLC[i] & kCanIdTypeExt)
#     else
#      define CanGetTxIdType(i)        (CanTxIdType[i] & kCanIdTypeExt)
#     endif
#    endif
#   else
#    define CanGetTxIdType(i)          (kCanIdTypeExt)
#   endif
#  else
#   define CanGetTxIdType(i)           (kCanIdTypeStd)
#  endif
# endif /* C_DRV_INTERNAL */

# define CanGetTxDlc(i)               CanTxDLC[i]
# define CanGetTxDataPtr(i)           CanTxDataPtr[i]
# define CanGetConfirmationOffset(i)  CanConfirmationOffset[i]
# define CanGetConfirmationMask(i)    CanConfirmationMask[i]
# define CanGetTxHwObj(i)             CanTxHwObj[i]
# define CanGetTxSendMask(i)          CanTxSendMask[i]
# define CanGetApplPreTransmitPtr(i)  CanTxApplPreTransmitPtr[i]    
# define CanGetApplConfirmationPtr(i) CanTxApplConfirmationPtr[i]
# define CanGetChannelOfTxObj(i)      CanTxMsgHandleToChannel[i]

# if defined( C_DRV_INTERNAL)
#  define CanGetRxId0(i)               CanRxId0[i]
#  define CanGetRxId1(i)               CanRxId1[i]
#  define CanGetRxId2(i)               CanRxId2[i]
#  define CanGetRxId3(i)               CanRxId3[i]
#  define CanGetRxId4(i)               CanRxId4[i]
#  if defined (C_ENABLE_EXTENDED_ID)
#   if defined (C_ENABLE_MIXED_ID)
#    if defined(C_HL_ENABLE_IDTYPE_IN_ID)
#     define CanGetRxIdType(i)         (C_LL_RX_IDTYPE_TABLE[i] & kCanIdTypeExt)
#    else
#     define CanGetRxIdType(i)         (CanRxIdType[i] & kCanIdTypeExt)
#    endif
#   else
#    define CanGetRxIdType(i)          (kCanIdTypeExt)
#   endif
#  else
#   define CanGetRxIdType(i)           (kCanIdTypeStd)
#  endif
# endif /* C_DRV_INTERNAL */

# define CanGetRxDataLen(i)           CanRxDataLen[i]
# define CanGetRxDataPtr(i)           CanRxDataPtr[i]
# define CanGetIndicationOffset(i)    CanIndicationOffset[i]
# define CanGetIndicationMask(i)      CanIndicationMask[i]
# define CanGetApplPrecopyPtr(i)      CanRxApplPrecopyPtr[i]
# define CanGetApplIndicationPtr(i)   CanRxApplIndicationPtr[i]

#define CAN_START_INDEX_INIT_OBJ(i)  CanInitObjectStartIndex[i]
#define CAN_START_INDEX_TX_OBJ(i)    CanTxStartIndex[i]
#define CAN_START_INDEX_RX_OBJ(i)    CanRxStartIndex[i]

#if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined(  C_ENABLE_DYN_TX_OBJECTS )  && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  /* set confirmation active for CanReleaseDynTxObj() */
# ifdef C_SINGLE_RECEIVE_CHANNEL 
  #define CanConfirmStart(txHandle)           {confirmHandle[0] = (txHandle);}
  #define CanConfirmEnd()                     {confirmHandle[0] = ((CanTransmitHandle)0xFFFFFFFF);}
# else
  #define CanConfirmStart(channel, txHandle)  {confirmHandle[channel] = (txHandle);}
  #define CanConfirmEnd(channel)              {confirmHandle[channel] = ((CanTransmitHandle)0xFFFFFFFF);}
# endif
#endif

#if defined ( C_ENABLE_PART_OFFLINE )
# ifdef C_SINGLE_RECEIVE_CHANNEL 
#  define CanPartOffline(sendGroup)    CanSetPartOffline( sendGroup)
#  define CanPartOnline(sendGroup)     CanSetPartOnline( (canuint8)~(canuint8)(sendGroup)) 
# else
#  define CanPartOffline(channel, sendGroup)    CanSetPartOffline(channel, sendGroup)
#  define CanPartOnline(channel,  sendGroup)    CanSetPartOnline(channel, (canuint8)~(canuint8)(sendGroup)) 
# endif
#else
# ifdef C_SINGLE_RECEIVE_CHANNEL 
#  define CanPartOffline(sendGroup)
#  define CanPartOnline(sendGroup)
# else
#  define CanPartOffline(channel, sendGroup)
#  define CanPartOnline(channel, sendGroup)
# endif
#endif

#if ( C_SECURITY_LEVEL == 0 )
# define CanDeclareGlobalInterruptOldStatus
# define CanPutGlobalInterruptOldStatus(x)
# define CanGetGlobalInterruptOldStatus(x)
# define CanSingleGlobalInterruptDisable()
# define CanSingleGlobalInterruptRestore()
# define CanNestedGlobalInterruptDisable()
# define CanNestedGlobalInterruptRestore()
#else  /* C_SECURITY_LEVEL == 0 */
# if defined ( C_ENABLE_OSEK_OS ) && defined ( C_ENABLE_OSEK_OS_INTCTRL )
#  define CanDeclareGlobalInterruptOldStatus
#  define CanPutGlobalInterruptOldStatus(x)
#  define CanGetGlobalInterruptOldStatus(x)
#  define CanSingleGlobalInterruptDisable()      DisableAllInterrupts()
#  define CanSingleGlobalInterruptRestore()      EnableAllInterrupts()
#  if defined ( osdOSEK2_1 )
#   define CanNestedGlobalInterruptDisable()     OsSaveDisableGlobalNested()
#   define CanNestedGlobalInterruptRestore()     OsRestoreEnableGlobalNested()
#  else
#   define CanNestedGlobalInterruptDisable()     SuspendAllInterrupts()
#   define CanNestedGlobalInterruptRestore()     ResumeAllInterrupts()
#  endif
# else
#  define CanDeclareGlobalInterruptOldStatus     tCanHLIntOld localInterruptOldStatus;
#  define CanPutGlobalInterruptOldStatus(x)      (localInterruptOldStatus = (x))
#  define CanGetGlobalInterruptOldStatus(x)      ((x) = localInterruptOldStatus)
#  define CanSingleGlobalInterruptDisable()      CanLL_GlobalInterruptDisable(&localInterruptOldStatus)
#  define CanSingleGlobalInterruptRestore()      CanLL_GlobalInterruptRestore(localInterruptOldStatus)
#  define CanNestedGlobalInterruptDisable()      CanLL_GlobalInterruptDisable(&localInterruptOldStatus)
#  define CanNestedGlobalInterruptRestore()      CanLL_GlobalInterruptRestore(localInterruptOldStatus)
# endif
#endif /* C_SECURITY_LEVEL == 0 */


/* KB begin CanLL_MacrosH */

#if defined(C_COMP_TI_TMS470)  
# define CAN_IRQ_FUNC_QUAL1
# define CAN_IRQ_FUNC_QUAL2
#endif




#if defined(C_COMP_TI_TMS470)  
# define kCanSCCMode  0
# define kCanHECCMode 1
#endif

/* mask to keep some bits unchanged in the DLC for use with variable DLC */
/*  - bit remaining unchanged should be masked with 1 e.g. id type or direction */
#define CanLL_DlcMask            0x00

/* datatypes of loop counters and local parameters - depends on the CPU type */
#define CANUINTX     vuintx
#define CANSINTX     vsintx

/* macros to set DLC and ID in rx-,tx-structures ---------------------------*/

#if defined(C_ENABLE_RANGE_EXTENDED_ID)
# define MK_RX_RANGE_MASK0(id)     ((vuint32)((vuint32)(id)) | (vuint32)0x80000000)
# define MK_RX_RANGE_CODE0(id)     ((vuint32)((vuint32)(id)))
#else
#  define MK_RX_RANGE_MASK0(id)    ((vuint32)(((vuint32)(id)) << 18)| (vuint32)0x80000000)
#  define MK_RX_RANGE_CODE0(id)    ((vuint32)(((vuint32)(id)) << 18))
#endif

#if defined (C_ENABLE_EXTENDED_ID)
# define MK_STDID(id)               MK_STDID0(id)
#else
# define MK_STDID(id)               ((vuint16)((vuint16)(id) << 2))
#endif
#define MK_STDID_HI(id)            ((vuint16)((vuint16)(id) << 2))
#define MK_STDID_LO(id)            ((vuint16)0x0000)

# define MK_STDID0(id)             ((vuint32)((vuint32)(id) << 18))

                                
#define MK_EXTID(id)               ((vuint32)((vuint32)(id) | (vuint32)0x80000000)) /* set also IDE bit */
#define MK_EXTID_HI(id)            ((vuint16)(((vuint32)0x80000000 | (vuint32)(id)) >> 16))
#define MK_EXTID_LO(id)            ((vuint16)(id))
#define MK_EXTID0(id)              ((vuint32)((vuint32)(id) | (vuint32)0x80000000))

/* return lowest 8 bit of the logical ID */

#if defined(C_ENABLE_RANGE_EXTENDED_ID)
# define XT_ID_LO(id)             (vuint32)((id) & (vuint32)0x000000FF)
#else
#  define XT_ID_LO(id)            (vuint32)(((id) >> 18) & (vuint32)0x000000FF)
#endif

#define C_RX_DIR                  0x00
#define C_TX_DIR                  0x08
                                
#define C_STD_ID                  (vuint32)0x00000000
#define C_EXT_ID                  (vuint32)0x80000000
                                
#define MK_TX_DLC(dlc)            (dlc)
#define MK_TX_DLC_EXT(dlc)        (dlc)
#define XT_TX_DLC(dlc)            (dlc)


#if defined(C_SINGLE_RECEIVE_CHANNEL)
/* Bus-Off functions */
# define CanResetBusOffStart(a)
# define CanResetBusOffEnd(a)       CanInit(a)

/* Bus-Sleep functions */
# define CanResetBusSleep(a)        CanInit(a)

#else
/* Bus-Off functions */
# define CanResetBusOffStart(ch,a)
# define CanResetBusOffEnd(ch,a)    CanInit(ch,a)
  
/* Bus-Sleep functions */
# define CanResetBusSleep(ch,a)     CanInit(ch,a)
#endif

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */

/* The macros CanRxActualIdRawx() have to provide the same the macros MK_STDIDx() for unused bits
   for standard identifier in mixed mode */

#if defined(C_ENABLE_EXTENDED_ID)
# define CanRxActualIdRaw0(rxStruct)       ((*(vuint32*)(rxStruct -> pChipMsgObj)) & (vuint32)0x9FFFFFFF)
# define CanRxActualIdRaw(rxStruct)         ((*(vuint32*)(rxStruct -> pChipMsgObj) & (vuint32)0x9FFFFFFF) )
# define CanRxActualIdExtHi(rxStruct)       ((*(vuint32*)(rxStruct -> pChipMsgObj) & (vuint32)0x1F000000) >> 24)
# define CanRxActualIdExtMidHi(rxStruct)    ((*(vuint32*)(rxStruct -> pChipMsgObj) & (vuint32)0x00FF0000) >> 16)
# define CanRxActualIdExtMidLo(rxStruct)    ((*(vuint32*)(rxStruct -> pChipMsgObj) & (vuint32)0x0000FF00) >>  8)
# define CanRxActualIdExtLo(rxStruct)       ((*(vuint32*)(rxStruct -> pChipMsgObj) & (vuint32)0x000000FF)      )

# if defined(C_COMP_TI_TMS470) 
#  define CanRxActualIdRawHi(rxStruct)      ((*(vuint16*)(rxStruct -> pChipMsgObj)) & (vuint16)0x9FFF)
#  define CanRxActualIdRawLo(rxStruct)      (*((vuint16*)(rxStruct -> pChipMsgObj) + 0x01))
# endif
#else

#if defined(C_COMP_TI_TMS470) 
# define CanRxActualIdRaw0(rxStruct)        ((*(vuint32*)(rxStruct -> pChipMsgObj)) & (vuint32)0x9FFFFFFF)
#  define CanRxActualIdRaw(rxStruct)        ((*(vuint16*)(rxStruct -> pChipMsgObj)) & (vuint16)0x9FFF)
#  define CanRxActualIdRawHi(rxStruct)      (vuint8)( (*(vuint16*)(rxStruct -> pChipMsgObj) & (vuint16)0x9F00) >> 8)
#  define CanRxActualIdRawLo(rxStruct)      (vuint8)( (*(vuint16*)(rxStruct -> pChipMsgObj) & (vuint16)0x00FF) )
# endif
#endif             

#if defined(C_COMP_TI_TMS470) 
# define CanRxActualExtId(rxStruct)         (vuint32)((*(vuint32*)(rxStruct -> pChipMsgObj)) & (vuint32)0x1FFFFFFF)
# define CanRxActualStdId(rxStruct)         (vuint16)(((*(vuint16*)(rxStruct -> pChipMsgObj)) >> 2) & (vuint32)0x000007FF)
#endif

#define CanRxActualData(rxStruct,i)        (vuint16)(*(rxStruct -> pChipData + (i)))
#if defined(C_COMP_TI_TMS470)  
# define CanRxActualDLC(rxStruct)          (vuint8)(*(vuint8*)(rxStruct -> pChipMsgObj + 0x07) & (vuint32)0x0000000F)
#endif



#define CanRxActualIdType(rxStruct)        (((vuint32)(*(vuint32*)(rxStruct -> pChipMsgObj) & C_EXT_ID)) ? kCanIdTypeExt : kCanIdTypeStd)



#if defined(C_COMP_TI_TMS470)  
# if defined(C_SINGLE_RECEIVE_CHANNEL)
#  define CanRxActualErrorCounter()         (((tCanCntrlRegBlock*)kCanBasisAdr) -> CanRec)
#  define CanTxActualErrorCounter()         (((tCanCntrlRegBlock*)kCanBasisAdr) -> CanTec)
# else
#  define CanRxActualErrorCounter(channel)  (((tCanCntrlRegBlock*)(CanBasisAdr[channel])) -> CanRec)
#  define CanTxActualErrorCounter(channel)  (((tCanCntrlRegBlock*)(CanBasisAdr[channel])) -> CanTec)
# endif
#endif

/* Macros for CAN message access within PreTransmit() function */
#if defined(C_ENABLE_EXTENDED_ID)
# define CanTxWriteActId(txStruct, id)                                      \
{                                                                           \
  ((tCanMailBox*)((txStruct).pChipData - 8)) -> CanMid = MK_STDID0(id);     \
}

# define CanTxWriteActExtId(txStruct,id)                                    \
{                                                                           \
  ((tCanMailBox*)((txStruct).pChipData - 8)) -> CanMid = MK_EXTID0(id);     \
}
#else
# define CanTxWriteActId(txStruct,id)                                       \
{                                                                           \
  ((tCanMailBox*)((txStruct).pChipData - 8)) -> CanMid = MK_STDID0(id);     \
}
#endif

#define CanTxWriteActDLC(txStruct,dlc)                                      \
{                                                                           \
  ((tCanMailBox*)((txStruct).pChipData - 8)) -> CanMcf = MK_TX_DLC(dlc);    \
}

/* macros for buffer access */
#define StartRxReadSync()                CanGlobalInterruptDisable()
#define EndRxReadSync()                  CanGlobalInterruptRestore()
                                        
#define StartRxWriteSync()               CanGlobalInterruptDisable()
#define EndRxWriteSync()                 CanGlobalInterruptRestore()
                                        
#define StartTxReadSync()               
#define EndTxReadSync()                 
                                        
#define StartTxWriteSync()               CanGlobalInterruptDisable()
#define EndTxWriteSync()                 CanGlobalInterruptRestore()

#define StartRxWriteCANSync() 
#define EndRxWriteCANSync()   

#define StartRxReadCANSync()  
#define EndRxReadCANSync()    

#define StartRxFullCANReadSync(Obj)
#define EndRxFullCANReadSync(Obj)

#define StartRxFullCANWriteSync(obj)
#define EndRxFullCANWriteSync(obj)

#define CanStartFlagWriteSync()          CanGlobalInterruptDisable()
#define CanEndFlagWriteSync()            CanGlobalInterruptRestore()

/* KB end CanLL_MacrosH */

/***************************************************************************/
/* error codes                                                             */
/***************************************************************************/
/* error numbers for User Assertions 0x00-0x1f - hardware independed */
#define kErrorTxVarDLCTooLarge            ((canuint8)0x01)
#define kErrorTxHdlTooLarge               ((canuint8)0x02)
#define kErrorIntRestoreTooOften          ((canuint8)0x03)
#define kErrorIntDisableTooOften          ((canuint8)0x04)
#define kErrorRxFullCanObj                ((canuint8)0x05)   /* not used any more */
#define kErrorFctCalledRec                ((canuint8)0x06)
#define kErrorNoDataBuffer                ((canuint8)0x07)
#define kErrorChannelHdlTooLarge          ((canuint8)0x08)
#define kErrorInitObjectHdlTooLarge       ((canuint8)0x09)
#define kErrorTxHwHdlTooLarge             ((canuint8)0x0A)
#define kErrorIllDirectTxHandle           ((canuint8)0x0B)
#define kErrorIllNonDirectTxHandle        ((canuint8)0x0C)

#define kErrorAccessedInvalidDynObj       ((canuint8)0x10)
#define kErrorAccessedStatObjAsDyn        ((canuint8)0x11)
#define kErrorDynObjReleased              ((canuint8)0x12)
#define kErrorWrongIDType                 ((canuint8)0x13)

#define kErrorPollingTaskRecursion        ((canuint8)0x14)
#define kErrorDisabledChannel             ((canuint8)0x15)
#define kErrorDisabledTxMessage           ((canuint8)0x16)

/* KB begin CanLL_ErrorCodesUserH */
/* error numbers for User Assertions 0x20-0x3f - hardware depended */
#define kErrorPriviledgedModeRequired ((vuint8)0x20)
#define kErrorFunctionNotSupported    ((vuint8)0x21)

/* KB end CanLL_ErrorCodesUserH */

/* error numbers for Gentool Assertions 0x40-0x5f */
#define kErrorIllegalROMTxID              ((canuint8)0x40)    /* not realized */
#define kErrorTxROMDLCTooLarge            ((canuint8)0x41)
#define kErrorIllegalROMRxID              ((canuint8)0x42)    /* not used any more */
#define kErrorRxROMDLCTooLarge            ((canuint8)0x43)    /* not realized */
#define kErrorToManyFullCanObjects        ((canuint8)0x44)

/* KB begin CanLL_ErrorCodesGentoolH */
/* error numbers for Gentool Assertions 0x60-0x7f - hardware depended */
#define kErrorWrongTSegValue          ((vuint8)0x60) /* TSeg1 and TSeg2 must not be equal to 0 ! */
/* KB end CanLL_ErrorCodesGentoolH */

/* error numbers for Hardware Assertions 0x80-0x9f */
#define kErrorhardware                    ((canuint8)0x80)    /* not realized */
#define kErrorWrongFilterMOde             ((canuint8)0x81)    /* not realized */
#define kErrorTxBufferBusy                ((canuint8)0x82)

/* KB begin CanLL_ErrorCodesHardwareH */
/* error numbers for Hardware Assertions 0xa0-0xbf - hardware depended */
/* KB end CanLL_ErrorCodesHardwareH */

/* error numbers for Internal Assertions 0xc0-0xdf */
#define kErrorIllTxQueueCnt               ((canuint8)0xc0)   /* not used any more */
#define kErrorTxIdNotFound                ((canuint8)0xc1)
#define kErrorTxHandleWrong               ((canuint8)0xc2)
#define kErrorIllIrptNumber               ((canuint8)0xc3)
#define kErrorIrqInPolling                ((canuint8)0xc4)
#define kErrorIntPndMissing               ((canuint8)0xc5)
#define kErrorRxFullCanIrq                ((canuint8)0xc6)
#define kErrorCanSleep                    ((canuint8)0xc7)
#define kErrorCanOnline                   ((canuint8)0xc8)
#define kErrorCanStop                     ((canuint8)0xc9)
#define kErrorRxHandleWrong               ((canuint8)0xcA)        

#define kErrorInconsistentDynObjTable     ((canuint8)0xd1)
#define kErrorReleasedUnusedDynObj        ((canuint8)0xd2)
#define kErrorReleasedInvalidDynObj       ((canuint8)0xd3)
#define kErrorTransmittedInvalidDynObj    ((canuint8)0xd4)    /* not used any more */
#define kErrorIllObjectConfig             ((canuint8)0xd5)  /* CAN object not configured correctly */

/* KB begin CanLL_ErrorCodesInternalH */
/* error numbers for Internal Assertions 0xe0-0xff - hardware depended */
#define kErrorWrongInterruptLine      ((vuint8)0xe0) /* CAN interrupts are mapped to the wrong interrupt line */
#define kErrorMemAlignment            ((vuint8)0xe1) /* computed memory alignment not correct */

/* KB end CanLL_ErrorCodesInternalH */

/***************************************************************************/
/* DummyFunction                                                           */
/***************************************************************************/
#define CAN_RECEIVE_FCT_DUMMY             ApplCanRxStructPtrDummy
#define CAN_RANGE_FCT_DUMMY               ApplCanRxStructPtrDummy
#define CAN_BUSOFF_FCT_DUMMY              ApplCanChannelDummy
#if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
# define CAN_INDICATION_FCT_DUMMY         ApplCanRxHandleDummy
# define CAN_PRECOPY_FCT_DUMMY            ApplCanRxStructPtrDummy
# define CAN_CONFIRMATION_FCT_DUMMY       ApplCanTxHandleDummy
# define CAN_PRETRANSMIT_FCT_DUMMY        ApplCanTxStructDummy
#else
# define CAN_INDICATION_FCT_DUMMY         NULL
# define CAN_PRECOPY_FCT_DUMMY            NULL
# define CAN_CONFIRMATION_FCT_DUMMY       NULL
# define CAN_PRETRANSMIT_FCT_DUMMY        NULL
#endif

#if defined( C_NO_COMPATIBILITY )
#else
/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/

# define CAN_TX_DLC(i)               CanGetTxDlc(i)             
# define CAN_TX_DATA_PTR(i)          CanGetTxDataPtr(i)         
# define CAN_CONFIRMATION_OFFSET(i)  CanGetConfirmationOffset(i)
# define CAN_CONFIRMATION_MASK(i)    CanGetConfirmationMask(i)  

# define CAN_RX_DATA_LEN(i)          CanGetRxDataLen(i)         
# define CAN_RX_DATA_PTR(i)          CanGetRxDataPtr(i)         
# define CAN_INDICATION_OFFSET(i)    CanGetIndicationOffset(i)  
# define CAN_INDICATION_MASK(i)      CanGetIndicationMask(i)   

# define CanInterruptDisable()       CanGlobalInterruptDisable()
# define CanInterruptRestore()       CanGlobalInterruptRestore()

# define ApplCanReceiveDummy0        ApplCanRxStructPtrDummy
# define ApplCanReceiveDummy1        ApplCanChannelDummy

# define CanGetTxDirectMsg           CanGetTxHwObj

# define canRxDlcRam                 canVariableRxDataLen

/* KB begin CanLL_CompatibilityDefinesH */

/* KB end CanLL_CompatibilityDefinesH */

/***************************************************************************/
/* end of compatibility defines                                            */
/***************************************************************************/
#endif /* C_NO_COMPATIBILITY */

/***************************************************************************/
/* Memory qualifier                                                        */
/***************************************************************************/
/* KB begin CanLL_MemoryQualifier */
/* memory qualifier for the CAN contoller registers */
#define MEMORY_CAN

/* memory qualifier for bit variables */
#define MEMORY_BIT

/***************************************************************************/
/* return values                                                           */
/***************************************************************************/
/* KB end CanLL_MemoryQualifier */

/***************************************************************************/
/* data types                                                              */
/***************************************************************************/

/* KB begin CanLL_DataTypesH */
/*lint -e659 name of datatype is not in the same line as closing brace */
/* data types for driver function parameters */
/* the types of CanInitHandle, CanChannelHandle and CanObjectHandle depends
   on the compiler and controller - what generates effective code */
typedef unsigned short    CanInitHandle;
typedef unsigned short    CanChannelHandle;
/* CanObjectHandle has to be unsigned! */
typedef unsigned short    CanObjectHandle;

/* the types of CanTransmitHandle and CanReceiveHandle depends
   on the compiler and controller - what generates effective code; 
   but also how many messages have to be handled. */
typedef unsigned short    CanTransmitHandle;
typedef unsigned short    CanReceiveHandle;

 /* ptr to msgObj & data on chip used in precopy function */
typedef volatile unsigned char MEMORY_CAN *  CanChipDataPtr;
typedef volatile unsigned char MEMORY_CAN *  CanChipMsgPtr;


#if defined(C_COMP_TI_TMS470)  
typedef vuint32 tCanRegType;
#endif


/* up to 5 tables can be defined for the transmission ID. The type and how 
   many tables are needed depends on the CPU bus type and on the ID registers 
   of the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCanRxId0 of type unsigned short. The 
   same CPU with 5 byte ID registers for extended ID needs in this case 3 
   tables and tCanRxId0 = unsigned short, tCanRxId1 = unsigned short and
   tCanRxId2 = unsigned char. */


#if defined(C_COMP_TI_TMS470)  
typedef vuint32 tCanTxId0;
#endif


/* type of variable for ID type kCanIdTypeExt or kCanIdTypeStd will be assigned to 
   a variable of this type */
typedef unsigned char    tCanIdType;

/* up to 3 tables can be defined for the reception ID. The type and how many
   tables are needed depends on the CPU bus type and on the ID registers of
   the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCanRxId0 of type unsigned short. The 
   same CPU with 5 byte ID registers for extended ID needs in this case 3 
   tables and tCanRxId0 = unsigned short, tCanRxId1 = unsigned short and
   tCanRxId2 = unsigned char. */

#if defined(C_COMP_TI_TMS470)  
typedef vuint32 tCanRxId0;
#endif


/* type of the variable, in which the old status of the interrupt flag or
   level is stored */


typedef vuint32 tCanHLIntOld;

/* this type could also be a struct if more than one variable is necessary */
typedef vuint32 tCanLLCanIntOld;

/* stucture of the CAN message object used by CanMsgTransmit() */

#if defined(C_COMP_TI_TMS470)  
typedef struct
{
  vuint16 IdRaw;        
  vuint16 IdRawExt;
  vuint16 reserved1;
  vuint8  reserved2;
  vuint8  DlcRaw;
  vuint8  DataFld[8];
} 
tCanMsgTransmitStruct;
#endif
/* structure for temporary storage of a CAN message */

typedef struct 
{
  canbittype w0l : 8;
  canbittype w0h : 8;
  canbittype w1l : 8;
  canbittype w1h : 8;
  canbittype w2l : 8;
  canbittype w2h : 8;
  canbittype w3l : 8;
  canbittype w3h : 8;
}
tCanTempObj;

typedef struct 
{
  vuint32 CanMdl;
  vuint32 CanMdh;
}
tCanMsgBuffer;

typedef union
{
  tCanTempObj   bytes;
  tCanMsgBuffer dwords;
}
tCanMsgBufferUnion;

/* control and status register block */
typedef volatile struct
{
  vuint32          CanMid;
  vuint32          CanMcf;
  tCanMsgBufferUnion CanMsg;
}
tCanMailBox;

typedef volatile union
{ 
  tCanMailBox s;
  vuint16   a[8];
}
tCanTempPreTransmitObj;   

typedef struct
{
  canbittype lobyte : 8;
  canbittype hibyte : 8;
}
tWordToByte;

/*lint -esym(750,CanTrr,CanAa,CanRfp,CanReserved_0,CanTec,CanRec,CanGif1,CanLnt,CanToc,CanTos,CanReserved_1,CanMots,CanMoto) */

#if defined(C_COMP_TI_TMS470)  

typedef volatile struct
{
  tCanRegType CanMe; 
  tCanRegType CanMd;
  tCanRegType CanTrs;
  tCanRegType CanTrr;
  tCanRegType CanTa;
  tCanRegType CanAa;
  tCanRegType CanRmp;
  tCanRegType CanRml;
  tCanRegType CanRfp;
  tCanRegType CanGam;
  tCanRegType CanMc;
  tCanRegType CanBtc;
  tCanRegType CanEs;
  tCanRegType CanTec;
  tCanRegType CanRec;
  tCanRegType CanGif0;
  tCanRegType CanGim;
  tCanRegType CanGif1;
  tCanRegType CanMim;
  tCanRegType CanMil;
  tCanRegType CanOpc;
  tCanRegType CanTioc;
  tCanRegType CanRioc;
  tCanRegType CanLnt;
  tCanRegType CanToc;
  tCanRegType CanTos;
  tCanRegType CanReserved_1[6];
  tCanRegType CanLam[32];
  tCanRegType CanMots[32];
  tCanRegType CanMoto[32];
}
tCanCntrlRegBlock;
#endif



/*lint -e659 name of datatype is not in the same line as closing brace */
/* KB end CanLL_DataTypesH */

typedef struct 
{
  CanChipDataPtr     pChipData;
  CanTransmitHandle  Handle;
/* CPU-specific part */
  /* KB begin CanLL_tCanTxInfoStruct */
  /* KB end CanLL_tCanTxInfoStruct */
} tCanTxInfoStruct;

/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
typedef struct 
{
  CanChannelHandle   Channel;
  CanTransmitHandle  Handle;
/* CPU-specific part */
  /* KB begin CanLL_tCanTxConfInfoStruct */
  /* KB end CanLL_tCanTxConfInfoStruct */
} tCanTxConfInfoStruct;

typedef struct 
{
  CanChannelHandle  Channel;
  CanChipMsgPtr     pChipMsgObj;
  CanChipDataPtr    pChipData;
  CanReceiveHandle  Handle;
  canuint8          EcuNumber;
/* CPU-specific part */
  /* KB begin CanLL_tCanRxInfoStruct */
  /* KB end CanLL_tCanRxInfoStruct */
} tCanRxInfoStruct;

typedef tCanRxInfoStruct          *CanRxInfoStructPtr;
typedef tCanTxInfoStruct          CanTxInfoStruct;
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
typedef tCanTxConfInfoStruct      *CanTxInfoStructPtr;

/* types of application functions called by driver ---------------------------*/
typedef C_CALLBACK_1 canuint8 (C_CALLBACK_2 *ApplCanMsgRcvFct)    (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 canuint8 (C_CALLBACK_2 *ApplRangeFct)        (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 canuint8 (C_CALLBACK_2 *ApplPrecopyFct)      (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplIndicationFct)   (CanReceiveHandle rxObject);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplConfirmationFct) (CanTransmitHandle txObject);
typedef C_CALLBACK_1 canuint8 (C_CALLBACK_2 *ApplPreTransmitFct)  (CanTxInfoStruct txStruct);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplChannelFct)      (CAN_CHANNEL_CANTYPE_ONLY);

typedef struct
{
  ApplChannelFct      ApplCanWakeUpFct; 
  ApplCanMsgRcvFct    ApplCanMsgReceivedFct;
  ApplRangeFct        ApplCanRangeFct[4];
  ApplChannelFct      ApplCanBusOffFct;
  ApplConfirmationFct ApplCanCancelNotificationFct;
  ApplChannelFct      ApplCanMsgTransmitCancelNotifyFct;
  canuint32           RangeMask[4][kCanNumberOfUsedCanRxIdTables];
  canuint32           RangeCode[4][kCanNumberOfUsedCanRxIdTables];
  canuint16           RangeActiveFlag;
} tCanChannelObject;



/* datatypes for indication and confirmation flags */
#if defined ( C_ENABLE_CONFIRMATION_FLAG )
union CanConfirmationBits 
{
  canuint8         _c[kCanNumberOfConfBytes];
# if defined ( V_CPUTYPE_BITARRAY_32BIT )
#  if kCanNumberOfConfFlags > 16
  struct _c_bits32 w[kCanNumberOfConfDWords];
#  elif kCanNumberOfConfFlags > 8
  struct _c_bits16 w[kCanNumberOfConfWords];
#  else
  struct _c_bits8  w[1];
#  endif
# else
#  if kCanNumberOfConfFlags > 8
  struct _c_bits16 w[kCanNumberOfConfWords];
#  else
  struct _c_bits8  w[1];
#  endif
# endif
}; 
#endif


#if defined ( C_ENABLE_INDICATION_FLAG )
union CanIndicationBits 
{
  canuint8         _c[kCanNumberOfIndBytes];
# if defined ( V_CPUTYPE_BITARRAY_32BIT )
#  if kCanNumberOfIndFlags > 16
  struct _c_bits32 w[kCanNumberOfIndDWords];
#  elif kCanNumberOfIndFlags > 8
  struct _c_bits16 w[kCanNumberOfIndWords];
#  else
  struct _c_bits8  w[1];
#  endif
# else
#  if kCanNumberOfIndFlags > 8
  struct _c_bits16 w[kCanNumberOfIndWords];
#  else
  struct _c_bits8 w[1];
#  endif
# endif
};
#endif

/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/

/*  CAN driver version */
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kCanMainVersion;
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kCanSubVersion;
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kCanBugFixVersion;

extern tCanRxInfoStruct          canRxInfoStruct[kCanNumberOfChannels];

#if defined( C_ENABLE_PRECOPY_FCT )
extern volatile CanReceiveHandle canRxHandle[kCanNumberOfChannels];
#endif

#ifdef C_ENABLE_CONFIRMATION_FLAG
extern volatile union CanConfirmationBits MEMORY_NEAR  CanConfirmationFlags;
#endif

#if defined( C_ENABLE_INDICATION_FLAG )
extern volatile union CanIndicationBits   MEMORY_NEAR  CanIndicationFlags;
#endif


#if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined(  C_ENABLE_DYN_TX_OBJECTS )  && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
extern CanTransmitHandle          confirmHandle[kCanNumberOfChannels];
#endif

/* ##RI1.4 - 3.31: Dynamic Receive DLC */
#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
# if ( kCanNumberOfRxObjects > 0 )
extern volatile canuint8 canVariableRxDataLen[kCanNumberOfRxObjects];
# endif
#endif

/* KB begin CanLL_ExternDeclarationH */
#if defined(C_COMP_TI_TMS470)  
extern tCanTempObj* canRDSRxPtr[kCanNumberOfChannels];
extern tCanTempObj* canRDSTxPtr[kCanNumberOfUsedTxCANObjects];
#endif

#if defined(C_SINGLE_RECEIVE_CHANNEL)
#else
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanMailboxBasisAdr[kCanNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanBasisAdr[kCanNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanRxFullCANObjMask[kCanNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanControllerMode[kCanNumberOfChannels];
#endif

/* KB end CanLL_ExternDeclarationH */

/***************************************************************************/
/* Data structures filled with data by parameter generating tool           */
/***************************************************************************/

/* structures for init and common objects -----------------------*/
#if defined (C_SINGLE_RECEIVE_CHANNEL )
#else
V_MEMROM0 extern V_MEMROM1 tCanChannelObject V_MEMROM2 CanChannelObject[kCanNumberOfChannels];

V_MEMROM0 extern V_MEMROM1 canuint8          V_MEMROM2 CanInitObjectStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 CanTxStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 CanTxStatStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 CanTxDynRomStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 CanTxDynRamStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle  V_MEMROM2 CanRxStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle  V_MEMROM2 CanRxBasicStartIndex[kCanNumberOfChannels+1];
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle  V_MEMROM2 CanRxFullStartIndex[kCanNumberOfHwChannels+1];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle   V_MEMROM2 CanLogHwTxStartIndex[kCanNumberOfHwChannels+1];

V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwTxStartIndex[kCanNumberOfHwChannels];
/*extern CanObjectHandle    MEMORY_ROM CanHwTxDirectStartIndex[kCanNumberOfChannels];*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwRxFullStartIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwRxBasicStartIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwUnusedStartIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwTxStopIndex[kCanNumberOfHwChannels];
/*extern CanObjectHandle    MEMORY_ROM CanHwTxDirectStopIndex[kCanNumberOfChannels];*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwRxFullStopIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwRxBasicStopIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwUnusedStopIndex[kCanNumberOfHwChannels];

V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwMsgTransmitIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 CanObjectHandle    V_MEMROM2 CanHwTxNormalIndex[kCanNumberOfHwChannels];
V_MEMROM0 extern V_MEMROM1 vsintx             V_MEMROM2 CanTxOffsetHwToLog[kCanNumberOfHwChannels];

# if ( kCanNumberOfTxObjects > 0 )
V_MEMROM0 extern V_MEMROM1 CanChannelHandle   V_MEMROM2 CanTxMsgHandleToChannel[kCanNumberOfTxObjects];
# endif

#if defined ( C_ENABLE_MULTI_ECU_CONFIG )
V_MEMROMO extern V_MEMROM1 tVIdentity         V_MEMROM2 CanChannelIdentityAssignment[kCanNumberOfChannels];
# endif
#endif



/* ROM generated */
                                        

# if ( kCanNumberOfTxObjects > 0 )
#  if defined( C_ENABLE_CAN_TRANSMIT )
/* structures for transmit objects -----------------------*/
V_MEMROM0 extern V_MEMROM1 tCanTxId0 V_MEMROM2       CanTxId0[kCanNumberOfTxObjects];  
#   if (kCanNumberOfUsedCanTxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanTxId1 V_MEMROM2       CanTxId1[kCanNumberOfTxObjects];  
#   endif
#   if (kCanNumberOfUsedCanTxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanTxId2 V_MEMROM2       CanTxId2[kCanNumberOfTxObjects];  
#   endif
#   if (kCanNumberOfUsedCanTxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanTxId3 V_MEMROM2       CanTxId3[kCanNumberOfTxObjects];  
#   endif
#   if (kCanNumberOfUsedCanTxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanTxId4 V_MEMROM2       CanTxId4[kCanNumberOfTxObjects];  
#   endif

#   if defined( C_ENABLE_MIXED_ID )
#    if defined(C_HL_ENABLE_IDTYPE_OWN_REG)
V_MEMROM0 extern V_MEMROM1 tCanIdType V_MEMROM2      CanTxIdType[kCanNumberOfTxObjects];
#    endif
#   endif

V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2        CanTxDLC[kCanNumberOfTxObjects];
V_MEMROM0 extern V_MEMROM1 TxDataPtr V_MEMROM2       CanTxDataPtr[kCanNumberOfTxObjects];

#   if defined( C_ENABLE_PART_OFFLINE )
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2        CanTxSendMask[kCanNumberOfTxObjects];
#   endif

#   if defined( C_ENABLE_CONFIRMATION_FLAG )
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2        CanConfirmationOffset[kCanNumberOfTxObjects];
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2        CanConfirmationMask[kCanNumberOfTxObjects];
#   endif
#   if defined( C_ENABLE_CONFIRMATION_FCT )
V_MEMROM0 extern V_MEMROM1 ApplConfirmationFct V_MEMROM2 CanTxApplConfirmationPtr[kCanNumberOfTxObjects];
#   endif
#   if defined( C_ENABLE_PRETRANSMIT_FCT )
V_MEMROM0 extern V_MEMROM1 ApplPreTransmitFct  V_MEMROM2 CanTxApplPreTransmitPtr[kCanNumberOfTxObjects];
#   endif

#   if defined( C_ENABLE_TX_FULLCAN_OBJECTS )
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2         CanTxHwObj[kCanNumberOfTxObjects];
#   endif
#   if defined(C_ENABLE_TX_DIRECT_OBJECTS)
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2         CanTxDirectTransmitSupport[kCanNumberOfUsedTxCANObjects];
#   endif

#  endif /* C_ENABLE_CAN_TRANSMIT */

#  if defined ( C_ENABLE_MULTI_ECU_PHYS )
V_MEMROMO extern V_MEMROM1 tVIdentity V_MEMROM2           CanTxIdentityAssignment[kCanNumberOfTxObjects];
#  endif

# endif /* ( kCanNumberOfTxObjects > 0 ) */

/* structures for basic and full can receive objects -----------------------*/

# if ( kCanNumberOfRxObjects > 0 )
#  if defined( C_SEARCH_HASH )
#   if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
V_MEMROM0 extern V_MEMROM1 tCanRxId0 V_MEMROM2       CanRxId0[kCanNumberOfRxFullCANObjects];  
#    if (kCanNumberOfUsedCanRxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanRxId1 V_MEMROM2       CanRxId1[kCanNumberOfRxFullCANObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanRxId2 V_MEMROM2       CanRxId2[kCanNumberOfRxFullCANObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanRxId3 V_MEMROM2       CanRxId3[kCanNumberOfRxFullCANObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanRxId4 V_MEMROM2       CanRxId4[kCanNumberOfRxFullCANObjects];  
#    endif

#    if defined( C_ENABLE_MIXED_ID )
#     if defined(C_HL_ENABLE_IDTYPE_IN_ID)
#     else
V_MEMROM0 extern V_MEMROM1 tCanIdType V_MEMROM2           CanRxIdType[kCanNumberOfRxFullCANObjects];
#     endif
#    endif
#   endif /* C_ENABLE_RX_FULLCAN_OBJECTS */
#  else   /* C_SEARCH_HASH */
V_MEMROM0 extern V_MEMROM1 tCanRxId0 V_MEMROM2       CanRxId0[kCanNumberOfRxObjects];  
#   if (kCanNumberOfUsedCanRxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanRxId1 V_MEMROM2       CanRxId1[kCanNumberOfRxObjects];  
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanRxId2 V_MEMROM2       CanRxId2[kCanNumberOfRxObjects];  
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanRxId3 V_MEMROM2       CanRxId3[kCanNumberOfRxObjects];  
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanRxId4 V_MEMROM2       CanRxId4[kCanNumberOfRxObjects];  
#   endif

#   if defined( C_ENABLE_MIXED_ID )
#    if defined(C_HL_ENABLE_IDTYPE_IN_ID)
#    else
V_MEMROM0 extern V_MEMROM1 tCanIdType V_MEMROM2           CanRxIdType[kCanNumberOfRxObjects];
#    endif
#   endif
#  endif  /* C_SEARCH_HASH */

V_MEMROM0 extern V_MEMROM1 canuint8             V_MEMROM2 CanRxDataLen[kCanNumberOfRxObjects];
V_MEMROM0 extern V_MEMROM1 RxDataPtr            V_MEMROM2 CanRxDataPtr[kCanNumberOfRxObjects];

#  if defined( C_ENABLE_PRECOPY_FCT )
V_MEMROM0 extern V_MEMROM1 ApplPrecopyFct       V_MEMROM2 CanRxApplPrecopyPtr[kCanNumberOfRxObjects];
#  endif
#  if defined( C_ENABLE_INDICATION_FLAG )
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2             CanIndicationOffset[kCanNumberOfRxObjects];
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2             CanIndicationMask[kCanNumberOfRxObjects];
#  endif
#  if defined( C_ENABLE_INDICATION_FCT )
V_MEMROM0 extern V_MEMROM1 ApplIndicationFct    V_MEMROM2 CanRxApplIndicationPtr[kCanNumberOfRxObjects];
#  endif
# endif /* ( kCanNumberOfRxObjects > 0 ) */

#if ( kCanNumberOfRxObjects > 0 )
# if defined( C_SEARCH_HASH )
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2     CanRxMsgIndirection[kHashSearchListCount + kHashSearchListCountEx + kCanNumberOfRxFullCANObjects];
# else
#  if defined( C_ENABLE_RX_MSG_INDIRECTION )
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2     CanRxMsgIndirection[kCanNumberOfRxObjects];
#  endif
# endif

# if defined( C_SEARCH_HASH )
#  if defined( C_ENABLE_EXTENDED_ID )
#    if( kHashSearchListCountEx > 0)
V_MEMROM0 extern V_MEMROM1 canuint32 V_MEMROM2            CanRxHashIdEx[kHashSearchListCountEx];
#    endif /* (kHashSearchListCountEx > 0) */
#   if defined( C_ENABLE_MIXED_ID )
#    if( kHashSearchListCount > 0)
V_MEMROM0 extern V_MEMROM1 canuint16 V_MEMROM2            CanRxHashId[kHashSearchListCount];
#    endif
#   endif
#  else
#   if( kHashSearchListCount > 0)
V_MEMROM0 extern V_MEMROM1 canuint16 V_MEMROM2            CanRxHashId[kHashSearchListCount];
#   endif 
#  endif
# endif

# if defined ( C_ENABLE_MULTI_ECU_PHYS )
V_MEMROMO extern V_MEMROM1 tVIdentity V_MEMROM2           CanRxIdentityAssignment[kCanNumberOfRxObjects];
# endif

#endif /* ( kCanNumberOfRxObjects > 0 ) */

/* KB begin CanLL_GeneratedStructureH */
/* structures for baudrate and filter initialization */
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanInitBTC[];
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanInitLAM0[];  
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanInitMID1[]; 
/* KB end CanLL_GeneratedStructureH */

/***************************************************************************/
/* Callback functions                                                      */
/***************************************************************************/

#if defined( C_ENABLE_INTCTRL_BY_APPL )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanInterruptDisable(void);
C_CALLBACK_1 void C_CALLBACK_2 ApplCanInterruptRestore(void);
#endif

#if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanAddCanInterruptRestore(CanChannelHandle channel);
C_CALLBACK_1 void C_CALLBACK_2 ApplCanAddCanInterruptDisable(CanChannelHandle channel);
#endif


#if defined ( C_ENABLE_HARDWARE_CHECK ) || \
    defined ( C_ENABLE_USER_CHECK )     || \
    defined ( C_ENABLE_GEN_CHECK )      || \
    defined ( C_ENABLE_INTERNAL_CHECK ) 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanFatalError(CAN_CHANNEL_CANTYPE_FIRST canuint8 errorNumber);
#endif

#if defined ( C_SINGLE_RECEIVE_CHANNEL )
/* these two functions are declared by the tool in case of multichannel */
C_CALLBACK_1 void C_CALLBACK_2 ApplCanBusOff(CAN_CHANNEL_CANTYPE_ONLY);
C_CALLBACK_1 void C_CALLBACK_2 ApplCanWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
#endif
#if defined(C_ENABLE_APPLCANPREWAKEUP_FCT)
C_CALLBACK_1 void C_CALLBACK_2 ApplCanPreWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
#endif
#if defined( C_ENABLE_OVERRUN )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanOverrun ( CAN_CHANNEL_CANTYPE_ONLY );
#endif
#if defined( C_ENABLE_FULLCAN_OVERRUN )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanFullCanOverrun ( CAN_CHANNEL_CANTYPE_ONLY );
#endif

/* Range precopy/ generic precopy functions ********************************/
#if defined ( C_SINGLE_RECEIVE_CHANNEL )
  #if defined ( C_ENABLE_RANGE_0 )
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanRange0Precopy( CanRxInfoStructPtr rxStruct );
  #endif
  #if defined ( C_ENABLE_RANGE_1 )
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanRange1Precopy( CanRxInfoStructPtr rxStruct );
  #endif
  #if defined ( C_ENABLE_RANGE_2 )
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanRange2Precopy( CanRxInfoStructPtr rxStruct );
  #endif
  #if defined ( C_ENABLE_RANGE_3 )
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanRange3Precopy( CanRxInfoStructPtr rxStruct );
  #endif
#endif

#if defined (C_ENABLE_COND_RECEIVE_FCT)
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgCondReceived  ( CanRxInfoStructPtr rxStruct );
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
# if defined(C_ENABLE_NOTIFY_CORRUPT_MAILBOX)
V_CALLBACK_1 void V_CALLBACK_2 ApplCanCorruptMailbox(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle);
# endif
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanMemCheckFailed(CAN_CHANNEL_CANTYPE_ONLY);
#endif

#if defined(C_SINGLE_RECEIVE_CHANNEL)
# if defined(C_ENABLE_RECEIVE_FCT)
/* in case of multiple CAN channels this function is declared by the tool */
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanMsgReceived(CanRxInfoStructPtr rxStruct);
# endif
#endif

/* ##RI1.4 - 2.7: Callbackfunction-DLC-Check */
#if defined( C_ENABLE_DLC_FAILED_FCT )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgDlcFailed( CanRxInfoStructPtr rxStruct ); 
#endif  /*C_ENABLE_DLC_FAILED_FCT */

#if defined( C_ENABLE_GENERIC_PRECOPY )
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanGenericPrecopy( CanRxInfoStructPtr rxStruct );
#endif

#if defined ( C_ENABLE_NOT_MATCHED_FCT )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgNotMatched( CanRxInfoStructPtr rxStruct );
#endif

#if defined ( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxConfirmation(CanTxInfoStructPtr txStruct);
#endif

#if defined( C_ENABLE_TX_OBSERVE )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxObjStart( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObject );
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxObjConfirmed( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObject );
C_CALLBACK_1 void C_CALLBACK_2 ApplCanInit( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObjectFirstUsed, CanObjectHandle txHwObjectFirstUnused);
#endif

#if defined( C_ENABLE_HW_LOOP_TIMER )
# if defined(C_HL_ENABLE_HW_LOOP_TIMER_CHANNEL_SPECIFIC)
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerStart(CAN_CHANNEL_CANTYPE_FIRST canuint8 source);
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanTimerLoop (CAN_CHANNEL_CANTYPE_FIRST canuint8 source);
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerEnd  (CAN_CHANNEL_CANTYPE_FIRST canuint8 source);
# else
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerStart(canuint8 source);
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanTimerLoop (canuint8 source);
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerEnd  (canuint8 source);
# endif
#endif

#if defined(C_ENABLE_MSG_TRANSMIT_CONF_FCT)
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgTransmitConf(CAN_CHANNEL_CANTYPE_ONLY);
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgTransmitInit(CAN_CHANNEL_CANTYPE_ONLY);
#endif

#if defined(C_SINGLE_RECEIVE_CHANNEL)
/* in case of multichannel these two functions are declared by the tool */
# if defined(C_ENABLE_CAN_CANCEL_NOTIFICATION)
C_CALLBACK_1 void C_CALLBACK_2 ApplCanCancelNotification( CanTransmitHandle txHandle );
# endif
# if defined(C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION)
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgCancelNotification( CAN_CHANNEL_CANTYPE_ONLY );
# endif
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanChannelDummy( CanChannelHandle channel );
#endif
#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) || \
    defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxHandleDummy( CanTransmitHandle txHandle );
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanRxStructPtrDummy( CanRxInfoStructPtr rxStruct );
#endif
#if defined ( C_HL_ENABLE_DUMMY_FCT_CALL )
C_CALLBACK_1 void C_CALLBACK_2 ApplCanRxHandleDummy( CanReceiveHandle rxHandle );
C_CALLBACK_1 canuint8 C_CALLBACK_2 ApplCanTxStructDummy( CanTxInfoStruct txStruct );
#endif

/* KB begin CanLL_CallBackFunctionsH */
/* KB end CanLL_CallBackFunctionsH */

/***************************************************************************/
/* function prototypes                                                     */
/***************************************************************************/


C_API_1 void C_API_2 CanInitPowerOn( void );
C_API_1 void C_API_2 CanInit( CAN_CHANNEL_CANTYPE_FIRST CanInitHandle initObject );

#if defined( C_ENABLE_CAN_TRANSMIT )
C_API_1 canuint8 C_API_2 CanTransmit(CanTransmitHandle txHandle) C_API_3;
#if defined( C_ENABLE_VARIABLE_DLC )
C_API_1 canuint8 C_API_2 CanTransmitVarDLC( CanTransmitHandle txHandle, canuint8 dlc);
#endif
C_API_1 void C_API_2 CanCancelTransmit( CanTransmitHandle txHandle );
#endif

#ifdef C_ENABLE_MSG_TRANSMIT
C_API_1 canuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST tCanMsgTransmitStruct *txMsgStruct);
# ifdef C_ENABLE_MSG_TRANSMIT_CONF_FCT
C_API_1 void C_API_2 CanCancelMsgTransmit( CAN_CHANNEL_CANTYPE_ONLY );
# endif
#endif

#if defined(C_ENABLE_TX_DIRECT_OBJECTS)
C_API_1 canuint8 C_API_2 CanDirectTransmit(CanTransmitHandle txHandle);
C_API_1 canuint8 C_API_2 CanDirectTxCopyData(CanTransmitHandle txHandle, TxDataPtr *canMemCopySrcPtr);
C_API_1 canuint8 C_API_2 CanDirectTxGetStatus(CanTransmitHandle txHandle);
#endif

#if defined ( C_ENABLE_OFFLINE )
C_API_1 void C_API_2 CanOnline( CAN_CHANNEL_CANTYPE_ONLY );           /* switch CanStatus to On */
C_API_1 void C_API_2 CanOffline( CAN_CHANNEL_CANTYPE_ONLY );
#endif

#if defined ( C_ENABLE_PART_OFFLINE  )
C_API_1 void C_API_2 CanSetPartOffline( CAN_CHANNEL_CANTYPE_FIRST canuint8 sendGroup);
C_API_1 void C_API_2 CanSetPartOnline( CAN_CHANNEL_CANTYPE_FIRST canuint8 invSendGroup);
C_API_1 canuint8 C_API_2 CanGetPartMode( CAN_CHANNEL_CANTYPE_ONLY );
#endif

C_API_1 canuint8 C_API_2 CanGetStatus( CAN_CHANNEL_CANTYPE_ONLY );

/* CanSleep functions */
C_API_1 canuint8 C_API_2 CanSleep( CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 canuint8 C_API_2 CanWakeUp( CAN_CHANNEL_CANTYPE_ONLY );

#if defined( C_ENABLE_ECU_SWITCH_PASS )
C_API_1 void C_API_2 CanSetActive(  CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 void C_API_2 CanSetPassive( CAN_CHANNEL_CANTYPE_ONLY );
#else
  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
#define    CanSetActive( channel )               /* remove calls to setActive/Passive*/
#define    CanSetPassive( channel )
  #else
#define    CanSetActive( )               /* remove calls to setActive/Passive*/
#define    CanSetPassive( )
  #endif
#endif

#if defined ( C_ENABLE_STOP )
C_API_1 canuint8 C_API_2 CanStart( CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 canuint8 C_API_2 CanStop( CAN_CHANNEL_CANTYPE_ONLY );
#endif

C_API_1 void C_API_2 CanGlobalInterruptDisable( void ) C_API_3;
C_API_1 void C_API_2 CanGlobalInterruptRestore( void ) C_API_3;

C_API_1 void C_API_2 CanCanInterruptDisable( CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 void C_API_2 CanCanInterruptRestore( CAN_CHANNEL_CANTYPE_ONLY );

#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_RX_FULLCAN_POLLING )  || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING ) || \
    defined( C_ENABLE_WAKEUP_POLLING )
C_API_1 void C_API_2 CanTask(void);
#endif
#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_CANCEL_IN_HW )
C_API_1 void C_API_2 CanTxTask( CAN_CHANNEL_CANTYPE_ONLY );
#endif
#if defined( C_ENABLE_ERROR_POLLING )
C_API_1 void C_API_2 CanErrorTask( CAN_CHANNEL_CANTYPE_ONLY );
#endif
#if defined( C_ENABLE_SLEEP_WAKEUP )
# if defined( C_ENABLE_WAKEUP_POLLING )
C_API_1 void C_API_2 CanWakeUpTask(CAN_CHANNEL_CANTYPE_ONLY);
# endif
#endif
#if defined ( C_ENABLE_RX_FULLCAN_POLLING )
C_API_1 void C_API_2 CanRxFullCANTask(CAN_CHANNEL_CANTYPE_ONLY);
#endif
#if defined ( C_ENABLE_RX_BASICCAN_POLLING )
C_API_1 void C_API_2 CanRxBasicCANTask(CAN_CHANNEL_CANTYPE_ONLY);
#endif


#if defined( C_ENABLE_DYN_TX_OBJECTS )
C_API_1 CanTransmitHandle C_API_2 CanGetDynTxObj(CanTransmitHandle txHandle );
C_API_1 CanTransmitHandle C_API_2 CanGetAnyDynTxObj( CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 canuint8          C_API_2 CanReleaseDynTxObj(CanTransmitHandle txHandle);

#if defined( C_ENABLE_DYN_TX_ID )
# if !defined( C_ENABLE_EXTENDED_ID ) ||\
     defined ( C_ENABLE_MIXED_ID )
C_API_1 void C_API_2 CanDynTxObjSetId(CanTransmitHandle txHandle, canuint16 id);
# endif
# if defined( C_ENABLE_EXTENDED_ID )
C_API_1 void C_API_2 CanDynTxObjSetExtId(CanTransmitHandle txHandle, canuint16 idExtHi, canuint16 idExtLo);
# endif
#endif
#if defined( C_ENABLE_DYN_TX_DLC )
C_API_1 void C_API_2 CanDynTxObjSetDlc(CanTransmitHandle txHandle, canuint8 dlc);
#endif
#if defined( C_ENABLE_DYN_TX_DATAPTR )
C_API_1 void C_API_2 CanDynTxObjSetDataPtr(CanTransmitHandle txHandle, void* pData);
#endif


#endif /* defined( C_ENABLE_DYN_TX_OBJECTS ) */

#if defined (C_ENABLE_TX_MASK_EXT_ID)
C_API_1 void C_API_2 CanSetTxIdExtHi( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask );
C_API_1 void C_API_2 CanSetTxIdExtMidHi( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask );
C_API_1 void C_API_2 CanSetTxIdExtMidLo( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask );
C_API_1 void C_API_2 CanSetTxIdExtLo( CAN_CHANNEL_CANTYPE_FIRST  canuint8 mask );
#endif

#if defined( C_ENABLE_TX_OBSERVE )
C_API_1 CanTransmitHandle C_API_2 CanTxGetActHandle( CanObjectHandle logicalTxHdl );
#endif

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
C_API_1 void C_API_2 CanSetVariableRxDatalen ( CanReceiveHandle rxHandle, canuint8 dataLen );
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
C_API_1 void     C_API_2 CanSetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 void     C_API_2 CanResetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
C_API_1 canuint8 C_API_2 CanGetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
#endif


/* KB begin CanLL_FunctionPrototypesH */


#if defined (C_ENABLE_MEMCOPY_SUPPORT)
# if defined(C_COMP_TI_TMS470) 
void CanCopyToCan(CanChipDataPtr dst, void * src, vuint8 len);
void CanCopyFromCan(void *dst, CanChipDataPtr src, vuint8 len);
# endif
#endif


/*lint +rw(interrupt) +fem interrupt __interrupt keyword*/
#if !defined(C_ENABLE_OSEK_OS)
# if !defined(C_ENABLE_TX_POLLING)          || \
     !defined(C_ENABLE_RX_FULLCAN_POLLING)  || \
     !defined(C_ENABLE_RX_BASICCAN_POLLING) || \
     !defined(C_ENABLE_ERROR_POLLING)       || \
     !defined(C_ENABLE_WAKEUP_POLLING)
#  if defined(C_COMP_TI_TMS470)  
CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Can0Interrupt(void);
#   if !defined(C_SINGLE_RECEIVE_CHANNEL)
CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Can1Interrupt(void);
#   endif
#  endif
# endif
#endif
/*lint -fem interrupt __interrupt keyword*/

/* KB end CanLL_FunctionPrototypesH */

#endif /* CAN_DEF_H */

/* End of channel */



/************   Organi, Version 3.2.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.0 Vector-Informatik GmbH  ************/
