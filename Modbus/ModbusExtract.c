/* Task: ModbusExtract	*/
// josch 161109
// josch 180313 MODBUS_EXT > 0 eingeführt, siehe auch projdef.h, UserRam.c
 
// Extrahieren von MODBUS_EXT x 32 Bits READ_MULTIPLE_COILS auf 128 Bytes zur Anzeige in der parli
// Task-Aufruf (10s) in usertask.h organisiert

#include "struct.h"
#include "defins.h"
#include "uramext.h"

#if MODBUS_UNI > 0

	#if MODBUS_EXT > 0  // bei Verwendung von READ_MULTIPLE_COILS in der ModbusTabelle
	void ModbusExtract(void)
	{
		char i,j;
		ULONG mask = 1;
		
		// Extraktion 32 Bit auf Einzelbytes
		for(j = 0; j < MODBUS_EXT; j++)
		{
			mask = 1;
			for(i = 0; i < 32; i++)
			{
				if(ul_dig32[j] & mask)
					uc_dig[j*32 + i] = 1;
				else
					uc_dig[j*32 + i] = 0;
					
				mask <<= 1;	
			}
		}
		
		// Extraktion 32 Bit auf 4 * 8 Bit
		i = 0;
		for(j = 0; j < MODBUS_EXT; j++)
		{
				uc_dig8[i++] = (char)(ul_dig32[j]);
				uc_dig8[i++] = (char)(ul_dig32[j] >> 8);
				uc_dig8[i++] = (char)(ul_dig32[j] >> 16);
				uc_dig8[i++] = (char)(ul_dig32[j] >> 24);
		}		
		
	}	
	#endif
				
#endif