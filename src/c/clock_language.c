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

const char *const *clock_language_get_grid(ClockLanguage language) {
  switch (language) {
    case CLOCK_LANGUAGE_EN:
    default:
      return s_english_grid;
  }
}
