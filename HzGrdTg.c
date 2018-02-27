/* Task zur Berechnung der Heizgradtage */
/* laeuft einmal pro Minute							*/

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"

#define H6 	1
#define H7 	2
#define H14 4
#define H21 8

void HeizGradTag ( void )
{
#if HEIZGRD == 1

	int ta;
	static char neustart = 1;
	
	if ( neustart )
	{
		HgtAnzeigen ( monHzGrdAnz );
		neustart = 0;
	}	

	if ( Mon == 0 || Mon > 12 )
		return;												// ein unsinniger Monat wuerde zu unkontrollierten Speicher-Zugriffen fuehren
		
	switch ( Std )
	{
		case 0:
			if ( Mon == 1 && Tag == 1 && indexMon < 12 )
			{														// Werte zu Vorjahres-Werten machen
				bicbus ( EEPADR, (char *)&sMonHzGrd, HEIZGRADTAGE_ADR + indexMon * sizeof(monHzGrd), sizeof(monHzGrd), BICRD );	
				sMonHzGrd.hzGrdTageVj = sMonHzGrd.hzGrdTage;
				sMonHzGrd.hzTageVj = sMonHzGrd.hzTage;
				sMonHzGrd.hzTageBewVj = sMonHzGrd.hzTageBew;				
				sMonHzGrd.hzGrdTage = 0; sMonHzGrd.hzTage = 0; sMonHzGrd.hzTageBew = 0;
				bicbus ( EEPADR, (char *)&sMonHzGrd, HEIZGRADTAGE_ADR + indexMon * sizeof(monHzGrd), sizeof(monHzGrd), BICWR );												
				++indexMon;
				bicbus ( EEPADR, &indexMon, INDEX_MON_ADR, 1, BICWR );							
			}
						
			break;	

		case 6:
			if ( ( merkerStd & H6 ) == 0 )
			{														// EEPROM : Temperatur-Werte von 7 und 14 Uhr loeschen
				ta7 = 0x7FFF;
				ta14 = 0x7FFF;																	
	 			bicbus ( EEPADR, (char *)&ta7, TA7_ADR, 4, BICWR );	// klappt nur, wenn ta7 und ta14 im RAM unmittelbar nacheinander
				merkerStd = H6;	
				bicbus ( EEPADR, &merkerStd, MERKER_STD_ADR, 1, BICWR );				
							
				if ( Tag == 1 && Mon != 1 )
				{												// EEPROM : aktuelle Monatswerte vorsichtshalber nochmal loeschen
					bicbus ( EEPADR, (char *)&sMonHzGrd, HEIZGRADTAGE_ADR + ( Mon-1 ) * sizeof(monHzGrd), sizeof(monHzGrd), BICRD );																		 
					sMonHzGrd.hzGrdTage = 0; sMonHzGrd.hzTage = 0; sMonHzGrd.hzTageBew = 0;
					bicbus ( EEPADR, (char *)&sMonHzGrd, HEIZGRADTAGE_ADR + ( Mon-1 ) * sizeof(monHzGrd), sizeof(monHzGrd), BICWR );						
				}
			}	
			
			break;	
		
		case 7:
			if ( ( merkerStd & H7 ) == 0 )
			{
				if ( ta1m.stat == 0 && ta1m.messw > -500 && ta1m.messw < 600 )
				{
					ta7 = ta1m.messw;
					bicbus ( EEPADR, (char *)&ta7, TA7_ADR, 2, BICWR );
					merkerStd = H7;
					bicbus ( EEPADR, &merkerStd, MERKER_STD_ADR, 1, BICWR );																				
				}				
			}
					
			break;
			
		case 14:
			if ( ( merkerStd & H14 ) == 0 )
			{
				if ( ta1m.stat == 0 && ta1m.messw > -500 && ta1m.messw < 600 )
				{
					ta14 = ta1m.messw;
					bicbus ( EEPADR, (char *)&ta14, TA14_ADR, 2, BICWR );
					merkerStd = H14;
					bicbus ( EEPADR, &merkerStd, MERKER_STD_ADR, 1, BICWR );																				
				}				
			}	
				
			break;
					
		case 21:
			if ( ( merkerStd & H21 ) == 0 )
			{
				if ( ta1m.stat == 0 && ta1m.messw > -500 && ta1m.messw < 600 )
				{
					ta = ta1m.messw;
					if ( ta7 > -500 && ta7 < 600 && ta14 > -500 && ta14 < 600 )
					{
						ta = ( 2 * ta + ta7 + ta14 ) / 4;						
						bicbus ( EEPADR, (char *)&sMonHzGrd, HEIZGRADTAGE_ADR + ( Mon-1 ) * sizeof(monHzGrd), sizeof(monHzGrd), BICRD );	
						sMonHzGrd.hzTageBew++;	// alle Temp. korrekt erfasst : bewerteter Tag
						if ( ta < 150 )
						{
							sMonHzGrd.hzTage++;		// Aussentemp. < 15.0 : Heiztag									
							sMonHzGrd.hzGrdTage += 150 - ta;
						}
	 					bicbus ( EEPADR, (char *)&sMonHzGrd, HEIZGRADTAGE_ADR + ( Mon-1 ) * sizeof(monHzGrd), sizeof(monHzGrd), BICWR );					
					}
					merkerStd = H21;
					bicbus ( EEPADR, &merkerStd, MERKER_STD_ADR, 1, BICWR );																				
				}				
			}
						
			break;	
			
		case 23:

			if ( Tag == 31 && Mon == 12 && indexMon != 0 )			// Verschieben augf Vorjahr vorbereiten
			{
				indexMon = 0;	
				bicbus ( EEPADR, &indexMon, INDEX_MON_ADR, 1, BICWR );							
			}
			
			break;									

	}
#endif
}


