/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     CAN_CB.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_CAN
*-----------------------------------------------------------------------------
* Description:    file to support all callback function of the CAN driver
*-----------------------------------------------------------------------------
* $Date: 2010-09-17 16:31:55 +0200 (Fr, 17 Sep 2010) $
* $Rev: 15284 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/src/CAN_CB.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "bap_types.h"
#include "GIO.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "can_inc.h"
#include "CAN_CB_cfg.h"
#include "Port.h"
#include "spi.h"
#include "bap_canubs.h"
#include "CAN_CB.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*************************START CAN driver Callback functions******************/

/*----------------------------------------------------------------------------*/
/* Function    : WriteProtReg                                                 */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 

    \param      addr
                volatile canuint32*
                value limits
    \param      msk
                canuint32
                value limits
    \param      val
                canuint32
                value limits

    \return     void
   */
/*----------------------------------------------------------------------------*/
void WriteProtReg(volatile canuint32* addr, canuint32 msk, canuint32 val)
{
	canuint32 tmp;
	
	tmp = *addr;
	tmp &= (~msk);
	tmp |= val;
	*addr = tmp;
}/* END OF WriteProtReg() */

/*----------------------------------------------------------------------------*/
/* Function    : DisableIrq                                                   */
/**
    \brief      callback function from CAN driver

    \brief      Description:
                callback function to disable all Interrupts
                in the Interrupt Request mask register

    \return     void
   */
/*----------------------------------------------------------------------------*/
void DisableIrq(void)
{
    asm(" stmfd sp!, {r0}");
    asm(" mrs r0, cpsr");
    asm(" orr r0, r0, #0xC0");
    asm(" msr cpsr, r0");
    asm(" ldmfd sp!, {r0}");
}/* END OF DisableIrq() */

/*----------------------------------------------------------------------------*/
/* Function    : EnableIrq                                                    */
/**
    \brief      callback function from CAN driver

    \brief      Description:
                callback function to enable all Interrupts
                in the Interrupt Request mask register

    \return     void
   */
/*----------------------------------------------------------------------------*/
void EnableIrq(void)
{
    asm(" stmfd sp!, {r0}");
    asm(" mrs r0, cpsr");
    asm(" bic r0, r0, #0xC0");
    asm(" msr cpsr, r0");
    asm(" ldmfd sp!, {r0}");
}/* END OF EnableIrq() */

/*----------------------------------------------------------------------------*/
/* Function    : ApplCanAddCanInterruptDisable                                */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 

    \param      channel
                CanChannelHandle
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplCanAddCanInterruptDisable(CanChannelHandle channel)
{
	channel = channel;
    *((volatile uint32*)osdREQMASK) &=~CAN1_INT_FLAG_CIM;   /* disable interrupt */	
}/* END OF ApplCanAddCanInterruptDisable() */

/*----------------------------------------------------------------------------*/
/* Function    : ApplCanAddCanInterruptRestore                                */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 

    \param      channel
                CanChannelHandle
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplCanAddCanInterruptRestore(CanChannelHandle channel)
{
	channel = channel;	
    *((volatile uint32*)osdFIRQPR)  &=~CAN1_INT_FLAG_CIM;   /* interrupt is IRQ */
	*((volatile uint32*)osdINTREQ)  &=~CAN1_INT_FLAG_CIM;   /* clear pending flag */
	*((volatile uint32*)osdREQMASK) |= CAN1_INT_FLAG_CIM;   /* enable interrupt  */	
}/* END OF ApplCanAddCanInterruptRestore() */

/*----------------------------------------------------------------------------*/
/* Function    : ApplCanMsgReceived                                           */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 

    \param      rxStruct
                CanRxInfoStructPtr

    \return     canuint8
                unsigned char
                0-255
   */
/*----------------------------------------------------------------------------*/
canuint8 ApplCanMsgReceived(CanRxInfoStructPtr rxStruct)
{
    rxStruct = rxStruct;
	return (kCanCopyData);
}/* END OF ApplCanMsgReceived() */

/*----------------------------------------------------------------------------*/
/* Function    : ApplCanOverrun                                               */
/** 
    \brief      callback function from CAN driver
  
    \return     void
   */
/*----------------------------------------------------------------------------*/
#if defined( C_ENABLE_OVERRUN )
void ApplCanOverrun ( void )
{
    ;
}/* END OF ApplCanOverrun() */
#endif


/*----------------------------------------------------------------------------*/
/* Function    : ApplCanFullCanOverrun                                        */
/** 
    \brief      callback function from CAN driver
  
    \return     void
   */
/*----------------------------------------------------------------------------*/
#if defined( C_ENABLE_FULLCAN_OVERRUN )
void ApplCanFullCanOverrun ( void )
{
    ;
}/* END OF ApplCanFullCanOverrun() */
#endif

/*----------------------------------------------------------------------------*/
/* Function    : ApplCanMsgDlcFailed                                          */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                called by can_drv.c run in the context 
                of HostTask 

    \param      rxStruct
                CanRxInfoStructPtr
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplCanMsgDlcFailed( CanRxInfoStructPtr rxStruct )
{
    rxStruct = rxStruct;
}/* END OF ApplCanMsgDlcFailed() */


