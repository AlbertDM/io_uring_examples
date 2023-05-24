CC = gcc
CFLAGS = -Wall -Wextra -g

# Targets
all: tcp_server tcp_client


tcp_server: tcp_server_uring.c
	# $(CC) $(CFLAGS) -o tcp_server tcp_server_uring.c -luring
	$(CC) $(CFLAGS) -o $@ $^ -luring

tcp_client: tcp_client_uring.c
	# $(CC) $(CFLAGS) -o tcp_client tcp_client_uring.c -luring
	$(CC) $(CFLAGS) -o $@ $^ -luring

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f tcp_server tcp_client *.o

