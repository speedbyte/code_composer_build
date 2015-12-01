..\12_tools\srec_cat ^
parrotboot_256_updater.plf ^
-Binary ^
-Output parrot_1.hex ^
-Intel

..\12_tools\srec_cat ^
parrot_1.hex ^
-Intel ^
-offset 0x80000000 ^
-Output parrotboot_256_updater.hex ^
-Intel

rem del parrot_1.hex