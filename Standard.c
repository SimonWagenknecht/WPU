/*									Standard.c																*/
/*		Standard-Adressen im EEPROM und RTC			
			für den Transport von Standardwerten zwischen Prozessor-RAM und EEPROM/RTC
			auf dem Board I²C Bus.
			Der Transport wird vom Task "BicMove.c" ausgeführt mit unterschiedlichen
			Richtungen je nach Auftrag:
			-	Normal Reset											(RAM	 <--	EEPROM)
			- Kaltstart							 (FLASH  --> RAM 		--> EEPROM)
			- Kommando SYS; 00; PARAM. SICHERN	(RAM 		--> EEPROM)
*/

// Im Regler R36B (Renesas CPL) wurde der EEPROM durch ein FERRO-RAM ersetzt. Der Begriff EEPROM wird weiterhin verwendet.

/*--------------------------------------------------------------*/
/* 		Tabelle der zu übertragenden Daten 												*/
/*		zwischen Prozessor-RAM und EEPROM	bzw. RTC-RAM						*/
/*--------------------------------------------------------------*/
/*		Tabellenaufbau (5 Parameter):
			1. Speicherschaltkreis EEPADR (EEPROM) oder RTCADR (RTC-Uhr)
			2. Zeiger auf die Variable im Toshiba-RAM		zu finden in "UserRam.c"
			3. Name der Speicherstelle im EEPROM/RTC    siehe oben bei den Speicherplatzdefinitionen
			4. Anzahl der zu übertragenden Bytes
			5. Kaltstartwert
*/
			
#include "sfr32c87.h"
#include "struct.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#if MODBUS_UNI > 0
#include "Modbus/modbus.h"
#endif

/* ------------------ Userparameter mit Kaltstartwerten ------------- */
const bicvec bicuser[]	=	{
	
	// Steuer
	{EEPADR,	(char	*)&Ta1mzk,		TA1MZK_ADR,			1, 	 10},	// [min]
	{EEPADR,	(char	*)&Ta1mhzk,		TA1MHZK_ADR,		1, 	 12},	// [h]
	{EEPADR,	(char	*)&bc_vorra,	BC_VORRA_ADR,		1, 	  0},	// 

	{EEPADR,	(char	*)&Schalt_Pause, SCHALT_PAUSE_ADR,	1, 3},	// 3 Sek Einschaltpause
	{EEPADR,	(char	*)&Tae,				TAE_ADR,				2, 	160},	// Temp. Winter ein	 [°C] * 10	
	{EEPADR,	(char	*)&Taa,				TAA_ADR,				2, 	170},	// Temp. Sommer aus	 [°C] * 10	

#if ARCHIV_SDM == 1		// Josch-SDM : Archiv mit SD-Card-Memory
	{EEPADR,	(char	*)&sdm_arch_stop,	SDM_ARCH_STOP_ADR,	1, 	0},	// 1 = Benutzer-Stopp
#endif


// MBUS
#if MBUSANZ > 0
	{EEPADR,	(char	*)&MbusFrequ,			MBUSFREQU_ADR,			2, 			1440},	// Abfrage-Frequenz in Minuten
	{EEPADR,	(char	*)&AktStd.std,		MBUSAKT_ADR,				1, 	  		06},	// Abfrage-Stunde
#endif

#if TAE_ANZ == 1
	{EEPADR,	(char	*)&TaeSkalMin,				TA_IN_ADR,			2, 	 -200},	// -20,0 °C -> 0 Volt
	{EEPADR,	(char	*)&TaeSkalMax,				TA_IN_ADR + 2,	2, 	 	400},	// 40,0 °C -> 10 Volt
#endif

#if TAA_ANZ > 0
	{EEPADR,	(char	*)&TaaSkalMin,				TA_OUT_ADR,			2, 	 -200},	// -20,0 °C -> 0 Volt
	{EEPADR,	(char	*)&TaaSkalMax,				TA_OUT_ADR + 2,	2, 	 	400},	// 40,0 °C -> 10 Volt
#endif


#if ANFORD == 1
	{EEPADR,	(char	*)&TmanfSkalMin,			TMANF_ADR,			2, 			0},	// 0 °C
	{EEPADR,	(char	*)&TmanfSkalMax,			TMANF_ADR + 2,	2, 	 1000},	// 90,0 °C
	{EEPADR,	(char	*)&TmanfSkalMinSpg,		TMANF_ADR + 4,	2, 		  0},	// 0 °C -> 0 Volt
	{EEPADR,	(char	*)&TmanfSkalMaxSpg,		TMANF_ADR + 6,	2, 	 1000},	// 90,0 °C -> 10,00 Volt
#endif

	{EEPADR,	(char	*)&TaErsatz[0],		TA_ERSATZ_ADR,			2, 	 -100},	// Januar 	-10,0 °C
	{EEPADR,	(char	*)&TaErsatz[1],		TA_ERSATZ_ADR + 2,	2, 	  -50},	// Februar 	 -5,0 °C
	{EEPADR,	(char	*)&TaErsatz[2],		TA_ERSATZ_ADR + 4,	2, 	    0},	// März 		 -0,0 °C
	{EEPADR,	(char	*)&TaErsatz[3],		TA_ERSATZ_ADR + 6,	2, 	   50},	// April 		  5,0 °C
	{EEPADR,	(char	*)&TaErsatz[4],		TA_ERSATZ_ADR + 8,	2, 	  100},	// Mai 			 10,0 °C
	{EEPADR,	(char	*)&TaErsatz[5],		TA_ERSATZ_ADR + 10,	2, 	  250},	// Juni 		 25,0 °C
	{EEPADR,	(char	*)&TaErsatz[6],		TA_ERSATZ_ADR + 12,	2, 	  250},	// Juli 		 25,0 °C
	{EEPADR,	(char	*)&TaErsatz[7],		TA_ERSATZ_ADR + 14,	2, 	  250},	// August 	 25,0 °C
	{EEPADR,	(char	*)&TaErsatz[8],		TA_ERSATZ_ADR + 16,	2, 	  100},	// September 10,0 °C
	{EEPADR,	(char	*)&TaErsatz[9],		TA_ERSATZ_ADR + 18,	2, 	   50},	// Oktober 		5,0 °C
	{EEPADR,	(char	*)&TaErsatz[10],	TA_ERSATZ_ADR + 20,	2, 	    0},	// November 	0,0 °C
	{EEPADR,	(char	*)&TaErsatz[11],	TA_ERSATZ_ADR + 22,	2, 	  -50},	// Dezember  -5,0 °C
	{EEPADR,	(char	*)&TaErsatzVerz,	TA_ERSATZ_VERZ_ADR,	1, 	   24},	// 24 Stunden

#if LEIST_BEGR == 1
	{EEPADR,	(char	*)&AnzahlBegrenzung, 		LEISTUNG_ANZBEG,	2, 		0},	
	{EEPADR,	(char	*)&DauerBegrenzung, 		LEISTUNG_DAUBEG,	2, 		0},	
#endif

// Einsatz von Ni1000 Fühler am R66
// Der Typ ist parametrierbar in SYS: In. Fuehlertyp (Service-Code 137)
// Standardwert 0 = Pt1000
//              1 = Ni1000 nach DIN43760    ( 7,5 kOhm parallel schalten ,  nicht für negative Temperaturen einsetzbar )
//              2 = Ni1000TK5000 nach L&S   ( 7,5 kOhm parallel schalten ,  nicht für negative Temperaturen einsetzbar )
//	{EEPADR,	(char *)&Tp_Type[U1],					TP_TYPE_ADR + U1, 1,	0},	// Beispiel: Eingang IN1 mit Typ 0 (Pt1000)
//	{EEPADR,	(char *)&Tp_Type[U3],					TP_TYPE_ADR + U3, 1,	2},	// Beispiel: Eingang IN3 mit Typ 2 (Ni1000TK5000 nach L&S)
//	{EEPADR,	(char *)&Tp_Type[U4],					TP_TYPE_ADR + U4, 1,	1},	// Beispiel: Eingang IN4 mit Typ 1 (Ni1000 nach DIN)

};
const char BICUSERZ =  sizeof(bicuser) / sizeof(bicvec) ;


