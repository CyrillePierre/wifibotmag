/*
 *
 * fichier : receiver_robot.h
 * fichier d'entete du module thread de reception robot
 */

#ifndef __RECEIVER_ROBOT_H__
#define __RECEIVER_ROBOT_H__

#include <QThread>

/* forward declarations */
class Controler ;
class QByteArray ;
class QTcpSocket ;

class ReceiverThread_Cont : public QThread 
{
    
 public:
  ReceiverThread_Cont( QTcpSocket *inTcpSocket, Controler *inCtrl ) ;
  ~ReceiverThread_Cont( ) ;
  /* Routine du thread de reception */
  void run() ;
  /* Analyse des données */
  void analyseData( QByteArray received ) ;

 private:
  /* Arret du thread */
  bool stopThread ;

  /* Socket de communication */
  QTcpSocket *tcpSocket ;

  /* Buffer de reception */
  QByteArray receiveBuff ;

   Controler *ctrl ;
} ;


#endif /* __RECEIVER_ROBOT_H__ */
