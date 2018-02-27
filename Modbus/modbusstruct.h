// Modbus Strukturen
// Include-Aufruf von "ustruct.h"

	typedef struct
		{
			unsigned char name[16];
			unsigned char address;
			signed char sio;
		} modbusDevice;

	typedef struct
		{
			unsigned char type;
			unsigned char param;
		} DataType;

	typedef struct
		{
			void * address; 							// Adresse des Parameters
			DataType riedelType; 					// Datentyp auf Riedel Seite
			DataType modbusType; 					// Datentyp auf Modbus Seite
			unsigned char convert;
			unsigned char deviceIndex;		// Index des Slaves in modbusDevices
			unsigned int registerAddress; // Adresse des Modbus Registers
			unsigned int numRegisters;		// Anzahl der zu bearbeitenden Register. Ein Register entspricht zwei Byte
			unsigned char operation; 			// Durchzuführende Operation
			unsigned int interval; 				// Anzahl der Sekunden Zwischen zwei Operationen
			unsigned char autoSend;				// Automatisches Senden der Daten
		} modbusTableRow;

	typedef struct
		{
			void * address; 							// Adresse des Parameters
			DataType riedelType; 					// Datentyp auf Riedel Seite
//			DataType modbusType; 					// Datentyp auf Modbus Seite
			unsigned char convert;
			unsigned int registerAddress; // Adresse des Modbus Registers
			unsigned int numRegisters;		// Anzahl der zu bearbeitenden Register. Ein Register entspricht zwei Byte
			unsigned char permission; 			// Berechtigung Lesen oder Lesen und Schreiben
			unsigned int timeout;			// Timeout in Minuten bevr der Datenpunkt einen Feher generiert
			unsigned char useFallback;		// Legt fest, ob bei einem Fehler auf den Ursprünglichen Wert zugegriffen werden soll
		} modbusSlaveTableRow;

/*********************************************************/
// die ersten Parameter beider Strukturen müssen gleich sein,
// sonst gibt es unvorhersehbare Seiteneffekte in generateShadowCopy
	typedef struct
		{
			unsigned long counter;
			void* oldAddress;
			unsigned char data[3]; // Hier werden die Daten für die Strukturen gelagert
			unsigned char status;
			unsigned char ready;
			unsigned char retries;
			unsigned char autoSend;				// Automatisches Senden der Daten
		} modbusTableRowMasterData;

	typedef struct
		{
			unsigned long counter;
			void* oldAddress;
			unsigned char data[3]; // Hier werden die Daten für die Strukturen gelagert
			unsigned char status;
		} modbusTableRowSlaveData;
/*********************************************************/
