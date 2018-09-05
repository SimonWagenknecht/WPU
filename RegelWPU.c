/*	Task:					Steuer WPU		*/
/*	WPU-Steuerung der Ausg�nge	AA und DA					*/

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"

//

///*---------------------- W�rmepumpensteuerung -----------------------------------*/
//// Takt: 1 s

void RegelWPU(void) 

{
	// F�r den Satrt der Mindestlaufzeit und der Sperrzeit
	static char NEUSTART = 1;		 	// Genereller Programmstart 
	// PID
	float fl_ei = 0;  		// Regelabweichung
	float fl_dy_rel = 0;	// Berechnung der relativen Stellgr��e
	float fl_Y_rel_beg = 0; 	
	float fl_Y_rel_min = 0; 
	
			
	if (NEUSTART == 1)	// Beim Neustart der Software wird beim ersten durchlauf keine Funkiton ausgef�hrt. Hintergrund: Das verhindert das verfr�hte Schalten von Ausg�ngen.
	{
		NEUSTART = 0;
	}	
	else		
	{	
			
		// Handbetrieb
			if (wps[WP1].chpa_rv_Hau > 0)
				{
					AA_UNI[U1]->awert = wps[WP1].ipa_rv_stellung * 10;
  	
				}
		// Automatikbetrieb	
			else
				{
					if (DA_UNI[0]->wert > 0) // || sp�ter oder die Betriebsmeldung (W�rmepumpe ist gefordert)
						{
							// Sollwertberechnung f�r RV QU
							wpd[0].tvsb = wps[0].iPa_Vol_ist ;
					// D�mpfung des berechneten Sollwertes: tvsb --> ged�mpfte tsol
							if( wpd[0].regstart == 0) 	// nach Reset oder bei gleit. WW-Vorrang
							{	 wpd[0].tsol			= wpd[0].tvsb;												// keine Filterung
								 wpd[0].fl_tsol	= (float)wpd[0].tsol / 10;
								 wpd[0].regstart	= 1;
							}	 
							else
							{
								wpd[0].fl_tsol = g_filter(wpd[0].fl_tsol, wpd[0].tvsb, wps[0].Ts, wps[0].Fzk);
								wpd[0].tsol 	  = (int)(wpd[0].fl_tsol * 10);					// f�r Anzeige
							}
							
							// Regelabweichung Soll - Ist
							fl_ei	= wpd[0].fl_tsol - wmengCtr[2].flow_h / 100;
							
							// -------------- PID-Regelalgorithmus ---------------------------------
							// Berechnung der relativen Stellgr��en
							fl_dy_rel = Dy_rel ( wps[0].Kp, wps[0].Kd, wps[0].Ts, wps[0].Tn, 
																			fl_ei, wpd[0].fl_ei1, wpd[0].fl_ei2 );
 	
							wpd[0].fl_y_rel += fl_dy_rel;
							
							wpd[0].dy_rel	= (int)(fl_dy_rel * 1000);					// f�r debug
 	
							//---------------------------------------------------------------------
							// Regelabweichungen merken
							wpd[0].fl_ei2 = wpd[0].fl_ei1;
							wpd[0].fl_ei1 = fl_ei;						
							// Parameterumwandlung float to int
							wpd[0].ei = (int)(fl_ei * 100);											// [0,1%] zur Anzeige
							                            
								// Ausgabe an 0-10V Ventil                                               
	                                                                         
								// Begrenzung der PID-Stellgr��e (anti windup) auf Kp * ei (gleitend) ode
								wpd[0].fl_y_rel = anti_windup( wpd[0].fl_y_rel, wps[0].Kp, wps[0].Wup,  fl_ei );		// in Funktion.c 
								wpd[0].si_y_rel = (int)(wpd[0].fl_y_rel * 10);		// debug                 
								                                                                         
								// Abfrage Ventilstellungsbegrenzungen			                             
								if(wps[0].Y_rel_beg > 0)													// Ventil�ffnungsbeginn g
								{                                                                        
									if(fl_dy_rel > 1)															// nur bei positiver �nde
									{	if(wpd[0].fl_y_rel < fl_Y_rel_beg)						// und Wert kleiner als V
											 wpd[0].fl_y_rel = fl_Y_rel_beg;						// dann Setzen auf Ventil
									}                                                                      
								}	                                                                       
								if(wps[0].Y_rel_min > 0)													// Minimalbegrenzung gese
								{			                                                                   
									if(wpd[0].fl_y_rel  < fl_Y_rel_min)						// Wert kleiner als Minim
										 wpd[0].fl_y_rel  = fl_Y_rel_min;						// dann setzen auf Minima
								}
								
								if(wpd[0].fl_y_rel > (wps[0].Y_rel_max / 10))													// Maximalbegrenzung gese
								{			                                                                   
										 wpd[0].fl_y_rel  = wps[0].Y_rel_max / 10;						// dann setzen auf Max
								}			                                                                     
									                                                                       
								// Ausgabe der PID-Stellgr��e als Stellsignal an den Systemausgang mit Be
								wpd[0].y_rel = y_stell(wpd[0].fl_y_rel);					// in Funktion.c         
 							                                                                       
								AA_UNI[U1]->awert = wpd[0].y_rel;									// Ausgabe an Stellventil 
								
								
						} // Ende Regelung Rv VOl
						else	// RV Sol nicht im Betrieb
							{
										AA_UNI[U1]->awert = 0;		// Ausgabe an Stellventil
										wpd[0].y_rel = 0;									 
										wpd[0].fl_y_rel = 0; 
										wpd[0].fl_ei2 = 0;
										wpd[0].fl_ei1 = 0; 
										wpd[0].ei = 0;
							}								
									 	 
				}	// Ende Automatik					
												
	}// ENDE: NEUSTART 		
}


//-------------------------------------------------------------------------------------------------
//										Unterprogramme
//-------------------------------------------------------------------------------------------------

//*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Ermittlung der Sarts bzgl BM und Freigabe							 								                                 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

