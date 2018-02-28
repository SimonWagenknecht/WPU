/* Definitionen im User-Bereich																	*/
#ifndef USERDEF_H_INCLUDED
#define USERDEF_H_INCLUDED

#define LED_DEBUG		0							// 1 = blendet Standard-Led-Ausgaben aus, macht den Weg frei für Led-Debug
#define COPY_TEST		0							// 1 = ermöglicht Test in CopyCheck.c

// Serieller Schnittstellen Index
#define SX				-1
#define S1				0
#define S2				1
#define S3				2

// Schnittstellen Eigenschaften
#define NULL_MODE 0xFF					// dummy, (alternativ MASTER, SLAVE)
#define NULL_PARI 0xFF          // dummy, (alternativ NOPAR, ODD, EVEN)
#define NULL_STOP 0             // dummy, (alternativ 1, 2) 

#define NOPAR     0
#define ODD       1
#define EVEN      2

// Kennzeichen im Noinit-Rambereich
#define RAMEX1		0x3456
#define RAMEX			0x4567

// Funktionen der Digitalausgänge
#define	STATISCH		1
#define DYNAMISCH		2
#define VERRIEG1		3
#define VERRIEG2		4

// Typen von Speicherplätzen für Alarmbehandlung
#define DIREKT			0						// char (kein Pointer)
#define MWSP				1						// Messwertspeicher
#define P_MWSP			2						// Pointer auf Speicher vom Typ mwsp
#define P_DINP			3						// Pointer auf Speicher vom Typ dinpsp

// Namen von Basis- und Erweiterungsmodulen
#define R36				0							// RIEcon 36
#define R37				1							// RIEcon 37
#define R38				2							// RIEcon 38
#define R39				3							// RIEcon 39
#define R33				4							// RIEcon 33
#define R66				5							// RIEcon 66 EA-Simulation

#define R3MAX			4							// maximal 4 Erweiterungsmodule möglich
#define ZAUFMAX		28						// maximal Aufträge zum Zähler lesen
		
// UNI Ein- und Ausgänge
#define U1				0
#define U2				1
#define U3				2
#define U4				3
#define U5				4
#define U6				5
#define U7				6
#define U8				7

// Bedarfsanforderungen
#define BEDARF_JA					1
#define BEDARF_NEIN				2
#define BEDARF_UNBEKANNT	0

// Textanzeige Betriebs-Regie
#define REG_SCHALTER			1		// Schalter	(Absenkung)
#define REG_BEDARF				2		// Bedarf		(Absenkung bzw. Kaskade)
#define REG_FERIEN				3		// Ferien		(Absenkung)
#define REG_ZEIT					4		// Zeit			(Absenkung bzw. Kaskade)

#define TXT_DEFAULT				"               "
#define TXT_SCHALTER			"SCHALTER       "	// 01	Schalter
#define TXT_BEDARF				"BEDARF         "	// 02	Bedarf
#define TXT_FERIEN				"FERIEN         "	// 03	Ferien
#define TXT_ZEIT					"ZEIT           "	// 04	Zeit

// Wilo-Pumpen. Benutzt in U_Wilo.c und wiloParli.h
#define HOLDING_REG_CNT 4

//---------------------------------------------------------------------------------------------------
// Alarm Klassen in der parli
//---------------------------------------------------------------------------------------------------
//								0				// Element ist keine Alarmklasse
#define GRUP			0x80		// Element ist der Sammelalarm einer Klasse (Oder-Bedingung mit entspr. Sammelalarmklasse) 

#define S_CLASS_ANZ	5+1		// Anzahl der Sammelalarmklassen (maximal 8, Index 0 wird nicht benutzt)
// Sammelalarmklassen (Index)
#define STANDARD	1				// Element gehört zum Sammelalarm der Klasse Standardalarme
#define FUEHLER   2				// Element gehört zum Sammelalarm der Klasse Fühlerfehler
#define MODULE		3				// Element gehört zum Sammelalarm der Klasse Modulfehler
#define KESSEL		4				// Element gehört zum Sammelalarm der Klasse Kessel
#define SOLAR			5				// Element gehört zum Sammelalarm der Klasse Solar

// Einzelalarme- und Ereignisklassen
#define EINZEL			10			// Element gehört zur Klasse Einzelalarme
#define EREIGNIS		11			// Element gehört zur Klasse Ereignisse			(kein Sammelalarm, keine rote LED-Anzeige, keine LCD-Anzeige) 
#define ANZEIGE			12			// Element gehört zur Klasse Anzeige			  (kein Sammelalarm, keine rote LED-Anzeige, nur   LCD-Anzeige)
#define ANZEIGE_ROT	13			// wie ANZEIGE, aber rote LED ein

//----------------------------------------------------------------------------------------------------
// Alarm Funktionen in der parli.	Ausführung erfolgt in Alarme.c siehe void AlarmFunktion(char alfunc)
//----------------------------------------------------------------------------------------------------
//								0				// Element hat keine Alarmfunktion
#define PUMP_AUS	1				// Element ruft die Funktion  PumpenAus() auf

// Kennzeichen für alle Expansionsparameter in einer Expansionsgruppe. Wird für die automatische Auswahl in der Funktion user_erste_zeile(...) benutzt.  
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#define EXP_RFS		30			// Expansionsgruppe RFS		in parli.h Funksensoren

