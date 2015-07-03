/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR_ASR.h
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition ASR specific types
*-----------------------------------------------------------------------------
* $Date: 2009-02-03 16:38:55 +0100 (Di, 03 Feb 2009) $
* $Rev: 4298 $
* $Author: schroeder $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/inc/VCR_ASR.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _VCR_ASR_H
#define _VCR_ASR_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "ATCommonData.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "VCR_CE.h"
#include "VCR_CI.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/* types for request messages from CAN -> VCR, e.g. to request read / write of
 * a specific parameter */
typedef enum {
    VCR_PERFREQ_READ  = 0,
    VCR_PERFREQ_WRITE = 1
}VCRPerfMeasReqTypeEnum;

/* types for status messages from VCR -> CAN e.g. for VCR status and commands */
typedef enum {
    VCR_PERFSTAT_STATUS      = 0,   /* indicate only current SIVR status  */
    VCR_PERFSTAT_READACK     = 1,   /* acknowledge for read request       */
    VCR_PERFSTAT_WRITEACK    = 2,   /* acknowledge for write request      */
    VCR_PERFSTAT_CMD         = 3    /* command message which may include several
                                       commands and sub commands          */
}VCRPerfMeasStatusTypeEnum;

typedef enum {
    VCR_PERFSIVR_STATUS_INACTIVE      = 0, /* SIVR is inacvtive                  */
    VCR_PERFSIVR_STATUS_ACTIVE_RECO   = 1, /* SIVR active and RECO is ongoing    */
    VCR_PERFSIVR_STATUS_ACTIVE_NORECO = 2  /* SIVR active and NO RECO is ongoing */
}VCRPerfSIVRStatusEnum;

typedef struct {
    VCRPerfMeasReqTypeEnum   type; /* indicate type of message */
    /* ID of the parameter - valid IDs to be specified */
    Uint16Type               param_id;
    /* 32 bit value -> for specific types cast may be required */
    Uint32Type               param_value;
}VCRPerfMeasReqMsgType;


typedef struct {
    VCRPerfMeasStatusTypeEnum type; /* == VCR_PERFSTAT_STATUS */
    VCRPerfSIVRStatusEnum     status;
    ATSIVRPromptEnum          menu_id; /* current menu id */
}VCRPerfMeasStatusType;

typedef struct {
    VCRPerfMeasStatusTypeEnum type; /* == VCR_PERFSTAT_READACK or VCR_PERFSTAT_WRITEACK */
    Uint16Type                param_id;
    /* 32 bit value -> for specific types cast may be required */
    Uint32Type                param_value;
}VCRPerfMeasAckType;

#define AST_SUB_TEXT_MAX_LEN 64
typedef Uint8Type ASRSubText[AST_SUB_TEXT_MAX_LEN+1];

typedef struct {
    /* command id */
    ATSIVRCmdIdEnum         cmd_id;
    /* confidence value if this command id */
    Uint16Type              confidence_rate;
    /* number sub commands between 0 .. VCR_MAXNUM_PERFM_SUBCMD */
    Uint8Type               num_sub_cmd;
    /* text for subcommands*/
    ASRSubText              sub[VCR_MAXNUM_PERFM_SUBCMD];
    /* length for each subcommand between 0 and AT_SIVR_MAX_PROMPT_TEXT_LEN */
    Uint16Type              sub_len[VCR_MAXNUM_PERFM_SUBCMD];

}VCRPerfMeasCmdInfoType;

typedef struct {
    /* number of commands in the message, between 1 and VCR_MAXNUM_PERFM_CMD */
   Uint8Type                  num_cmd;
   VCRPerfMeasCmdInfoType     info[VCR_MAXNUM_PERFM_CMD];
}VCRPerfMeasCmdValueType;


typedef struct {
    VCRPerfMeasStatusTypeEnum type; /* == VCR_PERFSTAT_CMD_STATIC */
    /* pointer to pool element from type VCRPerfMeasCmdValueType */
    VCRPerfMeasCmdValueType * cmd;
}VCRPerfMeasCmdType;

typedef union {
    VCRPerfMeasStatusType   status;
    VCRPerfMeasAckType      ack;
    VCRPerfMeasCmdType      cmd;
}VCRPerfMeasStatusUnionType;

typedef struct {
    VCRPerfMeasStatusUnionType u; /* message union */
}VCRPerfMeasStatusMsgType;


#endif /* _VCR_ASR_H */
