/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SD_FrameBuilder_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_SD 
*-----------------------------------------------------------------------------
* Description:    
*-----------------------------------------------------------------------------
* $Date: 2011-10-13 14:43:57 +0200 (Do, 13 Okt 2011) $
* $Rev: 22281 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/src/APP_SD_FrameBuilder_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <stdio.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_defines.h"    // vw

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "BAP_Dispatcher.h"
#include "ERM.h"
#include "APP_PHONE.h"
#include "APP_PHONE_AT.h"
#include "ATCommonData.h"
#include "KPB.h"
#include "CSHDL.h"

#include "TRACE.h"
#include "APP_SD_FrameBuilder_C1.id"

#include "APP_SD.h"
#include "APP_SD_CI.h"
#include "APP_SD_CE.h"
#include "APP_SD_Icons.h"

#include "APP_SD_FrameBuilder.h"

#include "APP_SD_FrameDataBase.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable to save the current call management function currently in progress
*/
extern AppSdContext gAppSdContext;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/


typedef  SuccessFailType (*_APP_SD_FB_sfSendFrame_t)( KpbFrameDataReqType *, const AppSdFrameDataType *, BooleanType );

static void _APP_SD_FB_vSetCell(   KpbFrameCellType *cell,   Uint8Type pos,
                                   KPB_enCellEntryType type, Uint8Type attr,
                                   const Uint8Type *content, Uint8Type length,
                                   Uint8Type offset );
