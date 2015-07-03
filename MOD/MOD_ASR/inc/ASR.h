/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ASR.h
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2009-01-29 17:01:51 +0100 (Do, 29 Jan 2009) $
* $Rev: 4178 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ASR/trunk/inc/ASR.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ASR_H
#define _ASR_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ASR_CI.h"
#include "ASR_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
/**
\brief	Maximum number of buttons to be handled
*/
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef struct
{ 
    Uint8Type active;
    Uint8Type recog;
    Uint16Type menuId;
}ASR_StatusType;


typedef union 
{
    Uint32Type instr;
    struct
    {
        Uint32Type Is_Command_ID                :1;
        Uint32Type Is_String_TimeMultiplexed    :1;
        Uint32Type Is_Muliplex_Last_Msg         :1;
        Uint32Type Cnt_Multiplex_String         :4;
        Uint32Type Is_SubCmd_Last_Msg           :1;
        Uint32Type Cnt_SubCmd                   :4;
        Uint32Type Is_NBest_Last_Msg            :1;
        Uint32Type Cnt_NBest                    :4;
        Uint32Type                              :15;
    }types;
    
}ASR_TypeUnionInfo;

typedef enum
{
    ASR_CMD_STATE_ONE_CMD,
    ASR_CMD_STATE_MORE_CMD,
    ASR_CMD_STATE_POOL_FREE,
    ASR_CMD_STATE_IDLE
}ASR_CmdStateType;

typedef enum
{
    ASR_NO_SUBCMD,
    ASR_ONE_SUBCMD,
    ASR_MORE_SUBCMD
}ASR_MoreCmds;
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
\brief	description of the interface ILED
*/

SuccessFailType ASR_sfInit(void);
SuccessFailType ASR_sfSendCyclicASRMessage(EventMaskType event);
void ASR_vSetPerfMeasOnOff(Uint8Type onOff);

#endif /* _ASR_H */

/* End Of File ASR.h */
