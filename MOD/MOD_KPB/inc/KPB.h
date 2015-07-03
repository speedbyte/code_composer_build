/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   KPB.h
*-----------------------------------------------------------------------------
* Module Name:  KPB    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for KPB
*-----------------------------------------------------------------------------
* $Date: 2012-02-09 09:48:25 +0100 (Do, 09 Feb 2012) $
* $Rev: 25642 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/inc/KPB.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _KPB_H
#define _KPB_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_BAP_ARRAY.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "LOGI_BUT_MAN.h"
#include "APP_CSHDL.h"

#include "KPB_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#define KPB_MAX_FRAME_CELL_NUMBER   12
#define KPB_MAX_CELLS_PER_ARRAY     6
#define KPB_MAX_KEY_BLOCK_NUMBER    8
#define KPB_MAX_CELL_CONTENT_SIZE   93  // number of bytes

// SUSANNE: 1433
// KPB Frame Catalogue S. 8: Independent from the display size class telephone numbers may 
// contain up to 40 characters 
// Ck5050 only accepts numbers up to 32 characters
#define MAX_CHARACTERS_PER_CELL_FOR_PHONE_NUMBERS 32 // 40

/* constant defines for the FrameData array mode */
#define KPB_ARRAY_MODE_NAME_ONLY        0x01U
#define KPB_ARRAY_MODE_TYPE_ONLY        0x02U
#define KPB_ARRAY_MODE_ATTRIBUTES_ONLY  0x04U
#define KPB_ARRAY_MODE_BACKWARD_ONLY    0x10U
#define KPB_ARRAY_MODE_POSITION_ONLY    0x40U

//#define KPB_ARRAY_MODE_NAME        (KPB_ARRAY_MODE_NAME_ONLY|KPB_ARRAY_MODE_POSITION_ONLY)
//#define KPB_ARRAY_MODE_TYPE        (KPB_ARRAY_MODE_TYPE_ONLY|KPB_ARRAY_MODE_POSITION_ONLY)
//#define KPB_ARRAY_MODE_ATTRIBUTES  (KPB_ARRAY_MODE_ATTRIBUTES_ONLY|KPB_ARRAY_MODE_POSITION_ONLY)
//
//#define KPB_ARRAY_MODE_3_COMBINED  (KPB_ARRAY_MODE_NAME|KPB_ARRAY_MODE_TYPE|KPB_ARRAY_MODE_ATTRIBUTES)

/* constant defines for KpbCellAttributeType */
#define KPB_CELL_ATTRIBUTE_VISIBLE      0x80U   // item is visible
#define KPB_CELL_ATTRIBUTE_ACTIVATED    0x08U   // check box or radio button selected
#define KPB_CELL_ATTRIBUTE_SELECTED     0x04U   // button ok was pressed when entry was focussed
#define KPB_CELL_ATTRIBUTE_FOCUSED      0x02U   // item is "ausgewählt"
#define KPB_CELL_ATTRIBUTE_SELECTABLE   0x01U   // item is selectable


#define KPB_SB_ATTRIBUTE_SHOW_ARROWS    0x01U
#define KPB_SB_ATTRIBUTE_ENDLESS_LIST   0x02U
#define KPB_SB_ATTRIBUTE_IS_POPUP       0x04U


#define KPB_SB_FIRST_LIST_CELL_FRAME_2  1
#define KPB_SB_FIRST_LIST_CELL_FRAME_3  2
#define KPB_SB_FIRST_LIST_CELL_FRAME_4  3
#define KPB_SB_FIRST_LIST_CELL_FRAME_6  4

#define KPB_SB_LAST_LIST_CELL_FRAME_2   11
#define KPB_SB_LAST_LIST_CELL_FRAME_3   7
#define KPB_SB_LAST_LIST_CELL_FRAME_4   5
#define KPB_SB_LAST_LIST_CELL_FRAME_6   7

//#define KPB_SB_NUM_VISIBLE_LIST_CELLS_FRAME_2 set via ASG Capabilities numVisibleElements
#define KPB_SB_NUM_VISIBLE_LIST_CELLS_FRAME_3   2
#define KPB_SB_NUM_VISIBLE_LIST_CELLS_FRAME_4   1
#define KPB_SB_NUM_VISIBLE_LIST_CELLS_FRAME_6   1

