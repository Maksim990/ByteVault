CXX = g++

CXXFLAGS = -Iinclude -Ofast -Lobj

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/bytevault.so

SOURCES = $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET)

$(TARGET):
	$(CXX) src/bytevault.cpp $(CXXFLAGS) -fPIC -c -o obj/libbytevault.so