#define EXP_GBP		31			// Expansionsgruppe GBP		in parli_genibus.h	Gruppe GENIBUS	
#define EXP_GEN		32			// Expansionsgruppe GEN		in parli_genibus.h	Gruppe Genibus Pumpen	

#define EXP_WLP		33			// Expansionsgruppe WLP		in parli_wilo.h

#define EXP_ZMB		34			// Expansionsgruppe ZMB		in parli_mbus.h
#define MAX_EXP_RFS	12
#define MAX_EXP_GBP	BUS_PU_MAX
#define MAX_EXP_GEN	BUS_PU_MAX
#define MAX_EXP_WLP	BUS_PU_MAX
#define MAX_EXP_ZMB	MBUSANZ


#define IDX0_FELD	99			// Kennzeichen für die Funktion get_feldadresse(...).
                          // Wenn ein Parameter aus einer Expansionsgruppe mit dem Index 0 zusätzlich in einer anderen Gruppe angezeigt werden soll, 
                          // dann verhindert dieses Kennzeichen das Benutzen der eingestellten Memberauswahl  (z,B Pumpennummer).

/*
 Definitionen für Ein- und Ausgangssignale
----------------------------------------------------------------------
	Typ(0-F), Klasse(00-FF), Num(0-F)
		
	Typ:		1 = Pt1000	Eingang
					2 = 0-10V		Eingang
					3	= Digital Eingang
					4 = Zähl		Eingang
					5 = Analog	Ausgang
					6 = Digital	Ausgang
					..
					
	Klasse:	Pt1000-Eingänge											reservierte Speicherplätze
					00 = Dummy								TP_FREI		1
					01 = Außentemperatur			TA				2
					02 = Vorlauf		Primär		TVP				2
					03 = Rücklauf		Primär		TRP				2
					04 = Vorlauf		Sekundär	TVS				4
					05 = Rücklauf		Sekundär	TRS				4
					06 = Raumtemperatur	 			TI				4
					07 = Temp.am PWÜ					TWWC			2
					08 = Speicher 1 Temp			TWW1L			2	oberer Fühler
					09 = Speicher 2 Temp			TWW2L			2 unterer Fühler
					0A = Temp.								TWWV			2
					0B = Kesselvorlauf				TVKE			8
					0C = Kesselrücklauf				TRKE			8
					0D = Vorlauf Sek.(Netztr.)TVSEK			2
##### ulsch
					0E = univ.Temp.fuehler		TP_UNI		8	nur zur Anzeige, keine weiteren Funktionen !	
					0F = Temp.am Solar-Koll.	TKOLL			2	
					10 = Solar-Speich.1 Temp	TPUF1L		2	oberer Fühler
					11 = Solar-Speich.2 Temp	TPUF2L		2	unterer Fühler	
					12 = WW Solar							TWWCSO		2 Regeltemp.am Solar-Übertrager	
					13 = Temp. vor PWÜ				TWWP			2 									
##### josch
					14 = Gemeins. Kesselvorl.	TVGKE			2
					15 = Zirkulationstemp.		TWWZ			2
##### ulsch
					16 = Gemeins.Kesselrückl.	TRGKE			2
					17 = Vorlauf sek. WT ges.	TVSGNT		4
					18 = Rücklauf Sek.Netztr. TRSEK			4
					19 = Rücklauf prim.Netztr.TRPNT			4
					1A = Rückauf sek. WT ges.	TRSGNT		4
#####	SiWa				
					1B = 										  TV_WP 		1	
					1C = 										  TR_WP 		1
					1D = 										TSPo_WP 		1	
					1E = 										TSPu_WP 		1
					1F = 										TVST_WP 		1	
					20 = 										TRST_WP 		1
					21 = 										 TVQ_WP 		1	
					22 = 										 TRQ_WP 		1
					23 = 										 TVH_WP 		1	
					24 = 										 TRH_WP 		1
					...
					
	Klasse: 0-10V-Eingänge											reservierte Speicherplätze
					00 = Dummy								AE_FREI		1
					01 = Luftqualitätsfühler	LQF				4
					02 = univ.Analogeingang		AE_UNI		8	nur zur Anzeige oder mit projektspez. Funktion, ohne Funktion im Pool !					
					03 = Adaptions-Kennwert		ADAPT			4
					04 = Außentemperatur			TAE				4
					05 = Leistungs-Analogeingang RM_POW	8	insbesondere für Kessel					
					06 = Druck-Analogeingang 	AE_DRU		8	insbesondere für Kessel					
					...
					
	Klasse:	Digitaleingänge											reservierte Speicherplätze
					00 = Dummy								DE_FREI		1
					01 = Digitaleingang				STBHK			4		Sicherh.temp.begrenzer Heizung
					02 = Digitaleingang				STBWW			2		Sicherh.temp.begrenzer Warmwasser
					03 = Digitaleingang				STWHK			4		Sicherh.temp.wächter   Heizung
					04 = Digitaleingang				STWWW			2		Sicherh.temp.wächter   Warmwasser
					05 = Digitaleingang				WWUW			2		Warmwasseroberwert
					06 = Digitaleingang				WWOW			2		Warmwasserunterwert
					07 = Digitaleingang				KEAL			2		Störmeldung Kessel (Si-Kette)
					08 = Digitaleingang				BRAL			2		Störmeldung Brenner
					09 = Digitaleingang				NACHSP		2		Störmeldung Nachspeisung
					0A = Digitaleingang				PUAL			4		Störmeldung Heizungspumpe
					0B = Digitaleingang				PUWZAL		2		Störmeldung Zirkulationspumpe
					0C = Digitaleingang				PUWLAL		2		Störmeldung (Ent-)Ladepumpe
					0D = Digitaleingang				STBWWSP		2		Stoermeldung STB Speicher
					0E = Digitaleingang				BMKES			2		Betriebsmeldung Kessel
					0F = Digitaleingang				PUSAMAL		2		Sammelstörmeldung Pumpen
					10 = Digitaleingang				STBNT			2		Sicherh.temp.begrenzer Netztrennung
					11 = Digitaleingang				SAMAL			2		Sammelstörmeldung allgemein
					12 = Digitaleingang				PUWTAL		2		Störmeldung Tauscherpumpe
					13 = Digitaleingang				FRGHK			4		Freigabe Regelung Heizkreis
##### ulsch
					14 = Digitaleingang				DE_UNI		8		universeller Digitaleingang, 	nur zur Anzeige, keine weiteren Funktionen !
					15 = Digitaleingang				FERINP		1		Ferienschalter
					16 = Digitaleingang				PUKEAL		2		Störmeldung Kesselpumpe
					17 = Digitaleingang				BMPU			2		Betriebsmeldung Heizungspumpe
					18 = Digitaleingang				BMPUWL		2		Betriebsmeldung Ladepumpe
					19 = Digitaleingang				BMPUWZ		2		Betriebsmeldung Zirkulationspumpe
					1A = Digitaleingang				BMPUWT		2		Betriebsmeldung Tauscherpumpe
					1B = Digitaleingang				HKAUS			4		Anlagenschalter Nichtnutzung HK
					1C = Digitaleingang				WWAUS			2		Anlagenschalter Nichtnutzung WWB
					1D = Digitaleingang				QUITTS		1		Quittierungstaste
#####ulsch
					1E = Digitaleingang				SSFKE			2		Schornsteinfegertaste					
					1F = Digitaleingang				STWNT			2		Sicherh.temp.wächter Netztrennung
					20 = Digitaleingang				STBKE			2		Sicherh.temp.begrenzer Kessel
					21 = Digitaleingang				DRKE			2		Max-Druck Kessel
					22 = Digitaleingang				PUDAL			4		Störmeldung Heizungspumpe, 2.Pumpe bei Doppelpumpe
					23 = Digitaleingang				BM_UNI		8		universeller Digitaleingang für Betriebsmeldungen
					24 = Digitaleingang				SM_UNI		8		universeller Digitaleingang für Störmeldungen
					25 = Digitaleingang				ADAPT_T		4		Adaptionsmeldung "zu tief"
					26 = Digitaleingang				ADAPT_H		4		Adaptionsmeldung "zu hoch"
					27 = Digitaleingang				RKZUNT		4		RM Klappe ZU NT
					28 = Digitaleingang				RKAUFNT		4		RM Klappe AUF NT
					29 = Digitaleingang				DRMAXNT		4		Max-Druck NT
					2A = Digitaleingang				DRMASNT		4		Max-Druck NT sek.
					2B = Digitaleingang				DRMISNT		4		Min-Druck NT sek.
					2C = Digitaleingang				RVZUNT		4		RM Ventil ZU NT
					2D = Digitaleingang				PUALNT		4		Störmeldung Hauptpumpe
					2E = Digitaleingang				BMPUNT		4		Betriebsmeldung Hauptpumpe
					2F = Digitaleingang				NTAUSKE		4		Not-Aus Kessel
					...	
					
	Klasse:	Zähleingänge												reservierte Speicherplätze
					00 = Dummy								ZE_FREI			1
					01 = Zähleingang R36			ZIN3 -ZIN10	8
					02 = Zähleingang R37_1		ZIN11-ZIN16	8
					03 = Zähleingang R37_2		ZIN21-ZIN26	8
					...
					
	Klasse:	Analogausgänge											reservierte Speicherplätze
					00 = Dummy								AA_FREI		1
					01 = Heizungsregelung			RVENT			4
					02 = Warmwasserregelung		RVENTWW		2
					03 = Kesselregelung				RVENTKE		2
					04 = Sammelstörmeldung		AASSTM		2
					05 = Drehz. WW-Zirkpumpe	PUWZ			2
					06 = Strahlpumpe					RVENTSP		4
					07 = Mischer							RVENTMI		4
					08 = Drehzahl Ladepumpe		PUWL			2		( HAST-AKKU : Entladepumpe )
					09 = Regelventil Netztr.	RVENTNT		2		
					0A = Analogausgang				AA_UNI		8	universeller Analogausgang, ohne Funktion im Pool	
					0B = Regelventil WW-Solar	RVENTSO		2				
					0C = Regelventil KE-Rückl	RVENTKR		2 Kesselrücklauf-Anhebung				
					0D = Außentemperatur			TAA				4
					0E = Wärme-Anforderung		TMANF			2
					0F = Drehzahl Kesselpumpe	DZPUKE		2
					...

	Klasse:	Relais-Ausgänge											reservierte Speicherplätze
					00 = Dummy								DA_FREI		1
					01 = Regelventil Heiz.zu	RxZU			4
					02 = Regelventil Heiz.auf	RxAUF			4
					03 = Regelventil WW.zu		RWWxZU		2
					04 = Regelventil WW.auf		RWWxAUF		2
					05 = Heizungs-Pumpe				PUxEA			4
					06 = Heizungs-Pumpe min		PUxMIN		4
					07 = WWLade-Pumpe					PUWLxEA		2
					08 = WWZirk-Pumpe					PUWZxEA		2
					09 = Relais Elvent				ELVENT		2
					0A = Relais 							PUGWW			2
					0B = Relais	Sammelstörg.	SSTM			2		Sammelstörmeldung
					0C = Kessel Stufe 1				KExS1EA		2	
					0D = Kessel Stufe 2				KExS2EA		2
					0E = Kessel Freigabe			KExEA			2
					0F = WWTauscher-Pumpe			PUWTxEA		2
					10 = Regelventil NT zu		RNTxZU		2
					11 = Regelventil NT auf		RNTxAUF		2
					12 = Ferienautomatik			FEROUT		2	
					13 = Digitalausgang				DA_UNI		8		universeller Digitalausgang, ohne Funktion im Pool	
					14 = Modulation Kessel ZU	MKExZU		2	
					15 = Modulat. Kessel AUF	MKExAUF		2	
					16 = Pumpe Kessel					PUKExEA		2	
					17 = Solar-Kollektor-Pu.	PUKOLxEA	2	
					18 = Solar-Pumpe					PUSOxEA		2	
					19 = Absperrklappe Kessel	KLKExAZ		2	Absperrklappe AUF/ZU																	
					1A = Absperrklappe Kessel	KLKExZU		2	Absperrklappe ZU
					1B = Absperrklappe Kessel	KLKExAUF	2	Absperrklappe AUF											
					1C = Pumpe Netztrennung		PUNTxEA		2 Hauptpumpe
					1D = Heizungs-Pumpe				PUDxEA		4	2.Pumpe bei Doppelpumpe
					1E = Ventil Kessel ZU			RKExZU		4	
					1F = Ventil Kessel AUF		RKExAUF		4	
					20 = Absperrklappe WT			KLNTxAZ		2	Absperrklappe AUF/ZU																	
					...
					
	Num:		1	 = Heizkreis 1
					2  = Heizkreis 2
					...
*/	

