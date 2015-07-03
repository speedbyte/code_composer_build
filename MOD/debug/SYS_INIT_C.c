/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:	SYS_INIT_C.c
*-----------------------------------------------------------------------------
* Module Name:	MOD_SYS_INIT
*-----------------------------------------------------------------------------
* Description:	Module to initialize the System and seperatly some different
* 				parts.
*-----------------------------------------------------------------------------
* $Date: 2011-11-08 11:56:21 +0100 (Di, 08 Nov 2011) $
* $Rev: 23158 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SYS_INIT/trunk/src/SYS_INIT_C.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "MOD_SYS_INIT_CE.h"
#include "uds_appl.h"
#include "GIO.h"
#include "NM_CB_cfg.h"
#include "SYS_INIT.h"
#include "SYS_INIT_C.id"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
WakeUp wakeupReason = NO_RESET;

extern Uint32Type FatalOsekErrorOccured ;
extern Uint16Type FatalOsekErrorCode ;
extern Uint16Type FatalOsekguiError;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfHwInit                                             */
/**
    \brief      Initialization function for HW

    \brief      Description:\n
                Initialization function for the Hardware parts Port, external \n
                watchdog \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfHwInit(void)
{
	SuccessFailType sF = SUCCESS;

#ifdef SYS_INIT_SUPPORT_PORT_IO
	SYS_INIT_PORT;
#endif
#ifdef SYS_INIT_SUPPORT_WD
	sF = SYS_INIT_WD;
#endif
#ifdef SYS_INIT_SUPPORT_WD
	sF = SYS_INIT_WD;
#endif
	return sF;
}/* END OF SYSINIT_sfHwInit() */

/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfPeripheralsInit                                    */
/**
    \brief      Initialization function for Peripherals

    \brief      Description:\n
                Initialization function for the adc, uart, spi, i2C \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfPeripheralsInit(void)
{

	SuccessFailType sF = SUCCESS;

#ifdef SYS_INIT_SUPPORT_ADC
	sF = SYS_INIT_ADC;
#endif

#ifdef SYS_INIT_SUPPORT_UART
	sF = SYS_INIT_UART1;
	sF = SYS_INIT_UART2;
#endif

#ifdef SYS_INIT_SUPPORT_SPI
	sF = SYS_INIT_SPI1;
	sF = SYS_INIT_SPI2;
	sF = SYS_INIT_SPI3;
	sF = SYS_INIT_SPI4;
	sF = SYS_INIT_SPI5;
#endif

#ifdef SYS_INIT_SUPPORT_I2C
	sF = SYS_INIT_I2C1;
	sF = SYS_INIT_I2C2;
#endif

#ifdef SYS_INIT_SUPPORT_EEPROM
	sF = SYS_INIT_EEPROM;
#endif

    /* check which wake up reason is occured                                */
    if(LOW == GIO_u8PinRead(PORT_CAN_ERROR, GIOA))
    {
        wakeupReason = KLEMME30_RESET;
    }
    else
    {
        wakeupReason = CAN_WAKE_UP;
    }

#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf( UHV_TRACE,
            UHV_SYS_INIT,
            "UHV-NAR wakeup reason: %x", wakeupReason);
#endif


#ifdef SYS_INIT_SUPPORT_CAN_TRANSCEIVER
    SYS_INIT_CANTR_NORMAL;
#endif

	return sF;
}/* END OF SYSINIT_sfPeripheralsInit() */

/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfSwInit                                             */
/**
    \brief      Initialization function for Software

    \brief      Description:\n
                Initialization function for standard software \n
                core, buffer and other software dependent parts \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfSwInit(void)
{
	SuccessFailType sF = SUCCESS;

#ifdef SYS_INIT_SUPPORT_CAN
	SYS_INIT_CAN1;
	SYS_INIT_CAN2;
#endif

#ifdef SYS_INIT_SUPPORT_NM
	SYS_INIT_NM;
#endif

#ifdef SYS_INIT_SUPPORT_DEH
	//sF = SYS_INIT_DEH;
#endif

#ifdef SYS_INIT_SUPPORT_ISO_TP
	SYS_INIT_TP;
	SYS_INIT_TP_CHANNEL(MsgBuf,BUF_SIZE);
#endif

#ifdef SYS_INIT_SUPPORT_UDS
    SYS_INIT_UDS;
    SYS_INIT_UDS_CLIENT(MsgBuf);
#endif


#ifdef SYS_INIT_SUPPORT_SDS
	SYS_INIT_SDS(BUF_SIZE);
#endif

#ifdef SYS_INIT_SUPPORT_SDSext
	SYS_INIT_SDSext(BUF_SIZE);
#endif

#ifdef SYS_INIT_SUPPORT_APP_DIAG
	SYS_INIT_APP_DIAG;
#endif

#ifdef SYS_INIT_SUPPORT_POOL
    SYS_INIT_POOL;
#endif

#ifdef SYS_INIT_SUPPORT_DEBUG_PRINTF
    SYS_INIT_DEBUG_PRINTF;
#endif

#if defined(USE_DEBUGGING_WITH_TRACE_ID)
    #ifdef SYS_INIT_SUPPORT_DEBUG_TRACE_CLIENT
    SYS_INIT_DEBUG_TRACE_CLIENT;
    #endif
#endif

#ifdef SYS_INIT_SUPPORT_PHY_BUT_MAN
    SYS_INIT_PHY_BUT_DRV;
#endif

#ifdef SYS_INIT_SUPPORT_LOGI_BUT_MAN
    SYS_INIT_LOGI_BUT_MAN;
#endif
    if (0xAEEAAEEA == FatalOsekErrorOccured)
    {
        DEBUG_VALUE2(OSEK_INTERFACE_SWI_TRC,MOD_OSEK_INTERFACE, "Fatal OSEK Error: ErrorCode: 0x%04X ErrorNumber: 0x%04X - see TechnicalReference_osCAN3.pdf",FatalOsekErrorCode,FatalOsekguiError);
        FatalOsekErrorOccured = 0;
    }

#ifdef SYS_INIT_SUPPORT_MUTE_MGR
    SYS_INIT_MUTE_MGR;
#endif

#ifdef SYS_INIT_SUPPORT_LED
    SYS_INIT_LED;
#endif

#ifdef SYS_INIT_SUPPORT_TEMPERATURE
    SYS_INIT_TEMPERATURE;
#endif

#ifdef SYS_INIT_SUPPORT_CRC
    SYS_INIT_CRC;
#endif

#ifdef SYS_INIT_SUPPORT_MONITOR_ADC
    SYS_INIT_MONITOR_ADC;
#endif

#ifdef SYS_INIT_SUPPORT_APP_DTC
    SYS_INIT_APP_DTC;
#endif

#ifdef SYS_INIT_SUPPORT_CONTEXT_MAN
    SYS_INIT_CONTEXT_MAN;
#endif
    return sF;
}/* END OF SYSINIT_sfSwInit() */


/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfInit()                                             */
/**
    \brief      Initialization function after Power down

    \brief      Description:\n
                complete init part after Power down. This \n
                includes HW, Peripherals, driver and protocols \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfInit(void)
{
	SuccessFailType sF = SUCCESS;

	sF = SYSINIT_sfHwInit();
	sF = SYSINIT_sfPeripheralsInit();
	sF = SYSINIT_sfSwInit();

	return sF;
}/* END OF SYSINIT_sfInit() */


/* End Of File SYS_INIT_C.c */
