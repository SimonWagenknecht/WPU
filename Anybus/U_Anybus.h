/***************** Any-Bus ***************/
// Diro - 20.09.2011

//**************** Zugriff von Anybus auf unsere Objekte ******************
#ifndef U_ANYBUS_H_INCLUDED
#define U_ANYBUS_H_INCLUDED

//#include "anybus.h"

#define ANYBUS_NONE_ACCESS						0x00			// Wenn fehler bei der Adressermittlung entstehen
#define ANYBUS_READ_ACCESS						0x01
#define ANYBUS_WRITE_ACCESS           0x02

// Any-Bus Empfang

#define ANY_RX_ENDE				0x80
#define ANY_RX_TIMEOUT		0x40
#define ANY_RX_BEGIN			0x20
#define ANY_TX_BEGIN			0x10
#define ANY_TX_ENDE				0x08
#define ANY_RX_EXECPTION	0x02
#define ANY_RX_ERR				0x01

typedef struct {
	void * pValue;		// Adresse von Riedel
	int	iSubDAE;					// Sub-DAE/Exp-Nummer f�r dieses Element
	// Datentyp - Anybus?? -> �ber die Riedel-konvertierung
	unsigned char	ucZugriff;	// ANYBUS_READ_ACCESS oder ANYBUS_WRITE_ACCESS oder beides? -> *-Parameter ANYBUS_READ_ACCESS, alle anderen ver�nderbar
	// Instancenumber automatisch??
} anybusTableRow;

typedef struct {
	unsigned int	uiTableRow;				// Eintrag in der Tabelle f�r anybusKonfiguration
	unsigned int	uiDAE;						// DAE-Nummer f�r dieses Element
	unsigned char	ucZugriff;				// �berschriebener Zugriff auf dieses Element  *-Parameter ANYBUS_READ_ACCESS, alle anderen ver�nderbar
	unsigned int	uiOrderNumber;		// laufende Nummer f�r alle initialisierten Element im Anybus (kein ANYBUS_NONE_ACCESS)
	unsigned int	uiInstanceNumber;	// Instancenumber f�r diese Element im Anybus
	unsigned int	uiInstanceAnzahl;	// Anzahl der Instancenumber f�r diese Element im Anybus (Uhrzeit, DATE haben mehr als eine INstance
	signed int	siProcessIndex;		// Position des Elementes in den Processdaten (Write oder Read)
	unsigned int	uiObjectTyp;	// Objekttyp - Abh�ngig vom Modul bei Bacnet ist es der Bacnet-Typ (Analog, Digital, Multistate)
	unsigned long	ulObjectInstance; // Instancenummer f�r das Hostobject - Bacnet kann 0..2039 Analog, 0..2039 Digital und 0..2039 Multistate Objekte haben
} anybusDataTableRow;

#endif