static SuccessFailType _APP_SD_FB_sfSendFrameS001(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS002( KpbFrameDataReqType *pFrameBuffer,
                                                   const AppSdFrameDataType *pFrameData,
                                                   BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameS007(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS008( KpbFrameDataReqType *pFrameBuffer,
                                                   const AppSdFrameDataType *pFrameData,
                                                   BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS009(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS010( KpbFrameDataReqType *pFrameBuffer,
                                                   const AppSdFrameDataType *pFrameData,
                                                   BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS012(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS_18(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS101(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS102(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS103(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS104(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS105(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameS111(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS114(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS115(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS116(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS117(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS201(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS211(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS212(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS213(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS301(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS305(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS306(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS402(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS404(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS405(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS406(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS407(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS408(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS409(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS410(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS413(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS425(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS428(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS501(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS502(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS511(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS512(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS513(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS514(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS515(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS517(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS518(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS51D(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS520(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS521(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS522(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS52C(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);
static SuccessFailType _APP_SD_FB_sfSendFrameS52D(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameS532(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS533(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS534(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameS535(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS536(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS541(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS542(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS543(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS539(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS553(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS554(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS555(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS556(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS562(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS563(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS565(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS566(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS567(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS5B2(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS568(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS571(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS572(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS573(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS574(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS575(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS576(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameS577(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType          activate);
//
static SuccessFailType _APP_SD_FB_sfSendFrameS578(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS579(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameS580(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS581(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS582(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS597(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS599(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS601(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS602(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS641(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS642(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS661(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS663(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS664(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS669(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS702(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS801(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS802(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS803(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS804(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS807(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS806(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS811(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS812(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS813(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS814(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS815(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS901(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS902(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS903(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS904(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS913(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS914(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS916(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);
 
static SuccessFailType _APP_SD_FB_sfSendFrameS917(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS923(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS924(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS925(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS927(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS928(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS929(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS930(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);
static SuccessFailType _APP_SD_FB_sfSendFrameS933(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS934(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS935(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS941(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS942(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameS945(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC01(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC03(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC04(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC07(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC12(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC13(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC14(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC16(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

//static SuccessFailType _APP_SD_FB_sfSendFrameSC17(KpbFrameDataReqType *pFrameBuffer,
//                                                  const AppSdFrameDataType *pFrameData,
//                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC20(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC22(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC23(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSC24(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSA01(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSA02(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSA03(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static SuccessFailType _APP_SD_FB_sfSendFrameSB34(KpbFrameDataReqType *pFrameBuffer,
                                                  const AppSdFrameDataType *pFrameData,
                                                  BooleanType          activate);

static Uint8Type  _APP_SD_u8FindDynamicTextPosition( Uint8Type *targetPrompt, const Uint8Type *staticPrompt, const Uint8Type *dynamicPrompt, Uint8Type staticPromptLen );
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/* maennel/18.05.11: assert macro at button of this array will not hit if array
 * is defined with number of elements
 */
static const _APP_SD_FB_sfSendFrame_t _APP_SD_FB_sfSendFrameTable[] =
{
/* APP_SD_CONTEXT_INVALID */    NULL,
/* APP_SD_CONTEXT_S001,   */    _APP_SD_FB_sfSendFrameS001, 
/* APP_SD_CONTEXT_S002,   */    _APP_SD_FB_sfSendFrameS002,
///* APP_SD_CONTEXT_S007,   */    _APP_SD_FB_sfSendFrameS007,
/* APP_SD_CONTEXT_S008,   */    _APP_SD_FB_sfSendFrameS008,
/* APP_SD_CONTEXT_S009,   */    _APP_SD_FB_sfSendFrameS009,
/* APP_SD_CONTEXT_S010,   */    _APP_SD_FB_sfSendFrameS010,
/* APP_SD_CONTEXT_S012,   */    _APP_SD_FB_sfSendFrameS012,
/* APP_SD_CONTEXT_S018,   */    _APP_SD_FB_sfSendFrameS_18,
/* APP_SD_CONTEXT_S101,   */    _APP_SD_FB_sfSendFrameS101,
/* APP_SD_CONTEXT_S102,   */    _APP_SD_FB_sfSendFrameS102,
/* APP_SD_CONTEXT_S103,   */    _APP_SD_FB_sfSendFrameS103,
/* APP_SD_CONTEXT_S104,   */    _APP_SD_FB_sfSendFrameS104,
/* APP_SD_CONTEXT_S105,   */    _APP_SD_FB_sfSendFrameS105,
///* APP_SD_CONTEXT_S111,   */    _APP_SD_FB_sfSendFrameS111,
/* APP_SD_CONTEXT_S114,   */    _APP_SD_FB_sfSendFrameS114,
/* APP_SD_CONTEXT_S115,   */    _APP_SD_FB_sfSendFrameS115,
/* APP_SD_CONTEXT_S116,   */    _APP_SD_FB_sfSendFrameS116,
/* APP_SD_CONTEXT_S117,   */    _APP_SD_FB_sfSendFrameS117,
/* APP_SD_CONTEXT_S201,   */    _APP_SD_FB_sfSendFrameS201,
/* APP_SD_CONTEXT_S202,   */    NULL,
/* APP_SD_CONTEXT_S203,   */    NULL,
/* APP_SD_CONTEXT_S211,   */    _APP_SD_FB_sfSendFrameS211,
/* APP_SD_CONTEXT_S212,   */    _APP_SD_FB_sfSendFrameS212,
/* APP_SD_CONTEXT_S213,   */    _APP_SD_FB_sfSendFrameS213,
/* APP_SD_CONTEXT_S301,   */    _APP_SD_FB_sfSendFrameS301,
/* APP_SD_CONTEXT_S305,   */    _APP_SD_FB_sfSendFrameS305,
/* APP_SD_CONTEXT_S306,   */    _APP_SD_FB_sfSendFrameS306,
/* APP_SD_CONTEXT_S402,   */    _APP_SD_FB_sfSendFrameS402,
/* APP_SD_CONTEXT_S404,   */    _APP_SD_FB_sfSendFrameS404,
/* APP_SD_CONTEXT_S405,   */    _APP_SD_FB_sfSendFrameS405,
/* APP_SD_CONTEXT_S406,   */    _APP_SD_FB_sfSendFrameS406,
/* APP_SD_CONTEXT_S407,   */    _APP_SD_FB_sfSendFrameS407,
/* APP_SD_CONTEXT_S408,   */    _APP_SD_FB_sfSendFrameS408,
/* APP_SD_CONTEXT_S408_USERLIST*/_APP_SD_FB_sfSendFrameS408,
/* APP_SD_CONTEXT_S408_ABR*/    _APP_SD_FB_sfSendFrameS408,
/* APP_SD_CONTEXT_S408_DEL*/    _APP_SD_FB_sfSendFrameS408,
/* APP_SD_CONTEXT_S408_PLAYER*/ _APP_SD_FB_sfSendFrameS408,
/* APP_SD_CONTEXT_S408_PRECALL*/_APP_SD_FB_sfSendFrameS408,
/* APP_SD_CONTEXT_S409,   */    _APP_SD_FB_sfSendFrameS409,
/* APP_SD_CONTEXT_S410,   */    _APP_SD_FB_sfSendFrameS410,
/* APP_SD_CONTEXT_S413,   */    _APP_SD_FB_sfSendFrameS413,
/* APP_SD_CONTEXT_S425,   */    _APP_SD_FB_sfSendFrameS425,
/* APP_SD_CONTEXT_S428,   */    _APP_SD_FB_sfSendFrameS428,
/* APP_SD_CONTEXT_S501,   */    _APP_SD_FB_sfSendFrameS501,
/* APP_SD_CONTEXT_S502,   */    _APP_SD_FB_sfSendFrameS502,
/* APP_SD_CONTEXT_S511,   */    _APP_SD_FB_sfSendFrameS511,
/* APP_SD_CONTEXT_S512,   */    _APP_SD_FB_sfSendFrameS512,
/* APP_SD_CONTEXT_S513,   */    _APP_SD_FB_sfSendFrameS513,
/* APP_SD_CONTEXT_S514,   */    _APP_SD_FB_sfSendFrameS514,
/* APP_SD_CONTEXT_S515,   */    _APP_SD_FB_sfSendFrameS515,
/* APP_SD_CONTEXT_S517,   */    _APP_SD_FB_sfSendFrameS517,
/* APP_SD_CONTEXT_S517_PASSIVE, */  _APP_SD_FB_sfSendFrameS517, 
/* APP_SD_CONTEXT_S518,   */    _APP_SD_FB_sfSendFrameS518,
/* APP_SD_CONTEXT_S51D,   */    _APP_SD_FB_sfSendFrameS51D,
/* APP_SD_CONTEXT_S51D_ACCEPT,   */ _APP_SD_FB_sfSendFrameS51D,
/* APP_SD_CONTEXT_S520,   */    _APP_SD_FB_sfSendFrameS520,
/* APP_SD_CONTEXT_S521,   */    _APP_SD_FB_sfSendFrameS521,
/* APP_SD_CONTEXT_S522,   */    _APP_SD_FB_sfSendFrameS522,
/* APP_SD_CONTEXT_S52C,   */    _APP_SD_FB_sfSendFrameS52C,
/* APP_SD_CONTEXT_S52D,   */    _APP_SD_FB_sfSendFrameS52D,
///* APP_SD_CONTEXT_S532,   */    _APP_SD_FB_sfSendFrameS532,
/* APP_SD_CONTEXT_S533,   */    _APP_SD_FB_sfSendFrameS533,
/* APP_SD_CONTEXT_S533_2, */    _APP_SD_FB_sfSendFrameS533,
/* APP_SD_CONTEXT_S534,   */    _APP_SD_FB_sfSendFrameS534,
/* APP_SD_CONTEXT_S534_2, */    _APP_SD_FB_sfSendFrameS534,
///* APP_SD_CONTEXT_S535,   */    _APP_SD_FB_sfSendFrameS535,
/* APP_SD_CONTEXT_S536,   */    _APP_SD_FB_sfSendFrameS536,
/* APP_SD_CONTEXT_S539,   */    _APP_SD_FB_sfSendFrameS539,
/* APP_SD_CONTEXT_S541,   */    _APP_SD_FB_sfSendFrameS541,
/* APP_SD_CONTEXT_S541_2  */    _APP_SD_FB_sfSendFrameS541,
/* APP_SD_CONTEXT_S542,   */    _APP_SD_FB_sfSendFrameS542,
/* APP_SD_CONTEXT_S543,   */    _APP_SD_FB_sfSendFrameS543,
/* APP_SD_CONTEXT_S553,   */    _APP_SD_FB_sfSendFrameS553,
/* APP_SD_CONTEXT_S554,   */    _APP_SD_FB_sfSendFrameS554,
/* APP_SD_CONTEXT_S555,   */    _APP_SD_FB_sfSendFrameS555,
/* APP_SD_CONTEXT_S556,   */    _APP_SD_FB_sfSendFrameS556,
/* APP_SD_CONTEXT_S562,   */    _APP_SD_FB_sfSendFrameS562,
/* APP_SD_CONTEXT_S563,   */    _APP_SD_FB_sfSendFrameS563,
/* APP_SD_CONTEXT_S565,   */    _APP_SD_FB_sfSendFrameS565,
/* APP_SD_CONTEXT_S566,   */    _APP_SD_FB_sfSendFrameS566,
/* APP_SD_CONTEXT_S567,   */    _APP_SD_FB_sfSendFrameS567,
/* APP_SD_CONTEXT_S568,   */    _APP_SD_FB_sfSendFrameS568,
/* APP_SD_CONTEXT_S571,   */    _APP_SD_FB_sfSendFrameS571,
/* APP_SD_CONTEXT_S5B2,   */    _APP_SD_FB_sfSendFrameS5B2,
/* APP_SD_CONTEXT_S572,   */    _APP_SD_FB_sfSendFrameS572,
/* APP_SD_CONTEXT_S573,   */    _APP_SD_FB_sfSendFrameS573,
/* APP_SD_CONTEXT_S574,   */    _APP_SD_FB_sfSendFrameS574,
/* APP_SD_CONTEXT_S575,   */    _APP_SD_FB_sfSendFrameS575,
/* APP_SD_CONTEXT_S576,   */    _APP_SD_FB_sfSendFrameS576,
/* APP_SD_CONTEXT_S577,   */    NULL,
/* APP_SD_CONTEXT_S578,   */    _APP_SD_FB_sfSendFrameS578,
/* APP_SD_CONTEXT_S579,   */    _APP_SD_FB_sfSendFrameS579,
///* APP_SD_CONTEXT_S580,   */    _APP_SD_FB_sfSendFrameS580,
/* APP_SD_CONTEXT_S581,   */    _APP_SD_FB_sfSendFrameS581,
/* APP_SD_CONTEXT_S582,   */    _APP_SD_FB_sfSendFrameS582,
/* APP_SD_CONTEXT_S597,   */    _APP_SD_FB_sfSendFrameS597,
/* APP_SD_CONTEXT_S599,   */    _APP_SD_FB_sfSendFrameS599,
/* APP_SD_CONTEXT_S601,   */    _APP_SD_FB_sfSendFrameS601,
/* APP_SD_CONTEXT_S602,   */    _APP_SD_FB_sfSendFrameS602,
/* APP_SD_CONTEXT_S641,   */    _APP_SD_FB_sfSendFrameS641,
/* APP_SD_CONTEXT_S642,   */    _APP_SD_FB_sfSendFrameS642,
/* APP_SD_CONTEXT_S661,   */    _APP_SD_FB_sfSendFrameS661,
/* APP_SD_CONTEXT_S663,   */    _APP_SD_FB_sfSendFrameS663,
/* APP_SD_CONTEXT_S663_PB */    _APP_SD_FB_sfSendFrameS663,
/* APP_SD_CONTEXT_S664,   */    _APP_SD_FB_sfSendFrameS664,
/* APP_SD_CONTEXT_S669,   */    _APP_SD_FB_sfSendFrameS669,
/* APP_SD_CONTEXT_S702,   */    _APP_SD_FB_sfSendFrameS702,
/* APP_SD_CONTEXT_S703,   */    NULL,
/* APP_SD_CONTEXT_S704,   */    NULL,
/* APP_SD_CONTEXT_S705,   */    NULL,
/* APP_SD_CONTEXT_S706,   */    NULL,
/* APP_SD_CONTEXT_S707,   */    NULL,
/* APP_SD_CONTEXT_S709,   */    NULL,
/* APP_SD_CONTEXT_S712,   */    NULL,
/* APP_SD_CONTEXT_S713,   */    NULL,
/* APP_SD_CONTEXT_S714,   */    NULL,
/* APP_SD_CONTEXT_S715,   */    NULL,
/* APP_SD_CONTEXT_S719,   */    NULL,
/* APP_SD_CONTEXT_S801,   */    _APP_SD_FB_sfSendFrameS801,
/* APP_SD_CONTEXT_S802,   */    _APP_SD_FB_sfSendFrameS802,
/* APP_SD_CONTEXT_S803,   */    _APP_SD_FB_sfSendFrameS803,
/* APP_SD_CONTEXT_S804,   */    _APP_SD_FB_sfSendFrameS804,
/* APP_SD_CONTEXT_S807,   */    _APP_SD_FB_sfSendFrameS807,
/* APP_SD_CONTEXT_S806,   */    _APP_SD_FB_sfSendFrameS806,
/* APP_SD_CONTEXT_S811,   */    _APP_SD_FB_sfSendFrameS811,
/* APP_SD_CONTEXT_S812,   */    _APP_SD_FB_sfSendFrameS812,
/* APP_SD_CONTEXT_S813,   */    _APP_SD_FB_sfSendFrameS813,
/* APP_SD_CONTEXT_S814,   */    _APP_SD_FB_sfSendFrameS814,
/* APP_SD_CONTEXT_S815,   */    _APP_SD_FB_sfSendFrameS815,
/* APP_SD_CONTEXT_S901,   */    _APP_SD_FB_sfSendFrameS901,
/* APP_SD_CONTEXT_S902,   */    _APP_SD_FB_sfSendFrameS902,
/* APP_SD_CONTEXT_S903,   */    _APP_SD_FB_sfSendFrameS903,
/* APP_SD_CONTEXT_S904,   */    _APP_SD_FB_sfSendFrameS904,
/* APP_SD_CONTEXT_S913,   */    _APP_SD_FB_sfSendFrameS913,
/* APP_SD_CONTEXT_S914,   */    _APP_SD_FB_sfSendFrameS914,
/* APP_SD_CONTEXT_S916,   */    _APP_SD_FB_sfSendFrameS916,
/* APP_SD_CONTEXT_S917,   */    _APP_SD_FB_sfSendFrameS917,
/* APP_SD_CONTEXT_S923,   */    _APP_SD_FB_sfSendFrameS923,
/* APP_SD_CONTEXT_S924,   */    _APP_SD_FB_sfSendFrameS924,
/* APP_SD_CONTEXT_S925,   */    _APP_SD_FB_sfSendFrameS925,
/* APP_SD_CONTEXT_S927,   */    _APP_SD_FB_sfSendFrameS927,
/* APP_SD_CONTEXT_S928,   */    _APP_SD_FB_sfSendFrameS928,
/* APP_SD_CONTEXT_S929,   */    _APP_SD_FB_sfSendFrameS929,
/* APP_SD_CONTEXT_S930,   */    _APP_SD_FB_sfSendFrameS930,
/* APP_SD_CONTEXT_S933,   */    _APP_SD_FB_sfSendFrameS933,
/* APP_SD_CONTEXT_S934,   */    _APP_SD_FB_sfSendFrameS934,
/* APP_SD_CONTEXT_S935,   */    _APP_SD_FB_sfSendFrameS935,
/* APP_SD_CONTEXT_S941,   */    _APP_SD_FB_sfSendFrameS941,
/* APP_SD_CONTEXT_S942,   */    _APP_SD_FB_sfSendFrameS942,
/* APP_SD_CONTEXT_S945,   */    _APP_SD_FB_sfSendFrameS945,
/* APP_SD_CONTEXT_SC01,   */    _APP_SD_FB_sfSendFrameSC01,
/* APP_SD_CONTEXT_SC03,   */    _APP_SD_FB_sfSendFrameSC03,
/* APP_SD_CONTEXT_SC04,   */    _APP_SD_FB_sfSendFrameSC04,
/* APP_SD_CONTEXT_SC07,   */    _APP_SD_FB_sfSendFrameSC07,
/* APP_SD_CONTEXT_SC12,   */    _APP_SD_FB_sfSendFrameSC12,
/* APP_SD_CONTEXT_SC12_2, */    _APP_SD_FB_sfSendFrameSC12,
/* APP_SD_CONTEXT_SC13,   */    _APP_SD_FB_sfSendFrameSC13,
/* APP_SD_CONTEXT_SC14,   */    _APP_SD_FB_sfSendFrameSC14,
/* APP_SD_CONTEXT_SC16,   */    _APP_SD_FB_sfSendFrameSC16,
///* APP_SD_CONTEXT_SC17,   */    _APP_SD_FB_sfSendFrameSC17,
/* APP_SD_CONTEXT_SC20,   */    _APP_SD_FB_sfSendFrameSC20,
/* APP_SD_CONTEXT_SC22,   */    _APP_SD_FB_sfSendFrameSC22,
/* APP_SD_CONTEXT_SC23,   */    _APP_SD_FB_sfSendFrameSC23,
/* APP_SD_CONTEXT_SC24,   */    _APP_SD_FB_sfSendFrameSC24,
/* APP_SD_CONTEXT_SP18,   */    _APP_SD_FB_sfSendFrameS_18,
/* APP_SD_CONTEXT_SA01,   */    _APP_SD_FB_sfSendFrameSA01,
/* APP_SD_CONTEXT_SA02,   */    _APP_SD_FB_sfSendFrameSA02,
/* APP_SD_CONTEXT_SA03,   */    _APP_SD_FB_sfSendFrameSA03,
/* APP_SD_CONTEXT_SB34,   */    _APP_SD_FB_sfSendFrameSB34
};

/* if this assertion fails, the jumptable matches not the number of context ids */
CCASSERT(APP_SD_CONTEXT_LAST == NUM_ELEMENTS_OF_ARRAY(_APP_SD_FB_sfSendFrameTable), _APP_SD_FB_sfSendFrameTable);


/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendAndActivateFrame                                                   */
/** 
    \brief      decide which specialized function to be called to completely 
                build up the whole frame.
                Each supported context is then send and activated via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendAndActivateFrame(  KpbFrameDataReqType *pFrameBuffer,
                                                    const AppSdFrameDataType *pFrameData,
                                                    BooleanType activate)
{
    SuccessFailType result = SUCCESS;

    activate = (BT_FALSE == _APP_SD_bIsKl15On()) ? BT_FALSE : activate;

    if( (pFrameData->common.ctxId < NUM_ELEMENTS_OF_ARRAY(_APP_SD_FB_sfSendFrameTable)) && 
        (_APP_SD_FB_sfSendFrameTable[pFrameData->common.ctxId] != NULL) && (CSHDL_bIsUHVOnConditionSatisfied() == BT_TRUE))
    {
        result = _APP_SD_FB_sfSendFrameTable[pFrameData->common.ctxId](pFrameBuffer, pFrameData, activate);
    }
    else
    {

        DEBUG_VALUE2( APP_SD_SWI_WNG, MOD_APP_SD, "FB_SaAFrame: Unsupported ctxId %d (%d)",
                      pFrameData->common.ctxId,
                      NUM_ELEMENTS_OF_ARRAY(_APP_SD_FB_sfSendFrameTable) );
        result = FAIL;
    }


    if( result != SUCCESS )
    {
        KPB_vReleaseFrameDataElement(&pFrameBuffer, BT_FALSE);

        DEBUG_VALUE1( APP_SD_SWI_ERR, MOD_APP_SD, "FB_SaAFrame: failed (%d)", result);
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfUpdateFrame                                                   */
/** 
    \brief      used to update a frame which is already shown in instrument
                cluster 
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfUpdateFrame( KpbFrameDataReqType *pFrameBuffer,
//                                          const AppSdFrameDataType *pFrameData)
//{
//    SuccessFailType result = SUCCESS;
//    activate = BT_FALSE;
//
////    KpbFrameDataReqType *pFrameBuffer = KPB_stGetFrameDataElement();
////
////    if(pFrameBuffer == NULL)
////    {
////        DEBUG_Printf(   UHV_WARNING,
////                        UHV_APP_SD,
////                        "_APP_SD_BF_sfUpdateFrame: GetFrameData failed");
////        return FAIL;
////    }
//
//
//    switch(pFrameData->common.ctxId)
//    {
//        case APP_SD_CONTEXT_S002:
//            result = _APP_SD_FB_sfSendFrameS002(pFrameBuffer, pFrameData, activate);
//            break;
//        case APP_SD_CONTEXT_S018:
//        case APP_SD_CONTEXT_SP18:
//            result = _APP_SD_FB_sfSendFrameS_18(pFrameBuffer, pFrameData, activate);
//            break;
//        default:
//            result = FAIL;
//            break;
//    }
//
//
//    if( result != SUCCESS )
//    {
//        KPB_vReleaseFrameDataElement(pFrameBuffer, BT_FALSE);
//
//    }
//
//    return result;
//}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS001                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS001( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    // TODO: set list element on top and listOperation from pFrameData

    pFrameBuffer->frameId       = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;

    // contains a list, we add three cells for semi static info
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 3;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s001.btName_len;
    (void)memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s001.btName, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Benutzer wird");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S001_ta_note_search_user_2_Zeile_1];
    (void)memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S001_ta_note_search_user_2_Zeile_1],
            //"Benutzer wird", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("verbunden");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[APP_SD_en_S001_ta_note_search_user_2_Zeile_2];
    (void)memcpy( (pFrameBuffer->cell[2].name.content),
            gAppSdContext.prompts[APP_SD_en_S001_ta_note_search_user_2_Zeile_2],
            //"verbunden", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS002                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS002( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{

    // TODO: set list element on top and listOperation from pFrameData
    pFrameBuffer->frameId       = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;

    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 2;
    
    //gAppSdContext.prompts[APP_SD_en_S002_ta_connect_Auswahl_1],    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Kein gekoppeltes");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S002_ta_note_no_bt_phone_2_Zeile_1];
    (void)memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S002_ta_note_no_bt_phone_2_Zeile_1],
            //"Kein gekoppeltes", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Tel. gefunden");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S002_ta_note_no_bt_phone_2_Zeile_2];
    (void)memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S002_ta_note_no_bt_phone_2_Zeile_2],
            //"Tel. gefunden", 
            pFrameBuffer->cell[1].name.length);
			
    return KPB_sfSendFrameData(pFrameBuffer,activate, KPB_RELATIVE_PRIO_LOW);
   
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS007                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameS007( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY text[3];
//    Uint8Type numCells                      = 0;
//    /* frame data */  
//    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
//    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
//    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
//    
//    /* get number of cells needed by text */
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S007_ta_query_new_user_3_Zeile_1])
//    {
//        numCells++;
//    }
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S007_ta_query_new_user_3_Zeile_2])
//    {
//        numCells++;
//    }
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S007_ta_query_new_user_3_Zeile_3])
//    {
//        numCells++;
//    }
//    switch(numCells)
//    {
//        case 1:
//            text[0] = APP_SD_en_S007_ta_query_new_user_3_Zeile_3;   // should be 0
//            text[1] = APP_SD_en_S007_ta_query_new_user_3_Zeile_1;
//            text[2] = APP_SD_en_S007_ta_query_new_user_3_Zeile_2;   // should be 0
//            break;
//        default:
//            text[0] = APP_SD_en_S007_ta_query_new_user_3_Zeile_1;   // should be 0
//            text[1] = APP_SD_en_S007_ta_query_new_user_3_Zeile_2;
//            text[2] = APP_SD_en_S007_ta_query_new_user_3_Zeile_3;   // should be 0
//            break; 
//    }
//    
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[0].name.length         = strlen("Benutzerprofil");
//    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[text[0]];
//    (void)memcpy( (pFrameBuffer->cell[0].name.content), 
//            gAppSdContext.prompts[text[0]],
//            //"Benutzerprofil", 
//            pFrameBuffer->cell[0].name.length);
//
//    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[1].pos                 = 1;
//    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[1].name.length         = strlen("anlegen");
//    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[text[1]];
//    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
//            gAppSdContext.prompts[text[1]],
//            //"anlegen", 
//            pFrameBuffer->cell[1].name.length);
//
//    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[2].pos                 = 2;
//    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[2].name.length         = 0;
//    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[text[2]];
//    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
//            gAppSdContext.prompts[text[2]],
//            //"", 
//            pFrameBuffer->cell[2].name.length);
//
//
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS008                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS008( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type i            = 0; 
    Uint16Type copiedBytes = 0;
    //Uint16Type textLen     = pFrameData->s008.helpText_len;  
    Uint16Type textLen          = gAppSdContext.prompts_len[APP_SD_en_S008_ta_help_text_Langtext_1]; 
    CONST_U8_PTR_CONST_T helpText  = gAppSdContext.prompts[APP_SD_en_S008_ta_help_text_Langtext_1];

    pFrameBuffer->frameId       = KPB_FRAME_ID_5;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;

    // we add two cells with static text
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    
    // take text from pFrameData and fill up the cells 0-6 with it.

    for(i=0; i<7; i++)
    {
        Uint16Type bytesRemain = textLen - copiedBytes;
        Uint16Type bytes = ( bytesRemain > KPB_MAX_CELL_CONTENT_SIZE ) ?
                            KPB_MAX_CELL_CONTENT_SIZE : bytesRemain;

        //(void)memset(pFrameBuffer->cell[i].name.content,0,KPB_MAX_CELL_CONTENT_SIZE);

        pFrameBuffer->cell[i].isSet               = BT_TRUE;  
        pFrameBuffer->cell[i].pos                 = i;
        pFrameBuffer->cell[i].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[i].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[i].name.length         = bytes;
        (void)memcpy( (pFrameBuffer->cell[i].name.content), 
            /*pFrameData->s008.*/helpText + copiedBytes, 
            pFrameBuffer->cell[i].name.length);
        
        pFrameBuffer->arrayHeader.elements++; 
        copiedBytes += bytes;
    
        if( copiedBytes >= textLen )
        {
            /* 1. last cell reached, now set all remaining cell lengths to zero
             * to avoid old text in CI when the length of the new text is shorter
             * then the old text. This will be handled in _KPB_vRemoveRedundantCell.
             */

            i=i+1;
            for (;i<7;i++)
            {
                pFrameBuffer->cell[i].name.length = 0x00u;
                pFrameBuffer->cell[i].isSet               = BT_TRUE;
                pFrameBuffer->cell[i].pos                 = i;
                pFrameBuffer->cell[i].type                = KPB_CELL_TYPE_ANY_STRING;
                pFrameBuffer->cell[i].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
            }
            break;
        } 
    }



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS009                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/

SuccessFailType _APP_SD_FB_sfSendFrameS009(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    // S009 cancelled!!!!!!!!!!!!!!!!!!!
    // TODO: wie kommen wir hier wieder raus????
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells          = 0;
    pFrameBuffer->frameId       = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    
    // get number of cells needed for text in current language
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_3])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_4])
    {
        numCells++;
    }

    // if one or two lines text, start at row index 1, else with row index 0
    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_4; // should be 0
            rows[1] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_1;
            rows[2] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_2;
            rows[3] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_3; // should be 0
            break;
        default:
            rows[0] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_1; 
            rows[1] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_2;
            rows[2] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_3;
            rows[3] = APP_SD_en_S009_ta_note_phone_initiated_4_Zeile_4;
            break;
    }    

    // set rows into cells
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefon wird");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Telefon wird", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("initialisiert.");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void)memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"initialisiert.", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Bitte warten!");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"Bitte warten!", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("Bitte warten!");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void)memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            //"Bitte warten!", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS010                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS010( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    // get number of cells needed for text in current language
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_3])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_4])
    {
        numCells++;
    }

    // if one or two lines text, start at row index 1, else with row index 0
    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_4; // should be 0
            rows[1] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_1;
            rows[2] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_2;
            rows[3] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_3; // should be 0
            break;
        default:
            rows[0] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_1; 
            rows[1] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_2;
            rows[2] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_3;
            rows[3] = APP_SD_en_S010_ta_note_enter_cradle_4_Zeile_4;
            break;
    }    

    // 4 cells needed, using 0,1,2,3 for that ( sh. HMI spec S. 213 )
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonhalter");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                    gAppSdContext.prompts[rows[0]],
                    //"Telefonhalter",
                    pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("in Aufnahme");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                    gAppSdContext.prompts[rows[1]],
                    //"in Aufnahme",
                    pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("einsetzen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
                    gAppSdContext.prompts[rows[2]],
                    //"einsetzen",
                    pFrameBuffer->cell[2].name.length);


    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = 0;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
                    gAppSdContext.prompts[rows[3]],
                    pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS012                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS012(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    // get number of cells needed for text in current language
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S012_ta_note_no_user_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S012_ta_note_no_user_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S012_ta_note_no_user_4_Zeile_3])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S012_ta_note_no_user_4_Zeile_4])
    {
        numCells++;
    }

    // if one or two lines text, start at row index 1, else with row index 0
    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S012_ta_note_no_user_4_Zeile_4; // should be 0
            rows[1] = APP_SD_en_S012_ta_note_no_user_4_Zeile_1;
            rows[2] = APP_SD_en_S012_ta_note_no_user_4_Zeile_2;
            rows[3] = APP_SD_en_S012_ta_note_no_user_4_Zeile_3; // should be 0
            break;
        default:
            rows[0] = APP_SD_en_S012_ta_note_no_user_4_Zeile_1; 
            rows[1] = APP_SD_en_S012_ta_note_no_user_4_Zeile_2;
            rows[2] = APP_SD_en_S012_ta_note_no_user_4_Zeile_3;
            rows[3] = APP_SD_en_S012_ta_note_no_user_4_Zeile_4;
            break;
    }    

    // 4 cells needed, using 0,1,2,3 for that ( sh. HMI spec S. 213 )
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Kein Benutzer");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Kein Benutzer", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("aktiv.");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void)memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"aktiv.", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Zuerst Telefon");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"Zuerst Telefon", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("verbinden");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            //"verbinden", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS_18                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS_18( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type activeUserIndex       = 0;
    Uint8Type netProviderIndex      = 0;
    Uint8Type signalStrengthIndex   = 0;
    Uint8Type statusIconIndex       = 0;
    Uint8Type statusIconOffset      = 0;
    const Uint8Type   *pTmpPtr      = NULL; 

    if(pFrameData->common.ctxId == APP_SD_CONTEXT_S018)
    {
        pFrameBuffer->frameId = KPB_FRAME_ID_1;
        activeUserIndex       = 1;
        netProviderIndex      = 2;
        signalStrengthIndex   = 0;
        statusIconIndex       = 3;
    }
    else
    {
        pFrameBuffer->frameId = KPB_FRAME_ID_7;
        activeUserIndex       = 0;
        netProviderIndex      = 1;
        signalStrengthIndex   = 3;
        statusIconIndex       = 2;
    }

    pFrameBuffer->listElemOnTop = 0;
    pFrameBuffer->listOperation = KPB_NO_LIST_OPERATION;

    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    = 4;


    /* ------------------------------- */
    /* set name of current active user */
    _APP_SD_FB_vSetCell( & (pFrameBuffer->cell[activeUserIndex]), 
                        activeUserIndex, 
                        KPB_CELL_TYPE_ANY_STRING,
                        KPB_CELL_ATTRIBUTE_VISIBLE,
                        (Uint8Type*)pFrameData->s_18.device_name,
                        pFrameData->s_18.device_name_len,
                        0);


    /* ------------------------------- */
    /* set name of network provider    */
    _APP_SD_FB_vSetCell( &(pFrameBuffer->cell[netProviderIndex]), 
                         netProviderIndex, 
                         KPB_CELL_TYPE_ANY_STRING,
                         KPB_CELL_ATTRIBUTE_VISIBLE,
                         (Uint8Type*)pFrameData->s_18.netprov_name,
                         pFrameData->s_18.netprov_name_len,
                         0);

    /* ------------------------------- */
    /* set signal strength information */
    
    if(pFrameData->s_18.signal_strength_avail == BT_TRUE)
    {
        if(pFrameData->s_18.signal_strength < KPB_SIGNAL_STRENGTH_ELEMENTS)
        {
            pTmpPtr = KPB_SIGNAL_STRENGTH[pFrameData->s_18.signal_strength];
        }
        else
        {
            pTmpPtr = KPB_SIGNAL_STRENGTH[0];
        }
         
        _APP_SD_FB_vSetCell( &(pFrameBuffer->cell[signalStrengthIndex]), 
                             signalStrengthIndex, 
                             KPB_CELL_TYPE_ANY_STRING,
                             KPB_CELL_ATTRIBUTE_VISIBLE,
                             pTmpPtr,
                             KPB_SIGNAL_STRENGTH_LENGTH,
                             0);
    }
    else
    {
        _APP_SD_FB_vSetCell( &(pFrameBuffer->cell[signalStrengthIndex]), 
                             signalStrengthIndex,  
                             KPB_CELL_TYPE_ANY_STRING,
                             KPB_CELL_ATTRIBUTE_VISIBLE,
                             NULL,
                             0,
                             0);
    }
    
    /* ------------------------------- */
    /* set battery level information   */

    if(pFrameData->s_18.battery_level_avail == BT_TRUE)
    {
        if(pFrameData->s_18.battery_level < KPB_BATTERY_LEVEL_ELEMENTS)
        {
            pTmpPtr = KPB_BATTERY_LEVEL[pFrameData->s_18.battery_level];
        }
        else
        {
            pTmpPtr = KPB_BATTERY_LEVEL[0];
        }
     
        _APP_SD_FB_vSetCell(& (pFrameBuffer->cell[statusIconIndex]), 
                             statusIconIndex, 
                             KPB_CELL_TYPE_ANY_STRING,
                             KPB_CELL_ATTRIBUTE_VISIBLE,
                             pTmpPtr,
                             KPB_BATTERY_LEVEL_LENGTH,
                             0);
        statusIconOffset += KPB_BATTERY_LEVEL_LENGTH;
    }
    else
    {
        _APP_SD_FB_vSetCell(& (pFrameBuffer->cell[statusIconIndex]), 
                             statusIconIndex, 
                             KPB_CELL_TYPE_ANY_STRING,
                             KPB_CELL_ATTRIBUTE_VISIBLE,
                             NULL,
                             0,
                             0);
    }

    /* ------------------------------- */
    /* set bluetooth information       */
    if( BT_FALSE == ERM_bIsBluetoothVisible() )
    {
        _APP_SD_FB_vSetCell(&(pFrameBuffer->cell[statusIconIndex]), 
                            statusIconIndex, 
                            KPB_CELL_TYPE_ANY_STRING,
                            KPB_CELL_ATTRIBUTE_VISIBLE,
                            KPB_BLUETOOTH_SYMBOL,
                            sizeof(KPB_BLUETOOTH_SYMBOL),
                            statusIconOffset);
        statusIconOffset += sizeof(KPB_BLUETOOTH_SYMBOL);
    }
    else
    {
        _APP_SD_FB_vSetCell(&(pFrameBuffer->cell[statusIconIndex]), 
                            statusIconIndex, 
                            KPB_CELL_TYPE_ANY_STRING,
                            KPB_CELL_ATTRIBUTE_VISIBLE,
                            KPB_BLUETOOTH_VISIBLE_SYMBOL,
                            sizeof(KPB_BLUETOOTH_VISIBLE_SYMBOL),
                            statusIconOffset);
        statusIconOffset += sizeof(KPB_BLUETOOTH_VISIBLE_SYMBOL);
    }

    if( NULL != ERM_pGetActiveHeadsetProfile())
    {   /* add icon for connected headset */
        _APP_SD_FB_vSetCell(&(pFrameBuffer->cell[statusIconIndex]), 
                            statusIconIndex, 
                            KPB_CELL_TYPE_ANY_STRING,
                            KPB_CELL_ATTRIBUTE_VISIBLE,
                            KPB_HEADSET_CONNECTED_SYMBOL,
                            sizeof(KPB_HEADSET_CONNECTED_SYMBOL),
                            statusIconOffset);
        statusIconOffset += sizeof(KPB_HEADSET_CONNECTED_SYMBOL);
    }

    // check if there is a player connected, too */
    if( NULL != ERM_pGetActivePlayerProfile())
    {   /* add icon for connected player */
        _APP_SD_FB_vSetCell(&(pFrameBuffer->cell[statusIconIndex]), 
                        statusIconIndex, 
                        KPB_CELL_TYPE_ANY_STRING,
                        KPB_CELL_ATTRIBUTE_VISIBLE,
                        KPB_PLAYER_CONNECTED_SYMBOL,
                        sizeof(KPB_PLAYER_CONNECTED_SYMBOL),
                        statusIconOffset);
    }   
    
    return KPB_sfSendFrameData(pFrameBuffer,activate, KPB_RELATIVE_PRIO_LOW);
   
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS101                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS101( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;

    // add one static cell to data

    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->arrayHeader.start       = 0; 
    pFrameBuffer->arrayHeader.elements    += 1;

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("MENUE");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S101_ta_main_menu_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S101_ta_main_menu_1_Zeile_1],
            //"MENUE", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer,activate, KPB_RELATIVE_PRIO_LOW);
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS102                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS102( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type tempBuffer[32];
    Sint32Type num = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
	num = snprintf( (char*)tempBuffer,
				sizeof(tempBuffer),
				"%d",
				pFrameData->s102.numEntries);
    if( (num > -1) )
    {
    }
    else
    {
        /* an error occured during snprintf */
        pFrameBuffer->cell[2].name.length = 0;
    }

	/* ---------------------------------------*/
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[0].name.content, gAppSdContext.prompts[APP_SD_en_S102_ta_note_pbook_loading_3_Zeile_1], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S102_ta_note_pbook_loading_3_Zeile_1]);

	/* ---------------------------------------*/

    pFrameBuffer->cell[1].isSet               = BT_TRUE;
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S102_ta_note_pbook_loading_3_Zeile_2], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S102_ta_note_pbook_loading_3_Zeile_2]);

	/* ---------------------------------------*/

    pFrameBuffer->cell[2].isSet               = BT_TRUE;
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[2].name.content, gAppSdContext.prompts[APP_SD_en_S102_ta_note_pbook_loading_3_Zeile_3], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S102_ta_note_pbook_loading_3_Zeile_3]);

	/* ---------------------------------------*/

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS103                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS103( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                      = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    // get number of cells needed for the text
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S103_ta_note_no_entries_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S103_ta_note_no_entries_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S103_ta_note_no_entries_4_Zeile_3])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S103_ta_note_no_entries_4_Zeile_4])
    {
        numCells++;
    }

    switch( numCells )
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_3;   // should be 0
            rows[1] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_1;
            rows[2] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_2;
            rows[3] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_4;   // should be 0
            break;
        default:
            rows[0] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_1;   
            rows[1] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_2;
            rows[2] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_3;
            rows[3] = APP_SD_en_S103_ta_note_no_entries_4_Zeile_4;  
            break; 
    }


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Keine Eintraege");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"Keine Eintraege", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("vorhanden");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"vorhanden", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            //"", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS104                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS104( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    // TODO: depending on last context
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S104_ta_head_phone_book_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S104_ta_head_phone_book_1_Zeile_1],
            //"Telefonbuch", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS105                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS105( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_2;
    // TODO: depending on last context
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;

    // we add one cell with semi static text
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 1;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s105.name, pFrameData->s105.name_len))
    {
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s105.name_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s105.name, 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS111                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameS111( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    pFrameBuffer->frameId       = KPB_FRAME_ID_3;
//    pFrameBuffer->listOperation = KPB_ENTER_MENUE;
//
//    // we add two cell with semi static text
//    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
//    pFrameBuffer->arrayHeader.start       = 0;
//    pFrameBuffer->arrayHeader.elements    += 2;
//    
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    // #2255 check if this is a number, then use phone number as cell type, else any string
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s111.name, pFrameData->s111.name_len))
//    {
//        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_PHONE_NUMBER;
//    }
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[0].name.length         = pFrameData->s111.name_len;
//    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
//            pFrameData->s111.name, 
//            pFrameBuffer->cell[0].name.length);
//
//    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[1].pos                 = 1;
//    // Ticket #1786, set to cell type phone number  
//    //pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
//    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[1].name.length         = pFrameData->s111.number_len;
//    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
//            pFrameData->s111.number, 
//            pFrameBuffer->cell[1].name.length);
//
//
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS114                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS114( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;


    // currently max 2 cells are needed for the text

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            "", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Anrufname");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S114_ta_note_voice_tag_reset_2_Zeile_1];  
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S114_ta_note_voice_tag_reset_2_Zeile_1],
            //"Anrufname", 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("zurueckgesetzt");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[APP_SD_en_S114_ta_note_voice_tag_reset_2_Zeile_2];  
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[APP_SD_en_S114_ta_note_voice_tag_reset_2_Zeile_2],
            //"zurueckgesetzt", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            "", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS115                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS115( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId                   = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation             = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data     = KPB_ARRAY_MODE_3_COMBINED;

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anrufname");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S115_ta_head_reset_voice_tag_2_Zeile_1];  
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S115_ta_head_reset_voice_tag_2_Zeile_1],
            //"Anrufname", 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("zuruecksetzen?");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S115_ta_head_reset_voice_tag_2_Zeile_2];  
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S115_ta_head_reset_voice_tag_2_Zeile_2],
            //"zuruecksetzen?", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS116                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS116( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    // get number of cells needed for the text
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_3])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_4])
    {
        numCells++;
    }

    switch( numCells )
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_4;   // should be 0
            rows[1] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_1;
            rows[2] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_2;
            rows[3] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_3;   // should be 0
            break;
        default:
            rows[0] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_1;   
            rows[1] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_2;
            rows[2] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_3;
            rows[3] = APP_SD_en_S116_ta_note_tag_no_memory_4_Zeile_4;  
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Speicher voll.");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];  
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Speicher voll.", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Keine Aufnahme");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];  
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"Keine Aufnahme", 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("moeglich");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];  
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"moeglich", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = 0;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];  
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            //"", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS117                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS117( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;

    // we add two cell with semi static text
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 2;

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s117.name, pFrameData->s117.name_len))
    {
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s117.name_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s117.name, 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S117_ta_head_nametag_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S117_ta_head_nametag_1_Zeile_1],
            //"Anrufname", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS201                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS201( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anruflisten");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S201_call_head_register_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S201_call_head_register_1_Zeile_1],
            //"Anruflisten", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS211                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS211( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s211.headline_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s211.headline, 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS213                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS213( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s213.date_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s213.date, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1786, headings have type any string
    //pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s213.time_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s213.time, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS212                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS212( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;

    // we add two cell with semi static text
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 2;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s212.name, pFrameData->s212.name_len))
    {
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s212.name_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s212.name, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1786, use type phone number 
    //pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_DIRECTORY;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s212.number_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s212.number, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS301                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS301( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S301_ta_note_enter_number_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S301_ta_note_enter_number_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S301_ta_note_enter_number_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S301_ta_note_enter_number_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_4; //should be 0
            rows[1] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_1;
            rows[2] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_2;
            rows[3] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_3; //should be 0
            break;
        default:
            rows[0] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_1;
            rows[1] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_2;
            rows[2] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_3;
            rows[3] = APP_SD_en_S301_ta_note_enter_number_4_Zeile_4;
            break;
    }
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Bitte");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content),
            gAppSdContext.prompts[rows[0]], 
            //"Bitte", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Rufnummer");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]], 
            //"Rufnummer", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("eingeben");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]], 
            //"eingeben", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = 0;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]], 
            //"", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS305                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS305( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint16Type maxCharPerCell           = KPB_u16GetNumChar();
    Uint16Type tmpLen                   = 0;

    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Nummernwahl");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S305_ta_head_dial_nbr_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S305_ta_head_dial_nbr_1_Zeile_1],
            //"Nummernwahl", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s305.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s305.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    tmpLen = (pFrameData->s305.spelledText.length > sizeof(pFrameBuffer->cell[2].name.content) ? 
              sizeof(pFrameBuffer->cell[2].name.content) : pFrameData->s305.spelledText.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = tmpLen;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s305.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    // empty cell
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0; 
    (void) memset( (pFrameBuffer->cell[3].name.content), 
                    0x00,
                    sizeof(pFrameBuffer->cell[3].name.content));



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS306                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS306( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Sprachmailbox:");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S306_ta_head_mailbox_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S306_ta_head_mailbox_1_Zeile_1],
            //"Sprachmailbox:", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s306.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s306.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s306.spelledText.length;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s306.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS402                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS402( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0< gAppSdContext.prompts_len[APP_SD_en_S402_ta_note_no_sms_4_Zeile_1])
    {
        numCells++;
    }
    if( 0< gAppSdContext.prompts_len[APP_SD_en_S402_ta_note_no_sms_4_Zeile_2])
    {
        numCells++;
    }
    if( 0< gAppSdContext.prompts_len[APP_SD_en_S402_ta_note_no_sms_4_Zeile_3])
    {
        numCells++;
    }
    if( 0< gAppSdContext.prompts_len[APP_SD_en_S402_ta_note_no_sms_4_Zeile_4])
    {
        numCells++;
    }

    switch( numCells )
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_4;
            rows[1] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_1;
            rows[2] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_2;
            rows[3] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_3;
            break;
        default:
            rows[0] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_1;
            rows[1] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_2;
            rows[2] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_3;
            rows[3] = APP_SD_en_S402_ta_note_no_sms_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Keine");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];    
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Keine", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Mitteilungen");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];    
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"Mitteilungen", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("vorhanden");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];    
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"vorhanden", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = 0;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];    
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            //"", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS404                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS404( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Mitteilungen");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S404_head_messages_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S404_head_messages_1_Zeile_1],
            //"Mitteilungen", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS405                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS405( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Mitteilung von");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S405_ta_head_sms_from_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S405_ta_head_sms_from_1_Zeile_1],
            //"Mitteilung von", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s405.origin, pFrameData->s405.origin_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s405.origin_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s405.origin, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS406                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS406( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type i            = 0; 
    Uint16Type copiedBytes = 0;
    Uint16Type textLen     = pFrameData->s406.smsText_len;  

    pFrameBuffer->frameId       = KPB_FRAME_ID_5;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;

    // we add two cells with static text
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    //pFrameBuffer->arrayHeader.elements    += 2;
    
    // take text from pFrameData and fill up the cells 0-6 with it.
    for(i=0; i<7; i++)
    {
        Uint16Type bytesRemain = textLen - copiedBytes;
        Uint16Type bytes = ( bytesRemain > KPB_MAX_CELL_CONTENT_SIZE ) ?
                            KPB_MAX_CELL_CONTENT_SIZE : bytesRemain;

        pFrameBuffer->cell[i].isSet               = BT_TRUE;  
        pFrameBuffer->cell[i].pos                 = i;
        pFrameBuffer->cell[i].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[i].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[i].name.length         = bytes;
        // RT #1175: Help and SMS text together
        if( bytes > 0 )
        {
                (void)memcpy( (pFrameBuffer->cell[i].name.content), 
                        pFrameData->s406.smsText + copiedBytes, 
                        pFrameBuffer->cell[i].name.length);

            copiedBytes += bytes;
        } 
        pFrameBuffer->arrayHeader.elements++; 
    }



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS408                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS408( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    /* Bitte warten */
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    // get number of cells needed for text
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S408_ta_wait_general_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S408_ta_wait_general_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S408_ta_wait_general_4_Zeile_3])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S408_ta_wait_general_4_Zeile_4])
    {
        numCells++;
    }
 
    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S408_ta_wait_general_4_Zeile_4; // should be 0 
            rows[1] = APP_SD_en_S408_ta_wait_general_4_Zeile_1; 
            rows[2] = APP_SD_en_S408_ta_wait_general_4_Zeile_2; 
            rows[3] = APP_SD_en_S408_ta_wait_general_4_Zeile_3; // should be 0
            break;
        default:
            rows[0] = APP_SD_en_S408_ta_wait_general_4_Zeile_1; 
            rows[1] = APP_SD_en_S408_ta_wait_general_4_Zeile_2; 
            rows[2] = APP_SD_en_S408_ta_wait_general_4_Zeile_3; 
            rows[3] = APP_SD_en_S408_ta_wait_general_4_Zeile_4; 
            break;
     }


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS407                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS407( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];   
    Uint8Type numCells                     = 0;
    pFrameBuffer->frameId                  = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation            = KPB_MENU_CHANGE;

    // contains a list, we add three cells for static info
    // use cell 0,1,2 for this
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 3;
    

    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S407_ta_function_restriction_3_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S407_ta_function_restriction_3_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S407_ta_function_restriction_3_Zeile_3])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S407_ta_function_restriction_3_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S407_ta_function_restriction_3_Zeile_1;
            row[2] = APP_SD_en_S407_ta_function_restriction_3_Zeile_2;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S407_ta_function_restriction_3_Zeile_1;   
            row[1] = APP_SD_en_S407_ta_function_restriction_3_Zeile_2;
            row[2] = APP_SD_en_S407_ta_function_restriction_3_Zeile_3;   
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Funktion nur");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Funktion nur", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("bei stehendem");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"bei stehendem", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Fahrzeug");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"Fahrzeug", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS409                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS409( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];   
    Uint8Type numCells                     = 0;
    pFrameBuffer->frameId       = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;

    // contains a list, we add one cells for static info
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 3;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_3])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_1;
            row[2] = APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_2;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_1;   
            row[1] = APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_2;
            row[2] = APP_SD_en_S409_ta_note_sms_no_read_3_Zeile_3; 
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Mitteilung kann");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Mitteilung kann", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("nicht angezeigt");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"nicht angezeigt", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("werden");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"werden", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS410                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS410( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];   
    Uint8Type numCells                     = 0;
    pFrameBuffer->frameId       = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;

    // contains a list, we add one cells for static info
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 3;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S410_ta_wait_general_4_Zeile_1])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S410_ta_wait_general_4_Zeile_2])
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S410_ta_wait_general_4_Zeile_3])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S410_ta_wait_general_4_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S410_ta_wait_general_4_Zeile_1;
            row[2] = APP_SD_en_S410_ta_wait_general_4_Zeile_2;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S410_ta_wait_general_4_Zeile_1;   
            row[1] = APP_SD_en_S410_ta_wait_general_4_Zeile_2;
            row[2] = APP_SD_en_S410_ta_wait_general_4_Zeile_3; 
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Bitte warten!");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            //"Bitte warten!", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS413                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS413( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Mitteilung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S413_ta_query_delete_sms_2_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S413_ta_query_delete_sms_2_Zeile_1],
            //"Mitteilung", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("loeschen?");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S413_ta_query_delete_sms_2_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S413_ta_query_delete_sms_2_Zeile_2],
            //"loeschen?", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS425                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS425( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_4; 
            rows[1] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_1; 
            rows[2] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_2; 
            rows[3] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_3; 
            break;
        default:
            rows[0] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_1; 
            rows[1] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_2; 
            rows[2] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_3; 
            rows[3] = APP_SD_en_S425_ta_note_sms_deleted_4_Zeile_4; 
            break;
    }        


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Mitteilung");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]], 
            //"Mitteilung", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("geloescht");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]], 
            //"geloescht", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS428                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS428( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_4; 
            rows[1] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_1; 
            rows[2] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_2; 
            rows[3] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_3; 
            break;
        default:
            rows[0] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_1; 
            rows[1] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_2; 
            rows[2] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_3; 
            rows[3] = APP_SD_en_S428_ta_note_sms_not_deleted_4_Zeile_4; 
            break;
    }        

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Loeschen der");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];  
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Loeschen der", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Mitteilung");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];  
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"Mitteilung", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("fehlgeschlagen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];  
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"fehlgeschlagen", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 2;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];  
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS501                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS501( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S501_ta_note_no_phone_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S501_ta_note_no_phone_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S501_ta_note_no_phone_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S501_ta_note_no_phone_4_Zeile_1])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_4; 
            rows[1] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_1; 
            rows[2] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_2; 
            rows[3] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_3; 
            break;
        default:
            rows[0] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_1; 
            rows[1] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_2; 
            rows[2] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_3; 
            rows[3] = APP_SD_en_S501_ta_note_no_phone_4_Zeile_4; 
            break;
    }        


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Keine");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];      
    (void) memcpy( (pFrameBuffer->cell[0].name.content),
            gAppSdContext.prompts[rows[0]], 
            //"Keine", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("kompatiblen");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];      
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]], 
            //"kompatiblen", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Mobiltelefone");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];      
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]], 
            //"Mobiltelefone", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("gefunden");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];      
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]], 
            //"gefunden", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS502                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS502( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    // TODO: list operation depending on predecessor context
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Bluetooth");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S502_head_bluetooth_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S502_head_bluetooth_1_Zeile_1],
            //"Bluetooth", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS511                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS511( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Sint32Type                      num = 0;
    Uint8Type tempBuffer[32];

    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

	num = snprintf( (char*)tempBuffer,
				sizeof(tempBuffer),
				"%d",
				ERM_MAX_NUM_USER_PROFILES /*pFrameData->s511.numUser*/);
	if ( num >-1 )
	{

	}
	else
	{
		tempBuffer[0] = '\0';
	}
	/* ---------------------------------------*/
	pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[0].name.content, gAppSdContext.prompts[APP_SD_en_S511_ta_note_max_users_4_Zeile_1], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S511_ta_note_max_users_4_Zeile_1]);

	/* ---------------------------------------*/

	pFrameBuffer->cell[1].isSet               = BT_TRUE;
	pFrameBuffer->cell[1].pos                 = 1;
	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_MENU_ITEM;
	pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
	pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S511_ta_note_max_users_4_Zeile_2], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S511_ta_note_max_users_4_Zeile_2]);

	/* ---------------------------------------*/

        
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[2].name.content, gAppSdContext.prompts[APP_SD_en_S511_ta_note_max_users_4_Zeile_3], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S511_ta_note_max_users_4_Zeile_3]);

	/* ---------------------------------------*/

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[3].name.content, gAppSdContext.prompts[APP_SD_en_S511_ta_note_max_users_4_Zeile_4], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S511_ta_note_max_users_4_Zeile_4]);

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_u8FindDynamicTextPosition                                                   */
/**
    \brief      Finde dynamic Text ( %U ) position in the whole text.

    \param      targetPrompt
                Uint8Type *
                pointer to the cell content which will be displayed.

    \param      staticPrompt
                const Uint8Type *
                pointer to the static prompt

    \param      dynamicPrompt
                Uint8Type *
                pointer to the dynamic prompt

    \param      staticPromptLen
                Uint8Type
                length of the static prompt.

    \return     Uint8Type
    			length of the entire cell including dynamic text if any.

*/
/*----------------------------------------------------------------------------*/
static Uint8Type  _APP_SD_u8FindDynamicTextPosition( Uint8Type *targetPrompt, const Uint8Type *staticPrompt, const Uint8Type *dynamicPrompt, Uint8Type staticPromptLen )
{
// Find % U
	Uint8Type i = 0, j = 0;

	while ( i < staticPromptLen)
	{
		if ( staticPrompt[i] == '%')
		{
			if ( (staticPrompt[i+1] == 'U'))
			{
				(void) memcpy( &targetPrompt[j],
						dynamicPrompt,
						strlen((const char *)dynamicPrompt));
				i += 2;
				j += strlen((const char *)dynamicPrompt);
				DEBUG_TEXT(APP_SD_SWI_TRC, MOD_APP_SD, "Found %U");
			}
			else
			{
				targetPrompt[j] = staticPrompt[i];
				i++;
				j++;
			}
		}
		else
		{
			targetPrompt[j] = staticPrompt[i];
			i++;
			j++;
		}
	}
	DEBUG_VALUE1(APP_SD_SWI_TRC, MOD_APP_SD, "Total Len = %d", j);
	return (j);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS512                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS512( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Benutzer");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S512_ta_head_delete_user_2_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S512_ta_head_delete_user_2_Zeile_1],
            //"Benutzer", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("loeschen");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S512_ta_head_delete_user_2_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S512_ta_head_delete_user_2_Zeile_2],
            //"loeschen", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS513                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS513( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type tempBuffer[32];

    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    (void) memcpy( tempBuffer,
    		pFrameData->s513.btName,
    		pFrameData->s513.btName_len);
    tempBuffer[pFrameData->s513.btName_len] = '\0';

    DEBUG_VALUE1(APP_SD_SWI_TRC, MOD_APP_SD, "Device Length = %d", pFrameData->s513.btName_len);

	/* ---------------------------------------*/
	pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[0].name.content, gAppSdContext.prompts[APP_SD_en_S513_ta_query_delete_user_2_Zeile_1], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S513_ta_query_delete_user_2_Zeile_1]);

	pFrameBuffer->cell[1].isSet               = BT_TRUE;
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S513_ta_query_delete_user_2_Zeile_2], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S513_ta_query_delete_user_2_Zeile_2]);


    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS514                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS514( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type tempBuffer[32];
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    (void) memcpy( tempBuffer,
    		pFrameData->s514.btName,
    		pFrameData->s514.btName_len);
    tempBuffer[pFrameData->s514.btName_len] = '\0';

    DEBUG_VALUE1(APP_SD_SWI_TRC, MOD_APP_SD, "Device Length = %d", pFrameData->s514.btName_len);

	/* ---------------------------------------*/
    pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0; // gAppSdContext.prompts_len[row[0]];

	/* ---------------------------------------*/

    pFrameBuffer->cell[1].isSet               = BT_TRUE;
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Verbunden mit");
    pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S514_ta_note_user_deleted_2_Zeile_1], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S514_ta_note_user_deleted_2_Zeile_1]);

	/* ---------------------------------------*/

    pFrameBuffer->cell[2].isSet               = BT_TRUE;
    pFrameBuffer->cell[2].pos                 = 2;
	pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[2].name.content, gAppSdContext.prompts[APP_SD_en_S514_ta_note_user_deleted_2_Zeile_2], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S514_ta_note_user_deleted_2_Zeile_2]);

	/* ---------------------------------------*/
	// empty
	pFrameBuffer->cell[3].isSet               = BT_TRUE;
	pFrameBuffer->cell[3].pos                 = 3;
	pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
	pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
	pFrameBuffer->cell[3].name.length         = 0;

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS515                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS515( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    // TODO: list operation depending on predecessor context
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Geraeteliste");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S515_head_device_list_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S515_head_device_list_1_Zeile_1],
            //"Geraeteliste", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS517                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS517( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
   
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_4;// should be 0
            rows[1] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_1;
            rows[2] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_2;
            rows[3] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_3;// should be 0 
            break;
        default:
            rows[0] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_1;
            rows[1] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_2;
            rows[2] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_3;
            rows[3] = APP_SD_en_S517_ta_note_bt_conn_setup_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Verbindung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Verbindung", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("wird");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"wird", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("hergestellt");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]],
            //"hergestellt", 
            pFrameBuffer->cell[2].name.length);
    
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS518                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS518( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_4;// should be 0
            rows[1] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_1;
            rows[2] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_2;
            rows[3] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_3;// should be 0 
            break;
        default:
            rows[0] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_1;
            rows[1] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_2;
            rows[2] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_3;
            rows[3] = APP_SD_en_S518_ta_note_bt_no_conn_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Verbindung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Verbindung", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("nicht");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"nicht", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("moeglich");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content),  
            gAppSdContext.prompts[rows[2]], 
            //"moeglich", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS51D                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS51D( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Passwort:");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S51D_ta_head_passkey_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S51D_ta_head_passkey_1_Zeile_1],
            //"Passwort:", 
            pFrameBuffer->cell[0].name.length);


    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s51D.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s51D.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s51D.spelledText.length;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s51D.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS520                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS520( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                      = 0;
    /* frame data */  
    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S520_ta_note_search_phones_3_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S520_ta_note_search_phones_3_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S520_ta_note_search_phones_3_Zeile_3])
    {
        numCells++;
    }
    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S520_ta_note_search_phones_3_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S520_ta_note_search_phones_3_Zeile_1;
            row[2] = APP_SD_en_S520_ta_note_search_phones_3_Zeile_2;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S520_ta_note_search_phones_3_Zeile_1 ;  // should be 0
            row[1] = APP_SD_en_S520_ta_note_search_phones_3_Zeile_2;
            row[2] = APP_SD_en_S520_ta_note_search_phones_3_Zeile_3;   // should be 0
            break; 
    }


    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    pFrameBuffer->arrayHeader.start       = 0;  // 
    pFrameBuffer->arrayHeader.elements    += 3;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Mobiltelefone");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Mobiltelefone", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("werden gesucht.");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"werden gesucht.", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Bitte warten!");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"Bitte warten!", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS521                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS521( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numCells                      = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_1;
            row[2] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_2;   
            row[3] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_4;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_1 ;  
            row[1] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_2;
            row[2] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_3;  
            row[3] = APP_SD_en_S521_ta_note_bt_passkey_err_4_Zeile_4; 
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anmeldung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Anmeldung", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("nicht");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"nicht", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("moeglich");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content),
            gAppSdContext.prompts[row[2]], 
            //"moeglich", 
            pFrameBuffer->cell[2].name.length);
    
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content),
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS522                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS522( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numCells                      = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S522_ta_music_paused_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S522_ta_music_paused_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S522_ta_music_paused_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S522_ta_music_paused_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S522_ta_music_paused_4_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S522_ta_music_paused_4_Zeile_1;
            row[2] = APP_SD_en_S522_ta_music_paused_4_Zeile_2;   
            row[3] = APP_SD_en_S522_ta_music_paused_4_Zeile_4;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S522_ta_music_paused_4_Zeile_1 ; 
            row[1] = APP_SD_en_S522_ta_music_paused_4_Zeile_2;
            row[2] = APP_SD_en_S522_ta_music_paused_4_Zeile_3; 
            row[3] = APP_SD_en_S522_ta_music_paused_4_Zeile_4;
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Musik-");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Musik-", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("wiedergabe");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"wiedergabe", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("zeitweise");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"zeitweise", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("angehalten");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content),
            gAppSdContext.prompts[row[3]],
            //"angehalten", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS52C                                                   */
/**
    \brief      completely build up the whole frame, then send and activate
                frame via KPB module

    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
                Status of function run
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS52C( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_6;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    pFrameBuffer->arrayHeader.start       = 0;
    pFrameBuffer->arrayHeader.elements    += 3;

    pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Passwort mit OK");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S52C_ta_confirm_pw_with_OK_2_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content),
            gAppSdContext.prompts[APP_SD_en_S52C_ta_confirm_pw_with_OK_2_Zeile_1],
            //"Passwort mit OK",
            pFrameBuffer->cell[0].name.length);


    pFrameBuffer->cell[1].isSet               = BT_TRUE;
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S52C_ta_confirm_pw_with_OK_2_Zeile_2];
    //pFrameBuffer->cell[1].name.length         = strlen("bestätigen:");
    (void) memcpy( (pFrameBuffer->cell[1].name.content),
            gAppSdContext.prompts[APP_SD_en_S52C_ta_confirm_pw_with_OK_2_Zeile_2],
            //"bestätigen:",
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = strlen((const char*)pFrameData->s52C.aucPinData);
    (void) memcpy( (pFrameBuffer->cell[2].name.content),
            pFrameData->s52C.aucPinData,
            pFrameBuffer->cell[2].name.length);

//    pFrameBuffer->cell[3].isSet               = BT_TRUE;
//    pFrameBuffer->cell[3].pos                 = 3;
//    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[3].name.length         = 0;

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS52C                                                   */
/**
    \brief      completely build up the whole frame, then send and activate
                frame via KPB module

    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
                Status of function run
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS52D( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;


    pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Passwort im");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S52D_ta_confirm_pw_in_device_3_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content),
            gAppSdContext.prompts[APP_SD_en_S52D_ta_confirm_pw_in_device_3_Zeile_1],
            //"Passwort im",
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Telefon");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S52D_ta_confirm_pw_in_device_3_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content),
            gAppSdContext.prompts[APP_SD_en_S52D_ta_confirm_pw_in_device_3_Zeile_2],
            //"Telefon",
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("bestätigen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[APP_SD_en_S52D_ta_confirm_pw_in_device_3_Zeile_3];
    (void) memcpy( (pFrameBuffer->cell[2].name.content),
            gAppSdContext.prompts[APP_SD_en_S52D_ta_confirm_pw_in_device_3_Zeile_3],
            //"bestätigen",
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS532                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameS532( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
//    Uint8Type numCells                  = 0;
//    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
//    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
//    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
//        
//    /* get number of cells needed by text */
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S532_ta_note_name_exists_4_Zeile_1])
//    {
//        numCells++;
//    }
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S532_ta_note_name_exists_4_Zeile_2])
//    {
//        numCells++;
//    }
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S532_ta_note_name_exists_4_Zeile_3])
//    {
//        numCells++;
//    }
//    if(0 < gAppSdContext.prompts_len[APP_SD_en_S532_ta_note_name_exists_4_Zeile_4])
//    {
//        numCells++;
//    }
//
//    switch(numCells)
//    {
//        case 1:
//            row[0] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_3;   // should be 0
//            row[1] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_1;
//            row[2] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_2;   
//            row[3] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_4;   // should be 0
//            break;
//        default:
//            row[0] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_1 ; 
//            row[1] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_2;
//            row[2] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_3; 
//            row[3] = APP_SD_en_S532_ta_note_name_exists_4_Zeile_4;
//            break; 
//    }
//    
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
//    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
//            gAppSdContext.prompts[row[0]], 
//            pFrameBuffer->cell[0].name.length);
//
//    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[1].pos                 = 1;
//    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[1].name.length         = strlen("Name bereits");
//    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
//    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
//            gAppSdContext.prompts[row[1]],
//            //"Name bereits", 
//            pFrameBuffer->cell[1].name.length);
//    
//    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[2].pos                 = 2;
//    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[2].name.length         = strlen("vergeben");
//    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
//    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
//            gAppSdContext.prompts[row[2]],
//            //"vergeben", 
//            pFrameBuffer->cell[2].name.length);
// 
//    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[3].pos                 = 3;
//    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
//    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
//            gAppSdContext.prompts[row[3]], 
//            pFrameBuffer->cell[3].name.length);
//
//
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS533                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS533( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S533_ta_note_name_stored_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S533_ta_note_name_stored_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S533_ta_note_name_stored_4_Zeile_3])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S533_ta_note_name_stored_4_Zeile_4])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_1;
            row[2] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_2;   
            row[3] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_4;   // should be 0
            break;
        default:
            row[0] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_1 ; 
            row[1] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_2;
            row[2] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_3; 
            row[3] = APP_SD_en_S533_ta_note_name_stored_4_Zeile_4;
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
    
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Name");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"Name", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("gespeichert");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"gespeichert", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS534                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS534( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S534_ta_note_new_user_3_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S534_ta_note_new_user_3_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S534_ta_note_new_user_3_Zeile_3])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            row[0] = APP_SD_en_S534_ta_note_new_user_3_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S534_ta_note_new_user_3_Zeile_1;
            row[2] = APP_SD_en_S534_ta_note_new_user_3_Zeile_2;   
            break;
        default:
            row[0] = APP_SD_en_S534_ta_note_new_user_3_Zeile_1 ; 
            row[1] = APP_SD_en_S534_ta_note_new_user_3_Zeile_2;
            row[2] = APP_SD_en_S534_ta_note_new_user_3_Zeile_3; 
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s534.btName_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s534.btName, 
            pFrameBuffer->cell[0].name.length);
    

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Benutzerprofil");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Benutzerprofil", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("erfolgreich");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[1]],
            //"erfolgreich", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("angelegt");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[2]],
            //"angelegt", 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS535                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameS535( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
//    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
//    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
//     
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[0].name.length         = strlen("Benutzer");
//    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S535_ta_head_user_1_Zeile_1];
//    (void) memcpy( (pFrameBuffer->cell[0].name.content),
//            gAppSdContext.prompts[APP_SD_en_S535_ta_head_user_1_Zeile_1], 
//            //"Benutzer", 
//            pFrameBuffer->cell[0].name.length);
//
//    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[1].pos                 = 1;
//    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
//    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[1].name.length         = pFrameData->s535.selectedText.length;
//    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
//            pFrameData->s535.selectedText.data, 
//            pFrameBuffer->cell[1].name.length);
//
//    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[2].pos                 = 2;
//    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
//    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[2].name.length         = pFrameData->s535.spelledText.length;
//    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
//            pFrameData->s535.spelledText.data, 
//            pFrameBuffer->cell[2].name.length);
//
//    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[3].pos                 = 3;
//    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[3].name.length         = 0;
//
//
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS536                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS536( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Benutzer:");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S536_ta_head_user_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content),
            gAppSdContext.prompts[APP_SD_en_S536_ta_head_user_1_Zeile_1], 
            //"Benutzer:", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s536.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s536.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s536.spelledText.length;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s536.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS539                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS539( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S539_ta_note_user_not_created_3_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S539_ta_note_user_not_created_3_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S539_ta_note_user_not_created_3_Zeile_3])
    {
        numCells++;
    }

    switch(numCells)
    {
        case 1:
        case 2:
            row[0] = APP_SD_en_S539_ta_note_user_not_created_3_Zeile_3;   // should be 0
            row[1] = APP_SD_en_S539_ta_note_user_not_created_3_Zeile_1;
            row[2] = APP_SD_en_S539_ta_note_user_not_created_3_Zeile_2;   
            break;
        default:
            row[0] = APP_SD_en_S539_ta_note_user_not_created_3_Zeile_1; 
            row[1] = APP_SD_en_S539_ta_note_user_not_created_3_Zeile_2;
            row[2] = APP_SD_en_S539_ta_note_user_not_created_3_Zeile_3; 
            break; 
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content),
            gAppSdContext.prompts[row[1]], 
            //"Benutzer nicht", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("angelegt");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"angelegt", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS541                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS541( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Benutzer");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S541_head_user_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S541_head_user_1_Zeile_1],
            //"Benutzer", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS542                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/

/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS542( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Benutzer:");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S542_ta_head_user_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S542_ta_head_user_1_Zeile_1],
            //"Benutzer:", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s542.btName_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s542.btName, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS543                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS543( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[3] = { APP_SD_enError,APP_SD_enError,APP_SD_enError };
    Uint8Type numStaticCells            = 0;    
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S543_ta_note_user_connected_3_Zeile_1])
    {
        numStaticCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S543_ta_note_user_connected_3_Zeile_2])
    {
        numStaticCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S543_ta_note_user_connected_3_Zeile_3])
    {
        numStaticCells++;
    }

    // one cell is used for btName
    switch(numStaticCells)
    { 
        case 1:
            /* 2 cells, 1 for static, 1 for btName */
            /* btName is at index 2 */
            rows[0] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_3;// should be 0
            rows[1] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_1;
            rows[2] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_2;// should be 0 
            break;
        case 2:
            /* 3 cells, 2 for static, 1 for btName */
            /* btName is at index 2 */
            rows[0] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_1;
            rows[1] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_2;
            rows[2] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_3;// should be 0
            break;
        case 3:
            /* 4 cells, 3 for static, one for btName */
            /* btName is at index 3 */
            rows[0] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_1;
            rows[1] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_2;
            rows[2] = APP_SD_en_S543_ta_note_user_connected_3_Zeile_3;
            break;
        default:
            break;
    }

    /* we assume that there is at least 1 line static text */
    /* that means btName must be at index 2 or 3 */    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Bereits");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]],
            //"Bereits", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("verbunden mit");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"verbunden mit", 
            pFrameBuffer->cell[1].name.length);

    if( numStaticCells == 1 ||
        numStaticCells == 2 )
    {        
            /* put btName at index 2 */
            pFrameBuffer->cell[2].isSet               = BT_TRUE;  
            pFrameBuffer->cell[2].pos                 = 2;
            pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
            pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
            pFrameBuffer->cell[2].name.length         = pFrameData->s543.btName_len;
            (void) memcpy( (pFrameBuffer->cell[2].name.content), 
                    pFrameData->s543.btName, 
                    pFrameBuffer->cell[2].name.length);

            /* empty */
            pFrameBuffer->cell[3].isSet               = BT_TRUE;  
            pFrameBuffer->cell[3].pos                 = 3;
            pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
            pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
            pFrameBuffer->cell[3].name.length         = 0;
    }
    else
    {
        /* put btName at index 3 */
        pFrameBuffer->cell[2].isSet               = BT_TRUE;  
        pFrameBuffer->cell[2].pos                 = 2;
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
        (void) memcpy( (pFrameBuffer->cell[2].name.content), 
                gAppSdContext.prompts[rows[2]], 
                pFrameBuffer->cell[2].name.length);

        pFrameBuffer->cell[3].isSet               = BT_TRUE;  
        pFrameBuffer->cell[3].pos                 = 3;
        pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[3].name.length         = pFrameData->s543.btName_len;
        (void) memcpy( (pFrameBuffer->cell[3].name.content), 
                pFrameData->s543.btName, 
                pFrameBuffer->cell[3].name.length);
    }



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS553                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS553( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    // TODO: list operation depending on predecessor context
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Sichtbarkeit");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S553_ta_query_visibility_2_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S553_ta_query_visibility_2_Zeile_1],
            //"Sichtbarkeit", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("einschalten?");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S553_ta_query_visibility_2_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S553_ta_query_visibility_2_Zeile_2],
            //"einschalten?", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS554                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS554( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[3];
    Uint8Type numStaticCells            = 0;    
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* get number of cells needed by text */
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S554_ta_note_visible_3_Zeile_1])
    {
        numStaticCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S554_ta_note_visible_3_Zeile_2])
    {
        numStaticCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S554_ta_note_visible_3_Zeile_3])
    {
        numStaticCells++;
    }

    switch(numStaticCells)
    {
        case 1:
            /* btName is at index 1, static cell text at index 2 */
            rows[0] = APP_SD_en_S554_ta_note_visible_3_Zeile_3; // should be 0
            rows[1] = APP_SD_en_S554_ta_note_visible_3_Zeile_1; 
            rows[2] = APP_SD_en_S554_ta_note_visible_3_Zeile_2;
            break;
        default:
            rows[0] = APP_SD_en_S554_ta_note_visible_3_Zeile_1; 
            rows[1] = APP_SD_en_S554_ta_note_visible_3_Zeile_2; 
            rows[2] = APP_SD_en_S554_ta_note_visible_3_Zeile_3;
            break;
    }

    if(numStaticCells == 1)
    {
        /* put btName at index 1, static cell directly after btName */
        /* use first static cell here */
        pFrameBuffer->cell[0].isSet               = BT_TRUE;  
        pFrameBuffer->cell[0].pos                 = 0;
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
        (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                gAppSdContext.prompts[rows[0]], 
                pFrameBuffer->cell[0].name.length);
    
        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[1].name.length         = pFrameData->s554.btName_len;
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                pFrameData->s554.btName, 
                pFrameBuffer->cell[1].name.length);
    }
    else
    {
        /* put btName at index 0, static cells directly after btName */
        pFrameBuffer->cell[0].isSet               = BT_TRUE;  
        pFrameBuffer->cell[0].pos                 = 0;
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[0].name.length         = pFrameData->s554.btName_len;
        (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                pFrameData->s554.btName, 
                pFrameBuffer->cell[0].name.length);
    
        /* use first static cell here */
        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        //pFrameBuffer->cell[1].name.length         = strlen("fuer 3 Minuten");
        pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[0]];
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                gAppSdContext.prompts[rows[0]],
                //"fuer 3 Minuten", 
                pFrameBuffer->cell[1].name.length);
    }

    /* use second static cell here */
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("sichtbar");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[1]],
            //"sichtbar", 
            pFrameBuffer->cell[2].name.length);

    /* use third static cell here */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[2]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS555                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS555( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s555.btName_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s555.btName, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("aktiv. Loeschen?");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S555_ta_query_del_act_user_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S555_ta_query_del_act_user_1_Zeile_1],
            //"aktiv. Loeschen?", 
            pFrameBuffer->cell[1].name.length);

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS556                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS556( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType  *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonname");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S556_ta_head_phone_name_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S556_ta_head_phone_name_1_Zeile_1],
            //"Telefonname", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s556.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s556.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s556.spelledText.length;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s556.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS562                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS562( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Headset"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S562_head_headset_1_Zeile_1]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S562_head_headset_1_Zeile_1],
            //"Headset", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS563                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS563( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s563.device_type_len; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s563.device_type, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s563.btName_len; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s563.btName, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS565                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS565( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;    
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
        
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_1])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_2])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_3])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_4])
    {
        numStaticCells++;
    }

    switch(numStaticCells)
    {
        case 1:
        case 2:
            row[0] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_4;//should be 0
            row[1] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_1;
            row[2] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_2;
            row[3] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_3;//should be 0
            break;
        default:
            row[0] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_1;
            row[1] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_2;
            row[2] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_3;
            row[3] = APP_SD_en_S565_ta_note_conn_terminated_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Verbindung"); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"Verbindung", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("getrennt"); 
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]]; 
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"getrennt", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]]; 
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS566                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS566( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;    
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S566_ta_note_no_act_device_4_Zeile_1])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S566_ta_note_no_act_device_4_Zeile_2])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S566_ta_note_no_act_device_4_Zeile_3])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S566_ta_note_no_act_device_4_Zeile_4])
    {
        numStaticCells++;
    }

    switch(numStaticCells)
    {
        case 1:
        case 2:
            row[0] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_4;//should be 0
            row[1] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_1;
            row[2] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_2;
            row[3] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_3;//should be 0
            break;
        default:
            row[0] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_1;
            row[1] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_2;
            row[2] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_3;
            row[3] = APP_SD_en_S566_ta_note_no_act_device_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Kein Geraet"); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"Kein Geraet", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("aktiv"); 
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]]; 
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"aktiv", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]]; 
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS567                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS567( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Gekopp. Geraete"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S567_paired_head_devices_1_Zeile_1]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S567_paired_head_devices_1_Zeile_1],
            //"Gekopp. Geraete", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS5B2                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS5B2( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Gekopp. Geraete"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S5B2_paired_head_devices_1_Zeile_1]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S5B2_paired_head_devices_1_Zeile_1],
            //"Gekopp. Geraete", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS568                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS568( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;    
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_1])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_2])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_3])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_4])
    {
        numStaticCells++;
    }

    switch(numStaticCells)
    {
        case 1:
        case 2:
            row[0] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_4;//should be 0
            row[1] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_1;
            row[2] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_2;
            row[3] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_3;//should be 0
            break;
        default:
            row[0] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_1;
            row[1] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_2;
            row[2] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_3;
            row[3] = APP_SD_en_S568_ta_note_no_paired_dev_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Keine"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Keine", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("gekoppelten"); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"gekoppelten", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Geraete"); 
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]]; 
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"Geraete", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]]; 
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS571                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS571( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s571.btName_len; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s571.btName, 
            pFrameBuffer->cell[0].name.length);
    
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = 0; 



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS572                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS572( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Aktuelle Verb."); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S572_ta_query_terminate_conn_2_Zeile_1]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S572_ta_query_terminate_conn_2_Zeile_1],
            //"Aktuelle Verb.", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("trennen?"); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S572_ta_query_terminate_conn_2_Zeile_2]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S572_ta_query_terminate_conn_2_Zeile_2],
            //"trennen?", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS573                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS573( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numStaticCells            = 0;    
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S573_ta_note_user_connected_3_Zeile_1])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S573_ta_note_user_connected_3_Zeile_2])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S573_ta_note_user_connected_3_Zeile_3])
    {
        numStaticCells++;
    }

    switch(numStaticCells)
    {
        case 1:
            row[0] = APP_SD_en_S573_ta_note_user_connected_3_Zeile_3;//should be 0
            row[1] = APP_SD_en_S573_ta_note_user_connected_3_Zeile_1;
            row[2] = APP_SD_en_S573_ta_note_user_connected_3_Zeile_2;
            break;
        default:
            row[0] = APP_SD_en_S573_ta_note_user_connected_3_Zeile_1;
            row[1] = APP_SD_en_S573_ta_note_user_connected_3_Zeile_2;
            row[2] = APP_SD_en_S573_ta_note_user_connected_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Bereits"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Bereits", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("verbunden mit"); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"verbunden mit", 
            pFrameBuffer->cell[1].name.length);

    if( numStaticCells == 1 ||
        numStaticCells == 2 )
    {    
        /* but btName at index 2 */
        pFrameBuffer->cell[2].isSet               = BT_TRUE;  
        pFrameBuffer->cell[2].pos                 = 2;
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[2].name.length         = pFrameData->s573.btName_len; 
        (void) memcpy( (pFrameBuffer->cell[2].name.content), 
                pFrameData->s573.btName, 
                pFrameBuffer->cell[2].name.length);

        /* index 3 is empty */
        pFrameBuffer->cell[3].isSet               = BT_TRUE;  
        pFrameBuffer->cell[3].pos                 = 3;
        pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[3].name.length         = 0; 
    }
    else
    {
        pFrameBuffer->cell[2].isSet               = BT_TRUE;  
        pFrameBuffer->cell[2].pos                 = 1;
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[1]]; 
        (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            pFrameBuffer->cell[2].name.length);

        /* put btName at index 3 */
        pFrameBuffer->cell[3].isSet               = BT_TRUE;  
        pFrameBuffer->cell[3].pos                 = 3;
        pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[3].name.length         = pFrameData->s573.btName_len; 
        (void) memcpy( (pFrameBuffer->cell[3].name.content), 
                pFrameData->s573.btName, 
                pFrameBuffer->cell[3].name.length);
    }    



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS574                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS574( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    Uint8Type tempBuffer[32];
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    (void) memcpy( tempBuffer,
    		pFrameData->s574.btName,
    		pFrameData->s574.btName_len);
    tempBuffer[pFrameData->s574.btName_len] = '\0';

    DEBUG_VALUE1(APP_SD_SWI_TRC, MOD_APP_SD, "Device Length = %d", pFrameData->s574.btName_len);

	/* ---------------------------------------*/
    pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0; // gAppSdContext.prompts_len[row[0]];

	/* ---------------------------------------*/

    pFrameBuffer->cell[1].isSet               = BT_TRUE;
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Verbunden mit");
    pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S574_ta_note_connected_2_Zeile_1], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S574_ta_note_connected_2_Zeile_1]);

	/* ---------------------------------------*/

    pFrameBuffer->cell[2].isSet               = BT_TRUE;
    pFrameBuffer->cell[2].pos                 = 2;
	pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[2].name.content, gAppSdContext.prompts[APP_SD_en_S574_ta_note_connected_2_Zeile_2], tempBuffer, gAppSdContext.prompts_len[APP_SD_en_S574_ta_note_connected_2_Zeile_2]);

	/* ---------------------------------------*/
	// empty
	pFrameBuffer->cell[3].isSet               = BT_TRUE;
	pFrameBuffer->cell[3].pos                 = 3;
	pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
	pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
	pFrameBuffer->cell[3].name.length         = 0;

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS575                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS575( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    // TODO: Speller shall send UP/Down list operation when up / down was
    // pressed. We need to set listOperation in APP_SD, not in FrameBuilder
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s575.device_type_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s575.device_type, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s575.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s575.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s575.spelledText.length;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s575.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS576                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS576( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s576.btName_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s576.btName, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("loeschen?");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S576_ta_query_del_device_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S576_ta_query_del_device_1_Zeile_1],
            //"loeschen?", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

///*----------------------------------------------------------------------------*/
///* Function    : _APP_SD_FB_sfSendFrameS577                                                   */
///** 
//    \brief      completely build up the whole frame, then send and activate 
//                frame via KPB module
//               
//    \param      pFrameBuffer
//                KpbFrameDataReqType *
//                pointer to send buffer
//
//    \param      pFrameData          
//                AppSdFrameDataType *
//                pointer to frame data which shall be send
//
//    \param      activate
//                BooleanType
//                whether to activate the frame or not
//
//    \return     SuccessFailType
//    			Status of function run
//    			SUCCESS, FAIL
//*/
///*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameS577( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
//    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
//    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
// 
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[0].name.length         = strlen("Autorisierung:");
//    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S577_ta_head_authorisation_1_Zeile_1];
//    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
//            gAppSdContext.prompts[APP_SD_en_S577_ta_head_authorisation_1_Zeile_1],
//            //"Autorisierung:", 
//            pFrameBuffer->cell[0].name.length);
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS578                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS578( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[2];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S578_ta_note_deleted_2_Zeile_1])
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S578_ta_note_deleted_2_Zeile_2])
    {
        numStaticCells++;
    }

    switch(numStaticCells)
    {
        case 1:
            row[0] = APP_SD_en_S578_ta_note_deleted_2_Zeile_2;//should be 0
            row[1] = APP_SD_en_S578_ta_note_deleted_2_Zeile_1;
            break;
        default:
            row[0] = APP_SD_en_S578_ta_note_deleted_2_Zeile_1;
            row[1] = APP_SD_en_S578_ta_note_deleted_2_Zeile_2;
            break;
    }

    if( numStaticCells == 1 )
    {
        /* put btName at index 1 */
        /* empty, cuase static text will follow btName */
        pFrameBuffer->cell[0].isSet               = BT_TRUE;  
        pFrameBuffer->cell[0].pos                 = 0;
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[0].name.length         = 0;

        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[1].name.length         = pFrameData->s578.btName_len;
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                pFrameData->s578.btName, 
                pFrameBuffer->cell[1].name.length);
    }
    else
    {
        /* put btName at index 0 */
        pFrameBuffer->cell[0].isSet               = BT_TRUE;  
        pFrameBuffer->cell[0].pos                 = 1;
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[0].name.length         = pFrameData->s578.btName_len;
        (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                pFrameData->s578.btName, 
                pFrameBuffer->cell[0].name.length);

        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_MENU_ITEM;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[0]];
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                gAppSdContext.prompts[row[0]],
                pFrameBuffer->cell[1].name.length);
    }

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_MENU_ITEM;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("geloescht");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[1]],
            //"geloescht", 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS579                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS579( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s579.btName_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s579.btName, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("aktiv. Loeschen?");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S579_ta_query_del_act_device_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S579_ta_query_del_act_device_1_Zeile_1],
            //"aktiv. Loeschen?", 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS580                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameS580( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    //TODO: list operation for up / down    
