#include "mysocket.h"
#include "log.h"
#include "calibrage.h"

#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>

#include <stdio.h>

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
        Log::get()->appendError(gai_strerror(err));
        return -1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, 0);
    if(sockfd == -1)
    {
        Log::get()->appendError(strerror(errno));
        return -1;

    }

    err = ::connect(sockfd, res->ai_addr, res->ai_addrlen);
    if(err == -1)
    {
        Log::get()->appendError(strerror(errno));
        return -1;
    }

    connected();

    return 1;
}

MySocket::MySocket()
{
    _connected  = false;    // Quand on commence on n'est pas connecté
    _started    = false;    // Au début on n'envoie pas les données
}

/**
 * @brief Slot appelée lorsque la socket est connectée
 */
void MySocket::connected()
{
    Log::get()->append("- Connexion réussie");

    _connected = true;

    emit connectChange(_connected);
}

/**
 * @brief Slot appelée lorsque la socket est déconnectée
 */
void MySocket::disconnected()
{
    Log::get()->append("Déconnexion");

    _connected = false;

    emit connectChange(_connected);
}

/**
 * Méthode principale qui lie les données venant du capteur et les envoie par signal
 */
void MySocket::run()
{
    while(1)
    {
        int a;

        a = read(sockfd, buff, SIZE);

        if(_started)
        {
            // Si on a lu une trame entière
            if (a == SIZE)
            {
                int16_t x = buff[2] + (((int16_t) buff[1]) << 8);   // On recupère mx
                int16_t y  = buff[4] + (((int16_t) buff[3]) << 8);  // On recupère my

                point.setX(x);
                point.setY(y);

                emit changed(point);
            }

        }
    }
}

/**
 * @brief permet d'envoyer le biais sur le réseau
 * @param valX : la valeur du biais en x
 * @param valY : la valeur du biais en y
 */
void MySocket::writeBiais()
{
    QPointF biais = Calibrage::getBiais();

    int16_t x = (int) biais.x();
    int16_t y = (int) biais.y();

    // id du biais
    writeBuff[0] = 2;

    // x
    writeBuff[1] = (x & 0xff00) >> 8;
    writeBuff[2] = x & 0xff;

    // y
    writeBuff[3] = (y & 0xff00) >> 8;;
    writeBuff[4] = y & 0xff;

    // z
    writeBuff[5] = 0;
    writeBuff[6] = 0;

    // angle
    writeBuff[7] = 0;
    writeBuff[8] = 0;

    // On envoie le biais
    write(sockfd, writeBuff, SIZE);
}

MySocket::~MySocket()
{
    _connected = false;

    emit connectChange(_connected);
}

/**
 * @brief Permet de savoir si la socket est connectée
 * @return VRAIE si la socket est connectée
 */
bool MySocket::isConnected()
{
    return _connected;
}

/**
 * @brief Permet d'envoyer ou pas les données
 * @param s : VRAIE si on commence la récupération des données, FAUX sinon
 */
void MySocket::setStarted(bool s)
{
    _started = s;
}
