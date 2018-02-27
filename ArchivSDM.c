/**********************************************************************************/
/*                                                                                */
/* Daten-Archivierung auf SD-Card-Memory                                          */
/*       ohne Filesystem                                                          */
/* Dr.Riedel Automatisierungstechnik GmbH                                         */
/* Programmierer: Joachim Schinske                                                */
/*                                                                                */
/* Stand: 22.03.2015                                                              */
/*        05.01.2016 Kommando: CHECK_FERRO_SDHC                                   */
/*        19.05.2016 Aktion 0Uhr mit set_daily_init()                             */
/*        11.01.2017 Indexberechnung in save_folge(...)  neu                      */
/*                   Konsistenzprüfung mit VP_Kontrolle(...)                      */
/*                   Einzelrücksetzung mit one_reset_dae(...)                     */
/*                   Kommando: WRITE_SDMEM                                        */
/*        01.02.2017 Zeitstempel auf Gültigkeit überprüfen in check_sdtime(...)   */
/*        05.04.2017 one_reset_dae(...) off_DP gesetzt                            */
/*        26.09.2017 Bei Fehler: Neustart verzögern                               */
/*                                                                                */
/**********************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "sfr32C87.h"
#include "defins.h"
#include "konvform.h"
#include "struct.h"
#include "constext.h"
#include "sramext.h"
#include "uramext.h"
#include "ferrodef.h"
#include "archivsdm.h"
#if SPEED_SAVE == 1			//$ Test
#include "standdef.h"
#endif

#include "gl_types.h"
#include "crcx.h"

extern void refresh_value(unsigned int dae, char exp);

#define  HWDOG_CNT			200		// Zeitüberwachung des timeout-counters zum Einbinden von hwdog(1)  
#define  FERRO_INIT_KZ	2015


#if ARCHIV_SDM == 1		// Josch-SDM

/*-------------------------- prototypes geerbt von CRD.c -----------------------------*/
//static BYTE initCRDReg(PCRD pCard, BYTE crdNo, BOOL init);
  static DWORD getValue(BYTE* data, BYTE cnt, BYTE rev , UINT start, UINT end);
  static BYTE sendCommand(BYTE cmd, BYTE arg3, BYTE arg2, BYTE arg1, BYTE arg0, DWORD timeout);
  static BYTE sendCommand2(BYTE cmd, DWORD adr, DWORD timeout);
//static WORD getStatus(PCRD pCard);
//static BYTE crc7Fkt(BYTE crc, const BYTE *buffer, UINT len);
  static BYTE crc7Ex(BYTE par0, BYTE par1, BYTE par2, BYTE par3, BYTE par4);
  static DWORD getR7(VOID);
  static DWORD getR3(VOID);
//static BYTE getR1b(DWORD timeout);
//static BYTE readTestData(PCRD pCard);
  static void waitForCard(WORD len);
//static BYTE sendStopCmd(PCRD pCard);
  static void writeBytes(BYTE *pData, WORD len, WORD *pcrc);
//static void writeBytes2(BYTE data, WORD len, WORD *pcrc);
//static void writeBytes3(PBYTE pData, WORD len, PWORD pcrc);
  static void readBytes(BYTE *pData, WORD len);
  static BOOL checkCRC16(char num, BYTE *pdata, WORD len);
  static BOOL waitForStartByte(DWORD timeout);
  static BOOL waitForNotBusy(DWORD timeout);
//static BYTE calcCardData(PCRD pCard);
  static VOID waitForMemCard(VOID);
//static DWORD DMAX(DWORD v1, DWORD v2);
//static INT gl_strlen(CPCHAR ptext);
  #ifndef DISABLE_CRC_CHECK
  static UINT crc16Fkt(UINT crc, const BYTE *buffer, UINT len);
  #endif
/*-------------------------- prototypes	geerbt von ICRD.c	-----------------------------*/
  static BYTE receiveByte(void);
  static BYTE sendByte(BYTE data);
  static BOOL selectCard(BYTE no, BOOL select);
  static BYTE setLED(BYTE no, BOOL value);

/*-------------------------- prototypes eigene ----------------------------------------*/

CRD_ERG init_Card(DWORD timeout_max);
BYTE    init_Card_Reg(DWORD timeout_max);

CRD_ERG read_data_block(DWORD adr, BYTE *pData, DWORD timeout_max);
BYTE    read_512_data_block(DWORD adr, BYTE *pData, DWORD timeout_max);

CRD_ERG write_data_block(DWORD adr, BYTE *pData, DWORD timeout_max);
BYTE    write_512_data_block(DWORD adr, BYTE *pData, DWORD timeout_max);

void * ferro_malloc(char anz, char max);
ULONG sdm_malloc(ULONG anz, char max);
CRD_ERG save_data(UINT dae, char exp_idx, char exp_dae, char *folge, ARC_ERG *a_erg);
CRD_ERG save_folge(ARC_ERG *a_erg, ULONG zeit);
CRD_ERG Verify_Komtab(UINT ktleng, ARC_ERG *a_erg, int *dae_err);
CRD_ERG Save_Komtab(UINT ktleng);
CRD_ERG Verify_ExpInfotab(ARC_ERG *a_erg, int *dae_err);
CRD_ERG Save_ExpInfotab(void);
void built_a_erg_text(ARC_ERG ret, char *ret_text);
void built_c_erg_text(CRD_ERG ret, char *ret_text);
void Check_SDM_Alarm(void);

char search_data(UINT dae, char exp, ULONG my_time, char *anz_DP, char *off_DP, ULONG *first_time, char *buffer);
char get_address(UINT dae, char exp, UINT *ferro_adr, ULONG *hp_adr);
void set_daily_init(void);
char VP_Kontrolle(UINT dae, char exp, char *vp_err);
void one_reset_dae(UINT r_dae, char r_exp);
char check_sdtime(ULONG sd_time, ULONG my_time);

/*-------------------------- Task ------------------------------------------------------*/
// Zeitbasis für den Taskaufruf:		100 ms

