#if KEANZ == 1
const Pgrup ke1[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[0],					ASCII_FORM, 0, P&vis,		V1, 0, 0},
		
	#if TV_KE1
	{"*01:"," VORLAUF KESSEL "," C    ", P&TVKE[KE1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*01:"," VORLAUF KESSEL "," C    ", P&TVKE[KE1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TR_KE1
	{"*02:"," RUECKL.KESSEL  "," C    ", P&TRKE[KE1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*02:"," RUECKL.KESSEL  "," C    ", P&TRKE[KE1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TVG_KE1
	{"*03:"," VORL.STRATEGIE "," C    ", P&TVGKE[KE1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*03:"," VORL.STRATEGIE "," C    ", P&TVGKE[KE1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TRG_KE1
	{"*04:"," RUECKL.GESAMT  "," C    ", P&TRGKE[KE1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*04:"," RUECKL.GESAMT  "," C    ", P&TRGKE[KE1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	{" 11:"," SOLLTEMP. MAX  "," C    ", P&kes[KE1].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 11:"," SOLLTEMP. MIN  "," C    ", P&kes[KE1].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	#if ( MODUL_KE1 || STUFIG_KE1 )
	{" 12:"," VORLAUF MAX    "," C    ", P&kes[KE1].TvMax,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 13:"," SCHALTDIFFERENZ"," K     ", P&kes[KE1].KesSchaltDiff,	US_INT, 1, P&vis,		V1, 0, 0},	
	#endif

	#if MODUL_KE1 == 2
	{" 14;"," SKAL.MODULAT.0%"," V     ", P&kes[KE1].ModMin,				US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE1
	{" 15;"," MOD.-VERZ.SOMM."," min   ", P&kes[KE1].ModVerzSomm,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 15;"," MOD.-VERZ.WINT."," min   ", P&kes[KE1].ModVerzWint,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," mod.-verz.cnt  "," s     ", P&ked[KE1].modVerz,	 			US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#if SOLLWERT_KE1	
	{" 16;"," SKAL.TEMP.MIN  "," C    ", P&kes[KE1].TvsolX1,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 16;"," SKAL.SPG.MIN   "," V     ", P&kes[KE1].SollY1,			   S_INT, 1, P&hid1,	V1, 0, 0},
	{" 17;"," SKAL.TEMP.MAX  "," C    ", P&kes[KE1].TvsolX2,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 17;"," SKAL.SPG.MAX   "," V     ", P&kes[KE1].SollY2,			   S_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if ( MODUL_KE1 && SOLLWERT_KE1 )
	{" 18;"," MODUL.LEISTUNG "," 1=JA  ", P&kes[KE1].Modulation,	  US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif
		
	#if STUFIG_KE1 == 2		
	{" 19;"," EIN STUFE 2    "," K*min ", P&kes[KE1].Einkrit2,			 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 19;"," AUS STUFE 2    "," K*min ", P&kes[KE1].Auskrit2,			 S_INT, 1, P&hid1,	V1, 0, 0},	
	#endif
		
	{"*20:"," SOLLTEMPERATUR "," C    ", P&ked[KE1].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},

	{" 21:"," ANHEBUNG KONST."," K     ", P&kes[KE1].Tvpa,					US_INT, 1, P&vis,		V1, 0, 0},
	#if SW_ANH_VAR
	{"*21:"," ANHEBUNG VARIAB"," K     ", P&ked[KE1].y_rel_anh,			 S_INT, 1, P&vis,		V1, 0, 0},	
	{" 22;"," ANHEB.VAR.MAX  "," K     ", P&kes[KE1].AnhebMax,			US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if SW_ANH_VAR
	{" 30;"," P-VERST.VL-ANH."," %/K   ", P&kes[KE1].Kp_anh,				US_INT, 2, P&hid1,	V1, 0, 0},
	{" 31;"," D-VERST.VL-ANH.","       ", P&kes[KE1].Kd_anh,				US_INT, 2, P&hid1,	V1, 0, 0},
	{" 32;"," TASTZEIT VL-ANH"," s     ", P&kes[KE1].Ts_anh,				US_INT, 1, P&hid1,	V1, 0, 0},
	{" 33;"," NACHSTELLZ.ANH."," s     ", P&kes[KE1].Tn_anh,				US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if RL_KE1												// RL-Begrenzung/Anhebung
	{" 38;"," RL BEGR/ANHEB  "," 0/1   ", P&kes[KE1].RlAnheb, 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 39:"," TAUPUNKT-TEMP. "," C    ", P&kes[KE1].Ttaup,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 40;"," P-VERST.RL     "," %/K   ", P&kes[KE1].Kp_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 41;"," D-VERST.RL     ","       ", P&kes[KE1].Kd_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 42;"," TASTZEIT RL    "," s     ", P&kes[KE1].Ts_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	{" 43;"," NACHSTELLZ.RL  "," s     ", P&kes[KE1].Tn_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE1 == 1
	{" 44;"," STELLZEIT RL   "," s     ", P&kes[KE1].Stellzeit_rl,	US_CHAR,0, P&hid1,	V1, 0, 0},
	{"*->:"," stellsumme rl  "," s     ", P&ked[KE1].stellsum_rl,	 	 S_INT, 2, P&hid2,	V0, 0, 0},
	{"*->:"," fahrzeit rl    "," s     ", P&ked[KE1].fahren_rl,		 	 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	{"*46;"," SOLLWERT RL    "," C    ", P&kes[KE1].Ttaup,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*47;"," REGELABW.RL    "," K     ", P&ked[KE1].ei_rl,				   S_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE1 == 2 
	{"*48;"," VENTILSTELL.RL "," %     ", P&RVENTKR[KE1],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE1].si_y_rel_rl,		 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE1].Wup_rl,			 	US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif

	#if SSF
	{" 50;"," SSF DAUER      "," min   ", P&kes[KE1].SSFdauer,		 US_CHAR, 0, P&hid1,	V1, 0, 0},			
	{"*52:"," SSF EIN ?      ","       ", P&ked[KE1].ssfEin, 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," SSF-Taste ein ?","       ", P&SSFKE[KE1],		 	 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
	{" ->;"," SSF Ctr        "," s     ", P&ked[KE1].ctrSSF,			  US_INT, 0, P&hid2,	V0, 0, 0},	
	#endif

	#if RM_POWER_ANZ
	{"*60:"," RM LEISTUNG    "," %     ", P&rmPower[U1].mwSkal,		ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*60:"," RM LEISTUNG    "," %     ", P&rmPower[U1].mwSkal,ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{" 61;"," LEISTUNG 0%    "," V     ", P&RmPowerPara[U1].Skal0,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 61;"," MESSWERT 10V   "," %     ", P&RmPowerPara[U1].Skal10,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm leistung "," s     ", P&RmPowerPara[U1].ZkFilt,	US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung roh"," V     ", P&RM_POWER[U1],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung fil"," V     ", P&rmPower[U1].mwFilt,			US_INT, 2, P&hid2,	V0, 0, 0},	
	#endif

	#if AE_DRUCK_ANZ
	{"*64:"," DRUCK RUECKLAUF"," bar   ", P&druck[U1].mwSkal,			ANA_FORM, 2, P&vis,		V1, 0, 0},
	{"*64:"," DRUCK RUECKLAUF"," bar   ", P&druck[U1].mwSkal,  ANA_FORM, 0x82, P&kom,		V1, 0, 0},
	{" 65;"," DRUCK 0 bar    "," V     ", P&DruckPara[U1].Skal0,		 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 65;"," MESSWERT 10V   "," bar   ", P&DruckPara[U1].Skal10,	 	 S_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," gl.druck       "," s     ", P&DruckPara[U1].ZkFilt,		US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," druck roh      "," V     ", P&AE_DRUCK[U1],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," druck fil      "," %     ", P&druck[U1].mwFilt,				US_INT, 1, P&hid2,	V0, 0, 0},	
	{" 66;"," DRUCK UGW      "," bar   ", P&DruckPara[U1].UGW,	 		 S_INT, 2, P&hid1,	V1, 0, 0},
	{" 66;"," DRUCK OGW      "," bar   ", P&DruckPara[U1].OGW,	 		 S_INT, 2, P&hid1,	V1, 0, 0},
	{" ->;"," hysterese sm   "," bar   ", P&DruckPara[U1].Hyst,		 	US_INT, 2, P&hid2,	V0, 0, 0},
	{"*67:"," SM DRUCK UGW ? ","       ", P&druck[U1].ugwSM,	 JANEIN_FORM, 0, P&vis,	A1, EINZEL, 0},
	{"*67:"," SM DRUCK OGW ? ","       ", P&druck[U1].ogwSM,	 JANEIN_FORM, 0, P&vis,	A1, EINZEL, 0},
	#endif
	
	#if SM_TABW
	{" 68;"," TDIFF.ALARM EIN"," K     ", P&kes[KE1].TabwMax,			 US_CHAR, 1, P&hid1,	V1, 0, 0},
	{" 69;"," ZVERZ.ALARM EIN"," min   ", P&kes[KE1].TabwZeit,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," temp.-abw.cnt  "," s     ", P&ked[KE1].tabwCnt,		 		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	{"*70:"," KESSEL         ","       ", P&ked[KE1].kesEin, 	 JANEIN_FORM, 2, P&vis,		V1, 0, 0},

	#if STUFIG_KE1 == 2	
	{" ->;"," Abweichung St2 "," K     ", P&ked[KE1].abw2,					 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," Abw. Summe St2 "," K*min ", P&ked[KE1].abwsumme2,			 S_INT, 1, P&hid2,	V0, 0, 0},	
	{" ->;"," Stufe 2 ein ?  ","       ", P&ked[KE1].stufe2, 	 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	{"*71:"," KESSEL STUFE 2 ","       ", P&KESTU2[KE1],	 	 JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	#endif	

	#if SOLLWERT_KE1 
	{"*->;"," sollwert       "," V     ", P&ked[KE1].sollwert,  		 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif		

	#if ( SOLLWERT_KE1 || MODUL_KE1 == 2 )
	{"*71:"," SOLLSPANNUNG   "," V     ", P&RVENTKE[KE1],				AOUT_FORMP, 2, P&vis,		V1, 0, 0},
	#endif

	#if BM_KE1	// Betriebsmeldung vorhanden
	{"*72:"," BM KESSEL      ","       ", P&BMKES[KE1],			 JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{" 72;"," WARTEZEIT BM KE"," min   ", P&kes[KE1].KeBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," keBm_cnt       "," s     ", P&ked[KE1].keBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
		
	#if PU_KE1 
	{"*73:"," KESSELPUMPE    ","       ", P&PUKEEA[KE1],		 JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	#endif

	#if PU_BUS_KE1 > 0								// Bus-Pumpe
	{"*73:"," BUS-PUMPE      ","       ", P&ked[KE1].busPuEin, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	#endif	

	#if ( PU_KE1 || PU_BUS_KE1 )
	{" 73;"," NACHLAUF PUMPE "," min   ", P&kes[KE1].Puna, 			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," nachlauf pumpe "," s     ", P&ked[KE1].puna, 		  		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if ( PU_KE1 && BM_PUKE1 )
	{"*74:"," BM PUMPE       ","       ", P&BMPUKE[KE1],		 JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{" 74;"," WARTEZEIT BM PU"," min   ", P&kes[KE1].PuBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," puBm_cnt       "," s     ", P&ked[KE1].puBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#if DZ_PU_KE1
	{" 75;"," P1 MODULATION  "," %     ", P&kes[KE1].P1ModDz,				 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 75;"," P1 DREHZAHL    "," %     ", P&kes[KE1].P1Dz,					 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 76;"," P2 MODULATION  "," %     ", P&kes[KE1].P2ModDz,				 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 76;"," P2 DREHZAHL    "," %     ", P&kes[KE1].P2Dz,					 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*77;"," DREHZAHL PUMPE "," %     ", P&ked[KE1].dzPu,			 		 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," drehzahl pumpe "," %     ", P&DZPUKE[KE1],				AOUT_FORMP, 1, P&hid2,	V0, 0, 0},
	#endif
		
	#if KL_KE1
	{"*78:"," ABSPERRKLAPPE  ","       ", P&KLKEAZ[KE1],		 JANEIN_FORMOP, 1, P&vis,		V1, 0, 0},
	{" 78;"," NACHLAUF KLAPPE"," min   ", P&kes[KE1].Klna, 			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," nachlauf klappe"," s     ", P&ked[KE1].klna, 		  		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	{"*79;"," KESSEL-LAUFZEIT"," h     ", P&ked[KE1].kesh,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," kessel-laufzeit"," s     ", P&ked[KE1].kesSec,				US_INT, 0, P&hid2,	V0, 0, 0},

	#if STUFIG_KE1 == 2	
	{" 79;"," LAUFZEIT STUFE2"," h     ", P&ked[KE1].kes2h,					US_INT, 0, P&hid1,	V1, 0, 0},	
	{" ->;"," laufzeit stufe2"," s     ", P&ked[KE1].kes2Sec,		 		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if BM_KE1	// Betriebsmeldung vorhanden
	{"*80:"," SM BM KESSEL ? ","       ", P&ked[KE1].keBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if ( PU_KE1 && BM_PUKE1 )
	{"*81:"," SM BM PUMPE ?  ","       ", P&ked[KE1].puBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if DR_KE1
	{"*82:"," SM DRUCK ?     ","       ", P&DRKE[KE1],			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if STB_KE1
	{"*83:"," SM STB ?       ","       ", P&STBKE[KE1],			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if NACHSP_KE1
	{"*84:"," SM NACHSPEISG.?","       ", P&NACHSP[KE1],		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if BRAL_KE1
	{"*85:"," SM BRENNER ?   ","       ", P&BRAL[KE1],			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if KEAL_KE1
	{"*85:"," SM KESSEL ?    ","       ", P&KEAL[KE1],			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if SM_TABW
	{"*86:"," SM REGELABW.?  ","       ", P&ked[KE1].abwAlarm, JANEIN_FORM, 0, P&vis, 	A1, EINZEL, 0},
	#endif
	#if PUAL_KE1
	{"*87:"," SM PUMPE ?     ","       ", P&PUKEAL[KE1],	 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if PU_BUS_KE1
	{"*87:"," SM BUS PUMPE ? ","       ", P&ked[KE1].busPuSm,	 JANEIN_FORM, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if NOTAUS_KE1
	{"*88:"," SM NOT_AUS ?   ","       ", P&NOTAUS[KE1],		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	
	{"*89:"," HANDBETRIEB ?  ","       ", P&kes[KE1].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	#if ( MODUL_KE1 || DZ_PU_KE1 )
	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&kes[KE1].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&kes[KE1].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&kes[KE1].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&kes[KE1].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},
	#if MODUL_KE1 == 1
	{" 94;"," STELLZEIT      "," s     ", P&kes[KE1].Stellzeit,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," stellsumme     "," s     ", P&ked[KE1].stellsum,			 S_INT, 2, P&hid2,	V0, 0, 0},
	{"*->:"," fahrzeit       "," s     ", P&ked[KE1].fahren,		 	 	 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	{"*96;"," SOLLWERT       "," C    ", P&ked[KE1].tvsb,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ked[KE1].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," MODULATION     "," %     ", P&ked[KE1].y_rel,			 		 S_INT, 1, P&hid1,	V1, 0, 0},
	#if MODUL_KE1 == 2
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE1].si_y_rel,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE1].Wup,			 			US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif

	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&kes[KE1].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},

	//********************* bei mod.Kessel: nur wenn Ausgang existiert !!!
//-	#if ( MODUL_KE1 || STUFIG_KE1 )
//-	{" ->;"," KESSEL EIN/AUS "," EIN=1 ", P&kes[KE1].Kesea,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	#endif		

	#if STUFIG_KE1 == 2
	{" ->;"," STUFE 2 EIN/AUS"," EIN=1 ", P&kes[KE1].Kestufe2,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	
	#if PU_KE1 == 1
	{" ->;"," KESSELPUMPE E/A"," EIN=1 ", P&kes[KE1].Puea,			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	
	#if ( DZ_PU_KE1	&& PU_BUS_KE1 == 0 )
	{" ->;"," DREHZAHL STELL."," %     ", P&kes[KE1].DzPuHand,			US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
	
	#if KL_KE1 == 1
	{" ->;"," KLAPPE AUF/ZU  "," AUF=1 ", P&kes[KE1].Kesklaz,			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	
	#if MODUL_KE1 == 1 
	{" ->;"," MODULATION ZU  "," s     ", P&ked[KE1].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," MODULATION AUF "," s     ", P&ked[KE1].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE1 == 2 || SOLLWERT_KE1
	{" ->;"," SOLLSPG.STELLEN"," V     ", P&kes[KE1].Kestell,			 	US_INT, 2, P&hid1,	V1, 0, 0},
	#endif

	#if RL_KE1 == 1
	{" ->;"," RL-VENTIL ZU   "," s     ", P&ked[KE1].rvzu,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," RL-VENTIL AUF  "," s     ", P&ked[KE1].rvauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE1 == 2
	{" ->;"," RL-VENTIL STELL"," %     ", P&kes[KE1].Kestell_rl,  	US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

};


#else

const Pgrup ke[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesAllg,							ASCII_FORM, 0, P&vis,		V1, 0, 0},	

	#if TVG_KE1
	{"*01:"," VORLAUF GESAMT "," C    ", P&TVGKE[0],					 	 	 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF GESAMT ","       ", P&TVGKE[0],						ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TRG_KE1
	{"*02:"," RUECKL.GESAMT  "," C    ", P&TRGKE[0],					 	 	 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*02:"," RUECKL.GESAMT  ","       ", P&TRGKE[0],						ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TVG_KE2
	{"*03:"," VORLAUF WEICHE "," C    ", P&TVGKE[1],					 	 	 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*03:"," VORLAUF WEICHE "," C    ", P&TVGKE[1],						ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	{"*->;"," vl strategie   "," C    ", P&VL_STRATEG,					 	 ANA_FORMP, 1, P&hid2,	V0, 0, 0},
	{"*->;"," vl folge aus   "," C    ", P&VL_FOLGE_AUS,					 ANA_FORMP, 1, P&hid2,	V0, 0, 0},
	{"*->;"," rl folge aus   "," C    ", P&RL_FOLGE_AUS,					 ANA_FORMP, 1, P&hid2,	V0, 0, 0},
	{"*->;"," vl dyn.anhebung"," C    ", P&VL_ANHEB,					 		 ANA_FORMP, 1, P&hid2,	V0, 0, 0},

	{" 10;"," SCHALTDIFFERENZ"," K     ", P&kes[0].KesSchaltDiff,			US_INT, 1, P&hid1,	V1, 0, 0},	
	{" 11;"," VL GESAMT MAX  "," C    ", P&kes[0].VlVerteilerMax,		 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 12;"," DIFFmin VL-RL  "," K     ", P&kes[0].FolgeKesAus,		 	 US_CHAR, 1, P&hid1,	V1, 0, 0},

	#if ( SOLLWERT_KE1 || STUFIG_KE1 || MODUL_KE1 == 1 )
	{" 13;"," EIN FOLGEKESSEL"," K*min ", P&kes[0].Einkrit1,				 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 13;"," AUS FOLGEKESSEL"," K*min ", P&kes[0].Auskrit1,				 	 S_INT, 1, P&hid1,	V1, 0, 0},		
	{" ->;"," mittl.Regelabw."," K     ", P&ked[0].abw1,					 		 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," Summe Regelabw."," K*min ", P&ked[0].abwsumme1,					 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if MODUL_KE1 == 2
	{" 14;"," MOD.KESSEL EIN "," %     ", P&kes[0].KesselEinMod,			US_INT, 1, P&hid1,	V1, 0, 0},	
	{" 14;"," VERZ.MOD.KE.EIN"," min   ", P&kes[0].KesselEinModVerz, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 15;"," MOD.KESSEL AUS "," %     ", P&kes[0].KesselAusMod,			US_INT, 1, P&hid1,	V1, 0, 0},	
	{" 15;"," VERZ.MOD.KE.AUS"," min   ", P&kes[0].KesselAusModVerz, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 16;"," MOD.-VERZ.SOMM."," min   ", P&kes[0].ModVerzSomm,	 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 16;"," MOD.-VERZ.WINT."," min   ", P&kes[0].ModVerzWint,	 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif

	{" 17;"," PAUSE KES EIN  "," min   ", P&kes[0].SchaltpauseZu,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 17;"," PAUSE KES AUS  "," min   ", P&kes[0].SchaltpauseAb,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," schaltpause ctr"," s     ", P&ked[0].pauseCtr,					US_INT, 0, P&hid2,	V0, 0, 0},

	{"*20:"," VORLAUF ANFORD."," C    ", P&ked[0].tvsb_anl,					US_INT, 1, P&vis,		V1, 0, 0},

	#if SW_ANH_VAR
	{"*21:"," ANF-ANHEB.DYN. "," K     ", P&ked[0].y_rel_anh,				   S_INT, 1, P&vis,		V1, 0, 0},	
	{" 22;"," ANHEB.VAR.MAX  "," K     ", P&kes[0].AnhebMax,					US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

//	{"*23:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],						ANA_FORM, 1, P&vis,		V1, 0, 0},      
//	{"*23:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],	 		 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
//	{"*24:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal,			 ANA_FORM, 1, P&vis,	V1, 0, 0},
//	{"*24:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal, 	  ANA_FORM, 0x81, P&kom,	V1, 0, 0},
//	{" ->;"," 0V ANFORD.INP  "," C    ", P&AnaInpPara[U1].Skal0,		 		S_INT, 1, P&hid1,	V0, 0, 0},	
//	{" ->;"," 10V ANFORD.INP "," C    ", P&AnaInpPara[U1].Skal10,	 		S_INT, 1, P&hid1,	V0, 0, 0},
//	{" ->;"," gl.anford.inp  "," s     ", P&AnaInpPara[U1].ZkFilt,		 US_INT, 0, P&hid2,	V0, 0, 0},
//	{"*->;"," anforderung inp"," V     ", P&AE_UNI[U1],					  	ANA_FORMP, 2, P&hid2,	V0, 0, 0},
//	{"*->;"," anforderung inp"," %     ", P&anaInp[U1].mwFilt,				 US_INT, 1, P&hid2,	V0, 0, 0},	

	#if AE_DRUCK_ANZ
	{"*34:"," DRUCK RUECKLAUF"," bar   ", P&druck[U1].mwSkal,			ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*34:"," DRUCK          "," bar   ", P&druck[U1].mwSkal,  ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{" 35;"," DRUCK 0 bar    "," V     ", P&DruckPara[U1].Skal0,		 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 35;"," MESSWERT 10V   "," bar   ", P&DruckPara[U1].Skal10,	 	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.druck       "," s     ", P&DruckPara[U1].ZkFilt,		US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," druck roh      "," V     ", P&AE_DRUCK[U1],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," druck fil      "," V     ", P&druck[U1].mwFilt,				US_INT, 2, P&hid2,	V0, 0, 0},	
	{" 36;"," DRUCK UGW      "," bar   ", P&DruckPara[U1].UGW,	 		 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 36;"," DRUCK OGW      "," bar   ", P&DruckPara[U1].OGW,	 		 S_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," hysterese sm   "," bar   ", P&DruckPara[U1].Hyst,		 	US_INT, 1, P&hid2,	V0, 0, 0},
	{"*37:"," SM DRUCK UGW ? ","       ", P&druck[U1].ugwSM,	 JANEIN_FORM, 0, P&vis,	A1, EINZEL, 0},
	{"*37:"," SM DRUCK OGW ? ","       ", P&druck[U1].ogwSM,	 JANEIN_FORM, 0, P&vis,	A1, EINZEL, 0},
	#endif

	#if MODUL_KE1
	{" 50;"," FUEHLER SSTM ? "," 1=JA  ", P&kes[0].FuehlerSSTM, 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#else
	{" ->;"," fuehler sstm ? "," 1=JA  ", P&kes[0].FuehlerSSTM, 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	#endif	
	
	{" 60;"," KES-FOLGE SOLL ","       ", P&kes[0].FolgeSoll,				 US_LONG, 0, P&hid1,	V1, 0, 0},
	{"*61:"," KES-FOLGE IST  ","       ", P&ked[0].folgeIst,				 US_LONG, 0, P&vis,		V1, 0, 0},
//	{" 64;"," KES-FOLGE SOMM.","       ", P&kes[0].FolgeSommer,			 US_LONG, 0, P&hid1,	V1, 0, 0},
//	{" 65;"," KES-FOLGE WINT.","       ", P&kes[0].FolgeWinter,			 US_LONG, 0, P&hid1,	V1, 0, 0},
	{" 66;"," FUEHR.KES.WECHS"," h     ", P&kes[0].KesWechseln,				US_INT, 0, P&hid1,	V1, 0, 0},	
	
	#if SM_TABW
	{" 68;"," TDIFF.ALARM EIN"," K     ", P&kes[0].TabwMax,			 		 US_CHAR, 1, P&hid1,	V1, 0, 0},
	{" 69;"," ZVERZ.ALARM EIN"," min   ", P&kes[0].TabwZeit,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," temp.-abw.cnt  "," s     ", P&ked[0].tabwCnt,		 				US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#if (KEANZ == 2)
	{"*71:"," KESSEL INAKTIV "," 2-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 2, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 3)
	{"*71:"," KESSEL INAKTIV "," 3-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 3, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 4)
	{"*71:"," KESSEL INAKTIV "," 4-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 4, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 5)
	{"*71:"," KESSEL INAKTIV "," 5-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 5, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 6)
	{"*71:"," KESSEL INAKTIV "," 6-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 6, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 7)
	{"*71:"," KESSEL INAKTIV "," 7-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 7, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 8)
	{"*71:"," KESSEL INAKTIV "," 8-1   ", P&KesSperre,	 	 		 	   DIGR_FORM, 8, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 2)
	{"*72:"," KESSELSTOERUNG "," 2-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 2, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 3)
	{"*72:"," KESSELSTOERUNG "," 3-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 3, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 4)
	{"*72:"," KESSELSTOERUNG "," 4-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 4, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 5)
	{"*72:"," KESSELSTOERUNG "," 5-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 5, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 6)
	{"*72:"," KESSELSTOERUNG "," 6-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 6, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 7)
	{"*72:"," KESSELSTOERUNG "," 7-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 7, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 8)
	{"*72:"," KESSELSTOERUNG "," 8-1   ", P&KesGestoert,	 	 		 	 DIGR_FORM, 8, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 2)
	{"*73:"," KESSELFREIGABEN"," 2-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 2, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 3)
	{"*73:"," KESSELFREIGABEN"," 3-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 3, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 4)
	{"*73:"," KESSELFREIGABEN"," 4-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 4, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 5)
	{"*73:"," KESSELFREIGABEN"," 5-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 5, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 6)
	{"*73:"," KESSELFREIGABEN"," 6-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 6, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 7)
	{"*73:"," KESSELFREIGABEN"," 7-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 7, P&vis,		V1, 0, 0},
	#endif
	#if (KEANZ == 8)
	{"*73:"," KESSELFREIGABEN"," 8-1   ", P&KesEin,	 	 		 	     	 DIGR_FORM, 8, P&vis,		V1, 0, 0},
	#endif
	{" ->;"," anzahl kes ein ","       ", P&AnzahlKesEin,			 			 US_CHAR, 0, P&hid2,	V0, 0, 0},

	#if (MODUL_KE1 == 2 )
	{"*76:"," MODULATION MW  "," %     ", P&ked[0].modulatMW,					US_INT, 1, P&vis,		V1, 0, 0},
	{" ->;"," verz.mod.ke.ctr"," s     ", P&ked[0].kesselModCtr,			 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if SM_TABW
	{"*86:"," SM REGELABW.?  ","       ", P&ked[0].abwAlarm, 		 JANEIN_FORM, 0, P&vis, 	A1, EINZEL, 0},
	#endif
	
	#if NOTAUS_KE1
	{"*88:"," SM NOT-AUS ?   ","       ", P&NOTAUS[0],		 	 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},		
	#endif
	
	#if SW_ANH_VAR
	{" 90;"," P-VERST. ANH.  "," %/K   ", P&kes[0].Kp_anh,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERST. ANH.  ","       ", P&kes[0].Kd_anh,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT ANH.  "," s     ", P&kes[0].Ts_anh,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZ.ANH."," s     ", P&kes[0].Tn_anh,						US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

};

const Pgrup ke1[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[0],					ASCII_FORM, 0, P&vis,		V1, 0, 0},

	#if TV_KE1
	{"*01:"," VORLAUF IST    "," C    ", P&TVKE[KE1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF IST    "," C    ", P&TVKE[KE1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TR_KE1
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if RM_POWER_ANZ
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U1].mwSkal,		ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U1].mwSkal,ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{" 06;"," LEISTUNG 0%    "," V     ", P&RmPowerPara[U1].Skal0,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 06;"," MESSWERT 10V   "," %     ", P&RmPowerPara[U1].Skal10,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm leistung "," s     ", P&RmPowerPara[U1].ZkFilt,	US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung roh"," V     ", P&RM_POWER[U1],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung fil"," V     ", P&rmPower[U1].mwFilt,			US_INT, 2, P&hid2,	V0, 0, 0},	
	#endif

	#if SOLLWERT_KE1	
	{" 14;"," SKAL.TEMP.MIN  "," C    ", P&kes[KE1].TvsolX1,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 14;"," SKAL.SPG.MIN   "," V     ", P&kes[KE1].SollY1,			   S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.TEMP.MAX  "," C    ", P&kes[KE1].TvsolX2,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.SPG.MAX   "," V     ", P&kes[KE1].SollY2,			   S_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE1
	{" 16;"," SKAL.MODULAT.0%"," V     ", P&kes[KE1].ModMin,				US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE1
	{" ->;"," mod.-verz.cnt  "," s     ", P&ked[KE1].modVerz,	 			US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
		
	#if ( MODUL_KE1 && SOLLWERT_KE1 )
	{" 17;"," MODUL.LEISTUNG "," 1=JA  ", P&kes[KE1].Modulation,	  US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif
		
	#if STUFIG_KE1 == 2		
	{" 17;"," EIN STUFE 2    "," K*min ", P&kes[KE1].Einkrit2,			 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 17;"," AUS STUFE 2    "," K*min ", P&kes[KE1].Auskrit2,			 S_INT, 1, P&hid1,	V1, 0, 0},	
	#endif

	{" 18:"," VORLAUF MAX    "," C    ", P&kes[KE1].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," VORLAUF MIN    "," C    ", P&kes[KE1].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	{"*20:"," VORLAUF BERECHN"," C    ", P&ked[KE1].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 21:"," SOLLWERT-ANHEB."," K     ", P&kes[KE1].Tvpa,				   S_INT, 1, P&vis,		V1, 0, 0},	

	#if RL_KE1												// RL-Begrenzung/Anhebung
	{" 38;"," begrenz./anheb."," 0/1   ", P&kes[KE1].RlAnheb, 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" 39:"," BEGRENZ.-TEMP. "," C    ", P&kes[KE1].Ttaup,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 40;"," P-VERST.BEGRENZ"," %/K   ", P&kes[KE1].Kp_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 41;"," D-VERST.BEGRENZ","       ", P&kes[KE1].Kd_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 42;"," TASTZEIT BEGR. "," s     ", P&kes[KE1].Ts_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	{" 43;"," NACHSTELLZ.BEGR"," s     ", P&kes[KE1].Tn_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE1 == 1
	{" 44;"," STELLZEIT BEGR "," s     ", P&kes[KE1].Stellzeit_rl,	US_CHAR,0, P&hid1,	V1, 0, 0},
	{"*->:"," stellsumme begr"," s     ", P&ked[KE1].stellsum_rl,	 	 S_INT, 2, P&hid2,	V0, 0, 0},
	{"*->:"," fahrzeit begr  "," s     ", P&ked[KE1].fahren_rl,		 	 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	{"*46;"," SOLLWERT BEGR. "," C    ", P&ked[KE1].soll_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*47;"," REGELABW.BEGR. "," K     ", P&ked[KE1].ei_rl,				   S_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE1 == 2 
	{"*48;"," VENTILSTELL.RL "," %     ", P&RVENTKR[KE1],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE1].si_y_rel_rl,		 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE1].Wup_rl,			 	US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif

	#if SSF == 1
	{" 50;"," SSF DAUER      "," min   ", P&kes[KE1].SSFdauer,		 US_CHAR, 0, P&hid1,	V1, 0, 0},			
	{"*52:"," SSF EIN ?      ","       ", P&ked[KE1].ssfEin, 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," SSF-Taste ein ?","       ", P&SSFKE[KE1],		 	 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
	{" ->;"," SSF Ctr        "," s     ", P&ked[KE1].ctrSSF,			  US_INT, 0, P&hid2,	V0, 0, 0},	
	#endif

	#if ( PU_KE1 || KL_KE1 )
	{" 56;"," BLOCKIERSCHUTZ "," JA=1  ", P&kes[KE1].Blockierschutz,US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif
	
	#if PU_KE1 
	{"*63:"," KESSELPUMPE    ","       ", P&PUKEEA[KE1],		 JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	
	#if DZ_PU_KE1
	{" 65;"," P1 MODULATION  "," %     ", P&kes[KE1].P1ModDz,				 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 65;"," P1 DREHZAHL    "," %     ", P&kes[KE1].P1Dz,					 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 66;"," P2 MODULATION  "," %     ", P&kes[KE1].P2ModDz,				 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 66;"," P2 DREHZAHL    "," %     ", P&kes[KE1].P2Dz,					 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*67;"," DREHZAHL PUMPE "," %     ", P&ked[KE1].dzPu,			 		 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," drehzahl pumpe "," %     ", P&DZPUKE[KE1],				AOUT_FORMP, 1, P&hid2,	V0, 0, 0},
	#endif
	
//-
//-	{" 64:"," dT VL-RL MIN   "," K     ", P&kes[KE1].TdiffMin,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 65:"," DZ PUMPE MAX   "," %     ", P&kes[KE1].PuDzMax,				US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 66:"," dT VL-RL MAX   "," K     ", P&kes[KE1].TdiffMax,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 67:"," DZ PUMPE MIN   "," %     ", P&kes[KE1].PuDzMin,				US_INT, 1, P&vis,		V1, 0, 0},
	{" 68;"," PUMPEN-NACHLAUF"," min   ", P&kes[KE1].Puna,	 			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," PumpenNachlCtr "," s     ", P&ked[KE1].puna, 					US_INT, 0, P&hid2,	V0, 0, 0},	

	#if BM_PUKE1
	{"*69:"," BM PUMPE       ","       ", P&BMPUKE[KE1],		 JANEIN_FORMIP, 1, P&vis,		V1, 0, 0},
	{" 69;"," WARTEZEIT BM PU"," min   ", P&kes[KE1].PuBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," puBm_cnt       "," s     ", P&ked[KE1].puBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#endif	// 	#if PU_KE1

	{" 70:"," KESSEL INAKTIV "," JA=1  ", P&kes[KE1].Sperre,	 	   US_CHAR, 0, P&vis,		V1, 0, 0},
//	{"*72:"," KESSEL EIN ?   ","       ", P&KE[KE1],	 	     JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{"*71:"," KESSEL EIN ?   ","       ", P&ked[KE1].kesEin,   JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," kessel ein ?   ","       ", P&ked[KE1].kesEinKE, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	#if STUFIG_KE1 == 2	
	{" ->;"," Abweichung St2 "," K     ", P&ked[KE1].abw2,					 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," Abw. Summe St2 "," K*min ", P&ked[KE1].abwsumme2,			 S_INT, 1, P&hid2,	V0, 0, 0},	
	{" ->;"," Stufe 2 ein ?  ","       ", P&ked[KE1].stufe2, 	 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	{"*72:"," KESSEL STUFE 2 ","       ", P&KESTU2[KE1],	 	 JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	#endif	

	#if SOLLWERT_KE1 
	{"*->;"," sollwert       "," V     ", P&ked[KE1].sollwert,  		 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif		
	#if ( SOLLWERT_KE1 || MODUL_KE1 == 2 )
	{"*72:"," SOLLSPANNUNG   "," V     ", P&RVENTKE[KE1],				AOUT_FORMP, 2, P&vis,		V1, 0, 0},
	#endif

	#if BM_KE1	// Betriebsmeldung vorhanden
	{"*73:"," BM KESSEL      ","       ", P&BMKES[KE1],			 JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{" 73;"," WARTEZEIT BM KE"," min   ", P&kes[KE1].KeBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," keBm_cnt       "," s     ", P&ked[KE1].keBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if KL_KE1
	{"*74:"," KESSELKLAPPE   ","       ", P&KLKEAZ[KE1],		 JANEIN_FORMOP, 1, P&vis,		V1, 0, 0},
	{" 74;"," NACHLAUF KLAPPE"," min   ", P&kes[KE1].Klna, 			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," nachlauf klappe"," s     ", P&ked[KE1].klna, 		  		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if PU_BUS_KE1	
	{"*->;"," pumpe ein ?    ","       ", P&ked[KE1].pu,	 	 JANEIN_FORM,	  0, P&hid2,	V0, 0, 0},	
	{"*75:"," BUS-PUMPE EIN ?","       ", P&ked[KE1].busPuEin, JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*76:"," PUMPENDREHZAHL "," %     ", P&ked[KE1].puDz, 		 			US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	{"*77;"," KESSEL-LAUFZEIT"," h     ", P&ked[KE1].kesh,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," kessel-laufzeit"," s     ", P&ked[KE1].kesSec,				US_INT, 0, P&hid2,	V0, 0, 0},
	{" 78;"," KESSEL-STARTS  ","       ", P&ked[KE1].kestarts, 			US_INT, 0, P&hid1,	V1, 0, 0},
// Fenzau
//	{" 78;"," STARTS/TAG     ","       ", P&ked[KE1].startsTag, 		US_INT, 0, P&hid1,	V1, 0, 0},
//	{" 78;"," STARTS/TAG MAX ","       ", P&kes[KE1].StartsTag, 		US_INT, 0, P&hid1,	V1, 0, 0},
//	{" 79:"," SM STARTS      ","       ", P&ked[KE1].smStarts, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},

	#if BM_KE1	// Betriebsmeldung vorhanden
	{"*80:"," SM BM KESSEL ? ","       ", P&ked[KE1].keBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if BM_PUKE1
	{"*81:"," SM BM PUMPE ?  ","       ", P&ked[KE1].puBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if STB_KE1
	{"*82:"," SM STB ?       ","       ", P&STBKE[KE1],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if BRAL_KE1
	{"*85:"," SM BRENNER ?   ","       ", P&BRAL[KE1],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if KEAL_KE1
	{"*86:"," SM KESSEL ?    ","       ", P&KEAL[KE1],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if PU_BUS_KE1	
	{"*88:"," SM BUS PUMPE ? ","       ", P&ked[KE1].busPuSm,	 JANEIN_FORM, 0, P&vis,		A1, EINZEL,   0},
	#endif

	{" ->;"," Kes.gestoert ? ","       ", P&ked[KE1].gestoert, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	{"*89:"," HANDBETRIEB ?  ","       ", P&kes[KE1].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	#if MODUL_KE1
	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&kes[KE1].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&kes[KE1].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&kes[KE1].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&kes[KE1].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},

	{"*96;"," SOLLWERT       "," C    ", P&ked[KE1].tvsb,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ked[KE1].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," MODULATION     "," %     ", P&ked[KE1].y_rel,				 	US_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," modulation     "," V     ", P&RVENTKE[KE1],				AOUT_FORMP, 2, P&hid2,	V0, 0, 0},
	#if MODUL_KE1 == 2
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE1].si_y_rel,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE1].Wup,			 			US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif
	
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&kes[KE1].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	//********************* bei mod.Kessel: nur wenn Ausgang existiert !!!
	#if ( MODUL_KE1 || STUFIG_KE1 )
	{" ->;"," KESSEL EIN/AUS "," EIN=1 ", P&kes[KE1].Kesea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if STUFIG_KE1 == 2
	{" ->;"," STUFE 2 EIN/AUS"," EIN=1 ", P&kes[KE1].Kestufe2,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if KL_KE1 == 1
	{" ->;"," KLAPPE AUF/ZU  "," AUF=1 ", P&kes[KE1].Kesklaz,			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE1 == 1 
	{" ->;"," MODULATION ZU  "," s     ", P&ked[KE1].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," MODULATION AUF "," s     ", P&ked[KE1].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE1 == 2 || SOLLWERT_KE1
	{" ->;"," SOLLSPG.STELLEN"," V     ", P&kes[KE1].Kestell,			 	US_INT, 2, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE1 == 1
	{" ->;"," BEGR-VENTIL ZU "," s     ", P&ked[KE1].rvzu,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," BEGR-VENTIL AUF"," s     ", P&ked[KE1].rvauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE1 == 2
	{" ->;"," RL-VENTIL STELL"," %     ", P&kes[KE1].Kestell_rl,  	US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
	#if PU_KE1 == 1
	{" ->;"," KESSELPUMPE E/A"," EIN=1 ", P&kes[KE1].Puea,			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if ( DZ_PU_KE1	&& PU_BUS_KE1 == 0 )
	{" ->;"," DREHZAHL STELL."," %     ", P&kes[KE1].DzPuHand,			US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

};


const Pgrup ke2[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[1],					ASCII_FORM, 0, P&vis,		V1, 0, 0},

	#if TV_KE2
	{"*01:"," VORLAUF IST    "," C    ", P&TVKE[KE2],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF IST    "," C    ", P&TVKE[KE2],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TR_KE2
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE2],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE2],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if RM_POWER_ANZ > 1
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U2].mwSkal,		ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U2].mwSkal,ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{" 06;"," LEISTUNG 0%    "," V     ", P&RmPowerPara[U2].Skal0,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 06;"," MESSWERT 10V   "," %     ", P&RmPowerPara[U2].Skal10,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm leistung "," s     ", P&RmPowerPara[U2].ZkFilt,	US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung roh"," V     ", P&RM_POWER[U2],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung fil"," V     ", P&rmPower[U2].mwFilt,			US_INT, 2, P&hid2,	V0, 0, 0},	
	#endif

	#if SOLLWERT_KE2	
	{" 14;"," SKAL.TEMP.MIN  "," C    ", P&kes[KE2].TvsolX1,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 14;"," SKAL.SPG.MIN   "," V     ", P&kes[KE2].SollY1,			   S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.TEMP.MAX  "," C    ", P&kes[KE2].TvsolX2,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.SPG.MAX   "," V     ", P&kes[KE2].SollY2,			   S_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE2
	{" 16;"," SKAL.MODULAT.0%"," V     ", P&kes[KE2].ModMin,				US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE2
	{" ->;"," mod.-verz.cnt  "," s     ", P&ked[KE2].modVerz,	 			US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#if ( MODUL_KE2 && SOLLWERT_KE2 )
	{" 17;"," MODUL.LEISTUNG "," 1=JA  ", P&kes[KE2].Modulation,	  US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif
		
	#if STUFIG_KE2 == 2		
	{" 17;"," EIN STUFE 2    "," K*min ", P&kes[KE2].Einkrit2,			 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 17;"," AUS STUFE 2    "," K*min ", P&kes[KE2].Auskrit2,			 S_INT, 1, P&hid1,	V1, 0, 0},	
	#endif

	{" 18:"," VORLAUF MAX    "," C    ", P&kes[KE2].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," VORLAUF MIN    "," C    ", P&kes[KE2].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	{"*20:"," VORLAUF BERECHN"," C    ", P&ked[KE2].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 21:"," SOLLWERT-ANHEB."," K     ", P&kes[KE2].Tvpa,				   S_INT, 1, P&vis,		V1, 0, 0},	

	#if RL_KE2												// RL-Begrenzung/Anhebung
	{" 38;"," begrenz./anheb."," 0/1   ", P&kes[KE2].RlAnheb, 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" 39:"," BEGRENZ.-TEMP. "," C    ", P&kes[KE2].Ttaup,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 40;"," P-VERST.BEGRENZ"," %/K   ", P&kes[KE2].Kp_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 41;"," D-VERST.BEGRENZ","       ", P&kes[KE2].Kd_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 42;"," TASTZEIT BEGR. "," s     ", P&kes[KE2].Ts_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	{" 43;"," NACHSTELLZ.BEGR"," s     ", P&kes[KE2].Tn_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE2 == 1
	{" 44;"," STELLZEIT BEGR "," s     ", P&kes[KE2].Stellzeit_rl,	US_CHAR,0, P&hid1,	V1, 0, 0},
	{"*->:"," stellsumme begr"," s     ", P&ked[KE2].stellsum_rl,	 	 S_INT, 2, P&hid2,	V0, 0, 0},
	{"*->:"," fahrzeit begr  "," s     ", P&ked[KE2].fahren_rl,		 	 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	{"*46;"," SOLLWERT BEGR. "," C    ", P&ked[KE2].soll_rl,				US_INT, 1, P&hid1,	V1, 0, 0},
	{"*47;"," REGELABW.BEGR. "," K     ", P&ked[KE2].ei_rl,				   S_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE2 == 2 
	{"*48;"," VENTILSTELL.RL "," %     ", P&RVENTKR[KE2],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE2].si_y_rel_rl,		 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE2].Wup_rl,			 	US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif

	#if SSF == 1
	{" 50;"," SSF DAUER      "," min   ", P&kes[KE2].SSFdauer,		 US_CHAR, 0, P&hid1,	V1, 0, 0},			
	{"*52:"," SSF EIN ?      ","       ", P&ked[KE2].ssfEin, 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," SSF-Taste ein ?","       ", P&SSFKE[KE2],		 	 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
	{" ->;"," SSF Ctr        "," s     ", P&ked[KE2].ctrSSF,			  US_INT, 0, P&hid2,	V0, 0, 0},	
	#endif

	#if ( PU_KE2 || KL_KE2 )
	{" 56;"," BLOCKIERSCHUTZ "," JA=1  ", P&kes[KE2].Blockierschutz,US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif
		
	#if PU_KE2 
	{"*63:"," KESSELPUMPE    ","       ", P&PUKEEA[KE2],		 JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	
	#if DZ_PU_KE2
	{" 65;"," P1 MODULATION  "," %     ", P&kes[KE2].P1ModDz,				 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 65;"," P1 DREHZAHL    "," %     ", P&kes[KE2].P1Dz,					 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 66;"," P2 MODULATION  "," %     ", P&kes[KE2].P2ModDz,				 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 66;"," P2 DREHZAHL    "," %     ", P&kes[KE2].P2Dz,					 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*67;"," DREHZAHL PUMPE "," %     ", P&ked[KE2].dzPu,			 		 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," drehzahl pumpe "," %     ", P&DZPUKE[KE2],				AOUT_FORMP, 1, P&hid2,	V0, 0, 0},
	#endif

//-
//-	{" 64:"," dT VL-RL MIN   "," K     ", P&kes[KE2].TdiffMin,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 65:"," DZ PUMPE MAX   "," %     ", P&kes[KE2].PuDzMax,				US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 66:"," dT VL-RL MAX   "," K     ", P&kes[KE2].TdiffMax,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 67:"," DZ PUMPE MIN   "," %     ", P&kes[KE2].PuDzMin,				US_INT, 1, P&vis,		V1, 0, 0},
	{" 68;"," PUMPEN-NACHLAUF"," min   ", P&kes[KE2].Puna,	 			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," PumpenNachlCtr "," s     ", P&ked[KE2].puna, 					US_INT, 0, P&hid2,	V0, 0, 0},	
	#endif	// 	#if PU_KE2

	#if BM_PUKE2
	{"*69:"," BM PUMPE       ","       ", P&BMPUKE[KE2],		 JANEIN_FORMIP, 1, P&vis,		V1, 0, 0},
	{" 69;"," WARTEZEIT BM PU"," min   ", P&kes[KE2].PuBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," puBm_cnt       "," s     ", P&ked[KE2].puBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	{" 70:"," KESSEL INAKTIV "," JA=1  ", P&kes[KE2].Sperre,	 	   US_CHAR, 0, P&vis,		V1, 0, 0},
//	{"*72:"," KESSEL EIN ?   ","       ", P&KE[KE2],	 	     JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{"*71:"," KESSEL EIN ?   ","       ", P&ked[KE2].kesEin,   JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," kessel ein ?   ","       ", P&ked[KE2].kesEinKE, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	#if STUFIG_KE2 == 2	
	{" ->;"," Abweichung St2 "," K     ", P&ked[KE2].abw2,					 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," Abw. Summe St2 "," K*min ", P&ked[KE2].abwsumme2,			 S_INT, 1, P&hid2,	V0, 0, 0},	
	{" ->;"," Stufe 2 ein ?  ","       ", P&ked[KE2].stufe2, 	 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	{"*72:"," KESSEL STUFE 2 ","       ", P&KESTU2[KE2],	 	 JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	#endif	

	#if SOLLWERT_KE2 
	{"*->;"," sollwert       "," V     ", P&ked[KE2].sollwert,  		 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif		
	#if ( SOLLWERT_KE2 || MODUL_KE2 == 2 )
	{"*72:"," SOLLSPANNUNG   "," V     ", P&RVENTKE[KE2],				AOUT_FORMP, 2, P&vis,		V1, 0, 0},
	#endif

	#if BM_KE2	// Betriebsmeldung vorhanden
	{"*73:"," BM KESSEL      ","       ", P&BMKES[KE2],			 JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{" 73;"," WARTEZEIT BM KE"," min   ", P&kes[KE2].KeBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," keBm_cnt       "," s     ", P&ked[KE2].keBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if KL_KE2
	{"*74:"," KESSELKLAPPE   ","       ", P&KLKEAZ[KE2],		 JANEIN_FORMOP, 1, P&vis,		V1, 0, 0},
	{" 74;"," NACHLAUF KLAPPE"," min   ", P&kes[KE2].Klna, 			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," nachlauf klappe"," s     ", P&ked[KE2].klna, 		  		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if PU_BUS_KE2
	{"*->;"," pumpe ein ?    ","       ", P&ked[KE2].pu,	 	 JANEIN_FORM,	  0, P&hid2,	V0, 0, 0},	
	{"*75:"," BUS-PUMPE EIN ?","       ", P&ked[KE2].busPuEin, JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*76:"," PUMPENDREHZAHL "," %     ", P&ked[KE2].puDz, 		 			US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	{"*77;"," KESSEL-LAUFZEIT"," h     ", P&ked[KE2].kesh,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," kessel-laufzeit"," s     ", P&ked[KE2].kesSec,				US_INT, 0, P&hid2,	V0, 0, 0},
	{" 78;"," KESSEL-STARTS  ","       ", P&ked[KE2].kestarts, 			US_INT, 0, P&hid1,	V1, 0, 0},

//Fenzau
//	{" 78;"," STARTS/TAG     ","       ", P&ked[KE2].startsTag, 		US_INT, 0, P&hid1,	V1, 0, 0},
//	{" 78;"," STARTS/TAG MAX ","       ", P&kes[KE2].StartsTag, 		US_INT, 0, P&hid1,	V1, 0, 0},
//	{" 79:"," SM STARTS      ","       ", P&ked[KE2].smStarts, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},

	#if BM_KE2	// Betriebsmeldung vorhanden
	{"*80:"," SM BM KESSEL ? ","       ", P&ked[KE2].keBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if BM_PUKE2
	{"*81:"," SM BM PUMPE ?  ","       ", P&ked[KE2].puBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if STB_KE2
	{"*82:"," SM STB ?       ","       ", P&STBKE[KE2],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if BRAL_KE2
	{"*85:"," SM BRENNER ?   ","       ", P&BRAL[KE2],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if KEAL_KE2
	{"*86:"," SM KESSEL ?    ","       ", P&KEAL[KE2],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if PU_BUS_KE2	
	{"*88:"," SM BUS PUMPE ? ","       ", P&ked[KE2].busPuSm,	 JANEIN_FORM, 0, P&vis,		A1, EINZEL,   0},
	#endif
	
	{" ->;"," Kes.gestoert ? ","       ", P&ked[KE2].gestoert, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	{"*89:"," HANDBETRIEB ?  ","       ", P&kes[KE2].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	#if MODUL_KE2
	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&kes[KE2].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&kes[KE2].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&kes[KE2].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&kes[KE2].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},

	{"*96;"," SOLLWERT       "," C    ", P&ked[KE2].tvsb,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ked[KE2].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," MODULATION     "," %     ", P&ked[KE2].y_rel,				 	US_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," modulation     "," V     ", P&RVENTKE[KE2],				AOUT_FORMP, 2, P&hid2,	V0, 0, 0},
	#if MODUL_KE2 == 2
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE2].si_y_rel,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE2].Wup,			 			US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif
	
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&kes[KE2].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	//********************* bei mod.Kessel: nur wenn Ausgang existiert !!!
	#if ( MODUL_KE2 || STUFIG_KE2 )
	{" ->;"," KESSEL EIN/AUS "," EIN=1 ", P&kes[KE2].Kesea,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif		
	#if STUFIG_KE2 == 2
	{" ->;"," STUFE 2 EIN/AUS"," EIN=1 ", P&kes[KE2].Kestufe2,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if KL_KE2 == 1
	{" ->;"," KLAPPE AUF/ZU  "," AUF=1 ", P&kes[KE2].Kesklaz,			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE2 == 1 
	{" ->;"," MODULATION ZU  "," s     ", P&ked[KE2].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," MODULATION AUF "," s     ", P&ked[KE2].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE2 == 2 || SOLLWERT_KE2
	{" ->;"," SOLLSPG.STELLEN"," V     ", P&kes[KE2].Kestell,			 	US_INT, 2, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE2 == 1
	{" ->;"," BEGR-VENTIL ZU "," s     ", P&ked[KE2].rvzu,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," BEGR-VENTIL AUF"," s     ", P&ked[KE2].rvauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE2 == 2
	{" ->;"," RL-VENTIL STELL"," %     ", P&kes[KE2].Kestell_rl,  	US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
	#if PU_KE2 == 1
	{" ->;"," KESSELPUMPE E/A"," EIN=1 ", P&kes[KE2].Puea,			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if ( DZ_PU_KE2	&& PU_BUS_KE2 == 0 )
	{" ->;"," DREHZAHL STELL."," %     ", P&kes[KE2].DzPuHand,			US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

};

#if KEANZ > 2
const Pgrup ke3[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[2],					ASCII_FORM, 0, P&vis,		V1, 0, 0},

	#if TV_KE3
	{"*01:"," VORLAUF   IST  "," C    ", P&TVKE[KE3],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   IST  "," C    ", P&TVKE[KE3],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TR_KE3
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE3],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE3],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if RM_POWER_ANZ > 2
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U3].mwSkal,		ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U3].mwSkal,ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{" 06;"," LEISTUNG 0%    "," V     ", P&RmPowerPara[U3].Skal0,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 06;"," MESSWERT 10V   "," %     ", P&RmPowerPara[U3].Skal10,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm leistung "," s     ", P&RmPowerPara[U3].ZkFilt,	US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung roh"," V     ", P&RM_POWER[U3],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung fil"," V     ", P&rmPower[U3].mwFilt,			US_INT, 2, P&hid2,	V0, 0, 0},	
	#endif

	#if SOLLWERT_KE3	
	{" 14;"," SKAL.TEMP.MIN  "," C    ", P&kes[KE3].TvsolX1,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 14;"," SKAL.SPG.MIN   "," V     ", P&kes[KE3].SollY1,			   S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.TEMP.MAX  "," C    ", P&kes[KE3].TvsolX2,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.SPG.MAX   "," V     ", P&kes[KE3].SollY2,			   S_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE3
	{" 16;"," SKAL.MODULAT.0%"," V     ", P&kes[KE3].ModMin,				US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE3
	{" ->;"," mod.-verz.cnt  "," s     ", P&ked[KE3].modVerz,	 			US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#if ( MODUL_KE3 && SOLLWERT_KE3 )
	{" 17;"," MODUL.LEISTUNG "," 1=JA  ", P&kes[KE3].Modulation,	  US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif

	{" 18:"," VORLAUF KES MAX"," C    ", P&kes[KE3].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," VORLAUF KES MIN"," C    ", P&kes[KE3].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	{"*20:"," VORLAUF BERECHN"," C    ", P&ked[KE3].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 21:"," SOLLWERT-ANHEB."," K     ", P&kes[KE3].Tvpa,				   S_INT, 1, P&vis,		V1, 0, 0},	

	#if RL_KE3												// RL-Begrenzung/Anhebung
	{" 38;"," begrenz./anheb."," 0/1   ", P&kes[KE3].RlAnheb, 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" 39:"," BEGRENZ.-TEMP. "," C    ", P&kes[KE3].Ttaup,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 40;"," P-VERST.BEGRENZ"," %/K   ", P&kes[KE3].Kp_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 41;"," D-VERST.BEGRENZ","       ", P&kes[KE3].Kd_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 42;"," TASTZEIT BEGR. "," s     ", P&kes[KE3].Ts_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	{" 43;"," NACHSTELLZ.BEGR"," s     ", P&kes[KE3].Tn_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE3 == 1
	{" 44;"," STELLZEIT BEGR "," s     ", P&kes[KE3].Stellzeit_rl,	US_CHAR,0, P&hid1,	V1, 0, 0},
	{"*->:"," stellsumme begr"," s     ", P&ked[KE3].stellsum_rl,	 	 S_INT, 2, P&hid2,	V0, 0, 0},
	{"*->:"," fahrzeit begr  "," s     ", P&ked[KE3].fahren_rl,		 	 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	{"*46;"," SOLLWERT BEGR. "," C    ", P&ked[KE3].soll_rl,				US_INT, 1, P&hid1,	V1, 0, 0},
	{"*47;"," REGELABW.BEGR. "," K     ", P&ked[KE3].ei_rl,				   S_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE3 == 2 
	{"*48;"," VENTILSTELL.RL "," %     ", P&RVENTKR[KE3],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE3].si_y_rel_rl,		 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE3].Wup_rl,			 	US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif

	#if SSF == 1
	{" 50;"," SSF DAUER      "," min   ", P&kes[KE3].SSFdauer,		 US_CHAR, 0, P&hid1,	V1, 0, 0},			
	{"*52:"," SSF EIN ?      ","       ", P&ked[KE3].ssfEin, 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," SSF-Taste ein ?","       ", P&SSFKE[KE3],		 	 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
	{" ->;"," SSF Ctr        "," s     ", P&ked[KE3].ctrSSF,			  US_INT, 0, P&hid2,	V0, 0, 0},	
	#endif

	#if ( PU_KE3 || KL_KE3 )
	{" 56;"," BLOCKIERSCHUTZ "," JA=1  ", P&kes[KE3].Blockierschutz,US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif

//-
//-	{" 64:"," dT VL-RL MIN   "," K     ", P&kes[KE3].TdiffMin,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 65:"," DZ PUMPE MAX   "," %     ", P&kes[KE3].PuDzMax,				US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 66:"," dT VL-RL MAX   "," K     ", P&kes[KE3].TdiffMax,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 67:"," DZ PUMPE MIN   "," %     ", P&kes[KE3].PuDzMin,				US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 68;"," PUMPEN-NACHLAUF"," min   ", P&kes[KE3].Puna,	 			 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{" ->;"," PumpenNachlCtr "," s     ", P&ked[KE3].puna, 					US_INT, 0, P&hid2,	V0, 0, 0},	

	{" 70:"," KESSEL INAKTIV "," JA=1  ", P&kes[KE3].Sperre,	 	   US_CHAR, 0, P&vis,		V1, 0, 0},
//	{"*72:"," KESSEL EIN ?   ","       ", P&KE[KE3],	 	     JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{"*71:"," KESSEL EIN ?   ","       ", P&ked[KE3].kesEin,   JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," kessel ein ?   ","       ", P&ked[KE3].kesEinKE, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	#if SOLLWERT_KE3 
	{"*->;"," sollwert       "," V     ", P&ked[KE3].sollwert,  		 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif		
	#if ( SOLLWERT_KE3 || MODUL_KE3 == 2 )
	{"*72:"," SOLLSPANNUNG   "," V     ", P&RVENTKE[KE3],				AOUT_FORMP, 2, P&vis,		V1, 0, 0},
	#endif

	#if BM_KE3	// Betriebsmeldung vorhanden
	{"*73:"," BM KESSEL      ","       ", P&BMKES[KE3],			 JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{" 73;"," WARTEZEIT BM KE"," min   ", P&kes[KE3].KeBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," keBm_cnt       "," s     ", P&ked[KE3].keBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if KL_KE3
	{"*74:"," KESSELKLAPPE   ","       ", P&KLKEAZ[KE3],		 JANEIN_FORMOP, 1, P&vis,		V1, 0, 0},
	{" 74;"," NACHLAUF KLAPPE"," min   ", P&kes[KE3].Klna, 			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," nachlauf klappe"," s     ", P&ked[KE3].klna, 		  		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if PU_BUS_KE3
	{"*->;"," pumpe ein ?    ","       ", P&ked[KE3].pu,	 	 JANEIN_FORM,	  0, P&hid2,	V0, 0, 0},	
	{"*75:"," BUS-PUMPE EIN ?","       ", P&ked[KE3].busPuEin, JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*76:"," PUMPENDREHZAHL "," %     ", P&ked[KE3].puDz, 		 			US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	{"*77;"," KESSEL-LAUFZEIT"," h     ", P&ked[KE3].kesh,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," kessel-laufzeit"," s     ", P&ked[KE3].kesSec,				US_INT, 0, P&hid2,	V0, 0, 0},
	{" 78;"," KESSEL-STARTS  ","       ", P&ked[KE3].kestarts, 			US_INT, 0, P&hid1,	V1, 0, 0},

	#if BM_KE3	// Betriebsmeldung vorhanden
	{"*80:"," SM BM KESSEL ? ","       ", P&ked[KE3].keBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if STB_KE3
	{"*82:"," SM STB ?       ","       ", P&STBKE[KE3],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if BRAL_KE3
	{"*85:"," SM BRENNER ?   ","       ", P&BRAL[KE3],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if KEAL_KE3
	{"*86:"," SM KESSEL ?    ","       ", P&KEAL[KE3],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if PU_BUS_KE3	
	{"*88:"," SM BUS PUMPE ? ","       ", P&ked[KE3].busPuSm,	 JANEIN_FORM, 0, P&vis,		A1, EINZEL,   0},
	#endif
	
	{" ->;"," Kes.gestoert ? ","       ", P&ked[KE3].gestoert, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	{"*89:"," HANDBETRIEB ?  ","       ", P&kes[KE3].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	#if MODUL_KE3
	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&kes[KE3].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&kes[KE3].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&kes[KE3].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&kes[KE3].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},

	{"*96;"," SOLLWERT       "," C    ", P&ked[KE3].tvsb,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ked[KE3].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," MODULATION     "," %     ", P&ked[KE3].y_rel,				 	US_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," modulation     "," V     ", P&RVENTKE[KE3],				AOUT_FORMP, 2, P&hid2,	V0, 0, 0},
	#if MODUL_KE3 == 2
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE3].si_y_rel,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE3].Wup,			 			US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif
	
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&kes[KE3].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	//********************* bei mod.Kessel: nur wenn Ausgang existiert !!!
	#if ( MODUL_KE3 || STUFIG_KE3 )
	{" ->;"," KESSEL EIN/AUS "," EIN=1 ", P&kes[KE3].Kesea,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif		
	#if KL_KE3 == 1
	{" ->;"," KLAPPE AUF/ZU  "," AUF=1 ", P&kes[KE3].Kesklaz,			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE3 == 1 
	{" ->;"," MODULATION ZU  "," s     ", P&ked[KE3].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," MODULATION AUF "," s     ", P&ked[KE3].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE3 == 2 || SOLLWERT_KE3
	{" ->;"," SOLLSPG.STELLEN"," V     ", P&kes[KE3].Kestell,			 	US_INT, 2, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE3 == 1
	{" ->;"," BEGR-VENTIL ZU "," s     ", P&ked[KE3].rvzu,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," BEGR-VENTIL AUF"," s     ", P&ked[KE3].rvauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE3 == 2
	{" ->;"," RL-VENTIL STELL"," %     ", P&kes[KE3].Kestell_rl,  	US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
};
#endif

#if KEANZ > 3
const Pgrup ke4[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[3],					ASCII_FORM, 0, P&vis,		V1, 0, 0},

	#if TV_KE4
	{"*01:"," VORLAUF   IST  "," C    ", P&TVKE[KE4],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   IST  "," C    ", P&TVKE[KE4],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TR_KE4
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE4],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," RUECKLAUF IST  "," C    ", P&TRKE[KE4],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if RM_POWER_ANZ > 3
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U4].mwSkal,		ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*05:"," RM LEISTUNG    "," %     ", P&rmPower[U4].mwSkal,ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{" 06;"," LEISTUNG 0%    "," V     ", P&RmPowerPara[U4].Skal0,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	{" 06;"," MESSWERT 10V   "," %     ", P&RmPowerPara[U4].Skal10,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm leistung "," s     ", P&RmPowerPara[U4].ZkFilt,	US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung roh"," V     ", P&RM_POWER[U4],				 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm leistung fil"," V     ", P&rmPower[U4].mwFilt,			US_INT, 2, P&hid2,	V0, 0, 0},	
	#endif

	#if SOLLWERT_KE4	
	{" 14;"," SKAL.TEMP.MIN  "," C    ", P&kes[KE4].TvsolX1,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 14;"," SKAL.SPG.MIN   "," V     ", P&kes[KE4].SollY1,			   S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.TEMP.MAX  "," C    ", P&kes[KE4].TvsolX2,			 	 S_INT, 1, P&hid1,	V1, 0, 0},
	{" 15;"," SKAL.SPG.MAX   "," V     ", P&kes[KE4].SollY2,			   S_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE4
	{" 16;"," SKAL.MODULAT.0%"," V     ", P&kes[KE4].ModMin,				US_INT, 1, P&hid1,	V1, 0, 0},
	#endif

	#if MODUL_KE4
	{" ->;"," mod.-verz.cnt  "," s     ", P&ked[KE4].modVerz,	 			US_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	
	#if ( MODUL_KE4 && SOLLWERT_KE4 )
	{" 17;"," MODUL.LEISTUNG "," 1=JA  ", P&kes[KE4].Modulation,	  US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif

	{" 18:"," VORLAUF KES MAX"," C    ", P&kes[KE4].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," VORLAUF KES MIN"," C    ", P&kes[KE4].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	{"*20:"," VORLAUF BERECHN"," C    ", P&ked[KE4].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 21:"," SOLLWERT-ANHEB."," K     ", P&kes[KE4].Tvpa,				   S_INT, 1, P&vis,		V1, 0, 0},	

	#if RL_KE4												// RL-Begrenzung/Anhebung
	{" 38;"," begrenz./anheb."," 0/1   ", P&kes[KE4].RlAnheb, 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" 39:"," BEGRENZ.-TEMP. "," C    ", P&kes[KE4].Ttaup,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 40;"," P-VERST.BEGRENZ"," %/K   ", P&kes[KE4].Kp_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 41;"," D-VERST.BEGRENZ","       ", P&kes[KE4].Kd_rl,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 42;"," TASTZEIT BEGR. "," s     ", P&kes[KE4].Ts_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	{" 43;"," NACHSTELLZ.BEGR"," s     ", P&kes[KE4].Tn_rl,					US_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE4 == 1
	{" 44;"," STELLZEIT BEGR "," s     ", P&kes[KE4].Stellzeit_rl,	US_CHAR,0, P&hid1,	V1, 0, 0},
	{"*->:"," stellsumme begr"," s     ", P&ked[KE4].stellsum_rl,	 	 S_INT, 2, P&hid2,	V0, 0, 0},
	{"*->:"," fahrzeit begr  "," s     ", P&ked[KE4].fahren_rl,		 	 S_INT, 0, P&hid2,	V0, 0, 0},
	#endif
	{"*46;"," SOLLWERT BEGR. "," C    ", P&ked[KE4].soll_rl,				US_INT, 1, P&hid1,	V1, 0, 0},
	{"*47;"," REGELABW.BEGR. "," K     ", P&ked[KE4].ei_rl,				   S_INT, 1, P&hid1,	V1, 0, 0},
	#if RL_KE4 == 2 
	{"*48;"," VENTILSTELL.RL "," %     ", P&RVENTKR[KE4],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE4].si_y_rel_rl,		 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE4].Wup_rl,			 	US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif

	#if SSF == 1
	{" 50;"," SSF DAUER      "," min   ", P&kes[KE4].SSFdauer,		 US_CHAR, 0, P&hid1,	V1, 0, 0},			
	{"*52:"," SSF EIN ?      ","       ", P&ked[KE4].ssfEin, 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," SSF-Taste ein ?","       ", P&SSFKE[KE4],		 	 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
	{" ->;"," SSF Ctr        "," s     ", P&ked[KE4].ctrSSF,			  US_INT, 0, P&hid2,	V0, 0, 0},	
	#endif

	#if ( PU_KE4 || KL_KE4 )
	{" 56;"," BLOCKIERSCHUTZ "," JA=1  ", P&kes[KE4].Blockierschutz,US_CHAR,0, P&hid1,	V1, 0, 0},
	#endif

//-
//-	{" 64:"," dT VL-RL MIN   "," K     ", P&kes[KE4].TdiffMin,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 65:"," DZ PUMPE MAX   "," %     ", P&kes[KE4].PuDzMax,				US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 66:"," dT VL-RL MAX   "," K     ", P&kes[KE4].TdiffMax,			US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 67:"," DZ PUMPE MIN   "," %     ", P&kes[KE4].PuDzMin,				US_INT, 1, P&vis,		V1, 0, 0},
//-	{" 68;"," PUMPEN-NACHLAUF"," min   ", P&kes[KE4].Puna,	 			 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{" ->;"," PumpenNachlCtr "," s     ", P&ked[KE4].puna, 					US_INT, 0, P&hid2,	V0, 0, 0},	

	{" 70:"," KESSEL INAKTIV "," JA=1  ", P&kes[KE4].Sperre,	 	   US_CHAR, 0, P&vis,		V1, 0, 0},
//	{"*72:"," KESSEL EIN ?   ","       ", P&KE[KE4],	 	     JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{"*71:"," KESSEL EIN ?   ","       ", P&ked[KE4].kesEin,   JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->;"," kessel ein ?   ","       ", P&ked[KE4].kesEinKE, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	#if SOLLWERT_KE4 
	{"*->;"," sollwert       "," V     ", P&ked[KE4].sollwert,  		 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif		
	#if ( SOLLWERT_KE4 || MODUL_KE4 == 2 )
	{"*72:"," SOLLSPANNUNG   "," V     ", P&RVENTKE[KE4],				AOUT_FORMP, 2, P&vis,		V1, 0, 0},
	#endif

	#if BM_KE4	// Betriebsmeldung vorhanden
	{"*73:"," BM KESSEL      ","       ", P&BMKES[KE4],			 JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{" 73;"," WARTEZEIT BM KE"," min   ", P&kes[KE4].KeBm_verz,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," keBm_cnt       "," s     ", P&ked[KE4].keBm_cnt,		  US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if KL_KE4
	{"*74:"," KESSELKLAPPE   ","       ", P&KLKEAZ[KE4],		 JANEIN_FORMOP, 1, P&vis,		V1, 0, 0},
	{" 74;"," NACHLAUF KLAPPE"," min   ", P&kes[KE4].Klna, 			 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," nachlauf klappe"," s     ", P&ked[KE4].klna, 		  		US_INT, 0, P&hid2,	V0, 0, 0},
	#endif

	#if PU_BUS_KE4
	{"*->;"," pumpe ein ?    ","       ", P&ked[KE4].pu,	 	 JANEIN_FORM,	  0, P&hid2,	V0, 0, 0},	
	{"*75:"," BUS-PUMPE EIN ?","       ", P&ked[KE4].busPuEin, JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*76:"," PUMPENDREHZAHL "," %     ", P&ked[KE4].puDz, 		 			US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	{"*77;"," KESSEL-LAUFZEIT"," h     ", P&ked[KE4].kesh,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," kessel-laufzeit"," s     ", P&ked[KE4].kesSec,				US_INT, 0, P&hid2,	V0, 0, 0},
	{" 78;"," KESSEL-STARTS  ","       ", P&ked[KE4].kestarts, 			US_INT, 0, P&hid1,	V1, 0, 0},

	#if BM_KE4	// Betriebsmeldung vorhanden
	{"*80:"," SM BM KESSEL ? ","       ", P&ked[KE4].keBm_sAl, JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif
	#if STB_KE4
	{"*82:"," SM STB ?       ","       ", P&STBKE[KE4],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if BRAL_KE4
	{"*85:"," SM BRENNER ?   ","       ", P&BRAL[KE4],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if KEAL_KE4
	{"*86:"," SM KESSEL ?    ","       ", P&KEAL[KE4],		 	 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	#if PU_BUS_KE4	
	{"*88:"," SM BUS PUMPE ? ","       ", P&ked[KE4].busPuSm,	 JANEIN_FORM, 0, P&vis,		A1, EINZEL,   0},
	#endif
	
	{" ->;"," Kes.gestoert ? ","       ", P&ked[KE4].gestoert, JANEIN_FORM, 0, P&hid2,	V0, 0, 0},

	{"*89:"," HANDBETRIEB ?  ","       ", P&kes[KE4].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	#if MODUL_KE4
	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&kes[KE4].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&kes[KE4].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&kes[KE4].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&kes[KE4].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},

	{"*96;"," SOLLWERT       "," C    ", P&ked[KE4].tvsb,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ked[KE4].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," MODULATION     "," %     ", P&ked[KE4].y_rel,				 	US_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," modulation     "," V     ", P&RVENTKE[KE4],				AOUT_FORMP, 2, P&hid2,	V0, 0, 0},
	#if MODUL_KE4 == 2
	{"*->;"," pid-stellgroess"," %     ", P&ked[KE4].si_y_rel,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->;"," pid-windup     "," %     ", P&kes[KE4].Wup,			 			US_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	#endif
	
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&kes[KE4].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	//********************* bei mod.Kessel: nur wenn Ausgang existiert !!!
	#if ( MODUL_KE4 || STUFIG_KE4 )
	{" ->;"," KESSEL EIN/AUS "," EIN=1 ", P&kes[KE4].Kesea,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif		
	#if KL_KE4 == 1
	{" ->;"," KLAPPE AUF/ZU  "," AUF=1 ", P&kes[KE4].Kesklaz,			 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE4 == 1 
	{" ->;"," MODULATION ZU  "," s     ", P&ked[KE4].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," MODULATION AUF "," s     ", P&ked[KE4].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if MODUL_KE4 == 2 || SOLLWERT_KE4
	{" ->;"," SOLLSPG.STELLEN"," V     ", P&kes[KE4].Kestell,			 	US_INT, 2, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE4 == 1
	{" ->;"," BEGR-VENTIL ZU "," s     ", P&ked[KE4].rvzu,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," BEGR-VENTIL AUF"," s     ", P&ked[KE4].rvauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
	#if RL_KE4 == 2
	{" ->;"," RL-VENTIL STELL"," %     ", P&kes[KE4].Kestell_rl,  	US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
};
#endif

#if KEANZ > 4
const Pgrup ke5[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[4],					ASCII_FORM, 0, P&vis,		V1, 0, 0},
};
#endif
#if KEANZ > 5
const Pgrup ke6[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[5],					ASCII_FORM, 0, P&vis,		V1, 0, 0},
};
#endif
#if KEANZ > 6
const Pgrup ke7[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[6],					ASCII_FORM, 0, P&vis,		V1, 0, 0},
}
#endif
#if KEANZ > 7
const Pgrup ke8[] = {
	{"***:"," KESSELSTEUERUNG","       ", P&KesTyp[7],					ASCII_FORM, 0, P&vis,		V1, 0, 0},
}
#endif


#endif	
