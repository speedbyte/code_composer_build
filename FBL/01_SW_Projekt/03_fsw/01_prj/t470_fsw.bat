..\..\12_tools\hex470 -@ t470_fsw.cmf > error.log

..\..\12_tools\srec_cat ^
..\05_out\T470_fsw.hex ^
-Intel ^
-offset 0x8000 ^
-random_fill 0x00008000 0x0007FFFC ^
-Output ..\05_out\T470_fsw.hex ^
-Intel
