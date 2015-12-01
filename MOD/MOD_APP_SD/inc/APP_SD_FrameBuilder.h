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
* $Date: 2011-06-20 10:53:23 +0200 (Mo, 20 Jun 2011) $
* $Rev: 20286 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/inc/APP_SD_FrameBuilder.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SD_FRAME_BUILDER_H
#define _APP_SD_FRAME_BUILDER_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "KPB_CE.h"
#include "ERM.h"
#include "APP_SD_FrameId.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define APP_SD_SC_SPELL_BUFFER_SIZE 50
#define APP_SD_MAX_VERSION_DATA 100
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef enum
{
    APP_SD_LIST_SCROLL_TYPE_STOP_END,
    APP_SD_LIST_SCROLL_TYPE_ENDLESS
}APP_SD_enListScrollType;

typedef struct
{
    KPB_enFrameIdType       frameId;
    APP_SD_enListScrollType scrollType;
    BooleanType             updateAttrOnly;
    Uint8Type               focusedListElemOffset;
    Uint16Type              listElemOnTop;
    Uint16Type              numListElems;    
    Uint16Type              maxElemsPerRequest;
    Uint16Type              requestedElems;
    KPB_enListOperationType listOperation;
}AppSdFrameDataListCommon;

//typedef struct
//{
//    APP_SD_enContextId        ctxId;
//    // common struct for all lists
//    AppSdFrameDataListCommon  list;
//
//}AppSdFrameDataCommon;

typedef struct
{
    APP_SD_enContextId        ctxId;
    // common struct for all lists
}AppSdFrameDataCommon;


typedef struct
{
    AppSdFrameDataCommon  common;
    AT_BTDeviceName       btName;
    Uint16Type            btName_len;  
}AppSdFrameDataS001;
 
typedef AppSdFrameDataS001 AppSdFrameDataS534;
typedef AppSdFrameDataS001 AppSdFrameDataS513;
typedef AppSdFrameDataS001 AppSdFrameDataS514;
typedef AppSdFrameDataS001 AppSdFrameDataS542;
typedef AppSdFrameDataS001 AppSdFrameDataS543;
typedef AppSdFrameDataS001 AppSdFrameDataS554;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS555;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS571;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS573;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS574;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS576;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS578;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS579;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS597;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataSC12;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS914;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS933;  // same as 001
typedef AppSdFrameDataS001 AppSdFrameDataS934;  // same as 001

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               device_type[15];
    Uint16Type              device_type_len;
    AT_BTDeviceName         btName;
    Uint16Type              btName_len;  
}AppSdFrameDataS563;
 
 
typedef struct
{
    AppSdFrameDataCommon    common;
    Uint16Type              numEntries;
}AppSdFrameDataS102;

typedef struct
{
    AppSdFrameDataCommon    common;
    AT_CallerId             name;
    Uint16Type              name_len;
}AppSdFrameDataS105;

typedef AppSdFrameDataS105 AppSdFrameDataS117;
typedef AppSdFrameDataS105 AppSdFrameDataS801;
typedef AppSdFrameDataS105 AppSdFrameDataS811;
typedef AppSdFrameDataS105 AppSdFrameDataS902;
typedef AppSdFrameDataS105 AppSdFrameDataS941;
typedef AppSdFrameDataS105 AppSdFrameDataS942;
typedef AppSdFrameDataS105 AppSdFrameDataS945;
typedef AppSdFrameDataS105 AppSdFrameDataSC20;

typedef struct
{
    Uint8Type   data[APP_SD_SC_SPELL_BUFFER_SIZE];
    Uint16Type  length;
}AppSdSpellerSelectType;

typedef struct
{
    Uint8Type   data[APP_SD_SC_SPELL_BUFFER_SIZE];
    Uint16Type  length;
}AppSdSpelledTextType;

typedef struct
{
    // TODO: spelled text should not be overwritten, when context changes due to
    // a phone event ( e.g. incoming call etc. )
    // spelled text must then be saved before context switch and restored
    // afterwards    

    AppSdFrameDataCommon    common;
    AppSdSpellerSelectType  selectedText;
    AppSdSpelledTextType    spelledText;
}AppSdFrameDataS305;

typedef AppSdFrameDataS305 AppSdFrameDataS306;
typedef AppSdFrameDataS305 AppSdFrameDataS51D;
typedef AppSdFrameDataS305 AppSdFrameDataS535;
typedef AppSdFrameDataS305 AppSdFrameDataS536;
typedef AppSdFrameDataS305 AppSdFrameDataS556;
typedef AppSdFrameDataS305 AppSdFrameDataS804;

