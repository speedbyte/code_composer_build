/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAP
 *
 * $Archive: /dev/BAP/include/bap.h $
 * $Revision: 78 $
 * $Modtime: 29.04.05 11:12 $
 * $Author: Clausr $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die oeffentliche Schnittstelle von BAP,
 *  dem Bedien- und Anzeigeprotokoll.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-11-22   1.4.1       ALA         Anpassung der BAP_VERSION_PATCH
 * 2006-06-07   1.4         ALA         Anpassung der BAP_VERSION
 * 2006-04-12   1.4         ALA         Anpassung fuer BAP_ROM_DATA_FAR.
 * 2006-03-31   1.4         ALA         Erweiterung um Funktion BAP_Acknowledge.
 * 2005-07-20   1.3.1       ALA         Anpassung der BAP_VERSION
 * 2005-05-04   1.3         ALA         Anpassung der BAP_VERSION
 * 2005-04-29   1.3         CRI         Anpassungen an BAP1.3
 * 2005-04-21   1.3         ALA         Die Deklarationen BAP_BCL_CanRxPreCopyHCS12 und
 *                                      BAP_BCL_CanRxIndicationFunction entfernt. Diese Funktionen sind
 *                                      nicht mehr im Standard BAP Core enthalten. Ihre Signatur wird vom
 *                                      CAN-Treiber vorgegeben. Beispielimplementierungen befinden sich
 *                                      im Ordner Integration
 * 2005-03-30   1.3         JOK         #defines fuer die 3-stellige BAP_Version zum Auslesen eingebaut
 * 2005-05-04   1.2.2       ALA         Anpassung der BAP_VERSION
 * 2005-03-22   1.2.1       JOK         Anpassung der BAP_VERSION
 * 2004-11-04   1.2         ALA         CacheSetRequest nach InitSendBuffer umbenannt.
 *                                      BAP_RequestError und BAP_IndicationError hinzugefuegt.
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         BAP_VERSION auf 1100 angepasst
 * 2004-06-07   1.1         JOK         Compilerabhaengig BAP_FAR definiert. Wird fuer grosse
 *                                      Anwendungen von Cosmic gebraucht, um auf paged Adressen
 *                                      zugreifen zu koennen. CodeWarrior macht dies automatisch
 *                                      entsprechend des angegebenen Speichermodells.
 * 2004-06-07   1.1         JOK         Deklarationen um BAP_FAR erweitert
 ******************************************************************/

#ifndef BAP_BAP_H
#define BAP_BAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"

/* Makro-Definitionen */

/**
 *  Definition der Version von BAP. Die Konfigurationsdatei prueft, ob die
 *  Version des Stacks mit der Version des Konfigtools uebereinstimmt.
 */
#define BAP_VERSION 1400

/**
 *  Definition der Major-Version von BAP zum Auslesen durch andere Anwendungen
 */
#define BAP_VERSION_MAJOR 1
/**
 *  Definition der Minor-Version von BAP zum Auslesen durch andere Anwendungen
 */
#define BAP_VERSION_MINOR 4
/**
 *  Definition der Patch-Version von BAP zum Auslesen durch andere Anwendungen
 */
#define BAP_VERSION_PATCH 1


/* Typ-Definitionen */

/* Const Deklarationen */

/* Externe globale Variablen */

/* Deklarationen von Funktionen */


/**
 *  Diese Funktion muss von der Applikation, welche den BAP Stack nutzt,
 *  implementiert werden und dient dem Abschalten von Interrupts (Interruptsperre).
 *  Die Namensgebung dieser Funktion wurde bewusst an den Namen
 *  der Callback-Funktion des Vector CAN-Treibers fuer IAR-Compiler
 *  angelehnt, so dass bei dieser Einstellung in CanGen die Funktionalitaet
 *  nur einmal bereitgestellt werden muss. Daher beginnt sie auch nicht mit BAP_.
 *
 *  @param apu8SaveValue Zeiger auf eine Variable in die der Zustand
 *  des Interrupt-Registers vor dem deaktivieren gesichert werden soll.
 *
 *  @remarks:
 *  Fuer HCS12DG256B und Metrowerks CodeWarrior kann z.B. die folgende
 *  Implementierung verwendet werden (moeglicherweise auch bei anderen
 *  Compilern, was jedoch im Einzelfall zu pruefen ist, siehe unten):
 *
 *  void disableint(uint8_t *apu8SaveValue)
 *  {
 *      /+ D enthaelt die Adresse
 *      asm
 *      {
 *          TFR D,X       /+ Kopiere D nach X
 *          TPA           /+ Kopiere Control-Register (alter Wert) nach A,
 *                        /+ wodurch D veraendert wird.
 *          SEI           /+ DisableInterrupts
 *          STAA 0,X      /+ Speichere A (alter Zustand des Control-Register)
 *                        /+ unter *apu8SaveValue
 *      }
 *  }
 *
 *  Die Beispielimplementierung setzt voraus,
 *  das die Parameteruebergabe im Register D erfolgt
 *  und dass Register innerhalb der Funktion geaendert
 *  werden duerfen, d.h. der Inhalt nicht wiederhergestellt
 *  werden muss.
 */
extern void
disableint(uint8_t *apu8SaveValue);


