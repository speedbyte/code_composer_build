/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ASR_C1.c
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:	  To be run in Host Task
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ASR/trunk/src/ASR_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ASR.h"
#include "can_inc.h"
#include "UHV_NAR.h"
#include "VCR_ASR.h"
#include "POOL.h"

#include "ASR_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/* *
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/* *
\brief	description of the method
*/
static void ASR_vString(Uint8Type *data, Uint8Type length);
static void ASR_vString0(const Uint8Type *data, Uint8Type start, Uint8Type length);
static void ASR_vString1(const Uint8Type *data, Uint8Type start, Uint8Type length);
static void ASR_vString2(const Uint8Type *data, Uint8Type start, Uint8Type length);
static void ASR_vString3(const Uint8Type *data, Uint8Type start, Uint8Type length);
static void ASR_vTypeInfo(Uint16Type conficence_rate, ASR_TypeUnionInfo type_Info, Uint32Type command_Id, Uint8Type second);
static StatusType ASR_stSetOsekRelAlarm(  AlarmType   inAlarmID, 
                                                TickType    inIncrement, 
                                                TickType    inCycle);
static void ASR_vOneCmdNoSubcmd(const VCRPerfMeasCmdInfoType *cmdInfo, Uint8Type multiple);
static void ASR_vOneCmdOneSubcmd(VCRPerfMeasCmdInfoType *cmdInfo, Uint8Type multiple);
static void ASR_vMoreCmd(void);
static void ASR_vOneCmd(void);
static void ASR_vOneCmdMoreSubcmd(VCRPerfMeasCmdInfoType *cmdInfo, Uint8Type multiple);

static void ASR_vRemoveCurrentCommand(void);
static void ASR_vStoreCommand(VCRPerfMeasCmdValueType * status);
static VCRPerfMeasCmdValueType* ASR_pstGetCurrentCommand(void);
static SuccessFailType ASR_sfWorkingJobs(void);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static VCRPerfMeasCmdValueType* commandPending[3] = {NULL, NULL, NULL};
static VCRPerfMeasCmdValueType* currentCommand = NULL;

Uint8Type zeroString[8] = {0U,0U,0U,0U,0U,0U,0U,0U};

static VCRPerfMeasStatusMsgType ASRStatusMsgType;

