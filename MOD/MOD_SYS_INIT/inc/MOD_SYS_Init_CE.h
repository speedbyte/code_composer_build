/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name: 	MOD_SYS_INIT_CE.h
*-----------------------------------------------------------------------------
* Module Name:	MOD_SYS_INIT
*-----------------------------------------------------------------------------
* Description:	configuration Header for external configuration of the system
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SYS_INIT/trunk/inc/MOD_SYS_Init_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _MOD_SYS_INIT_CE_H
#define _MOD_SYS_INIT_CE_H

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
#undef ISYS_COMPLETEINIT /* CONFIGURATE EXPORT INTERFACE(ISYS_COMPLETEINIT) */
#define ISYS_PARTINIT 100 /* CONFIGURATE EXPORT INTERFACE(ISYS_PARTINIT) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of configured interface
*/
#define ISYS_INIT_CFG 100
#if (ISYS_INIT_CFG != (ISYS_COMPLETEINIT | ISYS_PARTINIT))
	#error ("Incompatible interface: configured ifc = %d, reference ifc = %d!!!" ,ISYS_INIT_CFG, ISYSINIT1)
#endif

/***************************START Configuration Section************************/

/* Hardware init configuration section */
#define SYS_INIT_SUPPORT_PORT_IO
#define	SYS_INIT_SUPPORT_WD
#define	SYS_INIT_SUPPORT_CAN_TRANSCEIVER
/* Peripheral init configuration section */
#define	SYS_INIT_SUPPORT_ADC
#define SYS_INIT_SUPPORT_UART
#define	SYS_INIT_SUPPORT_SPI
#undef 	SYS_INIT_SUPPORT_I2C
#undef	SYS_INIT_SUPPORT_EEPROM
/* Software init configuration section */
#define SYS_INIT_SUPPORT_CAN
#define SYS_INIT_SUPPORT_NM
#define	SYS_INIT_SUPPORT_DEH
#define SYS_INIT_SUPPORT_ISO_TP
#define SYS_INIT_SUPPORT_UDS
#define SYS_INIT_SUPPORT_SDS
#define SYS_INIT_SUPPORT_SDSext
#define SYS_INIT_SUPPORT_APP_DIAG
#define SYS_INIT_SUPPORT_POOL
#undef  SYS_INIT_SUPPORT_DEBUG_PRINTF
#define SYS_INIT_SUPPORT_DEBUG_TRACE_CLIENT
#define SYS_INIT_SUPPORT_PHY_BUT_DRV
#define SYS_INIT_SUPPORT_LOGI_BUT_MAN
#undef  SYS_INIT_SUPPORT_MUTE_MGR
#undef  SYS_INIT_SUPPORT_LED
#define SYS_INIT_SUPPORT_TEMPERATURE
#define SYS_INIT_SUPPORT_CRC
#define SYS_INIT_SUPPORT_MONITOR_ADC
#define SYS_INIT_SUPPORT_APP_DTC
#define SYS_INIT_SUPPORT_CONTEXT_MAN

/*----------------------------------------------------------------------------*/
/* Ports part                                                                 */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_PORT_IO

#include "GIO.h"
#include "PORT.h"
/**
\brief	macro to define the Port initialization
*/
#define SYS_INIT_PORT               PORT_vInit()

#endif

/*----------------------------------------------------------------------------*/
/* Watchdog part                                                              */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_WD

#include "WD_C1.h"
/**
\brief	macro to define the Port initialization
*/
#define SYS_INIT_WD		(SuccessFailType)WD_u8Init()

#define SYS_INIT_WD		(SuccessFailType)WD_u8Init()
#endif

/*----------------------------------------------------------------------------*/
/* ADC part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_ADC

#include "ADC.h"
/**
\brief	macro to define the ADC initialization
*/
#define SYS_INIT_ADC		ADC_sfInit(ADC1)

#endif

/*----------------------------------------------------------------------------*/
/* UART part                                                                  */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_UART

#include "UART.h"
/**
\brief	macro to define the first UART initialization
*/
#define SYS_INIT_UART1		UART_sfInit(UART1)
/**
\brief	macro to define the second UART initialization
*/
#define SYS_INIT_UART2 		UART_sfInit(UART2)

#endif

/*----------------------------------------------------------------------------*/
/* SPI part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_SPI

#include "SPI.h"
/**
\brief	macro to define the first SPI initialization
*/
#define SYS_INIT_SPI1		SPI_sfInit(SPI1)
/**
\brief	macro to define the second SPI initialization
*/
#define SYS_INIT_SPI2 		SPI_sfInit(SPI2)
/**
\brief	macro to define the third SPI initialization
*/
#define SYS_INIT_SPI3 		SPI_sfInit(SPI3)
/**
\brief	macro to define the 4th SPI initialization
*/
#define SYS_INIT_SPI4 		SPI_sfInit(SPI4)
/**
\brief	macro to define the 5th SPI initialization
*/
#define SYS_INIT_SPI5 		SPI_sfInit(SPI5)
#endif

/*----------------------------------------------------------------------------*/
/* I2C part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_I2C

#include "I2C.h"
/**
\brief	macro to define the first I2C initialization
*/
#define SYS_INIT_I2C1		I2C_sfInit(I2C1)
/**
\brief	macro to define the second I2C initialization
*/
#define SYS_INIT_I2C2 		I2C_sfInit(I2C2)

#endif

/*----------------------------------------------------------------------------*/
/* EEPROM part                                                                */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_EEPROM

#include "EEPROM.h"
/**
\brief	macro to define the EEPROM initialization
*/
#define SYS_INIT_EEPROM		EEPROM_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* CAN part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_CAN

