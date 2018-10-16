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
// {Adresse des          										 {Riedel Typ,      {Modbus Typ,     Spezial-   Modbus  Register-   Register-   						Operation,      Intervall,	AutoSend}
//  Datenpunkts                   									 Komma},     Komma     },   Konvert.,  Device  adresse,     anzahl,                                   [sek]
//                                                                      idx
  	// Betriebsdaten
  	{P&mod_data.i_R_Aussenlufttemperatur,		  {US_INT,  0},   	{FLOAT, 0},  			NONE,      0,      1,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	
  	{P&mod_data.i_R_Ruecklauftemperatur,		  {US_INT,  0},   	{FLOAT, 0},  			NONE,      0,      2,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	
  	{P&mod_data.i_R_Ruecklaufsolltemperatur,  {US_INT,  0},   	{FLOAT, 0},  			NONE,      0,      53,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	
		//Historie
		{P&mod_data.uc_R_Verdichter1_Stunden,  		{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      72,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_Verdichter2_Stunden,  		{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      73,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
   	{P&mod_data.uc_R_Primaerpumpe_Stunden, 		{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      74,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_Heizungspumpe_Stunden,		{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      76,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
   	{P&mod_data.uc_R_Warmwasserpumpe_Stunden,	{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      77,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
    // 1. Heizkreis
    {P&mod_data.uc_W_Parallelverschiebung,  	{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      163,       		1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]	 
   	{P&mod_data.i_W_Raumtemperatur,  					{US_INT,  0},   	{U_INT_16, 0},  	NONE,      0,      46,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]
   	{P&mod_data.i_W_Festwertsolltemperatur, 	{US_INT,  0},   	{U_INT_16, 0},  	NONE,      0,      164,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]	 
   	{P&mod_data.i_W_Heizkurvenendpunkt,				{US_INT,  0},   	{S_INT_16, 0},  	NONE,      0,      165,       		1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]
   	{P&mod_data.i_W_Hysterese_HK_IN,  				{US_INT,  0},   	{U_INT_16, 0},  	NONE,      0,      47,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]
   	{P&mod_data.i_Hysterese_HK_OUT, 					{US_INT,  0},   	{U_INT_16, 0},  	NONE,      0,      47,      			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.i_W_Solltemp_dyn_Kuehlung,		{US_INT,  0},   	{S_INT_16, 0},  	NONE,      0,      170,       		1,        WRITE_SINGLE_HOLDING,  		 30,				TRUE},	// [0]
		// Modbus
		{P&mod_data.uc_W_Betriebsmodus,  				  {US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      142,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]
   	{P&mod_data.uc_W_Anzahl_Partystunden, 	  {US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      143,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_Anzahl_Urlaubstage,		  {US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      144,       		1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]
		// Warmwasser
		{P&mod_data.i_W_Hysterese_TWE,  				  {US_INT,  0},   	{U_INT_16, 0},  	NONE,      0,      172,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]
   	{P&mod_data.i_W_Solltemperatur_TWE, 	  	{US_INT,  0},   	{U_INT_16, 0},  	NONE,      0,      174,      			1,        WRITE_SINGLE_HOLDING,  	 	 30,				TRUE},	// [0]	 
		// Displayanzeigen
		{P&mod_data.uc_R_statusmeldung,  					{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      103,       		1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_WPU_Sperre,  						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      104,       		1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
   	{P&mod_data.uc_R_WPU_SM, 									{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      105,       		1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_WPU_Sensorik,						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      106,       		1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
		// Eingänge
		{P&mod_data.uc_R_Warmwasserthermostat,  	{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      57,        		1,        READ_MULTIPLE_COILS,   		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_Schwimmbadthermostat,  	{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      58,        		1,        READ_MULTIPLE_COILS,   		 30,				TRUE},	// [0]
   	{P&mod_data.uc_R_EVU_Sperre, 							{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      56,        		1,        READ_MULTIPLE_COILS,   		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_Sperre_Extern,						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      63,        		1,        READ_MULTIPLE_COILS,   		 30,				TRUE},	// [0]
		// Ausgänge
		{P&mod_data.uc_R_Verdichter1,  						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      41,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_Verdichter2,  						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      42,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
   	{P&mod_data.uc_R_Primaerpumpe, 						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      42,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_R_Heizungspumpe,						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      45,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
   	{P&mod_data.uc_R_Warmwasserpumpe,					{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      46,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
		{P&mod_data.uc_R_Sammelstoerung,					{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      57,       			1,        READ_HOLDING_REGS,  	 		 30,				TRUE},	// [0]
		// Zeitabgleich
		{P&mod_data.uc_RW_Stunde_Za,  						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      133,      			1,        WRITE_SINGLE_HOLDING,	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_setStunde_Za,  					{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      102,      			1,        WRITE_MULTIPLE_COILS,  		 30,				TRUE},	// [0]
		{P&mod_data.uc_RW_min_Za,  								{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      134,      			1,        WRITE_SINGLE_HOLDING,	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_setmin_Za,  							{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      103,      			1,        WRITE_MULTIPLE_COILS,  		 30,				TRUE},	// [0]
		{P&mod_data.uc_RW_Monat_Za,  							{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      135,      			1,        WRITE_SINGLE_HOLDING,	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_setMonat_Za,  						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      105,      			1,        WRITE_MULTIPLE_COILS,  		 30,				TRUE},	// [0]
		{P&mod_data.uc_RW_Wochentag_Za,  					{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      136,      			1,        WRITE_SINGLE_HOLDING,	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_setWochentag_Za,  				{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      107,      			1,        WRITE_MULTIPLE_COILS,  		 30,				TRUE},	// [0]
		{P&mod_data.uc_RW_Tag_Za,  								{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      137,      			1,        WRITE_SINGLE_HOLDING,	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_setTag_Za,  							{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      104,      			1,        WRITE_MULTIPLE_COILS,  		 30,				TRUE},	// [0]
		{P&mod_data.uc_RW_Jahr_Za,  							{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      138,      			1,        WRITE_SINGLE_HOLDING,	 		 30,				TRUE},	// [0]	 
   	{P&mod_data.uc_W_setJahr_Za,  						{US_CHAR,  0},   	{U_INT_16, 0},  	NONE,      0,      106,      			1,        WRITE_MULTIPLE_COILS,  		 30,				TRUE},	// [0]

   	// zu lesende Input-Register
  	//{P&modWiloPuData.wiloInReg.mwspActualDifferentialPressure,  {ANA_FORM,  0},   	{U_INT_16, 0},  	NONE,      0,      1,       			1,        READ_INPUT_REGS,  	 		 0,				FALSE},	// [0]	
		
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
	{"Dimplex-SW-L   ", 1, 				SX},						// Modbus Device idx 0
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