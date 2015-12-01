/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	DEBUG_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * can be used by other modules. It also contains various constants that can be 
 * configured by external people.
 * !!! THIS IS AN AUTOMATICALLY GENERATED FILE. PLEASE DO NOT CHANGE IT MANUALLY !!!
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-03-26 16:02:24 +0100 (Do, 26 Mrz 2009) $
* $Rev: 5552 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_DEBUG/trunk/inc/DEBUG_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _DEBUG_CE_H
#define _DEBUG_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define COMMAND(NAME)  { #NAME, UHV_## NAME, UHV_PARAMETER_## NAME }

/** 
    \brief	struture for storing the name, module number and their properties.
*/
typedef struct command
{
    char *module_name;
    Uint8Type module_value;
    Uint8Type module_property;
} DEBUG_ST_UHV_MODULES;
/** 
    \brief	enum for print levels to be used to determine the output messages.
*/
typedef enum {
    UHV_ERROR = 0,
    UHV_WARNING = 1,
    UHV_TRACE = 2,
    UHV_SPECIAL = 3,
    UHV_DEBUG = 4,
    UHV_DEBUG_1 = 5,
    UHV_DEBUG_2 = 6,
    UHV_DEBUG_3 = 7,
    UHV_NUM_LEVEL
} DEBUG_EN_PRINTLEVEL;

typedef enum {
UHV_ADC = 0,
UHV_APP_CAN = 1,
UHV_APP_COMMON = 2,
UHV_APP_CSHDL = 3,
UHV_APP_DEVICE = 4,
UHV_APP_DIAG = 5,
UHV_APP_DTC = 6,
UHV_APP_MDI = 7,
UHV_APP_MDI_BAP = 8,
UHV_APP_PHONE = 9,
UHV_APP_PHONEBOOK = 10,
UHV_APP_PHONEBOOK_BAP = 11,
UHV_APP_PHONE_BAP = 12,
UHV_APP_SD = 13,
UHV_APP_SMS = 14,
UHV_ASR = 15,
UHV_AT_CMD_HANDLER = 16,
UHV_AT_PARSER = 17,
UHV_BAP = 18,
UHV_BAP_DISPATCHER = 19,
UHV_CAN_DYN = 20,
UHV_CAN_ST = 21,
UHV_CONTEXT_MAN = 22,
UHV_CRC = 23,
UHV_CSHDL = 24,
UHV_DEH = 25,
UHV_DIAG_CAN = 26,
UHV_EEPROM = 27,
UHV_ERM = 28,
UHV_FBL_DATA = 29,
UHV_GIO = 30,
UHV_ISOTP = 31,
UHV_KPB = 32,
UHV_LED = 33,
UHV_LOGI_BUT_MAN = 34,
UHV_MFL_MGR = 35,
UHV_MONITOR_ADC = 36,
UHV_MUTE_MANAGER = 37,
UHV_NM = 38,
UHV_OSEK_DYN = 39,
UHV_OSEK_ST = 40,
UHV_PHY_BUT_DRV = 41,
UHV_POOL = 42,
UHV_PORT = 43,
UHV_POWERMANAGER = 44,
UHV_SDS_UDS = 45,
UHV_SPEED_MGR = 46,
UHV_SPI = 47,
UHV_SYS_INIT = 48,
UHV_TEMPERATURE = 49,
UHV_UART = 50,
UHV_UDS = 51,
UHV_VCR = 52,
UHV_WD = 53,
UHV_CALLBACK = 54,
UHV_OSEK_INTERFACE = 55,
UHV_MAIN = 56,
UHV_OUTPUT=57,
UHV_DEBUG_=58,
UHV_NUM_MODULE
} DEBUG_EN_PRINTMODULE;

