/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: TP_CFG.H
 **************************************************************************
 *  PURPOSE:
 *  Header-File configuring the transport protocol functionality
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.05.2005 - VW/Audi conformance, first version 
 *  V_00_01_01 Ki 30.05.2005 - Adjustments due to the comments by Audi 
 *  V_00_01_02 Ki 03.06.2005 - Further adjustments 
 *  V_00_01_03 Ki 07.06.2005 - C++ comment eliminated (MISRA conformance)  
 *  V_00_01_04 Ki 22.07.2005 - Workaround for CANdrv confirmation  
 *  V_00_01_05 Ki 04.08.2005 - Bug-Fix: length calculation CF transmit
 *                           - Modification: Conf. within Req-Function   
 *  V_00_01_06 Ki 09.08.2005 - New: cub_MAX_NO_OF_WAITS
 *  V_00_01_07 Ki 19.09.2005 - New: TpCanTransmit
 *  V_00_01_08 Ki 05.12.2005 - revision
 *  V_01_00_00 Ki 18.01.2006 - New: ISO15765_4_ADDRESSING
 *  V_01_02_00 Ki 02.06.2006 - Multi channel capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - TP_CHANNELS moved into tp.h
 *  V_01_02_03 Ki 02.11.2006 - unchanged
 *  V_01_02_04 Ki 19.12.2006 - i2s_types.h included
 *                           - use of i2s type instead of C types
 *                           - TpCanTransmit deleted
 *                           - CAN_TX_ID, CAN_RX_ID deleted
 *                           - Check for memory allocation: _IN2S_TP_C
 *  V_01_02_05 Ki 12.01.2007 - include can_inc.h
 *  V_01_02_06 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_07 Ki 30.03.2007 - no change
 *  V_01_02_08 Ki 11.04.2007 - no change
 *  V_01_02_09 Ki 02.07.2007 - cub_MAX_NO_OF_WAITS deleted
 *  V_01_02_10 Ki 11.07.2007 - no change
 *  V_01_02_11 Ki 23.07.2007 - cub_ST_DEF = 5
 *  V_01_02_12 Ki 02.11.2007 - new: cub_SECOND_FUNC_ADDRESS
 *                           - new: cul_REQ_CAN_ID_11BIT_FUNC_2
 *  V_01_02_13 Ki 18.06.2008 - new: cub_BUSY_REPEAT_REQUEST
 *  V_01_02_14 Ki 23.07.2008 - no change
 *  V_01_02_15 Ki 08.12.2008 - no change
 *************************************************************************/

#ifndef _I2S_TP_CFG_H_
#define _I2S_TP_CFG_H_

/* -----------------------------------------------------------------------*/
/*  - C o n s t a n t s --------------------------------------------------*/
/* -----------------------------------------------------------------------*/
#define TP_CHANNELS                                 1  /* number of supported TP channels */
#define cuw_TASK_CYCLE                       (word)10  /* call cycle of the TP task [ms] */

#define cub_USE_PADDING                             1  /* 0 = don´t pad, all others = pad */ 
#define cub_PADDING_BYTE                   (byte)0xAA  /* fill value for unused bytes */
#define cub_BUSY_REPEAT_REQUEST                     1  /* 0 = don´t send, all others = send */

/* ------------- mixed/fixed addressing only ----------------------------------*/
/* following 4 values are normally given by the iso standard !!! */
/* target and source address (two LSB) given below are relevant for MIXED or FIXED mode */ 
#define cul_CAN_ID_FIXED_PHYS       (dword)0x18DA0000  /* physical FIXED CANid mask */
#define cul_CAN_ID_FIXED_FUNC       (dword)0x18DB0000  /* functional FIXED CANid mask */ 
#define cul_CAN_ID_MIXED_PHYS       (dword)0x18CE0000  /* physical MIXED CANid mask */
#define cul_CAN_ID_MIXED_FUNC       (dword)0x18CD0000  /* functional MIXED CANid mask */
/* ------------- END mixed/fixed addressing only ------------------------------*/

/* ------------- normal/extended addressing only ------------------------------*/
#define cul_REQ_CAN_ID_11BIT_FUNC   (dword)0x00000700  /* 1st functional rx can id; same for all channels */
#define cul_REQ_CAN_ID_11BIT_FUNC_2 (dword)0x00000703  /* 2nd functional rx can id; same for all channels  */ 
                                                       /* do not use 0x7DF (OBD can id) for functional 11bit id */
/* ------------- END normal/extended addressing only --------------------------*/

/* ------------- extended addressing only -------------------------------------*/
#define cub_TESTER_ADR                     (byte)0xF1  /* default tester address (TA, SA) */
#define cub_ADR_EXTENSION                  (byte)0xAA  /* address of the remote device (AE) */ 
/* ------------- END extended addressing only ---------------------------------*/

