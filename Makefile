# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -I ./include -Wall -Wextra -O2

# Source files
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

# Executable name
EXECUTABLE = example_main

# Default target to build the executable
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Rule to build object files from source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(SRC_DIR)/*.o $(EXECUTABLE)

# Rule to recompile everything
rebuild: clean all

# Phony targets (not files)
.PHONY: all clean rebuild
