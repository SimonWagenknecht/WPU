/* Konvertierungen für externe Anwendung  */

#include <string.h>
#include <math.h>
#include "sfr32C87.h"
#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "snoinitext.h"
#include "constext.h"
#include "konvform.h"

unsigned char ucKonvertPuffer[20];
void form2float(float* pfl, char attr);
void float2form(float* pfl, char attr);

//void gwbitset(char bcd, char *pWert);


// Übergabe-Parameter:	Dat_Ptr...Zeiger auf Float bzw. Speicher
//											dae_num...Datenelement-Nummer
//											rw........Lesen(KONVRD) oder Schreiben(KONVWR)
//											exp_num...Expansionsnummer (z.B. WRE-Nr.)
// Return-Parameter:		anz.......Anzahl der gelesenen oder geschriebenen Bytes
//																anz = 0 : Fehler
// Dat_Ptr wird als Float interpretiert und der Wert dorthin gecastet
// Ausnahme: ASCII_FORM,TELE_FORM - hier wird die Adresse des ersten Zeichens als Float konvertiert
char RiedelKonvert(char* Dat_Ptr, unsigned int dae_num, char rw, unsigned char ucExp_num)
{
	// spezielle Nutzung
	char gnum, pnum, konv, attr, alclass, alfunc, anz;
	// allgemein
	char i, mask, c, ret_komma;
	int  intwert;
	unsigned long longwert;
	char 						*pWert;
	int							*pIntwert;
	mwsp 						*pMWSP = NULL;
	zaehlsp 				*pZAEHLSP;
	doutsp					*pDOUTSP;
	dinpsp					*pDINPSP;
	aasp						*pAASP;
	float						*pRetFloat;
	float						*pInFloat;
	char ubuf[3] = { 0 };
	
	gnum = pnum = konv = attr = anz = alclass = alfunc = ret_komma = 0;
	
	// Mit der Datenelement-nummer (dae_num) den Parameter aus der Parli
	// über die Komtab ermitteln
	gnum = KomTab[dae_num].gnum;		// Gruppennummer
	pnum = KomTab[dae_num].pnum;		// Parameternummer
	if( ((dae_num > 0) && (gnum == 0) && (pnum == 0)) || (dae_num > 511))
	{	return(0);										// Datenelement existiert nicht
	}	

	// Adresse von Wert, die Konvertierung und das Attribut aus der Parli ermitteln
	pWert = get_parli_info(gnum, pnum, &konv, &attr, &alclass, &alfunc, ucExp_num);
	
	pRetFloat = (float*)Dat_Ptr;
	pInFloat = (float*)Dat_Ptr;
	switch(konv)
	{
		case ANOUT_FORM:									// unsigned	char	Format
		case KALTST_FORM:
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(unsigned char*)pWert;
				anz = 4;
			}	
			else												// schreiben
			{	
				*(unsigned char*)pWert = ((unsigned char)*pInFloat);
				anz = 4;								
			}			
			break;
		case US_CHAR:									// unsigned	char	Format
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(unsigned char*)pWert;
																											//*pRetFloat /= pow(10,attr);
				form2float(pRetFloat, attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																											//*pInFloat *= pow(10,attr);
				float2form(pInFloat, attr);
				*(unsigned char*)pWert = ((unsigned char)*pInFloat);
				anz = 4;								
			}			
			break;
		case  S_CHAR:									// signed	char	Format
			// Übertragung als 2 Byte Integer
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(signed char*)pWert;
																											//*pRetFloat /= pow(10,attr);
				form2float(pRetFloat, attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																											//*pInFloat *= pow(10,attr);
				float2form(pInFloat, attr);
				*(signed char*)pWert = ((signed char)*pInFloat);
				anz = 4;								// Fehler
			}			
			break;
			
		case JANEIN_FORM:
		case JAFRAGE_FORM:
			// Übertragung als 2 Byte Integer
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(unsigned char*)pWert;
				anz = 4;
			}	
			else {
				*(unsigned char*)pWert = ((unsigned char)*pInFloat);
				anz = 4;								
//				anz = 0;									// schreiben nicht erlaubt
			}
			break;
				
		case JANEIN_FORMOP:						// Zeiger auf Digital-Ausgabe - Speicher 
			// Übertragung als 2 Byte Integer
			if(rw == KONVRD)
			{	
				pDOUTSP	=	(doutsp*)*(long*)pWert;	// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				*pRetFloat = (float) pDOUTSP->wert;
				anz = 4;		
			}	
			else {
				pDOUTSP	=	(doutsp*)*(long*)pWert;	// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				pDOUTSP->wert = *pInFloat;
				if(pDOUTSP->wert!=0) {
					pDOUTSP->wert = 1;
				}
				anz = 4;		
			}
			break;
		
		case JANEIN_FORMIP:						// Zeiger auf Digital-Eingabe - Speicher 
			// Übertragung als 2 Byte Integer
			if(rw == KONVRD)
			{	
				pDINPSP	=	(dinpsp*)*(long*)pWert;	// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				*pRetFloat = (float) pDINPSP->bwert;
				anz = 4;		
			}	
			else {
				pDINPSP	=	(dinpsp*)*(long*)pWert;	// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				pDINPSP->bwert = *pInFloat;
				if(pDINPSP->bwert!=0) {
					pDINPSP->bwert = 1;
				}
				pDINPSP->bstat = 0;
				anz = 4;		
			}
			break;
		//--------------------------------------------------------
