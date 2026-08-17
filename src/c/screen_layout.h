#pragma once

#include <pebble.h>

typedef struct {
  GRect bounds;
  int16_t cell_width;
  int16_t cell_height;
  int16_t row_pitch;
  int16_t grid_top;
  int16_t minute_dot_y;
  GFont letter_font;
} ScreenLayout;

// Builds the display-specific geometry used to draw the word grid.
ScreenLayout screen_layout_create(GRect bounds, uint8_t columns, uint8_t rows,
                                  GFont letter_font);

// Returns the exact bounds of one grid cell, including the trailing pixels.
GRect screen_layout_cell_rect(const ScreenLayout *layout, uint8_t row,
                              uint8_t column, uint8_t columns, uint8_t rows);

// Draws up to four minute indicator dots using the display-specific layout.
void screen_layout_draw_minute_dots(GContext *ctx, const ScreenLayout *layout,
                                    uint8_t count, GColor color);
