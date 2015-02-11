#include "mysocket.h"
#include "log.h"

#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>


/**
 * Cette fonction permet de créer une socket de type TCP puis la
 * connecter l'host passé en paramètre.
 * @param name le nom de la machine distante
 * @param port le port de connexion
 */
int MySocket::connection(const char *name, char const *port)
{
    struct addrinfo hints, *res;

    // Initialisation de l'addrinfo
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family		= AF_UNSPEC;	// IPv4 ou IPv6
    hints.ai_socktype	= SOCK_STREAM;

    // Récupération du sockaddr, création de la socket et connexion
    int err = getaddrinfo(name, port, &hints, &res);
    if(err != 0)
    {
        log->append_error(gai_strerror(err));
        return -1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, 0);
    if(sockfd == -1)
    {
        log->append_error(strerror(errno));
        return -1;

    }

    err = ::connect(sockfd, res->ai_addr, res->ai_addrlen);
    if(err == -1)
    {
        log->append_error(strerror(errno));
        return -1;
    }

    connected();
}

MySocket::MySocket(Log *log)
{
    this->log = log;

    _connected  = false;    // Quand on commence on n'est pas connecté
    _started    = false;    // Au début on n'envoie pas les données
}

/**
 * @brief Slot appelée lorsque la socket est connectée
 */
void MySocket::connected()
{
    log->append("- Connexion réussie");

    _connected = true;

    emit connect_change(_connected);
}

/**
 * @brief Slot appelée lorsque la socket est déconnectée
 */
void MySocket::disconnected()
{
    log->append("Déconnexion");

    _connected = false;

    emit connect_change(_connected);
}

/**
 * Méthode principale qui lie les données venant du capteur et les envoie par signal
 */
void MySocket::run()
{
    while(_started)
    {
        int a;

        a = read(sockfd, buff, SIZE);

        // Si on a lu une trame entière
        if (a == SIZE)
        {
            qDebug() << "start";

            int16_t x  = buff[1] << 8; // On recupère les 8 premiers bits de mx
            x  |= buff[2];             // On récupère les 8 derniers

            int16_t y  = buff[3] << 8; // On recupère les 8 premiers bits de my
            y  |= buff[4];             // On récupère les 8 derniers

            vect.setX(x);
            vect.setY(y);

            emit changed(vect);
        }

    }
}

MySocket::~MySocket()
{
    _connected = false;

    emit connect_change(_connected);
}

/**
 * @brief Permet de savoir si la socket est connectée
 * @return VRAIE si la socket est connectée
 */
bool MySocket::is_connected()
{
    return _connected;
}

/**
 * @brief Permet d'envoyer ou pas les données
 * @param s
 */
void MySocket::set_started()
{
    _started = !_started;
}
