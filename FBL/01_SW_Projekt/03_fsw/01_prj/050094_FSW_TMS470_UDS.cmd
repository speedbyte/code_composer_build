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


   VECTORS2 (RXI) :  origin=0x00008000   length=0x00000024
   ASW      (RXI) :  origin=0x00008024   length=0x00003FDB
   ROMWDT   (RXI) :  origin=0x0000C000   length=0x00000370
   ROMHAL (RXI)   :  origin=0x0000C400   length=0x000000A0
   RANGE1   (RXI) :  origin=0x00010000   length=0x00008000  fill=0x88888888
   ROMCONST (RXI):   origin=0x00018000   length=0x00000100
   ROMEXEC (RXI)  :  origin=0x00018100   length=0x00000F00 
   RANGE2   (RXI) :  origin=0x00019000   length=0x00007000  fill=0x99999999
   RANGE3   (RXI) :  origin=0x00020000   length=0x00008000  fill=0xAAAAAAAA
   RANGE4   (RXI) :  origin=0x00028000   length=0x00008000  fill=0xBBBBBBBB
   RANGE5   (RXI) :  origin=0x00030000   length=0x00007FFC  fill=0xCCCCCCCC
   RANGE6   (RXI) :  origin=0x00040000   length=0x00008000  fill=0xDDDDDDDD
   RANGE7   (RXI) :  origin=0x00048000   length=0x00008000  fill=0xEEEEEEEE
   RANGE6   (RXI) :  origin=0x00050000   length=0x00008000  fill=0xFFFFFFFF
   RANGE8   (RXI) :  origin=0x00058000   length=0x00008000  fill=0x11111111
   RANGE9   (RXI) :  origin=0x00060000   length=0x00008000  fill=0x22222222
   RANGE10  (RXI) :  origin=0x00068000   length=0x00008000  fill=0x33333333
   RANGE11  (RXI) :  origin=0x00070000   length=0x00008000  fill=0x44444444
   RANGE12  (RXI) :  origin=0x00078000   length=0x00008000  fill=0x55555555
   RANGE13  (RXI) :  origin=0x00080000   length=0x00010000  fill=0x66666666
   RANGE14  (RXI) :  origin=0x00090000   length=0x00010000  fill=0x77777777
   RANGE15  (RXI) :  origin=0x000a0000   length=0x00010000  fill=0x88888888
   RANGE16  (RXI) :  origin=0x000b0000   length=0x0000FFFC  fill=0x99999999
 
   CHKS     (R)  :  origin=0x000Bfffc   length=0x00000004  fill=0x12345678

   FLASH_REQ (RW) :  origin=0x00400000   length=0x00000004
   
   RAMDATA1 (RWXI) :  origin=0x00400004   length=0x000000A0
   RAMDATA2 (RWXI) :  origin=0x004000A4   length=0x00000F5C
   HISDATA (RW)   :  origin=0x00401010   length=0x00000040
   HISCONST (RW)  :  origin=0x00401050   length=0x00000100   
   HISTEXT (RWXI) :  origin=0x00401150   length=0x00000B00
   RAMWDT(RWXI)   :  origin=0x00401C50   length=0x00000370
   USTACK  (RW)   :  origin=0x00402000   length=0x00000400  /* fill=0xffffffff */
   SYSSTA  (RW)   :  origin=0x00402400   length=0x00000400
   RAMHAL(RWXI)   :  origin=0x00402800   length=0x000000A0

   /* peripherals */
   ADC0    (RW)   :  origin=0xFFF7F000   length=0x00000400
   HET0CTRL(RW)   :  origin=0xfff7fc00   length=0x00000400  /* HET control */
   GIO0   (RW)    :  origin=0xFFF7EC00   length=0x00000100  /* digital IO */
   SPI1   (RW)    :  origin=0xFFF7F800   length=0x00000100  /* SPI */
   SPI2   (RW)    :  origin=0xFFF7D400   length=0x00000100  /* SPI */
   SPI3   (RW)    :  origin=0xFFF7D500   length=0x00000100  /* SPI */
   SPI4   (RW)    :  origin=0xFFF7D600   length=0x00000100  /* SPI */
   SPI5   (RW)    :  origin=0xFFF7D700   length=0x00000100  /* SPI */
   SCC    (RW)    :  origin=0xFFF7E800   length=0x00000200  /* CAN */
   SCC2    (RW)    :  origin=0xFFF7EA00   length=0x00000200  /* CAN */
   SCC3    (RW)    :  origin=0xFFF7D000   length=0x00000200  /* CAN */
   CAN_MBOX (RW)  :  origin=0xFFF7E400   length=0x00000200  /* CAN MAILBOX0 */
   CAN_MBOX2 (RW)  :  origin=0xFFF7E600   length=0x00000200  /* CAN MAILBOX1 */
   CAN_MBOX3 (RW)  :  origin=0xFFF7D200   length=0x00000200  /* CAN MAILBOX2 */
                                               
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

  .WDTRAM      : {_RAMWDT = .;} > RAMWDT  
  .WDTROM      : {_ROMWDT = .;} > ROMWDT        

  .HALRAM      : {_RAMHAL = .;} > RAMHAL  
  .HALROM      : {_ROMHAL = .;} > ROMHAL        


  .watchdog: load = ROMWDT, run = RAMWDT 
  {

    ..\..\01_Adlatus\03_obj\ADLATUS_ExternalWatchdog.obj(.text)

    -l..\..\06_hallib\05_out\HALLIB_cc_tms470_VW_UDS.lib<ADLATUS_CanHandlerRAM.obj> (.text)
    
  } > ROMWDT

  .halfunctions: load = ROMHAL, run = RAMHAL
  {

    -l..\..\06_hallib\05_out\HALLIB_cc_tms470_VW_UDS.lib<ADLATUS_HalRAM.obj> (.text)
  
  } > ROMHAL
  
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

