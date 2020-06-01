CC = gcc
CFLAGS = -Wall

SRC = src/cat.c \
      src/true.c \
      src/false.c \
      src/wc.c \
      src/unlink.c \
      src/ls.c

OUT_DIR = bin

all: $(SRC)
	@if test -d bin; then printf ""; else mkdir bin; fi
	$(CC) src/cat.c $(CFLAGS) -o $(OUT_DIR)/cat
	$(CC) src/true.c $(CFLAGS) -o $(OUT_DIR)/true
	$(CC) src/false.c $(CFLAGS) -o $(OUT_DIR)/false
	$(CC) src/wc.c $(CFLAGS) -o $(OUT_DIR)/wc
	$(CC) src/unlink.c $(CFLAGS) -o $(OUT_DIR)/unlink
	$(CC) src/ls.c $(CFLAGS) -o $(OUT_DIR)/ls
	@echo "Output dir: $(OUT_DIR)/"

.PHONY: clean

clean:
	rm -rf $(OUT_DIR)/