/**
 *  Diese Funktion muss von der Applikation, welche den BAP Stack nutzt,
 *  implementiert werden und dient dem Wiederherstellen des Zustands der
 *  Interrupt-Flags vor dem Aufruf von disableint.
 *  Die Namensgebung dieser Funktion wurde bewusst an den Namen der
 *  Callback-Funktion des Vector CAN-Treibers fuer IAR-Compiler
 *  angelehnt, so dass bei dieser Einstellung in CanGen die Funktionalitaet
 *  nur einmal bereitgestellt werden muss. Daher beginnt sie auch nicht mit BAP_.
 *
 *  @param au8RestoreValue Der Zustand des Interrupt-Registers, der
 *  beim Aufruf von disableint gesichert wurde.
 *
 *  @remarks:
 *  Fuer HCS12DG256B und Metrowerks CodeWarrior
 *  kann z.B. die folgende Implementierung verwendet
 *  werden (moeglicherweise auch bei anderen Compilern,
 *  was jedoch im Einzelfall zu pruefen ist):
 *
 *  void restoreint(uint8_t au8RestoreValue)
 *  {
 *      /+ B enthaelt au8RestoreValue
 *      asm
 *      {
 *          TBA     /+ Kopiere B nach A (TBP gibt es nicht)
 *          TAP     /+ Stelle Control-Register (und damit
 *                  /+ auch das Interrupt-Flag) wieder her
 *      }
 *  }
 *
 *  Die Beispielimplementierung setzt voraus,
 *  das die Parameteruebergabe im Register B erfolgt
 *  und dass Register innerhalb der Funktion geaendert
 *  werden duerfen, d.h. der Inhalt nicht wiederhergestellt
 *  werden muss. Ausserdem werden neben dem Interruptflag
 *  auch andere Flags des Control-Registers geaendert.
 *  Daher muss sichergestellt sein, dass der Compiler nicht
 *  auf den Erhalt des Control-Registers ueber Funktionsaufrufe
 *  hinaus picht.
 */
extern void
restoreint(uint8_t au8RestoreValue);


/**
 *  Diese Funktion muss von der Applikation, welche den
 *  BAP-Stack nutzt, implementiert werden.
 *
 *  Die Applikation ist dafuer verantwortlich, dass die
 *  Debug-Information ueber den CAN-Bus ueber eine
 *  spezielle Can-Id versendet wird.
 *
 *  @returns BapErr_OK, falls das Versenden erfolgreich war.
 *  Erfolgreiches Versenden loescht die Ereignisse im Debug-Speicher.
 */
extern BAP_FAR BapError_et
BAP_SendDebugInfo(const uint8_t pDebugInfo[7]);


/**
 *  Diese Funktion ist ein CallBack, der bei asynchron auftretenden Ereignissen
 *  in BAP aufgerufen wird.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Status bezieht.
 *
 *  @param aeIndication
 *      Die Indication zeigt an, was gerade signalisiert wird.
 *  @see BapIndication_t
 *      Moegliche Indications fuer dieses CallBack sind:
 *          BapInd_Data,
 *          BapInd_Result,
 *          BapInd_Processing,
 *          BapInd_DataSetGet, 
 *          BapInd_DataSet,
 *          BapInd_Processing_CNF,
 *          BapInd_Start,
 *          BapInd_StartResult 
 *          BapInd_Abort,
 *
 *  @remarks
 *      Diese Funktion muss ausserhalb von BAP in der Applikation oder der
 *      Hauptschleife definitiert werden, damit sich das Projekt linken laesst.
 *
 */
extern BAP_FAR void BAP_IndicationVoid(lsgId_t aLsgId
    , fctId_t aFctId
    , BapIndication_et aeIndication);


#ifdef BAP_USES_DATATYPE_8
/**
 *  Diese Funktion ist ein CallBack, der bei asynchron auftretenden Ereignissen
 *  in BAP aufgerufen wird. Er signalisiert ankommende Daten und/oder Fehler,
 *  sogenannte Indications.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *      Bei folgenden Meldungen enthaelt dieser Parameter eventuell keine gueltige Werte:
 *      BapInd_Error
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Status bezieht.
 *
 *      Bei folgenden Meldungen enthaelt dieser Parameter eventuell keine gueltige Werte:
 *      BapInd_Error
 *
 *  @param aeIndication
 *      Die Indication zeigt an, was gerade signalisiert wird.
 *  @see BapIndication_t
 *      Moegliche Indications fuer dieses CallBack sind:
 *          BapInd_Data,
 *          BapInd_Result,
 *          BapInd_Processing,
 *          BapInd_DataSetGet, 
 *          BapInd_DataSet,
 *          BapInd_Start,
 *          BapInd_StartResult 
 *
 *  @param au8Value
 *      Der Parameter Value enthaelt
 *          bei BapInd_Data         den neu empfangenen und aktualisierten Wert
 *          bei BapInd_DataSet      den zu setzenden Wert
 *          bei BapInd_DataSetGet   den zu setzenden Wert
 *          bei BapInd_Start        den Parameter des Methodenaufrufs
 *          bei BapInd_StartResult  den Parameter des Methodenaufrufs
 *          bei BapInd_Result       der Rueckgabewert des Methodenaufrufs
 *          bei BapInd_Processing   den Zwischenwert der Methodenberechnung
 *
 *  @remarks
 *      Diese Funktion muss ausserhalb von BAP in der Applikation oder der
 *      Hauptschleife definitiert werden, damit sich das Projekt linken laesst.
 *
 */
extern BAP_FAR void BAP_IndicationInt8(lsgId_t aLsgId
    , fctId_t aFctId
    , BapIndication_et aeIndication
    , uint8_t au8Value);
#endif /* BAP_USES_DATATYPE_8 */