//	Name					Typ	Klasse Num
//-------------------------------------------
// Analogeingänge Pt1000
//-------------------------------------------
#define TP_FREI		0x1000
#define TA1				0x1011
#define TA2				0x1012

#define TVP1			0x1021
#define TVP2			0x1022

#define TRP1			0x1031
#define TRP2			0x1032

#define TVS1			0x1041
#define TVS2			0x1042
#define TVS3			0x1043
#define TVS4			0x1044

#define TRS1			0x1051
#define TRS2			0x1052
#define TRS3			0x1053
#define TRS4			0x1054

#define TI1				0x1061
#define TI2				0x1062
#define TI3				0x1063
#define TI4				0x1064

#define TWWC1			0x1071
#define TWWC2			0x1072

#define TWW1L1		0x1081
#define TWW1L2		0x1082

#define TWW2L1		0x1091
#define TWW2L2		0x1092

#define TWWV1			0x10A1
#define TWWV2			0x10A2

#define TVKE1			0x10B1
#define TVKE2			0x10B2
#define TVKE3			0x10B3
#define TVKE4			0x10B4
#define TVKE5			0x10B5
#define TVKE6			0x10B6
#define TVKE7			0x10B7
#define TVKE8			0x10B8
#define TVKE3			0x10B3
#define TVKE4			0x10B4
#define TVKE5			0x10B5
#define TVKE6			0x10B6
#define TVKE7			0x10B7
#define TVKE8			0x10B8

