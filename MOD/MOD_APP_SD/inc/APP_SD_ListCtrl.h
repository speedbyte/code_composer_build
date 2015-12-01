/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_SD_FrameBuilder.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SD    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_SD
*-----------------------------------------------------------------------------
* $Date: 2010-04-09 17:12:13 +0200 (Fr, 09 Apr 2010) $
* $Rev: 12157 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/inc/APP_SD_ListCtrl.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SD_LIST_CTRL_H
#define _APP_SD_LIST_CTRL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_SD_FrameBuilder.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/


//#define APP_SD_MAX_LIST_CONTENT_LENGTH      93
#define APP_SD_MAX_LIST_REQUEST_ELEMENTS    5



/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


typedef struct
{
    struct
    {
        KPB_enCellEntryType     type;     
        BooleanType             activated;     
    }                       cell;
    KpbCellNameType         data;     
}AppSdListBufType;


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
AppSdListBufType *APP_SD_stGetListBuffer(AppSdFrameDataListCommon *pListCtrl);
BooleanType       APP_SD_LC_bDataNeed(AppSdFrameDataListCommon *pListCtrl);
Uint16Type        APP_SD_LC_u16GetSelectedIndex(const AppSdFrameDataListCommon *pListCtrl);
SuccessFailType   APP_SD_LC_sfSetSelectedIndex(AppSdFrameDataListCommon *pListCtrl, Uint16Type selIndex);
Uint16Type        APP_SD_LC_u16GetStartIndex(const AppSdFrameDataListCommon *pListCtrl);
void              APP_SD_LC_vRequestData(const AppSdFrameDataListCommon *pListCtrl);
void              APP_SD_LC_vButtonUpDown(  AppSdFrameDataListCommon *pListCtrl, 
											LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown);
SuccessFailType   APP_SD_LC_sfSendListFrame(KpbFrameDataReqType *pFrameBuffer, 
                                            AppSdFrameDataListCommon *pListCtrl,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType activate );
Uint16Type        APP_SD_LC_u16GetElemsNeeded(AppSdFrameDataListCommon *pListCtrl);
Uint16Type        APP_SD_LC_u16GetNextListIndex(AppSdFrameDataListCommon *pListCtrl);
void              APP_SD_LC_vScrollUpDown( AppSdFrameDataListCommon *pListCtrl, 
											LOGI_BUT_MAN_ENUM_BUTTON_NAMES upOrDown, 
                                           Uint16Type number);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface IAPP_SD_ERM_INTERNAL
*/
//#ifdef IAPP_SD_ERM_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_ERM_INTERNAL) */



//#endif /* END DECLARE INTERFACE(IAPP_SD_LIST_CTRL_INTERNAL) */


#endif /* _APP_SD_LIST_CTRL_H */

/* End Of File APP_SD_LIST_CTRL.h */

