/************************************************************
 *                                                          *
 * Filename: f035.h - Unified F035 header file              *
 *                                                          *
 * Revision History:                                        *
 *                                                          *
 * 22-Feb-2005 SHD  Created according to Platform F035      *
 *                  specification v0.39 (originally called  *
 *                  pf035.h)                                *
 * 08-Aug-2005 SHD  Renamed to f035.h and added PLATFORM    *
 *                  definitions                             *
 * 22-Jun-2006 SHD  Added *_MIN and *_MAX timing limits     *
 *                  reformatted for readability             *
 * 20-Dec-2006 SHD  Added F035_16MB/4MB_WRAPPER hooks       *
 *                                                          *
 ************************************************************/
#if !defined (_F035_H_)
#define _F035_H_

#if !defined (F035)
#define F035
#endif

/* At compile time, this header file needs to know which
 * Flash wrapper to compile to.  As of V0.07 of the F035
 * API, the default flag is F035_4MB_WRAPPER (which can be
 * forced at the command line or in a header file included prior
 * to f035.h, but will be set by default).
 *
 * The F035_16MB_WRAPPER enables compile time support for older
 * devices using the older 16MB flash wrapper.
 */
#if !defined (F035_16MB_WRAPPER)
#if !defined (F035_4MB_WRAPPER)
#define F035_4MB_WRAPPER
#endif /* !defined (F035_4MB_WRAPPER) */
#else
#if !defined (F035_4MB_WRAPPER)
#undef  F035_4MB_WRAPPER
#endif /* !defined (F035_4MB_WRAPPER) */
#endif /* !defined (F035_16MB_WRAPPER) */

/* MAIN_ADDR_OFF specifies the offset of the main
 * flash address (not including offset imposed by
 * memory map register)
 */
#define MAIN_ADDR_OFF    0x00000000

/* MAIN_ADDR_MSK specifies the bit mask for determining
 * all address bits exclusive of the offest imposed by
 * the memory map register
 */
#define MAIN_ADDR_MSK    0x07ffffff

/* MAIN_ADDR_MSK specifies the bit mask for determining
 * all address bits exclusive of the offest imposed by
 * the memory map register
 */
#define MAX_FLASH_BANKS  0x8

/* MAIN_ADDR_MSK specifies the bit mask for determining
 * all address bits exclusive of the offest imposed by
 * the memory map register
 */
#define OTP_SECT_SIZE  0x800

/* MAIN_ADDR_MSK specifies the bit mask for determining
 * all address bits exclusive of the offest imposed by
 * the memory map register
 */
#define OTP_MAX_RNGE_MSK ((MAX_FLASH_BANKS*OTP_SECT_SIZE)-1)

/* MAIN_RNGE_MSK specifies the bit mask for including
 * all flash address bits within the range supported
 * by the wrapper.
 */
#if defined (F035_16MB_WRAPPER)
#define MAIN_RNGE_MSK    0x007fffff
#else /* F035_4MB_WRAPPER */
#define MAIN_RNGE_MSK    0x003fffff
#endif

/* MAIN_TYPE_MSK specifies the bit mask for determining
 * the memory type of the address range (main/ECC/PAR/COTP/TIOTP)
 */
#if defined (F035_16MB_WRAPPER)
#define MAIN_TYPE_MSK    0x03800000
#else /* F035_4MB_WRAPPER */
#define MAIN_TYPE_MSK    0x00600000
#endif

/* COTP_ADDR_OFF specifies the offset of the COTP
 * flash bits (i.e. non-ECC/Parity)
 */
#if defined (F035_16MB_WRAPPER)
#define COTP_ADDR_OFF    0x01000000
#else /* F035_4MB_WRAPPER */
#define COTP_ADDR_OFF    0x00600000
#endif

/* TIOTP_ADDR_OFF specifies the offset of the TI OTP
 * flash bits (i.e. non-ECC/Parity)
 */
#if defined (F035_16MB_WRAPPER)
#define TIOTP_ADDR_OFF   0x01800000
#else /* F035_4MB_WRAPPER */
#define TIOTP_ADDR_OFF   0x00604000
#endif

/* MAIN_ECC_OFF specifies the offset of the MAIN
 * flash's ECC bits
 */
#if defined (F035_16MB_WRAPPER)
#define MAIN_ECC_OFF     0x02000000
#else /* F035_4MB_WRAPPER */
#define MAIN_ECC_OFF     0x00400000
#endif

/* MAIN_PAR_OFF specifies the offset of the MAIN
 * flash's parity bits
 */
#if defined (F035_16MB_WRAPPER)
#define MAIN_PAR_OFF     0x02000000
#else /* F035_4MB_WRAPPER */
#define MAIN_PAR_OFF     0x00400000
#endif

/* COTP_ECC_OFF specifies the offset of the COTP
 * flash's ECC bits
 */
#if defined (F035_16MB_WRAPPER)
#define COTP_ECC_OFF     0x03000000
#else /* F035_4MB_WRAPPER */
#define COTP_ECC_OFF     0x00608000
#endif


/* TIOTP_ECC_OFF specifies the offset of the TI OTP
 * flash's ECC bits
 */
#if defined (F035_16MB_WRAPPER)
#define TIOTP_ECC_OFF    0x03800000
#else /* F035_4MB_WRAPPER */
#define TIOTP_ECC_OFF    0x0060c000
#endif

/* COTP_PAR_OFF specifies the offset of the COTP
 * flash's parity bits
 */
#if defined (F035_16MB_WRAPPER)
#define COTP_PAR_OFF     0x03000000
#else /* F035_4MB_WRAPPER */
#define COTP_PAR_OFF     0x00608000
#endif

/* TIOTP_PAR_OFF specifies the offset of the TI OTP
 * flash's parity bits
 */
#if defined (F035_16MB_WRAPPER)
#define TIOTP_PAR_OFF    0x03800000
#else /* F035_4MB_WRAPPER */
#define TIOTP_PAR_OFF    0x0060c000
#endif

/* ECC_ADDR_TRANS_SHFT is the bit shift for calculating
 * the amount of ECC flash which corresponds to a given
 * main flash range
 */
#if defined (F035_16MB_WRAPPER)
#define ECC_ADDR_TRANS_SHFT 2
#else /* F035_4MB_WRAPPER */
#define ECC_ADDR_TRANS_SHFT 1
#endif

/* ECC_ADDR_INCR is the number of bytes to increment
 * when reading/verifying ECC flash
 */
