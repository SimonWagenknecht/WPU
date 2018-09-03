/* Liste von Namen für die Ein- und Ausgänge													*/
/* Alle Pointer auf Ein-/Ausgänge mit einem Dummy-Wert belegen				*/
/* Laden der Pointerfelder mit den Adressen der Ein-Ausgangsspeicher	*/


#include "sramext.h"
#include "uramext.h"
#if MODBUS_UNI > 0
	extern void fill_dummy_modbus(void);
#endif	

// Klassennamen
const ea_klasse pt_name[] = {
// "........","........"
	{"FREI    "},								// 0
	{"TA1     ","TA2     ","TA3     ","TA4     "},		// 1
	{"TVP1    ","TVP2    ","TVP3    ","TVP4    "},		// 2
	{"TRP1    ","TRP2    ","TRP3    ","TRP4    "},		// 3
	{"TVS1    ","TVS2    ","TVS3    ","TVS4    "},		// 4
	{"TRS1    ","TRS2    ","TRS3    ","TRS4    "},		// 5
	{"TI1     ","TI2     ","TI3     ","TI4     "},		// 6
	{"TWWC1   ","TWWC2   ","TWWC3   ","TWWC4   "},		// 7
	{"TWW1L1  ","TWW1L2  ","TWW1L3  ","TWW1L4  "},		// 8
	{"TWW2L1  ","TWW2L2  ","TWW2L3  ","TWW2L4  "},		// 9
	{"TWWV1   ","TWWV2   ","TWWV3   ","TWWV4   "},		// A
	{"TVKE1   ","TVKE2   ","TVKE3   ","TVKE4   ","TVKE5   ","TVKE6   ","TVKE7   ","TVKE8   "},		// B
	{"TRKE1   ","TRKE2   ","TRKE3   ","TRKE4   ","TRKE5   ","TRKE6   ","TRKE7   ","TRKE8   "},		// C
	{"TVSEK1  ","TVSEK2  ","TVSEK3  ","TVSEK4  "},		// D
	{"TP_UNI1 ","TP_UNI2 ","TP_UNI3 ","TP_UNI4 ","TP_UNI5 ","TP_UNI6 ","TP_UNI7 ","TP_UNI8 "},		// E	
	{"TKOLL1  ","TKOLL2  ","TKOLL3  ","TKOLL4  "},		// F
	{"TPUF1L1 ","TPUF1L2 ","TPUF1L3 ","TPUF1L4 "},		// 10
	{"TPUF2L1 ","TPUF2L2 ","TPUF2L3 ","TPUF2L4 "},		// 11
	{"TWWCSO1 ","TWWCSO2 ","TWWCSO3 ","TWWCSO4 "},		// 12
	{"TWWP1   ","TWWP2   ","TWWP3   ","TWWP4   "},		// 13	
	{"TVGKE1  ","TVGKE2  ","TVGKE3  ","TVGKE4  "},		// 14	
	{"TWWZ1   ","TWWZ2   ","TWWZ3   ","TWWZ4   "},		// 15	
	{"TRGKE1  ","TRGKE2  ","TRGKE3  ","TRGKE4  "},		// 16	
	{"TVSGNT1 ","TVSGNT2 ","TVSGNT3 ","TVSGNT4 "},		// 17	
	{"TRSEK1  ","TRSEK2  ","TRSEK3  ","TRSEK4  "},		// 18
	{"TRPNT1  ","TRPNT2  ","TRPNT3  ","TRPNT4  "},		// 19
	{"TRSGNT1 ","TRSGNT2 ","TRSGNT3 ","TRSGNT4 "},		// 1A
	{"TV_WP1  "},																			// 1B	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TR_WP1  "},																			// 1C	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TSPo_WP1"},																		// 1D	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TSPu_WP1"},																		// 1E	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TVST_WP1"},																		// 1F	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TRST_WP1"},																		// 20	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TVQ_WP1 "},																		// 21	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TRQ_WP1 "},																		// 22	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TVH_WP1 "},																		// 23	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TRH_WP1 "},																		// 24	// SiWa 27.02.18 - Vordefinierte Fühler für WPU
	{"TSPm_WP1"},																		// 25	// SiWa 03.09.18 - Vordefinierte Fühler für WPU
};

const ea_klasse ae_name[] = {
// "........","........"
	{"FREI    "},
	{"LQF1    ","LQF2    ","LQF3    ","LQF4    "},		// 1
	{"AE_UNI1 ","AE_UNI2 ","AE_UNI3 ","AE_UNI4 ","AE_UNI5 ","AE_UNI6 ","AE_UNI7 ","AE_UNI8 "},	
	{"ADAPT1  ","ADAPT2  ","ADAPT3  ","ADAPT4  "},		// 3
	{"TAE1    "},																			// 4
	{"RM_POW1 ","RM_POW2 ","RM_POW3 ","RM_POW4 ","RM_POW5 ","RM_POW6 ","RM_POW7 ","RM_POW8 "},
	{"AE_DRU1 ","AE_DRU2 ","AE_DRU3 ","AE_DRU4 ","AE_DRU5 ","AE_DRU6 ","AE_DRU7 ","AE_DRU8 "},	
};	

