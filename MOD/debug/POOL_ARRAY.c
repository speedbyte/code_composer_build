/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_ARRAY.c
 *-----------------------------------------------------------------------------
 * Module Name: Pool handling
 *-----------------------------------------------------------------------------
 * Description: Contains the generated data areas for the pool handling
 *-----------------------------------------------------------------------------
 * $Date: 2011-11-10 12:34:21 +0100 (Do, 10 Nov 2011) $
 * $Rev: 23262 $
 * $Author: nikolov $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/src/POOL_ARRAY.c $
 *-----------------------------------------------------------------------------
*/
#include "POOL.h"

POOL_stATRspAllTypeElement POOL_stATRspAllType[POOL_enATRspAllTypeElements];
POOL_stATRspCallStatValTypeElement POOL_stATRspCallStatValType[POOL_enATRspCallStatValTypeElements];
POOL_stAppSdEventQueueTypeElement POOL_stAppSdEventQueueType[POOL_enAppSdEventQueueTypeElements];
POOL_stBapArrayHeaderElement POOL_stBapArrayHeader[POOL_enBapArrayHeaderElements];
POOL_stBapDispAckQueueElemElement POOL_stBapDispAckQueueElem[POOL_enBapDispAckQueueElemElements];
POOL_stBapDispAckQueuePayloadTypeElement POOL_stBapDispAckQueuePayloadType[POOL_enBapDispAckQueuePayloadTypeElements];
POOL_stBapIndDataTypeAppMdiElement POOL_stBapIndDataTypeAppMdi[POOL_enBapIndDataTypeAppMdiElements];
POOL_stBapIndDataTypeAppPBElement POOL_stBapIndDataTypeAppPB[POOL_enBapIndDataTypeAppPBElements];
POOL_stBapIndDataTypeAppPBLargeElement POOL_stBapIndDataTypeAppPBLarge[POOL_enBapIndDataTypeAppPBLargeElements];
POOL_stBapIndDataTypeAppPhoneElement POOL_stBapIndDataTypeAppPhone[POOL_enBapIndDataTypeAppPhoneElements];
POOL_stBapIndDataTypeKpbElement POOL_stBapIndDataTypeKpb[POOL_enBapIndDataTypeKpbElements];
POOL_stBapReqDataTypeElement POOL_stBapReqDataType[POOL_enBapReqDataTypeElements];
POOL_stVCRPerfMeasCmdTypeElement POOL_stVCRPerfMeasCmdType[POOL_enVCRPerfMeasCmdTypeElements];

POOL_stGenControl poolGenControl[POOL_enLAST] = {
{POOL_enATRspAllTypeElements,POOL_ATRspAllTypeSize,(Uint8Type*)POOL_stATRspAllType},
{POOL_enATRspCallStatValTypeElements,POOL_ATRspCallStatValTypeSize,(Uint8Type*)POOL_stATRspCallStatValType},
{POOL_enAppSdEventQueueTypeElements,POOL_AppSdEventQueueTypeSize,(Uint8Type*)POOL_stAppSdEventQueueType},
{POOL_enBapArrayHeaderElements,POOL_BapArrayHeaderSize,(Uint8Type*)POOL_stBapArrayHeader},
{POOL_enBapDispAckQueueElemElements,POOL_BapDispAckQueueElemSize,(Uint8Type*)POOL_stBapDispAckQueueElem},
{POOL_enBapDispAckQueuePayloadTypeElements,POOL_BapDispAckQueuePayloadTypeSize,(Uint8Type*)POOL_stBapDispAckQueuePayloadType},
{POOL_enBapIndDataTypeAppMdiElements,POOL_BapIndDataTypeAppMdiSize,(Uint8Type*)POOL_stBapIndDataTypeAppMdi},
{POOL_enBapIndDataTypeAppPBElements,POOL_BapIndDataTypeAppPBSize,(Uint8Type*)POOL_stBapIndDataTypeAppPB},
{POOL_enBapIndDataTypeAppPBLargeElements,POOL_BapIndDataTypeAppPBLargeSize,(Uint8Type*)POOL_stBapIndDataTypeAppPBLarge},
{POOL_enBapIndDataTypeAppPhoneElements,POOL_BapIndDataTypeAppPhoneSize,(Uint8Type*)POOL_stBapIndDataTypeAppPhone},
{POOL_enBapIndDataTypeKpbElements,POOL_BapIndDataTypeKpbSize,(Uint8Type*)POOL_stBapIndDataTypeKpb},
{POOL_enBapReqDataTypeElements,POOL_BapReqDataTypeSize,(Uint8Type*)POOL_stBapReqDataType},
{POOL_enVCRPerfMeasCmdTypeElements,POOL_VCRPerfMeasCmdTypeSize,(Uint8Type*)POOL_stVCRPerfMeasCmdType},
};
// ======================================= end of POOL_ARRAY.c ===
