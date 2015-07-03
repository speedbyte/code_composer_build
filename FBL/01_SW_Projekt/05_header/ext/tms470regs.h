/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  tms470regs.h                                                     |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the register of the tms470                                       |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   RS         Ralf Schmidgall           SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: tms470regs.h 5764 2010-06-28 07:04:00Z werner $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/20100706_S1NN_TMS470S_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/tms470regs.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __tms470regs_
#define __tms470regs_
/* ========================================================================== */

/*===== MEMORY CONTROLLER CONTROL REGISTERS ===================================*/
/* start address ffff_fd00 */
typedef volatile struct
{
   BITFIELD :16;
   UWORD SMCR0_UW;
   BITFIELD :16;
   UWORD SMCR1_UW;
   BITFIELD :16;
   UWORD SMCR2_UW;
   BITFIELD :16;
   UWORD SMCR3_UW;
   BITFIELD :16;
   UWORD SMCR4_UW;
   BITFIELD :16;
   UWORD SMCR5_UW;
   BITFIELD :16;
   UWORD SMCR6_UW;
   BITFIELD :16;
   UWORD SMCR7_UW;
   BITFIELD :16;
   UWORD SMCR8_UW;
   BITFIELD :16;
   UWORD SMCR9_UW;
   BITFIELD :16;
   UWORD reserved1;
   BITFIELD :16;
   UWORD WCR0_UW;
   BITFIELD :16;
   UWORD PCR0_UW;
   BITFIELD :16;
   UWORD PLR_UW;
   BITFIELD :16;
   UWORD PPROT_UW;
   BITFIELD :16;
   UWORD reserved2;
}
MEMCTL_ST;

extern MEMCTL_ST e_MEMCTL_ST;

/*===== ADDRESS MANAGER CONTROL REGISTERS =====================================*/
/* start address ffff_fe00 */
typedef volatile struct
{
   BITFIELD :16;
   UWORD MFBAHR0_UW;
   BITFIELD :16;
   UWORD MFBALR0_UW;
   BITFIELD :16;
   UWORD MFBAHR1_UW;
   BITFIELD :16;
   UWORD MFBALR1_UW;
   BITFIELD :16;
   UWORD MFBAHR2_UW;
   BITFIELD :16;
   UWORD MFBALR2_UW;
   BITFIELD :16;
   UWORD MFBAHR3_UW;
   BITFIELD :16;
   UWORD MFBALR3_UW;
   BITFIELD :16;
   UWORD MFBAHR4_UW;
   BITFIELD :16;
   UWORD MFBALR4_UW;
   BITFIELD :16;
   UWORD MFBAHR5_UW;
   BITFIELD :16;
   UWORD MFBALR5_UW;
   BITFIELD :16;
   UWORD MCBAHR0_UW;
   BITFIELD :16;
   UWORD MCBALR0_UW;
   BITFIELD :16;
   UWORD MCBAHR1_UW;
   BITFIELD :16;
   UWORD MCBALR1_UW;
   BITFIELD :16;
   UWORD MCBAHR2_UW;
   BITFIELD :16;
   UWORD MCBALR2_UW;
   BITFIELD :16;
   UWORD MCBAHR3_UW;
   BITFIELD :16;
   UWORD MCBALR3_UW;
   BITFIELD :16;
   UWORD MCBAHR4_UW;
   BITFIELD :16;
   UWORD MCBALR4_UW;
   BITFIELD :16;
   UWORD MCBAHR5_UW;
   BITFIELD :16;
   UWORD MCBALR5_UW;
}
ADRMAN_ST;

extern ADRMAN_ST e_ADRMAN_ST;

/****************************************************************************/
/* SARCIM_ST                                                                */
/*      Definition of the Central Interrupt Module register map.  (p.5-15)  */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct
{
    unsigned int   : 24;
    unsigned char  IrqIVec_UB;
    unsigned int   : 24;
    unsigned char  FiqIVec_UB;
    unsigned int   : 24;
    unsigned char  CimIVec_UB;

    unsigned long   Firqpr_UL;
    unsigned long   Intreq_UL;
    unsigned long   Reqmask_UL;

} SARCIM_ST;

/* #endif  */

extern SARCIM_ST e_SARCIM_ST;
/****************************************************************************/
/* SARSYS_ST                                                                */
/*      Definition of the System Module register map.                       */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct
{
    unsigned int   : 16;
    unsigned short ClkCntl_UW;
    unsigned int   : 32;
    unsigned int   : 16;
    unsigned short CramCR_UW;
    unsigned int   : 16;
    unsigned short GlbCtrl_UW;
    unsigned int   : 16;
    unsigned short SysECR_UW;
    unsigned int   : 16;
    unsigned short SysESR_UW;
    unsigned int   : 16;
    unsigned short AbrtESR_UW;
    unsigned int   : 16;
    unsigned short GlbStat_UW;
    unsigned int   : 16;
    unsigned short Dev_UW;
    unsigned int   : 32;
    unsigned int   : 16;
    unsigned short Ssif_UW;
    unsigned int   : 16;
    unsigned short Ssir_UW;
} SARSYS_ST;

/* #endif  */

