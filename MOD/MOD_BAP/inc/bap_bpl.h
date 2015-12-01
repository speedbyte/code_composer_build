/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BPL
 *
 * $Archive: /dev/BAP/bpl/bap_bpl.h $
 * $Revision: 53 $
 * $Modtime: 3.05.05 10:14 $
 * $Author: Clausr $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Schnittstelle zur BPL Schicht im BAP
 *  Projekt. Diese Schnittstelle darf nicht ausserhalb der BAP
 *  Komponente verwendet werden, da sie in zukuenftigen Versionen
 *  Aenderungen unterliegen darf.
 *
 *  Stattdessen ist die freigegebene Schnittstelle BAP.h zu ver-
 *  wenden.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-10-24   1.3.1.2     ALA         const vor void Rueckgabetyp entfernt. #include "bap_config.h" ergaenzt,
 * 2005-04-29   1.3         CRI         Kommentare an BAP1.3 angepasst
 * 2004-12-03   1.2         ALA         Vorwaertsdeklaration ergaenzt
 * 2004-11-04   1.2         ALA         BAP_BPL_SendError Funktion hinzugefuegt.
 * 2004-09-03   1.2         JOK         BAP_BPL_ProcessHBMsg() eingefuegt
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         BAP_BPL_Shutdown() als Funktionspointer auf 
 *                                      BAP_BPL_Init() definiert
 ******************************************************************/

#ifndef BAP_BPL_H
#define BAP_BPL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"
#include "bap_privatetypes.h"
#include "bap_config.h"


/* Makro-Definitionen */

/* Typ-Definitionen */

/* Const Deklarationen */

/* Externe globale Variablen */

#ifdef BAP_FSG
/**
 *  Legt die zu sendenden Daten in die Ausgangs-Queue.
 *  Wird von BAP_InitSendBufferInt8/16/32/BS verwendet.
 *
 *  Ein Senden auf dem Uebertragungsmedium wird nicht
 *  getriggert.
 *
 *  @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>poLsgRomRow bezeichnet das logische Steuergeraet.<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>poFctRomRow bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>eOpCode bezeichnet den OpCode, welcher die Operation der Daten beschreibt.<br>
 *                 -\>pData muss untypisiert auf eine Bytefolge zeigen, deren Laenge in au16Length 
 *  uebergeben wird, und die zu sendenden Daten enthaelt<br>
 *                 -\>u16Length muss die Laenge der zu sendenden Daten enthalten
 *
 *  @remarks
 *  Zur Zeit gibt es im BPL keine eigene Implementierung, es wird lediglich
 *  per Funktionszeiger direkt auf die Implementierung im BCL verwiesen.
 *  Dadurch wird der Stackspeicherbedarf verringert.
 *
 *  @remarks
 *  Da die Verwendung wie eine Funktion erfolgt, wurde hier die Namensgebung
 *  fuer Funktionen verwendet.
 *  
 */
