// Diese Datei wurde automatisch am : 04.04.2018 um 13:29:22 generiert und entspricht der Systemversion nach 19.07.2017 10:21:03!!!
// Path: C:\RIEcon36C\Softwarepool\Softwarepool_PES_2018\WPU_2018\WP_1_000_100000\User\Komtabparser.exe

#include "defins.h"
#include "struct.h"
#include "ustruct.h"
#include "sramext.h"
#include "uconstext.h"
#include "uramext.h"
#include "konvform.h"

const kotab KomTab[512] =  {
	{   0,   0},	//	  0=anl[] {"***:"," RIEDEL  R66E   ","       ", P&Kategorie_Typ,				ASCII_FORM, 0, P&vis,		V1, 0, 0},
	{   0,   1},	//	  1=anl[] {" 01:"," UHRZEIT        ","       ",				0,									UHR_FORM, 0, P&vis,		V1, 0, 0},
	{   0,   2},	//	  2=anl[] {" 02:"," DATUM          ","       ",				0,								DATUM_FORM, 0, P&vis,		V1, 0, 0},
	{   0,   3},	//	  3=anl[] {"*03:"," BETRIEBSSTUNDEN"," h     ", P&bst,											US_INT, 0, P&vis,		V1, 0, 0},
	{   0,   4},	//	  4=anl[] {" ->;"," leerData1      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
	{   0,   5},	//	  5=anl[] {" ->;"," leerData2      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
	{   0,   6},	//	  6=anl[] {" ->;"," leerData3      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
	{   0,   7},	//	  7=anl[] {" ->;"," leerData4      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
	{   0,   8},	//	  8=anl[] {" ->;"," leerSend1      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
	{   0,   9},	//	  9=anl[] {" ->;"," leerSend2      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
	{   0,  10},	//	 10=anl[] {" ->;"," leerSend3      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
	{   0,  11},	//	 11=anl[] {" ->;"," leerSend4      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
	{   0,  13},	//	 12=anl[] {" 12:"," AUSSENLUFTTEMP."," C    ", P&ta1m,										ANA_FORM, 1, P&vis,		V1, 0, 0},
	{   0,  14},	//	 13=anl[] {" 12:"," AUSSENLUFTTEMP."," C    ", P&ta1m,							 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{   0,  15},	//	 14=anl[] {" 13:"," DAEMPFUNGSZEIT "," min   ", P&Ta1mzk,									 US_CHAR, 0, P&vis,		V1, 0, 0},
	{   0,  30},	//	 15=anl[] {"*14:"," AUSSENT. MITTEL"," C    ", P&ta1mh,									ANA_FORM, 1, P&vis,		V1, 0, 0},
	{   0,  31},	//	 16=anl[] {"*14:"," AUSSENT. MITTEL"," C    ", P&ta1mh,							 ANA_FORM, 0x81, P&kom,		V1, 0, 0},
	{   0,  32},	//	 17=anl[] {" 15:"," MITTLUNGS-ZEIT "," h     ", P&Ta1mhzk,								 US_CHAR, 0, P&vis,		V1, 0, 0},
	{   0,  33},	//	 18=anl[] {" 15;"," MITTLUNGS-INIT "," C    ", P&ta1mh_temp,								 S_INT, 1, P&hid1,	V1, 0, 0},
	{   0,  34},	//	 19=anl[] {" 16:"," Ta-WINTER [EIN]"," C    ", P&Tae,											US_INT, 1, P&vis,		V1, 0, 0},
	{   0,  35},	//	 20=anl[] {" 17:"," Ta-SOMMER [AUS]"," C    ", P&Taa,											US_INT, 1, P&vis,		V1, 0, 0},
	{   0,  36},	//	 21=anl[] {"*18:"," SOMMER ?       ","       ", P&sowi,			 					 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{   0, 104},	//	 22=anl[] {"*80:"," SCHALTER HAND ?","       ", P&HardHand,							 JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{   0, 105},	//	 23=anl[] {"*81:"," R66 SCHALTER1-6","       ", P&sw_stat_info,						DYN_ASCII_FORM, 6, P&vis,		V1, 0, 0},
	{   0, 106},	//	 24=anl[] {"*90:"," SMGRUP STANDARD","       ", P&sstm[STANDARD],			 	US_CHAR, 0, P&vis,		A1, GRUP|STANDARD, 0},
	{   0, 107},	//	 25=anl[] {"*91:"," SMGRUP FUEHLER ","       ", P&sstm[FUEHLER],			 	US_CHAR, 0, P&vis,		A1, GRUP|FUEHLER,  0},
	{   0, 108},	//	 26=anl[] {"*92:"," SMGRUP MODULE  ","       ", P&sstm[MODULE],			 		US_CHAR, 0, P&vis,		A1, GRUP|MODULE,   0},
	{   0, 109},	//	 27=anl[] {"*->:"," SSTM ALARME    ","       ", P&sstm_alarme,		 	JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{   0, 110},	//	 28=anl[] {"*->:"," SSTM AL+FUEHLER","       ", P&sstm_all,		 			JANEIN_FORM, 0, P&vis,		V1, 0, 0},
	{   0, 111},	//	 29=anl[] {"*95:"," ANFORDERUNG MAX"," C    ", P&maxAnford,				 				 S_INT, 1, P&vis, 	V1, 0, 0},
	{   0, 117},	//	 30=anl[] {" 96:"," QUITTIER-TASTE "," EIN=1 ", P&quit_taste,			 				US_CHAR,  0, P&hid1,	V1, 0, 0},
	{   1,   0},	//	 31=wp1[] {"***:"," WAERMEPUMPE    ","       ", P&Wpmod,							ASCII_FORM, 0, P&vis,		V1},
	{   1,   1},	//	 32=wp1[] {"*01:"," TV-WWP         "," C    ", P&TV_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,   2},	//	 33=wp1[] {"*01:"," TV-WWP         "," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,   3},	//	 34=wp1[] {"*02:"," TR-WWP         "," C    ", P&TR_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,   4},	//	 35=wp1[] {"*02:"," TR-WWP         "," C    ", P&TR_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,   5},	//	 36=wp1[] {"*03:"," TSPo-WWP       "," C    ", P&TSPo_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,   6},	//	 37=wp1[] {"*03:"," TSPo-WWP       "," C    ", P&TSPo_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,   7},	//	 38=wp1[] {"*04:"," TSPu-WWP       "," C    ", P&TSPu_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,   8},	//	 39=wp1[] {"*04:"," TSPu-WWP       "," C    ", P&TSPu_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,   9},	//	 40=wp1[] {"*05:"," TVST-WWP       "," C    ", P&TVST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,  10},	//	 41=wp1[] {"*05:"," TVST-WWP       "," C    ", P&TVST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  11},	//	 42=wp1[] {"*06:"," TRST-WWP       "," C    ", P&TRST_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,  12},	//	 43=wp1[] {"*06:"," TRST-WWP       "," C    ", P&TRST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  13},	//	 44=wp1[] {"*07:"," TVQ-WWP        "," C    ", P&TVQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,  14},	//	 45=wp1[] {"*07:"," TVQ-WWP        "," C    ", P&TVQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  15},	//	 46=wp1[] {"*08:"," TRQ-WWP        "," C    ", P&TRQ_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,  16},	//	 47=wp1[] {"*08:"," TRQ-WWP        "," C    ", P&TRQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  17},	//	 48=wp1[] {"*09:"," TVH-WWP        "," C    ", P&TVH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,  18},	//	 49=wp1[] {"*09:"," TVH-WWP        "," C    ", P&TVH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  19},	//	 50=wp1[] {"*10:"," TRH-WWP        "," C    ", P&TRH_WP[WP1],						 	ANA_FORMP, 1, P&vis,		V1, 0, 0},
	{   1,  20},	//	 51=wp1[] {"*10:"," TRH-WWP        "," C    ", P&TRH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  21},	//	 52=wp1[] {"*11:"," BM WWP         ","       ", P&BM_UNI[U1],		 				JANEIN_FORMIP, 2, P&vis,		V1, 0, 0},
	{   1,  22},	//	 53=wp1[] {"*12:"," SM WWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	{   1,  23},	//	 54=wp1[] {"*13:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],						ANA_FORM, 1, P&vis,		V1, 0, 0},
	{   1,  24},	//	 55=wp1[] {"*13:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],	 		 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{   1,  36},	//	 56=wp1[] {" 17;"," Manu Sollwert  "," HAND=1", P&wps[WP1].Para_Manu_Sollwert, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{   1,  37},	//	 57=wp1[] {" ->;"," Sollwert       "," C    ", P&wps[WP1].T_manu_Sollwert,US_INT, 1, P&hid1,	V1, 0, 0},
	{   1,  38},	//	 58=wp1[] {"*20:"," Sollwert AA    "," C    ", P&maxAnford,				 				 S_INT, 1, P&vis, 	V1, 0, 0},
	{   1,  44},	//	 59=wp1[] {"*21:"," FREIGABE WWP   ","       ", P&DA_UNI[U1],	 			   JANEIN_FORMOP, 2, P&vis,		V1, 0, 0},
	{   1,  45},	//	 60=wp1[] {" ->;"," Manu FREIG WPU "," HAND=1", P&wps[WP1].WPU_Freigabe_Haut, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{   1,  46},	//	 61=wp1[] {" ->:"," WPU EIN/AUS    "," EIN=1 ", P&wps[WP1].WPU_Freigabe_Hand_stellen, US_CHAR, 0, P&hid1,	V1, 0, 0},
	{   1,  47},	//	 62=wp1[] {"*30:"," Quellenschutz ?","       ", P&wpd[WP1].Status_Quellenschutz, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{   1,  48},	//	 63=wp1[] {" ->;"," Quellenminimum "," C    ", P&wps[WP1].intPa_Quellentemperaturminimum,	S_INT, 1, P&hid1,	V1, 0, 0},
	{   1,  50},	//	 64=wp1[] {"*31:"," Frostschutz   ?","       ", P&wpd[WP1].Status_Frostschutz, JANEIN_FORM, 2, P&vis,		V1, 0, 0},
	{   1,  51},	//	 65=wp1[] {" ->;"," Speicherminimum"," C    ", P&wps[WP1].intPa_Speichertemperaturminimum,	S_INT, 1, P&hid1,	V1, 0, 0},
	{   3,  33},	//	 66=zlt[] {"*19;"," OBJ-ORT FEHLT ?","       ", P&ort_idf,									 JANEIN_FORM, 0, P&hid1,	E1, ANZEIGE, 0},
	{   3,  34},	//	 67=zlt[] {"*20;"," STATION-ADRESSE","       ", P&StationsAdr,								 	 US_CHAR, 0, P&hid1,	V1, 0, 0},
	{   3,  36},	//	 68=zlt[] {"021;"," MASTER -IDF    ","       ", P&Master_Idf,								 TELE_FORM, 0, P&hid1,	V1, 0, 0},
	{   3,  37},	//	 69=zlt[] {"022;"," STATION-IDF    ","       ", P&Station_Idf,								 TELE_FORM, 0, P&hid1,	V1, 0, 0},
	{   3,  38},	//	 70=zlt[] {"023;"," ALARM-IDF 1    ","       ", P&Alarm_Idf[0],							 TELE_FORM, 0, P&hid1,	V1, 0, 0},
	{   3,  39},	//	 71=zlt[] {"024;"," ALARM-IDF 2    ","       ", P&Alarm_Idf[1],							 TELE_FORM, 0, P&hid1,	V1, 0, 0},
	{   3,  40},	//	 72=zlt[] {"025;"," ALARM-IDF 3    ","       ", P&Alarm_Idf[2],							 TELE_FORM, 0, P&hid1,	V1, 0, 0},
	{   3,  41},	//	 73=zlt[] {"026;"," SERVICE-IDF    ","       ", P&Service_Idf,								 TELE_FORM, 0, P&hid1,	V1, 0, 0},
	{   3,  48},	//	 74=zlt[] {"*33;"," PIN FEHLT ?    ","       ", P&pin_fehlt,								 JANEIN_FORM, 0, P&hid1,	E1, ANZEIGE, 0},
	{   6,  67},	//	 75=sys[] {"*53;"," DAT-ERROR R38_1","       ", P&mod38[0].error,	 				 JANEIN_FORM, 0, P&r38vis[0],	E1, MODULE, 0},
	{   6,  84},	//	 76=sys[] {"*80;"," RESET COUNTER  ","       ", P&Resetcnt,							US_INT, 0, P&hid1,	V1, 0, 0},
	{   6,  88},	//	 77=sys[] {"*82;"," RTC-UHR ERROR  ","       ", P&rtc_error,				 JANEIN_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{   6,  89},	//	 78=sys[] {"*83;"," MAIN-CRASH:TASK","       ", P&noi_reanimate,			 TASK_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{   6,  95},	//	 79=sys[] {" ->;"," SDM-ARCHIV-INIT","       ", P&werks_sdm_code,		  US_INT,  2, P&hid1,	V1, 0, 0},
	{   6,  96},	//	 80=sys[] {" ->;"," SDM-DAE-INIT   "," -DAE,E", P&reset_dae,			    	  S_INT, 1, P&hid1,	V1, 0, 0},
	{   6,  98},	//	 81=sys[] {"*->;"," FerroRAM2 FEHLT","       ", P&ferro_err,			  JANEIN_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{   6,  99},	//	 82=sys[] {"*->;"," SDHC-Card FEHLT","       ", P&card_err,			  JANEIN_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{   6, 110},	//	 83=sys[] {"*96;"," PROGRAMM-TYP   ","       ", P&Version_PTyp, 			ASCII_FORM, 0, P&hid1,	V1, 0, 0},
	{   6, 111},	//	 84=sys[] {"*97;"," PROGR.-VERSION ","       ", P&Version_PVers,			 DATE_FORM, 0, P&hid1,	V1, 0, 0},
	{   6, 112},	//	 85=sys[] {"*98;"," SYSTEM-VERSION ","       ", P&Version_SVers, 		 DATE_FORM, 0, P&hid1,	V1, 0, 0},
	{   0,   0},	//	 86=Fuellbytes
	{   0,   0},	//	 87=Fuellbytes
	{   0,   0},	//	 88=Fuellbytes
	{   0,   0},	//	 89=Fuellbytes
	{   0,   0},	//	 90=Fuellbytes
	{   0,   0},	//	 91=Fuellbytes
	{   0,   0},	//	 92=Fuellbytes
	{   0,   0},	//	 93=Fuellbytes
	{   0,   0},	//	 94=Fuellbytes
	{   0,   0},	//	 95=Fuellbytes
	{   0,   0},	//	 96=Fuellbytes
	{   0,   0},	//	 97=Fuellbytes
	{   0,   0},	//	 98=Fuellbytes
	{   0,   0},	//	 99=Fuellbytes
	{   0,   0},	//	100=Fuellbytes
	{   0,   0},	//	101=Fuellbytes
	{   0,   0},	//	102=Fuellbytes
	{   0,   0},	//	103=Fuellbytes
	{   0,   0},	//	104=Fuellbytes
	{   0,   0},	//	105=Fuellbytes
	{   0,   0},	//	106=Fuellbytes
	{   0,   0},	//	107=Fuellbytes
	{   0,   0},	//	108=Fuellbytes
	{   0,   0},	//	109=Fuellbytes
	{   0,   0},	//	110=Fuellbytes
	{   0,   0},	//	111=Fuellbytes
	{   0,   0},	//	112=Fuellbytes
	{   0,   0},	//	113=Fuellbytes
	{   0,   0},	//	114=Fuellbytes
	{   0,   0},	//	115=Fuellbytes
	{   0,   0},	//	116=Fuellbytes
	{   0,   0},	//	117=Fuellbytes
	{   0,   0},	//	118=Fuellbytes
	{   0,   0},	//	119=Fuellbytes
	{   0,   0},	//	120=Fuellbytes
	{   0,   0},	//	121=Fuellbytes
	{   0,   0},	//	122=Fuellbytes
	{   0,   0},	//	123=Fuellbytes
	{   0,   0},	//	124=Fuellbytes
	{   0,   0},	//	125=Fuellbytes
	{   0,   0},	//	126=Fuellbytes
	{   0,   0},	//	127=Fuellbytes
	{   0,   0},	//	128=Fuellbytes
	{   0,   0},	//	129=Fuellbytes
	{   0,   0},	//	130=Fuellbytes
	{   0,   0},	//	131=Fuellbytes
	{   0,   0},	//	132=Fuellbytes
	{   0,   0},	//	133=Fuellbytes
	{   0,   0},	//	134=Fuellbytes
	{   0,   0},	//	135=Fuellbytes
	{   0,   0},	//	136=Fuellbytes
	{   0,   0},	//	137=Fuellbytes
	{   0,   0},	//	138=Fuellbytes
	{   0,   0},	//	139=Fuellbytes
	{   0,   0},	//	140=Fuellbytes
	{   0,   0},	//	141=Fuellbytes
	{   0,   0},	//	142=Fuellbytes
	{   0,   0},	//	143=Fuellbytes
	{   0,   0},	//	144=Fuellbytes
	{   0,   0},	//	145=Fuellbytes
	{   0,   0},	//	146=Fuellbytes
	{   0,   0},	//	147=Fuellbytes
	{   0,   0},	//	148=Fuellbytes
	{   0,   0},	//	149=Fuellbytes
	{   0,   0},	//	150=Fuellbytes
	{   0,   0},	//	151=Fuellbytes
	{   0,   0},	//	152=Fuellbytes
	{   0,   0},	//	153=Fuellbytes
	{   0,   0},	//	154=Fuellbytes
	{   0,   0},	//	155=Fuellbytes
	{   0,   0},	//	156=Fuellbytes
	{   0,   0},	//	157=Fuellbytes
	{   0,   0},	//	158=Fuellbytes
	{   0,   0},	//	159=Fuellbytes
	{   0,   0},	//	160=Fuellbytes
	{   0,   0},	//	161=Fuellbytes
	{   0,   0},	//	162=Fuellbytes
	{   0,   0},	//	163=Fuellbytes
	{   0,   0},	//	164=Fuellbytes
	{   0,   0},	//	165=Fuellbytes
	{   0,   0},	//	166=Fuellbytes
	{   0,   0},	//	167=Fuellbytes
	{   0,   0},	//	168=Fuellbytes
	{   0,   0},	//	169=Fuellbytes
	{   0,   0},	//	170=Fuellbytes
	{   0,   0},	//	171=Fuellbytes
	{   0,   0},	//	172=Fuellbytes
	{   0,   0},	//	173=Fuellbytes
	{   0,   0},	//	174=Fuellbytes
	{   0,   0},	//	175=Fuellbytes
	{   0,   0},	//	176=Fuellbytes
	{   0,   0},	//	177=Fuellbytes
	{   0,   0},	//	178=Fuellbytes
	{   0,   0},	//	179=Fuellbytes
	{   0,   0},	//	180=Fuellbytes
	{   0,   0},	//	181=Fuellbytes
	{   0,   0},	//	182=Fuellbytes
	{   0,   0},	//	183=Fuellbytes
	{   0,   0},	//	184=Fuellbytes
	{   0,   0},	//	185=Fuellbytes
	{   0,   0},	//	186=Fuellbytes
	{   0,   0},	//	187=Fuellbytes
	{   0,   0},	//	188=Fuellbytes
	{   0,   0},	//	189=Fuellbytes
	{   0,   0},	//	190=Fuellbytes
	{   0,   0},	//	191=Fuellbytes
	{   0,   0},	//	192=Fuellbytes
	{   0,   0},	//	193=Fuellbytes
	{   0,   0},	//	194=Fuellbytes
	{   0,   0},	//	195=Fuellbytes
	{   0,   0},	//	196=Fuellbytes
	{   0,   0},	//	197=Fuellbytes
	{   0,   0},	//	198=Fuellbytes
	{   0,   0},	//	199=Fuellbytes
	{   0,   0},	//	200=Fuellbytes
	{   0,   0},	//	201=Fuellbytes
	{   0,   0},	//	202=Fuellbytes
	{   0,   0},	//	203=Fuellbytes
	{   0,   0},	//	204=Fuellbytes
	{   0,   0},	//	205=Fuellbytes
	{   0,   0},	//	206=Fuellbytes
	{   0,   0},	//	207=Fuellbytes
	{   0,   0},	//	208=Fuellbytes
	{   0,   0},	//	209=Fuellbytes
	{   0,   0},	//	210=Fuellbytes
	{   0,   0},	//	211=Fuellbytes
	{   0,   0},	//	212=Fuellbytes
	{   0,   0},	//	213=Fuellbytes
	{   0,   0},	//	214=Fuellbytes
	{   0,   0},	//	215=Fuellbytes
	{   0,   0},	//	216=Fuellbytes
	{   0,   0},	//	217=Fuellbytes
	{   0,   0},	//	218=Fuellbytes
	{   0,   0},	//	219=Fuellbytes
	{   0,   0},	//	220=Fuellbytes
	{   0,   0},	//	221=Fuellbytes
	{   0,   0},	//	222=Fuellbytes
	{   0,   0},	//	223=Fuellbytes
	{   0,   0},	//	224=Fuellbytes
	{   0,   0},	//	225=Fuellbytes
	{   0,   0},	//	226=Fuellbytes
	{   0,   0},	//	227=Fuellbytes
	{   0,   0},	//	228=Fuellbytes
	{   0,   0},	//	229=Fuellbytes
	{   0,   0},	//	230=Fuellbytes
	{   0,   0},	//	231=Fuellbytes
	{   0,   0},	//	232=Fuellbytes
	{   0,   0},	//	233=Fuellbytes
	{   0,   0},	//	234=Fuellbytes
	{   0,   0},	//	235=Fuellbytes
	{   0,   0},	//	236=Fuellbytes
	{   0,   0},	//	237=Fuellbytes
	{   0,   0},	//	238=Fuellbytes
	{   0,   0},	//	239=Fuellbytes
	{   0,   0},	//	240=Fuellbytes
	{   0,   0},	//	241=Fuellbytes
	{   0,   0},	//	242=Fuellbytes
	{   0,   0},	//	243=Fuellbytes
	{   0,   0},	//	244=Fuellbytes
	{   0,   0},	//	245=Fuellbytes
	{   0,   0},	//	246=Fuellbytes
	{   0,   0},	//	247=Fuellbytes
	{   0,   0},	//	248=Fuellbytes
	{   0,   0},	//	249=Fuellbytes
	{   0,   0},	//	250=Fuellbytes
	{   0,   0},	//	251=Fuellbytes
	{   0,   0},	//	252=Fuellbytes
	{   0,   0},	//	253=Fuellbytes
	{   0,   0},	//	254=Fuellbytes
	{   0,   0},	//	255=Fuellbytes
	{   0,   0},	//	256=Fuellbytes
	{   0,   0},	//	257=Fuellbytes
	{   0,   0},	//	258=Fuellbytes
	{   0,   0},	//	259=Fuellbytes
	{   0,   0},	//	260=Fuellbytes
	{   0,   0},	//	261=Fuellbytes
	{   0,   0},	//	262=Fuellbytes
	{   0,   0},	//	263=Fuellbytes
	{   0,   0},	//	264=Fuellbytes
	{   0,   0},	//	265=Fuellbytes
	{   0,   0},	//	266=Fuellbytes
	{   0,   0},	//	267=Fuellbytes
	{   0,   0},	//	268=Fuellbytes
	{   0,   0},	//	269=Fuellbytes
	{   0,   0},	//	270=Fuellbytes
	{   0,   0},	//	271=Fuellbytes
	{   0,   0},	//	272=Fuellbytes
	{   0,   0},	//	273=Fuellbytes
	{   0,   0},	//	274=Fuellbytes
	{   0,   0},	//	275=Fuellbytes
	{   0,   0},	//	276=Fuellbytes
	{   0,   0},	//	277=Fuellbytes
	{   0,   0},	//	278=Fuellbytes
	{   0,   0},	//	279=Fuellbytes
	{   0,   0},	//	280=Fuellbytes
	{   0,   0},	//	281=Fuellbytes
	{   0,   0},	//	282=Fuellbytes
	{   0,   0},	//	283=Fuellbytes
	{   0,   0},	//	284=Fuellbytes
	{   0,   0},	//	285=Fuellbytes
	{   0,   0},	//	286=Fuellbytes
	{   0,   0},	//	287=Fuellbytes
	{   0,   0},	//	288=Fuellbytes
	{   0,   0},	//	289=Fuellbytes
	{   0,   0},	//	290=Fuellbytes
	{   0,   0},	//	291=Fuellbytes
	{   0,   0},	//	292=Fuellbytes
	{   0,   0},	//	293=Fuellbytes
	{   0,   0},	//	294=Fuellbytes
	{   0,   0},	//	295=Fuellbytes
	{   0,   0},	//	296=Fuellbytes
	{   0,   0},	//	297=Fuellbytes
	{   0,   0},	//	298=Fuellbytes
	{   0,   0},	//	299=Fuellbytes
	{   0,   0},	//	300=Fuellbytes
	{   0,   0},	//	301=Fuellbytes
	{   0,   0},	//	302=Fuellbytes
	{   0,   0},	//	303=Fuellbytes
	{   0,   0},	//	304=Fuellbytes
	{   0,   0},	//	305=Fuellbytes
	{   0,   0},	//	306=Fuellbytes
	{   0,   0},	//	307=Fuellbytes
	{   0,   0},	//	308=Fuellbytes
	{   0,   0},	//	309=Fuellbytes
	{   0,   0},	//	310=Fuellbytes
	{   0,   0},	//	311=Fuellbytes
	{   0,   0},	//	312=Fuellbytes
	{   0,   0},	//	313=Fuellbytes
	{   0,   0},	//	314=Fuellbytes
	{   0,   0},	//	315=Fuellbytes
	{   0,   0},	//	316=Fuellbytes
	{   0,   0},	//	317=Fuellbytes
	{   0,   0},	//	318=Fuellbytes
	{   0,   0},	//	319=Fuellbytes
	{   0,   0},	//	320=Fuellbytes
	{   0,   0},	//	321=Fuellbytes
	{   0,   0},	//	322=Fuellbytes
	{   0,   0},	//	323=Fuellbytes
	{   0,   0},	//	324=Fuellbytes
	{   0,   0},	//	325=Fuellbytes
	{   0,   0},	//	326=Fuellbytes
	{   0,   0},	//	327=Fuellbytes
	{   0,   0},	//	328=Fuellbytes
	{   0,   0},	//	329=Fuellbytes
	{   0,   0},	//	330=Fuellbytes
	{   0,   0},	//	331=Fuellbytes
	{   0,   0},	//	332=Fuellbytes
	{   0,   0},	//	333=Fuellbytes
	{   0,   0},	//	334=Fuellbytes
	{   0,   0},	//	335=Fuellbytes
	{   0,   0},	//	336=Fuellbytes
	{   0,   0},	//	337=Fuellbytes
	{   0,   0},	//	338=Fuellbytes
	{   0,   0},	//	339=Fuellbytes
	{   0,   0},	//	340=Fuellbytes
	{   0,   0},	//	341=Fuellbytes
	{   0,   0},	//	342=Fuellbytes
	{   0,   0},	//	343=Fuellbytes
	{   0,   0},	//	344=Fuellbytes
	{   0,   0},	//	345=Fuellbytes
	{   0,   0},	//	346=Fuellbytes
	{   0,   0},	//	347=Fuellbytes
	{   0,   0},	//	348=Fuellbytes
	{   0,   0},	//	349=Fuellbytes
	{   0,   0},	//	350=Fuellbytes
	{   0,   0},	//	351=Fuellbytes
	{   0,   0},	//	352=Fuellbytes
	{   0,   0},	//	353=Fuellbytes
	{   0,   0},	//	354=Fuellbytes
	{   0,   0},	//	355=Fuellbytes
	{   0,   0},	//	356=Fuellbytes
	{   0,   0},	//	357=Fuellbytes
	{   0,   0},	//	358=Fuellbytes
	{   0,   0},	//	359=Fuellbytes
	{   0,   0},	//	360=Fuellbytes
	{   0,   0},	//	361=Fuellbytes
	{   0,   0},	//	362=Fuellbytes
	{   0,   0},	//	363=Fuellbytes
	{   0,   0},	//	364=Fuellbytes
	{   0,   0},	//	365=Fuellbytes
	{   0,   0},	//	366=Fuellbytes
	{   0,   0},	//	367=Fuellbytes
	{   0,   0},	//	368=Fuellbytes
	{   0,   0},	//	369=Fuellbytes
	{   0,   0},	//	370=Fuellbytes
	{   0,   0},	//	371=Fuellbytes
	{   0,   0},	//	372=Fuellbytes
	{   0,   0},	//	373=Fuellbytes
	{   0,   0},	//	374=Fuellbytes
	{   0,   0},	//	375=Fuellbytes
	{   0,   0},	//	376=Fuellbytes
	{   0,   0},	//	377=Fuellbytes
	{   0,   0},	//	378=Fuellbytes
	{   0,   0},	//	379=Fuellbytes
	{   0,   0},	//	380=Fuellbytes
	{   0,   0},	//	381=Fuellbytes
	{   0,   0},	//	382=Fuellbytes
	{   0,   0},	//	383=Fuellbytes
	{   0,   0},	//	384=Fuellbytes
	{   0,   0},	//	385=Fuellbytes
	{   0,   0},	//	386=Fuellbytes
	{   0,   0},	//	387=Fuellbytes
	{   0,   0},	//	388=Fuellbytes
	{   0,   0},	//	389=Fuellbytes
	{   0,   0},	//	390=Fuellbytes
	{   0,   0},	//	391=Fuellbytes
	{   0,   0},	//	392=Fuellbytes
	{   0,   0},	//	393=Fuellbytes
	{   0,   0},	//	394=Fuellbytes
	{   0,   0},	//	395=Fuellbytes
	{   0,   0},	//	396=Fuellbytes
	{   0,   0},	//	397=Fuellbytes
	{   0,   0},	//	398=Fuellbytes
	{   0,   0},	//	399=Fuellbytes
	{   0,   0},	//	400=Fuellbytes
	{   0,   0},	//	401=Fuellbytes
	{   0,   0},	//	402=Fuellbytes
	{   0,   0},	//	403=Fuellbytes
	{   0,   0},	//	404=Fuellbytes
	{   0,   0},	//	405=Fuellbytes
	{   0,   0},	//	406=Fuellbytes
	{   0,   0},	//	407=Fuellbytes
	{   0,   0},	//	408=Fuellbytes
	{   0,   0},	//	409=Fuellbytes
	{   0,   0},	//	410=Fuellbytes
	{   0,   0},	//	411=Fuellbytes
	{   0,   0},	//	412=Fuellbytes
	{   0,   0},	//	413=Fuellbytes
	{   0,   0},	//	414=Fuellbytes
	{   0,   0},	//	415=Fuellbytes
	{   0,   0},	//	416=Fuellbytes
	{   0,   0},	//	417=Fuellbytes
	{   0,   0},	//	418=Fuellbytes
	{   0,   0},	//	419=Fuellbytes
	{   0,   0},	//	420=Fuellbytes
	{   0,   0},	//	421=Fuellbytes
	{   0,   0},	//	422=Fuellbytes
	{   0,   0},	//	423=Fuellbytes
	{   0,   0},	//	424=Fuellbytes
	{   0,   0},	//	425=Fuellbytes
	{   0,   0},	//	426=Fuellbytes
	{   0,   0},	//	427=Fuellbytes
	{   0,   0},	//	428=Fuellbytes
	{   0,   0},	//	429=Fuellbytes
	{   0,   0},	//	430=Fuellbytes
	{   0,   0},	//	431=Fuellbytes
	{   0,   0},	//	432=Fuellbytes
	{   0,   0},	//	433=Fuellbytes
	{   0,   0},	//	434=Fuellbytes
	{   0,   0},	//	435=Fuellbytes
	{   0,   0},	//	436=Fuellbytes
	{   0,   0},	//	437=Fuellbytes
	{   0,   0},	//	438=Fuellbytes
	{   0,   0},	//	439=Fuellbytes
	{   0,   0},	//	440=Fuellbytes
	{   0,   0},	//	441=Fuellbytes
	{   0,   0},	//	442=Fuellbytes
	{   0,   0},	//	443=Fuellbytes
	{   0,   0},	//	444=Fuellbytes
	{   0,   0},	//	445=Fuellbytes
	{   0,   0},	//	446=Fuellbytes
	{   0,   0},	//	447=Fuellbytes
	{   0,   0},	//	448=Fuellbytes
	{   0,   0},	//	449=Fuellbytes
	{   0,   0},	//	450=Fuellbytes
	{   0,   0},	//	451=Fuellbytes
	{   0,   0},	//	452=Fuellbytes
	{   0,   0},	//	453=Fuellbytes
	{   0,   0},	//	454=Fuellbytes
	{   0,   0},	//	455=Fuellbytes
	{   0,   0},	//	456=Fuellbytes
	{   0,   0},	//	457=Fuellbytes
	{   0,   0},	//	458=Fuellbytes
	{   0,   0},	//	459=Fuellbytes
	{   0,   0},	//	460=Fuellbytes
	{   0,   0},	//	461=Fuellbytes
	{   0,   0},	//	462=Fuellbytes
	{   0,   0},	//	463=Fuellbytes
	{   0,   0},	//	464=Fuellbytes
	{   0,   0},	//	465=Fuellbytes
	{   0,   0},	//	466=Fuellbytes
	{   0,   0},	//	467=Fuellbytes
	{   0,   0},	//	468=Fuellbytes
	{   0,   0},	//	469=Fuellbytes
	{   0,   0},	//	470=Fuellbytes
	{   0,   0},	//	471=Fuellbytes
	{   0,   0},	//	472=Fuellbytes
	{   0,   0},	//	473=Fuellbytes
	{   0,   0},	//	474=Fuellbytes
	{   0,   0},	//	475=Fuellbytes
	{   0,   0},	//	476=Fuellbytes
	{   0,   0},	//	477=Fuellbytes
	{   0,   0},	//	478=Fuellbytes
	{   0,   0},	//	479=Fuellbytes
	{   0,   0},	//	480=Fuellbytes
	{   0,   0},	//	481=Fuellbytes
	{   0,   0},	//	482=Fuellbytes
	{   0,   0},	//	483=Fuellbytes
	{   0,   0},	//	484=Fuellbytes
	{   0,   0},	//	485=Fuellbytes
	{   0,   0},	//	486=Fuellbytes
	{   0,   0},	//	487=Fuellbytes
	{   0,   0},	//	488=Fuellbytes
	{   0,   0},	//	489=Fuellbytes
	{   0,   0},	//	490=Fuellbytes
	{   0,   0},	//	491=Fuellbytes
	{   0,   0},	//	492=Fuellbytes
	{   0,   0},	//	493=Fuellbytes
	{   0,   0},	//	494=Fuellbytes
	{   0,   0},	//	495=Fuellbytes
	{   0,   0},	//	496=Fuellbytes
	{   0,   0},	//	497=Fuellbytes
	{   0,   0},	//	498=Fuellbytes
	{   0,   0},	//	499=Fuellbytes
	{   0,   0},	//	500=Fuellbytes
	{   0,   0},	//	501=Fuellbytes
	{   0,   0},	//	502=Fuellbytes
	{   0,   0},	//	503=Fuellbytes
	{   0,   0},	//	504=Fuellbytes
	{   0,   0},	//	505=Fuellbytes
	{   0,   0},	//	506=Fuellbytes
	{   0,   0},	//	507=Fuellbytes
	{   0,   0},	//	508=Fuellbytes
	{   0,   0},	//	509=Fuellbytes
	{   0,   0},	//	510=Fuellbytes
	{   0,   0},	//	511=Fuellbytes
};
const unsigned int KOMTAB_LENG = 86;

