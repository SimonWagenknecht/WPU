#ifndef STANDDEF_H_INCLUDED
#define STANDDEF_H_INCLUDED


/*--------------------------------------------------------------*/
/*          Speicherplätze  im EEPROM AT24C256                    */
/*--------------------------------------------------------------*/
// Speicher: 256 KBit (32K x 8 BIT)     Adressbereich: 0 - 7FFFH
// Die Adressen 0 - 0x00FF sind für System reserviert 
// Frei für User: ab 0x0200

//  Achtung ! Pagegröße des EEPROM's (64) beachten, d.h.
//  int-Werte  müssen auf geraden Adressen (modulo 2) liegen,
//  long-Werte : modulo 4,
//  15-stellige ASCII-Ketten: modulo 16

// Im Regler R36B (Renesas CPL) wurde der EEPROM durch ein FERRO-RAM ersetzt. Der Begriff EEPROM wird weiterhin verwendet.

/* ------------------------ Projekt ------------------------------*/

// Erweiterungsmodule
#define R37_1_BUS_ADR       0x0200  // 2 Byte   BUS-Kennzeichen +  ADRESSE auf dem BUS
#define R37_2_BUS_ADR       0x0202
#define R37_3_BUS_ADR       0x0204
#define R37_4_BUS_ADR       0x0206
                            
#define R38_1_BUS_ADR       0x0208  // 2 Byte   BUS-Kennzeichen +  ADRESSE auf dem BUS
#define R38_2_BUS_ADR       0x020A
#define R38_3_BUS_ADR       0x020C
#define R38_4_BUS_ADR       0x020E
// Lifetest R38
#define R38_1_LIFE_ADR      0x0210  // 2 Byte   Bitmuster
#define R38_2_LIFE_ADR      0x0212
#define R38_3_LIFE_ADR      0x0214
#define R38_4_LIFE_ADR      0x0216
                            
#define R39_1_BUS_ADR       0x0218  // 2 Byte   BUS-Kennzeichen +  ADRESSE auf dem BUS
#define R39_2_BUS_ADR       0x021A
#define R39_3_BUS_ADR       0x021C
#define R39_4_BUS_ADR       0x021E
                            
#define R39_1_DIGART_ADR    0x0220  // 2 Byte
#define R39_2_DIGART_ADR    0x0222  // 2 Byte
#define R39_3_DIGART_ADR    0x0224  // 2 Byte
#define R39_4_DIGART_ADR    0x0226  // 2 Byte
                            
#define R33_1_BUS_ADR       0x0228  // 2 Byte   BUS-Kennzeichen +  ADRESSE auf dem BUS
#define R33_2_BUS_ADR       0x022A
#define R33_3_BUS_ADR       0x022C
#define R33_4_BUS_ADR       0x022E
                                    
// Skalierungen der Analogausgänge R37
#define R37_1_AA_SKAL_ADR   0x0230  // 16 Byte
#define R37_2_AA_SKAL_ADR   0x0240  // 16 Byte
#define R37_3_AA_SKAL_ADR   0x0250  // 16 Byte
#define R37_4_AA_SKAL_ADR   0x0260  // 16 Byte

// Projekttyp
#define PROJ_TYP_ADR        0x0270  // 1 Byte

/* ---------------------- Steuer ---------------------------------*/
#define TA1MZK_ADR          0x0280  // 1 Byte
#define TA1MHZK_ADR         0x0281  // 1 Byte 
#define FL_TA1MH_ADR        0x0282  // 4 Byte
#define BC_VORRA_ADR        0x0286  // 1 Byte
#define SCHALT_PAUSE_ADR    0x0287  // 1 Byte
#define TAE_ADR             0x0288  // 2 Byte
#define TAA_ADR             0x028A  // 2 Byte
#define VERSION_ADR         0x028C  // 3 Byte

//$ Test
#define ASDM_TEST_START_ADR 0x0290  // 4 Byte

                                    // Gap 49 Byte
#define AA_SKAL_ADR         0x02C0  // 8 Byte                       
                                    // Gap 56 Byte
#define TA_ERSATZ_ADR       0x02C8  // 24 Byte
#define TA_ERSATZ_VERZ_ADR  0x02E0  // 1 Byte 

// ***** josch: Datenmanager
#define SLAVE_ADR           0x02F0  // 4 Byte   Slave-Adressen

/* ---------------------- Heizkreise ------------------------------*/
// Regler HK1
#define HK1_ADR             0x0300  // 2 Page
// Absenkungen HK1          
#define ABS10_BEGIN_ADR     0x03C0  // 1 Page

// Regler HK2               
#define HK2_ADR             0x0400  // 2 Page
// Absenkungen HK2          
#define ABS20_BEGIN_ADR     0x04C0  // 1 Page 

// Regler HK3               
#define HK3_ADR             0x0500  // 2 Page
// Absenkungen HK2          
#define ABS30_BEGIN_ADR     0x05C0  // 1 Page 

// Regler HK4               
#define HK4_ADR             0x0600  // 2 Page
// Absenkungen HK2          
#define ABS40_BEGIN_ADR     0x06C0  // 1 Page 

//-// Regler HK5
//-#define HK5_ADR             0x0700  // 2 Page
//-// Absenkungen HK1          
//-#define ABS50_BEGIN_ADR     0x07C0  // 1 Page
//-
//-// Regler HK6
//-#define HK6_ADR             0x0800  // 2 Page
//-// Absenkungen HK6          
//-#define ABS60_BEGIN_ADR     0x08C0  // 1 Page
//-
//-// Regler HK7
//-#define HK7_ADR             0x0900  // 2 Page
//-// Absenkungen HK7          
//-#define ABS70_BEGIN_ADR     0x09C0  // 1 Page
//-
//-// Regler HK8
//-#define HK8_ADR             0x0A00  // 2 Page
//-// Absenkungen HK8          
//-#define ABS80_BEGIN_ADR     0x0AC0  // 1 Page
//-
//-
//-/* ------------------------ Kessel ------------------------------------*/
//-// Kessel 1                                   
//-#define KE1_ADR             0x0B00  // 2 Page 
//-                            
//-// Kessel 2                 
//-#define KE2_ADR             0x0B80  // 2 Page
//-                            
//-// Kessel 3                                       
//-#define KE3_ADR             0x0C00  // 2 Page 
//-                            
//-// Kessel 4                 
//-#define KE4_ADR             0x0C80  // 2 Page

#define	KE_ADR							0x0700			// 3 Pages pro Kessel
#define	KE_PAR_LENG					0xC0				// 8 Kessel belegen 24 Pages, d.h.  0x0700...0x0CFF


