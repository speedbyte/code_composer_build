/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATParser_CE.h
*-----------------------------------------------------------------------------
* Module Name:    AT Parser external configuration
*-----------------------------------------------------------------------------
* Description:    The AT Parser process received messages from Parrot module .
*-----------------------------------------------------------------------------
* $Date: 2010-12-07 16:07:56 +0100 (Di, 07 Dez 2010) $
* $Rev: 16801 $
* $Author: gross $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/inc/ATParser_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATPARSER_CE_H
#define _ATPARSER_CE_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
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

/*----------------------------------------------------------------------------*/
/* GLOBAL Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#define ASCII_CR       '\r'
#define ASCII_LF       '\n'
#define AT_ASCII_QUOTE 0x27
#define AT_ASCII_COMMA ','


/*----------------------------------------------------------------------------*/
/* GLOBAL Constants                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* GLOBAL Enumerations                                                        */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Defines                                                             */
/*----------------------------------------------------------------------------*/
#define AT_BT_ADDRESS_LEN                 12
#define AT_CALLER_ID_STRING_MAX_LEN       50
#define AT_OPERATOR_NAME_STRING_MAX_LEN   16
#define AT_OPERATOR_NAME_PIND_MAX_LEN     32
#define AT_BT_DEVICE_NAME_MAX_LEN         32
#define AT_BT_DEVICE_NAME_MIN_LEN          1
#define AT_BT_SW_VERSION_MAX_LEN          16
#define AT_BT_PF_CODE_MAX_LEN             32

#define AT_DEVICE_NAME_MAX_LEN            30
#define AT_BT_COD_LEN                      6
#define AT_BITFIELD_LEN                    4

#define AT_MAX_PARIRING_DEVICE_ID          9 /* for range 0..9 */

#ifdef UHV_SHOW_UP_TO30_DEVICES
#define AT_MAX_DEVICES_AFTER_SEARCH        29 
#endif

#define AT_INVALID_DEVICE_ID            0xFF
#define AT_PARROT_MIN_DEVICE_ID            1
#define AT_PARROT_MAX_DEVICE_ID           10
#define AT_MAX_DEVICE_ID                   (AT_PARROT_MAX_DEVICE_ID-1) /* for range 0..9 */


#define AT_MAX_DEVICE_ID_STRING_LEN        2


#ifdef UHV_ACTIVATE_FEATURE_SMS
#define AT_MAX_SMS_LEN_ISO               160
#define AT_MAX_SMS_LEN_UTF8              320
    /* CSC - workaround - set len to 100 instead to AT_MAX_SMS_LEN_UTF8 */
#define AT_MAX_SMS_LEN_TOTAL             AT_MAX_SMS_LEN_UTF8 //AT_MAX_SMS_LEN_ISO
#define AT_MAX_CONC_SMS_LEN              200 //AT_MAX_SMS_LEN_UTF8
#define AT_MAX_CONC_SMS_LEN_TOTAL        1024
#define AT_MAX_NUM_SMS_FOLDER_SIZE       300
#define AT_SMS_ORIGIN_MAX_LEN             32
#define AT_SMS_DATE_MAX_LEN               20 /* CSC TODO - may need alignment!!! */
#define AT_SMS_PHONENUM_MAX_LEN           16
#define AT_SMS_CONC_RESP_LEN               1
#else
#define AT_MAX_SMS_LEN_ISO               1
#define AT_MAX_SMS_LEN_UTF8              1
    /* CSC - workaround - set len to 100 instead to AT_MAX_SMS_LEN_UTF8 */
#define AT_MAX_SMS_LEN_TOTAL             AT_MAX_SMS_LEN_UTF8 //AT_MAX_SMS_LEN_ISO
#define AT_MAX_CONC_SMS_LEN              1 //AT_MAX_SMS_LEN_UTF8
#define AT_MAX_CONC_SMS_LEN_TOTAL        1
#define AT_MAX_NUM_SMS_FOLDER_SIZE       1
#define AT_SMS_ORIGIN_MAX_LEN             32
#define AT_SMS_DATE_MAX_LEN               20 /* CSC TODO - may need alignment!!! */
#define AT_SMS_PHONENUM_MAX_LEN           16
#define AT_SMS_CONC_RESP_LEN               1
#endif


#define AT_MDI_ENTRY_DATA_LEN             90

#define AT_CMGREX_MAX_LEN                 60

#define AT_CUSTOMER_MAX_LEN               10
#define AT_PROJECT_MAX_LEN                10
#define AT_VERSION_NUM_MAX_LEN            20
#define AT_LANGUAGE_MAX_LEN               10
#define AT_LANGUAGE_VERSION_MAX_LEN       20

#define AT_MD5_KEY_LEN                    32
#define AT_MD5_KEY_ENCRYTED_LEN           32

#define AT_PBCV_HW_VERSION_LEN            7
#define AT_PBCV_SW_VERSION_LEN            7
#define AT_PBCV_MANUFACTURER_LEN          4

// Ck5050 2.51
//#define AT_MAX_PHONEBOOK_ENTRIES        1000
// CK5050 3.10.0
//#define AT_MAX_PHONEBOOK_ENTRIES        2500
//SUSANNE: 20.10.2009 As discussed with Oli and Augustin, the max pbSize is not exactly 1000 nor 2500
// it is dynamic and depends on the structure of the contacts. So we do not need this anymore,
// and will set 0xFFFF as upper boundary for pbSize-request responses accepted in ATParser

#ifdef __cplusplus
}
#endif  /*__cplusplus */

#endif /* _ATPARSER_CE_H */

/* End Of File ATParser_CE.h */
