/*		Task:	SteuerUni			*/

// Benutzersteuerung mit UNI-Elementen


#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"

#if STEUER_UNI == 1

void SteuerUni(void)
{

	// ----- Testprogramm --------------------------
	if(unid[0].starter == 1)
	{
		unid[0].counter = (UINT)unis[0].Loader * 60;
		unid[0].starter = 0;
	}
	
	if(unid[0].counter > 0)
		unid[0].counter--;	
	//----------------------------------------------
	
}

#endif


