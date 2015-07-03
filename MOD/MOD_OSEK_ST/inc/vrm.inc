; Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 

;*****************************************************************************
; Project Name: OSEK 2.2
;    File Name: vrm.inc
;
;  Module version: $vv$=1.00
;
;  Description: Assembler header file for release management
;
;-----------------------------------------------------------------------------
;               C O P Y R I G H T
;-----------------------------------------------------------------------------
; Copyright (c) 2004 Vector Informatik GmbH               All rights reserved.
;****************************************************************************/


; KB begin vrmAsmReleaseNumberReference 
; Assembler source release number reference
osdVrmAsmMajRelNumRef        .set 3
osdVrmAsmMinRelNumRef        .set 10

; Assembler generator release number reference
osdVrmAsmGenMajRelNumRef     .set 3
osdVrmAsmGenMinRelNumRef     .set 10
; KB end vrmAsmReleaseNumberReference 

; source VRM checks

 .if (osdVrmAsmMajRelNum!=osdVrmAsmMajRelNumRef)
 .emsg "VRM: major release number mismatch (Source)"
 .endif

 .if (osdVrmAsmMinRelNum!=osdVrmAsmMinRelNumRef)
 .emsg "VRM: minor release number mismatch (Source)"
 .endif


; generator VRM checks

 .if osdVrmAsmGenMajRelNum!=osdVrmAsmGenMajRelNumRef
 .emsg "VRM: major release number mismatch (Generator)"
 .endif

 .if osdVrmAsmGenMinRelNum!=osdVrmAsmGenMinRelNumRef
 .emsg "VRM: minor release number mismatch (Generator)"
 .endif