/* ------------------------ Warmwasser ---------------------------------*/
// Warmwasser 1                         
#define WW1_ADR             0x0D00  // 2 Page

// Warmwasser 2 
#define WW2_ADR             0x0D80  // 2 Page
                                        
/* ------------------------ Netztrennung -------------------------------*/
// Netztrennung 1
#define NT1_ADR             0x0E00  // 2 Page

// Netztrennung 2
#define NT2_ADR             0x0E80  // 2 Page

/* ------------------------ Solar-Regelung -----------------------------*/
// Solar-Regelung 1
#define SO1_ADR             0x0F00  // 2 Page

//// Solar-Regelung 2
//#define SO2_ADR             0x0F80  // 2 Page

/* ------------------------ WPU-Regelung -----------------------------*/
#define WP1_ADR             0x0F80  // 2 Page

/*----------------------------------------------------------------------*/
/*          Speicherplätze für Laufzeiten                               */
/*----------------------------------------------------------------------*/

#define KESTARTS_ADR				0x1000	// 2 byte, Kesselstarts, für 8 Kessel

#define KESH_ADR						0x1010	// 2 byte, Kessellaufzeit, für 8 Kessel

#define KES2H_ADR						0x1020	// 2 byte, Kessellaufzeit 2.Stufe, für 8 Kessel

// #define PUWLSTA1_ADR				0x1030	// 2 byte		Ladepumpenstarts (2 WW)
#define WPUSTA_ADR					0x1030		// 2 byte		WPU-Starts (Freigabe + BM)

//#define PUWLH1_ADR					0x1038	// 2 byte		Ladepumpenlaufzeit (2 WW)
#define WPULaufzeit_ADR			0x1038	// 2 byte		WPU-Laufzeit

#define SOLH1_ADR						0x1040	// 2 byte		Kollektorpumpenlaufzeit

#define PU_NT_H_ADR					0x1048	// Pumpenlaufzeit, Hauptpumpe (Netztrennung), je 2 Byte

#define PU_HK_H_ADR					0x1050	// Pumpenlaufzeiten, 4 Heizkreise, je 2 Byte

#define PUD_HK_H_ADR				0x1058	// Doppelpumpe: Laufzeiten 2.Pumpe, 4 Heizkreise, je 2 Byte

#define WTH_ADR							0x1060	// Tauscher-Laufzeit (2 Tauscher, Regel2WT.c)

#define PUWZH1_ADR					0x1070	// 2 byte		Zirkpumpenlaufzeit

/*----------------------------------------------------------------------*/
/*          Speicherplätze für Raumnamen                                */
/*----------------------------------------------------------------------*/
#define RAUMNAME1_ADR       0x1100  // 16 Byte
//                          0x1110  // 16 Byte
//                          0x1120  // 16 Byte
//                          0x1130  // 16 Byte

//------------------------------------------------------------------
#define TA_OUT_ADR          0x1140  // 4 Byte
                                    // int TaaSkalMin;
                                    // int TaaSkalMax;

#define TA_IN_ADR           0x1144  // 4 Byte
                                    // int TaeSkalMin;
                                    // int TaeSkalMax;
                                                                        
#define TMANF_ADR           0x1150  // 8 Byte
                                    // extern int TmanfSkalMin;
                                    // extern int TmanfSkalMax;
                                    // extern int TmanfSkalMinSpg;
                                    // extern int TmanfSkalMaxSpg;
                                    
/* ------------------------ Benutzersteuerung mit UNI-Elementen --------*/
#define STEUER_UNI_ADR      0x1180  // 1 Page
                                                                      
//------------------------------------------------------------------
// Adaption
//------------------------------------------------------------------
#define ADAPT_KORR_ADR      0x1400    // fuer 8 HK 8*20 Byte temperaturspez.Korrekturwerte, in 0,1 grd C
#define ADAPT_KORR_LNG      0x20      // für jeden Heizkreis max. 32 Byte -> insgesamt 4 Pages

// Ferienprogramm
#define FERIEN_ADR          0x1500    // 10 mal Ferienbeginn, Ferienende ( 10 * 2 * 3 = 60 Byte )


// skalierbare Analogeingänge
#define ANAINPPARA_ADR      0x1580    // 2 Pages belegt (max. 12 Byte Parameter pro Eingang)
#define DRUCKPARA_ADR      	0x1600    // 2 Pages belegt (12 Byte Parameter pro Eingang)
#define RMPOWERPARA_ADR     0x1680 		// 1 Page (6 Byte Parameter pro Eingang)

// MBUS
#define MBUS_PARA_ADR       0x1700    // 1 Pages für max. 8 Zähler
#define MBUS_PARA_LENG      8         // maximal 8 Byte pro Zähler, MBUS_PARA_LENG <= MBUSLENG, 
#define MBUSFREQU_ADR       0x1740                                  
#define MBUSAKT_ADR         0x1742                                  

//-// Zählwerte der MBus-Zähler am Monatsanfang zur Berechnung der Verbräuche
//-#define ZMB1_MONANF_ADR      0x1760
//-//-#define ZMB2_MONANF_ADR     0x1764  0x1768
//-//-#define ZMB3_MONANF_ADR     0x1768  0x1770
//-//optionale Erweiterung auf 6 Zähler
//-//-#define ZMB4_MONANF_ADR     0x176C  0x1778
//-//-#define ZMB5_MONANF_ADR     0x1770  0x1780
//-//-#define ZMB6_MONANF_ADR     0x1774  0x1788
//-//-#define ZMB_MONANF_OFFS   4
//-#define ZMB_MONANF_OFFS    8
//-

#define LEISTUNG_ANZBEG   0x1750    // 2 Byte
#define LEISTUNG_DAUBEG   0x1752    // 2 Byte

//------------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++
// Zählerdimensionierungen
//++++++++++++++++++++++++++++++++++++++++++++++++
// Offset zum nächsten Zähler
#define ZAEHL_DIM_OFFS      0x30        // 48 Bytes
// Member-Offset entsprechend zdim Zählerstruktur
#define KOMMA_OFFS            0         // 1 Byte
#define DIM_IDX_OFFS          1         // 1 Byte
#define FAKTOR_OFFS           2         // 2 Byte
#define EXPONENT_OFFS         4         // 1 Byte
#define KONFIG_OFFS           5         // 1 Byte
#define MEDIUM_OFFS           6         // 1 Byte
#define NAME_IDX_OFFS         7         // 1 Byte
#define STICHTAG_OFFS         8         // 3 Byte
#define RESERVE_OFFS         11         // 5 Byte
#define ZNAME_OFFS           16         // 16 Byte
#define ZNUMM_OFFS           32         // 16 Byte

