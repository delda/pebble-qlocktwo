#include "clock_language.h"

#include <string.h>

typedef enum {
  EN_WORD_IT,
  EN_WORD_IS,
  EN_WORD_A,
  EN_WORD_OCLOCK,
  EN_WORD_HALF,
  EN_WORD_QUARTER,
  EN_WORD_PAST,
  EN_WORD_TO,
  EN_WORD_COUNT,
} EnglishWord;

typedef enum {
  DE_WORD_ES,
  DE_WORD_IST,
  DE_WORD_UHR,
  DE_WORD_HALB,
  DE_WORD_VIERTEL,
  DE_WORD_NACH,
  DE_WORD_VOR,
  DE_WORD_COUNT,
} GermanWord;

typedef enum {
  IT_WORD_SONO,
  IT_WORD_LE,
  IT_WORD_AND,
  IT_WORD_SINGULAR_PREFIX,
  IT_WORD_ORE,
  IT_WORD_MEZZA,
  IT_WORD_QUARTO,
  IT_WORD_MENO,
  IT_WORD_UN,
  IT_WORD_COUNT,
} ItalianWord;

typedef enum {
  FR_WORD_IL,
  FR_WORD_EST,
  FR_WORD_LE,
  FR_WORD_HEURES,
  FR_WORD_DEMIE,
  FR_WORD_QUART,
  FR_WORD_ET,
  FR_WORD_ET_DEMIE,
  FR_WORD_MOINS,
  FR_WORD_HEURE,
  FR_WORD_MINUIT,
  FR_WORD_COUNT,
} FrenchWord;

typedef enum {
  ES_WORD_ES,
  ES_WORD_SON,
  ES_WORD_LA,
  ES_WORD_LAS,
  ES_WORD_MEDIA,
  ES_WORD_CUARTO,
  ES_WORD_Y,
  ES_WORD_MENOS,
  ES_WORD_COUNT,
} SpanishWord;

typedef enum {
  NL_WORD_HET,
  NL_WORD_IS,
  NL_WORD_UUR,
  NL_WORD_HALF,
  NL_WORD_KWART,
  NL_WORD_OVER,
  NL_WORD_VOOR,
  NL_WORD_COUNT,
} DutchWord;

typedef enum {
  NO_WORD_KLOKKEN,
  NO_WORD_ER,
  NO_WORD_KVART,
  NO_WORD_OVER,
  NO_WORD_HALV,
  NO_WORD_PA,
  NO_WORD_PA_KVART,
  NO_WORD_COUNT,
} NorwegianWord;

typedef enum {
  PT_WORD_E,
  PT_WORD_E_SINGULAR,
  PT_WORD_SAO,
  PT_WORD_HORA,
  PT_WORD_HORAS,
  PT_WORD_MEIA,
  PT_WORD_MENOS,
  PT_WORD_UM,
  PT_WORD_QUARTO,
  PT_WORD_MEIO_DIA,
  PT_WORD_NOITE,
  PT_WORD_CINCO,
  PT_WORD_COUNT,
} PortugueseWord;

typedef enum {
  SE_WORD_KLOCKAN,
  SE_WORD_AR,
  SE_WORD_KVART,
  SE_WORD_OVER,
  SE_WORD_I,
  SE_WORD_HALV,
  SE_WORD_COUNT,
} SwedishWord;

typedef enum {
  DK_WORD_KLOKKEN,
  DK_WORD_ER,
  DK_WORD_KVART,
  DK_WORD_MINUTTER,
  DK_WORD_OVER,
  DK_WORD_I,
  DK_WORD_HALV,
  DK_WORD_ET,
  DK_WORD_COUNT,
} DanishWord;

static const char *const s_english_grid[CLOCK_GRID_ROWS] = {
  "ITLISASTIME",
  "ACQUARTERDC",
  "TWENTYFIVEX",
  "HALFBTENFTO",
  "PASTERUNINE",
  "ONESIXTHREE",
  "FOURFIVETWO",
  "EIGHTELEVEN",
  "SEVENTWELVE",
  "TENSEOCLOCK",
};

static const ClockGridWord s_english_numbers[12] = {
  { 5, 0, 3 },  // ONE
  { 6, 8, 3 },  // TWO
  { 5, 6, 5 },  // THREE
  { 6, 0, 4 },  // FOUR
  { 6, 4, 4 },  // FIVE
  { 5, 3, 3 },  // SIX
  { 8, 0, 5 },  // SEVEN
  { 7, 0, 5 },  // EIGHT
  { 4, 7, 4 },  // NINE
  { 9, 0, 3 },  // TEN
  { 7, 5, 6 },  // ELEVEN
  { 8, 5, 6 },  // TWELVE
};

