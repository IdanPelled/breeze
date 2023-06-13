# Compiler
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
RELEASE_DIR := release

# Files
EXECUTABLE := $(BIN_DIR)/breeze
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
ZIP_FILE := $(RELEASE_DIR)/breeze.zip

# Default target
all: $(EXECUTABLE) zip

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create a zip file with source files
zip:
	@mkdir -p $(RELEASE_DIR)
	cd $(RELEASE_DIR)
	cd $(SRC_DIR) && zip -r ../$(ZIP_FILE) * && cd ../$(RELEASE_DIR) && zip -u ../$(ZIP_FILE) Makefile build.sh && cd ..

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(RELEASE_DIR)/*.zip

.PHONY: all zip clean
