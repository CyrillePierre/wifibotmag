/*
 * fichier : controler.cpp
 * Fichier d'implémentation de la classe Controler qui permet
 * la connection ?un wifibot, l'envoi de commande aux moteurs
 * et le passage du mode sans asservissement au mode asservi et
 * finalement la deconnection du wifibot.
 */

#include <QtNetwork/QTcpSocket>
#include <iostream>

#include "Receiver_robot.h"
#include "Sender_robot.h"

#include "Controler.h"

using namespace std ;

Controler::Controler( QObject *parent ) {

  /* Creation de la QTcpSocket */
   tcpSocket = new QTcpSocket( parent ) ;  
}


Controler::~Controler( ) {

  /* Deconnexion si connecté */
  if( isConnected( ) )
    disconnect_robot( ) ;
}


/*
 * Connexion au robot 
 */
bool Controler::connect_robot( const QString & hostName, quint16 port ) {

  if( ! isConnected( ) ) {
    /* Connection au wifibot */
    tcpSocket->connectToHost( hostName, port);

    if( tcpSocket->waitForConnected(2000)) {

      /* Demarrage des thread de reception et connection
	 des signaux aux slots du controleur */
      receiver = new ReceiverThread_Cont( tcpSocket, this ) ;
      receiver->start();
   
      /* Démarrage du thread d'emission */
      sender = new SenderThread( tcpSocket ) ;
      sender->start();

      return true ;
    } else
      return false ;
  } else
    return true ;
}

/*
 * Deconnection du robot 
 */
void Controler::disconnect_robot( ) {

  if( isConnected() ) {

	/* Arret des thread d'emission et
       de reception */
	delete receiver ;
    delete sender ;

    /* Deconnection du wifibot */
    tcpSocket->disconnectFromHost( ) ;

  }
}


/*
 * Mode asservi ou non 
 */
void Controler::setControl( bool onOff ) {

  sender->setControl( onOff ) ;

}

/*
 * Modification de la vitesse des moteurs gauche 
 */
void Controler::setSpeedLeft( qint8 speed ) {

  if( isConnected() )
    sender->setSpeedLeft( speed ) ;

}

/*
 * Modification de la vitesse des moteurs droit
 */
void Controler::setSpeedRight( qint8 speed ) {

  if( isConnected() )
    sender->setSpeedRight( speed ) ;

}

/*
 * Mode d'asservissement courant 
 */
bool Controler::getControl( ) {

  if( isConnected() )
    return sender->getControl( ) ;
  else
    return false ;

}

/*
 * Vitesse gauche courante 
 */
qint8 Controler::getSpeedLeft( ) {

  if( isConnected() )
    return sender->getSpeedLeft( ) ;
  else
    return 0 ;

}

/*
 * Vitesse droite courante 
 */
qint8 Controler::getSpeedRight( ) {

  if( isConnected() )
    return sender->getSpeedLeft( ) ;
  else
    return 0 ;
}

/*
 * Retourne l'etat de la connection avec le robot 
 */
bool Controler::isConnected( ) {
  
  return ( tcpSocket->state() == QAbstractSocket::ConnectedState ) ;

}

/*
 * Slots reemetant les signaux recus 
 */
void Controler::changedBatteryValue( quint8 value ) {
  emit batteryValue( value ) ;
}

void Controler::changedFlWheelTicks( quint8 value )  {
  emit flWheelTicks( value ) ;
}

void Controler::changedRlWheelTicks( quint8 value )  {
  emit rlWheelTicks( value ) ;
}

void Controler::changedFrWheelTicks( quint8 value )  {
  emit frWheelTicks( value ) ;
}

void Controler::changedRrWheelTicks( quint8 value )  {
  emit rrWheelTicks( value ) ;
}

void Controler::changedLeftIR( quint8 value )  {
  emit leftIR( value ) ;
}
 
void Controler::changedRightIR( quint8 value )  {
  emit rightIR( value ) ;
}