static const ClockGridWord
    s_english_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 2, 6, 4 },   // FIVE
  { 3, 5, 3 },   // TEN
  { 2, 0, 6 },   // TWENTY
  { 2, 0, 10 },  // TWENTYFIVE
};

static const ClockGridWord s_english_words[EN_WORD_COUNT] = {
  { 0, 0, 2 },  // IT
  { 0, 3, 2 },  // IS
  { 1, 0, 1 },  // A
  { 9, 5, 6 },  // OCLOCK
  { 3, 0, 4 },  // HALF
  { 1, 2, 7 },  // QUARTER
  { 4, 0, 4 },  // PAST
  { 3, 9, 2 },  // TO
};

// Matrix copied from the standard German QLOCKTWO front cover.
static const char *const s_german_grid[CLOCK_GRID_ROWS] = {
  "ESKISTAF\u00dcNF",
  "ZEHNZWANZIG",
  "DREIVIERTEL",
  "TGNACHVORJM",
  "HALBQZW\u00d6LF",
  "ZWEINSIEBEN",
  "KDREIRHF\u00dcNF",
  "ELFNEUNVIER",
  "WACHTZEHNRS",
  "BSECHSFMUHR",
};

static const ClockGridWord s_german_numbers[CLOCK_NUMBER_FORM_COUNT][12] = {
  [CLOCK_NUMBER_FORM_DEFAULT] = {
    { 5, 2, 4 },  // EINS
    { 5, 0, 4 },  // ZWEI
    { 6, 1, 4 },  // DREI
    { 7, 7, 4 },  // VIER
    { 6, 7, 4 },  // F\u00dcNF
    { 9, 1, 5 },  // SECHS
    { 5, 5, 6 },  // SIEBEN
    { 8, 1, 4 },  // ACHT
    { 7, 3, 4 },  // NEUN
    { 8, 5, 4 },  // ZEHN
    { 7, 0, 3 },  // ELF
    { 4, 5, 6 },  // ZW\u00d6LF
  },
  [CLOCK_NUMBER_FORM_OCLOCK] = {
    { 5, 2, 3 },  // EIN
    { 5, 0, 4 },  // ZWEI
    { 6, 1, 4 },  // DREI
    { 7, 7, 4 },  // VIER
    { 6, 7, 4 },  // F\u00dcNF
    { 9, 1, 5 },  // SECHS
    { 5, 5, 6 },  // SIEBEN
    { 8, 1, 4 },  // ACHT
    { 7, 3, 4 },  // NEUN
    { 8, 5, 4 },  // ZEHN
    { 7, 0, 3 },  // ELF
    { 4, 5, 6 },  // ZW\u00d6LF
  },
};

static const ClockGridWord
    s_german_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 0, 7, 4 },  // F\u00dcNF
  { 1, 0, 4 },  // ZEHN
  { 1, 4, 7 },  // ZWANZIG
  { 0, 0, 0 },  // Not used by German phrasing
};

static const ClockGridWord s_german_words[DE_WORD_COUNT] = {
  { 0, 0, 2 },  // ES
  { 0, 3, 3 },  // IST
  { 9, 8, 3 },  // UHR
  { 4, 0, 4 },  // HALB
  { 2, 4, 7 },  // VIERTEL
  { 3, 2, 4 },  // NACH
  { 3, 6, 3 },  // VOR
};

// Matrix copied from the Italian QLOCKTWO front cover.  The apostrophe in
// "l'una" is represented by the separate initial E ("e' l'una").
static const char *const s_italian_grid[CLOCK_GRID_ROWS] = {
  "SONORLEBORE",
  "ERLUNASDUEZ",
  "TREOTTONOVE",
  "DIECIUNDICI",
  "DODICISETTE",
  "QUATTROCSEI",
  "CINQUEAMENO",
  "ECUNOQUARTO",
  "VENTICINQUE",
  "DIECIPMEZZA",
};

static const ClockGridWord s_italian_numbers[12] = {
  { 1, 2, 4 },  // LUNA
  { 1, 7, 3 },  // DUE
  { 2, 0, 3 },  // TRE
  { 5, 0, 7 },  // QUATTRO
  { 8, 5, 6 },  // CINQUE
  { 5, 8, 3 },  // SEI
  { 4, 6, 5 },  // SETTE
  { 2, 3, 4 },  // OTTO
  { 2, 7, 4 },  // NOVE
  { 3, 0, 5 },  // DIECI
  { 3, 5, 6 },  // UNDICI
  { 4, 0, 6 },  // DODICI
};