// Zählerdimensionierung  R36 
#define ZAEHL01_DIM_ADR   0x1800    // 11 Bytes + 5 Bytes Gap
#define ZAEHL01_NAME_ADR  0x1810    // 16 Byte
#define ZAEHL01_NUMM_ADR  0x1820    // 16 Byte

#define ZAEHL02_DIM_ADR   0x1830
#define ZAEHL02_NAME_ADR  0x1840
#define ZAEHL02_NUMM_ADR  0x1850

#define ZAEHL03_DIM_ADR   0x1860
#define ZAEHL03_NAME_ADR  0x1870
#define ZAEHL03_NUMM_ADR  0x1880

#define ZAEHL04_DIM_ADR   0x1890
#define ZAEHL04_NAME_ADR  0x18A0
#define ZAEHL04_NUMM_ADR  0x18B0

#define ZAEHL05_DIM_ADR   0x18C0
#define ZAEHL05_NAME_ADR  0x18D0
#define ZAEHL05_NUMM_ADR  0x18E0

#define ZAEHL06_DIM_ADR   0x18F0
#define ZAEHL06_NAME_ADR  0x1900
#define ZAEHL06_NUMM_ADR  0x1910

#define ZAEHL07_DIM_ADR   0x1920
#define ZAEHL07_NAME_ADR  0x1930
#define ZAEHL07_NUMM_ADR  0x1940

#define ZAEHL08_DIM_ADR   0x1950
#define ZAEHL08_NAME_ADR  0x1960
#define ZAEHL08_NUMM_ADR  0x1970

// Zählerdimensionierung  R37_1 
#define ZAEHL11_DIM_ADR   0x1980    // 11 Bytes + 5 Bytes Gap 
#define ZAEHL11_NAME_ADR  0x1990    // 16 Byte
#define ZAEHL11_NUMM_ADR  0x19A0    // 16 Byte

#define ZAEHL12_DIM_ADR   0x19B0
#define ZAEHL12_NAME_ADR  0x19C0
#define ZAEHL12_NUMM_ADR  0x19D0

#define ZAEHL13_DIM_ADR   0x19E0
#define ZAEHL13_NAME_ADR  0x19F0
#define ZAEHL13_NUMM_ADR  0x1A00

#define ZAEHL14_DIM_ADR   0x1A10
#define ZAEHL14_NAME_ADR  0x1A20
#define ZAEHL14_NUMM_ADR  0x1A30

#define ZAEHL15_DIM_ADR   0x1A40
#define ZAEHL15_NAME_ADR  0x1A50
#define ZAEHL15_NUMM_ADR  0x1A60

#define ZAEHL16_DIM_ADR   0x1A70
#define ZAEHL16_NAME_ADR  0x1A80
#define ZAEHL16_NUMM_ADR  0x1A90

// Zählerdimensionierung  R37_2 
#define ZAEHL21_DIM_ADR   0x1AA0    // 11 Bytes + 5 Bytes Gap
#define ZAEHL21_NAME_ADR  0x1AB0    // 16 Byte
#define ZAEHL21_NUMM_ADR  0x1AC0    // 16 Byte

#define ZAEHL22_DIM_ADR   0x1AD0
#define ZAEHL22_NAME_ADR  0x1AE0
#define ZAEHL22_NUMM_ADR  0x1AF0

#define ZAEHL23_DIM_ADR   0x1B00
#define ZAEHL23_NAME_ADR  0x1B10
#define ZAEHL23_NUMM_ADR  0x1B20

#define ZAEHL24_DIM_ADR   0x1B30
#define ZAEHL24_NAME_ADR  0x1B40
#define ZAEHL24_NUMM_ADR  0x1B50

#define ZAEHL25_DIM_ADR   0x1B60
#define ZAEHL25_NAME_ADR  0x1B70
#define ZAEHL25_NUMM_ADR  0x1B80

#define ZAEHL26_DIM_ADR   0x1B90
#define ZAEHL26_NAME_ADR  0x1BA0
#define ZAEHL26_NUMM_ADR  0x1BB0


//#####MBus
#define ZAEHLMB1_DIM_ADR    0x1BC0
#define ZAEHLMB1_NAME_ADR   0x1BD0
#define ZAEHLMB1_NUMM_ADR   0x1BE0

#define ZAEHLMB2_DIM_ADR    0x1BF0
#define ZAEHLMB2_NAME_ADR   0x1C00
#define ZAEHLMB2_NUMM_ADR   0x1C10

#define ZAEHLMB3_DIM_ADR    0x1C20
#define ZAEHLMB3_NAME_ADR   0x1C30
#define ZAEHLMB3_NUMM_ADR   0x1C40

#define ZAEHLMB4_DIM_ADR    0x1C50
#define ZAEHLMB4_NAME_ADR   0x1C60
#define ZAEHLMB4_NUMM_ADR   0x1C70

#define ZAEHLMB5_DIM_ADR    0x1C80
#define ZAEHLMB5_NAME_ADR   0x1C90
#define ZAEHLMB5_NUMM_ADR   0x1CA0

#define ZAEHLMB6_DIM_ADR    0x1CB0
#define ZAEHLMB6_NAME_ADR   0x1CC0
#define ZAEHLMB6_NUMM_ADR   0x1CD0

#define ZAEHLMB7_DIM_ADR    0x1CE0
//#define ZAEHLMB7_NAME_ADR   0x1CF0
//#define ZAEHLMB7_NUMM_ADR   0x1D00

#define ZAEHLMB8_DIM_ADR    0x1D10
//#define ZAEHLMB8_NAME_ADR   0x1D20
//#define ZAEHLMB8_NUMM_ADR   0x1D30



// Speicher für aktuellen Zählwert (Sicherung jede Minute)
// R36
  #define ZAEHLER01_ADR   0x2000    // 8 Byte   
//#define ZAEHLER02_ADR   0x2008    // 8 Byte   
//#define ZAEHLER03_ADR   0x2010    // 8 Byte   
//#define ZAEHLER04_ADR   0x2018    // 8 Byte   
//#define ZAEHLER05_ADR   0x2020    // 8 Byte   
//#define ZAEHLER06_ADR   0x2028    // 8 Byte   
//#define ZAEHLER07_ADR   0x2030    // 8 Byte   
//#define ZAEHLER08_ADR   0x2038    // 8 Byte   

// R37_1
  #define ZAEHLER11_ADR   0x2040    // 8 Byte   
//#define ZAEHLER12_ADR   0x2048    // 8 Byte   
//#define ZAEHLER13_ADR   0x2050    // 8 Byte   
//#define ZAEHLER14_ADR   0x2058    // 8 Byte   
//#define ZAEHLER15_ADR   0x2060    // 8 Byte   
//#define ZAEHLER16_ADR   0x2068    // 8 Byte   

