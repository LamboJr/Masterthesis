# Compiler
CXX = g++
CC = gcc

# wx-config to use, may be overridden on make command line.
WX_CONFIG := wx-config

WX_CXXFLAGS := $(shell $(WX_CONFIG) --cxxflags)
WX_LIBS := $(shell $(WX_CONFIG) --libs)

# Flags
CFLAGS := 
CXXFLAGS := $(WX_CXXFLAGS)
LIBS := -lm
LDFLAGS := -lm $(WX_LIBS)

# Directories
C_DIR = C_Files/
CPP_DIR = 
OBJ_DIR = obj

TARGET = main

C_SRCS := $(wildcard $(C_DIR)/*.c)
CPP_SRCS := $(wildcard *.cpp)

C_OBJS := $(C_SRCS:$(C_DIR)/%.c=$(OBJ_DIR)/%.o)
CPP_OBJS := $(CPP_SRCS:%.cpp=$(OBJ_DIR)/%.o)

OBJS := $(C_OBJS) $(CPP_OBJS)

# Default target
all: $(OBJ_DIR) $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile C source files to object files in the object directory
$(C_OBJS): $(OBJ_DIR)/%.o: $(C_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile C++ source files to object files in the object directory
$(CPP_OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET) $(OBJ_DIR)/*.o

# Phony targets
.PHONY: all clean