/************************END CAN driver Callback functions*********************/


/********************START CAN transceiver Callback functions******************/

/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToNormal                                       */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                set the transceiver in the normal mode 
				CAN_EN = HIGH;      CAN_STB_ = HIGH; 
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToNormal(void)
{
    /*switch transceiver to normal mode*/
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutHigh);   // CAN_EN
	CAN_vPortSet(CAN3, RX);                                 // CAN_STB
#elif (defined(PROCESSOR_TMS470R1VF45AA))
    CAN_SET_IO(CAN_EN);
	CAN_SET_IO(CAN_STB_);
#endif
}/* END OF BusTransceiverToNormal() */


/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToStandby                                      */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                set the transceiver in the standby mode 
				CAN_EN = LOW;      CAN_STB_ = LOW; 
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToStandby(void)
{
    /*switch Trancseiver to stand-by mode*/
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
//    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutLow);    // CAN_EN
//	CAN_vPortClear(CAN3, RX);                               // CAN_STB
#elif (defined(PROCESSOR_TMS470R1VF45AA))
//	CAN_CLEAR_IO(CAN_EN);
//	CAN_CLEAR_IO(CAN_STB_);
#endif
}/* END OF BusTransceiverToStandby() */


/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToSleep                                      */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                set the transceiver in the sleep mode 
				first:  CAN_EN = HIGH;      CAN_STB_ = LOW; 
                second: CAN_EN = LOW;       CAN_STB_ = LOW; 
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToSleep(void)
{
    /* !!!! THE while LOOP Counter is hardware and compiler dependant. Please read the
     * CAN Transreciever data sheet about the max time !!! */
	canuint32 Iterations = 3000;
    
//    CanOffline();
    
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
//    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutHigh);   // CAN_EN
#elif (defined(PROCESSOR_TMS470R1VF45AA))
//    CAN_SET_IO(CAN_EN);
#endif

    while (Iterations != 0)
    {
        Iterations--;  
    }
    Iterations = 3000;
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
//	CAN_vPortClear(CAN3, RX);                               // CAN_STB
#elif (defined(PROCESSOR_TMS470R1VF45AA))
//	CAN_CLEAR_IO(CAN_STB_);
#endif

    while (Iterations != 0)
        {
            asm("nop");

            Iterations = Iterations - 1;  
        }

    /* Sleep CAN transceiver */		
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
//    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutLow);    // CAN_EN
//	CAN_vPortClear(CAN3, RX);                               // CAN_STB
#elif (defined(PROCESSOR_TMS470R1VF45AA))
//	CAN_CLEAR_IO(CAN_EN);
//	CAN_CLEAR_IO(CAN_STB_);
#endif
}/* END OF BusTransceiverToSleep() */

/*----------------------------------------------------------------------------*/
/* Function    : BusTransceiverToPowerOn                                      */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                set the transceiver in the Power on mode 
				CAN_EN = LOW;       CAN_STB_ = HIGH; 
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BusTransceiverToPowerOn(void)
{
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutLow);    // CAN_EN
	CAN_vPortSet(CAN3, RX);                                 // CAN_STB
#elif (defined(PROCESSOR_TMS470R1VF45AA))
	CAN_CLEAR_IO(CAN_EN);
	CAN_SET_IO(CAN_STB_);
#endif
}/* END OF BusTransceiverToPowerOn() */


/********************START CAN transceiver Callback functions******************/


/*----------------------------------------------------------------------------*/
/* Function    : App_BapDiagnoseSwitch                                          */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                called by can_drv.c 

    \param      rxObject
                CanReceiveHandle
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void App_BapDiagnoseSwitch(CanReceiveHandle rxObject)
{
	;
}/* END OF App_BapDiagnoseSwitch() */

/*----------------------------------------------------------------------------*/
/* Function    : BAP_BCL_CanRxPreCopy                                          */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                called by can_drv.c run in the context 
                of HostTask 

    \param      aRxStruct
                CanRxInfoStructPtr
                
    \return     canuint8
   */
/*----------------------------------------------------------------------------*/

canuint8 BAP_BCL_CanRxPreCopy(CanRxInfoStructPtr aRxStruct)
{
	canuint8 canRet = 0;
    BAP_CANUBS_RxData((BapCanInterfaceChannel_t) aRxStruct->Channel+1, aRxStruct->Handle, (ptr_t) aRxStruct->pChipData, CanRxActualDLC(aRxStruct) );
	
    return canRet;
}/* END OF BAP_BCL_CanRxPreCopy() */

/*----------------------------------------------------------------------------*/
/* Function    : App_SysInfoSwitch                                          */
/** 
    \brief      callback function from CAN driver
  
    \brief      Description:
                called by can_drv.c 

    \param      rxObject
                CanReceiveHandle
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
void App_SysInfoSwitch(CanReceiveHandle rxObject)
{
	;
}/* END OF App_SysInfoSwitch() */


void ApplCanFatalError( canuint8 errorNumber )
{
	DEBUG_VALUE1(CALLBACK_SWI_TRC, MOD_CALLBACK, "Value %d", errorNumber);
}

/* End Of File MOD_CanInterface.c */
