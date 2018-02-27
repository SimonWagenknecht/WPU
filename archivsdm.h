#ifndef ARCHIVARC_H_INCLUDED
#define ARCHIVARC_H_INCLUDED

// SD-Card-Memory 2GB SD-Karte: Speicherbereiche für Direktzugriffe (ohne FAT )
#define SDM_SIZE        2000000000        // 2.000.000.000 H  2 GB
// SDHC wird unterstützt. Speicheradressen werden automatisch auf Sektornummern umgerechnet. Speicherbereich bleibt unverändert.

#define SDM_MAXEXP_ADR  0x000FF900        // Adresse für Speicherung von Expandinformationen DAE 0 - DAE 511
#define SDM_KOMTA1_ADR  0x000FFA00        // Adresse für Komtabspeicherung DAE   0 - DAE 255
#define SDM_KOMTA2_ADR  0x000FFC00        // Adresse für Komtabspeicherung DAE 256 - DAE 511
#define SDM_INITKZ_ADR  0x000FFE00        // Adresse für die Speicherung eines Initialisierungskennzeichens (Sektor 0x7FF bei SDHC-Karten)

// Bereich für Komtab ohne Expandparameter
#define SDM_START_ADR   0x00100000        // Startadresse auf der SD-Karte (DAE 0), Start-Sektor 0x800 bei SDHC-Karten (1 Sektor = 512 Bytes)
#define SDM_BLOCK_DAE   0x00100000        // Bereich für ein DAE,                   

// Aufteilung eines Bereiches: Hauptpage, Verweispages, Datenpages
// Summe: 2048 Pages * 512 Byte = 100000H = SDM_BLOCK_DAE
#define SDM_HP_ANZ      1                 // Anzahl der Hauptpage
#define SDM_VP_ANZ      16                // Anzahl der Verweispages
#define SDM_DP_ANZ      2031              // Anzahl der Datenpages

#define SDM_OFFS_VP     0x0200            // Offset zu den Verweispages (= 1 Hauptpage)
#define SDM_OFFS_DP     0x2200            // Offset zu den Datenpages   (= 1 Hauptpage + 16 Verweispages)

// Bereich für Expandparameter
#define SDM_EXPAND_ADR  0x20100000;       // 20.100.000 H Die Speicherplätze werden dynamisch zugewiesen
#define SDM_EXPAND_SIZE 0x20000000;

// Status der Ablaufsteuerung
// asdm_status 
#define START              0
#define SD_INIT            1
#define CHECK_DAE          2
#define CHECK_VP           3
#define SAVE_DAE           4
#define FOLGE_SAVE_DAE     5
#define ACTION_EXP_DAE     6
#define ERROR              7
#define ENDE               8
// asdm_action zu ACTION_EXP_DAE
#define GET_EXP_TAB        0
#define CHECK_EXP_DAE      1
#define CHECK_EXP_VP       2
#define SAVE_EXP_DAE       3
#define FOLGE_SAVE_EXP_DAE 4

// Testparameter
#define SPEED_SAVE        0     // 1 = Hochgeschwindigkeit-Datenaufzeichnung mit einer DAE zum Testen des Ringspeichers
#define SPEED_DAE         31    // DAE-Nummer
#define ZEIT_MESSUNG      0     // 1 = Funktion Zeitmessung aktiviert   Josch-SDM

/* return values ArchivSDM */
typedef enum {
  ARC_OK                = 0,
  ARC_ERR_CARD          = 1,
  ARC_ERR_CARD_HEAP     = 2,
  ARC_ERR_SDM_INITKZ    = 3,
  ARC_ERR_FERRO_HEAP    = 4,
  ARC_ERR_FERRO_INIT    = 5,
  ARC_ERR_FERRO_INITKZ  = 6,
  ARC_ERR_FERRO_READ    = 7,
  ARC_ERR_FERRO_WRITE   = 8,
  ARC_ERR_LZKONV        = 9,
  ARC_ERR_KOMTAB        = 10,
  ARC_ERR_MAX_EXP       = 11,
  ARC_RUN               = 12,
  ARC_PAUSE             = 13,
  ARC_STOP              = 14,
  ARC_ERR_MONITOR       = 15,
} ARC_ERG;
// Debugger-Werte von enum-Variablen können symbolisch dargestellt werden.

