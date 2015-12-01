/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: TP.H
 **************************************************************************
 *  PURPOSE:
 *  Header-File including transport protocol functionality ISO 15765 API
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
 *  V_00_01_06 Ki 09.08.2005 - Function parameters match with C file
 *  V_00_01_07 Ki 19.09.2005 - TP_ubApplDataInd( unsigned char* mode)
 *                           - TP_ubApplDataCon() without parameter
 *  V_00_01_08 Ki 05.12.2005 - no change
 *  V_01_00_00 Ki 18.01.2006 - typedefs.h eliminated
 *  V_01_02_00 Ki 02.06.2006 - Multi channel capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - API function with/without ch parameter
 *                           - TP_CHANNELS moved from cfg to here 
 *  V_01_02_03 Ki 02.11.2006 - TP_ubApplDataCon with new param and ret-type
 *                           - new define: cub_RELEASE_BUFFER
 *                           - Version number moved into C file
 *                           - New function: TP_vGetVersionInfo
 *                           - Typedef t_APP_STATE moved into tp.h
 *  V_01_02_04 Ki 19.12.2006 - i2s_types.h included
 *                           - use of i2s type instead of C types
 *  V_01_02_05 Ki 12.01.2007 - no change
 *  V_01_02_06 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_07 Ki 30.03.2007 - no change
 *  V_01_02_08 Ki 11.04.2007 - no change
 *  V_01_02_09 Ki 02.07.2007 - no change
 *  V_01_02_10 Ki 11.07.2007 - no change
 *  V_01_02_11 Ki 23.07.2007 - no change
 *  V_01_02_12 Ki 02.11.2007 - no change
 *  V_01_02_13 Ki 18.06.2008 - no change
 *  V_01_02_14 Ki 23.07.2008 - no change
 *  V_01_02_15 Ki 08.12.2008 - no change
 *************************************************************************/

#ifndef _I2S_TP_H_
#define _I2S_TP_H_

#include "i2s_types.h"

/* ----------------------------------------------------------------------- */
/*  - C o n s t a n t s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */
#define TP_NORMAL                                   0  /* 11 bit id, no TA */
#define TP_NORMAL_FIXED                             1  /* 29 bit id, no TA */
#define TP_EXTENDED                                 2  /* 11 bit id, TA */ 
#define TP_MIXED                                    3  /* 29 bit id, AE */
#define TP_MIXED_11BIT                              4  /* 11 bit id, AE */

/* ----------------------------------------------------------------------- */
/*  - T y p e d e f s ---------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/* states between the transport layer and diagnostic layer */ 
typedef enum  {
  APP_STATE_IDLE=0,          /* no request for the application */
  APP_STATE_INDICATION,      /* pending request for the application */
  APP_STATE_CONFIRMATION,    /* applications response sent */
  APP_STATE_PROCESS,         /* request is in process by the application */
  APP_STATE_ERROR            /* transmission error (instead of confirmation) */
} t_APP_STATE;

#include "tp_cfg.h"           /* do not move this line */

#if (TP_CHANNELS > 1)
#define TP_CHANNEL_INDEX_API byte i2s_ch,
#define TP_CHANNEL_INDEX_API_2 byte i2s_ch
#define TP_CHANNEL_INDEX i2s_ch,
#define TP_CHANNEL_INDEX_2 i2s_ch
#else
#define TP_CHANNEL_INDEX_API 
#define TP_CHANNEL_INDEX 
#define TP_CHANNEL_INDEX_API_2 void
#define TP_CHANNEL_INDEX_2 
#endif

/* ----------------------------------------------------------------------- */
/*  - P r o t o t y p e s ------------------------------------------------ */
/* ----------------------------------------------------------------------- */

/* prototypes can vary by the UDS_CLIENTS value --> therefore use unique defines below */
/* to keep your application independent of UDS configuration */ 
extern void         TP_vInit           ( void );
extern void         TP_vTask           ( void );
extern void         TP_vGetVersionInfo ( I2S_VersionInfoType *psVersionInfo );
extern void         TP_vInitChannel    ( TP_CHANNEL_INDEX_API byte* RecBuf, word bufSize );
extern word         TP_uwApplDataInd   ( TP_CHANNEL_INDEX_API byte* mode );
extern byte         TP_ubApplDataReq   ( TP_CHANNEL_INDEX_API byte *buf, word len, byte rel );
extern t_APP_STATE  TP_eGetApplState   ( TP_CHANNEL_INDEX_API_2 );
extern void         TP_vCancelRequest  ( TP_CHANNEL_INDEX_API_2 );


#endif

