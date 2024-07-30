CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g
LDFLAGS =

RM = rm -f
MKDIR = mkdir -p ../build/core

SRC = main.c core/server.c core/client.c core/db.c core/network.c core/util.c core/log_util.c
OBJ = $(patsubst %.c,../build/%.o,$(SRC))
TARGET = ../build/t-speed-kv

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

../build/%.o: %.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../build/t-speed-kv $(OBJ)

.PHONY: all clean