#define KPB_NUM_CHAR_DISP_CLASS_SMALL   21
#define KPB_NUM_CHAR_DISP_CLASS_MEDIUM  31
          

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef KPB_enKeyBlock  KPB_KeyBlockType[KPB_MAX_KEY_BLOCK_NUMBER];

typedef enum
{
/* 0  */    KPB_NOTIFY_ASG_AVAILABLE,
/* 1  */    KPB_NOTIFY_VISIBLE_ON,
/* 2  */    KPB_NOTIFY_VISIBLE_OFF,
/* 3  */    KPB_NOTIFY_ASG_INIT_DONE,
/* 4  */    KPB_NOTIFY_ASG_INIT_DONE_ACTIVE,
/* 5  */    KPB_NOTIFY_VISIBLE_ON_BY_ASG,
/* 6  */    KPB_NOTIFY_BUSOFF_END

}KPB_enNotificationId;


typedef struct
{
    KPB_enFunctionIdType    fctType;
    KPB_enOpType            opType;
} KpbCommonType;

typedef struct
{
    KpbCommonType   common;
    KPB_enOpState   state;
} KpbOpStateType;

typedef struct
{
    KpbCommonType           common;
    KPB_enDisplaySizeClass  dispSize;
    Uint8Type               visibleElements;
} KpbAsgCapType;

typedef struct
{
    KpbCommonType           common;
    KPB_enVersionNumber     version;
} KpbAsgConfType;


typedef struct
{
    KpbCommonType           common;
    KPB_KeyBlockType        keyBlock;
} KpbMflBlockDefType;

typedef struct
{
    KpbCommonType      common;
    KPB_enLsgAlive     alive;
} KpbLsgStatusType;

typedef union
{
    struct
    {
        Uint8Type   kb7:1;
        Uint8Type   kb6:1;
        Uint8Type   kb5:1;
        Uint8Type   kb4:1;
        Uint8Type   kb3:1;
        Uint8Type   kb2:1;
        Uint8Type   kb1:1;
        Uint8Type   kb0:1;
    } keyBlock;
    Uint8Type   rawByte;
}KpbMflBlocksType;

typedef struct
{
    Uint8Type           focussedFrameId;
    KpbMflBlocksType    mflBlocks;
} KpbMflAssignedType;

typedef struct
{
    KpbCommonType           common;
    Uint8Type               frameId;
    KPB_enRelativePriority  relPrio;
    KpbMflAssignedType      mflAssigned;
    KPB_enAsgResponse       asgResponse;
    KPB_enFsgRequest        fsgRequest;
} KpbFrameStatusType;

typedef struct
{
    KpbCommonType           common;
    KPB_enFrameIdType       frameId;
    BapArrayHeaderType      arrayHeader;
}KpbFrameDataIndType;

typedef struct
{
    KpbCommonType           common;
    Uint8Type               frameId;
}KpbFrameDataAckType;

typedef struct
{
    KpbCommonType           common;
}KpbScrollBarIndType;

typedef union
{
    KpbCommonType       common;
    KpbOpStateType      opState;
    KpbAsgCapType       asgCap;
    KpbAsgConfType      asgConf;
    KpbMflBlockDefType  mflBlockDef;
    KpbLsgStatusType    lsgStatus;
    KpbFrameStatusType  frameStatus;
    KpbFrameDataIndType frameData;
    KpbFrameDataAckType frameDataAck;
    KpbScrollBarIndType scrollBar;
} KpbIndType;




/* ------------------------------------------ */
/* here starts the KPB request specific types */

typedef Uint8Type       KpbCellContentType[KPB_MAX_CELL_CONTENT_SIZE];

typedef union
{
    struct 
    {
        Uint8Type   visible:1;
        Uint8Type   :3;
        Uint8Type   activated:1;
        Uint8Type   selected:1;
        Uint8Type   focused:1;
        Uint8Type   selectable:1;
    }           bit;
    Uint8Type   rawByte;
}KpbCellAttributeType;

