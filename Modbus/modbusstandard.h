// Transport von Werten zwischen RAM und EEPROM
// Include-Aufruf von Standard.c

#if MODBUS_UNI > 0

		{EEPADR,	(char	*)&modbusDeviceAddresses[0],				MODBUS_ADDRESSES,				( MODBUS_DEVICE_COUNT > 64 ) ? 64 : MODBUS_DEVICE_COUNT },
		{EEPADR,	(char	*)(&modbusDeviceAddresses[0]+64),		MODBUS_ADDRESSES+64,		( MODBUS_DEVICE_COUNT > 64 ) ? MODBUS_DEVICE_COUNT -64 : 0 },
		{EEPADR,	(char	*)(&modbusSlaveAddresses[0]),				MODBUS_SLAVE_ADDRESSES,	3},

#endif

