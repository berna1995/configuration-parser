# Makefile for Configuration Parser library
# Author: Matteo Bernabito

CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -I src/
SOURCE_DIR = src
BUILD_DIR = build

.PHONY: clean check

# Generic rule to create objects file
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@-mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Creation of the test executable
test: $(BUILD_DIR)/test.o $(BUILD_DIR)/conf_parser.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@

# Execute the test, if it doesn't return 0, make will show it.
check: test
	@cd build && ./test

# Delete the build directory entirely.
clean:
	@-rm -rf $(BUILD_DIR)
