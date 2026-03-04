#include "tui.h"
#include <stdint.h>
#include <string.h>

void tui_widget_label(
    TuiBuffer *buf, int x, int y, const char *text, int fg, int bg, uint8_t bold) {

  if(buf == NULL || text == NULL) return;

  for(int i = 0; text[i] != '\0'; i++) {
    uint32_t cp = (uint32_t)(unsigned char)text[i];
    tui_buffer_set_cell(buf, x + i, y, cp, fg, bg, bold);
  }
}

void tui_widget_list(TuiBuffer *buf,
                     TuiRect    rect,
                     TuiList   *list,
                     int        fg,
                     int        bg,
                     int        sel_fg,
                     int        sel_bg) {
  if(buf == NULL || list == NULL) return;
  if(rect.w <= 0 || rect.h <= 0) return;

  int visible_rows = rect.h;

  for(int row_idx = 0; row_idx < visible_rows; row_idx++) {
    int item_idx = list->offset + row_idx;

    int buf_row = rect.y + row_idx;
    int buf_col = rect.x;

    int is_selected = (item_idx == list->selected);
    int cur_fg      = is_selected ? sel_fg : fg;
    int cur_bg      = is_selected ? sel_bg : bg;

    if(item_idx < list->count) {
      const char *text     = list->items[item_idx];
      int         text_len = (int)strlen(text);

      for(int col = 0; col < rect.w; col++) {
        uint32_t ch;
        if(col < text_len) {
          ch = (uint32_t)(unsigned char)text[col];
        } else {
          ch = ' ';
        }
        tui_buffer_set_cell(buf, buf_col + col, buf_row, ch, cur_fg, cur_bg, 0);
      }
    } else {
      for(int col = 0; col < rect.w; col++) {
        tui_buffer_set_cell(buf, buf_col + col, buf_row, ' ', fg, bg, 0);
      }
    }
  }
}

void tui_widget_list_move(TuiList *list, int delta, int visible_rows) {
  if(list == NULL || list->count <= 0) return;

  list->selected += delta;
  if(list->selected < 0) list->selected = 0;
  if(list->selected >= list->count) list->selected = list->count - 1;

  if(list->selected < list->offset) {
    list->offset = list->selected;
  }

  if(list->selected >= list->offset + visible_rows) {
    list->offset = list->selected - visible_rows + 1;
  }

  if(list->offset < 0) list->offset = 0;
}