#if defined (F035_16MB_WRAPPER)
#define ECC_ADDR_INCR 8
#else /* F035_4MB_WRAPPER */
#define ECC_ADDR_INCR 4
#endif

/* PAR_ADDR_TRANS_SHFT is the bit shift for calculating
 * the amount of parity flash which corresponds to a given
 * main flash range
 */
#if defined (F035_16MB_WRAPPER)
#define PAR_ADDR_TRANS_SHFT 3
#else /* F035_4MB_WRAPPER */
#define PAR_ADDR_TRANS_SHFT 1
#endif

/* PAR_ADDR_INCR is the number of bytes to increment
 * when reading/verifying parity flash
 */
#if defined (F035_16MB_WRAPPER)
#define PAR_ADDR_INCR 16
#else /* F035_4MB_WRAPPER */
#define PAR_ADDR_INCR 4
#endif

/* COTP_ADDR_TRANS translates the UINT32 * address a
 * into it's correct address within the Customer OTP,
 * based on the core select value c
 */
#define COTP_ADDR_TRANS_U32(a,c) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_ADDR_OFF+((UINT32)a&(OTP_SECT_SIZE-1))+(c*OTP_SECT_SIZE)); }
#define COTP_ADDR_TRANS_U16(a,c) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_ADDR_OFF+((UINT32)a&(OTP_SECT_SIZE-1))+(c*OTP_SECT_SIZE)); }
#define COTP_ADDR_TRANS_U8(a,c)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_ADDR_OFF+((UINT32)a&(OTP_SECT_SIZE-1))+(c*OTP_SECT_SIZE)); }

/* TIOTP_ADDR_TRANS translates the UINT32 * address 'a'
 * into it's correct address within the TI OTP,
 * based on the core select value 'c'
 */
#define TIOTP_ADDR_TRANS_U32(a,c) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_ADDR_OFF+((UINT32)a&(OTP_SECT_SIZE-1))+(c*OTP_SECT_SIZE)); }
#define TIOTP_ADDR_TRANS_U16(a,c) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_ADDR_OFF+((UINT32)a&(OTP_SECT_SIZE-1))+(c*OTP_SECT_SIZE)); }
#define TIOTP_ADDR_TRANS_U8(a,c)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_ADDR_OFF+((UINT32)a&(OTP_SECT_SIZE-1))+(c*OTP_SECT_SIZE)); }

/* PAR_ADDR_TRANS translates the UINT32 * address 'a'
 * into it's correct address within the main flash's
 * Parity region
 */
#define PAR_ADDR_TRANS_U32(a) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)MAIN_PAR_OFF+(((UINT32)a&MAIN_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }
#define PAR_ADDR_TRANS_U16(a) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)MAIN_PAR_OFF+(((UINT32)a&MAIN_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }
#define PAR_ADDR_TRANS_U8(a)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)MAIN_PAR_OFF+(((UINT32)a&MAIN_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }

/* ECC_ADDR_TRANS translates the UINT32 * address 'a'
 * into it's correct address within the main flash's
 * ECC region
 */
#define ECC_ADDR_TRANS_U32(a) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)MAIN_ECC_OFF+(((UINT32)a&MAIN_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }
#define ECC_ADDR_TRANS_U16(a) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)MAIN_ECC_OFF+(((UINT32)a&MAIN_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }
#define ECC_ADDR_TRANS_U8(a)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)MAIN_ECC_OFF+(((UINT32)a&MAIN_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }

/* COTP_PAR_TRANS translates the UINT32 * COTP address 'a'
 * into it's correct address within the COTP flash's
 * Parity region
 */
#define COTP_PAR_TRANS_U32(a) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_PAR_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }
#define COTP_PAR_TRANS_U16(a) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_PAR_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }
#define COTP_PAR_TRANS_U8(a)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_PAR_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }

/* TIOTP_PAR_TRANS translates the UINT32 * TIOTP address 'a'
 * into it's correct address within the TIOTP flash's
 * Parity region
 */
#define TIOTP_PAR_TRANS_U32(a) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_PAR_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }
#define TIOTP_PAR_TRANS_U16(a) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_PAR_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }
#define TIOTP_PAR_TRANS_U8(a)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_PAR_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>PAR_ADDR_TRANS_SHFT)); }

/* COTP_ECC_TRANS translates the UINT32 * COTP address 'a'
 * into it's correct address within the COTP flash's
 * ECC region
 */
#define COTP_ECC_TRANS_U32(a) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_ECC_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }
#define COTP_ECC_TRANS_U16(a) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_ECC_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }
#define COTP_ECC_TRANS_U8(a)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)COTP_ECC_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }

/* TIOTP_ECC_TRANS translates the UINT32 * TIOTP address 'a'
 * into it's correct address within the TIOTP flash's
 * ECC region
 */
#define TIOTP_ECC_TRANS_U32(a) { a=(UINT32 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_ECC_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }
#define TIOTP_ECC_TRANS_U16(a) { a=(UINT16 *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_ECC_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }
#define TIOTP_ECC_TRANS_U8(a)  { a=(UBYTE  *)(((UINT32)a&~MAIN_ADDR_MSK)+(UINT32)TIOTP_ECC_OFF+(((UINT32)a&OTP_MAX_RNGE_MSK)>>ECC_ADDR_TRANS_SHFT)); }

/************************************************************
 * The flash module uses several registers to control the   *
 * various modes of operation. All flash registers are      *
 * accessed in privilege mode only through VBUS interface.  *
 * 8/16/32 bit access to any register is allowed. Flash     *
 * registers are subdivided into four categories:           *
 *                                                          *
 *   o Configuration Registers:                             *
 *       These registers mainly control and configure the   *
 *       flash wrapper for various operating modes such as  *
 *       flash read mode, SECDED, interrupt, wait states    *
 *       and low power mode.                                *
 *   o Voltage Registers:                                   *
 *       These registers provide direct interface to the    *
 *       pump module during software algorithm flash        *
 *       program and erase.                                 *
 *   o Command Registers:                                   *
 *       These registers provide a direct interface to bank *
 *       module during software algorithm flash program and *
 *       erase.                                             *
 *   o FSM Control Registers:                               *
 *       These registers are used to control FSM during FSM *
 *       program and erase.                                 *
 *                                                          *
 * There are also two types of registers, local and global. *
 * Local registers are dedicated to a datapath module and   *
 * require the bank identifier when addressing them. Global *
 * registers support the remainder of the flash module.     *
 ************************************************************/

