/*	1.User-Konvertierungen für Task: Bedien, zur Ausgabe auf LCD-Anzeige
			Der Aufruf erfolgt aus Konvert.c
			
		2.Veränderung der Ersten Zeile: user_erste_zeile
			Aufruf erfolgt aus Bedien.c , Funktion ZweiZeilen

		3.User-Leittechnikkonvertierungen	zur Abarbeitung von Leittechnikkommnados
			Der Aufruf erfolgt aus LzKonv.c 


		reservierte Konvertierungsnummern: ab 200	(siehe uskonvform.h)

*/

#include <string.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "konvform.h"
#include "uskonvform.h"
#include "constext.h"
#include "uconstext.h"
#include "projdef.h"

extern void ctoa(char *buf, char ch);
extern void ctoa3(char	*buf,	unsigned char	ch);
extern void asci1(char* Visual_Ptr, char wert, char komma, char signflag);
extern void asci2(char* Visual_Ptr, unsigned int wert, char komma, char signflag);
extern void asci4(char* Visual_Ptr, unsigned long wert, char komma);
extern UINT bin2(char* Visual_Ptr, char komma, char signflag);
void asci4s ( char* Visual_Ptr, long wert, char komma );
extern void set_vornul(char* Visual_Ptr, char komma, char vorz, char ziffern, char overfl);

#if MODBUS_UNI > 0
extern char user_erste_zeile_modbus(char*	Visual_Ptr,	char gnum, char	pnum, unsigned char ucExp_num);
#endif


