#include "tui.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RENDER_BUF_SZE 65536
static char s_out[RENDER_BUF_SZE];
static int  s_out_pos;

static int out_write(const char *bytes, int len) {
  if(s_out_pos + len >= RENDER_BUF_SZE) return -1; /* bounds check */
  memcpy(s_out + s_out_pos, bytes, (size_t)len);
  s_out_pos += len;
  return 0;
}

static int out_puts(const char *str) {
  int len = 0;
  while(str[len])
    len++;
  return out_write(str, len);
}

static int out_utf8(uint32_t cp) {
  char buf[4];
  int  len;

  if(cp < 0x80) {
    buf[0] = (char)cp;
    len    = 1;
  } else if(cp < 0x800) {
    buf[0] = (char)(0xC0 | (cp >> 6));
    buf[1] = (char)(0x80 | (cp & 0x3F));
    len    = 2;
  } else if(cp < 0x10000) {
    buf[0] = (char)(0xE0 | (cp >> 12));
    buf[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
    buf[2] = (char)(0x80 | (cp & 0x3F));
    len    = 3;
  } else {
    buf[0] = (char)(0xF0 | (cp >> 18));
    buf[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    buf[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    buf[3] = (char)(0x80 | (cp & 0x3F));
    len    = 4;
  }
  return out_write(buf, len);
}

int tui_render_flush(TuiBuffer *buf) {
  if(buf == NULL) return -1;

  s_out_pos = 0;
  out_puts("\033[1;1H");

  int last_fg   = -2;
  int last_bg   = -2;
  int last_bold = -1;

  for(int row = 0; row < buf->rows; row++) {
    for(int col = 0; col < buf->cols; col++) {
      TuiCell *cell = tui_buffer_get_cell(buf, col, row);
      if(cell == NULL) continue;

      int need_sgr =
          (cell->fg != last_fg || cell->bg != last_bg || cell->bold != last_bold);

      if(need_sgr) {
        char sgr[TUI_ESC_BUF_SIZE];
        int  pos = 0;

        sgr[pos++] = '\033';
        sgr[pos++] = '[';
        sgr[pos++] = '0';

        if(cell->bold) {
          sgr[pos++] = ';';
          sgr[pos++] = '1';
        }
        if(cell->fg != TUI_COLOR_DEFAULT) {
          pos +=
              snprintf(sgr + pos, (size_t)(TUI_ESC_BUF_SIZE - pos), ";%d", cell->fg);
        }
        if(cell->bg != TUI_COLOR_DEFAULT) {
          pos +=
              snprintf(sgr + pos, (size_t)(TUI_ESC_BUF_SIZE - pos), ";%d", cell->bg);
        }
        sgr[pos++] = 'm';
        out_write(sgr, pos);

        last_fg   = cell->fg;
        last_bg   = cell->bg;
        last_bold = cell->bold;
      }
      out_utf8((uint32_t)cell->ch);
    }
    if(row < buf->rows - 1) {
      char move[TUI_ESC_BUF_SIZE];
      int  mlen = snprintf(move, TUI_ESC_BUF_SIZE, "\033[%d;1H", row + 2);
      out_write(move, mlen);
    }
  }

  out_puts("\033[0m");

  ssize_t written = write(1, s_out, (size_t)s_out_pos);
  if(written < 0) return -1;
  return 0;
}

int tui_render_clear_screen(void) {
  const char *seq = "\033[2J\033[1;1H";
  ssize_t     r   = write(1, seq, 10);
  return (r < 0) ? -1 : 0;
}

int tui_render_hide_cursor(void) {
  ssize_t r = write(1, "\033[?25l", 6);
  return (r < 0) ? -1 : 0;
}

int tui_render_show_cursor(void) {
  ssize_t r = write(1, "\033[?25h", 6);
  return (r < 0) ? -1 : 0;
}
