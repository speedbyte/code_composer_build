/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:  CSHDL_c1.h
*-----------------------------------------------------------------------------
* Module Name: CSHDL (Central state handler)
*-----------------------------------------------------------------------------
* Description: managing the system states for the TMS470 and Bluetooth module
*-----------------------------------------------------------------------------
* $Date: 2010-09-16 11:06:43 +0200 (Do, 16 Sep 2010) $
* $Rev: 15252 $
* $Author: gross $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CSHDL/trunk/inc/CSHDL_c1.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CSHDL_C1_H
#define _CSHDL_C1_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "CSHDL_CI.h"
#include "CSHDL_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the #define
*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the #define
*/

// Timings are in ms
#define UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF_TIMEOUT                  10000           // disconnect jingle plays about 1,5 seconds
#define PREPARE_CK5050_TIMEOUT                                         (10000 + 100)  // 10 seconds is wort case of Parrot


typedef enum
{
/*0*/  INVALID_SYSTEM_STATE = 0,
/*1*/  KL15_OFF,
/*2*/  UHV_ACTIVE_KL15_OFF,
/*3*/  UHV_ACTIVE_KL15_ON,
/*4*/  UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF,
/*5*/  PREPARE_CK5050_OFF,
/*6*/  WAIT_FOR_SHUTDOWN,
/*7*/  CRADLE_RESTRICTION_ON,
/*8*/  CRADLE_RESTRICTION_OFF,
/*9*/  CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES
}CSHDL_enSystemStates;


typedef enum
{
    IGNITION_OFF_DELAY_NOT_STARTED=0,
    IGNITION_OFF_DELAY_KL15_OFF_BEFORE_CK5050_OFF_RUNNING,
    IGNITION_OFF_DELAY_KL15_OFF_PREPARE_CK5050_SENT,
    IGNITION_OFF_DELAY_END
}CSHDL_enIGNITION_OFF_DELAY_States;



typedef enum
{
    CRADLE_RESTRICTION_INIT=0,
    CRADLE_RESTRICTION_CHECKING_CODING,
    CRADLE_RESTRICTION_CODING_OFF_CHECKING_CODING_CHANGE,
    CRADLE_RESTRICTION_CODING_ON_CHECKING_CRADLE_CONNECTION,
    CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED,
    CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED,
    CRADLE_RESTRICTION_DIAG_SESSION_ACTIVE,
    CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_SHUTDOWN_CK5050,
    CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_WAIT_CK5050_POFF_END,
    CRADLE_RESTRICTION_WAIT_CK5050_RESET_TIME
}CSHDL_enCradleRestriction_Internal_States;

typedef enum
{
    CK5050_AT_COMMUNICATION_NOT_STARTED=0, // Init after Reset of TMS
    CK5050_AT_COMMUNICATION_RUNNING,       // after *PSTU Event
    CK5050_AT_COMMUNICATION_FINISHED       // after *POFF Event
}CSHDL_enCK5050_AT_Communication_States;

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_Manager                                                */
/**
    \brief      Central state handler to manage the system states

    \brief      Description:\n
                function to manage the system status and the system\n
                flow\n

    \param      viod
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CSHDL_manager(void);

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_init                                                   */
/**
    \brief      Central state handler function to initialize the CSHDL variables

    \brief      Description:\n
                function to init the system status and the system variables\n
                flow\n

    \param      void
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CSHDL_init(void);

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_enGetIgnitionOFFDelayState                             */
/**
    \brief      This function get the information of the current CSHDL state.
                
    \param      void

    \return     CSHDL_enIGNITION_OFF_DELAY_States
*/
/*----------------------------------------------------------------------------*/
CSHDL_enIGNITION_OFF_DELAY_States CSHDL_enGetIgnitionOFFDelayState(void);

#endif /* _CSHDL_C1_H */

/* End Of File CSHDL_c1.h */