//    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
//    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
//    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
//     
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[0].name.length         = pFrameData->s580.device_type_len;
//    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
//            pFrameData->s580.device_type, 
//            pFrameBuffer->cell[0].name.length);
//
//
//    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[1].pos                 = 1;
//    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
//    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[1].name.length         = pFrameData->s580.selectedText.length;
//    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
//            pFrameData->s580.selectedText.data, 
//            pFrameBuffer->cell[1].name.length);
//
//    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[2].pos                 = 2;
//    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
//    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[2].name.length         = pFrameData->s580.spelledText.length;
//    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
//            pFrameData->s580.spelledText.data, 
//            pFrameBuffer->cell[2].name.length);
//
//    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[3].pos                 = 3;
//    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[3].name.length         = 0;
//
//
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS581                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS581( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_3] )
    {
        numCells++;
    }

    switch( numCells )  
    {
        case 1:
            row[0]  = APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_3;
            row[1]  = APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_1;
            row[2]  = APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_1;
            row[1]  = APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_2;
            row[2]  = APP_SD_en_S581_ta_wait_bt_acc_search_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("HeadSet wird"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Headset wird", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("gesucht."); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"gesucht.", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Bitte warten!"); 
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]]; 
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"Bitte warten!", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS582                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS582( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S582_ta_note_no_accessory_4_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S582_ta_note_no_accessory_4_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S582_ta_note_no_accessory_4_Zeile_3] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S582_ta_note_no_accessory_4_Zeile_4] )
    {
        numCells++;
    }

    switch( numCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_4;
            row[1]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_1;
            row[2]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_2;
            row[3]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_3;
            break;
        default:
            row[0]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_1;
            row[1]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_2;
            row[2]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_3;
            row[3]  = APP_SD_en_S582_ta_note_no_accessory_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Kein Headset");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"Kein Headset", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("gefunden");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"gefunden", 
            pFrameBuffer->cell[2].name.length);
 
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]],
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS597                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS597( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[2];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S597_ta_disconnected_2_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S597_ta_disconnected_2_Zeile_2] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
            row[0]  = APP_SD_en_S597_ta_disconnected_2_Zeile_2;    // should be 0
            row[1]  = APP_SD_en_S597_ta_disconnected_2_Zeile_1;
            break;
        default:
            row[0]  = APP_SD_en_S597_ta_disconnected_2_Zeile_1;
            row[1]  = APP_SD_en_S597_ta_disconnected_2_Zeile_2;
            break;
    }

    if( numStaticCells == 1)
    {   /* put btName at index 1 */
        /* empty */
        pFrameBuffer->cell[0].isSet               = BT_TRUE;  
        pFrameBuffer->cell[0].pos                 = 0;
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[0].name.length         = 0;
        /* btName */
        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[1].name.length         = pFrameData->s597.btName_len;
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                pFrameData->s597.btName, 
                pFrameBuffer->cell[1].name.length);
    }
    else
    {   /* put btName at index 0 */
        /* btName */
        pFrameBuffer->cell[0].isSet               = BT_TRUE;  
        pFrameBuffer->cell[0].pos                 = 0;
        pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[0].name.length         = pFrameData->s597.btName_len;
        (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                pFrameData->s597.btName, 
                pFrameBuffer->cell[0].name.length);

        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[0]];
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                gAppSdContext.prompts[row[0]],
                //"getrennt", 
                pFrameBuffer->cell[1].name.length);
    }

    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[1]] ;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[2].name.length);
    
    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS599                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS599( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Mobiltelefon ist nicht kompatibel */
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S599_ta_note_no_rsap_3_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S599_ta_note_no_rsap_3_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S599_ta_note_no_rsap_3_Zeile_3] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S599_ta_note_no_rsap_3_Zeile_3;    // should be 0
            row[1]  = APP_SD_en_S599_ta_note_no_rsap_3_Zeile_1;
            row[2]  = APP_SD_en_S599_ta_note_no_rsap_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_S599_ta_note_no_rsap_3_Zeile_1;
            row[1]  = APP_SD_en_S599_ta_note_no_rsap_3_Zeile_2;
            row[2]  = APP_SD_en_S599_ta_note_no_rsap_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/******************************************************************************/
