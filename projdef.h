/*------------------------------------------------------------------------*/
/*                            Projekt-Definitionen                        */
/*                            ====================                        */
/*  Wichtig:  Diese Projekt-Definitionen müssen mit den Einstellungen     */
/*            im ausgewählten Projekt übereinstimmen.                     */
/*            (char Proj_Typ = x   in UserConst.c)                        */
/*------------------------------------------------------------------------*/
// 
#ifndef PROJDEF_H_INCLUDED
#define PROJDEF_H_INCLUDED


// MBus Zähler-Typen
// Wärmezähler ( (Volumen im Rücklauf gemessen) Siemens / SPX 2WR4, 2WR5; ISTA Sensonic II; WDV/MOLLINE Senso Ultra )
#define WRM         4
// Warmwasserzähler ( PadPuls M2 )
#define WWS         6
// Kaltwasserzähler ( PadPuls M2 )
#define KWS         0x16
// Elektrozähler ( PadPuls M2 )
#define ELT         2
// Gaszähler 
#define GMB         3
// Klima Wärme-/Kältezähler ( WDV-MOLLINE (Hydrometer) SYCLAR INT 8, Energy-INT8 )
#define KLM         0x0D
// Warmwasserzähler von ISTA liefern als Medium "other" !
#define OTHER       0


// Index - Definitionen
#define ANL         0             // Index für Anlage

#define HK1         0             // Index für HK Regler 1
#define HK2         1             // Index für HK Regler 2
#define HK3         2             // Index für HK Regler 3
#define HK4         3             // Index für HK Regler 4

#define WW1         0             // Index für WW Regler 1
#define WW2         1             // Index für WW Regler 2

#define KE1         0             // Index für KE Regler 1
#define KE2         1             // Index für KE Regler 2
#define KE3         2             // Index für KE Regler 2
#define KE4         3             // Index für KE Regler 2
#define KE5         4             // Index für KE Regler 2
#define KE6         5             // Index für KE Regler 2
#define KE7         6             // Index für KE Regler 2
#define KE8         7             // Index für KE Regler 2

#define NT1         0             // Index für NT Regler 1
#define NT2         1             // Index für NT Regler 2

#define SO1         0             // Index für SO Regler 1
#define SO2         1             // Index für SO Regler 2

// SiWa
#define WP1         0             // Index für WP Regler 1

// Definitionen zur Implementierung von Anwendungen für serielle Schnittstellen S1 S2 S3
#define NULL_IMPL       0x0000    // Schnittstelle nicht verfügbar
#define GBUS_IMPL       0x0001    // RS485 RIEcon Gebäudebus
#define FUNK1_IMPL      0x0002    // RS485 Funkmodul EnOcean 
#define GENI1_IMPL      0x0004    // RS485 Genibus Pumpen
#define MBUS1_IMPL      0x0008    // RS232 MBUS
#define GBUS1_IMPL      0x0010    // RS485 Test Gebäudebus
#define MODBUS1_IMPL    0x0020    // RS485 oder RS232 MODBUS Master oder Slave
#define KNXBUS1_IMPL    0x0040    // KNX/EIB über RIEcon66-SPL-KNX
#define ANYBUS1_IMPL    0x0080    // RS232 Anybus
//....

// Nummern zur Auswahl einer Anwendung für die betreffende Schnittstelle
// Der Wert ist zur Laufzeit parametrierbar: char Funktion_Sx
#define NULL_FU         0         // keine Funktion
#define GBUS_FU         1         // RS485 RIEcon Gebäudebus
#define FUNK1_FU        2         // RS485 Funkmodul EnOcean 
#define GENI1_FU        3         // RS485 Genibus Pumpen
#define MBUS1_FU        4         // RS232 MBUS
#define GBUS1_FU        5         // RS485 Test Gebäudebus
#define MODBUS1_FU      6         // RS485 oder RS232 MODBUS Master oder Slave
#define KNXBUS1_FU      7         // KNX/EIB über RIEcon66-SPL-KNX
#define ANYBUS1_FU      8         // RS232 Anybus
//....

// --------------- Anlage ------------------------------------------------
#define ANLANZ      1             // projektierte Anzahl von Anlagen
#define ANLMAX      1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)

// Einstellung für Parli: anl[]
#define TA_ANL      1             // Außentemperatur anzeigen
#define TVP_ANL     0             // Primärvorlauf  anzeigen
#define TRP_ANL     0             // Primärrücklauf anzeigen
#define VARIPROJ    0             // 1 = variables Projekt
#define SSTM_PU     0             // Sammelstörmelde-Eingang für Pumpen
#define SSTM_ANL    0             // Sammelstörmelde-Eingang Anlage
#define ANFORD      0             // auch in Steuer.c benutzt
#define ANF_EXT     0             // Anzahl Anforderungen externer Regler, gilt für NT und Kessel

