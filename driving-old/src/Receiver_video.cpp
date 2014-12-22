/*
 * Realisation d'une interface de pilotage du robot Wifibot
 *
 * fichier : reveiver.cpp
 * Fichier d'implémentation pour la classe ReceiverThread qui est destinée 
 * a recuperer le flux provenant de la camera, à enlever les entete
 * et les en-queues, puis a creer une QImage a partir des données
 * recues. Cette QImage est ensuite envoyé a l'interface graphique 
 * par un signal avec en parametre la QImage. L'interface graphique 
 * s'occupe alors de l'afficher 
 */

#include <QtNetwork/QtNetwork>
#include <QBuffer>
#include <QImage>
#include <QMutexLocker>
#include <QMutex>
#include <QMessageBox>
#include <QtNetwork/QAbstractSocket>

#include "Receiver_video.h"

#pragma comment (lib, "QtNetworkd4")

ReceiverThread_Video::ReceiverThread_Video(QWidget *parent):QThread(parent)
{
	/* Instanciaton des objets utilisés */
	sock = new QTcpSocket();
	image = new QImage();
	mutex = new QMutex();
	buffer = new QByteArray(); 
	extract = new QByteArray();
	url = new QUrl();
	

	/* Connexion des signaux */
	connect(sock, SIGNAL(readyRead()),this,SLOT(SocketReadyRead()));

	/* Initialisation de la variable booleene gerant l'arret du thread */
	abort = false;
}


ReceiverThread_Video::~ReceiverThread_Video()
{
	/* Desctruction des objets */
	delete url ;
	delete extract ;
	delete buffer ;
	delete mutex ;
	delete image ;
	
	/* Arret du thread */
	abort = true ;
	sock->abort();
	delete sock ;
}


void ReceiverThread_Video::run()
{
  int begin, end;

  /* Tant que le thread ne doit pas etre
     arreté */
  while ( !abort ) 
    {
      // Prise du mutex protegeant le buffer de reception 
      mutex->lock();

		// Test de reception, il ne receptionne pas via le signal d'arrivée de donnée. Je met donc une reception ici 
		try{
		buffer->append( sock->readAll() ) ;
		}
		catch(...)
		{
			QMessageBox::warning(reinterpret_cast<QWidget*>(this),tr("Erreur"),tr("Erreur de lecture dans la lecture réseau"));
		}

      // Recherche de l'en-tete 
      if( buffer->contains( "Content-type: image/jpeg" ) ) {

	// Suppression des données avant l'entete 
	begin = buffer->indexOf( "Content-type: image/jpeg" ) ;
	buffer->remove( 0, begin );

	// Recherche de l'en-queue 
	if( buffer->contains( "--video boundary--" )) {
	  begin = 28 ;
	  // Suppression de l'entete 
	  buffer->remove( 0, begin );
	  // Recherche de l'indice de debut de l'en-queue 
	  end = buffer->indexOf( "--video boundary--", begin ) - 4 ;
	 //  Copie du buffer de reception 
	  *extract = *buffer ;
	  // On conserve seulement l'image dans le buffer extract 
	  extract->truncate( end );
	  // On conserve le reste dans le buffer de reception 
	  buffer->remove(0, end );

	  // Creation de l'image a partir des données binaires
	  //   et emission de l'image si tout s'est bien passé 
	  if( image->loadFromData ( *extract, 0 ) ) 
	    emit receivedImage( *image );
	}
      }
      // Débloquage du mutex 
      mutex->unlock();
      // Petite temporisation 
      usleep(100);
    }
}


void ReceiverThread_Video::httpReadyRead( const QHttpResponseHeader & resp ) 
{
  /* On prend le mutex qui protege le
     buffer de reception pendant toute 
     la durée de cette methode */
  QMutexLocker locker(mutex);

  /* Concatenation des octets recus à la suite
     du buffer de reception */
  buffer->append( sock->readAll() ) ;
}


bool ReceiverThread_Video::startVideo( const QString &adresse ) 
{
	// Connexion à l'adresse de la webcam passé en parametre
  sock->connectToHost(adresse,80);
  // Attente de la conexion à la webcam
	if(sock->waitForConnected(3000) == false)
	{	
		abort = true;
		sock->abort();
		return false;
	}

	// RAZ du buffer de reception 
	buffer->clear();

	// Envoi de la commande déclanchant l'envoi de la vidéo
	QString s = "GET /VIDEO.CGI HTTP/1.0\r\nUser-Agent: \r\nAuthorization: Basic \r\n\r\n";
	sock->write(s.toAscii());

	// Demarrage du thread 
	abort = false ;
	start();
	return true;
}

void ReceiverThread_Video::stopVideo( ) 
{
  /* Arret du thread */
  sock->close();
  abort = true ;
}

bool ReceiverThread_Video::isConnected()
{
	return (sock->state() == QAbstractSocket::ConnectedState);
	return true;
}