/************************************************************
 * Table 1-21. Flash registers starting offset address      *
 *  Register Category        Starting Offset Address        *
 *  ======================   ===========================    *
 *  Configuration Registers  0x000                          *
 *  Voltage Registers        0x080                          *
 *  Command Registers        0x100                          *
 *  FSM Control Registers    0x200                          *
 ************************************************************/

/* Configuration Registers - address offset 0x000-0x07f     */

/* FRDCNTL[31:25] Reserved
 * FRDCNTL[24]=IFLUSHEN (new in F035_4MB_WRAPPER)
 * FRDCNTL[23:17] Reserved
 * FRDCNTL[16]=IDLEN (new in F035_4MB_WRAPPER)
 * FRDCNTL[15:12] Reserved
 * FRDCNTL[11:8]=RWAIT (Fosc = Fsys / (2 * (WS+1)))
 * FRDCNTL[7:5]=Reserved
 * FRDCNTL[4]=ASWSTEN
 * FRDCNTL[3]=PRTY
 * FRDCNTL[2:0]=RM (read mode)
 */
#define FRDCNTL        0x0000/4

/* FSPRD[31:2] Reserved
 * FSPRD[1]=RM1
 * FSPRD[0]=RM0
 */
#define FSPRD          0x0004/4

/* FEDACCTRL1[31:20]=Reserved
 * FEDACCTRL1[19:16]=EDACCMODE (new in F035_4MB_WRAPPER)
 * FEDACCTRL1[15:11]=Reserved
 * FEDACCTRL1[10]=EOFEN
 * FEDACCTRL1[9]=EZFEN
 * FEDACCTRL1[8]=EPEN
 * FEDACCTRL1[7:6]=Reserved
 * FEDACCTRL1[5]=EOCV
 * FEDACCTRL1[4]=EZCV
 * FEDACCTRL1[3:0]=EDACEN[3:0]
 */
#define FEDACCTRL1     0x0008/4

/* FEDACCTRL2[31:16]=Reserved
 * FEDACCTRL2[15:0]=SEC_THRESHOLD[15:0]
 */
#define FEDACCTRL2     0x000c/4

/* FCOR_ERR_CNT[31:16]=Reserved
 * FCOR_ERR_CNT[15:0]=SEC_THRESHOLD[15:0]
 */
#define FCOR_ERR_CNT   0x0010/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FEDACCNT FCOR_ERR_CNT

/* FCOR_ERR_ADD[31:27]=Reserved
 * FCOR_ERR_ADD[26:0]=CERRADDR[26:0]
 */
#define FCOR_ERR_ADD   0x0014/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FCERRADDR FCOR_ERR_ADD

/* The 16MB wrapper definition is:
 *   FCOR_ERR_POS[31:9]=Reserved
 *   FCOR_ERR_POS[8]=ERRTYPE
 *   FCOR_ERR_POS[7]=SBZ
 *   FCOR_ERR_POS[6]=SBZ
 *   FCOR_ERR_POS[5:0]=SERRPOSITION[5:0]
 *
 * The 4MB wrapper definition is:
 *   FCOR_ERR_POS[31:10]=Reserved
 *   FCOR_ERR_POS[9]=ADDERR
 *   FCOR_ERR_POS[8]=ECCERR
 *   FCOR_ERR_POS[7:0]=SERR_POS[7:0]
 */
#define FCOR_ERR_POS  0x0018/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FCERRPOSITION FCOR_ERR_POS

/* The 16MB wrapper definition is:
 *   FEDACSTATUS[31:3]=Reserved
 *   FEDACSTATUS[2]=EOFSTATUS
 *   FEDACSTATUS[1]=EZFSTATUS
 *   FEDACSTATUS[0]=EPSTATUS
 *
 * The 4MB wrapper definition is:
 *   FEDACSTATUS[31:13]=Reserved
 *   FEDACSTATUS[12]=DUNCERR
 *   FEDACSTATUS[11]=ADDTAGERR
 *   FEDACSTATUS[10]=ADDPARERR
 *   FEDACSTATUS[9]=BUFPARERR
 *   FEDACSTATUS[8]=ECCMULERR
 *   FEDACSTATUS[7]=COM1MALGOOD
 *   FEDACSTATUS[6]=COM0MALGOOD
 *   FEDACSTATUS[5]=ECC1MALERR
 *   FEDACSTATUS[4]=ECC0MALERR
 *   FEDACSTATUS[3]=DCORERR
 *   FEDACSTATUS[2]=ERRONEFLG
 *   FEDACSTATUS[1]=ERRZEROFLG
 *   FEDACSTATUS[0]=ERRPRFFLG
 */
#define FEDACSTATUS    0x001c/4

/* The 16MB wrapper definition is:
 *   FUERRADDR[31:26]=Reserved
 *   FUERRADDR[25:0]=UERRADDR[25:0]
 *
 * The 4MB wrapper definition is:
 *   FUNC_ERR_ADD[31:0]=UNC_ERR_ADD[31:0]
 */
#define FUNC_ERR_ADD      0x0020/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FUERRADDR   FUNC_ERR_ADD

/* FEDACSDIS[31:29]=bankID1_inverse[2:0]
 * FEDACSDIS[28]=Reserved
 * FEDACSDIS[27:24]=SectorID1_inverse[3:0]
 * FEDACSDIS[23:21]=bankID1[2:0]
 * FEDACSDIS[20]=Reserved
 * FEDACSDIS[19:16]=SectorID1[3:0]
 * FEDACSDIS[15:13]=bankID0_inverse[2:0]
 * FEDACSDIS[12]=Reserved
 * FEDACSDIS[11:8]=SectorID0_inverse[3:0]
 * FEDACSDIS[7:5]=bankID0[2:0]
 * FEDACSDIS[4]=Reserved
 * FEDACSDIS[3:0]=SectorID0[3:0]
 */
#define FEDACSDIS      0x0024/4

/* FBPROT[31:1]=Reserved
 * FBPROT[0]=PROTL1DIS
 */
#define FBPROT         0x0030/4

/* FBSE[31:16]=Reserved
 * FBSE[15:0]=BSE[15:0]
 */
#define FBSE           0x0034/4

/* FBBUSY[31:8]=Reserved
 * FBBUSY[7:0]=BUSY[7:0]
 */
#define FBBUSY         0x0038/4