extern SARSYS_ST e_SARSYS_ST;
/****************************************************************************/
/****************************************************************************/
/* SARMMC_ST                                                                */
/* Definition of the Memory Control register map.                           */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct
{
    unsigned int   : 16;
    unsigned short smcR0_UW;
    unsigned int   : 16;
    unsigned short smcR1_UW;
    unsigned int   : 16;
    unsigned short smcR2_UW;
    unsigned int   : 16;
    unsigned short smcR3_UW;
    unsigned int   : 16;
    unsigned short smcR4_UW;
    unsigned int   : 16;
    unsigned short smcR5_UW;
    unsigned int   : 16;
    unsigned short smcR6_UW;
    unsigned int   : 16;
    unsigned short smcR7_UW;
    unsigned int   : 16;
    unsigned short smcR8_UW;
    unsigned int   : 16;
    unsigned short smcR9_UW;

    unsigned int : 32;

    unsigned int : 16;
    union
    {
        unsigned short wcr0_UW;
        struct
        {
            unsigned int             : 14;
            unsigned int WTWSOvr_B1  : 1;
            unsigned int WBEnable_B1 : 1;
        } wcr0_ST;
    } wcr0_UN;

    unsigned int : 16;
    union
    {
        unsigned short pcr0_UW;
        struct
        {
            unsigned int            : 11;
            unsigned int ClkDiv_B4  : 4;
            unsigned int PEnable_B1 : 1;
        } pcr0_ST;
    } pcr0_UN;

    unsigned int   : 16;
    unsigned short plr_UW;
    unsigned int   : 16;
    unsigned short pprot_UW;

} SARMMC_ST;

/* #endif  */

extern SARMMC_ST e_SARMMC_ST;


/****************************************************************************/
/* SARDEC_ST                                                                */
/*      Definition of the Address Manager register map.                     */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct
{
    unsigned int   : 16;
    unsigned short mfbahR0_UW;
    unsigned int   : 16;
    unsigned short mfbalR0_UW;
    unsigned int   : 16;
    unsigned short mfbahR1_UW;
    unsigned int   : 16;
    unsigned short mfbalR1_UW;
    unsigned int   : 16;
    unsigned short mfbahR2_UW;
    unsigned int   : 16;
    unsigned short mfbalR2_UW;
    unsigned int   : 16;
    unsigned short mfbahR3_UW;
    unsigned int   : 16;
    unsigned short mfbalR3_UW;
    unsigned int   : 16;
    unsigned short mfbahR4_UW;
    unsigned int   : 16;
    unsigned short mfbalR4_UW;
    unsigned int   : 16;
    unsigned short mfbahR5_UW;
    unsigned int   : 16;
    unsigned short mfbalR5_UW;

    unsigned int   : 16;
    unsigned short mcbahR0_UW;
    unsigned int   : 16;
    unsigned short mcbalR0_UW;
    unsigned int   : 16;
    unsigned short mcbahR1_UW;
    unsigned int   : 16;
    unsigned short mcbalR1_UW;
    unsigned int   : 16;
    unsigned short mcbahR2_UW;
    unsigned int   : 16;
    unsigned short mcbalR2_UW;
    unsigned int   : 16;
    unsigned short mcbahR3_UW;
    unsigned int   : 16;
    unsigned short mcbalR3_UW;
    unsigned int   : 16;
    unsigned short mcbahR4_UW;
    unsigned int   : 16;
    unsigned short mcbalR4_UW;
    unsigned int   : 16;
    unsigned short mcbahR5_UW;
    unsigned int   : 16;
    unsigned short mcbalR5_UW;

} SARDEC_ST;

/* #endif  */

extern SARDEC_ST e_SARDEC_ST;

/****************************************************************************/
/* SARRTI_ST                                                                */
/*      Definition of the Real Time Interrupt Module register map           */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct rti
{
    union
    {
        unsigned long DRTICNTR_UL;
        struct
        {
            unsigned int DRTICNTR_B21  : 21;
            unsigned int DRTIMOD_B11   : 11;
        } DRTICNTR_ST;
    } DRTICNTR_UN;

	union
	{
		unsigned long DRTIPCTL_UL;
		struct
		{
			unsigned int : 16;
			unsigned int : 2;
			unsigned int RTIM_B3 : 3;
			unsigned int PREDL_B11 : 11;
		} DRTIPCTL_ST;
	} DRTIPCTL_UN;

	union
	{
		unsigned long DRTICNTL_UL;
		struct
		{
			unsigned int : 16;
			unsigned int : 8;
			unsigned int RTIF_B1 : 1;
			unsigned int RTIE_B1 : 1;
			unsigned int : 6;
		} DRTICNTL_ST;
	} DRTICNTL_UN;

	unsigned long WKEY_UL;

	unsigned long RTICR1_UL;

	unsigned long RTICR2_UL;

	union
	{
		unsigned long DRTICINT_UL;
		struct
		{
			unsigned int : 16;
			unsigned int : 8;
			unsigned int CF1_B1 : 1;
			unsigned int CF2_B1 : 1;
			unsigned int CE1_B1 : 1;
			unsigned int CR2_B1 : 1;
			unsigned int : 4;
		} DRTICINT_ST;
	} DRTICINT_UN;

	union
	{
		unsigned long DRTICNTEN_UL;
		struct
		{
			unsigned int : 16;
			unsigned int : 14;
			unsigned int CNTEN_B2 : 2;
		} DRTICNTEN_ST;
	} DRTICNTEN_UN;

} SARRTI_ST;

