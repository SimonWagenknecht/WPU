#if MB_Dimplex == 1
const Pgrup dwp[] = {

	{"***:"," Dimplex-MODBUS ","       ", P&Leer15,																								ASCII_FORM,  0, P&vis,	V0, 0, 0},

	// Betriebsdaten
	{"*01:"," Aussenlufttemp."," C    ", P&mod_data.i_R_Aussenlufttemperatur, 													  US_INT,  1, P&vis,	V1, 0, 0},
	{"*02:"," Ruecklauftemp. "," C    ", P&mod_data.i_R_Ruecklauftemperatur, 													  US_INT,  1, P&vis,	V1, 0, 0},
	{"*03:"," RL-Solltemp    "," C    ", P&mod_data.i_R_Ruecklaufsolltemperatur, 											  US_INT,  1, P&vis,	V1, 0, 0},
	// Historie
	{"*04:"," Verdichter1-Lz "," h     ", P&mod_data.uc_R_Verdichter1_Stunden, 											     US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*05:"," Verdichter2-Lz "," h     ", P&mod_data.uc_R_Verdichter2_Stunden, 											     US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*06:"," Primaer-PU-Lz  "," h     ", P&mod_data.uc_R_Primaerpumpe_Stunden, 										     US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*07:"," HeizungsPU-Lz  "," h     ", P&mod_data.uc_R_Heizungspumpe_Stunden, 										     US_CHAR,  0, P&vis,	V0, 0, 0},
	{"*08:"," WarmwasserPU-Lz"," h     ", P&mod_data.uc_R_Warmwasserpumpe_Stunden, 									     US_CHAR,  0, P&vis,	V0, 0, 0},
	//1. Heizkreis
	{" 10:"," Parallelversch ","       ", P&mod_data.uc_W_Parallelverschiebung, 											   US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 11:"," Raumtemperatur "," C    ", P&mod_data.i_W_Raumtemperatur, 															    US_INT,  1, P&vis,	V0, 0, 0},
	{" 12:"," Festsolltempe  "," C    ", P&mod_data.i_W_Festwertsolltemperatur, 										     	US_INT,  1, P&vis,	V1, 0, 0},
	{" 13:"," Heizkurvenendp."," C    ", P&mod_data.i_W_Heizkurvenendpunkt, 										          US_INT,  1, P&vis,	V0, 0, 0},
	{" 14:"," Hysterese IN   "," K     ", P&mod_data.i_W_Hysterese_HK_IN, 									              US_INT,  0, P&vis,	V0, 0, 0},
	{"*15:"," Hysterese OUT  "," K     ", P&mod_data.i_Hysterese_HK_OUT, 									          	    US_INT,  0, P&vis,	V0, 0, 0},
	{" 16:"," Soll dyn. Kuehl"," C    ", P&mod_data.i_W_Solltemp_dyn_Kuehlung, 									        US_INT,  1, P&vis,	V0, 0, 0},
	// MODBUS
	{" 20:"," Betriebsmodus  ","       ", P&mod_data.uc_W_Betriebsmodus, 											       		 US_CHAR,  0, P&vis,	V1, 0, 0},
	{" 21:"," Partystunden   "," h     ", P&mod_data.uc_W_Anzahl_Partystunden, 											     US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 22:"," Urlaubstage    "," d     ", P&mod_data.uc_W_Anzahl_Urlaubstage, 										       US_CHAR,  0, P&vis,	V0, 0, 0},
	// Warmwasser
	{" 30:"," Hysterese TWE  "," K     ", P&mod_data.i_W_Hysterese_TWE, 											       		 	US_INT,  0, P&vis,	V0, 0, 0},
	{" 31:"," Solltemp. TWE  "," C    ", P&mod_data.i_W_Solltemperatur_TWE, 											        US_INT,  0, P&vis,	V1, 0, 0},
	// Displayanzeigen
	{"*40:"," Statusmeldung  ","       ", P&mod_data.uc_R_statusmeldung, 											     			 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*41:"," Sperren        ","       ", P&mod_data.uc_R_WPU_Sperre, 											       			 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*42:"," Störmeldungen  ","       ", P&mod_data.uc_R_WPU_SM, 										             			 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*43:"," Sensorik       ","       ", P&mod_data.uc_R_WPU_Sensorik, 										       			 US_CHAR,  0, P&vis,	V1, 0, 0},
	// Eingänge
	{"*50:"," WW-Thermostat  ","       ", P&mod_data.uc_R_Warmwasserthermostat, 											   US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*51:"," Schw.bad-Therm.","       ", P&mod_data.uc_R_Schwimmbadthermostat, 							    			 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*52:"," EVU-Sperre     ","       ", P&mod_data.uc_R_EVU_Sperre, 										         			 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*53:"," Sperre extern  ","       ", P&mod_data.uc_R_Sperre_Extern, 										       			 US_CHAR,  0, P&vis,	V1, 0, 0},
	// Ausgänge
	{"*54:"," Verdichter 1   ","       ", P&mod_data.uc_R_Verdichter1, 											     			 	 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*55:"," Verdichter 2   ","       ", P&mod_data.uc_R_Verdichter2, 											       			 US_CHAR,  0, P&vis,	V1, 0, 0},
	{"*56:"," Primaer-PU     ","       ", P&mod_data.uc_R_Primaerpumpe, 										             US_CHAR,  0, P&vis,	V0, 0, 0},
	{"*57:"," Heizungs-PU    ","       ", P&mod_data.uc_R_Heizungspumpe, 										       			 US_CHAR,  0, P&vis,	V0, 0, 0},
	{"*58:"," Warmwasser-PU  ","       ", P&mod_data.uc_R_Warmwasserpumpe, 										           US_CHAR,  0, P&vis,	V0, 0, 0},
	{"*59:"," Sammelstoerung ","       ", P&mod_data.uc_R_Sammelstoerung, 										       		 US_CHAR,  0, P&vis,	V1, 0, 0},
	// Zeitabgleich
	{" 70:"," Stunde         "," 0-23  ", P&mod_data.uc_RW_Stunde_Za, 											     			 	 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" ->:"," set Stunde     "," 0-1   ", P&mod_data.uc_W_setStunde_Za, 										       			 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 71:"," Minute         "," 0-59  ", P&mod_data.uc_RW_min_Za, 											     			 	 		 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" ->:"," set Minute     "," 0-1   ", P&mod_data.uc_W_setmin_Za, 										       			 		 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 72:"," Monat          "," 1-12  ", P&mod_data.uc_RW_Monat_Za,  											     			 	 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" ->:"," set Monat      "," 0-1   ", P&mod_data.uc_W_setMonat_Za,  										       			 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 73:"," Wochentag      "," Mo=1  ", P&mod_data.uc_RW_Wochentag_Za,								     			 	 		 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" ->:"," set Wochentag  "," 0-1   ", P&mod_data.uc_W_setWochentag_Za,							       			 		 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 74:"," Tag            "," 1-31  ", P&mod_data.uc_RW_Tag_Za,    											     			 	 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" ->:"," set Tag        "," 0-1   ", P&mod_data.uc_W_setTag_Za,    										       			 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" 75:"," Jahr           "," 0-99  ", P&mod_data.uc_RW_Jahr_Za,			      					     			 	 		 US_CHAR,  0, P&vis,	V0, 0, 0},
	{" ->:"," set Jahr       "," 0-1   ", P&mod_data.uc_W_setJahr_Za,					      		       			 		 US_CHAR,  0, P&vis,	V0, 0, 0},

};
#endif