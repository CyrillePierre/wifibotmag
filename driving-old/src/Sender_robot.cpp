/*
 * Realisation d'une interface de pilotage du robot Wifibot
 *
 * fichier : sender.cpp
 * fichier d'implémentation du thread d'emission
 * de trames vers le robot.
 */

#include "Sender_robot.h"

#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QtNetwork/QTcpSocket>

/*
 * Constructeur
 */
SenderThread::SenderThread( QTcpSocket *inTcpSocket ) {

  tcpSocket = inTcpSocket ;
  sendBuff.reserve( 2 ) ;
  sendBuff.fill( 0 ) ;
  lock = new QMutex( ) ;
  
}


/*
 * Destructeur
 */
SenderThread::~SenderThread( ) {

  stopThread = true ;
  wait() ;
  delete lock ;
  
}


void SenderThread::setControl( bool onOff ) {

  QMutexLocker locker( lock ) ;

  if( onOff ) {
    /* Mise à 1 du bit controle */
    sendBuff[0] = sendBuff[0] | CTRL_BIT ;
    sendBuff[1] = sendBuff[1] | CTRL_BIT ;
  } else {
    /* Mise à 0 */
    sendBuff[0] = sendBuff[0] & ~CTRL_BIT ;
    sendBuff[1] = sendBuff[1] & ~CTRL_BIT ;
  }

}


void SenderThread::setSpeedLeft( qint8 speed ) {

  QMutexLocker locker( lock ) ;
  bool negative = false ;

  if( speed < 0 ) {
    /* Valeur absolue de la vitesse */
    speed = -speed ;
    /* Memorisation de la negativite */
    negative = true ;
  } 

  if( getControl()  && speed <= 40 || ! getControl() && speed <= 60 ) {
    /* Positionnement du bit de direction */
    if( negative )
      /* à 0 (en arriere) */
      sendBuff[0] = sendBuff[0] & ~DIRECTION_BIT ;
    else 
      /* à 1 (en avant) */
      sendBuff[0] = sendBuff[0] | DIRECTION_BIT ;

    /* Mise à 0 de la partie vitesse */
    sendBuff[0] = sendBuff[0] & ~SPEED_BITS ;
    /* Positionnement de la nouvelle vitesse */
    sendBuff[0] = sendBuff[0] | speed ;
  }

}


void SenderThread::setSpeedRight( qint8 speed ) {

  QMutexLocker locker( lock ) ;
  bool negative = false ;

  if( speed < 0 ) {
    /* Valeur absolue de la vitesse */
    speed = -speed ;
    /* Memorisation de la negativite */
    negative = true ;
  } 

  if( getControl()  && speed <= 40 || ! getControl() && speed <= 60 ) {
    /* Positionnement du bit de direction */
    if( negative )
      /* à 0 (en arriere) */
      sendBuff[1] = sendBuff[1] & ~DIRECTION_BIT ;
    else 
      /* à 0 (en arriere) */
      sendBuff[1] = sendBuff[1] | DIRECTION_BIT ;

    /* Mise à 0 de la partie vitesse */
    sendBuff[1] = sendBuff[1] & ~SPEED_BITS ;
    /* Positionnement de la nouvelle vitesse */
    sendBuff[1] = sendBuff[1] | speed ;
  }

}

bool SenderThread::getControl( ) {
 
  /* Test du bit de controle */
  return sendBuff[0] & CTRL_BIT ;
  
}

qint8 SenderThread::getSpeedLeft( ) {
 
  /* En fonction du bit de direction on 
     retourne la vitesse ou son iverse */
  return (sendBuff[0] & DIRECTION_BIT) ? 
    sendBuff[0] & SPEED_BITS : - sendBuff[0] & SPEED_BITS ;
  
}

qint8 SenderThread::getSpeedRight( ) {
 
  /* En fonctoin du bit de direction on
     retourne la vitesse ou son inverse */
  return (sendBuff[1] & DIRECTION_BIT) ? 
    sendBuff[1] & SPEED_BITS : - sendBuff[1] & SPEED_BITS ;
}

/*
 * Routine du thread d'emission
 */
void SenderThread::run()
{

  stopThread = false ;

  while( ! stopThread ) {
    /* Envoi toutes les 60µs (16Hz) */
    lock->lock( ) ;
    tcpSocket->write( sendBuff ) ;
    lock->unlock( ) ; 

    /* Attente de l'envoi des données */
    while( tcpSocket->bytesToWrite() != 0 )
      tcpSocket->waitForBytesWritten( 100 ) ;

    /* Attente de 60ms */
    msleep ( 60 ) ;
  }

}

