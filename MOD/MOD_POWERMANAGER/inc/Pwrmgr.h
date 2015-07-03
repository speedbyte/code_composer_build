/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     Pwrmgr.h
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2011-05-02 11:13:43 +0200 (Mo, 02 Mai 2011) $
* $Rev: 19426 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POWERMANAGER/trunk/inc/Pwrmgr.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _PWRMGR_H
#define _PWRMGR_H



/*--------------------------------------------------------------------------*/
/* Battery Voltage Defaults                                                 */
/*--------------------------------------------------------------------------*/
/* The voltage format is 8.8 fixed point. Since we are only interested in   */
/* defining steps with a resolution of 0.5 Volts the fraction is either     */
/* 0x00, 0 volts or 0x80, 0.5 volts
 * Formula : See V:\Projektordner\VW\S000153VW_UHV-NAR\UHV_HIGH_VW_2010_KW22\28_HardwareDev\Documentation\UHV-NAR_Voltages_20090128.pdf
 * U_KL30/[V] = 5 / 256 * ADC12 + 0.5 */

#define BATTERY_VOLTAGE_NORMAL_LOW                  		/* 8.5 V                */  \
                        (410)
#define BATTERY_VOLTAGE_NORMAL_LOW_RECOVER          		/* 9.0 V                */  \
                        (434)
#define BATTERY_VOLTAGE_NORMAL_HIGH_RECOVER         		/* 15.0 V               */  \
                        (743)
#define BATTERY_VOLTAGE_NORMAL_HIGH                 		/* 15.5 V               */  \
                        (768)

#define BATTERY_VOLTAGE_EIGENDIAG_LOW                       /* 9.0 V               */  \
                        (434)
/* RT 1221 - Anheben der Spannung auf 9,15V - Schwelle wird angehoben wie von VW im Ticket gefordert */
#define BATTERY_VOLTAGE_EIGENDIAG_LOW_9_15                  /* 9.15 V               */  \
                        (445)
#define BATTERY_VOLTAGE_EIGENDIAG_LOW_RECOVER               /* 10.0 V               */  \
                        (487)
#define BATTERY_VOLTAGE_EIGENDIAG_HIGH_RECOVER				/* 16.5 V               */  \
                        (819)
#define BATTERY_VOLTAGE_EIGENDIAG_HIGH						/* 17.0 V               */  \
                        (845)

/*--------------------------------------------------------------------------*/
/* Typedefs                                                                 */
/*--------------------------------------------------------------------------*/
/**
    \brief      symbolic names for the different Voltage states
*/
typedef enum {
/** \brief device in valid Power state */
    PWMGR_NormalVoltage,
/** \brief device in under Power state */
    PWMGR_NormalLowVoltage,
/** \brief device in valid Power state, comming from under voltage. DTC are blocked (hysteresis) */
    PWMGR_NormalLowRecoverVoltage,
/** \brief device in over Power state */
    PWMGR_NormalHighVoltage,
/** \brief device in valid Power state, comming from over voltage. DTC are blocked (hysteresis) */
    PWMGR_NormalHighRecoverVoltage
}PWMGR_enVoltageStates;

typedef enum
{
    PWRMGR_STATE_NORMAL,
    PWRMGR_STATE_NORMAL_LOW_RECOVER,
    PWRMGR_STATE_HIGH_RECOVER,
    PWRMGR_STATE_NORMAL_HIGH,
    PWRMGR_STATE_NORMAL_LOW,
    PWRMGR_STATE_SHUTDOWN,
    PWRMGR_STATE_IS_SHUTDOWN
} PwrmgrStateType;


/*--------------------------------------------------------------------------*/
/* Method Protoypes                                                         */
/*--------------------------------------------------------------------------*/


SuccessFailType Pwrmgr_Manager(void);
SuccessFailType Pwrmgr_Manager_Handler_For_Rapid_Voltage_Changes(void);


SuccessFailType Pwrmgr_Init(void);
SuccessFailType Pwrmgr_Handler_Init_For_Rapid_Voltage_Changes(void);


PwrmgrStateType Pwrmgr_GetPowerManagerState(void);


FlagType Pwrmgr_GetCurrentScope(void);
FlagType Pwrmgr_GetCurrentScopeEigenDiag(void);


PWMGR_enVoltageStates Pwrmgr_GetVoltageState(void);
Uint16Type Pwrmgr_GetRapidPwrmgrVoltage(void);

#endif