/* ------ Userparameter ohne direkt angegebene Kaltstartwerte ----------- */
/* Die Kaltstartwerte werden in der speziellen Prozedur: Typelist_copy()
	 oder durch die Systeminitialisierungprozedur: SysEEP_InitUser(void) geladen.
	 (beide befinden sich in UserReply.c)																					*/
const bicvec bicuser1[]	=	{
	{EEPADR,	(char	*)&k_dummy,					K_DUMMY_ADR,		2},

#if SPEED_SAVE == 1			// Josch-SDM
	{EEPADR,	(char	*)&asdm_test_start,	ASDM_TEST_START_ADR, 4},
#endif	


// ***** josch: Datenmanager
#if DM_MODE == 1
	{EEPADR,	(char	*)&Slave[0],				SLAVE_ADR,		4},
#endif	

#if ANALOG_AE > 0
	{EEPADR,	(char	*)&AnaInpPara[0],		ANAINPPARA_ADR, 		( ( sizeof(AnaInpPara) > 64 ) ? 64 : sizeof(AnaInpPara) ) },
	{EEPADR,	(char	*)&AnaInpPara[0]+64,ANAINPPARA_ADR+64, 	( ( sizeof(AnaInpPara) > 64 ) ? ( sizeof(AnaInpPara) - 64 ) : 0 ) },
#endif

#if RM_POWER_ANZ
	{EEPADR,	(char	*)&RmPowerPara[0],  RMPOWERPARA_ADR, sizeof(RmPowerPara) },
#endif

#if AE_DRUCK_ANZ > 0
	{EEPADR,	(char	*)&DruckPara[0],		DRUCKPARA_ADR, 		( ( sizeof(DruckPara) > 64 ) ? 64 : sizeof(DruckPara) ) },
	{EEPADR,	(char	*)&DruckPara[0]+64, DRUCKPARA_ADR+64, ( ( sizeof(DruckPara) > 64 ) ? ( sizeof(DruckPara) - 64 ) : 0 ) },
#endif

#if LEIST_BER == 1
	{EEPADR,	(char	*)&MaxLeistung[0],	MITT_LEIST,											sizeof( maxLeistung ) },	
	{EEPADR,	(char	*)&MaxLeistung[1],	MITT_LEIST+SIZE_MITT_LEIST,			sizeof( maxLeistung ) },	
	{EEPADR,	(char	*)&MaxLeistung[2],	MITT_LEIST+SIZE_MITT_LEIST*2,		sizeof( maxLeistung ) },	
	{EEPADR,	(char	*)&MaxLeistung[3],	MITT_LEIST+SIZE_MITT_LEIST*3,		sizeof( maxLeistung ) },	
#endif

#if LEIST_BEGR == 1
	{EEPADR,	(char	*)&GrenzLeistung,		GRENZ_LEIST,										sizeof( grenzLeistungStat ) },
#endif

/***** ulsch : Adaption*****/
#if ( HKANZ >= 1 && ADAP_STAT_HK1 > 0 )
	{EEPADR,	(char	*)AdaptKorr[HK1], ADAPT_KORR_ADR,										sizeof(AdaptKorr[HK1]) },			// Korrekturtemp.
#endif
#if ( HKANZ >= 2 && ADAP_STAT_HK2 > 0 )
	{EEPADR,	(char	*)AdaptKorr[HK2],	ADAPT_KORR_ADR+ADAPT_KORR_LNG,		sizeof(AdaptKorr[HK2]) },			// Korrekturtemp.
#endif
#if ( HKANZ >= 3 && ADAP_STAT_HK3 > 0 )
	{EEPADR,	(char	*)AdaptKorr[HK3],	ADAPT_KORR_ADR+(ADAPT_KORR_LNG*2),sizeof(AdaptKorr[HK3]) },			// Korrekturtemp.
#endif
#if ( HKANZ >= 4 && ADAP_STAT_HK4 > 0 )
	{EEPADR,	(char	*)AdaptKorr[HK4],	ADAPT_KORR_ADR+(ADAPT_KORR_LNG*3),sizeof(AdaptKorr[HK4]) },			// Korrekturtemp.
#endif
	
/***** ulsch : Archivspeicher *****/	
	{EEPADR,	(char	*)&arch_par,				ARCH_PAR_ADR,		1},	

/***** ulsch : Ferienprogramm*****/	
	{EEPADR,	(char	*)ferien,						FERIEN_ADR,	60 },										// 10 * Ferienbeginn, -ende	

#if WMENG == 1
#if ZE7WM == 1
	{ EEPADR,		(char	*)&wmengCtr[0].monInVj,							MON_IN_VJ, 											sizeof(wmengCtr[0].monInVj) 				},
	{ EEPADR,		(char *)&wmengCtr[0].wmeng,								WMENG_ADR, 											sizeof(wmengCtr[0].wmeng) 					},
	{ EEPADR,		(char *)&wmengCtr[0].verbr,								WMENG_MON_ADR, 									sizeof(wmengCtr[0].verbr)						},	
	{ EEPADR,		(char *)&wmengCtr[0].verbr_vj,						WMENG_VJMON_ADR, 								sizeof(wmengCtr[0].verbr_vj)				},	
	{ EEPADR,		(char *)&wmengCtr[0].flow_max,						FLOWMAX_ADR, 										sizeof(wmengCtr[0].flow_max)				},
	{ EEPADR,		(char *)&wmengCtr[0].wmLeistg_max,				LEISTMAX_ADR,										sizeof(wmengCtr[0].wmLeistg_max)		},
#endif
#if ZE8WM == 1
	{ EEPADR,		(char	*)&wmengCtr[1].monInVj,							MON_IN_VJ+SIZE_WMENG, 					sizeof(wmengCtr[1].monInVj) 				},
	{ EEPADR,		(char *)&wmengCtr[1].wmeng,								WMENG_ADR+SIZE_WMENG, 					sizeof(wmengCtr[1].wmeng) 					},
	{ EEPADR,		(char *)&wmengCtr[1].verbr,								WMENG_MON_ADR+SIZE_WMENG, 			sizeof(wmengCtr[1].verbr)						},	
	{ EEPADR,		(char *)&wmengCtr[1].verbr_vj,						WMENG_VJMON_ADR+SIZE_WMENG, 		sizeof(wmengCtr[1].verbr_vj)				},	
	{ EEPADR,		(char *)&wmengCtr[1].flow_max,						FLOWMAX_ADR+SIZE_WMENG, 				sizeof(wmengCtr[1].flow_max)				},
	{ EEPADR,		(char *)&wmengCtr[1].wmLeistg_max,				LEISTMAX_ADR+SIZE_WMENG,				sizeof(wmengCtr[1].wmLeistg_max)		},
#endif
#if ZE9WM == 1
	{ EEPADR,		(char	*)&wmengCtr[2].monInVj,							MON_IN_VJ+2*SIZE_WMENG, 				sizeof(wmengCtr[2].monInVj) 				},
	{ EEPADR,		(char *)&wmengCtr[2].wmeng,								WMENG_ADR+2*SIZE_WMENG, 				sizeof(wmengCtr[2].wmeng) 					},
	{ EEPADR,		(char *)&wmengCtr[2].verbr,								WMENG_MON_ADR+2*SIZE_WMENG, 		sizeof(wmengCtr[2].verbr)						},	
	{ EEPADR,		(char *)&wmengCtr[2].verbr_vj,						WMENG_VJMON_ADR+2*SIZE_WMENG, 	sizeof(wmengCtr[2].verbr_vj)				},	
	{ EEPADR,		(char *)&wmengCtr[2].flow_max,						FLOWMAX_ADR+2*SIZE_WMENG, 			sizeof(wmengCtr[2].flow_max)				},
	{ EEPADR,		(char *)&wmengCtr[2].wmLeistg_max,				LEISTMAX_ADR+2*SIZE_WMENG,			sizeof(wmengCtr[2].wmLeistg_max)		},
#endif
#if ZE10WM == 1
	{ EEPADR,		(char	*)&wmengCtr[3].monInVj,							MON_IN_VJ+3*SIZE_WMENG, 				sizeof(wmengCtr[3].monInVj) 				},
	{ EEPADR,		(char *)&wmengCtr[3].wmeng,								WMENG_ADR+3*SIZE_WMENG, 				sizeof(wmengCtr[3].wmeng) 					},
	{ EEPADR,		(char *)&wmengCtr[3].verbr,								WMENG_MON_ADR+3*SIZE_WMENG, 		sizeof(wmengCtr[3].verbr)						},	
	{ EEPADR,		(char *)&wmengCtr[3].verbr_vj,						WMENG_VJMON_ADR+3*SIZE_WMENG, 	sizeof(wmengCtr[3].verbr_vj)				},	
	{ EEPADR,		(char *)&wmengCtr[3].flow_max,						FLOWMAX_ADR+3*SIZE_WMENG, 			sizeof(wmengCtr[3].flow_max)				},
	{ EEPADR,		(char *)&wmengCtr[3].wmLeistg_max,				LEISTMAX_ADR+3*SIZE_WMENG,			sizeof(wmengCtr[3].wmLeistg_max)		},
#endif
#endif

#if DIAG == 1	
/***** ulsch : Diagnose *****/			
//	{ EEPADR,		(char	*)&dis.begin.jahr,	DIAG_PARA,	sizeof(diagParam) },
	{ EEPADR,		(char	*)&dis,	DIAG_PARA,	sizeof( dis) },	
#endif
	
/***** ulsch : Heizgradtage *****/
#if HEIZGRD == 1	
	{EEPADR,	(char	*)&ta7,							TA7_ADR, 				2 },
	{EEPADR,	(char	*)&ta14,						TA14_ADR, 			2 },	
	{EEPADR,	(char	*)&indexMon,				INDEX_MON_ADR, 	1 },
	{EEPADR,	(char	*)&merkerStd,				MERKER_STD_ADR, 1 },			
#endif

	// Projekttyp
	{EEPADR,	(char	*)&proj_typ,				PROJ_TYP_ADR,		1},

//#####MBus
#if MBUSANZ > 0
	{EEPADR,	(char	*)&MbusPara[0],	    				MBUS_PARA_ADR,                    MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[0].zdim,				ZAEHLMB1_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[0].zwertMonAnf,	ZMB1_MONANF_ADR,												 4 },
#endif
#if MBUSANZ > 1
	{EEPADR,	(char	*)&MbusPara[1],	  					MBUS_PARA_ADR + MBUS_PARA_LENG,   MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[1].zdim,				ZAEHLMB2_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + ZMB_MONANF_OFFS,			 4 },
#endif
#if MBUSANZ > 2
	{EEPADR,	(char	*)&MbusPara[2],	  					MBUS_PARA_ADR + 2*MBUS_PARA_LENG, MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[2].zdim,				ZAEHLMB3_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + 2*ZMB_MONANF_OFFS,		 4 },
#endif
#if MBUSANZ > 3
	{EEPADR,	(char	*)&MbusPara[3],	  					MBUS_PARA_ADR + 3*MBUS_PARA_LENG, MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[3].zdim,				ZAEHLMB4_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + 2*ZMB_MONANF_OFFS,		 4 },
#endif
#if MBUSANZ > 4
	{EEPADR,	(char	*)&MbusPara[4],	  					MBUS_PARA_ADR + 4*MBUS_PARA_LENG, MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[4].zdim,				ZAEHLMB5_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + 2*ZMB_MONANF_OFFS,		 4 },
#endif
#if MBUSANZ > 5
	{EEPADR,	(char	*)&MbusPara[5],	  					MBUS_PARA_ADR + 5*MBUS_PARA_LENG, MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[5].zdim,				ZAEHLMB6_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + 2*ZMB_MONANF_OFFS,		 4 },
#endif
#if MBUSANZ > 6
	{EEPADR,	(char	*)&MbusPara[6],	  					MBUS_PARA_ADR + 6*MBUS_PARA_LENG, MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[6].zdim,				ZAEHLMB7_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + 2*ZMB_MONANF_OFFS,		 4 },
#endif
#if MBUSANZ > 7
	{EEPADR,	(char	*)&MbusPara[7],	  					MBUS_PARA_ADR + 7*MBUS_PARA_LENG, MBUSLENG },
	{EEPADR,	(char *)&mbZaehler[7].zdim,				ZAEHLMB8_DIM_ADR,												48 },
//-	{EEPADR,	(char *)&MbusData[1].zwertMonAnf,	ZMB1_MONANF_ADR + 2*ZMB_MONANF_OFFS,		 4 },
#endif
	// Zählerdimensionierung
	{EEPADR,	(char *)&zaehler[0].zdim,	ZAEHL01_DIM_ADR,		48 },	// ZAEHL01_DIM_ADR + ZAEHL01_NAME_ADR + ZAEHL01_NUMM_ADR
	{EEPADR,	(char *)&zaehler[1].zdim,	ZAEHL02_DIM_ADR,		48 },
	{EEPADR,	(char *)&zaehler[2].zdim,	ZAEHL03_DIM_ADR,		48 },
	{EEPADR,	(char *)&zaehler[3].zdim,	ZAEHL04_DIM_ADR,		48 },
	{EEPADR,	(char *)&zaehler[4].zdim,	ZAEHL05_DIM_ADR,		48 },
	{EEPADR,	(char *)&zaehler[5].zdim,	ZAEHL06_DIM_ADR,		48 },
	{EEPADR,	(char *)&zaehler[6].zdim,	ZAEHL07_DIM_ADR,		48 },
	{EEPADR,	(char *)&zaehler[7].zdim,	ZAEHL08_DIM_ADR,		48 },


	// Skalierungswerte für Analogausgabe R36		AA1-AA2
	{ EEPADR,	(char	*)&AASkal[0], 					AA_SKAL_ADR, 				8},


	//--------------- Pumpenbus ------------
#if ( GENI == 1 || WILO_MODBUS == 1 || GRUNDFOS_MODBUS == 1 )
	#if BUS_PU_MAX > 0
		{ EEPADR,	(char	*)&BusPuPara[0],	    				BUS_PU_PARA_ADR,                    BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 1
		{ EEPADR,	(char	*)&BusPuPara[1],	  					BUS_PU_PARA_ADR + BUS_PU_PARA_LENG,   BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 2
		{ EEPADR,	(char	*)&BusPuPara[2],	  					BUS_PU_PARA_ADR + 2*BUS_PU_PARA_LENG, BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 3
		{ EEPADR,	(char	*)&BusPuPara[3],	  					BUS_PU_PARA_ADR + 3*BUS_PU_PARA_LENG, BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 4
		{ EEPADR,	(char	*)&BusPuPara[4],	  					BUS_PU_PARA_ADR + 4*BUS_PU_PARA_LENG, BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 5
		{ EEPADR,	(char	*)&BusPuPara[5],	  					BUS_PU_PARA_ADR + 5*BUS_PU_PARA_LENG, BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 6
		{ EEPADR,	(char	*)&BusPuPara[6],	  					BUS_PU_PARA_ADR + 6*BUS_PU_PARA_LENG, BUS_PU_LENG },
	#endif
	#if BUS_PU_MAX > 7
		{ EEPADR,	(char	*)&BusPuPara[7],	  					BUS_PU_PARA_ADR + 7*BUS_PU_PARA_LENG, BUS_PU_LENG },
	#endif
#endif

	// -------------- Adressen für IN/OUT-Modul R37 -------------
	#if R37_MODULE > 0
	{EEPADR,	(char	*)&mod37[0].bus,				R37_1_BUS_ADR,			2},		// bus und adr stehen hintereinander
	// Skalierung
	{EEPADR,	(char	*)&mod37[0].AASkal[0],	R37_1_AA_SKAL_ADR, 16},		// AA1-AA4 
	// Zählerdimensionierung
	{EEPADR,	(char *)&mod37[0].zaehler[0].zdim,	ZAEHL11_DIM_ADR,	48 },	// ZAEHL11_DIM_ADR + ZAEHL11_NAME_ADR + ZAEHL11_NUMM_ADR
	{EEPADR,	(char *)&mod37[0].zaehler[1].zdim,	ZAEHL12_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[0].zaehler[2].zdim,	ZAEHL13_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[0].zaehler[3].zdim,	ZAEHL14_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[0].zaehler[4].zdim,	ZAEHL15_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[0].zaehler[5].zdim,	ZAEHL16_DIM_ADR,	48 },

	#endif

	#if R37_MODULE > 1
	{EEPADR,	(char	*)&mod37[1].bus,				R37_2_BUS_ADR,			2},
	// Skalierung
	{EEPADR,	(char	*)&mod37[1].AASkal[0],	R37_2_AA_SKAL_ADR, 16}, 
	// Zählerdimensionierung
	{EEPADR,	(char *)&mod37[1].zaehler[0].zdim,	ZAEHL21_DIM_ADR,	48 },	// ZAEHL21_DIM_ADR + ZAEHL21_NAME_ADR + ZAEHL21_NUMM_ADR
	{EEPADR,	(char *)&mod37[1].zaehler[1].zdim,	ZAEHL22_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[1].zaehler[2].zdim,	ZAEHL23_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[1].zaehler[3].zdim,	ZAEHL24_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[1].zaehler[4].zdim,	ZAEHL25_DIM_ADR,	48 },
	{EEPADR,	(char *)&mod37[1].zaehler[5].zdim,	ZAEHL26_DIM_ADR,	48 },

	#endif

	#if R37_MODULE > 2
	{EEPADR,	(char	*)&mod37[2].bus,				R37_3_BUS_ADR,			2},
	// Skalierung
	{EEPADR,	(char	*)&mod37[2].AASkal[0],	R37_3_AA_SKAL_ADR, 16}, 
	#endif

	#if R37_MODULE > 3
	{EEPADR,	(char	*)&mod37[3].bus,				R37_4_BUS_ADR,			2},
	// Skalierung
	{EEPADR,	(char	*)&mod37[3].AASkal[0],	R37_4_AA_SKAL_ADR, 16}, 
	#endif

	// --------------- Adressen für IN/OUT-Modul R38 ------------
	#if R38_MODULE > 0
	{EEPADR,	(char	*)&mod38[0].bus,		R38_1_BUS_ADR,	2},		// bus und adr stehen hintereinander
	{EEPADR,	(char	*)&mod38[0].life,		R38_1_LIFE_ADR,	2},		// 14 Bit, Bit0=1 -> Fühler IN 1 aktiviert usw.
	#endif

	#if R38_MODULE > 1
	{EEPADR,	(char	*)&mod38[1].bus,		R38_2_BUS_ADR,	2},
	{EEPADR,	(char	*)&mod38[1].life,		R38_2_LIFE_ADR,	2},
	#endif

	#if R38_MODULE > 2
	{EEPADR,	(char	*)&mod38[2].bus,		R38_3_BUS_ADR,	2},
	{EEPADR,	(char	*)&mod38[2].life,		R38_3_LIFE_ADR,	2},
	#endif

	#if R38_MODULE > 3
	{EEPADR,	(char	*)&mod38[3].bus,		R38_4_BUS_ADR,	2},
	{EEPADR,	(char	*)&mod38[3].life,		R38_4_LIFE_ADR,	2},
	#endif

	// ------------- Adressen für INP-Modul R39 ----------------
	#if R39_MODULE > 0
	{EEPADR,	(char	*)&mod39[0].bus,		R39_1_BUS_ADR,	2},		// bus und adr stehen hintereinander
	
	{EEPADR,	(char	*)&mod39[0].Digart,	R39_1_DIGART_ADR,	2},	// Signalart: Schließer(0) / Öffner(1)
	#endif

	#if R39_MODULE > 1
	{EEPADR,	(char	*)&mod39[1].bus,		R39_2_BUS_ADR,	2},

	{EEPADR,	(char	*)&mod39[1].Digart,	R39_2_DIGART_ADR,	2},	// Signalart: Schließer(0) / Öffner(1)
	#endif

	#if R39_MODULE > 2
	{EEPADR,	(char	*)&mod39[2].bus,		R39_3_BUS_ADR,	2},

	{EEPADR,	(char	*)&mod39[2].Digart,	R39_3_DIGART_ADR,	2},	// Signalart: Schließer(0) / Öffner(1)
	#endif

	#if R39_MODULE > 3
	{EEPADR,	(char	*)&mod39[3].bus,		R39_4_BUS_ADR,	2},

	{EEPADR,	(char	*)&mod39[3].Digart,	R39_4_DIGART_ADR,	2},	// Signalart: Schließer(0) / Öffner(1)
	#endif
	
	// -------------- Adressen für OUT-Modul R33 -------------
	#if R33_MODULE > 0
	{EEPADR,	(char	*)&mod33[0].bus,		R33_1_BUS_ADR,	2},		// bus und adr stehen hintereinander
	#endif

	#if R33_MODULE > 1
	{EEPADR,	(char	*)&mod33[1].bus,		R33_2_BUS_ADR,	2},
	#endif

	#if R33_MODULE > 2
	{EEPADR,	(char	*)&mod33[2].bus,		R33_3_BUS_ADR,	2},
	#endif

	#if R33_MODULE > 3
	{EEPADR,	(char	*)&mod33[3].bus,		R33_4_BUS_ADR,	2},
	#endif

/* ---------------------- Heizkreise ------------------------------*/

#if HKANZ > 0
	// Regler HK1
	{EEPADR,	(char	*)&hks[HK1],				HK1_ADR,				( ( HKSLENG > 64 ) ? 64 : HKSLENG )		},
	{EEPADR,	(char	*)&hks[HK1] + 64, 	HK1_ADR + 64,		( ( HKSLENG > 64 ) ? ( ( HKSLENG > 128 ) ? 64 : HKSLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&hks[HK1] + 128, 	HK1_ADR + 128,	( ( HKSLENG > 128 ) ? HKSLENG-128 : 0 )	},		
	
	// Pumpenlaufzeit
	{EEPADR,	(char	*)&hkd[HK1].puLz,		PU_HK_H_ADR			+ 0,					2 },	
	{EEPADR,	(char	*)&hkd[HK1].pudLz,	PUD_HK_H_ADR		+ 0,					2 },	
	// Raumnamen
	{EEPADR,	(char	*)&hkd[HK1].raumname,	RAUMNAME1_ADR	+ 0,					16 },	

	/* Absenktabelle	Regler HK1	*/
	// begin(1), begin_h(1), dauer(1), atemp(2), frei(3)
	{EEPADR,	(char	*)&abs_tab[HK1][0].begin,	ABS10_BEGIN_ADR,	8*ABSTABLENG}, // Alle Tage
#endif

#if HKANZ > 1	
	// Regler HK2
	{EEPADR,	(char	*)&hks[HK2],				HK2_ADR,				( ( HKSLENG > 64 ) ? 64 : HKSLENG )		},
	{EEPADR,	(char	*)&hks[HK2] + 64, 	HK2_ADR + 64,		( ( HKSLENG > 64 ) ? ( ( HKSLENG > 128 ) ? 64 : HKSLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&hks[HK2] + 128, 	HK2_ADR + 128,	( ( HKSLENG > 128 ) ? HKSLENG-128 : 0 )	},		
																													
	// Pumpenlaufzeit
	{EEPADR,	(char	*)&hkd[HK2].puLz,		PU_HK_H_ADR			+ 2,					2	},	
	{EEPADR,	(char	*)&hkd[HK2].pudLz,	PUD_HK_H_ADR		+ 2,					2	},	
	// Raumnamen
	{EEPADR,	(char	*)&hkd[HK2].raumname,	RAUMNAME1_ADR	+ 16,					16 },	

	/* Absenktabelle	Regler HK2	*/
	// begin(1), begin_h(1), dauer(1), atemp(2), frei(3)
	{EEPADR,	(char	*)&abs_tab[HK2][0].begin,	ABS20_BEGIN_ADR,	8*ABSTABLENG}, // Alle Tage
#endif

#if HKANZ > 2	
	// Regler HK3
	{EEPADR,	(char	*)&hks[HK3],				HK3_ADR,				( ( HKSLENG > 64 ) ? 64 : HKSLENG )		},
	{EEPADR,	(char	*)&hks[HK3] + 64, 	HK3_ADR + 64,		( ( HKSLENG > 64 ) ? ( ( HKSLENG > 128 ) ? 64 : HKSLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&hks[HK3] + 128, 	HK3_ADR + 128,	( ( HKSLENG > 128 ) ? HKSLENG-128 : 0 )	},		
																													
	// Pumpenlaufzeit
	{EEPADR,	(char	*)&hkd[HK3].puLz,		PU_HK_H_ADR			+ 4,					2	},	
	{EEPADR,	(char	*)&hkd[HK3].pudLz,	PUD_HK_H_ADR		+ 4,					2	},	
	// Raumnamen
	{EEPADR,	(char	*)&hkd[HK3].raumname,	RAUMNAME1_ADR	+ 32,					16 },	

	/* Absenktabelle	Regler HK3	*/
	// begin(1), begin_h(1), dauer(1), atemp(2), frei(3)
	{EEPADR,	(char	*)&abs_tab[HK3][0].begin,	ABS30_BEGIN_ADR,	8*ABSTABLENG}, // Alle Tage
#endif

#if HKANZ > 3	
	// Regler HK4
	{EEPADR,	(char	*)&hks[HK4],				HK4_ADR,				( ( HKSLENG > 64 ) ? 64 : HKSLENG )		},
	{EEPADR,	(char	*)&hks[HK4] + 64, 	HK4_ADR + 64,		( ( HKSLENG > 64 ) ? ( ( HKSLENG > 128 ) ? 64 : HKSLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&hks[HK4] + 128, 	HK4_ADR + 128,	( ( HKSLENG > 128 ) ? HKSLENG-128 : 0 )	},		
																													
	// Pumpenlaufzeit
	{EEPADR,	(char	*)&hkd[HK4].puLz,		PU_HK_H_ADR			+ 6,					2	},	
	{EEPADR,	(char	*)&hkd[HK4].pudLz,	PUD_HK_H_ADR		+ 6,					2	},	
	// Raumnamen
	{EEPADR,	(char	*)&hkd[HK4].raumname,	RAUMNAME1_ADR	+ 48,					16 },	

	/* Absenktabelle	Regler HK4	*/
	// begin(1), begin_h(1), dauer(1), atemp(2), frei(3)
	{EEPADR,	(char	*)&abs_tab[HK4][0].begin,	ABS40_BEGIN_ADR,	8*ABSTABLENG}, // Alle Tage
#endif


/* ------------------------ Kessel ------------------------------------*/

#if KEANZ > 0
	{EEPADR,	(char	*)&kes[KE1],					KE_ADR,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE1] + 64, 		KE_ADR + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE1] + 128, 		KE_ADR + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE1].kestarts,									KESTARTS_ADR	+ 0,					2},
	{EEPADR,	(char	*)&ked[KE1].kesh,											KESH_ADR			+ 0,					2},
	{EEPADR,	(char	*)&ked[KE1].kes2h,										KES2H_ADR			+ 0,					2},
#endif

#if KEANZ > 1
	{EEPADR,	(char	*)&kes[KE2],					KE_ADR + 1*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE2] + 64, 		KE_ADR + 1*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE2] + 128, 		KE_ADR + 1*KE_PAR_LENG + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE2].kestarts,									KESTARTS_ADR  + 2,					2},
	{EEPADR,	(char	*)&ked[KE2].kesh,											KESH_ADR 			+ 2,					2},
	{EEPADR,	(char	*)&ked[KE2].kes2h,										KES2H_ADR 		+ 2,					2},
#endif	

#if KEANZ > 2
	{EEPADR,	(char	*)&kes[KE3],					KE_ADR + 2*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE3] + 64, 		KE_ADR + 2*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE3] + 128, 		KE_ADR + 2*KE_PAR_LENG + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE3].kestarts,									KESTARTS_ADR  + 4,					2},
	{EEPADR,	(char	*)&ked[KE3].kesh,											KESH_ADR 			+ 4,					2},
	{EEPADR,	(char	*)&ked[KE3].kes2h,										KES2H_ADR 		+ 4,					2},
#endif	

#if KEANZ > 3
	{EEPADR,	(char	*)&kes[KE4],					KE_ADR + 3*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE4] + 64, 		KE_ADR + 3*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE4] + 128, 		KE_ADR + 3*KE_PAR_LENG + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE4].kestarts,									KESTARTS_ADR  + 6,					2},
	{EEPADR,	(char	*)&ked[KE4].kesh,											KESH_ADR 			+ 6,					2},
	{EEPADR,	(char	*)&ked[KE4].kes2h,										KES2H_ADR 		+ 6,					2},
#endif	

#if KEANZ > 4
	{EEPADR,	(char	*)&kes[KE5],					KE_ADR + 4*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE5] + 64, 		KE_ADR + 4*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE5] + 128, 		KE_ADR + 4*KE_PAR_LENG+ 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE5].kestarts,									KESTARTS_ADR  + 8,					2},
	{EEPADR,	(char	*)&ked[KE5].kesh,											KESH_ADR 			+ 8,					2},
	{EEPADR,	(char	*)&ked[KE5].kes2h,										KES2H_ADR 		+ 8,					2},
