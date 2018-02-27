const Pgrup dia[] = {
	{"***:"," DIAGNOSE       ","       ", P&Leer15,							ASCII_FORM, 0, P&hid1,	V1, 0, 0},

	{" 01;"," DIAGNOSE BEGINN","       ", P&dis.begin,		 		 	 DATE_FORM, 1, P&hid1,	V1, 0, 0},	
	{" 02;"," DIAGNOSE ENDE  ","       ", P&dis.end, 			 		 	 DATE_FORM, 1, P&hid1,	V1, 0, 0}, 
	
	{"*06;"," VOLUMENSTR.MAX "," m/h  ", P&did.flow_max,	   		 	 US_LONG, 3, P&hid1,	V1, 0, 0}, 
	{"*07;"," LEISTUNG MAX   "," kW    ", P&did.wmLeistg_max,				US_INT,	0, P&hid1,	V1, 0, 0},	
	
	{" 10;"," LEIST-BEREICH 1"," kW    ", P&dis.leistgBereich[0],		US_INT, 0, P&hid1,	V1, 0, 0},
	{"*11;"," ZEIT LEISTBER 1"," h     ", P&did.leistgBereich_h[0],	US_INT,	0, P&hid1,	V1, 0, 0},
	{" 12;"," LEIST-BEREICH 2"," kW    ", P&dis.leistgBereich[1],		US_INT, 0, P&hid1,	V1, 0, 0},
	{"*13;"," ZEIT LEISTBER 2"," h     ", P&did.leistgBereich_h[1],	US_INT,	0, P&hid1,	V1, 0, 0},
	{" 14;"," LEIST-BEREICH 3"," kW    ", P&dis.leistgBereich[2],		US_INT, 0, P&hid1,	V1, 0, 0},
	{"*15;"," ZEIT LEISTBER 3"," h     ", P&did.leistgBereich_h[2],	US_INT,	0, P&hid1,	V1, 0, 0},
	{" 16;"," LEIST-BEREICH 4"," kW    ", P&dis.leistgBereich[3],		US_INT, 0, P&hid1,	V1, 0, 0},
	{"*17;"," ZEIT LEISTBER 4"," h     ", P&did.leistgBereich_h[3],	US_INT,	0, P&hid1,	V1, 0, 0},
	
	{" 20;"," VOL-BEREICH 1  "," m/h  ", P&dis.flowBereich[0],		 US_LONG,	3, P&hid1,	V1, 0, 0},
	{"*21;"," ZEIT VOLBER 1  "," h     ", P&did.flowBereich_h[0],		US_INT,	0, P&hid1,	V1, 0, 0},
	{" 22;"," VOL-BEREICH 2  "," m/h  ", P&dis.flowBereich[1],		 US_LONG,	3, P&hid1,	V1, 0, 0},
	{"*23;"," ZEIT VOLBER 2  "," h     ", P&did.flowBereich_h[1],		US_INT,	0, P&hid1,	V1, 0, 0},										 	
	{" 24;"," VOL-BEREICH 3  "," m/h  ", P&dis.flowBereich[2],		 US_LONG,	3, P&hid1,	V1, 0, 0},
	{"*25;"," ZEIT VOLBER 3  "," h     ", P&did.flowBereich_h[2],		US_INT,	0, P&hid1,	V1, 0, 0},										 	
	{" 26;"," VOL-BEREICH 4  "," m/h  ", P&dis.flowBereich[3],		 US_LONG,	3, P&hid1,	V1, 0, 0},
	{"*27:"," ZEIT VOLBER 4  "," h     ", P&did.flowBereich_h[3],		US_INT,	0, P&hid1,	V1, 0, 0},	

#if WWANZ > 0
	{"*30;"," ANZ. SP.-LADUNG","       ", P&did.wwlad_anz, 					US_INT, 0, P&hid1,	V1, 0, 0},
	{"*31;"," SP.-LADUNG MAX "," min   ", P&did.wwlad_time,					US_INT, 0, P&hid1,	V1, 0, 0},										 	
	{"*32;"," ANZ. WW-VORRANG","       ", P&did.wwvor_anz, 					US_INT, 0, P&hid1,	V1, 0, 0},
	{"*33;"," WW-VORRANG MAX "," min   ", P&did.wwvor_time,					US_INT, 0, P&hid1,	V1, 0, 0},
#endif

	{"*40;"," TVP MAXIMUM    "," C    ", P&did.tvpMax.temp, 				 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*41;"," DATUM TVP MAX  ","       ", P&did.tvpMax.tag,		 	 DATE_FORM, 1, P&hid1,	V1, 0, 0},		
	{"*42;"," Ta TVP MAX     "," C    ", P&did.tvpMax.ta,				ANA_FORM, 1, P&hid1,	V1, 0, 0},
	{"*44;"," TVP MINIMUM    "," C    ", P&did.tvpMin.temp, 				 S_INT, 1, P&hid1,	V1, 0, 0},
	{"*45;"," DATUM TVP MIN  ","       ", P&did.tvpMin.tag,		 	 DATE_FORM, 1, P&hid1,	V1, 0, 0},		
	{"*46;"," Ta TVP MIN     "," C    ", P&did.tvpMin.ta,				ANA_FORM, 1, P&hid1,	V1, 0, 0},														 	
};

