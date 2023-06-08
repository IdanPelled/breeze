# This Makefile is used for the Client localy build breeze

# Compiler
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra

# Directories
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

# Files
EXECUTABLE := $(BIN_DIR)/breeze
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Default target
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(RELEASE_DIR)