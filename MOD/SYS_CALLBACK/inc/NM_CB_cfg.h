/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     NM_CB_cfg.h
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2009-03-10 10:02:17 +0100 (Di, 10 Mrz 2009) $
* $Rev: 4953 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/inc/NM_CB_cfg.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _NM_CB_CFG_H_
#define _NM_CB_CFG_H_

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Configuration section supports                                             */
/*----------------------------------------------------------------------------*/
#define NM_SUPPORT_CAN_TRANSCEIVER
#define NM_SUPPORT_CAN

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#ifdef NM_SUPPORT_CAN_TRANSCEIVER

/* CAN Transceiver status setting macro                                       */
#include "CAN_CB_cfg.h"
/** 
\brief	macro to set the CAN transceiver to normal mode
*/
#define NM_CAN_TRANSCEIVER_NORMAL			BusTransceiverToNormal
/** 
\brief	macro to set the CAN transceiver to standby mode
*/
#define NM_CAN_TRANSCEIVER_STANDBY			BusTransceiverToStandby
/** 
\brief	macro to set the CAN transceiver to sleep mode
*/
#define NM_CAN_TRANSCEIVER_SLEEP			BusTransceiverToSleep

#endif
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#ifdef NM_SUPPORT_CAN

/* CAN setting macro                                                          */
#include "can_inc.h"
/** 
\brief	macro to set the CAN driver to Wake up
*/
#define NM_CAN_WAKE_UP		(Uint8Type)CanWakeUp
/** 
\brief	macro to set the CAN driver to Stop
*/
#define NM_CAN_STOP			(Uint8Type)CanStop

#endif
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	default value for Write user data Bytes
*/
#define NM_SIGNAL_BYTE_DEFAULT                  0x00
 /* NWM wakeup - Byte 3                   */
#define NM_WAKE_UP_NO_RESET                     0x00
#define NM_WAKE_UP_KLEMME_30_RESET              0x00
#define NM_WAKE_UP_KLEMME_30_RESET_VALUE        0x02
#define NM_WAKE_UP_CAN_WAKE_UP                  0x01
/* NWM after-running reason - Byte 4      */
#define NM_WAKE_UP_KLEMME_15_ON                 0x01
#define NM_DIAGNOSE_ON_WO_KL15                  0x02
#define NM_OSEKNM_MIN_ACTIVETIME                0x04
#define NM_OSEKNM_CALL_ACTIVETIME               0x08
#define NM_OSEKNM_MEDIASTREAM_ACTIVETIME        0x10
/* NWM after-running reason - Byte 5      */
//#define NM_WAKE_UP_LOKAL_FUNC_POWER_ACTIVE      0x02
//#define NM_WAKE_UP_TELEFON_SLEEP_POWER_ACTIVE   0x01
/* NWM systeminfo - Byte 6                */
#define NM_TIME_OUT_ERROR_STORED                0x02
#define NM_SELF_DIAG_DEACTIVATED                0x04
#define NM_KSB_FUNCTION_QUALIFICATION           0x08
#define NM_MUTE_ACTIV                           0x10
#define NM_FUNC_LIMIT_TRANSPORTMODUS            0x20
#define NM_FUNCTIONAL_SHUTOFF                   0x40
#define NM_ONE_WIRE_MODE                        0x80

typedef enum
{
    NO_RESET = 0,
    KLEMME30_RESET,
    CAN_WAKE_UP
}WakeUp;

extern WakeUp wakeupReason;

#endif /* _NM_CB_CFG_H_ */

/* End Of File NM_CB_cfg.h */

