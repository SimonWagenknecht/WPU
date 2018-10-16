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
	
	int Sollwert;						// Sollwert in Bearbeitung
	//static int maxAnford_recent;		// ausgehender Sollwert letzter Programmaufruf
	int T_Unterst;					// Aktiver Fühler für die Funktion Unterstützung
	// Betriebszustände
	 int Betriebszustand=2; // aktueller Betriebszustand
	 
	// Für den Satrt der Mindestlaufzeit und der Sperrzeit
	static char NEUSTART = 1;		 	// Genereller Programmstart 
	static char neustart = 1;		 	// Ablauf nach Programmstart 
	static char sch_current = 0; 	// aktueller Zustand des Impuls
	static char sch_recent = 1;  	// letzter Zustand des Impuls	
	static char sch_Flanke	= 2;	// positive (=1) oder negative Flanke (=0) oder keine Zustandsänderung (=2)		
	static char BM_recent = 0;		// letzter Zustand der Betriebsmeldung			 
	static char WPU_Freigabe__recent = 0;		// letzter Zustand der WPU-Freigabe	
	static char verz_aktiv	= 0;	// Zustandsanzeige: 0 =  AUS und 1 = aktiv
	
			
		if (NEUSTART == 1)	// Beim Neustart der Software wird beim ersten durchlauf keine Funkiton ausgeführt. Hintergrund: Das verhindert das verfrühte Schalten von Ausgängen.
		{
			NEUSTART = 0;
		}	
		else		
		{	
			// Sollwertübergabe
				Sollwert = wpd[WP1].Eingehender_Sollwert; // Eingehender_Sollwert kommt aus Steuer
	
	
	// Parli: Speicherladung
	if (wps[WP1].chPa_Strategie == 1)
		{
		 hid1_dS1=SICHTSERV1;				// Mit Service-Code 1	sichtbare Parameter	(135), wenn Strategie 1 (Speicherladung aktiviert)		
		 hid1_dS2 =UNSICHTB;					// Mit Service-Code 1	sichtbare Parameter	(135), wenn Strategie 2 (Rücklaufregelung aktiviert)
		}
		if (wps[WP1].chPa_Strategie == 2)
			{
		 hid1_dS1=UNSICHTB; 					// Mit Service-Code 1	sichtbare Parameter	(135), wenn Strategie 1 (Speicherladung aktiviert)		
		 hid1_dS2 =SICHTSERV1;					// Mit Service-Code 1	sichtbare Parameter	(135), wenn Strategie 2 (Rücklaufregelung aktiviert)
			}
		
		if (wps[WP1].Sollwert_EA > 0)
			{
			hid1_SOLLW_EA = SICHTSERV1;
			}
			else
				{
					hid1_SOLLW_EA = UNSICHTB;
				}
		
		
	
	// Handbetrieb
		if (wps[WP1].WPU_Freigabe_Haut > 0)
			{
				DA_UNI[U1]->wert = wps[WP1].WPU_Freigabe_Hand_stellen;
			
				// Counter nullen
				wpd[WP1].Mindestlaufzeit_Cnt = 0;
			}	
	// Optional UV TWE
	#if UV_TWE > 0
		if (wps[WP1].chPa_uv_Hau > 0)
			{
				DA_UNI[U3]->wert = wps[WP1].chPa_uv_Hand_stellen;
			
			}
	#endif
	
	// Optional TWE-Anforderung
	#if TWE_ANF > 0
		if (wps[WP1].chPa_Freiagbe_TWE_Hau > 0)
			{
				DA_UNI[U2]->wert = wps[WP1].chPa_Freiagbe_TWE_Hand_stellen;
			
			}
	#endif
	
	// Automatikbetrieb	
		else
			{			
			// Betriebszustände		
			if ( (wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0)// Nur wenn die Counter nicht aktiv sind, werden die Betriebszustände geprüft
						|| (TWE_ANF > 0 && (station1_ea.messw > 0 || wpd[WP1].Mindestlaufzeit_Cnt > 0) && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0)) // Warmwasservorrang
			  {				
				
				if (wpd[WP1].Mindestlaufzeit_Cnt == 0)
				{
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
				 					
				//	Strategie 1 = Speicherladung, 2 = Rücklauftemperatur
					// Betriebszustand Speicherladeprinzip (Speicherladung)
				if (wps[WP1].chPa_Strategie == 1)
				{		
				 if (TWE_ANF > 0 && wpd[WP1].chPa_WPU_TWE > 0)
				 	{
				 		wpd[WP1].Status_Speicherladung = 0;
				 	}
				 	else
				 		{
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
														// Ende Betriebszustand Speicherladung 3.2 Fall	
											}	// Ende Speicherladung 3.1 und 3.2
										}	// ENDE Speicherladung	
									}	// Ende Mindestlaufzeit-bedingung 						
								}// ENDE Strategie --> Speicherladung
								if (wps[WP1].chPa_Strategie == 2)
									// Strategie Rücklauftemperaturregelung
									{
										if (TWE_ANF > 0 && wpd[WP1].chPa_WPU_TWE > 0)
				 								{
				 								wpd[WP1].Status_Speicherladung = 0;
				 								}
				 								else
				 								{
												// 1. Fall: Rücklauffühler funktioniert funktionieren
													if (TR_WP[WP1]->stat == 0)
														{
															// Rücklauf EIN
															wpd[WP1].intT_Ladebginn = Sollwert + wps[WP1].intPa_T_Speicherladung_on; 
															// Rücklauf AUS
															wpd[WP1].intT_Ladeende = Sollwert + wps[WP1].intPa_T_Speicherladung_off;
															
															// Einschalten
															if (Sollwert > 200 && TR_WP[WP1]->messw <= wpd[WP1].intT_Ladebginn && wpd[WP1].Status_Quellenschutz == 0)
																{
																	wpd[WP1].Status_Speicherladung = 1;
																}
															// Ausschalten	
																else if (Sollwert <= 200 || TR_WP[WP1]->messw >= wpd[WP1].intT_Ladeende || wpd[WP1].Status_Quellenschutz > 0)
																			{
																				wpd[WP1].Status_Speicherladung = 0;
																			}
														}
														else	// Rücklauffühler "fehlt"
															{
																wpd[WP1].Status_Speicherladung = 1;
															}
												}
									}			
											// 5. Betriebszustand Warmwasservorrang
											#if TWE_ANF > 0
												
												// Einschalten
												if (wpd[WP1].Status_WW_Vorrang == 0)
													{
													if (station1_ea.messw > 0 && wpd[WP1].Status_Quellenschutz == 0 && wpd[WP1].i_TWE_Sperrzeit_Cnt == 0)
															{
																wpd[WP1].Status_WW_Vorrang = 1;																						// Betriebszustand setzen
																wpd[WP1].i_TWE_Maxzeit_Cnt = wps[WP1].chPa_TWE_Maxlaufzeit_min * 60;			// Counter setzen
															}
													}												
												// Ausschalten
												if (wpd[WP1].Status_WW_Vorrang > 0)
													{
														if ( station1_ea.messw == 0 || wpd[WP1].i_TWE_Maxzeit_Cnt <= 0)		
															{
																wpd[WP1].Status_WW_Vorrang = 0;
																wpd[WP1].i_TWE_Sperrzeit_Cnt = wps[WP1].chPa_TWE_Sperrzeit_min * 60;			// Counter Sperrzeit setzen
																wpd[WP1].i_TWE_Maxzeit_Cnt = 0;																						// Counter Maxzeit nullen
															}												
													}																
											 #endif
											
											
											
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
															else if (wpd[WP1].Status_WW_Vorrang == 1)
																{	
																	Betriebszustand = 5;
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
												case 5:
													wpd[WP1].Status_WPU_Freigabe_oZeit = 1;
													Sollwert = 0;
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
															else if (wpd[WP1].Mindestlaufzeit_Cnt == 0)
																{
																			sch_Flanke = 2; // keine Zustandsänderung bzw keine Flanke
																}
	
										}	
										else if (neustart == 1)
												{
													neustart = 0;
												}		
						 
			  } // Ende:  Nur wenn die Counter nicht aktiv sind, werden die Betriebszustände geprüft ohne Warmwasservorrang
				 
						// Counter
										// Verzögertes Einschalten der WPU bzw. verzögerte Freigabe	ohne Retriggern
								if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
										{
											if (sch_Flanke == 1)
															{
																wpd[WP1].Verzoegerungszeit_Cnt = wps[WP1].chPa_Verzoegerung_min * 60;  // Start
																verz_aktiv = 1;																													// Verzögerung aktiv
															}	
										}
											else if ( wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Mindestlaufzeit_Cnt == 0) 
															{
																--wpd[WP1].Verzoegerungszeit_Cnt; // Counter läuft																
															}	
						 
						// Mindestlaufzeit der WPU ohne Retriggern
								if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt <= 0) 
										{
											if (verz_aktiv > 0)
															{
																wpd[WP1].Mindestlaufzeit_Cnt = wps[WP1].chPa_Mindestlaufzeit_min * 60;  // Start
																verz_aktiv = 0;																													// Verzögerung deaktiviert
															}	
										}
											else if ( wpd[WP1].Sperrzeit_Cnt <= 0 && wpd[WP1].Verzoegerungszeit_Cnt <= 0) 
												{
														--wpd[WP1].Mindestlaufzeit_Cnt; // Counter läuft	
												}				
											
						//	Sperrzeit der WPU ohne Retriggern
								if ( wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Mindestlaufzeit_Cnt <= 0) 
										{ 
											if (sch_Flanke == 0)  // negative Flanke
														{
															wpd[WP1].Sperrzeit_Cnt = wps[WP1].chPa_Sperrzeit_min * 60;  // Start 
														}		
										}
										else if ( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
											{
														--wpd[WP1].Sperrzeit_Cnt; // Counter läuft	
											}	
											
						// Counter zum Warmwasservorrang
												// Counter max
													if (wpd[WP1].i_TWE_Maxzeit_Cnt > 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0 )
														{
															--wpd[WP1].i_TWE_Maxzeit_Cnt;
														}
													// Counter Sperrzeit
													if (wpd[WP1].i_TWE_Sperrzeit_Cnt > 0 )
														{
															--wpd[WP1].i_TWE_Sperrzeit_Cnt;
														}	
					// Ende Counter
						
						// Definition für Heizen und Warmwasservorrang
						if (wpd[WP1].Status_WW_Vorrang > 0 && wpd[WP1].Status_WPU_Freigabe_oZeit > 0)
							{
								wpd[WP1].chPa_WPU_TWE = 1;
								wpd[WP1].chPa_WPU_HEI = 0;
							}
							else if (wpd[WP1].Status_WW_Vorrang == 0 && wpd[WP1].Status_WPU_Freigabe_oZeit > 0)
								{
									wpd[WP1].chPa_WPU_TWE = 0;
									wpd[WP1].chPa_WPU_HEI = 1;	
								}
								else
									{							
										wpd[WP1].chPa_WPU_TWE = 0;
										wpd[WP1].chPa_WPU_HEI = 0;	
									}
						
						//	Steuerung	
	 							
	 							if( wpd[WP1].Mindestlaufzeit_Cnt == 0 && wpd[WP1].Sperrzeit_Cnt == 0 && wpd[WP1].Verzoegerungszeit_Cnt == 0) 
											{
												DA_UNI[0]->wert = wpd[WP1].Status_WPU_Freigabe_oZeit;		// Freigabe WPU (Heizen)
												DA_UNI[1]->wert = wpd[WP1].chPa_WPU_TWE;								// Freigabe WPU (TWE)
												
												if (wpd[WP1].chPa_WPU_TWE > 0)
													{
														DA_UNI[2]->wert = 0;																// Freigabe UV SWP -> TWE	
													}
														else
															{
																DA_UNI[2]->wert = 1;	
															}
														
											if (wps[WP1].Sollwert_EA > 0)
												{
												if (TWE_ANF > 0 && wpd[WP1].chPa_WPU_TWE > 0)
												{
													maxAnford = wps[WP1].intPa_Sollwert_TWE;			// Freigabe der WPU im TWE-Betrieb
												}
												else if (wpd[WP1].chPa_WPU_HEI > 0 )	
													{
														maxAnford = wps[WP1].intPa_Sollwert_HEI;		// Freigabe der WPU im Heizbetrieb						
													}
													else
														{
															maxAnford = wps[WP1].intPa_Sollwert_AUS;		// Wärmepumpe ist aus
														}		
												}
															
										//		DA_UNI[2]->wert = wpd[WP1].chPa_WPU_TWE;		// Freigabe UV SWP/TWE														
											}
											else if (wpd[WP1].Verzoegerungszeit_Cnt > 0)		
												{
													DA_UNI[0]->wert = 0;
													DA_UNI[1]->wert = 0;
												if (wpd[WP1].chPa_WPU_TWE > 0)
													{
														DA_UNI[2]->wert = 0;													// Freigabe UV SWP -> TWE
													}
													else 
														{
															DA_UNI[2]->wert = 1;												// Freigabe UV SWP -> HEIZ	
														}
													if (wps[WP1].Sollwert_EA > 0)
														{
															maxAnford = wps[WP1].intPa_Sollwert_AUS;		// Wärmepumpe ist aus
														}
												
												}
												else if (wpd[WP1].Mindestlaufzeit_Cnt > 0 )		
													{
												DA_UNI[0]->wert = 1;															// Freigabe WPU 
												DA_UNI[1]->wert = wpd[WP1].chPa_WPU_TWE;					// Freigabe WPU TWE-Vorrang
														if (wpd[WP1].chPa_WPU_TWE > 0)	
																{
																DA_UNI[2]->wert = 0;											// Freigabe UV SWP -> TWE
																}
																else
																	{
																		DA_UNI[2]->wert = 1;									// Freigabe UV SWP -> TWE
																	}
										 			if (wps[WP1].Sollwert_EA > 0)
										 				{
														if (wpd[WP1].chPa_WPU_TWE > 0)
														{
															maxAnford = wps[WP1].intPa_Sollwert_TWE;			// Freigabe der WPU im TWE-Betrieb
														}
														else
															{
																maxAnford = wps[WP1].intPa_Sollwert_HEI;		// Freigabe der WPU im Heizbetrieb						
															}	
														}
														
																	
													}
														else if (wpd[WP1].Sperrzeit_Cnt > 0)	
														{
															DA_UNI[0]->wert = 0;
															DA_UNI[1]->wert = 0;
															DA_UNI[2]->wert = 1;
															
													if (wps[WP1].Sollwert_EA > 0)
														{
															maxAnford = wps[WP1].intPa_Sollwert_AUS;		// Wärmepumpe ist aus
														}
															
														}
						
		
													
								// Ende Automatik 	 
			}						
											
						// Zählung der Starts bzgl Freiagbe im Auto- und Handbetrieb
						if (WPU_Freigabe__recent == 0 && 	DA_UNI[0]->wert == 1	)
							{
								++wpd[WP1].WPU_Starts_Freigabe;
								bicbus ( EEPADR,	(char *)&wpd[WP1].WPU_Starts_Freigabe,	WPUSTA_ADR, 2, BICWR);
							}
							
							WPU_Freigabe__recent = DA_UNI[0]->wert;
							
						// Zählung der Starts bzgl Betriebsmeldung
						#if BM_WPU > 0
							if (BM_recent == 0 && BM_UNI[U1]->bwert == 1)
								{
									++wpd[WP1].WPU_Starts_BM;
									bicbus ( EEPADR,	(char *)&wpd[WP1].WPU_Starts_BM,	WPUSTA_ADR+2, 2, BICWR);
								}
							BM_recent = BM_UNI[U1]->bwert;
						#endif
						// Laufzeit Anforderung und BM
						// Anforderung
							if ( DA_UNI[0]->wert == 1 )
								{	
									if ( ++wpd[WP1].WPU_Freigabe_Laufzeit_sec >= 3600 )
										{	
											wpd[WP1].WPU_Freigabe_Laufzeit_sec = 0;
											wpd[WP1].WPU_Freigabe_Laufzeit_h++;							// Laufzeit h erhöhen
											bicbus ( EEPADR, (char*)&wpd[WP1].WPU_Freigabe_Laufzeit_h, WPULaufzeit_ADR, 2, BICWR );
										}
								}
						// BM
						#if BM_WPU > 0
							if ( BM_UNI[U1]->bwert == 1 )
								{	
									if ( ++wpd[WP1].WPU_BM_Laufzeit_sec >= 3600 )
										{	
											wpd[WP1].WPU_BM_Laufzeit_sec = 0;
											wpd[WP1].WPU_BM_Laufzeit_h++;							// Laufzeit h erhöhen
											bicbus ( EEPADR, (char*)&wpd[WP1].WPU_BM_Laufzeit_h, WPULaufzeit_ADR+2, 2, BICWR );
										}
								}
							#endif				


									//------ Steuerung SOLLWERT-----------------------------------------------//
										// Sollwertweitergabe an WPU 								
										if (wps[WP1].Sollwert_EA == 0)
											{
											
											#if TWE_ANF > 0	
												if (wpd[WP1].chPa_WPU_TWE > 0)
														{
															maxAnford = wps[WP1].intPa_Sollwert_TWE;			// 
														}
											#endif			
											
											if (wpd[WP1].Sperrzeit_Cnt > 0 || wpd[WP1].Verzoegerungszeit_Cnt > 0)
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
											}							

										
										if ( maxAnford < TmanfSkalMin )
											TMANF[0]->awert = 0;
										else if ( maxAnford > TmanfSkalMax )	
											TMANF[0]->awert = TmanfSkalMaxSpg;
										else
											TMANF[0]->awert = Gerade_YvonX ( maxAnford, TmanfSkalMin, TmanfSkalMinSpg, TmanfSkalMax, TmanfSkalMaxSpg );	
									
									//--------ENDE Ausgehender Sollwert-----------------------//			
																	
			
			// Allgemeine Funktion ohne Steuerung des Ausgangs
			// Unterstützung anfordern
			#if WPU_UST > 0
			
				if (TSPm_WP[WP1]->stat == 0)
					{
						T_Unterst = TSPm_WP[WP1]->messw;
					}
					else if (TSPo_WP[WP1]->stat == 0)
						{
							T_Unterst = TSPo_WP[WP1]->messw;
						}
						else
							{
								T_Unterst = 1500;
							}
						// EIN
						wpd[WP1].intT_Unterst_ANF_ein = Sollwert + wps[WP1].intPa_Unterst_ANF_ein; 
						// AUS
						wpd[WP1].intT_Unterst_ANF_aus = Sollwert + wps[WP1].intPa_Unterst_ANF_aus;
					
													if (wpd[WP1].Status_Unterstuetzung == 0)
														{
															if (T_Unterst <= wpd[WP1].intT_Unterst_ANF_ein  && T_Unterst < 1500)
																{
																	wpd[WP1].Status_Unterstuetzung = 1;
																}
														}
															// Ausschalten	
													if (wpd[WP1].Status_Unterstuetzung > 0)
														{
															if ( T_Unterst >= wpd[WP1].intT_Unterst_ANF_aus  || T_Unterst > 1000)
																{
																	wpd[WP1].Status_Unterstuetzung = 0;
																}
														}
			#endif// Ende Unterstützung anfordern
			
			// Ende Allgemeine Funktion
			// MODBUS - Dimplex
			#if MB_Dimplex > 0
				// Übergeben
				mod_data.i_W_Festwertsolltemperatur = maxAnford;
				// Übernehmen
				if (mod_data.uc_R_Verdichter1 > 0 || mod_data.uc_R_Verdichter2)
					{
						wpd[WP1].ch_BM_WPU_MB = 1;
					} 
					else
						{
							wpd[WP1].ch_BM_WPU_MB = 0;
						}
			#endif
			// -------------------------------- Diagnose ----------------------------------------------------//
			// Analysewerte zum Wärmepumpenbetrieb
			
			// Leistung und Arbeit des letzten WPU-Betriebs
		
		}// ENDE: NEUSTART 		
}//	Ende Steuer WPU


//-------------------------------------------------------------------------------------------------
//										Unterprogramme
//-------------------------------------------------------------------------------------------------

//*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Ermittlung der Sarts bzgl BM und Freigabe							 								                                 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