#define TRKE1			0x10C1
#define TRKE2			0x10C2
#define TRKE3			0x10C3
#define TRKE4			0x10C4
#define TRKE5			0x10C5
#define TRKE6			0x10C6
#define TRKE7			0x10C7
#define TRKE8			0x10C8

#define TVSEK1		0x10D1
#define TVSEK2		0x10D2

// ##### ulsch
#define TP_UNI1		0x10E1
#define TP_UNI2		0x10E2
#define TP_UNI3		0x10E3
#define TP_UNI4		0x10E4
#define TP_UNI5		0x10E5
#define TP_UNI6		0x10E6
#define TP_UNI7		0x10E7
#define TP_UNI8		0x10E8

#define TKOLL1		0x10F1
#define TKOLL2		0x10F2

#define TPUF1L1		0x1101
#define TPUF1L2		0x1102

#define TPUF2L1		0x1111
#define TPUF2L2		0x1112

#define TWWCSO1		0x1121
#define TWWCSO2		0x1122

#define TWWP1			0x1131
#define TWWP2			0x1132

// ##### josch
#define TVGKE1		0x1141
#define TVGKE2		0x1142

#define TWWZ1			0x1151
#define TWWZ2			0x1152

// ##### ulsch
#define TRGKE1		0x1161
#define TRGKE2		0x1162

#define TVSGNT1		0x1171
#define TVSGNT2		0x1172
#define TVSGNT3		0x1173
#define TVSGNT4		0x1174

#define TRSEK1		0x1181
#define TRSEK2		0x1182
#define TRSEK3		0x1183
#define TRSEK4		0x1184

#define TRPNT1		0x1191
#define TRPNT2		0x1192
#define TRPNT3		0x1193
#define TRPNT4		0x1194

