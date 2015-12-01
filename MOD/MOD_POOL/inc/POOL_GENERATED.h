/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_GENERATED.h
 *-----------------------------------------------------------------------------
 * Module Name: Pool handling
 *-----------------------------------------------------------------------------
 * Description: Contains the generated data areas for the pool handling
 *-----------------------------------------------------------------------------
 * $Date: 2011-11-10 12:34:21 +0100 (Do, 10 Nov 2011) $
 * $Rev: 23262 $
 * $Author: nikolov $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/inc/POOL_GENERATED.h $
 *-----------------------------------------------------------------------------
*/

/* Control Structure for Pools */
typedef struct POOL_stGenControl {
   Uint32Type noOfElements;
   Uint32Type elementSize;
   Uint8Type *addr;
} POOL_stGenControl;

/* Enumerations for Pool Names */
typedef enum {
   POOL_enATRspAllType,  /* Pool name */
   POOL_enATRspCallStatValType,  /* Pool name */
   POOL_enAppSdEventQueueType,  /* Pool name */
   POOL_enBapArrayHeader,  /* Pool name */
   POOL_enBapDispAckQueueElem,  /* Pool name */
   POOL_enBapDispAckQueuePayloadType,  /* Pool name */
   POOL_enBapIndDataTypeAppMdi,  /* Pool name */
   POOL_enBapIndDataTypeAppPB,  /* Pool name */
   POOL_enBapIndDataTypeAppPBLarge,  /* Pool name */
   POOL_enBapIndDataTypeAppPhone,  /* Pool name */
   POOL_enBapIndDataTypeKpb,  /* Pool name */
   POOL_enBapReqDataType,  /* Pool name */
   POOL_enVCRPerfMeasCmdType,  /* Pool name */
   POOL_enLAST    /* Last entry,must only be used internally in pool handling.*/
} POOL_enPoolNames;

/* Enumerations for Pools Elements */
typedef enum {
   POOL_enATRspAllTypeElements  = 2, /** Num of elem for pool ATRspAllType*/
   POOL_enATRspCallStatValTypeElements  = 2, /** Num of elem for pool ATRspCallStatValType*/
   POOL_enAppSdEventQueueTypeElements  = 5, /** Num of elem for pool AppSdEventQueueType*/
   POOL_enBapArrayHeaderElements  = 3, /** Num of elem for pool BapArrayHeader*/
   POOL_enBapDispAckQueueElemElements  = 4, /** Num of elem for pool BapDispAckQueueElem*/
   POOL_enBapDispAckQueuePayloadTypeElements  = 4, /** Num of elem for pool BapDispAckQueuePayloadType*/
   POOL_enBapIndDataTypeAppMdiElements  = 5, /** Num of elem for pool BapIndDataTypeAppMdi*/
   POOL_enBapIndDataTypeAppPBElements  = 5, /** Num of elem for pool BapIndDataTypeAppPB*/
   POOL_enBapIndDataTypeAppPBLargeElements  = 1, /** Num of elem for pool BapIndDataTypeAppPBLarge*/
   POOL_enBapIndDataTypeAppPhoneElements  = 5, /** Num of elem for pool BapIndDataTypeAppPhone*/
   POOL_enBapIndDataTypeKpbElements  = 5, /** Num of elem for pool BapIndDataTypeKpb*/
   POOL_enBapReqDataTypeElements  = 1, /** Num of elem for pool BapReqDataType*/
   POOL_enVCRPerfMeasCmdTypeElements  = 3, /** Num of elem for pool VCRPerfMeasCmdType*/
   POOL_enLASTSIZE    /** Last entry.*/
} POOL_enPoolElements;


/* Hash defines for Pools Element Size */
#  include "ATParser.h"
#define  POOL_ATRspAllTypeSize   sizeof(ATRspAllType) // sizeof pool element
#  include "ATParser.h"
#define  POOL_ATRspCallStatValTypeSize   sizeof(ATRspCallStatValType) // sizeof pool element
#  include "APP_SD_CI.h"
#define  POOL_AppSdEventQueueTypeSize   sizeof(AppSdEventQueueType) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapArrayHeaderSize   sizeof(BapArrayHeaderDataType) // sizeof pool element
#  include "BAP_Dispatcher_Ack.h"
#define  POOL_BapDispAckQueueElemSize   sizeof(BAP_DISP_AckQueueElem) // sizeof pool element
#  include "BAP_Dispatcher_Ack.h"
#define  POOL_BapDispAckQueuePayloadTypeSize   sizeof(BAP_DISP_AckQueuePayloadType) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapIndDataTypeAppMdiSize   sizeof(BapIndDataTypeAppMdi) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapIndDataTypeAppPBSize   sizeof(BapIndDataTypeAppPB) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapIndDataTypeAppPBLargeSize   sizeof(BapIndDataTypeAppPBLarge) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapIndDataTypeAppPhoneSize   sizeof(BapIndDataTypeAppPhone) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapIndDataTypeKpbSize   sizeof(BapIndDataTypeKpb) // sizeof pool element
#  include "APP_COMMON_Pool.h"
#define  POOL_BapReqDataTypeSize   sizeof(BapReqDataType) // sizeof pool element
#  include "VCR_ASR.h"
#define  POOL_VCRPerfMeasCmdTypeSize   sizeof(VCRPerfMeasCmdValueType) // sizeof pool element


