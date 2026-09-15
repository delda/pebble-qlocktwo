#include "screen_layout.h"

#define DOT_REFERENCE_DISPLAY_HEIGHT 168
#define DOT_REFERENCE_BOTTOM_OFFSET 5
#define MINUTE_DOT_COUNT 4
#define DOT_RADIUS 2
#define GRID_REFERENCE_DISPLAY_HEIGHT 228
#define GRID_REFERENCE_VERTICAL_OFFSET 9
#define ROUND_GRID_SQUARE_SIDE_NUMERATOR 1000
#define ROUND_GRID_SQUARE_DIAGONAL 1414
#define GABBRO_GRID_SIDE_EXPANSION 1

static int16_t prv_scale_from_reference_height(int16_t value,
                                               int16_t display_height) {
  return (value * display_height + DOT_REFERENCE_DISPLAY_HEIGHT / 2) /
         DOT_REFERENCE_DISPLAY_HEIGHT;
}

static int16_t prv_grid_vertical_offset(int16_t display_height) {
  return -(GRID_REFERENCE_VERTICAL_OFFSET * display_height +
           GRID_REFERENCE_DISPLAY_HEIGHT / 2) /
         GRID_REFERENCE_DISPLAY_HEIGHT;
}

static GRect prv_round_grid_bounds(GRect bounds) {
  // A square's diagonal is its side times sqrt(2). The 1000:1414 ratio is
  // sqrt(2) in integer form, so the square touches the circular display.
  const int16_t grid_side =
      (bounds.size.w * ROUND_GRID_SQUARE_SIDE_NUMERATOR) /
      ROUND_GRID_SQUARE_DIAGONAL;

  return GRect(bounds.origin.x + (bounds.size.w - grid_side) / 2,
               bounds.origin.y + (bounds.size.h - grid_side) / 2,
               grid_side, grid_side);
}

static GRect prv_gabbro_grid_bounds(GRect bounds) {
  const int16_t grid_side =
      (bounds.size.w * ROUND_GRID_SQUARE_SIDE_NUMERATOR) /
          ROUND_GRID_SQUARE_DIAGONAL +
      GABBRO_GRID_SIDE_EXPANSION;

  return GRect(bounds.origin.x + (bounds.size.w - grid_side) / 2,
               bounds.origin.y + (bounds.size.h - grid_side) / 2,
               grid_side, grid_side);
}

static GRect prv_grid_bounds(GRect bounds) {
  return PBL_PLATFORM_SWITCH(PBL_PLATFORM_TYPE_CURRENT, bounds, bounds,
                             prv_round_grid_bounds(bounds), bounds, bounds,
                             bounds, prv_gabbro_grid_bounds(bounds));
}

ScreenLayout screen_layout_create(GRect bounds, uint8_t columns, uint8_t rows,
                                  GFont letter_font) {
  const GRect grid_bounds = prv_grid_bounds(bounds);
  ScreenLayout layout = {
    .bounds = grid_bounds,
    .columns = columns,
    .cell_width = grid_bounds.size.w / columns,
    .cell_height = grid_bounds.size.h / rows,
    .minute_dot_y = grid_bounds.origin.y + grid_bounds.size.h -
                    prv_scale_from_reference_height(
                        DOT_REFERENCE_BOTTOM_OFFSET, grid_bounds.size.h),
    .letter_font = letter_font,
  };

  return layout;
}

GRect screen_layout_cell_rect(const ScreenLayout *layout, uint8_t row,
                              uint8_t column, uint8_t columns, uint8_t rows) {
  const int16_t x = layout->bounds.origin.x + column * layout->cell_width;
  const int16_t y = layout->bounds.origin.y + row * layout->cell_height +
                    prv_grid_vertical_offset(layout->bounds.size.h);
  const int16_t width = column == columns - 1
                            ? layout->bounds.origin.x + layout->bounds.size.w - x
                            : layout->cell_width;
  const int16_t height = row == rows - 1
                             ? layout->bounds.origin.y + layout->bounds.size.h - y
                             : layout->cell_height;
  return GRect(x, y, width, height);
}

void screen_layout_draw_minute_dots(GContext *ctx, const ScreenLayout *layout,
                                    uint8_t count, GColor color) {
  if (layout->columns < MINUTE_DOT_COUNT) {
    return;
  }

  const uint8_t dot_count = count > MINUTE_DOT_COUNT ? MINUTE_DOT_COUNT
                                                       : count;
  const uint8_t first_dot_boundary =
      (layout->columns - MINUTE_DOT_COUNT + 1) / 2;
  const int16_t default_first_dot_x =
      first_dot_boundary * layout->cell_width + layout->bounds.origin.x;
  const int16_t centered_first_dot_x =
      layout->bounds.origin.x +
      (layout->bounds.size.w -
       (MINUTE_DOT_COUNT - 1) * layout->cell_width) /
          2;
  const int16_t first_dot_x = PBL_PLATFORM_SWITCH(
      PBL_PLATFORM_TYPE_CURRENT, default_first_dot_x, default_first_dot_x,
      default_first_dot_x, default_first_dot_x, default_first_dot_x,
      default_first_dot_x, centered_first_dot_x);
  const int16_t horizontal_offset = PBL_PLATFORM_SWITCH(
      PBL_PLATFORM_TYPE_CURRENT, 0, 0, 2, 0, 0, 0, 0);
  const int16_t vertical_offset = PBL_PLATFORM_SWITCH(
      PBL_PLATFORM_TYPE_CURRENT, 0, 0, 6, 0, 0, 0, 4);

  graphics_context_set_fill_color(ctx, color);
  for (uint8_t index = 0; index < dot_count; ++index) {
    graphics_fill_circle(ctx,
                         GPoint(first_dot_x + index * layout->cell_width +
                                    horizontal_offset,
                                layout->minute_dot_y + vertical_offset),
                         DOT_RADIUS);
  }
}