void ArchivSDM(void)
{
	static BYTE neustart = 1;
	static BYTE sdm_ferro_init_run = 0;
	
	BYTE ret;
	BYTE bicret;
	BYTE  lzkonv_buf[16] = {0};
	BYTE  byte_anz;
	

	BYTE	ferro_sizeof = 0;
	UINT	ferro_kz = 0;
	UINT	sdm_kz = 0;
	BYTE	flag = 0;
	BYTE	state = 0;
	int   dae_err = -1;
	char  vp_err = 0;
	UINT	p_dae = 0;
	UINT	r_dae = 0;
	char	r_exp = 0;
	
	ARC_ERG a_erg;			// Ergebnisse der Archivierung  
	CRD_ERG c_erg;			// Ergebnisse der Card-Zugriffe

	DWORD sdm_sizeof;
	
	UINT i, j;
	BYTE folge = 0;
	ULONG zeit;
	
	timeout_max = 20000;     /* max. count of retry */

	//c_erg_gl = c_erg;					// Ergebnis der Card-Zugriffe veröffentlichen in der parli
	//a_erg_gl = a_erg;					// Ergebnis der Archivierung  veröffentlichen in der parli
	
	built_a_erg_text(a_erg_gl, a_erg_text);						// Anzeige in parli: SDM-ARCH-STATUS
	built_c_erg_text(c_erg_gl, c_erg_text);						// Anzeige in parli: SDM-CARD-STATUS
	if(	sdm_base_check == 1)
		Check_SDM_Alarm();
	
	if(sdm_arch_stop > 0)				// Benutzer-Stopp
	{
		a_erg    = ARC_STOP;
		a_erg_gl = a_erg;
		c_erg    = CRD_STOP;
		c_erg_gl = c_erg;
		return;
	}
	else
	{
		if(a_erg_gl == ARC_STOP)	// war Benutzer-Stopp ?
		{
			a_erg = ARC_OK;
			a_erg_gl = a_erg;
		}	
			
		if(c_erg_gl == CRD_STOP)
		{
			c_erg    = CRD_OK;
			c_erg_gl = c_erg;
		}
	}
	
	#if ZEIT_MESSUNG == 1
	if(write_zeit >= write_zeit_max)
		write_zeit_max = write_zeit;
	#endif
		
	//------------------------------------------------------
	// Initialisierung der Expandadressen nach Reset
	//------------------------------------------------------
	if(neustart > 0)
	{
		if(neustart > 1)	// Bei Fehler: Neustart verzögern
		{
			neustart--;
			return;
		}
		
		// ca. 50ms SDHC Class10
		if(monitor_init == 0)																// Datenmonitor initialisiert ?
		{
			a_erg = ARC_ERR_MONITOR;
			a_erg_gl = a_erg;
			return;																						// nein
		}
		else
			a_erg = ARC_OK;
		
		// Ferro-Expand-Adressen dynamisch festlegen. Basis ist die expinfo_tab[] in DMonitor.c (System) 
		ferro_mnext   = DAE_INFO_EXP_ADR;										// Beginn des Speicherbereiches für Expandparameter im Ferro-Ram
		ferro_msize   = FERRO_SIZE - DAE_INFO_EXP_ADR;			// Größe des verfügbaren Speicherbereiches
		ferro_sizeof  = sizeof(struct fe_info);							// Größe eines Info-Blocks
		
		sdm_mnext  = SDM_EXPAND_ADR;												// Beginn des Speicherbereiches für Expandparameter im SD-Card-Memory
		sdm_msize  = SDM_SIZE - SDM_EXPAND_ADR;							// Größe des verfügbaren Speicherbereiches
		sdm_sizeof = SDM_BLOCK_DAE;													// Speichergröße für ein Datenelement
		
		for(i = 0; i < KOMTAB_LENG; i++)
		{	
			exp_index = expinfo_tab[i].exp_idx;								// Expand Informationstabelle für jedes DAE der Komtab (SysRam)
			if(exp_index > 0)
			{
				max_expand = expinfo_tab[i].max_exp;
				
				// Expand-Speicherplatz für ferro alloziieren.
				exp_ferro_tab[exp_index-1] = (ferro_info *)ferro_malloc( ferro_sizeof, max_expand );
				if(exp_ferro_tab[exp_index-1] == 0)
				{
					a_erg    = ARC_ERR_FERRO_HEAP;
					a_erg_gl = a_erg;
					a_sperre = 1;
					break;
				}		
				
				// Expand-Speicherplatz für SD-Card-Memory alloziieren
				exp_sdm_tab[exp_index-1] = sdm_malloc( sdm_sizeof, max_expand );
				if(exp_sdm_tab[exp_index-1] == 0)
				{
					a_erg    = ARC_ERR_CARD_HEAP;
					a_erg_gl = a_erg;
					a_sperre = 1;
					break;
				}		
			}
		}
		
		// Initialisierungskennzeichen (Komtablänge) überprüfen
		// im Ferro-Ram
		bicret = xicbus(XEEPADR+2, (char	*)&ferro_kz, FERRO_INIT_KZ_ADR, 2, BICRD);
		if(bicret == 0)	// Fehler
		{
			a_erg    = ARC_ERR_FERRO_READ;											// nicht lesbar oder nicht gesteckt
			a_erg_gl = a_erg;
		}

		// Initialisierungskennzeichen (Komtablänge) überprüfen
		// auf der SD-Karte
		c_erg = init_Card(timeout_max);
		if(c_erg != CRD_OK)
		{																											// nicht lesbar oder nicht gesteckt
			c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
			if(a_erg != ARC_ERR_FERRO_READ)
			{
				a_erg = ARC_ERR_CARD;																// Archiv-Status Fehleranzeige
				a_erg_gl = a_erg;
			}	
		}
		
		sdm_base_check = 1;
		// Abbruch bei Fehler
		if(a_erg != ARC_OK || c_erg != CRD_OK)
		{
			neustart = 50;	// Neustart erst nach 5 Sekunden wieder ausführen
			return;
		}
		
		// Datenpage lesen
		dp_adr = SDM_INITKZ_ADR;
		c_erg = read_data_block(dp_adr, asdm_page, timeout_max);
		if(c_erg != CRD_OK)
		{
			c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
			a_erg = ARC_ERR_CARD;																// Archiv-Status Fehleranzeige
			a_erg_gl = a_erg;
			return;																							// Card Lesefehler
		}
		else
		{
			sdm_kz = *(unsigned int *)&asdm_page[0];						// Kennzeichen
		}
		
		// Kennzeichenvergleich
		flag = 0;
		dae_append = 0;
		if( (ferro_kz == sdm_kz) && (ferro_kz != 0) )
		{
			// Beide gespeicherte Kennzeichen sind gleich. Ferro und SD sind initialisiert worden.
			// ist auch die Komtablänge gleich geblieben ?
			if(KOMTAB_LENG > ferro_kz)
			{
				// nein, die neue Komtab ist länger geworden
				c_erg = Verify_Komtab(ferro_kz, &a_erg, &dae_err);	// Prüfen, ob die alten Parameter gleich geblieben sind  
				if(c_erg != CRD_OK)
				{
					c_erg_gl = c_erg;																	// Card-Status   Fehleranzeige 
					a_erg = ARC_ERR_CARD;															// Archiv-Status Fehleranzeige
					a_erg_gl = a_erg;
					return;																						// Card Lesefehler
				}
				else
				{
					if(a_erg == ARC_OK)																// der alte Parameter-Block ist gleich geblieben
					{
						dae_append = KOMTAB_LENG - ferro_kz;						// fehlende Parameter anhängen
					}
					else
					{
						komtab_changed = dae_err;												// parli
						a_erg_gl = a_erg;																// Archiv-Status Fehleranzeige
					}
					flag = 1;																					// mit oder ohne dae_append: Werkseinstellung aktivieren
				} 
			}
			else if(KOMTAB_LENG <= ferro_kz)
			{
				// nein, die neue Komtab ist kürzer geworden oder gleich lang geblieben
				c_erg = Verify_Komtab(KOMTAB_LENG, &a_erg, &dae_err);	// Prüfen, ob die Parameter gleich geblieben sind  
				if(c_erg != CRD_OK)
				{
					c_erg_gl = c_erg;																	// Card-Status   Fehleranzeige 
					a_erg = ARC_ERR_CARD;															// Archiv-Status Fehleranzeige
					a_erg_gl = a_erg;
					return;																						// Card Lesefehler
				}
				else
				{
					if(a_erg != ARC_OK)																// ungleich,
					{
						komtab_changed = dae_err;												// parli
						a_erg_gl = a_erg;																// Archiv-Status Fehleranzeige
						flag = 1;																				// Werkseinstellung aktivieren
					}
				}
				
				// Überprüfen auf veränderte max_exp
				if(flag == 0)
				{
					c_erg = Verify_ExpInfotab(&a_erg, &dae_err);
					if(c_erg != CRD_OK)
					{
						c_erg_gl = c_erg;																	// Card-Status   Fehleranzeige 
						a_erg = ARC_ERR_CARD;															// Archiv-Status Fehleranzeige
						a_erg_gl = a_erg;
						return;																						// Card Lesefehler
					}
					else
					{
						if(a_erg != ARC_OK)																// ungleich,
						{
							komtab_changed = dae_err;												// parli
							a_erg_gl = a_erg;																// Archiv-Status Fehleranzeige
							flag = 1;																				// Werkseinstellung aktivieren
						}
					}
				}
			}
		}
		else																										// ungleiche Kennzeichen,
		{
			flag = 1;																							// Werkseinstellung aktivieren
			if(ferro_kz != KOMTAB_LENG)
			{
				a_erg    = ARC_ERR_FERRO_INITKZ;
				a_erg_gl = a_erg;
			}
		
			if(sdm_kz != KOMTAB_LENG)
			{
				a_erg    = ARC_ERR_SDM_INITKZ;
				a_erg_gl = a_erg;
			}
		}	
			
		// Werkseinstellung aktivieren ?	
		if(flag == 1)
		{
			werks_sdm_code = 1111;
		}	
		
		neustart = 0;
		return;
	}

	//--------------------------------------------------------------------
	// Werkseinstellung wurde aktiviert
	//--------------------------------------------------------------------
				
	if(sdm_ferro_init_run == 1)
	{
			//  2ms ohne Endebehandlung
			//  45ms mit Endebehandlung
			max_expand = expinfo_tab[current_dae].max_exp;
			exp_index  = expinfo_tab[current_dae].exp_idx;	// normal 0, beginnt bei 1 (1.gefundene Expandgruppe), wenn max_expand > 0
			
		
			byte_anz = LzKonv(lzkonv_buf, current_dae, 0, 0);
			dae_info[0].akt_DP = 1;											// aktuelle Datenpage-Nr.
			dae_info[0].num_DP = 0;											// Anzahl der Einträge in der Datenpage
			dae_info[0].off_DP = (char)(byte_anz + 4);	// Offset zum nächsten Eintrag (+4 Byte Zeitstempel)
			dae_info[0].akt_VP = 1;											// aktuelle Verweispage-Nr.
			dae_info[0].num_VP = 0;											// Anzahl der Einträge in der Verweispage
			dae_info[0].num_HP = 0;											// Anzahl der Einträge in der Hauptpage
			dae_info[0].ovr_HP = 0;											// Anzahl der Datenpage-Überläufe (Ringpuffer)

			if(byte_anz == 0)														// Fehler in LzKonv, unbekannte Konvertierung
			{
				dae_info[0].akt_DP = 0;
				dae_info[0].off_DP = 0;
				dae_info[0].akt_VP = 0;
			}
				
			// Ferro-Adresse ermitteln
			if(max_expand == 0)
			{ //    Anfangsadresse	+ Offset
				ferro_adr = DAE_INFO_ADR    + DAE_INFO_LENG * current_dae;			// Normalbereich
			}
			else 
			{
				// Zugewiesene Speicherplätze
				ferro_adr = (UINT )&(exp_ferro_tab[exp_index-1][current_dae_exp]);
			}				
			
			bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICWR);
			if(bicret == 0)	// Fehler
			{
				sdm_ferro_init_run = 0;
				werks_sdm_code = 0;
				a_erg    = ARC_ERR_FERRO_READ;
				a_erg_gl = a_erg;
				a_sperre = 1;
			}
			else
			{	
				if(current_dae_exp > 0)						// noch beschäftigt mit den Expandparametern ?
				{
					werks_sdm_code = current_dae * 100 + (char)(current_dae_exp+1);	// Anzeige in parli
					if(++current_dae_exp >= max_expand)
					{
						current_dae_exp = 0;
						current_dae++;
					}
				}
				else if(max_expand > 0 && current_dae_exp == 0)
						current_dae_exp = 1;					// Start mit Expandparametern
				else
				{
					current_dae++;								// nächste DAE
					if(current_dae >= KOMTAB_LENG)
					{	
						// Endebehandlung
						current_dae = 0;
						sdm_ferro_init_run = 0;
						neustart = 1;

						// Kennzeichen eintragen im Ferro-Ram
						ferro_kz = KOMTAB_LENG;
						bicret = xicbus(XEEPADR+2, (char	*)&ferro_kz, FERRO_INIT_KZ_ADR, 2, BICWR);
						if(bicret == 0)	// Fehler
						{
							a_erg    = ARC_ERR_FERRO_WRITE;
							a_erg_gl = a_erg;
						}
						else
						{
							a_erg    = ARC_OK;
							a_erg_gl = a_erg;
						}	
						
						// Kennzeichen eintragen auf der SD-Karte
						i = 0;
						memset(asdm_page, i, 512);																	// Page löschen
						sdm_kz = KOMTAB_LENG;
						*(UINT *)(asdm_buffer) = sdm_kz;
						memcpy(&asdm_page[0], &asdm_buffer[0], 2);
						dp_adr = SDM_INITKZ_ADR;
						c_erg = write_data_block(dp_adr, asdm_page, timeout_max);
						if(c_erg != CRD_OK)
						{
							c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
							a_erg = ARC_ERR_CARD;																// Archiv-Status Fehleranzeige
							a_erg_gl = a_erg;
						}
						else
						{
							c_erg_gl = c_erg;
						}
						
						// Komtab speichern auf der SD-Karte (CRC-Code für jedes DAE)  
						c_erg = Save_Komtab(sdm_kz);													
						if(c_erg != CRD_OK)
						{
							c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
							a_erg = ARC_ERR_CARD;																// Archiv-Status Fehleranzeige
							a_erg_gl = a_erg;
						}
						
						// ExpandInfotab speichern auf der SD-Karte (max_exp für jedes DAE)  
						c_erg = Save_ExpInfotab();													
						if(c_erg != CRD_OK)
						{
							c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
							a_erg = ARC_ERR_CARD;																// Archiv-Status Fehleranzeige
							a_erg_gl = a_erg;
						}
								
					}	
					else
						werks_sdm_code = current_dae * 100;										// Anzeige in parli
				}
			}
			return;
	}			
	
	//--------------------------------------------------------------------
	// Werkseinstellung aktivieren
	//--------------------------------------------------------------------
	if(werks_sdm_code == 1111)	
	{
		// Initalisieren des Memory-Bereiches durch Rücksetzen der Zeiger im Ferro-Ram
		sdm_ferro_init_run = 1;
		
		if(dae_append > 0)
			current_dae = dae_append;			// nur die angehangenen DAE bearbeiten
		else
			current_dae = 0;							// alle DAE bearbeiten  	
		
		dae_append = 0;
		current_dae_exp = 0;
		werks_sdm_code = 0;
		
		return;
	}	
	
	//--------------------------------------------------------------------
	// Werkseinstellung für ein DAE (nur ferro_info)
	//--------------------------------------------------------------------
	if(reset_dae < 0)		// Syntax: int  xxx,y   -DAE,EXP    EXP einstellig
	{
		p_dae = 0 - reset_dae;
		r_dae = p_dae / 10;
		r_exp = p_dae % 10;
		
		one_reset_dae(r_dae, r_exp);
		
		reset_dae = 0;
		return;
	}	


	//-----------------------------------------------------------------------------------
	// Beginn der Monitorauswertung  mit evtl. Archivierungsprozedur
	//-----------------------------------------------------------------------------------
	if(a_sperre == 1)	// Archivsperre ausgelöst wegen Fehler in neustart oder Werkseinstellung
		return;
	
	// tägliche Rundumspeicherung initialisieren   oder von Hand
	if( (Std == 23 && Min == 59 && daily_init == 0) || daily_init == 2)
	{
		daily_init = 1;
		set_daily_init();
	}
	
	sdm_akt_dae = current_dae * 100 + current_dae_exp;				// Anzeige aktuelle DAE und EXP

	switch(asdm_status)
	{
		case START:
			#if SPEED_SAVE == 1 		// Testfunktion: permanente Speicherung mit einem DAE
			if(test_start == 1)
			{
				// Testbeginn: 0x1F07E260;	= 01.04.2015 00:00	wird mit Kaltstart 56 gesetzt
				asdm_test_start += 60;														// Addiere zur Startzeit 1 Minute
				bicbus ( EEPADR,	(char *)&asdm_test_start,	ASDM_TEST_START_ADR, 4, BICWR);
				asdm_time = asdm_test_start;											
				asdm_status = SD_INIT;										
			}
			#else										// normal
			// Start im 5 Minutenraster
			if( (((Min % 5) == 0 || Min == 0) && (asdm_minut != Min))  || test_start == 1) 
			{
				test_start = 0;

				asdm_time  = LongZeit;													// aktuelle Zeit
				asdm_minut = Min;																// nicht nochmal in der gleichen Minute
				asdm_status = SD_INIT;
			}
			#endif										
			else
			{
				// Karte gesteckt oder entfernt 	
				if(mount_check++ >= 100)															// alle 10 Sekunden
				{
					mount_check = 0;
					c_erg = init_Card(timeout_max);
					if(c_erg == CRD_OK)
					{
						if(c_erg_gl == CRD_ERR_CMD0 || c_erg_gl == CRD_ERR_OCR1)	// war nicht gesteckt ?
						{
				 			c_erg    = CRD_OK;
				 			c_erg_gl = c_erg;	
				 			a_erg    = ARC_OK;
				 			a_erg_gl = a_erg;
				 		}
				 	}
				 	else																						// Karte ist nicht gesteckt
				 	{
						c_erg_gl = c_erg;															// Card-Status   Fehleranzeige 
						a_erg = ARC_ERR_CARD;													// Archiv-Status Fehleranzeige
						a_erg_gl = a_erg;
					}
				}
			}	
			break;
			
		case SD_INIT:																						// Initialisierung der SD-Karte
			c_erg = init_Card(timeout_max);
			if(c_erg == CRD_OK)
			{
				#if SPEED_SAVE == 1 
				current_dae = SPEED_DAE;														//  Test-Datenelement
				#else
				current_dae = -1;																		// 1.Datenelement Vorbereitung
				#endif

				current_dae_exp = 0;
				max_expand = 0;
				exp_index  = 0;
				asdm_status = CHECK_DAE;
			}		
			else
			{
				c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
				a_erg = ARC_ERR_CARD;																// Archiv-Status Fehleranzeige
				a_erg_gl = a_erg;
				asdm_status = ERROR;																// zurück zum Start
			}
			break;

		case CHECK_DAE:																					// Untersuchung auf veränderte Parameter
			#if SPEED_SAVE == 1
			if( test_start == 0 )
				asdm_status = ENDE;
			#else
		  if( (current_dae + 1) >= KOMTAB_LENG )
				asdm_status = ENDE;
			#endif	
			else
			{
				#if SPEED_SAVE == 0
				current_dae++;																			// aktuelle DAE
				#endif
			
				a_erg    = ARC_RUN;
				a_erg_gl = a_erg;
				c_erg    = CRD_RUN;
				c_erg_gl = c_erg;
				state    = monitor_tab[current_dae].state;						// in DMonitor.c (System)
				
				#if SPEED_SAVE == 1
				if(state == DAE_CHANGING || state == DAE_INITIAL || state == DAE_MONITORING)
				#else
				if(state == DAE_CHANGING || state == DAE_INITIAL)
				#endif
					asdm_status = CHECK_VP;															// Normalparameter, Konsistenzprüfung
				else if(state == DAE_EXPMONI)
				{
					asdm_status = ACTION_EXP_DAE;												// Expandparameter
					asdm_action = GET_EXP_TAB;
				}
			}		
			break;
			
		case CHECK_VP:
			vp_err = 0;
			if(VP_Kontrolle(current_dae, 0, &vp_err) == 1)	// Prüfen auf Konsistenz der Ferro-Infos mit der Anzahl der Einträge in der Verweispage
			{	
				if(vp_err == 1)																			// Konsistenzfehler ?
					c_erg = save_folge(&a_erg, read_time);						// ja, Folge-Prozedure wiederholen
			}
			asdm_status = SAVE_DAE;																// Normalparameter, Datenspeicherung
			break;	
			
		case SAVE_DAE:																					// Speichern Normalparameter
			c_erg = save_data(current_dae, 0, 0, &folge, &a_erg);	// ca. 10ms
			if(c_erg == CRD_OK)
			{
				refresh_value(current_dae, 0);											// in DMonitor.c (System): oldvalue = akt.Wert , state = DAE_MONITORING
				if(folge == 1)																			// Zusätzliche Speicherungen (Verweispage usw.) ?
					asdm_status = FOLGE_SAVE_DAE;
				else
					#if SPEED_SAVE == 1
					asdm_status = ENDE;
					#else					
					asdm_status = CHECK_DAE;
					#endif
			}
			else
			{
				c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
				a_erg_gl = a_erg;																		// Archiv-Status Fehleranzeige
				asdm_status = ERROR;	// Fehler
			}			
			break;
			
		case FOLGE_SAVE_DAE:																		// Zusätzliche Speicherungen (Verweispage usw.)
			c_erg = save_folge(&a_erg, asdm_time);								// ca. 20ms
			if(c_erg == CRD_OK)
				#if SPEED_SAVE == 1
				asdm_status = ENDE;
				#else
				asdm_status = CHECK_DAE;
				#endif
			else
			{
				c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
				a_erg_gl = a_erg;																		// Archiv-Status Fehleranzeige
				asdm_status = ERROR;	// Fehler		
			}
			break;
			
		//--------------------------------------------------------------------------------------
		case ACTION_EXP_DAE:																		// Prozedur mit Expandparametern
			switch(asdm_action)
			{
				case GET_EXP_TAB:																		// Expandtabelle ermitteln
					max_expand = expinfo_tab[current_dae].max_exp;
					exp_index  = expinfo_tab[current_dae].exp_idx;		// beginnt bei 1 (1.gefundene Expandgruppe), wenn max_expand > 0
 					current_dae_exp = 0;
 					asdm_action = CHECK_EXP_DAE;
 					break;
 					
 				case CHECK_EXP_DAE:																	// Untersuchung auf veränderte Parameter
 					state = expmoni_tab[exp_index][current_dae_exp].state;
 					if(current_dae_exp >= max_expand)
 					{
						current_dae_exp = 0;
						max_expand = 0;
						exp_index  = 0;
 						asdm_status = CHECK_DAE;												// Ende Expandparameter , weiter mit Normalparameter
 					}
 					else if(state == DAE_CHANGING || state == DAE_INITIAL)
 						asdm_action = CHECK_EXP_VP;													// Konsistenzprüfung
 					else
 						current_dae_exp++;
 					break;
 					
 				case CHECK_EXP_VP:
					vp_err = 0;
					if(VP_Kontrolle(current_dae, current_dae_exp, &vp_err) == 1)				// Prüfen auf Konsistenz der Ferro-Infos mit der Anzahl der Einträge in der Verweispage
					{	
						if(vp_err == 1)																		// Konsistenzfehler ?
							c_erg = save_folge(&a_erg, read_time);					// ja, Folge-Prozedure wiederholen
					}
					asdm_action = SAVE_EXP_DAE;													// Speichern Expandparameter
					break;	
 				
 				case SAVE_EXP_DAE:																	// Speichern Expandparameter
 					c_erg = save_data(current_dae, exp_index, current_dae_exp, &folge, &a_erg);
					if(c_erg == CRD_OK)
 					{
 						refresh_value(current_dae, current_dae_exp + 1);
 						if(folge == 1)
 							asdm_action = FOLGE_SAVE_EXP_DAE;
 						else
 						{	
 							current_dae_exp++;
 							asdm_action = CHECK_EXP_DAE;
 						}
 					}
					else
					{
						c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
						a_erg_gl = a_erg;																		// Archiv-Status Fehleranzeige
						asdm_status = ERROR;	// Fehler		
					}
 					break;
 					
 				case FOLGE_SAVE_EXP_DAE:
					c_erg = save_folge(&a_erg, asdm_time);
					if(c_erg == CRD_OK)
					{	
						current_dae_exp++;
						asdm_action = CHECK_EXP_DAE;
					}
					else
					{
						c_erg_gl = c_erg;																		// Card-Status   Fehleranzeige 
						a_erg_gl = a_erg;																		// Archiv-Status Fehleranzeige
						asdm_status = ERROR;	// Fehler		
					}
					break;
 				
 				default:
 					// Fehler
 					asdm_status = ENDE;	
 					
 			}
 			break;
 			
 		case ERROR:
 			asdm_status = START;
 			asdm_action = GET_EXP_TAB;
 			test_start = 0;
 			break;
 		
 		
 		case ENDE:
 		default:
 			c_erg    = CRD_PAUSE;
 			c_erg_gl = c_erg;	
 			a_erg    = ARC_PAUSE;
 			a_erg_gl = a_erg;
 			asdm_status = START;
 			asdm_action = GET_EXP_TAB;
 			daily_init  = 0;
 			break;
	}	
}

