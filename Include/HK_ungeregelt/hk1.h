/*------------------------------ Heizkreis 1 -------------------------------------------*/
const Pgrup hk1[] = {
//-------------------
	{"_**:"," HEIZKREIS 1    ","       ", P&hkd[HK1].raumname,	ASCII_FORM, 0, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   IST  "," C    ", P&TVS[HK1],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*01:"," VORLAUF   IST  "," C    ", P&TVS[HK1],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},

	#if TRLBEG_HK1==1
	{"*02:"," RUECKLAUF IST  "," C    ", P&TRS[HK1],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*02:"," RUECKLAUF IST  "," C    ", P&TRS[HK1],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if TRAUMK_HK1==1
	{"*03:"," RAUM      IST  "," C    ", P&TI[HK1],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*03:"," RAUM      IST  "," C    ", P&TI[HK1],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if BEDARF_HK1==1 && BEDRAUM_HK1==1
	{" 04:"," RAUM-EXT. IST  "," C    ", P&hkd[HK1].tibed,					 S_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 04:"," raum-ext. ist  "," C    ", P&hkd[HK1].tibed,					 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif
	
	{" 06;"," SO/WI HK-EXTRA "," 1=JA  ", P&hks[HK1].SoWiExtra,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 07;"," Ta-WINTER [EIN]"," C    ", P&hks[HK1].Tae,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 08;"," Ta-SOMMER [AUS]"," C    ", P&hks[HK1].Taa,						US_INT, 1, P&hid1,	V1, 0, 0},
	{"*09;"," SOMMER ?       ","       ", P&hkd[HK1].sowi,		 JANEIN_FORM, 0, P&hid1,	V1, 0, 0},

	{" 10:"," STEIGUNG       ","       ", P&hks[HK1].Anst,					US_INT, 2, P&vis,		V1, 0, 0},
	{" 11:"," NIVEAU         "," C    ", P&hks[HK1].Tvpa,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 12:"," VORLAUF   MAX  "," C    ", P&hks[HK1].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 13:"," VORLAUF   MIN  "," C    ", P&hks[HK1].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	#if TRAUMK_HK1==1
	{" 14:"," RAUM      SOLL "," C    ", P&hks[HK1].Tiso,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 14:"," raum      soll "," C    ", P&hks[HK1].Tiso,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if BEDARF_HK1==1 && BEDRAUM_HK1==1
	{" 14:"," RAUM-EXT. SOLL "," C    ", P&hkd[HK1].tisobed,				US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 14:"," raum-ext. soll "," C    ", P&hkd[HK1].tisobed,				US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if TRAUMK_HK1==1
	{" 15:"," RAUM      MIN  "," C    ", P&hks[HK1].Timi,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 15:"," raum      min  "," C    ", P&hks[HK1].Timi,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if TRAUMK_HK1==1 || (BEDARF_HK1==1 && BEDRAUM_HK1==1)
	{" ->;"," RAUM MIN HYST. "," K     ", P&hks[HK1].TimiHyst,			US_INT, 1, P&hid1,	V0, 0, 0},
	#else
	{" ->;"," raum min hyst. "," K     ", P&hks[HK1].TimiHyst,			US_INT, 1, P&hid2,	V0, 0, 0},	
	#endif

	#if TRAUMK_HK1==1 || (BEDARF_HK1==1 && BEDRAUM_HK1==1)
	{" 16;"," KASK.SOLL MAX  "," C    ", P&hks[HK1].KaskBegr,			US_INT, 1, P&hid1,	V0, 0, 0},
	#else
	{" 16;"," kask.soll max  "," C    ", P&hks[HK1].KaskBegr,			US_INT, 1, P&hid2,	V0, 0, 0},	
	#endif
	
	#if (BEDARF_HK1==1 && BEDRAUM_HK1==1) || TRAUMK_HK1==1
	{" 17:"," Xp-RAUM KASKADE","       ", P&hks[HK1].Kpk,						US_INT, 2, P&vis,		V1, 0, 0},
	#else
	{" 17;"," xp-raum kaskade","       ", P&hks[HK1].Kpk,						US_INT, 2, P&hid2,	V0, 0, 0},
	#endif

	#if TRLBEG_HK1==1
	{" 18:"," RUECKLAUF MAX  "," C    ", P&hks[HK1].Trma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," Xp-RUECKL.BEGR.","       ", P&hks[HK1].Kpr,						US_INT, 2, P&vis,		V1, 0, 0},
	#endif

	{"*20:"," VORLAUF BERECHN"," C    ", P&hkd[HK1].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{"*->:"," vorlauf ber.hkl"," C    ", P&hkd[HK1].tvsb_hkl,			US_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur adapt"," K     ", P&hkd[HK1].adaptKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur kaska"," K     ", P&hkd[HK1].kaskaKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur wwvor"," K     ", P&hkd[HK1].wwvorKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," vorlauf filt.zk"," s     ", P&hks[HK1].Fzk,						US_INT, 1, P&hid2,	V0, 0, 0},

	#if FBH_HK1==1
	{" 25;"," ESTR.-PR.START ","       ", P&hks[HK1].StartAufheiz, DATE_FORM, 1, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG1 "," C    ", P&hks[HK1].EstrichTemp[0], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG2 "," C    ", P&hks[HK1].EstrichTemp[1], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG3 "," C    ", P&hks[HK1].EstrichTemp[2], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG4 "," C    ", P&hks[HK1].EstrichTemp[3], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG5 "," C    ", P&hks[HK1].EstrichTemp[4], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG6 "," C    ", P&hks[HK1].EstrichTemp[5], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG7 "," C    ", P&hks[HK1].EstrichTemp[6], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG8 "," C    ", P&hks[HK1].EstrichTemp[7], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG9 "," C    ", P&hks[HK1].EstrichTemp[8], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG10"," C    ", P&hks[HK1].EstrichTemp[9], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG11"," C    ", P&hks[HK1].EstrichTemp[10],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG12"," C    ", P&hks[HK1].EstrichTemp[11],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG13"," C    ", P&hks[HK1].EstrichTemp[12],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG14"," C    ", P&hks[HK1].EstrichTemp[13],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG15"," C    ", P&hks[HK1].EstrichTemp[14],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG16"," C    ", P&hks[HK1].EstrichTemp[15],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG17"," C    ", P&hks[HK1].EstrichTemp[16],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG18"," C    ", P&hks[HK1].EstrichTemp[17],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG19"," C    ", P&hks[HK1].EstrichTemp[18],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG20"," C    ", P&hks[HK1].EstrichTemp[19],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG21"," C    ", P&hks[HK1].EstrichTemp[20],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG22"," C    ", P&hks[HK1].EstrichTemp[21],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG23"," C    ", P&hks[HK1].EstrichTemp[22],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG24"," C    ", P&hks[HK1].EstrichTemp[23],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG25"," C    ", P&hks[HK1].EstrichTemp[24],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG26"," C    ", P&hks[HK1].EstrichTemp[25],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG27"," C    ", P&hks[HK1].EstrichTemp[26],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG28"," C    ", P&hks[HK1].EstrichTemp[27],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG29"," C    ", P&hks[HK1].EstrichTemp[28],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG30"," C    ", P&hks[HK1].EstrichTemp[29],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{"*26:"," ESTRICH-PROGR.?","       ", P&hkd[HK1].estrichProg,JANEIN_FORM, 0, P&vis, 	E1, STANDARD, 0},
	{"*27;"," ESTRICH AKT.TAG","       ", P&hkd[HK1].estrichTag,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*27;"," ESTR.AKT.SOLLT."," C    ", P&hkd[HK1].estrichTemp,			US_INT, 0, P&hid1,	V0, 0, 0},
	#endif

	{" 28:"," ANFORD.ANHEBUNG"," K     ", P&hks[HK1].TvpAnh,				US_INT, 1, P&vis,			V1, 0, 0},

	#if BEDARF_HK1==1
	{" 30;"," ABSENK-BEGIN WO"," h:min ", P&abs_tab[HK1][0].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" 31;"," ABSENK-DAUER WO"," h     ", P&abs_tab[HK1][0].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" 32;"," ABSENK-WERT  WO"," K     ", P&abs_tab[HK1][0].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},
	{" 34;"," GANZE WOCHE SET"," 1=JA  ", P&setwoche[HK1],				 US_CHAR, 0, P&hid1,		V1, 0, 0},

	{" 35;"," ABSENK-BEGIN Mo"," h:min ", P&abs_tab[HK1][1].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Mo"," h     ", P&abs_tab[HK1][1].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Mo"," K     ", P&abs_tab[HK1][1].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 36;"," ABSENK-BEGIN Di"," h:min ", P&abs_tab[HK1][2].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Di"," h     ", P&abs_tab[HK1][2].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Di"," K     ", P&abs_tab[HK1][2].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 37;"," ABSENK-BEGIN Mi"," h:min ", P&abs_tab[HK1][3].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Mi"," h     ", P&abs_tab[HK1][3].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Mi"," K     ", P&abs_tab[HK1][3].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 38;"," ABSENK-BEGIN Do"," h:min ", P&abs_tab[HK1][4].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Do"," h     ", P&abs_tab[HK1][4].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Do"," K     ", P&abs_tab[HK1][4].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 39;"," ABSENK-BEGIN Fr"," h:min ", P&abs_tab[HK1][5].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Fr"," h     ", P&abs_tab[HK1][5].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Fr"," K     ", P&abs_tab[HK1][5].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 40;"," ABSENK-BEGIN Sa"," h:min ", P&abs_tab[HK1][6].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Sa"," h     ", P&abs_tab[HK1][6].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Sa"," K     ", P&abs_tab[HK1][6].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 41;"," ABSENK-BEGIN So"," h:min ", P&abs_tab[HK1][7].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER So"," h     ", P&abs_tab[HK1][7].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  So"," K     ", P&abs_tab[HK1][7].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},
	
	#else
	{" 30:"," ABSENK-BEGIN WO"," h:min ", P&abs_tab[HK1][0].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" 31:"," ABSENK-DAUER WO"," h     ", P&abs_tab[HK1][0].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" 32:"," ABSENK-WERT  WO"," K     ", P&abs_tab[HK1][0].abwert,	US_INT, 1, P&vis,		V1, 0, 0},
	{" 34:"," GANZE WOCHE SET"," 1=JA  ", P&setwoche[HK1],				 US_CHAR, 0, P&vis,		V1, 0, 0},

	{" 35:"," ABSENK-BEGIN Mo"," h:min ", P&abs_tab[HK1][1].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Mo"," h     ", P&abs_tab[HK1][1].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Mo"," K     ", P&abs_tab[HK1][1].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 36:"," ABSENK-BEGIN Di"," h:min ", P&abs_tab[HK1][2].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Di"," h     ", P&abs_tab[HK1][2].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Di"," K     ", P&abs_tab[HK1][2].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 37:"," ABSENK-BEGIN Mi"," h:min ", P&abs_tab[HK1][3].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Mi"," h     ", P&abs_tab[HK1][3].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Mi"," K     ", P&abs_tab[HK1][3].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 38:"," ABSENK-BEGIN Do"," h:min ", P&abs_tab[HK1][4].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Do"," h     ", P&abs_tab[HK1][4].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Do"," K     ", P&abs_tab[HK1][4].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 39:"," ABSENK-BEGIN Fr"," h:min ", P&abs_tab[HK1][5].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Fr"," h     ", P&abs_tab[HK1][5].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Fr"," K     ", P&abs_tab[HK1][5].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 40:"," ABSENK-BEGIN Sa"," h:min ", P&abs_tab[HK1][6].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Sa"," h     ", P&abs_tab[HK1][6].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Sa"," K     ", P&abs_tab[HK1][6].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 41:"," ABSENK-BEGIN So"," h:min ", P&abs_tab[HK1][7].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER So"," h     ", P&abs_tab[HK1][7].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  So"," K     ", P&abs_tab[HK1][7].abwert,	US_INT, 1, P&vis,		V1, 0, 0},
	#endif
	
//#####ulsch : Ferienautomatik #####
	#if FER == 1
	{" 42:"," FERIENAUTOMATIK"," JA=1  ", P&hks[HK1].FerienAutom,	 		US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 43:"," FER.ABSENK-WERT"," K     ", P&hks[HK1].FerienAbsenk,		 US_INT, 1, P&vis,	V1, 0, 0},
	{"*->;"," ferienbetrieb ?","       ", P&hkd[HK1].absenFerien,	JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	#endif

	#if BEDARF_HK1==1
	{" 44:"," BEDARFS-ABSENK."," K     ", P&hks[HK1].Beabs,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" ->;"," bedarfs-absenk."," K     ", P&hks[HK1].Beabs,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if BEDRAUM_HK1 == 1
	{" 45:"," RAUMABSCHALTUNG"," JA=1  ", P&hks[HK1].AbschRaumanf,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 46:"," VENTILE AUSWERT"," JA=1  ", P&hks[HK1].VentilAuswert,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 47:"," VENT_AUF ABSCH."," %     ", P&hks[HK1].VentOffenMax,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{"*->;"," ventile offen  ","       ", P&hkd[HK1].ventOffen,				US_INT, 0, P&hid2,V0, 0, 0},
	{"*->;"," ventile gesamt ","       ", P&hkd[HK1].ventGesamt,			US_INT, 0, P&hid2,V0, 0, 0},
	{"*48:"," VENTILE OFFEN  "," %     ", P&hkd[HK1].ventOffenBer,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{"*49:"," RAUMABSCHALTG.?","       ", P&hkd[HK1].raumAbsch, JAFRAGE_FORM, 0, P&vis,	V1, 0, 0},
	#endif

	{" 50:"," HEIZ-TaGRENZE  "," C    ", P&hks[HK1].TaHeizgrenze,		 S_INT, 1, P&vis,	V1, 0, 0},
	{" 51:"," FROST-TaGRENZE "," C    ", P&hks[HK1].TaFrostgrenze,		 S_INT, 1, P&vis,	V1, 0, 0},
	{" 52:"," ABSCH-ZEITKONST"," h/K   ", P&hks[HK1].AbschZeitKonst,	US_INT, 1, P&vis,	V1, 0, 0},

	#if LEIBEGR_HK1 == 1
	{" 55;"," LEIST.BEGR/Ts  "," %     ", P&hks[HK1].LeistBegrTs,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif

	{" 60:"," SCHNELLAUFHEIZ."," JA=1  ", P&hks[HK1].Sauf,				 US_CHAR, 0, P&vis,		V1, 0, 0},
	{" 61:"," SO/WI-AUTOMATIK"," JA=1  ", P&hks[HK1].Swhk,				 US_CHAR, 0, P&vis,		V1, 0, 0},

	#if SWAUS_HK1 == 1	// Anlagenschalter für HK-Nichtnutzung
	{" 64:"," HKAUS-ABSENKW. "," K     ", P&hks[HK1].SchalterAbsenk,US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	#if TRLBEG_HK1==1
	{" ->;"," r-begr.prim/sek"," PRIM=1", P&hks[HK1].Psbegr,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	#endif

	#if FRG_HK1 == 1
	{"*66:"," ANFORDERUNG ?  ","       ", P&FRGHK[HK1], 		 JANEIN_FORMIP, 0, P&vis,		V1, 0, 0},
	#endif

//-	{" 68;"," TDIFF.ALARM EIN"," K     ", P&hks[HK1].TabwMax,			 US_CHAR, 1, P&hid1,	V1, 0, 0},
//-	{" 69;"," ZVERZ.ALARM EIN"," min   ", P&hks[HK1].TabwZeit,		 US_CHAR, 0, P&hid1,	V1, 0, 0},

	#if BEDARF_HK1==1
	{"*70:"," HEIZ-BEDARF ?  ","       ", P&hkd[HK1].bedarf,	JAFRAGE_FORM, 0, P&vis,		V1, 0, 0},
	#endif
	{" ->:"," bedarf         "," 1J 2N ", P&hkd[HK1].bedarf,	 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," vorheiz_bed    ","       ", P&hkd[HK1].vorheiz_bed,	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," ticopy         ","       ", P&hkd[HK1].ticopy,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," CountDown      ","       ", P&hkd[HK1].CountDown,  	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," vorrang bedarf ","       ", P&hkd[HK1].vorra_bed,		 US_CHAR, 0, P&hid2,	V0, 0, 0},

#if GENIBUS_HK1	== 0
	#if STRPU_HK1 == 0			// keine Strahlpumpe
		#if PUDO_HK1 == 1			// Doppelpumpe
	{"*72:"," PUMPE 1 EIN ?  ","       ", P&PU[HK1], 				 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{"*72:"," PUMPE 2 EIN ?  ","       ", P&PUD[HK1], 			 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
			#if PULZ_HK1 == 1
	{" 73;"," PU1-LAUFZEIT   "," h     ", P&hkd[HK1].puLz,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu1-lz minctr  "," min*5 ", P&hkd[HK1].pu1min,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" 73;"," PU2-LAUFZEIT   "," h     ", P&hkd[HK1].pudLz,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu2-lz minctr  "," min*5 ", P&hkd[HK1].pu2min,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
			#endif
		#else									// keine Doppelpumpe
	{"*72:"," HEIZ-PUMPE EIN?","       ", P&PU[HK1], 				 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
			#if PULZ_HK1 == 1
	{" 73;"," PUMPEN-LAUFZEIT"," h     ", P&hkd[HK1].puLz,					US_INT, 0, P&hid1,	V1, 0, 0},
			#endif
		#endif	

	{" 74;"," PU-NACHLAUFZEIT"," min   ", P&hks[HK1].Puna,					US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu-nachlauf cnt"," min*2 ", P&hkd[HK1].zpu,						 US_INT, 0, P&hid2,	V0, 0, 0},
	
		#if PUBM_HK1 == 1 // Betriebsmeldung Pumpe vorhanden
	{"*75:"," BM HEIZ-PU EIN?","       ", P&BMPU[HK1], 			 JANEIN_FORMIP, 0, P&vis,		V1, 0, 0},
		#endif
	#endif

	#if PUDO_HK1 == 1
	{" 76:"," FUEHR.-PU.AKT. ","       ", P&hks[HK1].FuehrPu,			 US_CHAR, 0, P&vis,	  V1, 0, 0},	
	{" 76;"," FUEHR.-PU WECHS"," h     ", P&hks[HK1].PuWechseln,		US_INT, 0, P&hid1,	V1, 0, 0},	
	{" 76;"," FUEHR.-PU.FEST ","       ", P&hks[HK1].FuehrPuFest,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif

#else									// GENIbus-Pumpe
	{" 71;"," PU-BUS EIN ?   "," 1=JA  ", P&hks[HK1].GeniPuFunktion,							US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*71;"," PU-BUS         ","       ", P&hkd[HK1].geniPuFunktion,				 			US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*71:"," PU-BUS         ","       ", P&hkd[HK1].geniPuFunktion,				 PU_FUNC_FORM, 0, P&vis,	V0, 0, 0},
	{"*->;"," pu-nummer      ","       ", P&hks[HK1].GeniPuNummer,								US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," pu-adresse     ","       ", P&hks[HK1].GeniPuAdresse,								US_CHAR, 0, P&hid2,	V0, 0, 0},
	{"*72;"," PU-BETRIEB SOLL"," 0..3  ", P&hkd[HK1].geniPuBetriebsartSoll,	 			US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*72:"," PU-BETRIEB SOLL","       ", P&hkd[HK1].geniPuBetriebsartSoll,	  PU_OPR_FORM, 0, P&vis,	V0, 0, 0},
	{" 73;"," PU-REG.ART AUTO"," 0..3  ", P&hks[HK1].GeniPuRegelart,							US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*73;"," PU-REG.ART SOLL"," 0..3  ", P&hkd[HK1].geniPuRegelartSoll, 		 			US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*73:"," PU-REG.ART SOLL","       ", P&hkd[HK1].geniPuRegelartSoll, 		  PU_CTR_FORM, 0, P&vis,	V0, 0, 0},
	{"*74:"," PU-SOLLWERT AKT"," %     ", P&hkd[HK1].geniPuSollwert,		 		 	 		 US_INT, 1, P&vis,	V1, 0, 0},
	{" 74;"," PU-SOLL NORMAL "," %     ", P&hks[HK1].GeniPuSollwert,		 		 	 		 US_INT, 1, P&hid1,	V1, 0, 0},
	{" 74;"," PU-SOLL ABSENK "," %     ", P&hks[HK1].GeniPuSollwertAbsenk,		 		 US_INT, 1, P&hid1,	V1, 0, 0},

	{"*75;"," PU-BETRIEB IST "," 0..3  ", P&hkd[HK1].geniPuBetriebsartIst,				US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*75:"," PU-BETRIEB IST ","       ", P&hkd[HK1].geniPuBetriebsartIst, 	  PU_OPR_FORM, 0, P&vis,	V0, 0, 0},
	{"*76;"," PU-REG.ART IST "," 0..3  ", P&hkd[HK1].geniPuRegelartIst,						US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*76:"," PU-REG.ART IST ","       ", P&hkd[HK1].geniPuRegelartIst,	 		  PU_CTR_FORM, 0, P&vis,	V0, 0, 0},
	{"*77:"," PU-DREHZAHL    "," 1/min ", P&hkd[HK1].geniPuDrehz, 						 		 US_INT, 0, P&vis,	V1, 0, 0},
	{"*77:"," PU-FOERDERHOEHE"," m     ", P&hkd[HK1].geniPuHoehe, 						 		 US_INT, 1, P&vis,	V1, 0, 0},
	{"*77:"," PU-FOERDERSTROM"," m/h  ", P&hkd[HK1].geniPuFluss , 						 		 US_INT, 1, P&vis,	V1, 0, 0},
	{"*78:"," PU-MEDIEN-TEMP."," C    ", P&hkd[HK1].geniPuTemp, 						 				S_INT, 1, P&vis,	V1, 0, 0},	
	{"*78:"," PU-LEISTUNG    "," W     ", P&hkd[HK1].geniPuLeist, 						 		 US_INT, 0, P&vis,	V1, 0, 0},
	{"*->;"," PU-ENERGIEVERBR"," kWh   ", P&hkd[HK1].geniPuEnerg, 						 		 US_INT, 0, P&hid1,	V1, 0, 0},
	{"*->;"," PU-BETRIEBSZEIT"," h     ", P&hkd[HK1].geniPuLz, 						 		 		 US_INT, 0, P&hid1,	V1, 0, 0},
	{"*79:"," PU-ALARM-CODE  ","       ", P&hkd[HK1].geniPuAlarmcode, 						US_CHAR, 0, P&vis,	V1, 0, 0},
//	{" 79:"," PU-ALARM QUIT.?"," 1=JA  ", P&hkd[HK1].geniPuAlarmQuit, 						US_CHAR, 0, P&vis,	V1, 0, 0},

	{" ->;"," PU-RM VERZOEG. "," min   ", P&hks[HK1].GeniPuBetriebSmVerz,					US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," pu-rm verz.cnt "," min/2 ", P&hkd[HK1].geniPuBetriebSmCtr,					 US_INT, 0, P&hid2,	V0, 0, 0},
	
#endif	

	#if SWAUS_HK1 == 1	// Anlagenschalter für HK-Nichtnutzung
	{"*78:"," SCHALT.HK-AUS ?","       ", P&HKAUS[HK1], 	 	 JANEIN_FORMIP, 0, P&vis,		E1, EREIGNIS, 0},
	#endif

	{"*80:"," BETRIEBS-REGIE ","       ", P&hkd[HK1].regie_txt, DYN_ASCII_FORM, 0, P&vis,		V1, 0, 0},
	{"*81:"," ABSENKPHASE ?  ","       ", P&hkd[HK1].absen,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*->:"," ABSCHALTPHASE ?","       ", P&hkd[HK1].absch,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#if TRAUMK_HK1==1 || BEDRAUM_HK1==1
	{"*->:"," STUETZBETRIEB ?","       ", P&hkd[HK1].stuetz,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif
	{"*->:"," FROSTBETRIEB ? ","       ", P&hkd[HK1].frost,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*82:"," AUFHEIZPHASE ? ","       ", P&hkd[HK1].aufhe,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*83:"," SOMMERBETRIEB ?","       ", P&hkd[HK1].somme,	 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->:"," absenkwert abs "," K     ", P&abs_ram[HK1].tvab,			US_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," absenkzeit abs "," s/10  ", P&abs_ram[HK1].zasd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," abschalt   abs "," s/10  ", P&abs_ram[HK1].zAbsch,		US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," aufheiz    abs "," s/10  ", P&abs_ram[HK1].zahd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," absenkwert bed "," K     ", P&abs_bed[HK1].tvab,			US_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," absenkzeit bed "," s/10  ", P&abs_bed[HK1].zasd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," abschalt   bed "," s/10  ", P&abs_bed[HK1].zAbsch,		US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," aufheiz    bed "," s/10  ", P&abs_bed[HK1].zahd,			US_INT, 0, P&hid2,	V0, 0, 0},

	#if WWANZ > 0
	{"*84:"," WW-VORRANG ?   ","       ", P&hkd[HK1].vorra,	 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#else
	{" ->:"," ww-vorrang ?   ","       ", P&hkd[HK1].vorra,	 	 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	#endif

	#if TRLBEG_HK1==1
	{"*85:"," RUECKL.BEGR. ? ","       ", P&hkd[HK1].rlbegr,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif

//-	{"*86:"," SM REGELABW.?  ","       ", P&hkd[HK1].abwAlarm, JANEIN_FORM, 0, P&vis, 	A1, EINZEL,   0},

	#if STW_HK1==1
	{"*87:"," SM STW ?       ","       ", P&STWHK[HK1], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
	#endif

	#if PUAL_HK1 == 1		// Störmeldung Pumpe vorhanden
		#if PUDO_HK1 == 1
	{"*88:"," SM PUMPE 1 ?   ","       ", P&PUAL[HK1], 			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
	{"*88:"," SM PUMPE 2 ?   ","       ", P&PUDAL[HK1], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
		#else
	{"*88:"," SM HEIZ-PUMPE ?","       ", P&PUAL[HK1], 			 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL,   0},
		#endif
	#endif

	#if PUBM_HK1 == 1 	// Betriebsmeldung Pumpe vorhanden
	{"*88:"," SM BM-HEIZ-PU ?","       ", P&hkd[HK1].puBm,	 	JANEIN_FORM, 0, P&vis,		E1, STANDARD, 0},
	#endif

	#if GENIBUS_HK1	== 1
	{"*88:"," SM GENIBUS ?   ","       ", P&hkd[HK1].geniPuBusSm,			JANEIN_FORM, 0, P&vis, A1, EINZEL,   0},
	{"*88:"," SM RM GB-PUMPE?","       ", P&hkd[HK1].geniPuBetriebSm,	JANEIN_FORM, 0, P&vis, A1, EINZEL,   0},
	{"*88:"," SM GB-PUMPE ?  ","       ", P&hkd[HK1].geniPuAlarm,			JANEIN_FORM, 0, P&vis, A1, EINZEL,   0},
	#endif

	{"*89:"," HANDBETRIEB ?  ","       ", P&hks[HK1].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

//-	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&hks[HK1].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
//-	{" 91;"," D-VERSTAERKUNG ","       ", P&hks[HK1].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," tastzeit       "," s     ", P&hks[HK1].Ts,						US_INT, 1, P&hid2,	V0, 0, 0},
//-	{" 93;"," NACHSTELLZEIT  "," s     ", P&hks[HK1].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},
//-
//-	#if DREIP_HK1==1
//-	{" 94;"," VENTILHUB MAX  "," mm    ", P&hks[HK1].Hub,					 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{" 95;"," STELLGESCHWIND."," mm/min", P&hks[HK1].Vst,			 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{"*->:"," stellzeit ber. "," s     ", P&hkd[HK1].stellzeit,			 S_INT, 2, P&hid2,	V0, 0, 0},
//-	{"*->:"," stellsumme     "," s     ", P&hkd[HK1].stellsum,			 S_INT, 2, P&hid2,	V0, 0, 0},
//-	{"*->:"," fahrzeit       "," s     ", P&hkd[HK1].fahren,				 S_INT, 0, P&hid2,	V0, 0, 0},
//-	#else
//-	{" 94;"," VENTILSTELL.MIN"," %     ", P&hks[HK1].Y_rel_min,	 		US_INT, 1, P&hid1,	V1, 0, 0},
//-	{" 95;"," OEFFNUNGSBEGINN"," %     ", P&hks[HK1].Y_rel_beg,	 		US_INT, 1, P&hid1,	V1, 0, 0},
//-	#endif
//-
//-	{"*96;"," SOLLWERT       "," C    ", P&hkd[HK1].tsol,					US_INT, 1, P&hid1,	V1, 0, 0},
//-	{"*97;"," REGELABWEICHUNG"," K     ", P&hkd[HK1].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
//-	{"*->;"," dy_rel         ","       ", P&hkd[HK1].dy_rel,				 S_INT, 3, P&hid2,	V0, 0, 0},
//-
//-	#if DREIP_HK1==0
//-	{"*98;"," VENTILSTELLUNG "," %     ", P&RVENT[HK1],					AOUT_FORMP, 1, P&hid1,	V1, 0, 0},
//-	#endif

	//-------------------------------------------------------------------------------------------------
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&hks[HK1].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},

//#####ulsch
#if GENIBUS_HK1	== 0
	#if STRPU_HK1 == 0
		#if PUDO_HK1 == 1
	{" ->:"," PUMPE 1 EIN/AUS"," EIN=1 ", P&hks[HK1].Puea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," PUMPE 2 EIN/AUS"," EIN=1 ", P&hks[HK1].Pu2ea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
		#else		
	{" ->:"," PUMPE EIN/AUS  "," EIN=1 ", P&hks[HK1].Puea,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
		#endif	
	#endif
#else
	{" ->;"," PU-BETRIEBSART "," 0..3  ", P&hks[HK1].GeniPuBetriebsartHand,	US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," PU-REGELART    "," 0..3  ", P&hks[HK1].GeniPuRegelartHand,		US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," PU-SOLL SETZEN "," %     ", P&hks[HK1].GeniPuSollwertHand,		 US_INT, 1, P&hid1,	V1, 0, 0},
#endif

//-	#if DREIP_HK1==1
//-	{" ->:"," VENTIL ZUFAHREN"," s     ", P&hkd[HK1].vzu,				 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	{" ->:"," VENTIL AUFFAHRN"," s     ", P&hkd[HK1].vauf,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
//-	#else
//-	{" ->:"," VENTIL STELLEN "," %     ", P&hks[HK1].Vstell,			  US_INT, 1, P&hid1,	V1, 0, 0},
//-	#endif

};


/*------------------------------ Heizkurven-Adaption ----------------------------------------------*/
	#if ADAP_STAT_HK1 > 0
	const Pgrup ad1[] = {
		{"***:"," ADAPTION HK1   ","       ", P&Leer15,								  ASCII_FORM, 0, P&vis,		V1, 0, 0},
		{"*00:"," ADAPT.-MELDUNG ","       ", P&AdaptMeldg[HK1][0], DYN_ASCII_FORM, 0, P&vis,		V1, 0, 0},

	#if ADAP_QU_HK1 == 0
		#if ADAP_STAT_HK1 == 1
		{" ->;"," R50-Anford.Rx  "," V     ", P&mwAdapt[HK1][0],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#else
		#if	ADAP_STAT_HK1 > 1	
		{" ->;"," 1.R50-Anford.Rx"," V     ", P&mwAdapt[HK1][0],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif
		#endif
						
		#if ADAP_STAT_HK1 > 1
		{" ->;"," 2.R50-Anford.Rx"," V     ", P&mwAdapt[HK1][1],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif
		#if ADAP_STAT_HK1 > 2
		{" ->;"," 3.R50-Anford.Rx"," V     ", P&mwAdapt[HK1][2],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif
		#if ADAP_STAT_HK1 > 3
		{" ->;"," 4.R50-Anford.Rx"," V     ", P&mwAdapt[HK1][3],				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		#endif

	//### josch
		{" ->;"," adapt_countdown"," min   ", P&adapt_countdown[HK1],		 US_CHAR, 0, P&hid2,	V0, 0, 0},

		#if ADAP_STAT_HK1 == 1
		{"*01:"," R50-ANFORD.    "," V     ", P&mwAdaption[HK1][0],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*01:"," R50-ANFORD.    "," V     ", P&mwAdaption[HK1][0],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#else
		#if ADAP_STAT_HK1 > 1
		{"*01:"," 1.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][0],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*01:"," 1.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][0],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
		#endif
				
		#if ADAP_STAT_HK1 > 1
		{"*02:"," 2.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][1],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*02:"," 2.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][1],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
		#if ADAP_STAT_HK1 > 2
		{"*03:"," 3.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][2],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*03:"," 3.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][2],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
		#if ADAP_STAT_HK1 > 3
		{"*04:"," 4.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][3],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*04:"," 4.R50-ANFORD.  "," V     ", P&mwAdaption[HK1][3],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
		#endif
	#else
		{"*->;"," Anforderung ana"," V     ", P&ADAPT[HK1],						 ANA_FORMP, 2, P&hid2,	V0, 0, 0},
		{"*->;"," Anford.bin tief","       ", P&ADAPT_T[HK1],			 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},
		{"*->;"," Anford.bin hoch","       ", P&ADAPT_H[HK1],			 JANEIN_FORMIP, 0, P&hid2,	V0, 0, 0},

		{"*01:"," ANFORDERUNG    "," V     ", P&mwAdaption[HK1][0],			ANA_FORM, 1, P&vis,		V1, 0, 0},
		{"*01:"," ANFORDERUNG    "," V     ", P&mwAdaption[HK1][0],  ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

		{" 10:"," MESSBEGINN     "," Uhr   ", P&hks[HK1].MessBeginn,		 US_CHAR, 0, P&vis,		V1, 0, 0},
		{" 11:"," ADAPTIONS-WERT "," K     ", P&hks[HK1].AdaptWert,			 US_CHAR, 1, P&vis,		V1, 0, 0},
		{" 12:"," REGELABW.-GRENZ"," K     ", P&hks[HK1].EiMaxAdapt,		 US_CHAR, 1, P&vis,		V1, 0, 0},

		{" 15;"," KASKADE-MODUS  ","       ", P&hks[HK1].KaskMode,		 	 US_CHAR, 0, P&hid1,	V1, 0, 0},

		{" ->;"," Vorlauf zu tief","       ", P&tvsLow[HK1], 						 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Vorlauf normal ","       ", P&tvsNormal[HK1], 				 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Vorlauf zu hoch","       ", P&tvsHigh[HK1], 					 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Leistg. zu tief","       ", P&leistgLow[HK1],				 	 US_CHAR, 0, P&hid2,	V0, 0, 0},
		{" ->;"," Leistg. zu hoch","       ", P&leistgHigh[HK1],				 US_CHAR, 0, P&hid2,	V0, 0, 0},

		{"*20;"," ADAPT.Ta=-25C "," K     ", P&AdaptKorr[HK1][0], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*21;"," ADAPT.Ta=-20C "," K     ", P&AdaptKorr[HK1][1], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*22;"," ADAPT.Ta=-15C "," K     ", P&AdaptKorr[HK1][2], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*23;"," ADAPT.Ta=-10C "," K     ", P&AdaptKorr[HK1][3], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*24;"," ADAPT.Ta=-5C  "," K     ", P&AdaptKorr[HK1][4], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*25;"," ADAPT.Ta=0C   "," K     ", P&AdaptKorr[HK1][5], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*26;"," ADAPT.Ta=5C   "," K     ", P&AdaptKorr[HK1][6], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*27;"," ADAPT.Ta=10C  "," K     ", P&AdaptKorr[HK1][7], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*28;"," ADAPT.Ta=15C  "," K     ", P&AdaptKorr[HK1][8], 			 	S_INT, 1, P&hid1,	V1, 0, 0},
		{"*29;"," ADAPT.Ta=20C  "," K     ", P&AdaptKorr[HK1][9], 			 	S_INT, 1, P&hid1,	V1, 0, 0},

	};
	#endif
