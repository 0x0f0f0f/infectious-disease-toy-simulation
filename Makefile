CC = gcc
CFLAGS = -Wall -pedantic -g -O2

.PHONY: bin

bin:
	$(CC) $(CFLAGS) main.c -o main -lraylib -lGL -lm -ldl -lpthread -lX11

