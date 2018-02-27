/*	Allgemeine Funktionen	*/

#include "struct.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#include "constext.h"


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Funktion: Zeitdiskretes Tiefpassfilter									*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* IN:	(float)	 Ausgangswert(A)
				(int)*10 Eingangswert(E), Tastzeit(t), Zeitkonstante(T)
	OUT:	(float)  neuer Ausgangswert
	Formel: A = A * e^(-t/T) + E * (1 - e^(-t/T)) 
*/	


//- Ohne e-Funktion
//- A = A * (1 - t/T)  +  E * t/T
//- Programmlaufzeit: 0,5ms
float g_filter(float alt, int neu, int ts, int zk)		
{
	float A = 0;
	float q = 0;
	float E = 0;

	E = (float)neu / 10;									//- neuer Eingangswert	
	
	if( (zk == 0) || (zk < ts) )
		A = E;
	else
	{
		q = (float)ts / (float)zk;
		A = (1 - q) * alt + q * E;
	}
	return(A);
}			


/*****					PID-Regler											*****/
/***** Berechnung der Stellgroessenveraenderung *****/
float Dy_rel ( int  kp, int kd, int ts, UINT tn, float ei, float ei1, float ei2 )	
{
	float dterm = 0;

	if ( tn > 0 && ( ts < tn ) )
	{	
		if ( kd > 0 )												// Abfrage wegen Sparen von Rechenzeit
			dterm = ((float)kd/100) * ( ei - 2 * ei1 + ei2 );
																		// Stellgrößenveränderung 	(in der urspr. Form Rechenzeit 330µs von Achim gemessen)	
		return ( ((float)kp/100) * ( ei - ei1 + ( ((float)ts ) * ei1 / ((float)tn ) ) + dterm ) );	
	}
	else
//#####ulsch: 25.08.06 nach Diskussion ( Siefke, Schinske, D.Rothenhagen, U.Schmidt )
//-		return ( ((float)kp/100) * ei );	// P-Regler
		return ( ((float)kp/100) * ( ei -ei1 ) );	// P-Regler -> keine Änderung der Stellgröße bei konstanter Regelabweichung
}		
/******************************************************************************************************/	
// Funktion: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
//           Maximalwert über 100% und unter 0% wird aus dem Produkt Kp * ei ermittelt (gleitend)
// Input:    float Stellgröße
//           int   Verstärkung Kp [0,01] %
//           int   Festwert    Wup [0,1] %  (0 = gleitend)
//           float Regelabweichung ei
// Output:   float Stellgröße bzw. begrenzte Stellgröße bei Werten über 100% oder unter 0% 
float anti_windup(float fl_y_rel, int Kp, int Wup, float fl_ei)
{
	float fl_y_rel_max;
	
	if(fl_ei < 0)
		fl_ei *= -1;																					// Absolutwert wird benötigt																				
	
	if(Wup == 0)																						// gleitend
		fl_y_rel_max = (float)Kp / 100 * fl_ei;
	else
		fl_y_rel_max = (float)Wup / 10;												// fester Maximalwert
			
	
	if(fl_y_rel > 0)																				// Stellgröße positiv
	{
		if(fl_y_rel > fl_y_rel_max + 100)											
			fl_y_rel = fl_y_rel_max + 100;											// Begrenzung auf positiven Maximalwert über 100%
	}
	
	if(fl_y_rel < 0)																				// Stellgröße negativ
	{
		if(fl_y_rel < fl_y_rel_max * (-1) )											
			fl_y_rel = fl_y_rel_max * (-1);											// Begrenzung auf negativen Maximalwert unter 0%
	}
	
	return(fl_y_rel);
}

// Funktion: Ausgabe der PID-Stellgröße als Stellsignal an den Systemausgang mit Begrenzung auf 0-100%
int y_stell(float fl_y_rel)
{
	int y_rel;
	
	y_rel = (int)(fl_y_rel * 10);										// PID-Stellgröße float to int wird zum Stellsignal

	// Begrenzung des Stellsignals auf 0% bzw. 100%
	if(y_rel > 1000)
		y_rel = 1000;
	if(y_rel < 0)
		y_rel = 0;
		
	return(y_rel);
}		
/******************************************************************************************************/	
	

/***** Daten eines Monats ( Heizgradtage ) zur Anzeige aus EEPROM in RAM kopieren *****/
void HgtAnzeigen ( char mon )
{
#if HEIZGRD == 1
	if ( mon > 0 && mon <= 12 )
		bicbus ( EEPADR, (char *)&sMonHzGrdAnz, HEIZGRADTAGE_ADR + ( mon-1 ) * sizeof(monHzGrd), sizeof(monHzGrd), BICRD );	
	else
	{
		if ( mon > 12 )
			mon = 0;
		sMonHzGrdAnz.hzGrdTage = 0;	sMonHzGrdAnz.hzTage = 0; sMonHzGrdAnz.hzTageBew = 0;
		sMonHzGrdAnz.hzGrdTageVj = 0;	sMonHzGrdAnz.hzTageVj = 0; sMonHzGrdAnz.hzTageBewVj = 0;			
	}		
#endif
}
		
