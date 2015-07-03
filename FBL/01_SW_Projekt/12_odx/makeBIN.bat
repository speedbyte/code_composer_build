
..\12_tools\srec_cat ^
%1.bin ^
-Binary ^
-Output parrot_1.hex ^
-Intel

..\12_tools\srec_cat ^
parrot_1.hex ^
-Intel ^
-offset 0x80000000 ^
-Output %1.hex ^
-Intel

rem del parrot_1.hex