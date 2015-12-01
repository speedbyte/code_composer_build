/**************************************************************
*  File C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\inc\nm_cfg.h
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
#ifndef NM_CFG_H
#define NM_CFG_H


#define GEN_MAGIC_NUMBER_NM_CFG 1303376549

#define NM_TYPE_VAG_OSEK


#define NM_DISABLE_INDEXED_NM
#define NM_DISABLE_CHANNEL_INDIRECTION
#define kNmNumberOfChannels 1
#define kNmEcuNr 26
#define kNmTxObj 14

#define NM_NUMBER_OF_CONFIG_NODES 32
#define NM_CONFIG_NOTUSED
#define NM_ENABLE_EXTENDED_CALLBACK
#define NM_BUS_DIAGNOSTICS_NOTUSED
#define NM_RINGDATA_ACCESS_NOTUSED
#define NM_COPY_RINGDATA_NOTUSED
#define NM_CAN_ERRORBIT_NOTUSED
#define NM_ENABLE_ERRORPIN_SEND
#define NM_BROADCAST_ACCESS_NOTUSED
#define NM_CALL_CYCLE 10
#define NM_COUNT_RING_TYP 4
#define NM_COUNT_RING_MAX 13
#define NM_COUNT_LIMPHOME 50
#define NM_COUNT_WAIT_BUSSLEEP 75
#define NM_TYPE_DBAG_USERDATA_ACCESS_NOTUSED
#define kNmCanPara 0
#define NM_NO_COMPATIBILITY
#define NM_DISABLE_MULTIPLE_NODES


/* NM_ENABLE_VAG_LAH_130 must be defined for 4 byte user data */
#define NM_DISABLE_VAG_LAH_130

/* NM_ENABLE_VAG_LAH_141 must be defined for 6 byte user data */
#define NM_ENABLE_VAG_LAH_141

/* NM_ENABLE_READ_USER_DATA can be defined for read access to user data */
#define NM_DISABLE_READ_USER_DATA

/* NM_ENABLE_SLEEPACK_CALLBACK can be defined for a call-back after sleep
   acknowledge */
#define NM_DISABLE_SLEEPACK_CALLBACK

/* NM_ENABLE_IMMEDIATE_ALIVE is used to send alive message immediately after
   sleep acknowledge, if own ECU becomes not ready to sleep after last ring
   message */
#define NM_ENABLE_IMMEDIATE_ALIVE

#endif
