/* Task: ModbusExtract	*/
// josch 161109
 
// Extrahieren von 4 x 32 Bits READ_MULTIPLE_COILS auf 128 Bytes zur Anzeige in der parli
// Task-Aufruf (10s) in usertask.h organisiert

#include "struct.h"
#include "defins.h"
#include "uramext.h"

#if MODBUS_UNI > 0
#if MODBUS_EXT == 1  // bei Verwendung von READ_MULTIPLE_COILS in der ModbusTabelle

void ModbusExtract(void)
{
	char i,j;
	ULONG mask = 1;
	
	for(j = 0; j < 4; j++)
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
}	
#endif			
#endif