/* #endif  */

extern SARRTI_ST e_SARRTI_ST;

/****************************************************************************/
/* e_GIO_ST                                                                 */
/*      Structure overlay of the GIO registers, this is defined in the      */
/*      linker command file                                                 */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct gio_st
{
    struct
    {
        unsigned int         : 24;
        unsigned int         : 7;
        unsigned int Pwdn_B1 : 1;
    } Pwdn_ST;

    unsigned int  : 24;
    unsigned char Ena_UB;

    unsigned int  : 24;
    unsigned char Pol_UB;

    unsigned int  : 24;
    unsigned char Flg_UB;

    unsigned int  : 24;
    unsigned char Pry_UB;

    unsigned int  : 24;
    unsigned char OffA_UB;

    unsigned int  : 24;
    unsigned char EmuA_UB;

    unsigned int  : 24;
    unsigned char OffB_UB;

    unsigned int  : 24;
    unsigned char EmuB_UB;

    struct
    {
        unsigned long Dir_UL;
        unsigned long Din_UL;
        unsigned long Dout_UL;
        unsigned long Dset_UL;
        unsigned long Dclr_UL;
    } Port_ST[8];

} GIO_ST;

/* #endif  */


extern GIO_ST e_GIO_ST;

/****************************************************************************/
/* HET_ST                                                                   */
/*      Definition of the HET register map.                                 */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct het
{
    union
    {
        unsigned long gcr_UL;
        struct
        {
            unsigned                  : 7;
            unsigned PowerDown_B1     : 1;
            unsigned                  : 7;
            unsigned ClkMaster_B1     : 1;
            unsigned                  : 7;
            unsigned Access64_B1      : 1;
            unsigned                  : 5;
            unsigned DebugFlag_B1     : 1;
            unsigned ContOnSuspend_B1 : 1;
            unsigned TurnOn_B1        : 1;
        } gcr_ST;
    } gcr_UN;

    union
    {
        unsigned long pfr_UL;
        struct
        {
            unsigned               : 21;
            unsigned LRPrescale_B3 : 3;
            unsigned               : 2;
            unsigned HRPrescale_B6 : 6;
        } pfr_ST;
    } pfr_UN;

    unsigned            : 24;
    unsigned Addr_B8    : 8;

    unsigned            : 24;
    unsigned Offset1_B8 : 8;

    unsigned            : 24;
    unsigned Offset2_B8 : 8;

    union
    {
        unsigned long ctrl1_UL;
        struct
        {
            unsigned            : 7;
            unsigned ExcEN34_B1 : 1;
            unsigned            : 7;
            unsigned ExcEN33_B1 : 1;
            unsigned            : 7;
            unsigned ExcEN32_B1 : 1;
            unsigned            : 5;
            unsigned ExcPL34_B1 : 1;
            unsigned ExcPL33_B1 : 1;
            unsigned ExcPL32_B1 : 1;
        } ctrl1_ST;
    } ctrl1_UN;

    union
    {
        unsigned long ctrl2_UL;
        struct
        {
            unsigned                   : 29;
            unsigned ApnctOvrflFlag_B1 : 1;
            unsigned ApnctUndflFlag_B1 : 1;
            unsigned PrgmOvrflFlag_B1  : 1;
        } ctrl2_ST;
    } ctrl2_UN;

    unsigned long SWPL_UL;
    unsigned long Flags_UL;

    unsigned             : 32;
    unsigned             : 32;
    unsigned             : 20;
    unsigned HRShare_B12 : 12;
    unsigned             : 32;

    unsigned long CCDir_UL;
    unsigned long CCDin_UL;
    unsigned long CCDwr_UL;
    unsigned long CCDSet_UL;
    unsigned long CCDClr_UL;

} HET_ST;

/* #endif  */

extern HET_ST e_HET0_ST;

/****************************************************************************/
/* SPI_ST                                                                   */
/*      Definition of the SPI register map.                                 */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef union
    {
       unsigned long buf_UL;
       struct
       {
           unsigned int               : 14;
           unsigned int   RcvrOvrn_B1 : 1;
           unsigned int   IntFlag_B1  : 1;
           unsigned short buf_UW;
       } buf_ST;
    } SPIBUF;

