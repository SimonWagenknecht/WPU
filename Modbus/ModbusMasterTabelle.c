#include "defins.h"
#include "struct.h"
#include "projdef.h"
#include "uramext.h"
#include "sramext.h"
#include "snoinitext.h"
#include "unoinitext.h"
#include "ustruct.h"
#include "userdef.h"
#include "uconstext.h"
#include "constext.h"
#include "konvform.h"
#include "uskonvform.h"
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
#if(WILO_MODBUS == 1) // IF-Modul Modbus Stratos (2097808)
   	// zu lesende Input-Register
  	{P&modWiloPuData.wiloInReg.mwspActualDifferentialPressure,  {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      1,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [0]	
		{P&modWiloPuData.wiloInReg.mwspFlowRate,  									{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      2,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [1]
  	{P&modWiloPuData.wiloInReg.mwspPowerConsumption,  					{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      3,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [2]
		{P&modWiloPuData.wiloInReg.mwspPowerRating,									{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      4,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [3]
		{P&modWiloPuData.wiloInReg.mwspOperationHours,              {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      5,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [4]
		{P&modWiloPuData.wiloInReg.mwspMainsCurrent,                {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      6,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [5]
		{P&modWiloPuData.wiloInReg.mwspSpeed,                       {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      7,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [6]
		{P&modWiloPuData.wiloInReg.mwspMediumTemperature,           {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      8,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [7]
		{P&modWiloPuData.wiloInReg.mwspOperatingHoursDP,          	{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      9,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [8]
		{P&modWiloPuData.wiloInReg.mwspCurrentOperationMode,        {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      10,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [9]		

		{P&modWiloPuData.wiloInReg.mwspPumpModule,                  {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      16,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [10]	
		{P&modWiloPuData.wiloInReg.mwspPumpType,                    {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      17,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [11]
		{P&modWiloPuData.wiloInReg.mwspMaxSpeed,                    {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      18,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [12]
		{P&modWiloPuData.wiloInReg.mwspMinSpeed,                    {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      19,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [13]
  	
		{P&modWiloPuData.wiloInReg.mwspSupportedErrors,             {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      26,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [14]
		{P&modWiloPuData.wiloInReg.mwspSupportedServiceMessages,    {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      27,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [15]
		{P&modWiloPuData.wiloInReg.mwspMaxPowerRating,              {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      28,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [16]
  	
		{P&modWiloPuData.wiloInReg.mwspServiceMessage,              {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      35,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [17]
		{P&modWiloPuData.wiloInReg.mwspErrorType,                   {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      36,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [18]
		{P&modWiloPuData.wiloInReg.mwspErrorMessage,                {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      37,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [19]
		{P&modWiloPuData.wiloInReg.mwspPumpStatus,                  {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      38,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [20]
		{P&modWiloPuData.wiloInReg.mwspStateDiagnostics,            {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      39,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [21]

		// zu schreibende Holding Register
		{P&modWiloPuData.wiloHoldReg.mwspSetValue,									{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      1,       			1,        WRITE_SINGLE_HOLDING, 	 0,				FALSE},	// [22]
		{P&modWiloPuData.wiloHoldReg.mwspPumpCommand,								{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      40,       			1,        WRITE_SINGLE_HOLDING,  	 0,				FALSE},	// [23]
		{P&modWiloPuData.wiloHoldReg.mwspOperationMode,							{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      42,       			1,        WRITE_SINGLE_HOLDING, 	 0,				FALSE},	// [24]
		{P&modWiloPuData.wiloHoldReg.mwspBusCommandTimer,						{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      300,      			1,        WRITE_SINGLE_HOLDING, 	 0,				FALSE},	// [25]
		
		// zu lesende Holding Register
		{P&modWiloPuData.wiloHoldReg.mwspSetValue,									{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      1,       			1,        READ_HOLDING_REGS,	 		 0, 			FALSE},	// [26]
		{P&modWiloPuData.wiloHoldReg.mwspPumpCommand,								{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      40,       			1,        READ_HOLDING_REGS,	 		 0, 			FALSE},	// [27]
		{P&modWiloPuData.wiloHoldReg.mwspOperationMode,							{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      42,       			1,        READ_HOLDING_REGS,	 		 0, 			FALSE},	// [28]
		{P&modWiloPuData.wiloHoldReg.mwspBusCommandTimer,						{ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      300,      			1,        READ_HOLDING_REGS,	 		 0, 			FALSE},	// [29]
#else
		// Alternative Tabellen
    #if MODBUS_EXT == 1	// siehe projdef.h
    {P&ul_dig32[0],        {NONE , 0},       {COILS, 0},     NONE,      0,          0,       32,         READ_MULTIPLE_COILS, 10,     TRUE},	// Extraktion auf Einzelbytes im Task ModbusExtract
    {P&ul_dig32[1],        {NONE , 0},       {COILS, 0},     NONE,      0,         32,       32,         READ_MULTIPLE_COILS, 10,     TRUE},	// 
    #endif
    
    {P&mod_rt[0],        {ANA_FORM,  1},   {S_INT_16, 1},    NONE,      0,      0x100,       1,          READ_HOLDING_REGS,  3,        TRUE},  // Raumtemperatur 1
    {P&mod_rt[1],        {ANA_FORM,  1},   {S_INT_16, 1},    NONE,      0,      0x101,       1,          READ_HOLDING_REGS,  3,        TRUE},  // Raumtemperatur 2
#endif                                                                        

};
	const unsigned int modbusMasterTableSize = (sizeof(modbusTable)/sizeof(modbusTableRow));
	modbusTableRowMasterData modbusTableData[(sizeof(modbusTable)/sizeof(modbusTableRow))];

//-----------------------------------
// Geräte
//-----------------------------------
const modbusDevice modbusDevices[] = {
//{"Name           ",	Adresse,	Schnittstelle}	// Schnittstellen Implementierung in projdef.h beachten
	#if (WILO_MODBUS == 1)
	{"IF-MOD.Stratos ", 1, 				SX},				    // Modbus Device idx 0 IF-Modul Modbus Stratos (2097808)   
	#else
	{"SENSOR 1       ", 1, 				SX},						// Modbus Device idx 0
	#endif
	};
	const unsigned int modbusDeviceCount = (sizeof(modbusDevices)/sizeof(modbusDevice));

#else	// kein Master definiert

	const modbusTableRow modbusTable[] = {
	};
	const unsigned int modbusMasterTableSize = 0;
	modbusTableRowMasterData modbusTableData[1];
	//-----------------------------------
	// kein Master keine Geräte
	//-----------------------------------
	const modbusDevice modbusDevices[] = {
	};
	const unsigned int modbusDeviceCount = 0;
#endif


unsigned char modbusDeviceAddresses[MODBUS_DEVICE_COUNT];

#endif