//-----------------------------------------------------------------------------
// Unterprogramme
//-----------------------------------------------------------------------------

//------------------------------------------------------------
// Komtab-Überprüfung
//------------------------------------------------------------
// Aus der ersten Zeile eines Parameters (Gruppe und Bezeichnung) wird ein CRC-Code für Vergleichszwecke erstellt.  
CRD_ERG Verify_Komtab(UINT ktleng, ARC_ERG *a_erg, int *dae_err)
{
	CRD_ERG c_erg = 0;

	char buffer[20] = {0};
	char gnum = 0;
	char pnum = 0;
	
	int i = 0;
	int j = 0;
	int idx = 0;
	UINT crc_k = 0;
	UINT crc_p = 0;
	
	ULONG kt1_adr = SDM_KOMTA1_ADR;
	ULONG kt2_adr = SDM_KOMTA2_ADR;
		
	// Vorbelegung der return Parameter
	*a_erg = ARC_OK;
	*dae_err = -1; // ungültige DAE-nummer
	
	c_erg = read_data_block(kt1_adr, asdm_page, timeout_max);	// 1. Komtab-Vergleichs-Page lesen
	if(c_erg != CRD_OK)
	{
		return c_erg;																						// Card Lesefehler			
	}					
	
	if(ktleng < 256)
		idx = ktleng;
	else
	  idx = 256;	
		
	for(i=0, j=0; i < idx; i++, j+=2)
	{
		gnum = KomTab[i].gnum;
		pnum = KomTab[i].pnum;
		memcpy(&buffer[0], Pgruppe[gnum].grkz, 4);								// 4  Byte ParameterGruppe 
		memcpy(&buffer[4], Pgruppe[gnum].Param[pnum].bez, 16);		// 16 Byte ParameterBezeichnung
		crc_k = Checksum(buffer, 20);															// Es wird eine CRC-Summe gebildet	
		crc_p = *(UINT *)&asdm_page[j];
		if(crc_k != crc_p)
		{
			*a_erg = ARC_ERR_KOMTAB;
			*dae_err = i;	
			return c_erg;
		}	
	}
	
	if(ktleng > 255)
	{
		c_erg = read_data_block(kt2_adr, asdm_page, timeout_max);	// 2. Komtab-Vergleichs-Page lesen
		if(c_erg != CRD_OK)
		{
			return c_erg;																						// Card Lesefehler			
		}
		
		for(i=256, j=0; i < ktleng; i++, j+=2 )
		{
			gnum = KomTab[i].gnum;
			pnum = KomTab[i].pnum;
			memcpy(&buffer[0], Pgruppe[gnum].grkz, 4);								// 4  Byte ParameterGruppe 
			memcpy(&buffer[4], Pgruppe[gnum].Param[pnum].bez, 16);		// 16 Byte ParameterBezeichnung
			crc_k = Checksum(buffer, 20);															// Es wird eine CRC-Summe gebildet	
			crc_p = *(UINT *)&asdm_page[j];
			if(crc_k != crc_p)
			{
				*a_erg = ARC_ERR_KOMTAB;
				*dae_err = i;	
				return c_erg;
			}	
		}
	}						
		 
	return c_erg;
}		 
		
//------------------------------------------------------------
// Komtab-Speicherung
//------------------------------------------------------------
// Aus der ersten Zeile eines Parameters (Gruppe und Bezeichnung) wird ein CRC-Code für Vergleichszwecke erstellt.  
CRD_ERG Save_Komtab(UINT ktleng)
{
	CRD_ERG c_erg = 0;

	char buffer[20] = {0};
	char gnum = 0;
	char pnum = 0;
	
	UINT i = 0;
	UINT j = 0;
	UINT idx = 0;
	UINT crc_k = 0;
	
	ULONG kt1_adr = SDM_KOMTA1_ADR;															// Adresse für Komtabspeicherung DAE   0 - DAE 255
	ULONG kt2_adr = SDM_KOMTA2_ADR;															// Adresse für Komtabspeicherung DAE 256 - DAE 511

	memset(asdm_page, i, 512);																	// Page löschen
	
	if(ktleng < 256)
		idx = ktleng;
	else
	  idx = 256;	
		
	for(i=0, j=0; i < idx; i++, j+=2)
	{
		gnum = KomTab[i].gnum;
		pnum = KomTab[i].pnum;
		memcpy(&buffer[0], Pgruppe[gnum].grkz, 4);								// 4  Byte ParameterGruppe 
		memcpy(&buffer[4], Pgruppe[gnum].Param[pnum].bez, 16);		// 16 Byte ParameterBezeichnung
		crc_k = Checksum(buffer, 20);															// Es wird eine CRC-Summe gebildet	
		*(UINT *)&asdm_page[j] = crc_k;
	}
	
	c_erg = write_data_block(kt1_adr, asdm_page, timeout_max);	// 1. Komtab-Vergleichs-Page schreiben
	if(c_erg != CRD_OK)
	{
		return c_erg;																							// Card Schreibfehler			
	}					
	
	if(ktleng > 255)
	{
		i = 0;
		memset(asdm_page, i, 512);
		
		for(i=256, j=0; i < ktleng; i++, j+=2 )
		{
			gnum = KomTab[i].gnum;
			pnum = KomTab[i].pnum;
			memcpy(&buffer[0], Pgruppe[gnum].grkz, 4);								// 4  Byte ParameterGruppe 
			memcpy(&buffer[4], Pgruppe[gnum].Param[pnum].bez, 16);		// 16 Byte ParameterBezeichnung
			crc_k = Checksum(buffer, 20);															// Es wird eine CRC-Summe gebildet	
			*(UINT *)&asdm_page[j] = crc_k;
		}
		
		c_erg = write_data_block(kt2_adr, asdm_page, timeout_max);	// 2. Komtab-Vergleichs-Page schreiben
		if(c_erg != CRD_OK)
		{
			return c_erg;																							// Card Schreibfehler	
		}			
	}					
		
	return c_erg;
}	

//------------------------------------------------------------
// Expand-Informationstabelle prüfen
//------------------------------------------------------------
CRD_ERG Verify_ExpInfotab(ARC_ERG *a_erg, int *dae_err)
{
	CRD_ERG c_erg = 0;
	UINT i = 0;
	ULONG exp_adr = SDM_MAXEXP_ADR;
	
	// Vorbelegung der return Parameter
	*a_erg = ARC_OK;
	*dae_err = -1; // ungültige DAE-nummer

	c_erg = read_data_block(exp_adr, asdm_page, timeout_max);	// ExpandInfotab lesen
	if(c_erg != CRD_OK)
	{
		return c_erg;																						// Card Lesefehler			
	}					
	
	for(i = 0; i < 512; i++)
	{
		if(asdm_page[i] != expinfo_tab[i].max_exp)
		{
			*a_erg = ARC_ERR_MAX_EXP;
			*dae_err = i;	
			return c_erg;
		}
	}	
	return c_erg;
}	

//------------------------------------------------------------
// Expand-Informationstabelle speichern
//------------------------------------------------------------
// dient zur Überprüfung auf max_exp Änderung
CRD_ERG Save_ExpInfotab(void)
{
	CRD_ERG c_erg = 0;
	UINT i = 0;
	ULONG exp_adr = SDM_MAXEXP_ADR;
	
	memset(asdm_page, i, 512);																	// Page löschen

	for(i = 0; i < 512; i++)
		asdm_page[i] = expinfo_tab[i].max_exp;

	c_erg = write_data_block(exp_adr, asdm_page, timeout_max);	// 
	if(c_erg != CRD_OK)
	{
		return c_erg;																							// Card Schreibfehler	
	}			
	return c_erg;
}	

