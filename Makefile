CC = gcc

CFLAGS = -Iinclude -Ofast

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/bytevault.so

SOURCES = $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET)

$(TARGET):
	$(CC) src/bytevault.c $(CFLAGS) -shared -fPIC -o obj/bytevault.so