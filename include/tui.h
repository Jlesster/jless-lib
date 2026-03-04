#ifndef TUI_H
#define TUI_H

#include <stddef.h>
#include <stdint.h>

#define TUI_MAX_COLS 512
#define TUI_MAX_ROWS 256

#define TUI_ESC_BUF_SIZE 64

#define TUI_KEY_UNKNOWN     0
#define TUI_KEY_ARROW_UP    1000
#define TUI_KEY_ARROW_DOWN  1001
#define TUI_KEY_ARROW_LEFT  1002
#define TUI_KEY_ARROW_RIGHT 1003

#define TUI_KEY_ENTER     '\r'
#define TUI_KEY_ESCAPE    27
#define TUI_KEY_BACKSPACE 127

#define TUI_BORDER_SINGLE  0
#define TUI_BORDER_DOUBLE  1
#define TUI_BORDER_ROUNDED 2

#define TUI_COLOR_DEFAULT (-1)

typedef struct {
  uint32_t ch;
  int      fg;
  int      bg;
  uint8_t  bold;
} TuiCell;

typedef struct {
  TuiCell *cells;
  int      rows;
  int      cols;
} TuiBuffer;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} TuiRect;

typedef struct {
  const char **items;
  int          count;
  int          selected;
  int          offset;
} TuiList;

typedef int TuiKey;

TuiBuffer *tui_buffer_create(int cols, int rows);

void tui_buffer_free(TuiBuffer *buf);

void tui_buffer_set_cell(
    TuiBuffer *buf, int col, int row, uint32_t ch, int fg, int bg, uint8_t bold);

void tui_buffer_clear(TuiBuffer *buf);

TuiCell *tui_buffer_get_cell(TuiBuffer *buf, int col, int row);

int tui_render_flush(TuiBuffer *buf);

int tui_render_clear_screen(void);

int tui_render_hide_cursor(void);

int tui_render_show_cursor(void);

void tui_box_draw(TuiBuffer *bif, TuiRect rect, int style, int fg, int bg);

void tui_box_fill(TuiBuffer *buf, TuiRect rect, uint32_t ch, int fg, int bg);

int tui_input_init(void);

void tui_input_restore(void);

TuiKey tui_input_read_key(void);

void tui_widget_label(
    TuiBuffer *buf, int x, int y, const char *textw, int fg, int bg, uint8_t bold);

void tui_widget_list(TuiBuffer *buf,
                     TuiRect    rect,
                     TuiList   *list,
                     int        fg,
                     int        bg,
                     int        sel_fg,
                     int        sel_bg);

void tui_widget_list_move(TuiList *list, int delta, int visible_rows);

#endif  // TUI_H
