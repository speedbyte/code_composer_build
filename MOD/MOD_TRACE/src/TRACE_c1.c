/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "UART.h"

#include "TRACE_FORMAT.h"

#include "TRACE_c1.id"
#include "TRACE.h"

#if !defined(USE_DEBUGGING_WITH_TRACE_ID)
// This function is required to create the library even if the old trace concept is used.
void dummmy_TRACE(void)
{
	;
}
#endif

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static Uint8Type _Debug_u8CalcChecksum(Uint8Type *buf, Uint8Type len);
static SuccessFailType _Debug_sfIsTracingPossible(void);


/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static Uint8Type Trace_Init_Done = 0;
static Uint8Type Trace_TX_MsgCounter; // used for all msg types
static TickType  Trace_TX_MsgTime;    // used for all msg types


static Uint8Type Trace_TX_MsgBuffer[TX_MSG_LENGTH];

static Uint16Type Trace_DebugMacroCalledCounter;
static Uint16Type Trace_DebugMacroRingBufferSucces;
static Uint16Type Trace_DebugMacroRingBufferFails;

static Uint16Type Trace_TX_BytesWrittenToRingBuffer = 0;


static Uint32Type Trace_TraceID_Fails = 0;


void Trace_vInit(void)
{
   // protokoll
   (void)memset(&Trace_TX_MsgBuffer[0], 0x00, sizeof(Trace_TX_MsgBuffer));
   Trace_TX_MsgCounter = 0;
   Trace_TX_MsgTime = 0;
   Trace_TX_BytesWrittenToRingBuffer = 0;
   Trace_DebugMacroCalledCounter = 1;  // Init with 1 to be synchron with PC-Client (Server does a Pre Increment)

   Trace_DebugMacroRingBufferSucces = 1; // Init with 1 to be synchron with PC-Client (Server does a Pre Increment)
   Trace_DebugMacroRingBufferFails = 0;

   Trace_TraceID_Fails = 0;

   Trace_Init_Done = BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : Trace_PrintStatistic                                        */
/**
    \brief      check internal Variables and print them out
    \param      none

    \return     none
*/
/*----------------------------------------------------------------------------*/

void Trace_PrintStatistic (void)
{
    // check internal Variables
    if (0 == Trace_DebugMacroRingBufferFails)
    {
        DEBUG_VALUE2(TRACE_SWI_DBG, MOD_TRACE, "Trace Messages: %d TxBufferSuccess: %d",
                     Trace_DebugMacroCalledCounter,
                     Trace_DebugMacroRingBufferSucces);
    }
    else
    {
        DEBUG_VALUE2(TRACE_SWI_WNG, MOD_TRACE, "Trace Messages: %d TxBufferFails: %d",
                     Trace_DebugMacroCalledCounter,
                     Trace_DebugMacroRingBufferFails);
    }

    if (Trace_TraceID_Fails > 0)
    {
        DEBUG_VALUE1(TRACE_SWI_WNG, MOD_TRACE, "Tracing of following Trace ID: 0x%X is not possible - please check if called inside a OSEK-Task",Trace_TraceID_Fails);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _Debug_u8CalcChecksum                                        */
/**
    \brief      Calculate a one byte CRC checksum character using XOR

    \param      pointer to a buffer, length of buffer

    \return     calculated CRC according to Spec: Trace_Konpept.doc
   */
/*----------------------------------------------------------------------------*/
static Uint8Type _Debug_u8CalcChecksum(Uint8Type *buf, Uint8Type len)
{
    Uint8Type  i;
    Uint8Type  checksum = 0x00;
    Uint8Type *ptr = (Uint8Type *) buf;

    /* calculate the XOR value for each byte */
    for (i=0; i < len;  i++)
    {
        checksum = checksum ^ ptr[i];
    }
    checksum &= 0x7F;   // setting MSB always to Zero

    return (Uint8Type) (checksum);
}

/*----------------------------------------------------------------------------*/
/* Function    : _Debug_sfIsTracingPossible                                   */
/**
    \brief      Check if tracing is allowed

    \param      none

    \return     SUCCES or FAIL
   */
/*----------------------------------------------------------------------------*/

static SuccessFailType _Debug_sfIsTracingPossible(void)
{
    TaskStateType osHostTaskState;
    TaskStateType osMonitorTaskState;
    TaskStateType osDriverTaskState;
    TaskStateType osAppTaskState;
    TaskStateType osDatabaseState;
    TaskStateType osSystemTaskState;

    // these checks are taken from the the former DEBUG_Printf function
    if (Trace_Init_Done != BT_TRUE)
    {
        // not Init yet => no Tracing
        return (FAIL);
    }

    if (isOsStarted() != BT_TRUE)
    {
        // no OS => no Tracing
        return (FAIL);
    }

    (void)GetTaskState(OsHostTask_10ms, &osHostTaskState);
    (void)GetTaskState(OsMonitorTask_50ms, &osMonitorTaskState);
    (void)GetTaskState(OsDriverTask_1ms, &osDriverTaskState);
    (void)GetTaskState(OsAppTask_event_100ms, &osAppTaskState);
    (void)GetTaskState(OsDatabaseTask_event, &osDatabaseState);
    (void)GetTaskState(OsSystemTask_event, &osSystemTaskState);

    /* Das Ergebnis der Funktionn GetTask koennte bereits bei folgeder Auswertung flalsch sein */
    if ( (RUNNING == osHostTaskState) ||
         (RUNNING == osMonitorTaskState) ||
         (RUNNING == osDriverTaskState) ||
         (RUNNING == osAppTaskState) ||
         (RUNNING == osDatabaseState) ||
         (RUNNING == osSystemTaskState)
       )
       {
            Uint32Type u32currentCpsr = _get_CPSR() ;
            if ((u32currentCpsr & 0x00000080) != 0) // IRQ is active
            {
                return (FAIL);
            }
       }
       else
       {
          // no Task running => no Tracing
          return (FAIL);
       }

    // further checks necessary ?, if not return success
    return(SUCCESS);
}


void Debug_Text (Uint32Type TraceID)
{
    Trace_DebugMacroCalledCounter++;


    if (SUCCESS == _Debug_sfIsTracingPossible ())
    {
        // GetResource
        semP(UART_LockResource);

        // Protokoll
        Trace_TX_MsgBuffer[0] =  TX_MSG_STARTBYTE;
        // increment MsgCounter
        Trace_TX_MsgCounter++;
        Trace_TX_MsgCounter &= 0x0F;
        // increment MsgCounter
        Trace_TX_MsgBuffer[1] =  TX_MSG_TYPE_ID_TEXT;
        Trace_TX_MsgBuffer[1] |= Trace_TX_MsgCounter;
        Trace_TX_MsgBuffer[2] =  (Uint8Type) ((TraceID & TX_MSG_FILE_COUNTER_ID_BITMASK) >> 16);  //file
        Trace_TX_MsgBuffer[3] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_HB_BITMASK) >>  8);  // line
        Trace_TX_MsgBuffer[4] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_LB_BITMASK));  // line
        // fill user data with zeros
        Trace_TX_MsgBuffer[5] =  0;
        Trace_TX_MsgBuffer[6] =  0;
        Trace_TX_MsgBuffer[7] =  0;
        Trace_TX_MsgBuffer[8] =  0;
        // get SystemTime
        Trace_TX_MsgTime = osGetSystemCounter();
        Trace_TX_MsgBuffer[9] =  (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[10] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[11] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[12] = (Uint8Type)(Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_1);

        Trace_TX_MsgBuffer[13] = _Debug_u8CalcChecksum(&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH_FOR_CRC);

        Trace_TX_BytesWrittenToRingBuffer = UART_u16WriteBlock(UART1,&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH);

        if (TX_MSG_LENGTH == Trace_TX_BytesWrittenToRingBuffer)
        {
           // all Bytes accepted
           Trace_DebugMacroRingBufferSucces ++;
        }
        else
        {
           //  Todo
           Trace_DebugMacroRingBufferFails ++;
        }
        // ReleaseResource
        semV(UART_LockResource);
    }
    else
    {
        Trace_TraceID_Fails = TraceID;
    }
}

