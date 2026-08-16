#include "screen_layout.h"

#define DOT_REFERENCE_DISPLAY_HEIGHT 168
#define DOT_REFERENCE_BOTTOM_OFFSET 5
#define DOT_RADIUS 2

static int16_t prv_scale_from_reference_height(int16_t value,
                                               int16_t display_height) {
  return (value * display_height + DOT_REFERENCE_DISPLAY_HEIGHT / 2) /
         DOT_REFERENCE_DISPLAY_HEIGHT;
}

ScreenLayout screen_layout_create(GRect bounds, uint8_t columns, uint8_t rows,
                                  GFont letter_font) {
  ScreenLayout layout = {
    .bounds = bounds,
    .cell_width = bounds.size.w / columns,
    .cell_height = bounds.size.h / rows,
    .minute_dot_y = bounds.size.h - prv_scale_from_reference_height(
        DOT_REFERENCE_BOTTOM_OFFSET, bounds.size.h),
    .letter_font = letter_font,
  };

  return layout;
}

GRect screen_layout_cell_rect(const ScreenLayout *layout, uint8_t row,
                              uint8_t column, uint8_t columns, uint8_t rows) {
  const int16_t x = column * layout->cell_width;
  const int16_t y = row * layout->cell_height;
  const int16_t width = column == columns - 1 ? layout->bounds.size.w - x
                                                : layout->cell_width;
  const int16_t height = row == rows - 1 ? layout->bounds.size.h - y
                                           : layout->cell_height;
  return GRect(x, y, width, height);
}

void screen_layout_draw_minute_dots(GContext *ctx, const ScreenLayout *layout,
                                    uint8_t count, GColor color) {
  // The dots sit between S/E, E/O, O/C, and C/L in "TENSEOCLOCK".
  static const uint8_t s_dot_boundaries[] = { 4, 5, 6, 7 };
  const uint8_t dot_count = count > ARRAY_LENGTH(s_dot_boundaries)
                                ? ARRAY_LENGTH(s_dot_boundaries)
                                : count;

  graphics_context_set_fill_color(ctx, color);
  for (uint8_t index = 0; index < dot_count; ++index) {
    graphics_fill_circle(ctx,
                         GPoint(s_dot_boundaries[index] * layout->cell_width,
                                layout->minute_dot_y),
                         DOT_RADIUS);
  }
}
