/* -------------------------------------------------------------------------- */
/* [FILE_BASIC_INFO]                                                          */
/* -------------------------------------------------------------------------- */
/* File name     : tms470_target.cmd                                          */
/* -------------------------------------------------------------------------- */
/* Description   : LCF File F05       ......                                  */
/* Note          :                                                            */
/* -------------------------------------------------------------------------- */
/* [FILE_BASIC_INFO_END]                                                      */
/******************************************************************************/
/******************************************************************************/
/* Target device of this linker command file                                  */
/*                                                                            */
/* TARGET_DEVICE:                                                             */
/*                                                                            */
/* ProductName = "TMS470R1VF338/348"                                          */
/* DeviceId = 0x00000857                                                      */
/*                                                                            */
/******************************************************************************/
-c                     /* ROM AUTOINITIALIZATION MODEL */
-x


/****************************************************************************************/
/* SPECIFY THE SYSTEM MEMORY MAP                                                        */
/****************************************************************************************/
MEMORY
{

   VECTORS (RXI)  :  origin=0x00000000   length=0x00000024
   ADLATUS (RXI)  :  origin=0x00000030   length=0x00003FC0
   PROTECTION (R) :  origin=0x00003FF0   length=0x00000010 fill=0xFFFFFFFF
   ROMCONST (RXI) :  origin=0x00004000   length=0x00000100 
   ROMEXEC (RXI)  :  origin=0x00004100   length=0x00001C00

   RAMDATA1 (RWXI) :  origin=0x00400004   length=0x000000A0
   RAMDATA2 (RWXI) :  origin=0x004000A4   length=0x00000E5C
   HISDATA (RW)   :  origin=0x00400f10   length=0x00000040
   HISCONST (RW)  :  origin=0x00401000   length=0x00000100   
   HISTEXT (RWXI) :  origin=0x00401100   length=0x00000A00
   USTACK  (RW)   :  origin=0x00402200   length=0x00000200  /* fill=0xffffffff */
   SYSSTA  (RW)   :  origin=0x00402400   length=0x00000400

   /* peripherals */
   ADC0    (RW)   :  origin=0xFFF7F000   length=0x00000400
   HET0CTRL(RW)   :  origin=0xfff7fc00   length=0x00000400  /* HET control */
   GIO0   (RW)    :  origin=0xFFF7EC00   length=0x00000400  /* digital IO */
   SPI1   (RW)    :  origin=0xfff7f800   length=0x00000400  /* SPI */
   SPI2   (RW)    :  origin=0xfff7d400   length=0x00000400  /* SPI */ 
   SCI1   (RW)    :  origin=0xFFF7F400   length=0x00000100  /* SCI */
   SCI2   (RW)    :  origin=0xFFF7F500   length=0x00000100  /* SCI */
   SCC    (RW)    :  origin=0xfff7E000   length=0x00000400  /* CAN */
   CAN_MBOX (RW)  :  origin=0xfff7dc00   length=0x00000100  /* CAN MAILBOX0 */
                                               
   /* peripherals */
   HET0PROG (RW)  :  origin=0x00800000   length=0x00000800
      
   /* sar module addresses */
                                                              /* Flash module control register          */
   FMC1    (RW)   :  origin=0xFFE88000   length=0x00000014 /* FMBAC1, FMBAC2, FMBSEA, FMBSEB, FMBRDY */
   FMC2    (RW)   :  origin=0xFFE89C00   length=0x00000010 /* FMREGOPT, FMBBUSY, FMPKEY              */
   FMC3    (RW)   :  origin=0xFFE8A814   length=0x00000004 /* FMPRDY                                 */
   FMC4    (RW)   :  origin=0xFFE8BC00   length=0x00000010 /* FMMAC1, FMMAC2, FMPAGP, FMMSTAT        */
   
   MMC     (RW)   :  origin=0xFFFFFD00   length=0x00000040
   DEC     (RW)   :  origin=0xFFFFFE00   length=0x000000F0
   CMRTI   (RW)   :  origin=0xFFFFFF00   length=0x00000020 /* Clock module & Real Time Interrupt */
   SYS     (RW)   :  origin=0xFFFFFFD0   length=0x00000030
   CIM     (RW)   :  origin=0xFFFFFF20   length=0x00000014 

}

/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************/
/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY                                */
/******************************************************************************/