// allgemein
#define QUIT_TASTE  0             // 1 = Quittungstaste vorhanden
#define FER         0             // = 1 : Ferienprogramm aktiv
#define HEIZGRD     0             // Heizgradtage
#define ARCHIV_ANL  1             // Archivierung mit Ferro-Ram
#define ARCHIV_SDM  1             // Josch-SDM : Archivierung mit SD-Card-Memory

// Komtab-Copy
#define COPY_TIMEOUT      15      // Minuten beim Empfang

#define COPY_RESERV_EMPF  4       // Anzahl der reservierten COPY_EMPF Einträge (Komtab-Nummern) Min=1, Max=16
#define COPY_EMPF         0       // Anzahl der über Bus zu empfangenen Datenelemente            Min=0, Max=COPY_RESERV_EMPF,  wenn > 0: Eintrag in CopyCheck.c erforderlich

#define COPY_RESERV_SEND  4       // Anzahl der reservierten COPY_SEND Einträge (Komtab-Nummern) Min=1, Max=16
#define COPY_SEND         0       // Anzahl der über Bus zu sendenden Datenelemente              Min=0, Max=COPY_RESERV_SEND,  wenn > 0: Eintrag in CopyCheck.c erforderlich

#define DIAG        0             // Diagnose für Soft-Wärmezähler
#define ANALOG_AE   0             // Skalierung der 0-10V Eingänge, Grenzwertüberwachung, Anzahl 0..8, immer die ersten n AE_UNI ab AE_UNI1 werden skaliert
#define WRE100      0             // WRE100 Kommandos für WRELES uund WREINIT werden unterstützt
#define MBUSANZ     0             // Anzahl MBus-Zähler (maximal 8)
#define MBUS_MASTER 0             // 1/0
#define STEUER_UNI  0             // 1/0 Benutzersteuerung mit UNI-Elementen (SteuerUni.c, parli_uni.h, UniStandard, UniDynam)

// ***** josch: Datenmanager
#define DS_MODE       0           // Gerät im DS_Modus (Data Slave) , Auswertung in der Funktion DReply() 
#define DS_RXBUF_MAX  16          // Puffergröße für Empfangsdaten
#define DS_TXBUF_MAX  32          // Puffergröße für Sendedaten

#define DM_MODE       0           // Gerät im DM_Modus (Data Master), Datenbearbeitung in der Funktion DManager(), Organisation der Datenübertragung im Task DTimer() 
#define DM_RXBUF_MAX  32          // Puffergröße für Empfangsdaten
#define DM_TXBUF_MAX  16          // Puffergröße für Sendedaten
#define DM_SLAVES     1           // Min:1 Max:4  Anzahl der Slave-Geräte, mit denen der Datenmanager arbeiten soll

/*****************************************************************************************************/
// Implementierung von Anwendungen für serielle Schnittstellen S1 S2 S3
/*****************************************************************************************************/
#define IMPLEMENT_S1      (GBUS_IMPL)
#define IMPLEMENT_S2      (GBUS_IMPL)
#define IMPLEMENT_S3      (GBUS_IMPL)  // + MBUS1_IMPL) + MODBUS1_IMPL) + GENI1_IMPL) + ANYBUS1_IMPL)

#define MODBUS_MASTER     0       // 1 = Modbus als Master 
#define MODBUS_SLAVE      0       // 1 = Modbus als Slave

//---------------------------
// Modbus 
//---------------------------
#if ( ( ( IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL ) || ( ( IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL ) || ( ( IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL ))
  #define MODBUS_SIO    1
  #define MODBUS_UNI    1
  #define MODBUS_EXT    0    // 1 = Verwendung von READ_MULTIPLE_COILS in der ModbusTabelle

	// Pumpen mit Modbus
  #define WILO_MODBUS          0   // HIER Angeben ob Grundfos oder Wilo (beides zusammen geht nicht)  // Wilo basierend auf IF-Modul Modbus Stratos (2097808)
  #define GRUNDFOS_MODBUS      0   // HIER Angeben ob Grundfos oder Wilo (beides zusammen geht nicht)

#else
  #define MODBUS_SIO           0
  #define MODBUS_UNI           0
  #define MODBUS_EXT           0
  #define WILO_MODBUS          0
  #define GRUNDFOS_MODBUS      0
#endif