static const ClockGridWord
    s_italian_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 6, 0, 6 },   // CINQUE
  { 9, 0, 5 },   // DIECI
  { 8, 0, 5 },   // VENTI
  { 8, 0, 11 },  // VENTICINQUE
};

static const ClockGridWord s_italian_words[IT_WORD_COUNT] = {
  { 0, 0, 4 },  // SONO
  { 0, 5, 2 },  // LE
  { 7, 0, 1 },  // E
  { 1, 0, 1 },  // E (for e' l'una)
  { 0, 8, 3 },  // ORE
  { 9, 6, 5 },  // MEZZA
  { 7, 5, 6 },  // QUARTO
  { 6, 7, 4 },  // MENO
  { 7, 2, 2 },  // UN
};

// Matrix copied from the French QLOCKTWO front cover. The hyphen separates
// "vingt-cinq", as on the original front cover.
static const char *const s_french_grid[CLOCK_GRID_ROWS] = {
  "ILNESTODEUX",
  "QUATRETROIS",
  "NEUFUNESEPT",
  "HUITSIXCINQ",
  "MIDIXMINUIT",
  "ONZERHEURES",
  "MOINSOLEDIX",
  "ETRQUARTPMD",
  "VINGT-CINQU",
  "ETSDEMIEPAM",
};

static const ClockGridWord s_french_numbers[12] = {
  { 2, 4, 3 },  // UNE
  { 0, 7, 4 },  // DEUX
  { 1, 6, 5 },  // TROIS
  { 1, 0, 6 },  // QUATRE
  { 3, 7, 4 },  // CINQ
  { 3, 4, 3 },  // SIX
  { 2, 7, 4 },  // SEPT
  { 3, 0, 4 },  // HUIT
  { 2, 0, 4 },  // NEUF
  { 4, 3, 3 },  // DIX
  { 5, 0, 4 },  // ONZE
  { 4, 0, 4 },  // MIDI
};

static const ClockGridWord
    s_french_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 8, 6, 4 },   // CINQ
  { 6, 8, 3 },   // DIX
  { 8, 0, 5 },   // VINGT
  { 8, 0, 10 },  // VINGT-CINQ
};

static const ClockGridWord s_french_words[FR_WORD_COUNT] = {
  { 0, 0, 2 },  // IL
  { 0, 3, 3 },  // EST
  { 6, 6, 2 },  // LE
  { 5, 5, 6 },  // HEURES
  { 9, 3, 5 },  // DEMIE
  { 7, 3, 5 },  // QUART
  { 7, 0, 2 },  // ET
  { 9, 0, 2 },  // ET (for ET DEMIE)
  { 6, 0, 5 },  // MOINS
  { 5, 5, 5 },  // HEURE
  { 4, 5, 6 },  // MINUIT
};

// Matrix copied from the Spanish QLOCKTWO front cover.
static const char *const s_spanish_grid[CLOCK_GRID_ROWS] = {
  "ESONELASUNA",
  "DOSITRESORE",
  "CUATROCINCO",
  "SEISASIETEN",
  "OCHONUEVEYO",
  "LADIEZSONCE",
  "DOCELYMENOS",
  "OVEINTEDIEZ",
  "VEINTICINCO",
  "MEDIACUARTO",
};

static const ClockGridWord s_spanish_numbers[12] = {
  { 0, 8, 3 },  // UNA
  { 1, 0, 3 },  // DOS
  { 1, 4, 4 },  // TRES
  { 2, 0, 6 },  // CUATRO
  { 2, 6, 5 },  // CINCO
  { 3, 0, 4 },  // SEIS
  { 3, 5, 5 },  // SIETE
  { 4, 0, 4 },  // OCHO
  { 4, 4, 5 },  // NUEVE
  { 5, 2, 4 },  // DIEZ
  { 5, 7, 4 },  // ONCE
  { 6, 0, 4 },  // DOCE
};

static const ClockGridWord
    s_spanish_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 2, 6, 5 },  // CINCO
  { 7, 7, 4 },  // DIEZ
  { 7, 1, 6 },  // VEINTE
  { 8, 0, 11 }, // VEINTICINCO
};

static const ClockGridWord s_spanish_words[ES_WORD_COUNT] = {
  { 0, 0, 2 },  // ES
  { 0, 1, 3 },  // SON
  { 0, 5, 2 },  // LA
  { 0, 5, 3 },  // LAS
  { 9, 0, 5 },  // MEDIA
  { 9, 5, 6 },  // CUARTO
  { 6, 5, 1 },  // Y
  { 6, 6, 5 },  // MENOS
};