// R37_2
  #define ZAEHLER21_ADR   0x2080    // 8 Byte   
//#define ZAEHLER22_ADR   0x2088    // 8 Byte   
//#define ZAEHLER23_ADR   0x2090    // 8 Byte   
//#define ZAEHLER24_ADR   0x2098    // 8 Byte   
//#define ZAEHLER25_ADR   0x20A0    // 8 Byte   
//#define ZAEHLER26_ADR   0x20A8    // 8 Byte   

// MBus
  #define ZAEHLERMB1_ADR  0x20C0    // 8 Byte   
//#define ZAEHLERMB2_ADR  0x20C8    // 8 Byte   
//#define ZAEHLERMB3_ADR  0x20D0    // 8 Byte   
//optionale Erweiterung auf 6 Zähler
//#define ZAEHLERMB4_ADR  0x20D8    // 8 Byte   
//#define ZAEHLERMB5_ADR  0x20E0    // 8 Byte   
//#define ZAEHLERMB6_ADR  0x20E8    // 8 Byte   
//optionale Erweiterung auf 8 Zähler
//#define ZAEHLERMB7_ADR  0x20F0    // 8 Byte   
//#define ZAEHLERMB8_ADR  0x20F8    // 8 Byte  
// Speicher für Vorjahres-Zählwert
#define ZVJ_OFFSET        0x100
// R36
  #define VJZAEHL01_ADR   0x2100    // 8 Byte   
//#define VJZAEHL02_ADR   0x2108    // 8 Byte   
//#define VJZAEHL03_ADR   0x2110    // 8 Byte   
//#define VJZAEHL04_ADR   0x2118    // 8 Byte   
//#define VJZAEHL05_ADR   0x2120    // 8 Byte   
//#define VJZAEHL06_ADR   0x2128    // 8 Byte   
//#define VJZAEHL07_ADR   0x2130    // 8 Byte   
//#define VJZAEHL08_ADR   0x2138    // 8 Byte   

// R37_1
  #define VJZAEHL11_ADR   0x2140    // 8 Byte   
//#define VJZAEHL12_ADR   0x2148    // 8 Byte   
//#define VJZAEHL13_ADR   0x2150    // 8 Byte   
//#define VJZAEHL14_ADR   0x2158    // 8 Byte   
//#define VJZAEHL15_ADR   0x2160    // 8 Byte   
//#define VJZAEHL16_ADR   0x2168    // 8 Byte   

// R37_2
  #define VJZAEHL21_ADR   0x2180    // 8 Byte   
//#define VJZAEHL22_ADR   0x2188    // 8 Byte   
//#define VJZAEHL23_ADR   0x2190    // 8 Byte   
//#define VJZAEHL24_ADR   0x2198    // 8 Byte   
//#define VJZAEHL25_ADR   0x21A0    // 8 Byte   
//#define VJZAEHL26_ADR   0x21A8    // 8 Byte   

// MBus
  #define VJZAEHLMB1_ADR  0x21C0    // 8 Byte   
//#define VJZAEHLMB2_ADR  0x21C8    // 8 Byte   
//#define VJZAEHLMB3_ADR  0x21D0    // 8 Byte   
//optionale Erweiterung auf 6 Zähler
//#define VJZAEHLMB4_ADR  0x21D8    // 8 Byte   
//#define VJZAEHLMB5_ADR  0x21E0    // 8 Byte   
//#define VJZAEHLMB6_ADR  0x21E8    // 8 Byte   
//optionale Erweiterung auf 8 Zähler
//#define VJZAEHLMB7_ADR  0x21F0    // 8 Byte   
//#define VJZAEHLMB8_ADR  0x21F8    // 8 Byte   

// Speicher für Verbrauchswerte des aktuellen Monats  (Sicherung jede Minute) 
// R36
  #define VERBRAU01_ADR   0x2200    // 4 Byte   
//#define VERBRAU02_ADR   0x2204    // 4 Byte   
//#define VERBRAU03_ADR   0x2208    // 4 Byte   
//#define VERBRAU04_ADR   0x220C    // 4 Byte   
//#define VERBRAU05_ADR   0x2210    // 4 Byte   
//#define VERBRAU06_ADR   0x2214    // 4 Byte   
//#define VERBRAU07_ADR   0x2218    // 4 Byte   
//#define VERBRAU08_ADR   0x221C    // 4 Byte   

// R37_1
  #define VERBRAU11_ADR   0x2220    // 4 Byte   
//#define VERBRAU12_ADR   0x2224    // 4 Byte   
//#define VERBRAU13_ADR   0x2228    // 4 Byte   
//#define VERBRAU14_ADR   0x222C    // 4 Byte   
//#define VERBRAU15_ADR   0x2230    // 4 Byte   
//#define VERBRAU16_ADR   0x2234    // 4 Byte   
//                        0x2230
//                        0x2234
// R37_2
  #define VERBRAU21_ADR   0x2240    // 4 Byte   
//#define VERBRAU22_ADR   0x2244    // 4 Byte   
//#define VERBRAU23_ADR   0x2248    // 4 Byte   
//#define VERBRAU24_ADR   0x224C    // 4 Byte   
//#define VERBRAU25_ADR   0x2250    // 4 Byte   
//#define VERBRAU26_ADR   0x2254    // 4 Byte   
//                        0x2250
//                        0x2254

// MBus
  #define VERBRAUMB1_ADR  0x2280    // 4 Byte 
//#define VERBRAUMB2_ADR  0x2284    // 4 Byte
//#define VERBRAUMB3_ADR  0x2288    // 4 Byte 
//#define VERBRAUMB4_ADR  0x228C    // 4 Byte 
//#define VERBRAUMB5_ADR  0x2290    // 4 Byte 
//#define VERBRAUMB6_ADR  0x2294    // 4 Byte 
//#define VERBRAUMB7_ADR  0x2298    // 4 Byte 
//#define VERBRAUMB8_ADR  0x229C    // 4 Byte 
  
  
// Monatspeicher für Verbräuche (4 Byte pro Monat) (Sicherung jede Stunde) 
// 2 * 48 Byte Verbrauch akt. Jahr + Vorjahr
#define ZMV_OFFSET        0x80    // Offset zur nächsten Zähleradresse
#define ZMV_VJOFFS        0x40    // Offset zum Vorjahresbereich innerhalb eines Zählerspeicherbereiches
// R36
  #define ZMV01_ADR       0x2300  // (64) 12*4 Byte aktuell + 16 Byte gap   +   (64) 12*4 Byte Vorjahr + 16 Byte gap