#define UHV_INIT_STRUCT \
COMMAND(ADC),\
COMMAND(APP_CAN),\
COMMAND(APP_COMMON),\
COMMAND(APP_CSHDL),\
COMMAND(APP_DEVICE),\
COMMAND(APP_DIAG),\
COMMAND(APP_DTC),\
COMMAND(APP_MDI),\
COMMAND(APP_MDI_BAP),\
COMMAND(APP_PHONE),\
COMMAND(APP_PHONEBOOK),\
COMMAND(APP_PHONEBOOK_BAP),\
COMMAND(APP_PHONE_BAP),\
COMMAND(APP_SD),\
COMMAND(APP_SMS),\
COMMAND(ASR),\
COMMAND(AT_CMD_HANDLER),\
COMMAND(AT_PARSER),\
COMMAND(BAP),\
COMMAND(BAP_DISPATCHER),\
COMMAND(CAN_DYN),\
COMMAND(CAN_ST),\
COMMAND(CONTEXT_MAN),\
COMMAND(CRC),\
COMMAND(CSHDL),\
COMMAND(DEH),\
COMMAND(DIAG_CAN),\
COMMAND(EEPROM),\
COMMAND(ERM),\
COMMAND(FBL_DATA),\
COMMAND(GIO),\
COMMAND(ISOTP),\
COMMAND(KPB),\
COMMAND(LED),\
COMMAND(LOGI_BUT_MAN),\
COMMAND(MFL_MGR),\
COMMAND(MONITOR_ADC),\
COMMAND(MUTE_MANAGER),\
COMMAND(NM),\
COMMAND(OSEK_DYN),\
COMMAND(OSEK_ST),\
COMMAND(PHY_BUT_DRV),\
COMMAND(POOL),\
COMMAND(PORT),\
COMMAND(POWERMANAGER),\
COMMAND(SDS_UDS),\
COMMAND(SPEED_MGR),\
COMMAND(SPI),\
COMMAND(SYS_INIT),\
COMMAND(TEMPERATURE),\
COMMAND(UART),\
COMMAND(UDS),\
COMMAND(VCR),\
COMMAND(WD),\
COMMAND(CALLBACK),\
COMMAND(OSEK_INTERFACE),\
COMMAND(MAIN),\
COMMAND(OUTPUT),\
COMMAND(DEBUG_)


#define UHV_PARAMETER_ADC	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_CAN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_COMMON	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_CSHDL	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_DEVICE	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_DIAG	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_DTC	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_MDI	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_MDI_BAP	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_PHONE	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_PHONEBOOK	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_PHONEBOOK_BAP	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_PHONE_BAP	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_SD	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_APP_SMS	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_ASR	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_AT_CMD_HANDLER	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_AT_PARSER	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_BAP	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_BAP_DISPATCHER	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_CAN_DYN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_CAN_ST	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_CONTEXT_MAN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_CRC	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_CSHDL	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_DEH	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_DIAG_CAN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_EEPROM	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_ERM	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_FBL_DATA	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_GIO	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_ISOTP	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_KPB	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_LED	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_LOGI_BUT_MAN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_MFL_MGR	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_MONITOR_ADC	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_MUTE_MANAGER	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_NM	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_OSEK_DYN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_OSEK_ST	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_PHY_BUT_DRV	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_POOL	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_PORT	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_POWERMANAGER	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_SDS_UDS	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_SPEED_MGR	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_SPI	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_SYS_INIT	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_TEMPERATURE	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_UART	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_UDS	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_VCR	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_WD	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_CALLBACK	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_OSEK_INTERFACE	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_MAIN	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL))

#define UHV_PARAMETER_OUTPUT	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL ))

#define UHV_PARAMETER_DEBUG_	((1<<UHV_ERROR) | (1<<UHV_WARNING) | (1<<UHV_TRACE) | (1<<UHV_SPECIAL ))


#define IDEBUG_WRITE
#define IDEBUG_INIT
#define IDEBUG_READ

#ifdef SYSTEM_TARGET_PRINT
	#define DEBUG_TARGET_PRINT
#else
	#ifdef SYSTEM_HOST_PRINT
		#define DEBUG_HOST_PRINT
	#endif
#endif

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#endif /* _DEBUG_CE_H */

/* End Of File DEBUG_CE.h */
