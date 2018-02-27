/*------------------------------ Heizkreis 2 -------------------------------------------*/
const Pgrup hk2[] = {
//-------------------
	{"_**:"," HEIZKREIS 2    ","       ", P&hkd[HK2].raumname,	ASCII_FORM, 0, P&vis,		V1, 0, 0},
//##### Nur Anforderung
//-	{"*01:"," VORLAUF   IST  "," C    ", P&TVS[HK2],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
//-	{"*01:"," VORLAUF   IST  "," C    ", P&TVS[HK2],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},

	#if TRLBEG_HK2==1
	{"*02:"," RUECKLAUF IST  "," C    ", P&TRS[HK2],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*02:"," RUECKLAUF IST  "," C    ", P&TRS[HK2],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if TRAUMK_HK2==1
	{"*03:"," RAUM      IST  "," C    ", P&TI[HK2],						 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*03:"," RAUM      IST  "," C    ", P&TI[HK2],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif

	#if BEDARF_HK2==1 && BEDRAUM_HK2==1
	{" 04:"," RAUM-EXT. IST  "," C    ", P&hkd[HK2].tibed,					 S_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 04:"," raum-ext. ist  "," C    ", P&hkd[HK2].tibed,					 S_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	{" 06;"," SO/WI HK-EXTRA "," 1=JA  ", P&hks[HK2].SoWiExtra,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 07;"," Ta-WINTER [EIN]"," C    ", P&hks[HK2].Tae,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 08;"," Ta-SOMMER [AUS]"," C    ", P&hks[HK2].Taa,						US_INT, 1, P&hid1,	V1, 0, 0},
	{"*09;"," SOMMER ?       ","       ", P&hkd[HK2].sowi,		 JANEIN_FORM, 0, P&hid1,	V1, 0, 0},

	{" 10:"," STEIGUNG       ","       ", P&hks[HK2].Anst,					US_INT, 2, P&vis,		V1, 0, 0},
	{" 11:"," NIVEAU         "," C    ", P&hks[HK2].Tvpa,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 12:"," VORLAUF   MAX  "," C    ", P&hks[HK2].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 13:"," VORLAUF   MIN  "," C    ", P&hks[HK2].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},

	#if TRAUMK_HK2==1
	{" 14:"," RAUM      SOLL "," C    ", P&hks[HK2].Tiso,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 14:"," raum      soll "," C    ", P&hks[HK2].Tiso,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if BEDARF_HK2==1 && BEDRAUM_HK2==1
	{" 14:"," RAUM-EXT. SOLL "," C    ", P&hkd[HK2].tisobed,				US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 14:"," raum-ext. soll "," C    ", P&hkd[HK2].tisobed,				US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if TRAUMK_HK2==1
	{" 15:"," RAUM      MIN  "," C    ", P&hks[HK2].Timi,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" 15:"," raum      min  "," C    ", P&hks[HK2].Timi,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if TRAUMK_HK2==1 || (BEDARF_HK2==1 && BEDRAUM_HK2==1)
	{" ->;"," RAUM MIN HYST. "," K     ", P&hks[HK2].TimiHyst,			US_INT, 1, P&hid1,	V0, 0, 0},
	#else
	{" ->;"," raum min hyst. "," K     ", P&hks[HK2].TimiHyst,			US_INT, 1, P&hid2,	V0, 0, 0},	
	#endif

	#if TRAUMK_HK2==1 || (BEDARF_HK2==1 && BEDRAUM_HK2==1)
	{" 16;"," KASK.SOLL MAX  "," C    ", P&hks[HK2].KaskBegr,			US_INT, 1, P&hid1,	V0, 0, 0},
	#else
	{" 16;"," kask.soll max  "," C    ", P&hks[HK2].KaskBegr,			US_INT, 1, P&hid2,	V0, 0, 0},	
	#endif
	
	#if (BEDARF_HK2==1 && BEDRAUM_HK2==1) || TRAUMK_HK2==1
	{" 17:"," Xp-RAUM KASKADE","       ", P&hks[HK2].Kpk,						US_INT, 2, P&vis,		V1, 0, 0},
	#else
	{" 17;"," xp-raum kaskade","       ", P&hks[HK2].Kpk,						US_INT, 2, P&hid2,	V0, 0, 0},
	#endif

	#if TRLBEG_HK2==1
	{" 18:"," RUECKLAUF MAX  "," C    ", P&hks[HK2].Trma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," Xp-RUECKL.BEGR.","       ", P&hks[HK2].Kpr,						US_INT, 2, P&vis,		V1, 0, 0},
	#endif

	{"*20:"," VORLAUF BERECHN"," C    ", P&hkd[HK2].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	{"*->:"," vorlauf ber.hkl"," C    ", P&hkd[HK2].tvsb_hkl,			US_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur adapt"," K     ", P&hkd[HK2].adaptKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur kaska"," K     ", P&hkd[HK2].kaskaKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{"*->:"," korrektur wwvor"," K     ", P&hkd[HK2].wwvorKorr,			 S_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," vorlauf filt.zk"," s     ", P&hks[HK2].Fzk,						US_INT, 1, P&hid2,	V0, 0, 0},

	#if FBH_HK2==1
	{" 25;"," ESTR.-PR.START ","       ", P&hks[HK2].StartAufheiz, DATE_FORM, 1, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG1 "," C    ", P&hks[HK2].EstrichTemp[0], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG2 "," C    ", P&hks[HK2].EstrichTemp[1], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG3 "," C    ", P&hks[HK2].EstrichTemp[2], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG4 "," C    ", P&hks[HK2].EstrichTemp[3], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG5 "," C    ", P&hks[HK2].EstrichTemp[4], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG6 "," C    ", P&hks[HK2].EstrichTemp[5], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG7 "," C    ", P&hks[HK2].EstrichTemp[6], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG8 "," C    ", P&hks[HK2].EstrichTemp[7], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG9 "," C    ", P&hks[HK2].EstrichTemp[8], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG10"," C    ", P&hks[HK2].EstrichTemp[9], US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG11"," C    ", P&hks[HK2].EstrichTemp[10],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG12"," C    ", P&hks[HK2].EstrichTemp[11],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG13"," C    ", P&hks[HK2].EstrichTemp[12],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG14"," C    ", P&hks[HK2].EstrichTemp[13],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG15"," C    ", P&hks[HK2].EstrichTemp[14],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG16"," C    ", P&hks[HK2].EstrichTemp[15],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG17"," C    ", P&hks[HK2].EstrichTemp[16],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG18"," C    ", P&hks[HK2].EstrichTemp[17],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG19"," C    ", P&hks[HK2].EstrichTemp[18],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG20"," C    ", P&hks[HK2].EstrichTemp[19],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG21"," C    ", P&hks[HK2].EstrichTemp[20],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG22"," C    ", P&hks[HK2].EstrichTemp[21],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG23"," C    ", P&hks[HK2].EstrichTemp[22],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG24"," C    ", P&hks[HK2].EstrichTemp[23],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG25"," C    ", P&hks[HK2].EstrichTemp[24],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG26"," C    ", P&hks[HK2].EstrichTemp[25],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG27"," C    ", P&hks[HK2].EstrichTemp[26],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG28"," C    ", P&hks[HK2].EstrichTemp[27],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG29"," C    ", P&hks[HK2].EstrichTemp[28],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," ESTR.TEMP.TAG30"," C    ", P&hks[HK2].EstrichTemp[29],US_CHAR, 0, P&hid1,	V0, 0, 0},
	{"*26:"," ESTRICH-PROGR.?","       ", P&hkd[HK2].estrichProg,JANEIN_FORM, 0, P&vis, 	E1, STANDARD, 0},
	{"*27;"," ESTRICH AKT.TAG","       ", P&hkd[HK2].estrichTag,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*27;"," ESTR.AKT.SOLLT."," C    ", P&hkd[HK2].estrichTemp,			US_INT, 0, P&hid1,	V0, 0, 0},
	#endif

	#if ( ANFORD > 0 || NTANZ > 0 || KEANZ > 0 )
	{" 28:"," ANFORD.ANHEBUNG"," K     ", P&hks[HK2].TvpAnh,				US_INT, 1, P&vis,		V1, 0, 0},
	#endif
	
	#if BEDARF_HK2==1
	{" 30;"," ABSENK-BEGIN WO"," h:min ", P&abs_tab[HK2][0].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" 31;"," ABSENK-DAUER WO"," h     ", P&abs_tab[HK2][0].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" 32;"," ABSENK-WERT  WO"," K     ", P&abs_tab[HK2][0].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},
	{" 34;"," GANZE WOCHE SET"," 1=JA  ", P&setwoche[HK2],				 US_CHAR, 0, P&hid1,		V1, 0, 0},

	{" 35;"," ABSENK-BEGIN Mo"," h:min ", P&abs_tab[HK2][1].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Mo"," h     ", P&abs_tab[HK2][1].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Mo"," K     ", P&abs_tab[HK2][1].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 36;"," ABSENK-BEGIN Di"," h:min ", P&abs_tab[HK2][2].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Di"," h     ", P&abs_tab[HK2][2].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Di"," K     ", P&abs_tab[HK2][2].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 37;"," ABSENK-BEGIN Mi"," h:min ", P&abs_tab[HK2][3].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Mi"," h     ", P&abs_tab[HK2][3].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Mi"," K     ", P&abs_tab[HK2][3].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 38;"," ABSENK-BEGIN Do"," h:min ", P&abs_tab[HK2][4].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Do"," h     ", P&abs_tab[HK2][4].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Do"," K     ", P&abs_tab[HK2][4].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 39;"," ABSENK-BEGIN Fr"," h:min ", P&abs_tab[HK2][5].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Fr"," h     ", P&abs_tab[HK2][5].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Fr"," K     ", P&abs_tab[HK2][5].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 40;"," ABSENK-BEGIN Sa"," h:min ", P&abs_tab[HK2][6].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER Sa"," h     ", P&abs_tab[HK2][6].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  Sa"," K     ", P&abs_tab[HK2][6].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},

	{" 41;"," ABSENK-BEGIN So"," h:min ", P&abs_tab[HK2][7].begin,ZEIT_FORM,0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-DAUER So"," h     ", P&abs_tab[HK2][7].dauer, US_CHAR, 0, P&hid1,		V1, 0, 0},
	{" ->;"," ABSENK-WERT  So"," K     ", P&abs_tab[HK2][7].abwert,	US_INT, 1, P&hid1,		V1, 0, 0},
	
	#else
	{" 30:"," ABSENK-BEGIN WO"," h:min ", P&abs_tab[HK2][0].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" 31:"," ABSENK-DAUER WO"," h     ", P&abs_tab[HK2][0].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" 32:"," ABSENK-WERT  WO"," K     ", P&abs_tab[HK2][0].abwert,	US_INT, 1, P&vis,		V1, 0, 0},
	{" 34:"," GANZE WOCHE SET"," 1=JA  ", P&setwoche[HK2],				 US_CHAR, 0, P&vis,		V1, 0, 0},

	{" 35:"," ABSENK-BEGIN Mo"," h:min ", P&abs_tab[HK2][1].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Mo"," h     ", P&abs_tab[HK2][1].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Mo"," K     ", P&abs_tab[HK2][1].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 36:"," ABSENK-BEGIN Di"," h:min ", P&abs_tab[HK2][2].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Di"," h     ", P&abs_tab[HK2][2].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Di"," K     ", P&abs_tab[HK2][2].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 37:"," ABSENK-BEGIN Mi"," h:min ", P&abs_tab[HK2][3].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Mi"," h     ", P&abs_tab[HK2][3].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Mi"," K     ", P&abs_tab[HK2][3].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 38:"," ABSENK-BEGIN Do"," h:min ", P&abs_tab[HK2][4].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Do"," h     ", P&abs_tab[HK2][4].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Do"," K     ", P&abs_tab[HK2][4].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 39:"," ABSENK-BEGIN Fr"," h:min ", P&abs_tab[HK2][5].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Fr"," h     ", P&abs_tab[HK2][5].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Fr"," K     ", P&abs_tab[HK2][5].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 40:"," ABSENK-BEGIN Sa"," h:min ", P&abs_tab[HK2][6].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER Sa"," h     ", P&abs_tab[HK2][6].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  Sa"," K     ", P&abs_tab[HK2][6].abwert,	US_INT, 1, P&vis,		V1, 0, 0},

	{" 41:"," ABSENK-BEGIN So"," h:min ", P&abs_tab[HK2][7].begin,ZEIT_FORM,0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-DAUER So"," h     ", P&abs_tab[HK2][7].dauer, US_CHAR, 0, P&vis,		V1, 0, 0},
	{" ->:"," ABSENK-WERT  So"," K     ", P&abs_tab[HK2][7].abwert,	US_INT, 1, P&vis,		V1, 0, 0},
	#endif
	
//#####ulsch : Ferienautomatik #####
	#if FER == 1
	{" 42:"," FERIENAUTOMATIK"," JA=1  ", P&hks[HK2].FerienAutom,	 		US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 43:"," FER.ABSENK-WERT"," K     ", P&hks[HK2].FerienAbsenk,		 US_INT, 1, P&vis,	V1, 0, 0},
	{"*->;"," ferienbetrieb ?","       ", P&hkd[HK2].absenFerien,	JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	#endif

	#if BEDARF_HK2==1
	{" 44:"," BEDARFS-ABSENK."," K     ", P&hks[HK2].Beabs,					US_INT, 1, P&vis,		V1, 0, 0},
	#else
	{" ->;"," bedarfs-absenk."," K     ", P&hks[HK2].Beabs,					US_INT, 1, P&hid2,	V0, 0, 0},
	#endif

	#if BEDRAUM_HK2 == 1
	{" 45:"," RAUMABSCHALTUNG"," JA=1  ", P&hks[HK2].AbschRaumanf,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 46:"," VENTILE AUSWERT"," JA=1  ", P&hks[HK2].VentilAuswert,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{" 47:"," VENT_AUF ABSCH."," %     ", P&hks[HK2].VentOffenMax,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{"*->;"," ventile offen  ","       ", P&hkd[HK2].ventOffen,				US_INT, 0, P&hid2,V0, 0, 0},
	{"*->;"," ventile gesamt ","       ", P&hkd[HK2].ventGesamt,			US_INT, 0, P&hid2,V0, 0, 0},
	{"*48:"," VENTILE OFFEN  "," %     ", P&hkd[HK2].ventOffenBer,	 US_CHAR, 0, P&vis,	V1, 0, 0},
	{"*49:"," RAUMABSCHALTG.?","       ", P&hkd[HK2].raumAbsch, JAFRAGE_FORM, 0, P&vis,	V1, 0, 0},
	#endif

	{" 50:"," HEIZ-TaGRENZE  "," C    ", P&hks[HK2].TaHeizgrenze,		 S_INT, 1, P&vis,	V1, 0, 0},
	{" 51:"," FROST-TaGRENZE "," C    ", P&hks[HK2].TaFrostgrenze,		 S_INT, 1, P&vis,	V1, 0, 0},
	{" 52:"," ABSCH-ZEITKONST"," h/K   ", P&hks[HK2].AbschZeitKonst,	US_INT, 1, P&vis,	V1, 0, 0},

	#if LEIBEGR_HK2 == 1
	{" 55;"," LEIST.BEGR/Ts  "," %     ", P&hks[HK2].LeistBegrTs,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif

	{" 60:"," SCHNELLAUFHEIZ."," JA=1  ", P&hks[HK2].Sauf,				 US_CHAR, 0, P&vis,		V1, 0, 0},
	{" 61:"," SO/WI-AUTOMATIK"," JA=1  ", P&hks[HK2].Swhk,				 US_CHAR, 0, P&vis,		V1, 0, 0},

	#if SWAUS_HK2 == 1	// Anlagenschalter für HK-Nichtnutzung
	{" 64:"," HKAUS-ABSENKW. "," K     ", P&hks[HK2].SchalterAbsenk,US_INT, 1, P&vis,		V1, 0, 0},
	#endif

	#if BEDARF_HK2==1
	{"*70:"," HEIZ-BEDARF ?  ","       ", P&hkd[HK2].bedarf,	JAFRAGE_FORM, 0, P&vis,		V1, 0, 0},
	#endif
	{" ->:"," bedarf         "," 1J 2N ", P&hkd[HK2].bedarf,	 		 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," vorheiz_bed    ","       ", P&hkd[HK2].vorheiz_bed,	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," ticopy         ","       ", P&hkd[HK2].ticopy,			 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->:"," CountDown      ","       ", P&hkd[HK2].CountDown,  	 US_CHAR, 0, P&hid2,	V0, 0, 0},
	{" ->;"," vorrang bedarf ","       ", P&hkd[HK2].vorra_bed,		 US_CHAR, 0, P&hid2,	V0, 0, 0},

	#if SWAUS_HK2 == 1	// Anlagenschalter für HK-Nichtnutzung
	{"*78:"," SCHALT.HK-AUS ?","       ", P&HKAUS[HK2], 	 	 JANEIN_FORMIP, 0, P&vis,		E1, EREIGNIS, 0},
	#endif

	{"*80:"," BETRIEBS-REGIE ","       ", P&hkd[HK2].regie_txt, DYN_ASCII_FORM, 0, P&vis,		V1, 0, 0},
	{"*81:"," ABSENKPHASE ?  ","       ", P&hkd[HK2].absen,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*->:"," ABSCHALTPHASE ?","       ", P&hkd[HK2].absch,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#if TRAUMK_HK2==1 || BEDRAUM_HK2==1
	{"*->:"," STUETZBETRIEB ?","       ", P&hkd[HK2].stuetz,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif
	{"*->:"," FROSTBETRIEB ? ","       ", P&hkd[HK2].frost,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*82:"," AUFHEIZPHASE ? ","       ", P&hkd[HK2].aufhe,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*83:"," SOMMERBETRIEB ?","       ", P&hkd[HK2].somme,	 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{" ->:"," absenkwert abs "," K     ", P&abs_ram[HK2].tvab,			US_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," absenkzeit abs "," s/10  ", P&abs_ram[HK2].zasd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," abschalt   abs "," s/10  ", P&abs_ram[HK2].zAbsch,		US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," aufheiz    abs "," s/10  ", P&abs_ram[HK2].zahd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," absenkwert bed "," K     ", P&abs_bed[HK2].tvab,			US_INT, 1, P&hid2,	V0, 0, 0},
	{" ->:"," absenkzeit bed "," s/10  ", P&abs_bed[HK2].zasd,			US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," abschalt   bed "," s/10  ", P&abs_bed[HK2].zAbsch,		US_INT, 0, P&hid2,	V0, 0, 0},
	{" ->:"," aufheiz    bed "," s/10  ", P&abs_bed[HK2].zahd,			US_INT, 0, P&hid2,	V0, 0, 0},

	#if WWANZ > 0
	{"*84:"," WW-VORRANG ?   ","       ", P&hkd[HK2].vorra,	 	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#else
	{" ->:"," ww-vorrang ?   ","       ", P&hkd[HK2].vorra,	 	 JANEIN_FORM, 0, P&hid2,	V0, 0, 0},
	#endif

	#if STW_HK2==1
	{"*87:"," SM STW ?       ","       ", P&STWHK[HK2], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif

};

