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
  CLOCK_LANGUAGE_DE,
  CLOCK_LANGUAGE_NL,
  CLOCK_LANGUAGE_PT,
  CLOCK_LANGUAGE_SE,
  CLOCK_LANGUAGE_DK,
  CLOCK_LANGUAGE_NO,
  CLOCK_LANGUAGE_CA,
  CLOCK_LANGUAGE_COUNT,
} ClockLanguage;

typedef struct {
  uint8_t row;
  uint8_t column;
  uint8_t length;
} ClockGridWord;

#define CLOCK_WORD_SET_MAX 32

typedef uint32_t ClockWordSet;

#define CLOCK_WORD_SET_BIT(index) ((ClockWordSet)1u << (index))

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

typedef enum {
  CLOCK_NUMBER_FORM_DEFAULT,
  CLOCK_NUMBER_FORM_OCLOCK,
  CLOCK_NUMBER_FORM_COUNT,
} ClockNumberForm;

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
  const ClockGridWord *number_forms[CLOCK_NUMBER_FORM_COUNT];
  const ClockGridWord *minute_quantities;
  const ClockGridWord *words;
  uint8_t word_count;
  ClockWordSet oclock_words;
  const ClockMinuteRule *minute_rules;
  ClockHourForm form_for_one;
  ClockHourForm form_for_other;
  const ClockHourOverride *hour_overrides;
  uint8_t hour_override_count;
} ClockLanguageProfile;

ClockLanguage clock_language_from_string(const char *value);
const ClockLanguageProfile *clock_language_get_profile(ClockLanguage language);
bool clock_language_get_grid_letter(const ClockLanguageProfile *profile,
                                    uint8_t row, uint8_t column,
                                    char letter[5]);
bool clock_language_get_hour_phrase(const ClockLanguageProfile *profile,
                                    uint8_t hour, uint8_t *number,
                                    ClockHourForm *form,
                                    ClockWordSet *words);
