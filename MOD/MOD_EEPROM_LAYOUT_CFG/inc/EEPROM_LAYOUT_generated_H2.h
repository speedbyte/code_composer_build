#define EEPROM_LAYOUT_OFFSET_MASK_0_31 \
/* Y_0 if ( i == 0 )  {	Offset = 0;	} */ \
/* Y */ if ( i == 1 )  {	Offset = 21;	} \
/* Y */ if ( i == 2 )  {	Offset = 23;	} \
/* Y */ if ( i == 3 )  {	Offset = 20;	} \
/* Y */ if ( i == 4 )  {	Offset = 16;	} \
/* Y_0 if ( i == 5 )  {	Offset = 0;	} */ \
/* Y_0 if ( i == 6 )  {	Offset = 0;	} */ \
/* Y_0 if ( i == 7 )  {	Offset = 0;	} */ \
/* Y */ if ( i == 8 )  {	Offset = 1;	} \
/* Y */ if ( i == 9 )  {	Offset = 2;	} \
/* Y */ if ( i == 13 )  {	Offset = 3;	} \
/* Y */ if ( i == 14 )  {	Offset = 26;	} \
/* Y_0 if ( i == 15 )  {	Offset = 0;	} */ \
/* Y_0 if ( i == 16 )  {	Offset = 0;	} */ \
/* Y */ if ( i == 19 )  {	Offset = 3;	} \
/* Y */ if ( i == 20 )  {	Offset = 13;	} \
/* Y */ if ( i == 31 )  {	Offset = 2;	} \




#define EEPROM_LAYOUT_OFFSET_MASK_32_63 \
/* Y_0 if ( i == 62 )  {	Offset = 0;	} */ \




#define EEPROM_LAYOUT_LENGTH_MASK_0_31 \
/* Y */ if ( i == 0 )  {	Length = 15;	} \
/* Y */ if ( i == 1 )  {	Length = 6;	} \
/* Y */ if ( i == 2 )  {	Length = 1;	} \
/* Y */ if ( i == 3 )  {	Length = 1;	} \
/* Y */ if ( i == 4 )  {	Length = 16;	} \
/* Y if ( i == 5 )  {	Length = 32;	} */ \
/* Y if ( i == 6 )  {	Length = 32;	} */ \
/* Y */ if ( i == 7 )  {	Length = 24;	} \
/* Y */ if ( i == 8 )  {	Length = 29;	} \
/* Y */ if ( i == 9 )  {	Length = 13;	} \
/* Y */ if ( i == 13 )  {	Length = 8;	} \
/* Y */ if ( i == 14 )  {	Length = 6;	} \
/* Y if ( i == 15 )  {	Length = 32;	} */ \
/* Y */ if ( i == 16 )  {	Length = 25;	} \
/* Y */ if ( i == 19 )  {	Length = 4;	} \
/* Y */ if ( i == 20 )  {	Length = 6;	} \
/* Y */ if ( i == 31 )  {	Length = 1;	} \




#define EEPROM_LAYOUT_LENGTH_MASK_32_63 \
/* Y */ if ( i == 62 )  {	Length = 6;	} \




/* End Of File EEPROM_LAYOUT_generated_H2.h */
