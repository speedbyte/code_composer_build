/* define variable types */
typedef int BOOL;
typedef unsigned char UBYTE;
typedef unsigned short int UINT16;
typedef unsigned long int UINT32;

/* define booleans */
#define FALSE 0
#define TRUE 1

/* define structures */

/*********************************************
 * The FLASH_STATUS_ST structure is used for *
 * storing and retrieving information beyond *
 * just the return value from a function.    *
 * for example, pulse data from program/erase*
 * and failing address and data              *
 *********************************************/
typedef struct {
  UINT32 stat1;
  UINT32 stat2;
  UINT32 stat3;
  UINT32 stat4;
} FLASH_STATUS_ST;

/*********************************************
 * The FLASH_ENG_INFO_ST structure contains  *
 * engineering information stored in the     *
 * engineering portion of the Flash (for F10 *
 * this is the ENGR0 row of the engineering  *
 * array, and for F05 the TI OTP sector)     *
 *********************************************/
typedef struct {
  UINT32 DevID;
  UINT32 LotNo;
  UINT16 FlowCheck;
  UINT16 WaferNo;
  UINT16 Xcoord;
  UINT16 Ycoord;
} FLASH_ENGR_INFO_ST;

/***************************************************
 * The FLASH_ARRAY_ST structure type defined       *
 * a UNINT32 array pointer which points to         *
 * the first control base address of a given       *
 * flash module.                                   *
 * The index of the array defines what 32 bit      *
 * offset from the first address to store the      *
 * intended data.  For example:                    *
 *   FLASH_ARRAY_ST cntl=0xffe88000;               *
 *   cntl[4]=0xa5; (this stores 0xa5 to 0xffe8010) *
 ***************************************************/
typedef volatile UINT32 * FLASH_ARRAY_ST;

/***************************************************
 * F10 devices can have up to 4 cores per module   *
 * F05 devices can have up to 8 cores per module   *
 ***************************************************/
typedef enum {
#ifdef F05
  FLASH_CORE0,
  FLASH_CORE1,
  FLASH_CORE2,
  FLASH_CORE3,
  FLASH_CORE4,
  FLASH_CORE5,
  FLASH_CORE6,
  FLASH_CORE7
#else
  FLASH_CORE0,
  FLASH_CORE1,
  FLASH_CORE2,
  FLASH_CORE3
#endif
} FLASH_CORE;

/***************************************************
 * F10 devices can have up to 4 sectors per core   *
 * F05 devices can have up to 32 sectors per core  *
 ***************************************************/
typedef enum {
#ifdef F05
  FLASH_SECT0,
  FLASH_SECT1,
  FLASH_SECT2,
  FLASH_SECT3,
  FLASH_SECT4,
  FLASH_SECT5,
  FLASH_SECT6,
  FLASH_SECT7,
  FLASH_SECT8,
  FLASH_SECT9,
  FLASH_SECT10,
  FLASH_SECT11,
  FLASH_SECT12,
  FLASH_SECT13,
  FLASH_SECT14,
  FLASH_SECT15,
  FLASH_SECT16,
  FLASH_SECT17,
  FLASH_SECT18,
  FLASH_SECT19,
  FLASH_SECT20,
  FLASH_SECT21,
  FLASH_SECT22,
  FLASH_SECT23,
  FLASH_SECT24,
  FLASH_SECT25,
  FLASH_SECT26,
  FLASH_SECT27,
  FLASH_SECT28,
  FLASH_SECT29,
  FLASH_SECT30,
  FLASH_SECT31
#else
  FLASH_SECT0,
  FLASH_SECT1,
  FLASH_SECT2,
  FLASH_SECT3
#endif
} FLASH_SECT;

UINT16 Flash_API_Version_U16(void);

BOOL Flash_Erase_Sector_B(UINT32 *start,
			  UINT32 length,
			  FLASH_CORE core,
			  FLASH_SECT sector,
			  UINT32 delay,
			  FLASH_ARRAY_ST cntl
			  );

BOOL Flash_Blank_B(UINT32 *start,
		   UINT32 length,
		   FLASH_CORE core,
		   FLASH_ARRAY_ST cntl,
		   FLASH_STATUS_ST *status
		   );

BOOL Flash_Verify_Zeros_B(UINT32 *start,
			  UINT32 length,
			  FLASH_CORE core,
			  FLASH_ARRAY_ST cntl,
			  FLASH_STATUS_ST *status
			  );

BOOL Flash_Compact_B(UINT32 *start,
		     FLASH_CORE core,
		     FLASH_SECT sector,
		     UINT32 delay,
		     FLASH_ARRAY_ST cntl,
		     FLASH_STATUS_ST *status
		     );

BOOL Flash_Erase_B(UINT32 *start,
		   UINT32 length,
		   FLASH_CORE core,
		   FLASH_SECT sector,
		   UINT32 delay,
		   FLASH_ARRAY_ST cntl,
		   FLASH_STATUS_ST *status
		   );

BOOL Flash_Prog_B(UINT32 *start,
		  UINT32 *buffer,
		  UINT32 length,
		  FLASH_CORE core,
		  UINT32 delay,
		  FLASH_ARRAY_ST cntl,
		  FLASH_STATUS_ST *status
		  );

BOOL Flash_Zeros_B(UINT32 *start,
		   UINT32 length,
		   FLASH_CORE core,
		   UINT32 delay,
		   FLASH_ARRAY_ST cntl,
		   FLASH_STATUS_ST *status
		   );

