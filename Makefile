INCLUDE := /usr/local/include/eigen3  /usr/include/hdf5/serial

CXX ?= g++
CXXFLAGS ?= -O3 -mavx2 -fopenmp  #-std=c++20
CPPFLAGS ?= $(foreach dir, $(INCLUDE), -I$(dir))
LDFLAGS ?= -L/usr/lib/x86_64-linux-gnu/hdf5/serial
LDLIBS ?= -lhdf5_cpp -lhdf5

SRC_DIR := src
BUILD_DIR := build

# Source files (located in src/)
SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Object files (store in build/)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Default target
all: ex_main

# Link
ex_main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: all clean
clean:
	rm -rf ex_main $(BUILD_DIR)
