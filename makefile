# Compiler
CXX ?= g++

# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
MAIN_PATH = main

# extensions
SRC_EXT = cpp

#code lists
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)')

# flags #
COMPILE_FLAGS = -O3 -std=c++20 -DNDEBUG -W -Wall -Wshadow
INCLUDES = -I include/ -I /usr/local/include

@PHONY: main
main: dirs $(SOURCES)
	$(CXX) $(COMPILE_FLAGS) $(SOURCES) -o $(BIN_PATH)/$@ $(INCLUDES)

@PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(BIN_PATH)
