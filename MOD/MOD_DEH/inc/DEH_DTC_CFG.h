/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     DEH_DTC_CFG.h
*-----------------------------------------------------------------------------
* Module Name:    DEH 
*-----------------------------------------------------------------------------
* Description:    Describe all DTC for the UHVNAR system 
*-----------------------------------------------------------------------------
* $Date:  $
* $Rev:  $
* $Author:  $
* $URL:  $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#define	DTC_904811_MIKROTELEFON_KURZSCHLUSSNACHMASSE 				     0
#define DTC_904812_MIKROTELEFON_KURZSCHLUSSNACHPLUS					     1
#define DTC_904813_MIKROTELEFON_UNTERBRECHUNG	      				     2
#define DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE	         3
#define DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS 	         4
#define DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER      5
#define DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG       	         6
#define DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE	      7
#define DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS 	      8
#define DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER    9
#define DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG       	      10
#define DTC_905000_FUNKTIONEINSCHRAENKUNGDURCHUEBERTEMPERATUR   11
#define DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER          12
#define DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE         13
#define DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS          14
#define DTC_905213_DREITASTENMODUL_UNTERBRECHUNG                15
#define DTC_000021_DATENBUSKOMFORT_DEFEKT				                16
#define DTC_000022_DATENBUSKOMFORT_KEINEKOMMUNIKATION				    17
#define DTC_C06400_INFOTAIMENTCAN_DEFEKT 	   				            18
#define DTC_C06500_INFOTAIMENTCAN_KEINEKOMMUNIKATION 					  19
#define DTC_C14000_BODYCOMPUTER1_KEINEKOMMUNIKATION 					  20
#define DTC_C15500_SG_SCHALTTAFELEINSATZ_KEINEKOMMUNIKATION		  21
#define DTC_C21200_SG_LENKSAEULENELEKTRONIK_KEINEKOMMUNIKATION  22
#define DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL          23
#define DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG                24
#define DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH                   25
#define DTC_D01300_SG_NICHTCODIERT                              26
#define DTC_D01400_SG_FALSCHCODIERT                             27
#define DTC_000016_SG_RADIO_KEINSIGNAL                          28
#define DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG        29
#define DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG        30
#define DTC_A00045_STEUERGERAETDEFEKT_PROGRAMMSPEICHERFEHLER    31
#define DTC_A00046_STEUERGERAETDEFEKT_EEPROMFEHLER              32
#define DTC_A00047_STEUERGERAETDEFEKT_WATCHDOGFEHER             33
#define DTC_A00500_UNGUELTIGER_DATENSATZ                        34
#define DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE     35
#define DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS      36
#define DTC_905113_BASEPLATE_UNTERBRECHNUNG           37
#define DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE    38
#define DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG     39
#define DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER    40

/*---------------------------------------------------------------------------*/
/* Event Fault data block for DEH Handler                                    */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint16_t             EventNumber;
    uint8_t              StatusOfDTC;
    uint8_t              LimitUnlearnCnt;
    uint8_t              OCC;

#ifdef DEH_READ_STD_FF
      tDEH_StdFreezeFrame StdFreezeFrames;
#endif

#ifdef DEH_READ_ADD_FF
      tDEH_AddFreezeFrame  AddFreezeFrame[DEH_MAX_NUMBER_ADD_FF];
#endif

} tDEH_EEPEventDataBlock;


typedef union
{
	tDEH_EEPEventDataBlock struc[DEH_MAX_NUMBER_EV_MEM];
	char AllString[EMB_BLOCK_SIZE * DEH_MAX_NUMBER_EV_MEM];
}
Union_EEPEventDataBlock;
