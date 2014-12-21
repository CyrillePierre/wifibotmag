
#ifndef RS232_H
#define RS232_H

#include <stdlib.h>

#define DRIVER_FILE "/dev/ttyAMA0"

/** descripteur de fichier du port serie */
int fd;

/** @brief Ouverture du port */
int uart_open();

/** @brief Ecriture sur le port serie */
ssize_t uart_write(const void *buf, size_t count);

/** @brief Lecture sur le port serie */
ssize_t uart_read(char *buf, size_t count);

/** @brief Fermeture du port */
void uart_close();

#endif