//#define ZMV02_ADR       0x2380
//#define ZMV03_ADR       0x2400
//#define ZMV04_ADR       0x2480
//#define ZMV05_ADR       0x2500
//#define ZMV06_ADR       0x2580
//#define ZMV07_ADR       0x2600
//#define ZMV08_ADR       0x2680

// R37_1
  #define ZMV11_ADR       0x2700
//#define ZMV12_ADR       0x2780
//#define ZMV13_ADR       0x2800
//#define ZMV14_ADR       0x2880
//#define ZMV15_ADR       0x2900
//#define ZMV16_ADR       0x2980
//                        0x2A00
//                        0x2A80
// R37_2
  #define ZMV21_ADR       0x2B00
//#define ZMV22_ADR       0x2B80
//#define ZMV23_ADR       0x2C00
//#define ZMV24_ADR       0x2C80
//#define ZMV25_ADR       0x2D00
//#define ZMV26_ADR       0x2D80
//                        0x2E00
//                        0x2E80
// MBus
  #define ZMVMB1_ADR      0x2F00
//#define ZMVMB2_ADR      0x2F80
//#define ZMVMB3_ADR      0x3000
//#define ZMVMB4_ADR      0x3080
//#define ZMVMB5_ADR      0x3100
//#define ZMVMB6_ADR      0x3180
//#define ZMVMB7_ADR      0x3200
//#define ZMVMB8_ADR      0x3280

//++++++++++++++++++++++++++++++++++++++++++++++++
// Zähler-Fehlerspeicher
//++++++++++++++++++++++++++++++++++++++++++++++++
#define ZFEHLER_OFFS      0x20    // 32 Byte Offset zum nächsten Zähler
// R36
#define Z01LH_ADR         0x3300  // 6 Byte Datum     letzte  Low-High-Flanke 
#define Z01HL_ADR         0x3306  // 6 Byte Datum     letzte  High-Low-Flanke
#define Z01EF_ADR         0x330C  // 6 Byte Datum     Erstes  Auftreten
#define Z01EFA_ADR        0x3312  // 1 Byte Fehlerart Erstes  Auftreten
#define Z01LF_ADR         0x3313  // 6 Byte Datum     Letztes Auftreten
#define Z01LFA_ADR        0x3319  // 1 Byte Fehlerart Letztes Auftreten
#define CLEARDONGLE01_ADR 0x331A  // 2 Byte Dongle-Nummer des PC-Bedieners
#define CLEARTAG01_ADR    0x331C  // 1 Byte Fehlerlöschung Tag
#define CLEARMON01_ADR    0x331D  // 1 Byte Fehlerlöschung Monat
#define CLEARJAHR01_ADR   0x331E  // 1 Byte Fehlerlöschung Jahr (+1 Gap)

//#define Z02LH_ADR       0x3320  // 32 Byte
//#define Z03LH_ADR       0x3340  // 32 Byte
//#define Z04LH_ADR       0x3360  // 32 Byte
//#define Z05LH_ADR       0x3380  // 32 Byte
//#define Z06LH_ADR       0x33A0  // 32 Byte
//#define Z07LH_ADR       0x33C0  // 32 Byte
//#define Z08LH_ADR       0x33E0  // 32 Byte
#define CLEARDONGLE_OFFS  (CLEARDONGLE01_ADR - Z01LH_ADR)

// R37_1
#define Z11LH_ADR         0x3400  // 6 Byte Datum     letzte  Low-High-Flanke 
#define Z11HL_ADR         0x3406  // 6 Byte Datum     letzte  High-Low-Flanke
#define Z11EF_ADR         0x340C  // 6 Byte Datum     Erstes  Auftreten
#define Z11EFA_ADR        0x3412  // 1 Byte Fehlerart Erstes  Auftreten
#define Z11LF_ADR         0x3413  // 6 Byte Datum     Letztes Auftreten
#define Z11LFA_ADR        0x3419  // 1 Byte Fehlerart Letztes Auftreten
#define CLEARDONGLE11_ADR 0x341A  // 2 Byte Dongle-Nummer des PC-Bedieners
#define CLEARTAG11_ADR    0x341C  // 1 Byte Fehlerlöschung Tag
#define CLEARMON11_ADR    0x341D  // 1 Byte Fehlerlöschung Monat
#define CLEARJAHR11_ADR   0x341E  // 1 Byte Fehlerlöschung Jahr (+1 Gap)
//#define Z12LH_ADR       0x3420  // 32 Byte
//#define Z13LH_ADR       0x3440  // 32 Byte
//#define Z14LH_ADR       0x3460  // 32 Byte
//#define Z15LH_ADR       0x3480  // 32 Byte
//#define Z16LH_ADR       0x34A0  // 32 Byte

// R37_2
#define Z21LH_ADR         0x3500  // 6 Byte Datum     letzte  Low-High-Flanke 
#define Z21HL_ADR         0x3506  // 6 Byte Datum     letzte  High-Low-Flanke
#define Z21EF_ADR         0x350C  // 6 Byte Datum     Erstes  Auftreten
#define Z21EFA_ADR        0x3512  // 1 Byte Fehlerart Erstes  Auftreten
#define Z21LF_ADR         0x3513  // 6 Byte Datum     Letztes Auftreten
#define Z21LFA_ADR        0x3519  // 1 Byte Fehlerart Letztes Auftreten
#define CLEARDONGLE21_ADR 0x351A  // 2 Byte Dongle-Nummer des PC-Bedieners
#define CLEARTAG21_ADR    0x351C  // 1 Byte Fehlerlöschung Tag
#define CLEARMON21_ADR    0x351D  // 1 Byte Fehlerlöschung Monat
#define CLEARJAHR21_ADR   0x351E  // 1 Byte Fehlerlöschung Jahr (+1 Gap)
//#define Z22LH_ADR       0x3520  // 32 Byte
//#define Z23LH_ADR       0x3540  // 32 Byte
//#define Z24LH_ADR       0x3560  // 32 Byte
//#define Z25LH_ADR       0x3580  // 32 Byte
//#define Z26LH_ADR       0x35A0  // 32 Byte