typedef  volatile struct spi_st
{
    /* SPICTRL1 Determines the bit transfer rate and sets the number of bits per character */
    /* Offset Address: 0x00 */
    union spi_ctrl1
    {
        struct
        {
          unsigned int : 16;          /* Reserved */
	      unsigned short  uw_Ctrl1;   /* SPI Control Register 1 */
	    } ctrl1_UW;
        struct
        {
            unsigned int              : 19;
            unsigned int TapSel_B8    : 8;
            unsigned int CharLen_B5   : 5;
        } ctrl1_ST;
    } ctrl1_UN;

    /* SPICTRL2 Sets the configuration of the different modes */
    /* Offset Address: 0x04 */
    union spi_ctrl2
    {
        struct
        {
           unsigned int : 16;      /* Reserved */
          unsigned short  uw_Ctrl2;      /* SPI Control Register 2 */
	    } ctrl2_UW;
        struct
        {
            unsigned int              : 26;
            unsigned int ClkMod_B1    : 1;
            unsigned int SpiEN_B1     : 1;
            unsigned int Master_B1    : 1;
            unsigned int PowerDown_B1 : 1;
            unsigned int Polarity_B1  : 1;
            unsigned int Phase_B1     : 1;
        } ctrl2_ST;
    } ctrl2_UN;

    /* SPICTRL3 Configures interaction between SPI and CPU */
    /* Offset Address: 0x08 */
    union spi_ctrl3
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_Ctrl3;      /* SPI Control Register 3 */
	    } ctrl3_UW;
        struct
        {
            unsigned int               : 26;
            unsigned int TriStateEN_B1 : 1;
            unsigned int DmaReqEN_B1   : 1;
            unsigned int OvrnIntEN_B1  : 1;
            unsigned int RcvrOvrn_B1   : 1;
            unsigned int IntEN_B1      : 1;
            unsigned int IntFlag_B1    : 1;
        } ctrl3_ST;
    } ctrl3_UN;

    /* SPIDAT0 Allows the concurrent transmission and reception of data (Three pin option). Data is shifted out of the MSB (bit 15) and into the LSB (bit 0)*/
    /* Offset Address: 0x0C */
    unsigned int : 16;      /* Reserved */
    unsigned short  uw_Dat0;       /* SPI Shift Register 0 */

    /* SPIDAT1 Allows the concurrent transmission and reception of data (Four pin option). Data is shifted out of the MSB (bit 15) and into the LSB (bit 0)*/
    /* Offset Address: 0x10 */
    unsigned int : 16;      /* Reserved */
    unsigned short  uw_Dat1;       /* SPI Shift Register 1 */

    /* SPIBUF Holds value after reception of configured character length */
    /* Offset Address: 0x14 */
    unsigned short  uw_Img_Reg;    /* Holds the mirror bits for RCVR OVRN and INT FLAG  */
    unsigned short  uw_Buf;        /* SPI Buffer Register */

    /* SPIEMU Used for debug purposes. Mirror of SPIBUF */
    /* Offset Address: 0x18 */
    unsigned int : 16;      /* Reserved */
    unsigned short  uw_Emu;        /* SPI Emulation Register */

    /* SPIPC1 Controls the direction of the different pins */
    /* Offset Address: 0x1C */
    union
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_PC1;        /* SPI Pin Control Register 1 */
	    } pc1_UW;
        struct
        {
            unsigned int              : 27;
            unsigned int SCSDir_B1    : 1;
            unsigned int SOMIDir_B1   : 1;
            unsigned int SIMODir_B1   : 1;
            unsigned int ClkDir_B1    : 1;
            unsigned int EnableDir_B1 : 1;
        } pc1_ST;
    } pc1_UN;

    /* SPIPC2 Reflects the value of the different pins */
    /* Offset Address: 0x20 */
    union
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_PC2;        /* SPI Pin Control Register 2 */
	    } pc2_UW;
        struct
        {
            unsigned int              : 27;
            unsigned int SCSDin_B1    : 1;
            unsigned int SOMIDin_B1   : 1;
            unsigned int SIMODin_B1   : 1;
            unsigned int ClkDin_B1    : 1;
            unsigned int EnableDin_B1 : 1;
        } pc2_ST;
    } pc2_UN;

   /* SPIPC3 Only active if the different pins are configured as general-purpose I/O pins. Indicates the values sent to the different pins */
    /* Offset Address: 0x24 */
    union
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_PC3;        /* SPI Pin Control Register 1 */
	    } pc3_UW;
        struct
        {
            unsigned int               : 27;
            unsigned int SCSDout_B1    : 1;
            unsigned int SOMIDout_B1   : 1;
            unsigned int SIMODout_B1   : 1;
            unsigned int ClkDout_B1    : 1;
            unsigned int EnableDout_B1 : 1;
        } pc3_ST;
    } pc3_UN;

    /* SPIPC4 Only active if the different pins are configured as general-purpose output pins. A value of one written to one of these bits sets the corresponding bit. */
    /* Offset Address: 0x28 */
    union
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_PC4;        /* SPI Pin Control Register 4 */
	    } pc4_UW;
        struct
        {
            unsigned int                  : 27;
            unsigned int SCSDoutSet_B1    : 1;
            unsigned int SOMIDoutSet_B1   : 1;
            unsigned int SIMODoutSet_B1   : 1;
            unsigned int ClkDoutSet_B1    : 1;
            unsigned int EnableDoutSet_B1 : 1;
        } pc4_ST;
    } pc4_UN;

    /* SPIPC5 Only active if the different pins are configured as general-purpose I/O pins. A value of one written to one of these bits clears the corresponding bit. */
    /* Offset Address: 0x2C */
    union
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_PC5;        /* SPI Pin Control Register 5 */
	    } pc5_UW;
        struct
        {
            unsigned int                  : 27;
            unsigned int SCSDoutClr_B1    : 1;
            unsigned int SOMIDoutClr_B1   : 1;
            unsigned int SIMODoutClr_B1   : 1;
            unsigned int ClkDoutClr_B1    : 1;
            unsigned int EnableDoutClr_B1 : 1;
        } pc5_ST;
    } pc5_UN;

    /* SPIPC6 Determines wheter the pins are used as general-purpose I/O pins, or as SPI functional pins. */
    /* Offset Address: 0x30 */
    union
    {
        struct
        {
          unsigned int : 16;      /* Reserved */
          unsigned short  uw_PC6;        /* SPI Pin Control Register 6 */
	    } pc6_UW;
        struct
        {
            unsigned int              : 27;
            unsigned int SCSFun_B1    : 1;
            unsigned int SOMIFun_B1   : 1;
            unsigned int SIMOFun_B1   : 1;
            unsigned int ClkFun_B1    : 1;
            unsigned int EnableFun_B1 : 1;
        } pc6_ST;
    } pc6_UN;

} SPI_ST;