// Matrix copied from the standard Dutch QLOCKTWO front cover.
static const char *const s_dutch_grid[CLOCK_GRID_ROWS] = {
  "HETKISAVIJF",
  "TIENATZVOOR",
  "OVERMEKWART",
  "HALFSPMOVER",
  "VOORTHGEENS",
  "TWEEAMCDRIE",
  "VIERVIJFZES",
  "ZEVENONEGEN",
  "ACHTTIENELF",
  "TWAALFPMUUR",
};

static const ClockGridWord s_dutch_numbers[12] = {
  { 4, 7, 3 },  // EEN
  { 5, 0, 4 },  // TWEE
  { 5, 7, 4 },  // DRIE
  { 6, 0, 4 },  // VIER
  { 6, 4, 4 },  // VIJF
  { 6, 8, 3 },  // ZES
  { 7, 0, 5 },  // ZEVEN
  { 8, 0, 4 },  // ACHT
  { 7, 6, 5 },  // NEGEN
  { 8, 4, 4 },  // TIEN
  { 8, 8, 3 },  // ELF
  { 9, 0, 6 },  // TWAALF
};

static const ClockGridWord
    s_dutch_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 0, 7, 4 },  // VIJF
  { 1, 0, 4 },  // TIEN
  { 0, 0, 0 },  // Not used by Dutch phrasing
  { 0, 0, 0 },  // Not used by Dutch phrasing
};

static const ClockGridWord s_dutch_words[NL_WORD_COUNT] = {
  { 0, 0, 3 },  // HET
  { 0, 4, 2 },  // IS
  { 9, 8, 3 },  // UUR
  { 3, 0, 4 },  // HALF
  { 2, 6, 5 },  // KWART
  { 3, 7, 4 },  // OVER
  { 1, 7, 4 },  // VOOR
};

// Matrix copied from the standard Portuguese QLOCKTWO front cover.
static const char *const s_portuguese_grid[CLOCK_GRID_ROWS] = {
  "\u00c9S\u00c3OUMATR\u00caS",
  "MEIODIADEZ",
  "DUASEISETEY",
  "QUATROHNOVE",
  "CINCOITONZE",
  "ZMEIALNOITE",
  "HORASYMENOS",
  "VINTECAMEIA",
  "UMVQUARTOPM",
  "DEZOEYCINCO",
};

static const ClockGridWord s_portuguese_numbers[12] = {
  { 0, 4, 3 },  // UMA
  { 2, 0, 4 },  // DUAS
  { 0, 7, 4 },  // TR\u00caS
  { 3, 0, 6 },  // QUATRO
  { 4, 0, 5 },  // CINCO
  { 2, 3, 4 },  // SEIS
  { 2, 7, 4 },  // SETE
  { 4, 4, 4 },  // OITO
  { 3, 7, 4 },  // NOVE
  { 9, 0, 3 },  // DEZ
  { 4, 7, 4 },  // ONZE
  { 0, 0, 0 },  // MEIO-DIA / MEIA-NOITE
};

static const ClockGridWord
    s_portuguese_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 9, 6, 5 },  // CINCO
  { 9, 0, 3 },  // DEZ
  { 7, 0, 5 },  // VINTE
  { 0, 0, 0 },  // Expressed as VINTE E CINCO
};

static const ClockGridWord s_portuguese_words[PT_WORD_COUNT] = {
  { 9, 4, 1 },  // E
  { 0, 0, 1 },  // \u00c9
  { 0, 1, 3 },  // S\u00c3O
  { 6, 0, 4 },  // HORA
  { 6, 0, 5 },  // HORAS
  { 5, 1, 4 },  // MEIA
  { 6, 6, 5 },  // MENOS
  { 8, 0, 2 },  // UM
  { 8, 3, 6 },  // QUARTO
  { 1, 0, 7 },  // MEIODIA
  { 5, 6, 5 },  // NOITE
  { 9, 6, 5 },  // CINCO
};

// Matrix copied from the standard Swedish QLOCKTWO front cover.
static const char *const s_swedish_grid[CLOCK_GRID_ROWS] = {
  "KLOCKANTÄRK",
  "FEMYISTIONI",
  "KVARTQIENZO",
  "TJUGOLIVIPM",
  "ÖVERKAMHALV",
  "ETTUSVLXTVÅ",
  "TREMYKYFYRA",
  "FEMSFLORSEX",
  "SJUÄTTAINIO",
  "TIOELVATOLV",
};