/* FBAC[31:28]=Reserved
 * FBAC[27:24]=BSLEEP[3:0]
 * FBAC[23:16]=OTPPROTDIS[7:0]
 * FBAC[15:8]=BAGP[7:0]
 * FBAC[7:6]=Reserved
 * FBAC[5:0]=BSTDBY[5:0]
 */
#define FBAC           0x003c/4

/* FBFALLBACK[31:28]=Reserved
 * FBFALLBACK[27:24]=FSM_PWRSAV (new in F035_4MB_WRAPPER)
 * FBFALLBACK[23:20]=Reserved
 * FBFALLBACK[19:16]=VBUS_PWRSAV (new in F035_4MB_WRAPPER)
 * FBFALLBACK[15:14]=BANKPWR7[1:0]
 * FBFALLBACK[13:14]=BANKPWR6[1:0]
 * FBFALLBACK[11:10]=BANKPWR5[1:0]
 * FBFALLBACK[9:8]=BANKPWR4[1:0]
 * FBFALLBACK[7:6]=BANKPWR3[1:0]
 * FBFALLBACK[5:4]=BANKPWR2[1:0]
 * FBFALLBACK[3:2]=BANKPWR1[1:0]
 * FBFALLBACK[1:0]=BANKPWR0[1:0]
 */
#define FBFALLBACK     0x0040/4

/* FBPRDY[31:16]=Reserved
 * FBPRDY[15]=PUMPRDY
 * FBPRDY[14:8]=Reserved
 * FBPRDY[7:0]=BANKRDY[7:0]
 */
#define FBPRDY         0x0044/4

/* FPAC1[31:27]=Reserved
 * FPAC1[26:16]=PSLEEP[10:0]
 * FPAC1[15:2]=Reserved
 * FPAC1[1:0]=PUMPPWR[1:0]
 */
#define FPAC1          0x0048/4

/* FPAC2[31:16]=Reserved
 * FPAC2[15:0]=PAGP[15:0]
 */
#define FPAC2          0x004c/4

/* FMAC[31:3]=Reserved
 * FMAC[2:0]=BANK[2:0]
 */
#define FMAC           0x0050/4

/* FMSTAT[31:15]=Reserved
 * FMSTAT[14]=ILA
 * FMSTAT[13]=DBT
 * FMSTAT[12]=PGV
 * FMSTAT[11]=PCV
 * FMSTAT[10]=EV
 * FMSTAT[9]=CV
 * FMSTAT[8]=BUSY
 * FMSTAT[7]=ERS
 * FMSTAT[6]=PGM
 * FMSTAT[5]=INVDAT
 * FMSTAT[4]=CSTAT/MAXPLS
 * FMSTAT[3]=5VSTAT
 * FMSTAT[2]=ESUSP
 * FMSTAT[1]=PSUSP
 * FMSTAT[0]=SLOCK
 */
#define FMSTAT         0x0054/4

/* FEMU_DMSW[31:16]=EMU_DMW[63:48]
 * FEMU_DMSW[15:0]=EMU_DMW[47:32]
 */
#define FEMU_DMSW      0x0058/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FEMUDMSW       FEMU_DMSW

/* FEMU_DLSW[31:16]=EMU_DMW[31:16]
 * FEMU_DLSW[15:0]=EMU_DMW[15:0]
 */
#define FEMU_DLSW      0x005c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FEMUDLSW       FEMU_DLSW

/* FEMU_ECC[31:8]=Reserved
 * FEMU_ECC[7:0]=EMUECC[7:0]
 */
#define FEMU_ECC       0x0060/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FEMUECC        FEMU_ECC

/* FLOCK[31:16]=Reserved
 * FLOCK[15:0]=ENCOM[15:0]
 */
#define FLOCK          0x0064/4

/* The 16MB wrapper definition is:
 *   FEMU_ADDR[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FEMU_ADDR[31:22]=Reserved
 *   FEMU_ADDR[21:0]=EMU_ADDR[21:0]
 */
#define FEMU_ADDR      0x0068/4

/* The 16MB wrapper definition is:
 *   FDIAGCTRL[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FDIAGCTRL[31:25]=Reserved
 *   FDIAGCTRL[24]=DIAG_TRIG
 *   FDIAGCTRL[23:20]=Reserved
 *   FDIAGCTRL[19:16]=DIAG_EN_KEY[3:0]
 *   FDIAGCTRL[15:13]=Reserved
 *   FDIAGCTRL[12]=DIAGECCSEL
 *   FDIAGCTRL[11:10]=Reserved
 *   FDIAGCTRL[9:8]=DIAG_BUF_SEL[1:0]
 *   FDIAGCTRL[7:3]=Reserved
 *   FDIAGCTRL[2:0]=DIAGMODE[2:0]
 */
#define FDIAGCTRL      0x006c/4

/* The 16MB wrapper definition is:
 *   FRAWDATAH[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FRAWDATAH[31:0]=RAWDATA[63:32]
 */
#define FRAWDATAH      0x0070/4

/* The 16MB wrapper definition is:
 *   FRAWDATAL[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FRAWDATAL[31:0]=RAWDATA[31:0]
 */
#define FRAWDATAL      0x0074/4

/* The 16MB wrapper definition is:
 *   FRAWECC[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FRAWECC[31:8]=Reserved
 *   FRAWECC[7:0]=RAWECC[7:0]
 */
#define FRAWECC        0x0078/4

/* The 16MB wrapper definition is:
 *   FPAR_OVR[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FPAR_OVR[31:17]=Reserved
 *   FPAR_OVR[16]=BNKINVPAR
 *   FPAR_OVR[15:12]=PAR_DIS_KEY[3:0]
 *   FPAR_OVR[11:9]=PAR_OVR_KEY[2:0]
 *   FPAR_OVR[8]=ADDINVPAR
 *   FPAR_OVR[7:0]=DAT_INV_PAR
 */
#define FPAR_OVR       0x007c/4

/* Voltage Registers - address offset 0x080-0x0ff */

/* FVREADCT[31:4]=Reserved
 * FVREADCT[3:0]=VREADCT[3:0]
 */
#define FVREADCT       0x0080/4

/* FVHVCT1[31:24]=VHVCT_P
 * FVHVCT1[23:16]=VHVCT_E
 * FVHVCT1[15:8]=VHVCT_C
 * FVHVCT1[7:0]=VHVCT_PV
 */
#define FVHVCT1        0x0084/4

