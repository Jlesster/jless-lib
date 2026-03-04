#include "tui.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

TuiBuffer *tui_buffer_create(int cols, int rows) {
  if(cols <= 0 || rows <= 0 || cols > TUI_MAX_COLS || rows > TUI_MAX_ROWS) {
    return NULL;
  }

  TuiBuffer *buf = malloc(sizeof(TuiBuffer));
  if(buf == NULL) {
    return NULL;
  }

  buf->cells = malloc(sizeof(TuiCell) * (size_t)rows * (size_t)cols);
  if(buf->cells == NULL) {
    free(buf);
    return NULL;
  }

  buf->rows = rows;
  buf->cols = cols;

  tui_buffer_clear(buf);

  return buf;
}

void tui_buffer_free(TuiBuffer *buf) {
  if(buf == NULL) {
    return;
  }

  free(buf->cells);
  buf->cells = NULL;

  free(buf);
}

void tui_buffer_clear(TuiBuffer *buf) {
  if(buf == NULL || buf->cells == NULL) {
    return;
  }

  int total = buf->rows * buf->cols;
  for(int i = 0; i < total; i++) {
    buf->cells[i].ch   = ' ';
    buf->cells[i].fg   = TUI_COLOR_DEFAULT;
    buf->cells[i].bg   = TUI_COLOR_DEFAULT;
    buf->cells[i].bold = 0;
  }
}

void tui_buffer_set_cell(
    TuiBuffer *buf, int col, int row, uint32_t ch, int fg, int bg, uint8_t bold) {
  if(buf == NULL) return;

  if(col < 0 || row < 0 || col >= buf->cols || row >= buf->rows) return;

  int idx = row * buf->cols + col;

  buf->cells[idx].ch   = ch;
  buf->cells[idx].fg   = fg;
  buf->cells[idx].bg   = bg;
  buf->cells[idx].bold = bold;
}

TuiCell *tui_buffer_get_cell(TuiBuffer *buf, int col, int row) {
  if(buf == NULL) return NULL;

  if(col < 0 || row < 0 || col >= buf->cols || row >= buf->rows) return NULL;

  return &buf->cells[row * buf->cols + col];
}
