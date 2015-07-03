/**************************************************************
*  File C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\inc\can_cfg.h
*  generated at Thu Apr 21 11:02:30 2011
*             Toolversion:   415
*               Bussystem:   Can
*
*  generated out of CANdb:   C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\cfg\PQ35_46_ICAN_V3_6_9_F_20081104_ASR_V1_2.dbc
*            Manufacturer:   Volkswagen AG
*                for node:   UHV_NAR
*   Generation parameters:   Target system = TMS470
*                            Compiler      = Texas Instruments
*
* License information:       
*   -    Serialnumber:       CBD0500058
*   - Date of license:       26.7.2005
*
***************************************************************
Software is licensed for:    
Volkswagen AG
EESN/1
VAG
TMS470
Texas Instruments
**************************************************************/
#ifndef CAN_CFG_H
#define CAN_CFG_H


#define GEN_MAGIC_NUMBER_CAN_CFG 1303376549

#define C_DISABLE_CANCEL_IN_HW
#include "v_cfg.h"



/* begin Fileversion check */
#if GEN_MAGIC_NUMBER_V_CFG !=1303376549
 #error "The magic number of the generated header v_cfg.h is different! Check time and date of generated files!"
#endif

/* end of Fileversion check */


#define kCanNumberOfChannels 1
#define kCanNumberOfInitObjects 1
#define C_ENABLE_OSEK_OS
#define C_DISABLE_OSEK_OS_INTCTRL
#define C_DISABLE_MULTICHANNEL_API
#define C_ENABLE_RECEIVE_FCT
#define C_DISABLE_ECU_SWITCH_PASS
#define C_ENABLE_TRANSMIT_QUEUE
#define C_ENABLE_OVERRUN
#define C_DISABLE_CAN_TX_CONF_FCT
#define C_DISABLE_INTCTRL_BY_APPL
#define C_DISABLE_USER_CHECK
#define C_DISABLE_HARDWARE_CHECK
#define C_DISABLE_GEN_CHECK
#define C_DISABLE_INTERNAL_CHECK
#define C_DISABLE_RX_MSG_INDIRECTION
#define C_DISABLE_DYN_TX_OBJECTS
#define C_DISABLE_DYN_TX_ID
#define C_DISABLE_DYN_TX_DLC
#define C_DISABLE_DYN_TX_DATAPTR
#define C_DISABLE_DYN_TX_PRETRANS_FCT
#define C_DISABLE_DYN_TX_CONF_FCT
#define C_DISABLE_EXTENDED_ID
#define C_DISABLE_MIXED_ID
#define C_DISABLE_RANGE_EXTENDED_ID
#define C_DISABLE_EXTENDED_STATUS
#define C_DISABLE_TX_OBSERVE
#define C_DISABLE_HW_LOOP_TIMER
#define C_DISABLE_NOT_MATCHED_FCT
#define C_SECURITY_LEVEL 30
#define C_ENABLE_PART_OFFLINE
#define C_DISABLE_MSG_TRANSMIT
#define C_DISABLE_MSG_TRANSMIT_CONF_FCT
#define C_ENABLE_RX_BASICCAN_OBJECTS
#define C_ENABLE_RX_FULLCAN_OBJECTS
#define C_ENABLE_TX_FULLCAN_OBJECTS

#define C_ENABLE_RANGE_0
#define C_DISABLE_RANGE_1
#define C_DISABLE_RANGE_2
#define C_DISABLE_RANGE_3

#define C_RANGE0_ACC_MASK 0x3f
#define C_RANGE0_ACC_CODE 0x400

#define ApplCanRange0Precopy NmPrecopy

#define ApplCanBusOff NmCanError

#define C_DISABLE_MULTICAN
#define kCanNumberOfTxStatObjects 15

#define kCanNumberOfTxDynObjects 0

#define kCanNumberOfTxObjects (kCanNumberOfTxStatObjects+kCanNumberOfTxDynObjects)

