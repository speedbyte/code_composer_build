/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

#ifndef _TRACE_CONFIG_H
#define _TRACE_CONFIG_H


/* DEFINE MAXIMUM 64 TRACE MODULES, VALID ID'S ARE 0-63 */


typedef enum {
MOD_ADC                  =  0,     // nothing to do           -> done
MOD_APP_CAN              =  1,     // done
MOD_APP_COMMON           =  2,     // done
MOD_APP_CSHDL            =  3,     // done
MOD_APP_DEVICE           =  4,     // done
MOD_APP_DIAG             =  5,     // done
MOD_APP_DTC              =  6,     // done
MOD_APP_MDI              =  7,     // done
MOD_APP_MDI_BAP          =  8,     // done
MOD_APP_PHONE            =  9,     // done
MOD_APP_PB               = 10,     // done
MOD_APP_PB_BAP           = 11,     // done
MOD_APP_PHONE_BAP        = 12,     // done
MOD_APP_SD               = 13,     // done
MOD_APP_SMS              = 14,     // DONE
MOD_ASR                  = 15,     // done
MOD_AT_CMD_HANDLER       = 16,     // ToDo  Thorsten             -> full done
MOD_AT_PARSER            = 17,     // ToDo  in progress Oliver
MOD_BAP                  = 18,     // ToDo  Thorsten             -> done
MOD_BAP_DISPATCHER       = 19,     // ToDo  Thorsten             -> done
MOD_CAN_DYN              = 20,     // nothing to do              -> done
MOD_CAN_ST               = 21,     // nothing to do              -> done
MOD_CONTEXT_MAN          = 22,     // ToDo  Vikas
MOD_CRC                  = 23,     // nothing to do              -> done
MOD_CSHDL                = 24,     // done
MOD_DEH                  = 25,     // nothing to do              -> done
MOD_DIAG_CAN             = 26,     // done
MOD_EEPROM               = 27,     // ToDo  Vikas  				 -> done
MOD_ERM                  = 28,     // done
MOD_FBL_DATA             = 29,     // done
MOD_GIO                  = 30,     // nothing to do             -> done
MOD_ISOTP                = 31,     // nothing to do             -> done
MOD_KPB                  = 32,     // ToDo  Thorsten            -> done
MOD_LED                  = 33,     // nothing to do             -> done
MOD_LOGI_BUT_MAN         = 34,     // ToDo  Vikas				-> done
MOD_MFL_MGR              = 35,     // ToDo  Vikas
MOD_MONITOR_ADC          = 36,     // nothing to do             -> done
MOD_ERM_JINGLE           = 37,     //
MOD_NM                   = 38,     // Vikas			            -> done
MOD_OSEK_DYN             = 39,     // nothing to do            -> done
MOD_OSEK_ST              = 40,     // nothing to do            -> done
MOD_PHY_BUT_DRV          = 41,     // Vikas                    -> done
MOD_POOL                 = 42,     // nothing to do             -> done
MOD_PORT                 = 43,     // nothing to do             -> done
MOD_POWERMANAGER         = 44,     // done
MOD_SDS_UDS              = 45,     // Vikas.....................->done
MOD_SPEED_MGR            = 46,     // done
MOD_SPI                  = 47,     // nothing to do             -> done
MOD_SYS_INIT             = 48,     // nothing to do             -> done
MOD_TEMPERATURE          = 49,     // nothing to do             -> done
MOD_UART                 = 50,     // ToDo  Vikas
MOD_UDS                  = 51,     // nothing to do             -> done
MOD_VCR                  = 52,     // ToDo Oliver               -> done
MOD_WD                   = 53,     // nothing to do             -> done
MOD_CALLBACK             = 54,     // done
MOD_OSEK_INTERFACE       = 55,     // ToDo  ??
MOD_MAIN                 = 56,     // nothing to do             -> done
MOD_TRACE                = 57,     //                              done
MOD_EEPROM_LAYOUT        = 58,     // Vikas
MOD_STD_DEH				 = 59,
MOD_UNUSED60             = 60,
MOD_UNUSED61             = 61,
MOD_UNUSED62             = 62,
MOD_UNUSED63             = 63       /* 63 = THIS IS THE LAST ONE !!!  */
} TRACE_MODULES;




#include "TRACE_CONFIG_GENERATED.h"

#endif /* _TRACE_CONFIG_H */

/* End Of File _TRACE_CONFIG_H */