/* FVHVCT2[31:12]=Reserved
 * FVHVCT2[11:8]=WCT
 * FVHVCT2[7:0]=VHVCT_READ
 */
#define FVHVCT2        0x0088/4

/* FVNVCT[31:4]=Reserved
 * FVNVCT[3:0]=VNVCT[3:0]
 */
#define FVNVCT         0x008c/4

/* FVPPCT[31:4]=Reserved
 * FVPPCT[15:12]=VPPCT_P
 * FVPPCT[11:8]=VPPCT_E
 * FVPPCT[7:4]=VPPCT_C
 * FVPPCT[3:0]=Reserved
 */
#define FVPPCT         0x0090/4

/* FVWLCT[31:4]=Reserved
 * FVWLCT[15:12]=VWLCT_E
 * FVWLCT[11:8]=VWLCT_C
 * FVWLCT[7:4]=VWLCT_EV
 * FVWLCT[3:0]=Reserved
 */
#define FVWLCT         0x0094/4

/* The 16MB wrapper definition is:
 *   FEFUSE[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FEFUSE[31:8]=Reserved
 *   FEFUSE[7:4]=EFUSE_SOFT_EN[3:0]
 *   FEFUSE[3:0]=EFUSE_EN[3:0]
 */
#define FEFUSE         0x0098/4

/* Command Registers - address offset 0x100-0x17f */

/* FBSTROBES[31:17]=Reserved
 * FBSTROBES[16]=FCLKEN
 * FBSTROBES[15:9]=VWLCT_C
 * FBSTROBES[8]=CTRLENZ
 * FBSTROBES[7]=Reserved
 * FBSTROBES[6]=NOCOLRED
 * FBSTROBES[5]=PRECOL
 * FBSTROBES[4]=ENGR
 * FBSTROBES[3]=OTP
 * FBSTROBES[2]=TEZ
 * FBSTROBES[1:0]=Reserved
 */
#define FBSTROBES      0x0100/4

/* FPSTROBES[31:9]=Reserved
 * FPSTROBES[8]=EXECUTEZ
 * FPSTROBES[7:2]=Reserved
 * FPSTROBES[1]=3VPWRDNZ
 * FPSTROBES[0]=5VPWRDNZ
 */
#define FPSTROBES      0x0104/4

/* FBMODE[31:3]=Reserved
 * FBMODE[2:0]=MODE[2:0]
 */
#define FBMODE         0x0108/4

/* FTCR[31:7]=Reserved
 * FTCR[6:0]=TCR[6:0]
 */
#define FTCR           0x010c/4

/* FADDR[31:20]=Reserved
 * FADDR[19:0]=ADDR[19:0]
 */
#define FADDR          0x0110/4

/* FWRITE[31:16]=Reserved
 * FWRITE[15:0]=WDATA[15:0]
 */
#define FWRITE         0x0114/4

/* FCBIT_SEL[31:1]=Reserved
 * FCBIT_SEL[0]=CBIT_SEL
 */
#define FCBIT_SEL      0x0118/4

/* FTCTRL[31:2]=Reserved
 * FTCTRL[8]=WP_EN
 * FTCTRL[7:2]=Reserved
 * FTCTRL[1]=TESTEN
 * FTCTRL[0]=reserved
 */
#define FTCTRL         0x011c/4

/* The 16MB wrapper definition is:
 *   FWPWRITE0[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FWPWRITE0[31:0]=WPDATA[31:0]
 */
#define FWPWRITE0      0x0120/4

/* The 16MB wrapper definition is:
 *   FWPWRITE1[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FWPWRITE1[31:0]=WPDATA[63:32]
 */
#define FWPWRITE1      0x0124/4

/* The 16MB wrapper definition is:
 *   FWPWRITE2[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FWPWRITE2[31:0]=WPDATA[95:64]
 */
#define FWPWRITE2      0x0128/4

/* The 16MB wrapper definition is:
 *   FWPWRITE3[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FWPWRITE3[31:0]=WPDATA[127:96]
 */
#define FWPWRITE3      0x012c/4

/* The 16MB wrapper definition is:
 *   FWPWRITE4[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FWPWRITE4[31:16]=Reserved
 *   FWPWRITE4[15:0]=WPDATA[143:128]
 */
#define FWPWRITE4      0x0130/4

/* FSM Control Registers - address offset 0x200-0x297 */

/* FSM_STATE[15:12]=Reserved
 * FSM_STATE[11]=FSM_CTRLENZ.
 * FSM_STATE[10]=FSM_EXECUTEZ
 * FSM_STATE[9]=Reserved
 * FSM_STATE[8]=FSM is active.
 * FSM_STATE[7]=ProgramENGR Active.
 * FSM_STATE[6]=ProgramOTP Active.
 * FSM_STATE[5:0]= Reserved
 */
#define FSM_STATE      0x0204/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSTATE       FSM_STATE

/* FSM_STATUS[15:4]=Reserved
 * FSM_STATUS[3]=CODFAIL (new in F035_4MB_WRAPPER)
 * FSM_STATUS[2]=NONOP
 * FSM_STATUS[1]=OVRPULCNT
 * FSM_STATUS[0]=INVDAT
 */
#define FSM_STATUS      0x0208/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSTATUS      FSM_STATUS

/* FSM_COMMAND[31:6]=Reserved
 * FSM_COMMAND[5:0]=State machine command
 */
#define FSM_COMMAND        0x020c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMCMND        FSM_COMMAND

/* FSM_PE_OSU[31:16]=Reserved
 * FSM_PE_OSU[15:8]=Setup time for program.
 * FSM_PE_OSU[7:0]=Setup time for erase.
 */
#define FSM_PE_OSU     0x0210/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPESETUP     FSM_PE_OSU

/* FSM_C_OSU[31:16]=Reserved
 * FSM_C_OSU[15:12]=Number of HCLKs to count V5STAT
 * FSM_C_OSU[11:0]=Setup for compaction.
 */
#define FSM_C_OSU      0x0214/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMCSETUP      FSM_C_OSU

/* FSM_PE_VSU[31:16]=Reserved
 * FSM_PE_VSU[15:8]=Setup time for program verify.
 * FSM_PE_VSU[7:0]=Setup time for erase verify.
 */
#define FSM_PE_VSU     0x0218/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPVEVSETUP   FSM_PE_VSU