typedef struct
{
    Uint8Type           length;
    KpbCellContentType  content;
}KpbCellNameType;

typedef struct
{
    BooleanType             isSet;
    Uint8Type               pos;
    KPB_enCellEntryType     type;
    KpbCellAttributeType    attribute;
    KpbCellNameType         name; 
}KpbFrameCellType;

typedef struct
{
    KpbCommonType           common;
    KPB_enPartToSend        partToSend;
    KPB_enFrameIdType       frameId;
    Uint8Type               listElemOnTop;
    KPB_enListOperationType listOperation;
    BapArrayHeaderType      arrayHeader;
    KpbFrameCellType        cell[KPB_MAX_FRAME_CELL_NUMBER];
}KpbFrameDataReqType;

typedef union
{
    struct 
    {
        Uint8Type   :5;
        Uint8Type   popup_frame:1;
        Uint8Type   endless_list:1;
        Uint8Type   arrows:1;
    }           bit;
    Uint8Type   rawByte;
}KpbSbAttributeType;

typedef struct
{
    KpbCommonType           common;
    KPB_enFrameIdType       frameId;
    KpbSbAttributeType      attribute;
    Uint8Type               sliderLength;
    Uint8Type               sliderPosition;
}KpbScrollBarReqType;

typedef union
{
    KpbCommonType           common;
    KpbFrameStatusType      frameStatus;
    KpbScrollBarReqType     scrollBar;
} KpbReqType;


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
\brief	description of the interface IKPB_INIT
*/
//#ifdef IKPB_INIT /* BEGIN DECLARE INTERFACE(IAPP_PHONE_INIT) */

/*----------------------------------------------------------------------------*/
/* Function    : KPB_sfInitApp(void)                   */   
/** 
    \brief      Inits the phone application 
  
    \brief      Description:\n
                Inits the Phone Application \n
    
    \return     SuccessFailType
                success or error
        
   */
/*----------------------------------------------------------------------------*/
SuccessFailType KPB_sfInitApp(void);

//#endif /* END DECLARE INTERFACE(IKPB_INIT) */

/** 
\brief	description of the interface IKPB_EVENTS
*/
//#ifdef IKPB_EVENTS /* BEGIN DECLARE INTERFACE(IAPP_PHONE_EVENTS) */

SuccessFailType KPB_sfHandleKpbRequest(KpbReqType *pKpbReq);
SuccessFailType KPB_sfSendFrameData( KpbFrameDataReqType *pFrameData,
                                     BooleanType activate, 
                                     KPB_enRelativePriority priority);
SuccessFailType KPB_sfShutdown();

BooleanType     KPB_bIsKeyAssigned(LOGI_BUT_MAN_ENUM_BUTTON_NAMES key);
BooleanType     KPB_bIsAsgAvailable();
BooleanType     KPB_bIsAsgPresentation();
Uint16Type      KPB_u16GetNumChar();
Uint8Type       KPB_u16GetVisListElements();
KPB_enAsgStatus KPB_enGetAsgState();
//KpbFrameDataReqType *KPB_stGetFrameDataElement();
void KPB_vReleaseFrameDataElement(KpbFrameDataReqType **pFrameData, BooleanType bSucc);
void KPB_vHandleEvents(EventMaskType events);
void KPB_vSwitchOpState(KPB_enOpState opState);
void KPB_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
void KPB_vSendFrameStatusPresReqOff(void);
void KPB_vSetAsgPresentationOff(void);

SuccessFailType KPB_sfAllocFrameBuffer( KpbFrameDataReqType **pFrameBuffer,
                                        BooleanType initBuff );
SuccessFailType KPB_sfFreeFrameBuffer( );
BooleanType KPB_bIsFsgQueuedUp();
BooleanType KPB_bGetLastCradleRestriction();
BooleanType _KPB_bGetVisbilitySetOnOff(void);  /*RT13591*/
//#endif /* END DECLARE INTERFACE(IKPB_EVENTS) */

#endif /* _KPB_H */

/* End Of File KPB.h */

