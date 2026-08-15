#include "color_theme.h"

#include <string.h>

static const ColorTheme s_themes[COLOR_THEME_COUNT] = {
  [COLOR_THEME_BLACK] = {
    .background = GColorBlack,
    .active_text = GColorWhite,
    .inactive_text = GColorDarkGray,
  },
  [COLOR_THEME_WHITE] = {
    .background = GColorWhite,
    .active_text = GColorBlack,
    .inactive_text = COLOR_FALLBACK(GColorLightGray, GColorBlack),
  },
};

const ColorTheme *color_theme_get(ColorThemeId id) {
  if (id >= COLOR_THEME_COUNT) {
    id = COLOR_THEME_BLACK;
  }

  return &s_themes[id];
}

ColorThemeId color_theme_id_from_string(const char *value) {
  if (value && strcmp(value, "white") == 0) {
    return COLOR_THEME_WHITE;
  }

  return COLOR_THEME_BLACK;
}