#include "can_inc.h"
/**
\brief	macro to define the first CAN part initialization
*/
#define SYS_INIT_CAN1		CanInitPowerOn()
/**
\brief	macro to define the second CAN part initialization
*/
#define SYS_INIT_CAN2 		//CanInitPowerOn()

#endif

/*----------------------------------------------------------------------------*/
/* CAN transceiver part                                                       */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_CAN_TRANSCEIVER

#include "CAN_CB.h"
/**
\brief	macro to set the CAN transceiver to normal state
*/
#define SYS_INIT_CANTR_NORMAL		BusTransceiverToNormal()
/**
\brief	macro to set the CAN transceiver to standby state
*/
#define SYS_INIT_CANTR_STDBY		BusTransceiverToStandby()
/**
\brief	macro to set the CAN transceiver to Power on state
*/
#define SYS_INIT_CANTR_PWRON		BusTransceiverToPowerOn()
/**
\brief	macro to set the CAN transceiver to sleep state
*/
#define SYS_INIT_CANTR_SLEEP		BusTransceiverToSleep()

#endif

/*----------------------------------------------------------------------------*/
/* Network management part                                                    */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_NM

#include "n_vnmdef.h"
/**
\brief	macro to define the Network management initialization
*/
#define SYS_INIT_NM			NmOsekInit(NM_NORMAL)

#endif

/*----------------------------------------------------------------------------*/
/* Diagnostic event handler part                                              */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_DEH

#include "deh.h"
/**
\brief	macro to define the Diagnostic event handler initialization
*/
#define SYS_INIT_DEH		DEH_Init()

#endif

/*----------------------------------------------------------------------------*/
/* ISO TP part                                                                */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_ISO_TP

#include "tp.h"
/**
\brief	macro to define the ISO TP initialization
*/
#define SYS_INIT_TP			TP_vInit()
#define SYS_INIT_TP_CHANNEL TP_vInitChannel

#endif


/*----------------------------------------------------------------------------*/
/* ISO TP part                                                                */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_UDS

#include "UDS.h"
/**
\brief	macro to define the UDS initialization
*/
#define SYS_INIT_UDS	    UDS_vInit()
#define SYS_INIT_UDS_CLIENT UDS_vInitClient

#endif

/*----------------------------------------------------------------------------*/
/* standard diagnosis Services part                                           */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_SDS

#include "sds.h"
/**
\brief	macro to define the standard diagnosis Services initialization
*/
#define SYS_INIT_SDS		SDS_vInit

#endif

/*----------------------------------------------------------------------------*/
/* standard diagnosis Services part                                           */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_SDSext

#include "sds_ext.h"
/**
\brief	macro to define the standard diagnosis Services initialization
*/
#define SYS_INIT_SDSext		SDSext_vInit

#endif

/*----------------------------------------------------------------------------*/
/* standard diagnosis Services part                                           */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_APP_DIAG

#include "APP_DIAG.h"
/**
\brief	macro to define the standard diagnosis Services initialization
*/
#define SYS_INIT_APP_DIAG		(void)APP_DIAG_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* Pool Handler part                                                          */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_POOL

#include "POOL.h"

#define SYS_INIT_POOL       (void)POOL_sfInit()

#endif


/*----------------------------------------------------------------------------*/
/* Debug part                                                                 */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_DEBUG_PRINTF

#include "DEBUG.h"

#define SYS_INIT_DEBUG_PRINTF       (void)DEBUG_sfInit()

#endif

#ifdef SYS_INIT_SUPPORT_DEBUG_TRACE_CLIENT

#include "TRACE.h"

#define SYS_INIT_DEBUG_TRACE_CLIENT  (void)Trace_vInit()

#endif
/*----------------------------------------------------------------------------*/
/* Button Driver part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_PHY_BUT_DRV

#include "PHY_BUT_DRV.h"

#define SYS_INIT_PHY_BUT_DRV    PHY_BUT_DRV_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* Button Manager part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_LOGI_BUT_MAN

#include "LOGI_BUT_MAN.h"

#define SYS_INIT_LOGI_BUT_MAN   (void)LOGI_BUT_MAN_sfInit()

#endif


/*----------------------------------------------------------------------------*/
/* Mute manager part                                                          */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_MUTE_MGR

#include "MUTE_MANAGER.h"

#define SYS_INIT_MUTE_MGR           (void)MUTE_MANAGER_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* LED part                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_LED

#include "LED.h"

#define SYS_INIT_LED           (void)LED_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* Temperature part                                                           */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_TEMPERATURE

#include "TEMPERATURE.h"

#define SYS_INIT_TEMPERATURE           (void)TEMPERATURE_sfInit()

#endif


/*----------------------------------------------------------------------------*/
/* CRC manager part                                                           */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_CRC

#include "CRCMGR.h"

#define SYS_INIT_CRC           (void)CRC_sfManagerInit()

#endif

/*----------------------------------------------------------------------------*/
/* Monitor ADC                                                            */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_MONITOR_ADC

#include "MONITOR_ADC.h"

#define SYS_INIT_MONITOR_ADC           (void)MONITOR_ADC_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* APP DTC                                                                */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_APP_DTC

#include "APP_DTC.h"

#define SYS_INIT_APP_DTC           (void)APP_DTC_sfInit()

#endif

/*----------------------------------------------------------------------------*/
/* Context Manager                                                         */
/*----------------------------------------------------------------------------*/
#ifdef SYS_INIT_SUPPORT_CONTEXT_MAN

#include "CONTEXT_MAN.h"

#define SYS_INIT_CONTEXT_MAN           (void)CONTEXT_MAN_sfInit()

#endif

#endif /* _MOD_SYS_INIT_CE_H */

/* End Of File MOD_SYS_INIT_CE.h */

