SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET := $(BIN_DIR)/hermitian_inverter

all: | $(BIN_DIR) $(OBJ_DIR) $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
    $(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)
