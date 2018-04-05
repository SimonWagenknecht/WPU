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
	
	// Handbetrieb
	if (wps[WP1].WPU_Freigabe_Haut > 0)
		{
			DA_UNI[U1]->wert = wps[WP1].WPU_Freigabe_Hand_stellen;
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
															
											// Steuerung der Freiagbe und des Sollwertes in den jeweilgen Betriebszuständen 
											switch(Betriebszustand)
											{
												// Betriebszustand Frostschutz
												case 1:
													DA_UNI[U1]->wert = 1;
													break;
												// Betriebszustand Quellenschutz
												case 2:
													DA_UNI[U1]->wert = 0;
													break;
												// Betriebszustand Speicherladung
												case 3:
													DA_UNI[U1]->wert = 1;
													break;
												// Kein Betriebszustand aktiv	
												default:
													DA_UNI[U1]->wert = 0;
													break; 		
											}

			// Ende Automatik 	 
			}
}