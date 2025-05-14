SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

CXX := nvcc
CXXFLAGS := -O3 -std=c++17
LDFLAGS := -lcublas -lcusolver

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET := $(BIN_DIR)/hermitian_inverter
.PHONY: all clean

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
    $(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

$(BIN_DIR) $(OBJ_DIR):
    @mkdir -p $@

clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)
