CC = gcc
CFLAGS = -Wall -g

SRC = src/cat.c \
      src/true.c \
      src/false.c \
      src/yes.c \
      src/wc.c \
      src/unlink.c \
      src/rm.c \
      src/ls.c \
      src/touch.c \
      src/whoami.c \
      src/pwd.c \
      src/base32.c \
      src/mkdir.c

ALL_BIN = cat \
	  true \
	  false \
	  yes \
	  wc \
	  unlink \
	  rm \
	  ls \
	  touch \
	  whoami \
	  pwd \
	  base32 \
	  mkdir

OUT_DIR = bin
INSTALL_DIR = /usr/bin

all: $(SRC) check_bin_dir $(ALL_BIN)
	@echo "Output dir: $(OUT_DIR)/"

check_bin_dir:
	@if test -d bin; then printf ""; else mkdir bin; fi

cat: src/cat.c check_bin_dir
	$(CC) src/cat.c $(CFLAGS) -o $(OUT_DIR)/cat

true: src/true.c check_bin_dir
	$(CC) src/true.c $(CFLAGS) -o $(OUT_DIR)/true

false: src/false.c check_bin_dir
	$(CC) src/false.c $(CFLAGS) -o $(OUT_DIR)/false

yes: src/yes.c check_bin_dir
	$(CC) src/yes.c $(CFLAGS) -o $(OUT_DIR)/yes

wc: src/wc.c check_bin_dir
	$(CC) src/wc.c $(CFLAGS) -o $(OUT_DIR)/wc

unlink: src/unlink.c check_bin_dir
	$(CC) src/unlink.c $(CFLAGS) -o $(OUT_DIR)/unlink

rm: src/rm.c check_bin_dir
	$(CC) src/rm.c $(CFLAGS) -o $(OUT_DIR)/rm

ls: src/ls.c check_bin_dir
	$(CC) src/ls.c $(CFLAGS) -lm -o $(OUT_DIR)/ls

touch: src/touch.c check_bin_dir
	$(CC) src/touch.c $(CLFAGS) -o $(OUT_DIR)/touch

whoami: src/whoami.c check_bin_dir
	$(CC) src/whoami.c $(CFLAGS) -o $(OUT_DIR)/whoami

pwd: src/pwd.c check_bin_dir
	$(CC) src/pwd.c $(CFLAGS) -o $(OUT_DIR)/pwd

base32: src/base32.c check_bin_dir
	$(CC) src/base32.c $(CFLAGS) -o $(OUT_DIR)/base32

mkdir: src/mkdir.c check_bin_dir
	$(CC) src/mkdir.c $(CFLAGS) -o $(OUT_DIR)/mkdir

.PHONY: clean install help

clean:
	rm -rf $(OUT_DIR)/

install: all
	cp $(OUT_DIR)/* $(INSTALL_DIR)
help:
	@`which echo` -e "Targets:"
	@`which echo` -e "\tcat"
	@`which echo` -e "\tyes"
	@`which echo` -e "\ttrue"
	@`which echo` -e "\tfalse"
	@`which echo` -e "\twc"
	@`which echo` -e "\tunlink"
	@`which echo` -e "\trm"
	@`which echo` -e "\tls"