static ASR_StatusType _ASR_Status;
static Uint8Type _ASR_PerfMeasValue;
static Uint8Type _cntOneCmdMoreSub;
static Uint8Type _cntMoreCmd;
static ASR_CmdStateType ASR_COMMAND_STATE;
Uint8Type firstCANframe = TRUE;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/* *
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : ASR_sfInit                                                   */
/**
    \brief      initialise ASR to default

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ASR_sfInit(void)
{
    SuccessFailType  sF     = SUCCESS;
    _ASR_Status.active      = 0;
    _ASR_Status.recog       = 0; 
    _ASR_Status.menuId      = 0x0000;
    _cntOneCmdMoreSub       = 0;
    _cntMoreCmd             = 0;
    ASR_COMMAND_STATE       = ASR_CMD_STATE_IDLE;
    _ASR_PerfMeasValue      = (Uint8Type)BT_FALSE;
    
    (void)SetRelAlarm(CTA_HOST_TASK_CYCLIC_CAN_ASR_TX_MSG, MSEC(10), 0);
    
    (void)SetRelAlarm(CTA_HOST_ASR_TRIGGER, MSEC(100), MSEC(50));
    
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vStoreCommand                                            */
/** 
    \brief      Stores the given command within the pending command list
  
    \brief      Description:\n
    			The write commands are enqueued but only one command pending is allowed per client.
                Precondition: The given command should not be present already within\n
*               the list 
 
    \param      status
                VCRPerfMeasCmdValueType *
                pointer to the VCRPerfMeasCmdValueType

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vStoreCommand(VCRPerfMeasCmdValueType * status)
{
    Uint16Type i = 0;
    for (i=0; i<3; i++)
    {
        if (commandPending[i] == NULL)
        {
            commandPending[i] = status;
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_pstGetCurrentCommand                                     */
/** 
    \brief      Returns the command which is pending having the highest\n
                priority, NULL when no pending command is available
  
    \brief      Description:\n
                Precondition: The current command's buisy state will be rested \n

    \return     VCRPerfMeasCmdValueType
                returns pointer to the VCRPerfMeasCmdValueType block of the highest\n
                priority pending command.
                value limits
*/
/*----------------------------------------------------------------------------*/
static VCRPerfMeasCmdValueType* ASR_pstGetCurrentCommand(void)
{
    VCRPerfMeasCmdValueType* prioCommand = NULL;
    Uint16Type i = 0;
    for (i=0; i<3; i++)
    {
        if (commandPending[i] != NULL)
        {
            if (prioCommand == NULL)
            {
                prioCommand = commandPending[i];
            }
        }
    }
    return prioCommand;    
}
/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_vRemoveCurrentCommand                                 */
/** 
    \brief      Removes the current command from the pending commands
  
    \brief      Description:\n
                Precondition: The current command's buisy state will be rested \n
                
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vRemoveCurrentCommand(void)
{
    Uint16Type i = 0;

    for (i=0; i<3; i++)
    {
        if (commandPending[i] == currentCommand)
        {
            commandPending[i] = NULL;
            (void)POOL_sfFree(currentCommand);
        }
    }
    currentCommand = NULL;  
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfCheckBusOff                                        */
/** 
    \brief      Task Function to handle transmit message for Host Task. 
  
    \brief      Waits for incoming events and if one arrives function \n
                set message values and transmit these message to CAN \n 

    \param      event
                EventMaskType
                Bitmask containing all unhandled events
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType ASR_sfSendCyclicASRMessage(EventMaskType event)     
{  
    SuccessFailType sF = SUCCESS;

    if (event & evHostTaskASRCyclicTxMsg)
    {        
        c1_mASR0_Status_c      = 0x00;
        c2_mASR0_Status_c      = 0x00;
        c3_mASR0_Status_c      = 0x00;
        c4_mASR0_Status_c      = 0x00;
        
        /*set or reset activation of the ASR-System */
        ASR0_STATUS_ACTIVE = _ASR_Status.active;  
        
        /*set or reset the recognition process */
        ASR0_STATUS_RECOGOPEN = _ASR_Status.recog;
        
        c3_mASR0_Status_c = (Uint8Type)((_ASR_Status.menuId & 0xff00) >> 8);
        c2_mASR0_Status_c = (Uint8Type)(_ASR_Status.menuId & 0xff);
        
        /* send the ASR0 */
        if ((Uint8Type)BT_TRUE == _ASR_PerfMeasValue)
        {
            if(CanTransmit( CanTxmASR0_Status ) == kCanTxOk)
            {
                DisableAllInterrupts();
                mASR0_Status_conf_b = 0;
                EnableAllInterrupts();
            }
        }
        (void)ASR_stSetOsekRelAlarm( CTA_HOST_TASK_CYCLIC_CAN_ASR_TX_MSG, MSEC(1000), 0);
        (void)ClearEvent(evHostTaskASRCyclicTxMsg);
    }
    
    if (event & evHostTaskASRStatus)
    {
        (void)ReceiveMessage(msgHostTaskASRStatus, &ASRStatusMsgType);

        switch (ASRStatusMsgType.u.status.status)
        {
            /* PTT active and NO RECO is ongoing */
            case VCR_PERFSIVR_STATUS_ACTIVE_NORECO:
                _ASR_Status.active  = 1U;
                _ASR_Status.recog   = 0U; 
                break;
            /* PTT active and VR is ongoing */    
            case VCR_PERFSIVR_STATUS_ACTIVE_RECO:
                _ASR_Status.active  = 1U;
                _ASR_Status.recog   = 1U; 
                break;
            /* PTT and voice recognition inacvtive */
            case VCR_PERFSIVR_STATUS_INACTIVE:
            default:
                _ASR_Status.active  = 0U;
                _ASR_Status.recog   = 0U; 
                break;
        }
        
        
        if (sizeof(ASRStatusMsgType.u.status.menu_id) < 4U)
        {
            _ASR_Status.menuId = (Uint16Type)ASRStatusMsgType.u.status.menu_id;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_TRACE,UHV_ASR,"Status active:%d recog: %d menuId: %d",
                    _ASR_Status.active, _ASR_Status.recog, _ASR_Status.menuId);
#endif
        DEBUG_VALUE2(ASR_SWI_TRC,MOD_ASR,"Status active:%d recog: %d",_ASR_Status.active, _ASR_Status.recog);
        DEBUG_VALUE2(ASR_SWI_TRC,MOD_ASR,"Status active:%d menuId: %d",_ASR_Status.active, _ASR_Status.menuId);
                    
        (void)ASR_stSetOsekRelAlarm(CTA_HOST_TASK_CYCLIC_CAN_ASR_TX_MSG, MSEC(0), 0);

        if (VCR_PERFSTAT_CMD == ASRStatusMsgType.u.status.type)
        {
            if ( NULL == ASRStatusMsgType.u.cmd.cmd )
            {   /* release element */
                ASR_COMMAND_STATE = ASR_CMD_STATE_POOL_FREE;
            }
            else
            {
 
                if (0 != ASRStatusMsgType.u.cmd.cmd->num_cmd)
                {
                    if (1 == ASRStatusMsgType.u.cmd.cmd->num_cmd)
                    {
                        ASR_COMMAND_STATE = ASR_CMD_STATE_ONE_CMD;
                        ASR_vStoreCommand(ASRStatusMsgType.u.cmd.cmd);
                    }
                    else if ( 1 < ASRStatusMsgType.u.cmd.cmd->num_cmd)
                    {
                        ASR_COMMAND_STATE = ASR_CMD_STATE_MORE_CMD;
                        ASR_vStoreCommand(ASRStatusMsgType.u.cmd.cmd);
                    }
                    else
                    {
                        ASR_COMMAND_STATE = ASR_CMD_STATE_POOL_FREE;
                    }
                }    
                else
                {
                    ASR_COMMAND_STATE = ASR_CMD_STATE_POOL_FREE;
                }
            }
        }
        (void)ClearEvent(evHostTaskASRStatus);
    }
    if (event & evHostTaskASRTrigger)
    {
        (void)ASR_sfWorkingJobs();
        (void)ClearEvent(evHostTaskASRTrigger);
    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : ASR_sfWorkingJobs                                            */
/** 
    \brief       
  
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType ASR_sfWorkingJobs(void)
{
    SuccessFailType sF = SUCCESS;
    
    switch (ASR_COMMAND_STATE)
    {
        case ASR_CMD_STATE_ONE_CMD:
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3,UHV_ASR,"ASR_CMD_STATE_ONE_CMD");
#endif
            DEBUG_TEXT(ASR_SWI_DBG,MOD_ASR,"ASR_CMD_STATE_ONE_CMD");
            ASR_vOneCmd();
            break;
         
        case ASR_CMD_STATE_MORE_CMD:
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3,UHV_ASR,"ASR_CMD_STATE_MORE_CMD");
#endif
            DEBUG_TEXT(ASR_SWI_DBG,MOD_ASR,"ASR_CMD_STATE_MORE_CMD");
            ASR_vMoreCmd();
            break;
            
        case ASR_CMD_STATE_POOL_FREE:
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3,UHV_ASR,"ASR free pool %d", ASRStatusMsgType.u.cmd.cmd);
#endif
            DEBUG_VALUE1(ASR_SWI_DBG,MOD_ASR,"ASR free pool %d", ASRStatusMsgType.u.cmd.cmd);
            (void)POOL_sfFree(ASRStatusMsgType.u.cmd.cmd);
            ASR_COMMAND_STATE = ASR_CMD_STATE_IDLE;
            break;
        
        case ASR_CMD_STATE_IDLE:
            /* do nothing - no current jobs available */
            break;
           
    }
    return sF;    
}
/*----------------------------------------------------------------------------*/
/* Function    : ASR_vMoreCmd                                         */
/** 
    \brief      Decision how many sub commands in the message. 
  
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vOneCmd(void)
{
    currentCommand = ASR_pstGetCurrentCommand();
    if (currentCommand == NULL)
    {
        currentCommand = NULL;
        return;
    }
    
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG_3,UHV_ASR,"ASR_vOneCmd subcmd %d", currentCommand->info[0].num_sub_cmd);
#endif
    DEBUG_VALUE1(ASR_SWI_DBG,MOD_ASR,"ASR_vOneCmd subcmd %d", currentCommand->info[0].num_sub_cmd);
    
    if (currentCommand->info[0].num_sub_cmd == 0)
    {
        ASR_vOneCmdNoSubcmd(&currentCommand->info[0], FALSE);
    }
    else if (currentCommand->info[0].num_sub_cmd == 1)
    {
        ASR_vOneCmdOneSubcmd(&currentCommand->info[0], FALSE);
    }
    else if (currentCommand->info[0].num_sub_cmd > 1)
    {
        ASR_vOneCmdMoreSubcmd(&currentCommand->info[0], FALSE);
    }
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vMoreCmd                                                 */
/** 
    \brief      Decision how many sub commands in the message.
  
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vMoreCmd(void)
{
    Uint8Type moreMsg;
    currentCommand = ASR_pstGetCurrentCommand();
    if (currentCommand == NULL)
    {
        currentCommand = NULL;
        return;
    }
    
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG_3,UHV_ASR,"ASR_vMoreCmd cmd %d", currentCommand->num_cmd);
#endif
    DEBUG_VALUE1(ASR_SWI_DBG,MOD_ASR,"ASR_vMoreCmd cmd %d", currentCommand->num_cmd);
    
    if (_cntMoreCmd < currentCommand->num_cmd)
    {
        if ((_cntMoreCmd+1) == currentCommand->num_cmd)
            moreMsg = FALSE;
        else
            moreMsg = TRUE;

        if (currentCommand->info[_cntMoreCmd].num_sub_cmd == 0)
        {
            ASR_vOneCmdNoSubcmd(&currentCommand->info[_cntMoreCmd], moreMsg);
        }
        else if (currentCommand->info[_cntMoreCmd].num_sub_cmd == 1)
        {
            ASR_vOneCmdOneSubcmd(&currentCommand->info[_cntMoreCmd], moreMsg);
        }
        else if (currentCommand->info[_cntMoreCmd].num_sub_cmd > 1)
        {
            ASR_vOneCmdMoreSubcmd(&currentCommand->info[_cntMoreCmd], moreMsg);
        }
    }
    return;
}



/*----------------------------------------------------------------------------*/
/* Function    : ASR_vOneCmdMoreSubcmd                                         */
/** 
    \brief      This function handle one command id with more than one subcommands. 

    \param      cmdInfo *
                VCRPerfMeasCmdInfoType
                structure with command id, confidence rate and subcommand  

    \param      multiple
                Uint8Type
                TRUE - last command id 
                FALSE - not last command id  

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vOneCmdMoreSubcmd(VCRPerfMeasCmdInfoType *cmdInfo, Uint8Type multiple)
{
    ASR_TypeUnionInfo typeInfo;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "ASR_vOneCmdMoreSubcmd");
#endif
    DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "ASR_vOneCmdMoreSubcmd");
    typeInfo.types.Is_SubCmd_Last_Msg = 0;
    typeInfo.types.Is_Command_ID = 1;
    typeInfo.types.Is_NBest_Last_Msg = 0;
    typeInfo.types.Is_String_TimeMultiplexed = 0;
    typeInfo.types.Cnt_Multiplex_String = 0;
    typeInfo.types.Is_Muliplex_Last_Msg = 0; 
    typeInfo.types.Cnt_NBest = _cntMoreCmd;   

    if (firstCANframe == TRUE)
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "OneCmdMoreSubcmd first Frame");
#endif
        DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "OneCmdMoreSubcmd first Frame");
        /* first CAN Frame */       
        typeInfo.types.Is_SubCmd_Last_Msg = 0;
        typeInfo.types.Is_Command_ID = 1;
        typeInfo.types.Cnt_SubCmd = 0;
    
        ASR_vTypeInfo(cmdInfo->confidence_rate,  typeInfo, (Uint32Type)cmdInfo->cmd_id, FALSE);
        firstCANframe = FALSE;
    }
    else
    {
        /* n-CAN Frame */
        if ( _cntOneCmdMoreSub < cmdInfo->num_sub_cmd)
        {
            typeInfo.types.Cnt_SubCmd = _cntOneCmdMoreSub+1;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "cnt subsmd %d", typeInfo.types.Cnt_SubCmd);
#endif
            DEBUG_VALUE1(ASR_SWI_DBG, MOD_ASR, "cnt subsmd %d", typeInfo.types.Cnt_SubCmd);
            if ((_cntOneCmdMoreSub+1) == cmdInfo->num_sub_cmd)
            {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                (void)DEBUG_Printf(UHV_DEBUG, UHV_ASR, "Is_SubCmd_Last_Msg = 1");
#endif
                DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "Is_SubCmd_Last_Msg = 1");
                typeInfo.types.Is_SubCmd_Last_Msg = 1;
                if(FALSE == multiple)
                {
                    typeInfo.types.Is_NBest_Last_Msg = 1;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                    (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "Is_NBest_Last_Msg = 1");
#endif
                    DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "Is_NBest_Last_Msg = 1");
                } 
            }
            ASR_vTypeInfo(cmdInfo->confidence_rate,  typeInfo, (Uint32Type)cmdInfo->cmd_id, TRUE);
            ASR_vString(cmdInfo->sub[_cntOneCmdMoreSub], cmdInfo->sub_len[_cntOneCmdMoreSub]);

            _cntOneCmdMoreSub++; 
        }
        else
        {   
            _cntOneCmdMoreSub = 0;
            firstCANframe = TRUE;
            if (FALSE == multiple)
            {
                ASR_vRemoveCurrentCommand();
                ASR_COMMAND_STATE = ASR_CMD_STATE_IDLE;
                _cntMoreCmd=0;
            }
            else
            {
                _cntMoreCmd++;
            }
        }

    }
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vOneCmdNoSubcmd                                         */
/** 
    \brief      This function handle one command id without subcommands. 
  
    \param      cmdInfo *
                VCRPerfMeasCmdInfoType
                structure with command id, confidence rate and subcommand  

    \param      multiple
                Uint8Type
                TRUE - last command id 
                FALSE - not last command id  

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vOneCmdNoSubcmd(const VCRPerfMeasCmdInfoType *cmdInfo, Uint8Type multiple)
{
    ASR_TypeUnionInfo typeInfo;
    
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "ASR_vOneCmdNoSubcmd");
#endif
    DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "ASR_vOneCmdNoSubcmd");
    
    typeInfo.types.Is_SubCmd_Last_Msg = 1;
    typeInfo.types.Is_Command_ID = 1;
    typeInfo.types.Is_NBest_Last_Msg = 0;
    typeInfo.types.Is_Muliplex_Last_Msg = 0;
    typeInfo.types.Cnt_SubCmd = 0;
    typeInfo.types.Cnt_NBest = _cntMoreCmd;
    typeInfo.types.Is_String_TimeMultiplexed = 0;
    typeInfo.types.Cnt_Multiplex_String = 0;

    if (FALSE == multiple)
    {
        typeInfo.types.Is_NBest_Last_Msg = 1;
    }

    ASR_vTypeInfo(cmdInfo->confidence_rate, typeInfo, (Uint32Type)cmdInfo->cmd_id, FALSE);
    if (FALSE == multiple)
    {
        ASR_vRemoveCurrentCommand();
        ASR_COMMAND_STATE = ASR_CMD_STATE_IDLE;
        _cntMoreCmd=0U;
    }
    else
    {
        _cntMoreCmd++;
    }
           
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vOneCmdOneSubcmd                                         */
/** 
    \brief      This function handle one command id with one subcommands.  
  
    \param      cmdInfo *
                VCRPerfMeasCmdInfoType
                structure with command id, confidence rate and subcommand  

    \param      multiple
                Uint8Type
                TRUE - last command id 
                FALSE - not last command id   

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vOneCmdOneSubcmd(VCRPerfMeasCmdInfoType *cmdInfo, Uint8Type multiple)
{
    ASR_TypeUnionInfo typeInfo;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "ASR_vOneCmdNoSubcmd");
#endif
    DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "ASR_vOneCmdNoSubcmd");
    typeInfo.types.Cnt_NBest = _cntMoreCmd;
    typeInfo.types.Is_String_TimeMultiplexed = 0;
    typeInfo.types.Cnt_Multiplex_String = 0;
    typeInfo.types.Is_Muliplex_Last_Msg = 0;    
    typeInfo.types.Is_NBest_Last_Msg = 0;

    if (firstCANframe == TRUE)
    {
        /* first TypeInfo CAN Frame */       
        typeInfo.types.Is_SubCmd_Last_Msg = 0;
        typeInfo.types.Is_Command_ID = 1;
        typeInfo.types.Cnt_SubCmd = 0;
        
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "OneCmdOneSubcmd first Frame");
#endif
        DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "OneCmdOneSubcmd first Frame");
        
        ASR_vTypeInfo(cmdInfo->confidence_rate,  typeInfo, (Uint32Type)cmdInfo->cmd_id, FALSE);
        firstCANframe = FALSE;
    }
    else
    {
        /* second TypeInfo with data string */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG, UHV_ASR, "OneCmdOneSubcmd second Frame");
