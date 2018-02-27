/*------------------------------ Allgemeine Parameter --------------------------------------*/
const Pgrup zmb[] = {
//-------------------
	{"***:"," ZAEHLER M-BUS  ","       ", P&Leer15,									ASCII_FORM, 0, P&vis,		V0, 0, 0},

	{" 01:"," AKTUALISIERUNG "," min   ", P&MbusFrequ,	 								US_INT, 0, P&vis,		V1, 0, 0},
	{" ->;"," Aktualis.aktiv?","       ", P&MbusAktualisierung,		 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Aktiver Zaehler","       ", P&MbusCtrAktiv,		     			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Abfrage-Status ","       ", P&MbusStatus,		     			 	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Timeout-Counter"," 100ms ", P&Timeout,			 			 				US_INT, 0, P&hid2,	V0, 0, 0},
	{" 02:"," SOFORT AKTUAL.?"," JA=1  ", P&MbCtrAkt,	 		 						 US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->;"," Tx-Pointer     ","       ", P&pMbusTx,			 			 			 US_LONG, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Tx-Counter     ","       ", P&MbusTxCtr,			 			 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Tx-Laenge      ","       ", P&MbusTxLng,			 			 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Rx-Pointer     ","       ", P&pMbusRx,			 			 			 US_LONG, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Rx-Counter     ","       ", P&MbusRxCtr,			 			 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," Rx-Laenge      ","       ", P&MbusRxLng,			 			 		 US_CHAR, 0, P&hid2,	V0, 0, 0},

#if MBUS_MASTER == 1
	{"*->;"," letzt.stichtag ","       ", P&LetzterStichtag,	 	 		 DATE_FORM, 1, P&hid2,	V0, 0, 0},
#endif

	{" 03:"," AKTUAL.-ZEIT   "," h:min ", P&AktStd,		 					 		 ZEIT_FORM, 0, P&vis,		V1, 0, 0},
	{"*04:"," LETZTE AKTUAL. "," h:min ", P&LetzteAkt,		 					 ZEIT_FORM, 0, P&vis,		V1, 0, 0},

	// Expansionsgruppe ZMB
	{" 09:"," ZAEHLER        "," # 1-60", P&zaehlerNummer,				EXPAND_CHAR, 	0, P&vis,								V0, 0, EXP_ZMB},
	{"_10:"," ZAEHLERNAME    ","       ", P&MBZE[0], 								 ZE_FORMP,  1, P&vis,								V1, 0, EXP_ZMB},
                                                                                          						
	{" 11;"," TYP            ","       ", P&MbusPara[0],		 		 MBUS_TYP_FORM, 0, P&hid1,							V1, 0, EXP_ZMB},
	{" 12;"," FREIGABE       "," JA=1  ", P&MbusPara[0].Freigabe,	 		 US_CHAR, 0, P&hid1,							V1, 0, EXP_ZMB},
	{"*->;"," Tel.-Folge     ","       ", P&MbusPara[0].Folge,		     US_CHAR, 0, P&hid2,							V0, 0, EXP_ZMB},
	{" 13;"," MBUS ADRESSE   ","       ", P&MbusPara[0].Adr,		     	 US_CHAR, 0, P&hid1,							V1, 0, EXP_ZMB},

	{"*14:"," ENERGIE        "," kWh   ", P&MbusData[0].elektro,	 		 US_LONG, 0, P&Sichtbar.elt,			V1, 0, EXP_ZMB},

	{"*14:"," WAERME         ","       ", P&MbusData[0],	 		 MBUS_WMENG_FORM, 0, P&Sichtbar.wrm,			V1, 0, EXP_ZMB},
	{"*15:"," KAELTE         "," kWh   ", P&MbusData[0].kaelte,	 		   US_LONG, 0, P&Sichtbar.klm,			V1, 0, EXP_ZMB},
	{"*16:"," VOLUMEN        ","       ", P&MbusData[0],	 		 	 MBUS_VOL_FORM, 0, P&Sichtbar.vol,			V1, 0, EXP_ZMB},
	{" ->;"," komma vo-wm    "," 74-30 ", P&MbusData[0].komma,	 		 DIGR_FORM, 8, P&Sichtbar.wrm_h2, 	V0, 0, EXP_ZMB},	

	{"*17:"," TEMP.VORLAUF   "," C    ", P&MbusData[0].tVorlauf,				 S_INT, 1, P&Sichtbar.wrm,			V1, 0, EXP_ZMB},
	{"*18:"," TEMP.RUECKLAUF "," C    ", P&MbusData[0].tRuecklauf,			 S_INT, 1, P&Sichtbar.wrm,			V1, 0, EXP_ZMB},
	{"*19:"," TEMP.DIFFERENZ "," K     ", P&MbusData[0].tDifferenz,			 S_INT, 1, P&Sichtbar.wrm,			V1, 0, EXP_ZMB},
	{"*20:"," VOLUMENSTROM   "," m/h  ", P&MbusData[0].volStrom,			 US_LONG, 3, P&Sichtbar.was,			V1, 0, EXP_ZMB},
// Wärme-Leistung kann negativ sein (VL < RL ?)
//	{"*21:"," LEISTUNG       "," kW    ", P&MbusData[0].leistung,			 US_LONG, 3, P&Sichtbar.wrm,			V1, 0, EXP_ZMB},
	{"*21:"," LEISTUNG       "," kW    ", P&MbusData[0].leistung,	 MBUS_S_LONG, 3, P&Sichtbar.wrm,			V1, 0, EXP_ZMB},

	{"*21:"," LEISTUNG       "," kW    ", P&MbusData[0].leistung,	 MBUS_S_LONG, 1, P&Sichtbar.elt,			V1, 0, EXP_ZMB},

	{"*30:"," FEHLER         "," 7-0   ", P&MbusData[0].errors,	 		 DIGR_FORM, 8, P&hid1,							V1, 0, EXP_ZMB},
                                                                                          						
	{"*31;"," MONAT.VERBRAUCH","       ", P&MBZE[0],			 			  		ZE_FORMP, 5, P&hid1,							V0, 0, EXP_ZMB},
                                                                                          						
	{"*32;"," ZAEHLERWERT    ","       ", P&MBZE[0],			 						ZE_FORMP, 0, P&hid2,							V1, 0, EXP_ZMB},		// Leittechnik, Archivierung
	{" ->;"," KOMMASTELLEN   ","       ", P&MBZE[0],			 						ZE_FORMP, 2, P&hid1,							V0, 0, EXP_ZMB},
	{" ->;"," DIMENSION      ","       ", P&MBZE[0],			 						ZE_FORMP, 3, P&hid1,							V0, 0, EXP_ZMB},
	{"*->;"," letzter wert   ","       ", P&MbusData[0],			 MBUS_ZWERT_FORM, 0, P&hid2,							V0, 0, EXP_ZMB},
                                                                                          						
	{"*33;"," ZAEHLERNUMMER  ","       ", P&MBZE[0], 								 ZE_FORMP, 41, P&hid1,							V1, 0, EXP_ZMB},
                                                                                          						
	{"*34;"," STICHTAG       ","       ", P&MBZE[0],								 ZE_FORMP, 37, P&hid1,							V0, 0, EXP_ZMB},
	{"*35;"," ZAEHLER VORJAHR","       ", P&MBZE[0], 								 ZE_FORMP, 42, P&hid1,							V0, 0, EXP_ZMB},
#if MBUS_MASTER == 0                                                                      						
	{" 36;"," SYNC.VORJAHR   ","       ", P&MBZE[0], 								 ZE_FORMP, 43, P&hid3,							V0, 0, EXP_ZMB},
#else                                                                                     						
	{" 36;"," SYNC.VORJAHR   ","       ", P&MBZE[0], 	 MBUSMASTER_SYNCVJ_FORM,  0, P&hid3,							V0, 0, EXP_ZMB},
#endif                                                                                    						
	{"*->;"," zaehlerkonfig. "," 8...1 ", P&MBZE[0],								 ZE_FORMP, 34, P&hid2,							V0, 0, EXP_ZMB},
	{"*->;"," zaehlermedium  ","       ", P&MBZE[0],								 ZE_FORMP, 38, P&hid2,							V0, 0, EXP_ZMB},
                                                                                          						
                                                                                          						
	{"*41:"," JANUAR         ","       ", P&MBZE[0],			 					 ZE_FORMP, 10, P&vis,								V1, 0, EXP_ZMB},
	{"*42:"," FEBRUAR        ","       ", P&MBZE[0],			 					 ZE_FORMP, 11, P&vis,								V1, 0, EXP_ZMB},
	{"*43:"," MAERZ          ","       ", P&MBZE[0],			 					 ZE_FORMP, 12, P&vis,								V1, 0, EXP_ZMB},
	{"*44:"," APRIL          ","       ", P&MBZE[0],			 					 ZE_FORMP, 13, P&vis,								V1, 0, EXP_ZMB},
	{"*45:"," MAI            ","       ", P&MBZE[0],			 					 ZE_FORMP, 14, P&vis,								V1, 0, EXP_ZMB},
	{"*46:"," JUNI           ","       ", P&MBZE[0],			 					 ZE_FORMP, 15, P&vis,								V1, 0, EXP_ZMB},
	{"*47:"," JULI           ","       ", P&MBZE[0],			 					 ZE_FORMP, 16, P&vis,								V1, 0, EXP_ZMB},
	{"*48:"," AUGUST         ","       ", P&MBZE[0],			 					 ZE_FORMP, 17, P&vis,								V1, 0, EXP_ZMB},
	{"*49:"," SEPTEMBER      ","       ", P&MBZE[0],			 					 ZE_FORMP, 18, P&vis,								V1, 0, EXP_ZMB},
	{"*50:"," OKTOBER        ","       ", P&MBZE[0],			 					 ZE_FORMP, 19, P&vis,								V1, 0, EXP_ZMB},
	{"*51:"," NOVEMBER       ","       ", P&MBZE[0],			 					 ZE_FORMP, 20, P&vis,								V1, 0, EXP_ZMB},
	{"*52:"," DEZEMBER       ","       ", P&MBZE[0],			 					 ZE_FORMP, 21, P&vis,								V1, 0, EXP_ZMB},
	{"*61:"," VORJAHR JAN    ","       ", P&MBZE[0],			 					 ZE_FORMP, 22, P&vis,								V1, 0, EXP_ZMB},
	{"*62:"," VORJAHR FEB    ","       ", P&MBZE[0],			 					 ZE_FORMP, 23, P&vis,								V1, 0, EXP_ZMB},
	{"*63:"," VORJAHR MRZ    ","       ", P&MBZE[0],			 					 ZE_FORMP, 24, P&vis,								V1, 0, EXP_ZMB},
	{"*64:"," VORJAHR APR    ","       ", P&MBZE[0],			 					 ZE_FORMP, 25, P&vis,								V1, 0, EXP_ZMB},
	{"*65:"," VORJAHR MAI    ","       ", P&MBZE[0],			 					 ZE_FORMP, 26, P&vis,								V1, 0, EXP_ZMB},
	{"*66:"," VORJAHR JUNI   ","       ", P&MBZE[0],			 					 ZE_FORMP, 27, P&vis,								V1, 0, EXP_ZMB},
	{"*67:"," VORJAHR JULI   ","       ", P&MBZE[0],			 					 ZE_FORMP, 28, P&vis,								V1, 0, EXP_ZMB},
	{"*68:"," VORJAHR AUG    ","       ", P&MBZE[0],			 					 ZE_FORMP, 29, P&vis,								V1, 0, EXP_ZMB},
	{"*69:"," VORJAHR SEPT   ","       ", P&MBZE[0],			 					 ZE_FORMP, 30, P&vis,								V1, 0, EXP_ZMB},
	{"*70:"," VORJAHR OKT    ","       ", P&MBZE[0],			 					 ZE_FORMP, 31, P&vis,								V1, 0, EXP_ZMB},
	{"*71:"," VORJAHR NOV    ","       ", P&MBZE[0],			 					 ZE_FORMP, 32, P&vis,								V1, 0, EXP_ZMB},
	{"*72:"," VORJAHR DEZ    ","       ", P&MBZE[0],			 					 ZE_FORMP, 33, P&vis,								V1, 0, EXP_ZMB},
                                                                                          						
#if MBUS_MASTER == 0                                                                      						
	{" 80;"," SYNC.AKT.MONAT ","       ", P&MBZE[0],									ZE_FORMP, 7, P&hid3,							V0, 0, EXP_ZMB},
	{" 81;"," SYNC.MON.1-24  ","       ", P&MBZE[0],									ZE_FORMP, 9, P&hid3,							V0, 0, EXP_ZMB},
	{" 82;"," SYNC.MON.VERB  ","       ", P&MBZE[0],								 ZE_FORMP, 40, P&hid3,							V0, 0, EXP_ZMB},
#else                                                                                     						
	{" 81;"," SYNC.MON.1-24  ","       ", P&MBZE[0],									ZE_FORMP, 9, P&hid3,							V0, 0, EXP_ZMB},
	{" 82;"," SYNC.MON.VERBR.","       ", P&MBZE[0],	 MBUSMASTER_SYNCMON_FORM, 0, P&hid3,							V0, 0, EXP_ZMB},
#endif
};

 
// für Archivierung außerdem ändern: UserRam.c / uramext.h, Steuer.c, parli.h, U_Mbus.c
// 05.01.15: für Archivierung sollte nun auch der ZAEHLERWERT benutzt werden können
const Pgrup mba[] = {
//-------------------
#if MBUSANZ
	{"***:"," M-BUS ARCHIV   ","       ", P&Leer15,									ASCII_FORM, 0, P&hid4,		V1, 0, 0},

	{"*11:"," 01-WAERME      ","       ", P&MbusData[0].wmeng,	 		 	 US_LONG, 0, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!
	{"*12:"," 01-VOLUMEN     ","       ", P&MbusData[0].volume,	 		 	 US_LONG, 0, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!
	{"*13:"," 01-TEMP.VORLAUF"," C    ", P&MbusData[0].tVorlauf,				 S_INT, 1, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!
	{"*14:"," 01-TEMP.RUECKL."," C    ", P&MbusData[0].tRuecklauf,			 S_INT, 1, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!
	{"*15:"," 01-VOLUMENSTROM"," m/h  ", P&MbusData[0].volStrom,			 US_LONG, 3, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!
	{"*16:"," 01-LEISTUNG    "," kW    ", P&MbusData[0].leistung,			 US_LONG, 3, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!
	{"*17:"," 01-ZAEHLERWERT ","       ", P&MBZE[0],			 						ZE_FORMP, 0, P&hid4,		V1, 0, IDX0_FELD},	// System ab 19.11.14 !!!

#if MBUSANZ > 1
	{"*21:"," 02-WAERME      ","       ", P&MbusData[1].wmeng,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*22:"," 02-VOLUMEN     ","       ", P&MbusData[1].volume,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*23:"," 02-TEMP.VORLAUF"," C    ", P&MbusData[1].tVorlauf,				 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*24:"," 02-TEMP.RUECKL."," C    ", P&MbusData[1].tRuecklauf,			 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*25:"," 02-VOLUMENSTROM"," m/h  ", P&MbusData[1].volStrom,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*26:"," 02-LEISTUNG    "," kW    ", P&MbusData[1].leistung,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*27:"," 02-ZAEHLERWERT ","       ", P&MBZE[1],			 						ZE_FORMP, 0, P&hid4,		V1, 0, 0},
#endif

#if MBUSANZ > 2
	{"*31:"," 03-WAERME      ","       ", P&MbusData[2].wmeng,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*32:"," 03-VOLUMEN     ","       ", P&MbusData[2].volume,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*33:"," 03-TEMP.VORLAUF"," C    ", P&MbusData[2].tVorlauf,				 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*34:"," 03-TEMP.RUECKL."," C    ", P&MbusData[2].tRuecklauf,			 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*35:"," 03-VOLUMENSTROM"," m/h  ", P&MbusData[2].volStrom,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*36:"," 03-LEISTUNG    "," kW    ", P&MbusData[2].leistung,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*37:"," 03-ZAEHLERWERT ","       ", P&MBZE[2],			 						ZE_FORMP, 0, P&hid4,		V1, 0, 0},
#endif

#if MBUSANZ > 3
	{"*41:"," 04-WAERME      ","       ", P&MbusData[3].wmeng,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*42:"," 04-VOLUMEN     ","       ", P&MbusData[3].volume,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*43:"," 04-TEMP.VORLAUF"," C    ", P&MbusData[3].tVorlauf,				 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*44:"," 04-TEMP.RUECKL."," C    ", P&MbusData[3].tRuecklauf,			 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*45:"," 04-VOLUMENSTROM"," m/h  ", P&MbusData[3].volStrom,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*46:"," 04-LEISTUNG    "," kW    ", P&MbusData[3].leistung,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*47:"," 04-ZAEHLERWERT ","       ", P&MBZE[3],			 						ZE_FORMP, 0, P&hid4,		V1, 0, 0},
#endif

#if MBUSANZ > 4
	{"*51:"," 05-WAERME      ","       ", P&MbusData[4].wmeng,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*52:"," 05-VOLUMEN     ","       ", P&MbusData[4].volume,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*53:"," 05-TEMP.VORLAUF"," C    ", P&MbusData[4].tVorlauf,				 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*54:"," 05-TEMP.RUECKL."," C    ", P&MbusData[4].tRuecklauf,			 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*55:"," 05-VOLUMENSTROM"," m/h  ", P&MbusData[4].volStrom,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*56:"," 05-LEISTUNG    "," kW    ", P&MbusData[4].leistung,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*57:"," 05-ZAEHLERWERT ","       ", P&MBZE[4],			 						ZE_FORMP, 0, P&hid4,		V1, 0, 0},
#endif

#if MBUSANZ > 5
	{"*61:"," 06-WAERME      ","       ", P&MbusData[5].wmeng,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*62:"," 06-VOLUMEN     ","       ", P&MbusData[5].volume,	 		 	 US_LONG, 0, P&hid4,		V1, 0, 0},
	{"*63:"," 06-TEMP.VORLAUF"," C    ", P&MbusData[5].tVorlauf,				 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*64:"," 06-TEMP.RUECKL."," C    ", P&MbusData[5].tRuecklauf,			 S_INT, 1, P&hid4,		V1, 0, 0},
	{"*65:"," 06-VOLUMENSTROM"," m/h  ", P&MbusData[5].volStrom,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*66:"," 06-LEISTUNG    "," kW    ", P&MbusData[5].leistung,			 US_LONG, 3, P&hid4,		V1, 0, 0},
	{"*67:"," 06-ZAEHLERWERT ","       ", P&MBZE[5],			 						ZE_FORMP, 0, P&hid4,		V1, 0, 0},
#endif

#endif

};