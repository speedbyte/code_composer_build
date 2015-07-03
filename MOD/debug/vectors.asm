;/****************************************************************************/
;/* boot_vectors.asm	 TMS470	INTERRUPT VECTORS SETUP	FILE		         */
;/****************************************************************************/
    
	.state32
    .global  _c_int00
    .global  _osUndefInstruction
    .global _osSWI
    .global _osPrefetchAbort
    .global _osDataAbort
    .global  _osIRQHandler
    .global  _osFIQ
    
    .sect ".vectors"
	
;/****************************************************************************/
;/*                                                                          */
;/* Jumptable to the jumptable located in the FSW section                    */
;/*                                                                          */
;/****************************************************************************/

	B _c_int00             ; RESET INTERRUPT
	B _osUndefInstruction  ; UNDEFINED INSTRUCTION INTERRUPT
	B _osSWI               ; SOFTWARE INTERRUPT
	B _osPrefetchAbort	   ; ABORT (PREFETCH) INTERRUPT
	B _osDataAbort	       ; ABORT (DATA) INTERRUPT
	.word 0                ; RESERVED
	B _osIRQHandler        ; IRQ INTERRUPT
	B _osFIQ               ; FIQ INTERRUPT


    
	.end

;/**************************** END of boot_vectors.asm ***********************/