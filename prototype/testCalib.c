
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/**
 * Cette macro permet à tout ceux qui ont la flemme de réécrire
 * la vérification des fonctions POSIX de n'écrire qu'une seule ligne
 */
#define ERROR_IF(FUNCTION) \
    if (FUNCTION) { perror(#FUNCTION); exit(errno); }


/**
 * Cette fonction permet de créer une socket de type TCP puis la
 * connecter l'host passé en paramètre.
 * @param name le nom de la machine distante
 * @param port le port de connexion
 */
int createTCPSocket(char const *name, char const *port)
{
    int sockfd;
	struct addrinfo hints, *res;

	// Initialisation de l'addrinfo
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family		= AF_UNSPEC;	// IPv4 ou IPv6
	hints.ai_socktype	= SOCK_STREAM;

	// Récupération du sockaddr, création de la socket et connexion
	ERROR_IF(          getaddrinfo(name, port, &hints, &res)          !=  0);
    ERROR_IF((sockfd = socket(res->ai_family, res->ai_socktype, 0))   == -1);
	ERROR_IF(          connect(sockfd, res->ai_addr, res->ai_addrlen) == -1);
 
    return sockfd;
}

/**
 * Il faut 3 arguments (entiers) pour le lancement.
 */
int main(int argc, char ** argv)
{
	unsigned char buf[9];
	int16_t mx ,my, mz, ang;
	int fd = createTCPSocket("192.168.0.44", "1337");

	if (argc == 4) {
		int16_t bx = atoi(argv[1]),
				by = atoi(argv[2]),
				bz = atoi(argv[3]);

		buf[0] = 2;		// id calibration
		buf[1] = (bx & 0xff00) >> 8;
		buf[2] = bx & 0xff;
		buf[3] = (by & 0xff00) >> 8;
		buf[4] = by & 0xff;
		buf[5] = (bz & 0xff00) >> 8;
		buf[6] = bz & 0xff;
		buf[8] = buf[7] = 0;

		write(fd, buf, 9);
	}
	else puts("il faut 3 entier en paramètre");

	while (read(fd, buf, 9)) {
		mx  = buf[2] + (((int16_t) buf[1]) << 8);
		my  = buf[4] + (((int16_t) buf[3]) << 8);
		mz  = buf[6] + (((int16_t) buf[5]) << 8);
		ang = buf[8] + (((int16_t) buf[7]) << 8);

		printf("[%d,%d,%d] : %d\n", mx, my, mz, ang);
	}

	return 0;
}