//------------------------------------------------------------
// Speichern von Parametern auf der SD-Card
//------------------------------------------------------------
CRD_ERG save_data(UINT dae, char exp_idx, char exp_dae, char *folge, ARC_ERG *a_erg)
{
	CRD_ERG c_erg = 0;

	char bicret = 0;
	char bt_anz = 0;
	char lz_exp = 0;
	ULONG start = 0;
	ULONG block = 0;
	UINT i			= 0;
	
	UINT akt_DP = 0;
	char num_DP = 0;
	char off_DP = 0;
	char akt_VP = 0;
	char num_VP = 0;
	char num_HP = 0;
	char ovr_HP = 0;
	
	UINT idx_DP 	= 0;
	UINT max_DP   = (UINT)SDM_DP_ANZ;			// Anzahl der Datenpages
	UINT vp_block = (UINT)SDM_OFFS_DP;		// 1 Hauptpage + 16 Verweispages
	
// Struktur für SD-Karte: Aufbau des Bereiches für ein DAE
// nur Information !! Die Struktur kann der Compiler nicht verarbeiten, weil die SD-Card-Adressen den Speicherbereich des Prozessors überschreiten.  
//typedef struct sdm {
//	pages hp[1];				// Hauptpage
//	pages vp[16];				// Verweispages
//	pages dp[2031];			// Datenpages
//} dae_page;		

	*folge = 0;																	// init: keine zusätzliche Speicherungen (Verweispage usw.)		
	*a_erg = ARC_OK;

	// Speicheradressen ermitteln
	if(exp_idx == 0)														// Normalparameter
	{
		ferro_adr = DAE_INFO_ADR  + DAE_INFO_LENG * dae;			
		start = (ULONG)SDM_START_ADR;
		block = (ULONG)SDM_BLOCK_DAE;
		hp_adr  = start + block * (ULONG)dae;
	}
	else																				// Expandparameter
	{
		ferro_adr = (UINT )&(exp_ferro_tab[exp_idx-1][exp_dae]);		
		block = (ULONG)SDM_BLOCK_DAE;
		hp_adr  	= exp_sdm_tab[exp_idx-1] + block * (ULONG)exp_dae;
		lz_exp    = exp_dae + 1;
	}
	
	// Page-Informationen vom Ferro-Ram lesen
	bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICRD);
	if(bicret == 0)	// Fehler
	{
		*a_erg = ARC_ERR_FERRO_READ;											// Ferro-Lesefehler
		 c_erg = CRD_ERR_OTHER;
		 return c_erg;
	}

	// Ermittlung der aktuellen Verweis- und Datenpage-Adresse
	akt_DP = dae_info[0].akt_DP;												// aktuelle Datenpage-Nr
	akt_VP = dae_info[0].akt_VP;												// aktuelle Verweispage-Nr
	if(akt_DP == 0 || akt_VP == 0)
	{
		*a_erg = ARC_ERR_FERRO_INIT; 											// Initialisierungsfehler
		 c_erg = CRD_ERR_OTHER;
		 return c_erg;
	}

	dp_adr = hp_adr + vp_block + (ULONG)(akt_DP - 1) * 512;		// Adresse der Datenpage   entspr. Datenpage-Nr

	// Informationsbuffer füllen
	*(ULONG *)(asdm_buffer) = asdm_time;								// Zeitstempel ablegen
	bt_anz = LzKonv(&asdm_buffer[4], dae, 0, lz_exp);		// aktuellen Wert holen und ablegen
	if(bt_anz == 0)
	{
		*a_erg = ARC_ERR_LZKONV; 													// Konvertierungsfehler
		 c_erg = CRD_ERR_OTHER;
		 return c_erg;
	}

	// Speicherplatz auf der Datenpage für den Neueintrag bestimmen 
	//       Anzahl der Einträge * Offset 
	num_DP = dae_info[0].num_DP;																		// Anzahl der Einträge in der Datenpage
	off_DP = dae_info[0].off_DP;																		// Offset der Einträge
	num_VP = dae_info[0].num_VP;
	num_HP = dae_info[0].num_HP;
	ovr_HP = dae_info[0].ovr_HP;

	idx_DP = (UINT)num_DP	 * off_DP;
	if( (idx_DP + off_DP) > 512)																		// Überlauf testen
	{	
		if(++akt_DP > max_DP)
		{
			// keine Datenpage mehr vorhanden, zurück auf LOS
			akt_DP = 1;																									// 1 = akt. Datenpage
			num_DP = 0;																									// 0 = Zeiger auf Datenpage-Anfang
			akt_VP = 1;																									// 1 = akt. Verweispage
			num_VP = 0;																									// 0 = Zeiger auf Verweispage-Anfang
			num_HP = 0;																									// 0 = Zeiger auf Hauptpage-Anfang    
			ovr_HP += 1;																								// Anzahl der Datenpage-Überläufe (Ringpuffer)
			dp_adr = hp_adr + vp_block + (ULONG)(akt_DP - 1) * 512;			// Adresse der Datenpage   entspr. Datenpage-Nr
		}
		else	// nächste Datenpage
		{
			dp_adr = hp_adr + vp_block + (ULONG)(akt_DP - 1) * 512;						// Adresse der Datenpage   entspr. Datenpage-Nr
			num_DP = 0;																									// Noch keine Einträge
		}
	}					
	
	if(num_DP == 0)																									// Noch keine Einträge ?
	{
		memset(asdm_page, i, 512);																		// Pagebuffer löschen
		memcpy(&asdm_page[0], &asdm_buffer[0], off_DP);								// Zeitstempel und Wert auf den ersten Platz eintragen
	}	
	else
	{
		// Datenpage lesen
		c_erg = read_data_block(dp_adr, asdm_page, timeout_max);
		if(c_erg != CRD_OK)
		{
			return c_erg;																								// Card Lesefehler			
		}
		
		memcpy(&asdm_page[idx_DP], &asdm_buffer[0], off_DP);					// Zeitstempel und Wert auf den berechneten Platz eintragen
	}						
			
	// Datenpage schreiben
	c_erg = write_data_block(dp_adr, asdm_page, timeout_max);
	if(c_erg != CRD_OK)
	{
		return c_erg;																									// Card Schreibfehler			
	}					
	
	if(num_DP == 0)																									// wurde neue Datenpage beschrieben ?
	{
		*folge = 1;																										// ja, Zusätzliche Speicherungen (Verweispage usw.) aktivieren
	}	
	num_DP++;																												// Anzahl der Einträge in der Datenpage erhöhen

	// Ferro-Info aktualisieren
	dae_info[0].akt_DP = akt_DP;																		// Datenpage-Nr. aktualisieren
	dae_info[0].num_DP = num_DP;
	dae_info[0].akt_VP = akt_VP;
	dae_info[0].num_VP = num_VP;
	dae_info[0].num_HP = num_HP;
	dae_info[0].ovr_HP = ovr_HP;
	
	bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICWR);
	if(bicret == 0)	// Fehler
	{
		*a_erg = ARC_ERR_FERRO_WRITE;																	// Ferro-Schreibfehler
		 c_erg = CRD_ERR_OTHER;
		 return c_erg;
	}
	
	return c_erg;
}	


//-----------------------------------------------------------------------------------------------------------
// Speichern von Folge-Parametern auf der SD-Card
//-----------------------------------------------------------------------------------------------------------
// Eine neue Datenpage wurde beschrieben: Eintragung des Zeitstempels in einer Verweispage und ggf. Hauptpage
CRD_ERG save_folge(ARC_ERG *a_erg, ULONG zeit)
{
	CRD_ERG c_erg = 0;
	char bicret = 0;
	UINT i = 0;

	char akt_VP = 0;
	char num_VP = 0;
	char off_VP = 4;					// 4 Byte Zeitstempel
	UINT idx_VP = 0;

	char ovr_HP = 0;
	char num_HP = 0;
	char off_HP = 4;					// 4 Byte Zeitstempel
	UINT idx_HP = 0;
	
	UINT akt_DP = 0;
	UINT max_DP = 0;					//  letzte möglich Datenpage-Nr. ind der aktuellen Verweispage-Nr.
	
	ULONG *time;
	char hp_flag = 0;
	
	*a_erg = ARC_OK;
	time = &zeit;
	
	// noch aktuell: Ferro-Infos dae_info[0] und ferro_adr
	// noch aktuell: Adresse der Hauptpage hp_adr

	akt_DP = dae_info[0].akt_DP;																		// aktuelle Datenpage-Nr. (sie ist schon von save_data(...) incrementiert worden)
	akt_VP = dae_info[0].akt_VP;																		// aktuelle Verweispage-Nr
	num_VP = dae_info[0].num_VP;																		// Anzahl der Einträge in der Verweispage
	num_HP = dae_info[0].num_HP;																		// Anzahl der Einträge in der Hauptpage
	ovr_HP = dae_info[0].ovr_HP;																		// Anzahl der Datenpage-Überläufe (Ringpuffer)
	
	// Indexberechnung 
	//-------- neu ------------------	
	max_DP = akt_VP * 128;																					// letzte möglich Datenpage-Nr. in der aktuellen Verweispage-Nr.
	if(akt_DP > max_DP)
	{	// Überlauf, nächste VP
		akt_VP++;
		num_VP = 0;
		idx_VP = 0;
	}
	else
	{
		idx_VP = ( (akt_DP - 1) % 128 ) * off_VP;												// Index (neu): determinierter Platz in der Verweispage, abgeleitet von der Datenpage-Nr.
	}

	vp_adr = hp_adr + (ULONG)akt_VP * 512;														// Adresse der Verweispage entspr. Verweispage-Nr.
	
	//---------- alt ----------------
//	idx_VP = (UINT)num_VP	 * off_VP;																// Index (alt): nächster freier Platz in der Verweispage
//	if( (idx_VP + off_VP) <= 512)																		// Verweispage-Überlauf ?
//	{																																// nein
//		vp_adr = hp_adr + (ULONG)akt_VP * 512;												// Adresse der Verweispage entspr. Verweispage-Nr.
//	}
//	else	// Überlauf, nächste VP
//	{
//		akt_VP++;
//		num_VP = 0;
//		idx_VP = 0;
//		vp_adr = hp_adr + (ULONG)akt_VP * 512;												// Adresse der Verweispage entspr. Verweispage-Nr.
//	}
	//--------------
	
	if(num_VP == 0)																									// noch keine Einträge ?
	{
		memset(asdm_page, i, 512);																		// Pagebuffer löschen
		memcpy(&asdm_page[0], time, 4);																// Zeitstempel auf den ersten Platz im Pagebuffer eintragen
	}
	else
	{			 	
		// Verweispage lesen
		c_erg = read_data_block(vp_adr, asdm_page, timeout_max);
		if(c_erg != CRD_OK)
		{
			return c_erg;																								// Card Lesefehler			
		}					
		memcpy(&asdm_page[idx_VP], time, 4);													// Zeitstempel auf den berechneten Platz eintragen		 	
	}	

	// Verweispage schreiben
	c_erg = write_data_block(vp_adr, asdm_page, timeout_max);
	if(c_erg != CRD_OK)
	{
		return c_erg;																									// Card Schreibfehler			
	}					
	
	if(num_VP == 0)															// Anzahl der Einträge noch 0 ?
	{
		hp_flag = 1;															// Eintrag in Hauptpage vornehmen
	}
	num_VP++;																		// Anzahl der Einträge erhöhen
			
	
	// Hauptpage aktualisieren, d.h. den ersten Eintrag einer Verweispage sichern
	if(hp_flag == 1)
	{
		// Hauptpage lesen
		c_erg = read_data_block(hp_adr, asdm_page, timeout_max);
		if(c_erg != CRD_OK)
		{
			return c_erg;																								// Card Lesefehler			
		}					

		idx_HP = (UINT)num_HP	 * off_HP;																// Index: nächster freier Platz
		if(num_HP == 0)																									// noch keine Einträge ?
		{
			if(ovr_HP == 0)																								// noch keine Überrundung
				memset(asdm_page, i, 512);																	// Pagebuffer löschen

			memcpy(&asdm_page[0], time, 4);																// Zeitstempel auf den ersten Platz im Pagebuffer eintragen
		}
		else
		{
			memcpy(&asdm_page[idx_HP], time, 4);													// Zeitstempel auf den berechneten Platz eintragen
		}				 	
		
		// Hauptpage schreiben
		c_erg = write_data_block(hp_adr, asdm_page, timeout_max);
		if(c_erg != CRD_OK)
		{
			return c_erg;	// Schreibfehler			
		}					
		num_HP++;																												// Anzahl der Einträge in der Hauptpage erhöhen, Überlauf wird in save_data behandelt 
	}	
	 
	// Ferro-Info aktualisieren
	dae_info[0].akt_VP = akt_VP;
	dae_info[0].num_VP = num_VP;
	dae_info[0].num_HP = num_HP;

	bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICWR);
	if(bicret == 0)	// Fehler
	{
		*a_erg = ARC_ERR_FERRO_WRITE;																	// Ferro-Schreibfehler
		 c_erg = CRD_ERR_OTHER;
		 return c_erg;
	}

	return c_erg;
}


//------------------------------------------------------------
// Ferro-Bereich zum Alloziieren von Expandparametern
//------------------------------------------------------------
void * ferro_malloc(char size, char max)
{
	UINT adr, len;
	
	adr = ferro_mnext;
	len = size * max;
	
	if( (size + len) > ferro_msize) 
		return 0;
		
	ferro_mnext = adr + len;
	
	return (void *)adr;
}
		 