//---------------------------
// Pumpen mit Genibus
//---------------------------
// Genibus-Pumpe ist nur aktivierbar, wenn keine Modbus-Pumpe ausgewählt ist
#if ( ( ( ( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) || ( ( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) ) && ( WILO_MODBUS == 0 && GRUNDFOS_MODBUS == 0 ) )
  #define GENI         1
#else
  #define GENI         0
#endif

//--------------------------------------------------------------------------
// für alle Pumpen
//--------------------------------------------------------------------------
#if (GENI == 1 || WILO_MODBUS == 1 || GRUNDFOS_MODBUS == 1)
  #define BUS_PU_MAX   2  // Pumpenanzahl gemäß Projektspezifkationen setzen!
#else	
  #define BUS_PU_MAX   0
#endif

/*****************************************************************************************************/

#define TAE_ANZ     0             // Anzahl Analogeingänge Außentemperatur, max.1 
#define TAA_ANZ     0             // Anzahl Analogausgänge Außentemperatur, max.4

#define TP_UNI_ANZ  8             // min.1 max.8 universelle Temperatureingänge definiert

#define DE_UNI_ANZ  8             // min.1 max.8 universelle Digitaleingänge definiert

#define BM_UNI_ANZ  8             // min.1 max.8 universelle Digitaleingänge für Betriebsmeldungen definiert

#define SM_UNI_ANZ  8             // min.1 max.8 universelle Digitaleingänge für Störmeldungen definiert

#define AE_UNI_ANZ  8             // min.1 max.8 universelle Analogeingaenge definiert

#define DA_UNI_ANZ  8             // min.1 max.8 universelle Digitalausgaenge definiert

#define AA_UNI_ANZ  8             // min.1 max.8 universelle Analogausgaenge definiert

#define RM_POWER_ANZ	0           // min 0 max.8 Analogeingaenge

#define AE_DRUCK_ANZ	0           // min 0 max.8 Analogeingaenge

// --------------- Heizkreis-Regelungen -----------------------------------
#define HKANZ       0             // projektierte Anzahl von Heizkreisen
#define HKMAX       1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)

// Einstellung für Parli: hk1[]
#define TRLBEG_HK1  0             // Rücklauftemperaturbegrenzung
#define TRAUMK_HK1  0             // Anzeige eigener  Raum- und Soll-Temp. (Kaskade)
#define BEDARF_HK1  0             // Bedarfssteuerung durch R50.  Wenn 0, Beabs = 0 setzen 
#define BEDRAUM_HK1 0             // Anzeige externer Raum- und Soll-Temp. vom R50 (Kaskade)
#define DREIP_HK1   0             // 1=DreipunktAusgabe 0=Analogausgabe
#define STW_HK1     0             // STW-ALARM
#define PUAL_HK1    0             // Pumpenalarm
#define PUBM_HK1    0             // Betriebsmeldung Pumpe
#define PUDO_HK1    0             // Doppelpumpe
#define STRPU_HK1   0             // Strahlpumpe statt Ventil
#define FRG_HK1     0             // 1 = Freigabe-Signal definiert ( RegelHk ) ( bei Lüftung ARES/PEWO/HAR0111 benutzt )
#define SWAUS_HK1   0             // 1 = Anlagenschalter-Signal definiert (HK Nichtnutzung )
#define PULZ_HK1    0             // Pumpenlaufzeit berechnen und anzeigen ( parli, steuer.c,)  (nicht setzen wenn über Genibus gesteuert) 
#define FBH_HK1     0             // Fußbodenheizung mit Estrich-Programm 
#define LEIBEGR_HK1 0             // Leistungsbegrenzung (Leistungsberechnung von ext.Wärmezähler)
#define PU_BUS_HK1  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index 

