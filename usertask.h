/*          usertask.h                                  */
/*  Header für  User - Task's													  */
/*																											*/
/*	Die Zeitbasis (Tick) beträgt	25 ms (40 Hz)
		
		STAT....Status (char)   0x00 = Task aktiv,  0x80 = Task gesperrt
		BICB....Kennzeichen, ob der Task den Board-I²C-Bus (bicbus) benutzt
		ZAEHL...Zeitzähler (int), wird nach Ausführung des Task's vom Zeitgeber neu geladen.
						Der Initialisierungswert gibt an, nach welcher Anzahl von Ticks der
						Task zum ersten mal gestartet wird.
		ZEITG...Zeitgeber (int), Taskwiederholzeit = Wert * 25 ms
		TASK....Taskname,  auf richtige Schreibweise (Groß- und Kleinbuchstaben) achten.  
*/
#ifndef USERTASK_H_INCLUDED
#define USERTASK_H_INCLUDED

#include "projdef.h"

#define	U1_STAT			0x00					
#define U1_BICB			NOIC
#define	U1_ZAEHL		40						// 1 Sek Startverzögerung
#define	U1_ZEITG		40						// 1 Sek
#define	U1_TASK			Alarme

#define	U2_STAT			0x00
#define U2_BICB			BICB					// enthält bicbus-Aufrufe
#define	U2_ZAEHL		40						// 1 s Startverzögerung
#define	U2_ZEITG		40						// 1 s								
#define	U2_TASK			Output				

#define	U3_STAT			0x00
#define U3_BICB			NOIC
#define	U3_ZAEHL		41						// 1,025 s Startverzögerung								
#define	U3_ZEITG		2							// 50 ms						
#define	U3_TASK			InoutR

#define	U4_STAT			0x00
#define U4_BICB			NOIC
#define	U4_ZAEHL		200						// 5s Startverzögerung
#define	U4_ZEITG		40/HKMAX			// 1s / HKMAX (alternierend für bis zu 4 Regelkreise)
#define	U4_TASK			RegelHk

#define	U5_STAT			0x00
#define U5_BICB			NOIC
#define	U5_ZAEHL		161						// 4,025 s Startverzögerung						
#define	U5_ZEITG		400/HKMAX			// 10 s / HKMAX (alternierend für bis zu 4 Regelkreise)
#define	U5_TASK			Absenk				

#define	U6_STAT			0x00
#define U6_BICB			NOIC
#define	U6_ZAEHL		162						// 4,05 s Startverzögerung		
#define	U6_ZEITG		400/HKMAX			// 10 s / HKMAX (alternierend für bis zu 4 Regelkreise)
#define	U6_TASK			Bedarf

#define	U7_STAT			0x00
#define U7_BICB			BICB					// enthält bicbus-Aufrufe
#define	U7_ZAEHL		402						// 10,05 s Startverzögerung	
#define	U7_ZEITG		1200					// 30 Sekunden		
#define	U7_TASK			Steuer				

#define	U8_STAT			0x00
#define U8_BICB			BICB					// enthält bicbus-Aufrufe
#define	U8_ZAEHL		403						// 10,075 Sek Startverzögerung
#define	U8_ZEITG		8							// 200 ms
#define	U8_TASK			Monza

//------------------Kessel, Steuerung und Regelung, Anzahl 1-8---------------------
#if KEANZ > 0	
#define	U9_STAT			0x00
#define U9_BICB			BICB
#define	U9_ZAEHL		404						// 10,1 s Startverzögerung		
#define	U9_ZEITG		40/KEMAX			// 1 sek / KEMAX (alternierend für bis zu 8 Kessel)
#define	U9_TASK			SteuerKe
#else 
#define	U9_STAT			0x80					
#define U9_BICB			NOIC
#define	U9_ZAEHL		4
#define	U9_ZEITG		4
#define	U9_TASK			Leer
#endif