//--------------------------------------------------------------------------------------
// 1.  User-Konvertierungen für Task: Bedien, zur Ausgabe auf LCD-Anzeige
//--------------------------------------------------------------------------------------
char User_Konv(char* Visual_Ptr, char *wert, char konv, char attr, char rw)
{
	#define KO_LONG		0x20		// ist auch in Konvert_Z definiert

	char i, ret = TRUE;		// Nur bei Konvertierungsfehlern auf FALSE gesetzt 
	MbusDynam *pZAEHLMB;
	MbusStandard *pMbusPara;
	zaehlsp *pZAEHLSP;
	char komma;
	UINT in;
	unsigned  long longwert;
	char ret_komma;
	char charwert;
	int intwert;
	signed char s_char;
	char vtext[7] = {0};

// ZLT Funktionsnamen für Schnittstellen
	switch(konv)
	{
		case FU_NAME_FORM:
			if(rw ==	KONVRD)						// lesen
			{	
				charwert = wert[0];
				asci1(Visual_Ptr,	charwert,	attr,	SIGNNO);
				memcpy(&Visual_Ptr[14],	&fu_namen[charwert], 6);		// Funktionsnamentext
			}
			else												// schreiben
			{	intwert	=	bin2(Visual_Ptr, attr, SIGNNO);
				if(intwert > 255)
					ret = FALSE;
				else
					wert[0] = (char)intwert;
			}
			break;
		
	}

// Modbus
#if MODBUS_UNI > 0
	switch(konv)
	{
		case SIO_NAME_FORM:
			s_char = wert[0];
			if(s_char == -1)
			{	
				if((IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL)
					strcat(vtext, "S1");
				else
					strcat(vtext, "__");
					
				if((IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL)
					strcat(vtext, "S2");
				else
					strcat(vtext, "__");
				
				if((IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL)
					strcat(vtext, "S3");
				else
					strcat(vtext, "__");
				
				strncpy(&Visual_Ptr[5],		"VARIABEL ",	9);
				strncpy(&Visual_Ptr[14], vtext, 6); 
			}
			
			else if(s_char == 0)	
				strncpy(&Visual_Ptr[5],		"FEST  S1       ",	15);
			else if(s_char == 1)	
				strncpy(&Visual_Ptr[5],		"FEST  S2       ",	15);
			else if(s_char == 2)	
				strncpy(&Visual_Ptr[5],		"FEST  S3       ",	15);
		break;
	}	

#endif

#if ( GENI == 1 || WILO_MODBUS == 1 || GRUNDFOS_MODBUS == 1 )

	#if BUS_PU_MAX > 0
		
		switch(konv)
		{
			case PU_CTR_FORM:
				if ( rw ==	KONVRD )
				{
					if ( *wert <= 3 )
						strcpy ( &Visual_Ptr[5],	pu_control_set[*wert]);
					else
						strcpy ( &Visual_Ptr[5],	ClearDisplay );
				}		
				break;	
				
			case PU_OPR_FORM:
				if ( rw ==	KONVRD )
				{
					if ( *wert <= 3 )
						strcpy ( &Visual_Ptr[5],	pu_operation_set[*wert]);
					else
						strcpy ( &Visual_Ptr[5],	ClearDisplay );
				}		
				break;		
				
			case PU_FUNC_FORM:
				if ( rw ==	KONVRD )
				{
					if ( *wert <= 2 )
						strcpy ( &Visual_Ptr[5],	pu_func_set[*wert]);
					else
						strcpy ( &Visual_Ptr[5],	ClearDisplay );
				}		
				break;		
	
		}
	
	#endif	// if BUS_PU_MAX
#endif		// if BUS_PUMPEN



#if ( MBUSANZ > 0 )

	switch ( konv )
	{
		case MBUS_S_LONG:									// speziell für Anzeige mit einer Kommastelle
			if ( rw ==	KONVRD )						// lesen
			{	
				asci4s ( Visual_Ptr, *(long*)wert, attr );
			}
			break;
			
		case MBUS_TYP_FORM:
			pMbusPara = (MbusStandard*)((long*)wert);	
			if ( rw ==	KONVRD )
			{
				asci1 ( Visual_Ptr,	pMbusPara->Typ,	0, SIGNNO ); 
				switch ( pMbusPara->Typ )
				{
					case WRM:	
						strcpy ( &Visual_Ptr[14],	"WAERME");				
						break;									
					case ELT:	
						strcpy ( &Visual_Ptr[14],	"ELT   ");				
						break;									
					case KWS:	
						strcpy ( &Visual_Ptr[14],	"KWZ   ");				
						break;									
					case WWS:	
						strcpy ( &Visual_Ptr[14],	"WWZ   ");				
						break;									
					case KLM:	
						strcpy ( &Visual_Ptr[14],	"KLIMA ");				
						break;	
					case GMB:	
						strcpy ( &Visual_Ptr[14],	"GAS   ");				
						break;	
					default:	
						strcpy ( &Visual_Ptr[14],	"???   ");				
						break;									
				}
			}
			else
			{
				in	=	bin2 ( Visual_Ptr, attr, SIGNNO );
				if ( in > 255)
					ret	=	FALSE;
				else
					pMbusPara->Typ = (char)in;
				break;
			}
			break;
		
		case MBUS_WMENG_FORM:
			if ( rw ==	KONVRD )
			{
				pZAEHLMB = (MbusDynam*)((long*)wert);	
				komma = pZAEHLMB->komma.wmeng;
				if ( komma <= 3 )
				{
					if ( pZAEHLMB->wmeng < 10000000 || komma == 0 )
						asci4 ( Visual_Ptr,	pZAEHLMB->wmeng, komma );
					else																	// falls Komma vorhanden, wird es um eine Stelle verschoben
						asci4 ( Visual_Ptr,	pZAEHLMB->wmeng / 10, (char)( komma - 1 ) );
					memcpy ( &Visual_Ptr[14],	&ze_dim_namen[4].zdtext, 6 );	// Dimension MWh
				}
				else
				{
					komma -= 3;
					asci4 ( Visual_Ptr,	pZAEHLMB->wmeng, komma );
					memcpy ( &Visual_Ptr[14],	&ze_dim_namen[5].zdtext, 6 );	// Dimension kWh					
				}
				ret =  komma | KO_LONG | 0x80;	
			}
			break;
			
		case MBUS_VOL_FORM:
			if ( rw ==	KONVRD )
			{
				pZAEHLMB = (MbusDynam*)((long*)wert);	
				komma = pZAEHLMB->komma.volume;
				if ( pZAEHLMB->volume < 10000000 || komma == 0 )
					asci4 ( Visual_Ptr,	pZAEHLMB->volume, komma );
				else																	// falls Komma vorhanden, wird es um eine Stelle verschoben
					asci4 ( Visual_Ptr,	pZAEHLMB->volume / 10, (char)( komma - 1 ) );
				memcpy ( &Visual_Ptr[14],	&ze_dim_namen[6].zdtext, 6 );		// Dimension cbm
				ret =  komma | KO_LONG | 0x80;
			}	
			break;

		case MBUS_ZWERT_FORM:
			if ( rw ==	KONVRD )
			{
				pZAEHLMB = (MbusDynam*)((long*)wert);
				komma = pZAEHLMB->kommaLT;
				charwert = pZAEHLMB->dim_idx; 
				longwert = basiswert_to_displaywert ( komma, charwert, pZAEHLMB->zwertAkt, &ret_komma);
				asci4 ( Visual_Ptr,	longwert, ret_komma );		
				memcpy ( &Visual_Ptr[14],	&ze_dim_namen[charwert].zdtext, 6 );											
				ret =  komma | KO_LONG | 0x80;
			}
			break;			

		case MBUSMASTER_SYNCVJ_FORM:
			pZAEHLSP = (zaehlsp*)*(long*)wert;	// Extrahieren der Adresse,	auf	die	"wert" zeigt
			komma = pZAEHLSP->zdim.komma;
			if ( rw ==	KONVRD )
			{
				charwert = pZAEHLSP->zdim.dim_idx;														// Dimensionsnummer
				longwert = basiswert_to_displaywert ( komma, charwert, pZAEHLSP->syncw_vj, &ret_komma );
				asci4 ( Visual_Ptr,	longwert, ret_komma );
				memcpy ( &Visual_Ptr[14],	&ze_dim_namen[charwert].zdtext,6 );	// Dimensionstext
				ret =  komma | KO_LONG | 0x80;
			}
			else
			{
				longwert	=	bin4 ( Visual_Ptr, komma, SIGNNO );
				pZAEHLSP->syncw_vj = displaywert_to_basiswert ( longwert, komma, pZAEHLSP->zdim.dim_idx );
				pZAEHLSP->zwert_vj = pZAEHLSP->syncw_vj;
				ret =  komma | 0x80;
			}
			break;			

		case MBUSMASTER_SYNCMON_FORM:
			pZAEHLSP = (zaehlsp*)*(long*)wert;	// Extrahieren der Adresse,	auf	die	"wert" zeigt
			charwert = pZAEHLSP->sync_monat;
			komma = pZAEHLSP->zdim.komma;
			ret = komma;
			if ( rw ==	KONVRD )
			{
				if ( charwert > 0 && charwert < 25 )
				{
					if(charwert < 13)
						longwert = basiswert_to_displaywert(komma, pZAEHLSP->zdim.dim_idx, pZAEHLSP->verbr_akt[charwert-1], &ret_komma);
					else
						longwert = basiswert_to_displaywert(komma, pZAEHLSP->zdim.dim_idx, pZAEHLSP->verbr_vor[charwert-13], &ret_komma);

					asci4(Visual_Ptr,	longwert, ret_komma);
					memcpy(&Visual_Ptr[14],	&ze_dim_namen[pZAEHLSP->zdim.dim_idx].zdtext,6);		// Dimensionstext
					ret =  komma | KO_LONG | 0x80;
				}				
			}
			else
			{
				if ( charwert > 0 && charwert < 25 )
				{
					longwert	=	bin4(Visual_Ptr, komma, SIGNNO);
					if(charwert < 13)
						pZAEHLSP->verbr_akt[charwert-1]  = (unsigned long)displaywert_to_basiswert(longwert, komma, pZAEHLSP->zdim.dim_idx);
					else
						pZAEHLSP->verbr_vor[charwert-13] = (unsigned long)displaywert_to_basiswert(longwert, komma, pZAEHLSP->zdim.dim_idx);
					
					if ( Mon == charwert )
						pZAEHLSP->vwert	= (unsigned long)displaywert_to_basiswert(longwert, komma, pZAEHLSP->zdim.dim_idx);					
					
					ret =  komma | 0x80;
				}				
			}
			break;			
	
	}
	
#endif	// if ( MBUSANZ


	return ( ret );
}

//------------------------------------------------------------------------------------------------------------
// 2.  Veränderung der ersten Zeile: Aufruf aus Bedien.c , Funktion ZweiZeilen
//------------------------------------------------------------------------------------------------------------
unsigned char user_erste_zeile(char*	Visual_Ptr,	char gnum, char	pnum, unsigned char ucExp_num, char alfunc)
{
	unsigned char ucReturn = 0;
	
	ucMax_expander = 0;
	

// Beispiel Fußbodenheizung mit MM-Modul
//	if(strcmp(Pgruppe[gnum].grkz,"FBH:") == 0) {
//		if(pnum > 0) {
//			// in der Parametergruppe FBH soll das ausgewählte Gerät als Nummer im Gruppennamen erscheinen
//			if(ucExp_num == 0) {
//				ucExp_num = ucBedien_FBH_Nummer;
//			}
//			if(Visual_Ptr != NULL) {
//				Visual_Ptr[2] = ucExp_num + 0x30;
//			}
//		}
//		ucMax_expander = 6;
//		ucReturn = ucMax_expander;
//	}
//

// Beispiel Funkempfang
	#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
	if(ucReturn == 0)							// wenn noch nicht benutzt
	{
		if(strcmp(Pgruppe[gnum].grkz,"RFS;") == 0)
		{
			if(alfunc == EXP_RFS)
			{	// in der Parametergruppe RFS soll das ausgewählte Gerät als Nummer im Gruppennamen erscheinen
				ctoa(&Visual_Ptr[1],bedien_rf_nummer);
				ucMax_expander = 12;
				ucReturn = ucMax_expander;
			}
		}
	}	
	#endif
	
	// Änderung neue Genibus-Implementierung
	#if GENI == 1
		if(ucReturn == 0)							// wenn noch nicht benutzt
		{
			if ( strcmp ( Pgruppe[gnum].grkz,"GEN;") == 0 )
			{
				if ( alfunc == EXP_GEN )	
				{	// in der Parametergruppe GEN soll das ausgewählte Gerät als Nummer im Gruppennamen erscheinen
					if ( uc_genibus_device_nr == 0 )
						uc_genibus_device_nr = 1;
					else if ( uc_genibus_device_nr > GENI_DEVICE_MAX )
						uc_genibus_device_nr = GENI_DEVICE_MAX;
					ctoa ( &Visual_Ptr[1],uc_genibus_device_nr );
					ucMax_expander = GENI_DEVICE_MAX;
					ucReturn = ucMax_expander;
				}
			}
		}
			
		if(ucReturn == 0)							// wenn noch nicht benutzt
		{
			if ( strcmp ( Pgruppe[gnum].grkz,"GBP:") == 0 )
			{
				if ( alfunc == EXP_GBP )
				{	// in der Parametergruppe GBP soll das ausgewählte Gerät als Nummer im Gruppennamen erscheinen
					if ( uc_genibus_device_nr == 0 )
						uc_genibus_device_nr = 1;
					else if ( uc_genibus_device_nr > BUS_PU_MAX )
						uc_genibus_device_nr = BUS_PU_MAX;
					ctoa ( &Visual_Ptr[1],uc_genibus_device_nr );
					ucMax_expander = BUS_PU_MAX;
					ucReturn = ucMax_expander;
				}
			}
		}	
	#endif	// if GENI == 1	

	#if WILO_MODBUS == 1
		if(ucReturn == 0)							// wenn noch nicht benutzt
		{
			if ( strcmp ( Pgruppe[gnum].grkz,"WLP:") == 0 )
			{
				if( alfunc == EXP_WLP)
				{	// in der Parametergruppe WLP soll das ausgewählte Gerät als Nummer im Gruppennamen erscheinen
					ctoa ( &Visual_Ptr[1],cWiloPuCount );
					if( cWiloPuCount < 10 )
						Visual_Ptr[1] = 'L';			// verbesserte Darstellung
					ucMax_expander = BUS_PU_MAX;
					ucReturn = ucMax_expander;
				}
			}
		}	
	#endif // WILO_MODBUS == 1
	
	#if GRUNDFOS_MODBUS == 1
	
	#endif // GRUNDFOS_MODBUS == 1

	#if ( ( IMPLEMENT_S3 & MBUS1_IMPL ) ==  MBUS1_IMPL )
		#if ( MBUSANZ > 0 )
		if(ucReturn == 0)							// wenn noch nicht benutzt
		{
			if(strcmp(Pgruppe[gnum].grkz,"ZMB:") == 0)
			{
				if(alfunc == EXP_ZMB)
				{	// in der Parametergruppe ZMB soll das ausgewählte Gerät als Nummer im Gruppennamen erscheinen
					ctoa(&Visual_Ptr[1], zaehlerNummer);
					ucMax_expander = MBUSANZ;
					ucReturn = ucMax_expander;
				}
			}
		}	
		#endif
	#endif													// ( IMPLEMENT_S3 & MBUS1_IMPL ==  MBUS1_IMPL )

// Modbus
	#if MODBUS_UNI > 0
		if(ucReturn == 0)							// wenn noch nicht benutzt
		{
			ucReturn = user_erste_zeile_modbus(Visual_Ptr,	gnum, pnum, ucExp_num);
		}	
	#endif

	return(ucReturn);
	
}

	
//--------------------------------------------------------------------------------------
// 3.  User-Leittechnikkonvertierungen	zur Abarbeitung von Leittechnikkommnados
//--------------------------------------------------------------------------------------
// Übergabe-Parameter:	Dat_Ptr...Zeiger auf Sende- bzw.Empfangspuffer
//											dae_num...Datenelement-Nummer
//											rw........Lesen(KONVRD) oder Schreiben(KONVWR)
// Return-Parameter:		anz.......Anzahl der gelesenen oder geschriebenen Bytes
//																anz = 0 : Fehler
char UserLzKonv ( char* Dat_Ptr, char* pWert, char konv, char attr, char rw, char exp_num )
{
	
	char  anz = 0;		// Vorbelegung Fehler
	MbusDynam *pZAEHLMB;
	MbusStandard *pMbusPara;

	if ( rw ==	KONVRD )														// lesen
	{
		switch ( konv )
		{
			case MBUS_WMENG_FORM:
//				pZAEHLMB = (MbusDynam*)*((long*)pWert);	
				pZAEHLMB = (MbusDynam*)((long*)pWert);	
				memcpy ( Dat_Ptr, &pZAEHLMB->wmeng, 4 );
				anz = 4;
				break;
		
			case MBUS_VOL_FORM:
//				pZAEHLMB = (MbusDynam*)*((long*)pWert);	
				pZAEHLMB = (MbusDynam*)((long*)pWert);	
				memcpy ( Dat_Ptr, &pZAEHLMB->volume, 4 );
				anz = 4;
				break;

			case MBUS_TYP_FORM:
//				pMbusPara = (MbusStandard*)*((char*)pWert);	
				pMbusPara = (MbusStandard*)((char*)pWert);	
				memcpy ( Dat_Ptr, &pMbusPara->Typ, 1 );
				Dat_Ptr[1] = 0;						// Integer Erweiterung
				anz = 2;
				break;

			case MBUS_S_LONG:
//				pZAEHLMB = (MbusDynam*)((long*)pWert);	
				memcpy ( Dat_Ptr, pWert, 4 );
				anz = 4;
				break;
		}
	}
	
	return ( anz );				

}


//------------------------------------------------------------------------------
// vom System nach User ausgelagerte Konvertierungen
//------------------------------------------------------------------------------

/***** ulsch : Gradtagszahlen *****/
/* Konvertierung 1 Byte (< 100) zu 2 Byte ASCII in Dezimaldarstellung */
/* speziell fuer Expansion von Parametern */
void ctoa_exp ( char *buf, char ch )
{
	*buf++ = '<';	
	strcpy ( buf, &monName[ch][0] );
	*( buf + 3 ) = '>';
}

char HzgGrd_Konvert(char* Visual_Ptr, char *wert, char konv, char attr, char rw)
{
	char	ret	=	TRUE;

#if HEIZGRD == 1
	char charwert;
	UINT intwert;

	switch(konv)
	{
		case EXP_HGT:												// Monat, unsigned char
			if ( rw ==	KONVRD )							// lesen
			{	
				charwert = wert[0];
				asci1 ( Visual_Ptr,	charwert,	attr,	SIGNNO );
			}
			else															// schreiben
			{	
				intwert	=	bin2 ( Visual_Ptr, attr, SIGNNO );
				if ( intwert == 0 || intwert > 12 )
					ret = FALSE;
				else
				{
					wert[0] = (char)intwert;
					HgtAnzeigen ( (char)intwert );
				}					
			}
			break;	
			
		case EXP_HGT_CHAR:									// unsigned	char Format
			if ( rw ==	KONVRD )							// lesen
			{	
				asci1 ( Visual_Ptr,	*wert,	attr,	SIGNNO );
				ctoa_exp ( Visual_Ptr+14,	monHzGrdAnz );				
			}					
			break;
			

		case EXP_HGT_INT:										// unsigned	int	Format
			if ( rw ==	KONVRD )							// lesen
			{
				asci2 ( Visual_Ptr,	*(UINT *)wert, attr, SIGNNO );
				ctoa_exp ( Visual_Ptr+14,	monHzGrdAnz );
			}
			break;
	
	}
#endif	
	return(ret);
	
}

//--------------------------------------------------------------------------------------
// Veränderung der ersten Zeile Parametertext: Aufruf aus Bedien.c , Funktion ZweiZeilen
//--------------------------------------------------------------------------------------
void usertext_erste_zeile(char*	Visual_Ptr,	char gnum, char	pnum)
{
//	char idx;
//	char text[16] = {0};
//	
//	if(strcmp(Pgruppe[gnum].grkz,"ANL:") == 0)
//	{
//		if(strcmp(Pgruppe[gnum].Param[pnum].num, "*96:") == 0) // ***AnFre 30.05.2013 'EXTERNE SSM  '
//		{
//			strcpy(text, extAlarmText);
//			if(text[0] != 0)
//				strcpy(&Visual_Ptr[5],  text);
//		}
//	}		
//	if(strcmp(Pgruppe[gnum].grkz,"QSM:") == 0)
//	{
//		if(pnum == 9) // ***AnFre 30.05.2013 'EXTERNE SSM  '
//		{
//			strcpy(text, extAlarmText);
//			if(text[0] != 0)
//				strcpy(&Visual_Ptr[5],  text);
//		}
//	}		
}


void asci4s ( char* Visual_Ptr, long wert, char komma )
{
	char *buf  = &Visual_Ptr[6];
	char ziffer, vorz, i;
	long subwert = 1000000;
	long maxwert = 10000000;
	char stellen = 6;
	char overfl = 0;


	vorz = ' ';
	/* Test auf negativen Wert */
	if ( wert & 0x80000000 )					// negativ
	{	
		wert = ( 0xFFFFFFFF - wert ) + 1;
		vorz = '-';
		if ( wert > 999999 )
		{
			wert = 999999;								// größte darstellbare neg. Zahl
			overfl = 1; 
		}
	}
	else
	{
		if ( wert > 9999999 )
		{
			wert = 9999999;								// größte darstellbare pos. Zahl
			overfl = 1; 
		}		
	}

	for ( i = 0;	i	<	stellen; i++ ) 
	{	
		ziffer =	0;
		while ( wert >= subwert )
		{	
			wert	-=	subwert;
			ziffer +=	1;
		};
		*buf++	=	(char)( ziffer + 0x30 );
		subwert = subwert / 10;
	}
	*buf++	=	(char)( (wert	%	10)	+	0x30 );

	set_vornul ( Visual_Ptr,	komma, vorz, 7, overfl );

}

		