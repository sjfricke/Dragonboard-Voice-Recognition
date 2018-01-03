CC = gcc
CFLAGS = -Wall -Werror
SPHINX = -I/usr/local/include -I/usr/local/include/sphinxbase -I/usr/local/include/pocketsphinx -L/usr/local/lib -lpocketsphinx -lsphinxbase -lsphinxad

main: basic add thread


basic:
	$(CC) $(CFLAGS) basic.c $(SPHINX) -o basic

add:
	$(CC) $(CFLAGS) add.c $(SPHINX) -o add

thread:
	$(CC) $(CFLAGS) thread.c $(SPHINX) -lpthread -o thread

clean:
	rm basic
	rm add
	rm thread
