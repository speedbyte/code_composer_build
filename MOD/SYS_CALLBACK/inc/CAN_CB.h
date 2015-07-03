/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     CAN_CB.h
*-----------------------------------------------------------------------------
* Module Name:	  MOD_CAN
*-----------------------------------------------------------------------------
* Description:	  configuration header for the module MOD_CAN
*-----------------------------------------------------------------------------
* $Date: 2009-03-19 13:44:37 +0100 (Do, 19 Mrz 2009) $
* $Rev: 5406 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/inc/CAN_CB.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef CAN_CB_H_
#define CAN_CB_H_

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "CAN_CB_cfg.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface ITRANSCEIVER
*/
#ifdef ITRANSCEIVER /* BEGIN DECLARE INTERFACE(ITRANSCEIVER) */

/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToNormal                                       */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:\n
                set the transceiver in the normal mode \n
				CAN_EN = HIGH;      CAN_STB_ = HIGH; \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToNormal(void);


/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToStandby                                      */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:\n
                set the transceiver in the standby mode \n
				CAN_EN = LOW;      CAN_STB_ = LOW; \n
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToStandby(void);


/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToSleep                                      */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:\n
                set the transceiver in the sleep mode \n
				first:  CAN_EN = HIGH;      CAN_STB_ = LOW; \n
                second: CAN_EN = LOW;       CAN_STB_ = LOW; \n
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToSleep(void);


/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToPowerOn                                      */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:\n
                set the transceiver in the Power on mode \n
				CAN_EN = LOW;       CAN_STB_ = HIGH; \n
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToPowerOn(void);


/*----------------------------------------------------------------------------*/
/* Function    : DisableIrq                                                   */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:\n
                callback function to disable the CAN specific Interrupts \n
                in the Interrupt Request mask register \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void DisableIrq(void);


/*----------------------------------------------------------------------------*/
/* Function    : EnableIrq                                                    */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:\n
                callback function to enable the CAN specific Interrupts \n
                in the Interrupt Request mask register \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void EnableIrq(void);


#endif /* END DECLARE INTERFACE(ITRANSCEIVER) */

#endif /* _CAN_CB_H */

/* End Of File CAN_CB.h */