/*    .vectors: 
	{

	} >  VECTORS2
  */
    /***************************************************************************/   	 
    /*                                                                         */
    /* Next Interface to FSW                                                   */
    /*                                                                         */
    /***************************************************************************/   

    /* INTERRUPT VECTORS  */
    .intvecs :  
	{

	} > VECTORS2     
	
    /************** ROM ROM runs in RAM RAM **********************************/

    /* Functions which are used by the BB for flashing.      */
    /* Changes of the location will have strange effects.    */
	/* Only the one who REALLY knows what he is doing should */
	/* make changes.                                         */

    /*************************** Rest of RAM *********************************/


 
	.sysmem		: > RAMDATA2			/* dynamic memory allocation area */
 
    /************** ROM ROM ROM ROM ********************************************/

	.data_set       : {} > ASW 


    /************** Rest of all **********************************************/

     .const	: { *(.const) } > ASW  	/* rest of constant data */
     .cinit	: { *(.cinit) } > ASW 	/* initialization tables */ 

     .text	:  /*{ *(.text) } */  > ASW | RANGE1	/* rest of code */
	 	  
    /************** TMS REGISTERS and INTERFACE FSW <-> BB *******************/
    
      .GIO0   :   {_e_GIO_ST  = .;}  > GIO0
      .HET0   :   {_e_HET0_ST = .;}  > HET0CTRL
      .ADC0   :   {_e_ADC0_ST = .;}  > ADC0
      .SPI1   :   {_e_SPI1_ST = .;}  > SPI1
      .SPI2   :   {_e_SPI2_ST = .;}  > SPI2
      .SPI3   :   {_e_SPI3_ST = .;}  > SPI3
      .SPI4   :   {_e_SPI4_ST = .;}  > SPI4
      .SPI5   :   {_e_SPI5_ST = .;}  > SPI5
      .SCC    :   {_e_SCC_ST  = .;}  > SCC
      .CAN_MB :   {_e_CAN_MB  = .;}  > CAN_MBOX
      .SCC2    :   {_e_SCC2_ST  = .;}  > SCC2
      .CAN_MB2 :   {_e_CAN2_MB  = .;}  > CAN_MBOX2
      .SCC3    :   {_e_SCC3_ST  = .;}  > SCC3
      .CAN_MB3 :   {_e_CAN3_MB  = .;}  > CAN_MBOX3
  
      .FMC1    : {_e_FLASHCTL0_ST = .;}  > FMC1 
 
      .MMC     : {_e_SARMMC_ST  = .;}  > MMC
      .DEC     : {_e_SARDEC_ST  = .;}  > DEC
      .SYS     : {_e_SARSYS_ST  = .;}  > SYS
      .CMRTI   : {_e_SARRTI_ST  = .;}  > CMRTI
      .CIM     : {_e_SARCIM_ST  = .;}  > CIM
  	
      .HET0PROG: {_e_HETPROGRAM0_UN=.;} > HET0PROG

      .RAMSTART : {_r_F_RAMSTART = .;} > RAMDATA1
      .REQUEST  : {_r_F_REQUEST = .;} > FLASH_REQ
      
      .CHCKSUM : {_r_F_CHCKSUM2 = .; } > CHKS 
    
  /************** END LINKER COMMAND FILE ********************************/    
}
