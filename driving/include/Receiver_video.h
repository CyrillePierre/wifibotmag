/*
 * Realisation d'une interface de pilotage du robot Wifibot
 *
 * fichier : reveiver_video.h
 * Fichier d'entete pour la classe ReceiverThread qui est destiné 
 * a recuperer le flux provenant de la camera, à enlever les entete
 * et les en-queues, puis a creer une QImage a prtir des données
 * recues. Cette QImage est ensuite envoyé a l'interface graphique 
 * par un signal avec en parametre la QImage. L'interface graphique 
 * s'occupe alors de l'afficher 
 */

#ifndef RECEIVER_VIDEO_H
#define RECEIVER_VIDEO_H

#include <QObject>
#include <QThread>

#include <QMainWindow>

/* Forward declaration des classe Qt utilisés dans notre interface */
class QHttp;
class QMutex;
class QImage;
class QByteArray;
class QUrl;
class QHttpResponseHeader;
class QTcpSocket;

class ReceiverThread_Video : public QThread
{    
  Q_OBJECT

public:
  ReceiverThread_Video(QWidget *parent = 0);
  ~ReceiverThread_Video();
  
	/* Methode demarrant la reception de la vidéo */
	bool startVideo(const QString &adresse) ;
	// Pour savoir si on est connecté.
	bool isConnected(void);
	
	void httpReadyRead(const QHttpResponseHeader & resp) ;

public slots:

  /* Arret de la reception */
  void stopVideo() ;
  /* Verifie si la connection s'est faite
     apres un certain temps (timeout) */
  //void testConnection() ;

signals:

  /* Signal emis a l'interface graphique lorsqu'une nouvelle image a été recue */
  void receivedImage(const QImage &image);
 /* Signal emis a l'interface graphique lorsque l'adresse n'est pas valide */
  //void unknowHost();    

protected:

  /* Routine du thread de reception */
  void run();

private:

  /* Poiteurs sur les objets Qt utilisés dans le receiver.
     Ils sont instanciés dans le constructeur */
  QMutex *mutex;
  QImage *image;
  QByteArray *buffer, *extract;
  QUrl *url;
  QTcpSocket *sock;
  /* Booleen permettant de terminer le thread */
  bool abort;
};

#endif /* RECEIVER_VIDEO_H */

