#include "tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *DEMO_ITEMS[] = {
  "Phase 1 - raw library",    "Phase 2 - App launcher",
  "Phase 3 - Config parser",  "Phase 4 - Expansion",
  "Phase 5 - TUI Components", "[01] Screen Buffer",
  "[02] Box Drawing Engine",  "[03] Input Event Loop",
  "[04] Widget Primitives",   "[05] Desktop File Parser",
  "[06] Fuzzy Finder Engine", "[07] TUI Search Interface",
  "[08] History & Frecency",
};
#define DEMO_ITEM_COUNT 13

static void draw_frame(TuiBuffer *buf, TuiList *list, int term_cols, int term_rows) {
  TuiRect screen = { .x = 0, .y = 0, .w = term_cols, .h = term_rows };
  tui_box_fill(buf, screen, ' ', TUI_COLOR_DEFAULT, TUI_COLOR_DEFAULT);

  TuiRect title_rect = { .x = 0, .y = 0, .w = term_cols, .h = 3 };
  tui_box_draw(buf, title_rect, TUI_BORDER_ROUNDED, 36, TUI_COLOR_DEFAULT);

  const char *title = " jless-lib - C TUI library demo";

  int title_len = (int)strlen(title);
  int title_x   = (term_cols - title_len) / 2;
  tui_widget_label(buf, title_x, 1, title, 36, TUI_COLOR_DEFAULT, 1);

  int     panel_y    = 3;
  int     panel_h    = term_rows - panel_y - 3;
  TuiRect panel_rect = { .x = 0, .y = panel_y, .w = term_cols, .h = panel_h };
  tui_box_draw(buf, panel_rect, TUI_BORDER_DOUBLE, 37, TUI_COLOR_DEFAULT);

  tui_widget_label(buf,
                   2,
                   panel_y + 1,
                   "C learning roadmap, select a project:",
                   33,
                   TUI_COLOR_DEFAULT,
                   1);

  int     list_y    = panel_y + 2;
  int     list_h    = panel_h - 3;
  TuiRect list_rect = { .x = 1, .y = list_y, .w = term_cols - 2, .h = list_h };

  tui_widget_list(buf, list_rect, list, 37, TUI_COLOR_DEFAULT, 30, 37);
  int     status_y    = term_rows - 3;
  TuiRect status_rect = { .x = 0, .y = status_y, .w = term_cols, .h = 3 };
  tui_box_draw(buf, status_rect, TUI_BORDER_SINGLE, 90, TUI_COLOR_DEFAULT);

  char        status[128];
  const char *sel_name = DEMO_ITEMS[list->selected];
  snprintf(status, sizeof(status), " Selected: %s [↑↓ navigate] [q quit]", sel_name);

  tui_widget_label(buf, 2, status_y + 1, status, 90, TUI_COLOR_DEFAULT, 0);

  char scroll_info[32];
  snprintf(
      scroll_info, sizeof(scroll_info), " %d/%d ", list->selected + 1, list->count);

  int si_x = term_cols - (int)strlen(scroll_info) - 2;
  tui_widget_label(buf, si_x, panel_y, scroll_info, 33, TUI_COLOR_DEFAULT, 0);
}

int main(void) {
  if(tui_input_init() < 0) {
    fprintf(stderr, "Failed to init terminal output\n");
    return 1;
  }

  int term_cols = 80;
  int term_rows = 24;

  TuiBuffer *buf = tui_buffer_create(term_cols, term_rows);
  if(buf == NULL) {
    fprintf(stderr, "Failed to create screen buffer\n");
    return 1;
  }

  tui_render_clear_screen();
  tui_render_hide_cursor();

  TuiList list = {
    .items    = DEMO_ITEMS,
    .count    = DEMO_ITEM_COUNT,
    .selected = 0,
    .offset   = 0,
  };

  while(1) {
    tui_buffer_clear(buf);
    draw_frame(buf, &list, term_cols, term_rows);

    tui_render_flush(buf);

    TuiKey key = tui_input_read_key();

    if(key == TUI_KEY_ARROW_UP) {
      tui_widget_list_move(&list, -1, term_rows - 9);
    } else if(key == TUI_KEY_ARROW_DOWN) {
      tui_widget_list_move(&list, 1, term_rows - 9);
    } else if(key == 'q' || key == TUI_KEY_ESCAPE) {
      break;
    }
  }

  tui_render_show_cursor();
  tui_render_clear_screen();
  tui_buffer_free(buf);
  buf = NULL;

  return 0;
}
