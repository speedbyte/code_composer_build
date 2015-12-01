/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     MOD_APP_Speed.h
*-----------------------------------------------------------------------------
* Module Name:    MOD_APP_CAN
*-----------------------------------------------------------------------------
* Description:    
*-----------------------------------------------------------------------------
* $Date: 2009-03-17 09:52:46 +0100 (Di, 17 Mrz 2009) $
* $Rev: 5309 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SPEED_MGR/trunk/inc/SpeedMgr.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef MOD_APP_SPEED_H_
#define MOD_APP_SPEED_H_

/** 
	\brief	6km/h 
*/
#define U16_SPEED_LIMIT_HIGH        700
/** 
	\brief	5km/h 
*/
#define U16_SPEED_LIMIT_LOW         600

/** 
	\brief	0km/h 
*/
#define U16_SPEED_ZERO              0

/*----------------------------------------------------------------------------*/
/* Function    : SPEED_vSpeedHandler                                          */
/** 
    \brief      Analysis the current vehicle speed for phonebook and BT \n
                visibility.
  
    \param      speed
                Uint16Type
                no value limits
                
    \return     void
                value limits
   */
/*----------------------------------------------------------------------------*/
extern void APP_CAN_Speed_vSpeedHandler(Uint16Type speed);

/*----------------------------------------------------------------------------*/
/* Function    : SPEED_un16CurrentVehicleSpeed                                */
/** 
    \brief      Give the current vehicle speed for phonebook and BT \n
                visibility.

    \return     Uint16Type
                no value limits
                
   */
/*----------------------------------------------------------------------------*/
extern Uint16Type APP_CAN_Speed_un16CurrentVehicleSpeed(void);


#endif /*MOD_APP_SPEED_MGR_H_*/
