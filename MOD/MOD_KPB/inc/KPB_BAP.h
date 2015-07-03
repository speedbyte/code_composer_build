/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   KPB_BAP.h
*-----------------------------------------------------------------------------
* Module Name:  KPB    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for KPB
*-----------------------------------------------------------------------------
* $Date: 2010-03-06 18:44:18 +0100 (Sa, 06 Mrz 2010) $
* $Rev: 11440 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/inc/KPB_BAP.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _KPB_BAP_H
#define _KPB_BAP_H

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
#include "KPB_CE.h"
#include "KPB.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

#define KPB_FRAME_DATA_REC_ADDR_NAME 1<<0
#define KPB_FRAME_DATA_REC_ADDR_TYPE 1<<1
#define KPB_FRAME_DATA_REC_ADDR_ATTR 1<<2


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendAsgCapabilities(const KPB_enDisplaySizeClass dispClass,
                                  const Uint8Type visibleElements);
void _KPB_vBapSendAsgConfig(const KPB_enVersionNumber versionNumber);
void _KPB_vBapSendOperationState(const KPB_enOpState opState);
void _KPB_vBapSendLsgStatus(const BooleanType alive);
void _KPB_vBapSendFrameStatus(const KpbFrameStatusType *pFrameStatus);
void _KPB_vBapSendScrollBar(const KpbScrollBarReqType *pScrollBar);
void _KPB_vBapSendMflBlockDefinition(const KPB_KeyBlockType keyBlock);
SuccessFailType _KPB_sfBapSendFrameData(const KpbFrameDataReqType *pFrameData);
SuccessFailType _KPB_sfParseBapIndication(  const BapIndication *pBapInd, 
                                            KpbIndType *pKpbInd);
SuccessFailType _KPB_sfBapSendFrameDataEmpty( const KPB_enFrameIdType frameId, 
                                              const BapArrayHeaderType *pGetHeader);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/



#endif /* _KPB_BAP_H */

/* End Of File KPB_BAP.h */

