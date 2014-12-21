
#include <unistd.h>
#include <stdio.h>
#include "rs232.h"

#define BUF_SIZE 256

int main(int argc, char **argv)
{
	if (argc == 2) {
		int nbread;
		char buf[BUF_SIZE];
		uart_open(argv[1]);

		while ((nbread = read(fd, buf, BUF_SIZE)))
			write(1, buf, nbread);

		uart_close();
	}
	else printf("Syntaxe : \n\t%s </dev/fichier>\n", argv[0]);

	return 0;
}