#endif	

#if KEANZ > 5
	{EEPADR,	(char	*)&kes[KE6],					KE_ADR + 5*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE6] + 64, 		KE_ADR + 5*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE6] + 128, 		KE_ADR + 5*KE_PAR_LENG + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE6].kestarts,									KESTARTS_ADR  + 10,					2},
	{EEPADR,	(char	*)&ked[KE6].kesh,											KESH_ADR 			+ 10,					2},
	{EEPADR,	(char	*)&ked[KE6].kes2h,										KES2H_ADR 		+ 10,					2},
#endif	

#if KEANZ > 6
	{EEPADR,	(char	*)&kes[KE7],					KE_ADR + 6*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE7] + 64, 		KE_ADR + 6*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE7] + 128, 		KE_ADR + 6*KE_PAR_LENG + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE7].kestarts,									KESTARTS_ADR  + 8,					2},
	{EEPADR,	(char	*)&ked[KE7].kesh,											KESH_ADR 			+ 8,					2},
	{EEPADR,	(char	*)&ked[KE7].kes2h,										KES2H_ADR 		+ 8,					2},
#endif	

#if KEANZ > 7
	{EEPADR,	(char	*)&kes[KE8],					KE_ADR + 7*KE_PAR_LENG,					( ( KESLENG > 64 ) ? 64 : KESLENG )		},
	{EEPADR,	(char	*)&kes[KE8] + 64, 		KE_ADR + 7*KE_PAR_LENG + 64,		( ( KESLENG > 64 ) ? ( ( KESLENG > 128 ) ? 64 : KESLENG-64 ) : 0 )	},		
	{EEPADR,	(char	*)&kes[KE8] + 128, 		KE_ADR + 7*KE_PAR_LENG + 128,		( ( KESLENG > 128 ) ? KESLENG-128 : 0 )	},		

	{EEPADR,	(char	*)&ked[KE8].kestarts,									KESTARTS_ADR  + 10,					2},
	{EEPADR,	(char	*)&ked[KE8].kesh,											KESH_ADR 			+ 10,					2},
	{EEPADR,	(char	*)&ked[KE8].kes2h,										KES2H_ADR 		+ 10,					2},
