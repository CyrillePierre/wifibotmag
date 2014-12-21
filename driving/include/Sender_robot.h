/*
 * Projet de 2eme année F1 2005-2006
 * Samuel LUCIER - Xiangdong HOU
 * Sous la responsabilit?de M. AUFRERE
 * Realisation d'une interface de pilotage du robot Wifibot
 *
 * fichier : sender_robot.h
 * fichier d'entete du module thread d'envoi au robot
 */

#ifndef __SENDER_ROBOT_H__
#define __SENDER_ROBOT_H__

#include <QThread>

/* forward declarations */
class QByteArray ;
class QMutex ;
class QTcpSocket ;

class SenderThread : public QThread 
{
  Q_OBJECT
    
 public:
  /* Constructeur / Destructeur */
  SenderThread( QTcpSocket *inTcpSocket ) ;
  ~SenderThread( ) ;

  /* Accesseurs */
  void setControl( bool onOff ) ;
  void setSpeedLeft( qint8 speed ) ;
  void setSpeedRight( qint8 speed ) ;
  bool getControl( ) ;
  qint8 getSpeedLeft( ) ;
  qint8 getSpeedRight( ) ;

  /* Routine du thread */
  void run() ;


 private:
  /* Arret du thread */
  bool stopThread ;
  
  /* Socket de communication */
  QTcpSocket *tcpSocket ;

  /* Buffer d'envoi */
  QByteArray sendBuff ;

  /* Mutex protegant le buffer d'envoi */
  QMutex *lock ;

  /* Constantes pour le format des mots de commande */
  static const quint8 CTRL_BIT = 0x80 ;
  static const quint8 DIRECTION_BIT = 0x40 ;
  static const quint8 SPEED_BITS = 0x3F ;

} ;

#endif /* __SENDER_ROBOT_H__ */