#ifdef BAP_USES_DATATYPE_16
/**
 *  Diese Funktion ist ein CallBack, der bei asynchron auftretenden Ereignissen
 *  in BAP aufgerufen wird. Er signalisiert ankommende Daten und Ereignisse,
 *  sogenannte Indications.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Status bezieht.
 *
 *  @param aeIndication
 *      Die Indication zeigt an, was gerade signalisiert wird.
 *  @see BapIndication_t
 *      Moegliche Indications fuer dieses CallBack sind:
 *          BapInd_Data,
 *          BapInd_Result,
 *          BapInd_Processing,
 *          BapInd_DataSetGet, 
 *          BapInd_DataSet,
 *          BapInd_Start,
 *          BapInd_StartResult 
 *
 *  @param au16Value
 *      Der Parameter Value enthaelt
 *          bei BapInd_Data         den neu empfangenen und aktualisierten Wert
 *          bei BapInd_DataSet      den zu setzenden Wert
 *          bei BapInd_DataSetGet   den zu setzenden Wert
 *          bei BapInd_Start        den Parameter des Methodenaufrufs
 *          bei BapInd_StartResult  den Parameter des Methodenaufrufs
 *          bei BapInd_Result       der Rueckgabewert des Methodenaufrufs
 *          bei BapInd_Processing   den Zwischenwert der Methodenberechnung
 *
 *  @remarks
 *      Diese Funktion muss ausserhalb von BAP in der Applikation oder der
 *      Hauptschleife definitiert werden, damit sich das Projekt linken laesst.
 *
 */
extern BAP_FAR void BAP_IndicationInt16(lsgId_t aLsgId
    , fctId_t aFctId
    , BapIndication_et aeIndication
    , uint16_t au16Value);
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_32
/**
 *  Diese Funktion ist ein CallBack, der bei asynchron auftretenden Ereignissen
 *  in BAP aufgerufen wird. Er signalisiert ankommende Daten und Ereignisse,
 *  sogenannte Indications.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Status bezieht.
 *
 *  @param aeIndication
 *      Die Indication zeigt an, was gerade signalisiert wird.
 *  @see BapIndication_t
 *      Moegliche Indications fuer dieses CallBack sind:
 *          BapInd_Data,
 *          BapInd_Result,
 *          BapInd_Processing,
 *          BapInd_DataSetGet, 
 *          BapInd_DataSet,
 *          BapInd_Start,
 *          BapInd_StartResult 
 *
 *  @param au32Value
 *      Der Parameter Value enthaelt
 *          bei BapInd_Data         den neu empfangenen und aktualisierten Wert
 *          bei BapInd_DataSet      den zu setzenden Wert
 *          bei BapInd_DataSetGet   den zu setzenden Wert
 *          bei BapInd_Start        den Parameter des Methodenaufrufs
 *          bei BapInd_StartResult  den Parameter des Methodenaufrufs
 *          bei BapInd_Result       der Rueckgabewert des Methodenaufrufs
 *          bei BapInd_Processing   den Zwischenwert der Methodenberechnung
 *
 *  @remarks
 *      Diese Funktion muss ausserhalb von BAP in der Applikation oder der
 *      Hauptschleife definitiert werden, damit sich das Projekt linken laesst.
 *
 */
extern BAP_FAR void BAP_IndicationInt32(lsgId_t aLsgId
    , fctId_t aFctId
    , BapIndication_et aeIndication
    , uint32_t au32Value);
#endif /* BAP_USES_DATATYPE_32 */


#ifdef BAP_USES_DATATYPE_BS
/**
 *  Diese Funktion ist ein CallBack, der bei asynchron auftretenden Ereignissen
 *  in BAP aufgerufen wird. Er signalisiert ankommende Daten und Ereignisse,
 *  sogenannte Indications.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Status bezieht.
 *
 *  @param aeIndication
 *      Die Indication zeigt an, was gerade signalisiert wird.
 *  @see BapIndication_t
 *          BapInd_Data,
 *          BapInd_Reset,
 *          BapInd_Result,
 *          BapInd_Changed,
 *          BapInd_Processing,
 *          BapInd_DataSetGet, 
 *          BapInd_DataSet,
 *          BapInd_DataGet,
 *          BapInd_Start,
 *          BapInd_StartResult 
 *
 *  @param apValue
 *      Der Parameter Value enthaelt
 *          bei BapInd_Data         den neu empfangenen und aktualisierten Wert
 *          bei BapInd_Reset        die vom FSG uebertragene BAPCONFIG
 *          bei BapInd_Result       der Rueckgabewert des Methodenaufrufs
 *          bei BapInd_Changed      die geaenderten Daten des Arrays (Semantik liegt bei Applikation)
 *          bei BapInd_Processing   den Zwischenwert der Methodenberechnung
 *          bei BapInd_DataSetGet   den zu setzenden Wert
 *          bei BapInd_DataSet      den zu setzenden Wert
 *          bei BapInd_DataGet      Informationen bezueglich der zu uebertragenen Arrayteile
 *          bei BapInd_Start        den Parameter des Methodenaufrufs
 *          bei BapInd_StartResult  den Parameter des Methodenaufrufs
 *
 *      Die Daten muessen von der Applikation kopiert werden und stehen nach dem
 *      Funktionsaufruf nicht mehr zur Verfuegung.
 *
 *  @param au16Length
 *      Enthaelt die Anzahl an Bytes, die ab dem Zeiger apValue gueltig sind.
 *
 *  @remarks
 *      Diese Funktion muss ausserhalb von BAP in der Applikation oder der
 *      Hauptschleife definitiert werden, damit sich das Projekt linken laesst.
 *
 *      Der uebergebene Zeiger darf nicht innerhalb der Applikation ueber
 *      den Funktionsaufruf hinaus zwischengespeichert werden.
 *
 */
extern BAP_FAR void BAP_IndicationByteSequence(lsgId_t aLsgId
    , fctId_t aFctId
    , BapIndication_et aeIndication
    , const volatile_ptr_t apValue
    , uint16_t au16Length);
#endif /* BAP_USES_DATATYPE_BS */


