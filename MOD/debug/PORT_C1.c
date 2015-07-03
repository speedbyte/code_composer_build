/*
 * GPIO Map: 7 A ( Interrupt Enabled )  + 1 A Interrupt Enabled and only Input + 8 B +8 C + 4 D
 */
#include "Common.h"
#include "GIO.h"
#include "PORT.h"
#include "SPI.h"
#ifdef UHV_ACTIVATE_FEATURE_LED
#include "LED.h"
#endif
#include "MONITOR_ADC.h"
#include "APP_DEVICE_AudioManager_Wire.h"
/* Dead Code. For future implementation. - VA --------*/
#define WD_PORT GIOD
#define WD_PIN PIN2
#define PORT_vPinModify(WD_PORT, WD_PIN, x) GIO_vPinModify(WD_PORT, WD_PIN, x)
/* --------------------- */
inline void _PORT_vInitGIO(void);

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))

void CAN_vPortSet(CAN_ST *pstCan, Uint8Type PIN )
{
    if (RX == PIN)
    {
        pstCan->RIOC_UN.RIOC_ST.RxFunc_B1 = 0;
        pstCan->RIOC_UN.RIOC_ST.RxDir_B1 = 1;
        pstCan->RIOC_UN.RIOC_ST.RxOut_B1 = 1;
    }
    else if (TX == PIN)
    {
        pstCan->TIOC_UN.TIOC_ST.TxFunc_B1 = 0;
        pstCan->TIOC_UN.TIOC_ST.TxDir_B1 = 1;
        pstCan->TIOC_UN.TIOC_ST.TxOut_B1 = 1;
        
    }
    else
    {
    }
    return;
}

void CAN_vPortClear(CAN_ST *pstCan, Uint8Type PIN )
{
    if ( RX == PIN )
    {
        pstCan->RIOC_UN.RIOC_ST.RxFunc_B1 = 0;
        pstCan->RIOC_UN.RIOC_ST.RxDir_B1 = 1;
        pstCan->RIOC_UN.RIOC_ST.RxOut_B1 = 0;
    }
    else if (TX == PIN)
    {
        pstCan->TIOC_UN.TIOC_ST.TxFunc_B1 = 0;
        pstCan->TIOC_UN.TIOC_ST.TxDir_B1 = 1;
        pstCan->TIOC_UN.TIOC_ST.TxOut_B1 = 0;
    }
    else
    {
    }
    return;
}

BooleanType CAN_vPortRead(CAN_ST *pstCan, Uint8Type PIN)
{
	if ( RX == PIN )
    {
		return((BooleanType)pstCan->RIOC_UN.RIOC_ST.RxOut_B1);
    }
    else
    {
    	return((BooleanType)pstCan->TIOC_UN.TIOC_ST.TxOut_B1);
    }
}
#endif


void PORT_vInit()
{
	_PORT_vInitGIO();
}


inline void _PORT_vInitGIO()
{
#ifdef UHV_ACTIVATE_FEATURE_LED
	LED_GREEN_LED_OFF  // PORT_LED_GREEN_ON_OFF - Low, PORT_LED_GREEN_TB - Low
	LED_RED_LED_OFF  // PORT_LED_RED_ON_OFF - Low, PORT_LED_RED_TB - Low
#endif
	GIO_vPinModify(GIOA, PORT_NOTUSED_PIN0_GIOA, PortIoOutHigh);
	GIO_vPinModify(GIOA, PORT_NOTUSED_PIN1_GIOA, PortIoOutHigh);
	GIO_vPinModify(GIOA, PORT_CAN_ERROR, PortIoIn);
    /* Set the CANInh Pin as a Input */
    GIO_vPinModify(GIOA, PORT_CAN_INHIBIT, PortIoIn);	
	GIO_vPinModify(GIOA, PORT_LOW_BATTERY, PortIoIn);
	GIO_vPinModify(GIOA, PORT_NOTUSED_PIN5_GIOA, PortIoOutHigh);
	GIO_vPinModify(GIOA, PORT_NOTUSED_PIN6_GIOA, PortIoOutHigh);
	GIO_vPinModify(GIOA, PORT_NOTUSED_PIN7_GIOA, PortIoOutHigh);
	
	GIO_vPinModify(GIOB, PORT_FLASH_EN, PortIoOutHigh);
	
	AUDIO_DIAG_END_INIT

	GIO_vPinModify(GIOB, PORT_NOTUSED_PIN2_GIOB, PortIoOutHigh);
    LOGI_BUT_MAN_CRADLE_WUP_OFF // ??? It is an input port
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutLow);   /* CAN_EN */
	CAN_vPortClear(CAN3, RX);                             /* CAN_STB */
#else if (defined(PROCESSOR_TMS470R1VF45AA))
	GIO_vPinModify(GIOB, PORT_CAN_EN, PortIoOutHigh);
	GIO_vPinModify(GIOB, PORT_CAN_STB, PortIoOutHigh);
#endif
	APP_DEVICE_AM_MUTE_OFF_BY_WIRE_INIT
	
	LOGI_BUT_MAN_RLADDER_ON_INIT
	
#ifdef UHV_ACTIVATE_FEATURE_LED
	LED_DIAG_STOP
#endif	

	LOGI_BUT_MAN_CRADLE_OFF


	GIO_vPinModify(GIOD, PORT_MON_WATCHDOG, PortIoOutHigh);
	GIO_vPinModify(GIOD, PORT_MON_BOOTS, PortIoOutHigh);
	GIO_vPinModify(GIOD, PORT_MON_NRESETS, PortIoOutLow);
	GIO_vPinModify(GIOD, PORT_MON_PWRHOLD, PortIoOutHigh);
	

/*switch Trancseiver to power on mode*/
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
    SPI_vPinModify(SPI5, SpiEnablePin, PortIoOutLow);   /* CAN_EN */
	CAN_vPortSet(CAN3, RX);                             /* CAN_STB */
#endif
}