/* FSM_CMP_VSU[31:16]=Reserved
 * FSM_CMP_VSU[15:12]=Address/EXECUTEZ access time all verify modes
 * FSM_CMP_VSU[11:0]=Setup time for compact verify.
 */
#define FSM_CMP_VSU    0x021c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMCVSETUP     FSM_CMP_VSU

/* FSM_EX_VA[31:8]=Reserved
 * FSM_EX_VA[7:0]=EXE_VALID (Access time for program verify and erase verify)
 */
#define FSM_EX_VA      0x0220/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPVEVACCESS  FSM_EX_VA

/* FSM_RD_H[31:8]=Reserved
 * FSM_RD_H[7:0]=RD_H[7:0]
 */
#define FSM_RD_H       0x0224/4

/* FSM_PC_OH[31:16]=Reserved
 * FSM_PC_OH[15:8]=Hold time for program.
 * FSM_PC_OH[7:0]=Hold time for compaction.
 */
#define FSM_PC_OH      0x0228/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPCHOLD      FSM_PC_OH

/* FSM_ERA_OH[31:16]=Reserved
 * FSM_ERA_OH[15:0]=Hold time for erase.
 */
#define FSM_ERA_OH     0x022c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMEHOLD       FSM_ERA_OH

/* FSM_SAV_PPUL[31:12]=Reserved
 * FSM_SAV_PPUL[11:0]=Saved program pulses for a program suspend.
 */
#define FSM_SAV_PPUL   0x0230/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSAVEDPP     FSM_SAV_PPUL

/* FSM_PE_VH[31:16]=Reserved
 * FSM_PE_VH[15:8]=Hold time for program verify
 * FSM_PE_VH[7:0]=Hold time for erase verify
 */
#define FSM_PE_VH      0x0234/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPVEVHOLD    FSM_PE_VH

/* FSM_VWL_STEP[31:24]=Reserved
 * FSM_VWL_STEP[23:20]=Current repeat countdown value
 * FSM_VWL_STEP[19:16]=Current value of VWLCT during compaction
 * FSM_VWL_STEP[15:12]=Current VWLCT value saved during erase suspend
 * FSM_VWL_STEP[11:8]=Repeat same VWLCT n+1 times
 * FSM_VWL_STEP[7:4]=Ending value of VWLCT during compaction
 * FSM_VWL_STEP[3:0]=Starting value of VWLCT during compaction.
 */
#define FSM_VWL_STEP   0x0238/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMVWLCMPCTCT  FSM_VWL_STEP

/* FSM_SWITCH_PNT[31:12]=Reserved
 * FSM_SWITCH_PNT[11:0]=Switch point for COD (SWITCH_PNT)
 */
#define FSM_SWITCH_PNT  0x023c/4

/* FSM_PRG_PW[31:16]=Reserved
 * FSM_PRG_PW[15:0]=Program pulse width
 */
#define FSM_PRG_PW     0x0240/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPWIDTH      FSM_PRG_PW

/* FSM_ERA_PW[31:0]=Erase pulse width
 */
#define FSM_ERA_PW     0x0244/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMEWIDTH      FSM_ERA_PW

/* FSM_CMP_PW[15:0]=Compaction pulse width
 */
#define FSM_CMP_PW     0x0248/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMCWIDTH      FSM_CMP_PW

/* The 16MB wrapper definition is:
 *   FSM_CMP_SEC_SU[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FSM_CMP_SEC_SU[31:6]=Reserved
 *   FSM_CMP_SEC_SU[5:0]=Compact Sector Setup Time
 */
#define FSM_CMP_SEC_SU 0x024c/4

/* The 16MB wrapper definition is:
 *   FSM_SAV_COD[31:0]=Reserved
 *
 * The 4MB wrapper definition is:
 *   FSM_SAV_COD[31:17]=Reserved
 *   FSM_SAV_COD[16]=CODFAILPCV
 *   FSM_SAV_COD[15]=SAVDIDCV
 *   FSM_SAV_COD[14]=DIDCV
 *   FSM_SAV_COD[13]=SAVECODMOD
 *   FSM_SAV_COD[12]=CODMODE
 *   FSM_SAV_COD[11:0]=SAVE_CMP_PUL
 */
#define FSM_SAV_COD    0x0250/4

/* FSM_SAV_ERS_PUL[15:12]=Reserved
 * FSM_SAV_ERS_PUL[11:0]=Saved erase pulses for an erase suspend.
 */
#define FSM_SAV_ERS_PUL 0x0254/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSAVEDEP      FSM_SAV_ERS_PUL

/* FSMTIMER[31:0]=Timer value for above time delays
 */
#define FSMTIMER       0x0258/4

/* FSM_MODE[31:18]=Reserved
 * FSM_MODE[17:16]=WSM submode for program resume
 * FSM_MODE[15:14]=WSM submode for erase resume
 * FSM_MODE[13:12]=WSM submode
 *   00- normal
 *   01- PRECOL = 1
 *   10- PRECOL = 0; OTP sector selected
 *   11- PRECOL = 0; ENGR sector selected
 * FSM_MODE[11:9]=Program WSM Command for Program Resume.
 *   000 - program
 *   001 - erase sector
 *   010 - program sector
 *   011 - erase test
 *   100 - erase bank
 *   101 - precondition verify
 *   110 - erase verify
 *   111 - compaction verify
 * FSM_MODE[8:6]=Erase WSM Mode for Erase Resume
 *   000 - read
 *   001 - program verify
 *   010 - program
 *   011 - erase verify
 *   100 - erase
 *   101 - compaction verify
 *   110 - compaction
 * FSM_MODE[5:3]=WSM mode
 *   000 - read
 *   001 - program verify
 *   010 - program
 *   011 - erase verify
 *   100 - erase
 *   101 - compaction verify
 *   110 - compaction
 * FSM_MODE[2:0]=WSM Command
 *   000 - program
 *   001 - erase sector
 *   010 - program sector
 *   011 - erase test
 *   100 - erase bank
 *   101 - precondition verify
 *   110 - erase verify
 *   111 - compaction verify
 */
#define FSM_MODE       0x025c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMWSMMODE     FSM_MODE

/* FSM_PGM[31:28]=Reserved
 * FSM_PGM[27]=0
 * FSM_PGM[26]=ECC for program suspend.
 * FSM_PGM[25:23]=Bank[2:0] for Program Suspend
 * FSM_PGM[22:0]=Relative Bank Address[22:0] for Program Suspend
 */