/**
 *  Diese Funktion ist ein CallBack, der bei asynchron auftretenden Ereignissen
 *  in BAP aufgerufen wird. Er signalisiert ankommende und interne Fehlerereignisse,
 *  sogenannte Indications.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Error bezieht.
 *
 *  @param aeErrorCode
 *      Der Parameter ErrorCode beschreibt den Fehler, der aufgetreten ist.
 *      Sowohl interne Fehler (z.B. BapErr_IllegalSequence, BapErr_SequenceNumber,
 *      BapErr_TimeoutSegmentation, BapErr_TimeoutHeartbeat, BapErr_RetryNotSuccessful,
 *      BapErr_IncompatibleProtocolVersion, BapErr_IncompatibleDataSpecification,
 *      BapErr_GetAllMessageCorrupted, BapErr_OversizeSegmentation) als auch vom FSG
 *      mittels BAP_RequestError versendete Fehler werden durch diese Funktion signalisiert.
 *
 *  @remarks
 *      In zukuenftigen Erweiterungen ist denkbar, dass als LsgId/FctId der Wert 0
 *      zurueckgegeben wird, falls ein interner Fehler keinem Steuergeraet/Funktion zugeordnet
 *      werden kann.
 */
extern BAP_FAR void BAP_IndicationError(lsgId_t aLsgId
    , fctId_t aFctId
    , BapError_et aeErrorCode);


#ifdef BAP_USES_ACKNOWLEDGE
/**
 *  Diese Funktion wird in folgenden Faellen vom BAP Stack aufgerufen:
 *  beim erfolgreichen Versenden eines Requests
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *  @param aeAcknowledge bezeichnet die Art der erhaltenen Bestaetigung
 *
 *  @remarks 
 *  Dieser Mechanismus steht im Small FSG nicht zur Verfuegung.
 *
 *  @remarks
 *  Der Callback wird nach erfolgreicher Uebergabe an den Treiber (UBS-Schnittstelle)
 *  ausgeloest. Es wird nicht garantiert, dass der Wert wirklich auf dem Bus
 *  ausgesendet wurde.
 *  Allerdings ist sichergestellt, dass ein Nachfolgender Request einen vorhergegangenen
 *  nicht mehr ueberschreiben kann (Ausnahme: unmittelbar vor dem Aussenden tritt ein
 *  BusOff auf).
 *
 *  @remarks
 *  Durch den BAP-Stack selbst generierte Requests (Heartbeat-Nachrichten, Retry-Versuche,
 *  automatische Beantwortung von Get/GetAll bei Funktionsklassen Property/Cache) 
 *  fuehren nicht zu einem Acknowledge.
 */
extern BAP_FAR void BAP_Acknowledge(lsgId_t aLsgId
    , fctId_t aFctId
    , BapAcknowledge_et aeAcknowledge);
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */


/**
 *  Stellt eine Anforderung/Anfrage an BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  ohne Daten zu uebergeben oder zu empfangen.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param aeRequest Die Art des Requests der durchgefuehrt werden soll.
 *  @see BapRequest_t Erlaubt sind fuer diesen Request in Abhaengigkeit von der Funktion-ID und Konfiguration
 *          BapReq_DataGet,
 *          BapReq_Changed,
 *          BapReq_Abort,
 *          BapReq_Processing,
 *          BapReq_Data          (Nur fuer FSG & VOID Datentyp)
 *          BapReq_Result        (Nur fuer FSG & VOID Datentyp)
 *          BapReq_Start:        (Nur fuer ASG & VOID Datentyp)
 *          BapReq_StartResult:  (Nur fuer ASG & VOID Datentyp)
 *          BapReq_DataSetGet:   (Nur fuer ASG & VOID Datentyp)
 *          BapReq_DataSet:      (Nur fuer ASG & VOID Datentyp)
 *          
 *
 *  @retval BapErr_OK
 *          Das Setzen konnte erfolgreich durchgefuehrt werden. Die Nachricht wurde
 *          zum Versenden eingereiht. Dennoch ist es moeglich, dass nicht alle
 *          Empfaenger die Daten erhalten werden. Sollte dies auftreten, so wird
 *          BAP_Indication mit dem entsprechenden Status aufgerufen (zeitverzoegert)
 *          wenn fuer den Requesttyp eine Ueberwachung vorgesehen ist.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf eines Request.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId
 *          oder der Requesttyp ist fuer diese Funktion nicht erlaubt oder unpassender
 *          Datentyp)
 *
 *  @retval BapErr_Busy
 *          Es steht noch eine Antwort zum letzten Aufruf aus (Ueberwachung laeuft)
 *
 */
BAP_IMPL_FAR BapError_et
BAP_RequestVoid(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest);


#ifdef BAP_USES_DATATYPE_8
/**
 *  Stellt eine Anforderung/Anfrage an BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und uebergibt einen 8 Bit Wert oder holt sich einen 8 Bit Wert als Ergebnis der
 *  Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param aeRequest Die Art des Requests der durchgefuehrt werden soll.
 *  @see BapRequest_t Erlaubt sind fuer diesen Request in Abhaengigkeit von der Funktion-ID und Konfiguration
 *          BapReq_DataSetGet,
 *          BapReq_DataSet,
 *          BapReq_Data,
 *          BapReq_Start,
 *          BapReq_StartResult,
 *          BapReq_Result,
 *          BapReq_Processing
 *
 *  @param au8Data
 *      Enthaelt die Daten die zusammen mit dem Requesttyp verarbeitet werden sollen.
 *
 *  @retval BapErr_OK
 *          Das Setzen konnte erfolgreich durchgefuehrt werden. Die Nachricht wurde
 *          zum Versenden eingereiht. Dennoch ist es moeglich, dass nicht alle
 *          Empfaenger die Daten erhalten werden. Sollte dies auftreten, so wird
 *          BAP_Indication mit dem entsprechenden Status aufgerufen (zeitverzoegert)
 *          wenn fuer den Requesttyp eine Ueberwachung vorgesehen ist.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf eines Request.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId
 *          oder der Requesttyp ist fuer diese Funktion nicht erlaubt oder unpassender
 *          Datentyp)
 *
 *  @retval BapErr_Busy
 *          Es steht noch eine Antwort zum letzten Aufruf aus (Ueberwachung laeuft)
 *
 */
