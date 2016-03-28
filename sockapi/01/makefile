.PHYON: all clean
CC=gcc
CFLAGS=-Wall -g
BIN=server server_mul client
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $> -o $@
clean:
	rm -f *.o $(BIN)