#define kCanNumberOfRxFullCANObjects 21
#define kCanNumberOfRxBasicCANObjects 0

#define kCanNumberOfTxDirectObjects 1


#define kCanMsgTransmitObj 30

#define kCanNumberOfConfFlags 13
#define kCanNumberOfIndFlags  15

#define kCanNumberOfConfirmationFlags 2
#define kCanNumberOfIndicationFlags 2
#define kCanNumberOfRxObjects (kCanNumberOfRxFullCANObjects+kCanNumberOfRxBasicCANObjects)
#define kCanNumberOfUsedTxCANObjects 2
#define kCanNumberOfUsedRxBasicCANObjects 2
#define kCanNumberOfUsedCanTxIdTables 1
#define kCanNumberOfUsedCanRxIdTables 1
#define kCanNumberOfUnusedObjects 7
#define kCanHwTxStartIndex 30
#define kCanHwUnusedStartIndex 23
#define kCanHwRxFullStartIndex 2
#define kCanHwRxBasicStartIndex 0
#define kCanHwTxNormalIndex 30


#define C_ENABLE_CONFIRMATION_FLAG
#define C_ENABLE_INDICATION_FLAG
#define C_DISABLE_PRETRANSMIT_FCT
#define C_ENABLE_CONFIRMATION_FCT
#define C_ENABLE_INDICATION_FCT
#define C_ENABLE_PRECOPY_FCT
#define C_ENABLE_COPY_TX_DATA
#define C_ENABLE_COPY_RX_DATA
#define C_ENABLE_DLC_CHECK
#define C_DISABLE_VARIABLE_DLC
#define C_ENABLE_DLC_FAILED_FCT
#define C_DISABLE_VARIABLE_RX_DATALEN
#define C_DISABLE_GENERIC_PRECOPY

#define C_SEND_GRP_NONE 0x0
#define C_SEND_GRP_ALL 0xff
#define C_SEND_GRP_NM 0x1
#define C_SEND_GRP_TP 0x2
#define C_SEND_GRP_DIAG 0x4
#define C_SEND_GRP_IL 0x8
#define C_SEND_GRP_APP 0x10
#define C_SEND_GRP_USER1 0x20
#define C_SEND_GRP_USER2 0x40
#define C_SEND_GRP_USER3 0x80

/* CAN channel assignment */


#define C_SEARCH_LINEAR
#define C_DISABLE_TX_DIRECT_OBJECTS
#define kCanBasisAdr 0xfff7e800
#define kCanMailboxBasisAdr 0xfff7e400
#define kCanControllerMode kCanHECCMode
#define kCanRxFullCANObjMask	0x7ffffc
#define C_ENABLE_FULLCAN_OVERRUN
#define C_DISABLE_SLEEP_WAKEUP
#define C_ENABLE_INT_OSCAT2
#define C_ENABLE_RX_FULLCAN_OBJECTS
#define C_DISABLE_PRIVILEDGED_DRIVER
#define CanWriteProtectedRegister(a,m,v) WriteProtReg(a,m,v)
#define CanDisableIrq() DisableIrq()
#define CanEnableIrq() EnableIrq()
#define C_DISABLE_TX_POLLING
#define C_DISABLE_RX_FULLCAN_POLLING
#define C_DISABLE_RX_BASICCAN_POLLING
#define C_DISABLE_ERROR_POLLING
#define C_DISABLE_WAKEUP_POLLING

#define C_DISABLE_OSEK_OS_INTCTRL

#undef C_DISABLE_VARIABLE_DLC
#define C_ENABLE_VARIABLE_DLC


#if !defined(C_ENABLE_APPLCANPRERXQUEUE)
/* all messages handled in the FIFO-Queue */
# define ApplCanPreRxQueue(a)  (kCanCopyData)
# define C_DISABLE_APPLCANPRERXQUEUE
#endif

#endif