static const ClockGridWord s_swedish_numbers[12] = {
  { 5, 0, 3 },  // ETT
  { 5, 8, 3 },  // TVÅ
  { 6, 0, 3 },  // TRE
  { 6, 7, 4 },  // FYRA
  { 7, 0, 3 },  // FEM
  { 7, 8, 3 },  // SEX
  { 8, 0, 3 },  // SJU
  { 8, 3, 4 },  // ÅTTA
  { 8, 8, 3 },  // NIO
  { 9, 0, 3 },  // TIO
  { 9, 3, 4 },  // ELVA
  { 9, 7, 4 },  // TOLV
};

static const ClockGridWord
    s_swedish_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 1, 0, 3 },  // FEM
  { 1, 6, 3 },  // TIO
  { 3, 0, 5 },  // TJUGO
  { 0, 0, 0 },  // Not used by Swedish phrasing
};

static const ClockGridWord s_swedish_words[SE_WORD_COUNT] = {
  { 0, 0, 7 },  // KLOCKAN
  { 0, 8, 2 },  // ÄR
  { 2, 0, 5 },  // KVART
  { 4, 0, 4 },  // ÖVER
  { 1, 4, 1 },  // I
  { 4, 7, 4 },  // HALV
};

// Matrix copied from the standard Danish QLOCKTWO front cover.
static const char *const s_danish_grid[CLOCK_GRID_ROWS] = {
  "KLOKKENVERO",
  "FEMTYVESKAM",
  "OJEKVARTVAT",
  "TIAMINUTTER",
  "FEMOVERILPM",
  "MONALISHALV",
  "ETTOTREFIRE",
  "FEMSEKSRSYV",
  "OTTERNIMETI",
  "ELLEVEATOLV",
};

static const ClockGridWord s_danish_numbers[12] = {
  { 6, 0, 2 }, { 6, 2, 2 }, { 6, 4, 3 }, { 6, 7, 4 },
  { 7, 0, 3 }, { 7, 3, 4 }, { 7, 8, 3 }, { 8, 0, 4 },
  { 8, 5, 2 }, { 8, 9, 2 }, { 9, 0, 6 }, { 9, 7, 4 },
};

static const ClockGridWord
    s_danish_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 1, 0, 3 }, { 3, 0, 2 }, { 1, 3, 4 }, { 0, 0, 0 },
};

static const ClockGridWord s_danish_words[DK_WORD_COUNT] = {
  { 0, 0, 7 }, { 0, 8, 2 }, { 2, 3, 5 }, { 3, 3, 8 },
  { 4, 3, 4 }, { 4, 8, 1 }, { 5, 7, 4 }, { 6, 0, 2 },
};

// Matrix copied from the standard Norwegian QLOCKTWO front cover.
static const char *const s_norwegian_grid[CLOCK_GRID_ROWS] = {
  "KLOKKENVERM",
  "FEMHP\u00c5SJUIS",
  "TILP\u00c5SIDOSN",
  "KVARTNP\u00c5STO",
  "OVERXAMBPMZ",
  "HALVBIEGENZ",
  "ETTNTOATREX",
  "FIREFEMSEKS",
  "SYV\u00c5TTENITI",
  "ELLEVESTOLV",
};

static const ClockGridWord s_norwegian_numbers[12] = {
  { 6, 0, 3 },  // ETT
  { 6, 4, 2 },  // TO
  { 6, 7, 3 },  // TRE
  { 7, 0, 4 },  // FIRE
  { 7, 4, 3 },  // FEM
  { 7, 7, 4 },  // SEKS
  { 8, 0, 3 },  // SYV
  { 8, 3, 4 },  // \u00c5TTE
  { 8, 9, 2 },  // NI
  { 2, 0, 2 },  // TI
  { 9, 0, 6 },  // ELLEVE
  { 9, 7, 4 },  // TOLV
};

static const ClockGridWord
    s_norwegian_minute_quantities[CLOCK_MINUTE_QUANTITY_COUNT - 1] = {
  { 1, 0, 3 },  // FEM
  { 2, 0, 2 },  // TI
  { 0, 0, 0 },  // Not used by Norwegian phrasing
  { 0, 0, 0 },  // Not used by Norwegian phrasing
};

static const ClockGridWord s_norwegian_words[NO_WORD_COUNT] = {
  { 0, 0, 7 },  // KLOKKEN
  { 0, 8, 2 },  // ER
  { 3, 0, 5 },  // KVART
  { 4, 0, 4 },  // OVER
  { 5, 0, 4 },  // HALV
  { 1, 4, 2 },  // P\u00c5
  { 3, 6, 2 },  // P\u00c5 (for KVART P\u00c5)
};

