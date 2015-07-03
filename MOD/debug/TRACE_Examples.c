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


#include "TRACE_Examples.id"
#include "TRACE.h"


void Trace_Example_Text (void)
{

   DEBUG_TEXT(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace beinhaltet nur Text");

   DEBUG_TEXT(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace beinhaltet nur Text und ist ein ERROR");
   DEBUG_TEXT(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace beinhaltet nur Text und ist eine WARNING");
   DEBUG_TEXT(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace beinhaltet nur Text und ist DEBUG-Trace ");
}


void Trace_Example_Values (void)
{
   Uint32Type u32Value = 1234567;

   Uint16Type u16Value1 = 11111;
   Uint16Type u16Value2 = 22222;

   Uint8Type u8Value1 = 1;
   Uint8Type u8Value2 = 2;
   Uint8Type u8Value3 = 3;
   Uint8Type u8Value4 = 4;

   DEBUG_VALUE1(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace hat einen Paramter %d", u32Value);

   DEBUG_VALUE2(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace hat zwei Paramter %d -> %d", u16Value1, u16Value2);
   DEBUG_VALUE4(TRACE_SWI_DBG, MOD_TRACE, "Dieser Trace hat vier Paramter ADC1 %d ADC2 %d ADC %d ADC %d", u8Value1,u8Value2,u8Value3,u8Value4);

}

/*
DEBUG_STATE_ENUM_NAME='TRACE_EXAMPLE_STATES' DEBUG_STATE_NAME='STATE_INIT' DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='TRACE_EXAMPLE_STATES' DEBUG_STATE_NAME='STATE_ON'   DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='TRACE_EXAMPLE_STATES' DEBUG_STATE_NAME='STATE_OFF'  DEBUG_STATE_NUMBER='2'
*/
typedef enum
{
   STATE_INIT,
   STATE_ON,
   STATE_OFF
}TRACE_EXAMPLE_STATES;


static TRACE_EXAMPLE_STATES TraceExampleOldState;
static TRACE_EXAMPLE_STATES TraceExampleState;

void Trace_Example_Enum (void)
{
    switch (TraceExampleState)
    {
       case STATE_INIT:
          TraceExampleState = STATE_ON;
          TraceExampleOldState = STATE_INIT;
          break;

       case STATE_ON:
          TraceExampleState = STATE_OFF;
          TraceExampleOldState = STATE_ON;
          break;

       case STATE_OFF:
          TraceExampleState = STATE_INIT;
          TraceExampleOldState = STATE_OFF;
          break;

       default:
          TraceExampleState = STATE_INIT;
          TraceExampleOldState = STATE_OFF;
          break;
    }


   DEBUG_STATE1(TRACE_SWI_DBG, MOD_TRACE, "ActualState = $TRACE_EXAMPLE_STATES$ %d ", TraceExampleState);

   DEBUG_STATE2(TRACE_SWI_DBG, MOD_TRACE, "OldState = $TRACE_EXAMPLE_STATES$ %d -> NewState = $TRACE_EXAMPLE_STATES$ %d", TraceExampleOldState, TraceExampleState);
}