/*
		case DIGL_FORM:										// 1 oder 2 Byte in Bitdarstellung
		case DIGR_FORM:
			if(rw ==	KONVRD)								// lesen
			{	if(attr <= 8)									// 1 Byte Kopieren
				{	memcpy(Dat_Ptr,	pWert, 1);
					Dat_Ptr[1] = 0;							// Integer Erweiterung
				}
				else
					memcpy(Dat_Ptr,	pWert, 2);
				anz = 2;
			}	
			else												// schreiben
			{	if(attr <= 8)
				{	if(Dat_Ptr[1] == 0)				// High-Teil muss 0 sein
					{	memcpy(pWert, Dat_Ptr,	1);			
						anz = 2;
					}
					else
						anz = 0;								// Fehler
				}
				else
				{	memcpy(pWert, Dat_Ptr,	2);
					anz = 2;
				}	
			}
			break;
*/
		//--------------------------------------------------------

		case US_INT:									// unsigned	int	Format
		case BAUD_FORM:
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(unsigned int*)pWert;
																										//			*pRetFloat /= pow(10,attr);
				form2float(pRetFloat, attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																														//*pInFloat *= pow(10,attr);
				float2form(pInFloat, attr);
				*(unsigned int*)pWert = ((unsigned int)*pInFloat);
				anz = 4;								// Fehler
			}			
			break;
		case  S_INT:									// signed	int	Format
