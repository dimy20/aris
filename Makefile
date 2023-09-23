CC = clang++
EXEC = main

SRC_DIR = ./src
BUILD_DIR = ./build
INCLUDE_DIR = ./include

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

CFLAGS = -Wall -Werror -Wconversion -std=c++20 -g  -I$(INCLUDE_DIR)
LIBS = -ldl

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUIL_DIR)/*.o $(EXEC)