#define TRSGNT1		0x11A1
#define TRSGNT2		0x11A2
#define TRSGNT3		0x11A3
#define TRSGNT4		0x11A4

// SiWa 27.02.18 - Vordefinierte Fühler für WPU
#define TV_WP1		0x11B1
#define TR_WP1		0x11B2
#define TSPo_WP1	0x11B3
#define TSPu_WP1	0x11B4
#define TVST_WP1	0x11B5
#define TRST_WP1	0x11B6
#define TVQ_WP1	0x11B7
#define TRQ_WP1	0x11B8
#define TVH_WP1	0x11B9
#define TRH_WP1	0x11BA	

//-------------------------------------------
// Analogeingänge 0-10V
//-------------------------------------------
#define AE_FREI		0x2000
#define LQF1			0x2011
#define LQF2			0x2012
#define LQF3			0x2013
#define LQF4			0x2014

#define AE_UNI1		0x2021
#define AE_UNI2		0x2022
#define AE_UNI3		0x2023
#define AE_UNI4		0x2024
#define AE_UNI5		0x2025
#define AE_UNI6		0x2026
#define AE_UNI7		0x2027
#define AE_UNI8		0x2028

#define ADAPT1		0x2031
#define ADAPT2		0x2032
#define ADAPT3		0x2033
#define ADAPT4		0x2034

#define TAE1			0x2041

#define RM_POW1		0x2051
#define RM_POW2		0x2052
#define RM_POW3		0x2053
#define RM_POW4		0x2054
#define RM_POW5		0x2055
#define RM_POW6		0x2056
#define RM_POW7		0x2057
#define RM_POW8		0x2058

#define AE_DRU1		0x2061
#define AE_DRU2		0x2062
#define AE_DRU3		0x2063
#define AE_DRU4		0x2064
#define AE_DRU5		0x2065
#define AE_DRU6		0x2066
#define AE_DRU7		0x2067
#define AE_DRU8		0x2068

//-------------------------------------------
// Digitaleingänge
//-------------------------------------------
#define DE_FREI		0x3000
#define STBHK1		0x3011					// Sicherh.temp.begrenzer Heizung
#define STBHK2		0x3012
#define STBHK3		0x3013
#define STBHK4		0x3014

#define STBWW1		0x3021					// Sicherh.temp.begrenzer Warmwasser
#define STBWW2		0x3022

#define STWHK1		0x3031					// Sicherh.temp.wächter   Heizung
#define STWHK2		0x3032
#define STWHK3		0x3033
#define STWHK4		0x3034

#define STWWW1		0x3041					// Sicherh.temp.wächter   Warmwasser
#define STWWW2		0x3042

#define WWUW1			0x3051					// Warmwasseroberwert
#define WWUW2			0x3052

#define WWOW1			0x3061					// Warmwasserunterwert
#define WWOW2			0x3062

#define KEAL1			0x3071					// Störmeldung Kessel (Si-Kette)
#define KEAL2			0x3072
#define KEAL3			0x3073
#define KEAL4			0x3074
#define KEAL5			0x3075
#define KEAL6			0x3076
#define KEAL7			0x3077
#define KEAL8			0x3078

#define BRAL1			0x3081					// Störmeldung Brenner
#define BRAL2			0x3082
#define BRAL3			0x3083
#define BRAL4			0x3084
#define BRAL5			0x3085
#define BRAL6			0x3086
#define BRAL7			0x3087
#define BRAL8			0x3088

#define NACHSP1		0x3091					// Störmeldung Nachspeisung
#define NACHSP2		0x3092
#define NACHSP3		0x3093
#define NACHSP4		0x3094
#define NACHSP5		0x3095
#define NACHSP6		0x3096
#define NACHSP7		0x3097
#define NACHSP8		0x3098

#define PUAL1			0x30A1					// Störmeldung Heizungspumpe
#define PUAL2			0x30A2
#define PUAL3			0x30A3
#define PUAL4			0x30A4

#define PUWZAL1		0x30B1					// Stoermeldung Zirkulationspumpe
#define PUWZAL2		0x30B2

#define PUWLAL1		0x30C1					// Stoermeldung (Ent-)Ladepumpe					
#define PUWLAL2		0x30C2

#define STBWWSP1	0x30D1					// Stoermeldung STB Speicher
#define STBWW2P2	0x30D2

#define BMKES1		0x30E1					// Betriebsmeldung Kessel
#define BMKES2		0x30E2
#define BMKES3		0x30E3
#define BMKES4		0x30E4
#define BMKES5		0x30E5
#define BMKES6		0x30E6
#define BMKES7		0x30E7
#define BMKES8		0x30E8

#define PUSAMAL1	0x30F1					// Sammelstörmeldung Pumpen
#define PUSAMAL2	0x30F2

#define STBNT1		0x3101					// Sicherh.temp.begrenzer Netztrennung
#define STBNT2		0x3102

#define SAMAL1		0x3111					// Sammelstörmeldung allgemein
#define SAMAL2		0x3112

#define PUWTAL1		0x3121					// Stoermeldung Tauscherpumpe					
#define PUWTAL2		0x3122

#define FRGHK1		0x3131					// Freigabe Regelung Heizkreis				
#define FRGHK2		0x3132
#define FRGHK3		0x3133
#define FRGHK4		0x3134