// Einstellung für Parli: hk2[]
#define TRLBEG_HK2  0             // Rücklauftemperaturbegrenzung
#define TRAUMK_HK2  0             // Anzeige eigener  Raum- und Soll-Temp. (mit Kaskade)
#define BEDARF_HK2  0             // Bedarfssteuerung durch R50.  Wenn 0, Beabs = 0 setzen 
#define BEDRAUM_HK2 0             // Anzeige externer Raum- und Soll-Temp. vom R50 (mit Kaskade) 
#define DREIP_HK2   0             // 1=DreipunktAusgabe 0=Analogausgabe
#define STW_HK2     0             // STW-ALARM
#define PUAL_HK2    0             // Pumpenalarm
#define PUBM_HK2    0             // Betriebsmeldung Pumpe
#define PUDO_HK2    0             // Doppelpumpe
#define STRPU_HK2   0             // Strahlpumpe statt Ventil
#define FRG_HK2     0             // 1 = Freigabe-Signal definiert ( RegelHk ) ( bei Lüftung ARES/PEWO/HAR0111 benutzt )
#define SWAUS_HK2   0             // 1 = Anlagenschalter-Signal definiert (HK Nichtnutzung )
#define PULZ_HK2    0             // Pumpenlaufzeit berechnen und anzeigen ( parli, steuer.c,) (nicht setzen wenn über Genibus gesteuert) 
#define FBH_HK2     0             // Fußbodenheizung mit Estrich-Programm 
#define LEIBEGR_HK2 0             // Leistungsbegrenzung (Leistungsberechnung von ext.Wärmezähler)
#define PU_BUS_HK2  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// Einstellung für Parli: hk3[]
#define TRLBEG_HK3  0             // Rücklauftemperaturbegrenzung
#define TRAUMK_HK3  0             // Kaskade mit eigenem Raumtemperaturfühler
#define BEDARF_HK3  0             // Bedarfssteuerung durch R50
#define BEDRAUM_HK3 0             // Kaskade mit Raum-/Soll-Temp. vom R50
#define DREIP_HK3   0             // 1=DreipunktAusgabe 0=Analogausgabe
#define STW_HK3     0             // STW-ALARM
#define PUAL_HK3    0             // Pumpenalarm
#define PUBM_HK3    0             // Betriebsmeldung Pumpe
#define PUDO_HK3    0             // Doppelpumpe
#define STRPU_HK3   0             // Strahlpumpe statt Ventil
#define FRG_HK3     0             // 1 = Freigabe-Signal definiert ( RegelHk ) ( bei Lüftung ARES/PEWO/HAR0111 benutzt )
#define SWAUS_HK3   0             // 1 = Anlagenschalter-Signal definiert (HK Nichtnutzung )
#define PULZ_HK3    0             // Pumpenlaufzeit berechnen und anzeigen ( parli, steuer.c,) (nicht setzen wenn über Genibus gesteuert) 
#define FBH_HK3     0             // Fußbodenheizung mit Estrich-Programm 
#define LEIBEGR_HK3 0             // Leistungsbegrenzung (Leistungsberechnung von ext.Wärmezähler)
#define PU_BUS_HK3  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// Einstellung für Parli: hk4[]
#define TRLBEG_HK4  0             // Rücklauftemperaturbegrenzung
#define TRAUMK_HK4  0             // Kaskade mit eigenem Raumtemperaturfühler
#define BEDARF_HK4  0             // Bedarfssteuerung durch R50
#define BEDRAUM_HK4 0             // Kaskade mit Raum-/Soll-Temp. vom R50
#define DREIP_HK4   0             // 1=DreipunktAusgabe 0=Analogausgabe
#define STW_HK4     0             // STW-ALARM
#define PUAL_HK4    0             // Pumpenalarm
#define PUBM_HK4    0             // Betriebsmeldung Pumpe
#define PUDO_HK4    0             // Doppelpumpe
#define STRPU_HK4   0             // Strahlpumpe statt Ventil
#define FRG_HK4     0             // 1 = Freigabe-Signal definiert ( RegelHk ) ( bei Lüftung ARES/PEWO/HAR0111 benutzt )
#define SWAUS_HK4   0             // 1 = Anlagenschalter-Signal definiert (HK Nichtnutzung )
#define PULZ_HK4    0             // Pumpenlaufzeit berechnen und anzeigen ( parli, steuer.c,)  (nicht setzen wenn über Genibus gesteuert) 
#define FBH_HK4     0             // Fußbodenheizung mit Estrich-Programm 
#define LEIBEGR_HK4 0             // Leistungsbegrenzung (Leistungsberechnung von ext.Wärmezähler)
#define PU_BUS_HK4  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

#define ESTR_STUFEN 30            // maximale Anzahl von Tagen (Größe des Temperaturen-Feldes)

#if ( PUDO_HK1 == 1 || PUDO_HK2 == 1 || PUDO_HK3 == 1 || PUDO_HK4 == 1 )
#define PUDO 1
#else
#define PUDO 0
#endif

// --------------- Heizkreis-Adaption -------------------------------------
#define ADAP_STAT_MAX 1           // max.(4) Quellen ( R50 ) pro HK, mindestens 1 wg. Speicher-Defin.
#define ADAP_STAT_HK1 0           // max.(4) Quellen
#define ADAP_QU_HK1   0           // Quelle : R50 (0), Analogeingang ADAPT (1), dig.Eingänge ADAPT_T / ADAPT_H (2)
#define ADAP_STAT_HK2 0
#define ADAP_QU_HK2   0
#define ADAP_STAT_HK3 0
#define ADAP_QU_HK3   0
#define ADAP_STAT_HK4 0
#define ADAP_QU_HK4   0

