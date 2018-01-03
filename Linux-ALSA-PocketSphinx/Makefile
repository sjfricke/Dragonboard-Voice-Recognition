CC = gcc
CFLAGS = -Wall -Werror
SPHINX = -I/usr/local/include -I/usr/local/include/sphinxbase -I/usr/local/include/pocketsphinx -L/usr/local/lib -lpocketsphinx -lsphinxbase -lsphinxad

main: basic


basic:
	$(CC) $(CFLAGS) basic.c $(SPHINX) -o basic

clean:
	rm basic
