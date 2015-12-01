/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_SD_FrameId.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SD
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-09-14 13:49:44 +0200 (Mi, 14 Sep 2011) $
* $Rev: 21719 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/inc/APP_SD_FrameId.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SD_FRAME_ID_H
#define _APP_SD_FRAME_ID_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
//#define ITEMPLATE 100 /* CONFIGURATE EXPORT INTERFACE(ITEMPLATE) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/
//#define ITMPL_Func IUART_Func




/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/

typedef enum
{
/* 0 */   APP_SD_CONTEXT_INVALID = 0,
/* 1 */   APP_SD_CONTEXT_S001,            // Benutzer <x> wird verbunden
/* 2 */   APP_SD_CONTEXT_S002,            // Kein gekopp. Telefon gefunden
/* 3 */   APP_SD_CONTEXT_S008,            // Hilfetext
/* 4 */   APP_SD_CONTEXT_S009,            // Telefon wird initialisiert. Bitte warten
/* 5 */   APP_SD_CONTEXT_S010,            // Cradle-Zwang aktiv
/* 6 */   APP_SD_CONTEXT_S012,            // Kein Benutzer aktiv. Zuerst Telefon verbinden.
/* 7 */   APP_SD_CONTEXT_S018,            // Phone Status Frame 1.0
/* 8 */   APP_SD_CONTEXT_S101,            // Telefon Hauptmenü
/* 9 */   APP_SD_CONTEXT_S102,            // Telefonbuch wird geladen <x> Einträge
/* 10 */   APP_SD_CONTEXT_S103,            // Telefonbuch Keine Einträge vorhanden ( PB, Anruflisten)
/* 11 */   APP_SD_CONTEXT_S104,            // Telefonbuch Liste der Namen
/* 12 */   APP_SD_CONTEXT_S105,            // Telefonbuch Liste der Nummern für Eintrag (PB + VCR )
///* 13 */   APP_SD_CONTEXT_S111,            // Telefonbuch Details einer Nummer für Eintrag
/* 14 */   APP_SD_CONTEXT_S114,            // Telefonbuch Anrufname zurückgesetzt 
/* 15 */   APP_SD_CONTEXT_S115,            // Telefonbuch Anrufname zurücksetzen? 
/* 16 */   APP_SD_CONTEXT_S116,            // Speicher voll, Aufnahme VoiceTag nicht möglich
/* 17 */   APP_SD_CONTEXT_S117,            // Telefonbuch Verwalten Anrufname eines Eintrages
/* 18 */   APP_SD_CONTEXT_S201,            // Anruflisten Hauptmenü
/* 19 */   APP_SD_CONTEXT_S202,            // Anurflisten löschen?
/* 20 */   APP_SD_CONTEXT_S203,            // Anruflisten gelöscht
/* 21 */   APP_SD_CONTEXT_S211,            // Anruflisten Liste der Nummern ( Missed, Last Dialed, Received )
/* 22 */   APP_SD_CONTEXT_S212,            // Anruflisten Anzeige Name / Nummer
/* 23 */   APP_SD_CONTEXT_S213,            // Sms Anzeige der Sendezeit  
/* 24 */   APP_SD_CONTEXT_S301,            // Bitte Rufnummer eingeben!
/* 25 */   APP_SD_CONTEXT_S305,            // Speller Nummernwahl ( 1.0 / 1.1 unterscheiden !!!)
/* 26 */   APP_SD_CONTEXT_S306,            // Speller Sprachmailbox anrufen ( 1.0 / 1.1 unterscheiden !!! )
/* 27 */   APP_SD_CONTEXT_S402,            // Sms Keine Mitteilungen vorhanden
/* 28 */   APP_SD_CONTEXT_S404,            // Sms Liste der Mitteilungen 
/* 29 */   APP_SD_CONTEXT_S405,            // Sms Mitteilung von Name / Nummer 
/* 30 */   APP_SD_CONTEXT_S406,            // Sms Anzeige des Textes
/* 31 */   APP_SD_CONTEXT_S407,            // Funktion nur bei stehendem Fahrzeug ( BT, )
/* 32 */   APP_SD_CONTEXT_S408,            // Bitte warten! ( Gerät aus Geräteliste auswählen, Anrufname zurücksetzen )
/* 33 */   APP_SD_CONTEXT_S408_USERLIST,   // Bitte warten! ( Gerät aus Geräteliste auswählen )
/* 34 */   APP_SD_CONTEXT_S408_ABR,        // Bitte warten! ( nach Druck auf Abbrechen in S001)
/* 35 */   APP_SD_CONTEXT_S408_DEL,        // Bitte warten! ( nach Druck auf löschen/OK in S513, S555, S576 undn S579)
/* 36 */   APP_SD_CONTEXT_S408_PLAYER,     // Bitte warten! ( nach Druck auf Abbrechen in SC12)
/* 37 */   APP_SD_CONTEXT_S408_PRECALL,    // Bitte warten! ( Anruf beenden im ReCall-State )
/* 38 */   APP_SD_CONTEXT_S409,            // Sms Mitteilung kann nicht angezeigt werden
/* 39 */   APP_SD_CONTEXT_S410,            // Bitte warten! ( Abfragen Sms )
/* 40 */   APP_SD_CONTEXT_S413,            // Sms Mitteilung löschen
/* 41 */   APP_SD_CONTEXT_S425,            // Sms Mitteilung gelöscht
/* 42 */   APP_SD_CONTEXT_S428,            // Sms Mitteilung löschen fehlgeschlagen
/* 43 */   APP_SD_CONTEXT_S501,            // BT Keine kompatiblem MobTels gefunden
/* 44 */   APP_SD_CONTEXT_S502,            // BT Hauptmenü
/* 45 */   APP_SD_CONTEXT_S511,            // BT Bereits <x> Benutzer angelegt
/* 46 */   APP_SD_CONTEXT_S512,            // BT Benutzer löschen: Liste mit Benutzern
/* 47 */   APP_SD_CONTEXT_S513,            // BT <Benutzer> löschen?
/* 48 */   APP_SD_CONTEXT_S514,            // BT <Benutzer> gelöscht
/* 49 */   APP_SD_CONTEXT_S515,            // Geräteliste nach Suche ( Tel, HeadSet, Player)
/* 50 */   APP_SD_CONTEXT_S517,            // BT Verbindung wird hergestellt ( Tel, HeadSet, Player)
/* 51 */   APP_SD_CONTEXT_S517_PASSIVE,    // BT Verbindung wird hergestellt ( Tel, HeadSet, Player)
/* 52 */   APP_SD_CONTEXT_S518,            // Verbindung nicht möglich ( Tel, Player, HeadSet )
/* 53 */   APP_SD_CONTEXT_S51D,            // **Event:** BT PIN-Eingabe ( 1.0 / 1.1 unterscheiden !!! ) 
/* 54 */   APP_SD_CONTEXT_S51D_ACCEPT,     // **Event:** BT PIN-Eingabe ( 1.0 / 1.1 unterscheiden !!! ) 
/* 55 */   APP_SD_CONTEXT_S520,            // BT Telefone werden gesucht. Bitte warten!
/* 56 */   APP_SD_CONTEXT_S521,            // BT Anmeldung nicht mögl. weil z.B. PIN falsch / timeout
/* 57 */   APP_SD_CONTEXT_S522,            // BT Musikwiedergabe zeitweise angehalten
/* 58 */   APP_SD_CONTEXT_S52C,            // SSP Pairing, Anzeige der 6 Stelligen PIN mit Yes/No Buttons
/* 59 */   APP_SD_CONTEXT_S52D,            // SSP Passwort im Telefon bestätigen.
/* 60 */   APP_SD_CONTEXT_S533,            // BT Name gespeichert ( HeadSet, Player )
/* 61 */   APP_SD_CONTEXT_S533_2,            // BT Name gespeichert ( HeadSet, Player )
/* 62 */   APP_SD_CONTEXT_S534,            // BT <Name> Benutzerprofil erfolgreich angelegt
/* 63 */   APP_SD_CONTEXT_S534_2,          // Name nicht gespeichert
/* 64 */   APP_SD_CONTEXT_S536,            // BT Speller Benutzername ändern ( gekopp. Gerät, 1.0 / 1.1 unterscheiden !!! )
/* 65 */   APP_SD_CONTEXT_S539,            // BT Benutzer nicht angelegt
/* 66 */   APP_SD_CONTEXT_S541,            // BT Liste der gekoppelten Geräte
/* 67 */   APP_SD_CONTEXT_S541_2,          // BT Liste der gekoppelten Geräte nach S002, hier wird direkt verbunden nach Auswahl
/* 68 */   APP_SD_CONTEXT_S542,            // BT Benutzer anzeigen
/* 69 */   APP_SD_CONTEXT_S543,            // BT Bereits verbunden mit <Benutzer> 
/* 70 */   APP_SD_CONTEXT_S553,            // BT Sichtbarkeit einschalten?
/* 71 */   APP_SD_CONTEXT_S554,            // BT Sichtbar für 3 min ( aus Menü oder 3BM-Button )
/* 72 */   APP_SD_CONTEXT_S555,            // BT <Benutzer> aktiven löschen?
/* 73 */   APP_SD_CONTEXT_S556,            // BT Speller Telefonname ändern ( Was ist Telefonname ??? )
/* 74 */   APP_SD_CONTEXT_S562,            // BT HeadSet Menü
/* 75 */   APP_SD_CONTEXT_S563,            // BT Geräte Info anzeigen ( HeadSet, Player )
/* 76 */   APP_SD_CONTEXT_S565,            // BT Verbindung getrennt ( HeadSet, Player ) 
/* 77 */   APP_SD_CONTEXT_S566,            // BT Kein Gerät aktiv ( HeadSet / Player )
/* 78 */   APP_SD_CONTEXT_S567,            // BT Headset Liste gekopp. Geräte anzeigen
/* 79 */   APP_SD_CONTEXT_S568,            // BT Keine gekopp. Geräte ( HeadSet, Player )
/* 80 */   APP_SD_CONTEXT_S571,            // BT gekopp. HeadSet / Player anzeigen
/* 81 */   APP_SD_CONTEXT_S5B2,            // BT gekopp. Player anzeigen
/* 82 */   APP_SD_CONTEXT_S572,            // BT Aktuelle Verbindung trennen? ( HeadSet, Player) 
/* 83 */   APP_SD_CONTEXT_S573,            // BT Bereits verbunden mit <Name> ( HeadSet, Player)
/* 84 */   APP_SD_CONTEXT_S574,            // BT Verbunden mit <Name>  ( Tel, Player, HeadSet )
/* 85 */   APP_SD_CONTEXT_S575,            // BT Speller HeadSet / Player umbenennen ( gekopp.  Gerät,  1.0 / 1.1 unterscheiden !!! )
/* 86 */   APP_SD_CONTEXT_S576,            // BT <Name> löschen? (HeadSet, Player )
/* 87 */   APP_SD_CONTEXT_S577,            // BT Autorisierung (HeadSet, Player)
/* 88 */   APP_SD_CONTEXT_S578,            // BT <Name> gelöscht (HeadSet, Player)
/* 89 */   APP_SD_CONTEXT_S579,            // BT <Name> aktiv. Löschen? (HeadSet, Player)
/* 90 */   APP_SD_CONTEXT_S581,            // BT HeadSet wird gesucht! Bitte warten!
/* 91 */   APP_SD_CONTEXT_S582,            // BT Kein HeadSet gefunden
/* 92 */   APP_SD_CONTEXT_S597,            // BT <Benutzer> getrennt
/* 93 */   APP_SD_CONTEXT_S599,            // BT MobTel nicht kompatibel
/* 94 */   APP_SD_CONTEXT_S601,            // Telefoneinstellungen
/* 95 */   APP_SD_CONTEXT_S602,            // Telefoneinstellungen AudioMode setzen
/* 96 */   APP_SD_CONTEXT_S641,            // Telefoneinstellunegn Signaleinstellungen Menü
/* 97 */   APP_SD_CONTEXT_S642,            // Telefoneinstellungen Signaleinstellungen Rufton ändern 
/* 98 */   APP_SD_CONTEXT_S661,            // Telefoneinstellungen Telefonbuch Menü
/* 99 */   APP_SD_CONTEXT_S663,            // Telefoneinstellungen Telefonbuch wird geladen
/* 100 */   APP_SD_CONTEXT_S663_PB,         // Telefoneinstellungen Telefonbuch wird geladen ( Sortierung wird geändert )
/* 101 */   APP_SD_CONTEXT_S664,            // Telefoneinstellungen Telefonbuch Aktualisierung nicht mögl.
/* 102 */   APP_SD_CONTEXT_S669,            // Telefoneinstellungen Telefonbuch Sortierung ändern
/* 103 */   APP_SD_CONTEXT_S702,            // Aufzeichnung Menü
/* 104 */   APP_SD_CONTEXT_S703,            // Aufzeichnung Aufnahme läuft
/* 105 */   APP_SD_CONTEXT_S704,            // Aufzeichnung Liste Dateinamen
/* 106 */   APP_SD_CONTEXT_S705,            // Aufzeichnung Info über Aufnahme anzeigen / Aufnahme abspielen
/* 107 */   APP_SD_CONTEXT_S706,            // Aufzeichnung Dateigröße von Aufnahme anzeigen
/* 108 */   APP_SD_CONTEXT_S707,            // Aufzeichnung Aufnahme löschen?
/* 109 */   APP_SD_CONTEXT_S709,            // Aufzeichnung Aufnahme gelöscht
/* 110 */   APP_SD_CONTEXT_S712,            // Aufzeichnung Keine Aufnahmen vorhanden
/* 111 */   APP_SD_CONTEXT_S713,            // Aufzeichnung Aufnahme beendet. Speicher voll!
/* 112 */   APP_SD_CONTEXT_S714,            // Aufzeichnung Alle Aufnahmen löschen?
/* 113 */   APP_SD_CONTEXT_S715,            // Aufzeichnung Alle Aufnahmen gelöscht!
/* 114 */   APP_SD_CONTEXT_S719,            // Aufzeichnung Aufnahme gespeichert
/* 115 */   APP_SD_CONTEXT_S801,            // Call Rufaufbau
/* 116 */   APP_SD_CONTEXT_S802,            // Call Fehler beim Rufaufbau
/* 117 */   APP_SD_CONTEXT_S803,            // Call Aktives Gespräch anzeigen
/* 118 */   APP_SD_CONTEXT_S804,            // Call Speller DTMF ( 1.0 / 1.1 unterscheiden !!! )
/* 119 */   APP_SD_CONTEXT_S807,            // Call Keine Rufnummer ( CallListen ) 
/* 120 */   APP_SD_CONTEXT_S806,            // Call Ungültige Rufnummer ( Telefonieren ) 
/* 121 */   APP_SD_CONTEXT_S811,            // Call Anruf gehalten
/* 122 */   APP_SD_CONTEXT_S812,            // Call Zweites Gespräch anzeigen
/* 123 */   APP_SD_CONTEXT_S813,            // Call Erstes Gespräch anzeigen
/* 124 */   APP_SD_CONTEXT_S814,            // Konferenz nicht möglich
/* 125 */   APP_SD_CONTEXT_S815,            // Konferenz (Menü) anzeigen
/* 126 */   APP_SD_CONTEXT_S901,            // **Event:** Eingehenden Anruf anzeigen
/* 127 */   APP_SD_CONTEXT_S902,            // **Event:** Anklopfenden Anruf anzeigen
/* 128 */   APP_SD_CONTEXT_S903,            // **Event:** Neue Mitteilung
/* 129 */   APP_SD_CONTEXT_S904,            // **Event:** Anruf in Abwesenheit (Menü) anzeigen  
/* 130 */   APP_SD_CONTEXT_S913,            // **Event:** HFP Connection lost
/* 131 */   APP_SD_CONTEXT_S914,            // **Event:** BT Connection lost
/* 132 */   APP_SD_CONTEXT_S916,            // **Event:** HFP Connection lost bei Klemme 15 aus
/* 133 */   APP_SD_CONTEXT_S917,            //** Event:** Mobiltelefon antwortet nicht
/* 134 */   APP_SD_CONTEXT_S923,            // **Event:** Aktualisierung Telefonbuch fehlgeschlagen
/* 135 */   APP_SD_CONTEXT_S924,            // **Event:** Nach PTT: Sprachbedienung aktiv
/* 136 */   APP_SD_CONTEXT_S925,            // **Event:** Telefonbuch unvollständig geladen
/* 137 */   APP_SD_CONTEXT_S927,            // **Event:** Telefonbuch laden fehlgeschlagen
/* 138 */   APP_SD_CONTEXT_S928,            // **Event:** Nach PTT: Sprachbedienung noch nicht bereit
/* 139 */   APP_SD_CONTEXT_S929,            // **Event:** Die durch VCR erkannte Rufnummer anzeigen
/* 140 */   APP_SD_CONTEXT_S930,            // **Event:** Telefonbuch wurde aktualisiert
/* 141 */   APP_SD_CONTEXT_S933,            // **Event:** Verbinden mit <Gerätename>? 
/* 142 */   APP_SD_CONTEXT_S934,            // **Event:** Verbunden mit BT-HeadSet <Gerätename>
/* 143 */   APP_SD_CONTEXT_S935,            // **Event:** Eingehende BT-Verb. nicht mögl. da HeadSet verbunden    
/* 144 */   APP_SD_CONTEXT_S941,            // **Event:** Eingehender Anruf bei Klemme 15 aus
/* 145 */   APP_SD_CONTEXT_S942,            // **Event:** Aktiver Anruf bei Klemme 15 aus (wird 60 Sek. angezeigt)
/* 146 */   APP_SD_CONTEXT_S945,            // **Event:** Rufaufbau bei Klemme 15 aus  
/* 147 */   APP_SD_CONTEXT_SC01,            // **Event:** VUI-Event Name nicht eindeutig erkannt, mehrere möglich 
/* 148 */   APP_SD_CONTEXT_SC03,            // Player Hauptmenü
/* 149 */   APP_SD_CONTEXT_SC04,            // Player Löschen nicht möglich, von Benutzer als Tel. benutzt
/* 150 */   APP_SD_CONTEXT_SC07,            // Player Keine kompatibl. gefunden
/* 151 */   APP_SD_CONTEXT_SC12,            // Player wird verbunden
/* 152 */   APP_SD_CONTEXT_SC12_2,          // Player wird verbunden: reconnect fall: standalone media player
/* 153 */   APP_SD_CONTEXT_SC13,            // Player werden gesucht. Bitte warten!
/* 154 */   APP_SD_CONTEXT_SC14,            // Sms Mitteilung wird vorgelesen
/* 155 */   APP_SD_CONTEXT_SC16,            // Telefoneinstellungen Signaleinstellungen Anrufername ansagen einstellen
/* 156 */   APP_SD_CONTEXT_SC20,            // Privatgespräch
/* 157 */   APP_SD_CONTEXT_SC22,            // Neues Telefonbuch verfügbar
/* 158 */   APP_SD_CONTEXT_SC23,            // **Event:** Eingehende BT-Verbind. nicht mögl. da Telefon verbunden
/* 159 */   APP_SD_CONTEXT_SC24,            // Entscheidungsframe bei 2. Call wenn Halten nicht möglich 
/* 160 */   APP_SD_CONTEXT_SP18,            // Phone Status Frame 1.1
/* 161 */   APP_SD_CONTEXT_SA01,            // Inforuf Frame
/* 162 */   APP_SD_CONTEXT_SA02,            // Pannenruf Frame
/* 163 */   APP_SD_CONTEXT_SA03,            // SOS Frame      
/* 164 */   APP_SD_CONTEXT_SB34,            // Version Information Frame      
    /* that must be the last index, insert additional ctxIds above */
    APP_SD_CONTEXT_LAST
} APP_SD_enContextId;


/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the #define
*/
//#define	EXAMPLE_DEFINE		1


#endif /* _APP_SD_FRAME_ID_H */

/* End Of File _APP_SD_FRAME_ID.h */