//------------------------------------------------------------
// SD-Card-Memory-Bereich zum Alloziieren von Expandparametern
//------------------------------------------------------------
ULONG sdm_malloc(ULONG size, char max)
{
	ULONG adr, len;
	
	adr = sdm_mnext;
	len = size * max;
	
	if( (adr + len) > sdm_msize) 
		return 0;
		
	sdm_mnext = adr + len;
	
	return (ULONG)adr;
}


//--------------------------------------------------------------------
// Karte initialisieren
//--------------------------------------------------------------------
CRD_ERG init_Card(DWORD timeout_max)
{
	BYTE ret;
	CRD_ERG	 c_erg = 0;
	
	#if ZEIT_MESSUNG == 1
	read_zeit = Zeitmess(1);
	#endif

	setLED(0, TRUE);
	spibus_flag = 1;
	c_erg = init_Card_Reg(timeout_max);
	selectCard(0, FALSE);
	spibus_flag = 0;
	setLED(0, FALSE);

	#if ZEIT_MESSUNG == 1
	read_zeit = Zeitmess(0);
	#endif

	return c_erg;
}	


BYTE init_Card_Reg(DWORD timeout_max)
{
	int timeout;
	BYTE ret, ret2;
	DWORD r7, r3;
  cardType = CARD_UNKOWN;
		
	// Karte in den Idle-Modus versetzen
	selectCard(0, FALSE);
	waitForMemCard();
	selectCard(0, TRUE);
	
	ret = sendCommand2(CMD0_RESETCARD, 0, timeout_max);							/* send CMD 0*/
  if(ret!=R1_IN_IDLE_STATE) return(CRD_ERR_CMD0); 								//Card is in Idle-Mode

  ret = sendCommand(CMD8_IF_COND,0 ,0 , 1, 0xaa, timeout_max/2);	/* send CMD 8 to verify the supplied voltage */
  r7 = getR7();
  if (ret == R1_IN_IDLE_STATE) { 																	/* supported command */    
    if( (r7&0xff)!=0xaa || (r7&0xf00)!=0x100 ) return(CRD_ERR_CMD8); //Card is in Idle-Mode
  } 

  ret = sendCommand(CMD58_GET_OCR,0,0,0,0, timeout_max);					/* read OCR*/
  r3 = getR3();

	if (ret==R1_IN_IDLE_STATE)
	{
	// Karte initialisieren
		timeout = timeout_max;
	  do
	  {
    	ret = sendCommand(CMD55_APP,0,0,0,0, timeout_max);					/* send CMD 55, das "Prefix" für ein ACMD */      
      if( (ret & 0xfe) !=0 ) {
      	return(CRD_ERR_CMD55); 																		/* Card isn't in Idle-Mode */
      }
      ret = sendCommand(ACMD41_OP_COND,0x40,0,0,0, timeout_max); 	/* send ACMD 41 */
    }while (ret==R1_IN_IDLE_STATE && timeout--);
			
    if (!timeout)
    	return CRD_ERR_ACMD41;
    	
    ret2 = sendCommand(CMD58_GET_OCR,0,0,0,0, timeout_max);				/* read OCR*/
    if (ret2) return(CRD_ERR_OCR1); 															/* Card is in Idle-Mode */      
    r3 = getR3();

    /* Verify the voltage range */
    if ( (r3 & 0xff8000) != 0xff8000 ) return CRD_ERR_OCR1;
	}
	else
		return CRD_ERR_OCR1;
		
//#ifdef DISABLE_CRC_CHECK
//    ret = sendCommand(CMD59_CRC_ONOFF,0,0,0,0, timeout_max);	/* CRC checking off */
//#else
//    ret = sendCommand(CMD59_CRC_ONOFF,0,0,0,1, timeout_max);	/* CRC checking on */
//#endif
//  if (ret) return CRD_ERR_CMD59;
		
  // Card-Type
  if (r3&0x40000000) { /* SD-HC card */
    cardType = CRD_HC_CARD;
    //pCard->HCCard = TRUE;
  } else { /* SD standard card */
    cardType = CRD_STD_CARD;
  }
  if ( cardType == CARD_UNKOWN ) return CRD_ERR_UNKOWN_CARD;
  
  // SD Status lesen (512 Bit = 64 Byte)
  // (analog zu getCRDStatusExt in CRD.c)
  ret = sendCommand(CMD55_APP,0,0,0,0, timeout_max);						/* Application-Specific Command:  send command 55 */
  if (ret) return(CRD_ERR_CMD55);
  ret = sendCommand(ACMD13_SEND_STATUS,0,0,0,0, timeout_max);		/* send command 13 */
  if (ret) return(CRD_ERR_CMD13);
  ret = receiveByte();
  if(!waitForStartByte(timeout_max))
  	return CRD_ERR_TIMEOUT;
  readBytes(asdm_buffer, 64);

	ret =	(BYTE)getValue(asdm_buffer, 64, 1, 440, 447);	// 0x00=Class0, 0x01= Class2, 0x02= Class4, 0x03= Class6, 0x04= Class10
	speedClass = ret * 2;
	if(ret == 4)
		speedClass = 10;
		
	performanceMove = (BYTE)getValue(asdm_buffer, 64, 1, 432, 439);	// bis Class 6 in [MB/sec], Class 10 = 0 (ignored)	

	return CRD_OK;
}		
	

//--------------------------------------------------------------------
// Einzelnen Block lesen
//--------------------------------------------------------------------
CRD_ERG read_data_block(DWORD adr, BYTE *pData, DWORD timeout_max)
{
	CRD_ERG	 c_erg = 0;
	
	if(cardType == CRD_HC_CARD)
		adr = adr / 0x200;				// HC-CARD Sektornummer

	setLED(0, TRUE);
	selectCard(0, TRUE);
	spibus_flag = 1;						// spibus-Funktion ist aktiv 
	c_erg = read_512_data_block(adr, pData, timeout_max);
	spibus_flag = 0;						// spibus-Funktion ist inaktiv 
	selectCard(0, FALSE);
	setLED(0, FALSE);
	
	return c_erg;
}	

BYTE read_512_data_block(DWORD adr, BYTE *pData, DWORD timeout_max)
{
	DWORD len = 512;
	
  if ( sendCommand2(CMD17_READ_SINGLE_BLOCK, adr, timeout_max) != 0 )
  	return CRD_ERR_CMD17;	/* read one data block */
  
  if(!waitForStartByte(timeout_max))
  	return CRD_ERR_CMD19;
  
  readBytes(pData, len);
  return checkCRC16(0, pData, len) ? CRD_OK : CRD_ERR_SBLOCK_CRC;

}
	
//--------------------------------------------------------------------
// Einzelnen Block schreiben
//--------------------------------------------------------------------
CRD_ERG write_data_block(DWORD adr, BYTE *pData, DWORD timeout_max)
{
	CRD_ERG	 c_erg = 0;

	if(cardType == CRD_HC_CARD)
		adr = adr / 0x200;				// HC-CARD Sektornummer
	
	#if ZEIT_MESSUNG == 1
  write_zeit = Zeitmess(1);
  #endif
  
	setLED(0, TRUE);
	selectCard(0, TRUE);
	spibus_flag = 1;						// spibus-Funktion ist aktiv 
	c_erg = write_512_data_block(adr, pData, timeout_max);
	spibus_flag = 0;						// spibus-Funktion ist inaktiv 
	selectCard(0, FALSE);
	setLED(0, FALSE);

	#if ZEIT_MESSUNG == 1
  write_zeit = Zeitmess(0);
  #endif
	
	return c_erg;
}	


BYTE write_512_data_block(DWORD adr, BYTE *pData, DWORD timeout_max)
{
	DWORD len = 512;
  BYTE ret, err=CRD_OK;
  WORD crc16=0;
  DWORD timeout;
  
  if ( sendCommand2(CMD24_WRITE_BLOCK, adr, timeout_max) != 0 )
  	return CRD_ERR_CMD24;	

  sendByte(0xFE);									 /*  start byte  */
  writeBytes(pData, len, &crc16);
  sendByte((BYTE) (crc16 >> 8) );	 /* send crc check sum */
  sendByte((BYTE) crc16);
  timeout = timeout_max;
  
  do
  {
		if( (timeout % HWDOG_CNT) == 0)
			hwdog(1);
    ret = receiveByte();
  } while ( (ret & 0x11) != 0x01 && timeout--);
  
  if ( (ret & 0x1f) != 0x05 || !timeout)
  	err = CRD_ERR_WRITE;  
  
  if (err != CRD_OK)
  {
    sendCommand(CMD12_STOP_TRANSMISSION,0,0,0,0, timeout_max);
    return err;
  }
  
  return waitForNotBusy(timeout_max) ? err : CRD_ERR_BUSY;
}  
  


/*-----------------------------------------------------------------------*/
/*	geerbt von CRD.c																										 */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
static DWORD cutBits(BYTE value, BYTE start, BYTE end)
{
  BYTE val=0xff;
  val>>=(7-end);
  value &= val;
  return (value >> start);
}
/*-----------------------------------------------------------------------*/
static DWORD getValue(BYTE* data, BYTE cnt, BYTE rev , UINT start, UINT end)
{
  DWORD value = 0, tmp=0;
  BYTE startpos, endpos, vrest=0, brest=0;
  BYTE i, vv, bit=0;

  startpos = start / 8;
  endpos   = end / 8;
  vrest = start % 8;
  brest = end % 8;
  for (i=startpos; i <= endpos ; ++i) {
    vv = data[(rev ? (cnt-i-1) : i)];
    if ( i == startpos && i == endpos ) {
      value = cutBits(vv, vrest, brest);
      break;
    } else if ( i == startpos ) {
      value = cutBits(vv, vrest, 7);
      bit = 7-vrest+1;      
      continue;
    } else if (i == endpos) {
      tmp = cutBits(vv, 0, brest);
      value |= (tmp << bit);
      bit += brest+1;
    } else {
      value |= (((DWORD)vv) << bit);
      bit += 8;
    }
  }  
  return value;
}

/* gets R7 back and calculates NCR  */
static DWORD getR7(void)
{
  INT i;
  DWORD ret, data=0;
  for (i=24; i >=0; i-=8) {
    ret = receiveByte();
    data |= ( ret << i );
  }
  return data;
}
/*-----------------------------------------------------------------------*/
static DWORD getR3(void)
{
  return getR7();
}

/*-----------------------------------------------------------------------*/
static VOID waitForMemCard()
{
  int j;  
  for(j = 0; j < 15; j++) sendByte(0xff); /* Send out 15x8 clocks */
}

/*--------------------------------------------------------------------------
 Function: send command to the memory card

 Parameter:
            (I) cmd        : command   Bit 47-40 
            (I) arg3       : argument  Bit 39-32 
            (I) arg2       : argument  Bit 31-24 
            (I) arg1       : argument  Bit 23-16 
            (I) arg0       : argument  Bit 15-08 

 return   : command respond R1
 ---------------------------------------------------------------------------*/
static BYTE sendCommand(BYTE cmd, BYTE arg3, BYTE arg2, BYTE arg1, BYTE arg0, DWORD timeout)
{
  BYTE ret;

  waitForCard(1);

  cmd = (cmd&0x3F)|0x40;

  sendByte(cmd);  /* Bit 47-40 */
  sendByte(arg3); /* Bit 39-32 */
  sendByte(arg2); /* Bit 31-24 */
  sendByte(arg1); /* Bit 23-16 */
  sendByte(arg0); /* Bit 15-08 */
	sendByte( (crc7Ex(cmd, arg3, arg2, arg1, arg0) << 1) | 0x01 );	 /* Bit7-0 */
  
  for(ret=0xff; (ret&STARTBIT_MASK) && timeout ; --timeout)
  { ret=receiveByte();   /* waiting for response */    
  	if( (timeout % HWDOG_CNT) == 0)
  		hwdog(1);
  }	
  return(ret);
}

/*-----------------------------------------------------------------------*/
static BYTE sendCommand2(BYTE cmd, DWORD adr, DWORD timeout)
{
  return sendCommand(cmd, (BYTE)(adr >> 24), (BYTE)(adr >> 16), (BYTE)(adr >> 8), (BYTE)adr, timeout);
}


/*-----------------------------------------------------------------------*/
static void waitForCard(WORD len)
{
  for (;len; --len) sendByte(0xff);
}

/*-----------------------------------------------------------------------*/
static BOOL waitForStartByte(DWORD timeout)
{
  for(; (receiveByte()!=0xFE) && timeout ; --timeout ); /* wait for startbyte 0xFE */  
  {
  	if( (timeout % HWDOG_CNT) == 0)
  		hwdog(1);
  }		

  return timeout!=0;
}

/*-----------------------------------------------------------------------*/
static void readBytes(BYTE *pData, WORD len)
{
  INT i;
  for(i = 0; i < len; i++) {
    pData[i]=receiveByte();
  }
}

