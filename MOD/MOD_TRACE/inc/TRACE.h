/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

#ifndef _TRACE_H
#define _TRACE_H



  extern void Trace_vInit(void);
  extern void Trace_PrintStatistic (void);


  extern void Debug_Text   (Uint32Type TraceID);
  extern void Debug_Value1 (Uint32Type TraceID, Uint32Type Value1);
  extern void Debug_Value2 (Uint32Type TraceID, Uint16Type Value1, Uint16Type Value2);
  extern void Debug_Value4 (Uint32Type TraceID, Uint8Type Value1, Uint8Type Value2, Uint8Type Value3, Uint8Type Value4);
  extern void Debug_String (Uint32Type TraceID, const char * str);
  extern void Debug_State1 (Uint32Type TraceID, Uint32Type State1);
  extern void Debug_State2 (Uint32Type TraceID, Uint16Type State1, Uint16Type State2);


  /*ModuleName is e.g. MOD_TRACE*/

  /* Debug macro to trace a simple textmessage. */
  #define DEBUG_TEXT(Switch_key, ModuleName, Text)                        \
               Switch_key##_TEXT(ModuleName, Text)

  /* Debug macro to trace a single 32bit-value. */
  #define DEBUG_VALUE1(Switch_key, ModuleName, FormatText, Value1)         \
               Switch_key##_VALUE1(ModuleName, FormatText, Value1)

  /* Debug macro to trace two 16bit-values. */
  #define DEBUG_VALUE2(Switch_key, ModuleName, FormatText, Value1, Value2)  \
               Switch_key##_VALUE2(ModuleName, FormatText, Value1, Value2)

  /* Debug macro to trace four 8bit-values. */
  #define DEBUG_VALUE4(Switch_key, ModuleName, FormatText, Value1, Value2, Value3, Value4) \
               Switch_key##_VALUE4(ModuleName, FormatText, Value1, Value2, Value3, Value4)

  /*  */
  #define DEBUG_STRING(Switch_key, ModuleName, FormatText, String)          \
               Switch_key##_STRING(ModuleName, FormatText, String)


  /* Debug macro to trace a single 32bit-state-value. */
  #define DEBUG_STATE1(Switch_key, ModuleName, FormatText, State1)          \
               Switch_key##_STATE1(ModuleName, FormatText, State1)


  #define DEBUG_STATE2(Switch_key, ModuleName, FormatText, State1, State2)  \
               Switch_key##_STATE2(ModuleName, FormatText, State1, State2)


#if defined(USE_DEBUGGING_WITH_TRACE_ID)
  #define Debug_Text_(file_id,line)                                           Debug_Text(file_id+line)
  #define Debug_Value1_(file_id,line, Value1)                                 Debug_Value1(file_id+line, Value1)
  #define Debug_Value2_(file_id,line, Value1, Value2)                         Debug_Value2(file_id+line, Value1, Value2)
  #define Debug_Value4_(file_id,line, Value1, Value2, Value3, Value4)         Debug_Value4(file_id+line, Value1, Value2, Value3, Value4)
  #define Debug_String_(file_id,line, String)                                 Debug_String(file_id+line, String)
  #define Debug_State1_(file_id,line, State1)                                 Debug_State1(file_id+line, State1)
  #define Debug_State2_(file_id,line, State1, State2)                         Debug_State2(file_id+line, State1, State2)


  #define LOG_TEXT(ModuleName, Text)                                          { Debug_Text_  (__C_FILE_ID, __LINE__); }
  #define LOG_VALUE1(ModuleName, FormatText, Value1)                          { Debug_Value1_ (__C_FILE_ID, __LINE__, (Value1)); }
  #define LOG_VALUE2(ModuleName, FormatText, Value1, Value2)                  { Debug_Value2_ (__C_FILE_ID, __LINE__, (Value1) , (Value2)); }
  #define LOG_VALUE4(ModuleName, FormatText, Value1, Value2, Value3, Value4)  { Debug_Value4_ (__C_FILE_ID, __LINE__, (Value1) , (Value2) , (Value3) , (Value4)); }
  #define LOG_STRING(ModuelName, Format,     String)                          { Debug_String_ (__C_FILE_ID, __LINE__, (String)); }
  #define LOG_STATE1(ModuleName, FormatText, State1)                          { Debug_State1_ (__C_FILE_ID, __LINE__, (State1)); }
  #define LOG_STATE2(ModuleName, FormatText, State1, State2)                  { Debug_State2_ (__C_FILE_ID, __LINE__, (State1) , (State2)); }

  #include <TRACE_CONFIG.h>


#else  // USE_DEBUGGING_WITH_TRACE_ID

  /* empty functions if debugging is disabled */
  #define Trace_vInit()                                                      {}

  #define LOG_TEXT(ModuleID, Text)                                           {}
  #define LOG_STRING(ModuleID, FormatText, DataString)                       {}
  #define LOG_VALUE1(ModuleID, FormatText, Value1)                           {}
  #define LOG_VALUE2(ModuleID, FormatText, Value1, Value2)                   {}
  #define LOG_VALUE4(ModuleID, FormatText, Value1, Value2, Value3, Value4)   {}
  #define LOG_STATE1(ModuleID, FormatText, State1)                           {}
  #define LOG_VALUE2(ModuleID, FormatText, State1, State2)                   {}


  #include <TRACE_CONFIG.h>


#endif

#endif /* _TRACE_H */

/* End Of File TRACE.h */
