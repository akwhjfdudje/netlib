CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -fPIC
LDFLAGS = -L. -lnet

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
LIB = libnet.a

SRCS = $(SRC_DIR)/addr.c $(SRC_DIR)/client.c $(SRC_DIR)/server.c $(SRC_DIR)/udp.c
OBJS = $(OBJ_DIR)/addr.o $(OBJ_DIR)/client.o $(OBJ_DIR)/server.o $(OBJ_DIR)/udp.o $(OBJ_DIR)/msg.o $(OBJ_DIR)/log.o

all: $(LIB) test

$(LIB): $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC_DIR)/test.c $(LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(LIB) test test.file

.PHONY: all clean