//		case ZEIT_FORM:								// 2 Byte	Zeit allgemein:	 Index[0]Min,	[1]Std
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(signed int*)pWert;
																														//*pRetFloat /= pow(10,attr);
				form2float(pRetFloat, attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																														//*pInFloat *= pow(10,attr);
				float2form(pInFloat, attr);
				*(signed int*)pWert = ((signed int)*pInFloat);
				anz = 4;								// Fehler
			}			
			break;
		
		case AOUT_FORMP:												// Analogausgänge (früher US_INTP)
			if(rw == KONVRD)
			{
				pAASP	=	(aasp*)*(long*)pWert;				// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				*pRetFloat = pAASP->awert/10.0;
				anz = 4;
			}	
			else {
				pAASP	=	(aasp*)*(long*)pWert;				// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				pAASP->awert = (*pInFloat)*10;
				pAASP->astat = 0;
				anz = 4;
			}
			break;		
		
		case ANA_FORM:
			if(rw == KONVRD)										// lesen
			{ 
				if((attr & 0x80) == 0) {
					*pRetFloat = *(signed int*)pWert;
																																//*pRetFloat /= pow(10,(attr&0x7F));
					form2float(pRetFloat, attr);
					anz = 4;
				} else {
					// Messwertstatus	
					*pRetFloat = pWert[2];
					anz = 4;
				}
			}
			else																// schreiben
			{	
				if((attr & 0x80) == 0)
				{	
																																//*pInFloat *= pow(10,(attr&0x7F));
					float2form(pInFloat, attr);
					*(signed int*)pWert = (signed int)*pInFloat;
					anz = 4;
				}
				else
				{	
					*(unsigned char*)&pWert[2] = ((unsigned char)*pInFloat);
					anz = 4;								
				}			
			}			
			break;			

		case ANA_FORMP:
			if(rw == KONVRD)
			{	pMWSP	=	(mwsp*)*(long*)pWert;	// Extrahieren der Adresse,	auf	die	"pWert" zeigt
				if((attr & 0x80) == 0) { 
					*pRetFloat = pMWSP->messw;
																																	//*pRetFloat /= pow(10,(attr&0x7F));
					form2float(pRetFloat, attr);
					anz = 4;
				}
				else
				{	
					// Messwertstatus	
					*pRetFloat = pMWSP->stat;
					anz = 4;
				}	
			}
			else	{
				if((attr & 0x80) == 0)
				{	
																																	//*pInFloat *= pow(10,(attr&0x7F));
					float2form(pInFloat, attr);
					pMWSP->messw = (signed int)*pInFloat;
					anz = 4;
				}
				else
				{	
					pMWSP->stat = ((unsigned char)*pInFloat);
					anz = 4;								
				}			
			}
			break;			

		

		//--------------------------------------------------------
		case US_LONGP:
			pWert = (unsigned char*)*pWert;

		case US_LONG:
			if(rw ==	KONVRD)						// lesen
			{	
				*pRetFloat = *(unsigned long*)pWert;
																																		//*pRetFloat /= pow(10,attr);
				form2float(pRetFloat, attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																																		//*pInFloat *= pow(10,attr);
				float2form(pInFloat, attr);
				*(unsigned long*)pWert = ((unsigned long)*pInFloat);
				anz = 4;								// Fehler
			}			
			break;


		case ASCII_FORM:
		case EA_NAME_FORM:
			if(attr == 0)
				attr = 15;								// Standard 15 Byte ASCII
			if(rw == KONVRD) {						// lesen
//				*pRetFloat = *((float*)pWert);
//				*pRetFloat = ((float)pWert);
				memcpy(pRetFloat, &pWert,	sizeof(float));
			} else												// schreiben
			{	
				memcpy(pWert, (unsigned char*)*(long*)pInFloat,	attr);
			}
			anz = attr;
			break;				
		
/*		case EEP_ASCII_FORM: 					// EEPROM 
			if(attr == 0)
				attr = 15;								// Standard 15 Byte ASCII
			if(rw ==	KONVRD)						// lesen
			{	
				bicbus(EEPADR,	(char	*)&Dat_Ptr[0], (unsigned	int)(	(long)pWert ),	attr, BICRD);
				anz = attr;
			}
			else
			{
				bicbus(EEPADR,	(char	*)&Dat_Ptr[0], (unsigned	int)(	(long)pWert ),	attr, BICWR);
				anz = attr;
			}
			break;	
*/
		case TELE_FORM:								// Telefonnummern
			if(attr == 0)
				attr = 16;								// Standard 16 Byte ASCII
			if(rw == KONVRD) {						// lesen
//				*pRetFloat = *(float*)pWert;
				memcpy(pRetFloat, &pWert,	sizeof(float));
			} else												// schreiben
			{	
				memcpy(pWert, (unsigned char*)*(long*)pInFloat,	attr);
			}
			anz = attr;
			break;			
		
/*		case UHR_FORM:
			if(rw == KONVRD)
				memcpy(Dat_Ptr,	&Sek, 3);
			else
			{	i = Dat_Ptr[0];						// Sekunde
				if(i > 59)
				{	anz = 0; break;}
				else		
					ubuf[0] = hextobcd(i);
					
				i = Dat_Ptr[1];						// Minute
				if(i > 59)
				{	anz = 0; break;}
				else		
					ubuf[1] = hextobcd(i);
				
				i = Dat_Ptr[2];						// Stunde
				if(i > 23)
				{	anz = 0; break;}
				else		
					ubuf[2] = hextobcd(i);

				bicbus(RTCADR, ubuf, ZEIT_ADR, 3, BICWR);
			}
			anz = 3;
			break;
		*/
		case TASK_FORM:
			if(rw == KONVRD)
			{	
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVRD, ucExp_num, 0);
				anz = 3;
				longwert = (long)&ucKonvertPuffer[10];
				memcpy(pRetFloat, &longwert, sizeof(float));
			} else												// schreiben
			{	
				anz = 3;
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				memcpy(&ucKonvertPuffer[10],(unsigned char*)(long)*pInFloat, anz);
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVWR, ucExp_num, 0);
			}
			break;
		case ZEIT_FORM:
			if(rw ==	KONVRD)						// lesen
			{	
				((unsigned char*)pRetFloat)[0] = ((unsigned char*)pWert)[1];// Tag oder Std
				((unsigned char*)pRetFloat)[1] = ((unsigned char*)pWert)[0];// Monat oder Min
																											//*pRetFloat /= pow(10,attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																											//*pInFloat *= pow(10,attr);
				((unsigned char*)pWert)[1] = ((unsigned char*)pInFloat)[0];
				((unsigned char*)pWert)[0] = ((unsigned char*)pInFloat)[1];
				anz = 4;								
			}			
			break;
