/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_CE.h
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2011-07-05 10:59:25 +0200 (Di, 05 Jul 2011) $
* $Rev: 20623 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/inc/VCR_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _VCR_CE_H
#define _VCR_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/


#define VCR_SUPPORT_MAIN_MENU_WITHOUT_MUSIC          1

/*----------------------------------------------------------------------------*/
/* SOP cw13-cw22 2010 postponed Features                                      */
/*----------------------------------------------------------------------------*/
#define VCR_SUPPORT_MAIN_MENU_WITHOUT_SMS            1

/*----------------------------------------------------------------------------*/
/* SOP 3 cw22 2011 - Remove Bluetooth option - see RT ticket 12014            */
/*----------------------------------------------------------------------------*/
#ifdef UHV_ACTIVATE_FEATURE_VCR_SOP3_PROMPTS
  #define VCR_SUPPORT_SETTING_MENU_WITHOUT_BLUETOOTH_OPTION            1
#else
  #define VCR_SUPPORT_SETTING_MENU_WITHOUT_BLUETOOTH_OPTION            0
#endif  
/*----------------------------------------------------------------------------*/
/* Config  */
/*----------------------------------------------------------------------------*/
#define VCR_CHECK_NETWORK_AVAILABILITY               0

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#define VCR_DEFAULT_LANGUAGE AT_SIVR_LANG_US_ENG
#define VCR_MAX_STRING_PROMPT_LEN 40

#define VCR_MAX_BTPIN_STRING_LEN 10

#define VCR_MAX_LANG_TABLE  (AT_SIRV_LANG_MAXNUM + 1) /* we only store up to x language idx */
#define VCR_LANG_UNDEF      AT_SIRV_LANG_MAXNUM

#define VCR_MAXNUM_RES_STORE      0x05
#define VCR_MAXNUM_PERFM_CMD      VCR_MAXNUM_RES_STORE
#define VCR_MAXNUM_PERFM_SUBCMD   0x02

/* VCR supervision timer  */
#define VCR_SUPERVISION_TIMER_ASYNC     90 /* 90 seconds for async events */
#define VCR_SUPERVISION_TIMER_OK        30 /* 30 seconds for OK */
#define VCR_PENDING_TIMER               10 /* 10 msec pending timer */
#define VCR_MDI_START_VCR_DELAY_PENDING_TIMER     1000 /* 1000 msec pending timer with delay after MDI pause OK*/

#define VCR_MUTE_DELAY_OFFSET         1750 /* msec */
#define VCR_LIST_DELAY                1500 /* msec */
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
typedef enum
{
    VCR_NOTIFY_SIVR_ACTIVATE,
    VCR_NOTIFY_SIVR_DEACTIVATE,
    VCR_NOTIFY_CONTACT_AMBIGUOUS,
    VCR_NOTIFY_MULTIPLE_NUMBERS,
    VCR_NOTIFY_DIAL_NUMBER_CHANGED,
    VCR_NOTIFY_DIAL_NUMBER_DELETE,
    VCR_NOTIFY_NOT_READY, // VCR could not used
    VCR_NOTIFY_RSTUEX_RECEIVED
} VCRNotificationIdEnum;



#endif /* _VCR_CE_H */

/* End Of File VCR_CE.h */