BAP_IMPL_FAR BapError_et
BAP_RequestInt8(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const uint8_t au8Data);
#endif /* BAP_USES_DATATYPE_8 */


#ifdef BAP_USES_DATATYPE_16
/**
 *  Stellt eine Anforderung/Anfrage an BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und uebergibt einen 16 Bit Wert oder holt sich einen 16 Bit Wert als Ergebnis der
 *  Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param aeRequest Die Art des Requests der durchgefuehrt werden soll.
 *  @see BapRequest_t Erlaubt sind fuer diesen Request in Abhaengigkeit von der Funktion-ID und Konfiguration
 *          BapReq_DataSetGet,
 *          BapReq_DataSet,
 *          BapReq_Data,
 *          BapReq_Start,
 *          BapReq_StartResult,
 *          BapReq_Result,
 *          BapReq_Processing
 *
 *  @param au16Data
 *      Enthaelt die Daten die zusammen mit dem Requesttyp verarbeitet werden sollen.
 *
 *  @retval BapErr_OK
 *          Das Setzen konnte erfolgreich durchgefuehrt werden. Die Nachricht wurde
 *          zum Versenden eingereiht. Dennoch ist es moeglich, dass nicht alle
 *          Empfaenger die Daten erhalten werden. Sollte dies auftreten, so wird
 *          BAP_Indication mit dem entsprechenden Status aufgerufen (zeitverzoegert)
 *          wenn fuer den Requesttyp eine Ueberwachung vorgesehen ist.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf eines Request.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId
 *          oder der Requesttyp ist fuer diese Funktion nicht erlaubt oder unpassender
 *          Datentyp)
 *
 *  @retval BapErr_Busy
 *          Es steht noch eine Antwort zum letzten Aufruf aus (Ueberwachung laeuft)
 *
 */
BAP_IMPL_FAR BapError_et
BAP_RequestInt16(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const uint16_t au16Data);
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_32
/**
 *  Stellt eine Anforderung/Anfrage an BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und uebergibt einen 32 Bit Wert oder holt sich einen 32 Bit Wert als Ergebnis der
 *  Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param aeRequest Die Art des Requests der durchgefuehrt werden soll.
 *  @see BapRequest_t Erlaubt sind fuer diesen Request in Abhaengigkeit von der Funktion-ID und Konfiguration
 *          BapReq_DataSetGet,
 *          BapReq_DataSet,
 *          BapReq_Data,
 *          BapReq_Start,
 *          BapReq_StartResult,
 *          BapReq_Result,
 *          BapReq_Processing
 *
 *  @param au32Data
 *      Enthaelt die Daten die zusammen mit dem Requesttyp verarbeitet werden sollen.
 *
 *  @retval BapErr_OK
 *          Das Setzen konnte erfolgreich durchgefuehrt werden. Die Nachricht wurde
 *          zum Versenden eingereiht. Dennoch ist es moeglich, dass nicht alle
 *          Empfaenger die Daten erhalten werden. Sollte dies auftreten, so wird
 *          BAP_Indication mit dem entsprechenden Status aufgerufen (zeitverzoegert)
 *          wenn fuer den Requesttyp eine Ueberwachung vorgesehen ist.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf eines Request.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId
 *          oder der Requesttyp ist fuer diese Funktion nicht erlaubt oder unpassender
 *          Datentyp)
 *
 *  @retval BapErr_Busy
 *          Es steht noch eine Antwort zum letzten Aufruf aus (Ueberwachung laeuft)
 *
 */
BAP_IMPL_FAR BapError_et
BAP_RequestInt32(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const uint32_t au32Data);
#endif /* BAP_USES_DATATYPE_32 */


#ifdef BAP_USES_DATATYPE_BS
/**
 *  Stellt eine Anforderung/Anfrage an BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und uebergibt eine Byte Sequence (C-Array) oder holt sich eine Byte Sequence als Ergebnis der
 *  Anforderung.
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param aeRequest Die Art des Requests der durchgefuehrt werden soll.
 *  @see BapRequest_t Erlaubt sind fuer diesen Request in Abhaengigkeit von der Funktion-ID und Konfiguration
 *          BapReq_DataSetGet,
 *          BapReq_DataSet,
 *          BapReq_Data,
 *          BapReq_Start,
 *          BapReq_StartResult,
 *          BapReq_Result,
 *          BapReq_Processing,
 *          BapReq_Data (Nur Arrays)
 *          BapReq_Changed (Nur Arrays)
 *
 *  @param apData
 *      Enthaelt einen Zeiger auf die Daten die zusammen mit dem Requesttyp verarbeitet werden sollen.
 *
 *  @param au16Length
 *      Beschreibt die Anzahl an Bytes die versendet oder gesetzt werden sollen. 
 *
 *  @remarks
 *  Mit dieser Funktion koennen auch structs uebertragen werden, allerdings ist die Applikation fuer die
 *  Serialisierung/Deserialisierung zustaendig.
 *
 *  @remarks 
 *  Es ist auch moeglich ByteSequences mit NULL Pointer auf die Daten und Groesse 0 zu uebertragen. 
 *  Dies entspricht einer Art Event. Der VOID Datentyp waere allerdings besser dafuer geeignet.
 *
 *  @retval BapErr_OK
 *          Das Setzen konnte erfolgreich durchgefuehrt werden. Die Nachricht wurde
 *          zum Versenden eingereiht. Dennoch ist es moeglich, dass nicht alle
 *          Empfaenger die Daten erhalten werden. Sollte dies auftreten, so wird
 *          BAP_Indication mit dem entsprechenden Status aufgerufen (zeitverzoegert)
 *          wenn fuer den Requesttyp eine Ueberwachung vorgesehen ist.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf eines Request.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId
 *          oder der Requesttyp ist fuer diese Funktion nicht erlaubt oder unpassender
 *          Datentyp oder Laenge der Bytesequenz zu lang)
 *
 *  @retval BapErr_Busy
 *          Es steht noch eine Antwort zum letzten Aufruf aus (Ueberwachung laeuft)
 *
 */