// --------------- Warmwasser-Regelungen ----------------------------------
#define WWANZ       0             // projektierte Anzahl von Regelkreisen
#define WWMAX       1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)

// Einstellung für SteuerWw
#define PUWL_MODE_WW1   0           // Einschalten der Ladepumpe im Ladebetrieb:
                                  //    0 = sofort
                                  //    1 = wenn Kesseltemp. höher als Speichertemp.
                                  //    2 = wenn TWWC höher als Speichertemp.
                                  //    3 = wenn TVSEK höher als Speichertemp.
// Einstellung für RegelWw
#define RCOND_WW1   0             // Bedingung für Regelung:
                                  //    0 = immer
                                  //    1 = Ladebetrieb abfragen
                                  //    2 = Zirk.Pumpe abfragen
                                  //        Zirk.Pu. wird bei Speicherladung abgeschaltet ( Siefke )
                                  //        ( aber nicht bei Legionellenbetrieb )       

// Einstellung für Parli: ww1[]
#define REGEL_WW1   0             // 1 = mit Warmwasserregelung, benutzt auch in RegelNt.c !!!
#define DREIP_WW1   0             // 1 = DreipunktAusgabe,  0=Analogausgabe

#define LPU_WW1     0             // 1 = Ladepumpe vorhanden ( SteuerWw ) (nicht setzen wenn über Genibus gesteuert)
#define LPUBM_WW1   0             // 1 = Betriebsmeldung Ladepumpe vorhanden
#define LPUAL_WW1   0             // 1 = Ladepumpen-Alarm vorhanden
#define LPUBED_WW1  0             // 1 = Schalten des Ladebetriebs nach Bedarfsanford. ( Kommando WR_KASKADE )

#define ZPU_WW1     0             // 1 = Zirkulationspumpe vorhanden (nicht setzen wenn über Genibus gesteuert)   
#define ZPU_LZ_WW1  0             // 1 = Laufzeit der Zirkulationspumpe berechnen / anzeigen              
#define ZPUBM_WW1   0             // 1 = Betriebsmeldung Zirk.pumpe vorhanden
#define ZPUAL_WW1   0             // 1 = Zirkulationspumpe-Alarm vorhanden
#define ZPUBED_WW1  0             // 1 = Schalten der Zirk.pumpe nach Bedarfsanford. ( Kommando WR_KASKADE )
#define TZIRK_WW1   0             // 1 = Zirkulationstemperatur Fühler vorhanden

#define TWWV_WW1    0             // 1 = Warmwassertemperatur Fühler vorhanden
#define TPU_WW1     0             // 1 = Tauscherpumpe vorhanden (nicht setzen wenn über Genibus gesteuert)
#define TPUAL_WW1   0             // 1 = Tauscherpumpen-Alarm vorhanden
#define TPUBM_WW1   0             // 1 = Betriebsmeldung Tauscherpumpe vorhanden
#define STRPU_WW1   0             // Strahlpumpe statt Ventil
#define SWAUS_WW1   0             // 1 = Anlagenschalter-Signal definiert (WW Nichtnutzung )

/***** ulsch *****/
#define SP_WW1      0             // 1 = Warmwasser-Speicher vorhanden ( Parli, RegelWw, SteuerWw )
#define SPTP_WW1    0             // Anzahl der Speichertemperaturfühler (parli)
#define STB_WW1     0             // STB-ALARM
#define STW_WW1     0             // STW-ALARM

// Pumpenbus
#define PU_BUS_TPU_WW1  0         // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PU_BUS_LPU_WW1  0         // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PU_BUS_ZPU_WW1  0         // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// Einstellung für SteuerWw
#define PUWL_MODE_WW2   0         // Einschalten der Ladepumpe im Ladebetrieb:
                                  //    0 = sofort
                                  //    1 = wenn Kesseltemp. höher als Speichertemp.
                                  //    2 = wenn TWWC höher als Speichertemp.
                                  //    3 = wenn TVSEK höher als Speichertemp.
// Einstellung für RegelWw
#define RCOND_WW2   0             // Bedingung für Regelung:
                                  //    0 = immer
                                  //    1 = Ladebetrieb abfragen
                                  //    2 = Zirk.Pumpe abfragen
                                  //        Zirk.Pu. wird bei Speicherladung abgeschaltet ( Siefke )
                                  //        ( aber nicht bei Legionellenbetrieb )       

