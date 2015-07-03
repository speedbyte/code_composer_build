/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_SD_Icons.c
*-----------------------------------------------------------------------------
* Module Name:  APP_SD    
*-----------------------------------------------------------------------------
* Description:  Icon definitions for APP_SD
*-----------------------------------------------------------------------------
* $Date: 2010-05-12 16:31:46 +0200 (Mi, 12 Mai 2010) $
* $Rev: 13051 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/src/APP_SD_Icons.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

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
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#include "APP_SD_Icons.h"

// define icons utf8 codes here

const Uint8Type KPB_SIGNAL_STRENGTH[KPB_SIGNAL_STRENGTH_ELEMENTS][KPB_SIGNAL_STRENGTH_LENGTH]  
     = {{0xEF,0xA0,0x88, 0xEF,0xA0,0x89, 0xEF,0xA0,0x8A, 0xEF,0xA0,0x8B, 0xEF,0xA0,0x8C},  //, 0xC2,0xA1},
        {0xEF,0xA0,0x83, 0xEF,0xA0,0x89, 0xEF,0xA0,0x8A, 0xEF,0xA0,0x8B, 0xEF,0xA0,0x8C},  //, 0xC2,0xA1},
        {0xEF,0xA0,0x83, 0xEF,0xA0,0x84, 0xEF,0xA0,0x8A, 0xEF,0xA0,0x8B, 0xEF,0xA0,0x8C},  //, 0xC2,0xA1},
        {0xEF,0xA0,0x83, 0xEF,0xA0,0x84, 0xEF,0xA0,0x85, 0xEF,0xA0,0x8B, 0xEF,0xA0,0x8C},  //, 0xC2,0xA1},
        {0xEF,0xA0,0x83, 0xEF,0xA0,0x84, 0xEF,0xA0,0x85, 0xEF,0xA0,0x86, 0xEF,0xA0,0x8C},  //, 0xC2,0xA1},
        {0xEF,0xA0,0x83, 0xEF,0xA0,0x84, 0xEF,0xA0,0x85, 0xEF,0xA0,0x86, 0xEF,0xA0,0x87} };//, 0xC2,0xA1} };


const  Uint8Type KPB_BATTERY_LEVEL[KPB_BATTERY_LEVEL_ELEMENTS][KPB_BATTERY_LEVEL_LENGTH]  
                = { {0xEF,0xB0,0x84}, //, 0xC2,0xA1},    // zero lines, separator
                    {0xEF,0xB0,0x83}, //, 0xC2,0xA1},    // one line, separator
                    {0xEF,0xB0,0x82}, //, 0xC2,0xA1},    // two lines, separator
                    {0xEF,0xB0,0x81}, //, 0xC2,0xA1},    // two lines, separator
                    {0xEF,0xB0,0x80}, //, 0xC2,0xA1},    // three lines, separator
                    {0xEF,0xB0,0x80}};//, 0xC2,0xA1}};   // four lines, separator

const  Uint8Type KPB_NEW_SMS_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]            = { 0xEF, 0x9F, 0xBB, 0x20 };
const  Uint8Type KPB_MISSED_CALLS_SYMBOLD[COMMON_SYMBOL_LENGTH_OFFSET]      = { 0xEF, 0x9F, 0xBD, 0x20 };  

const  Uint8Type KPB_READ_SMS_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]           = { 0xEF, 0x9F, 0xB7, 0x20 };
const  Uint8Type KPB_UNREAD_SMS_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]         = { 0xEF, 0x9F, 0xB8, 0x20 };

const  Uint8Type KPB_PHONE_CAT_OFFICE_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]   = { 0xEF, 0x9F, 0xB4, 0x20 }; /* U+F7F4 */
const  Uint8Type KPB_PHONE_CAT_HOME_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]     = { 0xEF, 0x9F, 0xB5, 0x20 }; /* U+F7F5 */
const  Uint8Type KPB_PHONE_CAT_MOBILE_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]   = { 0xEF, 0x9F, 0xB6, 0x20 }; /* U+F7F6 */
const  Uint8Type KPB_PHONE_CAT_OTHER_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]    = { 0xEF, 0x9F, 0xB9, 0x20 }; /* U+F7F9 */
const  Uint8Type KPB_PHONE_CAT_GENERAL_SYMBOL[COMMON_SYMBOL_LENGTH_OFFSET]  = { 0xEF, 0x9F, 0xB9, 0x20 }; /* U+F7F9 */

const Uint8Type KPB_BLUETOOTH_SYMBOL[COMMON_SYMBOL_LENGTH]         = { 0xEF, 0x9F, 0xBE };
const Uint8Type KPB_BLUETOOTH_VISIBLE_SYMBOL[COMMON_SYMBOL_LENGTH] = { 0xEF, 0x9F, 0xBF };
const Uint8Type KPB_PLAYER_CONNECTED_SYMBOL[COMMON_SYMBOL_LENGTH]  = { 0xEF, 0xAC, 0x8F }; /* U+FB0F */
const Uint8Type KPB_HEADSET_CONNECTED_SYMBOL[COMMON_SYMBOL_LENGTH] = { 0xEF, 0xA0, 0x80 }; /* U+F800 */
const Uint8Type KPB_ROAMING_SYMBOL[COMMON_SYMBOL_LENGTH]           = { 0xEF, 0xB0, 0x8B }; /* U+FC0B */

/* End Of File APP_SD_ICONS.c */

