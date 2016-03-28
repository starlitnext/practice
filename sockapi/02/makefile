.PHYON: all clean
CC=gcc
CFLAGS=-Wall -g
BIN=p2pserver p2pclient
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $> -o $@
clean:
	rm -f *.o $(BIN)