#endif	
	
/* ------------------------ Warmwasser ---------------------------------*/

#if WWANZ > 0	
	/* Warmwasser 1								*/
	{EEPADR,	(char	*)&wws[WW1],					WW1_ADR,				( ( WWSLENG > 64 ) ? 64 : WWSLENG )},
	{EEPADR,	(char	*)&wws[WW1] + 64,			WW1_ADR + 64,		( ( WWSLENG > 64 ) ? WWSLENG-64 : 0 )},

	{EEPADR,	(char	*)&wwd[WW1].puwlsta,	PUWLSTA1_ADR	+ 0,					2},
	{EEPADR,	(char	*)&wwd[WW1].puwlh,		PUWLH1_ADR		+ 0,					2},
	{EEPADR,	(char	*)&wwd[WW1].puwzh,		PUWZH1_ADR		+ 0,					2},
#endif
	
#if WWANZ > 1	
	/* Warmwasser 2								*/
	{EEPADR,	(char	*)&wws[WW2],					WW2_ADR,				( ( WWSLENG > 64 ) ? 64 : WWSLENG )},
	{EEPADR,	(char	*)&wws[WW2] + 64,			WW2_ADR + 64,		( ( WWSLENG > 64 ) ? WWSLENG-64 : 0 )},

	{EEPADR,	(char	*)&wwd[WW2].puwlsta,	PUWLSTA1_ADR	+ 2,					2},
	{EEPADR,	(char	*)&wwd[WW2].puwlh,		PUWLH1_ADR		+ 2,					2},
	{EEPADR,	(char	*)&wwd[WW2].puwzh,		PUWZH1_ADR		+ 2,					2},