// Each row represents 00, 05, ..., 55.  The data, rather than the renderer,
// describes the order of the words and which hour is being named.
#define EN_WORD(word) CLOCK_WORD_SET_BIT(EN_WORD_##word)
static const ClockMinuteRule s_english_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(OCLOCK), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(A) | EN_WORD(QUARTER) | EN_WORD(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(HALF) | EN_WORD(PAST), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(A) | EN_WORD(QUARTER) | EN_WORD(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, EN_WORD(IT) | EN_WORD(IS) | EN_WORD(TO), { 0, 0, 0 } },
};
#undef EN_WORD

#define DE_WORD(word) CLOCK_WORD_SET_BIT(DE_WORD_##word)
static const ClockMinuteRule s_german_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(UHR), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(NACH), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(NACH), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(VIERTEL) | DE_WORD(NACH),
    { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(NACH), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(VOR) | DE_WORD(HALB),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(HALB), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(NACH) | DE_WORD(HALB),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(VOR), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(VIERTEL) | DE_WORD(VOR),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(VOR), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    DE_WORD(ES) | DE_WORD(IST) | DE_WORD(VOR), { 0, 0, 0 } },
};
#undef DE_WORD

#define IT_WORD(word) CLOCK_WORD_SET_BIT(IT_WORD_##word)
static const ClockMinuteRule s_italian_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, 0, { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE) | IT_WORD(ORE), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, IT_WORD(AND), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, IT_WORD(AND), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, IT_WORD(UN) | IT_WORD(QUARTO) | IT_WORD(AND), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, IT_WORD(AND), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, IT_WORD(AND), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, IT_WORD(MEZZA) | IT_WORD(AND), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, IT_WORD(MENO), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, IT_WORD(MENO), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, IT_WORD(UN) | IT_WORD(QUARTO) | IT_WORD(MENO), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, IT_WORD(MENO), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, IT_WORD(MENO), { IT_WORD(SINGULAR_PREFIX), IT_WORD(SONO) | IT_WORD(LE), 0 } },
};
#undef IT_WORD

#define FR_WORD(word) CLOCK_WORD_SET_BIT(FR_WORD_##word)
static const ClockMinuteRule s_french_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, FR_WORD(IL) | FR_WORD(EST), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, FR_WORD(IL) | FR_WORD(EST), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(QUART) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(DEMIE) | FR_WORD(ET_DEMIE), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(MOINS), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(MOINS), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(QUART) | FR_WORD(MOINS) | FR_WORD(LE), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(MOINS), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(MOINS), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
};
#undef FR_WORD

#define ES_WORD(word) CLOCK_WORD_SET_BIT(ES_WORD_##word)
static const ClockMinuteRule s_spanish_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, 0, { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, ES_WORD(Y), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, ES_WORD(Y), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, ES_WORD(CUARTO) | ES_WORD(Y), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, ES_WORD(Y), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, ES_WORD(Y), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, ES_WORD(MEDIA) | ES_WORD(Y), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, ES_WORD(MENOS), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, ES_WORD(MENOS), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, ES_WORD(CUARTO) | ES_WORD(MENOS), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, ES_WORD(MENOS), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, ES_WORD(MENOS), { ES_WORD(ES) | ES_WORD(LA), ES_WORD(SON) | ES_WORD(LAS), 0 } },
};
#undef ES_WORD

#define NL_WORD(word) CLOCK_WORD_SET_BIT(NL_WORD_##word)
static const ClockMinuteRule s_dutch_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(UUR), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(KWART) | NL_WORD(OVER),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(VOOR) | NL_WORD(HALF),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(VOOR) | NL_WORD(HALF),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(HALF), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(OVER) | NL_WORD(HALF),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(OVER) | NL_WORD(HALF),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(KWART) | NL_WORD(VOOR),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(VOOR), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    NL_WORD(HET) | NL_WORD(IS) | NL_WORD(VOOR), { 0, 0, 0 } },
};
#undef NL_WORD

