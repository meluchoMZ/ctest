.PHONY: all test clean

TARGET = libctest.so
TEST_TARGET = ctest_test

SRC = src/ctest_core.c src/ctest_api.c
TEST_SRC = test/assertions/assertions_tests.c test/core/test_suite.c test/core/test_case.c
INCLUDES = include/ctest_api.h src/ctest_core.h

OBJECTS = ctest_core.o ctest_api.o

CC = gcc

CFLAGS = -c -fPIC -Wall -Wextra -Werror -Wpedantic -fsanitize=address -g -Iinclude -Isrc
TEST_CFLAGS = -Wall -Wextra -Werror -Wpedantic -g -Iinclude -Isrc

LDFLAGS = -shared

PREFIX ?= /usr
INCLUDE_DIR ?= $(PREFIX)/include
LIB_DIR ?= $(PREFIX)/lib64

$(TARGET): $(SRC)
	@echo "Compiling $(SRC)..."
	$(CC) $(CFLAGS) $(SRC)
	@echo "Linking shared library $(TARGET)..."
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) 
	rm -f *.o
	@echo "Compilation successful. Library $(TARGET) created"

test: $(TEST_SRC)
	@echo "Compiling $(TEST_TARGET)..."
	$(CC) $(TEST_CFLAGS) -o $(TEST_TARGET) $(SRC) $(TEST_SRC) && ./ctest_test

install: $(TARGET)
	@echo "Installing $(TARGET) to $(LIB_DIR)..."
	@mkdir -p $(LIB_DIR) $(INCLUDE_DIR)
	cp $(TARGET) $(LIB_DIR)/
	@echo "Installing public header to $(INCLUDE_DIR)..."
	cp $(INCLUDES) $(INCLUDE_DIR)/
	@echo "Updating shared library cache..."
	@ldconfig || true
	@echo "Installation complete."

uninstall:
	@echo "Removing installed files..."
	rm -f $(LIB_DIR)/$(TARGET)
	rm -f $(INCLUDE_DIR)/ctest_api.h
	@ldconfig || true
	@echo "Uninstalled successfully."

all: $(TARGET) install

clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET) $(TEST_TARGET)
	@echo "Cleanup complete."