const ea_klasse de_name[] = {
// "........","........"
	{"FREI    "},								// 0
	{"STBHK1  ","STBHK2  ","STBHK3  ","STBHK4  "},		// 1
	{"STBWW1  ","STBWW2  ","STBWW3  ","STBWW4  "},		// 2
	{"STWHK1  ","STWHK2  ","STWHK3  ","STWHK4  "},		// 3
	{"STWWW1  ","STWWW2  ","STWWW3  ","STWWW4  "},		// 4
	{"WWUW1   ","WWUW2   ","WWUW3   ","WWUW4   "},		// 5
	{"WWOW1   ","WWOW2   ","WWOW3   ","WWOW4   "},		// 6
	{"KEAL1   ","KEAL2   ","KEAL3   ","KEAL4   ","KEAL5   ","KEAL6   ","KEAL7   ","KEAL8   "},		// 7
	{"BRAL1   ","BRAL2   ","BRAL3   ","BRAL4   ","BRAL5   ","BRAL6   ","BRAL7   ","BRAL7   "},		// 8
	{"NACHSP1 ","NACHSP2 ","NACHSP3 ","NACHSP4 ","NACHSP5 ","NACHSP6 ","NACHSP7 ","NACHSP8 "},		// 9
	{"PUAL1   ","PUAL2   ","PUAL3   ","PUAL4   "},		// A
	{"PUWZAL1 ","PUWZAL2 ","PUWZAL3 ","PUWZAL4 "},		// B
	{"PUWLAL1 ","PUWLAL2 ","PUWLAL3 ","PUWLAL4 "},		// C	
	{"STBWWSP1","STBWWSP2","STBWWSP3","STBWWSP4"},		// D		
	{"BMKES1  ","BMKES2  ","BMKES3  ","BMKES4  ","BMKES5  ","BMKES6  ","BMKES7  ","BMKES8  "},		// E
	{"PUSAMAL1","PUSAMAL2","PUSAMAL3","PUSAMAL4"},		// F
	{"STBNT1  ","STBNT2  ","STBNT3  ","STBNT4  "},		// 10
	{"SAMAL1  ","SAMAL2  ","SAMAL3  ","SAMAL4  "},		// 11
	{"PUWTAL1 ","PUWTAL2 ","PUWTAL3 ","PUWTAL4 "},		// 12	
	{"FRGHK1  ","FRGHK2  ","FRGHK3  ","FRGHK4  "},		// 13		
	{"DE_UNI1 ","DE_UNI2 ","DE_UNI3 ","DE_UNI4 ","DE_UNI5 ","DE_UNI6 ","DE_UNI7 ","DE_UNI8 "},		// 14
	{"FERINP1 ","FERINP2 ","FERINP3 ","FERINP4 "},		// 15	
	{"PUKEAL1 ","PUKEAL2 ","PUKEAL3 ","PUKEAL4 ","PUKEAL5 ","PUKEAL6 ","PUKEAL7 ","PUKEAL8 "},		// 16	
	{"BMPU1   ","BMPU2   ","BMPU3   ","BMPU4   "},		// 17	
	{"BMPUWL1 ","BMPUWL2 ","BMPUWL3 ","BMPUWL4 "},		// 18
	{"BMPUWZ1 ","BMPUWZ2 ","BMPUWZ3 ","BMPUWZ4 "},		// 19
	{"BMPUWT1 ","BMPUWT2 ","BMPUWT3 ","BMPUWT4 "},		// 1A
	{"HKAUS1  ","HKAUS2  ","HKAUS3  ","HKAUS4  "},		// 1B
	{"WWAUS1  ","WWAUS2  ","WWAUS3  ","WWAUS4  "},		// 1C
	{"QUITTS1 ","QUITTS2 ","QUITTS3 ","QUITTS4 "},		// 1D
	{"SSFKE1  ","SSFKE2  ","SSFKE3  ","SSFKE4  ","SSFKE5  ","SSFKE6  ","SSFKE7  ","SSFKE8  "},		// 1E	
	{"STWNT1  ","STWNT2  ","STWNT3  ","STWNT4  "},		// 1F
	{"STBKE1  ","STBKE2  ","STBKE3  ","STBKE4  ","STBKE5  ","STBKE6  ","STBKE7  ","STBKE8  "},		// 20
	{"DRKE1   ","DRKE2   ","DRKE3   ","DRKE4   ","DRKE5   ","DRKE6   ","DRKE7   ","DRKE8   "},		// 21
	{"PUDAL1  ","PUDAL2  ","PUDAL3  ","PUDAL4  "},		// 22
	{"BM_UNI1 ","BM_UNI2 ","BM_UNI3 ","BM_UNI4 ","BM_UNI5 ","BM_UNI6 ","BM_UNI7 ","BM_UNI8 "},		// 23
	{"SM_UNI1 ","SM_UNI2 ","SM_UNI3 ","SM_UNI4 ","SM_UNI5 ","SM_UNI6 ","SM_UNI7 ","SM_UNI8 "},		// 24
	{"ADAPT_T1","ADAPT_T2","ADAPT_T3","ADAPT_T4"},		// 25
	{"ADAPT_H1","ADAPT_H2","ADAPT_H3","ADAPT_H4"},		// 26
	{"RKZUNT1 ","RKZUNT2 ","RKZUNT3 ","RKZUNT4 "},		// 27
	{"RKAUFNT1","RKAUFNT2","RKAUFNT3","RKAUFNT4"},		// 28
	{"DRMAXNT1","DRMAXNT2","DRMAXNT3","DRMAXNT4"},		// 29
	{"DRMASNT1","DRMASNT2","DRMASNT3","DRMASNT4"},		// 2A
	{"DRMISNT1","DRMISNT2","DRMISNT3","DRMISNT4"},		// 2B
	{"RVZUNT1 ","RVZUNT2 ","RVZUNT3 ","RVZUNT4 "},		// 2C
	{"PUALNT1 ","PUALNT2 ","PUALNT3 ","PUALNT4 "},		// 2D
	{"BMPUNT1 ","BMPUNT2 ","BMPUNT3 ","BMPUNT4 "},		// 2E
	{"NOTAUS1 ","NOTAUS2 ","NOTAUS3 ","NOTAUS4 ","NOTAUS5 ","NOTAUS6 ","NOTAUS7 ","NOTAUS8 "},		// 2F
	{"BMPUKE1 ","BMPUKE2 ","BMPUKE3 ","BMPUKE4 ","BMPUKE5 ","BMPUKE6 ","BMPUKE7 ","BMPUKE8 "},		// 30

};

const ea_klasse ze_name[] = {
// "........","........"
	{"FREI    "},
	{"ZIN3    ","ZIN4    ","ZIN5    ","ZIN6    ","ZIN7    ","ZIN8    ","ZIN9    ","ZIN10   "},	// 1 (R36)
	{"ZIN11   ","ZIN12   ","ZIN13   ","ZIN14   ","ZIN15   ","ZIN16   "},												// 2 (R37_1)
	{"ZIN21   ","ZIN22   ","ZIN23   ","ZIN24   ","ZIN25   ","ZIN26   "},												// 3 (R37_2)
};

const ea_klasse aa_name[] = {
// "........","........"
	{"FREI    "},								// 0
	{"RVENT1  ","RVENT2  ","RVENT3  ","RVENT4  "},		// 1
	{"RVENTWW1","RVENTWW2","RVENTWW3","RVENTWW4"},		// 2
	{"RVENTKE1","RVENTKE2","RVENTKE3","RVENTKE4","RVENTKE5","RVENTKE6","RVENTKE7","RVENTKE8"},		// 3
	{"AASSTM1 ","AASSTM2 ","AASSTM3 ","AASSTM4 "},		// 4
	{"PUWZ1   ","PUWZ2   ","PUWZ3   ","PUWZ4   "},		// 5
	{"RVENTSP1","RVENTSP2","RVENTSP3","RVENTSP4"},		// 6
	{"RVENTMI1","RVENTMI2","RVENTMI3","RVENTMI4"},		// 7
	{"PUWL1   ","PUWL2   ","PUWL3   ","PUWL4   "},		// 8
	{"RVENTNT1","RVENTNT2","RVENTNT3","RVENTNT4"},		// 9
	{"AA_UNI1 ","AA_UNI2 ","AA_UNI3 ","AA_UNI4 ","AA_UNI5 ","AA_UNI6 ","AA_UNI7 ","AA_UNI8 "},		// A	
	{"RVENTSO1","RVENTSO2","RVENTSO3","RVENTSO4"},		// B	
	{"RVENTKR1","RVENTKR2","RVENTKR3","RVENTKR4","RVENTKR5","RVENTKR6","RVENTKR7","RVENTKR8"},		// C	
	{"TAA1    ","TAA2    ","TAA3    ","TAA4    "},		// D	
	{"TMANF1  ","TMANF2  "},													// E	
	{"DZPUKE1 ","DZPUKE2 ","DZPUKE3 ","DZPUKE4 ","DZPUKE5 ","DZPUKE6 ","DZPUKE7 ","DZPUKE8 "},													// F	
};

