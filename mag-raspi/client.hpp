
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstdio>
#include <thread>
#include <unistd.h>

/**
 * Cette classe s'occupe de la gestion de chaque client qui se connecte
 * au serveur.
 */
class Client
{
	int _fd;

public:
	/** @brief Constructeur */
	Client(int fd)			 : _fd(fd)		{}
	Client(Client const & c) : _fd(c._fd)	{}
	Client(Client && c)		 : _fd(c._fd)	{}

	/**
	 * @brief Exécution de la gestion du client
	 *
	 * Cette méthode permet de préciser en dehors de la classe l'action à
	 * faire pour gérer le client. Pour cela il faut lui passer en paramètre
	 * une fonction ou un foncteur de la forme void (*)(Client *).
	 * @param c : le foncteur
	 */
	template <class Callable>
	void asyncRun(Callable && callable) const
	{
		std::thread th(std::forward<Callable>(callable), std::cref(*this));
		th.detach();
	}

	/**
	 * La méthode de comparaison peut être utilisée par le conteneur de client
	 * du serveur.
	 */
	inline bool operator<(Client const & c) const
	{
		return _fd < c._fd;
	}

	/** Fermeture de la socket du client */
	inline void close() const
	{
		::close(_fd);
	}

	/** Envoi de donnée au client. */
	template <typename Buffer>
	inline int write(Buffer && buf, std::size_t size) const
	{
		return ::write(_fd, std::forward<Buffer>(buf), size);
	}

	/** Réception de données du client. */
	template <typename Buffer>
	inline int read(Buffer && buf, std::size_t size) const
	{
		return ::read(_fd, std::forward<Buffer>(buf), size);
	}
};

#endif