#define	U10_STAT		0x80					
#define U10_BICB		NOIC
#define	U10_ZAEHL		4
#define	U10_ZEITG		4
#define	U10_TASK		Leer

//------------------Netztrennung , Vorregelung ------------------------------------
#if NTANZ == 1
#define	U11_STAT		0x00
#define U11_BICB		BICB
#define	U11_ZAEHL		163						// 4,1 s Startverzögerung		
#define	U11_ZEITG		40						// 1 sek
#define	U11_TASK		RegelNt
#else
	#if NTANZ == 2
	#define	U11_STAT		0x00
	#define U11_BICB		BICB
	#define	U11_ZAEHL		2403				// 2400,075 s Startverzögerung		
	#define	U11_ZEITG		40					// 1 sek
	#define	U11_TASK		Regel2Wt		
	#else
	#define	U11_STAT		0x80					
	#define U11_BICB		NOIC
	#define	U11_ZAEHL		4
	#define	U11_ZEITG		4
	#define	U11_TASK		Leer
	#endif
#endif

//------------------Warmwasser Steuerung und Regelung ----------------------------
#if WWANZ > 0							// Steuerung
#define	U12_STAT		0x00
#define U12_BICB		BICB					// enthält bicbus-Aufrufe
#define	U12_ZAEHL		164						// 4,15 s Startverzögerung							
#define	U12_ZEITG		1200					// 30 Sekunden		
#define	U12_TASK		SteuerWw

	#if ( REGEL_WW1 == 1 || REGEL_WW2 == 1 )			// mit Regelung
#define	U13_STAT		0x00
#define U13_BICB		NOIC					
#define	U13_ZAEHL		165						// 4,2 s Startverzögerung	
#define	U13_ZEITG		40						// 1 sek
#define	U13_TASK		RegelWw
	#else										// ohne Regelung
#define	U13_STAT		0x80					
#define U13_BICB		NOIC
#define	U13_ZAEHL		4
#define	U13_ZEITG		4
#define	U13_TASK		Leer
	#endif

#else											// kein Warmwasser
#define	U12_STAT		0x80					
#define U12_BICB		NOIC
#define	U12_ZAEHL		4
#define	U12_ZEITG		4
#define	U12_TASK		Leer

#define	U13_STAT		0x80					
#define U13_BICB		NOIC
#define	U13_ZAEHL		4
#define	U13_ZEITG		4
#define	U13_TASK		Leer
#endif

//------------------ Steuerung Solar ----------------------------------------------
#if SOLANZ > 0
#define	U14_STAT		0x00
#define U14_BICB		BICB
#define	U14_ZAEHL		166
#define	U14_ZEITG		40//400/SOLMAX		// 10 Sekunden pro Solar-Kreis
#define	U14_TASK		SteuerSol
#else
#define	U14_STAT		0x80					
#define U14_BICB		NOIC
#define	U14_ZAEHL		4
#define	U14_ZEITG		4
#define	U14_TASK		Leer
#endif

//------------------ Adaption ------------------------------------------------------
#if ADAP_STAT_HK1 > 0 || ADAP_STAT_HK2 > 0 || ADAP_STAT_HK3 > 0 || ADAP_STAT_HK4 > 0
#define	U15_STAT		0x00
#define U15_BICB		BICB
#define	U15_ZAEHL		166							
#define	U15_ZEITG		2400/HKMAX		//  1 Minute pro HK ! max. 4 HK !					
#define	U15_TASK		Adapt
#else
#define	U15_STAT		0x80					
#define U15_BICB		NOIC
#define	U15_ZAEHL		4
#define	U15_ZEITG		4
#define	U15_TASK		Leer
#endif

//------------------ Wärmemenge ------------------------------------------------------
#if WMENG == 1
#define	U16_STAT		0x00
#define U16_BICB		BICB					// enthält bicbus-Aufrufe
#define	U16_ZAEHL		1366					// 34,15 s Startverzögerung		
#define	U16_ZEITG		2							// 50 msec (4 Zähler), jeder Zähler 200 msek 
#define	U16_TASK		Wmeng