#define FSM_PGM        0x0260/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSAVEDPA     FSM_PGM

/* FSM_ERA[31:27]=Reserved
 * FSM_ERA[26]=ECC for erase suspend.
 * FSM_ERA[25:23]=Bank[2:0] for Erase Suspend
 * FSM_ERA[22:0]=Relative Bank Address[22:0] for Erase Suspend
 */
#define FSM_ERA        0x0264/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSAVEDEA     FSM_ERA

/* FSM_PRG_PUL[15:12]=Starting count for EC Erase Count (0).
 * FSM_PRG_PUL[11:0]=Max program pulses allow per word - 50 (0x32)
 */
#define FSM_PRG_PUL    0x0268/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMMAXPP       FSM_PRG_PUL

/* FSM_ERA_PUL[15:12]=Highest count which EC will progress to (15)
 * FSM_ERA_PUL[11:0]=Max erase pulses allow per sector - 3000 (0xbb8)
 */
#define FSM_ERA_PUL    0x026c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMMAXEP       FSM_ERA_PUL

/* FSM_CMP_PUL[15:12]=number of times to repeat each EC value (0)
 * FSM_CMP_PUL[11:0]=Max erase pulses allow per column - 2000 (0x7d0)
 */
#define FSM_CMP_PUL    0x0270/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMMAXCP       FSM_CMP_PUL

/* FSM_PUL_CNTR[15:12]=Loaded with EC count for erase operation
 * FSM_PUL_CNTR[11:0]=Current pulse count (depending on mode)
 */
#define FSM_PUL_CNTR   0x0274/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPLSCNT      FSM_PUL_CNTR

/* FSM_SAV_PGM[15:0]=Contains the data for a program
 *                 suspend operation
 */
#define FSM_SAV_PGM    0x0278/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMPSDATA      FSM_SAV_PGM

/* The following added for F035_4MB_WRAPPER:
 *   FSM_ST_MACHINE[15:7]=Reserved
 *   FSM_ST_MACHINE[14]=ONE_TIME_GOOD
 *   FSM_ST_MACHINE[13]=SWITCH_PNT
 *   FSM_ST_MACHINE[12]=PRE_CMP_FAIL_STOP
 *   FSM_ST_MACHINE[11]=DO_REDU_COL
 *   FSM_ST_MACHINE[10:7]=DBG_SHORT_ROW[3:0]
 * 
 * The following is defined on all wrappers:
 *
 *   FSM_ST_MACHINE[6]=BNK_ERAMODE
 *                     0 -- Bank erase applies TCR 31 only
 *                          until the first sector is
 *                          erased. After that the FSM does
 *                          normal sector erases.
 *                     1 -- Bank erase continues to applyTCR31
 *                          to ALL erase pulses.
 *   FSM_ST_MACHINE[5]=PRG_SEC_COF_ENA (Program Sector Continue On Fail Enable)
 *   FSM_ST_MACHINE[4]=Precondition Terminate Enable.
 *   FSM_ST_MACHINE[3]=Disturbance Test Enable.
 *   FSM_ST_MACHINE[2]=Test Command Enable.
 *   FSM_ST_MACHINE[1]=Invert data by row
 *   FSM_ST_MACHINE[0]=Max pulse override.
 */
#define FSM_ST_MACHINE 0x027c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMSMTEST      FSM_ST_MACHINE

/* FSMCMNDTEST[15:0]=Command from Test Port
 *                   (for FSM mode during Test)
 */
#define FSM_CMD_TST    0x0280/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMCMNDTEST    FSM_CMD_TST

/* FSMREADDATA[15:0]=Captures output of flash bank in PMT mode
 */
#define FSM_RD_DATA    0x0284/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMREADDATA    FSM_RD_DATA

/* FSMTCREG[15:3]=Reserved
 * FSMTCREG[2:0]=Write Enable code ('101')
 */
#define FSM_WR_ENA     0x0288/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMTCREG       FSM_WR_ENA

/* FSMTOTALPP[31:0]=Total accumulated programming pulses
 */
#define FSM_ACC_PP     0x028c/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMTOTALPP     FSM_ACC_PP

/* FSMTOTALEP[31:0]=Total accumulated erase pulses
 */
#define FSM_ACC_EP     0x0290/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMTOTALEP     FSM_ACC_EP

/* FSMTOTALCP[31:0]=Total accumulated compaction pulses
 */
#define FSM_ACC_CP     0x0294/4
/* for backwards compatibility to F035_16MB_WRAPPER */
#define FSMTOTALCP     FSM_ACC_CP

/* The 16MB wrapper definition is:
 *   FSM_ADDR[31:0]=Reserved
 *
 * The 4MB wrapper definition is (read only):
 *   FSM_ADDR[31]=ECCNOW
 *   FSM_ADDR[30:28]=Bank[2:0]
 *   FSM_ADDR[27:0]=CUR_ADDR[27:0]
 */
#define FSM_ADDR     0x02a0/4

/* The 16MB wrapper definition is:
 *   FSM_SECTOR[31:0]=Reserved
 *
 * The 4MB wrapper definition is (read only):
 *   FSM_SECTOR[31:16]=SECT_ERASED
 *   FSM_SECTOR[15:8]=Reserved
 *   FSM_SECTOR[7:4]=SECTOR
 *   FSM_SECTOR[3:0]=SEC_OUT
 */
#define FSM_SECTOR     0x02a4/4

/* The 16MB wrapper definition is:
 *   FWM_REV_ID[31:0]=Reserved
 *
 * The 4MB wrapper definition is (read only):
 *   FWM_REV_ID[31:16]=Reserved
 *   FWM_REV_ID[15:13]=Version
 *   FWM_REV_ID[12:8]=FWM_DAY
 *   FWM_REV_ID[7:4]=FWM_MON
 *   FWM_REV_ID[3:0]=FWM_YEAR
 */
#define FWM_REV_ID     0x02a8/4

/* useful constants */
/* Normal Read mode */
#define FSPRD_READ        0x0000
/* Read Margin 1 Mode */
#define FSPRD_RDM1        0x0002
/* Read Margin 0 Mode */
#define FSPRD_RDM0        0x0001

#define FRDCNTL_IFLUSHEN  0x01000000
#define FRDCNTL_IDLEN     0x00010000
#define FRDCNTL_RWAIT_MSK 0x00000f00
#define FRDCNTL_ASWSTEN   0x00000010
#define FRDCNTL_PRTY      0x00000008
#define FRDCNTL_PIPE      0x00000001