/* ----- single channel TP only -----------------------------------------------*/
#if( TP_CHANNELS == 1 )
#define cub_UDS_CLIENT                              1  /* client type 1=UDS */  
#define cub_BS_DEF                            (byte)0  /* default block size */
#define cub_ST_DEF                            (byte)0  /* default seperation time [ms]: 0..127 */
#define cuw_MIN_TIME_CONF                     (word)0  /* min time between FF-FC, FC-CF, CF-CF, CF-FC (Bs, Br) */
#define cuw_MAX_TIME_SEND                   (word)100  /* max time until msg sent (As, Ar) */
#define cuw_MAX_TIME_CONF                  (word)1000  /* max time between FF-FC, FC-CF, CF-CF, CF-FC (Bs, Br) */
#define cub_ADDRESSING                      TP_NORMAL  /* TP addressing mode (see tp.h) */       
/* -- normal/extended addressing only -----------*/
#define cub_SECOND_FUNC_ADDRESS                     0  /* 1=CAN ID 0x703 supported, 0=not supported */
#define cub_ISO15765_4_ADDRESSING                   0  /* 1=CAN ID 0x7DF supported, 0=not supported */
#define cul_REQ_CAN_ID_11BIT_PHYS   (dword)0x000007B0  /* pyhsical request can id for normal and extended addressing */
#define cul_RES_CAN_ID_11BIT        (dword)0x000004E4  /* response can id for normal and extended addressing */
/* -- END normal/extended addressing only -------*/
/* -- extended/mixed/fixed addressing only --*/
#define cub_ECU_ADR_PHYS                   (byte)0x55  /* physical own address (TA, SA) */
#define cub_ECU_ADR_FUNC                   (byte)0x33  /* functional own address TA, SA) */
/* -- END extended/mixed/fixed addressing only --*/
/* ----- END single channel TP only -------------------------------------------*/

/* ----- multiple channel TP only ---------------------------------------------*/
#elif( TP_CHANNELS == 3 )
#ifdef _I2S_TP_C_
const byte  acub_UDS_CLIENT[TP_CHANNELS]            = { 1, 1, 0 };
const byte  acub_BS_DEF[TP_CHANNELS]                = { 15, 15, 15 };
const byte  acub_ST_DEF[TP_CHANNELS]                = { 5, 5, 5 };
const word  acuw_MIN_TIME_CONF[TP_CHANNELS]         = { 0, 0, 0 };
const word  acuw_MAX_TIME_SEND[TP_CHANNELS]         = { 100, 100, 100 };
const word  acuw_MAX_TIME_CONF[TP_CHANNELS]         = { 1000, 1000, 1000 };
const byte  acub_ADDRESSING[TP_CHANNELS]            = { TP_NORMAL, TP_NORMAL, TP_EXTENDED };
/* -- normal/extended addressing only -----------*/
const byte  acub_SECOND_FUNC_ADDRESS[TP_CHANNELS]   = { 0, 0, 0 };
const byte  acub_ISO15765_4_ADDRESSING[TP_CHANNELS] = { 0, 0, 0 };
const dword acul_REQ_CAN_ID_11BIT_PHYS[TP_CHANNELS] = { 0x000007B0, 0x000007B1, 0x000007B2 }; 
const dword acul_RES_CAN_ID_11BIT[TP_CHANNELS]      = { 0x000004E4, 0x000004E5, 0x000004E6 };
/* -- END normal/extended addressing only -------*/
/* -- extended/mixed/fixed addressing only ------*/
const byte  acub_ECU_ADR_PHYS[TP_CHANNELS]          = { 0x55, 0x66, 0x77 };      
const byte  acub_ECU_ADR_FUNC[TP_CHANNELS]          = { 0x33, 0x33, 0x33 };
/* -- END extended/mixed/fixed addressing only --*/
#endif
/* ----- END multiple channel TP only ------------------------------------------*/
#else
#error "array size does not match with TP_CHANNELS value"
#endif

/****************************************************************************/
/* Vector CAN driver specific configuration items ***************************/
/****************************************************************************/
#include "can_inc.h"                                   /* defines needes in "DemoEcu.h" */
#include "DemoEcu.h"                                   /* insert here the ecu specific header file generated */
#if( TP_CHANNELS == 1 )
#define  CAN_TX_HANDLE             CanTx_DemoEcu_Resp  /* TP Tx handle within the vector can driver */          
#elif( TP_CHANNELS == 3 )
#ifdef _I2S_TP_C_
const byte acub_CAN_TX_HANDLE[TP_CHANNELS] = { CanTx_DemoEcu_Resp_0, CanTx_DemoEcu_Resp_1, CanTx_DemoEcu_Resp_2 };
#endif         
#else
#error "array size does not match with TP_CHANNELS value"
#endif

#endif

