/*
 *
 * fichier : controler.h
 * fichier d'entete du module controleur fournissant
 * une interface pour piloter le robot 
 */

#ifndef __CONTROLER_H__
#define __CONTROLER_H__

#include <QtNetwork/QTcpSocket>
#include <QWidget>

class SenderThread ;
class ReceiverThread_Cont ;

class Controler : public QObject
{
	Q_OBJECT

public :

	/* Constructeur */
	Controler( QObject *parent = 0 ) ;
	/* Destructeur */
	~Controler( ) ;

	/* Connection / déconnection */
	bool connect_robot( const QString & hostName, quint16 port ) ;
	void disconnect_robot(void) ;

	/* connecté */
	bool isConnected( ) ;

	/* Actions  */
	void setControl( bool onOff ) ;
	void setSpeedLeft( qint8 speed ) ;
	void setSpeedRight( qint8 speed ) ;

	/* Etat du robot */
	bool getControl( ) ;
	qint8 getSpeedLeft( ) ;
	qint8 getSpeedRight( ) ;

	/* methodes transmettant à l'interface graphique
	un changement dans les informations recues */		
	void changedBatteryValue( quint8 ) ;
	void changedFlWheelTicks( quint8 ) ;
	void changedRlWheelTicks( quint8 ) ;
	void changedFrWheelTicks( quint8 ) ;
	void changedRrWheelTicks( quint8 ) ;
	void changedLeftIR( quint8 ) ; 
	void changedRightIR( quint8 ) ;

/* Signaux indiquant la modification des parametres */
signals:

	void batteryValue( int ) ;
	void flWheelTicks( int ) ;
	void rlWheelTicks( int ) ;
	void frWheelTicks( int ) ;
	void rrWheelTicks( int ) ;
	void leftIR( int ) ; 
	void rightIR( int ) ;

private :

	/* Socket de communication */
	QTcpSocket *tcpSocket ;
 
	/* Threads d'emission et de reception */
	SenderThread *sender ;
	ReceiverThread_Cont *receiver ;

};


#endif /* __CONTROLER_H__ */


