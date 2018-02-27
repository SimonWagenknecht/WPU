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
	char i;
	
	for(i = 0; i < ZUL_DRUCK_ANZ; i++)
		zul_druck[i].stat = NICHTV;

	for(i = 0; i < ABL_DRUCK_ANZ; i++)
		abl_druck[i].stat = NICHTV;

}


//-----------------------------------
// Modbus-Master
//-----------------------------------

// Tabelle für Differenzdruck-Messumformer DTP RS485 MODBUS von Thermokon Sensortechnik
#if MODBUS_MASTER == 1

const modbusTableRow modbusTable[] = {
// {Adresse des           {Riedel Typ,      {Modbus Typ,     Spezial-   Modbus-     Register-    Register-   Operation,          Intervall AutoSend}
//  Datenpunkts            Komma      },     Komma      },   Konvert.,  Device idx  adresse,     anzahl,                         [sek],

    {P&zul_druck[0],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      0,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
    {P&abl_druck[0],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      1,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck

//  {P&zul_druck[1],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      2,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[1],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      3,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//                                                                      
//  {P&zul_druck[2],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      4,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[2],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      5,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[3],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      6,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[3],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      7,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[4],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      8,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[4],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      9,          0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[5],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      10,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[5],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      11,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[6],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      12,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[6],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      13,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[7],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      14,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[7],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      15,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[8],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      16,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[8],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      17,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//                                                                      
//  {P&zul_druck[9],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      18,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[9],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      19,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//
//  {P&zul_druck[10],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      20,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck
//  {P&abl_druck[10],			{ANA_FORM , 0},   {S_INT_16, 0},   NONE,      21,         0x0001,       1,          READ_INPUT_REGS,  1,        TRUE},  // Druck

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
	{"DRUCK ZULUFT 1 ", 1, 				S3},						// Modbus Device idx 0
	{"DRUCK ABLUFT 1 ", 2, 				S3},						// Modbus Device idx 1
//{"DRUCK ZULUFT 2 ", 3, 				S3},						// Modbus Device idx 2
//{"DRUCK ABLUFT 2 ", 4, 				S3},						// Modbus Device idx 3
//{"DRUCK ZULUFT 3 ", 5, 				S3},						// Modbus Device idx 4
//{"DRUCK ABLUFT 3 ", 6, 				S3},						// Modbus Device idx 5
//{"DRUCK ZULUFT 4 ", 7, 				S3},						// Modbus Device idx 6
//{"DRUCK ABLUFT 4 ", 8, 				S3},						// Modbus Device idx 7
//{"DRUCK ZULUFT 5 ", 9, 				S3},						// Modbus Device idx 8
//{"DRUCK ABLUFT 5 ", 10, 			S3},						// Modbus Device idx 9
//{"DRUCK ZULUFT 6 ", 11, 			S3},						// Modbus Device idx 10
//{"DRUCK ABLUFT 6 ", 12, 			S3},						// Modbus Device idx 11
//{"DRUCK ZULUFT 7 ", 13, 			S3},						// Modbus Device idx 12
//{"DRUCK ABLUFT 7 ", 14, 			S3},						// Modbus Device idx 13
//{"DRUCK ZULUFT 8 ", 15, 			S3},						// Modbus Device idx 14
//{"DRUCK ABLUFT 8 ", 16, 			S3},						// Modbus Device idx 15
//{"DRUCK ZULUFT 9 ", 17, 			S3},						// Modbus Device idx 16
//{"DRUCK ABLUFT 9 ", 18, 			S3},						// Modbus Device idx 17
//{"DRUCK ZULUFT 10", 19, 			S3},						// Modbus Device idx 18
//{"DRUCK ABLUFT 10", 20, 			S3},						// Modbus Device idx 19
//{"DRUCK ZULUFT 11", 21, 			S3},						// Modbus Device idx 20
//{"DRUCK ABLUFT 11", 22, 			S3},						// Modbus Device idx 21
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