#include <pebble.h>

#include "clock_language.h"

#define GRID_COLUMNS 11
#define GRID_ROWS CLOCK_GRID_ROWS

static Window *s_window;
static Layer *s_grid_layer;
static const ClockLanguage s_clock_language = CLOCK_LANGUAGE_EN;
static const char *const *s_letter_grid;

static void prv_grid_layer_update(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  const int16_t cell_width = bounds.size.w / GRID_COLUMNS;
  const int16_t cell_height = bounds.size.h / GRID_ROWS;
  const GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_text_color(ctx, GColorWhite);

  for (int row = 0; row < GRID_ROWS; ++row) {
    for (int column = 0; column < GRID_COLUMNS; ++column) {
      const int16_t x = column * cell_width;
      const int16_t y = row * cell_height;
      const int16_t width = (column == GRID_COLUMNS - 1) ? bounds.size.w - x
                                                          : cell_width;
      const int16_t height = (row == GRID_ROWS - 1) ? bounds.size.h - y
                                                     : cell_height;
      const GRect cell = GRect(x, y, width, height);
      const char letter[] = { s_letter_grid[row][column], '\0' };

      graphics_fill_rect(ctx, cell, 0, GCornerNone);
      graphics_draw_text(ctx, letter, font, cell,
                         GTextOverflowModeTrailingEllipsis,
                         GTextAlignmentCenter, NULL);
    }
  }
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  s_grid_layer = layer_create(layer_get_bounds(window_layer));
  layer_set_update_proc(s_grid_layer, prv_grid_layer_update);
  layer_add_child(window_layer, s_grid_layer);
}

static void prv_window_unload(Window *window) {
  layer_destroy(s_grid_layer);
}

static void prv_init(void) {
  s_letter_grid = clock_language_get_grid(s_clock_language);

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(s_window, true);
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
