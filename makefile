# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++2b -g

# Target executable
TARGET = ./main

# Target for delete
TARGET_DEL = ./main

# Source files
SRC = ./src/main.cpp ./src/floor.cpp

# Object files
OBJS = $(SRC:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	del $(TARGET_DEL) $(OBJS)