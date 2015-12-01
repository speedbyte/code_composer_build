/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
#ifndef _TRACE_CONFIG_GENERATED_H
#define _TRACE_CONFIG_GENERATED_H
#include "TRACE_CONFIG_CHANGEABLE.h"


#if (1 == ADC_SWI_ERR)
    #define ADC_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ADC_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ADC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ADC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ADC_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ADC_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ADC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ADC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ADC_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define ADC_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define ADC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define ADC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ADC_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define ADC_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define ADC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define ADC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ADC_SWI_WNG)
    #define ADC_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ADC_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ADC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ADC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ADC_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ADC_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ADC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ADC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ADC_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define ADC_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define ADC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ADC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ADC_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define ADC_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define ADC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define ADC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ADC_SWI_TRC)
    #define ADC_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ADC_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ADC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ADC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ADC_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ADC_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ADC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ADC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ADC_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define ADC_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define ADC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define ADC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ADC_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define ADC_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define ADC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define ADC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ADC_SWI_DBG)
    #define ADC_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ADC_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ADC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ADC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ADC_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ADC_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ADC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ADC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ADC_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define ADC_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define ADC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ADC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ADC_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define ADC_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define ADC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define ADC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CAN_SWI_ERR)
    #define APP_CAN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CAN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CAN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CAN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CAN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_CAN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_CAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CAN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_CAN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_CAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CAN_SWI_WNG)
    #define APP_CAN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CAN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CAN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CAN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CAN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_CAN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_CAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CAN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_CAN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_CAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CAN_SWI_TRC)
    #define APP_CAN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CAN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CAN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CAN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CAN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_CAN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_CAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CAN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_CAN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_CAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CAN_SWI_DBG)
    #define APP_CAN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CAN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CAN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CAN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CAN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_CAN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_CAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CAN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_CAN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_CAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_COMMON_SWI_ERR)
    #define APP_COMMON_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_COMMON_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_COMMON_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_COMMON_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_COMMON_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_COMMON_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_COMMON_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_COMMON_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_COMMON_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_COMMON_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_COMMON_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_COMMON_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_COMMON_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_COMMON_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_COMMON_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_COMMON_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_COMMON_SWI_WNG)
    #define APP_COMMON_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_COMMON_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_COMMON_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_COMMON_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_COMMON_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_COMMON_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_COMMON_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_COMMON_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_COMMON_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_COMMON_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_COMMON_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_COMMON_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_COMMON_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_COMMON_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_COMMON_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_COMMON_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_COMMON_SWI_TRC)
    #define APP_COMMON_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_COMMON_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_COMMON_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_COMMON_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_COMMON_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_COMMON_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_COMMON_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_COMMON_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_COMMON_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_COMMON_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_COMMON_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_COMMON_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_COMMON_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_COMMON_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_COMMON_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_COMMON_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_COMMON_SWI_DBG)
    #define APP_COMMON_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_COMMON_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_COMMON_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_COMMON_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_COMMON_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_COMMON_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_COMMON_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_COMMON_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_COMMON_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_COMMON_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_COMMON_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_COMMON_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_COMMON_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_COMMON_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_COMMON_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_COMMON_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CSHDL_SWI_ERR)
    #define APP_CSHDL_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CSHDL_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CSHDL_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CSHDL_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CSHDL_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CSHDL_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CSHDL_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CSHDL_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CSHDL_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_CSHDL_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_CSHDL_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CSHDL_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CSHDL_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_CSHDL_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_CSHDL_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CSHDL_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CSHDL_SWI_WNG)
    #define APP_CSHDL_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CSHDL_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CSHDL_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CSHDL_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CSHDL_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CSHDL_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CSHDL_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CSHDL_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CSHDL_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_CSHDL_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_CSHDL_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CSHDL_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CSHDL_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_CSHDL_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_CSHDL_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CSHDL_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CSHDL_SWI_TRC)
    #define APP_CSHDL_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CSHDL_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CSHDL_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CSHDL_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CSHDL_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CSHDL_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CSHDL_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CSHDL_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CSHDL_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_CSHDL_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_CSHDL_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CSHDL_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CSHDL_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_CSHDL_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_CSHDL_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CSHDL_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_CSHDL_SWI_DBG)
    #define APP_CSHDL_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_CSHDL_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_CSHDL_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_CSHDL_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_CSHDL_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_CSHDL_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_CSHDL_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_CSHDL_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_CSHDL_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_CSHDL_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_CSHDL_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_CSHDL_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_CSHDL_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_CSHDL_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_CSHDL_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_CSHDL_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DEVICE_SWI_ERR)
    #define APP_DEVICE_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DEVICE_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DEVICE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DEVICE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DEVICE_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DEVICE_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DEVICE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DEVICE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DEVICE_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_DEVICE_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_DEVICE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DEVICE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DEVICE_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_DEVICE_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_DEVICE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DEVICE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DEVICE_SWI_WNG)
    #define APP_DEVICE_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DEVICE_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DEVICE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DEVICE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DEVICE_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DEVICE_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DEVICE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DEVICE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DEVICE_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_DEVICE_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_DEVICE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DEVICE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DEVICE_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_DEVICE_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_DEVICE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DEVICE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DEVICE_SWI_TRC)
    #define APP_DEVICE_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DEVICE_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DEVICE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DEVICE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DEVICE_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DEVICE_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DEVICE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DEVICE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DEVICE_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_DEVICE_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_DEVICE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DEVICE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DEVICE_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_DEVICE_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_DEVICE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DEVICE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DEVICE_SWI_DBG)
    #define APP_DEVICE_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DEVICE_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DEVICE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DEVICE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DEVICE_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DEVICE_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DEVICE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DEVICE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DEVICE_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_DEVICE_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_DEVICE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DEVICE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DEVICE_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_DEVICE_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_DEVICE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DEVICE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DIAG_SWI_ERR)
    #define APP_DIAG_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DIAG_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DIAG_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DIAG_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DIAG_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DIAG_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DIAG_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DIAG_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DIAG_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_DIAG_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_DIAG_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DIAG_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DIAG_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_DIAG_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_DIAG_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DIAG_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DIAG_SWI_WNG)
    #define APP_DIAG_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DIAG_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DIAG_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DIAG_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DIAG_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DIAG_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DIAG_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DIAG_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DIAG_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_DIAG_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_DIAG_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DIAG_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DIAG_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_DIAG_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_DIAG_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DIAG_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DIAG_SWI_TRC)
    #define APP_DIAG_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DIAG_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DIAG_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DIAG_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DIAG_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DIAG_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DIAG_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DIAG_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DIAG_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_DIAG_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_DIAG_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DIAG_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DIAG_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_DIAG_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_DIAG_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DIAG_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DIAG_SWI_DBG)
    #define APP_DIAG_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DIAG_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DIAG_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DIAG_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DIAG_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DIAG_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DIAG_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DIAG_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DIAG_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_DIAG_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_DIAG_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DIAG_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DIAG_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_DIAG_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_DIAG_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DIAG_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DTC_SWI_ERR)
    #define APP_DTC_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DTC_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DTC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DTC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DTC_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DTC_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DTC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DTC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DTC_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_DTC_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_DTC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DTC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DTC_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_DTC_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_DTC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DTC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DTC_SWI_WNG)
    #define APP_DTC_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DTC_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DTC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DTC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DTC_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DTC_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DTC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DTC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DTC_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_DTC_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_DTC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DTC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DTC_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_DTC_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_DTC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DTC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DTC_SWI_TRC)
    #define APP_DTC_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DTC_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DTC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DTC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DTC_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DTC_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DTC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DTC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DTC_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_DTC_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_DTC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DTC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DTC_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_DTC_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_DTC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DTC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_DTC_SWI_DBG)
    #define APP_DTC_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_DTC_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_DTC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_DTC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_DTC_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_DTC_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_DTC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_DTC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_DTC_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_DTC_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_DTC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_DTC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_DTC_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_DTC_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_DTC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_DTC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_SWI_ERR)
    #define APP_MDI_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_MDI_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_MDI_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_SWI_WNG)
    #define APP_MDI_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_MDI_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_MDI_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_SWI_TRC)
    #define APP_MDI_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_MDI_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_MDI_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_SWI_DBG)
    #define APP_MDI_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_MDI_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_MDI_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_BAP_SWI_ERR)
    #define APP_MDI_BAP_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_BAP_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_BAP_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_BAP_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_BAP_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_MDI_BAP_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_BAP_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_MDI_BAP_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_BAP_SWI_WNG)
    #define APP_MDI_BAP_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_BAP_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_BAP_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_BAP_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_BAP_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_MDI_BAP_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_BAP_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_MDI_BAP_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_BAP_SWI_TRC)
    #define APP_MDI_BAP_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_BAP_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_BAP_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_BAP_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_BAP_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_MDI_BAP_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_BAP_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_MDI_BAP_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_MDI_BAP_SWI_DBG)
    #define APP_MDI_BAP_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_MDI_BAP_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_MDI_BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_MDI_BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_MDI_BAP_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_MDI_BAP_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_MDI_BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_MDI_BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_MDI_BAP_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_MDI_BAP_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_MDI_BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_MDI_BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_MDI_BAP_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_MDI_BAP_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_MDI_BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_MDI_BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_SWI_ERR)
    #define APP_PHONE_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_SWI_WNG)
    #define APP_PHONE_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_SWI_TRC)
    #define APP_PHONE_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_SWI_DBG)
    #define APP_PHONE_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_SWI_ERR)
    #define APP_PB_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_PB_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_PB_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_PB_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_SWI_WNG)
    #define APP_PB_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_PB_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_PB_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_PB_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_SWI_TRC)
    #define APP_PB_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_PB_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_PB_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_PB_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_SWI_DBG)
    #define APP_PB_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_PB_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_PB_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_PB_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_BAP_SWI_ERR)
    #define APP_PB_BAP_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_BAP_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_BAP_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_BAP_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_BAP_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_PB_BAP_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_BAP_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_PB_BAP_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_PB_BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_BAP_SWI_WNG)
    #define APP_PB_BAP_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_BAP_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_BAP_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_BAP_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_BAP_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_PB_BAP_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_BAP_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_PB_BAP_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_PB_BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_BAP_SWI_TRC)
    #define APP_PB_BAP_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_BAP_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_BAP_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_BAP_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_BAP_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_PB_BAP_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_BAP_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_PB_BAP_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_PB_BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PB_BAP_SWI_DBG)
    #define APP_PB_BAP_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PB_BAP_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PB_BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PB_BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PB_BAP_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PB_BAP_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PB_BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PB_BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PB_BAP_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_PB_BAP_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_PB_BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PB_BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PB_BAP_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_PB_BAP_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_PB_BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PB_BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_BAP_SWI_ERR)
    #define APP_PHONE_BAP_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_BAP_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_BAP_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_BAP_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_BAP_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_BAP_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_BAP_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_BAP_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_BAP_SWI_WNG)
    #define APP_PHONE_BAP_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_BAP_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_BAP_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_BAP_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_BAP_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_BAP_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_BAP_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_BAP_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_BAP_SWI_TRC)
    #define APP_PHONE_BAP_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_BAP_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_BAP_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_BAP_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_BAP_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_BAP_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_BAP_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_BAP_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_PHONE_BAP_SWI_DBG)
    #define APP_PHONE_BAP_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_PHONE_BAP_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_PHONE_BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_PHONE_BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_PHONE_BAP_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_PHONE_BAP_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_PHONE_BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_PHONE_BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_PHONE_BAP_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_PHONE_BAP_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_PHONE_BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_PHONE_BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_PHONE_BAP_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_PHONE_BAP_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_PHONE_BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_PHONE_BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SD_SWI_ERR)
    #define APP_SD_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SD_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SD_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SD_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SD_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SD_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SD_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SD_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SD_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_SD_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_SD_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SD_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SD_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_SD_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_SD_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SD_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SD_SWI_WNG)
    #define APP_SD_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SD_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SD_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SD_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SD_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SD_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SD_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SD_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SD_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_SD_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_SD_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SD_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SD_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_SD_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_SD_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SD_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SD_SWI_TRC)
    #define APP_SD_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SD_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SD_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SD_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SD_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SD_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SD_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SD_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SD_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_SD_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_SD_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SD_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SD_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_SD_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_SD_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SD_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SD_SWI_DBG)
    #define APP_SD_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SD_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SD_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SD_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SD_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SD_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SD_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SD_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SD_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_SD_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_SD_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SD_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SD_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_SD_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_SD_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SD_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SMS_SWI_ERR)
    #define APP_SMS_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SMS_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SMS_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SMS_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SMS_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SMS_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SMS_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SMS_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SMS_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define APP_SMS_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define APP_SMS_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SMS_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SMS_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define APP_SMS_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define APP_SMS_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SMS_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SMS_SWI_WNG)
    #define APP_SMS_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SMS_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SMS_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SMS_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SMS_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SMS_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SMS_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SMS_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SMS_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define APP_SMS_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define APP_SMS_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SMS_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SMS_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define APP_SMS_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define APP_SMS_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SMS_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SMS_SWI_TRC)
    #define APP_SMS_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SMS_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SMS_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SMS_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SMS_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SMS_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SMS_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SMS_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SMS_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define APP_SMS_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define APP_SMS_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SMS_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SMS_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define APP_SMS_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define APP_SMS_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SMS_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == APP_SMS_SWI_DBG)
    #define APP_SMS_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define APP_SMS_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define APP_SMS_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define APP_SMS_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define APP_SMS_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define APP_SMS_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define APP_SMS_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define APP_SMS_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define APP_SMS_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define APP_SMS_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define APP_SMS_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define APP_SMS_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define APP_SMS_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define APP_SMS_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define APP_SMS_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define APP_SMS_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ASR_SWI_ERR)
    #define ASR_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ASR_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ASR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ASR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ASR_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ASR_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ASR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ASR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ASR_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define ASR_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define ASR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define ASR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ASR_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define ASR_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define ASR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define ASR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ASR_SWI_WNG)
    #define ASR_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ASR_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ASR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ASR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ASR_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ASR_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ASR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ASR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ASR_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define ASR_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define ASR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ASR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ASR_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define ASR_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define ASR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define ASR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ASR_SWI_TRC)
    #define ASR_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ASR_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ASR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ASR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ASR_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ASR_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ASR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ASR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ASR_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define ASR_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define ASR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define ASR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ASR_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define ASR_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define ASR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define ASR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ASR_SWI_DBG)
    #define ASR_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ASR_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ASR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ASR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ASR_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ASR_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ASR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ASR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ASR_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define ASR_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define ASR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ASR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ASR_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define ASR_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define ASR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define ASR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_CMD_HANDLER_SWI_ERR)
    #define AT_CMD_HANDLER_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_CMD_HANDLER_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_CMD_HANDLER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_CMD_HANDLER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_CMD_HANDLER_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_CMD_HANDLER_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_CMD_HANDLER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_CMD_HANDLER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_CMD_HANDLER_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define AT_CMD_HANDLER_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define AT_CMD_HANDLER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_CMD_HANDLER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_CMD_HANDLER_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define AT_CMD_HANDLER_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define AT_CMD_HANDLER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_CMD_HANDLER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_CMD_HANDLER_SWI_WNG)
    #define AT_CMD_HANDLER_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_CMD_HANDLER_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_CMD_HANDLER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_CMD_HANDLER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_CMD_HANDLER_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_CMD_HANDLER_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_CMD_HANDLER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_CMD_HANDLER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_CMD_HANDLER_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define AT_CMD_HANDLER_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define AT_CMD_HANDLER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_CMD_HANDLER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_CMD_HANDLER_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define AT_CMD_HANDLER_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define AT_CMD_HANDLER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_CMD_HANDLER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_CMD_HANDLER_SWI_TRC)
    #define AT_CMD_HANDLER_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_CMD_HANDLER_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_CMD_HANDLER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_CMD_HANDLER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_CMD_HANDLER_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_CMD_HANDLER_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_CMD_HANDLER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_CMD_HANDLER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_CMD_HANDLER_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define AT_CMD_HANDLER_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define AT_CMD_HANDLER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_CMD_HANDLER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_CMD_HANDLER_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define AT_CMD_HANDLER_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define AT_CMD_HANDLER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_CMD_HANDLER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_CMD_HANDLER_SWI_DBG)
    #define AT_CMD_HANDLER_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_CMD_HANDLER_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_CMD_HANDLER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_CMD_HANDLER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_CMD_HANDLER_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_CMD_HANDLER_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_CMD_HANDLER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_CMD_HANDLER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_CMD_HANDLER_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define AT_CMD_HANDLER_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define AT_CMD_HANDLER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_CMD_HANDLER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_CMD_HANDLER_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define AT_CMD_HANDLER_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define AT_CMD_HANDLER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_CMD_HANDLER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_PARSER_SWI_ERR)
    #define AT_PARSER_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_PARSER_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_PARSER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_PARSER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_PARSER_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_PARSER_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_PARSER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_PARSER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_PARSER_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define AT_PARSER_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define AT_PARSER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_PARSER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_PARSER_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define AT_PARSER_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define AT_PARSER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_PARSER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_PARSER_SWI_WNG)
    #define AT_PARSER_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_PARSER_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_PARSER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_PARSER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_PARSER_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_PARSER_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_PARSER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_PARSER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_PARSER_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define AT_PARSER_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define AT_PARSER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_PARSER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_PARSER_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define AT_PARSER_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define AT_PARSER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_PARSER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_PARSER_SWI_TRC)
    #define AT_PARSER_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_PARSER_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_PARSER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_PARSER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_PARSER_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_PARSER_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_PARSER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_PARSER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_PARSER_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define AT_PARSER_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define AT_PARSER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_PARSER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_PARSER_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define AT_PARSER_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define AT_PARSER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_PARSER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == AT_PARSER_SWI_DBG)
    #define AT_PARSER_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define AT_PARSER_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define AT_PARSER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define AT_PARSER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define AT_PARSER_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define AT_PARSER_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define AT_PARSER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define AT_PARSER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define AT_PARSER_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define AT_PARSER_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define AT_PARSER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define AT_PARSER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define AT_PARSER_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define AT_PARSER_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define AT_PARSER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define AT_PARSER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_SWI_ERR)
    #define BAP_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define BAP_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define BAP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define BAP_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define BAP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_SWI_WNG)
    #define BAP_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define BAP_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define BAP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define BAP_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define BAP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_SWI_TRC)
    #define BAP_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define BAP_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define BAP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define BAP_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define BAP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_SWI_DBG)
    #define BAP_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define BAP_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define BAP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define BAP_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define BAP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_DISPATCHER_SWI_ERR)
    #define BAP_DISPATCHER_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_DISPATCHER_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_DISPATCHER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_DISPATCHER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_DISPATCHER_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_DISPATCHER_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_DISPATCHER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_DISPATCHER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_DISPATCHER_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define BAP_DISPATCHER_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define BAP_DISPATCHER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_DISPATCHER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_DISPATCHER_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define BAP_DISPATCHER_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define BAP_DISPATCHER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_DISPATCHER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_DISPATCHER_SWI_WNG)
    #define BAP_DISPATCHER_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_DISPATCHER_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_DISPATCHER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_DISPATCHER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_DISPATCHER_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_DISPATCHER_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_DISPATCHER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_DISPATCHER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_DISPATCHER_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define BAP_DISPATCHER_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define BAP_DISPATCHER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_DISPATCHER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_DISPATCHER_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define BAP_DISPATCHER_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define BAP_DISPATCHER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_DISPATCHER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_DISPATCHER_SWI_TRC)
    #define BAP_DISPATCHER_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_DISPATCHER_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_DISPATCHER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_DISPATCHER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_DISPATCHER_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_DISPATCHER_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_DISPATCHER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_DISPATCHER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_DISPATCHER_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define BAP_DISPATCHER_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define BAP_DISPATCHER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_DISPATCHER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_DISPATCHER_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define BAP_DISPATCHER_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define BAP_DISPATCHER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_DISPATCHER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == BAP_DISPATCHER_SWI_DBG)
    #define BAP_DISPATCHER_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define BAP_DISPATCHER_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define BAP_DISPATCHER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define BAP_DISPATCHER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define BAP_DISPATCHER_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define BAP_DISPATCHER_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define BAP_DISPATCHER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define BAP_DISPATCHER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define BAP_DISPATCHER_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define BAP_DISPATCHER_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define BAP_DISPATCHER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define BAP_DISPATCHER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define BAP_DISPATCHER_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define BAP_DISPATCHER_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define BAP_DISPATCHER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define BAP_DISPATCHER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_DYN_SWI_ERR)
    #define CAN_DYN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_DYN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_DYN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_DYN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_DYN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_DYN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_DYN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_DYN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_DYN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define CAN_DYN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define CAN_DYN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_DYN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_DYN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define CAN_DYN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define CAN_DYN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_DYN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_DYN_SWI_WNG)
    #define CAN_DYN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_DYN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_DYN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_DYN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_DYN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_DYN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_DYN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_DYN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_DYN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define CAN_DYN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define CAN_DYN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_DYN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_DYN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define CAN_DYN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define CAN_DYN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_DYN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_DYN_SWI_TRC)
    #define CAN_DYN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_DYN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_DYN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_DYN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_DYN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_DYN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_DYN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_DYN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_DYN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define CAN_DYN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define CAN_DYN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_DYN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_DYN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define CAN_DYN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define CAN_DYN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_DYN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_DYN_SWI_DBG)
    #define CAN_DYN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_DYN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_DYN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_DYN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_DYN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_DYN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_DYN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_DYN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_DYN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define CAN_DYN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define CAN_DYN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_DYN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_DYN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define CAN_DYN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define CAN_DYN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_DYN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_ST_SWI_ERR)
    #define CAN_ST_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_ST_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_ST_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_ST_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_ST_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_ST_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_ST_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_ST_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_ST_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define CAN_ST_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define CAN_ST_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_ST_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_ST_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define CAN_ST_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define CAN_ST_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_ST_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_ST_SWI_WNG)
    #define CAN_ST_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_ST_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_ST_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_ST_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_ST_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_ST_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_ST_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_ST_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_ST_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define CAN_ST_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define CAN_ST_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_ST_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_ST_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define CAN_ST_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define CAN_ST_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_ST_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_ST_SWI_TRC)
    #define CAN_ST_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_ST_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_ST_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_ST_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_ST_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_ST_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_ST_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_ST_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_ST_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define CAN_ST_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define CAN_ST_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_ST_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_ST_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define CAN_ST_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define CAN_ST_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_ST_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CAN_ST_SWI_DBG)
    #define CAN_ST_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CAN_ST_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CAN_ST_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CAN_ST_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CAN_ST_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CAN_ST_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CAN_ST_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CAN_ST_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CAN_ST_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define CAN_ST_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define CAN_ST_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CAN_ST_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CAN_ST_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define CAN_ST_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define CAN_ST_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define CAN_ST_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CONTEXT_MAN_SWI_ERR)
    #define CONTEXT_MAN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CONTEXT_MAN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CONTEXT_MAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CONTEXT_MAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CONTEXT_MAN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CONTEXT_MAN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CONTEXT_MAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CONTEXT_MAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CONTEXT_MAN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define CONTEXT_MAN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define CONTEXT_MAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define CONTEXT_MAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CONTEXT_MAN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define CONTEXT_MAN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define CONTEXT_MAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define CONTEXT_MAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CONTEXT_MAN_SWI_WNG)
    #define CONTEXT_MAN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CONTEXT_MAN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CONTEXT_MAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CONTEXT_MAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CONTEXT_MAN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CONTEXT_MAN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CONTEXT_MAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CONTEXT_MAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CONTEXT_MAN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define CONTEXT_MAN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define CONTEXT_MAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CONTEXT_MAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CONTEXT_MAN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define CONTEXT_MAN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define CONTEXT_MAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define CONTEXT_MAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CONTEXT_MAN_SWI_TRC)
    #define CONTEXT_MAN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CONTEXT_MAN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CONTEXT_MAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CONTEXT_MAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CONTEXT_MAN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CONTEXT_MAN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CONTEXT_MAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CONTEXT_MAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CONTEXT_MAN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define CONTEXT_MAN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define CONTEXT_MAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define CONTEXT_MAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CONTEXT_MAN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define CONTEXT_MAN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define CONTEXT_MAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define CONTEXT_MAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CONTEXT_MAN_SWI_DBG)
    #define CONTEXT_MAN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CONTEXT_MAN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CONTEXT_MAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CONTEXT_MAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CONTEXT_MAN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CONTEXT_MAN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CONTEXT_MAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CONTEXT_MAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CONTEXT_MAN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define CONTEXT_MAN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define CONTEXT_MAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CONTEXT_MAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CONTEXT_MAN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define CONTEXT_MAN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define CONTEXT_MAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define CONTEXT_MAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CRC_SWI_ERR)
    #define CRC_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CRC_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CRC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CRC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CRC_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CRC_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CRC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CRC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CRC_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define CRC_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define CRC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define CRC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CRC_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define CRC_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define CRC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define CRC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CRC_SWI_WNG)
    #define CRC_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CRC_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CRC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CRC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CRC_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CRC_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CRC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CRC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CRC_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define CRC_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define CRC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CRC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CRC_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define CRC_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define CRC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define CRC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CRC_SWI_TRC)
    #define CRC_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CRC_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CRC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CRC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CRC_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CRC_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CRC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CRC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CRC_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define CRC_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define CRC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define CRC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CRC_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define CRC_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define CRC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define CRC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CRC_SWI_DBG)
    #define CRC_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CRC_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CRC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CRC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CRC_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CRC_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CRC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CRC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CRC_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define CRC_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define CRC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CRC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CRC_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define CRC_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define CRC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define CRC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CSHDL_SWI_ERR)
    #define CSHDL_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CSHDL_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CSHDL_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CSHDL_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CSHDL_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CSHDL_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CSHDL_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CSHDL_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CSHDL_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define CSHDL_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define CSHDL_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define CSHDL_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CSHDL_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define CSHDL_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define CSHDL_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define CSHDL_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CSHDL_SWI_WNG)
    #define CSHDL_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CSHDL_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CSHDL_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CSHDL_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CSHDL_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CSHDL_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CSHDL_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CSHDL_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CSHDL_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define CSHDL_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define CSHDL_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CSHDL_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CSHDL_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define CSHDL_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define CSHDL_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define CSHDL_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CSHDL_SWI_TRC)
    #define CSHDL_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CSHDL_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CSHDL_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CSHDL_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CSHDL_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CSHDL_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CSHDL_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CSHDL_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CSHDL_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define CSHDL_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define CSHDL_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define CSHDL_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CSHDL_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define CSHDL_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define CSHDL_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define CSHDL_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CSHDL_SWI_DBG)
    #define CSHDL_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CSHDL_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CSHDL_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CSHDL_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CSHDL_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CSHDL_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CSHDL_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CSHDL_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CSHDL_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define CSHDL_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define CSHDL_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CSHDL_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CSHDL_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define CSHDL_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define CSHDL_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define CSHDL_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DEH_SWI_ERR)
    #define DEH_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DEH_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DEH_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DEH_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DEH_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DEH_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DEH_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DEH_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DEH_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define DEH_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define DEH_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define DEH_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DEH_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define DEH_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define DEH_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define DEH_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DEH_SWI_WNG)
    #define DEH_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DEH_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DEH_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DEH_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DEH_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DEH_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DEH_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DEH_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DEH_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define DEH_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define DEH_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define DEH_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DEH_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define DEH_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define DEH_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define DEH_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DEH_SWI_TRC)
    #define DEH_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DEH_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DEH_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DEH_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DEH_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DEH_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DEH_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DEH_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DEH_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define DEH_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define DEH_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define DEH_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DEH_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define DEH_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define DEH_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define DEH_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DEH_SWI_DBG)
    #define DEH_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DEH_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DEH_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DEH_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DEH_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DEH_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DEH_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DEH_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DEH_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define DEH_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define DEH_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define DEH_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DEH_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define DEH_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define DEH_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define DEH_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DIAG_CAN_SWI_ERR)
    #define DIAG_CAN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DIAG_CAN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DIAG_CAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DIAG_CAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DIAG_CAN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DIAG_CAN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DIAG_CAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DIAG_CAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DIAG_CAN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define DIAG_CAN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define DIAG_CAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define DIAG_CAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DIAG_CAN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define DIAG_CAN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define DIAG_CAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define DIAG_CAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DIAG_CAN_SWI_WNG)
    #define DIAG_CAN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DIAG_CAN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DIAG_CAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DIAG_CAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DIAG_CAN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DIAG_CAN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DIAG_CAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DIAG_CAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DIAG_CAN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define DIAG_CAN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define DIAG_CAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define DIAG_CAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DIAG_CAN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define DIAG_CAN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define DIAG_CAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define DIAG_CAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DIAG_CAN_SWI_TRC)
    #define DIAG_CAN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DIAG_CAN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DIAG_CAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DIAG_CAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DIAG_CAN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DIAG_CAN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DIAG_CAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DIAG_CAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DIAG_CAN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define DIAG_CAN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define DIAG_CAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define DIAG_CAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DIAG_CAN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define DIAG_CAN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define DIAG_CAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define DIAG_CAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == DIAG_CAN_SWI_DBG)
    #define DIAG_CAN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define DIAG_CAN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define DIAG_CAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define DIAG_CAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define DIAG_CAN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define DIAG_CAN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define DIAG_CAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define DIAG_CAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define DIAG_CAN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define DIAG_CAN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define DIAG_CAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define DIAG_CAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define DIAG_CAN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define DIAG_CAN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define DIAG_CAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define DIAG_CAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_SWI_ERR)
    #define EEPROM_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define EEPROM_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define EEPROM_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define EEPROM_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_SWI_WNG)
    #define EEPROM_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define EEPROM_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define EEPROM_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define EEPROM_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_SWI_TRC)
    #define EEPROM_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define EEPROM_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define EEPROM_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define EEPROM_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_SWI_DBG)
    #define EEPROM_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define EEPROM_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define EEPROM_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define EEPROM_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_SWI_ERR)
    #define ERM_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define ERM_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define ERM_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define ERM_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define ERM_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_SWI_WNG)
    #define ERM_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define ERM_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define ERM_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define ERM_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define ERM_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_SWI_TRC)
    #define ERM_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define ERM_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define ERM_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define ERM_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define ERM_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_SWI_DBG)
    #define ERM_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define ERM_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define ERM_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define ERM_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define ERM_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == FBL_DATA_SWI_ERR)
    #define FBL_DATA_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define FBL_DATA_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define FBL_DATA_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define FBL_DATA_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define FBL_DATA_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define FBL_DATA_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define FBL_DATA_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define FBL_DATA_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define FBL_DATA_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define FBL_DATA_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define FBL_DATA_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define FBL_DATA_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define FBL_DATA_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define FBL_DATA_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define FBL_DATA_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define FBL_DATA_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == FBL_DATA_SWI_WNG)
    #define FBL_DATA_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define FBL_DATA_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define FBL_DATA_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define FBL_DATA_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define FBL_DATA_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define FBL_DATA_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define FBL_DATA_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define FBL_DATA_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define FBL_DATA_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define FBL_DATA_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define FBL_DATA_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define FBL_DATA_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define FBL_DATA_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define FBL_DATA_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define FBL_DATA_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define FBL_DATA_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == FBL_DATA_SWI_TRC)
    #define FBL_DATA_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define FBL_DATA_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define FBL_DATA_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define FBL_DATA_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define FBL_DATA_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define FBL_DATA_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define FBL_DATA_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define FBL_DATA_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define FBL_DATA_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define FBL_DATA_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define FBL_DATA_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define FBL_DATA_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define FBL_DATA_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define FBL_DATA_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define FBL_DATA_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define FBL_DATA_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == FBL_DATA_SWI_DBG)
    #define FBL_DATA_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define FBL_DATA_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define FBL_DATA_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define FBL_DATA_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define FBL_DATA_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define FBL_DATA_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define FBL_DATA_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define FBL_DATA_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define FBL_DATA_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define FBL_DATA_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define FBL_DATA_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define FBL_DATA_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define FBL_DATA_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define FBL_DATA_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define FBL_DATA_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define FBL_DATA_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == GIO_SWI_ERR)
    #define GIO_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define GIO_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define GIO_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define GIO_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define GIO_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define GIO_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define GIO_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define GIO_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define GIO_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define GIO_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define GIO_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define GIO_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define GIO_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define GIO_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define GIO_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define GIO_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == GIO_SWI_WNG)
    #define GIO_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define GIO_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define GIO_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define GIO_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define GIO_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define GIO_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define GIO_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define GIO_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define GIO_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define GIO_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define GIO_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define GIO_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define GIO_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define GIO_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define GIO_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define GIO_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == GIO_SWI_TRC)
    #define GIO_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define GIO_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define GIO_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define GIO_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define GIO_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define GIO_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define GIO_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define GIO_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define GIO_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define GIO_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define GIO_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define GIO_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define GIO_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define GIO_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define GIO_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define GIO_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == GIO_SWI_DBG)
    #define GIO_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define GIO_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define GIO_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define GIO_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define GIO_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define GIO_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define GIO_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define GIO_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define GIO_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define GIO_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define GIO_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define GIO_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define GIO_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define GIO_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define GIO_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define GIO_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ISOTP_SWI_ERR)
    #define ISOTP_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ISOTP_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ISOTP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ISOTP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ISOTP_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ISOTP_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ISOTP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ISOTP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ISOTP_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define ISOTP_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define ISOTP_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define ISOTP_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ISOTP_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define ISOTP_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define ISOTP_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define ISOTP_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ISOTP_SWI_WNG)
    #define ISOTP_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ISOTP_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ISOTP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ISOTP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ISOTP_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ISOTP_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ISOTP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ISOTP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ISOTP_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define ISOTP_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define ISOTP_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ISOTP_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ISOTP_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define ISOTP_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define ISOTP_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define ISOTP_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ISOTP_SWI_TRC)
    #define ISOTP_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ISOTP_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ISOTP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ISOTP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ISOTP_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ISOTP_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ISOTP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ISOTP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ISOTP_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define ISOTP_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define ISOTP_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define ISOTP_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ISOTP_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define ISOTP_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define ISOTP_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define ISOTP_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ISOTP_SWI_DBG)
    #define ISOTP_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ISOTP_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ISOTP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ISOTP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ISOTP_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ISOTP_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ISOTP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ISOTP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ISOTP_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define ISOTP_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define ISOTP_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ISOTP_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ISOTP_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define ISOTP_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define ISOTP_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define ISOTP_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == KPB_SWI_ERR)
    #define KPB_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define KPB_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define KPB_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define KPB_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define KPB_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define KPB_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define KPB_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define KPB_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define KPB_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define KPB_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define KPB_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define KPB_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define KPB_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define KPB_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define KPB_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define KPB_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == KPB_SWI_WNG)
    #define KPB_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define KPB_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define KPB_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define KPB_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define KPB_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define KPB_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define KPB_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define KPB_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define KPB_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define KPB_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define KPB_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define KPB_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define KPB_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define KPB_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define KPB_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define KPB_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == KPB_SWI_TRC)
    #define KPB_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define KPB_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define KPB_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define KPB_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define KPB_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define KPB_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define KPB_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define KPB_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define KPB_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define KPB_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define KPB_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define KPB_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define KPB_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define KPB_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define KPB_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define KPB_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == KPB_SWI_DBG)
    #define KPB_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define KPB_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define KPB_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define KPB_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define KPB_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define KPB_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define KPB_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define KPB_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define KPB_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define KPB_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define KPB_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define KPB_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define KPB_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define KPB_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define KPB_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define KPB_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LED_SWI_ERR)
    #define LED_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LED_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LED_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LED_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LED_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LED_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LED_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LED_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LED_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define LED_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define LED_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define LED_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LED_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define LED_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define LED_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define LED_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LED_SWI_WNG)
    #define LED_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LED_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LED_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LED_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LED_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LED_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LED_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LED_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LED_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define LED_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define LED_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define LED_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LED_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define LED_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define LED_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define LED_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LED_SWI_TRC)
    #define LED_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LED_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LED_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LED_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LED_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LED_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LED_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LED_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LED_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define LED_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define LED_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define LED_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LED_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define LED_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define LED_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define LED_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LED_SWI_DBG)
    #define LED_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LED_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LED_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LED_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LED_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LED_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LED_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LED_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LED_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define LED_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define LED_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define LED_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LED_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define LED_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define LED_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define LED_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LOGI_BUT_MAN_SWI_ERR)
    #define LOGI_BUT_MAN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LOGI_BUT_MAN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LOGI_BUT_MAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LOGI_BUT_MAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LOGI_BUT_MAN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LOGI_BUT_MAN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LOGI_BUT_MAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LOGI_BUT_MAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LOGI_BUT_MAN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define LOGI_BUT_MAN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define LOGI_BUT_MAN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define LOGI_BUT_MAN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LOGI_BUT_MAN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define LOGI_BUT_MAN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define LOGI_BUT_MAN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define LOGI_BUT_MAN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LOGI_BUT_MAN_SWI_WNG)
    #define LOGI_BUT_MAN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LOGI_BUT_MAN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LOGI_BUT_MAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LOGI_BUT_MAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LOGI_BUT_MAN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LOGI_BUT_MAN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LOGI_BUT_MAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LOGI_BUT_MAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LOGI_BUT_MAN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define LOGI_BUT_MAN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define LOGI_BUT_MAN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define LOGI_BUT_MAN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LOGI_BUT_MAN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define LOGI_BUT_MAN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define LOGI_BUT_MAN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define LOGI_BUT_MAN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LOGI_BUT_MAN_SWI_TRC)
    #define LOGI_BUT_MAN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LOGI_BUT_MAN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LOGI_BUT_MAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LOGI_BUT_MAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LOGI_BUT_MAN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LOGI_BUT_MAN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LOGI_BUT_MAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LOGI_BUT_MAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LOGI_BUT_MAN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define LOGI_BUT_MAN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define LOGI_BUT_MAN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define LOGI_BUT_MAN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LOGI_BUT_MAN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define LOGI_BUT_MAN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define LOGI_BUT_MAN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define LOGI_BUT_MAN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == LOGI_BUT_MAN_SWI_DBG)
    #define LOGI_BUT_MAN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define LOGI_BUT_MAN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define LOGI_BUT_MAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define LOGI_BUT_MAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define LOGI_BUT_MAN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define LOGI_BUT_MAN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define LOGI_BUT_MAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define LOGI_BUT_MAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define LOGI_BUT_MAN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define LOGI_BUT_MAN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define LOGI_BUT_MAN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define LOGI_BUT_MAN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define LOGI_BUT_MAN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define LOGI_BUT_MAN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define LOGI_BUT_MAN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define LOGI_BUT_MAN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MFL_MGR_SWI_ERR)
    #define MFL_MGR_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MFL_MGR_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MFL_MGR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MFL_MGR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MFL_MGR_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MFL_MGR_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MFL_MGR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MFL_MGR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MFL_MGR_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define MFL_MGR_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define MFL_MGR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define MFL_MGR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MFL_MGR_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define MFL_MGR_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define MFL_MGR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define MFL_MGR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MFL_MGR_SWI_WNG)
    #define MFL_MGR_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MFL_MGR_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MFL_MGR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MFL_MGR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MFL_MGR_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MFL_MGR_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MFL_MGR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MFL_MGR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MFL_MGR_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define MFL_MGR_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define MFL_MGR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define MFL_MGR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MFL_MGR_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define MFL_MGR_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define MFL_MGR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define MFL_MGR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MFL_MGR_SWI_TRC)
    #define MFL_MGR_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MFL_MGR_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MFL_MGR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MFL_MGR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MFL_MGR_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MFL_MGR_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MFL_MGR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MFL_MGR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MFL_MGR_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define MFL_MGR_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define MFL_MGR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define MFL_MGR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MFL_MGR_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define MFL_MGR_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define MFL_MGR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define MFL_MGR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MFL_MGR_SWI_DBG)
    #define MFL_MGR_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MFL_MGR_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MFL_MGR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MFL_MGR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MFL_MGR_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MFL_MGR_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MFL_MGR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MFL_MGR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MFL_MGR_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define MFL_MGR_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define MFL_MGR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define MFL_MGR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MFL_MGR_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define MFL_MGR_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define MFL_MGR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define MFL_MGR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MONITOR_ADC_SWI_ERR)
    #define MONITOR_ADC_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MONITOR_ADC_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MONITOR_ADC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MONITOR_ADC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MONITOR_ADC_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MONITOR_ADC_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MONITOR_ADC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MONITOR_ADC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MONITOR_ADC_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define MONITOR_ADC_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define MONITOR_ADC_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define MONITOR_ADC_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MONITOR_ADC_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define MONITOR_ADC_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define MONITOR_ADC_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define MONITOR_ADC_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MONITOR_ADC_SWI_WNG)
    #define MONITOR_ADC_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MONITOR_ADC_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MONITOR_ADC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MONITOR_ADC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MONITOR_ADC_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MONITOR_ADC_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MONITOR_ADC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MONITOR_ADC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MONITOR_ADC_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define MONITOR_ADC_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define MONITOR_ADC_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define MONITOR_ADC_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MONITOR_ADC_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define MONITOR_ADC_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define MONITOR_ADC_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define MONITOR_ADC_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MONITOR_ADC_SWI_TRC)
    #define MONITOR_ADC_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MONITOR_ADC_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MONITOR_ADC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MONITOR_ADC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MONITOR_ADC_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MONITOR_ADC_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MONITOR_ADC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MONITOR_ADC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MONITOR_ADC_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define MONITOR_ADC_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define MONITOR_ADC_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define MONITOR_ADC_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MONITOR_ADC_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define MONITOR_ADC_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define MONITOR_ADC_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define MONITOR_ADC_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MONITOR_ADC_SWI_DBG)
    #define MONITOR_ADC_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MONITOR_ADC_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MONITOR_ADC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MONITOR_ADC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MONITOR_ADC_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MONITOR_ADC_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MONITOR_ADC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MONITOR_ADC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MONITOR_ADC_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define MONITOR_ADC_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define MONITOR_ADC_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define MONITOR_ADC_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MONITOR_ADC_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define MONITOR_ADC_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define MONITOR_ADC_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define MONITOR_ADC_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_JINGLE_SWI_ERR)
    #define ERM_JINGLE_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_JINGLE_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_JINGLE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_JINGLE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_JINGLE_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_JINGLE_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_JINGLE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_JINGLE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_JINGLE_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define ERM_JINGLE_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define ERM_JINGLE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_JINGLE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_JINGLE_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define ERM_JINGLE_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define ERM_JINGLE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_JINGLE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_JINGLE_SWI_WNG)
    #define ERM_JINGLE_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_JINGLE_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_JINGLE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_JINGLE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_JINGLE_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_JINGLE_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_JINGLE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_JINGLE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_JINGLE_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define ERM_JINGLE_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define ERM_JINGLE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_JINGLE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_JINGLE_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define ERM_JINGLE_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define ERM_JINGLE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_JINGLE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_JINGLE_SWI_TRC)
    #define ERM_JINGLE_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_JINGLE_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_JINGLE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_JINGLE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_JINGLE_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_JINGLE_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_JINGLE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_JINGLE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_JINGLE_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define ERM_JINGLE_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define ERM_JINGLE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_JINGLE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_JINGLE_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define ERM_JINGLE_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define ERM_JINGLE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_JINGLE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == ERM_JINGLE_SWI_DBG)
    #define ERM_JINGLE_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define ERM_JINGLE_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define ERM_JINGLE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define ERM_JINGLE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define ERM_JINGLE_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define ERM_JINGLE_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define ERM_JINGLE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define ERM_JINGLE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define ERM_JINGLE_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define ERM_JINGLE_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define ERM_JINGLE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define ERM_JINGLE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define ERM_JINGLE_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define ERM_JINGLE_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define ERM_JINGLE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define ERM_JINGLE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == NM_SWI_ERR)
    #define NM_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define NM_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define NM_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define NM_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define NM_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define NM_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define NM_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define NM_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define NM_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define NM_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define NM_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define NM_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define NM_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define NM_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define NM_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define NM_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == NM_SWI_WNG)
    #define NM_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define NM_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define NM_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define NM_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define NM_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define NM_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define NM_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define NM_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define NM_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define NM_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define NM_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define NM_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define NM_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define NM_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define NM_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define NM_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == NM_SWI_TRC)
    #define NM_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define NM_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define NM_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define NM_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define NM_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define NM_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define NM_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define NM_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define NM_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define NM_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define NM_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define NM_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define NM_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define NM_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define NM_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define NM_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == NM_SWI_DBG)
    #define NM_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define NM_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define NM_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define NM_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define NM_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define NM_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define NM_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define NM_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define NM_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define NM_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define NM_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define NM_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define NM_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define NM_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define NM_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define NM_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_DYN_SWI_ERR)
    #define OSEK_DYN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_DYN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_DYN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_DYN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_DYN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_DYN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_DYN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_DYN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_DYN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define OSEK_DYN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define OSEK_DYN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_DYN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_DYN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define OSEK_DYN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define OSEK_DYN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_DYN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_DYN_SWI_WNG)
    #define OSEK_DYN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_DYN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_DYN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_DYN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_DYN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_DYN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_DYN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_DYN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_DYN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define OSEK_DYN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define OSEK_DYN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_DYN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_DYN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define OSEK_DYN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define OSEK_DYN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_DYN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_DYN_SWI_TRC)
    #define OSEK_DYN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_DYN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_DYN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_DYN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_DYN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_DYN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_DYN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_DYN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_DYN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define OSEK_DYN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define OSEK_DYN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_DYN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_DYN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define OSEK_DYN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define OSEK_DYN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_DYN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_DYN_SWI_DBG)
    #define OSEK_DYN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_DYN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_DYN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_DYN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_DYN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_DYN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_DYN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_DYN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_DYN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define OSEK_DYN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define OSEK_DYN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_DYN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_DYN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define OSEK_DYN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define OSEK_DYN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_DYN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_ST_SWI_ERR)
    #define OSEK_ST_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_ST_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_ST_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_ST_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_ST_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_ST_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_ST_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_ST_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_ST_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define OSEK_ST_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define OSEK_ST_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_ST_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_ST_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define OSEK_ST_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define OSEK_ST_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_ST_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_ST_SWI_WNG)
    #define OSEK_ST_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_ST_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_ST_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_ST_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_ST_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_ST_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_ST_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_ST_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_ST_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define OSEK_ST_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define OSEK_ST_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_ST_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_ST_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define OSEK_ST_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define OSEK_ST_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_ST_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_ST_SWI_TRC)
    #define OSEK_ST_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_ST_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_ST_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_ST_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_ST_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_ST_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_ST_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_ST_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_ST_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define OSEK_ST_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define OSEK_ST_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_ST_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_ST_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define OSEK_ST_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define OSEK_ST_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_ST_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_ST_SWI_DBG)
    #define OSEK_ST_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_ST_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_ST_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_ST_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_ST_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_ST_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_ST_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_ST_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_ST_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define OSEK_ST_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define OSEK_ST_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_ST_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_ST_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define OSEK_ST_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define OSEK_ST_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_ST_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PHY_BUT_DRV_SWI_ERR)
    #define PHY_BUT_DRV_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PHY_BUT_DRV_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PHY_BUT_DRV_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PHY_BUT_DRV_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PHY_BUT_DRV_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PHY_BUT_DRV_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PHY_BUT_DRV_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PHY_BUT_DRV_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PHY_BUT_DRV_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define PHY_BUT_DRV_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define PHY_BUT_DRV_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define PHY_BUT_DRV_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PHY_BUT_DRV_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define PHY_BUT_DRV_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define PHY_BUT_DRV_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define PHY_BUT_DRV_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PHY_BUT_DRV_SWI_WNG)
    #define PHY_BUT_DRV_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PHY_BUT_DRV_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PHY_BUT_DRV_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PHY_BUT_DRV_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PHY_BUT_DRV_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PHY_BUT_DRV_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PHY_BUT_DRV_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PHY_BUT_DRV_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PHY_BUT_DRV_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define PHY_BUT_DRV_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define PHY_BUT_DRV_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define PHY_BUT_DRV_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PHY_BUT_DRV_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define PHY_BUT_DRV_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define PHY_BUT_DRV_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define PHY_BUT_DRV_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PHY_BUT_DRV_SWI_TRC)
    #define PHY_BUT_DRV_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PHY_BUT_DRV_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PHY_BUT_DRV_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PHY_BUT_DRV_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PHY_BUT_DRV_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PHY_BUT_DRV_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PHY_BUT_DRV_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PHY_BUT_DRV_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PHY_BUT_DRV_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define PHY_BUT_DRV_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define PHY_BUT_DRV_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define PHY_BUT_DRV_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PHY_BUT_DRV_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define PHY_BUT_DRV_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define PHY_BUT_DRV_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define PHY_BUT_DRV_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PHY_BUT_DRV_SWI_DBG)
    #define PHY_BUT_DRV_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PHY_BUT_DRV_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PHY_BUT_DRV_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PHY_BUT_DRV_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PHY_BUT_DRV_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PHY_BUT_DRV_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PHY_BUT_DRV_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PHY_BUT_DRV_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PHY_BUT_DRV_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define PHY_BUT_DRV_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define PHY_BUT_DRV_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define PHY_BUT_DRV_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PHY_BUT_DRV_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define PHY_BUT_DRV_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define PHY_BUT_DRV_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define PHY_BUT_DRV_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POOL_SWI_ERR)
    #define POOL_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POOL_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POOL_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POOL_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POOL_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POOL_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POOL_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POOL_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POOL_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define POOL_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define POOL_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define POOL_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POOL_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define POOL_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define POOL_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define POOL_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POOL_SWI_WNG)
    #define POOL_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POOL_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POOL_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POOL_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POOL_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POOL_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POOL_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POOL_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POOL_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define POOL_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define POOL_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define POOL_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POOL_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define POOL_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define POOL_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define POOL_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POOL_SWI_TRC)
    #define POOL_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POOL_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POOL_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POOL_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POOL_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POOL_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POOL_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POOL_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POOL_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define POOL_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define POOL_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define POOL_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POOL_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define POOL_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define POOL_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define POOL_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POOL_SWI_DBG)
    #define POOL_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POOL_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POOL_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POOL_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POOL_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POOL_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POOL_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POOL_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POOL_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define POOL_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define POOL_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define POOL_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POOL_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define POOL_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define POOL_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define POOL_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PORT_SWI_ERR)
    #define PORT_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PORT_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PORT_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PORT_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PORT_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PORT_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PORT_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PORT_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PORT_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define PORT_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define PORT_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define PORT_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PORT_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define PORT_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define PORT_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define PORT_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PORT_SWI_WNG)
    #define PORT_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PORT_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PORT_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PORT_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PORT_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PORT_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PORT_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PORT_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PORT_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define PORT_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define PORT_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define PORT_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PORT_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define PORT_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define PORT_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define PORT_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PORT_SWI_TRC)
    #define PORT_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PORT_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PORT_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PORT_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PORT_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PORT_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PORT_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PORT_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PORT_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define PORT_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define PORT_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define PORT_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PORT_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define PORT_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define PORT_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define PORT_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == PORT_SWI_DBG)
    #define PORT_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define PORT_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define PORT_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define PORT_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define PORT_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define PORT_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define PORT_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define PORT_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define PORT_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define PORT_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define PORT_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define PORT_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define PORT_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define PORT_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define PORT_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define PORT_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POWERMANAGER_SWI_ERR)
    #define POWERMANAGER_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POWERMANAGER_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POWERMANAGER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POWERMANAGER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POWERMANAGER_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POWERMANAGER_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POWERMANAGER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POWERMANAGER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POWERMANAGER_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define POWERMANAGER_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define POWERMANAGER_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define POWERMANAGER_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POWERMANAGER_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define POWERMANAGER_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define POWERMANAGER_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define POWERMANAGER_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POWERMANAGER_SWI_WNG)
    #define POWERMANAGER_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POWERMANAGER_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POWERMANAGER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POWERMANAGER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POWERMANAGER_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POWERMANAGER_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POWERMANAGER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POWERMANAGER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POWERMANAGER_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define POWERMANAGER_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define POWERMANAGER_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define POWERMANAGER_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POWERMANAGER_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define POWERMANAGER_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define POWERMANAGER_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define POWERMANAGER_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POWERMANAGER_SWI_TRC)
    #define POWERMANAGER_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POWERMANAGER_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POWERMANAGER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POWERMANAGER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POWERMANAGER_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POWERMANAGER_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POWERMANAGER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POWERMANAGER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POWERMANAGER_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define POWERMANAGER_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define POWERMANAGER_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define POWERMANAGER_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POWERMANAGER_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define POWERMANAGER_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define POWERMANAGER_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define POWERMANAGER_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == POWERMANAGER_SWI_DBG)
    #define POWERMANAGER_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define POWERMANAGER_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define POWERMANAGER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define POWERMANAGER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define POWERMANAGER_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define POWERMANAGER_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define POWERMANAGER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define POWERMANAGER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define POWERMANAGER_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define POWERMANAGER_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define POWERMANAGER_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define POWERMANAGER_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define POWERMANAGER_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define POWERMANAGER_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define POWERMANAGER_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define POWERMANAGER_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SDS_UDS_SWI_ERR)
    #define SDS_UDS_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SDS_UDS_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SDS_UDS_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SDS_UDS_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SDS_UDS_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SDS_UDS_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SDS_UDS_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SDS_UDS_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SDS_UDS_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define SDS_UDS_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define SDS_UDS_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define SDS_UDS_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SDS_UDS_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define SDS_UDS_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define SDS_UDS_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define SDS_UDS_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SDS_UDS_SWI_WNG)
    #define SDS_UDS_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SDS_UDS_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SDS_UDS_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SDS_UDS_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SDS_UDS_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SDS_UDS_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SDS_UDS_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SDS_UDS_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SDS_UDS_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define SDS_UDS_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define SDS_UDS_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SDS_UDS_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SDS_UDS_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define SDS_UDS_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define SDS_UDS_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define SDS_UDS_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SDS_UDS_SWI_TRC)
    #define SDS_UDS_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SDS_UDS_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SDS_UDS_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SDS_UDS_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SDS_UDS_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SDS_UDS_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SDS_UDS_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SDS_UDS_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SDS_UDS_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define SDS_UDS_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define SDS_UDS_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define SDS_UDS_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SDS_UDS_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define SDS_UDS_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define SDS_UDS_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define SDS_UDS_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SDS_UDS_SWI_DBG)
    #define SDS_UDS_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SDS_UDS_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SDS_UDS_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SDS_UDS_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SDS_UDS_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SDS_UDS_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SDS_UDS_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SDS_UDS_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SDS_UDS_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define SDS_UDS_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define SDS_UDS_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SDS_UDS_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SDS_UDS_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define SDS_UDS_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define SDS_UDS_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define SDS_UDS_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPEED_MGR_SWI_ERR)
    #define SPEED_MGR_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPEED_MGR_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPEED_MGR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPEED_MGR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPEED_MGR_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPEED_MGR_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPEED_MGR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPEED_MGR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPEED_MGR_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define SPEED_MGR_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define SPEED_MGR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPEED_MGR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPEED_MGR_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define SPEED_MGR_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define SPEED_MGR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define SPEED_MGR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPEED_MGR_SWI_WNG)
    #define SPEED_MGR_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPEED_MGR_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPEED_MGR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPEED_MGR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPEED_MGR_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPEED_MGR_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPEED_MGR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPEED_MGR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPEED_MGR_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define SPEED_MGR_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define SPEED_MGR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPEED_MGR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPEED_MGR_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define SPEED_MGR_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define SPEED_MGR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define SPEED_MGR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPEED_MGR_SWI_TRC)
    #define SPEED_MGR_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPEED_MGR_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPEED_MGR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPEED_MGR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPEED_MGR_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPEED_MGR_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPEED_MGR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPEED_MGR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPEED_MGR_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define SPEED_MGR_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define SPEED_MGR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPEED_MGR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPEED_MGR_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define SPEED_MGR_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define SPEED_MGR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define SPEED_MGR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPEED_MGR_SWI_DBG)
    #define SPEED_MGR_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPEED_MGR_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPEED_MGR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPEED_MGR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPEED_MGR_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPEED_MGR_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPEED_MGR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPEED_MGR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPEED_MGR_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define SPEED_MGR_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define SPEED_MGR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPEED_MGR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPEED_MGR_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define SPEED_MGR_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define SPEED_MGR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define SPEED_MGR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPI_SWI_ERR)
    #define SPI_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPI_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPI_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPI_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPI_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPI_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPI_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPI_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPI_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define SPI_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define SPI_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPI_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPI_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define SPI_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define SPI_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define SPI_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPI_SWI_WNG)
    #define SPI_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPI_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPI_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPI_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPI_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPI_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPI_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPI_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPI_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define SPI_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define SPI_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPI_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPI_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define SPI_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define SPI_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define SPI_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPI_SWI_TRC)
    #define SPI_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPI_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPI_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPI_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPI_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPI_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPI_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPI_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPI_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define SPI_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define SPI_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPI_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPI_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define SPI_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define SPI_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define SPI_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SPI_SWI_DBG)
    #define SPI_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SPI_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SPI_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SPI_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SPI_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SPI_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SPI_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SPI_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SPI_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define SPI_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define SPI_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SPI_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SPI_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define SPI_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define SPI_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define SPI_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SYS_INIT_SWI_ERR)
    #define SYS_INIT_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SYS_INIT_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SYS_INIT_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SYS_INIT_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SYS_INIT_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SYS_INIT_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SYS_INIT_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SYS_INIT_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SYS_INIT_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define SYS_INIT_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define SYS_INIT_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define SYS_INIT_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SYS_INIT_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define SYS_INIT_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define SYS_INIT_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define SYS_INIT_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SYS_INIT_SWI_WNG)
    #define SYS_INIT_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SYS_INIT_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SYS_INIT_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SYS_INIT_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SYS_INIT_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SYS_INIT_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SYS_INIT_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SYS_INIT_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SYS_INIT_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define SYS_INIT_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define SYS_INIT_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SYS_INIT_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SYS_INIT_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define SYS_INIT_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define SYS_INIT_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define SYS_INIT_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SYS_INIT_SWI_TRC)
    #define SYS_INIT_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SYS_INIT_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SYS_INIT_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SYS_INIT_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SYS_INIT_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SYS_INIT_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SYS_INIT_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SYS_INIT_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SYS_INIT_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define SYS_INIT_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define SYS_INIT_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define SYS_INIT_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SYS_INIT_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define SYS_INIT_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define SYS_INIT_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define SYS_INIT_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == SYS_INIT_SWI_DBG)
    #define SYS_INIT_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define SYS_INIT_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define SYS_INIT_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define SYS_INIT_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define SYS_INIT_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define SYS_INIT_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define SYS_INIT_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define SYS_INIT_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define SYS_INIT_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define SYS_INIT_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define SYS_INIT_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define SYS_INIT_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define SYS_INIT_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define SYS_INIT_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define SYS_INIT_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define SYS_INIT_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TEMPERATURE_SWI_ERR)
    #define TEMPERATURE_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TEMPERATURE_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TEMPERATURE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TEMPERATURE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TEMPERATURE_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TEMPERATURE_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TEMPERATURE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TEMPERATURE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TEMPERATURE_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define TEMPERATURE_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define TEMPERATURE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define TEMPERATURE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TEMPERATURE_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define TEMPERATURE_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define TEMPERATURE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define TEMPERATURE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TEMPERATURE_SWI_WNG)
    #define TEMPERATURE_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TEMPERATURE_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TEMPERATURE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TEMPERATURE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TEMPERATURE_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TEMPERATURE_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TEMPERATURE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TEMPERATURE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TEMPERATURE_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define TEMPERATURE_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define TEMPERATURE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define TEMPERATURE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TEMPERATURE_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define TEMPERATURE_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define TEMPERATURE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define TEMPERATURE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TEMPERATURE_SWI_TRC)
    #define TEMPERATURE_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TEMPERATURE_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TEMPERATURE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TEMPERATURE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TEMPERATURE_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TEMPERATURE_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TEMPERATURE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TEMPERATURE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TEMPERATURE_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define TEMPERATURE_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define TEMPERATURE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define TEMPERATURE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TEMPERATURE_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define TEMPERATURE_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define TEMPERATURE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define TEMPERATURE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == TEMPERATURE_SWI_DBG)
    #define TEMPERATURE_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define TEMPERATURE_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define TEMPERATURE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define TEMPERATURE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define TEMPERATURE_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define TEMPERATURE_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define TEMPERATURE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define TEMPERATURE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define TEMPERATURE_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define TEMPERATURE_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define TEMPERATURE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define TEMPERATURE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define TEMPERATURE_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define TEMPERATURE_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define TEMPERATURE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define TEMPERATURE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UART_SWI_ERR)
    #define UART_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UART_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UART_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UART_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UART_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UART_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UART_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UART_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UART_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define UART_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define UART_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define UART_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UART_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define UART_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define UART_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define UART_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UART_SWI_WNG)
    #define UART_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UART_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UART_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UART_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UART_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UART_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UART_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UART_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UART_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define UART_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define UART_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UART_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UART_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define UART_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define UART_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define UART_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UART_SWI_TRC)
    #define UART_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UART_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UART_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UART_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UART_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UART_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UART_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UART_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UART_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define UART_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define UART_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define UART_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UART_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define UART_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define UART_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define UART_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UART_SWI_DBG)
    #define UART_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UART_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UART_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UART_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UART_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UART_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UART_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UART_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UART_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define UART_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define UART_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UART_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UART_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define UART_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define UART_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define UART_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UDS_SWI_ERR)
    #define UDS_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UDS_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UDS_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UDS_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UDS_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UDS_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UDS_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UDS_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UDS_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define UDS_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define UDS_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define UDS_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UDS_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define UDS_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define UDS_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define UDS_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UDS_SWI_WNG)
    #define UDS_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UDS_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UDS_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UDS_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UDS_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UDS_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UDS_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UDS_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UDS_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define UDS_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define UDS_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UDS_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UDS_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define UDS_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define UDS_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define UDS_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UDS_SWI_TRC)
    #define UDS_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UDS_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UDS_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UDS_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UDS_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UDS_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UDS_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UDS_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UDS_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define UDS_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define UDS_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define UDS_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UDS_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define UDS_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define UDS_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define UDS_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UDS_SWI_DBG)
    #define UDS_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UDS_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UDS_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UDS_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UDS_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UDS_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UDS_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UDS_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UDS_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define UDS_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define UDS_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UDS_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UDS_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define UDS_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define UDS_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define UDS_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == VCR_SWI_ERR)
    #define VCR_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define VCR_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define VCR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define VCR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define VCR_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define VCR_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define VCR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define VCR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define VCR_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define VCR_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define VCR_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define VCR_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define VCR_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define VCR_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define VCR_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define VCR_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == VCR_SWI_WNG)
    #define VCR_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define VCR_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define VCR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define VCR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define VCR_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define VCR_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define VCR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define VCR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define VCR_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define VCR_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define VCR_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define VCR_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define VCR_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define VCR_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define VCR_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define VCR_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == VCR_SWI_TRC)
    #define VCR_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define VCR_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define VCR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define VCR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define VCR_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define VCR_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define VCR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define VCR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define VCR_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define VCR_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define VCR_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define VCR_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define VCR_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define VCR_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define VCR_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define VCR_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == VCR_SWI_DBG)
    #define VCR_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define VCR_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define VCR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define VCR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define VCR_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define VCR_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define VCR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define VCR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define VCR_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define VCR_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define VCR_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define VCR_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define VCR_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define VCR_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define VCR_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define VCR_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == WD_SWI_ERR)
    #define WD_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define WD_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define WD_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define WD_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define WD_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define WD_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define WD_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define WD_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define WD_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define WD_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define WD_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define WD_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define WD_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define WD_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define WD_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define WD_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == WD_SWI_WNG)
    #define WD_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define WD_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define WD_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define WD_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define WD_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define WD_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define WD_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define WD_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define WD_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define WD_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define WD_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define WD_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define WD_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define WD_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define WD_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define WD_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == WD_SWI_TRC)
    #define WD_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define WD_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define WD_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define WD_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define WD_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define WD_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define WD_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define WD_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define WD_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define WD_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define WD_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define WD_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define WD_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define WD_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define WD_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define WD_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == WD_SWI_DBG)
    #define WD_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define WD_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define WD_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define WD_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define WD_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define WD_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define WD_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define WD_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define WD_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define WD_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define WD_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define WD_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define WD_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define WD_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define WD_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define WD_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CALLBACK_SWI_ERR)
    #define CALLBACK_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CALLBACK_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CALLBACK_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CALLBACK_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CALLBACK_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CALLBACK_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CALLBACK_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CALLBACK_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CALLBACK_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define CALLBACK_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define CALLBACK_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define CALLBACK_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CALLBACK_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define CALLBACK_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define CALLBACK_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define CALLBACK_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CALLBACK_SWI_WNG)
    #define CALLBACK_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CALLBACK_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CALLBACK_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CALLBACK_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CALLBACK_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CALLBACK_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CALLBACK_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CALLBACK_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CALLBACK_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define CALLBACK_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define CALLBACK_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CALLBACK_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CALLBACK_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define CALLBACK_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define CALLBACK_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define CALLBACK_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CALLBACK_SWI_TRC)
    #define CALLBACK_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CALLBACK_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CALLBACK_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CALLBACK_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CALLBACK_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CALLBACK_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CALLBACK_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CALLBACK_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CALLBACK_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define CALLBACK_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define CALLBACK_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define CALLBACK_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CALLBACK_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define CALLBACK_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define CALLBACK_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define CALLBACK_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == CALLBACK_SWI_DBG)
    #define CALLBACK_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define CALLBACK_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define CALLBACK_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define CALLBACK_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define CALLBACK_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define CALLBACK_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define CALLBACK_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define CALLBACK_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define CALLBACK_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define CALLBACK_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define CALLBACK_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define CALLBACK_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define CALLBACK_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define CALLBACK_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define CALLBACK_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define CALLBACK_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_INTERFACE_SWI_ERR)
    #define OSEK_INTERFACE_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_INTERFACE_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_INTERFACE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_INTERFACE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_INTERFACE_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_INTERFACE_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_INTERFACE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_INTERFACE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_INTERFACE_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define OSEK_INTERFACE_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define OSEK_INTERFACE_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_INTERFACE_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_INTERFACE_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define OSEK_INTERFACE_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define OSEK_INTERFACE_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_INTERFACE_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_INTERFACE_SWI_WNG)
    #define OSEK_INTERFACE_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_INTERFACE_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_INTERFACE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_INTERFACE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_INTERFACE_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_INTERFACE_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_INTERFACE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_INTERFACE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_INTERFACE_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define OSEK_INTERFACE_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define OSEK_INTERFACE_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_INTERFACE_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_INTERFACE_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define OSEK_INTERFACE_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define OSEK_INTERFACE_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_INTERFACE_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_INTERFACE_SWI_TRC)
    #define OSEK_INTERFACE_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_INTERFACE_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_INTERFACE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_INTERFACE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_INTERFACE_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_INTERFACE_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_INTERFACE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_INTERFACE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_INTERFACE_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define OSEK_INTERFACE_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define OSEK_INTERFACE_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_INTERFACE_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_INTERFACE_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define OSEK_INTERFACE_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define OSEK_INTERFACE_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_INTERFACE_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == OSEK_INTERFACE_SWI_DBG)
    #define OSEK_INTERFACE_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define OSEK_INTERFACE_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define OSEK_INTERFACE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define OSEK_INTERFACE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define OSEK_INTERFACE_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define OSEK_INTERFACE_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define OSEK_INTERFACE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define OSEK_INTERFACE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define OSEK_INTERFACE_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define OSEK_INTERFACE_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define OSEK_INTERFACE_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define OSEK_INTERFACE_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define OSEK_INTERFACE_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define OSEK_INTERFACE_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define OSEK_INTERFACE_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define OSEK_INTERFACE_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MAIN_SWI_ERR)
    #define MAIN_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MAIN_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MAIN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MAIN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MAIN_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MAIN_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MAIN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MAIN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MAIN_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define MAIN_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define MAIN_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define MAIN_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MAIN_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define MAIN_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define MAIN_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define MAIN_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MAIN_SWI_WNG)
    #define MAIN_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MAIN_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MAIN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MAIN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MAIN_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MAIN_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MAIN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MAIN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MAIN_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define MAIN_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define MAIN_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define MAIN_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MAIN_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define MAIN_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define MAIN_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define MAIN_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MAIN_SWI_TRC)
    #define MAIN_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MAIN_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MAIN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MAIN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MAIN_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MAIN_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MAIN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MAIN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MAIN_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define MAIN_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define MAIN_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define MAIN_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MAIN_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define MAIN_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define MAIN_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define MAIN_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == MAIN_SWI_DBG)
    #define MAIN_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define MAIN_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define MAIN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define MAIN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define MAIN_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define MAIN_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define MAIN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define MAIN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define MAIN_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define MAIN_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define MAIN_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define MAIN_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define MAIN_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define MAIN_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define MAIN_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define MAIN_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
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
#if (1 == EEPROM_LAYOUT_SWI_ERR)
    #define EEPROM_LAYOUT_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_LAYOUT_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_LAYOUT_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_LAYOUT_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_LAYOUT_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_LAYOUT_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_LAYOUT_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_LAYOUT_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_LAYOUT_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define EEPROM_LAYOUT_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_LAYOUT_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_LAYOUT_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_LAYOUT_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define EEPROM_LAYOUT_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define EEPROM_LAYOUT_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_LAYOUT_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_LAYOUT_SWI_WNG)
    #define EEPROM_LAYOUT_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_LAYOUT_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_LAYOUT_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_LAYOUT_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_LAYOUT_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_LAYOUT_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_LAYOUT_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_LAYOUT_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_LAYOUT_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define EEPROM_LAYOUT_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_LAYOUT_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_LAYOUT_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_LAYOUT_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define EEPROM_LAYOUT_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define EEPROM_LAYOUT_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_LAYOUT_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_LAYOUT_SWI_TRC)
    #define EEPROM_LAYOUT_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_LAYOUT_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_LAYOUT_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_LAYOUT_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_LAYOUT_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_LAYOUT_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_LAYOUT_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_LAYOUT_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_LAYOUT_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define EEPROM_LAYOUT_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_LAYOUT_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_LAYOUT_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_LAYOUT_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define EEPROM_LAYOUT_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define EEPROM_LAYOUT_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_LAYOUT_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == EEPROM_LAYOUT_SWI_DBG)
    #define EEPROM_LAYOUT_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define EEPROM_LAYOUT_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define EEPROM_LAYOUT_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define EEPROM_LAYOUT_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define EEPROM_LAYOUT_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define EEPROM_LAYOUT_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define EEPROM_LAYOUT_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define EEPROM_LAYOUT_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define EEPROM_LAYOUT_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define EEPROM_LAYOUT_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define EEPROM_LAYOUT_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define EEPROM_LAYOUT_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define EEPROM_LAYOUT_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define EEPROM_LAYOUT_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define EEPROM_LAYOUT_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define EEPROM_LAYOUT_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == STD_DEH_SWI_ERR)
    #define STD_DEH_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define STD_DEH_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define STD_DEH_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define STD_DEH_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define STD_DEH_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define STD_DEH_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define STD_DEH_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define STD_DEH_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define STD_DEH_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define STD_DEH_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define STD_DEH_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define STD_DEH_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define STD_DEH_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define STD_DEH_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define STD_DEH_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define STD_DEH_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == STD_DEH_SWI_WNG)
    #define STD_DEH_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define STD_DEH_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define STD_DEH_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define STD_DEH_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define STD_DEH_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define STD_DEH_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define STD_DEH_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define STD_DEH_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define STD_DEH_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define STD_DEH_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define STD_DEH_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define STD_DEH_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define STD_DEH_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define STD_DEH_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define STD_DEH_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define STD_DEH_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == STD_DEH_SWI_TRC)
    #define STD_DEH_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define STD_DEH_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define STD_DEH_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define STD_DEH_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define STD_DEH_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define STD_DEH_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define STD_DEH_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define STD_DEH_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define STD_DEH_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define STD_DEH_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define STD_DEH_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define STD_DEH_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define STD_DEH_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define STD_DEH_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define STD_DEH_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define STD_DEH_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == STD_DEH_SWI_DBG)
    #define STD_DEH_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define STD_DEH_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define STD_DEH_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define STD_DEH_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define STD_DEH_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define STD_DEH_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define STD_DEH_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define STD_DEH_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define STD_DEH_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define STD_DEH_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define STD_DEH_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define STD_DEH_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define STD_DEH_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define STD_DEH_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define STD_DEH_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define STD_DEH_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED60_SWI_ERR)
    #define UNUSED60_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED60_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED60_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED60_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED60_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED60_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED60_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED60_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED60_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define UNUSED60_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define UNUSED60_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED60_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED60_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define UNUSED60_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define UNUSED60_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED60_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED60_SWI_WNG)
    #define UNUSED60_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED60_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED60_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED60_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED60_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED60_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED60_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED60_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED60_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define UNUSED60_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define UNUSED60_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED60_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED60_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define UNUSED60_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define UNUSED60_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED60_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED60_SWI_TRC)
    #define UNUSED60_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED60_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED60_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED60_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED60_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED60_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED60_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED60_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED60_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define UNUSED60_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define UNUSED60_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED60_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED60_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define UNUSED60_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define UNUSED60_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED60_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED60_SWI_DBG)
    #define UNUSED60_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED60_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED60_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED60_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED60_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED60_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED60_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED60_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED60_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define UNUSED60_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define UNUSED60_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED60_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED60_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define UNUSED60_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define UNUSED60_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED60_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED61_SWI_ERR)
    #define UNUSED61_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED61_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED61_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED61_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED61_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED61_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED61_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED61_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED61_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define UNUSED61_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define UNUSED61_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED61_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED61_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define UNUSED61_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define UNUSED61_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED61_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED61_SWI_WNG)
    #define UNUSED61_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED61_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED61_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED61_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED61_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED61_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED61_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED61_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED61_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define UNUSED61_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define UNUSED61_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED61_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED61_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define UNUSED61_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define UNUSED61_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED61_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED61_SWI_TRC)
    #define UNUSED61_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED61_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED61_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED61_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED61_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED61_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED61_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED61_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED61_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define UNUSED61_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define UNUSED61_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED61_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED61_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define UNUSED61_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define UNUSED61_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED61_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED61_SWI_DBG)
    #define UNUSED61_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED61_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED61_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED61_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED61_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED61_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED61_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED61_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED61_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define UNUSED61_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define UNUSED61_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED61_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED61_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define UNUSED61_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define UNUSED61_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED61_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED62_SWI_ERR)
    #define UNUSED62_SWI_ERR_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED62_SWI_ERR_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED62_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED62_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED62_SWI_ERR_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED62_SWI_ERR_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED62_SWI_ERR_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED62_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED62_SWI_ERR_TEXT(moduleID, format)                                         {}
    #define UNUSED62_SWI_ERR_VALUE1(moduleID, format, value)                                {}
    #define UNUSED62_SWI_ERR_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED62_SWI_ERR_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED62_SWI_ERR_STRING(moduleID, format, string)                               {}
    #define UNUSED62_SWI_ERR_STATE1(moduleID, format, state)                                {}
    #define UNUSED62_SWI_ERR_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED62_SWI_ERR_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED62_SWI_WNG)
    #define UNUSED62_SWI_WNG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED62_SWI_WNG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED62_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED62_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED62_SWI_WNG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED62_SWI_WNG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED62_SWI_WNG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED62_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED62_SWI_WNG_TEXT(moduleID, format)                                         {}
    #define UNUSED62_SWI_WNG_VALUE1(moduleID, format, value)                                {}
    #define UNUSED62_SWI_WNG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED62_SWI_WNG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED62_SWI_WNG_STRING(moduleID, format, string)                               {}
    #define UNUSED62_SWI_WNG_STATE1(moduleID, format, state)                                {}
    #define UNUSED62_SWI_WNG_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED62_SWI_WNG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED62_SWI_TRC)
    #define UNUSED62_SWI_TRC_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED62_SWI_TRC_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED62_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED62_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED62_SWI_TRC_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED62_SWI_TRC_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED62_SWI_TRC_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED62_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED62_SWI_TRC_TEXT(moduleID, format)                                         {}
    #define UNUSED62_SWI_TRC_VALUE1(moduleID, format, value)                                {}
    #define UNUSED62_SWI_TRC_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED62_SWI_TRC_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED62_SWI_TRC_STRING(moduleID, format, string)                               {}
    #define UNUSED62_SWI_TRC_STATE1(moduleID, format, state)                                {}
    #define UNUSED62_SWI_TRC_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED62_SWI_TRC_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#if (1 == UNUSED62_SWI_DBG)
    #define UNUSED62_SWI_DBG_TEXT(moduleID, format)                                         LOG_TEXT(moduleID, format)
    #define UNUSED62_SWI_DBG_VALUE1(moduleID, format, value)                                LOG_VALUE1(moduleID, format, value)
    #define UNUSED62_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       LOG_VALUE2(moduleID, format, value1, value2)
    #define UNUSED62_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       LOG_VALUE4(moduleID, format, value1, value2, value3, value4)
    #define UNUSED62_SWI_DBG_STRING(moduleID, format, string)                               LOG_STRING(moduleID, format, string)
    #define UNUSED62_SWI_DBG_STATE1(moduleID, format, state)                                LOG_STATE1(moduleID, format, state)
    #define UNUSED62_SWI_DBG_STATE2(moduleID, format, state1, state2)                       LOG_STATE2(moduleID, format, state1, state2)
    #define UNUSED62_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       LOG_STATE4(moduleID, format, state1, state2, state3, state4)
#else
    #define UNUSED62_SWI_DBG_TEXT(moduleID, format)                                         {}
    #define UNUSED62_SWI_DBG_VALUE1(moduleID, format, value)                                {}
    #define UNUSED62_SWI_DBG_VALUE2(moduleID, format, value1, value2)                       {}
    #define UNUSED62_SWI_DBG_VALUE4(moduleID, format, value1, value2, value3, value4)       {}
    #define UNUSED62_SWI_DBG_STRING(moduleID, format, string)                               {}
    #define UNUSED62_SWI_DBG_STATE1(moduleID, format, state)                                {}
    #define UNUSED62_SWI_DBG_STATE2(moduleID, format, state1, state2)                       {}
    #define UNUSED62_SWI_DBG_STATE4(moduleID, format, state1, state2, state3, state4)       {}
#endif
#endif
