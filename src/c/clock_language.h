#pragma once

#include <stdbool.h>
#include <stdint.h>

#define CLOCK_GRID_COLUMNS 11
#define CLOCK_GRID_ROWS 10
#define CLOCK_MINUTE_RULE_COUNT 12

typedef enum {
  CLOCK_LANGUAGE_EN,
  CLOCK_LANGUAGE_IT,
  CLOCK_LANGUAGE_FR,
  CLOCK_LANGUAGE_ES,
  CLOCK_LANGUAGE_COUNT,
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
  CLOCK_WORD_HOUR_SPECIAL,
  CLOCK_WORD_COUNT,
} ClockWord;

typedef uint16_t ClockWordSet;

#define CLOCK_WORD_BIT(word) ((ClockWordSet)1u << (word))

typedef enum {
  CLOCK_HOUR_FORM_ONE,
  CLOCK_HOUR_FORM_OTHER,
  CLOCK_HOUR_FORM_SPECIAL,
  CLOCK_HOUR_FORM_COUNT,
} ClockHourForm;

typedef enum {
  CLOCK_MINUTE_QUANTITY_NONE,
  CLOCK_MINUTE_QUANTITY_FIVE,
  CLOCK_MINUTE_QUANTITY_TEN,
  CLOCK_MINUTE_QUANTITY_TWENTY,
  CLOCK_MINUTE_QUANTITY_TWENTY_FIVE,
  CLOCK_MINUTE_QUANTITY_COUNT,
} ClockMinuteQuantity;

// A declarative phrase for one five-minute interval.  The active words are
// the union of `words` and the words selected for the displayed hour form.
typedef struct {
  int8_t hour_offset;
  ClockMinuteQuantity minute_quantity;
  ClockWordSet words;
  ClockWordSet hour_form_words[CLOCK_HOUR_FORM_COUNT];
} ClockMinuteRule;

// Exceptional hour spellings, such as the French MINUIT and MIDI.  A number
// of zero means that the hour is represented exclusively by `words`.
typedef struct {
  uint8_t hour;
  uint8_t number;
  ClockHourForm form;
  ClockWordSet words;
} ClockHourOverride;

typedef struct {
  const char *const *grid;
  const ClockGridWord *numbers;
  const ClockGridWord *minute_quantities;
  const ClockGridWord *words;
  const ClockMinuteRule *minute_rules;
  ClockHourForm form_for_one;
  ClockHourForm form_for_other;
  const ClockHourOverride *hour_overrides;
  uint8_t hour_override_count;
} ClockLanguageProfile;

ClockLanguage clock_language_from_string(const char *value);
const ClockLanguageProfile *clock_language_get_profile(ClockLanguage language);
bool clock_language_get_hour_phrase(const ClockLanguageProfile *profile,
                                    uint8_t hour, uint8_t *number,
                                    ClockHourForm *form,
                                    ClockWordSet *words);