/* Structure for Pool */
typedef struct POOL_stATRspAllTypeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_ATRspAllTypeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stATRspAllTypeElement;

/* Structure for Pool */
typedef struct POOL_stATRspCallStatValTypeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_ATRspCallStatValTypeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stATRspCallStatValTypeElement;

/* Structure for Pool */
typedef struct POOL_stAppSdEventQueueTypeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_AppSdEventQueueTypeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stAppSdEventQueueTypeElement;

/* Structure for Pool */
typedef struct POOL_stBapArrayHeaderElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapArrayHeaderSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapArrayHeaderElement;

/* Structure for Pool */
typedef struct POOL_stBapDispAckQueueElemElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapDispAckQueueElemSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapDispAckQueueElemElement;

/* Structure for Pool */
typedef struct POOL_stBapDispAckQueuePayloadTypeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapDispAckQueuePayloadTypeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapDispAckQueuePayloadTypeElement;

/* Structure for Pool */
typedef struct POOL_stBapIndDataTypeAppMdiElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapIndDataTypeAppMdiSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapIndDataTypeAppMdiElement;

/* Structure for Pool */
typedef struct POOL_stBapIndDataTypeAppPBElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapIndDataTypeAppPBSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapIndDataTypeAppPBElement;

/* Structure for Pool */
typedef struct POOL_stBapIndDataTypeAppPBLargeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapIndDataTypeAppPBLargeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapIndDataTypeAppPBLargeElement;

/* Structure for Pool */
typedef struct POOL_stBapIndDataTypeAppPhoneElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapIndDataTypeAppPhoneSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapIndDataTypeAppPhoneElement;

/* Structure for Pool */
typedef struct POOL_stBapIndDataTypeKpbElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapIndDataTypeKpbSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapIndDataTypeKpbElement;

/* Structure for Pool */
typedef struct POOL_stBapReqDataTypeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_BapReqDataTypeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stBapReqDataTypeElement;

/* Structure for Pool */
typedef struct POOL_stVCRPerfMeasCmdTypeElement {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_VCRPerfMeasCmdTypeSize];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_stVCRPerfMeasCmdTypeElement;


/* Extern Definition for Pools */
extern POOL_stATRspAllTypeElement  POOL_stATRspAllType[POOL_enATRspAllTypeElements];
extern POOL_stATRspCallStatValTypeElement  POOL_stATRspCallStatValType[POOL_enATRspCallStatValTypeElements];
extern POOL_stAppSdEventQueueTypeElement  POOL_stAppSdEventQueueType[POOL_enAppSdEventQueueTypeElements];
extern POOL_stBapArrayHeaderElement  POOL_stBapArrayHeader[POOL_enBapArrayHeaderElements];
extern POOL_stBapDispAckQueueElemElement  POOL_stBapDispAckQueueElem[POOL_enBapDispAckQueueElemElements];
extern POOL_stBapDispAckQueuePayloadTypeElement  POOL_stBapDispAckQueuePayloadType[POOL_enBapDispAckQueuePayloadTypeElements];
extern POOL_stBapIndDataTypeAppMdiElement  POOL_stBapIndDataTypeAppMdi[POOL_enBapIndDataTypeAppMdiElements];
extern POOL_stBapIndDataTypeAppPBElement  POOL_stBapIndDataTypeAppPB[POOL_enBapIndDataTypeAppPBElements];
extern POOL_stBapIndDataTypeAppPBLargeElement  POOL_stBapIndDataTypeAppPBLarge[POOL_enBapIndDataTypeAppPBLargeElements];
extern POOL_stBapIndDataTypeAppPhoneElement  POOL_stBapIndDataTypeAppPhone[POOL_enBapIndDataTypeAppPhoneElements];
extern POOL_stBapIndDataTypeKpbElement  POOL_stBapIndDataTypeKpb[POOL_enBapIndDataTypeKpbElements];
extern POOL_stBapReqDataTypeElement  POOL_stBapReqDataType[POOL_enBapReqDataTypeElements];
extern POOL_stVCRPerfMeasCmdTypeElement  POOL_stVCRPerfMeasCmdType[POOL_enVCRPerfMeasCmdTypeElements];

extern POOL_stGenControl poolGenControl[POOL_enLAST];
// =================================== end of POOL_GENERATED.h ===
