/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:  CSHDL_CI.h
*-----------------------------------------------------------------------------
* Module Name: CSHDL (Central state handler)
*-----------------------------------------------------------------------------
* Description: managing the system states for the TMS470 and Bluetooth module
*-----------------------------------------------------------------------------
* $Date: 2010-02-05 15:48:11 +0100 (Fr, 05 Feb 2010) $
* $Rev: 11084 $
* $Author: braun $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CSHDL/trunk/inc/CSHDL_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CSHDL_CI_H
#define _CSHDL_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of configured interface
*/


/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                                          */
/*----------------------------------------------------------------------------*/
/**
\brief	Interface function
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the method
*/


/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the module global variable
*/
typedef union
{
    Uint8Type   systemStates;
    struct
    {
        Uint8Type   s0  :1; /* b0 0:state not triggered   1:state triggered   */
        Uint8Type   s1  :1; /* b1 0:state not triggered   1:state triggered   */
        Uint8Type   s2  :1; /* b2 0:state not triggered   1:state triggered   */
        Uint8Type   s3  :1; /* b3 0:state not triggered   1:state triggered   */
        Uint8Type   s4  :1; /* b4 0:state not triggered   1:state triggered   */
        Uint8Type   s5  :1; /* b5 0:state not triggered   1:state triggered   */
        Uint8Type   s6  :1; /* b6 0:state not triggered   1:state triggered   */
        Uint8Type   s7  :1; /* b7 0:state not triggered   1:state triggered   */
    } state;
} CSHDL_SystemStateWorkFlowType;

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/
typedef enum
{
    CSHDL_Poff = 0,
    CSHDL_SYSinit,
    CSHDL_TMS470ready,
    CSHDL_SYSrun,
    CSHDL_BTSWupdate
} CSHDL_GesSystemStateType;

typedef enum
{
    CSHDL_CANoff = 0,
    CSHDL_CANinit,
    CSHDL_CANready
} CSHDL_CANStateType;

typedef enum
{
    CSHDL_CK5050off = 0,
    CSHDL_CK5050init,
    CSHDL_CK5050ready
} CSHDL_CK5050StateType;


typedef enum
{
    construct=0,
    stat,
    destruct
} CSHDL_SystemInStateType;

#endif /* _CSHDL_CI_H */

/* End Of File CSHDL_CI.h */
