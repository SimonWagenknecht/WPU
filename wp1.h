/*------------------------------ SWP und AWP -------------------------------------------*/
const Pgrup wp1[] = {
	
	#if WPU_SWP == 1
//-------------------
//-------------------
	{"***:"," WAERMEPUMPE    ","       ", P&Wpmod,							ASCII_FORM, 0, P&vis,		V1},
  // 1 - 20 Temperaturfühler
	{"*01:"," TV-SWP         "," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," TV-SWP         "," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*02:"," TR-SWP         "," C    ", P&TR_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*02:"," TR-SWP         "," C    ", P&TR_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*03:"," TSPo-SWP       "," C    ", P&TSPo_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*03:"," TSPo-SWP       "," C    ", P&TSPo_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#if TSPm > 0
	{"*04:"," TSPm-SWP       "," C    ", P&TSPm_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*04:"," TSPm-SWP       "," C    ", P&TSPm_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	{"*05:"," TSPu-SWP       "," C    ", P&TSPu_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*05:"," TSPu-SWP       "," C    ", P&TSPu_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#if TVST > 0
	{"*06:"," TVST-SWP       "," C    ", P&TVST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*06:"," TVST-SWP       "," C    ", P&TVST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TRST > 0
	{"*07:"," TRST-SWP       "," C    ", P&TRST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*07:"," TRST-SWP       "," C    ", P&TRST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	{"*08:"," TVQ-SWP        "," C    ", P&TVQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*08:"," TVQ-SWP        "," C    ", P&TVQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*09:"," TRQ-SWP        "," C    ", P&TRQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*09:"," TRQ-SWP        "," C    ", P&TRQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#if TVHG > 0
	{"*10:"," TVHG-SWP       "," C    ", P&TVH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*10:"," TVHG-SWP       "," C    ", P&TVH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	
	#if TRHG > 0
	{"*11:"," TRHG-SWP       "," C    ", P&TRH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*11:"," TRHG-SWP       "," C    ", P&TRH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	
	// Eingänge 
	#if BM_WPU > 0
	{"*20:"," BM SWP         ","       ", P&BM_UNI[U1],		 				JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	#endif
	#if MB_Dimplex > 0
	{"*->:"," BM ModBus DIMP ","       ", P&wpd[WP1].ch_BM_WPU_MB, 	JANEIN_FORM, 2, P&vis,		V0, 0, 0},
	#endif
	#if SM_WPU > 0
	{"*21:"," SM SWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	// Externe Anforderung
	
	#if ANF_EXT_WPU > 0
	{"*22:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],						ANA_FORM, 1, P&vis,		V1, 0, 0},      
	{"*23:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],	 		 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*->:"," ANF BUS aktiv  ","       ", P&wpd[WP1].Status_SW_DM_aktiv,	 JANEIN_FORM, 0, P&vis,		V0, 0, 0},
	{"*24:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal,			 ANA_FORM, 1, P&vis,	V0, 0, 0},
	{"*24:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal, 	  ANA_FORM, 0x81, P&kom,	V0, 0, 0},
	{"*->:"," ANF INP aktiv  ","       ", P&wpd[WP1].Status_SW_AE_aktiv,	 JANEIN_FORM, 0, P&vis,		V0, 0, 0},
	{" ->;"," 0V ANFORD.INP  "," C    ", P&AnaInpPara[U1].Skal0,		 		S_INT, 1, P&hid1,	V0, 0, 0},	
	{" ->;"," 10V ANFORD.INP "," C    ", P&AnaInpPara[U1].Skal10,	 		S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.anford.inp  "," s     ", P&AnaInpPara[U1].ZkFilt,		 US_INT, 0, P&hid1,	V0, 0, 0},
	{"*->;"," anforderung inp"," V     ", P&AE_UNI[U1],					  	ANA_FORMP, 2, P&hid1,	V0, 0, 0},
	{"*->;"," anforderung inp"," %     ", P&anaInp[U1].mwFilt,				 US_INT, 1, P&hid1,	V0, 0, 0},
	// Sollwertoffset, wenn AE oder DM aktiviert
	{" 25;"," ANFORD.  Offset"," C    ", P&wps[WP1].T_Sollwert_Offset,	 S_INT, 1, P&hid1,	V0, 0, 0},
	// Ersatzwert
	{" 26;"," Ersatz-ANFORD. "," C    ", P&wps[WP1].T_Ersatz_Sollwert,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	#endif
	// Interne Anforderung
	#if HKANZ > 0	
	{"*22:"," VL BER HK1     "," C    ", P&hkd[HK1].tvsb,					US_INT, 1, P&vis,		V0, 0, 0},
	{" 23;"," ANFORD.  Offset"," C    ", P&wps[WP1].T_Sollwert_Offset,	 S_INT, 1, P&hid1,	V0, 0, 0},
	#endif
	#if TWE_ANF > 0 && DM_MODE > 0
	//{" 24;"," TWE-ANFORDERUNG"," C    ", P&wps[WP1].TWE_Sollwert,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{"*24:"," E: TWE-ANF E/A "," 1=EIN ", P&station1_ea,				 		ANA_FORM, 0, P&vis, 	V0, 0, 0},
	#endif


	// Manuelle Sollwertvorgabe Sollwert
	{" 27;"," Manu ANFORD.   "," HAND=1", P&wps[WP1].Para_Manu_Sollwert, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," ANFORDERUNG    "," C    ", P&wps[WP1].T_manu_Sollwert,US_INT, 1, P&hid1,	V1, 0, 0},
	
	// Eingehender ausgewerteter Sollwert (wird in SteuerWPU ausgewertet)
	{"*28:"," ANFORD.  AKTIV "," C    ", P&wpd[WP1].Eingehender_Sollwert,	S_INT, 1, P&vis, 	V1, 0, 0},
	{" ->;"," Min Anf. AKTIV "," C    ", P&wps[WP1].iPa_T_Sollwert_IN_MIN,	S_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," Max Anf. AKTIV "," C    ", P&wps[WP1].iPa_T_Sollwert_IN_MAX,	S_INT, 1, P&hid1,	V1, 0, 0},
	// Ausgänge 
	{"*30:"," Sollwert AA    "," C    ", P&maxAnford,				 				 S_INT, 1, P&vis, 	V1, 0, 0},
	{" ->;"," TEMP.Sollw.MIN "," C    ", P&TmanfSkalMin,							 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," SPG.Sollw.MIN  "," Volt  ", P&TmanfSkalMinSpg,					 S_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," TEMP.Sollw.MAX "," C    ", P&TmanfSkalMax,							 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," SPG.Sollw.MAX  "," Volt  ", P&TmanfSkalMaxSpg,					 S_INT, 2, P&hid1,	V0, 0, 0},
	{"*->;"," ausgabe tmanf  "," Volt  ", P&TMANF[0],							AOUT_FORMP, 2, P&hid1,	V0, 0, 0},
	{" 31:"," Sollwert E/A ? "," EIN=1 ", P&wps[WP1].Sollwert_EA,		 US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," SOLLW SWP HEI  "," C    ", P&wps[WP1].intPa_Sollwert_HEI,	S_INT, 1, P&hid1_SOLLW_EA,	V0, 0, 0},
	{" ->;"," SOLLW SWP AUS  "," C    ", P&wps[WP1].intPa_Sollwert_AUS,	S_INT, 1, P&hid1_SOLLW_EA,	V0, 0, 0},
	
	#if TWE_ANF > 0
	{" ->;"," SOLLW SWP TWE  "," C    ", P&wps[WP1].intPa_Sollwert_TWE,	S_INT, 1, P&hid1,	V0, 0, 0},
	#endif

	{"*32:"," FREIGABE SWP   ","       ", P&DA_UNI[U1],	 			   JANEIN_FORMOP, 2, P&vis,		V0, 0, 0},
	{" 33;"," Manu FREIG SWP "," HAND=1", P&wps[WP1].WPU_Freigabe_Haut, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," SWP FRE EIN/AUS"," EIN=1 ", P&wps[WP1].WPU_Freigabe_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},

	#if TWE_ANF > 0
	{"*34:"," FREIG TWE VORRA","       ", P&DA_UNI[U2],	 			   JANEIN_FORMOP, 2, P&vis,		V0, 0, 0},
	{" 35;"," Manu TWE VORRA "," HAND=1", P&wps[WP1].chPa_Freiagbe_TWE_Hau, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," TWE VOR EIN/AUS"," EIN=1 ", P&wps[WP1].chPa_Freiagbe_TWE_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	#endif

	#if UV_TWE > 0
	{"*36:"," UV SWP AUF ?   ","       ", P&DA_UNI[U3],	 			   JANEIN_FORMOP, 2, P&vis,		V0, 0, 0},
	{" 37;"," Manu UV SWP    "," HAND=1", P&wps[WP1].chPa_uv_Hau, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," UV SWP STELLEN "," AUF=1 ", P&wps[WP1].chPa_uv_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif


	{" ->;"," Verzoegerungz  "," min   ", P&wps[WP1].chPa_Verzoegerung_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Verzoegerun_Cnt"," s     ", P&wpd[WP1].Verzoegerungszeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{" ->;"," Mindestlaufz   "," min   ", P&wps[WP1].chPa_Mindestlaufzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Mindestlauf_Cnt"," s     ", P&wpd[WP1].Mindestlaufzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{" ->;"," Sperrzeit      "," min   ", P&wps[WP1].chPa_Sperrzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Sperrzeit_Cnt  "," s     ", P&wpd[WP1].Sperrzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{"*->:"," freigabe swp   ","       ", P&wpd[WP1].Status_WPU_Freigabe_oZeit, JANEIN_FORM, 2, P&vis,		V0, 0, 0},
	
	#if RV_VOL > 0
	{"*38:"," V.STELL RV SWP "," %     ", P&AA_UNI[U1],								AOUT_FORMP, 1, P&vis,	V1, 0, 0},
	{" 39;"," HAND RV SWP    "," HAND=1", P&wps[WP1].chpa_rv_Hau,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," RV SWP stellen "," %     ", P&wps[WP1].ipa_rv_stellung,			 US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
	
	//Betriebszustände
	{" 40:"," Strategie      "," 1-2   ", P&wps[WP1].chPa_Strategie, US_CHAR, 0, P&hid1,		V1, 0, 0},	
	{"*41:"," Quellenschutz ?","       ", P&wpd[WP1].Status_Quellenschutz, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{" ->;"," Quellenminimum "," C    ", P&wps[WP1].intPa_Quellentemperaturminimum,	S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," Aktive Temp.   "," C    ", P&wpd[WP1].intT_Quellentemperatur_aktiv,	  S_INT, 1, P&hid1,	V0, 0, 0},
	{"*42:"," Frostschutz   ?","       ", P&wpd[WP1].Status_Frostschutz, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{" ->;"," Speicherminimum"," C    ", P&wps[WP1].intPa_Speicherminimum,	S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," Aktive Temp.   "," C    ", P&wpd[WP1].intT_Speichertemperatur_aktiv,	  S_INT, 1, P&hid1,	V0, 0, 0},
	{"*43:"," Speicherladung?","       ", P&wpd[WP1].Status_Speicherladung, JANEIN_FORM, 2, P&hid1_dS1,		V1, 0, 0},
	{" ->;"," dT Ladung ein ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_on,	S_INT, 1, P&hid1_dS1,	V1, 0, 0},
	{" ->;"," dT Ladung aus ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_off,	S_INT, 1, P&hid1_dS1,	V1, 0, 0},
	{"*->;"," Ladebeginn TSPo"," C    ", P&wpd[WP1].intT_Ladebginn,	  S_INT, 1, P&hid1_dS1,	V0, 0, 0},
	{"*->;"," Ladeende TSPu  "," C    ", P&wpd[WP1].intT_Ladeende,	  S_INT, 1, P&hid1_dS1,	V0, 0, 0},
	{"*43:"," Rueckl.-Regel ?","       ", P&wpd[WP1].Status_Speicherladung, JANEIN_FORM, 2, P&hid1_dS2,		V0, 0, 0},
	{" ->;"," dT RL ein     ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_on,	S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	{" ->;"," dT RL aus     ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_off,	S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	{"*->;"," RL-Beg. TR-SWP "," C    ", P&wpd[WP1].intT_Ladebginn,	  S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	{"*->;"," RL-Ende TR-SWP "," C    ", P&wpd[WP1].intT_Ladeende,	  S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	#if TWE_ANF > 0
	{"*44:"," Warmwasservor.?","       ", P&wpd[WP1].Status_WW_Vorrang, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{" ->;"," Maximale Dauer "," min   ", P&wps[WP1].chPa_TWE_Maxlaufzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Max. Dauer_Cnt "," s     ", P&wpd[WP1].i_TWE_Maxzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{" ->;"," Sperrzeit      "," min   ", P&wps[WP1].chPa_TWE_Sperrzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Sperrzeit_Cnt  "," s     ", P&wpd[WP1].i_TWE_Sperrzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	#endif
		#if WPU_UST > 0
	{"*45:"," Unterstuetzung?","       ", P&wpd[WP1].Status_Unterstuetzung, JANEIN_FORM, 2, P&hid1,		V1, 0, 0},
	{" ->;"," dT UST ein    ?"," K     ", P&wps[WP1].intPa_Unterst_ANF_ein,	S_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," dT UST aus    ?"," K     ", P&wps[WP1].intPa_Unterst_ANF_aus,	S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," UST-Beg. Temp. "," C    ", P&wpd[WP1].intT_Unterst_ANF_ein,	  S_INT, 1, P&hid1,	V0, 0, 0},
	{"*->;"," UST-Ende Temp. "," C    ", P&wpd[WP1].intT_Unterst_ANF_aus,	  S_INT, 1, P&hid1,	V0, 0, 0},
	#endif
	
	#if TWE_ANF > 0
	{"*45:"," WPU-HEIZEN ?   ","       ", P&wpd[WP1].chPa_WPU_HEI, 		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*46:"," WPU-TWE VORRA ?","       ", P&wpd[WP1].chPa_WPU_TWE, 		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif
//	// WPU-Infos
//	{"*50;"," WPU-STARTS Anf ","       ", P&wpd[WP1].WPU_Starts_Freigabe, 			US_INT, 0, P&vis,	V0, 0, 0},
//	{"*51;"," WPU-STARTS BM  ","       ", P&wpd[WP1].WPU_Starts_BM, 			US_INT, 0, P&vis,	V0, 0, 0},
//	{"*52;"," WPU-LZ Anf     "," h     ", P&wpd[WP1].WPU_Freigabe_Laufzeit_h,					US_INT, 0, P&vis,	V1, 0, 0},
//	{" ->;"," wpu-lz anf     "," s     ", P&wpd[WP1].WPU_Freigabe_Laufzeit_sec,				US_INT, 0, P&vis,	V0, 0, 0},
//	{"*53;"," WPU-LZ BM      "," h     ", P&wpd[WP1].WPU_BM_Laufzeit_h,					US_INT, 0, P&vis,	V1, 0, 0},
//	{" ->;"," wpu-lz bm      "," s     ", P&wpd[WP1].WPU_BM_Laufzeit_sec,				US_INT, 0, P&vis,	V0, 0, 0},
	
	#if RV_VOL > 0
	{" 80;"," P-VERSTAERKUNG "," %/m/h", P&wps[0].Kp,						US_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," D-VERSTAERKUNG ","       ", P&wps[0].Kd,						US_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," TASTZEIT       "," s     ", P&wps[0].Ts,						US_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," NACHSTELLZEIT  "," s     ", P&wps[0].Tn,						US_INT, 1, P&hid1,	V0, 0, 0},
	
	{" 81;"," VENTILSTELL.MIN"," %     ", P&wps[0].Y_rel_min,	 		US_INT, 1, P&hid1,	V0, 0, 0},
	{" 82;"," VENTILSTELL.MAX"," %     ", P&wps[0].Y_rel_max,	 		US_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," OEFFNUNGSBEGINN"," %     ", P&wps[0].Y_rel_beg,	 		US_INT, 1, P&hid1,	V0, 0, 0},
	
	{"*83;"," IST VOLSTR. WSP"," m/h  ", P&wmengCtr[2].flow_h,			US_LONG, 2, P&hid1,	V0, 0, 0},
	
	{" 84;"," SOLL Vol. WPU  "," m/h  ", P&wps[0].iPa_Vol_ist,		US_INT, 1, P&hid1,	V1, 0, 0},
	{"*85;"," REGELABWEICHUNG"," m/h  ", P&wpd[0].ei,						 S_INT, 2, P&hid1,	V0, 0, 0},
	{"*->;"," dy_rel         ","       ", P&wpd[0].dy_rel,				 S_INT, 3, P&hid1,	V0, 0, 0},
	//{"*86:"," V.STELL RV SWP "," %     ", P&AA_UNI[U1],					AOUT_FORMP, 1, P&vis,	V0, 0, 0},
	#endif
	
	#endif
	
	#if WPU_AWP == 1
//-------------------
	//-------------------
	{"***:"," WAERMEPUMPE    ","       ", P&Wpmod,							ASCII_FORM, 0, P&vis,		V1},
  // 1 - 20 Temperaturfühler
	{"*01:"," TV-AWP         "," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," TV-AWP         "," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*02:"," TR-AWP         "," C    ", P&TR_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*02:"," TR-AWP         "," C    ", P&TR_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*03:"," TSPo-AWP       "," C    ", P&TSPo_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*03:"," TSPo-AWP       "," C    ", P&TSPo_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#if TSPm > 0
	{"*04:"," TSPm-AWP       "," C    ", P&TSPm_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*04:"," TSPm-AWP       "," C    ", P&TSPm_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	{"*05:"," TSPu-AWP       "," C    ", P&TSPu_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*05:"," TSPu-AWP       "," C    ", P&TSPu_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#if TVST > 0
	{"*06:"," TVST-AWP       "," C    ", P&TVST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*06:"," TVST-AWP       "," C    ", P&TVST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	#if TRST > 0
	{"*07:"," TRST-AWP       "," C    ", P&TRST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*07:"," TRST-AWP       "," C    ", P&TRST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	{"*08:"," TVQ-AWP        "," C    ", P&TVQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*08:"," TVQ-AWP        "," C    ", P&TVQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*09:"," TRQ-AWP        "," C    ", P&TRQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*09:"," TRQ-AWP        "," C    ", P&TRQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#if TVHG > 0
	{"*10:"," TVHG-AWP       "," C    ", P&TVH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*10:"," TVHG-AWP       "," C    ", P&TVH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	
	#if TRHG > 0
	{"*11:"," TRHG-AWP       "," C    ", P&TRH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*11:"," TRHG-AWP       "," C    ", P&TRH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	#endif
	
	// Eingänge 
	#if BM_WPU > 0
	{"*20:"," BM AWP         ","       ", P&BM_UNI[U1],		 				JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	#endif
	#if MB_Dimplex > 0
	{"*->:"," BM ModBus DIMP ","       ", P&wpd[WP1].ch_BM_WPU_MB, 	JANEIN_FORM, 2, P&vis,		V0, 0, 0},
	#endif
	#if SM_WPU > 0
	{"*21:"," SM AWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	#endif
	// Externe Anforderung
	
	#if ANF_EXT_WPU > 0
	{"*22:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],						ANA_FORM, 1, P&vis,		V1, 0, 0},      
	{"*23:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],	 		 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*->:"," ANF BUS aktiv  ","       ", P&wpd[WP1].Status_SW_DM_aktiv,	 JANEIN_FORM, 0, P&vis,		V0, 0, 0},
	{"*24:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal,			 ANA_FORM, 1, P&vis,	V0, 0, 0},
	{"*24:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal, 	  ANA_FORM, 0x81, P&kom,	V0, 0, 0},
	{"*->:"," ANF INP aktiv  ","       ", P&wpd[WP1].Status_SW_AE_aktiv,	 JANEIN_FORM, 0, P&vis,		V0, 0, 0},
	{" ->;"," 0V ANFORD.INP  "," C    ", P&AnaInpPara[U1].Skal0,		 		S_INT, 1, P&hid1,	V0, 0, 0},	
	{" ->;"," 10V ANFORD.INP "," C    ", P&AnaInpPara[U1].Skal10,	 		S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.anford.inp  "," s     ", P&AnaInpPara[U1].ZkFilt,		 US_INT, 0, P&hid1,	V0, 0, 0},
	{"*->;"," anforderung inp"," V     ", P&AE_UNI[U1],					  	ANA_FORMP, 2, P&hid1,	V0, 0, 0},
	{"*->;"," anforderung inp"," %     ", P&anaInp[U1].mwFilt,				 US_INT, 1, P&hid1,	V0, 0, 0},
	// Sollwertoffset, wenn AE oder DM aktiviert
	{" 25;"," ANFORD.  Offset"," C    ", P&wps[WP1].T_Sollwert_Offset,	 S_INT, 1, P&hid1,	V0, 0, 0},
	// Ersatzwert
	{" 26;"," Ersatz-ANFORD. "," C    ", P&wps[WP1].T_Ersatz_Sollwert,	 S_INT, 1, P&hid1,	V0, 0, 0},	
	#endif
	// Interne Anforderung
	#if HKANZ > 0	
	{"*22:"," VL BER HK1     "," C    ", P&hkd[HK1].tvsb,					US_INT, 1, P&vis,		V0, 0, 0},
	{" 23;"," ANFORD.  Offset"," C    ", P&wps[WP1].T_Sollwert_Offset,	 S_INT, 1, P&hid1,	V0, 0, 0},
	#endif
	#if TWE_ANF > 0
	//{" 24;"," TWE-ANFORDERUNG"," C    ", P&wps[WP1].TWE_Sollwert,	 S_INT, 1, P&hid1,	V0, 0, 0},
	{" 24:"," E: TWE-ANF E/A "," 1=EIN ", P&station1_ea,				 		ANA_FORM, 0, P&vis, 	V0, 0, 0},
	#endif


	// Manuelle Sollwertvorgabe Sollwert
	{" 27;"," Manu ANFORD.   "," HAND=1", P&wps[WP1].Para_Manu_Sollwert, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," ANFORDERUNG    "," C    ", P&wps[WP1].T_manu_Sollwert,US_INT, 1, P&hid1,	V1, 0, 0},
	
	// Eingehender ausgewerteter Sollwert (wird in SteuerWPU ausgewertet)
	{"*28:"," ANFORD.  AKTIV "," C    ", P&wpd[WP1].Eingehender_Sollwert,	S_INT, 1, P&vis, 	V1, 0, 0},
	{" ->;"," Min Anf. AKTIV "," C    ", P&wps[WP1].iPa_T_Sollwert_IN_MIN,	S_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," Max Anf. AKTIV "," C    ", P&wps[WP1].iPa_T_Sollwert_IN_MAX,	S_INT, 1, P&hid1,	V1, 0, 0},
	// Ausgänge 
	{"*30:"," Sollwert AA    "," C    ", P&maxAnford,				 				 S_INT, 1, P&vis, 	V1, 0, 0},
	{" ->;"," TEMP.Sollw.MIN "," C    ", P&TmanfSkalMin,							 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," SPG.Sollw.MIN  "," Volt  ", P&TmanfSkalMinSpg,					 S_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," TEMP.Sollw.MAX "," C    ", P&TmanfSkalMax,							 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," SPG.Sollw.MAX  "," Volt  ", P&TmanfSkalMaxSpg,					 S_INT, 2, P&hid1,	V0, 0, 0},
	{"*->;"," ausgabe tmanf  "," Volt  ", P&TMANF[0],							AOUT_FORMP, 2, P&hid1,	V0, 0, 0},
	{" 31:"," Sollwert E/A ? "," EIN=1 ", P&wps[WP1].Sollwert_EA,		 US_CHAR, 0, P&hid1,	V0, 0, 0},
	{" ->;"," SOLLW AWP HEI  "," C    ", P&wps[WP1].intPa_Sollwert_HEI,	S_INT, 1, P&hid1_SOLLW_EA,	V0, 0, 0},
	{" ->;"," SOLLW AWP AUS  "," C    ", P&wps[WP1].intPa_Sollwert_AUS,	S_INT, 1, P&hid1_SOLLW_EA,	V0, 0, 0},
	#if TWE_ANF > 0
	{" ->;"," SOLLW AWP TWE  "," C    ", P&wps[WP1].intPa_Sollwert_TWE,	S_INT, 1, P&hid1,	V0, 0, 0},
	#endif

	{"*32:"," FREIGABE AWP   ","       ", P&DA_UNI[U1],	 			   JANEIN_FORMOP, 2, P&vis,		V0, 0, 0},
	{" 33;"," Manu FREIG AWP "," HAND=1", P&wps[WP1].WPU_Freigabe_Haut, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," AWP FRE EIN/AUS"," EIN=1 ", P&wps[WP1].WPU_Freigabe_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},

	#if TWE_ANF > 0
	{"*34:"," FREIG TWE VORRA","       ", P&DA_UNI[U2],	 			   JANEIN_FORMOP, 2, P&vis,		V0, 0, 0},
	{" 35;"," Manu TWE VORRA "," HAND=1", P&wps[WP1].chPa_Freiagbe_TWE_Hau, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," TWE VOR EIN/AUS"," EIN=1 ", P&wps[WP1].chPa_Freiagbe_TWE_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	#endif

	#if UV_TWE > 0
	{"*36:"," UV AWP AUF ?   ","       ", P&DA_UNI[U3],	 			   JANEIN_FORMOP, 2, P&vis,		V0, 0, 0},
	{" 37;"," Manu UV AWP    "," HAND=1", P&wps[WP1].chPa_uv_Hau, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," UV AWP STELLEN "," AUF=1 ", P&wps[WP1].chPa_uv_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},
	#endif


	{" ->;"," Verzoegerungz  "," min   ", P&wps[WP1].chPa_Verzoegerung_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Verzoegerun_Cnt"," s     ", P&wpd[WP1].Verzoegerungszeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{" ->;"," Mindestlaufz   "," min   ", P&wps[WP1].chPa_Mindestlaufzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Mindestlauf_Cnt"," s     ", P&wpd[WP1].Mindestlaufzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{" ->;"," Sperrzeit      "," min   ", P&wps[WP1].chPa_Sperrzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Sperrzeit_Cnt  "," s     ", P&wpd[WP1].Sperrzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{"*->:"," freigabe swp   ","       ", P&wpd[WP1].Status_WPU_Freigabe_oZeit, JANEIN_FORM, 2, P&vis,		V0, 0, 0},
	
	#if RV_VOL > 0
	{"*38:"," V.STELL RV AWP "," %     ", P&AA_UNI[U1],								AOUT_FORMP, 1, P&vis,	V1, 0, 0},
	{" 39;"," HAND RV AWP    "," HAND=1", P&wps[WP1].chpa_rv_Hau,				 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," RV AWP stellen "," %     ", P&wps[WP1].ipa_rv_stellung,			 US_INT, 1, P&hid1,	V1, 0, 0},
	#endif
	
	//Betriebszustände
	{" 40:"," Strategie      "," 1-2   ", P&wps[WP1].chPa_Strategie, US_CHAR, 0, P&hid1,		V1, 0, 0},	
	{"*41:"," Quellenschutz ?","       ", P&wpd[WP1].Status_Quellenschutz, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{" ->;"," Quellenminimum "," C    ", P&wps[WP1].intPa_Quellentemperaturminimum,	S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," Aktive Temp.   "," C    ", P&wpd[WP1].intT_Quellentemperatur_aktiv,	  S_INT, 1, P&hid1,	V0, 0, 0},
	{"*42:"," Frostschutz   ?","       ", P&wpd[WP1].Status_Frostschutz, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{" ->;"," Speicherminimum"," C    ", P&wps[WP1].intPa_Speicherminimum,	S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," Aktive Temp.   "," C    ", P&wpd[WP1].intT_Speichertemperatur_aktiv,	  S_INT, 1, P&hid1,	V0, 0, 0},
	{"*43:"," Speicherladung?","       ", P&wpd[WP1].Status_Speicherladung, JANEIN_FORM, 2, P&hid1_dS1,		V1, 0, 0},
	{" ->;"," dT Ladung ein ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_on,	S_INT, 1, P&hid1_dS1,	V1, 0, 0},
	{" ->;"," dT Ladung aus ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_off,	S_INT, 1, P&hid1_dS1,	V1, 0, 0},
	{"*->;"," Ladebeginn TSPo"," C    ", P&wpd[WP1].intT_Ladebginn,	  S_INT, 1, P&hid1_dS1,	V0, 0, 0},
	{"*->;"," Ladeende TSPu  "," C    ", P&wpd[WP1].intT_Ladeende,	  S_INT, 1, P&hid1_dS1,	V0, 0, 0},
	{"*43:"," Rueckl.-Regel ?","       ", P&wpd[WP1].Status_Speicherladung, JANEIN_FORM, 2, P&hid1_dS2,		V0, 0, 0},
	{" ->;"," dT RL ein     ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_on,	S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	{" ->;"," dT RL aus     ?"," K     ", P&wps[WP1].intPa_T_Speicherladung_off,	S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	{"*->;"," RL-Beg. TR-AWP "," C    ", P&wpd[WP1].intT_Ladebginn,	  S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	{"*->;"," RL-Ende TR-AWP "," C    ", P&wpd[WP1].intT_Ladeende,	  S_INT, 1, P&hid1_dS2,	V0, 0, 0},
	#if TWE_ANF > 0
	{"*44:"," Warmwasservor.?","       ", P&wpd[WP1].Status_WW_Vorrang, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{" ->;"," Maximale Dauer "," min   ", P&wps[WP1].chPa_TWE_Maxlaufzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Max. Dauer_Cnt "," s     ", P&wpd[WP1].i_TWE_Maxzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	{" ->;"," Sperrzeit      "," min   ", P&wps[WP1].chPa_TWE_Sperrzeit_min,		 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," Sperrzeit_Cnt  "," s     ", P&wpd[WP1].i_TWE_Sperrzeit_Cnt,		  US_INT, 0, P&hid1,	V0, 0, 0},
	#endif
	#if WPU_UST > 0
	{"*45:"," Unterstuetzung?","       ", P&wpd[WP1].Status_Unterstuetzung, JANEIN_FORM, 2, P&hid1,		V1, 0, 0},
	{" ->;"," dT UST ein    ?"," K     ", P&wps[WP1].intPa_Unterst_ANF_ein,	S_INT, 1, P&hid1,	V1, 0, 0},
	{" ->;"," dT UST aus    ?"," K     ", P&wps[WP1].intPa_Unterst_ANF_aus,	S_INT, 1, P&hid1,	V1, 0, 0},
	{"*->;"," UST-Beg. Temp. "," C    ", P&wpd[WP1].intT_Unterst_ANF_ein,	  S_INT, 1, P&hid1,	V0, 0, 0},
	{"*->;"," UST-Ende Temp. "," C    ", P&wpd[WP1].intT_Unterst_ANF_aus,	  S_INT, 1, P&hid1,	V0, 0, 0},
	#endif
	#if TWE_ANF > 0
	{"*45:"," WPU-HEIZEN ?   ","       ", P&wpd[WP1].chPa_WPU_HEI, 		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{"*46:"," WPU-TWE VORRA ?","       ", P&wpd[WP1].chPa_WPU_TWE, 		 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	#endif
//	// WPU-Infos
//	{"*50;"," WPU-STARTS Anf ","       ", P&wpd[WP1].WPU_Starts_Freigabe, 			US_INT, 0, P&vis,	V0, 0, 0},
//	{"*51;"," WPU-STARTS BM  ","       ", P&wpd[WP1].WPU_Starts_BM, 			US_INT, 0, P&vis,	V0, 0, 0},
//	{"*52;"," WPU-LZ Anf     "," h     ", P&wpd[WP1].WPU_Freigabe_Laufzeit_h,					US_INT, 0, P&vis,	V1, 0, 0},
//	{" ->;"," wpu-lz anf     "," s     ", P&wpd[WP1].WPU_Freigabe_Laufzeit_sec,				US_INT, 0, P&vis,	V0, 0, 0},
//	{"*53;"," WPU-LZ BM      "," h     ", P&wpd[WP1].WPU_BM_Laufzeit_h,					US_INT, 0, P&vis,	V1, 0, 0},
//	{" ->;"," wpu-lz bm      "," s     ", P&wpd[WP1].WPU_BM_Laufzeit_sec,				US_INT, 0, P&vis,	V0, 0, 0},
	
	#if RV_VOL > 0
	{" 80;"," P-VERSTAERKUNG "," %/m/h", P&wps[0].Kp,						US_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," D-VERSTAERKUNG ","       ", P&wps[0].Kd,						US_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," TASTZEIT       "," s     ", P&wps[0].Ts,						US_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," NACHSTELLZEIT  "," s     ", P&wps[0].Tn,						US_INT, 1, P&hid1,	V0, 0, 0},
	
	{" 81;"," VENTILSTELL.MIN"," %     ", P&wps[0].Y_rel_min,	 		US_INT, 1, P&hid1,	V0, 0, 0},
	{" 82;"," VENTILSTELL.MAX"," %     ", P&wps[0].Y_rel_max,	 		US_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," OEFFNUNGSBEGINN"," %     ", P&wps[0].Y_rel_beg,	 		US_INT, 1, P&hid1,	V0, 0, 0},
	
	{"*83;"," IST VOLSTR. WSP"," m/h  ", P&wmengCtr[2].flow_h,			US_LONG, 2, P&hid1,	V0, 0, 0},
	
	{" 84;"," SOLL Vol. WPU  "," m/h  ", P&wps[0].iPa_Vol_ist,		US_INT, 1, P&hid1,	V1, 0, 0},
	{"*85;"," REGELABWEICHUNG"," m/h  ", P&wpd[0].ei,						 S_INT, 2, P&hid1,	V0, 0, 0},
	{"*->;"," dy_rel         ","       ", P&wpd[0].dy_rel,				 S_INT, 3, P&hid1,	V0, 0, 0},
//	{"*86:"," VENTILSTELLUNG "," %     ", P&AA_UNI[U1],					AOUT_FORMP, 1, P&vis,	V0, 0, 0},
	#endif
		
	#endif

	};
