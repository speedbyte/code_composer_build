#define F05
#define BAC1       0x00
#define BAC2       0x01
#define BSEA       0x02
#define BSEB       0x03
#define DPTR       0x04
#define REGOPT     0x700
#define BBUSY      0x702
#define PROTKEY    0x703
#define PESETUP    0x806
#define CSETUP     0x808
#define PVEVSETUP  0x809
#define PCHOLD     0x80D
#define EHOLD      0x80E
#define PVEVHOLD   0x810
#define CVHOLD     0x812
#define PWIDTH     0x814
#define EWIDTH     0x815
#define CWIDTH     0x816
#define PVEVACCESS 0x817
#define CVACCESS   0x818
#define WSMREG     0x81B
#define MAXPP      0x81F
#define MAXEP      0x820
#define MAXCP      0x821
#define ADDRCTRLOW 0x826
#define PTR2       0xA02
#define PTR3       0xA03
#define PTR4       0xA04
#define MAC1       0xF00
#define MAC2       0xF01
#define MSTAT      0xF03
#define TCR        0xF04
#define MAX_PROG_PULSE 0x081F
#define PULSE_COUNTER  0x0822
#define PROG_PULSE_ACTIVE 0x0028
#define ERASE_PULSE_ACTIVE 0x0044
#define CMPCT_PULSE_ACTIVE 0x0064
#define READMODE   0x00000000
#define PIPEMODE   0x00000001
#define RDM0MODE   0x00000004
#define RDM1MODE   0x00000008

////////////////////////////////////////////////////////////////
// The PSAENABLE macro enables the PSA module for the CLEAN macro
// The PSAENABLE macro disables the PSA module for the CLEAN macro
// The CLEAN macro is used to "clean" values read from Flash control
// registers on dual CPU parts when in config mode.  A value is "clean"
// once it is stored to the PSA register (0xffffff40) and read back.
////////////////////////////////////////////////////////////////
#define PSAENABLE  *(volatile int *)0xffffff50=0;
#define PSADISABLE *(volatile int *)0xffffff50=1;
#define CLEAN(x) *(volatile int *)0xffffff40=x;x=*(volatile int *)0xffffff40;
asm(" .copy \"fapiver.h\"");