#else
#define	U16_STAT		0x80					
#define U16_BICB		NOIC
#define	U16_ZAEHL		4
#define	U16_ZEITG		4
#define	U16_TASK		Leer
#endif

//------------------ Diagnose ------------------------------------------------------
#if DIAG == 1
#define	U17_STAT		0x00
#define U17_BICB		XICB|BICB			// enthält xicbus- und bicbus-Aufrufe
#define	U17_ZAEHL		403						// 10,075 s Startverzögerung	
#define	U17_ZEITG		400						// 10 Sekunden, immer nur teilweise		
#define	U17_TASK		Diagnose		
#else
#define	U17_STAT		0x80					
#define U17_BICB		NOIC
#define	U17_ZAEHL		4
#define	U17_ZEITG		4
#define	U17_TASK		Leer
#endif

//------------------ Mittlere Leistung ------------------------------------------------------------
#if LEIST_BER == 1
#define	U18_STAT		0x00
#define U18_BICB		BICB
#define	U18_ZAEHL		404						// 10,1 sec Startverzoegerung
#define	U18_ZEITG		2400					// 1 min
#define	U18_TASK		LeistgMittel
#else
#define	U18_STAT		0x80					
#define U18_BICB		NOIC
#define	U18_ZAEHL		4
#define	U18_ZEITG		4
#define	U18_TASK		Leer
#endif

//------------------ CopyCheck ------------------------------------------------------
#if COPY_EMPF > 0 || COPY_SEND > 0
#define	U19_STAT		0x00
#define U19_BICB		NOIC
#define	U19_ZAEHL		167
#define	U19_ZEITG		2400					// 1 min
#define	U19_TASK		CopyCheck
#else
#define	U19_STAT		0x80					
#define U19_BICB		NOIC
#define	U19_ZAEHL		4
#define	U19_ZEITG		4
#define	U19_TASK		Leer
#endif

//------------------ Heizgradtage ------------------------------------------------------
#if GENI == 1
#define	U20_STAT		0x00					
#define U20_BICB		NOIC
#define	U20_ZAEHL		4
#define	U20_ZEITG		4
#define	U20_TASK		Genibus	
#else
#if HEIZGRD == 1
#define	U20_STAT		0x00
#define U20_BICB		BICB
#define	U20_ZAEHL		404						// 10,1 sec Startverzoegerung
#define	U20_ZEITG		2400					// 1 Minute
#define	U20_TASK		HeizGradTag
#else
#define	U20_STAT		0x80					
#define U20_BICB		NOIC
#define	U20_ZAEHL		4
#define	U20_ZEITG		4
#define	U20_TASK		Leer
#endif
#endif

//------------------ Archiv ------------------------------------------------------------
#if ARCHIV_ANL == 1
#define	U21_STAT		0x00
#define U21_BICB		XICB|BICB
#define	U21_ZAEHL		188							
#define	U21_ZEITG		40					
#define	U21_TASK		Archiv
#else
#define	U21_STAT		0x80					
#define U21_BICB		NOIC
#define	U21_ZAEHL		4
#define	U21_ZEITG		4
#define	U21_TASK		Leer
#endif

//------------------ AnalogEA	: Skalierung und Dämpfung der 0-10V Eingänge  ------------
#if ( ANALOG_AE || RM_POWER_ANZ || AE_DRUCK_ANZ )
#define	U22_STAT		0x00
#define U22_BICB		NOIC
#define	U22_ZAEHL		83								// 2,075 s Startverzögerung				
#define	U22_ZEITG		40 / 8						// 125 ms -> 1 s pro Eingang
#define	U22_TASK		AnalogEA					
#else
#define	U22_STAT		0x80					
#define U22_BICB		NOIC
#define	U22_ZAEHL		4
#define	U22_ZEITG		4
#define	U22_TASK		Leer
#endif

