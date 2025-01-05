# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++2b -g -I/opt/homebrew/Cellar/cryptopp/8.9.0/include/cryptopp -Isrc/util -Isrc/order -Isrc/floor -Isrc/auth
LDFLAGS = -L/opt/homebrew/Cellar/cryptopp/8.9.0/lib -lcryptopp

# Target executable
TARGET = $(BUILD_DIR)/main

# Target for delete
TARGET_DEL = $(BUILD_DIR)/main

# Source files - find all .cpp files in src and subdirectories
SRC = $(shell find src -name "*.cpp")

# Build directory
BUILD_DIR = ./build

# Object files - corresponding .o files in the build directory
OBJS = $(SRC:src/%.cpp=$(BUILD_DIR)/%.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)  # Create the directory for each object file
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET_DEL) $(OBJS)