typedef struct
{
    AppSdFrameDataCommon    common;
    ATSSPPinCodeBT          aucPinData;
}AppSdFrameDataS52C;

typedef struct
{
    // TODO: spelled text should not be overwritten, when context changes due to
    // a phone event ( e.g. incoming call etc. )
    // spelled text must then be saved before context switch and restored
    // afterwards    

    AppSdFrameDataCommon    common;
    AppSdSpellerSelectType  selectedText;
    AppSdSpelledTextType    spelledText;
    Uint8Type               device_type[15];
    Uint16Type              device_type_len;
}AppSdFrameDataS575;

typedef AppSdFrameDataS575 AppSdFrameDataS580;

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint16Type              numUser;
}AppSdFrameDataS511;




typedef struct
{
    AppSdFrameDataCommon    common;
    AT_CallerId             number;
    Uint16Type              number_len;
}AppSdFrameDataS929;


typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               call_type[15];  //array len changed from 10 to 15 for RT 1535 & 1617
    Uint16Type              call_type_len;
    AT_CallerId             name;
    Uint16Type              name_len;
}AppSdFrameDataS803;

typedef AppSdFrameDataS803 AppSdFrameDataS901;

typedef struct
{
    AppSdFrameDataCommon    common;
    AT_CallerId             name;
    Uint16Type              name_len;
    AT_CallerId             number;
    Uint16Type              number_len;
}AppSdFrameDataS212;

//typedef AppSdFrameDataS111 AppSdFrameDataS212;

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               headline[93];
    Uint16Type              headline_len;
}AppSdFrameDataS211;

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               date[20];
    Uint8Type               date_len;    
    Uint8Type               time[20];
    Uint8Type               time_len;
}AppSdFrameDataS213;


typedef struct
{
    AppSdFrameDataCommon    common;
    ATPhoneNumberType       origin;
    Uint16Type              origin_len;
}AppSdFrameDataS405;


typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               smsText[AT_MAX_SMS_LEN_TOTAL];
    Uint16Type              smsText_len;
}AppSdFrameDataS406;

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               device_name_len;
    AT_DeviceNameString     device_name;
    Uint16Type              netprov_name_len;
    AT_IndicatorString2     netprov_name;
    AT_IndicatorValue       signal_strength;
    BooleanType             signal_strength_avail;
    AT_IndicatorValue       battery_level;
    BooleanType             battery_level_avail;
}AppSdFrameDataS_18;

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               title[12];
    Uint16Type              title_len;
    AT_CallerId             name;
    Uint16Type              name_len;
}AppSdFrameDataS812;

typedef AppSdFrameDataS812 AppSdFrameDataS813;

typedef struct
{
    AppSdFrameDataCommon    common;
    Uint8Type               version[APP_SD_MAX_VERSION_DATA]; 
    Uint16Type              version_len; 
}AppSdFrameDataSB34;

