#include "defins.h"
#include "struct.h"
#include "projdef.h"
#include "uramext.h"
#include "ustruct.h"
#include "userdef.h"
#include "uconstext.h"
#include "konvform.h"
#include "modbus.h"

#if MODBUS_UNI == 1

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

// Tabelle für Lüftungsgerät Wolf CKL3000
// Modbus-Schnittstelle für WRS-K (Original) : Modbus Schnittstelle 3063000_201404.pdf

#define OFFS 1		// Tabellen-Index - OFFS = Registeradresse

const modbusTableRow modbusTable[] = {
// {Adresse des           {Riedel Typ,      {Modbus Typ,     Spezial-   Modbus-     Register-    Register-   Operation,           Intervall AutoSend}
//  Datenpunkts            Komma      },     Komma      },   Konvert.,  Device idx  adresse,     anzahl,                          [sek],

   {P&ul_dig32[0],        {NONE , 0},       {COILS, 0},      NONE,      0,            0,          32,          READ_MULTIPLE_COILS, 10,       TRUE},	// Extraktion auf Einzelbytes im Task ModbusExtract
   {P&ul_dig32[1],        {NONE , 0},       {COILS, 0},      NONE,      0,           32,          32,          READ_MULTIPLE_COILS, 10,       TRUE},	// 
   {P&ul_dig32[2],        {NONE , 0},       {COILS, 0},      NONE,      0,           64,          32,          READ_MULTIPLE_COILS, 10,       TRUE},	// 
   {P&ul_dig32[3],        {NONE , 0},       {COILS, 0},      NONE,      0,           96,          32,          READ_MULTIPLE_COILS, 10,       TRUE},	// 

   {P&mwsp_ana[0],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            1-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Zulufttemp.            		0,1 °C
   {P&mwsp_ana[1],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            2-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Außenlufttemp.         		0,1 °C
   {P&mwsp_ana[2],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            3-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Raumtemp.              		0,1 °C
   {P&mwsp_ana[3],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            4-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Ablufttemp.            		0,1 °C
   {P&mwsp_ana[4],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            5-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Luftqualität           		0,1  V
   {P&mwsp_ana[5],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            6-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Raumsollwertgeber      		0,1 °C
   {P&mwsp_ana[6],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            7-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Raumfeuchte            		0,1 %r.H.
   {P&mwsp_ana[7],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            8-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Abluftfeuchte          		0,1 %r.H.
   {P&mwsp_ana[8],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,            9-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Zuluftfeuchte          		0,1 %r.H.
   {P&mwsp_ana[9],        {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           10-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Zulufttemp.  		0,1 °C
   {P&mwsp_ana[10],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           11-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Temperatur   		0,1 °C
   {P&mwsp_ana[11],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,           12-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Frischluftanteil   1 %
   {P&mwsp_ana[12],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           13-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Drehz.Zuluft     0,1 %
   {P&mwsp_ana[13],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           14-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Drehz.Abluft     0,1 %
   {P&mwsp_ana[14],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           23-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.relative Feuchte 0,1 %r.H.
   {P&mwsp_ana[15],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           24-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.absolute Feuchte 0,1 g/kg
   {P&mwsp_ana[16],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           27-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Vereisungsfühler           0,1 °C
   {P&mwsp_ana[17],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           28-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stellsignal Heizen         0,1 %
   {P&mwsp_ana[18],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           29-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stellsignal Kühlen         0,1 %
   {P&mwsp_ana[19],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           30-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stellsignal WRG            0,1 %
   {P&mwsp_ana[20],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           31-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stellsignal Befeuchter     0,1 %
   {P&mwsp_ana[21],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           32-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Ablufttemp.nach Befeuchter 0,1 °C
   {P&mwsp_ana[22],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,           33-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stellsignal Nachheizen     0,1 %
   {P&mwsp_ana[23],       {ANA_FORM, 1},    {S_INT_16,1},    NONE,      0,          209-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Luftqualität CO2           0,1 ppm
   {P&mwsp_ana[24],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          210-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Druck Zuluft                 1 Pa
   {P&mwsp_ana[25],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          211-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Druck Abluft                 1 Pa
   {P&mwsp_ana[26],       {ANA_FORM, 1},    {S_INT_16,0},    NONE,      0,          212-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Volumenstrom Zuluft         10 m³/h
   {P&mwsp_ana[27],       {ANA_FORM, 1},    {S_INT_16,0},    NONE,      0,          213-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Volumenstrom Abluft         10 m³/h
   {P&mwsp_ana[28],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          214-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Betriebsart                  - -
   {P&mwsp_ana[29],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          215-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Ventilatorstufe    - -
   {P&mwsp_ana[30],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          216-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Druck Zuluft       1 Pa
   {P&mwsp_ana[31],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          217-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Druck Abluft       1 Pa
   {P&mwsp_ana[32],       {ANA_FORM, 1},    {S_INT_16,0},    NONE,      0,          218-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Vol.strom Zuluft  10 m³/h
   {P&mwsp_ana[33],       {ANA_FORM, 1},    {S_INT_16,0},    NONE,      0,          219-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// akt.Sollw.Vol.strom Abluft  10 m³/h
   {P&mwsp_ana[34],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          220-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Anford.Kälteerzeuger Stufe   - -
   {P&mwsp_ana[35],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          232-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stufe Elektroheizregister    - -
   {P&mwsp_ana[36],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          233-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Stufe Direktverdampfer       - -
   {P&mwsp_ana[37],       {ANA_FORM, 0},    {S_INT_16,0},    NONE,      0,          255-OFFS,      1,          READ_HOLDING_REGS,   30,       TRUE},	// Betriebsart Wärmepumpe       - -

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
	{"WOLF CKL 3000  ", 1, 				SX},						// Modbus Device idx 0
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
	2,		// S1   Wolf CKL3000 erfordert 2 Stopbits
	2,		// S2
	2,		// S3
};	


unsigned char modbusDeviceAddresses[MODBUS_DEVICE_COUNT];

#endif