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
	// Ausgänge 
	{"*11:"," FREIGABE WWP   "," EIN=1 ", P&DA_UNI[U1],			 		 			  US_CHAR, 0, P&vis,		V1, 0, 0},
	// Eingänge 
	{"*13:"," BM WWP         ","       ", P&BM_UNI[U1],		 				JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{"*14:"," SM WWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},

	
	};