/*-----------------------------------------------------------------------*/
static void writeBytes(BYTE *pData, WORD len, WORD *pcrc)
{
  WORD i;

  *pcrc = 0;
  for(i = 0; i < len; i++) {
    sendByte(pData[i]); 

  }
}

/* ---------------------------------------------------------------------- */
static BYTE crc7Ex(BYTE par0, BYTE par1, BYTE par2, BYTE par3, BYTE par4)
{
  BYTE crc;
  crc = 0;
  crc = crc7_tb[(crc << 1) ^ par0];
  crc = crc7_tb[(crc << 1) ^ par1];
  crc = crc7_tb[(crc << 1) ^ par2];
  crc = crc7_tb[(crc << 1) ^ par3];
  crc = crc7_tb[(crc << 1) ^ par4];
  return crc;
}

/*-----------------------------------------------------------------------*/
static BOOL checkCRC16(char num, BYTE *pdata, WORD len)
{
  WORD crc,calc;
  crc = receiveByte();
  crc <<= 8;
  crc |= receiveByte();

#ifdef DISABLE_CRC_CHECK
  return TRUE;
#else
  calc = crc16Fkt(0, pdata, len);
  return calc == crc;
#endif
}
/* ---------------------------------------------------------------------- */
static UINT crc16Fkt(UINT crc, const BYTE *buffer, UINT len)
{
  while (len--) {
    crc= (((crc)<<8)^crc16_tbl[(((crc)>>8)^(*buffer++))&0xff])&0xffff;
  }
  return crc;
}

/*-----------------------------------------------------------------------*/
static BOOL waitForNotBusy(DWORD timeout)
{
  waitForCard(1);
  while( !receiveByte() && timeout--  ) /* wait for not Busy */
  {	
  	if( (timeout % HWDOG_CNT) == 0)
  		hwdog(1);
	}
  return timeout!=0;
}

/*-----------------------------------------------------------------------*/
/*	geerbt von ICRD.c																													 */
/*-----------------------------------------------------------------------*/
/* Receive one Byte */
static BYTE receiveByte()
{
  /* must be implemented */
	char i,	rbyte;
	
	rbyte	=	0; 
	for(i	=	0; i < 8;	i++)// das Byte wird Bitweise nacheinander Empfangen MSB First
	{
		SPI_CLK = 0;				// Clock Impuls Low

		rbyte <<= 1;
		if(SPI_IN)
			rbyte |=	0x01;

		SPI_CLK = 1;				// Clock Impuls High
	}
	return (rbyte);
}

/*-----------------------------------------------------------------------*/
/* Send one Byte */
static BYTE sendByte(BYTE data)
{
  /* must be implemented */
	char i, mask;
	
	mask = 0x80;
	for(i	=	0;i	<	8; i++)	//das Byte wird Bitweise nacheinander Gesendet MSB First
	{
		if(data & mask)
			SPI_OUT	=	1;
		else
			SPI_OUT	=	0;		

		SPI_CLK = 0;				// Clock Impuls Low
		mask	>>=	1;
		SPI_CLK = 1;				// Clock Impuls High
	}
	SPI_OUT	=	1;					//setzt Output wieder auf High	
}

/*-----------------------------------------------------------------------*/
/* Select memory card: setup chip select signal */
static BOOL selectCard(BYTE no, BOOL select)
{
  /* must be implemented */
  SPI_CS = select ? FALSE:TRUE;
}

/*-----------------------------------------------------------------------*/
static BYTE setLED(BYTE no, BOOL value)
{
  /* optional: can be implemented */
  LCD_BL = value ? 1 : 0;
  return 0;
}

/*-----------------------------------------------------------------------*/
//BOOL getLED(BYTE no)
//{
//  /* optional: can be implemented */
//  return LCD_BL ? FALSE:TRUE;
//}
//

/*-----------------------------------------------------------------------*/
/*	eigene UP																														 */
/*-----------------------------------------------------------------------*/
// Generierung von Texten für die LCD-Anzeige aus dem ArchivSDM-Ergebnis-(Return)-Code
void built_a_erg_text(ARC_ERG ret, char *ret_text)
{
	switch(ret)
	{
		case ARC_OK:														// = 0,
			strcpy(ret_text, "BEREIT         ");
			break;
		case ARC_ERR_CARD:        							// = 1,
			strcpy(ret_text, "->SDM-CARD-STAT");
			break;
		case ARC_ERR_CARD_HEAP:   							// = 2,
			strcpy(ret_text, "SDM-HEAP-ERROR ");
			break;
		case ARC_ERR_SDM_INITKZ:								// = 3
			strcpy(ret_text, "SDM-INITKZ-ERR.");
			break;
		case ARC_ERR_FERRO_HEAP:  							// = 4,
			strcpy(ret_text, "FER-HEAP-ERROR ");
			break;
		case ARC_ERR_FERRO_INIT: 								// = 5,
			strcpy(ret_text, "FER-INIT-ERROR ");
			break;
		case ARC_ERR_FERRO_INITKZ: 							// = 6,
			strcpy(ret_text, "FER-INITKZ-ERR.");
			break;
		case ARC_ERR_FERRO_READ:  							// = 7,
			strcpy(ret_text, "FER-READ-ERROR ");
			break;
		case ARC_ERR_FERRO_WRITE: 							// = 8,
			strcpy(ret_text, "FER-WRITE-ERROR");
			break;
		case ARC_ERR_LZKONV:      							// = 9,
			strcpy(ret_text, "LZ-KONV-ERROR  ");
			break;
		case ARC_ERR_KOMTAB:      							// = 10,
			strcpy(ret_text, "KOMTAB-ERROR   ");
			break;
		case ARC_ERR_MAX_EXP:      							// = 11,
			strcpy(ret_text, "MAX-EXP-ERROR  ");
			break;
		case ARC_RUN:      											// = 12,
			strcpy(ret_text, "RUN            ");
			break;
		case ARC_PAUSE:      										// = 13,
			strcpy(ret_text, "PAUSE          ");
			break;
		case ARC_STOP:      										// = 14,
			strcpy(ret_text, "USER-STOP      ");
			break;
		case ARC_ERR_MONITOR:      							// = 15,
			strcpy(ret_text, "MONITORING ERR.");
			break;
		default:
			strcpy(ret_text, "ERROR:         ");
			ctoa(&ret_text[6], ret);
			break;
	}
}			

// Generierung von Texten für die LCD-Anzeige aus dem SDM-CARD-Ergebnis-(Return)-Code
void built_c_erg_text(CRD_ERG ret, char *ret_text)
{
	switch(ret)
	{
		case CRD_OK:              							// = 0x00,
			strcpy(ret_text, "BEREIT         ");
			break;
		case CRD_ERR_CMD0:
		case CRD_ERR_OCR1:        							// = 0x02,
			strcpy(ret_text, "NICHT GESTECKT ");
			break;
		case CRD_ERR_CMD55:       							// = 0x04,
			strcpy(ret_text, "INIT-ERR-CMD55 ");
			break;
		case CRD_ERR_ACMD41:      							// = 0x05,
			strcpy(ret_text, "INIT-ERR-CMD41 ");
			break;
		case CRD_ERR_CMD19:       							// = 0x0f,
			strcpy(ret_text, "WAIT-ERR-CMD19 ");
			break;
		case CRD_ERR_SBLOCK_CRC:  							// = 0x10,
			strcpy(ret_text, "READ-ERR-CRC16 ");
			break;
		case CRD_ERR_WRITE:       							// = 0x12,
			strcpy(ret_text, "WRITE-ERR-ACK  ");
			break;
		case CRD_ERR_BUSY:        							// = 0x13,
			strcpy(ret_text, "WRITE-ERR-BUSY ");
			break;
		case CRD_ERR_CMD24:       							// = 0x14,
			strcpy(ret_text, "WRITE-ERR-CMD24");
			break;
		case CRD_ERR_CMD17:       							// = 0x1f,
			strcpy(ret_text, "READ-ERR-CMD17 ");
			break;
		case CRD_ERR_OTHER:       							// = 0x30,
			strcpy(ret_text, "->SDM-ARCH-STAT");
			break;
		case CRD_RUN:				       							// = 0x31,
			strcpy(ret_text, "RUN            ");
			break;
		case CRD_PAUSE:				       						// = 0x32,
			strcpy(ret_text, "PAUSE          ");
			break;
		case CRD_STOP:				       						// = 0x33,
			strcpy(ret_text, "USER-STOP      ");
			break;
		default:
			strcpy(ret_text, "ERROR:         ");
			ctoa(&ret_text[6], ret);
			break;
	}
}					
		
// Generierung von Alarmen bei fehlendem Ferro2 oder fehlender SD-Card
void Check_SDM_Alarm(void)
{
	// SDM Test
	if(a_erg_gl == ARC_ERR_FERRO_READ || a_erg_gl == ARC_ERR_FERRO_WRITE)
		ferro_err = 1;		// Ferro 2 nicht gesteckt
	else
	{
		if(a_erg_gl == ARC_OK	|| a_erg_gl == ARC_RUN || a_erg_gl == ARC_PAUSE)
			ferro_err = 0;
	}		
		
	if(c_erg_gl == CRD_ERR_CMD0 || c_erg_gl == CRD_ERR_OCR1)
		card_err = 1;		// SD-Card nicht gesteckt
	else
	{
		if(c_erg_gl == CRD_OK || c_erg_gl == CRD_RUN || c_erg_gl == CRD_PAUSE)
			card_err = 0;
	}	
	
	sdm_alarm_check = 1;
}	
 
// Der Status von allen DAE's wird auf DAE_INITIAL gesetzt
void set_daily_init(void)
{
	UINT i;
	char j;
	char max_exp = 0;
	char exp_idx = 0;
	
	for(i = 0; i < KOMTAB_LENG; i++)
	{	
		// Untersuchung auf Expansion
		if(monitor_tab[i].state == DAE_EXPMONI)
		{
			max_exp = expinfo_tab[i].max_exp;
			exp_idx = expinfo_tab[i].exp_idx;
			
			for(j = 0; j < max_exp; j++)
			{
				expmoni_tab[exp_idx][j].state    = DAE_INITIAL;
				expmoni_tab[exp_idx][j].oldvalue = 0;
			}	
		}
		else
		{
			monitor_tab[i].state = DAE_INITIAL;
			monitor_tab[i].oldvalue = 0;
		}
	}				
}

// Prüfen auf Konsistenz der Ferro-Infos mit der Anzahl der Einträge in der Verweispage
// return lokal:   ret.........0=Funktionsfehler, 1=ok
//                 vp_err......0=ok, 1=Konsistenzfehler
// return global:  ferro_adr...Adresse der Ferro-Informationen
//                 hp_adr......Adresse der hauptpage  
//                 dae_info[]..Ferro-Infos
//                 dp_adr......Adresse der Datenpage   entspr. Datenpage-Nr
//                 asdm_page...Puffer für eine Page
//                 read_time...Erster Zeitstempel der Datenpage, deren Eintrag in der Verweispage fehlt
    
char VP_Kontrolle(UINT dae, char exp, char *vp_err)
{
	CRD_ERG c_erg = 0;
	char ret = 0;
	char adrret = 0;
	char bicret = 0;
	UINT rel_DP = 0;
	char num_VP = 0;
	UINT vp_block = (UINT)SDM_OFFS_DP;		// 1 Hauptpage + 16 Verweispages
	
	*vp_err = 0;
	
	// Hauptpage- und Ferro-Adresse ermitteln
	adrret = get_address(dae, exp, &ferro_adr, &hp_adr);
	if(adrret == 0)
		return ret; // Fehler
		
	// Page-Informationen vom Ferro-Ram lesen
	bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICRD);
	if(bicret == 0)	// Fehler
		return ret;
		
	// Verglichen wird die aktuelle Datenpage-Nr (modulo 128) mit der Anzahl der Einträge in der aktuellen Verweispage
	rel_DP = ( (dae_info[0].akt_DP - 1) % 128) + 1;	// relative Datenpage-Nummer (1-128) 
	num_VP = dae_info[0].num_VP;										// Anzahl der Einträge in der Verweispage
	
	if(num_VP == 0)
		return ret;																		// noch nicht auswertbar
		
	if(num_VP < rel_DP)															// letzter Eintrag in der Verweispage fehlt
	{
		// Datenpage lesen
		dp_adr = hp_adr + vp_block + (ULONG)(dae_info[0].akt_DP - 1) * 512;						// Adresse der Datenpage   entspr. Datenpage-Nr
		c_erg = read_data_block(dp_adr, asdm_page, timeout_max);
		if(c_erg != CRD_OK)
			return ret;																	// Card Lesefehler			
		
		memcpy(&read_time, &asdm_page[0], 4);					// Ersten Zeitstempel lesen
		*vp_err = 1;																	// Fehlerkennzeichen
	}	
			
	ret = 1;
	return ret;	
}

