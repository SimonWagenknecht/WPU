#include "defins.h"
#include "struct.h"
#include "projdef.h"
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
    #if MODBUS_EXT == 1	// siehe projdef.h
    {P&ul_dig32[0],        {NONE , 0},       {COILS, 0},     NONE,      0,          0,       32,         READ_MULTIPLE_COILS, 10,     TRUE},	// Extraktion auf Einzelbytes im Task ModbusExtract
    {P&ul_dig32[1],        {NONE , 0},       {COILS, 0},     NONE,      0,         32,       32,         READ_MULTIPLE_COILS, 10,     TRUE},	// 
    #endif
    
    {P&mod_rt[0],        {ANA_FORM,  1},   {S_INT_16, 1},    NONE,      0,      0x100,       1,          READ_HOLDING_REGS,  3,        TRUE},  // Raumtemperatur 1
    {P&mod_rt[1],        {ANA_FORM,  1},   {S_INT_16, 1},    NONE,      0,      0x101,       1,          READ_HOLDING_REGS,  3,        TRUE},  // Raumtemperatur 2
                                                                        

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
//{"Name           ",	Adresse,	Schnittstelle}	// Schnittstellen Implementierung in projdef.h beachten
	{"SENSOR 1       ", 1, 				SX},						// Modbus Device idx 0
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