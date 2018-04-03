/*	Task:					Steuer WPU		*/
/*	WPU-Steuerung der Ausgänge	AA und DA					*/

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"

//

///*---------------------- Wärmepumpensteuerung -----------------------------------*/
//// Takt: 1 s

void SteuerWPU(void) 

{
	if (wps[WP1].WPU_Freigabe_Haut > 0)
		{
			DA_UNI[U1]->wert = wps[WP1].WPU_Freigabe_Hand_stellen;
		}
		
		else
			{
				
			}
}