#endif
	
/* ------------------------ Netztrennung -------------------------------*/

#if NTANZ > 0
	// Netztrennung 1
	{EEPADR,	(char	*)&nts[NT1],					NT1_ADR,				( ( NTSLENG > 64 ) ? 64 : NTSLENG )},
	{EEPADR,	(char	*)&nts[NT1] + 64,			NT1_ADR + 64,		( ( NTSLENG > 64 ) ? NTSLENG-64 : 0 )},

	{EEPADR,	(char	*)&ntd[NT1].puLz,			PU_NT_H_ADR		+ 0,					2 },	
	{EEPADR,	(char	*)&ntd[NT1].lzStd,		WTH_ADR	+ 0,								2 },	
#endif	

#if NTANZ > 1
	// Netztrennung 2
	{EEPADR,	(char	*)&nts[NT2],					NT2_ADR,				( ( NTSLENG > 64 ) ? 64 : NTSLENG )},
	{EEPADR,	(char	*)&nts[NT2] + 64,			NT2_ADR + 64,		( ( NTSLENG > 64 ) ? NTSLENG-64 : 0 )},

	{EEPADR,	(char	*)&ntd[NT2].puLz,			PU_NT_H_ADR		+ 2,					2 },	
	{EEPADR,	(char	*)&ntd[NT2].lzStd,		WTH_ADR	+ 2,								2 },	
