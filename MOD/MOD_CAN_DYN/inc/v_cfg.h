/**************************************************************
*  File C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\inc\v_cfg.h
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
#ifndef VECTOR_CFG_H
#define VECTOR_CFG_H


#define GEN_MAGIC_NUMBER_V_CFG 1303376549


#define VGEN_CANGEN_VERSION 0x0415
#define VGEN_CANGEN_BUGFIX_VERSION 0x50

#define C_CLIENT_VAG


#define C_COMP_TI_TMS470
#define C_ENABLE_CAN_CHANNELS
#define C_VERSION_REF_IMPLEMENTATION 0x140
#define V_ENABLE_USE_DUMMY_STATEMENT
#define C_CPUTYPE_32BIT
#define C_CPUTYPE_BIGENDIAN
#define C_CPUTYPE_BITORDER_MSB2LSB
#define C_COMP_TI_470

/**************************************
*    Version defines for Ini Files    *
***************************************/

#define VGEN_VW_INI_VERSION 0x0100
#define VGEN_VW_INI_BUGFIXVERSION 0x00

/**************************************
*           used modules              *
***************************************/

#define VGEN_DISABLE_BRS
#define VGEN_ENABLE_CAN_DRV

/* NM Modules */

#define VGEN_ENABLE_NM_OSEK_D

/* TP Modules */

#define VGEN_ENABLE_TP_ISO_SC

/* Diagnose Modules */


#endif
