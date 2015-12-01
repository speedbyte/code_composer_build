/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: UDS.H
 **************************************************************************
 *  PURPOSE:
 *  Heaader-File including UDS protocol functionality in combinition with
 *  transport protocol ISO 15765 
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
 *  V_00_01_04 Ki 22.07.2005 - Configuration matching demo application  
 *  V_00_01_05 Ki 04.08.2005 - Bug-Fix: suppress response, not supported
 *                             in active mode handling
 *  V_00_01_06 Ki 09.08.2005 - Time trigger and timeout callback modified
 *  V_00_01_07 Ki 19.09.2005 - UDScub_MULTIPLE_FRAME deleted, UDScub_CANCELED new
 *  V_00_01_08 Ki 05.12.2005 - no change
 *  V_01_00_00 Ki 18.01.2006 - new function: UDS_ubGetSession
 *                           - typedefs.h eliminated
 *  V_01_01_00 Ki 09.03.2006 - no change
 *  V_01_02_00 Ki 02.06.2006 - Multi client capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - API function with/without ch parameter 
 *                           - UDS_CLIENTS moved from cfg to here
 *  V_01_02_03 Ki 02.11.2006 - UDS_ubIsIdle new caused by SDS
 *                           - Version number moved into C file
 *                           - New function: TP_vGetVersionInfo
 *                           - UDS_ubIgnoreFuncReq: param ch deleted
 *  V_01_02_04 Ki 19.12.2006 - i2s_types.h included
 *                           - use of i2s type instead of C types
 *  V_01_02_05 Ki 12.01.2007 - no change
 *  V_01_02_06 Ki 22.01.2007 - no change
 *  V_01_02_07 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_08 Ki 30.03.2007 - UDScub_INCORRECT_MSG_LEN new
 *  V_01_02_09 Ki 02.07.2007 - UDScub_IO_TEST_SESSION inserted
 *                           - UDS_MAX_NO_OF_SESSIONS = 6
 *  V_01_02_10 Ki 11.07.2007 - no change
 *  V_01_02_11 Ki 07.08.2007 - no change 
 *  V_01_02_12 Ki 02.11.2007 - no change
 *  V_01_02_14 Ki 23.07.2008 - new: UDScub_RPM_TOO_HIGH, UDScub_VEHICLE_SPEED_TOO_HIGH
 *************************************************************************/

#ifndef  _I2S_UDS_H_
#define  _I2S_UDS_H_

#include "i2s_types.h"

/* ----------------------------------------------------------------------- */
/*  - T y p e d e f s ---------------------------------------------------- */
/* ----------------------------------------------------------------------- */

typedef struct
{
  byte Sid;                        /* service id */
  byte (*func)(byte*,word*,byte);  /* address of the callback function, 0 = not supported */ 
  byte suppressRespAllowed;        /* pos. response suppress handling, 0 = do no suppress */ 
  byte ModeAvailability;           /* supported in session by sid: bit coded --> see description below */
} t_DiagServiceItem;

typedef struct
{
  void (*func)(void);  /* address of the callback function */ 
} t_DiagTimeoutFkt;

/* ----------------------------------------------------------------------- */
/*  - C o n s t a n t s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* return values for call back functions; don´t use 0x78 as return code for callbacks */
#define UDScub_IN_PROCESS                      (byte)0x00
#define UDScub_CANCELED                        (byte)0xFE
#define UDScub_FINISHED                        (byte)0xFF
 
#define UDScub_INVALID_ID                      (byte)0      /* invalid can identifier */
#define UDScub_PHYSICAL                        (byte)1      /* physical can identifier */
#define UDScub_FUNCTIONAL                      (byte)2      /* functional can identifier */
#define UDScub_OBD                             (byte)3      /* OBD can identifier */

/* supported diagnostic modes */ 
#define UDScub_DEFAULT_SESSION                    0x01
#define UDScub_PROGRAMMING_SESSION                0x02
#define UDScub_EXTENTED_DIAG_SESSION              0x03
#define UDScub_EOL_SESSION                        0x40
#define UDScub_IO_TEST_SESSION                    0x41
#define UDScub_DEVELOPMENT_SESSION                0x4F
#define UDScub_INVALID_SESSION                    0x00   /* value to recognize an invalid session */

/* negative response codes */
#define UDScub_SERVICE_NOT_SUPPORTED             (byte)0x11
#define UDScub_SUBFUNCTION_NOT_SUPPORTED         (byte)0x12
#define UDScub_INCORRECT_MSG_LEN                 (byte)0x13
#define UDScub_BUSY_REPEAT_REQUEST               (byte)0x21
#define UDScub_CONDITIONS_NOT_CORRECT            (byte)0x22
#define UDScub_REQUEST_OUT_OF_RANGE              (byte)0x31
#define UDScub_ACCESS_DENIED                     (byte)0x33
#define UDScub_INVALID_KEY                       (byte)0x35
#define UDScub_TIME_DELAY_NOT_EXPIRED            (byte)0x37
#define UDScub_REQUEST_RECEIVED_RESPONSE_PENDING (byte)0x78 
#define UDScub_SFNS_IN_ACTIVE_DIAGNOSTIC_SESSION (byte)0x7E
#define UDScub_SNS_IN_ACTIVE_DIAGNOSTIC_SESSION  (byte)0x7F
#define UDScub_RPM_TOO_HIGH                      (byte)0x81
#define UDScub_VEHICLE_SPEED_TOO_HIGH            (byte)0x88

#define UDS_MAX_NO_OF_SESSIONS                       6   /* number of supported sessions */
#define UDS_MAX_NO_OF_SERVICES                      28   /* number of supported services */

#include "uds_cfg.h"                                     /* do not move this line */
                                                         
#if (UDS_CLIENTS > 1)
#define UDS_CHANNEL_INDEX_API byte i2s_ch,
#define UDS_CHANNEL_INDEX_API_2 byte i2s_ch
#define UDS_CHANNEL_INDEX i2s_ch,
#define UDS_CHANNEL_INDEX_2 i2s_ch
#else
#define UDS_CHANNEL_INDEX_API
#define UDS_CHANNEL_INDEX 
#define UDS_CHANNEL_INDEX_API_2 void
#define UDS_CHANNEL_INDEX_2
#endif

/* ----------------------------------------------------------------------- */
/*  - P r o t o t y p e s ------------------------------------------------ */
/* ----------------------------------------------------------------------- */
/* prototypes can vary by the UDS_CLIENTS value --> therefore use unique defines below */
/* to keep your application independent of UDS configuration */ 
extern void UDS_vInit          ( void );
extern void UDS_vTask          ( void );
extern void UDS_vGetVersionInfo( I2S_VersionInfoType *psVersionInfo );
extern byte UDS_ubIgnoreFuncReq( byte* RxBuf, byte Len, byte mode );
extern void UDS_vInitClient    ( UDS_CHANNEL_INDEX_API byte* RxBuf );
extern byte UDS_ubSetSession   ( UDS_CHANNEL_INDEX_API byte bSession );
extern byte UDS_ubGetSession   ( UDS_CHANNEL_INDEX_API_2 );
extern byte UDS_ubIsIdle       ( UDS_CHANNEL_INDEX_API_2 );
extern void UDS_vSetReady      ( UDS_CHANNEL_INDEX_API byte state );
extern byte UDS_ubIsActive     ( UDS_CHANNEL_INDEX_API_2 );
extern byte UDS_ubSendMsg      ( UDS_CHANNEL_INDEX_API byte* buf, word len );

#endif