void Debug_Value1 (Uint32Type TraceID, Uint32Type Value1)
{
    Trace_DebugMacroCalledCounter++;


    if (SUCCESS == _Debug_sfIsTracingPossible ())
    {
        // GetResource
        semP(UART_LockResource);

        // Protokoll
        Trace_TX_MsgBuffer[0] =  TX_MSG_STARTBYTE;
        // increment MsgCounter
        Trace_TX_MsgCounter++;
        Trace_TX_MsgCounter &= 0x0F;
        // increment MsgCounter
        Trace_TX_MsgBuffer[1] =  TX_MSG_TYPE_ID_VALUE1;
        Trace_TX_MsgBuffer[1] |= Trace_TX_MsgCounter;
        Trace_TX_MsgBuffer[2] =  (Uint8Type) ((TraceID & TX_MSG_FILE_COUNTER_ID_BITMASK) >> 16);  //file
        Trace_TX_MsgBuffer[3] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_HB_BITMASK) >>  8);  // line
        Trace_TX_MsgBuffer[4] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_LB_BITMASK));  // line
        // fill user data
        Trace_TX_MsgBuffer[5] =  (Uint8Type)((Value1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[6] =  (Uint8Type)((Value1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[7] =  (Uint8Type)((Value1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[8] =  (Uint8Type)(Value1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_1);
        // get SystemTime
        Trace_TX_MsgTime = osGetSystemCounter();
        Trace_TX_MsgBuffer[9] =  (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[10] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[11] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[12] = (Uint8Type)(Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_1);

        Trace_TX_MsgBuffer[13] = _Debug_u8CalcChecksum(&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH_FOR_CRC);

        Trace_TX_BytesWrittenToRingBuffer = UART_u16WriteBlock(UART1,&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH);

        if (TX_MSG_LENGTH == Trace_TX_BytesWrittenToRingBuffer)
        {
           // all Bytes accepted
           Trace_DebugMacroRingBufferSucces ++;
        }
        else
        {
           //  Todo
           Trace_DebugMacroRingBufferFails ++;
        }
        // ReleaseResource
        semV(UART_LockResource);
    }
    else
    {
        Trace_TraceID_Fails = TraceID;
    }
}



void Debug_Value2 (Uint32Type TraceID, Uint16Type Value1, Uint16Type Value2)
{
    Trace_DebugMacroCalledCounter++;

    if (SUCCESS == _Debug_sfIsTracingPossible ())
    {
        // GetResource
        semP(UART_LockResource);

        // Protokoll
        Trace_TX_MsgBuffer[0] =  TX_MSG_STARTBYTE;
        // increment MsgCounter
        Trace_TX_MsgCounter++;
        Trace_TX_MsgCounter &= 0x0F;
        // increment MsgCounter
        Trace_TX_MsgBuffer[1] =  TX_MSG_TYPE_ID_VALUE2;
        Trace_TX_MsgBuffer[1] |= Trace_TX_MsgCounter;
        Trace_TX_MsgBuffer[2] =  (Uint8Type) ((TraceID & TX_MSG_FILE_COUNTER_ID_BITMASK) >> 16);  //file
        Trace_TX_MsgBuffer[3] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_HB_BITMASK) >>  8);  // line
        Trace_TX_MsgBuffer[4] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_LB_BITMASK));  // line
        // fill user data
        Trace_TX_MsgBuffer[5] =  (Uint8Type)((Value1 & TX_MSG_USER_DATA_8_OF_16_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[6] =  (Uint8Type)((Value1 & TX_MSG_USER_DATA_8_OF_16_BIT_MASK_1));
        Trace_TX_MsgBuffer[7] =  (Uint8Type)((Value2 & TX_MSG_USER_DATA_8_OF_16_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[8] =  (Uint8Type)(Value2 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_1);
        // get SystemTime
        Trace_TX_MsgTime = osGetSystemCounter();
        Trace_TX_MsgBuffer[9] =  (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[10] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[11] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[12] = (Uint8Type)(Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_1);

        Trace_TX_MsgBuffer[13] = _Debug_u8CalcChecksum(&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH_FOR_CRC);

        Trace_TX_BytesWrittenToRingBuffer = UART_u16WriteBlock(UART1,&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH);

        if (TX_MSG_LENGTH == Trace_TX_BytesWrittenToRingBuffer)
        {
           // all Bytes accepted
           Trace_DebugMacroRingBufferSucces ++;
        }
        else
        {
           //  Todo
           Trace_DebugMacroRingBufferFails ++;
        }
        // ReleaseResource
        semV(UART_LockResource);
    }
    else
    {
        Trace_TraceID_Fails = TraceID;
    }
}

void Debug_Value4 (Uint32Type TraceID, Uint8Type Value1, Uint8Type Value2, Uint8Type Value3, Uint8Type Value4)
{
    Trace_DebugMacroCalledCounter++;


    if (SUCCESS == _Debug_sfIsTracingPossible ())
    {
        // GetResource
        semP(UART_LockResource);

        Trace_TX_MsgBuffer[0] =  TX_MSG_STARTBYTE;
        // increment MsgCounter
        Trace_TX_MsgCounter++;
        Trace_TX_MsgCounter &= 0x0F;
        // increment MsgCounter
        Trace_TX_MsgBuffer[1] =  TX_MSG_TYPE_ID_VALUE4;
        Trace_TX_MsgBuffer[1] |= Trace_TX_MsgCounter;
        Trace_TX_MsgBuffer[2] =  (Uint8Type) ((TraceID & TX_MSG_FILE_COUNTER_ID_BITMASK) >> 16);  //file
        Trace_TX_MsgBuffer[3] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_HB_BITMASK) >>  8);  // line
        Trace_TX_MsgBuffer[4] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_LB_BITMASK));  // line
        // fill user data
        Trace_TX_MsgBuffer[5] =  Value1;
        Trace_TX_MsgBuffer[6] =  Value2;
        Trace_TX_MsgBuffer[7] =  Value3;
        Trace_TX_MsgBuffer[8] =  Value4;
        // get SystemTime
        Trace_TX_MsgTime = osGetSystemCounter();
        Trace_TX_MsgBuffer[9] =  (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[10] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[11] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[12] = (Uint8Type)(Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_1);

        Trace_TX_MsgBuffer[13] = _Debug_u8CalcChecksum(&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH_FOR_CRC);

        Trace_TX_BytesWrittenToRingBuffer = UART_u16WriteBlock(UART1,&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH);

        if (TX_MSG_LENGTH == Trace_TX_BytesWrittenToRingBuffer)
        {
           // all Bytes accepted
           Trace_DebugMacroRingBufferSucces ++;
        }
        else
        {
           //  Todo
           Trace_DebugMacroRingBufferFails ++;
        }
        // ReleaseResource
        semV(UART_LockResource);
    }
    else
    {
        Trace_TraceID_Fails = TraceID;
    }
}
void Debug_String (Uint32Type TraceID, const char * str)
{

}


void Debug_State1 (Uint32Type TraceID, Uint32Type State1)
{
    Trace_DebugMacroCalledCounter++;


    if (SUCCESS == _Debug_sfIsTracingPossible ())
    {
        // GetResource
        semP(UART_LockResource);

        // Protokoll
        Trace_TX_MsgBuffer[0] =  TX_MSG_STARTBYTE;
        // increment MsgCounter
        Trace_TX_MsgCounter++;
        Trace_TX_MsgCounter &= 0x0F;
        // increment MsgCounter
        Trace_TX_MsgBuffer[1] =  TX_MSG_TYPE_ID_STATE1;
        Trace_TX_MsgBuffer[1] |= Trace_TX_MsgCounter;
        Trace_TX_MsgBuffer[2] =  (Uint8Type) ((TraceID & TX_MSG_FILE_COUNTER_ID_BITMASK) >> 16);  //file
        Trace_TX_MsgBuffer[3] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_HB_BITMASK) >>  8);  // line
        Trace_TX_MsgBuffer[4] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_LB_BITMASK));  // line
        // fill user data
        Trace_TX_MsgBuffer[5] =  (Uint8Type)((State1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[6] =  (Uint8Type)((State1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[7] =  (Uint8Type)((State1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[8] =  (Uint8Type)(State1 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_1);
        // get SystemTime
        Trace_TX_MsgTime = osGetSystemCounter();
        Trace_TX_MsgBuffer[9] =  (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[10] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[11] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[12] = (Uint8Type)(Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_1);

        Trace_TX_MsgBuffer[13] = _Debug_u8CalcChecksum(&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH_FOR_CRC);

        Trace_TX_BytesWrittenToRingBuffer = UART_u16WriteBlock(UART1,&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH);

        if (TX_MSG_LENGTH == Trace_TX_BytesWrittenToRingBuffer)
        {
           // all Bytes accepted
           Trace_DebugMacroRingBufferSucces ++;
        }
        else
        {
           //  Todo
           Trace_DebugMacroRingBufferFails ++;
        }
        // ReleaseResource
        semV(UART_LockResource);
    }
    else
    {
        Trace_TraceID_Fails = TraceID;
    }
}


void Debug_State2 (Uint32Type TraceID, Uint16Type State1, Uint16Type State2)
{
    Trace_DebugMacroCalledCounter++;

    if (SUCCESS == _Debug_sfIsTracingPossible ())
    {
        // GetResource
        semP(UART_LockResource);

        // Protokoll
        Trace_TX_MsgBuffer[0] =  TX_MSG_STARTBYTE;
        // increment MsgCounter
        Trace_TX_MsgCounter++;
        Trace_TX_MsgCounter &= 0x0F;
        // increment MsgCounter
        Trace_TX_MsgBuffer[1] =  TX_MSG_TYPE_ID_STATE2;
        Trace_TX_MsgBuffer[1] |= Trace_TX_MsgCounter;
        Trace_TX_MsgBuffer[2] =  (Uint8Type) ((TraceID & TX_MSG_FILE_COUNTER_ID_BITMASK) >> 16);  //file
        Trace_TX_MsgBuffer[3] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_HB_BITMASK) >>  8);  // line
        Trace_TX_MsgBuffer[4] =  (Uint8Type) ((TraceID & TX_MSG_LINE_COUNTER_LB_BITMASK));  // line
        // fill user data
        Trace_TX_MsgBuffer[5] =  (Uint8Type)((State1 & TX_MSG_USER_DATA_8_OF_16_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[6] =  (Uint8Type)((State1 & TX_MSG_USER_DATA_8_OF_16_BIT_MASK_1));
        Trace_TX_MsgBuffer[7] =  (Uint8Type)((State2 & TX_MSG_USER_DATA_8_OF_16_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[8] =  (Uint8Type)(State2 & TX_MSG_USER_DATA_8_OF_32_BIT_MASK_1);
        // get SystemTime
        Trace_TX_MsgTime = osGetSystemCounter();
        Trace_TX_MsgBuffer[9] =  (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_4) >> 24);
        Trace_TX_MsgBuffer[10] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_3) >> 16);
        Trace_TX_MsgBuffer[11] = (Uint8Type)((Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_2) >> 8);
        Trace_TX_MsgBuffer[12] = (Uint8Type)(Trace_TX_MsgTime & TX_MSG_SYSTEM_TIME_BIT_MASK_1);

        Trace_TX_MsgBuffer[13] = _Debug_u8CalcChecksum(&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH_FOR_CRC);

        Trace_TX_BytesWrittenToRingBuffer = UART_u16WriteBlock(UART1,&Trace_TX_MsgBuffer[0], TX_MSG_LENGTH);

        if (TX_MSG_LENGTH == Trace_TX_BytesWrittenToRingBuffer)
        {
           // all Bytes accepted
           Trace_DebugMacroRingBufferSucces ++;
        }
        else
        {
           //  Todo
           Trace_DebugMacroRingBufferFails ++;
        }
        // ReleaseResource
        semV(UART_LockResource);
    }
    else
    {
        Trace_TraceID_Fails = TraceID;
    }
}



/* End Of File TRACE_c1.c */
