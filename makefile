# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++2b -g -I/opt/homebrew/Cellar/cryptopp/8.9.0/include/cryptopp
LDFLAGS = -L/opt/homebrew/Cellar/cryptopp/8.9.0/lib -lcryptopp

# Target executable
TARGET = ./main

# Target for delete
TARGET_DEL = ./main

# Source files
SRC = ./src/main.cpp ./src/floor.cpp src/order.cpp src/auth.cpp src/util.cpp

# Object files
OBJS = $(SRC:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	del $(TARGET_DEL) $(OBJS)