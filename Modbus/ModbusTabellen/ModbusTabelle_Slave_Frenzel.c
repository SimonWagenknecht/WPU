#include "defins.h"
#include "struct.h"
#include "projdef.h"
#include "sramext.h"
#include "uramext.h"
#include "ustruct.h"
#include "userdef.h"
#include "uconstext.h"
#include "konvform.h"
#include "modbus.h"


#if MODBUS_UNI > 0

//----------------------------------------------------------------------------------------------
// Aufruf von "InitEA.c"
//----------------------------------------------------------------------------------------------
void fill_dummy_modbus(void)
{

}

//-----------------------------------
// Modbus-Master
//-----------------------------------
#if MODBUS_MASTER == 1
const modbusTableRow modbusTable[] = {
// {Adresse des           {Riedel Typ,      {Modbus Typ,     Spezial-   Modbus  Register-    Register-   Operation,          Intervall,AutoSend}
//  Datenpunkts                   Komma},     Komma     },   Konvert.,  Device  adresse,     anzahl,                         [sek]
//                                                                      idx
    {P&ae_rt[0],          {ANA_FORM,  1},   {S_INT_16, 1},   NONE,      0,          0x100,       1,          READ_HOLDING_REGS,  1,        TRUE},  // Raumtemperatur
    {P&ae_co2[0],         {ANA_FORM , 0},   {S_INT_16, 0},   NONE,      0,          0x102,       1,          READ_HOLDING_REGS,  1,        TRUE},  // CO2
                                                                        
    {P&ae_rt[1],          {ANA_FORM,  1},   {S_INT_16, 1},   NONE,      1,          0x100,       1,          READ_HOLDING_REGS,  1,        TRUE},  // Raumtemperatur
    {P&ae_co2[1],         {ANA_FORM , 0},   {S_INT_16, 0},   NONE,      1,          0x102,       1,          READ_HOLDING_REGS,  1,        TRUE},  // CO2
                                                                        
    {P&ae_rt[2],          {ANA_FORM , 1},   {S_INT_16, 1},   NONE,      2,          0x100,       1,          READ_HOLDING_REGS,  1,        TRUE},  // Raumtemperatur
    {P&ae_co2[2],         {ANA_FORM , 0},   {S_INT_16, 0},   NONE,      2,          0x102,       1,          READ_HOLDING_REGS,  1,        TRUE},  // CO2
                                                                        

};
const unsigned int modbusMasterTableSize = 0;//(sizeof(modbusTable)/sizeof(modbusTableRow));
modbusTableRowData modbusTableData[1];

#else	// kein Master definiert

const modbusTableRow modbusTable[] = {
};
const unsigned int modbusMasterTableSize = 0;
modbusTableRowData modbusTableData[1];
#endif