int Gerade_YvonX ( int x, int x1, int y1, int x2, int y2 )
{
	// y = mx + n;
	// m = (y2-y1) / (x2-x1);			n = (y1*x2 - y2*x1) / (x2-x1)
	return((int)( ( (float)(y2 - y1) * (float)x + ( (float)y1 * (float)x2 - (float)y2 * (float)x1 ) ) / (float)(x2 - x1) ) );

}

// Ermittle von einem Alarm den Tabellenindex (Alarmtabelle)
// return: 0...kein Eintrag in der Alarmtabelle
char get_alspeicher_idx(char *p_wert)
{
	char idx = 0;
	char i, gnum, pnum, konv, attr, alclass, alfunc;
	char    *pADR;
	UINT dae;
	
	for(i = 0; i < SETAB_TABL; i++)		// Die Alarmtabelle hat die gleiche Länge und Ordnung wie die Sekundär-Ereignistabelle
	{
		// Alarmquelle untersuchen
		dae  = seku_evtab[i].dae;		 
		gnum = KomTab[dae].gnum;							// Gruppennummer
		pnum = KomTab[dae].pnum;							// Parameternummer
		pADR = get_parli_info(gnum, pnum, &konv, &attr, &alclass, &alfunc, 0);
		
		if(pADR == p_wert)
		{	idx = i;
			break;
		}	
	}
	
	return(idx);
	
}	

//------------------------------------------				
// Funktion: Mittelung von 2 Temperaturen
// Aufrufbeispiel:			if( TempMitt(TI[LU1], TP_UNI[U1], &pHkd->ti) )
// return: 0...beide Fühler gestört
char TempMitt(mwsp* pT1, mwsp* pT2, mwsp* pTmitt)
{
	char ret = 0;
	int t1, t2, teiler;
	
	t1 = 0;
	t2 = 0;
	teiler = 0;
	if(pT1->stat == 0)
	{	t1 = pT1->messw;
		teiler++;
	}	

	if(pT2->stat == 0)
	{	t2 = pT2->messw;
		teiler++;
	}	

	if(teiler > 0)
	{	pTmitt->messw = (t1 + t2) / teiler;
		pTmitt->stat = 0;
		ret = 1;
	}
	else
		pTmitt->stat = UEBERL;
	
	return(ret);
}

//------------------------------------------				
// Lese aktuellen Alarm
char get_alstatus(char *p_wert)
{
	char idx;
	
	idx = get_alspeicher_idx(p_wert);
	return(alarmtab[idx].alstatus);
}	

// Lese gespeicherten Alarm
char get_alspeicher(char *p_wert)
{
	char idx;
	
	idx = get_alspeicher_idx(p_wert);
	return(alarmtab[idx].alspeicher);
}	
	
// Überschreibe gespeicherten Alarm
void set_alspeicher(char *p_wert, char ea)
{
	char idx;
	
	idx = get_alspeicher_idx(p_wert);
	if(idx > 0)
		alarmtab[idx].alspeicher = ea;
}	

// Überschreibe Wartezeit
void set_alwaitlimit(char *p_wert, char wait)
{
	char idx;
	
	idx = get_alspeicher_idx(p_wert);
	if(idx > 0)
		alarmtab[idx].Waitlimit = wait;
}	


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 		Heizkreis abschalten																			 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  IN: hk.....Heizkreis 
//			stemp..1 = Starttemperatur eintragen
void hk_abschalten(char hk, char stemp)
{
	if(stemp == 1)															// Startemperatur eintragen ?
	{	hkd[hk].tsol			= hks[hk].Tsolu;				// Start-Sollwert Heizung	
		hkd[hk].fl_tsol	= (float)hkd[hk].tsol / 10;
	}
	
	hkd[hk].tvsb 		= 0;					// Berechneter Vorlauf
	hkd[hk].ei			= 0;
	hkd[hk].fl_ei1	= 0;
	hkd[hk].y_rel		= 0;
	hkd[hk].fl_y_rel = 0;
	if ( hks[hk].Haut == FALSE )
	{
		RAUF[hk]->wert	= 0;
		RZU[hk]->wert		= 255;				// Regelventil ständig zufahren
		RVENT[hk]->awert = hkd[hk].y_rel;		// Ausgabe an Stellventil
	}
}



UINT DayOfYear ( datum date )
{	
	static char DaysPerMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	UINT day;
	char i;
	
	for ( i = 0, day = 0; i < date.monat-1; i++ )
		day += DaysPerMon[i];
	if ( ( date.jahr % 4 ) == 0 && date.monat > 2 )
		++day;															// Schaltjahr, Monat > Februar
	day += date.tag;
	return ( day );		
}


//------------------------------------------				
void T_E_S_T___________P_U_N_K_T(void)
{
};	
/*
T_E_S_T___________P_U_N_K_T();
*/
//------------------------------------------				
