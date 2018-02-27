const Pgrup qsm[] = {
	{"***:"," QUITTIERBARE   ","       ", P&Stoermeld,							 ASCII_FORM, 0, P&vis,	V1, 0, 0},
	{"*01:"," QSM VORHANDEN ?","       ", P&un_qsm,			 						JANEIN_FORM, 0, P&vis,	V1, 0, 0},

//**PARSER-BEGIN-qsm[]**  AUTOMATISCHE ERZEUGUNG *****

//**PARSER-ENDE-qsm[]**  AUTOMATISCHE ERZEUGUNG *****

	{" 99:"," QUITTIER-TASTE "," EIN=1 ", P&quit_taste,			 						US_CHAR, 0, P&vis,	V1, 0, 0},
	{" ->;"," QUITTIER-TASTE ","       ", P&QUITTS[0],			 			JANEIN_FORMIP, 2, P&hid4,	V0, 0, 0},	// für erweiterte In-/Out Anzeige in ANL: 

};