// ulsch
#define DE_UNI1		0x3141					// universeller Eingang
#define DE_UNI2		0x3142
#define DE_UNI3		0x3143
#define DE_UNI4		0x3144
#define DE_UNI5		0x3145
#define DE_UNI6		0x3146
#define DE_UNI7		0x3147
#define DE_UNI8		0x3148

#define FERINP1		0x3151					// Ferienschalter

#define PUKEAL1		0x3161					// Störmeldung Kesselpumpe
#define PUKEAL2		0x3162
#define PUKEAL3		0x3163
#define PUKEAL4		0x3164
#define PUKEAL5		0x3165
#define PUKEAL6		0x3166
#define PUKEAL7		0x3167
#define PUKEAL8		0x3168

#define BMPU1			0x3171					// Betriebsmeldung Heizungspumpe				
#define BMPU2			0x3172
#define BMPU3			0x3173
#define BMPU4			0x3174

#define	BMPUWL1		0x3181					// Betriebsmeldung Ladepumpe				
#define	BMPUWL2		0x3182		

#define	BMPUWZ1		0x3191					// Betriebsmeldung Zirkulationspumpe		
#define	BMPUWZ2		0x3192

#define	BMPUWT1		0x31A1					// Betriebsmeldung Tauscherpumpe
#define	BMPUWT2		0x31A2

#define	HKAUS1		0x31B1					// Anlagenschalter Nichtnutzung HK
#define	HKAUS2		0x31B2
#define	HKAUS3		0x31B3
#define	HKAUS4		0x31B4

#define	WWAUS1		0x31C1					// Anlagenschalter Nichtnutzung WWB
#define	WWAUS2		0x31C2

#define	QUITTS1		0x31D1					// Quittierungstaste

#define	SSFKE1		0x31E1					// Schornsteinfeger-Taste
#define	SSFKE2		0x31E2
#define	SSFKE3		0x31E3
#define	SSFKE4		0x31E4
#define	SSFKE5		0x31E5
#define	SSFKE6		0x31E6
#define	SSFKE7		0x31E7
#define	SSFKE8		0x31E8

#define STWNT1		0x31F1					// Sicherh.temp.wächter Netztrennung
#define STWNT2		0x31F2

#define STBKE1		0x3201					// Sicherh.temp.begrenzer Kessel
#define STBKE2		0x3202
#define STBKE3		0x3203
#define STBKE4		0x3204
#define STBKE5		0x3205
#define STBKE6		0x3206
#define STBKE7		0x3207
#define STBKE8		0x3208

#define DRKE1			0x3211					// Max-Druck Kessel
#define DRKE2			0x3212
#define DRKE3			0x3213
#define DRKE4			0x3214
#define DRKE5			0x3215
#define DRKE6			0x3216
#define DRKE7			0x3217
#define DRKE8			0x3218

#define PUDAL1		0x3221					// Störmeldung Heizungspumpe, 2.Pumpe bei Doppelpumpe
#define PUDAL2		0x3222
#define PUDAL3		0x3223
#define PUDAL4		0x3224
					
#define BM_UNI1		0x3231					// universeller Digitaleingang für Betriebsmeldungen
#define BM_UNI2		0x3232
#define BM_UNI3		0x3233
#define BM_UNI4		0x3234
#define BM_UNI5		0x3235
#define BM_UNI6		0x3236
#define BM_UNI7		0x3237
#define BM_UNI8		0x3238

#define SM_UNI1		0x3241					// universeller Digitaleingang für Störmeldungen
#define SM_UNI2		0x3242
#define SM_UNI3		0x3243
#define SM_UNI4		0x3244
#define SM_UNI5		0x3245
#define SM_UNI6		0x3246
#define SM_UNI7		0x3247
#define SM_UNI8		0x3248
					
#define ADAPT_T1	0x3251					// Adaptionsmeldung "zu tief"
#define ADAPT_T2	0x3252
#define ADAPT_T3	0x3253
#define ADAPT_T4	0x3254
                                                                                  
#define ADAPT_H1	0x3261					// Adaptionsmeldung "zu hoch"
#define ADAPT_H2	0x3262                                                          
#define ADAPT_H3	0x3263                                                          
#define ADAPT_H4	0x3264                                                          
                                                                                  
#define RKZUNT1		0x3271					// RM Klappe ZU NT
#define RKZUNT2		0x3272                                                         
#define RKZUNT3		0x3273                                                          
#define RKZUNT4		0x3274                                                          
                                                                                  
#define RKAUFNT1	0x3281					// RM Klappe AUF NT
#define RKAUFNT2	0x3282                                                         
#define RKAUFNT3	0x3283                                                          
#define RKAUFNT4	0x3284

#define DRMAXNT1	0x3291					// Max-Druck NT 
#define DRMAXNT2	0x3292
#define DRMAXNT3	0x3293
#define DRMAXNT4	0x3294

#define DRMASNT1	0x32A1					// Max-Druck NT sek.
#define DRMASNT2	0x32A2
#define DRMASNT3	0x32A3
#define DRMASNT4	0x32A4

#define DRMISNT1	0x32B1					// Min-Druck NT sek.
#define DRMISNT2	0x32B2
#define DRMISNT3	0x32B3
#define DRMISNT4	0x32B4
                                                                                  
#define RVZUNT1		0x32C1					// RM Ventil ZU NT
#define RVZUNT2		0x32C2                                                         
#define RVZUNT3		0x32C3                                                          
#define RVZUNT4		0x32C4                                                          

