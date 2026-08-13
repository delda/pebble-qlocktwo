#include "clock_language.h"

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

static const ClockGridWord s_english_minute_quantities[] = {
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
};

const char *const *clock_language_get_grid(ClockLanguage language) {
  switch (language) {
    case CLOCK_LANGUAGE_EN:
    default:
      return s_english_grid;
  }
}

bool clock_language_get_number(ClockLanguage language, uint8_t number,
                               ClockGridWord *word) {
  if (number < 1 || number > 12 || !word) {
    return false;
  }

  switch (language) {
    case CLOCK_LANGUAGE_EN:
      *word = s_english_numbers[number - 1];
      return true;
    default:
      return false;
  }
}

bool clock_language_get_minute_quantity(ClockLanguage language, uint8_t minutes,
                                        ClockGridWord *word) {
  if (!word) {
    return false;
  }

  switch (language) {
    case CLOCK_LANGUAGE_EN:
      switch (minutes) {
        case 5:
          *word = s_english_minute_quantities[0];
          return true;
        case 10:
          *word = s_english_minute_quantities[1];
          return true;
        case 20:
          *word = s_english_minute_quantities[2];
          return true;
        case 25:
          *word = s_english_minute_quantities[3];
          return true;
        default:
          return false;
      }
    default:
      return false;
  }
}

bool clock_language_get_word(ClockLanguage language, ClockWord requested_word,
                             ClockGridWord *word) {
  if (!word || requested_word >= CLOCK_WORD_COUNT) {
    return false;
  }

  switch (language) {
    case CLOCK_LANGUAGE_EN:
      *word = s_english_words[requested_word];
      return true;
    default:
      return false;
  }
}
