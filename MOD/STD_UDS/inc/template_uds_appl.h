/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: UDS_APPL.H
 **************************************************************************
 *  PURPOSE:
 *  Header file of example implentations of UDS application
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.05.2005 - VW/Audi conformance, first version 
 *  V_00_01_01 Ki 30.05.2005 - Adjustments due to the comments by Audi 
 *  V_00_01_02 Ki 03.06.2005 - Further adjustments 
 *  V_00_01_03 Ki 07.06.2005 - C++ comments eliminated (MISRA conformance)  
 *  V_00_01_04 Ki 22.07.2005 - Configuration matching demo application  
 *  V_00_01_05 Ki 25.07.2005 - Bug-Fix: Service $22  
 *  V_00_01_06 Ki 09.08.2005 - no change
 *  V_00_01_07 Ki 19.09.2005 - RxTxBuf --> MsgBuf
 *  V_00_01_08 Ki 05.12.2005 - no change
 *  V_01_00_00 Ki 18.01.2006 - no change
 *  V_01_01_00 Ki 09.03.2006 - no change
 *  V_01_02_00 Ki 02.06.2006 - Multi client capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - no change 
 *  V_01_02_03 Ki 02.11.2006 - no change 
 *  V_01_02_04 Ki 19.12.2006 - i2s_types.h included
 *                           - use of i2s types instead of C types
 *  V_01_02_05 Ki 12.01.2007 - no change
 *  V_01_02_06 Ki 22.01.2007 - no change
 *  V_01_02_07 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_08 Ki 30.03.2007 - no change
 *  V_01_02_09 Ki 02.07.2007 - no change 
 *  V_01_02_10 Ki 11.07.2007 - no change 
 *  V_01_02_11 Ki 07.08.2007 - no change 
 *  V_01_02_12 Ki 02.11.2007 - no change
 *  V_01_02_13 Ki 18.06.2008 - new: SDS_SW_TEST (don´t care for user) 
 *  V_01_02_14 Ki 23.07.2008 - no change
 *************************************************************************/

#ifndef _I2S_UDS_APPL_H_
#define _I2S_UDS_APPL_H_

#include "i2s_types.h"

#define UDS_APPL_VERSION                 0x0102
#define UDS_APPL_BUGFIX_VERSION          0x14

/*************************************************************************
 *
 * DEFINES
 *
 *************************************************************************/

/*************************************************************************
 *
 * TYPEDEFS
 *
 *************************************************************************/

/*************************************************************************
 *
 * GLOBAL VARIABLES
 *
 *************************************************************************/

/*************************************************************************
 *
 * PROTOTYPES
 *
 *************************************************************************/
#define BUF_SIZE (word)1024        /* as VW80124 min. size is 255 bytes */
extern byte MsgBuf[BUF_SIZE];
extern byte MsgBuf_ECM[BUF_SIZE];

#define _SDS_SW_TEST

#endif