//------------------ Leistungsbegrenzung -----------------------------------------------
#if LEIST_BEGR == 1
#define	U23_STAT		0x00
#define U23_BICB		BICB
#define	U23_ZAEHL		43						// 1,1 sec Startverzoegerung
#define	U23_ZEITG		4							// 100 ms
#define	U23_TASK		LeistgGrenz
#else
#define	U23_STAT		0x80					
#define U23_BICB		NOIC
#define	U23_ZAEHL		4
#define	U23_ZEITG		4
#define	U23_TASK		Leer
#endif

//------------------ Datenmanager -------------------------------------------------------
#if DM_MODE == 1
#define	U24_STAT		0x00					
#define U24_BICB		NOIC
#define	U24_ZAEHL		4
#define	U24_ZEITG		4
#define	U24_TASK		DTimer
#else
#define	U24_STAT		0x80					
#define U24_BICB		NOIC
#define	U24_ZAEHL		4
#define	U24_ZEITG		4
#define	U24_TASK		Leer
#endif

//------------------ Benutzersteuerung mit UNI-Elementen --------------------------------
#if STEUER_UNI == 1
#define	U25_STAT		0x00					
#define U25_BICB		NOIC
#define	U25_ZAEHL		40
#define	U25_ZEITG		40
#define	U25_TASK		SteuerUni
#else
#define	U25_STAT		0x80					
#define U25_BICB		NOIC
#define	U25_ZAEHL		4
#define	U25_ZEITG		4
#define	U25_TASK		Leer
#endif

//------------------ Extrahieren von Modbus-Bitfeldern -----------------------------
#if (MODBUS_UNI > 0) && (MODBUS_EXT == 1)
#define	U26_STAT		0x00					
#define U26_BICB		NOIC
#define	U26_ZAEHL		46
#define	U26_ZEITG		40
#define	U26_TASK		ModbusExtract
#else
#define	U26_STAT		0x80					
#define U26_BICB		NOIC
#define	U26_ZAEHL		4
#define	U26_ZEITG		4
#define	U26_TASK		Leer
#endif

//----------------- Wilo Pumpen  --------------------------------------------------------------
#if WILO_MODBUS == 1
#define	U27_STAT		0x00					
#define U27_BICB		NOIC
#define	U27_ZAEHL		405						// 10,075 Sek Startverzögerung
#define	U27_ZEITG		40						// 1 s
#define	U27_TASK		WiloPumpenSteuerung
#else
#define	U27_STAT		0x80					
#define U27_BICB		NOIC
#define	U27_ZAEHL		4
#define	U27_ZEITG		4
#define	U27_TASK		Leer
#endif	

//----------------- Wärmepumpensteuerung --------------------------------------------------------------
#define	U28_STAT		0x00					
#define U28_BICB		NOIC
#define	U28_ZAEHL		40
#define	U28_ZEITG		40
#define	U28_TASK		SteuerWPU

//----------------- Reserve --------------------------------------------------------------
#define	U29_STAT		0x80					
#define U29_BICB		NOIC
#define	U29_ZAEHL		4
#define	U29_ZEITG		4
#define	U29_TASK		Leer

//------------------ letzter Task: Archivierung mit SD-Card ------------------------------
#if ARCHIV_SDM == 1	//...........Josch-SDM : Archiv mit SD-Card-Memory
#define	U30_STAT		0x00					
#define U30_BICB		XICB|SPIB
#define	U30_ZAEHL		400					// 10 sec Startverzoegerung
#define	U30_ZEITG		4						// 100 ms
#define	U30_TASK		ArchivSDM
#else
#define	U30_STAT		0x80					
#define U30_BICB		NOIC
#define	U30_ZAEHL		4
#define	U30_ZEITG		4
#define	U30_TASK		Leer
#endif	

#endif	// USERTASK_H_INCLUDED