BAP_IMPL_FAR BapError_et
BAP_RequestByteSequence(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const ptr_t apData
    , const uint16_t au16Length);
#endif /* BAP_USES_DATATYPE_BS */


#ifdef BAP_FSG
/**
 *  Stellt im FSG eine Fehleranforderung/anfrage an BAP fuer ein bestimmtes Steuergereat und Funktion
 *  und uebergibt den Fehlerwert.
 *
 *  @param aLsgId
 *      Der Parameter logisches Steuergeraet enthaelt die Nummer des Steuergeraetes,
 *      fuer die die Meldung gueltig ist.
 *
 *  @param aFctId
 *      Der Parameter Funktionskennung enthaelt die Nummer der Funktion
 *      (des Datums) auf den sich der Status bezieht.
 *
 *  @param aeErrorCode
 *      Der Parameter ErrorCode beschreibt den Fehler, der aufgetreten ist.
 *
 *  @returns
 *      Fehlercode, falls die Anfrage nicht akzeptiert werden kann.
 *
 *  @retval BapErr_OK
 *      Die Anfrage konnte erfolgreich durchgefuehrt werden.
 *
 *  @retval BapErr_InvalidState
 *      Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *      den Aufruf.
 *
 *  @retval BapErr_InvalidArg
 *      Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId) oder
 *      falscher Steuergeraetetyp (im Kombigeraet nur fuer FSG erlaubt).
 *
 *  @remarks
 *      Diese Funktion steht nur im FSG zur Verfuegung. Diese Funktion sollte nur als Antwort auf
 *      einen Request / eine Indication aufgerufen werden, also z.B. als Antwort auf einen BapInd_Start,
 *      BapInd_StartResult, BapInd_DataSet, BapInd_DataSetGet falls der uebergebene Wert unzulaessig ist.
 *
 *      Diese Funktion sollte ebenfalls als Antwort auf eine BapInd_Abort aufgerufen werden.
 */
BAP_IMPL_FAR BapError_et
BAP_RequestError(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapError_et aeErrorCode);
#endif /* BAP_FSG */


#if defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_8)
/**
 *  Stellt eine Anfrage an den Cache im BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und holt sich einen 8 Bit Wert als Ergebnis der Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param apu8OutData
 *      Adresse, wohin das Ergebnis aus dem Cache hinkopiert werden soll.
 *      Der Wert wird nicht veraendert wenn der Rueckgabewert ungleich BapErr_OK ist.
 *
 *  @remark Es ist nicht erlaubt einen NULL Pointer fuer apu8OutData anzugeben!
 * 
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Zugriff auf den
 *          Cache hat einen gueltigen Wert gefunden und diesen zurueckgegeben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf.
 *
 *  @retval BapErr_CacheNotAvailable
 *          Fuer die angegebenen FunktionID ist kein Cache-Eintrag konfiguriert.
 *
 *  @retval BapErr_CacheInvalid
 *          Der Cache-Eintrag der FunktionID war als ungueltig
 *          markiert und konnte daher nicht gelesen werden (keine Daten empfangen,
 *          initialisiert oder Timeouts).
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId),
 *          NULL Pointer Uebergabe oder falscher Steuergeraetetyp(im Kombigeraet nur fuer ASG). 
 *
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestInt8(const lsgId_t aLsgId
    , const fctId_t aFctId
    , uint8_t * const apu8OutData);
#endif /* defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_8) */


#if defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_16)
/**
 *  Stellt eine Anfrage an den Cache im BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und holt sich einen 16 Bit Wert als Ergebnis der Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param apu16OutData
 *      Adresse, wohin das Ergebnis aus dem Cache hinkopiert werden soll.
 *      Der Wert wird nicht veraendert wenn der Rueckgabewert ungleich BapErr_OK ist.
 *
 *  @remark Es ist nicht erlaubt einen NULL Pointer fuer apu16OutData anzugeben!
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Zugriff auf den
 *          Cache hat einen gueltigen Wert gefunden und diesen zurueckgegeben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf.
 *
 *  @retval BapErr_CacheNotAvailable
 *          Fuer die angegebenen FunktionID ist kein Cache-Eintrag konfiguriert.
 *
 *  @retval BapErr_CacheInvalid
 *          Der Cache-Eintrag der FunktionID war als ungueltig
 *          markiert und konnte daher nicht gelesen werden (keine Daten empfangen,
 *          initialisiert oder Timeouts).
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId),
 *          NULL Pointer Uebergabe oder falscher Steuergeraetetyp(im Kombigeraet nur fuer ASG). 
 *
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestInt16(const lsgId_t aLsgId
    , const fctId_t aFctId
    , uint16_t * const apu16OutData);
#endif /* defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_16) */


#if defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_32)
/**
 *  Stellt eine Anfrage an den Cache im BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und holt sich einen 32 Bit Wert als Ergebnis der Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param apu32OutData
 *      Adresse, wohin das Ergebnis aus dem Cache hinkopiert werden soll.
 *      Der Wert wird nicht veraendert wenn der Rueckgabewert ungleich BapErr_OK ist.
 *
 *  @remark Es ist nicht erlaubt einen NULL Pointer fuer apu32OutData anzugeben!
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Zugriff auf den
 *          Cache hat einen gueltigen Wert gefunden und diesen zurueckgegeben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf.
 *
 *  @retval BapErr_CacheNotAvailable
 *          Fuer die angegebenen FunktionID ist kein Cache-Eintrag konfiguriert.
 *
 *  @retval BapErr_CacheInvalid
 *          Der Cache-Eintrag der FunktionID war als ungueltig
 *          markiert und konnte daher nicht gelesen werden (keine Daten empfangen,
 *          initialisiert oder Timeouts).
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId),
 *          NULL Pointer Uebergabe oder falscher Steuergeraetetyp(im Kombigeraet nur fuer ASG). 
 *
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestInt32(const lsgId_t aLsgId
    , const fctId_t aFctId
    , uint32_t * const apu32OutData);
#endif /* defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_32) */


