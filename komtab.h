/* 			Header für Komtab-Versionen																					*/
/* 	Die Versionsnummern werden in der Parli eingetragen, als Kennzeichen
		für die Aufnahme eines Parameters in die automatisch erstellte Komtab.
		
		Werden bei einem schon existierenden Leittechnikprojekt neue Parameter
		in die Parli eingefügt, dann müssen diese mit einer höheren Versionsnummer
		versehen werden. Diese Parameter werden dann an die Komtab angehangen. 
*/		
#ifndef KOMTAB_H_INCLUDED
#define KOMTAB_H_INCLUDED


#define V0			0		// kein Komtab-Eintrag		
#define V1			1		// Standardversion 
#define V2			2		// 1. Änderung 
#define V3			3		// 2. Änderung 
#define V4			4		// 3. Änderung 

#define A1			1		// Alarme
#define A2			2		// Alarme
#define A3			3		// Alarme
#define A4			4		// Alarme

#define E1			1		// Ereignisse
#define E2			2		// Ereignisse
#define E3			3		// Ereignisse
#define E4			4		// Ereignisse

#endif // KOMTAB_H_INCLUDED