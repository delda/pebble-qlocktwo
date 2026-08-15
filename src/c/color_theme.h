#pragma once

#include <pebble.h>

// A named foreground/background combination used by the word grid.
typedef struct {
  GColor background;
  GColor active_text;
  GColor inactive_text;
} ColorTheme;

typedef enum {
  COLOR_THEME_BLACK,
  COLOR_THEME_WHITE,
  COLOR_THEME_COUNT,
} ColorThemeId;

// Returns the selected theme, or the black theme when id is invalid.
const ColorTheme *color_theme_get(ColorThemeId id);

// Converts a value received from Clay to a theme id. Unknown values use black.
ColorThemeId color_theme_id_from_string(const char *value);