#if defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_BS)
/**
 *  Stellt eine Anfrage an den Cache im BAP fuer ein bestimmtes Steuergeraet und Funktion
 *  und holt sich eine Byte-Sequenz als Ergebnis der Anforderung.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param appOutData
 *      Adresse, wohin die Adresse der Daten der Byte-Sequenz aus dem Cache hinkopiert werden soll.
 *      Der Wert wird nicht veraendert wenn der Rueckgabewert ungleich BapErr_OK ist.
 *
 *  @param apu16OutLength
 *      Adresse, wohin die Laenge der Byte-Sequenz aus dem Cache hinkopiert werden soll.
 *
 *  @remark Es ist nicht erlaubt NULL Pointer fuer die uebergebenen Pointer anzugeben!
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Zugriff auf den
 *          Cache hat einen gueltigen Wert gefunden und diesen zurueckgegeben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (running) fuer
 *          den Aufruf.
 *
 *  @retval BapErr_CacheNotAvailable
 *          Fuer die angegebenen FunktionID ist kein Cache-Eintrag konfiguriert.
 *
 *  @retval BapErr_CacheInvalid
 *          Der Cache-Eintrag der FunktionID war als ungueltig
 *          markiert und konnte daher nicht gelesen werden (keine Daten empfangen,
 *          initialisiert oder Timeouts).
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId),
 *          NULL Pointer Uebergabe oder falscher Steuergeraetetyp(im Kombigeraet nur fuer ASG). 
 *
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestByteSequence(const lsgId_t aLsgId
    , const fctId_t aFctId
    , ptr_t * const appOutData
    , uint16_t * const apu16OutLength);
#endif /* defined(BAP_ASG)  && defined(BAP_USES_DATATYPE_BS) */


#if defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_8)
/**
 *  Schreibt einen 8 Bit Wert in den Sendepuffer des BAP-Stacks fuer eine
 *  bestimmte FunktionId eines Steuergeraets. BAP_Start wird fehlschlagen wenn nicht alle
 *  Properties der Funktionsliste auf diese Weise initialisiert wurden!
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param au8Data enthaelt die Daten die in den Sendepuffer geschrieben werden sollen.
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Wert wurde in
 *          den Sendepuffer geschrieben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (initialized)
 *          fuer den Aufruf oder falscher Steuergeraetetyp (geht nur im FSG).
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId)
 *          oder der Datentyp stimmt nicht mit der Konfiguration ueberein
 *          oder der Aufruf war in einem Kombigeraet fuer ein ASG.
 *
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferInt8(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const uint8_t au8Data);
#endif /* defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_8) */


#if defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_16)
/**
 *  Schreibt einen 16 Bit Wert in den Sendepuffer des BAP-Stacks fuer eine
 *  bestimmte FunktionId eines Steuergeraets. BAP_Start wird fehlschlagen wenn nicht alle
 *  Properties der Funktionsliste auf diese Weise initialisiert wurden!
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param au16Data enthaelt die Daten die in den Sendepuffer geschrieben werden sollen.
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Wert wurde in
 *          den Sendepuffer geschrieben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (initialized)
 *          fuer den Aufruf.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId)
 *          oder der Datentyp stimmt nicht mit der Konfiguration ueberein
 *          oder der Aufruf war in einem Kombigeraet fuer ein ASG.
 *
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferInt16(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const uint16_t au16Data);
#endif /* defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_16) */


#if defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_32)
/**
 *  Schreibt einen 32 Bit Wert in den Sendepuffer des BAP-Stacks fuer eine
 *  bestimmte FunktionId eines Steuergeraets. BAP_Start wird fehlschlagen wenn nicht alle
 *  Properties der Funktionsliste auf diese Weise initialisiert wurden!
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param au32Data enthaelt die Daten die in den Sendepuffer geschrieben werden sollen.
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Wert wurde in
 *          den Sendepuffer geschrieben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (initialized)
 *          fuer den Aufruf.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId)
 *          oder der Datentyp stimmt nicht mit der Konfiguration ueberein
 *          oder der Aufruf war in einem Kombigeraet fuer ein ASG.
 *
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferInt32(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const uint32_t au32Data);
#endif /* defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_32) */


