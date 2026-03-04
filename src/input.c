#include "tui.h"
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios s_orig_termios;
static int            s_orig_set = 0;

void tui_input_restore(void) {
  if(!s_orig_set) return;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_orig_termios);

  s_orig_set = 0;
}

int tui_input_init(void) {
  if(tcgetattr(STDIN_FILENO, &s_orig_termios) < 0) {
    fprintf(stderr, "ERR: tcgetattr failed\n");
    return -1;
  }

  s_orig_set = 1;

  atexit(tui_input_restore);

  struct termios raw = s_orig_termios;

  raw.c_lflag &= (tcflag_t) ~(ECHO | ICANON | ISIG | IEXTEN);

  raw.c_iflag &= (tcflag_t) ~(IXON | ICRNL | BRKINT);

  raw.c_oflag &= (tcflag_t) ~(OPOST);

  raw.c_cc[VMIN]  = 1;
  raw.c_cc[VTIME] = 0;

  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
    fprintf(stderr, "ERR: tcsetattr failed\n");
    s_orig_set = 0;
    return -1;
  }
  return 0;
}

TuiKey tui_input_read_key(void) {
  unsigned char byte;
  ssize_t       n;
  n = read(STDIN_FILENO, &byte, 1);
  if(n <= 0) {
    return TUI_KEY_UNKNOWN;
  }

  if(byte != TUI_KEY_ESCAPE) {
    return (TuiKey)byte;
  }

  struct pollfd pfd;
  pfd.fd      = STDIN_FILENO;
  pfd.events  = POLLIN;
  pfd.revents = 0;

  int ready = poll(&pfd, 1, 50);

  if(ready <= 0) {
    return TUI_KEY_ESCAPE;
  }

  unsigned char seq0;
  n = read(STDIN_FILENO, &seq0, 1);
  if(n <= 0) {
    return TUI_KEY_UNKNOWN;
  }

  if(seq0 != '[') {
    return TUI_KEY_UNKNOWN;
  }

  unsigned char seq1;
  n = read(STDIN_FILENO, &seq1, 1);
  if(n <= 0) return TUI_KEY_UNKNOWN;

  switch(seq1) {
    case 'A': return TUI_KEY_ARROW_UP;
    case 'B': return TUI_KEY_ARROW_DOWN;
    case 'C': return TUI_KEY_ARROW_RIGHT;
    case 'D': return TUI_KEY_ARROW_LEFT;
    default: return TUI_KEY_UNKNOWN;
  }
}
