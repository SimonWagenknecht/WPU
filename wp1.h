/*------------------------------ Heizkreis 1 -------------------------------------------*/
const Pgrup wp1[] = {
//-------------------
	{"***:"," WAERMEPUMPE    ","       ", P&Wpmod,							ASCII_FORM, 0, P&vis,		V1},
  // 1 - 20 Temperaturfühler
	{"*01:"," TV-WWP         "," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," TV-WWP         "," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*02:"," TR-WWP         "," C    ", P&TR_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*02:"," TR-WWP         "," C    ", P&TR_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*03:"," TSPo-WWP       "," C    ", P&TSPo_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*03:"," TSPo-WWP       "," C    ", P&TSPo_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*04:"," TSPu-WWP       "," C    ", P&TSPu_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*04:"," TSPu-WWP       "," C    ", P&TSPu_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*05:"," TVST-WWP       "," C    ", P&TVST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*05:"," TVST-WWP       "," C    ", P&TVST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*06:"," TRST-WWP       "," C    ", P&TRST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*06:"," TRST-WWP       "," C    ", P&TRST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*07:"," TVQ-WWP        "," C    ", P&TVQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*07:"," TVQ-WWP        "," C    ", P&TVQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*08:"," TRQ-WWP        "," C    ", P&TRQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*08:"," TRQ-WWP        "," C    ", P&TRQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*09:"," TVH-WWP        "," C    ", P&TVH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*09:"," TVH-WWP        "," C    ", P&TVH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*10:"," TRH-WWP        "," C    ", P&TRH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*10:"," TRH-WWP        "," C    ", P&TRH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	// Eingänge 
	{"*11:"," BM WWP         ","       ", P&BM_UNI[U1],		 				JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{"*12:"," SM WWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	{"*13:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],						ANA_FORM, 1, P&vis,		V1, 0, 0},      
	{"*13:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],	 		 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*->:"," ANF BUS aktiv  ","       ", P&wpd[WP1].Status_SW_DM_aktiv,	 JANEIN_FORM, 0, P&vis,		V0, 0, 0},
	{"*14:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal,			 ANA_FORM, 1, P&vis,	V0, 0, 0},
	{"*14:"," ANFORDERUNG INP"," C    ", P&anaInp[U1].mwSkal, 	  ANA_FORM, 0x81, P&kom,	V0, 0, 0},
	{"*->:"," ANF INP aktiv  ","       ", P&wpd[WP1].Status_SW_AE_aktiv,	 JANEIN_FORM, 0, P&vis,		V0, 0, 0},
	{" ->;"," 0V ANFORD.INP  "," C    ", P&AnaInpPara[U1].Skal0,		 		S_INT, 1, P&hid1,	V0, 0, 0},	
	{" ->;"," 10V ANFORD.INP "," C    ", P&AnaInpPara[U1].Skal10,	 		S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," gl.anford.inp  "," s     ", P&AnaInpPara[U1].ZkFilt,		 US_INT, 0, P&hid2,	V0, 0, 0},
	{"*->;"," anforderung inp"," V     ", P&AE_UNI[U1],					  	ANA_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*->;"," anforderung inp"," %     ", P&anaInp[U1].mwFilt,				 US_INT, 1, P&hid2,	V0, 0, 0},	
	// Sollwertoffset, wenn AE oder DM aktiviert
	{" 15;"," Sollwert-Offset"," C    ", P&wps[WP1].T_Sollwert_Offset,	 S_INT, 1, P&hid1,	V0, 0, 0},
	// Ersatzwert
	{" 16;"," Ersatz-Sollw.  "," C    ", P&wps[WP1].T_Ersatz_Sollwert,	 S_INT, 1, P&hid1,	V0, 0, 0},
	// Manuelle Sollwertvorgabe Sollwert
	{" 17;"," Manu Sollwert  "," HAND=1", P&wps[WP1].Para_Manu_Sollwert, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->;"," Sollwert       "," C    ", P&wps[WP1].T_manu_Sollwert,US_INT, 1, P&hid1,	V1, 0, 0},
	// Ausgänge 

	{"*20:"," Sollwert AA    "," C    ", P&maxAnford,				 				 S_INT, 1, P&vis, 	V1, 0, 0},
	{" ->;"," TEMP.Sollw.MIN "," C    ", P&TmanfSkalMin,							 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," SPG.Sollw.MIN  "," Volt  ", P&TmanfSkalMinSpg,					 S_INT, 2, P&hid1,	V0, 0, 0},
	{" ->;"," TEMP.Sollw.MAX "," C    ", P&TmanfSkalMax,							 S_INT, 1, P&hid1,	V0, 0, 0},
	{" ->;"," SPG.Sollw.MAX  "," Volt  ", P&TmanfSkalMaxSpg,					 S_INT, 2, P&hid1,	V0, 0, 0},
	{"*->;"," ausgabe tmanf  "," Volt  ", P&TMANF[0],							AOUT_FORMP, 2, P&hid2,	V0, 0, 0},
	{"*21:"," FREIGABE WWP   ","       ", P&DA_UNI[U1],	 			   JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	{" ->;"," Manu FREIG WPU "," HAND=1", P&wps[WP1].WPU_Freigabe_Haut, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{" ->:"," WPU EIN/AUS    "," EIN=1 ", P&wps[WP1].WPU_Freigabe_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},
	
	};

