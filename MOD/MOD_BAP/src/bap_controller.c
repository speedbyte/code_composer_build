/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/util/bap_controller.c $
 * $Revision: 27 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Implementierung diverser Funktionen
 *  im BAP Projekt, welche nicht genau einer Schicht (BAL, BPL, BCL)
 *  zugeordnet werden koennen. Siehe auch bap_util.c.
 *
 *  Enthaelt das Verteilen der zyklischen Task-Aufrufe.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2004-04-10   1.3         CRI         Processing Ueberwachungstask im FSG hinzugefuegt
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Reihenfolge in BAP_Task() von BAP_BCL_TaskSend();BAP_BCL_TaskRxNotify();
 *                                      auf BAP_BCL_TaskRxNotify();BAP_BCL_TaskSend(); geaendert.
 *                                      Damit kann eine Anfrage noch im gleichen Task direkt beantwortet werden.
 ******************************************************************/


/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_bpl.h"
#include "bap_bcl.h"
#include "bap_debug.h"

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

/* Definition (Implementierung) von statischen Funktionen */

/* Definition (Implementierung) von globalen Funktionen */

/* Diese Funktion ist in bap.h dokumentiert */
BAP_IMPL_FAR void
BAP_Task(void)
{
    /* Initialisiere Zeitueberwachung fuer Indications (wird beim Polling und Notify modifiziert) */
#ifdef BAP_TASKTIME_LIMIT
    BAP_u16TaskTimeCounter = 0;
#endif /* #ifdef BAP_TASKTIME_LIMIT */   
      
    /* Aufrufe an BAL => BAL hat keinen Task */
    
    /* Aufrufe an BPL */
#ifdef BAP_ASG    
    BAP_BPL_TaskRetryMonitoring();
    BAP_BPL_TaskHeartbeatMonitoring();
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG
    BAP_BPL_TaskProcessingDispatcher();
    BAP_BPL_TaskHeartbeatDispatcher();
#endif /* #ifdef BAP_FSG */

    /* Aufrufe an BCL */
    BAP_BCL_ReadRingbuffer();       /* Abfrage des Empfangsringbuffer */
    BAP_BCL_TaskRxNotify();         /* Notifizierung empfangener Nachrichten an BPL */
    BAP_BCL_TaskSend();             /* Versenden von Nachrichten ueber Low-Level-Treiber */
    
    /* Aufruf an das Debug-Interface */
    BAP_DBG_Task();
    
    return;
}
