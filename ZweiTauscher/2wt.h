const Pgrup ntr[] = {
	{"***:"," NETZTRENNUNG   ","       ", P&Ntmod,							ASCII_FORM, 0, P&vis,		V1, 0, 0},

	{"*01:"," VL FERNWAERME  "," C    ", P&TVP[ANL],						ANA_FORMP, 	1, P&vis,		V1, 0, 0},      
	{"*01:"," VL FERNWAERME  "," C    ", P&TVP[ANL],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*02:"," RL FERNWAERME  "," C    ", P&TRP[ANL],						ANA_FORMP, 	1, P&vis,		V1, 0, 0},      
	{"*02:"," RL FERNWAERME  "," C    ", P&TRP[ANL],					ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*03:"," VORLAUF PRIM.  "," C    ", P&TP_UNI[U1],					ANA_FORMP, 	1, P&vis,		V1, 0, 0},      
	{"*03:"," VORLAUF PRIM.  "," C    ", P&TP_UNI[U1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*04:"," RUECKLAUF PRIM."," C    ", P&TP_UNI[U2],					ANA_FORMP, 	1, P&vis,		V1, 0, 0},      
	{"*04:"," RUECKLAUF PRIM."," C    ", P&TP_UNI[U2],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*05:"," VORLAUF SEK.   "," C    ", P&TVSGNT[NT1],				ANA_FORMP, 	1, P&vis,		V1, 0, 0},      
	{"*05:"," VORLAUF SEK.   "," C    ", P&TVSGNT[NT1],			ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*06:"," RUECKLAUF SEK. "," C    ", P&TRSGNT[NT1],				ANA_FORMP, 	1, P&vis,		V1, 0, 0},      
	{"*06:"," RUECKLAUF SEK. "," C    ", P&TRSGNT[NT1],			ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},

	{"*20:"," VORLAUF ANFORD."," C    ", P&ntd[0].tvsb_anl,				US_INT, 1, P&vis,		V1, 0, 0},

	{"*21;"," SW-ANHEB.VAR.  "," K     ", P&ntd[0].y_rel_anh,				 S_INT, 1, P&hid1,	V1, 0, 0},	
	{" 22;"," SW-ANHEB.KONST."," K     ", P&nts[0].Tvpa,				   	 S_INT, 1, P&hid1,	V1, 0, 0},	

	{"*23:"," SOLLTEMP.UST1  "," C    ", P&anfExt[0],						ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*23:"," SOLLTEMP.UST1  "," C    ", P&anfExt[0],				 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*24:"," SOLLTEMP.UST2  "," C    ", P&anfExt[1],						ANA_FORM, 1, P&vis,		V1, 0, 0},
	{"*24:"," SOLLTEMP.UST2  "," C    ", P&anfExt[1],				 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},

	{"*50:"," MODULATION MW  "," %     ", P&ntd[0].modulatMW,				US_INT, 1, P&vis,		V1, 0, 0},
	{" 51;"," MOD.2.WT EIN   "," %     ", P&nts[0].WtEinMod,				US_INT, 1, P&hid1,	V1, 0, 0},	
	{" 51;"," VERZ.MOD.WT.EIN"," min   ", P&nts[0].WtEinModVerz, 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" 52;"," MOD.2.WT AUS   "," %     ", P&nts[0].WtAusMod,				US_INT, 1, P&hid1,	V1, 0, 0},	
	{" 52;"," VERZ.MOD.WT.AUS"," min   ", P&nts[0].WtAusModVerz,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," verz.mod.wt.ctr"," s     ", P&ntd[0].wtModCtr,			 	 S_INT, 0, P&hid2,	V0, 0, 0},
	
//-	{" 60;"," EIN FOLGE-WT   "," K*min ", P&nts[0].Einkrit,				 	 S_INT, 1, P&hid1,	V1, 0, 0},
//-	{" 61;"," AUS FOLGE-WT   "," K*min ", P&nts[0].Auskrit,				 	 S_INT, 1, P&hid1,	V1, 0, 0},		
	{" 62;"," SCHALTDIFFERENZ"," K     ", P&nts[0].WtSchaltDiff,		US_INT, 1, P&hid1,	V1, 0, 0},	
	{" 63;"," DIFFmin VL-RL  "," K     ", P&nts[0].FolgeWtAus,		  US_INT, 1, P&hid1,	V1, 0, 0},
//-	{" ->;"," diff soll-ist  "," K     ", P&ntd[0].abw,					 		 S_INT, 1, P&hid2,	V0, 0, 0},
//-	{" ->;"," diff-summe     "," K*min ", P&ntd[0].abwsumme,			 	 S_INT, 1, P&hid2,	V0, 0, 0},

	{" 70:"," FUEHR.WT.AKT.  ","       ", P&nts[0].FuehrWt,			 	 US_CHAR, 0, P&vis,	  V1, 0, 0},	
	{" 71;"," FUEHR.WT.WECHS."," h     ", P&nts[0].WtWechseln,			US_INT, 0, P&hid1,	V1, 0, 0},	
	{" 72;"," FUEHR.WT.FEST  ","       ", P&nts[0].FuehrWtFest,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	{" 90;"," P-VERST. ANH.  "," %/K   ", P&nts[0].Kp_anh,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERST. ANH.  ","       ", P&nts[0].Kd_anh,					US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT ANH.  "," s     ", P&nts[0].Ts_anh,					US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZ.ANH."," s     ", P&nts[0].Tn_anh,					US_INT, 1, P&hid1,	V1, 0, 0},

};


const Pgrup wt1[] = {
	{"***:"," TAUSCHER 1     ","       ", P&Leer15,							ASCII_FORM, 0, P&vis,		V1, 0, 0},

	{"*03:"," VORLAUF SEKUND."," C    ", P&TVSEK[NT1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*03:"," VORLAUF SEKUND."," C    ", P&TVSEK[NT1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*04:"," RUECKLAUF PRIM."," C    ", P&TRPNT[NT1],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*04:"," RUECKLAUF PRIM."," C    ", P&TRPNT[NT1],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	
	{" 12:"," VORLAUF WT MAX "," C    ", P&nts[NT1].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 13:"," VORLAUF WT MIN "," C    ", P&nts[NT1].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 18:"," RUECKLAUF MAX  "," C    ", P&nts[NT1].Trma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," Xp-RUECKL.BEGR.","       ", P&nts[NT1].Kpr,						US_INT, 2, P&vis,		V1, 0, 0},
	{"*20:"," VORLAUF BERECHN"," C    ", P&ntd[NT1].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},
	
	{"*60:"," TAUSCHER EIN ? ","       ", P&ntd[NT1].wtEin,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},

	{" 62;"," TAUSCHER-LAUFZ."," h     ", P&ntd[NT1].lzStd,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," tauscher-laufz."," min/2 ", P&ntd[NT1].lzMin,				 US_CHAR, 0, P&hid2,	V0, 0, 0},
	
	{" 68;"," TDIFF.ALARM EIN"," K     ", P&nts[NT1].TabwMax,			 US_CHAR, 1, P&hid1,	V1, 0, 0},
	{" 69;"," ZVERZ.ALARM EIN"," min   ", P&nts[NT1].TabwZeit,		 US_CHAR, 0, P&hid1,	V1, 0, 0},

	{"*70:"," KLAPPE AUF ?   ","       ", P&KLNTAZ[NT1],	 	 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{" 71;"," KLAPPE-NACHLAUF"," min   ", P&nts[NT1].KlappeVerz,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*->;"," nachlauf-ctr   "," s     ", P&ntd[NT1].klappeCtr,	 		US_INT, 0, P&hid2,	V0, 0, 0},

	{"*77:"," RM VENTIL      "," %     ", P&anaInp[U1].mwSkal,			 ANA_FORM, 1, P&vis,	V1, 0, 0},
	{"*77:"," RM VENTIL      "," %     ", P&anaInp[U1].mwSkal, 	  ANA_FORM, 0x81, P&kom,	V1, 0, 0},
	{" ->;"," 0V RM VENTIL   "," %     ", P&AnaInpPara[U1].Skal0,		 		S_INT, 1, P&hid1,	V0, 0, 0},	
	{" ->;"," 10V RM VENTIL  "," %     ", P&AnaInpPara[U1].Skal10,	 		S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm ventil   "," s     ", P&AnaInpPara[U1].ZkFilt,		 US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm ventil      "," V     ", P&AE_UNI[U1],					  	ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm ventil      "," %     ", P&anaInp[U1].mwFilt,				 US_INT, 1, P&hid2,	V0, 0, 0},	

	{"*85:"," RUECKL.BEGR. ? ","       ", P&ntd[NT1].rlbegr,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*86:"," SM REGELABW.?  ","       ", P&ntd[NT1].abwAlarm, JANEIN_FORM, 0, P&vis, 	A1, EINZEL, 0},
	{"*87:"," SM STW ?       ","       ", P&STWNT[NT1], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},

	{"*89:"," HANDBETRIEB ?  ","       ", P&nts[NT1].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&nts[NT1].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&nts[NT1].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&nts[NT1].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&nts[NT1].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," vorlauf filt.zk"," s     ", P&nts[NT1].Fzk,						US_INT, 1, P&hid2,	V0, 0, 0},

	{"*96;"," SOLLWERT       "," C    ", P&ntd[NT1].tsol,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ntd[NT1].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," VENTILSTELLUNG "," %     ", P&RVENTNT[NT1],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},

	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&nts[NT1].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},

	{" ->:"," VENTIL STELLEN "," %     ", P&nts[NT1].Vstell,			  US_INT, 1, P&hid1,	V1, 0, 0},
	{" ->:"," KLAPPE AUF/ZU  "," AUF=1 ", P&nts[NT1].KlAZ,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	
};


const Pgrup wt2[] = {
	{"***:"," TAUSCHER 2     ","       ", P&Leer15,							ASCII_FORM, 0, P&vis,		V1, 0, 0},

	{"*03:"," VORLAUF SEKUND."," C    ", P&TVSEK[NT2],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*03:"," VORLAUF SEKUND."," C    ", P&TVSEK[NT2],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*04:"," RUECKLAUF PRIM."," C    ", P&TRPNT[NT2],					 ANA_FORMP, 1, P&vis,		V1, 0, 0},      
	{"*04:"," RUECKLAUF PRIM."," C    ", P&TRPNT[NT2],				ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	
	{" 12:"," VORLAUF WT MAX "," C    ", P&nts[NT2].Tvma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 13:"," VORLAUF WT MIN "," C    ", P&nts[NT2].Tvmi,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 18:"," RUECKLAUF MAX  "," C    ", P&nts[NT2].Trma,					US_INT, 1, P&vis,		V1, 0, 0},
	{" 19:"," Xp-RUECKL.BEGR.","       ", P&nts[NT2].Kpr,						US_INT, 2, P&vis,		V1, 0, 0},
	{"*20:"," VORLAUF BERECHN"," C    ", P&ntd[NT2].tvsb,					US_INT, 1, P&vis,		V1, 0, 0},

	{"*60:"," TAUSCHER EIN ? ","       ", P&ntd[NT2].wtEin,		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},

	{" 62;"," TAUSCHER-LAUFZ."," h     ", P&ntd[NT2].lzStd,					US_INT, 0, P&hid1,	V1, 0, 0},
	{" ->;"," tauscher-laufz."," min/2 ", P&ntd[NT2].lzMin,				 US_CHAR, 0, P&hid2,	V0, 0, 0},
	
	{" 68;"," TDIFF.ALARM EIN"," K     ", P&nts[NT2].TabwMax,			 US_CHAR, 1, P&hid1,	V1, 0, 0},
	{" 69;"," ZVERZ.ALARM EIN"," min   ", P&nts[NT2].TabwZeit,		 US_CHAR, 0, P&hid1,	V1, 0, 0},

	{"*70:"," KLAPPE AUF ?   ","       ", P&KLNTAZ[NT2],	 	 JANEIN_FORMOP, 0, P&vis,		V1, 0, 0},
	{" 71;"," KLAPPE-NACHLAUF"," min   ", P&nts[NT2].KlappeVerz,	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{"*->;"," nachlauf-ctr   "," s     ", P&ntd[NT2].klappeCtr,	 		US_INT, 0, P&hid2,	V0, 0, 0},

	{"*77:"," RM VENTIL      "," %     ", P&anaInp[U2].mwSkal,			 ANA_FORM, 1, P&vis,	V1, 0, 0},
	{"*77:"," RM VENTIL      "," %     ", P&anaInp[U2].mwSkal, 	  ANA_FORM, 0x81, P&kom,	V1, 0, 0},
	{" ->;"," 0V RM VENTIL   "," %     ", P&AnaInpPara[U2].Skal0,		 		S_INT, 1, P&hid1,	V0, 0, 0},	
	{" ->;"," 10V RM VENTIL  "," %     ", P&AnaInpPara[U2].Skal10,	 		S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.rm ventil   "," s     ", P&AnaInpPara[U2].ZkFilt,		 US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," rm ventil      "," V     ", P&AE_UNI[U2],					  	ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," rm ventil      "," %     ", P&anaInp[U2].mwFilt,				 US_INT, 1, P&hid2,	V0, 0, 0},	

	{"*85:"," RUECKL.BEGR. ? ","       ", P&ntd[NT2].rlbegr,	 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*86:"," SM REGELABW.?  ","       ", P&ntd[NT2].abwAlarm, JANEIN_FORM, 0, P&vis, 	A1, EINZEL, 0},
	{"*87:"," SM STW ?       ","       ", P&STWNT[NT2], 		 JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},

	{"*89:"," HANDBETRIEB ?  ","       ", P&nts[NT2].Haut,		 JANEIN_FORM, 0, P&vis,		E1, EREIGNIS, 0},

	{" 90;"," P-VERSTAERKUNG "," %/K   ", P&nts[NT2].Kp,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 91;"," D-VERSTAERKUNG ","       ", P&nts[NT2].Kd,						US_INT, 2, P&hid1,	V1, 0, 0},
	{" 92;"," TASTZEIT       "," s     ", P&nts[NT2].Ts,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" 93;"," NACHSTELLZEIT  "," s     ", P&nts[NT2].Tn,						US_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," vorlauf filt.zk"," s     ", P&nts[NT2].Fzk,						US_INT, 1, P&hid2,	V0, 0, 0},

	{"*96;"," SOLLWERT       "," C    ", P&ntd[NT2].tsol,					US_INT, 1, P&hid1,	V1, 0, 0},
	{"*97;"," REGELABWEICHUNG"," K     ", P&ntd[NT2].ei,						 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*98;"," VENTILSTELLUNG "," %     ", P&RVENTNT[NT2],				AOUT_FORMP, 1, P&hid1,	V1, 0, 0},

	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&nts[NT2].Haut,				 US_CHAR, 0, P&hid1,	V1, 0, 0},

	{" ->:"," VENTIL STELLEN "," %     ", P&nts[NT2].Vstell,			  US_INT, 1, P&hid1,	V1, 0, 0},
	{" ->:"," KLAPPE AUF/ZU  "," AUF=1 ", P&nts[NT2].KlAZ,		 		 US_CHAR, 0, P&hid1,	V1, 0, 0},

	
};