/* RWAIT=0, ASWSTEN=0, PRTY=0, RM=0 */
#define FRDCNTL_READ_NONP 0x0000
/* RWAIT=1, ASWSTEN=0, PRTY=0, RM=0 */
#define FRDCNTL_RDM1_NONP 0x0100
/* RWAIT=f, ASWSTEN=0, PRTY=0, RM=0 */
#define FRDCNTL_RDM0_NONP 0x0f00
/* RWAIT=1, ASWSTEN=0, PRTY=0, RM=1 */
#define FRDCNTL_READ_PIPE 0x0101
/* RWAIT=1, ASWSTEN=0, PRTY=0, RM=1 */
#define FRDCNTL_RDM1_PIPE 0x0101
/* RWAIT=f, ASWSTEN=0, PRTY=0, RM=1 */
#define FRDCNTL_RDM0_PIPE 0x0f01
/* RWAIT=1, ASWSTEN=0, PRTY=0, RM=2 */
#define FRDCNTL_READ_BRST 0x0102
/* RWAIT=1, ASWSTEN=0, PRTY=0, RM=2 */
#define FRDCNTL_RDM1_BRST 0x0102
/* RWAIT=f, ASWSTEN=0, PRTY=0, RM=2 */
#define FRDCNTL_RDM0_BRST 0x0f02

#define PROG_PULSE_ACTIVE 0x0010
#define ERASE_PULSE_ACTIVE 0x0021
#define CMPCT_PULSE_ACTIVE 0x0031

/* command value for programming main flash */
#define CMND_PROG_DATA_MAIN 0x0002
/* command value for programming main flash check bits */
#define CMND_PROG_CBIT_MAIN 0x0004
/* command value for erasing a sector */
#define CMND_ERS_SECT       0x0006
/* command value for erasing a bank */
#define CMND_ERS_BANK       0x0008
/* command value for programming customer OTP data */
#define CMND_PROG_DATA_COTP 0x000a
/* command value for programming customer OTP check bits */
#define CMND_PROG_CBIT_COTP 0x000c
/* command value for compacting a sector (sector validation) */
#define CMND_CMPCT_SECT     0x000e
/* command value for clear status */
#define CMND_CLEAR_STATUS   0x0010
/* command value for suspend */
#define CMND_SUSPEND        0x0012
/* command value for program resume */
#define CMND_PROG_RESUME    0x0014
/* command value for erase resume */
#define CMND_ERS_RESUME     0x0016
/* address offset for command unlock */
#define CMND_UNLOCK_ADDR    0x5550
/* data for command unlock */
#define CMND_UNLOCK_DATA    0xaaaa

/* constants and timings */

/* The MAX_DELAY value can be specified in your cl470 CFLAGS as '-dMAX_DELAY=<x>'
 * or explicitly in a previous included header file using '#define MAX_DELAY <x>'
 */
#if !defined (MAX_DELAY)
#define MAX_DELAY 12
#endif

#if (MAX_DELAY>16)
#define MAX_DELAY 16
#endif

/* 500ns * PROG_PLS_WIDTH  =    5us */
#define PROG_PLS_WIDTH 10
#define PWIDTH_MIN 0
#define PWIDTH_MAX 0xffff

/* 500ns * ERASE_PLS_WIDTH =  5.0ms */
#define ERASE_PLS_WIDTH 10000
#define EWIDTH_MIN 0
#define EWIDTH_MAX 0xffffffff

/* 500ns * CMPCT_PLS_WIDTH =  500us */
#define CMPCT_PLS_WIDTH 1000
#define CWIDTH_MIN 0
#define CWIDTH_MAX 0xffff

#define MAX_PROG_PLS    50
#define MAX_CMPCT_PLS 2000
#define MAX_ERASE_PLS 4095

#define PSETUP_DEFAULT 0x18
#define PSETUP_MIN 22
#define PSETUP_MAX 0xff

#define ESETUP_DEFAULT 0x18
#define ESETUP_MIN 0
#define ESETUP_MAX 0xff

#define CSETUP_DEFAULT 0xf068
#define CSETUP_MIN 250
#define CSETUP_MAX 0xfff

#define PVSETUP_DEFAULT 0x02
#define PVSETUP_MIN 25
#define PVSETUP_MAX 0xff

#define EVSETUP_DEFAULT 0x02
#define EVSETUP_MIN 25
#define EVSETUP_MAX 0xff

#define CVSETUP_DEFAULT 0xf19a
#define CVSETUP_MIN 0
#define CVSETUP_MAX 0xfff

#define PVEVACCESS_DEFAULT 0x10
#define PVEVACCESS_MIN 0
#define PVEVACCESS_MAX 0xff

#define PHOLD_DEFAULT 0x08
#define PHOLD_MIN 0
#define PHOLD_MAX 0xff

#define CHOLD_DEFAULT 0x66
#define CHOLD_MIN 0
#define CHOLD_MAX 0xff

#define EHOLD_DEFAULT 0x00a0
#define EHOLD_MIN 0
#define EHOLD_MAX 0xffff

#define PVHOLD_DEFAULT 0x04
#define PVHOLD_MIN 0
#define PVHOLD_MAX 0xff

#define EVHOLD_DEFAULT 0x04
#define EVHOLD_MIN 0
#define EVHOLD_MAX 0xff

#define VWLCMPCTCT_DEFAULT 0x0a80
#define VHVCT1_DEFAULT 0xf2f7dcb8
#define VHVCT2_DEFAULT 0x0f00
#define VREADCT_DEFAULT 0x005a
#define VPPCT_DEFAULT 0x4450
#define VWLCT_DEFAULT 0x0180
#define VNVCT_START_DEFAULT 0x4000
#define VNVCT_STOP_DEFAULT  0xe000
#define VNVCT_STEP_DEFAULT  0x4000

#define MAX_FOSC_DELAY_DEFAULT 0x06

/* define dummy checksum */
#define CHKSUM_DEFAULT 0xffffffff

#define FBMODE_READ  0x0
#define FBMODE_PVER  0x1
#define FBMODE_PROG  0x2
#define FBMODE_EVER  0x3
#define FBMODE_ERASE 0x4
#define FBMODE_CVER  0x5
#define FBMODE_CMPCT 0x6

/* set fapiver symbol based on toolset */
#include "fapiver.h"

#endif /* ifndef _F035_H_ */
