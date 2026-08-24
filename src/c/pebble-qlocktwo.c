#include <pebble.h>

#include "clock_language.h"
#include "color_theme.h"
#include "screen_layout.h"

#define PERSIST_KEY_COLOR_THEME 1
#define PERSIST_KEY_CLOCK_LANGUAGE 2

static Window *s_window;
static Layer *s_grid_layer;
static GFont s_letter_font;
static ClockLanguage s_clock_language = CLOCK_LANGUAGE_EN;
static const ClockLanguageProfile *s_clock_profile;
static const char *const *s_letter_grid;
static uint8_t s_hours;
static uint8_t s_minutes_rounded_to_five;
static uint8_t s_minutes_modulo_five;
static ColorThemeId s_color_theme = COLOR_THEME_BLACK;

static void prv_inbox_received_handler(DictionaryIterator *iterator,
                                       void *context) {
  Tuple *language = dict_find(iterator, MESSAGE_KEY_Language);
  if (language && language->type == TUPLE_CSTRING) {
    s_clock_language = clock_language_from_string(language->value->cstring);
    s_clock_profile = clock_language_get_profile(s_clock_language);
    s_letter_grid = s_clock_profile->grid;
    persist_write_int(PERSIST_KEY_CLOCK_LANGUAGE, s_clock_language);
    layer_mark_dirty(s_grid_layer);
  }

  Tuple *color = dict_find(iterator, MESSAGE_KEY_Color);
  if (color && color->type == TUPLE_CSTRING) {
    s_color_theme = color_theme_id_from_string(color->value->cstring);
    persist_write_int(PERSIST_KEY_COLOR_THEME, s_color_theme);
    layer_mark_dirty(s_grid_layer);
  }

  (void)context;
}

static void prv_update_time(struct tm *tick_time) {
  s_hours = tick_time->tm_hour;
  s_minutes_rounded_to_five = (tick_time->tm_min / 5) * 5;
  s_minutes_modulo_five = tick_time->tm_min % 5;

  if (s_grid_layer) {
    layer_mark_dirty(s_grid_layer);
  }
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  (void)units_changed;
  prv_update_time(tick_time);
}

static bool prv_is_grid_word_letter(const ClockGridWord *word, uint8_t row,
                                    uint8_t column) {
  return word && row == word->row && column >= word->column &&
         column < word->column + word->length;
}

static bool prv_is_phrase_letter(uint8_t row, uint8_t column) {
  const ClockMinuteRule *rule =
      &s_clock_profile->minute_rules[s_minutes_rounded_to_five / 5];
  uint8_t number;
  ClockHourForm hour_form;
  ClockWordSet hour_words;
  if (!clock_language_get_hour_phrase(s_clock_profile,
                                      s_hours + rule->hour_offset, &number,
                                      &hour_form, &hour_words)) {
    return false;
  }

  const ClockWordSet active_words =
      rule->words | rule->hour_form_words[hour_form] | hour_words;
  for (uint8_t word = 0; word < s_clock_profile->word_count; ++word) {
    if ((active_words & CLOCK_WORD_SET_BIT(word)) &&
        prv_is_grid_word_letter(&s_clock_profile->words[word], row, column)) {
      return true;
    }
  }

  if (number &&
      prv_is_grid_word_letter(&s_clock_profile->numbers[number - 1], row,
                              column)) {
    return true;
  }

  if (rule->minute_quantity == CLOCK_MINUTE_QUANTITY_NONE) {
    return false;
  }

  return prv_is_grid_word_letter(
      &s_clock_profile->minute_quantities[rule->minute_quantity - 1], row,
      column);
}

static void prv_grid_layer_update(Layer *layer, GContext *ctx) {
  const ScreenLayout layout = screen_layout_create(layer_get_bounds(layer),
                                                    CLOCK_GRID_COLUMNS,
                                                    CLOCK_GRID_ROWS,
                                                    s_letter_font);
  const ColorTheme *theme = color_theme_get(s_color_theme);

  graphics_context_set_fill_color(ctx, theme->background);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  for (int row = 0; row < CLOCK_GRID_ROWS; ++row) {
    for (int column = 0; column < CLOCK_GRID_COLUMNS; ++column) {
      const GRect cell = screen_layout_cell_rect(&layout, row, column,
                                                 CLOCK_GRID_COLUMNS,
                                                 CLOCK_GRID_ROWS);
      const char letter[] = { s_letter_grid[row][column], '\0' };
      const bool is_active = prv_is_phrase_letter(row, column);

      graphics_context_set_text_color(ctx,
                                      is_active ? theme->active_text
                                                : theme->inactive_text);
      // Pebble custom fonts have no bold style; draw a second, one-pixel-shifted
      // glyph to give every letter a consistent bold weight.
      graphics_draw_text(ctx, letter, layout.letter_font, cell,
                         GTextOverflowModeTrailingEllipsis,
                         GTextAlignmentCenter, NULL);
      graphics_draw_text(ctx, letter, layout.letter_font,
                         GRect(cell.origin.x + 1, cell.origin.y,
                               cell.size.w, cell.size.h),
                         GTextOverflowModeTrailingEllipsis,
                         GTextAlignmentCenter, NULL);
    }
  }

  screen_layout_draw_minute_dots(ctx, &layout, s_minutes_modulo_five,
                                 theme->active_text);
}

static uint32_t prv_letter_font_resource_id(void) {
  return PBL_PLATFORM_SWITCH(
      PBL_PLATFORM_TYPE_CURRENT, RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_18,
      RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_18,
      RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_18,
      RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_18,
      RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_26,
      RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_18,
      RESOURCE_ID_FONT_WORDCLOCK_STENCIL_MONO_18);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  s_letter_font = fonts_load_custom_font(
      resource_get_handle(prv_letter_font_resource_id()));
  s_grid_layer = layer_create(layer_get_bounds(window_layer));
  layer_set_update_proc(s_grid_layer, prv_grid_layer_update);
  layer_add_child(window_layer, s_grid_layer);
}

static void prv_window_unload(Window *window) {
  layer_destroy(s_grid_layer);
  fonts_unload_custom_font(s_letter_font);
}

static void prv_init(void) {
  s_clock_profile = clock_language_get_profile(s_clock_language);
  s_letter_grid = s_clock_profile->grid;
  if (persist_exists(PERSIST_KEY_COLOR_THEME)) {
    s_color_theme = persist_read_int(PERSIST_KEY_COLOR_THEME);
  }
  if (persist_exists(PERSIST_KEY_CLOCK_LANGUAGE)) {
    const int stored_language = persist_read_int(PERSIST_KEY_CLOCK_LANGUAGE);
    if (stored_language >= CLOCK_LANGUAGE_EN &&
        stored_language < CLOCK_LANGUAGE_COUNT) {
      s_clock_language = stored_language;
      s_clock_profile = clock_language_get_profile(s_clock_language);
      s_letter_grid = s_clock_profile->grid;
    }
  }

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(s_window, true);

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(),
                   app_message_outbox_size_maximum());

  time_t now = time(NULL);
  prv_update_time(localtime(&now));
  tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
