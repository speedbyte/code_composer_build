/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
#ifndef _TRACE_CONFIG_GENERATED_H
#define _TRACE_CONFIG_GENERATED_H
#if (1 == TRACE_SWI_ERR)
    #define TRACE_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TRACE_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TRACE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TRACE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TRACE_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TRACE_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TRACE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TRACE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TRACE_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define TRACE_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define TRACE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define TRACE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TRACE_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define TRACE_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define TRACE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define TRACE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TRACE_SWI_WNG)
    #define TRACE_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TRACE_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TRACE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TRACE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TRACE_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TRACE_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TRACE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TRACE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TRACE_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define TRACE_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define TRACE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define TRACE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TRACE_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define TRACE_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define TRACE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define TRACE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TRACE_SWI_TRC)
    #define TRACE_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TRACE_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TRACE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TRACE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TRACE_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TRACE_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TRACE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TRACE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TRACE_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define TRACE_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define TRACE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define TRACE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TRACE_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define TRACE_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define TRACE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define TRACE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TRACE_SWI_DBG)
    #define TRACE_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TRACE_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TRACE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TRACE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TRACE_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TRACE_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TRACE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TRACE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TRACE_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define TRACE_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define TRACE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define TRACE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TRACE_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define TRACE_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define TRACE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define TRACE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#endif
