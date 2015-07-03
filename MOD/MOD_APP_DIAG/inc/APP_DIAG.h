/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	APP_DIAG.h
 * \brief	This file contains prototypes of all the functions that can be used
 * by other modules. Detailed description found below. 
 * \brief	It also contains enum, structure, constant definition etc. that can 
 * be used by other modules. The application/system must include this file 
 * in order to use the interfaces provided by this module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-07-25 15:28:53 +0200 (Mo, 25 Jul 2011) $
* $Rev: 21027 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DIAG/trunk/inc/APP_DIAG.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DIAG_H
#define _APP_DIAG_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "tms470r1.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "APP_DIAG_CE.h"
#include "APP_DIAG_ID_CHART_PARAMS_LEN.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
/* return values of the function SDSext_WriteUserAndIOData() */
/* */

/*#define APP_DIAG_GET_EEPROM_FALLBACK_LANG  0     
#define APP_DIAG_GET_EEPROM_VOICE_CONTROL 1   
#define APP_DIAG_GET_EEPROM_AUDIO_PARAM 2       
#define APP_DIAG_GET_EEPROM_MUTE_TYPE 3         
#define APP_DIAG_GET_EEPROM_MEDIA 4             
#define APP_DIAG_GET_EEPROM_AERIAL_DIAG 5       
#define APP_DIAG_GET_EEPROM_BASEPLATE_DIAG 6    
#define APP_DIAG_GET_EEPROM_ROOFLINE_MODULE 7
#define APP_DIAG_GET_EEPROM_SMS_FUNCTIONALITY_OPERATING_UNIT   0x0911
#define APP_DIAG_GET_EEPROM_MASKING_OF_LANGUAGES   9
#define APP_DIAG_GET_EEPROM_MUTE_DELAY  10
*/
typedef enum
{
	enAppDiagResponseOkImm           = 0,
	enAppDiagResponseErrorImm,
	enAppDiagResponseErrorBusyImm,
	enAppDiagResponseWaitForCallBack,
	enAppDiagResponseFurtherRequestsPending,
	enAppDiagResponseAbortVasRequest,
	enAppDiagResponseInvalid = 0xFF
} APP_DIAG_ENUM_RESPONSE_CODES;

typedef enum {
	enAppDiag_PHONEBOOK_SORTORDER_PRIO_LASTNAME      = 0,
    enAppDiag_PHONEBOOK_SORTORDER_PRIO_FIRSTTNAME    = 1
} APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE;

/* Message from Host-Diagnose (SDSext) to Application */
typedef struct {
     Uint16Type       Identifier;
     Uint8Type        data[APP_DIAG_MAX_DATA_LENGTH];
     Uint8Type        length;
} APP_DIAG_ST_DIAG_REQUEST, *APP_DIAG_PST_DIAG_REQUEST; 

/* Message from Application to Host-Diagnose (SDSext) */
typedef struct {
     Uint16Type               Identifier;
     Uint8Type                data[APP_DIAG_MAX_DATA_LENGTH];
     Uint8Type                length;
//     APP_DIAG_enResponseCode  result;
     Uint8Type                result;
} APP_DIAG_ST_DIAG_RESPONSE, *APP_DIAG_PST_DIAG_RESPONSE;

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef IAPP_DIAG_INIT /* BEGIN DECLARE INTERFACE(IAPP_DIAG_INIT) */

void APP_DIAG_vHandleEvents(EventMaskType event);
SuccessFailType APP_DIAG_sfInit();
#endif

APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE APP_DIAG_GetDiag_PhonebookSortOrder(void);

/*----------------------------------------------------------------------------*/


#endif /* _APP_DIAG_H */

/* End Of File APP_DIAG.h */


