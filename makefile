CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

TARGET = shell
SRC = shell.c
HDR = shell.h
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