const ea_klasse da_name[] = {
// "........","........"
	{"FREI    "},							// 0
	{"R1ZU    ","R2ZU    ","R3ZU    ","R4ZU    "},	// 1
	{"R1AUF   ","R2AUF   ","R3AUF   ","R4AUF   "},	// 2
	{"RWW1ZU  ","RWW2ZU  ","RWW3ZU  ","RWW4ZU  "},	// 3
	{"RWW1AUF ","RWW2AUF ","RWW3AUF ","RWW4AUF "},	// 4
	{"PU1EA   ","PU2EA   ","PU3EA   ","PU4EA   "},	// 5
	{"PU1MIN  ","PU2MIN  ","PU3MIN  ","PU4MIN  "},	// 6
	{"PUWL1EA ","PUWL2EA ","PUWL3EA ","PUWL4EA "},	// 7
	{"PUWZ1EA ","PUWZ2EA ","PUWZ3EA ","PUWZ4EA "},	// 8
	{"ELVENT1 ","ELVENT2 ","ELVENT3 ","ELVENT4 "},	// 9
	{"PUGWW1  ","PUGWW2  ","PUGWW3  ","PUGWW4  "},	// A
	{"SSTM1   ","SSTM2   ","SSTM3   ","SSTM4   "},	// B
	{"KE1S1EA ","KE2S1EA ","KE3S1EA ","KE4S1EA ","KE5S1EA ","KE6S1EA ","KE7S1EA ","KE8S1EA "},	// C
	{"KE1S2EA ","KE2S2EA ","KE3S2EA ","KE4S2EA ","KE5S2EA ","KE6S2EA ","KE7S2EA ","KE8S2EA "},	// D
	{"KE1EA   ","KE2EA   ","KE3EA   ","KE4EA   ","KE5EA   ","KE6EA   ","KE7EA   ","KE8EA   "},	// E
	{"PUWT1EA ","PUWT2EA ","PUWT3EA ","PUWT4EA "},	// F
	{"RNT1ZU  ","RNT2ZU  ","RNT3ZU  ","RNT4ZU  "},	// 10
	{"RNT1AUF ","RNT2AUF ","RNT3AUF ","RNT4AUF "},	// 11
	{"FEROUT1 ","FEROUT2 ","FEROUT3 ","FEROUT4 "},	// 12		
	{"DA_UNI1 ","DA_UNI2 ","DA_UNI3 ","DA_UNI4 ","DA_UNI5 ","DA_UNI6 ","DA_UNI7 ","DA_UNI8 "},	// 13
	{"MKE1ZU  ","MKE2ZU  ","MKE3ZU  ","MKE4ZU  ","MKE5ZU  ","MKE6ZU  ","MKE7ZU  ","MKE8ZU  "},	// 14
	{"MKE1AUF ","MKE2AUF ","MKE3AUF ","MKE4AUF ","MKE5AUF ","MKE6AUF ","MKE7AUF ","MKE8AUF "},	// 15
	{"PUKE1EA ","PUKE2EA ","PUKE3EA ","PUKE4EA ","PUKE5EA ","PUKE6EA ","PUKE7EA ","PUKE8EA "},	// 16	
	{"PUKOL1EA","PUKOL2EA","PUKOL3EA","PUKOL4EA"},	// 17		
	{"PUSO1EA ","PUSO2EA ","PUSO3EA ","PUSO4EA "},	// 18			
	{"KLKE1AZ ","KLKE2AZ ","KLKE3AZ ","KLKE4AZ ","KLKE5AZ ","KLKE6AZ ","KLKE7AZ ","KLKE8AZ "},	// 19	
	{"KLKE1ZU ","KLKE2ZU ","KLKE3ZU ","KLKE4ZU ","KLKE5ZU ","KLKE6ZU ","KLKE7ZU ","KLKE8ZU "},	// 1A	
	{"KLKE1AUF","KLKE2AUF","KLKE3AUF","KLKE4AUF","KLKE5AUF","KLKE6AUF","KLKE7AUF","KLKE8AUF"},	// 1B				
	{"PUNT1EA ","PUNT2EA ","PUNT3EA ","PUNT4EA "},	// 1C				
	{"PUD1EA  ","PUD2EA  ","PUD3EA  ","PUD4EA  "},	// 1D
	{"RKE1ZU  ","RKE2ZU  ","RKE3ZU  ","RKE4ZU  ","RKE5ZU  ","RKE6ZU  ","RKE7ZU  ","RKE8ZU  "},	// 1E
	{"RKE1AUF ","RKE2AUF ","RKE3AUF ","RKE4AUF ","RKE5AUF ","RKE6AUF ","RKE7AUF ","RKE8AUF "},	// 1F
	{"KLNT1AZ ","KLNT2AZ ","KLNT3AZ ","KLNT4AZ "},	// 20	
	
};

// Typennamen mit Zeiger auf EA-Klassen
const ea_list ea_grup[] = {
// ".......",
	{"NICHT V"},
	{"PT1000:", pt_name},
	{"0-10V :", ae_name},
	{"DIGIT :", de_name},
	{"ZAEHL :", ze_name},
	{"0-10V :", aa_name},
	{"RELAIS:", da_name},
};

//-----------------------------------------------------------------------------------------
	
