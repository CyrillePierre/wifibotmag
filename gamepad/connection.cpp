
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include "connection.hpp"

/*
 * Cette macro permet à tout ceux qui ont la flemme de réécrire
 * la vérification des fonctions POSIX de n'écrire qu'une seule ligne
 */
#define ERROR_IF(FUNCTION) \
    if (FUNCTION) { perror(#FUNCTION); std::exit(errno); }

/**
 * @param name : addresse du serveur
 * @param port : port de connexion TCP
 */
Connection::Connection(char const * name, char const * port)
{
	struct addrinfo hints, *res;

	// Initialisation de l'addrinfo
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family		= AF_UNSPEC;	// IPv4 ou IPv6
	hints.ai_socktype	= SOCK_STREAM;

	// Récupération du sockaddr, création de la socket et connexion
	ERROR_IF(       getaddrinfo(name, port, &hints, &res)          !=  0);
    ERROR_IF((_fd = socket(res->ai_family, res->ai_socktype, 0))   == -1);
	ERROR_IF(connect(_fd, res->ai_addr, res->ai_addrlen) == -1);
}

Connection::~Connection()
{
	close(_fd);
}	
