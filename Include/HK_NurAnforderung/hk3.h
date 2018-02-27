/*------------------------------ Heizkreis 3 -------------------------------------------*/
const Pgrup hk3[] = {
//-------------------
	{"_**:"," HEIZKREIS 3    ","       ", P&hkd[HK3].raumname,	ASCII_FORM, 0, P&vis,		V1, 0, 0},
//-	{"*01:"," VORLAUF   IST  "," C    ", P&TVS[HK3],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
//-	{"*01:"," VORLAUF   IST  "," C    ", P&TVS[HK3],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},

	#if TRLBEG_HK3==1
	{"*02:"," RUECKLAUF IST  "," C    ", P&TRS[HK3],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*02:"," RUECKLAUF IST  "," C    ", P&TRS[HK3],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if TRAUMK_HK3==1
	{"*03:"," RAUM      IST  "," C    ", P&TI[HK3],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*03:"," RAUM      IST  "," C    ", P&TI[HK3],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if BEDARF_HK3==1 && BEDRAUM_HK3==1
	{"*04:"," RAUM-EXT. IST  "," C    ", P&hkd[HK3].tibed,					 S_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 04:"," raum-ext. ist  "," C    ", P&hkd[HK3].tibed,					 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	
	{" 06;"," SO/WI HK-EXTRA "," 1=JA  ", P&hks[HK3].SoWiExtra,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 07;"," Ta-WINTER [EIN]"," C    ", P&hks[HK3].Tae,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 08;"," Ta-SOMMER [AUS]"," C    ", P&hks[HK3].Taa,						US_INT, 1, P&hid1,	V1, 0, 0},
	{"*09;"," SOMMER ?       ","       ", P&hkd[HK3].sowi,		 JANEIN_FORM, 0, P&hid1,	V1, 0, 0},

	{" 10:"," STEIGUNG       ","       ", P&hks[HK3].Anst,					US_INT, 2, P&vis,		V1, 0, 0},
	{" 11:"," NIVEAU         "," C    ", P&hks[HK3].Tvpa,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 12:"," VORLAUF   MAX  "," C    ", P&hks[HK3].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 13:"," VORLAUF   MIN  "," C    ", P&hks[HK3].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	#if TRAUMK_HK3==1
	{" 14:"," RAUM      SOLL "," C    ", P&hks[HK3].Tiso,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 14:"," raum      soll "," C    ", P&hks[HK3].Tiso,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if BEDARF_HK3==1 && BEDRAUM_HK3==1
	{"*14:"," RAUM-EXT. SOLL "," C    ", P&hkd[HK3].tisobed,				US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 14:"," raum-ext. soll "," C    ", P&hkd[HK3].tisobed,				US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if TRAUMK_HK3==1
	{" 15:"," RAUM      MIN  "," C    ", P&hks[HK3].Timi,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 15:"," raum      min  "," C    ", P&hks[HK3].Timi,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if TRAUMK_HK3==1 || (BEDARF_HK3==1 && BEDRAUM_HK3==1)
	{" ->;"," RAUM MIN HYST. "," K     ", P&hks[HK3].TimiHyst,			US_INT, 1, P&hid1,	V0, 0, 0},
	#else
	{" ->;"," raum min hyst. "," K     ", P&hks[HK3].TimiHyst,			US_INT, 1, P&hid2,	V0, 0, 0},	
	#endif

	#if TRAUMK_HK3==1 || (BEDARF_HK3==1 && BEDRAUM_HK3==1)
	{" 16;"," KASK.SOLL MAX  "," C    ", P&hks[HK3].KaskBegr,			US_INT, 1, P&hid1,	V0, 0, 0},
	#else
	{" 16;"," kask.soll max  "," C    ", P&hks[HK3].KaskBegr,			US_INT, 1, P&hid2,	V0, 0, 0},	
	#endif
	
	#if (BEDARF_HK3==1 && BEDRAUM_HK3==1) || TRAUMK_HK3==1
	{" 17:"," Xp-RAUM KASKADE","       ", P&hks[HK3].Kpk,						US_INT, 2, P&vis,		V1, 0, 0},
	#else
	{" 17;"," xp-raum kaskade","       ", P&hks[HK3].Kpk,						US_INT, 2, P&hid2,	V0, 0, 0},
	#endif

	#if TRLBEG_HK3==1
	{" 18:"," RUECKLAUF MAX  "," C    ", P&hks[HK3].Trma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," Xp-RUECKL.BEGR.","       ", P&hks[HK3].Kpr,						US_INT, 2, P&vis,		V1, 0, 0},
	#endif

	{"*20:"," VORLAUF BERECHN"," C    ", P&hkd[HK3].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{"*->:"," vorlauf ber.hkl"," C    ", P&hkd[HK3].tvsb_hkl,			US_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur adapt"," K     ", P&hkd[HK3].adaptKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur kaska"," K     ", P&hkd[HK3].kaskaKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur wwvor"," K     ", P&hkd[HK3].wwvorKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," vorlauf filt.zk"," s     ", P&hks[HK3].Fzk,						US_INT, 1, P&hid2,	V0, 0, 0},

	#if LEIBEGR_HK3 == 1
	{"*21:"," ABSENK.LEI.BEGR"," K     ", P&hkd[HK3].leistBegrAbsenk,US_INT,1, P&vis,		V1,	0, 0},
	#endif

	#if FBH_HK3==1
	{" 25;"," ESTR.-PR.START ","       ", P&hks[HK3].StartAufheiz, DATE_FORM, 1, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG1 "," C    ", P&hks[HK3].EstrichTemp[0], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG2 "," C    ", P&hks[HK3].EstrichTemp[1], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG3 "," C    ", P&hks[HK3].EstrichTemp[2], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG4 "," C    ", P&hks[HK3].EstrichTemp[3], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG5 "," C    ", P&hks[HK3].EstrichTemp[4], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG6 "," C    ", P&hks[HK3].EstrichTemp[5], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG7 "," C    ", P&hks[HK3].EstrichTemp[6], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG8 "," C    ", P&hks[HK3].EstrichTemp[7], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG9 "," C    ", P&hks[HK3].EstrichTemp[8], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG10"," C    ", P&hks[HK3].EstrichTemp[9], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG11"," C    ", P&hks[HK3].EstrichTemp[10],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG12"," C    ", P&hks[HK3].EstrichTemp[11],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG13"," C    ", P&hks[HK3].EstrichTemp[12],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG14"," C    ", P&hks[HK3].EstrichTemp[13],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG15"," C    ", P&hks[HK3].EstrichTemp[14],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG16"," C    ", P&hks[HK3].EstrichTemp[15],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG17"," C    ", P&hks[HK3].EstrichTemp[16],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG18"," C    ", P&hks[HK3].EstrichTemp[17],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG19"," C    ", P&hks[HK3].EstrichTemp[18],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG20"," C    ", P&hks[HK3].EstrichTemp[19],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG21"," C    ", P&hks[HK3].EstrichTemp[20],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG22"," C    ", P&hks[HK3].EstrichTemp[21],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG23"," C    ", P&hks[HK3].EstrichTemp[22],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG24"," C    ", P&hks[HK3].EstrichTemp[23],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG25"," C    ", P&hks[HK3].EstrichTemp[24],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG26"," C    ", P&hks[HK3].EstrichTemp[25],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG27"," C    ", P&hks[HK3].EstrichTemp[26],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG28"," C    ", P&hks[HK3].EstrichTemp[27],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG29"," C    ", P&hks[HK3].EstrichTemp[28],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG30"," C    ", P&hks[HK3].EstrichTemp[29],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{"*26:"," ESTRICH-PROGR.?","       ", P&hkd[HK3].estrichProg,JANEIN_FORM, 0, P&vis, 	E1, STANDARD, 0},
	{"*27;"," ESTRICH AKT.TAG","       ", P&hkd[HK3].estrichTag,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*27;"," ESTR.AKT.SOLLT."," C    ", P&hkd[HK3].estrichTemp,			US_INT, 0, P&hid1,	V0, 0, 0},
	#endif

	#if ( ANFORD > 0 || NTANZ > 0 || KEANZ > 0 )
	{" 28:"," ANFORD.ANHEBUNG"," K     ", P&hks[HK3].TvpAnh,				US_INT, 1, P&vis,			V1, 0, 0},
	#endif
	
	#if BEDARF_HK3==1
	{" 29:"," VORRANG ZEITABS"," 1=JA  ", P&hks[HK3].VorrangZeitabsenkung, US_CHAR, 0, P&vis,	V1, 0, 0},
	#endif

	{" 30:"," ABSENK-BEGIN WO"," h:min ", P&abs_tab[HK3][0].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" 31:"," ABSENK-DAUER WO"," h     ", P&abs_tab[HK3][0].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" 32:"," ABSENK-WERT  WO"," K     ", P&abs_tab[HK3][0].abwert,	US_INT, 1, P&vis,		V1, 0, 0},
	{" 34:"," GANZE WOCHE SET"," 1=JA  ", P&setwoche[HK3],				 US_CHAR, 0, P&vis,		V1, 0, 0},

	{" 35:"," ABSENK-BEGIN Mo"," h:min ", P&abs_tab[HK3][1].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Mo"," h     ", P&abs_tab[HK3][1].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Mo"," K     ", P&abs_tab[HK3][1].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 36:"," ABSENK-BEGIN Di"," h:min ", P&abs_tab[HK3][2].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Di"," h     ", P&abs_tab[HK3][2].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Di"," K     ", P&abs_tab[HK3][2].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 37:"," ABSENK-BEGIN Mi"," h:min ", P&abs_tab[HK3][3].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Mi"," h     ", P&abs_tab[HK3][3].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Mi"," K     ", P&abs_tab[HK3][3].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 38:"," ABSENK-BEGIN Do"," h:min ", P&abs_tab[HK3][4].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Do"," h     ", P&abs_tab[HK3][4].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Do"," K     ", P&abs_tab[HK3][4].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 39:"," ABSENK-BEGIN Fr"," h:min ", P&abs_tab[HK3][5].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Fr"," h     ", P&abs_tab[HK3][5].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Fr"," K     ", P&abs_tab[HK3][5].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 40:"," ABSENK-BEGIN Sa"," h:min ", P&abs_tab[HK3][6].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Sa"," h     ", P&abs_tab[HK3][6].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Sa"," K     ", P&abs_tab[HK3][6].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 41:"," ABSENK-BEGIN So"," h:min ", P&abs_tab[HK3][7].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER So"," h     ", P&abs_tab[HK3][7].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  So"," K     ", P&abs_tab[HK3][7].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{"*->;"," ZEITABSENKPHASE","       ", P&abs_ram[HK3].absen,	 JANEIN_FORM, 0, P&hid1,V1, 0, 0},
	
//#####ulsch : Ferienautomatik #####
	#if FER == 1
	{" 42:"," FERIENAUTOMATIK"," JA=1  ", P&hks[HK3].FerienAutom,	 		US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 43:"," FER.ABSENK-WERT"," K     ", P&hks[HK3].FerienAbsenk,		 US_INT, 1, P&vis,	V1, 0, 0},
	{"*->;"," ferienbetrieb ?","       ", P&hkd[HK3].absenFerien,	JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	#endif

	#if BEDARF_HK3==1
	{" 44:"," BEDARFS-ABSENK."," K     ", P&hks[HK3].Beabs,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" ->;"," bedarfs-absenk."," K     ", P&hks[HK3].Beabs,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if BEDRAUM_HK3 == 1
	{" 45:"," RAUMABSCHALTUNG"," JA=1  ", P&hks[HK3].AbschRaumanf,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 46:"," VENTILE AUSWERT"," JA=1  ", P&hks[HK3].VentilAuswert,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 47:"," VENT_AUF ABSCH."," %     ", P&hks[HK3].VentOffenMax,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{"*->;"," ventile offen  ","       ", P&hkd[HK3].ventOffen,				US_INT, 0, P&hid2,V0, 0, 0},
	{"*->;"," ventile gesamt ","       ", P&hkd[HK3].ventGesamt,			US_INT, 0, P&hid2,V0, 0, 0},
	{"*48:"," VENTILE OFFEN  "," %     ", P&hkd[HK3].ventOffenBer,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{"*49:"," RAUMABSCHALTG.?","       ", P&hkd[HK3].raumAbsch, JAFRAGE_FORM, 0, P&vis,	V1, 0, 0},
	#endif

	{" 50:"," HEIZ-TaGRENZE  "," C    ", P&hks[HK3].TaHeizgrenze,		 S_INT, 1, P&vis,	V1, 0, 0},
	{" 51:"," FROST-TaGRENZE "," C    ", P&hks[HK3].TaFrostgrenze,		 S_INT, 1, P&vis,	V1, 0, 0},
	{" 52:"," ABSCH-ZEITKONST"," h/K   ", P&hks[HK3].AbschZeitKonst,	US_INT, 1, P&vis,	V1, 0, 0},

	#if ( BEDRAUM_HK3==1 || TRAUMK_HK3==1 )
	{" 53;"," KASKADE-MODUS  ","       ", P&hks[HK3].KaskMode,		 	 US_CHAR, 0, P&hid1,V1, 0, 0},
	#endif

	#if LEIBEGR_HK3 == 1
	{" 55;"," Xp LEIST.-BEGR."," K/%   ", P&hks[HK3].XpLeistBegr,		US_INT, 2, P&hid1,	V1, 0, 0},
	#endif

	#if BEDRAUM_HK3	== 1
	{" 56;"," VL MAX SOMMER  "," C    ", P&hks[HK3].VorlaufMaxSommer, US_INT, 1, P&hid1,	V1, 0, 0},
	#else
	{" ->;"," vl max sommer  "," C    ", P&hks[HK3].VorlaufMaxSommer, US_INT, 1, P&hid2,	V0, 0, 0},
	#endif	

	{" 60:"," SCHNELLAUFHEIZ."," JA=1  ", P&hks[HK3].Sauf,				 US_CHAR, 0, P&vis,		V1, 0, 0},
	{" 61:"," SO/WI-AUTOMATIK"," JA=1  ", P&hks[HK3].Swhk,				 US_CHAR, 0, P&vis,		V1, 0, 0},

	#if SWAUS_HK3 == 1	// Anlagenschalter für HK-Nichtnutzung
	{" 64:"," HKAUS-ABSENKW. "," K     ", P&hks[HK3].SchalterAbsenk,US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	#if TRLBEG_HK3==1
	{" ->;"," r-begr.prim/sek"," PRIM=1", P&hks[HK3].Psbegr,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	#endif

	#if FRG_HK3 == 1
	{"*66:"," ANFORDERUNG ?  ","       ", P&FRGHK[HK3], 		 JANEIN_FORMIP, 0, P&vis,		V1, 0, 0},
	#endif

//-	{" 68;"," TDIFF.ALARM EIN"," K     ", P&hks[HK3].TabwMax,			 US_CHAR, 1, P&hid1,	V1, 0, 0},
//-	{" 69;"," ZVERZ.ALARM EIN"," min   ", P&hks[HK3].TabwZeit,		 US_CHAR, 0, P&hid1,	V1, 0, 0},

	#if BEDARF_HK3==1
	{"*70:"," HEIZ-BEDARF ?  ","       ", P&hkd[HK3].bedarf,	JAFRAGE_FORM, 0, P&vis,		V1, 0, 0},
	#endif
	{" ->:"," bedarf         "," 1J 2N ", P&hkd[HK3].bedarf,	 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," vorheiz_bed    ","       ", P&hkd[HK3].vorheiz_bed,	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," ticopy         ","       ", P&hkd[HK3].ticopy,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," CountDown      ","       ", P&hkd[HK3].CountDown,  	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," vorrang bedarf ","       ", P&hkd[HK3].vorra_bed,		 US_CHAR, 0, P&hid2,	V0, 0, 0},

#if ( PU_BUS_HK3 == 0 || PULZ_HK3 == 1 )		// also auch wenn Schalten der Pumpe über Ausgang und Bus vorgesehen
	#if STRPU_HK3 == 0			// keine Strahlpumpe
		#if PUDO_HK3 == 1			// Doppelpumpe
	{"*72:"," PUMPE 1 EIN ?  ","       ", P&PU[HK3], 				 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{"*72:"," PUMPE 2 EIN ?  ","       ", P&PUD[HK3], 			 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
			#if PULZ_HK3 == 1
	{"*73;"," PU1-LAUFZEIT   "," h     ", P&hkd[HK3].puLz,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu1-lz minctr  "," min*5 ", P&hkd[HK3].pu1min,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{"*73;"," PU2-LAUFZEIT   "," h     ", P&hkd[HK3].pudLz,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu2-lz minctr  "," min*5 ", P&hkd[HK3].pu2min,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
			#endif
		#else									// keine Doppelpumpe
	{"*72:"," HEIZ-PUMPE EIN?","       ", P&PU[HK3], 				 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
			#if PULZ_HK3 == 1
	{"*73;"," PUMPEN-LAUFZEIT"," h     ", P&hkd[HK3].puLz,					US_INT, 0, P&hid1,	V1, 0, 0},
			#endif
		#endif	
	
		#if PUBM_HK3 == 1 // Betriebsmeldung Pumpe vorhanden
	{"*74:"," BM HEIZ-PU EIN?","       ", P&BMPU[HK3], 			 JANEIN_FORMIP, 0, P&vis,		V1, 0, 0},
		#endif
	#endif

	#if PUDO_HK3 == 1
	{" 76:"," FUEHR.-PU.AKT. ","       ", P&hks[HK3].FuehrPu,			 US_CHAR, 0, P&vis,	  V1, 0, 0},	
	{" 76;"," FUEHR.-PU WECHS"," h     ", P&hks[HK3].PuWechseln,		US_INT, 0, P&hid1,	V1, 0, 0},	
	{" 76;"," FUEHR.-PU.FEST ","       ", P&hks[HK3].FuehrPuFest,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif
#endif	

#if PU_BUS_HK3 > 0								// Bus-Pumpe
	{"*72:"," BUS-PUMPE EIN ?","       ", P&hkd[HK3].busPuEin, 			 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" 73;"," PU-SOLL NORMAL "," %     ", P&hks[HK3].BusPuSollwert,		 		US_INT, 1, P&hid1,	V1, 0, 0},
	{" 74;"," PU-SOLL ABSENK "," %     ", P&hks[HK3].BusPuSollwertAbsenk,	US_INT, 1, P&hid1,	V1, 0, 0},
#endif	

#if ( PULZ_HK3 == 1 || PU_BUS_HK3 > 0 )
	{" 75;"," PU-NACHLAUFZEIT"," min   ", P&hks[HK3].Puna,					US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu-nachlauf cnt"," min*2 ", P&hkd[HK3].zpu,						 US_INT, 0, P&hid2,	V0, 0, 0},
#endif

	#if SWAUS_HK3 == 1	// Anlagenschalter für HK-Nichtnutzung
	{"*78:"," SCHALT.HK-AUS ?","       ", P&HKAUS[HK3], 	 	 JANEIN_FORMIP, 0, P&vis,		E1, EREIGNIS, 0},
	#endif

	{"*80:"," BETRIEBS-REGIE ","       ", P&hkd[HK3].regie_txt, DYN_ASCII_FORM, 0, P&vis,		V1, 0, 0},
	{"*81:"," ABSENKPHASE ?  ","       ", P&hkd[HK3].absen,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*->:"," ABSCHALTPHASE ?","       ", P&hkd[HK3].absch,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#if TRAUMK_HK3==1 || BEDRAUM_HK3==1
	{"*->:"," STUETZBETRIEB ?","       ", P&hkd[HK3].stuetz,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif
	{"*->:"," FROSTBETRIEB ? ","       ", P&hkd[HK3].frost,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*82:"," AUFHEIZPHASE ? ","       ", P&hkd[HK3].aufhe,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*83:"," SOMMERBETRIEB ?","       ", P&hkd[HK3].somme,	 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->:"," absenkwert abs "," K     ", P&abs_ram[HK3].tvab,			US_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," absenkzeit abs "," s/10  ", P&abs_ram[HK3].zasd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," abschalt   abs "," s/10  ", P&abs_ram[HK3].zAbsch,		US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," aufheiz    abs "," s/10  ", P&abs_ram[HK3].zahd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," absenkwert bed "," K     ", P&abs_bed[HK3].tvab,			US_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," absenkzeit bed "," s/10  ", P&abs_bed[HK3].zasd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," abschalt   bed "," s/10  ", P&abs_bed[HK3].zAbsch,		US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," aufheiz    bed "," s/10  ", P&abs_bed[HK3].zahd,			US_INT, 0, P&hid2,	V0, 0, 0},

	#if WWANZ > 0
	{"*84:"," WW-VORRANG ?   ","       ", P&hkd[HK3].vorra,	 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#else
	{" ->:"," ww-vorrang ?   ","       ", P&hkd[HK3].vorra,	 	 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	#endif

	#if TRLBEG_HK3==1
	{"*85:"," RUECKL.BEGR. ? ","       ", P&hkd[HK3].rlbegr,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif

//-	{"*86:"," SM REGELABW.?  ","       ", P&hkd[HK3].abwAlarm, JANEIN_FORM, 0, P&vis, 	A1, EINZEL,   0},

	#if STW_HK3==1
	{"*87:"," SM STW ?       ","       ", P&STWHK[HK3], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
	#endif

	#if PUAL_HK3 == 1		// Störmeldung Pumpe vorhanden
		#if PUDO_HK3 == 1
	{"*88:"," SM PUMPE 1 ?   ","       ", P&PUAL[HK3], 			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
	{"*88:"," SM PUMPE 2 ?   ","       ", P&PUDAL[HK3], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
		#else
	{"*88:"," SM HEIZ-PUMPE ?","       ", P&PUAL[HK3], 			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
		#endif
	#endif

	#if PUBM_HK3 == 1 	// Betriebsmeldung Pumpe vorhanden
	{"*88:"," SM BM-HEIZ-PU ?","       ", P&hkd[HK3].puBm,	 	JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif

	#if PU_BUS_HK3 > 0
	{"*88:"," SM BUS PUMPE ? ","       ", P&hkd[HK3].busPuSm,	JANEIN_FORM, 0, P&vis,		A1, EINZEL,   0},
	#endif

	{"*89:"," HANDBETRIEB ?  ","       ", P&hks[HK3].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

//-	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&hks[HK3].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
//-	{" 91;"," D-VERSTAERKUNG ","       ", P&hks[HK3].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
//-	{" 92;"," TASTZEIT       "," s     ", P&hks[HK3].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 92;"," tastzeit       "," s     ", P&hks[HK3].Ts,						US_INT, 1, P&hid2,	V0, 0, 0},
//-	{" 93;"," NACHSTELLZEIT  "," s     ", P&hks[HK3].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},
//-
//-	#if DREIP_HK3==1
//-	{" 94;"," VENTILHUB MAX  "," mm    ", P&hks[HK3].Hub,					 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{" 95;"," STELLGESCHWIND."," mm/min", P&hks[HK3].Vst,			 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{"*->:"," stellzeit ber. "," s     ", P&hkd[HK3].stellzeit,			 S_INT, 2, P&hid2,	V0, 0, 0},
//-	{"*->:"," stellsumme     "," s     ", P&hkd[HK3].stellsum,			 S_INT, 2, P&hid2,	V0, 0, 0},
//-	{"*->:"," fahrzeit       "," s     ", P&hkd[HK3].fahren,				 S_INT, 0, P&hid2,	V0, 0, 0},
//-	#else
//-	{" 94;"," VENTILSTELL.MIN"," %     ", P&hks[HK3].Y_rel_min,	 		US_INT, 1, P&hid1,	V1, 0, 0},
//-	{" 95;"," OEFFNUNGSBEGINN"," %     ", P&hks[HK3].Y_rel_beg,	 		US_INT, 1, P&hid1,	V1, 0, 0},
//-	#endif
//-
//-	{"*96;"," SOLLWERT       "," C    ", P&hkd[HK3].tsol,					US_INT, 1, P&hid1,	V1, 0, 0},
//-	{"*97;"," REGELABWEICHUNG"," K     ", P&hkd[HK3].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
//-	{"*->;"," dy_rel         ","       ", P&hkd[HK3].dy_rel,				 S_INT, 3, P&hid2,	V0, 0, 0},
//-
//-	#if DREIP_HK3==0
//-	{"*98;"," VENTILSTELLUNG "," %     ", P&RVENT[HK3],					AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
//-	#endif

	//-------------------------------------------------------------------------------------------------
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&hks[HK3].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},

#if ( PU_BUS_HK3 == 0 || PULZ_HK3 == 1 )
	#if STRPU_HK3 == 0
		#if PUDO_HK3 == 1
	{" ->:"," PUMPE 1 EIN/AUS"," EIN=1 ", P&hks[HK3].Puea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," PUMPE 2 EIN/AUS"," EIN=1 ", P&hks[HK3].Pu2ea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
		#else		
	{" ->:"," PUMPE EIN/AUS  "," EIN=1 ", P&hks[HK3].Puea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
		#endif	
	#endif
#endif

//-	#if DREIP_HK3==1
//-	{" ->:"," VENTIL ZUFAHREN"," s     ", P&hkd[HK3].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{" ->:"," VENTIL AUFFAHRN"," s     ", P&hkd[HK3].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	#else
//-	{" ->:"," VENTIL STELLEN "," %     ", P&hks[HK3].Vstell,			  US_INT, 1, P&hid1,	V1, 0, 0},
//-	#endif

};


/*------------------------------ Heizkurven-Adaption ----------------------------------------------*/
	#if ADAP_STAT_HK3 > 0
	const Pgrup ad3[] = {
		{"***:"," ADAPTION HK3   ","       ", P&Leer15,								  ASCII_FORM, 0, P&vis,		V1, 0, 0},
		{"*00:"," ADAPT.-MELDUNG ","       ", P&AdaptMeldg[HK3][0], DYN_ASCII_FORM, 0, P&vis,		V1, 0, 0},

	#if ADAP_QU_HK3 == 0
		#if ADAP_STAT_HK3 == 1
		{" ->;"," R50-Anford.Rx  "," V     ", P&mwAdapt[HK3][0],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#else
		#if	ADAP_STAT_HK3 > 1	
		{" ->;"," 1.R50-Anford.Rx"," V     ", P&mwAdapt[HK3][0],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif
		#endif
						
		#if ADAP_STAT_HK3 > 1
		{" ->;"," 2.R50-Anford.Rx"," V     ", P&mwAdapt[HK3][1],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif
		#if ADAP_STAT_HK3 > 2
		{" ->;"," 3.R50-Anford.Rx"," V     ", P&mwAdapt[HK3][2],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif
		#if ADAP_STAT_HK3 > 3
		{" ->;"," 4.R50-Anford.Rx"," V     ", P&mwAdapt[HK3][3],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif

	//### josch
		{" ->;"," adapt_countdown"," min   ", P&adapt_countdown[HK3],		 US_CHAR, 0, P&hid2,	V0, 0, 0},

		#if ADAP_STAT_HK3 == 1
		{"*01:"," R50-ANFORD.    "," V     ", P&mwAdaption[HK3][0],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*01:"," R50-ANFORD.    "," V     ", P&mwAdaption[HK3][0],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#else
		#if ADAP_STAT_HK3 > 1
		{"*01:"," 1.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][0],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*01:"," 1.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][0],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
		#endif
				
		#if ADAP_STAT_HK3 > 1
		{"*02:"," 2.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][1],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*02:"," 2.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][1],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
		#if ADAP_STAT_HK3 > 2
		{"*03:"," 3.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][2],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*03:"," 3.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][2],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
		#if ADAP_STAT_HK3 > 3
		{"*04:"," 4.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][3],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*04:"," 4.R50-ANFORD.  "," V     ", P&mwAdaption[HK3][3],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
	#else
		{"*->;"," Anforderung ana"," V     ", P&ADAPT[HK3],						 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
		{"*->;"," Anford.bin tief","       ", P&ADAPT_T[HK3],			 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
		{"*->;"," Anford.bin hoch","       ", P&ADAPT_H[HK3],			 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},

		{"*01:"," ANFORDERUNG    "," V     ", P&mwAdaption[HK3][0],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*01:"," ANFORDERUNG    "," V     ", P&mwAdaption[HK3][0],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

		{" 10:"," MESSBEGINN     "," Uhr   ", P&hks[HK3].MessBeginn,		 US_CHAR, 0, P&vis,		V1, 0, 0},
		{" 11:"," ADAPTIONS-WERT "," K     ", P&hks[HK3].AdaptWert,			 US_CHAR, 1, P&vis,		V1, 0, 0},
		{" 12:"," REGELABW.-GRENZ"," K     ", P&hks[HK3].EiMaxAdapt,		 US_CHAR, 1, P&vis,		V1, 0, 0},

		{" ->;"," Vorlauf zu tief","       ", P&tvsLow[HK3], 						 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Vorlauf normal ","       ", P&tvsNormal[HK3], 				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Vorlauf zu hoch","       ", P&tvsHigh[HK3], 					 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Leistg. zu tief","       ", P&leistgLow[HK3],				 	 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Leistg. zu hoch","       ", P&leistgHigh[HK3],				 US_CHAR, 0, P&hid2,	V0, 0, 0},

		{"*20;"," ADAPT.Ta=-25C "," K     ", P&AdaptKorr[HK3][0], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*21;"," ADAPT.Ta=-20C "," K     ", P&AdaptKorr[HK3][1], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*22;"," ADAPT.Ta=-15C "," K     ", P&AdaptKorr[HK3][2], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*23;"," ADAPT.Ta=-10C "," K     ", P&AdaptKorr[HK3][3], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*24;"," ADAPT.Ta=-5C  "," K     ", P&AdaptKorr[HK3][4], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*25;"," ADAPT.Ta=0C   "," K     ", P&AdaptKorr[HK3][5], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*26;"," ADAPT.Ta=5C   "," K     ", P&AdaptKorr[HK3][6], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*27;"," ADAPT.Ta=10C  "," K     ", P&AdaptKorr[HK3][7], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*28;"," ADAPT.Ta=15C  "," K     ", P&AdaptKorr[HK3][8], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*29;"," ADAPT.Ta=20C  "," K     ", P&AdaptKorr[HK3][9], 			 	S_INT, 1, P&hid1,	V1, 0, 0},

	};
	#endif