#define PUALNT1		0x32D1					// SM Hauptpumpe
#define PUALNT2		0x32D2
#define PUALNT3		0x32D3
#define PUALNT4		0x32D4

#define BMPUNT1		0x32E1					// BM Hauptpumpe
#define BMPUNT2		0x32E2
#define BMPUNT3		0x32E3
#define BMPUNT4		0x32E4

#define NOTAUS1		0x32F1					// SM Not-Aus
#define NOTAUS2		0x32F2
#define NOTAUS3		0x32F3
#define NOTAUS4		0x32F4
#define NOTAUS5		0x32F5
#define NOTAUS6		0x32F6
#define NOTAUS7		0x32F7
#define NOTAUS8		0x32F8

#define BMPUKE1		0x3301					// SM Kesselpumpe
#define BMPUKE2		0x3302
#define BMPUKE3		0x3303
#define BMPUKE4		0x3304
#define BMPUKE5		0x3305
#define BMPUKE6		0x3306
#define BMPUKE7		0x3307
#define BMPUKE8		0x3308
//-------------------------------------------
// Zähleingänge R36
//-------------------------------------------
#define ZE_FREI		0x4000
#define ZIN3			0x4011
#define ZIN4			0x4012
#define ZIN5			0x4013
#define ZIN6			0x4014
#define ZIN7			0x4015
#define ZIN8			0x4016
#define ZIN9			0x4017
#define ZIN10			0x4018

// Zähleingänge R37_1
#define ZIN11			0x4021
#define ZIN12			0x4022
#define ZIN13			0x4023
#define ZIN14			0x4024
#define ZIN15			0x4025
#define ZIN16			0x4026

// Zähleingänge R37_2
#define ZIN21			0x4031
#define ZIN22			0x4032
#define ZIN23			0x4033
#define ZIN24			0x4034
#define ZIN25			0x4035
#define ZIN26			0x4036


//-------------------------------------------
// Analogausgänge
//-------------------------------------------
#define AA_FREI		0x5000
#define RVENT1		0x5011
#define RVENT2		0x5012
#define RVENT3		0x5013
#define RVENT4		0x5014

#define RVENTWW1	0x5021
#define RVENTWW2	0x5022

#define RVENTKE1	0x5031
#define RVENTKE2	0x5032
#define RVENTKE3	0x5033
#define RVENTKE4	0x5034
#define RVENTKE5	0x5035
#define RVENTKE6	0x5036
#define RVENTKE7	0x5037
#define RVENTKE8	0x5038

#define AASSTM1		0x5041
#define AASSTM2		0x5042

#define PUWZ1			0x5051					// Drehzahl Warmwasser-Zirkulationspumpe	
#define PUWZ2			0x5052

#define RVENTSP1	0x5061					// Strahlpumpe
#define RVENTSP2	0x5062
#define RVENTSP3	0x5063
#define RVENTSP4	0x5064

#define RVENTMI1	0x5071					// Mischer
#define RVENTMI2	0x5072
#define RVENTMI3	0x5073
#define RVENTMI4	0x5074

#define PUWL1			0x5081					// Drehzahl Ladepumpe	( HAST-AKKU : Entladepumpe )
#define PUWL2			0x5082

#define RVENTNT1	0x5091
#define RVENTNT2	0x5092

// ##### ulsch
#define AA_UNI1		0x50A1
#define AA_UNI2		0x50A2
#define AA_UNI3		0x50A3
#define AA_UNI4		0x50A4
#define AA_UNI5		0x50A5
#define AA_UNI6		0x50A6
#define AA_UNI7		0x50A7
#define AA_UNI8		0x50A8

#define RVENTSO1	0x50B1
#define RVENTSO2	0x50B2

// ##### josch
#define	RVENTKR1	0x50C1					// Kesselrücklauf-Anhebung				
#define	RVENTKR2	0x50C2
#define	RVENTKR3	0x50C3
#define	RVENTKR4	0x50C4
#define	RVENTKR5	0x50C5
#define	RVENTKR6	0x50C6
#define	RVENTKR7	0x50C7
#define	RVENTKR8	0x50C7

#define	TAA1			0x50D1					// Außentemperatur				
#define	TAA2			0x50D2
#define	TAA3			0x50D3
#define	TAA4			0x50D4

#define	TMANF1		0x50E1					// Wärme-Anforderung				
#define	TMANF2		0x50E2		

#define	DZPUKE1		0x50F1					// Drehzahl Kesselpumpe				
#define	DZPUKE2		0x50F2
#define	DZPUKE3		0x50F3
#define	DZPUKE4		0x50F4
#define	DZPUKE5		0x50F5
#define	DZPUKE6		0x50F6
#define	DZPUKE7		0x50F7
#define	DZPUKE8		0x50F8

//-------------------------------------------
// Relaisausgänge
//-------------------------------------------
#define DA_FREI		0x6000
#define R1ZU			0x6011
#define R2ZU			0x6012
#define R3ZU			0x6013
#define R4ZU			0x6014

#define R1AUF			0x6021
#define R2AUF			0x6022
#define R3AUF			0x6023
#define R4AUF			0x6024

#define RWW1ZU		0x6031
#define RWW2ZU		0x6032

#define RWW1AUF		0x6041
#define RWW2AUF		0x6042

