/*
    InterfaceVideo.h
*/

#ifndef __INTERFACEVIDEO_H__
#define __INTERFACEVIDEO_H__

#include <QScrollArea>

#include "Controler.h"

#include <iostream>
#include <fstream>
using namespace std;
//using Eigen::MatrixXd;


class QWidget;
class QLabel;
class QImage;
class ReceiverThread_Video;
class QString;
class Controler;



class InterfaceVideo : public QScrollArea
{
	Q_OBJECT

private:
	
	// Pour l'affichage de la vidéo.
	QLabel *imageLabel;
	// Pixmap sotckant l'image.
	QImage img;
	// Instance du thread de reception video.
	ReceiverThread_Video *receiverThread_video;

	Controler * controle;

	ofstream pFichier;

//	Eigen::MatrixXd Xp;
//	Eigen::MatrixXd X;
//	Eigen::MatrixXd Cx;
//	Eigen::MatrixXd Cy;
//	Eigen::MatrixXd Y;
//	Eigen::MatrixXd K;





public:

	InterfaceVideo(QWidget *parent, Controler * controle);

	// Affichage de l'image.
	void displayImage(void);
	// Information.
	bool isConnected(void);

	~InterfaceVideo()
	{
		pFichier.close();
	}

public slots:

	// Mise à jour de l'image à chaque fois qu'elle est prète.
	void updatePixmap(const QImage &image);
	// Connexion a la camera.
	bool connect_video(const QString &numWifiBot); 
	// Pour arréter la vidéo.
	void stop_video(void);

signals:

   //renvoyer l'image obtenue
   void imageChange(const QPixmap &);

};

#endif