/*			if(rw == KONVRD)
			{	
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVRD, ucExp_num);
				anz = 5;
				longwert = (long)&ucKonvertPuffer[8];
				memcpy(pRetFloat, &longwert, sizeof(float));
			} else												// schreiben
			{	
				anz = 5;
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				memcpy(&ucKonvertPuffer[8],(unsigned char*)(long)*pInFloat, anz);
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVWR, ucExp_num);
			}*/
			break;
		case DATUM_FORM:							// aktuelles Datum	3 Byte
			if(rw ==	KONVRD)						// lesen
			{	
				((unsigned char*)pRetFloat)[0] = Tag;
				((unsigned char*)pRetFloat)[1] = Mon;
				((unsigned char*)pRetFloat)[2] = Jahr;
																											//*pRetFloat /= pow(10,attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																											//*pInFloat *= pow(10,attr);
				Tag = ((unsigned char*)pInFloat)[0];
				Mon = ((unsigned char*)pInFloat)[1];
				Jahr = ((unsigned char*)pInFloat)[2];
				anz = 4;								
			}			
			break;
		case DATE_FORM:								// Allgemeines Datum 3 Byte
			if(rw ==	KONVRD)						// lesen
			{	
				((unsigned char*)pRetFloat)[0] = ((unsigned char*)pWert)[2];// Tag
				((unsigned char*)pRetFloat)[1] = ((unsigned char*)pWert)[1];// Monat
				((unsigned char*)pRetFloat)[2] = ((unsigned char*)pWert)[0]; // Jahr
																											//*pRetFloat /= pow(10,attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																											//*pInFloat *= pow(10,attr);
				((unsigned char*)pWert)[2] = ((unsigned char*)pInFloat)[0];
				((unsigned char*)pWert)[1] = ((unsigned char*)pInFloat)[1];
				((unsigned char*)pWert)[0] = ((unsigned char*)pInFloat)[2];
				anz = 4;								
			}			
			break;
		
