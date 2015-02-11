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
    MySocket(Log *log);

    int connection(char const *name, char const *port);
    bool is_connected();

    virtual ~MySocket();

public slots:
    void connected();
    void disconnected();

    void set_started();

signals:
    void changed(QVector2D);
    void connect_change(bool);

private:
    void run();
    int createTCPSocket(char const *name, char const *port);

    //QTcpSocket *socket;

    bool        _connected;
    bool        _started;

    char        buff[MAX_SIZE];
    QVector2D   vect;

    Log *log;

    int sockfd;
};

#endif // MYSOCKET_H