typedef union 
{
    AppSdFrameDataCommon    common;
    AppSdFrameDataS001      s001;  
    AppSdFrameDataCommon    s002;
    AppSdFrameDataCommon    s007;
    AppSdFrameDataCommon    s008;
    AppSdFrameDataCommon    s009;
    AppSdFrameDataCommon    s012;
    AppSdFrameDataS_18      s_18;
    AppSdFrameDataCommon    s101;
    AppSdFrameDataS102      s102;
    AppSdFrameDataCommon    s103;
    AppSdFrameDataCommon    s104;
    AppSdFrameDataS105      s105;
//    AppSdFrameDataS111      s111;
    AppSdFrameDataCommon    s115;
    AppSdFrameDataS117      s117;
    AppSdFrameDataCommon    s201;
    AppSdFrameDataS211      s211;
    AppSdFrameDataS212      s212;
    AppSdFrameDataS213      s213;
    AppSdFrameDataCommon    s301;
    AppSdFrameDataS305      s305;
    AppSdFrameDataS306      s306;
    AppSdFrameDataCommon    s402;
    AppSdFrameDataCommon    s404;
    AppSdFrameDataS405      s405;
    AppSdFrameDataS406      s406;
    AppSdFrameDataCommon    s408;
    AppSdFrameDataCommon    s409;
    AppSdFrameDataCommon    s410;
    AppSdFrameDataCommon    s413;
    AppSdFrameDataCommon    s425;
    AppSdFrameDataCommon    s428;
    AppSdFrameDataCommon    s501;
    AppSdFrameDataCommon    s502;
    AppSdFrameDataS511      s511;
    AppSdFrameDataS513      s513;
    AppSdFrameDataS514      s514;
    AppSdFrameDataCommon    s515;
    AppSdFrameDataCommon    s517;
    AppSdFrameDataCommon    s518;
    AppSdFrameDataS51D      s51D;
    AppSdFrameDataCommon    s520;
    AppSdFrameDataCommon    s521;
    AppSdFrameDataCommon    s522;
    AppSdFrameDataS52C      s52C;
    AppSdFrameDataS534      s534;
    AppSdFrameDataS535      s535;
    AppSdFrameDataS536      s536;
    AppSdFrameDataCommon    s541;
    AppSdFrameDataS542      s542;
    AppSdFrameDataS543      s543;
    AppSdFrameDataCommon    s553;
    AppSdFrameDataS554      s554;  
    AppSdFrameDataS555      s555;  
    AppSdFrameDataS556      s556;  
    AppSdFrameDataCommon    s562;  
    AppSdFrameDataS563      s563;  
    AppSdFrameDataCommon    s567;  
    AppSdFrameDataS571      s571;
    AppSdFrameDataCommon    s572;  
    AppSdFrameDataS573      s573;  
    AppSdFrameDataS574      s574;  
    AppSdFrameDataS575      s575;  
    AppSdFrameDataS576      s576;  
    AppSdFrameDataS578      s578;
    AppSdFrameDataS579      s579;  
    AppSdFrameDataS580      s580;  
    AppSdFrameDataCommon    s581;  
    AppSdFrameDataS597      s597;  
    AppSdFrameDataCommon    s599;
    AppSdFrameDataCommon    s601;
    AppSdFrameDataCommon    s641;
    AppSdFrameDataCommon    s661;
    AppSdFrameDataCommon    s669;
    AppSdFrameDataCommon    s702;
    AppSdFrameDataS801      s801;  
    AppSdFrameDataCommon    s802;  
    AppSdFrameDataS803      s803;  
    AppSdFrameDataS804      s804;  
    AppSdFrameDataCommon    s806;
    AppSdFrameDataCommon    s806_2;
    AppSdFrameDataS811      s811;  
    AppSdFrameDataS812      s812;  
    AppSdFrameDataS813      s813;  
    AppSdFrameDataCommon    s814;
    AppSdFrameDataCommon    s815;
    AppSdFrameDataS901      s901;  
    AppSdFrameDataS902      s902;  
    AppSdFrameDataCommon    s903;
    AppSdFrameDataCommon    s904;
    AppSdFrameDataCommon    s913;
    AppSdFrameDataS914      s914;  
    AppSdFrameDataCommon    s923;
    AppSdFrameDataCommon    s924;
    AppSdFrameDataCommon    s925;
    AppSdFrameDataCommon    s927;
    AppSdFrameDataCommon    s928;
    AppSdFrameDataS929      s929;
    AppSdFrameDataCommon    s930;
    AppSdFrameDataS933      s933;
    AppSdFrameDataS934      s934;
    AppSdFrameDataS941      s941;
    AppSdFrameDataS942      s942;
    AppSdFrameDataS945      s945;
    AppSdFrameDataSC12      sc12;  
    AppSdFrameDataCommon    sc14;
    AppSdFrameDataCommon    sc17;
    AppSdFrameDataSC20      sc20; 
    AppSdFrameDataSB34      sb34; 

}AppSdFrameDataType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface IAPP_SD_ERM_INTERNAL
*/
//#ifdef IAPP_SD_ERM_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_ERM_INTERNAL) */
#include "KPB.h"

SuccessFailType _APP_SD_FB_sfSendAndActivateFrame( KpbFrameDataReqType *pFrameBuffer,
                                                   const AppSdFrameDataType *pFrameData,
                                                   BooleanType activate);
//SuccessFailType _APP_SD_FB_sfUpdateFrame( KpbFrameDataReqType *pFrameBuffer,
//                                          const AppSdFrameDataType *pFrameData);
void _APP_SD_FB_vPresReqOff( Uint32Type line );

//#endif /* END DECLARE INTERFACE(IAPP_SD_ERM_INTERNAL) */


#endif /* _APP_SD_FRAME_BUILDER_H */

/* End Of File APP_PHONE_FRAME_BUILDER.h */