/*			if(rw == KONVRD)
			{	
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVRD, ucExp_num);
				anz = 8;
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				longwert = (long)&ucKonvertPuffer[5];
				memcpy(pRetFloat, &longwert, sizeof(float));
			} else												// schreiben
			{	
				anz = 8;
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				memcpy(&ucKonvertPuffer[5],(unsigned char*)(long)*pInFloat, anz);
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVWR, ucExp_num);
			}
			break;*/
		case UHR_FORM:		// Spezialkonv. Systemuhr:  Std:Min:Sek
			if(rw ==	KONVRD)						// lesen
			{	
				((unsigned char*)pRetFloat)[0] = Std;
				((unsigned char*)pRetFloat)[1] = Min;
				((unsigned char*)pRetFloat)[2] = Sek;
																											//*pRetFloat /= pow(10,attr);
				anz = 4;
			}	
			else												// schreiben
			{	
																											//*pInFloat *= pow(10,attr);
				Std = ((unsigned char*)pInFloat)[0];
				Min = ((unsigned char*)pInFloat)[1];
				Sek = ((unsigned char*)pInFloat)[2];
				anz = 4;								
			}			
			break;
			
/*		
		case GWOTAG_FORM:							// Gültige Wochentage 1 Byte <--> 3 Byte BCD
		case GWOTAG2_FORM:						// Gültige Wochentage 1 Byte <--> 4 Byte BCD
			anz = (konv == GWOTAG2_FORM) ? 4:3;
			if(rw == KONVRD)
			{	longwert = 0;
				mask = 0x01;
				for(i = 1; i < 8; i++)
				{	mask <<= 1;
					if(*pWert & mask)
					{	longwert <<= 4;
						longwert |= i;	
					}
				}			
				Dat_Ptr[0] = (char)longwert;
				longwert >>= 8;
				Dat_Ptr[1] = (char)longwert;
				longwert >>= 8;
				Dat_Ptr[2] = (char)longwert;

				if(konv == GWOTAG2_FORM)
				{	longwert >>= 8;
					Dat_Ptr[3] = (char)longwert;
				}	
			}
			else
			{	*pWert = 0;
				for(i = 0; i < anz; i++)
				{	c = Dat_Ptr[i] & 0x0F;
					gwbitset(c, pWert);
					c = (char)(Dat_Ptr[i] >> 4);
					gwbitset(c, pWert);
				}
			}		
			break;	
		
		case TASK_FORM:			// ReAnimation: Anzeige des System- oder Usertask, der zum Absturz führte
			if(rw == KONVRD)
			{	Dat_Ptr[0] = noi_reanimate;
				Dat_Ptr[1] = noi_sys_tasknum_copy;
				Dat_Ptr[2] = noi_user_tasknum_copy;
				anz = 3;
			}
			else
				anz = 0;
			break;
	*/			


		//--------------------------------------------------------

		case ZE_FORMP:																// Zeiger auf Zähler - Speicher 
			pZAEHLSP	=	(zaehlsp*)*(long*)pWert;				// Extrahieren der Adresse,	auf	die	"pWert" zeigt
			if(rw ==	KONVRD)														// lesen
			{	switch(attr)
				{	case 0:																	// 4 Byte Zählwert
						// Umwandeln Basiswert in Anzeigewert
						longwert = basiswert_to_displaywert(pZAEHLSP->zdim.komma, pZAEHLSP->zdim.dim_idx, pZAEHLSP->zwert, &ret_komma);
						*pRetFloat = longwert;
//						memcpy(Dat_Ptr, &longwert, 4);
						anz = 4;
						break;
					
					case 1:																	// Zählername in 15 Byte ASCII
						longwert = (long)&pZAEHLSP->zdim.zname;
						memcpy(pRetFloat, &longwert, sizeof(float));
						anz = 15;
						break;
					
					case 41:																// Zählernummer in 15 Byte ASCII
						longwert = (unsigned long)&pZAEHLSP->zdim.znumm;
						memcpy(pRetFloat, &longwert, sizeof(float));
						anz = 15;
						break;
					
					default:																// Monatverbräuche aktuell und Vorjahr
						if(attr > 9 && attr < 34)							// attr 10...33 = JAN...DEZ VJ
						{	
							if(attr < 22)
								// 4 Byte aktuell
								longwert = basiswert_to_displaywert(pZAEHLSP->zdim.komma, pZAEHLSP->zdim.dim_idx, pZAEHLSP->verbr_akt[attr-10], &ret_komma);
							else
								// 4 Byte Vorjahr
								longwert = basiswert_to_displaywert(pZAEHLSP->zdim.komma, pZAEHLSP->zdim.dim_idx, pZAEHLSP->verbr_vor[attr-22], &ret_komma);
							
							*pRetFloat = longwert;
//							memcpy(Dat_Ptr, &longwert, 4);
							anz = 4;
						}
						break;
				}		
			}
			else	// Schreiben
				anz = 0;
			break;				
		//--------------------------------------------------------
		
		default:
			if(rw == KONVRD)
			{	
				// Achim arbeitet in Konvert.c Hardcoded mit Index5
				memcpy(ucKonvertPuffer, Leer15, 15);
				Konvert( ucKonvertPuffer, pWert, konv, attr, KONVRD, ucExp_num, 0);
				anz = 15;
				longwert = (long)&ucKonvertPuffer[5];
				memcpy(pRetFloat, &longwert, sizeof(float));
			} else												// schreiben
			{	
			}
