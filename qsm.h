const Pgrup qsm[] = {
	{"***:"," QUITTIERBARE   ","       ", P&Stoermeld,							 ASCII_FORM, 0, P&vis,	V1, 0, 0},
	{"*01:"," QSM VORHANDEN ?","       ", P&un_qsm,			 						JANEIN_FORM, 0, P&vis,	V1, 0, 0},

//**PARSER-BEGIN-qsm[]**  AUTOMATISCHE ERZEUGUNG *****
// Diese Zeilen wurde automatisch am : 03.09.2018 um 14:55:35 generiert und entspricht der Systemversion nach 19.07.2017 10:21:03!!!
// Path: C:\RIEcon36C\Softwarepool\Softwarepool_PES_2018\WPU_2018\WP_1_000_100000\User\Komtabparser.exe
	{"ANL:"," SMGRUP STANDARD","       ", P&alarmtab[0].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"ANL:"," SMGRUP FUEHLER ","       ", P&alarmtab[1].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"ANL:"," SMGRUP MODULE  ","       ", P&alarmtab[2].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TV-AWP         ","       ", P&alarmtab[3].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TR-AWP         ","       ", P&alarmtab[4].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TSPo-AWP       ","       ", P&alarmtab[5].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TSPm-AWP       ","       ", P&alarmtab[6].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TSPu-AWP       ","       ", P&alarmtab[7].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TVQ-AWP        ","       ", P&alarmtab[8].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TRQ-AWP        ","       ", P&alarmtab[9].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TVHG-AWP       ","       ", P&alarmtab[10].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," TRHG-AWP       ","       ", P&alarmtab[11].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"AWP:"," SM AWP         ","       ", P&alarmtab[12].alspeicher,		JANEIN_FORM, 0, P&vis,	V1, 0, 0},
	{"AWP:"," ANFORDERUNG BUS","       ", P&alarmtab[13].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"ZLT;"," OBJ-ORT FEHLT ?","       ", P&alarmtab[14].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"ZLT;"," PIN FEHLT ?    ","       ", P&alarmtab[15].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"SYS;"," DAT-ERROR R38_1","       ", P&alarmtab[16].alspeicher,		JANEIN_FORM, 0, P&vis,	V1, 0, 0},
	{"SYS;"," RTC-UHR ERROR  ","       ", P&alarmtab[17].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"SYS;"," MAIN-CRASH:TASK","       ", P&alarmtab[18].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"SYS;"," FerroRAM2 FEHLT","       ", P&alarmtab[19].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
	{"SYS;"," SDHC-Card FEHLT","       ", P&alarmtab[20].alspeicher,		JANEIN_FORM, 0, P&hid3,	V0, 0, 0},
//**PARSER-ENDE-qsm[]**  AUTOMATISCHE ERZEUGUNG *****

	{" 99:"," QUITTIER-TASTE "," EIN=1 ", P&quit_taste,			 						US_CHAR, 0, P&vis,	V1, 0, 0},
	{" ->;"," QUITTIER-TASTE ","       ", P&QUITTS[0],			 			JANEIN_FORMIP, 2, P&hid4,	V0, 0, 0},	// für erweiterte In-/Out Anzeige in ANL: 

};