// Werkseinstellung für ein DAE (nur ferro_info) durchführen
void one_reset_dae(UINT current_dae, char current_dae_exp)
{
	char max_expand, exp_index, bicret;
	ferro_info	dae_info[1] = {0};
  char lzkonv_buf[16] = {0};
	char byte_anz;
	
		max_expand = expinfo_tab[current_dae].max_exp;
		exp_index  = expinfo_tab[current_dae].exp_idx;	// normal 0, beginnt bei 1 (1.gefundene Expandgruppe), wenn max_expand > 0
		
		byte_anz = LzKonv(lzkonv_buf, current_dae, 0, 0);
		
		// Ferro-Adresse ermitteln
		if(max_expand == 0)
		{ //    Anfangsadresse	+ Offset
			ferro_adr = DAE_INFO_ADR    + DAE_INFO_LENG * current_dae;			// Normalbereich
		}
		else 
		{
			// Zugewiesene Speicherplätze
			ferro_adr = (UINT )&(exp_ferro_tab[exp_index-1][current_dae_exp]);
		}				
	
		bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICRD);
		if(bicret != 0)
		{
			dae_info[0].akt_DP = 1;											// aktuelle Datenpage-Nr.
			dae_info[0].num_DP = 0;											// Anzahl der Einträge in der Datenpage
		  dae_info[0].off_DP = (char)(byte_anz + 4);	// Offset zum nächsten Eintrag (+4 Byte Zeitstempel)
			dae_info[0].akt_VP = 1;											// aktuelle Verweispage-Nr.
			dae_info[0].num_VP = 0;											// Anzahl der Einträge in der Verweispage
			dae_info[0].num_HP = 0;											// Anzahl der Einträge in der Hauptpage
			dae_info[0].ovr_HP = 0;											// Anzahl der Datenpage-Überläufe (Ringpuffer)
		}
			
		bicret = xicbus(XEEPADR+2,	(char	*)&dae_info, ferro_adr, 8,	BICWR);
	
}	


//***********************************************************************************
// Kommandos zum Lesen der SD-Card-Memory
//***********************************************************************************
 
void ArchSDMReply(char *RsTxBuf, char *RsRxBuf, char prot)
{
	ULONG sd_adr = 0;
	char offs = 0;
	char block = 0;
	char idx = 0;
	char ret = 0;
	UINT dae = 0;
	char exp = 0;
	char anz = 0;
	char dat_offs = 0;
	ULONG time = 0;
	ULONG first_time = 0;
	ULONG hp_adr = 0;
	UINT  ferro_adr = 0;	
	
	switch(RsRxBuf[1])				// Subkommando
  {
		case READ_SDMEM:			 // Test: Lese Inhalt von einem SD-Card Speicherplatz
			// Kommando:	[0]				0xAC, 0x60,
			//						[2]				(ULONG) SD-Card Adresse (Page Raster 512 Byte  0x200) 
			//						[6]				(char)  1. bis 8. 64 Byte Block 
			// Antwort:		(offs+0)	64 Byte
			
			offs   = RsFrame(RsTxBuf, 64, RCOK, prot);				// Rahmen
			sd_adr = *(ULONG *)&RsRxBuf[2];										// Nummer des ersten Datenelementes
			block  = RsRxBuf[6];
			if(block > 0 && block < 9)
			{
				block--;
				dae = (UINT)(block * 64);
				ret = init_Card(timeout_max);
				
				ret = read_data_block(sd_adr, sd_buf, timeout_max);
				if(ret == CRD_OK)
				{
					memcpy(&RsTxBuf[offs], &sd_buf[dae], 64);
				}
				else
					RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Lesefehler
			}		
			else
				RsFrame(RsTxBuf, 0,	ERANZ, prot);							// Fehler
			break;
			
		case READ_ARCHIV:			// Lese archivierte Daten in Form von Datenpaketen (Zeitstempel + Daten)
			// Kommando:	[0]	 0xAC, 0x61,
			//						[2]	(UINT)  DAE   (Datenelement Nr. 0-511)
			//						[4]	(char)  EXP   (Expand Nr.   0=kein Expand)
			//						[5]	(ULONG) TIME  (von dem Zeitpunkt an sind Daten gewünscht. Funktion: time2long )
			// Antwort:		wenn RCODE ok
			//   					[0]	(char)  ANZ   (Anzahl der Datenpakete. 0=keine Daten vom gewünschten Zeitpunkt verfügbar)
			//						[1] (ULONG) FIRST (Zeitstempel vom Beginn der Aufzeichnung. 0=es gibt keine Aufzeichnung)
			//						[5]	(char)  OFF   (Offset der Datenpakete)
			//						[6] (ULONG) TIME  (Zeitstempel des 1. Datenpaketes)
			//					  [10] 2-15 Byte    (Daten       des 1. Datenpaketes)
			//						...
			//						bis Sendepuffer voll (max 64 Bytes)
			
			dae		= *(UINT *)&RsRxBuf[2];
			exp 	= RsRxBuf[4];
			time	= *(ULONG *)&RsRxBuf[5];
			ret = search_data(dae, exp, time, &anz, &dat_offs, &first_time, sd_buf);
			if(ret == 0)
			{	// Leseproblem Ferro oder SD-Card
				RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Lesefehler
			}
			else
			{
				if(anz == 0)
				{	// keine Daten
					offs = RsFrame(RsTxBuf, 5, RCOK, prot);		// Rahmen ANZ und FIRST
					RsTxBuf[offs] = anz;
					*(ULONG *)&RsTxBuf[offs+1] = first_time;
				}
				else
				{
					block = (char)(anz * dat_offs);						// Datenpuffer
					idx   = block + 6;												// +ANZ +FIRST +OFF
					offs = RsFrame(RsTxBuf, idx, RCOK, prot);
					
					RsTxBuf[offs] = anz;
					*(ULONG *)&RsTxBuf[offs+1] = first_time;
					RsTxBuf[offs+5] = dat_offs;
					memcpy(&RsTxBuf[offs+6], &sd_buf[0], block);
				}
			}
			break;
			
		case READ_MEMINFO:		// Test: Lese Informationen zur Speicherbelegung aus dem Ferro-Ram		
			// Kommando:  [0] 0xAC, 0x62,
			//            [2] (UINT)  DAE   (Datenelement Nr. 0-511)
			//            [4] (char)  EXP   (Expand Nr.   0=kein Expand)
			// Antwort:    8 Byte ferro_info
			//             2 Byte Ferro-Adresse
			//             4 Byte Hauptpage-Adresse
			
			dae		= *(UINT *)&RsRxBuf[2];
			exp 	= RsRxBuf[4];
			offs   = RsFrame(RsTxBuf, 14, RCOK, prot);							// Rahmen
			
			// Hauptpage- und Ferro-Adresse ermitteln
			ret = get_address(dae, exp, &ferro_adr, &hp_adr);
			if(ret == 1) // OK
			{
				// Page-Informationen vom Ferro-Ram lesen
				ret = xicbus(XEEPADR+2,	(char	*)&RsTxBuf[offs], ferro_adr, 8,	BICRD);
				if(ret == 0)																				// Lese-Fehler
					RsFrame(RsTxBuf, 0,	ERDAT, prot);
					
				offs += 8;
				*(UINT *)&RsTxBuf[offs] = ferro_adr;								// Ferro-Adresse
				offs += 2;
				*(ULONG *)&RsTxBuf[offs] = hp_adr;									// Hauptpage-Adresse
			}
			else
				RsFrame(RsTxBuf, 0,	ERANZ, prot);													// Adress-Fehler
			
			break;
			
		case READ_ARCHIV_INFO:	// Test: Lese SD-Speicher-Informationen zu einem gefundenen DAE
			// Informationen sind nur gültig nach einem vorausgegangenen READ_ARCHIV - Kommando
			// Kommando:  [0]  0xAC, 0x63,
			// Antwort:    4 Byte: gefundene Adresse der Hauptpage.
			//             2 Byte: gefundene Verweispagenummer in der Hauptpage (1-15)
			//             4 Byte: gefundene Adresse der Verweispage
			//             2 Byte: gefundene Datenpagenummer in der Verweispage (1-2032)
			//             4 Byte: gefundene Adresse der Datenpage.
			//             2 Byte: gefundene Zeitstempelnummer in der Datenpage
			offs   = RsFrame(RsTxBuf, 18, RCOK, prot);							// Rahmen
			*(ULONG *)&RsTxBuf[offs] = search_hp_adr;
			offs += 4;
			*(UINT *)&RsTxBuf[offs]  = search_vp_num;
			offs += 2;
			*(ULONG *)&RsTxBuf[offs] = search_vp_adr;
			offs += 4;
			*(UINT *)&RsTxBuf[offs]  = search_dp_num;
			offs += 2;
			*(ULONG *)&RsTxBuf[offs] = search_dp_adr;
			offs += 4;
			*(UINT *)&RsTxBuf[offs]  = search_ts_num;
			break;
						
		case CHECK_FERRO_SDHC:		// Kommando für Prüfplatz: Ferro2 gesteckt ?, SDHC gesteckt ?
			// Kommando:  [0] 0xAC, 0x64
			offs	= RsFrame(RsTxBuf, 6, RCOK, prot);				// Rahmen
			RsTxBuf[offs++]	= sdm_alarm_check;						// 1= Check gültig
			RsTxBuf[offs++]	= ferro_err;									// 1= Ferro2 fehlt
			RsTxBuf[offs++]	= card_err;										// 1= SDHC-Card fehlt
			RsTxBuf[offs++] = cardType;										// Typ:   3 = SD-HC CARD    
			RsTxBuf[offs++] = speedClass;									// Class: 0 , 2 , 4, 6, 10
			RsTxBuf[offs++] = performanceMove;						// Speed: in [MB/sec] für Class 0-6, Class 10 = 0 (ignored)
			break;
			
		case WRITE_SDMEM:         // Test: Schreibe Daten auf einen Speicherplatz	
			// Kommando:	[0]				0xAC, 0x65,
			//						[2]				(ULONG) SD-Card Adresse (Page Raster 512 Byte  0x200) 
			//						[6]				(char)  1. bis 8. 64 Byte Block 
			// Antwort:		Ausführung OK, Error

			sd_adr = *(ULONG *)&RsRxBuf[2];										// Nummer des ersten Datenelementes
			block  = RsRxBuf[6];
			if(block > 0 && block < 9)
			{
				block--;
				dae = (UINT)(block * 64);
				ret = init_Card(timeout_max);
				
				ret = read_data_block(sd_adr, sd_buf, timeout_max);
				if(ret == CRD_OK)
				{
					memcpy(&sd_buf[dae], &RsRxBuf[7], 64);
					ret = write_data_block(sd_adr, sd_buf, timeout_max);
					if(ret != CRD_OK)
						RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Schreibfehler
				}
				else
						RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Lesefehler
			}
			else
				RsFrame(RsTxBuf, 0,	ERANZ, prot);							// Fehler
			break;
		
		default:
			RsFrame(RsTxBuf, 0,	ERUK, prot);								// unbekanntes Kommando
			break;
		
	}	// End switch
}		// End ArchSDMReply

