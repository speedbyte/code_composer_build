/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: UTIL
 *
 * $Archive: /dev/BAP/include/bap_types.h $
 * $Revision: 43 $
 * $Modtime: 05-04-26 14:14 $
 * $Author: Andreasl $
 *
 * Ersteller: Manuel Wittek, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt alle zugelassenen Standard-Typdefinitionen 
 *  im BAP Projekt. Diese Typen bzw. erweiterte Typen (z.B. structs)
 *  duerfen vom Zulieferer veraendert werden, um eine Anpassung an 
 *  Compiler und Prozessor zu ermoeglichen. 
 *
 *  Diese Datei darf und soll vom Zulieferer modifiziert werden, 
 *  insbesondere wenn die Namen der Standardtypen bereits in anderen 
 *  Header-Dateien im Projekt definiert worden sind.
 *
 *  @todo
 *  Anpassung an Compiler und Prozessor 
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 *
 * 2006-03-28   1.4         MWI         Erstellen der Datei, in der 
 *                                      die vom Zulieferer anpassbaren 
 *                                      Typen gelagert sind
 *
 ******************************************************************/

#ifndef BAP_USER_STDTYPES_H
#define BAP_USER_STDTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */
#include <stddef.h>

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */

/* Makro-Definitionen */
#ifndef NULL
/** Konstante fuer einen Null-Zeiger */
#define NULL        ((void*)0)
#endif

#ifndef INT8_MIN
/** Konstante fuer den kleinsten Wert eines 8-Bit Wertes */
#define INT8_MIN    CHAR_MIN
#endif /* INT8_MIN */

#ifndef INT8_MAX
/** Konstante fuer den groessten Wert eines 8-Bit Wertes */
#define INT8_MAX    CHAR_MAX
#endif /* INT8_MAX */

#ifndef INT16_MIN
/** Konstante fuer den kleinsten Wert eines 16-Bit Wertes */
#define INT16_MIN   INT_MIN
#endif /* INT16_MIN */

#ifndef INT16_MAX
/** Konstante fuer den groessten Wert eines 16-Bit Wertes */
#define INT16_MAX   INT_MAX
#endif /* INT16_MAX */

#ifndef INT32_MIN
/** Konstante fuer den kleinsten Wert eines 32-Bit Wertes */
#define INT32_MIN   LONG_MIN
#endif /* INT32_MIN */

#ifndef INT32_MAX
/** Konstante fuer den groessten Wert eines 32-Bit Wertes */
#define INT32_MAX   LONG_MAX
#endif /* INT32_MAX */

#ifndef BAP_MAX_TASK_TIME
/**
 *  Diese Konstante dient dazu, die max. Anzahl an Indication-Aufrufen je BAP_Task zu begrenzen.
 *  @see BAP_IND_RX_TIME
 *  @see BAP_BUS_TX_TIME
 *  @see BAP_u16TaskTimeCounter
 */
#define BAP_MAX_TASK_TIME     (uint16_t)500
#endif /* BAP_MAX_TASK_TIME */

#ifndef BAP_IND_RX_TIME
 /**
 *  Diese Konstante wird bei jeder Indication auf BAP_u16TaskTimeCounter addiert.
 *  @remarks:
 *  Zusaetzlich wird die Laenge der Daten addiert.
 *  @see BAP_MAX_TASK_TIME
 *  @see BAP_BUS_TX_TIME
 *  @see BAP_u16TaskTimeCounter
 */
#define BAP_IND_RX_TIME     (uint16_t)80
#endif /* BAP_IND_RX_TIME */

#ifndef BAP_BUS_TX_TIME
/**
 *  Diese Konstante wird bei jedem Versenden einer Nachricht auf BAP_u16TaskTimeCounter addiert.
 *  @see BAP_IND_RX_TIME
 *  @see BAP_MAX_TASK_TIME
 *  @see BAP_u16TaskTimeCounter
 */
#define BAP_BUS_TX_TIME     (uint16_t)40
#endif /* BAP_BUS_TX_TIME */


/**
 *  Hiermit koennen alle Callbacks, welche von der Applikation implementiert werden muessen, in den FAR-Bereich gelegt werden.
 *  @remarks 
 *  Dies vergroessert den ROM Bedarf fuer den Code geringfuegig. 
 *  Beim Cosmic-Compiler fuer HCS 12 kann alternativ auch +modf angegeben werden.
 */
#ifndef BAP_FAR
#ifdef COMPILER_COSMIC
#define BAP_FAR @far
#endif  /* #ifdef COMPILER_COSMIC */
#endif /* #ifndef BAP_FAR */

/* Typ-Definitionen */

/**
 *  Dieser Typ enthaelt einen Zeiger auf ein Bytearray, welches
 *  untypisiert ist.
 */
typedef const unsigned char* ptr_t;

/**
 *  Dieser Typ enthaelt einen Zeiger auf ein Bytearray, welches
 *  untypisiert ist. Dieses ByteArray kann auch im Interrupt veraendert werden.
 */
typedef const volatile unsigned char* volatile_ptr_t;

/**
 *  Dieser Typ speichert einen ganzzahligen Wert in 8 Bit ohne Vorzeichen.
 */
#ifndef STDINT_H
typedef unsigned char uint8_t;
#endif
/**
 *  Dieser Typ enthaelt einen Wert vom Typ boolean.
 */
typedef uint8_t bool_t;

/**
 *  Dieser Typ wird fuer Bitfelder mit maximal 8 Bits verwendet.
 *  Nur int ist portabel und nach ANSI Standard erlaubt, verbraucht
 *  aber eventuell mehr Speicher als notwendig.
 *
 *  @remark: es muss immer ein unsigned Typ verwendet werden.
 *  Falls der Compiler es erlaubt und Einsparungen an RAM/ROM moeglich
 *  sind, kann dies auch auf unsigned char umdefiniert werden.
 */
typedef unsigned int bitfield_t;

/**
 *  Dieser Typ speichert einen ganzzahligen Wert in 16 Bit ohne Vorzeichen.
 */
#ifndef STDINT_H
typedef unsigned short uint16_t;
#endif
/**
 *  Dieser Typ speichert einen ganzzahligen Wert in 32 Bit ohne Vorzeichen.
 */
#ifndef STDINT_H
typedef unsigned long uint32_t;
#endif

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif      /* #ifndef BAP_USER_STDTYPES_H */

