#include "defins.h"
#include "struct.h"
#include "projdef.h"
#include "uramext.h"
#include "ustruct.h"
#include "userdef.h"
#include "uconstext.h"
#include "konvform.h"
#include "modbus.h"
#include "belimo.h"		// erforderlich: nur für Belimo Gateway MP zu Modbus RTU


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
// {Adresse des                {Riedel Typ,      {Modbus Typ,     Spezial-   Modbus  Register-    Register-   Operation,          Intervall,AutoSend}
//  Datenpunkts                 Komma      },     Komma      },   Konvert.,  Device  adresse,     anzahl,                         [sek]
//                                                                           idx
    // Zuluft 1
    {P&LQ_CO2[VE1],            {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP1+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // CO2             von MP1
    {P&POSBOXZ[VE1],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP1+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Zul.   von MP1
    {P&vend[VE1].stat_zul,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP1+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Zul.   von MP1
    {P&vend[VE1].sernum_zul[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP1+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Zul.von MP1
    {P&VBOXZU[VE1],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP1+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Zul. an MP1
    {P&vend[VE1].zwang_zul,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP1+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Zul. an MP1
    {P&vens[VE1].Sens_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP1+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Zul. an MP1
    {P&vens[VE1].Antr_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP1+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Zul. an MP1
		// Abluft 1
    {P&AE_TI[VE1],             {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP2+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Raumtemp.       von MP2
    {P&POSBOXA[VE1],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP2+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Abl.   von MP2
    {P&vend[VE1].stat_abl,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP2+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Abl.   von MP2
    {P&vend[VE1].sernum_abl[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP2+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Abl.von MP2
    {P&VBOXAB[VE1],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP2+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Abl. an MP2
    {P&vend[VE1].zwang_abl,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP2+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Abl. an MP2
    {P&vens[VE1].Sens_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP2+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Abl. an MP2
    {P&vens[VE1].Antr_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP2+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Abl. an MP2

    // Zuluft 2
    {P&LQ_CO2[VE2],            {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP3+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // CO2             von MP3
    {P&POSBOXZ[VE2],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP3+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Zul.   von MP3
    {P&vend[VE2].stat_zul,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP3+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Zul.   von MP3
    {P&vend[VE2].sernum_zul[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP3+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Zul.von MP3
    {P&VBOXZU[VE2],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP3+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Zul. an MP3
    {P&vend[VE2].zwang_zul,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP3+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Zul. an MP3
    {P&vens[VE2].Sens_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP3+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Zul. an MP3
    {P&vens[VE2].Antr_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP3+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Zul. an MP3
		// Abluft 2
    {P&AE_TI[VE2],             {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP4+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Raumtemp.       von MP4
    {P&POSBOXA[VE2],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP4+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Abl.   von MP4
    {P&vend[VE2].stat_abl,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP4+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Abl.   von MP4
    {P&vend[VE2].sernum_abl[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP4+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Abl.von MP4
    {P&VBOXAB[VE2],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP4+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Abl. an MP4
    {P&vend[VE2].zwang_abl,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP4+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Abl. an MP4
    {P&vens[VE2].Sens_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP4+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Abl. an MP4
    {P&vens[VE2].Antr_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP4+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Abl. an MP4

    // Zuluft 3
    {P&LQ_CO2[VE3],            {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP5+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // CO2             von MP5
    {P&POSBOXZ[VE3],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP5+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Zul.   von MP5
    {P&vend[VE3].stat_zul,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP5+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Zul.   von MP5
    {P&vend[VE3].sernum_zul[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP5+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Zul.von MP5
    {P&VBOXZU[VE3],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP5+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Zul. an MP5
    {P&vend[VE3].zwang_zul,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP5+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Zul. an MP5
    {P&vens[VE3].Sens_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP5+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Zul. an MP5
    {P&vens[VE3].Antr_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP5+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Zul. an MP5
		// Abluft 3
    {P&AE_TI[VE3],             {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP6+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Raumtemp.       von MP6
    {P&POSBOXA[VE3],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP6+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Abl.   von MP6
    {P&vend[VE3].stat_abl,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP6+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Abl.   von MP6
    {P&vend[VE3].sernum_abl[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP6+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Abl.von MP6
    {P&VBOXAB[VE3],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP6+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Abl. an MP6
    {P&vend[VE3].zwang_abl,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP6+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Abl. an MP6
    {P&vens[VE3].Sens_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP6+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Abl. an MP6
    {P&vens[VE3].Antr_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP6+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Abl. an MP6

    // Zuluft 4
    {P&LQ_CO2[VE4],            {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP7+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // CO2             von MP7
    {P&POSBOXZ[VE4],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP7+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Zul.   von MP7
    {P&vend[VE4].stat_zul,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP7+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Zul.   von MP7
    {P&vend[VE4].sernum_zul[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP7+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Zul.von MP7
    {P&VBOXZU[VE4],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP7+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Zul. an MP7
    {P&vend[VE4].zwang_zul,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP7+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Zul. an MP7
    {P&vens[VE4].Sens_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP7+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Zul. an MP7
    {P&vens[VE4].Antr_typ_zul, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP7+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Zul. an MP7
		// Abluft 4
    {P&AE_TI[VE4],             {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP8+7,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Raumtemp.       von MP8
    {P&POSBOXA[VE4],           {ANA_FORMP, 1},   {S_INT_16, 2},   NONE,      0,      MP8+3,       1,          READ_HOLDING_REGS,  1,        FALSE},  // Position Abl.   von MP8
    {P&vend[VE4].stat_abl,     {US_INT,    0},   {U_INT_16, 0},   NONE,      0,      MP8+5,       1,          READ_HOLDING_REGS,  1,        FALSE},  // MPStatus Abl.   von MP8
    {P&vend[VE4].sernum_abl[0],{BINARY,    0},   {BINARY,   0},   NONE,      0,      MP8+8,       3,          READ_HOLDING_REGS,  60,       FALSE},  // Serial-Num. Abl.von MP8
    {P&VBOXAB[VE4],            {AOUT_FORMP,1},   {U_INT_16, 2},   NONE,      0,      MP8+0,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Sollwert    Abl. an MP8
    {P&vend[VE4].zwang_abl,    {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP8+1,       1,       WRITE_SINGLE_HOLDING,  1,        FALSE},  // Zwangsteu   Abl. an MP8
    {P&vens[VE4].Sens_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP8+6,       1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Sensortyp   Abl. an MP8
    {P&vens[VE4].Antr_typ_abl, {US_CHAR,   0},   {U_INT_16, 0},   NONE,      0,      MP8+11,      1,       WRITE_SINGLE_HOLDING,  60,       FALSE},  // Antriebstyp Abl. an MP8


};
const unsigned int modbusMasterTableSize = (sizeof(modbusTable)/sizeof(modbusTableRow));
modbusTableRowData modbusTableData[(sizeof(modbusTable)/sizeof(modbusTableRow))];

#else	// kein Master definiert

const modbusTableRow modbusTable[] = {
};
const unsigned int modbusMasterTableSize = 0;
modbusTableRowData modbusTableData[1];
#endif

//-----------------------------------
// Slave
//-----------------------------------
#if MODBUS_SLAVE == 1

const modbusSlaveTableRow modbusSlaveTable[] = {
//{Adresse des Datenpunkts,	{Riedel Typ,Komma},	{Modbus Typ,Komma},	Konvertierung,	Registeradresse,	Registeranzahl, Berechtigung, Timeout[m], Fallback}
	{P&Kategorie_Vers[0], 		{US_CHAR, 0}, 			{U_INT_8, 0}, 			NONE,						0,								1, 							READ_ONLY, 		0, 					FALSE}, // Programmversion Jahr
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
	{"UK24MOD        ", 1, 				SX},						// Modbus Device idx 0
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