#define PU1EA			0x6051
#define PU2EA			0x6052
#define PU3EA			0x6053
#define PU4EA			0x6054

#define PU1MIN		0x6061
#define PU2MIN		0x6062
#define PU3MIN		0x6063
#define PU4MIN		0x6064

#define PUWL1EA		0x6071
#define PUWL2EA		0x6072

#define PUWZ1EA		0x6081
#define PUWZ2EA		0x6082

#define ELVENT1		0x6091
#define ELVENT2		0x6092

#define PUGWW1EA	0x60A1
#define PUGWW2EA	0x60A2

#define SSTM1			0x60B1
#define SSTM2			0x60B2

#define KE1S1EA		0x60C1
#define KE2S1EA		0x60C2
#define KE3S1EA		0x60C3
#define KE4S1EA		0x60C4
#define KE5S1EA		0x60C5
#define KE6S1EA		0x60C6
#define KE7S1EA		0x60C7
#define KE8S1EA		0x60C8

#define KE1S2EA		0x60D1
#define KE2S2EA		0x60D2
#define KE3S2EA		0x60D3
#define KE4S2EA		0x60D4
#define KE5S2EA		0x60D5
#define KE6S2EA		0x60D6
#define KE7S2EA		0x60D7
#define KE8S2EA		0x60D8

#define KE1EA			0x60E1
#define KE2EA			0x60E2
#define KE3EA			0x60E3
#define KE4EA			0x60E4
#define KE5EA			0x60E5
#define KE6EA			0x60E6
#define KE7EA			0x60E7
#define KE8EA			0x60E8

#define PUWT1EA		0x60F1
#define PUWT2EA		0x60F2

#define RNT1ZU		0x6101
#define RNT2ZU		0x6102

#define RNT1AUF		0x6111
#define RNT2AUF		0x6112

//#####ulsch
#define FEROUT1		0x6121				// Ferienstatus

#define DA_UNI1		0x6131				// universeller Ausgang
#define DA_UNI2		0x6132
#define DA_UNI3		0x6133
#define DA_UNI4		0x6134
#define DA_UNI5		0x6135
#define DA_UNI6		0x6136
#define DA_UNI7		0x6137
#define DA_UNI8		0x6138

#define MKE1ZU		0x6141
#define MKE2ZU		0x6142
#define MKE3ZU		0x6143
#define MKE4ZU		0x6144
#define MKE5ZU		0x6145
#define MKE6ZU		0x6146
#define MKE7ZU		0x6147
#define MKE8ZU		0x6148

#define MKE1AUF		0x6151
#define MKE2AUF		0x6152
#define MKE3AUF		0x6153
#define MKE4AUF		0x6154
#define MKE5AUF		0x6155
#define MKE6AUF		0x6156
#define MKE7AUF		0x6157
#define MKE8AUF		0x6158

#define PUKE1EA		0x6161
#define PUKE2EA		0x6162
#define PUKE3EA		0x6163
#define PUKE4EA		0x6164
#define PUKE5EA		0x6165
#define PUKE6EA		0x6166
#define PUKE7EA		0x6167
#define PUKE8EA		0x6168

#define PUKOL1EA	0x6171
#define PUKOL2EA	0x6172

#define PUSO1EA		0x6181
#define PUSO2EA		0x6182

#define KLKE1AZ		0x6191
#define KLKE2AZ		0x6192
#define KLKE3AZ		0x6193
#define KLKE4AZ		0x6194
#define KLKE5AZ		0x6195
#define KLKE6AZ		0x6196
#define KLKE7AZ		0x6197
#define KLKE8AZ		0x6198

#define KLKE1ZU		0x61A1
#define KLKE2ZU		0x61A2
#define KLKE3ZU		0x61A3
#define KLKE4ZU		0x61A4
#define KLKE5ZU		0x61A5
#define KLKE6ZU		0x61A6
#define KLKE7ZU		0x61A7
#define KLKE8ZU		0x61A8

#define KLKE1AUF	0x61B1
#define KLKE2AUF	0x61B2
#define KLKE3AUF	0x61B3
#define KLKE4AUF	0x61B4
#define KLKE5AUF	0x61B5
#define KLKE6AUF	0x61B6
#define KLKE7AUF	0x61B7
#define KLKE8AUF	0x61B8

#define PUNT1EA		0x61C1
#define PUNT2EA		0x61C2

#define PUD1EA		0x61D1
#define PUD2EA		0x61D2
#define PUD3EA		0x61D3
#define PUD4EA		0x61D4

#define RKE1ZU		0x61E1
#define RKE2ZU		0x61E2
#define RKE3ZU		0x61E3
#define RKE4ZU		0x61E4
#define RKE5ZU		0x61E5
#define RKE6ZU		0x61E6
#define RKE7ZU		0x61E7
#define RKE8ZU		0x61E8

#define RKE1AUF		0x61F1
#define RKE2AUF		0x61F2
#define RKE3AUF		0x61F3
#define RKE4AUF		0x61F4
#define RKE5AUF		0x61F5
#define RKE6AUF		0x61F6
#define RKE7AUF		0x61F7
#define RKE8AUF		0x61F8

#define KLNT1AZ		0x6201
#define KLNT2AZ		0x6202
#define KLNT3AZ		0x6203
#define KLNT4AZ		0x6204



#endif // USERDEF_H_INCLUDED