/* #endif  */

extern SPI_ST e_SPI2_ST,e_SPI5_ST;

/*=========== SYSTEM MODULE CONTROL REGISTERS =============================================*/

/* EMU_ST */
typedef volatile struct
{
   ULONG EMUCR1_UL;		/* ff80 */
   BITFIELD :32;			/* ff84 */
   BITFIELD :32;			/* ff88 */
   ULONG EMUCR2_UL;		/* ff8C */
}
EMU_ST;


/*=========== MEMORY PROTECTION UNIT ======================================================*/
typedef volatile struct
{
   ULONG MPUAHR0_UL;
   ULONG MPUALR0_UL;
   ULONG MPUAHR1_UL;
   ULONG MPUALR1_UL;
   ULONG MPUAHR2_UL;
   ULONG MPUALR2_UL;
   ULONG MPUAHR3_UL;
   ULONG MPUALR3_UL;
   ULONG MPUCNTR_UL;
}
MPU_ST;

extern MPU_ST e_MPU_ST;

/*===== FLASH CONTROL REGISTERS ===============================================*/
/* Flash control register structure for F10 devices */
typedef volatile struct
{                      /* Offset */
  ULONG FMPCR_UL;      /* 0x0000 */
  ULONG FMMCR_UL;      /* 0x0004 */
  ULONG FMWTADD_UL;    /* 0x0008 */
  ULONG FMWTDAT_UL;    /* 0x000C */
  ULONG GAP1_UL;       /* 0x0010 */
  ULONG FMPKEY_UL;     /* 0x0014 */
  ULONG GAP2_UL[10];   /* 0x0018 */
  ULONG FMBACC_UL;     /* 0x0040 */
  ULONG FMBC3_UL;      /* 0x0044 */
  ULONG GAP3_UL[13];   /* 0x0048 */
  ULONG FMWAIT_UL;     /* 0x007C */
} FLASHCONTROLF10_ST;

extern FLASHCONTROLF10_ST e_FLASHCONTROLF10_ST;

/* Flash control register structure for F05 devices */
typedef volatile struct
{                      /* Offset */
  BITFIELD :16;        /* 0x0000 */
  UWORD FMBAC1_UW;
  BITFIELD :16;        /* 0x0004 */
  UWORD FMBAC2_UW;
  BITFIELD :16;        /* 0x0008 */
  UWORD FMBSEA_UW;
  BITFIELD :16;        /* 0x000C */
  UWORD FMBSEB_UW;
  BITFIELD :16;        /* 0x0010 */
  UWORD FMBRDY_UW;
  UBYTE GAP1_UB[7148]; /* gap */
  ULONG FMREGOPT_UL;   /* 0x1C00 */
  BITFIELD :32;
  ULONG FMBBUSY_UL;    /* 0x1C08 */
  ULONG FMPKEY_UL;     /* 0x1C0C */
  UBYTE GAP2_UB[3076]; /* gap */
  BITFIELD :16;        /* 0x2814 */
  UWORD FMPRDY_UW;
  UBYTE GAP3_UB[5096]; /* gap */
  BITFIELD :16;        /* 0x3C00 */
  UWORD FMMAC1_UW;
  BITFIELD :16;        /* 0x3C04 */
  UWORD FMMAC2_UW;
  BITFIELD :16;        /* 0x3C08 */
  UWORD FMPAGP_UW;
  BITFIELD :16;        /* 0x3C0C */
  UWORD FMMSTAT_UW;
} SARFMC_ST1;

extern SARFMC_ST1 e_SARFMC_ST1;

/* ------------------------------------------------------------------------- */
/* SCI_ST                                                                    */
/*      Definition of the SCI register map.                                  */

#if defined(_TMS470_BIG) || defined(__big_endian__)