#endif	

/* ------------------------ Solar-Regelung -----------------------------*/

// Solarregelung
#if SOLANZ > 0
	{EEPADR,	(char	*)&sos[SO1],					SO1_ADR,				( ( SOSLENG > 64 ) ? 64 : SOSLENG )},
	{EEPADR,	(char	*)&sos[SO1] + 64,			SO1_ADR + 64,		( ( SOSLENG > 64 ) ? SOSLENG-64 : 0 )},

	{EEPADR,	(char	*)&sod[SO1].lzKolPuH,	SOLH1_ADR			+ 0,					2},	
#endif

//#if SOLANZ > 1
//	{EEPADR,	(char	*)&sos[SO2],					SO2_ADR,				( ( SOSLENG > 64 ) ? 64 : SOSLENG )},
//	{EEPADR,	(char	*)&sos[SO2] + 64,			SO2_ADR + 64,		( ( SOSLENG > 64 ) ? SOSLENG-64 : 0 )},
//
//	{EEPADR,	(char	*)&sod[SO2].lzKolPuH,	SOLH1_ADR			+ 2,					2},	
//#endif

/* ------------------------ Benutzersteuerung mit UNI-Elementen --------*/
#if STEUER_UNI == 1
	{EEPADR,	(char	*)&unis[0],					STEUER_UNI_ADR,		UNISLENG},