// PHONE SETTINGS
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS601                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS601( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Einstellungen */
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S601_head_settings_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S601_head_settings_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS602                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS602( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Audio Mode: */
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S602_head_audio_mode_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S602_head_audio_mode_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS641                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS641( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Signaleinstell. */
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S641_ta_head_setting_signals_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S641_ta_head_setting_signals_1_Zeile_1],
            //"Signaleinstell.", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS642                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS642( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Aktiver Rufton: */
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Aktiver Rufton:");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S642_ta_head_active_tone_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S642_ta_head_active_tone_1_Zeile_1],
            //"Aktiver Rufton:", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS661                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS661( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId       = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S661_ta_head_phone_book_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S661_ta_head_phone_book_1_Zeile_1],
            //"Telefonbuch", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS663                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS663( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Telefonbuch wird geladen */
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_4; // should be 0
            row[1]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_1;
            row[2]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_2;
            row[3]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_3; // should be 0
            break;
        default:
            row[0]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_1;
            row[1]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_2;
            row[2]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_3;
            row[3]  = APP_SD_en_S663_ta_note_pb_loading_all_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("wird geladen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS664                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS664( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Telefonbuch kann nicht geladen werden */
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_4; // should be 0
            row[1]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_1;
            row[2]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_2;
            row[3]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_3; // should be 0
            break;
        default:
            row[0]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_1;
            row[1]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_2;
            row[2]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_3;
            row[3]  = APP_SD_en_S664_ta_note_pb_load_error_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Telefonbuch", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("kann nicht");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"kann nicht", 
            pFrameBuffer->cell[1].name.length);
    
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("geladen werden");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"geladen werden", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS669                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS669( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Sortierung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S669_ta_head_sorting_order_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S669_ta_head_sorting_order_1_Zeile_1],
            //"Sortierung", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS702                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS702( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    /* frame not needed, Recording of voice memo cancelled */
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = strlen("Telefonbuch");
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            "Telefonbuch", 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}



/******************************************************************************/
// CALL HANDLING
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS801                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS801( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Rufaufbau");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S801_ta_head_call_setup_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S801_ta_head_call_setup_1_Zeile_1],
            //"Rufaufbau", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // # Ticket 1552
   	// pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;

    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s801.name, pFrameData->s801.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s801.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s801.name, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS802                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS802( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    /*pFrameBuffer->frameId               = KPB_FRAME_ID_4;*/     // RT #3188
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;         // RT #3188
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    // RT #3188, the handling of the different error state was postponed to SOP2
    // to SOP1 only the screen "Rufaufbau fehlgeschlagen" was showen for every error
    // and a timeout for 5 sec. was added.
    if( 0 < gAppSdContext.prompts_len[/*APP_SD_en_S802_ta_note_conn_err_3_Zeile_1*/APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[/*APP_SD_en_S802_ta_note_conn_err_3_Zeile_2*/APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[/*APP_SD_en_S802_ta_note_conn_err_3_Zeile_3*/APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_3] )
    {
        numCells++;
    }

//    switch( numCells )  
//    {
//        case 1:
//            row[0]  = APP_SD_en_S802_ta_note_conn_err_3_Zeile_3;
//            row[1]  = APP_SD_en_S802_ta_note_conn_err_3_Zeile_1;
//            row[2]  = APP_SD_en_S802_ta_note_conn_err_3_Zeile_2;
//            break;
//        default:
//            row[0]  = APP_SD_en_S802_ta_note_conn_err_3_Zeile_1;
//            row[1]  = APP_SD_en_S802_ta_note_conn_err_3_Zeile_2;
//            row[2]  = APP_SD_en_S802_ta_note_conn_err_3_Zeile_3;
//            break;
//    }

    switch( numCells )  
    {
        case 1:
            row[0]  = APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_3;
            row[1]  = APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_1;
            row[2]  = APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_1;
            row[1]  = APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_2;
            row[2]  = APP_SD_en_S802_ta_note_call_estab_failed_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]]; 
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS803                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS803( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s803.call_type_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s803.call_type, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1552
    //pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s803.name, pFrameData->s803.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s803.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s803.name, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS804                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS804( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_ENTER_MENUE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
     
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("DTMF");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S804_ta_head_dtmf_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S804_ta_head_dtmf_1_Zeile_1],
            //"DTMF", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_SPELLER;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s804.selectedText.length;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s804.selectedText.data, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s804.spelledText.length;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s804.spelledText.data, 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS807                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS807( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{   /* Keine Rufnummer */ /* Frame S807 */
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S807_ta_note_no_nbr_3_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S807_ta_note_no_nbr_3_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S807_ta_note_no_nbr_3_Zeile_3] )
    {
        numCells++;
    }

    switch( numCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S807_ta_note_no_nbr_3_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S807_ta_note_no_nbr_3_Zeile_1;
            row[2]  = APP_SD_en_S807_ta_note_no_nbr_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_S807_ta_note_no_nbr_3_Zeile_1;
            row[1]  = APP_SD_en_S807_ta_note_no_nbr_3_Zeile_2;
            row[2]  = APP_SD_en_S807_ta_note_no_nbr_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Keine");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Rufnummer");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);
    
    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS806                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS806( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
      
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S806_ta_note_invalid_number_3_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S806_ta_note_invalid_number_3_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S806_ta_note_invalid_number_3_Zeile_3] )
    {
        numCells++;
    }

    switch( numCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S806_ta_note_invalid_number_3_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S806_ta_note_invalid_number_3_Zeile_1;
            row[2]  = APP_SD_en_S806_ta_note_invalid_number_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_S806_ta_note_invalid_number_3_Zeile_1;
            row[1]  = APP_SD_en_S806_ta_note_invalid_number_3_Zeile_2;
            row[2]  = APP_SD_en_S806_ta_note_invalid_number_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Rufnummer");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("ungueltig");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);
 
    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS811                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS811( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anruf gehalten:");

    // check if we show a conference or a single call as hold
    if( gAppSdContext.pCurCtxCtrl->s811.plccIndex == AT_CALL_INDEX_CONFERENCE )
    {
        pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S811_ta_head_conference_hold_2_Zeile_1];
        (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                gAppSdContext.prompts[APP_SD_en_S811_ta_head_conference_hold_2_Zeile_1],
                //"Konferenz gehalten:"
                pFrameBuffer->cell[0].name.length);

        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;

        pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S811_ta_head_conference_hold_2_Zeile_2];
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                gAppSdContext.prompts[APP_SD_en_S811_ta_head_conference_hold_2_Zeile_2],
                //"Konferenz gehalten:"
                pFrameBuffer->cell[1].name.length);
    }
    else
    {
        pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S811_ta_head_call_hold_1_Zeile_1];
        (void) memcpy( (pFrameBuffer->cell[0].name.content), 
                gAppSdContext.prompts[APP_SD_en_S811_ta_head_call_hold_1_Zeile_1],
                //"Anruf gehalten:", 
                pFrameBuffer->cell[0].name.length);

        pFrameBuffer->cell[1].isSet               = BT_TRUE;  
        pFrameBuffer->cell[1].pos                 = 1;
        // Ticket #1552
       	//pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        // #2255: if we can detect a phone number, then change cell type
        // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
        // else a too long name would be shortened the wrong way ( left side )
       	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
        if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s811.name, pFrameData->s811.name_len))
        {
            pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
        }
        pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[1].name.length         = pFrameData->s811.name_len;
        (void) memcpy( (pFrameBuffer->cell[1].name.content), 
                pFrameData->s811.name, 
                pFrameBuffer->cell[1].name.length);
    }



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS812                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS812( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s812.title_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s812.title, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1552
   	//pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s812.name, pFrameData->s812.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s812.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s812.name, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS813                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS813( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anruf 1");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S813_ta_head_call1_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S813_ta_head_call1_1_Zeile_1],
            //"Anruf 1", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s813.name, pFrameData->s813.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s813.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s813.name, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS814                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS814(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_3] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_3] )
    {
        numCells++;
    }

    switch( numCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_1;
            row[2]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_2;
            row[3]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_1;
            row[1]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_2;
            row[2]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_3;
            row[3]  = APP_SD_en_S814_ta_note_conf_not_poss_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Konferenz");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("nicht");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("moeglich");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);
    
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS815                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS815(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Konferenz");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S815_ta_head_conference_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S815_ta_head_conference_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS901                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS901(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->s901.call_type_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->s901.call_type, 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1552
    //pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s901.name, pFrameData->s901.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s901.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s901.name, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS902                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS902(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anruf klopft an");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S902_ta_head_call_waiting_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S902_ta_head_call_waiting_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1552
    //pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s902.name, pFrameData->s902.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->s902.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->s902.name, 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS903                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS903(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Neue");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S903_ta_note_new_sms_2_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S903_ta_note_new_sms_2_Zeile_1], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Mitteilung");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S903_ta_note_new_sms_2_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S903_ta_note_new_sms_2_Zeile_2], 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS904                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS904(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anruf in");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S904_ta_note_missed_call_2_Zeile_1] ;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_S904_ta_note_missed_call_2_Zeile_1], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Abwesenheit");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S904_ta_note_missed_call_2_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S904_ta_note_missed_call_2_Zeile_2], 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS913                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS913(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_1] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_2] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_3] )
    {
        numCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_4] )
    {
        numCells++;
    }

    switch( numCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_1;
            row[2]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_2;
            row[3]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_1;
            row[1]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_2;
            row[2]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_3;
            row[3]  = APP_SD_en_S913_ta_note_no_bt_conn_rsap_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Keine Bluetooth-");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Verbindung zum");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Mobiltelefon");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

     
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS914                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS914(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;

    DEBUG_VALUE1(APP_SD_SWI_TRC, MOD_APP_SD, "Device Length = %d", pFrameData->s914.btName_len);

	/* ---------------------------------------*/
	pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[0].name.content, gAppSdContext.prompts[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_1], pFrameData->s914.btName, gAppSdContext.prompts_len[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_1]);

	/* ---------------------------------------*/

	pFrameBuffer->cell[1].isSet               = BT_TRUE;
	pFrameBuffer->cell[1].pos                 = 1;
	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
	pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
	pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_2], pFrameData->s914.btName, gAppSdContext.prompts_len[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_2]);

	/* ---------------------------------------*/


    pFrameBuffer->cell[2].isSet               = BT_TRUE;
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[2].name.content, gAppSdContext.prompts[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_3], pFrameData->s914.btName, gAppSdContext.prompts_len[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_3]);

	/* ---------------------------------------*/

    pFrameBuffer->cell[3].isSet               = BT_TRUE;
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[3].name.content, gAppSdContext.prompts[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_4], pFrameData->s914.btName, gAppSdContext.prompts_len[APP_SD_en_S914_ta_note_no_bt_conn_acc_4_Zeile_4]);

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS916                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS916(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_1;
            row[2]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_2;
            row[3]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_1;
            row[1]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_2;
            row[2]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_3;
            row[3]  = APP_SD_en_S916_ta_note_no_bt_conn_rsap_4_Zeile_4; 
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Keine Bluetooth-");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Verbindung zu");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Mobiltelefon");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS917                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS917( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY rows[4];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S917_ta_phone_no_answer_4_Zeile_1])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S917_ta_phone_no_answer_4_Zeile_2])
    {
        numCells++;
    }
    if(0 < gAppSdContext.prompts_len[APP_SD_en_S917_ta_phone_no_answer_4_Zeile_3])
    {
        numCells++;
    }    

    switch(numCells)
    {
        case 1:
        case 2:
            rows[0] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_4;
            rows[1] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_1;
            rows[2] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_2;
            rows[3] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_3;
            break;
        case 3:
        default:
            rows[0] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_1;
            rows[1] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_2;
            rows[2] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_3;
            rows[3] = APP_SD_en_S917_ta_phone_no_answer_4_Zeile_4;
            break;
    }        


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[rows[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[rows[0]], 
            //"Mitteilung", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Mobiltelefon");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[rows[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[rows[1]], 
            //"Mitteilung", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("antwortet nicht ");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[rows[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[rows[2]], 
            //"geloescht", 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[rows[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[rows[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS923                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS923(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_1;
            row[2]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_2;
            row[3]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_1;
            row[1]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_2;
            row[2]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_3;
            row[3]  = APP_SD_en_S923_ta_note_pb_refresh_impos_4_Zeile_4; 
            break;
    }


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Aktualisierung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
  
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("des");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Telefonbuchs");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);
 
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("fehlgeschlagen");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS924                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS924(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S924_ta_note_voice_active_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S924_ta_note_voice_active_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S924_ta_note_voice_active_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S924_ta_note_voice_active_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_1;
            row[2]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_2;
            row[3]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_1;
            row[1]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_2;
            row[2]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_3;
            row[3]  = APP_SD_en_S924_ta_note_voice_active_4_Zeile_4; 
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Sprach-");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("bedienung");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("aktiv");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS925                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS925(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_1;
            row[2]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_2;
            row[3]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_1;
            row[1]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_2;
            row[2]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_3;
            row[3]  = APP_SD_en_S925_ta_note_pb_not_complete_4_Zeile_4; 
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("unvollstaendig");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("geladen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS927                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS927(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_1;
            row[2]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_2;
            row[3]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_1;
            row[1]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_2;
            row[2]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_3;
            row[3]  = APP_SD_en_S927_ta_err_load_phonebook_4_Zeile_4; 
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Laden des");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Telefonbuchs");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("fehlgeschlagen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS928                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS928(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
      
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_1;
            row[2]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_2;
            row[3]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_1;
            row[1]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_2;
            row[2]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_3;
            row[3]  = APP_SD_en_S928_ta_note_voice_not_ready_4_Zeile_4; 
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Sprach-");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
  
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("bedienung noch");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("nicht bereit");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);
 
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS929                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS929(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    Uint16Type maxCharPerCell           = KPB_u16GetNumChar();
    Uint16Type tmpLen                   = 0;

    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    tmpLen = (pFrameData->s929.number_len > sizeof(pFrameBuffer->cell[1].name.content) ? 
              sizeof(pFrameBuffer->cell[1].name.content) : pFrameData->s929.number_len);             
    
    /* empty */
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0;
    
    // headline
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Rufnummer:");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S929_ta_head_number_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S929_ta_head_number_1_Zeile_1], 
            pFrameBuffer->cell[1].name.length);

    // number 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = tmpLen;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s929.number, 
            pFrameBuffer->cell[2].name.length);
     
    /* empty in any case */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;
    (void) memset( (pFrameBuffer->cell[3].name.content), 
                    0x00,
                    sizeof(pFrameBuffer->cell[3].name.content));



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS930                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS930(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* name of enum migth change, cause its used for phonebook event, not sms */
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_4] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_1;
            row[2]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_2;
            row[3]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_1;
            row[1]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_2;
            row[2]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_3;
            row[3]  = APP_SD_en_S930_ta_note_copy_sms_failed_4_Zeile_4; 
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("wurde");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
 
    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("aktualisiert");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS933                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS933(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_S933_ta_query_connect_2_Zeile_1];
    (void) memcpy(  pFrameBuffer->cell[0].name.content, 
                    gAppSdContext.prompts[APP_SD_en_S933_ta_query_connect_2_Zeile_1],
                    pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameBuffer->cell[1].name.length = _APP_SD_u8FindDynamicTextPosition(pFrameBuffer->cell[1].name.content, gAppSdContext.prompts[APP_SD_en_S933_ta_query_connect_2_Zeile_2], pFrameData->s933.btName, gAppSdContext.prompts_len[APP_SD_en_S933_ta_query_connect_2_Zeile_2]);
 
    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS934                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS934(KpbFrameDataReqType *pFrameBuffer,
                                           const AppSdFrameDataType *pFrameData,
                                           BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_3] )
    {
        numStaticCells++;
    }

    switch( numStaticCells )  
    {
        case 1:
            /* put btName at index 2, static text at index 1, index 0 and 3 are empty  */
            row[0]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_1;
            row[2]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_2;// should be 0
            break;
        case 2:
            /* put btName a index 2, static text at 0 and 1, index 3 is empty */
            row[0]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_1;
            row[1]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_2;
            row[2]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_3;// should be 0
            break;
        default:
            /* put btName at index 3, static text in 0,1,2 */
            row[0]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_1;
            row[1]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_2;
            row[2]  = APP_SD_en_S934_ta_note_bt_acc_conn_3_Zeile_3;
            break;
    }


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Verbunden mit");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);
 
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 0;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Bluetooth-");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);
 
    if( numStaticCells == 1 ||
        numStaticCells == 2 )
    {   /* put btName at index 2 */
        pFrameBuffer->cell[2].isSet               = BT_TRUE;  
        pFrameBuffer->cell[2].pos                 = 2;
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[2].name.length         = pFrameData->s934.btName_len;
        (void) memcpy( (pFrameBuffer->cell[2].name.content), 
                pFrameData->s934.btName, 
                pFrameBuffer->cell[2].name.length);

        /* empty */
        pFrameBuffer->cell[3].isSet               = BT_TRUE;  
        pFrameBuffer->cell[3].pos                 = 3;
        pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[3].name.length         = 0;
    }
    else
    {   /* put btName at index 3 */
        pFrameBuffer->cell[2].isSet               = BT_TRUE;  
        pFrameBuffer->cell[2].pos                 = 2;
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        //pFrameBuffer->cell[2].name.length         = strlen("HeadSet");
        pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
        (void) memcpy( (pFrameBuffer->cell[2].name.content), 
                gAppSdContext.prompts[row[2]], 
                pFrameBuffer->cell[2].name.length);

        pFrameBuffer->cell[3].isSet               = BT_TRUE;  
        pFrameBuffer->cell[3].pos                 = 3;
        pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[3].name.length         = pFrameData->s934.btName_len;
        (void) memcpy( (pFrameBuffer->cell[3].name.content), 
                pFrameData->s934.btName, 
                pFrameBuffer->cell[3].name.length);
    }



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS935                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS935( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
      
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_4] )
    {
        numStaticCells++;
    }
    
    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_1;
            row[2]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_2;
            row[3]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_1;
            row[1]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_2;
            row[2]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_3;
            row[3]  = APP_SD_en_S935_ta_bt_no_conn_headset_4_Zeile_4;
            break;
    }


    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("BT-Verbindung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("nicht moeglich.");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("HeadSet");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("verbunden");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS941                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS941( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    /* empty */
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0;

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Anruf");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S941_ta_head_call_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S941_ta_head_call_1_Zeile_1], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    // Ticket 1552
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s941.name, pFrameData->s941.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s941.name_len;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s941.name, 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;

    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS942                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS942( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* empty */
    pFrameBuffer->cell[0].isSet               = BT_TRUE;
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0;

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Aktiver Anruf");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S942_ta_head_call_active_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S942_ta_head_call_active_1_Zeile_1], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    // Ticket #1552
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s942.name, pFrameData->s942.name_len))
    {
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s942.name_len;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s942.name, 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;

    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameS945                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameS945( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    /* empty */
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = 0;

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Rufaufbau");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_S945_ta_head_call_setup_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_S945_ta_head_call_setup_1_Zeile_1], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    // Ticket #1552
    //pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->s945.name, pFrameData->s945.name_len))
    {
        pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = pFrameData->s945.name_len;
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            pFrameData->s945.name, 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;

    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC01                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC01( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Auswahl");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SC01_ta_head_options_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SC01_ta_head_options_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC03                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC03( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_2;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Media Player");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SC03_ta_head_media_player_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SC03_ta_head_media_player_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC04                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC04( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_4] )
    {
        numStaticCells++;
    }
    
    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_1;
            row[2]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_2;
            row[3]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_1;
            row[1]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_2;
            row[2]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_3;
            row[3]  = APP_SD_en_SC04_ta_note_delete_impos_phone_active_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Loeschen nicht");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("moeglich, von");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Benutzer als");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("Tel. benutzt");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC07                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC07( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
 
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_4] )
    {
        numStaticCells++;
    }
    
    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_1;
            row[2]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_2;
            row[3]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_1;
            row[1]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_2;
            row[2]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_3;
            row[3]  = APP_SD_en_SC07_ta_no_media_player_found_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Keinen");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("kompatiblen");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Media Player");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);
 
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("gefunden");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC12                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC12( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[2];
    pFrameBuffer->frameId       = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation = KPB_MENU_CHANGE;

    // we have a list, and we add three cells for semi static info
    // prio for static cells is 0,1,2
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    
    // first cell is semis static text ( player name )  
    // so the static text need to start at cell id 1 
    row[0] = APP_SD_en_SC12_ta_note_connecting_player_3_Zeile_1;
    row[1] = APP_SD_en_SC12_ta_note_connecting_player_3_Zeile_2;

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = pFrameData->sc12.btName_len;
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            pFrameData->sc12.btName, 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Player wird");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Player wird", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("verbunden.");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[1]],
            //"verbunden.", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC13                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC13( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numCells                  = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;

    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC13_ta_note_searching_players_3_Zeile_1] )
    {
       numCells++; 
    } 
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC13_ta_note_searching_players_3_Zeile_2] )
    {
       numCells++; 
    } 
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC13_ta_note_searching_players_3_Zeile_3] )
    {
       numCells++; 
    } 

    switch(numCells)
    {
        case 1:
            row[0] = APP_SD_en_SC13_ta_note_searching_players_3_Zeile_3;
            row[1] = APP_SD_en_SC13_ta_note_searching_players_3_Zeile_1;
            row[2] = APP_SD_en_SC13_ta_note_searching_players_3_Zeile_2;
            break;
        default:
            row[0] = APP_SD_en_SC13_ta_note_searching_players_3_Zeile_1;
            row[1] = APP_SD_en_SC13_ta_note_searching_players_3_Zeile_2;
            row[2] = APP_SD_en_SC13_ta_note_searching_players_3_Zeile_3;
            break;
    }
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Player werden"); 
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]],
            //"Player werden", 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("gesucht."); 
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]]; 
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]],
            //"gesucht.", 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Bitte warten!"); 
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]]; 
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]],
            //"Bitte warten!", 
            pFrameBuffer->cell[2].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC14                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC14( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_3] )
    {
        numStaticCells++;
    }
     
    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_3;// should be 0
            row[1]  = APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_1;
            row[2]  = APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_1;
            row[1]  = APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_2;
            row[2]  = APP_SD_en_SC14_ta_note_reading_sms_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Mitteilung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("wird");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("vorgelesen");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC16                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC16( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anrufername");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SC16_ta_note_read_voice_tag_2_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SC16_ta_note_read_voice_tag_2_Zeile_1], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("ansagen:");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_SC16_ta_note_read_voice_tag_2_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_SC16_ta_note_read_voice_tag_2_Zeile_2], 
            pFrameBuffer->cell[1].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC17                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SD_FB_sfSendFrameSC17( KpbFrameDataReqType *pFrameBuffer,
//                                            const AppSdFrameDataType *pFrameData,
//                                            BooleanType          activate)
//{
//    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
//    Uint8Type numStaticCells            = 0;
//    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
//    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
//    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
//     
//    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_1] )
//    {
//        numStaticCells++;
//    }
//    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_2] )
//    {
//        numStaticCells++;
//    }
//    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_3] )
//    {
//        numStaticCells++;
//    }
//     
//    switch( numStaticCells )  
//    {
//        case 1:
//        case 2:
//            row[0]  = APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_3;// should be 0
//            row[1]  = APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_1;
//            row[2]  = APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_2;
//            break;
//        default:
//            row[0]  = APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_1;
//            row[1]  = APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_2;
//            row[2]  = APP_SD_en_SC17_ta_note_new_phone_found_3_Zeile_3;
//            break;
//    }
//
//    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[0].pos                 = 0;
//    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[0].name.length         = strlen("Neues");
//    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
//    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
//            gAppSdContext.prompts[row[0]], 
//            pFrameBuffer->cell[0].name.length);
//
//    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[1].pos                 = 1;
//    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[1].name.length         = strlen("Mobiltelefon");
//    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
//    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
//            gAppSdContext.prompts[row[1]], 
//            pFrameBuffer->cell[1].name.length);
//
//    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[2].pos                 = 2;
//    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    //pFrameBuffer->cell[2].name.length         = strlen("erkannt");
//    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
//    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
//            gAppSdContext.prompts[row[2]], 
//            pFrameBuffer->cell[2].name.length);
//
//    /* empty */
//    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
//    pFrameBuffer->cell[3].pos                 = 3;
//    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
//    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
//    pFrameBuffer->cell[3].name.length         = 0;
//
//
//
//    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC20                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC20( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_4;
    pFrameBuffer->listOperation         = KPB_NO_LIST_OPERATION;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //TODO:
    //pFrameBuffer->cell[0].name.length         = strlen("Privatgespräch");  
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SC20_ta_head_private_call_1_Zeile_1];
    //pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_enError];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SC20_ta_head_private_call_1_Zeile_1],
            //gAppSdContext.prompts[APP_SD_enError],
            //"Privatgespräch", 
            pFrameBuffer->cell[0].name.length);

    // TODO: ggf. check if name / nr. is available, else show empty cell
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    // Ticket #1552
   	//pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    // #2255: if we can detect a phone number, then change cell type
    // again set any string as default, only if a valid number is detected we choose phonenumber as cell type
    // else a too long name would be shortened the wrong way ( left side )
   	pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(pFrameData->sc20.name, pFrameData->sc20.name_len))
    {
        pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_PHONE_NUMBER;
    }
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = pFrameData->sc20.name_len;
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            pFrameData->sc20.name, 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC22                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC22( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[3];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
       
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_3] )
    {
        numStaticCells++;
    }
     
    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_3;// should be 0
            row[1]  = APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_1;
            row[2]  = APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_2;
            break;
        default:
            row[0]  = APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_1;
            row[1]  = APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_2;
            row[2]  = APP_SD_en_SC22_ta_note_new_pb_available_3_Zeile_3;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Neues");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]]; 
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("Telefonbuch");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("verfuegbar");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    /* empty */
    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC23                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC23( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    APP_SD_ENUM_TEXT_PROMPTS_SEC_DISPLAY row[4];
    Uint8Type numStaticCells            = 0;
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_1] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_2] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_3] )
    {
        numStaticCells++;
    }
    if( 0 < gAppSdContext.prompts_len[APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_4] )
    {
        numStaticCells++;
    }
     
    switch( numStaticCells )  
    {
        case 1:
        case 2:
            row[0]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_3;// should be 0
            row[1]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_1;
            row[2]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_2;
            row[3]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_4;// should be 0
            break;
        default:
            row[0]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_1;
            row[1]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_2;
            row[2]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_3;
            row[3]  = APP_SD_en_SC23_ta_note_bt_conn_impos_4_Zeile_4;
            break;
    }

    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("BT-Verbindung");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[row[0]];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[row[0]], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[1].name.length         = strlen("nicht moeglich.");
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[row[1]];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[row[1]], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[2].name.length         = strlen("Mobiltelefon");
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[row[2]];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[row[2]], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[3].name.length         = strlen("verbunden");
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[row[3]];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[row[3]], 
            pFrameBuffer->cell[3].name.length);



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSC24                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSC24( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_3;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    //pFrameBuffer->cell[0].name.length         = strlen("Anrufe");
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SC24_ta_head_calls_1_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SC24_ta_head_calls_1_Zeile_1], 
            pFrameBuffer->cell[0].name.length);

    /* empty */
    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = 0;



    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSA01                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSA01( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA01_ta_note_info_call_4_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA01_ta_note_info_call_4_Zeile_1], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA01_ta_note_info_call_4_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA01_ta_note_info_call_4_Zeile_2], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA01_ta_note_info_call_4_Zeile_3];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA01_ta_note_info_call_4_Zeile_3], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA01_ta_note_info_call_4_Zeile_4];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA01_ta_note_info_call_4_Zeile_4], 
            pFrameBuffer->cell[3].name.length);



    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSA02                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSA02( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA02_ta_note_service_call_4_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA02_ta_note_service_call_4_Zeile_1], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA02_ta_note_service_call_4_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA02_ta_note_service_call_4_Zeile_2], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA02_ta_note_service_call_4_Zeile_3];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA02_ta_note_service_call_4_Zeile_3], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA02_ta_note_service_call_4_Zeile_4];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA02_ta_note_service_call_4_Zeile_4], 
            pFrameBuffer->cell[3].name.length);



    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSA03                                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSA03( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    pFrameBuffer->frameId               = KPB_FRAME_ID_1;
    pFrameBuffer->listOperation         = KPB_MENU_CHANGE;
    //pFrameBuffer->arrayHeader.mode.data = KPB_ARRAY_MODE_3_COMBINED;
    
    pFrameBuffer->cell[0].isSet               = BT_TRUE;  
    pFrameBuffer->cell[0].pos                 = 0;
    pFrameBuffer->cell[0].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[0].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[0].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_1];
    (void) memcpy( (pFrameBuffer->cell[0].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_1], 
            pFrameBuffer->cell[0].name.length);

    pFrameBuffer->cell[1].isSet               = BT_TRUE;  
    pFrameBuffer->cell[1].pos                 = 1;
    pFrameBuffer->cell[1].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[1].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[1].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_2];
    (void) memcpy( (pFrameBuffer->cell[1].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_2], 
            pFrameBuffer->cell[1].name.length);

    pFrameBuffer->cell[2].isSet               = BT_TRUE;  
    pFrameBuffer->cell[2].pos                 = 2;
    pFrameBuffer->cell[2].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[2].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[2].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_3];
    (void) memcpy( (pFrameBuffer->cell[2].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_3], 
            pFrameBuffer->cell[2].name.length);

    pFrameBuffer->cell[3].isSet               = BT_TRUE;  
    pFrameBuffer->cell[3].pos                 = 3;
    pFrameBuffer->cell[3].type                = KPB_CELL_TYPE_ANY_STRING;
    pFrameBuffer->cell[3].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
    pFrameBuffer->cell[3].name.length         = gAppSdContext.prompts_len[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_4];
    (void) memcpy( (pFrameBuffer->cell[3].name.content), 
            gAppSdContext.prompts[APP_SD_en_SA03_ta_note_sos_call_4_Zeile_4], 
            pFrameBuffer->cell[3].name.length);



    if(BT_TRUE == _APP_SD_bIsKl15On())
    {
        return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
    }
    else
    {
        return KPB_sfSendFrameData(pFrameBuffer, BT_TRUE, KPB_RELATIVE_PRIO_HIGH);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_sfSendFrameSB34                                   */
/** 
    \brief      completely build up the whole frame, then send and activate 
                frame via KPB module
               
    \param      pFrameBuffer
                KpbFrameDataReqType *
                pointer to send buffer

    \param      pFrameData          
                AppSdFrameDataType *
                pointer to frame data which shall be send

    \param      activate
                BooleanType
                whether to activate the frame or not

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SD_FB_sfSendFrameSB34( KpbFrameDataReqType *pFrameBuffer,
                                            const AppSdFrameDataType *pFrameData,
                                            BooleanType          activate)
{
    const Uint8Type *pVersionData  = pFrameData->sb34.version;
    Uint16Type versionLen          = pFrameData->sb34.version_len; 
    Uint16Type copiedBytes = 0;
    Uint8Type i            = 0; 

    pFrameBuffer->frameId       = KPB_FRAME_ID_5;
    pFrameBuffer->listOperation = KPB_ENTER_MENUE;

    // we add two cells with static text
    //pFrameBuffer->arrayHeader.mode.data   = KPB_ARRAY_MODE_3_COMBINED;
    
    // take text from pFrameData and fill up the cells 0-6 with it.

    for(i=0; i<7; i++)
    {
        Uint16Type bytesRemain = versionLen - copiedBytes;
        Uint16Type bytes = ( bytesRemain > KPB_MAX_CELL_CONTENT_SIZE ) ?
                            KPB_MAX_CELL_CONTENT_SIZE : bytesRemain;

        pFrameBuffer->cell[i].isSet               = BT_TRUE;  
        pFrameBuffer->cell[i].pos                 = i;
        pFrameBuffer->cell[i].type                = KPB_CELL_TYPE_ANY_STRING;
        pFrameBuffer->cell[i].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
        pFrameBuffer->cell[i].name.length         = bytes;

        // RT #4657: Helptext is partly visible in Version information
        if( bytes > 0 )
        {
            (void)memcpy( (pFrameBuffer->cell[i].name.content), 
                        pVersionData + copiedBytes, 
                        pFrameBuffer->cell[i].name.length);

            copiedBytes += bytes;
        } 
        pFrameBuffer->arrayHeader.elements++; 
    }

    return KPB_sfSendFrameData(pFrameBuffer, activate, KPB_RELATIVE_PRIO_LOW);
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_vSetCell                                                   */
/** 
    \brief      set parameters of a single cell
               
    \param      cell
                KpbFrameCellType *
                pointer to cell buffer 

    \param      pos
                Uint8Type
                index of cell within the frame
    
    \param      type
                KPB_enCellEntryType
                type of cell

    \param      attr
                Uint8Type
                attributes of cell ( e.g. visible etc. )

    \param      content
                const Uint8Type *
                content of the cell

    \param      length
                Uint8Type
                length of cell content

    \param      offset
                Uint8Type
                offset within the cell where content shall be put into. This is
                e.g. needed for phone idle frame where several icons shall be 
                displayed next to each other within the same cell
*/
/*----------------------------------------------------------------------------*/
void _APP_SD_FB_vSetCell(  KpbFrameCellType *cell,   Uint8Type pos,
                           KPB_enCellEntryType type, Uint8Type attr,
                           const Uint8Type *content, Uint8Type length,
                           Uint8Type offset )
{
    if( offset > KPB_MAX_CELL_CONTENT_SIZE )
    {
        offset = 0;
    }    

    if((length + offset) > KPB_MAX_CELL_CONTENT_SIZE)
    {
        DEBUG_VALUE1( APP_SD_SWI_TRC, MOD_APP_SD, "vSetCell: invalid content length: %d", 
                      length);

        length = KPB_MAX_CELL_CONTENT_SIZE - offset;
    }
    
    cell->isSet             = BT_TRUE; 
    cell->pos               = pos;
    cell->type              = type;
    cell->attribute.rawByte = attr;
    cell->name.length       = length + offset;

    (void)memcpy((cell->name.content) + offset, content, length);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SD_FB_vPresReqOff                                       */
/** 
    \brief      call the function in KPB to release the display for HMI menu
               
*/
/*----------------------------------------------------------------------------*/
void _APP_SD_FB_vPresReqOff( Uint32Type line )
{
    DEBUG_VALUE1( APP_SD_SWI_TRC, MOD_APP_SD, "vPresReqOff: line %d", line);
//    gAppSdContext.ctxInView = APP_SD_UHV_NOT_IN_ASG_VIEW;
    (void)_APP_SD_vSetCtxHistoryState(APP_SD_UHV_NOT_IN_ASG_VIEW, __LINE__);
    KPB_vSendFrameStatusPresReqOff();
}

/* End Of File APP_SD_FrameBuilder_C1.c */


