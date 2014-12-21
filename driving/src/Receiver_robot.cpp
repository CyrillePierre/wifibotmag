/* 
 * Realisation d'une interface de pilotage du robot Wifibot
 *
 * fichier : receiver.cpp
 * fichier d'implémentation du thread de reception 
 */

#include <QByteArray>
#include <QtNetwork/QTcpSocket>

#include "Controler.h"

#include "Receiver_robot.h"

/*
 * Constructeur 
 */
ReceiverThread_Cont::ReceiverThread_Cont( QTcpSocket *inTcpSocket , Controler *inCtrl ) {

  tcpSocket = inTcpSocket ;
  receiveBuff.reserve( 7 ) ;
  receiveBuff.fill( 0 ) ;
  ctrl = inCtrl ;
}


/*
 * Destructeur 
 */
ReceiverThread_Cont::~ReceiverThread_Cont( ) {

  stopThread = true ;
  wait() ;

}


/*
 * Routine du thread de reception
 */
void ReceiverThread_Cont::run()
{
	stopThread = false ;

	while( ! stopThread ) 
	{
		if (!tcpSocket->waitForReadyRead())
			tcpSocket->error();
		if (tcpSocket->bytesAvailable() == 7) 
			this->analyseData(tcpSocket->read(7)) ;
	}
}

/*
 * Analyse des données recues 
 */

void ReceiverThread_Cont::analyseData( QByteArray received ) {

  if( received != receiveBuff ) {

    /* Si un des parametres a été modifié 
       on appelle une methode en transmettant
       la nouvelle valeur */
    if( received[0] != receiveBuff[0] ) 
      ctrl->changedBatteryValue( received[0] ) ;

    if( received[1] != receiveBuff[1] ) 
      ctrl->changedFlWheelTicks( received[1] ) ;

    if( received[2] != receiveBuff[2] ) 
      ctrl->changedRlWheelTicks(  received[2] ) ;

    if( received[3] != receiveBuff[3] ) 
      ctrl->changedFrWheelTicks(  received[3] ) ;

    if( received[4] != receiveBuff[4] ) 
      ctrl->changedRrWheelTicks(  received[4] ) ;

    if( received[5] != receiveBuff[5] ) 
      ctrl->changedLeftIR( received[5] ) ; 
    
    if( received[6] != receiveBuff[6] ) 
      ctrl->changedRightIR( received[6] ) ;

    receiveBuff = received ;
  }

}

