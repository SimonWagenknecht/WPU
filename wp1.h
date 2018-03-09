/*------------------------------ Heizkreis 1 -------------------------------------------*/
const Pgrup wp1[] = {
//-------------------
	{"***:"," WAERMEPUMPE    ","       ", P&Wpmod,							ASCII_FORM, 0, P&vis,		V1},

	{"*01:"," TV-WWP				 "," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," TV-WWP				 "," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*02:"," TR-WWP				 "," C    ", P&TR_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*02:"," TR-WWP				 "," C    ", P&TR_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*03:"," TSPo-WWP			 "," C    ", P&TSPo_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*03:"," TSPo-WWP			 "," C    ", P&TSPo_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*04:"," TSPu-WWP			 "," C    ", P&TSPu_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*04:"," TSPu-WWP			 "," C    ", P&TSPu_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*05:"," TVST-WWP			 "," C    ", P&TVST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*05:"," TVST-WWP			 "," C    ", P&TVST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{"*06:"," TRST-WWP			 "," C    ", P&TRST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*06:"," TRST-WWP			 "," C    ", P&TRST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
		{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
		{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
		{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
		{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
		{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{"*01:"," VORLAUF   PRIM."," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	
	};

