#include "clock_language.h"

#include <string.h>

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

static const ClockGridWord s_english_words[CLOCK_WORD_COUNT] = {
  { 0, 0, 2 },  // IT
  { 0, 3, 2 },  // IS
  { 1, 0, 1 },  // A
  { 9, 5, 6 },  // OCLOCK
  { 3, 0, 4 },  // HALF
  { 1, 2, 7 },  // QUARTER
  { 4, 0, 4 },  // PAST
  { 3, 9, 2 },  // TO
  { 1, 0, 1 },  // A
  { 0, 0, 2 },  // IT
  { 0, 0, 0 },  // Not used
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

static const ClockGridWord s_italian_words[CLOCK_WORD_COUNT] = {
  { 0, 0, 4 },  // SONO
  { 0, 5, 2 },  // LE
  { 7, 0, 1 },  // E
  { 0, 8, 3 },  // ORE
  { 9, 6, 5 },  // MEZZA
  { 7, 5, 6 },  // QUARTO
  { 7, 0, 1 },  // E
  { 6, 7, 4 },  // MENO
  { 7, 2, 2 },  // UN
  { 1, 0, 1 },  // E (for e' l'una)
  { 0, 0, 0 },  // Not used
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

static const ClockGridWord s_french_words[CLOCK_WORD_COUNT] = {
  { 0, 0, 2 },  // IL
  { 0, 3, 3 },  // EST
  { 6, 6, 2 },  // LE
  { 5, 5, 6 },  // HEURES
  { 9, 3, 5 },  // DEMIE
  { 7, 3, 5 },  // QUART
  { 7, 0, 2 },  // ET
  { 6, 0, 5 },  // MOINS
  { 6, 6, 2 },  // LE
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

static const ClockGridWord s_spanish_words[CLOCK_WORD_COUNT] = {
  { 0, 0, 2 },  // ES
  { 0, 1, 3 },  // SON
  { 0, 5, 2 },  // LA
  { 0, 5, 3 },  // LAS
  { 9, 0, 5 },  // MEDIA
  { 9, 5, 6 },  // CUARTO
  { 6, 5, 1 },  // Y
  { 6, 6, 5 },  // MENOS
  { 0, 5, 2 },  // LA
  { 0, 5, 2 },  // LA
  { 0, 0, 0 },  // Not used
};

#define W(word) CLOCK_WORD_BIT(CLOCK_WORD_##word)

// Each row represents 00, 05, ..., 55.  The data, rather than the renderer,
// describes the order of the words and which hour is being named.
static const ClockMinuteRule s_english_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(OCLOCK), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, W(IT) | W(IS) | W(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, W(IT) | W(IS) | W(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(A) | W(QUARTER) | W(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, W(IT) | W(IS) | W(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(IT) | W(IS) | W(PAST), { 0, 0, 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(HALF) | W(PAST), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(IT) | W(IS) | W(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, W(IT) | W(IS) | W(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(A) | W(QUARTER) | W(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, W(IT) | W(IS) | W(TO), { 0, 0, 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, W(IT) | W(IS) | W(TO), { 0, 0, 0 } },
};

static const ClockMinuteRule s_italian_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, 0, { W(SINGULAR_PREFIX), W(IT) | W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, W(PAST), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, W(PAST), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(QUARTER_ARTICLE) | W(QUARTER) | W(PAST), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, W(PAST), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(PAST), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(HALF) | W(PAST), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(TO), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, W(TO), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, W(QUARTER_ARTICLE) | W(QUARTER) | W(TO), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, W(TO), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, W(TO), { W(SINGULAR_PREFIX), W(IT) | W(IS), 0 } },
};

static const ClockMinuteRule s_french_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, W(IT) | W(IS) | W(PAST), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, W(IT) | W(IS) | W(PAST), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(QUARTER) | W(PAST), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, W(IT) | W(IS) | W(PAST), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(IT) | W(IS) | W(PAST), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(HALF) | W(PAST), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(IT) | W(IS) | W(TO), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, W(IT) | W(IS) | W(TO), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, W(IT) | W(IS) | W(QUARTER) | W(TO) | W(A), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, W(IT) | W(IS) | W(TO), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, W(IT) | W(IS) | W(TO), { W(A) | W(SINGULAR_PREFIX), W(OCLOCK), 0 } },
};

static const ClockMinuteRule s_spanish_minute_rules[CLOCK_MINUTE_RULE_COUNT] = {
  { 0, CLOCK_MINUTE_QUANTITY_NONE, 0, { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_FIVE, W(PAST), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TEN, W(PAST), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(QUARTER) | W(PAST), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY, W(PAST), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(PAST), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 0, CLOCK_MINUTE_QUANTITY_NONE, W(HALF) | W(PAST), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY_FIVE, W(TO), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TWENTY, W(TO), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_NONE, W(QUARTER) | W(TO), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_TEN, W(TO), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
  { 1, CLOCK_MINUTE_QUANTITY_FIVE, W(TO), { W(IT) | W(A), W(IS) | W(OCLOCK), 0 } },
};

static const ClockHourOverride s_french_hour_overrides[] = {
  { 0, 0, CLOCK_HOUR_FORM_SPECIAL, W(HOUR_SPECIAL) },
  { 12, 12, CLOCK_HOUR_FORM_SPECIAL, 0 },
};

static const ClockLanguageProfile s_profiles[CLOCK_LANGUAGE_COUNT] = {
  {
    s_english_grid, s_english_numbers, s_english_minute_quantities,
    s_english_words, s_english_minute_rules, CLOCK_HOUR_FORM_OTHER,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_italian_grid, s_italian_numbers, s_italian_minute_quantities,
    s_italian_words, s_italian_minute_rules, CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, NULL, 0,
  },
  {
    s_french_grid, s_french_numbers, s_french_minute_quantities,
    s_french_words, s_french_minute_rules, CLOCK_HOUR_FORM_ONE,
    CLOCK_HOUR_FORM_OTHER, s_french_hour_overrides,
    sizeof(s_french_hour_overrides) / sizeof(s_french_hour_overrides[0]),
  },
  {
    s_spanish_grid, s_spanish_numbers, s_spanish_minute_quantities,
    s_spanish_words, s_spanish_minute_rules, CLOCK_HOUR_FORM_ONE,
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
