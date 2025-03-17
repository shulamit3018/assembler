CC = gcc
CFLAGS = -g -ansi -pedantic -Wall

SRC = assemble.c error_codes.c language.c main.c macro.c process.c symbols.c utils.c 
OBJ_DIR = obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
TARGET = assembler
HEADERS = $(wildcard *.h)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -f $(OBJ) $(TARGET)
	rmdir $(OBJ_DIR) || exit 0