/* memory card error codes */
typedef enum {
  CRD_OK              = 0x00,
  CRD_ERR_CMD0        = 0x01,
  CRD_ERR_OCR1        = 0x02,
  CRD_ERR_CMD8        = 0x03,
  CRD_ERR_CMD55       = 0x04,
  CRD_ERR_ACMD41      = 0x05,
  CRD_ERR_CMD40       = 0x06,
  CRD_ERR_CMD58       = 0x07,
  CRD_ERR_CMD51       = 0x08,
  CRD_ERR_CMD10       = 0x09,
  CRD_ERR_TO_CMD10    = 0x0a,
  CRD_ERR_CID_CRC     = 0x0b,
  CRD_ERR_SCR_CRC     = 0x0c,
  CRD_ERR_TO_CMD9     = 0x0d,
  CRD_ERR_CCRD_CRC    = 0x0e,
  CRD_ERR_CMD19       = 0x0f,
  CRD_ERR_SBLOCK_CRC  = 0x10,
  CRD_ERR_R1B         = 0x11,
  CRD_ERR_WRITE       = 0x12,
  CRD_ERR_BUSY        = 0x13,
  CRD_ERR_CMD24       = 0x14,
  CRD_ERR_UNKOWN_CARD = 0x15,
  CRD_ERR_VALIDATION  = 0x16,
  CRD_ERR_TIMEOUT     = 0x17,
  CRD_ERR_CMD13       = 0x18,
  CRD_ERR_CMD32       = 0x19,
  CRD_ERR_CMD33       = 0x1a,
  CRD_ERR_CMD38       = 0x1b,
  CRD_ERR_CCRD_VER    = 0x1c,
  CRD_ERR_CMD59       = 0x1d,
  CRD_ERR_CLOCK       = 0x1e,
  CRD_ERR_CMD17       = 0x1f,
  CRD_ERR_SIZE        = 0x20,
  CRD_ERR_INIT        = 0x21,
  CRD_ERR_ERASE       = 0x22,
  CRD_ERR_CMD42       = 0x23,
  CRD_ERR_LOCK_PAR    = 0x24,
  CRD_ERR_CMD16       = 0x25,
  CRD_ERR_STATUS      = 0x26,
  CRD_ERR_LOCKED      = 0x27,
//josch
  CRD_ERR_OTHER       = 0x30,
  CRD_RUN             = 0x31,
  CRD_PAUSE           = 0x32,
  CRD_STOP            = 0x33,

} CRD_ERG;
// Debugger-Werte von enum-Variablen können symbolisch dargestellt werden.
  

/* memory-card commands */
#define CMD0_RESETCARD              0
#define CMD1_OP_COND                1
#define CMD8_IF_COND                8
#define CMD9_SEND_CSD               9
#define CMD10_SEND_CID              10
#define CMD12_STOP_TRANSMISSION     12
#define CMD13_GET_STATUS            13
#define CMD16_SET_BLOCKLEN          16
#define CMD17_READ_SINGLE_BLOCK     17
#define CMD18_READ_MULTIPLE_BLOCK   18
#define CMD24_WRITE_BLOCK           24
#define CMD25_WRITE_MULTIPLE_BLOCK  25
#define CMD32_ERASE_START_BLOCK     32
#define CMD33_ERASE_END_BLOCK       33
#define CMD35_ERASE_GRP_START       35
#define CMD36_ERASE_GRP_END         36
#define CMD38_ERASE                 38
#define CMD42_LOCKCRD               42
#define CMD51_SEND_SCR              51
#define CMD55_APP                   55
#define CMD58_GET_OCR               58
#define CMD59_CRC_ONOFF             59
#define ACMD13_SEND_STATUS          13
#define ACMD41_OP_COND              41

/* Response definitions */

/* R1 response */
#define R1_IN_IDLE_STATE  0x01
#define R1_ERASE_RESET    0x02
#define R1_ILLEGAL_CMD    0x04
#define R1_COMCRC_ERR     0x08
#define R1_ERASE_SEQ_ERR  0x10
#define R1_ADDRESS_ERR    0x20
#define R1_PARAM_ERR      0x40

/* card types */
#define CARD_UNKOWN    0x00
#define MMC_CARD       0x01
#define CRD_STD_CARD   0x02
#define CRD_HC_CARD    0x03

/* masks */
#define STARTBIT_MASK 0x80


// Archiv SD-Card-Memory Kommandos
#define READ_SDMEM        0x60    // Test: Lese Daten von einem Speicherplatz
#define READ_ARCHIV       0x61    // Lese archivierte Daten in Form von Datenpaketen (Zeitstempel + Daten)
#define READ_MEMINFO      0x62    // Test: Lese Informationen zur Speicherbelegung aus dem Ferro-Ram    
#define READ_ARCHIV_INFO  0x63    // Test: Lese SD-Speicher-Informationen zu einem gefundenen DAE
#define CHECK_FERRO_SDHC  0x64    // Kommando für Prüfplatz: Ferro2 gesteckt ?, SDHC gesteckt ?
#define WRITE_SDMEM       0x65    // Test: Schreibe Daten auf einen Speicherplatz

#endif // ARCHIVARC_H_INCLUDED