#endif	

/*-------------------------WPU-Steuerung---------------------------------*/
#if WPANZ > 1
	{EEPADR,	(char	*)&wps[WP1],					WP1_ADR,				( ( WPSLENG > 64 ) ? 64 : WPSLENG )},
	{EEPADR,	(char	*)&wps[WP1] + 64,			WP1_ADR + 64,		( ( WPSLENG > 64 ) ? WPSLENG-64 : 0 )},
#endif

//----------------------------------------------------------------------------------------

// Funkempfang
#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
		{EEPADR,	(char	*)&ucReceiverTyp_RF,						RF_RECEIVER_EADR,		1, 1},	//090310 Typ: SRC-RS485-EVC = 1
		{EEPADR,	(char	*)&rf_feld[0].ulRf_modul_id,		RF_MODULID1_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[1].ulRf_modul_id,		RF_MODULID2_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[2].ulRf_modul_id,		RF_MODULID3_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[3].ulRf_modul_id,		RF_MODULID4_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[4].ulRf_modul_id,		RF_MODULID5_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[5].ulRf_modul_id,		RF_MODULID6_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[6].ulRf_modul_id,		RF_MODULID7_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[7].ulRf_modul_id,		RF_MODULID8_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[8].ulRf_modul_id,		RF_MODULID9_EADR,		4, 0},
		{EEPADR,	(char	*)&rf_feld[9].ulRf_modul_id,		RF_MODULID10_EADR,	4, 0},
		{EEPADR,	(char	*)&rf_feld[10].ulRf_modul_id,		RF_MODULID11_EADR,	4, 0},
		{EEPADR,	(char	*)&rf_feld[11].ulRf_modul_id,		RF_MODULID12_EADR,	4, 0},
		{EEPADR,	(char	*)&rf_feld[0].ucSetup,					RF_SETUP1_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[1].ucSetup,					RF_SETUP2_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[2].ucSetup,					RF_SETUP3_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[3].ucSetup,					RF_SETUP4_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[4].ucSetup,					RF_SETUP5_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[5].ucSetup,					RF_SETUP6_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[6].ucSetup,					RF_SETUP7_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[7].ucSetup,					RF_SETUP8_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[8].ucSetup,					RF_SETUP9_EADR,			1, 0},
		{EEPADR,	(char	*)&rf_feld[9].ucSetup,					RF_SETUP10_EADR,		1, 0},
		{EEPADR,	(char	*)&rf_feld[10].ucSetup,					RF_SETUP11_EADR,		1, 0},
		{EEPADR,	(char	*)&rf_feld[11].ucSetup,					RF_SETUP12_EADR,		1, 0},
