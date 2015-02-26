
#pragma once

#include <cstdio>
#include <utility>
#include <unistd.h>

/**
 * Cette classe permet de communiquer sur le réseau avec des
 * méthode de très bas niveaux.
 * @author Cyrille PIERRE
 */
class Connection
{
	int	 _fd;			///< File descriptor de la socket
	bool _connected;	///< vaut true si le client est connecté.

public:
	/** @brief Constructeur */
	Connection(char const *, char const *);

	/** @brief Destructeur */
	~Connection();

	/** @brief Lecture de données sur la socket */
	template <typename Buffer>
	std::size_t read(Buffer &&, std::size_t) const;

	/** @brief Ecriture de données sur la socket */
	template <typename Buffer>
	std::size_t write(Buffer &&, std::size_t) const;
};

/**
 * Réception d'un buffer envoyé par le serveur.
 * @param buf  : le buffer utilisé pour le stockage des données reçues
 * @param size : la taille du buffer
 * @return le nombre d'octets reçu ou -1 en cas d'erreur
 */
template <typename Buffer>
std::size_t Connection::read(Buffer && buf, std::size_t size) const
{
	return ::read(_fd, std::forward<Buffer>(buf), size);
}

/**
 * Envoi d'un buffer au serveur sur lequel le client est connecté.
 * @param buf  : le buffer contenant les données à envoyer
 * @param size : la taille du buffer
 * @return le nombre d'octets envoyés ou -1 en cas d'erreur
 */
template <typename Buffer>
std::size_t Connection::write(Buffer && buf, std::size_t size) const
{
	return ::write(_fd, std::forward<Buffer>(buf), size);
}