// Unterprogramm zu Load_Userdef
// Alle Pointer auf Ein-/Ausgänge mit einem Dummy-Wert belegen
void fill_dummy(void)
{
	char i;
	
	bcta.stat = NICHTV;										// Broadcast- Außentemp.
	ta.stat   = NICHTV;
		
	// Modbus
	#if MODBUS_UNI > 0
		fill_dummy_modbus();	// in ModbusTabelle.c
	#endif

	//--------------------------------------------------------------------------
	// Alle Pointer auf Pt1000-Analogeingänge mit einem Dummy-Eingang vorbelegen
	// Status dieser Meßstelle meldet "nicht vorhanden" (Dummy-Fehler)
	//--------------------------------------------------------------------------
	// Anlage
	for(i = 0; i < ANLMAX; i++)
	{	TA[i]					= &mewe_dummy[0];			// Außentemperatur
		TVP[i]				= &mewe_dummy[0];			// Vorlauf  Primär
		TRP[i]				= &mewe_dummy[0];			// Rücklauf Primar
	}	

	// Heizkreis
	for(i = 0; i < HKMAX; i++)
	{	TVS[i]				= &mewe_dummy[0];			// Vorlauf  Sekundär
		TRS[i]				= &mewe_dummy[0];			// Rücklauf Sekundär
		TI[i]					= &mewe_dummy[0];			// Raumtemperatur
	}
	
	// Warmwasser
	for(i = 0; i < WWMAX; i++)
	{	TWWC[i]				= &mewe_dummy[0];			// Warmwassertemp. am Wärmeübertrager
		TWW1L[i]			= &mewe_dummy[0];			// Speichertemperatur oben
		TWW2L[i]			= &mewe_dummy[0];			// Speichertemperatur unten
		TWWV[i]				= &mewe_dummy[0];			// Warmwassertemperatur
		TWWP[i]				= &mewe_dummy[0];			// Warmwassertemperatur	vor PWÜ		
		TWWZ[i]				= &mewe_dummy[0];			// Zirkulationstemperatur		
	}
	
	// Kessel
	for(i = 0; i < KEMAX; i++)
	{	TVKE[i]				= &mewe_dummy[0];			// Vorlauf  Kessel
		TRKE[i]				= &mewe_dummy[0];			// Rücklauf Kessel
		TVGKE[i]			= &mewe_dummy[0];			// Gemeinsamer Vorlauf Kessel
		TRGKE[i]			= &mewe_dummy[0];			// Gemeinsamer Rücklauf Kessel
	}	
	
	// WPU - SiWa 27.02.18
	for(i = 0; i < WPMAX; i++)
	{	
		TV_WP[i]					= &mewe_dummy[0];
		TR_WP[i]					= &mewe_dummy[0];
		TSPo_WP[i]				= &mewe_dummy[0];			
		TSPu_WP[i]				= &mewe_dummy[0];			
		TVST_WP[i]				= &mewe_dummy[0];			
		TRST_WP[i]				= &mewe_dummy[0];			
		TVQ_WP[i]				= &mewe_dummy[0];			
		TRQ_WP[i]				= &mewe_dummy[0];			
		TVH_WP[i]				= &mewe_dummy[0];									
		TRH_WP[i]				= &mewe_dummy[0];
		TSPm_WP[i]				= &mewe_dummy[0];			
	}	
	
	
	
	// Vorregelkreis (Netztrennung)
	for(i = 0; i < NTMAX; i++)
	{	
		TVSEK[i]			= &mewe_dummy[0];			// Vorlauf  Sekundär
		TRSEK[i]			= &mewe_dummy[0];			// Rücklauf  Sekundär
		TVSGNT[i]			= &mewe_dummy[0];			// Vorlauf Sekundär gesamt
		TRSGNT[i]			= &mewe_dummy[0];			// Rücklauf Sekundär gesamt
		TRPNT[i]			= &mewe_dummy[0];			// Rücklauf prim.
	}

	for(i = 0; i < TP_UNI_ANZ; i++)
		TP_UNI[i]			= &mewe_dummy[0];

	// Solarkreis		
	for (i = 0; i < SOLMAX; i++ )
	{
		TKOLL[i]			= &mewe_dummy[0];
		TPUF1L[i]			= &mewe_dummy[0];	
		TPUF2L[i]			= &mewe_dummy[0];	
		TWWCSO[i]			= &mewe_dummy[0];
	}				
			
	//---------------------------------------------------------------------------
	// Alle Pointer auf 0-10V-Analogeingänge mit einem Dummy-Eingang vorbelegen
	// Status dieser Meßstelle meldet "nicht vorhanden" (Dummy-Fehler)
		 LQF[0]				= &mewe_dummy[0];
	for(i = 0; i < AE_UNI_ANZ; i++)
		AE_UNI[i]			= &mewe_dummy[0];
	for ( i = 0; i < (HKANZ ? HKANZ : 1); i++ )
		ADAPT[i] = &mewe_dummy[0];

	#if TAE_ANZ == 1	
	TAE[0] = &mewe_dummy[0];			
	#endif
	
	#if RM_POWER_ANZ
	for ( i = 0; i < RM_POWER_ANZ; i++ )
		RM_POWER[i]	= &mewe_dummy[0];
	#endif
	
	#if AE_DRUCK_ANZ
	for ( i = 0; i < AE_DRUCK_ANZ; i++ )
		AE_DRUCK[i]	= &mewe_dummy[0];
	#endif
					
	//---------------------------------------------------------------------------
	// Alle Pointer auf Digital-Eingänge mit einem Dummy-Eingang vorbelegen
	//---------------------------------------------------------------------------
	// Anlage
	for(i = 0; i < ANLMAX; i++)
	{	PUSAMAL[i]		= &di_mewe_dummy[0];			// Sammelmeldung Pumpenstörung
		SAMAL[i]			= &di_mewe_dummy[0];			// Sammelmeldung Anlage
//#####ulsch
		FERINP[i]			= &di_mewe_dummy[0];			// Ferienschalter		
		QUITTS[i]			= &di_mewe_dummy[0];			// Quittierungstaste
	}
	
	// Heizung
	for(i = 0; i < HKMAX; i++)
	{	STBHK[i]			= &di_mewe_dummy[0];			// Sicherheitstemperaturbegrenzer
		STWHK[i]			= &di_mewe_dummy[0];			// Sicherheitstemperaturwächter
		PUAL[i]				= &di_mewe_dummy[0];			// Pumpenstörmeldung
		FRGHK[i]			= &di_mewe_dummy[0];			// Freigabe Regelung Heizkreis		
		BMPU[i]				= &di_mewe_dummy[0];			// Betriebsmeldung Heizungspumpe
		HKAUS[i]			=	&di_mewe_dummy[0];			// Anlagenschalter Nichtnutzung HK
		#if ( PUDO == 1 )		
			PUDAL[i]		= &di_mewe_dummy[0];			// Pumpenstörmeldung (Doppelpumpe)
		#endif
		ADAPT_T[i]		=	&di_mewe_dummy[0];			// Adaptionsmeldung "zu tief"	
		ADAPT_H[i]		=	&di_mewe_dummy[0];			// Adaptionsmeldung "zu hoch"	
	}
	
	// Warmwasser	
	for(i = 0; i < WWMAX; i++)
	{	STBWW[i]			= &di_mewe_dummy[0];			// Sicherheitstemperaturbegrenzer
		STWWW[i]			= &di_mewe_dummy[0];			// Sicherheitstemperaturwächter
		WWUW[i]				= &di_mewe_dummy[0];			// Speicher-Temperaturwächter unten 
		WWOW[i]				= &di_mewe_dummy[0];			// Speicher-Temperaturwächter oben
		PUWZAL[i]			= &di_mewe_dummy[0];			// Pumpenstörmeldung
		PUWLAL[i]			= &di_mewe_dummy[0];			// Stoermeldung (Ent-)Ladepumpe		
		STBWWSP[i]		= &di_mewe_dummy[0];			// Stoermeldung STB Speicher			
		PUWTAL[i]			= &di_mewe_dummy[0];			// Stoermeldung Tauscherpumpe		
		BMPUWL[i]			= &di_mewe_dummy[0];			// Betriebsmeldung Ladepumpe
		BMPUWZ[i]			=	&di_mewe_dummy[0];			// Betriebsmeldung Zirkulationspumpe
		BMPUWT[i]			=	&di_mewe_dummy[0];			// Betriebsmeldung Tauscherpumpe
		WWAUS[i]			=	&di_mewe_dummy[0];			// Anlagenschalter Nichtnutzung WWB
	}
	
	// Kessel
	for(i = 0; i < KEMAX; i++)
	{	KEAL[i]				= &di_mewe_dummy[0];			// Kesselalarm
		BRAL[i]				= &di_mewe_dummy[0];			// Brenneralarm
		NACHSP[i]			= &di_mewe_dummy[0];			// Nachspeisealarm
		BMKES[i]			= &di_mewe_dummy[0];			// Betriebsmeldung Kessel
//ulsch
		PUKEAL[i]			= &di_mewe_dummy[0];			// Kesselpumpen-Alarm	
		SSFKE	[i]			= &di_mewe_dummy[0];			// Schornsteinfegertaste
		STBKE	[i]			= &di_mewe_dummy[0];			// STB Kessel
		DRKE	[i]			= &di_mewe_dummy[0];			// Max-Druck Kessel
		NOTAUS[i]			= &di_mewe_dummy[0];			// Not-Aus Kessel	
		BMPUKE[i]			= &di_mewe_dummy[0];			// Betriebsmeldung Kesselpumpe 	
	}
	
	// Netztrennung
	for(i = 0; i < NTMAX; i++)
	{	
		STBNT[i]			= &di_mewe_dummy[0];			// Sicherheitstemperaturbegrenzer
		STWNT[i]			= &di_mewe_dummy[0];			// Sicherheitstemperaturwächter
		RKZUNT[i]			= &di_mewe_dummy[0];			// RM Klappe ZU NT
		RKAUFNT[i]		= &di_mewe_dummy[0];			// RM Klappe AUF NT
		DRMAXNT[i]		= &di_mewe_dummy[0];			// Max-Druck NT
		DRMASNT[i]		= &di_mewe_dummy[0];			// Max-Druck NT sek.
		DRMISNT[i]		= &di_mewe_dummy[0];			// Min-Druck NT sek.
		RVZUNT[i]			= &di_mewe_dummy[0];			// RM Ventil ZU NT
		PUALNT[i]			= &di_mewe_dummy[0];			// Pumpenstörmeldung
		BMPUNT[i]			=	&di_mewe_dummy[0];			// Betriebsmeldung Tauscherpumpe
	}

	for(i = 0; i < DE_UNI_ANZ; i++)
		DE_UNI[i]			= &di_mewe_dummy[0];

	for ( i = 0; i < SM_UNI_ANZ; i++ )
		SM_UNI[i] = &di_mewe_dummy[0];					// universeller Digitaleingang für Störmeldungen	
	
	for (i = 0; i < BM_UNI_ANZ; i++ )
		BM_UNI[i] = &di_mewe_dummy[0];					// universeller Digitaleingang für Betriebsmeldungen
		
	//----------------------------------------------------------------------------
	// Alle Pointer auf Zähl-Eingänge mit einem Dummy-Eingang vorbelegen
	//----------------------------------------------------------------------------
	for(i=0; i<8; i++)
		ZE[i] = &zaehler_dummy[0];							// R36
	for(i=0; i<6; i++)
		X1ZE[i] = &zaehler_dummy[0];						// R37_1	
	for(i=0; i<6; i++)
		X2ZE[i] = &zaehler_dummy[0];						// R37_2	
				
	//----------------------------------------------------------------------------
	// Alle Pointer auf Analogausgänge mit einem Dummy-Ausgang vorbelegen
	//----------------------------------------------------------------------------
	// Heizung
	for(i = 0; i < HKMAX; i++)
	{	RVENT[i]			= &Aout_dummy[0];					// Regelventil
	}

	// Warmwasser
	for(i = 0; i < WWMAX; i++)
	{	RVENTWW[i]	= &Aout_dummy[0];						// Regelventil
		PUWZ[i]			= &Aout_dummy[0];						// Zirkulationspumpe
		PUWL[i]			= &Aout_dummy[0];						// Zirkulationspumpe		
		RVENTSP[i]	= &Aout_dummy[0];						// Strahlpumpe
		RVENTMI[i]	= &Aout_dummy[0];						// Mischer	
	}

	// Kessel
	for(i = 0; i < KEMAX; i++)
	{	RVENTKE[i]		= &Aout_dummy[0];					// Regelventil oder Führungsgröße
		RVENTKR[i]		= &Aout_dummy[0];					// Regelventil Rücklauf-Anhebung
		DZPUKE[i]			= &Aout_dummy[0];					// Drehzahl Kesselpumpe
	}

	// Netztrennung
	for(i = 0; i < NTMAX; i++)
	{	RVENTNT[i]		= &Aout_dummy[0];					// Regelventil
	}

	// Sammelstörmeldung über Analogausgang
	AASSTM[0]			= &Aout_dummy[0];

//ulsch	
	for(i = 0; i < AA_UNI_ANZ; i++)
		AA_UNI[i]			= &Aout_dummy[0];
		
	for (i = 0; i < SOLMAX; i++ )
		RVENTSO[i]		= &Aout_dummy[0];					// Regelventil	
		
	#if TAA_ANZ > 0
	for ( i = 0; i < TAA_ANZ; i++ )
		TAA[i] = &Aout_dummy[0];
	#endif	

	#if ANFORD > 0
	for ( i = 0; i < ANFORD; i++ )
		TMANF[i] = &Aout_dummy[0];
	#endif	

	//----------------------------------------------------------------------------
	// Alle Pointer auf Digitalausgänge mit einem Dummy-Ausgang vorbelegen
	//----------------------------------------------------------------------------
	// Heizung
	for(i = 0; i < HKMAX; i++)
	{	RZU[i]				= &Dout_dummy[0];					// Regelventil ZU-fahren
		RAUF[i]				= &Dout_dummy[0];					// Regelventil AUF-fahren
		PU[i]					= &Dout_dummy[0];					// Heizungspumpe
		PUMIN[i]			= &Dout_dummy[0];					// Heizungspumpe Minimaldrehzahl

		#if ( PUDO_HK1 == 1 || PUDO_HK2 == 1 || PUDO_HK3 == 1 || PUDO_HK4 == 1 )
			PUD[i]				= &Dout_dummy[0];				// Heizungspumpe (Doppelpumpe)
		#endif			
	}
	
	// Warmwasser
	for(i = 0; i < WWMAX; i++)
	{	RWWZU[i]			= &Dout_dummy[0];					// Regelventil ZU-fahren			
		RWWAUF[i]			= &Dout_dummy[0];					// Regelventil AUF-fahren	
		PUWLEA[i]			= &Dout_dummy[0];					// Ladepumpe
		PUWZEA[i]			= &Dout_dummy[0];					// Zikulationspumpe
		PUWTEA[i]			= &Dout_dummy[0];					// Tauscherpumpe
		ELVENT[i]			= &Dout_dummy[0];					// Elektrisches Umschaltventil
		PUGWW[i]			= &Dout_dummy[0];
	}
	
	// Kessel
	for(i = 0; i < KEMAX; i++)
	{	KESTU1[i]			= &Dout_dummy[0];					// Kessel Brennerstufe 1
		KESTU2[i]			= &Dout_dummy[0];					// Kessel Brennerstufe 2
		KE[i]					= &Dout_dummy[0];					// Kessel Freigabe
//ulsch	
		MKEZU[i]			= &Dout_dummy[0];					// Kessel Modulation ZU	
		MKEAUF[i]			= &Dout_dummy[0];					// Kessel Modulation AUF
		PUKEEA[i]			= &Dout_dummy[0];					// Kesselpumpe		
		KLKEAZ[i]			= &Dout_dummy[0];					// Kesselabsperrklappe AUF/ZU
		KLKEZU[i]			= &Dout_dummy[0];					// Kesselabsperrklappe ZU	
		KLKEAUF[i]		= &Dout_dummy[0];					// Kesselabsperrklappe AUF	
		RKEZU[i]			= &Dout_dummy[0];					// Rücklauf-Anhebung: Kesselkreis ZU	
		RKEAUF[i]			= &Dout_dummy[0];					// Rücklauf-Anhebung: Kesselkreis AUF
	}

	// Vorregelung (Netztrennung)
	for(i = 0; i < NTMAX; i++)
	{	RNTZU[i]			= &Dout_dummy[0];					// Regelventil ZU-fahren
		RNTAUF[i]			= &Dout_dummy[0];					// Regelventil AUF-fahren
		KLNTAZ[i]			= &Dout_dummy[0];					// Kesselabsperrklappe AUF/ZU
#if PU_NT1 > 0
		PUNTEA[i]			= &Dout_dummy[0];					// Pumpe Netztrennung (Hauptpumpe)
#endif
	}
	
	// Sammelstörmeldung über Relais
	SSTM[0]				= &Dout_dummy[0];						// Sammelstörmeldung

	// Ferienstatus
	FEROUT[0]			= &Dout_dummy[0];

//ulsch	
	for(i = 0; i < DA_UNI_ANZ; i++)
		DA_UNI[i]			= &Dout_dummy[0];		
		
	for ( i = 0; i < SOLMAX; i++ )
	{
		PUKOLEA[i]		= &Dout_dummy[0];
		PUSOEA[i]			= &Dout_dummy[0];	
	}				
}
// Ende fill_dummy