//			anz = UserLzKonv(Dat_Ptr, pWert, konv, attr, rw, ucExp_num);
			break;
	}			
	

	return(anz);	
	
}		
	 
// UP Umwandlung von einem zugeschnittenen Format in das float-Format, abhängig von der Kommastelle (Attribut)
void form2float(float* pfl, char attr)
{
	char i;
	char atr = attr & 0x7F;

	for(i=0; i<atr; i++)
		*pfl /= 10.0;

}			
	 
// UP Umwandlung vom float-Format in ein zugeschnittenen Format, abhängig von der Kommastelle (Attribut)
void float2form(float* pfl, char attr)
{
	char i;
	char atr = attr & 0x7F;

	for(i=0; i<atr; i++)
		*pfl *= 10.0;

}			

/*// Unterprogramm zum Ermitteln von Konvertierungsnummer und Attribut eines Komtab-Eintrages
// und Laden eines Puffers  
char LzForm(char* Dat_Ptr, unsigned int dae_num)		
{
	char gnum, pnum, konv, attr, alclass, alfunc;
	char *pWert;
	char buf[20] = {0};
	
	
	gnum = pnum = konv = attr = alclass = alfunc = 0;
		 
	// Mit der Datenelement-nummer (dae_num) den Parameter aus der Parli
	// über die Komtab ermitteln
	gnum = KomTab[dae_num].gnum;		// Gruppennummer
	pnum = KomTab[dae_num].pnum;		// Parameternummer
	if( ((dae_num > 0) && (gnum == 0) && (pnum == 0)) || (dae_num > 511))
	{	return(0);										// Datenelement existiert nicht
	}	

	// Adresse von Wert, die Konvertierung und das Attribut aus der Parli ermitteln
	pWert = get_parli_info(gnum, pnum, &konv, &attr, &alclass, &alfunc, 0);
	
	if(konv == ZE_FORMP)
	{
		attr	=	Konvert( &buf[0], pWert, konv, attr, KONVRD, 0);
		if(attr & 0x80)							// Bei Zählerkonvertierungen wird bit7 gesetzt,
																// der Rest ist Datenart und Kommastelle (siehe Z_Konvert in Konvert.c)
				attr = attr & 0x7F;			// Datenart und Kommastelle
	}
	
	
	Dat_Ptr[0] = konv;
	Dat_Ptr[1] = attr;
	
	return(1);
}
	
*/
