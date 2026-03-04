CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS =

LIB_SRC  = src/buffer.c src/render.c src/box.c src/input.c src/widget.c
DEMO_SRC = src/demo.c
LIB_OBJ  = $(LIB_SRC:.c=.o)
DEMO_OBJ = $(DEMO_SRC:.c=.o)

TARGET = demo

.PHONY: all clean valgrind

all: $(TARGET)

$(TARGET): $(LIB_OBJ) $(DEMO_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OBJ) $(DEMO_OBJ): include/tui.h

clean:
	rm -f $(LIB_OBJ) $(DEMO_OBJ) $(TARGET)

valgrind:
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)