//-----------------------------------
// Slave	Beispiel Andreas Solar
//-----------------------------------
#if MODBUS_SLAVE == 1
const modbusSlaveTableRow modbusSlaveTable[] = {
//{Adresse des Datenpunkts,			{RiedelTyp,					{ModbusTyp,		Konver-,	Reg.,	Reg., Berechtigung, Time-,Fallback}	// Datenpunkt-Bezeichnung
//																					Komma}				Komma}	tierung		Adr.	Anz.								out
	{P&Version_PTyp[0], 					{ASCII_FORM,		0},	{BINARY, 	0}, 	NONE, 	0, 		8, 		READ_ONLY,		0, 		FALSE}, //" PROGRAMM-TYP   " zB:"FW-R-1s13-JMX32"[00Hex]
	{P&Version_PVers_Ascii[0],		{ASCII_FORM, 		0}, {BINARY,	0},		NONE,		8,		4, 		READ_ONLY, 		0, 		FALSE}, //" PROGR.-VERSION " zB:"14:01:14"
  {(char*)(OBJBEZ_ADR),       {EEP_ASCII_FORM, 15}, {BINARY,  0},   NONE,   12,   8,    READ_WRITE,   0,    FALSE}, //" OBJ-BEZEICHNUNG"
  {(char*)(OBJNUM_ADR),       {EEP_ASCII_FORM,  9}, {BINARY,  0},   NONE,   20,   5,    READ_WRITE,   0,    FALSE}, //" OBJ-NUMMER     "
  {(char*)(STRASSE_ADR),      {EEP_ASCII_FORM, 15}, {BINARY,  0},   NONE,   25,   8,    READ_WRITE,   0,    FALSE}, //" STRASSE        "
  {(char*)(NUMMER_ADR),       {EEP_ASCII_FORM,  9}, {BINARY,  0},   NONE,   33,   5,    READ_WRITE,   0,    FALSE}, //" HAUSNUMMER     "
  {(char*)(ORT_ADR),          {EEP_ASCII_FORM, 15}, {BINARY,  0},   NONE,   38,   8,    READ_WRITE,   0,    FALSE}, //" ORT            "
  {(char*)(PLZAHL_ADR),       {EEP_ASCII_FORM,  9}, {BINARY,  0},   NONE,   46,   5,    READ_WRITE,   0,    FALSE}, //" PLZ            "
	{P&Station_Idf, 							{ASCII_FORM,		0},	{BINARY, 	0}, 	NONE, 	51, 	8, 		READ_WRITE,		0, 		FALSE}, //" STATION-IDF    "
	{P&Alarm_Idf[0], 							{ASCII_FORM,		0},	{BINARY, 	0}, 	NONE, 	59, 	8, 		READ_WRITE,		0, 		FALSE}, //" ALARM-IDF 1    "
	{P&Alarm_Idf[1], 							{ASCII_FORM,		0},	{BINARY, 	0}, 	NONE, 	67, 	8, 		READ_WRITE,		0, 		FALSE}, //" ALARM-IDF 2    "
	{P&Alarm_Idf[2], 							{ASCII_FORM,		0},	{BINARY, 	0}, 	NONE, 	75, 	8, 		READ_WRITE,		0, 		FALSE}, //" ALARM-IDF 3    "
                              	            		
	{0, 													{UHR_FORM,  		0}, {BINARY, 	0}, 	NONE,		83, 	1,		READ_WRITE, 	0, 		FALSE},	//" UHRZEIT        "
	{0, 													{DATUM_FORM,		0}, {BINARY, 	0}, 	NONE,		84, 	2,		READ_WRITE, 	0, 		FALSE},	//" DATUM          "
                              	            		
	{P&bst,												{US_INT, 				0},	{U_INT_16,0},		NONE,		86,		1,		READ_ONLY,		0,		FALSE},	//" BETRIEBSSTUNDEN"," h
	{P&ta1m.messw,              	{S_INT,					1},	{S_INT_16,1},		NONE,		87,		1,		READ_ONLY,		0,		FALSE},	//" AUSSENLUFTTEMP."," °C    "(Messwert)
	{P&ta1m.stat,									{US_CHAR,				0},	{U_INT_8,	0},		NONE,		88,		1,		READ_ONLY,		0,		FALSE},	//" AUSSENLUFTTEMP.","       "(Status)
	{P&ta1mh.messw,             	{S_INT,					1},	{S_INT_16,1},   NONE,		89,		1,		READ_ONLY,		0,		FALSE},	//" AUSSENT. MITTEL"," °C    "
	{P&ta1mh.stat,              	{US_CHAR,				0},	{U_INT_8,	0},		NONE,		90,		1,		READ_ONLY,		0,		FALSE},	//" AUSSENT. MITTEL","       "
	{P&Ta1mhzk,                 	{US_CHAR, 			0},	{U_INT_8, 0},   NONE,		91,		1,		READ_WRITE,		0,		FALSE},	//" MITTLUNGS-ZEIT "," h     "
	{P&Tae,												{US_INT, 				1},	{U_INT_16,1},		NONE,		92,		1,		READ_WRITE,		0,		FALSE},	//" Ta-WINTER [EIN]"," °C    "
	{P&Taa,												{US_INT, 				1},	{U_INT_16,1},		NONE,		93,		1,		READ_WRITE,		0,		FALSE},	//" Ta-SOMMER [AUS]"," °C    "
	{P&SomBegin[0],             	{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,		94,		1,		READ_WRITE,		0,		FALSE},	//" SOMMER-BEGINN  "," TT.MM "(Monat,Tag)
	{P&SomEnd[0],            	 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,		95,		1,		READ_WRITE,		0,		FALSE},	//" SOMMER-ENDE    "," TT.MM "(Monat,Tag)
	{P&sowi,			              	{US_CHAR,				0},	{U_INT_8,	0},		NONE,		96,		1,		READ_ONLY,		0,		FALSE},	//" SOMMER ?       ","Ja/Nein"
	{P&HardHand,                	{US_CHAR,				0},	{U_INT_8,	0},   NONE,		97,		1,		READ_ONLY,		0,		FALSE},	//" SCHALTER HAND ?","Ja/Nein" 
                              	
																// KES: KESSEL                  
	{P&TVKE[KE1],									{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  100, 	1, 		READ_ONLY,		0, 		FALSE}, //" VL-KESSEL TVKES"," °C    "(Messwert)
	{P&TVKE[KE1], 			         	{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 101,		1,		READ_ONLY,		0,		FALSE},	//" VL-KESSEL TVKES","       "(Status)
	{P&ked[KE1].ExtAnf1Ke,				{ANA_FORM, 			1},	{S_INT_16,1},		NONE,  102, 	1, 		READ_ONLY,		0, 		FALSE}, //" ANFORDER.EXT 1 "," °C    "(Messwert)
	{P&ked[KE1].ExtAnf1Ke.stat,		{US_CHAR,				0},	{U_INT_8,	0},		NONE,	 103,		1,		READ_ONLY,		0,		FALSE},	//" ANFORDER.EXT 1 ","       "(Status)
	{P&ked[KE1].ExtAnf2Ke,				{ANA_FORM, 			1},	{S_INT_16,1},		NONE,  104, 	1, 		READ_ONLY,		0, 		FALSE}, //" ANFORDER.EXT 2 "," °C    "(Messwert)
	{P&ked[KE1].ExtAnf2Ke.stat,		{US_CHAR,				0},	{U_INT_8,	0},		NONE,	 105,		1,		READ_ONLY,		0,		FALSE},	//" ANFORDER.EXT 2 ","       "(Status)
	{P&kes[KE1].Tvma,							{US_INT, 				1},	{S_INT_16,1},		NONE,  106, 	1, 		READ_WRITE,		0, 		FALSE}, //" VORLAUF   MAX  "," °C    "
	{P&kes[KE1].Tvmi,							{US_INT, 				1},	{S_INT_16,1},		NONE,  107, 	1, 		READ_WRITE,		0, 		FALSE}, //" VORLAUF   MIN  "," °C    "
	{P&ked[KE1].tvsb_kes,					{US_INT,				1},	{S_INT_16,1},		NONE,  108, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORLAUF-SOLLWER"," C    "
	{P&PUKEEA[KE1],		 	 					{JANEIN_FORMOP,	0},	{S_INT_8, 0},		NONE,	 109, 	1, 		READ_ONLY,		0, 		FALSE},	//" KESSEL EIN ?   ","Ja/Nein" 
	{P&hkdSoL[HK1].heizBed,   		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 110,		1,		READ_ONLY,		0,		FALSE},	//" HEIZ-BEDARF ?  ","Ja/Nein" 
	{P&wwd[WW1].twwBed,						{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 111,		1,		READ_ONLY,		0,		FALSE},	//" TWE-BEDARF ?   ","Ja/Nein" 
	{P&ked[KE1].keAl_sAl,					{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 112,		1,		READ_ONLY,		0,		FALSE},	//" SSM KESSEL ?   ","Ja/Nein" 
	{P&kes[KE1].Haut,							{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 113,		1,		READ_ONLY,		0,		FALSE},	//" HANDBETRIEB ?  ","Ja/Nein" 
                            		
																// HK1: HEIZUNG                    		
	{P&TVS[HK1],									{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  200, 	1, 		READ_ONLY,		0, 		FALSE}, //" VL-HEIZUNG  TH1"," C    "(Messwert)
	{P&TVS[HK1],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 201,		1,		READ_ONLY,		0,		FALSE},	//" VL-HEIZUNG  TH1","       "(Status)
	{P&TRS[HK1],									{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  202, 	1, 		READ_ONLY,		0, 		FALSE}, //" RL-HEIZUNG  TH2"," C    "(Messwert)
	{P&TRS[HK1],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 203,		1,		READ_ONLY,		0,		FALSE},	//" RL-HEIZUNG  TH2","       "(Status)
	{P&hkd[KE1].ExtAnf1Hk,				{ANA_FORM, 			1},	{S_INT_16,1},		NONE,  204, 	1, 		READ_ONLY,		0, 		FALSE}, //" ANFORDER.EXT 1 "," °C    "(Messwert)
	{P&hkd[KE1].ExtAnf1Hk.stat,		{US_CHAR,				0},	{U_INT_8,	0},		NONE,	 205,		1,		READ_ONLY,		0,		FALSE},	//" ANFORDER.EXT 1 ","       "(Status)
	{P&hkd[KE1].ExtAnf2Hk,				{ANA_FORM, 			1},	{S_INT_16,1},		NONE,  206, 	1, 		READ_ONLY,		0, 		FALSE}, //" ANFORDER.EXT 2 "," °C    "(Messwert)
	{P&hkd[KE1].ExtAnf2Hk.stat,		{US_CHAR,				0},	{U_INT_8,	0},		NONE,	 207,		1,		READ_ONLY,		0,		FALSE},	//" ANFORDER.EXT 2 ","       "(Status)
	{P&hkd[HK1].tvsb,							{US_INT, 				1},	{S_INT_16,1},		NONE,  208, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORLAUF BERECHN"," C    "
                            		
	{P&hks[HK1].Anst,							{US_INT, 				2},	{S_INT_16,2},		NONE,  210, 	1, 		READ_WRITE,		0, 		FALSE}, //" STEIGUNG       ","       "
	{P&hks[HK1].Tvpa,							{US_INT, 				1},	{S_INT_16,1},		NONE,  211, 	1, 		READ_WRITE,		0, 		FALSE}, //" NIVEAU         "," °C    "
	{P&hks[HK1].Tvma,							{US_INT, 				1},	{S_INT_16,1},		NONE,  212, 	1, 		READ_WRITE,		0, 		FALSE}, //" VORLAUF   MAX  "," °C    "
	{P&hks[HK1].Tvmi,							{US_INT, 				1},	{S_INT_16,1},		NONE,  213, 	1, 		READ_WRITE,		0, 		FALSE}, //" VORLAUF   MIN  "," °C    "
	{P&hks[HK1].Trma,							{US_INT, 				1},	{S_INT_16,1},		NONE,  214, 	1, 		READ_WRITE,		0, 		FALSE}, //" RUECKLAUF MAX  "," °C    "
                            		
	{P&abs_tab[HK1][0].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 220,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN WO"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][0].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 221,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER WO"," h     "
	{P&abs_tab[HK1][0].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 222,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT WO "," K     "
	{P&setwoche[HK1],							{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 223,		1,		READ_WRITE,		0,		FALSE},	//" GANZE WOCHE SET"," 1=JA  "
	{P&abs_tab[HK1][1].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 224,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN MO"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][1].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 225,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER MO"," h     "
	{P&abs_tab[HK1][1].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 226,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT MO "," K     "
	{P&abs_tab[HK1][2].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 227,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN DI"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][2].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 228,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER DI"," h     "
	{P&abs_tab[HK1][2].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 229,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT DI "," K     "
	{P&abs_tab[HK1][3].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 230,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN MI"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][3].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 231,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER MI"," h     "
	{P&abs_tab[HK1][3].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 232,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT MI "," K     "
	{P&abs_tab[HK1][4].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 233,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN DO"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][4].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 234,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER DO"," h     "
	{P&abs_tab[HK1][4].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 235,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT DO "," K     "
	{P&abs_tab[HK1][5].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 236,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN FR"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][5].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 237,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER FR"," h     "
	{P&abs_tab[HK1][5].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 238,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT FR "," K     "
	{P&abs_tab[HK1][6].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 239,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN SA"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][6].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 240,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER SA"," h     "
	{P&abs_tab[HK1][6].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 241,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT SA "," K     "
	{P&abs_tab[HK1][7].begin, 		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 242,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-BEGIN SO"," h:min "(Stunden,Minuten)
	{P&abs_tab[HK1][7].dauer, 		{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 243,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-DAUER SO"," h     "
	{P&abs_tab[HK1][7].abwert,		{US_INT,				0},	{U_INT_16,1},   NONE,	 244,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-WERT SO "," K     "
                            		
	{P&hks[HK1].TaHeizgrenze,			{S_INT, 				1},	{S_INT_16,1},		NONE,  250, 	1, 		READ_WRITE,		0, 		FALSE}, //" HEIZGRENZE  Ta "," C    "
	{P&hks[HK1].Swhk,							{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 251,		1,		READ_WRITE,		0,		FALSE},	//" SO/WI-AUTOMATIK"," JA=1  " 
                          	  	
	{P&hkd[HK1].puEinAnz,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 260,		1,		READ_ONLY,		0,		FALSE},	//" HEIZ-PUMPE EIN?","Ja/Nein" 
	{P&hkd[HK1].somme,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 261,		1,		READ_ONLY,		0,		FALSE},	//" SOMMERBETRIEB ?","Ja/Nein" 
	{P&hkd[HK1].abwAlarm,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 262,		1,		READ_ONLY,		0,		FALSE},	//" SM REGELABW.?  ","Ja/Nein" 
	{P&STWHK[HK1],								{JANEIN_FORMIP,	0},	{U_INT_8,	0},   NONE,	 263,		1,		READ_ONLY,		0,		FALSE},	//" SM STW ?       ","Ja/Nein" 
	{P&hkd[HK1].puSmAnz,					{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 264,		1,		READ_ONLY,		0,		FALSE},	//" HEIZ-PUMPE EIN?","Ja/Nein" 
	{P&hks[KE1].Haut,							{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 265,		1,		READ_ONLY,		0,		FALSE},	//" HANDBETRIEB ?  ","Ja/Nein" 
	{P&hkd[HK1].tsol,							{US_INT, 				1},	{S_INT_16,1},		NONE,  266, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOLLWERT       "," C    "
	{P&RVENT[HK1],								{AOUT_FORMP,		1},	{S_INT_16,1},		NONE,  267, 	1, 		READ_ONLY,		0, 		FALSE}, //" VENTILSTELLUNG "," %     "
                            		
																// HKS: HEIZUNG SOLAR                    		
	{P&hkdSoL[HK1].tvsb,					{US_INT, 				1},	{S_INT_16,1},		NONE,  280, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORLAUF BERECHN"," C    "
	{P&hkdSoL[HK1].hkSolJa,				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 281,		1,		READ_ONLY,		0,		FALSE},	//" SOLARBETRIEB ? ","Ja/Nein" 
	{P&hksSoL[KE1].Haut,					{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 282,		1,		READ_ONLY,		0,		FALSE},	//" HANDBETRIEB ?  ","Ja/Nein" 
	{P&hkdSoL[HK1].tsol,					{US_INT, 				1},	{S_INT_16,1},		NONE,  283, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOLLWERT       "," C    "
	{P&RVENTSO[HK1],							{AOUT_FORMP,		1},	{S_INT_16,1},		NONE,  284, 	1, 		READ_ONLY,		0, 		FALSE}, //" VENTILSTELLUNG "," %     "
                            		
																// TWE: WARMWASSER                    		
	{P&TW1_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  300, 	1, 		READ_ONLY,		0, 		FALSE}, //" REGELTEMP.  TW1"," C    "(Messwert)
	{P&TW1_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 301,		1,		READ_ONLY,		0,		FALSE},	//" REGELTEMP.  TW1","       "(Status)
	{P&TW2_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  302, 	1, 		READ_ONLY,		0, 		FALSE}, //" SPEICHER OB.TW2"," C    "(Messwert)
	{P&TW2_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 303,		1,		READ_ONLY,		0,		FALSE},	//" SPEICHER OB.TW2","       "(Status)
	{P&TW3_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  304, 	1, 		READ_ONLY,		0, 		FALSE}, //" SPEICHER MI.TW3"," C    "(Messwert)
	{P&TW3_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 305,		1,		READ_ONLY,		0,		FALSE},	//" SPEICHER MI.TW3","       "(Status)
	{P&TW4_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  306, 	1, 		READ_ONLY,		0, 		FALSE}, //" SPEICHER UN.TW4"," C    "(Messwert)
	{P&TW4_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 307,		1,		READ_ONLY,		0,		FALSE},	//" SPEICHER UN.TW4","       "(Status)
	{P&TWWZ[WW1],									{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  308, 	1, 		READ_ONLY,		0, 		FALSE}, //" ZIRK-RUECKL TW5"," C    "(Messwert)
	{P&TWWZ[WW1],       					{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 309,		1,		READ_ONLY,		0,		FALSE},	//" ZIRK-RUECKL TW5","       "(Status)
	{P&TWWP[WW1],									{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  310, 	1, 		READ_ONLY,		0, 		FALSE}, //" TWE-RUECKL. TW6"," C    "(Messwert)
	{P&TWWP[WW1],       					{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 311,		1,		READ_ONLY,		0,		FALSE},	//" TWE-RUECKL. TW6","       "(Status)
                            		
	{P&wws[WW1].Twwsol,						{US_INT, 				1},	{U_INT_16,1},		NONE,  320, 	1, 		READ_WRITE,		0, 		FALSE}, //" WW-SOLLTEMP.   "," °C    "
	{P&prs[PR1].profil[0].Zeit,		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 321,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-ZEIT 1  "," h:min "(Stunden,Minuten)
	{P&prs[PR1].profil[0].Dauer,	{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 322,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-DAUER 1 "," h     "
	{P&prs[PR1].profil[0].Twwsol,	{US_INT,				1},	{U_INT_16,1},   NONE,	 323,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-TEMP. 1 "," °C    "
	{P&prs[PR1].profil[1].Zeit,		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 324,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-ZEIT 2  "," h:min "(Stunden,Minuten)
	{P&prs[PR1].profil[1].Dauer,	{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 325,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-DAUER 2 "," h     "
	{P&prs[PR1].profil[1].Twwsol,	{US_INT,				1},	{U_INT_16,1},   NONE,	 326,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-TEMP. 2 "," °C    "
	{P&prs[PR1].profil[2].Zeit,		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 327,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-ZEIT 3  "," h:min "(Stunden,Minuten)
	{P&prs[PR1].profil[2].Dauer,	{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 328,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-DAUER 3 "," h     "
	{P&prs[PR1].profil[2].Twwsol,	{US_INT,				1},	{U_INT_16,1},   NONE,	 329,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-TEMP. 3 "," °C    "
	{P&prs[PR1].profil[3].Zeit,		{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 330,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-ZEIT 4  "," h:min "(Stunden,Minuten)
	{P&prs[PR1].profil[3].Dauer,	{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 331,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-DAUER 4 "," h     "
	{P&prs[PR1].profil[3].Twwsol,	{US_INT,				1},	{U_INT_16,1},   NONE,	 332,		1,		READ_WRITE,		0,		FALSE},	//" PROFIL-TEMP. 4 "," °C    "
	{P&prd[PR1].profEin[0],				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 333,		1,		READ_ONLY,		0,		FALSE},	//" PROFIL 1 EIN ? ","Ja/Nein" 
	{P&prd[PR1].profEin[1],				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 334,		1,		READ_ONLY,		0,		FALSE},	//" PROFIL 2 EIN ? ","Ja/Nein" 
	{P&prd[PR1].profEin[2],				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 335,		1,		READ_ONLY,		0,		FALSE},	//" PROFIL 3 EIN ? ","Ja/Nein" 
	{P&prd[PR1].profEin[3],				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 336,		1,		READ_ONLY,		0,		FALSE},	//" PROFIL 4 EIN ? ","Ja/Nein" 
                              	
	{P&wws[WW1].AbsTag,						{GWOTAG2_FORM,	0},	{U_INT_8, 0},   NONE,	 340,		1,		READ_WRITE,		0,		FALSE},	//" WW-ABSENKTAGE  "," Mo=1.."
	{P&wws[WW1].TwwAbs,						{US_INT,				1},	{U_INT_16,1},   NONE,	 341,		1,		READ_WRITE,		0,		FALSE},	//" ABSENK-SOLLTEMP"," °C    "
	{P&wws[WW1].Puwzab,						{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 342,		1,		READ_WRITE,		0,		FALSE},	//" ZPU-ABSCH.ZEIT "," h:min "(Stunden,Minuten)
	{P&wws[WW1].Puwzdau,					{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 343,		1,		READ_WRITE,		0,		FALSE},	//" ZPU-ABSCH.DAUER"," h     "
                              	
	{P&wws[WW1].Legjn,						{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 350,		1,		READ_WRITE,		0,		FALSE},	//" LEGIONELLENTOD "," JA=1  " 
	{P&wws[WW1].Legtag,						{GWOTAG2_FORM,	0},	{BINARY, 	0},   NONE,	 351,		1,		READ_WRITE,		0,		FALSE},	//" LEGION.-TAGE   "," Mo=1.."
	{P&wws[WW1].Legzeit,					{ZEIT_FORM,			0},	{BINARY, 	0},   NONE,	 352,		1,		READ_WRITE,		0,		FALSE},	//" LEGION.-ZEIT   "," h:min "(Stunden,Minuten)
	{P&wws[WW1].Legdau,						{US_CHAR,				0},	{U_INT_8,	0},   NONE,	 353,		1,		READ_WRITE,		0,		FALSE},	//" LEGION.-DAUER  "," h     "
	{P&wws[WW1].Twwleg,						{US_INT,				1},	{U_INT_16,1},   NONE,	 354,		1,		READ_WRITE,		0,		FALSE},	//" LEGION.-TEMP.  "," °C    "
                              	
	{P&wwd[WW1].wwlad,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 360,		1,		READ_ONLY,		0,		FALSE},	//" LADEBETR. KES ?","Ja/Nein" 
	{P&wwd[WW1].wwLadS,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 361,		1,		READ_ONLY,		0,		FALSE},	//" LADEBETR. SOL ?","Ja/Nein" 
	{P&wwd[WW1].solWwVW,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 362,		1,		READ_ONLY,		0,		FALSE},	//" SOL.VORWAERM.? ","Ja/Nein" 
	{P&wwd[WW1].solWwLdg,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 363,		1,		READ_ONLY,		0,		FALSE},	//" SOLARE LADUNG ?","Ja/Nein" 
	{P&wwd[WW1].solWwZir,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 364,		1,		READ_ONLY,		0,		FALSE},	//" SOLARE ZIRK. ? ","Ja/Nein" 
	{P&wwd[WW1].wwvor,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 365,		1,		READ_ONLY,		0,		FALSE},	//" WW-VORRANG ?   ","Ja/Nein" 
	{P&wwd[WW1].konvLegio,				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 366,		1,		READ_ONLY,		0,		FALSE},	//" KONV.-LEGION. ?","Ja/Nein" 
	{P&wwd[WW1].solarLegio,				{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 367,		1,		READ_ONLY,		0,		FALSE},	//" SOLAR-LEGION. ?","Ja/Nein" 
                              	
	{P&MVWLEA[WW1],								{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 370,		1,		READ_ONLY,		0,		FALSE},	//" LADEVENTIL AUF?","Ja/Nein" 
	{P&PUWZEA[WW1],								{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 371,		1,		READ_ONLY,		0,		FALSE},	//" ZIRK-PUMPE EIN?","Ja/Nein" 
	{P&wwd[WW1].puwzBm,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 372,		1,		READ_ONLY,		0,		FALSE},	//" SM ZIRK-PUMPE ?","Ja/Nein" 
	{P&PUWTEA[WW1],								{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 373,		1,		READ_ONLY,		0,		FALSE},	//" TAUSCH-PU EIN ?","Ja/Nein" 
	{P&wwd[WW1].puwtBm,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 374,		1,		READ_ONLY,		0,		FALSE},	//" SM TAUSCH-PUMP?","Ja/Nein" 
	{P&wwd[WW1].abwAlarm,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 375,		1,		READ_ONLY,		0,		FALSE},	//" SM REGELABW. ? ","Ja/Nein" 
	{P&STWWW[WW1],								{JANEIN_FORMIP,	0},	{U_INT_8,	0},   NONE,	 376,		1,		READ_ONLY,		0,		FALSE},	//" SM STW ?       ","Ja/Nein" 
                              	
	{P&wws[WW1].Haut,							{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 380,		1,		READ_ONLY,		0,		FALSE},	//" HANDBETRIEB ?  ","Ja/Nein" 
	{P&wwd[WW1].tsol,							{US_INT, 				1},	{S_INT_16,1},		NONE,  381, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOLLWERT       "," °C    "
	{P&RVENTWW[WW1],							{AOUT_FORMP,		1},	{S_INT_16,1},		NONE,  382, 	1, 		READ_ONLY,		0, 		FALSE}, //" VENTILSTELLUNG "," %     "
                              	
																// SOL: SOLARSTEUERUNG                      	
	{P&TS1_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  400, 	1, 		READ_ONLY,		0, 		FALSE}, //" KOLLEKTOR   TS1"," C    "(Messwert)
	{P&TS1_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 401,		1,		READ_ONLY,		0,		FALSE},	//" KOLLEKTOR   TS1","       "(Status)
	{P&TS2_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  402, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOLAR-TWE   TS2"," C    "(Messwert)
	{P&TS2_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 403,		1,		READ_ONLY,		0,		FALSE},	//" SOLAR-TWE   TS2","       "(Status)
	{P&TS3_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  404, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOLAR-HEIZ. TS3"," C    "(Messwert)
	{P&TS3_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 405,		1,		READ_ONLY,		0,		FALSE},	//" SOLAR-HEIZ. TS3","       "(Status)
	{P&TS4_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  406, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOLAR-PUFF. TS4"," C    "(Messwert)
	{P&TS4_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 407,		1,		READ_ONLY,		0,		FALSE},	//" SOLAR-PUFF. TS4","       "(Status)
	{P&TS5_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  408, 	1, 		READ_ONLY,		0, 		FALSE}, //" RL-SOLAR    TS5"," C    "(Messwert)
	{P&TS5_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 409,		1,		READ_ONLY,		0,		FALSE},	//" RL-SOLAR    TS5","       "(Status)
	{P&TS6_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  410, 	1, 		READ_ONLY,		0, 		FALSE}, //" PUFFER WARM TS6"," C    "(Messwert)
	{P&TS6_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 411,		1,		READ_ONLY,		0,		FALSE},	//" PUFFER WARM TS6","       "(Status)
	{P&TS7_[0],										{ANA_FORMP,			1},	{S_INT_16,1},		NONE,  412, 	1, 		READ_ONLY,		0, 		FALSE}, //" PUFFER KALT TS7"," C    "(Messwert)
	{P&TS7_[0],       						{ANA_FORMP,	 0x81},	{U_INT_8,	0},		NONE,	 413,		1,		READ_ONLY,		0,		FALSE},	//" PUFFER KALT TS7","       "(Status)
	{P&sod[SO1].kollEin,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 420,		1,		READ_ONLY,		0,		FALSE},	//" KOLLEKTORBETR.?","Ja/Nein" 
	{P&sod[SO1].puffEin,					{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 421,		1,		READ_ONLY,		0,		FALSE},	//" PUFFERRBETRIEB?","Ja/Nein" 
	{P&sod[SO1].pufLad,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 422,		1,		READ_ONLY,		0,		FALSE},	//" PUFFERLADUNG ? ","Ja/Nein" 
	{P&PUSOLEA[SO1],							{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 423,		1,		READ_ONLY,		0,		FALSE},	//" PU-SOLAR EIN ? ","Ja/Nein" 
	{P&sod[SO1].pusoBm,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 424,		1,		READ_ONLY,		0,		FALSE},	//" SM SOLARPUMPE ?","Ja/Nein" 
	{P&PUPUFEA[SO1],							{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 425,		1,		READ_ONLY,		0,		FALSE},	//" PU-PUFFER EIN ?","Ja/Nein" 
	{P&sod[SO1].pupuBm,						{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 426,		1,		READ_ONLY,		0,		FALSE},	//" SM PUFFERPUMPE?","Ja/Nein" 
	{P&MVKOLEA[SO1],							{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 427,		1,		READ_ONLY,		0,		FALSE},	//" UV-KOL PUF.WEG?","Ja/Nein" 
	{P&MVPUFEA[SO1],							{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 428,		1,		READ_ONLY,		0,		FALSE},	//" UV-PUF LADEWEG?","Ja/Nein" 
	{P&UVWWEA[SO1],								{JANEIN_FORMOP,	0},	{U_INT_8,	0},   NONE,	 429,		1,		READ_ONLY,		0,		FALSE},	//" UV-TWE-SOL SOL?","Ja/Nein" 
	{P&speicherMax,								{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 430,		1,		READ_ONLY,		0,		FALSE},	//" SPEICHER MAX ? ","Ja/Nein" 
	{P&pufferMax,									{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 431,		1,		READ_ONLY,		0,		FALSE},	//" PUFFER MAX ?   ","Ja/Nein" 
	{P&NACHSP[SO1],								{JANEIN_FORMIP,	0},	{U_INT_8,	0},   NONE,	 432,		1,		READ_ONLY,		0,		FALSE},	//" SM DRUCK MIN ? ","Ja/Nein" 
	{P&kwzDefekt,									{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 433,		1,		READ_ONLY,		0,		FALSE},	//" KWZ DEFEKT ?   ","Ja/Nein" 
	{P&v2MinAlarm,								{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 434,		1,		READ_ONLY,		0,		FALSE},	//" VOL.STR. ALARM?","Ja/Nein" 
	{P&pusoTempAl,								{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 435,		1,		READ_ONLY,		0,		FALSE},	//" TEMP SOL-PUMPE?","Ja/Nein" 
	{P&sos[SO1].Haut,							{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	 436,		1,		READ_ONLY,		0,		FALSE},	//" HANDBETRIEB   ?","Ja/Nein" 
                              	
																// SOS: SOLARSENSOR                      	
	{P&sod[SO1].SS1_Anz,					{ANA_FORM, 			0},	{S_INT_16,0},		NONE,  500, 	1, 		READ_ONLY,		0, 		FALSE}, //" SOL.-SENSOR SS1"," W/m2  "(Messwert)
	{P&sod[SO1].SS1_Anz.stat,			{US_CHAR,				0},	{U_INT_8,	0},		NONE,	 501,		1,		READ_ONLY,		0,		FALSE},	//" SOL.-SENSOR SS1","       "(Status)
	{P&sod[SO1].SS1_kWh_g,				{US_LONG, 			0},	{U_INT_32,0},		NONE,  502, 	2, 		READ_ONLY,		0, 		FALSE}, //" STRAHLUNG GESAM"," kWh/m2"
	{P&sod[SO1].SS1_kWh_vj,				{US_INT, 				0},	{U_INT_16,0},		NONE,  504, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. VORJAHR"," kWh/m2"

	{P&sod[SO1].SS1_Monat[0],			{US_INT, 				0},	{U_INT_16,0},		NONE,  510, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. JANUAR "," kWh/m2"
	{P&sod[SO1].SS1_Monat[1],			{US_INT, 				0},	{U_INT_16,0},		NONE,  511, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. FEBRUAR"," kWh/m2"
	{P&sod[SO1].SS1_Monat[2],			{US_INT, 				0},	{U_INT_16,0},		NONE,  512, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. MAERZ  "," kWh/m2"
	{P&sod[SO1].SS1_Monat[3],			{US_INT, 				0},	{U_INT_16,0},		NONE,  513, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. APRIL  "," kWh/m2"
	{P&sod[SO1].SS1_Monat[4],			{US_INT, 				0},	{U_INT_16,0},		NONE,  514, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. MAI    "," kWh/m2"
	{P&sod[SO1].SS1_Monat[5],			{US_INT, 				0},	{U_INT_16,0},		NONE,  515, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. JUNI   "," kWh/m2"
	{P&sod[SO1].SS1_Monat[6],			{US_INT, 				0},	{U_INT_16,0},		NONE,  516, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. JULI   "," kWh/m2"
	{P&sod[SO1].SS1_Monat[7],			{US_INT, 				0},	{U_INT_16,0},		NONE,  517, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. AUGUST "," kWh/m2"
	{P&sod[SO1].SS1_Monat[8],			{US_INT, 				0},	{U_INT_16,0},		NONE,  518, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. SEPTEMB"," kWh/m2"
	{P&sod[SO1].SS1_Monat[9],			{US_INT, 				0},	{U_INT_16,0},		NONE,  519, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. OKTOBER"," kWh/m2"
	{P&sod[SO1].SS1_Monat[10],		{US_INT, 				0},	{U_INT_16,0},		NONE,  520, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. NOVEMBE"," kWh/m2"
	{P&sod[SO1].SS1_Monat[11],		{US_INT, 				0},	{U_INT_16,0},		NONE,  521, 	1, 		READ_ONLY,		0, 		FALSE}, //" STRAHL. DEZEMBE"," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[0],	{US_INT, 				0},	{U_INT_16,0},		NONE,  522, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JANUAR "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[1],	{US_INT, 				0},	{U_INT_16,0},		NONE,  523, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR FEBRUAR"," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[2],	{US_INT, 				0},	{U_INT_16,0},		NONE,  524, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAERZ  "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[3],	{US_INT, 				0},	{U_INT_16,0},		NONE,  525, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR APRIL  "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[4],	{US_INT, 				0},	{U_INT_16,0},		NONE,  526, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAI    "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[5],	{US_INT, 				0},	{U_INT_16,0},		NONE,  527, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JUNI   "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[6],	{US_INT, 				0},	{U_INT_16,0},		NONE,  528, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JULI   "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[7],	{US_INT, 				0},	{U_INT_16,0},		NONE,  529, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR AUGUST "," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[8],	{US_INT, 				0},	{U_INT_16,0},		NONE,  530, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR SEPTEMB"," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[9],	{US_INT, 				0},	{U_INT_16,0},		NONE,  531, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR OKTOBER"," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[10],	{US_INT, 				0},	{U_INT_16,0},		NONE,  532, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR NOVEMBE"," kWh/m2"
	{P&sod[SO1].SS1_VJ_Monat[11],	{US_INT, 				0},	{U_INT_16,0},		NONE,  533, 	1, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR DEZEMBE"," kWh/m2"

																// KWZ: VOLUMENZAEHLER KALTWASSER
	{P&ZE[4],											{ZE_FORMP, 			0},	{U_INT_64,0},		NONE,  600, 	4, 		READ_ONLY,		0, 		FALSE}, //" ZAEHLERWERT    "," Liter "
	{P&ZE[4],											{ZE_FORMP, 			5},	{U_INT_64,0},		NONE,  604, 	4, 		READ_ONLY,		0, 		FALSE}, //" AKT.MON.VERBR. "," Liter "
	{P&volstrCtr.flow,		 				{US_LONG,				0},	{U_INT_32,0},		NONE,  608, 	2, 		READ_ONLY,		0, 		FALSE}, //" VOLUMENSTROM   "," l/h  "

	{P&ZE[4],											{ZE_FORMP, 		 10},	{U_INT_64,0},		NONE,  610, 	4, 		READ_ONLY,		0, 		FALSE}, //" JANUAR         "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 11},	{U_INT_64,0},		NONE,  614, 	4, 		READ_ONLY,		0, 		FALSE}, //" FEBRUAR        "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 12},	{U_INT_64,0},		NONE,  618, 	4, 		READ_ONLY,		0, 		FALSE}, //" MAERZ          "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 13},	{U_INT_64,0},		NONE,  622, 	4, 		READ_ONLY,		0, 		FALSE}, //" APRIL          "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 14},	{U_INT_64,0},		NONE,  626, 	4, 		READ_ONLY,		0, 		FALSE}, //" MAI            "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 15},	{U_INT_64,0},		NONE,  630, 	4, 		READ_ONLY,		0, 		FALSE}, //" JUNI           "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 16},	{U_INT_64,0},		NONE,  634, 	4, 		READ_ONLY,		0, 		FALSE}, //" JULI           "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 17},	{U_INT_64,0},		NONE,  638, 	4, 		READ_ONLY,		0, 		FALSE}, //" AUGUST         "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 18},	{U_INT_64,0},		NONE,  642, 	4, 		READ_ONLY,		0, 		FALSE}, //" SEPTEMBER      "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 19},	{U_INT_64,0},		NONE,  646, 	4, 		READ_ONLY,		0, 		FALSE}, //" OKTOBER        "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 20},	{U_INT_64,0},		NONE,  650, 	4, 		READ_ONLY,		0, 		FALSE}, //" NOVEMBER       "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 21},	{U_INT_64,0},		NONE,  654, 	4, 		READ_ONLY,		0, 		FALSE}, //" DEZEMBER       "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 22},	{U_INT_64,0},		NONE,  658, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JANUAR "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 23},	{U_INT_64,0},		NONE,  662, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR FEBRUAR"," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 24},	{U_INT_64,0},		NONE,  666, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAERZ  "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 25},	{U_INT_64,0},		NONE,  670, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR APRIL  "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 26},	{U_INT_64,0},		NONE,  674, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAI    "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 27},	{U_INT_64,0},		NONE,  678, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JUNI   "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 28},	{U_INT_64,0},		NONE,  682, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JULI   "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 29},	{U_INT_64,0},		NONE,  686, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR AUGUST "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 30},	{U_INT_64,0},		NONE,  690, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR SEPT.  "," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 31},	{U_INT_64,0},		NONE,  694, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR OKTOBER"," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 32},	{U_INT_64,0},		NONE,  698, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR NOVEMB."," Liter "
	{P&ZE[4],											{ZE_FORMP, 		 33},	{U_INT_64,0},		NONE,  702, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR DEZEMB."," Liter "

																// WZ1: WAERMEZAEHLER 1 KESSEL-EINTRAG
	{P&wmengCtr[1].wmeng,	 				{US_LONG,				0},	{U_INT_32,0},		NONE,  800, 	2, 		READ_ONLY,		0, 		FALSE}, //" WAERMEMENGE Q1 "," kWh   "
	{P&wmengCtr[1].wmLeistg,			{US_INT,				0},	{U_INT_16,0},		NONE,  802, 	1, 		READ_ONLY,		0, 		FALSE}, //" LEISTUNG    P1 "," kW    "
	{P&wmengCtr[1].flow,	 				{US_LONG,				0},	{U_INT_32,0},		NONE,  803, 	2, 		READ_ONLY,		0, 		FALSE}, //" VOLUMENSTROM   "," l/h   "
	{P&wmengCtr[1].verbr[0],			{US_LONG,				0},	{U_INT_32,0},		NONE,  810, 	2, 		READ_ONLY,		0, 		FALSE}, //" JANUAR         "," kWh   "
	{P&wmengCtr[1].verbr[1],			{US_LONG,				0},	{U_INT_32,0},		NONE,  812, 	2, 		READ_ONLY,		0, 		FALSE}, //" FEBRUAR        "," kWh   "
	{P&wmengCtr[1].verbr[2],			{US_LONG,				0},	{U_INT_32,0},		NONE,  814, 	2, 		READ_ONLY,		0, 		FALSE}, //" MAERZ          "," kWh   "
	{P&wmengCtr[1].verbr[3],			{US_LONG,				0},	{U_INT_32,0},		NONE,  816, 	2, 		READ_ONLY,		0, 		FALSE}, //" APRIL          "," kWh   "
	{P&wmengCtr[1].verbr[4],			{US_LONG,				0},	{U_INT_32,0},		NONE,  818, 	2, 		READ_ONLY,		0, 		FALSE}, //" MAI            "," kWh   "
	{P&wmengCtr[1].verbr[5],			{US_LONG,				0},	{U_INT_32,0},		NONE,  820, 	2, 		READ_ONLY,		0, 		FALSE}, //" JUNI           "," kWh   "
	{P&wmengCtr[1].verbr[6],			{US_LONG,				0},	{U_INT_32,0},		NONE,  822, 	2, 		READ_ONLY,		0, 		FALSE}, //" JULI           "," kWh   "
	{P&wmengCtr[1].verbr[7],			{US_LONG,				0},	{U_INT_32,0},		NONE,  824, 	2, 		READ_ONLY,		0, 		FALSE}, //" AUGUST         "," kWh   "
	{P&wmengCtr[1].verbr[8],			{US_LONG,				0},	{U_INT_32,0},		NONE,  826, 	2, 		READ_ONLY,		0, 		FALSE}, //" SEPTEMBER      "," kWh   "
	{P&wmengCtr[1].verbr[9],			{US_LONG,				0},	{U_INT_32,0},		NONE,  828, 	2, 		READ_ONLY,		0, 		FALSE}, //" OKTOBER        "," kWh   "
	{P&wmengCtr[1].verbr[10],			{US_LONG,				0},	{U_INT_32,0},		NONE,  830, 	2, 		READ_ONLY,		0, 		FALSE}, //" NOVEMBER       "," kWh   "
	{P&wmengCtr[1].verbr[11],			{US_LONG,				0},	{U_INT_32,0},		NONE,  832, 	2, 		READ_ONLY,		0, 		FALSE}, //" DEZEMBER       "," kWh   "
	{P&wmengCtr[1].verbr_vj[0],		{US_LONG,				0},	{U_INT_32,0},		NONE,  834, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JANUAR "," kWh   "
	{P&wmengCtr[1].verbr_vj[1],		{US_LONG,				0},	{U_INT_32,0},		NONE,  836, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR FEBRUAR"," kWh   "
	{P&wmengCtr[1].verbr_vj[2],		{US_LONG,				0},	{U_INT_32,0},		NONE,  838, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAERZ  "," kWh   "
	{P&wmengCtr[1].verbr_vj[3],		{US_LONG,				0},	{U_INT_32,0},		NONE,  840, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR APRIL  "," kWh   "
	{P&wmengCtr[1].verbr_vj[4],		{US_LONG,				0},	{U_INT_32,0},		NONE,  842, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAI    "," kWh   "
	{P&wmengCtr[1].verbr_vj[5],		{US_LONG,				0},	{U_INT_32,0},		NONE,  844, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JUNI   "," kWh   "
	{P&wmengCtr[1].verbr_vj[6],		{US_LONG,				0},	{U_INT_32,0},		NONE,  846, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JULI   "," kWh   "
	{P&wmengCtr[1].verbr_vj[7],		{US_LONG,				0},	{U_INT_32,0},		NONE,  848, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR AUGUST "," kWh   "
	{P&wmengCtr[1].verbr_vj[8],		{US_LONG,				0},	{U_INT_32,0},		NONE,  850, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR SEPTEMB"," kWh   "
	{P&wmengCtr[1].verbr_vj[9],		{US_LONG,				0},	{U_INT_32,0},		NONE,  852, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR OKTOBER"," kWh   "
	{P&wmengCtr[1].verbr_vj[10],	{US_LONG,				0},	{U_INT_32,0},		NONE,  854, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR NOVEMBE"," kWh   "
	{P&wmengCtr[1].verbr_vj[11],	{US_LONG,				0},	{U_INT_32,0},		NONE,  856, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR DEZEMBE"," kWh   "

																// WZ2: WAERMEZAEHLER 2 SOLAR-EINTRAG                    
	{P&wmengCtr[2].wmeng,	 				{US_LONG,				0},	{U_INT_32,0},		NONE,	 900, 	2, 		READ_ONLY,		0, 		FALSE}, //" WAERMEMENGE Q2 "," kWh   "
	{P&wmengCtr[2].wmLeistg,			{US_INT,				0},	{U_INT_16,0},		NONE,	 902, 	1, 		READ_ONLY,		0, 		FALSE}, //" LEISTUNG    P2 "," kW    "
	{P&wmengCtr[2].flow,	 				{US_LONG,				0},	{U_INT_32,0},		NONE,	 903, 	2, 		READ_ONLY,		0, 		FALSE}, //" VOLUMENSTROM   "," l/h   "

	{P&wmengCtr[2].verbr[0],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 910, 	2, 		READ_ONLY,		0, 		FALSE}, //" JANUAR         "," kWh   "
	{P&wmengCtr[2].verbr[1],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 912, 	2, 		READ_ONLY,		0, 		FALSE}, //" FEBRUAR        "," kWh   "
	{P&wmengCtr[2].verbr[2],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 914, 	2, 		READ_ONLY,		0, 		FALSE}, //" MAERZ          "," kWh   "
	{P&wmengCtr[2].verbr[3],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 916, 	2, 		READ_ONLY,		0, 		FALSE}, //" APRIL          "," kWh   "
	{P&wmengCtr[2].verbr[4],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 918, 	2, 		READ_ONLY,		0, 		FALSE}, //" MAI            "," kWh   "
	{P&wmengCtr[2].verbr[5],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 920, 	2, 		READ_ONLY,		0, 		FALSE}, //" JUNI           "," kWh   "
	{P&wmengCtr[2].verbr[6],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 922, 	2, 		READ_ONLY,		0, 		FALSE}, //" JULI           "," kWh   "
	{P&wmengCtr[2].verbr[7],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 924, 	2, 		READ_ONLY,		0, 		FALSE}, //" AUGUST         "," kWh   "
	{P&wmengCtr[2].verbr[8],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 926, 	2, 		READ_ONLY,		0, 		FALSE}, //" SEPTEMBER      "," kWh   "
	{P&wmengCtr[2].verbr[9],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 928, 	2, 		READ_ONLY,		0, 		FALSE}, //" OKTOBER        "," kWh   "
	{P&wmengCtr[2].verbr[10],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 930, 	2, 		READ_ONLY,		0, 		FALSE}, //" NOVEMBER       "," kWh   "
	{P&wmengCtr[2].verbr[11],			{US_LONG,				0},	{U_INT_32,0},		NONE,	 932, 	2, 		READ_ONLY,		0, 		FALSE}, //" DEZEMBER       "," kWh   "
	{P&wmengCtr[2].verbr_vj[0],		{US_LONG,				0},	{U_INT_32,0},		NONE,  934, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JANUAR "," kWh   "
	{P&wmengCtr[2].verbr_vj[1],		{US_LONG,				0},	{U_INT_32,0},		NONE,  936, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR FEBRUAR"," kWh   "
	{P&wmengCtr[2].verbr_vj[2],		{US_LONG,				0},	{U_INT_32,0},		NONE,  938, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAERZ  "," kWh   "
	{P&wmengCtr[2].verbr_vj[3],		{US_LONG,				0},	{U_INT_32,0},		NONE,  940, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR APRIL  "," kWh   "
	{P&wmengCtr[2].verbr_vj[4],		{US_LONG,				0},	{U_INT_32,0},		NONE,  942, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAI    "," kWh   "
	{P&wmengCtr[2].verbr_vj[5],		{US_LONG,				0},	{U_INT_32,0},		NONE,  944, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JUNI   "," kWh   "
	{P&wmengCtr[2].verbr_vj[6],		{US_LONG,				0},	{U_INT_32,0},		NONE,  946, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JULI   "," kWh   "
	{P&wmengCtr[2].verbr_vj[7],		{US_LONG,				0},	{U_INT_32,0},		NONE,  948, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR AUGUST "," kWh   "
	{P&wmengCtr[2].verbr_vj[8],		{US_LONG,				0},	{U_INT_32,0},		NONE,  950, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR SEPTEMB"," kWh   "
	{P&wmengCtr[2].verbr_vj[9],		{US_LONG,				0},	{U_INT_32,0},		NONE,  952, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR OKTOBER"," kWh   "
	{P&wmengCtr[2].verbr_vj[10],	{US_LONG,				0},	{U_INT_32,0},		NONE,  954, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR NOVEMBE"," kWh   "
	{P&wmengCtr[2].verbr_vj[11],	{US_LONG,				0},	{U_INT_32,0},		NONE,  956, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR DEZEMBE"," kWh   "

																// WZ3: WAERMEZAEHLER 3 NAHWAERMENETZ
	{P&wmengCtr[3].wmeng,	 				{US_LONG,				0},	{U_INT_32,0},		NONE, 1000, 	2, 		READ_ONLY,		0, 		FALSE}, //" WAERMEMENGE Q3 "," kWh   "
	{P&wmengCtr[3].wmLeistg,			{US_INT,				0},	{U_INT_16,0},		NONE, 1002, 	1, 		READ_ONLY,		0, 		FALSE}, //" LEISTUNG    P3 "," kW    "
	{P&wmengCtr[3].flow,	 				{US_LONG,				0},	{U_INT_32,0},		NONE, 1003, 	2, 		READ_ONLY,		0, 		FALSE}, //" VOLUMENSTROM   "," l/h   "

	{P&wmengCtr[3].verbr[0],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1010, 	2, 		READ_ONLY,		0, 		FALSE}, //" JANUAR         "," kWh   "
	{P&wmengCtr[3].verbr[1],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1012, 	2, 		READ_ONLY,		0, 		FALSE}, //" FEBRUAR        "," kWh   "
	{P&wmengCtr[3].verbr[2],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1014, 	2, 		READ_ONLY,		0, 		FALSE}, //" MAERZ          "," kWh   "
	{P&wmengCtr[3].verbr[3],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1016, 	2, 		READ_ONLY,		0, 		FALSE}, //" APRIL          "," kWh   "
	{P&wmengCtr[3].verbr[4],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1018, 	2, 		READ_ONLY,		0, 		FALSE}, //" MAI            "," kWh   "
	{P&wmengCtr[3].verbr[5],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1020, 	2, 		READ_ONLY,		0, 		FALSE}, //" JUNI           "," kWh   "
	{P&wmengCtr[3].verbr[6],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1022, 	2, 		READ_ONLY,		0, 		FALSE}, //" JULI           "," kWh   "
	{P&wmengCtr[3].verbr[7],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1024, 	2, 		READ_ONLY,		0, 		FALSE}, //" AUGUST         "," kWh   "
	{P&wmengCtr[3].verbr[8],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1026, 	2, 		READ_ONLY,		0, 		FALSE}, //" SEPTEMBER      "," kWh   "
	{P&wmengCtr[3].verbr[9],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1028, 	2, 		READ_ONLY,		0, 		FALSE}, //" OKTOBER        "," kWh   "
	{P&wmengCtr[3].verbr[10],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1030, 	2, 		READ_ONLY,		0, 		FALSE}, //" NOVEMBER       "," kWh   "
	{P&wmengCtr[3].verbr[11],			{US_LONG,				0},	{U_INT_32,0},		NONE, 1032, 	2, 		READ_ONLY,		0, 		FALSE}, //" DEZEMBER       "," kWh   "
	{P&wmengCtr[3].verbr_vj[0],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1034, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JANUAR "," kWh   "
	{P&wmengCtr[3].verbr_vj[1],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1036, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR FEBRUAR"," kWh   "
	{P&wmengCtr[3].verbr_vj[2],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1038, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAERZ  "," kWh   "
	{P&wmengCtr[3].verbr_vj[3],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1040, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR APRIL  "," kWh   "
	{P&wmengCtr[3].verbr_vj[4],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1042, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAI    "," kWh   "
	{P&wmengCtr[3].verbr_vj[5],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1044, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JUNI   "," kWh   "
	{P&wmengCtr[3].verbr_vj[6],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1046, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JULI   "," kWh   "
	{P&wmengCtr[3].verbr_vj[7],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1048, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR AUGUST "," kWh   "
	{P&wmengCtr[3].verbr_vj[8],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1050, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR SEPTEMB"," kWh   "
	{P&wmengCtr[3].verbr_vj[9],		{US_LONG,				0},	{U_INT_32,0},		NONE, 1052, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR OKTOBER"," kWh   "
	{P&wmengCtr[3].verbr_vj[10],	{US_LONG,				0},	{U_INT_32,0},		NONE, 1054, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR NOVEMBE"," kWh   "
	{P&wmengCtr[3].verbr_vj[11],	{US_LONG,				0},	{U_INT_32,0},		NONE, 1056, 	2, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR DEZEMBE"," kWh   "

																// ZAE: EXTERNER ZÄHLER  GAS	
	{P&X2ZE[0],										{ZE_FORMP, 			0},	{U_INT_64,0},		NONE, 1100, 	4, 		READ_ONLY,		0, 		FALSE}, //" ZAEHLERWERT    "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 			5},	{U_INT_64,0},		NONE, 1104, 	4, 		READ_ONLY,		0, 		FALSE}, //" AKT.MON.VERBR. "," Liter "

	{P&X2ZE[0],										{ZE_FORMP, 		 10},	{U_INT_64,0},		NONE, 1110, 	4, 		READ_ONLY,		0, 		FALSE}, //" JANUAR         "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 11},	{U_INT_64,0},		NONE, 1114, 	4, 		READ_ONLY,		0, 		FALSE}, //" FEBRUAR        "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 12},	{U_INT_64,0},		NONE, 1118, 	4, 		READ_ONLY,		0, 		FALSE}, //" MAERZ          "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 13},	{U_INT_64,0},		NONE, 1122, 	4, 		READ_ONLY,		0, 		FALSE}, //" APRIL          "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 14},	{U_INT_64,0},		NONE, 1126, 	4, 		READ_ONLY,		0, 		FALSE}, //" MAI            "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 15},	{U_INT_64,0},		NONE, 1130, 	4, 		READ_ONLY,		0, 		FALSE}, //" JUNI           "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 16},	{U_INT_64,0},		NONE, 1134, 	4, 		READ_ONLY,		0, 		FALSE}, //" JULI           "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 17},	{U_INT_64,0},		NONE, 1138, 	4, 		READ_ONLY,		0, 		FALSE}, //" AUGUST         "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 18},	{U_INT_64,0},		NONE, 1142, 	4, 		READ_ONLY,		0, 		FALSE}, //" SEPTEMBER      "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 19},	{U_INT_64,0},		NONE, 1146, 	4, 		READ_ONLY,		0, 		FALSE}, //" OKTOBER        "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 20},	{U_INT_64,0},		NONE, 1150, 	4, 		READ_ONLY,		0, 		FALSE}, //" NOVEMBER       "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 21},	{U_INT_64,0},		NONE, 1154, 	4, 		READ_ONLY,		0, 		FALSE}, //" DEZEMBER       "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 22},	{U_INT_64,0},		NONE, 1158, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JANUAR "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 23},	{U_INT_64,0},		NONE, 1162, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR FEBRUAR"," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 24},	{U_INT_64,0},		NONE, 1166, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAERZ  "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 25},	{U_INT_64,0},		NONE, 1170, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR APRIL  "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 26},	{U_INT_64,0},		NONE, 1174, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR MAI    "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 27},	{U_INT_64,0},		NONE, 1178, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JUNI   "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 28},	{U_INT_64,0},		NONE, 1182, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR JULI   "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 29},	{U_INT_64,0},		NONE, 1186, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR AUGUST "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 30},	{U_INT_64,0},		NONE, 1190, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR SEPT.  "," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 31},	{U_INT_64,0},		NONE, 1194, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR OKTOBER"," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 32},	{U_INT_64,0},		NONE, 1198, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR NOVEMB."," Liter "
	{P&X2ZE[0],										{ZE_FORMP, 		 33},	{U_INT_64,0},		NONE, 1202, 	4, 		READ_ONLY,		0, 		FALSE}, //" VORJAHR DEZEMB."," Liter "

																// QSM: QUITTIERBARE STÖRMELDUNGEN
	{P&quit_taste,								{US_CHAR,				0},	{U_INT_8,	0},   NONE,	1300,		1,		READ_WRITE,		0,		FALSE},	//" QUITTIER-TASTE ","EIN=1  " 
	{P&un_qsm,										{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1301,		1,		READ_ONLY,		0,		FALSE},	//" QSM VORHANDEN ?","Ja/Nein" 
	{P&alarmtab[4].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1302,		1,		READ_ONLY,		0,		FALSE},	//"ANL: SSM EXT.1 HKV ?","Ja/Nein" 
	{P&alarmtab[5].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1303,		1,		READ_ONLY,		0,		FALSE},	//"ANL: SSM EXT.2 WPU ?","Ja/Nein" 
	{P&alarmtab[6].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1304,		1,		READ_ONLY,		0,		FALSE},	//"ANL: SSM EXT.3 KES ?","Ja/Nein" 
	{P&alarmtab[7].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1305,		1,		READ_ONLY,		0,		FALSE},	//"ANL: EXTERNE SSM ?  ","Ja/Nein" 
	{P&alarmtab[11].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1306,		1,		READ_ONLY,		0,		FALSE},	//"KES: SSM KESSEL ?   ","Ja/Nein" 
	{P&alarmtab[17].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1307,		1,		READ_ONLY,		0,		FALSE},	//"HK1: SM REGELABW.?  ","Ja/Nein" 
	{P&alarmtab[18].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1308,		1,		READ_ONLY,		0,		FALSE},	//"HK1: SM STW ?       ","Ja/Nein" 
	{P&alarmtab[19].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1309,		1,		READ_ONLY,		0,		FALSE},	//"HK1: SM HEIZ-PUMPE ?","Ja/Nein" 
	{P&alarmtab[28].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1310,		1,		READ_ONLY,		0,		FALSE},	//"TWE: SM REGELABW. ? ","Ja/Nein" 
	{P&alarmtab[30].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1311,		1,		READ_ONLY,		0,		FALSE},	//"TWE: SM ZIRK-PUMPE ?","Ja/Nein" 
	{P&alarmtab[31].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1312,		1,		READ_ONLY,		0,		FALSE},	//"TWE: SM TAUSCH-PUMP?","Ja/Nein" 
	{P&alarmtab[40].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1313,		1,		READ_ONLY,		0,		FALSE},	//"SOL: SM DRUCK MIN ? ","Ja/Nein" 
	{P&alarmtab[41].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1314,		1,		READ_ONLY,		0,		FALSE},	//"SOL: KWZ DEFEKT ?   ","Ja/Nein" 
	{P&alarmtab[42].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1315,		1,		READ_ONLY,		0,		FALSE},	//"SOL: VOL.STR. ALARM?","Ja/Nein" 
	{P&alarmtab[44].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1316,		1,		READ_ONLY,		0,		FALSE},	//"SOL: SM SOLARPUMPE ?","Ja/Nein" 
	{P&alarmtab[45].alspeicher,		{JANEIN_FORM,		0},	{U_INT_8,	0},   NONE,	1317,		1,		READ_ONLY,		0,		FALSE},	//"SOL: SM PUFFERPUMPE?","Ja/Nein" 

};
const unsigned int modbusSlaveTableSize 	= (sizeof(modbusSlaveTable)/sizeof(modbusSlaveTableRow));
modbusTableRowData modbusSlaveTableData[(sizeof(modbusSlaveTable)/sizeof(modbusSlaveTableRow))];

#else	// kein Slave definiert

const modbusSlaveTableRow modbusSlaveTable[] = {
};
const unsigned int modbusSlaveTableSize 	= 0;
modbusTableRowData modbusSlaveTableData[1];
#endif


//-----------------------------------
// Geräte
//-----------------------------------
const modbusDevice modbusDevices[] = {
//{"Name           ",	Adresse,	Schnittstelle}
	{"ICONAG ZENTRALE", 1, 				S2},						// Modbus Device idx 0
//{"UK24MOD-dummy  ", 2, 				S3},						// Modbus Device idx 1
};
const unsigned int modbusDeviceCount = (sizeof(modbusDevices)/sizeof(modbusDevice)); 	

//-----------------------------------
// Schnittstellen
//-----------------------------------
// Parität
//-----------
// 0= keine Parität, 1= Odd parity, 2= Even parity
const char modbusSioParity_Standard[] = {
	0,		// S1
	0,		// S2
	0,		// S3
};	

// stop bits
//-----------------------------------
// 1= ein stop bit, 2= zwei stop bits
const char modbusSioStopBits_Standard[] = {
	1,		// S1
	1,		// S2
	1,		// S3
};	

unsigned char modbusDeviceAddresses[MODBUS_DEVICE_COUNT];

#endif