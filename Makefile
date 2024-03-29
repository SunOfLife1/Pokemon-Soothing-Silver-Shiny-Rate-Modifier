# Taken from the following Stack Overflow answer:
#   https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories/30602701#30602701

SRC_DIR := src
OBJ_DIR := obj

EXE := ShinyModifier.exe
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC := gcc
CFLAGS := -Iinclude -std=c99 -pedantic -Wall -Wextra -Og
LDFLAGS :=

.PHONY: all clean
.SILENT:

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	del /q $(OBJ_DIR) $(EXE)

-include $(OBJ:.o=.d)