#endif

// Modbus
#if MODBUS_UNI > 0
	#include "Modbus/modbusstandard.h"
#endif

// Änderung neue Genibus-Implementierung
#if GENI == 1
	{EEPADR,	(char	*)&genibus_device_table[0],	GENI_DEV1_ADR,		GENI_DEVICE_TABLE_SIZE},
#if BUS_PU_MAX > 1
	{EEPADR,	(char	*)&genibus_device_table[1],	GENI_DEV2_ADR,		GENI_DEVICE_TABLE_SIZE},
#endif
#if BUS_PU_MAX > 2
	{EEPADR,	(char	*)&genibus_device_table[2],	GENI_DEV3_ADR,		GENI_DEVICE_TABLE_SIZE},
#endif
#if BUS_PU_MAX > 3
	{EEPADR,	(char	*)&genibus_device_table[3],	GENI_DEV4_ADR,		GENI_DEVICE_TABLE_SIZE},	
#endif
#if BUS_PU_MAX > 4
	{EEPADR,	(char	*)&genibus_device_table[4],	GENI_DEV5_ADR,		GENI_DEVICE_TABLE_SIZE},
#endif
#if BUS_PU_MAX > 5
	{EEPADR,	(char	*)&genibus_device_table[5],	GENI_DEV6_ADR,		GENI_DEVICE_TABLE_SIZE},
#endif
#if BUS_PU_MAX > 6
	{EEPADR,	(char	*)&genibus_device_table[6],	GENI_DEV7_ADR,		GENI_DEVICE_TABLE_SIZE},
#endif
#if BUS_PU_MAX > 7
	{EEPADR,	(char	*)&genibus_device_table[7],	GENI_DEV8_ADR,		GENI_DEVICE_TABLE_SIZE},
#endif
	
	{EEPADR,	(char	*)&uc_genibus_own_adr,			GENI_OWN_ADR,			1},	
#endif

};
const char	BICUSER1Z = sizeof(bicuser1) / sizeof(bicvec);   // Anzahl der Einträge


		