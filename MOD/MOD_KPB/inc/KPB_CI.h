/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   KPB_CI.h
*-----------------------------------------------------------------------------
* Module Name:  KPB
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-06-01 13:15:54 +0200 (Mi, 01 Jun 2011) $
* $Rev: 20002 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/inc/KPB_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _KPB_CI_H
#define _KPB_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "bap_defines.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define ITEMPLATE 100 /* CONFIGURATE EXPORT INTERFACE(ITEMPLATE) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/
#define ITMPL_Func IUART_Func

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the #define
*/

#define LAST_MODE_ON    1
#define LAST_MODE_OFF   0



#define KPB_OBSERVED_CELLS_FRAME_ID_1     4
#define KPB_OBSERVED_CELLS_FRAME_ID_2     1
#define KPB_OBSERVED_CELLS_FRAME_ID_3     2
#define KPB_OBSERVED_CELLS_FRAME_ID_4     3
#define KPB_OBSERVED_CELLS_FRAME_ID_5     0
#define KPB_OBSERVED_CELLS_FRAME_ID_6     0
#define KPB_OBSERVED_CELLS_FRAME_ID_7     4

#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_1     0
#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_2     ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_1 + KPB_OBSERVED_CELLS_FRAME_ID_1 )
#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_3     ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_2 + KPB_OBSERVED_CELLS_FRAME_ID_2 )
#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_4     ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_3 + KPB_OBSERVED_CELLS_FRAME_ID_3 )
#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_5     ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_4 + KPB_OBSERVED_CELLS_FRAME_ID_4 )
#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_6     ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_5 + KPB_OBSERVED_CELLS_FRAME_ID_5 )
#define KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_7     ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_6 + KPB_OBSERVED_CELLS_FRAME_ID_6 )

#define KPB_OBSERVED_CELLS_MAX                   ( KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_7 + KPB_OBSERVED_CELLS_FRAME_ID_7 )





/*----------------------------------------------------------------------------*/
/* Module global type definitions                                             */
/*----------------------------------------------------------------------------*/
typedef struct
{
    Uint8Type   inPresentation:1;
    Uint8Type   cradleRestriction:1;
    Uint8Type   :6;
}KpbLastModeType;



typedef enum
{
/* 0 */    KPB_FRAME_DATA_INVALID = 0,
/* 1 */    KPB_FRAME_DATA_EMPTY_NOT_SENT,
/* 2 */    KPB_FRAME_DATA_EMPTY_SENT,
/* 3 */    KPB_FRAME_DATA_EMPTY_ACK_RECVD,
/* 4 */    KPB_FRAME_DATA_NOT_SENT,
/* 5 */    KPB_FRAME_DATA_SENDING_IN_PROGRESS,
/* 6 */    KPB_FRAME_DATA_SENT,
/* 7 */    KPB_FRAME_DATA_ACK_RECVD,
/* 8 */    KPB_FRAME_REQ_V_IS_OFF_SENT,
/* 9 */    KPB_FRAME_SET_V_IS_OFF_RECVD,
/* 10 */   KPB_FRAME_REQ_V_IS_ON_SENT,
/* 11 */   KPB_FRAME_SET_V_IS_ON_RECVD,
/* 12 */   KPB_FRAME_REQ_V_IS_ON_REFUSED,
/* 13 */   KPB_FRAME_REQ_V_IS_OFF_REFUSED,
/* 14 */   KPB_FRAME_SET_V_IS_OFF_WHILE_FDPENDING
}KpbSendFrameDataState;

typedef enum
{
    KPB_SHUTDOWN_INVALID = 0,
    KPB_SHUTDOWN_STATUS_WRITE_PERSISTENT,
    KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_NOT_SENT,
    KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_SENT,
    KPB_SHUTDOWN_STATUS_SET_V_IS_OFF_RECVD,
    KPB_SHUTDOWN_STATUS_OPSTATE_OFF_SENT
}KpbShutdownState;


typedef enum
{
    KPB_PENDING_INVALID = 0,
    KPB_PENDING_SEND_FD,
    KPB_PENDING_SHUTDOWN
}KPB_enPendingId;

typedef struct
{
    KPB_enPendingId     id;
}KpbCommonPending;

typedef struct
{
    KpbCommonPending        common;
    KpbSendFrameDataState   state;
    KpbFrameDataReqType     *pData;
    Uint8Type               retries;
    KPB_enPartToSend        partToSend;
    KPB_enFrameIdType       emptyFrameId;
    KPB_enFrameIdType       frameId;
    BapArrayHeaderType      getHeader;
    
    struct
    {
        KPB_enFrameIdType       id;
        KPB_enRelativePriority  priority;
    }                       activateFrame; 
}KpbSendFdPending;


typedef struct
{
    KpbCommonPending        common;
    KpbShutdownState        state;
}KpbShutdownPending;


typedef union
{
    KpbCommonPending       common;
    KpbSendFdPending       sendFd;
    KpbShutdownPending     shutdown;
}KpbPending;



/* struct to remember values for frameStatus context */
typedef struct
{
    KPB_enFrameIdType       activeFrameId;
    KPB_enRelativePriority  priority;
    KpbMflAssignedType      mflAssigned;
    BooleanType             isVisible;
}KpbFrameStatusVal;

typedef struct
{
    Uint8Type capabilities:1;
    Uint8Type config:1;
    Uint8Type mflBlockDef:1;
    Uint8Type :5;
}KpbAsgInitState;


typedef struct
{
    KPB_enOpState           opState;
    KPB_enVersionNumber     versionNumber;
    KPB_enDisplaySizeClass  displayClass;
    Uint8Type               visibleElements;
    Uint8Type               ASGSupervisionFailureCounter;
//    KPB_KeyBlockType        mflBlockDef;
    KPB_enKeyBlock          mflBlockDef[KPB_MAX_KEY_BLOCK_NUMBER];
    BooleanType             isAsgResponseOutstanding;
    KPB_enAsgStatus         asgState;
    BooleanType             asgAvailable;
/* variable to remember for frameStatus context */
    KpbFrameStatusVal       fs;
    KpbAsgInitState         asgInitState;
    KpbAsgInitState         asgInitStateAfterReset;
    KpbLastModeType         lastMode;
    BooleanType             kl15OnQueued;

    BooleanType             notifyPresentationOff;
    BooleanType             isFsgQueuedUp;
    BooleanType             isAppsNotified;

    struct
    {   
        KpbSendFdPending       sendFd;
        KpbShutdownPending     shutdown;
    }waitForProcessing;

    KpbPending              pending;

}KpbContext;

typedef struct
{
    KpbFrameDataReqType buffer;
    BooleanType         free;
 
}KpbFrameDataContext;
/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief  sends a BAP Error message with the given BAP fctId and errorCode	
*/
void _KPB_vChangeAsgState(KPB_enAsgStatus asgState);
void _KPB_vSetAsgAvailable(BooleanType isAvail);


/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
//#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/



#endif /* _KPB_CI_H */

/* End Of File KPB_CI.h */