// Alarmtabelle mit mindestens 7 Alarmen					
const events Pe_Tab[] = {
	// stat, dae, count 
	{DAE_YES ,  24, 0}, //   1=anl[] {"*90:"," SMGRUP STANDARD","       ", P&sstm[STANDARD],			 	US_CHAR, 0, P&vis,		A1, GRUP|STANDARD, 0},
	{DAE_YES ,  25, 0}, //   2=anl[] {"*91:"," SMGRUP FUEHLER ","       ", P&sstm[FUEHLER],			 	US_CHAR, 0, P&vis,		A1, GRUP|FUEHLER,  0},
	{DAE_YES ,  26, 0}, //   3=anl[] {"*92:"," SMGRUP MODULE  ","       ", P&sstm[MODULE],			 		US_CHAR, 0, P&vis,		A1, GRUP|MODULE,   0},
	{DAE_YES ,  53, 0}, //   4=wp1[] {"*12:"," SM WWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	{DAE_no  ,   0, 0},	//   5=Fuellbytes
	{DAE_no  ,   0, 0},	//   6=Fuellbytes
	{DAE_no  ,   0, 0},	//   7=Fuellbytes
};
const unsigned char PETAB_TABL = 7;

// Ereignisstabelle
const events Se_Tab[] = {
	// stat, dae, count 
	{DAE_YES ,  13, 0}, //   8/  9   1=anl[] {" 12:"," AUSSENLUFTTEMP."," C    ", P&ta1m,							 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  24, 0}, //  10/ 11   2=anl[] {"*90:"," SMGRUP STANDARD","       ", P&sstm[STANDARD],			 	US_CHAR, 0, P&vis,		A1, GRUP|STANDARD, 0},
	{DAE_YES ,  25, 0}, //  12/ 13   3=anl[] {"*91:"," SMGRUP FUEHLER ","       ", P&sstm[FUEHLER],			 	US_CHAR, 0, P&vis,		A1, GRUP|FUEHLER,  0},
	{DAE_YES ,  26, 0}, //  14/ 15   4=anl[] {"*92:"," SMGRUP MODULE  ","       ", P&sstm[MODULE],			 		US_CHAR, 0, P&vis,		A1, GRUP|MODULE,   0},
	{DAE_YES ,  33, 0}, //  16/ 17   5=wp1[] {"*01:"," TV-WWP         "," C    ", P&TV_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  35, 0}, //  18/ 19   6=wp1[] {"*02:"," TR-WWP         "," C    ", P&TR_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  37, 0}, //  20/ 21   7=wp1[] {"*03:"," TSPo-WWP       "," C    ", P&TSPo_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  39, 0}, //  22/ 23   8=wp1[] {"*04:"," TSPu-WWP       "," C    ", P&TSPu_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  41, 0}, //  24/ 25   9=wp1[] {"*05:"," TVST-WWP       "," C    ", P&TVST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  43, 0}, //  26/ 27  10=wp1[] {"*06:"," TRST-WWP       "," C    ", P&TRST_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  45, 0}, //  28/ 29  11=wp1[] {"*07:"," TVQ-WWP        "," C    ", P&TVQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  47, 0}, //  30/ 31  12=wp1[] {"*08:"," TRQ-WWP        "," C    ", P&TRQ_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  49, 0}, //  32/ 33  13=wp1[] {"*09:"," TVH-WWP        "," C    ", P&TVH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  51, 0}, //  34/ 35  14=wp1[] {"*10:"," TRH-WWP        "," C    ", P&TRH_WP[WP1],					 ANA_FORMP, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  53, 0}, //  36/ 37  15=wp1[] {"*12:"," SM WWP         ","       ", P&SM_UNI[U1], 		 			JANEIN_FORMIP, 0, P&vis,		A1, EINZEL, 0},
	{DAE_YES ,  55, 0}, //  38/ 39  16=wp1[] {"*13:"," ANFORDERUNG BUS"," C    ", P&anfExt[0],	 		 	 ANA_FORM, 0x81, P&kom,		E1, FUEHLER, 0},
	{DAE_YES ,  66, 0}, //  40/ 41  17=zlt[] {"*19;"," OBJ-ORT FEHLT ?","       ", P&ort_idf,									 JANEIN_FORM, 0, P&hid1,	E1, ANZEIGE, 0},
	{DAE_YES ,  74, 0}, //  42/ 43  18=zlt[] {"*33;"," PIN FEHLT ?    ","       ", P&pin_fehlt,								 JANEIN_FORM, 0, P&hid1,	E1, ANZEIGE, 0},
	{DAE_YES ,  75, 0}, //  44/ 45  19=sys[] {"*53;"," DAT-ERROR R38_1","       ", P&mod38[0].error,	 				 JANEIN_FORM, 0, P&r38vis[0],	E1, MODULE, 0},
	{DAE_YES ,  77, 0}, //  46/ 47  20=sys[] {"*82;"," RTC-UHR ERROR  ","       ", P&rtc_error,				 JANEIN_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{DAE_YES ,  78, 0}, //  48/ 49  21=sys[] {"*83;"," MAIN-CRASH:TASK","       ", P&noi_reanimate,			 TASK_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{DAE_YES ,  81, 0}, //  50/ 51  22=sys[] {"*->;"," FerroRAM2 FEHLT","       ", P&ferro_err,			  JANEIN_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
	{DAE_YES ,  82, 0}, //  52/ 53  23=sys[] {"*->;"," SDHC-Card FEHLT","       ", P&card_err,			  JANEIN_FORM, 0, P&hid1,	E1, EREIGNIS, 0},
};
const unsigned char SETAB_TABL = 23;
