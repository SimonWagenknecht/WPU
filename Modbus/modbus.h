//
#define MODBUS_DEVICE_COUNT 		128

// MODBUS-Functioncodes
// reads
#define READ_MULTIPLE_COILS      		1    // Einzel-Bit-Werte aus Ausgangsregistern Lesen
#define READ_DISCRETE_INP    				2    // Einzel-Bit-Werte aus Discreten Inputregistern lesen (read inputs)
#define READ_HOLDING_REGS    				3		 // 16Bit-Werte aus I/O-Registern lesen
#define READ_INPUT_REGS      				4    // 16Bit-Werte aus Input-Registern lesen (Meßwerte,nicht beschreibbar)

// writes
#define WRITE_SINGLE_COIL    				5    // Einzel-Bit-Werte in Ausgangsregister schreiben (wie Relais-Ausgänge, Einzelne Werte)
#define WRITE_SINGLE_HOLDING 				6    // 16Bit-Werte in I/O-Register schreiben (Einzelne Werte)
#define WRITE_MULTIPLE_COILS 				15	 // Einzel-Bit-Werte in Ausgangsregister schreiben (wie Relais-Ausgänge, mehrere Bits hintereinander)
#define WRITE_MULTIPLE_HOLDINGS 		16   // 16Bit-Werte in I/O-Register schreiben (Mehrere Werte hintereinander)

// Extended registers
#define READ_EXTENDED        				0x14
#define WRITE_EXTENDED       				0x15

// Exception Codes
#define ILLEGAL_FUNCTION						0x01
#define ILLEGAL_DATA_ADDRESS				0x02
#define	ILLEGAL_DATA_VALUE					0x03
#define SLAVE_DEVICE_FAILURE				0x04

// Modbus Datentypen:
#define U_INT_8			0
#define S_INT_8			1
#define U_INT_16		2
#define S_INT_16		3
#define U_INT_32		4
#define S_INT_32		5
#define FLOAT				6
#define BINARY			7
#define U_INT_64		8
#define S_INT_64		9
#define COILS       10
#define KONVERT_ERROR       255

// Spezialkonvertierungen bei der Übertragung von Daten
#define NONE							0x00
#define ENOCEAN_TEMP_1090	0x01

#define SIO_ERROR					0x03
#define	CRC_ERROR					0x04
#define EXEPTION					0x05
#define SoF_ERROR					0x07
#define	LENGT_ERROR				0x08
#define	RX_BUF_READY			0x80


// Berechtigungen für Modbus-Slave Table
#define READ_ONLY					0x01
#define READ_WRITE				0x02