// Einstellung für Parli: ww2[]
#define REGEL_WW2   0             // 1 = mit Warmwasserregelung, benutzt auch in RegelNt.c !!!
#define DREIP_WW2   0             // 1 = DreipunktAusgabe,  0=Analogausgabe

#define LPU_WW2     0             // 1 = Ladepumpe vorhanden ( SteuerWw ) (nicht setzen wenn über Genibus gesteuert)
#define LPUBM_WW2   0             // 1 = Betriebsmeldung Ladepumpe vorhanden
#define LPUAL_WW2   0             // 1 = Ladepumpen-Alarm vorhanden
#define LPUBED_WW2  0             // 1 = Schalten des Ladebetriebs nach Bedarfsanford. ( Kommando WR_KASKADE )

#define ZPU_WW2     0             // 1 = Zirkulationspumpe vorhanden (nicht setzen wenn über Genibus gesteuert)   
#define ZPU_LZ_WW2  0             // 1 = Laufzeit der Zirkulationspumpe berechnen / anzeigen              
#define ZPUBM_WW2   0             // 1 = Betriebsmeldung Zirk.pumpe vorhanden
#define ZPUAL_WW2   0             // 1 = Zirkulationspumpe-Alarm vorhanden
#define ZPUBED_WW2  0             // 1 = Schalten der Zirk.pumpe nach Bedarfsanford. ( Kommando WR_KASKADE )
#define TZIRK_WW2   0             // 1 = Zirkulationstemperatur Fühler vorhanden

#define TWWV_WW2    0             // 1 = Warmwassertemperatur Fühler vorhanden
#define TPU_WW2     0             // 1 = Tauscherpumpe vorhanden (nicht setzen wenn über Genibus gesteuert)
#define TPUAL_WW2   0             // 1 = Tauscherpumpen-Alarm vorhanden
#define TPUBM_WW2   0             // 1 = Betriebsmeldung Tauscherpumpe vorhanden
#define STRPU_WW2   0             // Strahlpumpe statt Ventil
#define SWAUS_WW2   0             // 1 = Anlagenschalter-Signal definiert (WW Nichtnutzung )

/***** ulsch *****/
#define SP_WW2      0             // 1 = Warmwasser-Speicher vorhanden ( Parli, RegelWw, SteuerWw )
#define SPTP_WW2    0             // Anzahl der Speichertemperaturfühler (parli)
#define STB_WW2     0             // STB-ALARM
#define STW_WW2     0             // STW-ALARM

// Pumpenbus
#define PU_BUS_TPU_WW2  0         // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PU_BUS_LPU_WW2  0         // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PU_BUS_ZPU_WW2  0         // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// --------------- Kessel-Regelungen --------------------------------------
#define KEANZ       0             // projektierte Anzahl von Regelkreisen
#define KEMAX       1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)
#define SSF         0             // Schornsteinfeger-Funktion
#define SW_ANH_VAR  0             // variable Sollwertanhebung
#define SM_TABW     0             // Regelabweichungs-Alarm

// SiWa 27.02.18 
// --------------- WPU-Regelungen --------------------------------------
#define WPANZ       1             // projektierte Anzahl von Regelkreisen
#define WPMAX       1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)

// Einstellung für Parli: ke1[]
#define SOLLWERT_KE1 0            // Sollwert-Ausgabe
#define MODUL_KE1   0             // Modulation. 1=Dreipunkt-Ausgabe 2=Analogausgabe
#define STUFIG_KE1  0             // Kesselsteuerung 1-stufig=1  2-stufig=2  
#define RL_KE1      0             // RL-Begrenzung. 1=Dreipunkt-Ausgabe 2=Analogausgabe 
#define TV_KE1      0             // 1 = Kesselvorlauftemperatur anzeigen
#define TR_KE1      0             // 1 = Kesselrücklauftemperatur anzeigen
#define TVG_KE1     0             // 1 = gemeinsame Vorlauftemperatur anzeigen und benutzen in SteuerKe.c
#define TRG_KE1     0             // 1 = gemeinsame Rücklauftemperatur anzeigen und benutzen in SteuerKe.c
#define BM_KE1      0             // 1 = Betriebsmeldung KESSEL anzeigen
#define KEAL_KE1    0             // 1 = Kesselalarm (Si-Kette) anzeigen
#define BRAL_KE1    0             // 1 = Störung Brenner anzeigen
#define NACHSP_KE1  0             // 1 = Störung Nachspeisung anzeigen
#define NOTAUS_KE1  0             // 1 = Meldung Not-Aus anzeigen
#define STB_KE1     0             // 1 = STB anzeigen
#define DR_KE1      0             // 1 = Max-Druck anzeigen

