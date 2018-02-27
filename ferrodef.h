#ifndef FERRODEF_H_INCLUDED
#define FERRODEF_H_INCLUDED

/*--------------------------------------------------------------*/
/* 					Speicherplätze	im FERRO-RAM	 FM24C256 						*/
/*--------------------------------------------------------------*/
// Speicher: 256 KBit (32K x 8 BIT)			Adressbereich: 0 - 7FFFH
// Zugriff über Adresse XEEPADR+2	(0xA4	Speicherriegel 2.Platz)

#define FERRO_SIZE			0x8000

// Infobereich für den Schnellzugriff bei der Auswahl der Pages auf der SD-Karte
// Aufbau pro DAE: aktuelle Datenpage-Nummer                    2 Byte     
//                 Anzahl der Einträge auf dieser Datenpage     1 Byte     
//                 Offset pro Eintrag  auf dieser Datenpage     1 Byte
//                 aktuelle Verweispage-Nummer                  1 Byte 
//                 Anzahl der Einträge auf dieser Verweispage   1 Byte
//                 Zeiger auf ältesten Eintrag in der Hauptpage 1 Byte   wird nach Datenpage-Überlauf wirksam (Ringpuffer)

#define DAE_INFO_LENG    8

// Adressen-Bereich für Komtab ohne Expandparameter
#define DAE_INFO_ADR      0x0000  // DAE 0  8 Byte
//                        0x0008  // DAE 1  8 Byte usw. bis DAE 511
// letzter                0x0FF8  // DAE 511


// Adressen-Bereich für Expandparameter
#define DAE_INFO_EXP_ADR 		0x1000	// Die Speicherplätze werden dynamisch zugewiesen
 

#define FERRO_INIT_KZ_ADR		0x7FF0	// 2 Byte Platz für Initialisierungkennzeichen

#endif // FERRODEF_H_INCLUDED