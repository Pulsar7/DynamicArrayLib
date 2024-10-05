# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -I ./include -Wall -Wextra -O0 -g

# Source files
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

# Test files
TEST_DIR = tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(TEST_DIR)/%.o)

# Executable names
TEST_EXECUTABLE = test_main

# Default target to build the executable
all: $(EXECUTABLE)

# Rule to build the main executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Rule to build object files from source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build object files from test files
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build the test executable
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECTS) $(OBJECTS)

# Rule to run the tests
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

# Clean up build artifacts
clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(EXECUTABLE) $(TEST_EXECUTABLE)

# Rule to recompile everything
rebuild: clean all

# Phony targets (not files)
.PHONY: all clean rebuild test