#define PU_KE1      0             // 1 = Kesselpumpe anzeigen und benutzen (nicht setzen wenn über Genibus gesteuert)
#define BM_PUKE1    0             // 1 = Betriebsmeldung KESSEL anzeigen
#define PUAL_KE1    0             // 1 = Störung Kesselpumpe anzeigen
#define DZ_PU_KE1   0             // 1 = Drehzahl-Regelung Kesselpumpe
#define KL_KE1      0             // 1 = Kesselabsperrklappe anzeigen und benutzen
#define PU_BUS_KE1  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// Einstellung für Parli: ke2[]
#define SOLLWERT_KE2 0            // Sollwert-Ausgabe
#define MODUL_KE2   0             // Modulation. 1=Dreipunkt-Ausgabe 2=Analogausgabe
#define STUFIG_KE2  0             // Kesselsteuerung 1-stufig=1  2-stufig=2  
#define RL_KE2      0             // RL-Begrenzung. 1=Dreipunkt-Ausgabe 2=Analogausgabe 
#define TV_KE2      0             // 1 = Kesselvorlauftemperatur anzeigen
#define TR_KE2      0             // 1 = Kesselrücklauftemperatur anzeigen
#define TVG_KE2     0             // 1 = gemeinsame Vorlauftemperatur anzeigen und benutzen in SteuerKe.c
#define TRG_KE2     0             // 1 = gemeinsame Rücklauftemperatur anzeigen und benutzen in SteuerKe.c
#define BM_KE2      0             // 1 = Betriebsmeldung KESSEL anzeigen
#define KEAL_KE2    0             // 1 = Kesselalarm (Si-Kette) anzeigen
#define BRAL_KE2    0             // 1 = Störung Brenner anzeigen
#define NACHSP_KE2  0             // 1 = Störung Nachspeisung anzeigen
#define NOTAUS_KE2  0             // 1 = Meldung Not-Aus anzeigen
#define STB_KE2     0             // 1 = STB anzeigen
#define DR_KE2      0             // 1 = Max-Druck anzeigen

#define PU_KE2      0             // 1 = Kesselpumpe anzeigen und benutzen (nicht setzen wenn über Genibus gesteuert)
#define BM_PUKE2    0             // 1 = Betriebsmeldung KESSEL anzeigen
#define PUAL_KE2    0             // 1 = Störung Kesselpumpe anzeigen
#define DZ_PU_KE2   0             // 1 = Drehzahl-Regelung Kesselpumpe
#define KL_KE2      0             // 1 = Kesselabsperrklappe anzeigen und benutzen
#define PU_BUS_KE2  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// --------------- Vor-Regelungen (Netztrennung) --------------------------
#define NTANZ       0             // projektierte Anzahl von Vorregelkreisen
#define NTMAX       1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)

// Einstellung für Parli: nt1[]
#define TVP_NT1     0             // Primärvorlauf  anzeigen
#define TRP_NT1     0             // Primärrücklauf anzeigen
#define DREIP_NT1   0             // 1=DreipunktAusgabe 0=Analogausgabe
#define STB_NT1     0             // STB-Alarm
#define STW_NT1     0             // STW-Alarm
#define PU_NT1      0             // Pumpe vorhanden ?  (nicht setzen wenn über Genibus gesteuert)
#define PULZ_NT1    0             // Laufzeit der Pumpe (nicht setzen wenn über Genibus gesteuert)
#define PUAL_NT1    0             // Pumpenalarm
#define PU_BUS_NT1  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PUBM_NT1    0             // Betriebsmeldung Pumpe

// Einstellung für Parli: nt2[]
#define TVP_NT2     0             // Primärvorlauf  anzeigen
#define TRP_NT2     0             // Primärrücklauf anzeigen
#define DREIP_NT2   0             // 1=DreipunktAusgabe 0=Analogausgabe
#define STB_NT2     0             // STB-Alarm
#define STW_NT2     0             // STW-Alarm
#define PU_NT2      0             // Pumpe vorhanden ?  (nicht setzen wenn über Genibus gesteuert)
#define PULZ_NT2    0             // Laufzeit der Pumpe (nicht setzen wenn über Genibus gesteuert)
#define PUAL_NT2    0             // Pumpenalarm
#define PU_BUS_NT2  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PUBM_NT2    0             // Betriebsmeldung Pumpe

// --------------- Solar-Regelungen ---------------------------------------
#define SOLANZ      0             // projektierte Anzahl von Solar-Regelkreisen
#define SOLMAX      1             // maximale Anzahl (wegen Speicherzuweisung mindestens 1)

#define PU_BUS_SO1  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index
#define PU_BUS_SO2  0             // ungleich 0: Pumpe busgesteuert, Wert = Parameter-Index

