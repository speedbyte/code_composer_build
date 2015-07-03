How does the bootloader update takes place?

1. REad the ECU Program Info. If the Program Info has a byte 0x44, then there was a flash update process done.
The byte SysProgrammedInfo is written in the E2P with 0. After the flash update via the bootloader, this cell is written with 0x01.

If there is 0x40 in the SW the we have to check if the Production has updated the E2p_ this can be found out by reading the ROM SW Version and if it is different then the process will start.