#define PT_WORD(word) CLOCK_WORD_SET_BIT(PT_WORD_##word)
static const ClockMinuteRule
    s_portuguese_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, 0,
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, PT_WORD(E),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, PT_WORD(E),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, PT_WORD(E) | PT_WORD(UM) | PT_WORD(QUARTO),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, PT_WORD(E),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, PT_WORD(E) | PT_WORD(CINCO),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, PT_WORD(E) | PT_WORD(MEIA),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, PT_WORD(MENOS) | PT_WORD(CINCO),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, PT_WORD(MENOS),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, PT_WORD(MENOS) | PT_WORD(UM) | PT_WORD(QUARTO),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, PT_WORD(MENOS),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, PT_WORD(MENOS),
    { PT_WORD(E_SINGULAR) | PT_WORD(HORA), PT_WORD(SAO) | PT_WORD(HORAS), 0 } },
};

static const ClockHourOverride s_portuguese_hour_overrides[] = {
  { 0, 0, CLOCK_HOUR_FORM_SPECIAL,
    PT_WORD(E_SINGULAR) | PT_WORD(MEIA) | PT_WORD(NOITE) },
  { 12, 0, CLOCK_HOUR_FORM_SPECIAL, PT_WORD(E_SINGULAR) | PT_WORD(MEIO_DIA) },
};
#undef PT_WORD

#define SE_WORD(word) CLOCK_WORD_SET_BIT(SE_WORD_##word)
static const ClockMinuteRule s_swedish_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, SE_WORD(KLOCKAN) | SE_WORD(AR), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(KVART) | SE_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(OVER), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(I) | SE_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(OVER) | SE_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(I), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(KVART) | SE_WORD(I), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(I), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, SE_WORD(KLOCKAN) | SE_WORD(AR) | SE_WORD(I), { 0, 0, 0 } },
};
#undef SE_WORD

