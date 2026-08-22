#pragma once

#include <stdbool.h>
#include <stdint.h>

#define CLOCK_GRID_ROWS 10

typedef enum {
  CLOCK_LANGUAGE_EN,
  CLOCK_LANGUAGE_IT,
  CLOCK_LANGUAGE_FR,
} ClockLanguage;

typedef struct {
  uint8_t row;
  uint8_t column;
  uint8_t length;
} ClockGridWord;

typedef enum {
  CLOCK_WORD_IT,
  CLOCK_WORD_IS,
  CLOCK_WORD_A,
  CLOCK_WORD_OCLOCK,
  CLOCK_WORD_HALF,
  CLOCK_WORD_QUARTER,
  CLOCK_WORD_PAST,
  CLOCK_WORD_TO,
  CLOCK_WORD_QUARTER_ARTICLE,
  CLOCK_WORD_SINGULAR_PREFIX,
  CLOCK_WORD_COUNT,
} ClockWord;

ClockLanguage clock_language_from_string(const char *value);
const char *const *clock_language_get_grid(ClockLanguage language);
bool clock_language_get_number(ClockLanguage language, uint8_t number,
                               ClockGridWord *word);
bool clock_language_get_minute_quantity(ClockLanguage language, uint8_t minutes,
                                        ClockGridWord *word);
bool clock_language_get_word(ClockLanguage language, ClockWord requested_word,
                             ClockGridWord *word);
