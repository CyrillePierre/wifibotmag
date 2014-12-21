
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>                            
#include <termios.h>                             
#include <stdio.h>
#include <pthread.h>

#include "rs232.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Cette fonction permet d'ouvrir et de configurer le port serie
 * pour la communication rs-232
 * @return 0 si une erreur est survenue
 */
int uart_open(char const * file)
{
	struct termios options;
	int res = 0;
	//  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	fd = open(file, O_RDWR); // open device for read&write
	if (fd >= 0)
	{
		res = 1;
		fcntl(fd,F_SETFL,0);
		//    fcntl(fd, F_SETFL, FNDELAY); //ne pas bloquer sur le read
		tcgetattr(fd,&options);
		usleep(10000);
		cfsetospeed(&options,B115200);
		cfsetispeed(&options,B115200);
		options.c_cflag &= ~PARENB; /* Parite   : none */
		options.c_cflag &= ~CSTOPB; /* Stop bit : 1    */
		options.c_cflag &= ~CSIZE;  /* Bits     : 8    */
		options.c_cflag |= CS8;
		options.c_oflag &= ~CRTSCTS;
		// options.c_iflag &= ~(IXON);
		// tcsetattr(fd,TCSANOW,&options);
		options.c_oflag &= ~OPOST;
		options.c_lflag &= ~(ICANON | ECHO | ECHONL|IEXTEN | ISIG);
		// c_cc
		options.c_cc[VMIN] = 1;
		options.c_cc[VTIME] = 4;		// timeout = 0.4s
		// flush!
		//    tcflush(fd,TCIOFLUSH);
		tcsetattr(fd, TCSANOW, &options);
		tcflush(fd,TCIOFLUSH);
		usleep(10000);
	}

	return res;	
}

/**
 * Cette fonction permet d'écrire sur le port serie. L'écriture est
 * thread safe (protégée par un mutex).
 * @param buf le buffer contenant les données à écrire
 * @param count la taille du buffer
 */
ssize_t uart_write(const void *buf, size_t count)
{
	ssize_t ret;

	// Ecriture synchronisee
	pthread_mutex_lock(&mutex);
	ret = write(fd, buf, count);
	pthread_mutex_unlock(&mutex);

	return ret;
}

/**
 * Cette fonction permet de lire une trame sur le port serie
 * @param buf le buffer contenant les données à écrire
 * @param count la taille du buffer
 */
ssize_t uart_read(char *buf, size_t count)
{
	int nb_read, size = 0;


	// Un fois qu'on connait le début de la trame, on lit
	// tant qu'on arrive pas au '/'
	while (size && buf[size - 1] != '/' && size <= count) {
		nb_read = read(fd, buf + size, count - size);

		if (nb_read == -1)
			return -1;

		else size += nb_read;
	}

	return read(fd, buf, count);
}

/**
 * Cette fonction permet de fermer le port serie
 */
void uart_close()
{
	close(fd);
}

