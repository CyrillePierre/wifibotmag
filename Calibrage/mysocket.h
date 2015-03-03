#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include <QThread>
#include <QVector2D>

int const SIZE = 9;         // Taille d'une trame
int const MAX_SIZE = 20;    // Taille du buffer

class Log;

/**
 * @brief Socket qui permet de communiquer avec le robot
 */
class MySocket : public QThread
{
    Q_OBJECT

public:
    MySocket();

    int connection(char const *name, char const *port);
    bool isConnected();



    virtual ~MySocket();

public slots:
    void connected();
    void disconnected();

    void setStarted(bool s);
    void writeBiais();

signals:
    void changed(QPointF);
    void connectChange(bool);

private:
    // Méthodes
    void run();
    int createTCPSocket(char const *name, char const *port);

    // Attributs
    bool        _connected;
    bool        _started;

    unsigned char        buff[MAX_SIZE];
    unsigned char        writeBuff[MAX_SIZE];

    QPointF              point;

    int sockfd;
};

#endif // MYSOCKET_H
