#if MODBUS_UNI > 0

const Pgrup modbusParli[] = {

#if (MODBUS_MASTER == 1 && MODBUS_SLAVE == 1)
	{"***;"," MODBUS         ","       ", P&ModBusMasterSlave,					ASCII_FORM,		0, P&vis,		V0, 0, 0},
	#elif MODBUS_MASTER == 1
	{"***;"," MODBUS         ","       ", P&ModBusMaster,								ASCII_FORM,		0, P&vis,		V0, 0, 0},
	#elif MODBUS_SLAVE == 1
	{"***;"," MODBUS         ","       ", P&ModBusSlave,								ASCII_FORM,		0, P&vis,		V0, 0, 0},
	{"*01;"," Konvertfehler  ","       ", P&ModBusKonvertError,					US_INT,				0, P&hid2,	V0, 0, 0},
	#else
	{"***;"," MODBUS         ","       ", P&Leer15,											ASCII_FORM,		0, P&vis,		V0, 0, 0},
#endif

#if MODBUS_MASTER == 1	//AnFre
	{" 00;"," GERAET         "," # 1-1 ", P&modbusDeviceNr,							EXPAND_CHAR,	0, P&vis,		V0, 0, 0},
	{"*01;"," BEZEICHNUNG    ","       ", P&modbusDevices[0].name,			ASCII_FORM,		0, P&vis,		V0, 0, 0},
	{" 02;"," ADRESSE        ","       ", P&modbusDeviceAddresses[0],			 US_CHAR,		0, P&vis,		V0, 0, 0},
	{"*03;"," R66-SCHNITTST. ","       ", P&modbusDevices[0].sio,		SIO_NAME_FORM,		0, P&vis,		V0, 0, 0},
	{"*03;"," R66-SCHNITTST. ","       ", P&modbusDevices[0].sio,					  S_CHAR,		0, P&hid2,	V0, 0, 0},
#endif

#if ((IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL)
	#if MODBUS_SLAVE == 1
	{" 04;"," SLAVEADRESSE S1","       ", P&modbusSlaveAddresses[0],			 US_CHAR,		0, P&vis,		V0, 0, 0},
	#endif
#endif

#if ((IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL)
	#if MODBUS_SLAVE == 1
	{" 07;"," SLAVEADRESSE S2","       ", P&modbusSlaveAddresses[1],			 US_CHAR,		0, P&vis,		V0, 0, 0},
	#endif
#endif

#if ((IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL)
	#if MODBUS_SLAVE == 1
	{" 10;"," SLAVEADRESSE S3","       ", P&modbusSlaveAddresses[2],			 US_CHAR,		0, P&vis,		V0, 0, 0},
	#endif
#endif


#if MODBUS_MASTER == 1	//AnFre
	#if ((IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL)
	{"*97;"," AKTIVE ZEILE S1","       ", P&activeRow[0],										S_INT,		0, P&vis,		V0, 0, 0},
	#endif

	#if ((IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL)
	{"*98;"," AKTIVE ZEILE S2","       ", P&activeRow[1],										S_INT,		0, P&vis,		V0, 0, 0},
	#endif

	#if ((IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL)
	{"*99;"," AKTIVE ZEILE S3","       ", P&activeRow[2],										S_INT,		0, P&vis,		V0, 0, 0},
	#endif
#endif

};

#endif