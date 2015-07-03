/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

#ifndef _TRACE_FORMAT_H
#define _TRACE_FORMAT_H


/*----------------------------------------------------------------------------*/
/* TX-Protokoll:  µC -> PC                                                    */
/*----------------------------------------------------------------------------*/
#define  TX_MSG_LENGTH                                   14
#define  TX_MSG_LENGTH_FOR_CRC                           13

#define  TX_MSG_STARTBYTE_POS                             0
#define  TX_MSG_TRACE_ID_POS                              1

#define  TX_MSG_STARTBYTE                              0xEA

#define  TX_MSG_TYPE_ID_TEXT                           0x00
#define  TX_MSG_TYPE_ID_VALUE1                         0x10
#define  TX_MSG_TYPE_ID_VALUE2                         0x20
#define  TX_MSG_TYPE_ID_STRING                         0x30
#define  TX_MSG_TYPE_ID_VALUE4                         0x40
#define  TX_MSG_TYPE_ID_STATE1                         0x50
#define  TX_MSG_TYPE_ID_STATE2                         0x60

#define  TX_MSG_FILE_COUNTER_ID_BITMASK          0x00FF0000
#define  TX_MSG_LINE_COUNTER_ID_BITMASK          0x0000FFFF
#define  TX_MSG_LINE_COUNTER_HB_BITMASK          0x0000FF00
#define  TX_MSG_LINE_COUNTER_LB_BITMASK          0x000000FF

#define  TX_MSG_USER_DATA_8_OF_32_BIT_MASK_4     0xFF000000
#define  TX_MSG_USER_DATA_8_OF_32_BIT_MASK_3     0x00FF0000
#define  TX_MSG_USER_DATA_8_OF_32_BIT_MASK_2     0x0000FF00
#define  TX_MSG_USER_DATA_8_OF_32_BIT_MASK_1     0x000000FF

#define  TX_MSG_USER_DATA_8_OF_16_BIT_MASK_2         0xFF00
#define  TX_MSG_USER_DATA_8_OF_16_BIT_MASK_1         0x00FF

#define  TX_MSG_SYSTEM_TIME_BIT_MASK_4           0xFF000000
#define  TX_MSG_SYSTEM_TIME_BIT_MASK_3           0x00FF0000
#define  TX_MSG_SYSTEM_TIME_BIT_MASK_2           0x0000FF00
#define  TX_MSG_SYSTEM_TIME_BIT_MASK_1           0x000000FF


#endif /* _TRACE_FORMAT_H */

/* End Of File _TRACE_FORMAT_H */