#endif
        DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "OneCmdOneSubcmd second Frame");
        typeInfo.types.Is_SubCmd_Last_Msg = 1;
        typeInfo.types.Is_Command_ID = 0;
        typeInfo.types.Cnt_SubCmd = 1;

        if (FALSE == multiple)
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3, UHV_ASR, "ASR_vOneCmdOneSubcmd Is_NBest_Last_Msg = 1");
#endif
            DEBUG_TEXT(ASR_SWI_DBG, MOD_ASR, "ASR_vOneCmdOneSubcmd Is_NBest_Last_Msg = 1");
            typeInfo.types.Is_NBest_Last_Msg = 1;
        }
    
        ASR_vTypeInfo(cmdInfo->confidence_rate,  typeInfo, (Uint32Type)cmdInfo->cmd_id, TRUE);
        ASR_vString(cmdInfo->sub[0], cmdInfo->sub_len[0]);
        firstCANframe = TRUE;
        if (FALSE == multiple)
        {
            ASR_vRemoveCurrentCommand();
            ASR_COMMAND_STATE = ASR_CMD_STATE_IDLE;
            _cntMoreCmd=0U;
        }
        else
        {
            _cntMoreCmd++;
        }    
    }
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vSetPerfMeasOnOff                                        */
/** 
    \brief      Set a variable for output ASR Can messages. 
  
    \brief      Set internal module variable if the developer mode is \n
                switch ON or OFF and transmit all ASR message to CAN \n 

    \param      onOff
                Uint8Type
                ON, OFF

    \return     void
*/
/*----------------------------------------------------------------------------*/
void ASR_vSetPerfMeasOnOff(Uint8Type onOff)
{
    _ASR_PerfMeasValue = onOff;
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vTypeInfo                                                */
/** 
    \brief      Fill CAN message parameter of CAN ID 7A1. 

    \param      conficence_rate
                Uint16Type 
                confidence rate of VCR

    \param      type_Info
                ASR_TypeUnionInfo 
                bitfiled with different types of the CAN message

    \param      command_Id
                Uint32Type 
                cammand ids of default words

    \param      second
                Uint8Type 
                last command id?

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vTypeInfo(Uint16Type conficence_rate, ASR_TypeUnionInfo type_Info, Uint32Type command_Id, Uint8Type second)
{   
    Uint8Type i;
    /* fill all 8 byte with zeros */ 
    for (i=0;i<8;i++)
    {
        mASR1_TypeInfo._c[i] = 0;
    }
 
    ASR1_COMMAND_ID             = type_Info.types.Is_Command_ID;
    ASR1_IS_STRING_MULTIPLEXED  = type_Info.types.Is_String_TimeMultiplexed;
    ASR1_IS_MULTIPLEX_LAST_MSG  = type_Info.types.Is_Muliplex_Last_Msg;
    ASR1_CNT_MULTIPLEX_STRING   = type_Info.types.Cnt_Multiplex_String;
    ASR1_CONFIDENCE_0           = (Uint8Type) (conficence_rate & 0x00ff);
    ASR1_CONFIDENCE_1           = (Uint8Type) ((conficence_rate & 0xff00) >> 8);
    ASR1_COMMAND_ID_0           = (Uint8Type) (command_Id & 0x00ff);
    ASR1_COMMAND_ID_1           = (Uint8Type) ((command_Id & 0xff00) >> 8);
    ASR1_IS_SUBCMD_LAST_MSG     = type_Info.types.Is_SubCmd_Last_Msg;
    ASR1_CNT_SUBCMD             = type_Info.types.Cnt_SubCmd;
    ASR1_IS_NBEST_LAST_MSG      = type_Info.types.Is_NBest_Last_Msg;
    ASR1_CNT_NBEST              = type_Info.types.Cnt_NBest;

    if (TRUE == second)
    {
        mASR1_TypeInfo._c[0] = 0;
        mASR1_TypeInfo._c[1] = 0;
        mASR1_TypeInfo._c[2] = 0;
        mASR1_TypeInfo._c[4] = 0;
        mASR1_TypeInfo._c[5] = 0;
    }

    /* send the CAN message 0x7A1 */
    if(CanTransmit( CanTxmASR1_TypeInfo ) == kCanTxOk)
    {
        DisableAllInterrupts();
        mASR1_TypeInfo_conf_b = 0;
        EnableAllInterrupts();
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : ASR_vString                                                  */
/** 
    \brief      Check the incomming string and remove not supported blanks
                between to charcters. Fill not used bytes with zeros.
          
    \param      *data
                Uint8Type 
                array with data 

    \param      length
                Uint8Type 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vString(Uint8Type *data, Uint8Type length)
{
    //static Uint8Type dataRem[24] = {0};
    Uint8Type counterRem=0, i;
    for (i=0; i<length; i++)
    {
        if ((  ('0' <= data[i+1]) && (data[i+1] <= '9')) 
            || (0x00 == data[i+1]))
        {
            if(data[i] != ' ') 
            {
                /* remove blanks between to characters */
                data[counterRem] = data[i];
                counterRem++;
            }
        }
        else
        {
            data[counterRem] = data[i];
            counterRem++;            
        }
    }
    length = counterRem;
    if (length <= 8)
    {
        ASR_vString0(data, 0, length);
        /* fill the not suppoprted bytes with zeros */
        ASR_vString1(zeroString, 0, 8);
        ASR_vString2(zeroString, 0, 8);
        ASR_vString3(zeroString, 0, 8);
    }
    else if (length <= 16)
    {
        ASR_vString0(data, 0, 8);
        ASR_vString1(data, 8, (length - 8));
        /* fill the not suppoprted bytes with zeros */
        ASR_vString2(zeroString, 0, 8);
        ASR_vString3(zeroString, 0, 8);                               
    }
    else if (length <= 24)
    {
        ASR_vString0(data, 0, 8);
        ASR_vString1(data, 8, 8);
        ASR_vString2(data, 16, (length - 16));
        /* fill the not suppoprted bytes with zeros */
        ASR_vString3(zeroString, 0, 8);
    }
    else
    {
        if ((length) > 32)
        {
            (length) = 32;
        }
        ASR_vString0(data, 0, 8);
        ASR_vString1(data, 8, 8);
        ASR_vString2(data, 16, 8);
        ASR_vString3(data, 24, (length - 24));                                
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vString0                                                 */
/** 
    \brief      Fill the incoming bytes with the CAN output buffer 

    \param      data *
                Uint8Type
                array with data

    \param      start
                Uint8Type
                start point to fill the outgoing message 
  
    \param      length
                Uint8Type  
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vString0(const Uint8Type *data, Uint8Type start, Uint8Type length)
{    
    Uint8Type i, j=0;
    for (i=0; i<length; i++)
    { 
        mASR2_String0._c[i] = data[start+i];
        j++;
    }
    if( j < 8)
    {
        for (i=j; i<8; i++)
        {
            mASR2_String0._c[i] = 0x00;
        }
    }
    /* send the ASR2 */
    if(CanTransmit( CanTxmASR2_String0 ) == kCanTxOk)
    {
        DisableAllInterrupts();
        mASR2_String0_conf_b = 0;
        EnableAllInterrupts();
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vString1                                                 */
/** 
    \brief      Fill the incoming bytes with the CAN output buffer  
  
    \param      data *
                Uint8Type
                array with data

    \param      start
                Uint8Type
                start point to fill the outgoing message 
  
    \param      length
                Uint8Type 
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vString1(const Uint8Type *data, Uint8Type start, Uint8Type length)
{    
    Uint8Type i, j=0U;
    for (i=0U; i<length; i++)
    { 
        mASR3_String1._c[i] = data[start+i];
        j++;
    }
    if( j < 8U)
    {
        for (i=j; i<8U; i++)
        {
            mASR3_String1._c[i] = 0x00;
        }
    }
    /* send the ASR3 */
    if(CanTransmit( CanTxmASR3_String1 ) == kCanTxOk)
    {
        DisableAllInterrupts();
        mASR3_String1_conf_b = 0;
        EnableAllInterrupts();
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vString2                                                 */
/** 
    \brief     Fill the incoming bytes with the CAN output buffer   
  
    \param      data *
                Uint8Type
                array with data

    \param      start
                Uint8Type
                start point to fill the outgoing message 
  
    \param      length
                Uint8Type 
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vString2(const Uint8Type *data, Uint8Type start, Uint8Type length)
{    
    Uint8Type i, j=0;
    for (i=0; i<length; i++)
    { 
        mASR4_String2._c[i] = data[start+i];
        j++;
    }
    if( j < 8)
    {
        for (i=j; i<8; i++)
        {
            mASR4_String2._c[i] = 0x00;
        }
    }

    /* send the ASR4 */
    if(CanTransmit( CanTxmASR4_String2 ) == kCanTxOk)
    {
        DisableAllInterrupts();
        mASR4_String2_conf_b = 0;
        EnableAllInterrupts();
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ASR_vString3                                                 */
/** 
    \brief      Fill the incoming bytes with the CAN output buffer   
  
    \param      data *
                Uint8Type
                array with data

    \param      start
                Uint8Type
                start point to fill the outgoing message 
  
    \param      length
                Uint8Type 
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
static void ASR_vString3(const Uint8Type *data, Uint8Type start, Uint8Type length)
{   
    Uint8Type i, j=0U;
    for (i=0U; i<length; i++)
    { 
        mASR5_String3._c[i] = data[start+i];
        j++;
    }
    if( j < 8U)
    {
        for (i=j; i<8U; i++)
        {
            mASR5_String3._c[i] = 0x00;
        }
    }
    /* send the ASR5 */
    if(CanTransmit( CanTxmASR5_String3 ) == kCanTxOk)
    {
        DisableAllInterrupts();
        mASR5_String3_conf_b = 0;
        EnableAllInterrupts();
    }
}



/*----------------------------------------------------------------------------*/
/* Function    : ASR_stSetOsekRelAlarm                                        */
/** 
    \brief      Checks if the specified alarm is currently active, in that case
                the alarm is cancel before set, otherwise it is set directly  
  
    \param      inAlarmID
                Osek alarm id

    \param      inIncrement

    \param      inCycle
                
    \return     StatusType 
*/
/*----------------------------------------------------------------------------*/
static StatusType ASR_stSetOsekRelAlarm(  AlarmType   inAlarmID, 
                                                TickType    inIncrement, 
                                                TickType    inCycle)
{
    StatusType status = E_OK;
    TickType ticksLeft;

    /* first check if the specified alarm is running */
    if( GetAlarm(inAlarmID, &ticksLeft) == E_OK )
    {
        /* the alarm is running, so cancel it */
        (void)CancelAlarm(inAlarmID);
    }
    
    /* now set the alarm */
    status = SetRelAlarm(inAlarmID, inIncrement, inCycle);

    return status;
}

/* End Of File ASR_C1.c */