//-----------------------------------------------------------------------------	
// Unterprogramm zu load_eapointer	36, 37, 38, 39, R33
// Laden der Pointerfelder mit den Adressen der Ein-Ausgangsspeicher
//------------------------------------------------------------------------------
char LoadPointer(char typ, char klasse, char num, char idx, char ext, char iodev, unsigned int *adress)
{
	mwsp					*ptadr = &mewe_dummy[0];
	mwsp					*aeadr = &mewe_dummy[0];
	doutsp 				*doadr = &Dout_dummy[0];
	dinpsp 				*diadr = &di_mewe_dummy[0];
	aasp					*aadr	 = &Aout_dummy[0];
	zaehlsp				*zaehladr;
	char    			fehl, func;
	
	func = fehl = 0;
	
	num -= 1;													// Aufzählung gelangt in nullbasierende Felder
																		// "num" ist auch der Return-Parameter
	switch(typ)
	{
		case 1://----------------------------- Pt1000-Eingang
			switch(iodev)
			{	
				case R36:
					ptadr =	&pt_mewe[idx];
					fehl  = (char)(idx);
					break;
				case R38:	
					ptadr =	&mod38[ext-1].pt_mewe[idx];
					fehl  = (char)(idx);
					break;
			}
					
			switch(klasse)
			{	// Anmerkung: nicht alle Klassen sind in einem Projekt vorhanden
				case 0:													// dummy
					num = 0;
					break; 

				case 1:													// Außentemperatur			TA
					mwpadr = &TA[num];
					break;

				case 2:													// Vorlauf		Primär		TVP
					mwpadr = &TVP[num];
					break;

				case 3:													// Rücklauf		Primär		TRP
					mwpadr = &TRP[num];
					break;
					
				case 4:													// Vorlauf		Sekundär	TVS
					mwpadr = &TVS[num];
					break;

				case 5:													// Rücklauf		Sekundär	TRS
					mwpadr = &TRS[num];
					break;	

				case 6:													// Raumtemperatur	 			TI
					mwpadr = &TI[num];
					break;	

				case 7:													// Temp.am Wärmeübertrager	TWWC	
					mwpadr = &TWWC[num];
					break;	

				case 8:													// Speichertemp. 1 oben			TWW1L
					mwpadr = &TWW1L[num];
					break;	

				case 9:													// Speichertemp. 2 unten		TWW2L
					mwpadr = &TWW2L[num];
					break;	

				case 0x0A:											// Warmwassertem.						TWWV
					mwpadr = &TWWV[num];
					break;

				case 0x0B:											// Vorlauf Kessel						TVKE
					mwpadr = &TVKE[num];
					break;
										
				case 0x0C:											// Rücklauf Kessel					TRKE
					mwpadr = &TRKE[num];
					break;
				
				case 0x0D:											// Vorlauf Sek. Netztrennung	TVSEK
					mwpadr = &TVSEK[num];
					break;

				case 0x0E:
					mwpadr = &TP_UNI[num];
					break;
					
				case 0x0F:
					mwpadr = &TKOLL[num];
					break;
					
				case 0x10:
					mwpadr = &TPUF1L[num];
					break;
					
				case 0x11:
					mwpadr = &TPUF2L[num];
					break;																
				
				case 0x12:
					mwpadr = &TWWCSO[num];
					break;
					
				case 0x13:
					mwpadr = &TWWP[num];
					break;					
				
				case 0x14:
					mwpadr = &TVGKE[num];					// Vorlauf Kessel-Gesamt
					break;					
				
				case 0x15:											// Temperatur Zirkulation
					mwpadr = &TWWZ[num];
					break;					

				case 0x16:
					mwpadr = &TRGKE[num];					// Rücklauf Kessel-Gesamt
					break;					

				case 0x17:
					mwpadr = &TVSGNT[num];				// Vorlauf WT sek.gesamt
					break;					
				
				case 0x18:											// Rücklauf Sek. Netztrennung
					mwpadr = &TRSEK[num];
					break;
				
				case 0x19:											// Rücklauf Sek. Netztrennung
					mwpadr = &TRPNT[num];
					break;

				case 0x1A:
					mwpadr = &TRSGNT[num];				// Rücklauf WT sek.gesamt
					break;					
				
				case 0x1B:
					mwpadr = &TV_WP[num];				// SiWa
					break;
					
				case 0x1C:
					mwpadr = &TR_WP[num];				// SiWa
					break;
					
				case 0x1D:
					mwpadr = &TSPo_WP[num];				// SiWa
					break;
					
				case 0x1E:
					mwpadr = &TSPu_WP[num];				// SiWa
					break;
					
				case 0x1F:
					mwpadr = &TVST_WP[num];				// SiWa
					break;
					
				case 0x20:
					mwpadr = &TRST_WP[num];				// SiWa
					break;
					
				case 0x21:
					mwpadr = &TVQ_WP[num];				// SiWa
					break;
					
				case 0x22:
					mwpadr = &TRQ_WP[num];				// SiWa
					break;
					
				case 0x23:
					mwpadr = &TVH_WP[num];				// SiWa
					break;
					
				case 0x24:
					mwpadr = &TRH_WP[num];				// SiWa
					break;
		
				case 0x25:
					mwpadr = &TSPm_WP[num];				// SiWa
					break;
					
				default:
					klasse = 0;
					break;	
			}
			if(klasse > 0)
			{
				if( (*mwpadr)->stat == NICHTV )		// Dummy-Vorbelegung ?
				{	*mwpadr = ptadr;				
					*adress = (unsigned int)mwpadr;
				}	
				else															// IN Name schon vergeben
					Fehler(7, idx);
			}
			break;
			
			
		case 2://------------------------------ 0-10V Eingang
			switch(iodev)
			{	
				case R36:
					aeadr =	&ae_mewe[idx-2];		// beim idx sind die ersten 2 IN's (Pt1000) abzuziehen
					fehl  = (char)(idx);
					break;
			}

			switch(klasse)
			{	// Anmerkung: nicht alle Klassen sind in einem Projekt vorhanden
				// beim idx sind die ersten 2 IN's (Pt1000) abzuziehen
				case 0:											// dummy
					num = 0;
					break;
				
				case 1:														// Beispiel: Luftqualitätsfühler	LQF
					aepadr	= &LQF[num];
					break;
					
				case 2:
					aepadr	= &AE_UNI[num];
					break;					

				case 3:															// Adaptions-Kennwert
					aepadr	= &ADAPT[num];
					break;

				#if TAE_ANZ == 1	
				case 4:	
					aepadr	= &TAE[num];
					break;
				#endif

				#if RM_POWER_ANZ
				case 5:
					aepadr = &RM_POWER[num];
					break;
				#endif

				#if AE_DRUCK_ANZ
				case 6:
					aepadr = &AE_DRUCK[num];
					break;
				#endif

				default:
						klasse = 0;
						break;	
				}
				if(klasse > 0)
				{
					if( (*aepadr)->stat == NICHTV )		// Dummy-Vorbelegung ?
					{	*aepadr = aeadr;				
						*adress = (unsigned int)aepadr;
					}	
					else															// IN Name schon vergeben
						Fehler(7, idx);
				}
				break;
			
		case 3://-------------------------------- Digital Eingang
			switch(iodev)
			{	
				case R36:
					diadr =	&di_mewe[idx-2];						// beim idx sind die ersten 2 IN's (Pt1000) abzuziehen
					fehl  = (char)(idx-2);
					break;
				case R37:	
					diadr =	&mod37[ext-1].di_mewe[idx];
					fehl  = (char)(idx);
					break;
				case R39:
					diadr =	&mod39[ext-1].di_mewe[idx];
					fehl  = (char)(idx);
					break;
			}
			
			switch(klasse)
			{	// Anmerkung: nicht alle Klassen sind in einem Projekt vorhanden
				
				case 0:											// dummy
					num = 0;
					break; 
				
				case 1:															// Sicherh.temp.begrenzer Heizung	STBHK
					dipadr = &STBHK[num];
					break;

				case 2:															// Sicherh.temp.begrenzer Warmw.	STBWW
					dipadr = &STBWW[num];
					break;

				case 3:															// Sicherh.temp.wächter Heizung		STWHK
					dipadr = &STWHK[num];
					break;

				case 4:															// Sicherh.temp.wächter	Wamw.			STWWW
					dipadr = &STWWW[num];
					break;

				case 5:															// Speichertemp.wächter unten 		WWUW
					dipadr = &WWUW[num];
					break;

				case 6:															// Speichertemp.wächter oben			WWOW
					dipadr = &WWOW[num];
					break;

				case 7:															// Kesselstörung Si-Kette					KEAL			
					dipadr = &KEAL[num];
					break;

				case 8:															// Brennerstörung 								BRAL			
					dipadr = &BRAL[num];
					break;

				case 9:															// Störung Nachspeisung						NACHSP
					dipadr = &NACHSP[num];
					break;

			case 0x0A:													// Pumpenstörung (Sammelmeldung)	PUAL			
						dipadr = &PUAL[num];
					break;
					
			case 0x0B:													// Pumpenstörung (Sammelmeldung)	PUWZAL
						dipadr = &PUWZAL[num];
					break;
					
			case 0x0C:													// Stoermeldung (Ent-)Ladepumpe		PUWLAL			
						dipadr = &PUWLAL[num];
					break;
					
			case 0x0D:													// Sicherheits-Temperatur-Begrenzer	STBWWSP			
						dipadr = &STBWWSP[num];
					break;															
					
			case 0x0E:													// Betriebsmeldung Kessel
						dipadr = &BMKES[num];
					break;															
		
			case 0x0F:													// Sammelmeldung Pumpenstörung  	PUSAMAL			
						dipadr = &PUSAMAL[num];
					break;
					
			case 0x10:													// Sicherh.temp.begrenzer Netztrennung	STBNT			
						dipadr = &STBNT[num];
					break;
					
			case 0x11:													// Sammelmeldung Anlage  	SAMAL			
						dipadr = &SAMAL[num];
					break;
					
			case 0x12:													// Stoermeldung Tauscherpumpe		PUWTAL			
						dipadr = &PUWTAL[num];
					break;
					
			case 0x13:													// Freigabe Regelung Heizkreis			
						dipadr = &FRGHK[num];
					break;					

//ulsch
			case 0x14:			
					dipadr = &DE_UNI[num];			
					break;
	
			case 0x15:													// Ferienschalter			
					dipadr = &FERINP[num];			
					break;
					
			case 0x16:													// Ferienschalter			
					dipadr = &PUKEAL[num];			
					break;					
//josch					
			case 0x17:													// Betriebsmeldung Heizungspumpe
					dipadr = &BMPU[num];			
					break;					
					
			case 0x18:													// Betriebsmeldung Ladepumpe
					dipadr = &BMPUWL[num];			
					break;					
					
			case 0x19:													// Betriebsmeldung Zirkulationspumpe
					dipadr = &BMPUWZ[num];			
					break;					
					
			case 0x1A:													// Betriebsmeldung Tauscherpumpe
					dipadr = &BMPUWT[num];			
					break;					
					
			case 0x1B:													// Anlagenschalter Nichtnutzung HK
					dipadr = &HKAUS[num];
					break;
								
			case 0x1C:													// Anlagenschalter Nichtnutzung WWB
					dipadr = &WWAUS[num];
					break;
								
			case 0x1D:													// Quittierungstaste
					dipadr = &QUITTS[num];
					break;

			case 0x1E:													// Schornsteinfegertaste
					dipadr = &SSFKE[num];
					break;
								
			case 0x1F:													// Sicherh.temp.wächter Netztrennung	STWNT			
						dipadr = &STWNT[num];
					break;
								
			case 0x20:													// Sicherh.temp.begrenzer Kessel	STBKE			
						dipadr = &STBKE[num];
					break;
								
			case 0x21:													// Max-Druck Kessel	DRKE			
						dipadr = &DRKE[num];
					break;

			#if ( PUDO == 1 )
			case 0x22:													// Pumpenstörung PUDAL (Doppelpumpe)			
						dipadr = &PUDAL[num];
					break;
			#endif
								
			case 0x23:			
					dipadr = &BM_UNI[num];			
					break;

			case 0x24:			
					dipadr = &SM_UNI[num];			
					break;

			case 0x25:			
					dipadr = &ADAPT_T[num];			
					break;

			case 0x26:			
					dipadr = &ADAPT_H[num];			
					break;

			case 0x27:			
					dipadr = &RKZUNT[num];			
					break;

			case 0x28:			
					dipadr = &RKAUFNT[num];			
					break;

			case 0x29:			
					dipadr = &DRMAXNT[num];			
					break;

			case 0x2A:			
					dipadr = &DRMASNT[num];			
					break;

			case 0x2B:			
					dipadr = &DRMISNT[num];			
					break;

			case 0x2C:			
					dipadr = &RVZUNT[num];			
					break;

			case 0x2D:
					dipadr = &PUALNT[num];
					break;
					
			case 0x2E:													// Betriebsmeldung Ladepumpe
					dipadr = &BMPUNT[num];			
					break;					

			case 0x2F:													// Not-Aus Kessel
					dipadr = &NOTAUS[num];			
					break;					

			case 0x30:													// Not-Aus Kessel
					dipadr = &BMPUKE[num];			
					break;					
								
			default:
					klasse = 0;
					break;	
			}		 
			if(klasse > 0)
			{
				if( (*dipadr)->bstat == NICHTV )		// Dummy-Vorbelegung ?
				{	*dipadr = diadr;				
					*adress = (unsigned int)dipadr;
				}	
				else																// IN Name schon vergeben
					Fehler(7, fehl);
			}
			break;
			
		case 4://----------------------------------- Zähleingang
			switch(klasse)									
			{	// Anmerkung: nicht alle Klassen sind in einem Projekt vorhanden
				// beim idx sind die ersten 2 IN's (Pt1000) abzuziehen
				case 0:																// dummy
					num = 0;
					break; 
				case 1:																// Standard Zähleingang R36
					if(ZE[num]->zstat	== NICHTV)	// Dummy-Vorbelegung ?
					{	if(num == (idx-2))								// Zähleingang ZINx muß INx entsprechen
						{	ZE[num]			= &zaehler[idx-2];
							ze_vis[num] = SICHTBAR;
							*adress = (unsigned int)&ZE[num];
						}
						else
							Fehler(10, idx);								// Falscher Zähleingang
					}
					else																// IN Name schon vergeben
						Fehler(7, idx);
					break;
				
				case 2:																	// Standard Zähleingang R37_1
					if(X1ZE[num]->zstat	== NICHTV)	// Dummy-Vorbelegung ?
					{	if(num == idx)											// Zähleingang ZINx muß INx entsprechen
						{	X1ZE[num]			= &mod37[ext-1].zaehler[idx];
							x1ze_vis[num] = SICHTBAR;
							*adress = (unsigned int)&X1ZE[num];
						}
						else
							Fehler(10, (char)(idx+10));				// Falscher Zähleingang   (+10: R36-IN's)
					}
					else																	
						Fehler(7, (char)(idx+10));					// IN Name schon vergeben (+10: R36-IN's)
					break;

				case 3:																	// Standard Zähleingang R37_2
					if(X2ZE[num]->zstat	== NICHTV)	// Dummy-Vorbelegung ?
					{	if(num == idx)											// Zähleingang ZINx muß INx entsprechen
						{	X2ZE[num]			= &mod37[ext-1].zaehler[idx];
							x2ze_vis[num] = SICHTBAR;
							*adress = (unsigned int)&X2ZE[num];
						}
						else
							Fehler(10, (char)(idx+10+6+4));			// Falscher Zähleingang
					}																				// (+10: R36-IN's  +6: R37_1 IN's  +4: Lücke)
					else																		
						Fehler(7, (char)(idx+10+6+4));				// IN Name schon vergeben
					break;
			}		
			break;   		
		
		case 5://--------------------------------- Analog-Ausgang
			switch(iodev)
			{	
				case R36:
					aadr =	&Aout[idx-10];							// beim idx sind die 10 IN's abzuziehen
					fehl  = (char)(idx-10);
					break;
						
				case R37:
					aadr =	&mod37[ext-1].Aout[idx-6];	// beim idx sind die 6 IN's abzuziehen
					fehl  = (char)(idx-6);
					break;
			}
			
			switch(klasse)									
			{	// Anmerkung: nicht alle Klassen sind in einem Projekt vorhanden
				// beim idx sind die 10 IN's abzuziehen
				case 0:															// dummy
					num = 0;
					break;
					 
				case 1:															// Heizungsregelung			RVENT
					aapadr	= &RVENT[num];
					break;

				case 2:															// Warmwasserregelung		RVENTWW
					aapadr	= &RVENTWW[num];
					break;

				case 3:															// Kesselregelung				RVENTKE
					aapadr	= &RVENTKE[num];
					break;

				case 4:															// Sammelstörmeldung		ASSTM
					aapadr	= &AASSTM[num];
					break;

				case 5:															// Warmwasserregelung		PUWZ 
					aapadr	= &PUWZ[num];
					break;
					
				case 6:															// Warmwasserregelung		RVENTSP
					aapadr	= &RVENTSP[num];
					break;
					
				case 7:															// Warmwasserregelung		RVENTMI 
					aapadr	= &RVENTMI[num];
					break;
					
				case 8:															// Warmwasserregelung		PUWL
					aapadr	= &PUWL[num];
					break;															
					
				case 9:															// Netztrennung					RVENTNT
					aapadr	= &RVENTNT[num];
					break;															
					
				case 0x0A:
					aapadr	= &AA_UNI[num];
					break;																			
					
				case 0x0B:													// Solar-Regelung
					aapadr	= &RVENTSO[num];
					break;															

				case 0x0C:													// Kesselregelung	Rücklauf-Anhebung
					aapadr	= &RVENTKR[num];
					break;

				#if TAA_ANZ > 0
				case 0x0D:													// Außentemperatur
					aapadr	= &TAA[num];
					break;
				#endif

				#if ANFORD > 0
				case 0x0E:													// Wärmeanforderung
					aapadr	= &TMANF[num];
					break;
				#endif
				
				case 0x0F:													// Drehzahl Kesselpumpe
					aapadr	= &DZPUKE[num];
					break;
				
				default:
					klasse = 0;
					break;	
				
			}
			if(klasse > 0)
			{
				if(	(*aapadr)->astat == NICHTV )		// Dummy-Vorbelegung ?
				{	 *aapadr = aadr;
					(*aapadr)->astat = 1;							// belegt markieren
					*adress = (unsigned int)aapadr;
				}	
				else																// AA Name schon vergeben
					Fehler(8, fehl);
			}
			break;		
				
		case 6://--------------------------------- Digital-Ausgang
			switch(iodev)
			{	
				case R36:
					doadr =	&Dout[idx-12];							// beim idx sind die 10 IN's und 2 AA's abzuziehen
					fehl  = (char)(idx-12);
					break;
					
				case R37:
					doadr =	&mod37[ext-1].Dout[idx-10];	// beim idx sind die 6 IN's und 4 AA's abzuziehen
					fehl  = (char)(idx-10);
					break;

				case R33:
					doadr =	&mod33[ext-1].Dout[idx];
					fehl  = (char)(idx);
					break;
			}
				
			switch(klasse)									
			{	// Anmerkung: nicht alle Klassen sind in einem Projekt vorhanden.
				// Die nicht benutzten Klassen werden in "Output.c" nicht behandelt (func=0).
				// Funktionen: STATISCH, DYNAMISCH, VERRIEG1, VERRIEG2
				case 0:															// dummy
					num = 0;
					break; 

				case 1:															// Regelventil Heiz.zu		RxZU
					dopadr	= &RZU[num];
					func		= VERRIEG1;
					break;

				case 2:															// Regelventil Heiz.auf		RxAUF
					dopadr	= &RAUF[num];
					func		= VERRIEG2;
					break;

				case 3:															// Regelventil WW.zu			RWWZU
					dopadr	= &RWWZU[num];
					func		= VERRIEG1;
					break;

				case 4:															// Regelventil WW.auf			RWWAUF
					dopadr	= &RWWAUF[num];
					func		= VERRIEG2;
					break;

				case 5:															// Heizungs-Pumpe					PUxEA
					dopadr	= &PU[num];
					func		= STATISCH;
					break;

				case 6:															// Heizungs-Pumpe min			PUxMIN
					dopadr	= &PUMIN[num];
					func		= STATISCH;
					break;

				case 7:																// Lade-Pumpe						PUWLxEA
					dopadr	= &PUWLEA[num];
					func		= STATISCH;
					break;

				case 8:																// Zikulations-Pumpe		PUWZxEA
					dopadr	= &PUWZEA[num];
					func		= STATISCH;
					break;

				case 9:																// ELVENTx
					dopadr	= &ELVENT[num];
					func		= STATISCH;
					break;

				case 0x0A:														// PUGWWxEA
					dopadr	= &PUGWW[num];
					func		= STATISCH;
					break;

				case 0x0B:														// Sammelstörmeldung	SSTMx
					dopadr	= &SSTM[num];
					func		= STATISCH;
					break;

				case 0x0C:														// Kessel Stufe 1			KExS1EA
					dopadr	= &KESTU1[num];
					func		= STATISCH;
					break;

				case 0x0D:														// Kessel Stufe 2			KExS2EA
					dopadr	= &KESTU2[num];
					func		= STATISCH;
					break;

				case 0x0E:														// Kessel Freigabe		KExEA
					dopadr	= &KE[num];
					func		= STATISCH;
					break;
					
				case 0x0F:														// Tauscher-Pumpe			PUWTxEA
					dopadr	= &PUWTEA[num];
					func		= STATISCH;
					break;

				case 0x10:														// Regelventil NT.zu	RNTxZU
					dopadr	= &RNTZU[num];
					func		= VERRIEG1;
					break;

				case 0x11:														// Regelventil NT.auf	RNTxAUF
					dopadr	= &RNTAUF[num];
					func		= VERRIEG2;
					break;

				case 0x12:														// Ferienstatus
					dopadr	= &FEROUT[num];
					func		= STATISCH;
					break;
					
				case 0x13:
					dopadr	= &DA_UNI[num];
					func		= STATISCH;
					break;
					
				case 0x14:
					dopadr	= &MKEZU[num];							// Modulation Kessel ZU
					func		= VERRIEG1;
					break;
					
				case 0x15:
					dopadr	= &MKEAUF[num];							// Modulation Kessel AUF
					func		= VERRIEG2;
					break;	
					
				case 0x16:
					dopadr	= &PUKEEA[num];							// Kesselpumpe E/A
					func		= STATISCH;
					break;	
					
				case 0x17:
					dopadr	= &PUKOLEA[num];
					func		= STATISCH;
					break;																							

				case 0x18:
					dopadr	= &PUSOEA[num];
					func		= STATISCH;
					break;																													

				case 0x19:
					dopadr	= &KLKEAZ[num];
					func		= STATISCH;
					break;																													

				case 0x1A:
					dopadr	= &KLKEZU[num];
					func		= VERRIEG1;
					break;

				case 0x1B:
					dopadr	= &KLKEAUF[num];
					func		= VERRIEG2;
					break;

				#if PU_NT1 > 0
				case 0x1C:
					dopadr	= &PUNTEA[num];
					func		= STATISCH;
					break;																													
				#endif

				#if ( PUDO_HK1 == 1 || PUDO_HK2 == 1 || PUDO_HK3 == 1 || PUDO_HK4 == 1 )
				case 0x1D:														// Heizungs-Pumpe	PUDxEA (Doppelpumpe)
					dopadr	= &PUD[num];
					func		= STATISCH;
					break;
				#endif
					
				case 0x1E:
					dopadr	= &RKEZU[num];							// Rücklauf-Anhebung: Kesselkreis ZU
					func		= VERRIEG1;
					break;
					
				case 0x1F:
					dopadr	= &RKEAUF[num];							// Rücklauf-Anhebung: Kesselkreis AUF
					func		= VERRIEG2;
					break;	

				case 0x20:
					dopadr	= &KLNTAZ[num];
					func		= STATISCH;
					break;																													
										
				default:
					klasse = 0;
					break;	
				
			}		
			if(klasse > 0)
			{
				if( (*dopadr)->func == NICHTV )			// Dummy-Vorbelegung ?
				{	 	 *dopadr = doadr;
						(*dopadr)->func = func;
						 *adress = (unsigned int)dopadr;
				}	
				else																// DA Name schon vergeben
					Fehler(9, fehl);
			}
			break;
	}
	return(num);	
}