// --------------- Zähleingänge R36 ---------------------------------------
// Einstellung für Parli: ze[0] bis ze[7]
#define ZE3         0             // 0 = nicht projektiert  
#define ZE4         0 
#define ZE5         0 
#define ZE6         0 
#define ZE7         0 
#define ZE8         0 
#define ZE9         0
#define ZE10        0

#define ZE7WM       0             // 1/0 Wärmemenge berechnen / nicht berechnen
#define ZE8WM       0             // 1/0 Wärmemenge berechnen / nicht berechnen
#define ZE9WM       0             // 1/0 Wärmemenge berechnen / nicht berechnen
#define ZE10WM      0             // 1/0 Wärmemenge berechnen / nicht berechnen

#if ( ZE7WM || ZE8WM || ZE9WM || ZE10WM )
#define WMENG 1
#else     
#define WMENG 0
#endif

#define LEIST_BER_ZE7       0         // Berechnung der maximalen Durchschnittsleistung (int./ext.WMZ)
#define LEIST_BER_ZE8       0         // Berechnung der maximalen Durchschnittsleistung (int./ext.WMZ)
#define LEIST_BER_ZE9       0         // Berechnung der maximalen Durchschnittsleistung (int./ext.WMZ)
#define LEIST_BER_ZE10      0         // Berechnung der maximalen Durchschnittsleistung (int./ext.WMZ)

#if ( LEIST_BER_ZE7 || LEIST_BER_ZE8 || LEIST_BER_ZE9 || LEIST_BER_ZE10 )
#define LEIST_BER 1
#else     
#define LEIST_BER 0
#endif

// Leistungsbegrenzung: Momentanleistung eines externen WMZ (max. einen Zähler auswählen !) 
#define LEIST_BEGR_ZE7      0
#define LEIST_BEGR_ZE8      0
#define LEIST_BEGR_ZE9      0
#define LEIST_BEGR_ZE10     0
#define LEIST_BEGR_MBUS     0

#if ( LEIST_BEGR_ZE7 || LEIST_BEGR_ZE8 || LEIST_BEGR_ZE9 || LEIST_BEGR_ZE10 || LEIST_BEGR_MBUS )
#define LEIST_BEGR 1
#else     
#define LEIST_BEGR 0
#endif

// ---------------- Erweiterungsmodule R37 ----------------------------
// Einstellung für Parli: sys[]
#define R37_MODULE  0             // Anzahl der Module R37
#define R37_MODMAX  1             // wegen Speicherzuweisung mindestens 1

// Zähleingänge R37_1 Einstellung für Parli: x1ze[0] bis x1ze[5]
//                                und für Datenübertragung (InoutR.c) !
#define ZE11        0
#define ZE12        0
#define ZE13        0
#define ZE14        0
#define ZE15        0
#define ZE16        0
#define ZEX1BELEG   (ZE11*1 + ZE12*2 + ZE13*4 + ZE14*8 + ZE15*16 + ZE16*32)

// Zähleingänge R37_2 Einstellung für Parli: x2ze[0] bis x2ze[5]
//                                und für Datenübertragung (InoutR.c) !
#define ZE21        0
#define ZE22        0
#define ZE23        0
#define ZE24        0
#define ZE25        0
#define ZE26        0
#define ZEX2BELEG   (ZE21*1 + ZE22*2 + ZE23*4 + ZE24*8 + ZE25*16 + ZE26*32)

// ---------------- Erweiterungsmodule R38 ----------------------------
// Einstellung für Parli: sys[]
#define R38_MODULE  1             // Anzahl der Module R38
#define R38_MODMAX  1             // wegen Speicherzuweisung mindestens 1

// ---------------- Erweiterungsmodule R39 ----------------------------
// Einstellung für Parli: sys[]
#define R39_MODULE  0             // Anzahl der Module R39
#define R39_MODMAX  1             // wegen Speicherzuweisung mindestens 1

// ---------------- Erweiterungsmodule R33 ----------------------------
// Einstellung für Parli: sys[]
#define R33_MODULE  0             // Anzahl der Module R33
#define R33_MODMAX  1             // wegen Speicherzuweisung mindestens 1

// Gesamtanzahl der Module: max. 4  (R3MAX)


//---------------------------
// Anybus
//---------------------------
#if ( (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL )
#define ANYBUS_MODUL    1
#define KOMTAB_ALL      1     // 1= gesamte Komtab, 0=benutzerdefiniert in AnybusTabelle.c

#else
#define ANYBUS_MODUL    0
#endif

#endif  // PROJDEF_H_INCLUDED



