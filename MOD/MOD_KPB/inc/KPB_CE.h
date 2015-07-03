/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   KPB_CE.h
*-----------------------------------------------------------------------------
* Module Name:  KPB
*-----------------------------------------------------------------------------
* Description:  
*-----------------------------------------------------------------------------
* $Date: 2011-04-26 14:04:38 +0200 (Di, 26 Apr 2011) $
* $Rev: 19307 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/inc/KPB_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _KPB_CE_H
#define _KPB_CE_H

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
//#define IKPB_INIT 100 /* CONFIGURATE EXPORT INTERFACE(IAPP_KPB_OSEK) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
//#define IUART_TEMPLATE_CFG 110
//#if (IUART_TEMPLATE_CFG != IUART)
//	#error ("Incompatible interface: configured ifc = %d, reference ifc = %d!!!" ,IUART_TEMPLATE_CFG, IUART)
//#endif
	

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

typedef enum
{
    KPB_INVALID_FCT_ID = 0,
    KPB_FSG_OPERATION_STATE,
    KPB_ASG_CAPABILITIES,
    KPB_ASG_CONFIG,
    KPB_MFL_BLOCK_DEFINITION,
    KPB_FRAME_STATUS,
    KPB_FRAME_DATA,
    KPB_FRAME_DATA_ACK,
    KPB_SCROLL_BAR,
    KPB_LSG_STATUS
} KPB_enFunctionIdType;

typedef enum
{
    KPB_OP_TYPE_INVALID = 0,
    KPB_OP_TYPE_GET,
    KPB_OP_TYPE_SET,
    KPB_OP_TYPE_SET_GET,
    KPB_OP_TYPE_STATUS
} KPB_enOpType;

typedef enum
{
    KPB_OP_STATE_NORMAL_OPERATION = 0,
    KPB_OP_STATE_OFF,
    KPB_OP_STATE_KL_15_FOLLOW_UP
}KPB_enOpState;

typedef enum
{
    KPB_DISP_SIZE_SMALL = 0,
    KPB_DISP_SIZE_MEDIUM,
    KPB_DISP_SIZE_LARGE
}KPB_enDisplaySizeClass;

typedef enum
{
    KPB_VERSION_UNSUPPORTED = 0,
    KPB_VERSION_1_0,
    KPB_VERSION_1_1
}KPB_enVersionNumber;

typedef enum
{
    KPB_LSG_ALIVE_FALSE = 0,
    KPB_LSG_ALIVE_TRUE
}KPB_enLsgAlive;


typedef enum
{
    KPB_KB_NO_KEYS = 0,
    KPB_KB_ALL_SHARED_MFL_KEYS,
    KPB_KB_UP_DOWN,
    KPB_KB_LEFT_RIGHT,
    KPB_KB_UP_DOWN_LEFT_RIGHT_OK,
    KPB_KB_ROTARY_1,
    KPB_KB_ROTARY_2,
    KPB_KB_OK,
    KPB_KB_BACK
}KPB_enKeyBlock;

typedef enum
{
    KPB_RELATIVE_PRIO_LOW,
    KPB_RELATIVE_PRIO_HIGH
}KPB_enRelativePriority;


typedef enum
{
    KPB_ASG_SET_V_IS_OFF = 0,
    KPB_ASG_SET_V_IS_ON,
    KPB_ASG_QUEUED_UP,
    KPB_ASG_REQ_V_IS_ON,
    KPB_ASG_NO_RESPONSE = 0xFF
} KPB_enAsgResponse;

typedef enum
{
    KPB_FSG_REQ_V_IS_OFF = 0,
    KPB_FSG_REQ_V_IS_ON,
    KPB_FSG_ACCEPT_ASG_REQ,
    KPB_FSG_REFUSE_ASG_REQ,
    KPB_FSG_REQ_V_IS_OFF_MENUE_QUIT_SHORT,
    KPB_FSG_REQ_V_IS_OFF_MENUE_QUIT_LONG,
    KPB_FSG_NO_REQUEST = 0xFF
} KPB_enFsgRequest;

typedef enum
{
    KPB_NO_LIST_OPERATION = 0,      // 0x00
    KPB_LIST_UP,                    // 0x01
    KPB_LIST_DOWN,                  // 0x02
    KPB_PAGE_FLIP_UP,               // 0x03
    KPB_PAGE_FLIP_DOWN,             // 0x04
    KPB_FAST_SCROLLING_UP,          // 0x05
    KPB_FAST_SCROLLING_DOWN,        // 0x06
    KPB_FAST_PAGE_FLIP_UP,          // 0x07
    KPB_FAST_PAGE_FLIP_DOWN,        // 0x08
    KPB_MOVING_TO_TOP_OF_LIST,      // 0x09
    KPB_MOVING_TO_BOTTOM_OF_LIST,   // 0x0A    
    KPB_MENU_CHANGE,                // 0x0B, v1.10
    KPB_ENTER_MENUE,                // 0x0C, v1.11
    KPB_LEAVE_MENUE                 // 0x0D, v1.11
//    KPB_LAUNCH_POPUP,             // 0x0E, v1.11                use 0x0B instead
//    KPB_LIST_OP_0x1B = 0x1B       // 0x1B, undefined in v1.11   use 0x0B instead
}KPB_enListOperationType;

typedef enum
{
    KPB_CELL_TYPE_ANY_STRING = 0,
    KPB_CELL_TYPE_DIRECTORY,
    KPB_CELL_TYPE_FILE_NAME,
    KPB_CELL_TYPE_PHONE_NUMBER,
    KPB_CELL_TYPE_MENU_ITEM,
    KPB_CELL_TYPE_CHECK_BOX,
    KPB_CELL_TYPE_RADIO_BUTTON,
    KPB_CELL_TYPE_SPELLER,
    KPB_CELL_TYPE_SUB_MENU
}KPB_enCellEntryType;

typedef enum
{
    KPB_FRAME_ID_INVALID = 0,
    KPB_FRAME_ID_1,
    KPB_FRAME_ID_2,
    KPB_FRAME_ID_3,
    KPB_FRAME_ID_4,
    KPB_FRAME_ID_5,
    KPB_FRAME_ID_6,
    KPB_FRAME_ID_7,
    KPB_FRAME_ID_ANY_FRAME = 0xFE,
    KPB_FRAME_ID_NOT_AVAIL = 0xFF
}KPB_enFrameIdType;

typedef enum
{
    KPB_CELL_PART_1 = 0,
    KPB_CELL_PART_2
}KPB_enPartToSend;

typedef enum
{
    KPB_ASG_STATUS_UNDEFINED,
    KPB_ASG_STATUS_DEAD,
    KPB_ASG_STATUS_ALIVE,     //changed by nikolov, RT:11346
    KPB_ASG_STATUS_SHUT_DOWN, //changed by nikolov, RT:11346
    KPB_ASG_STATUS_STARTING   //changed by nikolov, RT:11346
} KPB_enAsgStatus;


#endif /* _KPB_CE_H */

/* End Of File KPB_CE.h */