typedef volatile struct
{
    unsigned int : 24;
    union
    {
        unsigned char ccr_UB;
        struct
        {
            unsigned int Stop_B1       : 1;
            unsigned int Parity_B1     : 1;
            unsigned int ParityEN_B1   : 1;
            unsigned int Async_B1      : 1;
            unsigned int Mode_B1       : 1;
            unsigned int CharLength_B3 : 3;
        } ccr_ST;
    } ccr_UN;

    unsigned int : 24;
    union
    {
        unsigned char ctl1_UB;
        struct
        {
            unsigned int           : 4;
            unsigned int Sleep_B1  : 1;
            unsigned int RxRdy_B1  : 1;
            unsigned int RxWake_B1 : 1;
            unsigned int RxEN_B1   : 1;
        } ctl1_ST;
    } ctl1_UN;

    unsigned int : 24;
    union
    {
        unsigned char ctl2_UB;
        struct
        {
            unsigned int Cont_B1     : 1;
            unsigned int LoopBack_B1 : 1;
            unsigned int             : 2;
            unsigned int TxEmpty_B1  : 1;
            unsigned int TxRdy_B1    : 1;
            unsigned int TxWake_B1   : 1;
            unsigned int TxEN_B1     : 1;
        } ctl2_ST;
    } ctl2_UN;

    unsigned int : 24;
    union
    {
        unsigned char ctl3_UB;
        struct
        {
            unsigned int SwReset_B1     : 1;
            unsigned int PowerDown_B1   : 1;
            unsigned int Clock_B1       : 1;
            unsigned int RxIntEN_B1     : 1;
            unsigned int TxIntEN_B1     : 1;
            unsigned int WakeupIntEN_B1 : 1;
            unsigned int BrkdtIntEN_B1  : 1;
            unsigned int RxErrIntEN_B1  : 1;
        } ctl3_ST;
    } ctl3_UN;

    unsigned int : 24;
    union
    {
        unsigned char rxst_UB;
        struct
        {
            unsigned int Busy_B1   : 1;
            unsigned int           : 1;
            unsigned int FE_B1     : 1;
            unsigned int OE_B1     : 1;
            unsigned int PE_B1     : 1;
            unsigned int Wakeup_B1 : 1;
            unsigned int Brkdt_B1  : 1;
            unsigned int RxErr_B1  : 1;
        } rxst_ST;
    } rxst_UN;

    unsigned int  : 24;
    unsigned char HBaud_UB;

    unsigned int  : 24;
    unsigned char MBaud_UB;

    unsigned int  : 24;
    unsigned char LBaud_UB;

    unsigned int  : 24;
    unsigned char RxEmu_UB;

    unsigned int  : 24;
    unsigned char RxBuf_UB;

    unsigned int  : 24;
    unsigned char TxBuf_UB;

    struct
    {
        unsigned int : 24;
        union
        {
            unsigned char pc_UB;
            struct
            {
                unsigned int            : 4;
                unsigned int DataIn_B1  : 1;
                unsigned int DataOut_B1 : 1;
                unsigned int Func_B1    : 1;
                unsigned int DataDir_B1 : 1;
            } pc_ST;
        } pc_UN;
    } pc_PST[3];

} SCI_ST;

#endif

#if defined(_TMS470_LITTLE) || defined(__little_endian__)
#error "Little Endian Not Supported By the SCI Driver
#endif

/* ------------------------------------------------------------------------- */
/* e_SCIn_ST                                                                 */
/*      pointers to the SCI registers, this is defined in the linker command */
/*      file.                                                                */

extern SCI_ST e_SCI1_ST, e_SCI2_ST;


/****************************************************************************/
/* CAN_ST                                                                   */
/*      Definition of the CAN register map.                                 */
/****************************************************************************/
/* #if defined(_TMS470_BIG) || defined(__big_endian__) */

typedef volatile struct scc_st
{
    ULONG CANME_UL;
    ULONG CANMD_UL;
    ULONG CANTRS_UL;
    ULONG CANTRR_UL;
    ULONG CANTA_UL;
    ULONG CANAA_UL;
    ULONG CANRMP_UL;
    ULONG CANRML_UL;
    ULONG CANRFP_UL;
    ULONG CANGAM_UL;

    BITFIELD : 16;
    union
    {
        UWORD CANMC_UW;
        struct
        {
            BITFIELD LNTC_B1     : 1;
            BITFIELD LNTM_B1     : 1;
            BITFIELD SCM_B1      : 1;
            BITFIELD CCR_B1      : 1;
            BITFIELD PDR_B1      : 1;
            BITFIELD DBO_B1      : 1;
            BITFIELD WUBA_B1     : 1;
            BITFIELD CDR_B1      : 1;
            BITFIELD ABO_B1      : 1;
            BITFIELD STM_B1      : 1;
            BITFIELD SRES_B1     : 1;
            BITFIELD WBNR_B5     : 5;
        } CANMC_ST;
    } CANMC_UN;

    ULONG CANBTC_UL;

    union
    {
        UWORD CANES_UL;
        struct
        {
            BITFIELD Notused1   : 7;
            BITFIELD FE_B1      : 1;
            BITFIELD BE_B1      : 1;
            BITFIELD SA1_B1     : 1;
            BITFIELD CRCE_B1    : 1;
            BITFIELD SE_B1      : 1;
            BITFIELD ACKE_B1    : 1;
            BITFIELD BO_B1      : 1;
            BITFIELD EP_B1      : 1;
            BITFIELD EW_B1      : 1;
            BITFIELD Notused2   : 10;
            BITFIELD SMA_B1     : 1;
            BITFIELD CCE_B1     : 1;
            BITFIELD PDA_B1     : 1;
            BITFIELD Notused3   : 1;
            BITFIELD RM_B1      : 1;
            BITFIELD TM_B1      : 1;
        } CANES_ST;
    } CANES_UN;

    BITFIELD notused1 :24;
    UBYTE CANTEC_UL;
    BITFIELD notused2 :24;
    UBYTE CANREC_UL;
    ULONG CANGIFO_UL;
    ULONG CANGIM_UL;
    ULONG CANGIF1_UL;
    ULONG CANMIM_UL;
    ULONG CANMIL_UL;
    ULONG CANOPC_UL;

    union
    {
        UWORD CANTIOC_UL;
        struct
        {
            BITFIELD Notused1       : 28;
            BITFIELD TXFUNC_B1  : 1;
            BITFIELD TXDIR_B1   : 1;
            BITFIELD TXOUT_B1   : 1;
            BITFIELD TXIN_B1    : 1;
        } CANTIOC_ST;
    } CANTIOC_UN;

    union
    {
        UWORD CANRIOC_UL;
        struct
        {
            BITFIELD Notused1       : 28;
            BITFIELD RXFUNC_B1  : 1;
            BITFIELD RXDIR_B1   : 1;
            BITFIELD RXOUT_B1   : 1;
            BITFIELD RXIN_B1    : 1;
        } CANRIOC_ST;
    } CANRIOC_UN;
} SCC_ST;