// MBus
#define ZMB1LH_ADR        0x3600  // 6 Byte Datum     letzte  Low-High-Flanke 
#define ZMB1HL_ADR        0x3606  // 6 Byte Datum     letzte  High-Low-Flanke
#define ZMB1EF_ADR        0x360C  // 6 Byte Datum     Erstes  Auftreten
#define ZMB1EFA_ADR       0x3612  // 1 Byte Fehlerart Erstes  Auftreten
#define ZMB1LF_ADR        0x3613  // 6 Byte Datum     Letztes Auftreten
#define ZMB1LFA_ADR       0x3619  // 1 Byte Fehlerart Letztes Auftreten
#define CLEARDONGLEMB1_ADR  0x361A  // 2 Byte Dongle-Nummer des PC-Bedieners
#define CLEARTAGMB1_ADR   0x361C  // 1 Byte Fehlerlöschung Tag
#define CLEARMONMB1_ADR   0x361D  // 1 Byte Fehlerlöschung Monat
#define CLEARJAHRMB1_ADR  0x361E  // 1 Byte Fehlerlöschung Jahr (+1 Gap)
//#define ZMB2LH_ADR      0x3620  // 32 Byte
//#define ZMB3LH_ADR      0x3640  // 32 Byte
//#define ZMB4LH_ADR      0x3660  // 32 Byte
//#define ZMB5LH_ADR      0x3680  // 32 Byte
//#define ZMB6LH_ADR      0x36A0  // 32 Byte
//#define ZMB7LH_ADR      0x36C0  // 32 Byte
//#define ZMB8LH_ADR      0x36E0  // 32 Byte

//++++++++++++++++++++++++++++++++++++++++++++++++
// Zähler-Initialisierung
//++++++++++++++++++++++++++++++++++++++++++++++++
#define ZINIT_OFFS            8       // 8 Byte Offset zum nächsten Zähler
// R36
#define INITDONGLE01_ADR      0x3700  // 2 Byte Dongle-Nummer des PC-Bedieners
#define INITTAG01_ADR         0x3702  // 1 Byte Initialisierung Tag
#define INITMON01_ADR         0x3703  // 1 Byte Initialisierung Monat
#define INITJAHR01_ADR        0x3704  // 1 Byte Initialisierung Jahr (+3 Gap)
//#define INITDONGLE02_ADR    0x3708  // 8 Byte 
//#define INITDONGLE03_ADR    0x3710  // 8 Byte 
//#define INITDONGLE04_ADR    0x3718  // 8 Byte 
//#define INITDONGLE05_ADR    0x3728  // 8 Byte 
//#define INITDONGLE06_ADR    0x3730  // 8 Byte 
//#define INITDONGLE07_ADR    0x3738  // 8 Byte 
//#define INITDONGLE08_ADR    0x3740  // 8 Byte 

// R37_1
#define INITDONGLE11_ADR      0x3750  // 2 Byte Dongle-Nummer des PC-Bedieners
#define INITTAG11_ADR         0x3752  // 1 Byte Initialisierung Tag
#define INITMON11_ADR         0x3753  // 1 Byte Initialisierung Monat
#define INITJAHR11_ADR        0x3754  // 1 Byte Initialisierung Jahr (+3 Gap)
//#define INITDONGLE12_ADR    0x3758  // 8 Byte
//#define INITDONGLE13_ADR    0x3760  // 8 Byte
//#define INITDONGLE14_ADR    0x3768  // 8 Byte
//#define INITDONGLE15_ADR    0x3770  // 8 Byte
//#define INITDONGLE16_ADR    0x3678  // 8 Byte

// R37_2
#define INITDONGLE21_ADR      0x3780  // 2 Byte Dongle-Nummer des PC-Bedieners
#define INITTAG21_ADR         0x3782  // 1 Byte Initialisierung Tag
#define INITMON21_ADR         0x3783  // 1 Byte Initialisierung Monat
#define INITJAHR21_ADR        0x3784  // 1 Byte Initialisierung Jahr (+3 Gap)
//#define INITDONGLE22_ADR    0x3788  // 8 Byte
//#define INITDONGLE23_ADR    0x3790  // 8 Byte
//#define INITDONGLE24_ADR    0x3798  // 8 Byte
//#define INITDONGLE25_ADR    0x37A0  // 8 Byte
//#define INITDONGLE26_ADR    0x37A8  // 8 Byte

// MBus
#define INITDONGLEMB1_ADR     0x37B0  // 2 Byte Dongle-Nummer des PC-Bedieners
#define INITTAGMB1_ADR        0x37B2  // 1 Byte Initialisierung Tag
#define INITMONMB1_ADR        0x37B3  // 1 Byte Initialisierung Monat
#define INITJAHRMB1_ADR       0x37B4  // 1 Byte Initialisierung Jahr (+3 Gap)
//#define INITDONGLEMB2_ADR   0x37B8  // 8 Byte
//#define INITDONGLEMB3_ADR   0x37C0  // 8 Byte
//#define INITDONGLEMB4_ADR   0x37C8  // 8 Byte
//#define INITDONGLEMB5_ADR   0x37D0  // 8 Byte
//#define INITDONGLEMB6_ADR   0x37D8  // 8 Byte
//#define INITDONGLEMB7_ADR   0x37E0  // 8 Byte
//#define INITDONGLEMB8_ADR   0x37E8  // 8 Byte

//++++++++++++++++++++++++++++++++++++++++++++++++
// Zähler-Synchronisierung
//++++++++++++++++++++++++++++++++++++++++++++++++
#define SYNC_OFFS             0x18    // 24 Byte Offset zum nächsten Zähler
// R36
#define SYNCAKTU01_ADR        0x3800  // 8 Byte Synchronisationswert aktuell
#define SYNCVORJ01_ADR        0x3808  // 8 Byte Synchronisationswert Vorjahr
#define SYNCDONGLE01_ADR      0x3810  // 2 Byte Dongle-Nummer des PC-Bedieners
#define SYNCTAG01_ADR         0x3812  // 1 Byte Synchronisation Tag
#define SYNCMON01_ADR         0x3813  // 1 Byte Synchronisation Monat
#define SYNCJAHR01_ADR        0x3814  // 1 Byte Synchronisation Jahr (+3 Gap)
//#define SYNCAKTU02_ADR      0x3818  // 24 Byte 
//#define SYNCAKTU03_ADR      0x3830  // 24 Byte 
//#define SYNCAKTU04_ADR      0x3848  // 24 Byte 
//#define SYNCAKTU05_ADR      0x3860  // 24 Byte 
//#define SYNCAKTU06_ADR      0x3878  // 24 Byte 
//#define SYNCAKTU07_ADR      0x3890  // 24 Byte 
//#define SYNCAKTU08_ADR      0x38A8  // 24 Byte 
#define SDONGLE_OFF           (SYNCDONGLE01_ADR - SYNCAKTU01_ADR)

