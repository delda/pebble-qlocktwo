#include <pebble.h>

#include "clock_language.h"
#include "color_theme.h"
#include "screen_layout.h"

#define GRID_COLUMNS 11
#define GRID_ROWS CLOCK_GRID_ROWS
#define PERSIST_KEY_COLOR_THEME 1
#define PERSIST_KEY_CLOCK_LANGUAGE 2

static Window *s_window;
static Layer *s_grid_layer;
static GFont s_letter_font;
static ClockLanguage s_clock_language = CLOCK_LANGUAGE_EN;
static const char *const *s_letter_grid;
static uint8_t s_hours;
static uint8_t s_minutes;
static uint8_t s_seconds;
static uint8_t s_minutes_rounded_to_five;
static uint8_t s_minutes_modulo_five;
static ColorThemeId s_color_theme = COLOR_THEME_BLACK;

static void prv_inbox_received_handler(DictionaryIterator *iterator,
                                       void *context) {
  Tuple *language = dict_find(iterator, MESSAGE_KEY_Language);
  if (language && language->type == TUPLE_CSTRING) {
    s_clock_language = clock_language_from_string(language->value->cstring);
    s_letter_grid = clock_language_get_grid(s_clock_language);
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
  s_minutes = tick_time->tm_min;
  s_seconds = tick_time->tm_sec;
  s_minutes_rounded_to_five = (s_minutes / 5) * 5;
  s_minutes_modulo_five = s_minutes % 5;

  if (s_grid_layer) {
    layer_mark_dirty(s_grid_layer);
  }
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  (void)units_changed;
  prv_update_time(tick_time);
}

static bool prv_is_word_letter(ClockWord requested_word, uint8_t row,
                               uint8_t column) {
  ClockGridWord word;
  return clock_language_get_word(s_clock_language, requested_word, &word) &&
         row == word.row && column >= word.column &&
         column < word.column + word.length;
}

static bool prv_is_hour_number_letter(uint8_t hour, uint8_t row,
                                      uint8_t column) {
  const uint8_t hour_on_clock = hour % 12;
  const uint8_t display_hour = hour_on_clock == 0 ? 12 : hour_on_clock;
  ClockGridWord word;
  return clock_language_get_number(s_clock_language, display_hour, &word) &&
         row == word.row && column >= word.column &&
         column < word.column + word.length;
}

static bool prv_is_number_letter(uint8_t row, uint8_t column) {
  if (s_minutes >= 5) {
    return false;
  }

  return prv_is_hour_number_letter(s_hours, row, column);
}

static bool prv_is_half_past_letter(uint8_t row, uint8_t column) {
  if (s_minutes < 30 || s_minutes >= 35) {
    return false;
  }

  if (prv_is_word_letter(CLOCK_WORD_HALF, row, column) ||
      prv_is_word_letter(CLOCK_WORD_PAST, row, column)) {
    return true;
  }

  return prv_is_hour_number_letter(s_hours, row, column);
}

static bool prv_is_quarter_letter(uint8_t row, uint8_t column) {
  const bool is_quarter_past = s_minutes >= 15 && s_minutes < 20;
  const bool is_quarter_to = s_minutes >= 45 && s_minutes < 50;
  if (!is_quarter_past && !is_quarter_to) {
    return false;
  }

  if (s_clock_language == CLOCK_LANGUAGE_IT) {
    if (prv_is_word_letter(CLOCK_WORD_QUARTER_ARTICLE, row, column) ||
        prv_is_word_letter(CLOCK_WORD_QUARTER, row, column) ||
        (is_quarter_past &&
         prv_is_word_letter(CLOCK_WORD_PAST, row, column)) ||
        (is_quarter_to && prv_is_word_letter(CLOCK_WORD_TO, row, column))) {
      return true;
    }

    return prv_is_hour_number_letter(s_hours + (is_quarter_to ? 1 : 0), row,
                                     column);
  }

  if (prv_is_word_letter(CLOCK_WORD_A, row, column) ||
      prv_is_word_letter(CLOCK_WORD_QUARTER, row, column) ||
      (is_quarter_past && prv_is_word_letter(CLOCK_WORD_PAST, row, column)) ||
      (is_quarter_to && prv_is_word_letter(CLOCK_WORD_TO, row, column))) {
    return true;
  }

  return prv_is_hour_number_letter(s_hours + (is_quarter_to ? 1 : 0), row,
                                   column);
}

static bool prv_is_minute_quantity_letter(uint8_t row, uint8_t column) {
  if (s_minutes < 5 || (s_minutes >= 15 && s_minutes < 20) ||
      (s_minutes >= 30 && s_minutes < 35) ||
      (s_minutes >= 45 && s_minutes < 50)) {
    return false;
  }

  const bool is_minutes_past = s_minutes < 30;
  if ((is_minutes_past &&
       prv_is_word_letter(CLOCK_WORD_PAST, row, column)) ||
      (!is_minutes_past && prv_is_word_letter(CLOCK_WORD_TO, row, column))) {
    return true;
  }

  const uint8_t quantity = s_minutes_rounded_to_five <= 30
                               ? s_minutes_rounded_to_five
                               : 60 - s_minutes_rounded_to_five;
  ClockGridWord word;
  if (clock_language_get_minute_quantity(s_clock_language, quantity, &word) &&
      row == word.row && column >= word.column &&
      column < word.column + word.length) {
    return true;
  }

  return prv_is_hour_number_letter(
      s_hours + (is_minutes_past ? 0 : 1), row, column);
}

static bool prv_is_common_word_letter(uint8_t row, uint8_t column) {
  if (s_clock_language == CLOCK_LANGUAGE_IT) {
    const uint8_t displayed_hour =
        s_hours + (s_minutes >= 35 ? 1 : 0);
    if (displayed_hour % 12 == 1) {
      return prv_is_word_letter(CLOCK_WORD_SINGULAR_PREFIX, row, column);
    }
  }

  if (prv_is_word_letter(CLOCK_WORD_IT, row, column) ||
      prv_is_word_letter(CLOCK_WORD_IS, row, column)) {
    return true;
  }

  return s_minutes < 5 &&
         prv_is_word_letter(CLOCK_WORD_OCLOCK, row, column);
}

static void prv_grid_layer_update(Layer *layer, GContext *ctx) {
  const ScreenLayout layout = screen_layout_create(layer_get_bounds(layer),
                                                    GRID_COLUMNS, GRID_ROWS,
                                                    s_letter_font);
  const ColorTheme *theme = color_theme_get(s_color_theme);

  graphics_context_set_fill_color(ctx, theme->background);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  for (int row = 0; row < GRID_ROWS; ++row) {
    for (int column = 0; column < GRID_COLUMNS; ++column) {
      const GRect cell = screen_layout_cell_rect(&layout, row, column,
                                                 GRID_COLUMNS, GRID_ROWS);
      const char letter[] = { s_letter_grid[row][column], '\0' };
      const bool is_active =
          prv_is_number_letter(row, column) ||
          prv_is_half_past_letter(row, column) ||
          prv_is_quarter_letter(row, column) ||
          prv_is_minute_quantity_letter(row, column) ||
          prv_is_common_word_letter(row, column);

      graphics_context_set_text_color(ctx,
                                      is_active ? theme->active_text
                                                : theme->inactive_text);
      graphics_draw_text(ctx, letter, layout.letter_font, cell,
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
  s_letter_grid = clock_language_get_grid(s_clock_language);
  if (persist_exists(PERSIST_KEY_COLOR_THEME)) {
    s_color_theme = persist_read_int(PERSIST_KEY_COLOR_THEME);
  }
  if (persist_exists(PERSIST_KEY_CLOCK_LANGUAGE)) {
    const int stored_language = persist_read_int(PERSIST_KEY_CLOCK_LANGUAGE);
    if (stored_language == CLOCK_LANGUAGE_IT) {
      s_clock_language = CLOCK_LANGUAGE_IT;
      s_letter_grid = clock_language_get_grid(s_clock_language);
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
  tick_timer_service_subscribe(SECOND_UNIT, prv_tick_handler);
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
