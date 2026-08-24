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
  { 0, CLOCK_MINUTE_QUANTITY_TEN, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(QUART) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, FR_WORD(IL) | FR_WORD(EST) | FR_WORD(DEMIE) | FR_WORD(ET), { FR_WORD(LE) | FR_WORD(HEURE), FR_WORD(HEURES), 0 } },
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

#define FR_WORD(word) CLOCK_WORD_SET_BIT(FR_WORD_##word)
static const ClockHourOverride s_french_hour_overrides[] = {
  { 0, 0, CLOCK_HOUR_FORM_SPECIAL, FR_WORD(MINUIT) },
  { 12, 12, CLOCK_HOUR_FORM_SPECIAL, 0 },
};
#undef FR_WORD

static const ClockLanguageProfile s_profiles[CLOCK_LANGUAGE_COUNT] = {
  {
    s_english_grid, s_english_numbers, s_english_minute_quantities,
    s_english_words, EN_WORD_COUNT, s_english_minute_rules,
    CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_italian_grid, s_italian_numbers, s_italian_minute_quantities,
    s_italian_words, IT_WORD_COUNT, s_italian_minute_rules,
    CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_french_grid, s_french_numbers, s_french_minute_quantities,
    s_french_words, FR_WORD_COUNT, s_french_minute_rules,
    CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, s_french_hour_overrides,
    sizeof(s_french_hour_overrides) / sizeof(s_french_hour_overrides[0]),
  },
  {
    s_spanish_grid, s_spanish_numbers, s_spanish_minute_quantities,
    s_spanish_words, ES_WORD_COUNT, s_spanish_minute_rules,
    CLOCK_HOUR_FORM_ONE,
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
  return CLOCK_LANGUAGE_EN;
}

const ClockLanguageProfile *clock_language_get_profile(ClockLanguage language) {
  if (language >= CLOCK_LANGUAGE_COUNT) {
    return &s_profiles[CLOCK_LANGUAGE_EN];
  }
  return &s_profiles[language];
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
