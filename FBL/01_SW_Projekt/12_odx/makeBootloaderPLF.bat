..\12_tools\srec_cat ^
parrotboot_256_uart_bootloader.bin ^
-Binary ^
-Output parrot_1.hex ^
-Intel

..\12_tools\srec_cat ^
parrot_1.hex ^
-Intel ^
-offset 0x80000000 ^
-Output parrotboot_256_uart_bootloader.hex ^
-Intel

rem del parrot_1.hex