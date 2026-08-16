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
  [COLOR_THEME_RED] = {
    .background = GColorRed,
    .active_text = GColorWhite,
    .inactive_text = COLOR_FALLBACK(GColorDarkCandyAppleRed, GColorBlack),
  },
  [COLOR_THEME_PURPLE] = {
    .background = GColorVividViolet,
    .active_text = GColorWhite,
    .inactive_text = COLOR_FALLBACK(GColorDarkGray, GColorBlack),
  },
  [COLOR_THEME_AZURE] = {
    .background = GColorPictonBlue,
    .active_text = GColorWhite,
    .inactive_text = COLOR_FALLBACK(GColorBabyBlueEyes, GColorBlack),
  },
  [COLOR_THEME_GREEN] = {
    .background = GColorSpringBud,
    .active_text = GColorBlack,
    .inactive_text = COLOR_FALLBACK(GColorMediumAquamarine, GColorBlack),
  },
  [COLOR_THEME_PINK] = {
    .background = GColorBrilliantRose,
    .active_text = GColorWhite,
    .inactive_text = COLOR_FALLBACK(GColorJazzberryJam, GColorBlack),
  },
  [COLOR_THEME_BROWN] = {
    .background = GColorBrass,
    .active_text = GColorWhite,
    .inactive_text = COLOR_FALLBACK(GColorArmyGreen, GColorBlack),
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
  if (value && strcmp(value, "red") == 0) {
    return COLOR_THEME_RED;
  }
  if (value && strcmp(value, "purple") == 0) {
    return COLOR_THEME_PURPLE;
  }
  if (value && strcmp(value, "azure") == 0) {
    return COLOR_THEME_AZURE;
  }
  if (value && strcmp(value, "green") == 0) {
    return COLOR_THEME_GREEN;
  }
  if (value && strcmp(value, "pink") == 0) {
    return COLOR_THEME_PINK;
  }
  if (value && strcmp(value, "brown") == 0) {
    return COLOR_THEME_BROWN;
  }

  return COLOR_THEME_BLACK;
}
