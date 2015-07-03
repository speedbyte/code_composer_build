/****************************************************************************/
/* @Filename      LLD.H */
/*--------------------------------------------------------------------------*/
/* @Controller    Infineon TC1766 */
/* */
/* @Compiler      Tasking */
/* */
/* @Codegenerator 2.0.2 */
/* */
/* @Description   This file contains data types definition. */
/* */
/*--------------------------------------------------------------------------*/
/* @Date          02.12.2004 13:45:40 */
/* */
/****************************************************************************/
/* 
|
| Subversion Info:
| $Id: LLD.H 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/LLD.H $
|
*/ 


#ifndef LLD_H_
#define LLD_H_

/****************************************************************************/
/* @Defines */
/****************************************************************************/

#define  NULL             0 

/****************************************************************************/
/* @Typedefs */
/****************************************************************************/


typedef unsigned short IO_ChannelType;
typedef unsigned short *IO_MemPtrType;
typedef unsigned int IO_ValueType; 
typedef unsigned int OCPWM_ValueType; 
typedef unsigned int OCCCU_ValueType; 
typedef unsigned int OCTIM_ValueType; 
typedef unsigned int IO_PositionType; 
typedef unsigned int IO_SizeType;

typedef T_U32 IO_U32;

/*---------------------IO status enumeration-------------------------*/
/*@enum IO_STATUS | IO Status Enumeration
@comm Enumerates the IO Status returned by GetPortSync functions */


typedef enum
{
IO_LOW,              /*@emem Logical 0*/
IO_HIGH              /*@emem Logical 1*/
}IO_LevelType;



/*IO_DeviceType is recommended as 16 bit , */
/*but for simplicity and ease of use its taken as enum */
/*IO_DeviceType is used in IO_ErrorHook function */

typedef enum  
{ 
  TIM, 
  PWM, 
  CCU, 
  SCI, 
  SPI, 
  FLASH, 
  ADC, 
  OCWDT, 
  GPTA, 
  MSC,  
  MLI, 
  MULTI_CAN, 
  OCDIO 
} IO_DeviceType;

/*---------------------IO Error type enumeration-------------------------*/
/*@enum IO_ErrorType | IO Error Enumeration
  @comm Enumerates the IO Status returned by Get functions */

typedef enum
{
 IO_E_OK,					/*@emem If There is No error*/
 IO_E_PARAM_IGNORED,			/*@emem Invalid Parameter*/
 IO_E_BUSY,					/*@emem If the device is busy*/
 IO_E_UNKNOWN_MODE,			/*@emem If the mode is not supported*/
 IO_E_INVALID_CHANNEL_ID,	/*@emem If the channel Id is incorrect*/
 IO_E_MISSING_INIT,			/*@emem If the channel is not initialised*/
 IO_E_INVALID_VALUE,			/*@emem Value is incorrect*/
 IO_E_INVALID_SIZE,			/*@emem Size is incorrect*/
 IO_E_INVALID_POSITION,		/*@emem Position is incorrect*/
 IO_E_INVALID_NOTIF_TYPE,	/*@emem Notification type is incorrect*/	
 IO_E_ROMVERSION,			/*@emem The controller has ROM program memory*/
 IO_E_TYPE_MISMATCH,			/*@emem flash type is mismatched*/
 IO_E_FCN_SUSPENDED,			/*@emem ADC is suspended*/
 IO_E_FAILURE,               /*@emem erroneous operation*/
 IO_E_LOCKED,                /*@emem used in FLASH  for locked segments*/
 IO_E_INVALID_MODE           /*@emem invalid modes*/
}IO_ErrorType;




/*---------------------IO Notification type enumeration-------------------------*/
/*@enum IO_NotifType | IO Error Enumeration
  @comm Enumerates the IO Notification types for all peripherals */


typedef enum
{
	IO_N_RECEIVE =   1,			    /*@emem For Receive complete notification*/
	IO_N_TRANSMIT =  (1<<1),		/*@emem For Transmit complete notification*/
	IO_N_FCN_SUSPENDED = (1<<2),	/*@emem When the ADC conversion is suspended*/
	IO_N_PARITY =(1<<3),			/*@emem When Parity error occurs*/
	IO_N_FRAMING =(1<<4),			/*@emem When Framing error occurs*/
	IO_N_OVERRUN =(1<<5),			/*@emem When Overrun error occurs*/
	IO_N_THRESHOLD_1 =(1<<6),		/*@emem For the Threshold1 compare notification*/
	IO_N_THRESHOLD_2 =(1<<7),		/*@emem For the Threshold2 compare notification*/
	IO_N_CAPTURE =(1<<8),			/*@emem For Capture notification*/
	IO_N_RISING_EDGE =(1<<9),		/*@emem For Rising edge notification*/
	IO_N_FALLING_EDGE =(1<<10),		/*@emem For Falling edge notification*/
	IO_N_ALL =(1<<11),				/*@emem For All type of notification*/
	IO_N_READY =(1<<12),			/*@emem When ADC/SPI is ready for sample*/
	IO_N_TX_ERROR =(1<<13), 		/*@emem Tx error*/
	IO_N_RX_ERROR = (1<<14),		/*@emem Rx error*/
	IO_N_PHASE_ERROR = (1<<15),		/*@emem Phase error*/
	IO_N_BAUDRATE_ERROR = (1<<16),  /*@emem Baudrate error*/
	IO_N_CSSET = (1<<17),			/*@emem When Chip Select is Active*/
	IO_N_CSRESET =(1<<18),			/*@emem When Chip Select is Inactive*/
	IO_N_ROLLOVER=(1<<19)			/*@emem When Timer is rolled over*/
}IO_NotifType;


#endif 
