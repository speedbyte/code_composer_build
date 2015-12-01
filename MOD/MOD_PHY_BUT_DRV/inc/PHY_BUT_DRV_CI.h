/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     PHY_BUT_DRV_CI.h
*-----------------------------------------------------------------------------
* Module Name:    PHY_BUT_DRV
*-----------------------------------------------------------------------------
* Description:    Driver (host task) part of 3 Button module handling
*-----------------------------------------------------------------------------
* $Date: 2009-03-03 19:11:14 +0100 (Di, 03 Mrz 2009) $
* $Rev: 4912 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_PHY_BUT_DRV/trunk/inc/PHY_BUT_DRV_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _PHY_BUT_DRV_CI_H
#define _PHY_BUT_DRV_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
#define PHY_BUT_DRV_CRADLE_INIT
#define PHY_BUT_DRV_3BM_INIT
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/**
   \brief Minimum voltage lavel for detection of button1

   \brief if the voltage is less than this value a low voltage error
          is created for the diagnostic. The upper limit for the voltage
	  band is defined by  PHY_BUT_DRV_MINBUTTON2
*/
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
	#define _PHY_BUT_DRV_CRADLESHUT_MINBUTTON_BOTH_VW (105)
	#define _PHY_BUT_DRV_CRADLESHUT_MAXBUTTON_BOTH_VW (171)

	#define _PHY_BUT_DRV_CRADLESHUT_MINBUTTON1_VW (172)
	#define _PHY_BUT_DRV_CRADLESHUT_MAXBUTTON1_VW (252)

	#define _PHY_BUT_DRV_CRADLESHUT_MINBUTTON2_VW (253)
	#define _PHY_BUT_DRV_CRADLESHUT_MAXBUTTON2_VW (508)

	#define _PHY_BUT_DRV_CRADLE_MINBUTTON_BOTH_VW (105)
	#define _PHY_BUT_DRV_CRADLE_MAXBUTTON_BOTH_VW (158)

	#define _PHY_BUT_DRV_CRADLE_MINBUTTON1_VW (159) 
	#define _PHY_BUT_DRV_CRADLE_MAXBUTTON1_VW (225)

	#define _PHY_BUT_DRV_CRADLE_MINBUTTON2_VW (226)
	#define _PHY_BUT_DRV_CRADLE_MAXBUTTON2_VW (386)

	#define _PHY_BUT_DRV_CRADLE_MINNOTPRESSED_VW (387)
	#define _PHY_BUT_DRV_CRADLE_MAXNOTPRESSED_VW (508)

    #define _PHY_BUT_DRV_BUTTON_MIN_CRADLE_PRESENT_VW    (509)
	#define _PHY_BUT_DRV_BUTTON_MAX_CRADLE_PRESENT_VW    (633)

	#define _PHY_BUT_DRV_BUTTON_MIN_BASEPLATE_PRESENT_VW (634)
	#define _PHY_BUT_DRV_BUTTON_MAX_BASEPLATE_PRESENT_VW (752)


	#define _PHY_BUT_DRV_CRADLE_MINBUTTON1_SKODA (108) 
	#define _PHY_BUT_DRV_CRADLE_MAXBUTTON1_SKODA (209)

	#define _PHY_BUT_DRV_CRADLE_MINBUTTON2_SKODA (210)
	#define _PHY_BUT_DRV_CRADLE_MAXBUTTON2_SKODA (364)

	#define _PHY_BUT_DRV_CRADLE_MINBUTTON3_SKODA (365)
	#define _PHY_BUT_DRV_CRADLE_MAXBUTTON3_SKODA (503)

	#define _PHY_BUT_DRV_CRADLE_MINNOTPRESSED_SKODA (504)
	#define _PHY_BUT_DRV_CRADLE_MAXNOTPRESSED_SKODA (632)

    #define _PHY_BUT_DRV_BUTTON_MIN_CRADLE_PRESENT_SKODA    (504)
	#define _PHY_BUT_DRV_BUTTON_MAX_CRADLE_PRESENT_SKODA    (632)

	#define _PHY_BUT_DRV_BUTTON_MIN_BASEPLATE_PRESENT_SKODA (633)
	#define _PHY_BUT_DRV_BUTTON_MAX_BASEPLATE_PRESENT_SKODA (752)

#endif
/**
   \brief Minimum voltage lavel for detection of button1

   \brief if the voltage is less than this value a low voltage error
          is created for the diagnostic. The upper limit for the voltage
	  band is defined by  PHY_BUT_DRV_MINBUTTON2
*/
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
    #define _PHY_BUT_DRV_3BM_MINBUTTON1 (255)
	#define _PHY_BUT_DRV_3BM_MINBUTTON2 (315) 
	#define _PHY_BUT_DRV_3BM_MINBUTTON3 (417)
	#define _PHY_BUT_DRV_3BM_MINNOTPRESSED (567)
	#define _PHY_BUT_DRV_3BM_MAXNOTPRESSED (782)
#elif (defined(PROCESSOR_TMS470R1VF45AA))
    #define _PHY_BUT_DRV_3BM_MINBUTTON1 (267+1)
    #define _PHY_BUT_DRV_3BM_MINBUTTON2 (390+1)
    #define _PHY_BUT_DRV_3BM_MINBUTTON3 (490+1)
    #define _PHY_BUT_DRV_3BM_MINNOTPRESSED (670+1)
	#define _PHY_BUT_DRV_3BM_MAXNOTPRESSED (870)
#endif

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum{
	enPhyButDrvButtonPressed,
	enPhyButDrvButtonReleased
}PHY_BUT_DRV_ENUM_MOD_INTERNAL_STATE;

typedef struct {
	PHY_BUT_DRV_ENUM_MOD_INTERNAL_STATE enModuleInternalState;
	Uint8Type u8ValidityDownCount;
} PHY_BUT_DRV_ST_DEVICE, *PHY_BUT_DRV_PST_DEVICE;


#define PHY_BUT_DRV_UP_COUNTER 5
#define PHY_BUT_DRV_DOWN_COUNTER 1

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/
#ifdef PHY_BUT_DRV_3BM_INIT

SuccessFailType PHY_BUT_DRV_sfPL_3BM_Init(void);

#endif

#ifdef PHY_BUT_DRV_CRADLE_INIT

SuccessFailType PHY_BUT_DRV_sfPL_Cradle_Init(void);

#endif


void PHY_BUT_DRV_vPL_handle (LOGI_BUT_MAN_ENUM_DEVICES enDevice, PHY_BUT_DRV_ST_MESSAGE stNewState);
PHY_BUT_DRV_ST_MESSAGE _PHY_BUT_DRV_enGetState_3BM(void);
PHY_BUT_DRV_ST_MESSAGE _PHY_BUT_DRV_enGetState_Cradle(void);
SuccessFailType PHY_BUT_DRV_sfPL_Init(void);
#endif /* _PHY_BUT_DRV_CI_H */

/* End Of File PHY_BUT_DRV_CI.h */