//--------------------------------------------------------------------------------------------------------------
// Unterprogramme zu den ArchSDMReply-Kommandos
//--------------------------------------------------------------------------------------------------------------
char search_data(UINT dae, char exp, ULONG my_time, char *anz_DP, char *off_DP, ULONG *first_time, char *buffer)
{
	UINT  i = 0;
	UINT  j = 0;
	char  num = 0;
	char	off = 0;
	char	anz = 0;
	char	gefunden_old = 0;
	UINT	vp_idx_gefunden = 0;
	UINT  dp_idx_gefunden = 0;
	UINT  dp_idx_aktuell = 0;
	char  bufload = 0; 
	char  max_exp = 0;
	char  exp_idx = 0;
	char	num_HP  = 0;
	char	ovr_HP  = 0;
	char	old_HP  = 0;
	char	overflow= 0;
	char  ret = 0;									// false vorbelegt
	char  ret_crd = 0;
	char  bicret = 0;
	ULONG my_first = 0;
	ULONG sd_time1 = 0;
	ULONG sd_time2 = 0;

	UINT	ferro_adr = 0;
	ferro_info	sd_dae_info[1] = {0};
	char  sd_buf[512] = {0};

	search_hp_adr = 0;							// gefundene Adresse der Hauptpage.
	search_vp_num = 0;							// gefundene Verweispagenummer in der Hauptpage (1-16)
	search_vp_adr = 0;							// gefundene Adresse der Verweispage.
	search_dp_num = 0;							// gefundene Datenpagenummer in der Verweispage (1-2031)
	search_dp_adr = 0;							// gefundene Adresse der Datenpage.
	search_ts_num = 0;							// gefundene Zeitstempelnummer in der Datenpage
	
	// Hauptpage- und Ferro-Adresse ermitteln
	ret = get_address(dae, exp, &ferro_adr, &search_hp_adr);
	if(ret == 0) // Error
		return ret;
	
	// Page-Informationen vom Ferro-Ram lesen
	bicret = xicbus(XEEPADR+2,	(char	*)&sd_dae_info, ferro_adr, 8,	BICRD);
	if(bicret == 0)	// Fehler
		return ret;
		
	if(sd_dae_info[0].num_HP == 0)												// Anzahl der Einträge in der Hauptpage	
	{
		*anz_DP   = 0;																			// noch keine Einträge vorhanden
		*first_time = 0;
		ret = 1;																						// Ok-Return
		return ret;
	}	
	
	
	//-------------------
	// Datenpage suchen
	//-------------------
	
	// 1. Schritt: Suche die relevante Verweispage in der Hauptpage
	//---------------------------------------------------------------
	ret_crd = read_data_block(search_hp_adr, sd_buf, timeout_max);
	if(ret_crd != CRD_OK)
		return ret;																						// Error: SD-Lesefehler
		
	// Beginn der Aufzeichnung in der Hauptpage suchen.
	ovr_HP = sd_dae_info[0].ovr_HP;													// >0 = Ringpuffer aktiv : es gibt es alte Daten, die nach einem Überlauf nicht gelöscht wurden.
	num_HP = sd_dae_info[0].num_HP;													// aktuelle Verweispage-Nr = Anzahl der Einträge in der Hauptpage
	
	if(ovr_HP > 0) 
		old_HP = num_HP + 1;																	// wenn Ringpuffer aktiv: old_HP ist um 1 größer als num_HP, zeigt also auf den ältesten Eintrag
	else
		old_HP = 1;

	my_first = *(ULONG *)&sd_buf[(old_HP - 1) *4];					// Beginn der Aufzeichnung für dieses DAE
	*first_time = my_first;
	if(my_time < my_first)
	{
		// gewünschter Zeitstempel liegt vor dem Aufzeichnungbeginn
		my_time = my_first;																		// Abfrage-Zeitstempel anpassen auf den Aufzeichnungsbeginn 
	}	
		
	// Zeitstempel suchen
	overflow 		 = 0;
	gefunden_old = 0;
	if(ovr_HP > 0)																					// nach einem Überlauf
	{	// zuerst alle alten Zeitstempel untersuchen
		num = SDM_VP_ANZ;																			// max.Anzahl der Einträge in der Hauptpage (entspr. Anzahl der Verweispages)
		for(i=old_HP-1, j=old_HP; i < num; i++, j++)
		{
			sd_time1 = *(ULONG *)&sd_buf[i*4];									// erster   Zeitstempel
			if(j < num)
				sd_time2 = *(ULONG *)&sd_buf[j*4];								// nächster Zeitstempel
			else
			{	// es gibt keinen nächsten alten Zeitstempel, weitersuchen bei den neuen Zeitstempeln 
				overflow = 1;
				break;
			}
			if(my_time >= sd_time1 && my_time < sd_time2)
				break;
		}


		if(overflow == 0)																			// alten Zeitstempel gefunden
		{	gefunden_old = 1;
			// i = Verweispage-Index
			vp_idx_gefunden = i;
			search_vp_num = i + 1;															// gefundene Verweispagenummer, fertig
		}																
		else	// overflow == 1
		{
			//sd_time1 =																				// erster Zeitstempel   (alt) schon gefüllt
			sd_time2 = *(ULONG *)&sd_buf[0];										// nächster Zeitstempel (neu)
			if(my_time >= sd_time1 && my_time < sd_time2)
			{	
				gefunden_old = 1;
				vp_idx_gefunden = i;
				search_vp_num = i + 1;														// gefundene Verweispagenummer, fertig
			}
		}	
	}

	if(gefunden_old == 0)
	{	// Suchen (weitersuchen) bei den neuen Zeitstempeln
		num = num_HP;																					// Anzahl der Einträge in der Hauptpage (Offset 4)
		for(i=0, j=1; i<num; i++, j++)						
		{
			
			sd_time1 = *(ULONG *)&sd_buf[i*4];								// erster   Zeitstempel
			if(j < num)
				sd_time2 = *(ULONG *)&sd_buf[j*4];							// nächster Zeitstempel
			else																							// es gibt keinen neuen nächsten Zeitstempel
				break;	 

			if(my_time >= sd_time1 && my_time < sd_time2)
				break;
		}
		// i = Verweispage-Index
		vp_idx_gefunden = i;
		search_vp_num = i + 1;																	// gefundene Verweispagenummer																  
	}	
	
	// 2. Schritt: Suche die relevante Datenpage in der ermittelten Verweispage
	//--------------------------------------------------------------------------------
	search_vp_adr = search_hp_adr + (ULONG)SDM_OFFS_VP + (ULONG)vp_idx_gefunden * 0x200;							// Adresse der Verweispage
	ret_crd = read_data_block(search_vp_adr, sd_buf, timeout_max);
	if(ret_crd != CRD_OK)
		return ret;																						// Error: SD-Lesefehler
	
	if(gefunden_old == 1)																		// alte Datenpage
	{
		if(search_vp_num == 16)																// letzte Verweispage hat
			num = 111;																					// nur 111 Einträge (2031 - 15*128)
		else	  
	  	num = 128;																					// maximale Anzahl benutzen
	}  	
	else																										// neue Datenpages
	{
		if(vp_idx_gefunden == sd_dae_info[0].akt_VP - 1) 			// bin ich in der aktuellen Verweispage ?
			num = sd_dae_info[0].num_VP;												// ja, Anzahl der Einträge in der Verweispage benutzen
		else
	  	num = 128;																					// maximale Anzahl benutzen
	}			
	
	for(i=0, j=1; i<num; i++, j++)						
	{
		sd_time1 = *(ULONG *)&sd_buf[i*4];										// erster   Zeitstempel
		if(j < num)
			sd_time2 = *(ULONG *)&sd_buf[j*4];									// nächster Zeitstempel
		else
			break;	 
		
		if(my_time >= sd_time1 && my_time < sd_time2)
			break;
	}
	// i = Datenpage-Index
	dp_idx_gefunden = (UINT)vp_idx_gefunden * 128 + i;			// 128 Einträge in allen vorherigen Verweispages																			
	search_dp_num = dp_idx_gefunden + 1;										// gefundene Datenpagenummer																	
	
	// 3. Schritt; Suche den relevanten Zeitstempel in der ermittelten Datenpage 
	//--------------------------------------------------------------------------------
	search_dp_adr = search_hp_adr + (ULONG)SDM_OFFS_DP + (ULONG)dp_idx_gefunden * 0x200;							// Adresse der Datenpage		
	ret_crd = read_data_block(search_dp_adr, sd_buf, timeout_max);
	if(ret_crd != CRD_OK)
		return ret;																						// Error: SD-Lesefehler
			
	off = sd_dae_info[0].off_DP;														// Offset
	// Anzahl der Einträge ermitteln
	if(gefunden_old == 1)																		// alte Datenpage
	  num = (char)(512 / off);															// maximale Anzahl benutzen
	else
	{
		dp_idx_aktuell = sd_dae_info[0].akt_DP - 1;						// aktuelle Datenpage-Nummer
		if( dp_idx_gefunden == dp_idx_aktuell )								// bin ich in der aktuellen Datenpage ?
			num = sd_dae_info[0].num_DP;												// ja, Anzahl der Einträge in der Datenpage benutzen
		else
		  num = (char)(512 / off);														// nein, ich bin in einer älteren Datenpage, maximale Anzahl benutzen
	}	  
		 
	for(i=0; i<num; i++)						
	{
		sd_time1 = *(ULONG *)&sd_buf[i*off];									// Zeitstempel
		if( check_sdtime(sd_time1, my_time) == 1 )						// Gültigkeit überprüfen
		{	
			break;																							// gültig
		}
		else
		{
			continue;
		}
	}
	// i = Zeitstempel-Index
	search_ts_num = i + 1;																	// gefundene Zeitstempelnummer

	
	// 4. Schritt: Fülle *buffer mit Daten ab dem Zeitstempel-Index mit maximal 60 Byte 		
	//--------------------------------------------------------------------------------
	anz = 0;
	for(; i < num; i++)
	{
		sd_time2 = *(ULONG *)&sd_buf[i*off];									// Zeitstempel
		if( check_sdtime(sd_time2, my_time) == 1 )						// Gültigkeit überprüfen
		{	
			bufload = anz * off;
			if( bufload > (64-off) )
				break;

			memcpy(&buffer[bufload], &sd_buf[i*off], off);
			anz++;
		}
		else
		{
			continue;
		}
	}
	
	if(bufload <= (64-off) )																		// ist noch Platz im buffer ?
	{
		// Suche nach weiteren Datenpagen
		//----------------------------------
		if(gefunden_old == 1)		// Suche in alten Datenpagen
		{	
			// Suche in den überrundeten alten Datenpagen: es gibt immer eine nächste Datenpage
			if((dp_idx_gefunden + 1) >= SDM_DP_ANZ)									// größer als maximale Anzahl
			{
				// Ringspeicher: nächste DP ist die erste DP
				search_dp_adr = search_hp_adr + (ULONG)SDM_OFFS_DP;
				if(sd_dae_info[0].akt_DP == 1)												// ist die erste DP zur Zeit die aktuelle DP ?
				{
					num = sd_dae_info[0].num_DP;												// ja, Anzahl der Einträge in der Datenpage benutzen
				}
				else
	  			num = (char)(512 / off);														// nein, maximale Anzahl benutzen
			}
			else																										// es gibt eine nächste alte Datenpage
			{
				search_dp_adr += 0x200;																// Adresse erhöhen
	  		num = (char)(512 / off);															// maximale Anzahl benutzen
	  	}
		}
		else										// Suche in den aktuellen Datenpagen
		{
			bicret = 0;																							// Merker
			if( (dp_idx_gefunden + 1) <= dp_idx_aktuell)						// gibt es eine weitere Datenpage ?
			{
				search_dp_adr += 0x200;																// ja, nächste Datenpage lesen
				bicret = 1;																						// Merker setzen

				if((dp_idx_gefunden + 1) == dp_idx_aktuell)						// bin ich jetzt in der aktuellen Datenpage ?
					num = sd_dae_info[0].num_DP;												// ja, Anzahl der Einträge in der Datenpage benutzen
				else
	  			num = (char)(512 / off);														// nein, maximale Anzahl benutzen
	  	}
		}
		
		// Datenpage lesen ?
		if(gefunden_old == 1 || bicret == 1)
		{	
			ret_crd = read_data_block(search_dp_adr, sd_buf, timeout_max);
			if(ret_crd != CRD_OK)
				return ret;																						// Error: SD-Lesefehler
			
			
			for(i=0; i < num; i++)																	// buffer weiter füllen
			{
				sd_time2 = *(ULONG *)&sd_buf[i*off];									// Zeitstempel
				if( check_sdtime(sd_time2, my_time) == 1 )						// Gültigkeit überprüfen
				{	
					bufload += off;
					if( bufload > (64-off) )
						break;

					memcpy(&buffer[bufload], &sd_buf[i*off], off);
					anz++;
				}
				else
				{
					continue;
				}
			}
		}
	}
	
	*anz_DP = anz;	// Anzahl der Datenblöcke zurückgeben
	*off_DP = off;	// Offset der Datenblöcke zurückgeben
	ret = 1;				// OK
	return ret;
}		

//------------------------------------------------------------------
// UP: Adressen ermitteln (Hauptpage und Ferro)
//------------------------------------------------------------------
char get_address(UINT dae, char exp, UINT *ferro_adr, ULONG *hp_adr)
{
	char  ret = 0;
	ULONG start = 0;
	ULONG block = 0;
	char  max_exp = 0;
	char  exp_idx = 0;
	
	if(dae > 511)
		return ret;
	// Adressen ermitteln
	if(exp == 0)																					// Normalparameter
	{
		*ferro_adr = DAE_INFO_ADR  + DAE_INFO_LENG * dae;		// Adresse der Ferro-Infos	
		start = (ULONG)SDM_START_ADR;
		block = (ULONG)SDM_BLOCK_DAE;
		*hp_adr  = start + block * (ULONG)dae;								// Anfangsadresse des Speicherblocks auf der SD-Karte (Hauptpage)
	}
	else																									// Expandparameter
	{
		max_exp = expinfo_tab[dae].max_exp;									// Expandinformationen aus Datenmonitoring-Tabelle
		exp_idx = expinfo_tab[dae].exp_idx;
		if(max_exp == 0 || max_exp < exp)
			return ret;																				// Error: kein Expandeintrag vorhanden oder EXP zu groß
		
		// Die Ferro-Adresse befindet sich in der Expanddaten-Tabelle für den alloziierten Ferro-Bereich	
		*ferro_adr = (UINT )&(exp_ferro_tab[exp_idx-1][exp-1]);	
		// Die Hauptpage-Adresse befindet sich in der Expanddaten-Tabelle für den alloziierten SD-Card-Memory-Bereich
		block = (ULONG)SDM_BLOCK_DAE;
		*hp_adr = exp_sdm_tab[exp_idx-1] + block * (ULONG)(exp-1);
	}
	ret = 1;
	return ret;
}	

//------------------------------------------------------------------
// UP: Zeitstempel auf Gültigkeit überprüfen
//------------------------------------------------------------------
char check_sdtime(ULONG sd_time, ULONG my_time)
{
	ULONG	sys_time = 0;
	
	if(sd_time < my_time)		// darf nicht kleiner als der angefragte Zeitstempel sein
		return 0;							// ungültig
		
	sys_time = time2long(Sek, Min, Std, Tag, Mon, Jahr);	// aktuelle Zeit im R66

	if(sd_time > sys_time)	// darf nicht größer als die aktuelle Zeit sein			
		return 0;							// ungültig
		
	return 1;								// gültig
}

				
#endif // ARCHIV_SDM == 1
	
			