extern BAP_IMPL_FAR void (* const BAP_BPL_BufferSet) 
    (BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_FSG */


/* Deklarationen von Funktionen */


/**
 *  Wird vom BPL als Callback Funktion aufgerufen, z.B. beim Eintreffen neuer Nachrichten.
 *
 *  Die uebergeordnete Schicht muss diese Funktion implementieren.
 *
 *  Die Verbindung des CallBacks erfolgt automatisch ueber
 *  den Namen der Funktion und dem Linker.
 *
 *  @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>poLsgRomRow bezeichnet das logische Steuergeraet.<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>poFctRomRow bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>eOpCode bezeichnet den OpCode, welcher die Operation der Daten beschreibt.<br>
 *                 -\>pData zeigt untypisiert auf eine Bytefolge, deren Laenge in au16Length
 *  uebergeben wird, und die empfangenen Daten enthaelt<br>
 *                 -\>u16Length enthaelt die Laenge der empfangenen Daten
 *
 */
extern BAP_IMPL_FAR void 
BAP_BPL_DataReceived(BapInternalParameters_cpot apoPars);

/**
 *  Meldet Fehler in der Timeoutueberwachung (Retry/Heartbeat) oder vom BCL an 
 *  die uebergeordnete Schicht.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 *  @param aeErrorCode enthaelt den Fehlercode, der an BAL signalisiert wird.
 *
 */
extern BAP_IMPL_FAR void 
BAP_BPL_ErrorIndication(DBGVAR lsgId_t aLsgId
    , DBGVAR fctId_t aFctId
    , DBGVAR BapError_et aeErrorCode);


#if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT) 
/* Diese Funktion wird im FSG nie gebraucht und kann nicht ueber die BAP-API
 * von der Anwendung aufgerufen werden. Daher wird diese Funktion 
 * aus dem Code herausgenommen und um ROM zu sparen.
 */
/**
 *  Loescht ein Senden in der BPL Ebene
 *  Dies bedeutet das BAP_BCL_SendAbort aufgerufen wird und die Retryueberwachung der 
 *  Funktion deaktiviert wird
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @param aFctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 */
BAP_IMPL_FAR void 
BAP_BPL_AbortSend(DBGVAR lsgId_t aLsgId
    , DBGVAR fctId_t aFctId);
#endif /* #if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT)  */


/**
 *  Sendet die angegebenen Operation und Daten typlos mit der angegebenen Kennung.
 *
 *  @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>poLsgRomRow zeigt auf die Konfiguration des logischen Steuergeraetes<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>poFctRomRow zeigt auf die Konfiguration der angegebenen Funktion<br>
 *                 -\>eOpCode bezeichnet den OpCode, welcher die Operation der Daten beschreibt.<br>
 *                 -\>pData muss untypisiert auf eine Bytefolge zeigen, deren Laenge in au16Length 
 *                    uebergeben wird, und die zu sendenden Daten enthaelt<br>
 *                 -\>u16Length muss die Laenge der zu sendenden Daten enthalten
 *
 *  @remarks
 *  Der BPL ueberprueft, ob dieses Datenelement versendet werden darf.
 *  
 *  @retval BapErr_OK
 *      die Daten konnten erfolgreich an den BPL uebergeben werden
 *  @retval BapErr_InvalidArg 
 *      ein Parameter hat keinen zur Konfiguration passenden Wert
 *  @retval BapErr_Busy
 *      eine Retry-Ueberwachung laeuft gerade und es muss diese erst beendet werden
 *      bevor ein neuer Aufruf erlaubt ist
 */
BAP_IMPL_FAR DBGVAR BapError_et 
BAP_BPL_Send(BapInternalParameters_cpot apoPars);


#ifdef BAP_FSG
/**
 *  Sendet den angegebenen Fehlerwert mit der angegebenen Kennung.
 *
 *  @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>poLsgRomRow muss auf das aktuelle LSG zeigen<br>
 *                 -\>poFctRomRow muss auf die aktuelle FCT zeigen<br>
 *                 -\>pData muss auf den Fehlerwert zeigen (als uint8_t codiert)<br>
 *                 -\>u16Length muss mit dem Wert 1 vorbelegt sein<br>
 *                 -\>eOpCode muss mit BapOp_PropError vorbelegt sein
 *
 *  @remarks
 *  Der BPL ueberprueft, ob dieses Datenelement versendet werden darf.
 *  
 *  @retval BapErr_OK
 *      die Daten konnten erfolgreich an den BPL uebergeben werden
 *  @retval BapErr_InvalidArg 
 *      ein Parameter hat keinen zur Konfiguration passenden Wert
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BPL_SendError(BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_FSG */


#ifdef BAP_FSG
/**
 *  Versendet die Nutzdaten erneut, jedoch mit geaendertem OpCode.
 *  Diese Funktion wird bei der Beantwortung von Get-Anfragen und 
 *  der Beantwortung von GetAll-Anfragen verwendet.
 *
 *  @param apoPars ->eOpCode gibt den aus dem Request resultierenden OpCode zurueck
 *
 *  @remarks
 *  Ein weiterer Aufruf von BAP_BCL_Resend fuehrt anschliessend dazu,
 *  dass der neu gesetzte OpCode verwendet wird.
 *  
 *  @remarks
 *  Die Funktionalitaet der Funktion BAP_BCL_ResendWithOpCode wird im BPL 
 *  um das Retriggern des HB-Timers erweitert.
 */
BAP_IMPL_FAR void 
BAP_BPL_ResendStatus(BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_FSG */

/**
 *  Diese Funktion wird von BAP_Init aufgerufen.
 *
 *  @param apoLsgRomRow bezeichnet das logische Steuergeraet.
 *
 *  @returns Es werden keine Pruefungen ueber die des BAL hinaus gemacht. Daher
 *      wird immer BapErr_OK zurueckgegeben.
 */
BAP_IMPL_FAR DBGVAR BapError_et 
BAP_BPL_Init(BapLsgRomRow_pot apoLsgRomRow);

/**
 * Diese Funktion wird von BAP_Start aufgerufen.
 *
 *  @param apoLsgRomRow bezeichnet das logische Steuergeraet.
 *
 *  @returns Es werden keine Pruefungen ueber die des BAL hinaus gemacht. Daher
 *      wird immer BapErr_OK zurueckgegeben.
 */
#ifndef BAP_FSG /* reines ASG */
#define BAP_BPL_Start(apoLsgRomRow) BapErr_OK
#else
BAP_IMPL_FAR DBGVAR BapError_et 
BAP_BPL_Start(BapLsgRomRow_pot apoLsgRomRow);
#endif /* #ifndef BAP_FSG */

/**
 *  Diese Funktion wird von BAP_Shutdown aufgerufen.
 *
 *  @param apoLsgRomRow bezeichnet das logische Steuergeraet.
 *
 *  @returns Es werden keine Pruefungen ueber die des BAL hinaus gemacht. Daher
 *      wird immer BapErr_OK zurueckgegeben.
 */
#define BAP_BPL_Shutdown(apoLsgRomRow)    BAP_BPL_Init(apoLsgRomRow)

#ifdef BAP_ASG
/**
 * Diese Funktion wird von BAP_Task aufgerufen.
 *
 * Hier wird die Heartbeat Zeitueberwachung im ASG durchgefuehrt.
 */
BAP_IMPL_FAR void 
BAP_BPL_TaskHeartbeatMonitoring(void);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG
/**
 * Diese Funktion wird von BAP_Task aufgerufen.
 *
 * Hier wird das zyklische Versenden der Heartbeat
 * Nachrichten im FSG erledigt.
 */
BAP_IMPL_FAR void 
BAP_BPL_TaskHeartbeatDispatcher(void);
#endif /* #ifdef BAP_FSG */

#ifdef BAP_ASG
/**
 * Diese Funktion wird von BAP_Task aufgerufen.
 *
 * Hier werden die Zeitueberwachungen und
 * Wiederholungen fuer zu bestaetigende Auftraege
 * (DataSetGet, DataGet) durchgefuehrt.
 */
BAP_IMPL_FAR void 
BAP_BPL_TaskRetryMonitoring(void);
#endif /* #ifdef BAP_ASG */


#if defined(BAP_FSG) && defined(BAP_USES_METHODS)
/**
 * Diese Funktion wird von BAP_Task aufgerufen.
 *
 * Hier werden die BapInd_Processing_CNF fuer Methoden im FSG erzeugt, welche
 * von der Application mit einem BapReq_Processing quittiert werden sollten.
 */
BAP_IMPL_FAR void 
BAP_BPL_TaskProcessingDispatcher(void);
#else
#define BAP_BPL_TaskProcessingDispatcher() ((void)0)
#endif /* #if defined(BAP_FSG) && defined(BAP_USES_METHODS) */

#ifdef BAP_ASG
/**
 * Diese Funktion wird nach dem Empfang einer Heartbeat-Nachricht vom BPL oder
 * bei der Auswertung der GETALL-Nachricht vom BAL aufgerufen.
 * 
 * Es wird geprueft, ob eine Heartbeat-Ueberwachung gestartet werden soll.
 * Falls ja werden die Timer berechnet und die Ueberwachung gestartet.
 */
BAP_IMPL_FAR void 
BAP_BPL_ProcessHBMsg(BapLsgRomRow_pot apoLsgRomRow
    , DBGVAR uint8_t au8HearbeatTime);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_USES_ACKNOWLEDGE
/**
 *  Diese Funktion wird vom BPL aufgerufen, wenn eine Nachricht erfolgreich verschickt wurde.
 *
 *  @param apoFctRomRow zeigt auf die Funktion 
 *  @param aeOpCode enthaelt den gesendeten OpCode
 */
BAP_IMPL_FAR void
BAP_BPL_Acknowledge(BapFctRomRow_pot apoFctRomRow, BapOpCodes_et aeOpCode);
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BPL_H */

