CC = clang
EXEC = main

SRC_DIR = ./src
BUILD_DIR = ./build
INCLUDE_DIR = ./include

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

CFLAGS = -Wall -Werror -Wconversion -std=c11 -g  -I$(INCLUDE_DIR)
LIBS = -ldl

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUIL_DIR)/*.o $(EXEC)

