#ifndef PORT_H_
#define PORT_H_

#include "PORT_CE.h"
#include "CAN_CB_cfg.h"

typedef enum
{
    PortFunctional,
    PortIoIn,
    PortIoOut,
    PortIoOutHigh,
    PortIoOutLow
}PORTQUAL;

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
#define CAN3  (CAN_PST)HECC3_BASE
#define RX      1
#define TX      2
#endif

#define READ_CANTRANSCEIVER_INHIBIT    GIO_u8PinRead(PORT_CAN_INHIBIT, GIOA)

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
  #define READ_CANTRANSCEIVER_EN    SPI_bPinRead(SPI5, SpiEnablePin)  // CAN_EN
  #define READ_CANTRANSCEIVER_STB   CAN_vPortRead(CAN3, RX)           // CAN_STB
#endif

#if (defined(PROCESSOR_TMS470R1VF45AA))
  #define READ_CANTRANSCEIVER_EN   GIO_u8PinRead(CAN_EN);             // CAN_EN
  #define READ_CANTRANSCEIVER_STB  GIO_u8PinRead(CAN_STB_);           // CAN_STB
#endif




void PORT_vInit(void);

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
void CAN_vPortSet(CAN_ST *pstCan, Uint8Type PIN);
void CAN_vPortClear(CAN_ST *pstCan, Uint8Type PIN );
BooleanType CAN_vPortRead(CAN_ST *pstCan, Uint8Type PIN);
#endif

#endif /*PORT_H_*/