extern SCC_ST e_SCC_ST, e_SCC2_ST, e_SCC3_ST;

/* #endif  */


/****************************************************************************/
/* CAN Mailboxes                                                            */
/****************************************************************************/

typedef volatile struct can_mbox_st
{
   union {
     struct {
       UWORD  CanMID;             /* CAN message identifier            */
       UWORD  CanMID_low_unused;  /* Not used for Standard Id´s.       */
     } w;
     struct {
       ULONG  CanMID_UL;
     } l;
   } MID_ST;
   union {
     struct {
       UWORD  CanMCF_high_unused; /* Message control flags             */
       UBYTE  CanMCF_mid_unused;  /* Message control flags             */
       UBYTE  CanMCF;             /* Message control flags: DLC        */
     } bw;
     struct {
       ULONG CanMCF_UL;     /* Message control flags: DLC        */
     } l;
   } MCF_ST;
   union {                        /*  union to access CAN data in different */
      struct {                    /*  types                                 */
         ULONG   L0;     /* 1st Message data as long value byte0-3 */
         ULONG   L1;     /* 2nd Message data as long value byte4-7 */
      } l;
      struct {
         UWORD   W0;     /* 1st short of CAN message (byte 0-1)    */
         UWORD   W1;     /* 2nd short of CAN message (byte 2-3)    */
         UWORD   W2;     /* 3rd short of CAN message (byte 4-5)    */
         UWORD   W3;     /* 4th short of CAN message (byte 6-7)    */
      } w;
      struct {
         UBYTE   C0;     /* byte 0 of mailbox data                 */
         UBYTE   C1;     /* byte 1 of mailbox data                 */
         UBYTE   C2;     /* byte 2 of mailbox data                 */
         UBYTE   C3;     /* byte 3 of mailbox data                 */
         UBYTE   C4;     /* byte 4 of mailbox data                 */
         UBYTE   C5;     /* byte 5 of mailbox data                 */
         UBYTE   C6;     /* byte 6 of mailbox data                 */
         UBYTE   C7;     /* byte 7 of mailbox data                 */
      } c;

      UBYTE CanData_UB[7];   /* indexed access to mailbox data         */
   } u;
} CANMBOX;

extern CANMBOX e_CAN_MB[16];
extern CANMBOX e_CAN2_MB[16];
extern CANMBOX e_CAN3_MB[16];


/* #if defined(_TMS470_BIG) || defined(__big_endian__) */
/* Flash control register structure for F05 devices */

/* Flash Control Register FMMAC2 (F05)

	+--+-----+--+---+---+--+--+--+
	|15| ... |05| 04| 03|02|01|00|
  +--+-----+--+---+---+--+--+--+
	|  PSTDBY   |PMPPWR |  BANK  |

*/
#define S_FMMAC2_PSTDBY 0xFFE0  /* 1111 1111 1110 0000  default */
#define S_FMMAC2_PMPPWR 0x0018  /* 0000 0000 0001 1000  default */
/* Flash Control Register FMBAC2 (F05)

  +---+--+-----+--+--+--+--+--+--+--+--+--+
  | 15|14| ... |08|07|06|05|04|03|02|01|00|
  +---+--+-----+--+--+--+--+--+--+--+--+--+
  |OTP|  BSLEEP   |   PAGE    |   READ    |

*/
#define S_FMBAC2_OTP     0x8000
#define S_FMBAC2_BSLEEP  0x7F00  /*0111 1111 0000 0000  default */
#define S_PiplineModeFreqF05_UL  24

/* System Clock > 24 MHz requires 1 Wait State (but it is 24MHz) */
#define S_FMBAC2_PAGE  0 /* 0 Wait States */
#define S_FMBAC2_READ  0 /* 0 Wait States */

