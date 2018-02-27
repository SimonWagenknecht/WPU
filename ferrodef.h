#ifndef FERRODEF_H_INCLUDED
#define FERRODEF_H_INCLUDED

/*--------------------------------------------------------------*/
/* 					Speicherpl�tze	im FERRO-RAM	 FM24C256 						*/
/*--------------------------------------------------------------*/
// Speicher: 256 KBit (32K x 8 BIT)			Adressbereich: 0 - 7FFFH
// Zugriff �ber Adresse XEEPADR+2	(0xA4	Speicherriegel 2.Platz)

#define FERRO_SIZE			0x8000

// Infobereich f�r den Schnellzugriff bei der Auswahl der Pages auf der SD-Karte
// Aufbau pro DAE: aktuelle Datenpage-Nummer                    2 Byte     
//                 Anzahl der Eintr�ge auf dieser Datenpage     1 Byte     
//                 Offset pro Eintrag  auf dieser Datenpage     1 Byte
//                 aktuelle Verweispage-Nummer                  1 Byte 
//                 Anzahl der Eintr�ge auf dieser Verweispage   1 Byte
//                 Zeiger auf �ltesten Eintrag in der Hauptpage 1 Byte   wird nach Datenpage-�berlauf wirksam (Ringpuffer)

#define DAE_INFO_LENG    8

// Adressen-Bereich f�r Komtab ohne Expandparameter
#define DAE_INFO_ADR      0x0000  // DAE 0  8 Byte
//                        0x0008  // DAE 1  8 Byte usw. bis DAE 511
// letzter                0x0FF8  // DAE 511


// Adressen-Bereich f�r Expandparameter
#define DAE_INFO_EXP_ADR 		0x1000	// Die Speicherpl�tze werden dynamisch zugewiesen
 

#define FERRO_INIT_KZ_ADR		0x7FF0	// 2 Byte Platz f�r Initialisierungkennzeichen

#endif // FERRODEF_H_INCLUDED