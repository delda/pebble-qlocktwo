#include "screen_layout.h"

#define DOT_REFERENCE_DISPLAY_HEIGHT 168
#define DOT_REFERENCE_BOTTOM_OFFSET 5
#define DOT_RADIUS 2
#define CELL_HORIZONTAL_EXPANSION 4
#define CELL_VERTICAL_EXPANSION 12
#define ROW_VERTICAL_COMPACTION 2

static int16_t prv_scale_from_reference_height(int16_t value,
                                               int16_t display_height) {
  return (value * display_height + DOT_REFERENCE_DISPLAY_HEIGHT / 2) /
         DOT_REFERENCE_DISPLAY_HEIGHT;
}

ScreenLayout screen_layout_create(GRect bounds, uint8_t columns, uint8_t rows,
                                  GFont letter_font) {
  const int16_t cell_height = bounds.size.h / rows;
  const int16_t row_pitch = cell_height - ROW_VERTICAL_COMPACTION;
  const int16_t grid_height = row_pitch * (rows - 1) + cell_height;
  ScreenLayout layout = {
    .bounds = bounds,
    .cell_width = bounds.size.w / columns,
    .cell_height = cell_height,
    .row_pitch = row_pitch,
    .grid_top = (bounds.size.h - grid_height) / 2,
    .minute_dot_y = bounds.size.h - prv_scale_from_reference_height(
        DOT_REFERENCE_BOTTOM_OFFSET, bounds.size.h),
    .letter_font = letter_font,
  };

  return layout;
}

GRect screen_layout_cell_rect(const ScreenLayout *layout, uint8_t row,
                              uint8_t column, uint8_t columns, uint8_t rows) {
  const int16_t x = column * layout->cell_width;
  const int16_t y = layout->grid_top + row * layout->row_pitch;
  const int16_t width = column == columns - 1 ? layout->bounds.size.w - x
                                                : layout->cell_width;
  const int16_t height = row == rows - 1
                             ? layout->cell_height + layout->bounds.size.h % rows
                             : layout->cell_height;
  const int16_t horizontal_inset = CELL_HORIZONTAL_EXPANSION / 2;
  const int16_t vertical_inset = CELL_VERTICAL_EXPANSION / 2;

  // Let adjacent letter cells overlap slightly so the larger glyphs are not
  // clipped and the grid has less visual spacing between characters.
  return GRect(x - horizontal_inset, y - vertical_inset,
               width + CELL_HORIZONTAL_EXPANSION,
               height + CELL_VERTICAL_EXPANSION);
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