typedef volatile struct
{                           /* Offset */
  unsigned int :16;             /* 0x0000 */
  unsigned short FMBAC1_UW;
  unsigned int :16;             /* 0x0004 */
  unsigned short FMBAC2_UW;
  unsigned int :16;             /* 0x0008 */
  unsigned short FMBSEA_UW;
  unsigned int :16;             /* 0x000C */
  unsigned short FMBSEB_UW;
  unsigned int :16;             /* 0x0010 */
  unsigned short FMBRDY_UW;
  unsigned char reserved1_UB[7148]; /* reserved */
  unsigned long FMREGOPT_UL;        /* 0x1C00 */
  unsigned int :32;
  unsigned long FMBBUSY_UL;         /* 0x1C08 */
  unsigned long FMPKEY_UL;          /* 0x1C0C */
  unsigned char reserved2_UB[1008]; /* reserved */
  unsigned int :16;             /* 0x2000 */
  unsigned short FMFSMSTATE_UW;
  unsigned int :16;             /* 0x2004 */
  unsigned short FMFSMSTATUS_UW;
  unsigned int :16;             /* 0x2008 */
  unsigned short FMCR_UW;
  unsigned int :16;             /* 0x200C */
  unsigned short FMHAR_UW;
  unsigned int :16;             /* 0x2010 */
  unsigned short FMLAR_UW;
  unsigned int :16;             /* 0x2014 */
  unsigned short FMDR_UW;
  unsigned int :16;             /* 0x2018 */
  unsigned short FMPES_UW;
  unsigned int :16;             /* 0x201C */
  unsigned short FMMD_UW;
  unsigned int :16;             /* 0x2020 */
  unsigned short FMCS_UW;
  unsigned int :16;             /* 0x2024 */
  unsigned short FMPEVS_UW;
  unsigned int :16;             /* 0x2028 */
  unsigned short FMRMC_UW;
  unsigned int :16;             /* 0x202C */
  unsigned short FMCVS_UW;
  unsigned int :16;             /* 0x2030 */
  unsigned short FMSC_UW;
  unsigned int :16;             /* 0x2034 */
  unsigned short FMPCH_UW;
  unsigned int :16;             /* 0x2038 */
  unsigned short FMPEH_UW;
  unsigned int :16;             /* 0x203C */
  unsigned short FMSPP_UW;
  unsigned int :16;             /* 0x2040 */
  unsigned short FMPEVH_UW;
  unsigned char reserved3_UB[4];    /* reserved */
  unsigned int :16;             /* 0x2048 */
  unsigned short FMCVH_UW;
  unsigned int :16;             /* 0x204C */
  unsigned short FMHC_UW;
  unsigned int :16;             /* 0x2050 */
  unsigned short FMPPW_UW;
  unsigned int :16;             /* 0x2054 */
  unsigned short FMEPW_UW;
  unsigned int :16;             /* 0x2058 */
  unsigned short FMCPW_UW;
  unsigned int :16;             /* 0x205C */
  unsigned short FMPEVA_UW;
  unsigned int :16;             /* 0x2060 */
  unsigned short FMCVA_UW;
  unsigned int :16;             /* 0x2064 */
  unsigned short FMSEP_UW;
  unsigned int :16;             /* 0x2068 */
  unsigned short FMPWC_UW;
  unsigned int :16;             /* 0x206C */
  unsigned short FMWSMMR_UW;
  unsigned int :16;             /* 0x2070 */
  unsigned short FMPLAS_UW;
  unsigned int :16;             /* 0x2074 */
  unsigned short FMELAS_UW;
  unsigned int :16;             /* 0x2078 */
  unsigned short FMHAS_UW;
  unsigned int :16;             /* 0x207C */
  unsigned short FMMPP_UW;
  unsigned int :16;             /* 0x2080 */
  unsigned short FMMEP_UW;
  unsigned int :16;             /* 0x2084 */
  unsigned short FMMCP_UW;
  unsigned int :16;             /* 0x2088 */
  unsigned short FMPC_UW;
  unsigned int :16;             /* 0x208C */
  unsigned short FMSFAL_UW;
  unsigned int :16;             /* 0x2090 */
  unsigned short FMSLAL_UW;
  unsigned int :16;             /* 0x2094 */
  unsigned short FMSAH_UW;
  unsigned int :16;             /* 0x2098 */
  unsigned short FMACL_UW;
  unsigned int :16;             /* 0x209C */
  unsigned short FMACH_UW;
  unsigned int :16;             /* 0x20A0 */
  unsigned short FMPSD_UW;
  unsigned int :16;             /* 0x20A4 */
  unsigned short FMSMT_UW;
  unsigned char reserved4_UB[1884]; /* reserved */
  unsigned int :16;             /* 0x2804 */
  unsigned short FMPTR1_UW;
  unsigned int :16;             /* 0x2808 */
  unsigned short FMPTR2_UW;
  unsigned int :16;             /* 0x280C */
  unsigned short FMPTR3_UW;
  unsigned int :16;             /* 0x2810 */
  unsigned short FMPTR4_UW;
  unsigned int :16;             /* 0x2814 */
  unsigned short FMPRDY_UW;
  unsigned int :16;             /* 0x2818 */
  unsigned short FMCTR0_UW;
  unsigned int :16;             /* 0x281C */
  unsigned short FMCTR1_UW;
  unsigned int :16;             /* 0x2820 */
  unsigned short FMCTR2_UW;
  unsigned int :16;             /* 0x2824 */
  unsigned short FMCTR3_UW;
  unsigned int :16;             /* 0x2828 */
  unsigned short FMDL_UW;
  unsigned char reserved5_UB[5076]; /* reserved */
  unsigned int :16;             /* 0x3C00 */
  unsigned short FMMAC1_UW;
  unsigned int :16;             /* 0x3C04 */
  unsigned short FMMAC2_UW;
  unsigned int :16;             /* 0x3C08 */
  unsigned short FMPAGP_UW;
  unsigned int :16;             /* 0x3C0C */
  unsigned short FMMSTAT_UW;
  unsigned int :16;             /* 0x3C10 */
  unsigned short FMTCR_UW;
} FLASHCONTROLF05_ST;

extern FLASHCONTROLF05_ST e_FLASHCTL0_ST;
/*#endif*/
/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */



