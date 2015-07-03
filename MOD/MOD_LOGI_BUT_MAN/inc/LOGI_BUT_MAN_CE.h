/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	LOGI_BUT_MAN_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file mainly contains:
 * 			1. constants to switch on or off the exported interfaces. 
 * 			2. configurable constants.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-11-26 16:38:44 +0100 (Fr, 26 Nov 2010) $
* $Rev: 16561 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LOGI_BUT_MAN/trunk/inc/LOGI_BUT_MAN_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _LOGI_BUT_MAN_CE_H
#define _LOGI_BUT_MAN_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/


#define LOGI_BUT_MAN_3BM_MICMUTE_BUTTON    		enLogiButMan3bmButton1
#define LOGI_BUT_MAN_3BM_PTT_BUTTON             enLogiButMan3bmButton2
#define LOGI_BUT_MAN_3BM_BTVISIBLE_BUTTON       enLogiButMan3bmButton3

#define LOGI_BUT_MAN_CRADLE_INFO_BUTTON			enLogiButManCradleButton1
#define LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON	enLogiButManCradleButton2
#define LOGI_BUT_MAN_CRADLE_BOTH_BUTTON			enLogiButManCradleButtonBoth
#define LOGI_BUT_MAN_CRADLE_INFO_CALL 			enLogiButManVirtualButtonInfo
#define LOGI_BUT_MAN_CRADLE_SOS_CALL 			enLogiButManVirtualButtonSos
#define LOGI_BUT_MAN_CRADLE_SERVICE_CALL 		enLogiButManVirtualButtonService

#define CONTEXT_MAN_MFL_HOOK_BUTTON 			enLogiButManMflHook
#define CONTEXT_MAN_MFL_BACK_BUTTON 			enLogiButManMflBack
#define CONTEXT_MAN_MFL_OK_BUTTON 				enLogiButManMflOk
#define CONTEXT_MAN_MFL_UP_BUTTON 				enLogiButManMflUp
#define CONTEXT_MAN_MFL_DOWN_BUTTON 			enLogiButManMflDown
#define CONTEXT_MAN_MFL_PTT 					enLogiButManMflPTT

#define CONTEXT_MAN_LSS_UP_BUTTON 				enLogiButManLssUp
#define CONTEXT_MAN_LSS_DOWN_BUTTON 			enLogiButManLssDown
#define CONTEXT_MAN_LSS_OK_BUTTON 				enLogiButManLssOk

#define CONTEXT_MAN_ROLL_BUTTON_UP 				enLogiButManMflRollUp
#define CONTEXT_MAN_ROLL_BUTTON_DOWN 			enLogiButManMflRollDown

#define LOGI_BUT_MAN_NO_BUTTON 					enLogiButManNoButton

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
#define LOGI_BUT_MAN_CRADLE_OFF      SPI_vPinModify(SPI4, SpiSomiPin, PortIoOutLow);   /* CRADLE_OFF */\
    					SPI_vPinModify(SPI4, SpiClkPin, PortIoOutHigh);   /* CRADLE_TB */\
    					SPI_vPinModify(SPI4, SpiClkPin, PortIoOutLow);   /* CRADLE_TB */
    					
#define LOGI_BUT_MAN_CRADLE_ON      SPI_vPinModify(SPI4, SpiSomiPin, PortIoOutHigh);   /* CRADLE_ON */\
    					SPI_vPinModify(SPI4, SpiClkPin, PortIoOutHigh);   /* CRADLE_TB */\
    					SPI_vPinModify(SPI4, SpiClkPin, PortIoOutLow);   /* CRADLE_TB */

#define LOGI_BUT_MAN_CRADLE_WUP_ON   GIO_vPinModify(GIOB, PORT_CRADLE_WUP, PortIoOutHigh);
#define LOGI_BUT_MAN_CRADLE_WUP_OFF   GIO_vPinModify(GIOB, PORT_CRADLE_WUP, PortIoOutLow);

#elif (defined(PROCESSOR_TMS470R1VF45AA))

#define LOGI_BUT_MAN_CRADLE_OFF      GIO_vPinModify(GIOC, PORT_CRADLE_ON_OFF, PortIoOutLow); \
						GIO_vPinModify(GIOC, PORT_CRADLE_TB, PortIoOutHigh); \
						GIO_vPinModify(GIOC, PORT_CRADLE_TB, PortIoOutLow); 

#define LOGI_BUT_MAN_CRADLE_ON      GIO_vPinModify(GIOC, PORT_CRADLE_ON_OFF, PortIoOutHigh); \
						GIO_vPinModify(GIOC, PORT_CRADLE_TB, PortIoOutHigh); \
						GIO_vPinModify(GIOC, PORT_CRADLE_TB, PortIoOutLow); 

#define LOGI_BUT_MAN_CRADLE_WUP_ON   GIO_vPinModify(GIOB, PORT_CRADLE_WUP, PortIoOutHigh);
#define LOGI_BUT_MAN_CRADLE_WUP_OFF   GIO_vPinModify(GIOB, PORT_CRADLE_WUP, PortIoOutLow);
    					
#endif
/* We use PinModify during Init and PinWrite during later phases */
#define LOGI_BUT_MAN_RLADDER_ON GIO_vPinWrite(PORT_RLADDER_EN, TRUE, GIOC);
#define LOGI_BUT_MAN_RLADDER_ON_INIT GIO_vPinModify(GIOC, PORT_RLADDER_EN, PortIoOutHigh);

#define LOGI_BUT_MAN_RLADDER_OFF GIO_vPinWrite(PORT_RLADDER_EN, FALSE, GIOC);
#define LOGI_BUT_MAN_RLADDER_OFF_INIT GIO_vPinModify(GIOC, PORT_RLADDER_EN, PortIoOutLow);

#define LOGI_BUT_MAN_MIN_CNT_BUTTON_HANGING 600   // 600 is 30 seconds
#define LOGI_BUT_MAN_BORDER_WIEDERHOL 6   // the MFL repeat would be sent every 100ms.

#define LOGI_BUT_MAN_10_SEC_LONG_PRESS 10 // 10 second long press
#define LOGI_BUT_MAN_10_SEC_LONG_PRESS_3BM (LOGI_BUT_MAN_10_SEC_LONG_PRESS*16) // 10 second long press
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
#define ILOGI_BUT_MAN_TASK_APP        
#define ILOGI_BUT_MAN_INIT         
#define ILOGI_BUT_MAN_DIAG
#define ILOGI_BUT_MAN_DIAGNOSE_3BM
#define ILOGI_BUT_MAN_DIAGNOSE_CRADLE
#define ILOGI_BUT_MAN_GET_3BM
#define ILOGI_BUT_MAN_GET_CRADLE
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#endif /* _LOGI_BUT_MAN_CE_H */

/* End Of File LOGI_BUT_MAN_CE.h */