#define DK_WORD(word) CLOCK_WORD_SET_BIT(DK_WORD_##word)
static const ClockMinuteRule s_danish_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE,
    DK_WORD(KLOKKEN) | DK_WORD(ER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(OVER),
    { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(OVER),
    { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(ET) | DK_WORD(KVART) |
        DK_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(OVER),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(I) |
        DK_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(OVER) |
        DK_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(I),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(ET) | DK_WORD(KVART) |
        DK_WORD(I), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(I),
    { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE,
    DK_WORD(KLOKKEN) | DK_WORD(ER) | DK_WORD(MINUTTER) | DK_WORD(I),
    { 0, 0, 0 } },
};
#undef DK_WORD

#define NO_WORD(word) CLOCK_WORD_SET_BIT(NO_WORD_##word)
static const ClockMinuteRule
    s_norwegian_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, NO_WORD(KLOKKEN) | NO_WORD(ER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(OVER), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(KVART) | NO_WORD(OVER), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(PA) | NO_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(PA) | NO_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(OVER) | NO_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(OVER) | NO_WORD(HALV), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(KVART) | NO_WORD(PA_KVART), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(PA), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, NO_WORD(KLOKKEN) | NO_WORD(ER) | NO_WORD(PA), { 0, 0, 0 } },
};
#undef NO_WORD

#define FR_WORD(word) CLOCK_WORD_SET_BIT(FR_WORD_##word)
static const ClockHourOverride s_french_hour_overrides[] = {
  { 0, 0, CLOCK_HOUR_FORM_SPECIAL, FR_WORD(MINUIT) },
  { 12, 12, CLOCK_HOUR_FORM_SPECIAL, 0 },
};
#undef FR_WORD

static const ClockLanguageProfile s_profiles[CLOCK_LANGUAGE_COUNT] = {
  {
    s_english_grid, { s_english_numbers, s_english_numbers },
    s_english_minute_quantities,
    s_english_words, EN_WORD_COUNT, 0, s_english_minute_rules,
    CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_italian_grid, { s_italian_numbers, s_italian_numbers },
    s_italian_minute_quantities,
    s_italian_words, IT_WORD_COUNT, 0, s_italian_minute_rules,
    CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_french_grid, { s_french_numbers, s_french_numbers },
    s_french_minute_quantities,
    s_french_words, FR_WORD_COUNT, 0, s_french_minute_rules,
    CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, s_french_hour_overrides,
    sizeof(s_french_hour_overrides) / sizeof(s_french_hour_overrides[0]),
  },
  {
    s_spanish_grid, { s_spanish_numbers, s_spanish_numbers },
    s_spanish_minute_quantities,
    s_spanish_words, ES_WORD_COUNT, 0, s_spanish_minute_rules,
    CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_german_grid,
    {
      s_german_numbers[CLOCK_NUMBER_FORM_DEFAULT],
      s_german_numbers[CLOCK_NUMBER_FORM_OCLOCK],
    },
    s_german_minute_quantities, s_german_words, DE_WORD_COUNT,
    CLOCK_WORD_SET_BIT(DE_WORD_UHR),
    s_german_minute_rules, CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_dutch_grid, { s_dutch_numbers, s_dutch_numbers },
    s_dutch_minute_quantities,
    s_dutch_words, NL_WORD_COUNT, 0, s_dutch_minute_rules,
    CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_portuguese_grid, { s_portuguese_numbers, s_portuguese_numbers },
    s_portuguese_minute_quantities,
    s_portuguese_words, PT_WORD_COUNT, 0, s_portuguese_minute_rules,
    CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, s_portuguese_hour_overrides,
    sizeof(s_portuguese_hour_overrides) / sizeof(s_portuguese_hour_overrides[0]),
  },
  {
    s_swedish_grid, { s_swedish_numbers, s_swedish_numbers },
    s_swedish_minute_quantities,
    s_swedish_words, SE_WORD_COUNT, 0, s_swedish_minute_rules,
    CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_danish_grid, { s_danish_numbers, s_danish_numbers },
    s_danish_minute_quantities,
    s_danish_words, DK_WORD_COUNT, 0, s_danish_minute_rules,
    CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_norwegian_grid, { s_norwegian_numbers, s_norwegian_numbers },
    s_norwegian_minute_quantities,
    s_norwegian_words, NO_WORD_COUNT, 0, s_norwegian_minute_rules,
    CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
};

ClockLanguage clock_language_from_string(const char *value) {
  if (value && strcmp(value, "it") == 0) {
    return CLOCK_LANGUAGE_IT;
  }
  if (value && strcmp(value, "fr") == 0) {
    return CLOCK_LANGUAGE_FR;
  }
  if (value && strcmp(value, "es") == 0) {
    return CLOCK_LANGUAGE_ES;
  }
  if (value && strcmp(value, "de") == 0) {
    return CLOCK_LANGUAGE_DE;
  }
  if (value && strcmp(value, "nl") == 0) {
    return CLOCK_LANGUAGE_NL;
  }
  if (value && strcmp(value, "pt") == 0) {
    return CLOCK_LANGUAGE_PT;
  }
  if (value && strcmp(value, "se") == 0) {
    return CLOCK_LANGUAGE_SE;
  }
  if (value && strcmp(value, "dk") == 0) {
    return CLOCK_LANGUAGE_DK;
  }
  if (value && strcmp(value, "no") == 0) {
    return CLOCK_LANGUAGE_NO;
  }
  return CLOCK_LANGUAGE_EN;
}

const ClockLanguageProfile *clock_language_get_profile(ClockLanguage language) {
  if (language >= CLOCK_LANGUAGE_COUNT) {
    return &s_profiles[CLOCK_LANGUAGE_EN];
  }
  return &s_profiles[language];
}

bool clock_language_get_grid_letter(const ClockLanguageProfile *profile,
                                    uint8_t row, uint8_t column,
                                    char letter[5]) {
  if (!profile || !letter || row >= CLOCK_GRID_ROWS ||
      column >= CLOCK_GRID_COLUMNS) {
    return false;
  }

  const char *character = profile->grid[row];
  for (uint8_t index = 0; index < column; ++index) {
    const uint8_t first_byte = (uint8_t)*character;
    const uint8_t byte_count = first_byte < 0x80 ? 1
                               : first_byte < 0xE0 ? 2
                               : first_byte < 0xF0 ? 3
                                                   : 4;
    character += byte_count;
  }

  const uint8_t first_byte = (uint8_t)*character;
  const uint8_t byte_count = first_byte < 0x80 ? 1
                             : first_byte < 0xE0 ? 2
                             : first_byte < 0xF0 ? 3
                                                 : 4;
  memcpy(letter, character, byte_count);
  letter[byte_count] = '\0';
  return true;
}

bool clock_language_get_hour_phrase(const ClockLanguageProfile *profile,
                                    uint8_t hour, uint8_t *number,
                                    ClockHourForm *form,
                                    ClockWordSet *words) {
  if (!profile || !number || !form || !words) {
    return false;
  }

  const uint8_t normalized_hour = hour % 24;
  *number = normalized_hour % 12;
  if (*number == 0) {
    *number = 12;
  }
  *form = *number == 1 ? profile->form_for_one : profile->form_for_other;
  *words = 0;

  for (uint8_t index = 0; index < profile->hour_override_count; ++index) {
    const ClockHourOverride *override = &profile->hour_overrides[index];
    if (override->hour == normalized_hour) {
      *number = override->number;
      *form = override->form;
      *words = override->words;
      break;
    }
  }
  return true;
}