// R37_1
#define SYNCAKTU11_ADR        0x3900  // 8 Byte Synchronisationswert aktuell
#define SYNCVORJ11_ADR        0x3908  // 8 Byte Synchronisationswert Vorjahr
#define SYNCDONGLE11_ADR      0x3910  // 2 Byte Dongle-Nummer des PC-Bedieners
#define SYNCTAG11_ADR         0x3912  // 1 Byte Synchronisation Tag
#define SYNCMON11_ADR         0x3913  // 1 Byte Synchronisation Monat
#define SYNCJAHR11_ADR        0x3914  // 1 Byte Synchronisation Jahr (+3 Gap)
//#define SYNCAKTU12_ADR      0x3918  // 24 Byte 
//#define SYNCAKTU13_ADR      0x3930  // 24 Byte 
//#define SYNCAKTU14_ADR      0x3948  // 24 Byte 
//#define SYNCAKTU15_ADR      0x3960  // 24 Byte 
//#define SYNCAKTU16_ADR      0x3978  // 24 Byte 

// R37_2
#define SYNCAKTU21_ADR        0x3A00  // 8 Byte Synchronisationswert aktuell
#define SYNCVORJ21_ADR        0x3A08  // 8 Byte Synchronisationswert Vorjahr
#define SYNCDONGLE21_ADR      0x3A10  // 2 Byte Dongle-Nummer des PC-Bedieners
#define SYNCTAG21_ADR         0x3A12  // 1 Byte Synchronisation Tag
#define SYNCMON21_ADR         0x3A13  // 1 Byte Synchronisation Monat
#define SYNCJAHR21_ADR        0x3A14  // 1 Byte Synchronisation Jahr (+3 Gap)
//#define SYNCAKTU22_ADR      0x3A18  // 24 Byte 
//#define SYNCAKTU23_ADR      0x3A30  // 24 Byte 
//#define SYNCAKTU24_ADR      0x3A48  // 24 Byte 
//#define SYNCAKTU25_ADR      0x3A60  // 24 Byte 
//#define SYNCAKTU26_ADR      0x3A78  // 24 Byte 

// MBus
#define SYNCAKTUMB1_ADR       0x3B00  // 8 Byte Synchronisationswert aktuell
#define SYNCVORJMB1_ADR       0x3B08  // 8 Byte Synchronisationswert Vorjahr
#define SYNCDONGLEMB1_ADR     0x3B10  // 2 Byte Dongle-Nummer des PC-Bedieners
#define SYNCTAGMB1_ADR        0x3B12  // 1 Byte Synchronisation Tag
#define SYNCMONMB1_ADR        0x3B13  // 1 Byte Synchronisation Monat
#define SYNCJAHRMB1_ADR       0x3B14  // 1 Byte Synchronisation Jahr (+3 Gap)
//#define SYNCAKTUMB2_ADR     0x3B18  // 24 Byte 
//#define SYNCAKTUMB3_ADR     0x3B30  // 24 Byte 
//#define SYNCAKTUMB4_ADR     0x3B48  // 24 Byte 
//#define SYNCAKTUMB5_ADR     0x3B60  // 24 Byte 
//#define SYNCAKTUMB6_ADR     0x3B78  // 24 Byte 
//#define SYNCAKTUMB7_ADR     0x3B90  // 24 Byte 
//#define SYNCAKTUMB8_ADR     0x3BA8  // 24 Byte 


//-----------------------------------------------------------------------------------------
//// Monatsspeicher für Zählerstände (nach Struktur wie im R50, Sicherung jede Stunde)
//#define ZQ_OFFSET       0x100   // Offset zur nächsten Zähleradresse
//#define ZQ0_ADR         0x4000  // 4*64 Byte Speicherdatum, 
//#define ZQ1_ADR         0x4100  // 4*64 Byte Werte Zähler1   
//#define ZQ2_ADR         0x4200  // 4*64 Byte Werte Zähler2   
//#define ZQ3_ADR         0x4300  // 4*64 Byte Werte Zähler3   
//#define ZQ4_ADR         0x4400  // 4*64 Byte Werte Zähler4   
//#define ZQ5_ADR         0x4500  // 4*64 Byte Werte Zähler5   
//#define ZQ6_ADR         0x4600  // 4*64 Byte Werte Zähler6   
//#define ZQ7_ADR         0x4700  // 4*64 Byte Werte Zähler7   
//#define ZQ8_ADR         0x4800  // 4*64 Byte Werte Zähler8   

// Änderung neue Genibus-Implementierung
#if GENI == 1
  #define GENI_DEV1_ADR   0x4600
  #define GENI_DEV2_ADR   0x4640
  #define GENI_DEV3_ADR   0x4680
  #define GENI_DEV4_ADR   0x46C0
  #define GENI_DEV5_ADR   0x4700
  #define GENI_DEV6_ADR   0x4740
  #define GENI_DEV7_ADR   0x4780  
  #define GENI_DEV8_ADR   0x47C0  
  #define GENI_OWN_ADR    0x4800        
#endif

#define BUS_PU_PARA_ADR       0x4900    // 8 Pages für max. 8 Pumpen
#define BUS_PU_PARA_LENG      64        // maximal 64 Byte pro Pumpe 


#define MITT_LEIST      0x4D00    // für ZE7 bis ZE10 je 20H Byte
#define SIZE_MITT_LEIST 0x20

#define GRENZ_LEIST     0x4D80

//-------------------------------------------------------------------------------------------------------------
// Leittechnik Primär- und Sekundär- Ereignistabellen (in KomTab.c)
// Initialisiert durch Init_Evetab(void)
// reserviert 1000H  = 64 Pages
#define PE_TAB_ADR      0x5000  // 15 Byte * 8 Bit = 120 Primärer-Ereignisse * 16 Byte im EEPROM = 1920 (780H)
#define EVETABKZ_ADR    0x5780  // 2 Byte
//                      0x5782  // Gap 126  
#define SE_TAB_ADR      0x5800  // 15 Byte * 8 Bit = 120 Sekundär-Ereignisse * 16 Byte im EEPROM = 1920 (780H)
//                      0x5F80  // 
   
// Alarmtabelle für den Regler , ermittelt aus der Sekundär- Ereignistabelle (in KomTab.c)
// Initialisiert durch Init_Alarmtab(void)
// reserviert 300H  = 12 Pages
#define AL_TAB_ADR      0x6000  // 15 Byte * 8 Bit = 120 Ereignisse * 4 Byte im EEPROM = 480  (1E0H)
//                      0x61E0  // Gap Reserve für 47 Ereignisse 
#define ALATABKZ_ADR    0x62FC  // 2 Byte
   
//-------------------------------------------------------------------------------------------------------------

/***** Archivspeicher mit Ferro-Ram *****/
#define ARCH_PAR_FERRO  0x6F80  // 2 Pages
#define ARCH_PAR_ADR    0x7000  // 1 Byte

