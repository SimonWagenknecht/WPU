const Pgrup ze7[] = { // IN10
	{"_**:"," ZAEHLER IN10   ","       ", P&ZE[7],			 				ZE_FORMP, 1,	P&vis,	V1, 0, 0},
#if ZE10WM == 0
	{"*02:"," ZAEHLERWERT    ","       ", P&ZE[7],			 				ZE_FORMP, 0,	P&vis,	V1, 0, 0},
	{" 03;"," KOMMASTELLEN   ","       ", P&ZE[7],			 				ZE_FORMP, 2,	P&hid1,	V0, 0, 0},
	{" 04;"," DIMENSION      ","       ", P&ZE[7],			 				ZE_FORMP, 3,	P&hid1,	V0, 0, 0},
	{" 05;"," TEILERFAKTOR   ","       ", P&ZE[7],			 				ZE_FORMP, 4,	P&hid1,	V0, 0, 0},
	{" 06;"," IMPULSWERT     ","       ", P&ZE[7],							ZE_FORMP, 39,	P&hid1,	V0, 0, 0},
	{" ->;"," exponent       ","       ", P&ZE[7],							ZE_FORMP, 36,	P&hid2,	V0, 0, 0},
	{" 07;"," ZWISCHENZAEHLER","       ", P&ZE[7],							ZE_FORMP, 8,	P&hid1,	V0, 0, 0},
	{"*08;"," AKT.MON.VERBR. ","       ", P&ZE[7],			 				ZE_FORMP, 5,	P&hid1,	V0, 0, 0},
	{"*10:"," JANUAR         ","       ", P&ZE[7],			 				ZE_FORMP, 10, P&vis,	V1, 0, 0},
	{"*11:"," FEBRUAR        ","       ", P&ZE[7],			 				ZE_FORMP, 11, P&vis,	V1, 0, 0},
	{"*12:"," MAERZ          ","       ", P&ZE[7],			 				ZE_FORMP, 12, P&vis,	V1, 0, 0},
	{"*13:"," APRIL          ","       ", P&ZE[7],			 				ZE_FORMP, 13, P&vis,	V1, 0, 0},
	{"*14:"," MAI            ","       ", P&ZE[7],			 				ZE_FORMP, 14, P&vis,	V1, 0, 0},
	{"*15:"," JUNI           ","       ", P&ZE[7],			 				ZE_FORMP, 15, P&vis,	V1, 0, 0},
	{"*16:"," JULI           ","       ", P&ZE[7],			 				ZE_FORMP, 16, P&vis,	V1, 0, 0},
	{"*17:"," AUGUST         ","       ", P&ZE[7],			 				ZE_FORMP, 17, P&vis,	V1, 0, 0},
	{"*18:"," SEPTEMBER      ","       ", P&ZE[7],			 				ZE_FORMP, 18, P&vis,	V1, 0, 0},
	{"*19:"," OKTOBER        ","       ", P&ZE[7],			 				ZE_FORMP, 19, P&vis,	V1, 0, 0},
	{"*20:"," NOVEMBER       ","       ", P&ZE[7],			 				ZE_FORMP, 20, P&vis,	V1, 0, 0},
	{"*21:"," DEZEMBER       ","       ", P&ZE[7],			 				ZE_FORMP, 21, P&vis,	V1, 0, 0},
	{"*22:"," VORJAHR JANUAR ","       ", P&ZE[7],			 				ZE_FORMP, 22, P&vis,	V1, 0, 0},
	{"*23:"," VORJAHR FEBRUAR","       ", P&ZE[7],			 				ZE_FORMP, 23, P&vis,	V1, 0, 0},
	{"*24:"," VORJAHR MAERZ  ","       ", P&ZE[7],			 				ZE_FORMP, 24, P&vis,	V1, 0, 0},
	{"*25:"," VORJAHR APRIL  ","       ", P&ZE[7],			 				ZE_FORMP, 25, P&vis,	V1, 0, 0},
	{"*26:"," VORJAHR MAI    ","       ", P&ZE[7],			 				ZE_FORMP, 26, P&vis,	V1, 0, 0},
	{"*27:"," VORJAHR JUNI   ","       ", P&ZE[7],			 				ZE_FORMP, 27, P&vis,	V1, 0, 0},
	{"*28:"," VORJAHR JULI   ","       ", P&ZE[7],			 				ZE_FORMP, 28, P&vis,	V1, 0, 0},
	{"*29:"," VORJAHR AUGUST ","       ", P&ZE[7],			 				ZE_FORMP, 29, P&vis,	V1, 0, 0},
	{"*30:"," VORJAHR SEPT.  ","       ", P&ZE[7],			 				ZE_FORMP, 30, P&vis,	V1, 0, 0},
	{"*31:"," VORJAHR OKTOBER","       ", P&ZE[7],			 				ZE_FORMP, 31, P&vis,	V1, 0, 0},
	{"*32:"," VORJAHR NOVEMB.","       ", P&ZE[7],			 				ZE_FORMP, 32, P&vis,	V1, 0, 0},
	{"*33:"," VORJAHR DEZEMB.","       ", P&ZE[7],			 				ZE_FORMP, 33, P&vis,	V1, 0, 0},
	{" 34:"," SYNC.ZAEHLWERT ","       ", P&ZE[7],							ZE_FORMP, 6,  P&hid3,	V0, 0, 0},
	{" 35:"," SYNC.AKT.MONAT ","       ", P&ZE[7],							ZE_FORMP, 7,  P&hid3,	V0, 0, 0},
	{" 36:"," SYNC.MONAT 1-24","       ", P&ZE[7],							ZE_FORMP, 9,  P&hid3,	V0, 0, 0},
	{" 37:"," SYNC.MON.VERBR.","       ", P&ZE[7],							ZE_FORMP, 40, P&hid3,	V0, 0, 0},
	{" 38;"," ZAEHLERKONFIG. "," 8...1 ", P&ZE[7],							ZE_FORMP, 34,	P&hid1,	V0, 0, 0},
	{"*39;"," ZAEHLERMEDIUM  ","       ", P&ZE[7],							ZE_FORMP, 38,	P&hid1,	V0, 0, 0},
	{"*40;"," FEHLERSTATUS   "," 8...1 ", P&ZE[7],							ZE_FORMP, 35,	P&hid1,	V0, 0, 0},
	{"_41;"," ZAEHLERNUMMER  ","       ", P&ZE[7], 							ZE_FORMP, 41,	P&hid1,	V1, 0, 0},
	{"*42;"," STICHTAG       ","       ", P&ZE[7],							ZE_FORMP, 37,	P&hid1,	V0, 0, 0},
	{"*43;"," ZAEHLER VORJAHR","       ", P&ZE[7], 							ZE_FORMP, 42,	P&hid1,	V0, 0, 0},
	{" 44;"," SYNC.ZAEHL.VORJ","       ", P&ZE[7], 							ZE_FORMP, 43,	P&hid3,	V0, 0, 0},
 
	#if LEIST_BER_ZE10 == 1
	{" 45;"," MAX LOESCHEN   "," JA=1  ", P&MaxLeistung[3].resMax,					US_CHAR,	0, P&hid1, V1, 0, 0},    
	{" 50;"," LEISTG.MITTELN "," min   ", P&MaxLeistung[3].MittelnLeistg, 	US_CHAR,	0, P&hid1, V1, 0, 0}, 
	{"*51:"," LEI.MITTEL MAX "," kW    ", P&MaxLeistung[3].mittelMax,				US_INT,		0, P&vis,  V1, 0, 0},
	{"*52:"," LEISTG.MITTEL  "," kW    ", P&MaxLeistung[3].mittel,					US_INT,		0, P&vis,  V1, 0, 0}, 
	{"*53;"," DAT.LEI.MI.MAX ","       ", P&MaxLeistung[3].tag, 		 				DATE_FORM,1, P&hid1, V1, 0, 0},
	{"*54;"," ZEIT LEI.MI.MAX"," h:min ", P&MaxLeistung[3].zeit,		 				ZEIT_FORM,0, P&hid1, V1, 0, 0},
	{"*55;"," Ta LEI.MI.MAX  "," C    ", P&MaxLeistung[3].ta,							ANA_FORM, 1, P&hid1, V1, 0, 0},
	{"*56;"," VL LEI.MI.MAX  "," C    ", P&MaxLeistung[3].vl,							ANA_FORM, 1, P&hid1, V1, 0, 0},
	{"*57;"," RL LEI.MI.MAX  "," C    ", P&MaxLeistung[3].rl,							ANA_FORM, 1, P&hid1, V1, 0, 0},
	#if LEIST_BEGR_ZE10 == 1
//-	{" 60:"," GRENZ-LEISTUNG "," kW    ", P&GrenzLeistung.GrenzLeistg,			US_INT,		0, P&vis,  V1, 0, 0}, 
//-	{" 61;"," HYSTERESE      "," kW    ", P&GrenzLeistung.LeistgHyst,				US_INT,		0, P&vis,  V1, 0, 0}, 
	{"*62:"," LEISTUNG MOMENT"," kW    ", P&grenzLeistung.leistung,					US_INT,		0, P&vis,  V1, 0, 0}, 
//	{" 63:"," DAEMPFUNG      ","       ", P&GrenzLeistung.Daempfung,				US_INT,		0, P&vis,  V1, 0, 0}, 
//-	{"*64:"," LEISTUNG BEGR. "," kW    ", P&grenzLeistung.leistBegr,				US_INT,		0, P&vis,  V1, 0, 0}, 
//-	{"*65:"," BEGRENZUNG EIN?","       ", P&grenzLeistung.begrenzung,	 	 JANEIN_FORM, 0, P&vis,	 V1, 0, 0},
//-	{"*66;"," BEGRENZGS-STUFE","       ", P&grenzLeistung.begrenzung,	 			US_CHAR, 	0, P&hid1, V1, 0, 0},
//-	{" 67;"," BEGR.AKT.LEIST."," JA=1  ", P&GrenzLeistung.BegrAkt,				  US_CHAR,	0, P&hid1, V1, 0, 0}, 
	{" ->;"," Impuls-Abstand "," s     ", P&grenzLeistung.msec,							US_INT,		1, P&hid2, V0, 0, 0}, 
	{" ->;"," alter Imp-Absta"," s     ", P&grenzLeistung.altMsec,					US_INT,		1, P&hid2, V0, 0, 0}, 
	#endif
	#endif
#endif

#if ZE10WM == 1
	{"*02:"," WAERMEMENGE    "," kWh   ", P&wmengCtr[3].wmeng,	 US_LONG, 0,  P&vis,	V1, 0, 0},
	{"*->:"," VOLUMEN        ","       ", P&ZE[7],			 				ZE_FORMP, 0,  P&vis,	V1, 0, 0},
	{"*->;"," TEMP.VL TVFWs  "," C    ", P&wmengCtr[3].pVl,	 ANA_FORMP, 1,  P&hid1,	V0, 0, 0},
	{"*->;"," TEMP.RL TRH    "," C    ", P&wmengCtr[3].pRl,	 ANA_FORMP, 1,  P&hid1,	V0, 0, 0},
	{" 03;"," KOMMASTELLEN   ","       ", P&ZE[7],							ZE_FORMP, 2,  P&hid1,	V0, 0, 0},
	{" 04;"," DIMENSION      ","       ", P&ZE[7],							ZE_FORMP, 3,  P&hid1,	V0, 0, 0},
	{" 05;"," TEILERFAKTOR   ","       ", P&ZE[7],							ZE_FORMP, 4,  P&hid1,	V0, 0, 0},
	{" 06;"," IMPULSWERT     ","       ", P&ZE[7],							ZE_FORMP, 39, P&hid1,	V0, 0, 0},
	{" ->;"," exponent       ","       ", P&ZE[7],							ZE_FORMP, 36, P&hid2,	V0, 0, 0},
	{" 07;"," ZWISCHENZAEHLER","       ", P&ZE[7],							ZE_FORMP, 8,  P&hid1,	V0, 0, 0},
	{"*08;"," AKT.MON.VOLUM. ","       ", P&ZE[7],							ZE_FORMP, 5,  P&hid1,	V0, 0, 0},

	{"*10:"," JANUAR         "," kWh   ", P&wmengCtr[3].verbr[0],						US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*11:"," FEBRUAR        "," kWh   ", P&wmengCtr[3].verbr[1],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*12:"," MAERZ          "," kWh   ", P&wmengCtr[3].verbr[2],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*13:"," APRIL          "," kWh   ", P&wmengCtr[3].verbr[3],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*14:"," MAI            "," kWh   ", P&wmengCtr[3].verbr[4],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*15:"," JUNI           "," kWh   ", P&wmengCtr[3].verbr[5],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*16:"," JULI           "," kWh   ", P&wmengCtr[3].verbr[6],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*17:"," AUGUST         "," kWh   ", P&wmengCtr[3].verbr[7],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*18:"," SEPTEMBER      "," kWh   ", P&wmengCtr[3].verbr[8],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*19:"," OKTOBER        "," kWh   ", P&wmengCtr[3].verbr[9],				  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*20:"," NOVEMBER       "," kWh   ", P&wmengCtr[3].verbr[10],			  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*21:"," DEZEMBER       "," kWh   ", P&wmengCtr[3].verbr[11],			  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*22:"," VORJAHR JANUAR "," kWh   ", P&wmengCtr[3].verbr_vj[0],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*23:"," VORJAHR FEBRUAR"," kWh   ", P&wmengCtr[3].verbr_vj[1],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*24:"," VORJAHR MAERZ  "," kWh   ", P&wmengCtr[3].verbr_vj[2],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*25:"," VORJAHR APRIL  "," kWh   ", P&wmengCtr[3].verbr_vj[3],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*26:"," VORJAHR MAI    "," kWh   ", P&wmengCtr[3].verbr_vj[4],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*27:"," VORJAHR JUNI   "," kWh   ", P&wmengCtr[3].verbr_vj[5],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*28:"," VORJAHR JULI   "," kWh   ", P&wmengCtr[3].verbr_vj[6],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*29:"," VORJAHR AUGUST "," kWh   ", P&wmengCtr[3].verbr_vj[7],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*30:"," VORJAHR SEPT.  "," kWh   ", P&wmengCtr[3].verbr_vj[8],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*31:"," VORJAHR OKTOBER"," kWh   ", P&wmengCtr[3].verbr_vj[9],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*32:"," VORJAHR NOVEMB."," kWh   ", P&wmengCtr[3].verbr_vj[10],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{"*33:"," VORJAHR DEZEMB."," kWh   ", P&wmengCtr[3].verbr_vj[11],		  	US_LONG, 	0, P&vis,  V1, 0 ,0},
	{" 34;"," SYNC.WAERMEMENG"," kWh   ", P&wmengCtr[3].wmeng,							US_LONG,	0, P&hid3, V0, 0, 0},
	{" ->;"," SYNC.JAN       "," kWh   ", P&wmengCtr[3].verbr[0],						US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.FEB       "," kWh   ", P&wmengCtr[3].verbr[1],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.MRZ       "," kWh   ", P&wmengCtr[3].verbr[2],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.APR       "," kWh   ", P&wmengCtr[3].verbr[3],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.MAI       "," kWh   ", P&wmengCtr[3].verbr[4],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.JUN       "," kWh   ", P&wmengCtr[3].verbr[5],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.JUL       "," kWh   ", P&wmengCtr[3].verbr[6],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.AUG       "," kWh   ", P&wmengCtr[3].verbr[7],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.SEP       "," kWh   ", P&wmengCtr[3].verbr[8],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.OKT       "," kWh   ", P&wmengCtr[3].verbr[9],				  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.NOV       "," kWh   ", P&wmengCtr[3].verbr[10],			  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.DEZ       "," kWh   ", P&wmengCtr[3].verbr[11],			  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.JAN  "," kWh   ", P&wmengCtr[3].verbr_vj[0],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.FEB  "," kWh   ", P&wmengCtr[3].verbr_vj[1],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.MRZ  "," kWh   ", P&wmengCtr[3].verbr_vj[2],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.APR  "," kWh   ", P&wmengCtr[3].verbr_vj[3],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.MAI  "," kWh   ", P&wmengCtr[3].verbr_vj[4],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.JUN  "," kWh   ", P&wmengCtr[3].verbr_vj[5],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.JUL  "," kWh   ", P&wmengCtr[3].verbr_vj[6],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.AUG  "," kWh   ", P&wmengCtr[3].verbr_vj[7],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.SEP  "," kWh   ", P&wmengCtr[3].verbr_vj[8],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.OKT  "," kWh   ", P&wmengCtr[3].verbr_vj[9],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.NOV  "," kWh   ", P&wmengCtr[3].verbr_vj[10],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" ->;"," SYNC.VORJ.DEZ  "," kWh   ", P&wmengCtr[3].verbr_vj[11],		  	US_LONG, 	0, P&hid3, V0, 0 ,0},
	{" 35;"," WMZ SYNCHRONIS."," JA=1  ", P&wmengCtr[3].syncFlag,						US_CHAR,	0, P&hid3, V0, 0, 0},   
	
	{" 45;"," MAX LOESCHEN   "," JA=1  ", P&wmengCtr[3].resMax,							US_CHAR,	0, P&hid1, V1, 0, 0},    
	{"*46:"," VOLUMENSTROM   "," m/h  ", P&wmengCtr[3].flow,								US_LONG,	1, P&vis,	 V1, 0, 0},
	{"*47:"," LEISTUNG       "," kW    ", P&wmengCtr[3].wmLeistg,						US_INT,		0, P&vis,	 V1, 0, 0}, 
	{"*48;"," VOLUMENSTR.MAX "," m/h  ", P&wmengCtr[3].flow_max,						US_LONG,	1, P&hid1, V1, 0, 0}, 
	{"*49;"," LEISTUNG MAX   "," kW    ", P&wmengCtr[3].wmLeistg_max,				US_INT,		0, P&hid1, V1, 0, 0},
 
	#if LEIST_BER_ZE10 == 1
	{" 50;"," LEISTG.MITTELN "," min   ", P&MaxLeistung[3].MittelnLeistg, 	US_CHAR,	0, P&hid1, V1, 0, 0}, 
	{"*51:"," LEI.MITTEL MAX "," kW    ", P&MaxLeistung[3].mittelMax,				US_INT,		0, P&vis,  V1, 0, 0},
	{"*52;"," LEISTG.MITTEL  "," kW    ", P&MaxLeistung[3].mittel,					US_INT,		0, P&hid1, V1, 0, 0}, 
	{"*53;"," DAT.LEI.MI.MAX ","       ", P&MaxLeistung[3].tag, 		 				DATE_FORM,1, P&hid1, V1, 0, 0},
	{"*54;"," ZEIT LEI.MI.MAX"," h:min ", P&MaxLeistung[3].zeit,		 				ZEIT_FORM,0, P&hid1, V1, 0, 0},
	{"*55;"," Ta LEI.MI.MAX  "," C    ", P&MaxLeistung[3].ta,							ANA_FORM, 1, P&hid1, V1, 0, 0},
	{"*56;"," VL LEI.MI.MAX  "," C    ", P&MaxLeistung[3].vl,							ANA_FORM, 1, P&hid1, V1, 0, 0},
	{"*57;"," RL LEI.MI.MAX  "," C    ", P&MaxLeistung[3].rl,							ANA_FORM, 1, P&hid1, V1, 0, 0},
	{"*58;"," VSTR.LEI.MI.MAX"," m/h  ", P&MaxLeistung[3].flow,						US_LONG,	1, P&hid1, V1, 0, 0},
	#endif

	{" ->;"," vol.letzter imp"," l     ", P&wmengCtr[3].volumen,						US_INT,	  0, P&hid2, V0, 0, 0},
	{" ->;"," zeit seit imp. "," 200ms ", P&wmengCtr[3].zeitdiff,						US_INT,	  0, P&hid2, V0, 0, 0},
	{" ->;"," zeit letzt.imp."," 200ms ", P&wmengCtr[3].zeitdiffZuletzt,		US_INT,	  0, P&hid2, V0, 0, 0},

	{"*60;"," januar         ","       ", P&ZE[7],			 			ZE_FORMP, 10, P&hid2,	V0, 0, 0},
	{"*61;"," februar        ","       ", P&ZE[7],			 			ZE_FORMP, 11, P&hid2,	V0, 0, 0},
	{"*62;"," maerz          ","       ", P&ZE[7],			 			ZE_FORMP, 12, P&hid2,	V0, 0, 0},
	{"*63;"," april          ","       ", P&ZE[7],			 			ZE_FORMP, 13, P&hid2,	V0, 0, 0},
	{"*64;"," mai            ","       ", P&ZE[7],			 			ZE_FORMP, 14, P&hid2,	V0, 0, 0},
	{"*65;"," juni           ","       ", P&ZE[7],			 			ZE_FORMP, 15, P&hid2,	V0, 0, 0},
	{"*66;"," juli           ","       ", P&ZE[7],			 			ZE_FORMP, 16, P&hid2,	V0, 0, 0},
	{"*67;"," august         ","       ", P&ZE[7],			 			ZE_FORMP, 17, P&hid2,	V0, 0, 0},
	{"*68;"," september      ","       ", P&ZE[7],			 			ZE_FORMP, 18, P&hid2,	V0, 0, 0},
	{"*69;"," oktober        ","       ", P&ZE[7],			 			ZE_FORMP, 19, P&hid2,	V0, 0, 0},
	{"*70;"," november       ","       ", P&ZE[7],			 			ZE_FORMP, 20, P&hid2,	V0, 0, 0},
	{"*71;"," dezember       ","       ", P&ZE[7],			 			ZE_FORMP, 21, P&hid2,	V0, 0, 0},
	{"*72;"," vorjahr januar ","       ", P&ZE[7],			 			ZE_FORMP, 22, P&hid2,	V0, 0, 0},
	{"*73;"," vorjahr februar","       ", P&ZE[7],			 			ZE_FORMP, 23, P&hid2,	V0, 0, 0},
	{"*74;"," vorjahr maerz  ","       ", P&ZE[7],			 			ZE_FORMP, 24, P&hid2,	V0, 0, 0},
	{"*75;"," vorjahr april  ","       ", P&ZE[7],			 			ZE_FORMP, 25, P&hid2,	V0, 0, 0},
	{"*76;"," vorjahr mai    ","       ", P&ZE[7],			 			ZE_FORMP, 26, P&hid2,	V0, 0, 0},
	{"*77;"," vorjahr juni   ","       ", P&ZE[7],			 			ZE_FORMP, 27, P&hid2,	V0, 0, 0},
	{"*78;"," vorjahr juli   ","       ", P&ZE[7],			 			ZE_FORMP, 28, P&hid2,	V0, 0, 0},
	{"*79;"," vorjahr august ","       ", P&ZE[7],			 			ZE_FORMP, 29, P&hid2,	V0, 0, 0},
	{"*80;"," vorjahr sept.  ","       ", P&ZE[7],			 			ZE_FORMP, 30, P&hid2,	V0, 0, 0},
	{"*81;"," vorjahr oktober","       ", P&ZE[7],			 			ZE_FORMP, 31, P&hid2,	V0, 0, 0},
	{"*82;"," vorjahr novemb.","       ", P&ZE[7],			 			ZE_FORMP, 32, P&hid2,	V0, 0, 0},
	{"*83;"," vorjahr dezemb.","       ", P&ZE[7],			 			ZE_FORMP, 33, P&hid2,	V0, 0, 0},
	{" 85:"," sync.zaehlwert ","       ", P&ZE[7],						ZE_FORMP, 6,  P&hid3,	V0, 0, 0},
	{" 86:"," sync.akt.monat ","       ", P&ZE[7],						ZE_FORMP, 7,  P&hid3,	V0, 0, 0},
	{" 87:"," sync.monat 1-24","       ", P&ZE[7],						ZE_FORMP, 9,  P&hid3,	V0, 0, 0},
	{" 88:"," sync.mon.verbr.","       ", P&ZE[7],						ZE_FORMP, 40, P&hid3,	V0, 0, 0},

#endif
 
};
