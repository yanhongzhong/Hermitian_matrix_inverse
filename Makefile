# Makefile

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

CXX := nvcc
CXXFLAGS := -O2 -std=c++17
LDFLAGS := -lcublas -lcusolver

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET := $(BIN_DIR)/hermitian_inverter

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

clean:
	 rm -rf $(OBJ_DIR) $(BIN_DIR)
