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

void SteuerWPU(void) 

{
	
	int Sollwert;						// Sollwert in Bearbeitung
	//static int maxAnford_recent;		// ausgehender Sollwert letzter Programmaufruf
	
	// Betriebszust�nde
	 int Betriebszustand=2; // aktueller Betriebszustand
	 
	// F�r den Satrt der Mindestlaufzeit und der Sperrzeit
	static char NEUSTART = 1;		 	// Genereller Programmstart 
	static char neustart = 1;		 	// Ablauf nach Programmstart 
	static char sch_current = 0; 	// aktueller Zustand des Impuls
	static char sch_recent = 0;  	// letzter Zustand des Impuls	
	static char sch_Flanke	= 2;	// positive (=1) oder negative Flanke (=0) oder keine Zustands�nderung (=2)		
	static char BM_recent = 0;		// letzter Zustand der Betriebsmeldung			 
	static char WPU_Freigabe__recent = 0;		// letzter Zustand der WPU-Freigabe	
	
			// Sollwert�bergabe
				Sollwert = wpd[WP1].Eingehender_Sollwert; // Eingehender_Sollwert kommt aus Steuer
	
	
	// Handbetrieb
		if (wps[WP1].WPU_Freigabe_Haut > 0)
			{
				DA_UNI[U1]->wert = wps[WP1].WPU_Freigabe_Hand_stellen;
			
				// Counter nullen
				wpd[WP1].Mindestlaufzeit_Cnt = 0;
			}
	// Automatikbetrieb	
		else
			{
				
			if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) // Nur wenn die Counter nicht aktiv sind, werden die Betriebszust�nde gepr�ft
			  {				
				
				// Betriebszustand Quellenbegrenzung (TVQ > Minimale Quellentemperartur)
					
					// Status der F�hler �berpr�fen
						if (TVQ_WP[WP1]->stat == 0)
							{
							wpd[WP1].intT_Quellentemperatur_aktiv = TVQ_WP[WP1]->messw; 
				 			}
				 			else if (TRQ_WP[WP1]->stat == 0)
				 				{
									wpd[WP1].intT_Quellentemperatur_aktiv = TRQ_WP[WP1]->messw; 
				 				}
				 				else
				 					{
										wpd[WP1].intT_Quellentemperatur_aktiv = 990; // Der Betriebszustand tritt au�er Kraft
				 					}
				 					
				 					// Quellenschutz pr�fen
				 					if (wpd[WP1].intT_Quellentemperatur_aktiv < wps[WP1].intPa_Quellentemperaturminimum)
				 						{
				 							wpd[WP1].Status_Quellenschutz = 1;				// Quellenschtuz aktivieren
				 						}
				 						else if (wpd[WP1].intT_Quellentemperatur_aktiv >= wps[WP1].intPa_Quellentemperaturminimum + 30)
				 							{
				 								wpd[WP1].Status_Quellenschutz = 0;			// Quellenschutz deaktivieren
				 							}
				 							
				// Betriebszustand Frostschtuz (TSPU > Minimale Speichertemperartur)
					
					// Status der F�hler �berpr�fen
						if (TSPu_WP[WP1]->stat == 0)
							{
							wpd[WP1].intT_Speichertemperatur_aktiv = TSPu_WP[WP1]->messw; 
				 			}
				 			else if (TSPo_WP[WP1]->stat == 0)
				 				{
									wpd[WP1].intT_Speichertemperatur_aktiv = TSPo_WP[WP1]->messw; 
				 				}
				 				else
				 					{
										wpd[WP1].intT_Speichertemperatur_aktiv = 990; // Der Betriebszustand tritt au�er Kraft
				 					}
				 					
				 					// Frostschutz pr�fen
				 					if (wpd[WP1].intT_Speichertemperatur_aktiv < wps[WP1].intPa_Speicherminimum)
				 						{
				 							wpd[WP1].Status_Frostschutz = 1;				// Frostschtuz aktivieren
				 						}
				 						else if (wpd[WP1].intT_Speichertemperatur_aktiv >= wps[WP1].intPa_Speicherminimum + 50)
				 							{
				 								wpd[WP1].Status_Frostschutz = 0;			// Frostschutz deaktivieren
				 							}
				 					
				// Betriebszustand Speicherladeprinzip (Speicherladung)
				
						// 1. Fall: Beide Speicherf�hler funktionieren
							if (TSPo_WP[WP1]->stat == 0 && TSPu_WP[WP1]->stat == 0)
								{
									// Speicherladungladebeginn
									wpd[WP1].intT_Ladebginn = Sollwert + wps[WP1].intPa_T_Speicherladung_on; 
									// Speicherladeende
									wpd[WP1].intT_Ladeende = Sollwert + wps[WP1].intPa_T_Speicherladung_off;
									
									// Einschalten
									if (Sollwert > 200 && TSPo_WP[WP1]->messw <= wpd[WP1].intT_Ladebginn && wpd[WP1].Status_Quellenschutz == 0)
										{
											wpd[WP1].Status_Speicherladung = 1;
										}
									// Ausschalten	
										else if (Sollwert <= 200 || TSPu_WP[WP1]->messw >= wpd[WP1].intT_Ladeende || wpd[WP1].Status_Quellenschutz > 0)
													{
														wpd[WP1].Status_Speicherladung = 0;
													}
								}
						// 2. Fall: Beide Speichertemperaturen sind defekt  		
								else if ( TSPo_WP[WP1]->stat != 0 && TSPu_WP[WP1]->stat != 0) 
									{
									// Speicherladungladebeginn
									wpd[WP1].intT_Ladebginn = 990; 
									// Speicherladeende
									wpd[WP1].intT_Ladeende = 990;
									
									wpd[WP1].Status_Speicherladung = 0;
									}
						// 3. Fall: TSPo oder TSPu funktioniert nicht			
										else
											{
												// 3.1: Nur TSPo funktioniert
												if (TSPo_WP[WP1]->stat == 0)
													{
														// Speicherladungladebeginn
															wpd[WP1].intT_Ladebginn = Sollwert + wps[WP1].intPa_T_Speicherladung_on; 
														// Speicherladeende
															wpd[WP1].intT_Ladeende = wpd[WP1].intT_Ladebginn + 50;	
													// Einschalten
													if (Sollwert > 200 && TSPo_WP[WP1]->messw <= wpd[WP1].intT_Ladebginn && wpd[WP1].Status_Quellenschutz == 0 && wpd[WP1].Status_Frostschutz == 0)
														{
															wpd[WP1].Status_Speicherladung = 1;
														}
													// Ausschalten	
													else if (Sollwert <= 200 || TSPo_WP[WP1]->messw >= wpd[WP1].intT_Ladeende || wpd[WP1].Status_Quellenschutz > 0 || wpd[WP1].Status_Frostschutz > 0)
														{
															wpd[WP1].Status_Speicherladung = 0;
														}			
													}
													else if (TSPu_WP[WP1]->stat == 0)
														{
												// 3.2: Nur TSPu funktioniert
															// Speicherladeende
																wpd[WP1].intT_Ladeende = Sollwert + wps[WP1].intPa_T_Speicherladung_off;	
															// Speicherladungladebeginn
																wpd[WP1].intT_Ladebginn = wpd[WP1].intT_Ladeende - 50; 

															// Einschalten
															if (Sollwert > 200 && TSPu_WP[WP1]->messw <= wpd[WP1].intT_Ladebginn && wpd[WP1].Status_Quellenschutz == 0)
																{
																	wpd[WP1].Status_Speicherladung = 1;
																}
															// Ausschalten	
																else if (Sollwert <= 200 || TSPu_WP[WP1]->messw >= wpd[WP1].intT_Ladeende || wpd[WP1].Status_Quellenschutz > 0)
																	{
																		wpd[WP1].Status_Speicherladung = 0;
																	}	
														}
											// Ende Betriebszustand Speicherladung 3. Fall	
											}
											
											// Auswertung der Betriebszust�nde
											if (wpd[WP1].Status_Frostschutz == 1)
													{	
														Betriebszustand = 1;
													}
													else if (wpd[WP1].Status_Quellenschutz == 1)
														{
														Betriebszustand = 2;
														}
														else if (wpd[WP1].Status_Speicherladung == 1)
															{	
															Betriebszustand = 3;
															}
															else
															{	
																Betriebszustand = 0;
															}
															
											// Bestimmung der Freiagbe und des Sollwertes in den jeweilgen Betriebszust�nden ohne Counter 
											switch(Betriebszustand)
											{
												// Betriebszustand Frostschutz
												case 1:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 1;
													Sollwert = wps[WP1].intPa_Speicherminimum + 70;
													break;
												// Betriebszustand Quellenschutz
												case 2:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 0;
													Sollwert = 0;
													break;
												// Betriebszustand Speicherladung
												case 3:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 1;
													Sollwert = wpd[WP1].intT_Ladeende;
													break;
												// Kein Betriebszustand aktiv	
												default:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 0;
													Sollwert = 0;
													break; 		
											}				
											
						// Mindestlauf - und Sperrzeit
						// Bestimmung der Flanke
									if (neustart == 0)
										{
											
						 					sch_current = wpd[WP1].Status_WPU_Freigabe_oZeit;  
													if (sch_recent == 0 && sch_current ==0)  // negative Flanke
														{
															sch_recent = 1;
															sch_Flanke = 0;
														}	
														else if (sch_recent == 1 && sch_current ==1) // positive Flanke
															{
																sch_recent = 0;
																sch_Flanke = 1;
															}
															else
																{
																	sch_Flanke = 2; // keine Zustands�nderung bzw keine Flanke
																}
	
										}	
										else if (neustart == 1)
												{
													neustart = 0;
												}		
						 
			  } // Ende:  Nur wenn die Counter nicht aktiv sind, werden die Betriebszust�nde gepr�ft
				 
						// Verz�gertes Einschalten der WPU bzw. verz�gerte Freigabe	ohne Retriggern
								if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
										{
											if (sch_Flanke == 1)
															{
																wpd[WP1].Verzoegerungszeit_Cnt = wps[WP1].chPa_Verzoegerung_min * 60;  // Start
															}	
										}
											else if ( wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Mindestlaufzeit_Cnt == 0) 
															{
																--wpd[WP1].Verzoegerungszeit_Cnt; // Counter l�uft																
															}	
						 
						// Mindestlaufzeit der WPU ohne Retriggern
								if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
										{
											if (sch_Flanke == 1)
															{
																wpd[WP1].Mindestlaufzeit_Cnt = wps[WP1].chPa_Mindestlaufzeit_min * 60;  // Start
															}	
										}
											else if ( wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
												{
														--wpd[WP1].Mindestlaufzeit_Cnt; // Counter l�uft	
												}				
											
						//	Sperrzeit der WPU ohne Retriggern
								if ( wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Mindestlaufzeit_Cnt == 0) 
										{ 
											if (sch_Flanke == 0)  // negative Flanke
														{
															wpd[WP1].Sperrzeit_Cnt = wps[WP1].chPa_Sperrzeit_min * 60;  // Start 
														}		
										}
										else if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
											{
														--wpd[WP1].Sperrzeit_Cnt; // Counter l�uft	
											}	
											
						// Steuerung der Freigabe in Abh�ngigkeit der Counter 
										if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
											{
												DA_UNI[0]->wert = wpd[WP1].Status_WPU_Freigabe_oZeit;
											}
											else if (wpd[WP1].Verzoegerungszeit_Cnt > 0)		
												{
													DA_UNI[0]->wert = 0;
												}
												else if (wpd[WP1].Mindestlaufzeit_Cnt > 0)		
													{
														DA_UNI[0]->wert = 1;
													}
														else if (wpd[WP1].Sperrzeit_Cnt > 0)	
														{
															DA_UNI[0]->wert = 0;
														}
								// Ende Automatik 	 
			}						
											
						// Z�hlung der Starts bzgl Freiagbe im Auto- und Handbetrieb
						if (WPU_Freigabe__recent == 0 && 	DA_UNI[0]->wert == 1	)
							{
								++wpd[WP1].WPU_Starts_Freigabe;
								bicbus ( EEPADR,	(char *)&wpd[WP1].WPU_Starts_Freigabe,	WPUSTA_ADR, 2, BICWR);
							}
							
							WPU_Freigabe__recent = DA_UNI[0]->wert;
							
						// Z�hlung der Starts bzgl Betriebsmeldung
							if (BM_recent == 0 && BM_UNI[U1]->bwert == 1)
								{
									++wpd[WP1].WPU_Starts_BM;
									bicbus ( EEPADR,	(char *)&wpd[WP1].WPU_Starts_BM,	WPUSTA_ADR+2, 2, BICWR);
								}
							BM_recent = BM_UNI[U1]->bwert;
						
						// Laufzeit Anforderung und BM
						// Anforderung
							if ( DA_UNI[0]->wert == 1 )
								{	
									if ( ++wpd[WP1].WPU_Freigabe_Laufzeit_sec >= 3600 )
										{	
											wpd[WP1].WPU_Freigabe_Laufzeit_sec = 0;
											wpd[WP1].WPU_Freigabe_Laufzeit_h++;							// Laufzeit h erh�hen
											bicbus ( EEPADR, (char*)&wpd[WP1].WPU_Freigabe_Laufzeit_h, WPULaufzeit_ADR, 2, BICWR );
										}
								}
						// BM
							if ( BM_UNI[U1]->bwert == 1 )
								{	
									if ( ++wpd[WP1].WPU_BM_Laufzeit_sec >= 3600 )
										{	
											wpd[WP1].WPU_BM_Laufzeit_sec = 0;
											wpd[WP1].WPU_BM_Laufzeit_h++;							// Laufzeit h erh�hen
											bicbus ( EEPADR, (char*)&wpd[WP1].WPU_BM_Laufzeit_h, WPULaufzeit_ADR+2, 2, BICWR );
										}
								}
											


			
											//------ Steuerung SOLLWERT-----------------------------------------------//
											if (wpd[WP1].Sperrzeit_Cnt > 0 )
													{
													maxAnford = 0;
													}
													else if (wpd[WP1].Mindestlaufzeit_Cnt > 0 && Sollwert <= maxAnford)
														 {
														 	;
														 }
														 else
														 	{
														 		maxAnford = Sollwert;	
														 	}
													
										if ( maxAnford < TmanfSkalMin )
											TMANF[0]->awert = 0;
										else if ( maxAnford > TmanfSkalMax )	
											TMANF[0]->awert = TmanfSkalMaxSpg;
										else
											TMANF[0]->awert = Gerade_YvonX ( maxAnford, TmanfSkalMin, TmanfSkalMinSpg, TmanfSkalMax, TmanfSkalMaxSpg );					
										
										
									//--------ENDE Ausgehender Sollwert-----------------------//			
																	
			// -------------------------------- Diagnose ----------------------------------------------------//
			// Analysewerte zum W�rmepumpenbetrieb
			
			// Leistung und Arbeit des letzten WPU-Betriebs
				
}


//-------------------------------------------------------------------------------------------------
//										Unterprogramme
//-------------------------------------------------------------------------------------------------

//*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Ermittlung der Sarts bzgl BM und Freigabe							 								                                 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

