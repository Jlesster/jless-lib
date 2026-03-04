#include "tui.h"

typedef struct {
  uint32_t tl, tr, bl, br;
  uint32_t h, v;
} TuiBorderChars;

static const TuiBorderChars BORDER_STYLES[3] = {
  {
   /* single */
      .tl = 0x250C,
   .tr = 0x2510,
   .bl = 0x2514,
   .br = 0x2518,
   .h  = 0x2500,
   .v  = 0x2502,
   },
  {
   /* double */
      .tl = 0x2554,
   .tr = 0x2557,
   .bl = 0x255A,
   .br = 0x255D,
   .h  = 0x2550,
   .v  = 0x2551,
   },
  {
   /* rounded */
      .tl = 0x256D,
   .tr = 0x256E,
   .bl = 0x2570,
   .br = 0x256F,
   .h  = 0x2500,
   .v  = 0x2502,
   },
};

void tui_box_draw(TuiBuffer *buf, TuiRect rect, int style, int fg, int bg) {
  if(buf == NULL) return;
  if(rect.w < 2 || rect.h < 2) return;
  if(style < 0 || style > TUI_BORDER_ROUNDED) style = TUI_BORDER_SINGLE;

  const TuiBorderChars *b = &BORDER_STYLES[style];

  int x1 = rect.x;
  int y1 = rect.y;
  int x2 = rect.x + rect.w - 1;
  int y2 = rect.y + rect.h - 1;

  /* Corners */
  tui_buffer_set_cell(buf, x1, y1, b->tl, fg, bg, 0);
  tui_buffer_set_cell(buf, x2, y1, b->tr, fg, bg, 0);
  tui_buffer_set_cell(buf, x1, y2, b->bl, fg, bg, 0);
  tui_buffer_set_cell(buf, x2, y2, b->br, fg, bg, 0);

  /* Top and bottom edges */
  for(int col = x1 + 1; col < x2; col++) {
    tui_buffer_set_cell(buf, col, y1, b->h, fg, bg, 0);
    tui_buffer_set_cell(buf, col, y2, b->h, fg, bg, 0);
  }

  /* Left and right edges */
  for(int row = y1 + 1; row < y2; row++) {
    tui_buffer_set_cell(buf, x1, row, b->v, fg, bg, 0);
    tui_buffer_set_cell(buf, x2, row, b->v, fg, bg, 0);
  }
}

void tui_box_fill(TuiBuffer *buf, TuiRect rect, uint32_t ch, int fg, int bg) {
  if(buf == NULL) return;
  if(rect.w < 3 || rect.h < 3) return;
  for(int row = rect.y + 1; row < rect.y + rect.h - 1; row++) {
    for(int col = rect.x + 1; col < rect.x + rect.w - 1; col++) {
      tui_buffer_set_cell(buf, col, row, ch, fg, bg, 0);
    }
  }
}