BOOL Flash_Verify_B(UINT32 *start,
		    UINT32 *buffer,
		    UINT32 length,
		    FLASH_CORE core,
		    FLASH_ARRAY_ST cntl,
		    FLASH_STATUS_ST *status
		    );

BOOL Flash_PSA_Verify_B(UINT32 *start,
			UINT32 length,
			UINT32 psa,
			FLASH_CORE core,
			FLASH_ARRAY_ST cntl,
			FLASH_STATUS_ST *status
			);

UINT32 Flash_PSA_Calc_U32(UINT32 *start,
			  UINT32 length,
			  UINT32 psa_seed,
			  UINT32 mode,
			  FLASH_CORE core,
			  FLASH_ARRAY_ST cntl
			  );

UINT32 PSA_U32(UINT32 *start,
	       UINT32 length,
	       UINT32 pseed
	       );

BOOL Flash_Match_Key_B(volatile UINT32 *key_start,
		       const UINT32 key[],
		       FLASH_ARRAY_ST cntl
		       );

void Flash_Match_Key_V(volatile UINT32 *key_start,
		       const UINT32 key[],
		       FLASH_ARRAY_ST cntl
		       );

void Flash_EngRow_V(UINT32 *start,
		    FLASH_ARRAY_ST cntl,
		    FLASH_STATUS_ST *status
		    ); 

void Flash_EngInfo_V(UINT32 *start,
		     FLASH_ARRAY_ST cntl,
		     FLASH_ENGR_INFO_ST *info
		     ); 

/* added to allow for CPU Vt functionality - shd 30-Aug-2004 */
BOOL Flash_Vt_Verify_B(UINT32 *start,
		       UINT32 *buffer,
		       UINT32 length,
		       FLASH_CORE core,
		       UINT32 delay,
		       FLASH_ARRAY_ST cntl,
		       FLASH_STATUS_ST *status
		       );

/* added to allow for CPU Vt functionality - shd 30-Aug-2004 */
BOOL Flash_PSA_Vt_Verify_B(UINT32 *start,
			   UINT32 length,
			   UINT32 psa,
			   FLASH_CORE core,
			   UINT32 delay,
			   FLASH_ARRAY_ST cntl,
			   FLASH_STATUS_ST *status
			   );

/* added to allow for CPU Vt functionality - shd 30-Aug-2004 */
BOOL Flash_Vt_Blank_B(UINT32 *start,
		      UINT32 length,
		      FLASH_CORE core,
		      UINT32 delay,
		      FLASH_ARRAY_ST cntl,
		      FLASH_STATUS_ST *status
		      );

/* added to allow for CPU Vt functionality - shd 30-Aug-2004 */
BOOL Flash_Vt_Verify_Zeros_B(UINT32 *start,
			     UINT32 length,
			     FLASH_CORE core,
			     UINT32 delay,
			     FLASH_ARRAY_ST cntl,
			     FLASH_STATUS_ST *status
			     );

/* added to allow for raw wait delays - shd 30-Aug-2004 */
void wait_delay(UINT32 delay);

#ifndef F05
void Flash_Prog_Key_V(volatile UINT32 *key_start,
		      const UINT32 key[],
		      FLASH_CORE core,
		      UINT32 delay,
		      FLASH_ARRAY_ST cntl
		      );

void Flash_Zero_Key_V(volatile UINT32 *key_start,
		      const UINT32 key[],
		      FLASH_CORE core,
		      UINT32 delay,
		      FLASH_ARRAY_ST cntl
		      ); 
#endif

#ifdef F05
BOOL OTP_Blank_B(UINT32 *start,
		 UINT32 length,
		 FLASH_CORE core,
		 FLASH_ARRAY_ST cntl,
		 FLASH_STATUS_ST *status
		 );

BOOL OTP_Prog_B(UINT32 *start,
		UINT32 *buff,
		UINT32 length,
		FLASH_CORE core,
		UINT32 delay,
		FLASH_ARRAY_ST cntl,
		FLASH_STATUS_ST *status
		);

void OTP_Read_V(UINT32 *start,
		UINT32 *buff,
		UINT32 length,
		FLASH_CORE core,
		FLASH_ARRAY_ST cntl
		);

BOOL OTP_Verify_B(UINT32 *start,
		  UINT32 *buff,
		  UINT32 length,
		  FLASH_CORE core,
		  FLASH_ARRAY_ST cntl,
		  FLASH_STATUS_ST *status
		  );

BOOL OTP_PSA_Verify_B(UINT32 *start,
		      UINT32 length,
		      UINT32 psa,
		      FLASH_CORE core,
		      FLASH_ARRAY_ST cntl,
		      FLASH_STATUS_ST *status
		      );

void Flash_Set_Vread_V(FLASH_ARRAY_ST cntl);

BOOL Flash_Start_Erase_B(UINT32 *start,
			 FLASH_CORE core,
			 UINT32 delay,
			 FLASH_ARRAY_ST cntl
			 );

UINT16 Flash_Erase_Status_U16(FLASH_ARRAY_ST cntl);

UINT16 Flash_Aux_Engr_U16(volatile UINT16 *start,
			  FLASH_ARRAY_ST cntl
			  );

void init_state_machine(FLASH_CORE core,
			UINT32 delay,
			FLASH_ARRAY_ST cntl
			);

void Flash_Sector_Select_V(FLASH_CORE core,
			   FLASH_ARRAY_ST cntl
			   );

void Feed_Watchdog_V();

#endif