#if defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_BS)
/**
 *  Schreibt einen ByteSequenz in den Sendepuffer des BAP-Stacks fuer eine
 *  bestimmte FunktionId eines Steuergeraets. BAP_Start wird fehlschlagen wenn nicht alle
 *  Properties der Funktionsliste auf diese Weise initialisiert wurden!
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param apData enthaelt die Daten die in den Sendepuffer geschrieben werden sollen.
 *
 *  @param au16Length enthaelt die Anzhal der Daten.
 *
 *  @remark Es ist moeglich, einen Nullpointer fuer die Daten zu uebergeben. Dann ist es aber
 *  auch notwendig eine Datenlaenge (au16Length) von 0 anzugeben.
 *
 *  @remark Mit dieser Funktion ist es moeglich die Funktionsliste in einem FSG umzuschalten. 
 *  Zwingend notwendig dafuer ist die Existenz der Funktionen 2+3 (sonst kann die neue Funktionsliste
 *  nie im ASG ankommen) und das fuer alle Funktionen der neuen Konfiguration ein Eintrag in der XML
 *  Datenfestlegung existiert. Der Ablauf fuer ein Umschalten ist daher folgendermassen: BAP_Shutdown,
 *  BAP_InitSendBufferByteSequence(Fct.list), BAP_Start.  
 *
 *  @retval BapErr_OK
 *          Die Anfrage konnte erfolgreich durchgefuehrt werden. Der Wert wurde in
 *          den Sendepuffer geschrieben.
 *
 *  @retval BapErr_InvalidState
 *          Das angegebene LSG befindet sich nicht im richtigen Zustand (initialized)
 *          fuer den Aufruf.
 *
 *  @retval BapErr_IncompatibleDataSpecification
 *          Die neue Funktionsliste ist ungueltig. Entweder beinhaltet sie nicht Funktion 2+3 oder
 *          fuer mindestens einen Eintrag existiert keine XML Datenfestlegung.
 *
 *  @retval BapErr_InvalidArg
 *          Einer der uebergebenen Parameter ist ungueltig (ungueltige FctId oder LsgId),
 *          der Datentyp stimmt nicht mit der Konfiguration ueberein,
 *          ein NULL Pointer fuer apData mit au16Length > 0 wurde uebergeben
 *          oder der Aufruf war in einem Kombigeraet fuer ein ASG.
 *
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferByteSequence(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const ptr_t apData
    , const uint16_t au16Length);
#endif /* defined(BAP_FSG)  && defined(BAP_USES_DATATYPE_BS) */


/**
 *  Diese Funktion muss von der Hauptschleife (main() der gesamten Steuergeraeteapplikation)
 *  oder einem Timer aus zyklisch aufgerufen werden, nachdem BAP_Start() aufgerufen wurde.
 *
 *  Dies ist notwendig, um die Stacktiefe bei Interrupts zu begrenzen sowie Polling auf den Bus
 *  zu ermoeglichen.
 *
 *  Ausserdem werden hier Zeitueberwachungen (Heartbeat und Retry-Timer) durchgefuehrt.
 *  Die Zykluszeit muss entsprechend im Konfigurationstool passend beachtet werden bei
 *  Zeitgroessen, welche in Anzahl Tasks angegeben werden.
 */
BAP_IMPL_FAR void BAP_Task(void);


/**
 *  Diese Funktion muss direkt nach einem Neustart aufgerufen werden, bevor BAP gestartet werden kann.
 *  Danach darf die Funktion nicht mehr aufgerufen werden.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet
 *
 *  @retval BapErr_OK
 *          Die Intialisierung war erfolgreich.
 *
 *  @retval BapErr_InvalidArg
 *          Das angegebene LSG existiert nicht, oder es ist ein interner Fehler aufgetreten.
 *
 *  @retval BapErr_InvalidState
 *          BAP wurde schon einmal initialisiert.
 *
 */
BAP_IMPL_FAR BapError_et
BAP_Init(lsgId_t aLsgId);


/**
 *  Diese Funktion startet den Empfang und das Versenden von Nachrichten auf
 *  dem angegebenen logischen Steuergeraeten dieses BAP Stacks.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet
 *
 *  @retval BapErr_OK
 *          Das logische Steuergeraet konnte erfolgreich gestartet werden
 *
 *  @retval BapErr_InvalidArg
 *          Das angegebene LSG existiert nicht, oder es ist ein interner Fehler aufgetreten.
 *
 *  @retval BapErr_InvalidState
 *          BAP wurde nicht initialisiert oder laeuft schon.
 *
 *  @retval BapErr_SendBufferNotInitialized (tritt nur im FSG auf)
 *          Fuer ein Property wurde der Aufruf von BAP_InitSendBuffer vergessen.
 *
 *  @remarks
 *  Fuer alle logischen Steuergeraete muss zuerst BAP_Init aufgerufen werden, bevor
 *  das erste logische Steuergeraet gestartet werden darf.
 *  Ansonsten koennte es im schlimmsten Fall vorkommen (wenn mehrere logische 
 *  Steuergeraete die gleiche CAN-Id verwenden), dass eine Nachricht auf dem Bus 
 *  verloren geht oder ungueltige Daten uebertragen werden.
 */
BAP_IMPL_FAR BapError_et
BAP_Start(lsgId_t aLsgId);


/**
 *  Diese Funktion stoppt den Empfang und das Versenden von Nachrichten auf
 *  dem angegebenen logischen Steuergeraeten dieses BAP Stacks und initialisiert
 *  den Stack neu.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet
 *
 *  @retval BapErr_OK
 *          Das logische Steuergeraet konnte erfolgreich gestoppt und
 *          initialisiert werden.
 *
 *  @retval BapErr_InvalidArg
 *          Das angegebene LSG existiert nicht, oder es ist ein interner Fehler aufgetreten.
 *
 *  @retval BapErr_InvalidState
 *          BAP wurde nicht explizit ueber BAP_Init() initialisiert.
 */
BAP_IMPL_FAR BapError_et
BAP_Shutdown(lsgId_t aLsgId);


#if defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF)
/**
 *  Schaltet fuer die uebergebene Lsg in der
 *  Steuernachricht den Debug- und Diagnose-Betrieb ein oder aus.
 *
 *  Es kann immer nur eine Lsg zu einer Zeit ueberwacht werden.
 *
 *  @param aru8DebugCtrl
 *      Array mit 2 Bytes zum Schalten der Diagnose. Das 1.Byte gibt dabei
 *      die LsgID an und das 2.Byte den zu schaltenden Zustand (0=aus, 1=an).
 *
 *  @remarks
 *      Im Small FSG wird die Diagnose fuer alle LsgIDs eingeschaltet, falls
 *      die LsgID dem FSG bekannt ist.
 */
BAP_IMPL_FAR void
BAP_ProcessDebugControl(const uint8_t aru8DebugCtrl[2]);
#else
#define BAP_ProcessDebugControl(dummy) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BAP_H */

