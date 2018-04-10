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
	
	int Sollwert;			// Benutzter Sollwert (eingehender Sollwert: AE oder DM)
	
	// Betriebszustände
	 int Betriebszustand=2; // aktueller Betriebszustand
	 
	// Für den Satrt der Mindestlaufzeit und der Sperrzeit
	static char neustart = 1;		 	// Programmstart 
	static char sch_current = 0; 	// aktueller Zustand des Impuls
	static char sch_recent = 0;  	// letzter Zustand des Impuls	
				 char sch_Flanke	= 2;	// positive (=1) oder negative Flanke (=0) oder keine Zustandsänderung (=2)		
	
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
				// Sollwertübergabe
				Sollwert = TMANF[0]->awert;
				
				// Betriebszustand Quellenbegrenzung (TVQ > Minimale Quellentemperartur)
					
					// Status der Fühler überprüfen
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
										wpd[WP1].intT_Quellentemperatur_aktiv = 990; // Der Betriebszustand tritt außer Kraft
				 					}
				 					
				 					// Quellenschutz prüfen
				 					if (wpd[WP1].intT_Quellentemperatur_aktiv < wps[WP1].intPa_Quellentemperaturminimum)
				 						{
				 							wpd[WP1].Status_Quellenschutz = 1;				// Quellenschtuz aktivieren
				 						}
				 						else if (wpd[WP1].intT_Quellentemperatur_aktiv >= wps[WP1].intPa_Quellentemperaturminimum + 30)
				 							{
				 								wpd[WP1].Status_Quellenschutz = 0;			// Quellenschutz deaktivieren
				 							}
				 							
				// Betriebszustand Frostschtuz (TSPU > Minimale Speichertemperartur)
					
					// Status der Fühler überprüfen
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
										wpd[WP1].intT_Speichertemperatur_aktiv = 990; // Der Betriebszustand tritt außer Kraft
				 					}
				 					
				 					// Frostschutz prüfen
				 					if (wpd[WP1].intT_Speichertemperatur_aktiv < wps[WP1].intPa_Speicherminimum)
				 						{
				 							wpd[WP1].Status_Frostschutz = 1;				// Frostschtuz aktivieren
				 						}
				 						else if (wpd[WP1].intT_Speichertemperatur_aktiv >= wps[WP1].intPa_Speicherminimum + 50)
				 							{
				 								wpd[WP1].Status_Frostschutz = 0;			// Frostschutz deaktivieren
				 							}
				 					
				// Betriebszustand Speicherladeprinzip (Speicherladung)
				
						// 1. Fall: Beide Speicherfühler funktionieren
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
													if (Sollwert > 200 && TSPo_WP[WP1]->messw <= wpd[WP1].intT_Ladebginn && wpd[WP1].Status_Quellenschutz == 0)
														{
															wpd[WP1].Status_Speicherladung = 1;
														}
													// Ausschalten	
													else if (Sollwert <= 200 || TSPo_WP[WP1]->messw >= wpd[WP1].intT_Ladeende || wpd[WP1].Status_Quellenschutz > 0)
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
											
											// Auswertung der Betriebszustände
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
															
											// Bestimmung der Freiagbe und des Sollwertes in den jeweilgen Betriebszuständen ohne Counter 
											switch(Betriebszustand)
											{
												// Betriebszustand Frostschutz
												case 1:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 1;
													break;
												// Betriebszustand Quellenschutz
												case 2:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 0;
													break;
												// Betriebszustand Speicherladung
												case 3:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 1;
													break;
												// Kein Betriebszustand aktiv	
												default:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 0;
													break; 		
											}
						// Mindestlauf - und Sperrzeit
						// Bestimmung der Flanke
									if (neustart == 0)
										{
											
										if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0) // nur wenn, Counter steht
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
																	sch_Flanke = 2; // keine Zustandsänderung bzw keine Flanke
																}
											}
	
										}	
										else if (neustart == 1)
												{
													neustart = 0;
												}		
						 
						// Mindestlaufzeit der WPU ohne Retriggern
								if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0) 
										{
											if (sch_Flanke == 1)
															{
																wpd[WP1].Mindestlaufzeit_Cnt = wps[WP1].chPa_Mindestlaufzeit_min * 60;  // Start
															}	
										}
											else if ( wpd[WP1].Sperrzeit_Cnt == 0) 
												{
														--wpd[WP1].Mindestlaufzeit_Cnt; // Counter läuft	
												}				
											
						//	Sperrzeit der WPU ohne Retriggern
								if ( wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Mindestlaufzeit_Cnt == 0) 
										{ 
													if (sch_Flanke == 0)  // negative Flanke
														{
															wpd[WP1].Sperrzeit_Cnt = wps[WP1].chPa_Sperrzeit_min * 60;  // Start 
														}		
										}
										else if ( wpd[WP1].Mindestlaufzeit_Cnt == 0) 
											{
														--wpd[WP1].Sperrzeit_Cnt; // Counter läuft	
											}	
											
						// Steuerung der Freigabe
										if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0) 
											{
												DA_UNI[0]->wert = wpd[WP1].Status_WPU_Freigabe_oZeit;
											}		
																			
			// Ende Automatik 	 
			}
 // Ende Programm			
}