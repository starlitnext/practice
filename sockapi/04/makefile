.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=client server_old server server_fork client_select server_select
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
client:client.o commsocket.o
	$(CC) $(CFLAGS) $^ -o $@
	
server:server.o commsocket.o
	$(CC) $(CFLAGS) $^ -o $@
	
server_fork:server_fork.o commsocket.o
	$(CC) $(CFLAGS) $^ -o $@
	
clean:
	rm -f *.o $(BIN)