SECTIONS
{
  /************** RAM RAM RAM RAM *********************************************/
  
  /***********************************************/ 
  /* first define section for uninitialized data */
  /***********************************************/ 

  .ustack:    {_g_BBSuperStack_UL = .+ 0x100;} > USTACK

  .hisdata:
  {
    ..\03_obj\FlashLib_TMS470_PARROT.obj(.bss)
    ..\03_obj\ADLATUS_ParrotHal.obj (.bss)
    ..\03_obj\ADLATUS_SciHandler.obj (.bss)
  } > HISDATA 

  .hisconst: run = HISCONST
  {
      ..\03_obj\FlashLib_TMS470_PARROT.obj(.const)
  } > ROMCONST

  .histext: run = HISTEXT
  {
    ..\03_obj\FlashLib_TMS470_PARROT.obj(.text)
    ..\03_obj\ADLATUS_SciHandler.obj (.text)
    ..\03_obj\ADLATUS_ParrotHal.obj (.text)
	/*..\..\f35a_api\compact.obj(.text)
    ..\..\f35a_api\erase_stat.obj(.text)    
    ..\..\f35a_api\init_state.obj(.text)
    ..\..\f35a_api\Match_Key.obj(.text)
    ..\..\f35a_api\prog.obj(.text)
    ..\..\f35a_api\sector_select.obj(.text)
    ..\..\f35a_api\start_erase.obj(.text) */
    ..\..\f35a_api\icall16.obj(.text)
    ..\..\f35a_api\u_div16.obj (.text)
  } > ROMEXEC

  .data       : { *(.data) } >> RAMDATA1 | RAMDATA2  
  
  /***************************************************/
  /* Next define section for initialized data -- bss */
  /***************************************************/ 

  /* rest of bss */
  .bss		: { *(.bss) }  >> RAMDATA1 | RAMDATA2	/* rest of global & static variables */

   /* now locate the stacks in the right place */
		 
   .stack: 
   {    _StackUSER_  =  .+ 0x100;
		_StackFIQ_   = _StackUSER_  + 0x10;
		_StackIRQ_   = _StackFIQ_   + 0xD0;
		_StackABORT_ = _StackIRQ_   + 0x8;
		_StackUND_   = _StackABORT_ + 0x8;
		_StackSUPER_ = _StackUND_   + 0x10;
   } > SYSSTA		/* software system stack
    
	  
   /************** ROM ROM ROM ROM ********************************************/
   /*                                                                         */
   /* First always locate the BB code                                         */
   /*                                                                         */
   /***************************************************************************/
    /***************************************************************************/   	 
    /*                                                                         */
    /* Next Interface to FSW                                                   */
    /*                                                                         */
    /***************************************************************************/   
    /************** ROM ROM runs in RAM RAM **********************************/

    /* Functions which are used by the BB for flashing.      */
    /* Changes of the location will have strange effects.    */
	/* Only the one who REALLY knows what he is doing should */
	/* make changes.                                         */

    /*************************** Rest of RAM *********************************/


 
	.sysmem		: > RAMDATA2			/* dynamic memory allocation area */
 
    /************** ROM ROM ROM ROM ********************************************/

	.data_set       : {} > ADLATUS 

    /************** Rest of all **********************************************/

     .const	: { *(.const) } > ADLATUS  	/* rest of constant data */
     .cinit	: { *(.cinit) } > ADLATUS      /* initialization tables */ 

     .text	: { *(.text) }  >> ADLATUS   /* rest of code */
	 	  
    /************** TMS REGISTERS and INTERFACE FSW <-> BB *******************/
    
      .GIO0   :   {_e_GIO_ST  = .;}  > GIO0
      .HET0   :   {_e_HET0_ST = .;}  > HET0CTRL
      .ADC0   :   {_e_ADC0_ST = .;}  > ADC0
      .SPI1   :   {_e_SPI1_ST = .;}  > SPI1
      .SPI2   :   {_e_SPI2_ST = .;}  > SPI2
      .SCI1   :   {_e_SCI1_ST = .;}  > SCI1
      .SCI2   :   {_e_SCI2_ST = .;}  > SCI2
      .SCC    :   {_e_SCC_ST  = .;}  > SCC
      .CAN_MB :   {_e_CAN_MB  = .;}  > CAN_MBOX
  
      .FMC1    : {_e_FLASHCTL0_ST = .;}  > FMC1

      .MMC     : {_e_SARMMC_ST  = .;}  > MMC
      .DEC     : {_e_SARDEC_ST  = .;}  > DEC
      .SYS     : {_e_SARSYS_ST  = .;}  > SYS
      .CMRTI   : {_e_SARRTI_ST  = .;}  > CMRTI
      .CIM     : {_e_SARCIM_ST  = .;}  > CIM
  	
      .HET0PROG: {_e_HETPROGRAM0_UN=.;} > HET0PROG

      .RAMSTART : {_r_F_RAMSTART = .;} > RAMDATA1
    
  /************** END LINKER COMMAND FILE ********************************/    
}