/***** Josch-SDM : Archiv mit SD-Card-Memory *****/
#define SDM_ARCH_STOP_ADR 0x7002  // 1 Byte

/***** ulsch : Heizgradtage *****/
#define HEIZGRADTAGE_ADR  0x7040  // Heizgradtage fuer 24 Monate, 12 * 8 ( sizeof monHzGrd ) = 96 Byte = 1,5 Pages
//#define GRDTAGZAHL_ADR  0x7040  // Heizgradtage fuer 24 Monate, 12 * 8 ( sizeof monHzGrd ) = 96 Byte = 1,5 Pages
#define TA7_ADR           0x70A0  // Aussentemp. 7 Uhr 2 Byte
#define TA14_ADR          0x70A2  // Aussentemp. 14 Uhr 2 Byte
#define INDEX_MON_ADR     0x70A4  // 1 Byte
#define MERKER_STD_ADR    0x70A5  // 1 Byte

/***** ulsch : Diagnose *****/
#define DIAG_PARA         0x70C0    // max. 1 Page im EEPROM
//#####ulsch: HA-R-0114/HB-CH
#define DIAG2_PARA        0x7100    // max. 1 Page im EEPROM

/***** Waermezaehler *****/
#define SIZE_WMENG        0x80

// Zähler ZIN7
#define WMENG_ADR         0x7200    // 4 Byte im EEPROM
#define WMENG_MON_ADR     0x7204    // 48 Byte im EEPROM
#define FLOWMAX_ADR       0x7234    // 4 Byte im EEPROM
#define LEISTMAX_ADR      0x7238    // 2 Byte im EEPROM
#define MON_IN_VJ         0x723B    // 1 Byte im EEPROM
#define WMENG_VJMON_ADR   0x7240    // 48 Byte im EEPROM

// Zähler ZIN8
//-#define WMENG2_ADR       0x7280    // 4 Byte im EEPROM 
//-#define WMENG2_MON_ADR   0x7284    // 48 Byte im EEPROM
//-#define FLOWMAX2_ADR     0x72B4    // 4 Byte im EEPROM
//-#define LEISTMAX2_ADR    0x72B8    // 2 Byte im EEPROM
//-#define DAEMPFG_VOLSTR2  0x72BA    // 1 Byte im EEPROM
//-#define MON_IN_VJ2       0x72BB    // 1 Byte im EEPROM
//-#define WMENG2_VJMON_ADR 0x72C0    // 48 Byte im EEPROM
//-
// Zähler ZIN9
//-#define WMENG3_ADR       0x7300    // 4 Byte im EEPROM
//-#define WMENG3_MON_ADR   0x7304    // 48 Byte im EEPROM
//-#define FLOWMAX3_ADR     0x7334    // 4 Byte im EEPROM
//-#define LEISTMAX3_ADR    0x7338    // 2 Byte im EEPROM
//-#define DAEMPFG_VOLSTR38 0x733A    // 1 Byte im EEPROM
//-#define MON_IN_VJ3       0x733B    // 1 Byte im EEPROM
//-#define WMENG3_VJMON_ADR 0x7340    // 48 Byte im EEPROM
//-
// Zähler ZIN10
//-#define WMENG4_ADR       0x7380    // 4 Byte im EEPROM 
//-#define WMENG4_MON_ADR   0x7384    // 48 Byte im EEPROM
//-#define FLOWMAX4_ADR     0x73B4    // 4 Byte im EEPROM
//-#define LEISTMAX4_ADR    0x73B8    // 2 Byte im EEPROM
//-#define DAEMPFG_VOLSTR4  0x73BA    // 1 Byte im EEPROM
//-#define MON_IN_VJ4       0x73BB    // 1 Byte im EEPROM
//-#define WMENG4_VJMON_ADR 0x73C0    // 48 Byte im EEPROM
//-

// joja - modbus Geräteadressen
#if MODBUS_UNI > 0
  #include "Modbus/modbusstanddef.h"    // 0x7400 - 0x7483
#endif

#define K_DUMMY_ADR       0x7FFE  // 2 Byte   die letzten Bytes im EEPROM

//--------------------------------------------------------------------------------

// Adressen im Ferro-RAM
//#####ulsch: FW-R-0202/KA-WB
/***** ulsch : Diagnose *****/
#define DIAG2_ADR         0x7E80
#define FLOWMAXDIAG2_ADR  0x7E80    // 4 Byte im Ferro-RAM  ( sechstletzte Page )
#define LEISTMAXDIAG2_ADR 0x7E84    // 2 Byte im Ferro-RAM  ( sechstletzte Page )
#define FLOWDIAG2_ADR     0x7E86    // 4*2 Byte im Ferro-RAM
#define LEISTDIAG2_ADR    0x7E8E    // 4*2 Byte im Ferro-RAM
#define LADANZDIAG2_ADR   0x7E96    // 2 Byte im Ferro-RAM
#define LADZEITDIAG2_ADR  0x7E98    // 2 Byte im Ferro-RAM
#define WWVORANZDIAG2_ADR 0x7E9A    // 2 Byte im Ferro-RAM
#define WWVORZEITDIAG2_ADR 0x7E9C   // 2 Byte im Ferro-RAM
#define TVPMAXDIAG2_ADR   0x7E9E    // 8 Byte im Ferro-RAM ( sizeof extrTemp )
#define TVPMINDIAG2_ADR   0x7EA6    // 8 Byte im Ferro-RAM ( sizeof extrTemp )


/***** ulsch : Diagnose *****/
#define DIAG_ADR          0x7F40
#define FLOWMAXDIAG_ADR   0x7F40    // 4 Byte im Ferro-RAM  ( drittletzte Page )
#define LEISTMAXDIAG_ADR  0x7F44    // 2 Byte im Ferro-RAM  ( drittletzte Page )
#define FLOWDIAG_ADR      0x7F46    // 4*2 Byte im Ferro-RAM
#define LEISTDIAG_ADR     0x7F4E    // 4*2 Byte im Ferro-RAM
#define LADANZDIAG_ADR    0x7F56    // 2 Byte im Ferro-RAM
#define LADZEITDIAG_ADR   0x7F58    // 2 Byte im Ferro-RAM
#define WWVORANZDIAG_ADR  0x7F5A    // 2 Byte im Ferro-RAM
#define WWVORZEITDIAG_ADR 0x7F5C    // 2 Byte im Ferro-RAM
#define TVPMAXDIAG_ADR    0x7F5E    // 8 Byte im Ferro-RAM ( sizeof extrTemp )
#define TVPMINDIAG_ADR    0x7F66    // 8 Byte im Ferro-RAM ( sizeof extrTemp )



#endif  // STANDDEF_H_INCLUDED

