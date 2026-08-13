#pragma once

#include <stdint.h>

#define CLOCK_GRID_ROWS 10

typedef enum {
  CLOCK_LANGUAGE_EN,
} ClockLanguage;

const char *const *clock_language_get_grid(ClockLanguage language);
